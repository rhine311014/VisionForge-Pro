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
#include <functional>

class QDialog;

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
     * @brief 对话框创建器类型
     * @param tool 工具实例
     * @param parent 父窗口
     * @param image 当前图像
     * @return 对话框实例（调用者负责释放）
     */
    using DialogCreator = std::function<QDialog*(VisionTool*, QWidget*, const Base::ImageData::Ptr&)>;

    /**
     * @brief 设置对话框创建器（由UI层调用）
     * @param creator 创建器函数
     * @note 应在应用程序初始化时调用一次
     */
    static void setDialogCreator(DialogCreator creator);

    /**
     * @brief 工具类型枚举
     */
    enum ToolType {
        // 图像源
        Camera, File, Video,

        // 预处理
        Gray, Blur, Binary, ROI, ColorConvert, Edge, Morphology,
        SubPixelEdge,           // 亚像素边缘检测

        // 定位检测
        TemplateMatch, Match, FindCircle, FindLine, FindEdge,
        BlobAnalysis, AIDetection,
        QuickSearch,            // 快速搜索
        PatternSearch,          // 图案搜索
        CornerSearch,           // 角点搜索

        // 测量计算
        MeasureDistance, MeasureAngle, MeasureArea,
        CalcCenter, CalcOrientation,
        VirtualObject,          // 虚拟对象

        // 判定输出
        RangeJudge, LogicOperation, PLCOutput, SaveImage,

        // 标定
        CameraCalib, NinePointCalib,
        MultiPointAlignment,    // 多点对位
        AlignmentOutput,        // 对位输出

        // 识别
        CodeRead,

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
     * @brief 创建参数配置界面（嵌入式）
     * @return 参数配置窗口
     */
    virtual QWidget* createParamWidget() { return nullptr; }

    /**
     * @brief 创建配置对话框
     * @param parent 父窗口
     * @param image 当前图像（用于预览）
     * @return 配置对话框，调用者负责释放
     * @note 使用静态 DialogCreator 创建对话框，由 UI 层设置
     */
    QDialog* createConfigDialog(QWidget* parent,
                                const Base::ImageData::Ptr& image = nullptr);

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

private:
    static DialogCreator dialogCreator_;    // 对话框创建器（由UI层设置）
};

} // namespace Algorithm
} // namespace VisionForge
