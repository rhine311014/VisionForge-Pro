// svGuiFindObround.h

// ---------------------------------------------------------------------
//                            找长圆形交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"
#include "svCircularArc.h"
#include "svObround.h"
#pragma pack(push,8)

class SVGUI_API scGuiFindObround : public scGuiGraphic  
{
public:
	scGuiFindObround();
	virtual ~scGuiFindObround();

	virtual GraphicType GetType() const {return eGuiFindObround;};

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
		//X边、Y边的对边、X边的对边、Y边
		hitSideX = 4, hitParallelY = 5, hitParallelX = 6, hitSideY = 7,
		//旋转点
		hitRotation = 8, 
		// 标记点
		hitMarkerXO = 9, hitMarkerOY = 10, hitMarkerYOPP = 11, hitMarkerOPPX = 12,
		// 卡尺镜像点
		hitMirror = 13
	};

	// 自由度
	enum GuiAffineRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFMarker   = 8,	// 可改变卡尺
		eGuiDOFMirror	= 16,	// 可卡尺镜像
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation|eGuiDOFMarker|eGuiDOFMirror, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置仿射矩形
    void SetCenterLengthsRotation(double dCenterX,			// 中心点的X坐标
								  double dCenterY,			// 中心点的Y坐标
								  double dSideXLength,		// X边长度
								  double dSideYLength,		// Y边长度
								  double dRotation);		// 旋转角度（单位弧度）
		
	// 获取仿射矩形
	void GetCenterLengthsRotation(double& dCenterX,			// 中心点的X坐标 
								  double& dCenterY,			// 中心点的Y坐标
								  double& dSideXLength,		// X边长度 
								  double& dSideYLength,		// Y边长度 
								  double& dRotation) const;	// 旋转角度（单位弧度）


	// 设置仿射矩形
	void SetCenterLengthsRotation(const sc2Vector& vCenter,	  // 中心点
								  double dSideXLength,		  // X边长度
								  double dSideYLength,		  // Y边长度
								  const scRadian& rRotation); // 旋转角度
		
	// 获取仿射矩形
	void GetCenterLengthsRotation(sc2Vector& vCenter,		  // 中心点
								  double& dSideXLength,		  // X边长度
								  double& dSideYLength,		  // Y边长度
								  scRadian& rRotation) const; // 旋转角度

	// 设置仿射矩形
	void	SetAffineRect(const scAffineRect& affineRect);	
	// 获取仿射矩形
	scAffineRect GetAffineRect() const;	

	// 设置/获取长圆形
	void		SetObround(const scObround& obround);
	scObround	GetObround() const;

	void	SetCaliperParams(int nCaliperNum = 3,
							 CaliperSearchDirection nDirection = eInward,	// 设置卡尺搜索方向
							 double dSearchLength = 60.0,					// 设置卡尺搜索长度			 
							 double dProjectionLength = 20.0);			    // 设置卡尺投影长度

	
	void	GetCaliperParams(int& nCaliperNum,
							 CaliperSearchDirection& nDirection,	//卡尺搜索方向
							 double& dSearchLength,					//卡尺搜索长度			 
							 double& dProjectionLength);			//卡尺投影长度	

	void	SetCaliperNum(int nCaliperNum = 3);// 设置卡尺数量											
	void	SetCaliperSearchLength(double dSearchLength = 60.0);				     // 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength = 20.0);		     // 设置卡尺投影长度
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection = eInward);  // 设置卡尺搜索方向


	int	    GetCaliperNum()   const;			                          // 获取卡尺数量
	double	GetCaliperSearchLength()		const;			              // 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;			              // 获取卡尺投影长度
	int		GetCaliperSearchDirection()		const;			              // 获取卡尺搜索方向

	void	SetCaliperVisible(BOOL bVisible = TRUE);					  // 设置卡尺是否可见
	BOOL	IsCaliperVisible()				const;			              // 获取卡尺是否可见

	scAffineRect GetCaliperOY(int nIndex) const;            // 获取第nIndex个卡尺, [0-Y]
	scAffineRect GetCaliperYOPP(int nIndex) const;          // 获取第nIndex个卡尺, [Y-OPP]
	scAffineRect GetCaliperOPPX(int nIndex) const;          // 获取第nIndex个卡尺, [OPP-X]
	scAffineRect GetCaliperXO(int nIndex) const;            // 获取第nIndex个卡尺, [X-O]

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

	virtual BOOL SelectTest(CRect rect) ;			                    // 使用矩形框选中测试

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
	void DrawObround(CDC* pDC, const scAffineRect* pAffRect) const;
	void DrawCaliper(CDC* pDC, const scAffineRect* pAffRect) const;
	void DrawArrowNormal(CDC* pDC, const scAffineRect* pAffRect) const;
	void DrawArrowSelected(CDC* pDC, const scAffineRect* pAffRect) const;
    void UpdateLineCaliperDirection(const scAffineRect* pAffRect) const;

protected:
	scAffineRect* m_pAffineRect;
	scAffineRect* m_pAffineRectDrag;

	void*	m_pLShapeL[2];				// a pointer to the object
	void*	m_pLShapeC[2];				// a pointer to the object
	BOOL	m_bCaliperVisible;			// whether the caliper is visible or not

	TrackerHit	  m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[15];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色
};

#pragma pack(pop)
