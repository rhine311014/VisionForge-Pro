#pragma once

#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "Label.h"
#include "LogFileSave.h"
#include "afxwin.h"

// CDlgCalcuVirtualObject4 对话框

class CDlgCalcuVirtualObject4 : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalcuVirtualObject4)

public:
	CDlgCalcuVirtualObject4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalcuVirtualObject4();

// 对话框数据
	enum { IDD = IDD_DLG_CALCU_VIRTUAL_OBJECT4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CLabel m_labelNote;

	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;

	CXPStyleButtonST	m_btnSearchObjectPos0;
	CXPStyleButtonST	m_btnSearchObjectPos1;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos0;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos1;

	CXPStyleButtonST	m_btnSearchObjectPos2;
	CXPStyleButtonST	m_btnSearchObjectPos3;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos2;
	CXPStyleButtonST	m_btnSearchVirtualObjectPos3;

	CXPStyleButtonST	m_btnCalcuVirtualObject;



public:
	
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnSearchObjectPos0();
	afx_msg void OnBnClickedBtnSearchObjectPos1();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos0();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos1();
	afx_msg void OnBnClickedBtnSearchObjectPos2();
	afx_msg void OnBnClickedBtnSearchObjectPos3();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos2();
	afx_msg void OnBnClickedBtnSearchVirtualObjectPos3();
	afx_msg void OnBnClickedBtnCalcuVirtualObject();
	afx_msg void OnBnClickedOk();


public:
	CWnd *m_pVAPrevMainWnd;	
	vcXYDVisionAlign *m_pVisionASM;

	CStringArray	*m_psaSysInfoStrings;
	CThemeHelperST		m_themeHelper;

	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	int m_nEvent;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;

	BOOL m_bIsGrabbing;
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;						// 对象display
	std::vector<scGuiDisplay*> m_vpGuiVirtualObjectDisplay;			// 虚拟对象display
	std::vector<scGuiInteractiveContainer*> m_vpGuiInteractiveContainer;	
	std::vector<scGuiInteractiveContainer*> m_vpGuiVirtualObjectInteractiveContainer;	

//	std::vector<scGuiCross*> m_vpGuiCross;			// 十字GUI
	std::vector<scGuiText*> m_vpGuiText;
	std::vector<scGuiText*> m_vpGuiVirtualObjectText;

	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnVirtualObjectImageMarkPos;

	CMarkImagePos                      m_mpObjectMarkImagePos;      // 实时对象定位结果
	CMarkImagePos                      m_mpVirtualMarkImagePos;     // 虚拟对象定位结果

	BOOL m_bCalcuVirtualObjectSuccess;
	std::vector<CCoordPos>			m_vObjectToVirtualOffsets;

public:
	static  UINT ThreadReceiveData(LPVOID pParam);     // 工作线程
	void SearchObjectPos(int nPosIndex);
	void SearchVirtualObjectPos(int nPosIndex);

	void UpdateDialogLanguage();
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void UpdateStatusBar();
	void InitDlgItem();
	void SerialCamGrab();


	afx_msg void OnBnClickedCancel();
	CComboBox m_comboExProductIndex;
	afx_msg void OnCbnSelchangeComboExproductIndex();
	int m_nPreExProductIndex;//先前的扩展产品序号
};
