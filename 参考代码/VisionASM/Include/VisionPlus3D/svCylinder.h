#pragma once

#include "svDef3D.h"
#include "svLine3D.h"

#include <cassert>

class scCircle3D;

#pragma pack(push,8)

// 圆柱
// 由中心轴线、高度、半径定义
// 轴线的起始点即为圆柱中心点
class SVGEOMETRICS_API_3D scCylinder : public scShape3D
{
public:
	scCylinder();
	scCylinder(const scLine3D& line,float height,float radius);
	scCylinder(const scCylinder& cylinder);

	virtual ~scCylinder();

	scCylinder& operator=(const scCylinder& cylinder);

	bool operator==(const scCylinder& cylinder)const;
	bool operator!=(const scCylinder& cylinder)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eCylinder;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scLine3D& line,float height,float radius);
	void Get(scLine3D& line,float& height,float& radius)const;

	void SetAxisLine(const scLine3D& line) {m_AxisLine = line;}
	scLine3D GetAxisLine()const {return m_AxisLine;}

	void SetHeight(float height) {assert(height >= 0.f);m_fHeight = height;}
	float GetHeight()const {return m_fHeight;}

	void SetRadius(float radius) {assert(radius >= 0.f);m_fRadius = radius;}
	float GetRadius()const {return m_fRadius;}

	scCircle3D GetBottomCircle3D()const;
	scCircle3D GetTopCircle3D()const;

private:
	scLine3D m_AxisLine;  // 中心轴线
	float m_fHeight;      // 圆柱高度
	float m_fRadius;      // 圆柱半径
};

#pragma pack(pop)