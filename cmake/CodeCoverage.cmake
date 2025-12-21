# CodeCoverage.cmake - 代码覆盖率支持
# 适用于GCC/Clang编译器，生成lcov/gcov覆盖率报告

option(ENABLE_COVERAGE "启用代码覆盖率" OFF)

if(ENABLE_COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "代码覆盖率通常在Debug模式下使用，当前构建类型: ${CMAKE_BUILD_TYPE}")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "✅ 启用代码覆盖率")

        # 添加编译器覆盖率选项
        set(COVERAGE_COMPILER_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")

        # 添加链接器覆盖率选项
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")

        message(STATUS "  编译器标志: ${COVERAGE_COMPILER_FLAGS}")

        # 查找lcov和genhtml工具
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)

        if(LCOV_PATH AND GENHTML_PATH)
            message(STATUS "  lcov路径: ${LCOV_PATH}")
            message(STATUS "  genhtml路径: ${GENHTML_PATH}")

            # 添加覆盖率清理目标
            add_custom_target(coverage-clean
                COMMAND ${LCOV_PATH} --directory . --zerocounters
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "清理覆盖率计数器"
            )

            # 添加覆盖率生成目标
            add_custom_target(coverage
                # 清理旧数据
                COMMAND ${LCOV_PATH} --directory . --zerocounters

                # 运行测试
                COMMAND ${CMAKE_CTEST_COMMAND} --verbose

                # 捕获覆盖率数据
                COMMAND ${LCOV_PATH} --capture --directory . --output-file coverage.info

                # 过滤系统文件和测试文件
                COMMAND ${LCOV_PATH} --remove coverage.info '/usr/*' '*/test/*' '*/build/*' '*/googletest/*' --output-file coverage_filtered.info

                # 生成HTML报告
                COMMAND ${GENHTML_PATH} coverage_filtered.info --output-directory coverage_html

                # 显示摘要
                COMMAND ${LCOV_PATH} --list coverage_filtered.info

                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "生成代码覆盖率报告 (HTML: build/coverage_html/index.html)"
            )

            message(STATUS "  可用目标:")
            message(STATUS "    - coverage-clean: 清理覆盖率数据")
            message(STATUS "    - coverage: 运行测试并生成覆盖率报告")
            message(STATUS "  报告位置: build/coverage_html/index.html")
        else()
            message(WARNING "未找到lcov或genhtml，无法生成HTML覆盖率报告")
            if(NOT LCOV_PATH)
                message(STATUS "  请安装: sudo apt-get install lcov")
            endif()
        endif()

        # 查找gcovr（可选）
        find_program(GCOVR_PATH gcovr)
        if(GCOVR_PATH)
            message(STATUS "  gcovr路径: ${GCOVR_PATH}")

            # 添加XML格式覆盖率报告（适用于CI）
            add_custom_target(coverage-xml
                COMMAND ${CMAKE_CTEST_COMMAND} --verbose
                COMMAND ${GCOVR_PATH} --xml-pretty --exclude-unreachable-branches --print-summary -o coverage.xml --root ${CMAKE_SOURCE_DIR}
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "生成代码覆盖率报告 (XML: build/coverage.xml)"
            )

            message(STATUS "    - coverage-xml: 生成XML格式覆盖率报告（用于CI）")
        endif()

    elseif(MSVC)
        message(WARNING "MSVC编译器需要使用OpenCppCoverage等第三方工具，暂不支持")
        message(STATUS "  Windows下推荐使用: https://github.com/OpenCppCoverage/OpenCppCoverage")
    else()
        message(WARNING "当前编译器不支持代码覆盖率: ${CMAKE_CXX_COMPILER_ID}")
    endif()
else()
    message(STATUS "代码覆盖率已禁用 (使用 -DENABLE_COVERAGE=ON 启用)")
endif()

# 函数：为特定目标添加覆盖率排除
function(exclude_from_coverage target)
    if(ENABLE_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
        target_compile_options(${target} PRIVATE --coverage -fno-profile-arcs -fno-test-coverage)
    endif()
endfunction()
