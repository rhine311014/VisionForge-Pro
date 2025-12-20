// svGuiGrayscaleCurve.h

// ---------------------------------------------------------------------
//                            灰度变化曲线控件
// ---------------------------------------------------------------------


#pragma once

#include "svGuiGraphic.h"
#include "vpImage.h"
#include "svGuiCross.h"
#include <vector>
#include "svGuiDisplay.h"

#pragma pack(push,8)

class SVGUI_API scGuiGrayscaleCurve : public scGuiGraphic  
{
public:
	scGuiGrayscaleCurve();
	virtual ~scGuiGrayscaleCurve();

	virtual GraphicType GetType() const {return eGuiGrayscaleCurve;};

	virtual BOOL BoundingRect(CRect& rect) const;

	void Draw(CDC* pDC, LPRECT lpRect = NULL);
	void DrawLabel(CDC* pDC, LPRECT lpRect = NULL);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;

	BOOL Transform(double dScale, double dOffsetX, double dOffsetY);

	void SetTickLineStyle(StyleFlags Linestyle);					//设置刻度线线型
	void SetTickLineWidth(int nWidth);								//设置刻度线线宽
	void SetTickLineColor(COLORREF color);							//设置刻度线颜色

	int GetTickLineStyle();											//获取刻度线线型
	int GetTickLineWidth();											//获取刻度线线宽
	COLORREF GetTickLineColor();									//获取刻度线颜色

	void SetXLineVisible(bool bVisible);							//设置X方向刻度线与灰度变化曲线是否可见
	void SetYLineVisible(bool bVisible);							//设置Y方向刻度线与灰度变化曲线是否可见

	bool GetXLineVisible();											//获取X方向刻度线与灰度变化曲线是否可见
	bool GetYLineVisible();											//获取Y方向刻度线与灰度变化曲线是否可见

	void SetGrayLineStyle(StyleFlags Linestyle);					//设置灰度变化曲线线型(灰度图)
	void SetGrayLineWidth(int nWidth);								//设置灰度变化曲线线宽(灰度图)
	void SetGrayLineColor(COLORREF color);							//设置灰度变化曲线颜色(灰度图)

	int GetGrayLineStyle();											//获取灰度变化曲线线型(灰度图)
	int GetGrayLineWidth();											//获取灰度变化曲线线宽(灰度图)
	COLORREF GetGrayLineColor();									//获取灰度变化曲线颜色(灰度图)

	void SetRLineVisible(bool bVisible);							//设置灰度变化曲线是否可见(彩色图R通道)
	void SetRLineStyle(StyleFlags Linestyle);						//设置灰度变化曲线线型(彩色图R通道)
	void SetRLineWidth(int nWidth);									//设置灰度变化曲线线宽(彩色图R通道)
	void SetRLineColor(COLORREF color);								//设置灰度变化曲线颜色(彩色图R通道)

	bool GetRLineVisible();											//获取灰度变化曲线是否可见(彩色图R通道)
	int GetRLineStyle();											//获取灰度变化曲线线型(彩色图R通道)
	int GetRLineWidth();											//获取灰度变化曲线线宽(彩色图R通道)
	COLORREF GetRLineColor();										//获取灰度变化曲线颜色(彩色图R通道)

	void SetGLineVisible(bool bVisible);							//设置灰度变化曲线是否可见(彩色图G通道)
	void SetGLineStyle(StyleFlags Linestyle);						//设置灰度变化曲线线型(彩色图G通道)
	void SetGLineWidth(int nWidth);									//设置灰度变化曲线线宽(彩色图G通道)
	void SetGLineColor(COLORREF color);								//设置灰度变化曲线颜色(彩色图G通道)

	bool GetGLineVisible();											//获取灰度变化曲线是否可见(彩色图G通道)
	int GetGLineStyle();											//获取灰度变化曲线线型(彩色图G通道)
	int GetGLineWidth();											//获取灰度变化曲线线宽(彩色图G通道)
	COLORREF GetGLineColor();										//获取灰度变化曲线颜色(彩色图G通道)

	void SetBLineVisible(bool bVisible);							//设置灰度变化曲线是否可见(彩色图B通道)
	void SetBLineStyle(StyleFlags Linestyle);						//设置灰度变化曲线线型(彩色图B通道)
	void SetBLineWidth(int nWidth);									//设置灰度变化曲线线宽(彩色图B通道)
	void SetBLineColor(COLORREF color);								//设置灰度变化曲线颜色(彩色图B通道)

	bool GetBLineVisible();											//获取灰度变化曲线是否可见(彩色图B通道)
	int GetBLineStyle();											//获取灰度变化曲线线型(彩色图B通道)
	int GetBLineWidth();											//获取灰度变化曲线线宽(彩色图B通道)
	COLORREF GetBLineColor();										//获取灰度变化曲线颜色(彩色图B通道)

	void SetCrossLineVisible(bool bVisible);						//设置交叉线是否可见
	void SetCrossLineStyle(StyleFlags Linestyle);					//设置交叉线线型
	void SetCrossLineWidth(int nWidth);								//设置交叉线线宽
	void SetCrossLineColor(COLORREF color);							//设置交叉线颜色

	bool GetCrossLineVisible();										//获取交叉线是否可见
	int GetCrossLineStyle();										//获取交叉线线型
	int GetCrossLineWidth();										//获取交叉线线宽
	COLORREF GetCrossLineColor();									//获取交叉线颜色

protected:
	void DrawNormal(CDC* pDC, LPRECT lpRect = NULL) const;

	void  LoadCursor();

protected:
	mutable CPoint  m_ptCur;

	static HCURSOR g_hCursor[1];		// handles to the cursors

	
	bool        m_bShowXLine;           // 是否显示X方向刻度线和灰度变化曲线
	bool        m_bShowYLine;           // 是否显示Y方向刻度线和灰度变化曲线

	//刻度线
	StyleFlags	m_nTickLineStyle;			// 线型
	int			m_nTickLineWidth;			// 线宽
	COLORREF	m_TickLineColor;			// 线的颜色

	//灰度变化曲线(灰度图)
	StyleFlags	m_nGrayLineStyle;			// 线型
	int			m_nGrayLineWidth;			// 线宽
	COLORREF	m_GrayLineColor;			// 线的颜色

	//灰度变化曲线(彩色图R通道)
	bool        m_bShowRLine;				// 是否显示R通道曲线
	StyleFlags	m_nRLineStyle;				// 线型
	int			m_nRLineWidth;				// 线宽
	COLORREF	m_RLineColor;				// 线的颜色

	//灰度变化曲线(彩色图G通道)
	bool        m_bShowGLine;				// 是否显示G通道曲线
	StyleFlags	m_nGLineStyle;				// 线型
	int			m_nGLineWidth;				// 线宽
	COLORREF	m_GLineColor;				// 线的颜色

	//灰度变化曲线(彩色图B通道)
	bool        m_bShowBLine;				// 是否显示B通道曲线
	StyleFlags	m_nBLineStyle;				// 线型
	int			m_nBLineWidth;				// 线宽
	COLORREF	m_BLineColor;				// 线的颜色

	//交叉线
	bool		m_bShowCrossLine;           // 是否显示
	StyleFlags	m_nCrossLineStyle;			// 线型
	int			m_nCrossLineWidth;			// 线宽
	COLORREF	m_CrossLineColor;			// 线的颜色
};

#pragma pack(pop)