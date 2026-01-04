/**
 * @file IToolDialog.h
 * @brief 工具对话框抽象接口
 * @details 定义工具对话框的标准接口，支持插件化UI系统
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "base/ImageData.h"
#include <QDialog>
#include <functional>
#include <memory>

namespace VisionForge {
namespace UI {

/**
 * @brief 工具对话框抽象基类
 *
 * 所有工具配置对话框都应继承此接口。
 * 提供统一的图像设置、预览回调等功能。
 */
class IToolDialog : public QDialog {
    Q_OBJECT

public:
    explicit IToolDialog(QWidget* parent = nullptr) : QDialog(parent) {}
    virtual ~IToolDialog() = default;

    /**
     * @brief 设置预览图像
     * @param image 当前图像
     */
    virtual void setImage(Base::ImageData::Ptr image) { Q_UNUSED(image); }

    /**
     * @brief 设置预览图像（别名，兼容旧接口）
     */
    virtual void setPreviewImage(Base::ImageData::Ptr image) { setImage(image); }

    /**
     * @brief 获取关联的工具
     * @return 工具指针
     */
    virtual Algorithm::VisionTool* tool() const = 0;

    /**
     * @brief 获取工具类型
     */
    virtual Algorithm::VisionTool::ToolType toolType() const {
        return tool() ? tool()->toolType() : Algorithm::VisionTool::Custom;
    }

signals:
    /**
     * @brief 请求预览信号
     * @details 当用户修改参数后，发射此信号请求预览
     */
    void previewRequested();

    /**
     * @brief 参数已更新信号
     */
    void parametersChanged();
};

/**
 * @brief 工具对话框智能指针类型
 */
using IToolDialogPtr = std::unique_ptr<IToolDialog>;

} // namespace UI
} // namespace VisionForge
