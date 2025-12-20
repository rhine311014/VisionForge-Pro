#pragma once

#include "svSegment3D.h"

#pragma pack(push,8)

// 胶囊体，其上所有点到中心线段的距离均等于半径
// 由中心线段、半径确定
class SVGEOMETRICS_API_3D scCapsule : public scShape3D
{
public:
	scCapsule();
	scCapsule(const scSegment3D& seg,float radius);
	scCapsule(const scCapsule& capsule);

	virtual ~scCapsule();

	scCapsule& operator=(const scCapsule& capsule);

	bool operator==(const scCapsule& capsule)const;
	bool operator!=(const scCapsule& capsule)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eCapsule;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scSegment3D& seg,float radius);
	void Get(scSegment3D& seg,float& radius)const;

	void SetSegment3D(const scSegment3D& seg) {m_Segment = seg;}
	scSegment3D GetSegment3D()const {return m_Segment;}

	void SetRadius(float radius) {m_fRadius = radius;}
	float GetRadius()const {return m_fRadius;}

private:
	scSegment3D m_Segment;  // 中心线段
	float m_fRadius;        // 半径
};

#pragma pack(pop)

