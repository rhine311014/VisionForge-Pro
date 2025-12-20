#pragma once
#include "afxwin.h"
#include "Keyboard.h"
#include "vsBaseVisionAlign.h"
#include "svGuiDisplay.h"
#include "svDIB.h"
#include "svGuiCross.h"
#include "svGuiLine.h"
#include "svGuiLineSeg.h"
#include "svGuiCircle.h"
#include "svGuiEllipse.h"
#include "vsXYDVisionAlign.h"

const int ePointToPointDistance		= 0;
const int ePointToLineSegDistance	= 1;
const int ePointToLineDistance		= 2;
const int ePointToCircleDistance	= 3;
const int ePointToEllipseDistance	= 4;
const int eLineToCircleDistance		= 5;
const int eLineToEllipseDistance	= 6;
const int eCircleToCircleDistance	= 7;
const int eLineSegToLineSegDistance	= 8;
const int eLineSegToLineDistance	= 9;
const int eLineSegToCircleDistance	= 10;
const int eLineSegToEllipseDistance	= 11;

class CDlgManualMeasure : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualMeasure)

public:
	CDlgManualMeasure(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgManualMeasure();

// 对话框数据
	enum { IDD = IDD_DLG_MANUAL_MEASURE };

protected:
	HICON	m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

public:
	CThemeHelperST	m_themeHelper;
	CWnd			*m_pVAPrevMainWnd;
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplay;
	CStringArray    *m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	vcBaseVisionAlign *m_pVisionASM;




	//
	scGuiDisplay	m_pGuiDisplay;
	scGuiInteractiveContainer*	m_pInteractiveContainer;
	scGuiStaticContainer*		m_pStaticContainer;		
	cpImage		m_ImageInput;

	scGuiCross		m_GuiCross1;
	scGuiCross		m_GuiCross2;
	scGuiLineSeg	m_GuiLineSeg1;
	scGuiLineSeg	m_GuiLineSeg2;
	scGuiLine		m_GuiLine1;
	scGuiLine		m_GuiLine2;
	scGuiCircle		m_GuiCircle1;
	scGuiCircle		m_GuiCircle2;
	scGuiEllipse	m_GuiEllipse;

	scGuiLineSeg	m_GuiStaticLineSeg;
	scGuiCircle		m_GuiStaticCircle1;
	scGuiCircle		m_GuiStaticCircle2;

	// 状态栏显示
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	CStatusBarCtrl*	m_StatusBar;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	BOOL m_bResult;

	void UpdateGuiDisplay();
	void UpdateDialogLanguage();
	void UpdateBtnDisplay();
public:
	virtual void EnableSysKeyboard(BOOL bEnable){m_bSysKeyboardEnabled = bEnable;}
	virtual void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings){m_psaSysInfoStrings = psaSysInfoStrings;}
	void		 SetVisionASMRef(vcBaseVisionAlign *pVisionASM);

public:

	void InitMeasureElems(long nWidthNew,long nHeightNew);
	void UpdateMeasureElems(void);
	void UpdateDisplay(void);

	void UpdateResult(double dDistance, double dXDistance,double dYDistance);
	void UpdateResult(double dDistance,sc2Vector Vector1, double dXDistance,double dYDistance);
	void UpdateResult(double dDistance,sc2Vector Vector1,sc2Vector Vector2, double dXDistance,double dYDistance);

	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL DestroyWindow();
	void UpdateStatusBar();

	CXPStyleButtonST m_btnContinueGrab;
	CXPStyleButtonST m_btnLoadImage;
	CXPStyleButtonST m_btnSaveImage;
	CXPStyleButtonST m_btnExecute;
	int m_nRadDistanceMode;
	int m_nDisplayType;
	CString m_strInputValue;
	CString m_strCalibValue;
	CString m_strDistance;
	BOOL m_bPixelValueMode;
	afx_msg void OnBnClickedButtonContinuGrab();
	afx_msg void OnBnClickedButtonLoadImage();
	afx_msg void OnBnClickedButtonSaveImage();
	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnEnSetfocusEditInputValue();
	CComboBox m_comboDisplayImage;
	afx_msg void OnCbnSelchangeComboInputOrResultImage();
	afx_msg void OnBnClickedMeasureRadioChange();
	CString m_strActualDistance;
	int m_nCameraIndex;
	int m_nPrevCameraIndex;
	afx_msg void OnCameraChange();
	void GetCalibratedInfoToMeasure();
	afx_msg void OnBnClickedRadioInputValue();
	void ShowControl();

	CString m_strXDistance;
	CString m_strYDistance;
	CString m_strActualXDistance;
	CString m_strActualYDistance;
};
