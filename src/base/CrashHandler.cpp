/**
 * @file CrashHandler.cpp
 * @brief 崩溃处理和转储系统实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#include "base/CrashHandler.h"
#include "base/Logger.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QSysInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QEventLoop>

#ifdef Q_OS_WIN
#include <windows.h>
#include <dbghelp.h>
#include <psapi.h>
#include <signal.h>
#include <csignal>
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "psapi.lib")
#endif

namespace VisionForge {
namespace Base {

CrashHandler* CrashHandler::instance_ = nullptr;

// ============================================================
// CrashInfo 实现
// ============================================================

QJsonObject CrashInfo::toJson() const
{
    QJsonObject json;
    json["applicationName"] = applicationName;
    json["applicationVersion"] = applicationVersion;
    json["crashTime"] = crashTime.toString(Qt::ISODate);
    json["exceptionType"] = exceptionType;
    json["exceptionMessage"] = exceptionMessage;
    json["exceptionAddress"] = QString::number(exceptionAddress, 16);
    json["moduleName"] = moduleName;
    json["stackTrace"] = QJsonArray::fromStringList(stackTrace);
    json["dumpFilePath"] = dumpFilePath;
    json["logFilePath"] = logFilePath;
    json["osVersion"] = osVersion;
    json["cpuInfo"] = cpuInfo;
    json["totalMemoryMB"] = totalMemoryMB;
    json["availableMemoryMB"] = availableMemoryMB;
    json["processMemoryMB"] = processMemoryMB;
    return json;
}

QString CrashInfo::toText() const
{
    QString text;
    QTextStream stream(&text);

    stream << "==================== 崩溃报告 ====================\n";
    stream << "应用程序: " << applicationName << " v" << applicationVersion << "\n";
    stream << "崩溃时间: " << crashTime.toString("yyyy-MM-dd hh:mm:ss.zzz") << "\n";
    stream << "\n";
    stream << "==================== 异常信息 ====================\n";
    stream << "异常类型: " << exceptionType << "\n";
    stream << "异常消息: " << exceptionMessage << "\n";
    stream << "异常地址: 0x" << QString::number(exceptionAddress, 16).toUpper() << "\n";
    stream << "模块名称: " << moduleName << "\n";
    stream << "\n";
    stream << "==================== 堆栈跟踪 ====================\n";
    for (int i = 0; i < stackTrace.size(); ++i) {
        stream << QString("#%1 ").arg(i, 2, 10, QChar('0')) << stackTrace[i] << "\n";
    }
    stream << "\n";
    stream << "==================== 系统信息 ====================\n";
    stream << "操作系统: " << osVersion << "\n";
    stream << "CPU: " << cpuInfo << "\n";
    stream << "总内存: " << totalMemoryMB << " MB\n";
    stream << "可用内存: " << availableMemoryMB << " MB\n";
    stream << "进程内存: " << processMemoryMB << " MB\n";
    stream << "\n";
    stream << "==================== 转储文件 ====================\n";
    stream << "MiniDump: " << dumpFilePath << "\n";
    stream << "日志文件: " << logFilePath << "\n";
    stream << "==================================================\n";

    return text;
}

// ============================================================
// CrashHandlerConfig 实现
// ============================================================

QJsonObject CrashHandlerConfig::toJson() const
{
    QJsonObject json;
    json["dumpDirectory"] = dumpDirectory;
    json["applicationName"] = applicationName;
    json["applicationVersion"] = applicationVersion;
    json["enableMiniDump"] = enableMiniDump;
    json["enableStackTrace"] = enableStackTrace;
    json["enableRemoteReport"] = enableRemoteReport;
    json["reportServerUrl"] = reportServerUrl;
    json["maxDumpFiles"] = maxDumpFiles;
    json["restartOnCrash"] = restartOnCrash;
    json["restartCommand"] = restartCommand;
    return json;
}

CrashHandlerConfig CrashHandlerConfig::fromJson(const QJsonObject& json)
{
    CrashHandlerConfig config;
    config.dumpDirectory = json.value("dumpDirectory").toString();
    config.applicationName = json.value("applicationName").toString();
    config.applicationVersion = json.value("applicationVersion").toString();
    config.enableMiniDump = json.value("enableMiniDump").toBool(true);
    config.enableStackTrace = json.value("enableStackTrace").toBool(true);
    config.enableRemoteReport = json.value("enableRemoteReport").toBool(false);
    config.reportServerUrl = json.value("reportServerUrl").toString();
    config.maxDumpFiles = json.value("maxDumpFiles").toInt(10);
    config.restartOnCrash = json.value("restartOnCrash").toBool(false);
    config.restartCommand = json.value("restartCommand").toString();
    return config;
}

// ============================================================
// CrashHandler 实现
// ============================================================

CrashHandler::CrashHandler()
{
}

CrashHandler::~CrashHandler()
{
    if (initialized_) {
        shutdown();
    }
}

CrashHandler& CrashHandler::instance()
{
    static CrashHandler instance;
    instance_ = &instance;
    return instance;
}

bool CrashHandler::initialize(const CrashHandlerConfig& config)
{
    if (initialized_) {
        LOG_WARNING("CrashHandler已初始化");
        return true;
    }

    config_ = config;

    // 创建转储目录
    if (!config_.dumpDirectory.isEmpty()) {
        QDir dir(config_.dumpDirectory);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                LOG_ERROR(QString("无法创建转储目录: %1").arg(config_.dumpDirectory));
                return false;
            }
        }
    } else {
        config_.dumpDirectory = QCoreApplication::applicationDirPath() + "/crashes";
        QDir().mkpath(config_.dumpDirectory);
    }

    // 设置异常处理器
    setupExceptionHandlers();

    initialized_ = true;
    LOG_INFO(QString("CrashHandler已初始化，转储目录: %1").arg(config_.dumpDirectory));

    // 清理旧文件
    cleanupOldDumps();

    return true;
}

void CrashHandler::shutdown()
{
    if (!initialized_) {
        return;
    }

    removeExceptionHandlers();
    initialized_ = false;
    LOG_INFO("CrashHandler已关闭");
}

void CrashHandler::triggerTestCrash()
{
    LOG_WARNING("触发测试崩溃...");

    // 触发访问违例
    int* ptr = nullptr;
    *ptr = 42;
}

QString CrashHandler::createManualDump(const QString& reason)
{
#ifdef Q_OS_WIN
    QString dumpPath = generateDumpFileName();
    dumpPath.replace(".dmp", "_manual.dmp");

    HANDLE hFile = CreateFileW(
        reinterpret_cast<LPCWSTR>(dumpPath.utf16()),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        LOG_ERROR("无法创建手动转储文件");
        return QString();
    }

    BOOL success = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpWithFullMemory,
        nullptr,
        nullptr,
        nullptr);

    CloseHandle(hFile);

    if (success) {
        LOG_INFO(QString("手动转储已创建: %1 (原因: %2)").arg(dumpPath, reason));

        // 创建文本报告
        CrashInfo info;
        info.applicationName = config_.applicationName;
        info.applicationVersion = config_.applicationVersion;
        info.crashTime = QDateTime::currentDateTime();
        info.exceptionType = "ManualDump";
        info.exceptionMessage = reason;
        info.dumpFilePath = dumpPath;
        collectSystemInfo(info);
        writeTextReport(info);

        return dumpPath;
    }

    LOG_ERROR("创建手动转储失败");
    return QString();
#else
    Q_UNUSED(reason)
    LOG_WARNING("手动转储仅在Windows上支持");
    return QString();
#endif
}

void CrashHandler::cleanupOldDumps()
{
    QDir dir(config_.dumpDirectory);
    QStringList filters;
    filters << "*.dmp" << "*.txt";

    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Time);

    // 保留最新的maxDumpFiles个文件
    int removeCount = files.size() - config_.maxDumpFiles * 2;  // dmp + txt
    for (int i = 0; i < removeCount && i < files.size(); ++i) {
        QFile::remove(files[files.size() - 1 - i].absoluteFilePath());
    }
}

QStringList CrashHandler::getDumpFiles() const
{
    QDir dir(config_.dumpDirectory);
    QStringList filters;
    filters << "*.dmp";
    return dir.entryList(filters, QDir::Files, QDir::Time);
}

bool CrashHandler::uploadCrashReport(const CrashInfo& info)
{
    if (config_.reportServerUrl.isEmpty()) {
        return false;
    }

    QNetworkAccessManager manager;
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // 添加JSON数据
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       "form-data; name=\"crashInfo\"");
    jsonPart.setBody(QJsonDocument(info.toJson()).toJson());
    multiPart->append(jsonPart);

    // 添加转储文件
    if (!info.dumpFilePath.isEmpty() && QFile::exists(info.dumpFilePath)) {
        QFile* dumpFile = new QFile(info.dumpFilePath);
        if (dumpFile->open(QIODevice::ReadOnly)) {
            QHttpPart filePart;
            filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                              QString("form-data; name=\"dumpFile\"; filename=\"%1\"")
                              .arg(QFileInfo(info.dumpFilePath).fileName()));
            filePart.setBodyDevice(dumpFile);
            dumpFile->setParent(multiPart);
            multiPart->append(filePart);
        }
    }

    QNetworkRequest request(QUrl(config_.reportServerUrl));
    QNetworkReply* reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    if (success) {
        LOG_INFO("崩溃报告已上传");
    } else {
        LOG_ERROR(QString("崩溃报告上传失败: %1").arg(reply->errorString()));
    }

    return success;
}

void CrashHandler::setupExceptionHandlers()
{
#ifdef Q_OS_WIN
    // 设置未处理异常过滤器
    previousFilter_ = SetUnhandledExceptionFilter(unhandledExceptionFilter);

    // 设置无效参数处理器
    previousInvalidHandler_ = _set_invalid_parameter_handler(invalidParameterHandler);

    // 设置纯虚函数调用处理器
    previousPureCallHandler_ = _set_purecall_handler(pureCallHandler);

    // 设置信号处理器
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);
#endif
}

void CrashHandler::removeExceptionHandlers()
{
#ifdef Q_OS_WIN
    if (previousFilter_) {
        SetUnhandledExceptionFilter(previousFilter_);
        previousFilter_ = nullptr;
    }

    if (previousInvalidHandler_) {
        _set_invalid_parameter_handler(previousInvalidHandler_);
        previousInvalidHandler_ = nullptr;
    }

    if (previousPureCallHandler_) {
        _set_purecall_handler(previousPureCallHandler_);
        previousPureCallHandler_ = nullptr;
    }

    signal(SIGABRT, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
#endif
}

QString CrashHandler::generateDumpFileName() const
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    return QString("%1/%2_%3.dmp")
        .arg(config_.dumpDirectory)
        .arg(config_.applicationName.isEmpty() ? "crash" : config_.applicationName)
        .arg(timestamp);
}

void CrashHandler::collectSystemInfo(CrashInfo& info)
{
    info.osVersion = QSysInfo::prettyProductName();
    info.cpuInfo = QSysInfo::currentCpuArchitecture();

#ifdef Q_OS_WIN
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        info.totalMemoryMB = memStatus.ullTotalPhys / (1024 * 1024);
        info.availableMemoryMB = memStatus.ullAvailPhys / (1024 * 1024);
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        info.processMemoryMB = pmc.WorkingSetSize / (1024 * 1024);
    }
#endif
}

void CrashHandler::writeTextReport(const CrashInfo& info)
{
    QString textPath = info.dumpFilePath;
    textPath.replace(".dmp", ".txt");

    QFile file(textPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << info.toText();
        file.close();
    }

    // 更新lastCrashInfo_的日志路径
    const_cast<CrashInfo&>(info).logFilePath = textPath;
}

#ifdef Q_OS_WIN

LONG WINAPI CrashHandler::unhandledExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
    if (!instance_ || !instance_->initialized_) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    CrashInfo info;
    info.applicationName = instance_->config_.applicationName;
    info.applicationVersion = instance_->config_.applicationVersion;
    info.crashTime = QDateTime::currentDateTime();

    // 获取异常信息
    DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
    info.exceptionType = instance_->getExceptionDescription(exceptionCode);
    info.exceptionAddress = reinterpret_cast<quint64>(exceptionInfo->ExceptionRecord->ExceptionAddress);

    // 获取模块名称
    HMODULE hModule = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                       reinterpret_cast<LPCWSTR>(info.exceptionAddress),
                       &hModule);
    if (hModule) {
        wchar_t modulePath[MAX_PATH];
        if (GetModuleFileNameW(hModule, modulePath, MAX_PATH)) {
            info.moduleName = QString::fromWCharArray(modulePath);
        }
    }

    // 捕获堆栈跟踪
    if (instance_->config_.enableStackTrace) {
        instance_->captureStackTrace(info, exceptionInfo->ContextRecord);
    }

    // 收集系统信息
    instance_->collectSystemInfo(info);

    // 生成MiniDump
    if (instance_->config_.enableMiniDump) {
        info.dumpFilePath = instance_->generateDumpFileName();
        instance_->writeMiniDump(exceptionInfo, info.dumpFilePath);
    }

    // 写入文本报告
    instance_->writeTextReport(info);

    // 保存崩溃信息
    instance_->lastCrashInfo_ = info;

    // 调用回调
    if (instance_->crashCallback_) {
        instance_->crashCallback_(info);
    }

    // 远程上报
    if (instance_->config_.enableRemoteReport) {
        instance_->uploadCrashReport(info);
    }

    // 重启应用
    if (instance_->config_.restartOnCrash && !instance_->config_.restartCommand.isEmpty()) {
        // 使用ShellExecute启动新进程
        ShellExecuteW(nullptr, L"open",
                      reinterpret_cast<LPCWSTR>(instance_->config_.restartCommand.utf16()),
                      nullptr, nullptr, SW_SHOWNORMAL);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

void CrashHandler::invalidParameterHandler(const wchar_t* expression,
                                           const wchar_t* function,
                                           const wchar_t* file,
                                           unsigned int line,
                                           uintptr_t reserved)
{
    Q_UNUSED(reserved)

    QString msg = QString("无效参数: %1 in %2 (%3:%4)")
        .arg(expression ? QString::fromWCharArray(expression) : "unknown")
        .arg(function ? QString::fromWCharArray(function) : "unknown")
        .arg(file ? QString::fromWCharArray(file) : "unknown")
        .arg(line);

    LOG_ERROR(msg);

    // 触发异常处理
    RaiseException(STATUS_INVALID_PARAMETER, 0, 0, nullptr);
}

void CrashHandler::pureCallHandler()
{
    LOG_ERROR("纯虚函数调用");
    RaiseException(STATUS_NONCONTINUABLE_EXCEPTION, 0, 0, nullptr);
}

void CrashHandler::signalHandler(int signal)
{
    QString signalName;
    switch (signal) {
    case SIGABRT: signalName = "SIGABRT"; break;
    case SIGFPE: signalName = "SIGFPE"; break;
    case SIGILL: signalName = "SIGILL"; break;
    case SIGSEGV: signalName = "SIGSEGV"; break;
    default: signalName = QString::number(signal);
    }

    LOG_ERROR(QString("收到信号: %1").arg(signalName));

    // 重新抛出以触发unhandledExceptionFilter
    raise(signal);
}

bool CrashHandler::writeMiniDump(EXCEPTION_POINTERS* exceptionInfo, const QString& dumpPath)
{
    HANDLE hFile = CreateFileW(
        reinterpret_cast<LPCWSTR>(dumpPath.utf16()),
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    MINIDUMP_EXCEPTION_INFORMATION mdei;
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = exceptionInfo;
    mdei.ClientPointers = FALSE;

    MINIDUMP_TYPE dumpType = static_cast<MINIDUMP_TYPE>(
        MiniDumpWithDataSegs |
        MiniDumpWithHandleData |
        MiniDumpWithIndirectlyReferencedMemory |
        MiniDumpWithProcessThreadData |
        MiniDumpWithThreadInfo);

    BOOL success = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        dumpType,
        &mdei,
        nullptr,
        nullptr);

    CloseHandle(hFile);
    return success;
}

void CrashHandler::captureStackTrace(CrashInfo& info, CONTEXT* context)
{
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();

    // 初始化符号处理
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    SymInitialize(hProcess, nullptr, TRUE);

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(stackFrame));

#ifdef _M_X64
    DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
    stackFrame.AddrPC.Offset = context->Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#else
    DWORD machineType = IMAGE_FILE_MACHINE_I386;
    stackFrame.AddrPC.Offset = context->Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context->Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context->Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

    // 遍历堆栈
    const int MAX_FRAMES = 64;
    for (int i = 0; i < MAX_FRAMES; ++i) {
        if (!StackWalk64(machineType, hProcess, hThread, &stackFrame,
                        context, nullptr, SymFunctionTableAccess64,
                        SymGetModuleBase64, nullptr)) {
            break;
        }

        if (stackFrame.AddrPC.Offset == 0) {
            break;
        }

        // 获取符号信息
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(symbolBuffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;
        QString frameLine;

        if (SymFromAddr(hProcess, stackFrame.AddrPC.Offset, &displacement, symbol)) {
            frameLine = QString("0x%1 %2 + 0x%3")
                .arg(stackFrame.AddrPC.Offset, 16, 16, QChar('0'))
                .arg(symbol->Name)
                .arg(displacement, 0, 16);

            // 获取行号信息
            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(line);
            DWORD lineDisplacement = 0;
            if (SymGetLineFromAddr64(hProcess, stackFrame.AddrPC.Offset,
                                     &lineDisplacement, &line)) {
                frameLine += QString(" [%1:%2]").arg(line.FileName).arg(line.LineNumber);
            }
        } else {
            frameLine = QString("0x%1 <unknown>")
                .arg(stackFrame.AddrPC.Offset, 16, 16, QChar('0'));
        }

        info.stackTrace.append(frameLine);
    }

    SymCleanup(hProcess);
}

QString CrashHandler::getExceptionDescription(DWORD code)
{
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
    case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
    case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
    case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
    case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
    case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
    case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
    case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
    case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
    case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
    case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
    case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
    case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
    case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
    case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
    case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
    case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
    default: return QString("UNKNOWN_EXCEPTION (0x%1)").arg(code, 8, 16, QChar('0'));
    }
}

#endif // Q_OS_WIN

} // namespace Base
} // namespace VisionForge
