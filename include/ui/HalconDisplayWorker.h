/**
 * @file HalconDisplayWorker.h
 * @brief Halcon图像显示工作线程
 * @details 在子线程中处理图像转换和显示，避免阻塞主线程
 * @author VisionForge Team
 * @date 2025-12-14
 */

#pragma once

#include "base/ImageData.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#include "HalconCpp.h"
using namespace HalconCpp;
#endif

namespace VisionForge {
namespace UI {

/**
 * @class HalconDisplayWorker
 * @brief Halcon图像显示工作线程
 *
 * 功能：
 * - 在子线程中处理图像格式转换
 * - 在子线程中显示大尺寸图像
 * - 避免阻塞主线程UI
 * - 线程安全的窗口操作
 */
class HalconDisplayWorker : public QThread
{
    Q_OBJECT

public:
    explicit HalconDisplayWorker(QObject* parent = nullptr);
    ~HalconDisplayWorker() override;

    /**
     * @brief 设置Halcon窗口句柄（由主线程调用）
     */
    void setWindowHandle(const HTuple& windowHandle);

    /**
     * @brief 请求显示图像（由主线程调用）
     */
    void requestDisplayImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 请求清空窗口
     */
    void requestClearWindow();

    /**
     * @brief 设置XLD轮廓对象（由主线程调用）
     */
    void setXLDContours(const QList<HXLDCont>& contours,
                       const QStringList& colors = QStringList());

    /**
     * @brief 清除所有XLD轮廓
     */
    void clearXLDContours();

    /**
     * @brief 停止线程
     */
    void stop();

signals:
    /**
     * @brief 图像显示完成信号
     */
    void imageDisplayed();

    /**
     * @brief 显示错误信号
     */
    void displayError(const QString& errorMsg);

protected:
    void run() override;

private:
    /**
     * @brief 将ImageData转换为HImage
     */
    HImage imageDataToHImage(const Base::ImageData::Ptr& image);

    /**
     * @brief 执行图像显示
     */
    void performDisplay();

    /**
     * @brief 执行窗口清空
     */
    void performClear();

private:
#ifdef _WIN32
    HTuple windowHandle_;               // Halcon窗口句柄
    HImage currentHImage_;              // 当前Halcon图像
    QList<HXLDCont> xldContours_;       // XLD轮廓列表
    QStringList xldColors_;             // XLD轮廓颜色列表
#endif

    Base::ImageData::Ptr pendingImage_; // 待显示的图像

    QMutex mutex_;                      // 互斥锁
    QWaitCondition condition_;          // 条件变量

    bool stopRequested_;                // 停止标志
    bool hasNewImage_;                  // 有新图像标志
    bool clearRequested_;               // 清空请求标志
    bool hasXLDUpdate_;                 // XLD轮廓更新标志

    bool windowHandleSet_;              // 窗口句柄是否已设置
};

} // namespace UI
} // namespace VisionForge
