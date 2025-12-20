/**
 * @file CoordPosition.h
 * @brief VisionForge Pro 核心坐标位姿类
 *
 * 本文件定义了视觉对位系统中的核心坐标位姿类CoordPosition。
 * 该类直接参考VisionASM的CCoordPos设计，是图像坐标与平台坐标
 * 转换的核心数据结构。
 *
 * 主要功能：
 * - 存储主位置和辅助位置（支持多点对位）
 * - 齐次坐标变换（map/invMap）
 * - 与平台轴位置的相互转换
 * - 搜索区域管理
 *
 * @author VisionForge Team
 * @version 1.2.0
 * @date 2025
 */

#ifndef VISIONFORGE_COORD_POSITION_H
#define VISIONFORGE_COORD_POSITION_H

#include "PlatformTypes.h"
#include <QPointF>
#include <QRectF>
#include <QJsonObject>
#include <QJsonArray>
#include <QMatrix3x3>
#include <QGenericMatrix>
#include <vector>
#include <array>

namespace VisionForge {
namespace Platform {

/**
 * @brief 辅助位置最大数量
 *
 * 对应VisionASM的MaxAuxiliaryPosNum = 4
 * 用于多点对位场景（如4点对位）
 */
constexpr int MaxAuxiliaryPosNum = 4;

/**
 * @brief 坐标位置类型
 *
 * 用于标识操作的是主位置还是辅助位置
 */
enum class CoordPosType {
    AllPos = -2,        ///< 所有位置
    AllAuxPos = -1,     ///< 所有辅助位置
    MainPos = 0,        ///< 主位置
    AuxPos1 = 1,        ///< 辅助位置1
    AuxPos2 = 2,        ///< 辅助位置2
    AuxPos3 = 3,        ///< 辅助位置3
    AuxPos4 = 4         ///< 辅助位置4
};

/**
 * @brief 仿射矩形类
 *
 * 用于表示旋转矩形搜索区域
 */
struct AffineRect {
    double centerX = 0.0;   ///< 中心X坐标
    double centerY = 0.0;   ///< 中心Y坐标
    double width = 100.0;   ///< 宽度
    double height = 100.0;  ///< 高度
    double angle = 0.0;     ///< 旋转角度(degree)

    AffineRect() = default;

    AffineRect(double cx, double cy, double w, double h, double a = 0.0)
        : centerX(cx), centerY(cy), width(w), height(h), angle(a) {}

    /**
     * @brief 转换为轴对齐包围盒
     */
    QRectF toBoundingRect() const;

    /**
     * @brief 获取四个角点
     */
    std::array<QPointF, 4> getCorners() const;

    QJsonObject toJson() const;
    static AffineRect fromJson(const QJsonObject& json);
};

/**
 * @brief 核心坐标位姿类
 *
 * 这是VisionForge Pro对位系统的核心数据结构，直接参考VisionASM的CCoordPos。
 *
 * 坐标系说明：
 * - 图像坐标系：原点在图像左上角，X向右，Y向下
 * - 平台坐标系：由PlatformConfig定义，通常与机械坐标系对应
 *
 * 使用场景：
 * 1. 模板匹配结果存储
 * 2. 对位偏差计算
 * 3. 坐标变换（图像<->平台）
 * 4. 多点对位数据管理
 *
 * 使用示例：
 * @code
 * CoordPosition pos;
 * pos.setMainPos(100.0, 200.0);
 * pos.setAngle(1.5);
 *
 * // 应用标定矩阵变换
 * QMatrix3x3 H = calibration.getHomography();
 * CoordPosition platformPos = pos.map(H);
 *
 * // 转换为平台轴位置
 * PlatformAxisPos axisPos = platformPos.toPlatformAxis(PlatformType::XYD);
 * @endcode
 */
class CoordPosition {
public:
    // ============ 构造函数 ============

    /**
     * @brief 默认构造函数
     */
    CoordPosition();

    /**
     * @brief 主位置构造函数
     * @param x 主X坐标
     * @param y 主Y坐标
     * @param angle 旋转角度(degree)
     */
    CoordPosition(double x, double y, double angle = 0.0);

    /**
     * @brief 完整构造函数（含线角度）
     * @param x 主X坐标
     * @param y 主Y坐标
     * @param angle 旋转角度
     * @param lineAngle1 线角度1
     * @param lineAngle2 线角度2
     */
    CoordPosition(double x, double y, double angle,
                  double lineAngle1, double lineAngle2);

    /**
     * @brief 拷贝构造函数
     */
    CoordPosition(const CoordPosition& other);

    /**
     * @brief 赋值运算符
     */
    CoordPosition& operator=(const CoordPosition& other);

    /**
     * @brief 析构函数
     */
    ~CoordPosition() = default;

    // ============ 主位置访问 ============

    /**
     * @brief 获取主位置X坐标
     */
    double getMainX() const { return m_mainX; }

    /**
     * @brief 获取主位置Y坐标
     */
    double getMainY() const { return m_mainY; }

    /**
     * @brief 获取旋转角度
     */
    double getAngle() const { return m_angle; }

    /**
     * @brief 获取主位置点
     */
    QPointF getMainPos() const { return QPointF(m_mainX, m_mainY); }

    /**
     * @brief 设置主位置X坐标
     */
    void setMainX(double x) { m_mainX = x; }

    /**
     * @brief 设置主位置Y坐标
     */
    void setMainY(double y) { m_mainY = y; }

    /**
     * @brief 设置旋转角度
     */
    void setAngle(double angle) { m_angle = angle; }

    /**
     * @brief 设置主位置
     */
    void setMainPos(double x, double y) {
        m_mainX = x;
        m_mainY = y;
    }

    /**
     * @brief 设置主位置和角度
     */
    void setMainPosAngle(double x, double y, double angle) {
        m_mainX = x;
        m_mainY = y;
        m_angle = angle;
    }

    // ============ 辅助位置访问 ============

    /**
     * @brief 获取辅助位置X坐标
     * @param index 辅助位置索引(0-3)
     * @return X坐标，索引无效返回0
     */
    double getAuxX(int index) const;

    /**
     * @brief 获取辅助位置Y坐标
     * @param index 辅助位置索引(0-3)
     * @return Y坐标，索引无效返回0
     */
    double getAuxY(int index) const;

    /**
     * @brief 获取辅助位置角度
     * @param index 辅助位置索引(0-3)
     * @return 角度，索引无效返回0
     */
    double getAuxAngle(int index) const;

    /**
     * @brief 获取辅助位置点
     * @param index 辅助位置索引(0-3)
     * @return 位置点，索引无效返回(0,0)
     */
    QPointF getAuxPos(int index) const;

    /**
     * @brief 设置辅助位置
     * @param index 辅助位置索引(0-3)
     * @param x X坐标
     * @param y Y坐标
     * @return 设置是否成功
     */
    bool setAuxPos(int index, double x, double y);

    /**
     * @brief 设置辅助位置和角度
     * @param index 辅助位置索引
     * @param x X坐标
     * @param y Y坐标
     * @param angle 角度
     * @return 设置是否成功
     */
    bool setAuxPosAngle(int index, double x, double y, double angle);

    // ============ 线角度 ============

    /**
     * @brief 获取线角度1（两点连线角度）
     */
    double getLineAngle1() const { return m_lineAngle1; }

    /**
     * @brief 获取线角度2
     */
    double getLineAngle2() const { return m_lineAngle2; }

    /**
     * @brief 设置线角度1
     */
    void setLineAngle1(double angle) { m_lineAngle1 = angle; }

    /**
     * @brief 设置线角度2
     */
    void setLineAngle2(double angle) { m_lineAngle2 = angle; }

    /**
     * @brief 是否找到线1
     */
    bool hasFoundLine1() const { return m_foundLine1; }

    /**
     * @brief 是否找到线2
     */
    bool hasFoundLine2() const { return m_foundLine2; }

    /**
     * @brief 设置线1找到标志
     */
    void setFoundLine1(bool found) { m_foundLine1 = found; }

    /**
     * @brief 设置线2找到标志
     */
    void setFoundLine2(bool found) { m_foundLine2 = found; }

    // ============ 搜索区域 ============

    /**
     * @brief 获取搜索区域
     */
    const AffineRect& getSearchRect() const { return m_searchRect; }

    /**
     * @brief 设置搜索区域
     */
    void setSearchRect(const AffineRect& rect) {
        m_searchRect = rect;
        m_foundSearchRect = true;
    }

    /**
     * @brief 是否有有效搜索区域
     */
    bool hasSearchRect() const { return m_foundSearchRect; }

    // ============ 位置偏移 ============

    /**
     * @brief 偏移所有位置
     * @param offsetX X偏移量
     * @param offsetY Y偏移量
     */
    void offset(double offsetX, double offsetY);

    /**
     * @brief 偏移所有位置
     * @param offset 偏移向量
     */
    void offset(const QPointF& offsetVec) {
        offset(offsetVec.x(), offsetVec.y());
    }

    // ============ 坐标变换（核心功能） ============

    /**
     * @brief 正向坐标变换
     *
     * 使用2x2矩阵和平移向量进行仿射变换：
     * P' = M * P + T
     *
     * 用于从图像坐标变换到平台坐标。
     *
     * @param matrix 2x2变换矩阵
     * @param translation 平移向量，默认(0,0)
     * @return 变换后的坐标位姿
     */
    CoordPosition map(const QMatrix3x3& matrix,
                      const QPointF& translation = QPointF(0, 0)) const;

    /**
     * @brief 使用齐次矩阵进行正向变换
     *
     * 使用3x3齐次变换矩阵：
     * [x']   [h00 h01 h02] [x]
     * [y'] = [h10 h11 h12] [y]
     * [1 ]   [h20 h21 h22] [1]
     *
     * @param homography 3x3齐次变换矩阵
     * @return 变换后的坐标位姿
     */
    CoordPosition mapHomography(const double h[9]) const;

    /**
     * @brief 逆向坐标变换
     *
     * 使用2x2矩阵和平移向量的逆变换：
     * P = M^(-1) * (P' - T)
     *
     * 用于从平台坐标变换到图像坐标。
     *
     * @param matrix 2x2变换矩阵（将自动求逆）
     * @param translation 平移向量
     * @return 变换后的坐标位姿
     */
    CoordPosition invMap(const QMatrix3x3& matrix,
                         const QPointF& translation = QPointF(0, 0)) const;

    /**
     * @brief 使用齐次矩阵进行逆向变换
     * @param homography 3x3齐次变换矩阵（将自动求逆）
     * @return 变换后的坐标位姿
     */
    CoordPosition invMapHomography(const double h[9]) const;

    // ============ 平台轴位置转换 ============

    /**
     * @brief 转换为平台轴位置
     *
     * 根据平台类型将坐标位姿转换为平台轴位置。
     * 不同平台类型有不同的转换逻辑：
     * - XYD: 直接映射 (mainX->x1, mainY->y1, angle->d)
     * - X1X2Y: 需要计算双X的分配
     * - DXY: 需要考虑旋转优先
     *
     * @param type 目标平台类型
     * @param platformInfo 平台配置信息（可选，用于特殊计算）
     * @return 平台轴位置
     */
    PlatformAxisPos toPlatformAxis(PlatformType type,
                                   const void* platformInfo = nullptr) const;

    /**
     * @brief 从平台轴位置创建坐标位姿
     *
     * 将平台轴位置转换回坐标位姿表示。
     *
     * @param axisPos 平台轴位置
     * @param type 平台类型
     * @param platformInfo 平台配置信息（可选）
     * @return 坐标位姿
     */
    static CoordPosition fromPlatformAxis(const PlatformAxisPos& axisPos,
                                          PlatformType type,
                                          const void* platformInfo = nullptr);

    // ============ 运算符 ============

    /**
     * @brief 相等比较
     */
    bool operator==(const CoordPosition& other) const;

    /**
     * @brief 不等比较
     */
    bool operator!=(const CoordPosition& other) const {
        return !(*this == other);
    }

    // ============ 序列化 ============

    /**
     * @brief 转换为JSON对象
     */
    QJsonObject toJson() const;

    /**
     * @brief 从JSON对象创建
     */
    static CoordPosition fromJson(const QJsonObject& json);

    /**
     * @brief 转换为字符串（调试用）
     */
    QString toString() const;

    // ============ 工具方法 ============

    /**
     * @brief 检查位置是否为零
     */
    bool isZero(double tolerance = 1e-6) const;

    /**
     * @brief 计算两个位置的距离
     */
    static double distance(const CoordPosition& pos1, const CoordPosition& pos2);

    /**
     * @brief 计算两点连线角度
     */
    static double lineAngle(const CoordPosition& pos1, const CoordPosition& pos2);

private:
    // ============ 主位置 ============
    double m_mainX = 0.0;       ///< 主位置X（像素或世界坐标）
    double m_mainY = 0.0;       ///< 主位置Y
    double m_angle = 0.0;       ///< 旋转角度(degree)

    // ============ 辅助位置 ============
    std::array<double, MaxAuxiliaryPosNum> m_auxX;      ///< 辅助位置X
    std::array<double, MaxAuxiliaryPosNum> m_auxY;      ///< 辅助位置Y
    std::array<double, MaxAuxiliaryPosNum> m_auxAngle;  ///< 辅助位置角度

    // ============ 线角度 ============
    double m_lineAngle1 = 0.0;  ///< 线角度1（两点连线）
    double m_lineAngle2 = 0.0;  ///< 线角度2
    bool m_foundLine1 = false;  ///< 是否找到线1
    bool m_foundLine2 = false;  ///< 是否找到线2

    // ============ 搜索区域 ============
    AffineRect m_searchRect;    ///< 搜索区域
    bool m_foundSearchRect = false; ///< 是否有有效搜索区域

private:
    /**
     * @brief 应用2x2矩阵变换到单个点
     */
    static QPointF transformPoint(const QPointF& point,
                                  const QMatrix3x3& matrix,
                                  const QPointF& translation);

    /**
     * @brief 计算2x2矩阵的逆矩阵
     */
    static bool invertMatrix2x2(const QMatrix3x3& matrix, QMatrix3x3& invMatrix);
};

} // namespace Platform
} // namespace VisionForge

// 注册元类型
Q_DECLARE_METATYPE(VisionForge::Platform::CoordPosition)
Q_DECLARE_METATYPE(VisionForge::Platform::AffineRect)

#endif // VISIONFORGE_COORD_POSITION_H
