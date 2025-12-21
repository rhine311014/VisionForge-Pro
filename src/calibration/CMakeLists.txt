# ============================================================
# VisionForge Pro - Calibration 模块 CMakeLists.txt
# ============================================================
# 标定和对齐模块，提供：
# - 多点标定（5点/9点/16点等）
# - H矩阵计算（DLT + SVD）
# - 对齐引擎（单点/两点/三点对齐）
# - 与Platform模块集成
# ============================================================

# ============================================================
# 头文件
# ============================================================
set(CALIBRATION_HEADERS
    ${CMAKE_SOURCE_DIR}/include/calibration/CalibrationTypes.h
    ${CMAKE_SOURCE_DIR}/include/calibration/CalibrationManager.h
    ${CMAKE_SOURCE_DIR}/include/calibration/AlignmentEngine.h
)

# ============================================================
# 源文件
# ============================================================
set(CALIBRATION_SOURCES
    CalibrationTypes.cpp
    CalibrationManager.cpp
    AlignmentEngine.cpp
)

# ============================================================
# 创建静态库
# ============================================================
add_library(VisionForgeCalibration STATIC
    ${CALIBRATION_HEADERS}
    ${CALIBRATION_SOURCES}
)

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForgeCalibration PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# 查找Eigen库
# ============================================================
set(EIGEN3_INCLUDE_DIR "F:/机器视觉/eigen-5.0.0")
find_package(Eigen3 CONFIG QUIET)
if(NOT Eigen3_FOUND)
    # 如果CONFIG模式失败，手动配置
    message(STATUS "Eigen3 CONFIG未找到，使用手动配置: ${EIGEN3_INCLUDE_DIR}")
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${EIGEN3_INCLUDE_DIR}"
    )
endif()

# ============================================================
# 链接依赖
# ============================================================
target_link_libraries(VisionForgeCalibration PUBLIC
    # Qt库
    Qt6::Core
    Qt6::Gui

    # Eigen矩阵库（用于精密几何计算）
    Eigen3::Eigen

    # 内部依赖
    VisionForgePlatform
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForgeCalibration PRIVATE
        /utf-8  # 使用UTF-8编码（支持中文注释）
    )
endif()

# ============================================================
# 预处理器定义
# ============================================================
target_compile_definitions(VisionForgeCalibration PRIVATE
    # Eigen性能优化
    EIGEN_NO_DEBUG
)

# ============================================================
# 导出头文件（供其他模块使用）
# ============================================================
target_sources(VisionForgeCalibration PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES ${CALIBRATION_HEADERS}
)

# ============================================================
# 输出信息
# ============================================================
message(STATUS "VisionForgeCalibration: Calibration模块配置完成")
message(STATUS "  - 头文件: ${CALIBRATION_HEADERS}")
message(STATUS "  - 源文件: ${CALIBRATION_SOURCES}")
message(STATUS "  - 依赖: Qt6::Core, Qt6::Gui, OpenCV, VisionForgePlatform")
