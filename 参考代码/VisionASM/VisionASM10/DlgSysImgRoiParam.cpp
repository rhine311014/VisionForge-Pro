// DlgSysImgRoiParam.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysImgRoiParam.h"
#include "DlgSysEnvironSetting.h"
#include "DlgManualMeasure.h"
#include "VisionASMDLG.h"
#include "LightController.h"
#include "DlgLightCtrlSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysImgRoiParam dialog


CDlgSysImgRoiParam::CDlgSysImgRoiParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysImgRoiParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysImgRoiParam)
	m_bHorizontalMirror = FALSE;
	m_bVerticalMirror = FALSE;
	m_bGammaEnable = FALSE;
	m_nHeightCustom = 480;
	m_nWidthCustom = 640;
	m_nStartX = 0;
	m_nStartY = 0;
	m_nCameraIndex = 0;
	m_nVideoMode = 0;
	m_nRotationIndex = 0;
	m_dCrossX = 0.0;
	m_dCrossY = 0.0;
	m_bCrossVisible = FALSE;
	//}}AFX_DATA_INIT
	m_pVAPrevMainWnd = NULL;
	m_pParent = NULL;
	m_pVisionASM = NULL;
	m_pGUIDisplay = NULL;
	m_nPrevCameraIndex = m_nCameraIndex;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;
	m_strCrossX = _T("0");
	m_strCrossY = _T("0");

	m_dWhiteBalanceRatioR = 1.0;
	m_dWhiteBalanceRatioG = 1.0;
	m_dWhiteBalanceRatioB = 1.0;

	m_bCamPixelFormat = false;

	m_bShowParamSet = true;
	m_bEnableParamSet= true;

	m_bShowColorSet= true;
	m_bEnableColorSet= true;

	m_bShowOtherSet = true;
	m_bEnableOtherSet = true;

}


void CDlgSysImgRoiParam::InitDlgItem()
{
	ASSERT(m_pVisionASM != NULL);
	int nCamNum = m_pVisionASM->GetPosNum();

	m_btnSetCustom.SetThemeHelper(&m_themeHelper);
// 	m_btnSetCustom.SetIcon(IDI_ICON_OK_16);
// 	m_btnSetCustom.SetAlign(CButtonST::ST_ALIGN_HORIZ);
	
	m_btnGrabImage.SetThemeHelper(&m_themeHelper);
// 	m_btnGrabImage.SetIcon(IDI_ICON_GRAB_40);
// 	m_btnGrabImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnSaveImage.SetThemeHelper(&m_themeHelper);
// 	m_btnSaveImage.SetIcon(IDI_ICON_SAVE_IMAGE_40);
// 	m_btnSaveImage.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnManualMeasure.SetThemeHelper(&m_themeHelper);
	// 调整相机选择按钮
	int i = 0;
	for (i=0; i<8; i++)
	{
		if (i < nCamNum)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(m_bShowParamSet);
			if (m_pVisionASM->IsCameraFirstPosIndex(i))
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->EnableWindow(m_bEnableParamSet);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->EnableWindow(FALSE);
			}
		}
		else
		{
			GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
		}
	}
	SysPlatformInfo platInfo;
	m_pVisionASM->GetSysPlatformInfo(platInfo);
	//当目标和对象不在同一台控制器时，修改位置选择单选按钮的显示文字以适应目标端或对象端的显示
	if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 8)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_7));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_8));
		GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+6)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		GetDlgItem(IDC_RADIO_CAMERA0+7)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
	}
	else if (platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 8)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
		GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
		GetDlgItem(IDC_RADIO_CAMERA0+6)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_7));
		GetDlgItem(IDC_RADIO_CAMERA0+7)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_8));
	}
	else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 6)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
	}
	else if (platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 6)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
		GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
	}
	else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 4)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
	}
	else if(platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 4)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
	}
	else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 2)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));

	}
	else if(platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 2)
	{
		GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
		GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));

	}
// 
// 	// 更新视频模式界面显示
// 	for (i=0; i<6; i++)
// 	{
// 		// 相机视频模式：0：640*480，1：800*600，2:1024*768,3:1280*960,4:1600*1200,5：自定义
// 		if (m_pVisionASM->IsVideoModeSupported(m_nCameraIndex, i))	
// 		{
// 			GetDlgItem(IDC_RADIO_CAMERA_MODE0+i)->EnableWindow(TRUE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_RADIO_CAMERA_MODE0+i)->EnableWindow(FALSE);
// 		}
// 	}

	// 更新“连续采集/停止采集”按钮
	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(FALSE);
	}
	else
	{
		if (m_pVisionASM->GrabPosCamera(m_nCameraIndex))
		{
			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
			GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
			GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(m_bEnableOtherSet);
			GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(m_bEnableOtherSet);
		}
	}

	if ((m_pParent!=NULL) && (m_pParent->m_pParent != NULL)
		&& ((m_pParent->m_pParent)->m_SysOptionConfig.m_bShowGamma))
	{
		GetDlgItem(IDC_STATIC_GAMMA)->ShowWindow(m_bShowParamSet);
		GetDlgItem(IDC_STATIC_GAMMA_VAL)->ShowWindow(m_bShowParamSet);
		GetDlgItem(IDC_SLIDER_GAMMA)->ShowWindow(m_bShowParamSet);
		GetDlgItem(IDC_CHECK_GAMMA)->ShowWindow(m_bShowParamSet);

		GetDlgItem(IDC_STATIC_GAMMA)->EnableWindow(m_bEnableParamSet);
		GetDlgItem(IDC_STATIC_GAMMA_VAL)->EnableWindow(m_bEnableParamSet);
		GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(m_bEnableParamSet);
		GetDlgItem(IDC_CHECK_GAMMA)->EnableWindow(m_bEnableParamSet);
	}
	else
	{
		GetDlgItem(IDC_STATIC_GAMMA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_GAMMA_VAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SLIDER_GAMMA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_GAMMA)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_GAMMA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_GAMMA_VAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_GAMMA)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_GAMMA)->EnableWindow(FALSE);
	}

	// 调整“连续采集”和“保存图像”按钮的大小
	if (platInfo.m_bPlatformShareShow)
// 	int nButtonSize = 75;
	{
		GetDlgItem(IDC_CHECK_HORIZONTAL_MIRROR)->EnableWindow(FALSE);
// 	ScreenToClient(rect);
// 	GetDlgItem(IDC_BTN_LIVE_GRAB)->MoveWindow(rect.left, rect.top, nButtonSize, nButtonSize);
// 
		GetDlgItem(IDC_CHECK_VERTICAL_MIRROR)->EnableWindow(FALSE);	
// 	ScreenToClient(rect);
// 	GetDlgItem(IDC_BTN_SAVE_IMAGE)->MoveWindow(rect.left, rect.top, nButtonSize, nButtonSize);
	}

	GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(_T("1.50"));
	GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(_T("0.00"));

	UpdateDlgByAuthorize();

	::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}


// 根据相机状态更新“连续采集/停止采集”按钮显示
void CDlgSysImgRoiParam::UpdateBtnDisplay()
{
	if (!m_pVisionASM->IsPosCameraOnline(m_nCameraIndex))
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		return;
	}
	else
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->EnableWindow(m_bEnableOtherSet);
		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(m_bEnableOtherSet &&m_bCamPixelFormat);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(m_bEnableOtherSet &&m_bCamPixelFormat);

	}

	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(m_bEnableColorSet &&m_bCamPixelFormat);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(m_bEnableColorSet &&m_bCamPixelFormat);
	}
	else
	{
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(m_bEnableOtherSet);
		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
	}
}

void CDlgSysImgRoiParam::UpdateFramerateShutterGainDisplay()
{
	double dMin = 0.0, dMax = 0.0, dCurrent = 0.0;
	CString strVal;
// 	// 更新帧率显示
// 	if (m_pVisionASM->GetCameraFrameRate(m_nCameraIndex, dCurrent, dMin, dMax))
// 	{
// 		m_sliderFramerate.EnableWindow(TRUE);
// 		m_sliderFramerate.SetRange(ceil(dMin/0.1), floor(dMax/0.1), TRUE);
// 		m_sliderFramerate.SetPos( (int) ((dCurrent+0.05) / 0.1) );
// 		strVal.Format(_T("%.1fHz"), dCurrent);
// 		GetDlgItem(IDC_STATIC_FRAMERATE_VAL)->SetWindowText(strVal);
// 	}
// 	else
// 	{
// 		m_sliderFramerate.EnableWindow(FALSE);
// 	}

	CameraParam camParam;
	m_pVisionASM->GetPosCameraParam(m_nCameraIndex, camParam);
	

	int nCamIdx=m_pVisionASM->GetPosCameraIndex(m_nCameraIndex);
	
	BOOL bEnable=TRUE;

	if (m_pVisionASM->GetCameraShareType(nCamIdx)==eShareTar)
	{
		bEnable=FALSE;
	}
	else
	{
		bEnable=m_bEnableParamSet;
	}

	// 更新曝光显示
	if (m_pVisionASM->GetPosCameraShutter(m_nCameraIndex, dCurrent, dMin, dMax))
	{
		dCurrent = camParam.m_dShutter;
		m_sliderShutter.EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_SHUTTER_DOWN)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_SHUTTER_UP)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->EnableWindow(bEnable);
		m_sliderShutter.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
		m_sliderShutter.SetPos( (int) ((dCurrent+0.005) / 0.01) );
		strVal.Format(_T("%.2fms"), dCurrent);
		GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);
		strVal.Format(_T("%.2f"), dCurrent);
		GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(strVal);
	}
	else
	{
		m_sliderShutter.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHUTTER_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SHUTTER_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->EnableWindow(FALSE);
	}
	
	// 更新增益显示
	if (m_pVisionASM->GetPosCameraGain(m_nCameraIndex, dCurrent, dMin, dMax))
	{
		dCurrent = camParam.m_dGain;
		m_sliderGain.EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_GAIN_DOWN)->EnableWindow(bEnable);
		GetDlgItem(IDC_BUTTON_GAIN_UP)->EnableWindow(bEnable);
		GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->EnableWindow(bEnable);
		m_sliderGain.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
		m_sliderGain.SetPos( (int) ((dCurrent+0.005) / 0.01) );
		strVal.Format(_T("%.2fdb"), dCurrent);
		GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strVal);
		strVal.Format(_T("%.2f"), dCurrent);
		GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(strVal);
	}
	else
	{
		m_sliderGain.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GAIN_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GAIN_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->EnableWindow(FALSE);
	}

// 	// 更新Gamma显示
// 	bool dCurEnable = false;
// 	if (m_pVisionASM->GetPosCameraGamma(m_nCameraIndex, dCurEnable, dCurrent, dMin, dMax))
// 	{
// 		dCurEnable = camParam.m_bGammaEnable;
// 		dCurrent = camParam.m_dGamma;
// 
// 		m_bGammaEnable = dCurEnable;
// 		m_sliderGamma.EnableWindow(bEnable);
// // 		GetDlgItem(IDC_BUTTON_GAIN_DOWN)->EnableWindow(bEnable);
// // 		GetDlgItem(IDC_BUTTON_GAIN_UP)->EnableWindow(bEnable);
// 		m_sliderGamma.SetRange(ceil(dMin/0.1), floor(dMax/0.1), TRUE);
// 		m_sliderGamma.SetPos( (int) ((dCurrent+0.05) / 0.1) );
// 		strVal.Format(_T("%.2f"), dCurrent);
// 		GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);
// 	}
// 	else
// 	{
// 		m_sliderGamma.EnableWindow(FALSE);
// // 		GetDlgItem(IDC_BUTTON_GAIN_DOWN)->EnableWindow(FALSE);
// // 		GetDlgItem(IDC_BUTTON_GAIN_UP)->EnableWindow(FALSE);
// 	}
}

void CDlgSysImgRoiParam::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);

		if ((m_nCameraIndex >= m_pVisionASM->GetPosNum()) || (m_nCameraIndex < 0))
		{
			m_nCameraIndex = 0;
		}
		CameraParam camParam;
		m_pVisionASM->GetPosCameraParam(m_nCameraIndex, camParam);
		camParam.m_nVideoMode = m_nVideoMode;
		if (m_nVideoMode == 5)
		{
			camParam.m_nCustemStartX = m_nStartX;
			camParam.m_nCustemStartY = m_nStartY;
			camParam.m_nCustemWidth = m_nWidthCustom;
			camParam.m_nCustemHeight = m_nHeightCustom;
		}
		camParam.m_dFrameRate = m_sliderFramerate.GetPos()*0.01;
		camParam.m_dShutter = m_sliderShutter.GetPos()*0.01;
		camParam.m_dGain = m_sliderGain.GetPos()*0.01;
		camParam.m_dGamma = m_sliderGamma.GetPos()*0.1;
		camParam.m_bGammaEnable = m_bGammaEnable;

		camParam.m_dWhiteBalanceRatioR = m_dWhiteBalanceRatioR;
		camParam.m_dWhiteBalanceRatioG = m_dWhiteBalanceRatioG;
		camParam.m_dWhiteBalanceRatioB = m_dWhiteBalanceRatioB;

		m_pVisionASM->SetPosCameraParam(m_nCameraIndex, camParam);

		ImageRoi roi;
		m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
		roi.m_bMirrorHoriz = m_bHorizontalMirror;
		roi.m_bMirrorVert = m_bVerticalMirror;
		roi.m_nRotation = m_nRotationIndex;
		roi.m_dGamma = m_sliderGamma.GetPos()*0.1;
		roi.m_bGammaCorrectionEnable = m_bGammaEnable;
		m_dCrossX = atof(m_strCrossX);
		m_dCrossY = atof(m_strCrossY);
		roi.m_dCrossX = m_dCrossX;
		roi.m_dCrossY = m_dCrossY;
		roi.m_bCrossVisible = m_bCrossVisible;
		m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);
	}
	else
	{
		BeginWaitCursor();
		UpdateBtnDisplay();

		int nCamNum = m_pVisionASM->GetPosNum();
		
		int i = 0;
		for (i=0; i<8; i++)
		{
			if (i < nCamNum)
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(m_bShowParamSet);
				if (m_pVisionASM->IsCameraFirstPosIndex(i))
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->EnableWindow(m_bEnableParamSet);
				}
				else
				{
					GetDlgItem(IDC_RADIO_CAMERA0+i)->EnableWindow(FALSE);
				}
			}
			else
			{
				GetDlgItem(IDC_RADIO_CAMERA0+i)->ShowWindow(SW_HIDE);
			}
		}
		SysPlatformInfo platInfo;
		m_pVisionASM->GetSysPlatformInfo(platInfo);
		//当目标和对象不在同一台控制器时，修改位置选择单选按钮的显示文字以适应目标端或对象端的显示
		if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 8)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_7));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_8));
			GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+6)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
			GetDlgItem(IDC_RADIO_CAMERA0+7)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		}
		else if (platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 8)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
			GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
			GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
			GetDlgItem(IDC_RADIO_CAMERA0+6)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_7));
			GetDlgItem(IDC_RADIO_CAMERA0+7)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_8));
		}
		else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 6)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
		}
		else if (platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 6)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
			GetDlgItem(IDC_RADIO_CAMERA0+4)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_5));
			GetDlgItem(IDC_RADIO_CAMERA0+5)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_6));
		}
		else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 4)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
		}
		else if(platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 4)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+2)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_3));
			GetDlgItem(IDC_RADIO_CAMERA0+3)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_4));
		}
		else if(platInfo.m_bTargetInDifferentComputer && platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 2)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));

		}
		else if(platInfo.m_bTargetObjectCamSeparate && platInfo.m_nPositionNum == 2)
		{
			GetDlgItem(IDC_RADIO_CAMERA0+0)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_1));
			GetDlgItem(IDC_RADIO_CAMERA0+1)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_POS_2));

		}
		// 
		if ((m_nCameraIndex >= nCamNum) || (m_nCameraIndex < 0)||!m_pVisionASM->IsCameraFirstPosIndex(m_nCameraIndex))
		{
			m_nCameraIndex = 0;
			m_nPrevCameraIndex = m_nCameraIndex;
		}

		CameraParam camParam;
		m_pVisionASM->GetPosCameraParam(m_nCameraIndex, camParam);
		m_nVideoMode = camParam.m_nVideoMode;

		m_dWhiteBalanceRatioR = camParam.m_dWhiteBalanceRatioR;
		m_dWhiteBalanceRatioG = camParam.m_dWhiteBalanceRatioG;
		m_dWhiteBalanceRatioB = camParam.m_dWhiteBalanceRatioB;

		CString strRatio;
		strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioR);
		GetDlgItem(IDC_EDIT_RATIO_R)->SetWindowText(strRatio);
		strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioG);
		GetDlgItem(IDC_EDIT_RATIO_G)->SetWindowText(strRatio);
		strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioB);
		GetDlgItem(IDC_EDIT_RATIO_B)->SetWindowText(strRatio);

// 		// 更新视频模式界面显示
// 		for (i=0; i<6; i++)
// 		{
// 			// 相机视频模式：0：640*480，1：800*600，2:1024*768,3:1280*960,4:1600*1200,5：自定义
// 			if (m_pVisionASM->IsVideoModeSupported(m_nCameraIndex, i))	
// 			{
// 				GetDlgItem(IDC_RADIO_CAMERA_MODE0+i)->EnableWindow(TRUE);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_RADIO_CAMERA_MODE0+i)->EnableWindow(FALSE);
// 				m_nVideoMode = (m_nVideoMode == i) ? (i+1) : m_nVideoMode;
// 			}
// 		}
// 		if (camParam.m_nVideoMode != m_nVideoMode)
// 		{
// 			m_pVisionASM->SetCameraVideoMode(m_nCameraIndex, m_nVideoMode, 
// 				sc2Vector(camParam.m_nCustemStartX, camParam.m_nCustemStartY), sc2Vector(camParam.m_nCustemWidth, camParam.m_nRawHeight));
// 		}
// 
// 		// 更新自定义视频模式界面显示
// 		if ((m_nVideoMode == 5) && (m_pVisionASM->IsVideoModeSupported(m_nCameraIndex, 5)))
// 		{
// 			GetDlgItem(IDC_EDIT_START_X)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_EDIT_START_Y)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_EDIT_CUSTOM_WIDTH)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_EDIT_CUSTOM_HEIGHT)->EnableWindow(TRUE);
// 			GetDlgItem(IDC_BTN_SET_CUSTOM)->EnableWindow(TRUE);
// 			m_nStartX = camParam.m_nCustemStartX;
// 			m_nStartY = camParam.m_nCustemStartY;
// 			m_nWidthCustom = camParam.m_nCustemWidth;
// 			m_nHeightCustom = camParam.m_nCustemHeight;
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_EDIT_START_X)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_EDIT_START_Y)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_EDIT_CUSTOM_WIDTH)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_EDIT_CUSTOM_HEIGHT)->EnableWindow(FALSE);
// 			GetDlgItem(IDC_BTN_SET_CUSTOM)->EnableWindow(FALSE);
// 		}

		UpdateFramerateShutterGainDisplay();
		
		// 更新旋转镜像设置显示
		ImageRoi roi;
		m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
		m_bHorizontalMirror = roi.m_bMirrorHoriz;
		m_bVerticalMirror = roi.m_bMirrorVert;
		m_nRotationIndex = roi.m_nRotation;
		
		// 更新Gamma显示
		{
			double dGamma = 1.0;
			double dMin = 0.0;
			double dMax = 10.0;
			CString strVal;

			m_bGammaEnable = roi.m_bGammaCorrectionEnable;
			dGamma = roi.m_dGamma;

			m_sliderGamma.EnableWindow(m_bGammaEnable);
			m_sliderGamma.SetRange(ceil(dMin/0.1), floor(dMax/0.1), TRUE);
			m_sliderGamma.SetPos( (int) ((dGamma+0.05) / 0.1) );
			strVal.Format(_T("%.2f"), dGamma);
			GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);
		}

		m_dCrossX = roi.m_dCrossX;
		m_dCrossY = roi.m_dCrossY;
		m_strCrossX.Format(_T("%.01f"),m_dCrossX);
		m_strCrossY.Format(_T("%.01f"),m_dCrossY);
		m_bCrossVisible = roi.m_bCrossVisible;

		m_pParent->m_vpGuiCross.at(0)->SetCenterXYWidthHeight(m_dCrossX, m_dCrossY, 10000, 10000);
		m_pParent->m_vpGuiCross.at(0)->SetVisible(m_bCrossVisible);

		if (m_bCrossVisible)
		{
			GetDlgItem(IDC_EDIT_CROSS_X)->EnableWindow(m_bEnableOtherSet);
			GetDlgItem(IDC_EDIT_CROSS_Y)->EnableWindow(m_bEnableOtherSet);
			GetDlgItem(IDC_BTN_SET_CORSS_IMAGE_CENTER)->EnableWindow(m_bEnableOtherSet);
		}
		else
		{	
			GetDlgItem(IDC_EDIT_CROSS_X)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CROSS_Y)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_CORSS_IMAGE_CENTER)->EnableWindow(FALSE);
		}

		CamPixelFormat eFormat = eCamColorUnknown;
		m_pVisionASM->GetPosCameraPixelFormat(m_nCameraIndex,eFormat);
		if(eFormat == eCamBayer_BG8 || eFormat == eCamBayer_GB8 ||eFormat == eCamBayer_RG8 ||eFormat == eCamBayer_GR8 || eFormat == eCamRGBPacked)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(m_bEnableColorSet);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(m_bEnableColorSet);

			GetDlgItem(IDC_STATIC_GRP_WHITE_BALANCE)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_STATIC_RATIO_R)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_STATIC_RATIO_G)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_STATIC_RATIO_B)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_EDIT_RATIO_R)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_EDIT_RATIO_G)->ShowWindow(m_bShowColorSet);
			GetDlgItem(IDC_EDIT_RATIO_B)->ShowWindow(m_bShowColorSet);

			m_bCamPixelFormat = true;
		}
		else if(eFormat == eCamGray || eFormat == eCamColorUnknown)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);

			GetDlgItem(IDC_STATIC_GRP_WHITE_BALANCE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_RATIO_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_RATIO_G)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_RATIO_B)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_RATIO_R)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_RATIO_G)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_RATIO_B)->ShowWindow(SW_HIDE);

			m_bCamPixelFormat = false;
		}

		m_pVisionASM->UpdatePosCameraStatus(m_nCameraIndex);
		UpdateData(FALSE);

		// 设置当前相机图像显示GUIDisplay
		m_pVisionASM->SetPosDisplay(m_nCameraIndex, m_pGUIDisplay);

		// 更新按钮状态
// 		if (m_pVisionASM->IsCameraGrabbing(m_nCameraIndex))
// 		{
// 			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
// 			GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
// 			GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(TRUE);
// 		}


		EndWaitCursor();
	}
}

void CDlgSysImgRoiParam::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

	m_pVisionASM = pVisionASM;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplay.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplay.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}
}

vcBaseVisionAlign* CDlgSysImgRoiParam::GetVisionASMRef() const
{
	return m_pVisionASM;
}

void CDlgSysImgRoiParam::SetCameraDisplay( scGuiDisplay *pDisplay )
{
	m_pGUIDisplay = pDisplay;
}

void CDlgSysImgRoiParam::UpdateDialogLanguage()
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
		
		if(_T("IDD_SYS_IMGROI_PARAM") == strDlgID)
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

BOOL CDlgSysImgRoiParam::IsCameraGrabing() const
{
	if (m_pVisionASM == NULL)
	{
		return FALSE;
	}
	return m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex);
}

void CDlgSysImgRoiParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysImgRoiParam)
	DDX_Control(pDX, IDC_BTN_SET_CUSTOM, m_btnSetCustom);
	DDX_Control(pDX, IDC_BTN_SAVE_IMAGE, m_btnSaveImage);
	DDX_Control(pDX, IDC_BTN_LIVE_GRAB, m_btnGrabImage);
	DDX_Control(pDX, IDC_SLIDER_SHUTTER, m_sliderShutter);
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_sliderGain);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_sliderGamma);
	DDX_Control(pDX, IDC_SLIDER_FRAMERATE, m_sliderFramerate);
	DDX_Check(pDX, IDC_CHECK_HORIZONTAL_MIRROR, m_bHorizontalMirror);
	DDX_Check(pDX, IDC_CHECK_VERTICAL_MIRROR, m_bVerticalMirror);
	DDX_Check(pDX, IDC_CHECK_GAMMA, m_bGammaEnable);
	DDX_Text(pDX, IDC_EDIT_CUSTOM_HEIGHT, m_nHeightCustom);
	DDX_Text(pDX, IDC_EDIT_CUSTOM_WIDTH, m_nWidthCustom);
	DDX_Text(pDX, IDC_EDIT_START_X, m_nStartX);
	DDX_Text(pDX, IDC_EDIT_START_Y, m_nStartY);
	DDX_Radio(pDX, IDC_RADIO_CAMERA0, m_nCameraIndex);
	DDX_Radio(pDX, IDC_RADIO_CAMERA_MODE0, m_nVideoMode);
	DDX_Radio(pDX, IDC_RADIO_ROTATION_0, m_nRotationIndex);
	DDX_Text(pDX, IDC_EDIT_CROSS_X, m_strCrossX);
	DDX_Text(pDX, IDC_EDIT_CROSS_Y, m_strCrossY);
	// 	DDX_Text(pDX, IDC_EDIT_CROSS_X, m_dCrossX);
	// 	DDX_Text(pDX, IDC_EDIT_CROSS_Y, m_dCrossY);
	DDX_Check(pDX, IDC_CHECK_CROSS_VISIBLE, m_bCrossVisible);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_MANUAL_MEASURE, m_btnManualMeasure);
}


BEGIN_MESSAGE_MAP(CDlgSysImgRoiParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSysImgRoiParam)
	ON_BN_CLICKED(IDC_BTN_LIVE_GRAB, OnBtnLiveGrab)
	ON_BN_CLICKED(IDC_BTN_SAVE_IMAGE, OnBtnSaveImage)
	ON_BN_CLICKED(IDC_RADIO_CAMERA0, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_ROTATION_0, OnRotationChange)
	ON_BN_CLICKED(IDC_CHECK_HORIZONTAL_MIRROR, OnMirrorChange)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_CROSS_X, OnKillfocusEditCrossX)
	ON_EN_KILLFOCUS(IDC_EDIT_CROSS_Y, OnKillfocusEditCrossY)
	ON_BN_CLICKED(IDC_BTN_SET_CORSS_IMAGE_CENTER, OnBtnSetCorssImageCenter)
	ON_EN_SETFOCUS(IDC_EDIT_CROSS_X, OnSetfocusEditCrossX)
	ON_EN_SETFOCUS(IDC_EDIT_CROSS_Y, OnSetfocusEditCrossY)
	ON_BN_CLICKED(IDC_RADIO_CAMERA1, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA2, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA3, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA4, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA5, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA6, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_CAMERA7, OnCameraChange)
	ON_BN_CLICKED(IDC_RADIO_ROTATION_180, OnRotationChange)
	ON_BN_CLICKED(IDC_RADIO_ROTATION_270, OnRotationChange)
	ON_BN_CLICKED(IDC_RADIO_ROTATION_90, OnRotationChange)
	ON_BN_CLICKED(IDC_CHECK_VERTICAL_MIRROR, OnMirrorChange)
	ON_BN_CLICKED(IDC_CHECK_CROSS_VISIBLE, OnCheckCrossVisible)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SHUTTER_DOWN, &CDlgSysImgRoiParam::OnBnClickedButtonShutterDown)
	ON_BN_CLICKED(IDC_BUTTON_SHUTTER_UP, &CDlgSysImgRoiParam::OnBnClickedButtonShutterUp)
	ON_BN_CLICKED(IDC_BUTTON_GAIN_DOWN, &CDlgSysImgRoiParam::OnBnClickedButtonGainDown)
	ON_BN_CLICKED(IDC_BUTTON_GAIN_UP, &CDlgSysImgRoiParam::OnBnClickedButtonGainUp)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_MEASURE, &CDlgSysImgRoiParam::OnBnClickedButtonManualMeasure)
	ON_BN_CLICKED(IDC_CHECK_GAMMA, &CDlgSysImgRoiParam::OnBnClickedCheckGamma)
	ON_BN_CLICKED(IDC_BTN_AUTO_BALANCE, &CDlgSysImgRoiParam::OnBnClickedBtnAutoBalance)
	ON_BN_CLICKED(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT, &CDlgSysImgRoiParam::OnBnClickedBtnSetWhiteBalanceRatioDefault)
	ON_BN_CLICKED(IDC_BTN_LIGHT_CTRL_SET, &CDlgSysImgRoiParam::OnBnClickedBtnLightCtrlSet)
	ON_EN_SETFOCUS(IDC_EDIT_INPUT_SHUTTER_VAL, &CDlgSysImgRoiParam::OnEnSetfocusEditInputShutterVal)
	ON_EN_SETFOCUS(IDC_EDIT_INPUT_GAIN_VAL, &CDlgSysImgRoiParam::OnEnSetfocusEditInputGainVal)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT_SHUTTER_VAL, &CDlgSysImgRoiParam::OnEnKillfocusEditInputShutterVal)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT_GAIN_VAL, &CDlgSysImgRoiParam::OnEnKillfocusEditInputGainVal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysImgRoiParam message handlers

LRESULT CDlgSysImgRoiParam::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);
			
// 			int nIndex = (int)wParam;
// 			if (m_pParent->m_pGuiDisplay!=NULL && m_pParent->m_pGuiDisplay->GetSafeHwnd())
// 			{
// 				if (m_pParent->m_pGuiDisplay->HasImage())
// 				{
// 					m_pParent->m_vpGuiCross.at(0)->SetVisible(TRUE);
// 				}
// 				else
// 				{
// 					m_pParent->m_vpGuiCross.at(0)->SetVisible(FALSE);
// 				}
// 			}
		}
		break;
	case WM_UPDATE_STATUSBAR_INFO:
		{
			m_pVisionASM->GetStatusBarInfo(m_pParent->m_bStatusBarFlag, m_pParent->m_strStatusBarInfo1, m_pParent->m_strStatusBarInfo2);
			m_pParent->UpdateStatusBar();
		}
		break;
	case WM_CAMERA_REMOVAL:
		{
			scGuiDisplay* pDisplay = m_pVisionASM->GetCameraDisplay((int)wParam);
			if ((pDisplay != NULL) && ::IsWindow(pDisplay->GetSafeHwnd()))
			{
				pDisplay->ClearScreen();
			}
			UpdateBtnDisplay();

			m_pVisionASM->GetStatusBarInfo(m_pParent->m_bStatusBarFlag, m_pParent->m_strStatusBarInfo1, m_pParent->m_strStatusBarInfo2);
			m_pParent->UpdateStatusBar();
		}
		break;
	case WM_CAMERA_ARRIVAL:
		{
			UpdateBtnDisplay();

			m_pVisionASM->GetStatusBarInfo(m_pParent->m_bStatusBarFlag, m_pParent->m_strStatusBarInfo1, m_pParent->m_strStatusBarInfo2);
			m_pParent->UpdateStatusBar();
		}
		break;
	default:
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgSysImgRoiParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dShutterOld = 0;

	UpdateDialogLanguage();
	InitDlgItem();
	UpdateDlgData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSysImgRoiParam::OnBtnLiveGrab() 
{
	UpdateData(TRUE);
	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		if (!m_pVisionASM->FreezePosCamera(m_nCameraIndex))
		{
			//AfxMessageBox(_T("停止采集失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_STOP_GRAB));
			return;
		}
		m_pVisionASM->SetPosDisplay(m_nCameraIndex, NULL);
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_GRAB_IMAGE));	//_T("采集图像")
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(m_bEnableOtherSet);
		GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(m_bEnableOtherSet);
		CString strLog;
		strLog.Format(_T("点击[停止采集]停止采集图像\n"));
		g_opLogFile.SaveLogText(strLog,3);

		CamPixelFormat eFormat = eCamColorUnknown;
		m_pVisionASM->GetPosCameraPixelFormat(m_nCameraIndex,eFormat);
		if(eFormat == eCamBayer_BG8 || eFormat == eCamBayer_GB8 ||eFormat == eCamBayer_RG8 ||eFormat == eCamBayer_GR8)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		}
		else if (eFormat == eCamRGBPacked)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		}
		else if(eFormat == eCamGray || eFormat == eCamColorUnknown)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		}
	}
	else
	{
		if (!m_pVisionASM->GrabPosCamera(m_nCameraIndex))
		{
			//AfxMessageBox(_T("采集失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_START_GRAB));
			return;
		}
		m_pVisionASM->SetPosDisplay(m_nCameraIndex, m_pGUIDisplay);
		GetDlgItem(IDC_BTN_LIVE_GRAB)->SetWindowText(GetSysInfoString(m_psaSysInfoStrings,IDS_BTN_STOP_GRAB));	//_T("停止采集")
		GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(FALSE);
		CString strLog;
		strLog.Format(_T("点击[连续采集]开始采集图像\n"));
		g_opLogFile.SaveLogText(strLog,3);
		CamPixelFormat eFormat = eCamColorUnknown;
		m_pVisionASM->GetPosCameraPixelFormat(m_nCameraIndex,eFormat);
		if(eFormat == eCamBayer_BG8 || eFormat == eCamBayer_GB8 ||eFormat == eCamBayer_RG8 ||eFormat == eCamBayer_GR8 || eFormat == eCamRGBPacked)
		{

			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(m_bEnableColorSet);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(m_bEnableColorSet);
		}
		else if(eFormat == eCamGray || eFormat == eCamColorUnknown)
		{
			GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		}
	}
}

void CDlgSysImgRoiParam::OnBtnSaveImage() 
{
	if ((m_pGUIDisplay == NULL) || (m_pGUIDisplay->GetImageCP() == NULL))
	{
		//AfxMessageBox(_T("图像数据为空！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_EMPTY_IMAGE));
		return;
	}

	CFileDialog dlgSave(FALSE, _T("*.bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
						GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FILTER_BMP_FILE), this);	//_T("位图文件（*.bmp）|*.bmp||")
	if (dlgSave.DoModal() == IDOK)
	{
		CString strPath = dlgSave.GetPathName();
		m_pGUIDisplay->SaveImage(strPath);
	}
	CString strLog;
	strLog.Format(_T("点击[图像保存]图像保存成功\n"));
	g_opLogFile.SaveLogText(strLog,3);
}

void CDlgSysImgRoiParam::OnCameraChange() 
{
	UpdateData(TRUE);
	if (m_nCameraIndex == m_nPrevCameraIndex)
	{
		return;
	}

	// 更新图像显示
// 	if (m_pVisionASM->IsCameraGrabbing(m_nPrevCameraIndex))
// 	{
// 		m_pVisionASM->FreezeCamera(m_nPrevCameraIndex);
// 		m_pVisionASM->SetCameraDisplay(m_nPrevCameraIndex, NULL);
// 		m_pVisionASM->SetCameraDisplay(m_nCameraIndex, m_pGUIDisplay);
// 		m_pVisionASM->GrabCamera(m_nCameraIndex);
// 	}
// 	else
// 	{
// 		m_pVisionASM->SetCameraDisplay(m_nPrevCameraIndex, NULL);
// 		m_pVisionASM->SetCameraDisplay(m_nCameraIndex, m_pGUIDisplay);
// 		scImageGray imageNULL;
// 		m_pGUIDisplay->SetImage(imageNULL);
// 	}

	m_pVisionASM->FreezePosCamera(m_nPrevCameraIndex);
	m_pVisionASM->SetPosDisplay(m_nPrevCameraIndex, NULL);
//	m_pGUIDisplay->ClearScreen();
	m_pGUIDisplay->RemoveImage();
	m_pParent->m_vpGuiCross.at(0)->SetVisible(FALSE);


	if (m_pVisionASM->IsPosCameraOnline(m_nCameraIndex) && !m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		m_pVisionASM->SetPosDisplay(m_nCameraIndex, m_pGUIDisplay);
		m_pVisionASM->GrabPosCamera(m_nCameraIndex);
	}
	CString strLog;
	strLog.Format(_T("修改[位置%d]为[位置%d]\n"),m_nPrevCameraIndex,m_nCameraIndex);
	g_opLogFile.SaveLogText(strLog,3);

	m_nPrevCameraIndex = m_nCameraIndex;

	UpdateDlgData(FALSE);

}

// void CDlgSysImgRoiParam::OnVideoModeChange() 
// {
// 	int nOldVideoMode = m_nVideoMode;
// 	UpdateData(TRUE);
// 	if (m_nVideoMode == nOldVideoMode)
// 	{
// 		return;
// 	}
// 	if ((m_nVideoMode == 5) && (m_pVisionASM->IsVideoModeSupported(m_nCameraIndex, 5)))
// 	{
// 		GetDlgItem(IDC_EDIT_START_X)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_START_Y)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_CUSTOM_WIDTH)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_EDIT_CUSTOM_HEIGHT)->EnableWindow(TRUE);
// 		GetDlgItem(IDC_BTN_SET_CUSTOM)->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_EDIT_START_X)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_START_Y)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_CUSTOM_WIDTH)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_EDIT_CUSTOM_HEIGHT)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_BTN_SET_CUSTOM)->EnableWindow(FALSE);
// 
// 	}
// 
// 	m_pVisionASM->SetCameraVideoMode(m_nCameraIndex, m_nVideoMode, 
// 		sc2Vector(m_nStartX, m_nStartY), sc2Vector(m_nWidthCustom, m_nHeightCustom));
// 	m_pVisionASM->UpdateCameraVideo(m_nCameraIndex);
// 
// 	UpdateFramerateShutterGainDisplay();
// }

void CDlgSysImgRoiParam::OnRotationChange() 
{
	int nRotationOld;
	nRotationOld = m_nRotationIndex;
	UpdateData(TRUE);
	ImageRoi roi;
	m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
	roi.m_nRotation = m_nRotationIndex;
	m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);
	if (nRotationOld != m_nRotationIndex)
	{
		CString strLog;
		CString strVal[]={_T("0度"), _T("90度"),_T("180度"), _T("270度")};
		strLog.Format(_T("修改[旋转]由%s到%s\n"),strVal[nRotationOld],strVal[m_nRotationIndex]);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysImgRoiParam::OnMirrorChange() 
{
	BOOL bHoizontalMirrorOld,bVerticalMirrorOld;
	bHoizontalMirrorOld = m_bHorizontalMirror;
	bVerticalMirrorOld = m_bVerticalMirror;
	UpdateData(TRUE);
	ImageRoi roi;
	m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
	roi.m_bMirrorHoriz = m_bHorizontalMirror;
	roi.m_bMirrorVert = m_bVerticalMirror;
	m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);
	CString strLog,strValue;
	if (bHoizontalMirrorOld != m_bHorizontalMirror)
	{
		strValue = m_bHorizontalMirror ? _T("选中"):_T("未选中");
		strLog.Format(_T("修改[水平镜像]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
	if (bVerticalMirrorOld != m_bVerticalMirror)
	{
		strValue = m_bVerticalMirror ? _T("选中"):_T("未选中");
		strLog.Format(_T("修改[垂直镜像]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}

void CDlgSysImgRoiParam::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString strVal;
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_FRAMERATE:	
		{
// 			// 设置相机帧率
// 			double dFramerate = (double)(m_sliderFramerate.GetPos())*0.1;
// 			strVal.Format(_T("%.1fHz"), dFramerate);
// 			GetDlgItem(IDC_STATIC_FRAMERATE_VAL)->SetWindowText(strVal);
// 			m_pVisionASM->SetCameraFrameRate(m_nCameraIndex, dFramerate);
// 			m_pVisionASM->UpdateCameraFrameRate(m_nCameraIndex);
		}
		break;
	case IDC_SLIDER_SHUTTER:
		{
			// 设置相机的曝光时间
			double dShutter = (double)(m_sliderShutter.GetPos())*0.01;
			strVal.Format(_T("%.2fms"), dShutter);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);
			strVal.Format(_T("%.2f"), dShutter);
			GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(strVal);
			m_pVisionASM->SetPosCameraShutter(m_nCameraIndex, dShutter);
			m_pVisionASM->UpdatePosCameraShutter(m_nCameraIndex);
			if (m_dShutterOld != dShutter)
			{
				CString strLog;
				strLog.Format(_T("修改位置%d[曝光]由%lf到%lf"),m_nCameraIndex,m_dShutterOld,dShutter);
				g_opLogFile.SaveLogText(strLog,3);
			}
			m_dShutterOld = dShutter;
		}
		break;
	case IDC_SLIDER_GAIN:
		{
			// 设置相机的增益
			double dGain = (double)(m_sliderGain.GetPos())*0.01;
			strVal.Format(_T("%.2fdb"), dGain);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strVal);
			strVal.Format(_T("%.2f"), dGain);
			GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(strVal);
			m_pVisionASM->SetPosCameraGain(m_nCameraIndex, dGain);
			m_pVisionASM->UpdatePosCameraGain(m_nCameraIndex);
			if (m_dGainOld != dGain)
			{
				CString strLog;
				strLog.Format(_T("修改位置%d[增益]由%lf到%lf"),m_nCameraIndex,m_dGainOld,dGain);
				g_opLogFile.SaveLogText(strLog,3);
			}
			m_dGainOld = dGain;
		}
		break;
	case IDC_SLIDER_GAMMA:
		{
// 			// 设置相机的Gamma
// 			double dGamma = (double)(m_sliderGamma.GetPos())*0.1;
// 			strVal.Format(_T("%.2f"), dGamma);
// 			GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);
// 			m_pVisionASM->SetPosCameraGamma(m_nCameraIndex, m_bGammaEnable, dGamma);
// 			m_pVisionASM->UpdatePosCameraGamma(m_nCameraIndex);
// 			if (m_dGammaOld != dGamma)
// 			{
// 				CString strLog;
// 				strLog.Format(_T("修改位置%d[Gamma]由%lf到%lf"),m_nCameraIndex,m_dGammaOld,dGamma);
// 				g_opLogFile.SaveLogText(strLog,3);
// 			}
// 			m_dGammaOld = dGamma;

			{
				// 设置相机的Gamma
				double dGamma = (double)(m_sliderGamma.GetPos())*0.1;
				strVal.Format(_T("%.2f"), dGamma);
				GetDlgItem(IDC_STATIC_GAMMA_VAL)->SetWindowText(strVal);
				ImageRoi roi;
				m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
				roi.m_dGamma = dGamma;
				m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);
				if (m_dGammaOld != dGamma)
				{
					CString strLog;
					strLog.Format(_T("修改位置%d[Gamma]由%lf到%lf"),m_nCameraIndex,m_dGammaOld,dGamma);
					g_opLogFile.SaveLogText(strLog,3);
				}
				m_dGammaOld = dGamma;
			}
		}
		break;
	default:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgSysImgRoiParam::DestroyWindow() 
{
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->FreezeAllCameras();
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}
	
	return CDialog::DestroyWindow();
}

// void CDlgSysImgRoiParam::OnSetfocusEditStartX() 
// {
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_START_X)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_START_X, strInput);
// 		
// 		//if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
// 		//{
// 		//	SetDlgItemText(IDC_EDIT_START_X, strOutput);
// 		//}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditStartX();
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnSetfocusEditStartY() 
// {
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_START_Y)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_START_Y, strInput);
// 		
// 		//if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
// 		//{
// 		//	SetDlgItemText(IDC_EDIT_START_Y, strOutput);
// 		//}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditStartY();
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnSetfocusEditCustomWidth() 
// {
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_CUSTOM_WIDTH)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_CUSTOM_WIDTH, strInput);
// 		
// 		//if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
// 		//{
// 		//	SetDlgItemText(IDC_EDIT_CUSTOM_WIDTH, strOutput);
// 		//}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditCustomWidth();
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnSetfocusEditCustomHeight() 
// {
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
//  		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_CUSTOM_HEIGHT)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_CUSTOM_HEIGHT, strInput);
// 		
// 		//if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
// 		//{
// 		//	SetDlgItemText(IDC_EDIT_CUSTOM_HEIGHT, strOutput);
// 		//}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditCustomHeight();
// 	}	
// }
// 
// void CDlgSysImgRoiParam::OnBtnSetCustom() 
// {
// 	UpdateData(TRUE);
// 	m_pVisionASM->SetCameraVideoMode(m_nCameraIndex, m_nVideoMode, 
// 		sc2Vector(m_nStartX, m_nStartY), sc2Vector(m_nWidthCustom, m_nHeightCustom));
// 	if (!m_pVisionASM->UpdateCameraVideo(m_nCameraIndex))
// 	{
// 		//AfxMessageBox(_T("视频模式设置失败！"));
// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_CAMERA_INIT_FAILED));
// 	}
// 
// 	UpdateFramerateShutterGainDisplay();
// }
// 
// void CDlgSysImgRoiParam::OnKillfocusEditStartX() 
// {
// 	if (m_bKillFocusEnabled)
// 	{
// 		UpdateData(TRUE);
// 		PTFormat7QueryInfo qInfo;
// 		if (m_pVisionASM->QueryFormat7Info(m_nCameraIndex, qInfo))
// 		{
// 			m_nStartX = (m_nStartX < 0) ? 0 : m_nStartX;
// 			m_nStartX = (m_nStartX > (qInfo.m_uiMaxWidth-m_nWidthCustom)) ? (qInfo.m_uiMaxWidth-m_nWidthCustom) : m_nStartX;
// 			m_nStartX = ((m_nStartX % qInfo.m_uiUnitStartX) == 0) ? m_nStartX : (m_nStartX - m_nStartX % qInfo.m_uiUnitStartX);
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("相机未连接！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CAMERA_NOT_CONNECTED));
// 		}
// 		UpdateData(FALSE);
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnKillfocusEditStartY() 
// {
// 	if (m_bKillFocusEnabled)
// 	{
// 		UpdateData(TRUE);
// 		PTFormat7QueryInfo qInfo;
// 		if (m_pVisionASM->QueryFormat7Info(m_nCameraIndex, qInfo))
// 		{
// 			m_nStartY = (m_nStartY < 0) ? 0 : m_nStartY;
// 			m_nStartY = (m_nStartY > (qInfo.m_uiMaxHeight-m_nHeightCustom)) ? (qInfo.m_uiMaxHeight-m_nHeightCustom) : m_nStartY;
// 			m_nStartY = ((m_nStartY % qInfo.m_uiUnitStartY) == 0) ? m_nStartY : (m_nStartY - m_nStartY % qInfo.m_uiUnitStartY);
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("相机未连接！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CAMERA_NOT_CONNECTED));
// 		}
// 		UpdateData(FALSE);
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnKillfocusEditCustomWidth() 
// {
// 	if (m_bKillFocusEnabled)
// 	{
// 		UpdateData(TRUE);
// 		PTFormat7QueryInfo qInfo;
// 		if (m_pVisionASM->QueryFormat7Info(m_nCameraIndex, qInfo))
// 		{
// 			m_nWidthCustom = (m_nWidthCustom < qInfo.m_uiUnitWidth) ? qInfo.m_uiUnitWidth : m_nWidthCustom;
// 			m_nWidthCustom = (m_nWidthCustom > (qInfo.m_uiMaxWidth-m_nStartX)) ? (qInfo.m_uiMaxWidth-m_nStartX) : m_nWidthCustom;
// 			m_nWidthCustom = ((m_nWidthCustom % qInfo.m_uiUnitWidth) == 0) ? m_nWidthCustom : (m_nWidthCustom - m_nWidthCustom % qInfo.m_uiUnitWidth);
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("相机未连接！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CAMERA_NOT_CONNECTED));
// 		}
// 		UpdateData(FALSE);
// 	}
// }
// 
// void CDlgSysImgRoiParam::OnKillfocusEditCustomHeight() 
// {
// 	if (m_bKillFocusEnabled)
// 	{
// 		UpdateData(TRUE);
// 		PTFormat7QueryInfo qInfo;
// 		if (m_pVisionASM->QueryFormat7Info(m_nCameraIndex, qInfo))
// 		{
// 			m_nHeightCustom = (m_nHeightCustom < qInfo.m_uiUnitHeight) ? qInfo.m_uiUnitHeight : m_nHeightCustom;
// 			m_nHeightCustom = (m_nHeightCustom > (qInfo.m_uiMaxHeight-m_nStartY)) ? (qInfo.m_uiMaxHeight-m_nStartY) : m_nHeightCustom;
// 			m_nHeightCustom = ((m_nHeightCustom % qInfo.m_uiUnitHeight) == 0) ? m_nHeightCustom : (m_nHeightCustom - m_nHeightCustom % qInfo.m_uiUnitHeight);
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("相机未连接！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CAMERA_NOT_CONNECTED));
// 		}
// 		UpdateData(FALSE);
// 	}
// }

BOOL CDlgSysImgRoiParam::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysImgRoiParam::OnSetfocusEditCrossX() 
{
	// TODO: Add your control notification handler code here
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CROSS_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CROSS_X, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CROSS_X, strOutput);
			CString strLog;
			strLog.Format(_T("修改[十字线][X]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditCrossX();
	}	
}

void CDlgSysImgRoiParam::OnSetfocusEditCrossY() 
{
	// TODO: Add your control notification handler code here
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CROSS_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CROSS_Y, strInput);
		
		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CROSS_Y, strOutput);
			CString strLog;
			strLog.Format(_T("修改[十字线][Y]由%s到%s\n"),strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditCrossY();
	}
}

void CDlgSysImgRoiParam::OnKillfocusEditCrossX() 
{
	// TODO: Add your control notification handler code here
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CROSS_X, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CROSS_X, m_strCrossX);
		}
		else
		{
			m_strCrossX.Format(_T("%.01f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CROSS_X, m_strCrossX);

			m_dCrossX = atof(m_strCrossX);
			m_pParent->m_vpGuiCross.at(0)->SetCenterXYWidthHeight(m_dCrossX, m_dCrossY, 10000, 10000);
			ImageRoi roi;
			m_pVisionASM->GetImageROIParam(m_nCameraIndex, roi);
			roi.m_dCrossX = m_dCrossX;
			m_pVisionASM->SetImageROIParam(m_nCameraIndex, roi);
		}
	}	
}

void CDlgSysImgRoiParam::OnKillfocusEditCrossY() 
{
	// TODO: Add your control notification handler code here
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CROSS_Y, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CROSS_Y, m_strCrossY);
		}
		else
		{
			m_strCrossY.Format(_T("%.01f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CROSS_Y, m_strCrossY);

			m_dCrossY = atof(m_strCrossY);
			m_pParent->m_vpGuiCross.at(0)->SetCenterXYWidthHeight(m_dCrossX, m_dCrossY, 10000, 10000);
			ImageRoi roi;
			m_pVisionASM->GetImageROIParam(m_nCameraIndex, roi);
			roi.m_dCrossY = m_dCrossY;
			m_pVisionASM->SetImageROIParam(m_nCameraIndex, roi);
		}
	}	
}

void CDlgSysImgRoiParam::OnBtnSetCorssImageCenter() 
{
	// TODO: Add your control notification handler code here
	CString strCrossXOld,strCrossYOld;
	strCrossXOld = m_strCrossX;
	strCrossYOld = m_strCrossY;
	if (m_pGUIDisplay!=NULL && m_pGUIDisplay->GetSafeHwnd())
	{
		if (m_pGUIDisplay->HasImage())
		{
			m_dCrossX = m_pGUIDisplay->GetImageWidth()/2;
			m_dCrossY = m_pGUIDisplay->GetImageHeight()/2;
			m_strCrossX.Format(_T("%.01f"),m_dCrossX);
			m_strCrossY.Format(_T("%.01f"),m_dCrossY);

			ImageRoi roi;
			m_pVisionASM->GetImageROIParam(m_nCameraIndex, roi);
			roi.m_dCrossX = m_dCrossX;
			roi.m_dCrossY = m_dCrossY;
			m_pVisionASM->SetImageROIParam(m_nCameraIndex, roi);

			m_pParent->m_vpGuiCross.at(0)->SetCenterXYWidthHeight(m_dCrossX, m_dCrossY, 10000, 10000);

			UpdateData(FALSE);
		}
	}
	CString strLog;
	strLog.Format(_T("[十字线]设置为图像中心\n"));
	g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("修改[十字线][X]由%s到%s\n"),strCrossXOld,m_strCrossX);
	g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("修改[十字线][Y]由%s到%s\n"),strCrossYOld,m_strCrossY);
	g_opLogFile.SaveLogText(strLog,3);
	
}


void CDlgSysImgRoiParam::OnCheckCrossVisible() 
{
	// TODO: Add your control notification handler code here
	BOOL bCrossVisibleOld;
	bCrossVisibleOld = m_bCrossVisible;
	UpdateData(TRUE);
	ImageRoi roi;
	m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
	roi.m_bCrossVisible = m_bCrossVisible;
	m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);

	m_pParent->m_vpGuiCross.at(0)->SetVisible(m_bCrossVisible);

	if (m_bCrossVisible)
	{
		GetDlgItem(IDC_EDIT_CROSS_X)->EnableWindow(m_bEnableOtherSet);
		GetDlgItem(IDC_EDIT_CROSS_Y)->EnableWindow(m_bEnableOtherSet);
		GetDlgItem(IDC_BTN_SET_CORSS_IMAGE_CENTER)->EnableWindow(m_bEnableOtherSet);
	}
	else
	{	
		GetDlgItem(IDC_EDIT_CROSS_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CROSS_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CORSS_IMAGE_CENTER)->EnableWindow(FALSE);
	}
	if (bCrossVisibleOld != m_bCrossVisible)
	{
		CString strLog,strValue;
		strValue = m_bCrossVisible ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[启用]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


BOOL CDlgSysImgRoiParam::IsStringNumerical( LPCTSTR lpszVal )
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

void CDlgSysImgRoiParam::OnBnClickedButtonManualMeasure()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgManualMeasure dlgManualMeasure(this);
	dlgManualMeasure.SetSysInfoStringsRef(m_psaSysInfoStrings);
	dlgManualMeasure.EnableSysKeyboard(m_bSysKeyboardEnabled);
	//dlgManualMeasure.SetCameraIndex(m_nCameraIndex);
	//dlgManualMeasure.SetCameraDisplay(m_pGUIDisplay);
	dlgManualMeasure.SetVisionASMRef(m_pVisionASM);
	//dlgManualMeasure.SetVisionASMRef();
    CString strLog;
    strLog.Format(_T("点击[手动测量]进入[手动测量界面]\n"));
    g_opLogFile.SaveLogText(strLog,3);
	if (dlgManualMeasure.DoModal() == IDOK)
	{
	}
}


void CDlgSysImgRoiParam::OnBnClickedButtonShutterDown()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int nMin = m_sliderShutter.GetRangeMin();
	int nChangedShutterPos = m_sliderShutter.GetPos() - 1;
	if (nChangedShutterPos < nMin)
	{
		nChangedShutterPos = nMin;
	}
	m_sliderShutter.SetPos(nChangedShutterPos);
	double dShutter = (double)(nChangedShutterPos * 0.01);
	CString strTemp;
	strTemp.Format(_T("%.2fms"), dShutter);
	GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), dShutter);
	GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(strTemp);
	m_pVisionASM->SetCameraShutter(m_nCameraIndex, dShutter);
	m_pVisionASM->UpdatePosCameraShutter(m_nCameraIndex);
	UpdateData(FALSE);
}


void CDlgSysImgRoiParam::OnBnClickedButtonShutterUp()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int nMax = m_sliderShutter.GetRangeMax();
	int nChangedShutterPos = m_sliderShutter.GetPos() + 1;
	if (nChangedShutterPos > nMax)
	{
		nChangedShutterPos = nMax;
	}
	m_sliderShutter.SetPos(nChangedShutterPos);
	double dShutter = (double)(nChangedShutterPos * 0.01);
	CString strTemp;
	strTemp.Format(_T("%.2fms"), dShutter);
	GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), dShutter);
	GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(strTemp);
	m_pVisionASM->SetCameraShutter(m_nCameraIndex, dShutter);
	m_pVisionASM->UpdatePosCameraShutter(m_nCameraIndex);
	UpdateData(FALSE);
}


void CDlgSysImgRoiParam::OnBnClickedButtonGainDown()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int nMin = m_sliderGain.GetRangeMin();
	int nChangedGainPos = m_sliderGain.GetPos() - 1;
	if (nChangedGainPos < nMin)
	{
		nChangedGainPos = nMin;
	}
	m_sliderGain.SetPos(nChangedGainPos);
	double dGain = (double)(nChangedGainPos * 0.01);
	CString strTemp;
	strTemp.Format(_T("%.2fdb"), dGain);
	GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), dGain);
	GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(strTemp);
	m_pVisionASM->SetCameraGain(m_nCameraIndex, dGain);
	m_pVisionASM->UpdatePosCameraGain(m_nCameraIndex);
	UpdateData(FALSE);
}


void CDlgSysImgRoiParam::OnBnClickedButtonGainUp()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int nMax = m_sliderGain.GetRangeMax();
	int nChangedGainPos = m_sliderGain.GetPos() + 1;
	if (nChangedGainPos > nMax)
	{
		nChangedGainPos = nMax;
	}
	m_sliderGain.SetPos(nChangedGainPos);
	double dGain = (double)(nChangedGainPos * 0.01);
	CString strTemp;
	strTemp.Format(_T("%.2fdb"), dGain);
	GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strTemp);
	strTemp.Format(_T("%.2f"), dGain);
	GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(strTemp);
	m_pVisionASM->SetCameraGain(m_nCameraIndex, dGain);
	m_pVisionASM->UpdatePosCameraGain(m_nCameraIndex);
	UpdateData(FALSE);
}


void CDlgSysImgRoiParam::OnBnClickedCheckGamma()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bGammaEnableOld;
	bGammaEnableOld = m_bGammaEnable;
	UpdateData(TRUE);

// 	// 设置相机的Gamma
// 	CString strVal;
// 	double dGamma = (double)(m_sliderGamma.GetPos())*0.1;
// 	m_pVisionASM->SetPosCameraGamma(m_nCameraIndex, m_bGammaEnable, dGamma);
// 	m_pVisionASM->UpdatePosCameraGamma(m_nCameraIndex);
// 	if (m_dGammaOld != dGamma)
// 	{
// 		CString strLog;
// 		strLog.Format(_T("修改位置%d[Gamma]由%lf到%lf"),m_nCameraIndex,m_dGammaOld,dGamma);
// 		g_opLogFile.SaveLogText(strLog,3);
// 	}
// 	m_dGammaOld = dGamma;
// 
// 	CString strLog,strValue;
// 	if (bGammaEnableOld != m_bGammaEnable)
// 	{
// 		strValue = m_bGammaEnable ? _T("选中"):_T("未选中");
// 		strLog.Format(_T("修改[Gamma启用]为%s\n"),strValue);
// 		g_opLogFile.SaveLogText(strLog,3);
// 	}

	m_sliderGamma.EnableWindow(m_bGammaEnable);

	// 设置相机的Gamma
	CString strVal;
	double dGamma = (double)(m_sliderGamma.GetPos())*0.1;
	ImageRoi roi;
	m_pVisionASM->GetPosImageROIParam(m_nCameraIndex, roi);
	roi.m_bGammaCorrectionEnable = m_bGammaEnable;
	m_pVisionASM->SetPosImageROIParam(m_nCameraIndex, roi);
	if (m_dGammaOld != dGamma)
	{
		CString strLog;
		strLog.Format(_T("修改位置%d[Gamma]由%lf到%lf"),m_nCameraIndex,m_dGammaOld,dGamma);
		g_opLogFile.SaveLogText(strLog,3);
	}
	m_dGammaOld = dGamma;

	CString strLog,strValue;
	if (bGammaEnableOld != m_bGammaEnable)
	{
		strValue = m_bGammaEnable ? _T("选中"):_T("未选中");
		strLog.Format(_T("修改[Gamma启用]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,3);
	}
}


void CDlgSysImgRoiParam::OnBnClickedBtnAutoBalance()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		ClipMouseCursor(TRUE);

		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_WHITE_BALANCE_BEGIN);//_T("开始自动白平衡，请稍等...");
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();

		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(FALSE);
		if (!m_pVisionASM->SetPosAutoBalance(m_nCameraIndex))
		{

			ClipMouseCursor(FALSE);

			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_WHITE_BALANCE_FAIL);//_T("自动白平衡失败");
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();

			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_WHITE_BALANCE_FAIL));

			CString strLog;
			strLog.Format(_T("点击[自动白平衡],设置失败"));
			g_opLogFile.SaveLogText(strLog,3);

			//return;
		}
		else
		{

			ClipMouseCursor(FALSE);

			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_WHITE_BALANCE_SUCCEED);//_T("自动白平衡成功");
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();

			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_SB_AUTO_WHITE_BALANCE_SUCCEED));

			CString strLog,strOld,strNew;
			strOld.Format(_T("[%.2f, %.2f, %.2f]"),m_dWhiteBalanceRatioR,m_dWhiteBalanceRatioG,m_dWhiteBalanceRatioB);

			CameraParam camParam;
			m_pVisionASM->GetPosCameraParam(m_nCameraIndex, camParam);
			m_dWhiteBalanceRatioR = camParam.m_dWhiteBalanceRatioR;
			m_dWhiteBalanceRatioG = camParam.m_dWhiteBalanceRatioG;
			m_dWhiteBalanceRatioB = camParam.m_dWhiteBalanceRatioB;

			CString strRatio;
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioR);
			GetDlgItem(IDC_EDIT_RATIO_R)->SetWindowText(strRatio);
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioG);
			GetDlgItem(IDC_EDIT_RATIO_G)->SetWindowText(strRatio);
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioB);
			GetDlgItem(IDC_EDIT_RATIO_B)->SetWindowText(strRatio);

			strNew.Format(_T("[%.2f, %.2f, %.2f]"),m_dWhiteBalanceRatioR,m_dWhiteBalanceRatioG,m_dWhiteBalanceRatioB);

			strLog.Format(_T("点击[自动白平衡],设置成功，修改[R,G,B]由%s到%s"), strOld, strNew);
			g_opLogFile.SaveLogText(strLog,3);
		}

		GetDlgItem(IDC_BTN_AUTO_BALANCE)->EnableWindow(m_bEnableColorSet && m_bCamPixelFormat);
		GetDlgItem(IDC_BTN_SET_WHITE_BALANCE_RATIO_DEFAULT)->EnableWindow(m_bEnableColorSet && m_bCamPixelFormat);

	}
}


void CDlgSysImgRoiParam::OnBnClickedBtnSetWhiteBalanceRatioDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex))
	{
		m_pParent->m_bStatusBarFlag = TRUE;
		m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WHITE_BALANCE_DEFAULT_BEIGN);//_T("恢复默认白平衡，请稍等...");
		m_pParent->m_strStatusBarInfo2 = _T("");
		m_pParent->UpdateStatusBar();

		if (!m_pVisionASM->SetPosCameraWhiteBalanceRatioDefault(m_nCameraIndex))
		{
			m_pParent->m_bStatusBarFlag = FALSE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WHITE_BALANCE_DEFAULT_FAIL);//_T("恢复默认白平衡失败");
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();

			CString strLog;
			strLog.Format(_T("点击[恢复默认],设置失败"));
			g_opLogFile.SaveLogText(strLog,3);

			return;
		}
		else
		{
			m_pParent->m_bStatusBarFlag = TRUE;
			m_pParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_WHITE_BALANCE_DEFAULT_SUCCEED);//_T("恢复默认白平衡成功");
			m_pParent->m_strStatusBarInfo2 = _T("");
			m_pParent->UpdateStatusBar();

			CString strLog,strOld,strNew;
			strOld.Format(_T("[%.2f, %.2f, %.2f]"),m_dWhiteBalanceRatioR,m_dWhiteBalanceRatioG,m_dWhiteBalanceRatioB);

			CameraParam camParam;
			m_pVisionASM->GetPosCameraParam(m_nCameraIndex, camParam);
			m_dWhiteBalanceRatioR = camParam.m_dWhiteBalanceRatioR;
			m_dWhiteBalanceRatioG = camParam.m_dWhiteBalanceRatioG;
			m_dWhiteBalanceRatioB = camParam.m_dWhiteBalanceRatioB;

			CString strRatio;
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioR);
			GetDlgItem(IDC_EDIT_RATIO_R)->SetWindowText(strRatio);
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioG);
			GetDlgItem(IDC_EDIT_RATIO_G)->SetWindowText(strRatio);
			strRatio.Format(_T("%.2f"),m_dWhiteBalanceRatioB);
			GetDlgItem(IDC_EDIT_RATIO_B)->SetWindowText(strRatio);


			strNew.Format(_T("[%.2f, %.2f, %.2f]"),m_dWhiteBalanceRatioR,m_dWhiteBalanceRatioG,m_dWhiteBalanceRatioB);

			strLog.Format(_T("点击[恢复默认],设置成功，修改[R,G,B]由%s到%s"), strOld, strNew);
			g_opLogFile.SaveLogText(strLog,3);
		}
	}
}


void CDlgSysImgRoiParam::ClipMouseCursor(BOOL bClip)
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

void CDlgSysImgRoiParam::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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
	GetDlgItem(nID)->EnableWindow(bEnable);
	GetDlgItem(nID)->ShowWindow(nShow);

	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		CRect CRect2;
		pWnd->GetWindowRect(&CRect2);
		ScreenToClient(&CRect2);
		if (CRectGroupBox.PtInRect(CRect2.TopLeft()) && CRectGroupBox.PtInRect(CRect2.BottomRight()))
		{
			if(pWnd->GetDlgCtrlID() == IDC_STATIC_GAMMA || 
				pWnd->GetDlgCtrlID() ==IDC_STATIC_GAMMA_VAL || 
				pWnd->GetDlgCtrlID() ==IDC_SLIDER_GAMMA || 
				pWnd->GetDlgCtrlID() ==IDC_CHECK_GAMMA)
			{
				if ((m_pParent!=NULL) && (m_pParent->m_pParent != NULL))
				{
					pWnd->ShowWindow(nShow && (m_pParent->m_pParent)->m_SysOptionConfig.m_bShowGamma);
				}	
			}
			else
			{
				
				pWnd->ShowWindow(nShow);
			}
			
			// 无需改变Enable属性的控件
			if (pWnd->GetDlgCtrlID() !=IDC_EDIT_RATIO_R ||
				pWnd->GetDlgCtrlID() !=IDC_EDIT_RATIO_G ||
				pWnd->GetDlgCtrlID() !=IDC_EDIT_RATIO_B)
			{
				pWnd->EnableWindow(bEnable);
			}

			

		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgSysImgRoiParam::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			bool bShow = false;
			bool bEnable = false;
			CString strNodeName = _T("");
			// 相机参数
			bool bFind = vsConfig->GetCheckedByItem(eCamremaAuthorize_ParamSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowParamSet,m_bEnableParamSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_CAM_PARAM,m_bEnableParamSet,m_bShowParamSet);
			}
			
			// 相机显示参数
			bFind = vsConfig->GetCheckedByItem(eCamremaAuthorize_ShowSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,bShow,bEnable);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_DISPLAY_CONFIG,bEnable,bShow);
			}

			// 彩色相机设置
			bFind = vsConfig->GetCheckedByItem(eCamremaAuthorize_ColorSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowColorSet,m_bEnableColorSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_WHITE_BALANCE,m_bEnableColorSet &&m_bCamPixelFormat,m_bShowColorSet &&m_bCamPixelFormat);
			}

			// 其他设置
			bFind = vsConfig->GetCheckedByItem(eCamremaAuthorize_OtherItem,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowOtherSet,m_bEnableOtherSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_GRP_CROSS,m_bEnableOtherSet,m_bShowOtherSet);

				GetDlgItem(IDC_BTN_LIVE_GRAB)->ShowWindow(m_bShowOtherSet);
				GetDlgItem(IDC_BTN_SAVE_IMAGE)->ShowWindow(m_bShowOtherSet);
				GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->ShowWindow(m_bShowOtherSet);
				

				GetDlgItem(IDC_BTN_LIVE_GRAB)->EnableWindow(m_bEnableOtherSet);
				GetDlgItem(IDC_BTN_SAVE_IMAGE)->EnableWindow(m_bEnableOtherSet && !m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex));
				GetDlgItem(IDC_BUTTON_MANUAL_MEASURE)->EnableWindow(m_bEnableOtherSet && !m_pVisionASM->IsPosCameraGrabbing(m_nCameraIndex));
			}


		}
	}
}

void CDlgSysImgRoiParam::OnBnClickedBtnLightCtrlSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pVisionASM || !m_pVisionASM->m_pSystempOptionConfig)
	{
		return;
	}


	CDlgLightCtrlSetting dlg;
	dlg.SetSystemOptionLightConfig(m_pVisionASM->m_pSystempOptionConfig->m_sysOptLightConfig);
	if (IDOK == dlg.DoModal())
	{
		SystemOptionLightConfig lightConfig;
		dlg.GetSystemOptionLightConfig(lightConfig);
		m_pVisionASM->m_pSystempOptionConfig->m_sysOptLightConfig = lightConfig;
		m_pParent->m_SysOptionConfig.m_sysOptLightConfig = lightConfig;
		//m_pVisionASM->m_pSystempOptionConfig->SaveSystemOption();// 这里先不进行最后的保存   等退出系统设置的时候  再决定是否保存  Summer0228
		//CLightController::GetInstance().OpenConnect(m_pVisionASM->m_pSystempOptionConfig->m_sysOptLightConfig);
	}
	else
	{
		// 恢复原先来配置
		CLightController::GetInstance().OpenConnect(m_pVisionASM->m_pSystempOptionConfig->m_sysOptLightConfig);
	}
}


void CDlgSysImgRoiParam::OnEnSetfocusEditInputShutterVal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INPUT_SHUTTER_VAL, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INPUT_SHUTTER_VAL, strOutput);
			CString strLog;
			strLog.Format(_T("修改位置%d[曝光]由%s到%s"),m_nCameraIndex,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditInputShutterVal();
	}
}


void CDlgSysImgRoiParam::OnEnKillfocusEditInputShutterVal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		double dMin = 0.0, dMax = 0.0, dCurrent = 0.0;
		m_pVisionASM->GetPosCameraShutter(m_nCameraIndex, dCurrent, dMin, dMax);

		CString strVal;
		GetDlgItemText(IDC_EDIT_INPUT_SHUTTER_VAL, strVal);
		if (!IsStringNumerical(strVal))
		{
			dCurrent = (double)(m_sliderShutter.GetPos() * 0.01);
			strVal.Format(_T("%.2f"), dCurrent);
			SetDlgItemText(IDC_EDIT_INPUT_SHUTTER_VAL,strVal);
		}
		else
		{
			if (atof(strVal) < dMin)
			{
				dCurrent = dMin;
			}
			else if (atof(strVal) > dMax)
			{
				dCurrent = dMax;
			}
			else
			{
				dCurrent = atof(strVal);
			}

			CString strTemp;
			strTemp.Format(_T("%.2f"), dCurrent);
			GetDlgItem(IDC_EDIT_INPUT_SHUTTER_VAL)->SetWindowText(strTemp);

			strTemp.Format(_T("%.2fms"), dCurrent);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strTemp);

			m_sliderShutter.SetPos((int) ((dCurrent+0.005) / 0.01) );

			m_pVisionASM->SetCameraShutter(m_nCameraIndex, dCurrent);
			m_pVisionASM->UpdatePosCameraShutter(m_nCameraIndex);

			UpdateData(FALSE);
		}
	}
}


void CDlgSysImgRoiParam::OnEnSetfocusEditInputGainVal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_INPUT_GAIN_VAL, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_INPUT_GAIN_VAL, strOutput);
			CString strLog;
			strLog.Format(_T("修改位置%d[增益]由%s到%s"),m_nCameraIndex,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,3);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditInputGainVal();
	}
}


void CDlgSysImgRoiParam::OnEnKillfocusEditInputGainVal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		double dMin = 0.0, dMax = 0.0, dCurrent = 0.0;
		m_pVisionASM->GetPosCameraGain(m_nCameraIndex, dCurrent, dMin, dMax);

		CString strVal;
		GetDlgItemText(IDC_EDIT_INPUT_GAIN_VAL, strVal);
		if (!IsStringNumerical(strVal))
		{
			dCurrent = (double)(m_sliderGain.GetPos() * 0.01);
			strVal.Format(_T("%.2f"), dCurrent);
			SetDlgItemText(IDC_EDIT_INPUT_GAIN_VAL,strVal);
		}
		else
		{
			if (atof(strVal) < dMin)
			{
				dCurrent = dMin;
			}
			else if (atof(strVal) > dMax)
			{
				dCurrent = dMax;
			}
			else
			{
				dCurrent = atof(strVal);
			}

			CString strTemp;
			strTemp.Format(_T("%.2f"), dCurrent);
			GetDlgItem(IDC_EDIT_INPUT_GAIN_VAL)->SetWindowText(strTemp);

			strTemp.Format(_T("%.2fdb"), dCurrent);
			GetDlgItem(IDC_STATIC_GAIN_VAL)->SetWindowText(strTemp);

			m_sliderGain.SetPos((int)((dCurrent+0.005) / 0.01) );

			m_pVisionASM->SetCameraGain(m_nCameraIndex, dCurrent);
			m_pVisionASM->UpdatePosCameraGain(m_nCameraIndex);

			UpdateData(FALSE);
		}
	}
}
