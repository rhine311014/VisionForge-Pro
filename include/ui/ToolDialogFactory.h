/**
 * @file ToolDialogFactory.h
 * @brief 工具对话框工厂
 * @details 插件化UI系统核心，管理工具对话框的注册和创建
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/VisionTool.h"
#include "base/ImageData.h"
#include <QDialog>
#include <QMap>
#include <QString>
#include <QCoreApplication>
#include <functional>

namespace VisionForge {
namespace UI {

/**
 * @brief 工具对话框创建器函数类型
 * @param tool 工具实例
 * @param parent 父窗口
 * @return 对话框实例
 */
using ToolDialogCreator = std::function<QDialog*(Algorithm::VisionTool* tool, QWidget* parent)>;

/**
 * @class ToolDialogFactory
 * @brief 工具对话框工厂类
 *
 * 功能：
 * - 注册工具对话框创建器
 * - 根据工具类型创建对话框
 * - 支持运行时动态注册
 * - 消除MainWindow中的硬编码if-else链
 *
 * 使用示例：
 * @code
 * // 获取工具对话框
 * QDialog* dialog = ToolDialogFactory::instance().createDialog(tool, this);
 * if (dialog) {
 *     // 设置图像（如果支持）
 *     ToolDialogFactory::instance().setDialogImage(dialog, currentImage);
 *     dialog->exec();
 *     delete dialog;
 * }
 * @endcode
 */
class ToolDialogFactory {
    Q_DECLARE_TR_FUNCTIONS(ToolDialogFactory)

public:
    /**
     * @brief 获取工厂单例
     */
    static ToolDialogFactory& instance();

    /**
     * @brief 注册工具对话框
     * @param type 工具类型
     * @param creator 对话框创建函数
     */
    void registerDialog(Algorithm::VisionTool::ToolType type, ToolDialogCreator creator);

    /**
     * @brief 创建工具对话框
     * @param tool 工具实例
     * @param parent 父窗口
     * @return 对话框实例（调用者负责管理生命周期），如果未注册返回nullptr
     */
    QDialog* createDialog(Algorithm::VisionTool* tool, QWidget* parent = nullptr);

    /**
     * @brief 检查工具是否有注册的对话框
     * @param type 工具类型
     */
    bool hasDialog(Algorithm::VisionTool::ToolType type) const;

    /**
     * @brief 获取已注册的对话框数量
     */
    int registeredCount() const { return dialogs_.size(); }

    /**
     * @brief 尝试设置对话框的图像
     * @details 通过Qt元对象系统调用setImage或setPreviewImage方法
     * @param dialog 对话框实例
     * @param image 图像数据
     * @return 是否成功设置
     */
    static bool setDialogImage(QDialog* dialog, Base::ImageData::Ptr image);

private:
    ToolDialogFactory();
    ~ToolDialogFactory() = default;

    // 禁止拷贝
    ToolDialogFactory(const ToolDialogFactory&) = delete;
    ToolDialogFactory& operator=(const ToolDialogFactory&) = delete;

    /**
     * @brief 注册内置工具对话框
     */
    void registerBuiltInDialogs();

    QMap<Algorithm::VisionTool::ToolType, ToolDialogCreator> dialogs_;
};

/**
 * @brief 工具对话框注册辅助宏
 *
 * 在对话框类的cpp文件中使用此宏自动注册对话框。
 *
 * 用法示例：
 * @code
 * REGISTER_TOOL_DIALOG(VisionTool::Gray, GrayTool, GrayToolDialog)
 * @endcode
 *
 * @param Type 工具类型枚举值
 * @param ToolClass 工具类名称
 * @param DialogClass 对话框类名称
 */
#define REGISTER_TOOL_DIALOG(Type, ToolClass, DialogClass) \
    namespace { \
        struct DialogClass##Registrar { \
            DialogClass##Registrar() { \
                VisionForge::UI::ToolDialogFactory::instance().registerDialog( \
                    Type, \
                    [](VisionForge::Algorithm::VisionTool* tool, QWidget* parent) \
                        -> QDialog* { \
                        auto* typedTool = dynamic_cast<VisionForge::Algorithm::ToolClass*>(tool); \
                        if (!typedTool) return nullptr; \
                        return new DialogClass(typedTool, parent); \
                    } \
                ); \
            } \
        }; \
        static DialogClass##Registrar g_##DialogClass##Registrar; \
    }

} // namespace UI
} // namespace VisionForge
