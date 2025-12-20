// svGuiCalibration3D.h

// ---------------------------------------------------------------------
//                            标定块交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svAffineRect.h"

#pragma pack(push,8)


class SVGUI_API scGuiCalibration3D : public scGuiGraphic  
{
public:
	scGuiCalibration3D();
	virtual ~scGuiCalibration3D();

	virtual GraphicType GetType() const {return eGuiCalibration3D;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		//顶面操作点
		hitTopOrigin = 0, hitTopCornerX = 1, hitTopOpposite = 2, hitTopCornerY = 3,
		hitTopSideX = 4, hitTopParallelY = 5, hitTopParallelX = 6, hitTopSideY = 7,
		//斜面操作点
		hitBevelOrigin = 8, hitBevelCornerX = 9, hitBevelOpposite = 10, hitBevelCornerY = 11,
		hitBevelSideX = 12, hitBevelParallelY = 13, hitBevelParallelX = 14, hitBevelSideY = 15,
		//底面操作点
		hitBottomOrigin = 16, hitBottomCornerX = 17, hitBottomOpposite = 18, hitBottomCornerY = 19,
		hitBottomSideX = 20, hitBottomParallelY = 21, hitBottomParallelX = 22, hitBottomSideY = 23,
		hitRotation = 24,
		hitCenterCross = 25
	};

	// 自由度
	enum GuiAffineRectDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFRotation	= 4,	// 可改变旋转角度
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFRotation, // 所有的自由度
	};

public:	
	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition|eRotation）
	void EnableGuiDOF(DWORD dof);

	//设置仿射矩形
	void SetAffineRectTop(scAffineRect affRect);
	void SetAffineRectBevel(scAffineRect affRect);
	void SetAffineRectBottom(scAffineRect affRect);

	scAffineRect GetAffineRectTop() const;
	scAffineRect GetAffineRectBevel() const;
	scAffineRect GetAffineRectBottom() const;

	// 设置中心十字线尺寸
	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	
	sc2Vector GetetCenterCrossExtent() const;

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

protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

	void  LoadCursor();
	void  InitTipText();
	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;
protected:
	scAffineRect*	m_pAffineRect;
	scAffineRect*	m_pAffineRectDrag;

	sc2Vector	m_vCenterCrossSize;
	sc2Vector	m_vCenterCrossSizeDrag;

	TrackerHit		m_hitResult;
	CPoint			m_ptCur;
	static HCURSOR	g_hCursor[25];
	DWORD			m_DOF;
};

#pragma pack(pop)
