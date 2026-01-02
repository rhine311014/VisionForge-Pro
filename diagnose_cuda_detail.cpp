/**
 * 详细CUDA诊断程序
 * 检测CUDA环境和OpenCV CUDA支持的问题
 */
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>

// 检查DLL是否可加载
bool checkDllAvailable(const char* dllName)
{
#ifdef _WIN32
    HMODULE hModule = LoadLibraryExA(dllName, NULL, LOAD_LIBRARY_AS_DATAFILE);
    if (hModule != NULL) {
        FreeLibrary(hModule);
        return true;
    }
    return false;
#else
    return true;
#endif
}

// 获取DLL的完整路径
std::string getDllPath(const char* dllName)
{
#ifdef _WIN32
    HMODULE hModule = LoadLibraryA(dllName);
    if (hModule != NULL) {
        char path[MAX_PATH];
        GetModuleFileNameA(hModule, path, MAX_PATH);
        FreeLibrary(hModule);
        return std::string(path);
    }
    return "NOT FOUND";
#else
    return "N/A";
#endif
}

int main()
{
    std::cout << "====================================" << std::endl;
    std::cout << "  CUDA 详细诊断程序" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;

    // 1. OpenCV版本
    std::cout << "[1] OpenCV 版本信息" << std::endl;
    std::cout << "    版本: " << CV_VERSION << std::endl;
    std::cout << "    构建信息:" << std::endl;
    std::string buildInfo = cv::getBuildInformation();
    // 提取CUDA相关行
    size_t pos = 0;
    while ((pos = buildInfo.find("CUDA", pos)) != std::string::npos) {
        size_t lineStart = buildInfo.rfind('\n', pos);
        size_t lineEnd = buildInfo.find('\n', pos);
        if (lineStart == std::string::npos) lineStart = 0;
        else lineStart++;
        if (lineEnd == std::string::npos) lineEnd = buildInfo.length();
        std::cout << "    " << buildInfo.substr(lineStart, lineEnd - lineStart) << std::endl;
        pos = lineEnd;
    }
    std::cout << std::endl;

    // 2. 检查关键DLL
    std::cout << "[2] 关键 CUDA DLL 检查" << std::endl;
    const char* cudaDlls[] = {
        "cudart64_12.dll",
        "nvcuda.dll",        // NVIDIA CUDA Driver
        "nppc64_12.dll",     // NPP Core
        "nppial64_12.dll",   // NPP Image Arithmetic
        "nppig64_12.dll",    // NPP Image Geometry
        "nppist64_12.dll",   // NPP Image Statistics
        "nppitc64_12.dll",   // NPP Image Threshold
        "nppidei64_12.dll",  // NPP Image Data Exchange
        "nppif64_12.dll",    // NPP Image Filtering
        "nppim64_12.dll",    // NPP Image Morphology
        "cublas64_12.dll",   // cuBLAS
        "cublasLt64_12.dll", // cuBLAS Lt
        "cufft64_11.dll",    // cuFFT
        nullptr
    };

    bool allDllsFound = true;
    for (int i = 0; cudaDlls[i] != nullptr; ++i) {
        bool found = checkDllAvailable(cudaDlls[i]);
        if (found) {
            std::cout << "    [OK] " << cudaDlls[i] << std::endl;
            std::cout << "         路径: " << getDllPath(cudaDlls[i]) << std::endl;
        } else {
            std::cout << "    [MISSING] " << cudaDlls[i] << std::endl;
            allDllsFound = false;
        }
    }
    std::cout << std::endl;

    if (!allDllsFound) {
        std::cout << "[!] 警告: 部分CUDA DLL未找到！" << std::endl;
        std::cout << "    请将以下路径添加到系统PATH环境变量:" << std::endl;
        std::cout << "    D:\\Program Files\\CUDA12.5\\bin" << std::endl;
        std::cout << std::endl;
    }

    // 3. OpenCV CUDA支持检测
    std::cout << "[3] OpenCV CUDA 设备检测" << std::endl;
    try {
        std::cout << "    调用 cv::cuda::getCudaEnabledDeviceCount()..." << std::endl;
        int deviceCount = cv::cuda::getCudaEnabledDeviceCount();
        std::cout << "    返回值: " << deviceCount << std::endl;

        if (deviceCount > 0) {
            std::cout << std::endl;
            std::cout << "    检测到 " << deviceCount << " 个CUDA设备:" << std::endl;
            for (int i = 0; i < deviceCount; i++) {
                cv::cuda::DeviceInfo info(i);
                std::cout << "    设备 " << i << ": " << info.name() << std::endl;
                std::cout << "      计算能力: " << info.majorVersion() << "." << info.minorVersion() << std::endl;
                std::cout << "      总显存: " << info.totalMemory() / (1024*1024) << " MB" << std::endl;
                std::cout << "      兼容性: " << (info.isCompatible() ? "兼容" : "不兼容") << std::endl;
            }
        } else if (deviceCount == 0) {
            std::cout << std::endl;
            std::cout << "    [!] 未检测到CUDA设备" << std::endl;
            std::cout << std::endl;
            std::cout << "    可能原因:" << std::endl;
            std::cout << "    1. CUDA DLL未在PATH中（请检查上方DLL列表）" << std::endl;
            std::cout << "    2. NVIDIA驱动未正确安装" << std::endl;
            std::cout << "    3. GPU计算能力与OpenCV编译不匹配" << std::endl;
        } else {
            std::cout << "    [!] CUDA运行时错误（返回负值）" << std::endl;
        }
    } catch (const cv::Exception& e) {
        std::cout << "    [ERROR] OpenCV异常: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "    [ERROR] 标准异常: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "    [ERROR] 未知异常" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "  诊断完成" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << std::endl;
    std::cout << "按Enter退出..." << std::endl;
    std::cin.get();
    return 0;
}
