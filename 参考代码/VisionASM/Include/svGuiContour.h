// svGuiContour.h

// ---------------------------------------------------------------------
//                            Blob轮廓交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svBlobSceneDescription.h"

#pragma pack(push,8)

class SVGUI_API scGuiContour: public scGuiGraphic 
{
public:
	scGuiContour();
	virtual ~scGuiContour();

	virtual GraphicType GetType() const {return eGuiContour;};

public:	
	
	// 设置Blob边界的边界	
	void SetContourBoundary(scBlob * const * pscBlob, int nBlobNum,
		bool bDrawContour = true, bool bDrawMassCenter = true);	

	virtual BOOL BoundingRect(CRect& rect) const;

	CString GetTipText()	const;
	CString	GetTipText(int nHandle) const;
	CString	GetTipText(CPoint point) const;		
	
	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	int  HitTest(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	void SetHitHandle(int nHitHandle);	
	int GetHitHandle(){return m_nHitTest;}

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
	int   HitTestHandles(CPoint point) const;
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;


protected:

	void*	m_pLShape;		    // a pointer to the object of the rectangle
	static HCURSOR g_hCursor;	// handles to the cursors
	int     m_nHitTest;
	bool    m_bDrawContour;
	bool    m_bDrawMassCenterXY;

	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色
};

#pragma pack(pop)
