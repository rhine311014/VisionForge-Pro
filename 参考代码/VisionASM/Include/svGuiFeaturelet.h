// svGuiFeaturelet.h

// ---------------------------------------------------------------------
//                            轮廓特征显示控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svFeature.h"

#pragma pack(push,8)

class SVGUI_API scGuiFeaturelet: public scGuiGraphic 
{
public:
	scGuiFeaturelet();
	virtual ~scGuiFeaturelet();

	virtual GraphicType GetType() const {return eGuiFeaturelet;};

public:	

	// 设置轮廓特征
	void SetFeaturelet(const scFeatureletChainSet& featurelet);

	// 获取轮廓特征
	scFeatureletChainSet GetFeaturelet() const;

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
	scFeatureletChainSet*	m_pFeatureletSet;
	scFeatureletChainSet*	m_pFeaturelet;	// a pointer to the object of the rectangle
	static HCURSOR g_hCursor;				// handles to the cursors
	int     m_nHitTest;

	CRITICAL_SECTION                   m_csResourceStatus;
};

#pragma pack(pop)
