#pragma once

#include "svVector3D.h"

#pragma pack(push,8)

class SVGEOMETRICS_API_3D scVector4D
{
public:
	scVector4D();
	scVector4D(float v1,float v2,float v3,float v4);
	scVector4D(const scVector3D& v,float v4);
	scVector4D(float v1,const scVector3D& v);
	scVector4D(const scVector4D& v);
	~scVector4D() {}

	scVector4D& operator=(const scVector4D& v);

	bool operator==(const scVector4D& v)const;
	bool operator!=(const scVector4D& v)const;

	scVector4D operator+(const scVector4D& v)const;
	scVector4D& operator+=(const scVector4D& v);

	scVector4D operator-(const scVector4D& v)const;
	scVector4D& operator-=(const scVector4D& v);
	scVector4D operator-()const;

	float operator*(const scVector4D& v)const;
	scVector4D operator*(float scale)const;
	scVector4D& operator*=(float scale);
	friend scVector4D operator*(float scale,const scVector4D& v) {return v*scale;}

	scVector4D operator/(float scale)const;
	scVector4D& operator/=(float scale);

	float operator[](int nIndex)const;
	float& operator[](int nIndex);

public:
	float at(int nIndex)const;
	float& at(int nIndex);

	float* GetValuePtr();

	bool IsZero()const;

	float dot(const scVector4D& v)const;

	float Length()const;
	scVector4D Normalize()const;

	void Set(float dx,float dy,float dz,float dw);
	void SetX(float dx){m_fDir[0] = dx;}
	void SetY(float dy){m_fDir[1] = dy;}
	void SetZ(float dz){m_fDir[2] = dz;}
	void SetW(float dw){m_fDir[3] = dw;}

	void Get(float& dx,float& dy,float& dz,float& dw)const;
	float GetX()const {return m_fDir[0];}
	float GetY()const {return m_fDir[1];}
	float GetZ()const {return m_fDir[2];}
	float GetW()const {return m_fDir[3];}

private:
	float m_fDir[4];
};

#pragma pack(pop)