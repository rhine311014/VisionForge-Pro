// svGuiGraphic.h

// ---------------------------------------------------------------------
//                            带旋转角度的十字控件
// ---------------------------------------------------------------------



#pragma once

#include "svGuiGraphic.h"
#include "svCrossPoint.h"

#pragma pack(push,8)

class SVGUI_API scGuiCoordCross : public scGuiGraphic
{
public:
	scGuiCoordCross(void);
	~scGuiCoordCross(void);

	virtual GraphicType GetType() const {return eGuiCoordCross;};

	/////////////////////////////////////////////////////////////////////////////
	// Handles:
	//						 |
	//						 |
	//						 |
	//						 |
	//					     |  rotation  \
	//			 ——————*———*——— X
	//					     |center	  /
	//					     |	
	//						 |
	//						 |
	//						\|/ Y
	/////////////////////////////////////////////////////////////////////////////
	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing	= -1,	// 未击中
		hitXEnd		= 0,	// X方向终点
		hitYEnd		= 1,    // Y方向终点		
		hitXRot		= 2,	// X方向旋转点
		hitXSeg		= 3,	// X方向线段
		hitYSeg		= 4		// Y方向线段
	};

	// 方向修饰箭头
	enum DiectionAdornment
	{
		eNone = 0,		// 无箭头
		eArrow,			// 普通箭头
		eSolidArrow		// 实心箭头
	};

	// 自由度
	enum GuiCoordCrossDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSizeX	= 1,	// 可改变X轴长度
		eGuiDOFSizeY    = 2,    // 可改变Y轴长度	
		eGuiDOFRotation	= 4,	// 可改变旋转角度        
	    eGuiDOFPosition = 8,    // 可改变位置
		eGuiDOFAll		= eGuiDOFSizeX|eGuiDOFSizeY|eGuiDOFRotation|eGuiDOFPosition, // 所有的自由度
	};
    

public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);
	
	void SetCrossPoint(scCrossPoint cp);

	// 设置十字控件的中心、旋转角度和长宽
	// 注意：长宽是以屏幕像素为单位的，显示时没有缩放
	void SetCenterRotationLengths(double dCenterX,			// 中心X坐标
								  double dCenterY,			// 中心Y坐标
								  double dRotation,			// 旋转角度
								  double dXLength,			// X方向长度
								  double dYLength);			// Y方向长度

	// 获取十字控件的中心、旋转角度和长宽
	void GetCenterRotationLengths(double& dCenterX,			// 中心X坐标
								  double& dCenterY,			// 中心Y坐标
								  double& dRotation,		// 旋转角度
								  double& dXLength,			// X方向长度
								  double& dYLength) const;	// Y方向长度


	// 设置十字控件的中心、旋转角度和长宽
	// 注意：长宽是以屏幕像素为单位的，显示时没有缩放
	void SetCenterRotationLengths(const sc2Vector& vCenter,		// 中心坐标
								  const scRadian&  rRotation,	// 旋转角度
								  double dXLength,				// X方向长度
								  double dYLength);				// Y方向长度

	// 获取十字控件的中心、旋转角度和长宽
	void GetCenterRotationLengths(sc2Vector& vCenter,			// 中心坐标
								  scRadian&  rRotation,			// 旋转角度
								  double& dXLength,				// X方向长度
								  double& dYLength) const;		// Y方向长度

	void SetCenter(const sc2Vector& vCenter) ;					// 设置中心			
	void SetRotation(const scRadian& rRotation);				// 设置旋转角度
	void SetXLength(double dXLength);							// 设置X方向长度
	void SetYLength(double dYLength);							// 设置Y方向长度

	sc2Vector GetCenter() const;								// 获取中心
	scRadian  GetRotation() const;								// 获取旋转角度
	double    GetXLength() const;								// 获取X方向长度
	double	  GetYLength() const;								// 获取Y方向长度

	// 设置、获取X方向的修饰箭头样式
	void SetXDirectionAdornment(DiectionAdornment dirAdorn);
	DiectionAdornment GetXDirectionAdornment() { return m_dirAdornX; };

	// 设置、获取Y方向的修饰箭头样式
	void SetYDirectionAdornment(DiectionAdornment dirAdorn);
	DiectionAdornment GetYDirectionAdornment() { return m_dirAdornY; };

	// 设置刻度是否存在
	void SetRulerVisible(bool bVisible);
	bool GetRulerVisible();

	// 设置刻度距离
	void SetRulerStep(double dStep);
	double GetRulerStep();

	// 设置、获取旋转角度是否可以交互改变
	void SetRotationEnable(bool bEnable);
	bool GetRotationEnable();

	// 设置、获取X方向长度是否可以交互改变
	void SetXLenEnable(bool bEnable);
	bool GetXLenEnable();

	// 设置、获取Y方向长度是否可以交互改变
	void SetYLenEnable(bool bEnable);
	bool GetYLenEnable();

public:
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
//	BOOL  IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;

protected:
	void* m_pLShape;
	void* m_pLShapeDrag;
	DiectionAdornment m_dirAdornX;
	DiectionAdornment m_dirAdornY;
	bool  m_bRotationEnable;
	bool  m_bXLenEnable;
	bool  m_bYLenEnable;
	TrackerHit	  m_hitResult;
	static HCURSOR g_hCursor[5];		// handles to the cursors

	DWORD			m_DOF;				// 自由度

	double m_dRulerStep;
	bool   m_bRulerVisible;
};

#pragma pack(pop)
