#pragma once

#include "svDef.h"
#include "svError.h"
#include "svAngle.h"
#include "svVector.h"
#include <cassert>

#pragma pack(push,8)

// ---------------------------------------------------------------------
//                            二维矩阵sc2Matrix
// ---------------------------------------------------------------------
// 一个二维矩阵由4个自由度组成，一般有如下两种的表示方法：
// 1）xRot, yRot, xScale, yScale		(Rx, Ry, Sx, Sy)
// 2）scale, aspect, shear, rotation	(s, a, K, R)
// 
// 一个二维矩阵可以表示如下：
// |e11 e12|   |Sx(cosRx)  -Sy(sinRy)|   |s(cosR)  as(-sinR - cosR tanK)|
// |e21 e22| = |Sx(sinRx)   Sy(cosRy)| = |s(sinR)  as( cosR - sinR tanK)|
// 
// 也可以分解的表示如下：
// |Sx(cosRx)  -Sy(sinRy)|   |cosRx  -sinRy| |Sx  0 |
// |Sx(sinRx)   Sy(cosRy)| = |sinRx   cosRy| |0   Sy|
// 或者
// |s(cosR)  as(-sinR - cosR tanK)|       |cosR  -sinR| |1  -tanK| |1  0|
// |s(sinR)  as( cosR - sinR tanK)| = s * |sinR   cosR| |0    1  | |0  a|
// 
// 根据矩阵的元素求解相应自由度的过程如下：
// Rx = atan2(e21, e11)           s = sqrt(e11*e11 + e21*e21)
// Ry = atan2(-e12, e22)          a = det / (e11*e11 + e21*e21)
// Sx = sqrt(e11*e11 + e21*e21)   K = -atan((e11*e12 + e21*e22) / det)
// Sy = sqrt(e12*e12 + e22*e22)   R = atan2(e21, e11)
// 其中det为矩阵的行列式值，det = a * (s^2)。
// 
// 根据如上方式计算得到的自由度的取值范围如下：
// Rx [-PI, PI]                     s (0, +Inf)
// Ry [-PI, PI]                     a (-Inf, +Inf), a != 0
// Sx (0, +Inf)                     R [-PI, PI]
// Sy (0, +Inf)                     K (-PI/2, PI/2)

class SVCORE_API sc2Matrix
{
public:
	sc2Matrix() 
	{
		m_dE[0][0]	= 1.;
		m_dE[0][1]	= 0.;
		m_dE[1][0]	= 0.;
		m_dE[1][1]	= 1.;
		m_dDT		= 1.;
	}

	sc2Matrix(double e11, double e12, double e21, double e22)
	{
		m_dE[0][0] = e11;
		m_dE[0][1] = e12;
		m_dE[1][0] = e21;
		m_dE[1][1] = e22;
		SetDT();
	}

	//X轴旋转、Y轴旋转、X轴缩放因子、Y轴缩放因子
	sc2Matrix(const scRadian& xRot, const scRadian& yRot, double xScale, double yScale);

	//缩放因子、纵横比例、切变、旋转
	sc2Matrix(double scale, double aspect, const scRadian& shear, const scRadian& rotation);

	//返回矩阵的行列式值
	double Determinant() const { return m_dDT;}

	//矩阵的逆
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus Inverse(sc2Matrix& m) const;

	//返回矩阵的转置矩阵
	sc2Matrix Transpose() const;

	//判断矩阵是否奇异
	bool IsSingular() const;

	//判断矩阵是否为单位矩阵
	bool IsIdentity() const;

	//获取/设置矩阵元素的值
	//设置矩阵元素的值会改变当前矩阵的行列式
	double GetElement(int row,int column) const;
	void   SetElement(int row,int column, double value);

	//获取X轴旋转、Y轴旋转、X轴缩放因子、Y轴缩放因子
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetXRot(scRadian& xRot) const;
	HStatus GetYRot(scRadian& yRot) const;
	HStatus GetXScale(double& xScale) const;
	HStatus GetYScale(double& yScale) const;

	//获取缩放因子、纵横比例、切变、旋转
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetScale(double& scale) const;
	HStatus GetAspect(double& aspect) const;
	HStatus GetShear(scRadian& shear) const;
	HStatus GetRotation(scRadian& rotation) const;

	//单位矩阵
	static const sc2Matrix I;

	sc2Matrix  operator+(const sc2Matrix&) const;
	sc2Matrix& operator+=(const sc2Matrix&);

	sc2Matrix  operator-() const;
	sc2Matrix  operator-(const sc2Matrix&) const;
	sc2Matrix& operator-=(const sc2Matrix&);

	sc2Matrix  operator*(double) const;
	sc2Matrix& operator*=(double);

	sc2Matrix  operator*(const sc2Matrix&) const;
	sc2Matrix& operator*=(const sc2Matrix&);

	sc2Matrix  operator/(double) const;
	sc2Matrix& operator/=(double);

	sc2Matrix  operator/(const sc2Matrix&) const;
	sc2Matrix& operator/=(const sc2Matrix&);

	bool operator==(const sc2Matrix&) const;
	bool operator!=(const sc2Matrix&) const;

	//向量操作
	sc2Vector operator*(const sc2Vector&) const;
	friend SVCORE_API sc2Vector operator*(const sc2Vector&, const sc2Matrix&);

private:
	double m_dE[2][2];		// 矩阵元素
	double m_dDT;			// 矩阵的行列式值
	void SetDT() { m_dDT = m_dE[0][0] * m_dE[1][1] - m_dE[0][1] * m_dE[1][0]; }
};

inline HStatus sc2Matrix::Inverse(sc2Matrix& m) const
{
	if (IsSingular())
	{
		return HSingular;
	}

	double d1_DT  =   1.0 / m_dDT;
	m.m_dE[0][0] =   m_dE[1][1] * d1_DT;
	m.m_dE[0][1] = - m_dE[0][1] * d1_DT;
	m.m_dE[1][0] = - m_dE[1][0] * d1_DT;
	m.m_dE[1][1] =   m_dE[0][0] * d1_DT;

	return HSuccess;
}

inline sc2Matrix sc2Matrix::Transpose() const
{
	return sc2Matrix(m_dE[0][0], m_dE[1][0], m_dE[0][1], m_dE[1][1]);
}

inline bool sc2Matrix::IsSingular() const
{
	return ( m_dDT == 0. );
}

inline bool sc2Matrix::IsIdentity() const
{
	return ( m_dE[0][0] == 1. && m_dE[0][1] == 0. && m_dE[1][0] == 0. && m_dE[1][1] == 1. );
}

inline double sc2Matrix::GetElement(int row,int column) const
{
	assert((row == 0 || row == 1) && (column == 0 || column == 1));
	return m_dE[row][column];
}

inline void   sc2Matrix::SetElement(int row,int column, double value)
{
	assert((row == 0 || row == 1) && (column == 0 || column == 1));
	m_dE[row][column] = value;
	SetDT();
}

inline sc2Matrix sc2Matrix::operator + (const sc2Matrix& m) const
{
	return sc2Matrix(m_dE[0][0] + m.m_dE[0][0], m_dE[0][1] + m.m_dE[0][1], m_dE[1][0] + m.m_dE[1][0], m_dE[1][1] + m.m_dE[1][1]);
}

inline sc2Matrix& sc2Matrix::operator+=(const sc2Matrix& m)
{
	return *this = *this + m;
}

inline sc2Matrix sc2Matrix::operator-() const
{
	return sc2Matrix(-m_dE[0][0], -m_dE[0][1], -m_dE[1][0], -m_dE[1][1]);
}

inline sc2Matrix sc2Matrix::operator - (const sc2Matrix& m) const
{
	return sc2Matrix(m_dE[0][0] - m.m_dE[0][0], m_dE[0][1] - m.m_dE[0][1], m_dE[1][0] - m.m_dE[1][0], m_dE[1][1] - m.m_dE[1][1]);
}

inline sc2Matrix& sc2Matrix::operator-=(const sc2Matrix& m)
{
	return *this = *this - m;
}

inline sc2Matrix sc2Matrix::operator * (double s) const
{
	return sc2Matrix(m_dE[0][0] * s, m_dE[0][1] * s, m_dE[1][0] * s, m_dE[1][1] * s);
}

inline sc2Matrix& sc2Matrix::operator*=(double s)
{
	return *this = *this * s;
}

inline sc2Matrix sc2Matrix::operator * (const sc2Matrix& m) const
{
	return sc2Matrix(m_dE[0][0]*m.m_dE[0][0] + m_dE[0][1]*m.m_dE[1][0],
					 m_dE[0][0]*m.m_dE[0][1] + m_dE[0][1]*m.m_dE[1][1],
					 m_dE[1][0]*m.m_dE[0][0] + m_dE[1][1]*m.m_dE[1][0],
					 m_dE[1][0]*m.m_dE[0][1] + m_dE[1][1]*m.m_dE[1][1]);
}

inline sc2Matrix& sc2Matrix::operator*=(const sc2Matrix& m)
{
	return *this = *this * m;
}

inline sc2Matrix sc2Matrix::operator/(double s) const
{
	assert(s != 0);
	return *this * (1. / s);
}

inline sc2Matrix& sc2Matrix::operator/=(double s)
{
	return *this = *this / s;
}

inline sc2Matrix sc2Matrix::operator/(const sc2Matrix& m) const
{
	sc2Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return (*this) * mInverse;
}

inline sc2Matrix& sc2Matrix::operator/=(const sc2Matrix& m)
{
	return *this = *this / m;
}

inline bool sc2Matrix::operator == (const sc2Matrix& m) const
{
	return (m_dE[0][0] == m.m_dE[0][0] && m_dE[0][1] == m.m_dE[0][1] && m_dE[1][0] == m.m_dE[1][0] && m_dE[1][1] == m.m_dE[1][1]);
}

inline bool sc2Matrix::operator != (const sc2Matrix& m) const
{
	return (!(*this==m));
}

//向量操作
inline sc2Vector sc2Matrix::operator * (const sc2Vector& v) const
{
	return sc2Vector(m_dE[0][0] * v.GetX() + m_dE[0][1] * v.GetY(), m_dE[1][0] * v.GetX() + m_dE[1][1] * v.GetY());
}

inline sc2Vector operator * (const sc2Vector& v, const sc2Matrix& m)
{ 
	return sc2Vector(v.GetX() * m.m_dE[0][0] + v.GetY() * m.m_dE[1][0], v.GetX() * m.m_dE[0][1] + v.GetY() * m.m_dE[1][1]);
}

inline sc2Vector& operator*=(sc2Vector& v, const sc2Matrix& m)
{
	return v = v * m;
}

inline sc2Vector operator/(const sc2Vector& v, const sc2Matrix& m)
{
	sc2Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return v * mInverse;
}

inline sc2Vector& operator/=(sc2Vector& v, const sc2Matrix& m)
{
	return v = v / m;
}

inline sc2Matrix operator*(double s, const sc2Matrix& m)
{
	return m * s;
}

inline sc2Matrix operator/(double s, const sc2Matrix& m)
{
	sc2Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return mInverse * s;
}

bool SVCORE_API sfRealEqual(const sc2Matrix& m1, const sc2Matrix& m2, double epsilon = 1.e-15);

// ---------------------------------------------------------------------
//                            三维矩阵sc3Matrix
// ---------------------------------------------------------------------

class SVCORE_API sc3Matrix
{
public:
	sc3Matrix()
	{
		m_dE[0][0] = 1.; m_dE[0][1] = 0.; m_dE[0][2] = 0.;
		m_dE[1][0] = 0.; m_dE[1][1] = 1.; m_dE[1][2] = 0.;
		m_dE[2][0] = 0.; m_dE[2][1] = 0.; m_dE[2][2] = 1.; m_dDT = 1.;
	}

	sc3Matrix(double e11, double e12, double e13, double e21, double e22, double e23, double e31, double e32, double e33)
	{
		m_dE[0][0] = e11; m_dE[0][1] = e12; m_dE[0][2] = e13;
		m_dE[1][0] = e21; m_dE[1][1] = e22; m_dE[1][2] = e23;
		m_dE[2][0] = e31; m_dE[2][1] = e32; m_dE[2][2] = e33; SetDT();
	}

	//返回矩阵的行列式值
	double Determinant() const { return m_dDT;}

	//矩阵的逆
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus Inverse(sc3Matrix& m) const;

	//返回矩阵的转置矩阵
	sc3Matrix Transpose() const;

	//判断矩阵是否奇异
	bool IsSingular() const;

	//判断矩阵是否为单位矩阵
	bool IsIdentity() const;

	//获取/设置矩阵元素的值
	//设置矩阵元素的值会改变当前矩阵的行列式
	double GetElement(int row, int column) const;
	void   SetElement(int row, int column, double value);

	//获取X轴缩放因子、Y轴缩放因子、Z轴缩放因子
	//如果当前矩阵奇异，返回HSingular；否则，返回HSuccess
	HStatus GetXScale(double& xScale) const;
	HStatus GetYScale(double& yScale) const;
	HStatus GetZScale(double& zScale) const;

	//单位矩阵
	static const sc3Matrix I;

	sc3Matrix  operator+(const sc3Matrix&) const;
	sc3Matrix& operator+=(const sc3Matrix&);

	sc3Matrix  operator-() const;
	sc3Matrix  operator-(const sc3Matrix&) const;
	sc3Matrix& operator-=(const sc3Matrix&);

	sc3Matrix  operator*(double) const;
	sc3Matrix& operator*=(double);

	sc3Matrix  operator*(const sc3Matrix&) const;
	sc3Matrix& operator*=(const sc3Matrix&);

	sc3Matrix  operator/(double) const;
	sc3Matrix& operator/=(double);

	sc3Matrix  operator/(const sc3Matrix&) const;
	sc3Matrix& operator/=(const sc3Matrix&);

	bool operator == (const sc3Matrix&) const;
	bool operator != (const sc3Matrix&) const;

	//向量操作
	sc3Vector operator*(const sc3Vector&) const;
	friend SVCORE_API sc3Vector operator*(const sc3Vector&, const sc3Matrix&);

private:
	double m_dE[3][3];			// 矩阵元素
	double m_dDT;				// 矩阵的行列式值
	void SetDT ()
	{
		m_dDT = (m_dE[0][0] * (m_dE[1][1] * m_dE[2][2] - m_dE[1][2] * m_dE[2][1]) -
			m_dE[0][1] * (m_dE[1][0] * m_dE[2][2] - m_dE[1][2] * m_dE[2][0]) +
			m_dE[0][2] * (m_dE[1][0] * m_dE[2][1] - m_dE[1][1] * m_dE[2][0]));
	};
};

inline HStatus sc3Matrix::Inverse(sc3Matrix& m) const
{
	if (IsSingular())
	{
		return HSingular;
	}

	double d1_DT = 1.0 / m_dDT;
	m.m_dE[0][0] =  d1_DT * ( m_dE[1][1]*m_dE[2][2] - m_dE[1][2]*m_dE[2][1] );
	m.m_dE[0][1] = -d1_DT * ( m_dE[0][1]*m_dE[2][2] - m_dE[0][2]*m_dE[2][1] );
	m.m_dE[0][2] =  d1_DT * ( m_dE[0][1]*m_dE[1][2] - m_dE[0][2]*m_dE[1][1] );
	m.m_dE[1][0] = -d1_DT * ( m_dE[1][0]*m_dE[2][2] - m_dE[1][2]*m_dE[2][0] );
	m.m_dE[1][1] =  d1_DT * ( m_dE[0][0]*m_dE[2][2] - m_dE[0][2]*m_dE[2][0] );
	m.m_dE[1][2] = -d1_DT * ( m_dE[0][0]*m_dE[1][2] - m_dE[0][2]*m_dE[1][0] );
	m.m_dE[2][0] =  d1_DT * ( m_dE[1][0]*m_dE[2][1] - m_dE[1][1]*m_dE[2][0] );
	m.m_dE[2][1] = -d1_DT * ( m_dE[0][0]*m_dE[2][1] - m_dE[0][1]*m_dE[2][0] );
	m.m_dE[2][2] =  d1_DT * ( m_dE[0][0]*m_dE[1][1] - m_dE[0][1]*m_dE[1][0] );

	return HSuccess;
}

inline sc3Matrix sc3Matrix::Transpose() const
{
	return sc3Matrix(m_dE[0][0], m_dE[1][0], m_dE[2][0],
		m_dE[0][1], m_dE[1][1], m_dE[2][1],
		m_dE[0][2], m_dE[1][2], m_dE[2][2]);
}

inline bool sc3Matrix::IsSingular() const
{
	return (m_dDT == 0.);
}

inline bool sc3Matrix::IsIdentity() const
{
	return ( m_dE[0][0] == 1. && m_dE[0][1] == 0. && m_dE[0][2] == 0. 
		&& m_dE[1][0] == 0. && m_dE[1][1] == 1. && m_dE[1][2] == 0.
		&& m_dE[2][0] == 0. && m_dE[2][1] == 0. && m_dE[2][2] == 1. );
}

inline double sc3Matrix::GetElement(int row,int column) const
{
	assert(row >= 0 && row < 3 && column >= 0 && column < 3);
	return m_dE[row][column];
}

inline void sc3Matrix::SetElement(int row, int column, double value)
{ 
	assert(row >= 0 && row < 3 && column >= 0 && column < 3);
	m_dE[row][column] = value;
	SetDT();
}

inline sc3Matrix  sc3Matrix::operator+(const sc3Matrix& m) const
{
	return sc3Matrix(m_dE[0][0] + m.m_dE[0][0], m_dE[0][1] + m.m_dE[0][1], m_dE[0][2] + m.m_dE[0][2],
		m_dE[1][0] + m.m_dE[1][0], m_dE[1][1] + m.m_dE[1][1], m_dE[1][2] + m.m_dE[1][2],
		m_dE[2][0] + m.m_dE[2][0], m_dE[2][1] + m.m_dE[2][1], m_dE[2][2] + m.m_dE[2][2]);
}

inline sc3Matrix& sc3Matrix::operator+=(const sc3Matrix& m)
{
	return *this = *this + m;
}

inline sc3Matrix  sc3Matrix::operator-() const
{
	return sc3Matrix(-m_dE[0][0], -m_dE[0][1], -m_dE[0][2],
		-m_dE[1][0], -m_dE[1][1], -m_dE[1][2],
		-m_dE[2][0], -m_dE[2][1], -m_dE[2][2]);
}

inline sc3Matrix  sc3Matrix::operator-(const sc3Matrix& m) const
{
	return sc3Matrix(m_dE[0][0] - m.m_dE[0][0], m_dE[0][1] - m.m_dE[0][1], m_dE[0][2] - m.m_dE[0][2],
		m_dE[1][0] - m.m_dE[1][0], m_dE[1][1] - m.m_dE[1][1], m_dE[1][2] - m.m_dE[1][2],
		m_dE[2][0] - m.m_dE[2][0], m_dE[2][1] - m.m_dE[2][1], m_dE[2][2] - m.m_dE[2][2]);
}

inline sc3Matrix& sc3Matrix::operator-=(const sc3Matrix& m)
{
	return *this = *this - m;
}

inline sc3Matrix  sc3Matrix::operator*(double d) const
{
	return sc3Matrix(m_dE[0][0] * d, m_dE[0][1] * d, m_dE[0][2] * d,
		m_dE[1][0] * d, m_dE[1][1] * d, m_dE[1][2] * d,
		m_dE[2][0] * d, m_dE[2][1] * d, m_dE[2][2] * d);
}

inline sc3Matrix& sc3Matrix::operator*=(double s)
{
	return *this = *this * s;
}

inline sc3Matrix  sc3Matrix::operator*(const sc3Matrix& m) const
{
	return sc3Matrix(m_dE[0][0] * m.m_dE[0][0] + m_dE[0][1] * m.m_dE[1][0] + m_dE[0][2] * m.m_dE[2][0],
		m_dE[0][0] * m.m_dE[0][1] + m_dE[0][1] * m.m_dE[1][1] + m_dE[0][2] * m.m_dE[2][1],
		m_dE[0][0] * m.m_dE[0][2] + m_dE[0][1] * m.m_dE[1][2] + m_dE[0][2] * m.m_dE[2][2],
		m_dE[1][0] * m.m_dE[0][0] + m_dE[1][1] * m.m_dE[1][0] + m_dE[1][2] * m.m_dE[2][0],
		m_dE[1][0] * m.m_dE[0][1] + m_dE[1][1] * m.m_dE[1][1] + m_dE[1][2] * m.m_dE[2][1],
		m_dE[1][0] * m.m_dE[0][2] + m_dE[1][1] * m.m_dE[1][2] + m_dE[1][2] * m.m_dE[2][2],
		m_dE[2][0] * m.m_dE[0][0] + m_dE[2][1] * m.m_dE[1][0] + m_dE[2][2] * m.m_dE[2][0],
		m_dE[2][0] * m.m_dE[0][1] + m_dE[2][1] * m.m_dE[1][1] + m_dE[2][2] * m.m_dE[2][1],
		m_dE[2][0] * m.m_dE[0][2] + m_dE[2][1] * m.m_dE[1][2] + m_dE[2][2] * m.m_dE[2][2]);
}

inline sc3Matrix& sc3Matrix::operator*=(const sc3Matrix& m)
{
	return *this = *this * m;
}

inline sc3Matrix sc3Matrix::operator/(double s) const
{
	assert(s != 0);
	return *this * (1. / s);
}

inline sc3Matrix& sc3Matrix::operator/=(double s)
{
	return *this = *this / s; 
}

inline sc3Matrix sc3Matrix::operator/(const sc3Matrix& m) const
{
	sc3Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return (*this) * mInverse;
}

inline sc3Matrix& sc3Matrix::operator/=(const sc3Matrix& m)
{
	return *this = *this / m;
}

inline bool sc3Matrix::operator == (const sc3Matrix& m) const
{
	return (m_dE[0][0] == m.m_dE[0][0] && m_dE[0][1] == m.m_dE[0][1] && m_dE[0][2] == m.m_dE[0][2]
		&& m_dE[1][0] == m.m_dE[1][0] && m_dE[1][1] == m.m_dE[1][1] && m_dE[1][2] == m.m_dE[1][2]
		&& m_dE[2][0] == m.m_dE[2][0] && m_dE[2][1] == m.m_dE[2][1] && m_dE[2][2] == m.m_dE[2][2]);
}

inline bool sc3Matrix::operator != (const sc3Matrix& m) const
{
	return (!(*this==m));
}

//向量操作
inline sc3Vector sc3Matrix::operator*(const sc3Vector& v) const
{
	return sc3Vector(m_dE[0][0] * v.GetX() + m_dE[0][1] * v.GetY() + m_dE[0][2] * v.GetZ(),
		m_dE[1][0] * v.GetX() + m_dE[1][1] * v.GetY() + m_dE[1][2] * v.GetZ(),
		m_dE[2][0] * v.GetX() + m_dE[2][1] * v.GetY() + m_dE[2][2] * v.GetZ());
}

inline sc3Vector operator*(const sc3Vector& v, const sc3Matrix& m)
{
	return sc3Vector(v.GetX() * m.m_dE[0][0] + v.GetY() * m.m_dE[1][0] + v.GetZ() * m.m_dE[2][0],
		v.GetX() * m.m_dE[0][1] + v.GetY() * m.m_dE[1][1] + v.GetZ() * m.m_dE[2][1],
		v.GetX() * m.m_dE[0][2] + v.GetY() * m.m_dE[1][2] + v.GetZ() * m.m_dE[2][2]);
}

inline sc3Vector& operator*=(sc3Vector& v, const sc3Matrix& m)
{
	return v = v * m;
}

inline sc3Vector operator/(const sc3Vector& v, const sc3Matrix& m)
{
	sc3Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return v * mInverse;
}

inline sc3Vector& operator/=(sc3Vector& v, const sc3Matrix& m)
{
	return v = v / m;
}

inline sc3Matrix operator*(double s, const sc3Matrix& m)
{
	return m * s;
}

inline sc3Matrix operator/(double s, const sc3Matrix& m)
{
	sc3Matrix mInverse;
	HStatus status = m.Inverse(mInverse);

	assert(IsSuccess(status));

	return mInverse * s;
}

bool SVCORE_API sfRealEqual(const sc3Matrix& m1, const sc3Matrix& m2, double epsilon = 1.e-15);

bool SVCORE_API sfMatrixInvert(double* matrix, double* matrix_invert, int m, int n);

#pragma pack(pop)
