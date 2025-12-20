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
//                            二维刚体变换sc2XformRigid
// ---------------------------------------------------------------------
// 二维刚体变换由旋转角度angle、平移向量t组成。

class SVCORE_API sc2XformRigid : public sc2XformBase
{
public:
	sc2XformRigid() : m_VectorT(sc2Vector()), m_Angle(scRadian(0)), m_SinAngle(0), m_CosAngle(1) {}

	sc2XformRigid(const scRadian& a, const sc2Vector& t) : m_VectorT(t), m_Angle(a), m_SinAngle(sin(a)), m_CosAngle(cos(a)) {}

	//设置/获取角度、平移量
	const scRadian& GetAngle() const { return m_Angle; }
	const sc2Vector& GetTrans() const { return m_VectorT; }
	void SetAngle(const scRadian& a) { m_Angle = a; m_SinAngle = sin(a); m_CosAngle= cos(a); }
	void SetTrans(const sc2Vector& t) { m_VectorT = t; }

	//获取当前的正弦、余弦分量
	double GetCosAngle() const { return m_CosAngle; }
	double GetSinAngle() const { return m_SinAngle; }

	//获取相对应的线性变换
	sc2XformLinear Linear() const { return (sc2XformLinear(sc2Matrix(m_CosAngle,-m_SinAngle,m_SinAngle,m_CosAngle), m_VectorT)); }

	//逆变换
	sc2XformRigid Inverse() const;

	//从左到右组合两个变换
	sc2XformRigid operator * (const sc2XformRigid& xform) const;
	sc2XformRigid Compose (const sc2XformRigid& xform) const {return *this * xform;}

	//从左到右组合当前变换与一个线性变换
	sc2XformLinear operator * (const sc2XformLinear& xform) const;
	sc2XformLinear Compose (const sc2XformLinear& xform) const {return *this * xform;}

	//从左到右组合两个变换，返回变换后的结果的基类指针
	sc2XformBasePtrh operator*(const sc2XformBase& rhs) const { return sc2XformBase::operator*(rhs); }

	//映射一个二维坐标点
	sc2Vector operator*(const sc2Vector& pt) const { return sc2XformBase::operator*(pt); }

	//逆映射一个二维坐标点
	sc2Vector InvMapPoint(const sc2Vector& pt) const;

	//映射/逆映射一个二维向量（长度和方向）
	sc2Vector MapVector(const sc2Vector& vect) const;
	sc2Vector InvMapVector(const sc2Vector& vect) const;

	//映射/逆映射一个角度
	scRadian  MapAngle(const scRadian& ang) const;
	scRadian  InvMapAngle(const scRadian& ang) const;

	//逻辑操作符
	bool operator == (const sc2XformRigid& xform) const { return ( (m_VectorT == xform.m_VectorT) && (m_SinAngle == xform.m_SinAngle) && (m_CosAngle == xform.m_CosAngle) ); }
	bool operator != (const sc2XformRigid& xform) const { return !(*this == xform); }

	//单位变换
	static const sc2XformRigid I;

	//判断是否为单位变换
	bool IsIdentity() const { return m_VectorT.IsNull() && m_SinAngle == 0 && m_CosAngle == 1; }

	//操作线性变换*刚体变换
	friend SVCORE_API sc2XformLinear operator* (const sc2XformLinear &x, const sc2XformRigid &r);

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
	scRadian m_Angle;
	double m_SinAngle;
	double m_CosAngle;
};

inline sc2XformRigid sc2XformRigid::Inverse() const
{
	return (sc2XformRigid(-m_Angle, sc2Vector(-m_CosAngle*m_VectorT.GetX() - m_SinAngle*m_VectorT.GetY(), m_SinAngle*m_VectorT.GetX() - m_CosAngle*m_VectorT.GetY())));

}

inline sc2XformRigid sc2XformRigid::operator * (const sc2XformRigid& xform) const
{
	return sc2XformRigid (m_Angle+xform.m_Angle,
		sc2Vector(m_CosAngle*xform.m_VectorT.GetX() - m_SinAngle*xform.m_VectorT.GetY(), m_SinAngle*xform.m_VectorT.GetX() + m_CosAngle*xform.m_VectorT.GetY()) + m_VectorT);
}

inline sc2XformLinear sc2XformRigid::operator * (const sc2XformLinear& xform) const
{
	return (Linear() * xform);
}

inline sc2Vector sc2XformRigid::InvMapPoint(const sc2Vector& pt) const
{
	return (Inverse() * pt);
}

inline sc2Vector sc2XformRigid::MapVector(const sc2Vector& vect) const
{
	return (sc2Vector(m_CosAngle*vect.GetX()-m_SinAngle*vect.GetY(),m_SinAngle*vect.GetX()+m_CosAngle*vect.GetY()));
}

inline sc2Vector sc2XformRigid::InvMapVector(const sc2Vector& vect) const
{
	return (sc2Vector(m_CosAngle*vect.GetX()+m_SinAngle*vect.GetY(),-m_SinAngle*vect.GetX()+m_CosAngle*vect.GetY()));
}

inline scRadian  sc2XformRigid::MapAngle(const scRadian& ang) const
{
	return ang + GetAngle();
}

inline scRadian  sc2XformRigid::InvMapAngle(const scRadian& ang) const
{
	return ang - GetAngle();
}

inline sc2XformLinear operator* (const sc2XformLinear &x, const sc2XformRigid &r)
{
	return (x * r.Linear());
}

// ------重载基类的虚函数------ //

inline bool sc2XformRigid::IsLinear() const
{
	return true;
}

inline sc2XformLinear sc2XformRigid::LinearXform(const sc2Vector& atThisPt) const
{
	return Linear();
}

inline sc2Vector sc2XformRigid::MapPoint(const sc2Vector& pt) const
{
	return (MapVector(pt) + m_VectorT);
}

inline sc2XformBasePtrh sc2XformRigid::ComposeBase(const sc2XformBase& rhs) const
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

inline sc2XformBasePtrh sc2XformRigid::InverseBase() const
{	
	sc2XformBasePtrh pResult = new sc2XformRigid(Inverse());

	return pResult;
}

inline sc2XformBasePtrh sc2XformRigid::Clone() const
{	
	sc2XformBasePtrh pResult = new sc2XformRigid(*this);

	return pResult;
}

inline bool SVCORE_API sfRealEqual(const sc2XformRigid& x1, const sc2XformRigid& x2, double epsilon = 1e-15)
{
	return (sfRealEqual(x1.GetTrans(), x2.GetTrans(), epsilon) &&
		  sfRealEqual(x1.GetCosAngle(),x2.GetCosAngle(),epsilon) &&
		  sfRealEqual(x1.GetSinAngle(),x2.GetSinAngle(),epsilon));
}

#pragma pack(pop)
