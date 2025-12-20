#pragma once

#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"


// CDlgCalibObjectPos2Code 对话框

class CDlgCalibObjectPos2Code : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibObjectPos2Code)

public:
	CDlgCalibObjectPos2Code(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibObjectPos2Code();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_OBJECT_POS2_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CWnd *m_pVAPrevMainWnd;	
	vcXYDVisionAlign *m_pVisionASM;
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;

	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings);
	void EnableSysKeyboard(BOOL bEnable);
	void InitDlgItem();
	void UpdateDialogLanguage();



	CLabel m_labelNote;
	CComboBox m_comboExProductIndex;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;
	std::vector<scGuiInteractiveContainer*> m_vpGuiInteractiveContainer;
	std::vector<BOOL> m_vbCalibObjectPosSuccess;


	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;
	void UpdateStatusBar();

	// 通信交互
	int m_nEvent;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;
	static  UINT ThreadReceiveData(LPVOID pParam);

	// 相机采集
	BOOL m_bIsGrabbing;
	void CameraGrab();

	// 二维码定位
	BOOL DmCodeSearch(int nPosIndex);

public:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonPos1CameraShutter();
	afx_msg void OnBnClickedButtonPos2CameraShutter();
	afx_msg void OnBnClickedBtnSearchObject0MovePos0();
	afx_msg void OnBnClickedBtnSearchObject1MovePos0();
	afx_msg void OnBnClickedBtnSetDmcodeParam();
	afx_msg void OnBnClickedBtnCalibObjectPos();
	afx_msg void OnBnClickedOk();

	
};
