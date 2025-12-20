// svGuiMask.h

// ---------------------------------------------------------------------
//                         掩模图像交互控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svImage.h"

#pragma pack(push,8)


class SVGUI_API scGuiMask : public scGuiGraphic  
{
public:
	scGuiMask();
	virtual ~scGuiMask();

	virtual GraphicType GetType() const {return eGuiMask;};

	// 图像绘制方式
	enum PaintMode
	{
		pmGDI     = 0,					// 使用GDI绘制
		pmGDIPlus = 1					// 使用GDIPlus绘制
	};

	// 透明度
	enum Transparency
	{
		transparencyNone = 0,			// 不透明
		transparencyHalf = 50,			// 半透明
		transparencyFull = 100			// 全透明
	};

	// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,				// 未击中
		hitRect	   = 0					// 击中掩模图像区域
	};

	BOOL	SetImage(const scImageGray& imageGray);						// 从内存中加载掩模图像
	BOOL	SetImage(long lWidth, long lHeight);						// 创建掩模图像
	BOOL	SetImage(LPCTSTR lpszPathName);								// 从图像文件中加载掩模图像
	BOOL	SaveImage(LPCTSTR lpszPathName);							// 将当前图像存储到图像文件
	scImageGray* GetImage() const;										// 获取掩模图像
	PBYTE	GetImageAddr() const;										// 获取掩模图像裸数据的存储地址（自顶向下，非四字节对齐）
	long	GetImageWidth() const;										// 获取掩模图像的宽
	long	GetImageHeight() const;										// 获取掩模图像的高
	BOOL	UpdateImage();												// 更新显示掩模图像 

	BOOL	BackupImage();												// 备份掩模图像
	BOOL	RestoreImage();												// 恢复掩模图像
	BOOL	IsImageChanged();											// 查询掩模图像是否发生了改变

	void	SetImageOffset(double dOffsetX, double dOffsetY);			// 设置掩模图像相对于显示控件中当前背景图像的位置(逻辑坐标) 
	void	GetImageOffset(double& dOffsetX, double& dOffsetY) const;	// 获取掩模图像相对于显示控件中当前背景图像的位置(逻辑坐标) 
	scRect	GetRect() const;											// 获取掩模图像显示时相对的外接矩形
	CRect	GetImageBoundRect() const;									// 获取掩模图像显示时相对的外接矩形


	// 注： 只有当掩模值的映射颜色为纯黑（即RGB颜色中红=0, 绿=0, 蓝=0 ）时, 
	//      才能使该掩模值对应的像素实现完全透明显示
	void	 SetColorMap(unsigned char nMaskVal, COLORREF color);		// 设置掩模的颜色
	COLORREF GetColorMap(unsigned char nMaskVal) const;					// 获取掩模的颜色

	void	SetTransparencyMap(unsigned char nMaskVal, Transparency tr);// 设置掩模的透明度
	int		GetTransparencyMap(unsigned char nMaskVal) const;			// 获取掩模的透明度

	void	SetDrawState(BOOL bDraw);									// 设置掩模工具处于绘制状态
	BOOL	IsDrawState() const;										// 获取掩模工具是否处于绘制状态

	void	SetDrawLineWidth(int nWidth);								// 设置绘制时的线宽
	int		GetDrawLineWidth() const;									// 获取绘制时的线宽

	void    SetDrawLineMaskColor(unsigned char nMaskVal);				// 设置绘制时所用的掩模值
	BYTE    GetDrawLineMaskColor() const;								// 获取绘制时所用的掩模值

	void	SetDrawMode(PaintMode mode);								// 设置绘制方式
	int		GetDrawMode() const;										// 获取绘制方式

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

protected:
	BOOL	AllocateImageMemory(long lWidth, long lHeight, long lAlignModulus = 1); // 分配图像的数据存储空间

	BOOL	CopyImageData(const scImageGray* pImage);				// 拷贝图像数据
	BOOL    CopyImageData(void* pDib);								// 从MyDib中拷贝图像数据

	BOOL	AllocateBakImageMemory();								// 分配备份图像的数据存储空间
	BOOL	CopyBakImageData();										// 从当前掩模图像拷贝数据到备份图像

	BOOL	AllocateTrueImage();									// 分配图像的数据存储空间
	BOOL	CopyTrueImageData();									// 从8位图像拷贝得到真实图像数据

	void	FillImage(CPoint pt);									// 填充掩模图像

	void	Invalidate();
	void	LoadCursor();
	void	InitTipText();
	int		HitTestHandles(CPoint point) const;
	CRect	GetHandleRect(TrackerHit nHandle) const;

private:


	scRect*			m_pRect;
	scRect*         m_pRectDrag;

	scImageGray*	m_pImage;
	scImageGray*	m_pBakImage;
	
	void*			m_pTrueMyDib;

	BOOL			m_bImageChanged;
	BOOL			m_bDraw;
	int				m_nDrawLineWidth;
	unsigned char	m_nDrawLineColor;
	TrackerHit		m_hitResult;

	RGBQUAD 		m_rgbaColorMap[256];
	static HCURSOR	g_hCursor;			// handles to the cursors

	int				m_nDrawMode;
};

#pragma pack(pop)
