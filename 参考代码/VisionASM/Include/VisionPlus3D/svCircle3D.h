#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

class scPlane;

#pragma pack(push,8)

// 三维圆
// 由圆心、法向量、半径确定
// p = center + radius*(u*cos(theta) + v*sin(theta));
// p为圆上任意一点，u，v为圆平面上垂直的向量基，可由法向量生成
class SVGEOMETRICS_API_3D scCircle3D: public scShape3D
{
public:
	scCircle3D();
	scCircle3D(const scPoint3D& center,const scVector3D& normal,float radius);
	scCircle3D(const scCircle3D& circle);

	virtual ~scCircle3D();

	scCircle3D& operator=(const scCircle3D& circle);

	bool operator==(const scCircle3D& circle)const;
	bool operator!=(const scCircle3D& cirlce)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eCircle3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& p,const scVector3D& normal,float radius);
	void Get(scPoint3D& p,scVector3D& normal,float& radius)const;

	void SetCenter(const scPoint3D& p) {m_Center = p;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetNormal(const scVector3D& v) {m_Normal = v.Normalize();}
	scVector3D GetNormal()const {return m_Normal;}

	void SetRadius(float radius) {m_fRadius = radius;}
	float GetRadius()const {return m_fRadius;}

	// 获取圆所在平面
	scPlane GetPlane()const;

private:
	scPoint3D  m_Center;  // 圆心
	scVector3D m_Normal;  // 法向量
	float      m_fRadius; // 半径
};

#pragma pack(pop)