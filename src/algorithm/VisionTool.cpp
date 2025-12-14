/**
 * @file VisionTool.cpp
 * @brief 视觉工具基类实现
 */

#include "algorithm/VisionTool.h"

namespace VisionForge {
namespace Algorithm {

VisionTool::VisionTool(QObject* parent)
    : QObject(parent)
    , enabled_(true)
    , displayName_("未命名工具")
{
}

QIcon VisionTool::toolIcon() const
{
    // 默认图标（可以后续添加）
    return QIcon();
}

void VisionTool::setEnabled(bool enabled)
{
    if (enabled_ != enabled) {
        enabled_ = enabled;
        emit paramChanged();
    }
}

void VisionTool::setDisplayName(const QString& name)
{
    if (displayName_ != name) {
        displayName_ = name;
        emit paramChanged();
    }
}

void VisionTool::setDebugImage(const Base::ImageData::Ptr& image)
{
    debugImage_ = image;
    emit debugImageUpdated();
}

void VisionTool::setStatusText(const QString& text)
{
    if (statusText_ != text) {
        statusText_ = text;
        emit statusTextChanged(text);
    }
}

} // namespace Algorithm
} // namespace VisionForge
