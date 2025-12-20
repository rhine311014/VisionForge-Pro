#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

class scPlane;

#pragma pack(push,8)

// 椭圆
// 由椭圆新、椭圆姿态轴、两半径定义
// p = center + uLength*u*cos(theta) + vLength*v*sin(theta);
class SVGEOMETRICS_API_3D scEllipse3D: public scShape3D
{
public:
	scEllipse3D();
	scEllipse3D(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLength);
	scEllipse3D(const scEllipse3D& ellipse);

	virtual ~scEllipse3D();

	scEllipse3D& operator=(const scEllipse3D& ellipse);
	bool operator==(const scEllipse3D& ellipse)const;
	bool operator!=(const scEllipse3D& ellipse)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eEllipse3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uRadius,float vRadius);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uRadius,float& vRadius);

	void SetCenter(const scPoint3D& p) {m_Center = p;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetAxisU()const {return m_AxisU;}
	scVector3D GetAxisV()const {return m_AxisV;}
	scVector3D GetAxisW()const {return m_AxisW;}

	void SetRadiusU(float radius) {m_fRadiusU = radius;}
	void SetRadiusV(float radius) {m_fRadiusV = radius;}

	float GetRadiusU()const {return m_fRadiusU;}
	float GetRadiusV()const {return m_fRadiusV;}

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

	// 获取椭圆所在平面
	scPlane GetPlane()const;

private:
	scPoint3D  m_Center;  // 椭圆心
	scVector3D m_AxisU;   // 椭圆姿态轴，三轴相互垂直
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float      m_fRadiusU; // U轴半径
	float      m_fRadiusV; // V轴半径
};

#pragma pack(pop)