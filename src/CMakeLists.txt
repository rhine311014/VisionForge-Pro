# ============================================================
# VisionForge Pro - src/CMakeLists.txt
# ============================================================

# ============================================================
# 子模块目录
# ============================================================

# 平台核心模型（P0重构核心）
add_subdirectory(platform)

# 标定和对齐模块
add_subdirectory(calibration)

# 基础设施层
add_subdirectory(base)

# HAL层
add_subdirectory(hal)

# 算法层
add_subdirectory(algorithm)

# 3D算法模块（P5 3D算法集成）
add_subdirectory(algorithm3d)

# 核心业务层
add_subdirectory(core)

# 通信层
add_subdirectory(comm)

# 工业级PLC通信模块
add_subdirectory(plc)

# 远程诊断层
add_subdirectory(remote)

# UI层
add_subdirectory(ui)

# 独立工具程序
add_subdirectory(tools)

# ============================================================
# 主程序
# ============================================================

set(MAIN_SOURCES
    main.cpp
)

# ============================================================
# 资源文件（可选）
# ============================================================
# set(RESOURCE_FILES
#     ${CMAKE_SOURCE_DIR}/resources/resources.qrc
# )

# ============================================================
# 创建可执行文件
# ============================================================
add_executable(VisionForge
    ${MAIN_SOURCES}
    # ${RESOURCE_FILES}
)

# ============================================================
# 链接库
# ============================================================
target_link_libraries(VisionForge PRIVATE
    # 内部库
    VisionForgePlatform
    VisionForgeCalibration
    VisionForgeBase
    VisionForgeHAL
    VisionForgeAlgorithm
    VisionForgeAlgorithm3D
    VisionForgeCore
    VisionForgeComm
    VisionForgePLC
    VisionForgeRemote
    VisionForgeUI

    # Qt库
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
    Qt6::Xml
    Qt6::WebSockets

    # OpenCV库
    ${OpenCV_LIBS}
)

# Halcon库（可选）
if(USE_HALCON)
    target_link_libraries(VisionForge PRIVATE
        ${HALCONROOT}/lib/x64-win64/halconcpp.lib
    )
endif()

# ONNX Runtime库（可选）
if(USE_ONNX_RUNTIME)
    target_link_libraries(VisionForge PRIVATE
        onnxruntime
    )
endif()

# TensorRT库（可选）
if(USE_TENSORRT)
    target_link_libraries(VisionForge PRIVATE
        nvinfer
        nvonnxparser
        ${CUDA_LIBRARIES}
    )
endif()

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# ============================================================
# 预编译头（提升编译速度）
# ============================================================
target_precompile_headers(VisionForge PRIVATE
    <QtWidgets>
    <QtCore>
    <QtGui>
    <opencv2/opencv.hpp>
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForge PRIVATE
        /utf-8  # 使用UTF-8编码
    )
    # 解决LIBCMT与MSVCRT库冲突警告 (LNK4098)
    target_link_options(VisionForge PRIVATE
        /NODEFAULTLIB:libcmt
    )
endif()

# ============================================================
# Windows应用程序配置
# ============================================================
if(WIN32)
    # 暂时保留控制台窗口（方便调试）
    # set_target_properties(VisionForge PROPERTIES
    #     WIN32_EXECUTABLE ON
    # )
endif()

# ============================================================
# 安装规则
# ============================================================
install(TARGETS VisionForge
    RUNTIME DESTINATION bin
)

# Windows下使用windeployqt自动部署Qt DLL
if(WIN32)
    # 查找windeployqt
    get_target_property(QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET VisionForge POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --release
                --no-compiler-runtime
                --no-translations
                "$<TARGET_FILE:VisionForge>"
            COMMENT "运行 windeployqt 部署Qt依赖..."
        )
    endif()

    # 复制OpenCV DLL到输出目录（确保使用CUDA版本）
    if(OpenCV_FOUND AND OpenCV_DIR)
        set(OPENCV_WORLD_DLL "${OpenCV_DIR}/x64/vc17/bin/opencv_world${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}0.dll")
        if(EXISTS "${OPENCV_WORLD_DLL}")
            add_custom_command(TARGET VisionForge POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${OPENCV_WORLD_DLL}"
                    "$<TARGET_FILE_DIR:VisionForge>"
                COMMENT "复制OpenCV CUDA DLL到输出目录..."
            )
            message(STATUS "将复制OpenCV DLL: ${OPENCV_WORLD_DLL}")
        else()
            message(WARNING "OpenCV DLL未找到: ${OPENCV_WORLD_DLL}")
        endif()
    endif()

    # 复制CUDA DLL到输出目录（OpenCV CUDA版本依赖）
    # 自动检测CUDA Toolkit路径
    if(DEFINED ENV{CUDA_PATH})
        set(CUDA_TOOLKIT_ROOT "$ENV{CUDA_PATH}")
    elseif(EXISTS "D:/Program Files/CUDA12.5")
        set(CUDA_TOOLKIT_ROOT "D:/Program Files/CUDA12.5")
    elseif(EXISTS "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.5")
        set(CUDA_TOOLKIT_ROOT "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.5")
    endif()

    if(CUDA_TOOLKIT_ROOT AND EXISTS "${CUDA_TOOLKIT_ROOT}/bin")
        set(CUDA_BIN_DIR "${CUDA_TOOLKIT_ROOT}/bin")

        # CUDA运行时和NPP库列表
        set(CUDA_DLLS
            "cudart64_12.dll"
            "nppc64_12.dll"
            "nppial64_12.dll"
            "nppicc64_12.dll"
            "nppidei64_12.dll"
            "nppif64_12.dll"
            "nppig64_12.dll"
            "nppim64_12.dll"
            "nppist64_12.dll"
            "nppisu64_12.dll"
            "nppitc64_12.dll"
            "cufft64_11.dll"
            "cublas64_12.dll"
            "cublasLt64_12.dll"
        )

        foreach(DLL ${CUDA_DLLS})
            set(DLL_PATH "${CUDA_BIN_DIR}/${DLL}")
            if(EXISTS "${DLL_PATH}")
                add_custom_command(TARGET VisionForge POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${DLL_PATH}"
                        "$<TARGET_FILE_DIR:VisionForge>"
                )
            endif()
        endforeach()

        message(STATUS "将复制CUDA DLL从: ${CUDA_BIN_DIR}")
    else()
        message(WARNING "未找到CUDA Toolkit，跳过CUDA DLL复制")
    endif()

    # 复制cuDNN DLL到输出目录（深度学习推理依赖）
    # 自动检测cuDNN路径
    if(EXISTS "D:/Program Files/cuDNN 9.2.0/bin/12.9")
        set(CUDNN_BIN_DIR "D:/Program Files/cuDNN 9.2.0/bin/12.9")
    elseif(EXISTS "D:/Program Files/cuDNN/cudnn-windows-x86_64-8.9.7.29_cuda12-archive/bin")
        set(CUDNN_BIN_DIR "D:/Program Files/cuDNN/cudnn-windows-x86_64-8.9.7.29_cuda12-archive/bin")
    endif()

    if(CUDNN_BIN_DIR AND EXISTS "${CUDNN_BIN_DIR}")
        # cuDNN 9.x DLL列表
        set(CUDNN_DLLS
            "cudnn64_9.dll"
            "cudnn_adv64_9.dll"
            "cudnn_cnn64_9.dll"
            "cudnn_engines_precompiled64_9.dll"
            "cudnn_engines_runtime_compiled64_9.dll"
            "cudnn_graph64_9.dll"
            "cudnn_heuristic64_9.dll"
            "cudnn_ops64_9.dll"
        )

        foreach(DLL ${CUDNN_DLLS})
            set(DLL_PATH "${CUDNN_BIN_DIR}/${DLL}")
            if(EXISTS "${DLL_PATH}")
                add_custom_command(TARGET VisionForge POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${DLL_PATH}"
                        "$<TARGET_FILE_DIR:VisionForge>"
                )
            endif()
        endforeach()

        message(STATUS "将复制cuDNN DLL从: ${CUDNN_BIN_DIR}")
    else()
        message(WARNING "未找到cuDNN，跳过cuDNN DLL复制")
    endif()
endif()

# ============================================================
# 翻译文件编译配置
# ============================================================
# 收集翻译源文件
set(TS_FILES
    ${CMAKE_SOURCE_DIR}/translations/visionforge_en_US.ts
    ${CMAKE_SOURCE_DIR}/translations/visionforge_ja_JP.ts
)

# 编译.ts文件为.qm文件
qt_add_lrelease(VisionForge
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# 将翻译文件复制到输出目录
add_custom_command(TARGET VisionForge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:VisionForge>/translations"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} "$<TARGET_FILE_DIR:VisionForge>/translations"
    COMMENT "复制翻译文件到输出目录..."
)

# 安装翻译文件
install(FILES ${QM_FILES}
    DESTINATION bin/translations
)
