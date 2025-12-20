// svGuiFindGenRect.h

// ---------------------------------------------------------------------
//                            找一般圆角矩形交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"
#include "svCircularArc.h"
#include "svGenRect.h"

#pragma pack(push,8)

class SVGUI_API scGuiFindGenRect : public scGuiGraphic  
{
public:
	scGuiFindGenRect();
	virtual ~scGuiFindGenRect();

	virtual GraphicType GetType() const {return eGuiFindGenRect;};

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
		//圆弧
		hitRound = 9,
		// 标记点
		hitMarkerXO = 10, hitMarkerOY = 11, hitMarkerPo = 12,
		// 卡尺镜像点
		hitMirror = 13,
	};

	// 自由度
	enum GuiAffineRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFRound	= 8,	// 可改变圆弧半径长度
		eGuiDOFMarker   = 16,	// 可改变卡尺
		eGuiDOFMirror	= 32,	// 可卡尺镜像
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation|eGuiDOFRound|eGuiDOFMarker|eGuiDOFMirror, // 所有的自由度
	};

public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	// 设置仿射矩形
	void SetCenterLengthsRotationRound(double dCenterX,			// 中心点的X坐标
		double dCenterY,			// 中心点的Y坐标
		double dSideXLength,		// X边长度
		double dSideYLength,		// Y边长度
		double dRotation,			// 旋转角度（单位弧度）
		double dRound);		

	// 获取仿射矩形
	void GetCenterLengthsRotationRound(double& dCenterX,			// 中心点的X坐标 
		double& dCenterY,			// 中心点的Y坐标
		double& dSideXLength,		// X边长度 
		double& dSideYLength,		// Y边长度 
		double& dRotation,			// 旋转角度（单位弧度）
		double& dRound) const;	


	// 设置仿射矩形
	void SetCenterLengthsRotationRound(const sc2Vector& vCenter,	  // 中心点
		double dSideXLength,		  // X边长度
		double dSideYLength,		  // Y边长度
		const scRadian& rRotation,	 // 旋转角度
		double dRound);		

	// 获取仿射矩形
	void GetCenterLengthsRotationRound(sc2Vector& vCenter,		  // 中心点
		double& dSideXLength,		  // X边长度
		double& dSideYLength,		  // Y边长度
		scRadian& rRotation,		  // 旋转角度
		double& dRound) const; 


	void SetAffineRect(const scAffineRect& affrect, double dRound = 0);		// 设置仿射矩形，默认认为skew=0
	scAffineRect GetAffineRect() const;										// 获取仿射矩形
	double GetRound() const;												// 获取圆弧半径

	void		SetGenRect(const scGenRect& genRect);
	scGenRect	GetGenRect() const;

	void	SetCaliperParams(int nCaliperNum = 3,
		CaliperSearchDirection nDirection = eInward,	// 设置卡尺搜索方向
		double dSearchLength = 60.0,					// 设置卡尺搜索长度			 
		double dProjectionLength = 20.0);			    // 设置卡尺投影长度

	void	SetCaliperNum(int nCaliperNum = 3);										// 设置卡尺数量											
	void	SetCaliperSearchLength(double dSearchLength = 60.0);						// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength = 20.0);				// 设置卡尺投影长度
	void	SetCaliperSearchDirection(CaliperSearchDirection nDirection = eInward);   // 设置卡尺搜索方向



	void	SetCaliperParamsLineX(int nCaliperNum = 3,
							 CaliperSearchDirection nDirection = eInward,	// 设置卡尺搜索方向
							 double dSearchLength = 60.0,					// 设置卡尺搜索长度			 
							 double dProjectionLength = 20.0);			    // 设置卡尺投影长度

	
	void	GetCaliperParamsLineX(int& nCaliperNum,
							 CaliperSearchDirection& nDirection,	//卡尺搜索方向
							 double& dSearchLength,					//卡尺搜索长度			 
							 double& dProjectionLength);			//卡尺投影长度	

	void	SetCaliperParamsLineY(int nCaliperNum = 3,
		CaliperSearchDirection nDirection = eInward,	// 设置卡尺搜索方向
		double dSearchLength = 60.0,					// 设置卡尺搜索长度			 
		double dProjectionLength = 20.0);			    // 设置卡尺投影长度

	void	GetCaliperParamsLineY(int& nCaliperNum,
		CaliperSearchDirection& nDirection,	//卡尺搜索方向
		double& dSearchLength,					//卡尺搜索长度			 
		double& dProjectionLength);			//卡尺投影长度


	void	SetCaliperParamsCircle(int nCaliperNum = 3,
		CaliperSearchDirection nDirection = eInward,	// 设置卡尺搜索方向
		double dSearchLength = 60.0,					// 设置卡尺搜索长度			 
		double dProjectionLength = 20.0);			    // 设置卡尺投影长度

	void	GetCaliperParamsCircle(int& nCaliperNum,
		CaliperSearchDirection& nDirection,		//卡尺搜索方向
		double& dSearchLength,					//卡尺搜索长度			 
		double& dProjectionLength);				//卡尺投影长度	


	void	SetCaliperNumLineX(int nCaliperNum = 3);										// 设置卡尺数量											
	void	SetCaliperSearchLengthLineX(double dSearchLength = 60.0);						// 设置卡尺搜索长度
	void	SetCaliperProjectionLengthLineX(double dProjectionLength = 20.0);				// 设置卡尺投影长度
	void	SetCaliperSearchDirectionLineX(CaliperSearchDirection nDirection = eInward);   // 设置卡尺搜索方向


	int	    GetCaliperNumLineX()   const;													// 获取卡尺数量
	double	GetCaliperSearchLengthLineX()		const;										// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthLineX()	const;										// 获取卡尺投影长度
	int		GetCaliperSearchDirectionLineX()		const;									// 获取卡尺搜索方向


	void	SetCaliperNumLineY(int nCaliperNum = 3);										// 设置卡尺数量											
	void	SetCaliperSearchLengthLineY(double dSearchLength = 60.0);						// 设置卡尺搜索长度
	void	SetCaliperProjectionLengthLineY(double dProjectionLength = 20.0);				// 设置卡尺投影长度
	void	SetCaliperSearchDirectionLineY(CaliperSearchDirection nDirection = eInward);   // 设置卡尺搜索方向


	int	    GetCaliperNumLineY()   const;													// 获取卡尺数量
	double	GetCaliperSearchLengthLineY()		const;										// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthLineY()	const;										// 获取卡尺投影长度
	int		GetCaliperSearchDirectionLineY()		const;									// 获取卡尺搜索方向


	void	SetCaliperNumCircle(int nCaliperNum = 3);// 设置卡尺数量											
	void	SetCaliperSearchLengthCircle(double dSearchLength = 60.0);				     // 设置卡尺搜索长度
	void	SetCaliperProjectionLengthCircle(double dProjectionLength = 20.0);		     // 设置卡尺投影长度
	void	SetCaliperSearchDirectionCircle(CaliperSearchDirection nDirection = eInward);  // 设置卡尺搜索方向

	int	    GetCaliperNumCircle()   const;												// 获取卡尺数量
	double	GetCaliperSearchLengthCircle()		const;									// 获取卡尺搜索长度
	double	GetCaliperProjectionLengthCircle()	const;									// 获取卡尺投影长度
	int		GetCaliperSearchDirectionCircle()		const;									// 获取卡尺搜索方向


	void	SetCaliperVisibleLineX(BOOL bVisible = TRUE);					  // 设置卡尺是否可见
	BOOL	IsCaliperVisibleLineX()				const;			              // 获取卡尺是否可见

	void	SetCaliperVisibleLineY(BOOL bVisible = TRUE);					  // 设置卡尺是否可见
	BOOL	IsCaliperVisibleLineY()				const;			              // 获取卡尺是否可见

	void	SetCaliperVisibleCircle(BOOL bVisible = TRUE);					  // 设置卡尺是否可见
	BOOL	IsCaliperVisibleCircle()				const;			             // 获取卡尺是否可见


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
	
	BOOL          IsNormalAffineRect(const scGenRect* pGenRect) const;

	scCircularArc GetCircularArcPo(const scGenRect* pGenRect) const;
	scCircularArc GetCircularArcPx(const scGenRect* pGenRect) const;
	scCircularArc GetCircularArcPy(const scGenRect* pGenRect) const;
	scCircularArc GetCircularArcPopp(const scGenRect* pGenRect) const;
	scLineSeg GetLineSegOY(const scGenRect* pGenRect) const;
	scLineSeg GetLineSegYOPP(const scGenRect* pGenRect) const;
	scLineSeg GetLineSegOPPX(const scGenRect* pGenRect) const;
	scLineSeg GetLineSegXO(const scGenRect* pGenRect) const;




	void DrawGenRect(CDC* pDC, const scGenRect* pGenRect) const;
	void DrawCaliper(CDC* pDC, const scAffineRect* pAffRect) const;
	void DrawArrowNormal(CDC* pDC, const scAffineRect* pAffRect) const;
	void DrawArrowSelected(CDC* pDC, const scAffineRect* pAffRect) const;

	void UpdateLineCaliperDirection(const scGenRect* pGenRect) const;
    void UpdateLineCaliperDirectionLineX(const scGenRect* pGenRect) const;
	void UpdateLineCaliperDirectionLineY(const scGenRect* pGenRect) const;
	void UpdateLineCaliperDirectionCircle(const scGenRect* pGenRect) const;

protected:
	scGenRect* m_pGenRect;
	scGenRect* m_pGenRectDrag;

	void*	m_pLShapeL[4];				// a pointer to the object
	void*	m_pLShapeC[4];				// a pointer to the object
	BOOL	m_bCaliperVisibleLineX;			// whether the caliper is visible or not
	BOOL	m_bCaliperVisibleLineY;			// whether the caliper is visible or not
	BOOL	m_bCaliperVisibleCircle;

	CaliperSearchDirection m_nCaliperSearchDirectionLineX;
	CaliperSearchDirection m_nCaliperSearchDirectionLineY;
	CaliperSearchDirection m_nCaliperSearchDirectionCircle;

	TrackerHit	  m_hitResult;
	CPoint  m_ptCur;
	static HCURSOR g_hCursor[15];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色
};

#pragma pack(pop)
