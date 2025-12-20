#pragma once

#include "svRectangle3D.h"

#pragma pack(push,8)

// 长圆体
// 由中心矩形、半径定义
// 长圆体上所有的点到中心矩形的距离均等于半径
class SVGEOMETRICS_API_3D scLozenge: public scShape3D
{
public:
	scLozenge();
	scLozenge(const scRectangle3D& rect,float radius);
	scLozenge(const scLozenge& lozenge);

	virtual ~scLozenge();

	scLozenge& operator=(const scLozenge& lozenge);

	bool operator==(const scLozenge& lozenge)const;
	bool operator!=(const scLozenge& lozenge)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eLozenge;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scRectangle3D& rect,float radius);
	void Get(scRectangle3D& rect,float& radius)const;

	void SetRectangle3D(const scRectangle3D& rect) {m_Rect = rect;}
	scRectangle3D GetRectangle3D()const {return m_Rect;}

	void SetRadius(float radius) {m_fRadius = radius;}
	float GetRadius()const {return m_fRadius;}

private:
	scRectangle3D m_Rect;
	float m_fRadius;
};

#pragma pack(pop)