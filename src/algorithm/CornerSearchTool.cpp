/**
 * @file CornerSearchTool.cpp
 * @brief 角点搜索工具实现文件
 *
 * @details
 * 本文件实现了CornerSearchTool类的所有方法，包括：
 * - 四种角点检测算法的实现：Harris、Shi-Tomasi、FAST、ORB
 * - 亚像素精化处理
 * - 参数序列化和反序列化
 *
 * @author VisionForge Team
 * @date 2025-12-30
 */

#include "algorithm/CornerSearchTool.h"
#include "base/Logger.h"

#include <QElapsedTimer>
#include <QJsonArray>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

namespace VisionForge {
namespace Algorithm {

/**
 * @brief 构造函数实现
 *
 * 初始化工具的显示名称
 */
CornerSearchTool::CornerSearchTool(QObject* parent)
    : VisionTool(parent)
{
    setDisplayName(tr("角点搜索"));
}

/**
 * @brief 执行角点检测的主入口方法
 *
 * 处理流程：
 * 1. 验证输入图像的有效性
 * 2. 将彩色图像转换为灰度图像
 * 3. 根据选择的方法调用对应的检测函数
 * 4. 整理输出结果
 */
bool CornerSearchTool::process(const Base::ImageData::Ptr& input, ToolResult& output)
{
    // 步骤1：验证输入图像
    if (!input || input->isEmpty()) {
        output.success = false;
        output.errorMessage = tr("输入图像为空");
        return false;
    }

    // 启动计时器
    QElapsedTimer timer;
    timer.start();

    // 步骤2：转换为灰度图像（角点检测需要灰度图）
    cv::Mat mat = input->mat();
    cv::Mat grayMat;
    if (mat.channels() == 3) {
        // BGR转灰度
        cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);
    } else {
        // 已经是灰度图，直接使用
        grayMat = mat;
    }

    bool success = false;
    corners_.clear();  // 清空之前的检测结果

    // 步骤3：根据选择的方法执行检测
    try {
        switch (method_) {
            case Harris:
                // Harris角点检测
                success = processHarris(grayMat);
                break;
            case ShiTomasi:
                // Shi-Tomasi角点检测（Good Features to Track）
                success = processShiTomasi(grayMat);
                break;
            case FAST:
                // FAST特征点检测
                success = processFAST(grayMat);
                break;
            case ORB:
                // ORB特征点检测
                success = processORB(grayMat);
                break;
        }
    } catch (const cv::Exception& e) {
        // 捕获OpenCV异常
        output.success = false;
        output.errorMessage = QString("OpenCV错误: %1").arg(e.what());
        LOG_ERROR(output.errorMessage);
        return false;
    }

    // 记录执行时间
    output.executionTime = timer.elapsed();

    // 步骤4：处理成功时整理输出结果
    if (success) {
        output.success = true;
        output.setValue("cornerCount", static_cast<int>(corners_.size()));

        // 将检测结果转换为JSON数组
        QJsonArray cornerArray;
        for (const auto& corner : corners_) {
            QJsonObject obj;
            obj["x"] = corner.x;
            obj["y"] = corner.y;
            obj["response"] = corner.response;
            obj["angle"] = corner.angle;
            obj["size"] = corner.size;
            cornerArray.append(obj);
        }
        output.data["corners"] = cornerArray;

        // 更新状态文本
        setStatusText(tr("找到 %1 个角点，耗时 %2 ms")
            .arg(corners_.size()).arg(output.executionTime));
    }

    return success;
}

/**
 * @brief 使用Harris算法检测角点
 *
 * Harris角点检测算法步骤：
 * 1. 计算Harris响应图
 * 2. 归一化响应值到[0,1]范围
 * 3. 阈值化并进行非极大值抑制
 * 4. 可选的亚像素精化
 *
 * @param gray 灰度输入图像
 * @return 是否检测成功
 */
bool CornerSearchTool::processHarris(const cv::Mat& gray)
{
    // 步骤1：计算Harris响应图
    // cornerHarris函数计算每个像素的Harris角点响应值
    cv::Mat harrisResponse;
    cv::cornerHarris(gray,              // 输入图像
                     harrisResponse,     // 输出响应图
                     harrisBlockSize_,   // 邻域大小
                     harrisKSize_,       // Sobel算子孔径
                     harrisK_);          // Harris参数k

    // 步骤2：归一化响应值到[0,1]范围，便于阈值化
    cv::Mat normalized;
    cv::normalize(harrisResponse, normalized, 0, 1, cv::NORM_MINMAX);

    // 用于存储检测到的角点
    std::vector<cv::Point2f> points;

    // 步骤3：阈值化并进行非极大值抑制
    // 遍历图像（避开边界像素）
    for (int y = 1; y < normalized.rows - 1; ++y) {
        for (int x = 1; x < normalized.cols - 1; ++x) {
            float val = normalized.at<float>(y, x);

            // 检查是否超过阈值
            if (val > harrisThreshold_) {
                // 局部极大值检测（3x3邻域非极大值抑制）
                bool isMax = true;
                for (int dy = -1; dy <= 1 && isMax; ++dy) {
                    for (int dx = -1; dx <= 1 && isMax; ++dx) {
                        if (dx == 0 && dy == 0) continue;  // 跳过中心点
                        // 如果存在邻域点响应值>=当前点，则不是极大值
                        if (normalized.at<float>(y + dy, x + dx) >= val) {
                            isMax = false;
                        }
                    }
                }
                // 如果是局部极大值，添加到角点列表
                if (isMax) {
                    points.push_back(cv::Point2f(static_cast<float>(x), static_cast<float>(y)));
                }
            }
        }
    }

    // 步骤4：亚像素精化（如果启用）
    if (useSubPixel_ && !points.empty()) {
        // 设置亚像素精化参数
        cv::Size winSize(subPixelWinSize_, subPixelWinSize_);  // 搜索窗口大小
        // 迭代终止条件：精度达到0.01或迭代30次
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.01);
        // 执行亚像素精化
        cv::cornerSubPix(gray, points, winSize, cv::Size(-1, -1), criteria);
    }

    // 步骤5：限制角点数量（按响应值排序后取前N个）
    if (static_cast<int>(points.size()) > maxCorners_) {
        points.resize(maxCorners_);
    }

    // 步骤6：转换结果到CornerPoint结构
    for (const auto& pt : points) {
        CornerPoint cp;
        cp.x = pt.x;
        cp.y = pt.y;
        // 获取该点的响应值
        cp.response = normalized.at<float>(static_cast<int>(pt.y), static_cast<int>(pt.x));
        corners_.push_back(cp);
    }

    return true;
}

/**
 * @brief 使用Shi-Tomasi算法检测角点
 *
 * Shi-Tomasi是Harris的改进版本，使用最小特征值作为角点度量。
 * 使用OpenCV的goodFeaturesToTrack函数实现。
 *
 * @param gray 灰度输入图像
 * @return 是否检测成功
 */
bool CornerSearchTool::processShiTomasi(const cv::Mat& gray)
{
    // 存储检测到的角点
    std::vector<cv::Point2f> points;

    // 步骤1：使用goodFeaturesToTrack检测角点
    // 这是Shi-Tomasi算法的OpenCV实现
    cv::goodFeaturesToTrack(
        gray,               // 输入图像
        points,             // 输出角点
        maxCorners_,        // 最大角点数量
        qualityLevel_,      // 质量水平（相对于最佳角点的比例）
        minDistance_,       // 角点之间的最小距离
        cv::noArray(),      // mask（不使用）
        blockSize_);        // 计算梯度的邻域大小

    // 步骤2：亚像素精化（如果启用）
    if (useSubPixel_ && !points.empty()) {
        cv::Size winSize(subPixelWinSize_, subPixelWinSize_);
        cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.01);
        cv::cornerSubPix(gray, points, winSize, cv::Size(-1, -1), criteria);
    }

    // 步骤3：转换结果到CornerPoint结构
    for (const auto& pt : points) {
        CornerPoint cp;
        cp.x = pt.x;
        cp.y = pt.y;
        // Shi-Tomasi不直接提供响应值，保持默认值0
        corners_.push_back(cp);
    }

    return true;
}

/**
 * @brief 使用FAST算法检测特征点
 *
 * FAST是一种高速角点检测算法，通过比较中心像素与其圆周上的像素来检测特征点。
 * 特点是速度极快，适合实时应用。
 *
 * @param gray 灰度输入图像
 * @return 是否检测成功
 */
bool CornerSearchTool::processFAST(const cv::Mat& gray)
{
    // 存储检测到的特征点
    std::vector<cv::KeyPoint> keypoints;

    // 步骤1：执行FAST检测
    cv::FAST(gray,              // 输入图像
             keypoints,         // 输出特征点
             fastThreshold_,    // 强度差阈值
             fastNMS_,          // 是否使用非极大值抑制
             static_cast<cv::FastFeatureDetector::DetectorType>(fastType_));  // 检测器类型

    // 步骤2：如果检测数量超过限制，按响应值排序后截取
    if (static_cast<int>(keypoints.size()) > maxCorners_) {
        // 按响应值降序排序
        std::sort(keypoints.begin(), keypoints.end(),
            [](const cv::KeyPoint& a, const cv::KeyPoint& b) {
                return a.response > b.response;  // 响应值大的排在前面
            });
        // 只保留前maxCorners_个
        keypoints.resize(maxCorners_);
    }

    // 步骤3：转换结果到CornerPoint结构
    for (const auto& kp : keypoints) {
        CornerPoint cp;
        cp.x = kp.pt.x;           // 特征点X坐标
        cp.y = kp.pt.y;           // 特征点Y坐标
        cp.response = kp.response; // 响应值
        cp.angle = kp.angle;       // 方向（FAST不计算方向，通常为-1）
        cp.size = kp.size;         // 尺度
        corners_.push_back(cp);
    }

    return true;
}

/**
 * @brief 使用ORB算法检测特征点
 *
 * ORB (Oriented FAST and Rotated BRIEF) 是一种快速且具有旋转不变性的特征检测器。
 * 它结合了FAST检测器和BRIEF描述子，并添加了方向计算。
 *
 * @param gray 灰度输入图像
 * @return 是否检测成功
 */
bool CornerSearchTool::processORB(const cv::Mat& gray)
{
    // 步骤1：创建ORB检测器
    // nfeatures参数指定最大特征点数量
    auto orb = cv::ORB::create(maxCorners_);

    // 存储检测到的特征点
    std::vector<cv::KeyPoint> keypoints;

    // 步骤2：执行特征点检测
    orb->detect(gray, keypoints);

    // 步骤3：转换结果到CornerPoint结构
    for (const auto& kp : keypoints) {
        CornerPoint cp;
        cp.x = kp.pt.x;           // 特征点X坐标
        cp.y = kp.pt.y;           // 特征点Y坐标
        cp.response = kp.response; // 响应值
        cp.angle = kp.angle;       // 特征点方向（ORB计算方向）
        cp.size = kp.size;         // 特征点尺度
        corners_.push_back(cp);
    }

    return true;
}

/**
 * @brief 序列化参数到JSON对象
 *
 * 将所有检测参数打包为JSON格式，用于保存配置
 */
QJsonObject CornerSearchTool::serializeParams() const
{
    QJsonObject json;

    // 检测方法
    json["method"] = static_cast<int>(method_);

    // Harris参数
    json["harrisBlockSize"] = harrisBlockSize_;
    json["harrisKSize"] = harrisKSize_;
    json["harrisK"] = harrisK_;
    json["harrisThreshold"] = harrisThreshold_;

    // Shi-Tomasi参数
    json["maxCorners"] = maxCorners_;
    json["qualityLevel"] = qualityLevel_;
    json["minDistance"] = minDistance_;
    json["blockSize"] = blockSize_;

    // FAST参数
    json["fastThreshold"] = fastThreshold_;
    json["fastNMS"] = fastNMS_;
    json["fastType"] = fastType_;

    // 通用参数
    json["useSubPixel"] = useSubPixel_;
    json["subPixelWinSize"] = subPixelWinSize_;

    return json;
}

/**
 * @brief 从JSON对象反序列化参数
 *
 * 从JSON对象恢复所有参数设置
 */
void CornerSearchTool::deserializeParams(const QJsonObject& json)
{
    // 检测方法
    method_ = static_cast<Method>(json.value("method").toInt(ShiTomasi));

    // Harris参数
    harrisBlockSize_ = json.value("harrisBlockSize").toInt(2);
    harrisKSize_ = json.value("harrisKSize").toInt(3);
    harrisK_ = json.value("harrisK").toDouble(0.04);
    harrisThreshold_ = json.value("harrisThreshold").toDouble(0.01);

    // Shi-Tomasi参数
    maxCorners_ = json.value("maxCorners").toInt(100);
    qualityLevel_ = json.value("qualityLevel").toDouble(0.01);
    minDistance_ = json.value("minDistance").toDouble(10.0);
    blockSize_ = json.value("blockSize").toInt(3);

    // FAST参数
    fastThreshold_ = json.value("fastThreshold").toInt(10);
    fastNMS_ = json.value("fastNMS").toBool(true);
    fastType_ = json.value("fastType").toInt(cv::FastFeatureDetector::TYPE_9_16);

    // 通用参数
    useSubPixel_ = json.value("useSubPixel").toBool(true);
    subPixelWinSize_ = json.value("subPixelWinSize").toInt(5);
}

} // namespace Algorithm
} // namespace VisionForge
