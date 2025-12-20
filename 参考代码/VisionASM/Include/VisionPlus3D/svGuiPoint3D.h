#pragma once

#include "svPoint3D.h"
#include "svGuiGraphic3D.h"

class SVGEOMETRICSGUI_API_3D scGuiPoint3D : public scGuiGraphic3D
{
public:
	scGuiPoint3D();
	virtual ~scGuiPoint3D();

	virtual GuiGraphicType3D GetType()const { return eGuiPoint3D;}  // 获取图形类型

public:
	virtual void Draw(unsigned int nProgramID) const; // 绘制
	virtual void GenGpuData();
	virtual void TransDataToGPU();
	virtual void ReleaseGpuData();

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Point3D;}

	void SetPoint3DParam(float x,float y,float z);
	void GetPoint3DParam(float& x,float& y,float& z);

	void SetPoint3DX(float x);
	void SetPoint3DY(float y);
	void SetPoint3DZ(float z);

	float GetPoint3DX()const {return m_Point3D[0];}
	float GetPoint3DY()const {return m_Point3D[1];}
	float GetPoint3DZ()const {return m_Point3D[2];}

	void SetPoint3D(scPoint3D point);
	scPoint3D GetPoint3D()const;

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

private:
	scPoint3D m_Point3D;
};
