#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svVector.h"
#include "svMatrix.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            三维线性变换sc3Xform
// ---------------------------------------------------------------------
// 三维线性变换由矩阵c、平移向量t组成。

class SVCORE_API sc3Xform
{
public:
	sc3Xform() : m_VectorT(sc3Vector()), m_MatrixC(sc3Matrix()) {}

	sc3Xform(const sc3Matrix& c, const sc3Vector& t) :  m_VectorT(t), m_MatrixC(c) {}

	//获取/设置matrix，trans
	const sc3Matrix& GetMatrix() const { return m_MatrixC; }
	const sc3Vector& GetTrans() const  { return m_VectorT; }
	void SetMatrix(const sc3Matrix& c) { m_MatrixC = c; }
	void SetTrans(const sc3Vector& t)  { m_VectorT = t; }

	//逆变换
	//如果当前变换奇异，则返回HSingular；否则，返回HSuccess
	HStatus Inverse(sc3Xform& xform) const;

	//从左到右组合两个变换
	sc3Xform operator * (const sc3Xform& xform) const;
	sc3Xform compose (const sc3Xform& xform) const { return *this * xform; }

	//获取X轴缩放因子、Y轴缩放因子、Z轴缩放因子
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetXScale(double& xScale) const { return m_MatrixC.GetXScale(xScale); }
	HStatus GetYScale(double& yScale) const { return m_MatrixC.GetYScale(yScale); }
	HStatus GetZScale(double& zScale) const { return m_MatrixC.GetZScale(zScale); }

	//映射一个三维坐标点
	//result = c * pt + t
	sc3Vector   operator * (const sc3Vector &pt) const;
	sc3Vector   mapPoint (const sc3Vector &pt) const;

	//逆映射一个三维坐标点
	//result = c^-1 * (pt - t)
	sc3Vector   invMapPoint (const sc3Vector &pt) const;

	//映射一个三维向量（长度和方向）
	//result = c * vect
	sc3Vector mapVector (const sc3Vector &vect) const;

	//逆映射一个三维向量（长度和方向）
	//result = c^-1 * vect
	sc3Vector invMapVector (const sc3Vector &vect) const;

	bool operator == (const sc3Xform&) const;
	bool operator != (const sc3Xform&) const;

	//单位变换
	static const sc3Xform I;

	//返回是否单位变换
	bool IsIdentity() const { return m_VectorT.IsNull() && m_MatrixC.IsIdentity(); }

	//返回是否奇异
	bool IsSingular() const {return m_MatrixC.IsSingular();}

private:
	sc3Vector m_VectorT;
	sc3Matrix m_MatrixC;
};

inline HStatus sc3Xform::Inverse(sc3Xform& xform) const
{
	sc3Matrix ic;
	HStatus status = m_MatrixC.Inverse(ic);

	assert(IsSuccess(status));

	if (IsSuccess(status))
	{
		xform.SetMatrix(ic);
		xform.SetTrans(ic * -m_VectorT);
	}

	return status;
}

inline sc3Xform sc3Xform::operator*(const sc3Xform& xform) const
{
	return sc3Xform(m_MatrixC * xform.m_MatrixC, m_MatrixC * xform.m_VectorT + m_VectorT);
}

inline sc3Vector sc3Xform::operator * (const sc3Vector &pt) const
{
	return (m_MatrixC * pt) + m_VectorT;
}

inline sc3Vector sc3Xform::mapPoint (const sc3Vector &pt) const
{
	return *this * pt;
}

inline sc3Vector sc3Xform::invMapPoint (const sc3Vector &pt) const
{
	sc3Xform ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	return (ixform * pt);
}

inline sc3Vector sc3Xform::mapVector (const sc3Vector &vect) const
{
	return (m_MatrixC * vect);
}

inline sc3Vector sc3Xform::invMapVector (const sc3Vector &vect) const
{
	sc3Xform ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	return (ixform * vect);
}

inline bool sc3Xform::operator==(const sc3Xform& xform) const
{
	return ( (m_MatrixC == xform.m_MatrixC) && (m_VectorT == xform.m_VectorT) );
}

inline bool sc3Xform::operator != (const sc3Xform& xform) const
{
	return !(*this == xform);
}

inline bool SVCORE_API sfRealEqual(const sc3Xform& x1, const sc3Xform& x2, double epsilon = 1e-15)
{
	return (sfRealEqual(x1.GetTrans(), x2.GetTrans(), epsilon) && sfRealEqual(x1.GetMatrix(), x2.GetMatrix(), epsilon));
}

#pragma pack(pop)
