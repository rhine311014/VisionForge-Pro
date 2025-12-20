#if !defined(AFX_DLGSYSIMGROIPARAM_H__978ED29C_03CB_42B9_AD69_5914E22E72DB__INCLUDED_)
#define AFX_DLGSYSIMGROIPARAM_H__978ED29C_03CB_42B9_AD69_5914E22E72DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysImgRoiParam.h : header file
//

#include <VECTOR>
#include "vsVisionAlignDef.h"
#include "vsBaseVisionAlign.h"
#include "KeyBoard.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysImgRoiParam dialog
class CDlgSysEnvironSetting;
class CDlgSysImgRoiParam : public CDialog
{
// Construction
public:
	CDlgSysImgRoiParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysImgRoiParam)
	enum { IDD = IDD_SYS_IMGROI_PARAM };
	CXPStyleButtonST	m_btnSetCustom;
	CXPStyleButtonST	m_btnSaveImage;
	CXPStyleButtonST	m_btnGrabImage;
	CXPStyleButtonST	m_btnManualMeasure;
	CSliderCtrl	m_sliderShutter;
	CSliderCtrl	m_sliderGain;
	CSliderCtrl	m_sliderFramerate;
	CSliderCtrl	m_sliderGamma;
	BOOL	m_bGammaEnable;
	BOOL	m_bHorizontalMirror;
	BOOL	m_bVerticalMirror;
	int		m_nHeightCustom;
	int		m_nWidthCustom;
	int		m_nStartX;
	int		m_nStartY;
	int		m_nCameraIndex;
	int		m_nVideoMode;
	int		m_nRotationIndex;
	double	m_dCrossX;
	double	m_dCrossY;
	CString m_strCrossX;
	CString m_strCrossY;
	BOOL	m_bCrossVisible;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysImgRoiParam)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysImgRoiParam)
	afx_msg void OnBtnLiveGrab();
	afx_msg void OnBtnSaveImage();
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraChange();
	afx_msg void OnRotationChange();
	afx_msg void OnMirrorChange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditCrossX();
	afx_msg void OnKillfocusEditCrossY();
	afx_msg void OnBtnSetCorssImageCenter();
	afx_msg void OnSetfocusEditCrossX();
	afx_msg void OnSetfocusEditCrossY();
	afx_msg void OnCheckCrossVisible();
	afx_msg void OnBnClickedButtonManualMeasure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void InitDlgItem();

	/************************************************************************************
	/* Function name	: UpdateDlgData
	/* Description	    : 
	/* Return type		: void 
	/* Argument
	/* [in] --  BOOL bSaveAndValidate:	如果为TRUE，则将所有界面设置保存到对应VisionAlinger
	/*									中；如果为FALSE，则将VisionAlinger中的参数更新到终端，
	/*									包括界面参数和图像显示。
	/************************************************************************************/
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	// 根据相机状态更新“连续采集/停止采集”按钮显示
	void UpdateBtnDisplay();
	// 根据图像模式更新帧率、曝光时间和增益控件的显示
	void UpdateFramerateShutterGainDisplay();

	// 设置图像显示控件。注意先后顺序，应先调用SetCameraDisplay，再调SetVisionASMRef
	void SetCameraDisplay(scGuiDisplay *pDisplay);

	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);
	vcBaseVisionAlign* GetVisionASMRef () const;
	
	BOOL IsCameraGrabing() const;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }
	BOOL IsStringNumerical(LPCTSTR lpszVal);
protected:
	CThemeHelperST		m_themeHelper;
	
	CWnd *m_pVAPrevMainWnd;
	CDlgSysEnvironSetting *m_pParent;
	vcBaseVisionAlign *m_pVisionASM;
	scGuiDisplay *m_pGUIDisplay;
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplay;
	int m_nPrevCameraIndex;

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	
	double m_dWhiteBalanceRatioR;	// 自动白平衡R系数
	double m_dWhiteBalanceRatioG;	// 自动白平衡R系数
	double m_dWhiteBalanceRatioB;	// 自动白平衡R系数	

	void UpdateDialogLanguage();
public:
	afx_msg void OnBnClickedButtonShutterDown();
	afx_msg void OnBnClickedButtonShutterUp();
	afx_msg void OnBnClickedButtonGainDown();
	afx_msg void OnBnClickedButtonGainUp();
	double	m_dShutterOld;
	double	m_dGainOld;
	double	m_dFramerateOld;
	double	m_dGammaOld;
	afx_msg void OnBnClickedCheckGamma();
	afx_msg void OnBnClickedBtnAutoBalance();
	afx_msg void OnBnClickedBtnSetWhiteBalanceRatioDefault();
	
	void ClipMouseCursor(BOOL bClip);

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow); // 将nID的Group范围内的控件设置为启用禁用以及显示隐藏
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

private:
	bool m_bCamPixelFormat;			// 相机像素格式是否彩色

	bool m_bShowParamSet;
	bool m_bEnableParamSet;

	bool m_bShowColorSet;
	bool m_bEnableColorSet;

	bool m_bShowOtherSet;
	bool m_bEnableOtherSet;
public:
	afx_msg void OnBnClickedBtnLightCtrlSet();

	afx_msg void OnEnSetfocusEditInputShutterVal();
	afx_msg void OnEnSetfocusEditInputGainVal();
	afx_msg void OnEnKillfocusEditInputShutterVal();
	afx_msg void OnEnKillfocusEditInputGainVal();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSIMGROIPARAM_H__978ED29C_03CB_42B9_AD69_5914E22E72DB__INCLUDED_)
