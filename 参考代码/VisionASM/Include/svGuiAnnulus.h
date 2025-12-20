// svGuiAnnulus.h

// ---------------------------------------------------------------------
//                            圆环交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svAnnulus.h"

#pragma pack(push,8)


class SVGUI_API scGuiAnnulus  : public scGuiGraphic
{
public:
	scGuiAnnulus();
	virtual ~scGuiAnnulus();

	virtual GraphicType GetType() const {return eGuiAnnulus;};

	////////////////////////////////////////////////////////////
	//		      *  *  *   
	//	      *	            *
	//             *  *       
	//    *    *         *     *
	//   *   *             *    *
	//  *   *               *    *
	//  *   *      *center  +    +Radius
	//  *   *               *    *
	//   *   *             *    *
	//    *    *         *     *
	//            *  *     
	//        *             * 
	//            *  *  *
	/////////////////////////////////////////////////////////
	enum TrackerHit
	{
		hitNothing = -1, 
		hitFiducialRight= 0,	// 基准半径右调节点
		hitFiducialBottom= 1,	// 基准半径下调节点
		hitFiducialLeft= 2,		// 基准半径左调节点
		hitFiducialTop= 3,		// 基准半径上调节点
		hitOtherRight	= 4,	// 另外半径右调节点
		hitOtherBottom= 5,		// 另外半径下调节点
		hitOtherLeft= 6,		// 另外半径左调节点
		hitOtherTop= 7,			// 另外半径下调节点
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 8,
		hitCenterYSeg = 9,
		hitAnnulus = 10			// 圆环							
	};

	// 自由度
	enum GuiAnnulusDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变尺寸
		eGuiDOFPosition	= 2,	// 可改变位置		
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition, // 所有的自由度
	};

public:
    
	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	void SetCenterRadiusRadialScale(double dCenterX,			// 圆心的X坐标
									double dCenterY,			// 圆心的Y坐标
									double dRadius,				// 半径
									double dRadialScale);		// 径向缩放比例	
	void GetCenterRadiusRadialScale(double& dCenterX,			// 圆心的X坐标
									double& dCenterY,			// 圆心的Y坐标
									double& dRadius,			// 半径
									double& dRadialScale) const;// 径向缩放比例	

	void SetAnnulus(const scAnnulus& annulus);					// 设置圆环
	scAnnulus GetAnnulus() const;								// 获取圆环

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

	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;
	void  LoadCursor();
	void  InitTipText();

protected:

	scAnnulus* m_pAnnulus;
	scAnnulus* m_pAnnulusDrag;
	TrackerHit m_hitResult;
	static HCURSOR g_hCursor[11]; // g_hCursor[2]-->g_hCursor[3]
    
	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 
};

#pragma pack(pop)
