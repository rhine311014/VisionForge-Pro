/**
 * @file AIDetectionTool.cpp
 * @brief AI检测工具实现
 */

#include "algorithm/AIDetectionTool.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QDir>
#include <QJsonArray>
#include <algorithm>

namespace {
/**
 * @brief 从.dltp项目目录中查找最佳的.hdl模型文件
 * @param dltpPath .dltp文件路径
 * @return 找到的.hdl模型文件路径，如果未找到返回空字符串
 *
 * .dltp是MVTec Deep Learning Tool的项目文件(SQLite数据库)，
 * 训练完成后会在同名目录中生成.hdl模型文件。
 * 优先查找顺序：best_model.hdl > *_opt*.hdl > model.hdl > 任意.hdl
 */
QString findHdlModelFromDltp(const QString& dltpPath)
{
    QFileInfo dltpInfo(dltpPath);
    if (!dltpInfo.exists()) {
        return QString();
    }

    // 查找同名目录 (去掉.dltp扩展名)
    QString baseName = dltpInfo.completeBaseName();
    QString projectDir = dltpInfo.absolutePath() + "/" + baseName;

    QDir dir(projectDir);
    if (!dir.exists()) {
        return QString();
    }

    // 递归查找所有.hdl文件
    QStringList hdlFiles;
    std::function<void(const QDir&)> findHdlRecursive = [&](const QDir& d) {
        // 查找当前目录中的.hdl文件
        QStringList files = d.entryList(QStringList() << "*.hdl", QDir::Files);
        for (const QString& f : files) {
            hdlFiles.append(d.absoluteFilePath(f));
        }
        // 递归查找子目录
        QStringList subDirs = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : subDirs) {
            findHdlRecursive(QDir(d.absoluteFilePath(subDir)));
        }
    };

    findHdlRecursive(dir);

    if (hdlFiles.isEmpty()) {
        return QString();
    }

    // 按优先级查找
    // 1. 优先查找 best_model.hdl
    for (const QString& f : hdlFiles) {
        if (f.endsWith("best_model.hdl", Qt::CaseInsensitive)) {
            return f;
        }
    }

    // 2. 查找优化后的模型 (*_opt*.hdl 或 *TensorRT*.hdl)
    for (const QString& f : hdlFiles) {
        QString fileName = QFileInfo(f).fileName().toLower();
        if (fileName.contains("opt") || fileName.contains("tensorrt") || fileName.contains("gpu")) {
            return f;
        }
    }

    // 3. 查找 model.hdl
    for (const QString& f : hdlFiles) {
        if (f.endsWith("model.hdl", Qt::CaseInsensitive)) {
            return f;
        }
    }

    // 4. 返回第一个找到的.hdl文件
    return hdlFiles.first();
}

} // anonymous namespace

#ifdef USE_HALCON
#include <HalconCpp.h>
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

AIDetectionTool::AIDetectionTool(QObject* parent)
    : VisionTool(parent)
    , modelLoaded_(false)
    , taskType_(ObjectDetection)
    , inferenceEngine_(OpenCVDNN)
    , confidenceThreshold_(0.5f)
    , nmsThreshold_(0.4f)
    , inputSize_(640, 640)
    , useGPU_(false)
#ifdef USE_HALCON
    , halconDLModel_(nullptr)
    , halconModelLoaded_(false)
    , halconDeepOCRHandle_(nullptr)
    , halconDeepOCRLoaded_(false)
#endif
{
    setDisplayName(toolName());

    // 默认类别名称（COCO数据集部分类别）
    classNames_ = {
        "person", "bicycle", "car", "motorcycle", "airplane",
        "bus", "train", "truck", "boat", "traffic light",
        "fire hydrant", "stop sign", "parking meter", "bench", "bird",
        "cat", "dog", "horse", "sheep", "cow"
    };
}

AIDetectionTool::~AIDetectionTool()
{
    unloadModel();
}

bool AIDetectionTool::loadModel(const QString& modelPath, const QString& configPath)
{
    try {
        QFileInfo fileInfo(modelPath);
        if (!fileInfo.exists()) {
            LOG_ERROR(QString("模型文件不存在: %1").arg(modelPath));
            return false;
        }

        QString ext = fileInfo.suffix().toLower();

        // 根据文件扩展名选择加载方式
        if (ext == "onnx") {
            net_ = cv::dnn::readNetFromONNX(modelPath.toStdString());
        }
        else if (ext == "pb" || ext == "pbtxt") {
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromTensorflow(modelPath.toStdString(),
                                                      configPath.toStdString());
            } else {
                net_ = cv::dnn::readNetFromTensorflow(modelPath.toStdString());
            }
        }
        else if (ext == "caffemodel") {
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromCaffe(configPath.toStdString(),
                                                 modelPath.toStdString());
            } else {
                LOG_ERROR("Caffe模型需要配置文件(.prototxt)");
                return false;
            }
        }
        else if (ext == "weights") {
            // Darknet YOLO
            if (!configPath.isEmpty()) {
                net_ = cv::dnn::readNetFromDarknet(configPath.toStdString(),
                                                   modelPath.toStdString());
            } else {
                LOG_ERROR("Darknet模型需要配置文件(.cfg)");
                return false;
            }
        }
        else if (ext == "hdvppmodel" || ext == "model" || ext == "bin") {
            // 海康深度学习模型 (.bin是海康训练后的模型格式)
            inferenceEngine_ = HikvisionDL;
            modelPath_ = modelPath;
            configPath_ = configPath;
            modelLoaded_ = true;
            LOG_INFO(QString("海康模型已设置: %1 (需要海康SDK支持)").arg(modelPath));
            return true;
        }
        else if (ext == "hdl" || ext == "hdict" || ext == "hdev" || ext == "hdm" || ext == "dltp") {
            // Halcon深度学习模型
            // .hdl  - HALCON深度学习模型（默认格式）
            // .hdict - HALCON字典文件（可包含模型数据）
            // .hdev - HALCON脚本文件
            // .hdm  - HALCON深度学习模型（旧格式）
            // .dltp - MVTec Deep Learning Tool项目文件（SQLite数据库，需要从同名目录中查找.hdl模型）
#ifdef USE_HALCON
            // 处理.dltp文件：从同名目录中查找.hdl模型
            QString actualModelPath = modelPath;
            if (ext == "dltp") {
                LOG_INFO(QString(".dltp是MVTec Deep Learning Tool项目文件，尝试从同名目录中查找.hdl模型..."));
                QString hdlPath = findHdlModelFromDltp(modelPath);
                if (hdlPath.isEmpty()) {
                    LOG_ERROR(QString(".dltp项目目录中未找到.hdl模型文件。\n"
                                     "请确保已在Deep Learning Tool中完成训练并导出模型。\n"
                                     "或者直接选择.hdl模型文件进行加载。\n"
                                     "项目文件: %1").arg(modelPath));
                    return false;
                }
                actualModelPath = hdlPath;
                LOG_INFO(QString("从.dltp项目中找到模型: %1").arg(actualModelPath));
            }

            // 根据任务类型或文件名判断是否为OCR模型
            bool isOCRModel = (taskType_ == OCR) ||
                              actualModelPath.toLower().contains("ocr") ||
                              actualModelPath.toLower().contains("text") ||
                              modelPath.toLower().contains("ocr") ||
                              modelPath.toLower().contains("text");

            if (isOCRModel) {
                // 使用Deep OCR加载，支持识别模型（自动创建检测+识别的完整Deep OCR）
                if (loadHalconDeepOCRModel(actualModelPath)) {
                    inferenceEngine_ = HalconDL;
                    modelPath_ = actualModelPath;
                    configPath_ = configPath;
                    modelLoaded_ = true;
                    taskType_ = OCR;
                    LOG_INFO(QString("Halcon Deep OCR模型加载成功: %1").arg(actualModelPath));
                    return true;
                }
                // 如果Deep OCR加载失败，尝试普通DL模型加载
                LOG_WARNING("Deep OCR模型加载失败，尝试普通DL模型加载...");
            }

            if (loadHalconDLModel(actualModelPath)) {
                inferenceEngine_ = HalconDL;
                modelPath_ = actualModelPath;  // 保存实际加载的模型路径
                configPath_ = configPath;
                modelLoaded_ = true;
                LOG_INFO(QString("Halcon DL模型加载成功: %1").arg(actualModelPath));
                return true;
            } else {
                LOG_ERROR(QString("Halcon DL模型加载失败: %1").arg(actualModelPath));
                return false;
            }
#else
            inferenceEngine_ = HalconDL;
            modelPath_ = modelPath;
            configPath_ = configPath;
            modelLoaded_ = true;
            LOG_WARNING(QString("Halcon支持未编译，模型已设置: %1").arg(modelPath));
            return true;
#endif
        }
        else {
            LOG_ERROR(QString("不支持的模型格式: %1\n支持的格式:\n"
                              "- OpenCV DNN: .onnx, .pb, .caffemodel, .weights\n"
                              "- 海康SDK: .bin, .hdvppmodel, .model\n"
                              "- Halcon DL: .hdl, .hdict, .hdm, .dltp").arg(ext));
            return false;
        }

        if (net_.empty()) {
            LOG_ERROR("模型加载失败");
            return false;
        }

        // 设置后端
        if (useGPU_) {
            net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            LOG_INFO("使用CUDA后端");
        } else {
            net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
            net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }

        modelPath_ = modelPath;
        configPath_ = configPath;
        modelLoaded_ = true;

        LOG_INFO(QString("模型加载成功: %1").arg(modelPath));
        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("模型加载异常: %1").arg(e.what()));
        modelLoaded_ = false;
        return false;
    }
}

void AIDetectionTool::unloadModel()
{
    net_ = cv::dnn::Net();

#ifdef USE_HALCON
    unloadHalconDLModel();
#endif

    modelLoaded_ = false;
    modelPath_.clear();
    configPath_.clear();
}

bool AIDetectionTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    if (!modelLoaded_) {
        output.success = false;
        output.errorMessage = "模型未加载";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    cv::Mat src = input->mat();
    detections_.clear();

    try {
        cv::Mat netOutput;

        switch (inferenceEngine_) {
        case OpenCVDNN:
            if (!inferWithOpenCVDNN(src, netOutput)) {
                output.success = false;
                output.errorMessage = "推理失败";
                return false;
            }
            break;

        case ONNXRuntime:
        case TensorRT:
            // 暂时回退到OpenCV DNN
            LOG_WARNING("ONNX Runtime/TensorRT暂未实现，使用OpenCV DNN");
            if (!inferWithOpenCVDNN(src, netOutput)) {
                output.success = false;
                output.errorMessage = "推理失败";
                return false;
            }
            break;

        case HikvisionDL:
            // 海康深度学习推理
            LOG_INFO("使用海康深度学习引擎进行推理");
            output.success = false;
            output.errorMessage = "海康深度学习推理需要MVS SDK支持，请确保已安装海康SDK并配置正确";
            // TODO: 实现海康深度学习推理
            // 1. 使用 MV_DL_CreateSession 创建推理会话
            // 2. 使用 MV_DL_Process 进行推理
            // 3. 使用 MV_DL_GetResult 获取结果
            return false;

        case HalconDL:
#ifdef USE_HALCON
            // Halcon深度学习推理
            LOG_INFO("使用Halcon深度学习引擎进行推理");

            // 如果是OCR任务
            if (taskType_ == OCR) {
                // 优先使用Deep OCR
                if (halconDeepOCRLoaded_) {
                    if (!inferWithHalconDeepOCR(src, output)) {
                        return false;
                    }
                }
                // 如果Deep OCR未加载，尝试使用普通DL模型（文字检测模型）
                else if (halconModelLoaded_) {
                    LOG_INFO("使用普通DL模型进行OCR推理...");
                    if (!inferWithHalconDL(src, output)) {
                        return false;
                    }
                    // 将检测结果转换为OCR结果
                    ocrResults_.clear();
                    for (const auto& det : detections_) {
                        ocrResults_.emplace_back(det.className, det.confidence, det.boundingBox, 0);
                    }
                } else {
                    output.success = false;
                    output.errorMessage = "Halcon模型未加载。OCR任务需要Deep OCR模型(.dltp)或文字检测DL模型(.hdl)";
                    return false;
                }
            } else {
                if (!halconModelLoaded_) {
                    output.success = false;
                    output.errorMessage = "Halcon DL模型未加载";
                    return false;
                }
                if (!inferWithHalconDL(src, output)) {
                    // 错误信息已在inferWithHalconDL中设置
                    return false;
                }
            }
            // Halcon推理直接处理结果，跳过OpenCV后处理
            output.success = true;
            output.executionTime = timer.elapsed();

            // 根据任务类型记录不同的结果
            if (taskType_ == OCR) {
                output.setValue("ocrResultCount", ocrResultCount());

                // 记录OCR结果
                QJsonArray ocrArray;
                for (const auto& ocr : ocrResults_) {
                    QJsonObject ocrObj;
                    ocrObj["text"] = ocr.text;
                    ocrObj["confidence"] = static_cast<double>(ocr.confidence);
                    ocrObj["x"] = ocr.boundingBox.x();
                    ocrObj["y"] = ocr.boundingBox.y();
                    ocrObj["width"] = ocr.boundingBox.width();
                    ocrObj["height"] = ocr.boundingBox.height();
                    ocrObj["angle"] = ocr.angle;
                    ocrArray.append(ocrObj);
                }
                output.data["ocrResults"] = ocrArray;

                // 合并所有识别文本
                QString allText;
                for (const auto& ocr : ocrResults_) {
                    if (!allText.isEmpty()) allText += "\n";
                    allText += ocr.text;
                }
                output.setValue("recognizedText", allText);

                setStatusText(QString("识别到 %1 个文本区域").arg(ocrResultCount()));
                LOG_DEBUG(QString("Halcon Deep OCR完成，识别到 %1 个文本区域，耗时: %2ms")
                         .arg(ocrResultCount())
                         .arg(output.executionTime));
            } else {
                output.setValue("detectionCount", detectionCount());

                // 记录检测结果
                QJsonArray detectionArray;
                for (const auto& det : detections_) {
                    QJsonObject detObj;
                    detObj["classId"] = det.classId;
                    detObj["className"] = det.className;
                    detObj["confidence"] = static_cast<double>(det.confidence);
                    detObj["x"] = det.boundingBox.x();
                    detObj["y"] = det.boundingBox.y();
                    detObj["width"] = det.boundingBox.width();
                    detObj["height"] = det.boundingBox.height();
                    detObj["centerX"] = det.center.x();
                    detObj["centerY"] = det.center.y();
                    detectionArray.append(detObj);
                }
                output.data["detections"] = detectionArray;

                setStatusText(QString("检测到 %1 个目标").arg(detectionCount()));
                LOG_DEBUG(QString("Halcon DL检测完成，检测到 %1 个目标，耗时: %2ms")
                         .arg(detectionCount())
                         .arg(output.executionTime));
            }

            // 绘制结果
            {
                cv::Mat resultImage = drawResults(src);
                output.outputImage = Base::ImageMemoryPool::instance().allocate(
                    resultImage.cols, resultImage.rows, resultImage.type());
                if (output.outputImage) {
                    resultImage.copyTo(output.outputImage->mat());
                }
            }

            setDebugImage(output.outputImage);
            return true;
#else
            LOG_ERROR("Halcon深度学习推理需要编译USE_HALCON支持");
            output.success = false;
            output.errorMessage = "Halcon深度学习推理需要Halcon DL许可证，请确保已配置正确";
            return false;
#endif
        }

        // 后处理
        switch (taskType_) {
        case ObjectDetection:
        case DefectDetection:
            postprocessDetection(netOutput, src.size());
            break;

        case Classification:
            postprocessClassification(netOutput);
            break;

        case Segmentation:
            // TODO: 实现语义分割后处理
            break;

        case OCR:
            // OCR任务使用Halcon Deep OCR处理，不走OpenCV后处理
            LOG_WARNING("OCR任务应使用Halcon Deep OCR引擎");
            break;
        }

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("detectionCount", detectionCount());

        // 记录检测结果
        QJsonArray detectionArray;
        for (const auto& det : detections_) {
            QJsonObject detObj;
            detObj["classId"] = det.classId;
            detObj["className"] = det.className;
            detObj["confidence"] = static_cast<double>(det.confidence);
            detObj["x"] = det.boundingBox.x();
            detObj["y"] = det.boundingBox.y();
            detObj["width"] = det.boundingBox.width();
            detObj["height"] = det.boundingBox.height();
            detObj["centerX"] = det.center.x();
            detObj["centerY"] = det.center.y();
            detectionArray.append(detObj);
        }
        output.data["detections"] = detectionArray;

        // 绘制结果
        cv::Mat resultImage = drawResults(src);
        output.outputImage = Base::ImageMemoryPool::instance().allocate(
            resultImage.cols, resultImage.rows, resultImage.type());
        if (output.outputImage) {
            resultImage.copyTo(output.outputImage->mat());
        }

        setDebugImage(output.outputImage);
        setStatusText(QString("检测到 %1 个目标").arg(detectionCount()));

        LOG_DEBUG(QString("AI检测完成，检测到 %1 个目标，耗时: %2ms")
                 .arg(detectionCount())
                 .arg(output.executionTime));

        return true;

    } catch (const cv::Exception& e) {
        output.success = false;
        output.errorMessage = QString("AI检测失败: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool AIDetectionTool::inferWithOpenCVDNN(const cv::Mat& input, cv::Mat& output)
{
    try {
        // 预处理
        cv::Mat blob = preprocess(input);

        // 设置输入
        net_.setInput(blob);

        // 前向推理
        std::vector<cv::Mat> outputs;
        net_.forward(outputs, net_.getUnconnectedOutLayersNames());

        if (!outputs.empty()) {
            output = outputs[0];
        }

        return true;

    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("OpenCV DNN推理失败: %1").arg(e.what()));
        return false;
    }
}

cv::Mat AIDetectionTool::preprocess(const cv::Mat& input)
{
    // 创建blob
    cv::Mat blob;
    double scaleFactor = 1.0 / 255.0;

    // YOLO类模型的预处理
    cv::dnn::blobFromImage(input, blob, scaleFactor,
                           cv::Size(inputSize_.width(), inputSize_.height()),
                           cv::Scalar(0, 0, 0), true, false);

    return blob;
}

void AIDetectionTool::postprocessDetection(const cv::Mat& output, const cv::Size& originalSize)
{
    detections_.clear();

    // 处理YOLO格式输出
    // 输出格式: [batch, num_detections, 85] 其中85 = 4(box) + 1(confidence) + 80(classes)
    // 或者 [batch, 85, num_detections] 需要转置

    cv::Mat detection = output;

    // 检查输出格式并转置如果需要
    if (detection.dims == 3) {
        // 取第一个batch
        detection = detection.reshape(1, detection.size[1]);
    }

    // 检查是否需要转置 (YOLOv5/v8格式)
    if (detection.cols > detection.rows && detection.rows < 100) {
        cv::transpose(detection, detection);
    }

    float xFactor = static_cast<float>(originalSize.width) / inputSize_.width();
    float yFactor = static_cast<float>(originalSize.height) / inputSize_.height();

    std::vector<DetectionResult> rawDetections;

    for (int i = 0; i < detection.rows; ++i) {
        const float* data = detection.ptr<float>(i);

        // 获取类别置信度
        float confidence = data[4];  // objectness

        if (confidence < confidenceThreshold_) continue;

        // 获取最大类别分数
        int classId = -1;
        float maxClassScore = 0;

        for (int j = 5; j < detection.cols && j - 5 < classNames_.size(); ++j) {
            float classScore = data[j];
            if (classScore > maxClassScore) {
                maxClassScore = classScore;
                classId = j - 5;
            }
        }

        float finalConfidence = confidence * maxClassScore;
        if (finalConfidence < confidenceThreshold_) continue;

        // 解析边界框 (center_x, center_y, width, height)
        float cx = data[0] * xFactor;
        float cy = data[1] * yFactor;
        float w = data[2] * xFactor;
        float h = data[3] * yFactor;

        float x = cx - w / 2;
        float y = cy - h / 2;

        QString className = (classId >= 0 && classId < classNames_.size())
                            ? classNames_[classId] : QString("class_%1").arg(classId);

        rawDetections.emplace_back(classId, className, finalConfidence,
                                   QRectF(x, y, w, h));
    }

    // 应用NMS
    applyNMS(rawDetections);
    detections_ = std::move(rawDetections);
}

void AIDetectionTool::postprocessClassification(const cv::Mat& output)
{
    detections_.clear();

    cv::Mat scores = output.reshape(1, 1);  // 展平为1行

    // 找到最大分数的类别
    cv::Point maxLoc;
    double maxScore;
    cv::minMaxLoc(scores, nullptr, &maxScore, nullptr, &maxLoc);

    int classId = maxLoc.x;
    QString className = (classId >= 0 && classId < classNames_.size())
                        ? classNames_[classId] : QString("class_%1").arg(classId);

    // 对于分类任务，边界框是整个图像
    detections_.emplace_back(classId, className, static_cast<float>(maxScore),
                            QRectF(0, 0, 1, 1));
}

void AIDetectionTool::applyNMS(std::vector<DetectionResult>& detections)
{
    if (detections.size() <= 1) return;

    // 按置信度排序
    std::sort(detections.begin(), detections.end(),
             [](const DetectionResult& a, const DetectionResult& b) {
                 return a.confidence > b.confidence;
             });

    std::vector<bool> suppressed(detections.size(), false);

    for (size_t i = 0; i < detections.size(); ++i) {
        if (suppressed[i]) continue;

        for (size_t j = i + 1; j < detections.size(); ++j) {
            if (suppressed[j]) continue;
            if (detections[i].classId != detections[j].classId) continue;

            // 计算IoU
            QRectF intersection = detections[i].boundingBox.intersected(detections[j].boundingBox);
            double intersectionArea = intersection.width() * intersection.height();
            double unionArea = detections[i].boundingBox.width() * detections[i].boundingBox.height() +
                              detections[j].boundingBox.width() * detections[j].boundingBox.height() -
                              intersectionArea;

            double iou = (unionArea > 0) ? intersectionArea / unionArea : 0;

            if (iou > nmsThreshold_) {
                suppressed[j] = true;
            }
        }
    }

    // 移除被抑制的检测
    auto it = std::remove_if(detections.begin(), detections.end(),
                            [&suppressed, &detections](const DetectionResult& det) {
                                size_t idx = &det - &detections[0];
                                return suppressed[idx];
                            });
    detections.erase(it, detections.end());
}

cv::Mat AIDetectionTool::drawResults(const cv::Mat& input) const
{
    cv::Mat result = input.clone();
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }

    // 颜色列表
    std::vector<cv::Scalar> colors = {
        cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255),
        cv::Scalar(255, 255, 0), cv::Scalar(255, 0, 255), cv::Scalar(0, 255, 255),
        cv::Scalar(128, 0, 255), cv::Scalar(255, 128, 0), cv::Scalar(0, 128, 255)
    };

    // 绘制检测结果
    for (const auto& det : detections_) {
        cv::Scalar color = colors[det.classId % colors.size()];

        // 绘制边界框
        cv::Rect rect(static_cast<int>(det.boundingBox.x()),
                     static_cast<int>(det.boundingBox.y()),
                     static_cast<int>(det.boundingBox.width()),
                     static_cast<int>(det.boundingBox.height()));
        cv::rectangle(result, rect, color, 2);

        // 绘制中心点
        cv::Point center(static_cast<int>(det.center.x()),
                        static_cast<int>(det.center.y()));
        cv::circle(result, center, 4, color, -1);

        // 绘制标签 (类别名: 置信度%)
        QString label = QString("%1: %2%").arg(det.className).arg(det.confidence * 100, 0, 'f', 1);
        int baseline;
        cv::Size textSize = cv::getTextSize(label.toStdString(),
                                           cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

        cv::rectangle(result,
                     cv::Point(rect.x, rect.y - textSize.height - 5),
                     cv::Point(rect.x + textSize.width, rect.y),
                     color, -1);

        cv::putText(result, label.toStdString(),
                   cv::Point(rect.x, rect.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    }

    // 绘制OCR结果
    cv::Scalar ocrColor(0, 255, 0);  // 绿色
    for (size_t i = 0; i < ocrResults_.size(); ++i) {
        const auto& ocr = ocrResults_[i];

        // 绘制文字区域边界框
        cv::Rect rect(static_cast<int>(ocr.boundingBox.x()),
                     static_cast<int>(ocr.boundingBox.y()),
                     static_cast<int>(ocr.boundingBox.width()),
                     static_cast<int>(ocr.boundingBox.height()));
        cv::rectangle(result, rect, ocrColor, 2);

        // 绘制识别文本 (文本 置信度%)
        QString label = QString("%1 (%2%)").arg(ocr.text).arg(ocr.confidence * 100, 0, 'f', 1);
        int baseline;
        cv::Size textSize = cv::getTextSize(label.toStdString(),
                                           cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);

        // 背景
        cv::rectangle(result,
                     cv::Point(rect.x, rect.y - textSize.height - 5),
                     cv::Point(rect.x + textSize.width + 4, rect.y),
                     ocrColor, -1);

        // 文本
        cv::putText(result, label.toStdString(),
                   cv::Point(rect.x + 2, rect.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    }

    return result;
}

QJsonObject AIDetectionTool::serializeParams() const
{
    QJsonObject json;
    json["modelPath"] = modelPath_;
    json["configPath"] = configPath_;
    json["taskType"] = static_cast<int>(taskType_);
    json["inferenceEngine"] = static_cast<int>(inferenceEngine_);
    json["confidenceThreshold"] = static_cast<double>(confidenceThreshold_);
    json["nmsThreshold"] = static_cast<double>(nmsThreshold_);
    json["inputWidth"] = inputSize_.width();
    json["inputHeight"] = inputSize_.height();
    json["useGPU"] = useGPU_;

    QJsonArray classArray;
    for (const auto& name : classNames_) {
        classArray.append(name);
    }
    json["classNames"] = classArray;

    return json;
}

void AIDetectionTool::deserializeParams(const QJsonObject& json)
{
    modelPath_ = json.value("modelPath").toString();
    configPath_ = json.value("configPath").toString();
    taskType_ = static_cast<TaskType>(json.value("taskType").toInt(static_cast<int>(ObjectDetection)));
    inferenceEngine_ = static_cast<InferenceEngine>(json.value("inferenceEngine").toInt(static_cast<int>(OpenCVDNN)));
    confidenceThreshold_ = static_cast<float>(json.value("confidenceThreshold").toDouble(0.5));
    nmsThreshold_ = static_cast<float>(json.value("nmsThreshold").toDouble(0.4));
    inputSize_.setWidth(json.value("inputWidth").toInt(640));
    inputSize_.setHeight(json.value("inputHeight").toInt(640));
    useGPU_ = json.value("useGPU").toBool(false);

    classNames_.clear();
    QJsonArray classArray = json.value("classNames").toArray();
    for (const auto& val : classArray) {
        classNames_.append(val.toString());
    }

    // 如果有模型路径，尝试加载
    if (!modelPath_.isEmpty()) {
        loadModel(modelPath_, configPath_);
    }
}

void AIDetectionTool::setTaskType(TaskType type)
{
    if (taskType_ != type) {
        taskType_ = type;
        emit paramChanged();
    }
}

void AIDetectionTool::setInferenceEngine(InferenceEngine engine)
{
    if (inferenceEngine_ != engine) {
        inferenceEngine_ = engine;
        emit paramChanged();
    }
}

void AIDetectionTool::setConfidenceThreshold(float threshold)
{
    if (qAbs(confidenceThreshold_ - threshold) > 0.001f) {
        confidenceThreshold_ = threshold;
        emit paramChanged();
    }
}

void AIDetectionTool::setNmsThreshold(float threshold)
{
    if (qAbs(nmsThreshold_ - threshold) > 0.001f) {
        nmsThreshold_ = threshold;
        emit paramChanged();
    }
}

void AIDetectionTool::setInputSize(int width, int height)
{
    if (inputSize_.width() != width || inputSize_.height() != height) {
        inputSize_.setWidth(width);
        inputSize_.setHeight(height);
        emit paramChanged();
    }
}

void AIDetectionTool::setClassNames(const QStringList& names)
{
    classNames_ = names;
    emit paramChanged();
}

void AIDetectionTool::setUseGPU(bool use)
{
    if (useGPU_ != use) {
        useGPU_ = use;

        // 如果模型已加载，重新设置后端
        if (modelLoaded_) {
            if (useGPU_) {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
            } else {
                net_.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
                net_.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
            }
        }

        emit paramChanged();
    }
}

#ifdef USE_HALCON
bool AIDetectionTool::loadHalconDLModel(const QString& modelPath)
{
    try {
        // 卸载之前的模型
        unloadHalconDLModel();

        HString hModelPath = modelPath.toStdString().c_str();
        QString ext = QFileInfo(modelPath).suffix().toLower();

        HDlModel* dlModel = nullptr;

        // 根据文件扩展名选择不同的加载方式
        if (ext == "hdict") {
            // .hdict是字典文件，需要用ReadDict读取，然后从中获取模型
            LOG_INFO(QString("读取Halcon字典文件: %1").arg(modelPath));

            HDict dict;
            try {
                dict.ReadDict(hModelPath, HTuple(), HTuple());
                LOG_INFO("字典文件读取成功");
            } catch (const HException& e) {
                LOG_ERROR(QString("读取字典文件失败: %1 (错误码: %2)")
                         .arg(e.ErrorMessage().Text())
                         .arg(e.ErrorCode()));

                // 检查是否是许可证问题
                if (e.ErrorCode() == 5180 || e.ErrorCode() == 5181) {
                    LOG_ERROR("Halcon许可证问题，请检查Halcon DL许可证是否有效");
                }
                return false;
            }

            // 获取并打印字典中所有的键
            try {
                HTuple keys;
                GetDictParam(dict.GetHandle(), "keys", HTuple(), &keys);

                QString keyList;
                for (Hlong i = 0; i < keys.Length(); ++i) {
                    if (i > 0) keyList += ", ";
                    keyList += QString::fromUtf8(keys[i].S().Text());
                }
                LOG_INFO(QString("字典包含的键: %1").arg(keyList));
            } catch (const HException& e) {
                LOG_WARNING(QString("获取字典键列表失败: %1").arg(e.ErrorMessage().Text()));
            }

            // 尝试从字典中获取深度学习模型句柄
            bool modelFound = false;

            // 常见的模型键名
            QStringList modelKeys = {"dl_model_handle", "DLModelHandle", "model", "Model",
                                     "dl_model", "DLModel", "deep_ocr_handle", "DeepOcrHandle",
                                     "DlModelHandle", "dl_model_detection", "dl_model_segmentation"};

            for (const QString& key : modelKeys) {
                try {
                    HTuple modelHandle = dict.GetDictTuple(key.toStdString().c_str());
                    if (modelHandle.Length() > 0) {
                        LOG_INFO(QString("找到键 '%1'，尝试创建模型...").arg(key));
                        dlModel = new HDlModel(modelHandle[0].H());
                        if (dlModel->IsInitialized()) {
                            modelFound = true;
                            LOG_INFO(QString("从字典中成功加载模型，键: %1").arg(key));
                            break;
                        } else {
                            LOG_WARNING(QString("键 '%1' 的值不是有效的DL模型").arg(key));
                            delete dlModel;
                            dlModel = nullptr;
                        }
                    }
                } catch (const HException& e) {
                    // 键不存在或类型不匹配，继续尝试下一个
                    LOG_DEBUG(QString("键 '%1' 不存在或类型不匹配: %2").arg(key).arg(e.ErrorMessage().Text()));
                }
            }

            // 如果没有找到标准模型键
            if (!modelFound) {
                // 某些.hdict文件是预处理参数文件，不包含模型本身
                LOG_WARNING("字典文件不包含直接可用的DL模型句柄");
                LOG_WARNING("此.hdict文件可能是:");
                LOG_WARNING("  1. 预处理参数文件（需要配合.hdl模型文件使用）");
                LOG_WARNING("  2. Deep OCR配置文件（需要使用create_deep_ocr API）");
                LOG_WARNING("  3. 训练数据字典（不是推理用的模型）");
                LOG_WARNING("请确保加载正确的模型文件 (.hdl)");
                return false;
            }

        } else if (ext == "hdl" || ext == "hdm") {
            // .hdl和.hdm是直接的深度学习模型文件
            LOG_INFO(QString("读取Halcon DL模型文件: %1").arg(modelPath));
            dlModel = new HDlModel();
            dlModel->ReadDlModel(hModelPath);

        } else {
            LOG_ERROR(QString("不支持的Halcon文件格式: %1").arg(ext));
            return false;
        }

        // 检查模型是否有效
        if (!dlModel || !dlModel->IsInitialized()) {
            if (dlModel) delete dlModel;
            LOG_ERROR("Halcon DL模型句柄无效");
            return false;
        }

        halconDLModel_ = dlModel;

        // 获取模型信息（可选）
        try {
            HTuple modelType = dlModel->GetDlModelParam("type");
            if (modelType.Length() > 0) {
                LOG_INFO(QString("Halcon DL模型类型: %1").arg(modelType.S().Text()));
            }
        } catch (...) {
            // 某些模型可能不支持这些参数查询，忽略错误
            LOG_DEBUG("获取模型详细信息失败，继续使用模型");
        }

        // 设置运行时参数 - 使用 Halcon 官方示例的方式
        try {
            // 设置批量大小
            dlModel->SetDlModelParam("batch_size", 1);

            // 查询可用的DL设备 - 参考 Halcon 示例代码
            // query_available_dl_devices (['runtime', 'runtime'], ['gpu', 'cpu'], DLDeviceHandles)
            HTuple dlDeviceHandles;
            bool deviceSet = false;

            try {
                // 构建查询参数 - 同时查询 GPU 和 CPU 设备
                HTuple genParamName, genParamValue;

                if (useGPU_) {
                    // 优先 GPU，然后 CPU
                    genParamName.Append(HString("runtime"));
                    genParamName.Append(HString("runtime"));
                    genParamValue.Append(HString("gpu"));
                    genParamValue.Append(HString("cpu"));
                } else {
                    // 只查询 CPU
                    genParamName.Append(HString("runtime"));
                    genParamValue.Append(HString("cpu"));
                }

                QueryAvailableDlDevices(genParamName, genParamValue, &dlDeviceHandles);

                if (dlDeviceHandles.Length() > 0) {
                    LOG_INFO(QString("找到 %1 个DL设备").arg(dlDeviceHandles.Length()));

                    // 获取第一个设备的信息
                    HTuple deviceHandle = dlDeviceHandles[0];
                    HTuple deviceType;
                    try {
                        GetDlDeviceParam(deviceHandle, "type", &deviceType);
                        LOG_INFO(QString("使用设备类型: %1").arg(deviceType.S().Text()));
                    } catch (...) {
                        LOG_DEBUG("无法获取设备类型信息");
                    }

                    // 使用 'device' 参数设置设备句柄 (Halcon 官方推荐方式)
                    dlModel->SetDlModelParam(HString("device"), deviceHandle);
                    deviceSet = true;
                    LOG_INFO("Halcon DL设备设置成功");
                } else {
                    LOG_WARNING("未找到任何可用的DL设备");
                }
            } catch (const HException& e) {
                LOG_WARNING(QString("查询/设置DL设备失败: %1 (错误码: %2)")
                    .arg(e.ErrorMessage().Text()).arg(e.ErrorCode()));
            }

            // 如果设备句柄方式失败，尝试直接设置 runtime 参数
            if (!deviceSet) {
                LOG_INFO("尝试直接设置runtime参数...");
                try {
                    dlModel->SetDlModelParam("runtime", "cpu");
                    deviceSet = true;
                    LOG_INFO("Halcon DL运行时设置成功: cpu");
                } catch (const HException& e) {
                    LOG_WARNING(QString("设置CPU运行时失败: %1").arg(e.ErrorMessage().Text()));
                }
            }

            if (!deviceSet) {
                LOG_WARNING("警告: 无法设置DL运行时设备，推理可能失败");
            }
        } catch (const HException& e) {
            LOG_WARNING(QString("设置模型参数失败: %1 (使用默认值)").arg(e.ErrorMessage().Text()));
        }

        halconModelLoaded_ = true;
        LOG_INFO(QString("Halcon DL模型加载成功: %1").arg(modelPath));
        return true;

    } catch (const HException& e) {
        LOG_ERROR(QString("Halcon DL模型加载异常: %1 (错误码: %2)")
                 .arg(e.ErrorMessage().Text())
                 .arg(e.ErrorCode()));
        halconModelLoaded_ = false;
        return false;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("Halcon DL模型加载异常: %1").arg(e.what()));
        halconModelLoaded_ = false;
        return false;
    }
}

bool AIDetectionTool::inferWithHalconDL(const cv::Mat& input, ToolResult& output)
{
    try {
        detections_.clear();

        HDlModel* dlModel = static_cast<HDlModel*>(halconDLModel_);
        if (!dlModel || !dlModel->IsInitialized()) {
            output.errorMessage = "Halcon DL模型未初始化";
            return false;
        }

        // 0. 确保运行时设备已正确设置 - 推理前再次尝试设置
        try {
            // 查询当前模型的设备设置
            HTuple currentDevice;
            try {
                currentDevice = dlModel->GetDlModelParam("device");
                LOG_DEBUG(QString("当前模型设备句柄: %1").arg(currentDevice.Length() > 0 ? "已设置" : "未设置"));
            } catch (...) {
                LOG_DEBUG("无法获取当前设备设置");
            }

            // 如果没有设备，尝试再次设置
            HTuple dlDeviceHandles;
            HTuple genParamName, genParamValue;
            genParamName.Append(HString("runtime"));
            genParamName.Append(HString("runtime"));
            genParamValue.Append(HString("gpu"));
            genParamValue.Append(HString("cpu"));

            QueryAvailableDlDevices(genParamName, genParamValue, &dlDeviceHandles);

            if (dlDeviceHandles.Length() > 0) {
                HTuple deviceHandle = dlDeviceHandles[0];
                HTuple deviceType;
                try {
                    GetDlDeviceParam(deviceHandle, "type", &deviceType);
                    LOG_INFO(QString("推理使用设备: %1").arg(deviceType.S().Text()));
                } catch (...) {}

                dlModel->SetDlModelParam(HString("device"), deviceHandle);
                LOG_INFO("推理前设备设置成功");
            } else {
                LOG_WARNING("推理前未找到可用DL设备，尝试直接推理...");
            }
        } catch (const HException& e) {
            LOG_WARNING(QString("推理前设备设置失败: %1 (错误码: %2)")
                .arg(e.ErrorMessage().Text()).arg(e.ErrorCode()));
        }

        // 1. 查询模型期望的图像参数
        int modelNumChannels = 3;  // 默认假设3通道
        QString modelImageType = "real";  // 默认假设real类型（大多数DL模型使用）
        int modelImageWidth = 0;
        int modelImageHeight = 0;

        try {
            HTuple numChannels = dlModel->GetDlModelParam("image_num_channels");
            if (numChannels.Length() > 0) {
                modelNumChannels = static_cast<int>(numChannels[0].I());
                LOG_INFO(QString("模型期望图像通道数: %1").arg(modelNumChannels));
            }
        } catch (const HException& e) {
            LOG_WARNING(QString("无法获取模型图像通道数，使用默认值3: %1").arg(e.ErrorMessage().Text()));
        }

        // 查询模型期望的图像类型 (byte, real, uint2等)
        try {
            HTuple imageType = dlModel->GetDlModelParam("image_type");
            if (imageType.Length() > 0) {
                modelImageType = QString::fromUtf8(imageType.S().Text());
                LOG_INFO(QString("模型期望图像类型: %1").arg(modelImageType));
            }
        } catch (const HException& e) {
            LOG_WARNING(QString("无法获取模型图像类型，使用默认值real: %1").arg(e.ErrorMessage().Text()));
        }

        // 查询模型期望的图像尺寸
        try {
            HTuple imgWidth = dlModel->GetDlModelParam("image_width");
            HTuple imgHeight = dlModel->GetDlModelParam("image_height");
            if (imgWidth.Length() > 0 && imgHeight.Length() > 0) {
                modelImageWidth = static_cast<int>(imgWidth[0].I());
                modelImageHeight = static_cast<int>(imgHeight[0].I());
                LOG_INFO(QString("模型期望图像尺寸: %1x%2").arg(modelImageWidth).arg(modelImageHeight));
            }
        } catch (const HException& e) {
            LOG_DEBUG(QString("无法获取模型图像尺寸: %1").arg(e.ErrorMessage().Text()));
        }

        // 2. 预处理输入图像
        cv::Mat resizedInput = input;

        // 2.1 调整图像尺寸（如果模型有尺寸要求）
        if (modelImageWidth > 0 && modelImageHeight > 0) {
            if (input.cols != modelImageWidth || input.rows != modelImageHeight) {
                // 警告：如果模型期望尺寸很小，可能是单行文本识别模型
                if (modelImageWidth < 200 || modelImageHeight < 200) {
                    LOG_WARNING(QString("警告: 模型期望尺寸较小 (%1x%2)，这可能是单行文本识别模型，"
                        "将整图缩放到此尺寸可能导致无法识别。建议使用 Deep OCR (.dltp) 模型进行整图文字识别。")
                        .arg(modelImageWidth).arg(modelImageHeight));
                }
                cv::resize(input, resizedInput, cv::Size(modelImageWidth, modelImageHeight), 0, 0, cv::INTER_LINEAR);
                LOG_INFO(QString("图像尺寸调整: %1x%2 -> %3x%4")
                    .arg(input.cols).arg(input.rows)
                    .arg(modelImageWidth).arg(modelImageHeight));
            }
        }

        // 3. 将OpenCV Mat转换为Halcon HImage，匹配模型期望的通道数和类型
        HImage hImage;

        // 保持图像数据在整个推理过程中有效（声明在外层作用域）
        cv::Mat processedInput;
        cv::Mat finalImage;       // 最终用于生成HImage的图像
        cv::Mat channelR, channelG, channelB;  // 通道数据

        // 根据模型期望的通道数转换输入图像
        if (modelNumChannels == 1) {
            // 模型期望灰度图像
            if (resizedInput.channels() == 3) {
                cv::cvtColor(resizedInput, processedInput, cv::COLOR_BGR2GRAY);
                LOG_INFO("输入图像从BGR转换为灰度以匹配模型要求");
            } else if (resizedInput.channels() == 4) {
                cv::cvtColor(resizedInput, processedInput, cv::COLOR_BGRA2GRAY);
                LOG_INFO("输入图像从BGRA转换为灰度以匹配模型要求");
            } else {
                processedInput = resizedInput.clone();
            }

            // 根据模型期望的图像类型生成Halcon图像
            if (modelImageType == "real" || modelImageType == "float") {
                // 模型期望float类型图像，需要转换并归一化
                processedInput.convertTo(finalImage, CV_32FC1, 1.0 / 255.0);  // 归一化到0-1
                LOG_INFO("图像类型转换: byte -> real (归一化到0-1)");

                // 确保连续性
                if (!finalImage.isContinuous()) {
                    finalImage = finalImage.clone();
                }

                hImage.GenImage1("real", finalImage.cols, finalImage.rows,
                               static_cast<void*>(finalImage.data));
            } else {
                // 默认使用byte类型
                if (!processedInput.isContinuous()) {
                    finalImage = processedInput.clone();
                } else {
                    finalImage = processedInput;
                }
                hImage.GenImage1("byte", finalImage.cols, finalImage.rows,
                               static_cast<void*>(finalImage.data));
            }
        } else if (modelNumChannels == 3) {
            // 模型期望3通道RGB图像
            cv::Mat rgb;
            if (resizedInput.channels() == 1) {
                cv::cvtColor(resizedInput, rgb, cv::COLOR_GRAY2RGB);
                LOG_INFO("输入图像从灰度转换为RGB以匹配模型要求");
            } else if (resizedInput.channels() == 3) {
                cv::cvtColor(resizedInput, rgb, cv::COLOR_BGR2RGB);
            } else if (resizedInput.channels() == 4) {
                cv::cvtColor(resizedInput, rgb, cv::COLOR_BGRA2RGB);
                LOG_INFO("输入图像从BGRA转换为RGB以匹配模型要求");
            } else {
                output.errorMessage = QString("不支持的图像通道数: %1").arg(resizedInput.channels());
                return false;
            }

            if (modelImageType == "real" || modelImageType == "float") {
                // 模型期望float类型3通道图像
                rgb.convertTo(finalImage, CV_32FC3, 1.0 / 255.0);  // 归一化到0-1
                LOG_INFO("图像类型转换: byte -> real (3通道，归一化到0-1)");

                std::vector<cv::Mat> channels;
                cv::split(finalImage, channels);

                channelR = channels[0].clone();
                channelG = channels[1].clone();
                channelB = channels[2].clone();

                hImage.GenImage3("real", finalImage.cols, finalImage.rows,
                               static_cast<void*>(channelR.data),
                               static_cast<void*>(channelG.data),
                               static_cast<void*>(channelB.data));
            } else {
                // 使用byte类型
                finalImage = rgb;
                std::vector<cv::Mat> channels;
                cv::split(finalImage, channels);

                channelR = channels[0].clone();
                channelG = channels[1].clone();
                channelB = channels[2].clone();

                hImage.GenImage3("byte", finalImage.cols, finalImage.rows,
                               static_cast<void*>(channelR.data),
                               static_cast<void*>(channelG.data),
                               static_cast<void*>(channelB.data));
            }
        } else {
            output.errorMessage = QString("不支持的模型期望通道数: %1").arg(modelNumChannels);
            return false;
        }

        // 4. 创建推理输入字典
        HDict dlSample;
        dlSample.SetDictObject(hImage, "image");

        // 5. 执行推理
        // ApplyDlModel需要一个HDictArray作为输入
        HDict dlResult;
        try {
            // 从单个HDict创建HDictArray
            HDict sampleArray[1] = { dlSample };
            HDictArray dlSampleBatch(sampleArray, 1);

            HTuple outputs;  // 空tuple表示获取所有输出

            // 使用HDlModel的成员方法ApplyDlModel，返回HDictArray
            HDictArray dlResultBatch = dlModel->ApplyDlModel(dlSampleBatch, outputs);

            if (dlResultBatch.Length() > 0) {
                const HDict* resultTools = dlResultBatch.Tools();
                dlResult = resultTools[0];
            }
        } catch (const HException& e) {
            output.errorMessage = QString("Halcon DL推理失败: %1 (错误码: %2)")
                .arg(e.ErrorMessage().Text()).arg(e.ErrorCode());
            LOG_ERROR(output.errorMessage);
            return false;
        }

        // 6. 解析结果
        // 检测结果的格式取决于模型类型，这里处理常见的目标检测格式
        try {
            // 尝试获取检测结果 (object detection format)
            HTuple bboxRow1, bboxCol1, bboxRow2, bboxCol2;
            HTuple classIds, classNamesResult, confidences;

            bool hasBbox = false;
            try {
                bboxRow1 = dlResult.GetDictTuple("bbox_row1");
                bboxCol1 = dlResult.GetDictTuple("bbox_col1");
                bboxRow2 = dlResult.GetDictTuple("bbox_row2");
                bboxCol2 = dlResult.GetDictTuple("bbox_col2");
                hasBbox = bboxRow1.Length() > 0;
            } catch (...) {
                hasBbox = false;
            }

            if (hasBbox) {
                // 获取类别和置信度
                try {
                    classIds = dlResult.GetDictTuple("bbox_class_id");
                } catch (...) {}
                try {
                    classNamesResult = dlResult.GetDictTuple("bbox_class_name");
                } catch (...) {}
                try {
                    confidences = dlResult.GetDictTuple("bbox_confidence");
                } catch (...) {}

                // 遍历所有检测结果
                for (Hlong i = 0; i < bboxRow1.Length(); ++i) {
                    float confidence = 1.0f;
                    if (i < confidences.Length()) {
                        confidence = static_cast<float>(confidences[i].D());
                    }

                    // 置信度过滤
                    if (confidence < confidenceThreshold_) continue;

                    int classId = 0;
                    if (i < classIds.Length()) {
                        classId = static_cast<int>(classIds[i].I());
                    }

                    QString className;
                    if (i < classNamesResult.Length()) {
                        className = QString::fromUtf8(classNamesResult[i].S().Text());
                    } else if (classId >= 0 && classId < classNames_.size()) {
                        className = classNames_[classId];
                    } else {
                        className = QString("class_%1").arg(classId);
                    }

                    // 计算边界框
                    double row1 = bboxRow1[i].D();
                    double col1 = bboxCol1[i].D();
                    double row2 = bboxRow2[i].D();
                    double col2 = bboxCol2[i].D();

                    QRectF bbox(col1, row1, col2 - col1, row2 - row1);

                    detections_.emplace_back(classId, className, confidence, bbox);
                }
            }
            // 尝试其他可能的结果格式（分类）
            else {
                HTuple classIdResult, classNameResult, confidenceResult;
                bool hasClass = false;

                try {
                    classIdResult = dlResult.GetDictTuple("class_id");
                    hasClass = classIdResult.Length() > 0;
                } catch (...) {}

                if (hasClass) {
                    int id = static_cast<int>(classIdResult.I());

                    try {
                        classNameResult = dlResult.GetDictTuple("class_name");
                    } catch (...) {}
                    try {
                        confidenceResult = dlResult.GetDictTuple("confidence");
                    } catch (...) {}

                    QString name;
                    if (classNameResult.Length() > 0) {
                        name = QString::fromUtf8(classNameResult.S().Text());
                    } else if (id >= 0 && id < classNames_.size()) {
                        name = classNames_[id];
                    } else {
                        name = QString("class_%1").arg(id);
                    }

                    float conf = confidenceResult.Length() > 0 ? static_cast<float>(confidenceResult.D()) : 1.0f;

                    // 分类结果，边界框为整个图像
                    detections_.emplace_back(id, name, conf, QRectF(0, 0, input.cols, input.rows));
                }
            }
        } catch (const HException& e) {
            LOG_WARNING(QString("解析Halcon DL结果时出错: %1").arg(e.ErrorMessage().Text()));
            // 继续处理，可能部分结果已被解析
        }

        // 应用NMS
        if (taskType_ == ObjectDetection || taskType_ == DefectDetection) {
            applyNMS(detections_);
        }

        LOG_DEBUG(QString("Halcon DL推理完成，检测到 %1 个目标").arg(detections_.size()));
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Halcon DL推理失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("Halcon DL推理异常: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

bool AIDetectionTool::loadHalconDeepOCRModel(const QString& modelPath)
{
    try {
        // 首先清理已有的Deep OCR资源
        if (halconDeepOCRLoaded_ && halconDeepOCRHandle_ != nullptr) {
            try {
                HTuple* handle = static_cast<HTuple*>(halconDeepOCRHandle_);
                HalconCpp::ClearHandle(*handle);
                delete handle;
            } catch (...) {}
            halconDeepOCRHandle_ = nullptr;
            halconDeepOCRLoaded_ = false;
        }

        HString hModelPath = modelPath.toStdString().c_str();
        QString ext = QFileInfo(modelPath).suffix().toLower();

        LOG_INFO(QString("加载Halcon Deep OCR模型: %1").arg(modelPath));

        HTuple* deepOcrHandle = new HTuple();

        // 对于.hdl文件，检查是否为识别模型（小尺寸如120x32）
        // 如果是识别模型，需要使用create_deep_ocr创建完整的Deep OCR
        if (ext == "hdl") {
            try {
                // 先读取DL模型来检查其类型
                HDlModel tempModel;
                tempModel.ReadDlModel(hModelPath);

                int modelWidth = 0, modelHeight = 0;
                try {
                    HTuple imgWidth = tempModel.GetDlModelParam("image_width");
                    HTuple imgHeight = tempModel.GetDlModelParam("image_height");
                    if (imgWidth.Length() > 0 && imgHeight.Length() > 0) {
                        modelWidth = static_cast<int>(imgWidth[0].I());
                        modelHeight = static_cast<int>(imgHeight[0].I());
                    }
                } catch (...) {}

                // 如果模型尺寸很小（宽度<300且高度<100），说明是识别模型
                bool isRecognitionModel = (modelWidth > 0 && modelHeight > 0 &&
                                          modelWidth < 300 && modelHeight < 100);

                if (isRecognitionModel) {
                    LOG_INFO(QString("检测到识别模型 (尺寸: %1x%2)，将创建完整的Deep OCR...")
                            .arg(modelWidth).arg(modelHeight));

                    // 使用create_deep_ocr创建完整的Deep OCR
                    // 参数: GenParamName, GenParamValue 设置为 "mode", "auto" 来启用检测+识别
                    try {
                        // 首先创建一个默认的Deep OCR（带预训练检测和识别模型）
                        HTuple paramNames, paramValues;
                        paramNames.Append(HString("mode"));
                        paramValues.Append(HString("auto"));
                        HalconCpp::CreateDeepOcr(paramNames, paramValues, deepOcrHandle);
                        LOG_INFO("Deep OCR创建成功（使用预训练检测模型）");

                        // 查询Deep OCR的参数，确认检测模型已加载
                        try {
                            HTuple detectionModel;
                            HalconCpp::GetDeepOcrParam(*deepOcrHandle, "detection_model", &detectionModel);
                            LOG_INFO(QString("Deep OCR检测模型句柄: %1").arg(detectionModel.Length() > 0 ? "已加载" : "未加载"));
                        } catch (const HException& e) {
                            LOG_WARNING(QString("查询检测模型失败: %1").arg(e.ErrorMessage().Text()));
                        }

                        // 查询可用的Deep OCR参数
                        try {
                            HTuple paramList;
                            HalconCpp::GetDeepOcrParam(*deepOcrHandle, "param_names", &paramList);
                            QString paramStr;
                            for (Hlong i = 0; i < paramList.Length() && i < 20; ++i) {
                                if (i > 0) paramStr += ", ";
                                paramStr += QString::fromUtf8(paramList[i].S().Text());
                            }
                            LOG_DEBUG(QString("Deep OCR可用参数: %1%2")
                                     .arg(paramStr)
                                     .arg(paramList.Length() > 20 ? "..." : ""));
                        } catch (...) {}

                        // 设置用户训练的识别模型
                        // 先读取用户的识别模型
                        HTuple recognitionModelHandle;
                        HalconCpp::ReadDlModel(hModelPath, &recognitionModelHandle);
                        LOG_INFO("用户识别模型读取成功");

                        // 设置DL设备（GPU/CPU）
                        try {
                            HTuple dlDeviceHandles;
                            HTuple gpuName, gpuValue;
                            gpuName.Append(HString("runtime"));
                            gpuValue.Append(HString("gpu"));
                            QueryAvailableDlDevices(gpuName, gpuValue, &dlDeviceHandles);

                            if (dlDeviceHandles.Length() > 0) {
                                SetDlModelParam(recognitionModelHandle, "device", dlDeviceHandles[0]);
                                LOG_INFO("识别模型已设置GPU设备");
                            } else {
                                HTuple cpuName, cpuValue;
                                cpuName.Append(HString("runtime"));
                                cpuValue.Append(HString("cpu"));
                                QueryAvailableDlDevices(cpuName, cpuValue, &dlDeviceHandles);
                                if (dlDeviceHandles.Length() > 0) {
                                    SetDlModelParam(recognitionModelHandle, "device", dlDeviceHandles[0]);
                                    LOG_INFO("识别模型已设置CPU设备");
                                }
                            }
                        } catch (const HException& e) {
                            LOG_WARNING(QString("设置识别模型设备失败: %1").arg(e.ErrorMessage().Text()));
                        }

                        // 设置识别模型
                        HalconCpp::SetDeepOcrParam(*deepOcrHandle, "recognition_model", recognitionModelHandle);
                        LOG_INFO("已设置用户训练的识别模型");

                        // 再次验证设置
                        try {
                            HTuple recModel;
                            HalconCpp::GetDeepOcrParam(*deepOcrHandle, "recognition_model", &recModel);
                            LOG_INFO(QString("验证识别模型设置: %1").arg(recModel.Length() > 0 ? "成功" : "失败"));
                        } catch (...) {}

                    } catch (const HException& e) {
                        delete deepOcrHandle;
                        LOG_ERROR(QString("创建Deep OCR失败: %1 (错误码: %2)")
                                 .arg(e.ErrorMessage().Text())
                                 .arg(e.ErrorCode()));
                        return false;
                    }
                } else {
                    // 模型尺寸较大，可能是检测模型或完整模型，尝试直接作为Deep OCR使用
                    LOG_INFO(QString("检测到较大尺寸模型 (%1x%2)，尝试直接使用...")
                            .arg(modelWidth).arg(modelHeight));
                    try {
                        HalconCpp::ReadDeepOcr(hModelPath, deepOcrHandle);
                        LOG_INFO("模型作为Deep OCR加载成功");
                    } catch (const HException& e) {
                        // 如果无法作为Deep OCR加载，尝试创建Deep OCR并设置为检测模型
                        LOG_WARNING(QString("无法直接作为Deep OCR加载: %1，尝试作为检测模型使用...").arg(e.ErrorMessage().Text()));
                        try {
                            HTuple paramNames, paramValues;
                            paramNames.Append(HString("mode"));
                            paramValues.Append(HString("auto"));
                            HalconCpp::CreateDeepOcr(paramNames, paramValues, deepOcrHandle);

                            HTuple detectionModelHandle;
                            HalconCpp::ReadDlModel(hModelPath, &detectionModelHandle);
                            HalconCpp::SetDeepOcrParam(*deepOcrHandle, "detection_model", detectionModelHandle);
                            LOG_INFO("已设置用户训练的检测模型");
                        } catch (const HException& e2) {
                            delete deepOcrHandle;
                            LOG_ERROR(QString("作为检测模型设置失败: %1").arg(e2.ErrorMessage().Text()));
                            return false;
                        }
                    }
                }
            } catch (const HException& e) {
                delete deepOcrHandle;
                LOG_ERROR(QString("读取DL模型失败: %1 (错误码: %2)")
                         .arg(e.ErrorMessage().Text())
                         .arg(e.ErrorCode()));
                return false;
            }
        } else if (ext == "hdo") {
            // .hdo是标准的Deep OCR模型文件
            try {
                HalconCpp::ReadDeepOcr(hModelPath, deepOcrHandle);
                LOG_INFO("Deep OCR模型读取成功 (.hdo)");
            } catch (const HException& e) {
                delete deepOcrHandle;
                LOG_ERROR(QString("读取Deep OCR模型失败: %1 (错误码: %2)")
                         .arg(e.ErrorMessage().Text())
                         .arg(e.ErrorCode()));
                return false;
            }
        } else if (ext == "hdict") {
            // 尝试从字典文件创建Deep OCR
            try {
                HDict dict;
                dict.ReadDict(hModelPath, HTuple(), HTuple());

                // 尝试从字典获取Deep OCR句柄
                QStringList ocrKeys = {"deep_ocr_handle", "DeepOcrHandle", "deep_ocr", "DeepOcr"};
                bool found = false;

                for (const QString& key : ocrKeys) {
                    try {
                        HTuple handle = dict.GetDictTuple(key.toStdString().c_str());
                        if (handle.Length() > 0) {
                            *deepOcrHandle = handle[0];
                            found = true;
                            LOG_INFO(QString("从字典找到Deep OCR句柄，键: %1").arg(key));
                            break;
                        }
                    } catch (...) {}
                }

                if (!found) {
                    delete deepOcrHandle;
                    LOG_ERROR("字典中未找到Deep OCR句柄，请使用.hdl或.hdo格式的模型文件");
                    return false;
                }
            } catch (const HException& e) {
                delete deepOcrHandle;
                LOG_ERROR(QString("从字典加载Deep OCR失败: %1").arg(e.ErrorMessage().Text()));
                return false;
            }
        } else {
            // 其他格式：尝试直接读取为Deep OCR
            try {
                HalconCpp::ReadDeepOcr(hModelPath, deepOcrHandle);
                LOG_INFO("Deep OCR模型读取成功");
            } catch (const HException& e) {
                delete deepOcrHandle;
                LOG_ERROR(QString("加载Deep OCR模型失败: %1").arg(e.ErrorMessage().Text()));
                return false;
            }
        }

        halconDeepOCRHandle_ = deepOcrHandle;
        halconDeepOCRLoaded_ = true;

        LOG_INFO(QString("Halcon Deep OCR模型加载成功: %1").arg(modelPath));
        return true;

    } catch (const HException& e) {
        LOG_ERROR(QString("Halcon Deep OCR加载异常: %1 (错误码: %2)")
                 .arg(e.ErrorMessage().Text())
                 .arg(e.ErrorCode()));
        halconDeepOCRLoaded_ = false;
        return false;
    } catch (const std::exception& e) {
        LOG_ERROR(QString("Halcon Deep OCR加载异常: %1").arg(e.what()));
        halconDeepOCRLoaded_ = false;
        return false;
    }
}

bool AIDetectionTool::inferWithHalconDeepOCR(const cv::Mat& input, ToolResult& output)
{
    try {
        ocrResults_.clear();

        HTuple* deepOcrHandle = static_cast<HTuple*>(halconDeepOCRHandle_);
        if (!deepOcrHandle || deepOcrHandle->Length() == 0) {
            output.errorMessage = "Halcon Deep OCR句柄无效";
            return false;
        }

        LOG_DEBUG(QString("输入图像: %1x%2, 通道数: %3")
                 .arg(input.cols).arg(input.rows).arg(input.channels()));

        // 1. 将OpenCV Mat转换为Halcon HImage
        // 注意：需要确保图像数据在整个推理过程中有效
        HImage hImage;
        cv::Mat continuous;  // 声明在外层作用域，确保生命周期
        cv::Mat rgb;
        cv::Mat channelR, channelG, channelB;

        if (input.channels() == 1) {
            if (!input.isContinuous()) {
                continuous = input.clone();
            } else {
                continuous = input;
            }
            hImage.GenImage1("byte", continuous.cols, continuous.rows,
                           static_cast<void*>(continuous.data));
            LOG_DEBUG("创建灰度HImage成功");
        } else if (input.channels() == 3) {
            cv::cvtColor(input, rgb, cv::COLOR_BGR2RGB);

            std::vector<cv::Mat> channels;
            cv::split(rgb, channels);

            channelR = channels[0].clone();
            channelG = channels[1].clone();
            channelB = channels[2].clone();

            hImage.GenImage3("byte", input.cols, input.rows,
                           static_cast<void*>(channelR.data),
                           static_cast<void*>(channelG.data),
                           static_cast<void*>(channelB.data));
            LOG_DEBUG("创建RGB HImage成功");
        } else {
            output.errorMessage = QString("不支持的图像通道数: %1").arg(input.channels());
            return false;
        }

        // 验证HImage创建成功
        HTuple width, height, channels_count;
        try {
            hImage.GetImageSize(&width, &height);
            channels_count = hImage.CountChannels();
            LOG_DEBUG(QString("HImage验证: %1x%2, 通道数: %3")
                     .arg(width[0].I()).arg(height[0].I()).arg(channels_count[0].I()));
        } catch (const HException& e) {
            output.errorMessage = QString("HImage验证失败: %1").arg(e.ErrorMessage().Text());
            return false;
        }

        // 2. 执行Deep OCR推理
        HTuple resultHandle;
        try {
            LOG_DEBUG("开始执行 ApplyDeepOcr...");
            HalconCpp::ApplyDeepOcr(hImage, *deepOcrHandle, "auto", &resultHandle);
            LOG_DEBUG(QString("ApplyDeepOcr 完成，结果句柄长度: %1").arg(resultHandle.Length()));
        } catch (const HException& e) {
            output.errorMessage = QString("Deep OCR推理失败: %1 (错误码: %2)")
                .arg(e.ErrorMessage().Text()).arg(e.ErrorCode());
            LOG_ERROR(output.errorMessage);
            return false;
        }

        // 3. 解析Deep OCR结果
        // resultHandle 是一个包含字典句柄的 HTuple，需要使用 GetDictTuple 直接操作
        try {
            if (resultHandle.Length() == 0) {
                LOG_WARNING("ApplyDeepOcr 返回空结果");
                return true;
            }

            // 获取字典句柄
            HTuple dictHandle = resultHandle[0];
            LOG_DEBUG(QString("结果字典句柄类型: %1").arg(dictHandle.Type()));

            // 打印字典中的所有键，用于调试
            try {
                HTuple keys;
                HalconCpp::GetDictParam(dictHandle, "keys", HTuple(), &keys);
                QString keyList;
                for (Hlong i = 0; i < keys.Length(); ++i) {
                    if (i > 0) keyList += ", ";
                    keyList += QString::fromUtf8(keys[i].S().Text());
                }
                LOG_DEBUG(QString("Deep OCR结果字典包含的键: %1").arg(keyList.isEmpty() ? "(空)" : keyList));
            } catch (const HException& e) {
                LOG_DEBUG(QString("无法获取结果字典的键: %1 (错误码: %2)")
                         .arg(e.ErrorMessage().Text()).arg(e.ErrorCode()));
            }

            // 获取识别的单词 - 注意：键名是 "words"（复数）
            // words 可能是字符串数组，也可能是嵌套字典句柄数组
            HTuple words;
            try {
                HalconCpp::GetDictTuple(dictHandle, "words", &words);
                LOG_DEBUG(QString("获取到 'words' 键，长度: %1，类型: %2").arg(words.Length()).arg(words.Type()));
            } catch (const HException& e) {
                LOG_DEBUG(QString("获取 'words' 键失败: %1 (错误码: %2)")
                         .arg(e.ErrorMessage().Text()).arg(e.ErrorCode()));
                // 如果 "words" 失败，尝试 "word"
                try {
                    HalconCpp::GetDictTuple(dictHandle, "word", &words);
                    LOG_DEBUG(QString("获取到 'word' 键，长度: %1").arg(words.Length()));
                } catch (...) {}
            }

            if (words.Length() > 0) {
                LOG_INFO(QString("Deep OCR识别到 %1 个文字").arg(words.Length()));

                // 获取边界框 - 尝试多种方式
                HObject wordBoxesObj;
                HTuple row1Array, col1Array, row2Array, col2Array;
                bool hasBoxes = false;

                // 首先尝试作为HObject获取
                try {
                    HalconCpp::GetDictObject(&wordBoxesObj, dictHandle, "word_boxes_on_image");
                    HalconCpp::SmallestRectangle1(wordBoxesObj, &row1Array, &col1Array, &row2Array, &col2Array);
                    hasBoxes = (row1Array.Length() > 0);
                    LOG_DEBUG(QString("获取到边界框区域(HObject)，数量: %1").arg(row1Array.Length()));
                } catch (const HException& e) {
                    LOG_DEBUG(QString("GetDictObject获取边界框失败: %1").arg(e.ErrorMessage().Text()));
                }

                // 如果HObject方式失败，尝试作为Tuple获取
                HTuple wordBoxesTuple;
                if (!hasBoxes) {
                    try {
                        HalconCpp::GetDictTuple(dictHandle, "word_boxes_on_image", &wordBoxesTuple);
                        LOG_DEBUG(QString("获取到边界框Tuple，长度: %1，类型: %2")
                                 .arg(wordBoxesTuple.Length()).arg(wordBoxesTuple.Type()));
                    } catch (...) {}
                }

                // 遍历每个单词
                for (Hlong i = 0; i < words.Length(); ++i) {
                    QString text;
                    float confidence = 1.0f;
                    QRectF bbox(0, 0, input.cols, input.rows);

                    // 检查 words[i] 的类型
                    int wordType = words[i].Type();
                    LOG_DEBUG(QString("words[%1] 类型: %2").arg(i).arg(wordType));

                    if (wordType == 2) {  // H_TYPE_STRING - 直接是字符串
                        try {
                            text = QString::fromUtf8(words[i].S().Text());
                            LOG_DEBUG(QString("文字 %1 (字符串): '%2'").arg(i).arg(text));
                        } catch (...) {
                            LOG_DEBUG(QString("获取第 %1 个单词字符串失败").arg(i));
                            continue;
                        }
                    } else if (wordType == 16) {  // H_TYPE_HANDLE - 可能是嵌套字典
                        // words[i] 是一个字典句柄，需要从中提取文本
                        HTuple wordDict = words[i];
                        LOG_DEBUG(QString("words[%1] 是句柄，尝试作为字典解析...").arg(i));

                        // 尝试获取字典中的键
                        try {
                            HTuple wordKeys;
                            HalconCpp::GetDictParam(wordDict, "keys", HTuple(), &wordKeys);
                            QString keyList;
                            for (Hlong k = 0; k < wordKeys.Length(); ++k) {
                                if (k > 0) keyList += ", ";
                                keyList += QString::fromUtf8(wordKeys[k].S().Text());
                            }
                            LOG_DEBUG(QString("单词字典[%1]的键: %2").arg(i).arg(keyList));

                            // 尝试获取文本 - 键名是 "word"
                            // word 可能是单个字符串，也可能是字符串数组（每行一个字符串）
                            HTuple textValues;
                            HTuple rowVals, colVals, len1Vals, len2Vals;
                            bool hasMultipleLines = false;

                            try {
                                HalconCpp::GetDictTuple(wordDict, "word", &textValues);
                                LOG_DEBUG(QString("'word' 键值长度: %1，类型: %2")
                                         .arg(textValues.Length())
                                         .arg(textValues.Length() > 0 ? textValues[0].Type() : -1));

                                // 获取边界框参数（可能也是数组）
                                try {
                                    HalconCpp::GetDictTuple(wordDict, "row", &rowVals);
                                    HalconCpp::GetDictTuple(wordDict, "col", &colVals);
                                    HalconCpp::GetDictTuple(wordDict, "length1", &len1Vals);
                                    HalconCpp::GetDictTuple(wordDict, "length2", &len2Vals);
                                } catch (...) {}

                                // word 是字符串数组，每个元素是一行文字
                                if (textValues.Length() > 1) {
                                    hasMultipleLines = true;
                                    LOG_INFO(QString("检测到多行文本，共 %1 行").arg(textValues.Length()));

                                    // 遍历每行文字
                                    for (Hlong lineIdx = 0; lineIdx < textValues.Length(); ++lineIdx) {
                                        QString lineText;
                                        try {
                                            lineText = QString::fromUtf8(textValues[lineIdx].S().Text());
                                        } catch (...) {
                                            continue;
                                        }

                                        if (lineText.isEmpty()) continue;

                                        // 构造该行的边界框
                                        QRectF lineBbox(0, 0, input.cols, input.rows);
                                        if (lineIdx < rowVals.Length() && lineIdx < colVals.Length() &&
                                            lineIdx < len1Vals.Length() && lineIdx < len2Vals.Length()) {
                                            double centerRow = rowVals[lineIdx].D();
                                            double centerCol = colVals[lineIdx].D();
                                            double halfWidth = len1Vals[lineIdx].D();
                                            double halfHeight = len2Vals[lineIdx].D();
                                            lineBbox = QRectF(centerCol - halfWidth, centerRow - halfHeight,
                                                             halfWidth * 2, halfHeight * 2);
                                        }

                                        ocrResults_.emplace_back(lineText, 1.0f, lineBbox, 0);
                                        LOG_INFO(QString("识别结果 %1: '%2' 边界框(%3,%4,%5,%6)")
                                                .arg(ocrResults_.size()).arg(lineText)
                                                .arg(lineBbox.x(), 0, 'f', 1).arg(lineBbox.y(), 0, 'f', 1)
                                                .arg(lineBbox.width(), 0, 'f', 1).arg(lineBbox.height(), 0, 'f', 1));
                                    }
                                } else if (textValues.Length() == 1) {
                                    // 单个字符串
                                    text = QString::fromUtf8(textValues[0].S().Text());
                                    LOG_DEBUG(QString("从 'word' 获取单行文本: '%1'").arg(text));
                                }
                            } catch (const HException& e) {
                                LOG_DEBUG(QString("获取 'word' 键失败: %1").arg(e.ErrorMessage().Text()));
                            }

                            // 如果是多行文本，已经在上面处理完了，跳过后续处理
                            if (hasMultipleLines) {
                                continue;  // 跳过当前words[i]的后续处理，继续下一个
                            }

                            // 如果仍然没有文本，尝试 char_candidates
                            if (text.isEmpty()) {
                                try {
                                    HTuple charCandidates;
                                    HalconCpp::GetDictTuple(wordDict, "char_candidates", &charCandidates);
                                    LOG_DEBUG(QString("'char_candidates' 长度: %1").arg(charCandidates.Length()));
                                    QString chars;
                                    for (Hlong c = 0; c < charCandidates.Length(); ++c) {
                                        if (charCandidates[c].Type() == 2) {
                                            chars += QString::fromUtf8(charCandidates[c].S().Text());
                                        }
                                    }
                                    if (!chars.isEmpty()) {
                                        text = chars;
                                        LOG_DEBUG(QString("从 'char_candidates' 获取文本: '%1'").arg(text));
                                    }
                                } catch (...) {}
                            }

                            // 从 row, col, length1, length2 构造边界框（单行情况）
                            if (rowVals.Length() > 0 && colVals.Length() > 0 &&
                                len1Vals.Length() > 0 && len2Vals.Length() > 0) {
                                double centerRow = rowVals[0].D();
                                double centerCol = colVals[0].D();
                                double halfWidth = len1Vals[0].D();
                                double halfHeight = len2Vals[0].D();
                                bbox = QRectF(centerCol - halfWidth, centerRow - halfHeight,
                                             halfWidth * 2, halfHeight * 2);
                                LOG_DEBUG(QString("从旋转矩形参数构造边界框: 中心(%1,%2) 大小(%3x%4)")
                                         .arg(centerCol, 0, 'f', 1).arg(centerRow, 0, 'f', 1)
                                         .arg(halfWidth * 2, 0, 'f', 1).arg(halfHeight * 2, 0, 'f', 1));
                            }

                        } catch (const HException& e) {
                            LOG_DEBUG(QString("解析单词字典[%1]失败: %2").arg(i).arg(e.ErrorMessage().Text()));
                        }
                    } else {
                        LOG_DEBUG(QString("words[%1] 未知类型: %2").arg(i).arg(wordType));
                        continue;
                    }

                    // 如果没有从单词字典获取到边界框，使用全局边界框数组
                    if (bbox.width() == input.cols && hasBoxes && i < row1Array.Length()) {
                        double row1 = row1Array[i].D();
                        double col1 = col1Array[i].D();
                        double row2 = row2Array[i].D();
                        double col2 = col2Array[i].D();
                        bbox = QRectF(col1, row1, col2 - col1, row2 - row1);
                        LOG_DEBUG(QString("使用全局边界框: (%1,%2)-(%3,%4)")
                                 .arg(col1, 0, 'f', 1).arg(row1, 0, 'f', 1)
                                 .arg(col2, 0, 'f', 1).arg(row2, 0, 'f', 1));
                    }

                    if (!text.isEmpty()) {
                        ocrResults_.emplace_back(text, confidence, bbox, 0);
                        LOG_INFO(QString("识别结果 %1: '%2' (置信度: %.2f)")
                                .arg(ocrResults_.size()).arg(text).arg(confidence));
                    }
                }
            }

            // 如果没有单词结果，尝试获取整行文本
            if (ocrResults_.empty()) {
                // 尝试多种可能的键名
                QStringList textLineKeys = {"text_line", "text_lines", "text_line_string"};
                for (const QString& key : textLineKeys) {
                    HTuple textLines;
                    try {
                        HalconCpp::GetDictTuple(dictHandle, key.toStdString().c_str(), &textLines);
                        if (textLines.Length() > 0) {
                            LOG_DEBUG(QString("获取到 '%1'，长度: %2").arg(key).arg(textLines.Length()));
                            for (Hlong i = 0; i < textLines.Length(); ++i) {
                                QString text = QString::fromUtf8(textLines[i].S().Text());
                                LOG_DEBUG(QString("文本行 %1: '%2'").arg(i).arg(text));
                                if (!text.isEmpty()) {
                                    ocrResults_.emplace_back(text, 1.0f, QRectF(0, 0, input.cols, input.rows), 0);
                                }
                            }
                            if (!ocrResults_.empty()) break;
                        }
                    } catch (const HException& e) {
                        LOG_DEBUG(QString("获取 '%1' 失败: %2").arg(key).arg(e.ErrorMessage().Text()));
                    }
                }
            }

            // 如果还是没有结果，尝试其他可能的键名
            if (ocrResults_.empty()) {
                QStringList possibleKeys = {"text", "texts", "recognition_result", "ocr_result",
                                           "detected_text", "recognized_words", "char"};
                for (const QString& key : possibleKeys) {
                    try {
                        HTuple values;
                        HalconCpp::GetDictTuple(dictHandle, key.toStdString().c_str(), &values);
                        if (values.Length() > 0) {
                            LOG_DEBUG(QString("找到键 '%1'，长度: %2").arg(key).arg(values.Length()));
                            for (Hlong i = 0; i < values.Length(); ++i) {
                                try {
                                    QString text = QString::fromUtf8(values[i].S().Text());
                                    if (!text.isEmpty()) {
                                        ocrResults_.emplace_back(text, 1.0f, QRectF(0, 0, input.cols, input.rows), 0);
                                    }
                                } catch (...) {
                                    // 可能不是字符串类型，跳过
                                }
                            }
                            if (!ocrResults_.empty()) break;
                        }
                    } catch (...) {}
                }
            }

        } catch (const HException& e) {
            LOG_WARNING(QString("解析Deep OCR结果时出错: %1").arg(e.ErrorMessage().Text()));
        }

        LOG_DEBUG(QString("Deep OCR推理完成，识别到 %1 个文本区域").arg(ocrResults_.size()));
        return true;

    } catch (const HException& e) {
        output.success = false;
        output.errorMessage = QString("Deep OCR推理失败: %1").arg(e.ErrorMessage().Text());
        LOG_ERROR(output.errorMessage);
        return false;
    } catch (const std::exception& e) {
        output.success = false;
        output.errorMessage = QString("Deep OCR推理异常: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }
}

void AIDetectionTool::unloadHalconDLModel()
{
    // 卸载DL模型
    if (halconModelLoaded_ && halconDLModel_ != nullptr) {
        try {
            HDlModel* dlModel = static_cast<HDlModel*>(halconDLModel_);
            dlModel->Clear();
            delete dlModel;
            LOG_INFO("Halcon DL模型已卸载");
        } catch (const HException& e) {
            LOG_WARNING(QString("卸载Halcon DL模型时出错: %1").arg(e.ErrorMessage().Text()));
        }
    }
    halconDLModel_ = nullptr;
    halconModelLoaded_ = false;

    // 卸载Deep OCR模型
    if (halconDeepOCRLoaded_ && halconDeepOCRHandle_ != nullptr) {
        try {
            HTuple* handle = static_cast<HTuple*>(halconDeepOCRHandle_);
            // Deep OCR句柄通过ClearHandle清理
            HalconCpp::ClearHandle(*handle);
            delete handle;
            LOG_INFO("Halcon Deep OCR模型已卸载");
        } catch (const HException& e) {
            LOG_WARNING(QString("卸载Halcon Deep OCR模型时出错: %1").arg(e.ErrorMessage().Text()));
        }
    }
    halconDeepOCRHandle_ = nullptr;
    halconDeepOCRLoaded_ = false;
}
#endif // USE_HALCON

} // namespace Algorithm
} // namespace VisionForge
