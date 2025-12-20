#pragma once

#include "svShape3D.h"

class scVector3D;
#pragma pack(push,4)

class SVGEOMETRICS_API_3D scPoint3D : public scShape3D
{
public:
	scPoint3D();
	scPoint3D(float x,float y,float z);
	scPoint3D(const scPoint3D& point);

	virtual ~scPoint3D(){}

	scPoint3D& operator=(const scPoint3D& point);
	scPoint3D  operator+(const scPoint3D& point) const;
	scPoint3D  operator-(const scPoint3D& point) const;
	scPoint3D  operator-()const;
	scPoint3D& operator+=(const scPoint3D& point);
	scPoint3D& operator-=(const scPoint3D& point);

	scPoint3D  operator*(float scale) const;
	friend scPoint3D operator*(float scale,const scPoint3D& point) {return (point*scale);}
	scPoint3D& operator*=(float scale);

	scPoint3D  operator/(float scale) const;
	scPoint3D& operator/=(float scale);

	bool operator==(const scPoint3D& point)const;
	bool operator!=(const scPoint3D& point)const;

	float operator[](int nIndex)const;
	float& operator[](int nIndex);

	virtual float GetPerimeter() const {return 0.f;}
	virtual float GetArea() const {return 0.f;}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return ePoint3D;}

	virtual scSphere GetExternalSphere()const;

public:
	float at(int nIndex) const;
	float& at(int nIndex);

	float Length() const;
	float Distance(const scPoint3D& point) const;
	
	void Set(float x,float y,float z);
	void Get(float& x,float& y,float& z)const;

	void SetX(float x) { m_fX[0] = x;}
	void SetY(float y) { m_fX[1] = y;}
	void SetZ(float z) { m_fX[2] = z;}

	float GetX() const { return m_fX[0];}
	float GetY() const { return m_fX[1];}
	float GetZ() const { return m_fX[2];}

private:
	float m_fX[3];
};

#pragma pack(pop)
