# ============================================================
# VisionForge Pro - Platform 模块 CMakeLists.txt
# ============================================================
# 核心平台领域模型，定义工业对位平台的基础类型和坐标系统
# 参考VisionASM架构设计
# ============================================================

# ============================================================
# 头文件
# ============================================================
set(PLATFORM_HEADERS
    ${CMAKE_SOURCE_DIR}/include/platform/PlatformTypes.h
    ${CMAKE_SOURCE_DIR}/include/platform/CoordPosition.h
    ${CMAKE_SOURCE_DIR}/include/platform/PlatformConfig.h
)

# ============================================================
# 源文件
# ============================================================
set(PLATFORM_SOURCES
    PlatformTypes.cpp
    CoordPosition.cpp
    PlatformConfig.cpp
)

# ============================================================
# 创建静态库
# ============================================================
add_library(VisionForgePlatform STATIC
    ${PLATFORM_HEADERS}
    ${PLATFORM_SOURCES}
)

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForgePlatform PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# 链接依赖
# ============================================================
target_link_libraries(VisionForgePlatform PUBLIC
    Qt6::Core
    Qt6::Gui
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForgePlatform PRIVATE
        /utf-8  # 使用UTF-8编码（支持中文注释）
    )
endif()

# ============================================================
# 导出头文件（供其他模块使用）
# ============================================================
target_sources(VisionForgePlatform PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES ${PLATFORM_HEADERS}
)

# ============================================================
# 输出信息
# ============================================================
message(STATUS "VisionForgePlatform: Platform模块配置完成")
message(STATUS "  - 头文件: ${PLATFORM_HEADERS}")
message(STATUS "  - 源文件: ${PLATFORM_SOURCES}")
