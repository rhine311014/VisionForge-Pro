#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

class scRectangle3D;

#pragma pack(push,8)

// 视锥体
// 由视点、视线方向、视平面两轴、近裁平面到视点的距离、远裁平面到视点的距离、近裁平面在视平面两轴方向上的半长定义
class SVGEOMETRICS_API_3D scFrustum : public scShape3D
{
public:
	scFrustum();
	scFrustum(const scPoint3D& origion,const scVector3D& viewDir,const scVector3D& axisU,const scVector3D& axisV,float nearDis,float farDis,float uBound,float vBound);
	scFrustum(const scFrustum& frustum);

	virtual ~scFrustum();

	scFrustum& operator=(const scFrustum& frustum);

	bool operator==(const scFrustum& frustum)const;
	bool operator!=(const scFrustum& frustum)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eFrustum;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& origion,const scVector3D& viewDir,const scVector3D& axisU,const scVector3D& axisV,float nearDis,float farDis,float uBound,float vBound);
	void Get(scPoint3D& origion,scVector3D& viewDir,scVector3D& axisU,scVector3D& axisV,float& nearDis,float& farDis,float& uBound,float& vBound)const;

	void SetOrigion(const scPoint3D& origion) {m_Origion = origion;}
	scPoint3D GetOrigion()const {return m_Origion;}

	void SetViewDirection(const scVector3D& vec) {m_ViewDirection = vec.Normalize();}
	scVector3D GetViewDirection()const {return m_ViewDirection;}

	void SetAxisU(const scVector3D& uAxis) {m_AxisU = uAxis.Normalize();}
	scVector3D GetAxisU()const {return m_AxisU;}

	void SetAxisV(const scVector3D& vAxis) {m_AxisV = vAxis.Normalize();}
	scVector3D GetAxisV()const {return m_AxisV;}

	void SetNearPlaneDistance(float dis) {m_fNearPlaneDis = dis;}
	float GetNearPlaneDistance()const {return m_fNearPlaneDis;}

	void SetFarPlaneDistance(float dis) {m_fFarPlaneDis = dis;}
	float GetFarPlaneDistance()const {return m_fFarPlaneDis;}

	void SetBoundU(float uBound) {m_fBoundU = uBound;}
	float GetBoundU()const {return m_fBoundU;}

	void SetBoundV(float vBound) {m_fBoundV = vBound;}
	float GetBoundV()const {return m_fBoundV;}

	scRectangle3D GetNearRectangle3D()const;
	scRectangle3D GetFarRectangle3D()const;

	// 施密特正交化
	void SchmidtOrthogonalization(); 

private:
	scPoint3D m_Origion;        // 视点
	scVector3D m_ViewDirection; // 视线方向
	scVector3D m_AxisU;         // 视平面轴，两轴相互垂直
	scVector3D m_AxisV;
	float m_fNearPlaneDis;      // 近裁平面到视点的距离
	float m_fFarPlaneDis;       // 远裁平面到视点的距离
	float m_fBoundU;            // 近裁平面在U轴方向上的半长
	float m_fBoundV;            // 近裁平面在V轴方向上的半长
};

#pragma pack(pop)