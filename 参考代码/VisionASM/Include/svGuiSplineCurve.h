
// ---------------------------------------------------------------------
//                        样条曲线交互图形
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svSplineCurve.h"

#pragma pack(push,8)

class SVGUI_API scGuiSplineCurve : public scGuiGraphic
{
public:
	scGuiSplineCurve();
	virtual ~scGuiSplineCurve();

	virtual GraphicType GetType() const {return eGuiSplineCurve;};

	enum TrackerHit
	{
		hitNothing = -1,		// 未击中
		hitFitPoint = 0,		// 击中了拟合点
		hitControlPoint	= 1,	// 击中了控制点
		hitSpline = 2,			// 击中了曲线
	};

	// 自由度
	enum GuiPolylineDOF
	{
		eGuiDOFNone				= 0,		// 禁止所有的自由度
		eGuiDOFFitPoints		= 1,		// 可改变拟合点坐标
		eGuiDOFControlPoints	= 2,		// 可改变控制点坐标		
		eGuiDOFPosition		    = 4,		// 可改变位置，平移
		eGuiDOFAll				= eGuiDOFFitPoints|eGuiDOFControlPoints|eGuiDOFPosition, // 所有的自由度
	};

public:

	//设置自由度（注：输入参数可通过运算符"|"合并不同的自由度如:eFitPoints|eControlPoints|ePosition）
	void EnableGuiDOF(DWORD dof);

	// 设置/获取样条曲线的节点向量、控制点、权重、阶次、以及开闭
	void SetSplineCurveInfo(const std::vector<double>& vNode,const std::vector<sc2Vector>& vControlPoints,const std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose = false);
	void GetSplineCurveInfo(std::vector<double>& vNode,std::vector<sc2Vector>& vControlPoints,std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose)const;
	
	// 设置/获取样条曲线的控制点、权重、阶次、以及开闭
	void SetSplineCurveInfo(const std::vector<sc2Vector>& vControlPoints,const std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose = false);
	void GetSplineCurveInfo(std::vector<sc2Vector>& vControlPoints,std::vector<double>& vWeight,int nOrder,bool bFlagOpenClose)const;

	// 设置/获取样条曲线的拟合点、阶次、权重、节点计算方法、以及开闭
	void SetSplineCurveInfo(const std::vector<sc2Vector>& vFitPoints,int nOrder,const std::vector<double>& vWeight,NodeMethod method = SubtenseAdd,bool bFlagOpenClose = false);
	void GetSplineCurveInfo(std::vector<sc2Vector>& vFitPoints,int nOrder,std::vector<double>& vWeight,NodeMethod method ,bool bFlagOpenClose)const;

	// 获取样条曲线的节点向量、控制点、拟合点、权重、阶次、以及开闭
	void GetSplineCurveInfo(std::vector<double>& vNode,std::vector<sc2Vector>& vControlPoints,std::vector<sc2Vector>& vFitPoints,std::vector<double>& vWeight,int nOrder,
							NodeMethod method,bool bFlagOpenClose)const;

	void SetClosed(bool bClosed = true);																	// 设置样条曲线是否闭合
	bool IsClosed() const;																					// 获取样条曲线是否闭合

	bool SetControlPoints(const std::vector<sc2Vector>& vPoints,const std::vector<double>& vWeight);		// 重新设置控制点和权重
	bool SetControlPoint(int nIndex, double dX,double dY,double dWeight = 1.0);								// 改变控制点坐标和权重
	bool GetControlPoint(int nIndex, double& dX,double& dY) const;											// 返回控制点坐标
	int  GetControlPointNum() const;																		// 获取控制点的个数

	bool InsertControlPoint(int nIndex, double dX, double dY,double dWeight = 1.0);							// 在第nIndex处插入控制点（dX，dY）
	bool AddControlPoint(double dX, double dY,double dWeight = 1.0);										// 在样条曲线控制点序列的末尾加入点，并设置点的坐标
	bool RemoveControlPoint(int nIndex);																	// 删除第nIndex个控制点和权重
	void RemoveControlPoints();																				// 删除所有控制点和权重


	bool SetFitPoints(const std::vector<sc2Vector>& vPoints,const std::vector<double>& vWeight);			// 重新设置拟合点和权重
	bool SetFitPoint(int nIndex, double dX,double dY,double dWeight = 1.0);									// 改变拟合点坐标和权重
	bool GetFitPoint(int nIndex, double& dX,double& dY) const;												// 返回拟合点坐标
	int  GetFitPointNum() const;																			// 获取拟合点的个数

	bool InsertFitPoint(int nIndex, double dX, double dY,double dWeight = 1.0);								// 在第nIndex处插入控制点（dX，dY）
	bool AddFitPoint(double dX, double dY,double dWeight = 1.0);											// 在样条曲线控制点序列的末尾加入点，并设置点的坐标
	
	bool RemoveFitPoint(int nIndex);								// 删除第nIndex个拟合点
	void RemoveFitPoints();											// 删除所有拟合点 

	void SetSpline(const scSplineCurve& spline);					// 设置样条曲线
	scSplineCurve GetSpline() const;								// 获取样条曲线

	bool IsSuccessed();												// 样条曲线是否生成成功

	void SetDistance(double dDistance);                             // 设置绘制样条曲线上点的间隔距离
	//double GetDistacne()const;											// 获取绘制样条曲线上点的间隔距离

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

	// helper for Implementation
protected:	
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;

	void InitTipText();
	void LoadCursor();
	void HitTestHandles(CPoint point,TrackerHit& hitResult,int& nPos) const;

protected:
	scSplineCurve*	m_pSpline;
	scSplineCurve*   m_pSplineDrag;
	TrackerHit  m_hitResult;
	CPoint  m_ptCur;
	int	m_nHitPos;
	double m_dDistance;					// 绘制样条曲线的点间隔距离
	static HCURSOR g_hCursor[3];		// handles to the cursors

	DWORD			m_DOF;				// 自由度
	BOOL			m_bEnableFill;		// 是否可以填充
	COLORREF 		m_FillColor;		// 填充颜色

};

#pragma pack(pop)