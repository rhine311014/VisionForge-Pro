# ============================================================
# 工具层 (Tools Layer)
# ============================================================
# 独立运行的工具程序

# ============================================================
# 工位配置工具 (StationConfigTool)
# ============================================================
add_executable(StationConfigTool WIN32
    StationConfigToolMain.cpp
)

# 包含目录
target_include_directories(StationConfigTool PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖库
target_link_libraries(StationConfigTool PRIVATE
    VisionForgeUI
    VisionForgeCore
    VisionForgeBase
    VisionForgePlatform
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# 设置属性
set_target_properties(StationConfigTool PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    # 输出到bin目录
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)

# 编译和链接选项
if(MSVC)
    target_compile_options(StationConfigTool PRIVATE /utf-8)
    # 解决LIBCMT与MSVCRT库冲突警告 (LNK4098)
    target_link_options(StationConfigTool PRIVATE /NODEFAULTLIB:libcmt)
endif()

# Windows下添加版本信息资源
if(WIN32)
    # 可选：添加图标和版本信息
    # target_sources(StationConfigTool PRIVATE ${CMAKE_SOURCE_DIR}/resources/StationConfigTool.rc)
endif()

# 部署Qt依赖
if(WIN32)
    # 使用windeployqt部署依赖
    add_custom_command(TARGET StationConfigTool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "部署 StationConfigTool Qt依赖..."
        COMMAND $<TARGET_FILE:Qt6::windeployqt>
            --no-compiler-runtime
            --no-translations
            $<TARGET_FILE:StationConfigTool>
        WORKING_DIRECTORY $<TARGET_FILE_DIR:StationConfigTool>
        COMMENT "运行 windeployqt 为 StationConfigTool 部署Qt依赖"
    )
endif()

# 输出信息
message(STATUS "工位配置工具 (StationConfigTool) 配置完成")
