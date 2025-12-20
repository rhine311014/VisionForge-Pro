// DlgSysKindAlignCalibTarget.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindAlignCalibTarget.h"
#include "afxdialogex.h"


#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"

#include "DlgSysKind.h"
#include "DlgSearchToolCopyPos.h"

#include "DlgCalibTargetDmCodeParam.h"
#include "DlgSearchToolModelLibrary.h"

// CDlgSysKindAlignCalibTarget 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindAlignCalibTarget, CDialog)

	CDlgSysKindAlignCalibTarget::CDlgSysKindAlignCalibTarget(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindAlignCalibTarget::IDD, pParent)
	//, m_nVirtualObjectCandidateEnable(0)
	, m_nCalibTargetMarkEnable(0)
	, m_nObjectCandidateEnable(0)
	, m_nObjectCandidateSync(0)
	, m_bObjectCandidateOffsetEnable1(FALSE)
	, m_bObjectCandidateOffsetEnable2(FALSE)
	, m_bObjectCandidateOffsetEnable3(FALSE)
	, m_bObjectCandidateOffsetEnable4(FALSE)
	, m_bEnableOutLine(FALSE)
	, m_nOutLinePercent(5)
{
	//{{AFX_DATA_INIT(CDlgSysKindAlignCalibTarget)
	m_strAlignerOffsetD = _T("0.000000");
	m_strAlignerOffsetX = _T("0.000000");
	m_strAlignerOffsetY = _T("0.000000");
	m_strAlignerPrecisionX = _T("0.010000");
	m_strAlignerPrecisionY = _T("0.010000");
	m_strAlignerPrecisionD = _T("0.010000");
	m_strObjectCandidateOffsetD1 = _T("0.000000");
	m_strObjectCandidateOffsetX1 = _T("0.000000");
	m_strObjectCandidateOffsetY1 = _T("0.000000");
	m_strObjectCandidateOffsetD2 = _T("0.000000");
	m_strObjectCandidateOffsetX2 = _T("0.000000");
	m_strObjectCandidateOffsetY2 = _T("0.000000");
	m_strObjectCandidateOffsetD3 = _T("0.000000");
	m_strObjectCandidateOffsetX3 = _T("0.000000");
	m_strObjectCandidateOffsetY3 = _T("0.000000");
	m_strObjectCandidateOffsetD4 = _T("0.000000");
	m_strObjectCandidateOffsetX4 = _T("0.000000");
	m_strObjectCandidateOffsetY4 = _T("0.000000");
	m_nAlignerMaxTimes = 1;
	m_nPlatformIndex = 0;
	m_strCurProductName = _T("");
	//}}AFX_DATA_INIT
	// 	m_pStatusBar = NULL;
	// 	m_bStatusBarFlag = TRUE;
	// 	m_hIconSucceed = NULL;
	// 	m_hIconFailed = NULL;

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;
	m_nPrevProductIdx = 0;
	m_nCurProductIdx = 0;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_strProductNameOld.Empty();

	m_bCopyObjectToCalib = FALSE;


	m_bObjectShutterEnable0 = FALSE;
	m_bObjectShutterEnable1 = FALSE;
	m_bObjectShutterEnable2 = FALSE;
	m_bObjectShutterEnable3 = FALSE;
	m_bObjectShutterEnable4 = FALSE;

	m_bObjectGainEnable0 = FALSE;
	m_bObjectGainEnable1 = FALSE;


	m_bObjectGammaEnable0 = FALSE;
	m_bObjectGammaEnable1 = FALSE;


	//	m_pParent = (CVisionASMDlg *)pParent;
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalProductData.push_back(pTotalProduct);
	// 	}


}

CDlgSysKindAlignCalibTarget::~CDlgSysKindAlignCalibTarget()
{
	DestroyWindow();
}

void CDlgSysKindAlignCalibTarget::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysKindAlignCalibTarget)
	//	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	//DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrlProduct);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	// 	DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT, m_btnNewProduct);
	// 	DDX_Control(pDX, IDC_BUTTON_DELETE_PRODUCT, m_btnDeleteProduct);
	// 	DDX_Control(pDX, IDC_BTN_PRODUCT_RENAME, m_btnRename);
	// 	DDX_Control(pDX, IDC_BTN_AMEND_BENCHMARK, m_btnAmendBenchMark);
	// 	DDX_Control(pDX, IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, m_comboAlignerOffsetCoordType);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	// 	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	// 	DDX_Text(pDX, IDC_EDIT_ALIGNER_MAX_TIMES, m_nAlignerMaxTimes);
	// 	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME, m_strCurProductName);
	//}}AFX_DATA_MAP
	//DDX_Radio(pDX, IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0, m_nVirtualObjectCandidateEnable);
	DDX_Radio(pDX, IDC_RADIO_CALIB_TARGET_MARK_ENALBE0, m_nCalibTargetMarkEnable);
	DDX_Control(pDX, IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE, m_comboCalibTargetMarkPatternMode);
	DDX_Radio(pDX, IDC_RADIO_OBJECTC_SEARCH_MODE0, m_nObjectCandidateEnable);
	DDX_Radio(pDX, IDC_RADIO_OBJECTC_SEARCH_SYNC0, m_nObjectCandidateSync);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, m_strObjectCandidateOffsetD1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, m_strObjectCandidateOffsetX1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, m_strObjectCandidateOffsetY1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, m_strObjectCandidateOffsetD2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, m_strObjectCandidateOffsetX2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, m_strObjectCandidateOffsetY2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, m_strObjectCandidateOffsetD3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, m_strObjectCandidateOffsetX3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, m_strObjectCandidateOffsetY3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, m_strObjectCandidateOffsetD4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, m_strObjectCandidateOffsetX4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, m_strObjectCandidateOffsetY4);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE1, m_bObjectCandidateOffsetEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE2, m_bObjectCandidateOffsetEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE3, m_bObjectCandidateOffsetEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE4, m_bObjectCandidateOffsetEnable4);
	DDX_Control(pDX, IDC_STATIC_POS0, m_labelPos0);
	DDX_Control(pDX, IDC_STATIC_POS1, m_labelPos1);
	DDX_Control(pDX, IDC_STATIC_POS2, m_labelPos2);
	DDX_Control(pDX, IDC_STATIC_POS3, m_labelPos3);
	DDX_Control(pDX, IDC_STATIC_POS4, m_labelPos4);
	DDX_Control(pDX, IDC_STATIC_POS5, m_labelPos5);
	DDX_Control(pDX, IDC_STATIC_POS6, m_labelPos6);
	DDX_Control(pDX, IDC_STATIC_POS7, m_labelPos7);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB0, m_btnCopyObjectToCalib0);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB1, m_btnCopyObjectToCalib1);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB2, m_btnCopyObjectToCalib2);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB3, m_btnCopyObjectToCalib3);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB4, m_btnCopyObjectToCalib4);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB5, m_btnCopyObjectToCalib5);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB6, m_btnCopyObjectToCalib6);
	DDX_Control(pDX, IDC_BTN_COPY_OBJECT_TO_CALIB7, m_btnCopyObjectToCalib7);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE,  m_bObjectShutterEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE1, m_bObjectShutterEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2, m_bObjectShutterEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3, m_bObjectShutterEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4, m_bObjectShutterEnable4);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE,  m_bObjectGainEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1, m_bObjectGainEnable1);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE,  m_bObjectGammaEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1, m_bObjectGammaEnable1);

	DDX_Check(pDX, IDC_CHK_ENABLE_OUTLINE, m_bEnableOutLine);
	DDX_Text(pDX, IDC_EDIT_OUTLINE_PERCENT, m_nOutLinePercent);
}


BEGIN_MESSAGE_MAP(CDlgSysKindAlignCalibTarget, CDialog)
	//{{AFX_MSG_MAP(CDlgSysKindAlignCalibTarget)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM1, OnGuiDisplayMenuCustom1)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM2, OnGuiDisplayMenuCustom2)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM3, OnGuiDisplayMenuCustom3)
	// 	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME, OnBtnProductRename)
	// 	ON_BN_CLICKED(IDC_BTN_AMEND_BENCHMARK, OnBtnAmendBenchmark)
	//	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT, OnButtonNewProduct)
	// 	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRODUCT, OnButtonDeleteCurProduct)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnSetfocusEditAlignerMaxTimes)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnSetfocusEditAlignerXPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnSetfocusEditAlignerYPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnSetfocusEditAlignerDPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnSetfocusEditAlignerDOffset)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnSetfocusEditAlignerXOffset)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnSetfocusEditAlignerYOffset)
	// 	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduct)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnKillfocusEditAlignerXPrecision)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnKillfocusEditAlignerYPrecision)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnKillfocusEditAlignerDPrecision)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnKillfocusEditAlignerDOffset)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnKillfocusEditAlignerXOffset)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnKillfocusEditAlignerYOffset)
	// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnKillfocusEditAlignerMaxTimes)
	//	ON_WM_CLOSE()
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
	// 	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnClickListProduct)
	// 	ON_BN_CLICKED(IDC_BTN_CLONE_PRODUCT, OnBtnCloneProduct)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, OnSetfocusEditObjectCandidateDOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, OnSetfocusEditObjectCandidateXOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, OnSetfocusEditObjectCandidateYOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, OnSetfocusEditObjectCandidateDOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, OnSetfocusEditObjectCandidateXOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, OnSetfocusEditObjectCandidateYOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, OnSetfocusEditObjectCandidateDOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, OnSetfocusEditObjectCandidateXOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, OnSetfocusEditObjectCandidateYOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, OnSetfocusEditObjectCandidateDOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, OnSetfocusEditObjectCandidateXOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, OnSetfocusEditObjectCandidateYOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, OnKillfocusEditObjectCandidateDOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, OnKillfocusEditObjectCandidateXOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, OnKillfocusEditObjectCandidateYOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, OnKillfocusEditObjectCandidateDOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, OnKillfocusEditObjectCandidateXOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, OnKillfocusEditObjectCandidateYOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, OnKillfocusEditObjectCandidateDOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, OnKillfocusEditObjectCandidateXOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, OnKillfocusEditObjectCandidateYOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, OnKillfocusEditObjectCandidateDOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, OnKillfocusEditObjectCandidateXOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, OnKillfocusEditObjectCandidateYOffset4)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB0, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib0)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB1, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib1)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB2, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib2)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB3, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib3)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB4, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib4)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB5, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib5)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB6, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib6)
	ON_BN_CLICKED(IDC_BTN_COPY_OBJECT_TO_CALIB7, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib7)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE1, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE2, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE3, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE4, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGainEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGammaEnable)

	ON_BN_CLICKED(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioCalibTargetMarkChange)
	ON_BN_CLICKED(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioCalibTargetMarkChange)
	ON_BN_CLICKED(IDC_RADIO_OBJECTC_SEARCH_MODE0, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchModeChange)
	ON_BN_CLICKED(IDC_RADIO_OBJECTC_SEARCH_MODE1, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchModeChange)
	ON_BN_CLICKED(IDC_RADIO_OBJECTC_SEARCH_SYNC0, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchSyncChange)
	ON_BN_CLICKED(IDC_RADIO_OBJECTC_SEARCH_SYNC1, &CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchSyncChange)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE, &CDlgSysKindAlignCalibTarget::OnCbnSelchangeComboCalibTargetMarkPatternMode)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE1, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable1)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE2, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable2)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE3, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable3)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_OBJECTC_OFFSET_ENABLE4, &CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable4)

	ON_BN_CLICKED(IDC_BTN_DMCODE_PARAM_SETTING, &CDlgSysKindAlignCalibTarget::OnBnClickedBtnDmcodeParamSetting)
	ON_BN_CLICKED(IDC_CHK_ENABLE_OUTLINE, &CDlgSysKindAlignCalibTarget::OnBnClickedChkEnableOutline)
	ON_EN_SETFOCUS(IDC_EDIT_OUTLINE_PERCENT, &CDlgSysKindAlignCalibTarget::OnEnSetfocusEditOutlinePercent)
	ON_EN_KILLFOCUS(IDC_EDIT_OUTLINE_PERCENT, &CDlgSysKindAlignCalibTarget::OnEnKillfocusEditOutlinePercent)
END_MESSAGE_MAP()


// CDlgSysKindAlignCalibTarget 消息处理程序


BOOL CDlgSysKindAlignCalibTarget::OnInitDialog()
{
	CDialog::OnInitDialog();
	//	m_bSysKeyboardEnabled = FALSE;
	// TODO:  在此添加额外的初始化
	// 
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalProductData.push_back(pTotalProduct);
	// 	}
	// 
	// 
	// 
	// 	// TODO:  在此添加额外的初始化
	// 	CString strModuelDir;
	// 	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
	// 	strModuelDir.ReleaseBuffer();
	// 	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   

	m_strModuleDir = m_pParentKind->m_strModuleDir;

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	int nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_strProductNameOld = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);

	UpdateDialogLanguage();
	InitDlgItem();
	
	UpdateDlgData(FALSE);
	UpdateProductListDisplay();

	m_comboCalibTargetMarkPatternMode.GetWindowText(strCalibTargetMarkPatternModeOld);
	// 
	// 	int nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlgSysKindAlignCalibTarget::InitDlgItem()
{
	int i = 0, j = 0;

	m_comboCalibTargetMarkPatternMode.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_SINGLE_PATTERN_SINGLE_MARK));
	m_comboCalibTargetMarkPatternMode.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_DOUBLE_PATTERN_SINGLE_MARK));
	m_comboCalibTargetMarkPatternMode.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_SINGLE_PATTERN_MULTI_MARK));
	m_comboCalibTargetMarkPatternMode.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_SINGLE_FOUR_POINTS));
	m_comboCalibTargetMarkPatternMode.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_DM_CODE));

	// 
	m_vpLabelPos.push_back(&m_labelPos0);
	m_vpLabelPos.push_back(&m_labelPos1);
	m_vpLabelPos.push_back(&m_labelPos2);
	m_vpLabelPos.push_back(&m_labelPos3);
	m_vpLabelPos.push_back(&m_labelPos4);
	m_vpLabelPos.push_back(&m_labelPos5);
	m_vpLabelPos.push_back(&m_labelPos6);
	m_vpLabelPos.push_back(&m_labelPos7);

	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib0);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib1);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib2);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib3);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib4);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib5);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib6);
	m_vpBtnCopyObjectToCalib.push_back(&m_btnCopyObjectToCalib7);

	// 设置按钮
	m_btnCopyObjectToCalib0.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib0.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib0.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCopyObjectToCalib1.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib1.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib1.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCopyObjectToCalib2.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib2.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCopyObjectToCalib3.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib3.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCopyObjectToCalib4.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib4.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib4.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnCopyObjectToCalib5.SetThemeHelper(&m_themeHelper);
	// 	m_btnCopyObjectToCalib5.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnCopyObjectToCalib5.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_btnCopyObjectToCalib6.SetThemeHelper(&m_themeHelper);
	m_btnCopyObjectToCalib7.SetThemeHelper(&m_themeHelper);


	// 调整控制板
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	// 创建scGUIDisplay
	//	m_vpTargetGuiDisplay.clear();
	//	m_vpObjectGuiDisplay.clear();
	//	m_vpVirtualObjectGuiDisplay.clear();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	m_vvpObjectGuiDisplay.resize(MAXPOSNUM);
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<MAXPOSNUM; i++)
	{
		for (j = 0; j < OBJECTCANDIDATENUM; j++)
		{
			pGUIDisplay = new scGuiDisplay;
			pGUIDisplay->Create(IDC_STATIC_OBJECTC_DISPLAY0+ i*OBJECTCANDIDATENUM + j, this, WS_BORDER);
			pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CLICK | SV_DISPLAY_CUSTOM1 | SV_DISPLAY_CUSTOM2 | SV_DISPLAY_CUSTOM3);
			switch (lgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				pGUIDisplay->SetLanguage(0);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("Clear Search Tool"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("Copy Search Tool To ..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("Get Search Tool From Model library"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			}
			m_vvpObjectGuiDisplay[i].push_back(pGUIDisplay);
			pGUIDisplay = NULL;
		}
	}


	// 	// 动态计算并调整对话框大小
	int nMaxCamNum = 0;
	int nClientWidth = 0, nClientHeight = 0;
	int nDispInterval = 0, nGRPIndent = 6;
	int nFrame = 0;
	int nStatusBarHeight = 0;	// 状态栏高度
	int nListWidth = 30;
	CRect rcGRP, rcItem1, rcItem2;

	int nPosNum = 0;
	BOOL bVirtualObjUsed = FALSE;
	for (i=0; i<nPlatformNum; i++)
	{
		nPosNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
		SysPlatformInfo platformInfo;
		m_pParent->m_vpVisionASM.at(i)->GetSysPlatformInfo(platformInfo);
		//if (nPosNum == 1)	// 单相机时，以拍摄位置数为模板数
		{
			nPosNum = platformInfo.m_nPositionNum;
		}
		nMaxCamNum = max(nMaxCamNum, nPosNum);
		bVirtualObjUsed |= (platformInfo.m_eAlignerObjectType == eObjectVirtual);
	}

	CRect rcClient;
	GetClientRect(rcClient);
	for(i = 0; i < 8; i++)
	{
		GetDlgItem(IDC_STATIC_SEPARATOR2+i)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.top = rcClient.top;
		rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_STATIC_SEPARATOR2+i)->MoveWindow(rcItem1);
	}



	//////////////////////////////////////////////////////////////////////////
	// 根据默认平台调整界面
	SysPlatformInfo platformInfo;
	//	int nPosNum = 0, i = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
	
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;
	// 	// 目标Mark界面修改
	// 	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	// 	{
	// 		for (i=0; i<6; i++)
	// 		{
	// 			if (i < nPosNum)
	// 			{
	// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
	// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
	// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 			}
	// 			else
	// 			{
	// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
	// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
	// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 	}
	// 	else
	// 	{
	// 		for (i=0; i<6; i++)
	// 		{
	// 			GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
	// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 		if (platformInfo.m_bBenchMarkAmend)
	// 		{
	// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 		}
	// 		else
	// 		{
	// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 		}
	// 	}
	// 
	// 	// 虚拟对象Mark界面修改
	// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	// 	{
	// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
	// 		for (i=0; i<6; i++)
	// 		{
	// 			if (i < nPosNum)
	// 			{
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 			}
	// 			else
	// 			{
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
	// 		for (i=0; i<6; i++)
	// 		{
	// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 	}

	// 	// 对象Mark界面修改
	// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	// 	{
	// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
	// 		for (i=0; i<MAXPOSNUM; i++)
	// 		{
	// 			if (i < nPosNum)
	// 			{
	// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 
	// 				if (i < MAXPOSNUM -1)
	// 				{
	// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
	// 				}
	// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
	// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
	// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
	// 
	// 
	// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
	// 				{
	// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
	// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
	// 				}
	// 			}
	// 			else
	// 			{
	// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 
	// 				if (i < MAXPOSNUM -1)
	// 				{
	// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
	// 				}
	// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
	// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
	// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
	// 
	// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
	// 				{
	// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
	// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
	// 				}
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
	// 		for (i=0; i<MAXPOSNUM; i++)
	// 		{
	// 			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
	// 			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 
	// 			if (i < MAXPOSNUM -1)
	// 			{
	// 				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
	// 			}
	// 			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
	// 			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
	// 			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
	// 
	// 
	// 			for(j = 0; j < OBJECTCANDIDATENUM; j++)
	// 			{
	// 				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
	// 				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 	}
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
	// 	for (i=0; i<6; i++)
	// 	{
	// 		if (i < nPosNum)
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 		}
	// 		else
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 	}


// 	// 虚拟对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);
// 
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 
// 				for(j = 0; j < VIRTUALOBJECTCANDIDATENUM; j++)
// 				{
// 					//GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(FALSE);
// 
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			if (i < MAXPOSNUM -1)
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 			}
// 			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 
// 			for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}


	// 关联标定Mark界面修改
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
	{
// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);
		
		EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);


		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);

		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}

				for(j = 0; j < ALNCALIBTARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				}
			}
			else
			{
				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
	{

		EnableAndShowControl(FALSE,FALSE);

		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);

		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
		for (i=0; i<MAXPOSNUM; i++)
		{
			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);


			for(j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
	}

	// 	m_nPrevPlatformIdx = m_nPlatformIndex;
	// 	m_nCurProductIdx = m_comboCurProduct.GetCurSel();
	// 	m_nPrevProductIdx = m_nCurProductIdx;

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignCalibTarget::EnableAndShowControl(bool bEnable,bool bShow)
{
	bool bShowTemplateSet = false;
	bool bEnableTemplateSet = false;
	if(NULL != m_pParentKind)
	{
		bShowTemplateSet = m_pParentKind->m_bShowCalibTargetTemplateSet;
		bEnableTemplateSet = m_pParentKind->m_bEnableCalibTargetTemplateSet;
	}
	bEnable = bEnable && bEnableTemplateSet;
	bShow = bShow && bShowTemplateSet;

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_SHUTTER_ENABLE1)->ShowWindow(bShow);

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAIN_ENABLE1)->ShowWindow(bShow);

	//暂时禁用Gamma实时拍照切换
	{
		bEnable = false;
		bShow = false;
	}

	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_OBJECTC_GAMMA_ENABLE1)->ShowWindow(bShow);

}

void CDlgSysKindAlignCalibTarget::MoveDlgItem()
{
	if (GetDlgItem(IDC_STATIC_POS0)->GetSafeHwnd() != NULL)
	{
		int i = 0, j = 0;

		// 调整控制板
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

		// 创建scGUIDisplay
		//	m_vpTargetGuiDisplay.clear();
		//	m_vpObjectGuiDisplay.clear();
		//	m_vpVirtualObjectGuiDisplay.clear();

		// 	// 动态计算并调整对话框大小
		int nMaxCamNum = 0;
		int nClientWidth = 0, nClientHeight = 0;
		int nDispInterval = 0, nGRPIndent = 6;
		int nFrame = 0;
		int nStatusBarHeight = 0;	// 状态栏高度
		int nListWidth = 30;
		CRect rcGRP, rcItem1, rcItem2;

		int nPosNum = 0;
		BOOL bVirtualObjUsed = FALSE;
		for (i=0; i<nPlatformNum; i++)
		{
			nPosNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
			SysPlatformInfo platformInfo;
			m_pParent->m_vpVisionASM.at(i)->GetSysPlatformInfo(platformInfo);
			//if (nPosNum == 1)	// 单相机时，以拍摄位置数为模板数
			{
				nPosNum = platformInfo.m_nPositionNum;
			}
			nMaxCamNum = max(nMaxCamNum, nPosNum);
			bVirtualObjUsed |= (platformInfo.m_eAlignerObjectType == eObjectVirtual);
		}

		CRect rcClient;
		GetClientRect(rcClient);
		for(i = 0; i < 8; i++)
		{
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			rcItem1.top = rcClient.top;
			rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->MoveWindow(rcItem1);
		}


		//////////////////////////////////////////////////////////////////////////
		// 根据默认平台调整界面
		SysPlatformInfo platformInfo;
		//	int nPosNum = 0, i = 0;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
		CPlatformOptionInfo optionInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
//		nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		nPosNum = platformInfo.m_nPositionNum;
		// 		// 对象Mark界面修改
		// 		if (platformInfo.m_eAlignerObjectType != eObjectBench)
		// 		{
		// 			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
		// 			for (i=0; i<MAXPOSNUM; i++)
		// 			{
		// 				if (i < nPosNum)
		// 				{
		// 					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
		// 					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
		// 					//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
		// 
		// 					if (i < MAXPOSNUM -1)
		// 					{
		// 						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
		// 					}
		// 					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
		// 					m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
		// 					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
		// 
		// 
		// 					for(j = 0; j < OBJECTCANDIDATENUM; j++)
		// 					{
		// 						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
		// 						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
		// 					}
		// 				}
		// 				else
		// 				{
		// 					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
		// 					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
		// 					// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
		// 
		// 					if (i < MAXPOSNUM -1)
		// 					{
		// 						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
		// 					}
		// 					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
		// 					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
		// 					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
		// 
		// 					for(j = 0; j < OBJECTCANDIDATENUM; j++)
		// 					{
		// 						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
		// 						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
		// 					}
		// 				}
		// 			}
		// 		}
		// 		else
		// 		{
		// 			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
		// 			for (i=0; i<MAXPOSNUM; i++)
		// 			{
		// 				// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
		// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
		// 				// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
		// 
		// 				if (i < MAXPOSNUM -1)
		// 				{
		// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
		// 				}
		// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
		// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
		// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
		// 
		// 
		// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
		// 				{
		// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
		// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
		// 				}
		// 			}
		// 		}




// 		// 虚拟对象Mark界面修改
// 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 		{
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);
// 
// 			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 			for (i=0; i<MAXPOSNUM; i++)
// 			{
// 				if (i < nPosNum)
// 				{
// 					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 					//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 					if (i < MAXPOSNUM -1)
// 					{
// 						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
// 					}
// 					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 					m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
// 
// 					for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 					{
// 						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 					}
// 					for(j = 0; j < VIRTUALOBJECTCANDIDATENUM; j++)
// 					{
// 						//GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
// 						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 					}
// 				}
// 				else
// 				{
// 					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 					// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 					if (i < MAXPOSNUM -1)
// 					{
// 						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 					}
// 					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 					for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 					{
// 						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(FALSE);
// 
// 			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 			for (i=0; i<MAXPOSNUM; i++)
// 			{
// 				// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 				// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}

		// 关联标定Mark界面修改
		if (platformInfo.m_bTargetObjectCamSeparate == TRUE|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
		{
			// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
			// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
			// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);

			EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);

			GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);

			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
			for (i=0; i<MAXPOSNUM; i++)
			{
				if (i < nPosNum)
				{
					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
					m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);

					for(j = 0; j < OBJECTCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}

					for(j = 0; j < ALNCALIBTARGETCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
						m_vvpObjectGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					}
				}
				else
				{
					// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
					// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
					m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);

					for(j = 0; j < OBJECTCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}
				}
			}
		}
		else
		{

			EnableAndShowControl(FALSE,FALSE);

			GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);

			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
			for (i=0; i<MAXPOSNUM; i++)
			{
				// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);


				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignCalibTarget::CheckDlgData()
{
	UpdateData(TRUE);

	// 对位次数
	m_nAlignerMaxTimes  = (m_nAlignerMaxTimes > 10) ? 10 : ((m_nAlignerMaxTimes < 1) ? 1 : m_nAlignerMaxTimes);
}

void CDlgSysKindAlignCalibTarget::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	UpdateData(TRUE);

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CProductData product;
	if (bSaveAndValidate)
	{
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		product = pTotalProduct->GetItem(nCurProdIdx);
		product.m_pAlignerParam->SetCalibTargetMarkEnable(m_nCalibTargetMarkEnable);
		product.m_pAlignerParam->SetCalibTargetMarkPatternMode((CalibTargetMarkPatternMode)m_comboCalibTargetMarkPatternMode.GetCurSel());
		product.m_pAlignerParam->SetCalibTargetDMCodeOutLineEnable(m_bEnableOutLine);
		product.m_pAlignerParam->SetCalibTargetDMCodeOutLinePercent((double)m_nOutLinePercent/100);

		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
		product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(0, cameraParamEnable);

		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;;
		product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(1, cameraParamEnable);


//		product.m_pAlignerParam->SetVirtualObjectMarkEnable(m_nVirtualObjectCandidateEnable);
// 		product.m_pAlignerParam->SetObjectCandidateEnable(m_nObjectCandidateEnable);
// 		product.m_pAlignerParam->SetObjectCandidateSyncEnable(m_nObjectCandidateSync);
// 
// 		CCoordPos pos;		
// 		int i=0; 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX1);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX1);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX1);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable1);
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX2);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX2);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX2);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable2);
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX3);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX3);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX3);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable3);
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX4);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX4);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX4);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable4);
		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else
	{
		BeginWaitCursor();
		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
		product = pTotalProduct->GetItem(nCurProdIdx);

		m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;
		// 
		// 		m_nCurProductIdx = nCurProdIdx;
		// 		m_nPrevProductIdx = m_nCurProductIdx;
		// 		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
		// 		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
		// 		m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
		// 		m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
		// 		m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());
		// 		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
		// 		pos = product.m_pAlignerParam->GetAlignerOffset();
		// 		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
		// 		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
		// 		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
		
		m_nCalibTargetMarkEnable = product.m_pAlignerParam->GetCalibTargetMarkEnable();
		m_comboCalibTargetMarkPatternMode.SetCurSel((int)product.m_pAlignerParam->GetCalibTargetMarkPatternMode());
		if(product.m_pAlignerParam->GetCalibTargetMarkPatternMode()==eDmCodeCalib)
		{
			GetDlgItem(IDC_BTN_DMCODE_PARAM_SETTING)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CHK_ENABLE_OUTLINE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_DMCODE_PARAM)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_DMCODE_PARAM_SETTING)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHK_ENABLE_OUTLINE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_DMCODE_PARAM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
		}

		m_bEnableOutLine = product.m_pAlignerParam->GetCalibTargetDMCodeOutLineEnable();
		m_nOutLinePercent = product.m_pAlignerParam->GetCalibTargetDMCodeOutLinePercent()*100;
		if (product.m_pAlignerParam->GetCalibTargetDMCodeOutLineEnable())
		{
			GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_PERCENT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_PERCENT)->EnableWindow(FALSE);
		}

		m_bObjectShutterEnable0 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bShutterEnable;
		m_bObjectGainEnable0 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bGainEnable;
		m_bObjectGammaEnable0 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bGammaEnable;

		m_bObjectShutterEnable1 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bShutterEnable;
		m_bObjectGainEnable1 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bGainEnable;
		m_bObjectGammaEnable1 = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bGammaEnable;

//		m_nVirtualObjectCandidateEnable = (int)product.m_pAlignerParam->GetVirtualObjectMarkEnable();
// 		m_nObjectCandidateEnable = (int)product.m_pAlignerParam->GetObjectCandidateEnable();
// 		m_nObjectCandidateSync = (int)product.m_pAlignerParam->GetObjectCandidateSyncEnable();

// 		CCoordPos pos;
// 		int i = 0;
// 		i++;
// 		pos = product.m_pAlignerParam->GetObjectCandidateOffset(i);
// 		m_strObjectCandidateOffsetX1.Format(_T("%.6f"), pos.GetPosX());
// 		m_strObjectCandidateOffsetY1.Format(_T("%.6f"), pos.GetPosY());
// 		m_strObjectCandidateOffsetD1.Format(_T("%.6f"), pos.GetAngle());
// 		m_bObjectCandidateOffsetEnable1 = product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i);
// 
// 		i++;
// 		pos = product.m_pAlignerParam->GetObjectCandidateOffset(i);
// 		m_strObjectCandidateOffsetX2.Format(_T("%.6f"), pos.GetPosX());
// 		m_strObjectCandidateOffsetY2.Format(_T("%.6f"), pos.GetPosY());
// 		m_strObjectCandidateOffsetD2.Format(_T("%.6f"), pos.GetAngle());
// 		m_bObjectCandidateOffsetEnable2 = product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i);
// 
// 		i++;
// 		pos = product.m_pAlignerParam->GetObjectCandidateOffset(i);
// 		m_strObjectCandidateOffsetX3.Format(_T("%.6f"), pos.GetPosX());
// 		m_strObjectCandidateOffsetY3.Format(_T("%.6f"), pos.GetPosY());
// 		m_strObjectCandidateOffsetD3.Format(_T("%.6f"), pos.GetAngle());
// 		m_bObjectCandidateOffsetEnable3 = product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i);
// 
// 		i++;
// 		pos = product.m_pAlignerParam->GetObjectCandidateOffset(i);
// 		m_strObjectCandidateOffsetX4.Format(_T("%.6f"), pos.GetPosX());
// 		m_strObjectCandidateOffsetY4.Format(_T("%.6f"), pos.GetPosY());
// 		m_strObjectCandidateOffsetD4.Format(_T("%.6f"), pos.GetAngle());
// 		m_bObjectCandidateOffsetEnable4 = product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i);


		// 目标Mark模板图像显示
		int i = 0;
		int j = 0;
		// 		if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
		// 		{
		// 			CBaseSearchTool* pTargetSearchTool = NULL;
		// 			scImageGray image;
		// 			for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
		// 			{
		// 				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
		// 				if (pTargetSearchTool != NULL)
		// 				{
		// 					image.Release();
		// 					pTargetSearchTool->GetPatternImage(image);
		// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
		// 					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
		// 				}
		// 				else
		// 				{
		// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
		// 				}
		// 			}
		// 		}
		// 
		// 		// 虚拟对象Mark模板图像显示
		// 		if (product.m_pAlignerParam->GetVirtualObjectMarkEnable())
		// 		{
		// 			CBaseSearchTool* pVirtualObjSearchTool = NULL;
		// 			scImageGray image;
		// 			for (i=0; i<product.m_pAlignerParam->GetVirtualObjectMarkPatternNum(); i++)
		// 			{
		// 				pVirtualObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
		// 				if (pVirtualObjSearchTool != NULL)
		// 				{
		// 					image.Release();
		// 					pVirtualObjSearchTool->GetPatternImage(image);
		// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
		// 					m_vpVirtualObjectGuiDisplay.at(i)->SetImage(image, TRUE);
		// 				}
		// 				else
		// 				{
		// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
		// 				}
		// 			}
		// 		}

// 		// 对象Mark模板图像显示
// 		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
// 		{
// 			CBaseSearchTool* pObjSearchTool = NULL;
// 			scImageGray image;
// 			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
// 			{
// 				// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				// 				if (pObjSearchTool != NULL)
// 				// 				{
// 				// 					image.Release();
// 				// 					pObjSearchTool->GetPatternImage(image);
// 				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
// 				// 				}
// 				// 				else
// 				// 				{
// 				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 				}
// 
// 				for(j = 0; j<OBJECTCANDIDATENUM; j++)
// 				{
// 					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx,i, j);
// 					if (pObjSearchTool != NULL)
// 					{
// 						image.Release();
// 						pObjSearchTool->GetPatternImage(image);
// 						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 						m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
// 					}
// 					else
// 					{
// 						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 					}
// 				}
// 			}
// 		}
// 
// 		// 虚拟对象Mark模板图像显示
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			for (j = 0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 			}
// 		}
// 		if (product.m_pAlignerParam->GetObjectMarkType() == eObjectVirtual)
// 		{
// 			CBaseSearchTool* pObjSearchTool = NULL;
// 			scImageGray image;
// 			for (i=0; i<product.m_pAlignerParam->GetVirtualObjectMarkPatternNum(); i++)
// 			{
// 				// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				// 				if (pObjSearchTool != NULL)
// 				// 				{
// 				// 					image.Release();
// 				// 					pObjSearchTool->GetPatternImage(image);
// 				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
// 				// 				}
// 				// 				else
// 				// 				{
// 				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 				}
// 
// 				for(j = 0; j<VIRTUALOBJECTCANDIDATENUM; j++)
// 				{
// 					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnVirtualObjectSearchTool(nCurProdIdx,i, j);
// 					if (pObjSearchTool != NULL)
// 					{
// 						image.Release();
// 						pObjSearchTool->GetPatternImage(image);
// 						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 						m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
// 					}
// 					else
// 					{
// 						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 					}
// 				}
// 			}
// 		}


		// 关联标定Mark模板图像显示
		for (i=0; i<MAXPOSNUM; i++)
		{
			for (j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				m_vvpObjectGuiDisplay[i][j]->ClearScreen();
			}
		}
		if (product.m_pAlignerParam->GetTargetObjectCamSeparateEnable() == TRUE ||
			(product.m_pAlignerParam->GetTargetObjectCamSeparateEnable() == FALSE && product.m_pAlignerParam->GetTargetCalibUseBoardEnable() == TRUE))
		{
			CBaseSearchTool* pObjSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetCalibTargetMarkPatternNum(); i++)
			{
				// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
				// 				if (pObjSearchTool != NULL)
				// 				{
				// 					image.Release();
				// 					pObjSearchTool->GetPatternImage(image);
				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
				// 				}
				// 				else
				// 				{
				// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 				}

				for(j = 0; j<ALNCALIBTARGETCANDIDATENUM; j++)
				{
					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,i, j);
					BOOL bExistSearchTool = IsSearchToolSelected(pObjSearchTool);
					if (pObjSearchTool != NULL && bExistSearchTool)
					{
						image.Release();
						pObjSearchTool->GetPatternImage(image);
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
						m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}

		// 		CBaseSearchTool* pObjSearchTool = NULL;
		// 		scImageGray image;
		// 		for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
		// 		{
		// 			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
		// 			if (pObjSearchTool != NULL)
		// 			{
		// 				image.Release();
		// 				pObjSearchTool->GetPatternImage(image);
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 				m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
		// 			}
		// 			else
		// 			{
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 			}
		// 		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}



void CDlgSysKindAlignCalibTarget::GetDlgTempData(CProductData& product)
{

	UpdateData(TRUE);	
	product.m_pAlignerParam->SetCalibTargetMarkEnable(m_nCalibTargetMarkEnable);
	product.m_pAlignerParam->SetCalibTargetMarkPatternMode((CalibTargetMarkPatternMode)m_comboCalibTargetMarkPatternMode.GetCurSel());
	product.m_pAlignerParam->SetCalibTargetDMCodeOutLineEnable(m_bEnableOutLine);
	product.m_pAlignerParam->SetCalibTargetDMCodeOutLinePercent((double)m_nOutLinePercent/100);

	CameraParamEnable cameraParamEnable;
	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
	product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(0, cameraParamEnable);

	cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
	cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
	cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;;
	product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(1, cameraParamEnable);


//	product.m_pAlignerParam->SetVirtualObjectMarkEnable(m_nVirtualObjectCandidateEnable);
// 
// 	product.m_pAlignerParam->SetObjectCandidateEnable(m_nObjectCandidateEnable);
// 	product.m_pAlignerParam->SetObjectCandidateSyncEnable(m_nObjectCandidateSync);
// 
// 	CCoordPos pos;		
// 	int i=0; 
// 	i++;		
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX1);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetX1);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetX1);
// 	product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 	product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable1);
// 
// 	i++;		
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX2);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetX2);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetX2);
// 	product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 	product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable2);
// 
// 	i++;		
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX3);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetX3);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetX3);
// 	product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 	product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable3);
// 
// 	i++;		
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX4);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetX4);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetX4);
// 	product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 	product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable4);


	//BeginWaitCursor();
	//int nProductNum = pTotalProduct->GetCount();
	//nCurProdIdx = pTotalProduct->GetCurProductIndex();
	// 		if (nProductNum > 0)
	// 		{
	// 			if (nCurProdIdx < 0)
	// 			{
	// 				nCurProdIdx = 0;
	// 			}
	// 			else if (nCurProdIdx >= nProductNum)
	// 			{
	// 				nCurProdIdx = nProductNum - 1;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			nCurProdIdx = -1;
	// 		}
	//pTotalProduct->SetCurProductIndex(nCurProdIdx);
	// 		// 从总产品信息中获取当前产品信息，如果总产品数为0，则增加一个空产品
	// 		if (nCurProdIdx < 0)
	// 		{
	// 			SysPlatformInfo platformInfo;
	// 			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	// 			product = CProductData(platformInfo);
	// 			CString strNewName = GetNewDefName();
	// 			pTotalProduct->AddItem(product);
	// 			pTotalProduct->SetItemName(0, strNewName);
	// 			pTotalProduct->SetCurProductIndex(0);
	// 			nCurProdIdx = pTotalProduct->GetCurProductIndex();
	// 
	// 			m_bProdNewCreate = TRUE;
	// 			m_strProductNameOld = strNewName;
	// 		}
	//product = pTotalProduct->GetItem(nCurProdIdx);

	//		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateCurVisionASMTool(FALSE); // 更新当前视觉对位工具（加载定位工具）
	//		m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);
	// 
	// 		m_nCurProductIdx = nCurProdIdx;
	// 		m_nPrevProductIdx = m_nCurProductIdx;
	// 		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
	// 		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
	// 		m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
	// 		m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
	// 		m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());
	// 		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
	// 		pos = product.m_pAlignerParam->GetAlignerOffset();
	// 		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
	// 		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
	// 		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
	// 
	//nProductNum = pTotalProduct->GetCount();

	// 		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
	// 		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
	// 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
	// 		{
	// 			int nMBReturn = IDNO;
	// 			if (!m_bProdNewCreate)
	// 			{
	// 				// 				CString strInfo = _T("该产品所使用的平台标定结果与当前平台的标定结果不同。");
	// 				// 				strInfo += _T("\n仍然要使用该产品的标定结果，请点击“是”；");
	// 				// 				strInfo += _T("\n应用当前平台的标定结果到该产品，请点击“否”。");
	// 				nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIBRATED_INFO_VARIED_PLEASE_SELECT), MB_YESNO);
	// 			}
	// 
	// 			if (nMBReturn == IDYES)	// 用产品标定结果代替当前平台标定结果
	// 			{
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(product.m_vpCalibratedInfo);
	// 			}
	// 			else if (nMBReturn == IDNO)	// 用当前平台标定结果代替产品标定结果
	// 			{
	// 				int j = 0;
	// 				for (j=0; j<product.m_vpCalibratedInfo.size(); j++)
	// 				{
	// 					if (product.m_vpCalibratedInfo.at(j) != NULL)
	// 					{
	// 						delete product.m_vpCalibratedInfo.at(j);
	// 						product.m_vpCalibratedInfo.at(j) = NULL;
	// 					}
	// 				}
	// 				int nNewSize = vpCurCalibratedInfo.size();
	// 				product.m_vpCalibratedInfo.resize(nNewSize, NULL);
	// 				for (j=0; j<nNewSize; j++)
	// 				{
	// 					if (vpCurCalibratedInfo.at(j) != NULL)
	// 					{
	// 						product.m_vpCalibratedInfo.at(j) = new CCalibratedInfo(*vpCurCalibratedInfo.at(j));
	// 					}
	// 				}
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
	// 
	// 				m_bCalibInfoChanged = TRUE;
	// 			}
	// 		}

	//// 目标Mark模板图像显示
	//int i = 0, j = 0;
	// 		if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
	// 		{
	// 			CBaseSearchTool* pTargetSearchTool = NULL;
	// 			scImageGray image;
	// 			for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
	// 			{
	// 				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
	// 				if (pTargetSearchTool != NULL)
	// 				{
	// 					image.Release();
	// 					pTargetSearchTool->GetPatternImage(image);
	// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
	// 					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 			}
	// 		}
	// 
	// 		// 虚拟对象Mark模板图像显示
	// 		if (product.m_pAlignerParam->GetVirtualObjectMarkEnable())
	// 		{
	// 			CBaseSearchTool* pVirtualObjSearchTool = NULL;
	// 			scImageGray image;
	// 			for (i=0; i<product.m_pAlignerParam->GetVirtualObjectMarkPatternNum(); i++)
	// 			{
	// 				pVirtualObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
	// 				if (pVirtualObjSearchTool != NULL)
	// 				{
	// 					image.Release();
	// 					pVirtualObjSearchTool->GetPatternImage(image);
	// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
	// 					m_vpVirtualObjectGuiDisplay.at(i)->SetImage(image, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 			}
	// 		}

	//// 对象Mark模板图像显示
	//if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	//{
	//	CBaseSearchTool* pObjSearchTool = NULL;
	//	scImageGray image;
	//	for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
	//	{
	//		// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	//		// 				if (pObjSearchTool != NULL)
	//		// 				{
	//		// 					image.Release();
	//		// 					pObjSearchTool->GetPatternImage(image);
	//		// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	//		// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
	//		// 				}
	//		// 				else
	//		// 				{
	//		// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	//		// 				}

	//		for(j = 0; j<OBJECTCANDIDATENUM; j++)
	//		{
	//			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(i, j);
	//			if (pObjSearchTool != NULL)
	//			{
	//				image.Release();
	//				pObjSearchTool->GetPatternImage(image);
	//				m_vvpObjectGuiDisplay[i][j]->ClearScreen();
	//				m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
	//			}
	//			else
	//			{
	//				m_vvpObjectGuiDisplay[i][j]->ClearScreen();
	//			}
	//		}
	//	}
	//}
	// 		CBaseSearchTool* pObjSearchTool = NULL;
	// 		scImageGray image;
	// 		for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
	// 		{
	// 			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	// 			if (pObjSearchTool != NULL)
	// 			{
	// 				image.Release();
	// 				pObjSearchTool->GetPatternImage(image);
	// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 				m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
	// 			}
	// 			else
	// 			{
	// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 			}
	// 		}

	//UpdateData(FALSE);
	//EndWaitCursor();

}

void CDlgSysKindAlignCalibTarget::UpdateStatusBar()
{
	// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysKindAlignCalibTarget::UpdateProductListDisplay()
{
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	// 	if (pTotalProduct == NULL)
	// 	{
	// 		return;
	// 	}
	// 
	// 	CString strTemp;
	// 	int nProductNum = pTotalProduct->GetCount();
	// 	m_listctrlProduct.DeleteAllItems();
	// 	for (int i=0; i<nProductNum; i++)
	// 	{
	// 		// 序号
	// 		strTemp.Format(_T("%d"), i);
	// 		m_listctrlProduct.InsertItem(i, strTemp);
	// 
	// 		// 产品名称
	// 		strTemp = pTotalProduct->GetItemName(i);
	// 		m_listctrlProduct.SetItemText(i, 1, strTemp);
	// 
	// 		if (i == pTotalProduct->GetCurProductIndex())
	// 		{
	// 			m_listctrlProduct.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
	// 		}
	// 	}
}

void CDlgSysKindAlignCalibTarget::UpdateDialogLanguage()
{
//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_SYS_KIND_ALIGN_CALIB_TARGET") == strDlgID)
		{
			int nID;
			CString strValue = xml.GetAttribute(_T("Value"));
			SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				xml.GetAttribute(_T("Id"),  nID);
				strValue = xml.GetAttribute(_T("Value"));

				if(GetDlgItem(nID) != NULL)
				{
					GetDlgItem(nID)->SetWindowText(strValue);
				}	  
			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();	
}

CString CDlgSysKindAlignCalibTarget::GetNewDefName()
{
	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	int nNum = pTotalProduct->GetCount();
	CString strName;
	for (int i=0; i<=nNum; i++)
	{
		strName.Format(_T("Job%d"), i);
		if (IsNameUnique(strName))
		{
			return strName;
		}
	}
	return strName;
	// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	// 	int nNum = pTotalProduct->GetCount();
	// 	CString strName;
	// 	strName.Format(_T("Product%d"), nNum-1);
	// 	if (!IsNameUnique(strName))
	// 	{
	// 		for (int i=0; i<=nNum; i++)
	// 		{
	// 			strName.Format(_T("Product%d"), i);
	// 			if (IsNameUnique(strName))
	// 			{
	// 				return strName;
	// 			}
	// 		}
	// 	}
	// 	return strName;
}

BOOL CDlgSysKindAlignCalibTarget::IsNameUnique( LPCTSTR lpszName )
{
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	// 	int nProductNum = pTotalProduct->GetCount();
	// 	CString strName = lpszName;
	// 	if (strName.IsEmpty())
	// 	{
	// 		return FALSE;
	// 	}
	// 	for (int i=0; i<nProductNum; i++)
	// 	{
	// 		if (strName == pTotalProduct->GetItemName(i))
	// 		{
	// 			return FALSE;
	// 		}
	// 	}
	return TRUE;
}

BOOL CDlgSysKindAlignCalibTarget::IsNameLegal( LPCTSTR lpszName )
{
	CString strName = lpszName;
	if (strName.IsEmpty())
	{
		return FALSE;
	}

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	int nNum = pTotalProduct->GetCount();
	for (int i=0; i<nNum; i++)
	{
		if (strName == pTotalProduct->GetItemName(i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignCalibTarget::IsCalibratedInfosEqual( std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2 )
{
	int nSize1 = vpCalibratedInfo1.size();
	int nSize2 = vpCalibratedInfo2.size();
	if (nSize1 != nSize2)
	{
		return FALSE;
	}
	for (int i=0; i<nSize1; i++)
	{
		if(!(*(vpCalibratedInfo1.at(i)) == *(vpCalibratedInfo2.at(i))))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignCalibTarget::IsProductChanged()
{
	if (m_bProdPatternChanged)
	{
		return TRUE;
	}

	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	//	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);// 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	ASSERT(pTotalProduct != NULL);
	CProductData product;
	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	{
		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	}
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);

	if (m_nCalibTargetMarkEnable != (int)product.m_pAlignerParam->GetCalibTargetMarkEnable())
	{
		return TRUE;
	}
	if (m_bEnableOutLine!=product.m_pAlignerParam->GetCalibTargetDMCodeOutLineEnable() || ((double)m_nOutLinePercent/100)!= product.m_pAlignerParam->GetCalibTargetDMCodeOutLinePercent())
	{
		return TRUE;
	}
	if (m_comboCalibTargetMarkPatternMode.GetCurSel() != (int)product.m_pAlignerParam->GetCalibTargetMarkPatternMode())
	{
		return TRUE;
	}

	if (m_bObjectShutterEnable0 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bShutterEnable
		|| m_bObjectGainEnable0 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bGainEnable
		|| m_bObjectGammaEnable0 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(0).m_bGammaEnable)
	{
		return TRUE;
	}

	if (m_bObjectShutterEnable1 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bShutterEnable
		|| m_bObjectGainEnable1 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bGainEnable
		|| m_bObjectGammaEnable1 != product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(1).m_bGammaEnable)
	{
		return TRUE;
	}




// 	if (m_nVirtualObjectCandidateEnable != (int)product.m_pAlignerParam->GetVirtualObjectMarkEnable())
// 	{
// 		return TRUE;
// 	}
// 	if (m_nObjectCandidateEnable != (int)product.m_pAlignerParam->GetObjectCandidateEnable())
// 	{
// 		return TRUE;
// 	}
// 
// 	if (m_nObjectCandidateSync != (int)product.m_pAlignerParam->GetObjectCandidateSyncEnable())
// 	{
// 		return TRUE;
// 	}
// 
// 	CCoordPos pos;
// 	int i = 0;
// 
// 	i++;
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX1);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetY1);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetD1);
// 	if (pos.m_dPosX != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosX 
// 		|| pos.m_dPosY != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosY 
// 		|| pos.m_dAngle != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dAngle
// 		|| m_bObjectCandidateOffsetEnable1 != product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i))
// 	{
// 		return TRUE;
// 	}
// 
// 	i++;
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX2);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetY2);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetD2);
// 	if (pos.m_dPosX != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosX 
// 		|| pos.m_dPosY != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosY 
// 		|| pos.m_dAngle != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dAngle
// 		|| m_bObjectCandidateOffsetEnable2 != product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i))
// 	{
// 		return TRUE;
// 	}
// 
// 	i++;
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX3);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetY3);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetD3);
// 	if (pos.m_dPosX != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosX 
// 		|| pos.m_dPosY != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosY 
// 		|| pos.m_dAngle != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dAngle
// 		|| m_bObjectCandidateOffsetEnable3 != product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i))
// 	{
// 		return TRUE;
// 	}
// 
// 	i++;
// 	pos.m_dPosX = atof(m_strObjectCandidateOffsetX4);
// 	pos.m_dPosY = atof(m_strObjectCandidateOffsetY4);
// 	pos.m_dAngle = atof(m_strObjectCandidateOffsetD4);
// 	if (pos.m_dPosX != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosX 
// 		|| pos.m_dPosY != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dPosY 
// 		|| pos.m_dAngle != product.m_pAlignerParam->GetObjectCandidateOffset(i).m_dAngle
// 		|| m_bObjectCandidateOffsetEnable4 != product.m_pAlignerParam->GetObjectCandidateOffsetEnable(i))
// 	{
// 		return TRUE;
// 	}

	// 	if (m_bProdPatternChanged || m_bProdNameChanged || m_bProdBenchMarkChanged || m_bProdNewCreate || m_bCalibInfoChanged)
	// 	{
	// 		return TRUE;
	// 	}
	// 
	// 	UpdateData(TRUE);
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx); // 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	// 	ASSERT(pTotalProduct != NULL);
	// 	CProductData product;
	// 	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	// 	{
	// 		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	// 	}
	// 	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	// 	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	// 	if (m_nAlignerMaxTimes != product.m_pAlignerParam->GetAlignerMaxTime())
	// 	{
	// 		return TRUE;
	// 	}
	// 	if (m_comboAlignerOffsetCoordType.GetCurSel() != (int)product.m_pAlignerParam->GetAlignerOffsetCoordType())
	// 	{
	// 		return TRUE;
	// 	}
	// 	CCoordPos pos;
	// 	pos.m_dPosX = atof(m_strAlignerOffsetX);
	// 	pos.m_dPosY = atof(m_strAlignerOffsetY);
	// 	pos.m_dAngle = atof(m_strAlignerOffsetD);
	// 	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset().m_dPosX 
	// 		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset().m_dPosY 
	// 		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset().m_dAngle)
	// 	{
	// 		return TRUE;
	// 	}
	// 	pos.m_dPosX = atof(m_strAlignerPrecisionX);
	// 	pos.m_dPosY = atof(m_strAlignerPrecisionY);
	// 	pos.m_dAngle = atof(m_strAlignerPrecisionD);
	// 	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerPrecision().m_dPosX 
	// 		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerPrecision().m_dPosY 
	// 		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerPrecision().m_dAngle)
	// 	{
	// 		return TRUE;
	// 	}
	return FALSE;
}

BOOL CDlgSysKindAlignCalibTarget::IsSearchToolSelected(CBaseSearchTool* pSearchTool)
{
	//UpdateData(TRUE);
	BOOL bExistSearchTool =TRUE;
	SysPlatformInfo sysPlat;
	DlgSearchToolInfo dlgSearchTool;
	CSearchToolType nSearchToolType = eSearchToolUnknown;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlat);
	dlgSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDlgSearchToolData();
	if (pSearchTool!= NULL)
	{
		nSearchToolType = pSearchTool->GetSearchToolType();
		vector<map<pair<int,int>,BOOL>>::iterator iterSys;
		map<pair<int,CString>,BOOL>::iterator iterDlg;
		iterSys = sysPlat.m_vmapSysSearchToolInfo.begin();
		for (;iterSys != sysPlat.m_vmapSysSearchToolInfo.end();iterSys++)
		{
			if (nSearchToolType == (*iterSys).begin()->first.second)
			{
				for (iterDlg = dlgSearchTool.m_mapDlgSearchToolInfo.begin();iterDlg != dlgSearchTool.m_mapDlgSearchToolInfo.end();iterDlg++)
				{
					if ((*iterSys).begin()->first.first == iterDlg->first.first)
					{
						bExistSearchTool = iterDlg->second;
						break;
					}
				}
				break;
			}
		}
	}

	return bExistSearchTool;
}

BOOL CDlgSysKindAlignCalibTarget::IsStringNumerical( LPCTSTR lpszVal )
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignCalibTarget::CreateDirectoryPro( LPCTSTR lpszDir )
{
	CString strDir = lpszDir;
	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{	
		// 创建多级目录
		int nLen = 0;
		DWORD dwLevelXAttrib = 0;
		int nSlashIdx = strDir.Find('\\');
		CString strDirLevelX = _T("");
		CString strTemp = strDir;
		while (nSlashIdx != -1)
		{
			nLen = strDirLevelX.GetLength();
			nSlashIdx += (nLen == 0) ? 0 : (nLen + 1);
			strDirLevelX = strDir.Left(nSlashIdx);
			strTemp = strDir.Mid(nSlashIdx + 1);
			dwLevelXAttrib = GetFileAttributes(strDirLevelX);
			if ((dwLevelXAttrib == 0xFFFFFFFF) || ((dwLevelXAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0))
			{
				if (!CreateDirectory(strDirLevelX, NULL))
				{
					return FALSE;
				}
			}
			nSlashIdx = strTemp.Find('\\');
		}
		if (!CreateDirectory(strDir, NULL))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgSysKindAlignCalibTarget::ClearDirectoryPro( LPCTSTR lpszDir )
{
	CFileFind fileFind; 
	CString strDir(lpszDir);
	CString strPath = strDir + _T("\\*.*");
	BOOL bFound = fileFind.FindFile(strPath); 
	while (bFound) 
	{ 
		bFound = fileFind.FindNextFile(); 

		if (!fileFind.IsDots()) 
		{ 
			CString strFileName = fileFind.GetFileName();
			CString strSubFile = strDir + _T("\\") + strFileName;
			if (fileFind.IsDirectory()) 
			{ 
				if (!ClearDirectoryPro(strSubFile)) //先删除文件夹下边的所有文件
				{
					return FALSE;
				}
				if (!RemoveDirectory(strSubFile)) // 删除该文件夹
				{
					return FALSE;
				}
			} 
			else 
			{ 
				if (!DeleteFile(strSubFile))	//将该文件夹删除
				{
					return FALSE;
				}
			} 
		}
	} 
	fileFind.Close(); 
	return TRUE; 
}

BOOL CDlgSysKindAlignCalibTarget::RemoveDirectoryPro( LPCTSTR lpszDir )
{
	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return TRUE;
	}

	if (!ClearDirectoryPro(lpszDir))
	{
		return FALSE;
	}
	if (!RemoveDirectory(lpszDir))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDlgSysKindAlignCalibTarget::RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
{
	CString strOldDir = lpszOldDir;
	CString strNewDir = lpszNewDir;
	if (strOldDir.IsEmpty() || strNewDir.IsEmpty())
	{
		return FALSE;
	}

	// 如果文件夹不存在，则返回TRUE
	DWORD dwAttributes = GetFileAttributes(strOldDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return FALSE;
	}

	// 文件夹存在，则重命名文件夹
	SHFILEOPSTRUCT shFileOp={0};
	shFileOp.fFlags = FOF_NOCONFIRMATION; // 不出现确认对话框
	TCHAR pszzDir[MAX_PATH] = {0};
	TCHAR *p = strOldDir.GetBuffer(strOldDir.GetLength());
	_tcscat(pszzDir, p);
	shFileOp.pFrom	= pszzDir; 
	shFileOp.pTo	= lpszNewDir;	
	shFileOp.wFunc	= FO_RENAME;	//重命名操作
	if (SHFileOperation(&shFileOp) != 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
// 保存前一个产品的设置
BOOL CDlgSysKindAlignCalibTarget::SavePrevProduct()
{
	BOOL bSucceed = TRUE;
	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	m_nPrevProductIdx = pView->m_nPrevProductIdx;

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	{
		// 保存对位参数
		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
		product.m_pAlignerParam->SetCalibTargetMarkEnable(m_nCalibTargetMarkEnable);
		product.m_pAlignerParam->SetCalibTargetMarkPatternMode((CalibTargetMarkPatternMode)m_comboCalibTargetMarkPatternMode.GetCurSel());
		product.m_pAlignerParam->SetCalibTargetDMCodeOutLineEnable(m_bEnableOutLine);
		product.m_pAlignerParam->SetCalibTargetDMCodeOutLinePercent((double)m_nOutLinePercent/100);

		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable0;
		product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(0, cameraParamEnable);

		cameraParamEnable.m_bShutterEnable = m_bObjectShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bObjectGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bObjectGammaEnable1;;
		product.m_pAlignerParam->SetEnableCalibTargetSearchShutter(1, cameraParamEnable);


//		product.m_pAlignerParam->SetVirtualObjectMarkEnable(m_nVirtualObjectCandidateEnable);
// 		product.m_pAlignerParam->SetObjectCandidateEnable(m_nObjectCandidateEnable);
// 		product.m_pAlignerParam->SetObjectCandidateSyncEnable(m_nObjectCandidateSync);
// 
// 		CCoordPos pos;		
// 		int i=0; 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX1);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX1);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX1);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable1);
// 
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX2);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX2);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX2);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable2);
// 
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX3);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX3);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX3);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable3);
// 
// 
// 		i++;		
// 		pos.m_dPosX = atof(m_strObjectCandidateOffsetX4);
// 		pos.m_dPosY = atof(m_strObjectCandidateOffsetX4);
// 		pos.m_dAngle = atof(m_strObjectCandidateOffsetX4);
// 		product.m_pAlignerParam->SetObjectCandidateOffset(i, pos);
// 		product.m_pAlignerParam->SetObjectCandidateOffsetEnable(i, m_bObjectCandidateOffsetEnable4);


		// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
		// 		CCoordPos pos;
		// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
		// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
		// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
		// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
		// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
		// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
		// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
		// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
		// 		product.m_pAlignerParam->SetAlignerOffset(pos);
		// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
		// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
		// 		{
		// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
		// 		}

		// 		// by adk
		// 		CBaseSearchTool *pSearchTool = NULL;
		// 		SysPlatformInfo platformInfo;
		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		// 		for (int j=0; j<nPatternNum; j++)
		// 		{
		// 			// 			int nPatSel = m_vnCurSelPat[j];
		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
		// 			
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
		// 		} // end by

		pTotalProduct->SetItem(m_nPrevProductIdx, product);
		m_bProdPatternChanged = FALSE;
	}
	// 	UpdateData(TRUE);
	// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	// 	{
	// 		// 保存对位参数
	// 		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
	// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
	// 		CCoordPos pos;
	// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
	// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
	// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
	// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
	// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
	// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
	// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
	// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
	// 		product.m_pAlignerParam->SetAlignerOffset(pos);
	// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
	// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
	// 		{
	// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
	// 		}
	// 
	// 		// 		// by adk
	// 		// 		CBaseSearchTool *pSearchTool = NULL;
	// 		// 		SysPlatformInfo platformInfo;
	// 		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	// 		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
	// 		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	// 		// 		for (int j=0; j<nPatternNum; j++)
	// 		// 		{
	// 		// 			// 			int nPatSel = m_vnCurSelPat[j];
	// 		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
	// 		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
	// 		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
	// 		// 			
	// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
	// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
	// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
	// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
	// 		// 		} // end by
	// 
	// 		pTotalProduct->SetItem(m_nPrevProductIdx, product);
	// 
	// 
	// 		// 保存产品信息到磁盘
	// 		CString strDir, strTemp;
	// 		SysPlatformInfo platformInfo;
	// 		vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	// 		pVisionASM->GetSysPlatformInfo(platformInfo);
	// 		// 如果产品被重命名，则删除旧文件架
	// 		if ((m_bProdNameChanged) && (!m_strProductNameOld.IsEmpty()))
	// 		{
	// 			strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	// 			strTemp.Format(_T("\\Jobs\\%s"), m_strProductNameOld);
	// 			strDir += strTemp;
	// 			RemoveDirectoryPro(strDir);
	// 		}
	// 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	// 		strTemp.Format(_T("\\Jobs\\%s"), pTotalProduct->GetItemName(m_nPrevProductIdx));
	// 		strDir += strTemp;        
	// 		//ClearDirectoryPro(strDir); // Mali 2013.9.5 为提升效率
	// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nPrevProductIdx);
	// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
	// 
	// 
	// 		// 保存模板到磁盘
	// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	// 		CBaseSearchTool *pSearchTool = NULL;
	// 
	// 		int j = 0;
	// // 		// 保存目标Mark模板
	// // 		if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	// // 		{
	// // 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
	// // 			{
	// // 				CreateDirectoryPro(strDir + _T("\\TargetModels"));
	// // 				for (j=0; j<nPatternNum; j++)
	// // 				{
	// // 					strTemp.Format(_T("\\TargetModels\\M%d"), j);
	// // 					pSearchTool = pVisionASM->GetAlnTargetSearchToolParam(j);
	// // 					if (pSearchTool != NULL) 
	// // 					{						
	// // 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
	// // 						{
	// // 							//strTemp.Format(_T("CH%d目标Mark模板保存失败！"), j);
	// // 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTEN_SAVE_FAILED));
	// // 							AfxMessageBox(strTemp);
	// // 							bSucceed = FALSE;
	// // 						}						
	// // 
	// // 					}
	// // 				}
	// // 
	// // 			}		
	// // 		}
	// // 		else
	// // 		{
	// // 			RemoveDirectoryPro(strDir + _T("\\TargetModels"));
	// // 		}
	// // 
	// // 		// 保存虚拟对象Mark模板
	// // 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	// // 		{
	// // 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
	// // 			{
	// // 				CreateDirectoryPro(strDir + _T("\\ObjectExModels"));
	// // 				for (j=0; j<nPatternNum; j++)
	// // 				{
	// // 					strTemp.Format(_T("\\ObjectExModels\\M%d"), j);
	// // 					pSearchTool = pVisionASM->GetAlnVirtualSearchToolParam(j);
	// // 					if (pSearchTool != NULL)
	// // 					{
	// // 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
	// // 						{
	// // 							//strTemp.Format(_T("CH%d虚拟对象Mark模板保存失败！"), j);
	// // 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_FAILED));
	// // 							AfxMessageBox(strTemp);
	// // 							bSucceed = FALSE;
	// // 						}
	// // 					}
	// // 				}
	// // 
	// // 			}
	// // 		}
	// // 		else
	// // 		{
	// // 			RemoveDirectoryPro(strDir + _T("\\ObjectExModels"));
	// // 		}
	// 
	// 
	// 		if (platformInfo.m_eAlignerObjectType != eObjectBench)
	// 		{
	// 			// 保存对象Mark模板
	// 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
	// 			{
	// 				CreateDirectoryPro(strDir + _T("\\ObjectModels"));
	// 				for (j=0; j<nPatternNum; j++)
	// 				{
	// 					strTemp.Format(_T("\\ObjectModels\\M%d"), j);
	// 					pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
	// 					if (pSearchTool != NULL)
	// 					{					
	// 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
	// 						{
	// 							//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
	// 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
	// 							AfxMessageBox(strTemp);
	// 							bSucceed = FALSE;
	// 						}	
	// 					}
	// 				}
	// 			}
	// 		}
	// 		else
	// 		{
	// 			RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
	// 		}
	// 
	// 		// 		// 保存对象Mark模板
	// 		// 		if (m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
	// 		// 		{
	// 		// 			CreateDirectoryPro(strDir + _T("\\ObjectModels"));
	// 		// 			for (j=0; j<nPatternNum; j++)
	// 		// 			{
	// 		// 				strTemp.Format(_T("\\ObjectModels\\M%d"), j);
	// 		// 				pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
	// 		// 				if (pSearchTool != NULL)
	// 		// 				{					
	// 		// 					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
	// 		// 					{
	// 		// 						//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
	// 		// 						strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
	// 		// 						AfxMessageBox(strTemp);
	// 		// 						bSucceed = FALSE;
	// 		// 					}	
	// 		// 				}
	// 		// 			}
	// 		// 		}
	// 
	// 		m_strProductNameOld.Empty();
	// 	}

	return bSucceed;
}
void CDlgSysKindAlignCalibTarget::OnButtonNewProduct()
{
	// 	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	// 	if (IsProductChanged())
	// 	{
	// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 		if (nMBReturn == IDYES)
	// 		{
	// 			if (SavePrevProduct())
	// 			{
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 		else if (nMBReturn == IDCANCEL)
	// 		{
	// 			return;
	// 		}
	// 		else // 不保存对产品的修改
	// 		{
	// 			if (m_bProdNameChanged)
	// 			{
	// 				CString strOldDir, strNewDir;
	// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
	// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 				}
	// 
	// 				// 如果文件夹存在，则重命名文件夹
	// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 				{
	// 					if (strCurName != m_strProductNameOld)
	// 					{						
	// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 						{
	// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
	// 						}
	// 					}
	// 				}
	// 
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
	// 				UpdateDlgData(FALSE);
	// 			}
	// 			if (m_bProdNewCreate)
	// 			{
	// 				CString strDir;
	// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 				}
	// 				strDir = m_strModuleDir + strDir;
	// 				if (!RemoveDirectoryPro(strDir))
	// 				{
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
	// 				}
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
	// 			}
	// 			if (m_bProdNameChanged || m_bProdNewCreate)
	// 			{
	// 				UpdateProductListDisplay();
	// 			}
	// 
	// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 			{                        
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 			}
	// 
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 	}
	// 
	// 	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	// 	int nCount = pTotalProduct->GetCount();
	// 	CString strName = GetNewDefName();
	// 	// 	if (nCount < 1)
	// 	// 	{
	// 	SysPlatformInfo platformInfo;
	// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	// 	CProductData product(platformInfo);
	// 	pTotalProduct->AddItem(product);
	// 	pTotalProduct->SetItemName(nCount, strName);
	// 	pTotalProduct->SetCurProductIndex(nCount);
	// 	// 	}
	// 	// 	else
	// 	// 	{
	// 	// //		UpdateDlgData(TRUE);
	// 	// 		int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	// 	// 		if (pTotalProduct->CopyItem(nFormIdx, nCount))
	// 	// 		{
	// 	// 			pTotalProduct->SetItemName(nCount, strName);
	// 	// 			pTotalProduct->SetCurProductIndex(nCount);
	// 	// 		}
	// 	// 	}
	// 	m_strProductNameOld = strName;
	// 	m_bProdNewCreate = TRUE;

	UpdateDlgData(FALSE);
	//	UpdateProductListDisplay();
}


void CDlgSysKindAlignCalibTarget::OnBtnCloneProduct() 
{
	// TODO: Add your control notification handler code here
	// 	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	// 	if (IsProductChanged())
	// 	{
	// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 		if (nMBReturn == IDYES)
	// 		{
	// 			if (SavePrevProduct())
	// 			{
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 		else if (nMBReturn == IDCANCEL)
	// 		{
	// 			return;
	// 		}
	// 		else // 不保存对产品的修改
	// 		{
	// 			if (m_bProdNameChanged)
	// 			{
	// 				CString strOldDir, strNewDir;
	// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
	// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 				}
	// 
	// 				// 如果文件夹存在，则重命名文件夹
	// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 				{
	// 					if (strCurName != m_strProductNameOld)
	// 					{						
	// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 						{
	// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
	// 						}
	// 					}
	// 				}
	// 
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
	// 				UpdateDlgData(FALSE);
	// 			}
	// 			if (m_bProdNewCreate)
	// 			{
	// 				CString strDir;
	// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 				}
	// 				strDir = m_strModuleDir + strDir;
	// 				if (!RemoveDirectoryPro(strDir))
	// 				{
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
	// 				}
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
	// 			}
	// 			if (m_bProdNameChanged || m_bProdNewCreate)
	// 			{
	// 				UpdateProductListDisplay();
	// 			}
	// 
	// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 			{                        
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 			}
	// 
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 	}
	// 
	// 	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	// 	int nCount = pTotalProduct->GetCount();
	// 	CString strName = GetNewDefName();
	// 	// 	if (nCount < 1)
	// 	// 	{
	// 	// 		SysPlatformInfo platformInfo;
	// 	// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	// 	// 		CProductData product(platformInfo);
	// 	// 		pTotalProduct->AddItem(product);
	// 	// 		pTotalProduct->SetItemName(0, strName);
	// 	// 		pTotalProduct->SetCurProductIndex(0);
	// 	// 	}
	// 	// 	else
	// 	// 	{
	// 	//		UpdateDlgData(TRUE);
	// 	int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	// 	if (pTotalProduct->CopyItem(nFormIdx, nCount))
	// 	{
	// 		pTotalProduct->SetItemName(nCount, strName);
	// 		pTotalProduct->SetCurProductIndex(nCount);
	// 	}
	// 	//	}
	// 	m_strProductNameOld = strName;
	// 	m_bProdNewCreate = TRUE;
	// 
	// 	// 拷贝模板：保存模板到新产品的模板文件夹下以达到拷贝模板的目的
	// 	int nPrevProdIdxTmp = m_nPrevProductIdx;
	// 	m_nPrevProductIdx = nCount;
	// 	SavePrevProduct();
	// 	m_nPrevProductIdx = nPrevProdIdxTmp;

	UpdateDlgData(FALSE);
	//	UpdateProductListDisplay();
}

void CDlgSysKindAlignCalibTarget::OnButtonDeleteCurProduct() 
{
	// 	// 用户删除操作确认
	// 	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_ROMOVE_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要删除当前产品？")
	// 	{
	// 		return;
	// 	}
	// 
	// 	UpdateData(TRUE);
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	// 	int nCount = pTotalProduct->GetCount();
	// 	if (nCount > 0)
	// 	{
	// 		CString strDir;
	// 		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
	// 		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 		{
	// 			SysPlatformInfo platformInfo;
	// 			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 			pVisionASM->GetSysPlatformInfo(platformInfo);
	// 			strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 		}
	// 		strDir = m_strModuleDir + strDir;
	// 		if (!RemoveDirectoryPro(strDir))
	// 		{
	// 			//AfxMessageBox(_T("删除产品数据文件失败！"));
	// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
	// 		}
	// 		if (pTotalProduct->RemoveItem(m_nCurProductIdx))
	// 		{
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 
	// 			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
	// 			pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
	// 			UpdateDlgData(FALSE);
	// 			UpdateProductListDisplay();
	// 			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurProductIndex());
	// 		}
	// 		else
	// 		{
	// 			//AfxMessageBox(_T("删除产品失败！"));
	// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
	// 		}
	// 	}
	// 	else
	// 	{
	// 		//AfxMessageBox(_T("当前没有任何产品！"));
	// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	// 	}
}

void CDlgSysKindAlignCalibTarget::OnBtnProductRename()
{
	// 	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
	// 	{
	// 		//AfxMessageBox(_T("请选择当前产品"));
	// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
	// 	}
	// 	CString strOldName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 
	// 	CDlgProductRename  dlgProductRename(strOldName, this);
	// 	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
	// 	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 	if (dlgProductRename.DoModal() == IDOK)
	// 	{
	// 		CString strNewName = dlgProductRename.GetNewName();
	// 		if (strNewName != strOldName)
	// 		{
	// 			CString strOldDir, strNewDir;
	// 			// 			strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strOldName);
	// 			// 			strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strNewName);
	// 			{
	// 				SysPlatformInfo platformInfo;
	// 				vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 				pVisionASM->GetSysPlatformInfo(platformInfo);
	// 				strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strOldName);
	// 				strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strNewName);
	// 			}
	// 			// 如果新命名的文件夹存在，则删除该文件夹
	// 			DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 			if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 			{
	// 				if (!RemoveDirectoryPro(strNewDir))
	// 				{
	// 					//AfxMessageBox(_T("重命名失败！"));
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 					return;
	// 				}
	// 			}
	// 			// 如果文件夹存在，则重命名文件夹
	// 			dwAttributes = GetFileAttributes(strOldDir);
	// 			if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 			{
	// 				if (!RenameDirecotryPro(strOldDir, strNewDir))
	// 				{
	// 					//AfxMessageBox(_T("重命名失败！"));
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 					return;
	// 				}
	// 			}		
	// 
	// 			m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, strNewName);
	// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
	// 			m_bProdNameChanged = TRUE;
	// 			UpdateDlgData(FALSE);
	// 			UpdateProductListDisplay();
	// 		}
	// 	}
}

void CDlgSysKindAlignCalibTarget::OnBtnAmendBenchmark() 
{
	// 	CDlgBenchMarkAmend dlgBenchMarkAmend;
	// 	UpdateData(TRUE);
	// 	dlgBenchMarkAmend.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	// 	dlgBenchMarkAmend.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 	if (dlgBenchMarkAmend.DoModal() == IDOK)
	// 	{
	// 		// 标记基准修正信息已修改
	// 		m_bProdBenchMarkChanged = TRUE;
	// 	}
}

// 自定义菜单响应
LRESULT CDlgSysKindAlignCalibTarget::OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < OBJECTCANDIDATENUM; j++)
		{
			if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
			{
				CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
				CProductData product = pTotalProduct->GetItem(nCurProdIdx);

				CameraParamEnable cameraParamEnable;
				cameraParamEnable = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(j);
				BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

				if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
				{
					CameraParamSet tmpameraParamSet;
					product.m_pAlignerParam->SetCalibTargetSearchShutter(i,j, tmpameraParamSet);
					pTotalProduct->SetItem(nCurProdIdx, product);
				}

				CString strLog;
				if (j == 0)
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d对象模板\n"),i+1);
					g_opLogFile.SaveLogText(strLog,4);
				}
				else
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d候选模板%d\n"),i+1,j);
					g_opLogFile.SaveLogText(strLog,4);
				}
				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,i,j);
				if (pSearchTool == NULL)
				{
					return S_OK;
				}

				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnCalibTargetSearchTool(nCurProdIdx,i, j);
				m_vvpObjectGuiDisplay[i][j]->ClearScreen();

				// 标记产品模板已修改
				m_bProdPatternChanged = TRUE;
				// 标记指定位置关联模板已经被修改了			
				m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged[i][j] = TRUE;
				return S_OK;
			}
		}
	}

	return S_OK ;
}

LRESULT CDlgSysKindAlignCalibTarget::OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0, nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < ALNCALIBTARGETCANDIDATENUM; j++)
		{
			if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
			{
				CString strLog;
				strLog.Format(_T("点击右键选择[拷贝定位工具到...]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,i,j);

				CDlgSearchToolCopyPos dlgSearchToolCopyPos;
				dlgSearchToolCopyPos.m_nPosNum = nPatternNum;
				dlgSearchToolCopyPos.m_nPosCNum = ALNCALIBTARGETCANDIDATENUM;
				dlgSearchToolCopyPos.m_nSrcCandidateIndex = j;
				dlgSearchToolCopyPos.SetSysInfoStringsRef(m_psaSysInfoStrings);

				if (dlgSearchToolCopyPos.DoModal() == IDOK)
				{
					nDstCandidateIndex = dlgSearchToolCopyPos.m_nCandidateIndex;
					int k = 0;
					for (k = 0; k < MAXPOSNUM && k < dlgSearchToolCopyPos.m_vbCopyPos.size(); k++)
					{
						if (dlgSearchToolCopyPos.m_vbCopyPos.at(k) == TRUE && ((k != i)||(nDstCandidateIndex != j)))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
							{
								CString strWarning;
								strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),k+1,nDstCandidateIndex);
								strWarning = strWarning + 
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
								MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
								return S_OK;

							}

							// 检查存储空间是否超过阈值
							if(1)
							{
								if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
								{
									if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
									{
										double dUsedRate = 0.0;
										CString StrTmp = m_strModuleDir;
										m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

										if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchTool != NULL)
										{
											CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
											CString strTmp2;
											strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
											CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

											AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

											CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
											m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
											// 避免后续无法保存的情况

											return S_OK;

										}
									}
								}
							}

							CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
							CProductData product = pTotalProduct->GetItem(nCurProdIdx);

							// 拷贝源为空模板时，将拷贝目标位置的产品执行清空操作
							if (pSearchTool == NULL)
							{
								CameraParamEnable cameraParamEnable;
								cameraParamEnable = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(nDstCandidateIndex);
								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

								if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
								{
									CameraParamSet tmpameraParamSet;
									product.m_pAlignerParam->SetCalibTargetSearchShutter(k,nDstCandidateIndex, tmpameraParamSet);
									pTotalProduct->SetItem(nCurProdIdx, product);
								}
								CBaseSearchTool *pSearchToolDst = NULL;
								pSearchToolDst = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								if (pSearchToolDst == NULL)
								{
									continue;
								}

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnCalibTargetSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								m_vvpObjectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
							}
							else
							{
								CameraParamEnable cameraParamEnable;
								cameraParamEnable = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(j);
								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

								if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
								{
									product.m_pAlignerParam->SetCalibTargetSearchShutter(k,nDstCandidateIndex, product.m_pAlignerParam->GetCalibTargetSearchShutter(i,j));
									pTotalProduct->SetItem(nCurProdIdx, product);
								}

								//获得拷贝之前的窗口 
								scGuiDisplay* pOldDisPlay = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosDisplay(k);

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnCalibTargetSearchTool(nCurProdIdx, k, nDstCandidateIndex, pSearchTool);

								// 显示模板训练图像
								if (pSearchTool != NULL)
								{
									cpImage imagePattern;
									pSearchTool->GetPatternImage(imagePattern);



									m_vvpObjectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
									m_vvpObjectGuiDisplay[k][nDstCandidateIndex]->SetImage(imagePattern, TRUE);
								}
								else
								{
									m_vvpObjectGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
								}

								//设置拷贝之前的窗口 
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosDisplay(k, pOldDisPlay); 
							}

							
							
							// 标记指定位置关联模板已经被修改了			
							m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged[k][nDstCandidateIndex] = TRUE;
							///////////////////////////////////////////////////////////////////////
							
							strLog.Format(_T("将位置%d候选%d关联模板复制到位置%d候选%d\n"),i+1,j,k+1,nDstCandidateIndex);
							g_opLogFile.SaveLogText(strLog,5);

							/////////////////////////////////////////////////////////////////////////
						}
					}


					m_bProdPatternChanged = TRUE;
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}

LRESULT CDlgSysKindAlignCalibTarget::OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0,nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;


	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < ALNCALIBTARGETCANDIDATENUM; j++)
		{
			if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
			{
				CString strLog,strText;

				strLog.Format(_T("点击右键选择[获取定位工具从模板库]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				CDlgSearchToolModelLibrary dlgSearchToolModelLibrary;
				dlgSearchToolModelLibrary.m_vpVisionASM= m_pParent->m_vpVisionASM;
				dlgSearchToolModelLibrary.m_nCurPlatformIndex = m_pParentKind->m_nPlatformIndex;
				dlgSearchToolModelLibrary.SetSysInfoStringsRef(m_psaSysInfoStrings);
				dlgSearchToolModelLibrary.EnableSysKeyboard(m_bSysKeyboardEnabled);

				dlgSearchToolModelLibrary.m_nSearchModeFrom = eCalibTargetSearch;
				dlgSearchToolModelLibrary.m_nPosIndexFrom = i;
				dlgSearchToolModelLibrary.m_nCandidatePosFrom = j;

				if (dlgSearchToolModelLibrary.DoModal() == IDOK)
				{
					CBaseSearchTool *pSearchTool =NULL;
					pSearchTool = dlgSearchToolModelLibrary.GetSelectedSearchTool();
					if(NULL == pSearchTool)
					{
						return S_FALSE;
					}

					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
					{
						CString strWarning;
						strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),i+1,j);
						strWarning = strWarning + 
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
						MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
						return S_FALSE;

					}

					// 检查存储空间是否超过阈值
					if(1)
					{
						if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
							{
								double dUsedRate = 0.0;
								CString StrTmp = m_strModuleDir;
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

								// 从其他模板拷贝到空产品时，需要进行资源管控
								CBaseSearchTool *pSearchToolDstTmp = NULL;
								pSearchToolDstTmp = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,i,j);
								if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchToolDstTmp == NULL)
								{
									CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
									CString strTmp2;
									strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
									CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

									AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

									CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
									m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
									// 避免后续无法保存的情况

									return S_FALSE;

								}
							}
						}
					}


					bool bRet = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnCalibTargetSearchTool(nCurProdIdx,i,j,pSearchTool);
					if(!bRet)
					{
						return S_FALSE;
					}
					cpImage imagePattern;
					bRet = dlgSearchToolModelLibrary.GetSelectedImage(imagePattern);
					if(!bRet)
					{
						return S_FALSE;
					}
					m_vvpObjectGuiDisplay[i][j]->ClearScreen();
					m_vvpObjectGuiDisplay[i][j]->SetImage(imagePattern);

					// 标记产品模板已修改
					m_bProdPatternChanged = TRUE;
					// 标记指定位置目标模板已经被修改了
					m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged[i][j] = TRUE;

					///////////////////////////////////////////////////////////////////////
					dlgSearchToolModelLibrary.GetImageContent(dlgSearchToolModelLibrary.GetSelectImageIndex(),strLog);

					strText.Format(_T("将%s复制到产品%d位置%d候选%d\n"),strLog, m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData()->GetCurProductIndex()
						,i+1,j);
					g_opLogFile.SaveLogText(strText,5);


					/////////////////////////////////////////////////////////////////////////
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}



// 双击Display控件响应
LRESULT CDlgSysKindAlignCalibTarget::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		OnButtonNewProduct();
	}
	int nCurProdIdx = m_nCurProductIdx;
	int i = 0, j = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPatternNum = platformInfo.m_nPositionNum;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	// 
	// 	// Target mark check
	// 	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	// 	{
	// 		for (i=0; i<nPatternNum; i++)
	// 		{
	// 			if (wParam == m_vpTargetGuiDisplay.at(i)->GetID())
	// 			{
	// 				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
	// 				CDlgSearch dlgSearch(this, pSearchTool);
	// 				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
	// 				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
	// 				if (dlgSearch.DoModal() == IDOK)
	// 				{
	// 					pSearchTool = dlgSearch.GetSearchTool();
	// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnTargetSearchToolParam(i, pSearchTool);
	// 
	// 					// 显示模板训练图像
	// 					if (pSearchTool != NULL)
	// 					{
	// 						scImageGray imagePattern;
	// 						pSearchTool->GetPatternImage(imagePattern);
	// 						m_vpTargetGuiDisplay.at(i)->ClearScreen();
	// 						m_vpTargetGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
	// 					}
	// 					else
	// 					{
	// 						m_vpTargetGuiDisplay.at(i)->ClearScreen();
	// 					}
	// 					// 标记产品模板已修改
	// 					m_bProdPatternChanged = TRUE;
	// 				}
	// 				return S_OK;
	// 			}
	// 		}
	// 	}
	// 
	// 	// Virtual Object mark check
	// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	// 	{
	// 		for (i=0; i<nPatternNum; i++)
	// 		{
	// 			if (wParam == m_vpVirtualObjectGuiDisplay.at(i)->GetID())
	// 			{
	// 				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
	// 				CDlgSearch dlgSearch(this, pSearchTool);
	// 				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
	// 				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
	// 				if (dlgSearch.DoModal() == IDOK)
	// 				{
	// 					pSearchTool = dlgSearch.GetSearchTool();
	// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnVirtualSearchToolParam(i, pSearchTool);
	// 
	// 					// 显示模板训练图像
	// 					if (pSearchTool != NULL)
	// 					{
	// 						scImageGray imagePattern;
	// 						pSearchTool->GetPatternImage(imagePattern);
	// 						m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
	// 						m_vpVirtualObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
	// 					}
	// 					else
	// 					{
	// 						m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
	// 					}
	// 					// 标记产品模板已修改
	// 					m_bProdPatternChanged = TRUE;
	// 				}
	// 				return S_OK;
	// 			}
	// 		}
	// 	}

// 	// Object mark check
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		for (i=0; i<nPatternNum; i++)
// 		{
// 			for (j=0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
// 				{
// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnVirtualObjectSearchTool(nCurProdIdx,i,j);
// 					LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
// 					if (m_psaSysInfoStrings != NULL)
// 					{
// 						lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
// 					}
// 					switch (lgidLanguage)
// 					{
// 					case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
// 						if(pSearchTool!=NULL) pSearchTool->SetLanguage(0);
// 						break;
// 					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
// 						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
// 						break;
// 					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
// 					default:
// 						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
// 						break;
// 					}
// 
// 					CDlgSearch dlgSearch(this, pSearchTool);
// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 					if (dlgSearch.DoModal() == IDOK)
// 					{
// 						pSearchTool = dlgSearch.GetSearchTool();
// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnVirtualObjectSearchTool(nCurProdIdx,i, j, pSearchTool);
// 
// 						// 显示模板训练图像
// 						if (pSearchTool != NULL)
// 						{
// 							scImageGray imagePattern;
// 							pSearchTool->GetPatternImage(imagePattern);
// 							
// 							m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 							m_vvpObjectGuiDisplay[i][j]->SetImage(imagePattern, TRUE);
// 						}
// 						else
// 						{
// 							m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 						}
// 						// 标记产品模板已修改
// 						m_bProdPatternChanged = TRUE;
// 					}
// 					return S_OK;
// 				}
// 				// 				if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
// 				// 				{
// 				// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				// 					CDlgSearch dlgSearch(this, pSearchTool);
// 				// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 				// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 				// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 				// 					if (dlgSearch.DoModal() == IDOK)
// 				// 					{
// 				// 						pSearchTool = dlgSearch.GetSearchTool();
// 				// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
// 				// 
// 				// 						// 显示模板训练图像
// 				// 						if (pSearchTool != NULL)
// 				// 						{
// 				// 							scImageGray imagePattern;
// 				// 							pSearchTool->GetPatternImage(imagePattern);
// 				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 							m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 				// 						}
// 				// 						else
// 				// 						{
// 				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				// 						}
// 				// 						// 标记产品模板已修改
// 				// 						m_bProdPatternChanged = TRUE;
// 				// 					}
// 				// 					return S_OK;
// 				// 				}
// 			}
// 
// 		}
// 
// 
// 
// 		// Virtual Object mark check
// 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 		{
// 			for (i=0; i<nPatternNum; i++)
// 			{
// 				for (j=0; j < VIRTUALOBJECTCANDIDATENUM; j++)
// 				{
// 					if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
// 					{
// 						pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnVirtualObjectSearchTool(nCurProdIdx,i,j);
// 						CDlgSearch dlgSearch(this, pSearchTool);
// 						dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 						dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 						dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 						if (dlgSearch.DoModal() == IDOK)
// 						{
// 							pSearchTool = dlgSearch.GetSearchTool();
// 							m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnVirtualObjectSearchTool(nCurProdIdx,i, j, pSearchTool);
// 
// 							// 显示模板训练图像
// 							if (pSearchTool != NULL)
// 							{
// 								scImageGray imagePattern;
// 								pSearchTool->GetPatternImage(imagePattern);
// 								m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 								m_vvpObjectGuiDisplay[i][j]->SetImage(imagePattern, TRUE);
// 							}
// 							else
// 							{
// 								m_vvpObjectGuiDisplay[i][j]->ClearScreen();
// 							}
// 							// 标记产品模板已修改
// 							m_bProdPatternChanged = TRUE;
// 						}
// 						return S_OK;
// 					}
// 					// 				if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
// 					// 				{
// 					// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 					// 					CDlgSearch dlgSearch(this, pSearchTool);
// 					// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 					// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 					// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 					// 					if (dlgSearch.DoModal() == IDOK)
// 					// 					{
// 					// 						pSearchTool = dlgSearch.GetSearchTool();
// 					// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
// 					// 
// 					// 						// 显示模板训练图像
// 					// 						if (pSearchTool != NULL)
// 					// 						{
// 					// 							scImageGray imagePattern;
// 					// 							pSearchTool->GetPatternImage(imagePattern);
// 					// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 					// 							m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 					// 						}
// 					// 						else
// 					// 						{
// 					// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 					// 						}
// 					// 						// 标记产品模板已修改
// 					// 						m_bProdPatternChanged = TRUE;
// 					// 					}
// 					// 					return S_OK;
// 					// 				}
// 				}
// 
// 			}
// 		}
// 
// 	}
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE || (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
	{
		for (i=0; i<nPatternNum; i++)
		{
			for (j=0; j < OBJECTCANDIDATENUM; j++)
			{
				if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
				{
					CString strLog;
					if (j == 0)
					{
						strLog.Format(_T("双击[位置%d]对象模板"),i+1);
						g_opLogFile.SaveLogText(strLog,4);
					}
					else
					{
						strLog.Format(_T("双击[位置%d]候选模板%d"),i+1,j);
						g_opLogFile.SaveLogText(strLog,4);
					}

					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nCurProdIdx,i,j);

					// 检查存储空间是否超过阈值
					if(1)
					{
						if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
							{
								double dUsedRate = 0.0;
								CString StrTmp = m_strModuleDir;
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

								if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) && pSearchTool==NULL)
								{
									CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
									CString strTmp2;
									strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
									CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

									AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！"));

									CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
									m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
									// 避免后续无法保存的情况

									return S_OK;

								}
							}
						}
					}

					LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
					if (m_psaSysInfoStrings != NULL)
					{
						lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
					}
					switch (lgidLanguage)
					{
					case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(0);
						break;
					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
						break;
					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
					default:
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
						break;
					}

					CDlgSearch dlgSearch(this, pSearchTool);
					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);


					CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
					CProductData product = pTotalProduct->GetItem(nCurProdIdx);

					CameraParamEnable cameraParamEnable;
					cameraParamEnable = product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(j);
					BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

					if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
					{
						dlgSearch.SetProductSearchShutterInfo(product.m_pAlignerParam->GetEnableCalibTargetSearchShutter(j) ,product.m_pAlignerParam->GetCalibTargetSearchShutter(i,j));
					}


					if (dlgSearch.DoModal() == IDOK)
					{
						if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable)  && bSystemOptionShutter)
						{
							product.m_pAlignerParam->SetCalibTargetSearchShutter(i,j, dlgSearch.GetProductSearchShutterInfoShutter());
							pTotalProduct->SetItem(nCurProdIdx, product);
						}

						pSearchTool = dlgSearch.GetSearchTool();
						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnCalibTargetSearchTool(nCurProdIdx,i, j, pSearchTool);

						// 显示模板训练图像
						if (pSearchTool != NULL)
						{
							cpImage imagePattern;
							pSearchTool->GetPatternImage(imagePattern);

							m_vvpObjectGuiDisplay[i][j]->ClearScreen();
							m_vvpObjectGuiDisplay[i][j]->SetImage(imagePattern, TRUE);
						}
						else
						{
							m_vvpObjectGuiDisplay[i][j]->ClearScreen();
						}
						// 标记产品模板已修改
						m_bProdPatternChanged = TRUE;
						// 标记指定位置关联模板已经被修改了			
						m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged[i][j] = TRUE;
					}
					return S_OK;
				}
				// 				if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
				// 				{
				// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
				// 					CDlgSearch dlgSearch(this, pSearchTool);
				// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
				// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
				// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
				// 					if (dlgSearch.DoModal() == IDOK)
				// 					{
				// 						pSearchTool = dlgSearch.GetSearchTool();
				// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
				// 
				// 						// 显示模板训练图像
				// 						if (pSearchTool != NULL)
				// 						{
				// 							scImageGray imagePattern;
				// 							pSearchTool->GetPatternImage(imagePattern);
				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 							m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
				// 						}
				// 						else
				// 						{
				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 						}
				// 						// 标记产品模板已修改
				// 						m_bProdPatternChanged = TRUE;
				// 					}
				// 					return S_OK;
				// 				}
			}

		}
	}


	// 	for (i=0; i<nPatternNum; i++)
	// 	{
	// 		if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
	// 		{
	// 			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	// 			CDlgSearch dlgSearch(this, pSearchTool);
	// 			dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 			dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
	// 			dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
	// 			if (dlgSearch.DoModal() == IDOK)
	// 			{
	// 				pSearchTool = dlgSearch.GetSearchTool();
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
	// 				
	// 				// 显示模板训练图像
	// 				if (pSearchTool != NULL)
	// 				{
	// 					scImageGray imagePattern;
	// 					pSearchTool->GetPatternImage(imagePattern);
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 					m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 				// 标记产品模板已修改
	// 				m_bProdPatternChanged = TRUE;
	// 			}
	// 			return S_OK;
	// 		}
	// 	}


	return S_OK ;
}

void CDlgSysKindAlignCalibTarget::OnPlatformChange() 
{
	UpdateData(TRUE);
	// 	if (m_nPlatformIndex == m_nPrevPlatformIdx)
	// 	{
	// 		return;
	// 	}
	// 
	// 	// 保存前一个产品的设置
	// 	if (IsProductChanged())
	// 	{
	// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 		if (nMBReturn == IDYES)
	// 		{
	// 			if (SavePrevProduct())
	// 			{
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 		else if (nMBReturn == IDCANCEL)
	// 		{
	// 			m_nPlatformIndex = m_nPrevPlatformIdx;
	// 			UpdateData(FALSE);
	// 			return;
	// 		}
	// 		else // 不保存对产品的修改
	// 		{
	// 			if (m_bProdNameChanged)
	// 			{
	// 				CString strOldDir, strNewDir;
	// 				CString strCurName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
	// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, m_strProductNameOld);
	// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, strCurName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 				}
	// 				// 如果文件夹存在，则重命名文件夹
	// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 				{
	// 					if (strCurName != m_strProductNameOld)
	// 					{						
	// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 						{
	// 							//AfxMessageBox(_T("重命名失败！"));
	// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 						}
	// 					}
	// 				}
	// 
	// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
	// 			}
	// 			if (m_bProdNewCreate)
	// 			{
	// 				CString strDir;
	// 				CString strProductName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
	// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPrevPlatformIdx, strProductName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 				}
	// 				strDir = m_strModuleDir + strDir;
	// 				if (!RemoveDirectoryPro(strDir))
	// 				{
	// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
	// 				}
	// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->RemoveItem(m_nPrevProductIdx);
	// 			}
	// 
	// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 			{                        
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 			}
	// 
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// 保存产品概要信息到磁盘
	// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
	// 	}

	//////////////////////////////////////////////////////////////////////////
	// 修改新选择的平台的显示界面
	SysPlatformInfo platformInfo;
	int nPosNum = 0, i = 0, j = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;
	// 	// 目标Mark界面修改
	// 	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	// 	{
	// 		for (i=0; i<6; i++)
	// 		{
	// 			if (i < nPosNum)
	// 			{
	// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
	// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
	// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 			}
	// 			else
	// 			{
	// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
	// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
	// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 	}
	// 	else
	// 	{
	// 		for (i=0; i<6; i++)
	// 		{
	// 			GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
	// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 		if (platformInfo.m_bBenchMarkAmend)
	// 		{
	// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 		}
	// 		else
	// 		{
	// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	// 		}
	// 	}
	// 
	// 	// 虚拟对象Mark界面修改
	// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	// 	{
	// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
	// 		for (i=0; i<6; i++)
	// 		{
	// 			if (i < nPosNum)
	// 			{
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 			}
	// 			else
	// 			{
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
	// 		for (i=0; i<6; i++)
	// 		{
	// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 	}

// 	// 对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
// 
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			if (i < MAXPOSNUM -1)
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 			}
// 			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 
// 			for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 
// 
// 
// 	// 虚拟对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);
// 
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_SHOW);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 
// 				for(j = 0; j < VIRTUALOBJECTCANDIDATENUM; j++)
// 				{
// 					//GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_SHOW);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(FALSE);
// 
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			if (i < MAXPOSNUM -1)
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
// 			}
// 			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);
// 
// 
// 			for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
// 				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}


	// 关联标定Mark界面修改
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE || (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
	{
		// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(TRUE);
		// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(TRUE);
		EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				//				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_SHOW);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}

				for(j = 0; j < ALNCALIBTARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowCalibTargetTemplateSet);
				}
			}
			else
			{
				// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
				// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
				m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
	{
		// 		GetDlgItem(IDC_STATIC_VIRTUAL_OBJECTC_SEARCH_MODE)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE0)->EnableWindow(FALSE);
		// 		GetDlgItem(IDC_RADIO_VIRTUAL_OBJECTC_SEARCH_MODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CALIB_TARGET_MARK_ENALBE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CALIB_TARGET_MARK_PATTERN_MODE)->EnableWindow(FALSE);
		EnableAndShowControl(false,false);
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
		for (i=0; i<MAXPOSNUM; i++)
		{
			// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
			m_vpBtnCopyObjectToCalib.at(i)->ShowWindow(SW_HIDE);


			for(j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				GetDlgItem(IDC_STATIC_OBJECTC_PATTERN0+i*OBJECTCANDIDATENUM+j)->ShowWindow(SW_HIDE);
				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
	}
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
	// 	for (i=0; i<6; i++)
	// 	{
	// 		if (i < nPosNum)
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 		}
	// 		else
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 	}

	UpdateDlgData(FALSE);
	//	UpdateProductListDisplay();


	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_nPrevProductIdx = m_nCurProductIdx;



	// 	m_nPrevPlatformIdx = m_nPlatformIndex;
	// 	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	// 	m_nPrevProductIdx = m_nCurProductIdx;
	// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
}



void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerMaxTimes() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_MAX_TIMES)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerMaxTimes();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerXPrecision() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_X_PRECISION)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerXPrecision();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerYPrecision() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_Y_PRECISION)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerYPrecision();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerDPrecision() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_D_PRECISION)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerDPrecision();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerXOffset() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerXOffset();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerYOffset() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerYOffset();
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnSetfocusEditAlignerDOffset() 
{
	// 	if (m_bSysKeyboardEnabled)
	// 	{
	// 		m_bKillFocusEnabled = FALSE;
	// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
	// 
	// 		CRect rcWnd;
	// 		CString strInput, strOutput;
	// 		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET)->GetWindowRect(&rcWnd);
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strInput);
	// 
	// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strOutput);
	// 		}
	// 		m_bKillFocusEnabled = TRUE;
	// 		OnKillfocusEditAlignerDOffset();
	// 	}	
}



void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateXOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateXOffset1();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateYOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateYOffset1();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateDOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateDOffset1();
	}	
}



void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateXOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateXOffset2();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateYOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateYOffset2();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateDOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateDOffset2();
	}	
}



void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateXOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateXOffset3();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateYOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateYOffset3();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateDOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateDOffset3();
	}	
}




void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateXOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateXOffset4();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateYOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateYOffset4();
	}	
}


void CDlgSysKindAlignCalibTarget::OnSetfocusEditObjectCandidateDOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectCandidateDOffset4();
	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerMaxTimes() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		UpdateData(TRUE);
	// 		m_nAlignerMaxTimes = (m_nAlignerMaxTimes<1) ? 1 : ((m_nAlignerMaxTimes>10) ? 10 : m_nAlignerMaxTimes);
	// 		UpdateData(FALSE);
	// 	}
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerXPrecision() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerPrecisionX.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	// 		}
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerYPrecision() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerPrecisionY.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	// 		}
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerDPrecision() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerPrecisionD.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	// 		}
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerXOffset() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerOffsetX.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	// 		}
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerYOffset() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerOffsetY.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	// 		}
	// 	}	
}

void CDlgSysKindAlignCalibTarget::OnKillfocusEditAlignerDOffset() 
{
	// 	if (m_bKillFocusEnabled)
	// 	{
	// 		CString strVal;
	// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strVal);
	// 		if (!IsStringNumerical(strVal))
	// 		{
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	// 		}
	// 		else
	// 		{
	// 			m_strAlignerOffsetD.Format(_T("%.6f"), atof(strVal));
	// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	// 		}
	// 	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateXOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, m_strObjectCandidateOffsetX1);
		}
		else
		{
			m_strObjectCandidateOffsetX1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET1, m_strObjectCandidateOffsetX1);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateYOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, m_strObjectCandidateOffsetY1);
		}
		else
		{
			m_strObjectCandidateOffsetY1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET1, m_strObjectCandidateOffsetY1);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateDOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, m_strObjectCandidateOffsetD1);
		}
		else
		{
			m_strObjectCandidateOffsetD1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET1, m_strObjectCandidateOffsetD1);
		}
	}	
}



void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateXOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, m_strObjectCandidateOffsetX2);
		}
		else
		{
			m_strObjectCandidateOffsetX2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET2, m_strObjectCandidateOffsetX2);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateYOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, m_strObjectCandidateOffsetY2);
		}
		else
		{
			m_strObjectCandidateOffsetY2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET2, m_strObjectCandidateOffsetY2);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateDOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, m_strObjectCandidateOffsetD2);
		}
		else
		{
			m_strObjectCandidateOffsetD2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET2, m_strObjectCandidateOffsetD2);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateXOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, m_strObjectCandidateOffsetX3);
		}
		else
		{
			m_strObjectCandidateOffsetX3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET3, m_strObjectCandidateOffsetX3);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateYOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, m_strObjectCandidateOffsetY3);
		}
		else
		{
			m_strObjectCandidateOffsetY3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET3, m_strObjectCandidateOffsetY3);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateDOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, m_strObjectCandidateOffsetD3);
		}
		else
		{
			m_strObjectCandidateOffsetD3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET3, m_strObjectCandidateOffsetD3);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateXOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, m_strObjectCandidateOffsetX4);
		}
		else
		{
			m_strObjectCandidateOffsetX4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_X_OFFSET4, m_strObjectCandidateOffsetX4);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateYOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, m_strObjectCandidateOffsetY4);
		}
		else
		{
			m_strObjectCandidateOffsetY4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_Y_OFFSET4, m_strObjectCandidateOffsetY4);
		}
	}	
}


void CDlgSysKindAlignCalibTarget::OnKillfocusEditObjectCandidateDOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, m_strObjectCandidateOffsetD4);
		}
		else
		{
			m_strObjectCandidateOffsetD4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_OBJECTC_D_OFFSET4, m_strObjectCandidateOffsetD4);
		}
	}	
}

/************************************************************************************
/* Function name	: OnClickListProduct
/* Description	    : 变更当前产品的消息响应，包括保存前一个当前产品的所有设置和加载
/*					  显示新当前产品的信息，包括模板信息。注：模板信息保存直接写到磁
/*					  盘，点产品管理“取消”时不能取消对模板信息的修改。
/* Return type		: void 
/************************************************************************************/
void CDlgSysKindAlignCalibTarget::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// 	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
	// 	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
	// 	if (nSelected < 0)
	// 	{
	// 		m_listctrlProduct.SetItemState(m_nCurProductIdx, LVIS_SELECTED, LVIS_SELECTED);
	// 	}
	// 	else
	// 	{
	// 		m_nCurProductIdx = nSelected;
	// 	}
	// 
	// 	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
	// 	{
	// 		// 保存前一个产品的设置
	// 		if (IsProductChanged())
	// 		{
	// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 			if (nMBReturn == IDYES)
	// 			{
	// 				if (SavePrevProduct())
	// 				{
	// 					// 恢复修改标志
	// 					m_bProdPatternChanged = FALSE;
	// 					m_bProdNameChanged = FALSE;
	// 					m_bProdBenchMarkChanged = FALSE;
	// 					m_bProdNewCreate = FALSE;
	// 					m_bCalibInfoChanged = FALSE;
	// 				}
	// 			}
	// 			else if (nMBReturn == IDCANCEL)
	// 			{
	// 				m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
	// 				m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
	// 				m_nCurProductIdx = m_nPrevProductIdx;
	// 				return;
	// 			}
	// 			else // 不保存对产品的修改
	// 			{
	// 				if (m_bProdNameChanged)
	// 				{
	// 					CString strOldDir, strNewDir;
	// 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
	// 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
	// 					{
	// 						SysPlatformInfo platformInfo;
	// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 						pVisionASM->GetSysPlatformInfo(platformInfo);
	// 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 					}
	// 					// 如果文件夹存在，则重命名文件夹
	// 					DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 					{
	// 						if (strCurName != m_strProductNameOld)
	// 						{						
	// 							if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 							{
	// 								//AfxMessageBox(_T("重命名失败！"));
	// 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 							}
	// 						}
	// 					}
	// 
	// 					m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
	// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
	// 					//					UpdateDlgData(FALSE);
	// 				}
	// 				if (m_bProdNewCreate)
	// 				{
	// 					CString strDir;
	// 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
	// 					strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 					strDir = m_strModuleDir + strDir;
	// 					if (!RemoveDirectoryPro(strDir))
	// 					{
	// 						//AfxMessageBox(_T("删除产品数据文件失败！"));
	// 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
	// 					}
	// 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
	// 				}
	// 				if (m_bProdNewCreate || m_bProdNameChanged)
	// 				{
	// 					UpdateProductListDisplay();
	// 				}
	// 
	// 				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 				{                        
	// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 				}
	// 
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 
	// 		m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
	// 		m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 		m_nPrevProductIdx = m_nCurProductIdx;
	// 		UpdateDlgData(FALSE);
	// 		m_listctrlProduct.SetItemState(m_nCurProductIdx, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
	// 	}

	*pResult = 0;
}

BOOL CDlgSysKindAlignCalibTarget::DestroyWindow() 
{
	int i = 0 , j = 0;
	//	int nSize = m_vpTargetGuiDisplay.size();
	// 	for (i=0; i<nSize; i++)
	// 	{
	// 		delete m_vpTargetGuiDisplay.at(i);
	// 		m_vpTargetGuiDisplay.at(i) = NULL;
	// 	}
	// 	m_vpTargetGuiDisplay.clear();

	for(i = 0; i < m_vvpObjectGuiDisplay.size(); i++)
	{
		for(j = 0; j < m_vvpObjectGuiDisplay[i].size(); j++)
		{
			delete m_vvpObjectGuiDisplay[i][j];
			m_vvpObjectGuiDisplay[i][j] = NULL;
		}
		m_vvpObjectGuiDisplay[i].clear();
	}
	m_vvpObjectGuiDisplay.clear();

	// 	nSize = m_vpVirtualObjectGuiDisplay.size();
	// 	for (i=0; i<nSize; i++)
	// 	{
	// 		delete m_vpVirtualObjectGuiDisplay.at(i);
	// 		m_vpVirtualObjectGuiDisplay.at(i) = NULL;
	// 	}
	// 	m_vpVirtualObjectGuiDisplay.clear();
	// 
	// 	if (m_pStatusBar != NULL)
	// 	{
	// 		delete m_pStatusBar;
	// 		m_pStatusBar = NULL;
	// 	}
	// 	if (m_hIconSucceed != NULL)
	// 	{
	// 		DestroyIcon(m_hIconSucceed);
	// 		m_hIconSucceed = NULL;
	// 	}
	// 	if (m_hIconFailed != NULL)
	// 	{
	// 		DestroyIcon(m_hIconFailed);
	// 		m_hIconFailed = NULL;
	// 	}

	return CDialog::DestroyWindow();
}

void CDlgSysKindAlignCalibTarget::OnOK() 
{
	// 	if (IsProductChanged())
	// 	{
	// 		if (SavePrevProduct())
	// 		{
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 		else
	// 		{
	// 			//AfxMessageBox(_T("产品保存失败！"));
	// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_SAVE_PRODUCT));
	// 			return;
	// 		}	
	// 	}
	// 	else
	// 	{
	// 		// 保存产品概要信息到磁盘
	// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
	// 	}

	CDialog::OnOK();
}

void CDlgSysKindAlignCalibTarget::OnCancel() 
{
	// 	// 如果当前产品已修改，提示是否保存当前产品的设置
	// 	if (IsProductChanged())
	// 	{
	// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 		if (nMBReturn == IDYES)
	// 		{
	// 			if (SavePrevProduct())
	// 			{
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 		else if (nMBReturn == IDCANCEL)
	// 		{
	// 			return;
	// 		}
	// 		else // 不保存对产品的修改
	// 		{
	// 			if (m_bProdNameChanged)
	// 			{
	// 				CString strOldDir, strNewDir;
	// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 
	// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
	// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 				}
	// 				// 如果文件夹存在，则重命名文件夹
	// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 				{
	// 					if (strCurName != m_strProductNameOld)
	// 					{						
	// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 						{
	// 							//AfxMessageBox(_T("重命名失败！"));
	// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 						}
	// 					}
	// 				}
	// 
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
	// 			}
	// 			if (m_bProdNewCreate)
	// 			{
	// 				CString strDir;
	// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 				}
	// 				strDir = m_strModuleDir + strDir;
	// 				if (!RemoveDirectoryPro(strDir))
	// 				{
	// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
	// 				}
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
	// 			}
	// 
	// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 			{                        
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 			}
	// 
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 	}

	CDialog::OnCancel();
}

void CDlgSysKindAlignCalibTarget::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}


void CDlgSysKindAlignCalibTarget::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}

void CDlgSysKindAlignCalibTarget::CopyObjectToCalib(int nObjectPosIndex)
{
	// TODO: 在此添加控件通知处理程序代码


	CTotalCalibData *pTotalCalibData;
	pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
	CCalibData* pCalibData;
	pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
	if (pCalibData == NULL)
	{
		return;
	}	
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitCalibObjectSearchToolRepository(m_nCurProductIdx);

	CBaseSearchTool* pObjSearchTool = NULL;
	pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(m_nCurProductIdx, nObjectPosIndex, 0);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoCalibObjectSearchTool(m_nCurProductIdx, nObjectPosIndex, 0);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(m_nCurProductIdx, nObjectPosIndex, 0, pObjSearchTool);

	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibObjectSearchToolRepository(m_nCurProductIdx);

	CString strLog;
	strLog.Format(_T("点击[复制到标定]将位置%d复制到标定\n"),nObjectPosIndex+1);
	g_opLogFile.SaveLogText(strLog,4);

	m_bCopyObjectToCalib = TRUE;
	// 标记对象指定位置模板已经修改了
	m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[nObjectPosIndex][0] = true;

}

void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib0()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 0;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 1;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 2;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib3()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 3;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib4()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 4;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib5()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 5;
	CopyObjectToCalib(nObjectPosIndex);
}

void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib6()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 6;
	CopyObjectToCalib(nObjectPosIndex);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnCopyObjectToCalib7()
{
	// TODO: 在此添加控件通知处理程序代码
	int nObjectPosIndex = 7;
	CopyObjectToCalib(nObjectPosIndex);
}



BOOL CDlgSysKindAlignCalibTarget::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysKindAlignCalibTarget::OnBnClickedRadioCalibTargetMarkChange()
{
	// TODO: 在此添加控件通知处理程序代码m_nCalibTargetMarkEnable
	int nCalibTargetMarkEnableOld = m_nCalibTargetMarkEnable;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nCalibTargetMarkEnableOld != m_nCalibTargetMarkEnable)
	{
		CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
		m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
		CString strLog;
		strLog.Format(_T("修改[关联模板]由%s到%s\n"),strVal[nCalibTargetMarkEnableOld],strVal[m_nCalibTargetMarkEnable]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchModeChange()
{
	// TODO: 在此添加控件通知处理程序代码m_nObjectCandidateEnable
	int nObjectCandidateEnableOld = m_nObjectCandidateEnable;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nObjectCandidateEnableOld != m_nObjectCandidateEnable)
	{
		CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
		m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
		CString strLog;
		strLog.Format(_T("修改[候选搜索]由%s到%s\n"),strVal[nObjectCandidateEnableOld],strVal[m_nObjectCandidateEnable]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedRadioObjectcSearchSyncChange()
{
	// TODO: 在此添加控件通知处理程序代码m_nObjectCandidateSync
	int nObjectCandidateSyncOld = m_nObjectCandidateSync;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nObjectCandidateSyncOld != m_nObjectCandidateSync)
	{
		CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
		m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
		CString strLog;
		strLog.Format(_T("修改[成对搜索]由%s到%s\n"),strVal[nObjectCandidateSyncOld],strVal[m_nObjectCandidateSync]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnCbnSelchangeComboCalibTargetMarkPatternMode()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCalibTargetMarkPatternMode;
	m_comboCalibTargetMarkPatternMode.GetWindowText(strCalibTargetMarkPatternMode);

	if (strCalibTargetMarkPatternModeOld != strCalibTargetMarkPatternMode)
	{
		CString strLog;
		strLog.Format(_T("修改[关联模板类型]由%s到%s\n"),strCalibTargetMarkPatternModeOld,strCalibTargetMarkPatternMode);
		g_opLogFile.SaveLogText(strLog,4);
		strCalibTargetMarkPatternModeOld = strCalibTargetMarkPatternMode;
	}


	if (m_comboCalibTargetMarkPatternMode.GetCurSel() == eDmCodeCalib)
	{
		GetDlgItem(IDC_BTN_DMCODE_PARAM_SETTING)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHK_ENABLE_OUTLINE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DMCODE_PARAM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_BTN_DMCODE_PARAM_SETTING)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_ENABLE_OUTLINE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DMCODE_PARAM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PERCENT)->ShowWindow(SW_HIDE);
	}
}
void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bObjectCandidateOffsetEnable1Old =m_bObjectCandidateOffsetEnable1;

	UpdateData(TRUE);
	if (bObjectCandidateOffsetEnable1Old != m_bObjectCandidateOffsetEnable1)
	{
		CString strLog;
		CString strValue = m_bObjectCandidateOffsetEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable2()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bObjectCandidateOffsetEnable2Old =m_bObjectCandidateOffsetEnable2;

	UpdateData(TRUE);
	if (bObjectCandidateOffsetEnable2Old != m_bObjectCandidateOffsetEnable2)
	{
		CString strLog;
		CString strValue = m_bObjectCandidateOffsetEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable3()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bObjectCandidateOffsetEnable3Old =m_bObjectCandidateOffsetEnable3;

	UpdateData(TRUE);
	if (bObjectCandidateOffsetEnable3Old != m_bObjectCandidateOffsetEnable3)
	{
		CString strLog;
		CString strValue = m_bObjectCandidateOffsetEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcOffsetEnable4()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bObjectCandidateOffsetEnable4Old =m_bObjectCandidateOffsetEnable4;

	UpdateData(TRUE);
	if (bObjectCandidateOffsetEnable4Old != m_bObjectCandidateOffsetEnable4)
	{
		CString strLog;
		CString strValue = m_bObjectCandidateOffsetEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcShutterEnable()
{
	// TODO: 在此添加控件通知处理程序代码
    BOOL bAlignerCalibTargetShutterEnable0Old = m_bObjectShutterEnable0;
    BOOL bAlignerCalibTargetShutterEnable1Old = m_bObjectShutterEnable1;
    BOOL bAlignerCalibTargetShutterEnable2Old = m_bObjectShutterEnable2;
    BOOL bAlignerCalibTargetShutterEnable3Old = m_bObjectShutterEnable3;
    BOOL bAlignerCalibTargetShutterEnable4Old = m_bObjectShutterEnable4;
	UpdateData(TRUE);
    if (bAlignerCalibTargetShutterEnable0Old != m_bObjectShutterEnable0)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable0 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerCalibTargetShutterEnable1Old != m_bObjectShutterEnable1)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable1 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置1]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerCalibTargetShutterEnable2Old != m_bObjectShutterEnable2)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable2 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置2]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerCalibTargetShutterEnable3Old != m_bObjectShutterEnable3)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable3 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置3]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerCalibTargetShutterEnable4Old != m_bObjectShutterEnable4)
    {
        CString strLog;
        CString strValue = m_bObjectShutterEnable4 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置4]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
}

void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGainEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerObjectGainEnable0Old = m_bObjectGainEnable0;
	BOOL bAlignerObjectGainEnable1Old = m_bObjectGainEnable1;

	UpdateData(TRUE);
	if (bAlignerObjectGainEnable0Old != m_bObjectGainEnable0)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGainEnable1Old != m_bObjectGainEnable1)
	{
		CString strLog;
		CString strValue =m_bObjectGainEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignCalibTarget::OnBnClickedCheckAlignerObjectcGammaEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerObjectGammaEnable0Old = m_bObjectGammaEnable0;
	BOOL bAlignerObjectGammaEnable1Old = m_bObjectGammaEnable1;

	UpdateData(TRUE);
	if (bAlignerObjectGammaEnable0Old != m_bObjectGammaEnable0)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerObjectGammaEnable1Old != m_bObjectGammaEnable1)
	{
		CString strLog;
		CString strValue =m_bObjectGammaEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignCalibTarget::OnBnClickedBtnDmcodeParamSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();

	if (pTotalProductData->GetCount()<= 0)
	{
		return;
	}

	CString strLog;
	strLog.Format(_T("点击[二位码关联参数]进入[二维码关联参数设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);

	int nCurProductIdx = pTotalProductData->GetCurProductIndex();
	// 获取当前产品对应的标定数据
	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

	scDMCodeParam DmCodeParam;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);

	for (int i=0;i<m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosNum();i++)
	{
		if(optionInfo.m_bEnableSearchShutter)
		{
			DmCodeParam = pProductData->m_vDMcodeCalib.at(i);
			if (DmCodeParam.m_dShutter >= 0.001 )
			{
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SwitchToSearchShutter(i, DmCodeParam.m_dShutter);
			}
		}
		
	}


	CDlgCalibTargetDmcodeParam dlg;
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlg.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));	
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.SetLogLevel(5);
	dlg.DoModal();

	if (optionInfo.m_bEnableSearchShutter == TRUE)
	{
		for (int j=0; j<m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum(); j++)
		{
			// 确保实时采集为环境设置中的曝光时间
			Sleep(10);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateCameraShutter(j);
		}
	}

}


void CDlgSysKindAlignCalibTarget::OnBnClickedChkEnableOutline()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bEnableOutLine)
	{
		GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PERCENT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_OUTLINE_PERCENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PERCENT)->EnableWindow(FALSE);
	}
}


void CDlgSysKindAlignCalibTarget::OnEnSetfocusEditOutlinePercent()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OUTLINE_PERCENT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OUTLINE_PERCENT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OUTLINE_PERCENT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[忽略点百分比]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOutlinePercent();
	}
}


void CDlgSysKindAlignCalibTarget::OnEnKillfocusEditOutlinePercent()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nOutLinePercent = ((m_nOutLinePercent<0 && m_nOutLinePercent >100) ? 0 : m_nOutLinePercent);
		UpdateData(FALSE);
	}
}

void CDlgSysKindAlignCalibTarget::EnableWindowByAuthorizeset(bool bEnable)
{
	SysPlatformInfo platformInfo;
	if (m_nPlatformIndex >=0 && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex) )
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	}

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		pWnd->EnableWindow(bEnable && (platformInfo.m_bTargetObjectCamSeparate == TRUE || (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE)));
		pWnd = pWnd->GetNextWindow();
	}
}