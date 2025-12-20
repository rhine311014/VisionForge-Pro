#pragma once

#include "svDef3D.h"
#include "svPoint3D.h"
#include "svVector3D.h"
#include "float.h"

#pragma pack(push,8)

class scZeroPlaneResult;
class cpPointCloudGenerateToolBase;

// 平面
// p*m_Normal = m_fConstant
class SVGEOMETRICS_API_3D scPlane: public scShape3D
{
public:
	scPlane();
	scPlane(const scVector3D& normal,float constant);                    // 由平面法向量、原点到平面的距离定义
	scPlane(const scVector3D& normal,const scPoint3D& p);                // 由平面法向量、平面上一点定义
	scPlane(const scVector3D& u,const scVector3D& v,float constant);     // 由平面上不平行的两个向量、原点到平面的距离定义
	scPlane(const scVector3D& u,const scVector3D& v,const scPoint3D& p); // 由平面上不平行的两个向量、平面上一点定义
	scPlane(const scPlane& plane);

	virtual ~scPlane();

	scPlane& operator=(const scPlane& plane);
	
	bool operator==(const scPlane& plane)const;
	bool operator!=(const scPlane& plane)const;

	virtual float GetPerimeter() const {return FLT_MAX;}
	virtual float GetArea() const {return FLT_MAX;}
	virtual float GetVolume() const {return 0.f;}

	virtual bool IsFinite() const;
	virtual ShapeType3D GetType3D()const { return ePlane;}

	virtual scSphere GetExternalSphere()const;

public:
	void Set(const scVector3D& normal,float constant);
	void Get(scVector3D& normal,float& constant)const;

	void SetNormal(const scVector3D& normal) {m_Normal = normal.Normalize();}
	scVector3D GetNormal()const {return m_Normal;}

	void SetConstant(float constant) {m_fConstant = constant;}
	float GetConstant()const {return m_fConstant;}

	// 转换为X倾斜、Y倾斜无量纲的基准面
	scZeroPlaneResult TransformZeroPlane();

	// 转换为X倾斜、Y倾斜量纲为mm/pix的基准面
	scZeroPlaneResult TransformZeroPlane(cpPointCloudGenerateToolBase* pPointCloudGenerateTool);

private:
	scVector3D m_Normal;
	float      m_fConstant;
};

#pragma pack(pop)