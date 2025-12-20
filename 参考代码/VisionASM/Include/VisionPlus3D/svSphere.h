#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include <cassert>

#pragma pack(push,8)

// 球体
// 由中心、半径定义
class SVGEOMETRICS_API_3D scSphere : public scShape3D
{
public:
	scSphere();
	scSphere(const scPoint3D& center,float radius);
	scSphere(const scSphere& sphere);

	virtual ~scSphere();

	scSphere& operator=(const scSphere& sphere);

	bool operator==(const scSphere& sphere)const;
	bool operator!=(const scSphere& sphere)const;

	virtual float GetPerimeter() const{return 0.f;}
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eSphere;}

	virtual scSphere GetExternalSphere()const {return *this;}
	scSphere GetExternalSphere(const scSphere& sphere)const;

public:
	void Set(const scPoint3D& center,float radius);
	void Get(scPoint3D& center,float& radius)const;

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter() const {return m_Center;}

	void SetRadius(float radius) {assert(radius >= 0.f);m_fRadius = radius;}
	float GetRadius()const {return m_fRadius;}

private:
	scPoint3D m_Center;
	float m_fRadius;
};

#pragma pack(pop)