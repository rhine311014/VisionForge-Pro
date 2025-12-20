#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svEllipse3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiEllipse3D: public scGuiGraphic3D
{
public:
	scGuiEllipse3D();
	virtual ~scGuiEllipse3D();

	virtual GuiGraphicType3D GetType()const { return eGuiEllipse3D;}

	enum GuiEllipse3DDOF
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
		eHitEllipse = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Ellipse;}

	void SetEllipse3D(const scEllipse3D& ellispe);
	scEllipse3D GetEllipse3D()const {return m_Ellipse;}

	void SetEllipse3DCenter(const scPoint3D& p);
	scPoint3D GetEllipse3DCenter()const {return m_Ellipse.GetCenter();}

	void SetEllipse3DAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetEllipse3DAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetEllipse3DAxisU()const {return m_Ellipse.GetAxisU();}
	scVector3D GetEllipse3DAxisV()const {return m_Ellipse.GetAxisV();}
	scVector3D GetEllipse3DAxisW()const {return m_Ellipse.GetAxisW();}

	void SetEllipse3DRadiusU(float uLength);
	float GetEllipse3DRadiusU()const {return m_Ellipse.GetRadiusU();}

	void SetEllipse3DRadiusV(float vLength);
	float GetEllipse3DRadiusV()const {return m_Ellipse.GetRadiusV();}

	void SetSampleNum(int nSampleNum);
	int  GetSampleNum()const {return m_nSampleNum;}

	void SetFillType(GuiGraphicFillType fillType);
	GuiGraphicFillType GetFillType()const {return m_DrawType;}

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scEllipse3D m_Ellipse;
	GuiGraphicFillType m_DrawType;
	int m_nSampleNum;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)