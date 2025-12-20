// svGuiCaliperResult.h

// ---------------------------------------------------------------------
//                            卡尺投影数据交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svCaliperResult.h"
#include "svImage.h"
#include "svRect.h"

#pragma pack(push,8)


class SVGUI_API scGuiCaliperResult : public scGuiGraphic
{
public:
	scGuiCaliperResult();
	virtual ~scGuiCaliperResult();

	virtual GraphicType GetType() const {return eGuiCaliperResult;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		// 投影曲线、滤波投影线、对比度阈值上边缘
		hitProject = 0, hitFilProj = 1, hitContrast1 = 2,
		// 对比度阈值下边缘、单边缘、另一个边缘
		hitContrast2 = 3,hitOneEdge = 4, hitTwoEdge1 = 5, hitTwoEdge2 = 6															
	};

public:

	// 设置单边结果
	void SetCaliperResult(scCaliperOneEdgeResult* pCapOneResult, int nNum, bool bVisible = true);

	// 设置双边结果
	void SetCaliperResult(scCaliperTwoEdgeResult* pCapTwoResult, int nNum, bool bVisible = true);

	// 设置采样后的图像
	void SetImage(const scImageGray& image, BOOL bVisible = TRUE);	

	// 设置卡尺结果数据
	// 结果数据包括：投影数据pdProj，投影滤波数据pdFilter，数据长度nProjLen，对比度阈值dContrast
	void SetProjection(double*  pdProj,	double* pdFilter, int nProjLen, double  dContrast);
	
	// 设置绘制位置
	void SetPos( double dOrgOffsetX,           // 投影曲线左上角x偏移
				double dOrgOffsetY,            // 投影曲线左上角y偏移
				double dFilterOffsetY,         // 滤波投影曲线左上角y偏移  
				double dProjHeight,            // 投影曲线,图像高度,滤波曲线为2倍
				double dProjWidth,             // 投影曲线,图像,滤波曲线宽度
				double  dImageOffsetY = 0);    // 图像y偏移

	// 设置矩形的绘制区域
	void SetDrawRect(double dX, double dY, double dWidth, double dHeight);

	// 设置矩形的绘制区域
	void SetDrawRect(const scRect& rect );
	scRect GetDrawRect() const;

	// 设置被点中的卡尺结果序号，要求序号在[0,nNum-1]范围内
	void SetHitCaliperResult(int nHitCaliperResult);
	// 获取被点中的卡尺结果序号,序号从0开始，范围[0,nNum-1]
	int	 GetHitCaliperResult() const;			
	
	virtual BOOL BoundingRect(CRect& rect) const;

	CString GetTipText()	const;
	CString	GetTipText(int nHandle, int nNum) const;
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


protected:

	void  LoadCursor();
	void  InitTipText();
	void  HitTestHandles(CPoint point, TrackerHit& hitResult, int& nNum) const;
	CRect GetBoundRect(void* pLShape) const;
	CRect GetHandleRect(TrackerHit nHandle) const;
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;
	void  DrawCurve(CDC* pDC)const;
	void  DrawAxes(CDC* pDC)const;
	void  DrawText(CDC* pDC)const;
	void  DrawImage(CDC* pDC) const;


protected:

	scCaliperOneEdgeResult* m_pCapOneResult;
	scCaliperTwoEdgeResult* m_pCapTwoResult;
	int		m_nResultNum;


	void*	m_pLShape;		    // a pointer to the object
	void*   m_pMyDib;	

	bool    m_bOneEdge;         // 是否为单边
	bool    m_bSetCapResult;	// 是否绘制卡尺结果
	bool    m_bIsDrawImage ;    // 是否绘制图像

	TrackerHit	 m_hitResult;
	int		m_nHitCaliperResult;
	static HCURSOR g_hCursor;   // handles to the cursors
};

#pragma pack(pop)
