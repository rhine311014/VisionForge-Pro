// svGuiMaskRect.h

// ---------------------------------------------------------------------
//                        掩模矩形交互图形
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svMaskRect.h"

#pragma pack(push,8)



class SVGUI_API scGuiMaskRect : public scGuiGraphic
{
public:
	scGuiMaskRect();
	virtual ~scGuiMaskRect();

	virtual GraphicType GetType() const {return eGuiMaskRect;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		//左上角、右上角、右下角、左下角
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		//上边中心、右边中心、下边中心、左边中心
		hitTopCenter = 4, hitRightCenter = 5, hitBottomCenter = 6, hitLeftCenter = 7,
		//掩模区域左上角、右上角、右下角、左下角
		hitMaskTopLeft = 8,hitMaskTopRight = 9,hitMaskBottomRight =10,hitMaskBottomLeft = 11,
		//掩模区域边中心、右边中心、下边中心、左边中心
		hitMaskTopCenter = 12,hitMaskRightCenter = 13,hitMaskBottomCenter = 14,hitMaskLeftCenter=15,
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 16, hitCenterYSeg = 17,
		//掩模区域十字中心X边、十字中心Y边
		hitMaskCenterXSeg = 18,hitMaskCenterYSeg = 19,
		//上边、右边、下边、左边
		hitSideTop = 20, hitSideRight = 21, hitSideBottom = 22, hitSideLeft = 23,
		//掩模区域上边、右边、下边、左边
		hitMaskSideTop = 24,hitMaskSideRight = 25,hitMaskSideBottom = 26,hitMaskSideLeft = 27
	};

	// 自由度
	enum GuiRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置		
		eGuiDOFMask     = 4,    // 可掩模
		eGuiDOFMaskSize = 8,    // 可改变掩模宽、高
		eGuiDOFMaskPosition = 16,// 可改变掩模位置
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFMask|eGuiDOFMaskSize|eGuiDOFMaskPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	// 设置MaskRect
	void SetMaskRect(scMaskRect maskrect);
	scMaskRect GetMaskRect();

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
	sc2Vector GetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）




	// dCenterX:中心点X坐标, dCenterY: 中心点Y坐标, dWidth: 宽, dHeight:高 
	void SetCenterWidthHeight_Mask(double dCenterX, double dCenterY, double dWidth, double dHeight);		
	void GetCenterWidthHeight_Mask(double& dCenterX, double& dCenterY, double& dWidth, double& dHeight) const;	

	// dX:左上点X坐标, dY: 左上点Y坐标, dWidth: 宽, dHeight:高 
	void SetXYWidthHeight_Mask(double dX, double dY, double dWidth, double dHeight);	
	void GetXYWidthHeight_Mask(double& dX, double& dY, double& dWidth, double& dHeight) const;

	// vCenterX:中心点, vSize:尺寸 
	void SetCenterWidthHeight_Mask(const sc2Vector& vCenter, const sc2Vector& vSize);		
	void GetCenterWidthHeight_Mask(sc2Vector& vCenter, sc2Vector& vSize) const;	

	// vUL:左上点，vSize:尺寸 
	void SetXYWidthHeight_Mask(const sc2Vector& vUL, const sc2Vector& vSize);	
	void GetXYWidthHeight_Mask(sc2Vector& vUL, sc2Vector& vSize) const; 

	// 用矩形scRect设置
	void   SetRect_Mask(const scRect& rect); 
	scRect GetRect_Mask() const; 

	void SetCenterCrossExtent_Mask(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetCenterCrossExtent_Mask() const;								// 获取中心十字线尺寸（宽度和高度）

	void SetCenterCrossVisible_Mask(BOOL bVisible);		// 设置中心十字线是否可见
	BOOL IsCenterCrossVisible_Mask() const;				// 获取中心十字线是否可见


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
	scMaskRect*	m_pMaskRect;
	scMaskRect* m_pMaskRectDrag;
	TrackerHit m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[28];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;		 
	sc2Vector	m_vCenterCrossSizeDrag;	

	BOOL        m_bMaskCenterCrossVisible;
	sc2Vector   m_vMaskCenterCrossSize;
	sc2Vector   m_vMaskCenterCrossSizeDrag;
};

#pragma pack(pop)
