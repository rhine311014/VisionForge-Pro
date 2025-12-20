// svPolylineArc.h

// ---------------------------------------------------------------------
//                            多段线交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svPolylineArc.h"

#pragma pack(push,8)


class SVGUI_API scGuiPolylineArc : public scGuiGraphic 
{
public:
	scGuiPolylineArc();
	virtual ~scGuiPolylineArc();

	virtual GraphicType GetType() const {return eGuiPolylineArc;};

	enum TrackerHit
	{
		hitNothing	= -1,	// 未击中
		hitVertex	= 0,	// 击中顶点
		hitBend		= 1,	// 击中弧中点
		hitLineArc	= 2,	// 击中线、弧
	};

	// 自由度
	enum GuiPolylineArcDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变顶点
		eGuiDOFPosition	= 2,	// 可改变位置
		eGuiDOFConvex	= 4,	// 可改变凸度
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition|eGuiDOFConvex, // 所有的自由度
	};

public:
	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	void SetClosed(bool bClosed = true);					 // 设置多段线是否闭合
	bool IsClosed() const;									 // 获取多段线是否闭合

	bool SetVertexData(int nIndex, scVertexData data);			// 改变顶点数据
	bool GetVertexData(int nIndex, scVertexData& data) const;	// 返回顶点数据

	bool AddVertex(scVertexData data);						 // 在末尾加入点，并设置点的坐标

	bool InsertVertex(int nIndex, scVertexData data);		// 在第nIndex处插入点

	bool RemoveVertex(int nIndex);							 // 删除第nIndex个点

	void RemoveVertices();									 // 删除所有点 

	void SetPolylineArc(const scPolylineArc& polylineArc);	// 设置多段线
	scPolylineArc GetPolylineArc() const;					// 获取多段线

	int  GetVertexNum() const;                               // 获取点的个数
	int	 GetLineArcNum() const;								 // 获取边的数量

	void	SetFillEnabled(BOOL bEnable);								// 设置是否允许填充
	BOOL	IsFillEnabled() const;										// 获取是否允许填充

	void    SetFillColor( COLORREF color);								// 设置填充的颜色
	COLORREF GetFillColor() const;										// 获取填充的颜色

	void SetFirstVertexColor(COLORREF color);				// 设置第一个顶点Handle颜色
	COLORREF GetFirstVertexColor() const;					// 获取第一个顶点Handle颜色

	void SetLastVertexColor(COLORREF color);				// 设置最后一个顶点Handle颜色
	COLORREF GetLastVertexColor() const;					// 获取最后一个顶点Handle颜色

	void EnableParamEditMenu(BOOL bEnable);					// 设置是否允许鼠标右键菜单编辑控件
	BOOL IsEnableParamEditMenu();							// 获取是否允许鼠标右键菜单编辑控件

	bool ReorderVertex(int nIdx);							// 顶点数据重新排序，从第nIdex个顶点分割，将第nIdx+1个顶点作为起始点

	bool InsertVertex(int nIdx, sc2Vector point);			// 在第nIdex个顶点后插入point到线段/弧线的投影点

	virtual BOOL BoundingRect(CRect& rect) const;

	CString  GetTipText() const;
	CString	 GetTipText(int nHandle, int nPos) const;
	CString	 GetTipText(CPoint point) const;

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	int  HitTest(CPoint point) const;
	void HitTestHandles(CPoint point,int& nHitRes,int& nPos) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	BOOL DragStart(CPoint point);
	void DragAnimate(CPoint ptStart, CPoint ptStop);
	void DragStop();
	void DragCancel();

protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawPolylineArc(CDC* pDC, scPolylineArc* pPolylineArc) const;

	void LoadCursor();
	void InitTipText();

protected:
	scPolylineArc* m_pPolylineArc;
	scPolylineArc* m_pPolylineArcDrag;

	TrackerHit	m_hitResult;
	int			m_nHitPos;
	static HCURSOR g_hCursor[3];

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	COLORREF		m_cFirstVertexColor;
	COLORREF		m_cLastVertexColor;

	BOOL			m_bEnableParamEditMenu;

	CRITICAL_SECTION	m_csCriticalSection;
};

#pragma pack(pop)
