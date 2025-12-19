# ============================================================
# VisionForge Pro - 单元测试配置
# ============================================================

message(STATUS "===========================================")
message(STATUS "配置单元测试...")
message(STATUS "===========================================")

# 查找Qt Test模块
find_package(Qt6 REQUIRED COMPONENTS Test)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# ============================================================
# 测试辅助库
# ============================================================
add_library(VisionForgeTestUtils STATIC
    TestUtils.cpp
    TestUtils.h
)

target_link_libraries(VisionForgeTestUtils PUBLIC
    Qt6::Core
    Qt6::Test
    VisionForgeBase
)

target_include_directories(VisionForgeTestUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# 测试可执行文件宏
# ============================================================
macro(add_visionforge_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE
        Qt6::Core
        Qt6::Test
        Qt6::Widgets
        VisionForgeTestUtils
        VisionForgeBase
        VisionForgeAlgorithm
        VisionForgeCore
        ${OpenCV_LIBS}
    )
    # 如果启用Halcon，链接Halcon库
    if(USE_HALCON)
        target_link_libraries(${TEST_NAME} PRIVATE
            halconcpp
            halcon
        )
    endif()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
    )
endmacro()

# ============================================================
# 基础层测试
# ============================================================
add_visionforge_test(test_ImageData test_ImageData.cpp)
add_visionforge_test(test_ImageMemoryPool test_ImageMemoryPool.cpp)
add_visionforge_test(test_ConfigManager test_ConfigManager.cpp)
add_visionforge_test(test_Logger test_Logger.cpp)

# ============================================================
# 算法层测试
# ============================================================
add_visionforge_test(test_VisionTool test_VisionTool.cpp)

# ============================================================
# 核心组件测试 (P1 - 高优先级)
# ============================================================
add_visionforge_test(test_GPUAccelerator test_GPUAccelerator.cpp)
add_visionforge_test(test_ParallelProcessor test_ParallelProcessor.cpp)
add_visionforge_test(test_ErrorRecovery test_ErrorRecovery.cpp)
add_visionforge_test(test_ToolChain test_ToolChain.cpp)

# GPU测试条件编译
if(USE_CUDA)
    target_compile_definitions(test_GPUAccelerator PRIVATE USE_CUDA)
    # 添加CUDA包含路径
    target_include_directories(test_GPUAccelerator PRIVATE ${CUDA_INCLUDE_DIRS})
endif()

# ============================================================
# 测试输出
# ============================================================
message(STATUS "单元测试配置完成")
message(STATUS "  - test_ImageData")
message(STATUS "  - test_ImageMemoryPool")
message(STATUS "  - test_ConfigManager")
message(STATUS "  - test_Logger")
message(STATUS "  - test_VisionTool")
message(STATUS "  - test_GPUAccelerator (P1)")
message(STATUS "  - test_ParallelProcessor (P1)")
message(STATUS "  - test_ErrorRecovery (P1)")
message(STATUS "  - test_ToolChain (P1)")
message(STATUS "===========================================")
