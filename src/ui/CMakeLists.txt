# ============================================================
# UI层 (UI Layer)
# ============================================================

# 收集源文件
set(UI_SOURCES
    ImageViewer.cpp
    ToolParameterPanel.cpp
    ToolChainPanel.cpp
    ResultTablePanel.cpp
    HistoryPanel.cpp
    MainWindow.cpp
    BatchProcessWorker.cpp
    Theme.cpp
    # 插件化UI系统
    ToolDialogFactory.cpp
    PLCConfigDialog.cpp
    CameraConfigDialog.cpp
    RecipeManagerWidget.cpp
    RecipeEditorDialog.cpp
    CameraCalibDialog.cpp
    NinePointCalibDialog.cpp
    QRCalibDialog.cpp
    SystemSettingsDialog.cpp
    PreviewHelper.cpp
    # 工具对话框 - 预处理
    GrayToolDialog.cpp
    BlurToolDialog.cpp
    ThresholdToolDialog.cpp
    ColorConvertToolDialog.cpp
    EdgeToolDialog.cpp
    MorphologyToolDialog.cpp
    # 工具对话框 - 检测
    TemplateMatchToolDialog.cpp
    CircleToolDialog.cpp
    LineToolDialog.cpp
    FindEdgeToolDialog.cpp
    SubPixelEdgeToolDialog.cpp
    BlobToolDialog.cpp
    AIDetectionToolDialog.cpp
    # 工具对话框 - 测量和判定
    MeasureDistanceToolDialog.cpp
    MeasureAngleToolDialog.cpp
    MeasureAreaToolDialog.cpp
    CalcCenterToolDialog.cpp
    CalcOrientationToolDialog.cpp
    RangeJudgeToolDialog.cpp
    LogicOperationToolDialog.cpp
    SaveImageToolDialog.cpp
    # 工具对话框 - ROI和PLC
    ROIToolDialog.cpp
    PLCOutputToolDialog.cpp
    # 系统对话框
    LoginDialog.cpp
    StatisticsPanel.cpp
    UserManagementDialog.cpp
    UIModeManager.cpp
    ShortcutManager.cpp
    # 多相机多位置对位对话框
    MultiCameraManagerDialog.cpp
    MultiPointAlignmentDialog.cpp
    AlignmentOutputDialog.cpp
    WorkStationDialog.cpp
    # 工位配置工具
    StationConfigTool.cpp
    # 相机设置对话框
    CameraSetupDialog.cpp
    # 场景切换栏
    SceneSwitchBar.cpp
    # 操作员界面组件
    OperatorToolBar.cpp
    InfoBar.cpp
)

# 如果启用Halcon，添加HalconImageViewer
if(USE_HALCON)
    list(APPEND UI_SOURCES
        HalconImageViewer.cpp
        HalconDisplayWorker.cpp
        HalconUtils.cpp
        ShapeMatchParamPanel.cpp
        ShapeModelLibraryDialog.cpp
        ShapeMatchToolDialog.cpp
        ModelCheckPointDialog.cpp
        # 多工位多相机视图组件
        MultiCameraView.cpp
        StationSwitchBar.cpp
        # 手动调试中心
        ManualDebugDialog.cpp
        # 标定向导
        CalibrationWizard.cpp
        # 读码工具对话框
        CodeReadToolDialog.cpp
    )
endif()

# 收集头文件
set(UI_HEADERS
    ${CMAKE_SOURCE_DIR}/include/ui/ImageViewer.h
    ${CMAKE_SOURCE_DIR}/include/ui/ToolParameterPanel.h
    ${CMAKE_SOURCE_DIR}/include/ui/ToolChainPanel.h
    ${CMAKE_SOURCE_DIR}/include/ui/ResultTablePanel.h
    ${CMAKE_SOURCE_DIR}/include/ui/HistoryPanel.h
    ${CMAKE_SOURCE_DIR}/include/ui/MainWindow.h
    ${CMAKE_SOURCE_DIR}/include/ui/BatchProcessWorker.h
    ${CMAKE_SOURCE_DIR}/include/ui/ROI.h
    ${CMAKE_SOURCE_DIR}/include/ui/Theme.h
    # 插件化UI系统
    ${CMAKE_SOURCE_DIR}/include/ui/IToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/ToolDialogFactory.h
    ${CMAKE_SOURCE_DIR}/include/ui/PLCConfigDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/CameraConfigDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/RecipeManagerWidget.h
    ${CMAKE_SOURCE_DIR}/include/ui/RecipeEditorDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/CameraCalibDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/NinePointCalibDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/QRCalibDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/SystemSettingsDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/PreviewHelper.h
    # 工具对话框头文件 - 预处理
    ${CMAKE_SOURCE_DIR}/include/ui/GrayToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/BlurToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/ThresholdToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/ColorConvertToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/EdgeToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/MorphologyToolDialog.h
    # 工具对话框头文件 - 检测
    ${CMAKE_SOURCE_DIR}/include/ui/TemplateMatchToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/CircleToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/LineToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/FindEdgeToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/SubPixelEdgeToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/BlobToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/AIDetectionToolDialog.h
    # 工具对话框头文件 - 测量和判定
    ${CMAKE_SOURCE_DIR}/include/ui/MeasureDistanceToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/MeasureAngleToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/MeasureAreaToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/CalcCenterToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/CalcOrientationToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/RangeJudgeToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/LogicOperationToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/SaveImageToolDialog.h
    # 工具对话框头文件 - ROI和PLC
    ${CMAKE_SOURCE_DIR}/include/ui/ROIToolDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/PLCOutputToolDialog.h
    # 系统对话框头文件
    ${CMAKE_SOURCE_DIR}/include/ui/LoginDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/StatisticsPanel.h
    ${CMAKE_SOURCE_DIR}/include/ui/UserManagementDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/UIModeManager.h
    ${CMAKE_SOURCE_DIR}/include/ui/ShortcutManager.h
    # 多相机多位置对位对话框头文件
    ${CMAKE_SOURCE_DIR}/include/ui/MultiCameraManagerDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/MultiPointAlignmentDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/AlignmentOutputDialog.h
    ${CMAKE_SOURCE_DIR}/include/ui/WorkStationDialog.h
    # 工位配置工具头文件
    ${CMAKE_SOURCE_DIR}/include/ui/StationConfigTool.h
    # 相机设置对话框头文件
    ${CMAKE_SOURCE_DIR}/include/ui/CameraSetupDialog.h
    # 场景切换栏头文件
    ${CMAKE_SOURCE_DIR}/include/ui/SceneSwitchBar.h
    # 操作员界面组件头文件
    ${CMAKE_SOURCE_DIR}/include/ui/OperatorToolBar.h
    ${CMAKE_SOURCE_DIR}/include/ui/InfoBar.h
)

# 如果启用Halcon，添加HalconImageViewer头文件
if(USE_HALCON)
    list(APPEND UI_HEADERS
        ${CMAKE_SOURCE_DIR}/include/ui/HalconImageViewer.h
        ${CMAKE_SOURCE_DIR}/include/ui/HalconDisplayWorker.h
        ${CMAKE_SOURCE_DIR}/include/ui/HalconUtils.h
        ${CMAKE_SOURCE_DIR}/include/ui/ShapeMatchParamPanel.h
        ${CMAKE_SOURCE_DIR}/include/ui/ShapeModelLibraryDialog.h
        ${CMAKE_SOURCE_DIR}/include/ui/ShapeMatchToolDialog.h
        ${CMAKE_SOURCE_DIR}/include/ui/ModelCheckPointDialog.h
        # 多工位多相机视图组件头文件
        ${CMAKE_SOURCE_DIR}/include/ui/MultiCameraView.h
        ${CMAKE_SOURCE_DIR}/include/ui/StationSwitchBar.h
        # 手动调试中心头文件
        ${CMAKE_SOURCE_DIR}/include/ui/ManualDebugDialog.h
        # 标定向导头文件
        ${CMAKE_SOURCE_DIR}/include/ui/CalibrationWizard.h
        # 读码工具对话框头文件
        ${CMAKE_SOURCE_DIR}/include/ui/CodeReadToolDialog.h
    )
endif()

# 创建UI层静态库
add_library(VisionForgeUI STATIC
    ${UI_SOURCES}
    ${UI_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeUI PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(VisionForgeUI PUBLIC
    VisionForgeBase
    VisionForgeHAL
    VisionForgeAlgorithm
    VisionForgeCalibration
    VisionForgeComm
    VisionForgeCore
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    ${OpenCV_LIBS}
)

# 如果启用Halcon，链接Halcon库
if(USE_HALCON)
    target_link_libraries(VisionForgeUI PUBLIC
        halconcpp
        halcon
    )
endif()

# 如果启用海康相机，添加编译定义
if(USE_HIKVISION_MVS)
    target_compile_definitions(VisionForgeUI PRIVATE USE_HIKVISION_MVS)
    target_include_directories(VisionForgeUI PRIVATE
        ${HIKVISION_MVS_ROOT}/Development/Includes
    )
endif()

# 如果启用Basler相机，添加编译定义
if(USE_BASLER_PYLON)
    target_compile_definitions(VisionForgeUI PRIVATE USE_BASLER_PYLON)
endif()

# CUDA支持（需要包含CUDA头文件路径）
if(USE_CUDA AND CUDA_FOUND)
    target_include_directories(VisionForgeUI PRIVATE ${CUDA_INCLUDE_DIRS})
    target_compile_definitions(VisionForgeUI PRIVATE USE_CUDA)
endif()

# 设置属性
set_target_properties(VisionForgeUI PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# 输出信息
message(STATUS "UI层 (VisionForgeUI) 配置完成")
