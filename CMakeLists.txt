# ============================================================
# VisionForge Pro - CMake 配置文件
# ============================================================
cmake_minimum_required(VERSION 3.20)

# 项目信息
project(VisionForge
    VERSION 1.4.4
    DESCRIPTION "VisionForge Pro - 通用工业视觉检测平台"
    LANGUAGES CXX
)

# ============================================================
# CMake策略设置
# ============================================================
# CMP0146: FindCUDA模块已移除，设为OLD允许OpenCV等第三方库使用
if(POLICY CMP0146)
    cmake_policy(SET CMP0146 OLD)
endif()

# ============================================================
# 编译选项
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 自动处理Qt的MOC、UIC、RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 输出目录配置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================
# 构建类型
# ============================================================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "===========================================")
message(STATUS "VisionForge Pro 配置信息")
message(STATUS "===========================================")
message(STATUS "CMake版本: ${CMAKE_VERSION}")
message(STATUS "项目版本: ${PROJECT_VERSION}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++标准: C++${CMAKE_CXX_STANDARD}")
message(STATUS "===========================================")

# ============================================================
# 代码覆盖率支持
# ============================================================
include(cmake/CodeCoverage.cmake)

# ============================================================
# 平台特定配置
# ============================================================
if(WIN32)
    # Windows平台配置
    add_definitions(-DUNICODE -D_UNICODE)

    # MSVC编译器选项
    if(MSVC)
        # 启用多核编译
        add_compile_options(/MP)

        # 警告级别
        add_compile_options(/W4)

        # 禁用特定警告
        add_compile_options(/wd4251)  # DLL接口警告
        add_compile_options(/wd4275)  # 非DLL接口基类警告

        # Release优化 - 使用生成器表达式确保只在Release配置生效
        # 注意：不能同时使用 /O2 和 /RTC1，所以需要正确区分配置
        add_compile_options(
            "$<$<CONFIG:Release>:/O2>"
            "$<$<CONFIG:Release>:/Ob2>"
            "$<$<CONFIG:Release>:/Oi>"
            "$<$<CONFIG:Release>:/Ot>"
            "$<$<CONFIG:Release>:/GL>"
        )
        add_link_options(
            "$<$<CONFIG:Release>:/LTCG>"
            "$<$<CONFIG:Release>:/OPT:REF>"
            "$<$<CONFIG:Release>:/OPT:ICF>"
        )

        # 设置运行时库
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

# ============================================================
# 依赖库路径配置
# ============================================================

# Qt路径
if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "F:/Qt/6.9.3/msvc2022_64" CACHE PATH "Qt安装路径")
endif()
message(STATUS "Qt路径: ${CMAKE_PREFIX_PATH}")

# OpenCV路径 (CUDA版本)
if(NOT DEFINED OpenCV_DIR)
    set(OpenCV_DIR "D:/Program Files/OPENCV CUDA" CACHE PATH "OpenCV路径")
endif()
message(STATUS "OpenCV路径: ${OpenCV_DIR}")

# Halcon路径（可选）
option(USE_HALCON "使用Halcon库" ON)
if(USE_HALCON)
    if(NOT DEFINED HALCONROOT)
        set(HALCONROOT "F:/Halcon/HALCON-24.11-Progress-Steady" CACHE PATH "Halcon路径")
    endif()
    message(STATUS "Halcon路径: ${HALCONROOT}")
endif()

# ============================================================
# 工业相机SDK配置
# ============================================================

# 海康威视MVS SDK（可选）
option(USE_HIKVISION_MVS "使用海康威视MVS SDK" OFF)
if(USE_HIKVISION_MVS)
    if(NOT DEFINED HIKVISION_MVS_ROOT)
        set(HIKVISION_MVS_ROOT "D:/MVS" CACHE PATH "海康威视MVS SDK路径")
    endif()
    message(STATUS "海康MVS路径: ${HIKVISION_MVS_ROOT}")
endif()

# Basler Pylon SDK（可选）
option(USE_BASLER_PYLON "使用Basler Pylon SDK" OFF)
if(USE_BASLER_PYLON)
    if(NOT DEFINED PYLON_ROOT)
        set(PYLON_ROOT "C:/Program Files/Basler/pylon 6" CACHE PATH "Basler Pylon SDK路径")
    endif()
    message(STATUS "Basler Pylon路径: ${PYLON_ROOT}")
endif()

# ONNX Runtime路径（可选）
option(USE_ONNX_RUNTIME "使用ONNX Runtime" OFF)
if(USE_ONNX_RUNTIME)
    if(NOT DEFINED ONNXRUNTIME_DIR)
        set(ONNXRUNTIME_DIR "F:/Program Files/onnxruntime" CACHE PATH "ONNX Runtime路径")
    endif()
    message(STATUS "ONNX Runtime路径: ${ONNXRUNTIME_DIR}")
endif()

# TensorRT路径（可选）
option(USE_TENSORRT "使用TensorRT" OFF)
if(USE_TENSORRT)
    if(NOT DEFINED TensorRT_DIR)
        set(TensorRT_DIR "C:/Program Files/NVIDIA/TensorRT-8.6.1" CACHE PATH "TensorRT路径")
    endif()
    message(STATUS "TensorRT路径: ${TensorRT_DIR}")
endif()

# ============================================================
# 查找依赖库
# ============================================================

# 查找Qt
find_package(Qt6 6.2 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Network
    Sql
    Xml
    SerialPort
    Concurrent
    WebSockets
    LinguistTools
)
message(STATUS "Qt版本: ${Qt6_VERSION}")

# 查找OpenCV
find_package(OpenCV 4 REQUIRED)
message(STATUS "OpenCV版本: ${OpenCV_VERSION}")
message(STATUS "OpenCV库: ${OpenCV_LIBS}")

# 查找Halcon（可选）
if(USE_HALCON)
    # Halcon没有标准的find_package，需要手动配置
    if(EXISTS "${HALCONROOT}/include/halconcpp/HalconCpp.h")
        message(STATUS "Halcon: 已找到")
        include_directories(${HALCONROOT}/include)
        include_directories(${HALCONROOT}/include/halconcpp)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64位
            link_directories(${HALCONROOT}/lib/x64-win64)
        else()
            # 32位
            link_directories(${HALCONROOT}/lib/x86sse2-win32)
        endif()
        # 定义宏，表示启用了Halcon
        add_definitions(-DUSE_HALCON)
    else()
        message(WARNING "Halcon未找到: ${HALCONROOT}/include/halconcpp/HalconCpp.h")
        set(USE_HALCON OFF)
    endif()
endif()

# 查找海康威视MVS SDK（可选）
if(USE_HIKVISION_MVS)
    # 海康MVS SDK实际路径结构：Development/Includes, Development/Libraries
    set(HIKVISION_INCLUDE_DIR "${HIKVISION_MVS_ROOT}/Development/Includes")
    set(HIKVISION_LIB_DIR_64 "${HIKVISION_MVS_ROOT}/Development/Libraries/win64")
    set(HIKVISION_LIB_DIR_32 "${HIKVISION_MVS_ROOT}/Development/Libraries/win32")

    if(EXISTS "${HIKVISION_INCLUDE_DIR}/MvCameraControl.h")
        message(STATUS "海康威视MVS SDK: 已找到")
        include_directories(${HIKVISION_INCLUDE_DIR})
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories(${HIKVISION_LIB_DIR_64})
        else()
            link_directories(${HIKVISION_LIB_DIR_32})
        endif()
        add_definitions(-DUSE_HIKVISION_MVS)
    else()
        message(WARNING "海康威视MVS SDK未找到: ${HIKVISION_INCLUDE_DIR}/MvCameraControl.h")
        set(USE_HIKVISION_MVS OFF)
    endif()
endif()

# 查找Basler Pylon SDK（可选）
if(USE_BASLER_PYLON)
    if(EXISTS "${PYLON_ROOT}/include/pylon/PylonIncludes.h")
        message(STATUS "Basler Pylon SDK: 已找到")
        include_directories(${PYLON_ROOT}/include)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            link_directories(${PYLON_ROOT}/lib/x64)
        else()
            link_directories(${PYLON_ROOT}/lib/Win32)
        endif()
        add_definitions(-DUSE_BASLER_PYLON)
    else()
        message(WARNING "Basler Pylon SDK未找到: ${PYLON_ROOT}/include/pylon/PylonIncludes.h")
        set(USE_BASLER_PYLON OFF)
    endif()
endif()

# 查找ONNX Runtime（可选）
if(USE_ONNX_RUNTIME)
    # ONNX Runtime没有标准的find_package，需要手动配置
    if(EXISTS "${ONNXRUNTIME_DIR}/include/onnxruntime_cxx_api.h")
        message(STATUS "ONNX Runtime: 已找到")
        include_directories(${ONNXRUNTIME_DIR}/include)
        link_directories(${ONNXRUNTIME_DIR}/lib)
    else()
        message(WARNING "ONNX Runtime未找到")
        set(USE_ONNX_RUNTIME OFF)
    endif()
endif()

# 查找TensorRT（可选）
if(USE_TENSORRT)
    find_package(CUDA QUIET)
    if(CUDA_FOUND AND EXISTS "${TensorRT_DIR}/include/NvInfer.h")
        message(STATUS "TensorRT: 已找到")
        message(STATUS "CUDA版本: ${CUDA_VERSION}")
        include_directories(${TensorRT_DIR}/include)
        link_directories(${TensorRT_DIR}/lib)
    else()
        message(WARNING "TensorRT或CUDA未找到")
        set(USE_TENSORRT OFF)
    endif()
endif()

# 查找OpenMP（性能优化）
option(USE_OPENMP "使用OpenMP并行化" ON)
if(USE_OPENMP)
    find_package(OpenMP QUIET)
    if(OpenMP_CXX_FOUND)
        message(STATUS "OpenMP: 已找到")
        message(STATUS "OpenMP版本: ${OpenMP_CXX_VERSION}")
        # 定义宏，表示启用了OpenMP
        add_definitions(-DUSE_OPENMP)
    else()
        message(WARNING "OpenMP未找到，部分性能优化功能将不可用")
        set(USE_OPENMP OFF)
    endif()
endif()

# 查找CUDA（GPU加速）
option(USE_CUDA "使用CUDA GPU加速" ON)
if(USE_CUDA)
    # 检查OpenCV是否编译了CUDA支持（通过检查OpenCV_CUDA_VERSION变量）
    if(DEFINED OpenCV_CUDA_VERSION AND OpenCV_CUDA_VERSION)
        message(STATUS "CUDA GPU加速: 已启用")
        message(STATUS "OpenCV CUDA版本: ${OpenCV_CUDA_VERSION}")
        add_definitions(-DUSE_CUDA)

        # 查找CUDA Toolkit（OpenCV CUDA版本需要）
        find_package(CUDAToolkit QUIET)
        if(CUDAToolkit_FOUND)
            message(STATUS "CUDA Toolkit版本: ${CUDAToolkit_VERSION}")
        endif()
    else()
        # 备用检测：检查OpenCV组件列表中是否包含cuda模块
        set(OPENCV_HAS_CUDA_MODULE FALSE)
        foreach(lib ${OpenCV_LIB_COMPONENTS})
            if(lib MATCHES "opencv_cuda")
                set(OPENCV_HAS_CUDA_MODULE TRUE)
                break()
            endif()
        endforeach()

        if(OPENCV_HAS_CUDA_MODULE)
            message(STATUS "CUDA GPU加速: 已启用 (通过组件检测)")
            add_definitions(-DUSE_CUDA)
        else()
            message(WARNING "OpenCV未包含CUDA模块，GPU加速将不可用")
            set(USE_CUDA OFF)
        endif()
    endif()
endif()

# ============================================================
# 包含目录
# ============================================================
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# ============================================================
# 编译选项定义
# ============================================================
add_definitions(
    -DVISIONFORGE_VERSION="${PROJECT_VERSION}"
    -DVISIONFORGE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    -DVISIONFORGE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    -DVISIONFORGE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

if(USE_HALCON)
    add_definitions(-DUSE_HALCON)
endif()

if(USE_ONNX_RUNTIME)
    add_definitions(-DUSE_ONNX_RUNTIME)
endif()

if(USE_TENSORRT)
    add_definitions(-DUSE_TENSORRT)
endif()

# ============================================================
# 子目录
# ============================================================
add_subdirectory(src)

# 测试（可选）
option(BUILD_TESTS "编译测试" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# 示例（可选）
option(BUILD_EXAMPLES "编译示例" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ============================================================
# 安装配置
# ============================================================
install(TARGETS VisionForge
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装资源文件
install(DIRECTORY resources/
    DESTINATION bin/resources
    PATTERN "*.png"
    PATTERN "*.qss"
    PATTERN "*.qm"
)

# 安装文档
install(FILES
    README.md
    LICENSE
    DESTINATION docs
)

# ============================================================
# CPack 打包配置
# ============================================================
set(CPACK_PACKAGE_NAME "VisionForge Pro")
set(CPACK_PACKAGE_VENDOR "VisionForge Team")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VisionForge Pro")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "VisionForge Pro")
    set(CPACK_NSIS_PACKAGE_NAME "VisionForge Pro ${PROJECT_VERSION}")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
endif()

include(CPack)

# ============================================================
# 输出配置总结
# ============================================================
message(STATUS "===========================================")
message(STATUS "配置选项:")
message(STATUS "  USE_HALCON: ${USE_HALCON}")
message(STATUS "  USE_HIKVISION_MVS: ${USE_HIKVISION_MVS}")
message(STATUS "  USE_BASLER_PYLON: ${USE_BASLER_PYLON}")
message(STATUS "  USE_ONNX_RUNTIME: ${USE_ONNX_RUNTIME}")
message(STATUS "  USE_TENSORRT: ${USE_TENSORRT}")
message(STATUS "  USE_OPENMP: ${USE_OPENMP}")
message(STATUS "  USE_CUDA: ${USE_CUDA}")
message(STATUS "  BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "  BUILD_EXAMPLES: ${BUILD_EXAMPLES}")
message(STATUS "===========================================")
message(STATUS "输出目录:")
message(STATUS "  可执行文件: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  库文件: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "===========================================")
