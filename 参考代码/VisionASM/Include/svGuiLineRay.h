// svGuiRay.h

// ---------------------------------------------------------------------
//                          射线交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svLineRay.h"

#pragma pack(push,8)

class SVGUI_API scGuiLineRay : public scGuiGraphic
{
public:
	scGuiLineRay();
	virtual ~scGuiLineRay();

	virtual GraphicType GetType() const {return eGuiLineRay;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//		
	//
	//                   start point
	//			              *-----------------Ray
	//						        Rotation
	//
	//
	//
	/////////////////////////////////////////////////////////////////////////////
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,	// 未击中	
		hitRotation = 0, 	// 起始点
		hitLine				// 线
	};

	// 自由度
	enum GuiRayDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度	
		eGuiDOFRotation	= 1,	// 可改变旋转角度
		eGuiDOFPosition = 2,    // 可改变位置
		eGuiDOFAll		= eGuiDOFRotation|eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置射线参数
	void	SetXYRotation(double dX,				// 射线起始点X坐标
		                  double dY,				// 射线起始点Y坐标
		                  double dRotation);		// 射线旋转角

	void	GetXYRotation(double& dX,				// 射线起始点X坐标
		                  double& dY,				// 射线起始点Y坐标
		                  double& dRotation) const;	// 射线旋转角

	// 设置射线参数
	void	SetXYRotation(const sc2Vector& vRef,	 // 射线起始点坐标
		                  const scRadian& rRotation);// 射线旋转角

	void	GetXYRotation(sc2Vector& vRef,			 // 射线起始点坐标
		                  scRadian& rRotation) const;// 射线旋转角

	void	GetXY(double& dX, double& dY) const;	// 获取射线起始点X、Y坐标
	sc2Vector GetXY() const;						// 获取射线起始点坐标
	scRadian  GetRotation() const;					// 获取射线旋转角

	void	SetRay(const scLineRay& ray);			// 设置射线
	scLineRay   GetRay() const;						// 获取射线

	virtual BOOL BoundingRect(CRect& rect) const;

	CString GetTipText()	const;
	CString	GetTipText(int nHandle) const;
	CString	GetTipText(CPoint point) const;	

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	int  HitTest(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	BOOL DragStart(CPoint point);
	void DragAnimate(CPoint ptStart, CPoint ptStop);
	void DragStop();
	void DragCancel();

protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

	void  LoadCursor();
	void  InitTipText();
	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;	

protected:
	scLineRay*	m_pLShape;					// a pointer to the object
	scLineRay*	m_pLShapeDrag;

	TrackerHit m_hitResult;
	static HCURSOR g_hCursor[2];		// handles to the cursors

	DWORD		m_DOF;				    // 自由度
};

#pragma pack(pop)