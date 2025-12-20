#pragma once
#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "KeyBoard.h"
#include "LogFileSave.h"
#include "DmCodeCalibrate.h"
// CDlgCalibTargetPos8Board 对话框

class CDlgCalibTargetPos8Board : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibTargetPos8Board)

public:
	CDlgCalibTargetPos8Board(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibTargetPos8Board();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_TARGET_POS8_BOARD };

	CThemeHelperST		m_themeHelper;
	CWnd *m_pVAPrevMainWnd;		
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;
	vcXYDVisionAlign *m_pVisionASM;
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	BOOL m_bIsGrabbing;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;						// 对象display
	std::vector<scGuiDisplay*> m_vpGuiTargetDisplay;			// 虚拟对象display
	std::vector<scGuiInteractiveContainer*> m_vpGuiInteractiveContainer;	
	std::vector<scGuiInteractiveContainer*> m_vpGuiTargetInteractiveContainer;	
	std::vector<scGuiCross*> m_vpGuiCross;			// 十字GUI
	std::vector<scGuiText*> m_vpGuiText;
	std::vector<scGuiText*> m_vpGuiTargetText;

	CMarkImagePos                      m_mpObjectMarkImagePos;      // 实时对象定位结果
	CMarkImagePos                      m_mpVirtualMarkImagePos;     // 虚拟对象定位结果

	std::vector<CMarkImagePos*>		   m_vpmpCalibMarkImagePos;
	std::vector<CMarkImagePos*>		   m_vpmpCalibMarkBoardPos;
	sc2Vector						   m_vCalibBoardSize;

	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos1;
	// 虚拟对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos1;


	std::vector<BOOL>				m_vbCalibTargetPosSuccess;
	BOOL							m_bCalibTargetPosSuccess;

	BOOL							m_bCommGetPlatformAxisPosSuccess;

	BOOL m_bCalcuVirtualObjectSuccess;
	std::vector<CCoordPos>			m_vObjectToVirtualOffsets;


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

	CBitmap m_bitMap;

	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void UpdateStatusBar();
	BOOL SetGUI();

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }

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
	void SearchTargetPos(int nPosIndex);

	void SearchObjectPos1(int nPosIndex);
	void SearchTargetPos1(int nPosIndex);

	void CommGetPlatformAxisPos();

	void SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave);
	bool CreateMultipleDirectory(CString& szPath, char split);
	bool SaveCSVFile(CString strImageFileDirt,CString strFileName,CString& strSaveData );
	bool GetCalibResultCString(CString & strCalibResult);

public:
	cpImage m_image0;
	cpImage m_image1;

protected:
	int m_nType;			// 0：目标Mark；1：对象Mark

	void InitDlgItem();
	void UpdateDlgItem(BOOL bEnable);
	void UpdateDialogLanguage();

	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	BOOL IsStringNumerical(LPCTSTR lpszVal);

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
	CXPStyleButtonST	m_btnSearchObject0Pos0;
	CXPStyleButtonST	m_btnSearchObject1Pos0;
	CXPStyleButtonST	m_btnSearchObject0Pos1;
	CXPStyleButtonST	m_btnSearchObject1Pos1;
	CXPStyleButtonST	m_btnSearchVirtualObject0Pos0;
	CXPStyleButtonST	m_btnSearchVirtualObject1Pos0;
	CXPStyleButtonST	m_btnSearchVirtualObject0Pos1;
	CXPStyleButtonST	m_btnSearchVirtualObject1Pos1;
	CXPStyleButtonST	m_btnCalcuVirtualObject;
	CXPStyleButtonST	m_btnCommGetPlatformAxisPos;
	CLabel	m_labelNote;
	afx_msg void OnBnClickedBtnSerialGrab();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSearchObject0Pos0();
	afx_msg void OnBnClickedBtnSearchObject1Pos0();
	afx_msg void OnBnClickedBtnSearchObject0Pos1();
	afx_msg void OnBnClickedBtnSearchObject1Pos1();
	afx_msg void OnBnClickedBtnSearchVirtualObject0Pos0();
	afx_msg void OnBnClickedBtnSearchVirtualObject1Pos0();
	afx_msg void OnBnClickedBtnSearchVirtualObject0Pos1();
	afx_msg void OnBnClickedBtnSearchVirtualObject1Pos1();
	afx_msg void OnBnClickedBtnSearchVirtualObject2MovePos0();
	afx_msg void OnBnClickedBtnSearchVirtualObject3MovePos0();
	afx_msg void OnBnClickedBtnCalcuVirtualObject();
	int m_nRadioCalibTargetPos;
	afx_msg void OnBnClickedBtnSearchTarget0MovePos1();
	afx_msg void OnBnClickedBtnCommGetPlatformAxisPos();

	CString m_strCalibBoardTaget5PosX0;
	CString m_strCalibBoardTaget5PosY0;
	CString m_strCalibBoardTaget5PosX1;
	CString m_strCalibBoardTaget5PosY1;
	CString m_strCalibBoardTaget6PosX0;
	CString m_strCalibBoardTaget6PosY0;
	CString m_strCalibBoardTaget6PosX1;
	CString m_strCalibBoardTaget6PosY1;
	CString m_strCalibBoardTaget7PosX0;
	CString m_strCalibBoardTaget7PosY0;
	CString m_strCalibBoardTaget7PosX1;
	CString m_strCalibBoardTaget7PosY1;
	CString m_strCalibBoardTaget8PosX0;
	CString m_strCalibBoardTaget8PosY0;
	CString m_strCalibBoardTaget8PosX1;
	CString m_strCalibBoardTaget8PosY1;
	afx_msg void OnEnSetfocusEditValCalibrationTarget5PosX0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget5PosX0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget5PosY0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget5PosY0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget5PosX1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget5PosX1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget5PosY1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget5PosY1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget6PosX0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget6PosX0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget6PosY0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget6PosY0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget6PosX1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget6PosX1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget6PosY1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget6PosY1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget7PosX0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget7PosX0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget7PosY0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget7PosY0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget7PosX1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget7PosX1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget7PosY1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget7PosY1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget8PosX0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget8PosX0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget8PosY0();
	afx_msg void OnEnKillfocusEditValCalibrationTarget8PosY0();
	afx_msg void OnEnSetfocusEditValCalibrationTarget8PosX1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget8PosX1();
	afx_msg void OnEnSetfocusEditValCalibrationTarget8PosY1();
	afx_msg void OnEnKillfocusEditValCalibrationTarget8PosY1();

	afx_msg void OnBnClickedBtnSearchTargetAll();
	CString m_strCalibBoardSizeX;
	CString m_strCalibBoardSizeY;
	afx_msg void OnEnSetfocusEditValCalibBoardSizeX();
	afx_msg void OnEnKillfocusEditValCalibBoardSizeY();
	afx_msg void OnEnKillfocusEditValCalibBoardSizeX();
	afx_msg void OnEnSetfocusEditValCalibBoardSizeY();
	afx_msg void OnBnClickedRadioCalibTargetPos0();
public:
	BOOL m_bIsDmCodeSearch;
	afx_msg void OnBnClickedBtnSetDmcodeParam();
	BOOL DmCodeSearch(int nPosIndex);
	void UpdateDmCodeSearchResult(int nPosIndex);
	CComboBox m_comboExProductIndex;

	double dShutterList[4];

	afx_msg void OnBnClickedSetCalibTargetShutter1();
	afx_msg void OnBnClickedSetCalibTargetShutter2();
	afx_msg void OnBnClickedSetCalibTargetShutter3();
	afx_msg void OnBnClickedSetCalibTargetShutter4();

	void SetCalibTargetShutter(int nPosIndex);
	void ResetCameraShutter();

	void CallDlgSearch(int nPosIndex);
	void SetCalibTargetSearchTool(int nPosIndex);
	BOOL m_bStopUpdateDisplayImage;
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
};
