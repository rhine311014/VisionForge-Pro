#pragma once
#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "LogFileSave.h"
// CDlgCalcuBenchPick 对话框

class CDlgCalcuBenchPick : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalcuBenchPick)

public:
	CDlgCalcuBenchPick(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalcuBenchPick();

// 对话框数据
	enum { IDD = IDD_DLG_CALCU_BENCH_PICK };


	CThemeHelperST		m_themeHelper;
	CWnd *m_pVAPrevMainWnd;		
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	vcXYDVisionAlign *m_pVisionASM;
	CStringArray	*m_psaSysInfoStrings;

	BOOL m_bIsGrabbing;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;						// 对象display
	std::vector<scGuiDisplay*> m_vpGuiVirtualObjectDisplay;			// 虚拟对象display
	std::vector<scGuiInteractiveContainer*> m_vpGuiInteractiveContainer;	
	std::vector<scGuiInteractiveContainer*> m_vpGuiVirtualObjectInteractiveContainer;	
	std::vector<scGuiCross*> m_vpGuiCross;			// 十字GUI
	std::vector<scGuiText*> m_vpGuiText;
	std::vector<scGuiText*> m_vpGuiVirtualObjectText;

	CMarkImagePos                      m_mpObjectMarkImagePos;      // 实时对象定位结果
	CMarkImagePos                      m_mpVirtualMarkImagePos;     // 虚拟对象定位结果

	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos;
	// 虚拟对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnVirtualObjectImageMarkPos;

	BOOL m_bCalcuVirtualObjectSuccess;
	std::vector<CCoordPos>			m_vObjectToVirtualOffsets;

	BOOL							m_bCommGetBenchPickPlatformAxisPosSuccess;

	BOOL m_bDisplayCross;

	std::vector<CCoordPos> m_vMarkCorrdPos;
	std::vector<double> m_vdPreShutter;
	std::vector<PTTriggerMode> m_vPrevTriggerMode;
	std::vector<BOOL> m_vPrevGrabStatus;

	CSliderCtrl	m_sliderShutter0;
	CSliderCtrl	m_sliderShutter1;

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void UpdateStatusBar();
	BOOL SetGUI();

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	bool UpdateGuiDisplay();

	std::vector<CCoordPos> GetManualSearchResult();
	void UpdateBtnLiveGrab();
	void UpdateShutterDisplay();


	int m_nEvent;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;

	static  UINT ThreadReceiveData(LPVOID pParam);     // 工作线程

	void SearchObjectPos(int nPosIndex);
	void SearchVirtualObjectPos(int nPosIndex);


	void CommGetBenchPickPlatformAxisPos();

public:
	cpImage m_image0;
	cpImage m_image1;

protected:
	int m_nType;			// 0：目标Mark；1：对象Mark

	void InitDlgItem();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnGrabImage;
	CXPStyleButtonST	m_btnSearchObjectPos0;
	CXPStyleButtonST	m_btnSearchObjectPos1;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos0;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos1;
	CXPStyleButtonST	m_btnCalcuVirtualObject;
	CLabel	m_labelNote;
	afx_msg void OnBnClickedBtnSerialGrab();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSearchObjectPos0();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos0();
	afx_msg void OnBnClickedBtnSearchObjectPos1();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos1();
	afx_msg void OnBnClickedBtnCalcuVirtualObject();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nRadio;
	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRadio();
	CComboBox m_comboExProductIndex;

	int m_nCurRegTriggerIndex;
};
