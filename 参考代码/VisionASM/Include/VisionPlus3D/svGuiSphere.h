#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svSphere.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiSphere: public scGuiGraphic3D
{
public:
	scGuiSphere();
	virtual ~scGuiSphere();

	virtual GuiGraphicType3D GetType()const { return eGuiSphere;}

	enum GuiSphereDOF
	{
		eGuiNone = 0,
		eGuiRadius = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitRadius = 1,
		eHitSphere = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Shpere;}

	void SetSphere(const scSphere& sphere);
	scSphere GetSphere()const {return m_Shpere;}

	void SetSphereParam(const scPoint3D& center,float radius);
	void GetSphereParam(scPoint3D& center,float& radius)const;

	void SetSphereCenter(const scPoint3D& center);
	scPoint3D GetSphereCenter()const {return m_Shpere.GetCenter();}

	void SetSphereRadius(float radius);
	float GetSphereRadius()const {return m_Shpere.GetRadius();}

	void SetFillType(GuiGraphicFillType fillType) {m_DrawType = fillType;}
	GuiGraphicFillType GetFillType()const {return m_DrawType;}


	void SetRadiusSampleNum(int num);
	int GetRadiusSampleNum()const {return m_nRadiusSampleNum;}

	void SetCircleSampleNum(int num);
	int GetCircleSampleNum()const {return m_nCircleSampleNum;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scSphere m_Shpere;
	int m_nRadiusSampleNum;
	int m_nCircleSampleNum;
	GuiGraphicFillType m_DrawType;

	scVector3D m_AxisU;
	scVector3D m_AxisV;
	scVector3D m_AxisW;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)
