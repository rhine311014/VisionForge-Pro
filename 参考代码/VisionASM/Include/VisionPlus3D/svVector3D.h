#pragma once

#include "svPoint3D.h"
#include <vector>

#pragma pack(push,8)

class SVGEOMETRICS_API_3D scVector3D
{
public:
	scVector3D(); // 默认为零向量
	scVector3D(float dx,float dy,float dz);
	scVector3D(const scPoint3D& P);
	scVector3D(const scPoint3D& P1,const scPoint3D& P2); // P2 - P1
	scVector3D(const scVector3D& vec);

	~scVector3D();

	scVector3D& operator=(const scVector3D& vec);
	scVector3D& operator=(const scPoint3D& point);

	scVector3D operator+(const scVector3D& vec) const;
	scVector3D operator-(const scVector3D& vec) const;
	scVector3D operator-()const;
	scVector3D& operator+=(const scVector3D& vec);
	scVector3D& operator-=(const scVector3D& vec);

	scVector3D operator*(float scale) const;
	float operator*(const scVector3D& vec)const;
	friend scVector3D operator*(float scale,const scVector3D& vec) {return (vec*scale);}
	scVector3D& operator*=(float scale);
	
	scVector3D operator/(float scale)const;
	scVector3D& operator/=(float scale);

	bool operator==(const scVector3D& vec)const;
	bool operator!=(const scVector3D& vec)const;

	float operator[](int nIndex)const;
	float& operator[](int nIndex);

public:
	float at(int nIndex)const;
	float& at(int nIndex);

	void Set(float dx,float dy,float dz);
	void Get(float& dx,float& dy,float& dz)const;

	void SetX(float dx) {m_fDir[0] = dx;}
	void SetY(float dy) {m_fDir[1] = dy;}
	void SetZ(float dz) {m_fDir[2] = dz;}

	float GetX() const {return m_fDir[0];}
	float GetY() const {return m_fDir[1];}
	float GetZ() const {return m_fDir[2];}

	float Length() const; // 向量长度
	scVector3D Normalize()const; // 单位化

	float Dot(const scVector3D& vec)const;  // 点乘
	scVector3D Cross(const scVector3D& vec)const; // 叉乘

	float Distance(const scVector3D& vec)const; // 距离

	float Angle(const scVector3D& vec)const; // [0,PI]

	bool IsParallel(const scVector3D& v)const;      // 判断两向量是否平行
	bool IsPerpendicular(const scVector3D& v)const; // 判断两向量是否垂直

	std::vector<scVector3D> GenPerpendicularVector(int n)const; // 生成n个与当前向量垂直的向量
	bool GenOrthogonalVector(scVector3D& u,scVector3D& v)const; // 生成两个单位向量，与当前向量构成的三个向量两两垂直

	scVector3D Rotate(const scVector3D& axis,float angle)const; // 返回绕axis向量旋转angle度后的新向量

	// 是否为0向量
	bool IsZero()const;

	scPoint3D TransToPoint()const;

private:
	float m_fDir[3];
};

#pragma pack(pop)