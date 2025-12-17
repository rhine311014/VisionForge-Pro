# ============================================================
# VisionForge Pro - src/comm/CMakeLists.txt
# PLC通信模块
# ============================================================

set(COMM_SOURCES
    PLCComm.cpp
    ModbusTCPComm.cpp
    MitsubishiMCComm.cpp
    KeyencePCLinkComm.cpp
    PLCManager.cpp
)

set(COMM_HEADERS
    ${CMAKE_SOURCE_DIR}/include/comm/PLCDef.h
    ${CMAKE_SOURCE_DIR}/include/comm/PLCComm.h
    ${CMAKE_SOURCE_DIR}/include/comm/ModbusTCPComm.h
    ${CMAKE_SOURCE_DIR}/include/comm/MitsubishiMCComm.h
    ${CMAKE_SOURCE_DIR}/include/comm/KeyencePCLinkComm.h
    ${CMAKE_SOURCE_DIR}/include/comm/PLCManager.h
)

# 创建静态库
add_library(VisionForgeComm STATIC
    ${COMM_SOURCES}
    ${COMM_HEADERS}
)

# 包含目录
target_include_directories(VisionForgeComm PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接Qt库
target_link_libraries(VisionForgeComm PUBLIC
    Qt6::Core
    Qt6::Network
    VisionForgeBase
)

# 编译选项
if(MSVC)
    target_compile_options(VisionForgeComm PRIVATE
        /utf-8
    )
endif()
