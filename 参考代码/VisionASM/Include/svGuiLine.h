// svGuiLine.h

// ---------------------------------------------------------------------
//                          直线交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svLine.h"

#pragma pack(push,8)

class SVGUI_API scGuiLine : public scGuiGraphic  
{
public:
	scGuiLine();
	virtual ~scGuiLine();

	virtual GraphicType GetType() const {return eGuiLine;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//		
	//
	//
	//			---------------*-----------------Line
	//						  Rotation
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
	enum GuiLineDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度	
		eGuiDOFRotation	= 1,	// 可改变旋转角度
		eGuiDOFPosition = 2,    // 可改变位置
		eGuiDOFAll		= eGuiDOFRotation|eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);
	
	// 设置直线参数
	void	SetXYRotation(double dX,				// 直线参考点X坐标
						  double dY,				// 直线参考点Y坐标
						  double dRotation);		// 直线旋转角

	void	GetXYRotation(double& dX,				// 直线参考点X坐标
						  double& dY,				// 直线参考点Y坐标
						  double& dRotation) const;	// 直线旋转角

	// 设置直线参数
	void	SetXYRotation(const sc2Vector& vRef,	// 直线参考点坐标
						  const scRadian& rRotation);// 直线旋转角

	void	GetXYRotation(sc2Vector& vRef,			// 直线参考点坐标
						  scRadian& rRotation) const;// 直线旋转角

	void	GetXY(double& dX, double& dY) const;	// 获取直线参考点X、Y坐标
	sc2Vector GetXY() const;						// 获取直线参考点坐标
	scRadian  GetRotation() const;					// 获取直线旋转角

	void	SetLine(const scLine& line);			// 设置直线
	scLine  GetLine() const;						// 获取直线

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
	BOOL  IsOnLine(double dX, double dY, double dRotation, CPoint point) const;

protected:
	void*	m_pLShape;					// a pointer to the object
	void*	m_pLShapeDrag;

	scLine*	m_pLine;
	scLine* m_pLineDrag;
	TrackerHit m_hitResult;
	static HCURSOR g_hCursor[2];		// handles to the cursors
	
	DWORD		m_DOF;				    // 自由度
};

#pragma pack(pop)
