# ============================================================
# VisionForge Pro - src/algorithm3d/CMakeLists.txt
# 工业级3D算法模块
# ============================================================

message(STATUS "配置3D算法模块...")

# ============================================================
# 源文件
# ============================================================
set(ALGORITHM3D_SOURCES
    Algorithm3DTypes.cpp
    PointCloud3D.cpp
    DepthMap.cpp
    Calibration3D.cpp
    Measurement3D.cpp
)

set(ALGORITHM3D_HEADERS
    ${CMAKE_SOURCE_DIR}/include/algorithm3d/Algorithm3DTypes.h
    ${CMAKE_SOURCE_DIR}/include/algorithm3d/PointCloud3D.h
    ${CMAKE_SOURCE_DIR}/include/algorithm3d/DepthMap.h
    ${CMAKE_SOURCE_DIR}/include/algorithm3d/Calibration3D.h
    ${CMAKE_SOURCE_DIR}/include/algorithm3d/Measurement3D.h
)

# ============================================================
# 创建静态库
# ============================================================
add_library(VisionForgeAlgorithm3D STATIC
    ${ALGORITHM3D_SOURCES}
    ${ALGORITHM3D_HEADERS}
)

# 设置目标属性
set_target_properties(VisionForgeAlgorithm3D PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForgeAlgorithm3D PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# 查找Eigen（必需）
# ============================================================
set(EIGEN3_INCLUDE_DIR "F:/机器视觉/eigen-5.0.0")
if(NOT TARGET Eigen3::Eigen)
    find_package(Eigen3 CONFIG QUIET)
    if(NOT Eigen3_FOUND)
        # 如果CONFIG模式失败，手动配置
        message(STATUS "Algorithm3D: Eigen3 CONFIG未找到，使用手动配置: ${EIGEN3_INCLUDE_DIR}")
        add_library(Eigen3::Eigen INTERFACE IMPORTED)
        set_target_properties(Eigen3::Eigen PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${EIGEN3_INCLUDE_DIR}"
        )
    endif()
else()
    message(STATUS "Algorithm3D: 使用已存在的Eigen3::Eigen目标")
endif()

# ============================================================
# 可选：PCL点云库
# ============================================================
option(VISIONFORGE_USE_PCL "Use PCL for point cloud processing" OFF)

if(VISIONFORGE_USE_PCL)
    find_package(PCL 1.10 COMPONENTS common io filters segmentation registration surface)
    if(PCL_FOUND)
        message(STATUS "PCL found: ${PCL_VERSION}")
        target_compile_definitions(VisionForgeAlgorithm3D PUBLIC VISIONFORGE_USE_PCL)
        target_include_directories(VisionForgeAlgorithm3D PUBLIC ${PCL_INCLUDE_DIRS})
        target_link_libraries(VisionForgeAlgorithm3D PUBLIC ${PCL_LIBRARIES})
        target_compile_definitions(VisionForgeAlgorithm3D PUBLIC ${PCL_DEFINITIONS})
    else()
        message(WARNING "PCL not found, using built-in point cloud processing")
    endif()
endif()

# ============================================================
# 链接依赖库
# ============================================================
target_link_libraries(VisionForgeAlgorithm3D PUBLIC
    Qt6::Core
    Qt6::Gui
    Eigen3::Eigen
    VisionForgeBase
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForgeAlgorithm3D PRIVATE
        /utf-8
        /W4
        /WX-
        /bigobj
    )
    # 禁用一些Eigen相关警告
    target_compile_options(VisionForgeAlgorithm3D PRIVATE
        /wd4127  # conditional expression is constant
        /wd4244  # conversion from 'double' to 'float'
        /wd4267  # conversion from 'size_t' to 'int'
    )
else()
    target_compile_options(VisionForgeAlgorithm3D PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
endif()

# ============================================================
# 预编译头
# ============================================================
target_precompile_headers(VisionForgeAlgorithm3D PRIVATE
    <QObject>
    <QString>
    <QImage>
    <QJsonObject>
    <QJsonArray>
    <QMutex>
    <Eigen/Core>
    <Eigen/Dense>
    <memory>
    <vector>
    <cmath>
)

# ============================================================
# 安装规则
# ============================================================
install(TARGETS VisionForgeAlgorithm3D
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES ${ALGORITHM3D_HEADERS}
    DESTINATION include/algorithm3d
)

message(STATUS "3D算法模块配置完成")
message(STATUS "  - 功能: 点云处理、深度图处理、3D标定、3D测量")
message(STATUS "  - 依赖: Eigen3 (必需), PCL (可选)")
if(VISIONFORGE_USE_PCL AND PCL_FOUND)
    message(STATUS "  - PCL集成: 已启用 (${PCL_VERSION})")
else()
    message(STATUS "  - PCL集成: 未启用 (使用内置实现)")
endif()
