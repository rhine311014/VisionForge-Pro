#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svEllipsoid.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiEllipsoid: public scGuiGraphic3D
{
public:
	scGuiEllipsoid();
	virtual ~scGuiEllipsoid();

	virtual GuiGraphicType3D GetType()const { return eGuiEllipsoid;}

	enum GuiEllipsoidDOF
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
		eHitEllipsoid = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Ellipsoid;}

	void SetEllipsoid(const scEllipsoid& ellipsoid);
	scEllipsoid GetEllipsoid()const {return m_Ellipsoid;}

	void SetEllipsoidCenter(const scPoint3D& center);
	scPoint3D GetEllipsoidCenter()const {return m_Ellipsoid.GetCenter();}

	void SetEllipsoidAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetEllipsoidAxis(scVector3D& u,scVector3D& v,scVector3D& w) {m_Ellipsoid.GetAxis(u,v,w);}

	scVector3D GetEllipsoidAxisU()const {return m_Ellipsoid.GetAxisU();}
	scVector3D GetEllipsoidAxisV()const {return m_Ellipsoid.GetAxisV();}
	scVector3D GetEllipsoidAxisW()const {return m_Ellipsoid.GetAxisW();}

	void SetEllipsoidRadiusU(float uRadius);
	float GetEllipsoidRadiusU()const {return m_Ellipsoid.GetRadiusU();}

	void SetEllipsoidRadiusV(float vRadius);
	float GetEllipsoidRadiusV()const {return m_Ellipsoid.GetRadiusV();}

	void SetEllipsoidRadiusW(float wRadius);
	float GetEllipsoidRadiusW()const {return m_Ellipsoid.GetRadiusW();}

	void SetRadiusWSampleNum(int num);
	int GetRadiusWSampleNum()const {return m_nRadiusWSampleNum;}

	void SetEllipseSampleNum(int num);
	int GetEllipseSampleNum()const {return m_nEllipseSampleNum;}

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
	scEllipsoid m_Ellipsoid;
	int m_nRadiusWSampleNum;
	int m_nEllipseSampleNum;
	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)