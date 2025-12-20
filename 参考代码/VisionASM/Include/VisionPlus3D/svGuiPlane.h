#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svPlane.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiPlane:public scGuiGraphic3D
{
public:
	scGuiPlane();
	virtual ~scGuiPlane();

	virtual GuiGraphicType3D GetType()const { return eGuiPlane;}

	enum GuiPlaneDOF
	{
		eGuiNone = 0,
		eGuiMove = 1,
		eGuiRotate = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Plane;}

	virtual void SetViewFrustumMaxLength(float length);

	void EnableLimit(bool bEnble);
	bool IsLimit()const {return m_bLimit;}

	void SetLimitParam(int sampleNum,float radius);
	void GetLimitParam(int& sampleNum,float& radius)const;

	void SetPlane(const scPlane& plane);
	scPlane GetPlane()const {return m_Plane;}

	void SetPlaneNormal(const scVector3D& normal);
	scVector3D GetPlaneNormal()const {return m_Plane.GetNormal();}

	void SetPlaneConstant(float constant);
	float GetPlaneConstant()const {return m_Plane.GetConstant();}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scPlane m_Plane;
	float m_fDrawMaxLength;

	bool m_bLimit;

	int m_nLimitSampleNum;
	float m_fLimitRadius;
};

#pragma pack(pop)