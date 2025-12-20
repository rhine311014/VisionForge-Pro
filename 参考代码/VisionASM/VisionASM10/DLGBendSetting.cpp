// DLGBendSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DLGBendSetting.h"
#include "afxdialogex.h"
#include "LogFileSave.h"


// CDlgBendSetting 对话框

IMPLEMENT_DYNAMIC(CDlgBendSetting, CDialog)

CDlgBendSetting::CDlgBendSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBendSetting::IDD, pParent)
	, m_bEnableMoveSnap(FALSE)
	, m_nPosIndexFisrt(0)
	, m_bEnableAxisMoveLimit(FALSE)
	, m_dAxisAllowMoveMaxX(0)
	, m_dAxisAllowMoveMaxY(0)
	, m_dAxisAllowMoveMaxD(0)
	, m_bEnableFbsLRY(FALSE)
	, m_bBendPressParaWrite(FALSE)
	, m_lBendPressParaWriteAddre(10000)
	, m_bEnableReadWsInfo(FALSE)
	, m_bEnableReadInspectWsInfo(FALSE)
	, m_bEnableReadStartTimeInfo(FALSE)
	, m_bEnableReadEndTimeInfo(FALSE)
	, m_bEnableReadProsessingTimeInfo(FALSE)
	, m_strLeftDisOffset(_T("0.000000"))
	, m_strRightDisOffset(_T("0.000000"))
	, m_bEnableUseMathCaluLRDis(FALSE)
	, m_strPlatformName1(_T("平台号"))
	, m_strPlatformName2(_T("平台号2"))
	, m_bEnableFbisTarObc(FALSE)
{
	m_bKillFocusEnabled = TRUE;
	m_bSysKeyboardEnabled = TRUE;
	m_psaSysInfoStrings = NULL;

	m_bEnableMultiCalibExtension = FALSE;
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

	m_bBendThreOffset = FALSE;
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
	m_bBendInspectSnCodeInvert=FALSE;
	m_bBendInspectSnCodeLength=10;
	m_bBendInspectGetSnOnce=TRUE;

	m_nPlatformIndex = 0;
}

CDlgBendSetting::~CDlgBendSetting()
{
}

void CDlgBendSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_BEND_INSPECT, m_bBendAlignInspect);

	DDX_Radio(pDX, IDC_RADIO_BEND_PLATFORM_INSPECT, m_bBendType);
	DDX_Text(pDX, IDC_EDIT_BEND_PIXEL_SIZE, m_strPixelSize);

	DDX_Check(pDX, IDC_CHECK_BEND_DISTANCE_OFFSET, m_bBendDisOffset);
	DDX_Text(pDX, IDC_EDIT_TARGET_DISTANCE_OFFSET, m_strTargetDisOffset);
	DDX_Text(pDX, IDC_EDIT_OBJECT_DISTANCE_OFFSET, m_strObjectDisOffset);

	DDX_Check(pDX, IDC_CHECK_BEND_RESULT_OFFSET, m_bBendResultOffset);
	DDX_Text(pDX, IDC_EDIT_LEFT_OFFSET_X, m_strLeftOffsetX);
	DDX_Text(pDX, IDC_EDIT_LEFT_OFFSET_Y, m_strLeftOffsetY);
	DDX_Text(pDX, IDC_EDIT_RIGHT_OFFSET_X, m_strRightOffsetX);
	DDX_Text(pDX, IDC_EDIT_RIGHT_OFFSET_Y, m_strRightOffsetY);

	DDX_Check(pDX, IDC_CHECK_BEND_THRE_OFFSET, m_bBendThreOffset);
	DDX_Text(pDX, IDC_EDIT_THRE_OFFSET_X, m_strThreOffsetX);
	DDX_Text(pDX, IDC_EDIT_THRE_OFFSET_Y, m_strThreOffsetY);

	DDX_Check(pDX, IDC_CHECK_BEND_PRESS_PARAMETER, m_bBendPressPara);
	DDX_Text(pDX, IDC_EDIT_PRESS_OK_MIN_Y, m_strPressOKMinY);
	DDX_Text(pDX, IDC_EDIT_PRESS_OK_MAX_Y, m_strPressOKMaxY);
	DDX_Text(pDX, IDC_EDIT_PRESS_NG_MIN_Y, m_strPressNGMinY);
	DDX_Text(pDX, IDC_EDIT_PRESS_NG_MAX_Y, m_strPressNGMaxY);
	DDX_Check(pDX, IDC_CHECK_BEND_LOAD_THRE, m_bBendLoadThre);
	DDX_Text(pDX, IDC_EDIT_BEND_LOAD_X, m_strBendLoadThreX);
	DDX_Text(pDX, IDC_EDIT_BEND_LOAD_Y, m_strBendLoadThreY);
	DDX_Check(pDX, IDC_CHECK_BENDING_PRODUCT_ID, m_bBendingProductID);
	DDX_Check(pDX, IDC_CHECK_BENDING_INS, m_bBendingInspect);
	DDX_Check(pDX, IDC_CHECK_BEND_ALIGN_DMCODE, m_bBendAlignDmCode);
	DDX_Check(pDX, IDC_CHECK_BEND_INSPECT_DMCODE, m_bBendInspectDmCode);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MOVE_SNAP, m_bEnableMoveSnap);
	DDX_Radio(pDX, IDC_RADIO_POSTION1_FRIST, m_nPosIndexFisrt);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SEND_OFFSET, m_bEnableAxisMoveLimit);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_X, m_dAxisAllowMoveMaxX);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_Y, m_dAxisAllowMoveMaxY);
	DDX_Text(pDX, IDC_EDIT_OFFSET_MAX_D, m_dAxisAllowMoveMaxD);

	DDX_Check(pDX, IDC_CHECK_BENDING_PRODUCT_SN_Invert, m_bBendInspectSnCodeInvert);
	DDX_Text(pDX, IDC_EDIT_BENG_SNCODE_LENGTH, m_bBendInspectSnCodeLength);
	DDX_Check(pDX, IDC_CHECK_BENDING_PRODUCT_GET_SN_ONES, m_bBendInspectGetSnOnce);

	DDX_Check(pDX, IDC_CHECK_FBS_LRY, m_bEnableFbsLRY);
	DDX_Check(pDX, IDC_CHECK_BEND_PRESS_PARAMETER_WRITE, m_bBendPressParaWrite);
	DDX_Text(pDX, IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, m_lBendPressParaWriteAddre);
	DDX_Check(pDX, IDC_CHECK_READ_WS_INFO, m_bEnableReadWsInfo);
	DDX_Check(pDX, IDC_CHECK_READ_WS_INFO2, m_bEnableReadInspectWsInfo);
	DDX_Check(pDX, IDC_CHECK_READ_WS_INFO3, m_bEnableReadStartTimeInfo);
	DDX_Check(pDX, IDC_CHECK_READ_WS_INFO4, m_bEnableReadEndTimeInfo);
	DDX_Check(pDX, IDC_CHECK_READ_WS_INFO5, m_bEnableReadProsessingTimeInfo);
	DDX_Text(pDX, IDC_EDIT_LEFT_DIS_OFFSET, m_strLeftDisOffset);
	DDX_Text(pDX, IDC_EDIT_RIGHT_DIS_OFFSET, m_strRightDisOffset);
	DDX_Check(pDX, IDC_CHECK_USE_MATH_CALU_LRDIS, m_bEnableUseMathCaluLRDis);
	DDX_Text(pDX, IDC_EDIT_PLARFORM_NAME1, m_strPlatformName1);
	DDX_Text(pDX, IDC_EDIT_PLARFORM_NAME2, m_strPlatformName2);
	DDX_Check(pDX, IDC_CHECK_FBIS_TAR_OBC, m_bEnableFbisTarObc);
}


BEGIN_MESSAGE_MAP(CDlgBendSetting, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT_BEND_PIXEL_SIZE, &CDlgBendSetting::OnSetfocusEditBendPixelSize)
	ON_EN_KILLFOCUS(IDC_EDIT_BEND_PIXEL_SIZE, &CDlgBendSetting::OnKillfocusEditBendPixelSize)
	ON_EN_SETFOCUS(IDC_EDIT_TARGET_DISTANCE_OFFSET, &CDlgBendSetting::OnSetfocusEditTargetDistanceOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_TARGET_DISTANCE_OFFSET, &CDlgBendSetting::OnKillfocusEditTargetDistanceOffset)
	ON_EN_SETFOCUS(IDC_EDIT_OBJECT_DISTANCE_OFFSET, &CDlgBendSetting::OnSetfocusEditObjectDistanceOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECT_DISTANCE_OFFSET, &CDlgBendSetting::OnKillfocusEditObjectDistanceOffset)
	ON_EN_SETFOCUS(IDC_EDIT_LEFT_OFFSET_X, &CDlgBendSetting::OnSetfocusEditLeftOffsetX)
	ON_EN_KILLFOCUS(IDC_EDIT_LEFT_OFFSET_X, &CDlgBendSetting::OnKillfocusEditLeftOffsetX)
	ON_EN_SETFOCUS(IDC_EDIT_LEFT_OFFSET_Y, &CDlgBendSetting::OnSetfocusEditLeftOffsetY)
	ON_EN_KILLFOCUS(IDC_EDIT_LEFT_OFFSET_Y, &CDlgBendSetting::OnKillfocusEditLeftOffsetY)
	ON_EN_SETFOCUS(IDC_EDIT_RIGHT_OFFSET_X, &CDlgBendSetting::OnSetfocusEditRightOffsetX)
	ON_EN_KILLFOCUS(IDC_EDIT_RIGHT_OFFSET_X, &CDlgBendSetting::OnKillfocusEditRightOffsetX)
	ON_EN_SETFOCUS(IDC_EDIT_RIGHT_OFFSET_Y, &CDlgBendSetting::OnSetfocusEditRightOffsetY)
	ON_EN_KILLFOCUS(IDC_EDIT_RIGHT_OFFSET_Y, &CDlgBendSetting::OnKillfocusEditRightOffsetY)
	ON_EN_SETFOCUS(IDC_EDIT_THRE_OFFSET_X, &CDlgBendSetting::OnSetfocusEditThreOffsetX)
	ON_EN_KILLFOCUS(IDC_EDIT_THRE_OFFSET_X, &CDlgBendSetting::OnKillfocusEditThreOffsetX)
	ON_EN_SETFOCUS(IDC_EDIT_THRE_OFFSET_Y, &CDlgBendSetting::OnSetfocusEditThreOffsetY)
	ON_EN_KILLFOCUS(IDC_EDIT_THRE_OFFSET_Y, &CDlgBendSetting::OnKillfocusEditThreOffsetY)
	ON_EN_SETFOCUS(IDC_EDIT_PRESS_OK_MIN_Y, &CDlgBendSetting::OnSetfocusEditPressOkMinY)
	ON_EN_KILLFOCUS(IDC_EDIT_PRESS_OK_MIN_Y, &CDlgBendSetting::OnKillfocusEditPressOkMinY)
	ON_EN_SETFOCUS(IDC_EDIT_PRESS_OK_MAX_Y, &CDlgBendSetting::OnSetfocusEditPressOkMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_PRESS_OK_MAX_Y, &CDlgBendSetting::OnKillfocusEditPressOkMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_PRESS_NG_MIN_Y, &CDlgBendSetting::OnSetfocusEditPressNgMinY)
	ON_EN_KILLFOCUS(IDC_EDIT_PRESS_NG_MIN_Y, &CDlgBendSetting::OnKillfocusEditPressNgMinY)
	ON_EN_SETFOCUS(IDC_EDIT_PRESS_NG_MAX_Y, &CDlgBendSetting::OnSetfocusEditPressNgMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_PRESS_NG_MAX_Y, &CDlgBendSetting::OnKillfocusEditPressNgMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_BEND_LOAD_X, &CDlgBendSetting::OnKillfocusEditBendLoadX)
	ON_EN_SETFOCUS(IDC_EDIT_BEND_LOAD_X, &CDlgBendSetting::OnSetfocusEditBendLoadX)
	ON_EN_KILLFOCUS(IDC_EDIT_BEND_LOAD_Y, &CDlgBendSetting::OnKillfocusEditBendLoadY)
	ON_EN_SETFOCUS(IDC_EDIT_BEND_LOAD_Y, &CDlgBendSetting::OnSetfocusEditBendLoadY)
	ON_BN_CLICKED(IDC_CHECK_BENDING_INS, &CDlgBendSetting::OnBnClickedCheckBendingIns)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_X, &CDlgBendSetting::OnEnKillfocusEditOffsetMaxX)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_X, &CDlgBendSetting::OnEnSetfocusEditOffsetMaxX)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_Y, &CDlgBendSetting::OnEnKillfocusEditOffsetMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_Y, &CDlgBendSetting::OnEnSetfocusEditOffsetMaxY)
	ON_EN_KILLFOCUS(IDC_EDIT_OFFSET_MAX_D, &CDlgBendSetting::OnEnKillfocusEditOffsetMaxD)
	ON_EN_SETFOCUS(IDC_EDIT_OFFSET_MAX_D, &CDlgBendSetting::OnEnSetfocusEditOffsetMaxD)
//	ON_EN_SETFOCUS(IDC_EDIT_BENG_SNCODE_LENGTH, &CDlgBendSetting::OnEnSetfocusEditSnCode)

	
//	ON_EN_SETFOCUS(IDC_EDIT_BENG_NUM_OF_INSPECT_RESULTS, &CDlgBendSetting::OnEnSetfocusEditBengNumOfInspectResults)
	ON_EN_KILLFOCUS(IDC_EDIT_BENG_SNCODE_LENGTH, &CDlgBendSetting::OnEnKillfocusEditBengSncodeLength)
	ON_EN_SETFOCUS(IDC_EDIT_BENG_SNCODE_LENGTH, &CDlgBendSetting::OnEnSetfocusEditBengSncodeLength)
//	ON_EN_KILLFOCUS(IDC_EDIT_BENG_NUM_OF_INSPECT_RESULTS, &CDlgBendSetting::OnEnKillfocusEditBengNumOfInspectResults)
ON_BN_CLICKED(IDC_CHECK_BEND_PRESS_PARAMETER_WRITE, &CDlgBendSetting::OnBnClickedCheckBendPressParameterWrite)
ON_EN_KILLFOCUS(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, &CDlgBendSetting::OnEnKillfocusEditIdcBendPressParameterWriteAddre)
ON_EN_SETFOCUS(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, &CDlgBendSetting::OnEnSetfocusEditIdcBendPressParameterWriteAddre)
ON_BN_CLICKED(IDC_CHECK_BEND_INSPECT, &CDlgBendSetting::OnBnClickedCheckBendInspect)
ON_BN_CLICKED(IDC_CHECK_BEND_ALIGN_DMCODE, &CDlgBendSetting::OnBnClickedCheckBendAlignDmcode)
ON_BN_CLICKED(IDC_CHECK_BEND_DISTANCE_OFFSET, &CDlgBendSetting::OnBnClickedCheckBendDistanceOffset)
ON_BN_CLICKED(IDC_CHECK_BEND_RESULT_OFFSET, &CDlgBendSetting::OnBnClickedCheckBendResultOffset)
ON_BN_CLICKED(IDC_CHECK_BEND_THRE_OFFSET, &CDlgBendSetting::OnBnClickedCheckBendThreOffset)
ON_BN_CLICKED(IDC_CHECK_BEND_PRESS_PARAMETER, &CDlgBendSetting::OnBnClickedCheckBendPressParameter)
ON_BN_CLICKED(IDC_CHECK_BEND_LOAD_THRE, &CDlgBendSetting::OnBnClickedCheckBendLoadThre)
ON_BN_CLICKED(IDC_CHECK_ENABLE_SEND_OFFSET, &CDlgBendSetting::OnBnClickedCheckEnableSendOffset)
ON_BN_CLICKED(IDC_CHECK_BEND_INSPECT_DMCODE, &CDlgBendSetting::OnBnClickedCheckBendInspectDmcode)
ON_BN_CLICKED(IDC_CHECK_ENABLE_MOVE_SNAP, &CDlgBendSetting::OnBnClickedCheckEnableMoveSnap)
ON_BN_CLICKED(IDC_CHECK_FBS_LRY, &CDlgBendSetting::OnBnClickedCheckFbsLry)
ON_BN_CLICKED(IDC_CHECK_BENDING_PRODUCT_ID, &CDlgBendSetting::OnBnClickedCheckBendingProductId)
ON_BN_CLICKED(IDC_CHECK_BENDING_PRODUCT_GET_SN_ONES, &CDlgBendSetting::OnBnClickedCheckBendingProductGetSnOnes)
ON_BN_CLICKED(IDC_CHECK_BENDING_PRODUCT_SN_Invert, &CDlgBendSetting::OnBnClickedCheckBendingProductSnInvert)
ON_BN_CLICKED(IDC_RADIO_BEND_PLATFORM_INSPECT, &CDlgBendSetting::OnBnClickedRadioBendPlatformInspect)
ON_BN_CLICKED(IDC_RADIO_BEND_IMAGE_INSPECT, &CDlgBendSetting::OnBnClickedRadioBendImageInspect)
ON_BN_CLICKED(IDC_RADIO_POSTION1_FRIST, &CDlgBendSetting::OnBnClickedRadioPostion1Frist)
ON_BN_CLICKED(IDC_RADIO_POSTION2_FRIST, &CDlgBendSetting::OnBnClickedRadioPostion2Frist)
ON_BN_CLICKED(IDC_CHECK_READ_WS_INFO, &CDlgBendSetting::OnBnClickedCheckReadWsInfo)
ON_BN_CLICKED(IDC_CHECK_READ_WS_INFO2, &CDlgBendSetting::OnBnClickedCheckReadWsInfo2)
ON_BN_CLICKED(IDC_CHECK_READ_WS_INFO3, &CDlgBendSetting::OnBnClickedCheckReadWsInfo3)
ON_BN_CLICKED(IDC_CHECK_READ_WS_INFO4, &CDlgBendSetting::OnBnClickedCheckReadWsInfo4)
ON_BN_CLICKED(IDC_CHECK_READ_WS_INFO5, &CDlgBendSetting::OnBnClickedCheckReadWsInfo5)
ON_EN_SETFOCUS(IDC_EDIT_LEFT_DIS_OFFSET, &CDlgBendSetting::OnEnSetfocusEditLeftDisOffset)
ON_EN_KILLFOCUS(IDC_EDIT_LEFT_DIS_OFFSET, &CDlgBendSetting::OnEnKillfocusEditLeftDisOffset)
ON_EN_SETFOCUS(IDC_EDIT_RIGHT_DIS_OFFSET, &CDlgBendSetting::OnEnSetfocusEditRightDisOffset)
ON_EN_KILLFOCUS(IDC_EDIT_RIGHT_DIS_OFFSET, &CDlgBendSetting::OnEnKillfocusEditRightDisOffset)
ON_BN_CLICKED(IDC_CHECK_USE_MATH_CALU_LRDIS, &CDlgBendSetting::OnBnClickedCheckUseMathCaluLrdis)
ON_BN_CLICKED(IDC_CHECK_FBIS_TAR_OBC, &CDlgBendSetting::OnBnClickedCheckFbisTarObc)
END_MESSAGE_MAP()


// CDlgBendSetting 消息处理程序

BOOL CDlgBendSetting::IsStringNumerical(LPCTSTR lpszVal)
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



BOOL CDlgBendSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_bEnableMultiCalibExtension)
	{
		GetDlgItem(IDC_CHECK_BEND_ALIGN_DMCODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_BEND_INSPECT_DMCODE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CHECK_BEND_ALIGN_DMCODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_BEND_INSPECT_DMCODE)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_CHECK_ENABLE_MOVE_SNAP)->EnableWindow(m_bBendingInspect);
	GetDlgItem(IDC_RADIO_POSTION1_FRIST)->EnableWindow(m_bBendingInspect);
	GetDlgItem(IDC_RADIO_POSTION2_FRIST)->EnableWindow(m_bBendingInspect);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBendSetting::OnSetfocusEditBendPixelSize()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		PostMessage(WM_SETFOCUS);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_BEND_PIXEL_SIZE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_BEND_PIXEL_SIZE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_BEND_PIXEL_SIZE, strOutput);
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditBendPixelSize();
	}


}


void CDlgBendSetting::OnKillfocusEditBendPixelSize()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_BEND_PIXEL_SIZE, strVal);
		strOldVal =  m_strPixelSize;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_BEND_PIXEL_SIZE, m_strPixelSize);
		}
		else
		{
			m_strPixelSize.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_BEND_PIXEL_SIZE, m_strPixelSize);
		}

		if (strOldVal != m_strPixelSize)
		{
			CString strLog;
			strLog.Format(_T("修改[像素当量]由%s到%s"),strOldVal,m_strPixelSize);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	

}


void CDlgBendSetting::OnSetfocusEditTargetDistanceOffset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TARGET_DISTANCE_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TARGET_DISTANCE_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TARGET_DISTANCE_OFFSET, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetDistanceOffset();
	}
}


void CDlgBendSetting::OnKillfocusEditTargetDistanceOffset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_TARGET_DISTANCE_OFFSET, strVal);
		strOldVal = m_strTargetDisOffset;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_TARGET_DISTANCE_OFFSET, m_strTargetDisOffset);
		}
		else
		{
			m_strTargetDisOffset.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_TARGET_DISTANCE_OFFSET, m_strTargetDisOffset);
		}

		if (strOldVal != m_strTargetDisOffset)
		{
			CString strLog;
			strLog.Format(_T("修改[目标距离补偿]由%s到%s"),strOldVal,m_strTargetDisOffset);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnSetfocusEditObjectDistanceOffset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OBJECT_DISTANCE_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OBJECT_DISTANCE_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_DISTANCE_OFFSET, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditObjectDistanceOffset();
	}

}


void CDlgBendSetting::OnKillfocusEditObjectDistanceOffset()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_OBJECT_DISTANCE_OFFSET, strVal);
		strOldVal = m_strObjectDisOffset;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_OBJECT_DISTANCE_OFFSET, m_strObjectDisOffset);
		}
		else
		{
			m_strObjectDisOffset.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_OBJECT_DISTANCE_OFFSET, m_strObjectDisOffset);
		}

		if (strOldVal != m_strObjectDisOffset)
		{
			CString strLog;
			strLog.Format(_T("修改[对象距离补偿]由%s到%s"),strOldVal,m_strObjectDisOffset);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnSetfocusEditLeftOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LEFT_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LEFT_OFFSET_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_X, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditLeftOffsetX();
	}

}


void CDlgBendSetting::OnKillfocusEditLeftOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_LEFT_OFFSET_X, strVal);
		strOldVal = m_strLeftOffsetX;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_X, m_strLeftOffsetX);
		}
		else
		{
			m_strLeftOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_X, m_strLeftOffsetX);
		}

		if (strOldVal != m_strLeftOffsetX)
		{
			CString strLog;
			strLog.Format(_T("修改[左侧修正ΔLX]由%s到%s"),strOldVal,m_strLeftOffsetX);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	

}


void CDlgBendSetting::OnSetfocusEditLeftOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LEFT_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LEFT_OFFSET_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditLeftOffsetY();
	}
}


void CDlgBendSetting::OnKillfocusEditLeftOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_LEFT_OFFSET_Y, strVal);
		strOldVal = m_strLeftOffsetY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_Y, m_strLeftOffsetY);
		}
		else
		{
			m_strLeftOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_LEFT_OFFSET_Y, m_strLeftOffsetY);
		}

		if (strOldVal != m_strLeftOffsetY)
		{
			CString strLog;
			strLog.Format(_T("修改[左侧修正ΔLY]由%s到%s"),strOldVal,m_strLeftOffsetY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}

void CDlgBendSetting::OnSetfocusEditRightOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_RIGHT_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_RIGHT_OFFSET_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_X, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditRightOffsetX();
	}

}


void CDlgBendSetting::OnKillfocusEditRightOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_RIGHT_OFFSET_X, strVal);
		strOldVal = m_strRightOffsetX;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_X, m_strRightOffsetX);
		}
		else
		{
			m_strRightOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_X, m_strRightOffsetX);
		}

		if (strOldVal != m_strRightOffsetX)
		{
			CString strLog;
			strLog.Format(_T("修改[右侧修正ΔRX]由%s到%s"),strOldVal,m_strRightOffsetX);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}

void CDlgBendSetting::OnSetfocusEditRightOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_RIGHT_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_RIGHT_OFFSET_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditRightOffsetY();
	}
}


void CDlgBendSetting::OnKillfocusEditRightOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_RIGHT_OFFSET_Y, strVal);
		strOldVal = m_strRightOffsetY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_Y, m_strRightOffsetY);
		}
		else
		{
			m_strRightOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_RIGHT_OFFSET_Y, m_strRightOffsetY);
		}

		if (strOldVal != m_strRightOffsetY)
		{
			CString strLog;
			strLog.Format(_T("修改[右侧修正ΔRY]由%s到%s"),strOldVal,m_strRightOffsetY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}

}



void CDlgBendSetting::OnSetfocusEditThreOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_THRE_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_THRE_OFFSET_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_X, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditThreOffsetX();
	}

}


void CDlgBendSetting::OnKillfocusEditThreOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_THRE_OFFSET_X, strVal);
		strOldVal = m_strThreOffsetX;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_X, m_strThreOffsetX);
		}
		else
		{
			m_strThreOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_X, m_strThreOffsetX);
		}

		if (strOldVal != m_strThreOffsetX)
		{
			CString strLog;
			strLog.Format(_T("修改[阈值修正X]由%s到%s"),strOldVal,m_strThreOffsetX);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	


}


void CDlgBendSetting::OnSetfocusEditThreOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_THRE_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_THRE_OFFSET_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditThreOffsetY();
	}

}


void CDlgBendSetting::OnKillfocusEditThreOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_THRE_OFFSET_Y, strVal);
		strOldVal = m_strThreOffsetY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_Y, m_strThreOffsetY);
		}
		else
		{
			m_strThreOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_THRE_OFFSET_Y, m_strThreOffsetY);
		}

		if (strOldVal != m_strThreOffsetY)
		{
			CString strLog;
			strLog.Format(_T("修改[阈值修正Y]由%s到%s"),strOldVal,m_strThreOffsetY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnSetfocusEditPressOkMinY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PRESS_OK_MIN_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_PRESS_OK_MIN_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PRESS_OK_MIN_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPressOkMinY();
	}


}


void CDlgBendSetting::OnKillfocusEditPressOkMinY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_PRESS_OK_MIN_Y, strVal);
		strOldVal = m_strPressOKMinY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PRESS_OK_MIN_Y, m_strPressOKMinY);
		}
		else
		{
			m_strPressOKMinY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PRESS_OK_MIN_Y, m_strPressOKMinY);
		}

		if (strOldVal != m_strPressOKMinY)
		{
			CString strLog;
			strLog.Format(_T("修改[压合阈值YMIN]由%s到%s"),strOldVal,m_strPressOKMinY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnSetfocusEditPressOkMaxY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PRESS_OK_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_PRESS_OK_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PRESS_OK_MAX_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPressOkMaxY();
	}

}


void CDlgBendSetting::OnKillfocusEditPressOkMaxY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_PRESS_OK_MAX_Y, strVal);
		strOldVal = m_strPressOKMaxY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PRESS_OK_MAX_Y, m_strPressOKMaxY);
		}
		else
		{
			m_strPressOKMaxY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PRESS_OK_MAX_Y, m_strPressOKMaxY);
		}

		if (strOldVal != m_strPressOKMaxY)
		{
			CString strLog;
			strLog.Format(_T("修改[压合阈值YMAX]由%s到%s"),strOldVal,m_strPressOKMaxY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	

}


void CDlgBendSetting::OnSetfocusEditPressNgMinY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PRESS_NG_MIN_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_PRESS_NG_MIN_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PRESS_NG_MIN_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPressNgMinY();
	}

}


void CDlgBendSetting::OnKillfocusEditPressNgMinY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_PRESS_NG_MIN_Y, strVal);
		strOldVal = m_strPressNGMinY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PRESS_NG_MIN_Y, m_strPressNGMinY);
		}
		else
		{
			m_strPressNGMinY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PRESS_NG_MIN_Y, m_strPressNGMinY);
		}

		if (strOldVal != m_strPressNGMinY)
		{
			CString strLog;
			strLog.Format(_T("修改[报警阈值YMIN]由%s到%s"),strOldVal,m_strPressNGMinY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	

}


void CDlgBendSetting::OnSetfocusEditPressNgMaxY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PRESS_NG_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_PRESS_NG_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PRESS_NG_MAX_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditPressNgMaxY();
	}
}


void CDlgBendSetting::OnKillfocusEditPressNgMaxY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_PRESS_NG_MAX_Y, strVal);
		strOldVal = m_strPressNGMaxY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_PRESS_NG_MAX_Y, m_strPressNGMaxY);
		}
		else
		{
			m_strPressNGMaxY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_PRESS_NG_MAX_Y, m_strPressNGMaxY);
		}

		if (strOldVal != m_strPressNGMaxY)
		{
			CString strLog;
			strLog.Format(_T("修改[报警阈值YMAX]由%s到%s"),strOldVal,m_strPressNGMaxY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}



void CDlgBendSetting::OnKillfocusEditBendLoadX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_BEND_LOAD_X, strVal);
		strOldVal = m_strBendLoadThreX;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_BEND_LOAD_X, m_strBendLoadThreX);
		}
		else
		{
			m_strBendLoadThreX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_BEND_LOAD_X, m_strBendLoadThreX);
		}

		if (strOldVal != m_strBendLoadThreX)
		{
			CString strLog;
			strLog.Format(_T("修改[上料管控X]由%s到%s"),strOldVal,m_strBendLoadThreX);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnSetfocusEditBendLoadX()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_BEND_LOAD_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_BEND_LOAD_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_BEND_LOAD_X, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditBendLoadX();
	}
}


void CDlgBendSetting::OnKillfocusEditBendLoadY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_BEND_LOAD_Y, strVal);
		strOldVal = m_strBendLoadThreY;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_BEND_LOAD_Y, m_strBendLoadThreY);
		}
		else
		{
			m_strBendLoadThreY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_BEND_LOAD_Y, m_strBendLoadThreY);
		}

		if (strOldVal != m_strBendLoadThreY)
		{
			CString strLog;
			strLog.Format(_T("修改[上料管控Y]由%s到%s"),strOldVal,m_strBendLoadThreY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnSetfocusEditBendLoadY()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_BEND_LOAD_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_BEND_LOAD_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_BEND_LOAD_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditBendLoadY();
	}
}


void CDlgBendSetting::OnBnClickedCheckBendingIns()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendingInspect;
	UpdateData(TRUE);

	if (bOldEnable != m_bBendingInspect)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BENDING_INS, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendingInspect ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}

	GetDlgItem(IDC_CHECK_ENABLE_MOVE_SNAP)->EnableWindow(m_bBendingInspect);
	GetDlgItem(IDC_RADIO_POSTION1_FRIST)->EnableWindow(m_bBendingInspect);
	GetDlgItem(IDC_RADIO_POSTION2_FRIST)->EnableWindow(m_bBendingInspect);
	UpdateData(FALSE);
}


void CDlgBendSetting::OnEnKillfocusEditOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strVal);
		double dAxisAllowMoveMaxX = m_dAxisAllowMoveMaxX;
		if (!IsStringNumerical(strVal))
		{
			CString strValOld;
			strValOld.Format(_T("%.6f"),m_dAxisAllowMoveMaxX);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strValOld);
		}
		else
		{
			m_dAxisAllowMoveMaxX = atof(strVal);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strVal);
		}

		if (dAxisAllowMoveMaxX != m_dAxisAllowMoveMaxX)
		{
			CString strLog;
			strLog.Format(_T("修改[X轴最大偏差量限制]由%.6f到%.6f"),dAxisAllowMoveMaxX,m_dAxisAllowMoveMaxX);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnEnSetfocusEditOffsetMaxX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_X, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxX();
	}
}


void CDlgBendSetting::OnEnKillfocusEditOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strVal);
		double dAxisAllowMoveMaxY = m_dAxisAllowMoveMaxY;
		if (!IsStringNumerical(strVal))
		{
			CString strValOld;
			strValOld.Format(_T("%.6f"),m_dAxisAllowMoveMaxY);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strValOld);
		}
		else
		{
			m_dAxisAllowMoveMaxY = atof(strVal);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strVal);
		}

		if (dAxisAllowMoveMaxY != m_dAxisAllowMoveMaxY)
		{
			CString strLog;
			strLog.Format(_T("修改[Y轴最大偏差量限制]由%.6f到%.6f"),dAxisAllowMoveMaxY,m_dAxisAllowMoveMaxY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnEnSetfocusEditOffsetMaxY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_Y, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxY();
	}
}


void CDlgBendSetting::OnEnKillfocusEditOffsetMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strVal);
		double dAxisAllowMoveMaxD = m_dAxisAllowMoveMaxD;
		if (!IsStringNumerical(strVal))
		{
			CString strValOld;
			strValOld.Format(_T("%.6f"),m_dAxisAllowMoveMaxD);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strValOld);
		}
		else
		{
			m_dAxisAllowMoveMaxD = atof(strVal);
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strVal);
		}

		if (dAxisAllowMoveMaxD != m_dAxisAllowMoveMaxY)
		{
			CString strLog;
			strLog.Format(_T("修改[θ轴最大偏差量限制]由%.6f到%.6f"),dAxisAllowMoveMaxD,m_dAxisAllowMoveMaxY);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}

void CDlgBendSetting::OnEnSetfocusEditOffsetMaxD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OFFSET_MAX_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OFFSET_MAX_D, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditOffsetMaxD();
	}
}

//SN码长度
void CDlgBendSetting::OnEnKillfocusEditBengSncodeLength()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal,strValOld;
		GetDlgItemText(IDC_EDIT_BENG_SNCODE_LENGTH, strVal);
		int nOldLength = m_bBendInspectSnCodeLength;
		if (!IsStringNumerical(strVal))
		{
			CString strValOld;
			strValOld.Format(_T("%d"),m_bBendInspectSnCodeLength);
			SetDlgItemText(IDC_EDIT_BENG_SNCODE_LENGTH, strValOld);
		}
		else
		{
			m_bBendInspectSnCodeLength = atoi(strVal);
			SetDlgItemText(IDC_EDIT_BENG_SNCODE_LENGTH, strVal);
		}

		if (nOldLength != m_bBendInspectSnCodeLength)
		{
			CString strLog;
			strLog.Format(_T("修改[SN码长度]由%d到%d"),nOldLength,m_bBendInspectSnCodeLength);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnEnSetfocusEditBengSncodeLength()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_BENG_SNCODE_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_BENG_SNCODE_LENGTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_BENG_SNCODE_LENGTH, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditBengSncodeLength();
	}
}



void CDlgBendSetting::OnBnClickedCheckBendPressParameterWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldBendPressParaWrite = m_bBendPressParaWrite;
	m_bBendPressParaWrite = !m_bBendPressParaWrite;
	if (bOldBendPressParaWrite != m_bBendPressParaWrite)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_PRESS_PARAMETER_WRITE, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendAlignInspect ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnEnKillfocusEditIdcBendPressParameterWriteAddre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, strVal);
		long lBendPressParaWriteAddre = m_lBendPressParaWriteAddre;
		if (!IsStringNumerical(strVal))
		{
			CString strValOld;
			strValOld.Format(_T("%d"),m_lBendPressParaWriteAddre);
			SetDlgItemText(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, strValOld);
		}
		else
		{
			m_lBendPressParaWriteAddre = atol(strVal);
			SetDlgItemText(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, strVal);
		}

		if (lBendPressParaWriteAddre != m_lBendPressParaWriteAddre)
		{
			CString strLog;
			strLog.Format(_T("修改[压合参数写入地址]由%d到%d"),lBendPressParaWriteAddre,m_lBendPressParaWriteAddre);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}
}


void CDlgBendSetting::OnEnSetfocusEditIdcBendPressParameterWriteAddre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_IDC_BEND_PRESS_PARAMETER_WRITE_ADDRE, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditIdcBendPressParameterWriteAddre();
	}
}


void CDlgBendSetting::OnBnClickedCheckBendInspect()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendAlignInspect;
	m_bBendAlignInspect = !m_bBendAlignInspect;
	if (bOldEnable != m_bBendAlignInspect)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_INSPECT, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendAlignInspect ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendAlignDmcode()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendAlignDmCode;
	m_bBendAlignDmCode = !m_bBendAlignDmCode;
	if (bOldEnable != m_bBendAlignDmCode)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_ALIGN_DMCODE, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendAlignDmCode ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendDistanceOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendDisOffset;
	m_bBendDisOffset = !m_bBendDisOffset;
	if (bOldEnable != m_bBendDisOffset)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_DISTANCE_OFFSET, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendDisOffset ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendResultOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendResultOffset;
	m_bBendResultOffset = !m_bBendResultOffset;
	if (bOldEnable != m_bBendResultOffset)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_RESULT_OFFSET, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendResultOffset ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendThreOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendThreOffset;
	m_bBendThreOffset = !m_bBendThreOffset;
	if (bOldEnable != m_bBendThreOffset)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_THRE_OFFSET, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendThreOffset ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendPressParameter()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendPressPara;
	m_bBendPressPara = !m_bBendPressPara;
	if (bOldEnable != m_bBendPressPara)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_PRESS_PARAMETER, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendPressPara ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendLoadThre()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendLoadThre;
	m_bBendLoadThre = !m_bBendLoadThre;
	if (bOldEnable != m_bBendLoadThre)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_LOAD_THRE, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendLoadThre ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckEnableSendOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableAxisMoveLimit;
	m_bEnableAxisMoveLimit = !m_bEnableAxisMoveLimit;
	if (bOldEnable != m_bEnableAxisMoveLimit)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_ENABLE_SEND_OFFSET, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableAxisMoveLimit ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendInspectDmcode()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendInspectDmCode;
	m_bBendInspectDmCode = !m_bBendInspectDmCode;
	if (bOldEnable != m_bBendInspectDmCode)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BEND_INSPECT_DMCODE, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendInspectDmCode ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckEnableMoveSnap()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableMoveSnap;
	m_bEnableMoveSnap = !m_bEnableMoveSnap;
	if (bOldEnable != m_bEnableMoveSnap)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_ENABLE_MOVE_SNAP, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableMoveSnap ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckFbsLry()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableFbsLRY;
	m_bEnableFbsLRY = !m_bEnableFbsLRY;
	if (bOldEnable != m_bEnableFbsLRY)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_FBS_LRY, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableFbsLRY ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendingProductId()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendingProductID;
	m_bBendingProductID = !m_bBendingProductID;
	if (bOldEnable != m_bBendingProductID)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BENDING_PRODUCT_ID, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendingProductID ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendingProductGetSnOnes()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendInspectGetSnOnce;
	m_bBendInspectGetSnOnce = !m_bBendInspectGetSnOnce;
	if (bOldEnable != m_bBendInspectGetSnOnce)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BENDING_PRODUCT_GET_SN_ONES, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendInspectGetSnOnce ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckBendingProductSnInvert()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bBendInspectSnCodeInvert;
	m_bBendInspectSnCodeInvert = !m_bBendInspectSnCodeInvert;
	if (bOldEnable != m_bBendInspectSnCodeInvert)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_BENDING_PRODUCT_SN_Invert, strTemp);
		strLog.Format(_T("%s[%s]"),m_bBendInspectSnCodeInvert ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedRadioBendPlatformInspect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog,strTemp;
	GetDlgItemText(IDC_RADIO_BEND_PLATFORM_INSPECT, strTemp);
	strLog.Format(_T("选中[%s]"),strTemp);
	g_opBendLogFile.SaveLogText(strLog,2);
}


void CDlgBendSetting::OnBnClickedRadioBendImageInspect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog,strTemp;
	GetDlgItemText(IDC_RADIO_BEND_IMAGE_INSPECT, strTemp);
	strLog.Format(_T("选中[%s]"),strTemp);
	g_opBendLogFile.SaveLogText(strLog,2);
}


void CDlgBendSetting::OnBnClickedRadioPostion1Frist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog,strTemp;
	GetDlgItemText(IDC_RADIO_POSTION1_FRIST, strTemp);
	strLog.Format(_T("选中[%s]"),strTemp);
	g_opBendLogFile.SaveLogText(strLog,2);
}


void CDlgBendSetting::OnBnClickedRadioPostion2Frist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog,strTemp;
	GetDlgItemText(IDC_RADIO_POSTION2_FRIST, strTemp);
	strLog.Format(_T("选中[%s]"),strTemp);
	g_opBendLogFile.SaveLogText(strLog,2);
}


void CDlgBendSetting::OnBnClickedCheckReadWsInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableReadWsInfo;
	m_bEnableReadWsInfo = !m_bEnableReadWsInfo;
	if (bOldEnable != m_bEnableReadWsInfo)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_READ_WS_INFO, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableReadWsInfo ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}





void CDlgBendSetting::OnBnClickedCheckReadWsInfo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableReadInspectWsInfo;
	m_bEnableReadInspectWsInfo = !m_bEnableReadInspectWsInfo;
	if (bOldEnable != m_bEnableReadInspectWsInfo)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_READ_WS_INFO2, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableReadInspectWsInfo ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckReadWsInfo3()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableReadStartTimeInfo;
	m_bEnableReadStartTimeInfo = !m_bEnableReadStartTimeInfo;
	if (bOldEnable != m_bEnableReadStartTimeInfo)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_READ_WS_INFO3, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableReadStartTimeInfo ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckReadWsInfo4()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableReadEndTimeInfo;
	m_bEnableReadEndTimeInfo = !m_bEnableReadEndTimeInfo;
	if (bOldEnable != m_bEnableReadEndTimeInfo)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_READ_WS_INFO4, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableReadEndTimeInfo ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckReadWsInfo5()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableReadProsessingTimeInfo;
	m_bEnableReadProsessingTimeInfo = !m_bEnableReadProsessingTimeInfo;
	if (bOldEnable != m_bEnableReadProsessingTimeInfo)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_READ_WS_INFO5, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableReadProsessingTimeInfo ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnEnSetfocusEditLeftDisOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LEFT_DIS_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LEFT_DIS_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LEFT_DIS_OFFSET, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditLeftDisOffset();
	}
}


void CDlgBendSetting::OnEnKillfocusEditLeftDisOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_LEFT_DIS_OFFSET, strVal);
		strOldVal = m_strLeftDisOffset;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_LEFT_DIS_OFFSET, m_strLeftDisOffset);
		}
		else
		{
			m_strLeftDisOffset.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_LEFT_DIS_OFFSET, m_strLeftDisOffset);
		}

		if (strOldVal != m_strLeftDisOffset)
		{
			CString strLog;
			strLog.Format(_T("修改[右侧修正ΔLDis]由%s到%s"),strOldVal,m_strLeftDisOffset);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnEnSetfocusEditRightDisOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_RIGHT_DIS_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_RIGHT_DIS_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_DIS_OFFSET, strOutput);

		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditRightDisOffset();
	}
}


void CDlgBendSetting::OnEnKillfocusEditRightDisOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal,strOldVal;
		GetDlgItemText(IDC_EDIT_RIGHT_DIS_OFFSET, strVal);
		strOldVal = m_strRightDisOffset;
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_RIGHT_DIS_OFFSET, m_strRightDisOffset);
		}
		else
		{
			m_strRightDisOffset.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_RIGHT_DIS_OFFSET, m_strRightDisOffset);
		}

		if (strOldVal != m_strRightDisOffset)
		{
			CString strLog;
			strLog.Format(_T("修改[右侧修正ΔRDis]由%s到%s"),strOldVal,m_strRightDisOffset);
			g_opBendLogFile.SaveLogText(strLog,2);
		}
	}	
}


void CDlgBendSetting::OnBnClickedCheckUseMathCaluLrdis()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableUseMathCaluLRDis;
	m_bEnableUseMathCaluLRDis = !m_bEnableUseMathCaluLRDis;
	if (bOldEnable != m_bEnableUseMathCaluLRDis)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_USE_MATH_CALU_LRDIS, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableUseMathCaluLRDis ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}


void CDlgBendSetting::OnBnClickedCheckFbisTarObc()
{
	// TODO: 在此添加控件通知处理程序代码
	int bOldEnable = m_bEnableFbisTarObc;
	m_bEnableFbisTarObc = !m_bEnableFbisTarObc;
	if (bOldEnable != m_bEnableFbisTarObc)
	{
		CString strLog,strTemp;
		GetDlgItemText(IDC_CHECK_FBIS_TAR_OBC, strTemp);
		strLog.Format(_T("%s[%s]"),m_bEnableFbisTarObc ? _T("勾选"):_T("取消勾选"),strTemp);
		g_opBendLogFile.SaveLogText(strLog,2);
	}
}
