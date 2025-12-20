#pragma once

#include "svPoint3D.h"
#include "svVector3D.h"

class scCircle3D;

#pragma pack(push,8)

// 圆环面
// 由圆心、姿态轴、内圆半径、外圆半径定义
// 圆环面可看做以外圆上任意点为圆心、外圆在该点切线方向为法向量、内圆半径为半径所形成的圆的集合
class SVGEOMETRICS_API_3D scTorus : public scShape3D
{
public:
	scTorus();
	scTorus(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float rOuter,float rInner);
	scTorus(const scTorus& torus);

	virtual ~scTorus();

	scTorus& operator=(const scTorus& torus);

	bool operator==(const scTorus& torus)const;
	bool operator!=(const scTorus& torus)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eTorus;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float rOuter,float rInner);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& rOuter,float& rInner)const;

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetAxisU()const {return m_AxisU;}
	scVector3D GetAxisV()const {return m_AxisV;}
	scVector3D GetAxisW()const {return m_AxisW;}

	void SetRadiusOuter(float rOuter) {m_fRadiusOuter = rOuter;}
	float GetRadiusOuter()const {return m_fRadiusOuter;}

	void SetRadiusInner(float rInner) {m_fRadiusInner = rInner;}
	float GetRadiusInner()const {return m_fRadiusInner;} 

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

	scCircle3D GetOuterCirlce3D()const;  // 获取圆环面的外圆
	scCircle3D GetInnerCircle3D(float theta)const; // 获取圆环面的内圆
 
private:
	scPoint3D m_Center;
	scVector3D m_AxisU;
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float      m_fRadiusOuter;
	float      m_fRadiusInner;
};

#pragma pack(pop)