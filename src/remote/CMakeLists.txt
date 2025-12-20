# ============================================================
# VisionForge Pro - 远程诊断模块
# ============================================================

message(STATUS "配置远程诊断模块...")

# 查找Qt模块
find_package(Qt6 REQUIRED COMPONENTS WebSockets Network Sql)

# 源文件列表
set(REMOTE_SOURCES
    RemoteProtocol.cpp
    WebSocketServer.cpp
    PerformanceMonitor.cpp
    RemoteLogCollector.cpp
    RemoteDatabaseManager.cpp
    SecurityCertificateManager.cpp
    TokenAuthManager.cpp
    RBACManager.cpp
    SecurityManager.cpp
)

# 头文件列表
set(REMOTE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/remote/RemoteProtocol.h
    ${CMAKE_SOURCE_DIR}/include/remote/WebSocketServer.h
    ${CMAKE_SOURCE_DIR}/include/remote/PerformanceMonitor.h
    ${CMAKE_SOURCE_DIR}/include/remote/RemoteLogCollector.h
    ${CMAKE_SOURCE_DIR}/include/remote/RemoteDatabaseManager.h
    ${CMAKE_SOURCE_DIR}/include/remote/SecurityCertificateManager.h
    ${CMAKE_SOURCE_DIR}/include/remote/TokenAuthManager.h
    ${CMAKE_SOURCE_DIR}/include/remote/RBACManager.h
    ${CMAKE_SOURCE_DIR}/include/remote/SecurityManager.h
)

# 创建远程诊断库
add_library(VisionForgeRemote STATIC
    ${REMOTE_SOURCES}
    ${REMOTE_HEADERS}
)

# 链接依赖库
target_link_libraries(VisionForgeRemote PUBLIC
    Qt6::Core
    Qt6::WebSockets
    Qt6::Network
    Qt6::Sql
)

# 包含目录
target_include_directories(VisionForgeRemote PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 设置编译属性
set_target_properties(VisionForgeRemote PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
    POSITION_INDEPENDENT_CODE ON
)

# 安装规则
install(TARGETS VisionForgeRemote
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

message(STATUS "远程诊断模块配置完成")
message(STATUS "  - RemoteProtocol (消息协议)")
message(STATUS "  - WebSocketServer (WebSocket服务器)")
message(STATUS "  - PerformanceMonitor (性能监控器)")
message(STATUS "  - RemoteLogCollector (日志收集器)")
message(STATUS "  - RemoteDatabaseManager (数据库管理器)")
message(STATUS "  - SecurityCertificateManager (SSL/TLS证书管理器)")
message(STATUS "  - TokenAuthManager (Token认证管理器)")
message(STATUS "  - RBACManager (RBAC权限管理器)")
message(STATUS "  - SecurityManager (安全统一管理器)")
