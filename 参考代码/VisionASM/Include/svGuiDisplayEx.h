#pragma once

#include "svGuiDisplay.h"
#pragma pack(push,8)

#ifdef SVGUIEX_EXPORTS
#define SVGUIEX_API __declspec(dllexport)
#else
#define SVGUIEX_API __declspec(dllimport)
#endif

class scGuiDIBBaseEx;
class CDlgHeightSetting;
class CDlgLuminanceSetting;
class SVGUIEX_API scGuiDisplayEx : public scGuiDisplay
{
public:
	scGuiDisplayEx();
	virtual ~scGuiDisplayEx();

	enum HitRamp
	{
		eHitNone = -1,
		eHitConfusion,
		eHitHeightLow,
		eHitHeightHigh,
		eHitLuminanceLow,
		eHitLuminanceHigh
	};

	friend class CDlgHeightSetting;
	friend class CDlgLuminanceSetting;

	// 在给定位置上创建显示窗体
	// nPosID：父窗体上确定待创建显示控件位置的控件的ID；
	// lpRect：待创建显示控件在父窗体上的位置；
	// pParentWnd：父窗体指针；
	virtual BOOL	Create(UINT nPosID, CWnd* pParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);		
	virtual BOOL	Create(LPRECT lpRect, CWnd* pParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);
	virtual BOOL	Create(HWND handlePosWnd, HWND handleParentWnd, DWORD dwStyle = WS_HSCROLL|WS_VSCROLL, DWORD dwExStyle = NULL);	

	// 设置/获取高度+浓淡图像，image必须为32位图像，高度+浓淡
	bool SetImageCombine(const cpImage& image);

	// 设置/获取高度图像，image必须为灰度图像，灰度值表示高度
	bool SetImageHeight(const cpImage& image,bool bCopy = false);
	const cpImage* GetImageHeight()const {return m_pImageHeight;}
	bool GetImageHeightPixelValue(long lImageX, long lImageY, unsigned short& value, BYTE& R, BYTE& G, BYTE& B) const;

	// 设置/获取浓淡图像，image必须为灰度图像，灰度值表示浓淡
	bool SetImageLuminance(const cpImage& image,bool bCopy = false);
	const cpImage* GetImageLuminance()const {return m_pImageLuminance;}
	bool GetImageLuminancePixelValue(long lImageX, long lImageY, unsigned short& value, BYTE& R, BYTE& G, BYTE& B) const;

	// 获取高度+浓淡图像(32位)
	bool GetImageCombine(cpImage& image)const;

	// 获取高度伪彩色图像
	bool GetImageHeight(cpImage& image)const;

	// 获取浓淡图像(显示)
	bool GetImageLuminance(cpImage& image)const;

	// 获取融合图像
	bool GetImageConfusion(cpImage& image)const;

	// 设置/获取融合比例
	void SetConfusionRatio(double ratio);
	double GetConfusionRatio()const {return m_dConfusionRatio;}

	// 设置/获取高度刻度最大最小值(显示相关)
	void SetRealHeight(double dHeightMin,double dHeightMax);
	void GetRealHeight(double& dHeightMin,double& dHeightMax);

	// 设置/获取融合图例是否可见
	void SetConfusionRampVisible(bool bShow);
	bool IsConfusionRampVisible()const {return m_bConfusionRampVisible;}

	// 设置/获取高度色柱图例是否可见
	void SetHeightRampVisible(bool bShow);
	bool IsHeightRampVisible() const {return m_bHeightRampVisible;}

	// 设置/获取浓淡色柱图例是否可见
	void SetLuminanceRampVisible(bool bShow);
	bool IsLuminanceRampVisible()const {return m_bLuminanceRampVisible;}

	// 设置/获取融合比例图例位置
	void SetConfusionRampRect(CRect rc);
	CRect GetConfusionRampRect()const {return m_rcConfusionRamp;}

	// 设置/获取高度图例位置
	void SetHeightRampRect(CRect rc);
	CRect GetHeightRampRect()const {return m_rcHeightRamp;}

	// 设置/获取浓淡图例位置
	void SetLuminanceRampRect(CRect rc);
	CRect GetLuminanceRampRect()const {return m_rcLuminanceRamp;}

	// 设置/获取是否自动适应高度色柱
	void SetAutoFitColorBar(bool bAutoFit) {m_bAutoFitColorBar = bAutoFit;}
	bool IsAutoFitColorBar()const {return m_bAutoFitColorBar;}

	// 设置/获取是否自动适应浓淡色柱
	void SetAutoFitLuminanceBar(bool bAutoFit) {m_bAutoFitLuminanceBar = bAutoFit;}
	bool IsAutoFitLuminanceBar()const {return m_bAutoFitLuminanceBar;}

	// 设置/获取高度最大最小值(像素值)
	void SetHeightMaxMin(DWORD lHeightMin,DWORD lHeightMax);
	void GetHeightMaxMin(DWORD& lHeightMin,DWORD& lHeightMax)const;

	// 设置/获取调节高度最大最小值（像素值）
	void SetAdjustHeightMaxMin(DWORD lAdjustHeightMin,DWORD lAdjustHeightMax);
	void GetAdjustHeightMaxMin(DWORD& lAdjustHeightMin,DWORD& lAdjustHeightMax)const;

	// 设置/获取浓淡最大最小值(像素值)
	void SetLuminanceMaxMin(DWORD lLuminanceMin,DWORD lLuminanceMax);
	void GetLuminanceMaxMin(DWORD& lLuminanceMin,DWORD& lLuminanceMax)const;

	// 设置/获取调节浓淡最大最小值（像素值）
	void SetAdjustLuminanceMaxMin(DWORD lAdjustLuminanceMin,DWORD lAdjustLuminanceMax);
	void GetAdjustLuminanceMaxMin(DWORD& lAdjustLuminanceMin,DWORD& lAdjustLuminanceMax)const;

	// 设置/获取融合比例微调步长
	void SetConfusionRatioStep(double dDelta);
	double GetConfusionRatioStep() const {return m_dConfusionRatioStep;}

	// 设置/获取高度微调步长
	void SetHeightStep(double dDelta);
	double GetHeightStep()const {return m_dRealHeigtStep;}

	// 设置/获取浓淡微调步长
	void SetLuminanceStep(double dDelta);
	double GetLuminanceStep()const {return m_dLuminanceStep;}

	// 从显示控件中删除图像(高度图像+浓淡图像),会影响图形的显示位置
	virtual void RemoveImage(BOOL bReset = TRUE);

	// 从显示控件中删除高度图像,会影响图形的显示位置
	virtual void RemoveHeightImage(BOOL bReset = TRUE);

	// 从显示控件中删除浓淡图像,会影响图形的显示位置
	virtual void RemoveLuminanceImage(BOOL bReset = TRUE);

	// // 清除屏幕内容(删除图像,清空静态和交互图形容器内容)
	virtual void ClearScreen(BOOL bReset = TRUE);

protected:
	// 更新高度图像的最小最大值
	bool UpdateHeightMinMax();

	// 更新浓淡图像的最小最大值
	bool UpdateLuminanceMinMax();

	// 绘制图像
	void DrawImageEx(CDC* pDC, CRect rcDC) const;

	// 加载图例
	void LoadBmpRamp();

	// 加载图例拖拽光标
	void LoadCursorRamp();

	// 绘制图例
	void DrawRamp(CDC* pDC,CRect rcDC);

	// 绘制图例按钮
	void DrawRampButton(CDC* pDC,CRect rcDC);

	// 击中测试
	int HitRampTest(CPoint point)const;

	// 创建图例按钮
	bool CreateRampButton();

protected:
	const cpImage* m_pImageHeight;    // 高度图像指针
	const cpImage* m_pImageLuminance; // 浓淡图像指针

	bool     m_bCopyImageHeight;      // 是否复制高度图像
	bool     m_bCopyImageLuminance;   // 是否复制浓淡图像

	DWORD    m_lHeightMin;            // 最小高度（图像像素值），0为未检测到的点，所以该值不能为0
	DWORD    m_lHeightMax;            // 最大高度（图像像素值）
	DWORD    m_lLuminanceMin;         // 最小浓淡值
	DWORD    m_lLuminanceMax;         // 最大浓淡值

	DWORD    m_lAdjustHeightMin;      // 可调节最小高度值
	DWORD    m_lAdjustHeightMax;      // 可调节最大高度值
	DWORD    m_lAdjustLuminanceMin;   // 可调节最小浓淡值
	DWORD    m_lAdjustLuminanceMax;   // 可调节最大浓淡值

	CRect    m_rcHeightRamp;          // 高度色柱
	CRect    m_rcLuminanceRamp;       // 灰度色柱
	CRect    m_rcConfusionRamp;       // 高度灰度混合比例图例

	bool     m_bHeightRampVisible;    // 高度色柱是否可见
	bool     m_bLuminanceRampVisible; // 灰度色柱是否可见
	bool     m_bConfusionRampVisible; // 高度灰度混合比例图例是否可见

	double   m_dConfusionRatio;       // 融合比例0~1

	scGuiDIBBaseEx* m_pDibEx;         // 绘制dib

	static HBITMAP  g_hBmpRamp[7];    // 图例
	static HCURSOR  g_hCursorRamp[1]; // 图例拖拽光标

	HitRamp  m_hitRamp;               // 是否击中图例handle

	LOGFONT  m_RampFont;              // 图例字体

	double   m_dRealHeightMin;        // 真实高度（mm）
	double   m_dRealHeightMax;

	bool     m_bAutoFitColorBar;     // 自动适应彩带
	bool     m_bAutoFitLuminanceBar; // 自动适应浓淡色柱

	CButton  m_BtnConfusionLeft;     // 减小融合比例按钮
	CButton  m_BtnConfusionRight;    // 增加融合比例按钮
	CButton  m_BtnHeightSetting;     // 高度设置按钮
	CButton  m_BtnLuminanceSetting;  // 浓淡设置按钮

	double   m_dConfusionRatioStep;  // 融合比例微调步长
	double   m_dRealHeigtStep;       // 高度微调步长
	double   m_dLuminanceStep;       // 浓淡微调步长

protected:
	//{{AFX_MSG(scGuiDisplay)
	afx_msg void OnPaint();
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
 	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
 	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
 	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAutoFitColorBar();
	afx_msg void OnAutoFitLuminanceBar();
	afx_msg void OnBtnConfusionLeft();
	afx_msg void OnBtnConfusionRight();
	afx_msg void OnBtnHeightSetting();
	afx_msg void OnBtnLuminanceSetting();
	afx_msg void OnConfusionRampVisible();
	afx_msg void OnHeightRampVisible();
	afx_msg void OnLuminanceRampVisible();
	afx_msg void OnSaveImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#pragma pack(pop)