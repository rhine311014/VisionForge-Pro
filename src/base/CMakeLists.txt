# ============================================================
# 基础设施层 (Base Layer)
# ============================================================

# 收集源文件
set(BASE_SOURCES
    ImageData.cpp
    AlignedMemory.cpp
    ImageMemoryPool.cpp
    Logger.cpp
    ConfigManager.cpp
    PerformanceMonitor.cpp
    GPUAccelerator.cpp
    TranslationManager.cpp
    PermissionManager.cpp
    DataExporter.cpp
    PluginManager.cpp
    SystemMonitor.cpp
)

# 收集头文件
set(BASE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/base/ImageData.h
    ${CMAKE_SOURCE_DIR}/include/base/AlignedMemory.h
    ${CMAKE_SOURCE_DIR}/include/base/ImageMemoryPool.h
    ${CMAKE_SOURCE_DIR}/include/base/Logger.h
    ${CMAKE_SOURCE_DIR}/include/base/ConfigManager.h
    ${CMAKE_SOURCE_DIR}/include/base/PerformanceMonitor.h
    ${CMAKE_SOURCE_DIR}/include/base/GPUAccelerator.h
    ${CMAKE_SOURCE_DIR}/include/base/TranslationManager.h
    ${CMAKE_SOURCE_DIR}/include/base/PermissionManager.h
    ${CMAKE_SOURCE_DIR}/include/base/DataExporter.h
    ${CMAKE_SOURCE_DIR}/include/base/PluginManager.h
    ${CMAKE_SOURCE_DIR}/include/base/SystemMonitor.h
)

# 创建基础设施层静态库
add_library(VisionForgeBase STATIC
    ${BASE_SOURCES}
    ${BASE_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeBase PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(VisionForgeBase PUBLIC
    Qt6::Core
    Qt6::Gui
    ${OpenCV_LIBS}
)

# CUDA支持（可选）
if(USE_CUDA AND CUDA_FOUND)
    target_include_directories(VisionForgeBase PRIVATE ${CUDA_INCLUDE_DIRS})
    target_link_libraries(VisionForgeBase PUBLIC ${CUDA_LIBRARIES})
    target_compile_definitions(VisionForgeBase PRIVATE USE_CUDA)
endif()

# 设置属性
set_target_properties(VisionForgeBase PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# 输出信息
message(STATUS "基础设施层 (VisionForgeBase) 配置完成")
