/**
 * @file CrashHandler.h
 * @brief 崩溃处理和转储系统
 * @details 捕获应用程序崩溃，生成诊断信息
 *
 * 功能特性：
 * - 异常捕获：捕获未处理的C++异常和系统异常
 * - MiniDump生成：生成Windows .dmp文件
 * - 堆栈跟踪：记录崩溃时的调用堆栈
 * - 系统信息：收集崩溃时的系统状态
 * - 远程上报：支持将崩溃报告上传到服务器
 *
 * @author VisionForge Team
 * @date 2025-12-22
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include <functional>
#include <memory>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dbghelp.h>
#endif

namespace VisionForge {
namespace Base {

/**
 * @brief 崩溃信息结构
 */
struct CrashInfo {
    QString applicationName;        ///< 应用程序名称
    QString applicationVersion;     ///< 应用程序版本
    QDateTime crashTime;            ///< 崩溃时间
    QString exceptionType;          ///< 异常类型
    QString exceptionMessage;       ///< 异常消息
    quint64 exceptionAddress;       ///< 异常地址
    QString moduleName;             ///< 发生异常的模块
    QStringList stackTrace;         ///< 堆栈跟踪
    QString dumpFilePath;           ///< MiniDump文件路径
    QString logFilePath;            ///< 日志文件路径

    // 系统信息
    QString osVersion;              ///< 操作系统版本
    QString cpuInfo;                ///< CPU信息
    qint64 totalMemoryMB;           ///< 总内存(MB)
    qint64 availableMemoryMB;       ///< 可用内存(MB)
    qint64 processMemoryMB;         ///< 进程内存使用(MB)

    QJsonObject toJson() const;
    QString toText() const;
};

/**
 * @brief 崩溃处理配置
 */
struct CrashHandlerConfig {
    QString dumpDirectory;          ///< 转储文件目录
    QString applicationName;        ///< 应用程序名称
    QString applicationVersion;     ///< 应用程序版本
    bool enableMiniDump = true;     ///< 启用MiniDump
    bool enableStackTrace = true;   ///< 启用堆栈跟踪
    bool enableRemoteReport = false;///< 启用远程上报
    QString reportServerUrl;        ///< 上报服务器URL
    int maxDumpFiles = 10;          ///< 最大保留转储文件数
    bool restartOnCrash = false;    ///< 崩溃后自动重启
    QString restartCommand;         ///< 重启命令

    QJsonObject toJson() const;
    static CrashHandlerConfig fromJson(const QJsonObject& json);
};

/**
 * @class CrashHandler
 * @brief 崩溃处理器单例类
 *
 * 使用方法：
 * @code
 * // 在main()开始时初始化
 * CrashHandler::instance().initialize(config);
 *
 * // 程序正常退出前清理
 * CrashHandler::instance().shutdown();
 * @endcode
 */
class CrashHandler
{
public:
    using CrashCallback = std::function<void(const CrashInfo&)>;

    /**
     * @brief 获取单例实例
     */
    static CrashHandler& instance();

    /**
     * @brief 初始化崩溃处理器
     * @param config 配置
     * @return 是否成功
     */
    bool initialize(const CrashHandlerConfig& config);

    /**
     * @brief 关闭崩溃处理器
     */
    void shutdown();

    /**
     * @brief 是否已初始化
     */
    bool isInitialized() const { return initialized_; }

    /**
     * @brief 获取配置
     */
    CrashHandlerConfig config() const { return config_; }

    /**
     * @brief 设置崩溃回调
     */
    void setCrashCallback(CrashCallback callback) { crashCallback_ = callback; }

    /**
     * @brief 手动触发崩溃报告（用于测试）
     */
    void triggerTestCrash();

    /**
     * @brief 手动创建转储文件
     * @param reason 创建原因
     * @return 转储文件路径
     */
    QString createManualDump(const QString& reason);

    /**
     * @brief 获取最近的崩溃信息
     */
    CrashInfo lastCrashInfo() const { return lastCrashInfo_; }

    /**
     * @brief 清理旧的转储文件
     */
    void cleanupOldDumps();

    /**
     * @brief 获取所有转储文件列表
     */
    QStringList getDumpFiles() const;

    /**
     * @brief 上传崩溃报告
     * @param info 崩溃信息
     * @return 是否成功
     */
    bool uploadCrashReport(const CrashInfo& info);

private:
    CrashHandler();
    ~CrashHandler();
    CrashHandler(const CrashHandler&) = delete;
    CrashHandler& operator=(const CrashHandler&) = delete;

    // 内部方法
    void setupExceptionHandlers();
    void removeExceptionHandlers();
    QString generateDumpFileName() const;
    void collectSystemInfo(CrashInfo& info);
    void writeTextReport(const CrashInfo& info);

#ifdef Q_OS_WIN
    static LONG WINAPI unhandledExceptionFilter(EXCEPTION_POINTERS* exceptionInfo);
    static void invalidParameterHandler(const wchar_t* expression,
                                        const wchar_t* function,
                                        const wchar_t* file,
                                        unsigned int line,
                                        uintptr_t reserved);
    static void pureCallHandler();
    static void signalHandler(int signal);

    bool writeMiniDump(EXCEPTION_POINTERS* exceptionInfo, const QString& dumpPath);
    void captureStackTrace(CrashInfo& info, CONTEXT* context);
    QString getExceptionDescription(DWORD code);
#endif

private:
    bool initialized_ = false;
    CrashHandlerConfig config_;
    CrashInfo lastCrashInfo_;
    CrashCallback crashCallback_;

#ifdef Q_OS_WIN
    LPTOP_LEVEL_EXCEPTION_FILTER previousFilter_ = nullptr;
    _invalid_parameter_handler previousInvalidHandler_ = nullptr;
    _purecall_handler previousPureCallHandler_ = nullptr;
#endif

    static CrashHandler* instance_;
};

// ============================================================
// 辅助宏定义
// ============================================================

/**
 * @brief 初始化崩溃处理器的便捷宏
 */
#define VISIONFORGE_INIT_CRASH_HANDLER(appName, appVersion, dumpDir) \
    do { \
        VisionForge::Base::CrashHandlerConfig cfg; \
        cfg.applicationName = appName; \
        cfg.applicationVersion = appVersion; \
        cfg.dumpDirectory = dumpDir; \
        VisionForge::Base::CrashHandler::instance().initialize(cfg); \
    } while(0)

/**
 * @brief 关闭崩溃处理器的便捷宏
 */
#define VISIONFORGE_SHUTDOWN_CRASH_HANDLER() \
    VisionForge::Base::CrashHandler::instance().shutdown()

} // namespace Base
} // namespace VisionForge
