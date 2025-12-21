# ============================================================
# VisionForge Pro - src/plc/CMakeLists.txt
# 工业级PLC通信模块
# ============================================================

message(STATUS "配置PLC通信模块...")

# ============================================================
# 源文件
# ============================================================
set(PLC_SOURCES
    PLCTypes.cpp
    PLCProtocol.cpp
    PLCClient.cpp
)

set(PLC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/plc/PLCTypes.h
    ${CMAKE_SOURCE_DIR}/include/plc/PLCProtocol.h
    ${CMAKE_SOURCE_DIR}/include/plc/PLCClient.h
)

# ============================================================
# 创建静态库
# ============================================================
add_library(VisionForgePLC STATIC
    ${PLC_SOURCES}
    ${PLC_HEADERS}
)

# 设置目标属性
set_target_properties(VisionForgePLC PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)

# ============================================================
# 包含目录
# ============================================================
target_include_directories(VisionForgePLC PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============================================================
# 链接依赖库
# ============================================================
target_link_libraries(VisionForgePLC PUBLIC
    Qt6::Core
    Qt6::Network
    VisionForgeBase
)

# ============================================================
# 编译选项
# ============================================================
if(MSVC)
    target_compile_options(VisionForgePLC PRIVATE
        /utf-8
        /W4
        /WX-
    )
else()
    target_compile_options(VisionForgePLC PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# 预编译头
# ============================================================
target_precompile_headers(VisionForgePLC PRIVATE
    <QObject>
    <QString>
    <QByteArray>
    <QTcpSocket>
    <QTimer>
    <QMutex>
    <memory>
    <vector>
    <functional>
)

# ============================================================
# 安装规则
# ============================================================
install(TARGETS VisionForgePLC
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES ${PLC_HEADERS}
    DESTINATION include/plc
)

message(STATUS "PLC通信模块配置完成")
message(STATUS "  - 协议支持: Modbus TCP, Mitsubishi MC 3E")
message(STATUS "  - 特性: 异步通信, 自动重连, 心跳检测")
