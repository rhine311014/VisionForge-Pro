#include "algorithm/CodeReadTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/opencv.hpp>
#include <QJsonArray>

namespace VisionForge {
namespace Algorithm {

CodeReadTool::CodeReadTool(QObject* parent)
    : VisionTool(parent)
    , shapeMatchTool_(new ShapeMatchTool(this))
    , codeType_(DataMatrixECC200)
    , timeoutMs_(100)
    , refRow_(0), refCol_(0), refAngle_(0)
    , codeRoiRow1_(0), codeRoiCol1_(0), codeRoiRow2_(0), codeRoiCol2_(0)
{
    setDisplayName("读码工具");
#ifdef _WIN32
    codeHandle_ = HTuple();
    is2DCode_ = true;
#endif
}

CodeReadTool::~CodeReadTool()
{
    clearCodeReader();
}

void CodeReadTool::setCodeType(CodeType type)
{
    if (codeType_ != type) {
        codeType_ = type;
        initCodeReader();
        emit paramChanged();
    }
}

void CodeReadTool::setTimeout(int timeoutMs)
{
    timeoutMs_ = timeoutMs;
    // Update model param if exists
#ifdef _WIN32
    try {
        if (codeHandle_.Length() > 0) {
            if (is2DCode_) {
                SetDataCode2dParam(codeHandle_, "timeout", timeoutMs_);
            } else {
                SetBarCodeParam(codeHandle_, "timeout", timeoutMs_);
            }
        }
    } catch (...) {}
#endif
    emit paramChanged();
}

void CodeReadTool::clearCodeReader()
{
#ifdef _WIN32
    try {
        if (codeHandle_.Length() > 0) {
            if (is2DCode_) {
                ClearDataCode2dModel(codeHandle_);
            } else {
                ClearBarCodeModel(codeHandle_);
            }
        }
    } catch (const HException& e) {
        LOG_ERROR(QString("清除读码模型失败: %1").arg(e.ErrorMessage().Text()));
    }
    codeHandle_ = HTuple();
#endif
}

void CodeReadTool::initCodeReader()
{
    clearCodeReader();
#ifdef _WIN32
    try {
        const char* typeName = "Data Matrix ECC 200";
        is2DCode_ = true;

        switch (codeType_) {
        case DataMatrixECC200: typeName = "Data Matrix ECC 200"; is2DCode_ = true; break;
        case QRCode:           typeName = "QR Code"; is2DCode_ = true; break;
        case PDF417:           typeName = "PDF417"; is2DCode_ = true; break;
        case Aztec:            typeName = "Aztec Code"; is2DCode_ = true; break;
        case Code128:          typeName = "Code 128"; is2DCode_ = false; break;
        case Code39:           typeName = "Code 39"; is2DCode_ = false; break;
        case EAN13:            typeName = "EAN-13"; is2DCode_ = false; break;
        case Code93:           typeName = "Code 93"; is2DCode_ = false; break;
        case Interleaved2of5:  typeName = "2/5 Interleaved"; is2DCode_ = false; break;
        case Codabar:          typeName = "Codabar"; is2DCode_ = false; break;
        default:               typeName = "Data Matrix ECC 200"; is2DCode_ = true; break;
        }

        if (is2DCode_) {
            // CreateDataCode2dModel(CodeType, GenParamNames, GenParamValues, &DataCodeHandle)
            CreateDataCode2dModel(typeName, HTuple(), HTuple(), &codeHandle_);
            SetDataCode2dParam(codeHandle_, "timeout", timeoutMs_);
        } else {
            // CreateBarCodeModel(GenParamNames, GenParamValues, &BarCodeHandle)
            // 创建空参数的条码模型，类型在FindBarCode时指定
            CreateBarCodeModel(HTuple(), HTuple(), &codeHandle_);
        }
        LOG_INFO(QString("读码模型初始化成功: %1").arg(typeName));
    } catch (const HException& e) {
        LOG_ERROR(QString("初始化读码模型失败: %1").arg(QString::fromUtf8(e.ErrorMessage().Text())));
    }
#endif
}

bool CodeReadTool::train(const Base::ImageData::Ptr& image, const QRect& templateRoi, const QRect& codeRoi)
{
    if (!image) return false;

    // 1. 训练形状匹配模型
    if (!shapeMatchTool_->trainModel(image, templateRoi.top(), templateRoi.left(), templateRoi.bottom(), templateRoi.right())) {
        return false;
    }

    // 2. 记录参考位姿
    // 形状匹配工具的参考点通常是ROI的中心
    refRow_ = (templateRoi.top() + templateRoi.bottom()) / 2.0;
    refCol_ = (templateRoi.left() + templateRoi.right()) / 2.0;
    refAngle_ = 0.0;

    // 3. 记录读码ROI
    codeRoiRow1_ = codeRoi.top();
    codeRoiCol1_ = codeRoi.left();
    codeRoiRow2_ = codeRoi.bottom();
    codeRoiCol2_ = codeRoi.right();

    // 4. 初始化读码器
    initCodeReader();

    return true;
}

bool CodeReadTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

#ifdef _WIN32
    try {
        // 1. 执行形状匹配（粗定位）
        ToolResult matchResult;
        if (!shapeMatchTool_->process(input, matchResult)) {
            output.success = false;
            output.errorMessage = "粗定位失败: " + matchResult.errorMessage;
            return false;
        }

        // 检查是否找到匹配
        int numMatches = matchResult.getValue<int>("num_matches", 0);
        if (numMatches == 0) {
            output.success = false;
            output.errorMessage = "未找到定位标记";
            return false;
        }

        // 获取最佳匹配位置
        double curRow = matchResult.getValue<double>("row");
        double curCol = matchResult.getValue<double>("col");
        double curAngle = matchResult.getValue<double>("angle");

        // 2. 计算变换矩阵 (Reference -> Current)
        HTuple homMat2D;
        VectorAngleToRigid(refRow_, refCol_, refAngle_, curRow, curCol, curAngle, &homMat2D);

        // 3. 变换读码ROI
        HRegion codeRegion;
        codeRegion.GenRectangle1(codeRoiRow1_, codeRoiCol1_, codeRoiRow2_, codeRoiCol2_);

        // 使用AffineTransRegion全局函数
        HRegion transformedRegion;
        AffineTransRegion(codeRegion, &transformedRegion, homMat2D, "nearest_neighbor");

        // 4. 读码
        HImage hImg = imageDataToHImage(input);
        HImage reducedImg = hImg.ReduceDomain(transformedRegion);

        HXLDCont symbolXLDs;
        HTuple resultHandles, decodedStrings;

        if (is2DCode_) {
            // FindDataCode2d: 返回HXLDCont, Image, Handle, GenParamNames, GenParamValues, &ResultHandles, &DecodedStrings
            FindDataCode2d(reducedImg, &symbolXLDs, codeHandle_, "stop_after_result_num", 1, &resultHandles, &decodedStrings);
        } else {
            // 对于一维码，我们需要传递类型
            const char* typeName = "auto";
            switch (codeType_) {
            case Code128: typeName = "Code 128"; break;
            case Code39: typeName = "Code 39"; break;
            case EAN13: typeName = "EAN-13"; break;
            case Code93: typeName = "Code 93"; break;
            case Interleaved2of5: typeName = "2/5 Interleaved"; break;
            case Codabar: typeName = "Codabar"; break;
            default: typeName = "auto"; break;
            }
            // FindBarCode: Image, &SymbolRegions, Handle, CodeType, &DecodedDataStrings
            HRegion symbolRegions;
            FindBarCode(reducedImg, &symbolRegions, codeHandle_, typeName, &decodedStrings);
        }

        // 5. 处理结果
        if (decodedStrings.Length() > 0) {
            QString resultStr = QString::fromUtf8(decodedStrings[0].S().Text());
            output.success = true;
            output.setValue("code", resultStr);
            output.setValue("type", is2DCode_ ? "2D" : "1D");
            statusText_ = "解码成功: " + resultStr;

            // 绘制结果
            cv::Mat resultImg = input->mat().clone();

            // 绘制ROI
            HTuple roiRow1, roiCol1, roiRow2, roiCol2;
            SmallestRectangle1(transformedRegion, &roiRow1, &roiCol1, &roiRow2, &roiCol2);
            cv::rectangle(resultImg,
                cv::Point(static_cast<int>(roiCol1.D()), static_cast<int>(roiRow1.D())),
                cv::Point(static_cast<int>(roiCol2.D()), static_cast<int>(roiRow2.D())),
                cv::Scalar(0, 255, 0), 2);

            // 绘制文字
            cv::putText(resultImg, resultStr.toStdString(),
                cv::Point(static_cast<int>(roiCol1.D()), static_cast<int>(roiRow1.D()) - 10),
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

            output.outputImage = Base::ImageMemoryPool::instance().allocate(resultImg.cols, resultImg.rows, resultImg.type());
            resultImg.copyTo(output.outputImage->mat());
        } else {
            output.success = false;
            output.errorMessage = "解码失败";

            // Draw ROI in red
            cv::Mat resultImg = input->mat().clone();
            HTuple roiRow1, roiCol1, roiRow2, roiCol2;
            SmallestRectangle1(transformedRegion, &roiRow1, &roiCol1, &roiRow2, &roiCol2);
            cv::rectangle(resultImg,
                cv::Point(static_cast<int>(roiCol1.D()), static_cast<int>(roiRow1.D())),
                cv::Point(static_cast<int>(roiCol2.D()), static_cast<int>(roiRow2.D())),
                cv::Scalar(0, 0, 255), 2);
            output.outputImage = Base::ImageMemoryPool::instance().allocate(resultImg.cols, resultImg.rows, resultImg.type());
            resultImg.copyTo(output.outputImage->mat());
        }

        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("处理异常: %1").arg(QString::fromUtf8(e.ErrorMessage().Text()));
        return false;
    }
#else
    output.success = false;
    output.errorMessage = "Halcon未启用";
    return false;
#endif
}

QJsonObject CodeReadTool::serializeParams() const
{
    QJsonObject json;
    json["codeType"] = codeType_;
    json["timeout"] = timeoutMs_;
    json["refRow"] = refRow_;
    json["refCol"] = refCol_;
    json["codeRoiRow1"] = codeRoiRow1_;
    json["codeRoiCol1"] = codeRoiCol1_;
    json["codeRoiRow2"] = codeRoiRow2_;
    json["codeRoiCol2"] = codeRoiCol2_;
    
    // Serialize internal shape match tool
    json["shapeMatch"] = shapeMatchTool_->serializeParams();
    
    return json;
}

void CodeReadTool::deserializeParams(const QJsonObject& json)
{
    if (json.contains("codeType")) setCodeType(static_cast<CodeType>(json["codeType"].toInt()));
    if (json.contains("timeout")) setTimeout(json["timeout"].toInt());
    
    refRow_ = json["refRow"].toDouble();
    refCol_ = json["refCol"].toDouble();
    codeRoiRow1_ = json["codeRoiRow1"].toDouble();
    codeRoiCol1_ = json["codeRoiCol1"].toDouble();
    codeRoiRow2_ = json["codeRoiRow2"].toDouble();
    codeRoiCol2_ = json["codeRoiCol2"].toDouble();

    if (json.contains("shapeMatch")) {
        shapeMatchTool_->deserializeParams(json["shapeMatch"].toObject());
    }
}

#ifdef _WIN32
HImage CodeReadTool::imageDataToHImage(const Base::ImageData::Ptr& image)
{
    const cv::Mat& mat = image->mat();
    HImage hImg;
    if (mat.channels() == 1) {
        hImg.GenImage1("byte", mat.cols, mat.rows, (void*)mat.data);
    } else if (mat.channels() == 3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        std::vector<cv::Mat> channels(3);
        cv::split(rgb, channels);
        hImg.GenImage3("byte", mat.cols, mat.rows, (void*)channels[0].data, (void*)channels[1].data, (void*)channels[2].data);
    } else if (mat.channels() == 4) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGRA2RGB);
        std::vector<cv::Mat> channels(3);
        cv::split(rgb, channels);
        hImg.GenImage3("byte", mat.cols, mat.rows, (void*)channels[0].data, (void*)channels[1].data, (void*)channels[2].data);
    }
    return hImg;
}
#endif

}
}
