// DlgSysKindCalibSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindCalibSetting.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"

#include "DlgSysKind.h"
#include "DlgSearchToolCopyPos.h"
#include "DlgCalibTargetDmcodeParam.h"
#include "DlgDistorCorrImage.h"
#include "DlgSearchToolModelLibrary.h"
#include "DlgCalibrateEvaluate.h"
#include "DlgCalibResult.h"
//#include "vsBaseAlignTool.h"
// CDlgSysKindCalibSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindCalibSetting, CDialog)

	CDlgSysKindCalibSetting::CDlgSysKindCalibSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindCalibSetting::IDD, pParent)
	, m_nSearchDelayTime(0)
	, m_nRadioPosIndex(0)
	, m_bPos1(FALSE)
	, m_bPos2(FALSE)
	, m_bPos3(FALSE)
	, m_bPos4(FALSE)
	, m_bPos5(FALSE)
	, m_bPos6(FALSE)
	, m_bPos7(FALSE)
	, m_bPos8(FALSE)
	, m_bEnableWholeCalibCacuPos(FALSE)
{
	//{{AFX_DATA_INIT(CDlgSysKindCalibSetting)
	m_strAlignerOffsetD = _T("0.000000");
	m_strAlignerOffsetX = _T("0.000000");
	m_strAlignerOffsetY = _T("0.000000");
	m_strAlignerPrecisionX = _T("0.010000");
	m_strAlignerPrecisionY = _T("0.010000");
	m_strAlignerPrecisionD = _T("0.010000");
	m_nAlignerMaxTimes = 1;
	m_nPlatformIndex = 0;
	m_strCurProductName = _T("");
	m_strCalibMovementX1 = _T("0.5");
	m_strCalibMovementY1 = _T("0.5");
	m_strCalibMovementD1 = _T("0.5");
	m_strCamCalibMovementX1 = _T("0.5");
	m_strCamCalibMovementY1 = _T("0.5");
	m_strCalibRefineMoveD1 = _T("0.5");
	m_strCalibRefineTime1 = _T("0");
	m_strSearchDelayTime1 = _T("30");
	m_strCalibMovementX2 = _T("0.5");
	m_strCalibMovementY2 = _T("0.5");
	m_strCalibMovementD2 = _T("0.5");
	m_strCamCalibMovementX2 = _T("0.5");
	m_strCamCalibMovementY2 = _T("0.5");
	m_strCalibRefineMoveD2 = _T("0.5");
	m_strCalibRefineTime2 = _T("0");
	m_strSearchDelayTime2 = _T("30");
	m_strCalibMovementX3 = _T("0.5");
	m_strCalibMovementY3 = _T("0.5");
	m_strCalibMovementD3 = _T("0.5");
	m_strCamCalibMovementX3 = _T("0.5");
	m_strCamCalibMovementY3 = _T("0.5");
	m_strCalibRefineMoveD3 = _T("0.5");
	m_strCalibRefineTime3 = _T("0");
	m_strSearchDelayTime3 = _T("30");
	m_strCalibMovementX4 = _T("0.5");
	m_strCalibMovementY4 = _T("0.5");
	m_strCalibMovementD4 = _T("0.5");
	m_strCamCalibMovementX4 = _T("0.5");
	m_strCamCalibMovementY4 = _T("0.5");
	m_strCalibRefineMoveD4 = _T("0.5");
	m_strCalibRefineTime4 = _T("0");
	m_strSearchDelayTime4 = _T("30");
	m_strCalibMovementX5 = _T("0.5");
	m_strCalibMovementY5 = _T("0.5");
	m_strCalibMovementD5 = _T("0.5");
	m_strCamCalibMovementX5 = _T("0.5");
	m_strCamCalibMovementY5 = _T("0.5");
	m_strCalibRefineMoveD5 = _T("0.5");
	m_strCalibRefineTime5 = _T("0");
	m_strSearchDelayTime5 = _T("30");
	m_strCalibMovementX6 = _T("0.5");
	m_strCalibMovementY6 = _T("0.5");
	m_strCalibMovementD6 = _T("0.5");
	m_strCamCalibMovementX6 = _T("0.5");
	m_strCamCalibMovementY6 = _T("0.5");
	m_strCalibRefineMoveD6 = _T("0.5");
	m_strCalibRefineTime6 = _T("0");
	m_strSearchDelayTime6 = _T("30");
	m_strCalibMovementX7 = _T("0.5");
	m_strCalibMovementY7 = _T("0.5");
	m_strCalibMovementD7 = _T("0.5");
	m_strCamCalibMovementX7 = _T("0.5");
	m_strCamCalibMovementY7= _T("0.5");
	m_strCalibRefineMoveD7 = _T("0.5");
	m_strCalibRefineTime7 = _T("0");
	m_strSearchDelayTime7 = _T("30");
	m_strCalibMovementX8 = _T("0.5");
	m_strCalibMovementY8 = _T("0.5");
	m_strCalibMovementD8 = _T("0.5");
	m_strCamCalibMovementX8 = _T("0.5");
	m_strCamCalibMovementY8= _T("0.5");
	m_strCalibRefineMoveD8 = _T("0.5");
	m_strCalibRefineTime8 = _T("0");
	m_strSearchDelayTime8 = _T("30");
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
	m_strProductNameOldFa.Empty();
	//	m_pParent = (CVisionASMDlg *)pParent;
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalCalibData.push_back(pTotalProduct);
	// 	}


	m_nSrcProductIndex = 0;
	m_nDstProductIndex = 0;

	m_strCurProductNameFa = _T("");
	m_nSrcProductIndexFa = 0;
	m_nDstProductIndexFa = 0;
	m_nEnableDisCorIndex1 = 0;
	m_nEnableDisCorIndex2 = 0;
	m_nEnableDisCorIndex3 = 0;
	m_nEnableDisCorIndex4 = 0;
	m_nEnableDisCorIndex5 = 0;
	m_nEnableDisCorIndex6 = 0;
	m_nEnableDisCorIndex7 = 0;
	m_nEnableDisCorIndex8 = 0;

	m_bShowXYD						=true;
	m_bEnableXYD					=true;

	m_bShowDistortionCorrection		=true;
	m_bEnableDistortionCorrectionD	=true;
	m_bShowTemplateSet				=true;
	m_bEnableTemplateSet			=true;
	m_bShowCalibrationAlgorithm		=true;
	m_bEnableCalibrationAlgorithm	=true;
	m_bShowDmCodeSet				=true;
	m_bEnableDmCodeSet				=true;

	m_bShowCopyCalib				=true;
	m_bEnableCopyCalib				=true;
	m_bShowCalibList				=true;
	m_bEnableCalibList				=true;

}

CDlgSysKindCalibSetting::~CDlgSysKindCalibSetting()
{
	DestroyWindow();
}

void CDlgSysKindCalibSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysKindCalibSetting)
	//	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrlProduct);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT, m_btnNewProduct);
	DDX_Control(pDX, IDC_BUTTON_DELETE_PRODUCT, m_btnDeleteProduct);
	DDX_Control(pDX, IDC_BTN_PRODUCT_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_COMBO_CALIB_SRC, m_comboProductSrc);
	DDX_Control(pDX, IDC_COMBO_CALIB_DST, m_comboProductDst);
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
	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME, m_strCurProductName);
	//}}AFX_DATA_MAP
	//	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME, m_nSearchDelayTime);
	DDX_Control(pDX, IDC_STATIC_POS0, m_labelPos0);
	DDX_Control(pDX, IDC_STATIC_POS1, m_labelPos1);
	DDX_Control(pDX, IDC_STATIC_POS2, m_labelPos2);
	DDX_Control(pDX, IDC_STATIC_POS3, m_labelPos3);
	DDX_Control(pDX, IDC_STATIC_POS4, m_labelPos4);
	DDX_Control(pDX, IDC_STATIC_POS5, m_labelPos5);
	DDX_Control(pDX, IDC_STATIC_POS6, m_labelPos6);
	DDX_Control(pDX, IDC_STATIC_POS7, m_labelPos7);
	DDX_Check(pDX, IDC_CHECK_CALIB_REFINE, m_bCalibRefine);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT1, m_strCalibMovementX1);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT1, m_strCalibMovementY1);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT1, m_strCalibMovementD1);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, m_strCamCalibMovementX1);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, m_strCamCalibMovementY1);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME1, m_strCalibRefineTime1);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, m_strCalibRefineMoveD1);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME1, m_strSearchDelayTime1);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT2, m_strCalibMovementX2);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT2, m_strCalibMovementY2);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT2, m_strCalibMovementD2);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, m_strCamCalibMovementX2);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, m_strCamCalibMovementY2);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME2, m_strCalibRefineTime2);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, m_strCalibRefineMoveD2);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME2, m_strSearchDelayTime2);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT3, m_strCalibMovementX3);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT3, m_strCalibMovementY3);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT3, m_strCalibMovementD3);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, m_strCamCalibMovementX3);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, m_strCamCalibMovementY3);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME3, m_strCalibRefineTime3);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, m_strCalibRefineMoveD3);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME3, m_strSearchDelayTime3);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT4, m_strCalibMovementX4);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT4, m_strCalibMovementY4);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT4, m_strCalibMovementD4);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, m_strCamCalibMovementX4);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, m_strCamCalibMovementY4);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME4, m_strCalibRefineTime4);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, m_strCalibRefineMoveD4);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME4, m_strSearchDelayTime4);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT5, m_strCalibMovementX5);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT5, m_strCalibMovementY5);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT5, m_strCalibMovementD5);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, m_strCamCalibMovementX5);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, m_strCamCalibMovementY5);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME5, m_strCalibRefineTime5);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, m_strCalibRefineMoveD5);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME5, m_strSearchDelayTime5);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT6, m_strCalibMovementX6);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT6, m_strCalibMovementY6);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT6, m_strCalibMovementD6);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, m_strCamCalibMovementX6);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, m_strCamCalibMovementY6);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME6, m_strCalibRefineTime6);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, m_strCalibRefineMoveD6);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME6, m_strSearchDelayTime6);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT7, m_strCalibMovementX7);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT7, m_strCalibMovementY7);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT7, m_strCalibMovementD7);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, m_strCamCalibMovementX7);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, m_strCamCalibMovementY7);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME7, m_strCalibRefineTime7);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, m_strCalibRefineMoveD7);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME7, m_strSearchDelayTime7);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_MOVEMENT8, m_strCalibMovementX8);
	DDX_Text(pDX, IDC_EDIT_Y_AXIS_MOVEMENT8, m_strCalibMovementY8);
	DDX_Text(pDX, IDC_EDIT_D_AXIS_MOVEMENT8, m_strCalibMovementD8);
	DDX_Text(pDX, IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, m_strCamCalibMovementX8);
	DDX_Text(pDX, IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, m_strCamCalibMovementY8);
	DDX_Text(pDX, IDC_EDIT_REFINE_TIME8, m_strCalibRefineTime8);
	DDX_Text(pDX, IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, m_strCalibRefineMoveD8);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME8, m_strSearchDelayTime8);
	DDX_Control(pDX, IDC_RADIO_POS1, m_radioPos1);
	DDX_Control(pDX, IDC_RADIO_POS2, m_radioPos2);
	DDX_Control(pDX, IDC_RADIO_POS3, m_radioPos3);
	DDX_Control(pDX, IDC_RADIO_POS4, m_radioPos4);
	DDX_Control(pDX, IDC_RADIO_POS5, m_radioPos5);
	DDX_Control(pDX, IDC_RADIO_POS6, m_radioPos6);
	DDX_Control(pDX, IDC_RADIO_POS7, m_radioPos7);
	DDX_Control(pDX, IDC_RADIO_POS8, m_radioPos8);
	DDX_Radio(pDX, IDC_RADIO_POS1, m_nRadioPosIndex);
	DDX_Control(pDX, IDC_CHECK_POS1, m_checkPos1);
	DDX_Control(pDX, IDC_CHECK_POS2, m_checkPos2);
	DDX_Control(pDX, IDC_CHECK_POS3, m_checkPos3);
	DDX_Control(pDX, IDC_CHECK_POS4, m_checkPos4);
	DDX_Control(pDX, IDC_CHECK_POS5, m_checkPos5);
	DDX_Control(pDX, IDC_CHECK_POS6, m_checkPos6);
	DDX_Control(pDX, IDC_CHECK_POS7, m_checkPos7);
	DDX_Control(pDX, IDC_CHECK_POS8, m_checkPos8);
	DDX_Check(pDX, IDC_CHECK_POS1, m_bPos1);
	DDX_Check(pDX, IDC_CHECK_POS2, m_bPos2);
	DDX_Check(pDX, IDC_CHECK_POS3, m_bPos3);
	DDX_Check(pDX, IDC_CHECK_POS4, m_bPos4);
	DDX_Check(pDX, IDC_CHECK_POS5, m_bPos5);
	DDX_Check(pDX, IDC_CHECK_POS6, m_bPos6);
	DDX_Check(pDX, IDC_CHECK_POS7, m_bPos7);
	DDX_Check(pDX, IDC_CHECK_POS8, m_bPos8);
	DDX_Control(pDX, IDC_LIST_PRODUCT_FA, m_listctrlProductFa);
	DDX_Control(pDX, IDC_COMBO_CALIB_SRC_FA, m_comboProductSrcFa);
	DDX_Control(pDX, IDC_COMBO_CALIB_DST_FA, m_comboProductDstFa);
	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME_FA, m_strCurProductNameFa);
	DDX_Control(pDX, IDC_CMB_CALIBMODE, m_comboCalibMarkPatternMode);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX1, m_nEnableDisCorIndex1);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX2, m_nEnableDisCorIndex2);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX3, m_nEnableDisCorIndex3);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX4, m_nEnableDisCorIndex4);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX5, m_nEnableDisCorIndex5);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX6, m_nEnableDisCorIndex6);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX7, m_nEnableDisCorIndex7);
	DDX_Check(pDX, IDC_CHECK_DISTORCORR_INDEX8, m_nEnableDisCorIndex8);
	DDX_Control(pDX, IDC_COMBO_H_DOF, m_comboHomographyDOF);
	DDX_Control(pDX, IDC_COMBO_WHOLE_CALIB_POS, m_cmbWholeCalibCacuPos);
	DDX_Check(pDX, IDC_CHK_WHOLE_CALIB_CACU_POS, m_bEnableWholeCalibCacuPos);
}


BEGIN_MESSAGE_MAP(CDlgSysKindCalibSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysKindCalibSetting)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM1, OnGuiDisplayMenuCustom1)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM2, OnGuiDisplayMenuCustom2)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM3, OnGuiDisplayMenuCustom3)
	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME, OnBtnProductRename)
	//	ON_BN_CLICKED(IDC_BTN_AMEND_BENCHMARK, OnBtnAmendBenchmark)
	//	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT, OnButtonNewProduct)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRODUCT, OnButtonDeleteCurProduct)
	//	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnSetfocusEditAlignerMaxTimes)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnSetfocusEditAlignerXPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnSetfocusEditAlignerYPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnSetfocusEditAlignerDPrecision)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnSetfocusEditAlignerDOffset)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnSetfocusEditAlignerXOffset)
	// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnSetfocusEditAlignerYOffset)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduct)
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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnClickListProduct)
	ON_BN_CLICKED(IDC_BTN_CLONE_PRODUCT, OnBtnCloneProduct)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT1, OnSetfocusEditXAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT1, OnSetfocusEditYAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT1, OnSetfocusEditDAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, OnSetfocusEditCameraXAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, OnSetfocusEditCameraYAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, OnSetfocusEditRefineDAxisMovement1)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME1, OnSetfocusEditRefineTime1)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME1, OnSetfocusEditSearchDelayTime1)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT2, OnSetfocusEditXAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT2, OnSetfocusEditYAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT2, OnSetfocusEditDAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, OnSetfocusEditCameraXAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, OnSetfocusEditCameraYAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, OnSetfocusEditRefineDAxisMovement2)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME2, OnSetfocusEditRefineTime2)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME2,  OnSetfocusEditSearchDelayTime2)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT3, OnSetfocusEditXAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT3, OnSetfocusEditYAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT3, OnSetfocusEditDAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, OnSetfocusEditCameraXAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, OnSetfocusEditCameraYAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, OnSetfocusEditRefineDAxisMovement3)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME3, OnSetfocusEditRefineTime3)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME3,  OnSetfocusEditSearchDelayTime3)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT4, OnSetfocusEditXAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT4, OnSetfocusEditYAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT4, OnSetfocusEditDAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, OnSetfocusEditCameraXAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, OnSetfocusEditCameraYAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, OnSetfocusEditRefineDAxisMovement4)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME4, OnSetfocusEditRefineTime4)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME4,  OnSetfocusEditSearchDelayTime4)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT5, OnSetfocusEditXAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT5, OnSetfocusEditYAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT5, OnSetfocusEditDAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, OnSetfocusEditCameraXAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, OnSetfocusEditCameraYAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, OnSetfocusEditRefineDAxisMovement5)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME5, OnSetfocusEditRefineTime5)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME5,  OnSetfocusEditSearchDelayTime5)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT6, OnSetfocusEditXAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT6, OnSetfocusEditYAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT6, OnSetfocusEditDAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, OnSetfocusEditCameraXAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, OnSetfocusEditCameraYAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, OnSetfocusEditRefineDAxisMovement6)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME6, OnSetfocusEditRefineTime6)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME6,  OnSetfocusEditSearchDelayTime6)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT7, OnSetfocusEditXAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT7, OnSetfocusEditYAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT7, OnSetfocusEditDAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, OnSetfocusEditCameraXAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, OnSetfocusEditCameraYAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, OnSetfocusEditRefineDAxisMovement7)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME7, OnSetfocusEditRefineTime7)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME7,  OnSetfocusEditSearchDelayTime7)
	ON_EN_SETFOCUS(IDC_EDIT_X_AXIS_MOVEMENT8, OnSetfocusEditXAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT8, OnSetfocusEditYAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_D_AXIS_MOVEMENT8, OnSetfocusEditDAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, OnSetfocusEditCameraXAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, OnSetfocusEditCameraYAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, OnSetfocusEditRefineDAxisMovement8)
	ON_EN_SETFOCUS(IDC_EDIT_REFINE_TIME8, OnSetfocusEditRefineTime8)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME8,  OnSetfocusEditSearchDelayTime8)


	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_COPY_CALIB_DATA, &CDlgSysKindCalibSetting::OnBnClickedBtnCopyCalibData)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_SRC, &CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibSrc)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_DST, &CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibDst)
	ON_BN_CLICKED(IDC_BTN_COPY_PATTERN, &CDlgSysKindCalibSetting::OnBnClickedBtnCopyPattern)
	ON_STN_DBLCLK(IDC_STATIC_POS0, &CDlgSysKindCalibSetting::OnDblclkStaticPos0)
	ON_STN_DBLCLK(IDC_STATIC_POS1, &CDlgSysKindCalibSetting::OnDblclkStaticPos1)
	ON_STN_DBLCLK(IDC_STATIC_POS2, &CDlgSysKindCalibSetting::OnDblclkStaticPos2)
	ON_STN_DBLCLK(IDC_STATIC_POS3, &CDlgSysKindCalibSetting::OnDblclkStaticPos3)
	ON_STN_DBLCLK(IDC_STATIC_POS4, &CDlgSysKindCalibSetting::OnDblclkStaticPos4)
	ON_STN_DBLCLK(IDC_STATIC_POS5, &CDlgSysKindCalibSetting::OnDblclkStaticPos5)
	ON_STN_DBLCLK(IDC_STATIC_POS6, &CDlgSysKindCalibSetting::OnDblclkStaticPos6)
	ON_STN_DBLCLK(IDC_STATIC_POS7, &CDlgSysKindCalibSetting::OnDblclkStaticPos7)
	ON_BN_CLICKED(IDC_RADIO_POS1, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS2, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS3, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS4, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS5, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS6, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS7, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_RADIO_POS8, &CDlgSysKindCalibSetting::OnBnClickedRadioPosChange)
	ON_BN_CLICKED(IDC_CHECK_POS1, &CDlgSysKindCalibSetting::OnBnClickedCheckPos1)
	ON_BN_CLICKED(IDC_CHECK_POS2, &CDlgSysKindCalibSetting::OnBnClickedCheckPos2)
	ON_BN_CLICKED(IDC_CHECK_POS3, &CDlgSysKindCalibSetting::OnBnClickedCheckPos3)
	ON_BN_CLICKED(IDC_CHECK_POS4, &CDlgSysKindCalibSetting::OnBnClickedCheckPos4)
	ON_BN_CLICKED(IDC_CHECK_POS5, &CDlgSysKindCalibSetting::OnBnClickedCheckPos5)
	ON_BN_CLICKED(IDC_CHECK_POS6, &CDlgSysKindCalibSetting::OnBnClickedCheckPos6)
	ON_BN_CLICKED(IDC_CHECK_POS7, &CDlgSysKindCalibSetting::OnBnClickedCheckPos7)
	ON_BN_CLICKED(IDC_CHECK_POS8, &CDlgSysKindCalibSetting::OnBnClickedCheckPos8)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT_FA, OnClickListProductFa)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_SRC_FA, &CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibSrcFa)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_DST_FA, &CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibDstFa)
	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME_FA, &CDlgSysKindCalibSetting::OnBtnProductRenameFa)
	ON_BN_CLICKED(IDC_BTN_COPY_CALIB_DATA_FA, &CDlgSysKindCalibSetting::OnBnClickedBtnCopyCalibDataFa)
	ON_BN_CLICKED(IDC_BTN_DMCODE_SETTING, &CDlgSysKindCalibSetting::OnBnClickedBtnDmcodeSetting)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX1, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex1)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX2, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex2)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX3, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex3)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX4, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex4)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX5, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex5)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX6, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex6)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX7, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex7)
	ON_BN_CLICKED(IDC_BTN_DISTORCORR_INDEX8, &CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex8)
	ON_CBN_SELCHANGE(IDC_CMB_CALIBMODE, &CDlgSysKindCalibSetting::OnCbnSelchangeCmbCalibmode)
	ON_CBN_SELCHANGE(IDC_COMBO_H_DOF, &CDlgSysKindCalibSetting::OnCbnSelchangeComboHDof)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX1, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex1)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX2, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex2)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX3, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex3)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX4, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex4)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX5, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex5)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX6, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex6)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX7, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex7)
	ON_BN_CLICKED(IDC_CHECK_DISTORCORR_INDEX8, &CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex8)
	ON_BN_CLICKED(IDC_CHK_WHOLE_CALIB_CACU_POS, &CDlgSysKindCalibSetting::OnBnClickedChkWholeCalibCacuPos)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_X_AXIS_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_Y_AXIS_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_D_AXIS_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME1, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime1)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME2, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime2)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME3, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime3)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME4, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime4)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME5, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime5)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME6, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime6)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME7, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime7)
	ON_EN_KILLFOCUS(IDC_EDIT_REFINE_TIME8, &CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime8)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement1)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement2)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement3)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement4)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement5)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement6)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement7)
	ON_EN_KILLFOCUS(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, &CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement8)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME1, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime1)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME2, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime2)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME3, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime3)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME4, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime4)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME5, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime5)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME6, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime6)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME7, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime7)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME8, &CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime8)
	END_MESSAGE_MAP()


// CDlgSysKindCalibSetting 消息处理程序


BOOL CDlgSysKindCalibSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_bSysKeyboardEnabled = FALSE;

	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	CTotalCalibData *pTotalProduct = NULL;
	for (int i=0; i<nPlatformNum; i++)
	{
		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalCalibData();
		m_vpTotalCalibData.push_back(pTotalProduct);
	}

	m_nPlatformIndex = m_pParentKind->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	// TODO:  在此添加额外的初始化
	CString strModuelDir;
	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
	strModuelDir.ReleaseBuffer();
	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));

	UpdateDialogLanguage();
	InitDlgItem();

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();

	int nPlatformNumNow = m_pParentKind->m_nPlatformIndex;
	int nCurProductIdx = m_vpTotalCalibData.at(nPlatformNumNow)->GetCurCalibIndex();

	int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
	int nCurProductIdxFa = 0;
	m_vpTotalCalibData.at(nPlatformNumNow)->GetDlgCalibIndex(nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);

	m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);
	m_strProductNameOldFa = m_vpTotalCalibData.at(nPlatformNumNow)->GetCalibNameFa(nCurProductIdxFa);	//zzc大产品名称
	
	m_fontChangeProduct.CreatePointFont(180, _T("宋体")); 
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetFont(&m_fontChangeProduct); 
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_HIDE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlgSysKindCalibSetting::InitDlgItem()
{
	int i = 0, j = 0;
	UpdateDlgByAuthorize();
	m_comboCalibMarkPatternMode.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_TEMPLATE_CALIB));//传统模板标定
	m_comboCalibMarkPatternMode.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_DMCODE_CALIB));//二维码标定
	m_comboCalibMarkPatternMode.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_WHOLE_DMCODE_CALIB));//全局二维码靶标标定
	
	m_comboHomographyDOF.InsertString(eAuto,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_HOMOGRAPHYDOF_AUTO));
	m_comboHomographyDOF.InsertString(eScaleRotate,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_HOMOGRAPHYDOF_SCALEROTATE));
	m_comboHomographyDOF.InsertString(eScaleXYRotateXY,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_HOMOGRAPHYDOF_SCALEXYROTATEXY));
	m_comboHomographyDOF.InsertString(eProject,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_HOMOGRAPHYDOF_PROJECT));
	SysPlatformInfo platformInfo;
		
	// 创建产品列表
	CRect rcListRect;
	m_listctrlProduct.GetClientRect(rcListRect);
	m_listctrlProduct.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
	m_listctrlProduct.InsertColumn(0, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_INDEX)/*_T("序号")*/, LVCFMT_LEFT, rcListRect.Width()/4);
	m_listctrlProduct.InsertColumn(1, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME)/*_T("产品名称")*/, LVCFMT_LEFT, rcListRect.Width()*3/4);
	m_listctrlProduct.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_listctrlProductFa.GetClientRect(rcListRect);
	m_listctrlProductFa.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
	m_listctrlProductFa.InsertColumn(0, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_INDEX)/*_T("序号")*/, LVCFMT_LEFT, rcListRect.Width()/4);
	m_listctrlProductFa.InsertColumn(1, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME_FA)/*_T("产品名称大")*/, LVCFMT_LEFT, rcListRect.Width()*3/4);
	m_listctrlProductFa.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	// 
	// 	// Mali 2013.8.16
	// 	m_comboAlignerOffsetCoordType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_COORD));
	// 	m_comboAlignerOffsetCoordType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PLTFM_COORD));

	// 
	m_vpLabelPos.push_back(&m_labelPos0);
	m_vpLabelPos.push_back(&m_labelPos1);
	m_vpLabelPos.push_back(&m_labelPos2);
	m_vpLabelPos.push_back(&m_labelPos3);
	m_vpLabelPos.push_back(&m_labelPos4);
	m_vpLabelPos.push_back(&m_labelPos5);
	m_vpLabelPos.push_back(&m_labelPos6);
	m_vpLabelPos.push_back(&m_labelPos7);

	// 以下操作也可以在资源视图设置Notify属性中实现，但是为了便于升级时统一版本，使用文本形式实现
	GetDlgItem(IDC_STATIC_POS0)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS1)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS2)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS3)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS4)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS5)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS6)->ModifyStyle(0,SS_NOTIFY);
	GetDlgItem(IDC_STATIC_POS7)->ModifyStyle(0,SS_NOTIFY);

	m_vpRadioPos.push_back(&m_radioPos1);
	m_vpRadioPos.push_back(&m_radioPos2);
	m_vpRadioPos.push_back(&m_radioPos3);
	m_vpRadioPos.push_back(&m_radioPos4);
	m_vpRadioPos.push_back(&m_radioPos5);
	m_vpRadioPos.push_back(&m_radioPos6);
	m_vpRadioPos.push_back(&m_radioPos7);
	m_vpRadioPos.push_back(&m_radioPos8);

	m_vpCheckPos.push_back(&m_checkPos1);
	m_vpCheckPos.push_back(&m_checkPos2);
	m_vpCheckPos.push_back(&m_checkPos3);
	m_vpCheckPos.push_back(&m_checkPos4);
	m_vpCheckPos.push_back(&m_checkPos5);
	m_vpCheckPos.push_back(&m_checkPos6);
	m_vpCheckPos.push_back(&m_checkPos7);
	m_vpCheckPos.push_back(&m_checkPos8);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX1);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY1);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD1);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX1);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY1);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD1);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime1);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime1);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX2);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY2);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD2);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX2);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY2);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD2);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime2);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime2);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX3);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY3);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD3);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX3);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY3);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD3);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime3);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime3);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX4);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY4);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD4);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX4);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY4);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD4);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime4);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime4);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX5);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY5);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD5);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX5);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY5);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD5);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime5);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime5);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX6);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY6);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD6);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX6);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY6);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD6);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime6);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime6);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX7);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY7);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD7);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX7);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY7);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD7);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime7);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime7);

	m_vstrCalibMovementX.push_back(&m_strCalibMovementX8);
	m_vstrCalibMovementY.push_back(&m_strCalibMovementY8);
	m_vstrCalibMovementD.push_back(&m_strCalibMovementD8);
	m_vstrCamCalibMovementX.push_back(&m_strCamCalibMovementX8);
	m_vstrCamCalibMovementY.push_back(&m_strCamCalibMovementY8);
	m_vstrCalibRefineMoveD.push_back(&m_strCalibRefineMoveD8);
	m_vstrCalibRefineTime.push_back(&m_strCalibRefineTime8);
	m_vstrSearchDelayTime.push_back(&m_strSearchDelayTime8);

	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex1);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex2);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex3);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex4);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex5);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex6);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex7);
	m_vnEnableDisCor.push_back(&m_nEnableDisCorIndex8);

	m_vdCalibTileX.resize(MAXPOSNUM);
	m_vdCalibTileY.resize(MAXPOSNUM);
	m_vbEnableOutPoints.resize(MAXPOSNUM);
	m_vnThre.resize(MAXPOSNUM);

	// 调整控制板
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	// 设置按钮
	m_btnNewProduct.SetThemeHelper(&m_themeHelper);
	// 	m_btnNewProduct.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnNewProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnDeleteProduct.SetThemeHelper(&m_themeHelper);
	// 	m_btnDeleteProduct.SetIcon(IDI_ICON_DELETE_PRODUCT_48);
	// 	m_btnDeleteProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnRename.SetThemeHelper(&m_themeHelper);
	// 	m_btnRename.SetIcon(IDI_ICON_RENAME_PRODUCT_16);
	// 	m_btnRename.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	// m_btnAmendBenchMark.SetThemeHelper(&m_themeHelper);
	// 	m_btnAmendBenchMark.SetIcon(IDI_ICON_BENCHFIXTURE_40);
	// 	m_btnAmendBenchMark.SetAlign(CButtonST::ST_ALIGN_VERT);


	int nColorOffset = -20;
	CString strText;
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM1, strText);
	// 	//	m_btnShowPlatform1.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform1.SetIcon(m_pParent->m_anIDPlatformIcon[0], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform1.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	//	m_btnShowPlatform1.SetFlat(TRUE);
	// 	m_btnShowPlatform1.SetWindowText(strText);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM2, strText);
	// 	// 	m_btnShowPlatform2.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform2.SetIcon(m_pParent->m_anIDPlatformIcon[1], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform2.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform2.SetFlat(FALSE);
	// 	m_btnShowPlatform2.SetWindowText(strText);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM3, strText);
	// 	// 	m_btnShowPlatform3.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform3.SetIcon(m_pParent->m_anIDPlatformIcon[2], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform3.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform3.SetFlat(FALSE);
	// 	m_btnShowPlatform3.SetWindowText(strText);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM4, strText);
	// 	// 	m_btnShowPlatform4.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform4.SetIcon(m_pParent->m_anIDPlatformIcon[3], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform4.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform4.SetFlat(FALSE);
	// 	m_btnShowPlatform4.SetWindowText(strText);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	m_btnShowPlatform1.SetCheck(1);
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	// 创建scGUIDisplay
	m_vpGuiDisplay.clear();
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<MAXPOSNUM; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_CH0_PATTERN_DISPLAY+i*2, this, WS_BORDER);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
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
		//		pGUIDisplay->SetBackgroundColor(RGB(0,0,0));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CUSTOM1 | SV_DISPLAY_CUSTOM2 | SV_DISPLAY_CUSTOM3);
		m_vpGuiDisplay.push_back(pGUIDisplay);
		pGUIDisplay = NULL;

	}


	// 动态计算并调整对话框大小
	int nMaxCamNum = 0;
	int nClientWidth = 0, nClientHeight = 0;
	int nDispInterval = 0, nGRPIndent = 6;
	int nFrame = 6;
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
	}

	CRect rcWnd, rcClient;
	// 	GetWindowRect(rcWnd);
	// 	GetClientRect(rcClient);
	// 	rcWnd.right = rcWnd.left + (rcWnd.Width() - rcClient.Width()) + nClientWidth;
	// 	rcWnd.bottom = rcWnd.top + (rcWnd.Height() - rcClient.Height()) + nClientHeight;
	// 	MoveWindow(rcWnd);
	// 	GetClientRect(rcClient);

	// 调整Groups
	int nIntendY = 10, nIntervalY = 10;
	// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->GetWindowRect(rcGRP);
	// 	ScreenToClient(rcGRP);
	// 	GetClientRect(rcClient);
	// 	rcGRP.right = rcClient.right - nFrame - nIntendY;
	// 	rcGRP.bottom = rcClient.bottom-nStatusBarHeight-nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->MoveWindow(rcGRP);

	GetClientRect(rcClient);
	// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->MoveWindow(rcItem1);
	// 
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->MoveWindow(rcItem1);
	// 
	// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->MoveWindow(rcItem1);


	// 调整 Control Panel
	int nButtonSize = 75;

	// 调整 Control Panel
	int nButtonSizeWidth = 75;
	int nButtonSizeHeight = 30;
	nIntendY = 0, nIntervalY = 0; 
	int nIntendYOK = 6,nIntervalYOK = 6;

	// 调整产品管理中的按钮大小
	CRect rcRect;
	nIntervalY = 10; 
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->MoveWindow(rcRect);

	GetDlgItem(IDC_BTN_CLONE_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BTN_CLONE_PRODUCT)->MoveWindow(rcRect);

	GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->MoveWindow(rcRect);

	// 调整产品列表大
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.bottom = (rcItem1.top + rcItem2.top - nIntervalY)/2 - nIntervalY/4;
	int tempTop = rcItem1.top;
	GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem1);

	// 调整产品列表
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.bottom = rcItem2.top - nIntervalY;
	rcItem1.top = (tempTop + rcItem2.top - nIntervalY)/2 + nIntervalY/4;
	GetDlgItem(IDC_LIST_PRODUCT)->MoveWindow(rcItem1);

	GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowCalibList);
	GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableCalibList);

	GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.top = rcClient.top;
	rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
	GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem1);

	// 	// 调整产品信息相关
	// 	GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	rcItem1.right = rcClient.right - nFrame;
	// 	GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->MoveWindow(rcItem1);


	GetDlgItem(IDC_STATIC_SEPARATOR3)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.right = rcClient.right;
	GetDlgItem(IDC_STATIC_SEPARATOR3)->MoveWindow(rcItem1);

	GetDlgItem(IDC_STATIC_SEPARATOR11)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.right = rcClient.right;
	GetDlgItem(IDC_STATIC_SEPARATOR11)->MoveWindow(rcItem1);

	//////////////////////////////////////////////////////////////////////////
	// 根据默认平台调整界面
	//SysPlatformInfo platformInfo;
	//	int nPosNum = 0, i = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;

	int nShow = SW_SHOW;
	if (!m_bShowXYD)
	{
		nShow =SW_HIDE;
	}

	GetDlgItem(IDC_STATIC_DST_POS)->EnableWindow(m_bEnableCopyCalib);
	GetDlgItem(IDC_STATIC_DST_POS)->ShowWindow(m_bShowCopyCalib);

	GetDlgItem(IDC_STATIC_COPY_TO_POS)->EnableWindow(m_bEnableCopyCalib);
	GetDlgItem(IDC_STATIC_COPY_TO_POS)->ShowWindow(m_bShowCopyCalib);

	GetDlgItem(IDC_STATIC_SRC_POS)->EnableWindow(m_bEnableCopyCalib);
	GetDlgItem(IDC_STATIC_SRC_POS)->ShowWindow(m_bShowCopyCalib);

	GetDlgItem(IDC_STATIC_COPY_TO_PRO)->EnableWindow(m_bEnableCopyCalib);
	GetDlgItem(IDC_STATIC_COPY_TO_PRO)->ShowWindow(m_bShowCopyCalib);


	int nShowCopyCalib = SW_SHOW;
	// 对象Mark界面修改		
	for (i=0; i<MAXPOSNUM; i++)
	{
		if (i < nPosNum)
		{	
			if (m_bShowXYD==false)
			{
				nShow = SW_HIDE;
			}
			GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(m_bShowTemplateSet);
			GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->EnableWindow(m_bEnableTemplateSet);
			m_vpGuiDisplay.at(i)->ShowWindow(m_bShowTemplateSet);
			m_vpGuiDisplay.at(i)->EnableWindow(m_bEnableTemplateSet);

			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(nShow);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));

			if (m_bShowCopyCalib==false)
			{
				nShowCopyCalib = SW_HIDE;
			}

			m_vpLabelPos.at(i)->ShowWindow(nShow);
			m_vpLabelPos.at(i)->EnableWindow(m_bEnableXYD);

			m_vpRadioPos.at(i)->ShowWindow(nShowCopyCalib);
			m_vpCheckPos.at(i)->ShowWindow(nShowCopyCalib);
			m_vpRadioPos.at(i)->EnableWindow(m_bEnableCopyCalib);
			m_vpCheckPos.at(i)->EnableWindow(m_bEnableCopyCalib);

			for(j = 0; j < 16; j++)
			{
				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(nShow);
			}

			if (platformInfo.m_eCamPlatformType == eCamPlatformSepFix)
			{
				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
			}
			else if (platformInfo.m_eCamPlatformType == eCamPlatformSepX)
			{
				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
			}
			else if (platformInfo.m_eCamPlatformType == eCamPlatformSepXY)
			{
				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
			}
			else if (platformInfo.m_eCamPlatformType == eCamPlatformShareX)
			{
				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
			}	

			int nShowDistortionCorrection =(m_bShowDistortionCorrection)?SW_SHOW:SW_HIDE;

			BOOL bEnableDisCor = FALSE;
			CTotalCalibData *pTotalCalibData;
			if(m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)!=NULL)
			{
				pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
				if (pTotalCalibData != NULL)
				{
					CCalibData* pCalibData;
					pCalibData = pTotalCalibData->GetItemPtr(pTotalCalibData->GetCurCalibIndex());
					if (pCalibData != NULL && i<pCalibData->m_vpCalibrateParam.size())
					{
						bEnableDisCor = pCalibData->m_vpCalibrateParam.at(i)->m_nEnableDisCor;
					}
				}
			}



			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(nShowDistortionCorrection);
			GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(nShowDistortionCorrection);

			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->EnableWindow(m_bEnableDistortionCorrectionD);
			GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->EnableWindow(m_bEnableDistortionCorrectionD);


		}
		else
		{
			GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(SW_HIDE);
			m_vpGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
			m_vpRadioPos.at(i)->ShowWindow(SW_HIDE);
			m_vpCheckPos.at(i)->ShowWindow(SW_HIDE);
			for(j = 0; j < 16; j++)
			{
				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(SW_HIDE);
			}
			
			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
		}
	}
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	
	if (optionInfo.m_bEnableDmCodeCalib)
	{
		GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(m_bShowDmCodeSet);
		GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(m_bEnableDmCodeSet);
	}
	else
	{
		GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(FALSE);
	}
	
	
	BOOL bPlatformType = (platformInfo.m_eMidPlatformType != ePlatformXD ||
		platformInfo.m_eMidPlatformType != ePlatformYD || 
		platformInfo.m_eMidPlatformType != ePlatformX ||
		platformInfo.m_eMidPlatformType != ePlatformY ||
		platformInfo.m_eMidPlatformType != ePlatformXDPY);


	GetDlgItem(IDC_COMBO_H_DOF)->ShowWindow(m_bShowCalibrationAlgorithm && bPlatformType);
	GetDlgItem(IDC_COMBO_H_DOF)->EnableWindow(m_bEnableCalibrationAlgorithm && bPlatformType);
	GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(m_bShowCalibrationAlgorithm &&bPlatformType);
	GetDlgItem(IDC_STATIC_H_DOF)->EnableWindow(m_bEnableCalibrationAlgorithm &&bPlatformType);

	GetDlgItem(IDC_LIST_PRODUCT_FA)->ShowWindow(m_bShowCalibList);
	GetDlgItem(IDC_LIST_PRODUCT_FA)->EnableWindow(m_bEnableCalibList);


	// 保存一部分控件的位置信息，以便于单双层结构之间的切换
	CRect tmpRect;

	// 按钮
	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_PRODUCT_RENAME_FA,tmpRect));

	GetDlgItem(IDC_BTN_PRODUCT_RENAME)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_PRODUCT_RENAME,tmpRect));

	GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_COPY_CALIB_DATA_FA,tmpRect));

	GetDlgItem(IDC_BTN_COPY_CALIB_DATA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_COPY_CALIB_DATA,tmpRect));

	// 下拉框
	GetDlgItem(IDC_COMBO_CALIB_SRC_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_CALIB_SRC_FA,tmpRect));

	GetDlgItem(IDC_COMBO_CALIB_SRC)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_CALIB_SRC,tmpRect));

	GetDlgItem(IDC_COMBO_CALIB_DST_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_CALIB_DST_FA,tmpRect));

	GetDlgItem(IDC_COMBO_CALIB_DST)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_CALIB_DST,tmpRect));

	// 编辑框
	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CUR_PRODUCT_NAME_FA,tmpRect));

	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->EnableWindow(m_bEnableXYD);
	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->ShowWindow(/*m_bShowXYD*/false);

	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->EnableWindow(m_bEnableXYD);
	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(/*m_bShowXYD*/false);

	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->EnableWindow(m_bEnableCalibList);
	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->ShowWindow(/*m_bShowCalibList*/false);

	GetDlgItem(IDC_BTN_PRODUCT_RENAME)->EnableWindow(m_bEnableCalibList && platformInfo.m_bProductFaChd);
	GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(/*m_bShowCalibList && platformInfo.m_bProductFaChd*/false);

	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CUR_PRODUCT_NAME,tmpRect));

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindCalibSetting::MoveDlgItem()
{
	
	if (GetDlgItem(IDC_STATIC_AXIS_X1)->GetSafeHwnd() != NULL)
	{
		UpdateDlgByAuthorize();
		int i = 0, j = 0;

		// 创建产品列表
		CRect rcListRect;

		// 调整控制板
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());
		
		
		int nColorOffset = -20;
		CString strText;

		// 动态计算并调整对话框大小
		int nMaxCamNum = 0;
		int nClientWidth = 0, nClientHeight = 0;
		int nDispInterval = 0, nGRPIndent = 6;
		int nFrame = 6;
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
		CRect rcWnd, rcClient;
		// 	GetWindowRect(rcWnd);
		// 	GetClientRect(rcClient);
		// 	rcWnd.right = rcWnd.left + (rcWnd.Width() - rcClient.Width()) + nClientWidth;
		// 	rcWnd.bottom = rcWnd.top + (rcWnd.Height() - rcClient.Height()) + nClientHeight;
		// 	MoveWindow(rcWnd);
		// 	GetClientRect(rcClient);

		// 调整Groups
		int nIntendY = 10, nIntervalY = 10;
		// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->GetWindowRect(rcGRP);
		// 	ScreenToClient(rcGRP);
		// 	GetClientRect(rcClient);
		// 	rcGRP.right = rcClient.right - nFrame - nIntendY;
		// 	rcGRP.bottom = rcClient.bottom-nStatusBarHeight-nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->MoveWindow(rcGRP);

		GetClientRect(rcClient);
		// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->MoveWindow(rcItem1);
		// 
		// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->MoveWindow(rcItem1);
		// 
		// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->MoveWindow(rcItem1);


		// 调整 Control Panel
		int nButtonSize = 75;

		// 调整 Control Panel
		int nButtonSizeWidth = 75;
		int nButtonSizeHeight = 30;
		nIntendY = 0, nIntervalY = 0; 
		int nIntendYOK = 6,nIntervalYOK = 6;

		// 调整产品管理中的按钮大小
		CRect rcRect;
		nIntervalY = 10; 
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->MoveWindow(rcRect);

		GetDlgItem(IDC_BTN_CLONE_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BTN_CLONE_PRODUCT)->MoveWindow(rcRect);

		GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->MoveWindow(rcRect);

		// 调整产品列表大
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
		ScreenToClient(rcItem2);
		GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.bottom = (rcItem1.top + rcItem2.top - nIntervalY)/2 - nIntervalY/4;
		int tempTop = rcItem1.top;
		GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem1);

		// 调整产品列表
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
		ScreenToClient(rcItem2);
		GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.bottom = rcItem2.top - nIntervalY;
		rcItem1.top = (tempTop + rcItem2.top - nIntervalY)/2 + nIntervalY/4;
		GetDlgItem(IDC_LIST_PRODUCT)->MoveWindow(rcItem1);

		GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowCalibList);
		GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableCalibList);

		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		int nShowCopyCalib =(m_bShowCopyCalib)?SW_SHOW:SW_HIDE;

		GetDlgItem(IDC_STATIC_DST_POS)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_STATIC_DST_POS)->ShowWindow(m_bShowCopyCalib);

		GetDlgItem(IDC_STATIC_COPY_TO_POS)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_STATIC_COPY_TO_POS)->ShowWindow(m_bShowCopyCalib);

		CPlatformOptionInfo optionInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);

		if (optionInfo.m_bEnableDmCodeCalib)
		{
			GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(m_bShowDmCodeSet);
			GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(m_bEnableDmCodeSet);
		}
		else
		{
			GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(FALSE);
		}



		for(int it=0;it<MAXPOSNUM && i<8;it++)
		{
			if(it<platformInfo.m_nPositionNum)
			{
				GetDlgItem(IDC_RADIO_POS1+it)->EnableWindow(m_bEnableCopyCalib);
				GetDlgItem(IDC_RADIO_POS1+it)->ShowWindow(m_bShowCopyCalib);

				GetDlgItem(IDC_CHECK_POS1+it)->EnableWindow(m_bEnableCopyCalib);
				GetDlgItem(IDC_CHECK_POS1+it)->ShowWindow(m_bShowCopyCalib);



			}
			else
			{
				GetDlgItem(IDC_RADIO_POS1+it)->EnableWindow(FALSE);
				GetDlgItem(IDC_RADIO_POS1+it)->ShowWindow(FALSE);

				GetDlgItem(IDC_CHECK_POS1+it)->EnableWindow(FALSE);
				GetDlgItem(IDC_CHECK_POS1+it)->ShowWindow(FALSE);

			}
		}
		
		GetDlgItem(IDC_STATIC_SRC_POS)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_STATIC_SRC_POS)->ShowWindow(m_bShowCopyCalib);

		GetDlgItem(IDC_STATIC_COPY_TO_PRO)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_STATIC_COPY_TO_PRO)->ShowWindow(m_bShowCopyCalib);


		GetDlgItem(IDC_LIST_PRODUCT_FA)->ShowWindow(m_bShowCalibList);
		GetDlgItem(IDC_LIST_PRODUCT_FA)->EnableWindow(m_bEnableCalibList);

		GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowCalibList);
		GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableCalibList);
		//GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(SW_SHOW);



		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->ShowWindow(/*m_bShowCalibList*/false);
		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->EnableWindow(/*m_bEnableCalibList*/false);
		GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->ShowWindow(/*m_bShowXYD*/false);


		GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->ShowWindow(nShowCopyCalib);
		GetDlgItem(IDC_COMBO_CALIB_SRC_FA)->ShowWindow(nShowCopyCalib);
		GetDlgItem(IDC_COMBO_CALIB_DST_FA)->ShowWindow(nShowCopyCalib);

		GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_COMBO_CALIB_SRC_FA)->EnableWindow(m_bEnableCopyCalib);
		GetDlgItem(IDC_COMBO_CALIB_DST_FA)->EnableWindow(m_bEnableCopyCalib);


		GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->EnableWindow(/*m_bEnableXYD*/false);
		GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->ShowWindow(/*m_bShowXYD*/false);

		GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->EnableWindow(/*m_bEnableXYD*/false);
		GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(/*m_bShowXYD*/false);

		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->EnableWindow(/*m_bEnableCalibList*/false);
		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->ShowWindow(/*m_bShowCalibList*/false);

		GetDlgItem(IDC_BTN_PRODUCT_RENAME)->EnableWindow(false/*m_bEnableCalibList && platformInfo.m_bProductFaChd*/);
		GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(false/*m_bShowCalibList && platformInfo.m_bProductFaChd*/);


		if(platformInfo.m_bProductFaChd)
		{
			// 双层结构

			// 显示小产品控件



			GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowCalibList);
			GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableCalibList);
			//GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(SW_SHOW);

			

			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(/*nShowCopyCalib*/false);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA)->ShowWindow(nShowCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_SRC)->ShowWindow(nShowCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_DST)->ShowWindow(nShowCopyCalib);
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(/*m_bShowXYD*/false);

			GetDlgItem(IDC_BTN_COPY_CALIB_DATA)->EnableWindow(m_bEnableCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_SRC)->EnableWindow(m_bEnableCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_DST)->EnableWindow(m_bEnableCopyCalib);


			//GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(SW_SHOW);

			// 修改产品列表大的列名
			LVCOLUMN m_vcolumn;
			CString strText = GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME_FA)/*_T("产品名称大")*/;
			m_vcolumn.mask = LVCF_TEXT;
			m_vcolumn.pszText =strText.GetBuffer(0);
			m_vcolumn.cchTextMax = strText.GetLength();
			m_listctrlProductFa.SetColumn(1,&m_vcolumn);

			// 按钮相关
			map<int, CRect>::iterator iter;
			iter = m_InitPosition.find(IDC_BTN_PRODUCT_RENAME_FA);
			if(iter != m_InitPosition.end())
			{
				GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->MoveWindow(iter->second);
			}
		}
		else
		{
			// 单层结构

			// 隐藏小产品控件，只保留大产品控件
			GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(HIDE_WINDOW);
			//GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_COMBO_CALIB_SRC)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_COMBO_CALIB_DST)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(HIDE_WINDOW);
			//GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(HIDE_WINDOW);

			
			GetDlgItem(IDC_COMBO_CALIB_SRC_FA)->ShowWindow(nShowCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_DST_FA)->ShowWindow(nShowCopyCalib);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->ShowWindow(nShowCopyCalib);
			
			GetDlgItem(IDC_COMBO_CALIB_SRC_FA)->EnableWindow(m_bEnableCopyCalib);
			GetDlgItem(IDC_COMBO_CALIB_DST_FA)->EnableWindow(m_bEnableCopyCalib);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->EnableWindow(m_bEnableCopyCalib);

			// 把小产品控件名称赋值给大产品控件
			CString tempString;
			//GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowTextA(tempString);
			//GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->SetWindowTextA(tempString);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->GetWindowText(tempString);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->SetWindowText(tempString);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA)->GetWindowText(tempString);
			GetDlgItem(IDC_BTN_COPY_CALIB_DATA_FA)->SetWindowText(tempString);

			// 修改产品列表大的列名
			LVCOLUMN m_vcolumn;
			CString strText = GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME)/*_T("产品名称")*/;
			m_vcolumn.mask = LVCF_TEXT;
			m_vcolumn.pszText =strText.GetBuffer(0);
			m_vcolumn.cchTextMax = strText.GetLength();
			m_listctrlProductFa.SetColumn(1,&m_vcolumn);

			// 调整产品列表大
			GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			rcItem2.bottom = rcItem1.bottom;
			GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem2);

			// 调整按钮重命名大
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			int temDst = rcItem2.left - rcItem1.left;
			rcItem1.left+=temDst;
			rcItem1.right+=temDst;
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->MoveWindow(rcItem1);
		}
		
		GetDlgItem(IDC_BTN_COPY_PATTERN)->ShowWindow(nShowCopyCalib);
		GetDlgItem(IDC_BTN_COPY_PATTERN)->EnableWindow(m_bEnableCopyCalib);

		GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.top = rcClient.top;
		rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem1);

		// 		// 调整产品信息相关
		// 		GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->GetWindowRect(rcItem1);
		// 		ScreenToClient(rcItem1);
		// 		rcItem1.right = rcClient.right - nFrame;
		// 		GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->MoveWindow(rcItem1);


		GetDlgItem(IDC_STATIC_SEPARATOR3)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.right = rcClient.right;
		GetDlgItem(IDC_STATIC_SEPARATOR3)->MoveWindow(rcItem1);

		GetDlgItem(IDC_STATIC_SEPARATOR11)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.right = rcClient.right;
		GetDlgItem(IDC_STATIC_SEPARATOR11)->MoveWindow(rcItem1);

		// 开放标定算法选项：针对非以下平台类型均可进行标定算法切换
		BOOL bPlatformType = (platformInfo.m_eMidPlatformType != ePlatformXD ||
			platformInfo.m_eMidPlatformType != ePlatformYD || 
			platformInfo.m_eMidPlatformType != ePlatformX ||
			platformInfo.m_eMidPlatformType != ePlatformY ||
			platformInfo.m_eMidPlatformType != ePlatformXDPY);
			//if (sysPlatformInfo.m_bPlatformPick)
		{
			m_comboHomographyDOF.ShowWindow(m_bShowCalibrationAlgorithm && bPlatformType);
			GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(m_bShowCalibrationAlgorithm &&bPlatformType);
		}

		GetDlgItem(IDC_COMBO_H_DOF)->EnableWindow(m_bEnableCalibrationAlgorithm && bPlatformType);
		GetDlgItem(IDC_STATIC_H_DOF)->EnableWindow(m_bEnableCalibrationAlgorithm && bPlatformType);
		//else
		//{
		//	m_comboHomographyDOF.ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(SW_HIDE);
		//}

		//////////////////////////////////////////////////////////////////////////
		// 根据默认平台调整界面
//		SysPlatformInfo platformInfo;
		//	int nPosNum = 0, i = 0;
//		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
		//nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		nPosNum = platformInfo.m_nPositionNum;
		int nShow = SW_SHOW;
		if (!m_bShowXYD)
		{
			nShow =SW_HIDE;
		}
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{	
				if (m_bShowXYD==false)
				{
					nShow = SW_HIDE;
				}
				GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(m_bShowTemplateSet);
				GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->EnableWindow(m_bEnableTemplateSet);
				m_vpGuiDisplay.at(i)->ShowWindow(m_bShowTemplateSet);
				m_vpGuiDisplay.at(i)->EnableWindow(m_bEnableTemplateSet);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(nShow);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(nShow);

				m_vpRadioPos.at(i)->ShowWindow(m_bShowCopyCalib);
				m_vpRadioPos.at(i)->EnableWindow(m_bEnableCopyCalib);

				m_vpCheckPos.at(i)->ShowWindow(m_bShowCopyCalib);
				m_vpCheckPos.at(i)->EnableWindow(m_bEnableCopyCalib);

				for(j = 0; j < 16; j++)
				{
					GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(nShow);
				}

				if (platformInfo.m_eCamPlatformType == eCamPlatformSepFix)
				{
					GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(FALSE);
					GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
					GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
					GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
				}
				else if (platformInfo.m_eCamPlatformType == eCamPlatformSepX)
				{
					GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(TRUE);
					GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
					GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
				}
				else if (platformInfo.m_eCamPlatformType == eCamPlatformSepXY)
				{
					GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
				}
				else if (platformInfo.m_eCamPlatformType == eCamPlatformShareX)
				{
					GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(m_bEnableXYD);
					GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
					GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
				}	

				BOOL bEnableDisCor = FALSE;
				CTotalCalibData *pTotalCalibData;
				if(m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)!=NULL)
				{
					pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
					if (pTotalCalibData != NULL)
					{
						CCalibData* pCalibData;
						pCalibData = pTotalCalibData->GetItemPtr(pTotalCalibData->GetCurCalibIndex());
						if (pCalibData != NULL && i<pCalibData->m_vpCalibrateParam.size())
						{
							bEnableDisCor = pCalibData->m_vpCalibrateParam.at(i)->m_nEnableDisCor;
						}
					}
				}

				// 获取当前产品对应的标定数据



				int nShowDistortionCorrection =(m_bShowDistortionCorrection)?SW_SHOW:SW_HIDE;
				GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(nShowDistortionCorrection);
				GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(nShowDistortionCorrection);
				GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->EnableWindow(m_bEnableDistortionCorrectionD && bEnableDisCor);
				GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->EnableWindow(m_bEnableDistortionCorrectionD);
			}
			else
			{
				GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(SW_HIDE);
				m_vpGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
				for(j = 0; j < 16; j++)
				{
					GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(SW_HIDE);
				}

				GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->EnableWindow(FALSE);
				GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->EnableWindow(FALSE);
			}
		}
	}

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindCalibSetting::CheckDlgData()
{
	UpdateData(TRUE);

	// 对位次数
	m_nAlignerMaxTimes  = (m_nAlignerMaxTimes > 10) ? 10 : ((m_nAlignerMaxTimes < 1) ? 1 : m_nAlignerMaxTimes);
}

void CDlgSysKindCalibSetting::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);

	int nCurProdIdx = 0;
	CCalibData product;

	if (bSaveAndValidate) // 从界面获取信息
	{
		nCurProdIdx = pTotalProduct->GetCurCalibIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		pTotalProduct->SetCurCalibIndex(nCurProdIdx);
		product = pTotalProduct->GetItem(nCurProdIdx);

		product.SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
		product.SetCalibHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());
		product.SetWholeCalibCacuPos(m_cmbWholeCalibCacuPos.GetCurSel());
		product.SetWholeCalibCacuPosEnable(m_bEnableWholeCalibCacuPos);
		
		CCoordPos pos;
		int i=0; 
		CString strInfo;
		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
		{
			if (product.m_vpCalibrateParam.at(i) == NULL)
			{
				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
			}

			pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
			pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
			pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibMovement(pos);

			pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
			pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
			pos.m_dAngle = 0.0;
			product.m_vpCalibrateParam.at(i)->SetCameraCalibMovement(pos);

			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineMoveD(atof(*m_vstrCalibRefineMoveD.at(i)));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineTime(atoi(*m_vstrCalibRefineTime.at(i)));
			product.m_vpCalibrateParam.at(i)->SetSearchDelayTime(atoi(*m_vstrSearchDelayTime.at(i)));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibrateRefine(m_bCalibRefine);

			product.m_vpCalibrateParam.at(i)->m_dTileX = m_vdCalibTileX.at(i);
			product.m_vpCalibrateParam.at(i)->m_dTileY = m_vdCalibTileY.at(i);
			product.m_vpCalibrateParam.at(i)->m_bOutPoints = m_vbEnableOutPoints.at(i);
			product.m_vpCalibrateParam.at(i)->m_iThre = m_vnThre.at(i);
			product.m_vpCalibrateParam.at(i)->m_nEnableDisCor = *m_vnEnableDisCor.at(i);

			product.m_vpCalibrateParam.at(i)->SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());

		}

		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else //从存储的数据更新界面
	{
		BeginWaitCursor();

		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurCalibIndex();

		if (nProductNum > 0)
		{
			if (nCurProdIdx < 0)
			{
				nCurProdIdx = 0;
			}
			else if (nCurProdIdx >= nProductNum)
			{
				nCurProdIdx = nProductNum - 1;
			}
		}
		else
		{
			nCurProdIdx = -1;
		}
		pTotalProduct->SetCurCalibIndex(nCurProdIdx);

		// 从总产品信息中获取当前产品信息，如果总产品数为0，则增加一个空产品
		if (nCurProdIdx < 0)
		{
			SysPlatformInfo platformInfo;
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
			product = CCalibData(platformInfo);
			CString strNewName = GetNewDefName();
			pTotalProduct->AddItem(product);
			pTotalProduct->SetItemName(0, strNewName);
			pTotalProduct->SetCurCalibIndex(0);
			pTotalProduct->SetCalibIndex(0,0,0);
			nCurProdIdx = pTotalProduct->GetCurCalibIndex();

			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCurProdIdx);

			m_bProdNewCreate = TRUE;
			m_strProductNameOld = strNewName;
			m_strProductNameOldFa = strNewName;
		}
		product = pTotalProduct->GetItem(nCurProdIdx);


		//m_bLoadSingleProductSearchToolEnable优先级高于m_bLoadSingleProductSearchToolEnableFaChd，所以在单产品下，不会加载整个大产品
		// 启用大小品种且只加载单个大产品时，加载当前品种对应大品种下的其他全部小品种
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd && !sysPlatformInfo.m_bLoadSingleProductSearchToolEnable)
		{
			int nProductIndexFa = 0;
			int nProductIndexChd = 0;
			pTotalProduct->GetDlgCalibIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);

			int nProductCountChd = pTotalProduct->GetCalibCountChd(nProductIndexFa);
			for (int i =0; i<nProductCountChd; i++)
			{
				int nProductIndex = pTotalProduct->GetCalibIndex(nProductIndexFa, i);
				if (nProductIndex == nCurProdIdx) continue;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nProductIndex, TRUE, TRUE);
			}
		}

		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nCurProdIdx, TRUE, TRUE); // 更新当前视觉对位工具（加载定位工具）

		int nProductIndexFa, nProductIndexChd;
		pTotalProduct->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
		m_strCurProductNameFa.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalProduct->GetCalibNameFa(nProductIndexFa));
		m_strCurProductName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalProduct->GetItemName(nCurProdIdx));

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;

		m_comboCalibMarkPatternMode.SetCurSel((int)product.GetCalibMarkPatternMode());
		m_comboHomographyDOF.SetCurSel((int)product.GetCalibHomographyDOF());

		SysPlatformInfo platformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
		m_cmbWholeCalibCacuPos.ResetContent();
		for (int i=0;i<platformInfo.m_nPositionNum;i++)
		{
			CString strInfo;
			strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1+i);
			//strInfo.Format(_T("位置%d"),i+1);
			m_cmbWholeCalibCacuPos.InsertString(i,strInfo);
		}
		m_cmbWholeCalibCacuPos.SetCurSel(product.GetWholeCalibCacuPos());

		// 开放标定算法选项：针对非以下平台类型均可进行标定算法切换
		BOOL bPlatformType = (sysPlatformInfo.m_eMidPlatformType != ePlatformXD ||
			sysPlatformInfo.m_eMidPlatformType != ePlatformYD || 
			sysPlatformInfo.m_eMidPlatformType != ePlatformX ||
			sysPlatformInfo.m_eMidPlatformType != ePlatformY ||
			sysPlatformInfo.m_eMidPlatformType != ePlatformXDPY);
		//if (sysPlatformInfo.m_bPlatformPick)
		{
			m_comboHomographyDOF.ShowWindow(m_bShowCalibrationAlgorithm && bPlatformType);
			GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(m_bShowCalibrationAlgorithm &&bPlatformType);
		}
		//else
		//{
		//	m_comboHomographyDOF.ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(SW_HIDE);
		//}

		CPlatformOptionInfo optionInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
		if((product.GetCalibMarkPatternMode()==eCalibUseDmCode || product.GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib) && optionInfo.m_bEnableDmCodeCalib)
		{
			GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(m_bShowDmCodeSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(SW_HIDE);
		}

		if (product.GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib && optionInfo.m_bEnableDmCodeCalib)
		{
			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(m_bShowDmCodeSet);
			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(m_bShowDmCodeSet);

			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(m_bEnableDmCodeSet);
			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->EnableWindow(m_bEnableDmCodeSet);

		}
		else
		{
			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(FALSE);
		}

		m_bEnableWholeCalibCacuPos = product.GetWholeCalibCacuPosEnable();
		if (m_bEnableWholeCalibCacuPos)
		{
			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(m_bEnableDmCodeSet);
		}
		else
		{
			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(FALSE);
		}


		CCoordPos pos;
		int i=0; 
		CString strInfo;
		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
		{
			if (product.m_vpCalibrateParam.at(i) == NULL)
			{
				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
			}

			pos = product.m_vpCalibrateParam.at(i)->GetPlatformCalibMovement();
			strInfo.Format(_T("%.3f"),pos.m_dPosX);
			*m_vstrCalibMovementX.at(i) = strInfo;
			strInfo.Format(_T("%.3f"),pos.m_dPosY);
			*m_vstrCalibMovementY.at(i) = strInfo;
			strInfo.Format(_T("%.3f"),pos.m_dAngle);
			*m_vstrCalibMovementD.at(i) = strInfo;

			pos = product.m_vpCalibrateParam.at(i)->GetCameraCalibMovement();
			strInfo.Format(_T("%.3f"),pos.m_dPosX);
			*m_vstrCamCalibMovementX.at(i) = strInfo;
			strInfo.Format(_T("%.3f"),pos.m_dPosY);
			*m_vstrCamCalibMovementY.at(i) = strInfo;

			strInfo.Format(_T("%.3f"),product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineMoveD());
			*m_vstrCalibRefineMoveD.at(i) = strInfo;
			strInfo.Format(_T("%d"),product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineTime());
			*m_vstrCalibRefineTime.at(i) = strInfo;
			strInfo.Format(_T("%d"),product.m_vpCalibrateParam.at(i)->GetSearchDelayTime());
			*m_vstrSearchDelayTime.at(i) = strInfo;
			m_bCalibRefine = product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineTime();

			m_vdCalibTileX.at(i) = product.m_vpCalibrateParam.at(i)->m_dTileX;
			m_vdCalibTileY.at(i) = product.m_vpCalibrateParam.at(i)->m_dTileY;
			m_vbEnableOutPoints.at(i) = product.m_vpCalibrateParam.at(i)->m_bOutPoints;
			m_vnThre.at(i) = product.m_vpCalibrateParam.at(i)->m_iThre;
			*m_vnEnableDisCor.at(i) = product.m_vpCalibrateParam.at(i)->m_nEnableDisCor;
			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1+i)->EnableWindow((BOOL)(product.m_vpCalibrateParam.at(i)->m_nEnableDisCor) && m_bEnableDistortionCorrectionD );

			m_comboCalibMarkPatternMode.SetCurSel((int)product.m_vpCalibrateParam.at(i)->GetCalibMarkPatternMode());

		}
		nProductNum = pTotalProduct->GetCount();

		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
//		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
// 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
// 		{
// 			int nMBReturn = IDNO;
// 			if (!m_bProdNewCreate)
// 			{
// 				CString strInfo = _T("该产品的平台标定结果与系统的标定结果不同。");
// 				strInfo += _T("\n使用该产品的标定结果更新系统标定结果，请点击“是”；");
// 				strInfo += _T("\n使用系统标定结果更新该产品的标定结果，请点击“否”。");
// 				nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
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
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
// 
// 				m_bCalibInfoChanged = TRUE;
// 			}
// 		}


		int j = 0;
		// 目标Mark模板图像显示
		for (i=0; i<MAXPOSNUM; i++)
		{		
			m_vpGuiDisplay.at(i)->ClearScreen();
		}
		CBaseSearchTool* pCalibSearchTool = NULL;
		cpImage image;
		//SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		int nPatternNum = platformInfo.m_nPositionNum;
		for (i=0; i<nPatternNum; i++)
		{
			pCalibSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, i, 0);
			BOOL bExistSearchTool = IsSearchToolSelected(pCalibSearchTool);
			if (pCalibSearchTool != NULL && bExistSearchTool)
			{
				image.Release();
				pCalibSearchTool->GetPatternImage(image);
				m_vpGuiDisplay.at(i)->ClearScreen();
				m_vpGuiDisplay.at(i)->SetImage(image, TRUE);
			}
			else
			{
				m_vpGuiDisplay.at(i)->ClearScreen();
			}
		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}

// void CDlgSysKindCalibSetting::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
// {
// 	ASSERT(GetSafeHwnd() != NULL);
// 	if (GetSafeHwnd() == NULL)
// 	{
// 		return;
// 	}
// 
// 	UpdateData(TRUE);
// 	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// 	ASSERT(pTotalProduct != NULL);
// 	int nCurProdIdx = 0;
// 	CCalibData product;
// 	if (bSaveAndValidate) // 从界面获取信息
// 	{
// 
// 		nCurProdIdx = pTotalProduct->GetCurCalibIndex();
// 		if (nCurProdIdx >= pTotalProduct->GetCount())
// 		{
// 			return;
// 		}
// 		pTotalProduct->SetCurCalibIndex(nCurProdIdx);
// 		product = pTotalProduct->GetItem(nCurProdIdx);
// 		product.SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 		product.SetCalibHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());
// 		product.SetWholeCalibCacuPos(m_cmbWholeCalibCacuPos.GetCurSel());
// 		product.SetWholeCalibCacuPosEnable(m_bEnableWholeCalibCacuPos);
// 		//		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
// 		CCoordPos pos;
// 		// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
// 		// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
// 		// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
// 		// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
// 		// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
// 		// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
// 		// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
// 		// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
// 		// 		product.m_pAlignerParam->SetAlignerOffset(pos);
// 
// 		int i=0; 
// 		CString strInfo;
// 		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
// 		{
// 			if (product.m_vpCalibrateParam.at(i) == NULL)
// 			{
// 				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
// 			}
// 
// 			pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
// 			pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibMovement(pos);
// 			pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
// 			pos.m_dAngle = 0.0;
// 			product.m_vpCalibrateParam.at(i)->SetCameraCalibMovement(pos);
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineMoveD(atof(*m_vstrCalibRefineMoveD.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineTime(atoi(*m_vstrCalibRefineTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetSearchDelayTime(atoi(*m_vstrSearchDelayTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibrateRefine(m_bCalibRefine);
// 
// 			product.m_vpCalibrateParam.at(i)->m_dTileX = m_vdCalibTileX.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_dTileY = m_vdCalibTileY.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_bOutPoints = m_vbEnableOutPoints.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_iThre = m_vnThre.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_nEnableDisCor = *m_vnEnableDisCor.at(i);
// 			product.m_vpCalibrateParam.at(i)->SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 		}
// 
// // 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
// // 		for (i=0; i<product.m_vpCalibratedInfo.size(); i++)
// // 		{
// // 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// // 		}
// 		pTotalProduct->SetItem(nCurProdIdx, product);
// 	}
// 	else //从存储的数据更新界面
// 	{
// 		BeginWaitCursor();
// 
// 		// 用存储的数据精度、补偿、检测更新界面显示
// 
// 		// 用存储的数据目标搜索信息更新界面显示
// 
// 		// 用存储的数据对象搜索信息更新界面显示
// 
// 		int nProductNum = pTotalProduct->GetCount();
// 		nCurProdIdx = pTotalProduct->GetCurCalibIndex();
// 
// 		// ShiYue 20190228
// // 		{
// // 			SysPlatformInfo sysPlatformInfo;
// // 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// // 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// // 			{
// // 				CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// // 				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// // 				if (pPreProduct != NULL && m_nPrevProductIdx != nCurProdIdx)
// // 				{
// // 					pPreProduct->DeleteObjectSearchTools();
// // 				}
// // 			}
// // 		}
// 
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
// 		pTotalProduct->SetCurCalibIndex(nCurProdIdx);
// 		// 从总产品信息中获取当前产品信息，如果总产品数为0，则增加一个空产品
// 		if (nCurProdIdx < 0)
// 		{
// 			SysPlatformInfo platformInfo;
// 			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 			product = CCalibData(platformInfo);
// 			CString strNewName = GetNewDefName();
// 			pTotalProduct->AddItem(product);
// 			pTotalProduct->SetItemName(0, strNewName);
// 			pTotalProduct->SetCurCalibIndex(0);
// 			pTotalProduct->SetCalibIndex(0,0,0);
// 			nCurProdIdx = pTotalProduct->GetCurCalibIndex();
// 
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCurProdIdx);
// 
// 			m_bProdNewCreate = TRUE;
// 			m_strProductNameOld = strNewName;
// 			m_strProductNameOldFa = strNewName;
// 		}
// 		product = pTotalProduct->GetItem(nCurProdIdx);
// 
// 
// 		// 20190228
// 		// 释放原来产品的定位工具20181204-mfq
// // 		{
// // 			SysPlatformInfo sysPlatformInfo;
// // 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// // 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// // 			{
// // 				CTotalProductData *pTotalProduct = NULL;
// // 				pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
// // 				CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// // 				if (pPreProduct != NULL)
// // 				{
// // 					pPreProduct->DeleteObjectSearchTools();
// // 					pPreProduct->DeleteTargetSearchTools();
// // 					pPreProduct->DeleteVirtualObjectSearchTools();
// // 					pPreProduct->DeleteCalibTargetSearchTools();
// // 					pPreProduct->DeleteInspectSearchTools();
// // 				}
// // 			}
// // 		}
// 		//m_bLoadSingleProductSearchToolEnable优先级高于m_bLoadSingleProductSearchToolEnableFaChd，所以在单产品下，不会加载整个大产品
// 		// 启用大小品种且只加载单个大产品时，加载当前品种对应大品种下的其他全部小品种
// 		SysPlatformInfo sysPlatformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 		if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd && !sysPlatformInfo.m_bLoadSingleProductSearchToolEnable)
// 		{
// 			int nProductIndexFa = 0;
// 			int nProductIndexChd = 0;
// 			pTotalProduct->GetDlgCalibIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);
// 
// 			int nProductCountChd = pTotalProduct->GetCalibCountChd(nProductIndexFa);
// 			for (int i =0; i<nProductCountChd; i++)
// 			{
// 				int nProductIndex = pTotalProduct->GetCalibIndex(nProductIndexFa, i);
// 				if (nProductIndex == nCurProdIdx) continue;
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nProductIndex, TRUE, TRUE);
// 			}
// 		}
// 
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nCurProdIdx, TRUE, TRUE); // 更新当前视觉对位工具（加载定位工具）
// 		// m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);
// //		m_strCurProductName.Format(_T("%03d|%s"), nCurProdIdx+1, pTotalProduct->GetItemName(nCurProdIdx));
// 
// 		int nProductIndexFa, nProductIndexChd;
// 		pTotalProduct->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
// 		m_strCurProductNameFa.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalProduct->GetCalibNameFa(nProductIndexFa));
// 		m_strCurProductName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalProduct->GetItemName(nCurProdIdx));
// 
// 
// 		m_nCurProductIdx = nCurProdIdx;
// 		m_nPrevProductIdx = m_nCurProductIdx;
// 		//		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
// 		//		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
// 		// 		m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
// 		// 		m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
// 		// 		m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());
// 		// 		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
// 		// 		pos = product.m_pAlignerParam->GetAlignerOffset();
// 		// 		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
// 		// 		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
// 		// 		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
// 		// 		m_nSearchDelayTime = product.m_pAlignerParam->GetSearchDelayTime();
// 		m_comboCalibMarkPatternMode.SetCurSel((int)product.GetCalibMarkPatternMode());
// 		m_comboHomographyDOF.SetCurSel((int)product.GetCalibHomographyDOF());
// 
// 		SysPlatformInfo platformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
// 		m_cmbWholeCalibCacuPos.ResetContent();
// 		for (int i=0;i<platformInfo.m_nPositionNum;i++)
// 		{
// 			CString strInfo;
// 			strInfo = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1+i);
// 			//strInfo.Format(_T("位置%d"),i+1);
// 			m_cmbWholeCalibCacuPos.InsertString(i,strInfo);
// 		}
// 		m_cmbWholeCalibCacuPos.SetCurSel(product.GetWholeCalibCacuPos());
// 
// 		/*SysPlatformInfo platformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);*/
// 		if (sysPlatformInfo.m_bPlatformPick)
// 		{
// 			m_comboHomographyDOF.ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			m_comboHomographyDOF.ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_H_DOF)->ShowWindow(SW_HIDE);
// 		}
// 
// 		CPlatformOptionInfo optionInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
// 		if((product.GetCalibMarkPatternMode()==eCalibUseDmCode || product.GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib) && optionInfo.m_bEnableDmCodeCalib)
// 		{
// 			GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(SW_HIDE);
// 		}
// 
// 		if (product.GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib && optionInfo.m_bEnableDmCodeCalib)
// 		{
// 			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(SW_HIDE);
// 		}
// 		m_bEnableWholeCalibCacuPos = product.GetWholeCalibCacuPosEnable();
// 		if (m_bEnableWholeCalibCacuPos)
// 		{
// 			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(TRUE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(FALSE);
// 		}
// 
// 
// 		CCoordPos pos;
// 		int i=0; 
// 		CString strInfo;
// 		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
// 		{
// 			if (product.m_vpCalibrateParam.at(i) == NULL)
// 			{
// 				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
// 			}
// 
// 			pos = product.m_vpCalibrateParam.at(i)->GetPlatformCalibMovement();
// 			strInfo.Format(_T("%.3f"),pos.m_dPosX);
// 			*m_vstrCalibMovementX.at(i) = strInfo;
// 			strInfo.Format(_T("%.3f"),pos.m_dPosY);
// 			*m_vstrCalibMovementY.at(i) = strInfo;
// 			strInfo.Format(_T("%.3f"),pos.m_dAngle);
// 			*m_vstrCalibMovementD.at(i) = strInfo;
// 
// 			pos = product.m_vpCalibrateParam.at(i)->GetCameraCalibMovement();
// 			strInfo.Format(_T("%.3f"),pos.m_dPosX);
// 			*m_vstrCamCalibMovementX.at(i) = strInfo;
// 			strInfo.Format(_T("%.3f"),pos.m_dPosY);
// 			*m_vstrCamCalibMovementY.at(i) = strInfo;
// 
// 			strInfo.Format(_T("%.3f"),product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineMoveD());
// 			*m_vstrCalibRefineMoveD.at(i) = strInfo;
// 			strInfo.Format(_T("%d"),product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineTime());
// 			*m_vstrCalibRefineTime.at(i) = strInfo;
// 			strInfo.Format(_T("%d"),product.m_vpCalibrateParam.at(i)->GetSearchDelayTime());
// 			*m_vstrSearchDelayTime.at(i) = strInfo;
// 			m_bCalibRefine = product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineTime();
// 
// 			m_vdCalibTileX.at(i) = product.m_vpCalibrateParam.at(i)->m_dTileX;
// 			m_vdCalibTileY.at(i) = product.m_vpCalibrateParam.at(i)->m_dTileY;
// 			m_vbEnableOutPoints.at(i) = product.m_vpCalibrateParam.at(i)->m_bOutPoints;
// 			m_vnThre.at(i) = product.m_vpCalibrateParam.at(i)->m_iThre;
// 			*m_vnEnableDisCor.at(i) = product.m_vpCalibrateParam.at(i)->m_nEnableDisCor;
// 			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1+i)->EnableWindow((BOOL)(product.m_vpCalibrateParam.at(i)->m_nEnableDisCor));
// 		}
// 		nProductNum = pTotalProduct->GetCount();
// 
// 		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
// //		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
// // 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
// // 		{
// // 			int nMBReturn = IDNO;
// // 			if (!m_bProdNewCreate)
// // 			{
// // 				CString strInfo = _T("该产品的平台标定结果与系统的标定结果不同。");
// // 				strInfo += _T("\n使用该产品的标定结果更新系统标定结果，请点击“是”；");
// // 				strInfo += _T("\n使用系统标定结果更新该产品的标定结果，请点击“否”。");
// // 				nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
// // 			}
// // 
// // 			if (nMBReturn == IDYES)	// 用产品标定结果代替当前平台标定结果
// // 			{
// // 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(product.m_vpCalibratedInfo);
// // 			}
// // 			else if (nMBReturn == IDNO)	// 用当前平台标定结果代替产品标定结果
// // 			{
// // 				int j = 0;
// // 				for (j=0; j<product.m_vpCalibratedInfo.size(); j++)
// // 				{
// // 					if (product.m_vpCalibratedInfo.at(j) != NULL)
// // 					{
// // 						delete product.m_vpCalibratedInfo.at(j);
// // 						product.m_vpCalibratedInfo.at(j) = NULL;
// // 					}
// // 				}
// // 				int nNewSize = vpCurCalibratedInfo.size();
// // 				product.m_vpCalibratedInfo.resize(nNewSize, NULL);
// // 				for (j=0; j<nNewSize; j++)
// // 				{
// // 					if (vpCurCalibratedInfo.at(j) != NULL)
// // 					{
// // 						product.m_vpCalibratedInfo.at(j) = new CCalibratedInfo(*vpCurCalibratedInfo.at(j));
// // 					}
// // 				}
// // 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
// // 
// // 				m_bCalibInfoChanged = TRUE;
// // 			}
// // 		}
// 
// 
// 		int j = 0;
// 		// 目标Mark模板图像显示
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{		
// 			m_vpGuiDisplay.at(i)->ClearScreen();
// 		}
// 		CBaseSearchTool* pCalibSearchTool = NULL;
// 		cpImage image;
// 		//SysPlatformInfo platformInfo;
// 		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// //		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		int nPatternNum = platformInfo.m_nPositionNum;
// 		for (i=0; i<nPatternNum; i++)
// 		{
// 			pCalibSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, i, 0);
// 			BOOL bExistSearchTool = IsSearchToolSelected(pCalibSearchTool);
// 			if (pCalibSearchTool != NULL && bExistSearchTool)
// 			{
// 				image.Release();
// 				pCalibSearchTool->GetPatternImage(image);
// 				m_vpGuiDisplay.at(i)->ClearScreen();
// 				m_vpGuiDisplay.at(i)->SetImage(image, TRUE);
// 			}
// 			else
// 			{
// 				m_vpGuiDisplay.at(i)->ClearScreen();
// 			}
// 		}
// 
// 		UpdateData(FALSE);
// 		EndWaitCursor();
// 	}
// }


void CDlgSysKindCalibSetting::UpdateDlgPatternData(BOOL bSaveAndValidate/* = TRUE*/)
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

//	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CCalibData product;
	if (bSaveAndValidate) // 从界面获取信息
	{
	}
	else //从存储的数据更新界面
	{
		BeginWaitCursor();

		// 用存储的数据精度、补偿、检测更新界面显示

		// 用存储的数据目标搜索信息更新界面显示

		// 用存储的数据对象搜索信息更新界面显示

		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurCalibIndex();	
		product = pTotalProduct->GetItem(nCurProdIdx);

		// m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nCurProdIdx, TRUE); // 更新当前视觉对位工具（加载定位工具）
	
		int i=0; 
	
		int j = 0;
		// 目标Mark模板图像显示
		CBaseSearchTool* pCalibSearchTool = NULL;
		cpImage image;
		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		int nPatternNum = platformInfo.m_nPositionNum;
		for (i=0; i<nPatternNum; i++)
		{
			pCalibSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, i, 0);
			if (pCalibSearchTool != NULL)
			{
				image.Release();
				pCalibSearchTool->GetPatternImage(image);
				m_vpGuiDisplay.at(i)->ClearScreen();
				m_vpGuiDisplay.at(i)->SetImage(image, TRUE);
			}
			else
			{
				m_vpGuiDisplay.at(i)->ClearScreen();
			}
		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}


void CDlgSysKindCalibSetting::UpdateDlgTempData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	// 	ASSERT(GetSafeHwnd() != NULL);
	// 	if (GetSafeHwnd() == NULL)
	// 	{
	// 		return;
	// 	}
	// 
	// 	UpdateData(TRUE);
	// 	CTotalProductData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	// 	ASSERT(pTotalProduct != NULL);
	// 	int nCurProdIdx = 0;
	// 	CProductData product;
	// 	if (bSaveAndValidate) // 从界面获取信息
	// 	{
	// 		// 
	// 		// 		// 从界面获取精度、补偿、检测等信息
	// 		// 
	// 		// 		// 从界面获取目标搜索信息
	// 		// 
	// 		// 		// 从界面获取对象搜索信息
	// 		// 
	// 		// 		nCurProdIdx = pTotalProduct->GetCurProductIndex();
	// 		// 		if (nCurProdIdx >= pTotalProduct->GetCount())
	// 		// 		{
	// 		// 			return;
	// 		// 		}
	// 		// 		pTotalProduct->SetCurProductIndex(nCurProdIdx);
	// 		// 		product = pTotalProduct->GetItem(nCurProdIdx);
	// 		// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
	// 		// 		CCoordPos pos;
	// 		// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
	// 		// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
	// 		// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
	// 		// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
	// 		// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
	// 		// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
	// 		// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
	// 		// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
	// 		// 		product.m_pAlignerParam->SetAlignerOffset(pos);
	// 		// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
	// 		// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
	// 		// 		{
	// 		// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
	// 		// 		}
	// 		// 		pTotalProduct->SetItem(nCurProdIdx, product);
	// 	}
	// 	else //从存储的数据更新界面
	// 	{
	// 		BeginWaitCursor();
	// 
	// 
	// 
	// 
	// 		// 用存储的数据对象搜索信息更新界面显示
	// 
	// 		int nProductNum = pTotalProduct->GetCount();
	// 		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
	// 		product = pTotalProduct->GetItem(nCurProdIdx);
	// 
	// 		// 用存储的数据精度、补偿、检测更新界面显示
	// 		m_pParentKind->m_pDlgKindAlignSetting->GetDlgTempData(product);	
	// 		m_pParentKind->m_pDlgKindAlignTarget->GetDlgTempData(product);	
	// 		m_pParentKind->m_pDlgKindAlignObject->GetDlgTempData(product);	
	// 
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
	// 		m_nSearchDelayTime = product.m_pAlignerParam->GetSearchDelayTime();
	// 
	// 		if (product.m_pAlignerParam->GetTargetCandidateEnable())
	// 		{
	// 			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,_T("有效"));
	// 		}
	// 		else
	// 		{
	// 			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,_T("无效"));
	// 		}
	// 
	// 		if (product.m_pAlignerParam->GetTargetCandidateSyncEnable())
	// 		{
	// 			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,_T("有效"));
	// 		}
	// 		else
	// 		{
	// 			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,_T("无效"));
	// 		}
	// 
	// 
	// 		if (product.m_pAlignerParam->GetObjectCandidateEnable())
	// 		{
	// 			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,_T("有效"));
	// 		}
	// 		else
	// 		{
	// 			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,_T("无效"));
	// 		}
	// 
	// 		if (product.m_pAlignerParam->GetObjectCandidateSyncEnable())
	// 		{
	// 			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,_T("有效"));
	// 		}
	// 		else
	// 		{
	// 			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,_T("无效"));
	// 		}
	// 
	// 		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
	// 		// 		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
	// 		// 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
	// 		// 		{
	// 		// 			int nMBReturn = IDNO;
	// 		// 			if (!m_bProdNewCreate)
	// 		// 			{
	// 		// 				// 				CString strInfo = _T("该产品所使用的平台标定结果与当前平台的标定结果不同。");
	// 		// 				// 				strInfo += _T("\n仍然要使用该产品的标定结果，请点击“是”；");
	// 		// 				// 				strInfo += _T("\n应用当前平台的标定结果到该产品，请点击“否”。");
	// 		// 				nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIBRATED_INFO_VARIED_PLEASE_SELECT), MB_YESNO);
	// 		// 			}
	// 		// 
	// 		// 			if (nMBReturn == IDYES)	// 用产品标定结果代替当前平台标定结果
	// 		// 			{
	// 		// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(product.m_vpCalibratedInfo);
	// 		// 			}
	// 		// 			else if (nMBReturn == IDNO)	// 用当前平台标定结果代替产品标定结果
	// 		// 			{
	// 		// 				int j = 0;
	// 		// 				for (j=0; j<product.m_vpCalibratedInfo.size(); j++)
	// 		// 				{
	// 		// 					if (product.m_vpCalibratedInfo.at(j) != NULL)
	// 		// 					{
	// 		// 						delete product.m_vpCalibratedInfo.at(j);
	// 		// 						product.m_vpCalibratedInfo.at(j) = NULL;
	// 		// 					}
	// 		// 				}
	// 		// 				int nNewSize = vpCurCalibratedInfo.size();
	// 		// 				product.m_vpCalibratedInfo.resize(nNewSize, NULL);
	// 		// 				for (j=0; j<nNewSize; j++)
	// 		// 				{
	// 		// 					if (vpCurCalibratedInfo.at(j) != NULL)
	// 		// 					{
	// 		// 						product.m_vpCalibratedInfo.at(j) = new CCalibratedInfo(*vpCurCalibratedInfo.at(j));
	// 		// 					}
	// 		// 				}
	// 		// 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
	// 		// 
	// 		// 				m_bCalibInfoChanged = TRUE;
	// 		// 			}
	// 		// 		}
	// 
	// 		// 目标Mark模板图像显示
	// 		int i = 0, j = 0;
	// 		if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
	// 		{
	// 			CBaseSearchTool* pTargetSearchTool = NULL;
	// 			scImageGray image;
	// 			for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
	// 			{
	// 				j = m_vpTargetPatSel.at(i);
	// 				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(i, j);
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
	// 
	// 				for(j = 0; j<TARGETCANDIDATENUM; j++)
	// 				{
	// 					pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(i, j);
	// 					if (pTargetSearchTool != NULL)
	// 					{
	// 						image.Release();
	// 						pTargetSearchTool->GetPatternImage(image);
	// 						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
	// 						m_vvpTargetGuiDisplay[i][j]->SetImage(image, TRUE);
	// 					}
	// 					else
	// 					{
	// 						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
	// 					}
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
	// 
	// 		// 对象Mark模板图像显示
	// 		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	// 		{
	// 			CBaseSearchTool* pObjSearchTool = NULL;
	// 			scImageGray image;
	// 			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
	// 			{
	// 				j = m_vpObjectPatSel.at(i);
	// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(i, j);
	// 				if (pObjSearchTool != NULL)
	// 				{
	// 					image.Release();
	// 					pObjSearchTool->GetPatternImage(image);
	// 					m_vpGuiDisplay.at(i)->ClearScreen();
	// 					m_vpGuiDisplay.at(i)->SetImage(image, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 
	// 				for(j = 0; j<OBJECTCANDIDATENUM; j++)
	// 				{
	// 					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(i, j);
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
	// 		// 		CBaseSearchTool* pObjSearchTool = NULL;
	// 		// 		scImageGray image;
	// 		// 		for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
	// 		// 		{
	// 		// 			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	// 		// 			if (pObjSearchTool != NULL)
	// 		// 			{
	// 		// 				image.Release();
	// 		// 				pObjSearchTool->GetPatternImage(image);
	// 		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 		// 				m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
	// 		// 			}
	// 		// 			else
	// 		// 			{
	// 		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 		// 			}
	// 		// 		}
	// 
	// 		UpdateData(FALSE);
	// 		EndWaitCursor();
	// 	}
}

void CDlgSysKindCalibSetting::UpdateStatusBar()
{
	// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysKindCalibSetting::UpdateProductListDisplay()
{
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	if (pTotalProduct == NULL)
	{
		return;
	}


	CString strTemp;
	CString strTemp1;

	int nProductIndexFa, nProductIndexChd;
	pTotalProduct->GetDlgCalibIndex( m_nCurProductIdx, nProductIndexFa, nProductIndexChd);

	// 大品种刷新列表
	m_listctrlProductFa.DeleteAllItems();
	m_comboProductSrcFa.ResetContent();
	m_comboProductDstFa.ResetContent();
	for(int i=0; i<pTotalProduct->GetCalibCountFa(); i++)
	{
		//序号
		strTemp.Format(_T("%03d"), i+1);
		m_listctrlProductFa.InsertItem(i, strTemp);

		// 产品名称
		strTemp1 = pTotalProduct->GetCalibNameFa(i);
		m_listctrlProductFa.SetItemText(i, 1, strTemp1);
		m_comboProductSrcFa.InsertString(i, strTemp + _T("|") + strTemp1);
		m_comboProductDstFa.InsertString(i, strTemp + _T("|") + strTemp1);

		if (i == nProductIndexFa)
		{
			m_listctrlProductFa.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
		}
	}

	// 小品种刷新列表
	m_listctrlProduct.DeleteAllItems();
	m_comboProductSrc.ResetContent();
	m_comboProductDst.ResetContent();
	for (int i=0; i<pTotalProduct->GetCalibCountChd(nProductIndexFa); i++)
	{
		// 序号
		strTemp.Format(_T("%03d"), i+1);
		m_listctrlProduct.InsertItem(i, strTemp);

		// 产品名称
		int nProductIndex =  pTotalProduct->GetCalibIndex(nProductIndexFa,i);
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_listctrlProduct.SetItemText(i, 1, strTemp1);
		m_comboProductSrc.InsertString(i, strTemp + _T("|") + strTemp1);
		m_comboProductDst.InsertString(i, strTemp + _T("|") + strTemp1);

		if (i == nProductIndexChd)
		{
			m_listctrlProduct.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
		}
	}
	if (m_nSrcProductIndex >= m_comboProductSrc.GetCount())
	{
		m_nSrcProductIndex = 0;
	}
	m_comboProductSrc.SetCurSel(m_nSrcProductIndex);

	if (m_nDstProductIndex >= m_comboProductDst.GetCount())
	{
		m_nDstProductIndex = 0;
	}
	m_comboProductDst.SetCurSel(m_nDstProductIndex);

	if (m_nSrcProductIndexFa >= m_comboProductSrcFa.GetCount())
	{
		m_nSrcProductIndexFa = 0;
	}
	m_comboProductSrcFa.SetCurSel(m_nSrcProductIndexFa);

	if (m_nDstProductIndexFa >= m_comboProductDstFa.GetCount())
	{
		m_nDstProductIndexFa = 0;
	}
	m_comboProductDstFa.SetCurSel(m_nDstProductIndexFa);

	//UpdateProductComboDisplay();
}

void CDlgSysKindCalibSetting::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_KIND_CALIB_SETTING") == strDlgID)
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

CString CDlgSysKindCalibSetting::GetNewDefName()
{
	CString strName;
	strName = _T("------");
	return strName;

	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
	int nNum = pTotalProduct->GetCount();
//	CString strName;
	for (int i=0; i<=nNum; i++)
	{
		strName.Format(_T("JobCalib%d"), i+1);
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

BOOL CDlgSysKindCalibSetting::IsNameUnique( LPCTSTR lpszName )
{
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nProductNum = pTotalProduct->GetCount();
	CString strName = lpszName;
	if (strName.IsEmpty())
	{
		return FALSE;
	}
	for (int i=0; i<nProductNum; i++)
	{
		if (strName == pTotalProduct->GetItemName(i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindCalibSetting::IsNameLegal( LPCTSTR lpszName )
{
	CString strName = lpszName;
	if (strName.IsEmpty())
	{
		return FALSE;
	}

	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
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

BOOL CDlgSysKindCalibSetting::IsCalibratedInfosEqual( std::vector<CCalibratedInfo*> vpCalibratedInfo1, 
	std::vector<CCalibratedInfo*> vpCalibratedInfo2 )
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

BOOL CDlgSysKindCalibSetting::IsProductChanged()
{


	if (m_bProdPatternChanged || m_bProdNameChanged || m_bProdNewCreate || m_bCalibInfoChanged)
	{
		return TRUE;
	}


	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPrevPlatformIdx); // 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	ASSERT(pTotalProduct != NULL);
	CCalibData product;
	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	{
		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	}

	pTotalProduct->SetCurCalibIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);

	if (m_comboCalibMarkPatternMode.GetCurSel()!=(int)product.GetCalibMarkPatternMode() 
			|| m_comboHomographyDOF.GetCurSel()!=(int)product.GetCalibHomographyDOF()
			|| m_cmbWholeCalibCacuPos.GetCurSel()!=product.GetWholeCalibCacuPos()
			|| m_bEnableWholeCalibCacuPos != product.m_bEnableWholeCalibCacuPos)
	{
		return TRUE;
	}

	CCoordPos pos;
	int i=0; 
	CString strInfo;
	for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
	{
		if (product.m_vpCalibrateParam.at(i) == NULL)
		{
			product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
		}

		pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
		pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
		pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
		if (pos.m_dPosX != product.m_vpCalibrateParam.at(i)->GetPlatformCalibMovement().m_dPosX
			|| pos.m_dPosY != product.m_vpCalibrateParam.at(i)->GetPlatformCalibMovement().m_dPosY
			|| pos.m_dAngle != product.m_vpCalibrateParam.at(i)->GetPlatformCalibMovement().m_dAngle)
		{
			return TRUE;
		}

		pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
		pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
		pos.m_dAngle = 0.0;
		if (pos.m_dPosX != product.m_vpCalibrateParam.at(i)->GetCameraCalibMovement().m_dPosX
			|| pos.m_dPosY != product.m_vpCalibrateParam.at(i)->GetCameraCalibMovement().m_dPosY)
		{
			return TRUE;
		}

		if (atof(*m_vstrCalibRefineMoveD.at(i)) != product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineMoveD()
			|| atoi(*m_vstrCalibRefineTime.at(i)) != product.m_vpCalibrateParam.at(i)->GetPlatformCalibRefineTime()
			|| atoi(*m_vstrSearchDelayTime.at(i)) != product.m_vpCalibrateParam.at(i)->GetSearchDelayTime())
		{
			return TRUE;
		}

		if( m_vdCalibTileX.at(i)		!= product.m_vpCalibrateParam.at(i)->m_dTileX		|| 
			m_vdCalibTileY.at(i)		!= product.m_vpCalibrateParam.at(i)->m_dTileY		|| 
			m_vbEnableOutPoints.at(i)	!= product.m_vpCalibrateParam.at(i)->m_bOutPoints	||
			m_vnThre.at(i)				!= product.m_vpCalibrateParam.at(i)->m_iThre ||
			*m_vnEnableDisCor.at(i)		!= product.m_vpCalibrateParam.at(i)->m_nEnableDisCor)
		{
			return TRUE;
		}

		if (m_comboCalibMarkPatternMode.GetCurSel() != (int)product.m_vpCalibrateParam.at(i)->GetCalibMarkPatternMode())
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CDlgSysKindCalibSetting::IsSearchToolSelected(CBaseSearchTool* pSearchTool)
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

BOOL CDlgSysKindCalibSetting::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgSysKindCalibSetting::CreateDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindCalibSetting::ClearDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindCalibSetting::RemoveDirectoryPro( LPCTSTR lpszDir )
{
	AlignToolLogRecord(lpszDir, m_nPrevPlatformIdx);

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

BOOL CDlgSysKindCalibSetting::RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
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


BOOL CDlgSysKindCalibSetting::SavePrevProduct()
{
	BOOL bSucceed = TRUE;
	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData();

	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	{
		// 保存对位参数
		CCalibData product = pTotalProduct->GetItem(m_nPrevProductIdx);
		product.SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
		product.SetCalibHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());
		product.SetWholeCalibCacuPos(m_cmbWholeCalibCacuPos.GetCurSel());
		product.SetWholeCalibCacuPosEnable(m_bEnableWholeCalibCacuPos);

		int i=0; 
		CCoordPos pos;
		CString strInfo;
		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
		{
			if (product.m_vpCalibrateParam.at(i) == NULL)
			{
				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
			}

			pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
			pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
			pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibMovement(pos);

			pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
			pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
			pos.m_dAngle = 0.0;
			product.m_vpCalibrateParam.at(i)->SetCameraCalibMovement(pos);

			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineMoveD(atof(*m_vstrCalibRefineMoveD.at(i)));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineTime(atoi(*m_vstrCalibRefineTime.at(i)));
			product.m_vpCalibrateParam.at(i)->SetSearchDelayTime(atoi(*m_vstrSearchDelayTime.at(i)));
			product.m_vpCalibrateParam.at(i)->SetPlatformCalibrateRefine(m_bCalibRefine);

			product.m_vpCalibrateParam.at(i)->m_nEnableDisCor = *m_vnEnableDisCor.at(i);
			product.m_vpCalibrateParam.at(i)->m_dTileX = m_vdCalibTileX.at(i);
			product.m_vpCalibrateParam.at(i)->m_dTileY = m_vdCalibTileY.at(i);
			product.m_vpCalibrateParam.at(i)->m_bOutPoints = m_vbEnableOutPoints.at(i);
			product.m_vpCalibrateParam.at(i)->m_iThre = m_vnThre.at(i);

			product.m_vpCalibrateParam.at(i)->SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
			//product.m_vpCalibrateParam.at(i)->SetHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());

		}
		pTotalProduct->SetItem(m_nPrevProductIdx, product);

		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveCalibDataInfo(m_nPrevProductIdx);
		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
		}

		bSucceed = SaveSearchToolInfo();

		m_strProductNameOld.Empty();
		m_strProductNameOldFa.Empty();
	}

	return bSucceed;
}


// BOOL CDlgSysKindCalibSetting::SavePrevProduct()
// {
// 	BOOL bSucceed = TRUE;
// 	UpdateData(TRUE);
// 	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData();
// 
// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// 	{
// 		// 保存对位参数
// 		CCalibData product = pTotalProduct->GetItem(m_nPrevProductIdx);
// 		product.SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 		product.SetCalibHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());
// 		product.SetWholeCalibCacuPos(m_cmbWholeCalibCacuPos.GetCurSel());
// 		product.SetWholeCalibCacuPosEnable(m_bEnableWholeCalibCacuPos);
// 
// 		int i=0; 
// 		CCoordPos pos;
// 		CString strInfo;
// 		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
// 		{
// 			if (product.m_vpCalibrateParam.at(i) == NULL)
// 			{
// 				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
// 			}
// 
// 			pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
// 			pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibMovement(pos);
// 
// 			pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
// 			pos.m_dAngle = 0.0;
// 			product.m_vpCalibrateParam.at(i)->SetCameraCalibMovement(pos);
// 
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineMoveD(atof(*m_vstrCalibRefineMoveD.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineTime(atoi(*m_vstrCalibRefineTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetSearchDelayTime(atoi(*m_vstrSearchDelayTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibrateRefine(m_bCalibRefine);
// 
// 			product.m_vpCalibrateParam.at(i)->m_nEnableDisCor = *m_vnEnableDisCor.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_dTileX = m_vdCalibTileX.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_dTileY = m_vdCalibTileY.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_bOutPoints = m_vbEnableOutPoints.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_iThre = m_vnThre.at(i);
// 
// 			product.m_vpCalibrateParam.at(i)->SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 
// 		}
// 		pTotalProduct->SetItem(m_nPrevProductIdx, product);
// 
// 
// 		// 保存产品信息到磁盘
// 		CString strDir, strTemp;
// 		SysPlatformInfo platformInfo;
// 		vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 		pVisionASM->GetSysPlatformInfo(platformInfo);
// 
// 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 		strTemp.Format(_T("\\JobCalibs\\%03d"), m_nPrevProductIdx+1);
// 		strDir += strTemp;
// 
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveCalibDataInfo(m_nPrevProductIdx);
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 		}
// 
// 		//// 检查存储空间是否超过阈值
// 		if(1)
// 		{
// 			if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
// 			{
// 				if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
// 				{
// 					double dUsedRate = 0.0;
// 					CString StrTmp = m_strModuleDir;
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);
// 
// 					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp))
// 					{
// 						CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
// 						CString strTmp2;
// 						strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
// 						CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);
// 
// 						AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n标定模板未保存到磁盘！"));
// 
// 						CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n标定模板未保存到磁盘！"));
// 						// 避免后续无法保存的情况
// 
// 						m_strProductNameOld.Empty();
// 						m_strProductNameOldFa.Empty();
// 
// 						return TRUE;
// 
// 					}
// 				}
// 			}
// 		}
// 
// 		CCalibData *pCalibData = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 
// 		// 保存模板到磁盘
// 		//		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		int nPatternNum = platformInfo.m_nPositionNum;
// 		CBaseSearchTool *pSearchTool = NULL;
// 		int j = 0;
// 		i = 0;
// 		//if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//{
// 		//	CreateDirectoryPro(strDir + _T("\\CalibModels"));
// 		//	for (j=0; j<nPatternNum; j++)
// 		//	{
// 		//		strTemp.Format(_T("\\Pos%d_M%d"), j, i);
// 		//		pSearchTool = pVisionASM->GetRepoCalibObjectSearchTool(m_nPrevProductIdx, j, i);
// 		//		if (pSearchTool != NULL) 
// 		//		{
// 		//			CString strDestDir = strDir + _T("\\CalibModels")+strTemp;
// 		//			if (!pSearchTool->SaveModelToFile(strDestDir))
// 		//			{
// 		//				//strTemp.Format(_T("CH%d标定Mark模板保存失败！"), j);
// 		//				strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_FAILED));
// 		//				AfxMessageBox(strTemp);
// 		//				return FALSE;
// 		//			}
// 		//		}
// 		//		else
// 		//		{
// 		//			RemoveDirectoryPro(strDir + _T("\\CalibModels")+strTemp);
// 		//		}
// 		//	}
// 
// 		//}			
// 
// 		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			if(!m_pParentKind->m_pDlgKindAlignView->SaveSearchTool(eCalibSearch,strDir,pCalibData->m_vvpCalibObjectSearchToolRepo,m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged))
// 			{
// 				return FALSE;
// 			}
// 
// 		}			
// 
// 		m_strProductNameOld.Empty();
// 		m_strProductNameOldFa.Empty();
// 	}
// 
// 	return bSucceed;
// }

// 保存前一个产品的设置
// BOOL CDlgSysKindCalibSetting::SavePrevProduct()
// {
// 	BOOL bSucceed = TRUE;
// 	UpdateData(TRUE);
// 	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData();
// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// 	{
// 		// 保存对位参数
// 		CCalibData product = pTotalProduct->GetItem(m_nPrevProductIdx);
// 		product.SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 		product.SetCalibHomographyDOF((HomographyDOF)m_comboHomographyDOF.GetCurSel());
// 		product.SetWholeCalibCacuPos(m_cmbWholeCalibCacuPos.GetCurSel());
// 		product.SetWholeCalibCacuPosEnable(m_bEnableWholeCalibCacuPos);
// 		int i=0; 
// 		CCoordPos pos;
// 		CString strInfo;
// 		for (i = 0;i<product.m_vpCalibrateParam.size() && i<MAXPOSNUM; i++ )
// 		{
// 			if (product.m_vpCalibrateParam.at(i) == NULL)
// 			{
// 				product.m_vpCalibrateParam.at(i) = new CCalibrateParam;
// 			}
// 
// 			pos.m_dPosX = atof(*m_vstrCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCalibMovementY.at(i));
// 			pos.m_dAngle = atof(*m_vstrCalibMovementD.at(i));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibMovement(pos);
// 			pos.m_dPosX = atof(*m_vstrCamCalibMovementX.at(i));
// 			pos.m_dPosY = atof(*m_vstrCamCalibMovementY.at(i));
// 			pos.m_dAngle = 0.0;
// 			product.m_vpCalibrateParam.at(i)->SetCameraCalibMovement(pos);
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineMoveD(atof(*m_vstrCalibRefineMoveD.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibRefineTime(atoi(*m_vstrCalibRefineTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetSearchDelayTime(atoi(*m_vstrSearchDelayTime.at(i)));
// 			product.m_vpCalibrateParam.at(i)->SetPlatformCalibrateRefine(m_bCalibRefine);
// 			
// 			product.m_vpCalibrateParam.at(i)->m_nEnableDisCor = *m_vnEnableDisCor.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_dTileX = m_vdCalibTileX.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_dTileY = m_vdCalibTileY.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_bOutPoints = m_vbEnableOutPoints.at(i);
// 			product.m_vpCalibrateParam.at(i)->m_iThre = m_vnThre.at(i);
// 			product.m_vpCalibrateParam.at(i)->SetCalibMarkPatternMode((CalibMarkPatternMode)m_comboCalibMarkPatternMode.GetCurSel());
// 		}
// // 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
// // 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// // 		{
// // 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// // 		}
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
// // 		// 如果产品被重命名，则删除旧文件架
// // 		if ((m_bProdNameChanged) && (!m_strProductNameOld.IsEmpty()))
// // 		{
// // 			strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// // 			strTemp.Format(_T("\\JobCalibs\\%s"), m_strProductNameOld);
// // 			strDir += strTemp;
// // 			RemoveDirectoryPro(strDir);
// // 		}
// 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 		//strTemp.Format(_T("\\JobCalibs\\%s"), pTotalProduct->GetItemName(m_nPrevProductIdx));
// 		strTemp.Format(_T("\\JobCalibs\\%03d"), m_nPrevProductIdx+1);
// 		strDir += strTemp;        
// 		//ClearDirectoryPro(strDir); // Mali 2013.9.5 为提升效率
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveCalibDataInfo(m_nPrevProductIdx);
// 		//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 		}
// 		
// 		CCalibData *pCalibData = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 
// 		// 保存模板到磁盘
// //		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		int nPatternNum = platformInfo.m_nPositionNum;
// 		CBaseSearchTool *pSearchTool = NULL;
// 		int j = 0;
// 		i = 0;
// 		//if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//{
// 		//	CreateDirectoryPro(strDir + _T("\\CalibModels"));
// 		//	for (j=0; j<nPatternNum; j++)
// 		//	{
// 		//		strTemp.Format(_T("\\Pos%d_M%d"), j, i);
// 		//		pSearchTool = pVisionASM->GetRepoCalibObjectSearchTool(m_nPrevProductIdx, j, i);
// 		//		if (pSearchTool != NULL) 
// 		//		{
// 		//			CString strDestDir = strDir + _T("\\CalibModels")+strTemp;
// 		//			if (!pSearchTool->SaveModelToFile(strDestDir))
// 		//			{
// 		//				//strTemp.Format(_T("CH%d标定Mark模板保存失败！"), j);
// 		//				strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_FAILED));
// 		//				AfxMessageBox(strTemp);
// 		//				return FALSE;
// 		//			}
// 		//		}
// 		//		else
// 		//		{
// 		//			RemoveDirectoryPro(strDir + _T("\\CalibModels")+strTemp);
// 		//		}
// 		//	}
// 
// 		//}			
// 
// 		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		{
// 			if(!m_pParentKind->m_pDlgKindAlignView->SaveSearchTool(eCalibSearch,strDir,pCalibData->m_vvpCalibObjectSearchToolRepo,m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged))
// 			{
// 				return FALSE;
// 			}
// 
// 		}			
// 
// 		m_strProductNameOld.Empty();
// 		m_strProductNameOldFa.Empty();
// 	}
// 
// 	return bSucceed;
// }

BOOL CDlgSysKindCalibSetting::SaveSearchToolInfo()
{
	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData();
	if ((m_nPrevProductIdx < 0) && (m_nPrevProductIdx >= pTotalProduct->GetCount()))
	{
		return FALSE;
	}
	// 保存产品信息到磁盘
	CString strDir, strTemp;
	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);

	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	strTemp.Format(_T("\\JobCalibs\\%03d"), m_nPrevProductIdx+1);
	strDir += strTemp;        

	CCalibData *pCalibData = pTotalProduct->GetItemPtr(m_nPrevProductIdx);

	// 保存模板到磁盘

	if (m_bProdNewCreate || m_bProdPatternChanged ) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
	{
		if(!m_pParentKind->m_pDlgKindAlignView->SaveSearchTool(eCalibSearch,strDir,pCalibData->m_vvpCalibObjectSearchToolRepo,m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged))
		{
			return FALSE;
		}

	}
	return TRUE;
}
void CDlgSysKindCalibSetting::OnButtonNewProduct()
{
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	if (IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			return;
		}
		else // 不保存对产品的修改
		{
			if (m_bProdNameChanged)
			{
// 				CString strOldDir, strNewDir;
// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
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
				int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
				int nCurProductIdxFa = 0;
				m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
				m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);
				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
				UpdateDlgData(FALSE);
			}
			if (m_bProdNewCreate)
			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
			}
			if (m_bProdNameChanged || m_bProdNewCreate)
			{
				UpdateProductListDisplay();
			}

			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
			{                        
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
			}
			LoadSearchToolInfo();
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
	}

	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
			CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
			if (pPreProduct != NULL)
			{
				pPreProduct->DeleteObjectSearchTools();
			}
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();
	// 	if (nCount < 1)
	// 	{
	SysPlatformInfo platformInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	CCalibData product(platformInfo);
	pTotalProduct->AddItem(product);
	pTotalProduct->SetItemName(nCount, strName);
	pTotalProduct->SetCurCalibIndex(nCount);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCount);
	// 	}
	// 	else
	// 	{
	// //		UpdateDlgData(TRUE);
	// 		int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	// 		if (pTotalProduct->CopyItem(nFormIdx, nCount))
	// 		{
	// 			pTotalProduct->SetItemName(nCount, strName);
	// 			pTotalProduct->SetCurProductIndex(nCount);
	// 		}
	// 	}
	m_strProductNameOld = strName;
	m_strProductNameOldFa = strName;
	m_bProdNewCreate = TRUE;

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();
}

void CDlgSysKindCalibSetting::CreateNewProduct(int nNewProductIndexFa, int nNewProductIndexChd)
{
	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();

	SysPlatformInfo platformInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	CCalibData product(platformInfo);
	pTotalProduct->AddItem(product);
	pTotalProduct->SetItemName(nCount, strName);
	pTotalProduct->SetCalibIndex(nNewProductIndexFa, nNewProductIndexChd, nCount);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCount);
}

// void CDlgSysKindCalibSetting::CreateNewProduct(int nNewProductIndexFa, int nNewProductIndexChd)
// {
// 	CString strLog;
// 	int nNewProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCount();
// 	if (nNewProductIndex < 0)
// 	{
// 		return;
// 	}
// 	else if (nNewProductIndex < m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData()->GetCount())
// 	{
// 		SetCurProductIndex(nNewProductIndex);
// 	}
// 	else
// 	{
// 		// 如果前一个产品已修改，提示是否保存前一个产品的设置
// 		if (IsProductChanged())
// 		{
// 			strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT) + "\n";
// 			g_opLogFile.SaveLogText(strLog,5);
// 
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 			if (nMBReturn == IDYES)
// 			{
// 				strLog = _T("弹窗选择：是\n");
// 				g_opLogFile.SaveLogText(strLog,5);
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
// // 			else if (nMBReturn == IDCANCEL)
// // 			{
// // 				return;
// // 			}
// 			else // 不保存对产品的修改
// 			{
// 				strLog = _T("弹窗选择：否\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				if (m_bProdNameChanged)
// 				{
// 					// 				CString strOldDir, strNewDir;
// 					// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 					// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 					// 				{
// 					// 					SysPlatformInfo platformInfo;
// 					// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 					// 				}
// 					// 
// 					// 				// 如果文件夹存在，则重命名文件夹
// 					// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// 					// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 					// 				{
// 					// 					if (strCurName != m_strProductNameOld)
// 					// 					{						
// 					// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// 					// 						{
// 					// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
// 					// 						}
// 					// 					}
// 					// 				}
// 					int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
// 					int nCurProductIdxFa = 0;
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 					strLog.Format(_T("当前标定信息%d(大品种%d_小品种%d)[标定]"),m_nPrevProductIdx+1,nCurProductIdxFa+1,nCurProductIdxCh+1);
// 					g_opLogFile.SaveLogText(strLog,6);
// 
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
// 					UpdateDlgData(FALSE);
// 				}
// 				if (m_bProdNewCreate)
// 				{
// // 					CString strDir;
// // 					CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 						strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 					}
// // 					strDir = m_strModuleDir + strDir;
// // 					if (!RemoveDirectoryPro(strDir))
// // 					{
// // 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// // 					}
// // 					m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 				}
// 				if (m_bProdNameChanged || m_bProdNewCreate)
// 				{
// 					UpdateProductListDisplay();
// 				}
// 
// 				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 				{                        
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 				}
// 				LoadSearchToolInfo();
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		// ShiYue 20190228
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 				CTotalCalibData *pTotalProduct = NULL;
// 				pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
// 				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 
// 				if (pPreProduct != NULL)
// 				{
// 					pPreProduct->DeleteObjectSearchTools();
// 				}
// 			}
// 			// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 				CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
// 				for (int i=0; i<pTotalProduct->GetCalibCountFa(); i++ )
// 				{
// 					if(i == nNewProductIndexFa) continue;
// 
// 					for (int j =0; j< pTotalProduct->GetCalibCountChd(i); j++)
// 					{
// 						int nProductIndex = pTotalProduct->GetCalibIndex(i, j);
// 						CCalibData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 						if (pPreProductData != NULL)
// 						{
// 							pPreProductData->DeleteObjectSearchTools();
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// // 		// 释放原来产品的定位工具20170516
// // 		{
// // 			SysPlatformInfo sysPlatformInfo;
// // 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// // 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// // 			{
// // 				CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// // 				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// // 				if (pPreProduct != NULL)
// // 				{
// // 					pPreProduct->DeleteObjectSearchTools();
// // 				}
// // 			}
// // 		}
// 
// 		// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
// 		CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// 		int nCount = pTotalProduct->GetCount();
// 		CString strName = GetNewDefName();
// 
// 		SysPlatformInfo platformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
// 		CCalibData product(platformInfo);
// 		pTotalProduct->AddItem(product);
// 		pTotalProduct->SetItemName(nCount, strName);
// 		pTotalProduct->SetCurCalibIndex(nCount);
// 		pTotalProduct->SetCalibIndex(nNewProductIndexFa, nNewProductIndexChd, nCount);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCount);
// 
// 		m_strProductNameOld = strName;
// 		m_strProductNameOldFa = strName;
// 		m_bProdNewCreate = TRUE;
// 		
// 		
// 		strLog.Format(_T("新建标定信息%d：大品种%d,小品种%d[标定]\n"),nCount+1,nNewProductIndexFa+1, nNewProductIndexChd+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		UpdateDlgData(FALSE);
// 		UpdateProductListDisplay();
// 	}
// }
// 

void CDlgSysKindCalibSetting::CreateNewProductMulitple(int nNewProductIndex, int nNewProductNum)
{

	if (nNewProductIndex < 0)
	{
		return;
	}
	else if (nNewProductIndex < m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData()->GetCount())
	{
		SetCurProductIndex(nNewProductIndex);
	}
	else
	{
		// 如果前一个产品已修改，提示是否保存前一个产品的设置
		if (IsProductChanged())
		{
			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
			if (nMBReturn == IDYES)
			{
				if (SavePrevProduct())
				{
					// 恢复修改标志
					m_bProdPatternChanged = FALSE;
					m_bProdNameChanged = FALSE;
					m_bProdBenchMarkChanged = FALSE;
					m_bProdNewCreate = FALSE;
					m_bCalibInfoChanged = FALSE;
				}
			}
// 			else if (nMBReturn == IDCANCEL)
// 			{
// 				return;
// 			}
			else // 不保存对产品的修改
			{
				if (m_bProdNameChanged)
				{
					// 				CString strOldDir, strNewDir;
					// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
					// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
					// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
					// 				{
					// 					SysPlatformInfo platformInfo;
					// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
					// 					pVisionASM->GetSysPlatformInfo(platformInfo);
					// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
					// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
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
					int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
					int nCurProductIdxFa = 0;
					m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
					m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

					m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
					UpdateDlgData(FALSE);
				}
				if (m_bProdNewCreate)
				{
// 					CString strDir;
// 					CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 					{
// 						SysPlatformInfo platformInfo;
// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 						//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 						strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 					}
// 					strDir = m_strModuleDir + strDir;
// 					if (!RemoveDirectoryPro(strDir))
// 					{
// 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 					}
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
				}
				if (m_bProdNameChanged || m_bProdNewCreate)
				{
					UpdateProductListDisplay();
				}

				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
				{                        
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
				}
				LoadSearchToolInfo();
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}

		// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
		CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
		int nCount = pTotalProduct->GetCount();
		CString strName = GetNewDefName();
		// 	if (nCount < 1)
		// 	{
		SysPlatformInfo platformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
		CCalibData product(platformInfo);

		int nIndex = 0;
		for (nIndex = nCount; nIndex < nCount + nNewProductNum; nIndex++)
		{
			pTotalProduct->AddItem(product);
			pTotalProduct->SetItemName(nIndex, strName);
			pTotalProduct->SetCurCalibIndex(nIndex);
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(TRUE);

		// 	}
		// 	else
		// 	{
		// //		UpdateDlgData(TRUE);
		// 		int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
		// 		if (pTotalProduct->CopyItem(nFormIdx, nCount))
		// 		{
		// 			pTotalProduct->SetItemName(nCount, strName);
		// 			pTotalProduct->SetCurProductIndex(nCount);
		// 		}
		// 	}
		m_strProductNameOld = strName;
		m_bProdNewCreate = TRUE;

		UpdateDlgData(FALSE);
		UpdateProductListDisplay();
	}
}

void CDlgSysKindCalibSetting::OnBtnCloneProduct() 
{
	// TODO: Add your control notification handler code here
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	if (IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			return;
		}
		else // 不保存对产品的修改
		{
			if (m_bProdNameChanged)
			{
// 				CString strOldDir, strNewDir;
// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
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
				int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
				int nCurProductIdxFa = 0;
				m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
				m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
				UpdateDlgData(FALSE);
			}
			if (m_bProdNewCreate)
			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
			}
			if (m_bProdNameChanged || m_bProdNewCreate)
			{
				UpdateProductListDisplay();
			}

			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
			{                        
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
			}
			LoadSearchToolInfo();
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();
	// 	if (nCount < 1)
	// 	{
	// 		SysPlatformInfo platformInfo;
	// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	// 		CProductData product(platformInfo);
	// 		pTotalProduct->AddItem(product);
	// 		pTotalProduct->SetItemName(0, strName);
	// 		pTotalProduct->SetCurProductIndex(0);
	// 	}
	// 	else
	// 	{
	//		UpdateDlgData(TRUE);
	int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	if (pTotalProduct->CopyItem(nFormIdx, nCount))
	{
		pTotalProduct->SetItemName(nCount, strName);
		pTotalProduct->SetCurCalibIndex(nCount);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCount);
	}
	//	}
	m_strProductNameOld = strName;
	m_bProdNewCreate = TRUE;

	// 拷贝模板：保存模板到新产品的模板文件夹下以达到拷贝模板的目的
	int nPrevProdIdxTmp = m_nPrevProductIdx;
	m_nPrevProductIdx = nCount;
	SavePrevProduct();
	m_nPrevProductIdx = nPrevProdIdxTmp;

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();
}

void CDlgSysKindCalibSetting::OnButtonDeleteCurProduct() 
{
	// 用户删除操作确认
	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_ROMOVE_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要删除当前产品？")
	{
		return;
	}

	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	if (nCount > 0)
	{
		CString strDir;
		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{
			SysPlatformInfo platformInfo;
			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!RemoveDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("删除产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
		}
		if (pTotalProduct->RemoveItem(m_nCurProductIdx))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;

			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
			pTotalProduct->SetCurCalibIndex(m_nCurProductIdx);
			UpdateDlgData(FALSE);
			UpdateProductListDisplay();
			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurCalibIndex());
			int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
			int nCurProductIdxFa = 0;
			m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(pTotalProduct->GetCurCalibIndex(), nCurProductIdxFa, nCurProductIdxCh);
			m_strProductNameOldFa = pTotalProduct->GetCalibNameFa(nCurProductIdxFa);
		}
		else
		{
			//AfxMessageBox(_T("删除产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
}


void CDlgSysKindCalibSetting::DeleteCurProduct(int nCurProductIndex) 
{
	SetCurProductIndex(nCurProductIndex);
	UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	SysPlatformInfo platformInfo;
	if (nCount > 0)
	{
		CString strDir;
		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{
			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!RemoveDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("删除产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
		}

		for(int i = m_nCurProductIdx+1; i < nCount; i++)
		{
			CString strOldDir , strNewDir;
			strOldDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i+1);
			strOldDir = m_strModuleDir + strOldDir;
			strNewDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i);
			strNewDir = m_strModuleDir + strNewDir;
			RenameDirecotryPro(strOldDir,strNewDir);
		}

		if (pTotalProduct->RemoveItem(m_nCurProductIdx))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;

			for(int i = m_nCurProductIdx;i < pTotalProduct->GetCount(); i++)
			{
				pTotalProduct->GetItem(i).SetCalibIndex(i);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(i);
			}

			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
			pTotalProduct->SetCurCalibIndex(m_nCurProductIdx);
			UpdateDlgData(FALSE);
			UpdateProductListDisplay();
			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurCalibIndex());
			int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
			int nCurProductIdxFa = 0;
			m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(pTotalProduct->GetCurCalibIndex(), nCurProductIdxFa, nCurProductIdxCh);
			m_strProductNameOldFa = pTotalProduct->GetCalibNameFa(nCurProductIdxFa);
		}
		else
		{
			//AfxMessageBox(_T("删除产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
}

void CDlgSysKindCalibSetting::ClearCurProduct(int nCurProductIndex)
{
	//SetCurProductIndex(nCurProductIndex);
	//UpdateData(TRUE);
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();

	if (nCount > 0)
	{
		CString strDir;
		SysPlatformInfo platformInfo;
		CString strProductName = pTotalProduct->GetItemName(nCurProductIndex);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\JobCalibs\\%s"), m_nPlatformIndex, strProductName);
		{

			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, nCurProductIndex+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!ClearDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("清空产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT_DATA_FILE));
		}
		CreateDirectoryPro(strDir + _T("\\CalibModels"));

		CString strName = GetNewDefName();
		pTotalProduct->SetItemName(nCurProductIndex, strName);

		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(nCurProductIndex);
		UpdateDlgData(FALSE);
		if (pTotalProduct->ClearItem(nCurProductIndex))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
			UpdateProductListDisplay();
		}
		else
		{
			//AfxMessageBox(_T("清空产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
}


void CDlgSysKindCalibSetting::SetProductListSelect(int nProductFa, int nProductChd)
{
	m_listctrlProduct.SetItemState(nProductChd, LVIS_SELECTED, LVIS_SELECTED);
	m_listctrlProductFa.SetItemState(nProductFa, LVIS_SELECTED, LVIS_SELECTED);
}

void CDlgSysKindCalibSetting::ResetProductChangeStatus()
{
	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;
}

void CDlgSysKindCalibSetting::RestoreProductModify()
{
	if (m_bProdNameChanged)
	{
		int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
		int nCurProductIdxFa = 0;
		m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
		m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

		m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
		//	UpdateDlgData(FALSE);
	}
	if (m_bProdNewCreate)
	{
	}
	if (m_bProdNewCreate || m_bProdNameChanged)
	{
		UpdateProductListDisplay();
	}

	if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	{                        
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	}
	LoadSearchToolInfo();
	// 恢复修改标志
	ResetProductChangeStatus();
}

void CDlgSysKindCalibSetting::FreePreProductMem(int nCurProductIndex, int nPreProductIndex, int nPlatformIndex)
{
	int nCurProductIndexFa = 0;
	int nCurProductIndexChd = 0;
	int nPreProductIndexFa = 0;
	int nPreProductIndexChd = 0;
	SysPlatformInfo sysPlatformInfo;

	// 获取产品索引对应的大小品种序号
	m_vpTotalCalibData.at(nPlatformIndex)->GetDlgCalibIndex(nCurProductIndex, nCurProductIndexFa, nCurProductIndexChd);
	m_vpTotalCalibData.at(nPlatformIndex)->GetDlgCalibIndex(nPreProductIndex, nPreProductIndexFa, nPreProductIndexChd);

	// 获取平台信息
	m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);


	if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
	{
		CTotalCalibData *pTotalProduct = NULL;
		pTotalProduct = m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetTotalCalibData();
		CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);

		if (pPreProduct != NULL)
		{
			pPreProduct->DeleteObjectSearchTools();
		}
	}
	// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
	else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
	{

		if(nPreProductIndexFa != nCurProductIndexFa)
		{
			CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetTotalCalibData();
			for (int i=0; i<pTotalProduct->GetCalibCountFa(); i++ )
			{
				if(i == nCurProductIndexFa) continue;

				for (int j =0; j< pTotalProduct->GetCalibCountChd(i); j++)
				{
					int nProductIndex = pTotalProduct->GetCalibIndex(i, j);
					CCalibData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
					if (pPreProductData != NULL)
					{
						pPreProductData->DeleteObjectSearchTools();
					}
				}
			}
		}
	}
}



void CDlgSysKindCalibSetting::SetCurProductInfo()
{
	m_vpTotalCalibData.at(m_nPlatformIndex)->SetCurCalibIndex(m_nCurProductIdx);
	m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

	int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
	int nCurProductIdxFa = 0;
	m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);
	m_strProductNameOldFa = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nCurProductIdxFa);

	m_vpTotalCalibData.at(m_nPlatformIndex)->UpdateCalibLastChdIndex(nCurProductIdxFa, nCurProductIdxCh);

}


void CDlgSysKindCalibSetting::SetProductChangeTipInfoStart(const CString& strTipInfo)
{
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strTipInfo);
}


void CDlgSysKindCalibSetting::SetProductChangeTipInfoEnd(const CString& strTipInfo)
{
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strTipInfo);
}

void CDlgSysKindCalibSetting::SetCalibProductName(const CString& strName)
{
	CString strOldName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	if (strName != strOldName)
	{
		m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, strName);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nCurProductIdx);
		m_bProdNameChanged = TRUE;
		UpdateDlgData(FALSE);
		UpdateProductListDisplay();
	}
}


void CDlgSysKindCalibSetting::SetCalibProductNameFa(const CString& strName)
{
	int nProductIndexFa, nProductIndexChd;
	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
	CString strOldName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nProductIndexFa);

	if (strName != strOldName)
	{
		m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nProductIndexFa, strName);
		m_bProdNameChanged = TRUE;
		UpdateDlgData(FALSE);
		UpdateProductListDisplay();
	}
}



void CDlgSysKindCalibSetting::OnBtnProductRename()
{
	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalCalibData.at(m_nPlatformIndex)->GetCount()))
	{
		//AfxMessageBox(_T("请选择当前产品"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
	}
	CString strOldName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

	CDlgProductRename  dlgProductRename(strOldName, this);
	dlgProductRename.m_nParentType = 2;
	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgProductRename.DoModal() == IDOK)
	{
		CString strNewName = dlgProductRename.GetNewName();
		if (strNewName != strOldName)
		{
// 			CString strOldDir, strNewDir;
// 			// 			strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strOldName);
// 			// 			strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strNewName);
// 			{
// 				SysPlatformInfo platformInfo;
// 				vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 				pVisionASM->GetSysPlatformInfo(platformInfo);
// 				strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strOldName);
// 				strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strNewName);
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

			m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, strNewName);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nCurProductIdx);
			m_bProdNameChanged = TRUE;
			UpdateDlgData(FALSE);
			UpdateProductListDisplay();
		}
	}
}

void CDlgSysKindCalibSetting::OnBtnProductRenameFa()
{
	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalCalibData.at(m_nPlatformIndex)->GetCount()))
	{
		//AfxMessageBox(_T("请选择当前产品"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
	}
	int nProductIndexFa, nProductIndexChd;
	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
	CString strOldName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nProductIndexFa);

	CDlgProductRename  dlgProductRename(strOldName, this);
	dlgProductRename.m_nParentType = 2;
	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgProductRename.DoModal() == IDOK)
	{
		CString strNewName = dlgProductRename.GetNewName();
		if (strNewName != strOldName)
		{
			m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nProductIndexFa, strNewName);
			m_bProdNameChanged = TRUE;
			UpdateDlgData(FALSE);
			UpdateProductListDisplay();
		}
	}
}

void CDlgSysKindCalibSetting::OnBtnAmendBenchmark() 
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
LRESULT CDlgSysKindCalibSetting::OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam)
{

	if (m_vpTotalCalibData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;

	for (i=0; i<nPatternNum; i++)
	{
		if (wParam == m_vpGuiDisplay.at(i)->GetID())
		{
			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, i, 0);
			if (pSearchTool == NULL)
			{
				return S_OK;
			}

			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoCalibObjectSearchTool(nCurProdIdx,i, 0);
			m_vpGuiDisplay.at(i)->ClearScreen();

			// 标记产品模板已修改
			m_bProdPatternChanged = TRUE;
			// 标记标定指定位置模板已经修改了
			m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[i][0] = TRUE;
			return S_OK;
		}
	}

	return S_OK ;
}


LRESULT CDlgSysKindCalibSetting::OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam)
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


	for (i=0; i<nPatternNum; i++)
	{
		if (wParam == m_vpGuiDisplay[i]->GetID())
		{
			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx,i,0);

			CDlgSearchToolCopyPos dlgSearchToolCopyPos;
			dlgSearchToolCopyPos.m_nPosNum = nPatternNum;
			dlgSearchToolCopyPos.m_nPosCNum = 1;
			dlgSearchToolCopyPos.m_nSrcCandidateIndex = 0;
			dlgSearchToolCopyPos.SetSysInfoStringsRef(m_psaSysInfoStrings);

			if (dlgSearchToolCopyPos.DoModal() == IDOK)
			{
				int k = 0;
				for (k = 0; k < MAXPOSNUM && k < dlgSearchToolCopyPos.m_vbCopyPos.size(); k++)
				{
					if (dlgSearchToolCopyPos.m_vbCopyPos.at(k) == TRUE && k != i)
					{
						if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
						{
							CString strWarning;
							strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),k+1,0);
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


						// 拷贝源为空模板时，将拷贝目标位置的产品执行清空操作
						if (pSearchTool == NULL)
						{
							CBaseSearchTool *pSearchToolDst = NULL;
							pSearchToolDst = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, k, 0);
							if (pSearchToolDst == NULL)
							{
									continue;
							}

							m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoCalibObjectSearchTool(nCurProdIdx,k, 0);
							m_vpGuiDisplay.at(k)->ClearScreen();
						}
						else
						{
							//获得拷贝之前的窗口 
							scGuiDisplay* pOldDisPlay = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosDisplay(k);

							m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(nCurProdIdx, k, 0, pSearchTool);

							// 显示模板训练图像
							if (pSearchTool != NULL)
							{
								cpImage imagePattern;
								pSearchTool->GetPatternImage(imagePattern);



								m_vpGuiDisplay[k]->ClearScreen();
								m_vpGuiDisplay[k]->SetImage(imagePattern, TRUE);
							}
							else
							{
								m_vpGuiDisplay[k]->ClearScreen();
							}

							//设置拷贝之前的窗口 
							m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosDisplay(k, pOldDisPlay);
						}
						
						// 标记对象指定位置模板已经修改了
						m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[k][0] = TRUE;
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

	return S_OK ;
}


LRESULT CDlgSysKindCalibSetting::OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam)
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
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;


	for (i=0; i<nPatternNum; i++)
	{
		if (wParam == m_vpGuiDisplay[i]->GetID())
		{
				CString strLog;
				strLog.Format(_T("点击右键选择[获取定位工具从模板库]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				CDlgSearchToolModelLibrary dlgSearchToolModelLibrary;
				dlgSearchToolModelLibrary.m_vpVisionASM= m_pParent->m_vpVisionASM;
				dlgSearchToolModelLibrary.m_nCurPlatformIndex = m_pParentKind->m_nPlatformIndex;
				dlgSearchToolModelLibrary.SetSysInfoStringsRef(m_psaSysInfoStrings);
				dlgSearchToolModelLibrary.EnableSysKeyboard(m_bSysKeyboardEnabled);

				dlgSearchToolModelLibrary.m_nSearchModeFrom = eCalibSearch;
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
						strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),i+1,0);
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
								pSearchToolDstTmp = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx,i,j);
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

									return S_OK;

								}
							}
						}
					}

					bool bRet = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(nCurProdIdx,i,j,pSearchTool);
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
					m_vpGuiDisplay[i]->ClearScreen();
					m_vpGuiDisplay[i]->SetImage(imagePattern);

					// 标记产品模板已修改
					m_bProdPatternChanged = TRUE;
					// 标记指定位置目标模板已经被修改了
					m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[i][0] = TRUE;

					///////////////////////////////////////////////////////////////////////
					CString strImageContent = _T("");
					dlgSearchToolModelLibrary.GetImageContent(dlgSearchToolModelLibrary.GetSelectImageIndex(),strImageContent);

					strLog.Format(_T("将%s复制到产品%d位置%d候选%d\n"),strImageContent, m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData()->GetCurProductIndex()
						,i+1,j);
					g_opLogFile.SaveLogText(strLog,5);


					/////////////////////////////////////////////////////////////////////////
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			
		}
	}

	return S_OK ;
}

// 双击Display控件响应
LRESULT CDlgSysKindCalibSetting::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	if (m_vpTotalCalibData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		OnButtonNewProduct();
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPatternNum = platformInfo.m_nPositionNum;

	for (i=0; i<nPatternNum; i++)
	{
		if (wParam == m_vpGuiDisplay.at(i)->GetID())
		{
		    CString strLog;
		    strLog.Format(_T("双击[位置%d]标定模板"),i+1);
			g_opLogFile.SaveLogText(strLog,4);
			
			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nCurProdIdx, i, 0);

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
			if (dlgSearch.DoModal() == IDOK)
			{
				pSearchTool = dlgSearch.GetSearchTool();
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(nCurProdIdx, i, 0, pSearchTool);

				// 显示模板训练图像
				if (pSearchTool != NULL)
				{
					cpImage imagePattern;
					pSearchTool->GetPatternImage(imagePattern);
					
					m_vpGuiDisplay.at(i)->ClearScreen();
					m_vpGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
				}
				else
				{
					m_vpGuiDisplay.at(i)->ClearScreen();
				}
				// 标记产品模板已修改
				m_bProdPatternChanged = TRUE;
				// 标记标定指定位置模板已经修改了
				m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[i][0] = TRUE;
			}
			return S_OK;
		}
	}


	return S_OK ;
}


void CDlgSysKindCalibSetting::OnPlatformChange() 
{

	CString strLog;
	MoveDlgItem();

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();

	if (IsWindowVisible())
	{
		int nFaIndex = 0;  // 大品种索引
		int nChIndex = 0;  // 小品种索引
		m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetDlgCalibIndex(m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetCurCalibIndex(),nFaIndex,nChIndex);
		strLog.Format(_T("关闭产品%d（大品种%d,小品种%d）[标定]\n"),m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetCurCalibIndex()+1,nFaIndex+1,nChIndex+1);
		g_opLogFile.SaveLogText(strLog,3);
		strLog.Format(_T("关闭[工位%d][标定]\n"),m_nPrevPlatformIdx+1);
		g_opLogFile.SaveLogText(strLog,2);

		m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurCalibIndex(),nFaIndex,nChIndex);
		strLog.Format(_T("打开[工位%d][标定]\n"),m_nPlatformIndex+1);
		g_opLogFile.SaveLogText(strLog,2);
	}


	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurCalibIndex();
	m_nPrevProductIdx = m_nCurProductIdx;
	m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

	int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
	int nCurProductIdxFa = 0;
	m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);

	strLog.Format(_T("当前产品%d（大品种%d,小品种%d）[标定]\n"),m_nCurProductIdx+1,nCurProductIdxFa+1, nCurProductIdxCh+1);
	g_opLogFile.SaveLogText(strLog,3);
	m_strProductNameOldFa = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nCurProductIdxFa);
}

// 
// void CDlgSysKindCalibSetting::OnPlatformChange() 
// {
// 	CString strLog;
// 	UpdateData(TRUE);
// 	if (m_nPlatformIndex == m_nPrevPlatformIdx)
// 	{
// 		return;
// 	}
// 
// 	// 保存前一个产品的设置
// 	if (IsProductChanged())
// 	{
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
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
// // 				CString strOldDir, strNewDir;
// // 				CString strCurName = m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// // 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, m_strProductNameOld);
// // 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, strCurName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// // 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// // 				}
// // 				// 如果文件夹存在，则重命名文件夹
// // 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// // 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// // 				{
// // 					if (strCurName != m_strProductNameOld)
// // 					{						
// // 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// // 						{
// // 							//AfxMessageBox(_T("重命名失败！"));
// // 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// // 						}
// // 					}
// // 				}
// 				int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
// 				int nCurProductIdxFa = 0;
// 				m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 				m_vpTotalCalibData.at(m_nPrevPlatformIdx)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 
// 				m_vpTotalCalibData.at(m_nPrevPlatformIdx)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				//m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
// 				m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveCalibDataInfo(m_nPrevProductIdx);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// // 				CString strDir;
// // 				CString strProductName = m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// // 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPrevPlatformIdx, strProductName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 					strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 
// // 				}
// // 				strDir = m_strModuleDir + strDir;
// // 				if (!RemoveDirectoryPro(strDir))
// // 				{
// // 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 				}
// // 				m_vpTotalCalibData.at(m_nPrevPlatformIdx)->RemoveItem(m_nPrevProductIdx);
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 			}
// 			LoadSearchToolInfo();
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
// 		//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 		}
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 修改新选择的平台的显示界面
// 	UpdateDlgByAuthorize();
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0, i = 0, j = 0;
// 	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
// //	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	nPosNum = platformInfo.m_nPositionNum;
// 	for (i=0; i<MAXPOSNUM; i++)
// 	{
// 		if (i < nPosNum)
// 		{			
// 			GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(m_bShowXYD);
// 			m_vpGuiDisplay.at(i)->ShowWindow(m_bShowTemplateSet);
// 			m_vpGuiDisplay.at(i)->EnableWindow(m_bEnableTemplateSet);
// 
// 			if (i < MAXPOSNUM -1)
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
// 			}
// 			m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 			m_vpLabelPos.at(i)->ShowWindow(m_bShowXYD);
// 
// 			m_vpRadioPos.at(i)->ShowWindow(m_bShowCopyCalib);
// 			m_vpRadioPos.at(i)->EnableWindow(m_bEnableCopyCalib);
// 			
// 			m_vpCheckPos.at(i)->ShowWindow(m_bShowCopyCalib);
// 			m_vpCheckPos.at(i)->EnableWindow(m_bEnableCopyCalib);
// 
// 
// 			for(j = 0; j < 16; j++)
// 			{
// 				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->EnableWindow(m_bEnableXYD);
// 				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(m_bShowXYD);
// 			}
// 
// 			if (platformInfo.m_eCamPlatformType == eCamPlatformSepFix)
// 			{
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
// 			}
// 			else if (platformInfo.m_eCamPlatformType == eCamPlatformSepX)
// 			{
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
// 			}
// 			else if (platformInfo.m_eCamPlatformType == eCamPlatformSepXY)
// 			{
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(TRUE);
// 			}
// 			else if (platformInfo.m_eCamPlatformType == eCamPlatformShareX)
// 			{
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_X1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1+i*16)->EnableWindow(TRUE);
// 				GetDlgItem(IDC_STATIC_CAM_AXIS_Y1+i*16)->EnableWindow(FALSE);
// 				GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1+i*16)->EnableWindow(FALSE);
// 			}	
// 
// 			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(m_bShowDistortionCorrection);
// 			GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(m_bShowDistortionCorrection);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_STATIC_PATTERN_CH0+i*2)->ShowWindow(SW_HIDE);
// 			m_vpGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			if (i < MAXPOSNUM -1)
// 			{
// 				GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
// 			}
// 			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 			m_vpRadioPos.at(i)->ShowWindow(SW_HIDE);
// 			m_vpCheckPos.at(i)->ShowWindow(SW_HIDE);
// 			for(j = 0; j < 16; j++)
// 			{
// 				GetDlgItem(IDC_STATIC_AXIS_X1+i*16+j)->ShowWindow(SW_HIDE);
// 			}
// 			
// 			GetDlgItem(IDC_BTN_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_CHECK_DISTORCORR_INDEX1 + i)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	CPlatformOptionInfo optionInfo;
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
// 	if (optionInfo.m_bEnableDmCodeCalib)
// 	{
// 		GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(m_bShowDmCodeSet);
// 		GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(m_bEnableDmCodeSet);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_CMB_CALIBMODE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CMB_CALIBMODE)->EnableWindow(FALSE);
// 	}
// 	// 根据大小品种调整对话框控件
// 	MoveDlgItem();
// 
// 	UpdateDlgData(FALSE);
// 	UpdateProductListDisplay();
// 
// 	if (IsWindowVisible())
// 	{
// 		
// 
// 		int nFaIndex = 0;  // 大品种索引
// 		int nChIndex = 0;  // 小品种索引
// 		m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetDlgCalibIndex(m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetCurCalibIndex(),nFaIndex,nChIndex);
// 		strLog.Format(_T("关闭产品%d（大品种%d,小品种%d）[标定]\n"),m_vpTotalCalibData.at(m_nPrevPlatformIdx)->GetCurCalibIndex()+1,nFaIndex+1,nChIndex+1);
// 		g_opLogFile.SaveLogText(strLog,3);
// 		strLog.Format(_T("关闭[工位%d][标定]\n"),m_nPrevPlatformIdx+1);
// 		g_opLogFile.SaveLogText(strLog,2);
// 
// 		m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurCalibIndex(),nFaIndex,nChIndex);
// 		strLog.Format(_T("打开[工位%d][标定]\n"),m_nPlatformIndex+1);
// 		g_opLogFile.SaveLogText(strLog,2);
// 		//strLog.Format(_T("打开产品%d[标定]\n"),m_nCurProductIdx+1);
// 		//g_opLogFile.SaveLogText(strLog,3);
// 	}
// 	
// 
// 	m_nPrevPlatformIdx = m_nPlatformIndex;
// 	m_nCurProductIdx = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurCalibIndex();
// 	m_nPrevProductIdx = m_nCurProductIdx;
// 	m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 
// 	int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
// 	int nCurProductIdxFa = 0;
// 	m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 
// 	strLog.Format(_T("当前产品%d（大品种%d,小品种%d）[标定]\n"),m_nCurProductIdx+1,nCurProductIdxFa+1, nCurProductIdxCh+1);
// 	g_opLogFile.SaveLogText(strLog,3);
// 	m_strProductNameOldFa = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nCurProductIdxFa);
// }

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerMaxTimes() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_MAX_TIMES)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerMaxTimes();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerXPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXPrecision();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerYPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYPrecision();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerDPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDPrecision();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerXOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerYOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset();
	}	
}

void CDlgSysKindCalibSetting::OnSetfocusEditAlignerDOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset();
	}	
}


void CDlgSysKindCalibSetting::OnKillfocusEditAlignerMaxTimes() 
{
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nAlignerMaxTimes = (m_nAlignerMaxTimes<1) ? 1 : ((m_nAlignerMaxTimes>10) ? 10 : m_nAlignerMaxTimes);
		UpdateData(FALSE);
	}
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerXPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
		}
		else
		{
			m_strAlignerPrecisionX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
		}
	}	
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerYPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
		}
		else
		{
			m_strAlignerPrecisionY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
		}
	}	
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerDPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
		}
		else
		{
			m_strAlignerPrecisionD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
		}
	}	
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerXOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
		}
		else
		{
			m_strAlignerOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
		}
	}	
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerYOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
		}
		else
		{
			m_strAlignerOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
		}
	}	
}

void CDlgSysKindCalibSetting::OnKillfocusEditAlignerDOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
		}
		else
		{
			m_strAlignerOffsetD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
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

void CDlgSysKindCalibSetting::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strLog;
	int nProductIndexFa = 0;
	int nProductIndexChd = 0;

	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);

	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);

	if (nSelected < 0)
	{
		m_pParentKind->m_pDlgKindAlignView->SetProductListSelect(nProductIndexFa, nProductIndexChd);
		return;
	}

	int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nProductIndexFa,nSelected);
	if (m_pParentKind->m_pDlgKindAlignView->CheckMemUseRateWarning(nDstProductIndex))
	{
		return;
	}

	if ((nDstProductIndex >= 0) && (nDstProductIndex != m_nCurProductIdx))
	{
		strLog.Format(_T("点击大品种%d小品种列表：小品种%d切换到小品种%d\n"),nProductIndexFa+1,nProductIndexChd+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,5);
		m_pParentKind->m_pDlgKindAlignView->SetCurProductIndex(nDstProductIndex);
	}

	*pResult = 0;
}

void CDlgSysKindCalibSetting::OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult)
{

	CString strLog;
	int nProductIndexFa = 0;
	int nProductIndexChd = 0;

	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);

	POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);

	if (nSelected < 0)
	{
		m_pParentKind->m_pDlgKindAlignView->SetProductListSelect(nProductIndexFa, nProductIndexChd);
		return;
	}

	if(nSelected == nProductIndexFa)
	{
		return;
	}

	nProductIndexChd = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibLastChdIndex(nSelected);
	int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSelected, nProductIndexChd);
	if (m_pParentKind->m_pDlgKindAlignView->CheckMemUseRateWarning(nDstProductIndex))
	{
		return;
	}	 

	if ((nDstProductIndex >= 0) && (nDstProductIndex != m_nCurProductIdx))
	{
		strLog.Format(_T("点击大品种列表：大品种%d切换到大品种%d\n"),nProductIndexFa+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,5);
		m_pParentKind->m_pDlgKindAlignView->SetCurProductIndex(nDstProductIndex);
	}

	*pResult = 0;
}

// void CDlgSysKindCalibSetting::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	int nProductIndexFa, nProductIndexChd;
// 	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
// 
// 	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
// 	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
// 
// 	if (nSelected >= 0)
// 	{
// 		if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
// 			int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
// 			int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nProductIndexFa,nSelected);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 
// 			}
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 
// 
// 			}
// 			else
// 			{
// 				if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
// 				{
// 					CString strWarning;
// 					strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANGE_PRODUCT_FAIL_OF_MEM_OUT));
// 					strWarning = strWarning + 
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
// 					MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
// 					m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 					return ;
// 				}
// 
// 			}
// 
// 		}
// 	}
// 
// 	
// 	
// 	if (nSelected < 0)
// 	{
// 		m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 	}
// 	else
// 	{
// 		m_nCurProductIdx = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nProductIndexFa,nSelected);
// 	}
// 
// 	//CString strLog;
// 	//strLog.Format(_T("点击小品种列表：小品种%d切换到小品种%d[标定]\n"),nProductIndexFa+1,nSelected+1);
// 	//g_opLogFile.SaveLogText(strLog,5);
// 	//strLog.Format(_T("关闭产品%d[标定]\n"),m_nPrevProductIdx+1);
// 	//g_opLogFile.SaveLogText(strLog,3);
// 	//strLog.Format(_T("打开产品%d[标定]\n"),m_nCurProductIdx+1);
// 	//g_opLogFile.SaveLogText(strLog,3);
// 	//strLog.Format(_T("关闭产品%d[标定]\n"),m_nPrevProductIdx+1);
// 
// 	CString strLog;
// 	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
// 	{
// 		strLog.Format(_T("点击标定信息大品种%d小品种列表：小品种%d切换到小品种%d[标定]\n"),nProductIndexFa+1,nProductIndexChd+1,nSelected+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		strLog.Format(_T("标定信息产品序号%d切换到标定信息产品序号%d\n"),m_nPrevProductIdx+1,m_nCurProductIdx+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		CString strProductChange;
// 		strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_CHANGING);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
// 
// 		// 保存前一个产品的设置
// 		if (IsProductChanged())
// 		{
// 			strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT) + "\n";
// 			g_opLogFile.SaveLogText(strLog,5);
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 			if (nMBReturn == IDYES)
// 			{
// 				strLog = _T("弹窗选择：是\n");
// 				g_opLogFile.SaveLogText(strLog,5);
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
// 				strLog = _T("弹窗选择：取消\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
// 				m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 				m_nCurProductIdx = m_nPrevProductIdx;
// 				return;
// 			}
// 			else // 不保存对产品的修改
// 			{
// 				strLog = _T("弹窗选择：否\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				if (m_bProdNameChanged)
// 				{
// // 					CString strOldDir, strNewDir;
// // 					CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// // 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// // 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// // 					}
// // 					// 如果文件夹存在，则重命名文件夹
// // 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// // 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// // 					{
// // 						if (strCurName != m_strProductNameOld)
// // 						{						
// // 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// // 							{
// // 								//AfxMessageBox(_T("重命名失败！"));
// // 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// // 							}
// // 						}
// // 					}
// 
// 					int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
// 					int nCurProductIdxFa = 0;
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
// 					//					UpdateDlgData(FALSE);
// 				}
// 				if (m_bProdNewCreate)
// 				{
// // 					CString strDir;
// // 					CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 						strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 					}
// // 					strDir = m_strModuleDir + strDir;
// // 					if (!RemoveDirectoryPro(strDir))
// // 					{
// // 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 					}
// // 					m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 				}
// 				if (m_bProdNewCreate || m_bProdNameChanged)
// 				{
// 					UpdateProductListDisplay();
// 				}
// 
// 				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 				{                        
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 				}
// 				LoadSearchToolInfo();
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		ClipMouseCursor(TRUE);//锁定鼠标
// 		// 释放原来产品的定位工具20170516
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 				CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// 				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 				if (pPreProduct != NULL)
// 				{
// 					pPreProduct->DeleteObjectSearchTools();
// 				}
// 			}
// 		}
// 
// 		m_vpTotalCalibData.at(m_nPlatformIndex)->SetCurCalibIndex(m_nCurProductIdx);
// 		m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetProductCameraShutter(m_nCurProductIdx); 
// 		int nCurProductIdxFa = 0;
// 		int nCurProductIdxCh = 0;
// 		m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx,nCurProductIdxFa,nCurProductIdxCh);
// 		strLog.Format(_T("当前标定信息产品%d：大品种%d,小品种%d[标定]\n"),m_nCurProductIdx+1,nCurProductIdxFa+1, nCurProductIdxCh+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 		//m_nPrevProductIdx = m_nCurProductIdx;
// 		UpdateDlgData(FALSE);
// 
// 		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 		}
// 		m_listctrlProduct.SetItemState(nSelected, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 
// 		ClipMouseCursor(FALSE);//解除鼠标锁定
// 		m_pParentKind->m_pDlgKindAlignView->SetCurProductIndex(m_nCurProductIdx);
// 		m_pParentKind->SetProductSearchToolChanged(false);
// 		m_pParentKind->SetCalibObjectSearchToolChanged(false);
// 		strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_CHANGE_SUCCEED);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
// 		
// 	}
// 
// 	*pResult = 0;
// }
// 
// void CDlgSysKindCalibSetting::OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	CString strLog;
// 	int nProductIndexFa, nProductIndexChd;
// 	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nProductIndexFa, nProductIndexChd);
// 
// 	POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
// 	int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);
// 	if (nSelected >= 0)
// 	{
// 		if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
// 			int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);
// 			// 读取当前大品种的上一次选中的小品种
// 			nProductIndexChd = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibLastChdIndex(nSelected);
// 			int nDstProductIndex=m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSelected,nProductIndexChd);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 
// 			}
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 
// 
// 			}
// 			else
// 			{
// 				if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
// 				{
// 					CString strWarning;
// 					strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANGE_PRODUCT_FAIL_OF_MEM_OUT));
// 					strWarning = strWarning + 
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
// 					MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
// 					m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 					return ;
// 				}
// 
// 			}
// 
// 		}
// 	}
// 	
// 	if (nSelected < 0)
// 	{
// 		m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 		return;
// 	}
// 
// 	if(nSelected == nProductIndexFa)
// 		return;
// 
// 	CString strProductChange;
// 	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_CHANGING);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
// 
// 	strLog.Format(_T("点击标定信息大品种列表：大品种%d切换到大品种%d[标定]\n"),nProductIndexFa+1,nSelected+1);
// 	g_opLogFile.SaveLogText(strLog,5);
// 
// 	if (IsProductChanged())
// 	{
// 		strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT) + "\n";
// 		g_opLogFile.SaveLogText(strLog,5);
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 		if (nMBReturn == IDYES)
// 		{
// 			strLog = _T("弹窗选择：是\n");
// 			g_opLogFile.SaveLogText(strLog,5);
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
// 			strLog = _T("弹窗选择：取消\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
// 			m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 			m_nCurProductIdx = m_nPrevProductIdx;
// 			return;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			strLog = _T("弹窗选择：否\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			if (m_bProdNameChanged)
// 			{
// // 					CString strOldDir, strNewDir;
// // 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// // 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// // 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// // 					}
// // 					// 如果文件夹存在，则重命名文件夹
// // 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// // 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// // 					{
// // 						if (strCurName != m_strProductNameOld)
// // 						{						
// // 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// // 							{
// // 								//AfxMessageBox(_T("重命名失败！"));
// // 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// // 							}
// // 						}
// // 					}
// 				int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
// 				int nCurProductIdxFa = 0;
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 				//					UpdateDlgData(FALSE);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// // 					CString strDir;
// // 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 					//strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%03d"), m_nPlatformIndex, m_nPrevProductIdx+1);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 						strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 					}
// // 					strDir = m_strModuleDir + strDir;
// // 					if (!RemoveDirectoryPro(strDir))
// // 					{
// // 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 					}
// // 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 			}
// 			if (m_bProdNewCreate || m_bProdNameChanged)
// 			{
// 				UpdateProductListDisplay();
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 			}
// 			LoadSearchToolInfo();
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 		}
// 	}
// 
// 	ClipMouseCursor(TRUE);//锁定鼠标
// 	// 释放原来产品的定位工具20170516
// 	{
// 		SysPlatformInfo sysPlatformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 		{
// 			CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// 			CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 			if (pPreProduct != NULL)
// 			{
// 				pPreProduct->DeleteObjectSearchTools();
// 			}
// 		}
// 		// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 		{
// 			CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
// 			for (int i=0; i<pTotalProduct->GetCalibCountFa(); i++ )
// 			{
// 				if(i == nSelected) continue;
// 
// 				for (int j =0; j< pTotalProduct->GetCalibCountChd(i); j++)
// 				{
// 					int nProductIndex = pTotalProduct->GetCalibIndex(i, j);
// 					CCalibData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 					if (pPreProductData != NULL)
// 					{
// 						pPreProductData->DeleteObjectSearchTools();
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 
// 	// 保存记录
// 	m_vpTotalCalibData.at(m_nPlatformIndex)->UpdateCalibLastChdIndex(nProductIndexFa, nProductIndexChd);
// 
// 	// 读取当前大品种的上一次选中的小品种
// 	nProductIndexChd = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibLastChdIndex(nSelected);
// 
// 	// 将界面上的索引转换成内部索引，并更新为当前索引
// 	m_nCurProductIdx = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSelected,nProductIndexChd);
// 
// 	strLog.Format(_T("当前标定信息%d：大品种%d,小品种%d[标定]\n"),m_nCurProductIdx+1,nSelected+1, nProductIndexChd+1);
// 	g_opLogFile.SaveLogText(strLog,5);
// 
// 	// 设置当前索引
// 	m_vpTotalCalibData.at(m_nPlatformIndex)->SetCurCalibIndex(m_nCurProductIdx);
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetProductCameraShutter(m_nCurProductIdx); 
// 
// 	m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 	m_strProductNameOldFa = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nSelected);	//zzc大产品名称
// 
// 	UpdateDlgData(FALSE);
// 
// 	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 	}
// 
// 	//m_listctrlProductFa.SetItemState(nSelected, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 	UpdateProductListDisplay();
// 	ClipMouseCursor(FALSE);//解除鼠标锁定
// 	m_pParentKind->m_pDlgKindAlignView->SetCurProductIndex(m_nCurProductIdx);
// 	m_pParentKind->SetProductSearchToolChanged(false);
// 	m_pParentKind->SetCalibObjectSearchToolChanged(false);
// 	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_CHANGE_SUCCEED);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
// 	
// }

void CDlgSysKindCalibSetting::SetComboCalibCopySrcIndex(int nSrc)
{
	m_comboProductSrc.SetCurSel(nSrc);
	UpdateData(FALSE);
}

void CDlgSysKindCalibSetting::SetComboCalibCopyDstIndex(int nDst)
{
	m_comboProductDst.SetCurSel(nDst);
	UpdateData(FALSE);
}

void CDlgSysKindCalibSetting::SetCurProductIndex(int nCurProdcutIndex)
{
	int nCurProductIndexFa, nCurProductIndexChd;
	m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nCurProductIndexFa, nCurProductIndexChd);

	if (nCurProdcutIndex < 0)
	{
		m_listctrlProduct.SetItemState(nCurProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
		m_listctrlProductFa.SetItemState(nCurProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		m_nCurProductIdx = nCurProdcutIndex;
	}

	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
	{
		// 保存前一个产品的设置
		if (IsProductChanged())
		{
			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
			if (nMBReturn == IDYES)
			{
				if (SavePrevProduct())
				{
					// 恢复修改标志
					m_bProdPatternChanged = FALSE;
					m_bProdNameChanged = FALSE;
					m_bProdBenchMarkChanged = FALSE;
					m_bProdNewCreate = FALSE;
					m_bCalibInfoChanged = FALSE;
				}
			}
			else if (nMBReturn == IDCANCEL)
			{
				m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
				m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
				m_nCurProductIdx = m_nPrevProductIdx;
				return;
			}
			else // 不保存对产品的修改
			{
				if (m_bProdNameChanged)
				{
					// 					CString strOldDir, strNewDir;
					// 					CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
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
					int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
					int nCurProductIdxFa = 0;
					m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nPrevProductIdx, nCurProductIdxFa, nCurProductIdxCh);
					m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

					m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nPrevProductIdx);
					//					UpdateDlgData(FALSE);
				}
				if (m_bProdNewCreate)
				{
// 					CString strDir;
// 					CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 					{
// 						SysPlatformInfo platformInfo;
// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 						//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 						strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 					}
// 					strDir = m_strModuleDir + strDir;
// 					if (!RemoveDirectoryPro(strDir))
// 					{
// 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 					}
// 					m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
				}
				if (m_bProdNewCreate || m_bProdNameChanged)
				{
					UpdateProductListDisplay();
				}

				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
				{                        
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
				}
				LoadSearchToolInfo();

				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}

		// ShiYue 20190228
		{
			SysPlatformInfo sysPlatformInfo;
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
			{
				CTotalCalibData *pTotalProduct = NULL;
				pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);

				if (pPreProduct != NULL)
				{
					pPreProduct->DeleteObjectSearchTools();
				}
			}
			// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
			{
				int nTempCurProductIndexFa, nTempCurProductIndexChd;
				//根据当前产品序号，获得当前应该修改的大产品序号
				m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(nCurProdcutIndex,nTempCurProductIndexFa,nTempCurProductIndexChd);
				if(nCurProductIndexFa != nTempCurProductIndexFa)
				{
					CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
					for (int i=0; i<pTotalProduct->GetCalibCountFa(); i++ )
					{
						if(i == nTempCurProductIndexFa) continue;

						for (int j =0; j< pTotalProduct->GetCalibCountChd(i); j++)
						{
							int nProductIndex = pTotalProduct->GetCalibIndex(i, j);
							CCalibData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
							if (pPreProductData != NULL)
							{
								pPreProductData->DeleteObjectSearchTools();
							}
						}
					}
				}
			}
		}

		m_vpTotalCalibData.at(m_nPlatformIndex)->SetCurCalibIndex(m_nCurProductIdx);
		m_strProductNameOld = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

		int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
		int nCurProductIdxFa = 0;
		m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);
		m_strProductNameOldFa = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibNameFa(nCurProductIdxFa);

//		m_nPrevProductIdx = m_nCurProductIdx;

		UpdateDlgData(FALSE);

		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
		{
			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
		{
			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
		}

//		m_listctrlProduct.SetItemState(m_nCurProductIdx, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
		UpdateProductListDisplay();
	}

}

BOOL CDlgSysKindCalibSetting::DestroyWindow() 
{
	int i = 0, j = 0;
	int nSize ;

	nSize = m_vpGuiDisplay.size();
	for (i=0; i<nSize; i++)
	{
		delete m_vpGuiDisplay.at(i);
		m_vpGuiDisplay.at(i) = NULL;
	}
	m_vpGuiDisplay.clear();

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
	m_fontChangeProduct.DeleteObject();
	return CDialog::DestroyWindow();
}



BOOL CDlgSysKindCalibSetting::OnMyOK()
{
	if (IsProductChanged())
	{
		if (SavePrevProduct())
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
			m_pParentKind->SetCalibObjectSearchToolChanged(FALSE);
		}
		else
		{
			//AfxMessageBox(_T("产品保存失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_SAVE_PRODUCT));
			return FALSE;
		}	
	}
	else
	{
		// 保存产品概要信息到磁盘
		//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
		}
	}
	
	return TRUE;
}


BOOL CDlgSysKindCalibSetting::CancelProductChange()
{
	if (m_bProdNameChanged)
	{
		// 				CString strOldDir, strNewDir;
		// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
		// 
		// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
		// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
		// 				{
		// 					SysPlatformInfo platformInfo;
		// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
		// 					pVisionASM->GetSysPlatformInfo(platformInfo);
		// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
		// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
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

		int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
		int nCurProductIdxFa = 0;
		m_vpTotalCalibData.at(m_nPlatformIndex)->GetCurDlgCalibIndex(nCurProductIdxFa, nCurProductIdxCh);
		m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

		m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nCurProductIdx);
	}
	if (m_bProdNewCreate)
	{
// 		CString strDir;
// 		CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 		//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 		{
// 			SysPlatformInfo platformInfo;
// 			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 			pVisionASM->GetSysPlatformInfo(platformInfo);
// 			//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 			strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
// 		}
// 		strDir = m_strModuleDir + strDir;
// 		if (!RemoveDirectoryPro(strDir))
// 		{
// 			//AfxMessageBox(_T("删除产品数据文件失败！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 		}
// 		m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
	}

	if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	{                        
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	}

	// 恢复修改标志
	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;

	return TRUE;
}


BOOL CDlgSysKindCalibSetting::OnMyCancel()
{
	// 如果当前产品已修改，提示是否保存当前产品的设置
	if (IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
				m_pParentKind->SetCalibObjectSearchToolChanged(FALSE);
			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			return FALSE;
		}
		else // 不保存对产品的修改
		{
			if (m_bProdNameChanged)
			{
// 				CString strOldDir, strNewDir;
// 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 
// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
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
				int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
				int nCurProductIdxFa = 0;
				m_vpTotalCalibData.at(m_nPlatformIndex)->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);
				m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nCurProductIdxFa, m_strProductNameOldFa);

				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nCurProductIdx);
			}
			if (m_bProdNewCreate)
			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 				}
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
			}

			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
			{                        
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
			}
			LoadSearchToolInfo();
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
	}
	
	CString strLog;
	strLog.Format(_T("[不保存]退出标定"));
	g_opLogFile.SaveLogText(strLog,3);
	
	// 释放原来产品的定位工具20181204-mfq
	{
        if (m_pParentKind->m_pDlgKindAlignView->m_nCurProductIdx != m_pParentKind->m_pDlgKindAlignView->m_nOldProductIdx)
        {
			SysPlatformInfo sysPlatformInfo;
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
			{
				CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
				CCalibData *pPreProduct = pTotalProduct->GetItemPtr(m_nCurProductIdx);
				if (pPreProduct != NULL)
				{
					pPreProduct->DeleteObjectSearchTools();
				}
			}
			// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
			{
				CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);

				int nOldProductIndexFa = 0;
				int nOldProductIndexChd = 0;
				pTotalProduct->GetDlgCalibIndex(m_pParentKind->m_pDlgKindAlignView->m_nOldProductIdx, nOldProductIndexFa, nOldProductIndexChd);

				for (int i=0; i<pTotalProduct->GetCalibCountFa(); i++ )
				{
					if(i == nOldProductIndexFa) continue;

					for (int j =0; j< pTotalProduct->GetCalibCountChd(i); j++)
					{
						int nProductIndex = pTotalProduct->GetCalibIndex(i, j);
						CCalibData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
						if (pPreProductData != NULL)
						{
							pPreProductData->DeleteObjectSearchTools();
						}
					}
				}
			}
        }
	}

	return TRUE;
}

void CDlgSysKindCalibSetting::OnOK() 
{
	if (!OnMyOK())
	{
		return;
	}
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
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
// 	}

	CDialog::OnOK();
}

void CDlgSysKindCalibSetting::OnCancel() 
{
	if (!OnMyCancel())
	{
		return;
	}
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
// // 				CString strOldDir, strNewDir;
// // 				CString strCurName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// // 
// // 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// // 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					strOldDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// // 					strNewDir.Format(_T("%s%s\\JobCalibs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// // 				}
// // 				// 如果文件夹存在，则重命名文件夹
// // 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// // 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// // 				{
// // 					if (strCurName != m_strProductNameOld)
// // 					{						
// // 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// // 						{
// // 							//AfxMessageBox(_T("重命名失败！"));
// // 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// // 						}
// // 					}
// // 				}
// 
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibDataInfo(m_nCurProductIdx);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalCalibData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\JobCalibs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\JobCalibs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 				}
// 				m_vpTotalCalibData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadCalibDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
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

void CDlgSysKindCalibSetting::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}


void CDlgSysKindCalibSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}
void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement1();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement1();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置1][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime1();
	}
}




void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement2();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement2();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement2();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement2();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement2();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement2();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime1();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置2][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime2();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement3();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement3();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement3();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement3();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement3();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement3();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime3();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置3][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime3();
	}
}




void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement4();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement4();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement4();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement4();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement4();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement4();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime4();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置4][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime4();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement5();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement5();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement5();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement5();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement5();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement5();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime5();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置5][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime5();
	}
}





void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement6();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement6();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement6();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement6();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement6();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement6();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime6();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置6][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime6();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement7();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement7();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime7() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME7)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME7, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME7, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置7][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime7();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditXAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_X_AXIS_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][平台X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditXAxisMovement8();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditYAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_Y_AXIS_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][平台Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditYAxisMovement8();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditDAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_D_AXIS_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][平台θ移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditDAxisMovement8();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraXAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][相机X移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraXAxisMovement8();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditCameraYAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][相机Y移动]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditCameraYAxisMovement8();
	}
}



void CDlgSysKindCalibSetting::OnSetfocusEditRefineDAxisMovement8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][校正角度]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineAxisDMovement8();
	}
}


void CDlgSysKindCalibSetting::OnSetfocusEditRefineTime8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REFINE_TIME8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REFINE_TIME8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][校正次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRefineTime8();
	}
}

void CDlgSysKindCalibSetting::OnSetfocusEditSearchDelayTime8() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME8)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME8, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME8, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[位置8][搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime8();
	}
}

void CDlgSysKindCalibSetting::OnBnClickedBtnCopyCalibDataFa()
{

	int nSrcFaProductIndex = m_comboProductSrcFa.GetCurSel();
	int nDstFaProductIndex = m_comboProductDstFa.GetCurSel();

	//源小品种数量和目标小品种数量
	int nSrcProductNumChd = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibCountChd(nSrcFaProductIndex);
	int nDstProductNumChd = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibCountChd(nDstFaProductIndex);
	CString strLog; 
	strLog.Format(_T("点击标定信息大品种拷贝：大品种%d拷贝到大品种%d[标定]\n"),nSrcFaProductIndex+1,nDstFaProductIndex+1);
	g_opLogFile.SaveLogText(strLog,5);

	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		int nSrcProductIndexChild=m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibLastChdIndex(nSrcFaProductIndex);
		int nSrcProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSrcFaProductIndex,nSrcProductIndexChild);//获取源大产品上一个小产品序号
		int nDstProductIndexChild=m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibLastChdIndex(nDstFaProductIndex);
		int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nDstFaProductIndex,nDstProductIndexChild);//获取目标大产品上一个小产品序号
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nSrcProductIndex)
				|| !m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_PRODUCT_FAIL_OF_MEM_OUT));
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return ;
			}

		}

	}
	ClipMouseCursor(TRUE);
	CString strProductChange;
	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_COPYING);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);	

	
	ClipMouseCursor(FALSE);
	if(nSrcProductNumChd > nDstProductNumChd)//目标小品种不够，先补齐不够的小产品
	{
		for(int i=0; i<nSrcProductNumChd - nDstProductNumChd ;i++)
		{
			//创建新的产品需要从产品浏览页面创建
			m_pParentKind->m_pDlgKindAlignView->NewProduct(nDstFaProductIndex,nDstProductNumChd + i);
		}
	}
	else if(nSrcProductNumChd < nDstProductNumChd)//目标小品种多，则清空多余的
	{
		for(int i=nSrcProductNumChd;i<nDstProductNumChd;i++)
		{
			int nIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nDstFaProductIndex,i);
			ClearCurProduct(nIndex);		
		}
	}
	ClipMouseCursor(TRUE);
	for(int i=0; i<nSrcProductNumChd; i++)
	{
		int nSrcProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSrcFaProductIndex,i);
		int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nDstFaProductIndex,i);
		CopyCalibData(nSrcProductIndex, nDstProductIndex);
	}
	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_COPY_SUCCEED);;
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
	ClipMouseCursor(FALSE);
}

void CDlgSysKindCalibSetting::OnBnClickedBtnCopyCalibData()
{

	int nSrcProductIndexFa = m_comboProductSrcFa.GetCurSel();
	int nSrcProductIndexChd = m_comboProductSrc.GetCurSel();
	int nSrcProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nSrcProductIndexFa,nSrcProductIndexChd);

	int nDstProductIndexFa = m_comboProductDstFa.GetCurSel();
	int nDstProductIndexChd = m_comboProductDst.GetCurSel();
	int nDstProductIndex = m_vpTotalCalibData.at(m_nPlatformIndex)->GetCalibIndex(nDstProductIndexFa,nDstProductIndexChd);

	CString strLog; 
	strLog.Format(_T("点击标定信息小品种拷贝：大品种%d小品种%d拷贝到大品种%d小品种%d[标定]\n"),nSrcProductIndexChd+1,nSrcProductIndexFa+1,nDstProductIndexFa+1,nDstProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,5);

	// TODO: 在此添加控件通知处理程序代码
	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nSrcProductIndex)
				|| !m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_PRODUCT_FAIL_OF_MEM_OUT));
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return ;
			}

		}

	}
	ClipMouseCursor(TRUE);
	CString strProductChange;
	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_COPYING);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);	

	

	
	strLog.Format(_T("点击[产品复制]由产品%d复制到产品%d\n"),nSrcProductIndex+1,nDstProductIndex+1);
	g_opLogFile.SaveLogText(strLog,4);

	CopyCalibData(nSrcProductIndex, nDstProductIndex);

	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_COPY_SUCCEED);;
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowTextA(strProductChange);
	ClipMouseCursor(FALSE);
}

void CDlgSysKindCalibSetting::CopyCalibData(int nSrcProductIndex, int nDstProductIndex)
{
	if (nDstProductIndex < 0 || nSrcProductIndex < 0)
	{
		return;
	}

	if (nDstProductIndex == nSrcProductIndex)
	{
		return;
	}

	if (nSrcProductIndex == m_nCurProductIdx)
	{
		if (IsProductChanged())
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}	
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalCalibData *pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);

	CCalibData *pSrcProduct = pTotalProduct->GetItemPtr(nSrcProductIndex);
	if (pSrcProduct == NULL)
	{
		return;
	}	

	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitCalibObjectSearchToolRepository(nSrcProductIndex);
	}

	CCalibData *pDstProduct = pTotalProduct->GetItemPtr(nDstProductIndex);
	if (pDstProduct != NULL)
	{
		pDstProduct->DeleteObjectSearchTools();
	}

	CString strSrcProductName;
	strSrcProductName = pTotalProduct->GetItemName(nDstProductIndex);
	if (pTotalProduct->CopyItem(nSrcProductIndex, nDstProductIndex))
	{
		pTotalProduct->SetItemName(nDstProductIndex, strSrcProductName);
	}
	//m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(TRUE);
	if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(TRUE))
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(TRUE);
	}


	pDstProduct = pTotalProduct->GetItemPtr(nDstProductIndex);
	if (pDstProduct != NULL)
	{
		pDstProduct->ResetObjectSearchTools();
	}


	int nSrcProductIndexFa = 0;
	int nSrcProductIndexChd = 0;
	int nDstProductIndexFa = 0;
	int nDstProductIndexChd = 0;
	int nCurProductIndexFa = 0;
	int nCurProductIndexChd = 0;
	pTotalProduct->GetDlgCalibIndex(nSrcProductIndex, nSrcProductIndexFa, nSrcProductIndexChd);
	pTotalProduct->GetDlgCalibIndex(nDstProductIndex, nDstProductIndexFa, nDstProductIndexChd);
	pTotalProduct->GetDlgCalibIndex(m_nCurProductIdx, nCurProductIndexFa, nCurProductIndexChd);


	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPatternNum = platformInfo.m_nPositionNum;
	int i = 0, j = 0;	

	// 标定对象Mark模板拷贝
	CBaseSearchTool* pObjSearchTool = NULL;
//	scImageGray image;
	for (i=0; i < nPatternNum; i++)
	{
		for(j = 0; j<CALIBOBJECTCANDIDATENUM; j++)
		{
			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(nSrcProductIndex, i, j);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(nDstProductIndex, i, j, pObjSearchTool);
		}
	}
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibObjectSearchToolRepository(nDstProductIndex);
	pDstProduct->m_bCalibObjectSearchToolInited = TRUE;

	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteObjectSearchTools();
			}

			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteObjectSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteObjectSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteObjectSearchTools();
			}
		}
	}


	if (nDstProductIndex == m_nCurProductIdx)
	{
		UpdateDlgData(FALSE);
		UpdateProductListDisplay();
		//如果目标产品中不存在当前产品时，工具改变标识符不发生改变
		// 标定复制时，仅改变标定所有的标识符
		m_pParentKind->SetCalibObjectSearchToolChanged(true);
	}	


}


void CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibSrc()
{
	// TODO: 在此添加控件通知处理程序代码
	int m_nSrcProductIndexOld = m_nSrcProductIndex;
	m_nSrcProductIndex = m_comboProductSrc.GetCurSel();
	if (m_nSrcProductIndexOld != m_nSrcProductIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[复制产品]中被复制对象由%d到%d\n"),m_nSrcProductIndexOld+1,m_nSrcProductIndex+1);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibSrcFa()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData = m_pParentKind->m_pDlgKindAlignView->m_vpTotalProductData.at(m_nPlatformIndex);
	int nSrcProductIndexFa = m_comboProductSrcFa.GetCurSel();
	int nCount =  pTotalProductData->GetProductCountChd(nSrcProductIndexFa);
	m_comboProductSrc.ResetContent();
	CTotalCalibData*  pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	for(int i=0; i < nCount; i++)
	{
		CString strTemp,strTemp1;
		// 序号
		strTemp.Format(_T("%03d"), i+1);

		int nProductIndex =  pTotalProductData->GetProductIndex(nSrcProductIndexFa,i);
		// 产品名称
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_comboProductSrc.InsertString(i, strTemp + _T("|") + strTemp1);
	}
	m_comboProductSrc.SetCurSel(0);
//	m_comboProductSrc.InsertString(nCount, _T("ALL"));
	m_nSrcProductIndexFa = nSrcProductIndexFa;
}

void CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibDst()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_nDstProductIndex = m_comboProductDst.GetCurSel();
	int m_nDstProductIndexOld = m_nDstProductIndex;
	m_nDstProductIndex = m_comboProductDst.GetCurSel();
	if (m_nDstProductIndexOld != m_nDstProductIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[复制产品]中复制对象由产品%d到产品%d\n"),m_nDstProductIndexOld+1,m_nDstProductIndex+1);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindCalibSetting::OnCbnSelchangeComboCalibDstFa()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData = m_pParentKind->m_pDlgKindAlignView->m_vpTotalProductData.at(m_nPlatformIndex);
	int nDstFaProductIndex = m_comboProductDstFa.GetCurSel();
	int nCount =  pTotalProductData->GetProductCountChd(nDstFaProductIndex);
	m_comboProductDst.ResetContent();
	CTotalCalibData*  pTotalProduct = m_vpTotalCalibData.at(m_nPlatformIndex);
	for(int i=0; i < nCount; i++)
	{
		CString strTemp,strTemp1;
		// 序号
		strTemp.Format(_T("%03d"), i+1);

		int nProductIndex =  pTotalProductData->GetProductIndex(nDstFaProductIndex,i);
		// 产品名称
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_comboProductDst.InsertString(i, strTemp + _T("|") + strTemp1);
	}
	//m_comboProductDst.InsertString(nCount, _T("ALL"));
	m_comboProductDst.SetCurSel(0);
	m_nDstProductIndexFa = nDstFaProductIndex;
}

void CDlgSysKindCalibSetting::OnBnClickedBtnCopyPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsProductChanged())
	{
		if (SavePrevProduct())
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
		;
	}

	UpdateData(TRUE);
	
	CTotalCalibData *pTotalCalibData;
	pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
	CCalibData* pCalibData;
	pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
	if (pCalibData == NULL)
	{
		return;
	}	

	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPatternNum = platformInfo.m_nPositionNum;
	int i = 0, j = 0;	
	CCalibrateParam calibParam;
	CBaseSearchTool* pObjSearchTool = NULL;
	int nSrcPosIndex = m_nRadioPosIndex;
	int nDstPosIndex = 0;
	pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoCalibObjectSearchTool(m_nCurProductIdx, nSrcPosIndex, 0);
	for(i = 0; i < nPatternNum; i++)
	{
		if (m_vpCheckPos.at(i)->GetCheck() == BST_CHECKED && i != nSrcPosIndex)
		{
			if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),i+1,0);
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return;

			}
			//获得拷贝之前的窗口 
			scGuiDisplay* pOldDisPlay = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosDisplay(i);

			nDstPosIndex = i;
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoCalibObjectSearchTool(m_nCurProductIdx, nDstPosIndex, 0);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoCalibObjectSearchTool(m_nCurProductIdx, nDstPosIndex, 0, pObjSearchTool);
			m_bProdPatternChanged  = TRUE;

			if (pCalibData->m_vpCalibrateParam[nSrcPosIndex] != NULL)
			{
				*pCalibData->m_vpCalibrateParam.at(i) = *pCalibData->m_vpCalibrateParam[nSrcPosIndex];
			}
			
			
			if (pCalibData->m_vpCalibratedInfo[nSrcPosIndex] != NULL)
			{
				*pCalibData->m_vpCalibratedInfo.at(i) = *pCalibData->m_vpCalibratedInfo[nSrcPosIndex];
			}

			if (platformInfo.m_bEnableMultiCalibExtension)
			{
				// 单个产品扩展多标定
				for (int nExIndex = 0; nExIndex < platformInfo.m_nMultiCalibExtensionMaxNum; nExIndex++)
				{
					if (pCalibData->m_vpCalibratedInfo[nSrcPosIndex + nPatternNum*(nExIndex+1)] != NULL)
					{
						*pCalibData->m_vpCalibratedInfo.at(i + nPatternNum*(nExIndex+1)) = *pCalibData->m_vpCalibratedInfo[nSrcPosIndex + nPatternNum*(nExIndex+1)];
					}
				}
			}
			
			

			//设置拷贝之前的窗口 
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosDisplay(i, pOldDisPlay); 

			// bug修复：执行复制模板后,模板信息未保存磁盘的bug
			m_bProdPatternChanged = TRUE;
			m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged[i][0] = TRUE;
			CString strLog;
			strLog.Format(_T("点击[复制模板]将位置%d复制到位置%d\n"),nSrcPosIndex+1,nDstPosIndex+1);
			g_opLogFile.SaveLogText(strLog,4);
		}		
	}
	UpdateDlgData(FALSE);
	
}


BOOL CDlgSysKindCalibSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysKindCalibSetting::OnDblclkStaticPos0()
{
	ShowCalibratedResult(0);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos1()
{
	ShowCalibratedResult(1);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos2()
{
	ShowCalibratedResult(2);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos3()
{
	ShowCalibratedResult(3);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos4()
{
	ShowCalibratedResult(4);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos5()
{
	ShowCalibratedResult(5);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos6()
{
	ShowCalibratedResult(6);
}

void CDlgSysKindCalibSetting::OnDblclkStaticPos7()
{
	ShowCalibratedResult(7);
}


bool CDlgSysKindCalibSetting::GetCalibratedResultDetailDescription(CString &strDescription,vcBaseVisionAlign *pVisionASM,int nPosIndex)
{
	if(!pVisionASM)
	{
		return false;
	}
	// 如果当前位置超限，则直接返回不做处理
	SysPlatformInfo platformInfo;
	pVisionASM->GetSysPlatformInfo(platformInfo);
	int nPosNum = platformInfo.m_nPositionNum;
	if (nPosIndex >= nPosNum)
	{
		return false;
	}

	// 获取当前平台对应的全部标定数据
	CTotalCalibData *pTotalCalibData;
	pTotalCalibData = pVisionASM->GetTotalCalibData();
	if (pTotalCalibData == NULL)
	{
		return false;
	}

	// 获取当前产品对应的标定数据
	CCalibData* pCalibData;
	pCalibData = pTotalCalibData->GetItemPtr(pTotalCalibData->GetCurCalibIndex());
	if (pCalibData == NULL)
	{
		return false;
	}

	// 获取当前位置对应的标定结果
	CCalibratedInfo* pCalibratedInfo;
	pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nPosIndex);
	if (pCalibratedInfo == NULL)
	{
		return false;
	}

	CString strTemp;
	CString strOut;

	// 2*2矩阵(目前已经不用2*2矩阵,)
	//sc2Matrix mat = pCalibratedInfo->GetPlatformTransferMatrix();
	//strTemp.Format(_T("PlatTM: "));strOut += strTemp;
	//strTemp.Format(_T("A00:%.6f "), mat.GetElement(0, 0));strOut += strTemp;
	//strTemp.Format(_T("A01:%.6f "), mat.GetElement(0, 1));strOut += strTemp;
	//strTemp.Format(_T("A10:%.6f "), mat.GetElement(1, 0));strOut += strTemp;
	//strTemp.Format(_T("A11:%.6f\n"), mat.GetElement(1, 1));strOut += strTemp;

	// 3*3矩阵
	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	h[0] = 1.0;
	h[4] = 1.0;
	h[8] = 1.0;

	// 3*3矩阵
	double h1[9];
	memset(h1, 0.0, sizeof(double)*9);
	h1[0] = 1.0;
	h1[4] = 1.0;
	h1[8] = 1.0;

	// 3*3矩阵
	double h2[9];
	memset(h2, 0.0, sizeof(double)*9);
	h2[0] = 1.0;
	h2[4] = 1.0;
	h2[8] = 1.0;

	CPlatformOptionInfo optionInfo;
	pVisionASM->GetPlatformOptionInfo(optionInfo);
	if (!(optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib))
	{
		pCalibratedInfo->GetPlatformTransferH(h);
		strTemp.Format(_T("转换矩阵: \n"));strOut += strTemp;
		strTemp.Format(_T("\th0:%.6f "), h[0]);strOut += strTemp;
		strTemp.Format(_T("h1:%.6f "), h[1]);strOut += strTemp;
		strTemp.Format(_T("h2:%.6f \n"), h[2]);strOut += strTemp;
		strTemp.Format(_T("\th3:%.6f "), h[3]);strOut += strTemp;
		strTemp.Format(_T("h4:%.6f "), h[4]);strOut += strTemp;
		strTemp.Format(_T("h5:%.6f \n"), h[5]);strOut += strTemp;
		strTemp.Format(_T("\th6:%.6f "), h[6]);strOut += strTemp;
		strTemp.Format(_T("h7:%.6f "), h[7]);strOut += strTemp;
		strTemp.Format(_T("h8:%.6f\n"), h[8]);strOut += strTemp;
	}
	else
	{
		pCalibratedInfo->GetPlatformTransferH1(h1);
		strTemp.Format(_T("图像-靶标转换矩阵H1:  \n"));strOut += strTemp;
		strTemp.Format(_T("\th1[0]:%.6f "), h1[0]);strOut += strTemp;
		strTemp.Format(_T("h1[1]:%.6f "), h1[1]);strOut += strTemp;
		strTemp.Format(_T("h1[2]:%.6f "), h1[2]);strOut += strTemp;
		strTemp.Format(_T("\th1[3]:%.6f "), h1[3]);strOut += strTemp;
		strTemp.Format(_T("h1[4]:%.6f "), h1[4]);strOut += strTemp;
		strTemp.Format(_T("h1[5]:%.6f "), h1[5]);strOut += strTemp;
		strTemp.Format(_T("\th1[6]:%.6f "), h1[6]);strOut += strTemp;
		strTemp.Format(_T("h1[7]:%.6f "), h1[7]);strOut += strTemp;
		strTemp.Format(_T("h1[8]:%.6f\n"), h1[8]);strOut += strTemp;

		pCalibratedInfo->GetPlatformTransferH2(h2);
		strTemp.Format(_T("靶标-平台转换矩阵H2:  \n"));strOut += strTemp;
		strTemp.Format(_T("\th2[0]:%.6f "), h2[0]);strOut += strTemp;
		strTemp.Format(_T("h2[1]:%.6f "), h2[1]);strOut += strTemp;
		strTemp.Format(_T("h2[2]:%.6f "), h2[2]);strOut += strTemp;
		strTemp.Format(_T("\th2[3]:%.6f "), h2[3]);strOut += strTemp;
		strTemp.Format(_T("h2[4]:%.6f "), h2[4]);strOut += strTemp;
		strTemp.Format(_T("h2[5]:%.6f "), h2[5]);strOut += strTemp;
		strTemp.Format(_T("\th2[6]:%.6f "), h2[6]);strOut += strTemp;
		strTemp.Format(_T("h2[7]:%.6f "), h2[7]);strOut += strTemp;
		strTemp.Format(_T("h2[8]:%.6f\n"), h2[8]);strOut += strTemp;

		pCalibratedInfo->GetPlatformTransferH(h);
		strTemp.Format(_T("图像-平台转换矩阵H: \n"));strOut += strTemp;
		strTemp.Format(_T("\th0:%.6f "), h[0]);strOut += strTemp;
		strTemp.Format(_T("h1:%.6f "), h[1]);strOut += strTemp;
		strTemp.Format(_T("h2:%.6f \n"), h[2]);strOut += strTemp;
		strTemp.Format(_T("\th3:%.6f "), h[3]);strOut += strTemp;
		strTemp.Format(_T("h4:%.6f "), h[4]);strOut += strTemp;
		strTemp.Format(_T("h5:%.6f \n"), h[5]);strOut += strTemp;
		strTemp.Format(_T("\th6:%.6f "), h[6]);strOut += strTemp;
		strTemp.Format(_T("h7:%.6f "), h[7]);strOut += strTemp;
		strTemp.Format(_T("h8:%.6f\n"), h[8]);strOut += strTemp;
	}


	// 畸变校正结果DistortCorrectResult

	if(nPosIndex<pCalibData->m_vpCalibrateParam.size() && pCalibData->m_vpCalibrateParam.at(nPosIndex)->m_nEnableDisCor)
	{
		double p[15];
		memset(p, 0.0, sizeof(double)*15);
		pCalibratedInfo->m_result.GetNonlinearProjectResult(p);
		double dRms = pCalibratedInfo->m_result.GetRMS();
		strTemp.Format(_T("畸变系数: "));strOut += strTemp;
		strTemp.Format(_T("p0:%.6f "), p[0]);strOut += strTemp;
		strTemp.Format(_T("p1:%.6f "), p[1]);strOut += strTemp;
		strTemp.Format(_T("p2:%.6f "), p[2]);strOut += strTemp;
		strTemp.Format(_T("p3:%.6f "), p[3]);strOut += strTemp;
		strTemp.Format(_T("p4:%.6f "), p[4]);strOut += strTemp;
		strTemp.Format(_T("p5:%.6f "), p[5]);strOut += strTemp;
		strTemp.Format(_T("p6:%.6f "), p[6]);strOut += strTemp;
		strTemp.Format(_T("p7:%.6f "), p[7]);strOut += strTemp;
		strTemp.Format(_T("p8:%.6f "), p[8]);strOut += strTemp;
		strTemp.Format(_T("p9:%.6f "), p[9]);strOut += strTemp;
		strTemp.Format(_T("p10:%.6f "), p[10]);strOut += strTemp;
		strTemp.Format(_T("p11:%.6f "), p[11]);strOut += strTemp;
		strTemp.Format(_T("p12:%.6f "), p[12]);strOut += strTemp;
		strTemp.Format(_T("p13:%.6f "), p[13]);strOut += strTemp;
		strTemp.Format(_T("p14:%.6f "), p[14]);strOut += strTemp;
		strTemp.Format(_T("p15:%.6f\n"), dRms);strOut += strTemp;
	}



	// 平台坐标	
	CCoordPos pos = pCalibratedInfo->GetMarkPlatformCoordPos();
	strTemp.Format(_T("基准平台坐标: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
	strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

	// 图像坐标
	pos = pCalibratedInfo->GetMarkImgCoordPos();
	strTemp.Format(_T("基准图像坐标: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
	strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

	// 轴位置
	CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
	strTemp.Format(_T("平台轴位置: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pCalibPlatformAxisPos->m_dPosX);strOut += strTemp;
	strTemp.Format(_T("Y:%.6f "), pCalibPlatformAxisPos->m_dPosY);strOut += strTemp;
	strTemp.Format(_T("D:%.6f\n"), pCalibPlatformAxisPos->m_dAngle);strOut += strTemp;

	vcBaseAlignTool* tmpAlignTool = pVisionASM->GetAlignTool();
	// 配置相机轴非固定模式时，显示相机轴信息
	if (eCamPlatformSepXY == platformInfo.m_eCamPlatformType 
		|| eCamPlatformSepX == platformInfo.m_eCamPlatformType
		|| eCamPlatformShareX == platformInfo.m_eCamPlatformType)
	{
		// 相机小轴基准轴位置
		CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)pCalibratedInfo->GetCamPlatformAxisPos();
		strTemp.Format(_T("相机小轴基准轴位置: "));strOut += strTemp;
		strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
		strTemp.Format(_T("Y:%.6f \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;

		// 相机大轴基准轴位置
		CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)pCalibratedInfo->GetCamAxisPlatformAxisPos();
		strTemp.Format(_T("相机大轴基准轴位置: "));strOut += strTemp;
		strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
		strTemp.Format(_T("Y:%.6f \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;

		
		// 相机实时轴
		if(tmpAlignTool)
		{
			// 
			if(nPosIndex >= platformInfo.m_nPositionNum/2 && nPosIndex < platformInfo.m_nPositionNum
				&& platformInfo.m_bTargetObjectCamSeparate)
			{
				std::vector<CPlatformAxisPos*> TmpvPlatformAxisPos  = tmpAlignTool->GetTargetCameraPos();
				if(nPosIndex<TmpvPlatformAxisPos.size() && TmpvPlatformAxisPos.at(nPosIndex))
				{
					// 相机小轴实时轴位置
					CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)TmpvPlatformAxisPos.at(nPosIndex);
					strTemp.Format(_T("相机小轴实时轴位置: "));strOut += strTemp;
					strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
					strTemp.Format(_T("Y:%.6f \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;
				}

				std::vector<CPlatformAxisPos*> TmpvCamAxisPlatformAxisPos  = tmpAlignTool->GetTargetCamAxisPlatformAxisPos();
				if(nPosIndex<TmpvCamAxisPlatformAxisPos.size() && TmpvCamAxisPlatformAxisPos.at(nPosIndex))
				{
					// 相机小轴实时轴位置
					CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)TmpvCamAxisPlatformAxisPos.at(nPosIndex);
					strTemp.Format(_T("相机大轴实时轴位置: "));strOut += strTemp;
					strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
					strTemp.Format(_T("Y:%.6f \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;
				}
			}
			else if ((nPosIndex >= 0 && nPosIndex < platformInfo.m_nPositionNum /2 && 
				platformInfo.m_bTargetObjectCamSeparate) || (nPosIndex >= 0 && nPosIndex < platformInfo.m_nPositionNum && 
				!platformInfo.m_bTargetObjectCamSeparate))
			{
				std::vector<CPlatformAxisPos*> TmpvPlatformAxisPos  = tmpAlignTool->GetObjectCameraPos();
				if(nPosIndex<TmpvPlatformAxisPos.size() && TmpvPlatformAxisPos.at(nPosIndex))
				{
					// 相机小轴实时轴位置
					CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)TmpvPlatformAxisPos.at(nPosIndex);
					strTemp.Format(_T("相机小轴实时轴位置: "));strOut += strTemp;
					strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
					strTemp.Format(_T("Y:%.6f \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;
				}

				std::vector<CPlatformAxisPos*> TmpvCamAxisPlatformAxisPos  = tmpAlignTool->GetObjectCamAxisPlatformAxisPos();
				if(nPosIndex<TmpvCamAxisPlatformAxisPos.size() && TmpvCamAxisPlatformAxisPos.at(nPosIndex))
				{
					// 相机小轴实时轴位置
					CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)TmpvCamAxisPlatformAxisPos.at(nPosIndex);
					strTemp.Format(_T("相机大轴实时轴位置: "));strOut += strTemp;
					strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
					strTemp.Format(_T("Y:%.6f \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;
				}
			}
		}
	}



	strTemp.Format(_T("--------------------------------------------------\n"));
	strOut += strTemp;
	if (platformInfo.m_bTargetObjectCamSeparate)
	{
		if (nPosIndex >= platformInfo.m_nPositionNum/2 && (FALSE == platformInfo.m_bTarObjCamSepAutoCalibTar))
		{
			if (pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point
				|| pVisionASM->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
			{
				double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
				double ddx[4];
				double ddy[4];
				for (int i=0;i<4;i++)
				{
					ddx[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosX(i);
					ddy[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosY(i);
				}
				strTemp.Format(_T("差值(像素)(超过2,建议重新关联）MaxError:%.3f [X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f] "),dMaxError,ddx[0],ddy[0],ddx[1],ddy[1],ddx[2],ddy[2],ddx[3],ddy[3]);
				strOut += strTemp;
			}
			else
			{
				strTemp.Format(_T("比值(与1比较越接近越好,两数值相除结果在[0.95~1.05]即可,否则建议重新标定）X/Y:%.3f,X:%.3f,Y:%.3f "),h[0]/h[4],h[0],h[4]);
				strOut += strTemp;
			}

		}
		else
		{
			if (optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
			{
				//求精前后平台坐标差值
				strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
				strOut += strTemp;
				strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
				strOut += strTemp;
			}
			else
			{
				//求精前后平台坐标差值
				strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
				strOut += strTemp;
			}

		}
	}
	else
	{
		if (optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
		{
			//求精前后平台坐标差值
			strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
			strOut += strTemp;
			strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
			strOut += strTemp;
		}
		else
		{
			//求精前后平台坐标差值
			strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
			strOut += strTemp;
		}

	}



	if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
	{
		for(int nExIndex = 0; nExIndex < platformInfo.m_nMultiCalibExtensionMaxNum; nExIndex++)
		{
			strTemp.Format(_T("\n\n扩展%d:\n"), nExIndex);strOut += strTemp;

			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nPosIndex+nPosNum*(nExIndex+1));
			if (pCalibratedInfo == NULL)
			{
				continue;
			}

			// 2*2矩阵
			//sc2Matrix mat = pCalibratedInfo->GetPlatformTransferMatrix();
			//strTemp.Format(_T("PlatTM: "));strOut += strTemp;
			//strTemp.Format(_T("A00:%.6f "), mat.GetElement(0, 0));strOut += strTemp;
			//strTemp.Format(_T("A01:%.6f "), mat.GetElement(0, 1));strOut += strTemp;
			//strTemp.Format(_T("A10:%.6f "), mat.GetElement(1, 0));strOut += strTemp;
			//strTemp.Format(_T("A11:%.6f\n"), mat.GetElement(1, 1));strOut += strTemp;

			// 3*3矩阵
			double h[9];
			memset(h, 0.0, sizeof(double)*9);
			h[0] = 1.0;
			h[4] = 1.0;
			h[8] = 1.0;

			// 3*3矩阵
			double h1[9];
			memset(h1, 0.0, sizeof(double)*9);
			h1[0] = 1.0;
			h1[4] = 1.0;
			h1[8] = 1.0;

			// 3*3矩阵
			double h2[9];
			memset(h2, 0.0, sizeof(double)*9);
			h2[0] = 1.0;
			h2[4] = 1.0;
			h2[8] = 1.0;

			CPlatformOptionInfo optionInfo;
			pVisionASM->GetPlatformOptionInfo(optionInfo);
			if (!(optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib))
			{
				pCalibratedInfo->GetPlatformTransferH(h);
				strTemp.Format(_T("转换矩阵: \n"));strOut += strTemp;
				strTemp.Format(_T("\th0:%.6f "), h[0]);strOut += strTemp;
				strTemp.Format(_T("h1:%.6f "), h[1]);strOut += strTemp;
				strTemp.Format(_T("h2:%.6f \n"), h[2]);strOut += strTemp;
				strTemp.Format(_T("\th3:%.6f "), h[3]);strOut += strTemp;
				strTemp.Format(_T("h4:%.6f "), h[4]);strOut += strTemp;
				strTemp.Format(_T("h5:%.6f \n"), h[5]);strOut += strTemp;
				strTemp.Format(_T("\th6:%.6f "), h[6]);strOut += strTemp;
				strTemp.Format(_T("h7:%.6f "), h[7]);strOut += strTemp;
				strTemp.Format(_T("h8:%.6f\n"), h[8]);strOut += strTemp;

			}
			else
			{
				pCalibratedInfo->GetPlatformTransferH1(h1);
				strTemp.Format(_T("图像-靶标转换矩阵H1: \n"));strOut += strTemp;
				strTemp.Format(_T("\th1[0]:%.6f "), h1[0]);strOut += strTemp;
				strTemp.Format(_T("h1[1]:%.6f "), h1[1]);strOut += strTemp;
				strTemp.Format(_T("h1[2]:%.6f "), h1[2]);strOut += strTemp;
				strTemp.Format(_T("\th1[3]:%.6f "), h1[3]);strOut += strTemp;
				strTemp.Format(_T("h1[4]:%.6f "), h1[4]);strOut += strTemp;
				strTemp.Format(_T("h1[5]:%.6f "), h1[5]);strOut += strTemp;
				strTemp.Format(_T("\th1[6]:%.6f "), h1[6]);strOut += strTemp;
				strTemp.Format(_T("h1[7]:%.6f "), h1[7]);strOut += strTemp;
				strTemp.Format(_T("h1[8]:%.6f\n"), h1[8]);strOut += strTemp;

				pCalibratedInfo->GetPlatformTransferH2(h2);
				strTemp.Format(_T("靶标-平台转换矩阵H2: \n"));strOut += strTemp;
				strTemp.Format(_T("\th2[0]:%.6f "), h2[0]);strOut += strTemp;
				strTemp.Format(_T("h2[1]:%.6f "), h2[1]);strOut += strTemp;
				strTemp.Format(_T("h2[2]:%.6f "), h2[2]);strOut += strTemp;
				strTemp.Format(_T("\th2[3]:%.6f "), h2[3]);strOut += strTemp;
				strTemp.Format(_T("h2[4]:%.6f "), h2[4]);strOut += strTemp;
				strTemp.Format(_T("h2[5]:%.6f "), h2[5]);strOut += strTemp;
				strTemp.Format(_T("\th2[6]:%.6f "), h2[6]);strOut += strTemp;
				strTemp.Format(_T("h2[7]:%.6f "), h2[7]);strOut += strTemp;
				strTemp.Format(_T("h2[8]:%.6f\n"), h2[8]);strOut += strTemp;
			}


			// 畸变校正结果DistortCorrectResult
			if(nPosIndex<pCalibData->m_vpCalibrateParam.size() && pCalibData->m_vpCalibrateParam.at(nPosIndex)->m_nEnableDisCor)
			{
				double p[15];
				memset(p, 0.0, sizeof(double)*15);
				pCalibratedInfo->m_result.GetNonlinearProjectResult(p);
				double dRms = pCalibratedInfo->m_result.GetRMS();
				strTemp.Format(_T("畸变系数: "));strOut += strTemp;
				strTemp.Format(_T("p0:%.6f "), p[0]);strOut += strTemp;
				strTemp.Format(_T("p1:%.6f "), p[1]);strOut += strTemp;
				strTemp.Format(_T("p2:%.6f "), p[2]);strOut += strTemp;
				strTemp.Format(_T("p3:%.6f "), p[3]);strOut += strTemp;
				strTemp.Format(_T("p4:%.6f "), p[4]);strOut += strTemp;
				strTemp.Format(_T("p5:%.6f "), p[5]);strOut += strTemp;
				strTemp.Format(_T("p6:%.6f "), p[6]);strOut += strTemp;
				strTemp.Format(_T("p7:%.6f "), p[7]);strOut += strTemp;
				strTemp.Format(_T("p8:%.6f "), p[8]);strOut += strTemp;
				strTemp.Format(_T("p9:%.6f "), p[9]);strOut += strTemp;
				strTemp.Format(_T("p10:%.6f "), p[10]);strOut += strTemp;
				strTemp.Format(_T("p11:%.6f "), p[11]);strOut += strTemp;
				strTemp.Format(_T("p12:%.6f "), p[12]);strOut += strTemp;
				strTemp.Format(_T("p13:%.6f "), p[13]);strOut += strTemp;
				strTemp.Format(_T("p14:%.6f "), p[14]);strOut += strTemp;
				strTemp.Format(_T("p15:%.6f\n"), dRms);strOut += strTemp;

			}
			
			// 平台坐标	
			CCoordPos pos = pCalibratedInfo->GetMarkPlatformCoordPos();
			strTemp.Format(_T("基准平台坐标: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
			strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

			// 图像坐标
			pos = pCalibratedInfo->GetMarkImgCoordPos();
			strTemp.Format(_T("基准图像坐标: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
			strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

			// 轴位置
			CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
			strTemp.Format(_T("平台轴位置: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pCalibPlatformAxisPos->m_dPosX);strOut += strTemp;
			strTemp.Format(_T("Y:%.6f "), pCalibPlatformAxisPos->m_dPosY);strOut += strTemp;
			strTemp.Format(_T("D:%.6f\n"), pCalibPlatformAxisPos->m_dAngle);strOut += strTemp;


			if (eCamPlatformSepXY == platformInfo.m_eCamPlatformType 
				|| eCamPlatformSepX == platformInfo.m_eCamPlatformType
				|| eCamPlatformShareX == platformInfo.m_eCamPlatformType)
			{
				// 相机小轴基准轴位置
				CPlatformXYAxisPos* pCamPlatformAxisPos = (CPlatformXYAxisPos*)pCalibratedInfo->GetCamPlatformAxisPos();
				strTemp.Format(_T("相机小轴基准轴位置: "));strOut += strTemp;
				strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
				strTemp.Format(_T("Y:%.6f \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;

				// 相机大轴基准轴位置
				CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)pCalibratedInfo->GetCamAxisPlatformAxisPos();
				strTemp.Format(_T("相机大轴基准轴位置: "));strOut += strTemp;
				strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
				strTemp.Format(_T("Y:%.6f \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;
				// 相机实时轴
				if(tmpAlignTool)
				{
					// 
					if(nPosIndex >= platformInfo.m_nPositionNum/2 && nPosIndex < platformInfo.m_nPositionNum
						&& platformInfo.m_bTargetObjectCamSeparate)
					{
						if((nExIndex+1<tmpAlignTool->m_vvpTargetCamPlatformAxisPosMultiEx.size()) &&
							(nPosIndex<tmpAlignTool->m_vvpTargetCamPlatformAxisPosMultiEx.at(nExIndex+1).size()) && 
							(tmpAlignTool->m_vvpTargetCamPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex)))
						{
							// 相机小轴实时轴位置
							CPlatformXYAxisPos* pCamPlatformAxisPos =(CPlatformXYAxisPos*)(tmpAlignTool->m_vvpTargetCamPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex));
							strTemp.Format(_T("相机小轴实时轴位置: "));strOut += strTemp;
							strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
							strTemp.Format(_T("Y:%.6f  \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;
						}

						if((nExIndex+1<tmpAlignTool->m_vvpTargetCamAxisPlatformAxisPosMultiEx.size()) &&
							(nPosIndex<tmpAlignTool->m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).size()) && 
							(tmpAlignTool->m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex)))
						{
							// 相机大轴实时轴位置
							CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)(tmpAlignTool->m_vvpTargetCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex));
							strTemp.Format(_T("相机大轴实时轴位置: "));strOut += strTemp;
							strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
							strTemp.Format(_T("Y:%.6f  \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;
						}

					}
					else if ((nPosIndex >= 0 && nPosIndex < platformInfo.m_nPositionNum /2 && 
						platformInfo.m_bTargetObjectCamSeparate) || (nPosIndex >= 0 && nPosIndex < platformInfo.m_nPositionNum && 
						!platformInfo.m_bTargetObjectCamSeparate))
					{
						if((nExIndex+1<tmpAlignTool->m_vvpObjectCamPlatformAxisPosMultiEx.size()) && 
							(nPosIndex<tmpAlignTool->m_vvpObjectCamPlatformAxisPosMultiEx.at(nExIndex+1).size()) && 
							(tmpAlignTool->m_vvpObjectCamPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex)))
						{
							// 相机小轴实时轴位置
							CPlatformXYAxisPos* pCamPlatformAxisPos =(CPlatformXYAxisPos*)(tmpAlignTool->m_vvpObjectCamPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex));
							strTemp.Format(_T("相机小轴实时轴位置: "));strOut += strTemp;
							strTemp.Format(_T("X:%.6f "), pCamPlatformAxisPos->m_dPosX);strOut += strTemp;
							strTemp.Format(_T("Y:%.6f  \n"), pCamPlatformAxisPos->m_dPosY);strOut += strTemp;
						}

						if((nExIndex+1<tmpAlignTool->m_vvpObjectCamAxisPlatformAxisPosMultiEx.size()) && 
							(nPosIndex<tmpAlignTool->m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).size()) && 
							(tmpAlignTool->m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex)))
						{
							// 相机大轴实时轴位置
							CPlatformXYAxisPos* pCamAxisPlatformAxisPos = (CPlatformXYAxisPos*)(tmpAlignTool->m_vvpObjectCamAxisPlatformAxisPosMultiEx.at(nExIndex+1).at(nPosIndex));
							strTemp.Format(_T("相机大轴实时轴位置: "));strOut += strTemp;
							strTemp.Format(_T("X:%.6f "), pCamAxisPlatformAxisPos->m_dPosX);strOut += strTemp;
							strTemp.Format(_T("Y:%.6f  \n"), pCamAxisPlatformAxisPos->m_dPosY);strOut += strTemp;
						}
					}
				}
			}
			

			strTemp.Format(_T("--------------------------------------------------\n"));
			strOut += strTemp;
			if (platformInfo.m_bTargetObjectCamSeparate)
			{
				if (nPosIndex >= platformInfo.m_nPositionNum/2 && (FALSE == platformInfo.m_bTarObjCamSepAutoCalibTar))
				{
					if (pVisionASM->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point
						|| pVisionASM->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
					{
						double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
						double ddx[4];
						double ddy[4];
						for (int i=0;i<4;i++)
						{
							ddx[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosX(i);
							ddy[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosY(i);
						}
						strTemp.Format(_T("差值(像素)(超过2,建议重新关联）MaxError:%.3f [X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f] "),dMaxError,ddx[0],ddy[0],ddx[1],ddy[1],ddx[2],ddy[2],ddx[3],ddy[3]);
						strOut += strTemp;
					}
					else
					{
						strTemp.Format(_T("比值(与1比较越接近越好,两数值相除结果在[0.95~1.05]即可,否则建议重新标定）X/Y:%.3f,X:%.3f,Y:%.3f "),h[0]/h[4],h[0],h[4]);
						strOut += strTemp;
					}

				}
				else
				{
					if (optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
					{
						//求精前后平台坐标差值
						strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
						strOut += strTemp;
						strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
						strOut += strTemp;
					}
					else
					{
						//求精前后平台坐标差值
						strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
						strOut += strTemp;
					}

				}
			}
			else
			{
				if (optionInfo.m_bEnableDmCodeCalib && pVisionASM->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
				{
					//求精前后平台坐标差值
					strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
					strOut += strTemp;
					strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
					strOut += strTemp;
				}
				else
				{
					//求精前后平台坐标差值
					strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
					strOut += strTemp;
				}

			}


		}
	}
	strDescription = strOut;
	return true;
}

void CDlgSysKindCalibSetting::ShowCalibratedResult_Original(int nPosIndex)
{
	// 如果当前位置超限，则直接返回不做处理
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPosNum = platformInfo.m_nPositionNum;
	if (nPosIndex >= nPosNum)
	{
		return;
	}

	// 获取当前平台对应的全部标定数据
	CTotalCalibData *pTotalCalibData;
	pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
	if (pTotalCalibData == NULL)
	{
		return;
	}

	// 获取当前产品对应的标定数据
	CCalibData* pCalibData;
	pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
	if (pCalibData == NULL)
	{
		return;
	}

	// 获取当前位置对应的标定结果
	CCalibratedInfo* pCalibratedInfo;
	pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nPosIndex);
	if (pCalibratedInfo == NULL)
	{
		return;
	}

	CString strTemp;
	CString strOut;

	// 2*2矩阵
	sc2Matrix mat = pCalibratedInfo->GetPlatformTransferMatrix();
	strTemp.Format(_T("PlatTM: "));strOut += strTemp;
	strTemp.Format(_T("A00:%.6f "), mat.GetElement(0, 0));strOut += strTemp;
	strTemp.Format(_T("A01:%.6f "), mat.GetElement(0, 1));strOut += strTemp;
	strTemp.Format(_T("A10:%.6f "), mat.GetElement(1, 0));strOut += strTemp;
	strTemp.Format(_T("A11:%.6f\n"), mat.GetElement(1, 1));strOut += strTemp;

	// 3*3矩阵
	double h[9];
	memset(h, 0.0, sizeof(double)*9);
	h[0] = 1.0;
	h[4] = 1.0;
	h[8] = 1.0;

	// 3*3矩阵
	double h1[9];
	memset(h1, 0.0, sizeof(double)*9);
	h1[0] = 1.0;
	h1[4] = 1.0;
	h1[8] = 1.0;

	// 3*3矩阵
	double h2[9];
	memset(h2, 0.0, sizeof(double)*9);
	h2[0] = 1.0;
	h2[4] = 1.0;
	h2[8] = 1.0;

	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	if (!(optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib))
	{
		pCalibratedInfo->GetPlatformTransferH(h);
		strTemp.Format(_T("PlatTMH: "));strOut += strTemp;
		strTemp.Format(_T("h0:%.6f "), h[0]);strOut += strTemp;
		strTemp.Format(_T("h1:%.6f "), h[1]);strOut += strTemp;
		strTemp.Format(_T("h2:%.6f "), h[2]);strOut += strTemp;
		strTemp.Format(_T("h3:%.6f "), h[3]);strOut += strTemp;
		strTemp.Format(_T("h4:%.6f "), h[4]);strOut += strTemp;
		strTemp.Format(_T("h5:%.6f "), h[5]);strOut += strTemp;
		strTemp.Format(_T("h6:%.6f "), h[6]);strOut += strTemp;
		strTemp.Format(_T("h7:%.6f "), h[7]);strOut += strTemp;
		strTemp.Format(_T("h8:%.6f\n"), h[8]);strOut += strTemp;
	}
	else
	{
		pCalibratedInfo->GetPlatformTransferH1(h1);
		strTemp.Format(_T("PlatTMH1: "));strOut += strTemp;
		strTemp.Format(_T("h1[0]:%.6f "), h1[0]);strOut += strTemp;
		strTemp.Format(_T("h1[1]:%.6f "), h1[1]);strOut += strTemp;
		strTemp.Format(_T("h1[2]:%.6f "), h1[2]);strOut += strTemp;
		strTemp.Format(_T("h1[3]:%.6f "), h1[3]);strOut += strTemp;
		strTemp.Format(_T("h1[4]:%.6f "), h1[4]);strOut += strTemp;
		strTemp.Format(_T("h1[5]:%.6f "), h1[5]);strOut += strTemp;
		strTemp.Format(_T("h1[6]:%.6f "), h1[6]);strOut += strTemp;
		strTemp.Format(_T("h1[7]:%.6f "), h1[7]);strOut += strTemp;
		strTemp.Format(_T("h1[8]:%.6f\n"), h1[8]);strOut += strTemp;

		pCalibratedInfo->GetPlatformTransferH2(h2);
		strTemp.Format(_T("PlatTMH2: "));strOut += strTemp;
		strTemp.Format(_T("h2[0]:%.6f "), h2[0]);strOut += strTemp;
		strTemp.Format(_T("h2[1]:%.6f "), h2[1]);strOut += strTemp;
		strTemp.Format(_T("h2[2]:%.6f "), h2[2]);strOut += strTemp;
		strTemp.Format(_T("h2[3]:%.6f "), h2[3]);strOut += strTemp;
		strTemp.Format(_T("h2[4]:%.6f "), h2[4]);strOut += strTemp;
		strTemp.Format(_T("h2[5]:%.6f "), h2[5]);strOut += strTemp;
		strTemp.Format(_T("h2[6]:%.6f "), h2[6]);strOut += strTemp;
		strTemp.Format(_T("h2[7]:%.6f "), h2[7]);strOut += strTemp;
		strTemp.Format(_T("h2[8]:%.6f\n"), h2[8]);strOut += strTemp;
	}
	

	// 畸变校正结果DistortCorrectResult
	double p[15];
	memset(p, 0.0, sizeof(double)*15);
	pCalibratedInfo->m_result.GetNonlinearProjectResult(p);
	double dRms = pCalibratedInfo->m_result.GetRMS();
	strTemp.Format(_T("DCR: "));strOut += strTemp;
	strTemp.Format(_T("p0:%.6f "), p[0]);strOut += strTemp;
	strTemp.Format(_T("p1:%.6f "), p[1]);strOut += strTemp;
	strTemp.Format(_T("p2:%.6f "), p[2]);strOut += strTemp;
	strTemp.Format(_T("p3:%.6f "), p[3]);strOut += strTemp;
	strTemp.Format(_T("p4:%.6f "), p[4]);strOut += strTemp;
	strTemp.Format(_T("p5:%.6f "), p[5]);strOut += strTemp;
	strTemp.Format(_T("p6:%.6f "), p[6]);strOut += strTemp;
	strTemp.Format(_T("p7:%.6f "), p[7]);strOut += strTemp;
	strTemp.Format(_T("p8:%.6f "), p[8]);strOut += strTemp;
	strTemp.Format(_T("p9:%.6f "), p[9]);strOut += strTemp;
	strTemp.Format(_T("p10:%.6f "), p[10]);strOut += strTemp;
	strTemp.Format(_T("p11:%.6f "), p[11]);strOut += strTemp;
	strTemp.Format(_T("p12:%.6f "), p[12]);strOut += strTemp;
	strTemp.Format(_T("p13:%.6f "), p[13]);strOut += strTemp;
	strTemp.Format(_T("p14:%.6f "), p[14]);strOut += strTemp;
	strTemp.Format(_T("p15:%.6f\n"), dRms);strOut += strTemp;

	// 平台坐标	
	CCoordPos pos = pCalibratedInfo->GetMarkPlatformCoordPos();
	strTemp.Format(_T("MarkPlatCoord: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
	strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

	// 图像坐标
	pos = pCalibratedInfo->GetMarkImgCoordPos();
	strTemp.Format(_T("MarkImgCoord: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
	strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

	// 轴位置
	CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
	strTemp.Format(_T("PlatAxisPos: "));strOut += strTemp;
	strTemp.Format(_T("X:%.6f "), pCalibPlatformAxisPos->m_dPosX);strOut += strTemp;
	strTemp.Format(_T("Y:%.6f "), pCalibPlatformAxisPos->m_dPosY);strOut += strTemp;
	strTemp.Format(_T("D:%.6f\n"), pCalibPlatformAxisPos->m_dAngle);strOut += strTemp;

	strTemp.Format(_T("--------------------------------------------------\n"));
	strOut += strTemp;
	if (platformInfo.m_bTargetObjectCamSeparate)
	{
		if (nPosIndex >= platformInfo.m_nPositionNum/2 && (FALSE == platformInfo.m_bTarObjCamSepAutoCalibTar))
		{
			if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point
				|| m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
			{
				double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
				double ddx[4];
				double ddy[4];
				for (int i=0;i<4;i++)
				{
					ddx[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosX(i);
					ddy[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosY(i);
				}
				strTemp.Format(_T("差值(像素)(超过2,建议重新关联）MaxError:%.3f [X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f] "),dMaxError,ddx[0],ddy[0],ddx[1],ddy[1],ddx[2],ddy[2],ddx[3],ddy[3]);
				strOut += strTemp;
			}
			else
			{
				strTemp.Format(_T("比值(与1比较越接近越好,两数值相除结果在[0.95~1.05]即可,否则建议重新标定）X/Y:%.3f,X:%.3f,Y:%.3f "),h[0]/h[4],h[0],h[4]);
				strOut += strTemp;
			}

		}
		else
		{
			if (optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
			{
				//求精前后平台坐标差值
				strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
				strOut += strTemp;
				strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
				strOut += strTemp;
			}
			else
			{
				//求精前后平台坐标差值
				strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
				strOut += strTemp;
			}
			
		}
	}
	else
	{
		if (optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
		{
			//求精前后平台坐标差值
			strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
			strOut += strTemp;
			strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
			strOut += strTemp;
		}
		else
		{
			//求精前后平台坐标差值
			strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
			strOut += strTemp;
		}
		
	}



	if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
	{
		for(int nExIndex = 0; nExIndex < platformInfo.m_nMultiCalibExtensionMaxNum; nExIndex++)
		{
			strTemp.Format(_T("\n\nExtension%d:\n"), nExIndex);strOut += strTemp;

			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nPosIndex+nPosNum*(nExIndex+1));
			if (pCalibratedInfo == NULL)
			{
				continue;
			}

			// 2*2矩阵
			sc2Matrix mat = pCalibratedInfo->GetPlatformTransferMatrix();
			strTemp.Format(_T("PlatTM: "));strOut += strTemp;
			strTemp.Format(_T("A00:%.6f "), mat.GetElement(0, 0));strOut += strTemp;
			strTemp.Format(_T("A01:%.6f "), mat.GetElement(0, 1));strOut += strTemp;
			strTemp.Format(_T("A10:%.6f "), mat.GetElement(1, 0));strOut += strTemp;
			strTemp.Format(_T("A11:%.6f\n"), mat.GetElement(1, 1));strOut += strTemp;

			// 3*3矩阵
			double h[9];
			memset(h, 0.0, sizeof(double)*9);
			h[0] = 1.0;
			h[4] = 1.0;
			h[8] = 1.0;

			// 3*3矩阵
			double h1[9];
			memset(h1, 0.0, sizeof(double)*9);
			h1[0] = 1.0;
			h1[4] = 1.0;
			h1[8] = 1.0;

			// 3*3矩阵
			double h2[9];
			memset(h2, 0.0, sizeof(double)*9);
			h2[0] = 1.0;
			h2[4] = 1.0;
			h2[8] = 1.0;

			CPlatformOptionInfo optionInfo;
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
			if (!(optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib))
			{
				pCalibratedInfo->GetPlatformTransferH(h);
				strTemp.Format(_T("PlatTMH: "));strOut += strTemp;
				strTemp.Format(_T("h0:%.6f "), h[0]);strOut += strTemp;
				strTemp.Format(_T("h1:%.6f "), h[1]);strOut += strTemp;
				strTemp.Format(_T("h2:%.6f "), h[2]);strOut += strTemp;
				strTemp.Format(_T("h3:%.6f "), h[3]);strOut += strTemp;
				strTemp.Format(_T("h4:%.6f "), h[4]);strOut += strTemp;
				strTemp.Format(_T("h5:%.6f "), h[5]);strOut += strTemp;
				strTemp.Format(_T("h6:%.6f "), h[6]);strOut += strTemp;
				strTemp.Format(_T("h7:%.6f "), h[7]);strOut += strTemp;
				strTemp.Format(_T("h8:%.6f\n"), h[8]);strOut += strTemp;

			}
			else
			{
				pCalibratedInfo->GetPlatformTransferH1(h1);
				strTemp.Format(_T("PlatTMH1: "));strOut += strTemp;
				strTemp.Format(_T("h1[0]:%.6f "), h1[0]);strOut += strTemp;
				strTemp.Format(_T("h1[1]:%.6f "), h1[1]);strOut += strTemp;
				strTemp.Format(_T("h1[2]:%.6f "), h1[2]);strOut += strTemp;
				strTemp.Format(_T("h1[3]:%.6f "), h1[3]);strOut += strTemp;
				strTemp.Format(_T("h1[4]:%.6f "), h1[4]);strOut += strTemp;
				strTemp.Format(_T("h1[5]:%.6f "), h1[5]);strOut += strTemp;
				strTemp.Format(_T("h1[6]:%.6f "), h1[6]);strOut += strTemp;
				strTemp.Format(_T("h1[7]:%.6f "), h1[7]);strOut += strTemp;
				strTemp.Format(_T("h1[8]:%.6f\n"), h1[8]);strOut += strTemp;

				pCalibratedInfo->GetPlatformTransferH2(h2);
				strTemp.Format(_T("PlatTMH2: "));strOut += strTemp;
				strTemp.Format(_T("h2[0]:%.6f "), h2[0]);strOut += strTemp;
				strTemp.Format(_T("h2[1]:%.6f "), h2[1]);strOut += strTemp;
				strTemp.Format(_T("h2[2]:%.6f "), h2[2]);strOut += strTemp;
				strTemp.Format(_T("h2[3]:%.6f "), h2[3]);strOut += strTemp;
				strTemp.Format(_T("h2[4]:%.6f "), h2[4]);strOut += strTemp;
				strTemp.Format(_T("h2[5]:%.6f "), h2[5]);strOut += strTemp;
				strTemp.Format(_T("h2[6]:%.6f "), h2[6]);strOut += strTemp;
				strTemp.Format(_T("h2[7]:%.6f "), h2[7]);strOut += strTemp;
				strTemp.Format(_T("h2[8]:%.6f\n"), h2[8]);strOut += strTemp;
			}


			// 畸变校正结果DistortCorrectResult
			double p[15];
			memset(p, 0.0, sizeof(double)*15);
			pCalibratedInfo->m_result.GetNonlinearProjectResult(p);
			double dRms = pCalibratedInfo->m_result.GetRMS();
			strTemp.Format(_T("DCR: "));strOut += strTemp;
			strTemp.Format(_T("p0:%.6f "), p[0]);strOut += strTemp;
			strTemp.Format(_T("p1:%.6f "), p[1]);strOut += strTemp;
			strTemp.Format(_T("p2:%.6f "), p[2]);strOut += strTemp;
			strTemp.Format(_T("p3:%.6f "), p[3]);strOut += strTemp;
			strTemp.Format(_T("p4:%.6f "), p[4]);strOut += strTemp;
			strTemp.Format(_T("p5:%.6f "), p[5]);strOut += strTemp;
			strTemp.Format(_T("p6:%.6f "), p[6]);strOut += strTemp;
			strTemp.Format(_T("p7:%.6f "), p[7]);strOut += strTemp;
			strTemp.Format(_T("p8:%.6f "), p[8]);strOut += strTemp;
			strTemp.Format(_T("p9:%.6f "), p[9]);strOut += strTemp;
			strTemp.Format(_T("p10:%.6f "), p[10]);strOut += strTemp;
			strTemp.Format(_T("p11:%.6f "), p[11]);strOut += strTemp;
			strTemp.Format(_T("p12:%.6f "), p[12]);strOut += strTemp;
			strTemp.Format(_T("p13:%.6f "), p[13]);strOut += strTemp;
			strTemp.Format(_T("p14:%.6f "), p[14]);strOut += strTemp;
			strTemp.Format(_T("p15:%.6f\n"), dRms);strOut += strTemp;

			// 平台坐标	
			CCoordPos pos = pCalibratedInfo->GetMarkPlatformCoordPos();
			strTemp.Format(_T("MarkPlatCoord: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
			strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

			// 图像坐标
			pos = pCalibratedInfo->GetMarkImgCoordPos();
			strTemp.Format(_T("MarkImgCoord: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pos.GetPosX());strOut += strTemp;
			strTemp.Format(_T("Y:%.6f\n"), pos.GetPosY());strOut += strTemp;

			// 轴位置
			CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
			strTemp.Format(_T("PlatAxisPos: "));strOut += strTemp;
			strTemp.Format(_T("X:%.6f "), pCalibPlatformAxisPos->m_dPosX);strOut += strTemp;
			strTemp.Format(_T("Y:%.6f "), pCalibPlatformAxisPos->m_dPosY);strOut += strTemp;
			strTemp.Format(_T("D:%.6f\n"), pCalibPlatformAxisPos->m_dAngle);strOut += strTemp;


			strTemp.Format(_T("--------------------------------------------------\n"));
			strOut += strTemp;
			if (platformInfo.m_bTargetObjectCamSeparate)
			{
				if (nPosIndex >= platformInfo.m_nPositionNum/2 && (FALSE == platformInfo.m_bTarObjCamSepAutoCalibTar))
				{
					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibTargetMarkPatternMode() == eSinglePatternGuanlian4Point
								|| m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibTargetMarkPatternMode() == eDmCodeCalib)
					{
						double dMaxError = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX();
						double ddx[4];
						double ddy[4];
						for (int i=0;i<4;i++)
						{
							ddx[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosX(i);
							ddy[i] = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetAuxiliaryPosY(i);
						}
						strTemp.Format(_T("差值(像素)(超过2,建议重新关联）MaxError:%.3f [X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f][X:%.3f,Y:%.3f] "),dMaxError,ddx[0],ddy[0],ddx[1],ddy[1],ddx[2],ddy[2],ddx[3],ddy[3]);
						strOut += strTemp;
					}
					else
					{
						strTemp.Format(_T("比值(与1比较越接近越好,两数值相除结果在[0.95~1.05]即可,否则建议重新标定）X/Y:%.3f,X:%.3f,Y:%.3f "),h[0]/h[4],h[0],h[4]);
						strOut += strTemp;
					}

				}
				else
				{
					if (optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
					{
						//求精前后平台坐标差值
						strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
						strOut += strTemp;
						strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
						strOut += strTemp;
					}
					else
					{
						//求精前后平台坐标差值
						strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
						strOut += strTemp;
					}
					
				}
			}
			else
			{
				if (optionInfo.m_bEnableDmCodeCalib && m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibMarkPatternMode()==eWholeDmcodeBoardCalib)
				{
					//求精前后平台坐标差值
					strTemp.Format(_T("H1差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f\n "),h1[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH1().GetPosY());
					strOut += strTemp;
					strTemp.Format(_T("H2差值(mm)(越小越好,两数值超过%d,建议重新标定）X:%.3f,Y:%.3f "),2,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffsetH2().GetPosY());
					strOut += strTemp;
				}
				else
				{
					//求精前后平台坐标差值
					strTemp.Format(_T("差值(mm)(越小越好,两数值超过%.3f,建议重新标定）X:%.3f,Y:%.3f "),h[0]*5,pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosX(),pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset().GetPosY());
					strOut += strTemp;
				}
				
			}


		}
	}

	// 对话框标题
	CString strCaption;
	strCaption.Format(_T("CalibratedResult%d"), nPosIndex+1);

	// 弹出对话框
	MessageBox(strOut, strCaption);

	return;
}
void CDlgSysKindCalibSetting::ShowCalibratedResult(int nPosIndex)
{
	// 如果当前位置超限，则直接返回不做处理
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPosNum = platformInfo.m_nPositionNum;
	if (nPosIndex >= nPosNum)
	{
		return;
	}
	CString	strDescription;
	//获取对标定信息的文本描述
	if(!GetCalibratedResultDetailDescription(strDescription,m_pParent->m_vpVisionASM.at(m_nPlatformIndex),nPosIndex))
	{
		return;
	}
	// 对话框标题
	CString strCaption;
	strCaption.Format(_T("CalibratedResult%d"), nPosIndex+1);

	if (/*m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystempOptionConfig->m_bIsShowCalibrateEvaluate*/ /*&&*/ ((platformInfo.m_bTargetObjectCamSeparate==FALSE) ||
			(platformInfo.m_bTargetObjectCamSeparate==TRUE && 
			((nPosIndex<nPosNum/2 && 
			(platformInfo.m_eMidPlatformType==ePlatformXYD || 
			platformInfo.m_eMidPlatformType==ePlatformX1X2Y || 
			platformInfo.m_eMidPlatformType==ePlatformXY1Y2 || 
			platformInfo.m_eMidPlatformType==ePlatformXY|| 
			platformInfo.m_eMidPlatformType==ePlatformXD|| 
			platformInfo.m_eMidPlatformType==ePlatformYD)) || 
			(nPosIndex>=nPosNum/2 &&platformInfo.m_eMidPlatformType==ePlatformXYPD)))))
		{


			if(platformInfo.m_eMidPlatformType == ePlatformXYD|| 
				platformInfo.m_eMidPlatformType==ePlatformX1X2Y || 
				platformInfo.m_eMidPlatformType==ePlatformXY1Y2 )
			{
				if(IDOK == MessageBox(strDescription,strCaption,MB_OKCANCEL))
				{
					CDlgCalibResult dlgCalibResult(m_pParent->m_vpVisionASM.at(m_nPlatformIndex),nPosIndex,this);
					dlgCalibResult.DoModal();
				}

			}
			else if(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystempOptionConfig->m_bIsShowCalibrateEvaluate)
			{
				CDlgCalibrateEvaluate dlg;
				dlg.SetVisionAlignerRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
				dlg.m_nCurProductIdx = m_nCurProductIdx;
				dlg.DoModal();
			}
			else
			{
				MessageBox(strDescription,strCaption);
				/*ShowCalibratedResult_Original(nPosIndex);*/
			}
			return;
		}
		else
		{
			MessageBox(strDescription,strCaption);
			//ShowCalibratedResult_Original(nPosIndex);
		}
	


	

}
void CDlgSysKindCalibSetting::OnBnClickedRadioPosChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRadioPosIndexOld = m_nRadioPosIndex;
	UpdateData(TRUE);

	CString strVal[] = {_T("位置1"), _T("位置2"),_T("位置3"), _T("位置4"),_T("位置5"), _T("位置6"),_T("位置7"), _T("位置8")};
	if (nRadioPosIndexOld != m_nRadioPosIndex)
	{
		CString strLog;
		strLog.Format(_T("修改Radio被复制位置由%s到%s\n"),strVal[nRadioPosIndexOld],strVal[m_nRadioPosIndex]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}



void CDlgSysKindCalibSetting::OnBnClickedCheckPos1()
{
	// TODO: 在此添加控件通知处理程序代码m_checkPos1
	 BOOL bPos1Old = m_bPos1;

	UpdateData(TRUE);
	if (bPos1Old != m_bPos1)
	{
		CString strLog;
		CString strValue = m_bPos1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos2()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos2Old = m_bPos2;

	UpdateData(TRUE);
	if (bPos2Old != m_bPos2)
	{
		CString strLog;
		CString strValue = m_bPos2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos3()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos3Old = m_bPos3;

	UpdateData(TRUE);
	if (bPos3Old != m_bPos3)
	{
		CString strLog;
		CString strValue = m_bPos3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos4()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos4Old = m_bPos4;

	UpdateData(TRUE);
	if (bPos4Old != m_bPos4)
	{
		CString strLog;
		CString strValue = m_bPos4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos5()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos5Old = m_bPos5;

	UpdateData(TRUE);
	if (bPos5Old != m_bPos5)
	{
		CString strLog;
		CString strValue = m_bPos5 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置5]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos6()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos6Old = m_bPos6;

	UpdateData(TRUE);
	if (bPos6Old != m_bPos6)
	{
		CString strLog;
		CString strValue = m_bPos6 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置6]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos7()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos7Old = m_bPos7;

	UpdateData(TRUE);
	if (bPos7Old != m_bPos7)
	{
		CString strLog;
		CString strValue = m_bPos7 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置7]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckPos8()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bPos8Old = m_bPos8;

	UpdateData(TRUE);
	if (bPos8Old != m_bPos8)
	{
		CString strLog;
		CString strValue = m_bPos8 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改Ckeck[位置8]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDmcodeSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProductData;
	pTotalProductData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();

	if (pTotalProductData->GetCount()<= 0)
	{
		return;
	}

	int nCurProductIdx = pTotalProductData->GetCurProductIndex();
	// 获取当前产品对应的标定数据
	CProductData* pProductData;
	pProductData = pTotalProductData->GetItemPtr(nCurProductIdx);

	scDMCodeParam DmCodeParam;
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}
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

void CDlgSysKindCalibSetting::OnCbnSelchangeComboHDof()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CDlgSysKindCalibSetting::OnCbnSelchangeCmbCalibmode()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_comboCalibMarkPatternMode.GetCurSel()==eCalibUseDmCode || m_comboCalibMarkPatternMode.GetCurSel()==eWholeDmcodeBoardCalib)
	{
		GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(m_bShowDmCodeSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_DMCODE_SETTING)->ShowWindow(SW_HIDE);
	}
	if (m_comboCalibMarkPatternMode.GetCurSel()==eWholeDmcodeBoardCalib )
	{
		GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(m_bShowDmCodeSet);
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(m_bShowDmCodeSet);
		GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->EnableWindow(m_bEnableDmCodeSet);
	}
	else
	{
		GetDlgItem(IDC_CHK_WHOLE_CALIB_CACU_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->ShowWindow(SW_HIDE);
	}
	if (m_bEnableWholeCalibCacuPos)
	{
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(m_bEnableDmCodeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(FALSE);
	}
}

void CDlgSysKindCalibSetting::DistorCorrImage(int nPosIndex)
{
	CDlgDistorCorrImage Dlg;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	Dlg.m_nPosNum = platformInfo.m_nPositionNum;
	Dlg.m_bTargetObjectCamSeparate = platformInfo.m_bTargetObjectCamSeparate;
	Dlg.m_bCopyResult = optionInfo.m_bAutoCopyCalibData;

	Dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	Dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);
	Dlg.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex),nPosIndex);

	Dlg.m_dTileX = m_vdCalibTileX.at(nPosIndex);
	Dlg.m_dTileY = m_vdCalibTileY.at(nPosIndex);
	Dlg.m_bOutputOutpoints = m_vbEnableOutPoints.at(nPosIndex);
	Dlg.m_nThre = m_vnThre.at(nPosIndex);

	if (Dlg.DoModal() == IDOK)
	{
		m_vdCalibTileX.at(nPosIndex) = Dlg.m_dTileX;
		m_vdCalibTileY.at(nPosIndex) = Dlg.m_dTileY;
		m_vbEnableOutPoints.at(nPosIndex) = Dlg.m_bOutputOutpoints;
		m_vnThre.at(nPosIndex) = Dlg.m_nThre;

		// 获取当前平台对应的全部标定数据
		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
		if (pTotalCalibData == NULL)
		{
			return;
		}
		// 获取当前产品对应的标定数据
		CCalibData* pCalibData;
		pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
		if (pCalibData == NULL)
		{
			return;
		}

		// 获取扩展产品索引
		int nCurExProductIndex = 0;
		int nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosNum();

		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bEnableMultiCalibExtension == TRUE && optionInfo.m_bEnableCommMultiCalibExtension == TRUE)
		{
			nCurExProductIndex = Dlg.m_nCurExProductIndex;
		}

		// 设置当前位置对应的标定结果
		pCalibData->m_vpCalibratedInfo.at(nPosIndex + nCurExProductIndex*nPosNum)->m_result = Dlg.m_result;
		//m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(pCalibData->m_vpCalibratedInfo);
		//m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibratedInfoToCurProduct(pCalibData->m_vpCalibratedInfo);

		// 拷贝位置
		for(int nCopIndex = 0 ; nCopIndex < Dlg.m_vbCopyPos.size(); nCopIndex++)
		{
			if(Dlg.m_vbCopyPos[nCopIndex])
			{
				int nTmpPosIndex = nCopIndex;
				pCalibData->m_vpCalibratedInfo.at(nTmpPosIndex + nCurExProductIndex*nPosNum)->m_result = Dlg.m_result;
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(pCalibData->m_vpCalibratedInfo);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveCalibratedInfoToCurProduct(pCalibData->m_vpCalibratedInfo);

		// 标记产品模板已修改
		m_bProdPatternChanged = TRUE;

		// 保存畸变校正图片
		CPlatformOptionInfo optionInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
		if(optionInfo.m_bSaveCalibData)
		{
			CString strImageFileDirt;
			char ch = '\\';
			strImageFileDirt.Format(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDataRecordDirectory() + _T("\\CalibRecordData\\ProdcutIndex%d\\DistorCorr\\Ex%d"),m_nCurProductIdx,nCurExProductIndex);
			CreateMultipleDirectory(strImageFileDirt,ch);
			CString strImageName;
			strImageName.Format(_T("_Pos%d_ImageInput.bmp"),nPosIndex);
			SaveCalibSearchImage(strImageFileDirt,strImageName,Dlg.m_ImageInPut);

			strImageName.Format(_T("_Pos%d_ImageResult.bmp"),nPosIndex);
			SaveCalibSearchImage(strImageFileDirt,strImageName,Dlg.m_ImageResult);

			// 保存畸变校正的结果
			CString strResultName;
			strResultName.Format(_T("DistorReslut"));
			CString strOut;
			CString strTemp;
			// 畸变校正结果DistortCorrectResult
			double p[15];
			memset(p, 0.0, sizeof(double)*15);
			Dlg.m_result.GetNonlinearProjectResult(p);
			double dRms = Dlg.m_result.GetRMS();
			strTemp.Format(_T("%d,"),nPosIndex);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[0]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[1]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[2]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[3]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[4]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[5]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[6]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[7]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[8]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[9]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[10]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[11]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[12]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[13]);strOut += strTemp;
			strTemp.Format(_T("%.6f,"), p[14]);strOut += strTemp;
			strTemp.Format(_T("%.6f,\n"), dRms);strOut += strTemp;
			SaveCSVFile(strImageFileDirt,strResultName, strOut);
		}
		
	}
}

// 保存CVS 文件
bool CDlgSysKindCalibSetting::SaveCSVFile(CString strImageFileDirt,CString strName,CString& strSaveData )
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strTmp;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strFileName.Format("%s\\%s.csv", strImageFileDirt, strName);

	//判断文件是否存在
	CStdioFile file;
	CFileStatus rStatus;
	BOOL bRes = CFile::GetStatus(strFileName, rStatus);

	if (!file.Open(strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		return  false;
	}
	file.SeekToEnd();

	if(!bRes)//绘制表头
	{
		file.WriteString(_T("校正时间,位置,p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,\n"));
	}

	strTmp.Format("%04d-%02d-%02d-%02d-%02d-%02d-%03d,", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	strOut += strTmp;
	strOut += strSaveData;

	strOut+=_T("\n");
	file.WriteString(strOut);
	file.Close();
	return true;
}

// 保存畸变校正前的图片
void CDlgSysKindCalibSetting::SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave)
{
	CString strTmp = strImageName;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString strFileName;
	CString strOut;

	strTmp.Format("%04d%02d%02d_%02d_%02d_%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour,sys.wMinute,sys.wSecond);
	strTmp.Append(strImageName);
	strFileName.Format("%s\\%s", strImageFileDirt, strTmp);

	scDIB scDibTmp;
	scDibTmp.Init(imageSave);
	scDibTmp.Write(strFileName);

}

/************************************************************************/
/* 
功能：
	创建多级目录
参数：
	CString& szPath	文件路径名
	char split		分割字符，根据路径设置
*/
/************************************************************************/
bool CDlgSysKindCalibSetting::CreateMultipleDirectory(CString& szPath, char split)
{
	CString strDir(szPath);
	//	检测路径是否以 结尾
	if (strDir.GetAt(strDir.GetLength()-1) != split)
	{
		strDir.AppendChar(split);
	}
	std::vector<CString> vPath;	//	存放每一层目录字符串
	CString strTemp;			//	临时变量,存放目录字符串
	bool bSuccess = false;		//	成功标志
	// 遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != split) // 如果当前字符不是 split('\','//')
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else //如果当前字符是'\\'
		{
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(split);
		}
	}

	//	遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//	如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex1()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(0);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex2()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(1);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex3()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(2);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex4()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(3);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex5()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(4);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex6()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(5);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex7()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(6);
}


void CDlgSysKindCalibSetting::OnBnClickedBtnDistorcorrIndex8()
{
	// TODO: 在此添加控件通知处理程序代码
	DistorCorrImage(7);
}

void CDlgSysKindCalibSetting::ClipMouseCursor(BOOL bClip)
{
	if (bClip)
	{
		CRect rect;
		CPoint pt;
		GetCursorPos(&pt);
		rect.left = pt.x;
		rect.top = pt.y;
		rect.bottom = rect.top+1;
		rect.right  = rect.left+1;
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(NULL);
	}

}

void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex1)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX1)->EnableWindow(m_bEnableDistortionCorrectionD);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX1)->EnableWindow(FALSE);
	}
	
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex2)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX2)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex3)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX3)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX3)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex4)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX4)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex5)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX5)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX5)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex6)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX6)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX6)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex7()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex7)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX7)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX7)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedCheckDistorcorrIndex8()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_nEnableDisCorIndex8)
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX8)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DISTORCORR_INDEX8)->EnableWindow(FALSE);
	}
}


void CDlgSysKindCalibSetting::OnBnClickedChkWholeCalibCacuPos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bEnableWholeCalibCacuPos)
	{
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_WHOLE_CALIB_POS)->EnableWindow(FALSE);
	}
}


BOOL CDlgSysKindCalibSetting::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
		if (((ch > '9') || ((ch < '0'))) && (ch != '.'))
		{
			if ((i == 0) && ((ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindCalibSetting::IsStringPositiveIntNumerical(LPCTSTR lpszVal)
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i = 0; i < nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || (ch < '0'))
		{
			if ((i == 0) && ((ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT1, m_strCalibMovementX1);
		}
		else
		{
			m_strCalibMovementX1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT1, m_strCalibMovementX1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT2, m_strCalibMovementX2);
		}
		else
		{
			m_strCalibMovementX2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT2, m_strCalibMovementX2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT3, m_strCalibMovementX3);
		}
		else
		{
			m_strCalibMovementX3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT3, m_strCalibMovementX3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT4, m_strCalibMovementX4);
		}
		else
		{
			m_strCalibMovementX4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT4, m_strCalibMovementX4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT5, m_strCalibMovementX5);
		}
		else
		{
			m_strCalibMovementX5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT5, m_strCalibMovementX5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT6, m_strCalibMovementX6);
		}
		else
		{
			m_strCalibMovementX6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT6, m_strCalibMovementX6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT7, m_strCalibMovementX7);
		}
		else
		{
			m_strCalibMovementX7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT7, m_strCalibMovementX7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditXAxisMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT8, m_strCalibMovementX8);
		}
		else
		{
			m_strCalibMovementX8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_X_AXIS_MOVEMENT8, m_strCalibMovementX8);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT1, m_strCalibMovementY1);
		}
		else
		{
			m_strCalibMovementY1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT1, m_strCalibMovementY1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT2, m_strCalibMovementY2);
		}
		else
		{
			m_strCalibMovementY2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT2, m_strCalibMovementY2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT3, m_strCalibMovementY3);
		}
		else
		{
			m_strCalibMovementY3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT3, m_strCalibMovementY3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT4, m_strCalibMovementY4);
		}
		else
		{
			m_strCalibMovementY4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT4, m_strCalibMovementY4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT5, m_strCalibMovementY5);
		}
		else
		{
			m_strCalibMovementY5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT5, m_strCalibMovementY5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT6, m_strCalibMovementY6);
		}
		else
		{
			m_strCalibMovementY6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT6, m_strCalibMovementY6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT7, m_strCalibMovementY7);
		}
		else
		{
			m_strCalibMovementY7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT7, m_strCalibMovementY7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditYAxisMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT8, m_strCalibMovementY8);
		}
		else
		{
			m_strCalibMovementY8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_Y_AXIS_MOVEMENT8, m_strCalibMovementY8);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT1, m_strCalibMovementD1);
		}
		else
		{
			m_strCalibMovementD1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT1, m_strCalibMovementD1);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT2, m_strCalibMovementD2);
		}
		else
		{
			m_strCalibMovementD2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT2, m_strCalibMovementD2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT3, m_strCalibMovementD3);
		}
		else
		{
			m_strCalibMovementD3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT3, m_strCalibMovementD3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT4, m_strCalibMovementD4);
		}
		else
		{
			m_strCalibMovementD4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT4, m_strCalibMovementD4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT5, m_strCalibMovementD5);
		}
		else
		{
			m_strCalibMovementD5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT5, m_strCalibMovementD5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT6, m_strCalibMovementD6);
		}
		else
		{
			m_strCalibMovementD6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT6, m_strCalibMovementD6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT7, m_strCalibMovementD7);
		}
		else
		{
			m_strCalibMovementD7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT7, m_strCalibMovementD7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditDAxisMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT8, m_strCalibMovementD8);
		}
		else
		{
			m_strCalibMovementD8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_D_AXIS_MOVEMENT8, m_strCalibMovementD8);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, m_strCalibRefineMoveD1);
		}
		else
		{
			m_strCalibRefineMoveD1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT1, m_strCalibRefineMoveD1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, m_strCalibRefineMoveD2);
		}
		else
		{
			m_strCalibRefineMoveD2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT2, m_strCalibRefineMoveD2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, m_strCalibRefineMoveD3);
		}
		else
		{
			m_strCalibRefineMoveD3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT3, m_strCalibRefineMoveD3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, m_strCalibRefineMoveD4);
		}
		else
		{
			m_strCalibRefineMoveD4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT4, m_strCalibRefineMoveD4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, m_strCalibRefineMoveD5);
		}
		else
		{
			m_strCalibRefineMoveD5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT5, m_strCalibRefineMoveD5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, m_strCalibRefineMoveD6);
		}
		else
		{
			m_strCalibRefineMoveD6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT6, m_strCalibRefineMoveD6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, m_strCalibRefineMoveD7);
		}
		else
		{
			m_strCalibRefineMoveD7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT7, m_strCalibRefineMoveD7);
		}
	}
}
void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineAxisDMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, m_strCalibRefineMoveD8);
		}
		else
		{
			m_strCalibRefineMoveD8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_AXIS_D_MOVEMENT8, m_strCalibRefineMoveD8);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME1, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME1, m_strCalibRefineTime1);
		}
		else
		{
			m_strCalibRefineTime1.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME1, m_strCalibRefineTime1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME2, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME2, m_strCalibRefineTime2);
		}
		else
		{
			m_strCalibRefineTime2.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME2, m_strCalibRefineTime2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME3, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME3, m_strCalibRefineTime3);
		}
		else
		{
			m_strCalibRefineTime3.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME3, m_strCalibRefineTime3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME4, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME4, m_strCalibRefineTime4);
		}
		else
		{
			m_strCalibRefineTime4.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME4, m_strCalibRefineTime4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME5, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME5, m_strCalibRefineTime5);
		}
		else
		{
			m_strCalibRefineTime5.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME5, m_strCalibRefineTime5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME6, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME6, m_strCalibRefineTime6);
		}
		else
		{
			m_strCalibRefineTime6.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME6, m_strCalibRefineTime6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME7, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME7, m_strCalibRefineTime7);
		}
		else
		{
			m_strCalibRefineTime7.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME7, m_strCalibRefineTime7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditRefineTime8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REFINE_TIME8, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REFINE_TIME8, m_strCalibRefineTime8);
		}
		else
		{
			m_strCalibRefineTime8.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_REFINE_TIME8, m_strCalibRefineTime8);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, m_strCamCalibMovementX1);
		}
		else
		{
			m_strCamCalibMovementX1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT1, m_strCamCalibMovementX1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, m_strCamCalibMovementX2);
		}
		else
		{
			m_strCamCalibMovementX2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT2, m_strCamCalibMovementX2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, m_strCamCalibMovementX3);
		}
		else
		{
			m_strCamCalibMovementX3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT3, m_strCamCalibMovementX3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, m_strCamCalibMovementX4);
		}
		else
		{
			m_strCamCalibMovementX4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT4, m_strCamCalibMovementX4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, m_strCamCalibMovementX5);
		}
		else
		{
			m_strCamCalibMovementX5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT5, m_strCamCalibMovementX5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, m_strCamCalibMovementX6);
		}
		else
		{
			m_strCamCalibMovementX6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT6, m_strCamCalibMovementX6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, m_strCamCalibMovementX7);
		}
		else
		{
			m_strCamCalibMovementX7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT7, m_strCamCalibMovementX7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraXAxisMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, m_strCamCalibMovementX8);
		}
		else
		{
			m_strCamCalibMovementX8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_X_AXIS_MOVEMENT8, m_strCamCalibMovementX8);
		}
	}
}


void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, m_strCamCalibMovementY1);
		}
		else
		{
			m_strCamCalibMovementY1.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT1, m_strCamCalibMovementY1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, m_strCamCalibMovementY2);
		}
		else
		{
			m_strCamCalibMovementY2.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT2, m_strCamCalibMovementY2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, m_strCamCalibMovementY3);
		}
		else
		{
			m_strCamCalibMovementY3.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT3, m_strCamCalibMovementY3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, m_strCamCalibMovementY4);
		}
		else
		{
			m_strCamCalibMovementY4.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT4, m_strCamCalibMovementY4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, m_strCamCalibMovementY5);
		}
		else
		{
			m_strCamCalibMovementY5.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT5, m_strCamCalibMovementY5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, m_strCamCalibMovementY6);
		}
		else
		{
			m_strCamCalibMovementY6.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT6, m_strCamCalibMovementY6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, m_strCamCalibMovementY7);
		}
		else
		{
			m_strCamCalibMovementY7.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT7, m_strCamCalibMovementY7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditCameraYAxisMovement8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, m_strCamCalibMovementY8);
		}
		else
		{
			m_strCamCalibMovementY8.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CAMERA_Y_AXIS_MOVEMENT8, m_strCamCalibMovementY8);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime1()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME1, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME1, m_strSearchDelayTime1);
		}
		else
		{
			m_strSearchDelayTime1.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME1, m_strSearchDelayTime1);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime2()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME2, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME2, m_strSearchDelayTime2);
		}
		else
		{
			m_strSearchDelayTime2.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME2, m_strSearchDelayTime2);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime3()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME3, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME3, m_strSearchDelayTime3);
		}
		else
		{
			m_strSearchDelayTime3.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME3, m_strSearchDelayTime3);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime4()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME4, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME4, m_strSearchDelayTime4);
		}
		else
		{
			m_strSearchDelayTime4.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME4, m_strSearchDelayTime4);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime5()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME5, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME5, m_strSearchDelayTime5);
		}
		else
		{
			m_strSearchDelayTime5.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME5, m_strSearchDelayTime5);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime6()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME6, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME6, m_strSearchDelayTime6);
		}
		else
		{
			m_strSearchDelayTime6.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME6, m_strSearchDelayTime6);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime7()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME7, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME7, m_strSearchDelayTime7);
		}
		else
		{
			m_strSearchDelayTime7.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME7, m_strSearchDelayTime7);
		}
	}
}

void CDlgSysKindCalibSetting::OnEnKillfocusEditSearchDelayTime8()
{
	if(m_bKillFocusEnabled == TRUE)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME8, strVal);
		if (!IsStringPositiveIntNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME8, m_strSearchDelayTime8);
		}
		else
		{
			m_strSearchDelayTime8.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME8, m_strSearchDelayTime8);
		}
	}
}

BOOL CDlgSysKindCalibSetting::LoadSearchToolInfo()
{
	CTotalCalibData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalCalibData();
	if ((m_nPrevProductIdx < 0) || (m_nPrevProductIdx >= pTotalProduct->GetCount()))
	{
		return FALSE;
	}

	BOOL bSucceed = TRUE;
	// 保存产品信息到磁盘
	CString strDir, strTemp;
	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);

	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	strTemp.Format(_T("\\JobCalibs\\%03d"), m_nPrevProductIdx+1);
	strDir += strTemp; 
	CCalibData *pCalibData = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
	bSucceed = bSucceed && m_pParentKind->LoadSearchTool(eCalibSearch,strDir,pCalibData->m_vvpCalibObjectSearchToolRepo,m_pParentKind->m_vvbAlnCalibObjectSearchToolChanged);

	return bSucceed;
}

void CDlgSysKindCalibSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
{
	CRect CRectGroupBox;
	// 检查是否为Group控件
	if(NULL== GetDlgItem(nID)) return;
	if (NULL== (CStatic*) GetDlgItem(nID)) return;
	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	ScreenToClient(&CRectGroupBox);

	int nShow = SW_SHOW;
	if (bShow)
	{
		nShow = SW_SHOW;
	}
	else
	{
		nShow = SW_HIDE;
	}

	// Group 控件本身也进行显示或隐藏
	GetDlgItem(nID)->EnableWindow(FALSE);
	GetDlgItem(nID)->ShowWindow(SW_HIDE);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) && CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysKindCalibSetting::UpdateDlgByAuthorize()
{
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{

			CString strNodeName = _T("");
			// XYD步长
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_XYDMove,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowXYD,m_bEnableXYD);
			
			// 畸变矫正
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_DistortionCorrection,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowDistortionCorrection,m_bEnableDistortionCorrectionD);
			
			// 模板设置
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_TemplateSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowTemplateSet,m_bEnableTemplateSet);

			// 标定算法
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_CalibrationAlgorithm,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCalibrationAlgorithm,m_bEnableCalibrationAlgorithm);

			// 全局算法标定计算设置
			//vsConfig->GetCheckedByItem(eCalibrateAuthorize_CalibWhole,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowDmCodeSet,m_bEnableDmCodeSet);

			// 二维码参数设置
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_DmCodeSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowDmCodeSet,m_bEnableDmCodeSet);

			// 拷贝标定
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_CopyCalib,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCopyCalib,m_bEnableCopyCalib);

			// 标定产品列表
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_CalibList,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCalibList,m_bEnableCalibList);

		}
	}
}