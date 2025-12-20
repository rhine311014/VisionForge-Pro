#pragma once

#include "svEllipseArc.h"
#include "svGuiGraphic.h"

#pragma pack(push,8)

class scRect;
class SVGUI_API scGuiEllipseArc : public scGuiGraphic
{
public:
	scGuiEllipseArc();
	virtual ~scGuiEllipseArc();

	virtual GraphicType GetType() const {return eGuiEllipseArc;}

	enum TrackerHit
	{
		hitNothing		 = -1,	// 未击中
		hitStartAngle	 = 0,	// 起始角
		hitStartPoint	 = 1,	// 起始点
		hitEndAngle		 = 2,	// 终止角
		hitEndPoint		 = 3,	// 终止点
		hitBend			 = 4,	// 弧线中点
		hitArcSpan       = 5,   // 
		hitCenterXSeg    = 6,   // 十字中心X边、
		hitCenterYSeg    = 7,   // 十字中心Y边
		hitEllipseArc	 = 8,   // 椭圆弧
	};

	// 自由度
	enum GuiEllipseArcDOF
	{
		eGuiDOFNone	      = 0,	 // 禁止所有的自由度	
		eGuiDOFStartAngle = 1,   // 可改变起始角
		eGuiDOFStart	  = 2,	 // 可改变起始点
		eGuiDOFEndAngle   = 4,   // 可改变终止角
		eGuiDOFEnd        = 8,   // 可改变终止点
		eGuiDOFBend       = 16,  // 可改变弧线中点
		eGuiDOFArcSpan    = 32,  // 可改变张角	
		eGuiDOFPosition   = 64,  // 可改变位置
		eGuiDOFAll		= eGuiDOFStartAngle|eGuiDOFStart|eGuiDOFEndAngle|eGuiDOFEnd|eGuiDOFBend|eGuiDOFArcSpan|eGuiDOFPosition, // 所有的自由度
	};

public:
	void EnableGuiDOF(DWORD dof);          // 设置自由度

	void SetCenterRadiiAngleStartAngleEnd(double dCenterX,        // 椭圆心X
		                                  double dCenterY,        // 椭圆新Y
		                                  double dRadiusX,        // X轴半径
		                                  double dRadiusY,        // Y轴半径
		                                  double rRotation,       // 旋转角度
		                                  double dAngleStart,     // 起始角度
		                                  double dAngleSpan);     // 角跨度

	void GetCenterRadiiAngleStartAngleEnd(double& dCenterX,        // 椭圆心X
		                                  double& dCenterY,        // 椭圆新Y
		                                  double& dRadiusX,        // X轴半径
		                                  double& dRadiusY,        // Y轴半径
		                                  double& dRotation,       // 旋转角度
		                                  double& dAngleStart,     // 起始角度
		                                  double& dAngleSpan)const;     // 角跨度

	void SetCenterRadiiAngleStartAngleEnd(sc2Vector& vCenter,      // 椭圆心
		                                  sc2Vector  vRadii,       // 半长轴
		                                  scRadian&  rRotation,    // 旋转角度
		                                  scRadian&  rAngleStart,  // 起始角度
		                                  scRadian&  rAngleSpan);  // 角跨度

	void GetCenterRadiiAngleStartAngleEnd(sc2Vector& vCenter,      // 椭圆心
		                                  sc2Vector&  vRadii,      // 半长轴
		                                  scRadian&  rRotation,    // 旋转角度
		                                  scRadian&  rAngleStart,  // 起始角度
		                                  scRadian&  rAngleSpan)const;  // 角跨度

	void SetEllipseArc(const scEllipseArc& ellipseArc);            // 设置椭圆弧
	scEllipseArc GetEllipseArc() const;                            // 获取椭圆弧

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

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

	scRect BoundingBox()const;                                          // 椭圆弧外接矩形

protected:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

protected:
	void  LoadCursor();
	void  InitTipText();
	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;

	void DrawEllipseArc(CDC* pDC,const scEllipseArc& ellipseArc)const;

	// 3点与弧度确定椭圆
	bool Trans3PointsAndSpanToEllipseArc(scEllipseArc& ellipseArc,sc2Vector vStart,sc2Vector vMid,sc2Vector vEnd,double dFraction,double dEpsilon = 1.e-5)const;

	void CentrifugalToCentral(const scEllipse& ellipse,const scRadian &rCentrifugal,scRadian &rCentral)const;  // 离心角转圆心角  [-PI,PI]
	void CentralToCentrifugal(const scEllipse& ellipse,const scRadian &rCentral,scRadian &rCentrifugal)const;  // 圆心角转离心角  [-PI,PI]

	double EllipseArcTrimAngle(const scEllipseArc& ellipseArc,scRadian rAngle)const;                   // 椭圆切线角度

	BOOL IsOnEllipseArc(const scEllipseArc& ellipseArc,CPoint point,int nVicinitySize)const;                             // 是否在椭圆线上

protected:
	scEllipseArc*	m_pLShape;					// a pointer to the object
	scEllipseArc*	m_pLShapeDrag;
	CPoint	m_ptCur;
	TrackerHit		m_hitResult;
	static HCURSOR	g_hCursor[9];		// handles to the cursors
	BOOL	m_bEllipseArcArrowVisible;	//

	DWORD	        m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			//

	CPoint  m_ptDragStart;
};

#pragma pack(pop)