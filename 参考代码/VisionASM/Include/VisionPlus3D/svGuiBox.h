#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svBox.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiBox:public scGuiGraphic3D
{
public:
	scGuiBox();
	virtual ~scGuiBox();

	virtual GuiGraphicType3D GetType()const { return eGuiBox;}

	enum GuiBoxDOF
	{
		eGuiNone = 0,
		eGuiMove = 1,
		eGuiCorner = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitCorner = 1,
		eHitBox = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Box;}

	void SetBox(const scBox& box);
	scBox GetBox()const {return m_Box;}

	void SetBoxCenter(const scPoint3D& center);
	scPoint3D GetBoxCenter() const {return m_Box.GetCenter();}

	void SetBoxAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetBoxAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetBoxAxisU() const {return m_Box.GetAxisU();}
	scVector3D GetBoxAxisV() const {return m_Box.GetAxisV();}
	scVector3D GetBoxAxisW() const {return m_Box.GetAxisW();}

	void SetBoxLengthU(float uLength);
	float GetBoxLengthU()const {return m_Box.GetLengthU();}

	void SetBoxLengthV(float vLength);
	float GetBoxLengthV()const {return m_Box.GetLengthV();}

	void SetBoxLengthW(float wLength);
	float GetBoxLengthW()const {return m_Box.GetLengthW();}

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
	scBox m_Box;
	GuiGraphicFillType m_DrawType;

	int m_nDragIndex;
	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)