// svshape.h


#pragma once

#include "svDef.h"
#include "svError.h"
#include "svHandle.h"
#include "svVector.h"
#include "svXform.h"
#include "svRLEBuffer.h"

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            图形基类
// ---------------------------------------------------------------------
// 图形基类scShape是抽象基类。它是图形的基类，定义了图形的基本接口函数



class scShape;
class scRect;
class cpImageMask;

typedef scPtrHandle<scShape> scShapePtrh;
typedef scPtrHandle_const<scShape> scShapePtrh_const;

#define smDerivedShapePtrhDecl(class) \
	smDerivedPtrHdlDcl(class ## Ptrh, class, scShapePtrh, scShapePtrh_const)

class SVCORE_API scShape : public virtual scRepBase
{
public:
	virtual ~scShape();

public:
	
	virtual ShapeType GetType() const = 0;				// 返回类型的枚举值
	virtual scShapePtrh Clone() const = 0;							// 复制得到新的图形

	virtual bool IsOpenContour() const = 0;							// 图形是否为开的轮廓
	virtual bool IsRegion() const = 0;								// 图形是否为区域
	virtual bool IsFinite() const = 0;								// 图形是否为有限的
	virtual bool IsEmpty() const = 0;								// 图形是否为空
	virtual scRect BoundingBox() const;								// 返回图形的最小外接矩形
	virtual bool GetImageMask(cpImageMask& imagemask,int nXStart=-INT_MAX,int nXEnd=INT_MAX,int nYStart=-INT_MAX,int nYEnd=INT_MAX) const;		// 获得掩模图像

	virtual bool Within(const sc2Vector& p) const;					// 判断点p是否在图形内部

	virtual sc2Vector NearestPoint(const sc2Vector& p) const;		// 计算图形上离点p最近的点
	virtual double DistanceToPoint(const sc2Vector& p) const;		// 计算图形与点p之间的距离

	virtual bool IsEqual(const scShape& rhs) const = 0;				// 判断给定图形rhs是否与当前图形相等，仅供内部使用
	virtual bool IsMultipleContours() const;						// 判断是否为多图形，仅供内部使用

};

#pragma pack(pop)
