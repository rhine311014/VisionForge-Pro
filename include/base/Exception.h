/**
 * @file Exception.h
 * @brief VisionForge自定义异常层次结构
 * @author VisionForge Team
 * @date 2025-12-19
 *
 * @details
 * 本文件定义了VisionForge项目的异常类层次结构，提供统一的错误处理机制。
 *
 * ## 设计模式
 * - **模板方法模式**：基类定义异常结构，子类覆盖type()方法
 * - **工厂模式**：通过VF_THROW宏简化异常创建
 *
 * ## 异常层次结构
 * @code
 * std::exception
 *   └── VisionForgeException (基类)
 *         ├── AlgorithmException (算法相关: 1000-1999)
 *         ├── CameraException (相机相关: 2000-2999)
 *         ├── CommunicationException (通讯相关: 3000-3999)
 *         ├── ConfigurationException (配置相关: 4000-4999)
 *         ├── FileIOException (文件IO相关: 5000-5999)
 *         └── ValidationException (参数验证相关: 6000-6999)
 * @endcode
 *
 * ## 错误码分配
 * 每类异常使用独立的错误码范围，便于日志分析和问题定位：
 * - 1xxx: 算法错误
 * - 2xxx: 相机错误
 * - 3xxx: 通讯错误
 * - 4xxx: 配置错误
 * - 5xxx: 文件IO错误
 * - 6xxx: 验证错误
 *
 * ## 线程安全
 * - 异常对象创建后不可变，线程安全
 * - what()返回的C字符串生命周期与异常对象相同
 * - 可跨线程抛出和捕获
 *
 * ## 内存管理
 * - 使用std::string管理字符串，自动释放
 * - 异常对象遵循RAII原则
 * - 析构函数标记noexcept，符合异常安全准则
 *
 * ## 使用示例
 * @code
 * // 使用宏抛出异常（推荐）
 * VF_THROW_CODE(AlgorithmException, "模板匹配失败", AlgorithmException::MatchFailed);
 *
 * // 使用检查宏
 * VF_CHECK_NULL(imagePtr, "图像指针不能为空");
 * VF_CHECK_RANGE(threshold, 0, 255, "阈值必须在0-255范围内");
 *
 * // 捕获异常
 * try {
 *     algorithm->execute();
 * } catch (const AlgorithmException& e) {
 *     LOG_ERROR(QString("[%1] %2").arg(e.code()).arg(e.what()));
 * } catch (const VisionForgeException& e) {
 *     LOG_ERROR(QString("系统错误: %1").arg(e.what()));
 * }
 * @endcode
 *
 * ## 最佳实践
 * - 优先使用VF_THROW宏，自动填充文件和行号
 * - 捕获时先捕获具体类型，最后捕获基类
 * - 错误消息使用中文，便于日志分析
 */

#pragma once

#include <exception>
#include <string>
#include <QString>

namespace VisionForge {
namespace Base {

/**
 * @class VisionForgeException
 * @brief VisionForge异常基类
 *
 * 所有VisionForge项目特定异常的基类。
 * 提供错误消息、错误码和源位置信息。
 */
class VisionForgeException : public std::exception
{
public:
    /**
     * @brief 构造函数
     * @param message 错误消息
     * @param code 错误码 (默认-1)
     * @param file 源文件名 (可选)
     * @param line 源代码行号 (可选)
     */
    explicit VisionForgeException(const std::string& message,
                                   int code = -1,
                                   const char* file = nullptr,
                                   int line = 0)
        : message_(message)
        , code_(code)
        , file_(file ? file : "")
        , line_(line)
    {
        buildFullMessage();
    }

    explicit VisionForgeException(const QString& message,
                                   int code = -1,
                                   const char* file = nullptr,
                                   int line = 0)
        : message_(message.toStdString())
        , code_(code)
        , file_(file ? file : "")
        , line_(line)
    {
        buildFullMessage();
    }

    virtual ~VisionForgeException() noexcept = default;

    /**
     * @brief 获取异常消息
     * @return 异常消息的C字符串
     */
    const char* what() const noexcept override
    {
        return fullMessage_.c_str();
    }

    /**
     * @brief 获取错误消息
     * @return 错误消息字符串
     */
    const std::string& message() const noexcept { return message_; }

    /**
     * @brief 获取错误消息 (QString版本)
     * @return 错误消息QString
     */
    QString messageQ() const { return QString::fromStdString(message_); }

    /**
     * @brief 获取错误码
     * @return 错误码
     */
    int code() const noexcept { return code_; }

    /**
     * @brief 获取异常类型名称
     * @return 异常类型名称
     */
    virtual const char* type() const noexcept { return "VisionForgeException"; }

    /**
     * @brief 获取源文件名
     * @return 源文件名
     */
    const std::string& file() const noexcept { return file_; }

    /**
     * @brief 获取源代码行号
     * @return 行号
     */
    int line() const noexcept { return line_; }

protected:
    void buildFullMessage()
    {
        fullMessage_ = std::string("[") + type() + "] " + message_;
        if (!file_.empty()) {
            fullMessage_ += " (at " + file_ + ":" + std::to_string(line_) + ")";
        }
        if (code_ != -1) {
            fullMessage_ += " [code: " + std::to_string(code_) + "]";
        }
    }

    std::string message_;
    std::string fullMessage_;
    int code_;
    std::string file_;
    int line_;
};

/**
 * @class AlgorithmException
 * @brief 算法异常类
 *
 * 用于算法执行过程中的错误，如:
 * - 图像处理失败
 * - 匹配失败
 * - 检测失败
 * - 计算错误
 */
class AlgorithmException : public VisionForgeException
{
public:
    /// 算法错误码
    enum ErrorCode {
        ProcessingFailed = 1001,    ///< 处理失败
        MatchFailed = 1002,         ///< 匹配失败
        DetectionFailed = 1003,     ///< 检测失败
        CalibrationFailed = 1004,   ///< 标定失败
        InvalidParameter = 1005,    ///< 无效参数
        ModelNotLoaded = 1006,      ///< 模型未加载
        ImageEmpty = 1007,          ///< 图像为空
        ROIInvalid = 1008,          ///< ROI无效
    };

    explicit AlgorithmException(const std::string& message,
                                 ErrorCode code = ProcessingFailed,
                                 const char* file = nullptr,
                                 int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit AlgorithmException(const QString& message,
                                 ErrorCode code = ProcessingFailed,
                                 const char* file = nullptr,
                                 int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "AlgorithmException"; }
};

/**
 * @class CameraException
 * @brief 相机异常类
 *
 * 用于相机操作中的错误，如:
 * - 连接失败
 * - 采集失败
 * - 参数设置失败
 */
class CameraException : public VisionForgeException
{
public:
    /// 相机错误码
    enum ErrorCode {
        ConnectionFailed = 2001,    ///< 连接失败
        NotConnected = 2002,        ///< 未连接
        GrabFailed = 2003,          ///< 采集失败
        SetParameterFailed = 2004,  ///< 参数设置失败
        DeviceNotFound = 2005,      ///< 设备未找到
        InitFailed = 2006,          ///< 初始化失败
        Timeout = 2007,             ///< 超时
    };

    explicit CameraException(const std::string& message,
                              ErrorCode code = ConnectionFailed,
                              const char* file = nullptr,
                              int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit CameraException(const QString& message,
                              ErrorCode code = ConnectionFailed,
                              const char* file = nullptr,
                              int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "CameraException"; }
};

/**
 * @class CommunicationException
 * @brief 通讯异常类
 *
 * 用于PLC/设备通讯中的错误，如:
 * - 连接失败
 * - 发送/接收失败
 * - 超时
 */
class CommunicationException : public VisionForgeException
{
public:
    /// 通讯错误码
    enum ErrorCode {
        ConnectionFailed = 3001,    ///< 连接失败
        NotConnected = 3002,        ///< 未连接
        SendFailed = 3003,          ///< 发送失败
        ReceiveFailed = 3004,       ///< 接收失败
        Timeout = 3005,             ///< 超时
        InvalidAddress = 3006,      ///< 无效地址
        ProtocolError = 3007,       ///< 协议错误
    };

    explicit CommunicationException(const std::string& message,
                                     ErrorCode code = ConnectionFailed,
                                     const char* file = nullptr,
                                     int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit CommunicationException(const QString& message,
                                     ErrorCode code = ConnectionFailed,
                                     const char* file = nullptr,
                                     int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "CommunicationException"; }
};

/**
 * @class ConfigurationException
 * @brief 配置异常类
 *
 * 用于配置/项目管理中的错误，如:
 * - 配置文件无效
 * - 项目加载失败
 * - 配方错误
 */
class ConfigurationException : public VisionForgeException
{
public:
    /// 配置错误码
    enum ErrorCode {
        InvalidConfig = 4001,       ///< 无效配置
        LoadFailed = 4002,          ///< 加载失败
        SaveFailed = 4003,          ///< 保存失败
        MissingKey = 4004,          ///< 缺少必要键值
        ParseError = 4005,          ///< 解析错误
        VersionMismatch = 4006,     ///< 版本不匹配
    };

    explicit ConfigurationException(const std::string& message,
                                     ErrorCode code = InvalidConfig,
                                     const char* file = nullptr,
                                     int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit ConfigurationException(const QString& message,
                                     ErrorCode code = InvalidConfig,
                                     const char* file = nullptr,
                                     int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "ConfigurationException"; }
};

/**
 * @class FileIOException
 * @brief 文件IO异常类
 *
 * 用于文件操作中的错误，如:
 * - 文件不存在
 * - 读写失败
 * - 权限不足
 */
class FileIOException : public VisionForgeException
{
public:
    /// 文件IO错误码
    enum ErrorCode {
        FileNotFound = 5001,        ///< 文件未找到
        ReadFailed = 5002,          ///< 读取失败
        WriteFailed = 5003,         ///< 写入失败
        PermissionDenied = 5004,    ///< 权限不足
        InvalidFormat = 5005,       ///< 格式无效
        DiskFull = 5006,            ///< 磁盘已满
    };

    explicit FileIOException(const std::string& message,
                              ErrorCode code = FileNotFound,
                              const char* file = nullptr,
                              int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit FileIOException(const QString& message,
                              ErrorCode code = FileNotFound,
                              const char* file = nullptr,
                              int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "FileIOException"; }
};

/**
 * @class ValidationException
 * @brief 参数验证异常类
 *
 * 用于参数验证失败的情况，如:
 * - 参数超出范围
 * - 无效参数类型
 * - 空指针
 */
class ValidationException : public VisionForgeException
{
public:
    /// 验证错误码
    enum ErrorCode {
        NullPointer = 6001,         ///< 空指针
        OutOfRange = 6002,          ///< 超出范围
        InvalidType = 6003,         ///< 无效类型
        InvalidValue = 6004,        ///< 无效值
        RequiredMissing = 6005,     ///< 缺少必要参数
    };

    explicit ValidationException(const std::string& message,
                                  ErrorCode code = InvalidValue,
                                  const char* file = nullptr,
                                  int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    explicit ValidationException(const QString& message,
                                  ErrorCode code = InvalidValue,
                                  const char* file = nullptr,
                                  int line = 0)
        : VisionForgeException(message, static_cast<int>(code), file, line) {}

    const char* type() const noexcept override { return "ValidationException"; }
};

} // namespace Base
} // namespace VisionForge

/// @brief 方便的异常抛出宏，自动包含源文件位置
#define VF_THROW(ExceptionType, message) \
    throw ExceptionType(message, ExceptionType::ErrorCode(-1), __FILE__, __LINE__)

#define VF_THROW_CODE(ExceptionType, message, code) \
    throw ExceptionType(message, code, __FILE__, __LINE__)

/// @brief 条件检查宏，失败时抛出异常
#define VF_CHECK(condition, ExceptionType, message) \
    do { \
        if (!(condition)) { \
            VF_THROW(ExceptionType, message); \
        } \
    } while(0)

#define VF_CHECK_NULL(ptr, message) \
    VF_CHECK((ptr) != nullptr, VisionForge::Base::ValidationException, message)

#define VF_CHECK_RANGE(value, min, max, message) \
    VF_CHECK((value) >= (min) && (value) <= (max), VisionForge::Base::ValidationException, message)
