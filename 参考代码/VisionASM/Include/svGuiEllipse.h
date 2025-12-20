// svGuiEllipse.h

// ---------------------------------------------------------------------
//                           椭圆交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svEllipse.h"

#pragma pack(push,8)

class SVGUI_API scGuiEllipse : public scGuiGraphic  
{
public:
	scGuiEllipse();
	virtual ~scGuiEllipse();

	virtual GraphicType GetType() const {return eGuiEllipse;};

	//////////////////////////////////////////////////////////////////////////
	//						  *
	//	            *                   *
	//	      *                             *
	//     	*                                  *
	//	   *RadiusX                             * Rotation
	//     *                                    *
	//	    *                                  *
	//	      *                             *
	//	            *                   *
	//	                      * RadiusY
	//////////////////////////////////////////////////////////////////////////
	enum TrackerHit
	{
		hitNothing     = -1, //未点中		
		hitRadiusX     =  0, //轴X 
		hitRadiusY     =  1, //轴Y 
		hitRotation    =  2, //旋转点  	
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 3,
		hitCenterYSeg = 4,
		hitEllipseLine =  5  //椭圆边									
	}; 

	// 自由度
	enum GuiEllipseDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSizeX	= 1,	// 可改变X轴长度
		eGuiDOFSizeY    = 2,    // 可改变Y轴长度
		eGuiDOFRotation = 4,    // 可改变旋转角度
		eGuiDOFPosition	= 8,	// 可改变位置		
		eGuiDOFAll		= eGuiDOFSizeX|eGuiDOFSizeY|eGuiDOFRotation|eGuiDOFPosition, // 所有的自由度
	};

public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	//设置椭圆的中心坐标，XY半轴，以及旋转角
	void  SetCenterXYRadiusXYRotation(double dCenterX, double dCenterY, double dRadiusX, 
		double dRadiusY, double dRotation);
	//获取椭圆的中心坐标，XY半轴，以及旋转角
	void  GetCenterXYRadiusXYRotation(double& dCenterX, double& dCenterY, double& dRadiusX, 
		double& dRadiusY, double& dRotation) const; 

	//设置椭圆的中心坐标，XY半轴，以及旋转角
	void  SetCenterXYRadiusXYRotation(const sc2Vector& vCenter, double dRadiusX, 
		double dRadiusY, const scRadian& rRotation);
	//获取椭圆的中心坐标，XY半轴，以及旋转角
	void  GetCenterXYRadiusXYRotation(sc2Vector& vCenter, double& dRadiusX, 
		double& dRadiusY, scRadian& rRotation) const; 

	void  SetEllipse(const scEllipse& ellipse);			// 设置椭圆 
	scEllipse GetEllipse() const;						// 获取椭圆

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

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

	void	SetFillEnabled(BOOL bEnable);								// 设置是否允许填充
	BOOL	IsFillEnabled() const;										// 获取是否允许填充

	void    SetFillColor( COLORREF color);								// 设置填充的颜色
	COLORREF GetFillColor() const;										// 获取填充的颜色

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawEllipse(CDC* pDC, scEllipse* pEllipse) const;

	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;
	void  LoadCursor();
	void  InitTipText();


protected:

	scEllipse*	m_pEllipse;
	scEllipse*	m_pEllipseDrag;
	TrackerHit	m_hitResult;
	CPoint		m_ptCur;
	static HCURSOR g_hCursor[6];

	DWORD		    m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 
};

#pragma pack(pop)
