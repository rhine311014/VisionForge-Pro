# VisionForge Pro 多工位多相机改进方案

## 一、现状分析

### 1.1 当前架构优势

| 模块 | 状态 | 说明 |
|------|------|------|
| CameraManager | ✅ 完善 | 支持多相机分组、同步采集、硬件触发 |
| WorkStation | ✅ 完善 | 支持多位置、多种对位方式、迭代对位 |
| PlatformConfig | ✅ 完善 | 支持16种平台类型 |
| RecipeManager | ✅ 完善 | 独立方案管理、模板库 |

### 1.2 当前不足

| 问题 | 影响 |
|------|------|
| MainWindow 只管理单相机 | 无法同时显示多相机画面 |
| 缺少多工位切换UI | 调试时需要频繁手动切换 |
| 位置-相机关联不直观 | 配置复杂，调试困难 |
| 缺少统一的工位显示视图 | 无法一览全局状态 |

---

## 二、改进目标

1. **多相机同时显示** - 支持2/4/6/8路画面同时显示
2. **工位快速切换** - 一键切换不同工位配置
3. **位置-相机直观关联** - 清晰展示每个位置使用哪个相机
4. **统一调试界面** - 集成位置移动、采集、定位的一站式调试

---

## 三、核心数据结构设计

### 3.1 位置-相机关联结构 (借鉴 VisionASM CPosInfo)

```cpp
// include/core/PositionCameraBinding.h

namespace VisionForge {
namespace Core {

/**
 * @brief 位置-相机绑定信息
 * 简洁地将位置与相机关联，支持一个相机服务多个位置
 */
struct PositionCameraBinding {
    QString positionId;         // 位置ID
    QString positionName;       // 位置名称 (如 "Pos0-Target", "Pos1-Object")
    bool enabled = true;        // 是否启用
    int cameraIndex = 0;        // 相机索引 (0-7)
    QString cameraId;           // 相机ID (CameraManager中的ID)

    // 显示配置
    int displayIndex = -1;      // 在多画面显示中的位置 (-1=自动分配)

    // 采集配置
    int captureOrder = 0;       // 顺序采集时的顺序
    bool useHardwareTrigger = false; // 是否使用硬件触发

    QJsonObject toJson() const;
    static PositionCameraBinding fromJson(const QJsonObject& json);
};

/**
 * @brief 工位完整配置 (借鉴 VisionASM SysPlatformInfo)
 * 集中管理工位的所有配置
 */
struct StationConfig {
    // 基本信息
    QString id;                 // 工位ID (如 "WS0", "WS1")
    QString name;               // 工位名称 (如 "工位1-上料")
    int index = 0;              // 工位索引 (0-5)
    bool enabled = true;        // 是否启用

    // 平台配置
    Platform::PlatformType platformType = Platform::PlatformType::XYD;
    QString platformConfigId;   // 关联的平台配置ID

    // 相机配置
    int cameraNum = 2;          // 相机数量 (1-8)
    QVector<QString> cameraIds; // 相机ID列表
    bool shareCameraSystem = false; // 是否与其他工位共享相机
    QString sharedFromStationId;    // 共享源工位ID

    // 位置配置
    int positionNum = 2;        // 位置数量 (1-8)
    QVector<PositionCameraBinding> positionBindings; // 位置-相机绑定

    // 显示配置
    enum DisplayMode {
        ByPosition = 0,     // 按位置显示 (每个位置一个窗口)
        ByCamera = 1        // 按相机显示 (每个相机一个窗口)
    };
    DisplayMode displayMode = ByPosition;
    int displayLayout = 0;      // 布局模式: 0=自动, 1=1x1, 2=1x2, 3=2x2, 4=2x3, 5=2x4

    // 触发配置
    bool useHardwareTrigger = false;
    QString triggerSource;      // 触发源 (GPIO/PLC/软触发)

    // 方法
    QJsonObject toJson() const;
    static StationConfig fromJson(const QJsonObject& json);

    // 辅助方法
    QString getCameraIdForPosition(int posIndex) const;
    QVector<int> getPositionsForCamera(const QString& cameraId) const;
};

} // namespace Core
} // namespace VisionForge
```

### 3.2 多工位管理器

```cpp
// include/core/MultiStationManager.h

namespace VisionForge {
namespace Core {

/**
 * @brief 多工位管理器 (单例)
 * 统一管理所有工位配置和切换
 */
class MultiStationManager : public QObject {
    Q_OBJECT

public:
    static MultiStationManager& instance();

    // 最大工位数
    static constexpr int MAX_STATION_NUM = 6;

    // ========== 工位管理 ==========

    // 创建/删除工位
    QString createStation(const QString& name, Platform::PlatformType type);
    bool removeStation(const QString& stationId);

    // 获取工位
    StationConfig* getStation(const QString& stationId);
    StationConfig* getStationByIndex(int index);
    QVector<StationConfig*> getAllStations();
    int getStationCount() const;

    // ========== 当前工位 ==========

    // 切换工位
    bool setCurrentStation(const QString& stationId);
    bool setCurrentStationByIndex(int index);
    StationConfig* currentStation();
    int currentStationIndex() const;

    // ========== 位置-相机绑定 ==========

    // 为工位添加位置
    bool addPositionBinding(const QString& stationId, const PositionCameraBinding& binding);
    bool removePositionBinding(const QString& stationId, const QString& positionId);
    bool updatePositionBinding(const QString& stationId, const PositionCameraBinding& binding);

    // 获取绑定
    PositionCameraBinding* getPositionBinding(const QString& stationId, const QString& positionId);
    QVector<PositionCameraBinding> getBindingsForCamera(const QString& stationId, const QString& cameraId);

    // ========== 相机共享 ==========

    // 设置相机共享
    bool setSharedCamera(const QString& targetStationId, const QString& sourceStationId);
    bool clearSharedCamera(const QString& stationId);

    // ========== 配置持久化 ==========

    bool saveConfig(const QString& filePath = QString());
    bool loadConfig(const QString& filePath = QString());

signals:
    void stationCreated(const QString& stationId);
    void stationRemoved(const QString& stationId);
    void currentStationChanged(const QString& stationId, int index);
    void stationConfigChanged(const QString& stationId);
    void positionBindingChanged(const QString& stationId, const QString& positionId);

private:
    MultiStationManager(QObject* parent = nullptr);

    QMap<QString, StationConfig> stations_;
    QString currentStationId_;
    int stationCount_ = 0;
};

} // namespace Core
} // namespace VisionForge
```

---

## 四、UI 改进设计

### 4.1 多相机显示组件

```cpp
// include/ui/MultiCameraView.h

namespace VisionForge {
namespace UI {

/**
 * @brief 多相机显示视图
 * 支持1/2/4/6/8路画面同时显示
 */
class MultiCameraView : public QWidget {
    Q_OBJECT

public:
    enum LayoutMode {
        Layout_1x1 = 1,     // 单画面
        Layout_1x2 = 2,     // 1行2列
        Layout_2x2 = 4,     // 2行2列
        Layout_2x3 = 6,     // 2行3列
        Layout_2x4 = 8      // 2行4列
    };

    explicit MultiCameraView(QWidget* parent = nullptr);

    // 布局控制
    void setLayoutMode(LayoutMode mode);
    LayoutMode layoutMode() const;

    // 关联工位
    void setStation(Core::StationConfig* station);

    // 更新画面
    void updateImage(int viewIndex, const HalconCpp::HImage& image);
    void updateImage(const QString& positionId, const HalconCpp::HImage& image);

    // 选中控制
    void setSelectedView(int index);
    int selectedView() const;

    // 获取单个视图
    HalconImageViewer* getViewer(int index);

signals:
    void viewSelected(int index);
    void viewDoubleClicked(int index);  // 双击最大化

private:
    void setupLayout();
    void updateLayoutFromStation();

    LayoutMode layoutMode_ = Layout_2x2;
    QVector<HalconImageViewer*> viewers_;
    Core::StationConfig* currentStation_ = nullptr;
    int selectedIndex_ = 0;

    QGridLayout* gridLayout_;
};

} // namespace UI
} // namespace VisionForge
```

### 4.2 工位切换工具栏

```cpp
// include/ui/StationSwitchBar.h

namespace VisionForge {
namespace UI {

/**
 * @brief 工位切换工具栏
 * 显示所有工位按钮，支持快速切换
 */
class StationSwitchBar : public QWidget {
    Q_OBJECT

public:
    explicit StationSwitchBar(QWidget* parent = nullptr);

    // 更新工位按钮
    void refreshStations();

    // 设置当前工位
    void setCurrentStation(int index);
    void setCurrentStation(const QString& stationId);

    // 显示模式
    void setShowAllButton(bool show);    // 显示"全部"按钮
    void setShowStationStatus(bool show); // 显示工位状态指示

signals:
    void stationSelected(int index);
    void stationSelected(const QString& stationId);
    void showAllStations();  // 显示全部工位

private:
    void setupUI();
    void updateButtons();

    QHBoxLayout* layout_;
    QPushButton* btnShowAll_;           // 显示全部
    QVector<QPushButton*> stationBtns_; // 工位按钮 (最多6个)
    QButtonGroup* buttonGroup_;

    int currentIndex_ = 0;
};

} // namespace UI
} // namespace VisionForge
```

### 4.3 位置-相机配置对话框

```cpp
// include/ui/PositionCameraConfigDialog.h

namespace VisionForge {
namespace UI {

/**
 * @brief 位置-相机配置对话框
 * 直观配置每个位置使用哪个相机
 */
class PositionCameraConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit PositionCameraConfigDialog(Core::StationConfig* station, QWidget* parent = nullptr);

    // 获取配置结果
    QVector<Core::PositionCameraBinding> getBindings() const;

private:
    void setupUI();
    void loadFromStation();
    void updatePreview();

    Core::StationConfig* station_;

    // 位置数量选择
    QSpinBox* spinPositionNum_;

    // 位置配置表格
    // | 位置 | 启用 | 相机 | 显示位置 |
    // | Pos0 |  ✓  | Cam0 |    0    |
    // | Pos1 |  ✓  | Cam0 |    1    |
    // | Pos2 |  ✓  | Cam1 |    2    |
    QTableWidget* tableBindings_;

    // 预览区域 - 显示布局效果
    QLabel* previewLabel_;
};

} // namespace UI
} // namespace VisionForge
```

---

## 五、配置文件结构设计

### 5.1 目录结构 (借鉴 VisionASM)

```
VisionForgeConfig/
├── global.json                     # 全局配置
├── cameras.json                    # 相机配置 (CameraManager)
├── stations/                       # 工位配置目录
│   ├── stations.json               # 工位列表
│   ├── WS0/                        # 工位0
│   │   ├── station.json            # 工位配置
│   │   ├── platform.json           # 平台配置
│   │   └── calibration/            # 标定数据
│   ├── WS1/                        # 工位1
│   │   ├── station.json
│   │   └── ...
│   └── WS2/                        # 工位2
└── recipes/                        # 方案目录 (现有结构)
    └── 产品名/
        ├── 产品名.vfr
        ├── models/
        │   ├── WS0/                # 工位0的模板
        │   │   ├── Pos0_Target/
        │   │   └── Pos1_Object/
        │   └── WS1/                # 工位1的模板
        └── calibration/
```

### 5.2 station.json 示例

```json
{
    "id": "WS0",
    "name": "工位1-上料对位",
    "index": 0,
    "enabled": true,

    "platform": {
        "type": "XYD",
        "configId": "platform_xyd_001"
    },

    "cameras": {
        "count": 2,
        "ids": ["cam_hikv_001", "cam_hikv_002"],
        "shared": false
    },

    "positions": {
        "count": 4,
        "bindings": [
            {
                "positionId": "pos0",
                "positionName": "Pos0-Target",
                "enabled": true,
                "cameraIndex": 0,
                "cameraId": "cam_hikv_001",
                "displayIndex": 0,
                "captureOrder": 0
            },
            {
                "positionId": "pos1",
                "positionName": "Pos1-Target",
                "enabled": true,
                "cameraIndex": 0,
                "cameraId": "cam_hikv_001",
                "displayIndex": 1,
                "captureOrder": 1
            },
            {
                "positionId": "pos2",
                "positionName": "Pos2-Object",
                "enabled": true,
                "cameraIndex": 1,
                "cameraId": "cam_hikv_002",
                "displayIndex": 2,
                "captureOrder": 2
            },
            {
                "positionId": "pos3",
                "positionName": "Pos3-Object",
                "enabled": true,
                "cameraIndex": 1,
                "cameraId": "cam_hikv_002",
                "displayIndex": 3,
                "captureOrder": 3
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
    }
}
```

---

## 六、MainWindow 改进

### 6.1 新增成员

```cpp
// MainWindow.h 新增

class MainWindow : public QMainWindow {
    // ...现有代码...

private:
    // ========== 多工位多相机支持 ==========

    // 多相机显示
    UI::MultiCameraView* multiCameraView_;      // 替代单一 imageViewer_

    // 工位切换
    UI::StationSwitchBar* stationSwitchBar_;    // 工位切换工具栏

    // 当前工位
    Core::StationConfig* currentStation_ = nullptr;

    // ========== 新增槽函数 ==========

private slots:
    // 工位切换
    void onStationChanged(int index);
    void onShowAllStations();

    // 多相机采集
    void onCaptureAllPositions();       // 采集所有位置
    void onCaptureCurrentPosition();    // 采集当前选中位置

    // 配置
    void onConfigurePositionCamera();   // 打开位置-相机配置对话框
};
```

### 6.2 初始化流程

```cpp
void MainWindow::initMultiStationSupport()
{
    // 1. 创建多相机显示视图
    multiCameraView_ = new UI::MultiCameraView(this);

    // 2. 创建工位切换工具栏
    stationSwitchBar_ = new UI::StationSwitchBar(this);
    toolBar_->addWidget(stationSwitchBar_);

    // 3. 连接信号
    connect(stationSwitchBar_, &UI::StationSwitchBar::stationSelected,
            this, &MainWindow::onStationChanged);
    connect(stationSwitchBar_, &UI::StationSwitchBar::showAllStations,
            this, &MainWindow::onShowAllStations);

    // 4. 加载默认工位
    auto& manager = Core::MultiStationManager::instance();
    if (manager.getStationCount() > 0) {
        onStationChanged(0);
    }
}

void MainWindow::onStationChanged(int index)
{
    auto& manager = Core::MultiStationManager::instance();

    // 切换工位
    manager.setCurrentStationByIndex(index);
    currentStation_ = manager.currentStation();

    if (!currentStation_) return;

    // 更新多相机视图
    multiCameraView_->setStation(currentStation_);

    // 更新窗口标题
    setWindowTitle(QString("VisionForge Pro - %1").arg(currentStation_->name));

    // 记录日志
    LOG_INFO(QString("切换到工位: %1 (%2)")
             .arg(currentStation_->name)
             .arg(currentStation_->id));
}
```

---

## 七、实现优先级

### Phase 1: 核心数据结构 (1-2天)
- [ ] PositionCameraBinding 结构
- [ ] StationConfig 结构
- [ ] MultiStationManager 基础实现
- [ ] 配置文件读写

### Phase 2: 多相机显示 (2-3天)
- [ ] MultiCameraView 组件
- [ ] 布局切换 (1x1, 2x2, 2x3, 2x4)
- [ ] 单画面选中/最大化

### Phase 3: 工位切换UI (1-2天)
- [ ] StationSwitchBar 组件
- [ ] MainWindow 集成
- [ ] 工位状态显示

### Phase 4: 配置界面 (2-3天)
- [ ] PositionCameraConfigDialog
- [ ] 系统设置中的工位管理
- [ ] 向导式配置流程

### Phase 5: 调试功能完善 (2-3天)
- [ ] 单工位调试模式
- [ ] 全工位预览模式
- [ ] 快捷键支持 (F1-F6切换工位)

---

## 八、与现有模块的集成

### 8.1 与 CameraManager 集成

```cpp
// StationConfig 获取相机时调用 CameraManager
ICamera* StationConfig::getCamera(int positionIndex) {
    QString cameraId = getCameraIdForPosition(positionIndex);
    return HAL::CameraManager::instance().getCamera(cameraId);
}

// 批量采集
HAL::BatchCaptureResult StationConfig::captureAllPositions() {
    QStringList cameraIds;
    for (const auto& binding : positionBindings) {
        if (binding.enabled && !cameraIds.contains(binding.cameraId)) {
            cameraIds.append(binding.cameraId);
        }
    }
    return HAL::CameraManager::instance().captureSimultaneous(cameraIds);
}
```

### 8.2 与 WorkStation 集成

```cpp
// MultiStationManager 创建 WorkStation 时同步配置
void MultiStationManager::syncToWorkStation(const QString& stationId) {
    auto* config = getStation(stationId);
    auto* ws = Core::WorkStationManager::instance().getWorkStation(stationId);

    if (!config || !ws) return;

    // 同步位置配置
    for (const auto& binding : config->positionBindings) {
        Core::PositionConfig posConfig;
        posConfig.id = binding.positionId;
        posConfig.name = binding.positionName;
        posConfig.cameraId = binding.cameraId;
        // ...
        ws->updatePosition(posConfig);
    }
}
```

### 8.3 与 RecipeManager 集成

```cpp
// 方案加载时同步工位模板
void RecipeManager::loadRecipeForStation(const QString& stationId) {
    auto* recipe = currentRecipe();
    if (!recipe) return;

    // 加载该工位的模板
    QString modelPath = recipe->modelLibraryPath() + "/" + stationId;
    // ...
}
```

---

## 九、快捷键设计

| 快捷键 | 功能 |
|--------|------|
| F1-F6 | 切换工位1-6 |
| F7 | 显示全部工位 |
| Ctrl+1-8 | 选中画面1-8 |
| Space | 采集当前位置 |
| Ctrl+Space | 采集所有位置 |
| Tab | 切换到下一个画面 |
| Enter | 最大化/还原当前画面 |

---

## 十、总结

本方案借鉴 VisionASM 的以下优点：
1. **SysPlatformInfo 集中配置** → StationConfig
2. **CPosInfo 位置-相机绑定** → PositionCameraBinding
3. **分层配置目录** → stations/WS0/station.json
4. **多工位切换按钮** → StationSwitchBar

同时保留 VisionForge Pro 的现有优势：
1. CameraManager 的完善多相机管理
2. WorkStation 的对位流程
3. PlatformConfig 的多平台支持
4. RecipeManager 的方案管理

通过增量式改进，在不破坏现有功能的前提下，实现多工位多相机的便捷调试体验。
