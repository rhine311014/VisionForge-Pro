// svGuiAnnulusSection.h

// ---------------------------------------------------------------------
//                            圆环段交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svAnnulusSection.h"

#pragma pack(push,8)


class SVGUI_API scGuiAnnulusSection : public scGuiGraphic  
{
public:
	scGuiAnnulusSection();
	virtual ~scGuiAnnulusSection();

	virtual GraphicType GetType() const {return eGuiAnnulusSection;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//							PointStart				
	//					  * * * *+* * * + AngleEnd
	//		    	       *      *      *
	//		    	        *      *      *
	//		    	                      
	//		    			 *		*	   * 
	//							  					   
	//		    	    Radii+      + Bend + Radii	
	//							  				
	//				         *      *      *	
	//		     	                      	
	//		      	        *      *      *
	//				       *      *      *
	//		              * * * *+* * * + AngleEnd
	//                           PointEnd
	/////////////////////////////////////////////////////////////////////////////

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing		= -1,	// 未击中
		hitStartAngle	= 0,	// 起始角
		hitStartPoint	= 1,	// 起始点
		hitEndAngle		= 2,	// 终止角
		hitEndPoint		= 3,	// 终止点
		hitFiducialRadius= 4,	// 基准半径调节点
		hitOtherRadius	= 5,	// 另外半径调节点
		hitBend			= 6,	// 弧线中点
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 7,
		hitCenterYSeg = 8,
		hitAnnulusSection= 9	// 圆环段
	};

	// 方向修饰箭头
	enum DiectionAdorment
	{
		eNone = 0,		// 无箭头
		eArrow,			// 普通箭头
		eSolidArrow		// 实心箭头
	};
    
	// 自由度
	enum GuiAnnulusSectionDOF
	{
		eGuiDOFNone		    = 0,	// 禁止所有的自由度 
		eGuiDOFStartAngle	= 1,	// 可改变起始角
		eGuiDOFStartPoint   = 2,    // 可改变起始点
		eGuiDOFEndAngle     = 4,    // 可改变终止角
		eGuiDOFEndPoint     = 8,    // 可改变终止点 
		eGuiDOFBend         = 16,   // 可改变弧线中点
		eGuiDOFSize         = 32,   // 可改变尺寸
		eGuiDOFPosition	    = 64,   // 可改变位置	
		eGuiDOFAll		    = eGuiDOFStartAngle|eGuiDOFStartPoint|eGuiDOFEndAngle|eGuiDOFEndPoint
		                         |eGuiDOFBend|eGuiDOFSize|eGuiDOFPosition, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	// 设置圆环段参数
	void SetCenterRadiusAngleStartAngleSpanRadialScale(double dCenterX,			// 圆心的X坐标
													double dCenterY,			// 圆心的Y坐标
													double dRadius,				// 半径
													double dAngleStart,			// 起始角度，单位弧度，值域[-π，π)
													double dAngleSpan,			// 张角，单位弧度，值域[-4π，4π)
													double dRadialScale);		// 径向缩放比例	
	// 获取圆环段参数
	void GetCenterRadiusAngleStartAngleSpanRadialScale(double& dCenterX,		// 圆心的X坐标
													double& dCenterY,			// 圆心的Y坐标
													double& dRadius,			// 半径
													double& dAngleStart,		// 起始角度，单位弧度，值域[-π，π)
													double& dAngleSpan,			// 张角，单位弧度，值域[-4π，4π)
													double& dRadialScale) const;// 径向缩放比例	

	// 设置圆环段参数
	void SetCenterRadiusAngleStartAngleSpanRadialScale(sc2Vector vCenter,		// 圆心坐标
													double dRadius,				// 半径
													const scRadian& rAngleStart,// 起始角度，值域[-π，π)
													const scRadian& rAngleSpan,	// 张角，值域[-4π，4π)
													double dRadialScale);		// 径向缩放比例	
	// 获取圆环段参数
	void GetCenterRadiusAngleStartAngleSpanRadialScale(sc2Vector& vCenter,		// 圆心坐标
													double dRadius,				// 半径
													scRadian& rAngleStart,		// 起始角度，值域[-π，π)
													scRadian& rAngleSpan,		// 张角，值域[-4π，4π)
													double dRadialScale);		// 径向缩放比例	

	void SetAnnulusSection(const scAnnulusSection& annulusSection);				// 设置圆环段
	scAnnulusSection GetAnnulusSection() const;									// 获取圆环段

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

	
	void SetDirectionAdornment(DiectionAdorment directAdorn);			// 设置圆弧上的箭头样式
	DiectionAdorment GetDirectionAdornment()const						// 获取圆弧上的箭头样式
	{ return m_dirAdorn; };

	virtual BOOL BoundingRect(CRect& rect) const;

	CString GetTipText()			const;
	CString	GetTipText(int nHandle) const;
	CString	GetTipText(CPoint point)const;



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
	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;
//	BOOL IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

protected:

	scAnnulusSection* m_pAnnulusSection;
	scAnnulusSection* m_pAnnulusSectionDrag;
	TrackerHit	   m_hitResult;
	CPoint		   m_ptCur;
	DiectionAdorment m_dirAdorn;

	
    DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色
	
	static HCURSOR g_hCursor[10];		// handles to the cursors

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 

};

#pragma pack(pop)
