#pragma once

#include "svPoint3D.h"
#include "svVector3D.h"
#include "float.h"

#pragma pack(push,8)

// 直线
// 由起始点、方向定义
class SVGEOMETRICS_API_3D scLine3D : scShape3D
{
public:
	scLine3D();
	scLine3D(const scPoint3D& P,const scVector3D& dir);
	scLine3D(const scLine3D& line);

	virtual ~scLine3D();

	scLine3D& operator=(const scLine3D& line);

	bool operator==(const scLine3D& line)const;
	bool operator!=(const scLine3D& line)const;

	virtual float GetPerimeter() const {return FLT_MAX;}
	virtual float GetArea() const {return 0.f;}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eLine3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void SetOrigion(const scPoint3D& P);
	void SetDirection(const scVector3D& dir);

	scPoint3D GetOrigion() const;
	scVector3D GetDirection() const;

	// 判断直线是否退化
	bool Degenerate() const;

private:
	scPoint3D  m_Point;
	scVector3D m_Dir;
};

#pragma pack(pop)