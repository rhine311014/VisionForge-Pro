// svPolyline.h

// ---------------------------------------------------------------------
//                            多边形交互控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "svPolyline.h"

#pragma pack(push,8)


class SVGUI_API scGuiPolyline : public scGuiGraphic 
{
public:
	scGuiPolyline();
	virtual ~scGuiPolyline();

	virtual GraphicType GetType() const {return eGuiPolyline;};

	enum TrackerHit
	{
		hitNothing = -1,   // 未击中
		hitVertex = 0,	   // 击中了点
		hitLine			   // 击中了线段
	};

	// 自由度
	enum GuiPolylineDOF
	{
		eGuiDOFNone		= 0,	// 禁止所有的自由度
		eGuiDOFSize		= 1,	// 可改变宽、高
		eGuiDOFPosition	= 2,	// 可改变位置		
		eGuiDOFAll		= eGuiDOFSize|eGuiDOFPosition, // 所有的自由度
	};

public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eSize|ePosition）
	void EnableGuiDOF(DWORD dof);

	void SetClosed(bool bClosed = true);					 // 设置多边形是否闭合
	bool IsClosed() const;									 // 获取多边形是否闭合
	bool SetVertex(int nIndex, double dX,double dY);         // 改变点坐标
	bool GetVertex(int nIndex, double& dX,double& dY) const; // 返回点位置
	int  GetVertexNum() const;                               // 获取点的个数
	int	 GetLineSegNum() const;								 // 获取边的数量
                           

	bool InsertVertex(int nIndex, double dX, double dY);	 // 在第nIndex处插入点（dX，dY）
	bool InsertVertex(int nIndex);							 // 在第nIndex处插入点，其坐标为其前后两点的中点坐标

	bool AddVertex(double dX, double dY);					 // 在多边形顶点序列的末尾加入点，并设置点的坐标
	bool AddVertex();										 // 在多边形顶点序列的末尾加入点，其坐标为其前后两点的中点坐标

	bool RemoveVertex(int nIndex);							 // 删除第nIndex个点
	bool RemoveVertices();									 // 删除所有点 

	void SetPolyline(const scPolyline& polyline);			 // 设置多边形
	scPolyline GetPolyline() const;							 // 获取多边形

	// 当多边形处于选中状态时，设置多边形第nIndex点被点击中,编号范围[0,GetVertexNum()-1]
	bool SetHitVertex(int nIndex);		

	// 当多边形处于选中状态时，设置多边形第nIndex边被点击中,
	// 多边形闭合时编号范围[0,GetVertexNum()-1],开时编号范围是[0,GetVertexNum()-2]；
	bool SetHitLine(int nIndex);	

	// 返回被点击的点编号，编号范围[0,GetVertexNum()-1]；
	// 若未被击中则返回-1 
	int  GetHitVertex() const;                             
	
	// 返回被点击的线段编号，多边形闭合时编号范围[0,GetVertexNum()-1],开时编号范围是[0,GetVertexNum()-2]；
	// 若未被击中则返回-1
	int  GetHitLine() const;

	bool ReorderVertex(int nIdx);							// 顶点数据重新排序，从第nIdex个顶点分割，将第nIdx+1个顶点作为起始点

	bool InsertVertex(int nIdx, sc2Vector point);			// 在第nIdex个顶点后插入point到线段的投影点

	virtual BOOL BoundingRect(CRect& rect) const;

	CString  GetTipText() const;
	CString	 GetTipText(int nHandle) const;
	CString	 GetTipText(CPoint point) const;

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	int  HitTest(CPoint point) const;
	void HitTestHandles(CPoint point,TrackerHit& hitResult,int& nPos) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	BOOL DragStart(CPoint point);
	void DragAnimate(CPoint ptStart, CPoint ptStop);
	void DragStop();
	void DragCancel();

	void	SetFillEnabled(BOOL bEnable);								// 设置是否允许填充
	BOOL	IsFillEnabled() const;										// 获取是否允许填充

	void    SetFillColor( COLORREF color);								// 设置填充的颜色
	COLORREF GetFillColor() const;										// 获取填充的颜色

	void SetFirstVertexColor(COLORREF color);							// 设置第一个顶点Handle颜色
	COLORREF GetFirstVertexColor() const;								// 获取第一个顶点Handle颜色

	void SetLastVertexColor(COLORREF color);							// 设置最后一个顶点Handle颜色
	COLORREF GetLastVertexColor() const;								// 获取最后一个顶点Handle颜色

	void EnableParamEditMenu(BOOL bEnable);								// 设置是否允许鼠标右键菜单编辑控件
	BOOL IsEnableParamEditMenu();										// 获取是否允许鼠标右键菜单编辑控件

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

//	BOOL IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;
	void LoadCursor();
	void AddTipText(int Index); 
	void RemoveTipText(int Index);

protected:
	scPolyline* m_pPolyline;
	scPolyline* m_pPolylineDrag;

	TrackerHit	m_hitResult;
	int			m_nHitPos;
	static HCURSOR g_hCursor[2];

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

	COLORREF		m_cFirstVertexColor;
	COLORREF		m_cLastVertexColor;

	BOOL			m_bEnableParamEditMenu;

	CRITICAL_SECTION	m_csCriticalSection;
};

#pragma pack(pop)
