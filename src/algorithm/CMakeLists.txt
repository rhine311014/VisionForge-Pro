# ============================================================
# 算法层 (Algorithm Layer)
# ============================================================

# 收集源文件
set(ALGORITHM_SOURCES
    VisionTool.cpp
    GrayTool.cpp
    BlurTool.cpp
    ThresholdTool.cpp
    EdgeTool.cpp
    MorphologyTool.cpp
    ROITool.cpp
    BlobTool.cpp
    CircleTool.cpp
    LineTool.cpp
    ToolFactory.cpp
    # 标定相关
    CalibrationResult.cpp
    CameraCalibTool.cpp
    NinePointCalibTool.cpp
    CalibrationManager.cpp
    # 测量工具
    MeasureDistanceTool.cpp
    MeasureAngleTool.cpp
    MeasureAreaTool.cpp
    # 判定输出工具
    RangeJudgeTool.cpp
    SaveImageTool.cpp
    PLCOutputTool.cpp
    LogicOperationTool.cpp
    # 模板匹配
    TemplateMatchTool.cpp
)

# 如果启用Halcon，添加Halcon相关工具
if(USE_HALCON)
    list(APPEND ALGORITHM_SOURCES
        ShapeMatchTool.cpp
        ShapeModelManager.cpp
    )
endif()

# 收集头文件
set(ALGORITHM_HEADERS
    ${CMAKE_SOURCE_DIR}/include/algorithm/VisionTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/GrayTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/BlurTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ThresholdTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/EdgeTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/MorphologyTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ROITool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/BlobTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/CircleTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/LineTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ToolFactory.h
    # 标定相关
    ${CMAKE_SOURCE_DIR}/include/algorithm/CalibrationResult.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/CameraCalibTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/NinePointCalibTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/CalibrationManager.h
    # 测量工具
    ${CMAKE_SOURCE_DIR}/include/algorithm/MeasureDistanceTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/MeasureAngleTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/MeasureAreaTool.h
    # 判定输出工具
    ${CMAKE_SOURCE_DIR}/include/algorithm/RangeJudgeTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/SaveImageTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/PLCOutputTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/LogicOperationTool.h
    # 模板匹配
    ${CMAKE_SOURCE_DIR}/include/algorithm/TemplateMatchTool.h
)

# 如果启用Halcon，添加Halcon相关头文件
if(USE_HALCON)
    list(APPEND ALGORITHM_HEADERS
        ${CMAKE_SOURCE_DIR}/include/algorithm/ShapeMatchTool.h
        ${CMAKE_SOURCE_DIR}/include/algorithm/HalconObjectWrapper.h
        ${CMAKE_SOURCE_DIR}/include/algorithm/ShapeModelManager.h
    )
endif()

# 创建算法层静态库
add_library(VisionForgeAlgorithm STATIC
    ${ALGORITHM_SOURCES}
    ${ALGORITHM_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeAlgorithm PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(VisionForgeAlgorithm PUBLIC
    VisionForgeBase
    Qt6::Core
    Qt6::Network
    ${OpenCV_LIBS}
)

# Halcon支持（可选）
if(USE_HALCON)
    target_link_libraries(VisionForgeAlgorithm PUBLIC ${HALCON_LIBS})
endif()

# OpenMP支持（可选）
if(USE_OPENMP AND OpenMP_CXX_FOUND)
    target_link_libraries(VisionForgeAlgorithm PUBLIC OpenMP::OpenMP_CXX)
endif()

# 设置属性
set_target_properties(VisionForgeAlgorithm PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# Windows平台：防止min/max宏冲突
if(WIN32)
    target_compile_definitions(VisionForgeAlgorithm PRIVATE NOMINMAX)
endif()

# 输出信息
message(STATUS "算法层 (VisionForgeAlgorithm) 配置完成")
