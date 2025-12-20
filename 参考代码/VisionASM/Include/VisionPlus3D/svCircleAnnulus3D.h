#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include "svCircle3D.h"

class scPlane;

#pragma pack(push,8)

// 圆环
// 由圆心、法向量、外圆半径、内圆半径定义，其余与圆类似
class SVGEOMETRICS_API_3D scCircleAnnulus3D: public scShape3D
{
public:
	scCircleAnnulus3D();
	scCircleAnnulus3D(const scPoint3D& center,const scVector3D& normal,float fRadiusInner,float fRadiusOuter);
	scCircleAnnulus3D(const scCircle3D& circleOuter,float ratio); // ratio = innerRadius / outerRadius
	scCircleAnnulus3D(const scCircleAnnulus3D& annulus);

	virtual ~scCircleAnnulus3D();

	scCircleAnnulus3D& operator=(const scCircleAnnulus3D& annulus);

	bool operator==(const scCircleAnnulus3D& annulus)const;
	bool operator!=(const scCircleAnnulus3D& annulus)const;

	virtual float GetPerimeter() const {return 0.f;}
	virtual float GetArea() const {return (GetOuterCircle().GetArea() - GetInnerCircle().GetArea());}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eCircleAnnulus3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& normal,float fRadiusInner,float fRadiusOuter);
	void Get(scPoint3D& center,scVector3D& normal,float& fRadiusInner,float& fRadiusOuter)const;

	void Set(const scCircle3D& circleOuter,float ratio);
	void Get(scCircle3D& circleOuter,float& ratio)const;

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetNormal(const scVector3D& v) {m_Normal = v.Normalize();}
	scVector3D GetNormal()const {return m_Normal;}

	void SetRadius(float inner,float outer);
	void GetRadius(float& inner,float& outer)const;

	float GetRadiusInner()const {return m_fRadiusInner;}
	float GetRadiusOuter()const {return m_fRadiusOuter;}

	scCircle3D GetInnerCircle()const;
	scCircle3D GetOuterCircle()const;

	// 获取圆环所在平面
	scPlane GetPlane()const;

private:
	scPoint3D  m_Center;        // 圆心
	scVector3D m_Normal;        // 法向量
	float      m_fRadiusInner;  // 内圆半径
	float      m_fRadiusOuter;  // 外圆半径
};

#pragma pack(pop)