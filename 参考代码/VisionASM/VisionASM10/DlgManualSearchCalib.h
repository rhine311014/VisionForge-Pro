#pragma once
#include "svGuiDisplay.h"
#include "svGuiCrossVar.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "BtnST.h"
#include "KeyBoard.h"

// CDlgCalcuVirtualObject 对话框

#define GUICROSSXYSIZE                          100;

class CDlgManualSearchCalib : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualSearchCalib)

public:
	CDlgManualSearchCalib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgManualSearchCalib();

// 对话框数据
	enum { IDD = IDD_DLG_MANUAL_SEARCH_CALIB };

	CThemeHelperST		m_themeHelper;
	CWnd *m_pVAPrevMainWnd;		
	scGuiDisplay* m_vpPrevGuiDisplays;
	vcXYDVisionAlign *m_pVisionASM;
	CStringArray	*m_psaSysInfoStrings;

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;

	BOOL m_bIsGrabbing;
	scGuiDisplay* m_vpGuiDisplay;						// 对象display
	scGuiInteractiveContainer* m_vpGuiInteractiveContainer;		
	//scGuiCross* m_vpGuiCross;			// 十字GUI

	std::vector<scGuiCoordCross*>	m_vpGuiCross;
	svGuiCrossVar m_vGuiCrossVar;


	CMarkImagePos                      m_mpMarkImagePos;      // 实时对象定位结果
	double m_dMarkX;
	double m_dMarkY;
	double m_dStepLength;
	
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


	int m_nCurPosIndex;
	int m_nMarkNum;  // iyo 需要显示mark数量，一共2个，可选择显示2个或者1个；
	int m_nCurPlatformIndex;

	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void UpdateStatusBar();
	BOOL SetGUI();

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void UpdateDialogLanguage();

	std::vector<CCoordPos> GetManualSearchResult();
	void UpdateBtnLiveGrab();
	void SetMarkType(int ntype);

	void OnUp();
	void OnDown();
	void OnLeft();
	void OnRight();
	BOOL CheckMoveStepValid();

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
	CXPStyleButtonST m_btnMoveUp;
	CXPStyleButtonST m_btnMoveDown;
	CXPStyleButtonST m_btnMoveLeft;
	CXPStyleButtonST m_btnMoveRight;
	CLabel	m_labelNote;
	afx_msg void OnBnClickedBtnSerialGrab();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	CString m_strStepLength;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocusEditStepLength();
	afx_msg LRESULT OnGuiDisplayDrapStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayDoubleClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	int m_nStepLen;

	sc2Vector vdMarkCenter;   // iyo
	int nMoveIndex;           // 移动位置序号，在多标记点里，代表同一个画面的另一个标记点；
};
