// svGuiObround.h

// ---------------------------------------------------------------------
//                            长圆形交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"
#include "svCircularArc.h"
#include "svLineSeg.h"
#include "svObround.h"
#pragma pack(push,8)

class SVGUI_API scGuiObround : public scGuiGraphic  
{
public:
	scGuiObround();
	virtual ~scGuiObround();

	virtual GraphicType GetType() const {return eGuiObround;};

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
		//X弧边、Y边的对边、X弧边的对边、Y边
		hitSideX = 4, hitParallelY = 5, hitParallelX = 6, hitSideY = 7,
		//旋转点
		hitRotation = 8,									
	};

	// 自由度
	enum GuiAffineRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置仿射矩形
	void SetCenterLengthsRotation(  double dCenterX,			// 中心点的X坐标
									double dCenterY,			// 中心点的Y坐标
									double dSideXLength,		// X边长度
									double dSideYLength,		// Y边长度
									double dRotation);			// 旋转角度（单位弧度）			
	// 获取仿射矩形
	void GetCenterLengthsRotation(  double& dCenterX,			// 中心点的X坐标 
									double& dCenterY,			// 中心点的Y坐标
									double& dSideXLength,		// X边长度 
									double& dSideYLength,		// Y边长度 
									double& dRotation) const;	// 旋转角度（单位弧度）

	// 设置仿射矩形
	void SetCenterLengthsRotation(const sc2Vector& vCenter,	    // 中心点
							      double dSideXLength,		    // X边长度
								  double dSideYLength,		    // Y边长度
								   const scRadian& rRotation);  // 旋转角度

	// 获取仿射矩形
	void GetCenterLengthsRotation(  sc2Vector& vCenter,		     // 中心点
									double& dSideXLength,		 // X边长度
									double& dSideYLength,	     // Y边长度
									scRadian& rRotation) const;	 // 旋转角度

	// 设置仿射矩形, 切边角度强制转换为0
	void	SetAffineRect(const scAffineRect& affineRect);	
	// 获取仿射矩形
	scAffineRect GetAffineRect() const;	

	// 设置长圆形
	void		SetObround(const scObround& obround);
	scObround	GetObround() const;

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

	virtual BOOL SelectTest(CRect rect) ;			                // 使用矩形框选中测试

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
	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

	BOOL          IsNormalAffineRect(const scAffineRect* pAffRect) const;
	scCircularArc GetXOCirArc(const scAffineRect* pAffRect) const;
	scCircularArc GetYOPPCirArc(const scAffineRect* pAffRect) const;
	scLineSeg     GetOYLineSeg(const scAffineRect* pAffRect) const;
	scLineSeg     GetOPPXLineSeg(const scAffineRect* pAffRect) const;
	void          DrawObround(CDC* pDC, const scAffineRect* pAffRect) const;

protected:
	scAffineRect* m_pAffineRect;
	scAffineRect* m_pAffineRectDrag;
	TrackerHit	  m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[10];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色
};

#pragma pack(pop)
