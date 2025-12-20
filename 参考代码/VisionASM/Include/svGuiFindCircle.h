// svGuiFindCircle.h

// ---------------------------------------------------------------------
//                           圆拟合交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svCircularArc.h"
#include "svAffineRect.h"
#include "svAnnulusSection.h"

#pragma pack(push,8)


class SVGUI_API scGuiFindCircle : public scGuiGraphic  
{
public:
	scGuiFindCircle();
	virtual ~scGuiFindCircle();

	virtual GraphicType GetType() const {return eGuiFindCircle;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//					  AngleStart +  * PointStart
	//		    	                     *
	//		    	                      *
	//									  		   \
	//		    						   *		\
	//		    	       Marker+---------*---------- 
	//							 |					/ |
	//							 |				   /  |
	//		    	             |         * Bend	  |	
	//							 |					  |
	//							 |					  |
	//				             ----------*----------	
	//									   *
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
		hitMarker		= 5,	// 仿射矩形标记点
		hitMirror		= 6,	// 卡尺镜像点
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 7,
		hitCenterYSeg = 8,
		hitCircle		= 9,		// 圆弧
		hitMaskCircle  = 10,         // 掩模
		hitMaskRegion  = 11,
		hitMaskSideStart = 12,
		hitMaskSideEnd   = 13,
		hitAnnulusSideRadial  = 14,     // 圆环径向边缘
		hitAnnulusSideInner  = 15, // 圆环内边缘
		hitAnnulusSideOuter  = 16, // 圆环外边缘
		hitAnnulusRegion  = 17, // 圆环区域
	};

	// 自由度
	enum GuiFindCircleDOF
	{
		eGuiDOFNone	      = 0,	 // 禁止所有的自由度	
		eGuiDOFStartAngle = 1,   // 可改变起始角
		eGuiDOFStart	  = 2,	 // 可改变起始点
		eGuiDOFEndAngle   = 4,   // 可改变终止角
		eGuiDOFEnd        = 8,   // 可改变终止点
        eGuiDOFBend       = 16,  // 可改变弧线中点
		eGuiDOFMarker     = 32,  // 可改变仿射矩形标记点	
		eGuiDOFPosition   = 64,  // 可改变位置
		eGuiDOFMask       = 128, // 可掩模
		eGuiDOFMirror       = 256, // 可卡尺镜像
		eGuiDOFAll		= eGuiDOFStartAngle|eGuiDOFStart|eGuiDOFEndAngle|eGuiDOFEnd|eGuiDOFBend|eGuiDOFMarker|eGuiDOFPosition|eGuiDOFMask|eGuiDOFMirror, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	void SetGuiDOFMaskEnable(BOOL bEnable);			// 设置是否允许掩模dof
	BOOL IsGuiDOFMaskEnable() const;				// 获取是否允许掩模dof

	// 设置圆弧参数
	void SetCenterRadiusAngleStartAngleSpan(double dCenterX,			// 圆心的X坐标
											double dCenterY,			// 圆心的Y坐标
											double dRadius,				// 半径
											double dAngleStart,			// 起始角度，值域[-π，π)
											double dAngleSpan);			// 角跨度，值域[-4π，4π)
	// 获取圆弧参数
	void GetCenterRadiusAngleStartAngleSpan(double& dCenterX,			// 圆心的X坐标
											double& dCenterY,			// 圆心的Y坐标
											double& dRadius,			// 半径
											double& dAngleStart,		// 起始角度，值域[-π，π)
											double& dAngleSpan) const;	// 角跨度，值域[-4π，4π)

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

	void	SetCircularArc(const scCircularArc& circularArc);			// 设置圆弧
	scCircularArc GetCircularArc() const;								// 获取圆弧

	void	SetCaliperParams(int nCaliperNum,							// 设置卡尺数量
							CaliperSearchDirection nDirection,			// 设置卡尺搜索方向
							double dSearchLength,						// 设置卡尺搜索长度			 
							double dProjectionLength,					// 设置卡尺投影长度
							bool   bVisible = TRUE);					// 设置卡尺是否可见			

	void	SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);		// 设置卡尺投影长度
	void	SetCaliperVisible(bool bVisible);							// 设置卡尺是否可见
	void    SetCircularArcArrowVisible(bool bVisible);					// 设置圆弧箭头是否可见
	void	SetAnnulusFindCircleVisible(BOOL bVisible);					// 设置找圆工具圆环是否可见

	int		GetCaliperNum()					const;			// 获取卡尺数量
	int		GetCaliperSearchDirection()		const;			// 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;			// 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			// 获取卡尺投影长度
	BOOL	IsCaliperVisible()				const;			// 获取卡尺是否可见
	BOOL	IsCircularArcArrowVisible()		const;			// 获取圆弧箭头是否可见
	BOOL	IsAnnulusFindCircleVisible()	const;			// 获取找圆工具圆环是否可见

	void	SetMaskData(std::vector<double> vMaskData);					// 设置工具掩模数据
	void	GetMaskData(std::vector<double>& vMaskData) const;			// 获取工具掩模数据
	void	GetCaliperMasked(std::vector<bool>& vMasked) const;			// 获取卡尺是否被掩模覆盖
	void	GetCaliperMasked(std::vector<BOOL>& vMasked) const;			// 获取卡尺是否被掩模覆盖

	// 获取第nIndex个卡尺的参数
	void	GetCaliper(int	   nIndex,						// 卡尺序号，序号范围从0到nCaliperNum-1
						double& dCenterX,					// 仿射矩形（卡尺）中心点的X坐标  
						double& dCenterY,					// 仿射矩形（卡尺）中心点的Y坐标 
						double& dSideXLength,				// 仿射矩形（卡尺）X边长度 
						double& dSideYLength,				// 仿射矩形（卡尺）Y边长度 
						scRadian& rRotation,				// 仿射矩形（卡尺）旋转角度（单位弧度）
						scRadian& rSkew)	const;			// 仿射矩形（卡尺）切变角度（单位弧度）

	scAffineRect GetCaliper(int nIndex) const;				// 获取第nIndex个卡尺	
	
	BOOL    SetCaliperSpacing(double dSpacing);				// 设置卡尺间距
	double  GetCaliperSpacing() const;						// 获取卡尺间距

	void	SetDragSpacingChange(BOOL bChange);				// 设置拖拽起始点时卡尺间距是否改变
	BOOL	GetDragSpacingChange();

	scAnnulusSection GetAnnulusFindCircle() const;			// 获取找圆工具圆环

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

	void	EnableFindCircleParamEdit(BOOL bEnable);
	BOOL	IsEnableFindCircleParamEdit();
	BOOL    SetFindCircleParam(bool bOneEdgeOrTwoEdge,double dContrastThreshold,EdgePolarity nEdgePolarity1,EdgePolarity nEdgePolarity2,long lFilterSize,long nOutLinePrecent,bool bSortByScore);	
	void    GetFindCircleParam(bool& bOneEdgeOrTwoEdge,double& dContrastThreshold,EdgePolarity& nEdgePolarity1,EdgePolarity& nEdgePolarity2,long& lFilterSize,long& nOutLinePrecent,bool& bSortByScore);	

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
	virtual BOOL OnGraphicParamEdit(CPoint point);

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

protected:
	void  LoadCursor();
	void  InitTipText();
	CRect GetHandleRect(TrackerHit nHandle) const;
	int   HitTestHandles(CPoint point) const;
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

	double ModifyMaskOffset(double dStart, double dEnd) const;
	double GetMaskOffset(void* pLCircle, sc2Vector point) const;
	sc2Vector GetMaskPoint(void* pLCircle, double dOffset) const;

protected:
	void*	m_pLShape;					// a pointer to the object
	void*	m_pLShapeDrag;
	CPoint	m_ptCur;
	TrackerHit		m_hitResult;
	static HCURSOR	g_hCursor[18];		// handles to the cursors
	BOOL	m_bCaliperVisible;			// whether the caliper is visible or not 
	BOOL	m_bCircularArcArrowVisible;	// whether the CircleArcArrow is visible or not
	BOOL	m_bAnnulusFindCircleVisible;
	BOOL	m_bDragSpacingChange;

	DWORD	        m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	
	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 

	std::vector<sc2Vector> m_vMaskRange;
	std::vector<sc2Vector> m_vMaskRangeDrag;

	mutable int m_nSelectedMaskRegionIndex;
	CPoint  m_ptDragStart;

	BOOL m_bEnableFindCircleParamEdit;
	//找圆工具参数
	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lFilterSize;
	long m_nOutLinePrecent;
	bool m_bSortByScore;
};

#pragma pack(pop)
