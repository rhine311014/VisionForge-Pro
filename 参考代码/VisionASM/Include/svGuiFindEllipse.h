#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"
#include "svEllipseArc.h"
#include "svCircularArc.h"

#pragma pack(push,8)

class SVGUI_API scGuiFindEllipse : public scGuiGraphic
{
public:
	scGuiFindEllipse();
	virtual ~scGuiFindEllipse();

	virtual GraphicType GetType() const {return eGuiFindEllipse;};

	enum TrackerHit
	{
		hitNothing		 = -1,	// 未击中
		hitStartAngle	 = 0,	// 起始角
		hitStartPoint	 = 1,	// 起始点
		hitEndAngle		 = 2,	// 终止角
		hitEndPoint		 = 3,	// 终止点
		hitBend			 = 4,	// 弧线中点
		hitArcSpan       = 5,   // 
		hitMarker		 = 6,	// 仿射矩形标记点
		hitMirror		 = 7,	// 卡尺镜像点
		hitCenterXSeg    = 8,   // 十字中心X边、
		hitCenterYSeg    = 9,   // 十字中心Y边
		hitEllipse	     = 10,   // 椭圆弧
		hitMaskEllipse   = 11,   // 掩模
		hitMaskRegion    = 12,
		hitMaskSideStart = 13,
		hitMaskSideEnd   = 14,
	};

	// 自由度
	enum GuiFindEllipseDOF
	{
		eGuiDOFNone	      = 0,	 // 禁止所有的自由度	
		eGuiDOFStartAngle = 1,   // 可改变起始角
		eGuiDOFStart	  = 2,	 // 可改变起始点
		eGuiDOFEndAngle   = 4,   // 可改变终止角
		eGuiDOFEnd        = 8,   // 可改变终止点
		eGuiDOFBend       = 16,  // 可改变弧线中点
		eGuiDOFArcSpan    = 32,  // 可改变张角
		eGuiDOFMarker     = 64,  // 可改变仿射矩形标记点	
		eGuiDOFPosition   = 128,  // 可改变位置
		eGuiDOFMask       = 256, // 可掩模
		eGuiDOFMirror	  = 512, // 可卡尺镜像
		eGuiDOFAll		= eGuiDOFStartAngle|eGuiDOFStart|eGuiDOFEndAngle|eGuiDOFEnd|eGuiDOFBend|eGuiDOFArcSpan|eGuiDOFMarker|eGuiDOFPosition|eGuiDOFMask|eGuiDOFMirror, // 所有的自由度
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

	void SetCaliperParams(int nCaliperNum,							// 设置卡尺数量
		                  CaliperSearchDirection nDirection,		// 设置卡尺搜索方向
		                  double dSearchLength,						// 设置卡尺搜索长度			 
		                  double dProjectionLength,					// 设置卡尺投影长度
		                  bool   bVisible = TRUE);					// 设置卡尺是否可见			

	void	SetCaliperNum(int nCaliperNum);							// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);			// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);	// 设置卡尺投影长度
	void	SetCaliperVisible(bool bVisible);						// 设置卡尺是否可见
	void    SetEllipseArcArrowVisible(bool bVisible);				// 设置椭圆弧箭头是否可见

	int		GetCaliperNum()					const;			        // 获取卡尺数量
	int		GetCaliperSearchDirection()		const;			        // 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;			        // 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			        // 获取卡尺投影长度
	BOOL	IsCaliperVisible()				const;			        // 获取卡尺是否可见
	BOOL	IsEllipseArcArrowVisible()		const;			        // 获取椭圆弧箭头是否可见

	void	SetMaskData(std::vector<double> vMaskData);				// 设置工具掩模数据
	void	GetMaskData(std::vector<double>& vMaskData) const;		// 获取工具掩模数据
	void	GetCaliperMasked(std::vector<bool>& vMasked) const;		// 获取卡尺是否被掩模覆盖
	void	GetCaliperMasked(std::vector<BOOL>& vMasked) const;		// 获取卡尺是否被掩模覆盖

	// 获取第nIndex个卡尺的参数
	void	GetCaliper(int	nIndex,                                 // 卡尺序号，序号范围从0到nCaliperNum-1
		               double& dCenterX,					        // 仿射矩形（卡尺）中心点的X坐标  
		               double& dCenterY,					        // 仿射矩形（卡尺）中心点的Y坐标 
		               double& dSideXLength,				        // 仿射矩形（卡尺）X边长度 
		               double& dSideYLength,				        // 仿射矩形（卡尺）Y边长度 
		               scRadian& rRotation,				            // 仿射矩形（卡尺）旋转角度（单位弧度）
		               scRadian& rSkew)	const;			            // 仿射矩形（卡尺）切变角度（单位弧度）

	scAffineRect GetCaliper(int nIndex) const;				        // 获取第nIndex个卡尺	

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

	virtual void OnGraphicMaskDelete(CPoint point);
	virtual void OnGraphicMaskClear(CPoint point);

protected:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

protected:
	void  LoadCursor();
	void  InitTipText();
	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;

	double ModifyMaskOffset(double dStart, double dEnd) const;
	double GetMaskOffset(void* pLEllipse, sc2Vector point) const;
	sc2Vector GetMaskPoint(void* pLEllipse, double dOffset) const;

public:

	void DrawEllipseArc(CDC* pDC,const scEllipseArc& ellipseArc)const;                                 // 画椭圆弧
	double EllipseArcTrimAngle(const scEllipseArc& ellipseArc,scRadian rAngle)const;                   // 椭圆切线角度
	BOOL IsOnEllipseArc(const scEllipseArc& ellipseArc,CPoint point)const;                             // 是否在椭圆线上

	static void CentrifugalToCentral(const scEllipse& ellipse,const scRadian &rCentrifugal,scRadian &rCentral);  // 离心角转圆心角  [-PI,PI]
	static void CentralToCentrifugal(const scEllipse& ellipse,const scRadian &rCentral,scRadian &rCentrifugal);  // 圆心角转离心角  [-PI,PI]

	double   GetMaskOffset(const scEllipseArc& ellipseArc,sc2Vector point)const;
	scRadian GetEllipseArcSpan(const scEllipseArc& ellipseArc,const sc2Vector& ptStart,const sc2Vector& ptEnd)const;  // 根据两点，确定离心角弧度
	void     CalculateOuterInnerArc(void *pLFindEllipse,const scRadian& rStartPhi,const scRadian& rEndPhi,scEllipseArc& ellipseOuterArc,scEllipseArc& ellipseInnerArc)const;

protected:
	void*	m_pLShape;					// a pointer to the object
	void*	m_pLShapeDrag;
	CPoint	m_ptCur;
	TrackerHit		m_hitResult;
	static HCURSOR	g_hCursor[15];		// handles to the cursors
	BOOL	m_bCaliperVisible;			// whether the caliper is visible or not 
	BOOL	m_bEllipseArcArrowVisible;	// 

	DWORD	        m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 

	std::vector<sc2Vector> m_vMaskRange;
	std::vector<sc2Vector> m_vMaskRangeDrag;

	mutable int m_nSelectedMaskRegionIndex;
	CPoint  m_ptDragStart;

};

#pragma pack(pop)