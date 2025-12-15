# ============================================================
# 算法层 (Algorithm Layer)
# ============================================================

# 收集源文件
set(ALGORITHM_SOURCES
    VisionTool.cpp
    GrayTool.cpp
    BlurTool.cpp
    ThresholdTool.cpp
    EdgeDetectionTool.cpp
    ToolFactory.cpp
)

# 收集头文件
set(ALGORITHM_HEADERS
    ${CMAKE_SOURCE_DIR}/include/algorithm/VisionTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/GrayTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/BlurTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ThresholdTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/EdgeDetectionTool.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ToolFactory.h
)

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
    ${OpenCV_LIBS}
)

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

# 输出信息
message(STATUS "算法层 (VisionForgeAlgorithm) 配置完成")
