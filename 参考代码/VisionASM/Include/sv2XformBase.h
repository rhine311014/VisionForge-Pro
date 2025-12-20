#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svVector.h"
#include "svMatrix.h"
#include "svHandle.h"
#include <cassert>

#pragma pack(push,8)

class sc2XformBase;
class sc2XformLinear;

typedef scPtrHandle<sc2XformBase>       sc2XformBasePtrh;
typedef scPtrHandle_const<sc2XformBase> sc2XformBasePtrh_const;

// ---------------------------------------------------------------------
//                            二维变换基类sc2XformBase
// ---------------------------------------------------------------------
// 所有的二维变换类都派生自二维变换基类sc2XformBase，包括二维线性变换
// sc2XformLinear、二维刚体变换sc2XformRigid等。

class SVCORE_API sc2XformBase : public scRepBase
{
public:
	//返回当前变换是否线性变换
	virtual bool IsLinear() const = 0;

	//返回当前变换在给定点的最优线性变换
	virtual sc2XformLinear LinearXform(const sc2Vector& atThisPt) const = 0;

	//映射一个二维坐标点
	virtual sc2Vector MapPoint(const sc2Vector& pt) const = 0;
	sc2Vector operator*(const sc2Vector& pt) const;

	//从左到右组合两个变换
	virtual sc2XformBasePtrh ComposeBase(const sc2XformBase& rhs) const = 0;
	sc2XformBasePtrh operator*(const sc2XformBase& rhs) const;

	//逆变换
	virtual sc2XformBasePtrh InverseBase() const = 0;

	//拷贝到一个新分配的对象
	virtual sc2XformBasePtrh Clone() const = 0;

protected:
	//不允许不同类型的二维变换进行相互赋值操作，因此该操作为protected
	sc2XformBase& operator=(const sc2XformBase& rhs);
};

inline sc2Vector sc2XformBase::operator*(const sc2Vector& pt) const
{
	return MapPoint(pt);
}

inline sc2XformBasePtrh sc2XformBase::operator*(const sc2XformBase& rhs) const
{
	return ComposeBase(rhs);	
}

inline sc2XformBase& sc2XformBase::operator=(const sc2XformBase& rhs)
{
	if (this != &rhs)
	{
		
	}

	return *this;
}

#pragma pack(pop)
