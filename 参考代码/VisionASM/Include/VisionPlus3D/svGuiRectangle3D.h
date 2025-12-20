#pragma once

#include "svGuiDef3D.h"
#include "svGuiGraphic3D.h"
#include "svRectangle3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiRectangle3D: public scGuiGraphic3D
{
public:
	scGuiRectangle3D();
	virtual ~scGuiRectangle3D();

	virtual GuiGraphicType3D GetType()const { return eGuiRectangle3D;}

	enum GuiRectangle3DDOF
	{
		eGuiNone = 0,
		eGuiEndPoint = 1,
		eGuiMove = 2,
		eGuiRotate = 4
	};

	enum TrackerHit
	{
		eHitNone = 0,
		eHitEndPoint = 1,
		eHitRectangle = 2
	};

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Rectangle;}

	void SetRectangle3D(const scRectangle3D& rect);
	scRectangle3D GetRectangle3D()const {return m_Rectangle;}

	void SetRectangle3DParam(const scPoint3D& center,const scVector3D& u,const scVector3D& v,const scVector3D& w,float uLength,float vLenght);
	void GetRectangle3DParam(scPoint3D& center,scVector3D& u,scVector3D& v,scVector3D& w,float& uLength,float& vLenght)const;

	void SetRectangle3DCenter(const scPoint3D& p);
	scPoint3D GetRectangle3DCenter()const {return m_Rectangle.GetCenter();}

	void SetRectangle3DAxis(const scVector3D& u,const scVector3D& v,const scVector3D& w);
	void GetRectangle3DAxis(scVector3D& u,scVector3D& v,scVector3D& w)const;

	scVector3D GetRectangle3DAxisU()const {return m_Rectangle.GetAxisU();}
	scVector3D GetRectangle3DAxisV()const {return m_Rectangle.GetAxisV();}
	scVector3D GetRectangle3DAxisW()const {return m_Rectangle.GetAxisW();}

	void SetRectangle3DLength(float uLength,float vLength);
	void GetRectangle3DLength(float& uLength,float& vLength)const;

	void SetRectangle3DLengthU(float uLength);
	void SetRectangle3DLengthV(float vLength);

	float GetRectangle3DLengthU()const {return m_Rectangle.GetLengthU();}
	float GetRectangle3DLengthV()const {return m_Rectangle.GetLengthV();}

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
	scRectangle3D m_Rectangle;
	GuiGraphicFillType m_DrawType;
	int m_nEndPointDragIndex;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)