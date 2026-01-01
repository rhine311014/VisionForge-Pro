/**
 * @file OperatorToolBar.h
 * @brief 操作员底部工具栏
 * @details 提供快速访问常用功能的大图标按钮栏
 * @author VisionForge Team
 * @date 2025-12-31
 */

#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>

namespace VisionForge {
namespace UI {

/**
 * @class OperatorToolBar
 * @brief 操作员底部工具栏
 *
 * 功能特点：
 * - 大图标按钮，便于触屏操作
 * - 可配置的按钮显示/隐藏
 * - 支持自定义图标和颜色
 * - 提供常用操作的快捷入口
 */
class OperatorToolBar : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 按钮类型枚举
     */
    enum ButtonType {
        Camera,         // 相机
        Communication,  // 通信
        Product,        // 产品
        System,         // 系统
        Options,        // 选项
        Preview,        // 图像预览
        Run,            // 运行
        Login,          // 登录
        Exit,           // 退出
        Hide,           // 隐藏
        Station         // 工位
    };

    explicit OperatorToolBar(QWidget* parent = nullptr);
    ~OperatorToolBar() override;

    /**
     * @brief 设置按钮可见性
     * @param type 按钮类型
     * @param visible 是否可见
     */
    void setButtonVisible(ButtonType type, bool visible);

    /**
     * @brief 设置按钮启用状态
     * @param type 按钮类型
     * @param enabled 是否启用
     */
    void setButtonEnabled(ButtonType type, bool enabled);

    /**
     * @brief 设置按钮文本
     * @param type 按钮类型
     * @param text 按钮文本
     */
    void setButtonText(ButtonType type, const QString& text);

    /**
     * @brief 设置按钮图标大小
     * @param size 图标尺寸
     */
    void setIconSize(int size);

    /**
     * @brief 获取当前图标大小
     */
    int iconSize() const { return iconSize_; }

    /**
     * @brief 设置运行状态
     * @param running 是否运行中
     */
    void setRunningState(bool running);

    /**
     * @brief 设置登录状态
     * @param loggedIn 是否已登录
     * @param username 用户名
     */
    void setLoginState(bool loggedIn, const QString& username = QString());

signals:
    void cameraClicked();
    void communicationClicked();
    void productClicked();
    void systemClicked();
    void optionsClicked();
    void previewClicked();
    void runClicked();
    void loginClicked();
    void exitClicked();
    void hideClicked();
    void stationClicked();

private:
    void setupUI();
    void setupStyles();
    QPushButton* createToolButton(const QString& iconName, const QString& text, ButtonType type);
    void applyButtonStyle(QPushButton* btn, bool highlight = false);

private:
    QHBoxLayout* layout_;
    QMap<ButtonType, QPushButton*> buttons_;
    int iconSize_;
    bool isRunning_;
    QString normalStyle_;
    QString highlightStyle_;
    QString dangerStyle_;
};

} // namespace UI
} // namespace VisionForge
