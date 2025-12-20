#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            一维线性变换sc1Xform
// ---------------------------------------------------------------------
// 一维线性变换由缩放scale、平移offset组成。

class SVCORE_API sc1Xform
{
public:
	sc1Xform() : m_dScale(1), m_dOffset(0) {}

	sc1Xform(double scale, double offset) : m_dScale(scale), m_dOffset(offset) {}

	//获取/设置scale，offset
	double	GetScale () const { return m_dScale; }
	double	GetOffset() const { return m_dOffset; }
	void	SetScale (double scale)  { m_dScale  = scale; }
	void	SetOffset(double offset) { m_dOffset = offset; }

	//逆变换
	//如果当前变换奇异，则返回HSingular；否则，返回HSuccess
	HStatus Inverse(sc1Xform& xform) const;

	//从左到右组合两个变换
	sc1Xform operator * (const sc1Xform& xform) const;
	sc1Xform Compose    (const sc1Xform& xform) const { return (*this * xform); }

	//映射/逆映射一个一维坐标点
	double operator * (double pt) const { return (m_dScale * pt) + m_dOffset; }
	double MapPoint   (double pt) const { return (*this * pt); }
	double InvMapPoint (double pt) const;

	//映射/逆映射一个一维向量（长度）
	double MapVector (double vect) const { return (m_dScale * vect); }
	double InvMapVector (double vect) const;

	bool operator == (const sc1Xform&) const;
	bool operator != (const sc1Xform&) const;

	//单位变换
	static const sc1Xform I;

	//返回是否单位变换
	bool IsIdentity() const { return ((m_dScale == 1) && (m_dOffset == 0)); }

	//返回是否奇异
	bool IsSingular() const {return m_dScale == 0;}

private:
	double m_dScale;
	double m_dOffset;
};

inline HStatus sc1Xform::Inverse(sc1Xform& xform) const
{
	if (IsSingular())
	{
		return HSingular;
	}

	double inverseScale = 1/m_dScale;
	xform =  sc1Xform(inverseScale, inverseScale * -m_dOffset);

	return HSuccess;
}

inline sc1Xform sc1Xform::operator * (const sc1Xform& xform) const
{
	return sc1Xform(m_dScale * xform.m_dScale, m_dScale * xform.m_dOffset + m_dOffset);
}

inline double sc1Xform::InvMapPoint (double pt) const
{
	sc1Xform ixform;
	HStatus status = Inverse(ixform);

	assert(IsSuccess(status));

	return (ixform * pt);
}

inline double sc1Xform::InvMapVector (double vect) const
{
	assert(!IsSingular());

	return (vect / m_dScale);
}

inline bool sc1Xform::operator == (const sc1Xform& xform) const
{
	return ( (m_dScale == xform.m_dScale) && (m_dOffset == xform.m_dOffset) );
}

inline bool sc1Xform::operator != (const sc1Xform& xform) const
{
	return ( (m_dScale != xform.m_dScale) || (m_dOffset != xform.m_dOffset) );
}

inline bool SVCORE_API sfRealEqual(const sc1Xform& x1, const sc1Xform& x2, double epsilon = 1e-15)
{
	return sfRealEqual(x1.GetScale(),  x2.GetScale(),  epsilon) && sfRealEqual(x1.GetOffset(), x2.GetOffset(), epsilon);
}

#pragma pack(pop)
