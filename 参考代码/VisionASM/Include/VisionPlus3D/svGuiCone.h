#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svCone.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiCone : public scGuiGraphic3D
{
public:
	scGuiCone();
	virtual ~scGuiCone();

	virtual GuiGraphicType3D GetType()const { return eGuiCone;}

	enum GuiConeDOF
	{
		eGuiNone = 0,
		eGuiAngleHeight = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitAngleHeight = 1,
		eHitCone = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Cone;}

	void SetCone(const scCone& cone);
	scCone GetCone()const {return m_Cone;}

	void SetConeOrigion(const scPoint3D& origion);
	scPoint3D GetConeOrigion()const {return m_Cone.GetOrigion();}

	void SetConeAxis(const scVector3D& axis);
	scVector3D GetConeAxis()const {return m_Cone.GetAxis();}

	void SetConeDegree(float degree);
	float GetConeDegree()const {return m_Cone.GetDegree();}

	void SetConeHeightMin(float hMin);
	float GetConeHeightMin()const {return m_Cone.GetHeightMin();}

	void SetConeHeightMax(float hMax);
	float GetConeHeightMax()const {return m_Cone.GetHeightMax();}

	void SetSampleNum(int nSampleNum);
	int  GetSampleNum()const {return m_nSampleNum;}

	// 是否封闭,默认封闭
	void SetClosed(bool bclosed);
	bool IsClosed()const {return m_bClosed;}

	void SetFillType(GuiGraphicFillType fillType) {m_DrawType = fillType;}
	GuiGraphicFillType GetFillType()const {return m_DrawType;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scCone m_Cone;
	int    m_nSampleNum;
	bool   m_bClosed;

	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	scPoint3D m_ptOld;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)