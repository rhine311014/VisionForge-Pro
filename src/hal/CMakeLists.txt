# ============================================================
# 硬件抽象层 (HAL Layer)
# ============================================================

# 基础源文件
set(HAL_SOURCES
    SimulatedCamera.cpp
    CameraFactory.cpp
)

# 基础头文件
set(HAL_HEADERS
    ${CMAKE_SOURCE_DIR}/include/hal/ICamera.h
    ${CMAKE_SOURCE_DIR}/include/hal/SimulatedCamera.h
    ${CMAKE_SOURCE_DIR}/include/hal/CameraFactory.h
)

# ============================================================
# 海康威视MVS SDK支持
# ============================================================
if(USE_HIKVISION_MVS)
    list(APPEND HAL_SOURCES HikvisionCamera.cpp)
    list(APPEND HAL_HEADERS ${CMAKE_SOURCE_DIR}/include/hal/HikvisionCamera.h)
    message(STATUS "HAL层: 启用海康威视MVS支持")
endif()

# ============================================================
# Basler Pylon SDK支持
# ============================================================
if(USE_BASLER_PYLON)
    list(APPEND HAL_SOURCES BaslerCamera.cpp)
    list(APPEND HAL_HEADERS ${CMAKE_SOURCE_DIR}/include/hal/BaslerCamera.h)
    message(STATUS "HAL层: 启用Basler Pylon支持")
endif()

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

# ============================================================
# 链接相机SDK
# ============================================================

# 海康威视MVS SDK
if(USE_HIKVISION_MVS)
    target_include_directories(VisionForgeHAL PRIVATE
        ${HIKVISION_MVS_ROOT}/Development/Includes
    )
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_link_libraries(VisionForgeHAL PRIVATE
            ${HIKVISION_MVS_ROOT}/Development/Libraries/win64/MvCameraControl.lib
        )
    else()
        target_link_libraries(VisionForgeHAL PRIVATE
            ${HIKVISION_MVS_ROOT}/Development/Libraries/win32/MvCameraControl.lib
        )
    endif()
    target_compile_definitions(VisionForgeHAL PRIVATE USE_HIKVISION_MVS)
endif()

# Basler Pylon SDK
if(USE_BASLER_PYLON)
    target_include_directories(VisionForgeHAL PRIVATE
        ${PYLON_ROOT}/include
    )
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64位
        file(GLOB PYLON_LIBS "${PYLON_ROOT}/lib/x64/*.lib")
        target_link_libraries(VisionForgeHAL PRIVATE ${PYLON_LIBS})
    else()
        # 32位
        file(GLOB PYLON_LIBS "${PYLON_ROOT}/lib/Win32/*.lib")
        target_link_libraries(VisionForgeHAL PRIVATE ${PYLON_LIBS})
    endif()
    target_compile_definitions(VisionForgeHAL PRIVATE USE_BASLER_PYLON)
endif()

# 设置属性
set_target_properties(VisionForgeHAL PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# MSVC编译选项
if(MSVC)
    target_compile_options(VisionForgeHAL PRIVATE /utf-8)
endif()

# 输出信息
message(STATUS "硬件抽象层 (VisionForgeHAL) 配置完成")
message(STATUS "  - 海康威视MVS: ${USE_HIKVISION_MVS}")
message(STATUS "  - Basler Pylon: ${USE_BASLER_PYLON}")
