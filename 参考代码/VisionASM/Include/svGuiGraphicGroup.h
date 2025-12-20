// svGuiGraphicGroup.h

// ---------------------------------------------------------------------
//                        图形组
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svShapeGroup.h"

#pragma pack(push,8)

class SVGUI_API scGuiGraphicGroup : public scGuiGraphic
{
public:
	scGuiGraphicGroup();
	virtual ~scGuiGraphicGroup();

	virtual GraphicType GetType() const {return eGuiGraphicGroup;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitGraphic = 0
	};

	// 自由度
	enum GuiGraphicDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度	
		eGuiDOFPosition = 1,	// 可改变位置
		eGuiDOFAll		= eGuiDOFPosition // 所有的自由度
	};

public:
	//设置自由度
	void EnableGuiDOF(DWORD dof);

	// 设置/获取图形组
	BOOL SetShapeGroup(const scShapeGroup& group);
	scShapeGroup GetShapeGroup();
	
	// 设置选中序号
	void SetSelectIndex(int nIndex);

	scGuiGraphic* GetGuiGraphic(int nIndex);

	// 获取图形个数
	int	 GetGuiGraphicCount() const;

	virtual void SetLineStyle(StyleFlags style);
	virtual void SetLineWidth(int nWidth);
	virtual void SetLineColor(COLORREF color);

	virtual void SetLineStyle2(StyleFlags style);
	virtual void SetLineWidth2(int nWidth);
	virtual void SetLineColor2(COLORREF color);

	virtual void SetSelectedLineStyle(StyleFlags style);
	virtual void SetSelectedLineWidth(int nWidth);
	virtual void SetSelectedLineColor(COLORREF color);

	virtual void SetDraggingLineStyle(StyleFlags style);
	virtual void SetDraggingLineWidth(int nWidth);
	virtual void SetDraggingLineColor(COLORREF color);

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
protected:
	void LoadCursor();
	void InitTipText();
	void FreeGraphic();
	void Invalidate();
	
	scGuiGraphic* CreateGraphicFromShape(scShape* pShape);
	scShape* CreateShapeFromGraphic(scGuiGraphic* pGuiGraphic);

protected:
	std::vector<scGuiGraphic*>	m_vpGuiGraphic;
	std::vector<int>			m_viProperty;

	CRITICAL_SECTION	m_csCriticalSection;

	int		m_nSelectIndex;

	DWORD		m_DOF;
	TrackerHit  m_hitResult;
	static HCURSOR g_hCursor[1];
};

#pragma pack(pop)
