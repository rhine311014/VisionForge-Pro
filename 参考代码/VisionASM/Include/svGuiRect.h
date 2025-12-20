// svGuiRect.h

// ---------------------------------------------------------------------
//                        矩形交互图形
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svRect.h"

#pragma pack(push,8)



class SVGUI_API scGuiRect : public scGuiGraphic
{
public:
	scGuiRect();
	virtual ~scGuiRect();

	virtual GraphicType GetType() const {return eGuiRect;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//	 TopLeft	  Top		TopRight  
	//		*---------------------*
	//		|					  |
	//		|					  |
	//	Left|		   *center	  | Right
	//		|					  |
	//		|					  |
	//		*---------------------*
	//	BottomLeft	  Bottom	BottomRight
	/////////////////////////////////////////////////////////////////////////////

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		//左上角、右上角、右下角、左下角
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		//上边中心、右边中心、下边中心、左边中心
		hitTopCenter = 4, hitRightCenter = 5, hitBottomCenter = 6, hitLeftCenter = 7,
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 8, hitCenterYSeg = 9,
		//上边、右边、下边、左边
		hitSideTop = 10, hitSideRight = 11, hitSideBottom = 12, hitSideLeft = 13									
	};

	// 自由度
	enum GuiRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置		
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	// dCenterX:中心点X坐标, dCenterY: 中心点Y坐标, dWidth: 宽, dHeight:高 
	void SetCenterWidthHeight(double dCenterX, double dCenterY, double dWidth, double dHeight);		
	void GetCenterWidthHeight(double& dCenterX, double& dCenterY, double& dWidth, double& dHeight) const;	

	// dX:左上点X坐标, dY: 左上点Y坐标, dWidth: 宽, dHeight:高 
	void SetXYWidthHeight(double dX, double dY, double dWidth, double dHeight);	
	void GetXYWidthHeight(double& dX, double& dY, double& dWidth, double& dHeight) const;

	// vCenterX:中心点, vSize:尺寸 
	void SetCenterWidthHeight(const sc2Vector& vCenter, const sc2Vector& vSize);		
	void GetCenterWidthHeight(sc2Vector& vCenter, sc2Vector& vSize) const;	

	// vUL:左上点，vSize:尺寸 
	void SetXYWidthHeight(const sc2Vector& vUL, const sc2Vector& vSize);	
	void GetXYWidthHeight(sc2Vector& vUL, sc2Vector& vSize) const; 

	// 用矩形scRect设置
	void   SetRect(const scRect& rect); 
	scRect GetRect() const; 

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

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

	void	SetFillEnabled(BOOL bEnable);								// 设置是否允许填充
	BOOL	IsFillEnabled() const;										// 获取是否允许填充

	void    SetFillColor( COLORREF color);								// 设置填充的颜色
	COLORREF GetFillColor() const;										// 获取填充的颜色

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

	void  LoadCursor();
	void  InitTipText();
	CRect GetBoundRect(void* pLShape) const;
	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;	

protected:
	scRect*	m_pRect;
	scRect* m_pRectDrag;
	TrackerHit m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[14];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 
};

#pragma pack(pop)
