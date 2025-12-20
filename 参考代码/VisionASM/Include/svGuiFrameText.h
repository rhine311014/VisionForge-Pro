// svGuiText.h

// ---------------------------------------------------------------------
//                        文本显示控件
// ---------------------------------------------------------------------

#pragma once

#include "svGuiGraphic.h"
#include "svText.h"

#pragma pack(push,8)

enum AnchorMode
{
	eLeftTop = 0,
	eLeftBottom,
	eRightTop,
	eRightBottom
};

enum FrameShowMode
{
	eFitText = 0,
	eFixedSize
};

enum AlignmentHorizontal
{
	eAlignmentHorizontalleft = 0,
	eAlignmentHorizontalCenter,
	eAlignmentHorizontalRight
};

enum AlignmentVertical
{
	eAlignmentVerticalTop = 0,
	eAlignmentVerticalCenter,
	eAlignmentVerticalBottom
};

//////////////////////////////////////////////////////////////////////////
// 背景框显示模式：1、背景框适应文本 2、背景框固定大小（默认背景框适应文本）
// 1、背景框适应文本时，文本只有水平对齐方式
// 2、背景框固定大小时，文本可设置水平和垂直方向对齐方式（默认左上对齐），文本超出背景框时可选择显示或不显示（默认显示）
// 背景框位置通过背景框顶点进行设置，文本位置由背景框位置及边距决定（默认边距为0,即文本位置）
//////////////////////////////////////////////////////////////////////////

class SVGUI_API scGuiFrameText : public scGuiGraphic  
{
public:
	scGuiFrameText();
	virtual ~scGuiFrameText();

	virtual GraphicType GetType() const {return eGuiFrameText;};
public:
	// 设置文本是否跟随图片缩放
	void	SetTextZoomEnable(bool bEnable);
	bool	IsTextZoomEnable() const;

	// 设置/获取文本超出背景框后是否显示
	void SetOutRangeShowEnable(bool bEnable);
	bool IsOutRangeShowEnable() const;

	// 设置/获取文本
	void	SetText(LPCTSTR lpszText);
	LPCTSTR	GetText() const;

	LPCTSTR GetTextRow(int row) const;
	int GetTextRowCount() const;

	// 设置/获取字体
	void	 SetFont(LOGFONT LogFont);			
	LOGFONT GetFont() const;

	// 设置/获取文本行间距
	void SetTextRowSpace(double space);
	double GetTextRowSpace() const;

	// 设置/获取文本对齐方式
	void SetTextAlignment(AlignmentHorizontal horizontal, AlignmentVertical vertical);
	AlignmentHorizontal GetTextAlignmentHorizontal() const;
	AlignmentVertical GetTextAlignmentVertical() const;

	// 设置/获取文本上下左右边距
	void SetTextMargin(double top, double bottom, double left, double right);
	void GetTextMargin(double& top, double& bottom, double& left, double& right) const;

	// 获取文本的宽高
	bool GetTextRegion(scRect& rect) const;
	bool GetTextRowRegion(int row, scRect& rect) const;

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

public:
	// 设置/获取背景框是否可见
	void SetFrameEnable(bool bEnable);
	bool IsFrameEnable() const;

	// 设置/获取背景框显示模式
	void SetFrameShowMode(FrameShowMode mode);
	FrameShowMode GetFrameShowMode() const;

	// 设置/获取背景框锚点，位置
	void SetFramePos(sc2Vector vPos, AnchorMode anchor = eLeftTop);
	AnchorMode GetAnchorMode() const;
	sc2Vector GetFramePos() const;
	sc2Vector GetFramePos(AnchorMode anchor) const;

	// 设置/获取背景框大小
	void SetFrameSize(sc2Vector vSize);
	sc2Vector GetFrameSize() const;

	// 设置/获取背景框旋转角度
	void	SetRotation(scRadian rot);
	scRadian  GetRotation() const;

	void SetFramePosSizeRotation(double dX, double dY, sc2Vector vSize, scRadian rRot, AnchorMode anchor);
	void SetFramePosSizeRotation(sc2Vector vPos, sc2Vector vSize, scRadian rRot, AnchorMode anchor);

	// 设置/获取背景框线型、线宽、颜色
	void SetFrameLineStyle(StyleFlags style);
	void SetFrameLineWidth(int width);
	void SetFrameLineColor(COLORREF color);

	StyleFlags GetFrameLineStyle() const;
	int GetFrameLineWidth() const;
	COLORREF GetFrameLineColor() const;

	// 设置/获取背景是否填充
	void SetBackgroundFillEnable(bool bEnable);
	bool IsBackgroundFillEnable() const;

	// 设置/获取背景填充颜色
	void SetBackgroundFillColor(COLORREF color);
	COLORREF GetBackgroundFillColor() const;

	// 设置/获取文本框是否与DC显示区域相对位置不变（只适用于ConstContainer）
	void EnableRelativePosUnchange(bool bChange);
	bool IsRelativePosUnchange()const;

	virtual BOOL BoundingRect(CRect& rect) const;

private:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;
protected:
	void*	 m_pLShape;
	LOGFONT	 m_TextFont;
private:
	bool		m_bTextZoom;			// 文本缩放
	int			m_nAlignmentHorizontal;	// 水平对齐
	int			m_nAlignmentVertical;	// 竖直对齐
	bool		m_bOutRangeShow;		// 文本超出显示
	double		m_dRowSpace;			// 行距
	double		m_dMarginTop;			// 上边距
	double		m_dMarginBottom;		// 下边距
	double		m_dMarginLeft;			// 左边距
	double		m_dMarginRight;			// 右边距

	bool		m_bFrameVisible;		// 背景框是否可见
	int			m_nFrameShowMode;		// 背景框显示模式
	StyleFlags	m_nFrameLineStyle;		// 背景框线型
	int			m_nFrameLineWidth;		// 背景框线宽
	COLORREF	m_cFrameLineColor;		// 背景框颜色
	int			m_nFrameAnchor;			// 背景框锚点
	sc2Vector	m_vFrameSize;			// 背景框大小
	bool		m_bBackgroundFill;		// 背景填充
	COLORREF	m_cBackgroundColor;		// 背景填充颜色

	bool		m_bRelativePosChange;	// 相对位置是否改变
};

#pragma pack(pop)
