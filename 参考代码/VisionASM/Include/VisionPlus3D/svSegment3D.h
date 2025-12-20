#pragma once

#include "svPoint3D.h"
#include "svVector3D.h"

class scRay3D;
class scLine3D;

#pragma pack(push,8)

// 线段
class SVGEOMETRICS_API_3D scSegment3D : public scShape3D
{
public:
	scSegment3D();
	scSegment3D(const scPoint3D& p1,const scPoint3D& p2);                // 由起始点、终止点定义
	scSegment3D(const scPoint3D& p1,const scVector3D& dir,float length); // 由起始点、线段方向、线段长度定义，p2 = p1 + length*dir
	scSegment3D(const scSegment3D& seg);

	virtual ~scSegment3D();

	scSegment3D& operator=(const scSegment3D& seg);
	bool operator==(const scSegment3D& seg)const;
	bool operator!=(const scSegment3D& seg)const;

	virtual float GetPerimeter() const {return (m_P1 - m_P2).Length();}
	virtual float GetArea() const {return 0.f;}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eSegment3D;}

	virtual scSphere GetExternalSphere()const;
	
public:
	void SetP1(const scPoint3D& P);
	void SetP2(const scPoint3D& P);

	scPoint3D GetP1() const;
	scPoint3D GetP2() const;

	bool Degenerate() const;

	scRay3D GetRay3D() const;
	scLine3D GetLine3D() const;

private:
	scPoint3D m_P1;
	scPoint3D m_P2;
};

#pragma pack(pop)
