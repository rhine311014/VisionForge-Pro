#pragma once

#include "svPoint3D.h"
#include "svVector3D.h"
#include "float.h"

class scLine3D;

#pragma pack(push,8)

// 射线
// 由射线起点、射线方向定义
class SVGEOMETRICS_API_3D scRay3D : public scShape3D
{
public:
	scRay3D();
	scRay3D(const scPoint3D& P,const scVector3D& dir);
	scRay3D(const scRay3D& ray);

	virtual ~scRay3D();

	scRay3D& operator=(const scRay3D& ray);
	bool operator==(const scRay3D& ray) const;
	bool operator!=(const scRay3D& ray) const;

	virtual float GetPerimeter() const {return FLT_MAX;}
	virtual float GetArea() const {return 0.f;}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eRay3D;}

	virtual scSphere GetExternalSphere()const;

public:
	void SetOrigion(const scPoint3D& P);
	void SetDirection(const scVector3D& dir);

	scPoint3D  GetOrigion() const;
	scVector3D GetDirection() const;

	// 判断射线是否退化
	bool Degenerate() const;

	// 获取射线所在直线
	scLine3D GetLine3D() const;

private:
	scPoint3D  m_Point;
	scVector3D m_Dir;
};

#pragma pack(pop)