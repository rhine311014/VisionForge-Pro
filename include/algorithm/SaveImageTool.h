/**
 * @file SaveImageTool.h
 * @brief 图像保存工具
 * @details 保存图像到文件，支持多种格式和命名规则
 * @author VisionForge Team
 * @date 2025-12-18
 */

#pragma once

#include "algorithm/VisionTool.h"
#include <QString>
#include <QDir>

namespace VisionForge {
namespace Algorithm {

/**
 * @class SaveImageTool
 * @brief 图像保存工具
 *
 * 支持的功能:
 * - 多种图像格式 (PNG, JPG, BMP, TIFF)
 * - 自动命名 (时间戳、序号、自定义前缀)
 * - 按日期分目录
 * - 条件保存 (OK/NG分别保存)
 * - JPEG质量设置
 */
class SaveImageTool : public VisionTool
{
    Q_OBJECT

public:
    /**
     * @brief 图像格式
     */
    enum ImageFormat {
        PNG,
        JPEG,
        BMP,
        TIFF
    };
    Q_ENUM(ImageFormat)

    /**
     * @brief 命名模式
     */
    enum NamingMode {
        Timestamp,      // 时间戳: 20251218_143025_123
        Sequential,     // 序号: 000001, 000002
        Custom          // 自定义: prefix_timestamp
    };
    Q_ENUM(NamingMode)

    /**
     * @brief 保存条件
     */
    enum SaveCondition {
        Always,         // 总是保存
        OnlyOK,         // 仅保存OK结果
        OnlyNG          // 仅保存NG结果
    };
    Q_ENUM(SaveCondition)

    explicit SaveImageTool(QObject* parent = nullptr);
    ~SaveImageTool() override = default;

    // ========== VisionTool接口 ==========
    ToolType toolType() const override { return ToolType::SaveImage; }
    QString toolName() const override { return "图像保存"; }
    bool process(const Base::ImageData::Ptr& input, ToolResult& output) override;
    QJsonObject serializeParams() const override;
    void deserializeParams(const QJsonObject& json) override;

    // ========== 保存设置 ==========

    /**
     * @brief 设置保存目录
     */
    void setSaveDirectory(const QString& dir);
    QString saveDirectory() const { return saveDir_; }

    /**
     * @brief 设置图像格式
     */
    void setImageFormat(ImageFormat format);
    ImageFormat imageFormat() const { return format_; }

    /**
     * @brief 设置命名模式
     */
    void setNamingMode(NamingMode mode);
    NamingMode namingMode() const { return namingMode_; }

    /**
     * @brief 设置文件名前缀
     */
    void setPrefix(const QString& prefix);
    QString prefix() const { return prefix_; }

    /**
     * @brief 设置JPEG质量 (1-100)
     */
    void setJpegQuality(int quality);
    int jpegQuality() const { return jpegQuality_; }

    /**
     * @brief 设置是否按日期分目录
     */
    void setCreateDateSubfolder(bool create);
    bool createDateSubfolder() const { return createDateSubfolder_; }

    /**
     * @brief 设置保存条件
     */
    void setSaveCondition(SaveCondition condition);
    SaveCondition saveCondition() const { return saveCondition_; }

    /**
     * @brief 设置判定结果 (用于条件保存)
     */
    void setJudgeResult(bool isOK) { judgeResult_ = isOK; }

    /**
     * @brief 获取保存计数
     */
    int saveCount() const { return saveCount_; }

    /**
     * @brief 重置计数器
     */
    void resetCounter() { saveCount_ = 0; }

    /**
     * @brief 获取最后保存的文件路径
     */
    QString lastSavedPath() const { return lastSavedPath_; }

signals:
    void imageSaved(const QString& path);
    void saveError(const QString& error);

private:
    QString generateFilename();
    QString getFormatExtension() const;
    bool ensureDirectoryExists(const QString& path);

private:
    QString saveDir_;
    ImageFormat format_;
    NamingMode namingMode_;
    QString prefix_;
    int jpegQuality_;
    bool createDateSubfolder_;
    SaveCondition saveCondition_;
    bool judgeResult_;
    int saveCount_;
    QString lastSavedPath_;
};

} // namespace Algorithm
} // namespace VisionForge
