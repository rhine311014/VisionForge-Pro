/**
 * @file AlgorithmBackendFactory.h
 * @brief 算法后端工厂类
 * @details 统一管理算法后端的创建，将 #ifdef USE_HALCON 集中在此处
 * @author VisionForge Team
 * @date 2025-12-25
 */

#pragma once

#include "algorithm/backend/ICircleDetector.h"
#include "algorithm/backend/ILineDetector.h"
#include "algorithm/backend/IBlobAnalyzer.h"
#include <QString>
#include <memory>

namespace VisionForge {
namespace Algorithm {
namespace Backend {

/**
 * @brief 后端类型枚举
 */
enum class BackendType {
    Auto,       // 自动选择（优先Halcon）
    OpenCV,     // 强制使用OpenCV
    Halcon      // 强制使用Halcon（需要编译时启用）
};

/**
 * @brief 算法后端工厂类
 *
 * 这是整个项目中唯一使用 #ifdef USE_HALCON 的地方（除了Halcon后端实现本身）。
 * 业务代码只需通过工厂获取接口指针，不需要关心具体使用哪个后端。
 */
class AlgorithmBackendFactory {
public:
    /**
     * @brief 获取单例实例
     */
    static AlgorithmBackendFactory& instance();

    /**
     * @brief 创建圆检测器
     * @param backend 后端类型
     * @return 圆检测器智能指针
     */
    ICircleDetectorPtr createCircleDetector(BackendType backend = BackendType::Auto);

    /**
     * @brief 创建线检测器
     * @param backend 后端类型
     * @return 线检测器智能指针
     */
    ILineDetectorPtr createLineDetector(BackendType backend = BackendType::Auto);

    /**
     * @brief 创建Blob分析器
     * @param backend 后端类型
     * @return Blob分析器智能指针
     */
    IBlobAnalyzerPtr createBlobAnalyzer(BackendType backend = BackendType::Auto);

    /**
     * @brief 检查是否支持Halcon后端
     * @return true如果编译时启用了Halcon
     */
    bool isHalconAvailable() const;

    /**
     * @brief 获取默认后端类型
     * @return 默认后端类型
     */
    BackendType defaultBackend() const;

    /**
     * @brief 设置默认后端类型
     * @param backend 后端类型
     */
    void setDefaultBackend(BackendType backend);

    /**
     * @brief 获取可用后端列表
     * @return 后端名称列表
     */
    QStringList availableBackends() const;

    /**
     * @brief 后端类型转字符串
     * @param backend 后端类型
     * @return 后端名称字符串
     */
    static QString backendToString(BackendType backend);

    /**
     * @brief 字符串转后端类型
     * @param str 后端名称字符串
     * @return 后端类型
     */
    static BackendType stringToBackend(const QString& str);

private:
    AlgorithmBackendFactory();
    ~AlgorithmBackendFactory() = default;

    // 禁止拷贝
    AlgorithmBackendFactory(const AlgorithmBackendFactory&) = delete;
    AlgorithmBackendFactory& operator=(const AlgorithmBackendFactory&) = delete;

private:
    BackendType defaultBackend_;
};

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
