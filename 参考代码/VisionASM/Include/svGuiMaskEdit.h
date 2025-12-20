// svGuiMaskEdit.h

// ---------------------------------------------------------------------
//                         掩模图像编辑窗口
// ---------------------------------------------------------------------

#pragma once

/////////////////////////////////////////////////////////////////////////////
// svGuiMaskEdit window

#include "svGuiDisplay.h"
#include "svGuiEllipse.h"
#include "svGuiAffineRect.h"
#include "svGuiRect.h"
#include "svGuiMask.h"
#include <commctrl.h>

#pragma pack(push,8)

class CCustomToolBar;
class CCustomDialogBar;
class CCustomStatusBar;

class SVGUI_API scGuiMaskEdit : public CWnd
{
	// Construction
public:
	scGuiMaskEdit();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(scGuiMaskEdit)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~scGuiMaskEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(scGuiMaskEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMaskFileOpen();
	afx_msg void OnMaskFileSave();
	afx_msg void OnMaskFileSaveAs();
	afx_msg void OnMaskCut();
	afx_msg void OnUpdateMaskCut(CCmdUI* pCmdUI);
	afx_msg void OnMaskCopy();
	afx_msg void OnUpdateMaskCopy(CCmdUI* pCmdUI);
	afx_msg void OnMaskPaste();
	afx_msg void OnUpdateMaskPaste(CCmdUI* pCmdUI);	
	afx_msg void OnMaskUndo();
	afx_msg void OnUpdateMaskUndo(CCmdUI* pCmdUI);
	afx_msg void OnMaskRedo();
	afx_msg void OnUpdateMaskRedo(CCmdUI* pCmdUI);
	afx_msg void OnMaskErase();
	afx_msg void OnUpdateMaskErase(CCmdUI* pCmdUI);	
	afx_msg void OnMaskTransMask();
	afx_msg void OnMaskTransShape();
	afx_msg void OnUpdateMaskTransShape(CCmdUI* pCmdUI);
	afx_msg void OnMaskPointer();
	afx_msg void OnUpdateMaskPointer(CCmdUI* pCmdUI);
	afx_msg void OnMaskPan();
	afx_msg void OnUpdateMaskPan(CCmdUI* pCmdUI);
	afx_msg void OnMaskZoomIn();
	afx_msg void OnUpdateMaskZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnMaskZoomOut();
	afx_msg void OnUpdateMaskZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnMaskFit();
	afx_msg void OnUpdateMaskFit(CCmdUI* pCmdUI);
	afx_msg void OnMaskZoomNormal();
	afx_msg void OnUpdateMaskZoomNormal(CCmdUI* pCmdUI);
	afx_msg void OnMaskSelectRegion();
	afx_msg void OnUpdateMaskSelectRegion(CCmdUI* pCmdUI);
	afx_msg void OnMaskPaint();
	afx_msg void OnUpdateMaskPaint(CCmdUI* pCmdUI);
	afx_msg void OnMaskPen();
	afx_msg void OnUpdateMaskPen(CCmdUI* pCmdUI);
	afx_msg void OnMaskBrush();
	afx_msg void OnUpdateMaskBrush(CCmdUI* pCmdUI);
	afx_msg void OnMaskFillAffineRect();
	afx_msg void OnUpdateMaskFillAffineRect(CCmdUI* pCmdUI);
	afx_msg void OnMaskFillEllipse();
	afx_msg void OnUpdateMaskFillEllipse(CCmdUI* pCmdUI);
	afx_msg void OnStaticBrushSize4();
	afx_msg void OnStaticBrushSize8();
	afx_msg void OnStaticBrushSize16();
	afx_msg void OnStaticBrushSize32();
	afx_msg void OnStaticBrushSize64();
	afx_msg void OnMaskColor0();
	afx_msg void OnUpdateMaskColor0(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor1();
	afx_msg void OnUpdateMaskColor1(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor2();
	afx_msg void OnUpdateMaskColor2(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor3();
	afx_msg void OnUpdateMaskColor3(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor4();
	afx_msg void OnUpdateMaskColor4(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor5();
	afx_msg void OnUpdateMaskColor5(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor6();
	afx_msg void OnUpdateMaskColor6(CCmdUI* pCmdUI);
	afx_msg void OnMaskColor7();
	afx_msg void OnUpdateMaskColor7(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSVDisplayLButtonDown(WPARAM wParam, LPARAM lParam);	//x64//HRESULT
	afx_msg LRESULT OnSVDisplayMouseDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSVDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSVDisplayMouseUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSVDisplayTMChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSVDisplayDragStop(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
public:
	static BOOL RegisterWndClass(HINSTANCE hInstance);							// 注册掩模编辑控件类


	BOOL    Create(LPRECT lpRect, CWnd* pParentWnd);							// 创建掩模编辑工具
	BOOL	Create(HWND handlePosWnd, HWND handleParentWnd);		


	//	设置掩模图像中像素值所对应的颜色、透明度和提示文字，(注：掩模值的数量 nNum >0 且 nNum <= 8)
	BOOL	 SetMaskCustomColors(PBYTE pMaskVals, int nNum, COLORREF* pMaskColors, scGuiMask::Transparency* pTrs, CString* pTipText);
	int		 GetMaskNum() const;												//  获取掩模图像中掩模值数量
	BYTE	 GetMaskVal(int nIndex) const;										//  获取第nIndex个掩模值
	COLORREF GetMaskColor(BYTE nMaskVal)  const;								//  获取掩模图像中掩模值所对应的颜色	
	int		 GetMaskTransparency(BYTE nMaskVal)  const;							//  获取掩模图像中掩模值所对应颜色的透明度
	LPCTSTR  GetMaskTipText(BYTE nMaskVal)  const;								//  获取掩模图像中掩模值所对应颜色（调色板工具条按钮）的提示文字

	BOOL	SetRefImage(const scImageGray& image);								// 设置掩模编辑工具的参考图像				
	BOOL	SetRefImage(const scImageRGB& image);								// 设置掩模编辑工具的参考图像
	BOOL	SetRefImage(const scImage* image);									// 设置掩模编辑工具的参考图像
	BOOL	SetRefImage(const cpImage& image);									// 设置掩模编辑工具的参考图像

	BOOL	GetRefImage(scImageGray& image) const;								// 获取掩模编辑工具参考图像
	BOOL	GetRefImage(scImageRGB& image) const;								// 获取掩模编辑工具参考图像
	BOOL	GetRefImage(scImage* image) const;									// 获取掩模编辑工具参考图像
	BOOL	GetRefImage(cpImage& image) const;									// 获取掩模编辑工具参考图像

	PBYTE	GetRefImageAddr()  const;											// 获取掩模编辑工具参考图像的数据指针
	LONG	GetRefImageWidth()  const;											// 获取掩模编辑工具参考图像的宽
	LONG	GetRefImageHeight()  const;											// 获取掩模编辑工具参考图像的高
	SVPixelFormat	GetRefImagePixelFormat() const;								// 获取掩模编辑工具参考图像的象素格式

	BOOL	SetMaskImage(const scImageGray&  image);							// 设置掩模编辑工具的掩模图像
	BOOL	GetMaskImage(scImageGray&  image) const;							// 获取掩模编辑工具中的掩模图像
	BOOL	SetMaskImage(const cpImage& image);									// 设置掩模编辑工具的掩模图像
	BOOL	GetMaskImage(cpImage& image) const;									// 获取掩模编辑工具中的掩模图像
	PBYTE	GetMaskImageAddr() const;											// 获取掩模编辑工具掩模图像的数据指针
	LONG	GetMaskImageWidth() const;											// 获取掩模编辑工具掩模图像的宽
	LONG	GetMaskImageHeight() const;											// 获取掩模编辑工具掩模图像的高
	SVPixelFormat	GetMaskImagePixelFormat() const;							// 获取掩模编辑工具掩模图像的象素格式

	void	SetMaskImageOffset(LONG lOffsetX, LONG lOffsetY);					// 设置掩模图像相对于参考图像的偏移量
	void	GetMaskImageOffset(LONG& lOffsetX, LONG& lOffsetY) const;			// 获取掩模图像相对于参考图像的偏移量

	void	SetMaskPosWindowEnabled(BOOL bEnable);								// 设置是否可以在界面上修改掩模图像的偏移量和尺寸
	BOOL	IsMaskPosWindowEnabled() const;										// 获取是否可以在界面上修改掩模图像的偏移量和尺寸

	// 设置掩模绘制方式(默认gdi)
	void	SetMaskPaintMode(scGuiDisplay::PaintMode mode);						// 设置Mask绘制方式
	int		GetMaskPaintMode() const;											// 获取Mask绘制模式

	// 设置对话框背景颜色
	void	SetBkgdColor(COLORREF color);										// 设置背景框颜色
	COLORREF GetBkgdColor() const;												// 获取背景框颜色

	// 设置、获取Display控件背景参数（背景模式、背景背景颜色、背景前景色）
	void	SetDisplayBackgroundParam(DispBGMode bgMode, COLORREF colorBack = RGB(250,250,250), COLORREF colorFore = RGB(200,200,200), CSize szGrid = CSize(10,10));							
	void	GetDisplayBackgroundParam(DispBGMode& bgMode, COLORREF& colorBack, COLORREF& colorFore, CSize& szGrid) const;

private:
	void UpdateToolCmdUI();								// 更新工具条控件状态
	BOOL InitDlgItem();									// 初始化窗体上的控件
	BOOL InitPaletteToolBar();							// 初始化调色板控件
	void SetStaticBrushBmpVisible(BOOL bVisible);		// 设置与画刷相关控件（图片）是否可见
	void SetStaticBrushSizeVisible(BOOL bVisible);		// 设置与画刷相关控件（文字）是否可见

	void SetMaskGraphic();								// 设置掩模图形工具

	void CopyMaskImage();								// 拷贝选中区域的掩模图像
	void CutMaskImage();								// 剪切选中区域的掩模图像
	void PasteMaskImage();								// 粘贴选中区域的掩模图像

	void FillMaskImage();								// 用当前选中颜色填充填充掩模图像整个矩形区域
	void FillMaskImage(scGuiAffineRect* graRegion);	// 用当前选中颜色填充填充掩模图像的仿射矩形区域
	void FillMaskImage(scGuiEllipse* geRegion);		// 用当前选中颜色填充填充掩模图像的椭圆区域

	BOOL ResizeMaskImage(LONG lWidth, LONG lHeight);	// 改变掩模图像尺寸，并将原掩模图像内容拷贝到新的掩模图像中

	BOOL SetMaskImage(LONG lWidth, LONG lHeight);		// 用给定尺寸构造掩模图像
	void InvalidateDisplay(BOOL bErase = TRUE);			// 刷新显示控件

private:
	CCustomToolBar*			m_pControlToolBar;				// 控制工具条
	CCustomToolBar*			m_pDrawToolBar;					// 绘制工具条
	CCustomToolBar*			m_pPaletteToolBar;				// 调色工具条
	CCustomDialogBar*		m_pBrushDialogBar;				// 画笔宽度对话框工具条
	CCustomStatusBar*		m_pDisplayStatusBar;			// 显示状态栏

	UINT				m_nDisplayFlag;					// 显示控制标志：放大、缩小等
	UINT				m_nEditFlag;					// 编辑标志
	UINT				m_nDrawFlag;					// 绘制标志
	UINT				m_nPaletteFlag;					// 调色板标志
	BOOL				m_bCutCopyFlag;					// 是否可以拷贝、复制的标志
	BOOL				m_bPasteFlag;					// 是否可以粘贴的标志
	BOOL				m_bUndoFlag;					// Undo标志
	BOOL				m_bRedoFlag;					// Redo标志

	BOOL				m_bMaskPosWindowEnable;			// 是否可以在界面上修改掩模图像的偏移量和尺寸
	BOOL				m_bDisplayStatusBarEnable;

	scGuiDisplay		m_Display;						// 显示控件
	scGuiInteractiveContainer* m_pIGContainer;			// 交互控件容器

	scGuiRect			m_grSelectRegion;				// 用于剪切/拷贝的矩形区域
	scGuiAffineRect		m_graFillRegion;				// 用于填充的仿射矩形区域
	scGuiEllipse		m_geFillRegion;					// 用于填充的椭圆区域
	scGuiMask			m_gmMask;						// 掩模图形工具
	scGuiMask			m_gmRegionMask;					// 用于剪切/拷贝的掩模图形工具

	scGuiRect			m_grBrushRegion;				// 画刷矩形区域


	// 掩模调色板数据
	typedef struct tagMaskPalette{
		UINT		nID;			// ID						
		BYTE		nMaskVal;		// 掩模图像象素值
		RGBTRIPLE	rgbColor;		// 掩模图像象素对应的颜色
		CString		strTipText;		// 调色板上颜色的提示文字
		int			nTr;			// 透明度
	} MASKPALETTE, *PMASKPALETTE;
	enum { MaxNum = 8 };

	BYTE				m_nMaskVal;						// 掩模值
	int					m_nMaskNum;						// 掩模值数量
	UINT				m_nMaskCurColorIndex;			// 当前调色板掩模颜色
	MASKPALETTE			m_mpMaskPal[MaxNum];			// 掩模调色板数据
	COLORREF			m_crPaletteMask;				// 调色板掩模颜色
	CImageList*			m_pImageList;					// 调色板颜色图像链表
	int					m_nBrushSize;					// 画刷尺寸

	COLORREF			m_cBackground;					// 对话框背景颜色

	CString				m_strMaskFile;					// 掩模文件名
};

#pragma pack(pop)
