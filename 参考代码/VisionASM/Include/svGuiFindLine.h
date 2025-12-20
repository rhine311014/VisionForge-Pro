// svGuiFindLine.h

// ---------------------------------------------------------------------
//                           直线合交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svLineSeg.h"
#include "svAffineRect.h"

#pragma pack(push,8)


class SVGUI_API scGuiFindLine : public scGuiGraphic  
{
public:
	scGuiFindLine();
	virtual ~scGuiFindLine();

	virtual GraphicType GetType() const {return eGuiFindLine;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//
	//								 ---------+	Marker
	//								|         |
	//								|         |
	//					 Start *---------------------* End
	//								|		  |
	//								|		 \|/
	//								 ----+----
	//									  Rotation
	//
	/////////////////////////////////////////////////////////////////////////////

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,	// 未击中	
		hitStart = 0, 		// 起始点
		hitEnd,				// 终点
		hitMarker,			// 标记点
		hitMirror,			// 卡尺镜像点
		hitRotation,		// 旋转
		hitCliperLine,		// 卡尺线
		hitMaskLine,		// 掩模线
		hitMaskRegion,
		hitMaskSideStart,
		hitMaskSideEnd,
		hitRectangleSideX,	// 矩形框X边
		hitRectangleSideY,	// 矩形框Y边
		hitRectangleRegion,	// 矩形区域
	};

	// 自由度
	enum GuiFindLineDOF
	{
		eGuiDOFNone	    = 0,	// 禁止所有的自由度	
		eGuiDOFStart	= 1,	// 可改变起始点
		eGuiDOFEnd      = 2,    // 可改变终止点
		eGuiDOFMarker   = 4,    // 可改变卡尺形状
		eGuiDOFRotation = 8,    // 可改变搜索方向
		eGuiDOFPosition = 16,   // 可改变位置
		eGuiDOFMask	= 32,		// 可掩模
		eGuiDOFMirror = 64,		// 可卡尺镜像
		eGuiDOFAll		= eGuiDOFStart|eGuiDOFEnd|eGuiDOFMarker|eGuiDOFPosition|eGuiDOFMask|eGuiDOFMirror, // 所有的自由度
	};


public:	

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	void SetGuiDOFMaskEnable(BOOL bEnable);			// 设置是否允许掩模dof
	BOOL IsGuiDOFMaskEnable() const;				// 获取是否允许掩模dof

	//	设置线段参数
	void SetStartXYEndXY(double dStartX,			// 起始点X坐标
						 double dStartY,			// 起始点Y坐标
						 double dEndX,				// 终点X坐标
						 double dEndY);				// 终点Y坐标
	// 获取线段参数
	void GetStartXYEndXY(double& dStartX,			// 起始点X坐标
					 	 double& dStartY,			// 起始点Y坐标
						 double& dEndX,				// 终点X坐标
						 double& dEndY) const;		// 终点Y坐标
	
	//	设置线段参数
	void SetStartXYEndXY(const sc2Vector& vStart, 	// 起始点
						 const sc2Vector& vEnd);	// 终点

	// 获取线段参数
	void GetStartXYEndXY(sc2Vector& vStart, 		// 起始点
						 sc2Vector& vEnd) const;	// 终点

	void	SetLineSeg(const scLineSeg& lineSeg);   // 设置线段
	scLineSeg GetLineSeg() const;					// 获取线段

	void	SetCaliperParams(int	nCaliperNum,						// 设置卡尺数量
							const scRadian& rSearchDirection,			// 设置卡尺搜索方向
							double dSearchLength,						// 设置卡尺搜索长度							 
							double dProjectionLength,					// 设置卡尺投影长度
							BOOL	bVisible = TRUE);					// 设置卡尺是否可见

	void	SetCaliperNum(int nCaliperNum);								// 设置卡尺数量				
	void	SetCaliperSearchDirection(const scRadian& rSearchDirection);// 设置卡尺搜索方向
	void	SetCaliperSearchLength(double dSearchLength);				// 设置卡尺搜索长度
	void	SetCaliperProjectionLength(double dProjectionLength);		// 设置卡尺投影长度
	void	SetCaliperVisible(BOOL bVisible);							// 设置卡尺是否可见
	void	SetLineSegArrowVisible(BOOL bVisible);						// 设置线段箭头是否可见
	void	SetAffineRectFindLineVisible(BOOL bVisible);				// 设置找线工具矩形区域是否可见

	int		GetCaliperNum()					const;						// 获取卡尺数量
	scRadian GetCaliperSearchDirection()		const;					// 获取卡尺搜索方向
	double	GetCaliperSearchLength()		const;						// 获取卡尺搜索长度
	double	GetCaliperProjectionLength()	const;						// 获取卡尺投影长度
	BOOL	IsCaliperVisible()				const;						// 获取卡尺是否可见
	BOOL	IsLineSegArrowVisible()			const;						// 获取线段箭头是否可见
	BOOL	IsAffineRectFindLineVisible()	const;						// 获取找线工具矩形区域是否可见

	void	SetMaskData(std::vector<double> vMaskData);					// 设置工具掩模数据
	void	GetMaskData(std::vector<double>& vMaskData) const;			// 获取工具掩模数据
	void	GetCaliperMasked(std::vector<bool>& vMasked) const;			// 获取卡尺是否被掩模覆盖
	void	GetCaliperMasked(std::vector<BOOL>& vMasked) const;			// 获取卡尺是否被掩模覆盖

	// 获取第nIndex个卡尺的参数
	void	GetCaliper(int	   nIndex,									// 卡尺序号，序号范围从0到nCaliperNum-1
						double& dCenterX,								// 仿射矩形（卡尺）中心点的X坐标  
						double& dCenterY,								// 仿射矩形（卡尺）中心点的Y坐标 
						double& dSideXLength,							// 仿射矩形（卡尺）X边长度 
						double& dSideYLength,							// 仿射矩形（卡尺）Y边长度 
						scRadian& rRotation,							// 仿射矩形（卡尺）旋转角度（单位弧度）
						scRadian& rSkew)	const;						// 仿射矩形（卡尺）切变角度（单位弧度）
	scAffineRect GetCaliper(int nIndex) const;							// 获取第nIndex个卡尺

	BOOL    SetCaliperSpacing(double dSpacing);							// 设置卡尺间距
	double  GetCaliperSpacing() const;									// 获取卡尺间距

	void	SetDragSpacingChange(BOOL bChange);							// 设置拖拽起始点时卡尺间距是否改变
	BOOL	GetDragSpacingChange();

	scAffineRect   GetAffineRectFindLine() const;						// 获取找线工具仿射矩形

	void	EnableFindLineParamEdit(BOOL bEnable);
	BOOL	IsEnableFindLineParamEdit();
	BOOL    SetFindLineParam(bool bOneEdgeOrTwoEdge,double dContrastThreshold,EdgePolarity nEdgePolarity1,EdgePolarity nEdgePolarity2,long lFilterSize,long nOutLinePrecent,bool bSortByScore);	
	void    GetFindLineParam(bool& bOneEdgeOrTwoEdge,double& dContrastThreshold,EdgePolarity& nEdgePolarity1,EdgePolarity& nEdgePolarity2,long& lFilterSize,long& nOutLinePrecent,bool& bSortByScore);	

	virtual BOOL BoundingRect(CRect& rect) const;

	CString GetTipText() const;
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
	double GetMaskOffset(void* pLine, sc2Vector point) const;
	sc2Vector GetMaskPoint(void* pLine, double dOffset) const;

protected:
	void*	m_pLShape;					// a pointer to the object
	void*	m_pLShapeDrag;
	CPoint  m_ptCur;
	TrackerHit     m_hitResult;
	static HCURSOR g_hCursor[13];		// handles to the cursors
	BOOL	m_bCaliperVisible;			// whether the caliper is visible or not
	BOOL	m_bLineSegArrowVisible;		// whether the LineSegArrow is visible or not
	BOOL	m_bAffineRectFindLineVisible;
	BOOL	m_bDragSpacingChange;
	
	DWORD	        m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	std::vector<sc2Vector> m_vMaskRange;
	std::vector<sc2Vector> m_vMaskRangeDrag;

	mutable int m_nSelectedMaskRegionIndex;
	CPoint  m_ptDragStart;

	BOOL m_bEnableFindLineParamEdit;
	//找线工具参数
	bool m_bOneEdgeOrTwoEdge;		//true:OneEdge; false:TwoEdge
	double m_dContrastThreshold;
	EdgePolarity m_nEdgePolarity1;
	EdgePolarity m_nEdgePolarity2;
	long m_lFilterSize;
	long m_nOutLinePrecent;
	bool m_bSortByScore;
};

#pragma pack(pop)
