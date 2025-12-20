# ============================================================
# 核心业务层 (Core Layer)
# ============================================================

# 收集源文件
set(CORE_SOURCES
    ToolChain.cpp
    TaskScheduler.cpp
    VisionProject.cpp
    Recipe.cpp
    RecipeManager.cpp
    WorkStation.cpp
)

# 收集头文件
set(CORE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/core/ToolChain.h
    ${CMAKE_SOURCE_DIR}/include/core/TaskScheduler.h
    ${CMAKE_SOURCE_DIR}/include/core/VisionProject.h
    ${CMAKE_SOURCE_DIR}/include/core/Recipe.h
    ${CMAKE_SOURCE_DIR}/include/core/RecipeManager.h
    ${CMAKE_SOURCE_DIR}/include/core/WorkStation.h
)

# 创建核心业务层静态库
add_library(VisionForgeCore STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeCore PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(VisionForgeCore PUBLIC
    VisionForgeBase
    VisionForgeAlgorithm
    VisionForgeHAL
    Qt6::Core
    ${OpenCV_LIBS}
)

# 如果启用Halcon，添加编译定义
if(USE_HALCON)
    target_compile_definitions(VisionForgeCore PRIVATE USE_HALCON)
endif()

# 设置属性
set_target_properties(VisionForgeCore PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# 输出信息
message(STATUS "核心业务层 (VisionForgeCore) 配置完成")
