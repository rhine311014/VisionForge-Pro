# VisionForge Pro 多工位多相机综合改进方案

> 整合 `改造建议生成.md` 与 `MULTI_STATION_CAMERA_DESIGN.md` 的完整实施方案

---

## 一、现状痛点分析

### 1.1 核心问题

| 痛点 | 具体表现 | 影响程度 |
|------|----------|----------|
| **扩展性差** | 增加工位/相机需修改大量代码 | ⭐⭐⭐⭐⭐ |
| **调试繁琐** | 缺乏集中调试界面，切换相机/位置不便 | ⭐⭐⭐⭐⭐ |
| **配置僵化** | 参数硬编码或分散管理 | ⭐⭐⭐⭐ |
| **显示单一** | MainWindow 只管理单相机显示 | ⭐⭐⭐⭐ |
| **标定复杂** | 多位置标定容易漏做或顺序错误 | ⭐⭐⭐ |

### 1.2 现有架构优势（需保留）

| 模块 | 状态 | 说明 |
|------|------|------|
| CameraManager | ✅ 完善 | 支持多相机分组、同步采集、硬件触发 |
| WorkStation | ✅ 完善 | 支持多位置、多种对位方式、迭代对位 |
| PlatformConfig | ✅ 完善 | 支持16种平台类型 |
| RecipeManager | ✅ 完善 | 独立方案管理、模板库 |

---

## 二、改进目标

### 2.1 功能目标

1. **多相机同时显示** - 支持 1/2/4/6/8 路画面
2. **工位快速切换** - 一键切换，F1-F6 快捷键
3. **位置-相机直观关联** - 可视化配置表格
4. **手动调试中心** - 集成位置移动、采集、定位
5. **标定向导** - 分步引导，防止遗漏
6. **通信协议标准化** - 指令携带工位/相机/位置ID

### 2.2 设计原则

- **配置驱动** - 通过 JSON 配置动态构建，减少重编译
- **容器化管理** - 使用 `std::vector`/`QMap` 替代硬编码变量
- **增量改进** - 不破坏现有功能，逐步升级

---

## 三、核心架构设计

### 3.1 层级关系

```
System (系统)
  └── Station (工位) [1-6个]
        ├── Platform (平台配置)
        ├── Cameras (相机列表) [1-8个]
        └── Positions (位置列表) [1-8个]
              └── PositionCameraBinding (位置-相机绑定)
```

### 3.2 核心数据结构

#### 3.2.1 位置-相机绑定 (PositionCameraBinding)

```cpp
// include/core/PositionCameraBinding.h

namespace VisionForge::Core {

/**
 * @brief 位置-相机绑定信息
 * 借鉴 VisionASM CPosInfo，简洁高效
 */
struct PositionCameraBinding {
    // 基本信息
    QString positionId;         // 位置ID (如 "pos0")
    QString positionName;       // 位置名称 (如 "Pos0-Target")
    bool enabled = true;        // 是否启用

    // 相机关联
    int cameraIndex = 0;        // 相机索引 (0-7)
    QString cameraId;           // 相机ID (CameraManager中的ID)

    // 显示配置
    int displayIndex = -1;      // 多画面显示位置 (-1=自动分配)

    // 采集配置
    int captureOrder = 0;       // 顺序采集时的顺序
    bool useHardwareTrigger = false;

    // 平台坐标 (拍照位置)
    double platX = 0.0;
    double platY = 0.0;
    double platD = 0.0;

    // 序列化
    QJsonObject toJson() const;
    static PositionCameraBinding fromJson(const QJsonObject& json);
};

} // namespace VisionForge::Core
```

#### 3.2.2 工位配置 (StationConfig)

```cpp
// include/core/StationConfig.h

namespace VisionForge::Core {

/**
 * @brief 工位完整配置
 * 借鉴 VisionASM SysPlatformInfo，集中管理所有配置
 */
struct StationConfig {
    // ========== 基本信息 ==========
    QString id;                 // 工位ID (如 "WS0", "WS1")
    QString name;               // 工位名称 (如 "工位1-上料对位")
    int index = 0;              // 工位索引 (0-5)
    bool enabled = true;        // 是否启用

    // ========== 平台配置 ==========
    Platform::PlatformType platformType = Platform::PlatformType::XYD;
    QString platformConfigId;   // 关联的平台配置ID

    // ========== 相机配置 ==========
    int cameraNum = 2;          // 相机数量 (1-8)
    QVector<QString> cameraIds; // 相机ID列表

    // 相机共享
    bool shareCameraSystem = false;
    QString sharedFromStationId;

    // ========== 位置配置 ==========
    int positionNum = 2;        // 位置数量 (1-8)
    QVector<PositionCameraBinding> positionBindings;

    // ========== 显示配置 ==========
    enum DisplayMode {
        ByPosition = 0,     // 按位置显示
        ByCamera = 1        // 按相机显示
    };
    DisplayMode displayMode = ByPosition;

    enum LayoutMode {
        Layout_Auto = 0,
        Layout_1x1 = 1,
        Layout_1x2 = 2,
        Layout_2x2 = 4,
        Layout_2x3 = 6,
        Layout_2x4 = 8
    };
    LayoutMode displayLayout = Layout_Auto;

    // ========== 触发配置 ==========
    bool useHardwareTrigger = false;
    QString triggerSource;      // "GPIO" / "PLC" / "Software"

    // ========== 通信配置 ==========
    QString plcAddress;         // PLC地址
    int plcPort = 502;          // PLC端口

    // ========== 辅助方法 ==========
    QString getCameraIdForPosition(int posIndex) const;
    QVector<int> getPositionsForCamera(const QString& cameraId) const;
    ICamera* getCamera(int positionIndex) const;

    // 序列化
    QJsonObject toJson() const;
    static StationConfig fromJson(const QJsonObject& json);
};

} // namespace VisionForge::Core
```

#### 3.2.3 多工位管理器 (MultiStationManager)

```cpp
// include/core/MultiStationManager.h

namespace VisionForge::Core {

/**
 * @brief 多工位管理器 (单例)
 * 统一管理所有工位配置和切换
 */
class MultiStationManager : public QObject {
    Q_OBJECT

public:
    static MultiStationManager& instance();
    static constexpr int MAX_STATION_NUM = 6;

    // ========== 工位管理 ==========
    QString createStation(const QString& name, Platform::PlatformType type);
    bool removeStation(const QString& stationId);
    StationConfig* getStation(const QString& stationId);
    StationConfig* getStationByIndex(int index);
    QVector<StationConfig*> getAllStations();
    int getStationCount() const;

    // ========== 当前工位 ==========
    bool setCurrentStation(const QString& stationId);
    bool setCurrentStationByIndex(int index);
    StationConfig* currentStation();
    int currentStationIndex() const;

    // ========== 位置-相机绑定 ==========
    bool addPositionBinding(const QString& stationId, const PositionCameraBinding& binding);
    bool updatePositionBinding(const QString& stationId, const PositionCameraBinding& binding);
    bool removePositionBinding(const QString& stationId, const QString& positionId);

    // ========== 配置持久化 ==========
    bool saveConfig(const QString& filePath = QString());
    bool loadConfig(const QString& filePath = QString());
    QString getConfigDirectory() const;

    // ========== 与现有模块同步 ==========
    void syncToWorkStation(const QString& stationId);
    void syncToCameraManager(const QString& stationId);

signals:
    void stationCreated(const QString& stationId);
    void stationRemoved(const QString& stationId);
    void currentStationChanged(const QString& stationId, int index);
    void stationConfigChanged(const QString& stationId);

private:
    MultiStationManager(QObject* parent = nullptr);

    QMap<QString, StationConfig> stations_;
    QString currentStationId_;
};

} // namespace VisionForge::Core
```

---

## 四、UI 组件设计

### 4.1 多相机显示视图 (MultiCameraView)

```cpp
// include/ui/MultiCameraView.h

namespace VisionForge::UI {

/**
 * @brief 多相机显示视图
 * 支持 1/2/4/6/8 路画面同时显示
 */
class MultiCameraView : public QWidget {
    Q_OBJECT

public:
    explicit MultiCameraView(QWidget* parent = nullptr);

    // 布局控制
    void setLayoutMode(Core::StationConfig::LayoutMode mode);
    void setAutoLayout(int viewCount);  // 根据数量自动选择布局

    // 关联工位
    void setStation(Core::StationConfig* station);

    // 更新画面
    void updateImage(int viewIndex, const HalconCpp::HImage& image);
    void updateImage(const QString& positionId, const HalconCpp::HImage& image);
    void updateAllImages(const QMap<QString, HalconCpp::HImage>& images);

    // 选中控制
    void setSelectedView(int index);
    int selectedView() const;
    HalconImageViewer* getViewer(int index);
    HalconImageViewer* getSelectedViewer();

    // 最大化
    void maximizeView(int index);
    void restoreLayout();
    bool isMaximized() const;

signals:
    void viewSelected(int index, const QString& positionId);
    void viewDoubleClicked(int index);  // 双击最大化/还原

private:
    void setupLayout();
    void updateLayoutFromStation();
    void clearAllViewers();

    QGridLayout* gridLayout_;
    QVector<HalconImageViewer*> viewers_;
    Core::StationConfig* currentStation_ = nullptr;
    int selectedIndex_ = 0;
    bool maximized_ = false;
    int maximizedIndex_ = -1;
};

} // namespace VisionForge::UI
```

### 4.2 工位切换工具栏 (StationSwitchBar)

```cpp
// include/ui/StationSwitchBar.h

namespace VisionForge::UI {

/**
 * @brief 工位切换工具栏
 * 显示所有工位按钮，支持快速切换
 */
class StationSwitchBar : public QWidget {
    Q_OBJECT

public:
    explicit StationSwitchBar(QWidget* parent = nullptr);

    void refreshStations();
    void setCurrentStation(int index);
    void setShowAllButton(bool show);
    void setShowStationStatus(bool show);  // 显示OK/NG状态

    // 状态更新
    void setStationStatus(int index, bool ok);  // 设置工位状态

signals:
    void stationSelected(int index);
    void showAllStations();

private:
    void setupUI();
    void setupShortcuts();  // F1-F6 快捷键

    QHBoxLayout* layout_;
    QPushButton* btnShowAll_;
    QVector<QPushButton*> stationBtns_;
    QButtonGroup* buttonGroup_;
    QVector<QLabel*> statusLabels_;  // 状态指示灯
};

} // namespace VisionForge::UI
```

### 4.3 手动调试中心 (ManualDebugDialog) - **新增核心功能**

```cpp
// include/ui/ManualDebugDialog.h

namespace VisionForge::UI {

/**
 * @brief 手动调试中心
 * 参考 VisionASM DlgManualSearch 设计
 * 集成位置移动、采集、定位的一站式调试
 */
class ManualDebugDialog : public QDialog {
    Q_OBJECT

public:
    explicit ManualDebugDialog(QWidget* parent = nullptr);

    // 设置当前工位
    void setStation(Core::StationConfig* station);

private:
    void setupUI();
    void setupConnections();

    // ========== UI 组件 ==========

    // 工位/相机/位置选择
    QComboBox* comboStation_;       // 工位选择
    QComboBox* comboCamera_;        // 相机选择
    QComboBox* comboPosition_;      // 位置选择

    // 图像显示
    HalconImageViewer* imageViewer_;

    // 采集控制
    QPushButton* btnSingleCapture_; // 单次拍照
    QPushButton* btnContinuous_;    // 连续拍照
    QPushButton* btnStopCapture_;   // 停止

    // 轴控制 (Jog)
    QGroupBox* groupAxisControl_;
    QPushButton* btnXPlus_, *btnXMinus_;  // X轴 +/-
    QPushButton* btnYPlus_, *btnYMinus_;  // Y轴 +/-
    QPushButton* btnDPlus_, *btnDMinus_;  // D轴 +/-
    QDoubleSpinBox* spinJogStep_;         // 步进量

    // 定位测试
    QPushButton* btnRunSearch_;     // 执行定位
    QPushButton* btnTeachModel_;    // 示教模板

    // 结果显示
    QLabel* labelPosX_, *labelPosY_, *labelAngle_;  // 定位坐标
    QLabel* labelScore_;            // 匹配分数
    QLabel* labelTime_;             // 耗时

    // 平台坐标显示
    QLabel* labelPlatX_, *labelPlatY_, *labelPlatD_;

    // ========== 槽函数 ==========
private slots:
    void onStationChanged(int index);
    void onCameraChanged(int index);
    void onPositionChanged(int index);

    void onSingleCapture();
    void onContinuousCapture();
    void onStopCapture();

    void onAxisJog(int axis, int direction);  // axis: 0=X,1=Y,2=D; direction: +1/-1
    void onMoveToPosition();         // 移动到选中位置

    void onRunSearch();
    void onTeachModel();

    void updateResults(const SearchResult& result);
    void updatePlatformPos(double x, double y, double d);
};

} // namespace VisionForge::UI
```

### 4.4 标定向导 (CalibrationWizard) - **新增核心功能**

```cpp
// include/ui/CalibrationWizard.h

namespace VisionForge::UI {

/**
 * @brief 多位置标定向导
 * 分步引导完成多位置标定，防止遗漏
 */
class CalibrationWizard : public QWizard {
    Q_OBJECT

public:
    explicit CalibrationWizard(Core::StationConfig* station, QWidget* parent = nullptr);

private:
    void setupPages();

    Core::StationConfig* station_;

    // 向导页面
    QWizardPage* createIntroPage();          // 介绍页
    QWizardPage* createPositionSelectPage(); // 选择要标定的位置
    QWizardPage* createCalibrationPage(int posIndex); // 单位置标定页
    QWizardPage* createSummaryPage();        // 完成总结页
};

/**
 * @brief 单位置标定页面
 */
class CalibPositionPage : public QWizardPage {
    Q_OBJECT

public:
    CalibPositionPage(Core::StationConfig* station, int posIndex, QWidget* parent = nullptr);

    bool isComplete() const override;

private:
    void setupUI();

    Core::StationConfig* station_;
    int posIndex_;

    // UI
    QLabel* labelInstruction_;      // 操作指引
    HalconImageViewer* imageViewer_;
    QPushButton* btnCapture_;       // 拍照
    QPushButton* btnMoveToPos_;     // 移动到位置
    QCheckBox* checkCompleted_;     // 完成确认

    // 标定点列表 (9宫格)
    QTableWidget* tableCalibPoints_;
    int completedPoints_ = 0;
};

} // namespace VisionForge::UI
```

### 4.5 位置-相机配置对话框 (PositionCameraConfigDialog)

```cpp
// include/ui/PositionCameraConfigDialog.h

namespace VisionForge::UI {

/**
 * @brief 位置-相机配置对话框
 * 可视化配置每个位置使用哪个相机
 */
class PositionCameraConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit PositionCameraConfigDialog(Core::StationConfig* station, QWidget* parent = nullptr);

    QVector<Core::PositionCameraBinding> getBindings() const;

private:
    void setupUI();
    void loadFromStation();
    void updatePreview();

    Core::StationConfig* station_;

    // 位置数量
    QSpinBox* spinPositionNum_;

    // 配置表格
    // | 位置 | 名称 | 启用 | 相机 | 显示位置 | 采集顺序 |
    QTableWidget* tableBindings_;

    // 预览区域
    QLabel* previewLabel_;

    // 快速配置
    QPushButton* btnAutoConfig_;    // 自动配置 (位置0-1用相机0, 位置2-3用相机1...)
    QPushButton* btnClearAll_;      // 清空配置
};

} // namespace VisionForge::UI
```

---

## 五、通信协议标准化

### 5.1 指令格式设计

所有 PLC 指令必须携带工位/相机/位置标识：

```cpp
// include/comm/StationCommand.h

namespace VisionForge::Comm {

/**
 * @brief 工位指令结构
 * 统一的指令格式，支持精确寻址
 */
struct StationCommand {
    // 寻址信息
    int stationId = -1;     // 工位ID (-1=当前工位)
    int cameraId = -1;      // 相机ID (-1=所有相机)
    int positionId = -1;    // 位置ID (-1=所有位置)

    // 指令类型
    enum CommandType {
        CMD_CAPTURE,        // 采集
        CMD_SEARCH,         // 定位
        CMD_CALIBRATE,      // 标定
        CMD_MOVE_TO_POS,    // 移动到位置
        CMD_OUTPUT_RESULT,  // 输出结果
    };
    CommandType type;

    // 指令参数
    QVariantMap params;

    // 序列化为通信格式
    QByteArray toBytes() const;
    static StationCommand fromBytes(const QByteArray& data);
};

/**
 * @brief 指令分发器
 */
class CommandDispatcher : public QObject {
    Q_OBJECT

public:
    static CommandDispatcher& instance();

    // 分发指令到对应工位
    void dispatch(const StationCommand& cmd);

signals:
    void commandReceived(const StationCommand& cmd);
    void commandCompleted(const StationCommand& cmd, bool success);
};

} // namespace VisionForge::Comm
```

### 5.2 PLC 通信示例

```cpp
// 接收 PLC 指令并分发
void PLCHandler::onDataReceived(const QByteArray& data)
{
    auto cmd = StationCommand::fromBytes(data);

    // 日志记录
    LOG_INFO(QString("收到指令: Station=%1, Camera=%2, Position=%3, Type=%4")
             .arg(cmd.stationId)
             .arg(cmd.cameraId)
             .arg(cmd.positionId)
             .arg(cmd.type));

    // 分发到对应工位处理
    CommandDispatcher::instance().dispatch(cmd);
}

// 工位处理指令
void WorkStation::handleCommand(const StationCommand& cmd)
{
    switch (cmd.type) {
    case StationCommand::CMD_CAPTURE:
        if (cmd.positionId >= 0) {
            capturePosition(cmd.positionId);
        } else {
            captureAllPositions();
        }
        break;

    case StationCommand::CMD_SEARCH:
        runSearch(cmd.positionId);
        break;

    // ...
    }
}
```

---

## 六、配置文件结构

### 6.1 目录结构

```
VisionForgeConfig/
├── global.json                     # 全局配置
├── cameras.json                    # 相机配置
├── stations/                       # 工位配置目录
│   ├── stations.json               # 工位列表索引
│   ├── WS0/                        # 工位0
│   │   ├── station.json            # 工位配置
│   │   ├── platform.json           # 平台配置
│   │   └── calibration/            # 标定数据
│   │       ├── pos0_calib.json
│   │       └── pos1_calib.json
│   ├── WS1/                        # 工位1
│   └── WS2/                        # 工位2
└── recipes/                        # 方案目录
    └── ProductA/
        ├── ProductA.vfr
        └── models/
            ├── WS0/                # 工位0的模板
            │   ├── Pos0_Target/
            │   └── Pos1_Object/
            └── WS1/                # 工位1的模板
```

### 6.2 stations.json 示例

```json
{
    "version": "1.0",
    "stationCount": 2,
    "currentStation": "WS0",
    "stations": [
        {
            "id": "WS0",
            "name": "工位1-上料对位",
            "configPath": "WS0/station.json"
        },
        {
            "id": "WS1",
            "name": "工位2-下料对位",
            "configPath": "WS1/station.json"
        }
    ]
}
```

### 6.3 station.json 完整示例

```json
{
    "id": "WS0",
    "name": "工位1-上料对位",
    "index": 0,
    "enabled": true,

    "platform": {
        "type": "XYD",
        "configId": "platform_xyd_001",
        "xRange": 500.0,
        "yRange": 500.0,
        "dRange": 360.0
    },

    "cameras": {
        "count": 2,
        "ids": ["cam_hikv_001", "cam_hikv_002"],
        "shared": false,
        "sharedFrom": null
    },

    "positions": {
        "count": 4,
        "bindings": [
            {
                "positionId": "pos0",
                "positionName": "Pos0-Target上",
                "enabled": true,
                "cameraIndex": 0,
                "cameraId": "cam_hikv_001",
                "displayIndex": 0,
                "captureOrder": 0,
                "platX": 100.0,
                "platY": 100.0,
                "platD": 0.0
            },
            {
                "positionId": "pos1",
                "positionName": "Pos1-Target下",
                "enabled": true,
                "cameraIndex": 0,
                "cameraId": "cam_hikv_001",
                "displayIndex": 1,
                "captureOrder": 1,
                "platX": 100.0,
                "platY": 200.0,
                "platD": 0.0
            },
            {
                "positionId": "pos2",
                "positionName": "Pos2-Object上",
                "enabled": true,
                "cameraIndex": 1,
                "cameraId": "cam_hikv_002",
                "displayIndex": 2,
                "captureOrder": 2,
                "platX": 300.0,
                "platY": 100.0,
                "platD": 0.0
            },
            {
                "positionId": "pos3",
                "positionName": "Pos3-Object下",
                "enabled": true,
                "cameraIndex": 1,
                "cameraId": "cam_hikv_002",
                "displayIndex": 3,
                "captureOrder": 3,
                "platX": 300.0,
                "platY": 200.0,
                "platD": 0.0
            }
        ]
    },

    "display": {
        "mode": "ByPosition",
        "layout": 4
    },

    "trigger": {
        "useHardware": false,
        "source": "software"
    },

    "communication": {
        "plcAddress": "192.168.1.100",
        "plcPort": 502
    }
}
```

---

## 七、MainWindow 集成

### 7.1 新增成员与槽函数

```cpp
// MainWindow.h 新增

class MainWindow : public QMainWindow {
    // ...现有代码...

private:
    // ========== 多工位多相机支持 ==========
    UI::MultiCameraView* multiCameraView_;
    UI::StationSwitchBar* stationSwitchBar_;
    Core::StationConfig* currentStation_ = nullptr;

private slots:
    // 工位切换
    void onStationChanged(int index);
    void onShowAllStations();

    // 多相机采集
    void onCaptureAllPositions();
    void onCaptureCurrentPosition();

    // 调试工具
    void onOpenManualDebug();       // 打开手动调试中心
    void onOpenCalibrationWizard(); // 打开标定向导
    void onConfigurePositionCamera(); // 位置-相机配置
};
```

### 7.2 菜单与工具栏

```cpp
void MainWindow::setupMultiStationMenus()
{
    // 工位菜单
    QMenu* stationMenu = menuBar()->addMenu("工位(&S)");

    // 工位切换 (F1-F6)
    for (int i = 0; i < 6; ++i) {
        QAction* act = stationMenu->addAction(
            QString("工位%1").arg(i + 1),
            [this, i]() { onStationChanged(i); }
        );
        act->setShortcut(QKeySequence(Qt::Key_F1 + i));
    }

    stationMenu->addSeparator();
    stationMenu->addAction("显示全部工位", this, &MainWindow::onShowAllStations, QKeySequence(Qt::Key_F7));

    stationMenu->addSeparator();
    stationMenu->addAction("工位配置...", this, &MainWindow::onConfigurePositionCamera);

    // 调试菜单
    QMenu* debugMenu = menuBar()->addMenu("调试(&D)");
    debugMenu->addAction("手动调试中心...", this, &MainWindow::onOpenManualDebug, QKeySequence("Ctrl+D"));
    debugMenu->addAction("标定向导...", this, &MainWindow::onOpenCalibrationWizard);

    // 采集菜单项
    debugMenu->addSeparator();
    debugMenu->addAction("采集当前位置", this, &MainWindow::onCaptureCurrentPosition, QKeySequence(Qt::Key_Space));
    debugMenu->addAction("采集所有位置", this, &MainWindow::onCaptureAllPositions, QKeySequence("Ctrl+Space"));
}
```

---

## 八、快捷键汇总

| 快捷键 | 功能 | 说明 |
|--------|------|------|
| F1-F6 | 切换工位1-6 | 快速切换 |
| F7 | 显示全部工位 | 总览模式 |
| Ctrl+1-8 | 选中画面1-8 | 多画面选择 |
| Space | 采集当前位置 | 单次采集 |
| Ctrl+Space | 采集所有位置 | 批量采集 |
| Tab | 切换到下一个画面 | 循环选择 |
| Enter | 最大化/还原画面 | 双击效果 |
| Ctrl+D | 打开手动调试中心 | 调试工具 |
| Esc | 退出最大化/关闭对话框 | 通用 |

---

## 九、实施路线图

### Phase 1: 核心数据结构 (2天)

- [ ] `PositionCameraBinding` 结构实现
- [ ] `StationConfig` 结构实现
- [ ] `MultiStationManager` 基础实现
- [ ] JSON 配置文件读写
- [ ] 单元测试

### Phase 2: 多相机显示 (3天)

- [ ] `MultiCameraView` 组件
- [ ] 布局切换 (1x1, 2x2, 2x3, 2x4)
- [ ] 单画面选中/最大化
- [ ] 与 CameraManager 集成

### Phase 3: 工位切换UI (2天)

- [ ] `StationSwitchBar` 组件
- [ ] MainWindow 集成
- [ ] 快捷键 F1-F6
- [ ] 状态指示

### Phase 4: 手动调试中心 (3天)

- [ ] `ManualDebugDialog` 完整实现
- [ ] 轴控制 Jog 功能
- [ ] 定位测试功能
- [ ] 与 MotionController 集成

### Phase 5: 标定向导 (2天)

- [ ] `CalibrationWizard` 实现
- [ ] 分步引导流程
- [ ] 标定数据保存

### Phase 6: 配置界面完善 (2天)

- [ ] `PositionCameraConfigDialog`
- [ ] 系统设置中的工位管理
- [ ] 配置导入/导出

### Phase 7: 通信协议集成 (2天)

- [ ] `StationCommand` 结构
- [ ] `CommandDispatcher` 分发器
- [ ] PLC 通信测试

### Phase 8: 测试与优化 (3天)

- [ ] 集成测试
- [ ] 现场验证
- [ ] 性能优化
- [ ] 文档完善

**总计: 约 19 个工作日**

---

## 十、风险与应对

| 风险 | 可能性 | 影响 | 应对措施 |
|------|--------|------|----------|
| 与现有模块冲突 | 中 | 高 | 增量改进，保留旧接口兼容 |
| 性能下降 | 低 | 中 | 多画面使用独立线程渲染 |
| 配置迁移困难 | 中 | 中 | 提供迁移工具和向导 |
| 用户学习成本 | 低 | 低 | 保持界面一致性，提供帮助文档 |

---

## 十一、总结

本方案整合了两份设计文档的精华：

**从 `改造建议生成.md` 吸收：**
- 痛点分析框架
- 手动调试中心设计思路
- 标定向导设计思路
- 通信协议标准化
- 四阶段实施路线图

**从 `MULTI_STATION_CAMERA_DESIGN.md` 吸收：**
- 详细的 C++ 数据结构定义
- UI 组件设计 (MultiCameraView, StationSwitchBar)
- JSON 配置文件格式
- 与现有模块集成方案
- 快捷键设计

**最终方案特点：**
1. **配置驱动** - 通过 JSON 动态构建，无需重编译
2. **容器化管理** - 告别硬编码变量
3. **一站式调试** - 手动调试中心解决现场调试痛点
4. **向导式标定** - 防止遗漏，降低操作门槛
5. **增量改进** - 保留现有优势，逐步升级
