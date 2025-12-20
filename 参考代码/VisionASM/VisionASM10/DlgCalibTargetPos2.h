#pragma once


#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "LogFileSave.h"
#include "DmCodeCalibrate.h"

// CDlgCalibTargetPos2 对话框

class CDlgCalibTargetPos2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibTargetPos2)

public:
	CDlgCalibTargetPos2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibTargetPos2();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_TARGET_POS2 };


public:
	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	CThemeHelperST		m_themeHelper;
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
    //std::vector<CDmCodeCalibrate> m_vDmCodeCalibTool;
    BOOL m_bIsDmCodeSearch;
	vcXYDVisionAlign *m_pVisionASM;
	CWnd *m_pVAPrevMainWnd;		
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplays;

	std::vector<scGuiDisplay*> m_vpGuiObjectDisplay;			// 对象display
	std::vector<scGuiDisplay*> m_vpGuiTargetDisplay;			// 目标display
	std::vector<scGuiInteractiveContainer*> m_vpGuiObjectInteractiveContainer;	
	std::vector<scGuiInteractiveContainer*> m_vpGuiTargetInteractiveContainer;	

	COLORREF m_colorMovePos1;
	COLORREF m_colorMovePos2;

	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos1;
	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos0;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos1;

	// 对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos2;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnObjectImageMarkPos3;
	// 虚拟对象结果十字
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos2;
	std::vector<scGuiCoordCross*>	m_vpGuiAlnTargetImageMarkPos3;

	std::vector<scGuiText*> m_vpGuiObjectText;
	std::vector<scGuiText*> m_vpGuiTargetText;

	std::vector<CMarkImagePos*> m_vpmpCalibMarkImagePos;

	BOOL m_bIsGrabbing;
	BOOL m_bCommGetPlatformAxisPosSuccess;
	BOOL m_bCalibTargetPosSuccess;
	std::vector<BOOL> m_vbCalibTargetPosSuccess;
	

	int m_nEvent;
	BOOL m_bExitThread;
	CWinThread* m_pThreadReceiveData;
	HANDLE m_hEventReceiveThreadExit;

	void InitDlgItem();
	void UpdateStatusBar();
	void EnableSysKeyboard(BOOL bEnable){m_bSysKeyboardEnabled = bEnable;}
 	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	void OnBnClickedBtnSerialGrab();
	void UpdateDialogLanguage();

	static  UINT ThreadReceiveData(LPVOID pParam);     // 工作线程

	void CommGetPlatformAxisPos();
	void SearchObjectPos(int nPosIndex);
	void SearchTargetPos(int nPosIndex);
	void SearchObjectPos1(int nPosIndex);
	void SearchTargetPos1(int nPosIndex);

	void SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave);
	bool CreateMultipleDirectory(CString& szPath, char split);
	bool SaveCSVFile(CString strImageFileDirt,CString strFileName,CString& strSaveData );
	bool GetCalibResultCString(CString & strCalibResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CXPStyleButtonST m_btnCommGetPlatformAxisPos;
	CXPStyleButtonST m_btnSearchObject1Pos1;
	CXPStyleButtonST m_btnSearchObject1Pos2;
	CXPStyleButtonST m_btnSearchTarget2Pos1;
	CXPStyleButtonST m_btnSearchTarget2Pos2;
	CXPStyleButtonST m_btnCalibrateTargetPos;
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST m_btnCancel;
	CLabel	m_labelNote;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCommGetPlatformAxisPos();
	afx_msg void OnBnClickedBtnSearchObject1MovePos1();
	afx_msg void OnBnClickedBtnSearchObject2MovePos2();
	afx_msg void OnBnClickedBtnSearchTarget2MovePos1();
	afx_msg void OnBnClickedBtnSearchTarget2MovePos2();
	afx_msg void OnBnClickedBtnCalibTargetPos();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	
    afx_msg void OnBnClickedBtnSetDmcodeParam();
    BOOL DmCodeSearch(int nPosIndex);
	CComboBox m_comboExProductIndex;

	CVisionASMConfig *m_pVisionASMConfig;

	double dShutterList[2];

	afx_msg void OnBnClickedSetCalibTargetShutter1();
	afx_msg void OnBnClickedSetCalibTargetShutter2();

	void SetCalibTargetShutter(int nPosIndex);
	void ResetCameraShutter();

	void CallDlgSearch(int nPosIndex);
	void SetCalibTargetSearchTool(int nPosIndex);
	BOOL m_bStopUpdateDisplayImage;
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
};
