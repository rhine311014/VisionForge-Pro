#pragma once

#include "vsXYDVisionAlign.h"
#include "svGuiDisplay.h"
#include "XPStyleButtonST.h"
#include "svGuiRect.h"
#include "afxwin.h"
#include "Keyboard.h"


// CDlgAlignOffsetByAreaSet 对话框


#define MAX_AREA_RECT_NUM 25

class CDlgVisionRunAlignInfo;

class CDlgAlignOffsetByAreaSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlignOffsetByAreaSet)

public:
	CDlgAlignOffsetByAreaSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlignOffsetByAreaSet();

// 对话框数据
	enum { IDD = IDD_DLG_ALIGN_OFFSET_BY_AREA_SET };

	// 设置显示引用
	void SetParentWnd(CDlgVisionRunAlignInfo* pParentWnd) { m_pParentWnd = pParentWnd; }
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void SetKeyBoard(bool bEnable) { m_bKeyboardEnable = bEnable; }

	void UpdateDlgData(bool bSaveAndValidate);



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


	void InitDlgItem();			// 初始化Dlg
	void InitStatusUI();		// 初始化状态栏
	void InitDisplayUI();		// 初始化显示控件
	void CleanDisplayUI();		// 释放显示控件


	void OnCamGrab(bool bStart = true);
	int GetLanguageType();
	void RestoreStatus();
	void UpdateStatusBar();		// 更新状态显示
	void UpdateDialogLanguage();
	void UpdateAreaRectShow();
	void UpdateSearchRectToGui();		// 更新区域大小到GUI
	void UpdateSearchRectFromGui();		// 更新区域大小到控件
	void UpdateGuiStyle();


	CDlgVisionRunAlignInfo* m_pParentWnd;
	vcXYDVisionAlign *m_pVisionASM;
	CStringArray	*m_psaSysInfoStrings;
	bool m_bIsGrabbing;
	bool  m_bKeyboardEnable;
	CKeyBoard m_KeyBoard;

	// GUI
	CWnd *m_pVAPrevMainWnd;	
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;

	// 搜索矩形
	scGuiRect	m_GuiSearchRect;

	// 分区域矩形 最多25个
	scGuiRect   m_GuiAreaRect[MAX_AREA_RECT_NUM];




	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;


	//
	int m_nEditRowNum;
	int m_nEditColNum;
	double m_dEditStartX;
	double m_dEditStartY;
	double m_dEditWidth;
	double m_dEditHeight;
	CXPStyleButtonST m_btnNormalColor;
	CXPStyleButtonST m_btnLightColor;
	BOOL m_bChkAlignOffsetByAreaEnable;
	int m_nEditLineWidth;
	int m_nEditLightLineWidth;
	COLORREF m_clrNormal;
	COLORREF m_clrLight;
	int m_nRdSearchType;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayDragStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayDragStop(WPARAM wParam, LPARAM lParam);

	afx_msg void OnEnChangeEditRowNum();
	afx_msg void OnEnChangeEditColNum();
	afx_msg void OnBnClickedBtnNormalColor();
	afx_msg void OnBnClickedBtnLightColor();
	afx_msg void OnEnChangeEditLineWidth();
	afx_msg void OnEnChangeEditLightLineWidth();
	afx_msg void OnEnChangeEditStartX();
	afx_msg void OnEnChangeEditStartY();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnSetfocusEditRowNum();
	afx_msg void OnEnSetfocusEditColNum();
	afx_msg void OnEnSetfocusEditStartX();
	afx_msg void OnEnSetfocusEditStartY();
	afx_msg void OnEnSetfocusEditHeight();
	afx_msg void OnEnSetfocusEditWidth();
	afx_msg void OnEnSetfocusEditLineWidth();
	afx_msg void OnEnSetfocusEditLightLineWidth();
	afx_msg void OnBnClickedBtnGrabImage();

	int		m_nCurPositionIdx;
	scGuiDisplay m_guiDisplay;


};
