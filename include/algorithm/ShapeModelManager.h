/**
 * @file ShapeModelManager.h
 * @brief 形状模板管理器
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QImage>
#include <memory>
#include <vector>

namespace VisionForge {
namespace Algorithm {

/**
 * @struct ShapeModelInfo
 * @brief 形状模板信息
 */
struct ShapeModelInfo {
    QString id;              // 唯一ID
    QString name;            // 模板名称
    QString description;     // 模板描述
    QString filePath;        // 模板文件路径(.shm)
    QString thumbnailPath;   // 缩略图路径
    QDateTime createTime;    // 创建时间
    QDateTime modifyTime;    // 修改时间

    // 模板参数
    int numLevels;           // 金字塔层数
    double angleStart;       // 起始角度
    double angleExtent;      // 角度范围
    int minContrast;         // 最小对比度

    // 匹配参数（新增）
    double minScore;         // 最小分数
    int numMatches;          // 最大匹配数
    double maxOverlap;       // 最大重叠
    QString subPixel;        // 亚像素精度
    double greediness;       // 贪婪度
    double scaleMin;         // 最小缩放
    double scaleMax;         // 最大缩放
    int matchType;           // 匹配模式 (0=标准, 1=缩放, 2=仿射)

    // 统计信息
    int usageCount;          // 使用次数

    // 序列化
    QJsonObject toJson() const;
    static ShapeModelInfo fromJson(const QJsonObject& json);
};

using ShapeModelInfoPtr = std::shared_ptr<ShapeModelInfo>;

/**
 * @class ShapeModelManager
 * @brief 形状模板管理器
 *
 * 功能：
 * - 管理多个形状模板
 * - 支持导入/导出
 * - 支持预览
 * - 支持批量操作
 */
class ShapeModelManager : public QObject
{
    Q_OBJECT

public:
    static ShapeModelManager& instance();

    /**
     * @brief 加载模板库配置
     * @param libraryPath 模板库目录
     */
    bool loadLibrary(const QString& libraryPath);

    /**
     * @brief 保存模板库配置
     */
    bool saveLibrary();

    /**
     * @brief 设置模板库路径
     */
    void setLibraryPath(const QString& path);

    /**
     * @brief 获取模板库路径
     */
    QString getLibraryPath() const { return libraryPath_; }

    /**
     * @brief 添加模板到库
     * @param info 模板信息
     * @param modelFilePath 模板文件路径（会被复制到库目录）
     * @param thumbnail 缩略图（可选）
     */
    bool addModel(ShapeModelInfoPtr info, const QString& modelFilePath,
                 const QImage& thumbnail = QImage());

    /**
     * @brief 从库中移除模板
     */
    bool removeModel(const QString& modelId);

    /**
     * @brief 更新模板信息
     */
    bool updateModel(ShapeModelInfoPtr info);

    /**
     * @brief 获取所有模板
     */
    std::vector<ShapeModelInfoPtr> getAllModels() const;

    /**
     * @brief 根据ID获取模板
     */
    ShapeModelInfoPtr getModel(const QString& modelId) const;

    /**
     * @brief 搜索模板
     */
    std::vector<ShapeModelInfoPtr> searchModels(const QString& keyword) const;

    /**
     * @brief 导出模板（复制到指定目录）
     */
    bool exportModel(const QString& modelId, const QString& targetPath);

    /**
     * @brief 批量导出
     */
    bool exportModels(const QStringList& modelIds, const QString& targetDir);

    /**
     * @brief 导入模板到库
     */
    bool importModel(const QString& modelFilePath, const QString& name,
                    const QString& description = QString());

    /**
     * @brief 批量导入
     */
    int importModels(const QStringList& modelFilePaths);

    /**
     * @brief 批量删除
     */
    bool removeModels(const QStringList& modelIds);

    /**
     * @brief 增加模板使用计数
     */
    void incrementUsage(const QString& modelId);

    /**
     * @brief 生成缩略图
     */
    QImage generateThumbnail(const QString& modelFilePath,
                            int width = 128, int height = 128);

    /**
     * @brief 获取模板缩略图
     */
    QImage getThumbnail(const QString& modelId) const;

signals:
    /**
     * @brief 模板库已加载
     */
    void libraryLoaded();

    /**
     * @brief 模板已添加
     */
    void modelAdded(const QString& modelId);

    /**
     * @brief 模板已移除
     */
    void modelRemoved(const QString& modelId);

    /**
     * @brief 模板已更新
     */
    void modelUpdated(const QString& modelId);

private:
    ShapeModelManager();
    ~ShapeModelManager();
    ShapeModelManager(const ShapeModelManager&) = delete;
    ShapeModelManager& operator=(const ShapeModelManager&) = delete;

    QString generateModelId() const;
    bool copyModelFile(const QString& srcPath, const QString& dstPath);
    bool saveThumbnail(const QImage& thumbnail, const QString& path);

private:
    QString libraryPath_;                                    // 模板库目录
    std::vector<ShapeModelInfoPtr> models_;                  // 模板列表
    QString configFileName_;                                 // 配置文件名
};

} // namespace Algorithm
} // namespace VisionForge
