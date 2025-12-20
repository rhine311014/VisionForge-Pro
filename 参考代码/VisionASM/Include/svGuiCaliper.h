// svGuiCaliper.h

// ---------------------------------------------------------------------
//                            卡尺交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"

#pragma pack(push,8)


class SVGUI_API scGuiCaliper : public scGuiGraphic  
{
public:
	scGuiCaliper();
	virtual ~scGuiCaliper();

	virtual GraphicType GetType() const {return eGuiCaliper;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//		o		 sideX		  x
	//		*---------------------*
	//		|					  |
	//		|					  |
	// sideY|		    		  * rotation
	//		|					  |
	//		|					  |
	//		*----------*----------*
	//		y		  skew		  opp
	/////////////////////////////////////////////////////////////////////////////
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		//原点、X角点、对角点、Y角点
		hitOrigin = 0, hitCornerX = 1, hitOpposite = 2, hitCornerY = 3,
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 4, hitCenterYSeg = 5,
		//X边、Y边的对边、X边的对边、Y边
		hitSideX = 6, hitParallelY = 7, hitParallelX = 8, hitSideY = 9,
		//切变点、旋转点
		hitSkew = 10, hitRotation = 11,									
	};

	// 自由度
	enum GuiCaliperDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFSkew		= 8,	// 可改变切变角度
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation|eGuiDOFSkew, // 所有的自由度
	};


public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置卡尺(仿射矩形)参数
	void SetCenterLengthsRotationSkew(double dCenterX,			// 中心点的X坐标
									double dCenterY,			// 中心点的Y坐标
									double dSideXLength,		// 仿射矩形X边长度
									double dSideYLength,		// 仿射矩形Y边长度
									double dRotation,			// 旋转角度（单位弧度）
									double dSkew);				// 切变角度（单位弧度）
	// 获取卡尺(仿射矩形)参数
	void GetCenterLengthsRotationSkew(double& dCenterX,			// 中心点的X坐标 
									double& dCenterY,			// 中心点的Y坐标
									double& dSideXLength,		// 仿射矩形X边长度 
									double& dSideYLength,		// 仿射矩形Y边长度 
									double& dRotation,			// 旋转角度（单位弧度）
									double& dSkew) const;		// 切变角度（单位弧度）

	// 设置卡尺(仿射矩形)参数
	void SetCenterLengthsRotationSkew(const sc2Vector& vCenter,	// 中心点
									double dSideXLength,		// X边长度
									double dSideYLength,		// Y边长度
									const scRadian& rRotation,	// 旋转角度
									const scRadian& rSkew);		// 切变角度
	// 获取卡尺(仿射矩形)参数
	void GetCenterLengthsRotationSkew(sc2Vector& vCenter,		// 中心点
									double& dSideXLength,		// X边长度
									double& dSideYLength,		// Y边长度
									scRadian& rRotation,		// 旋转角度
									scRadian& rSkew) const;		// 切变角度

	void	SetAffineRect(const scAffineRect& affineRect);		// 设置卡尺(仿射矩形)
	scAffineRect GetAffineRect() const;							// 获取卡尺(仿射矩形)

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
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

protected:
	scAffineRect* m_pAffineRect;
	scAffineRect* m_pAffineRectDrag;
	TrackerHit	  m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[12];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			//
};

#pragma pack(pop)
