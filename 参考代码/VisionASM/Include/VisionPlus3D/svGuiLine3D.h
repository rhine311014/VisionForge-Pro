#pragma once

#include "svGuiGraphic3D.h"
#include "svLine3D.h"

#pragma pack(push,8)

class SVGEOMETRICSGUI_API_3D scGuiLine3D: public scGuiGraphic3D
{
public:
	scGuiLine3D();
	virtual ~scGuiLine3D();

	virtual GuiGraphicType3D GetType()const { return eGuiLine3D;}

	enum eGuiLine3DDOF
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

	virtual scShape3D* GetShape() {return (scShape3D*)&m_Line;}

	virtual void SetViewFrustumMaxLength(float length);

	void SetLine3D(const scLine3D& line);
	scLine3D GetLine3D() const;

	void SetLine3DParam(const scPoint3D& p,const scVector3D& vec);
	void GetLine3DParam(scPoint3D& p,scVector3D& vec)const;

	void SetLine3DOrigion(const scPoint3D& p);
	scPoint3D GetLine3DOrigion()const;

	void SetLine3DDirection(const scVector3D& vec);
	scVector3D GetLine3DDirection()const;

	// 鼠标响应测试
	// disZ为几何图形到窗口平面的最短距离
	virtual bool HitTest(const scPoint3D& pt1,const scPoint3D& pt2,float& disZ);

	// 图形移动
	virtual void Move(const scPoint3D& ptStart,const scPoint3D& ptStop);

	// 图形旋转
	virtual void Rotate(const scVector3D& axis,float angle);

private:
	scLine3D m_Line;
	float m_fDrawMaxLength;

	unsigned int m_nHandleArrayObj;
	unsigned int m_nHandleVertexObj;
};

#pragma pack(pop)