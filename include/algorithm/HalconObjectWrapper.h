/**
 * @file HalconObjectWrapper.h
 * @brief Halcon对象包装类，用于在Qt系统中传递
 * @author VisionForge Team
 * @date 2025-12-15
 */

#pragma once

#ifdef _WIN32

#include <HalconCpp.h>
#include <QVariant>
#include <QSharedPointer>

using namespace HalconCpp;

namespace VisionForge {
namespace Algorithm {

/**
 * @brief Halcon对象包装基类
 */
class HalconObjectWrapper {
public:
    virtual ~HalconObjectWrapper() = default;

    enum ObjectType {
        XLD_Contour,
        XLD_Polygon,
        Region,
        Image
    };

    virtual ObjectType type() const = 0;
};

/**
 * @brief XLD轮廓包装类
 */
class XLDContourWrapper : public HalconObjectWrapper {
public:
    XLDContourWrapper() = default;
    explicit XLDContourWrapper(const HXLDCont& contours) : contours_(contours) {}

    ObjectType type() const override { return XLD_Contour; }

    const HXLDCont& contours() const { return contours_; }
    void setContours(const HXLDCont& contours) { contours_ = contours; }

private:
    HXLDCont contours_;
};

/**
 * @brief Region包装类
 */
class RegionWrapper : public HalconObjectWrapper {
public:
    RegionWrapper() = default;
    explicit RegionWrapper(const HRegion& region) : region_(region) {}

    ObjectType type() const override { return Region; }

    const HRegion& region() const { return region_; }
    void setRegion(const HRegion& region) { region_ = region; }

private:
    HRegion region_;
};

// 定义智能指针类型
using HalconObjectPtr = QSharedPointer<HalconObjectWrapper>;
using XLDContourPtr = QSharedPointer<XLDContourWrapper>;
using RegionPtr = QSharedPointer<RegionWrapper>;

} // namespace Algorithm
} // namespace VisionForge

// 注册到Qt元类型系统
Q_DECLARE_METATYPE(VisionForge::Algorithm::HalconObjectPtr)

#endif // _WIN32
