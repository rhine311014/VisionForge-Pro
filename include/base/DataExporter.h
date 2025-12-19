/**
 * @file DataExporter.h
 * @brief 数据导入导出管理器
 * @details 支持多种格式的数据导入导出功能
 * @author VisionForge Team
 * @date 2025-12-19
 */

#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

namespace VisionForge {
namespace Base {

/**
 * @brief 导出格式枚举
 */
enum class ExportFormat {
    CSV,            // CSV格式
    JSON,           // JSON格式
    XML,            // XML格式（Excel兼容）
    HTML,           // HTML表格
    TXT             // 纯文本
};

/**
 * @brief 数据记录结构
 */
struct DataRecord {
    QDateTime timestamp;                    // 时间戳
    QString toolChainName;                  // 工具链名称
    bool overallResult;                     // 总体结果(OK/NG)
    double totalTime;                       // 总执行时间(ms)
    QMap<QString, QVariant> toolResults;    // 各工具结果 {工具名: 结果值}
    QMap<QString, double> toolTimes;        // 各工具执行时间

    DataRecord() : overallResult(true), totalTime(0.0) {
        timestamp = QDateTime::currentDateTime();
    }
};

/**
 * @brief 导出配置
 */
struct ExportConfig {
    ExportFormat format;            // 导出格式
    bool includeHeader;             // 是否包含表头
    bool includeTimestamp;          // 是否包含时间戳
    bool includeToolTimes;          // 是否包含工具执行时间
    QString delimiter;              // CSV分隔符
    QString dateFormat;             // 日期格式
    QStringList selectedColumns;    // 选择的列（空表示全部）

    ExportConfig()
        : format(ExportFormat::CSV)
        , includeHeader(true)
        , includeTimestamp(true)
        , includeToolTimes(true)
        , delimiter(",")
        , dateFormat("yyyy-MM-dd hh:mm:ss.zzz")
    {}
};

/**
 * @class DataExporter
 * @brief 数据导入导出管理器（单例）
 *
 * 功能：
 * - 记录检测结果数据
 * - 导出数据到多种格式
 * - 导入项目配置
 * - 支持批量数据操作
 */
class DataExporter : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static DataExporter& instance();

    // ==================== 数据记录 ====================

    /**
     * @brief 添加一条数据记录
     * @param record 数据记录
     */
    void addRecord(const DataRecord& record);

    /**
     * @brief 添加简化记录
     * @param toolChainName 工具链名称
     * @param success 是否成功
     * @param totalTime 总执行时间
     * @param results 结果数据
     */
    void addRecord(const QString& toolChainName, bool success,
                   double totalTime, const QMap<QString, QVariant>& results);

    /**
     * @brief 获取所有记录
     */
    QVector<DataRecord> getAllRecords() const { return records_; }

    /**
     * @brief 获取记录数量
     */
    int recordCount() const { return records_.size(); }

    /**
     * @brief 清空所有记录
     */
    void clearRecords();

    /**
     * @brief 设置最大记录数
     */
    void setMaxRecords(int maxRecords);

    // ==================== 数据导出 ====================

    /**
     * @brief 导出数据到文件
     * @param filePath 文件路径
     * @param config 导出配置
     * @return true如果成功
     */
    bool exportToFile(const QString& filePath, const ExportConfig& config = ExportConfig());

    /**
     * @brief 导出数据到字符串
     * @param config 导出配置
     * @return 导出的字符串
     */
    QString exportToString(const ExportConfig& config = ExportConfig()) const;

    /**
     * @brief 导出选定记录
     * @param filePath 文件路径
     * @param records 要导出的记录
     * @param config 导出配置
     * @return true如果成功
     */
    bool exportRecords(const QString& filePath,
                       const QVector<DataRecord>& records,
                       const ExportConfig& config = ExportConfig());

    /**
     * @brief 按时间范围导出
     * @param filePath 文件路径
     * @param startTime 开始时间
     * @param endTime 结束时间
     * @param config 导出配置
     * @return true如果成功
     */
    bool exportByTimeRange(const QString& filePath,
                           const QDateTime& startTime,
                           const QDateTime& endTime,
                           const ExportConfig& config = ExportConfig());

    // ==================== 项目配置导入导出 ====================

    /**
     * @brief 导出项目配置
     * @param filePath 文件路径
     * @param config 项目配置JSON
     * @return true如果成功
     */
    bool exportProjectConfig(const QString& filePath, const QJsonObject& config);

    /**
     * @brief 导入项目配置
     * @param filePath 文件路径
     * @param config 输出的项目配置
     * @return true如果成功
     */
    bool importProjectConfig(const QString& filePath, QJsonObject& config);

    // ==================== 工具类方法 ====================

    /**
     * @brief 获取支持的导出格式列表
     */
    static QStringList supportedFormats();

    /**
     * @brief 根据文件扩展名获取格式
     */
    static ExportFormat formatFromExtension(const QString& extension);

    /**
     * @brief 获取格式的文件扩展名
     */
    static QString extensionForFormat(ExportFormat format);

    /**
     * @brief 获取格式的文件过滤器
     */
    static QString fileFilterForFormat(ExportFormat format);

    /**
     * @brief 获取所有格式的文件过滤器
     */
    static QString allFileFilters();

signals:
    /**
     * @brief 记录添加信号
     */
    void recordAdded(const DataRecord& record);

    /**
     * @brief 数据导出完成信号
     */
    void exportCompleted(const QString& filePath, bool success);

    /**
     * @brief 配置导入完成信号
     */
    void configImported(bool success);

private:
    DataExporter();
    ~DataExporter();
    DataExporter(const DataExporter&) = delete;
    DataExporter& operator=(const DataExporter&) = delete;

    // 导出方法
    QString exportToCSV(const QVector<DataRecord>& records, const ExportConfig& config) const;
    QString exportToJSON(const QVector<DataRecord>& records, const ExportConfig& config) const;
    QString exportToXML(const QVector<DataRecord>& records, const ExportConfig& config) const;
    QString exportToHTML(const QVector<DataRecord>& records, const ExportConfig& config) const;
    QString exportToTXT(const QVector<DataRecord>& records, const ExportConfig& config) const;

    // 辅助方法
    QStringList getColumnHeaders(const QVector<DataRecord>& records) const;
    QString escapeCSV(const QString& value, const QString& delimiter) const;
    QString escapeXML(const QString& value) const;
    QString escapeHTML(const QString& value) const;
    QString variantToString(const QVariant& value) const;

private:
    QVector<DataRecord> records_;
    int maxRecords_;
};

} // namespace Base
} // namespace VisionForge

// 便捷宏
#define DATA_EXPORTER VisionForge::Base::DataExporter::instance()
