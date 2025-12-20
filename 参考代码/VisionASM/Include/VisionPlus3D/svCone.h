#pragma once

#include "svPoint3D.h"
#include "svVector3D.h"

class scCircle3D;

#pragma pack(push,8)

// 圆锥
// 由圆锥顶点、中心轴方向、角度、最短距离、最长距离定义

class SVGEOMETRICS_API_3D scCone : public scShape3D
{
public:
	scCone();
	scCone(const scPoint3D& origion,const scVector3D& axis,float degree,float hMin,float hMax);
	scCone(const scCone& cone);

	virtual ~scCone();

	scCone& operator=(const scCone& cone);

	bool operator==(const scCone& cone)const;
	bool operator!=(const scCone& cone)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eCone;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& origion,const scVector3D& axis,float degree,float hMin,float hMax);
	void Get(scPoint3D& origion,scVector3D& axis,float& degree,float& hMin,float& hMax)const;

	void SetOrigion(const scPoint3D& origion) {m_Origion = origion;}
	scPoint3D GetOrigion()const {return m_Origion;}

	void SetAxis(const scVector3D& axis) {m_Axis = axis.Normalize();}
	scVector3D GetAxis()const {return m_Axis;}

	void SetDegree(float degree) {m_fDegree = degree;}
	float GetDegree()const {return m_fDegree;}

	void SetHeightMin(float hMin) {m_fHeightMin = hMin;}
	float GetHeightMin()const {return m_fHeightMin;}

	void SetHeightMax(float hMax) {m_fHeightMax = hMax;}
	float GetHeightMax()const {return m_fHeightMax;}

	scCircle3D GetTopCircle3D()const;
	scCircle3D GetBottomCircle3D()const;

private:
	scPoint3D  m_Origion;    // 顶点
	scVector3D m_Axis;       // 中心轴方向
	float      m_fDegree;    // 单位为弧度，圆锥面上任意一点与顶点的连线与轴线的夹角
	float      m_fHeightMin; // 最短距离，圆锥面上的点在轴线上的投影与圆锥顶点的最短距离
	float      m_fHeightMax; // 最长距离，圆锥面上的点在轴线上的投影与圆锥顶点的最长距离
};

#pragma pack(pop)