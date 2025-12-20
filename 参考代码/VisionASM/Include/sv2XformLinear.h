#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svVector.h"
#include "svMatrix.h"
#include "sv2XformBase.h"
#include "sv2XformComposed.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            二维线性变换sc2XformLinear
// ---------------------------------------------------------------------
// 二维刚体变换由矩阵c、平移向量t组成。

class SVCORE_API sc2XformLinear : public sc2XformBase
{
public:
	sc2XformLinear() {}

	sc2XformLinear(const sc2Matrix& c, const sc2Vector& t) : m_VectorT(t), m_MatrixC(c) {}

	sc2XformLinear(const sc2XformLinear& xform) : m_VectorT(xform.GetTrans()), m_MatrixC(xform.GetMatrix()) {}

	sc2XformLinear(const sc2Vector& t, const scRadian& xRot, const scRadian& yRot,
		double xScale, double yScale) : m_VectorT(t), m_MatrixC(sc2Matrix(xRot,yRot,xScale,yScale)) {}

	sc2XformLinear(const sc2Vector& t, double scale, double aspect,
		const scRadian& shear, const scRadian& rotation) : m_VectorT(t), m_MatrixC(sc2Matrix(scale,aspect,shear,rotation)) {}

	//获取/设置matrix，trans
	const sc2Matrix& GetMatrix() const { return m_MatrixC; }
	const sc2Vector& GetTrans() const { return m_VectorT; }
	void SetMatrix(const sc2Matrix& c) { m_MatrixC = c; }
	void SetTrans(const sc2Vector& t) { m_VectorT = t; }

	//逆变换
	//如果当前变换奇异（不可逆），则返回HSingular；否则，返回HSuccess
	HStatus Inverse(sc2XformLinear& xform) const;

	//从左到右组合两个线性变换
	sc2XformLinear operator*(const sc2XformLinear& xform) const { return sc2XformLinear(m_MatrixC * xform.m_MatrixC, m_MatrixC * xform.m_VectorT + m_VectorT); }
	sc2XformLinear Compose(const sc2XformLinear& xform) const { return *this * xform; }

	//从左到右组合两个变换，返回变换后的结果的基类指针
	sc2XformBasePtrh operator*(const sc2XformBase& rhs) const { return sc2XformBase::operator*(rhs); }

	//映射一个二维坐标点
	sc2Vector operator*(const sc2Vector& pt) const { return sc2XformBase::operator*(pt); }

	//获取X轴旋转、Y轴旋转、X轴缩放因子、Y轴缩放因子
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetXRot(scRadian& xRot) const { return m_MatrixC.GetXRot(xRot); }
	HStatus GetYRot(scRadian& yRot) const { return m_MatrixC.GetYRot(yRot); }
	HStatus GetXScale(double& xScale) const { return m_MatrixC.GetXScale(xScale); }
	HStatus GetYScale(double& yScale) const { return m_MatrixC.GetYScale(yScale); }

	//获取缩放因子、纵横比例、切变、旋转
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetScale(double& scale) const { return m_MatrixC.GetScale(scale); }
	HStatus GetAspect(double& aspect) const { return m_MatrixC.GetAspect(aspect); }
	HStatus GetShear(scRadian& shear) const { return m_MatrixC.GetShear(shear); }
	HStatus GetRotation(scRadian& rotation) const { return m_MatrixC.GetRotation(rotation); }

	//逆映射一个二维坐标点
	sc2Vector InvMapPoint(const sc2Vector& pt) const;

	//映射/逆映射一个二维向量（长度和方向）
	sc2Vector MapVector(const sc2Vector& vect) const;
	sc2Vector InvMapVector(const sc2Vector& vect) const;

	//映射/逆映射一个角度
	scRadian  MapAngle(const scRadian& ang) const;
	scRadian  InvMapAngle(const scRadian& ang) const;

	//映射/逆映射一个面积
	double    MapArea(double area) const;
	double    InvMapArea(double area) const;

	//逻辑操作符
	bool operator == (const sc2XformLinear& xform) const { return (m_VectorT == xform.m_VectorT) && (m_MatrixC == xform.m_MatrixC); }
	bool operator != (const sc2XformLinear& xform) const { return !(*this == xform); }

	//单位变换
	static const sc2XformLinear I;

	//判断是否为单位变换
	bool IsIdentity() const { return m_VectorT.IsNull() && m_MatrixC.IsIdentity(); }

	//判断该变换是否可逆
	bool IsSingular() const { return m_MatrixC.IsSingular(); }

	// ------重载基类的虚函数------ //

	//返回当前变换是否线性变换
	virtual bool IsLinear() const;

	//返回当前变换在给定点的最优线性变换
	virtual sc2XformLinear LinearXform(const sc2Vector& atThisPt) const;

	//映射一个二维坐标点
	virtual sc2Vector MapPoint(const sc2Vector& pt) const;

	//从左到右组合两个变换
	virtual sc2XformBasePtrh ComposeBase(const sc2XformBase& rhs) const;

	//逆变换
	virtual sc2XformBasePtrh InverseBase() const;

	//拷贝到一个新分配的对象
	virtual sc2XformBasePtrh Clone() const;

private:
	sc2Vector m_VectorT;
	sc2Matrix m_MatrixC;
};

inline HStatus sc2XformLinear::Inverse(sc2XformLinear& xform) const
{
	sc2Matrix ic;
	HStatus status = m_MatrixC.Inverse(ic);

	assert(IsSuccess(status));

	if (IsSuccess(status))
	{
		xform.SetMatrix(ic);
		xform.SetTrans(ic * -m_VectorT);
	}

	return status;
}

inline sc2Vector sc2XformLinear::InvMapPoint(const sc2Vector& pt) const
{
	sc2XformLinear ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	return (ixform * pt);
}

inline sc2Vector sc2XformLinear::MapVector(const sc2Vector& vect) const
{
	return (m_MatrixC * vect);
}

inline sc2Vector sc2XformLinear::InvMapVector(const sc2Vector& vect) const
{
	sc2Matrix ic;
	HStatus status = m_MatrixC.Inverse(ic);

	assert(IsSuccess(status));

	return (ic * vect);
}

inline scRadian  sc2XformLinear::MapAngle(const scRadian& ang) const
{
	sc2Vector vect = MapVector(sc2Vector(1,ang));
	return arctan(vect.GetY(), vect.GetX());
}

inline scRadian  sc2XformLinear::InvMapAngle(const scRadian& ang) const
{
	sc2XformLinear ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	return ixform.MapAngle(ang);
}

inline double    sc2XformLinear::MapArea(double area) const
{
	return (area * fabs(m_MatrixC.Determinant()));
}

inline double    sc2XformLinear::InvMapArea(double area) const
{
	assert(IsSingular());

	return (area / fabs(m_MatrixC.Determinant()));
}

// ------重载基类的虚函数------ //

inline bool sc2XformLinear::IsLinear() const
{
	return true;
}

inline sc2XformLinear sc2XformLinear::LinearXform(const sc2Vector& atThisPt) const
{
	return *this;
}

inline sc2Vector sc2XformLinear::MapPoint(const sc2Vector& pt) const
{
	return (m_MatrixC * pt) + m_VectorT;
}

inline sc2XformBasePtrh sc2XformLinear::ComposeBase(const sc2XformBase& rhs) const
{
	sc2XformBasePtrh pResult = NULL;

	if (rhs.IsLinear())
	{
		sc2XformLinear xfm = rhs.LinearXform(sc2Vector());

		pResult = new sc2XformLinear(Compose(xfm));
	}
	else
	{
		pResult = new sc2XformComposed(*this, rhs);
	}

	return pResult;
}

inline sc2XformBasePtrh sc2XformLinear::InverseBase() const
{	
	sc2XformLinear ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	sc2XformBasePtrh pResult = new sc2XformLinear(ixform);

	return pResult;
}

inline sc2XformBasePtrh sc2XformLinear::Clone() const
{
	sc2XformBasePtrh pResult = new sc2XformLinear(*this);

	return pResult;
}

inline bool SVCORE_API sfRealEqual(const sc2XformLinear& x1, const sc2XformLinear& x2, double epsilon = 1e-15)
{
	return (sfRealEqual(x1.GetTrans(),  x2.GetTrans(),  epsilon) && sfRealEqual(x1.GetMatrix(), x2.GetMatrix(), epsilon));
}

#pragma pack(pop)
