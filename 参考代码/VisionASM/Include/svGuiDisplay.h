// svGuiDisplay.h

// ---------------------------------------------------------------------
//                           显示控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiDef.h"
#include "svGuiObject.h"
#include "svGuiInteractiveContainer.h"
#include "svGuiStaticContainer.h"
#include "svImage.h"
#include "vpImage.h"
#include "svJpeg.h"

#pragma pack(push,8)

/////////////////////////////////////////////////////////////////////////////
// scGuiDisplay window


class scGuiDIBBase;
class SVGUI_API scGuiDisplay : public CWnd, public scGuiObject
{
// Construction
public:
	scGuiDisplay();
	virtual ~scGuiDisplay();

	// 显示图像时控件变换状态枚举
	enum TransformMode {
		tmIdle = -1,			// 空闲
		tmPanWait,				// 等待拖动平移控件显示的内容
		tmPanMove,				// 拖动平移控件显示的内容
		tmZoomIn,				// 放大控件显示的内容
		tmZoomOut				// 缩小控件显示的内容
	};

	// 图像绘制方式
	enum PaintMode
	{
		pmGDI     = 0,          // 使用GDI绘制
		pmGDIPlus = 1           // 使用GDIPlus绘制
	};

	// 8位图像调色板映射模式
	enum PaletteMapMode
	{
		pmGray = 0,             // 灰度显示
		pmJet,                  // Jet显示
		pmRGB332,               // RGB332显示
		pmThal,                 // Thal显示
		pmCustom,               // 自定义显示
	};

	// 自定义消息结构
	typedef struct tagWndMsg{
		CWnd* pWnd;				// 响应自定义消息的窗体
		UINT  nFlags;			// 响应的消息类型
		tagWndMsg()
		{
			pWnd = NULL;
			nFlags = 0;
		}
	} WndMsg;

public:
	static BOOL RegisterWndClass(HINSTANCE hInstance);					// 注册显示控件类


	// 在给定位置上创建显示窗体
	// nPosID：父窗体上确定待创建显示控件位置的控件的ID；
	// lpRect：待创建显示控件在父窗体上的位置；
	// pParentWnd：父窗体指针；
	virtual BOOL	Create(UINT nPosID, CWnd* pParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);		
	virtual BOOL	Create(LPRECT lpRect, CWnd* pParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);
	virtual BOOL	Create(HWND handlePosWnd, HWND handleParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);		

	void	RegisterWndMsg(CWnd* pWnd, UINT nFlags);					// 注册响应Display自定义消息的窗口，并指定其响应消息的类型
	void	RegisterWndMsg(HWND handleWnd, UINT nFlags);
	UINT	GetID()	const;												// 获取当前Display控件的标号

	BOOL	SetImage(const scImage8& image, BOOL bCopy = FALSE);		// 设置待显示的8位图像，如果bCopy=TRUE则拷贝图像，否则只记录输入图像的地址
	BOOL	SetImage(const scImage24& image, BOOL bCopy = FALSE);		// 设置待显示的24位图像，如果bCopy=TRUE则拷贝图像，否则只记录输入图像的地址
	BOOL	SetImage(const scImage* pImage, BOOL bCopy = FALSE);		// 设置待显示的8位或24位图像，如果bCopy=TRUE则拷贝图像，否则只记录输入图像的地址
	virtual BOOL SetImage(const cpImage& image, BOOL bCopy = FALSE);		// 设置待显示的8位或24位图像，如果bCopy=TRUE则拷贝图像，否则只记录输入图像的地址
	virtual BOOL    SaveImage(LPCTSTR lpszPathName);							// 保存显示控件中的图像
	virtual BOOL    SaveImageToPng(LPCTSTR lpszPathName);						// 保存显示控件中的图像
	virtual BOOL    SaveImageToJpeg(LPCTSTR lpszPathName, int nQuality = 100);	// 保存显示控件中的图像
	virtual BOOL    SaveImageToVdb(LPCTSTR lpszPathName);						// 保存显示控件中的图像
	virtual BOOL    SaveScreen(LPCTSTR lpszPathName);							// 保存显示控件中的图像（包括Graphic）
	virtual BOOL    SaveScreenToPng(LPCTSTR lpszPathName);						// 保存显示控件中的图像（包括Graphic）
	virtual BOOL    SaveScreenToJpeg(LPCTSTR lpszPathName, int nQuality = 100);	// 保存显示控件中的图像（包括Graphic）
	virtual BOOL    SaveScreenToVdb(LPCTSTR lpszPathName);						// 保存显示控件中的图像（包括Graphic）
	BOOL	IsCopyImage() const;										// 获取是否对输入的图像进行拷贝
	BOOL	HasImage() const;											// 获取显示控件中是否有用于显示的图像
	SVPixelFormat	GetImagePixelFormat() const;						// 获取图像的象素格式
	const scImage* GetImage() const;									// 获取图像对象指针
	const cpImage* GetImageCP() const;									// 获取图像对象指针
	PBYTE	GetImageAddr() const;										// 获取图像数据指针
	LONG	GetImageWidth() const;										// 获取图像的宽
	LONG	GetImageHeight() const;										// 获取图像的高
	BOOL	GetImagePixelValue(LONG lImageX, LONG lImageY,				// 获取图像(lImageX,lImageY)位置的像素值
							   BYTE& R, BYTE& G, BYTE& B) const;

	BOOL	IsImageVisible() const;										// 判断显示控件是否可以显示图像
	void	ShowImage(BOOL bVisible = TRUE);							// 根据bShow控制显示控件显示或隐藏所包含的图像

	virtual void    RemoveImage(BOOL bReset = TRUE);							// 从显示控件中删除图像,会影响图形的显示位置
	void	RemoveGraphics();											// 从显示控件中删除图形（静态图形和交互图形）
	void	HideAllGraphics();											// 从显示控件中隐藏图形（静态图形和交互图形）
	virtual void    ClearScreen(BOOL bReset = TRUE);							// 清除屏幕内容(删除图像,清空静态和交互图形容器内容)

	void	EnableDrawGraphicWithNoImage(BOOL bEnable = TRUE);			// 设置无图像数据时是否绘制动态/静态图形

	//// 设置、获取交互图形的容器
	BOOL	SetInteractiveContainer(scGuiInteractiveContainer* pIGContainer);
	scGuiInteractiveContainer* GetInteractiveContainer() const;

	//// 设置、获取静态图形的容器
	BOOL	SetStaticContainer(scGuiStaticContainer* pSGContainer);
	scGuiStaticContainer*	GetStaticContainer() const;

	
	BOOL SetConstContainer(scGuiStaticContainer* pCGContainer);
	scGuiStaticContainer*	GetConstContainer() const;

	scGuiStaticContainer*	GetSelectedGraphics() const;				// 获取被选中的交互图形

	int		GetSelectedGraphicCount() const;							// 获取被选中交互图形的数量
	CString GetSelectedGraphicKey(int nIndex = 0) const;				// 获取第nIndex个被选中交互图形的Key
	scGuiGraphic* GetSelectedGraphic(int nIndex = 0) const;				// 获取第nIndex个被选中交互图形对象指针			
	
	// 将基于控件窗口的坐标转换为基于图像的坐标
	void	TransformPos(double dX, double dY, double& dImageX, double& dImageY) const;	

	BOOL	SetScaleFactor(double dFactor);								// 设置缩放因子,dFactor必须大于1.0
	double	GetScaleFactor() const;										// 获取缩放因子

	void	Scale(double dScale);										// 缩放在控件窗口上显示的图像
	void	FitImage();													// 以适合控件窗口的比例显示图像
	void	FitGraphic();												// 以适合图形外接矩形的比例显示图像
	void	FitImageGraphic();											// 以适合图像与图形外接矩形的比例显示图像

	double	GetScale() const;											// 获取图像显示时的缩放比例	
	void	GetTranslate(double& dOffsetX, double& dOffsetY) const;		// 获取图像显示时，经缩放后相对于显示窗口的平移

	void	PanXY(int nHorz, int nVert);								// 设置、获取滚动条滑块相对于其基准位置的偏移
	void	GetPanXY(int& nHorz, int& nVert) const;	
	
	void	SetAutoFit(BOOL bAutoFit = TRUE);							// 设置、获取图像是否自动以适合窗口的比例显示
	BOOL	IsAutoFit() const;

	void	SetFitRect(CRect& rect);									// 设置、取消、获取图像Fit到某一个矩形区域
	void	CancelFitRect();
	CRect	GetFitRect() const;
	
	void	SetTransformMode(TransformMode mode);						// 设置、获取图像显示的变换状态
	int		GetTransformMode() const;

	void    SetImagePaintMode(PaintMode mode);                          // 设置、获取图像绘制方式
	int     GetImagePaintMode() const;

	void    SetGraphicPaintMode(PaintMode mode);                        // 设置、获取图形绘制方式
	int     GetGraphicPaintMode() const;
	
	void    SetPaletteMapMode(PaletteMapMode mode);                     // 设置、获取调色板映射方式
	int     GetPaletteMapMode()const;

	BOOL    SetCustomPalette(std::vector<COLORREF>& clr);               // 设置自定义调色板,数量必须>=256
	BOOL    SetCustomPalette(cpImage& clrImage);                        // 设置自定义调色板，图像宽 * 高必须>= 256
	BOOL    SetCustomPalette(const char* clrImgPath);                   // 设置自定义调色版图像路径

	std::vector<COLORREF> GetCustomPalette()const;                      // 获取自定义调色版

	BOOL	IsHorzScrollBarEnabled() const;								// 获取是否显示水平滚动条
	BOOL	IsVertScrollBarEnabled() const;								// 获取是否显示垂直滚动条
	
	void	SetPopMenuEnabled(BOOL bEnable);							// 设置、获取是否允许弹出菜单
	BOOL	IsPopMenuEnabled() const;
	void	SetLanguage(int nLanguage);									// 设置中英文菜单 0：eng；1：chs
	int		GetLanguage();	

	void	SetKeyBoardEnable(BOOL bEnable);							// 设置软键盘默认状态
	BOOL	GetKeyBoardEnable();

	void	SetInteractiveGraphicTipsEnabled(BOOL bEnable);				// 设置、获取是否显示交互图形的TipText
	BOOL	IsInteractiveGraphicTipsEnabled() const;

	void	SetBackgroundColor(COLORREF color);							// 设置、获取控件背景颜色
	COLORREF GetBackgroundColor() const;

	// 设置、获取控件背景参数（背景模式、背景背景颜色、背景前景色）
	void	SetBackgroundParam(DispBGMode bgMode, COLORREF colorBack = RGB(250,250,250), COLORREF colorFore = RGB(200,200,200), CSize szGrid = CSize(10,10));							
	void	GetBackgroundParam(DispBGMode& bgMode, COLORREF& colorBack, COLORREF& colorFore, CSize& szGrid) const;

	
	void	MyInvalidate();												// 刷新


    // 设置发出客户自定义消息nFlags的菜单项的名称及该菜单项的可见性
	void SetCustomMenuVisableAndName(CustomMsg nFlags, CString strMenuName, BOOL bVisible);

	// 获取发出客户自定义消息nFlags的菜单项的名称及该菜单项的可见性
    BOOL GetCustomMenuVisableAndName(CustomMsg nFlags, CString& strMenuName, BOOL& bVisible);
	
	virtual void OnMouseDownEvent(UINT nFlags, CPoint point);

	// 开启/禁用鼠标滚轮缩放功能
	void EnableMouseWheelZoom(BOOL bEnable);
	BOOL IsMouseWheelZoomEnabled();

	// 获取绘图区、客户区
	CRect	GetDisplayImageDrawRect();
	CRect	GetDisplayClientRect();

protected:
	BOOL	UpdateMyDibFromImageData();									// 用图像数据更新MyDib
	
	void	DrawBackground(CDC* pDC, CRect rcDC) const;					// 绘制背景
	void	DrawImage(CDC* pDC, CRect rcDC) const;						// 绘制图像
	void	DrawInteractiveGraphics(CDC* pDC, CRect rcDC) const;		// 绘制交互图形
	void	DrawStaticGraphics(CDC* pDC, CRect rcDC) const;				// 绘制静态图形
	void	DrawConstGraphics(CDC* pDC, CRect rcDC) const;				// 绘制静态const图形

	void	UpdateView(BOOL bFit = FALSE);								// 更新图像图形滚动条
	void	UpdateView(CPoint point);									// 更新图像图形滚动条
	void	UpdateViewMouseWheel(CPoint point);							// 更新图像图形滚动条

	void	UpdateMaxScale(LONG lWidth, LONG lHeight);					// 更新可以实现的最大放大倍数

	void	AdjustViewImage();											// 根据图像的宽高，调整显示图像的区域（缩放比例、平移量）	
	void	AdjustScrollBarPos(CPoint point);							// 根据当前鼠标位置调整滚动条的临时滚动位置
	void	AdjustScrollBarPosMouseWheel(CPoint point);					// 根据当前鼠标位置调整滚动条的临时滚动位置

	void	AdjustScrollBarPos();										// 调整滚动条的临时滚动位置

	void	AdjustScrollBar();											// 根据图像的显示区域调整滚动条
	void	AdjustInteractiveGraphics();								// 根据图像显示区域调整交互图形
	void	AdjustStaticGraphics();										// 根据图像显示区域调整静态图形
	void	AdjustConstGraphics();										// 根据图像显示区域调整静态const图形

	void	SetToolBarEnabled(BOOL bEnable);							// 设置、获取是否允许工具条
	BOOL	IsToolBarEnabled() const;

	std::vector<int>	GetEditableMaskIndex() const;					// 获取所有可编辑且可见掩膜的索引

	void	InitToolTip();												// 初始化ToolTip控件
	void	InitToolBar();												// 初始化ToolBar控件
	void	UpdateToolCmdUI();											// 更新工具条控件状态

	void	LoadDefaultCursor();										// 加载鼠标样式
	
	void    PrintClient();                                              // 截取客户区的图像（包括控件）
	void    WriteBmpToFile(HBITMAP hBitmap, CString strFilePath);       // 将BMP写入指定路径
	void    WriteBmpToPngFile(HBITMAP hBitmap, CString strFilePath);    // 将Png写入指定路径
	void    WriteBmpToJpegFile(HBITMAP hBitmap, CString strFilePath, int nQuality); // 将Jpeg写入指定路径
	void    WriteBmpToVdbFile(HBITMAP hBitmap, CString strFilePath); // 将Vdb写入指定路径

protected:

	CWnd*	m_pParentWnd;				//父窗体

	WndMsg	m_WndMsg;					//自定义消息对象

	CDC		 m_MemDC;					// 内存DC	
	CBitmap* m_pBitmap;					// 当前内存位图
	CBitmap* m_pOldBitmap;				// 原内存位图

	scGuiDIBBase* m_pDib;				// 图像对象

	SVPixelFormat m_pixelFormat;		// 像素格式
	
	const scImage8*  m_pImage8;
	const scImage24* m_pImage24;
	const cpImage*	 m_pImageCP;

	scImage8*	m_pCopyImage8;
	scImage24*	m_pCopyImage24;
	cpImage*	m_pCopyImageCP;
	BOOL		m_bCopyImage;			// 图像拷贝模式
	BOOL		m_bImageVisible;		// 是否显示图像




	PBYTE	m_pShareImage;				// 共享的图像数据指针
	PBYTE	m_pCopyImage;				// 拷贝的图像数据指针
	LONG	m_lImageWidth;				// 图像的宽
	LONG	m_lImageHeight;				// 图像的高
	
	CRect	m_rcClientRect;				// 窗口的客户区域

	CRect	m_rcDIB;					// 整幅图像矩形区域
	CRect	m_rcGraphic;				// 所有图形外接矩形区域
	CRect	m_rcDIBGraphic;				// 图像和所有图形的外接矩形区域

	CRect	m_rcView;					// 在窗口上显示时，图像占据的区域
	CRect	m_rcOldView;				// 在窗口上显示时，图像原来占据的区域
	mutable CRect	m_rcRealView;		// 在窗口上显示时，图像实际占据的区域

	BOOL	m_bFirstCall;				// 是否第一次调用
	BOOL	m_bAutoFit;					// 是否自动以适合窗口的比例显示图像
	int		m_nFitMode;					// 适应模式

	CRect	m_RectFit;					// Fit到Rect区域

	double  m_dScaleFactor;				// 缩放因子
	double	m_dScale;					// 缩放比例
	double  m_dMaxScale;				// 最大的缩放比例
	
	LONG	m_lOffsetX;					// X方向平移量,不包括滚动条的影响
	LONG	m_lOffsetY;					// Y方向平移量,不包括滚动条的影响

	int		m_nHorz;					// 水平滚动条位置
	int		m_nVert;					// 垂直滚动条位置
	double  m_dHorz;					// 水平滚动条位置（中间计算结果）
	double  m_dVert;					// 垂直滚动条位置

	long	m_lHorzMax;					// 水平滚动条最大值
	long	m_lHorzMin;					// 水平滚动条最小值

	long	m_lVertMax;					// 垂直滚动条最大值
	long	m_lVertMin;					// 垂直滚动条最小值

	TransformMode m_tmTransformMode;	// 图像显示变换状态
	COLORREF	  m_clrBackGround;		// 背景背景颜色	
	COLORREF	  m_clrForeGround;		// 背景前景颜色
	DispBGMode    m_bgDispMode;			// 背景模式
	CSize		  m_szGridSize;

	PaintMode     m_pmImagePaintMode;   // 图像绘制方式
	PaintMode     m_pmGraphicPaintMode; // 图形绘制方式

	PaletteMapMode m_pmPaletteMapMode;  // 调色板映射模式
	std::vector<COLORREF> m_clrCustomPalette; // 自定义调色板
	
	BOOL	m_bHorzScrollBarEnabled;	// 是否显示水平滚动条
	BOOL	m_bVertScrollBarEnabled;	// 是否显示垂直滚动条

	BOOL	m_bPopMenuEnabled;			// 是否允许弹出菜单
	BOOL	m_bOnMenu;					// 是否正在显示弹出对话框
	int		m_nLanguage;				// 语言:0-eng;1-chs

	BOOL	m_bEnableKeyBoard;			// 软键盘默认状态

	BOOL	m_bClientEdgeStyleEnabled;  // 是否具有3D外观

	CToolTipCtrl m_ToolTip;				// Tool tip工具
	BOOL		 m_bIGTipEnabled;		// 是否显示交互图形的tiptext

	BOOL	m_bEnableDrawGraphicWithNoImage;

	scGuiInteractiveContainer*	m_pIGContainer;	 // 交互图形容器对象
	scGuiStaticContainer*		m_pSGContainer;	 // 静态图形容器对象
	scGuiStaticContainer*		m_pSelContainer; // 被选中交互图形容器对象

	scGuiStaticContainer*		m_pCGContainer;	// Const GUI Container, 容器中的控件相对于客户区固定，
												// 位置不随图像、交互操作改变

	CPoint	m_ptCurPoint;

	UINT		 m_nID;					// Display控件的标号

	static UINT	   g_nCount;			// Display控件数量
	static HCURSOR g_hCursor[5];		// 鼠标样式句柄


	BOOL	m_bMultiSelected;
	BOOL	m_bDragging;
	BOOL	m_bDblClicked;

	HBITMAP m_hBitmapPrtClient;

	CToolBar*			m_pControlToolBar;		// 控制工具条
	BOOL				m_bToolBarEnabled;		// 控制工具条是否有效
	UINT				m_nDisplayFlag;			// 显示控制标志：放大、缩小等

	BOOL	m_bMouseWheelZoomEnable;

	void	FitImage2();													// 以适合控件窗口的比例显示图像

	scGuiGraphic*		m_pGuiEditMask;			// 正在编辑掩膜指针

	HINSTANCE			m_hSaveInstance;

// Operations
public:

    CString m_strMenuName[3];
	BOOL    m_bMenuVisible[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(scGuiDisplay)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation

// Generated message map functions
protected:
	//{{AFX_MSG(scGuiDisplay)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnPointer();
	afx_msg void OnPan();
	afx_msg void OnAutoFit();
	afx_msg void OnFitImage();
	afx_msg void OnFitGraphic();
	afx_msg void OnFitImageGraphic();
	afx_msg void OnZoomNormal();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCustom1();
	afx_msg void OnCustom2();
	afx_msg void OnCustom3();
	afx_msg void OnDisplayPointer();
	afx_msg void OnUpdateDisplayPointer(CCmdUI* pCmdUI);
	afx_msg void OnDisplayPan();
	afx_msg void OnUpdateDisplayPan(CCmdUI* pCmdUI);
	afx_msg void OnDisplayZoomIn();
	afx_msg void OnUpdateDisplayZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnDisplayZoomOut();
	afx_msg void OnUpdateDisplayZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnDisplayFit();
	afx_msg void OnUpdateDisplayFit(CCmdUI* pCmdUI);
	afx_msg void OnDisplayFileSaveImage();
	afx_msg void OnDisplayFileSaveScreen();
	afx_msg void OnUpdateDisplayFileSaveScreen(CCmdUI* pCmdUI);
	afx_msg void OnDisplayZoomNormal();
	afx_msg void OnUpdateDisplayZoomNormal(CCmdUI* pCmdUI);
	afx_msg void OnGraphicMaskDelete();
	afx_msg void OnGraphicMaskClear();
	afx_msg void OnGraphicParamEdit();
	afx_msg void OnExitMaskEdit();
	afx_msg LRESULT OnExitEditMask(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSaveImage();
	afx_msg void OnSaveScreen();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#pragma pack(pop)
