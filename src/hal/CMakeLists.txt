# ============================================================
# 硬件抽象层 (HAL Layer)
# ============================================================

# 收集源文件
set(HAL_SOURCES
    SimulatedCamera.cpp
)

# 收集头文件
set(HAL_HEADERS
    ${CMAKE_SOURCE_DIR}/include/hal/ICamera.h
    ${CMAKE_SOURCE_DIR}/include/hal/SimulatedCamera.h
)

# 创建HAL层静态库
add_library(VisionForgeHAL STATIC
    ${HAL_SOURCES}
    ${HAL_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeHAL PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(VisionForgeHAL PUBLIC
    VisionForgeBase
    Qt6::Core
    ${OpenCV_LIBS}
)

# 设置属性
set_target_properties(VisionForgeHAL PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# 输出信息
message(STATUS "硬件抽象层 (VisionForgeHAL) 配置完成")
