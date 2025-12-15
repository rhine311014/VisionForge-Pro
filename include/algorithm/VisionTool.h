/**
 * @file VisionTool.h
 * @brief 视觉工具抽象基类
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QIcon>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 工具结果结构
 */
struct ToolResult {
    bool success;                           // 是否成功
    QString errorMessage;                   // 错误消息
    QJsonObject data;                       // 结果数据
    Base::ImageData::Ptr outputImage;       // 输出图像
    double executionTime;                   // 执行时间(毫秒)
    QVariantMap displayObjects;             // 显示对象(如Halcon XLD轮廓)

    ToolResult() : success(false), executionTime(0.0) {}

    /**
     * @brief 获取值（泛型）
     */
    template<typename T>
    T getValue(const QString& key, const T& defaultValue = T()) const {
        return data.value(key).toVariant().value<T>();
    }

    /**
     * @brief 设置值
     */
    void setValue(const QString& key, const QVariant& value) {
        data[key] = QJsonValue::fromVariant(value);
    }

    /**
     * @brief 设置显示对象
     */
    void setDisplayObject(const QString& key, const QVariant& object) {
        displayObjects[key] = object;
    }

    /**
     * @brief 获取显示对象
     */
    QVariant getDisplayObject(const QString& key) const {
        return displayObjects.value(key);
    }
};

/**
 * @brief 视觉工具抽象基类
 *
 * 所有视觉算法工具的基类，定义统一接口
 */
class VisionTool : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 工具类型枚举
     */
    enum ToolType {
        // 图像源
        Camera, File, Video,

        // 预处理
        Gray, Blur, Binary, ROI, ColorConvert,

        // 定位检测
        TemplateMatch, Match, FindCircle, FindLine, FindEdge,
        BlobAnalysis, AIDetection,

        // 测量计算
        MeasureDistance, MeasureAngle, MeasureArea,
        CalcCenter, CalcOrientation,

        // 判定输出
        RangeJudge, LogicOperation, PLCOutput, SaveImage,

        // 自定义
        Custom
    };
    Q_ENUM(ToolType)

    explicit VisionTool(QObject* parent = nullptr);
    virtual ~VisionTool() = default;

    // ========== 核心接口 ==========

    /**
     * @brief 获取工具类型
     */
    virtual ToolType toolType() const = 0;

    /**
     * @brief 获取工具名称
     */
    virtual QString toolName() const = 0;

    /**
     * @brief 获取工具图标
     */
    virtual QIcon toolIcon() const;

    /**
     * @brief 处理图像
     * @param input 输入图像
     * @param output 输出结果
     * @return true如果成功
     */
    virtual bool process(const Base::ImageData::Ptr& input, ToolResult& output) = 0;

    /**
     * @brief 创建参数配置界面
     * @return 参数配置窗口
     */
    virtual QWidget* createParamWidget() { return nullptr; }

    /**
     * @brief 序列化参数
     */
    virtual QJsonObject serializeParams() const = 0;

    /**
     * @brief 反序列化参数
     */
    virtual void deserializeParams(const QJsonObject& json) = 0;

    // ========== 调试接口 ==========

    /**
     * @brief 获取调试图像
     */
    virtual Base::ImageData::Ptr getDebugImage() const { return debugImage_; }

    /**
     * @brief 获取状态文本
     */
    virtual QString getStatusText() const { return statusText_; }

    // ========== 属性 ==========

    /**
     * @brief 检查是否启用
     */
    bool isEnabled() const { return enabled_; }

    /**
     * @brief 设置启用状态
     */
    void setEnabled(bool enabled);

    /**
     * @brief 获取显示名称
     */
    QString displayName() const { return displayName_; }

    /**
     * @brief 设置显示名称
     */
    void setDisplayName(const QString& name);

signals:
    /**
     * @brief 参数改变信号
     */
    void paramChanged();

    /**
     * @brief 处理开始信号
     */
    void processingStarted();

    /**
     * @brief 处理完成信号
     * @param success 是否成功
     * @param elapsedMs 耗时(毫秒)
     */
    void processingFinished(bool success, qint64 elapsedMs);

    /**
     * @brief 调试图像更新信号
     */
    void debugImageUpdated();

    /**
     * @brief 状态文本改变信号
     */
    void statusTextChanged(const QString& text);

protected:
    /**
     * @brief 设置调试图像
     */
    void setDebugImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 设置状态文本
     */
    void setStatusText(const QString& text);

protected:
    bool enabled_;                          // 是否启用
    QString displayName_;                   // 显示名称
    Base::ImageData::Ptr debugImage_;       // 调试图像
    QString statusText_;                    // 状态文本
};

} // namespace Algorithm
} // namespace VisionForge
