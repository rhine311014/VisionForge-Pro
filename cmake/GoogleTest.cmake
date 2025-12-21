# ============================================================
# Google Test 集成配置
# ============================================================

# 使用FetchContent下载Google Test
include(FetchContent)

message(STATUS "正在配置Google Test...")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0  # 使用稳定版本
    GIT_SHALLOW    TRUE
)

# 防止Google Test覆盖父项目的编译器/链接器设置
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# 禁用Google Test安装
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

# 下载并配置Google Test
FetchContent_MakeAvailable(googletest)

# 设置包含目录（使其对所有测试可用）
include_directories(${googletest_SOURCE_DIR}/googletest/include)
include_directories(${googletest_SOURCE_DIR}/googlemock/include)

message(STATUS "Google Test 配置完成")
message(STATUS "  版本: 1.14.0")
message(STATUS "  GTest包含目录: ${googletest_SOURCE_DIR}/googletest/include")
message(STATUS "  GMock包含目录: ${googletest_SOURCE_DIR}/googlemock/include")
