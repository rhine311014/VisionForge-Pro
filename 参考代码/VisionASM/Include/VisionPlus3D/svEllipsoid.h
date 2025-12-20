#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include "float.h"

#pragma pack(push,8)

// 椭球
// 由椭球心、姿态轴、姿态轴对应的半径定义
class SVGEOMETRICS_API_3D scEllipsoid : public scShape3D
{
public:
	scEllipsoid();
	scEllipsoid(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uRadius,float vRadius,float wRadius);
	scEllipsoid(const scEllipsoid& ellipsoid);

	virtual ~scEllipsoid();

	scEllipsoid& operator=(const scEllipsoid& ellipsoid);

	bool operator==(const scEllipsoid& ellipsoid)const;
	bool operator!=(const scEllipsoid& ellipsoid)const;

	virtual float GetPerimeter() const {return FLT_MAX;}
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eEllipsoid;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uRadius,float vRadius,float wRadius);
	void Get(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uRadius,float& vRadius,float& wRadius);

	void SetCenter(const scPoint3D& center) {m_Center = center;}
	scPoint3D GetCenter()const {return m_Center;}

	void SetAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetAxis(scVector3D& u,scVector3D& v,scVector3D& w);

	scVector3D GetAxisU()const {return m_AxisU;}
	scVector3D GetAxisV()const {return m_AxisV;}
	scVector3D GetAxisW()const {return m_AxisW;}

	void SetRadiusU(float uRadius) {m_fRadiusU = uRadius;}
	float GetRadiusU()const {return m_fRadiusU;}

	void SetRadiusV(float vRadius) {m_fRadiusV = vRadius;}
	float GetRadiusV()const {return m_fRadiusV;}

	void SetRadiusW(float wRadius) {m_fRadiusW = wRadius;}
	float GetRadiusW()const {return m_fRadiusW;}

	// 施密特正交化,使u、v、w三轴互相垂直,以u轴为基准
	void SchmidtOrthogonalization(); 

private:
	scPoint3D m_Center;  // 椭球心
	scVector3D m_AxisU;  // 姿态轴，轴相互垂直
	scVector3D m_AxisV;
	scVector3D m_AxisW;
	float m_fRadiusU;    // U轴半径
	float m_fRadiusV;    // V轴半径
	float m_fRadiusW;    // W轴半径
};

#pragma pack(pop)
