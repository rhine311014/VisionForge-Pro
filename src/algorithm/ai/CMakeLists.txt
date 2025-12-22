# ============================================================
# VisionForge Pro - AI深度学习模块 CMakeLists.txt
# ============================================================
# P6 AI Detection Module
# 功能:
# - 推理引擎 (ONNX Runtime, TensorRT, OpenCV DNN)
# - YOLO检测器 (v5/v8/v11, 检测/分割/姿态/OBB)
# - 实例分割 (YOLOv8-seg, Mask R-CNN, SAM)
# - OCR文字识别 (PaddleOCR/PPOCR)
# - 异常检测 (PatchCore, FastFlow, STFPM)
# - 模型管理与转换
# ============================================================

message(STATUS "配置AI深度学习模块...")

# ============================================================
# 源文件
# ============================================================
set(AI_SOURCES
    InferenceEngine.cpp
    ONNXRuntimeEngine.cpp
    TensorRTEngine.cpp
    YOLODetector.cpp
    InstanceSegmentation.cpp
    OCREngine.cpp
    AnomalyDetector.cpp
    AIModelManager.cpp
)

set(AI_HEADERS
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/InferenceEngine.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/ONNXRuntimeEngine.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/TensorRTEngine.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/YOLODetector.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/InstanceSegmentation.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/OCREngine.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/AnomalyDetector.h
    ${CMAKE_SOURCE_DIR}/include/algorithm/ai/AIModelManager.h
)

# ============================================================
# 创建静态库
# ============================================================
add_library(VisionForgeAI STATIC
    ${AI_SOURCES}
    ${AI_HEADERS}
)

# 设置目标属性
set_target_properties(VisionForgeAI PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForgeAI PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# ONNX Runtime支持 (可选)
# ============================================================
option(USE_ONNX_RUNTIME "Enable ONNX Runtime support" OFF)

if(USE_ONNX_RUNTIME)
    find_package(onnxruntime QUIET)

    if(onnxruntime_FOUND)
        message(STATUS "AI: ONNX Runtime found")
        target_compile_definitions(VisionForgeAI PUBLIC USE_ONNX_RUNTIME)
        target_link_libraries(VisionForgeAI PUBLIC onnxruntime::onnxruntime)
    else()
        # 尝试手动查找
        set(ONNXRUNTIME_ROOT "" CACHE PATH "ONNX Runtime installation directory")

        if(EXISTS "${ONNXRUNTIME_ROOT}/include/onnxruntime/onnxruntime_cxx_api.h")
            message(STATUS "AI: ONNX Runtime found at ${ONNXRUNTIME_ROOT}")
            target_compile_definitions(VisionForgeAI PUBLIC USE_ONNX_RUNTIME)
            target_include_directories(VisionForgeAI PUBLIC
                ${ONNXRUNTIME_ROOT}/include
            )

            if(WIN32)
                target_link_libraries(VisionForgeAI PUBLIC
                    ${ONNXRUNTIME_ROOT}/lib/onnxruntime.lib
                )
            else()
                target_link_libraries(VisionForgeAI PUBLIC
                    ${ONNXRUNTIME_ROOT}/lib/libonnxruntime.so
                )
            endif()
        else()
            message(WARNING "AI: ONNX Runtime enabled but not found. Set ONNXRUNTIME_ROOT.")
        endif()
    endif()
endif()

# ============================================================
# TensorRT支持 (可选, 仅NVIDIA GPU)
# ============================================================
option(USE_TENSORRT "Enable TensorRT support (requires NVIDIA GPU)" OFF)

if(USE_TENSORRT)
    find_package(CUDA QUIET)

    if(CUDA_FOUND)
        # 查找TensorRT
        set(TENSORRT_ROOT "" CACHE PATH "TensorRT installation directory")

        if(EXISTS "${TENSORRT_ROOT}/include/NvInfer.h")
            message(STATUS "AI: TensorRT found at ${TENSORRT_ROOT}")
            target_compile_definitions(VisionForgeAI PUBLIC USE_TENSORRT)

            target_include_directories(VisionForgeAI PUBLIC
                ${CUDA_INCLUDE_DIRS}
                ${TENSORRT_ROOT}/include
            )

            target_link_libraries(VisionForgeAI PUBLIC
                ${CUDA_LIBRARIES}
                ${CUDA_CUBLAS_LIBRARIES}
            )

            if(WIN32)
                target_link_libraries(VisionForgeAI PUBLIC
                    ${TENSORRT_ROOT}/lib/nvinfer.lib
                    ${TENSORRT_ROOT}/lib/nvonnxparser.lib
                )
            else()
                target_link_libraries(VisionForgeAI PUBLIC
                    ${TENSORRT_ROOT}/lib/libnvinfer.so
                    ${TENSORRT_ROOT}/lib/libnvonnxparser.so
                )
            endif()
        else()
            message(WARNING "AI: TensorRT enabled but not found. Set TENSORRT_ROOT.")
        endif()
    else()
        message(WARNING "AI: TensorRT requires CUDA, but CUDA not found")
    endif()
endif()

# ============================================================
# 链接依赖库
# ============================================================
target_link_libraries(VisionForgeAI PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    ${OpenCV_LIBS}
    VisionForgeBase
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForgeAI PRIVATE
        /utf-8
        /W4
        /WX-
        /bigobj
    )
    # 禁用一些警告
    target_compile_options(VisionForgeAI PRIVATE
        /wd4127  # conditional expression is constant
        /wd4244  # conversion warnings
        /wd4267  # size_t to int warnings
        /wd4456  # local variable hiding
    )
else()
    target_compile_options(VisionForgeAI PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
endif()

# Windows平台定义
if(WIN32)
    target_compile_definitions(VisionForgeAI PRIVATE NOMINMAX)
endif()

# ============================================================
# 预编译头
# ============================================================
target_precompile_headers(VisionForgeAI PRIVATE
    <QObject>
    <QString>
    <QImage>
    <QJsonObject>
    <QJsonArray>
    <QMutex>
    <opencv2/core.hpp>
    <opencv2/imgproc.hpp>
    <opencv2/dnn.hpp>
    <memory>
    <vector>
    <string>
    <chrono>
)

# ============================================================
# 安装规则
# ============================================================
install(TARGETS VisionForgeAI
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES ${AI_HEADERS}
    DESTINATION include/algorithm/ai
)

# ============================================================
# 输出信息
# ============================================================
message(STATUS "AI深度学习模块配置完成")
message(STATUS "  - 功能: 推理引擎, YOLO检测, 实例分割, OCR, 异常检测, 模型管理")
message(STATUS "  - ONNX Runtime: ${USE_ONNX_RUNTIME}")
message(STATUS "  - TensorRT: ${USE_TENSORRT}")
