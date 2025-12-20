#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svEllipseAnnulus3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiEllipseAnnulus3D: public scGuiGraphic3D
{
public:
	scGuiEllipseAnnulus3D();
	virtual ~scGuiEllipseAnnulus3D();

	virtual GuiGraphicType3D GetType()const { return eGuiEllipseAnnulus3D;}

	enum GuiEllipseAnnulus3DDOF
	{
		eGuiNone = 0,
		eGuiRadius = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitInnerRadius = 1,
		eHitOuterRadius = 2,
		eHitEllipseAnnulus = 3
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_EllipseAnnulus;}

	void SetEllipseAnnulus3D(const scEllipseAnnulus3D& ellipseAnnulus);
	scEllipseAnnulus3D GetEllipseAnnulus3D()const {return m_EllipseAnnulus;}

	void SetEllipseAnnulus3DCenter(const scPoint3D& center);
	scPoint3D GetEllipseAnnulus3DCenter()const {return m_EllipseAnnulus.GetCenter();}

	void SetEllipseAnnulus3DAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetEllipseAnnulus3DAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetEllipseAnnulus3DAxisU()const {return m_EllipseAnnulus.GetAxisU();}
	scVector3D GetEllipseAnnulus3DAxisV()const {return m_EllipseAnnulus.GetAxisV();}
	scVector3D GetEllipseAnnulus3DAxisW()const {return m_EllipseAnnulus.GetAxisW();}

	void SetEllipseAnnulus3DRadiusU(float uRadius);
	float GetEllipseAnnulus3DRadiusU()const {return m_EllipseAnnulus.GetRadiusU();}

	void SetEllipseAnnulus3DRadiusV(float vRadius);
	float GetEllipseAnnulus3DRadiusV()const {return m_EllipseAnnulus.GetRadiusV();}

	void SetEllipseAnnulus3DRatio(float ratio);
	float GetEllipseAnnulus3DRatio()const {return m_EllipseAnnulus.GetRatio();}

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
	scEllipseAnnulus3D m_EllipseAnnulus;
	GuiGraphicFillType m_DrawType;
	int m_nSampleNum;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)