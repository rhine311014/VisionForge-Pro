/**
 * @file AlgorithmBackendFactory.cpp
 * @brief 算法后端工厂类实现
 */

#include "algorithm/backend/AlgorithmBackendFactory.h"
#include "algorithm/backend/OpenCVCircleDetector.h"
#include "algorithm/backend/OpenCVLineDetector.h"
#include "algorithm/backend/OpenCVBlobAnalyzer.h"
#include "base/Logger.h"

// Halcon后端仅在启用时包含
#ifdef USE_HALCON
#include "algorithm/backend/HalconCircleDetector.h"
#include "algorithm/backend/HalconLineDetector.h"
#include "algorithm/backend/HalconBlobAnalyzer.h"
#endif

namespace VisionForge {
namespace Algorithm {
namespace Backend {

AlgorithmBackendFactory& AlgorithmBackendFactory::instance()
{
    static AlgorithmBackendFactory instance;
    return instance;
}

AlgorithmBackendFactory::AlgorithmBackendFactory()
    : defaultBackend_(BackendType::Auto)
{
}

ICircleDetectorPtr AlgorithmBackendFactory::createCircleDetector(BackendType backend)
{
    // 处理Auto模式
    if (backend == BackendType::Auto) {
#ifdef USE_HALCON
        // 如果编译时启用了Halcon，优先使用Halcon
        LOG_DEBUG("自动选择后端: Halcon");
        return std::make_unique<HalconCircleDetector>();
#else
        // 否则使用OpenCV
        LOG_DEBUG("自动选择后端: OpenCV");
        return std::make_unique<OpenCVCircleDetector>();
#endif
    }

    // 强制指定后端
    if (backend == BackendType::Halcon) {
#ifdef USE_HALCON
        LOG_DEBUG("使用Halcon后端");
        return std::make_unique<HalconCircleDetector>();
#else
        LOG_WARNING("请求Halcon后端，但未编译Halcon支持，回退到OpenCV");
        return std::make_unique<OpenCVCircleDetector>();
#endif
    }

    // OpenCV后端
    LOG_DEBUG("使用OpenCV后端");
    return std::make_unique<OpenCVCircleDetector>();
}

ILineDetectorPtr AlgorithmBackendFactory::createLineDetector(BackendType backend)
{
    if (backend == BackendType::Auto) {
#ifdef USE_HALCON
        return std::make_unique<HalconLineDetector>();
#else
        return std::make_unique<OpenCVLineDetector>();
#endif
    }

    if (backend == BackendType::Halcon) {
#ifdef USE_HALCON
        return std::make_unique<HalconLineDetector>();
#else
        LOG_WARNING("请求Halcon后端，但未编译Halcon支持，回退到OpenCV");
        return std::make_unique<OpenCVLineDetector>();
#endif
    }

    return std::make_unique<OpenCVLineDetector>();
}

IBlobAnalyzerPtr AlgorithmBackendFactory::createBlobAnalyzer(BackendType backend)
{
    if (backend == BackendType::Auto) {
#ifdef USE_HALCON
        return std::make_unique<HalconBlobAnalyzer>();
#else
        return std::make_unique<OpenCVBlobAnalyzer>();
#endif
    }

    if (backend == BackendType::Halcon) {
#ifdef USE_HALCON
        return std::make_unique<HalconBlobAnalyzer>();
#else
        LOG_WARNING("请求Halcon后端，但未编译Halcon支持，回退到OpenCV");
        return std::make_unique<OpenCVBlobAnalyzer>();
#endif
    }

    return std::make_unique<OpenCVBlobAnalyzer>();
}

bool AlgorithmBackendFactory::isHalconAvailable() const
{
#ifdef USE_HALCON
    return true;
#else
    return false;
#endif
}

BackendType AlgorithmBackendFactory::defaultBackend() const
{
    return defaultBackend_;
}

void AlgorithmBackendFactory::setDefaultBackend(BackendType backend)
{
    defaultBackend_ = backend;
}

QStringList AlgorithmBackendFactory::availableBackends() const
{
    QStringList backends;
    backends << "OpenCV";
#ifdef USE_HALCON
    backends << "Halcon";
#endif
    return backends;
}

QString AlgorithmBackendFactory::backendToString(BackendType backend)
{
    switch (backend) {
        case BackendType::Auto:
            return "Auto";
        case BackendType::OpenCV:
            return "OpenCV";
        case BackendType::Halcon:
            return "Halcon";
        default:
            return "Unknown";
    }
}

BackendType AlgorithmBackendFactory::stringToBackend(const QString& str)
{
    if (str.compare("OpenCV", Qt::CaseInsensitive) == 0) {
        return BackendType::OpenCV;
    } else if (str.compare("Halcon", Qt::CaseInsensitive) == 0) {
        return BackendType::Halcon;
    } else {
        return BackendType::Auto;
    }
}

} // namespace Backend
} // namespace Algorithm
} // namespace VisionForge
