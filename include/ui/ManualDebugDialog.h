/**
 * @file ManualDebugDialog.h
 * @brief 手动调试中心
 *
 * 参考 VisionASM DlgManualSearch 设计
 * 集成位置移动、采集、定位的一站式调试
 */

#ifndef VISIONFORGE_UI_MANUAL_DEBUG_DIALOG_H
#define VISIONFORGE_UI_MANUAL_DEBUG_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QTimer>
#include <memory>

namespace VisionForge {

namespace Core {
struct StationConfig;
class MultiStationManager;
}

namespace HAL {
class ICamera;
}

namespace UI {

class HalconImageViewer;

/**
 * @brief 搜索结果结构
 */
struct SearchResult {
    double posX = 0.0;          ///< X坐标
    double posY = 0.0;          ///< Y坐标
    double angle = 0.0;         ///< 角度
    double score = 0.0;         ///< 匹配分数
    double timeMs = 0.0;        ///< 耗时(ms)
    bool success = false;       ///< 是否成功
    QString message;            ///< 结果消息
};

/**
 * @brief 手动调试中心对话框
 *
 * 功能：
 * - 工位/相机/位置选择
 * - 单次/连续拍照
 * - 轴控制 (Jog)
 * - 定位测试
 * - 模板示教
 * - 结果显示
 */
class ManualDebugDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     */
    explicit ManualDebugDialog(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~ManualDebugDialog() override;

    /**
     * @brief 设置当前工位
     */
    void setStation(Core::StationConfig* station);

    /**
     * @brief 获取当前工位
     */
    Core::StationConfig* currentStation() const { return currentStation_; }

signals:
    /**
     * @brief 请求移动轴
     * @param axis 轴号 (0=X, 1=Y, 2=D)
     * @param direction 方向 (+1/-1)
     * @param step 步进量
     */
    void axisJogRequested(int axis, int direction, double step);

    /**
     * @brief 请求移动到位置
     * @param positionId 位置ID
     */
    void moveToPositionRequested(const QString& positionId);

    /**
     * @brief 定位执行完成
     */
    void searchCompleted(const SearchResult& result);

    /**
     * @brief 模板示教请求
     */
    void teachModelRequested(const QString& positionId);

private slots:
    // 选择变化
    void onStationChanged(int index);
    void onCameraChanged(int index);
    void onPositionChanged(int index);

    // 采集控制
    void onSingleCapture();
    void onContinuousCapture();
    void onStopCapture();
    void doContinuousCapture();

    // 轴控制
    void onXPlus();
    void onXMinus();
    void onYPlus();
    void onYMinus();
    void onDPlus();
    void onDMinus();
    void onMoveToPosition();

    // 定位测试
    void onRunSearch();
    void onTeachModel();

    // 结果更新
    void updateResults(const SearchResult& result);
    void updatePlatformPos(double x, double y, double d);

    // 刷新
    void refreshStations();
    void refreshCameras();
    void refreshPositions();

private:
    /**
     * @brief 初始化UI
     */
    void setupUI();

    /**
     * @brief 创建选择区域
     */
    QGroupBox* createSelectionGroup();

    /**
     * @brief 创建采集控制区域
     */
    QGroupBox* createCaptureGroup();

    /**
     * @brief 创建轴控制区域
     */
    QGroupBox* createAxisControlGroup();

    /**
     * @brief 创建定位测试区域
     */
    QGroupBox* createSearchGroup();

    /**
     * @brief 创建结果显示区域
     */
    QGroupBox* createResultGroup();

    /**
     * @brief 创建平台坐标显示区域
     */
    QGroupBox* createPlatformPosGroup();

    /**
     * @brief 建立信号连接
     */
    void setupConnections();

    /**
     * @brief 获取当前选中的相机
     */
    HAL::ICamera* getCurrentCamera();

    /**
     * @brief 执行单次采集
     */
    void doCapture();

    // ========== UI 组件 ==========

    // 工位/相机/位置选择
    QComboBox* comboStation_;       ///< 工位选择
    QComboBox* comboCamera_;        ///< 相机选择
    QComboBox* comboPosition_;      ///< 位置选择

    // 图像显示
    HalconImageViewer* imageViewer_;

    // 采集控制
    QPushButton* btnSingleCapture_; ///< 单次拍照
    QPushButton* btnContinuous_;    ///< 连续拍照
    QPushButton* btnStopCapture_;   ///< 停止
    QTimer* continuousTimer_;       ///< 连续拍照定时器
    bool isContinuousMode_ = false; ///< 是否连续模式

    // 轴控制 (Jog)
    QGroupBox* groupAxisControl_;
    QPushButton* btnXPlus_;         ///< X轴+
    QPushButton* btnXMinus_;        ///< X轴-
    QPushButton* btnYPlus_;         ///< Y轴+
    QPushButton* btnYMinus_;        ///< Y轴-
    QPushButton* btnDPlus_;         ///< D轴+
    QPushButton* btnDMinus_;        ///< D轴-
    QDoubleSpinBox* spinJogStep_;   ///< 步进量
    QPushButton* btnMoveToPos_;     ///< 移动到位置

    // 定位测试
    QPushButton* btnRunSearch_;     ///< 执行定位
    QPushButton* btnTeachModel_;    ///< 示教模板

    // 结果显示
    QLabel* labelPosX_;             ///< X坐标
    QLabel* labelPosY_;             ///< Y坐标
    QLabel* labelAngle_;            ///< 角度
    QLabel* labelScore_;            ///< 匹配分数
    QLabel* labelTime_;             ///< 耗时
    QLabel* labelStatus_;           ///< 状态

    // 平台坐标显示
    QLabel* labelPlatX_;            ///< 平台X
    QLabel* labelPlatY_;            ///< 平台Y
    QLabel* labelPlatD_;            ///< 平台D

    // ========== 数据成员 ==========
    Core::StationConfig* currentStation_ = nullptr;
    QString currentCameraId_;
    QString currentPositionId_;
};

} // namespace UI
} // namespace VisionForge

#endif // VISIONFORGE_UI_MANUAL_DEBUG_DIALOG_H
