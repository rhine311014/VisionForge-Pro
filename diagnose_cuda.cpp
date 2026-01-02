/**
 * CUDA诊断程序
 * 用于测试OpenCV CUDA检测问题
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>

int main() {
    std::cout << "=== OpenCV CUDA 诊断 ===" << std::endl;
    std::cout << "OpenCV 版本: " << CV_VERSION << std::endl;
    std::cout << std::endl;

    try {
        std::cout << "调用 cv::cuda::getCudaEnabledDeviceCount()..." << std::endl;
        int count = cv::cuda::getCudaEnabledDeviceCount();
        std::cout << "返回值: " << count << std::endl;

        if (count > 0) {
            std::cout << std::endl;
            for (int i = 0; i < count; i++) {
                cv::cuda::DeviceInfo info(i);
                std::cout << "设备 " << i << ": " << info.name() << std::endl;
                std::cout << "  计算能力: " << info.majorVersion() << "." << info.minorVersion() << std::endl;
                std::cout << "  总显存: " << info.totalMemory() / (1024*1024) << " MB" << std::endl;
                std::cout << "  兼容性: " << (info.isCompatible() ? "兼容" : "不兼容") << std::endl;
            }
        } else {
            std::cout << std::endl;
            std::cout << "可能原因:" << std::endl;
            std::cout << "1. OpenCV未编译CUDA支持 (但DLL=1.3GB表明已编译)" << std::endl;
            std::cout << "2. CUDA计算能力不匹配 (RTX 3080 Ti = SM 8.6)" << std::endl;
            std::cout << "3. CUDA驱动/运行时版本不兼容" << std::endl;
            std::cout << "4. 环境变量PATH中缺少CUDA DLLs" << std::endl;
        }

    } catch (const cv::Exception& e) {
        std::cout << "OpenCV异常: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "标准异常: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "未知异常" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "按Enter退出..." << std::endl;
    std::cin.get();
    return 0;
}
