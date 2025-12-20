// svGuiAffineRect.h

// ---------------------------------------------------------------------
//                            仿射矩形交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svMaskAffineRect.h"

#pragma pack(push,8)

class SVGUI_API scGuiMaskAffineRect : public scGuiGraphic  
{
public:
	scGuiMaskAffineRect();
	virtual ~scGuiMaskAffineRect();

	virtual GraphicType GetType() const {return eGuiMaskAffineRect;};

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
		//掩模区域 原点、X角点、对角点、Y角点
		hitMaskOrigin = 4, hitMaskCornerX = 5, hitMaskOpposite = 6, hitMaskCornerY = 7,
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 8, hitCenterYSeg = 9,
		//X边、Y边的对边、X边的对边、Y边
		hitSideX = 10, hitParallelY = 11, hitParallelX = 12, hitSideY = 13,
		//掩模区域 十字中心X边、十字中心Y边
		hitMaskCenterXSeg = 14, hitMaskCenterYSeg = 15,
		//掩模区域 X边、Y边的对边、X边的对边、Y边
		hitMaskSideX = 16, hitMaskParallelY = 17, hitMaskParallelX = 18, hitMaskSideY = 19,
		//切变点、旋转点
		hitSkew = 20, hitRotation = 21,									
	};

	// 自由度
	enum GuiAffineRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFSkew		= 8,	// 可改变切变角度
		eGuiDOFMask			= 16,	// 可掩模
		eGuiDOFMaskSize		= 32,	// 可改掩模变宽、高
		eGuiDOFMaskPosition	= 64,	// 可改变掩模位置
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation|eGuiDOFSkew|eGuiDOFMask|eGuiDOFMaskSize|eGuiDOFMaskPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置MaskAffineRect
	void SetMaskAffineRect(scMaskAffineRect maskAffineRect);
	scMaskAffineRect GetMaskAffineRect() const;

	// 获取仿射矩形
	scAffineRect GetAffineRect() const;	
	scAffineRect GetAffineRect_Mask() const;	

	// 设置仿射矩形
	void SetCenterLengthsRotationSkew(double dCenterX,			// 中心点的X坐标
									  double dCenterY,			// 中心点的Y坐标
									  double dSideXLength,		// X边长度
									  double dSideYLength,		// Y边长度
									  double dCenterX_Mask,			// 中心点的X坐标
									  double dCenterY_Mask,			// 中心点的Y坐标
									  double dSideXLength_Mask,		// X边长度
									  double dSideYLength_Mask,		// Y边长度
									  double dRotation,			// 旋转角度（单位弧度）
									  double dSkew);			// 切变角度（单位弧度）
	// 获取仿射矩形
	void GetCenterLengthsRotationSkew(double& dCenterX,			// 中心点的X坐标 
									  double& dCenterY,			// 中心点的Y坐标
									  double& dSideXLength,		// X边长度 
									  double& dSideYLength,		// Y边长度 
									  double& dRotation,		// 旋转角度（单位弧度）
									  double& dSkew) const;		// 切变角度（单位弧度）
	void GetCenterLengthsRotationSkew(double& dCenterX,			// 中心点的X坐标 
									  double& dCenterY,			// 中心点的Y坐标
									  double& dSideXLength,		// X边长度 
									  double& dSideYLength,		// Y边长度 
									  double& dCenterX_Mask,			// 中心点的X坐标 
									  double& dCenterY_Mask,			// 中心点的Y坐标
									  double& dSideXLength_Mask,		// X边长度 
									  double& dSideYLength_Mask,		// Y边长度 
									  double& dRotation,		// 旋转角度（单位弧度）
									  double& dSkew) const;		// 切变角度（单位弧度）

	// 设置仿射矩形
	void SetCenterLengthsRotationSkew(const sc2Vector& vCenter,	// 中心点
									  double dSideXLength,		// X边长度
									  double dSideYLength,		// Y边长度
									  const sc2Vector& vCenter_Mask,// 中心点
									  double dSideXLength_Mask,		// X边长度
									  double dSideYLength_Mask,		// Y边长度
									  const scRadian& rRotation,// 旋转角度
									  const scRadian& rSkew);	// 切变角度
	// 获取仿射矩形
	void GetCenterLengthsRotationSkew(sc2Vector& vCenter,		// 中心点
									  double& dSideXLength,		// X边长度
									  double& dSideYLength,		// Y边长度
									  scRadian& rRotation,		// 旋转角度
									  scRadian& rSkew) const;	// 切变角度
	void GetCenterLengthsRotationSkew(sc2Vector& vCenter,		// 中心点
									  double& dSideXLength,		// X边长度
									  double& dSideYLength,		// Y边长度
									  sc2Vector& vCenter_Mask,		// 中心点
									  double& dSideXLength_Mask,	// X边长度
									  double& dSideYLength_Mask,	// Y边长度
									  scRadian& rRotation,		// 旋转角度
									  scRadian& rSkew) const;	// 切变角度

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

	void SetCenterCrossExtent_Mask(const sc2Vector& vSize = sc2Vector(16,16));	// 设置掩模中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent_Mask() const;								// 获取掩模中心十字线尺寸（宽度和高度）

	void SetCenterCrossVisible_Mask(BOOL bVisible);		// 设置掩模中心十字线是否可见
	BOOL IsCenterCrossVisible_Mask() const;				// 获取掩模中心十字线是否可见

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
	scMaskAffineRect* m_pMaskAffineRect;
	scMaskAffineRect* m_pMaskAffineRectDrag;

	TrackerHit	  m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[22];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 

	BOOL        m_bMaskCenterCrossVisible;
	sc2Vector   m_vMaskCenterCrossSize;
	sc2Vector   m_vMaskCenterCrossSizeDrag;

private:
	sc2Vector m_vMaskConerPo;
	sc2Vector m_vMaskConerPx;
	sc2Vector m_vMaskConerPy;
	sc2Vector m_vMaskConerPopp;
};

#pragma pack(pop)
