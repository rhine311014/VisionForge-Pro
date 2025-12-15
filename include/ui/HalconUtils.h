/**
 * @file HalconUtils.h
 * @brief Halcon工具函数集
 * @details 提供Halcon常用的辅助函数，如字体设置、颜色配置等
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#include <QString>

#ifdef _WIN32
#include "HalconCpp.h"

// 解决Halcon宏冲突
#ifdef high
#undef high
#endif
#ifdef low
#undef low
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

using namespace HalconCpp;
#endif

namespace VisionForge {
namespace UI {

/**
 * @class HalconUtils
 * @brief Halcon工具类
 *
 * 提供常用的Halcon辅助函数
 */
class HalconUtils
{
public:
    /**
     * @brief 设置显示字体
     * @param windowHandle Halcon窗口句柄
     * @param size 字体大小（-1表示使用默认值16）
     * @param font 字体类型："mono"（等宽字体）、"sans"（无衬线字体）、"serif"（衬线字体）、"Courier"
     * @param bold 是否粗体
     * @param slant 是否斜体
     *
     * 参考MVTec官方字体设置函数
     */
#ifdef _WIN32
    static void SetDisplayFont(
        const HTuple& windowHandle,
        int size = -1,
        const QString& font = "mono",
        bool bold = false,
        bool slant = false
    );
#endif

    /**
     * @brief 设置窗口颜色模式
     * @param windowHandle Halcon窗口句柄
     * @param numGray 灰度级别（12推荐）
     */
#ifdef _WIN32
    static void SetColoredMode(const HTuple& windowHandle, int numGray = 12);
#endif

    /**
     * @brief 启用UTF-8字符串编码
     *
     * 使Halcon能够正确显示中文等UTF-8字符
     */
    static void EnableUTF8();

    /**
     * @brief 设置线宽
     * @param windowHandle Halcon窗口句柄
     * @param lineWidth 线宽（像素）
     */
#ifdef _WIN32
    static void SetLineWidth(const HTuple& windowHandle, int lineWidth);
#endif

    /**
     * @brief 设置绘制模式
     * @param windowHandle Halcon窗口句柄
     * @param drawMode 绘制模式："margin"（边缘）、"fill"（填充）
     */
#ifdef _WIN32
    static void SetDrawMode(const HTuple& windowHandle, const QString& drawMode);
#endif

    /**
     * @brief 设置颜色
     * @param windowHandle Halcon窗口句柄
     * @param color 颜色名称或RGB值
     */
#ifdef _WIN32
    static void SetColor(const HTuple& windowHandle, const QString& color);
#endif

    /**
     * @brief 显示文本
     * @param windowHandle Halcon窗口句柄
     * @param text 文本内容
     * @param coordSystem 坐标系："window"（窗口坐标）、"image"（图像坐标）
     * @param row 行坐标
     * @param col 列坐标
     * @param color 文本颜色
     * @param box 是否显示背景框
     * @param boxColor 背景框颜色
     */
#ifdef _WIN32
    static void DisplayText(
        const HTuple& windowHandle,
        const QString& text,
        const QString& coordSystem,
        double row,
        double col,
        const QString& color = "white",
        bool box = true,
        const QString& boxColor = "black"
    );
#endif

    /**
     * @brief 显示带阴影的文本（更清晰）
     * @param windowHandle Halcon窗口句柄
     * @param text 文本内容
     * @param row 行坐标
     * @param col 列坐标
     * @param color 文本颜色
     */
#ifdef _WIN32
    static void DisplayTextWithShadow(
        const HTuple& windowHandle,
        const QString& text,
        double row,
        double col,
        const QString& color = "yellow"
    );
#endif

    /**
     * @brief 初始化窗口（设置常用默认参数）
     * @param windowHandle Halcon窗口句柄
     */
#ifdef _WIN32
    static void InitializeWindow(const HTuple& windowHandle);
#endif

    /**
     * @brief 获取图像尺寸并自适应窗口显示区域
     * @param windowHandle Halcon窗口句柄
     * @param image Halcon图像
     */
#ifdef _WIN32
    static void FitImageToWindow(const HTuple& windowHandle, const HImage& image);
#endif

private:
    HalconUtils() = delete;  // 工具类，禁止实例化
};

} // namespace UI
} // namespace VisionForge
