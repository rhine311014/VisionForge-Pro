// svGuiLineSeg.h

// ---------------------------------------------------------------------
//                          线段交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svLineSeg.h"

#pragma pack(push,8)


class SVGUI_API scGuiLineSeg : public scGuiGraphic   
{
public:
	scGuiLineSeg();
	virtual ~scGuiLineSeg();

	virtual GraphicType GetType() const {return eGuiLineSeg;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//		
	//
	//
	//					 Start *---------------------* End
	//	
	//
	//
	//
	/////////////////////////////////////////////////////////////////////////////
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,	// 未击中	
		hitStart = 0, 		// 起始点
		hitEnd,				// 终点
		hitLine				// 线
	};

	// 自由度
	enum GuiLineSegDOF
	{
		eGuiDOFNone	    = 0,	// 禁止所有的自由度	
		eGuiDOFStart	= 1,	// 可改变起始点
		eGuiDOFEnd      = 2,    // 可改变终止点
		eGuiDOFPosition = 4,    // 可改变位置
		eGuiDOFAll		= eGuiDOFStart|eGuiDOFEnd|eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置线段参数
	void SetStartXYEndXY(double dStartX,			// 起始点X坐标
						 double dStartY,			// 起始点Y坐标
						 double dEndX,				// 终点X坐标
						 double dEndY);				// 终点Y坐标
	// 获取线段参数
	void GetStartXYEndXY(double& dStartX,			// 起始点X坐标
						 double& dStartY,			// 起始点Y坐标
						 double& dEndX,				// 终点X坐标
						 double& dEndY) const;		// 终点Y坐标	
	// 获取起点XY坐标
	void	GetStartXY(double& dStartX, double& dStartY) const;
	// 获取终点XY坐标
	void	GetEndXY(double& dEndX, double& dEndY) const;

	//	设置线段参数
	void SetStartXYEndXY(const sc2Vector& vStart, 	// 起始点
						 const sc2Vector& vEnd);	// 终点

	// 获取线段参数
	void GetStartXYEndXY(sc2Vector& vStart, 		// 起始点
						 sc2Vector& vEnd) const;			// 终点

	
	void	  SetLineSeg(const scLineSeg& lineSeg);	// 设置线段
	scLineSeg GetLineSeg() const;					// 获取线段
	sc2Vector GetStartXY() const;					// 获取起点
	sc2Vector GetEndXY() const;						// 获取终点
	scRadian  GetRotation() const;					// 获取旋转角度

	void	SetLineSegArrowVisible(BOOL bVisible);						// 设置线段箭头是否可见
	BOOL	IsLineSegArrowVisible()				const;					// 获取线段箭头是否可见

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
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

protected:
	void*	m_pLShape;					// a pointer to the object
	void*	m_pLShapeDrag;

	scLineSeg* m_pLineSeg;
	scLineSeg* m_plineSegDrag;
	TrackerHit m_hitResult;
	static HCURSOR g_hCursor[3];		// handles to the cursors
	
	BOOL	m_bLineSegArrowVisible;		// whether the LineSegArrow is visible or not

	DWORD	   m_DOF;				    // 自由度
};

#pragma pack(pop)
