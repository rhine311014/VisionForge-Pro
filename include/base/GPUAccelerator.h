/**
 * @file GPUAccelerator.h
 * @brief GPU加速管理器
 * @details 管理GPU设备，提供GPU加速功能
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include "base/ImageData.h"
#include <QString>
#include <QStringList>
#include <opencv2/opencv.hpp>

#ifdef USE_CUDA
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/core/cuda_stream_accessor.hpp>
#endif

#include <QMutex>
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <future>
#include <atomic>

namespace VisionForge {
namespace Base {

/**
 * @brief GPU加速模式
 */
enum class GPUAccelMode {
    Disabled = 0,   // 禁用GPU加速（仅使用CPU）
    CUDA = 1,       // 启用CUDA GPU加速
    Auto = 2        // 自动选择（有GPU用GPU，否则用CPU）
};

/**
 * @brief GPU设备信息
 */
struct GPUDeviceInfo {
    int deviceId;                   // 设备ID
    QString name;                   // 设备名称
    size_t totalMemory;             // 总内存（字节）
    size_t freeMemory;              // 可用内存（字节）
    int computeCapability;          // 计算能力
    int multiProcessorCount;        // 多处理器数量
    bool isAvailable;               // 是否可用

    GPUDeviceInfo()
        : deviceId(-1)
        , totalMemory(0)
        , freeMemory(0)
        , computeCapability(0)
        , multiProcessorCount(0)
        , isAvailable(false)
    {}
};

/**
 * @brief GPU加速器
 *
 * 单例模式，管理GPU设备和加速功能
 */
class GPUAccelerator {
public:
    /**
     * @brief 获取单例实例
     */
    static GPUAccelerator& instance();

    /**
     * @brief 检查CUDA是否可用
     */
    bool isCudaAvailable() const;

    /**
     * @brief 获取GPU设备数量
     */
    int getDeviceCount() const;

    /**
     * @brief 获取当前设备ID
     */
    int getCurrentDevice() const;

    /**
     * @brief 设置当前设备
     * @param deviceId 设备ID
     * @return true如果成功
     */
    bool setDevice(int deviceId);

    /**
     * @brief 获取设备信息
     * @param deviceId 设备ID（-1表示当前设备）
     */
    GPUDeviceInfo getDeviceInfo(int deviceId = -1) const;

    /**
     * @brief 获取所有设备信息
     */
    QList<GPUDeviceInfo> getAllDevices() const;

    /**
     * @brief 打印设备信息
     */
    void printDeviceInfo() const;

    /**
     * @brief 启用/禁用GPU加速（兼容旧接口）
     */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /**
     * @brief 检查GPU加速是否启用
     */
    bool isEnabled() const { return enabled_ && isCudaAvailable(); }

    /**
     * @brief 设置GPU加速模式
     * @param mode 加速模式
     */
    void setAccelMode(GPUAccelMode mode);

    /**
     * @brief 获取当前加速模式
     * @return 加速模式
     */
    GPUAccelMode getAccelMode() const { return accelMode_; }

    /**
     * @brief 获取实际使用的加速状态
     * @return true如果实际在使用GPU加速
     */
    bool isUsingGPU() const;

    /**
     * @brief 获取加速模式名称
     * @param mode 加速模式
     * @return 模式名称字符串
     */
    static QString getAccelModeName(GPUAccelMode mode);

    /**
     * @brief 保存设置到配置
     */
    void saveSettings();

    /**
     * @brief 从配置加载设置
     */
    void loadSettings();

#ifdef USE_CUDA
    // ========== GPU加速图像处理 ==========

    /**
     * @brief GPU图像上传
     * @param cpuImage CPU图像
     * @param gpuImage GPU图像（输出）
     */
    static void upload(const cv::Mat& cpuImage, cv::cuda::GpuMat& gpuImage);

    /**
     * @brief GPU图像下载
     * @param gpuImage GPU图像
     * @param cpuImage CPU图像（输出）
     */
    static void download(const cv::cuda::GpuMat& gpuImage, cv::Mat& cpuImage);

    /**
     * @brief GPU高斯模糊
     * @param src 源图像
     * @param dst 目标图像
     * @param ksize 核大小
     * @param sigmaX X方向sigma
     * @param sigmaY Y方向sigma
     */
    static void gaussianBlur(const cv::cuda::GpuMat& src,
                            cv::cuda::GpuMat& dst,
                            cv::Size ksize,
                            double sigmaX,
                            double sigmaY = 0);

    /**
     * @brief GPU灰度转换
     * @param src 源图像（BGR）
     * @param dst 目标图像（灰度）
     */
    static void cvtColor(const cv::cuda::GpuMat& src,
                        cv::cuda::GpuMat& dst,
                        int code);

    /**
     * @brief GPU阈值处理
     * @param src 源图像
     * @param dst 目标图像
     * @param thresh 阈值
     * @param maxval 最大值
     * @param type 阈值类型
     */
    static void threshold(const cv::cuda::GpuMat& src,
                         cv::cuda::GpuMat& dst,
                         double thresh,
                         double maxval,
                         int type);

    /**
     * @brief GPU Sobel边缘检测
     * @param src 源图像
     * @param dst 目标图像
     * @param ddepth 输出深度
     * @param dx X方向导数阶数
     * @param dy Y方向导数阶数
     * @param ksize 核大小
     */
    static void sobel(const cv::cuda::GpuMat& src,
                     cv::cuda::GpuMat& dst,
                     int ddepth,
                     int dx,
                     int dy,
                     int ksize = 3);

    /**
     * @brief GPU Canny边缘检测
     * @param src 源图像
     * @param dst 目标图像
     * @param low_thresh 低阈值
     * @param high_thresh 高阈值
     * @param apperture_size 孔径大小
     * @param L2gradient 是否使用L2梯度
     */
    static void canny(const cv::cuda::GpuMat& src,
                     cv::cuda::GpuMat& dst,
                     double low_thresh,
                     double high_thresh,
                     int apperture_size = 3,
                     bool L2gradient = false);

    // ========== 新增GPU操作 ==========

    /**
     * @brief GPU图像缩放
     * @param src 源图像
     * @param dst 目标图像
     * @param dsize 目标尺寸
     * @param interpolation 插值方法
     */
    static void resize(const cv::cuda::GpuMat& src,
                      cv::cuda::GpuMat& dst,
                      cv::Size dsize,
                      int interpolation = cv::INTER_LINEAR);

    /**
     * @brief GPU形态学腐蚀
     * @param src 源图像
     * @param dst 目标图像
     * @param kernel 结构元素
     * @param iterations 迭代次数
     */
    static void erode(const cv::cuda::GpuMat& src,
                     cv::cuda::GpuMat& dst,
                     const cv::Mat& kernel,
                     int iterations = 1);

    /**
     * @brief GPU形态学膨胀
     * @param src 源图像
     * @param dst 目标图像
     * @param kernel 结构元素
     * @param iterations 迭代次数
     */
    static void dilate(const cv::cuda::GpuMat& src,
                      cv::cuda::GpuMat& dst,
                      const cv::Mat& kernel,
                      int iterations = 1);

    /**
     * @brief GPU中值滤波
     * @param src 源图像
     * @param dst 目标图像
     * @param ksize 核大小
     */
    static void medianFilter(const cv::cuda::GpuMat& src,
                            cv::cuda::GpuMat& dst,
                            int ksize);

    /**
     * @brief GPU双边滤波
     * @param src 源图像
     * @param dst 目标图像
     * @param d 邻域直径
     * @param sigmaColor 颜色空间sigma
     * @param sigmaSpace 坐标空间sigma
     */
    static void bilateralFilter(const cv::cuda::GpuMat& src,
                               cv::cuda::GpuMat& dst,
                               int d,
                               float sigmaColor,
                               float sigmaSpace);

    /**
     * @brief GPU图像加法
     * @param src1 源图像1
     * @param src2 源图像2
     * @param dst 目标图像
     */
    static void add(const cv::cuda::GpuMat& src1,
                   const cv::cuda::GpuMat& src2,
                   cv::cuda::GpuMat& dst);

    /**
     * @brief GPU图像减法
     * @param src1 源图像1
     * @param src2 源图像2
     * @param dst 目标图像
     */
    static void subtract(const cv::cuda::GpuMat& src1,
                        const cv::cuda::GpuMat& src2,
                        cv::cuda::GpuMat& dst);

    /**
     * @brief GPU绝对差值
     * @param src1 源图像1
     * @param src2 源图像2
     * @param dst 目标图像
     */
    static void absdiff(const cv::cuda::GpuMat& src1,
                       const cv::cuda::GpuMat& src2,
                       cv::cuda::GpuMat& dst);

    // ========== 异步流处理 ==========

    /**
     * @brief 获取默认CUDA流
     * @return CUDA流引用
     */
    cv::cuda::Stream& getDefaultStream();

    /**
     * @brief 异步上传到GPU
     * @param cpuImage CPU图像
     * @param gpuImage GPU图像（输出）
     * @param stream CUDA流
     */
    static void uploadAsync(const cv::Mat& cpuImage,
                           cv::cuda::GpuMat& gpuImage,
                           cv::cuda::Stream& stream);

    /**
     * @brief 异步下载从GPU
     * @param gpuImage GPU图像
     * @param cpuImage CPU图像（输出）
     * @param stream CUDA流
     */
    static void downloadAsync(const cv::cuda::GpuMat& gpuImage,
                             cv::Mat& cpuImage,
                             cv::cuda::Stream& stream);

    /**
     * @brief 等待流完成
     * @param stream CUDA流
     */
    static void waitStream(cv::cuda::Stream& stream);

#endif // USE_CUDA

private:
    GPUAccelerator();
    ~GPUAccelerator() = default;

    // 禁止拷贝
    GPUAccelerator(const GPUAccelerator&) = delete;
    GPUAccelerator& operator=(const GPUAccelerator&) = delete;

    void detectDevices();

private:
    bool cudaAvailable_;
    bool enabled_;
    int deviceCount_;
    int currentDevice_;
    QList<GPUDeviceInfo> devices_;
    GPUAccelMode accelMode_;   // GPU加速模式

#ifdef USE_CUDA
    cv::cuda::Stream defaultStream_;  // 默认CUDA流
#endif
};

// ============================================================
// GPU内存池
// ============================================================

#ifdef USE_CUDA

/**
 * @brief GPU内存池
 *
 * 管理GPU内存分配，减少显存分配开销
 */
class GpuMemoryPool {
public:
    /**
     * @brief 获取单例实例
     */
    static GpuMemoryPool& instance();

    /**
     * @brief 从池中分配GPU图像
     * @param rows 行数
     * @param cols 列数
     * @param type OpenCV类型
     * @return GPU图像
     */
    cv::cuda::GpuMat allocate(int rows, int cols, int type);

    /**
     * @brief 归还GPU图像到池中
     * @param mat GPU图像
     */
    void release(cv::cuda::GpuMat& mat);

    /**
     * @brief 预分配指定规格的GPU内存
     * @param rows 行数
     * @param cols 列数
     * @param type OpenCV类型
     * @param count 预分配数量
     */
    void preallocate(int rows, int cols, int type, int count);

    /**
     * @brief 清空GPU内存池
     */
    void clear();

    /**
     * @brief 获取池中总内存使用量（字节）
     */
    size_t getTotalMemoryUsage() const;

    /**
     * @brief 获取池中对象数量
     */
    size_t getPoolSize() const;

    /**
     * @brief 统计信息
     */
    struct Statistics {
        size_t totalAllocations;    ///< 总分配次数
        size_t cacheHits;           ///< 缓存命中
        size_t cacheMisses;         ///< 缓存未命中
        double hitRate;             ///< 命中率
        size_t poolSize;            ///< 池大小
        size_t memoryUsage;         ///< 内存使用（字节）
    };

    /**
     * @brief 获取统计信息
     */
    Statistics getStatistics() const;

    /**
     * @brief 重置统计
     */
    void resetStatistics();

    /**
     * @brief 设置最大池大小
     */
    void setMaxPoolSize(size_t maxSize) { maxPoolSize_ = maxSize; }

private:
    GpuMemoryPool();
    ~GpuMemoryPool();

    GpuMemoryPool(const GpuMemoryPool&) = delete;
    GpuMemoryPool& operator=(const GpuMemoryPool&) = delete;

    struct PoolKey {
        int rows;
        int cols;
        int type;

        bool operator<(const PoolKey& other) const {
            if (rows != other.rows) return rows < other.rows;
            if (cols != other.cols) return cols < other.cols;
            return type < other.type;
        }

        size_t getMemorySize() const;
    };

private:
    std::map<PoolKey, std::vector<cv::cuda::GpuMat>> pool_;
    mutable QMutex mutex_;
    size_t maxPoolSize_;

    // 统计
    mutable std::atomic<size_t> totalAllocations_{0};
    mutable std::atomic<size_t> cacheHits_{0};
    mutable std::atomic<size_t> cacheMisses_{0};
};

#endif // USE_CUDA

} // namespace Base
} // namespace VisionForge
