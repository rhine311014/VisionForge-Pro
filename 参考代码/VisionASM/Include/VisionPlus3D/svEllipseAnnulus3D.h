#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include "svEllipse3D.h"

class scPlane;

#pragma pack(push,8)

// 椭圆环
// 由椭圆心、姿态轴、两外椭圆半径、内椭圆与外椭圆半径比值定义
class SVGEOMETRICS_API_3D scEllipseAnnulus3D:public scShape3D
{
public:
	scEllipseAnnulus3D();
	scEllipseAnnulus3D(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLength,float ratio);
	scEllipseAnnulus3D(const scEllipse3D& ellipseOuter,float ratio);
	scEllipseAnnulus3D(const scEllipseAnnulus3D& ellipseAnnulus);

	virtual ~scEllipseAnnulus3D();

	scEllipseAnnulus3D& operator=(const scEllipseAnnulus3D& ellipseAnnulus);

	bool operator==(const scEllipseAnnulus3D& ellipseAnnulus)const;
	bool operator!=(const scEllipseAnnulus3D& ellipseAnnulus)const;

	virtual float GetPerimeter() const {return 0.f;}
	virtual float GetArea() const;
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eEllipseAnnulus3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLength,float ratio);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uLength,float& vLength,float& ratio)const;

	void Set(const scEllipse3D& ellipseOuter,float ratio);
	void Get(scEllipse3D& ellipseOuter,float& ratio)const;

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetAxisU()const {return m_AxisU;}
	scVector3D GetAxisV()const {return m_AxisV;}
	scVector3D GetAxisW()const {return m_AxisW;}

	void SetRadiusU(float uRadius) {m_fRadiusU = uRadius;}
	void SetRadiusV(float vRadius) {m_fRadiusV = vRadius;}

	float GetRadiusU()const {return m_fRadiusU;}
	float GetRadiusV()const {return m_fRadiusV;}

	void SetRatio(float ratio) {m_fRatio = ratio;}
	float GetRatio()const {return m_fRatio;}

	scEllipse3D GetEllipse3DInner()const;
	scEllipse3D GetEllipse3DOuter()const;

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

	// 获取椭圆环所在平面
	scPlane GetPlane()const;

private:
	scPoint3D  m_Center;   // 椭圆心
	scVector3D m_AxisU;    // 姿态轴，三轴相互垂直
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float      m_fRadiusU; // 外椭圆U轴半径
	float      m_fRadiusV; // 外椭圆V轴半径
	float      m_fRatio;   // 内椭圆与外椭圆半径比值
};

#pragma pack(pop)