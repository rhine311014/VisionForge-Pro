#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"

class scSegment3D;
class scPlane;

#pragma pack(push,8)

// 三角形
// 由三个三维点定义
class SVGEOMETRICS_API_3D scTriangle3D: public scShape3D
{
public:
	scTriangle3D();
	scTriangle3D(const scPoint3D& p1,const scPoint3D& p2,const scPoint3D& p3);
	scTriangle3D(const scTriangle3D& triangle);

	virtual ~scTriangle3D();

	scTriangle3D& operator=(const scTriangle3D& triangle);
	bool operator==(const scTriangle3D& triangle)const;
	bool operator!=(const scTriangle3D& triangle)const;

	scPoint3D operator[](int nIndex)const;
	scPoint3D& operator[](int nIndex);

	virtual float GetPerimeter() const; 
	virtual float GetArea() const;
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return eTriangle3D;}

	virtual scSphere GetExternalSphere()const;

public:
	scPoint3D at(int nIndex)const;
	scPoint3D& at(int nIndex);

	void Set(const scPoint3D& p1,const scPoint3D& p2,const scPoint3D& p3);
	void Get(scPoint3D& p1,scPoint3D& p2,scPoint3D& p3)const;

	void SetP1(const scPoint3D& p1) {m_Point[0] = p1;}
	void SetP2(const scPoint3D& p2) {m_Point[1] = p2;}
	void SetP3(const scPoint3D& p3) {m_Point[2] = p3;}

	scPoint3D GetP1()const {return m_Point[0];}
	scPoint3D GetP2()const {return m_Point[1];}
	scPoint3D GetP3()const {return m_Point[2];}

	// 获取三角形的法向量
	scVector3D GetNormal() const; 

	// 获取除第n个点之外的两个点组成的线段
	scSegment3D GetSegment3D(int n)const;

	// 获取三角形所在平面
	scPlane GetPlane()const;

	// 获取三角形重心
	scPoint3D GetGravityCenter()const;

	// 获取三角形垂心
	scPoint3D GetOrthoCenter()const;

private:
	scPoint3D m_Point[3];
};

#pragma pack(pop)