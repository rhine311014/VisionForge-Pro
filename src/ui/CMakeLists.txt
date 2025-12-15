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
    Theme.cpp
)

# 如果启用Halcon，添加HalconImageViewer
if(USE_HALCON)
    list(APPEND UI_SOURCES
        HalconImageViewer.cpp
        HalconDisplayWorker.cpp
        HalconUtils.cpp
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
    ${CMAKE_SOURCE_DIR}/include/ui/ROI.h
    ${CMAKE_SOURCE_DIR}/include/ui/Theme.h
)

# 如果启用Halcon，添加HalconImageViewer头文件
if(USE_HALCON)
    list(APPEND UI_HEADERS
        ${CMAKE_SOURCE_DIR}/include/ui/HalconImageViewer.h
        ${CMAKE_SOURCE_DIR}/include/ui/HalconDisplayWorker.h
        ${CMAKE_SOURCE_DIR}/include/ui/HalconUtils.h
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

# 设置属性
set_target_properties(VisionForgeUI PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# 输出信息
message(STATUS "UI层 (VisionForgeUI) 配置完成")
