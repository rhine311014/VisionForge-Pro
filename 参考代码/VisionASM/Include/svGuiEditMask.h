#pragma once

#include "svGuiGraphic.h"
#include "vpImage.h"

#include "svGuiRect.h"
#include "svGuiAffineRect.h"
#include "svGuiEllipse.h"

#define MAX_UNDO_STEP 8

#pragma pack(push,8)

class CDialogDrawToolBar;

class SVGUI_API scGuiEditMask :	public scGuiGraphic
{
public:
	scGuiEditMask(void);
	virtual ~scGuiEditMask(void);

	virtual GraphicType GetType() const {return eGuiEditMask;};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,				// 未击中
		hitDIBRect = 0,					// 击中掩模图像区域
		hitAffineRect = 1,				// 击中仿射矩形
		hitEllipse	= 2,				// 击中椭圆
	};

	// 掩膜类型
	enum MaskType
	{
		maskPositive = 0,				// 正掩膜
		maskNegtive = 1					// 负掩膜
	};

	// 掩膜绘制模式
	enum MaskEditMode
	{
		mePositive = 0,                // 只能绘制正掩膜
		meNegtive,                     // 只能绘制负掩膜
		meBoth                         // 正负掩膜均能绘制
	};

	// 绘制类型
	enum DrawType
	{
		drawBrush = 0,
		drawErase = 1,
		drawAffineRect = 2,
		drawEllipse = 3
	};

	// 工具栏类型
	enum ToolBarStyle
	{
		ToolBarHorizontal = 0,
		ToolBarVertical = 1
	};

	// 设置掩模图像宽高，根据宽高创建掩膜图像，bClear:是否清空掩膜图像
	BOOL	SetMaskImageSize(long lWidth, long lHeight);

	// 设置掩膜填充图像，nOffsetX,nOffsetY为相对于掩膜图像原点的偏移，bInverse：图像是否反色，bClear：是否清空掩膜图像
	BOOL	SetImage(const cpImage& imageGray, int nOffsetX = 0, int nOffsetY = 0, BOOL bInverse = FALSE, BOOL bClear = TRUE);	// 加载掩模图像
	BOOL	SetImage(LPCTSTR lpszPathName,int nOffsetX = 0, int nOffsetY = 0, BOOL bInverse = FALSE, BOOL bClear = TRUE);	// 加载掩模图像
	
	void	EnableEditMask(BOOL bEnable);								// 设置是否编辑掩膜
	BOOL	IsEditMaskEnable() const;									// 获取掩膜是否编辑

	cpImage GetMaskImage() const;										// 获取掩模图像
	BOOL	SaveMaskImage(LPCTSTR lpszPathName);						// 保存掩膜图像

	long	GetMaskImageWidth() const;									// 获取掩模图像的宽
	long	GetMaskImageHeight() const;									// 获取掩模图像的高 

	void	SetMaskImageOffset(int nOffsetX, int nOffsetY);					// 设置掩模图像相对于显示控件中当前背景图像的位置(逻辑坐标) 
	void	GetMaskImageOffset(int& nOffsetX, int& nOffsetY) const;			// 获取掩模图像相对于显示控件中当前背景图像的位置(逻辑坐标) 

	void	SetMaskType(MaskType type);									// 设置掩膜图像类型
	int		GetMaskType();												// 获取掩膜图像类型

	void    SetMaskEditMode(MaskEditMode mode);                         // 设置掩膜绘制模式
	int     GetMaskEditMode()const;                                     // 获取掩膜绘制模式

public:
	// 掩膜编辑工具栏
	void	SetToolBarStyle(ToolBarStyle type);							// 设置工具栏风格
	int		GetToolBarStyle() const;									// 获取工具栏风格

	void	SetToolBarLanguage(int nLanguage);							// 设置工具栏语言 -1:随Display设置 0:英文 1:中文
	int		GetToolBarLanguage() const;									// 获取工具栏语言

	void	SetToolBarBkgColor(COLORREF rgb);							// 设置工具栏对话框背景色
	COLORREF	GetToolBarBkgColor() const;								// 获取工具栏对话框背景色

	void	SetToolBarCustomTitle(CString strTitle);					// 设置工具栏标题栏自定义标题
	CString	GetToolBarCustomTitle() const;								// 获取工具栏标题栏自定义标题

	void	SetBrushLineWidth(int nWidth);								// 设置画刷线宽
	int		GetBrushLineWidth() const;									// 获取画刷线宽

	void	SetBrushLineWidthRange(int nMin, int nMax);					// 设置画刷线宽范围
	void	GetBrushLineWidthRange(int& nMin, int& nMax) const;			// 获取画刷线宽范围

	void	SetMaskTransparency(int nTransparency);						// 设置掩膜透明度:0~100
	int		GetMaskTransparency() const;								// 获取掩膜透明度

	void	SetMaskTransparencyRange(int nMinPercent, int nMaxPercent);	// 设置掩膜透明度范围：0~100
	void	GetMaskTransparencyRange(int& nMinPercent, int& nMaxPercent) const;	// 获取掩膜透明度范围

	void	SetDrawType(DrawType type);									// 设置当前绘制类型
	int		GetDrawType() const;										// 获取当前绘制类型

	void	SetBoundVisible(BOOL bVisible);								// 设置掩膜图像边界是否可见（非编辑状态）
	BOOL	IsBoundVisible() const;										// 获取掩膜图像边界是否可见（非编辑状态）

	CDialogDrawToolBar* GetDialogDrawToolBar() const;		

	virtual void	ClearContext();
	virtual void	SetVisible(BOOL bVisible = TRUE);
	virtual BOOL	BoundingRect(CRect& rect) const;					// 获取掩模图像的外接矩形

	BOOL	BackupImage();												// 备份掩模图像
	int		UndoImage();												// 回退掩模图像,返回可回退的步数
	int		RedoImage();												// 撤销回退掩膜图像，返回可撤销的步数

	void	FillImage(BOOL bClear);						// 填充/清空掩模图像
	void	FillImageAffineRect(BOOL bClear);			// 填充/清空仿射矩形区域
	void	FillImageEllipse(BOOL bClear);				// 填充/清空椭圆区域						

	void	Invalidate();

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
	void	InitImage();
	void	FreeImage();
	void	FillImage(CPoint pt);

	void	LoadCursor();
	void	InitTipText();
protected:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawEdit(CDC* pDC, LPRECT lpRect = NULL) const;
	void DrawBrushRect(CDC* pDC, LPRECT lpRect = NULL) const;
private:
	Gdiplus::Bitmap*		m_pImage;
	Gdiplus::Bitmap*		m_vpBakImages[MAX_UNDO_STEP];
	Gdiplus::ColorPalette*	m_pPalette;

	int				m_nOffsetX;		// 掩膜X偏移
	int				m_nOffsetY;		// 掩膜Y偏移

	BOOL			m_bEditMask;		// 是否编辑掩膜
	int				m_nMaskType;		// 掩膜类型
	int				m_nToolBarStyle;	// 工具栏类型
	int				m_nToolBarLanguage;	// 工具栏语言

	int				m_nDrawType;		// 绘制类型：0：画刷，1：橡皮，2：仿射矩形，3：椭圆
	int				m_nBrushLineWidth;	// 绘制画刷和橡皮的大小
	int				m_nTransparency;	// 掩膜透明度

	int				m_nUndoStep;		// 回退步数
	int				m_nRedoStep;		// 撤销回退步数

	BOOL			m_bBoundVisible;	// 是否显示图像边界（非编辑状态）

	MaskEditMode    m_meMaskEditMode;

	scGuiAffineRect		m_guiAffineRect;
	scGuiEllipse		m_guiEllipse;
	CDialogDrawToolBar*	m_pDlgToolBar;	// 工具栏对话框

	TrackerHit		m_hitResult;
	static HCURSOR	g_hCursor;
};

#pragma pack(pop)
