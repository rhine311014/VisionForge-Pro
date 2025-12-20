#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"

class scTriangle3D;

#pragma pack(push,8)

// 四面体
// 由四个三维点定义
class SVGEOMETRICS_API_3D scTetrahedron:public scShape3D
{
public:
	scTetrahedron();
	scTetrahedron(const scPoint3D& p1,const scPoint3D& p2,const scPoint3D& p3,const scPoint3D& p4);
	scTetrahedron(const scTetrahedron& tetrahedron);

	virtual ~scTetrahedron();

	scTetrahedron& operator=(const scTetrahedron& tetrahedron);

	bool operator==(const scTetrahedron& tetrahedron)const;
	bool operator!=(const scTetrahedron& tetrahedron)const;

	virtual float GetPerimeter() const;
	virtual float GetArea() const;
	virtual float GetVolume() const;

	virtual bool IsFinite() const {return true;}
	virtual ShapeType3D GetType3D()const { return eTetrahedron;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scPoint3D& p1,const scPoint3D& p2,const scPoint3D& p3,const scPoint3D& p4);
	void Get(scPoint3D& p1,scPoint3D& p2,scPoint3D& p3,scPoint3D& p4)const;

	void SetP1(const scPoint3D& p1) {m_Point[0] = p1;}
	scPoint3D GetP1()const {return m_Point[0];}

	void SetP2(const scPoint3D& p2) {m_Point[1] = p2;}
	scPoint3D GetP2()const {return m_Point[1];}

	void SetP3(const scPoint3D& p3) {m_Point[2] = p3;}
	scPoint3D GetP3()const {return m_Point[2];}

	void SetP4(const scPoint3D& p4) {m_Point[3] = p4;}
	scPoint3D GetP4()const {return m_Point[3];}

	// 除了第n个点之外三个点组成的三角形
	scTriangle3D GetTriangle3D(int n) const;

private:
	scPoint3D m_Point[4];
};

#pragma pack(pop)