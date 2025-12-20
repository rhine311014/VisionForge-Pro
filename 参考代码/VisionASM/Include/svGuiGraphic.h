// svGuiGraphic.h

// ---------------------------------------------------------------------
//                           图形交互控件基类
// ---------------------------------------------------------------------

#pragma once 

#include "svGuiDef.h"
#include "svGuiObject.h"
#include "sv2XformLinear.h"

#pragma pack(push,8)

// GUI图形的类型
enum GraphicType{
	eGuiUnknown = 0,  
	eGuiAffineRect = 1,  
	eGuiMaskAffineRect,  
	eGuiAnnulus ,
	eGuiAnnulusSection,  
	eGuiCaliper,  
	eGuiCaliperResult,
	eGuiCaliperScore, 
	eGuiCircle,
	eGuiCircularArc,  
	eGuiContour, 
	eGuiCoordCross,
	eGuiCross,
	eGuiEllipse,  
	eGuiEllipseAnnulus ,
	eGuiFeaturelet, 
	eGuiFindCircle,  
	eGuiFindEllipse,
	eGuiFindGenRect,  
	eGuiFindLine,  
	eGuiFindObround,  
	eGuiGenRect,  
	eGuiHistogram, 
	eGuiLine,  
	eGuiLineSeg,   
	eGuiMask,  
	eGuiMaskRect,
	eGuiObround,  
	eGuiPolyline, 
	eGuiRect,
	eGuiText,  
	eGuiGrayscaleCurve,
	eGuiFrameText,
	eGuiEllipseArc,
	eGuiLineRay,
	eGuiSplineCurve,
	eGuiEditMask,
	eGuiGraphicGroup,
	eGuiCalibration3D,
	eGuiPolylineArc,
	eGuiFindMarkedCircle,
};

class SVGUI_API scGuiGraphic : public scGuiObject
{
public:
	scGuiGraphic();				// 构造函数
	virtual ~scGuiGraphic();	// 析构函数

	virtual GraphicType GetType() const = 0;

	//Line Style Flags
	enum StyleFlags
	{
		solidLine = 0,		
		dashLine = 1,		/* -------  */
		dotLine = 2,		/* .......  */
		dashDotLine = 3,	/* _._._._  */
		dashDotDotLine= 4	/* _.._.._  */
	};

	// tip text structure
	struct CTipTextNode
	{
		BOOL	bEnable;
		BOOL	bDefault;
		CString strText;

		CTipTextNode()
		{
			bEnable  = FALSE;
			bDefault = TRUE;
			strText	 = _T("");
		}
	};

public:

	// 显示所在的窗口
	void	 SetContext(CWnd* pWnd);					// 设置图形绘制的窗体
	CWnd*	 GetContext() const;						// 获取图形绘制的窗体
	virtual	 void ClearContext();						// 清空图形绘制的窗体 

	// 交互属性
	virtual void	 SetVisible(BOOL bVisible = TRUE);	// 设置图形是否可见
	BOOL	 IsVisible() const;							// 获取图形是否可见

	void	 SetInteractive(BOOL bInteractive = TRUE);	// 设置图形是否可交互
	BOOL	 IsInteractive() const;						// 获取图形是否可交互

	void	 SetSelected(BOOL bSelected = TRUE);		// 设置图形是否被选中
	BOOL	 IsSelected() const;						// 获取图形是否被选中

	void	 SetMultiSelectable(BOOL bSelectable = TRUE);// 设置图形是否可以被多选
	BOOL	 IsMultiSelectable() const;					// 获取图形是否可以被多选

	void	 SetMultiSelected(BOOL bSelected = TRUE);	// 设置图形是否被多选了
	BOOL	 IsMultiSelected() const;					// 获取图形是否被多选了

	void	 SetDragging(BOOL bDragging = TRUE);		// 设置图形是否为被拖拽中
	BOOL	 IsDragging() const;						// 获取图形是否为被拖拽中

	void	 SetCanceled(BOOL bCanceled = TRUE);		// 设置是否取消图形的改变
	BOOL	 IsCanceled() const;						// 获取是否取消图形的改变


	void	 SetVicinitySize(int nSize);				// 设置交互距离
	int		 GetVicinitySize() const;					// 获取交互距离

	// 图形绘制属性
	virtual void	 SetLineStyle(StyleFlags style);			// 设置绘制的线型
	virtual void	 SetLineWidth(int nWidth);					// 设置绘制的线宽
	virtual void	 SetLineColor(COLORREF color);				// 设置绘制的线颜色

	int		 GetLineStyle() const;						// 获取绘制的线型
	int      GetLineWidth() const;						// 获取绘制的线型
	COLORREF GetLineColor() const;						// 获取绘制的线型

	virtual void	 SetLineStyle2(StyleFlags style);			// 设置绘制的线型2
	virtual void	 SetLineWidth2(int nWidth);					// 设置绘制的线宽2
	virtual void	 SetLineColor2(COLORREF color);				// 设置绘制的线颜色2

	int		 GetLineStyle2() const;						// 获取绘制的线型2
	int      GetLineWidth2() const;						// 获取绘制的线型2
	COLORREF GetLineColor2() const;						// 获取绘制的线型2

	virtual void	 SetSelectedLineStyle(StyleFlags style);	// 设置选中后绘制的线型
	virtual void	 SetSelectedLineWidth(int nWidth);			// 设置选中后绘制的线宽
	virtual void	 SetSelectedLineColor(COLORREF color);		// 设置选中后绘制的线颜色

	int		 GetSelectedLineStyle() const;				// 获取选中后绘制的线型
	int      GetSelectedLineWidth() const;				// 获取选中后绘制的线宽
	COLORREF GetSelectedLineColor() const;				// 获取选中后绘制的线颜色

	virtual void	 SetDraggingLineStyle(StyleFlags style);	// 设置拖动时绘制的线型
	virtual void	 SetDraggingLineWidth(int nWidth);			// 设置拖动时绘制的线宽
	virtual void	 SetDraggingLineColor(COLORREF color);		// 设置拖动时绘制的线颜色

	int		 GetDraggingLineStyle() const;				// 获取拖动时绘制的线型
	int      GetDraggingLineWidth() const;				// 获取拖动时绘制的线宽
	COLORREF GetDraggingLineColor() const;				// 获取拖动时绘制的线颜色

	void	 SetMaskLineStyle(StyleFlags style);		// 设置掩模绘制的线型
	void	 SetMaskLineWidth(int nWidth);				// 设置掩模绘制的线宽
	void	 SetMaskLineColor(COLORREF color);			// 设置掩模绘制的线颜色

	int		 GetMaskLineStyle() const;					// 获取掩模绘制的线型
	int      GetMaskLineWidth() const;					// 获取掩模绘制的线宽
	COLORREF GetMaskLineColor() const;					// 获取掩模绘制的线颜色

	void	 SetHandleSize(int nSize);					// 设置绘制Handle的尺寸
	void	 SetHandleLineStyle(StyleFlags style);		// 设置绘制Handle的线型
	void	 SetHandleLineWidth(int nWidth);			// 设置绘制Handle的线宽
	void	 SetHandleColor(COLORREF color);			// 设置绘制Handle的线颜色
	void	 SetVertexHandleColor(COLORREF color);		// 设置绘制顶点Handle的颜色

	CSize	 GetHandleSize()		const;				// 获取绘制Handle的尺寸
	int		 GetHandleLineStyle()	const;				// 获取绘制Handle的线型
	int		 GetHandleLineWidth()	const;				// 获取绘制Handle的线宽
	COLORREF GetHandleColor()		const;				// 获取绘制Handle的线颜色
	COLORREF GetVertexHandleColor() const;				// 获取置绘制顶点Handle的颜色


	// 中心十字线
	virtual void	SetCenterCrossVisible(BOOL bVisible);		// 设置中心十字线是否可见
	virtual BOOL	IsCenterCrossVisible() const;				// 获取中心十字线是否可见


	// 标记相关属性
	void	 SetLabel(CString strLable);				// 设置图形的标识
	CString  GetLabel() const;							// 获取图形的标识

	void	 SetLabelVisible(BOOL bVisible);			// 设置图形的标识是否显示
	BOOL	 IsLabelVisible() const;					// 获取图形的标识是否显示

	void	 SetLabelFont(const LOGFONT* lpLogFont);	// 设置图形标识的字体
	void	 GetLabelFont(LOGFONT* pLogFont) const;		// 获取图形标识的字体


	virtual int		GetHandleCount() const;										// 获取Handle的数量

	virtual	void	SetTipTextEnabled(BOOL bEnable);					     	// 设置能否显示所有Handle的提示性文字
	virtual	BOOL	IsTipTextEnabled() const;							        // 获取能否显示所有Handle的提示性文字

	virtual void	SetTipTextDefault(BOOL bDefault);					        // 设置显示所有Handle的提示性文字是否是程序内部默认的
	virtual BOOL	IsTipTextDefault() const;							        // 获取显示所有Handle的提示性文字是否是程序内部默认的

	virtual	void	SetTipText(LPCTSTR lpszTipText);						    // 设置显示所有Handle的提示性文字
	virtual CString GetTipText()	const;									    // 获取显示所有Handle的提示性文字

	virtual void	SetTipTextEnabled(int nHandle, BOOL bEnable);				// 设置能否显示第nHandle个Handle的提示性文字
	virtual BOOL	IsTipTextEnabled(int nHandle) const;						// 获取能否显示第nHandle个Handle的提示性文字

	virtual	void	SetTipTextDefault(int nHandle, BOOL bDefault);				// 设置显示第nHandle个Handle的提示性文字是否是程序内部默认的
	virtual	BOOL	IsTipTextDefault(int nHandle) const;						// 获取显示第nHandle个Handle的提示性文字是否是程序内部默认的

	virtual void	SetTipText(int nHandle, LPCTSTR lpszTipText);				// 设置显示第nHandle个Handle的提示性文字

	virtual CString	GetTipText(int nHandle)  const;								// 获取显示第nHandle个Handle的提示性文字
	virtual CString	GetTipText(CPoint point) const;								// 获取鼠标位置为point时对应Handle的提示性文字


	double	GetScale() const;													// 获取图形的缩放比例
	void	GetOffset(double& dOffsetX, double& dOffsetY) const;				// 获取图形的偏移量

	virtual BOOL BoundingRect(CRect& rect) const;								// 获取图形外接矩形

	// 变换
	virtual BOOL Transform(double dScale, double dOffsetX, double dOffsetY);	// 对图形进行变换，先进行比例变换，然后再进行平移变换

	virtual BOOL	DragStart(CPoint point) ;									// 开始拖拽图形
	virtual void	DragAnimate(CPoint ptStart, CPoint ptStop);					// 拖拽图形
	virtual void	DragStop();													// 拖拽结束，记下改变后的图形
	virtual void	DragCancel();												// 拖拽取消
	virtual int		HitTest(CPoint point) const;								// 击中测试
	
	virtual BOOL	SetCursor(CWnd* pWnd, UINT nHitTest) const = 0;				// 改变鼠标样式
	virtual void	Draw(CDC* pDC, LPRECT lpRect = NULL) = 0;				// 绘制正常的图形
	virtual void	DrawLabel(CDC* pDC, LPRECT lpRect = NULL) = 0;		// 绘制标识

	void	 SetPopMenuEnabled(BOOL bEnabled = TRUE);			// 设置图形是否可弹出右键菜单
	BOOL	 IsPopMenuEnabled() const;							// 获取图形是否可弹出右键菜单

	virtual void OnGraphicMaskDelete(CPoint point) {};
	virtual void OnGraphicMaskClear(CPoint point) {};
	virtual BOOL OnGraphicParamEdit(CPoint point) {return FALSE;};

	void	 SetGraphicParamEditEnabled(BOOL bEnabled = TRUE) {m_bGraphicParamEditEnabled = bEnabled;};	// 设置是否允许修改参数
	BOOL	 IsGraphicParamEditEnabled() const {return m_bGraphicParamEditEnabled;};						// 获取是否允许修改参数


protected:
	void	SetDefaults();														 // 设置图形默认属性					

	// 绘制
	virtual void	DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;			// 绘制正常的图形
	virtual void	DrawSelected(CDC* pDC, LPRECT lpRect = NULL) const;			// 绘制选中时的图形
	virtual void	DrawAnimate(CDC* pDC, LPRECT lpRect = NULL) const;			// 绘制拖动时的图形

	virtual BOOL IsOnSegment(CPoint ptStart, CPoint ptEnd, CPoint point) const;


protected:
	CWnd*	m_pWnd;						// 用于显示图形的控件对象指针

	double	m_dScale;					// 缩放系数
	double	m_dOffsetX;					// X方向偏移量
	double	m_dOffsetY;					// Y方向偏移量

	sc2XformLinear m_xFormLinear;		// 一致缩放平移变换
	
	BOOL		m_bGraphicParamEditEnabled;	// 是否允许修改参数

	BOOL	m_bVisible;					// 是否显示

	StyleFlags	m_nLineStyle;			// 线型
	int			m_nLineWidth;			// 线宽
	COLORREF	m_LineColor;			// 线的颜色

	StyleFlags	m_nLineStyle2;			// 线型2
	int			m_nLineWidth2;			// 线宽2
	COLORREF	m_LineColor2;			// 线的颜色2

	CString		m_strLabel;				// 标记
	LOGFONT*	m_pLabelFont;			// 标记的字体
	BOOL		m_bLabelVisible;		// 是否显示标记

	StyleFlags	m_nSelectedLineStyle;	// 选中后图形的线型
	int			m_nSelectedLineWidth;	// 选中后图形的线宽	
	COLORREF	m_SelectedLineColor;	// 选中后图形的颜色

	StyleFlags	m_nDraggingLineStyle;	// 拖动时图形的线型
	int			m_nDraggingLineWidth;	// 拖动时图形的线宽	
	COLORREF	m_DraggingLineColor;	// 拖动时图形的颜色

	StyleFlags	m_nMaskLineStyle;		// 掩模图形的线型
	int			m_nMaskLineWidth;		// 掩模图形的线宽	
	COLORREF	m_MaskLineColor;		// 掩模图形的颜色
	
	CSize		m_csHandleSize;			// handle尺寸
	StyleFlags	m_nHandleLineStyle;		// handle线型
	int			m_nHandleLineWidth;		// handle线宽
	COLORREF	m_HandleColor;			// handle颜色 	
	COLORREF	m_VertexHandleColor;	// 顶点handle颜色

	int			m_nVicinitySize;		// 交互距离

	CString		m_strTipText;			// 提示性文本

	BOOL		m_bSelected;			// 是否被选中
	BOOL		m_bInteractive;			// 是否可交互
	BOOL		m_bDragging;			// 是否正被拖动
	BOOL		m_bCanceled;			// 是否取消图形改变		

	BOOL		m_bMultiSelectable;		// 是否可以被多选
	BOOL		m_bMultiSelected;		// 是否被多选了

	int			m_nHandleCount;			// Handle的数量

	CTipTextNode	m_UnifiedTipText;	// 所有Hanlde统一的提示性文本
	CTipTextNode*	m_pTipText;			// 各个Hanlde的提示性文本


	BOOL		m_bCenterCrossVisible;

	BOOL		m_bPopMenuEnabled;
};

#pragma pack(pop)
