// svGuiCircularArc.h


// ---------------------------------------------------------------------
//                            圆弧交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svCircularArc.h"

#pragma pack(push,8)


class SVGUI_API scGuiCircularArc : public scGuiGraphic  
{
public:
	scGuiCircularArc();
	virtual ~scGuiCircularArc();

	virtual GraphicType GetType() const {return eGuiCircularArc;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//					  AngleStart +  * PointStart
	//		    	                     *
	//		    	                      *
	//		    	                      
	//		    						   * 
	//							  					   
	//		    	                       * Bend	
	//							  				
	//				                       *	
	//		     	                      	
	//		      	                      *
	//				                     *
	//		               AngleEnd  +  * PointEnd
	/////////////////////////////////////////////////////////////////////////////

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing		= -1,	// 未击中
		hitStartAngle	= 0,	// 起始角
		hitStartPoint	= 1,	// 起始点
		hitEndAngle		= 2,	// 终止角
		hitEndPoint		= 3,	// 终止点
		hitBend			= 4,	// 弧线中点
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 5,
		hitCenterYSeg = 6,
		hitCircle		= 7		// 圆弧
	};	
	
	// 方向修饰箭头
	enum DiectionAdorment
	{
		eNone = 0,		// 无箭头
		eArrow,			// 普通箭头
		eSolidArrow		// 实心箭头
	};

	// 自由度
	enum GuiCircularArcDOF
	{
		eGuiDOFNone		    = 0,	// 禁止所有的自由度 
		eGuiDOFStartAngle	= 1,	// 可改变起始角
		eGuiDOFStartPoint   = 2,    // 可改变起始点
		eGuiDOFEndAngle     = 4,    // 可改变终止角
		eGuiDOFEndPoint     = 8,    // 可改变终止点 
		eGuiDOFBend         = 16,   // 可改变弧线中点
		eGuiDOFPosition	    = 32,   // 可改变位置	
		eGuiDOFAll		    = eGuiDOFStartAngle|eGuiDOFStartPoint|eGuiDOFEndAngle|eGuiDOFEndPoint|eGuiDOFBend|eGuiDOFPosition,// 所有的自由度
		 
	};


public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(double dCenterX,			// 圆心的X坐标
											double dCenterY,			// 圆心的Y坐标
											double dRadius,				// 半径
											double dAngleStart,			// 起始角度，单位弧度，值域[-π，π)
											double dAngleSpan);			// 张角，单位弧度，值域[-4π，4π)
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(double& dCenterX,			// 圆心的X坐标
											double& dCenterY,			// 圆心的Y坐标
											double& dRadius,			// 半径
											double& dAngleStart,		// 起始角度，单位弧度，值域[-π，π)
											double& dAngleSpan) const;	// 张角，值域[-4π，4π)

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(const sc2Vector& vCenter,	// 圆心坐标
											double dRadius,				// 半径
											const scRadian& rAngleStart,// 起始角度，单位弧度，值域[-π，π)
											const scRadian& rAngleSpan);// 张角，值域[-4π，4π)
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(sc2Vector& vCenter,			// 圆心坐标
											double& dRadius,			// 半径
											scRadian& rAngleStart,		// 起始角度，单位弧度，值域[-π，π)
											scRadian& rAngleSpan) const;// 张角，值域[-4π，4π)

	void SetCircularArc(const scCircularArc& circularArc);				// 设置圆弧
	scCircularArc GetCircularArc() const;								// 获取圆弧

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

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

	void  LoadCursor();
	void  InitTipText();
	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;

protected:

	scCircularArc* m_pCircularArc;
	scCircularArc* m_pCircularArcDrag;
	TrackerHit	   m_hitResult;
	CPoint		   m_ptCur;
	DiectionAdorment m_dirAdorn;
	static HCURSOR g_hCursor[8];		// handles to the cursors

	DWORD		   m_DOF;				// 自由度

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 

};

#pragma pack(pop)
