# ============================================================
# VisionForge Pro - 单元测试配置
# ============================================================

message(STATUS "===========================================")
message(STATUS "配置单元测试...")
message(STATUS "===========================================")

# 查找Qt Test模块
find_package(Qt6 REQUIRED COMPONENTS Test)

# 可选：集成Google Test
option(USE_GTEST "使用Google Test框架" ON)

if(USE_GTEST)
    include(${CMAKE_SOURCE_DIR}/cmake/GoogleTest.cmake)
    include(GoogleTest)  # CMake模块，提供gtest_discover_tests命令
    enable_testing()
    message(STATUS "Google Test已启用")
endif()

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
# 测试可执行文件宏（Qt Test）
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
# Google Test测试宏
# ============================================================
if(USE_GTEST)
    macro(add_gtest_test TEST_NAME TEST_SOURCE)
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME} PRIVATE
            gtest
            gtest_main
            gmock
            Qt6::Core
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
        # 添加到CTest
        gtest_discover_tests(${TEST_NAME})
        set_target_properties(${TEST_NAME} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
        )
    endmacro()
endif()

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
# Google Test测试（可选）
# ============================================================
if(USE_GTEST)
    message(STATUS "配置Google Test测试...")

    # 添加Google Test测试
    add_gtest_test(test_ImageData_gtest test_ImageData_gtest.cpp)
    add_gtest_test(test_BasicImageProcessing_gtest test_BasicImageProcessing_gtest.cpp)
    add_gtest_test(test_EdgeAndMorphology_gtest test_EdgeAndMorphology_gtest.cpp)
    add_gtest_test(test_ShapeDetection_gtest test_ShapeDetection_gtest.cpp)
    add_gtest_test(test_Measurement_gtest test_Measurement_gtest.cpp)
    add_gtest_test(test_AdvancedTools_gtest test_AdvancedTools_gtest.cpp)
    add_gtest_test(test_MatchingTools_gtest test_MatchingTools_gtest.cpp)
    add_gtest_test(test_CalculationTools_gtest test_CalculationTools_gtest.cpp)
    add_gtest_test(test_LogicTools_gtest test_LogicTools_gtest.cpp)

    # 远程诊断安全模块测试
    add_executable(test_SecurityManager_gtest test_SecurityManager_gtest.cpp)
    target_link_libraries(test_SecurityManager_gtest
        VisionForgeRemote
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Sql
    )
    gtest_discover_tests(test_SecurityManager_gtest)

    # TokenAuthManager测试
    add_executable(test_TokenAuthManager_gtest test_TokenAuthManager_gtest.cpp)
    target_link_libraries(test_TokenAuthManager_gtest
        VisionForgeRemote
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
    )
    gtest_discover_tests(test_TokenAuthManager_gtest)

    # RBACManager测试
    add_executable(test_RBACManager_gtest test_RBACManager_gtest.cpp)
    target_link_libraries(test_RBACManager_gtest
        VisionForgeRemote
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Sql
    )
    gtest_discover_tests(test_RBACManager_gtest)

    # Platform模块测试（P0核心）
    add_executable(test_Platform_gtest test_Platform_gtest.cpp)
    target_link_libraries(test_Platform_gtest
        VisionForgePlatform
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Gui
    )
    gtest_discover_tests(test_Platform_gtest)

    # Calibration模块测试（P1标定对齐）
    add_executable(test_Calibration_gtest test_Calibration_gtest.cpp)
    target_link_libraries(test_Calibration_gtest
        VisionForgeCalibration
        VisionForgePlatform
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Gui
    )
    gtest_discover_tests(test_Calibration_gtest)

    # PLC通信模块测试（工业级PLC协议）
    add_executable(test_PLC_gtest test_PLC_gtest.cpp)
    target_link_libraries(test_PLC_gtest
        VisionForgePLC
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        Qt6::Core
        Qt6::Test
        Qt6::Network
    )
    set_target_properties(test_PLC_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
    )
    gtest_discover_tests(test_PLC_gtest)

    message(STATUS "  - test_ImageData_gtest (GTest)")
    message(STATUS "  - test_BasicImageProcessing_gtest (GTest)")
    message(STATUS "  - test_EdgeAndMorphology_gtest (GTest)")
    message(STATUS "  - test_ShapeDetection_gtest (GTest)")
    message(STATUS "  - test_Measurement_gtest (GTest)")
    message(STATUS "  - test_AdvancedTools_gtest (GTest)")
    message(STATUS "  - test_MatchingTools_gtest (GTest)")
    message(STATUS "  - test_CalculationTools_gtest (GTest)")
    message(STATUS "  - test_LogicTools_gtest (GTest)")
    message(STATUS "  - test_SecurityManager_gtest (GTest - 安全模块)")
    message(STATUS "  - test_TokenAuthManager_gtest (GTest - Token认证)")
    message(STATUS "  - test_RBACManager_gtest (GTest - RBAC权限管理)")
    message(STATUS "  - test_Platform_gtest (GTest - P0平台核心)")
    message(STATUS "  - test_Calibration_gtest (GTest - P1标定对齐)")
    message(STATUS "  - test_PLC_gtest (GTest - 工业级PLC通信)")

    # 3D算法模块测试（P5 3D算法集成）
    # 注意：Eigen通过VisionForgeAlgorithm3D的PUBLIC链接传递
    set(EIGEN3_INCLUDE_DIR "F:/机器视觉/eigen-5.0.0")
    add_executable(test_Algorithm3D_gtest test_Algorithm3D_gtest.cpp)
    target_include_directories(test_Algorithm3D_gtest PRIVATE ${EIGEN3_INCLUDE_DIR})
    target_link_libraries(test_Algorithm3D_gtest
        VisionForgeAlgorithm3D
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Gui
    )
    set_target_properties(test_Algorithm3D_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
    )
    gtest_discover_tests(test_Algorithm3D_gtest)
    message(STATUS "  - test_Algorithm3D_gtest (GTest - P5 3D算法)")

    # AI深度学习模块测试（P6 AI检测算法）
    add_executable(test_AI_gtest test_AI_gtest.cpp)
    target_link_libraries(test_AI_gtest
        VisionForgeAI
        VisionForgeBase
        GTest::gtest
        GTest::gtest_main
        Qt6::Core
        Qt6::Test
        Qt6::Gui
        Qt6::Network
        ${OpenCV_LIBS}
    )
    set_target_properties(test_AI_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
    )
    gtest_discover_tests(test_AI_gtest)
    message(STATUS "  - test_AI_gtest (GTest - P6 AI深度学习)")
endif()

# ============================================================
# 测试输出
# ============================================================
message(STATUS "单元测试配置完成")
message(STATUS "Qt Test测试:")
message(STATUS "  - test_ImageData")
message(STATUS "  - test_ImageMemoryPool")
message(STATUS "  - test_ConfigManager")
message(STATUS "  - test_Logger")
message(STATUS "  - test_VisionTool")
message(STATUS "  - test_GPUAccelerator (P1)")
message(STATUS "  - test_ParallelProcessor (P1)")
message(STATUS "  - test_ErrorRecovery (P1)")
message(STATUS "  - test_ToolChain (P1)")
if(USE_GTEST)
    message(STATUS "Google Test测试:")
    message(STATUS "  - test_ImageData_gtest")
endif()
message(STATUS "===========================================")
