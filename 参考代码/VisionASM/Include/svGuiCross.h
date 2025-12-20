// svGuiCross.h


// ---------------------------------------------------------------------
//                           十字标记交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svVector.h"
#include "svCrossPoint.h"

#pragma pack(push,8)


class SVGUI_API scGuiCross : public scGuiGraphic
{
public:
	scGuiCross();
	virtual ~scGuiCross();

	virtual GraphicType GetType() const {return eGuiCross;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//		
	//					  |
	//					  |
	//			      ----*----
	//					  |center
	//					  |	
	//
	//
	//
	/////////////////////////////////////////////////////////////////////////////
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,	// 未击中	
		hitCenter  = 0, 	// 中心点
		hitHorz,			// 水平线段
		hitVert				// 垂直线段
	};

	// 自由度
	enum GuiCrossDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度	
		eGuiDOFPosition	= 1,	// 可改变位置	
		eGuiDOFAll		= eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// Operations
	void SetCrossPoint(scCrossPoint cp);

	void SetCenterXYWidthHeight(double dCenterX,				// 中心点的X坐标
								double dCenterY,				// 中心点的Y坐标
								double dWidth  = 16,			// 宽度
								double dHeight = 16);			// 高度
	void GetCenterXYWidthHeight(double& dCenterX,				// 中心点的X坐标
								double& dCenterY,				// 中心点的Y坐标
								double& dWidth,					// 宽度
								double& dHeight)const;			// 高度
	void GetCenterXY(double& dCenterX,double& dCenterY)const;	// 获取中心点的X坐标和Y坐标
	void GetExtent(double& dWidth, double& dHeight) const;		// 获取宽度和高度

	void SetCenterExtent(const sc2Vector& vCenter,				// 中心点
		const sc2Vector& vSize = sc2Vector(16,16));				// 尺寸（宽度和高度）
	sc2Vector GetCenter() const;								// 获取中心点
	sc2Vector GetExtent() const;								// 获取尺寸（宽度和高度）

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

	// helper for Implementation
protected:	

	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;	

	void  LoadCursor();
	void  InitTipText();
	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;	

protected:
	sc2Vector	m_vCenter;				// 
	sc2Vector	m_vCenterDrag;			// 
	sc2Vector	m_vSize;				// 
	sc2Vector	m_vSizeDrag;			// 
	TrackerHit  m_hitResult;			// 
	static HCURSOR g_hCursor;			// handles to the cursors

	DWORD		m_DOF;				    // 自由度
};

#pragma pack(pop)
