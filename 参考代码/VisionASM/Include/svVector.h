#pragma once

#include "svDef.h"
#include "svError.h"
#include "svMath.h"
#include "svAngle.h"
#include "svPair.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            二维向量sc2Vector
// ---------------------------------------------------------------------


class SVCORE_API sc2Vector
{
public:
	sc2Vector ()						{m_dX[0] = 0.;         m_dX[1] = 0.; }
	sc2Vector (double x, double y)		{m_dX[0] = x ;         m_dX[1] = y; }
	sc2Vector (double r, scRadian t)	{m_dX[0] = r * cos(t); m_dX[1] = r * sin(t);}
	sc2Vector (const scFPair& p)		{m_dX[0] = p.x();      m_dX[1] = p.y();}
	sc2Vector (const scDPair& p)		{m_dX[0] = p.x();      m_dX[1] = p.y();}
	sc2Vector (const scIPair& p)		{m_dX[0] = p.x();      m_dX[1] = p.y();}
	sc2Vector (const scSPair& p)		{m_dX[0] = p.x();      m_dX[1] = p.y();}
	class NoInit {};
	sc2Vector(const NoInit&) {}

	double	GetX () const		{ return m_dX[0]; }
	double	GetY () const		{ return m_dX[1]; }
	void	SetX (double newX)	{ m_dX[0] = newX; }
	void	SetY (double newY)	{ m_dX[1] = newY; }

	//向量长度
	double	Length () const		{ return sqrt(sfSquare(m_dX[0]) + sfSquare(m_dX[1])); }

	//判断向量是否为零向量
	bool	IsNull() const		{ return m_dX[0] == 0. && m_dX[1] == 0.; }

	//根据下标访问向量元素
	double& operator [] (int d)       { assert(d >= 0 && d < 2); return m_dX[d];}
	double  operator [] (int d) const { assert(d >= 0 && d < 2); return m_dX[d];}

	//向量角度，范围（-PI，PI]
	//如果当前向量为零向量，返回HNullVector；否则，返回HSuccess
	HStatus	Angle (scRadian& r) const;

	//平行于当前向量的单位向量
	//如果当前向量为零向量，返回HNullVector；否则，返回HSuccess
	HStatus	Unit (sc2Vector& vectorUnit) const;

	//向量vectorToBeProjected在当前向量上的投影
	//如果当前向量为零向量，返回HNullVector；否则，返回HSuccess
	HStatus Project (const sc2Vector& vectorToBeProjected, sc2Vector& vectorResult) const;

	//平行于当前向量的向量（长度与当前向量相等）
	//零向量垂直于自己
	sc2Vector Perpendicular() const { return sc2Vector(-m_dX[1], m_dX[0]); }

	//两个向量的距离（两个向量的差的长度）
	double Distance(const sc2Vector& v) const { return (*this - v).Length(); }

	sc2Vector operator+(const sc2Vector&) const;
	sc2Vector& operator+=(const sc2Vector&);

	sc2Vector operator-() const;
	sc2Vector operator-(const sc2Vector&) const;
	sc2Vector& operator-=(const sc2Vector&);

	sc2Vector  operator*(double d) const;
	friend sc2Vector operator*(double, const sc2Vector&);
	sc2Vector& operator*=(double);

	double operator*(const sc2Vector&) const;	// 点乘
	double Dot(const sc2Vector&) const;			// 点乘
	double Cross(const sc2Vector&) const;		// 叉乘（二维向量的叉乘是一个标量）

	sc2Vector  operator/(double d) const;
	sc2Vector& operator/=(double);

	bool operator==(const sc2Vector&) const;
	bool operator!=(const sc2Vector&) const;

private:
	double m_dX[2];
};

//VisionWare内部使用
class sc2VectorX : public sc2Vector
{
public:
  sc2VectorX() : sc2Vector(sc2Vector::NoInit()) {}
  sc2Vector& operator=(const sc2Vector& rhs) {return sc2Vector::operator=(rhs);}
};

inline sc2Vector sc2Vector::operator+(const sc2Vector& v) const
{ 
	return sc2Vector ( m_dX[0] + v.m_dX[0], m_dX[1] + v.m_dX[1]);
}

inline sc2Vector& sc2Vector::operator+=(const sc2Vector& v)
{
	m_dX[0] += v.m_dX[0];
	m_dX[1] += v.m_dX[1];
	return *this;
}

inline sc2Vector sc2Vector::operator- () const
{ 
	return sc2Vector(-m_dX[0], -m_dX[1]); 
}

inline sc2Vector sc2Vector::operator-(const sc2Vector& v) const
{ 
	return sc2Vector( m_dX[0] - v.m_dX[0], m_dX[1] - v.m_dX[1]); 
}

inline sc2Vector& sc2Vector::operator-=(const sc2Vector& v)
{
	m_dX[0] -= v.m_dX[0];
	m_dX[1] -= v.m_dX[1];
	return *this;
}

inline sc2Vector sc2Vector::operator* (double d) const
{ 
	return sc2Vector(m_dX[0] * d, m_dX[1] * d); 
}

inline sc2Vector operator*(double d, const sc2Vector& v)
{ 
	return v * d; 
}

inline sc2Vector& sc2Vector::operator*=(double d)
{
	m_dX[0] *= d;
	m_dX[1] *= d;
	return *this;
}

inline double sc2Vector::operator*(const sc2Vector& v) const
{
	return (m_dX[0] * v.m_dX[0] + m_dX[1] * v.m_dX[1]);
}

inline double sc2Vector::Dot(const sc2Vector& v) const
{ 
	return *this * v; 
}

inline double sc2Vector::Cross(const sc2Vector& v) const
{ 
	return m_dX[0] * v.m_dX[1] - m_dX[1] * v.m_dX[0]; 
}

inline sc2Vector sc2Vector::operator/ (double d) const
{ 
	assert(d != 0.0); 
	return *this * (1. / d); 
}

inline sc2Vector& sc2Vector::operator/=(double d)
{ 
	return *this = *this / d;
}

inline bool sc2Vector::operator==(const sc2Vector& v) const
{ 
	return ((m_dX[0] == v.m_dX[0]) && (m_dX[1] == v.m_dX[1]));
}

inline bool sc2Vector::operator !=(const sc2Vector& v) const
{ 
	return !(*this == v); 
}

bool SVCORE_API sfRealEqual(const sc2Vector& v1, const sc2Vector& v2, double epsilon = 1.e-15);

// ---------------------------------------------------------------------
//                            三维向量sc3Vector
// ---------------------------------------------------------------------

class SVCORE_API sc3Vector
{
public:
	sc3Vector ()								{m_dX[0] = 0.; m_dX[1] = 0.; m_dX[2] = 0.; }
	sc3Vector (double x, double y, double z)	{m_dX[0] = x; m_dX[1] = y; m_dX[2] = z;}

	double GetX () const      { return m_dX[0]; }
	double GetY () const      { return m_dX[1]; }
	double GetZ () const      { return m_dX[2]; }
	void   SetX (double newX) { m_dX[0] = newX; }
	void   SetY (double newY) { m_dX[1] = newY; }
	void   SetZ (double newZ) { m_dX[2] = newZ; }

	//向量长度
	double Length () const { return sqrt(sfSquare(m_dX[0]) + sfSquare(m_dX[1]) + sfSquare(m_dX[2])); }

	//判断向量是否为零向量
	bool IsNull() const { return m_dX[0] == 0. && m_dX[1] == 0. && m_dX[2] == 0.; }

	//根据下标访问向量元素
	double& operator [] (int d)       { assert(d >= 0 && d < 3); return m_dX[d];}
	double  operator [] (int d) const { assert(d >= 0 && d < 3); return m_dX[d];}

	//平行于当前向量的单位向量
	//如果当前向量为零向量，返回HNullVector；否则，返回HSuccess
	HStatus Unit(sc3Vector& vectorUnit) const;

	//向量vectorToBeProjected在当前向量上的投影
	//如果当前向量为零向量，返回HNullVector；否则，返回HSuccess
	HStatus Project (const sc3Vector& vectorToBeProjected, sc3Vector& vectorResult) const;

	//两个向量的距离（两个向量的差的长度）
	double Distance(const sc3Vector& v) const { return (*this - v).Length(); }

	sc3Vector operator+(const sc3Vector&) const;
	sc3Vector& operator+=(const sc3Vector&);

	sc3Vector operator-() const;
	sc3Vector operator-(const sc3Vector&) const;
	sc3Vector& operator-=(const sc3Vector&);

	sc3Vector  operator* (double d) const;
	friend sc3Vector operator*(double, const sc3Vector&);
	sc3Vector& operator*=(double);

	double operator*(const sc3Vector&) const;	// 点乘
	double Dot(const sc3Vector&) const;			// 点乘
	sc3Vector Cross(const sc3Vector&) const;	// 叉乘

	sc3Vector  operator/(double d) const;
	sc3Vector& operator/=(double);

	bool operator==(const sc3Vector&) const;
	bool operator!=(const sc3Vector&) const;

private:
	double m_dX[3];
};

inline sc3Vector sc3Vector::operator+(const sc3Vector& v) const
{ 
	return sc3Vector ( m_dX[0] + v.m_dX[0], m_dX[1] + v.m_dX[1], m_dX[2] + v.m_dX[2]); 
}

inline sc3Vector& sc3Vector::operator+=(const sc3Vector& v)
{ 
	return *this = *this + v;
}

inline sc3Vector sc3Vector::operator- () const
{ 
	return sc3Vector(-m_dX[0], -m_dX[1], -m_dX[2]);
}

inline sc3Vector sc3Vector::operator-(const sc3Vector& v) const
{
	return sc3Vector( m_dX[0] - v.m_dX[0], m_dX[1] - v.m_dX[1], m_dX[2] - v.m_dX[2]);
}

inline sc3Vector& sc3Vector::operator-=(const sc3Vector& v)
{ 
	return *this = *this - v; 
}

inline sc3Vector sc3Vector::operator*(double d) const
{
	return sc3Vector(m_dX[0] * d, m_dX[1] * d, m_dX[2] * d); 
}

inline sc3Vector operator*(double d, const sc3Vector& v)
{ 
	return v * d; 
}

inline sc3Vector& sc3Vector::operator *= (double d)
{
	return *this = *this * d; 
}

inline double sc3Vector::operator*(const sc3Vector& v) const
{ 
	return (m_dX[0] * v.m_dX[0] + m_dX[1] * v.m_dX[1] + m_dX[2] * v.m_dX[2]); 
}

inline double sc3Vector::Dot(const sc3Vector& v) const
{ 
	return *this * v;
}

inline sc3Vector sc3Vector::Cross(const sc3Vector& v) const
{
	double x = m_dX[1]*v.m_dX[2] - m_dX[2]*v.m_dX[1];
	double y = m_dX[2]*v.m_dX[0] - m_dX[0]*v.m_dX[2];
	double z = m_dX[0]*v.m_dX[1] - m_dX[1]*v.m_dX[0];
	return sc3Vector(x, y, z);
}

inline sc3Vector sc3Vector::operator/ (double d) const
{ 
	assert(d != 0.0); 
	return *this * (1. / d); 
}

inline sc3Vector& sc3Vector::operator/=(double d)
{ 
	return *this = *this / d;
}

inline bool sc3Vector::operator==(const sc3Vector& v) const
{ 
	return ((m_dX[0] == v.m_dX[0]) && (m_dX[1] == v.m_dX[1]) && (m_dX[2] == v.m_dX[2])); 
}

inline bool sc3Vector::operator!= (const sc3Vector& v) const
{ 
	return !(*this == v); 
}

bool SVCORE_API sfRealEqual(const sc3Vector& v1, const sc3Vector& v2, double epsilon = 1.e-15);

#pragma pack(pop)
