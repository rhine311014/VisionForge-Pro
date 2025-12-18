/**
 * @file SaveImageTool.cpp
 * @brief 图像保存工具实现
 */

#include "algorithm/SaveImageTool.h"
#include "base/Logger.h"
#include <opencv2/imgcodecs.hpp>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>

namespace VisionForge {
namespace Algorithm {

SaveImageTool::SaveImageTool(QObject* parent)
    : VisionTool(parent)
    , saveDir_("./saved_images")
    , format_(PNG)
    , namingMode_(Timestamp)
    , prefix_("img")
    , jpegQuality_(95)
    , createDateSubfolder_(true)
    , saveCondition_(Always)
    , judgeResult_(true)
    , saveCount_(0)
{
    setDisplayName(toolName());
}

bool SaveImageTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    QElapsedTimer timer;
    timer.start();

    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = "输入图像为空";
        return false;
    }

    // 检查保存条件
    bool shouldSave = false;
    switch (saveCondition_) {
    case Always:
        shouldSave = true;
        break;
    case OnlyOK:
        shouldSave = judgeResult_;
        break;
    case OnlyNG:
        shouldSave = !judgeResult_;
        break;
    }

    if (!shouldSave) {
        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("saved", false);
        output.setValue("reason", "条件不满足");
        setStatusText("跳过保存 (条件不满足)");
        return true;
    }

    // 生成保存路径
    QString targetDir = saveDir_;

    if (createDateSubfolder_) {
        QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        targetDir = QDir(saveDir_).filePath(dateStr);
    }

    if (!ensureDirectoryExists(targetDir)) {
        output.success = false;
        output.errorMessage = QString("创建目录失败: %1").arg(targetDir);
        emit saveError(output.errorMessage);
        return false;
    }

    // 生成文件名
    QString filename = generateFilename();
    QString fullPath = QDir(targetDir).filePath(filename);

    // 保存图像
    std::vector<int> params;
    if (format_ == JPEG) {
        params.push_back(cv::IMWRITE_JPEG_QUALITY);
        params.push_back(jpegQuality_);
    } else if (format_ == PNG) {
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(3);  // 中等压缩
    }

    bool saved = cv::imwrite(fullPath.toStdString(), input->mat(), params);

    if (saved) {
        saveCount_++;
        lastSavedPath_ = fullPath;

        output.success = true;
        output.executionTime = timer.elapsed();
        output.setValue("saved", true);
        output.setValue("path", fullPath);
        output.setValue("count", saveCount_);

        setStatusText(QString("已保存: %1").arg(filename));

        LOG_DEBUG(QString("图像已保存: %1").arg(fullPath));
        emit imageSaved(fullPath);
    } else {
        output.success = false;
        output.errorMessage = QString("保存图像失败: %1").arg(fullPath);
        emit saveError(output.errorMessage);
        LOG_ERROR(output.errorMessage);
    }

    // 传递输入图像到输出
    output.outputImage = input;

    return saved;
}

QString SaveImageTool::generateFilename()
{
    QString extension = getFormatExtension();
    QString filename;

    switch (namingMode_) {
    case Timestamp: {
        QDateTime now = QDateTime::currentDateTime();
        filename = QString("%1_%2.%3")
                  .arg(prefix_)
                  .arg(now.toString("yyyyMMdd_HHmmss_zzz"))
                  .arg(extension);
        break;
    }
    case Sequential: {
        filename = QString("%1_%2.%3")
                  .arg(prefix_)
                  .arg(saveCount_ + 1, 6, 10, QChar('0'))
                  .arg(extension);
        break;
    }
    case Custom: {
        QDateTime now = QDateTime::currentDateTime();
        filename = QString("%1_%2_%3.%4")
                  .arg(prefix_)
                  .arg(now.toString("yyyyMMdd_HHmmss"))
                  .arg(saveCount_ + 1, 4, 10, QChar('0'))
                  .arg(extension);
        break;
    }
    }

    return filename;
}

QString SaveImageTool::getFormatExtension() const
{
    switch (format_) {
    case PNG:  return "png";
    case JPEG: return "jpg";
    case BMP:  return "bmp";
    case TIFF: return "tiff";
    default:   return "png";
    }
}

bool SaveImageTool::ensureDirectoryExists(const QString& path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}

void SaveImageTool::setSaveDirectory(const QString& dir)
{
    if (saveDir_ != dir) {
        saveDir_ = dir;
        emit paramChanged();
    }
}

void SaveImageTool::setImageFormat(ImageFormat format)
{
    if (format_ != format) {
        format_ = format;
        emit paramChanged();
    }
}

void SaveImageTool::setNamingMode(NamingMode mode)
{
    if (namingMode_ != mode) {
        namingMode_ = mode;
        emit paramChanged();
    }
}

void SaveImageTool::setPrefix(const QString& prefix)
{
    if (prefix_ != prefix) {
        prefix_ = prefix;
        emit paramChanged();
    }
}

void SaveImageTool::setJpegQuality(int quality)
{
    quality = qBound(1, quality, 100);
    if (jpegQuality_ != quality) {
        jpegQuality_ = quality;
        emit paramChanged();
    }
}

void SaveImageTool::setCreateDateSubfolder(bool create)
{
    if (createDateSubfolder_ != create) {
        createDateSubfolder_ = create;
        emit paramChanged();
    }
}

void SaveImageTool::setSaveCondition(SaveCondition condition)
{
    if (saveCondition_ != condition) {
        saveCondition_ = condition;
        emit paramChanged();
    }
}

QJsonObject SaveImageTool::serializeParams() const
{
    QJsonObject json;
    json["saveDir"] = saveDir_;
    json["format"] = static_cast<int>(format_);
    json["namingMode"] = static_cast<int>(namingMode_);
    json["prefix"] = prefix_;
    json["jpegQuality"] = jpegQuality_;
    json["createDateSubfolder"] = createDateSubfolder_;
    json["saveCondition"] = static_cast<int>(saveCondition_);
    return json;
}

void SaveImageTool::deserializeParams(const QJsonObject& json)
{
    saveDir_ = json.value("saveDir").toString("./saved_images");
    format_ = static_cast<ImageFormat>(json.value("format").toInt(0));
    namingMode_ = static_cast<NamingMode>(json.value("namingMode").toInt(0));
    prefix_ = json.value("prefix").toString("img");
    jpegQuality_ = json.value("jpegQuality").toInt(95);
    createDateSubfolder_ = json.value("createDateSubfolder").toBool(true);
    saveCondition_ = static_cast<SaveCondition>(json.value("saveCondition").toInt(0));
}

} // namespace Algorithm
} // namespace VisionForge
