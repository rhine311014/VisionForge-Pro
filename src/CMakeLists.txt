# ============================================================
# VisionForge Pro - src/CMakeLists.txt
# ============================================================

# ============================================================
# 源文件收集
# ============================================================

# 基础设施层源文件
file(GLOB_RECURSE BASE_SOURCES
    base/*.cpp
    base/*.h
)

# HAL层源文件
file(GLOB_RECURSE HAL_SOURCES
    hal/*.cpp
    hal/*.h
)

# 算法层源文件
file(GLOB_RECURSE ALGORITHM_SOURCES
    algorithm/*.cpp
    algorithm/*.h
)

# 核心业务层源文件
file(GLOB_RECURSE CORE_SOURCES
    core/*.cpp
    core/*.h
)

# UI层源文件
file(GLOB_RECURSE UI_SOURCES
    ui/*.cpp
    ui/*.h
    ui/*.ui
)

# 主程序
set(MAIN_SOURCES
    main.cpp
)

# ============================================================
# 资源文件
# ============================================================
set(RESOURCE_FILES
    ${CMAKE_SOURCE_DIR}/resources/resources.qrc
)

# ============================================================
# 创建可执行文件
# ============================================================
add_executable(VisionForge
    ${MAIN_SOURCES}
    ${BASE_SOURCES}
    ${HAL_SOURCES}
    ${ALGORITHM_SOURCES}
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${RESOURCE_FILES}
)

# ============================================================
# 链接库
# ============================================================
target_link_libraries(VisionForge PRIVATE
    # Qt库
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
    Qt6::Xml

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
endif()

# ============================================================
# Windows应用程序图标和信息
# ============================================================
if(WIN32)
    # 设置为Windows GUI应用（不显示控制台）
    set_target_properties(VisionForge PROPERTIES
        WIN32_EXECUTABLE ON
    )

    # 添加版本信息资源文件（需要创建）
    # target_sources(VisionForge PRIVATE ${CMAKE_SOURCE_DIR}/resources/app.rc)
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
endif()
