/**
 * @file DataExporter.cpp
 * @brief 数据导入导出管理器实现
 * @author VisionForge Team
 * @date 2025-12-19
 */

#include "base/DataExporter.h"
#include "base/Logger.h"

#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>
#include <QSet>
#include <QPointF>
#include <QRectF>

namespace VisionForge {
namespace Base {

DataExporter::DataExporter()
    : QObject(nullptr)
    , maxRecords_(100000)  // 默认最多10万条记录
{
}

DataExporter::~DataExporter()
{
}

DataExporter& DataExporter::instance()
{
    static DataExporter instance;
    return instance;
}

void DataExporter::addRecord(const DataRecord& record)
{
    records_.append(record);

    // 限制记录数量
    if (maxRecords_ > 0 && records_.size() > maxRecords_) {
        records_.removeFirst();
    }

    emit recordAdded(record);
}

void DataExporter::addRecord(const QString& toolChainName, bool success,
                              double totalTime, const QMap<QString, QVariant>& results)
{
    DataRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.toolChainName = toolChainName;
    record.overallResult = success;
    record.totalTime = totalTime;
    record.toolResults = results;

    addRecord(record);
}

void DataExporter::clearRecords()
{
    records_.clear();
    LOG_INFO(tr("数据记录已清空"));
}

void DataExporter::setMaxRecords(int maxRecords)
{
    maxRecords_ = maxRecords;
    if (maxRecords > 0 && records_.size() > maxRecords) {
        records_ = records_.mid(records_.size() - maxRecords);
    }
}

bool DataExporter::exportToFile(const QString& filePath, const ExportConfig& config)
{
    return exportRecords(filePath, records_, config);
}

QString DataExporter::exportToString(const ExportConfig& config) const
{
    switch (config.format) {
    case ExportFormat::CSV:
        return exportToCSV(records_, config);
    case ExportFormat::JSON:
        return exportToJSON(records_, config);
    case ExportFormat::XML:
        return exportToXML(records_, config);
    case ExportFormat::HTML:
        return exportToHTML(records_, config);
    case ExportFormat::TXT:
        return exportToTXT(records_, config);
    default:
        return exportToCSV(records_, config);
    }
}

bool DataExporter::exportRecords(const QString& filePath,
                                  const QVector<DataRecord>& records,
                                  const ExportConfig& config)
{
    // 确保目录存在
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            LOG_ERROR(tr("无法创建目录: %1").arg(dir.absolutePath()));
            emit exportCompleted(filePath, false);
            return false;
        }
    }

    // 生成内容
    QString content;
    ExportConfig cfg = config;

    // 根据文件扩展名自动确定格式
    QString ext = fileInfo.suffix().toLower();
    if (!ext.isEmpty()) {
        cfg.format = formatFromExtension(ext);
    }

    switch (cfg.format) {
    case ExportFormat::CSV:
        content = exportToCSV(records, cfg);
        break;
    case ExportFormat::JSON:
        content = exportToJSON(records, cfg);
        break;
    case ExportFormat::XML:
        content = exportToXML(records, cfg);
        break;
    case ExportFormat::HTML:
        content = exportToHTML(records, cfg);
        break;
    case ExportFormat::TXT:
        content = exportToTXT(records, cfg);
        break;
    }

    // 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(tr("无法打开文件: %1").arg(filePath));
        emit exportCompleted(filePath, false);
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << content;
    file.close();

    LOG_INFO(tr("数据导出成功: %1 (%2条记录)").arg(filePath).arg(records.size()));
    emit exportCompleted(filePath, true);
    return true;
}

bool DataExporter::exportByTimeRange(const QString& filePath,
                                      const QDateTime& startTime,
                                      const QDateTime& endTime,
                                      const ExportConfig& config)
{
    QVector<DataRecord> filtered;
    for (const auto& record : records_) {
        if ((!startTime.isValid() || record.timestamp >= startTime) &&
            (!endTime.isValid() || record.timestamp <= endTime)) {
            filtered.append(record);
        }
    }

    return exportRecords(filePath, filtered, config);
}

bool DataExporter::exportProjectConfig(const QString& filePath, const QJsonObject& config)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR(tr("无法保存项目配置: %1").arg(filePath));
        return false;
    }

    QJsonDocument doc(config);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    LOG_INFO(tr("项目配置已导出: %1").arg(filePath));
    return true;
}

bool DataExporter::importProjectConfig(const QString& filePath, QJsonObject& config)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_ERROR(tr("无法读取项目配置: %1").arg(filePath));
        emit configImported(false);
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR(tr("解析项目配置失败: %1").arg(error.errorString()));
        emit configImported(false);
        return false;
    }

    config = doc.object();
    LOG_INFO(tr("项目配置已导入: %1").arg(filePath));
    emit configImported(true);
    return true;
}

QStringList DataExporter::supportedFormats()
{
    return {"CSV", "JSON", "XML", "HTML", "TXT"};
}

ExportFormat DataExporter::formatFromExtension(const QString& extension)
{
    QString ext = extension.toLower();
    if (ext == "csv") return ExportFormat::CSV;
    if (ext == "json") return ExportFormat::JSON;
    if (ext == "xml") return ExportFormat::XML;
    if (ext == "html" || ext == "htm") return ExportFormat::HTML;
    if (ext == "txt") return ExportFormat::TXT;
    return ExportFormat::CSV;
}

QString DataExporter::extensionForFormat(ExportFormat format)
{
    switch (format) {
    case ExportFormat::CSV: return "csv";
    case ExportFormat::JSON: return "json";
    case ExportFormat::XML: return "xml";
    case ExportFormat::HTML: return "html";
    case ExportFormat::TXT: return "txt";
    default: return "csv";
    }
}

QString DataExporter::fileFilterForFormat(ExportFormat format)
{
    switch (format) {
    case ExportFormat::CSV: return tr("CSV文件 (*.csv)");
    case ExportFormat::JSON: return tr("JSON文件 (*.json)");
    case ExportFormat::XML: return tr("XML文件 (*.xml)");
    case ExportFormat::HTML: return tr("HTML文件 (*.html *.htm)");
    case ExportFormat::TXT: return tr("文本文件 (*.txt)");
    default: return tr("CSV文件 (*.csv)");
    }
}

QString DataExporter::allFileFilters()
{
    return tr("CSV文件 (*.csv);;JSON文件 (*.json);;XML文件 (*.xml);;HTML文件 (*.html);;文本文件 (*.txt);;所有文件 (*.*)");
}

// ==================== 导出实现 ====================

QString DataExporter::exportToCSV(const QVector<DataRecord>& records, const ExportConfig& config) const
{
    if (records.isEmpty()) {
        return QString();
    }

    QStringList lines;
    QStringList headers = getColumnHeaders(records);

    // 表头
    if (config.includeHeader) {
        QStringList headerRow;
        if (config.includeTimestamp) {
            headerRow << tr("时间戳");
        }
        headerRow << tr("工具链") << tr("结果") << tr("总时间(ms)");

        for (const QString& h : headers) {
            headerRow << h;
            if (config.includeToolTimes) {
                headerRow << h + tr("_时间(ms)");
            }
        }

        lines << headerRow.join(config.delimiter);
    }

    // 数据行
    for (const auto& record : records) {
        QStringList row;

        if (config.includeTimestamp) {
            row << record.timestamp.toString(config.dateFormat);
        }

        row << escapeCSV(record.toolChainName, config.delimiter);
        row << (record.overallResult ? "OK" : "NG");
        row << QString::number(record.totalTime, 'f', 3);

        for (const QString& h : headers) {
            QVariant value = record.toolResults.value(h);
            row << escapeCSV(variantToString(value), config.delimiter);

            if (config.includeToolTimes) {
                double time = record.toolTimes.value(h, 0.0);
                row << QString::number(time, 'f', 3);
            }
        }

        lines << row.join(config.delimiter);
    }

    return lines.join("\n");
}

QString DataExporter::exportToJSON(const QVector<DataRecord>& records, const ExportConfig& config) const
{
    QJsonObject root;
    root["exportTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["recordCount"] = records.size();

    QJsonArray recordsArray;
    for (const auto& record : records) {
        QJsonObject obj;

        if (config.includeTimestamp) {
            obj["timestamp"] = record.timestamp.toString(Qt::ISODate);
        }

        obj["toolChain"] = record.toolChainName;
        obj["result"] = record.overallResult ? "OK" : "NG";
        obj["totalTime"] = record.totalTime;

        QJsonObject results;
        for (auto it = record.toolResults.begin(); it != record.toolResults.end(); ++it) {
            results[it.key()] = QJsonValue::fromVariant(it.value());
        }
        obj["results"] = results;

        if (config.includeToolTimes) {
            QJsonObject times;
            for (auto it = record.toolTimes.begin(); it != record.toolTimes.end(); ++it) {
                times[it.key()] = it.value();
            }
            obj["toolTimes"] = times;
        }

        recordsArray.append(obj);
    }

    root["records"] = recordsArray;

    QJsonDocument doc(root);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
}

QString DataExporter::exportToXML(const QVector<DataRecord>& records, const ExportConfig& config) const
{
    QString xml;
    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml += "<?mso-application progid=\"Excel.Sheet\"?>\n";
    xml += "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n";
    xml += "  xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\">\n";
    xml += "  <Worksheet ss:Name=\"" + tr("检测数据") + "\">\n";
    xml += "    <Table>\n";

    QStringList headers = getColumnHeaders(records);

    // 表头行
    if (config.includeHeader) {
        xml += "      <Row>\n";
        if (config.includeTimestamp) {
            xml += "        <Cell><Data ss:Type=\"String\">" + tr("时间戳") + "</Data></Cell>\n";
        }
        xml += "        <Cell><Data ss:Type=\"String\">" + tr("工具链") + "</Data></Cell>\n";
        xml += "        <Cell><Data ss:Type=\"String\">" + tr("结果") + "</Data></Cell>\n";
        xml += "        <Cell><Data ss:Type=\"String\">" + tr("总时间(ms)") + "</Data></Cell>\n";

        for (const QString& h : headers) {
            xml += "        <Cell><Data ss:Type=\"String\">" + escapeXML(h) + "</Data></Cell>\n";
        }
        xml += "      </Row>\n";
    }

    // 数据行
    for (const auto& record : records) {
        xml += "      <Row>\n";

        if (config.includeTimestamp) {
            xml += "        <Cell><Data ss:Type=\"String\">"
                   + record.timestamp.toString(config.dateFormat) + "</Data></Cell>\n";
        }

        xml += "        <Cell><Data ss:Type=\"String\">"
               + escapeXML(record.toolChainName) + "</Data></Cell>\n";
        xml += "        <Cell><Data ss:Type=\"String\">"
               + QString(record.overallResult ? "OK" : "NG") + "</Data></Cell>\n";
        xml += "        <Cell><Data ss:Type=\"Number\">"
               + QString::number(record.totalTime, 'f', 3) + "</Data></Cell>\n";

        for (const QString& h : headers) {
            QVariant value = record.toolResults.value(h);
            QString type = value.typeId() == QMetaType::Double ||
                          value.typeId() == QMetaType::Int ||
                          value.typeId() == QMetaType::LongLong ? "Number" : "String";
            xml += "        <Cell><Data ss:Type=\"" + type + "\">"
                   + escapeXML(variantToString(value)) + "</Data></Cell>\n";
        }

        xml += "      </Row>\n";
    }

    xml += "    </Table>\n";
    xml += "  </Worksheet>\n";
    xml += "</Workbook>\n";

    return xml;
}

QString DataExporter::exportToHTML(const QVector<DataRecord>& records, const ExportConfig& config) const
{
    QString html;
    html += "<!DOCTYPE html>\n<html>\n<head>\n";
    html += "<meta charset=\"UTF-8\">\n";
    html += "<title>" + tr("VisionForge Pro 检测数据") + "</title>\n";
    html += "<style>\n";
    html += "body { font-family: 'Microsoft YaHei', Arial, sans-serif; margin: 20px; }\n";
    html += "h1 { color: #2196F3; }\n";
    html += "table { border-collapse: collapse; width: 100%; margin-top: 20px; }\n";
    html += "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n";
    html += "th { background-color: #2196F3; color: white; }\n";
    html += "tr:nth-child(even) { background-color: #f9f9f9; }\n";
    html += ".ok { color: #4CAF50; font-weight: bold; }\n";
    html += ".ng { color: #F44336; font-weight: bold; }\n";
    html += "</style>\n</head>\n<body>\n";

    html += "<h1>" + tr("VisionForge Pro 检测数据导出") + "</h1>\n";
    html += "<p>" + tr("导出时间: ") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "</p>\n";
    html += "<p>" + tr("记录数量: ") + QString::number(records.size()) + "</p>\n";

    html += "<table>\n";

    QStringList headers = getColumnHeaders(records);

    // 表头
    if (config.includeHeader) {
        html += "<tr>";
        if (config.includeTimestamp) {
            html += "<th>" + tr("时间戳") + "</th>";
        }
        html += "<th>" + tr("工具链") + "</th>";
        html += "<th>" + tr("结果") + "</th>";
        html += "<th>" + tr("总时间(ms)") + "</th>";

        for (const QString& h : headers) {
            html += "<th>" + escapeHTML(h) + "</th>";
        }
        html += "</tr>\n";
    }

    // 数据行
    for (const auto& record : records) {
        html += "<tr>";

        if (config.includeTimestamp) {
            html += "<td>" + record.timestamp.toString(config.dateFormat) + "</td>";
        }

        html += "<td>" + escapeHTML(record.toolChainName) + "</td>";
        html += "<td class=\"" + QString(record.overallResult ? "ok" : "ng") + "\">"
                + (record.overallResult ? "OK" : "NG") + "</td>";
        html += "<td>" + QString::number(record.totalTime, 'f', 3) + "</td>";

        for (const QString& h : headers) {
            QVariant value = record.toolResults.value(h);
            html += "<td>" + escapeHTML(variantToString(value)) + "</td>";
        }

        html += "</tr>\n";
    }

    html += "</table>\n</body>\n</html>";

    return html;
}

QString DataExporter::exportToTXT(const QVector<DataRecord>& records, const ExportConfig& config) const
{
    QStringList lines;
    lines << tr("VisionForge Pro 检测数据导出");
    lines << tr("导出时间: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    lines << tr("记录数量: %1").arg(records.size());
    lines << "================================================================================";
    lines << "";

    for (int i = 0; i < records.size(); ++i) {
        const auto& record = records[i];

        lines << tr("记录 #%1").arg(i + 1);
        lines << "--------------------------------------------------------------------------------";

        if (config.includeTimestamp) {
            lines << tr("  时间戳: %1").arg(record.timestamp.toString(config.dateFormat));
        }

        lines << tr("  工具链: %1").arg(record.toolChainName);
        lines << tr("  结果: %1").arg(record.overallResult ? "OK" : "NG");
        lines << tr("  总时间: %1 ms").arg(record.totalTime, 0, 'f', 3);

        if (!record.toolResults.isEmpty()) {
            lines << tr("  工具结果:");
            for (auto it = record.toolResults.begin(); it != record.toolResults.end(); ++it) {
                QString timeStr;
                if (config.includeToolTimes && record.toolTimes.contains(it.key())) {
                    timeStr = tr(" (耗时: %1 ms)").arg(record.toolTimes.value(it.key()), 0, 'f', 3);
                }
                lines << tr("    %1: %2%3").arg(it.key()).arg(variantToString(it.value())).arg(timeStr);
            }
        }

        lines << "";
    }

    return lines.join("\n");
}

// ==================== 辅助方法 ====================

QStringList DataExporter::getColumnHeaders(const QVector<DataRecord>& records) const
{
    QSet<QString> headerSet;
    for (const auto& record : records) {
        for (auto it = record.toolResults.begin(); it != record.toolResults.end(); ++it) {
            headerSet.insert(it.key());
        }
    }

    QStringList headers = headerSet.values();
    headers.sort();
    return headers;
}

QString DataExporter::escapeCSV(const QString& value, const QString& delimiter) const
{
    QString escaped = value;
    escaped.replace("\"", "\"\"");

    if (escaped.contains(delimiter) || escaped.contains("\"") ||
        escaped.contains("\n") || escaped.contains("\r")) {
        escaped = "\"" + escaped + "\"";
    }

    return escaped;
}

QString DataExporter::escapeXML(const QString& value) const
{
    QString escaped = value;
    escaped.replace("&", "&amp;");
    escaped.replace("<", "&lt;");
    escaped.replace(">", "&gt;");
    escaped.replace("\"", "&quot;");
    escaped.replace("'", "&apos;");
    return escaped;
}

QString DataExporter::escapeHTML(const QString& value) const
{
    QString escaped = value;
    escaped.replace("&", "&amp;");
    escaped.replace("<", "&lt;");
    escaped.replace(">", "&gt;");
    return escaped;
}

QString DataExporter::variantToString(const QVariant& value) const
{
    if (!value.isValid()) {
        return QString();
    }

    switch (value.typeId()) {
    case QMetaType::Double:
        return QString::number(value.toDouble(), 'f', 6);
    case QMetaType::Float:
        return QString::number(value.toFloat(), 'f', 4);
    case QMetaType::Int:
    case QMetaType::LongLong:
        return QString::number(value.toLongLong());
    case QMetaType::Bool:
        return value.toBool() ? "true" : "false";
    case QMetaType::QPointF: {
        QPointF p = value.toPointF();
        return QString("(%1, %2)").arg(p.x(), 0, 'f', 2).arg(p.y(), 0, 'f', 2);
    }
    case QMetaType::QRectF: {
        QRectF r = value.toRectF();
        return QString("(%1, %2, %3x%4)")
            .arg(r.x(), 0, 'f', 2).arg(r.y(), 0, 'f', 2)
            .arg(r.width(), 0, 'f', 2).arg(r.height(), 0, 'f', 2);
    }
    default:
        return value.toString();
    }
}

} // namespace Base
} // namespace VisionForge
