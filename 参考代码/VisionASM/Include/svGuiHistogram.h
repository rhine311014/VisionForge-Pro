// svGuiHistogram.h

// ---------------------------------------------------------------------
//                           直方图交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svRect.h"

#pragma pack(push,8)


class SVGUI_API scGuiHistogram :public scGuiGraphic 
{
public:
	scGuiHistogram();
	virtual ~scGuiHistogram();

	virtual GraphicType GetType() const {return eGuiHistogram;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,		
		hitHistogram = 0, // 直方图
		hitMeanGrey = 1	  // 灰度均值												
	};
public:
	// 设置直方图数据
	void SetHistogram(const svStd vector<long>& histogram); 

	// 设置直方图绘制区域(矩形)
	// 矩形绘制区域的左上角（dX,dY）,宽dWidth和高dHeight
	void SetDrawRect(double dX, double dY, double dWidth, double dHeight);

	// 获取直方图绘制区域(矩形)
	void GetDrawRect(double& dX, double& dY, double& dWidth, double& dHeight) const;

	// 设置直方图绘制区域(矩形)，矩形区域为rect
	void SetDrawRect(const scRect& rect);

	// 获取直方图绘制区域(矩形)
	scRect GetDrawRect() const;

	//设置/获取X、Y轴的刻度数量
	void SetAxisScaleNum(int nXNum,int nYNum);
	void GetAxisScaleNum(int& nXNum,int& nYNum)const;

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

	void  InitTipText();
	void  LoadCursor();
	int   HitTestHandles(CPoint point) const;
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;	
	CRect GetBoundRect() const;
	CRect GetHandleRect(TrackerHit nHandle) const;
	void DrawAxes(CDC* pDC)const;



protected:

	void*		m_pLShape;	
	TrackerHit  m_hitResult;
	static HCURSOR g_hCursor;  // handles to the cursors

	int m_nXAxisScaleNum;
	int m_nYAxisScaleNum;
};

#pragma pack(pop)