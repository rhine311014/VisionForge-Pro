// DlgSysPlatformOptionParam.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformOptionParam.h"
#include "svXMLConfigurator.h"

#include "DlgSysEnvironSetting.h"
#include "DlgCalibAxisDirection.h"
#include "DynDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgSysPlatformOptionParam 对话框


CDlgSysPlatformOptionParam::CDlgSysPlatformOptionParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysPlatformOptionParam::IDD, pParent)
	, m_bChkSelectAll(FALSE)
	, m_bSearchWithDistanceInspect(FALSE)
	, m_bSendInsDistanceInSearchResult(FALSE)
	, m_strOffsetPicturePath(_T(""))
	, m_bEnableDmCodeCalib(FALSE)
	, m_nTarInfoCommModeInDiffPC(0)
	, m_bEnableAlignAfterSearch(FALSE)
	, m_strOffsetMaxX(_T(""))
	, m_strOffsetMaxY(_T(""))
	, m_strOffsetMaxD(_T(""))
	, m_strAxisMoveMaxX(_T(""))
	, m_strAxisMoveMaxY(_T(""))
	, m_strAxisMoveMaxD(_T(""))
	, m_bOmitRePickObjectEnable(FALSE)
	, m_bOmitRePickObjectEnable_MM(FALSE)
	, m_strRePickOffsetMaxX(_T(""))
	, m_strRePickOffsetMaxY(_T(""))
	, m_strRePickOffsetMaxX_MM(_T(""))
	, m_strRePickOffsetMaxY_MM(_T(""))
	, m_bObjectSearchExProduct(FALSE)
	, m_bTargetSearchExProduct(FALSE)
	, m_bAlignExProduct(TRUE)
	, m_bShowOffsetTips(FALSE)
	, m_bSetOffsetTip(FALSE)
	, m_bRecordCamFixMarkPos(FALSE)
	, m_bSaveCalibData(FALSE)
	, m_bAutoCopyCalibData(FALSE)
	, m_bEnableCalibrateCheckByAxisMoveOption(FALSE)
	, m_dCalibrateCheckAxisMoveRangeX(0)
	, m_dCalibrateCheckAxisMoveRangeY(0)
	, m_dCalibrateCheckAxisMoveRangeD(0)
	, m_bChkSendAlignMulResults(FALSE)
	, m_bSendAreaInspectSubResult(FALSE)
	, m_bReadWriteAxisPosWithCmd(FALSE)
	, m_bEnableCommMultiCalibExtension(FALSE)
	, m_bVisionOnLineStatus(FALSE)
	, m_bEnableAlignBenchCenter(FALSE)
	, m_bSetVisionManualSearchToPLC(FALSE)
	, m_bAlignOneTime(FALSE)
	, m_bEnableMutiTarget(FALSE)
	, m_bObjMulSearchPosXYPD(FALSE)
	, m_bTargetAngleMulAlignerXYPD(FALSE)
	, m_bLastPickSetting(FALSE)
	, m_bEnablePickPosUpdate(FALSE)
	, m_bAskStagePosEachMove(FALSE)
	, m_bTargetPosSame(FALSE)
	, m_bObjectPosSame(FALSE)
	, m_bSaveAlignPos(FALSE)
	, m_bEnableSearchCindidate(FALSE)
	, m_bSearchTimes(FALSE)
	, m_nSearchTimes(1)
	, m_bEnableSearchShutter(FALSE)
	, m_bManualAfTarSearchFail(FALSE)
	, m_bManualAfObjSearchFail(FALSE)
	, m_bAlignFinishClearSnapStatus(FALSE)
	, m_bManualAfTarSearchFailShowButton(FALSE)
	, m_bManualAfObjSearchFailShowButton(FALSE)
	//,m_bInspectEnable(FALSE)
	, m_bShowEightShapeAlign(FALSE)
	, m_bEnablePickSendRelativateAxis(FALSE)
	, m_nSearchDelayTime(0)
	, m_bFAALAlignMoreTimeTargetSeach(FALSE)
{
	m_psaSysInfoStrings		= NULL;
	m_bSysKeyboardEnabled	= FALSE;
	m_bKillFocusEnabled		= TRUE;

	m_pParent				= NULL;
	m_pVisionASM			= NULL;
	m_nListLen				= 0;

	map<pair<int,CString>,BOOL>::iterator iterTmp;
	iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
	for (;iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)
	{
		iterTmp->second = FALSE;
	}
	m_hBmpOffsetDir = NULL;
	m_strCalibrateCheckAxisMoveRangeX = _T("0");
	m_strCalibrateCheckAxisMoveRangeY = _T("0");
	m_strCalibrateCheckAxisMoveRangeD = _T("0");
	m_nListAligner			= 0;//lzk

	m_bShowFWEL = FALSE;
//	m_bShowPCB = FALSE;
	m_bShowEightShapeAlign = FALSE;
	m_bShowBend = FALSE;
	m_bEnableAllControl = FALSE;

	m_bShowCalibrateCtl = true;
	m_bEnableCalibrateCtl = true;

	m_bShowOffsetCtl = true;
	m_bEnableOffsetCtl = true;
	m_bEnableAlignOffsetMarkByAera = FALSE;

	m_bEnablePhotoLightCtrl = FALSE;

	m_bSetVisionManualSearchToPLC2 = FALSE;
	m_nVisionManualSearchToPlcAddre = 10000;
}

CDlgSysPlatformOptionParam::~CDlgSysPlatformOptionParam()
{
	DeleteObject(m_hBmpOffsetDir);
	
}

void CDlgSysPlatformOptionParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_TOOL_LIST, m_listCtrlSearchTool);
	DDX_Check(pDX, IDC_CHECK_SELECT_ALL, m_bChkSelectAll);
	//DDX_Check(pDX, IDC_CHK_SEARCH_WITH_DISTANCE_INSPECT, m_bSearchWithDistanceInspect);
	//DDX_Check(pDX, IDC_CHK_SEND_INSPECT_DISTANCE, m_bSendInsDistanceInSearchResult);
	//DDX_Text(pDX, IDC_EDT_OFFSET_PICTURE_PATH, m_strOffsetPicturePath);
	DDX_Check(pDX, IDC_CHK_ENABLE_DMCODE_CALIB, m_bEnableDmCodeCalib);
	//DDX_Radio(pDX, IDC_RDO_TARGET_INFO_COMM_MODE, m_nTarInfoCommModeInDiffPC);
	//DDX_Check(pDX, IDC_CHK_SEARCH_AND_ALIGN, m_bEnableAlignAfterSearch);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_X, m_strOffsetMaxX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_Y, m_strOffsetMaxY);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_D, m_strOffsetMaxD);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_X, m_strAxisMoveMaxX);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_Y, m_strAxisMoveMaxY);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_D, m_strAxisMoveMaxD);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_X2, m_strAxisMoveMaxX_LowerLimit);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_Y2, m_strAxisMoveMaxY_LowerLimit);
	DDX_Text(pDX, IDC_EDIT_AXIS_MOVE_MAX_D2, m_strAxisMoveMaxD_LowerLimit);
	DDX_Check(pDX, IDC_CHECK_OMIT_REPICK_OBJECT_ENABLE, m_bOmitRePickObjectEnable);
	DDX_Check(pDX, IDC_CHECK_OMIT_REPICK_OBJECT_ENABLE_MM, m_bOmitRePickObjectEnable_MM);
	DDX_Text(pDX, IDC_EDIT_REPICK_OFFSET_MAX_X, m_strRePickOffsetMaxX);
	DDX_Text(pDX, IDC_EDIT_REPICK_OFFSET_MAX_Y, m_strRePickOffsetMaxY);
	DDX_Text(pDX, IDC_EDIT_REPICK_OFFSET_MAX_X_MM, m_strRePickOffsetMaxX_MM);
	DDX_Text(pDX, IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, m_strRePickOffsetMaxY_MM);
	DDX_Check(pDX, IDC_CHK_OBJSEARCH_EX, m_bObjectSearchExProduct);
	DDX_Check(pDX, IDC_CHK_TARSEARCH_EX, m_bTargetSearchExProduct);
	DDX_Check(pDX, IDC_CHK_ALIGN_EX, m_bAlignExProduct);
	//DDX_Check(pDX, IDC_CHECK_SHOW_OFFSET_TIPS, m_bShowOffsetTips);
	//DDX_Check(pDX, IDC_CHECK_SET_OFFSET_TIP, m_bSetOffsetTip);
	//DDX_Check(pDX, IDC_CHK_RECORD_CAMERA_FIX_MARK, m_bRecordCamFixMarkPos);
	DDX_Check(pDX, IDC_CHECK_SAVE_CALIB_DATA, m_bSaveCalibData);
	DDX_Check(pDX, IDC_CHECK_COPY_CALIB_DATA, m_bAutoCopyCalibData);
	/*DDX_Check(pDX, IDC_CHECK_CALIB_CHECK_BY_AXISMOVE, m_bEnableCalibrateCheckByAxisMoveOption);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, m_dCalibrateCheckAxisMoveRangeX);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, m_dCalibrateCheckAxisMoveRangeY);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, m_dCalibrateCheckAxisMoveRangeD);*/
	DDX_Control(pDX, IDC_CMB_TARAUTOCALIB_ALIGN_TYPE, m_cmbTarAutoCalibAlignType);
	//DDX_Check(pDX, IDC_CHK_SEND_ALIGN_MUL_RESULT, m_bChkSendAlignMulResults);
	DDX_Control(pDX, IDC_LIST_ALIGNER_TYPE, m_listCtrlAlignerType);
	//DDX_Check(pDX, IDC_CHK_SEND_AREA_INSPECT_SUB_RESULT, m_bSendAreaInspectSubResult);
	//DDX_Check(pDX, IDC_CHECK_READ_WRITE_AXIS_P0S_WITN_CMD, m_bReadWriteAxisPosWithCmd);

	DDX_Check(pDX, IDC_CHECK_ENABLE_COMM_MULTI_CALIB_EXTENSION, m_bEnableCommMultiCalibExtension);
	//DDX_Check(pDX, IDC_CHECK_VISION_STATUS, m_bVisionOnLineStatus);
	//DDX_Check(pDX, IDC_CHECK_ENABLE_ALIGN_BENCH_CENTER, m_bEnableAlignBenchCenter);
	//DDX_Check(pDX, IDC_CHECK_SET_VISION_MANUAL_SEARCH_TO_PLC, m_bSetVisionManualSearchToPLC);
	//DDX_Check(pDX, IDC_CHECK_ALIGN_ONE_TIME, m_bAlignOneTime);
	//DDX_Check(pDX, IDC_CHECK_ENABLE_MUTI_TARGET, m_bEnableMutiTarget);
	//DDX_Check(pDX, IDC_CHECK_OBJ_MUL_SEARCH_POS_XYPD, m_bObjMulSearchPosXYPD);
	//DDX_Check(pDX, IDC_CHECK_TARGET_ANGLE_MUL_ALIGNER_XYPD, m_bTargetAngleMulAlignerXYPD);
	DDX_Check(pDX, IDC_CHECK_LAST_PICK_SETTING, m_bLastPickSetting);
	DDX_Check(pDX, IDC_CHECK_PICK_POS_UPDATE, m_bEnablePickPosUpdate);
	DDX_Check(pDX, IDC_CHECK_ASK_STAGE_POS_EACH_MOVE, m_bAskStagePosEachMove);
	DDX_Check(pDX, IDC_CHECK_TARGET_POS_SAME, m_bTargetPosSame);
	DDX_Check(pDX, IDC_CHECK_OBJECT_POS_SAME, m_bObjectPosSame);
	DDX_Check(pDX,IDC_CHK_FAAL_ALIGN_ONE_TIME_TARGET_SEARCH_SNAP,m_bFAALAlignOneTimeTargetSeachSnap);
	//DDX_Check(pDX, IDC_CHECK_SAVE_ALIGN_POS, m_bSaveAlignPos);
	//DDX_Check(pDX, IDC_CHECK_SEARCH_CINDIDATE_ENABLE, m_bEnableSearchCindidate);

	// DDX_Check(pDX, IDC_CHECK_SEARCHTIME, m_bSearchTimes);
	//DDX_Text(pDX, IDC_EDIT_SEARCHTIME, m_nSearchTimes);

	//DDX_Check(pDX, IDC_CHECK_ENABLE_SEARCH_SHUTTER, m_bEnableSearchShutter);

	//DDX_Check(pDX, IDC_CHECK_SHOW_PCB, m_bShowPCB);
	//DDX_Check(pDX, IDC_CHECK_SHOW_FWEL, m_bShowFWEL);

	//DDX_Check(pDX, IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED, m_bManualAfTarSearchFail);
	//DDX_Check(pDX, IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED, m_bManualAfObjSearchFail);

	//DDX_Check(pDX, IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED_SHOW_BUTTON, m_bManualAfTarSearchFailShowButton);
	//DDX_Check(pDX, IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED_SHOW_BUTTON, m_bManualAfObjSearchFailShowButton);

	//DDX_Check(pDX, IDC_CHECK_INSPECT_UI_SHOW, m_bInspectEnable);

	//DDX_Check(pDX, IDC_CHECK_TARGT_USE_VIRTUAL_COORD, m_bTargetUseFixedVirtualCoordinates);
	//DDX_Check(pDX, IDC_CHECK_MUL_MODE_ENABLE, m_bEnableMultiMarkMode);
	//DDX_Check(pDX, IDC_CHECK_ENABLE_TARGET_OBJECT_VIRTUAL_MODE, m_bEnableTargetAndObjectPositionVirtualMode);

	//DDX_Check(pDX, IDC_CHECK_CALIB_SEND_ABS_POS, m_bCalibSendAbsPos);
	DDX_Check(pDX, IDC_CHECK_CALIB_SEND_RELATIVE_POS, m_bCalibSendRelativePos);

	/*DDX_Check(pDX, IDC_CHECK_CALIBRATE_CHECK_BY_SIZE, m_bEnableCalibrateCheckBySize);
	DDX_Check(pDX, IDC_CHECK_RELVEVANCE_CHECK_SIZE, m_bEnableRelevanceCheckBySize);
	DDX_Check(pDX, IDC_CHECK_FIX_TARGET_CHECK_CALIB, m_bEnableCalibrateCheckByFixedTargetAligner);*/


	//DDX_Check(pDX, IDC_CHECK_EIGHT_SHAPE_ENABLED, m_bShowEightShapeAlign);
	DDX_Check(pDX, IDC_CHECK_SEND_RELATIVE_AXIS, m_bEnablePickSendRelativateAxis);	
	DDX_Text(pDX, IDC_EDIT1, m_nSearchDelayTime);
	DDX_Check(pDX, IDC_CHECK_MORE_TIME_TARGET_SEARCH, m_bFAALAlignMoreTimeTargetSeach);
}

BEGIN_MESSAGE_MAP(CDlgSysPlatformOptionParam, CDialogEx)
	
	//
	/*ON_BN_CLICKED(IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED, &CDlgSysPlatformOptionParam::OnBnClickedCheckManualTargetSearchFailed)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED, &CDlgSysPlatformOptionParam::OnBnClickedCheckManualObjectSearchFailed)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED_SHOW_BUTTON, &CDlgSysPlatformOptionParam::OnBnClickedCheckManualTargetSearchFailedShowButton)
	ON_BN_CLICKED(IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED_SHOW_BUTTON, &CDlgSysPlatformOptionParam::OnBnClickedCheckManualObjectSearchFailedShowButton)*/

	//ON_BN_CLICKED(IDC_CHECK_SHOW_FWEL, &CDlgSysPlatformOptionParam::OnBnClickedCheckShowFwel)
	//ON_BN_CLICKED(IDC_CHECK_SHOW_PCB, &CDlgSysPlatformOptionParam::OnBnClickedCheckShowPcb)

	//ON_BN_CLICKED(IDC_CHECK_ENABLE_SEARCH_SHUTTER, &CDlgSysPlatformOptionParam::OnBnClickedCheckEnableSearchShutter)
	//ON_EN_SETFOCUS(IDC_EDIT_SEARCHTIME, &CDlgSysPlatformOptionParam::OnSetfocusEditSearchtime)
	//ON_EN_KILLFOCUS(IDC_EDIT_SEARCHTIME, &CDlgSysPlatformOptionParam::OnKillfocusEditSearchtime)
	// ON_BN_CLICKED(IDC_CHECK_SEARCHTIME, &CDlgSysPlatformOptionParam::OnBnClickedCheckSearchtime)

	ON_BN_CLICKED(IDC_CHECK_SEARCH_CINDIDATE_ENABLE, &CDlgSysPlatformOptionParam::OnBnClickedCheckSearchCindidateEnable)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_POS_SAME, &CDlgSysPlatformOptionParam::OnBnClickedCheckObjectPosSame)
	//ON_BN_CLICKED(IDC_CHECK_SAVE_ALIGN_POS, &CDlgSysPlatformOptionParam::OnBnClickedCheckSaveAlignPos)
	ON_BN_CLICKED(IDC_CHECK_TARGET_POS_SAME, &CDlgSysPlatformOptionParam::OnBnClickedCheckTargetPosSame)
	ON_BN_CLICKED(IDC_CHECK_ASK_STAGE_POS_EACH_MOVE, &CDlgSysPlatformOptionParam::OnBnClickedCheckAskStagePosEachMove)
	ON_BN_CLICKED(IDC_CHECK_PICK_POS_UPDATE, &CDlgSysPlatformOptionParam::OnBnClickedCheckPickPosUpdate)
	ON_BN_CLICKED(IDC_CHECK_LAST_PICK_SETTING, &CDlgSysPlatformOptionParam::OnBnClickedCheckLastPickSetting)
	//ON_BN_CLICKED(IDC_CHECK_OBJ_MUL_SEARCH_POS_XYPD, &CDlgSysPlatformOptionParam::OnBnClickedCheckObjMulSearchPosXypd)
	//ON_BN_CLICKED(IDC_CHECK_TARGET_ANGLE_MUL_ALIGNER_XYPD, &CDlgSysPlatformOptionParam::OnBnClickedCheckTargetAngleMulAlignerXypd)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MUTI_TARGET, &CDlgSysPlatformOptionParam::OnBnClickedCheckEnableMutiTarget)
	ON_BN_CLICKED(IDC_CHECK_ALIGN_ONE_TIME, &CDlgSysPlatformOptionParam::OnBnClickedCheckAlignOneTime)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_COMM_MULTI_CALIB_EXTENSION, &CDlgSysPlatformOptionParam::OnBnClickedCheckEnableCommMultiCalibExtension)
	ON_BN_CLICKED(IDC_CHECK_VISION_STATUS, &CDlgSysPlatformOptionParam::OnBnClickedCheckVisionStatus)
	//ON_BN_CLICKED(IDC_CHECK_ENABLE_ALIGN_BENCH_CENTER, &CDlgSysPlatformOptionParam::OnBnClickedCheckEnableAlignBenchCenter)
	//ON_BN_CLICKED(IDC_CHECK_SET_VISION_MANUAL_SEARCH_TO_PLC, &CDlgSysPlatformOptionParam::OnBnClickedCheckSetVisionManualSearchToPlc)

	//
	ON_NOTIFY(NM_CLICK, IDC_SEARCH_TOOL_LIST, &CDlgSysPlatformOptionParam::OnNMClickSearchToolList)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, &CDlgSysPlatformOptionParam::OnBnClickedCheckSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET_SEARCH_TOOL, &CDlgSysPlatformOptionParam::OnBnClickedButtonResetSearchTool)
    ON_NOTIFY(NM_DBLCLK, IDC_SEARCH_TOOL_LIST, &CDlgSysPlatformOptionParam::OnNMDblclkSearchToolList)
	ON_BN_CLICKED(IDC_BTN_SET_OFFSET_PICTURE, &CDlgSysPlatformOptionParam::OnBnClickedBtnSetOffsetPicture)
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_X, &CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxX)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_X, &CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxX)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_Y, &CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_Y, &CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_D, &CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxD)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_D, &CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxD)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_X, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxX)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_X, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxX)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_Y, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_Y, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_D, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxD)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_D, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxD)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_X2, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxX2)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_X2, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxX2)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_Y2, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxY2)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_Y2, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxY2)
	ON_EN_SETFOCUS(IDC_EDIT_AXIS_MOVE_MAX_D2, &CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxD2)
	ON_EN_KILLFOCUS(IDC_EDIT_AXIS_MOVE_MAX_D2, &CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxD2)
	ON_EN_SETFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_X, &CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxX)
	ON_EN_KILLFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_X, &CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxX)
	ON_EN_SETFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_Y, &CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_Y, &CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, &CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxXMm)
	ON_EN_KILLFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, &CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxXMm)
	ON_EN_SETFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, &CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxYMm)
	ON_EN_KILLFOCUS(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, &CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxYMm)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, &CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeX)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, &CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeX)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, &CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeY)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, &CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeY)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, &CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeD)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, &CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeD)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALIGNER_TYPE, &CDlgSysPlatformOptionParam::OnNMClickListAlignerType)

	ON_BN_CLICKED(IDC_BUTTON_SET_CALIB_DIRECTION, &CDlgSysPlatformOptionParam::OnBnClickedBtnCalibDirection)

	ON_BN_CLICKED(IDC_CHECK_CALIB_SEND_RELATIVE_POS, &CDlgSysPlatformOptionParam::OnBnClickedCheckCalibSendRelativePos)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_OTHER_SET, &CDlgSysPlatformOptionParam::OnBnClickedButtonOffsetOtherSet)
	ON_BN_CLICKED(IDC_BUTTON_INSPECT_SET2, &CDlgSysPlatformOptionParam::OnBnClickedButtonInspectSet2)
	ON_BN_CLICKED(IDC_BUTTON_SNAP_SET, &CDlgSysPlatformOptionParam::OnBnClickedButtonSnapSet)
	ON_BN_CLICKED(IDC_BUTTON_ALIGNER_OPTION_SET2, &CDlgSysPlatformOptionParam::OnBnClickedButtonAlignerOptionSet2)
	ON_BN_CLICKED(IDC_BUTTON_PLATFORM_COMM_SET3, &CDlgSysPlatformOptionParam::OnBnClickedButtonPlatformCommSet3)
	ON_BN_CLICKED(IDC_BUTTON_CALIBTATE_SET4, &CDlgSysPlatformOptionParam::OnBnClickedButtonCalibtateSet4)
	ON_BN_CLICKED(IDC_CHK_FAAL_ALIGN_ONE_TIME_TARGET_SEARCH_SNAP, &CDlgSysPlatformOptionParam::OnBnClickedChkFaalAlignOneTimeTargetSearchSnap)
	ON_EN_SETFOCUS(IDC_EDIT1, &CDlgSysPlatformOptionParam::OnEnSetSouSuoTime)
	ON_BN_CLICKED(IDC_CHECK_MORE_TIME_TARGET_SEARCH, &CDlgSysPlatformOptionParam::OnBnClickedCheckMoreTimeTargetSearch)
END_MESSAGE_MAP()


// CDlgSysPlatformOptionParam 消息处理程序
void CDlgSysPlatformOptionParam::UpdateDialogLanguage()
{
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

		if(_T("IDD_SYS_PLATFORM_OPTION_PARAM") == strDlgID)
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

BOOL CDlgSysPlatformOptionParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);

	return TRUE;
}

void CDlgSysPlatformOptionParam::UpdateDlgData(BOOL bSaveAndValidate)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	SysPlatformInfo platformInfo;
	m_pVisionASM->GetSysPlatformInfo(platformInfo);
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);

		map<pair<int,CString>,BOOL>::iterator iter;
		map<pair<int,CString>,BOOL>::iterator iterTmp;

		iter = m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
		iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();

		for(;(iter != m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.end())&&(iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end());iter++,iterTmp++)
		{
			iter->second = iterTmp->second;
		}

		//lzk
		/////////////////////////////////////////////////////////////////////////////////////
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2,iterTmp2Vs2;
			iter2Vs2 = m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();
			iterTmp2Vs2 = m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();
			for(;(iter2Vs2 != m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.end())&&(iterTmp2Vs2 != m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.end());iter2Vs2++,iterTmp2Vs2++)
			{
				iter2Vs2->m_bOpenAligner = iterTmp2Vs2->m_bOpenAligner;
			}
		}
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3,iterTmp3Vs3;
			iter3Vs3 = m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();
			iterTmp3Vs3 = m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();
			for(;(iter3Vs3 != m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.end())&&(iterTmp3Vs3 != m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.end());iter3Vs3++,iterTmp3Vs3++)
			{
				iter3Vs3->m_bOpenAligner = iterTmp3Vs3->m_bOpenAligner;
			}
		}

		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4,iterTmp4Vs4;
			iter4Vs4 = m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();
			iterTmp4Vs4 = m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();
			for(;(iter4Vs4 != m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.end())&&(iterTmp4Vs4 != m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.end());iter4Vs4++,iterTmp4Vs4++)
			{
				iter4Vs4->m_bOpenAligner = iterTmp4Vs4->m_bOpenAligner;
			}
		}



		/////////////////////////////////////////////////////////////////////////////////////////


		m_platformOptionInfo.m_bSearchWithDistanceInspect = m_bSearchWithDistanceInspect;
		m_platformOptionInfo.m_bSendInsDistanceInSearchResult = m_bSendInsDistanceInSearchResult;
		m_platformOptionInfo.m_strOffsetPicturePath = m_strOffsetPicturePath;
		m_platformOptionInfo.m_bEnableDmCodeCalib = m_bEnableDmCodeCalib;
		m_platformOptionInfo.m_nTarInfoCommModeInDiffPC = m_nTarInfoCommModeInDiffPC;
		m_platformOptionInfo.m_bEnableAlignAfterSearch = m_bEnableAlignAfterSearch;
		m_platformOptionInfo.m_bRecordCamFixMarkPos = m_bRecordCamFixMarkPos;
		m_platformOptionInfo.m_bObjectSearchExProduct = m_bObjectSearchExProduct;
		m_platformOptionInfo.m_bTargetSearchExProduct = m_bTargetSearchExProduct;
		m_platformOptionInfo.m_bAlignExProduct = m_bAlignExProduct;

		m_platformOptionInfo.m_bSaveCalibData = m_bSaveCalibData;
		m_platformOptionInfo.m_bAutoCopyCalibData = m_bAutoCopyCalibData;
		m_platformOptionInfo.m_bSendAreaInspectSubResult = m_bSendAreaInspectSubResult;

		m_platformOptionInfo.m_dOffsetMaxX = atof(m_strOffsetMaxX);
		m_platformOptionInfo.m_dOffsetMaxY = atof(m_strOffsetMaxY);
		m_platformOptionInfo.m_dOffsetMaxD = atof(m_strOffsetMaxD);

		m_platformOptionInfo.m_dAxisMoveMaxX = atof(m_strAxisMoveMaxX);
		m_platformOptionInfo.m_dAxisMoveMaxY = atof(m_strAxisMoveMaxY);
		m_platformOptionInfo.m_dAxisMoveMaxD = atof(m_strAxisMoveMaxD);

		m_platformOptionInfo.m_dAxisMoveMaxX_LowerLimit = atof(m_strAxisMoveMaxX_LowerLimit);
		m_platformOptionInfo.m_dAxisMoveMaxY_LowerLimit = atof(m_strAxisMoveMaxY_LowerLimit);
		m_platformOptionInfo.m_dAxisMoveMaxD_LowerLimit = atof(m_strAxisMoveMaxD_LowerLimit);

		// 像素判定
		m_platformOptionInfo.m_bOmitRePickObjectEnable    = m_bOmitRePickObjectEnable;
		m_platformOptionInfo.m_dRePickOffsetMaxX = atof(m_strRePickOffsetMaxX);
		m_platformOptionInfo.m_dRePickOffsetMaxY = atof(m_strRePickOffsetMaxY);

		// 实际物理坐标判定
		m_platformOptionInfo.m_bOmitRePickObjectEnable_MM    = m_bOmitRePickObjectEnable_MM;
		m_platformOptionInfo.m_dRePickOffsetMaxX_MM = atof(m_strRePickOffsetMaxX_MM);
		m_platformOptionInfo.m_dRePickOffsetMaxY_MM = atof(m_strRePickOffsetMaxY_MM);
		
		m_platformOptionInfo.m_bEnableCalibrateCheckByAxisMoveOption = m_bEnableCalibrateCheckByAxisMoveOption;
		m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeX = m_dCalibrateCheckAxisMoveRangeX;
		m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeY = m_dCalibrateCheckAxisMoveRangeY;
		m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeD = m_dCalibrateCheckAxisMoveRangeD;

		m_platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip = m_bSetOffsetTip;
		m_platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips = m_bShowOffsetTips;

		m_platformOptionInfo.m_eTarAutoCalibAlignMode = (TargetAutoCalibAlignMode)m_cmbTarAutoCalibAlignType.GetCurSel();

		m_platformOptionInfo.m_bSendMulAlignResult	= m_bChkSendAlignMulResults;
		m_platformOptionInfo.m_bReadWriteAxisPosWithCmd = m_bReadWriteAxisPosWithCmd;


		m_platformOptionInfo.m_bEnableCommMultiCalibExtension = m_bEnableCommMultiCalibExtension;
		m_platformOptionInfo.m_bVisionOnLineStatus = m_bVisionOnLineStatus;
		m_platformOptionInfo.m_bEnableAlignBenchCenter = m_bEnableAlignBenchCenter;
		m_platformOptionInfo.m_bSetVisionManualSearchToPLC = m_bSetVisionManualSearchToPLC;

		m_platformOptionInfo.m_bSetVisionManualSearchToPLC2 = m_bSetVisionManualSearchToPLC2;
		m_platformOptionInfo.m_nVisionManualSearchToPlcAddre = m_nVisionManualSearchToPlcAddre;

		m_platformOptionInfo.m_bAlignOneTime = m_bAlignOneTime;
		m_platformOptionInfo.m_bEnableMutiTarget = m_bEnableMutiTarget;
		m_platformOptionInfo.m_bObjMulSearchPosXYPD = m_bObjMulSearchPosXYPD;
		m_platformOptionInfo.m_bTargetAngleMulAlignerXYPD = m_bTargetAngleMulAlignerXYPD;
		m_platformOptionInfo.m_bLastPickSetting = m_bLastPickSetting;
		m_platformOptionInfo.m_bEnablePickPosUpdate = m_bEnablePickPosUpdate;

		m_platformOptionInfo.m_bEnablePickSendRelativateAxis = m_bEnablePickSendRelativateAxis;

		m_platformOptionInfo.m_bAskStagePosEachMove = m_bAskStagePosEachMove;
		m_platformOptionInfo.m_nSearchDelayTime = m_nSearchDelayTime;
		m_platformOptionInfo.m_bTargetPosSame = m_bTargetPosSame;
		m_platformOptionInfo.m_bObjectPosSame = m_bObjectPosSame;
		m_platformOptionInfo.m_bSaveAlignPos = m_bSaveAlignPos;
		m_platformOptionInfo.m_bEnableSearchCindidate = m_bEnableSearchCindidate;
		m_platformOptionInfo.m_bSearchTimes = m_bSearchTimes;
		m_platformOptionInfo.m_nSearchTimes = m_nSearchTimes;
		m_platformOptionInfo.m_bEnableSearchShutter = m_bEnableSearchShutter;
		m_platformOptionInfo.m_bEnablePhotoLightCtrl = m_bEnablePhotoLightCtrl;

		m_platformOptionInfo.m_bShowEightShapeAlign = m_bShowEightShapeAlign;
//		m_platformOptionInfo.m_bShowPCB = m_bShowPCB;
		m_platformOptionInfo.m_bShowFWEL = m_bShowFWEL;
		m_platformOptionInfo.m_bShowBend = m_bShowBend;
		m_platformOptionInfo.m_bManualAfTarSearchFail = m_bManualAfTarSearchFail;
		m_platformOptionInfo.m_bManualAfObjSearchFail = m_bManualAfObjSearchFail;
		m_platformOptionInfo.m_bAlignFinishClearSnapStatus = m_bAlignFinishClearSnapStatus;

		m_platformOptionInfo.m_bExeObjectCheckModeWhenAlign = m_bExeObjectCheckModeWhenAlign;
		m_platformOptionInfo.m_bExeTargetCheckModeWhenAlign = m_bExeTargetCheckModeWhenAlign;

		m_platformOptionInfo.m_bManualAfTarSearchFailShowButton = m_bManualAfTarSearchFailShowButton;
		m_platformOptionInfo.m_bManualAfObjSearchFailShowButton = m_bManualAfObjSearchFailShowButton;

		//m_platformOptionInfo.m_bInspectEnable = m_bInspectEnable;

		m_platformOptionInfo.m_bTargetUseFixedVirtualCoordinates = m_bTargetUseFixedVirtualCoordinates;
		m_platformOptionInfo.m_bEnableMultiMarkMode = m_bEnableMultiMarkMode;
		m_platformOptionInfo.m_bEnableAlignOffsetMarkByAera = m_bEnableAlignOffsetMarkByAera;

		m_platformOptionInfo.m_bEnableTargetAndObjectPositionVirtualMode = m_bEnableTargetAndObjectPositionVirtualMode;

		m_platformOptionInfo.m_bCalibrateChangeXDirection = m_bCalibrateChangeXDirection;
		m_platformOptionInfo.m_bCalibrateChangeYDirection = m_bCalibrateChangeYDirection;
		m_platformOptionInfo.m_bCalibrateChangeDDirection = m_bCalibrateChangeDDirection;

		//m_platformOptionInfo.m_bCalibSendAbsPos = m_bCalibSendAbsPos;
		m_platformOptionInfo.m_bCalibSendRelativePos = m_bCalibSendRelativePos;

		m_platformOptionInfo.m_bEnableCalibrateCheckBySize = m_bEnableCalibrateCheckBySize;
		m_platformOptionInfo.m_bEnableRelevanceCheckBySize = m_bEnableRelevanceCheckBySize;
		m_platformOptionInfo.m_bEnableCalibrateCheckByFixedTargetAligner = m_bEnableCalibrateCheckByFixedTargetAligner;

		m_platformOptionInfo.m_bFAALAlignOneTimeTargetSeachSnap = m_bFAALAlignOneTimeTargetSeachSnap;

		m_platformOptionInfo.m_bTargetSeachSnapEverytime = m_bFAALAlignMoreTimeTargetSeach;

	}
	else
	{
		map<pair<int,CString>,BOOL>::iterator iter;
		map<pair<int,CString>,BOOL>::iterator iterTmp;

		iter = m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
		iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();

        BOOL bSelectAll = TRUE;

		for(;(iter != m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.end())&&(iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end());iter++,iterTmp++)
		{
			iterTmp->second = iter->second;
            if (!iter->second)
            {
                bSelectAll = FALSE;
            }
		}

		//lzk
		/////////////////////////////////////////////////////////////////////////////////////
		SysPlatformInfo platformInfo;
		m_nListAligner = 0;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		//2vs2
		m_listCtrlAlignerType.DeleteAllItems();
		m_listCtrlAlignerType.DeleteColumn(0);
		//创建定位工具列表
		CRect	rcListRect;
		m_listCtrlAlignerType.GetClientRect(rcListRect);
		m_listCtrlAlignerType.ClientToScreen(rcListRect);
		ScreenToClient(rcListRect);
		m_listCtrlAlignerType.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
		//m_listCtrlAlignerType.InsertColumn(0,_T("选择使用的对位方式类型"),LVCFMT_LEFT,rcListRect.Width()*9/10);
		m_listCtrlAlignerType.InsertColumn(0,GetSysInfoString(m_psaSysInfoStrings,IDS_ALIGN_TYPE_SELECT),LVCFMT_LEFT,rcListRect.Width()*9/10);
		
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
			for (iter2Vs2 = m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();iter2Vs2 != m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iter2Vs2++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		//3vs3
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
			for (iter3Vs3 = m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();iter3Vs3 != m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iter3Vs3++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		//4vs4
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
			for (iter4Vs4 = m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();iter4Vs4 != m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iter4Vs4++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		if (m_nListAligner == 0)
		{
			m_listCtrlAlignerType.InsertItem(m_nListAligner,_T("没有合适的对位方式"));
		}
		//2vs2
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
			vector<Aligner2Vs2TypeInfo>::iterator iterTmp2Vs2;
			iter2Vs2 = m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();
			iterTmp2Vs2 = m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();
			for(;(iter2Vs2 != m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.end())&&(iterTmp2Vs2 != m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.end());iter2Vs2++,iterTmp2Vs2++)
			{
				iterTmp2Vs2->m_bOpenAligner = iter2Vs2->m_bOpenAligner;
				if (!iter2Vs2->m_bOpenAligner)
				{
					bSelectAll = FALSE;
				}
			}
		}
		//3vs3
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
			vector<Aligner3Vs3TypeInfo>::iterator iterTmp3Vs3;
			iter3Vs3 = m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();
			iterTmp3Vs3 = m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();
			for(;(iter3Vs3 != m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.end())&&(iterTmp3Vs3 != m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.end());iter3Vs3++,iterTmp3Vs3++)
			{
				iterTmp3Vs3->m_bOpenAligner = iter3Vs3->m_bOpenAligner;
				if (!iter3Vs3->m_bOpenAligner)
				{
					bSelectAll = FALSE;
				}
			}
		}
		//4vs4
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8/* && platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
			vector<Aligner4Vs4TypeInfo>::iterator iterTmp4Vs4;
			iter4Vs4 = m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();
			iterTmp4Vs4 = m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();
			for(;(iter4Vs4 != m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.end())&&(iterTmp4Vs4 != m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.end());iter4Vs4++,iterTmp4Vs4++)
			{
				iterTmp4Vs4->m_bOpenAligner = iter4Vs4->m_bOpenAligner;
				if (!iter4Vs4->m_bOpenAligner)
				{
					bSelectAll = FALSE;
				}
			}
		}


		/////////////////////////////////////////////////////////////////////////////////////////

		m_bSetOffsetTip = m_platformOptionInfo.m_OffsetShowParam.m_bSetOffsetTip;
		m_bShowOffsetTips = m_platformOptionInfo.m_OffsetShowParam.m_bShowOffsetTips;

        m_bChkSelectAll = bSelectAll;
        UpdateDlgSearchToolDataState();
		UpdateAlignerTypeInfoState();//lzk

		m_bSearchWithDistanceInspect = m_platformOptionInfo.m_bSearchWithDistanceInspect;
		m_bSendInsDistanceInSearchResult = m_platformOptionInfo.m_bSendInsDistanceInSearchResult;
		m_strOffsetPicturePath = m_platformOptionInfo.m_strOffsetPicturePath;
		m_bEnableDmCodeCalib = m_platformOptionInfo.m_bEnableDmCodeCalib;
		m_nTarInfoCommModeInDiffPC = m_platformOptionInfo.m_nTarInfoCommModeInDiffPC;
		m_bEnableAlignAfterSearch = m_platformOptionInfo.m_bEnableAlignAfterSearch;
		m_bRecordCamFixMarkPos = m_platformOptionInfo.m_bRecordCamFixMarkPos;
		m_bObjectSearchExProduct = m_platformOptionInfo.m_bObjectSearchExProduct;
		m_bTargetSearchExProduct = m_platformOptionInfo.m_bTargetSearchExProduct;
		m_bAlignExProduct = m_platformOptionInfo.m_bAlignExProduct;
		m_bSaveCalibData = m_platformOptionInfo.m_bSaveCalibData;
		m_bAutoCopyCalibData = m_platformOptionInfo.m_bAutoCopyCalibData;
		m_bSendAreaInspectSubResult = m_platformOptionInfo.m_bSendAreaInspectSubResult;

		m_strOffsetMaxX.Format(_T("%f"), m_platformOptionInfo.m_dOffsetMaxX);
		m_strOffsetMaxY.Format(_T("%f"), m_platformOptionInfo.m_dOffsetMaxY);
		m_strOffsetMaxD.Format(_T("%f"), m_platformOptionInfo.m_dOffsetMaxD);

		m_strAxisMoveMaxX.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxX);
		m_strAxisMoveMaxY.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxY);
		m_strAxisMoveMaxD.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxD);

		m_strAxisMoveMaxX_LowerLimit.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxX_LowerLimit);
		m_strAxisMoveMaxY_LowerLimit.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxY_LowerLimit);
		m_strAxisMoveMaxD_LowerLimit.Format(_T("%f"), m_platformOptionInfo.m_dAxisMoveMaxD_LowerLimit);
		// 像素判定
		m_bOmitRePickObjectEnable = m_platformOptionInfo.m_bOmitRePickObjectEnable;
		m_strRePickOffsetMaxX.Format(_T("%f"), m_platformOptionInfo.m_dRePickOffsetMaxX);
		m_strRePickOffsetMaxY.Format(_T("%f"), m_platformOptionInfo.m_dRePickOffsetMaxY);

		// 实际物理坐标判定
		m_bOmitRePickObjectEnable_MM = m_platformOptionInfo.m_bOmitRePickObjectEnable_MM;
		m_strRePickOffsetMaxX_MM.Format(_T("%f"), m_platformOptionInfo.m_dRePickOffsetMaxX_MM);
		m_strRePickOffsetMaxY_MM.Format(_T("%f"), m_platformOptionInfo.m_dRePickOffsetMaxY_MM);

		m_bEnableCalibrateCheckByAxisMoveOption = m_platformOptionInfo.m_bEnableCalibrateCheckByAxisMoveOption;
		m_dCalibrateCheckAxisMoveRangeX = m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeX;
		m_dCalibrateCheckAxisMoveRangeY = m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeY;
		m_dCalibrateCheckAxisMoveRangeD = m_platformOptionInfo.m_dCalibrateCheckAxisMoveRangeD;
		m_strCalibrateCheckAxisMoveRangeX.Format(_T("%f"), m_dCalibrateCheckAxisMoveRangeX);
		m_strCalibrateCheckAxisMoveRangeY.Format(_T("%f"), m_dCalibrateCheckAxisMoveRangeY);
		m_strCalibrateCheckAxisMoveRangeD.Format(_T("%f"), m_dCalibrateCheckAxisMoveRangeD);
		m_cmbTarAutoCalibAlignType.SetCurSel((int)m_platformOptionInfo.m_eTarAutoCalibAlignMode);

		m_bChkSendAlignMulResults = m_platformOptionInfo.m_bSendMulAlignResult;
		m_bReadWriteAxisPosWithCmd = m_platformOptionInfo.m_bReadWriteAxisPosWithCmd;

		// 原选项内容
		m_bEnableCommMultiCalibExtension = m_platformOptionInfo.m_bEnableCommMultiCalibExtension;
		m_bVisionOnLineStatus = m_platformOptionInfo.m_bVisionOnLineStatus;
		m_bEnableAlignBenchCenter = m_platformOptionInfo.m_bEnableAlignBenchCenter;
		m_bSetVisionManualSearchToPLC = m_platformOptionInfo.m_bSetVisionManualSearchToPLC;

		m_bSetVisionManualSearchToPLC2 = m_platformOptionInfo.m_bSetVisionManualSearchToPLC2;
		m_nVisionManualSearchToPlcAddre = m_platformOptionInfo.m_nVisionManualSearchToPlcAddre;

		m_bAlignOneTime = m_platformOptionInfo.m_bAlignOneTime;
		m_bEnableMutiTarget = m_platformOptionInfo.m_bEnableMutiTarget;
		m_bObjMulSearchPosXYPD = m_platformOptionInfo.m_bObjMulSearchPosXYPD;
		m_bTargetAngleMulAlignerXYPD = m_platformOptionInfo.m_bTargetAngleMulAlignerXYPD;
		m_bLastPickSetting = m_platformOptionInfo.m_bLastPickSetting;
		m_bEnablePickPosUpdate = m_platformOptionInfo.m_bEnablePickPosUpdate;
		m_bAskStagePosEachMove = m_platformOptionInfo.m_bAskStagePosEachMove;
		m_nSearchDelayTime = m_platformOptionInfo.m_nSearchDelayTime;
		m_bTargetPosSame = m_platformOptionInfo.m_bTargetPosSame;
		m_bObjectPosSame = m_platformOptionInfo.m_bObjectPosSame;
		m_bSaveAlignPos = m_platformOptionInfo.m_bSaveAlignPos;
		m_bEnableSearchCindidate = m_platformOptionInfo.m_bEnableSearchCindidate;
		m_bSearchTimes = m_platformOptionInfo.m_bSearchTimes;
		m_nSearchTimes = m_platformOptionInfo.m_nSearchTimes;
		m_bEnableSearchShutter = m_platformOptionInfo.m_bEnableSearchShutter;
		m_bShowEightShapeAlign = m_platformOptionInfo.m_bShowEightShapeAlign;
//		m_bShowPCB = m_platformOptionInfo.m_bShowPCB;
		m_bShowFWEL = m_platformOptionInfo.m_bShowFWEL;
		m_bShowBend = m_platformOptionInfo.m_bShowBend;
		m_bManualAfTarSearchFail = m_platformOptionInfo.m_bManualAfTarSearchFail;
		m_bManualAfObjSearchFail = m_platformOptionInfo.m_bManualAfObjSearchFail;
		m_bAlignFinishClearSnapStatus = m_platformOptionInfo.m_bAlignFinishClearSnapStatus;

		m_bExeObjectCheckModeWhenAlign = m_platformOptionInfo.m_bExeObjectCheckModeWhenAlign;
		m_bExeTargetCheckModeWhenAlign = m_platformOptionInfo.m_bExeTargetCheckModeWhenAlign;

		m_bEnablePhotoLightCtrl = m_platformOptionInfo.m_bEnablePhotoLightCtrl;

		m_bManualAfTarSearchFailShowButton = m_platformOptionInfo.m_bManualAfTarSearchFailShowButton;
		m_bManualAfObjSearchFailShowButton = m_platformOptionInfo.m_bManualAfObjSearchFailShowButton;
		//m_bInspectEnable = m_platformOptionInfo.m_bInspectEnable;

		m_bTargetUseFixedVirtualCoordinates = m_platformOptionInfo.m_bTargetUseFixedVirtualCoordinates;
		m_bEnableMultiMarkMode = m_platformOptionInfo.m_bEnableMultiMarkMode;
		m_bEnableAlignOffsetMarkByAera = m_platformOptionInfo.m_bEnableAlignOffsetMarkByAera;
		m_bEnableTargetAndObjectPositionVirtualMode = m_platformOptionInfo.m_bEnableTargetAndObjectPositionVirtualMode;

		m_bCalibrateChangeXDirection = m_platformOptionInfo.m_bCalibrateChangeXDirection;
		m_bCalibrateChangeYDirection = m_platformOptionInfo.m_bCalibrateChangeYDirection;
		m_bCalibrateChangeDDirection = m_platformOptionInfo.m_bCalibrateChangeDDirection;

		//m_bCalibSendAbsPos = m_platformOptionInfo.m_bCalibSendAbsPos;
		m_bCalibSendRelativePos = m_platformOptionInfo.m_bCalibSendRelativePos;

		m_bEnableCalibrateCheckBySize = m_platformOptionInfo.m_bEnableCalibrateCheckBySize;
		m_bEnableRelevanceCheckBySize = m_platformOptionInfo.m_bEnableRelevanceCheckBySize;
		m_bEnableCalibrateCheckByFixedTargetAligner = m_platformOptionInfo.m_bEnableCalibrateCheckByFixedTargetAligner;
		m_bEnablePickSendRelativateAxis = m_platformOptionInfo.m_bEnablePickSendRelativateAxis;
		m_bFAALAlignOneTimeTargetSeachSnap=m_platformOptionInfo.m_bFAALAlignOneTimeTargetSeachSnap ;

		m_bFAALAlignMoreTimeTargetSeach = m_platformOptionInfo.m_bTargetSeachSnapEverytime;

		//if (platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar)
		//{
		//	GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_SHOW);
		//	GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_SHOW);
		//}
		//else
		//{
		//	GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_HIDE);
		//}

		BOOL bEnableTargetAutoCalib = platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar;
		GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
		GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
		GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(m_bShowCalibrateCtl);
		GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(m_bShowCalibrateCtl);

		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);

		//// 拾取相关设置的启用/禁用（因为缺少布局，目前采用禁用的处理）
		//SetGroupBoxStatus(IDC_STATIC_PICK_SETTING,platformInfo.m_bPlatformPick,TRUE);

		//// XY+D相关的启用/禁用
		//SetGroupBoxStatus(IDC_STATIC_XYPD_SETTING,platformInfo.m_eMidPlatformType== PlatformType::ePlatformXDPY,TRUE);
		//
		//// 产品扩展相关
		//SetGroupBoxStatus(IDC_STC_PRODUCT_COMM_EX,platformInfo.m_bEnableMultiCalibExtension && vsConfig->m_CommParamInfo.m_eExProtocolType == ExProtocolType::eExProReserved1,TRUE);
		//
		CString strDir;
		if (GetAndCheckOffsetPicturePath(strDir))
		{
			strDir = strDir + _T("\\")+m_strOffsetPicturePath;
			ShowOffsetthumbnail(strDir);
		}
		UpdateData(FALSE);
	}
}

DlgSearchToolInfo CDlgSysPlatformOptionParam::GetDlgSearchToolData() const
{
	return m_dlgSearchToolInfo;
}

void CDlgSysPlatformOptionParam::SetDlgSearchToolData(const DlgSearchToolInfo& searchToolData)
{
	m_dlgSearchToolInfo = searchToolData;
}

//lzk
AlignerTypeInfo	CDlgSysPlatformOptionParam::GetAlignerTypeInfo() const
{
	return m_alignerTypeInfo;
}
//lzk
void CDlgSysPlatformOptionParam::SetAlignerTypeInfo(const AlignerTypeInfo& alignerTypeInfo)
{
	m_alignerTypeInfo = alignerTypeInfo;
}

void CDlgSysPlatformOptionParam::SetPlatformOptionInfo(const CPlatformOptionInfo& optionInfo)
{
	m_platformOptionInfo = optionInfo;
}

CPlatformOptionInfo CDlgSysPlatformOptionParam::GetPlatformOptionInfo()const
{
	return m_platformOptionInfo;
}

BOOL CDlgSysPlatformOptionParam::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message == WM_SYSKEYDOWN)&& (pMsg->wParam == VK_F4)) return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgSysPlatformOptionParam::InitDlgItem()
{
	UpdateDlgByAuthorize();

	m_listCtrlSearchTool.DeleteAllItems();
	m_listCtrlSearchTool.DeleteColumn(0);
    
    
	//创建定位工具列表
	CRect	rcListRect;
	m_listCtrlSearchTool.GetClientRect(rcListRect);
	m_listCtrlSearchTool.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
    
	m_listCtrlSearchTool.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_listCtrlSearchTool.InsertColumn(0,GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_SEARCH_TOOL)/*_T("状态")*/,LVCFMT_LEFT,rcListRect.Width()*9/10);
	
	//lzk
	m_listCtrlAlignerType.DeleteAllItems();
	m_listCtrlAlignerType.DeleteColumn(0);
	//创建定位工具列表
	m_listCtrlAlignerType.GetClientRect(rcListRect);
	m_listCtrlAlignerType.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
	m_listCtrlAlignerType.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//m_listCtrlAlignerType.InsertColumn(0,_T("选择使用的对位方式类型"),LVCFMT_LEFT,rcListRect.Width()*9/10);
	m_listCtrlAlignerType.InsertColumn(0,GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_ALIGNER_TYPE),LVCFMT_LEFT,rcListRect.Width()*9/10);
	
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		//////////////////////////////////////////////////////////////////////////
		//if (((platformInfo.m_bPlatformPick && platformInfo.m_ePickCamPlatformType == ePickCamPlatformSepFix)  || (platformInfo.m_eMidPlatformType == ePlatformXYD) || (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)|| (platformInfo.m_eMidPlatformType == ePlatformXY1Y2) || (platformInfo.m_eMidPlatformType == ePlatformXY) || (platformInfo.m_eMidPlatformType == ePlatformXD)|| (platformInfo.m_eMidPlatformType == ePlatformYD)))
		//{
		//	int nShowCalibCheckByAxis = m_bShowCalibrateCtl?SW_SHOW:SW_HIDE;
		//	for (int i=0;i<8;i++)
		//	{
		//		GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->ShowWindow(nShowCalibCheckByAxis);
		//		GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->EnableWindow(m_bEnableCalibrateCtl);
		//	}

		//}
		//else
		//{
		//	for (int i=0;i<8;i++)
		//	{
		//		GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->ShowWindow(SW_HIDE);
		//		GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->EnableWindow(FALSE);
		//	}
		//}
		//////////////////////////////////////////////////////////////////////////

		vector<map<pair<int,int>,BOOL>>::iterator   iterSys;
		map<pair<int,CString>,BOOL>::iterator iterDlg;
		iterSys = platformInfo.m_vmapSysSearchToolInfo.begin();	
		
		for (;iterSys != platformInfo.m_vmapSysSearchToolInfo.end();iterSys++)
		{

			if ((*iterSys).begin()->second)
			{
				for (iterDlg = m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
				{
					if ((*iterSys).begin()->first.first == iterDlg->first.first)
					{
						m_listCtrlSearchTool.InsertItem(m_nListLen,GetSysInfoString(m_psaSysInfoStrings,(*iterSys).begin()->first.first));
					//	m_listCtrlSearchTool.SetCheck(m_nListLen,iterDlg->second);
                        
                        m_nListLen++;
						break;
					}
				}
			}
		}

		//lzk 对位方式信息
		/////////////////////////////////////////////////////////////////////////////////////
		//2vs2
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
			for (iter2Vs2 = m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();iter2Vs2 != m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iter2Vs2++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter2Vs2->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		//3vs3
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
			for (iter3Vs3 = m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();iter3Vs3 != m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iter3Vs3++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter3Vs3->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		//4vs4
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
			for (iter4Vs4 = m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();iter4Vs4 != m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iter4Vs4++)
			{
				m_listCtrlAlignerType.InsertItem(m_nListAligner,GetSysInfoString(m_psaSysInfoStrings,iter4Vs4->m_nAlignerTypeID));
				m_nListAligner++;			
			}
		}
		if (m_nListAligner == 0)
		{
			m_listCtrlAlignerType.InsertItem(m_nListAligner,_T("没有合适的对位方式"));
		}

		/////////////////////////////////////////////////////////////////////////////////////////

		if (m_nListLen == 0)   //列表为空，则使用m_dlgSearchToolInfo.m_mapDlgSearchToolInfo中的第一个
		{
			m_listCtrlSearchTool.InsertItem(m_nListLen,GetSysInfoString(m_psaSysInfoStrings,m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin()->first.first));
			//m_listCtrlSearchTool.SetCheck(m_nListLen,TRUE);
			m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin()->second = TRUE;
		}

		m_cmbTarAutoCalibAlignType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_TAR_AUTOCALIB_ALING_ON_OBJECT));
		m_cmbTarAutoCalibAlignType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_TAR_AUTOCALIB_ALING_TO_TARGET));
        m_cmbTarAutoCalibAlignType.SetCurSel(0);
		//if (platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar)
		//{
		//	GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_SHOW);
		//	GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_SHOW);
		//}
		//else
		//{
		//	GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_HIDE);
		//	GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(SW_HIDE);
		//}
		BOOL bEnableTargetAutoCalib = platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar;
		GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
		GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
		GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow( m_bShowCalibrateCtl);
		GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow( m_bShowCalibrateCtl);
		

		// 超级权限
		//SetEnableAllControl(m_bEnableAllControl && (m_pVisionASM->GetUsePassWordMode() == 0));
	}
}

void CDlgSysPlatformOptionParam::UpdateDlgSearchToolDataState()
{
    if (m_pVisionASM != NULL)
    {
        int nNum = 0;
        SysPlatformInfo platformInfo;
        m_pVisionASM->GetSysPlatformInfo(platformInfo);


        vector<map<pair<int,int>,BOOL>>::iterator   iterSys;
        map<pair<int,CString>,BOOL>::iterator iterDlg;
        iterSys = platformInfo.m_vmapSysSearchToolInfo.begin();	

        for (;iterSys != platformInfo.m_vmapSysSearchToolInfo.end();iterSys++)
        {

            if ((*iterSys).begin()->second)
            {
                for (iterDlg = m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != m_dlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
                {
                    if ((*iterSys).begin()->first.first == iterDlg->first.first)
                    {
                        //m_listCtrlSearchTool.InsertItem(m_nListLen,GetSysInfoString(m_psaSysInfoStrings,(*iterSys).begin()->first.first));
                        m_listCtrlSearchTool.SetCheck(nNum,iterDlg->second);

                        nNum++;
                        break;
                    }
                }
            }
        }

    }
}

//lzk
void CDlgSysPlatformOptionParam::UpdateAlignerTypeInfoState()
{
	if (m_pVisionASM != NULL)
	{
		int nNum = 0;
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		//2vs2
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
			/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
		{
			vector<Aligner2Vs2TypeInfo>::iterator iter2Vs2;
			for (iter2Vs2 = m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();iter2Vs2 != m_alignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iter2Vs2++)
			{
				m_listCtrlAlignerType.SetCheck(nNum,iter2Vs2->m_bOpenAligner);
				nNum++;		
			}
		}
		//3vs3
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
		{
			vector<Aligner3Vs3TypeInfo>::iterator iter3Vs3;
			for (iter3Vs3 = m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();iter3Vs3 != m_alignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iter3Vs3++)
			{
				m_listCtrlAlignerType.SetCheck(nNum,iter3Vs3->m_bOpenAligner);
				nNum++;		
			}
		}
		//4vs4
		if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
			|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
		{
			vector<Aligner4Vs4TypeInfo>::iterator iter4Vs4;
			for (iter4Vs4 = m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();iter4Vs4 != m_alignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iter4Vs4++)
			{
				m_listCtrlAlignerType.SetCheck(nNum,iter4Vs4->m_bOpenAligner);
				nNum++;		
			}
		}
		if (nNum == 0)
		{

		}
	}
}

void CDlgSysPlatformOptionParam::SetVisionASMRef(vcBaseVisionAlign *pVisionASM)
{
	m_pVisionASM = pVisionASM;
}

void CDlgSysPlatformOptionParam::OnNMClickSearchToolList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	DWORD dwPos = GetMessagePos();
	CPoint pt(dwPos);
	m_listCtrlSearchTool.ScreenToClient(&pt);
	UINT nFlag;
	
	int nItem = m_listCtrlSearchTool.HitTest(pt,&nFlag);
	if (nFlag == LVHT_ONITEMSTATEICON)
	{
		int nSearchToolNum =0;
		int nTmpDlgNum = 0;
		map<pair<int,CString>,BOOL> mapTmpOld;
		mapTmpOld = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo;

		

		if (m_pVisionASM != NULL)
		{
			SysPlatformInfo platformInfo;
			m_pVisionASM->GetSysPlatformInfo(platformInfo);
			//map<pair<int,int>,pair<BOOL,int>>
		   //         排序,ID     是否使用,标识符
			vector<map<pair<int,int>,BOOL >>::iterator iterPlat;
			iterPlat = platformInfo.m_vmapSysSearchToolInfo.begin();
			int nPlatNum = 0;
			int nToolNum = 0; 
			for (;iterPlat != platformInfo.m_vmapSysSearchToolInfo.end(); iterPlat++ ) //查找列表项对应的int型工具名称
			{
				if ((*iterPlat).begin()->second )
				{
					nPlatNum ++;
				}
				if ((nPlatNum -1) == nItem)
				{
					nToolNum = (*iterPlat).begin()->first.first;
					break;
				}
			}                                                     
			map<pair<int,CString>,BOOL>::iterator iterTmp;                          //map<pair<int,CString>,BOOL>
			iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();			//      ID,保存名字,是否使用


			for (;iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)  //将值传给在m_tmpDlgSearchToolInfo中对应的定位工具
			{
				if (iterTmp->first.first == nToolNum)
				{
					iterTmp->second = !m_listCtrlSearchTool.GetCheck(nItem);

                    CString strToolName = m_listCtrlSearchTool.GetItemText(nItem,0);
                    CString strLog,strValue;
                    strValue = iterTmp->second ? _T("选中状态"):_T("未选中状态");
                    strLog.Format(_T("修改[%s]为%s\n"),strToolName,strValue);
                    g_opLogFile.SaveLogText(strLog,3);
					break;
				}
			}

			//遍历查看列表中定位工具是否都未选中
			iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
			for (;iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)
			{
				if (iterTmp->second)
				{
					nSearchToolNum++;
				}
			}

			if (nSearchToolNum == 0)       //未选定任何定位工具而进行保存时，默认选定初始选定定位工具的第一个
			{
				map<pair<int,CString>,BOOL>::iterator iterTmpOld;
				iterTmpOld = mapTmpOld.begin();
				iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
				for (;iterTmpOld != mapTmpOld.end() && iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++,iterTmpOld++)
				{
					if (iterTmpOld->second)
					{
						iterTmp->second = TRUE;
						break;
					}
				}
				AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_CHOOSE_SEARCH_TOOL));  //_T("定位工具不能为空,已选定当前定位工具！")
			}	
		}
		if (nSearchToolNum == 0)
		{
			UpdateDlgData(TRUE);
			InitDlgItem();           
			//UpdateDlgData(FALSE);

		}
        if (IsAllSearchToolSelected())
        {
            m_bChkSelectAll = TRUE;
        }
        else
        {
            m_bChkSelectAll = FALSE;
        }
        UpdateData(FALSE);
	}

	*pResult = 0;

}

BOOL  CDlgSysPlatformOptionParam::IsAllSearchToolSelected()
{
    BOOL bAllSelected = TRUE;
    map<pair<int,CString>,BOOL>::iterator iterTmp;
    for (iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)
    {
        if (!iterTmp->second)
        {
            bAllSelected = FALSE;
            break;
        }
    }
    return bAllSelected;
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckSelectAll()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bChkSelectAll)
	{
		for(int i = 0;i < m_listCtrlSearchTool.GetItemCount();i++) 
		{

			m_listCtrlSearchTool.SetCheck(i, TRUE);

			m_listCtrlSearchTool.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
		if (m_pVisionASM != NULL)
		{
			SysPlatformInfo platformInfo;
			m_pVisionASM->GetSysPlatformInfo(platformInfo);



			vector<map<pair<int,int>,BOOL>>::iterator iterSys;
			map<pair<int,CString>,BOOL>::iterator iterTmp;
			iterSys = platformInfo.m_vmapSysSearchToolInfo.begin();		
			for (;iterSys != platformInfo.m_vmapSysSearchToolInfo.end();iterSys++)
			{
				if ((*iterSys).begin()->second)
				{
					for (iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)
					{
						if ((*iterSys).begin()->first.first == iterTmp->first.first)
						{
							iterTmp->second = TRUE;
							break;
						}
					}
				}
			}
		}
	}
	
    CString strLog,strValue;
    strValue = m_bChkSelectAll ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[全选]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,3);
}

void CDlgSysPlatformOptionParam::OnBnClickedButtonResetSearchTool()
{
	// TODO: 在此添加控件通知处理程序代码

	for(int i = 0;i < m_listCtrlSearchTool.GetItemCount();i++) 
	{

		m_listCtrlSearchTool.SetCheck(i, FALSE);

		m_listCtrlSearchTool.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);


		vector<map<pair<int,int>,BOOL>>::iterator iterSys;
		map<pair<int,CString>,BOOL>::iterator iterDlg;
		iterSys = platformInfo.m_vmapSysSearchToolInfo.begin();	

		for (iterDlg= m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
		{
			iterDlg->second = FALSE;
		}

		int nListNum = 0;

		for (;iterSys != platformInfo.m_vmapSysSearchToolInfo.end();iterSys++)
		{
			if ((*iterSys).begin()->second)
			{
				for (iterDlg = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
				{
					if ((*iterSys).begin()->first.first == iterDlg->first.first)
					{
						switch ((*iterSys).begin()->first.first)
						{
						case IDS_CMB_QUICK_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_PATTERN_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_CORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_CIRCLE_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_FIXEDPOS_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_PATTERNCORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MPATTERN_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MQUICK_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MQUICK_CORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MPATTERNCORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
						case IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL:
							iterDlg->second = TRUE;
							m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
							m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
							break;
                        case IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL:
                            iterDlg->second = TRUE;
                            m_listCtrlSearchTool.SetCheck(nListNum, TRUE);
                            m_listCtrlSearchTool.SetItemState(nListNum, LVIS_SELECTED, LVIS_SELECTED);
                            break;
						}
					}
				}
				nListNum ++;
			}
		}

        m_bChkSelectAll = FALSE;
        UpdateData(FALSE);
	}

    CString strLog;
    strLog.Format(_T("点击[恢复默认设置]\n"));
    g_opLogFile.SaveLogText(strLog,3);
}

void CDlgSysPlatformOptionParam::OnNMDblclkSearchToolList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    DWORD dwPos = GetMessagePos();
    CPoint pt(dwPos);
    m_listCtrlSearchTool.ScreenToClient(&pt);
    UINT nFlag;

    int nItem = m_listCtrlSearchTool.HitTest(pt,&nFlag);
    if (nFlag == LVHT_ONITEMSTATEICON)
    {
        int nSearchToolNum =0;
        int nTmpDlgNum = 0;
        map<pair<int,CString>,BOOL> mapTmpOld;
        mapTmpOld = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo;



        if (m_pVisionASM != NULL)
        {
            SysPlatformInfo platformInfo;
            m_pVisionASM->GetSysPlatformInfo(platformInfo);
            //map<pair<int,int>,pair<BOOL,int>>
            //         排序,ID     是否使用,标识符
            vector<map<pair<int,int>,BOOL >>::iterator iterPlat;
            iterPlat = platformInfo.m_vmapSysSearchToolInfo.begin();
            int nPlatNum = 0;
            int nToolNum = 0; 
            for (;iterPlat != platformInfo.m_vmapSysSearchToolInfo.end(); iterPlat++ ) //查找列表项对应的int型工具名称
            {
                if ((*iterPlat).begin()->second )
                {
                    nPlatNum ++;
                }
                if ((nPlatNum -1) == nItem)
                {
                    nToolNum = (*iterPlat).begin()->first.first;
                    break;
                }
            }                                                     
            map<pair<int,CString>,BOOL>::iterator iterTmp;                          //map<pair<int,CString>,BOOL>
            iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();			//      ID,保存名字,是否使用


            for (;iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)  //将值传给在m_tmpDlgSearchToolInfo中对应的定位工具
            {
                if (iterTmp->first.first == nToolNum)
                {
                    iterTmp->second = !m_listCtrlSearchTool.GetCheck(nItem);
                    break;
                }
            }

            //遍历查看列表中定位工具是否都未选中
            iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
            for (;iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++)
            {
                if (iterTmp->second)
                {
                    nSearchToolNum++;
                }
            }

            if (nSearchToolNum == 0)       //未选定任何定位工具而进行保存时，默认选定初始选定定位工具的第一个
            {
                map<pair<int,CString>,BOOL>::iterator iterTmpOld;
                iterTmpOld = mapTmpOld.begin();
                iterTmp = m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();
                for (;iterTmpOld != mapTmpOld.end() && iterTmp != m_tmpDlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterTmp++,iterTmpOld++)
                {
                    if (iterTmpOld->second)
                    {
                        iterTmp->second = TRUE;
                        break;
                    }
                }
                AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_CHOOSE_SEARCH_TOOL));  //_T("定位工具不能为空,已选定当前定位工具！")
            }	
        }
        if (nSearchToolNum == 0)
        {
            UpdateDlgData(TRUE);
            InitDlgItem();           
            //UpdateDlgData(FALSE);

        }
        if (IsAllSearchToolSelected())
        {
            m_bChkSelectAll = TRUE;
        }
        else
        {
            m_bChkSelectAll = FALSE;
        }
        UpdateData(FALSE);
    }
    *pResult = 0;
}

void CDlgSysPlatformOptionParam::OnBnClickedBtnSetOffsetPicture()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strEXEDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\')); 

	CString filter = _T("图片(*.BMP)|*.BMP||");
	CFileDialog dlg(TRUE,NULL,strEXEDir,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter,NULL);
	if(dlg.DoModal())
	{
		//m_strOffsetPicturePath = dlg.GetPathName();
		m_strOffsetPicturePath = dlg.GetFileName();
		// 	// 获取当前EXE文件夹的路径
		CString strDir;
		if (GetAndCheckOffsetPicturePath(strDir))
		{
			strDir = strDir + _T("\\")+m_strOffsetPicturePath;
			if(CopyFile(dlg.GetPathName(),strDir,FALSE))
			{
				ShowOffsetthumbnail(strDir);
			}
			
		}	
	}	
}

void CDlgSysPlatformOptionParam::ShowOffsetthumbnail(CString strDir)
{
	//HBITMAP hbmp;
	CRect rc;
	GetDlgItem(IDC_STC_OFFSET_PICTURE)->GetClientRect(rc);
	DeleteObject(m_hBmpOffsetDir);
	m_hBmpOffsetDir = NULL;
	m_hBmpOffsetDir = (HBITMAP)::LoadImage(NULL,strDir,IMAGE_BITMAP,rc.Width(),rc.Height(),LR_LOADFROMFILE|LR_DEFAULTSIZE);
	if (m_hBmpOffsetDir)
	{
		GetDlgItem(IDC_STC_OFFSET_PICTURE)->ShowWindow(m_bShowOffsetCtl);
		((CStatic*)GetDlgItem(IDC_STC_OFFSET_PICTURE))->SetBitmap((HBITMAP)m_hBmpOffsetDir);
	}
	else
	{
		GetDlgItem(IDC_STC_OFFSET_PICTURE)->ShowWindow(SW_HIDE);
	}
}

BOOL CDlgSysPlatformOptionParam::GetAndCheckOffsetPicturePath(CString &strDir)
{
	CString strEXEDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\')); 
	CFileFind findFile;
	BOOL IsFild, IsDir;
	SysPlatformInfo platInfo;

	m_pVisionASM->GetSysPlatformInfo(platInfo);
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
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDlgSysPlatformOptionParam::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

}

BOOL CDlgSysPlatformOptionParam::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgSysPlatformOptionParam::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
		if (((ch > '9') || (ch < '0')) && (ch != '.'))
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

void CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大补偿值][X轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxX();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, m_strOffsetMaxX);
		}
		else
		{
			m_strOffsetMaxX.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, m_strOffsetMaxX);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大补偿值][Y轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxY();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, m_strOffsetMaxY);
		}
		else
		{
			m_strOffsetMaxY.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, m_strOffsetMaxY);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditOffsetMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大补偿值][θ轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxD();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditOffsetMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, m_strOffsetMaxD);
		}
		else
		{
			m_strOffsetMaxD.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, m_strOffsetMaxD);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-上限][X轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxX();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X, strVal);
		if (!IsStringNumerical(strVal) || atof(strVal) <= atof(m_strAxisMoveMaxX_LowerLimit))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X, m_strAxisMoveMaxX);
		}
		else
		{
			m_strAxisMoveMaxX.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X, m_strAxisMoveMaxX);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-上限][Y轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxY();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y, strVal);
		if (!IsStringNumerical(strVal)|| atof(strVal) <= atof(m_strAxisMoveMaxY_LowerLimit))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y, m_strAxisMoveMaxY);
		}
		else
		{
			m_strAxisMoveMaxY.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y, m_strAxisMoveMaxY);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-上限][θ轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxD();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D, strVal);
		if (!IsStringNumerical(strVal) || atof(strVal) <= atof(m_strAxisMoveMaxD_LowerLimit))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D, m_strAxisMoveMaxD);
		}
		else
		{
			m_strAxisMoveMaxD.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D, m_strAxisMoveMaxD);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxX2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_X2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-下限][X轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxX2();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxX2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X2, strVal);
		// 下限不允许超过上限
		if (!IsStringNumerical(strVal) || atof(strVal)>= atof(m_strAxisMoveMaxX))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X2, m_strAxisMoveMaxX_LowerLimit);
		}
		else
		{
			m_strAxisMoveMaxX_LowerLimit.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_X2, m_strAxisMoveMaxX_LowerLimit);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxY2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_Y2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-下限][Y轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxY2();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxY2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y2, strVal);
		if (!IsStringNumerical(strVal) || atof(strVal)>= atof(m_strAxisMoveMaxY))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y2, m_strAxisMoveMaxY_LowerLimit);
		}
		else
		{
			m_strAxisMoveMaxY_LowerLimit.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_Y2, m_strAxisMoveMaxY_LowerLimit);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditAxisMoveMaxD2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_AXIS_MOVE_MAX_D2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[最大移动值-下限][θ轴]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditAxisMoveMaxD2();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditAxisMoveMaxD2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D2, strVal);
		if (!IsStringNumerical(strVal) || atof(strVal)>= atof(m_strAxisMoveMaxD))
		{
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D2, m_strAxisMoveMaxD_LowerLimit);
		}
		else
		{
			m_strAxisMoveMaxD_LowerLimit.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_AXIS_MOVE_MAX_D2, m_strAxisMoveMaxD_LowerLimit);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REPICK_OFFSET_MAX_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[忽略拾取不起来的产品][X(pixel)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRepickOffsetMaxX();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X, m_strRePickOffsetMaxX);
		}
		else
		{
			m_strRePickOffsetMaxX.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X, m_strRePickOffsetMaxX);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REPICK_OFFSET_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[忽略拾取不起来的产品][Y(pixel)]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRepickOffsetMaxY();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y, m_strRePickOffsetMaxY);
		}
		else
		{
			m_strRePickOffsetMaxY.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y, m_strRePickOffsetMaxY);
		}
	}	
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxXMm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REPICK_OFFSET_MAX_X_MM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRepickOffsetMaxXMm();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxXMm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, m_strRePickOffsetMaxX_MM);
		}
		else
		{
			m_strRePickOffsetMaxX_MM.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_X_MM, m_strRePickOffsetMaxX_MM);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditRepickOffsetMaxYMm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRepickOffsetMaxYMm();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditRepickOffsetMaxYMm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, m_strRePickOffsetMaxY_MM);
		}
		else
		{
			m_strRePickOffsetMaxY_MM.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_REPICK_OFFSET_MAX_Y_MM, m_strRePickOffsetMaxY_MM);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeX()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValAxisMoveRangeX();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeX()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, m_strCalibrateCheckAxisMoveRangeX);
		}
		else
		{
			m_strCalibrateCheckAxisMoveRangeX.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_X, m_strCalibrateCheckAxisMoveRangeX);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeY()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValAxisMoveRangeY();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeY()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, m_strCalibrateCheckAxisMoveRangeY);
		}
		else
		{
			m_strCalibrateCheckAxisMoveRangeY.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_Y, m_strCalibrateCheckAxisMoveRangeY);
		}
	}
}

void CDlgSysPlatformOptionParam::OnEnSetfocusEditValAxisMoveRangeD()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValAxisMoveRangeD();
	}	
}

void CDlgSysPlatformOptionParam::OnEnKillfocusEditValAxisMoveRangeD()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, m_strCalibrateCheckAxisMoveRangeD);
		}
		else
		{
			m_strCalibrateCheckAxisMoveRangeD.Format(_T("%f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_AXIS_MOVE_RANGE_D, m_strCalibrateCheckAxisMoveRangeD);
		}
	}
}

void CDlgSysPlatformOptionParam::OnNMClickListAlignerType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPos = GetMessagePos();
	CPoint pt(dwPos); 
	m_listCtrlAlignerType.ScreenToClient(&pt);
	UINT nFlag;
	int nItem = m_listCtrlAlignerType.HitTest(pt,&nFlag);
	if (nFlag == LVHT_ONITEMSTATEICON)
	{ 
		int nAlignerNum =0;
		AlignerTypeInfo alignerTmpOld;
		alignerTmpOld = m_tmpAlignerTypeInfo;
		if (m_pVisionASM != NULL)
		{
			SysPlatformInfo platformInfo;
			m_pVisionASM->GetSysPlatformInfo(platformInfo);  
			//2vs2
			if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 2) 
				|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 4)
				/*|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_bTarCamWithDiffObjCam == TRUE && platformInfo.m_nPositionNum == 8)*/)
			{
				vector<Aligner2Vs2TypeInfo>::iterator iterTmp2Vs2;
				iterTmp2Vs2 = m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();			
				for (;iterTmp2Vs2 != m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iterTmp2Vs2++)  
				{
					CString strAlignerName = m_listCtrlAlignerType.GetItemText(nItem,0);
					if (strAlignerName == GetSysInfoString(m_psaSysInfoStrings,iterTmp2Vs2->m_nAlignerTypeID))
					{
						iterTmp2Vs2->m_bOpenAligner = !m_listCtrlAlignerType.GetCheck(nItem);
						CString strLog,strValue;
						strValue = iterTmp2Vs2->m_bOpenAligner ? _T("选中状态"):_T("未选中状态");
						strLog.Format(_T("修改[%s]为%s\n"),strAlignerName,strValue);
						g_opLogFile.SaveLogText(strLog,3);
						break;
					}
				}
				iterTmp2Vs2 = m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();			
				for (;iterTmp2Vs2 != m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iterTmp2Vs2++) 
				{
					if (iterTmp2Vs2->m_bOpenAligner)
					{
						nAlignerNum++;
					}
				}
				//如果全部不选中
				if (nAlignerNum == 0)     
				{
					vector<Aligner2Vs2TypeInfo>::iterator iterTmp2Vs2Old;
					iterTmp2Vs2Old = alignerTmpOld.m_vAligner2Vs2TypeInfo.begin();				
					iterTmp2Vs2 = m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.begin();
					for (;iterTmp2Vs2Old != alignerTmpOld.m_vAligner2Vs2TypeInfo.end() && iterTmp2Vs2 != m_tmpAlignerTypeInfo.m_vAligner2Vs2TypeInfo.end();iterTmp2Vs2++,iterTmp2Vs2Old++)
					{
						if (iterTmp2Vs2Old->m_bOpenAligner)
						{
							iterTmp2Vs2->m_bOpenAligner = TRUE;
							break;
						}
					}
					AfxMessageBox(_T("对位方式不能全部为空，已选定当前对位方式"));  //_T("定位工具不能为空,已选定当前定位工具！")
				}
			}
			//3vs3
			if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 3) 
				|| (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 6))
			{
				vector<Aligner3Vs3TypeInfo>::iterator iterTmp3Vs3;
				iterTmp3Vs3 = m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();			
				for (;iterTmp3Vs3 != m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iterTmp3Vs3++)  
				{
					CString strAlignerName = m_listCtrlAlignerType.GetItemText(nItem,0);
					if (strAlignerName == GetSysInfoString(m_psaSysInfoStrings,iterTmp3Vs3->m_nAlignerTypeID))
					{
						iterTmp3Vs3->m_bOpenAligner = !m_listCtrlAlignerType.GetCheck(nItem);
						CString strLog,strValue;
						strValue = iterTmp3Vs3->m_bOpenAligner ? _T("选中状态"):_T("未选中状态");
						strLog.Format(_T("修改[%s]为%s\n"),strAlignerName,strValue);
						g_opLogFile.SaveLogText(strLog,3);
						break;
					}
				}
				iterTmp3Vs3 = m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();			
				for (;iterTmp3Vs3 != m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iterTmp3Vs3++) 
				{
					if (iterTmp3Vs3->m_bOpenAligner)
					{
						nAlignerNum++;
					}
				}
				//如果全部不选中
				if (nAlignerNum == 0)     
				{
					vector<Aligner3Vs3TypeInfo>::iterator iterTmp3Vs3Old;
					iterTmp3Vs3Old = alignerTmpOld.m_vAligner3Vs3TypeInfo.begin();				
					iterTmp3Vs3 = m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.begin();
					for (;iterTmp3Vs3Old != alignerTmpOld.m_vAligner3Vs3TypeInfo.end() && iterTmp3Vs3 != m_tmpAlignerTypeInfo.m_vAligner3Vs3TypeInfo.end();iterTmp3Vs3++,iterTmp3Vs3Old++)
					{
						if (iterTmp3Vs3Old->m_bOpenAligner)
						{
							iterTmp3Vs3->m_bOpenAligner = TRUE;
							break;
						}
					}
					AfxMessageBox(_T("对位方式不能全部为空，已选定当前对位方式"));  //_T("定位工具不能为空,已选定当前定位工具！")
				}
			}
			//4vs4
			if ((platformInfo.m_bTargetObjectCamSeparate != TRUE && platformInfo.m_nPositionNum == 4) 
				|| (platformInfo.m_bTargetObjectCamSeparate == TRUE	&& platformInfo.m_nPositionNum == 8 /*&& platformInfo.m_bTarCamWithDiffObjCam == FALSE*/))
			{
				vector<Aligner4Vs4TypeInfo>::iterator iterTmp4Vs4;
				iterTmp4Vs4 = m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();			
				for (;iterTmp4Vs4 != m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iterTmp4Vs4++)  
				{
					CString strAlignerName = m_listCtrlAlignerType.GetItemText(nItem,0);
					if (strAlignerName == GetSysInfoString(m_psaSysInfoStrings,iterTmp4Vs4->m_nAlignerTypeID))
					{
						iterTmp4Vs4->m_bOpenAligner = !m_listCtrlAlignerType.GetCheck(nItem);
						CString strLog,strValue;
						strValue = iterTmp4Vs4->m_bOpenAligner ? _T("选中状态"):_T("未选中状态");
						strLog.Format(_T("修改[%s]为%s\n"),strAlignerName,strValue);
						g_opLogFile.SaveLogText(strLog,3);
						break;
					}
				}
				iterTmp4Vs4 = m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();			
				for (;iterTmp4Vs4 != m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iterTmp4Vs4++) 
				{
					if (iterTmp4Vs4->m_bOpenAligner)
					{
						nAlignerNum++;
					}
				}
				//如果全部不选中
				if (nAlignerNum == 0)     
				{
					vector<Aligner4Vs4TypeInfo>::iterator iterTmp4Vs4Old;
					iterTmp4Vs4Old = alignerTmpOld.m_vAligner4Vs4TypeInfo.begin();				
					iterTmp4Vs4 = m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.begin();
					for (;iterTmp4Vs4Old != alignerTmpOld.m_vAligner4Vs4TypeInfo.end() && iterTmp4Vs4 != m_tmpAlignerTypeInfo.m_vAligner4Vs4TypeInfo.end();iterTmp4Vs4++,iterTmp4Vs4Old++)
					{
						if (iterTmp4Vs4Old->m_bOpenAligner)
						{
							iterTmp4Vs4->m_bOpenAligner = TRUE;
							break;
						}
					}
					AfxMessageBox(_T("对位方式不能全部为空，已选定当前对位方式"));  //_T("定位工具不能为空,已选定当前定位工具！")
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////
		if (nAlignerNum == 0)
		{
			UpdateDlgData(TRUE);
			InitDlgItem();           
		}
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckEnableCommMultiCalibExtension()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableCommMultiCalibExtensionOld = m_bEnableCommMultiCalibExtension;
	UpdateData(TRUE);
	if (bEnableCommMultiCalibExtensionOld != m_bEnableCommMultiCalibExtension)
	{
		CString strVal = m_bEnableCommMultiCalibExtension ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[通信支持单产品扩展]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckVisionStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bVisionOnLineStatusOld = m_bVisionOnLineStatus;
	UpdateData(TRUE);
	if (bVisionOnLineStatusOld != m_bVisionOnLineStatus)
	{
		CString strVal = m_bVisionOnLineStatus ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[视觉通信链接状态（寄存器通信）]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckEnableAlignBenchCenter()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableAlignBenchCenterOld = m_bEnableAlignBenchCenter;
	UpdateData(TRUE);
	if (bEnableAlignBenchCenterOld != m_bEnableAlignBenchCenter)
	{
		CString strVal = m_bEnableAlignBenchCenter ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[对位完成发送居中对位结]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckSetVisionManualSearchToPlc()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bSetVisionManualSearchToPLCOld = m_bSetVisionManualSearchToPLC;
	UpdateData(TRUE);
	if (bSetVisionManualSearchToPLCOld != m_bSetVisionManualSearchToPLC)
	{
		CString strVal = m_bSetVisionManualSearchToPLC ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[定位失败后通知PLC弹出手动定位框(寄存器通信)]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckAlignOneTime()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignOneTimeOld = m_bAlignOneTime;
	UpdateData(TRUE);
	if (bAlignOneTimeOld != m_bAlignOneTime)
	{
		CString strVal = m_bAlignOneTime ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[1次对位]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckEnableMutiTarget()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableMutiTargetOld = m_bEnableMutiTarget;
	UpdateData(TRUE);
	if (bEnableMutiTargetOld != m_bEnableMutiTarget)
	{
		CString strVal = m_bEnableMutiTarget ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[多目标位置对位]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckObjMulSearchPosXypd()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bObjMulSearchPosXYPDOld = m_bObjMulSearchPosXYPD;
	UpdateData(TRUE);
	if (bObjMulSearchPosXYPDOld != m_bObjMulSearchPosXYPD)
	{
		CString strVal = m_bObjMulSearchPosXYPD ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[XY+D平台多位置定位拍照]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckTargetAngleMulAlignerXypd()
{
	// TODO: 在此添加控件通知处理程序代码

	BOOL bTargetAngleMulAlignerXYPDOld =  m_bTargetAngleMulAlignerXYPD;
	UpdateData(TRUE);
	if (bTargetAngleMulAlignerXYPDOld != m_bTargetAngleMulAlignerXYPD)
	{
		CString strVal = m_bTargetAngleMulAlignerXYPD ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[XY+D平台目标角度多次对位]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckLastPickSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bLastPickSettingOld = m_bLastPickSetting;
	UpdateData(TRUE);
	if (bLastPickSettingOld != m_bLastPickSetting)
	{
		CString strVal = m_bLastPickSetting ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[最后一次拾取判定]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckPickPosUpdate()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnablePickPosUpdateOld = m_bEnablePickPosUpdate;
	UpdateData(TRUE);
	if (bEnablePickPosUpdateOld != m_bEnablePickPosUpdate)
	{
		CString strVal = m_bEnablePickPosUpdate ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[拾取旋转中心位置修正]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckAskStagePosEachMove()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAskStagePosEachMoveOld = m_bAskStagePosEachMove;
	UpdateData(TRUE);
	if (bAskStagePosEachMoveOld != m_bAskStagePosEachMove)
	{
		CString strVal = m_bAskStagePosEachMove ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[每次对位时都查询平台当前位置]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckTargetPosSame()
{
	// TODO: 在此添加控件通知处理程序代码m_bTargetPosSame
	BOOL bTargetPosSameOld = m_bTargetPosSame;
	UpdateData(TRUE);
	if (bTargetPosSameOld != m_bTargetPosSame)
	{
		CString strVal = m_bTargetPosSame ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[目标拍照的平台位置一致]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckObjectPosSame()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bObjectPosSameOld = m_bObjectPosSame;
	UpdateData(TRUE);
	if (bObjectPosSameOld != m_bObjectPosSame)
	{
		CString strVal = m_bObjectPosSame ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[对象拍照的平台位置一致]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckSaveAlignPos()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bSaveAlignPosOld = m_bSaveAlignPos;
	UpdateData(TRUE);
	if (bSaveAlignPosOld != m_bSaveAlignPos)
	{
		CString strVal = m_bSaveAlignPos ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[保存对位位置]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckSearchCindidateEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableSearchCindidateOld = m_bEnableSearchCindidate;
	UpdateData(TRUE);
	if (bEnableSearchCindidateOld != m_bEnableSearchCindidate)
	{
		CString strVal = m_bEnableSearchCindidate ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[是否启用候补定位(候补指令可忽略)]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckSearchtime()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bSearchTimesOld = m_bSearchTimes;
	UpdateData(TRUE);
	if (bSearchTimesOld != m_bSearchTimes)
	{
		CString strVal = m_bSearchTimes ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[定位失败后重定位次数]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnSetfocusEditSearchtime()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCHTIME)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCHTIME, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCHTIME, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[定位失败后重新定位次数]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,3);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditSearchtime();
	}
}

void CDlgSysPlatformOptionParam::OnKillfocusEditSearchtime()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		//		m_nSearchTimes = (m_nSearchTimes<0) ? 0 : (m_nSearchTimes);
		m_nSearchTimes  = (m_nSearchTimes > 5) ? 5 : ((m_nSearchTimes < 1) ? 1 : m_nSearchTimes);
		UpdateData(FALSE);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckEnableSearchShutter()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEnableSearchShutterOld = m_bEnableSearchShutter;
	UpdateData(TRUE);
	if (bEnableSearchShutterOld != m_bEnableSearchShutter)
	{
		CString strVal = m_bEnableSearchShutter ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[定位时设置相机曝光时间]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckShowPcb()
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData(TRUE);
// 
// 	CString strVal = m_bShowPCB ? _T("开启状态"):_T("未开启状态");
// 	CString strLog;
// 	strLog.Format(_T("修改[显示PCB参数]为%s\n"),strVal);
// 	g_opLogFile.SaveLogText(strLog,3);
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckShowFwel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strVal = m_bShowFWEL ? _T("开启状态"):_T("未开启状态");
	CString strLog;
	strLog.Format(_T("修改[显示焊接参数]为%s\n"),strVal);
	g_opLogFile.SaveLogText(strLog,3);
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckManualTargetSearchFailed()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bManualAfTarSearchFailOld = m_bManualAfTarSearchFail;
	UpdateData(TRUE);
	if (bManualAfTarSearchFailOld != m_bManualAfTarSearchFail)
	{
		CString strVal = m_bManualAfTarSearchFail ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[目标定位失败后手动选择Mark]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckManualObjectSearchFailed()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bManualAfObjSearchFailOld = m_bManualAfObjSearchFail;
	UpdateData(TRUE);
	if (bManualAfObjSearchFailOld != m_bManualAfObjSearchFail)
	{
		CString strVal = m_bManualAfObjSearchFail ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[对象定位失败后手动选择Mark]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckManualTargetSearchFailedShowButton()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bManualAfTarSearchFailShowButtonOld = m_bManualAfTarSearchFailShowButton;
	UpdateData(TRUE);
	if (bManualAfTarSearchFailShowButtonOld != m_bManualAfTarSearchFailShowButton)
	{
		CString strVal = m_bManualAfTarSearchFailShowButton ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[显示目标定位失败后手动选择Mark按钮]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckManualObjectSearchFailedShowButton()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bManualAfObjSearchFailShowButtonOld = m_bManualAfObjSearchFailShowButton;
	UpdateData(TRUE);
	if (bManualAfObjSearchFailShowButtonOld != m_bManualAfObjSearchFailShowButton)
	{
		CString strVal = m_bManualAfObjSearchFailShowButton ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[显示对象定位失败后手动选择Mark按钮]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedBtnCalibDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString strLog;
	//strLog.Format(_T("点击[对位偏差方向设置]进入[对位偏差方向设置界面]\n"));
	//g_opLogFile.SaveLogText(strLog,4);

	CDlgCalibAxisDirection dlg;
	dlg.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlg.m_bReverseX = m_bCalibrateChangeXDirection;
	dlg.m_bReverseY = m_bCalibrateChangeYDirection;
	dlg.m_bReverseD = m_bCalibrateChangeDDirection;
	if (dlg.DoModal() == IDOK)
	{
		m_bCalibrateChangeXDirection = dlg.m_bReverseX;
		m_bCalibrateChangeYDirection = dlg.m_bReverseY;
		m_bCalibrateChangeDDirection = dlg.m_bReverseD;
	}
}

void CDlgSysPlatformOptionParam::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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
	//GetDlgItem(nID)->EnableWindow(bEnable);
	//GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) || CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedCheckCalibSendRelativePos()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	// 发送相对上一位置的相对偏差量选项时，必须启用发送绝对轴位置，否则存在bug；
	//m_bCalibSendAbsPos = TRUE;
	UpdateData(FALSE);
}

void CDlgSysPlatformOptionParam::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bEnableAllControl = !m_bEnableAllControl;

	//SetEnableAllControl(m_bEnableAllControl && (m_pVisionASM->GetUsePassWordMode() == 0));

	CDialogEx::OnRButtonDblClk(nFlags, point);
}

void CDlgSysPlatformOptionParam::SetEnableAllControl(BOOL bEnable)
{
	SetGroupBoxStatus(IDC_STATIC_SEARCH_TOOL_TYPE,bEnable,TRUE);
	SetGroupBoxStatus(IDC_STATIC_OFFSET_PICTURE,bEnable,TRUE);
	SetGroupBoxStatus(IDC_STATIC_INSPECT_SETTING,bEnable,TRUE);
	SetGroupBoxStatus(IDC_OPTION_CONFIG,bEnable,TRUE);
	SetGroupBoxStatus(IDC_STATIC_CALIBRATE_SETTING,bEnable,TRUE);

	if (bEnable && m_pVisionASM)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		BOOL bEnableTargetAutoCalib = platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar;
		GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
		GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);

		// 拾取相关设置的启用/禁用（因为缺少布局，目前采用禁用的处理）
		SetGroupBoxStatus(IDC_STATIC_PICK_SETTING,platformInfo.m_bPlatformPick,TRUE);

		//// XY+D相关的启用/禁用
		//SetGroupBoxStatus(IDC_STATIC_XYPD_SETTING,platformInfo.m_eMidPlatformType== ePlatformXDPY,platformInfo.m_eMidPlatformType== ePlatformXDPY);


		BOOL IsExProtocolType = FALSE;
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL != vsConfig)
		{
			IsExProtocolType = (vsConfig->m_CommParamInfo.m_eExProtocolType==eExProReserved1);
		}

		// 产品扩展相关
		SetGroupBoxStatus(IDC_STC_PRODUCT_COMM_EX,platformInfo.m_bEnableMultiCalibExtension && IsExProtocolType,TRUE);
	}
}

void CDlgSysPlatformOptionParam::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		bool m_bShowInspect = true;
		bool m_bEnableInspect = true;

		bool m_bShowAlignType = true;
		bool m_bEnableAlignType = true;

		bool m_bShowPick = true;
		bool m_bEnablePick = true;

		bool m_bShowSnap = true;
		bool m_bEnableSnap = true;

		bool m_bShowAlignOption = true;
		bool m_bEnableAlignOption = true;

		bool m_bShowPlatformComm = true;
		bool m_bEnablePlatformComm = true;

		
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			bool bShow = false;
			bool bEnable = false;
			CString strNodeName = _T("");
			// 定位工具
			bool bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_SearchTool,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_SEARCH_TOOL_TYPE,bEnable,bShow);
			}

			// 补偿设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_OffSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowOffsetCtl,m_bEnableOffsetCtl);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_OFFSET_PICTURE,m_bEnableOffsetCtl,m_bShowOffsetCtl);
			}

			// 检测设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_Inspect,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowInspect,m_bEnableInspect);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_INSPECT_SETTING,m_bEnableInspect,m_bShowInspect);
			}

			// 对位方式设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_AlignType,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignType,m_bEnableAlignType);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_ALIGN_TYPESETTING,m_bEnableAlignType,m_bShowAlignType);
			}
			// 对位参数设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_AlignOption,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowAlignOption,m_bEnableAlignOption);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_ALIGN_PARAM_SETTING,m_bEnableAlignOption,m_bShowAlignOption);
				// XY+D相关的启用/禁用
				//SetGroupBoxStatus(IDC_STATIC_XYPD_SETTING,(platformInfo.m_eMidPlatformType== ePlatformXDPY) && bEnable,(platformInfo.m_eMidPlatformType== ePlatformXDPY) && bShow);

			}
			// 平台通信设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_PlatformComm,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowPlatformComm,m_bEnablePlatformComm);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_COMM_SETTING,m_bEnablePlatformComm,m_bShowPlatformComm);
				BOOL IsExProtocolType = (vsConfig->m_CommParamInfo.m_eExProtocolType==eExProReserved1);
				// 产品扩展相关
				SetGroupBoxStatus(IDC_STC_PRODUCT_COMM_EX,platformInfo.m_bEnableMultiCalibExtension && IsExProtocolType && m_bEnablePlatformComm,m_bShowPlatformComm);
			}
			// 拍照设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_SnapSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowSnap,m_bEnableSnap);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_SNAP_SETTING,m_bEnableSnap,m_bShowSnap);
			}
			// 拾取设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_PickSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowPick,m_bEnablePick);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_PICK_SETTING,m_bEnablePick && platformInfo.m_bPlatformPick,m_bShowPick && platformInfo.m_bPlatformPick);
			}

			

			// 标定关联设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_CalibrationSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowCalibrateCtl,m_bEnableCalibrateCtl);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_CALIBRATE_SETTING,m_bEnableCalibrateCtl,m_bShowCalibrateCtl);

				BOOL bEnableTargetAutoCalib = platformInfo.m_bTargetObjectCamSeparate&&platformInfo.m_bTarObjCamSepAutoCalibTar;
				GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
				GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->EnableWindow(bEnableTargetAutoCalib && m_bEnableCalibrateCtl);
				GetDlgItem(IDC_STATIC_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(m_bShowCalibrateCtl);
				GetDlgItem(IDC_CMB_TARAUTOCALIB_ALIGN_TYPE)->ShowWindow(m_bShowCalibrateCtl);

				int nShowCalibCheckByAxis = SW_HIDE;
				//BOOL bEnableCalibCheckByAxis = (((platformInfo.m_bPlatformPick && platformInfo.m_ePickCamPlatformType == ePickCamPlatformSepFix)  
				//	|| (platformInfo.m_eMidPlatformType == ePlatformXYD) || 
				//	(platformInfo.m_eMidPlatformType == ePlatformX1X2Y)|| 
				//	(platformInfo.m_eMidPlatformType == ePlatformXY1Y2) || 
				//	(platformInfo.m_eMidPlatformType == ePlatformXY) || 
				//	(platformInfo.m_eMidPlatformType == ePlatformXD)|| 
				//	(platformInfo.m_eMidPlatformType == ePlatformYD)));
				//if (bEnableCalibCheckByAxis && m_bShowCalibrateCtl)
				//{
				//	nShowCalibCheckByAxis = SW_SHOW;
				//}

				//for (int i=0;i<8;i++)
				//{
				//	GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->ShowWindow(nShowCalibCheckByAxis);
				//	GetDlgItem(IDC_CHECK_CALIB_CHECK_BY_AXISMOVE+i)->EnableWindow(bEnableCalibCheckByAxis && m_bEnableCalibrateCtl);
				//}

			}

			


			// 更多设置
			bFind = vsConfig->GetCheckedByItem(ePlatformOptionAuthorize_More,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
					GetDlgItem(IDC_BUTTON_OFFSET_OTHER_SET)->EnableWindow(bEnable && m_bEnableOffsetCtl);
					GetDlgItem(IDC_BUTTON_OFFSET_OTHER_SET)->ShowWindow(bShow && m_bShowOffsetCtl);
					GetDlgItem(IDC_BUTTON_INSPECT_SET2)->EnableWindow(bEnable && m_bEnableInspect);
					GetDlgItem(IDC_BUTTON_INSPECT_SET2)->ShowWindow(bShow && m_bShowInspect);
					GetDlgItem(IDC_BUTTON_SNAP_SET)->EnableWindow(bEnable && m_bEnableSnap);
					GetDlgItem(IDC_BUTTON_SNAP_SET)->ShowWindow(bShow && m_bShowSnap);
					GetDlgItem(IDC_BUTTON_ALIGNER_OPTION_SET2)->EnableWindow(bEnable && m_bEnableAlignOption);
					GetDlgItem(IDC_BUTTON_ALIGNER_OPTION_SET2)->ShowWindow(bShow && m_bShowAlignOption);
					GetDlgItem(IDC_BUTTON_PLATFORM_COMM_SET3)->EnableWindow(bEnable && m_bEnablePlatformComm);
					GetDlgItem(IDC_BUTTON_PLATFORM_COMM_SET3)->ShowWindow(bShow && m_bShowPlatformComm);
					GetDlgItem(IDC_BUTTON_CALIBTATE_SET4)->EnableWindow(bEnable && m_bEnableCalibrateCtl);
					GetDlgItem(IDC_BUTTON_CALIBTATE_SET4)->ShowWindow(bShow && m_bShowCalibrateCtl);
			}

			GetDlgItem(IDC_OPTION_CONFIG)->EnableWindow(m_bEnableAlignType || m_bEnableAlignOption || m_bEnablePlatformComm || m_bEnableSnap || m_bEnablePick || m_bEnableCalibrateCtl);
			GetDlgItem(IDC_OPTION_CONFIG)->ShowWindow(m_bShowAlignType || m_bShowAlignOption || m_bShowPlatformComm || m_bShowSnap || m_bShowPick || m_bShowCalibrateCtl);

		}
	}
}

void CDlgSysPlatformOptionParam::OnBnClickedButtonOffsetOtherSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CDynDialogEx dlg(this);

	dlg.SetWindowTitle(_T("设置补偿备注"));

	int nNums = 2;
	CRect rectCtrl[2];
	BOOL bCheck[2];
	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 160;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;
		
		
	}
	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SHOW_OFFSET_TIPS);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SET_OFFSET_TIP);
	bCheck[0] = m_bShowOffsetTips;
	bCheck[1] = m_bSetOffsetTip;

	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[2];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}

	for (int it=0;it<nNums;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}
	

	if (dlg.DoModal() == IDOK)
	{
		m_bShowOffsetTips = bCheck[0];
		m_bSetOffsetTip = bCheck[1];
	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}
	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
	
}


void CDlgSysPlatformOptionParam::OnBnClickedButtonInspectSet2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDynDialogEx dlg(this);

	dlg.SetWindowTitle(GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_INSPECT_SETTING));
	
	int nNums = 3;
	CRect rectCtrl[3];
	BOOL bCheck[3];
	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 160;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;

	}
	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDS_CHK_SEARCH_WITH_DISTANCE_INSPECT);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDS_CHK_SEND_INSPECT_DISTANCE);
	vStrCheck[2] = GetSysInfoString(m_psaSysInfoStrings,IDS_CHK_SEND_AREA_INSPECT_SUB_RESULT);
	bCheck[0] = m_bSearchWithDistanceInspect;
	bCheck[1] = m_bSendInsDistanceInSearchResult;
	bCheck[2] = m_bSendAreaInspectSubResult;

	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[3];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}

	for (int it=0;it<nNums;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}

	if (dlg.DoModal() == IDOK)
	{
		m_bSearchWithDistanceInspect = bCheck[0] ;
		m_bSendInsDistanceInSearchResult = bCheck[1];
		m_bSendAreaInspectSubResult = bCheck[2];
	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}
	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnBnClickedButtonSnapSet()
{
	// TODO: 在此添加控件通知处理程序代码

	CDynDialogEx dlg(this);

	dlg.SetWindowTitle(GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_SNAP_SETTING));
	CString strEditBuf(_T(""));
	int nNums = 10;
	CRect rectCtrl[10];
	BOOL bCheck[10];
	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 120;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;

	}
	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHK_RECORD_CAMERA_FIX_MARK);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SAVE_ALIGN_POS);
	vStrCheck[2] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SEARCH_CINDIDATE_ENABLE);
	vStrCheck[3] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SEARCHTIME);

	vStrCheck[4] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ENABLE_SEARCH_SHUTTER);
	vStrCheck[5] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED);
	vStrCheck[6] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED);

	vStrCheck[7] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_MANUAL_TARGET_SEARCH_FAILED_SHOW_BUTTON);
	vStrCheck[8] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_MANUAL_OBJECT_SEARCH_FAILED_SHOW_BUTTON);

	vStrCheck[9] = _T("允许拍照时切换光源");

	bCheck[0] = m_bRecordCamFixMarkPos;
	bCheck[1] = m_bSaveAlignPos;
	bCheck[2] = m_bEnableSearchCindidate;
	bCheck[3] = m_bSearchTimes;
	bCheck[4] = m_bEnableSearchShutter;
	bCheck[5] = m_bManualAfTarSearchFail;
	bCheck[6] = m_bManualAfObjSearchFail;
	bCheck[7] = m_bManualAfTarSearchFailShowButton;
	bCheck[8] = m_bManualAfObjSearchFailShowButton;
	bCheck[9] = m_bEnablePhotoLightCtrl;
	strEditBuf.Format(_T("%d"),m_nSearchTimes);

	CRect rectEdit;
	rectEdit.left = rectCtrl[3].right+20;
	rectEdit.right = rectEdit.left + 24;
	rectEdit.top = rectCtrl[3].top;
	rectEdit.bottom = rectCtrl[3].bottom;

	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[10];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}
	CString strTMPEditBuf(strEditBuf);

	for (int it=0;it<nNums;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}
	dlg.AddDlgControl(_T("EDIT"), _T(""), STYLE_EDIT, EXSTYLE_EDIT, &rectEdit, (void*)&strEditBuf);

	if (dlg.DoModal() == IDOK)
	{
		m_bRecordCamFixMarkPos				= bCheck[0];
		m_bSaveAlignPos						= bCheck[1];
		m_bEnableSearchCindidate			= bCheck[2];
		m_bSearchTimes						= bCheck[3];
		m_bEnableSearchShutter				= bCheck[4];
		m_bManualAfTarSearchFail			= bCheck[5];
		m_bManualAfObjSearchFail			= bCheck[6];
		m_bManualAfTarSearchFailShowButton	= bCheck[7];
		m_bManualAfObjSearchFailShowButton	= bCheck[8];
		m_bEnablePhotoLightCtrl             = bCheck[9];
		m_nSearchTimes = atoi(strEditBuf);

	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}

	if (strTMPEditBuf != strEditBuf)
	{
		strLog.AppendFormat(_T("修改[%s]:由%s改为%s\t"),vStrCheck[3],strTMPEditBuf,strEditBuf);
	}

	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnBnClickedButtonAlignerOptionSet2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDynDialogEx dlg(this);
	
	dlg.SetWindowTitle(GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_ALIGN_PARAM_SETTING));
	CString strEditBuf(_T(""));
	int nNums = 13;
	CRect rectCtrl[13];
	BOOL bCheck[13];
	int nRadio1 = m_nTarInfoCommModeInDiffPC;

	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 150;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;
	}

	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHK_SEARCH_AND_ALIGN);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHK_SEND_ALIGN_MUL_RESULT);
	vStrCheck[2] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ALIGN_ONE_TIME);
	vStrCheck[3] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ENABLE_MUTI_TARGET);

	vStrCheck[4] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_TARGT_USE_VIRTUAL_COORD);
	vStrCheck[5] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_MUL_MODE_ENABLE);
	vStrCheck[6] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ENABLE_TARGET_OBJECT_VIRTUAL_MODE);

	vStrCheck[7] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_OBJ_MUL_SEARCH_POS_XYPD);
	vStrCheck[8] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_TARGET_ANGLE_MUL_ALIGNER_XYPD);

	vStrCheck[9] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SHOW_FWEL);
	//vStrCheck[10] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SHOW_PCB);
	vStrCheck[10] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_EIGHT_SHAPE_ENABLED);
	vStrCheck[11] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_BEND_SHOW);
	vStrCheck[12] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ALIGN_OFFSET_MARK_BY_AERA);
	

	bCheck[0] = m_bEnableAlignAfterSearch;
	bCheck[1] = m_bChkSendAlignMulResults;
	bCheck[2] = m_bAlignOneTime;
	bCheck[3] = m_bEnableMutiTarget;
	bCheck[4] = m_bTargetUseFixedVirtualCoordinates;
	bCheck[5] = m_bEnableMultiMarkMode;
	bCheck[6] = m_bEnableTargetAndObjectPositionVirtualMode;
	bCheck[7] = m_bObjMulSearchPosXYPD;
	bCheck[8] = m_bTargetAngleMulAlignerXYPD;

	bCheck[9] = m_bShowFWEL;
//	bCheck[10] = m_bShowPCB;
	bCheck[10] = m_bShowEightShapeAlign;
	bCheck[11] = m_bShowBend;
	bCheck[12] = m_bEnableAlignOffsetMarkByAera;


	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[13];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}

	int nTMRadio1  = nRadio1;

	for (int it=0;it<nNums;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}
	
	CRect rectRadio;
	rectRadio.top = rectCtrl[nNums-1].bottom;
	rectRadio.bottom = rectRadio.top + 4.5*lRowStepSize ;
	rectRadio.left = rectCtrl[nNums-1].left -5;
	rectRadio.right = rectCtrl[nNums-1].right+5;

	dlg.AddDlgControl(_T("BUTTON"), GetSysInfoString(m_psaSysInfoStrings,IDC_GRP_DIFFERENT_PC), STYLE_GROUPBOX, EXSTYLE_GROUPBOX,&rectRadio);
	CRect rectRadio2(rectCtrl[8]);
	
	rectRadio2.top =rectRadio.top+lRowStepSize;
	rectRadio2.bottom =rectRadio2.top +lRowStepSize;

	dlg.AddDlgControl(_T("BUTTON"), GetSysInfoString(m_psaSysInfoStrings,IDC_RDO_TARGET_INFO_COMM_MODE), STYLE_RADIO_GROUP, EXSTYLE_RADIO, &rectRadio2, (void*)&nRadio1);
	rectRadio2.top +=lRowStepSize;
	rectRadio2.bottom +=lRowStepSize;
	dlg.AddDlgControl(_T("BUTTON"), GetSysInfoString(m_psaSysInfoStrings,IDC_RDO_COMM_TAR_PLATFORMPOS_WITH_PLC), STYLE_RADIO, EXSTYLE_RADIO,&rectRadio2);
	rectRadio2.top +=lRowStepSize;
	rectRadio2.bottom +=lRowStepSize;
	dlg.AddDlgControl(_T("BUTTON"), GetSysInfoString(m_psaSysInfoStrings,IDC_RDO_COMM_TAR_IMGPOS_WITH_PLC), STYLE_RADIO, EXSTYLE_RADIO,&rectRadio2);

	
	if (dlg.DoModal() == IDOK)
	{
		m_bEnableAlignAfterSearch						= bCheck[0] ;
		m_bChkSendAlignMulResults						= bCheck[1] ;
		m_bAlignOneTime									= bCheck[2] ;
		m_bEnableMutiTarget								= bCheck[3] ;
		m_bTargetUseFixedVirtualCoordinates				= bCheck[4] ;
		m_bEnableMultiMarkMode							= bCheck[5] ;
		m_bEnableTargetAndObjectPositionVirtualMode		= bCheck[6] ;
		m_bObjMulSearchPosXYPD							= bCheck[7] ;
		m_bTargetAngleMulAlignerXYPD					= bCheck[8] ;
		m_bShowFWEL										= bCheck[9] ;
		m_bShowEightShapeAlign							= bCheck[10] ;
		m_bShowBend										= bCheck[11] ;
		m_bEnableAlignOffsetMarkByAera					= bCheck[12];
		m_nTarInfoCommModeInDiffPC						= nRadio1;

	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}

	if (nTMRadio1 != nRadio1)
	{
		strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),GetSysInfoString(m_psaSysInfoStrings,IDC_GRP_DIFFERENT_PC),nTMRadio1,nRadio1);
	}

	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnBnClickedButtonPlatformCommSet3()
{
	// TODO: 在此添加控件通知处理程序代码
	CDynDialogEx dlg(this);
	
	dlg.SetWindowTitle(GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_COMM_SETTING));

	CString strEditBuf(_T(""));
	int nNums = 5;
	CRect rectCtrl[5];
	BOOL bCheck[5];
	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 160;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;
	}
	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_READ_WRITE_AXIS_P0S_WITN_CMD);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_VISION_STATUS);
	vStrCheck[2] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_ENABLE_ALIGN_BENCH_CENTER);
	vStrCheck[3] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_SET_VISION_MANUAL_SEARCH_TO_PLC);
	vStrCheck[4] = _T("自定义弹窗回复地址");

	bCheck[0] = m_bReadWriteAxisPosWithCmd;
	bCheck[1] = m_bVisionOnLineStatus;
	bCheck[2] = m_bEnableAlignBenchCenter;
	bCheck[3] = m_bSetVisionManualSearchToPLC;
	bCheck[4] = m_bSetVisionManualSearchToPLC2;
	strEditBuf.Format(_T("%d"),m_nVisionManualSearchToPlcAddre);

	CRect rectEdit;
	rectEdit.left = rectCtrl[4].right+20;
	rectEdit.right = rectEdit.left + 24;
	rectEdit.top = rectCtrl[4].top;
	rectEdit.bottom = rectCtrl[4].bottom;

	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[5];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}
	CString strTMPEditBuf(strEditBuf);

	for (int it=0;it<nNums;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}
	dlg.AddDlgControl(_T("EDIT"), _T(""), STYLE_EDIT, EXSTYLE_EDIT, &rectEdit, (void*)&strEditBuf);

	if (dlg.DoModal() == IDOK)
	{
		m_bReadWriteAxisPosWithCmd		= bCheck[0];
		m_bVisionOnLineStatus			= bCheck[1];
		m_bEnableAlignBenchCenter		= bCheck[2];
		m_bSetVisionManualSearchToPLC	= bCheck[3];
		m_bSetVisionManualSearchToPLC2	= bCheck[4];
		m_nVisionManualSearchToPlcAddre = atoi(strEditBuf);
	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}

	if (strTMPEditBuf != strEditBuf)
	{
		strLog.AppendFormat(_T("修改[%s]:由%s改为%s\t"),vStrCheck[4],strTMPEditBuf,strEditBuf);
	}

	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnBnClickedButtonCalibtateSet4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDynDialogEx dlg(this);

	dlg.SetWindowTitle(GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_CALIBRATE_SETTING));

	int nNums = 4;
	CRect rectCtrl[4];
	BOOL bCheck[4];
	std::vector<CString> vStrCheck;
	vStrCheck.resize(nNums);
	long lRowStepSize = (long)(ROWSTEPSIZE * 1.1);
	for (int it=0;it<nNums;it++)
	{
		rectCtrl[it].left = 8;
		rectCtrl[it].right = 160;
		rectCtrl[it].top = 5+it*lRowStepSize;
		rectCtrl[it].bottom = 5+(it+1)*lRowStepSize;

	}
	vStrCheck[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_CALIBRATE_CHECK_BY_SIZE);
	vStrCheck[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_FIX_TARGET_CHECK_CALIB);
	vStrCheck[2] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_RELVEVANCE_CHECK_SIZE);
	vStrCheck[3] = GetSysInfoString(m_psaSysInfoStrings,IDC_CHECK_CALIB_CHECK_BY_AXISMOVE);
	
	bCheck[0] = m_bEnableCalibrateCheckBySize;
	bCheck[1] = m_bEnableCalibrateCheckByFixedTargetAligner;
	bCheck[2] = m_bEnableRelevanceCheckBySize;
	bCheck[3] = m_bEnableCalibrateCheckByAxisMoveOption;
	
	// 日志记录
	CString strLog = _T("");
	BOOL bTMPCheck[4];
	for (int it=0;it<nNums;it++)
	{
		bTMPCheck[it] = bCheck[it];
	}
	
	SysPlatformInfo platformInfo;
	if (NULL !=m_pVisionASM)
	{
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
	}

	BOOL bEnableCalibCheckByAxis = (((platformInfo.m_bPlatformPick && platformInfo.m_ePickCamPlatformType == ePickCamPlatformSepFix)  
		|| (platformInfo.m_eMidPlatformType == ePlatformXYD) || 
		(platformInfo.m_eMidPlatformType == ePlatformX1X2Y)|| 
		(platformInfo.m_eMidPlatformType == ePlatformXY1Y2) || 
		(platformInfo.m_eMidPlatformType == ePlatformXY) || 
		(platformInfo.m_eMidPlatformType == ePlatformXD)|| 
		(platformInfo.m_eMidPlatformType == ePlatformYD)));

	DWORD dwStyle =(STYLE_CHECKBOX | WS_GROUP);
	DWORD dwStyle2 =STYLE_STATIC;
	DWORD dwStyle3 =STYLE_EDIT;
	if (!bEnableCalibCheckByAxis)
	{
		dwStyle = (STYLE_CHECKBOX | WS_GROUP | WS_DISABLED);
		dwStyle2 =(STYLE_STATIC | WS_DISABLED);
		dwStyle3 =(STYLE_EDIT | WS_DISABLED);
	}

	for (int it=0;it<nNums-1;it++)
	{
		dlg.AddDlgControl(_T("BUTTON"),vStrCheck[it], (STYLE_CHECKBOX | WS_GROUP), EXSTYLE_CHECKBOX,&rectCtrl[it],(void*)&bCheck[it]);
	}
	// 随轴移动
	dlg.AddDlgControl(_T("BUTTON"),vStrCheck[nNums-1], dwStyle, EXSTYLE_CHECKBOX,&rectCtrl[nNums-1],(void*)&bCheck[nNums-1]);

	CString strXYDStatic[3];
	strXYDStatic[0] = GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_AXIS_MOVE_X);
	strXYDStatic[1] = GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_AXIS_MOVE_Y);
	strXYDStatic[2] = GetSysInfoString(m_psaSysInfoStrings,IDC_STATIC_AXIS_MOVE_D);
	CString strXYDCEdit[3];
	strXYDCEdit[0].Format(_T("%4f"),m_dCalibrateCheckAxisMoveRangeX);
	strXYDCEdit[1].Format(_T("%4f"),m_dCalibrateCheckAxisMoveRangeY);
	strXYDCEdit[2].Format(_T("%4f"),m_dCalibrateCheckAxisMoveRangeD);

	CRect rectCtrlStatic(rectCtrl[nNums-1]);
	rectCtrlStatic.top = rectCtrl[nNums-1].bottom;
	rectCtrlStatic.right = rectCtrlStatic.left +50;
	rectCtrlStatic.bottom = rectCtrlStatic.top + lRowStepSize;
	CRect rectCtrlCEdit(rectCtrlStatic);
	rectCtrlCEdit.top -=2;
	rectCtrlCEdit.bottom =rectCtrlCEdit.top + ROWSTEPSIZE;
	rectCtrlCEdit.left = rectCtrlStatic.right +5;
	rectCtrlCEdit.right = rectCtrlCEdit.left +40;

	//dlg.AddDlgControl(_T("STATIC"),strXYDStatic[0], STYLE_STATIC, EXSTYLE_STATIC,&rectCtrlStatic);
	dlg.AddDlgControl(_T("STATIC"),strXYDStatic[0], dwStyle2, EXSTYLE_STATIC,&rectCtrlStatic);
	dlg.AddDlgControl(_T("EDIT"),_T(""),dwStyle3,EXSTYLE_EDIT,&rectCtrlCEdit,(void*)&strXYDCEdit[0]);

	rectCtrlStatic.top +=lRowStepSize;
	rectCtrlStatic.bottom +=lRowStepSize;
	rectCtrlCEdit.top +=lRowStepSize;
	rectCtrlCEdit.bottom +=lRowStepSize;
	dlg.AddDlgControl(_T("STATIC"),strXYDStatic[1], dwStyle2, EXSTYLE_STATIC,&rectCtrlStatic);
	dlg.AddDlgControl(_T("EDIT"),_T(""),dwStyle3,EXSTYLE_EDIT,&rectCtrlCEdit,(void*)&strXYDCEdit[1]);

	rectCtrlStatic.top +=lRowStepSize;
	rectCtrlStatic.bottom +=lRowStepSize;
	rectCtrlCEdit.top +=lRowStepSize;
	rectCtrlCEdit.bottom +=lRowStepSize;
	dlg.AddDlgControl(_T("STATIC"),strXYDStatic[2], dwStyle2, EXSTYLE_STATIC,&rectCtrlStatic);
	dlg.AddDlgControl(_T("EDIT"),_T(""),dwStyle3,EXSTYLE_EDIT,&rectCtrlCEdit,(void*)&strXYDCEdit[2]);

	if (dlg.DoModal() == IDOK)
	{
		m_bEnableCalibrateCheckBySize					= bCheck[0] ;
		m_bEnableCalibrateCheckByFixedTargetAligner		= bCheck[1];
		m_bEnableRelevanceCheckBySize					= bCheck[2] ;
		m_bEnableCalibrateCheckByAxisMoveOption			= bCheck[3];

		m_dCalibrateCheckAxisMoveRangeX =				atof(strXYDCEdit[0]);
		m_dCalibrateCheckAxisMoveRangeY =				atof(strXYDCEdit[1]);
		m_dCalibrateCheckAxisMoveRangeD =				atof(strXYDCEdit[2]);

	}

	for (int it=0;it<nNums;it++)
	{
		if(bTMPCheck[it] != bCheck[it])
		{
			strLog.AppendFormat(_T("修改[%s]:由%d改为%d\t"),vStrCheck[it],bTMPCheck[it],bCheck[it]);
		}
	}
	if (strLog != _T("")) // 发生改变才进行日志记录
	{
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnBnClickedChkFaalAlignOneTimeTargetSearchSnap()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL m_bFAALAlignOneTimeTargetSeachSnapOld = m_bFAALAlignOneTimeTargetSeachSnap;
	UpdateData(TRUE);
	if (m_bFAALAlignOneTimeTargetSeachSnapOld != m_bFAALAlignOneTimeTargetSeachSnap)
	{
		CString strVal = m_bFAALAlignOneTimeTargetSeachSnap ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[FAAL一次对位目标也拍照（目标注册）]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysPlatformOptionParam::OnEnSetSouSuoTime()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT1, strOutput);
		}

	}
}


void CDlgSysPlatformOptionParam::OnBnClickedCheckMoreTimeTargetSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL m_bFAALAlignMoreTimeTargetSeachOld = m_bFAALAlignMoreTimeTargetSeach;
	UpdateData(TRUE);
	if (m_bFAALAlignMoreTimeTargetSeachOld != m_bFAALAlignMoreTimeTargetSeach)
	{
		CString strVal = m_bFAALAlignMoreTimeTargetSeach ? _T("开启状态"):_T("未开启状态");
		CString strLog;
		strLog.Format(_T("修改[FAAL多次拍目标目标]为%s\n"),strVal);
		g_opLogFile.SaveLogText(strLog,3);
	}
}
