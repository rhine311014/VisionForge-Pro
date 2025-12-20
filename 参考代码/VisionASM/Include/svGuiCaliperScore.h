// svGuiCaliperScore.h

// ---------------------------------------------------------------------
//                            卡尺评价函数交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svRect.h"

#pragma pack(push,8)

class SVGUI_API scGuiCaliperScore : public scGuiGraphic 
{
public:
	scGuiCaliperScore();
	virtual ~scGuiCaliperScore();

	virtual GraphicType GetType() const {return eGuiCaliperScore;};

	enum TrackerHit
	{
		hitNothing = -1, 
		hitY0      =  0,  
		hitY1      =  1,
		hitX0      =  2,
		hitX1      =  3,
		hitXc      =  4,
		hitY0H     =  5,  
		hitY1H     =  6,
		hitX0H     =  7,
		hitX1H     =  8,
		hitXcH     =  9
	};

	// 自由度
	enum GuiCaliperScoreDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFX		= 1,	// 可改变X轴数据
        eGuiDOFY        = 2,    // 可改变Y轴数据		
		eGuiDOFAll		= eGuiDOFX|eGuiDOFY, // 所有的自由度
	};

public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置单边映射函数参数
	BOOL SetXYParameters(double X0,double X1,double XC, double Y0,double Y1);

	// 获取单边映射函数参数
	BOOL GetXYParameters(double& X0,double& X1,double& XC, double& Y0,double& Y1) const;

	// 设置双边映射函数参数
	BOOL SetXYParameters(double X0,double X1,double XC,double Y0,double Y1,
		double X0H,double X1H,double XCH, double Y0H,double Y1H);

	// 获取双边映射函数参数
	BOOL GetXYParameters(double& X0,double& X1,double& XC,double& Y0,double& Y1,
		double& X0H,double& X1H,double& XCH, double& Y0H,double& Y1H) const;

	// 设置、获取矩形绘制区域
	void  SetDrawRect(double dX, double dY, double dWidth, double dHeight);
	void  GetDrawRect(double& dX, double& dY, double& dWidth, double& dHeight) const;

	// 设置、获取矩形绘制区域
	void   SetDrawRect(const scRect& rect);
	scRect GetDrawRect() const;

	virtual BOOL BoundingRect(CRect& rect) const;

	CString  GetTipText() const;
	CString	 GetTipText(int nHandle) const;
	CString	 GetTipText(CPoint point) const;


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

	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const; 
	void  LoadCursor();
	void  InitTipText();

protected:
	void*	m_pLShape;
	void*	m_pLShapeDrag;
	TrackerHit	   m_hitResult;
	static HCURSOR g_hCursor[2];

	DWORD			m_DOF;				// 自由度

};

#pragma pack(pop)
