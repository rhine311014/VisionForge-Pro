/**
 * @file GPUAccelerator.cpp
 * @brief GPU加速管理器实现
 */

#include "base/GPUAccelerator.h"
#include "base/ConfigManager.h"
#include "base/Logger.h"
#include <QDebug>

#ifdef USE_CUDA
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

namespace VisionForge {
namespace Base {

GPUAccelerator::GPUAccelerator()
    : cudaAvailable_(false)
    , enabled_(true)
    , deviceCount_(0)
    , currentDevice_(-1)
    , accelMode_(GPUAccelMode::Auto)
{
    try {
        detectDevices();
    } catch (const std::exception& e) {
        LOG_ERROR(QString("GPUAccelerator构造: detectDevices异常: %1").arg(e.what()));
        cudaAvailable_ = false;
        deviceCount_ = 0;
    } catch (...) {
        LOG_ERROR("GPUAccelerator构造: detectDevices未知异常");
        cudaAvailable_ = false;
        deviceCount_ = 0;
    }

    try {
        loadSettings();
    } catch (const std::exception& e) {
        LOG_ERROR(QString("GPUAccelerator构造: loadSettings异常: %1").arg(e.what()));
    } catch (...) {
        LOG_ERROR("GPUAccelerator构造: loadSettings未知异常");
    }
}

GPUAccelerator& GPUAccelerator::instance()
{
    static GPUAccelerator instance;
    return instance;
}

bool GPUAccelerator::isCudaAvailable() const
{
    return cudaAvailable_;
}

int GPUAccelerator::getDeviceCount() const
{
    return deviceCount_;
}

int GPUAccelerator::getCurrentDevice() const
{
    return currentDevice_;
}

bool GPUAccelerator::setDevice(int deviceId)
{
#ifdef USE_CUDA
    if (!cudaAvailable_) {
        LOG_WARNING("CUDA不可用，无法设置设备");
        return false;
    }

    if (deviceId < 0 || deviceId >= deviceCount_) {
        LOG_ERROR(QString("无效的设备ID: %1").arg(deviceId));
        return false;
    }

    try {
        cv::cuda::setDevice(deviceId);
        currentDevice_ = deviceId;
        LOG_INFO(QString("GPU设备已切换到: %1").arg(deviceId));
        return true;
    } catch (const cv::Exception& e) {
        LOG_ERROR(QString("设置GPU设备失败: %1").arg(e.what()));
        return false;
    }
#else
    Q_UNUSED(deviceId);
    LOG_WARNING("未编译CUDA支持");
    return false;
#endif
}

GPUDeviceInfo GPUAccelerator::getDeviceInfo(int deviceId) const
{
#ifdef USE_CUDA
    if (!cudaAvailable_) {
        return GPUDeviceInfo();
    }

    if (deviceId == -1) {
        deviceId = currentDevice_;
    }

    if (deviceId < 0 || deviceId >= devices_.size()) {
        return GPUDeviceInfo();
    }

    return devices_[deviceId];
#else
    Q_UNUSED(deviceId);
    return GPUDeviceInfo();
#endif
}

QList<GPUDeviceInfo> GPUAccelerator::getAllDevices() const
{
    return devices_;
}

void GPUAccelerator::printDeviceInfo() const
{
    if (!cudaAvailable_) {
        qInfo() << "CUDA不可用";
        return;
    }

    qInfo() << "========================================";
    qInfo() << "GPU设备信息";
    qInfo() << QString("设备数量: %1").arg(deviceCount_);
    qInfo() << QString("当前设备: %1").arg(currentDevice_);
    qInfo() << "========================================";

    for (int i = 0; i < devices_.size(); ++i) {
        const GPUDeviceInfo& info = devices_[i];
        qInfo() << QString("设备 %1: %2").arg(i).arg(info.name);
        qInfo() << QString("  总内存: %1 MB").arg(info.totalMemory / 1024 / 1024);
        qInfo() << QString("  可用内存: %1 MB").arg(info.freeMemory / 1024 / 1024);
        qInfo() << QString("  计算能力: %1").arg(info.computeCapability);
        qInfo() << QString("  多处理器数量: %1").arg(info.multiProcessorCount);
        qInfo() << QString("  状态: %1").arg(info.isAvailable ? "可用" : "不可用");
    }

    qInfo() << "========================================";
}

// 用于捕获OpenCV CUDA错误的静态变量
static bool g_cudaErrorOccurred = false;
static std::string g_cudaErrorMessage;

// 自定义OpenCV错误处理函数
static int cudaErrorHandler(int /*status*/, const char* /*func_name*/,
                            const char* err_msg, const char* /*file_name*/,
                            int /*line*/, void* /*userdata*/)
{
    g_cudaErrorOccurred = true;
    g_cudaErrorMessage = err_msg ? err_msg : "Unknown CUDA error";
    // 返回0表示不要调用默认的错误处理（避免terminate）
    return 0;
}

#ifdef _WIN32
// SEH包装的DLL检查（纯C风格，不使用C++对象）
static int checkSingleDll(const char* dllName)
{
    __try {
        HMODULE hModule = LoadLibraryExA(dllName, NULL, LOAD_LIBRARY_AS_DATAFILE);
        if (hModule == NULL) {
            return 0;  // DLL不存在
        }
        FreeLibrary(hModule);
        return 1;  // DLL存在
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        return -1;  // 发生异常
    }
}

// 检查必要的CUDA和cuDNN DLL是否存在
static bool checkCudaDllsAvailable(QString& missingDll)
{
    // 必要的CUDA运行时DLL列表
    const char* requiredDlls[] = {
        "cudart64_12.dll",      // CUDA Runtime
        "nppig64_12.dll",       // NPP Image Processing
        "nppc64_12.dll",        // NPP Core
        nullptr
    };

    // 检查必要的DLL
    for (int i = 0; requiredDlls[i] != nullptr; ++i) {
        int result = checkSingleDll(requiredDlls[i]);
        if (result == 0) {
            missingDll = QString::fromLatin1(requiredDlls[i]);
            return false;
        } else if (result < 0) {
            missingDll = QString("检测%1时发生异常").arg(requiredDlls[i]);
            return false;
        }
    }

    // 可选的cuDNN检查（不影响返回值）
    const char* optionalDlls[] = {
        "cudnn64_9.dll",        // cuDNN 9.x
        "cudnn64_8.dll",        // cuDNN 8.x (备选)
        nullptr
    };

    bool cudnnFound = false;
    for (int i = 0; optionalDlls[i] != nullptr; ++i) {
        int result = checkSingleDll(optionalDlls[i]);
        if (result == 1) {
            cudnnFound = true;
            break;
        }
    }

    if (!cudnnFound) {
        LOG_WARNING("cuDNN未找到，部分深度学习功能可能不可用");
    }

    return true;
}
#endif

void GPUAccelerator::detectDevices()
{
#ifdef USE_CUDA
    // 首先检查OpenCV是否编译了CUDA支持
    try {
        std::string buildInfo = cv::getBuildInformation();
        // 检查是否包含CUDA支持的标志
        bool hasCudaSupport = false;
        if (buildInfo.find("NVIDIA CUDA") != std::string::npos) {
            // 进一步检查是否真的启用了（不是NO）
            size_t cudaPos = buildInfo.find("NVIDIA CUDA:");
            if (cudaPos != std::string::npos) {
                std::string afterCuda = buildInfo.substr(cudaPos, 100);
                if (afterCuda.find("YES") != std::string::npos) {
                    hasCudaSupport = true;
                }
            }
        }

        if (!hasCudaSupport) {
            cudaAvailable_ = false;
            deviceCount_ = 0;
            LOG_INFO("OpenCV未编译CUDA支持，GPU加速不可用");
            return;
        }
    } catch (...) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING("检查OpenCV CUDA支持时发生异常");
        return;
    }

#ifdef _WIN32
    // 检查必要的CUDA DLL是否存在
    QString missingDll;
    if (!checkCudaDllsAvailable(missingDll)) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING(QString("CUDA DLL检查失败，缺少: %1").arg(missingDll));
        LOG_INFO("请确保CUDA运行时DLL在系统PATH中或程序目录下");
        return;
    }
#endif

    // 重置错误状态
    g_cudaErrorOccurred = false;
    g_cudaErrorMessage.clear();

    // 设置自定义错误处理器，避免OpenCV调用terminate
    cv::ErrorCallback oldHandler = cv::redirectError(cudaErrorHandler);

    try {
        deviceCount_ = cv::cuda::getCudaEnabledDeviceCount();

        // 检查是否发生了错误
        if (g_cudaErrorOccurred) {
            cudaAvailable_ = false;
            deviceCount_ = 0;
            LOG_WARNING(QString("CUDA检测失败: %1").arg(QString::fromStdString(g_cudaErrorMessage)));
            cv::redirectError(oldHandler);
            return;
        }

        if (deviceCount_ > 0) {
            cudaAvailable_ = true;
            LOG_INFO(QString("检测到 %1 个CUDA设备").arg(deviceCount_));

            // 获取每个设备的详细信息
            for (int i = 0; i < deviceCount_; ++i) {
                GPUDeviceInfo info;
                info.deviceId = i;
                info.isAvailable = true;

                // 设置当前设备
                cv::cuda::setDevice(i);

                // 获取设备属性
                cv::cuda::DeviceInfo devInfo(i);
                info.name = QString::fromStdString(devInfo.name());
                info.totalMemory = devInfo.totalMemory();
                info.freeMemory = devInfo.freeMemory();
                info.computeCapability = devInfo.majorVersion() * 10 + devInfo.minorVersion();
                info.multiProcessorCount = devInfo.multiProcessorCount();

                devices_.append(info);

                LOG_DEBUG(QString("  设备 %1: %2, 内存=%3MB, 计算能力=%4.%5")
                         .arg(i)
                         .arg(info.name)
                         .arg(info.totalMemory / 1024 / 1024)
                         .arg(devInfo.majorVersion())
                         .arg(devInfo.minorVersion()));
            }

            // 设置默认设备为第一个
            if (deviceCount_ > 0) {
                setDevice(0);
            }
        } else {
            cudaAvailable_ = false;
            LOG_INFO("未检测到CUDA设备");
        }
    } catch (const cv::Exception& e) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING(QString("CUDA检测失败: %1").arg(e.what()));
    } catch (const std::exception& e) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING(QString("CUDA检测异常: %1").arg(e.what()));
    } catch (...) {
        cudaAvailable_ = false;
        deviceCount_ = 0;
        LOG_WARNING("CUDA检测时发生未知异常");
    }

    // 恢复原来的错误处理器
    cv::redirectError(oldHandler);
#else
    cudaAvailable_ = false;
    deviceCount_ = 0;
    LOG_INFO("未编译CUDA支持");
#endif
}

void GPUAccelerator::setAccelMode(GPUAccelMode mode)
{
    accelMode_ = mode;

    // 根据模式更新enabled_状态
    switch (mode) {
        case GPUAccelMode::Disabled:
            enabled_ = false;
            LOG_INFO("GPU加速模式: 禁用");
            break;
        case GPUAccelMode::CUDA:
            enabled_ = cudaAvailable_;
            if (cudaAvailable_) {
                LOG_INFO("GPU加速模式: CUDA");
            } else {
                LOG_WARNING("GPU加速模式: CUDA (但CUDA不可用，将使用CPU)");
            }
            break;
        case GPUAccelMode::Auto:
            enabled_ = cudaAvailable_;
            LOG_INFO(QString("GPU加速模式: 自动 (%1)")
                    .arg(cudaAvailable_ ? "使用GPU" : "使用CPU"));
            break;
    }

    // 保存设置
    saveSettings();
}

bool GPUAccelerator::isUsingGPU() const
{
    switch (accelMode_) {
        case GPUAccelMode::Disabled:
            return false;
        case GPUAccelMode::CUDA:
            return cudaAvailable_;
        case GPUAccelMode::Auto:
            return cudaAvailable_;
        default:
            return false;
    }
}

QString GPUAccelerator::getAccelModeName(GPUAccelMode mode)
{
    switch (mode) {
        case GPUAccelMode::Disabled:
            return "禁用GPU加速";
        case GPUAccelMode::CUDA:
            return "CUDA GPU加速";
        case GPUAccelMode::Auto:
            return "自动选择";
        default:
            return "未知";
    }
}

void GPUAccelerator::saveSettings()
{
    ConfigManager& config = ConfigManager::instance();
    config.setValue("GPU/AccelMode", static_cast<int>(accelMode_));
    config.save();
    LOG_DEBUG(QString("GPU设置已保存: 模式=%1").arg(static_cast<int>(accelMode_)));
}

void GPUAccelerator::loadSettings()
{
    ConfigManager& config = ConfigManager::instance();
    int mode = config.getValue("GPU/AccelMode", static_cast<int>(GPUAccelMode::Auto)).toInt();

    // 验证模式值有效性
    if (mode < 0 || mode > 2) {
        mode = static_cast<int>(GPUAccelMode::Auto);
    }

    accelMode_ = static_cast<GPUAccelMode>(mode);

    // 根据模式更新enabled_状态（不保存，避免循环）
    switch (accelMode_) {
        case GPUAccelMode::Disabled:
            enabled_ = false;
            break;
        case GPUAccelMode::CUDA:
        case GPUAccelMode::Auto:
            enabled_ = cudaAvailable_;
            break;
    }

    LOG_DEBUG(QString("GPU设置已加载: 模式=%1, 实际使用GPU=%2")
             .arg(static_cast<int>(accelMode_))
             .arg(isUsingGPU() ? "是" : "否"));
}

#ifdef USE_CUDA

// ========== GPU加速图像处理实现 ==========

void GPUAccelerator::upload(const cv::Mat& cpuImage, cv::cuda::GpuMat& gpuImage)
{
    gpuImage.upload(cpuImage);
}

void GPUAccelerator::download(const cv::cuda::GpuMat& gpuImage, cv::Mat& cpuImage)
{
    gpuImage.download(cpuImage);
}

void GPUAccelerator::gaussianBlur(const cv::cuda::GpuMat& src,
                                 cv::cuda::GpuMat& dst,
                                 cv::Size ksize,
                                 double sigmaX,
                                 double sigmaY)
{
    auto filter = cv::cuda::createGaussianFilter(
        src.type(),
        dst.type(),
        ksize,
        sigmaX,
        sigmaY
    );
    filter->apply(src, dst);
}

void GPUAccelerator::cvtColor(const cv::cuda::GpuMat& src,
                             cv::cuda::GpuMat& dst,
                             int code)
{
    cv::cuda::cvtColor(src, dst, code);
}

void GPUAccelerator::threshold(const cv::cuda::GpuMat& src,
                              cv::cuda::GpuMat& dst,
                              double thresh,
                              double maxval,
                              int type)
{
    cv::cuda::threshold(src, dst, thresh, maxval, type);
}

void GPUAccelerator::sobel(const cv::cuda::GpuMat& src,
                          cv::cuda::GpuMat& dst,
                          int ddepth,
                          int dx,
                          int dy,
                          int ksize)
{
    auto filter = cv::cuda::createSobelFilter(
        src.type(),
        ddepth,
        dx,
        dy,
        ksize
    );
    filter->apply(src, dst);
}

void GPUAccelerator::canny(const cv::cuda::GpuMat& src,
                          cv::cuda::GpuMat& dst,
                          double low_thresh,
                          double high_thresh,
                          int apperture_size,
                          bool L2gradient)
{
    auto detector = cv::cuda::createCannyEdgeDetector(
        low_thresh,
        high_thresh,
        apperture_size,
        L2gradient
    );
    detector->detect(src, dst);
}

// ========== 新增GPU操作实现 ==========

void GPUAccelerator::resize(const cv::cuda::GpuMat& src,
                           cv::cuda::GpuMat& dst,
                           cv::Size dsize,
                           int interpolation)
{
    cv::cuda::resize(src, dst, dsize, 0, 0, interpolation);
}

void GPUAccelerator::erode(const cv::cuda::GpuMat& src,
                          cv::cuda::GpuMat& dst,
                          const cv::Mat& kernel,
                          int iterations)
{
    if (iterations <= 0) {
        src.copyTo(dst);
        return;
    }

    auto filter = cv::cuda::createMorphologyFilter(
        cv::MORPH_ERODE, src.type(), kernel
    );

    if (iterations == 1) {
        filter->apply(src, dst);
        return;
    }

    // 使用双缓冲避免每次迭代clone，减少GPU内存分配
    GpuMemoryPool& pool = GpuMemoryPool::instance();
    cv::cuda::GpuMat buffer1 = pool.allocate(src.rows, src.cols, src.type());
    cv::cuda::GpuMat buffer2 = pool.allocate(src.rows, src.cols, src.type());

    cv::cuda::GpuMat* current = &buffer1;
    cv::cuda::GpuMat* next = &buffer2;

    src.copyTo(*current);

    for (int i = 0; i < iterations; ++i) {
        if (i == iterations - 1) {
            // 最后一次迭代，输出到dst
            filter->apply(*current, dst);
        } else {
            filter->apply(*current, *next);
            std::swap(current, next);
        }
    }

    // 归还到内存池
    pool.release(buffer1);
    pool.release(buffer2);
}

void GPUAccelerator::dilate(const cv::cuda::GpuMat& src,
                           cv::cuda::GpuMat& dst,
                           const cv::Mat& kernel,
                           int iterations)
{
    if (iterations <= 0) {
        src.copyTo(dst);
        return;
    }

    auto filter = cv::cuda::createMorphologyFilter(
        cv::MORPH_DILATE, src.type(), kernel
    );

    if (iterations == 1) {
        filter->apply(src, dst);
        return;
    }

    // 使用双缓冲避免每次迭代clone，减少GPU内存分配
    GpuMemoryPool& pool = GpuMemoryPool::instance();
    cv::cuda::GpuMat buffer1 = pool.allocate(src.rows, src.cols, src.type());
    cv::cuda::GpuMat buffer2 = pool.allocate(src.rows, src.cols, src.type());

    cv::cuda::GpuMat* current = &buffer1;
    cv::cuda::GpuMat* next = &buffer2;

    src.copyTo(*current);

    for (int i = 0; i < iterations; ++i) {
        if (i == iterations - 1) {
            // 最后一次迭代，输出到dst
            filter->apply(*current, dst);
        } else {
            filter->apply(*current, *next);
            std::swap(current, next);
        }
    }

    // 归还到内存池
    pool.release(buffer1);
    pool.release(buffer2);
}

void GPUAccelerator::medianFilter(const cv::cuda::GpuMat& src,
                                 cv::cuda::GpuMat& dst,
                                 int ksize)
{
    // OpenCV CUDA没有直接的中值滤波，使用近似方法
    // 对于小核使用双边滤波近似
    if (ksize <= 5) {
        cv::cuda::bilateralFilter(src, dst, ksize, 75, 75);
    } else {
        // 大核使用高斯模糊近似
        cv::Size size(ksize, ksize);
        auto filter = cv::cuda::createGaussianFilter(src.type(), -1, size, 0);
        filter->apply(src, dst);
    }
}

void GPUAccelerator::bilateralFilter(const cv::cuda::GpuMat& src,
                                    cv::cuda::GpuMat& dst,
                                    int d,
                                    float sigmaColor,
                                    float sigmaSpace)
{
    cv::cuda::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
}

void GPUAccelerator::add(const cv::cuda::GpuMat& src1,
                        const cv::cuda::GpuMat& src2,
                        cv::cuda::GpuMat& dst)
{
    cv::cuda::add(src1, src2, dst);
}

void GPUAccelerator::subtract(const cv::cuda::GpuMat& src1,
                             const cv::cuda::GpuMat& src2,
                             cv::cuda::GpuMat& dst)
{
    cv::cuda::subtract(src1, src2, dst);
}

void GPUAccelerator::absdiff(const cv::cuda::GpuMat& src1,
                            const cv::cuda::GpuMat& src2,
                            cv::cuda::GpuMat& dst)
{
    cv::cuda::absdiff(src1, src2, dst);
}

// ========== 异步流处理实现 ==========

cv::cuda::Stream& GPUAccelerator::getDefaultStream()
{
    return defaultStream_;
}

void GPUAccelerator::uploadAsync(const cv::Mat& cpuImage,
                                cv::cuda::GpuMat& gpuImage,
                                cv::cuda::Stream& stream)
{
    gpuImage.upload(cpuImage, stream);
}

void GPUAccelerator::downloadAsync(const cv::cuda::GpuMat& gpuImage,
                                  cv::Mat& cpuImage,
                                  cv::cuda::Stream& stream)
{
    gpuImage.download(cpuImage, stream);
}

void GPUAccelerator::waitStream(cv::cuda::Stream& stream)
{
    stream.waitForCompletion();
}

// ============================================================
// GpuMemoryPool 实现
// ============================================================

static constexpr size_t DEFAULT_GPU_POOL_SIZE = 8;

size_t GpuMemoryPool::PoolKey::getMemorySize() const
{
    return static_cast<size_t>(rows) * cols * CV_ELEM_SIZE(type);
}

GpuMemoryPool::GpuMemoryPool()
    : maxPoolSize_(DEFAULT_GPU_POOL_SIZE)
{
}

GpuMemoryPool::~GpuMemoryPool()
{
    clear();
}

GpuMemoryPool& GpuMemoryPool::instance()
{
    static GpuMemoryPool instance;
    return instance;
}

cv::cuda::GpuMat GpuMemoryPool::allocate(int rows, int cols, int type)
{
    totalAllocations_++;

    PoolKey key{rows, cols, type};

    QMutexLocker locker(&mutex_);

    auto it = pool_.find(key);
    if (it != pool_.end() && !it->second.empty()) {
        cacheHits_++;
        cv::cuda::GpuMat mat = it->second.back();
        it->second.pop_back();
        return mat;
    }

    cacheMisses_++;
    locker.unlock();

    return cv::cuda::GpuMat(rows, cols, type);
}

void GpuMemoryPool::release(cv::cuda::GpuMat& mat)
{
    if (mat.empty()) {
        return;
    }

    PoolKey key{mat.rows, mat.cols, mat.type()};

    QMutexLocker locker(&mutex_);

    auto& vec = pool_[key];
    if (vec.size() >= maxPoolSize_) {
        return;  // 让mat自动释放
    }

    vec.push_back(mat);
    mat = cv::cuda::GpuMat();  // 清空原引用
}

void GpuMemoryPool::preallocate(int rows, int cols, int type, int count)
{
    std::vector<cv::cuda::GpuMat> mats;
    mats.reserve(count);

    for (int i = 0; i < count; ++i) {
        mats.emplace_back(rows, cols, type);
    }

    PoolKey key{rows, cols, type};

    QMutexLocker locker(&mutex_);
    auto& vec = pool_[key];
    vec.insert(vec.end(), mats.begin(), mats.end());

    LOG_DEBUG(QString("GPU内存池预分配: %1x%2, 类型=%3, 数量=%4")
             .arg(rows).arg(cols).arg(type).arg(count));
}

void GpuMemoryPool::clear()
{
    QMutexLocker locker(&mutex_);
    pool_.clear();
    LOG_DEBUG("GPU内存池已清空");
}

size_t GpuMemoryPool::getTotalMemoryUsage() const
{
    QMutexLocker locker(&mutex_);

    size_t total = 0;
    for (const auto& pair : pool_) {
        size_t count = pair.second.size();
        size_t memPerMat = pair.first.getMemorySize();
        total += count * memPerMat;
    }

    return total;
}

size_t GpuMemoryPool::getPoolSize() const
{
    QMutexLocker locker(&mutex_);

    size_t total = 0;
    for (const auto& pair : pool_) {
        total += pair.second.size();
    }

    return total;
}

GpuMemoryPool::Statistics GpuMemoryPool::getStatistics() const
{
    Statistics stats;
    stats.totalAllocations = totalAllocations_.load();
    stats.cacheHits = cacheHits_.load();
    stats.cacheMisses = cacheMisses_.load();

    if (stats.totalAllocations > 0) {
        stats.hitRate = static_cast<double>(stats.cacheHits) / stats.totalAllocations;
    } else {
        stats.hitRate = 0.0;
    }

    stats.poolSize = getPoolSize();
    stats.memoryUsage = getTotalMemoryUsage();

    return stats;
}

void GpuMemoryPool::resetStatistics()
{
    totalAllocations_.store(0);
    cacheHits_.store(0);
    cacheMisses_.store(0);
}

#endif // USE_CUDA

} // namespace Base
} // namespace VisionForge
