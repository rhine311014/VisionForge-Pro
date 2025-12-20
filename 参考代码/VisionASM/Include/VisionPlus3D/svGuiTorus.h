#pragma once


#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svTorus.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiTorus: public scGuiGraphic3D
{
public:
	scGuiTorus();
	virtual ~scGuiTorus();

	virtual GuiGraphicType3D GetType()const { return eGuiTorus;}

	enum GuiTorusDOF
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
		eHitTorus = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Torus;}

	void SetTorus(const scTorus& torus);
	scTorus GetTorus()const {return m_Torus;}

	void SetTorusCenter(const scPoint3D& center);
	scPoint3D GetTorusCenter()const {return m_Torus.GetCenter();}

	void SetTorusAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetTorusAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetTorusAxisU()const {return m_Torus.GetAxisU();}
	scVector3D GetTorusAxisV()const {return m_Torus.GetAxisV();}
	scVector3D GetTorusAxisW()const {return m_Torus.GetAxisW();}

	void SetTorusRadiusOuter(float radius);
	float GetTorusRadiusOuter()const {return m_Torus.GetRadiusOuter();}

	void SetTorusRadiusInner(float radius);
	float GetTorusRadiusInner()const {return m_Torus.GetRadiusInner();}

	void SetTorusInnerSampleNum(int num);
	int GetTorusInnerSampleNum()const {return m_nInnerSampleNum;}

	void SetTorusOuterSampleNum(int num);
	int GetTorusOuterSampleNum()const {return m_nOuterSampleNum;}

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
	scTorus m_Torus;
	GuiGraphicFillType m_DrawType;

	int m_nInnerSampleNum;
	int m_nOuterSampleNum;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)