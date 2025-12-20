#if !defined(AFX_DLGSETCAMERASERIAL_H__F14500EA_7DED_4D97_A213_6BF9847DFE02__INCLUDED_)
#define AFX_DLGSETCAMERASERIAL_H__F14500EA_7DED_4D97_A213_6BF9847DFE02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetCameraSerial.h : header file
//
#include "KeyBoard.h"
#include <VECTOR>

#include "svGuiDisplay.h"
//#include "vsGrab.h"
#include "vsCamera.h"
#include "vsVisionAlignDef.h"

#define MaxPlatformNum	6	// 最大的平台数量

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCameraSerial dialog

class CVisionASMDlg;
class CDlgSetCameraSerial : public CDialog
{
// Construction
public:
	CDlgSetCameraSerial(CWnd* pParent);   // standard constructor
	~CDlgSetCameraSerial();

// Dialog Data
	//{{AFX_DATA(CDlgSetCameraSerial)
	enum { IDD = IDD_DLG_SET_CAMERA_SERIAL };
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST m_btnCancel;
	CXPStyleButtonST m_btnApply;
	CXPStyleButtonST m_btnShowPlatform1;
	CXPStyleButtonST m_btnShowPlatform2;
	CXPStyleButtonST m_btnShowPlatform3;
	CXPStyleButtonST m_btnShowPlatform4;
	CXPStyleButtonST m_btnShowPlatform5;
	CXPStyleButtonST m_btnShowPlatform6;
	CXPStyleButtonST m_btnPanel;
	CString m_strOldSN0;
	CString m_strOldSN1;
	CString m_strOldSN2;
	CString m_strOldSN3;
	CString m_strOldSN4;
	CString m_strOldSN5;
	CString m_strOldSN6;
	CString m_strOldSN7;
	CComboBox m_comboNewSN0;
	CComboBox m_comboNewSN1;
	CComboBox m_comboNewSN2;
	CComboBox m_comboNewSN3;
	CComboBox m_comboNewSN4;
	CComboBox m_comboNewSN5;
	CComboBox m_comboNewSN6;
	CComboBox m_comboNewSN7;
	CComboBox m_comboSN;
	CString m_strOldPosCamIndex0;
	CString m_strOldPosCamIndex1;
	CString m_strOldPosCamIndex2;
	CString m_strOldPosCamIndex3;
	CString m_strOldPosCamIndex4;
	CString m_strOldPosCamIndex5;
	CString m_strOldPosCamIndex6;
	CString m_strOldPosCamIndex7;
	CComboBox m_comboNewPosCamIndex0;
	CComboBox m_comboNewPosCamIndex1;
	CComboBox m_comboNewPosCamIndex2;
	CComboBox m_comboNewPosCamIndex3;
	CComboBox m_comboNewPosCamIndex4;
	CComboBox m_comboNewPosCamIndex5;
	CComboBox m_comboNewPosCamIndex6;
	CComboBox m_comboNewPosCamIndex7;
	BOOL m_bPosEnable0;
	BOOL m_bPosEnable1;
	BOOL m_bPosEnable2;
	BOOL m_bPosEnable3;
	BOOL m_bPosEnable4;
	BOOL m_bPosEnable5;
	BOOL m_bPosEnable6;
	BOOL m_bPosEnable7;
	CString	m_strHelpInfo;
	int		m_nPlatformIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetCameraSerial)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetCameraSerial)
	virtual BOOL OnInitDialog();
	afx_msg void OnPlatformChange();
	virtual void OnOK();
	virtual void OnCancel();
	BOOL ConfigShareCamInfo(CArray<CString, CString> *paulSN);   //配置相机共享方式
	afx_msg void OnClose();
	afx_msg void OnBtnGrab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CThemeHelperST		m_themeHelper;
	
	CVisionASMDlg *m_pParent;
	int m_nPlatformNum;
	int m_nPrevPlatformIndex;
	std::vector<DWORD>* m_pvdwInitErrCode;
	std::vector<CComboBox*> m_vpcmbNewSN;
	std::vector<CString*> m_vpstrOldSN;
	std::vector<CComboBox*> m_vpcmbNewPosCamIndex;	
	std::vector<BOOL*> m_vpbPosEnable;
	std::vector<CString*> m_vpstrOldPosCamIndex;
	std::vector<std::vector<CameraParam>> m_vPreCameraParam;    //记录之前各平台各工位的相机参数，重新配置时利用其释放相机对象
// 	CArray<unsigned long, unsigned long> *m_paulOldSN;
// 
// 	CArray<unsigned long, unsigned long> m_aulBusCamSN;

	CArray<CString, CString> *m_paulOldSN;

	CArray<CString, CString> m_aulBusCamSN;

	CArray<CPosInfo, CPosInfo> *m_paulOldPosInfo;


	std::vector<CWnd*> m_vpOldMainWnd;



protected:
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;

	BOOL SaveAndInvalidateAll();
	void UpdateDialogLanguage();
// 	BOOL IsSNSettingsValid();

public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

	void SetInitErrorCode(std::vector<DWORD>* pvdwInitErrCode);

public:
	BOOL            m_bNotEnterApp;  // Mali 2013.9.18

public:
	scGuiDisplay *m_pGuiDisplay;
	scPTGrab2*	  m_pCamera;
	cpImage	  m_ImageInput;
	CRITICAL_SECTION                   m_csResource;

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;

	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;
	
protected:
	static void WINAPI GrabImageFinishedCallback(void* pOwner, PTStatus status, unsigned char* pImage, 					
		long lWidth, long lHeight, PTPixelFormat pixelFormat); // 相机采集完成的回调函数，所有相机只有一个函数，这个是设计的比较麻烦的问题
	void ImageProcess(int nIndex, cpImage srcImage);
	BOOL IsShareCameraSystem();           //判断软件系统是否是相机共享系统
	void UpdateStatusBar();
public:
	CString strcomboNewSN0Old;
	CString strcomboNewSN1Old;
	CString strcomboNewSN2Old;
	CString strcomboNewSN3Old;
	CString strcomboNewSN4Old;
	CString strcomboNewSN5Old;
	CString strcomboNewSN6Old;
	CString strcomboNewSN7Old;
	CString strcomboSNOld;
	CString strcomboNewPosCamIndex0Old;
	CString strcomboNewPosCamIndex1Old;
	CString strcomboNewPosCamIndex2Old;
	CString strcomboNewPosCamIndex3Old;
	CString strcomboNewPosCamIndex4Old;
	CString strcomboNewPosCamIndex5Old;
	CString strcomboNewPosCamIndex6Old;
	CString strcomboNewPosCamIndex7Old;
	afx_msg void OnCbnSelchangeCmbCh0NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh1NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh2NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh3NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh4NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh5NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh6NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbCh7NewSerialNumber();
	afx_msg void OnCbnSelchangeCmbSerialNumber();
	afx_msg void OnCbnSelchangeCmbPos0NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos1NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos2NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos3NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos4NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos5NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos6NewCamIndex();
	afx_msg void OnCbnSelchangeCmbPos7NewCamIndex();
	afx_msg void OnBnClickedCheckPos0Enable();
	afx_msg void OnBnClickedCheckPos1Enable();
	afx_msg void OnBnClickedCheckPos2Enable();
	afx_msg void OnBnClickedCheckPos3Enable();
	afx_msg void OnBnClickedCheckPos4Enable();
	afx_msg void OnBnClickedCheckPos5Enable();
	afx_msg void OnBnClickedCheckPos6Enable();
	afx_msg void OnBnClickedCheckPos7Enable();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETCAMERASERIAL_H__F14500EA_7DED_4D97_A213_6BF9847DFE02__INCLUDED_)
