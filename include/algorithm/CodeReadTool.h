#pragma once

#include "algorithm/VisionTool.h"
#include "algorithm/ShapeMatchTool.h"
#include <QRectF>

#ifdef _WIN32
#include "HalconCpp.h"
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace Algorithm {

/**
 * @class CodeReadTool
 * @brief 读码工具（包含粗定位修正）
 * 
 * 功能：
 * 1. 使用形状匹配进行粗定位
 * 2. 根据定位结果修正读码区域（ROI）
 * 3. 识别二维码（Data Matrix, QR Code等）或一维码
 */
class CodeReadTool : public VisionTool {
    Q_OBJECT

public:
    /**
     * @brief 码制类型
     */
    enum CodeType {
        // 二维码
        DataMatrixECC200,
        QRCode,
        PDF417,
        Aztec,
        // 一维码
        Code128,
        Code39,
        EAN13,
        Code93,
        Interleaved2of5,
        Codabar
    };
    Q_ENUM(CodeType)

    explicit CodeReadTool(QObject* parent = nullptr);
    ~CodeReadTool() override;

    // ========== VisionTool接口实现 ==========
    ToolType toolType() const override { return CodeRead; }
    QString toolName() const override { return "读码工具"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 配置接口 ==========

    /**
     * @brief 设置码制
     */
    void setCodeType(CodeType type);
    CodeType getCodeType() const { return codeType_; }

    /**
     * @brief 获取内部的形状匹配工具（用于配置匹配参数）
     */
    ShapeMatchTool* getShapeMatchTool() const { return shapeMatchTool_; }

    /**
     * @brief 训练工具
     * @param image 参考图像
     * @param templateRoi 模板区域（用于粗定位）
     * @param codeRoi 读码区域（相对于参考图像）
     * @return 是否成功
     */
    bool train(const Base::ImageData::Ptr& image, const QRect& templateRoi, const QRect& codeRoi);

    /**
     * @brief 设置超时时间（毫秒）
     */
    void setTimeout(int timeoutMs);
    int getTimeout() const { return timeoutMs_; }

private:
    void initCodeReader();
    void clearCodeReader();
#ifdef _WIN32
    HImage imageDataToHImage(const Base::ImageData::Ptr& image);
#endif

private:
    ShapeMatchTool* shapeMatchTool_; // 内部组合形状匹配工具
    CodeType codeType_;
    int timeoutMs_;

    // 参考信息（训练时保存）
    double refRow_;     // 参考图像中模板中心的行坐标
    double refCol_;     // 参考图像中模板中心的列坐标
    double refAngle_;   // 参考图像中模板的角度（通常为0）
    
    // 读码ROI（相对于参考图像）
    // 使用 QRectF 保存，但在Halcon中使用时会转换为 Region
    double codeRoiRow1_;
    double codeRoiCol1_;
    double codeRoiRow2_;
    double codeRoiCol2_;

#ifdef _WIN32
    HTuple codeHandle_; // 读码模型句柄
    bool is2DCode_;     // 是否为二维码
#endif
};

} // namespace Algorithm
} // namespace VisionForge
