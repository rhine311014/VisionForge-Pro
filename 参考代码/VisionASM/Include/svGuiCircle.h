// svGuiCircle.h


// ---------------------------------------------------------------------
//                            圆形交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svCircle.h"

#pragma pack(push,8)


class SVGUI_API scGuiCircle  : public scGuiGraphic
{
public:
	scGuiCircle();
	virtual ~scGuiCircle();

	virtual GraphicType GetType() const {return eGuiCircle;};

	////////////////////////////////////////////////////////////
	//            *  *
	//        *         *
	//      *             *
	//     *               *
	//     *      *center  *right
	//     *               *
	//      *             *
	//        *         *
	//            *  *
	/////////////////////////////////////////////////////////
	enum TrackerHit
	{
		hitNothing = -1, 
		hitRight = 0, 
		hitBottom = 1, 
		hitLeft = 2, 
		hitTop = 3, 
		//十字中心X边、十字中心Y边
		hitCenterXSeg = 4,
		hitCenterYSeg = 5,
		hitCircle = 6										
	};

	// 透明度
	enum Transparency
	{
		transparencyNone = 0,			// 不透明
		transparencyHalf = 50,			// 半透明
		transparencyFull = 100			// 全透明
	};

	// 自由度
	enum GuiCircleDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变大小
		eGuiDOFPosition	= 2,	// 可改变位置	
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition, // 所有的自由度
	};


public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	void  SetCenterRadius(double dCenterX, double dCenterY, double dRadius);          // 设置圆心与半径	
	void  GetCenterRadius(double &dCenterX, double &dCenterY, double &dRadius) const; // 获取圆心与半径 
	
	void  SetCenterRadius(const sc2Vector& vCenter, double dRadius);				  // 设置圆心与半径	
	void  GetCenterRadius(sc2Vector& vCenter, double &dRadius) const;				  // 获取圆心与半径 

	void	 SetCircle(const scCircle& circle);										  // 设置圆	
	scCircle GetCircle() const;														  // 获取圆

	void SetCenterCrossExtent(const sc2Vector& vSize = sc2Vector(16,16));	// 设置中心十字线尺寸（宽度和高度）
	sc2Vector GetetCenterCrossExtent() const;								// 获取中心十字线尺寸（宽度和高度）

	virtual BOOL BoundingRect(CRect& rect) const;

	CString  GetTipText() const;
	CString	 GetTipText(int nHandle) const;
	CString	 GetTipText(CPoint point) const;

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

	int   HitTestHandles(CPoint point) const;
	CRect GetHandleRect(TrackerHit nHandle) const;
	void  LoadCursor();
	void  InitTipText();

protected:

	scCircle*	m_pCircle;
	scCircle*	m_pCircleDrag;
	TrackerHit m_hitResult;
	static HCURSOR g_hCursor[7];
	
	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	sc2Vector	m_vCenterCrossSize;				// 
	sc2Vector	m_vCenterCrossSizeDrag;			// 
};

#pragma pack(pop)
