// DlgSysKindAlignSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindAlignSetting.h"
#include "afxdialogex.h"


#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"


#include "DlgSysKind.h"

#include "DlgOffsetCalculate.h"			//20160510增加对角补偿
#include "DlgProductOffsetSetting.h"	// 补偿设置（补偿的方向、X和Y补偿值对调）

#include "CDlgPCBSetting.h"
#include "CDlgWeldInfo.h"
#include "DlgTargetFixOffsetSetting.h"
#include "DlgAlignOffsetDirectionSetting.h"
#include "DLGEightShapeAlignSetting.h"
#include "DLGBendSetting.h"
#include "DlgPhotoLightCtrlSet.h"

// CDlgSysKindAlignSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindAlignSetting, CDialog)

CDlgSysKindAlignSetting::CDlgSysKindAlignSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindAlignSetting::IDD, pParent)
	, m_bCheckTargetAndObjectDis(FALSE)
//	, m_strTargetAndObjectDis(_T(""))
	, m_nCheckMode(FALSE)
//	, m_bCheckTargetDis(FALSE)
//	, m_strTargetDis(_T(""))
//	, m_bCheckObjectDis(FALSE)	
//	, m_strObjectDis(_T(""))
//	, m_bCheckTargetAngle(FALSE)
//	, m_strTargetAngle(_T(""))
//	, m_bCheckPixelDis(FALSE)
//	, m_strPixelDisX1(_T(""))
// 	, m_strPixelDisY1(_T(""))
// 	, m_strPixelDisX2(_T(""))
// 	, m_strPixelDisY2(_T(""))
	, m_nSearchDelayTime(0)
	, m_nObjectSearchDelayTime(0)
	, m_nObjectSearchDelayTime2(0)
	, m_bEnableObjectSearchDelayTime2(FALSE)
// 	, m_strMovementRatioX(_T("1.0"))
// 	, m_strMovementRatioY(_T("1.0"))
// 	, m_bCheckEightShapeAlign(FALSE)
// 	, m_nEightShapeAlignMode(0)
// 	, m_dEightShapeAngle(0)
	//, m_bDiffPosWithSameSearchResultEnabled(FALSE)
	, m_strAlnTargetOffsetX(_T(""))
	, m_strAlnTargetOffsetY(_T(""))
	, m_bCheckTargetOffsetEnable(FALSE)
// 	, m_bObjectSearchShutter(FALSE)
// 	, m_bTargetSearchShutter(FALSE)
// 	, m_bInspectSearchShutter(FALSE)
	//, m_bEnableMutiTarget(FALSE)
	, m_bEnableSendTargetOffsetToPLC(FALSE)
	, m_strCurFaCalibName(_T(""))
	, m_nMultiCalibExtensionIndex(0)
	, m_bShowOffsetTips(FALSE)
	, m_bSetOffsetTip(FALSE)
//	, m_bEnableTarObjSeparateCheck(FALSE)
//	, m_strObjectCheckDis(_T(""))
	, m_bAlignAccordingToPickEnable(FALSE)
{
	//20160510增加对角补偿
	m_dObjectL1 = 0;
	m_dObjectW1 = 0;
	m_dTargetL2 = 0;
	m_dTargetW2 = 0;
	m_dF1 = 0;
	m_dF2 = 0;
	m_bOffsetXYEnable = false;
	m_nPosMode = 0;
	m_bEnableObjectPositionVirtualMode = false;
	m_bEnableTargetPositionVirtualMode = false;
	m_nObjectPositionVirtualMode = 0;
	m_nTargetPositionVirtualMode = 0;

	// 补偿设置（补偿的方向、X和Y补偿值对调）
	// 	m_nProductOffsetXDirection = 0;
	// 	m_nProductOffsetYDirection = 0;
	// 	m_nProductOffsetDDirection = 0;
	// 
	// 	m_nProductOffsetXSetting = 0;
	// 	m_nProductOffsetYSetting = 0;
	// 	m_nProductOffsetDSetting = 0;

	m_bEnableProductOffsetXDirection = FALSE;
	m_bEnableProductOffsetYDirection = FALSE;
	m_bEnableProductOffsetDDirection = FALSE;

	m_bEnableProductOffsetXSetting = FALSE;
	m_bEnableProductOffsetYSetting = FALSE;
	m_bEnableProductOffsetDSetting = FALSE;



	//{{AFX_DATA_INIT(CDlgSysKindAlignSetting)
	m_strAlignerBaseOffsetD = _T("0.000000");
	m_strAlignerBaseOffsetX = _T("0.000000");
	m_strAlignerBaseOffsetY = _T("0.000000");
	m_strAlignerOffsetD = _T("0.000000");
	m_strAlignerOffsetX = _T("0.000000");
	m_strAlignerOffsetY = _T("0.000000");
	m_strAlignerOffsetD2 = _T("0.000000");
	m_strAlignerOffsetX2 = _T("0.000000");
	m_strAlignerOffsetY2 = _T("0.000000");
	m_strAlignerOffsetD3 = _T("0.000000");
	m_strAlignerOffsetX3 = _T("0.000000");
	m_strAlignerOffsetY3 = _T("0.000000");
	m_strAlignerOffsetD4 = _T("0.000000");
	m_strAlignerOffsetX4 = _T("0.000000");
	m_strAlignerOffsetY4 = _T("0.000000");
	m_strAlignerOffsetD5 = _T("0.000000");
	m_strAlignerOffsetX5 = _T("0.000000");
	m_strAlignerOffsetY5 = _T("0.000000");
	m_strAlignerOffsetD6 = _T("0.000000");
	m_strAlignerOffsetX6 = _T("0.000000");
	m_strAlignerOffsetY6 = _T("0.000000");
	m_strAlignerBenchOffsetD = _T("0.000000");
	m_strAlignerBenchOffsetX = _T("0.000000");
	m_strAlignerBenchOffsetY = _T("0.000000");
	m_strAlignerPrecisionY = _T("0.010000");
	m_strAlignerPrecisionD = _T("0.010000");
	m_nAlignerMaxTimes = 1;
	m_nPlatformIndex = 0;
	m_strCurProductName = _T("");
	m_strCurCalibName = _T("");
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

	m_nPrevAlignerOffsetIndex = 0;
	m_nPrevAlignerBenchOffsetIndex = 0;
	m_bAlignAngleVertical = FALSE;

	//	m_pParent = (CVisionASMDlg *)pParent;
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalProductData.push_back(pTotalProduct);
	// 	}
	m_strAlignerWeight = _T("0.800000");




	m_bEnableTarFixOffset = FALSE;
	m_nTarFixOffsetType = 0;
	m_bAlignOffsetReverseX = FALSE;
	m_bAlignOffsetReverseY = FALSE;
	m_bAlignOffsetReverseD = FALSE;

	m_nPreMultiCalibExtensionIndex = 0;
	m_hBmpOffsetDir = NULL;

	m_bShowCheckMode			=true;
	m_bEnableCheckModeAuthoroze	=true;
	m_bShowOffset				=true;
	m_bEnableOffset				=true;
	m_bShowAlignToolParamSet	=true;
	m_bEnableAlignToolParamSet	=true;
	m_bShowAlignTypeSet			=true;
	m_bEnableAlignTypeSet		=true;
	m_bShowAlignOther			=true;
	m_bEnableAlignOther			=true;

	m_bShowAlignSpecialApplicationSet = true;
	m_bEnableAlignSpecialApplicationSet = true;

	m_bShowAlignerCirclePos		= false;
	
	m_bTargetDisThre = FALSE;
	m_bObjectDisThre = FALSE;
	m_bTargetAngleThre = FALSE;
	m_bObjectAngleThre = FALSE;

	m_strTargetDisThre = _T("0.010000");
	m_strObjectDisThre = _T("0.010000");
	m_strTargetAngleThre = _T("0.010000");
	m_strObjectAngleThre = _T("0.010000");

	// PCB参数
	m_strPCBInsPreX = _T("0.010000");
	m_strPCBInsPreY = _T("0.010000");
	m_strPCBInsPreD = _T("0.010000");
	m_nPCBInsPreType = 0;
	m_bPCBShowInspectResult = TRUE;
	m_bShowPCB4To3Align = TRUE;

	// 专用焊接参数
	m_bEnableWeld		  = FALSE;

	m_strWeldMovementRatioX		= _T("1.000000");
	m_strWeldMovementRatioY		= _T("1.000000");
	m_strWeldMovementRatioD		= _T("1.000000");	
	m_strWeldAmmendMoveRatioX	= _T("1.000000");
	m_strWeldAmmendMoveRatioY	= _T("1.000000");

	m_nWeldTargetSearchTimes	= 0;
	m_nWeldSearchBench			= 0;
	m_nWeldAngleBench			= 0;

	// 八字对位参数
	m_bCheckEightShapeAlign = FALSE;
	m_nEightShapeAlignMode = 0;
	m_dEightShapeAngle = 0.0;


	// 折弯参数
	m_bBendAlignInspect = FALSE;

	m_bBendType = FALSE;
	m_strPixelSize = _T("0.000000");
	m_bBendAlignDmCode = FALSE;

	m_bBendDisOffset = FALSE;
	m_strTargetDisOffset = _T("0.000000");
	m_strObjectDisOffset = _T("0.000000");

	m_bBendResultOffset = FALSE;
	m_strLeftOffsetX = _T("0.000000");
	m_strLeftOffsetY = _T("0.000000");
	m_strRightOffsetX = _T("0.000000");
	m_strRightOffsetY = _T("0.000000");
	m_strLeftDisOffset = _T("0.000000");
	m_strRightDisOffset = _T("0.000000");

	m_bBendThreOffset = FALSE;
	m_bEnableFbsLRY = FALSE;
	m_strThreOffsetX = _T("0.000000");
	m_strThreOffsetY = _T("0.000000");

	m_bBendPressPara = FALSE;
	m_strPressOKMinY = _T("0.000000");
	m_strPressOKMaxY = _T("0.000000");
	m_strPressNGMinY = _T("0.000000");
	m_strPressNGMaxY = _T("0.000000");


	m_bBendLoadThre = FALSE;
	m_strBendLoadThreX = _T("0.000000");
	m_strBendLoadThreY = _T("0.000000");

	m_bBendingInspect = FALSE;
	m_bBendingProductID = FALSE;
	m_bBendInspectDmCode = FALSE;
	m_bEnableMoveSnap = FALSE;

	m_bEnableAxisMoveLimit = FALSE;
	m_dAxisAllowMoveMaxX = 1;
	m_dAxisAllowMoveMaxY = 1;
	m_dAxisAllowMoveMaxD = 0.5;

	m_bBendInspectSnCodeInvert=FALSE;
	m_bBendInspectSnCodeLength=10;
	m_bBendInspectGetSnOnce=FALSE;

	m_strPlatformName1 = _T("平台号");
	m_strPlatformName2 = _T("平台号2");
	m_bEnableFbisTarObc = FALSE;

	m_nPosIndexFisrt = 0;

	m_bPhotoLightCtrlInfoChange = FALSE;

	m_bBendPressParaWrite = FALSE;
	m_lBendPressParaWriteAddre = 10000;

	m_bEnableReadWsInfo = FALSE;
	m_bEnableReadInspectWsInfo = FALSE;
	m_bEnableReadStartTimeInfo = FALSE;
	m_bEnableReadEndTimeInfo = FALSE;
	m_bEnableReadProsessingTimeInfo = FALSE;

	m_bEnableUseMathCaluLRDis = FALSE;
}

CDlgSysKindAlignSetting::~CDlgSysKindAlignSetting()
{
	DestroyWindow();
}

void CDlgSysKindAlignSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysKindAlignSetting)
	//	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	// DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrlProduct);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	// 	DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT, m_btnNewProduct);
	// 	DDX_Control(pDX, IDC_BUTTON_DELETE_PRODUCT, m_btnDeleteProduct);
	// 	DDX_Control(pDX, IDC_BTN_PRODUCT_RENAME, m_btnRename);
	// 	DDX_Control(pDX, IDC_BTN_AMEND_BENCHMARK, m_btnAmendBenchMark);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_2VS2_TYPE, m_comboAligner2Vs2Type);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_3VS3_TYPE, m_comboAligner3Vs3Type);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_4VS4_TYPE, m_comboAligner4Vs4Type);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS,m_comboAlignerCirclePos);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_MOVE_TYPE, m_comboAlignerMoveType);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, m_comboAlignerOffsetCoordType);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_OFFSET_INDEX, m_comboAlignerOffsetIndex);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_BENCH_OFFSET_INDEX, m_comboAlignerBenchOffsetIndex);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	// 	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);	
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_BASE_OFFSET, m_strAlignerBaseOffsetD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_BASE_OFFSET, m_strAlignerBaseOffsetX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_BASE_OFFSET, m_strAlignerBaseOffsetY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET2, m_strAlignerOffsetD2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET2, m_strAlignerOffsetX2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET2, m_strAlignerOffsetY2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET3, m_strAlignerOffsetD3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET3, m_strAlignerOffsetX3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET3, m_strAlignerOffsetY3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET4, m_strAlignerOffsetD4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET4, m_strAlignerOffsetX4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET4, m_strAlignerOffsetY4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET5, m_strAlignerOffsetD5);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET5, m_strAlignerOffsetX5);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET5, m_strAlignerOffsetY5);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET6, m_strAlignerOffsetD6);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET6, m_strAlignerOffsetX6);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET6, m_strAlignerOffsetY6);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_BENCH_OFFSET, m_strAlignerBenchOffsetD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_BENCH_OFFSET, m_strAlignerBenchOffsetX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, m_strAlignerBenchOffsetY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_MAX_TIMES, m_nAlignerMaxTimes);
	DDX_Text(pDX, IDC_EDIT_CALIB_NAME, m_strCurCalibName);
	//	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME, m_strCurProductName);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_TARGET_AND_OBJECT_DIS, m_bCheckTargetAndObjectDis);
//	DDX_Text(pDX, IDC_EDIT_TARGET_AND_OBJECT_DIS, m_strTargetAndObjectDis);
	DDX_Radio(pDX, IDC_RADIO_ALIGN_CHECK_MODE0, m_nCheckMode);
//	DDX_Check(pDX, IDC_CHECK_TARGET_DIS, m_bCheckTargetDis);	
//	DDX_Text(pDX, IDC_EDIT_TARGET_DIS, m_strTargetDis);
//	DDX_Check(pDX, IDC_CHECK_OBJECT_DIS, m_bCheckObjectDis);
//	DDX_Text(pDX, IDC_EDIT_OBJECT_DIS, m_strObjectDis);
//	DDX_Check(pDX, IDC_CHECK_TARGET_ANGLE, m_bCheckTargetAngle);
//	DDX_Text(pDX, IDC_EDIT_TARGET_ANGLE, m_strTargetAngle);
//	DDX_Check(pDX, IDC_CHECK_PIXEL_DIS, m_bCheckPixelDis);
//	DDX_Text(pDX, IDC_EDIT_PIXEL_DIS_X1, m_strPixelDisX1);
// 	DDX_Text(pDX, IDC_EDIT_PIXEL_DIS_Y1, m_strPixelDisY1);
// 	DDX_Text(pDX, IDC_EDIT_PIXEL_DIS_X2, m_strPixelDisX2);
// 	DDX_Text(pDX, IDC_EDIT_PIXEL_DIS_Y2, m_strPixelDisY2);
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME, m_nSearchDelayTime);
	DDX_Text(pDX, IDC_EDIT_OBJECT_SEARCH_DELAY_TIME, m_nObjectSearchDelayTime);
	DDX_Text(pDX, IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2, m_nObjectSearchDelayTime2);
	DDX_Check(pDX, IDC_CHECK__OBJECT_SEARCH_DELAY_TIME2, m_bEnableObjectSearchDelayTime2);
	//DDX_Check(pDX, IDC_CHECK_EIGHT_SHAPE_ALIGN, m_bCheckEightShapeAlign);
	//DDX_Radio(pDX, IDC_RADIO_POSITIVE_EIGHT, m_nEightShapeAlignMode);
	//DDX_Text(pDX, IDC_EDIT_EIGHIT_ANGLE, m_dEightShapeAngle);
	//DDX_Check(pDX, IDC_CHECK_MUL_MARK_MODE, m_bDiffPosWithSameSearchResultEnabled);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGET_X_OFFSET, m_strAlnTargetOffsetX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, m_strAlnTargetOffsetY);
	DDX_Check(pDX, IDC_CHK_SALN_OFFSET, m_bCheckTargetOffsetEnable);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_ANGLE_VERTICAL, m_bAlignAngleVertical);
	DDX_Radio(pDX, IDC_RADIO_ALIGNER_ANGLE_VERTICAL_POSITIVE, m_nAlignAngleVerticalMode);
	// 	DDX_Check(pDX, IDC_CHECK_OBJECT_SHUTTER, m_bObjectSearchShutter);
	// 	DDX_Check(pDX, IDC_CHECK_TARGET_SHUTTER, m_bTargetSearchShutter);
	// 	DDX_Check(pDX, IDC_CHECK_INSPECT_SHUTTER, m_bInspectSearchShutter);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_WIEGHT_4VS4, m_strAlignerWeight);
	//DDX_Check(pDX, IDC_CHECK_ENABLEMUTITARGET, m_bEnableMutiTarget);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SEND_OFFSET, m_bEnableSendTargetOffsetToPLC);
	DDX_Text(pDX, IDC_EDIT_CALIB_NAME_FA, m_strCurFaCalibName);


	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboMultiCalibExtension);
	DDX_CBIndex(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_nMultiCalibExtensionIndex);
//	DDX_Check(pDX, IDC_CHECK_OBJTAR_SEPARATE_CHECK, m_bEnableTarObjSeparateCheck);
//	DDX_Text(pDX, IDC_EDIT_OBJECT_CHECK_DIS, m_strObjectCheckDis);
	DDX_Check(pDX, IDC_CHK_ALIGN_ACCORDING_TO_PICK_ENABLE, m_bAlignAccordingToPickEnable);


	DDX_Check(pDX, IDC_CHECK_TARGET_DIS_THRE, m_bTargetDisThre);
	DDX_Check(pDX, IDC_CHECK_OBJECT_DIS_THRE, m_bObjectDisThre);
	DDX_Check(pDX, IDC_CHECK_TARGET_ANGLE_THRE, m_bTargetAngleThre);
	DDX_Check(pDX, IDC_CHECK_OBJECT_ANGLE_THRE, m_bObjectAngleThre);

	DDX_Text(pDX, IDC_EDIT_TARGET_DIS_THRE, m_strTargetDisThre);
	DDX_Text(pDX, IDC_EDIT_OBJECT_DIS_THRE, m_strObjectDisThre);
	DDX_Text(pDX, IDC_EDIT_TARGET_ANGLE_THRE, m_strTargetAngleThre);
	DDX_Text(pDX, IDC_EDIT_OBJECT_ANGLE_THRE, m_strObjectAngleThre);
}


BEGIN_MESSAGE_MAP(CDlgSysKindAlignSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysKindAlignSetting)
//	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
// 	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME, OnBtnProductRename)
// 	ON_BN_CLICKED(IDC_BTN_AMEND_BENCHMARK, OnBtnAmendBenchmark)
	//	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
// 	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT, OnButtonNewProduct)
// 	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRODUCT, OnButtonDeleteCurProduct)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnSetfocusEditAlignerMaxTimes)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnSetfocusEditAlignerXPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnSetfocusEditAlignerYPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnSetfocusEditAlignerDPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_BASE_OFFSET, OnSetfocusEditAlignerDBaseOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_BASE_OFFSET, OnSetfocusEditAlignerXBaseOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, OnSetfocusEditAlignerYBaseOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnSetfocusEditAlignerDOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnSetfocusEditAlignerXOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnSetfocusEditAlignerYOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET2, OnSetfocusEditAlignerDOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET2, OnSetfocusEditAlignerXOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET2, OnSetfocusEditAlignerYOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET3, OnSetfocusEditAlignerDOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET3, OnSetfocusEditAlignerXOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET3, OnSetfocusEditAlignerYOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET4, OnSetfocusEditAlignerDOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET4, OnSetfocusEditAlignerXOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET4, OnSetfocusEditAlignerYOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET5, OnSetfocusEditAlignerDOffset5)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET5, OnSetfocusEditAlignerXOffset5)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET5, OnSetfocusEditAlignerYOffset5)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET6, OnSetfocusEditAlignerDOffset6)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET6, OnSetfocusEditAlignerXOffset6)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET6, OnSetfocusEditAlignerYOffset6)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, OnSetfocusEditAlignerDBenchOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, OnSetfocusEditAlignerXBenchOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, OnSetfocusEditAlignerYBenchOffset)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduct)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnKillfocusEditAlignerXPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnKillfocusEditAlignerYPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnKillfocusEditAlignerDPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_BASE_OFFSET, OnKillfocusEditAlignerDBaseOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_BASE_OFFSET, OnKillfocusEditAlignerXBaseOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, OnKillfocusEditAlignerYBaseOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnKillfocusEditAlignerDOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnKillfocusEditAlignerXOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnKillfocusEditAlignerYOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET2, OnKillfocusEditAlignerDOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET2, OnKillfocusEditAlignerXOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET2, OnKillfocusEditAlignerYOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET3, OnKillfocusEditAlignerDOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET3, OnKillfocusEditAlignerXOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET3, OnKillfocusEditAlignerYOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET4, OnKillfocusEditAlignerDOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET4, OnKillfocusEditAlignerXOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET4, OnKillfocusEditAlignerYOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET5, OnKillfocusEditAlignerDOffset5)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET5, OnKillfocusEditAlignerXOffset5)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET5, OnKillfocusEditAlignerYOffset5)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET6, OnKillfocusEditAlignerDOffset6)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET6, OnKillfocusEditAlignerXOffset6)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET6, OnKillfocusEditAlignerYOffset6)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, OnKillfocusEditAlignerDBenchOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, OnKillfocusEditAlignerXBenchOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, OnKillfocusEditAlignerYBenchOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnKillfocusEditAlignerMaxTimes)
	//	ON_WM_CLOSE()
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnClickListProduct)
//	ON_BN_CLICKED(IDC_BTN_CLONE_PRODUCT, OnBtnCloneProduct)
	//}}AFX_MSG_MAP
//	ON_EN_SETFOCUS(IDC_EDIT_TARGET_AND_OBJECT_DIS, &CDlgSysKindAlignSetting::OnEnSetfocusEditTargetAndObjectDis)
//	ON_EN_SETFOCUS(IDC_EDIT_TARGET_DIS, &CDlgSysKindAlignSetting::OnEnSetfocusEditTargetDis)
//	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_DIS, &CDlgSysKindAlignSetting::OnEnSetfocusEditObjectDis)
//	ON_EN_SETFOCUS(IDC_EDIT_TARGET_ANGLE, &CDlgSysKindAlignSetting::OnEnSetfocusEditTargetAngle)
//	ON_EN_SETFOCUS(IDC_EDIT_PIXEL_DIS_X1, &CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisX1)
//	ON_EN_SETFOCUS(IDC_EDIT_PIXEL_DIS_X2, &CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisX2)
//	ON_EN_SETFOCUS(IDC_EDIT_PIXEL_DIS_Y1, &CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisY1)
//	ON_EN_SETFOCUS(IDC_EDIT_PIXEL_DIS_Y2, &CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisY2)
//	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_AND_OBJECT_DIS, &CDlgSysKindAlignSetting::OnEnKillfocusEditTargetAndObjectDis)
//	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_DIS, &CDlgSysKindAlignSetting::OnEnKillfocusEditTargetDis)
//	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_DIS, &CDlgSysKindAlignSetting::OnEnKillfocusEditObjectDis)
//	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_ANGLE, &CDlgSysKindAlignSetting::OnEnKillfocusEditTargetAngle)
//	ON_EN_KILLFOCUS(IDC_EDIT_PIXEL_DIS_X1, &CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisX1)
//	ON_EN_KILLFOCUS(IDC_EDIT_PIXEL_DIS_X2, &CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisX2)
//	ON_EN_KILLFOCUS(IDC_EDIT_PIXEL_DIS_Y1, &CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisY1)
//	ON_EN_KILLFOCUS(IDC_EDIT_PIXEL_DIS_Y2, &CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisY2)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_DELAY_TIME, &CDlgSysKindAlignSetting::OnEnSetfocusEditSearchDelayTime)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH_DELAY_TIME, &CDlgSysKindAlignSetting::OnEnKillfocusEditSearchDelayTime)
	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME, &CDlgSysKindAlignSetting::OnEnSetfocusEditObjectSearchDelayTime)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME, &CDlgSysKindAlignSetting::OnEnKillfocusEditObjectSearchDelayTime)

	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_OFFSET_INDEX, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerOffsetIndex)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerOffsetCoordType)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_CALCULATE, &CDlgSysKindAlignSetting::OnBnClickedButtonOffsetCalculate)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2, &CDlgSysKindAlignSetting::OnKillfocusEditObjectSearchDelayTime2)
	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2, &CDlgSysKindAlignSetting::OnSetfocusEditObjectSearchDelayTime2)
//	ON_EN_SETFOCUS(IDC_EDIT_MOVEMENT_RATIO_X, &CDlgSysKindAlignSetting::OnEnSetfocusEditMovementRatioX)
//	ON_EN_KILLFOCUS(IDC_EDIT_MOVEMENT_RATIO_X, &CDlgSysKindAlignSetting::OnEnKillfocusEditMovementRatioX)
//	ON_EN_SETFOCUS(IDC_EDIT_MOVEMENT_RATIO_Y, &CDlgSysKindAlignSetting::OnEnSetfocusEditMovementRatioY)
//	ON_EN_KILLFOCUS(IDC_EDIT_MOVEMENT_RATIO_Y, &CDlgSysKindAlignSetting::OnEnKillfocusEditMovementRatioY)
	//ON_EN_SETFOCUS(IDC_EDIT_EIGHIT_ANGLE, &CDlgSysKindAlignSetting::OnEnSetfocusEditEighitAngle)
	//ON_EN_KILLFOCUS(IDC_EDIT_EIGHIT_ANGLE, &CDlgSysKindAlignSetting::OnEnKillfocusEditEighitAngle)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, &CDlgSysKindAlignSetting::OnEnSetfocusEditAlignerTargetXOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, &CDlgSysKindAlignSetting::OnEnKillfocusEditAlignerTargetXOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, &CDlgSysKindAlignSetting::OnEnSetfocusEditAlignerTargetYOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, &CDlgSysKindAlignSetting::OnEnKillfocusEditAlignerTargetYOffset)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_BENCH_OFFSET_INDEX, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerBenchOffsetIndex)
	ON_BN_CLICKED(IDC_BUTTON_PRODUCT_OFFSET, &CDlgSysKindAlignSetting::OnBnClickedButtonProductOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_WIEGHT_4VS4, &CDlgSysKindAlignSetting::OnKillfocusEditAlignerWieght4vs4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_WIEGHT_4VS4, &CDlgSysKindAlignSetting::OnSetfocusEditAlignerWieght4vs4)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_4VS4_TYPE, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner4vs4Type)
	ON_BN_CLICKED(IDC_CHECK__OBJECT_SEARCH_DELAY_TIME2, &CDlgSysKindAlignSetting::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_TARGET_AND_OBJECT_DIS, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetAndObjectDis)
	ON_BN_CLICKED(IDC_CHK_SALN_OFFSET, &CDlgSysKindAlignSetting::OnBnClickedChkSalnOffset)
//	ON_BN_CLICKED(IDC_CHECK_TARGET_ANGLE, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetAngle)
	ON_BN_CLICKED(IDC_CHECK_TARGET_SHUTTER, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetShutter)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_SHUTTER, &CDlgSysKindAlignSetting::OnBnClickedCheckObjectShutter)
	//ON_BN_CLICKED(IDC_CHECK_MUL_MARK_MODE, &CDlgSysKindAlignSetting::OnBnClickedCheckMulMarkMode)
	//ON_BN_CLICKED(IDC_CHECK_ENABLEMUTITARGET, &CDlgSysKindAlignSetting::OnBnClickedCheckEnablemutitarget)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_ANGLE_VERTICAL, &CDlgSysKindAlignSetting::OnBnClickedCheckAlignerAngleVertical)
	//ON_BN_CLICKED(IDC_CHECK_EIGHT_SHAPE_ALIGN, &CDlgSysKindAlignSetting::OnBnClickedCheckEightShapeAlign)
	ON_BN_CLICKED(IDC_RADIO_ALIGN_CHECK_MODE0, &CDlgSysKindAlignSetting::OnAlignCheckModeChange)
	ON_BN_CLICKED(IDC_RADIO_ALIGN_CHECK_MODE1, &CDlgSysKindAlignSetting::OnAlignCheckModeChange)
//	ON_BN_CLICKED(IDC_CHECK_TARGET_DIS, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetDis)
//	ON_BN_CLICKED(IDC_CHECK_OBJECT_DIS, &CDlgSysKindAlignSetting::OnBnClickedCheckObjectDis)
	ON_BN_CLICKED(IDC_RADIO_ALIGNER_ANGLE_VERTICAL_POSITIVE, &CDlgSysKindAlignSetting::OnAlignerAngleVerticalChange)
	ON_BN_CLICKED(IDC_RADIO_ALIGNER_ANGLE_VERTICAL_NEGATIVE, &CDlgSysKindAlignSetting::OnAlignerAngleVerticalChange)
	//ON_BN_CLICKED(IDC_RADIO_POSITIVE_EIGHT, &CDlgSysKindAlignSetting::OnAlignCheckEightModeChange)
	//ON_BN_CLICKED(IDC_RADIO_NEGTIVE_EIGHT, &CDlgSysKindAlignSetting::OnAlignCheckEightModeChange)
//	ON_BN_CLICKED(IDC_CHECK_PIXEL_DIS, &CDlgSysKindAlignSetting::OnBnClickedCheckPixelDis)
	//ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, &CDlgSysKindAlignSetting::OnAlignerOffsetCoordTypeChange)
	//DDX_Control(pDX, IDC_COMBO_ALIGNER_2VS2_TYPE, m_comboAligner2Vs2Type);
	//DDX_Control(pDX, IDC_COMBO_ALIGNER_3VS3_TYPE, m_comboAligner3Vs3Type);
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_2VS2_TYPE, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner2vs2Type)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_3VS3_TYPE, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner3vs3Type)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_MOVE_TYPE, &CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerMoveType)
	ON_EN_KILLFOCUS(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, &CDlgSysKindAlignSetting::OnKillfocusEditAmmendMovementRatioX)
	ON_EN_SETFOCUS(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, &CDlgSysKindAlignSetting::OnSetfocusEditAmmendMovementRatioX)
	ON_EN_KILLFOCUS(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, &CDlgSysKindAlignSetting::OnKillfocusEditAmmendMovementRatioY)
	ON_EN_SETFOCUS(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, &CDlgSysKindAlignSetting::OnSetfocusEditAmmendMovementRatioY)
	ON_BN_CLICKED(IDC_BTN_PCB_SETTING, &CDlgSysKindAlignSetting::OnBnClickedBtnPcbSetting)
	ON_BN_CLICKED(IDC_BTN_WELD_SETTING, &CDlgSysKindAlignSetting::OnBnClickedBtnWeldSetting)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SEND_OFFSET, &CDlgSysKindAlignSetting::OnBnClickedCheckEnableSendOffset)
	ON_BN_CLICKED(IDC_BTN_TAR_FIX_OFFSET, &CDlgSysKindAlignSetting::OnBnClickedBtnTarFixOffset)
	ON_BN_CLICKED(IDC_BTN_ALIGN_OFFSET_DIRECTION_SETTING, &CDlgSysKindAlignSetting::OnBnClickedBtnAlignOffsetSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_INDEX, &CDlgSysKindAlignSetting::OnCbnSelchangeComboExproductIndex)
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS, &CDlgSysKindAlignSetting::OnSelchangeComboAligner4vs4Circlepos)
//	ON_BN_CLICKED(IDC_CHECK_OBJTAR_SEPARATE_CHECK, &CDlgSysKindAlignSetting::OnBnClickedCheckObjtarSeparateCheck)
//	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_CHECK_DIS, &CDlgSysKindAlignSetting::OnEnSetfocusEditObjectCheckDis)
//	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_CHECK_DIS, &CDlgSysKindAlignSetting::OnEnKillfocusEditObjectCheckDis)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_DIS_THRE, &CDlgSysKindAlignSetting::OnSetfocusEditTargetDisThre)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_DIS_THRE, &CDlgSysKindAlignSetting::OnKillfocusEditTargetDisThre)
	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_DIS_THRE, &CDlgSysKindAlignSetting::OnSetfocusEditObjectDisThre)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_DIS_THRE, &CDlgSysKindAlignSetting::OnKillfocusEditObjectDisThre)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_ANGLE_THRE, &CDlgSysKindAlignSetting::OnSetfocusEditTargetAngleThre)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_ANGLE_THRE, &CDlgSysKindAlignSetting::OnKillfocusEditTargetAngleThre)
	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_ANGLE_THRE, &CDlgSysKindAlignSetting::OnSetfocusEditObjectAngleThre)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_ANGLE_THRE, &CDlgSysKindAlignSetting::OnKillfocusEditObjectAngleThre)
	ON_BN_CLICKED(IDC_CHECK_TARGET_DIS_THRE, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetDisThre)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_DIS_THRE, &CDlgSysKindAlignSetting::OnBnClickedCheckObjectDisThre)
	ON_BN_CLICKED(IDC_CHECK_TARGET_ANGLE_THRE, &CDlgSysKindAlignSetting::OnBnClickedCheckTargetAngleThre)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_ANGLE_THRE, &CDlgSysKindAlignSetting::OnBnClickedCheckObjectAngleThre)
	ON_BN_CLICKED(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING, &CDlgSysKindAlignSetting::OnBnClickedBtnEightShapeAlignSetting)
	ON_BN_CLICKED(IDC_BTN_BEND_SETTING, &CDlgSysKindAlignSetting::OnBnClickedBtnBendSetting)
	ON_BN_CLICKED(IDC_BTN_CMD_LIGHT_CTRL_SET, &CDlgSysKindAlignSetting::OnBnClickedBtnCmdLightCtrlSet)
	END_MESSAGE_MAP()


// CDlgSysKindAlignSetting 消息处理程序


BOOL CDlgSysKindAlignSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
// 
// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
// 	CTotalProductData *pTotalProduct = NULL;
// 	for (int i=0; i<nPlatformNum; i++)
// 	{
// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
// 		m_vpTotalProductData.push_back(pTotalProduct);
// 	}



	// TODO:  在此添加额外的初始化
// 	CString strModuelDir;
// 	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
// 	strModuelDir.ReleaseBuffer();
//	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   

	m_strModuleDir = m_pParentKind->m_strModuleDir;
	
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;//不加这句会导致崩溃
	int nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_strProductNameOld = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);

	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);
	UpdateProductListDisplay();

// 	int nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);
// 

	if(0)
	{
		TCHAR szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, MAX_PATH);
	
		CString strCurWorkDir = szPath;
		strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));
		HBITMAP hbmp;

		hbmp = (HBITMAP)::LoadImage(NULL,strCurWorkDir+"\\OffSet.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE);

		if (hbmp != NULL)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_PICTURE))->SetBitmap((HBITMAP)hbmp);
		}
	}

	GetDlgItem(IDC_STATIC_GRP_ALIGN_PRECISION)->SetFocus();
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}




void CDlgSysKindAlignSetting::InitDlgItem()
{
	// lzk
	if (m_pParent != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);

		//2vs2
		vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
		int nComboNum = 0;
		iter2Vs2 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.begin();		
		for (;iter2Vs2 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.end();iter2Vs2++)  
		{
			if (iter2Vs2->m_bOpenAligner)
			{
				m_comboAligner2Vs2Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
				m_comboAligner2Vs2Type.SetItemData(nComboNum,iter2Vs2->m_alignerType);
				nComboNum ++;
			}
		}
		//3vs3
		vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
		nComboNum = 0;
		iter3Vs3 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.begin();		
		for (;iter3Vs3 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.end();iter3Vs3++)  
		{
			if (iter3Vs3->m_bOpenAligner)
			{
				m_comboAligner3Vs3Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
				m_comboAligner3Vs3Type.SetItemData(nComboNum,iter3Vs3->m_alignerType);
				nComboNum ++;
			}
		}
		//4vs4
		vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
		nComboNum = 0;
		iter4Vs4 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.begin();		
		for (;iter4Vs4 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.end();iter4Vs4++)  
		{
			if (iter4Vs4->m_bOpenAligner)
			{
				m_comboAligner4Vs4Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
				m_comboAligner4Vs4Type.SetItemData(nComboNum,iter4Vs4->m_alignerType);
				nComboNum ++;
			}
		}
	}
	int i = 0;	

	//m_comboAligner2Vs2Type.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO));
	//m_comboAligner2Vs2Type.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1));
	//m_comboAligner2Vs2Type.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2));
	//m_comboAligner2Vs2Type.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1L2));
	//m_comboAligner2Vs2Type.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2L1));
	//m_comboAligner2Vs2Type.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12Dot1L2));
	//m_comboAligner2Vs2Type.InsertString(6, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12Dot2L1));
	//m_comboAligner2Vs2Type.InsertString(7, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12L1));
	//m_comboAligner2Vs2Type.InsertString(8, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12L2));
	//m_comboAligner2Vs2Type.InsertString(9, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_L12));
	//m_comboAligner2Vs2Type.InsertString(10, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_CTO3));
	//m_comboAligner2Vs2Type.InsertString(11, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_COMBINE_C12));
	//m_comboAligner2Vs2Type.InsertString(12, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_COMBINE_C12V));
	////jjh0601
	//m_comboAligner2Vs2Type.InsertString(13, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO_WEIGHT));
	//m_comboAligner2Vs2Type.InsertString(14, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1_WEIGHT));
	//m_comboAligner2Vs2Type.InsertString(15, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2_WEIGHT));

	//m_comboAligner3Vs3Type.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO));
	//m_comboAligner3Vs3Type.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1));
	//m_comboAligner3Vs3Type.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2));
	//m_comboAligner3Vs3Type.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C3));
	//m_comboAligner3Vs3Type.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C13L12));
	//m_comboAligner3Vs3Type.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C13AL12));
	//m_comboAligner3Vs3Type.InsertString(6, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2L13));
	//m_comboAligner3Vs3Type.InsertString(7, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2TL13));
	////jjh0611
	//m_comboAligner3Vs3Type.InsertString(8, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_WEIGHT));
	//m_comboAligner3Vs3Type.InsertString(9, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1_WEIGHT));
	//m_comboAligner3Vs3Type.InsertString(10, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2_WEIGHT));
	//m_comboAligner3Vs3Type.InsertString(11, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_L1C_WEIGHT)); //jjh0619
	//
	//m_comboAligner4Vs4Type.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_AUTO));
	//m_comboAligner4Vs4Type.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_123C1));
	//m_comboAligner4Vs4Type.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_124C2));
	//m_comboAligner4Vs4Type.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_134C3));
	//m_comboAligner4Vs4Type.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_234C4));
	//m_comboAligner4Vs4Type.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE));
	//m_comboAligner4Vs4Type.InsertString(6, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_4VS4));
	//m_comboAligner4Vs4Type.InsertString(7, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_TARGET_4VS4));
	//m_comboAligner4Vs4Type.InsertString(8, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_OBJECT_4VS4));
	//m_comboAligner4Vs4Type.InsertString(9, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_1234_WEIGHT));//_T("位置1234_基准5_均方误差_权重"));
	//m_comboAligner4Vs4Type.InsertString(10,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_WEIGHT));// _T("位置1234_基准5_综合对位_权重"));
	//m_comboAligner4Vs4Type.InsertString(11,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_56_4V2));//_T("位置1234_56_4V2"));
	//m_comboAligner4Vs4Type.InsertString(12,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_CENTER));//位置1、2、3、4中心对位5、6、7、8中心，长边对位角度（不重新构造点）
	//	
	m_comboAlignerCirclePos.InsertString(0,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_1));//_T("位置1"));
	m_comboAlignerCirclePos.InsertString(1,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_2));//_T("位置2"));
	//m_comboAlignerCirclePos.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_3));//_T("位置3"));
	//m_comboAlignerCirclePos.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_4));//_T("位置4"));

	m_comboAlignerMoveType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_XYD));
	m_comboAlignerMoveType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_XY));
// 	m_comboAlignerMoveType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_XD));
// 	m_comboAlignerMoveType.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_YD));
// 	m_comboAlignerMoveType.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_X));
// 	m_comboAlignerMoveType.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_Y));
// 	m_comboAlignerMoveType.InsertString(6, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_MOVE_D));

	m_comboAlignerOffsetCoordType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_COORD));
	m_comboAlignerOffsetCoordType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PLTFM_COORD));
	m_comboAlignerOffsetCoordType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_PLATFORM_COORD));
	m_comboAlignerOffsetCoordType.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_COORD_DIAGONAL));				//20160510增加对角补偿
	m_comboAlignerOffsetCoordType.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_COORD_DIAGONAL2));			//20160510增加对角补偿

	// 补偿
	CString strCombItem;
	for(i = 0; i < (ALIGNOFFSETNUM - 5); i++)
	{
		strCombItem.Format(_T("%d"),i+6);
		m_comboAlignerOffsetIndex.InsertString(i, strCombItem);
	}
	m_comboAlignerOffsetIndex.SetCurSel(0);

	// 下料补偿
	for(i = 0; i < ALIGNBENCHOFFSETNUM; i++)
	{
		strCombItem.Format(_T("%d"),i+1);
		m_comboAlignerBenchOffsetIndex.InsertString(i, strCombItem);
	}
	m_comboAlignerBenchOffsetIndex.SetCurSel(0);

	// 修改新选择的平台的显示界面
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);

	int nShowAlignTypeSet = m_bShowAlignTypeSet?SW_SHOW:SW_HIDE;

	// 对位模式界面修改
	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
		/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
	{
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(nShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(nShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
	}

	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
	{
		//jjh0611
		m_comboAlignerCirclePos.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_3));//_T("位置3")); //jjh0611
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(nShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(nShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
	}

	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
	{
		//jjh0601
		m_comboAlignerCirclePos.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_3));//_T("位置3"));
		m_comboAlignerCirclePos.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_4));//_T("位置4"));
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(nShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(nShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
	}

	//if (platformInfo.m_bPlatformPick == TRUE || 1)
	//{		
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(TRUE);		
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(TRUE);		
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(FALSE);		
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
	//}

    //if (FALSE) // 八字对位显示
    //{
    //    GetDlgItem(IDC_CHECK_EIGHT_SHAPE_ALIGN)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_RADIO_POSITIVE_EIGHT)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_RADIO_NEGTIVE_EIGHT)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_STATIC_EIGHT_ANGLE)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_STATIC_DEGREE)->ShowWindow(SW_SHOW);
    //}
    //else
    //{
    //    GetDlgItem(IDC_CHECK_EIGHT_SHAPE_ALIGN)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_RADIO_POSITIVE_EIGHT)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_RADIO_NEGTIVE_EIGHT)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_STATIC_EIGHT_ANGLE)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_STATIC_DEGREE)->ShowWindow(SW_HIDE);
    //}

	int nShowOffset = m_bShowOffset?SW_SHOW:SW_HIDE;

	if (platformInfo.m_bTargetCalibUseBoard && platformInfo.m_bTargetObjectCamSeparate)
	{
		GetDlgItem(IDC_BTN_TAR_FIX_OFFSET)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_TAR_FIX_OFFSET)->ShowWindow(SW_HIDE);
	}

	// 专用焊接参数
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}

	
	if (optionInfo.m_bShowFWEL)
	{
		GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(FALSE);
	}

	//光源控制
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(FALSE);
	}

	// PCB 参数
	if (optionInfo.m_bShowBend)
	{
		GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(FALSE);
	}
	GetDlgItem(IDC_BTN_PCB_SETTING)->ShowWindow(FALSE);

	if (optionInfo.m_bShowEightShapeAlign)
	{
		GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(FALSE);
	}

	// 补偿备注显示设置参数
	CPlatformOptionInfo     platformOptionInfo;
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	CProductData product;
	int nCurProdIdx = pTotalProduct->GetCurProductIndex();	
	product = pTotalProduct->GetItem(nCurProdIdx);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(platformOptionInfo);
	m_bSetOffsetTip = platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
	m_bShowOffsetTips = platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;
	for (int i=0;i<ALIGNOFFSETNUM;i++)
	{
		m_vstrOffSetTip.push_back(product.m_pAlignerParam->GetAlignerOffsetTip(i));
	}
	if (m_bShowOffsetTips)
	{
		GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(0));
		GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP2)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(1));
		GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP3)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(2));
		GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP4)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(3));
		GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP5)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(4));
		GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(nShowOffset);
		GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(5));
	}
	else
	{
		GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(0));
		GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP2)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(1));
		GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP3)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(2));
		GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP4)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(3));
		GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP5)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(4));
		GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(5));
	}



	//// 多目标对位设置
	//if (optionInfo.m_bEnableMutiTarget == TRUE)
	//{
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->EnableWindow(TRUE);	
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->EnableWindow(TRUE);	
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->EnableWindow(FALSE);	
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->EnableWindow(FALSE);	
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->ShowWindow(SW_HIDE);
	//}

	// 启用单产品扩展，才启用控件
	int nShowCheckMode = m_bShowCheckMode?SW_SHOW:SW_HIDE;
	if (platformInfo.m_bEnableMultiCalibExtension)
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(nShowCheckMode);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(nShowCheckMode);
		m_comboMultiCalibExtension.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_comboMultiCalibExtension.InsertString(it,(LPCTSTR)strNum);
		}
		m_comboMultiCalibExtension.SetCurSel(0);

	}
	else
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		m_comboMultiCalibExtension.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_comboMultiCalibExtension.InsertString(it,(LPCTSTR)strNum);
		}
		m_comboMultiCalibExtension.SetCurSel(0);
	}

	if (m_pParent->m_SysOptionConfig.m_bCheckModeOnly)
	{
		GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_TARGET_DIS_THRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_OBJECT_DIS_THRE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_CHECK_TARGET_DIS_THRE)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_CHECK_OBJECT_DIS_THRE)->EnableWindow(m_bEnableCheckModeAuthoroze);
	}

	CRect tmpRect;
	GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CALIB_NAME_FA,tmpRect));

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignSetting::MoveDlgItem()
{
	if (GetDlgItem(IDC_STATIC_GRP_ALIGN_PRECISION)->GetSafeHwnd() != NULL)
	{
		// 修改新选择的平台的显示界面
		SysPlatformInfo platformInfo;
		CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
		m_nPlatformIndex = pView->m_nPlatformIndex;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);

		if(platformInfo.m_bProductFaChd)
		{
			// 双层结构
			CRect tmpRect,tmpRect2;
			map<int, CRect>::iterator iter; 

			iter = m_InitPosition.find(IDC_EDIT_CALIB_NAME_FA);  
			if(iter != m_InitPosition.end())  
			{
				GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->MoveWindow(iter->second);
				GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->ShowWindow(m_bShowAlignToolParamSet);
				GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(m_bShowAlignToolParamSet);
			}
		}
		else
		{
			// 单层结构
			CRect rect1,rect2;
			GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->GetWindowRect(rect1);
			ScreenToClient(rect1);
			GetDlgItem(IDC_EDIT_CALIB_NAME)->GetWindowRect(rect2);
			ScreenToClient(rect2);

			rect1.right = rect2.right;
			GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->MoveWindow(rect1);
			GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(HIDE_WINDOW);
		}

		// 对位模式界面修改
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
			GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
			GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(m_bShowAlignTypeSet);
			GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		}
		else
		{
			GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);		
			GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
		}

		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
			GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
			GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(m_bShowAlignTypeSet);
			GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		}
		else
		{
			GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);		
			GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
		}

		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
			GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(m_bShowAlignTypeSet);
			GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		}
		else
		{
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);		
			GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
		}

		//if (platformInfo.m_bPlatformPick == TRUE || 1)
		//{		
		//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(TRUE);		
		//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(TRUE);		
		//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
		//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
		//}
		//else
		//{
		//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(FALSE);		
		//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
		//}

// 		// 针对X平台的运动量系数
// 		if (platformInfo.m_eMidPlatformType == ePlatformX)
// 		{
// 			GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_X)->ShowWindow(TRUE);
// 			GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_X)->ShowWindow(TRUE);
// 			GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_Y)->ShowWindow(TRUE);
// 			GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_Y)->ShowWindow(TRUE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_X)->ShowWindow(FALSE);
// 			GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_X)->ShowWindow(FALSE);
// 			GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_Y)->ShowWindow(FALSE);
// 			GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_Y)->ShowWindow(FALSE);
// 		}

		// 专用焊接参数
		CPlatformOptionInfo optionInfo;
		if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
		{
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
		}

//		BOOL bEnableProductSize = (m_comboAlignerOffsetCoordType.GetCurSel() == 3) || (m_comboAlignerOffsetCoordType.GetCurSel() == 4) && (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode);
		BOOL bEnableProductSize = (m_comboAlignerOffsetCoordType.GetCurSel() == 3) || (m_comboAlignerOffsetCoordType.GetCurSel() == 4) || (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode);
		GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(m_bEnableOffset && bEnableProductSize);

		if (optionInfo.m_bShowFWEL)
		{
			GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
			GetDlgItem(IDC_BTN_WELD_SETTING)->EnableWindow(m_bEnableAlignSpecialApplicationSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(FALSE);
			GetDlgItem(IDC_BTN_WELD_SETTING)->EnableWindow(FALSE);
		}

		//光源控制
		if (optionInfo.m_bEnablePhotoLightCtrl)
		{
			GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(m_bShowAlignSpecialApplicationSet);
			GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->EnableWindow(m_bEnableAlignSpecialApplicationSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(FALSE);
			GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->EnableWindow(FALSE);
		}
		

		// PCB 参数
		if (optionInfo.m_bShowBend)
		{
			GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
			GetDlgItem(IDC_BTN_BEND_SETTING)->EnableWindow(m_bEnableAlignSpecialApplicationSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(FALSE);
			GetDlgItem(IDC_BTN_BEND_SETTING)->EnableWindow(FALSE);
		}
			GetDlgItem(IDC_BTN_PCB_SETTING)->ShowWindow(FALSE);
			GetDlgItem(IDC_BTN_PCB_SETTING)->EnableWindow(FALSE);


		if (optionInfo.m_bShowEightShapeAlign)
		{
			GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
			GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->EnableWindow(m_bEnableAlignSpecialApplicationSet);
		}
		else
		{
			GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(FALSE);
			GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->EnableWindow(FALSE);
		}

		// 补偿备注显示设置参数
		CPlatformOptionInfo     platformOptionInfo;
		CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
		CProductData product;
		int nCurProdIdx = pTotalProduct->GetCurProductIndex();	
		product = pTotalProduct->GetItem(nCurProdIdx);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(platformOptionInfo);
		m_bSetOffsetTip = platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
		m_bShowOffsetTips = platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;
		if (m_bShowOffsetTips)
		{
			GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP1)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(0));
			GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP2)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(1));
			GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP3)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(2));
			GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP4)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(3));
			GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP5)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(4));
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(product.m_pAlignerParam->GetAlignerOffsetTip(5));
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(FALSE);
		}



		if (m_pParent->m_SysOptionConfig.m_bCheckModeOnly)
		{
			GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TARGET_DIS)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
			GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(m_bEnableCheckModeAuthoroze);
			GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(m_bEnableCheckModeAuthoroze);
			GetDlgItem(IDC_CHECK_TARGET_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
			GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
		}
	}
	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignSetting::CheckDlgData()
{
	UpdateData(TRUE);

	// 对位次数
	m_nAlignerMaxTimes  = (m_nAlignerMaxTimes > 10) ? 10 : ((m_nAlignerMaxTimes < 1) ? 1 : m_nAlignerMaxTimes);
}

void CDlgSysKindAlignSetting::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CProductData product;


	CPlatformOptionInfo     platformOptionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(platformOptionInfo);
	m_bSetOffsetTip = platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
	m_bShowOffsetTips = platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;


    //小数位数格式化
    CString strXY,strD,strTip;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);

	if (bSaveAndValidate)
	{	
		UpdateData(TRUE);
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		product = pTotalProduct->GetItem(nCurProdIdx);
		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
		//product.m_pAlignerParam->SetEnableMutiTarget(m_bEnableMutiTarget);

		if (m_bShowOffsetTips)
		{
			int i;
			for(i =0 ;i<ALIGNOFFSETNUM;i++)
			{
				product.m_pAlignerParam->SetAlignerOffsetTip(i,m_vstrOffSetTip[i]);
			}
			for (i=0;i<5;i++)
			{
				GetDlgItem(IDC_STATIC_TIP1+i)->ShowWindow(m_bShowOffset);
				GetDlgItem(IDC_STATIC_TIP1+i)->SetWindowText(m_vstrOffSetTip[i]);
			}
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(m_vstrOffSetTip[m_comboAlignerOffsetIndex.GetCurSel()+5]);
			
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(FALSE);
		}


		CCoordPos pos;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX);
			pos.m_dPosY = atof(m_strAlignerPrecisionY);
			pos.m_dAngle = atof(m_strAlignerPrecisionD);
		}
		
		product.m_pAlignerParam->SetAlignerPrecision(pos);
		//lzk
		product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel()));
		product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel()));
		product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel()));

		//product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetCurSel());
		//product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetCurSel());
		//product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetCurSel());
		product.m_pAlignerParam->SetAlignerMoveType((AlignerMoveType)m_comboAlignerMoveType.GetCurSel());
		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());

		product.m_pAlignerParam->SetAlignerWeight(atof(m_strAlignerWeight));
		product.m_pAlignerParam->SetAlignerCirclePos(m_comboAlignerCirclePos.GetCurSel());

		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerBaseOffsetX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerBaseOffsetY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerBaseOffsetD)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerBaseOffsetX);
			pos.m_dPosY = atof(m_strAlignerBaseOffsetY);
			pos.m_dAngle = atof(m_strAlignerBaseOffsetD);		
		}

			
		product.m_pAlignerParam->SetAlignerBaseOffset(pos);

		int i=0; 
		i=0;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX);
			pos.m_dPosY = atof(m_strAlignerOffsetY);
			pos.m_dAngle = atof(m_strAlignerOffsetD);	
		}
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		i++;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX2)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY2)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD2)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX2);
			pos.m_dPosY = atof(m_strAlignerOffsetY2);
			pos.m_dAngle = atof(m_strAlignerOffsetD2);	
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		i++;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX3)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY3)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD3)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX3);
			pos.m_dPosY = atof(m_strAlignerOffsetY3);
			pos.m_dAngle = atof(m_strAlignerOffsetD3);	
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		i++;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX4)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY4)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD4)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX4);
			pos.m_dPosY = atof(m_strAlignerOffsetY4);
			pos.m_dAngle = atof(m_strAlignerOffsetD4);	
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		i++;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX5)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY5)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD5)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX5);
			pos.m_dPosY = atof(m_strAlignerOffsetY5);
			pos.m_dAngle = atof(m_strAlignerOffsetD5);	
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
		m_saAlignerOffsetX.SetAt(nCurSel, m_strAlignerOffsetX6);
		m_saAlignerOffsetY.SetAt(nCurSel, m_strAlignerOffsetY6);
		m_saAlignerOffsetD.SetAt(nCurSel,  m_strAlignerOffsetD6);

		for (i = 0; i < (ALIGNOFFSETNUM - 5) &&  i < m_saAlignerOffsetX.GetSize(); i++)
		{
			if (m_pParent->m_SysOptionConfig.m_bChange2um)
			{
				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i))/PRECISIONXYUNIT;
				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i))/PRECISIONXYUNIT;
				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i))/PRECISIONDUNIT;
			}
			else
			{
				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i));
				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i));
				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i));	
			}
			product.m_pAlignerParam->SetAlignerOffset(i+5, pos);
		}

		nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
		m_saAlignerBenchOffsetX.SetAt(nCurSel, m_strAlignerBenchOffsetX);
		m_saAlignerBenchOffsetY.SetAt(nCurSel, m_strAlignerBenchOffsetY);
		m_saAlignerBenchOffsetD.SetAt(nCurSel,  m_strAlignerBenchOffsetD);

		for (i = 0; i < ALIGNBENCHOFFSETNUM && i < m_saAlignerBenchOffsetX.GetSize(); i++)
		{
			pos.m_dPosX = atof(m_saAlignerBenchOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saAlignerBenchOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saAlignerBenchOffsetD.GetAt(i));
			product.m_pAlignerParam->SetAlignerBenchOffset(i, pos);
		}

		product.m_pAlignerParam->SetSearchDelayTime(m_nSearchDelayTime);
		product.m_pAlignerParam->SetObjectSearchDelayTime(m_nObjectSearchDelayTime);
		product.m_pAlignerParam->SetObjectSearchDelayTime2(m_nObjectSearchDelayTime2);
		product.m_pAlignerParam->SetEnableObjectSearchDelayTime2(m_bEnableObjectSearchDelayTime2);

		product.m_pAlignerParam->EnableEightShapeAlign(m_bCheckEightShapeAlign);
		product.m_pAlignerParam->SetEightShapeAlignMode(m_nEightShapeAlignMode);
		product.m_pAlignerParam->SetEightShapeAngle(m_dEightShapeAngle);

	    //product.m_pAlignerParam->EnableMulMarkMode(m_bDiffPosWithSameSearchResultEnabled);
		product.m_pAlignerParam->SetEnableAlignAngleVectical(m_bAlignAngleVertical);
		product.m_pAlignerParam->SetAlignAngleVecticalMode(m_nAlignAngleVerticalMode);
		
		BOOL bAlnTargetOffsetEnable;
		double dTargetOffsetX,dTargetOffsetY;

		bAlnTargetOffsetEnable = m_bCheckTargetOffsetEnable;
		dTargetOffsetX = atof(m_strAlnTargetOffsetX);
		dTargetOffsetY = atof(m_strAlnTargetOffsetY);

		product.m_pAlignerParam->EnableAlnTargetOffset(bAlnTargetOffsetEnable);
		product.m_pAlignerParam->SetAlnTargetOffsetX(dTargetOffsetX);
		product.m_pAlignerParam->SetAlnTargetOffsetY(dTargetOffsetY);
		product.m_pAlignerParam->EnableAlnSendTargetOffsetToPLC(m_bEnableSendTargetOffsetToPLC);

		//保存是否允许目标固定补偿以及目标固定补偿值	
		product.m_pAlignerParam->EnableAlnTargetFixOffset(m_bEnableTarFixOffset);
		product.m_pAlignerParam->SetTargetOffsetType(m_nTarFixOffsetType);
		for (i=0;i<ALIGNOFFSETNUM;i++)
		{
			pos.m_dPosX = atof(m_saTarFixOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saTarFixOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saTarFixOffsetD.GetAt(i));
			product.m_pAlignerParam->SetAlnTargetFixOffset(i,pos);
		}

		//保存是否反置对位平台移动量的XYD方向
		product.m_pAlignerParam->EnableAlnOffsetReverseX(m_bAlignOffsetReverseX);
		product.m_pAlignerParam->EnableAlnOffsetReverseY(m_bAlignOffsetReverseY);
		product.m_pAlignerParam->EnableAlnOffsetReverseD(m_bAlignOffsetReverseD);


		// 输入模式/检查模式
		CString strInfo = _T("");
		int nIndex = m_comboMultiCalibExtension.GetCurSel();

		strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
		m_saMult_CheckTargetAndObjectDis.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_nCheckMode);
		m_saMult_CheckMode.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bTargetDisThre);
		m_saMult_CheckTargetDisThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bObjectDisThre);
		m_saMult_CheckObjectDisThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bTargetAngleThre);
		m_saMult_CheckTargetAngleThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bObjectAngleThre);
		m_saMult_CheckObjectAngleThre.SetAt(nIndex, strInfo);


		m_saMult_TargetDisThre.SetAt(nIndex, m_strTargetDisThre);
		m_saMult_ObjectDisThre.SetAt(nIndex, m_strObjectDisThre);
		m_saMult_TargetAngleThre.SetAt(nIndex, m_strTargetAngleThre);
		m_saMult_ObjectAngleThre.SetAt(nIndex, m_strObjectAngleThre);

		
		int nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum + 1;
		CAlignCheckInfo checkInfo;
		for (i = 0; i < nNums_CheckInfo && m_saMult_CheckTargetAndObjectDis.GetSize(); i++)
		{
			checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(i);

			checkInfo.m_bCheckTargetAndObjectDis = atoi(m_saMult_CheckTargetAndObjectDis.GetAt(i));
			checkInfo.m_nCheckMode = atoi(m_saMult_CheckMode.GetAt(i));

			checkInfo.m_bCheckTargetDis = atoi(m_saMult_CheckTargetDisThre.GetAt(i));
			checkInfo.m_bCheckObjectDis = atoi(m_saMult_CheckObjectDisThre.GetAt(i));
			checkInfo.m_bCheckTargetAngle = atoi(m_saMult_CheckTargetAngleThre.GetAt(i));
			checkInfo.m_bCheckObjectAngle = atoi(m_saMult_CheckObjectAngleThre.GetAt(i));

			checkInfo.m_dTargetDisThreshold = atof(m_saMult_TargetDisThre.GetAt(i));
			checkInfo.m_dObjectDisThreshold = atof(m_saMult_ObjectDisThre.GetAt(i));
			checkInfo.m_dTargetAngleThreshold = atof(m_saMult_TargetAngleThre.GetAt(i));
			checkInfo.m_dObjectAngleThreshold = atof(m_saMult_ObjectAngleThre.GetAt(i));

			product.m_pAlignerParam->SetAlignCheckInfo(checkInfo,i);

		}


		//20160510增加对角补偿
		CProductSizeInfo productSizeInfo;
		productSizeInfo = product.m_pAlignerParam->GetProductSizeInfo();

		productSizeInfo.m_dObjectL1 = m_dObjectL1;
		productSizeInfo.m_dObjectW1 = m_dObjectW1;
		productSizeInfo.m_dTargetL2 = m_dTargetL2;
		productSizeInfo.m_dTargetW2 = m_dTargetW2;
		productSizeInfo.m_dF1 = m_dF1;
		productSizeInfo.m_dF2 = m_dF2;
		productSizeInfo.m_bOffsetXYEnable = m_bOffsetXYEnable;

		product.m_pAlignerParam->SetProductSizeInfo(productSizeInfo);

		//20160510增加对角补偿
		product.m_pAlignerParam->SetPositionModeInfo(m_nPosMode);

		CProductPositionVirtualInfo productPositionVirtualInfo;
		productPositionVirtualInfo = product.m_pAlignerParam->GetProductPositionVirtualInfo();

		productPositionVirtualInfo.m_bEnableObjectPositionVirtualMode = m_bEnableObjectPositionVirtualMode;
		productPositionVirtualInfo.m_bEnableTargetPositionVirtualMode = m_bEnableTargetPositionVirtualMode;
		productPositionVirtualInfo.m_nObjectPositionVirtualMode = m_nObjectPositionVirtualMode;
		productPositionVirtualInfo.m_nTargetPositionVirtualMode = m_nTargetPositionVirtualMode;

		product.m_pAlignerParam->SetProductPositionVirtualInfo(productPositionVirtualInfo);

		// 补偿设置（补偿的方向、X和Y补偿值对调）
		CProductOffsetInfo productOffsetInfo;
		productOffsetInfo = product.m_pAlignerParam->GetProductOffsetInfo();

// 		productOffsetInfo.m_nProductOffsetXDirection = m_nProductOffsetXDirection;
// 		productOffsetInfo.m_nProductOffsetYDirection = m_nProductOffsetYDirection;
// 		productOffsetInfo.m_nProductOffsetDDirection = m_nProductOffsetDDirection;
// 
// 		productOffsetInfo.m_nProductOffsetXSetting = m_nProductOffsetXSetting;
// 		productOffsetInfo.m_nProductOffsetYSetting = m_nProductOffsetYSetting;
// 		productOffsetInfo.m_nProductOffsetDSetting = m_nProductOffsetDSetting;

		productOffsetInfo.m_bEnableProductOffsetXDirection = m_bEnableProductOffsetXDirection;
		productOffsetInfo.m_bEnableProductOffsetYDirection = m_bEnableProductOffsetYDirection;
		productOffsetInfo.m_bEnableProductOffsetDDirection = m_bEnableProductOffsetDDirection;

		productOffsetInfo.m_bEnableProductOffsetXSetting = m_bEnableProductOffsetXSetting;
		productOffsetInfo.m_bEnableProductOffsetYSetting = m_bEnableProductOffsetYSetting;
		productOffsetInfo.m_bEnableProductOffsetDSetting = m_bEnableProductOffsetDSetting;

		product.m_pAlignerParam->SetProductOffsetInfo(productOffsetInfo);

		product.m_pAlignerParam->SetAlignAccordingToPickPlatformEnable(m_bAlignAccordingToPickEnable);

		// 焊接专用参数设置
		CWeldInfo WeldInfo;
		WeldInfo = product.m_pAlignerParam->GetWeldInfo();

		// 0.是否启用焊接参数
		WeldInfo.m_bEnableWeld = m_bEnableWeld;
		// 1. 运动量系数
		WeldInfo.m_WeldMovementRatio = CCoordPos(atof(m_strWeldMovementRatioX), atof(m_strWeldMovementRatioY), atof(m_strWeldMovementRatioD));
		// 2. 补偿系数
		WeldInfo.m_WeldAmmendMovementRatio = CCoordPos(atof(m_strWeldAmmendMoveRatioX), atof(m_strWeldAmmendMoveRatioY), 0.0);
		// 3. 目标多次定位/目标一次定位/目标零次定位
		WeldInfo.m_nWeldTargetSearchTimes = m_nWeldTargetSearchTimes;
		// 4. 对象基准/目标基准/无基准
		WeldInfo.m_nWeldSearchBench = m_nWeldSearchBench;
		// 5. 角度以对象为基准/角度以目标为基准/角度无基准
		WeldInfo.m_nWeldAngleBench = m_nWeldAngleBench;
		product.m_pAlignerParam->SetWeldInfo(WeldInfo);


// 		// PCB参数设置
// 		CPCBInfo PCBInfo;
// 		PCBInfo = product.m_pAlignerParam->GetPCBInfo();
// 
// 		if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD)/PRECISIONDUNIT;
// 		}
// 		else
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX);
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY);
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD);
// 		}
// 		
// 		PCBInfo.m_nPCBInsPreType = m_nPCBInsPreType;
// 		PCBInfo.m_bPCBShowInspectResult = m_bPCBShowInspectResult;
// 		PCBInfo.m_bShowPCB4To3Align = m_bShowPCB4To3Align;
// 
// 		product.m_pAlignerParam->SetPCBInfo(PCBInfo);


		// 折弯参数
		CBendInfo BendInfo;
		BendInfo = product.m_pAlignerParam->GetBendInfo();
		BendInfo.m_bBendAlignInspect = m_bBendAlignInspect;

		BendInfo.m_bBendType = m_bBendType;
		BendInfo.m_dBendImageRadio = atof(m_strPixelSize);
		BendInfo.m_bBendAlignDmCode = m_bBendAlignDmCode;

		BendInfo.m_bBendDisOffset = m_bBendDisOffset;
		BendInfo.m_dBendTargetDisOffset = atof(m_strTargetDisOffset);
		BendInfo.m_dBendObjectDisOffset = atof(m_strObjectDisOffset);

		BendInfo.m_bBendResultOffset = m_bBendResultOffset;
		BendInfo.m_scBendLeftDisOffset.SetX(atof(m_strLeftOffsetX));
		BendInfo.m_scBendLeftDisOffset.SetY(atof(m_strLeftOffsetY));
		BendInfo.m_scBendRightDisOffset.SetX(atof(m_strRightOffsetX));
		BendInfo.m_scBendRightDisOffset.SetY(atof(m_strRightOffsetY));
		BendInfo.m_dLeftDisOffset = atof(m_strLeftDisOffset);
		BendInfo.m_dRightDisOffset = atof(m_strRightDisOffset);

		BendInfo.m_bEnableFbsLRY = m_bEnableFbsLRY;
		BendInfo.m_bBendThreOffset = m_bBendThreOffset;
		BendInfo.m_scBendStanderThre.SetX(atof(m_strThreOffsetX));
		BendInfo.m_scBendStanderThre.SetY(atof(m_strThreOffsetY));

		BendInfo.m_bBendPressPara = m_bBendPressPara;
		BendInfo.m_bBendPressParaWrite = m_bBendPressParaWrite;
		BendInfo.m_lBendPressParaWriteAddre = m_lBendPressParaWriteAddre;
		BendInfo.m_dBendPressOKYmin = atof(m_strPressOKMinY);
		BendInfo.m_dBendPressOKYmax = atof(m_strPressOKMaxY);
		BendInfo.m_dBendPressNGYmin = atof(m_strPressNGMinY);
		BendInfo.m_dBendPressNGYmax = atof(m_strPressNGMaxY);

		BendInfo.m_bBendLoadThre = m_bBendLoadThre;
		BendInfo.m_scBendLoadThre.SetX(atof(m_strBendLoadThreX));
		BendInfo.m_scBendLoadThre.SetY(atof(m_strBendLoadThreY));

		BendInfo.m_bBendingInspect = m_bBendingInspect;
		BendInfo.m_bBendingProductID = m_bBendingProductID;
		BendInfo.m_bBendInspectDmCode = m_bBendInspectDmCode;
		BendInfo.m_bEnableReadWsInfo = m_bEnableReadWsInfo;

		BendInfo.m_bEnableReadInspectWsInfo = m_bEnableReadInspectWsInfo;
		BendInfo.m_bEnableReadStartTimeInfo = m_bEnableReadStartTimeInfo;
		BendInfo.m_bEnableReadEndTimeInfo = m_bEnableReadEndTimeInfo;
		BendInfo.m_bEnableReadProsessingTimeInfo = m_bEnableReadProsessingTimeInfo;

		BendInfo.m_bEnableUseMathCaluLRDis = m_bEnableUseMathCaluLRDis;

		BendInfo.m_bEnableMoveSnap = m_bEnableMoveSnap;
		BendInfo.m_nPosIndexFisrt = m_nPosIndexFisrt;

		BendInfo.m_bEnableAxisMoveLimit = m_bEnableAxisMoveLimit;
		BendInfo.m_dAxisAllowMoveMaxX = m_dAxisAllowMoveMaxX;
		BendInfo.m_dAxisAllowMoveMaxY = m_dAxisAllowMoveMaxY;
		BendInfo.m_dAxisAllowMoveMaxD = m_dAxisAllowMoveMaxD;


		BendInfo.m_bBendInspectSnCodeInvert=m_bBendInspectSnCodeInvert;

		BendInfo.m_bBendInspectSnCodeLength=m_bBendInspectSnCodeLength;
		BendInfo.m_bBendInspectGetSnOnce=m_bBendInspectGetSnOnce;

		BendInfo.m_strPlatformName1 = m_strPlatformName1;
		BendInfo.m_strPlatformName2 = m_strPlatformName2;
		BendInfo.m_bEnableFbisTarObc = m_bEnableFbisTarObc;

		product.m_pAlignerParam->SetBendInfo(BendInfo);

		product.m_pAlignerParam->SetLightCtrlInfo(m_cPhotoLightCtrl);

		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else
	{
		BeginWaitCursor();
		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
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
		pTotalProduct->SetCurProductIndex(nCurProdIdx);
		// 从总产品信息中获取当前产品信息，如果总产品数为0，则增加一个空产品
		if (nCurProdIdx < 0)
		{
			SysPlatformInfo platformInfo;
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
			product = CProductData(platformInfo);
			CString strNewName = GetNewDefName();
			pTotalProduct->AddItem(product);
			pTotalProduct->SetItemName(0, strNewName);
			pTotalProduct->SetCurProductIndex(0);
			pTotalProduct->SetProductIndex(0,0,0);
			nCurProdIdx = pTotalProduct->GetCurProductIndex();

			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(nCurProdIdx);

			m_bProdNewCreate = TRUE;
			m_strProductNameOld = strNewName;
		}

		product = pTotalProduct->GetItem(nCurProdIdx);


		if (m_bShowOffsetTips)
		{
			int i;
			for(i =0 ;i<ALIGNOFFSETNUM;i++)
			{
				m_vstrOffSetTip[i] = product.m_pAlignerParam->GetAlignerOffsetTip(i);
			}
			for (i=0;i<5;i++)
			{
				GetDlgItem(IDC_STATIC_TIP1+i)->ShowWindow(m_bShowOffset);
				GetDlgItem(IDC_STATIC_TIP1+i)->SetWindowText(m_vstrOffSetTip[i]);
			}
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(m_vstrOffSetTip[m_comboAlignerOffsetIndex.GetCurSel()+5]);

		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP3)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP4)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP5)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(FALSE);
		}


		//m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);
		int nProductIndexFa, nProductIndexChd;
		pTotalProduct->GetDlgProductIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);
		m_strCurProductName.Format(_T("%03d-%03d|%s"), nProductIndexFa+1, nProductIndexChd+1, pTotalProduct->GetItemName(nCurProdIdx));

		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
		m_strCurFaCalibName.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalCalibData->GetCalibNameFa(nProductIndexFa));
		m_strCurCalibName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalCalibData->GetItemName(nCurProdIdx));

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;
		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();

		//m_bEnableMutiTarget=product.m_pAlignerParam->GetEnableMutiTarget();
		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerPrecisionX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerPrecisionY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerPrecisionD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowTextA(_T("Precision X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowTextA(_T("Precision Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowTextA(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
               
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowTextA(_T("设定精度X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowTextA(_T("设定精度Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowTextA(_T("设定精度θ(°)"));
                break;
            }    
            GetDlgItem(IDC_STATIC_X_DW)->SetWindowTextA(_T("um"));
            GetDlgItem(IDC_STATIC_Y_DW)->SetWindowTextA(_T("um"));
            GetDlgItem(IDC_STATIC_D_DW)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowTextA(_T("Precision X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowTextA(_T("Precision Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowTextA(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowTextA(_T("设定精度X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowTextA(_T("设定精度Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowTextA(_T("设定精度θ(°)"));
                break;
            }

            GetDlgItem(IDC_STATIC_X_DW)->SetWindowTextA(_T("mm"));
            GetDlgItem(IDC_STATIC_Y_DW)->SetWindowTextA(_T("mm"));
            GetDlgItem(IDC_STATIC_D_DW)->SetWindowTextA(_T("°"));
		}
		
		if (m_pParent != NULL)
		{
			SysPlatformInfo platformInfo;
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
			
			m_comboAligner2Vs2Type.ResetContent();
			m_comboAligner3Vs3Type.ResetContent();
			m_comboAligner4Vs4Type.ResetContent();
			
			//2vs2
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
			int nComboNum = 0;
			iter2Vs2 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.begin();		
			for (;iter2Vs2 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.end();iter2Vs2++)  
			{
				if (iter2Vs2->m_bOpenAligner)
				{
					m_comboAligner2Vs2Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
					m_comboAligner2Vs2Type.SetItemData(nComboNum,iter2Vs2->m_alignerType);
					nComboNum ++;
				}
			}
			//3vs3
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
			nComboNum = 0;
			iter3Vs3 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.begin();		
			for (;iter3Vs3 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.end();iter3Vs3++)  
			{
				if (iter3Vs3->m_bOpenAligner)
				{
					m_comboAligner3Vs3Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
					m_comboAligner3Vs3Type.SetItemData(nComboNum,iter3Vs3->m_alignerType);
					nComboNum ++;
				}
			}
			//4vs4
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
			nComboNum = 0;
			iter4Vs4 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.begin();		
			for (;iter4Vs4 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.end();iter4Vs4++)  
			{
				if (iter4Vs4->m_bOpenAligner)
				{
					m_comboAligner4Vs4Type.InsertString(nComboNum,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
					m_comboAligner4Vs4Type.SetItemData(nComboNum,iter4Vs4->m_alignerType);
					nComboNum ++;
				}
			}
			
			//2vs2
			iter2Vs2 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.begin();		
			for (;iter2Vs2 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.end();iter2Vs2++)  
			{
				if (iter2Vs2->m_alignerType == product.m_pAlignerParam->GetAligner2Vs2Type())
				{
					if (iter2Vs2->m_bOpenAligner)
					{
						//m_comboAligner2Vs2Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
						int nFInd = m_comboAligner2Vs2Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
						if (nFInd != CB_ERR)
						{
							m_comboAligner2Vs2Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
						}
						break;
					}
					else
					{
						iter2Vs2 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.begin();
						for (;iter2Vs2 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.end();iter2Vs2++)  
						{
							if (iter2Vs2->m_bOpenAligner)
							{
								//m_comboAligner2Vs2Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
								int nFInd = m_comboAligner2Vs2Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
								if (nFInd != CB_ERR)
								{
									m_comboAligner2Vs2Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
								}
								product.m_pAlignerParam->SetAligner2Vs2Type(iter2Vs2->m_alignerType);
								pTotalProduct->SetItem(nCurProdIdx, product);
								break;
							}
						}
						break;
					}
				}
			}
			//3vs3
			iter3Vs3 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.begin();		
			for (;iter3Vs3 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.end();iter3Vs3++)  
			{
				if (iter3Vs3->m_alignerType == product.m_pAlignerParam->GetAligner3Vs3Type())
				{
					if (iter3Vs3->m_bOpenAligner)
					{
						//m_comboAligner3Vs3Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
						int nFInd = m_comboAligner3Vs3Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
						if (nFInd != CB_ERR)
						{
							m_comboAligner3Vs3Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
						}
						break;
					}
					else
					{
						iter3Vs3 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.begin();		
						for (;iter3Vs3 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.end();iter3Vs3++)  
						{
							if (iter3Vs3->m_bOpenAligner)
							{
								//m_comboAligner3Vs3Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
								int nFInd = m_comboAligner3Vs3Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
								if (nFInd != CB_ERR)
								{
									m_comboAligner3Vs3Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
								}
								product.m_pAlignerParam->SetAligner3Vs3Type(iter3Vs3->m_alignerType);
								break;
							}
						}
						break;
					}

				}
			}
			//4vs4
			iter4Vs4 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.begin();		
			for (;iter4Vs4 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.end();iter4Vs4++)  
			{
				if (iter4Vs4->m_alignerType == product.m_pAlignerParam->GetAligner4Vs4Type())
				{
					if (iter4Vs4->m_bOpenAligner)
					{
						//m_comboAligner4Vs4Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
						int nFInd = m_comboAligner4Vs4Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
						if (nFInd != CB_ERR)
						{
							m_comboAligner4Vs4Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
						}
						break;
					}
					else
					{
						iter4Vs4 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.begin();		
						for (;iter4Vs4 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.end();iter4Vs4++)  
						{
							if (iter4Vs4->m_bOpenAligner)
							{
								//m_comboAligner4Vs4Type.SelectString(-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
								int nFInd = m_comboAligner4Vs4Type.FindStringExact(-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
								if (nFInd != CB_ERR)
								{
									m_comboAligner4Vs4Type.SelectString(nFInd-1,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
								}
								product.m_pAlignerParam->SetAligner4Vs4Type(iter4Vs4->m_alignerType);
								break;
							}
						}
						break;
					}

				}
			}
		}

		//m_comboAligner2Vs2Type.SetCurSel((int)product.m_pAlignerParam->GetAligner2Vs2Type());
		//m_comboAligner3Vs3Type.SetCurSel((int)product.m_pAlignerParam->GetAligner3Vs3Type());
		//m_comboAligner4Vs4Type.SetCurSel((int)product.m_pAlignerParam->GetAligner4Vs4Type());
		m_comboAlignerCirclePos.SetCurSel(product.m_pAlignerParam->GetAlignerCirclePos());
		m_comboAlignerMoveType.SetCurSel((int)product.m_pAlignerParam->GetAlignerMoveType());

		CString strSelText4 = _T("");
		CString strSelText2 = _T("");  //jjh0601
		CString strSelText3 = _T("");  //jjh0611
		m_comboAligner2Vs2Type.GetLBText(m_comboAligner2Vs2Type.GetCurSel(), strSelText2);
		m_comboAligner4Vs4Type.GetLBText(m_comboAligner4Vs4Type.GetCurSel(), strSelText4);
		m_comboAligner3Vs3Type.GetLBText(m_comboAligner3Vs3Type.GetCurSel(), strSelText3); //jjh0611
		//if ( (m_comboAligner4Vs4Type.GetCurSel() == 9) || (m_comboAligner4Vs4Type.GetCurSel() == 10) )
		if ( (strSelText4 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_1234_WEIGHT)) 
			|| (strSelText4 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_WEIGHT))
			|| (strSelText2 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO_WEIGHT))
			|| (strSelText2 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1_WEIGHT))
			|| (strSelText2 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2_WEIGHT))
			|| (strSelText3 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_WEIGHT))  //jjh0611
			|| (strSelText3 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1_WEIGHT))
			|| (strSelText3 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2_WEIGHT))
			|| (strSelText3 == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_L1C_WEIGHT)))  //jjh0619
		{
			m_bShowAlignerCirclePos = true;
			GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet && m_bShowAlignerCirclePos);
			GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet && m_bShowAlignerCirclePos);

			GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(m_bShowAlignTypeSet  && m_bShowAlignerCirclePos);
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(m_bShowAlignTypeSet  && m_bShowAlignerCirclePos);
		}
		else
		{
			m_bShowAlignerCirclePos = false;
			GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(SW_HIDE);
		}

		double dWeight = product.m_pAlignerParam->GetAlignerWeight();
		m_strAlignerWeight.Format(_T("%.6f"), dWeight);

		int nPos = product.m_pAlignerParam->GetAlignerCirclePos();
		m_comboAlignerCirclePos.SetCurSel(nPos);

		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
		if ( (m_comboAlignerOffsetCoordType.GetCurSel() == 3) || (m_comboAlignerOffsetCoordType.GetCurSel() == 4) )	//20160510增加对角补偿
		{
			GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(m_bEnableOffset);
			//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_SHOW);
			//GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_SHOW);
		}
		else
		{
			SysPlatformInfo platformInfo;
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
			if (platformOptionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
			{
				GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(m_bEnableOffset);
				//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_SHOW);
				//GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(FALSE);
				//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_HIDE);
				//GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_HIDE);
			}

// 			GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_HIDE);
		}

		pos = product.m_pAlignerParam->GetAlignerBaseOffset();
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerBaseOffsetX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerBaseOffsetY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerBaseOffsetD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			GetDlgItem(IDC_STATIC_OFFSET_X_BASE_DW)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_BASE_DW)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_BASE_DW)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerBaseOffsetX.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerBaseOffsetY.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerBaseOffsetD.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_BASE_DW)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_BASE_DW)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_BASE_DW)->SetWindowTextA(_T("°"));
		}
		

		int i = 0;
		pos = product.m_pAlignerParam->GetAlignerOffset(i);

		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerOffsetX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerOffsetY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerOffsetD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
 			GetDlgItem(IDC_STATIC_OFFSET_X_DW)->SetWindowTextA(_T("um"));
 			GetDlgItem(IDC_STATIC_OFFSET_Y_DW)->SetWindowTextA(_T("um"));
 			GetDlgItem(IDC_STATIC_OFFSET_D_DW)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_DW)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW)->SetWindowTextA(_T("°"));
		}

		i++;
		pos = product.m_pAlignerParam->GetAlignerOffset(i);
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerOffsetX2.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerOffsetY2.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerOffsetD2.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			GetDlgItem(IDC_STATIC_OFFSET_X_DW2)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW2)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW2)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerOffsetX2.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerOffsetY2.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerOffsetD2.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_DW2)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW2)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW2)->SetWindowTextA(_T("°"));
		}
		

		i++;
		pos = product.m_pAlignerParam->GetAlignerOffset(i);
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerOffsetX3.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerOffsetY3.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerOffsetD3.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			GetDlgItem(IDC_STATIC_OFFSET_X_DW3)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW3)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW3)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerOffsetX3.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerOffsetY3.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerOffsetD3.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_DW3)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW3)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW3)->SetWindowTextA(_T("°"));
		}

		i++;
		pos = product.m_pAlignerParam->GetAlignerOffset(i);
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerOffsetX4.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerOffsetY4.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerOffsetD4.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			GetDlgItem(IDC_STATIC_OFFSET_X_DW4)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW4)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW4)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerOffsetX4.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerOffsetY4.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerOffsetD4.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_DW4)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW4)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW4)->SetWindowTextA(_T("°"));
		}

		i++;
		pos = product.m_pAlignerParam->GetAlignerOffset(i);
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerOffsetX5.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerOffsetY5.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerOffsetD5.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			GetDlgItem(IDC_STATIC_OFFSET_X_DW5)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW5)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW5)->SetWindowTextA(_T("°"));
			GetDlgItem(IDC_STATIC_OFFSET_X_DW6)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW6)->SetWindowTextA(_T("um"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW6)->SetWindowTextA(_T("°"));
		}
		else
		{
			m_strAlignerOffsetX5.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerOffsetY5.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerOffsetD5.Format(_T("%.6f"), pos.GetAngle());
			GetDlgItem(IDC_STATIC_OFFSET_X_DW5)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW5)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW5)->SetWindowTextA(_T("°"));
			GetDlgItem(IDC_STATIC_OFFSET_X_DW6)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_Y_DW6)->SetWindowTextA(_T("mm"));
			GetDlgItem(IDC_STATIC_OFFSET_D_DW6)->SetWindowTextA(_T("°"));
		}

		CString strPosX,strPosY,strPosD;
		m_saAlignerOffsetX.SetSize((ALIGNOFFSETNUM - 5));
		m_saAlignerOffsetY.SetSize((ALIGNOFFSETNUM - 5));
		m_saAlignerOffsetD.SetSize((ALIGNOFFSETNUM - 5));
		for (i = 0; i < (ALIGNOFFSETNUM - 5); i++)
		{
			pos = product.m_pAlignerParam->GetAlignerOffset(i+5);

			if(m_pParent->m_SysOptionConfig.m_bChange2um)
			{
				strPosX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
				strPosY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
				strPosD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);
			}
			else
			{
				strPosX.Format(_T("%.6f"), pos.GetPosX());
				strPosY.Format(_T("%.6f"), pos.GetPosY());
				strPosD.Format(_T("%.6f"), pos.GetAngle());
			}

			m_saAlignerOffsetX.SetAt(i,strPosX);
			m_saAlignerOffsetY.SetAt(i,strPosY);
			m_saAlignerOffsetD.SetAt(i,strPosD);			
		}
		m_strAlignerOffsetX6 = m_saAlignerOffsetX.GetAt(m_comboAlignerOffsetIndex.GetCurSel());
		m_strAlignerOffsetY6 = m_saAlignerOffsetY.GetAt(m_comboAlignerOffsetIndex.GetCurSel());
		m_strAlignerOffsetD6 = m_saAlignerOffsetD.GetAt(m_comboAlignerOffsetIndex.GetCurSel());


		m_saAlignerBenchOffsetX.SetSize(ALIGNBENCHOFFSETNUM);
		m_saAlignerBenchOffsetY.SetSize(ALIGNBENCHOFFSETNUM);
		m_saAlignerBenchOffsetD.SetSize(ALIGNBENCHOFFSETNUM);
		for (i = 0; i < ALIGNBENCHOFFSETNUM; i++)
		{
			pos = product.m_pAlignerParam->GetAlignerBenchOffset(i);
			strPosX.Format(_T("%.6f"), pos.GetPosX());
			strPosY.Format(_T("%.6f"), pos.GetPosY());
			strPosD.Format(_T("%.6f"), pos.GetAngle());
			m_saAlignerBenchOffsetX.SetAt(i,strPosX);
			m_saAlignerBenchOffsetY.SetAt(i,strPosY);
			m_saAlignerBenchOffsetD.SetAt(i,strPosD);			
		}
		m_strAlignerBenchOffsetX = m_saAlignerBenchOffsetX.GetAt(m_comboAlignerBenchOffsetIndex.GetCurSel());
		m_strAlignerBenchOffsetY = m_saAlignerBenchOffsetY.GetAt(m_comboAlignerBenchOffsetIndex.GetCurSel());
		m_strAlignerBenchOffsetD = m_saAlignerBenchOffsetD.GetAt(m_comboAlignerBenchOffsetIndex.GetCurSel());

		m_nSearchDelayTime = product.m_pAlignerParam->GetSearchDelayTime();
		m_nObjectSearchDelayTime = product.m_pAlignerParam->GetObjectSearchDelayTime();
		m_nObjectSearchDelayTime2 = product.m_pAlignerParam->GetObjectSearchDelayTime2();
		m_bEnableObjectSearchDelayTime2 = product.m_pAlignerParam->GetEnableObjectSearchDelayTime2();

		m_bCheckEightShapeAlign = product.m_pAlignerParam->GetEnableEightShapeAlign();
		m_nEightShapeAlignMode  = product.m_pAlignerParam->GetEightShapeAlignMode();
		m_nAlignAngleVerticalMode = product.m_pAlignerParam->GetAlignAngleVecticalMode();
		m_dEightShapeAngle      = product.m_pAlignerParam->GetEightShapeAngle();

		//m_bDiffPosWithSameSearchResultEnabled = product.m_pAlignerParam->GetMulMarkMode();
		m_bAlignAngleVertical = product.m_pAlignerParam->GetEnableAlignAngleVectical();

		double dTargetOffsetX,dTargetOffsetY;

		m_bCheckTargetOffsetEnable = product.m_pAlignerParam->GetEnableAlnTargetOffset();
		dTargetOffsetX = product.m_pAlignerParam->GetAlnTargetOffsetX();
		dTargetOffsetY = product.m_pAlignerParam->GetAlnTargetOffsetY() ;
		m_bEnableSendTargetOffsetToPLC = product.m_pAlignerParam->GetEnableAlnSendTargetOffsetToPLC();

		m_strAlnTargetOffsetX.Format(_T("%.6f"),dTargetOffsetX);
		m_strAlnTargetOffsetY.Format(_T("%.6f"),dTargetOffsetY);		

		m_bEnableTarFixOffset = product.m_pAlignerParam->GetEnableAlnTargetFixOffset();
		m_nTarFixOffsetType = product.m_pAlignerParam->GetTargetOffsetType();
		m_saTarFixOffsetX.SetSize(ALIGNOFFSETNUM);
		m_saTarFixOffsetY.SetSize(ALIGNOFFSETNUM);
		m_saTarFixOffsetD.SetSize(ALIGNOFFSETNUM);
		for (i=0;i<ALIGNOFFSETNUM;i++)
		{
			pos = product.m_pAlignerParam->GetAlnTargetFixOffset(i);
			strPosX.Format(_T("%.6f"), pos.GetPosX());
			strPosY.Format(_T("%.6f"), pos.GetPosY());
			strPosD.Format(_T("%.6f"), pos.GetAngle());
			m_saTarFixOffsetX.SetAt(i,strPosX);
			m_saTarFixOffsetY.SetAt(i,strPosY);
			m_saTarFixOffsetD.SetAt(i,strPosD);
		}

		m_bAlignOffsetReverseX = product.m_pAlignerParam->GetEnableAlnOffsetReverseX();
		m_bAlignOffsetReverseY = product.m_pAlignerParam->GetEnableAlnOffsetReverseY();
		m_bAlignOffsetReverseD = product.m_pAlignerParam->GetEnableAlnOffsetReverseD();


		// 输入模式/检查模式
		int nIndex = m_nMultiCalibExtensionIndex;
		int nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum +1;

		m_saMult_CheckTargetAndObjectDis.SetSize(nNums_CheckInfo);
		m_saMult_CheckMode.SetSize(nNums_CheckInfo);

		m_saMult_CheckTargetDisThre.SetSize(nNums_CheckInfo);
		m_saMult_CheckObjectDisThre.SetSize(nNums_CheckInfo);
		m_saMult_CheckTargetAngleThre.SetSize(nNums_CheckInfo);
		m_saMult_CheckObjectAngleThre.SetSize(nNums_CheckInfo);

		m_saMult_TargetDisThre.SetSize(nNums_CheckInfo);
		m_saMult_ObjectDisThre.SetSize(nNums_CheckInfo);
		m_saMult_TargetAngleThre.SetSize(nNums_CheckInfo);
		m_saMult_ObjectAngleThre.SetSize(nNums_CheckInfo);


		CAlignCheckInfo checkInfo;
		for (int it =0; it < nNums_CheckInfo;it++)
		{
			CString strInfo = _T("");
			checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(it);

			strInfo.Format(_T("%d"),checkInfo.m_bCheckTargetAndObjectDis);
			m_saMult_CheckTargetAndObjectDis.SetAt(it, strInfo);

			strInfo.Format(_T("%d"),checkInfo.m_nCheckMode);
			m_saMult_CheckMode.SetAt(it, strInfo);



			strInfo.Format(_T("%d"),checkInfo.m_bCheckTargetDis);
			m_saMult_CheckTargetDisThre.SetAt(it, strInfo);

			strInfo.Format(_T("%d"),checkInfo.m_bCheckObjectDis);
			m_saMult_CheckObjectDisThre.SetAt(it, strInfo);

			strInfo.Format(_T("%d"),checkInfo.m_bCheckTargetAngle);
			m_saMult_CheckTargetAngleThre.SetAt(it, strInfo);

			strInfo.Format(_T("%d"),checkInfo.m_bCheckObjectAngle);
			m_saMult_CheckObjectAngleThre.SetAt(it, strInfo);



			strInfo.Format(_T("%.6f"),checkInfo.m_dTargetDisThreshold);
			m_saMult_TargetDisThre.SetAt(it,strInfo);

			strInfo.Format(_T("%.6f"),checkInfo.m_dObjectDisThreshold);
			m_saMult_ObjectDisThre.SetAt(it,strInfo);

			strInfo.Format(_T("%.6f"),checkInfo.m_dTargetAngleThreshold);
			m_saMult_TargetAngleThre.SetAt(it,strInfo);

			strInfo.Format(_T("%.6f"),checkInfo.m_dObjectAngleThreshold);
			m_saMult_ObjectAngleThre.SetAt(it,strInfo);

		}
		checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(nIndex);

		if (m_pParent->m_SysOptionConfig.m_bCheckModeOnly)
		{
			checkInfo.m_nCheckMode = 1;
			checkInfo.m_bCheckTargetAndObjectDis = TRUE;
			checkInfo.m_bCheckTargetDis = TRUE;
			checkInfo.m_bCheckObjectDis = TRUE;
// 			checkInfo.m_bCheckTargetAngle = TRUE;
// 			checkInfo.m_bCheckObjectAngle = TRUE;

			for (int it = 0; it < nNums_CheckInfo && m_saMult_CheckTargetAndObjectDis.GetSize(); it++)
			{
				CString strInfo = _T("");
				strInfo.Format(_T("%d"),1);
				m_saMult_CheckTargetAndObjectDis.SetAt(it, strInfo);
				
				strInfo.Format(_T("%d"),1);
				m_saMult_CheckMode.SetAt(it, strInfo);

				strInfo.Format(_T("%d"),1);
				m_saMult_CheckTargetDisThre.SetAt(it, strInfo);

				strInfo.Format(_T("%d"),1);
				m_saMult_CheckObjectDisThre.SetAt(it, strInfo);
			}
		}

		m_bCheckTargetAndObjectDis = checkInfo.m_bCheckTargetAndObjectDis;
		m_nCheckMode = checkInfo.m_nCheckMode;
		
		m_bTargetDisThre = checkInfo.m_bCheckTargetDis;
		m_bObjectDisThre = checkInfo.m_bCheckObjectDis;
		m_bTargetAngleThre = checkInfo.m_bCheckTargetAngle;
		m_bObjectAngleThre = checkInfo.m_bCheckObjectAngle;


		m_strTargetDisThre.Format(_T("%.6f"), checkInfo.m_dTargetDisThreshold);
		m_strObjectDisThre.Format(_T("%.6f"), checkInfo.m_dObjectDisThreshold);
		m_strTargetAngleThre.Format(_T("%.6f"), checkInfo.m_dTargetAngleThreshold);
		m_strObjectAngleThre.Format(_T("%.6f"), checkInfo.m_dObjectAngleThreshold);
		SysPlatformInfo platformInfo;
		if (m_pParent)
		{
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		}
		BOOL bShowAngle = ((platformInfo.m_bTargetObjectCamSeparate && 8 == platformInfo.m_nCamNum && 8 == platformInfo.m_nPositionNum) || ( !platformInfo.m_bTargetObjectCamSeparate && 4 == platformInfo.m_nCamNum && 4 == platformInfo.m_nPositionNum) );
		
		GetDlgItem(IDC_STATIC_TARGET_ANGLE_THRE)->ShowWindow(m_bShowCheckMode && bShowAngle);
		GetDlgItem(IDC_EDIT_TARGET_ANGLE_THRE)->ShowWindow(m_bShowCheckMode  && bShowAngle);
		GetDlgItem(IDC_CHECK_TARGET_ANGLE_THRE)->ShowWindow(m_bShowCheckMode && bShowAngle);
		GetDlgItem(IDC_STATIC_OBJECT_ANGLE_THRE)->ShowWindow(m_bShowCheckMode && bShowAngle);
		GetDlgItem(IDC_EDIT_OBJECT_ANGLE_THRE)->ShowWindow(m_bShowCheckMode && bShowAngle);
		GetDlgItem(IDC_CHECK_OBJECT_ANGLE_THRE)->ShowWindow(m_bShowCheckMode && bShowAngle);
		

	

		// 20160510增加对角补偿
		CProductSizeInfo productSizeInfo;
		productSizeInfo = product.m_pAlignerParam->GetProductSizeInfo();

		m_dObjectL1 = productSizeInfo.m_dObjectL1;
		m_dObjectW1 = productSizeInfo.m_dObjectW1;
		m_dTargetL2 = productSizeInfo.m_dTargetL2;
		m_dTargetW2 = productSizeInfo.m_dTargetW2;
		m_dF1 = productSizeInfo.m_dF1;
		m_dF2 = productSizeInfo.m_dF2;
		m_bOffsetXYEnable = productSizeInfo.m_bOffsetXYEnable;

		CString strInfo;
		//strInfo.Format(_T("对象:L1=%.2lf;W1=%.2lf\r\n\r\n目标:L2=%.2lf;W2=%.2lf"),m_dObjectL1,m_dObjectW1,m_dTargetL2,m_dTargetW2);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->SetWindowText(strInfo);
		strInfo.Format(_T("%s:L1=%.2lf;W1=%.2lf"),GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PRODUCT_OBJECT_SIZE),m_dObjectL1,m_dObjectW1);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->SetWindowText(strInfo);
		strInfo.Format(_T("%s:L2=%.2lf;W2=%.2lf"),GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PRODUCT_TARGET_SIZE),m_dTargetL2,m_dTargetW2);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->SetWindowText(strInfo);

		//20160510增加对角补偿
		m_nPosMode = product.m_pAlignerParam->GetPositionModeInfo();

		CProductPositionVirtualInfo productPositionVirtualInfo;
		productPositionVirtualInfo = product.m_pAlignerParam->GetProductPositionVirtualInfo();

		m_bEnableObjectPositionVirtualMode = productPositionVirtualInfo.m_bEnableObjectPositionVirtualMode;
		m_bEnableTargetPositionVirtualMode = productPositionVirtualInfo.m_bEnableTargetPositionVirtualMode;
		m_nObjectPositionVirtualMode = productPositionVirtualInfo.m_nObjectPositionVirtualMode;
		m_nTargetPositionVirtualMode = productPositionVirtualInfo.m_nTargetPositionVirtualMode;

		// 补偿设置（补偿的方向、X和Y补偿值对调）
		CProductOffsetInfo productOffsetInfo;
		productOffsetInfo = product.m_pAlignerParam->GetProductOffsetInfo();

// 		m_nProductOffsetXDirection = productOffsetInfo.m_nProductOffsetXDirection;
// 		m_nProductOffsetYDirection = productOffsetInfo.m_nProductOffsetYDirection;
// 		m_nProductOffsetDDirection = productOffsetInfo.m_nProductOffsetDDirection;
// 
// 		m_nProductOffsetXSetting = productOffsetInfo.m_nProductOffsetXSetting;
// 		m_nProductOffsetYSetting = productOffsetInfo.m_nProductOffsetYSetting;
// 		m_nProductOffsetDSetting = productOffsetInfo.m_nProductOffsetDSetting;

		m_bEnableProductOffsetXDirection = productOffsetInfo.m_bEnableProductOffsetXDirection;
		m_bEnableProductOffsetYDirection = productOffsetInfo.m_bEnableProductOffsetYDirection;
		m_bEnableProductOffsetDDirection = productOffsetInfo.m_bEnableProductOffsetDDirection;

		m_bEnableProductOffsetXSetting = productOffsetInfo.m_bEnableProductOffsetXSetting;
		m_bEnableProductOffsetYSetting = productOffsetInfo.m_bEnableProductOffsetYSetting;
		m_bEnableProductOffsetDSetting = productOffsetInfo.m_bEnableProductOffsetDSetting;

		m_bAlignAccordingToPickEnable = product.m_pAlignerParam->GetAlignAccordingToPickPlatformEnable();

		// 专用焊接参数
		CWeldInfo WeldInfo;
		WeldInfo = product.m_pAlignerParam->GetWeldInfo();

		// 0.是否启用焊接参数
		m_bEnableWeld = WeldInfo.m_bEnableWeld;
		// 1. 运动量系数
		m_strWeldMovementRatioX.Format(_T("%.6f"), WeldInfo.m_WeldMovementRatio.m_dPosX);
		m_strWeldMovementRatioY.Format(_T("%.6f"), WeldInfo.m_WeldMovementRatio.m_dPosY);
		m_strWeldMovementRatioD.Format(_T("%.6f"), WeldInfo.m_WeldMovementRatio.m_dAngle);
		// 2. 补偿系数
		m_strWeldAmmendMoveRatioX.Format(_T("%.6f"), WeldInfo.m_WeldAmmendMovementRatio.m_dPosX);
		m_strWeldAmmendMoveRatioY.Format(_T("%.6f"), WeldInfo.m_WeldAmmendMovementRatio.m_dPosY);
		// 3. 目标多次定位/目标一次定位/目标零次定位
		m_nWeldTargetSearchTimes = WeldInfo.m_nWeldTargetSearchTimes;
		// 4. 对象基准/目标基准/无基准
		m_nWeldSearchBench = WeldInfo.m_nWeldSearchBench;
		// 5. 角度以对象为基准/角度以目标为基准/角度无基准
		m_nWeldAngleBench = WeldInfo.m_nWeldAngleBench;



// 		// PCB参数设置
// 		CPCBInfo PCBInfo;
// 		PCBInfo = product.m_pAlignerParam->GetPCBInfo();
// 
// 		if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			m_strPCBInsPreX.Format(strXY, PRECISIONXYUNIT*PCBInfo.m_PCBInsPre.m_dPosX);
// 			m_strPCBInsPreY.Format(strXY, PRECISIONXYUNIT*PCBInfo.m_PCBInsPre.m_dPosY);
// 			m_strPCBInsPreD.Format(strD, PRECISIONDUNIT*PCBInfo.m_PCBInsPre.m_dAngle);
// 		}
// 		else
// 		{
// 			m_strPCBInsPreX.Format(_T("%.6f"), PCBInfo.m_PCBInsPre.m_dPosX);
// 			m_strPCBInsPreY.Format(_T("%.6f"), PCBInfo.m_PCBInsPre.m_dPosY);
// 			m_strPCBInsPreD.Format(_T("%.6f"), PCBInfo.m_PCBInsPre.m_dAngle);
// 		}
// 		
// 		m_nPCBInsPreType = PCBInfo.m_nPCBInsPreType;
// 		m_bPCBShowInspectResult = PCBInfo.m_bPCBShowInspectResult;
// 		m_bShowPCB4To3Align = PCBInfo.m_bShowPCB4To3Align;


		// 折弯参数
		CBendInfo BendInfo;
		BendInfo = product.m_pAlignerParam->GetBendInfo();

		m_bBendAlignInspect = BendInfo.m_bBendAlignInspect;

		m_bBendType = BendInfo.m_bBendType;
		m_strPixelSize.Format(_T("%.6f"), BendInfo.m_dBendImageRadio);
		m_bBendAlignDmCode = BendInfo.m_bBendAlignDmCode;

		m_bBendDisOffset = BendInfo.m_bBendDisOffset;
		m_strTargetDisOffset.Format(_T("%.6f"), BendInfo.m_dBendTargetDisOffset);
		m_strObjectDisOffset.Format(_T("%.6f"), BendInfo.m_dBendObjectDisOffset);

		m_bBendResultOffset = BendInfo.m_bBendResultOffset;
		m_strLeftOffsetX.Format(_T("%.6f"), BendInfo.m_scBendLeftDisOffset.GetX());
		m_strLeftOffsetY.Format(_T("%.6f"), BendInfo.m_scBendLeftDisOffset.GetY());
		m_strRightOffsetX.Format(_T("%.6f"), BendInfo.m_scBendRightDisOffset.GetX());
		m_strRightOffsetY.Format(_T("%.6f"), BendInfo.m_scBendRightDisOffset.GetY());
		m_strLeftDisOffset.Format(_T("%.6f"), BendInfo.m_dLeftDisOffset);
		m_strRightDisOffset.Format(_T("%.6f"), BendInfo.m_dRightDisOffset);

		m_bEnableFbsLRY =  BendInfo.m_bEnableFbsLRY;
		m_bBendThreOffset = BendInfo.m_bBendThreOffset;
		m_strThreOffsetX.Format(_T("%.6f"), BendInfo.m_scBendStanderThre.GetX());
		m_strThreOffsetY.Format(_T("%.6f"), BendInfo.m_scBendStanderThre.GetY());


		m_bBendPressPara = BendInfo.m_bBendPressPara;
		m_bBendPressParaWrite = BendInfo.m_bBendPressParaWrite;
		m_lBendPressParaWriteAddre = BendInfo.m_lBendPressParaWriteAddre;
		m_strPressOKMinY.Format(_T("%.6f"), BendInfo.m_dBendPressOKYmin);
		m_strPressOKMaxY.Format(_T("%.6f"), BendInfo.m_dBendPressOKYmax);
		m_strPressNGMinY.Format(_T("%.6f"), BendInfo.m_dBendPressNGYmin);
		m_strPressNGMaxY.Format(_T("%.6f"), BendInfo.m_dBendPressNGYmax);

		m_bBendLoadThre = BendInfo.m_bBendLoadThre;
		m_strBendLoadThreX.Format(_T("%.6f"), BendInfo.m_scBendLoadThre.GetX());
		m_strBendLoadThreY.Format(_T("%.6f"), BendInfo.m_scBendLoadThre.GetY());

		m_bBendingInspect = BendInfo.m_bBendingInspect;
		m_bBendingProductID = BendInfo.m_bBendingProductID;
		m_bBendInspectDmCode = BendInfo.m_bBendInspectDmCode;
		m_bEnableReadWsInfo = BendInfo.m_bEnableReadWsInfo;

		m_bEnableReadInspectWsInfo = BendInfo.m_bEnableReadInspectWsInfo;
		m_bEnableReadStartTimeInfo = BendInfo.m_bEnableReadStartTimeInfo;
		m_bEnableReadEndTimeInfo = BendInfo.m_bEnableReadEndTimeInfo;
		m_bEnableReadProsessingTimeInfo = BendInfo.m_bEnableReadProsessingTimeInfo;

		m_bEnableUseMathCaluLRDis = BendInfo.m_bEnableUseMathCaluLRDis;

		m_bEnableMoveSnap = BendInfo.m_bEnableMoveSnap;
		m_nPosIndexFisrt = BendInfo.m_nPosIndexFisrt;

		m_bEnableAxisMoveLimit = BendInfo.m_bEnableAxisMoveLimit;
		m_dAxisAllowMoveMaxX = BendInfo.m_dAxisAllowMoveMaxX;
		m_dAxisAllowMoveMaxY = BendInfo.m_dAxisAllowMoveMaxY;
		m_dAxisAllowMoveMaxD = BendInfo.m_dAxisAllowMoveMaxD;

		m_bBendInspectSnCodeLength=BendInfo.m_bBendInspectSnCodeLength;
		m_bBendInspectSnCodeInvert=BendInfo.m_bBendInspectSnCodeInvert;
		m_bBendInspectGetSnOnce=BendInfo.m_bBendInspectGetSnOnce;

		m_strPlatformName1 = BendInfo.m_strPlatformName1;
		m_strPlatformName2 = BendInfo.m_strPlatformName2;
		m_bEnableFbisTarObc = BendInfo.m_bEnableFbisTarObc;

		m_cPhotoLightCtrl = product.m_pAlignerParam->GetLightCtrlInfo();

		LoadOffsetDirectionPicture();
		UpdateData(FALSE);
		EndWaitCursor();
	}
}


void CDlgSysKindAlignSetting::UpdateCalibDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	if (bSaveAndValidate)
	{	
	
	}
	else
	{
		BeginWaitCursor();
		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
		
		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
		int nProductIndexFa, nProductIndexChd;
		pTotalProduct->GetDlgProductIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);
		m_strCurFaCalibName.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalCalibData->GetCalibNameFa(nProductIndexFa));
		m_strCurCalibName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalCalibData->GetItemName(nCurProdIdx));

		UpdateData(FALSE);
		EndWaitCursor();
	}
}


void CDlgSysKindAlignSetting::GetDlgTempData(CProductData& product)
{	
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateData(TRUE);
	
	product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
	CCoordPos pos;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerPrecisionX);
		pos.m_dPosY = atof(m_strAlignerPrecisionY);
		pos.m_dAngle = atof(m_strAlignerPrecisionD);
	}
	
	product.m_pAlignerParam->SetAlignerPrecision(pos);

	product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel()));
	product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel()));
	product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel()));

	//product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetCurSel());
	//product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetCurSel());
	//product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetCurSel());
	product.m_pAlignerParam->SetAlignerMoveType((AlignerMoveType)m_comboAlignerMoveType.GetCurSel());

	product.m_pAlignerParam->SetAlignerWeight(atof(m_strAlignerWeight));
	product.m_pAlignerParam->SetAlignerCirclePos(m_comboAlignerCirclePos.GetCurSel());


	product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());

	if (m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerBaseOffsetX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerBaseOffsetY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerBaseOffsetD)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerBaseOffsetX);
		pos.m_dPosY = atof(m_strAlignerBaseOffsetY);
		pos.m_dAngle = atof(m_strAlignerBaseOffsetD);		
	}		
	product.m_pAlignerParam->SetAlignerBaseOffset(pos);

	int i=0; 
	i=0;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX);
		pos.m_dPosY = atof(m_strAlignerOffsetY);
		pos.m_dAngle = atof(m_strAlignerOffsetD);	
	}
	product.m_pAlignerParam->SetAlignerOffset(i, pos);

	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX2)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY2)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD2)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX2);
		pos.m_dPosY = atof(m_strAlignerOffsetY2);
		pos.m_dAngle = atof(m_strAlignerOffsetD2);	
	}	
	product.m_pAlignerParam->SetAlignerOffset(i, pos);

	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX3)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY3)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD3)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX3);
		pos.m_dPosY = atof(m_strAlignerOffsetY3);
		pos.m_dAngle = atof(m_strAlignerOffsetD3);	
	}
	product.m_pAlignerParam->SetAlignerOffset(i, pos);

	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX4)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY4)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD4)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX4);
		pos.m_dPosY = atof(m_strAlignerOffsetY4);
		pos.m_dAngle = atof(m_strAlignerOffsetD4);	
	}	
	product.m_pAlignerParam->SetAlignerOffset(i, pos);

	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX5)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY5)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD5)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX5);
		pos.m_dPosY = atof(m_strAlignerOffsetY5);
		pos.m_dAngle = atof(m_strAlignerOffsetD5);	
	}	
	product.m_pAlignerParam->SetAlignerOffset(i, pos);

	int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
	m_saAlignerOffsetX.SetAt(nCurSel, m_strAlignerOffsetX6);
	m_saAlignerOffsetY.SetAt(nCurSel, m_strAlignerOffsetY6);
	m_saAlignerOffsetD.SetAt(nCurSel,  m_strAlignerOffsetD6);

	for (i = 0; i < (ALIGNOFFSETNUM - 5) && i < m_saAlignerOffsetX.GetSize(); i++)
	{
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i))/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i))/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i))/PRECISIONDUNIT;
		}
		else
		{
			pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i));
		}
		
		product.m_pAlignerParam->SetAlignerOffset(i+5, pos);
	}

	nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
	m_saAlignerBenchOffsetX.SetAt(nCurSel, m_strAlignerBenchOffsetX);
	m_saAlignerBenchOffsetY.SetAt(nCurSel, m_strAlignerBenchOffsetY);
	m_saAlignerBenchOffsetD.SetAt(nCurSel,  m_strAlignerBenchOffsetD);

	for (i = 0; i < ALIGNBENCHOFFSETNUM && i < m_saAlignerBenchOffsetX.GetSize(); i++)
	{
		pos.m_dPosX = atof(m_saAlignerBenchOffsetX.GetAt(i));
		pos.m_dPosY = atof(m_saAlignerBenchOffsetY.GetAt(i));
		pos.m_dAngle = atof(m_saAlignerBenchOffsetD.GetAt(i));
		product.m_pAlignerParam->SetAlignerBenchOffset(i, pos);
	}

	product.m_pAlignerParam->SetSearchDelayTime(m_nSearchDelayTime);
	product.m_pAlignerParam->SetObjectSearchDelayTime(m_nObjectSearchDelayTime);
	product.m_pAlignerParam->SetObjectSearchDelayTime2(m_nObjectSearchDelayTime2);
	product.m_pAlignerParam->SetEnableObjectSearchDelayTime2(m_bEnableObjectSearchDelayTime2);

	product.m_pAlignerParam->EnableEightShapeAlign(m_bCheckEightShapeAlign);
	product.m_pAlignerParam->SetEightShapeAlignMode(m_nEightShapeAlignMode);
	product.m_pAlignerParam->SetEightShapeAngle(m_dEightShapeAngle);

	//product.m_pAlignerParam->EnableMulMarkMode(m_bDiffPosWithSameSearchResultEnabled);
	product.m_pAlignerParam->SetEnableAlignAngleVectical(m_bAlignAngleVertical);
	product.m_pAlignerParam->SetAlignAngleVecticalMode(m_nAlignAngleVerticalMode);

	BOOL bAlnTargetOffsetEnable;
	double dTargetOffsetX,dTargetOffsetY;

	bAlnTargetOffsetEnable = m_bCheckTargetOffsetEnable;
	dTargetOffsetX = atof(m_strAlnTargetOffsetX);
	dTargetOffsetY = atof(m_strAlnTargetOffsetY);

	product.m_pAlignerParam->EnableAlnTargetOffset(bAlnTargetOffsetEnable);
	product.m_pAlignerParam->SetAlnTargetOffsetX(dTargetOffsetX);
	product.m_pAlignerParam->SetAlnTargetOffsetY(dTargetOffsetY);
	product.m_pAlignerParam->EnableAlnSendTargetOffsetToPLC(m_bEnableSendTargetOffsetToPLC);

	product.m_pAlignerParam->EnableAlnTargetFixOffset(m_bEnableTarFixOffset);
	product.m_pAlignerParam->SetTargetOffsetType(m_nTarFixOffsetType);
	for (i=0;i<ALIGNOFFSETNUM;i++)
	{
		pos.m_dPosX = atof(m_saTarFixOffsetX.GetAt(i));
		pos.m_dPosY = atof(m_saTarFixOffsetY.GetAt(i));
		pos.m_dAngle = atof(m_saTarFixOffsetD.GetAt(i));
		product.m_pAlignerParam->SetAlnTargetFixOffset(i,pos);
	}

	product.m_pAlignerParam->EnableAlnOffsetReverseX(m_bAlignOffsetReverseX);
	product.m_pAlignerParam->EnableAlnOffsetReverseY(m_bAlignOffsetReverseY);
	product.m_pAlignerParam->EnableAlnOffsetReverseD(m_bAlignOffsetReverseD);


	// 输入模式、检查模式
	CString strInfo = _T("");
	int nIndex = m_comboMultiCalibExtension.GetCurSel();

	strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
	m_saMult_CheckTargetAndObjectDis.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_nCheckMode);
	m_saMult_CheckMode.SetAt(nIndex, strInfo);


	strInfo.Format(_T("%d"),m_bTargetDisThre);
	m_saMult_CheckTargetDisThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectDisThre);
	m_saMult_CheckObjectDisThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bTargetAngleThre);
	m_saMult_CheckTargetAngleThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectAngleThre);
	m_saMult_CheckObjectAngleThre.SetAt(nIndex, strInfo);


	m_saMult_TargetDisThre.SetAt(nIndex,m_strTargetDisThre);
	m_saMult_ObjectDisThre.SetAt(nIndex,m_strTargetDisThre);
	m_saMult_TargetAngleThre.SetAt(nIndex,m_strTargetDisThre);
	m_saMult_ObjectAngleThre.SetAt(nIndex,m_strTargetDisThre);


	int nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum + 1;
	CAlignCheckInfo checkInfo;
	for (i = 0; i < nNums_CheckInfo && m_saMult_CheckTargetAndObjectDis.GetSize(); i++)
	{
		checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(i);

		checkInfo.m_bCheckTargetAndObjectDis = atoi(m_saMult_CheckTargetAndObjectDis.GetAt(i));
		checkInfo.m_nCheckMode = atoi(m_saMult_CheckMode.GetAt(i));

		checkInfo.m_bCheckTargetDis = atoi(m_saMult_CheckMode.GetAt(i));
		checkInfo.m_bCheckObjectDis = atoi(m_saMult_CheckMode.GetAt(i));
		checkInfo.m_bCheckTargetAngle = atoi(m_saMult_CheckMode.GetAt(i));
		checkInfo.m_bCheckObjectAngle = atoi(m_saMult_CheckMode.GetAt(i));

		checkInfo.m_dTargetDisThreshold = atof(m_saMult_TargetDisThre.GetAt(i));
		checkInfo.m_dObjectDisThreshold = atof(m_saMult_ObjectDisThre.GetAt(i));
		checkInfo.m_dTargetAngleThreshold = atof(m_saMult_TargetAngleThre.GetAt(i));
		checkInfo.m_dObjectAngleThreshold = atof(m_saMult_ObjectAngleThre.GetAt(i));

		product.m_pAlignerParam->SetAlignCheckInfo(checkInfo,i);

	}

	// 专用焊接参数
	CWeldInfo WeldInfo;
	WeldInfo = product.m_pAlignerParam->GetWeldInfo();

	// 0.是否启用焊接参数
	WeldInfo.m_bEnableWeld = m_bEnableWeld;
	// 1. 运动量系数
	WeldInfo.m_WeldMovementRatio = CCoordPos(atof(m_strWeldMovementRatioX), atof(m_strWeldMovementRatioY),atof(m_strWeldMovementRatioD));
	// 2. 补偿系数
	WeldInfo.m_WeldAmmendMovementRatio = CCoordPos(atof(m_strWeldAmmendMoveRatioX), atof(m_strWeldAmmendMoveRatioY), 0.0);
	// 3. 目标多次定位/目标一次定位/目标零次定位
	WeldInfo.m_nWeldTargetSearchTimes = m_nWeldTargetSearchTimes;
	// 4. 对象基准/目标基准/无基准
	WeldInfo.m_nWeldSearchBench = m_nWeldSearchBench;
	// 5. 角度以对象为基准/角度以目标为基准/角度无基准
	WeldInfo.m_nWeldAngleBench = m_nWeldAngleBench;

	product.m_pAlignerParam->SetWeldInfo(WeldInfo);

// 	// PCB参数设置
// 	CPCBInfo PCBInfo;
// 	PCBInfo = product.m_pAlignerParam->GetPCBInfo();
// 
// 	if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 	{
// 		PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX)/PRECISIONXYUNIT;
// 		PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY)/PRECISIONXYUNIT;
// 		PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD)/PRECISIONDUNIT;
// 	}
// 	else
// 	{
// 		PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX);
// 		PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY);
// 		PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD);
// 	}
// 
// 	PCBInfo.m_nPCBInsPreType = m_nPCBInsPreType;
// 	PCBInfo.m_bPCBShowInspectResult = m_bPCBShowInspectResult;
// 	PCBInfo.m_bShowPCB4To3Align = m_bShowPCB4To3Align;
// 
// 	product.m_pAlignerParam->SetPCBInfo(PCBInfo);


	// 折弯参数
	CBendInfo BendInfo;
	BendInfo = product.m_pAlignerParam->GetBendInfo();
	BendInfo.m_bBendAlignInspect = m_bBendAlignInspect;

	BendInfo.m_bBendType = m_bBendType;
	BendInfo.m_dBendImageRadio = atof(m_strPixelSize);
	BendInfo.m_bBendAlignDmCode = m_bBendAlignDmCode;

	BendInfo.m_bBendDisOffset = m_bBendDisOffset;
	BendInfo.m_dBendTargetDisOffset = atof(m_strTargetDisOffset);
	BendInfo.m_dBendObjectDisOffset = atof(m_strObjectDisOffset);

	BendInfo.m_bBendResultOffset = m_bBendResultOffset;
	BendInfo.m_scBendLeftDisOffset.SetX(atof(m_strLeftOffsetX));
	BendInfo.m_scBendLeftDisOffset.SetY(atof(m_strLeftOffsetY));
	BendInfo.m_scBendRightDisOffset.SetX(atof(m_strRightOffsetX));
	BendInfo.m_scBendRightDisOffset.SetY(atof(m_strRightOffsetY));
	BendInfo.m_dLeftDisOffset = atof(m_strLeftDisOffset);
	BendInfo.m_dRightDisOffset = atof(m_strRightDisOffset);

	BendInfo.m_bEnableFbsLRY = m_bEnableFbsLRY;
	BendInfo.m_bBendThreOffset = m_bBendThreOffset;
	BendInfo.m_scBendStanderThre.SetX(atof(m_strThreOffsetX));
	BendInfo.m_scBendStanderThre.SetY(atof(m_strThreOffsetY));

	BendInfo.m_bBendPressPara = m_bBendPressPara;
	BendInfo.m_bBendPressParaWrite = m_bBendPressParaWrite;
	BendInfo.m_lBendPressParaWriteAddre = m_lBendPressParaWriteAddre;
	BendInfo.m_dBendPressOKYmin = atof(m_strPressOKMinY);
	BendInfo.m_dBendPressOKYmax = atof(m_strPressOKMaxY);
	BendInfo.m_dBendPressNGYmin = atof(m_strPressNGMinY);
	BendInfo.m_dBendPressNGYmax = atof(m_strPressNGMaxY);

	BendInfo.m_bBendLoadThre = m_bBendLoadThre;
	BendInfo.m_scBendLoadThre.SetX(atof(m_strBendLoadThreX));
	BendInfo.m_scBendLoadThre.SetY(atof(m_strBendLoadThreY));

	BendInfo.m_bBendingInspect = m_bBendingInspect;
	BendInfo.m_bBendingProductID = m_bBendingProductID;
	BendInfo.m_bBendInspectDmCode = m_bBendInspectDmCode;
	BendInfo.m_bEnableReadWsInfo = m_bEnableReadWsInfo;

	BendInfo.m_bEnableReadInspectWsInfo = m_bEnableReadInspectWsInfo;
	BendInfo.m_bEnableReadStartTimeInfo = m_bEnableReadStartTimeInfo;
	BendInfo.m_bEnableReadEndTimeInfo = m_bEnableReadEndTimeInfo;
	BendInfo.m_bEnableReadProsessingTimeInfo = m_bEnableReadProsessingTimeInfo;

	BendInfo.m_bEnableUseMathCaluLRDis = m_bEnableUseMathCaluLRDis;

	BendInfo.m_bEnableMoveSnap = m_bEnableMoveSnap;
	BendInfo.m_nPosIndexFisrt = m_nPosIndexFisrt;

	BendInfo.m_bEnableAxisMoveLimit = m_bEnableAxisMoveLimit;
	BendInfo.m_dAxisAllowMoveMaxX = m_dAxisAllowMoveMaxX;
	BendInfo.m_dAxisAllowMoveMaxY = m_dAxisAllowMoveMaxY;
	BendInfo.m_dAxisAllowMoveMaxD = m_dAxisAllowMoveMaxD;

	BendInfo.m_bBendInspectSnCodeInvert=m_bBendInspectSnCodeInvert;

	BendInfo.m_bBendInspectSnCodeLength=m_bBendInspectSnCodeLength;
	BendInfo.m_bBendInspectGetSnOnce=m_bBendInspectGetSnOnce;

	BendInfo.m_strPlatformName1 = m_strPlatformName1;
	BendInfo.m_strPlatformName2 = m_strPlatformName2;
	BendInfo.m_bEnableFbisTarObc = m_bEnableFbisTarObc;

	product.m_pAlignerParam->SetBendInfo(BendInfo);

	product.m_pAlignerParam->SetLightCtrlInfo(m_cPhotoLightCtrl);

}

void CDlgSysKindAlignSetting::UpdateStatusBar()
{
// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysKindAlignSetting::UpdateProductListDisplay()
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

void CDlgSysKindAlignSetting::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_KIND_ALIGN_SETTING") == strDlgID)
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

CString CDlgSysKindAlignSetting::GetNewDefName()
{
	CString strName0;
	strName0 = _T("------");
	return strName0;

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

BOOL CDlgSysKindAlignSetting::IsNameUnique( LPCTSTR lpszName )
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

void CDlgSysKindAlignSetting::ShowTipSysKeyboard(CPoint point)

{
	CRect  Rect;
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	CProductData product;
	int nCurProdIdx = pTotalProduct->GetCurProductIndex();
	product = pTotalProduct->GetItem(nCurProdIdx);
	int nCursel;
	nCursel = m_comboAlignerOffsetIndex.GetCurSel();
	for(int i=0;i<6;i++)
	{
		GetDlgItem(IDC_STATIC_TIP1 + i)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		if(Rect.PtInRect(point))
		{
			::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
			CRect rcWnd;
			CString strInput, strOutput;
			GetDlgItem(IDC_STATIC_TIP1 + i)->GetWindowRect(&rcWnd);
			GetDlgItemText(IDC_STATIC_TIP1 + i, strInput);
			if (i==5)
			{
				if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
				{
					SetDlgItemText(IDC_STATIC_TIP1 + i, strOutput);
					if ((nCursel>=0)&&(nCursel<m_comboAlignerOffsetIndex.GetCount()))
					{
						m_vstrOffSetTip[nCursel+5] = strOutput;
						product.m_pAlignerParam->SetAlignerOffsetTip(nCursel+5,strOutput);
					}
				}
				break;
			}
			if (TRUE == m_ctrlKeyboard.ShowStandardKeyBoard(rcWnd, strInput, strOutput))
			{
				SetDlgItemText(IDC_STATIC_TIP1 + i, strOutput);
				m_vstrOffSetTip[i] = strOutput;
				product.m_pAlignerParam->SetAlignerOffsetTip(i,strOutput);
			}
			break;
		}
	}
	
}

BOOL CDlgSysKindAlignSetting::IsNameLegal( LPCTSTR lpszName )
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

BOOL CDlgSysKindAlignSetting::IsCalibratedInfosEqual( std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2 )
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

BOOL CDlgSysKindAlignSetting::IsProductChanged()
{
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
	if (m_nAlignerMaxTimes != product.m_pAlignerParam->GetAlignerMaxTime())
	{
		return TRUE;
	}
	//if (m_bEnableMutiTarget!=product.m_pAlignerParam->GetEnableMutiTarget())
	//{
	//	return TRUE;
	//}
	if (m_comboAlignerOffsetCoordType.GetCurSel() != (int)product.m_pAlignerParam->GetAlignerOffsetCoordType())
	{
		return TRUE;
	}

	//20160510增加对角补偿
	if (m_dObjectL1 != product.m_pAlignerParam->GetProductSizeInfo().m_dObjectL1)
	{
		return TRUE;
	}
	if (m_dObjectW1 != product.m_pAlignerParam->GetProductSizeInfo().m_dObjectW1)
	{
		return TRUE;
	}
	if (m_dTargetL2 != product.m_pAlignerParam->GetProductSizeInfo().m_dTargetL2)
	{
		return TRUE;
	}
	if (m_dTargetW2 != product.m_pAlignerParam->GetProductSizeInfo().m_dTargetW2)
	{
		return TRUE;
	}
	if (m_dF1 != product.m_pAlignerParam->GetProductSizeInfo().m_dF1)
	{
		return TRUE;
	}
	if (m_dF2 != product.m_pAlignerParam->GetProductSizeInfo().m_dF2)
	{
		return TRUE;
	}
	if (m_bOffsetXYEnable != product.m_pAlignerParam->GetProductSizeInfo().m_bOffsetXYEnable)
	{
		return TRUE;
	}
	if (m_nPosMode != product.m_pAlignerParam->GetPositionModeInfo())
	{
		return TRUE;
	}

	if (m_bEnableObjectPositionVirtualMode != product.m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableObjectPositionVirtualMode)
	{
		return TRUE;
	}
	if (m_bEnableTargetPositionVirtualMode != product.m_pAlignerParam->GetProductPositionVirtualInfo().m_bEnableTargetPositionVirtualMode)
	{
		return TRUE;
	}
	if (m_nObjectPositionVirtualMode != product.m_pAlignerParam->GetProductPositionVirtualInfo().m_nObjectPositionVirtualMode)
	{
		return TRUE;
	}
	if (m_nTargetPositionVirtualMode != product.m_pAlignerParam->GetProductPositionVirtualInfo().m_nTargetPositionVirtualMode)
	{
		return TRUE;
	}
	if (m_bEnableProductOffsetXDirection != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetXDirection)
	{
		return TRUE;
	}

	if (m_bEnableProductOffsetYDirection != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetYDirection)
	{
		return TRUE;
	}

	if (m_bEnableProductOffsetDDirection != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetDDirection)
	{
		return TRUE;
	}


	if (m_bEnableProductOffsetXSetting != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetXSetting)
	{
		return TRUE;
	}

	if (m_bEnableProductOffsetYSetting != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetYSetting)
	{
		return TRUE;
	}

	if (m_bEnableProductOffsetDSetting != product.m_pAlignerParam->GetProductOffsetInfo().m_bEnableProductOffsetDSetting)
	{
		return TRUE;
	}

	CCoordPos pos;

	if (m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerBaseOffsetX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerBaseOffsetY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerBaseOffsetD)/PRECISIONDUNIT;	
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerBaseOffsetX);
		pos.m_dPosY = atof(m_strAlignerBaseOffsetY);
		pos.m_dAngle = atof(m_strAlignerBaseOffsetD);		
	}
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerBaseOffset().m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerBaseOffset().m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerBaseOffset().m_dAngle)
	{
		return TRUE;
	}

	int i = 0;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX);
		pos.m_dPosY = atof(m_strAlignerOffsetY);
		pos.m_dAngle = atof(m_strAlignerOffsetD);
	}

	
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i).m_dAngle)
	{
		return TRUE;
	}

	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX2)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY2)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD2)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX2);
		pos.m_dPosY = atof(m_strAlignerOffsetY2);
		pos.m_dAngle = atof(m_strAlignerOffsetD2);
	}
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i).m_dAngle)
	{
		return TRUE;
	}


	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX3)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY3)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD3)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX3);
		pos.m_dPosY = atof(m_strAlignerOffsetY3);
		pos.m_dAngle = atof(m_strAlignerOffsetD3);
	}
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i).m_dAngle)
	{
		return TRUE;
	}


	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX4)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY4)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD4)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX4);
		pos.m_dPosY = atof(m_strAlignerOffsetY4);
		pos.m_dAngle = atof(m_strAlignerOffsetD4);
	}
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i).m_dAngle)
	{
		return TRUE;
	}


	i++;
	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX5)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerOffsetY5)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerOffsetD5)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerOffsetX5);
		pos.m_dPosY = atof(m_strAlignerOffsetY5);
		pos.m_dAngle = atof(m_strAlignerOffsetD5);
	}
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i).m_dAngle)
	{
		return TRUE;
	}


	for (i = 0; i < (ALIGNOFFSETNUM - 5); i++)
	{
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i))/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i))/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i))/PRECISIONDUNIT;
		}
		else
		{
			pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i));
		}

		
		if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset(i+5).m_dPosX 
			|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset(i+5).m_dPosY 
			|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset(i+5).m_dAngle)
		{
			return TRUE;
		}
	}

	for (i = 0; i < ALIGNBENCHOFFSETNUM; i++)
	{
		pos.m_dPosX = atof(m_saAlignerBenchOffsetX.GetAt(i));
		pos.m_dPosY = atof(m_saAlignerBenchOffsetY.GetAt(i));
		pos.m_dAngle = atof(m_saAlignerBenchOffsetD.GetAt(i));
		if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerBenchOffset(i).m_dPosX 
			|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerBenchOffset(i).m_dPosY 
			|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerBenchOffset(i).m_dAngle)
		{
			return TRUE;
		}
	}

	if(m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
		pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
		pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
	}
	else
	{
		pos.m_dPosX = atof(m_strAlignerPrecisionX);
		pos.m_dPosY = atof(m_strAlignerPrecisionY);
		pos.m_dAngle = atof(m_strAlignerPrecisionD);
	}
	
	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerPrecision().m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerPrecision().m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerPrecision().m_dAngle)
	{
		return TRUE;
	}

	if(m_nSearchDelayTime != product.m_pAlignerParam->GetSearchDelayTime())
	{
		return TRUE;
	}


	if(m_nObjectSearchDelayTime != product.m_pAlignerParam->GetObjectSearchDelayTime())
	{
		return TRUE;
	}

	if(m_nObjectSearchDelayTime2 != product.m_pAlignerParam->GetObjectSearchDelayTime2())
	{
		return TRUE;
	}

	if(m_bEnableObjectSearchDelayTime2 != product.m_pAlignerParam->GetEnableObjectSearchDelayTime2())
	{
		return TRUE;
	}

	if (m_bCheckEightShapeAlign != product.m_pAlignerParam->GetEnableEightShapeAlign())
	{
		return TRUE;
	}


	if (m_nEightShapeAlignMode != product.m_pAlignerParam->GetEightShapeAlignMode())
	{
		return TRUE;
	}
	if(m_dEightShapeAngle != product.m_pAlignerParam->GetEightShapeAngle())
	{
		return TRUE;
	}
	//if (m_bDiffPosWithSameSearchResultEnabled != product.m_pAlignerParam->GetMulMarkMode())
	//{
	//	return TRUE;
	//}
	if (m_bAlignAngleVertical != product.m_pAlignerParam->GetEnableAlignAngleVectical())
	{
		return TRUE;
	}
	if (m_nAlignAngleVerticalMode != product.m_pAlignerParam->GetAlignAngleVecticalMode())
	{
		return TRUE;
	}

	if (m_bCheckTargetOffsetEnable != product.m_pAlignerParam->GetEnableAlnTargetOffset())
	{
		return TRUE;
	}
	double dTargetOffsetX,dTargetOffsetY;
	dTargetOffsetX = atof(m_strAlnTargetOffsetX); 
	dTargetOffsetY = atof(m_strAlnTargetOffsetY);
	if (dTargetOffsetX != product.m_pAlignerParam->GetAlnTargetOffsetX() || dTargetOffsetY != product.m_pAlignerParam->GetAlnTargetOffsetY())
	{
		return TRUE;
	}
	if (m_bAlignOffsetReverseX != product.m_pAlignerParam->GetEnableAlnOffsetReverseX()
		|| m_bAlignOffsetReverseY != product.m_pAlignerParam->GetEnableAlnOffsetReverseY()
		|| m_bAlignOffsetReverseD != product.m_pAlignerParam->GetEnableAlnOffsetReverseD())
	{
		return TRUE;
	}
	if (m_bEnableSendTargetOffsetToPLC != product.m_pAlignerParam->GetEnableAlnSendTargetOffsetToPLC())
	{
		return TRUE;
	}

	if(m_bEnableTarFixOffset != product.m_pAlignerParam->GetEnableAlnTargetFixOffset())
	{
		return TRUE;
	}

	if (m_nTarFixOffsetType != product.m_pAlignerParam->GetTargetOffsetType())
	{
		return TRUE;
	}

	for (i=0;i<ALIGNOFFSETNUM;i++)
	{
		pos.m_dPosX = atof(m_saTarFixOffsetX.GetAt(i));
		pos.m_dPosY = atof(m_saTarFixOffsetY.GetAt(i));
		pos.m_dAngle = atof(m_saTarFixOffsetD.GetAt(i));
		if (pos.m_dPosX != product.m_pAlignerParam->GetAlnTargetFixOffset(i).m_dPosX 
			|| pos.m_dPosY != product.m_pAlignerParam->GetAlnTargetFixOffset(i).m_dPosY 
			|| pos.m_dAngle != product.m_pAlignerParam->GetAlnTargetFixOffset(i).m_dAngle)
		{
			return TRUE;
		}
	}


	// 输入模式、检查模式
	
	

	CString strInfo = _T("");
	int nIndex = m_comboMultiCalibExtension.GetCurSel();


	strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
	m_saMult_CheckTargetAndObjectDis.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_nCheckMode);
	m_saMult_CheckMode.SetAt(nIndex, strInfo);



	strInfo.Format(_T("%d"),m_bTargetDisThre);
	m_saMult_CheckTargetDisThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectDisThre);
	m_saMult_CheckObjectDisThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bTargetAngleThre);
	m_saMult_CheckTargetAngleThre.SetAt(nIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectAngleThre);
	m_saMult_CheckObjectAngleThre.SetAt(nIndex, strInfo);


	m_saMult_TargetDisThre.SetAt(nIndex, m_strTargetDisThre);
	m_saMult_ObjectDisThre.SetAt(nIndex, m_strObjectDisThre);
	m_saMult_TargetAngleThre.SetAt(nIndex, m_strTargetAngleThre);
	m_saMult_ObjectAngleThre.SetAt(nIndex, m_strObjectAngleThre);



	int nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum + 1;
	CAlignCheckInfo checkInfo;
	CAlignCheckInfo TempCheckInfo;
	for (i = 0; i < nNums_CheckInfo && m_saMult_CheckTargetAndObjectDis.GetSize(); i++)
	{
		checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(i);

		checkInfo.m_bCheckTargetAndObjectDis = atoi(m_saMult_CheckTargetAndObjectDis.GetAt(i));
		checkInfo.m_nCheckMode = atoi(m_saMult_CheckMode.GetAt(i));


		checkInfo.m_bCheckTargetDis = atoi(m_saMult_CheckTargetDisThre.GetAt(i));
		checkInfo.m_bCheckObjectDis = atoi(m_saMult_CheckObjectDisThre.GetAt(i));
		checkInfo.m_bCheckTargetAngle = atoi(m_saMult_CheckTargetAngleThre.GetAt(i));
		checkInfo.m_bCheckObjectAngle = atoi(m_saMult_CheckObjectAngleThre.GetAt(i));

		checkInfo.m_dTargetDisThreshold = atof(m_saMult_TargetDisThre.GetAt(i));
		checkInfo.m_dObjectDisThreshold = atof(m_saMult_ObjectDisThre.GetAt(i));
		checkInfo.m_dTargetAngleThreshold = atof(m_saMult_TargetAngleThre.GetAt(i));
		checkInfo.m_dObjectAngleThreshold = atof(m_saMult_ObjectAngleThre.GetAt(i));


		TempCheckInfo = product.m_pAlignerParam->GetAlignCheckInfo(i);

		if (TempCheckInfo.m_bCheckTargetAndObjectDis != checkInfo.m_bCheckTargetAndObjectDis)
		{
			return TRUE;
		}

		if (TempCheckInfo.m_nCheckMode != checkInfo.m_nCheckMode)
		{
			return TRUE;
		}

		if ( (TempCheckInfo.m_bCheckTargetDis != checkInfo.m_bCheckTargetDis) || (TempCheckInfo.m_bCheckObjectDis != checkInfo.m_bCheckObjectDis) )
		{
			return TRUE;
		}

		if ( (TempCheckInfo.m_bCheckTargetAngle != checkInfo.m_bCheckTargetAngle) || (TempCheckInfo.m_bCheckObjectAngle != checkInfo.m_bCheckObjectAngle) )
		{
			return TRUE;
		}


		if ( (!(fabs(TempCheckInfo.m_dTargetDisThreshold - checkInfo.m_dTargetDisThreshold) < DBL_EPSILON)) 
		  || (!(fabs(TempCheckInfo.m_dObjectDisThreshold - checkInfo.m_dObjectDisThreshold) < DBL_EPSILON)) )
		{
			return TRUE;
		}


		if ( (!(fabs(TempCheckInfo.m_dTargetAngleThreshold - checkInfo.m_dTargetAngleThreshold) < DBL_EPSILON)) 
		  || (!(fabs(TempCheckInfo.m_dObjectAngleThreshold - checkInfo.m_dObjectAngleThreshold) < DBL_EPSILON)) )
		{
			return TRUE;
		}

	}

	if (m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel()) != product.m_pAlignerParam->GetAligner2Vs2Type()
		|| m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel()) != product.m_pAlignerParam->GetAligner3Vs3Type()
		|| m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel()) != product.m_pAlignerParam->GetAligner4Vs4Type())
	{
		return TRUE;
	}

	//if (m_comboAligner2Vs2Type.GetCurSel() != product.m_pAlignerParam->GetAligner2Vs2Type()
	//	|| m_comboAligner3Vs3Type.GetCurSel() != product.m_pAlignerParam->GetAligner3Vs3Type()
	//	|| m_comboAligner4Vs4Type.GetCurSel() != product.m_pAlignerParam->GetAligner4Vs4Type())
	//{
	//	return TRUE;
	//}

	if (atof(m_strAlignerWeight) != product.m_pAlignerParam->GetAlignerWeight())
	{
		return TRUE;
	}

	if(m_comboAlignerCirclePos.GetCurSel() != product.m_pAlignerParam->GetAlignerCirclePos())
	{
		return TRUE;
	}

	if (m_comboAlignerMoveType.GetCurSel() != product.m_pAlignerParam->GetAlignerMoveType())
	{
		return TRUE;
	}


	if (m_bAlignAccordingToPickEnable != product.m_pAlignerParam->GetAlignAccordingToPickPlatformEnable())
	{
		return TRUE;
	}

	// 专用焊接参数

	// 0.是否启用焊接参数
	if (m_bEnableWeld != product.m_pAlignerParam->GetWeldInfo().m_bEnableWeld)
	{
		return TRUE;
	}

	// 1. 运动量系数
	if (atof(m_strWeldMovementRatioX) != product.m_pAlignerParam->GetWeldInfo().m_WeldMovementRatio.m_dPosX)
	{
		return TRUE;
	}
	if (atof(m_strWeldMovementRatioY) != product.m_pAlignerParam->GetWeldInfo().m_WeldMovementRatio.m_dPosY)
	{
		return TRUE;
	}
	if (atof(m_strWeldMovementRatioD) != product.m_pAlignerParam->GetWeldInfo().m_WeldMovementRatio.m_dAngle)
	{
		return TRUE;
	}

	// 2. 补偿系数
	if (atof(m_strWeldAmmendMoveRatioX) != product.m_pAlignerParam->GetWeldInfo().m_WeldAmmendMovementRatio.m_dPosX)
	{
		return TRUE;
	}

	if (atof(m_strWeldAmmendMoveRatioY) != product.m_pAlignerParam->GetWeldInfo().m_WeldAmmendMovementRatio.m_dPosY)
	{
		return TRUE;
	}



	// 3. 目标多次定位/目标一次定位/目标零次定位
	if (m_nWeldTargetSearchTimes != product.m_pAlignerParam->GetWeldInfo().m_nWeldTargetSearchTimes)
	{
		return TRUE;
	}
	// 4. 对象基准/目标基准/无基准
	if (m_nWeldSearchBench != product.m_pAlignerParam->GetWeldInfo().m_nWeldSearchBench)
	{
		return TRUE;
	}

	// 5. 角度以对象为基准/角度以目标为基准/角度无基准
	if (m_nWeldAngleBench != product.m_pAlignerParam->GetWeldInfo().m_nWeldAngleBench)
	{
		return TRUE;
	}

// 	if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 	{
// 		// PCB参数设置
// 		if (atof(m_strPCBInsPreX)/PRECISIONXYUNIT != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dPosX)
// 		{
// 			return TRUE;
// 		}
// 
// 		if (atof(m_strPCBInsPreY)/PRECISIONXYUNIT != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dPosY)
// 		{
// 			return TRUE;
// 		}
// 
// 		if (atof(m_strPCBInsPreD)/PRECISIONDUNIT != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dAngle)
// 		{
// 			return TRUE;
// 		}
// 	}
// 	else
// 	{
// 		// PCB参数设置
// 		if (atof(m_strPCBInsPreX) != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dPosX)
// 		{
// 			return TRUE;
// 		}
// 
// 		if (atof(m_strPCBInsPreY) != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dPosY)
// 		{
// 			return TRUE;
// 		}
// 
// 		if (atof(m_strPCBInsPreD) != product.m_pAlignerParam->GetPCBInfo().m_PCBInsPre.m_dAngle)
// 		{
// 			return TRUE;
// 		}
// 	}
// 
// 
// 	if (m_nPCBInsPreType != product.m_pAlignerParam->GetPCBInfo().m_nPCBInsPreType)
// 	{
// 		return TRUE;
// 	}
// 
// 	if (m_bPCBShowInspectResult != product.m_pAlignerParam->GetPCBInfo().m_bPCBShowInspectResult)
// 	{
// 		return TRUE;
// 	}
// 
// 	if (m_bShowPCB4To3Align != product.m_pAlignerParam->GetPCBInfo().m_bShowPCB4To3Align)
// 	{
// 		return TRUE;
// 	}

	// 折弯参数
	// 1
	if (m_bBendAlignInspect != product.m_pAlignerParam->GetBendInfo().m_bBendAlignInspect)
	{
		return TRUE;
	}

	if (m_bBendType != product.m_pAlignerParam->GetBendInfo().m_bBendType)
	{
		return TRUE;
	}
	if (atof(m_strPixelSize) != product.m_pAlignerParam->GetBendInfo().m_dBendImageRadio)
	{
		return TRUE;
	}
	if (m_bBendAlignDmCode != product.m_pAlignerParam->GetBendInfo().m_bBendAlignDmCode)
	{
		return TRUE;
	}

	// 2
	if (m_bBendDisOffset != product.m_pAlignerParam->GetBendInfo().m_bBendDisOffset)
	{
		return TRUE;
	}
	if (atof(m_strTargetDisOffset) != product.m_pAlignerParam->GetBendInfo().m_dBendTargetDisOffset)
	{
		return TRUE;
	}
	if (atof(m_strObjectDisOffset) != product.m_pAlignerParam->GetBendInfo().m_dBendObjectDisOffset)
	{
		return TRUE;
	}

	// 3
	if (m_bBendResultOffset != product.m_pAlignerParam->GetBendInfo().m_bBendResultOffset)
	{
		return TRUE;
	}
	if (atof(m_strLeftOffsetX) != product.m_pAlignerParam->GetBendInfo().m_scBendLeftDisOffset.GetX())
	{
		return TRUE;
	}
	if (atof(m_strLeftOffsetY) != product.m_pAlignerParam->GetBendInfo().m_scBendLeftDisOffset.GetY())
	{
		return TRUE;
	}
	if (atof(m_strRightOffsetX) != product.m_pAlignerParam->GetBendInfo().m_scBendRightDisOffset.GetX())
	{
		return TRUE;
	}
	if (atof(m_strRightOffsetY) != product.m_pAlignerParam->GetBendInfo().m_scBendRightDisOffset.GetY())
	{
		return TRUE;
	}
	if (atof(m_strLeftDisOffset) != product.m_pAlignerParam->GetBendInfo().m_dLeftDisOffset)
	{
		return TRUE;
	}
	if (atof(m_strRightDisOffset) != product.m_pAlignerParam->GetBendInfo().m_dRightDisOffset)
	{
		return TRUE;
	}

	// 4
	if (m_bEnableFbsLRY != product.m_pAlignerParam->GetBendInfo().m_bEnableFbsLRY)
	{
		return TRUE;
	}
	// 4
	if (m_bBendThreOffset != product.m_pAlignerParam->GetBendInfo().m_bBendThreOffset)
	{
		return TRUE;
	}
	if (atof(m_strThreOffsetX) != product.m_pAlignerParam->GetBendInfo().m_scBendStanderThre.GetX())
	{
		return TRUE;
	}
	if (atof(m_strThreOffsetY) != product.m_pAlignerParam->GetBendInfo().m_scBendStanderThre.GetY())
	{
		return TRUE;
	}

	// 5
	if (m_bBendPressPara != product.m_pAlignerParam->GetBendInfo().m_bBendPressPara)
	{
		return TRUE;
	}
	if (m_bBendPressParaWrite != product.m_pAlignerParam->GetBendInfo().m_bBendPressParaWrite)
	{
		return TRUE;
	}
	if (m_lBendPressParaWriteAddre != product.m_pAlignerParam->GetBendInfo().m_lBendPressParaWriteAddre)
	{
		return TRUE;
	}
	if (atof(m_strPressOKMinY) != product.m_pAlignerParam->GetBendInfo().m_dBendPressOKYmin)
	{
		return TRUE;
	}
	if (atof(m_strPressOKMaxY) != product.m_pAlignerParam->GetBendInfo().m_dBendPressOKYmax)
	{
		return TRUE;
	}
	if (atof(m_strPressNGMinY) != product.m_pAlignerParam->GetBendInfo().m_dBendPressNGYmin)
	{
		return TRUE;
	}
	if (atof(m_strPressNGMaxY) != product.m_pAlignerParam->GetBendInfo().m_dBendPressNGYmax)
	{
		return TRUE;
	}

	// 6
	if (m_bBendLoadThre != product.m_pAlignerParam->GetBendInfo().m_bBendLoadThre)
	{
		return TRUE;
	}

	if (atof(m_strBendLoadThreX) != product.m_pAlignerParam->GetBendInfo().m_scBendLoadThre.GetX())
	{
		return TRUE;
	}

	if (atof(m_strBendLoadThreY) != product.m_pAlignerParam->GetBendInfo().m_scBendLoadThre.GetY())
	{
		return TRUE;
	}

	// 7
	if (m_bBendingInspect != product.m_pAlignerParam->GetBendInfo().m_bBendingInspect)
	{
		return TRUE;
	}

	// 8
	if (m_bEnableReadWsInfo != product.m_pAlignerParam->GetBendInfo().m_bEnableReadWsInfo)
	{
		return TRUE;
	}
	if (m_bEnableReadInspectWsInfo != product.m_pAlignerParam->GetBendInfo().m_bEnableReadInspectWsInfo)
	{
		return TRUE;
	}
	if (m_bEnableReadStartTimeInfo != product.m_pAlignerParam->GetBendInfo().m_bEnableReadStartTimeInfo)
	{
		return TRUE;
	}
	if (m_bEnableReadEndTimeInfo != product.m_pAlignerParam->GetBendInfo().m_bEnableReadEndTimeInfo)
	{
		return TRUE;
	}
	if (m_bEnableReadProsessingTimeInfo != product.m_pAlignerParam->GetBendInfo().m_bEnableReadProsessingTimeInfo)
	{
		return TRUE;
	}

	if (m_bEnableUseMathCaluLRDis != product.m_pAlignerParam->GetBendInfo().m_bEnableUseMathCaluLRDis)
	{
		return TRUE;
	}

	if (m_bBendingProductID != product.m_pAlignerParam->GetBendInfo().m_bBendingProductID)
	{
		return TRUE;
	}
	if (m_bBendInspectDmCode != product.m_pAlignerParam->GetBendInfo().m_bBendInspectDmCode)
	{
		return TRUE;
	}

	if (m_bEnableMoveSnap != product.m_pAlignerParam->GetBendInfo().m_bEnableMoveSnap)
	{
		return TRUE;
	}
	if (m_nPosIndexFisrt != product.m_pAlignerParam->GetBendInfo().m_nPosIndexFisrt)
	{
		return TRUE;
	}

	if (m_bEnableAxisMoveLimit != product.m_pAlignerParam->GetBendInfo().m_bEnableAxisMoveLimit)
	{
		return TRUE;
	}

	if (m_dAxisAllowMoveMaxX != product.m_pAlignerParam->GetBendInfo().m_dAxisAllowMoveMaxX)
	{
		return TRUE;
	}
	if (m_dAxisAllowMoveMaxY != product.m_pAlignerParam->GetBendInfo().m_dAxisAllowMoveMaxY)
	{
		return TRUE;
	}
	if (m_dAxisAllowMoveMaxD != product.m_pAlignerParam->GetBendInfo().m_dAxisAllowMoveMaxD)
	{
		return TRUE;
	}


	if (m_bBendInspectSnCodeInvert != product.m_pAlignerParam->GetBendInfo().m_bBendInspectSnCodeInvert)
	{
		return TRUE;
	}
	if (m_bBendInspectSnCodeLength != product.m_pAlignerParam->GetBendInfo().m_bBendInspectSnCodeLength)
	{
		return TRUE;
	}

	if (m_bBendInspectGetSnOnce != product.m_pAlignerParam->GetBendInfo().m_bBendInspectGetSnOnce)
	{
		return TRUE;
	}

	if (m_strPlatformName1 != product.m_pAlignerParam->GetBendInfo().m_strPlatformName1)
	{
		return TRUE;
	}
	if (m_strPlatformName2 != product.m_pAlignerParam->GetBendInfo().m_strPlatformName2)
	{
		return TRUE;
	}
	if (m_bEnableFbisTarObc != product.m_pAlignerParam->GetBendInfo().m_bEnableFbisTarObc)
	{
		return TRUE;
	}

	//光源控制功能参数是否发生改变
	if (m_bPhotoLightCtrlInfoChange)
	{
		return TRUE;
	}


	if (m_bShowOffsetTips)
	{
		for(i =0 ;i<ALIGNOFFSETNUM;i++)
		{
			if (m_vstrOffSetTip[i] != product.m_pAlignerParam->GetAlignerOffsetTip(i))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CDlgSysKindAlignSetting::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgSysKindAlignSetting::IsStringOverMaxX(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	CString strVal = lpszVal;
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxX)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxX)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CDlgSysKindAlignSetting::IsStringOverMaxY(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	CString strVal = lpszVal;
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxY)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxY)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CDlgSysKindAlignSetting::IsStringOverMaxD(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	CString strVal = lpszVal;
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxD)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxD)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgSysKindAlignSetting::IsStringPositiveNumerical(LPCTSTR lpszVal)
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

BOOL CDlgSysKindAlignSetting::CreateDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindAlignSetting::ClearDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindAlignSetting::RemoveDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindAlignSetting::RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
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
BOOL CDlgSysKindAlignSetting::SavePrevProduct()
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
		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
		//product.m_pAlignerParam->SetEnableMutiTarget(m_bEnableMutiTarget);
		CCoordPos pos;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX);
			pos.m_dPosY = atof(m_strAlignerPrecisionY);
			pos.m_dAngle = atof(m_strAlignerPrecisionD);
		}
		
		product.m_pAlignerParam->SetAlignerPrecision(pos);
		//lzk
		product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel()));
		product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel()));
		product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel()));

		//product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetCurSel());
		//product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetCurSel());
		//product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetCurSel());
		product.m_pAlignerParam->SetAlignerMoveType((AlignerMoveType)m_comboAlignerMoveType.GetCurSel());

		product.m_pAlignerParam->SetAlignerCirclePos(m_comboAlignerCirclePos.GetCurSel());
		product.m_pAlignerParam->SetAlignerWeight(atof(m_strAlignerWeight));

		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());

		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerBaseOffsetX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerBaseOffsetY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerBaseOffsetD)/PRECISIONDUNIT;	
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerBaseOffsetX);
			pos.m_dPosY = atof(m_strAlignerBaseOffsetY);
			pos.m_dAngle = atof(m_strAlignerBaseOffsetD);		
		}		
		product.m_pAlignerParam->SetAlignerBaseOffset(pos);

		int i=0; 
		i=0;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD)/PRECISIONDUNIT;
		}		
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX);
			pos.m_dPosY = atof(m_strAlignerOffsetY);
			pos.m_dAngle = atof(m_strAlignerOffsetD);
		}
		CString strTip;
		product.m_pAlignerParam->SetAlignerOffset(i, pos);

		strTip = m_vstrOffSetTip[i];
		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
		i++;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX2)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY2)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD2)/PRECISIONDUNIT;
		}		
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX2);
			pos.m_dPosY = atof(m_strAlignerOffsetY2);
			pos.m_dAngle = atof(m_strAlignerOffsetD2);
		}	
		product.m_pAlignerParam->SetAlignerOffset(i, pos);
		strTip = m_vstrOffSetTip[i];
		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);

		i++;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX3)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY3)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD3)/PRECISIONDUNIT;
		}		
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX3);
			pos.m_dPosY = atof(m_strAlignerOffsetY3);
			pos.m_dAngle = atof(m_strAlignerOffsetD3);
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);
		strTip = m_vstrOffSetTip[i];
		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);

		i++;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX4)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY4)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD4)/PRECISIONDUNIT;
		}		
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX4);
			pos.m_dPosY = atof(m_strAlignerOffsetY4);
			pos.m_dAngle = atof(m_strAlignerOffsetD4);
		}		
		product.m_pAlignerParam->SetAlignerOffset(i, pos);
		strTip = m_vstrOffSetTip[i];
		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);

		i++;
		if(m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX5)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerOffsetY5)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerOffsetD5)/PRECISIONDUNIT;
		}		
		else
		{
			pos.m_dPosX = atof(m_strAlignerOffsetX5);
			pos.m_dPosY = atof(m_strAlignerOffsetY5);
			pos.m_dAngle = atof(m_strAlignerOffsetD5);
		}	
		product.m_pAlignerParam->SetAlignerOffset(i, pos);
		strTip = m_vstrOffSetTip[i];
		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);

		int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
		m_saAlignerOffsetX.SetAt(nCurSel, m_strAlignerOffsetX6);
		m_saAlignerOffsetY.SetAt(nCurSel, m_strAlignerOffsetY6);
		m_saAlignerOffsetD.SetAt(nCurSel,  m_strAlignerOffsetD6);

		for (i = 0; i < (ALIGNOFFSETNUM - 5) && i < m_saAlignerOffsetX.GetSize(); i++)
		{
			if(m_pParent->m_SysOptionConfig.m_bChange2um)
			{
				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i))/PRECISIONXYUNIT;
				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i))/PRECISIONXYUNIT;
				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i))/PRECISIONDUNIT;
			}		
			else
			{
				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i));
				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i));
				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i));
			}

			
			product.m_pAlignerParam->SetAlignerOffset(i+5, pos);
			strTip = m_vstrOffSetTip[i+5];
			product.m_pAlignerParam->SetAlignerOffsetTip(i+5,strTip);
		}

		nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
		m_saAlignerBenchOffsetX.SetAt(nCurSel, m_strAlignerBenchOffsetX);
		m_saAlignerBenchOffsetY.SetAt(nCurSel, m_strAlignerBenchOffsetY);
		m_saAlignerBenchOffsetD.SetAt(nCurSel,  m_strAlignerBenchOffsetD);

		for (i = 0; i < ALIGNBENCHOFFSETNUM && i < m_saAlignerBenchOffsetX.GetSize(); i++)
		{
			pos.m_dPosX = atof(m_saAlignerBenchOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saAlignerBenchOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saAlignerBenchOffsetD.GetAt(i));
			product.m_pAlignerParam->SetAlignerBenchOffset(i, pos);
		}

		product.m_pAlignerParam->SetSearchDelayTime(m_nSearchDelayTime);
		product.m_pAlignerParam->SetObjectSearchDelayTime(m_nObjectSearchDelayTime);
		product.m_pAlignerParam->SetObjectSearchDelayTime2(m_nObjectSearchDelayTime2);
		product.m_pAlignerParam->SetEnableObjectSearchDelayTime2(m_bEnableObjectSearchDelayTime2);

		product.m_pAlignerParam->EnableEightShapeAlign(m_bCheckEightShapeAlign);
		product.m_pAlignerParam->SetEightShapeAlignMode(m_nEightShapeAlignMode);
		product.m_pAlignerParam->SetEightShapeAngle(m_dEightShapeAngle);

		//product.m_pAlignerParam->EnableMulMarkMode(m_bDiffPosWithSameSearchResultEnabled);
		product.m_pAlignerParam->SetEnableAlignAngleVectical(m_bAlignAngleVertical);
		product.m_pAlignerParam->SetAlignAngleVecticalMode(m_nAlignAngleVerticalMode);

		BOOL bAlnTargetOffsetEnable;
		double dTargetOffsetX,dTargetOffsetY;

		bAlnTargetOffsetEnable = m_bCheckTargetOffsetEnable;
		dTargetOffsetX = atof(m_strAlnTargetOffsetX);
		dTargetOffsetY = atof(m_strAlnTargetOffsetY);

		product.m_pAlignerParam->EnableAlnTargetOffset(bAlnTargetOffsetEnable);
		product.m_pAlignerParam->SetAlnTargetOffsetX(dTargetOffsetX);
		product.m_pAlignerParam->SetAlnTargetOffsetY(dTargetOffsetY);
		product.m_pAlignerParam->EnableAlnSendTargetOffsetToPLC(m_bEnableSendTargetOffsetToPLC);
		
		product.m_pAlignerParam->EnableAlnTargetFixOffset(m_bEnableTarFixOffset);
		product.m_pAlignerParam->SetTargetOffsetType(m_nTarFixOffsetType);
		for (i=0;i<ALIGNOFFSETNUM;i++)
		{
			pos.m_dPosX = atof(m_saTarFixOffsetX.GetAt(i));
			pos.m_dPosY = atof(m_saTarFixOffsetY.GetAt(i));
			pos.m_dAngle = atof(m_saTarFixOffsetD.GetAt(i));
			product.m_pAlignerParam->SetAlnTargetFixOffset(i,pos);
		}

		product.m_pAlignerParam->EnableAlnOffsetReverseX(m_bAlignOffsetReverseX);
		product.m_pAlignerParam->EnableAlnOffsetReverseY(m_bAlignOffsetReverseY);
		product.m_pAlignerParam->EnableAlnOffsetReverseD(m_bAlignOffsetReverseD);


		// 输入模式、检测模式	
		CString strInfo = _T("");
		int nIndex = m_comboMultiCalibExtension.GetCurSel();
	
		strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
		m_saMult_CheckTargetAndObjectDis.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_nCheckMode);
		m_saMult_CheckMode.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bTargetDisThre);
		m_saMult_CheckTargetDisThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bObjectDisThre);
		m_saMult_CheckObjectDisThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bTargetAngleThre);
		m_saMult_CheckTargetAngleThre.SetAt(nIndex, strInfo);

		strInfo.Format(_T("%d"),m_bObjectAngleThre);
		m_saMult_CheckObjectAngleThre.SetAt(nIndex, strInfo);

		m_saMult_TargetDisThre.SetAt(nIndex,m_strTargetDisThre);
		m_saMult_ObjectDisThre.SetAt(nIndex,m_strObjectDisThre);
		m_saMult_TargetAngleThre.SetAt(nIndex,m_strTargetAngleThre);
		m_saMult_ObjectAngleThre.SetAt(nIndex,m_strObjectAngleThre);


		int nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum + 1;
		CAlignCheckInfo checkInfo;
		for (int j = 0; j < nNums_CheckInfo && m_saMult_CheckTargetAndObjectDis.GetSize(); j++)
		{
			checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(j);

			checkInfo.m_bCheckTargetAndObjectDis = atoi(m_saMult_CheckTargetAndObjectDis.GetAt(j));
			checkInfo.m_nCheckMode = atoi(m_saMult_CheckMode.GetAt(j));

			checkInfo.m_bCheckTargetDis = atoi(m_saMult_CheckTargetDisThre.GetAt(j));
			checkInfo.m_bCheckObjectDis = atoi(m_saMult_CheckObjectDisThre.GetAt(j));
			checkInfo.m_bCheckTargetAngle = atoi(m_saMult_CheckTargetAngleThre.GetAt(j));
			checkInfo.m_bCheckObjectAngle = atoi(m_saMult_CheckObjectAngleThre.GetAt(j));

			checkInfo.m_dTargetDisThreshold = atof(m_saMult_TargetDisThre.GetAt(j));
			checkInfo.m_dObjectDisThreshold = atof(m_saMult_ObjectDisThre.GetAt(j));
			checkInfo.m_dTargetAngleThreshold = atof(m_saMult_TargetAngleThre.GetAt(j));
			checkInfo.m_dObjectAngleThreshold = atof(m_saMult_ObjectAngleThre.GetAt(j));

			product.m_pAlignerParam->SetAlignCheckInfo(checkInfo,j);

		}

		//20160510增加对角补偿
		CProductSizeInfo productSizeInfo;
		productSizeInfo = product.m_pAlignerParam->GetProductSizeInfo();

		productSizeInfo.m_dObjectL1 = m_dObjectL1;
		productSizeInfo.m_dObjectW1 = m_dObjectW1;
		productSizeInfo.m_dTargetL2 = m_dTargetL2;
		productSizeInfo.m_dTargetW2 = m_dTargetW2;
		productSizeInfo.m_dF1 = m_dF1;
		productSizeInfo.m_dF2 = m_dF2;
		productSizeInfo.m_bOffsetXYEnable = m_bOffsetXYEnable;

		product.m_pAlignerParam->SetProductSizeInfo(productSizeInfo);

		//20160510增加对角补偿
		product.m_pAlignerParam->SetPositionModeInfo(m_nPosMode);

		CProductPositionVirtualInfo productPositionVirtualInfo;
		productPositionVirtualInfo = product.m_pAlignerParam->GetProductPositionVirtualInfo();

		productPositionVirtualInfo.m_bEnableObjectPositionVirtualMode = m_bEnableObjectPositionVirtualMode;
		productPositionVirtualInfo.m_bEnableTargetPositionVirtualMode = m_bEnableTargetPositionVirtualMode;
		productPositionVirtualInfo.m_nObjectPositionVirtualMode = m_nObjectPositionVirtualMode;
		productPositionVirtualInfo.m_nTargetPositionVirtualMode = m_nTargetPositionVirtualMode;

		product.m_pAlignerParam->SetProductPositionVirtualInfo(productPositionVirtualInfo);

		// 补偿设置（补偿的方向、X和Y补偿值对调）
		CProductOffsetInfo productOffsetInfo;
		productOffsetInfo = product.m_pAlignerParam->GetProductOffsetInfo();

		productOffsetInfo.m_bEnableProductOffsetXDirection = m_bEnableProductOffsetXDirection;
		productOffsetInfo.m_bEnableProductOffsetYDirection = m_bEnableProductOffsetYDirection;
		productOffsetInfo.m_bEnableProductOffsetDDirection = m_bEnableProductOffsetDDirection;

		productOffsetInfo.m_bEnableProductOffsetXSetting = m_bEnableProductOffsetXSetting;
		productOffsetInfo.m_bEnableProductOffsetYSetting = m_bEnableProductOffsetYSetting;
		productOffsetInfo.m_bEnableProductOffsetDSetting = m_bEnableProductOffsetDSetting;

		product.m_pAlignerParam->SetProductOffsetInfo(productOffsetInfo);

		product.m_pAlignerParam->SetAlignAccordingToPickPlatformEnable(m_bAlignAccordingToPickEnable);

		// 专用焊接参数
		CWeldInfo WeldInfo;
		WeldInfo = product.m_pAlignerParam->GetWeldInfo();

		// 0.是否启用焊接参数
		WeldInfo.m_bEnableWeld = m_bEnableWeld;
		// 1. 运动量系数
		WeldInfo.m_WeldMovementRatio = CCoordPos(atof(m_strWeldMovementRatioX), atof(m_strWeldMovementRatioY), atof(m_strWeldMovementRatioD));
		// 2. 补偿系数
		WeldInfo.m_WeldAmmendMovementRatio = CCoordPos(atof(m_strWeldAmmendMoveRatioX), atof(m_strWeldAmmendMoveRatioY), 0.0);
		// 3. 目标多次定位/目标一次定位/目标零次定位
		WeldInfo.m_nWeldTargetSearchTimes = m_nWeldTargetSearchTimes;
		// 4. 对象基准/目标基准/无基准		
		WeldInfo.m_nWeldSearchBench = m_nWeldSearchBench;
		// 5. 角度以对象为基准/角度以目标为基准/角度无基准
		WeldInfo.m_nWeldAngleBench = m_nWeldAngleBench;

		product.m_pAlignerParam->SetWeldInfo(WeldInfo);


// 		// PCB参数设置
// 		CPCBInfo PCBInfo;
// 		PCBInfo = product.m_pAlignerParam->GetPCBInfo();
// 		if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD)/PRECISIONDUNIT;
// 		}
// 		else
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX);
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY);
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD);
// 		}
// 		
// 		PCBInfo.m_nPCBInsPreType = m_nPCBInsPreType;
// 		PCBInfo.m_bPCBShowInspectResult = m_bPCBShowInspectResult;
// 		PCBInfo.m_bShowPCB4To3Align = m_bShowPCB4To3Align;
// 		product.m_pAlignerParam->SetPCBInfo(PCBInfo);


		// 折弯参数
		CBendInfo BendInfo;
		BendInfo = product.m_pAlignerParam->GetBendInfo();
		BendInfo.m_bBendAlignInspect = m_bBendAlignInspect;

		BendInfo.m_bBendType = m_bBendType;
		BendInfo.m_dBendImageRadio = atof(m_strPixelSize);
		BendInfo.m_bBendAlignDmCode = m_bBendAlignDmCode;

		BendInfo.m_bBendDisOffset = m_bBendDisOffset;
		BendInfo.m_dBendTargetDisOffset = atof(m_strTargetDisOffset);
		BendInfo.m_dBendObjectDisOffset = atof(m_strObjectDisOffset);

		BendInfo.m_bBendResultOffset = m_bBendResultOffset;
		BendInfo.m_scBendLeftDisOffset.SetX(atof(m_strLeftOffsetX));
		BendInfo.m_scBendLeftDisOffset.SetY(atof(m_strLeftOffsetY));
		BendInfo.m_scBendRightDisOffset.SetX(atof(m_strRightOffsetX));
		BendInfo.m_scBendRightDisOffset.SetY(atof(m_strRightOffsetY));
		BendInfo.m_dLeftDisOffset = atof(m_strLeftDisOffset);
		BendInfo.m_dRightDisOffset = atof(m_strRightDisOffset);

		BendInfo.m_bEnableFbsLRY = m_bEnableFbsLRY;
		BendInfo.m_bBendThreOffset = m_bBendThreOffset;
		BendInfo.m_scBendStanderThre.SetX(atof(m_strThreOffsetX));
		BendInfo.m_scBendStanderThre.SetY(atof(m_strThreOffsetY));

		BendInfo.m_bBendPressPara = m_bBendPressPara;
		BendInfo.m_bBendPressParaWrite = m_bBendPressParaWrite;
		BendInfo.m_lBendPressParaWriteAddre = m_lBendPressParaWriteAddre;
		BendInfo.m_dBendPressOKYmin = atof(m_strPressOKMinY);
		BendInfo.m_dBendPressOKYmax = atof(m_strPressOKMaxY);
		BendInfo.m_dBendPressNGYmin = atof(m_strPressNGMinY);
		BendInfo.m_dBendPressNGYmax = atof(m_strPressNGMaxY);

		BendInfo.m_bBendLoadThre = m_bBendLoadThre;
		BendInfo.m_scBendLoadThre.SetX(atof(m_strBendLoadThreX));
		BendInfo.m_scBendLoadThre.SetY(atof(m_strBendLoadThreY));

		BendInfo.m_bBendingInspect = m_bBendingInspect;
		BendInfo.m_bBendingProductID = m_bBendingProductID;
		BendInfo.m_bBendInspectDmCode = m_bBendInspectDmCode;
		BendInfo.m_bEnableReadWsInfo = m_bEnableReadWsInfo;

		BendInfo.m_bEnableReadInspectWsInfo = m_bEnableReadInspectWsInfo;
		BendInfo.m_bEnableReadStartTimeInfo = m_bEnableReadStartTimeInfo;
		BendInfo.m_bEnableReadEndTimeInfo = m_bEnableReadEndTimeInfo;
		BendInfo.m_bEnableReadProsessingTimeInfo = m_bEnableReadProsessingTimeInfo;

		BendInfo.m_bEnableUseMathCaluLRDis = m_bEnableUseMathCaluLRDis;

		BendInfo.m_bEnableMoveSnap = m_bEnableMoveSnap;
		BendInfo.m_nPosIndexFisrt = m_nPosIndexFisrt;

		BendInfo.m_bEnableAxisMoveLimit = m_bEnableAxisMoveLimit;
		BendInfo.m_dAxisAllowMoveMaxX = m_dAxisAllowMoveMaxX;
		BendInfo.m_dAxisAllowMoveMaxY = m_dAxisAllowMoveMaxY;
		BendInfo.m_dAxisAllowMoveMaxD = m_dAxisAllowMoveMaxD;
		
		BendInfo.m_bBendInspectSnCodeLength=m_bBendInspectSnCodeLength;
		BendInfo.m_bBendInspectSnCodeInvert=m_bBendInspectSnCodeInvert;
		BendInfo.m_bBendInspectGetSnOnce=m_bBendInspectGetSnOnce;

		BendInfo.m_strPlatformName1 = m_strPlatformName1;
		BendInfo.m_strPlatformName2 = m_strPlatformName2;
		BendInfo.m_bEnableFbisTarObc = m_bEnableFbisTarObc;

		product.m_pAlignerParam->SetBendInfo(BendInfo);

		product.m_pAlignerParam->SetLightCtrlInfo(m_cPhotoLightCtrl);

		pTotalProduct->SetItem(m_nPrevProductIdx, product);
	}

	return bSucceed;
}

// 保存前一个产品的设置
// BOOL CDlgSysKindAlignSetting::SavePrevProduct()
// {
//  	BOOL bSucceed = TRUE;
// 	UpdateData(TRUE);
// 	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
// 	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
// 	m_nPrevProductIdx = pView->m_nPrevProductIdx;
// 
// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// 	{
// 		// 保存对位参数
// 		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
// 		product.m_pAlignerParam->SetEnableMutiTarget(m_bEnableMutiTarget);
// 		CCoordPos pos;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
// 		}
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerPrecisionX);
// 			pos.m_dPosY = atof(m_strAlignerPrecisionY);
// 			pos.m_dAngle = atof(m_strAlignerPrecisionD);
// 		}
// 		
// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
// 		//lzk
// 		product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel()));
// 		product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel()));
// 		product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel()));
// 
// 		//product.m_pAlignerParam->SetAligner2Vs2Type((Aligner2Vs2Type)m_comboAligner2Vs2Type.GetCurSel());
// 		//product.m_pAlignerParam->SetAligner3Vs3Type((Aligner3Vs3Type)m_comboAligner3Vs3Type.GetCurSel());
// 		//product.m_pAlignerParam->SetAligner4Vs4Type((Aligner4Vs4Type)m_comboAligner4Vs4Type.GetCurSel());
// 		product.m_pAlignerParam->SetAlignerMoveType((AlignerMoveType)m_comboAlignerMoveType.GetCurSel());
// 
// 		product.m_pAlignerParam->SetAlignerCirclePos(m_comboAlignerCirclePos.GetCurSel());
// 		product.m_pAlignerParam->SetAlignerWeight(atof(m_strAlignerWeight));
// 
// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
// 
// 		if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerBaseOffsetX)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerBaseOffsetY)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerBaseOffsetD)/PRECISIONDUNIT;	
// 		}
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerBaseOffsetX);
// 			pos.m_dPosY = atof(m_strAlignerBaseOffsetY);
// 			pos.m_dAngle = atof(m_strAlignerBaseOffsetD);		
// 		}		
// 		product.m_pAlignerParam->SetAlignerBaseOffset(pos);
// 
// 		int i=0; 
// 		i=0;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerOffsetY)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerOffsetD)/PRECISIONDUNIT;
// 		}		
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX);
// 			pos.m_dPosY = atof(m_strAlignerOffsetY);
// 			pos.m_dAngle = atof(m_strAlignerOffsetD);
// 		}
// 		CString strTip;
// 		product.m_pAlignerParam->SetAlignerOffset(i, pos);
// 
// 		strTip = m_vstrOffSetTip[i];
// 		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
// 		i++;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX2)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerOffsetY2)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerOffsetD2)/PRECISIONDUNIT;
// 		}		
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX2);
// 			pos.m_dPosY = atof(m_strAlignerOffsetY2);
// 			pos.m_dAngle = atof(m_strAlignerOffsetD2);
// 		}	
// 		product.m_pAlignerParam->SetAlignerOffset(i, pos);
// 		strTip = m_vstrOffSetTip[i];
// 		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
// 
// 		i++;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX3)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerOffsetY3)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerOffsetD3)/PRECISIONDUNIT;
// 		}		
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX3);
// 			pos.m_dPosY = atof(m_strAlignerOffsetY3);
// 			pos.m_dAngle = atof(m_strAlignerOffsetD3);
// 		}		
// 		product.m_pAlignerParam->SetAlignerOffset(i, pos);
// 		strTip = m_vstrOffSetTip[i];
// 		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
// 
// 		i++;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX4)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerOffsetY4)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerOffsetD4)/PRECISIONDUNIT;
// 		}		
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX4);
// 			pos.m_dPosY = atof(m_strAlignerOffsetY4);
// 			pos.m_dAngle = atof(m_strAlignerOffsetD4);
// 		}		
// 		product.m_pAlignerParam->SetAlignerOffset(i, pos);
// 		strTip = m_vstrOffSetTip[i];
// 		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
// 
// 		i++;
// 		if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX5)/PRECISIONXYUNIT;
// 			pos.m_dPosY = atof(m_strAlignerOffsetY5)/PRECISIONXYUNIT;
// 			pos.m_dAngle = atof(m_strAlignerOffsetD5)/PRECISIONDUNIT;
// 		}		
// 		else
// 		{
// 			pos.m_dPosX = atof(m_strAlignerOffsetX5);
// 			pos.m_dPosY = atof(m_strAlignerOffsetY5);
// 			pos.m_dAngle = atof(m_strAlignerOffsetD5);
// 		}	
// 		product.m_pAlignerParam->SetAlignerOffset(i, pos);
// 		strTip = m_vstrOffSetTip[i];
// 		product.m_pAlignerParam->SetAlignerOffsetTip(i,strTip);
// 
// 		int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
// 		m_saAlignerOffsetX.SetAt(nCurSel, m_strAlignerOffsetX6);
// 		m_saAlignerOffsetY.SetAt(nCurSel, m_strAlignerOffsetY6);
// 		m_saAlignerOffsetD.SetAt(nCurSel,  m_strAlignerOffsetD6);
// 
// 		for (i = 0; i < (ALIGNOFFSETNUM - 5) && i < m_saAlignerOffsetX.GetSize(); i++)
// 		{
// 			if(m_pParent->m_SysOptionConfig.m_bChange2um)
// 			{
// 				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i))/PRECISIONXYUNIT;
// 				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i))/PRECISIONXYUNIT;
// 				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i))/PRECISIONDUNIT;
// 			}		
// 			else
// 			{
// 				pos.m_dPosX = atof(m_saAlignerOffsetX.GetAt(i));
// 				pos.m_dPosY = atof(m_saAlignerOffsetY.GetAt(i));
// 				pos.m_dAngle = atof(m_saAlignerOffsetD.GetAt(i));
// 			}
// 
// 			
// 			product.m_pAlignerParam->SetAlignerOffset(i+5, pos);
// 			strTip = m_vstrOffSetTip[i+5];
// 			product.m_pAlignerParam->SetAlignerOffsetTip(i+5,strTip);
// 		}
// 
// 		nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
// 		m_saAlignerBenchOffsetX.SetAt(nCurSel, m_strAlignerBenchOffsetX);
// 		m_saAlignerBenchOffsetY.SetAt(nCurSel, m_strAlignerBenchOffsetY);
// 		m_saAlignerBenchOffsetD.SetAt(nCurSel,  m_strAlignerBenchOffsetD);
// 
// 		for (i = 0; i < ALIGNBENCHOFFSETNUM && i < m_saAlignerBenchOffsetX.GetSize(); i++)
// 		{
// 			pos.m_dPosX = atof(m_saAlignerBenchOffsetX.GetAt(i));
// 			pos.m_dPosY = atof(m_saAlignerBenchOffsetY.GetAt(i));
// 			pos.m_dAngle = atof(m_saAlignerBenchOffsetD.GetAt(i));
// 			product.m_pAlignerParam->SetAlignerBenchOffset(i, pos);
// 		}
// 
// 		product.m_pAlignerParam->SetSearchDelayTime(m_nSearchDelayTime);
// 		product.m_pAlignerParam->SetObjectSearchDelayTime(m_nObjectSearchDelayTime);
// 		product.m_pAlignerParam->SetObjectSearchDelayTime2(m_nObjectSearchDelayTime2);
// 		product.m_pAlignerParam->SetEnableObjectSearchDelayTime2(m_bEnableObjectSearchDelayTime2);
// 
// 		product.m_pAlignerParam->EnableEightShapeAlign(m_bCheckEightShapeAlign);
// 		product.m_pAlignerParam->SetEightShapeAlignMode(m_nEightShapeAlignMode);
// 		product.m_pAlignerParam->SetEightShapeAngle(m_dEightShapeAngle);
// 
// 		product.m_pAlignerParam->EnableMulMarkMode(m_bDiffPosWithSameSearchResultEnabled);
// 		product.m_pAlignerParam->SetEnableAlignAngleVectical(m_bAlignAngleVertical);
// 		product.m_pAlignerParam->SetAlignAngleVecticalMode(m_nAlignAngleVerticalMode);
// 
// 		BOOL bAlnTargetOffsetEnable;
// 		double dTargetOffsetX,dTargetOffsetY;
// 
// 		bAlnTargetOffsetEnable = m_bCheckTargetOffsetEnable;
// 		dTargetOffsetX = atof(m_strAlnTargetOffsetX);
// 		dTargetOffsetY = atof(m_strAlnTargetOffsetY);
// 
// 		product.m_pAlignerParam->EnableAlnTargetOffset(bAlnTargetOffsetEnable);
// 		product.m_pAlignerParam->SetAlnTargetOffsetX(dTargetOffsetX);
// 		product.m_pAlignerParam->SetAlnTargetOffsetY(dTargetOffsetY);
// 		product.m_pAlignerParam->EnableAlnSendTargetOffsetToPLC(m_bEnableSendTargetOffsetToPLC);
// 		product.m_pAlignerParam->EnableAlnTargetFixOffset(m_bEnableTarFixOffset);
// 		for (i=0;i<ALIGNOFFSETNUM;i++)
// 		{
// 			pos.m_dPosX = atof(m_saTarFixOffsetX.GetAt(i));
// 			pos.m_dPosY = atof(m_saTarFixOffsetY.GetAt(i));
// 			pos.m_dAngle = atof(m_saTarFixOffsetD.GetAt(i));
// 			product.m_pAlignerParam->SetAlnTargetFixOffset(i,pos);
// 		}
// 		/*product.m_pAlignerParam->SetAlnTargetFixOffsetX(m_dTarFixOffsetX);
// 		product.m_pAlignerParam->SetAlnTargetFixOffsetY(m_dTarFixOffsetY);
// 		product.m_pAlignerParam->SetAlnTargetFixOffsetD(m_dTarFixOffsetD);*/
// 
// 		product.m_pAlignerParam->EnableAlnOffsetReverseX(m_bAlignOffsetReverseX);
// 		product.m_pAlignerParam->EnableAlnOffsetReverseY(m_bAlignOffsetReverseY);
// 		product.m_pAlignerParam->EnableAlnOffsetReverseD(m_bAlignOffsetReverseD);
// 
// // 		product.m_pAlignerParam->SetEnableTargetSearchShutter(m_bTargetSearchShutter);
// // 		product.m_pAlignerParam->SetEnableObjectSearchShutter(m_bObjectSearchShutter);
// //		product.m_pAlignerParam->SetEnableInspectSearchShutter(m_bInspectSearchShutter);
// 
// 		CAlignCheckInfo checkInfo;
// 		int nSel = m_comboMultiCalibExtension.GetCurSel();
// 
// 		CString tmp = _T("");
// 		tmp.Format(_T("%d"),m_bCheckTargetAndObjectDis);
// 		m_saAMultiCalib_CheckTargetAndObjectDis.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_MaxTargetAndObjectDis.SetAt(nSel,m_strTargetAndObjectDis);
// 
// 		tmp.Format(_T("%d"),m_bEnableTarObjSeparateCheck);
// 		m_saAMultiCalib_CheckTargetAndObjectSeparateCheck.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_MaxObjectDis.SetAt(nSel,m_strObjectCheckDis);
// 
// 		tmp.Format(_T("%d"),m_nCheckMode);
// 		m_saAMultiCalib_CheckMode.SetAt(nSel,tmp);
// 
// 		tmp.Format(_T("%d"),m_bCheckTargetDis);
// 		m_saAMultiCalib_CheckTargetDis.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_TargetDis0.SetAt(nSel,m_strTargetDis);
// 
// 		tmp.Format(_T("%d"),m_bCheckObjectDis);
// 		m_saAMultiCalib_CheckObjectDis.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_ObjectDis0.SetAt(nSel,m_strObjectDis);
// 
// 		tmp.Format(_T("%d"),m_bCheckTargetAngle);
// 		m_saAMultiCalib_CheckTargetAngle.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_MaxTargetAngle.SetAt(nSel,m_strTargetAngle);
// 
// 		tmp.Format(_T("%d"),m_bCheckPixelDis);
// 		m_saAMultiCalib_CheckPixelDis.SetAt(nSel,tmp);
// 
// 		m_saAMultiCalib_PixelDisX1.SetAt(nSel,m_strPixelDisX1);
// 		m_saAMultiCalib_PixelDisY1.SetAt(nSel,m_strPixelDisY1);
// 		m_saAMultiCalib_PixelDisX2.SetAt(nSel,m_strPixelDisX2);
// 		m_saAMultiCalib_PixelDisY2.SetAt(nSel,m_strPixelDisY2);
// 
// 		int nNums_CheckInfo = 1;
// 		nNums_CheckInfo = product.m_pAlignerParam->m_nMultiCalibExtensionMaxNum + 1;
// 
// 		for (int j = 0; j < nNums_CheckInfo && m_saAMultiCalib_CheckTargetAndObjectDis.GetSize(); j++)
// 		{
// 			checkInfo = product.m_pAlignerParam->GetAlignCheckInfo(j);
// 
// 			checkInfo.m_bCheckTargetAndObjectDis = atoi(m_saAMultiCalib_CheckTargetAndObjectDis.GetAt(j));
// 			checkInfo.m_dMaxTargetAndObjectDis = atof(m_saAMultiCalib_MaxTargetAndObjectDis.GetAt(j));
// 
// 			checkInfo.m_bEnableTarObjSeparateCheck = atoi(m_saAMultiCalib_CheckTargetAndObjectSeparateCheck.GetAt(j));
// 			checkInfo.m_dMaxObjectDis = atof(m_saAMultiCalib_MaxObjectDis.GetAt(j));
// 
// 			checkInfo.m_nCheckMode = atoi(m_saAMultiCalib_CheckMode.GetAt(j));
// 
// 			checkInfo.m_bCheckTargetDis = atoi(m_saAMultiCalib_CheckTargetDis.GetAt(j));
// 			checkInfo.m_dTargetDis0 = atof(m_saAMultiCalib_TargetDis0.GetAt(j));
// 
// 			checkInfo.m_bCheckObjectDis = atoi(m_saAMultiCalib_CheckObjectDis.GetAt(j));
// 			checkInfo.m_dObjectDis0 = atof(m_saAMultiCalib_ObjectDis0.GetAt(j));
// 
// 			checkInfo.m_bCheckTargetAngle =atoi(m_saAMultiCalib_CheckTargetAngle.GetAt(j));
// 			checkInfo.m_dMaxTargetAngle = atof(m_saAMultiCalib_MaxTargetAngle.GetAt(j));
// 
// 			checkInfo.m_bCheckPixelDis = atoi(m_saAMultiCalib_CheckPixelDis.GetAt(j));
// 			checkInfo.m_dPixelDisX1 = atof(m_saAMultiCalib_PixelDisX1.GetAt(j));
// 			checkInfo.m_dPixelDisY1 = atof(m_saAMultiCalib_PixelDisY1.GetAt(j));
// 			checkInfo.m_dPixelDisX2 = atof(m_saAMultiCalib_PixelDisX2.GetAt(j));
// 			checkInfo.m_dPixelDisY2 = atof(m_saAMultiCalib_PixelDisY2.GetAt(j));
// 
// 			product.m_pAlignerParam->SetAlignCheckInfo(checkInfo,j);
// 
// 		}
// 		//checkInfo = product.m_pAlignerParam->GetAlignCheckInfo();
// 
// 		//checkInfo.m_bCheckTargetAndObjectDis = m_bCheckTargetAndObjectDis;
// 		//checkInfo.m_dMaxTargetAndObjectDis = atof(m_strTargetAndObjectDis);
// 
// 		//checkInfo.m_nCheckMode = m_nCheckMode;
// 
// 		//checkInfo.m_bCheckTargetDis = m_bCheckTargetDis;
// 		//checkInfo.m_dTargetDis0 = atof(m_strTargetDis);
// 
// 		//checkInfo.m_bCheckObjectDis = m_bCheckObjectDis;
// 		//checkInfo.m_dObjectDis0 = atof(m_strObjectDis);
// 
// 		//checkInfo.m_bCheckTargetAngle = m_bCheckTargetAngle;
// 		//checkInfo.m_dMaxTargetAngle = atof(m_strTargetAngle);
// 
// 		//checkInfo.m_bCheckPixelDis = m_bCheckPixelDis;
// 		//checkInfo.m_dPixelDisX1 = atof(m_strPixelDisX1);
// 		//checkInfo.m_dPixelDisY1 = atof(m_strPixelDisY1);
// 		//checkInfo.m_dPixelDisX2 = atof(m_strPixelDisX2);
// 		//checkInfo.m_dPixelDisY2 = atof(m_strPixelDisY2);
// 
// 		//product.m_pAlignerParam->SetAlignCheckInfo(checkInfo);
// 
// 		//20160510增加对角补偿
// 		CProductSizeInfo productSizeInfo;
// 		productSizeInfo = product.m_pAlignerParam->GetProductSizeInfo();
// 
// 		productSizeInfo.m_dObjectL1 = m_dObjectL1;
// 		productSizeInfo.m_dObjectW1 = m_dObjectW1;
// 		productSizeInfo.m_dTargetL2 = m_dTargetL2;
// 		productSizeInfo.m_dTargetW2 = m_dTargetW2;
// 		productSizeInfo.m_dF1 = m_dF1;
// 		productSizeInfo.m_dF2 = m_dF2;
// 		productSizeInfo.m_bOffsetXYEnable = m_bOffsetXYEnable;
// 
// 		product.m_pAlignerParam->SetProductSizeInfo(productSizeInfo);
// 
// 		//20160510增加对角补偿
// 		product.m_pAlignerParam->SetPositionModeInfo(m_nPosMode);
// 
// 		CProductPositionVirtualInfo productPositionVirtualInfo;
// 		productPositionVirtualInfo = product.m_pAlignerParam->GetProductPositionVirtualInfo();
// 
// 		productPositionVirtualInfo.m_bEnableObjectPositionVirtualMode = m_bEnableObjectPositionVirtualMode;
// 		productPositionVirtualInfo.m_bEnableTargetPositionVirtualMode = m_bEnableTargetPositionVirtualMode;
// 		productPositionVirtualInfo.m_nObjectPositionVirtualMode = m_nObjectPositionVirtualMode;
// 		productPositionVirtualInfo.m_nTargetPositionVirtualMode = m_nTargetPositionVirtualMode;
// 
// 		product.m_pAlignerParam->SetProductPositionVirtualInfo(productPositionVirtualInfo);
// 
// 		// 补偿设置（补偿的方向、X和Y补偿值对调）
// 		CProductOffsetInfo productOffsetInfo;
// 		productOffsetInfo = product.m_pAlignerParam->GetProductOffsetInfo();
// 
// // 		productOffsetInfo.m_nProductOffsetXDirection = m_nProductOffsetXDirection;
// // 		productOffsetInfo.m_nProductOffsetYDirection = m_nProductOffsetYDirection;
// // 		productOffsetInfo.m_nProductOffsetDDirection = m_nProductOffsetDDirection;
// // 
// // 		productOffsetInfo.m_nProductOffsetXSetting = m_nProductOffsetXSetting;
// // 		productOffsetInfo.m_nProductOffsetYSetting = m_nProductOffsetYSetting;
// // 		productOffsetInfo.m_nProductOffsetDSetting = m_nProductOffsetDSetting;
// 
// 		productOffsetInfo.m_bEnableProductOffsetXDirection = m_bEnableProductOffsetXDirection;
// 		productOffsetInfo.m_bEnableProductOffsetYDirection = m_bEnableProductOffsetYDirection;
// 		productOffsetInfo.m_bEnableProductOffsetDDirection = m_bEnableProductOffsetDDirection;
// 
// 		productOffsetInfo.m_bEnableProductOffsetXSetting = m_bEnableProductOffsetXSetting;
// 		productOffsetInfo.m_bEnableProductOffsetYSetting = m_bEnableProductOffsetYSetting;
// 		productOffsetInfo.m_bEnableProductOffsetDSetting = m_bEnableProductOffsetDSetting;
// 
// 		product.m_pAlignerParam->SetProductOffsetInfo(productOffsetInfo);
// 
// 		product.m_pAlignerParam->SetAlignAccordingToPickPlatformEnable(m_bAlignAccordingToPickEnable);
// 
// 		// 专用焊接参数
// 		CWeldInfo WeldInfo;
// 		WeldInfo = product.m_pAlignerParam->GetWeldInfo();
// 
// 		// 0.是否启用焊接参数
// 		WeldInfo.m_bEnableWeld = m_bEnableWeld;
// 		// 1. 运动量系数
// 		WeldInfo.m_WeldMovementRatio = CCoordPos(atof(m_strWeldMovementRatioX), atof(m_strWeldMovementRatioY), atof(m_strWeldMovementRatioD));
// 		// 2. 补偿系数
// 		WeldInfo.m_WeldAmmendMovementRatio = CCoordPos(atof(m_strWeldAmmendMoveRatioX), atof(m_strWeldAmmendMoveRatioY), 0.0);
// 		// 3. 目标多次定位/目标一次定位/目标零次定位
// 		WeldInfo.m_nWeldTargetSearchTimes = m_nWeldTargetSearchTimes;
// 		// 4. 对象基准/目标基准/无基准		
// 		WeldInfo.m_nWeldSearchBench = m_nWeldSearchBench;
// 		// 5. 角度以对象为基准/角度以目标为基准/角度无基准
// 		WeldInfo.m_nWeldAngleBench = m_nWeldAngleBench;
// 
// 		product.m_pAlignerParam->SetWeldInfo(WeldInfo);
// 
// 
// 		// PCB参数设置
// 		CPCBInfo PCBInfo;
// 		PCBInfo = product.m_pAlignerParam->GetPCBInfo();
// 		if (m_pParent->m_SysOptionConfig.m_bChange2um)
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY)/PRECISIONXYUNIT;
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD)/PRECISIONDUNIT;
// 		}
// 		else
// 		{
// 			PCBInfo.m_PCBInsPre.m_dPosX = atof(m_strPCBInsPreX);
// 			PCBInfo.m_PCBInsPre.m_dPosY = atof(m_strPCBInsPreY);
// 			PCBInfo.m_PCBInsPre.m_dAngle = atof(m_strPCBInsPreD);
// 		}
// 		
// 		PCBInfo.m_nPCBInsPreType = m_nPCBInsPreType;
// 		PCBInfo.m_bPCBShowInspectResult = m_bPCBShowInspectResult;
// 		PCBInfo.m_bShowPCB4To3Align = m_bShowPCB4To3Align;
// 		product.m_pAlignerParam->SetPCBInfo(PCBInfo);
// 
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
// 	}
// 
// 
// // 	UpdateData(TRUE);
// // 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
// // 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// // 	{
// // 		// 保存对位参数
// // 		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
// // 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
// // 		CCoordPos pos;
// // 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
// // 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
// // 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
// // 		product.m_pAlignerParam->SetAlignerPrecision(pos);
// // 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
// // 		pos.m_dPosX = atof(m_strAlignerOffsetX);
// // 		pos.m_dPosY = atof(m_strAlignerOffsetY);
// // 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
// // 		product.m_pAlignerParam->SetAlignerOffset(pos);
// // 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
// // 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// // 		{
// // 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// // 		}
// // 
// // 		// 		// by adk
// // 		// 		CBaseSearchTool *pSearchTool = NULL;
// // 		// 		SysPlatformInfo platformInfo;
// // 		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// // 		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
// // 		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// // 		// 		for (int j=0; j<nPatternNum; j++)
// // 		// 		{
// // 		// 			// 			int nPatSel = m_vnCurSelPat[j];
// // 		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
// // 		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
// // 		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
// // 		// 			
// // 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
// // 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
// // 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
// // 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
// // 		// 		} // end by
// // 
// // 		pTotalProduct->SetItem(m_nPrevProductIdx, product);
// // 
// // 
// // 		// 保存产品信息到磁盘
// // 		CString strDir, strTemp;
// // 		SysPlatformInfo platformInfo;
// // 		vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// // 		pVisionASM->GetSysPlatformInfo(platformInfo);
// // 		// 如果产品被重命名，则删除旧文件架
// // 		if ((m_bProdNameChanged) && (!m_strProductNameOld.IsEmpty()))
// // 		{
// // 			strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// // 			strTemp.Format(_T("\\Jobs\\%s"), m_strProductNameOld);
// // 			strDir += strTemp;
// // 			RemoveDirectoryPro(strDir);
// // 		}
// // 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// // 		strTemp.Format(_T("\\Jobs\\%s"), pTotalProduct->GetItemName(m_nPrevProductIdx));
// // 		strDir += strTemp;        
// // 		//ClearDirectoryPro(strDir); // Mali 2013.9.5 为提升效率
// // 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nPrevProductIdx);
// // 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// // 
// // 
// // 		// 保存模板到磁盘
// // 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// // 		CBaseSearchTool *pSearchTool = NULL;
// // 
// // 		int j = 0;
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
// // 
// // 
// // 		if (platformInfo.m_eAlignerObjectType != eObjectBench)
// // 		{
// // 			// 保存对象Mark模板
// // 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// // 			{
// // 				CreateDirectoryPro(strDir + _T("\\ObjectModels"));
// // 				for (j=0; j<nPatternNum; j++)
// // 				{
// // 					strTemp.Format(_T("\\ObjectModels\\M%d"), j);
// // 					pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
// // 					if (pSearchTool != NULL)
// // 					{					
// // 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// // 						{
// // 							//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// // 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
// // 							AfxMessageBox(strTemp);
// // 							bSucceed = FALSE;
// // 						}	
// // 					}
// // 				}
// // 			}
// // 		}
// // 		else
// // 		{
// // 			RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
// // 		}
// // 
// // 		// 		// 保存对象Mark模板
// // 		// 		if (m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// // 		// 		{
// // 		// 			CreateDirectoryPro(strDir + _T("\\ObjectModels"));
// // 		// 			for (j=0; j<nPatternNum; j++)
// // 		// 			{
// // 		// 				strTemp.Format(_T("\\ObjectModels\\M%d"), j);
// // 		// 				pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
// // 		// 				if (pSearchTool != NULL)
// // 		// 				{					
// // 		// 					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// // 		// 					{
// // 		// 						//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// // 		// 						strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
// // 		// 						AfxMessageBox(strTemp);
// // 		// 						bSucceed = FALSE;
// // 		// 					}	
// // 		// 				}
// // 		// 			}
// // 		// 		}
// // 
// // 		m_strProductNameOld.Empty();
// // 	}
// 
// 	return bSucceed;
// }

void CDlgSysKindAlignSetting::OnButtonNewProduct()
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


void CDlgSysKindAlignSetting::OnBtnCloneProduct() 
{
	// TODO: Add your control notification handler code here
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
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

void CDlgSysKindAlignSetting::OnButtonDeleteCurProduct() 
{
	// 用户删除操作确认
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

void CDlgSysKindAlignSetting::OnBtnProductRename()
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

void CDlgSysKindAlignSetting::OnBtnAmendBenchmark() 
{
	CDlgBenchMarkAmend dlgBenchMarkAmend;
	UpdateData(TRUE);
	dlgBenchMarkAmend.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	dlgBenchMarkAmend.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgBenchMarkAmend.DoModal() == IDOK)
	{
		// 标记基准修正信息已修改
		m_bProdBenchMarkChanged = TRUE;
	}
}

// 双击Display控件响应
LRESULT CDlgSysKindAlignSetting::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
// 	if (m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
// 	{
// 		OnButtonNewProduct();
// 	}
// 
// 	int i = 0;
// 	SysPlatformInfo platformInfo;
// 	CBaseSearchTool *pSearchTool = NULL;
// 	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
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
// 
// 	// Object mark check
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		for (i=0; i<nPatternNum; i++)
// 		{
// 			if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
// 			{
// 				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				CDlgSearch dlgSearch(this, pSearchTool);
// 				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 				if (dlgSearch.DoModal() == IDOK)
// 				{
// 					pSearchTool = dlgSearch.GetSearchTool();
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
// 
// 					// 显示模板训练图像
// 					if (pSearchTool != NULL)
// 					{
// 						scImageGray imagePattern;
// 						pSearchTool->GetPatternImage(imagePattern);
// 						m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 						m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 					}
// 					else
// 					{
// 						m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 					}
// 					// 标记产品模板已修改
// 					m_bProdPatternChanged = TRUE;
// 				}
// 				return S_OK;
// 			}
// 		}
// 
// 	}
// 	// 	for (i=0; i<nPatternNum; i++)
// 	// 	{
// 	// 		if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
// 	// 		{
// 	// 			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 	// 			CDlgSearch dlgSearch(this, pSearchTool);
// 	// 			dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 	// 			dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 	// 			dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 	// 			if (dlgSearch.DoModal() == IDOK)
// 	// 			{
// 	// 				pSearchTool = dlgSearch.GetSearchTool();
// 	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
// 	// 				
// 	// 				// 显示模板训练图像
// 	// 				if (pSearchTool != NULL)
// 	// 				{
// 	// 					scImageGray imagePattern;
// 	// 					pSearchTool->GetPatternImage(imagePattern);
// 	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 	// 					m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 	// 				}
// 	// 				else
// 	// 				{
// 	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 	// 				}
// 	// 				// 标记产品模板已修改
// 	// 				m_bProdPatternChanged = TRUE;
// 	// 			}
// 	// 			return S_OK;
// 	// 		}
// 	// 	}


	return S_OK ;
}

void CDlgSysKindAlignSetting::OnPlatformChange() 
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
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 修改新选择的平台的显示界面
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0, i = 0;
// 	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
// 	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
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
// 
// 	// 对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<6; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<6; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
//	}
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
	// 修改新选择的平台的显示界面
	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	UpdateDlgByAuthorize();
	// 对位模式界面修改
	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
		/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
	{
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(m_bShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_2VS2_TYPE)->ShowWindow(SW_HIDE);
	}

	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
	{
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(m_bShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_3VS3_TYPE)->ShowWindow(SW_HIDE);
	}

	if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
		|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
	{
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);		
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(m_bShowAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);		
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALIGNER_4VS4_TYPE)->ShowWindow(SW_HIDE);
	}

	//if (platformInfo.m_bPlatformPick == TRUE || 1)
	//{		
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(TRUE);		
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(TRUE);		
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->EnableWindow(FALSE);		
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_CHECK_MUL_MARK_MODE)->ShowWindow(SW_HIDE);
	//}
	// 八字对位显示
    //if (FALSE)
    //{
    //    GetDlgItem(IDC_CHECK_EIGHT_SHAPE_ALIGN)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_RADIO_POSITIVE_EIGHT)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_RADIO_NEGTIVE_EIGHT)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_STATIC_EIGHT_ANGLE)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->ShowWindow(SW_SHOW);
    //    GetDlgItem(IDC_STATIC_DEGREE)->ShowWindow(SW_SHOW);
    //}
    //else
    //{
    //    GetDlgItem(IDC_CHECK_EIGHT_SHAPE_ALIGN)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_RADIO_POSITIVE_EIGHT)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_RADIO_NEGTIVE_EIGHT)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_STATIC_EIGHT_ANGLE)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->ShowWindow(SW_HIDE);
    //    GetDlgItem(IDC_STATIC_DEGREE)->ShowWindow(SW_HIDE);
    //}
	if (platformInfo.m_bTargetCalibUseBoard && platformInfo.m_bTargetObjectCamSeparate)
	{
		GetDlgItem(IDC_BTN_TAR_FIX_OFFSET)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_TAR_FIX_OFFSET)->ShowWindow(SW_HIDE);
	}
// 	// 针对X平台的运动量系数
// 	if (platformInfo.m_eMidPlatformType == ePlatformX)
// 	{
// 		GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_X)->ShowWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_X)->ShowWindow(TRUE);
// 		GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_Y)->ShowWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_Y)->ShowWindow(TRUE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_X)->ShowWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_X)->ShowWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_MOVEMENT_RATIO_Y)->ShowWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_Y)->ShowWindow(FALSE);
// 	}


	// 专用焊接参数
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}
	if (optionInfo.m_bShowFWEL)
	{
		GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_WELD_SETTING)->ShowWindow(FALSE);
	}

	//光源控制
	if (optionInfo.m_bEnablePhotoLightCtrl)
	{
		GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_CMD_LIGHT_CTRL_SET)->ShowWindow(FALSE);
	}

	// PCB 参数
	if (optionInfo.m_bShowBend)
	{
		GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_BEND_SETTING)->ShowWindow(FALSE);
	}
	GetDlgItem(IDC_BTN_PCB_SETTING)->ShowWindow(FALSE);

	if (optionInfo.m_bShowEightShapeAlign)
	{
		GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(m_bShowAlignSpecialApplicationSet);
	}
	else
	{
		GetDlgItem(IDC_BTN_EIGHT_SHAPE_ALIGN_SETTING)->ShowWindow(FALSE);
	}
	// 相机曝光时间设置
// 	if (m_pParent->m_SysOptionConfig.m_bEnableSearchShutter == TRUE)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_SEARCH_SHUTTER)->EnableWindow(TRUE);	
// 		GetDlgItem(IDC_STATIC_GRP_SEARCH_SHUTTER)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CHECK_TARGET_SHUTTER)->EnableWindow(TRUE);	
// 		GetDlgItem(IDC_CHECK_TARGET_SHUTTER)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CHECK_OBJECT_SHUTTER)->EnableWindow(TRUE);	
// 		GetDlgItem(IDC_CHECK_OBJECT_SHUTTER)->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_SEARCH_SHUTTER)->EnableWindow(FALSE);	
// 		GetDlgItem(IDC_STATIC_GRP_SEARCH_SHUTTER)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CHECK_TARGET_SHUTTER)->EnableWindow(FALSE);	
// 		GetDlgItem(IDC_CHECK_TARGET_SHUTTER)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CHECK_OBJECT_SHUTTER)->EnableWindow(FALSE);	
// 		GetDlgItem(IDC_CHECK_OBJECT_SHUTTER)->ShowWindow(SW_HIDE);
// 	}


	//// 多目标对位设置
	//if (optionInfo.m_bEnableMutiTarget == TRUE)
	//{
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->EnableWindow(TRUE);	
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->EnableWindow(TRUE);	
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->EnableWindow(FALSE);	
	//	GetDlgItem(IDC_STATIC_MUTITARGET)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->EnableWindow(FALSE);	
	//	GetDlgItem(IDC_CHECK_ENABLEMUTITARGET)->ShowWindow(SW_HIDE);
	//}
	// 启用单产品扩展，才启用控件
	if (platformInfo.m_bEnableMultiCalibExtension)
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(m_bShowCheckMode);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(m_bShowCheckMode);
		m_comboMultiCalibExtension.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_comboMultiCalibExtension.InsertString(it,(LPCTSTR)strNum);
		}
		m_comboMultiCalibExtension.SetCurSel(0);

	}
	else
	{
		GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		m_comboMultiCalibExtension.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_comboMultiCalibExtension.InsertString(it,(LPCTSTR)strNum);
		}
		m_comboMultiCalibExtension.SetCurSel(0);
	}
	if (m_pParent->m_SysOptionConfig.m_bCheckModeOnly)
	{
		GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TARGET_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_TARGET_AND_OBJECT_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE0)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_RADIO_ALIGN_CHECK_MODE1)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_CHECK_TARGET_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
		GetDlgItem(IDC_CHECK_OBJECT_DIS)->EnableWindow(m_bEnableCheckModeAuthoroze);
	}

	UpdateDlgData(FALSE);
//	UpdateProductListDisplay();

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_nPrevProductIdx = m_nCurProductIdx;

	MoveDlgItem();
// 	m_nPrevPlatformIdx = m_nPlatformIndex;
// 	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
// 	m_nPrevProductIdx = m_nCurProductIdx;
//	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerMaxTimes() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最多对位次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerMaxTimes();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXPrecision() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[设定精度][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXPrecision();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYPrecision() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[设定精度][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYPrecision();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDPrecision() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[设定精度][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDPrecision();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXBaseOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_BASE_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_BASE_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BASE_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值0(基础补偿)]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXBaseOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYBaseOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_BASE_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值0(基础补偿)]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYBaseOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDBaseOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_BASE_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_BASE_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BASE_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值0(基础补偿)]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDBaseOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值1]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值1]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset() 
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
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值1]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset();
	}	
}


void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值2]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset2();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值2]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset2();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值2]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset2();
	}	
}



void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值3]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset3();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值3]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset3();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值3]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset3();
	}	
}



void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值4]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset4();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值4]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset4();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值4]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset4();
	}	
}




void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值5]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset5();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值5]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset5();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset5() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[补偿值5]的[D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset5();
	}	
}




void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXOffset6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[其他补偿]补偿%d的[X]由%s到%s\n"),m_comboAlignerOffsetIndex.GetCurSel()+6,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset6();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYOffset6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[其他补偿]补偿%d的[Y]由%s到%s\n"),m_comboAlignerOffsetIndex.GetCurSel()+6,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset6();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDOffset6() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET6)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET6, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET6, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[其他补偿]补偿%d的[D]由%s到%s\n"),m_comboAlignerOffsetIndex.GetCurSel()+6,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset6();
	}	
}


void CDlgSysKindAlignSetting::OnSetfocusEditAlignerXBenchOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_BENCH_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[下料补偿%d]的[X]由%s到%s\n"), m_comboAlignerBenchOffsetIndex.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXBenchOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerYBenchOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[下料补偿%d]的[Y]由%s到%s\n"),m_comboAlignerBenchOffsetIndex.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYBenchOffset();
	}	
}

void CDlgSysKindAlignSetting::OnSetfocusEditAlignerDBenchOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_BENCH_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[下料补偿%d]的[D]由%s到%s\n"),m_comboAlignerBenchOffsetIndex.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDBenchOffset();
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerMaxTimes() 
{
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nAlignerMaxTimes = (m_nAlignerMaxTimes<1) ? 1 : ((m_nAlignerMaxTimes>50) ? 50 : m_nAlignerMaxTimes);
		UpdateData(FALSE);
	}
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
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

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
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

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strVal);
		if (!IsStringPositiveNumerical(strVal))
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

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXBaseOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_BASE_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BASE_OFFSET, m_strAlignerBaseOffsetX);
		}
		else
		{
			m_strAlignerBaseOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BASE_OFFSET, m_strAlignerBaseOffsetX);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYBaseOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, m_strAlignerBaseOffsetY);
		}
		else
		{
			m_strAlignerBaseOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BASE_OFFSET, m_strAlignerBaseOffsetY);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDBaseOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_BASE_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BASE_OFFSET, m_strAlignerBaseOffsetD);
		}
		else
		{
			m_strAlignerBaseOffsetD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BASE_OFFSET, m_strAlignerBaseOffsetD);
		}
	}	
}


void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
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

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
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

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
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



void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET2, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET2, m_strAlignerOffsetX2);
		}
		else
		{
			m_strAlignerOffsetX2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET2, m_strAlignerOffsetX2);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET2, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET2, m_strAlignerOffsetY2);
		}
		else
		{
			m_strAlignerOffsetY2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET2, m_strAlignerOffsetY2);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET2, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET2, m_strAlignerOffsetD2);
		}
		else
		{
			m_strAlignerOffsetD2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET2, m_strAlignerOffsetD2);
		}
	}	
}


void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET3, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET3, m_strAlignerOffsetX3);
		}
		else
		{
			m_strAlignerOffsetX3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET3, m_strAlignerOffsetX3);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET3, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET3, m_strAlignerOffsetY3);
		}
		else
		{
			m_strAlignerOffsetY3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET3, m_strAlignerOffsetY3);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET3, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET3, m_strAlignerOffsetD3);
		}
		else
		{
			m_strAlignerOffsetD3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET3, m_strAlignerOffsetD3);
		}
	}	
}



void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET4, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET4, m_strAlignerOffsetX4);
		}
		else
		{
			m_strAlignerOffsetX4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET4, m_strAlignerOffsetX4);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET4, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET4, m_strAlignerOffsetY4);
		}
		else
		{
			m_strAlignerOffsetY4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET4, m_strAlignerOffsetY4);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET4, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET4, m_strAlignerOffsetD4);
		}
		else
		{
			m_strAlignerOffsetD4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET4, m_strAlignerOffsetD4);
		}
	}	
}



void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset5() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET5, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET5, m_strAlignerOffsetX5);
		}
		else
		{
			m_strAlignerOffsetX5.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET5, m_strAlignerOffsetX5);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset5() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET5, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET5, m_strAlignerOffsetY5);
		}
		else
		{
			m_strAlignerOffsetY5.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET5, m_strAlignerOffsetY5);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset5() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET5, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET5, m_strAlignerOffsetD5);
		}
		else
		{
			m_strAlignerOffsetD5.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET5, m_strAlignerOffsetD5);
		}
	}	
}



void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXOffset6() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET6, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET6, m_strAlignerOffsetX6);
		}
		else
		{
			m_strAlignerOffsetX6.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET6, m_strAlignerOffsetX6);

			int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerOffsetX.SetAt(nCurSel,m_strAlignerOffsetX6);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYOffset6() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET6, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET6, m_strAlignerOffsetY6);
		}
		else
		{
			m_strAlignerOffsetY6.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET6, m_strAlignerOffsetY6);

			int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerOffsetY.SetAt(nCurSel,m_strAlignerOffsetY6);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDOffset6() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET6, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET6, m_strAlignerOffsetD6);
		}
		else
		{
			m_strAlignerOffsetD6.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET6, m_strAlignerOffsetD6);

			int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerOffsetD.SetAt(nCurSel,m_strAlignerOffsetD6);
		}
	}	
}



void CDlgSysKindAlignSetting::OnKillfocusEditAlignerXBenchOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, m_strAlignerBenchOffsetX);
		}
		else
		{
			m_strAlignerBenchOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_BENCH_OFFSET, m_strAlignerBenchOffsetX);

			int nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerBenchOffsetX.SetAt(nCurSel,m_strAlignerBenchOffsetX);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerYBenchOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, m_strAlignerBenchOffsetY);
		}
		else
		{
			m_strAlignerBenchOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_BENCH_OFFSET, m_strAlignerBenchOffsetY);

			int nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerBenchOffsetY.SetAt(nCurSel,m_strAlignerBenchOffsetY);
		}
	}	
}

void CDlgSysKindAlignSetting::OnKillfocusEditAlignerDBenchOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, m_strAlignerBenchOffsetD);
		}
		else
		{
			m_strAlignerBenchOffsetD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_BENCH_OFFSET, m_strAlignerBenchOffsetD);

			int nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
			if (nCurSel<0)
			{
				return;
			}
			m_saAlignerBenchOffsetD.SetAt(nCurSel,m_strAlignerBenchOffsetD);
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
void CDlgSysKindAlignSetting::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
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

BOOL CDlgSysKindAlignSetting::DestroyWindow() 
{
// 	int i = 0;
// 	int nSize = m_vpTargetGuiDisplay.size();
// 	for (i=0; i<nSize; i++)
// 	{
// 		delete m_vpTargetGuiDisplay.at(i);
// 		m_vpTargetGuiDisplay.at(i) = NULL;
// 	}
// 	m_vpTargetGuiDisplay.clear();
// 
// 	nSize = m_vpObjectGuiDisplay.size();
// 	for (i=0; i<nSize; i++)
// 	{
// 		delete m_vpObjectGuiDisplay.at(i);
// 		m_vpObjectGuiDisplay.at(i) = NULL;
// 	}
// 	m_vpObjectGuiDisplay.clear();
// 
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
	DeleteObject(m_hBmpOffsetDir);
	return CDialog::DestroyWindow();
}

void CDlgSysKindAlignSetting::OnOK() 
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

void CDlgSysKindAlignSetting::OnCancel() 
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
//	CDialog::OnOK();

	CDialog::OnCancel();
}

void CDlgSysKindAlignSetting::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

// void CDlgSysKindAlignSetting::OnEnSetfocusEditTargetAndObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_TARGET_AND_OBJECT_DIS)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_TARGET_AND_OBJECT_DIS, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_AND_OBJECT_DIS, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[目标/对象最大距离差]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditTargetAndObjectDis();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditTargetDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_TARGET_DIS)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_TARGET_DIS, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_DIS, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查距离][目标距离]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditTargetDis();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_OBJECT_DIS)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_OBJECT_DIS, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_OBJECT_DIS, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查距离][对象距离]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditObjectDis();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditTargetAngle()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_TARGET_ANGLE)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_TARGET_ANGLE, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_ANGLE, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查目标角度][最大角度]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditTargetAngle();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisX1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_PIXEL_DIS_X1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查像素距离][最大距离][X1]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditPixelDisX1();
// 	}	
// }

// void CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisY1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_PIXEL_DIS_Y1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_Y1, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y1, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查像素距离][最大距离][Y1]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditPixelDisY1();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisX2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_PIXEL_DIS_X1)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查像素距离][最大距离][X2]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditPixelDisX1();
// 	}	
// }



// void CDlgSysKindAlignSetting::OnEnSetfocusEditPixelDisY2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_PIXEL_DIS_Y2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_Y2, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y2, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[检查像素距离][最大距离][Y2]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditPixelDisY2();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditTargetAndObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_TARGET_AND_OBJECT_DIS, strVal);
// 		if (!IsStringPositiveNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_AND_OBJECT_DIS, m_strTargetAndObjectDis);
// 		}
// 		else
// 		{
// 			m_strTargetAndObjectDis.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_TARGET_AND_OBJECT_DIS, m_strTargetAndObjectDis);
// 		}
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditTargetDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_TARGET_DIS, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_DIS, m_strTargetDis);
// 		}
// 		else
// 		{
// 			m_strTargetDis.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_TARGET_DIS, m_strTargetDis);
// 		}
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_OBJECT_DIS, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_OBJECT_DIS, m_strObjectDis);
// 		}
// 		else
// 		{
// 			m_strObjectDis.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_OBJECT_DIS, m_strObjectDis);
// 		}
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditTargetAngle()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_TARGET_ANGLE, strVal);
// 		if (!IsStringPositiveNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_TARGET_ANGLE, m_strTargetAngle);
// 		}
// 		else
// 		{
// 			m_strTargetAngle.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_TARGET_ANGLE, m_strTargetAngle);
// 		}
// 	}
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisX1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, m_strPixelDisX1);
// 		}
// 		else
// 		{
// 			m_strPixelDisX1.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X1, m_strPixelDisX1);
// 		}
// 	}
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisY1()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_Y1, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y1, m_strPixelDisY1);
// 		}
// 		else
// 		{
// 			m_strPixelDisY1.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y1, m_strPixelDisY1);
// 		}
// 	}
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisX2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_X2, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X2, m_strPixelDisX2);
// 		}
// 		else
// 		{
// 			m_strPixelDisX2.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_X2, m_strPixelDisX2);
// 		}
// 	}
// }

// void CDlgSysKindAlignSetting::OnEnKillfocusEditPixelDisY2()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_PIXEL_DIS_Y2, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y2, m_strPixelDisY2);
// 		}
// 		else
// 		{
// 			m_strPixelDisY2.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_PIXEL_DIS_Y2, m_strPixelDisY2);
// 		}
// 	}
// }


void CDlgSysKindAlignSetting::OnEnSetfocusEditSearchDelayTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_DELAY_TIME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_DELAY_TIME, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对位参数设置][目标搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditSearchDelayTime();
	}	
}


void CDlgSysKindAlignSetting::OnEnKillfocusEditSearchDelayTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nSearchDelayTime = (m_nSearchDelayTime<0 ? 0 : m_nSearchDelayTime);
		UpdateData(FALSE);
	}
}



void CDlgSysKindAlignSetting::OnEnSetfocusEditObjectSearchDelayTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对位参数设置][对象搜索延迟]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditObjectSearchDelayTime();
	}	
}


void CDlgSysKindAlignSetting::OnEnKillfocusEditObjectSearchDelayTime()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nObjectSearchDelayTime = (m_nObjectSearchDelayTime<0 ? 0 : m_nObjectSearchDelayTime);
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerOffsetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CPlatformOptionInfo     platformOptionInfo;
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	CProductData product;
	int nCurProdIdx = pTotalProduct->GetCurProductIndex();	
	product = pTotalProduct->GetItem(nCurProdIdx);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(platformOptionInfo);
	m_bSetOffsetTip = platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
	m_bShowOffsetTips = platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;
	int nCurSel = m_comboAlignerOffsetIndex.GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}
	m_saAlignerOffsetX.SetAt(m_nPrevAlignerOffsetIndex, m_strAlignerOffsetX6);
	m_saAlignerOffsetY.SetAt(m_nPrevAlignerOffsetIndex, m_strAlignerOffsetY6);
	m_saAlignerOffsetD.SetAt(m_nPrevAlignerOffsetIndex,  m_strAlignerOffsetD6);
	
	if (nCurSel != m_nPrevAlignerOffsetIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[其他补偿]由%d到%d\n"),m_nPrevAlignerOffsetIndex+6,nCurSel+6);
		g_opLogFile.SaveLogText(strLog,4);

		m_strAlignerOffsetX6 = m_saAlignerOffsetX.GetAt(nCurSel);
		m_strAlignerOffsetY6 = m_saAlignerOffsetY.GetAt(nCurSel);
		m_strAlignerOffsetD6 = m_saAlignerOffsetD.GetAt(nCurSel);

		if (m_bShowOffsetTips)
		{
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(m_bShowOffset);
			GetDlgItem(IDC_STATIC_TIP6)->SetWindowText(m_vstrOffSetTip[5+nCurSel]);
		}
		else
		{
			GetDlgItem(IDC_STATIC_TIP6)->ShowWindow(FALSE);
		}

		m_nPrevAlignerOffsetIndex= nCurSel;
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	UpdateDlgByAuthorize();
	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}


BOOL CDlgSysKindAlignSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


//20160510增加对角补偿
void CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerOffsetCoordType()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( (m_comboAlignerOffsetCoordType.GetCurSel() == 3) || ((m_comboAlignerOffsetCoordType.GetCurSel() == 4)) )
	{
		GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(m_bEnableOffset);
	//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_SHOW);
	}
	else
	{
		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		CPlatformOptionInfo optionInfo;
		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetPlatformOptionInfo(optionInfo);
		if (optionInfo.m_bEnableTargetAndObjectPositionVirtualMode)
		{
			GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(m_bEnableOffset);
		//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_SHOW);
		//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_HIDE);
		}

// 		GetDlgItem(IDC_BUTTON_OFFSET_CALCULATE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->ShowWindow(SW_HIDE);
	}

	//m_comboAlignerOffsetCoordType
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	int comboAlignerOffsetCoordTypeOld = (int)product.m_pAlignerParam->GetAlignerOffsetCoordType();
	CString strAlignerOffsetCoordTypeOld,m_strAlignerOffsetCoordType;
	m_comboAlignerOffsetCoordType.GetLBText(comboAlignerOffsetCoordTypeOld,strAlignerOffsetCoordTypeOld);
	m_comboAlignerOffsetCoordType.GetWindowText(m_strAlignerOffsetCoordType);
	//UpdateDlgData(TRUE);
	//CString strVal[] = {_T("Mark坐标"), _T("平台坐标"), _T("Mark与平台坐标"), _T("对角Mark坐标DO"), _T("对角Mark坐标D2")};
	if (comboAlignerOffsetCoordTypeOld != m_comboAlignerOffsetCoordType.GetCurSel())
	{
		CString strLog;
		//strLog.Format(_T("修改工位%d产品%d产品设置中[补偿设置->坐标类型]由%s到%s\n"),m_nPrevPlatformIdx,m_nCurProductIdx+1,strVal[m_comboAlignerOffsetCoordTypeOld],strVal[m_comboAlignerOffsetCoordType.GetCurSel()]);
		strLog.Format(_T("修改[补偿设置][坐标类型]由%s到%s\n"),strAlignerOffsetCoordTypeOld,m_strAlignerOffsetCoordType);
		g_opLogFile.SaveLogText(strLog,4);
	}
	
	UpdateData(TRUE);
}

//20160510增加对角补偿
void CDlgSysKindAlignSetting::OnBnClickedButtonOffsetCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgOffsetCalculate dlgOffsetCalculate;
	dlgOffsetCalculate.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgOffsetCalculate.EnableSysKeyboard(m_bSysKeyboardEnabled);

	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetPlatformOptionInfo(optionInfo);

	dlgOffsetCalculate.SetSysPlatformInfo(platformInfo);
	dlgOffsetCalculate.SetPlatformOptionInfo(optionInfo);

	dlgOffsetCalculate.m_dObjectL1 = m_dObjectL1;
	dlgOffsetCalculate.m_dObjectW1 = m_dObjectW1;
	dlgOffsetCalculate.m_dTargetL2 = m_dTargetL2;
	dlgOffsetCalculate.m_dTargetW2 = m_dTargetW2;
	dlgOffsetCalculate.m_dF1 = m_dF1;
	dlgOffsetCalculate.m_dF2 = m_dF2;
	dlgOffsetCalculate.m_bOffsetXYEnable = m_bOffsetXYEnable;
	dlgOffsetCalculate.m_nRadioPosMode = m_nPosMode;
	dlgOffsetCalculate.m_bEnableObjectPositionVirtualMode = m_bEnableObjectPositionVirtualMode;
	dlgOffsetCalculate.m_bEnableTargetPositionVirtualMode = m_bEnableTargetPositionVirtualMode;
	dlgOffsetCalculate.m_nObjectPositionVirtualMode = m_nObjectPositionVirtualMode;
	dlgOffsetCalculate.m_nTargetPositionVirtualMode = m_nTargetPositionVirtualMode;
	if (dlgOffsetCalculate.DoModal() == IDOK)
	{
		m_dObjectL1 = dlgOffsetCalculate.m_dObjectL1;
		m_dObjectW1 = dlgOffsetCalculate.m_dObjectW1;
		m_dTargetL2 = dlgOffsetCalculate.m_dTargetL2;
		m_dTargetW2 = dlgOffsetCalculate.m_dTargetW2;
		m_dF1 = dlgOffsetCalculate.m_dF1;
		m_dF2 = dlgOffsetCalculate.m_dF2;
		m_bOffsetXYEnable = dlgOffsetCalculate.m_bOffsetXYEnable;

		m_nPosMode  = dlgOffsetCalculate.m_nRadioPosMode;
		m_bEnableObjectPositionVirtualMode = dlgOffsetCalculate.m_bEnableObjectPositionVirtualMode;
		m_bEnableTargetPositionVirtualMode = dlgOffsetCalculate.m_bEnableTargetPositionVirtualMode;
		m_nObjectPositionVirtualMode = dlgOffsetCalculate.m_nObjectPositionVirtualMode;
		m_nTargetPositionVirtualMode = dlgOffsetCalculate.m_nTargetPositionVirtualMode;

		CString strInfo;
		//strInfo.Format(_T("对象:L1=%.2lf;W1=%.2lf/r/n/r/n目标:L2=%.2lf;W2=%.2lf"),m_dObjectL1,m_dObjectW1,m_dTargetL2,m_dTargetW2);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->SetWindowText(strInfo);
		strInfo.Format(_T("%s:L1=%.2lf;W1=%.2lf"),GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PRODUCT_OBJECT_SIZE),m_dObjectL1,m_dObjectW1);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE)->SetWindowText(strInfo);
		strInfo.Format(_T("%s:L2=%.2lf;W2=%.2lf"),GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PRODUCT_TARGET_SIZE),m_dTargetL2,m_dTargetW2);
		//GetDlgItem(IDC_STATIC_PRODUCT_SIZE2)->SetWindowText(strInfo);
	}
}

// 补偿设置（补偿的方向、X和Y补偿值对调）
void CDlgSysKindAlignSetting::OnBnClickedButtonProductOffset()
{
	// TODO: 在此添加控件通知处理程序代码

	CDlgProductOffsetSetting dlgProductOffsetSetting;
	dlgProductOffsetSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgProductOffsetSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);

//  	dlgProductOffsetSetting.m_nProductOffsetXDirection = m_nProductOffsetXDirection;
// 	dlgProductOffsetSetting.m_nProductOffsetYDirection = m_nProductOffsetYDirection;
// 	dlgProductOffsetSetting.m_nProductOffsetDDirection = m_nProductOffsetDDirection;
// 
// 	dlgProductOffsetSetting.m_nProductOffsetXSetting = m_nProductOffsetXSetting;
// 	dlgProductOffsetSetting.m_nProductOffsetYSetting = m_nProductOffsetYSetting;
// 	dlgProductOffsetSetting.m_nProductOffsetDSetting = m_nProductOffsetDSetting;

	dlgProductOffsetSetting.m_bEnableProductOffsetXDirection = m_bEnableProductOffsetXDirection;
	dlgProductOffsetSetting.m_bEnableProductOffsetYDirection = m_bEnableProductOffsetYDirection;
	dlgProductOffsetSetting.m_bEnableProductOffsetDDirection = m_bEnableProductOffsetDDirection;

	dlgProductOffsetSetting.m_bEnableProductOffsetXSetting = m_bEnableProductOffsetXSetting;
	dlgProductOffsetSetting.m_bEnableProductOffsetYSetting = m_bEnableProductOffsetYSetting;
	dlgProductOffsetSetting.m_bEnableProductOffsetDSetting = m_bEnableProductOffsetDSetting;

	if (dlgProductOffsetSetting.DoModal() == IDOK)
	{
// 		m_nProductOffsetXDirection = dlgProductOffsetSetting.m_nProductOffsetXDirection;
// 		m_nProductOffsetYDirection = dlgProductOffsetSetting.m_nProductOffsetYDirection;
// 		m_nProductOffsetDDirection = dlgProductOffsetSetting.m_nProductOffsetDDirection;
// 
// 		m_nProductOffsetXSetting = dlgProductOffsetSetting.m_nProductOffsetXSetting;
// 		m_nProductOffsetYSetting = dlgProductOffsetSetting.m_nProductOffsetYSetting;
// 		m_nProductOffsetDSetting = dlgProductOffsetSetting.m_nProductOffsetDSetting;

		m_bEnableProductOffsetXDirection = dlgProductOffsetSetting.m_bEnableProductOffsetXDirection;
		m_bEnableProductOffsetYDirection = dlgProductOffsetSetting.m_bEnableProductOffsetYDirection;
		m_bEnableProductOffsetDDirection = dlgProductOffsetSetting.m_bEnableProductOffsetDDirection;

		m_bEnableProductOffsetXSetting = dlgProductOffsetSetting.m_bEnableProductOffsetXSetting;
		m_bEnableProductOffsetYSetting = dlgProductOffsetSetting.m_bEnableProductOffsetYSetting;
		m_bEnableProductOffsetDSetting = dlgProductOffsetSetting.m_bEnableProductOffsetDSetting;

	}

}

		
		
void CDlgSysKindAlignSetting::OnKillfocusEditObjectSearchDelayTime2()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nObjectSearchDelayTime2 = (m_nObjectSearchDelayTime2<0 ? 0 : m_nObjectSearchDelayTime2);
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignSetting::OnSetfocusEditObjectSearchDelayTime2()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_SEARCH_DELAY_TIME2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对位参数设置][对象搜索延迟2]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditObjectSearchDelayTime();
	}
}

//
//void CDlgSysKindAlignSetting::OnEnSetfocusEditEighitAngle()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (m_bSysKeyboardEnabled)
//	{
//		m_bKillFocusEnabled = FALSE;
//		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
//
//		CRect rcWnd;
//		CString strInput, strOutput;
//		GetDlgItem(IDC_EDIT_EIGHIT_ANGLE)->GetWindowRect(&rcWnd);
//		GetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strInput);
//
//		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
//		{
//			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOutput);
//			if (strInput != strOutput)
//			{
//				CString strLog;
//				strLog.Format(_T("修改[八字型对位][八字角度]由%s到%s\n"),strInput,strOutput);
//				g_opLogFile.SaveLogText(strLog,4);
//			}
//		}
//		m_bKillFocusEnabled = TRUE;
//		OnEnKillfocusEditEighitAngle();
//	}
//}
//
//
//void CDlgSysKindAlignSetting::OnEnKillfocusEditEighitAngle()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (m_bKillFocusEnabled)
//	{
//		CString strVal;
//		CString strOldVal;
//		GetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strVal);
//		if (!IsStringNumerical(strVal))
//		{
//			
//			strOldVal.Format("%f",m_dEightShapeAngle);
//			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOldVal);
//		}
//		else
//		{
//			strOldVal.Format(_T("%.6f"), atof(strVal));
//			SetDlgItemText(IDC_EDIT_EIGHIT_ANGLE, strOldVal);
//		}
//	}
//}


void CDlgSysKindAlignSetting::OnEnSetfocusEditAlignerTargetXOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGET_X_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[使用目标补偿多次对位]的[X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAlignerTargetXOffset();
	}	
}


void CDlgSysKindAlignSetting::OnEnKillfocusEditAlignerTargetXOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, m_strAlnTargetOffsetX);
		}
		else
		{
			m_strAlnTargetOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_X_OFFSET, m_strAlnTargetOffsetX);
		}
	}
}


void CDlgSysKindAlignSetting::OnEnSetfocusEditAlignerTargetYOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[使用目标补偿多次对位]的[Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAlignerTargetYOffset();
	}	
}


void CDlgSysKindAlignSetting::OnEnKillfocusEditAlignerTargetYOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, m_strAlnTargetOffsetY);
		}
		else
		{
			m_strAlnTargetOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGET_Y_OFFSET, m_strAlnTargetOffsetY);
		}
	}
}


void CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerBenchOffsetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nCurSel = m_comboAlignerBenchOffsetIndex.GetCurSel();
	if (nCurSel < 0)
	{
		return;
	}
	m_saAlignerBenchOffsetX.SetAt(m_nPrevAlignerBenchOffsetIndex, m_strAlignerBenchOffsetX);
	m_saAlignerBenchOffsetY.SetAt(m_nPrevAlignerBenchOffsetIndex, m_strAlignerBenchOffsetY);
	m_saAlignerBenchOffsetD.SetAt(m_nPrevAlignerBenchOffsetIndex,  m_strAlignerBenchOffsetD);

	if (nCurSel != m_nPrevAlignerBenchOffsetIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[下料补偿]由%d到%d\n"),m_nPrevAlignerBenchOffsetIndex+1,nCurSel+1);
		g_opLogFile.SaveLogText(strLog,4);

		m_strAlignerBenchOffsetX = m_saAlignerBenchOffsetX.GetAt(nCurSel);
		m_strAlignerBenchOffsetY = m_saAlignerBenchOffsetY.GetAt(nCurSel);
		m_strAlignerBenchOffsetD = m_saAlignerBenchOffsetD.GetAt(nCurSel);

		m_nPrevAlignerBenchOffsetIndex= nCurSel;
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignSetting::OnKillfocusEditAlignerWieght4vs4()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{

		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, m_strAlignerWeight);
		}
		else
		{
			m_strAlignerWeight.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, m_strAlignerWeight);
		}

		double dWeight = atof(m_strAlignerWeight);

		if (dWeight < 0.0)
		{
			m_strAlignerWeight.Format(_T("%.6f"), atof(_T("0")));
			SetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, m_strAlignerWeight);
		}
		else if (dWeight > 1.0)
		{
			m_strAlignerWeight.Format(_T("%.6f"), atof(_T("1")));
			SetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, m_strAlignerWeight);
		}

	}
}


void CDlgSysKindAlignSetting::OnSetfocusEditAlignerWieght4vs4()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_WIEGHT_4VS4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对位模式设置][比例权重系数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerWieght4vs4();
	}
}


void CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner4vs4Type()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strSelText = _T("");
	m_comboAligner4Vs4Type.GetLBText(m_comboAligner4Vs4Type.GetCurSel(), strSelText);
	//if ( (m_comboAligner4Vs4Type.GetCurSel() == 9) || (m_comboAligner4Vs4Type.GetCurSel() == 10) )
	if ( (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_1234_WEIGHT)) 
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234_COMBINE_WEIGHT)) )
	{
		//jjh0601
		if (m_comboAlignerCirclePos.GetCount()==2)
		{
			m_comboAlignerCirclePos.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_3));//_T("位置3"));
			m_comboAlignerCirclePos.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_4));//_T("位置4"));
		}
		else if (m_comboAlignerCirclePos.GetCount()==3)  //jjh0611
		{
			m_comboAlignerCirclePos.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_4));//_T("位置4"));
		}
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->EnableWindow(m_bEnableAlignTypeSet);

	}
	else
	{
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(SW_HIDE);
	}
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);

	//lzk
	int comboAligner4Vs4TypeOld_TMP = (int)product.m_pAlignerParam->GetAligner4Vs4Type();
	int nComboNum = 0;
	if (m_pParent != NULL)
	{
		vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
		iter4Vs4 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.begin();		
		for (;iter4Vs4 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner4Vs4TypeInfo.end();iter4Vs4++)  
		{
			if (iter4Vs4->m_bOpenAligner)
			{
				if ((int)iter4Vs4->m_alignerType == comboAligner4Vs4TypeOld_TMP )
				{
					break;
				}
				nComboNum ++;
			}
		}
	}
	if(nComboNum>=m_comboAligner4Vs4Type.GetCount() || nComboNum<0) return ;

	int comboAligner4Vs4TypeOld = nComboNum;



	int nAlinger4Vs4Type;
	CString strAligner4Vs4TypeOld,m_strAligner4Vs4Type;
	m_comboAligner4Vs4Type.GetLBText(comboAligner4Vs4TypeOld, strAligner4Vs4TypeOld);
	nAlinger4Vs4Type = m_comboAligner4Vs4Type.GetItemData(m_comboAligner4Vs4Type.GetCurSel());
	m_comboAligner4Vs4Type.GetWindowText(m_strAligner4Vs4Type);
	if (comboAligner4Vs4TypeOld != nAlinger4Vs4Type)
	{
		CString strLog;
		strLog.Format(_T("修改[4VS4对位模式]由%s到%s\n"),strAligner4Vs4TypeOld,m_strAligner4Vs4Type);
		g_opLogFile.SaveLogText(strLog,4);
	}


	//UpdateDlgData(TRUE);
}

void CDlgSysKindAlignSetting::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableObjectSearchDelayTime2Old = m_bEnableObjectSearchDelayTime2;

	UpdateData(TRUE);
	if (bEnableObjectSearchDelayTime2Old != m_bEnableObjectSearchDelayTime2)
	{
		CString strLog;
		CString strValue = m_bEnableObjectSearchDelayTime2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[对象搜索延时2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignSetting::OnBnClickedCheckTargetAndObjectDis()
{
	// TODO: 在此添加控件通知处理程序代码m_bCheckTargetAndObjectDis

	BOOL bCheckTargetAndObjectDisOld = m_bCheckTargetAndObjectDis;

	UpdateData(TRUE);

	int nCurSel = m_comboMultiCalibExtension.GetCurSel();
	if (nCurSel<0)
	{
		return;
	}

	CString strInfo = _T("");
	strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
	m_saMult_CheckTargetAndObjectDis.SetAt(nCurSel,strInfo);

	if (bCheckTargetAndObjectDisOld != m_bCheckTargetAndObjectDis)
	{
		CString strLog;
		CString strValue = m_bCheckTargetAndObjectDis ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[检查设置][搜索距离是否有效]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


// void CDlgSysKindAlignSetting::OnBnClickedCheckTargetAndObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码m_bCheckTargetAndObjectDis
// 	BOOL bCheckTargetAndObjectDisOld = m_bCheckTargetAndObjectDis;
// 
// 	UpdateData(TRUE);
// 
// 	int nCurSel = m_comboMultiCalibExtension.GetCurSel();
// 	if (nCurSel<0)
// 	{
// 		return;
// 	}
// 	CString tmp = _T("");
// 	tmp.Format(_T("%d"),m_bCheckTargetAndObjectDis);
// 	m_saAMultiCalib_CheckTargetAndObjectDis.SetAt(nCurSel,tmp);
// 
// 	if (bCheckTargetAndObjectDisOld != m_bCheckTargetAndObjectDis)
// 	{
// 		CString strLog;
// 		CString strValue = m_bCheckTargetAndObjectDis ? _T("选中状态"):_T("未选中状态");
// 		strLog.Format(_T("修改[检查设置][搜索距离是否有效]为%s\n"),strValue);
// 		g_opLogFile.SaveLogText(strLog,4);
// 	}
// }


void CDlgSysKindAlignSetting::OnBnClickedChkSalnOffset()
{
	// TODO: 在此添加控件通知处理程序代码m_bCheckTargetOffsetEnable
	BOOL bCheckTargetOffsetEnableOld = m_bCheckTargetOffsetEnable;

	UpdateData(TRUE);
	if (bCheckTargetOffsetEnableOld != m_bCheckTargetOffsetEnable)
	{
		CString strLog;
		CString strValue = m_bCheckTargetOffsetEnable ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[使用目标补偿多次对位]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


// void CDlgSysKindAlignSetting::OnBnClickedCheckTargetAngle()
// {
// 	// TODO: 在此添加控件通知处理程序代码m_bCheckTargetAngle
// 	BOOL bCheckTargetAngleOld = m_bCheckTargetAngle;
// 
// 	UpdateData(TRUE);
// 	if (bCheckTargetAngleOld != m_bCheckTargetAngle)
// 	{
// 		CString strLog;
// 		CString strValue = m_bCheckTargetAngle ? _T("选中状态"):_T("未选中状态");
// 		strLog.Format(_T("修改[检查设置][检查目标]为%s\n"),strValue);
// 		g_opLogFile.SaveLogText(strLog,4);
// 	}
// }


void CDlgSysKindAlignSetting::OnBnClickedCheckTargetShutter()
{
	//// TODO: 在此添加控件通知处理程序代码m_bTargetSearchShutter
	//BOOL bTargetSearchShutterOld = m_bTargetSearchShutter;

	//UpdateDlgData(TRUE);
	//if (bTargetSearchShutterOld != m_bTargetSearchShutter)
	//{
	//	CString strLog;
	//	CString strValue = m_bTargetSearchShutter ? _T("选中状态"):_T("未选中状态");
	//	strLog.Format(_T("修改[定位模板曝光设置][目标模板曝光设置]为%s\n"),strValue);
	//	g_opLogFile.SaveLogText(strLog,4);
	//}
}


void CDlgSysKindAlignSetting::OnBnClickedCheckObjectShutter()
{
	//// TODO: 在此添加控件通知处理程序代码m_bObjectSearchShutter
	//BOOL bObjectSearchShutterOld = m_bObjectSearchShutter;

	//UpdateDlgData(TRUE);
	//if (bObjectSearchShutterOld != m_bObjectSearchShutter)
	//{
	//	CString strLog;
	//	CString strValue = m_bObjectSearchShutter ? _T("选中状态"):_T("未选中状态");
	//	strLog.Format(_T("修改[定位模板曝光设置][对象模板曝光设置]为%s\n"),strValue);
	//	g_opLogFile.SaveLogText(strLog,4);
	//}
}


//void CDlgSysKindAlignSetting::OnBnClickedCheckMulMarkMode()
//{
//	// TODO: 在此添加控件通知处理程序代码m_bDiffPosWithSameSearchResultEnabled
//	BOOL bDiffPosWithSameSearchResultEnabledOld = m_bDiffPosWithSameSearchResultEnabled;
//
//	UpdateData(TRUE);
//	if (bDiffPosWithSameSearchResultEnabledOld != m_bDiffPosWithSameSearchResultEnabled)
//	{
//		CString strLog;
//		CString strValue = m_bDiffPosWithSameSearchResultEnabled ? _T("选中状态"):_T("未选中状态");
//		strLog.Format(_T("修改[多标记点模式设置][多标记点模式有效]为%s\n"),strValue);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}


//void CDlgSysKindAlignSetting::OnBnClickedCheckEnablemutitarget()
//{
//	// TODO: 在此添加控件通知处理程序代码m_bEnableMutiTarget
//	BOOL bEnableMutiTargetOld = m_bEnableMutiTarget;
//
//	UpdateData(TRUE);
//	if (bEnableMutiTargetOld != m_bEnableMutiTarget)
//	{
//		CString strLog;
//		CString strValue = m_bEnableMutiTarget ? _T("选中状态"):_T("未选中状态");
//		strLog.Format(_T("修改[多目标对位设置][多目标对位有效]为%s\n"),strValue);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}


void CDlgSysKindAlignSetting::OnBnClickedCheckAlignerAngleVertical()
{
	// TODO: 在此添加控件通知处理程序代码m_bAlignAngleVertical
	BOOL bAlignAngleVerticalOld = m_bAlignAngleVertical;

	UpdateData(TRUE);
	if (bAlignAngleVerticalOld != m_bAlignAngleVertical)
	{
		CString strLog;
		CString strValue = m_bAlignAngleVertical ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[对位对象与目标Mark角度垂直]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


//void CDlgSysKindAlignSetting::OnBnClickedCheckEightShapeAlign()
//{
//	// TODO: 在此添加控件通知处理程序代码m_bCheckEightShapeAlign
//	BOOL bCheckEightShapeAlignOld = m_bCheckEightShapeAlign;
//
//	UpdateData(TRUE);
//	if (bCheckEightShapeAlignOld != m_bCheckEightShapeAlign)
//	{
//		CString strLog;
//		CString strValue = m_bCheckEightShapeAlign ? _T("选中状态"):_T("未选中状态");
//		strLog.Format(_T("修改[启用八字型对位]为%s\n"),strValue);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}

//void CDlgSysKindAlignSetting::OnBnClickedCheckTargetDis()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	BOOL bCheckTargetDisOld = m_bCheckTargetDis;

//	UpdateData(TRUE);
//	if (bCheckTargetDisOld != m_bCheckTargetDis)
//	{
//		CString strLog;
//		CString strValue = m_bCheckTargetDis ? _T("选中状态"):_T("未选中状态");
//		strLog.Format(_T("修改[检查设置][目标距离]为%s\n"),strValue);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}



// void CDlgSysKindAlignSetting::OnBnClickedCheckObjectDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码m_bCheckObjectDis
// 	BOOL bCheckObjectDisOld = m_bCheckObjectDis;
// 
// 	UpdateData(TRUE);
// 	if (bCheckObjectDisOld != m_bCheckObjectDis)
// 	{
// 		CString strLog;
// 		CString strValue = m_bCheckObjectDis ? _T("选中状态"):_T("未选中状态");
// 		strLog.Format(_T("修改[检查设置][对象距离]为%s\n"),strValue);
// 		g_opLogFile.SaveLogText(strLog,4);
// 	}
//}


void CDlgSysKindAlignSetting::OnAlignCheckModeChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCheckModeOld = m_nCheckMode;
	UpdateData(TRUE);

	CString strVal[] = {_T("输入模式"), _T("检查模式")};
	if (nCheckModeOld != m_nCheckMode)
	{
		CString strLog;
		strLog.Format(_T("修改[输入/检查模式]由%s到%s\n"),strVal[nCheckModeOld],strVal[m_nCheckMode]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignSetting::OnAlignerAngleVerticalChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAlignAngleVerticalModeOld = m_nAlignAngleVerticalMode;
	UpdateData(TRUE);

	CString strVal[] = {_T("+90°"), _T("-90°")};
	if (nAlignAngleVerticalModeOld != m_nAlignAngleVerticalMode)
	{
		CString strLog;
		strLog.Format(_T("修改[对象到目标Mark角度]由%s到%s\n"),strVal[nAlignAngleVerticalModeOld],strVal[m_nAlignAngleVerticalMode]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

//void CDlgSysKindAlignSetting::OnAlignCheckEightModeChange()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	int nEightShapeAlignModeOld = m_nEightShapeAlignMode;
//	UpdateData(TRUE);
//
//	CString strVal[] = {_T("顺八字"), _T("逆八字")};
//	if (nEightShapeAlignModeOld != m_nEightShapeAlignMode)
//	{
//		CString strLog;
//		strLog.Format(_T("修改[启用八字对位]由%s到%s\n"),strVal[nEightShapeAlignModeOld],strVal[m_nEightShapeAlignMode]);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}



//void CDlgSysKindAlignSetting::OnBnClickedCheckPixelDis()
//{
//	// TODO: 在此添加控件通知处理程序代码m_bCheckPixelDis
//	BOOL bCheckPixelDisOld = m_bCheckPixelDis;
//	
//	UpdateData(TRUE);
//	if (bCheckPixelDisOld != m_bCheckPixelDis)
//
//
//	{
//		CString strLog;
//		CString strValue = m_bCheckPixelDis ? _T("选中状态"):_T("未选中状态");
//		strLog.Format(_T("修改[检查象素距离]为%s\n"),strValue);
//		g_opLogFile.SaveLogText(strLog,4);
//	}
//}

void CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner2vs2Type()
{
	// TODO: 在此添加控件通知处理程序代码

	//jjh0601
	CString strSelText = _T("");
	m_comboAligner2Vs2Type.GetLBText(m_comboAligner2Vs2Type.GetCurSel(), strSelText);
	if ( (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_AUTO_WEIGHT)) 
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C1_WEIGHT))
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_2VS2_TYPE_POS12_12C2_WEIGHT)))
	{
		if (m_comboAlignerCirclePos.GetCount()==4)
		{
			m_comboAlignerCirclePos.DeleteString(3);//_T("位置4"));
			m_comboAlignerCirclePos.DeleteString(2);//_T("位置3"));

		}
		else if (m_comboAlignerCirclePos.GetCount()==3)  //jjh0611
		{
			m_comboAlignerCirclePos.DeleteString(2);//_T("位置3"));
		}
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->EnableWindow(m_bEnableAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(SW_HIDE);
	}

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);



	//lzk
	int comboAligner2Vs2TypeOld_TMP = (int)product.m_pAlignerParam->GetAligner2Vs2Type();
	int nComboNum = 0;
	if (m_pParent != NULL)
	{
		vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
		iter2Vs2 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.begin();	
		
		for (;iter2Vs2 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner2Vs2TypeInfo.end();iter2Vs2++)  
		{
			if (iter2Vs2->m_bOpenAligner)
			{
				
				if ((int)iter2Vs2->m_alignerType == comboAligner2Vs2TypeOld_TMP )
				{
					break;
				}
				nComboNum ++;
			}
		}
	}
	if(nComboNum>=m_comboAligner2Vs2Type.GetCount() || nComboNum<0) return ;

	int comboAligner2Vs2TypeOld = nComboNum;

	int nAlinger2Vs2Type;
	nAlinger2Vs2Type = m_comboAligner2Vs2Type.GetItemData(m_comboAligner2Vs2Type.GetCurSel());
	CString strAlinger2Vs2TypeOld, m_strAlinger2Vs2Type;
	m_comboAligner2Vs2Type.GetLBText(comboAligner2Vs2TypeOld,strAlinger2Vs2TypeOld);
	m_comboAligner2Vs2Type.GetWindowText(m_strAlinger2Vs2Type);


	if (comboAligner2Vs2TypeOld != nAlinger2Vs2Type)
	{
		CString strLog;
		strLog.Format(_T("修改[2VS2对位模式]由%s到%s\n"),strAlinger2Vs2TypeOld,m_strAlinger2Vs2Type);
		g_opLogFile.SaveLogText(strLog,4);
	}



}

void CDlgSysKindAlignSetting::OnCbnSelchangeComboAligner3vs3Type()
{
	// TODO: 在此添加控件通知处理程序代码
	//jjh0611
	CString strSelText = _T("");
	m_comboAligner3Vs3Type.GetLBText(m_comboAligner3Vs3Type.GetCurSel(), strSelText);
	if ( (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_WEIGHT)) 
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C1_WEIGHT))
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_123C2_WEIGHT))
		|| (strSelText == GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_3VS3_TYPE_POS123_AUTO_L1C_WEIGHT))) //jjh0619
	{
		if (m_comboAlignerCirclePos.GetCount()==4)
		{
			m_comboAlignerCirclePos.DeleteString(3);//_T("位置4"));
			//m_comboAlignerCirclePos.DeleteString(2);//_T("位置3"));

		}
		else if (m_comboAlignerCirclePos.GetCount()==2)  //jjh0611
		{
			m_comboAlignerCirclePos.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_ALIGNER_4VS4_CIRCLE_POS_3));//_T("位置3"));
		}
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(m_bShowAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(m_bShowAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->EnableWindow(m_bEnableAlignTypeSet);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->EnableWindow(m_bEnableAlignTypeSet);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->EnableWindow(m_bEnableAlignTypeSet);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALIGNER_WIEGHT_4VS4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)->ShowWindow(SW_HIDE);
	}



	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);

	//lzk
	int comboAligner3Vs3TypeOld_TMP = (int)product.m_pAlignerParam->GetAligner3Vs3Type();
	int nComboNum = 0;
	if (m_pParent != NULL)
	{
		vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
		iter3Vs3 = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.begin();		
		for (;iter3Vs3 != m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlignerTypeInfo().m_vAligner3Vs3TypeInfo.end();iter3Vs3++)  
		{
			if (iter3Vs3->m_bOpenAligner)
			{
				if ((int)iter3Vs3->m_alignerType == comboAligner3Vs3TypeOld_TMP )
				{
					break;
				}
				nComboNum ++;
			}
		}

	}
	if(nComboNum>=m_comboAligner3Vs3Type.GetCount() || nComboNum<0) return ;

	int comboAligner3Vs3TypeOld = nComboNum;


	int nAlinger3Vs3Type;
	nAlinger3Vs3Type = m_comboAligner3Vs3Type.GetItemData(m_comboAligner3Vs3Type.GetCurSel());
	CString strAligner3Vs3TypeOld,m_strAligner3Vs3Type;
	m_comboAligner3Vs3Type.GetLBText(comboAligner3Vs3TypeOld,strAligner3Vs3TypeOld);
	m_comboAligner3Vs3Type.GetWindowText(m_strAligner3Vs3Type);	
	if (comboAligner3Vs3TypeOld != nAlinger3Vs3Type)
	{
		CString strLog;
		strLog.Format(_T("修改[3VS3对位模式]由%s到%s\n"),strAligner3Vs3TypeOld,m_strAligner3Vs3Type);
		g_opLogFile.SaveLogText(strLog,4);
	}

}

void CDlgSysKindAlignSetting::OnCbnSelchangeComboAlignerMoveType()
{
	// TODO: 在此添加控件通知处理程序代码m_comboAlignerMoveType
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	int comboAlignerMoveTypeOld = (int)product.m_pAlignerParam->GetAlignerMoveType();
	CString strAlignerMoveTypeOld,m_strAlignerMoveType;
	m_comboAlignerMoveType.GetLBText(comboAlignerMoveTypeOld, strAlignerMoveTypeOld);
	m_comboAlignerMoveType.GetWindowText(m_strAlignerMoveType);
	if (comboAlignerMoveTypeOld != m_comboAlignerMoveType.GetCurSel())
	{
		CString strLog;
		strLog.Format(_T("修改[轴移动类型]由%s到%s\n"),strAlignerMoveTypeOld,m_strAlignerMoveType);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

// 专用焊接机参数

// 1. 运动量系数X
// void CDlgSysKindAlignSetting::OnEnSetfocusEditMovementRatioX()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// /*	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_X)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_X, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_X, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditMovementRatioX();
// 	}*/	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditMovementRatioX()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// /*	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_X, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_X, m_strMovementRatioX);
// 		}
// 		else
// 		{
// 			m_strMovementRatioX.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_X, m_strMovementRatioX);
// 		}
// 	}*/	
// }


// 1. 运动量系数Y
// void CDlgSysKindAlignSetting::OnEnSetfocusEditMovementRatioY()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	/*if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_MOVEMENT_RATIO_Y)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_Y, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_Y, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditMovementRatioY();
// 	}*/
// 
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditMovementRatioY()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	/*if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_Y, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_Y, m_strMovementRatioY);
// 		}
// 		else
// 		{
// 			m_strMovementRatioY.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_MOVEMENT_RATIO_Y, m_strMovementRatioY);
// 		}
// 	}	*/
// }



// 2. 补偿系数X
void CDlgSysKindAlignSetting::OnSetfocusEditAmmendMovementRatioX()
{
	// TODO: 在此添加控件通知处理程序代码

	/*if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAmmendMovementRatioX();
	}*/

}

void CDlgSysKindAlignSetting::OnKillfocusEditAmmendMovementRatioX()
{
	// TODO: 在此添加控件通知处理程序代码

	/*if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, m_strAmmendMoveRatioX);
		}
		else
		{
			m_strAmmendMoveRatioX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_X, m_strAmmendMoveRatioX);
		}
	}*/
}


// 2. 补偿系数Y
void CDlgSysKindAlignSetting::OnSetfocusEditAmmendMovementRatioY()
{
	// TODO: 在此添加控件通知处理程序代码

	/*if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAmmendMovementRatioY();
	}*/
}

void CDlgSysKindAlignSetting::OnKillfocusEditAmmendMovementRatioY()
{
	// TODO: 在此添加控件通知处理程序代码

	/*if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, m_strAmmendMoveRatioY);
		}
		else
		{
			m_strAmmendMoveRatioY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AMMEND_MOVEMENT_RATIO_Y, m_strAmmendMoveRatioY);
		}
	}*/
}




void CDlgSysKindAlignSetting::OnBnClickedCheckEnableSendOffset()
{
	BOOL bEnableSendTargetOffsetToPLCOld = m_bEnableSendTargetOffsetToPLC;

	UpdateData(TRUE);
	if (bEnableSendTargetOffsetToPLCOld != m_bEnableSendTargetOffsetToPLC)
	{
		CString strLog;
		CString strValue = m_bEnableSendTargetOffsetToPLC ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[发送补偿PLC]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}
void CDlgSysKindAlignSetting::OnBnClickedBtnTarFixOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[目标固定补偿]进入[目标固定补偿界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);
	CDlgTargetFixOffsetSetting dlg;
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);

	dlg.m_bEnableTargetFixOffset = m_bEnableTarFixOffset;
	dlg.m_nTargetOffsetType = m_nTarFixOffsetType;


	CString str;
	for (int i=0;i<ALIGNOFFSETNUM;i++)
	{
		str = m_saTarFixOffsetX.GetAt(i);
		dlg.m_saTarFixOffsetX.SetAt(i,str);
		str = m_saTarFixOffsetY.GetAt(i);
		dlg.m_saTarFixOffsetY.SetAt(i,str);
		str = m_saTarFixOffsetD.GetAt(i);
		dlg.m_saTarFixOffsetD.SetAt(i,str);
	}
	//dlg.m_saTarFixOffsetX = m_saTarFixOffsetX;
	//dlg.m_saTarFixOffsetY = m_saTarFixOffsetY;
	//dlg.m_saTarFixOffsetD = m_saTarFixOffsetD;
	if (dlg.DoModal() == IDOK)
	{
		m_bEnableTarFixOffset = dlg.m_bEnableTargetFixOffset;
		m_nTarFixOffsetType = dlg.m_nTargetOffsetType;

		for (int i=0;i<ALIGNOFFSETNUM;i++)
		{
			str = dlg.m_saTarFixOffsetX.GetAt(i);
			m_saTarFixOffsetX.SetAt(i,str);
			str = dlg.m_saTarFixOffsetY.GetAt(i);
			m_saTarFixOffsetY.SetAt(i,str);
			str = dlg.m_saTarFixOffsetD.GetAt(i);
			m_saTarFixOffsetD.SetAt(i,str);
		}
		/*m_saTarFixOffsetX = dlg.m_saTarFixOffsetX;
		m_saTarFixOffsetY = dlg.m_saTarFixOffsetY;
		m_saTarFixOffsetD = dlg.m_saTarFixOffsetD;*/
	}
}


void CDlgSysKindAlignSetting::OnBnClickedBtnAlignOffsetSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[对位偏差方向设置]进入[对位偏差方向设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);

	CDlgAlignOffsetDirectionSetting dlg;
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.m_bReverseX = m_bAlignOffsetReverseX;
	dlg.m_bReverseY = m_bAlignOffsetReverseY;
	dlg.m_bReverseD = m_bAlignOffsetReverseD;
	if (dlg.DoModal() == IDOK)
	{
		m_bAlignOffsetReverseX = dlg.m_bReverseX;
		m_bAlignOffsetReverseY = dlg.m_bReverseY;
		m_bAlignOffsetReverseD = dlg.m_bReverseD;
	}
}

void CDlgSysKindAlignSetting::OnCbnSelchangeComboExproductIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nSel = m_comboMultiCalibExtension.GetCurSel();
	if (nSel < 0)
	{
		return;
	}

	CString strInfo = _T("");

	strInfo.Format(_T("%d"),m_bCheckTargetAndObjectDis);
	m_saMult_CheckTargetAndObjectDis.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);

	strInfo.Format(_T("%d"),m_nCheckMode);
	m_saMult_CheckMode.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);



	strInfo.Format(_T("%d"),m_bTargetDisThre);
	m_saMult_CheckTargetDisThre.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectDisThre);
	m_saMult_CheckObjectDisThre.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);

	strInfo.Format(_T("%d"),m_bTargetAngleThre);
	m_saMult_CheckTargetAngleThre.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);

	strInfo.Format(_T("%d"),m_bObjectAngleThre);
	m_saMult_CheckObjectAngleThre.SetAt(m_nPreMultiCalibExtensionIndex, strInfo);

	m_saMult_TargetDisThre.SetAt(m_nPreMultiCalibExtensionIndex,m_strTargetDisThre);
	m_saMult_ObjectDisThre.SetAt(m_nPreMultiCalibExtensionIndex,m_strObjectDisThre);
	m_saMult_TargetAngleThre.SetAt(m_nPreMultiCalibExtensionIndex,m_strTargetAngleThre);
	m_saMult_ObjectAngleThre.SetAt(m_nPreMultiCalibExtensionIndex,m_strObjectAngleThre);

	if (nSel != m_nPreMultiCalibExtensionIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[扩展平台]由%d到%d\n"),m_nPrevAlignerOffsetIndex,nSel);
		g_opLogFile.SaveLogText(strLog,4);


		int i = nSel;

		m_bCheckTargetAndObjectDis = atoi(m_saMult_CheckTargetAndObjectDis.GetAt(i));
		m_nCheckMode = atoi(m_saMult_CheckMode.GetAt(i));

		m_bTargetDisThre = atoi(m_saMult_CheckTargetDisThre.GetAt(i));
		m_bObjectDisThre = atoi(m_saMult_CheckObjectDisThre.GetAt(i));
		m_bTargetAngleThre = atoi(m_saMult_CheckTargetAngleThre.GetAt(i));
		m_bObjectAngleThre = atoi(m_saMult_CheckObjectAngleThre.GetAt(i));

		m_strTargetDisThre = m_saMult_TargetDisThre.GetAt(i);
		m_strObjectDisThre = m_saMult_ObjectDisThre.GetAt(i);
		m_strTargetAngleThre = m_saMult_TargetAngleThre.GetAt(i);
		m_strObjectAngleThre = m_saMult_ObjectAngleThre.GetAt(i);

		m_nPreMultiCalibExtensionIndex= nSel;

		UpdateData(FALSE);
	}
}

void CDlgSysKindAlignSetting::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPlatformOptionInfo     platformOptionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(platformOptionInfo);
	m_bSetOffsetTip = platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
	m_bShowOffsetTips = platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;
	if(/*m_bSysKeyboardEnabled && */m_bShowOffsetTips && m_bSetOffsetTip)
		ShowTipSysKeyboard(point);
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

//iyo
void CDlgSysKindAlignSetting::OnSelchangeComboAligner4vs4Circlepos()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码m_comboAlignerMoveType
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);
	CProductData product;
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	int comboAlignerCirclePosOld = (int)product.m_pAlignerParam->GetAlignerCirclePos();
	CString strAlignerCirclePosOld,m_strAlignerCirclePos;
	m_comboAlignerCirclePos.GetLBText(comboAlignerCirclePosOld, strAlignerCirclePosOld);
	m_comboAlignerCirclePos.GetWindowText(m_strAlignerCirclePos);
	if (comboAlignerCirclePosOld != m_comboAlignerCirclePos.GetCurSel())
	{
		CString strLog;
		strLog.Format(_T("修改[圆心位置]由%s到%s\n"),strAlignerCirclePosOld,m_strAlignerCirclePos);
		g_opLogFile.SaveLogText(strLog,4);
	}

}

void CDlgSysKindAlignSetting::LoadOffsetDirectionPicture()
{
	//TCHAR szPath[MAX_PATH];
	//GetModuleFileName(NULL, szPath, MAX_PATH);

	//CString strCurWorkDir = szPath;
	//strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	CPlatformOptionInfo optionInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	//HBITMAP hbmp;

	CString strEXEDir;
	CString strDir;
	CFileFind findFile;
	BOOL IsFild, IsDir;
	SysPlatformInfo platInfo;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\'));   
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platInfo);
	strDir = strEXEDir + platInfo.m_strPlatformDetailSavePath; //Bin\VSConfig\WS0
	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (IsDir)
	{
		CRect rc;
		GetDlgItem(IDC_STATIC_OFFSET_DIRECTION)->GetClientRect(rc);
		strDir = strDir + _T("\\")+optionInfo.m_strOffsetPicturePath;
		DeleteObject(m_hBmpOffsetDir);
		m_hBmpOffsetDir = NULL;
		m_hBmpOffsetDir = (HBITMAP)::LoadImage(NULL,strDir,IMAGE_BITMAP,rc.Width(),rc.Height(),LR_LOADFROMFILE|LR_DEFAULTSIZE);
		if (m_hBmpOffsetDir)
		{
			GetDlgItem(IDC_STATIC_OFFSET_DIRECTION)->ShowWindow(m_bShowAlignOther);
			((CStatic*)GetDlgItem(IDC_STATIC_OFFSET_DIRECTION))->SetBitmap((HBITMAP)m_hBmpOffsetDir);
		}
		else
		{
			GetDlgItem(IDC_STATIC_OFFSET_DIRECTION)->ShowWindow(SW_HIDE);
		}
	}
}

// void CDlgSysKindAlignSetting::OnBnClickedCheckObjtarSeparateCheck()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 	if (m_bEnableTarObjSeparateCheck)
// 	{
// 		GetDlgItem(IDC_EDIT_OBJECT_CHECK_DIS)->ShowWindow(SW_SHOW);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_EDIT_OBJECT_CHECK_DIS)->ShowWindow(SW_HIDE);
// 	}
// }


// void CDlgSysKindAlignSetting::OnEnSetfocusEditObjectCheckDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_OBJECT_CHECK_DIS)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_OBJECT_CHECK_DIS, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_OBJECT_CHECK_DIS, strOutput);
// 			if (strInput != strOutput)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改[对象最大距离差]由%s到%s\n"),strInput,strOutput);
// 				g_opLogFile.SaveLogText(strLog,4);
// 			}
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditObjectCheckDis();
// 	}	
// }


// void CDlgSysKindAlignSetting::OnEnKillfocusEditObjectCheckDis()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_OBJECT_CHECK_DIS, strVal);
// 		if (!IsStringPositiveNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_OBJECT_CHECK_DIS, m_strObjectCheckDis);
// 		}
// 		else
// 		{
// 			m_strObjectCheckDis.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_OBJECT_CHECK_DIS, m_strObjectCheckDis);
// 		}
// 	}	
// }


void CDlgSysKindAlignSetting::OnSetfocusEditTargetDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_DIS_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_DIS_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_DIS_THRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标最大距离差]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetDisThre();
	}
}


void CDlgSysKindAlignSetting::OnKillfocusEditTargetDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_TARGET_DIS_THRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_DIS_THRE, m_strTargetDisThre);
		}
		else
		{
			m_strTargetDisThre.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_DIS_THRE, m_strTargetDisThre);
		}
	}
}



void CDlgSysKindAlignSetting::OnSetfocusEditObjectDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OBJECT_DIS_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OBJECT_DIS_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_DIS_THRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象最大距离差]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectDisThre();
	}
}


void CDlgSysKindAlignSetting::OnKillfocusEditObjectDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OBJECT_DIS_THRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_DIS_THRE, m_strObjectDisThre);
		}
		else
		{
			m_strObjectDisThre.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OBJECT_DIS_THRE, m_strObjectDisThre);
		}
	}
}


void CDlgSysKindAlignSetting::OnSetfocusEditTargetAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_ANGLE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_ANGLE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_ANGLE_THRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[目标最大角度差]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetAngleThre();
	}
}


void CDlgSysKindAlignSetting::OnKillfocusEditTargetAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_TARGET_ANGLE_THRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_ANGLE_THRE, m_strTargetAngleThre);
		}
		else
		{
			m_strTargetAngleThre.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_ANGLE_THRE, m_strTargetAngleThre);
		}
	}
}


void CDlgSysKindAlignSetting::OnSetfocusEditObjectAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OBJECT_ANGLE_THRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OBJECT_ANGLE_THRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_ANGLE_THRE, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[对象最大角度差]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectAngleThre();
	}
}


void CDlgSysKindAlignSetting::OnKillfocusEditObjectAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OBJECT_ANGLE_THRE, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_ANGLE_THRE, m_strObjectAngleThre);
		}
		else
		{
			m_strObjectAngleThre.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OBJECT_ANGLE_THRE, m_strObjectAngleThre);
		}
	}
}


void CDlgSysKindAlignSetting::OnBnClickedCheckTargetDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	// TODO: 在此添加控件通知处理程序代码
	BOOL bTargetDisThreOld = m_bTargetDisThre;

	UpdateData(TRUE);
	if (bTargetDisThreOld != m_bTargetDisThre)
	{
		CString strLog;
		CString strValue = m_bTargetDisThre ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[检查设置][目标最大距离差]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}

}


void CDlgSysKindAlignSetting::OnBnClickedCheckObjectDisThre()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bObjectDisThreOld = m_bObjectDisThre;

	UpdateData(TRUE);
	if (bObjectDisThreOld != m_bObjectDisThre)
	{
		CString strLog;
		CString strValue = m_bObjectDisThre ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[检查设置][对象最大距离差]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}

}


void CDlgSysKindAlignSetting::OnBnClickedCheckTargetAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bTargetAngleThreOld = m_bTargetAngleThre;

	UpdateData(TRUE);
	if (bTargetAngleThreOld != m_bTargetAngleThre)
	{
		CString strLog;
		CString strValue = m_bTargetAngleThre ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[检查设置][目标最大角度差]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignSetting::OnBnClickedCheckObjectAngleThre()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bObjectAngleThreOld = m_bObjectAngleThre;

	UpdateData(TRUE);
	if (bObjectAngleThreOld != m_bObjectAngleThre)
	{
		CString strLog;
		CString strValue = m_bObjectAngleThre ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[检查设置][对象最大角度差]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
{
	CRect CRectGroupBox;
	// 检查是否为Group控件
	if(NULL== GetDlgItem(nID)) return;
	if (NULL== (CStatic*) GetDlgItem(nID)) return;
	GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	ScreenToClient(&CRectGroupBox);
	SysPlatformInfo platformInfo;
	if (m_pParent)
	{
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	}

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
	GetDlgItem(nID)->EnableWindow(bEnable);
	GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) || CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			
			if(pWnd->GetDlgCtrlID()== IDC_COMBO_EXPRODUCT_INDEX || pWnd->GetDlgCtrlID()==IDC_STATIC_EXPRODUCT_INDEX)
			{

				pWnd->EnableWindow(bEnable && platformInfo.m_bEnableMultiCalibExtension);
				pWnd->ShowWindow(nShow && platformInfo.m_bEnableMultiCalibExtension);
			}
			else if(pWnd->GetDlgCtrlID()== IDC_STATIC_TARGET_ANGLE_THRE ||
				pWnd->GetDlgCtrlID()== IDC_EDIT_TARGET_ANGLE_THRE ||
				pWnd->GetDlgCtrlID()== IDC_CHECK_TARGET_ANGLE_THRE ||
				pWnd->GetDlgCtrlID()== IDC_STATIC_OBJECT_ANGLE_THRE ||
				pWnd->GetDlgCtrlID()== IDC_EDIT_OBJECT_ANGLE_THRE ||
				pWnd->GetDlgCtrlID()== IDC_CHECK_OBJECT_ANGLE_THRE)
			{
				BOOL bShowAngle = ((platformInfo.m_bTargetObjectCamSeparate && 8 == platformInfo.m_nCamNum && 8 == platformInfo.m_nPositionNum) || ( !platformInfo.m_bTargetObjectCamSeparate && 4 == platformInfo.m_nCamNum && 4 == platformInfo.m_nPositionNum) );
				pWnd->EnableWindow(bEnable && bShowAngle);
				pWnd->ShowWindow(nShow && bShowAngle);
			}
			else if (pWnd->GetDlgCtrlID()==IDC_STATIC_ALIGNER_WEIGHT_4VS4 || pWnd->GetDlgCtrlID()==IDC_EDIT_ALIGNER_WIEGHT_4VS4 || pWnd->GetDlgCtrlID()==IDC_STATIC_ALIGNER_WEIGHT_4VS4_POS || pWnd->GetDlgCtrlID()==IDC_COMBO_ALIGNER_4VS4_CIRCLEPOS)
			{
				pWnd->EnableWindow(bEnable && m_bShowAlignerCirclePos);
				pWnd->ShowWindow(nShow && m_bShowAlignerCirclePos);
			}
			else if(pWnd->GetDlgCtrlID()==IDC_BTN_TAR_FIX_OFFSET)
			{
				pWnd->EnableWindow(bEnable && platformInfo.m_bTargetCalibUseBoard && platformInfo.m_bTargetObjectCamSeparate);
				pWnd->ShowWindow(nShow && platformInfo.m_bTargetCalibUseBoard && platformInfo.m_bTargetObjectCamSeparate);
			}
			else if(pWnd->GetDlgCtrlID()==IDC_EDIT_CALIB_NAME_FA)
			{
				//pWnd->EnableWindow(bEnable);
				pWnd->ShowWindow(nShow);
			}
			else if(pWnd->GetDlgCtrlID()==IDC_EDIT_CALIB_NAME)
			{
				//pWnd->EnableWindow(bEnable);
				pWnd->ShowWindow(bShow && platformInfo.m_bProductFaChd);
			}
			else
			{
				pWnd->EnableWindow(bEnable);
				pWnd->ShowWindow(nShow);
			}


		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysKindAlignSetting::UpdateDlgByAuthorize()
{
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			CString strNodeName = _T("");
			// 检查模式
			bool bFind = vsConfig->GetCheckedByItem(eProductAuthorize_CheckMode,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCheckMode,m_bEnableCheckModeAuthoroze);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_ALIGN_CHECK,m_bEnableCheckModeAuthoroze,m_bShowCheckMode);
			}

			// 对位补偿
			bFind = vsConfig->GetCheckedByItem(eProductAuthorize_Offset,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowOffset,m_bEnableOffset);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_ALIGN_OFFSET,m_bEnableOffset,m_bShowOffset);
				SetGroupBoxStatus(IDC_STATIC_TARGET_OFFSET,m_bEnableOffset,m_bShowOffset);
				SetGroupBoxStatus(IDC_STATIC_GRP_ALIGN_OFFSET7,m_bEnableOffset,m_bShowOffset);
			}

			// 对位延时、精度
			bFind = vsConfig->GetCheckedByItem(eProductAuthorize_AlignToolParamSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignToolParamSet,m_bEnableAlignToolParamSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_ALIGN_PRECISION,m_bEnableAlignToolParamSet,m_bShowAlignToolParamSet);
			}

			// 对位模式切换设置
			bFind = vsConfig->GetCheckedByItem(eProductAuthorize_AlignTypeSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignTypeSet,m_bEnableAlignTypeSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_ALIGN_TYPE,m_bEnableAlignTypeSet,m_bShowAlignTypeSet);
			}

			// 其他对位设置
			bFind = vsConfig->GetCheckedByItem(eProductAuthorize_AlignOther,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignOther,m_bEnableAlignOther);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_ALIGN_OTHER_SET,m_bEnableAlignOther,m_bShowAlignOther);
			}

			// 特殊类应用设置
			bFind = vsConfig->GetCheckedByItem(eProductAuthorize_SpecialApplicationSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignSpecialApplicationSet,m_bEnableAlignSpecialApplicationSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_ALIGN_SPECIAL_APP_SET,m_bEnableAlignSpecialApplicationSet,m_bShowAlignSpecialApplicationSet);
			}

		}
	}
}

void CDlgSysKindAlignSetting::OnBnClickedBtnPcbSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[PCB参数设置]进入[PCB参数设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);
	CCDlgPCBSetting dlgPCBSetting;

	dlgPCBSetting.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgPCBSetting.EnableSysKeyboard(m_bSysKeyboardEnabled);

	dlgPCBSetting.m_bChange2um = m_pParent->m_SysOptionConfig.m_bChange2um;
	dlgPCBSetting.m_strPCBInsPreX = m_strPCBInsPreX;
	dlgPCBSetting.m_strPCBInsPreY = m_strPCBInsPreY;
	dlgPCBSetting.m_strPCBInsPreD = m_strPCBInsPreD;
	dlgPCBSetting.m_nPCBInsPreType = m_nPCBInsPreType;
	dlgPCBSetting.m_bPCBShowInspectResult = m_bPCBShowInspectResult;
	dlgPCBSetting.m_bShowPCB4To3Align = m_bShowPCB4To3Align;
	if (dlgPCBSetting.DoModal() == IDOK)
	{
		m_strPCBInsPreX = dlgPCBSetting.m_strPCBInsPreX;
		m_strPCBInsPreY = dlgPCBSetting.m_strPCBInsPreY;
		m_strPCBInsPreD = dlgPCBSetting.m_strPCBInsPreD;
		m_nPCBInsPreType = dlgPCBSetting.m_nPCBInsPreType;
		m_bPCBShowInspectResult = dlgPCBSetting.m_bPCBShowInspectResult;
		m_bShowPCB4To3Align = dlgPCBSetting.m_bShowPCB4To3Align;
	}

}

void CDlgSysKindAlignSetting::OnBnClickedBtnWeldSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[焊接机参数设置]进入[焊接机参数设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);
	CCDlgWeldInfo dlgWeldInfo;

	dlgWeldInfo.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgWeldInfo.EnableSysKeyboard(m_bSysKeyboardEnabled);

	dlgWeldInfo.m_bEnableWeld = m_bEnableWeld;

	dlgWeldInfo.m_strWeldMovementRatioX = m_strWeldMovementRatioX;
	dlgWeldInfo.m_strWeldMovementRatioY = m_strWeldMovementRatioY;
	dlgWeldInfo.m_strWeldMovementRatioD = m_strWeldMovementRatioD;

	dlgWeldInfo.m_strWeldAmmendMoveRatioX = m_strWeldAmmendMoveRatioX;
	dlgWeldInfo.m_strWeldAmmendMoveRatioY = m_strWeldAmmendMoveRatioY;

	dlgWeldInfo.m_nWeldTargetSearchTimes  = m_nWeldTargetSearchTimes;
	dlgWeldInfo.m_nWeldSearchBench		  = m_nWeldSearchBench;
	dlgWeldInfo.m_nWeldAngleBench		  = m_nWeldAngleBench;


	if (dlgWeldInfo.DoModal() == IDOK)
	{
		m_bEnableWeld = dlgWeldInfo.m_bEnableWeld;

		m_strWeldMovementRatioX = dlgWeldInfo.m_strWeldMovementRatioX;
		m_strWeldMovementRatioY = dlgWeldInfo.m_strWeldMovementRatioY;
		m_strWeldMovementRatioD = dlgWeldInfo.m_strWeldMovementRatioD;

		m_strWeldAmmendMoveRatioX = dlgWeldInfo.m_strWeldAmmendMoveRatioX;
		m_strWeldAmmendMoveRatioY = dlgWeldInfo.m_strWeldAmmendMoveRatioY;

		m_nWeldTargetSearchTimes	= dlgWeldInfo.m_nWeldTargetSearchTimes;
		m_nWeldSearchBench			= dlgWeldInfo.m_nWeldSearchBench;
		m_nWeldAngleBench			= dlgWeldInfo.m_nWeldAngleBench;

	}


}

void CDlgSysKindAlignSetting::OnBnClickedBtnEightShapeAlignSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[八字对位设置]进入[八字对位设置界面]\n"));
	g_opLogFile.SaveLogText(strLog,4);

	CDLGEightShapeAlignSetting dlg;
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_bSysKeyboardEnabled);

	dlg.m_bCheckEightShapeAlign = m_bCheckEightShapeAlign;
	dlg.m_nEightShapeAlignMode = m_nEightShapeAlignMode;
	dlg.m_dEightShapeAngle = m_dEightShapeAngle;
	if (dlg.DoModal() == IDOK)
	{
		m_bCheckEightShapeAlign = dlg.m_bCheckEightShapeAlign;
		m_nEightShapeAlignMode = dlg.m_nEightShapeAlignMode;
		m_dEightShapeAngle = dlg.m_dEightShapeAngle;
	}
}

void CDlgSysKindAlignSetting::OnBnClickedBtnBendSetting()
{
	// TODO: 在此添加控件通知处理程序代码

	CDlgBendSetting dlgBendInfo;

	dlgBendInfo.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgBendInfo.EnableSysKeyboard(m_bSysKeyboardEnabled);

	SysPlatformInfo platformInfo;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	dlgBendInfo.m_nPlatformIndex = m_nPlatformIndex;
	dlgBendInfo.m_bEnableMultiCalibExtension = platformInfo.m_bEnableMultiCalibExtension;
	dlgBendInfo.m_bBendAlignInspect			= m_bBendAlignInspect;

	dlgBendInfo.m_bBendType				= m_bBendType;
	dlgBendInfo.m_strPixelSize			= m_strPixelSize;
	dlgBendInfo.m_bBendAlignDmCode			= m_bBendAlignDmCode;

	dlgBendInfo.m_bBendDisOffset		= m_bBendDisOffset;
	dlgBendInfo.m_strTargetDisOffset	= m_strTargetDisOffset;
	dlgBendInfo.m_strObjectDisOffset	= m_strObjectDisOffset;

	dlgBendInfo.m_bBendResultOffset		= m_bBendResultOffset;
	dlgBendInfo.m_strLeftOffsetX		= m_strLeftOffsetX;
	dlgBendInfo.m_strLeftOffsetY		= m_strLeftOffsetY;
	dlgBendInfo.m_strRightOffsetX		= m_strRightOffsetX;
	dlgBendInfo.m_strRightOffsetY		= m_strRightOffsetY;
	dlgBendInfo.m_strLeftDisOffset		= m_strLeftDisOffset;
	dlgBendInfo.m_strRightDisOffset		= m_strRightDisOffset;

	dlgBendInfo.m_bEnableFbsLRY         = m_bEnableFbsLRY;
	dlgBendInfo.m_bBendThreOffset		= m_bBendThreOffset;
	dlgBendInfo.m_strThreOffsetX		= m_strThreOffsetX;
	dlgBendInfo.m_strThreOffsetY		= m_strThreOffsetY;

	dlgBendInfo.m_bBendPressPara		= m_bBendPressPara;
	dlgBendInfo.m_bBendPressParaWrite		= m_bBendPressParaWrite;
	dlgBendInfo.m_lBendPressParaWriteAddre		= m_lBendPressParaWriteAddre;
	dlgBendInfo.m_strPressOKMinY		= m_strPressOKMinY;
	dlgBendInfo.m_strPressOKMaxY		= m_strPressOKMaxY;
	dlgBendInfo.m_strPressNGMinY		= m_strPressNGMinY;
	dlgBendInfo.m_strPressNGMaxY		= m_strPressNGMaxY;

	dlgBendInfo.m_bBendLoadThre			= m_bBendLoadThre;
	dlgBendInfo.m_strBendLoadThreX		= m_strBendLoadThreX;
	dlgBendInfo.m_strBendLoadThreY		= m_strBendLoadThreY;

	dlgBendInfo.m_bBendingInspect			= m_bBendingInspect;
	dlgBendInfo.m_bBendingProductID			= m_bBendingProductID;
	dlgBendInfo.m_bBendInspectDmCode		= m_bBendInspectDmCode;
	dlgBendInfo.m_bEnableReadWsInfo		= m_bEnableReadWsInfo;

	dlgBendInfo.m_bEnableReadInspectWsInfo = m_bEnableReadInspectWsInfo;
	dlgBendInfo.m_bEnableReadStartTimeInfo = m_bEnableReadStartTimeInfo;
	dlgBendInfo.m_bEnableReadEndTimeInfo = m_bEnableReadEndTimeInfo;
	dlgBendInfo.m_bEnableReadProsessingTimeInfo = m_bEnableReadProsessingTimeInfo;

	dlgBendInfo.m_bEnableUseMathCaluLRDis = m_bEnableUseMathCaluLRDis;

	dlgBendInfo.m_bEnableMoveSnap			= m_bEnableMoveSnap;
	dlgBendInfo.m_nPosIndexFisrt			= m_nPosIndexFisrt;

	dlgBendInfo.m_bEnableAxisMoveLimit = m_bEnableAxisMoveLimit;
	dlgBendInfo.m_dAxisAllowMoveMaxX = m_dAxisAllowMoveMaxX;
	dlgBendInfo.m_dAxisAllowMoveMaxY = m_dAxisAllowMoveMaxY;
	dlgBendInfo.m_dAxisAllowMoveMaxD = m_dAxisAllowMoveMaxD;

	dlgBendInfo.m_bBendInspectSnCodeLength=m_bBendInspectSnCodeLength;
	dlgBendInfo.m_bBendInspectSnCodeInvert=m_bBendInspectSnCodeInvert;
	dlgBendInfo.m_bBendInspectGetSnOnce=m_bBendInspectGetSnOnce;

	dlgBendInfo.m_strPlatformName1 = m_strPlatformName1;
	dlgBendInfo.m_strPlatformName2 = m_strPlatformName2;
	dlgBendInfo.m_bEnableFbisTarObc = m_bEnableFbisTarObc;

	//g_opBendLogFile 操作日志
	CString strLog;
	strLog.Format(_T("点击进入工位%d[折弯参数设置]"),m_nPlatformIndex+1);
	g_opBendLogFile.SaveLogText(strLog,1);

	if (dlgBendInfo.DoModal() == IDOK)
	{
		m_bBendAlignInspect			= dlgBendInfo.m_bBendAlignInspect;

		m_bBendType				= dlgBendInfo.m_bBendType;
		m_strPixelSize			= dlgBendInfo.m_strPixelSize;
		m_bBendAlignDmCode		= dlgBendInfo.m_bBendAlignDmCode;

		m_bBendDisOffset		= dlgBendInfo.m_bBendDisOffset;
		m_strTargetDisOffset	= dlgBendInfo.m_strTargetDisOffset;
		m_strObjectDisOffset	= dlgBendInfo.m_strObjectDisOffset;

		m_strLeftDisOffset		= dlgBendInfo.m_strLeftDisOffset;
		m_strRightDisOffset		= dlgBendInfo.m_strRightDisOffset;
		m_bBendResultOffset		= dlgBendInfo.m_bBendResultOffset;
		m_strLeftOffsetX		= dlgBendInfo.m_strLeftOffsetX;
		m_strLeftOffsetY		= dlgBendInfo.m_strLeftOffsetY;
		m_strRightOffsetX		= dlgBendInfo.m_strRightOffsetX;
		m_strRightOffsetY		= dlgBendInfo.m_strRightOffsetY;
		m_bEnableFbsLRY         = dlgBendInfo.m_bEnableFbsLRY;
		m_bBendThreOffset       = dlgBendInfo.m_bBendThreOffset;
		m_strThreOffsetX		= dlgBendInfo.m_strThreOffsetX;
		m_strThreOffsetY		= dlgBendInfo.m_strThreOffsetY;

		m_bBendPressPara        = dlgBendInfo.m_bBendPressPara;
		m_bBendPressParaWrite        = dlgBendInfo.m_bBendPressParaWrite;
		m_lBendPressParaWriteAddre        = dlgBendInfo.m_lBendPressParaWriteAddre;
		m_strPressOKMinY        = dlgBendInfo.m_strPressOKMinY;
		m_strPressOKMaxY        = dlgBendInfo.m_strPressOKMaxY;
		m_strPressNGMinY        = dlgBendInfo.m_strPressNGMinY;
		m_strPressNGMaxY        = dlgBendInfo.m_strPressNGMaxY;

		m_bBendLoadThre			= dlgBendInfo.m_bBendLoadThre;
		m_strBendLoadThreX		= dlgBendInfo.m_strBendLoadThreX;
		m_strBendLoadThreY		= dlgBendInfo.m_strBendLoadThreY;

		m_bEnableReadInspectWsInfo = dlgBendInfo.m_bEnableReadInspectWsInfo;
		m_bEnableReadStartTimeInfo = dlgBendInfo.m_bEnableReadStartTimeInfo;
		m_bEnableReadEndTimeInfo = dlgBendInfo.m_bEnableReadEndTimeInfo;
		m_bEnableReadProsessingTimeInfo = dlgBendInfo.m_bEnableReadProsessingTimeInfo;

		m_bEnableUseMathCaluLRDis = dlgBendInfo.m_bEnableUseMathCaluLRDis;

		m_bEnableReadWsInfo			= dlgBendInfo.m_bEnableReadWsInfo;
		m_bBendingInspect			= dlgBendInfo.m_bBendingInspect;
		m_bBendingProductID			= dlgBendInfo.m_bBendingProductID;
		m_bBendInspectDmCode		= dlgBendInfo.m_bBendInspectDmCode;
		m_bEnableMoveSnap			= dlgBendInfo.m_bEnableMoveSnap;
		m_nPosIndexFisrt			= dlgBendInfo.m_nPosIndexFisrt;

		m_bEnableAxisMoveLimit = dlgBendInfo.m_bEnableAxisMoveLimit;
		m_dAxisAllowMoveMaxX = dlgBendInfo.m_dAxisAllowMoveMaxX;
		m_dAxisAllowMoveMaxY = dlgBendInfo.m_dAxisAllowMoveMaxY;
		m_dAxisAllowMoveMaxD = dlgBendInfo.m_dAxisAllowMoveMaxD;

		m_bBendInspectSnCodeLength=dlgBendInfo.m_bBendInspectSnCodeLength;
		m_bBendInspectSnCodeInvert=dlgBendInfo.m_bBendInspectSnCodeInvert;
		m_bBendInspectGetSnOnce=dlgBendInfo.m_bBendInspectGetSnOnce;

		m_strPlatformName1 = dlgBendInfo.m_strPlatformName1;
		m_strPlatformName2 = dlgBendInfo.m_strPlatformName2;
		m_bEnableFbisTarObc = dlgBendInfo.m_bEnableFbisTarObc;

		strLog.Format(_T("保存退出工位%d[折弯参数设置]"),m_nPlatformIndex+1);
		g_opBendLogFile.SaveLogText(strLog,1);
	}
	else
	{
		strLog.Format(_T("不保存退出工位%d[折弯参数设置]"),m_nPlatformIndex+1);
		g_opBendLogFile.SaveLogText(strLog,1);
	}
}


void CDlgSysKindAlignSetting::OnBnClickedBtnCmdLightCtrlSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPhotoLightCtrlSet dlg;
	dlg.SetSystemOptionConfig(m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystempOptionConfig);
	dlg.m_cPhotoLightCtrl = m_cPhotoLightCtrl;
	if (dlg.DoModal() == IDOK)
	{
		m_cPhotoLightCtrl = dlg.m_cPhotoLightCtrl;
		m_bPhotoLightCtrlInfoChange = TRUE;  //点击保存默认发生改变
	}
}
