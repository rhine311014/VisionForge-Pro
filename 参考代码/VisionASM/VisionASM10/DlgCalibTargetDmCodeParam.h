// CDlgCalibTargetDmcodeParam 对话框

#pragma once
#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "KeyBoard.h"
#include "svGuiCoordCross.h"
#include "svGuiRect.h"
//#include "DlgSimplePositionSelect.h"

// CDlgCalibTargetDmcodeParam 对话框
#define MAXDMCODENUM 8
class CDlgCalibTargetDmcodeParam : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibTargetDmcodeParam)

public:
	CDlgCalibTargetDmcodeParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibTargetDmcodeParam();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_TARGET_DMCODE_PARAM };
protected:
	
	CWnd *m_pVAPrevMainWnd;		
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplay;

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	// 状态栏显示
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	CStatusBarCtrl*	m_StatusBar;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	bool m_bIsBoardCalib;

	scGuiDisplay	m_pGuiDisplay;
	CString m_strKeyText;
	scGuiInteractiveContainer*	m_pInteractiveContainer;
	scGuiStaticContainer*		m_pStaticContainer;	

	scGuiRect m_GuiDmCodeSearchRect;
	scGuiRect m_GuiCornersSearchRect;

	CGuiGraphics      m_GuiCalibrateGraphics;
	CDmCodeCalibrate   m_DmCodeCalibTool;

	std::vector<CRITICAL_SECTION>      m_vcsResource;
	std::vector<cpImage>		m_vImageInput;

	std::vector<CButton*>		m_vpCheckPos;

protected:
	void UpdateGuiDisplay();
	void UpdateTrainGui(bool bShow);
	void UpdateResultGui(bool bShow);
	void UpdateDlgData(bool bSave);
	void UpdateDialogLanguage();
	void UpdateBtnDisplay();
	void CreateGUID(CString &strGUID);
	void CopyParameter(int nSrcPosIndex,int nDstPosIndex);
public:
	virtual void EnableSysKeyboard(BOOL bEnable){m_bSysKeyboardEnabled = bEnable;}
	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM);
	void UpdateStatusBar();
//	
	void SetIsBoardCalibrate(bool bBoardCalib);
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
//
	void InitDlgItem();
	void AddGuiResultGraphicsToDisplay(scGuiDisplay* pGuiDisplay);
	void AddGuiTrainGraphicsToDisplay(scGuiDisplay* pGuiDisplay);

	bool SetGuiByImage(double nWidth,double nHeight);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void PostMessageUpdateGuiCalibrateTargetGraphics(BOOL bShow);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	CString m_strCalibDMCodeMaxTime;
	CString m_strCalibDMCodeMaxNum;
//	
	vcBaseVisionAlign *m_pVisionASM;
	int m_LogLevel;
	void SetLogLevel(int level);
	afx_msg void OnEnSetfocusEditValDMCodeMaxNum();
	afx_msg void OnEnKillfocusEditValDMCodeMaxNum();
	afx_msg void OnEnSetfocusEditValDMCodeMaxTime();
	afx_msg void OnEnKillfocusEditValDMCodeMaxTime();

	CString m_strCalibDMCodeGridThre;
	CString m_strCalibDMCodeGridSize;
	afx_msg void OnEnSetfocusEditValDMCodeGridSize();
	afx_msg void OnEnKillfocusEditValCodeGridSize();
	afx_msg void OnEnSetfocusEditValDMCodeGridThre();
	afx_msg void OnEnKillfocusEditValCodeGridThre();

	
	BOOL m_bDmcodeOutputPoint;
	BOOL m_bDmcodeRecode;

	scDMCodeParam m_vDmCodeParam[MAXDMCODENUM];
	scDMCodeParam m_vtmpDmCodeParam[MAXDMCODENUM];
	double m_dEnvironmentShutter[MAXDMCODENUM];

	afx_msg void OnBnClickedOk();
    int m_nPosIndex;
    int m_nPrePosIndex;
    afx_msg void OnBnClickedRdoPos1();
    int m_nPosNum;
    void OnDmcodemirrorChange();
	void OnDeCodeMethodChange();
    afx_msg void OnBnClickedCheckDmcoderedecode();
    afx_msg void OnBnClickedCheckDmcodeoutpoint();
    void OnPosChange();
    afx_msg void OnBnClickedRdoPos2();
    afx_msg void OnBnClickedRdoPos3();
    afx_msg void OnBnClickedRdoPos4();
    afx_msg void OnBnClickedRdoPos5();
    afx_msg void OnBnClickedRdoPos6();
    afx_msg void OnBnClickedRdoPos7();
    afx_msg void OnBnClickedRdoPos8();
	virtual void OnCancel();
	CComboBox m_comboDisplayImage;
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnGrabImage();
	afx_msg void OnBnClickedBtnLoadImage();
	afx_msg void OnCbnSelchangeComboImageType();

	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnDmcodeFullImage();
	afx_msg void OnBnClickedBtnCornersFullImage();
	afx_msg void OnBnClickedBtnShutterSetting();
	int m_nDecodeMethod;
	int m_nDmCodeMirror;
	CString m_strSampleLevel;
	CString m_strMinBarSize;
	CString m_strMaxBarSize;
	afx_msg void OnBnClickedRdoDmcodeMethodDefault();
	afx_msg void OnBnClickedRdoDecodeMethodVsb();
	afx_msg void OnBnClickedRdoMirrorModeClose();
	afx_msg void OnBnClickedRdoMirrorModeOpen();
	afx_msg void OnBnClickedRdoMirrorModeAny();
	afx_msg void OnEnSetfocusEditSampleLevel();
	afx_msg void OnEnKillfocusEditSampleLevel();
	afx_msg void OnEnSetfocusEditMinBarSize();
	afx_msg void OnEnKillfocusEditMinBarSize();
	afx_msg void OnEnSetfocusEditMaxBarSize();
	afx_msg void OnEnKillfocusEditMaxBarSize();
	CComboBox m_cmbCopySrc;
	CButton m_chkDstCopyPos1;
	CButton m_chkDstCopyPos2;
	CButton m_chkDstCopyPos3;
	CButton m_chkDstCopyPos4;
	CButton m_chkDstCopyPos5;
	CButton m_chkDstCopyPos6;
	CButton m_chkDstCopyPos7;
	CButton m_chkDstCopyPos8;
	afx_msg void OnBnClickedButtonCopy();
};


