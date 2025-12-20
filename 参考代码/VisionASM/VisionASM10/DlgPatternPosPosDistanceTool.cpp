// DlgDonleQuicksearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgPatternPosPosDistanceTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// DlgDonleQuicksearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternPosPosDistanceTool, CDialogEx)

CDlgPatternPosPosDistanceTool::CDlgPatternPosPosDistanceTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternPosPosDistanceTool::IDD, pParent)
	, m_dPixelRatio(1)
	, m_bFirstSearchToolConst(FALSE)
	, m_bSecondSearchToolConst(FALSE)
	, m_bEnableMax1(FALSE)
	, m_bEnableMin1(FALSE)
	, m_dDistanceMax1(5)
	, m_dDistanceMin1(0)
	, m_dDistance1(0)
	, m_dDistanceMax2(0)
	, m_dDistanceMax3(0)
	, m_dDistanceMin2(0)
	, m_dDistanceMin3(0)
	, m_dDistance3(0)
	, m_bEnableMin2(FALSE)
	, m_bEnableMin3(FALSE)
	, m_bEnableMax3(FALSE)
	, m_bEnableMax2(FALSE)
	, m_bEnablePosPosDist(FALSE)
	, m_bEnableFirstCoarsePattern(FALSE)
{
	ZeroMemory(&m_dDistance2, sizeof(DECIMAL));
	m_dAngleHigh = 5.0;
	m_dAngleLow = -5.0;
	m_nSearchNum = 1;
	m_dAcceptThre = 0.6;
	m_dConfusionThre = 0.8;
	m_nPatAccuracyMode = 1;
	m_nPatSpeedMode = 3;
	m_bEnableMask = FALSE;
	

	m_nSearchNum1 = 1;
	m_dAcceptThre1 = 0.6;
	m_dConfusionThre1 = 0.8;
	m_bEnableMask1 = FALSE;
	m_dAngleHigh1 = 5.0;
	m_dAngleLow1 = -5.0;
	m_nPatAccuracyMode1 = 1;
	m_nPatSpeedMode1 = 3;

	m_nSearchNum2 = 1;
	m_dAcceptThre2 = 0.6;
	m_dConfusionThre2 = 0.8;
	m_bEnableMask2 = FALSE;
	m_dAngleHigh2 = 5.0;
	m_dAngleLow2 = -5.0;
	m_nPatAccuracyMode2 = 1;
	m_nPatSpeedMode2 = 3;

	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

CDlgPatternPosPosDistanceTool::~CDlgPatternPosPosDistanceTool()
{
}

void CDlgPatternPosPosDistanceTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_SET_MASK1, m_btnSetMask1);
	DDX_Control(pDX, IDC_BTN_SET_MASK2, m_btnSetMask2);
	DDX_Control(pDX, IDC_BTN_SET_CENTER1, m_btnSetCenter1);
	DDX_Control(pDX, IDC_BTN_SET_CENTER2, m_btnSetCenter2);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_COMBO_PRECISION1, m_comboAccuracy1);
	DDX_Control(pDX, IDC_COMBO_PRECISION2, m_comboAccuracy2);
	DDX_Control(pDX, IDC_COMBO_SPEED1, m_comboSpeed1);
	DDX_Control(pDX, IDC_COMBO_SPEED2, m_comboSpeed2);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MASK1,m_bEnableMask1);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MASK2,m_bEnableMask2);
	DDX_Text(pDX, IDC_EDIT_LOW1, m_dAngleLow1);
	DDX_Text(pDX, IDC_EDIT_HIGH1, m_dAngleHigh1);
	DDX_Text(pDX, IDC_EDIT_LOW2, m_dAngleLow2);
	DDX_Text(pDX, IDC_EDIT_HIGH2, m_dAngleHigh2);
	DDX_Text(pDX, IDC_EDIT_TOOL1_SIMILAR1, m_dAcceptThre1);
	DDX_Text(pDX, IDC_EDIT_TOOL1_SIMILAR2, m_dConfusionThre1);
	DDX_Text(pDX, IDC_EDIT_SEARCH_NUM1, m_nSearchNum1);
	DDX_Text(pDX, IDC_EDIT_TOOL2_SIMILAR1, m_dAcceptThre2);
	DDX_Text(pDX, IDC_EDIT_TOOL2_SIMILAR2, m_dConfusionThre2);
	DDX_Text(pDX, IDC_EDIT_SEARCH_NUM2, m_nSearchNum2);
	DDX_Text(pDX, IDC_EDT_PIXEL_RATIO, m_dPixelRatio);
	DDX_Check(pDX, IDC_CHECK_FIRST_SERCH_TOOL_CONST_POINT, m_bFirstSearchToolConst);
	DDX_Check(pDX, IDC_CHECK_SECOND_SERCH_TOOL_CONST_POINT, m_bSecondSearchToolConst);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_1, m_bEnableMax1);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_1, m_bEnableMin1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_1, m_dDistanceMax1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_1, m_dDistanceMin1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_1, m_dDistance1);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_2, m_dDistanceMax2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MAX_3, m_dDistanceMax3);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_3, m_dDistanceMin2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_MIN_4, m_dDistanceMin3);
	DDX_Text(pDX, IDC_EDT_DISTANCE_3, m_dDistance2);
	DDX_Text(pDX, IDC_EDT_DISTANCE_4, m_dDistance3);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_3, m_bEnableMin2);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MIN_4, m_bEnableMin3);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_3, m_bEnableMax3);
	DDX_Check(pDX, IDC_CHECK_DISTANCE_MAX_2, m_bEnableMax2);
	DDX_Check(pDX, IDC_CHECK_ENABLE_POSPOS_DIST, m_bEnablePosPosDist);

	DDX_Text(pDX, IDC_EDIT_LOW, m_dAngleLow);
	DDX_Text(pDX, IDC_EDIT_HIGH, m_dAngleHigh);
	DDX_Text(pDX, IDC_EDIT_TOOL_SIMILAR1, m_dAcceptThre);
	DDX_Text(pDX, IDC_EDIT_TOOL_SIMILAR2, m_dConfusionThre);
	DDX_Text(pDX, IDC_EDIT_SEARCH_NUM, m_nSearchNum);
	DDX_Control(pDX, IDC_COMBO_PRECISION, m_comboAccuracy);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MASK,m_bEnableMask);
	DDX_Check(pDX, IDC_CHECK_ENABLE_FIRST_COARSE_PATTERN, m_bEnableFirstCoarsePattern);
}


BEGIN_MESSAGE_MAP(CDlgPatternPosPosDistanceTool, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PRECISION1, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision1)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED1, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed1)
	ON_CBN_SELCHANGE(IDC_COMBO_PRECISION2, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision2)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED2, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed2)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnGetImg)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER1, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMark1)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER2, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMark2)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH1, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnTool1Searchall)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH2, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnTool2Searchall)
	ON_BN_CLICKED(IDC_BTN_SET_MASK1, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask1)
	ON_BN_CLICKED(IDC_BTN_SET_MASK2, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask2)

	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASK1, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableMask1)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASK2, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnablemask2)

	ON_EN_SETFOCUS(IDC_EDIT_LOW1, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleLow1)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH1, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleHigh1)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL1_SIMILAR1, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAcceptThre1)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL1_SIMILAR2, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtConfusionThre1)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_NUM1, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtSearchNum1)
	ON_EN_SETFOCUS(IDC_EDIT_LOW2, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleLow2)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH2, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleHigh2)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL2_SIMILAR1, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtAcceptThre2)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL2_SIMILAR2, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtConfusionThre2)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_NUM2, &CDlgPatternPosPosDistanceTool::OnSetfocusEdtSearchNum2)
	ON_BN_CLICKED(IDC_CHECK_FIRST_SERCH_TOOL_CONST_POINT, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckFirstSerchToolConstPoint)
	ON_BN_CLICKED(IDC_CHECK_SECOND_SERCH_TOOL_CONST_POINT, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckSecondSerchToolConstPoint)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MAX_1, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax1)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MIN_1, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin1)
	ON_EN_SETFOCUS(IDC_EDT_PIXEL_RATIO, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtPixelRatio)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_1, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax1)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_1, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin1)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MAX_2, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax2)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MIN_3, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin3)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MAX_3, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax3)
	ON_BN_CLICKED(IDC_CHECK_DISTANCE_MIN_4, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin4)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_2, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax2)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_3, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin3)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MAX_3, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax3)
	ON_EN_SETFOCUS(IDC_EDT_DISTANCE_MIN_4, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin4)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_POSPOS_DIST, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnablePosposDist)
	ON_BN_CLICKED(IDC_BTN_SET_MASK, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASK, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableMask)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetCenter)
	ON_CBN_SELCHANGE(IDC_COMBO_PRECISION, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, &CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_LOW, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEditLow)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH, &CDlgPatternPosPosDistanceTool::OnEnSetfocusEditHigh)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_FIRST_COARSE_PATTERN, &CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableFirstCoarsePattern)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, &CDlgPatternPosPosDistanceTool::OnBnClickedBtnFullImageSearch)
END_MESSAGE_MAP()


// DlgDonleQuicksearchTool 消息处理程序
BOOL CDlgPatternPosPosDistanceTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnSetMask1.SetThemeHelper(&m_themeHelper);
	m_btnSetMask2.SetThemeHelper(&m_themeHelper);
	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_btnSetCenter1.SetThemeHelper(&m_themeHelper);
	m_btnSetCenter2.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}	

	m_GuiPatternDisplay.Create(IDC_STATIC_IMG, this, 0);
	m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiPatternDisplay.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiPatternDisplay.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiPatternDisplay.SetLanguage(1);
		break;
	}

	m_GuiPatternDisplay2.Create(IDC_STATIC_IMG1, this, 0);
	m_GuiPatternDisplay2.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiPatternDisplay2.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiPatternDisplay2.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiPatternDisplay2.SetLanguage(1);
		break;
	}
	m_GuiPatternDisplay3.Create(IDC_STATIC_IMG2, this, 0);
	m_GuiPatternDisplay3.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));


	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiPatternDisplay3.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiPatternDisplay3.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiPatternDisplay3.SetLanguage(1);
		break;
	}

	m_comboAccuracy.InsertString(0,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_comboAccuracy.InsertString(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_comboAccuracy.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_comboAccuracy.SetCurSel(m_nPatAccuracyMode);

	m_comboSpeed.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_comboSpeed.InsertString(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_comboSpeed.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed.InsertString(4,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed.InsertString(5,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed.SetCurSel(m_nPatSpeedMode);

	m_comboAccuracy1.InsertString(0,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_comboAccuracy1.InsertString(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_comboAccuracy1.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_comboAccuracy1.SetCurSel(m_nPatAccuracyMode1);

	m_comboSpeed1.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_comboSpeed1.InsertString(1,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_comboSpeed1.InsertString(2,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed1.InsertString(3,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed1.InsertString(4,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed1.InsertString(5,GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed1.SetCurSel(m_nPatSpeedMode1);


	m_comboAccuracy2.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_FINE));
	m_comboAccuracy2.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_STANDARD));
	m_comboAccuracy2.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACCURACY_COARSE));
	m_comboAccuracy2.SetCurSel(m_nPatAccuracyMode2);

	m_comboSpeed2.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARTER));
	m_comboSpeed2.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_HALF));
	m_comboSpeed2.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_STANDARD));
	m_comboSpeed2.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_DOUBLE));
	m_comboSpeed2.InsertString(4, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_QUARDRUPLE));
	m_comboSpeed2.InsertString(5, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPEED_OCTUPLE));
	m_comboSpeed2.SetCurSel(m_nPatSpeedMode2);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

//更新工具界面显示
void CDlgPatternPosPosDistanceTool::UpdateControlDisplay()
{
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CENTER1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SET_CENTER1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SET_CENTER2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH2)->EnableWindow(TRUE);
	}

	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{
			GetDlgItem(IDC_CHECK_ENABLE_MASK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(m_bEnableMask);

			GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(m_bEnableMask1);
			GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(m_bEnableMask2);
		}
		else
		{
			GetDlgItem(IDC_CHECK_ENABLE_MASK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(FALSE);

			GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_ENABLE_MASK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(m_bEnableMask);

		GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(m_bEnableMask1);
		GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(m_bEnableMask2);
	}
}

//定位工具训练数据与界面之间的数据获取
void CDlgPatternPosPosDistanceTool::UpdateTrainData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);

		//工具参数传递
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_nPatAccuracyMode);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_nPatSpeedMode+1);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_bEnableMask;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  =  m_dAngleLow;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh = m_dAngleHigh;

		//工具1参数传递
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1 = (Accuracy)(1<<m_nPatAccuracyMode1);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1 = (CoarseScale)(m_nPatSpeedMode1+1);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1 = m_bEnableMask1/*m_imageMask1.IsValid() ? m_bEnableMask : false*/;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1  =  m_dAngleLow1;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1 = m_dAngleHigh1;

		//工具2参数传递
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy2 = (Accuracy)(1<<m_nPatAccuracyMode2);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale2 = (CoarseScale)(m_nPatSpeedMode2+1);
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable2 = m_bEnableMask2/*m_imageMask1.IsValid() ? m_bEnableMask : false*/;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow2  =  m_dAngleLow2;
		((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh2 = m_dAngleHigh2;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		//工具界面设置
		m_nPatAccuracyMode = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
		m_nPatSpeedMode = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
		m_dAngleLow	    = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
		m_dAngleHigh	= ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
		m_bEnableMask = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	
		if (m_bEnableMask)
		{
			GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(FALSE);
		}
		m_comboAccuracy.SetCurSel(m_nPatAccuracyMode1);
		m_comboSpeed.SetCurSel(m_nPatSpeedMode1);


		//工具1界面设置
		m_nPatAccuracyMode1 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1/2;
		m_nPatSpeedMode1 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1-1;
		m_dAngleLow1	    = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1;
		m_dAngleHigh1	= ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1;
		m_bEnableMask1 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1;	
		if (m_bEnableMask1)
		{
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(FALSE);
		}
		m_comboAccuracy1.SetCurSel(m_nPatAccuracyMode1);
		m_comboSpeed1.SetCurSel(m_nPatSpeedMode1);

		//工具2界面设置
		m_nPatAccuracyMode2 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy2/2;
		m_nPatSpeedMode2 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale2-1;
		m_dAngleLow2	    = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow2;
		m_dAngleHigh2	= ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh2;
		m_bEnableMask2 = ((CPatternPosPosDistanceTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable2;
		if (m_bEnableMask2)
		{
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(FALSE);
		}
		m_comboAccuracy2.SetCurSel(m_nPatAccuracyMode2);
		m_comboSpeed2.SetCurSel(m_nPatSpeedMode2);
		
		UpdateData(FALSE);
	}
}

//定位工具搜索数据与界面之间的数据获取
void CDlgPatternPosPosDistanceTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);

	if (bValidate) // 从界面获取搜索参数
	{
		UpdateData(TRUE);
		//工具搜索数据
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_nSearchNum;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_dAcceptThre;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_dConfusionThre;

		//工具1搜索数据
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1 = m_nSearchNum1;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1 = m_dAcceptThre1;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1 = m_dConfusionThre1;

		//工具2搜索数据
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum2 = m_nSearchNum2;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2 = m_dAcceptThre2;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2 = m_dConfusionThre1;

		//测量参数
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bFirstSearchToolConst = m_bFirstSearchToolConst;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSecondSearchToolConst = m_bSecondSearchToolConst;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio = m_dPixelRatio;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[0] = m_dDistanceMax1;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[0] = m_dDistanceMin1;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[0] = m_bEnableMax1;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[0] = m_bEnableMin1;

		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[1] = m_dDistanceMax2;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[1] = m_dDistanceMin2;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[1] = m_bEnableMax2;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[1] = m_bEnableMin2;

		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[2] = m_dDistanceMax3;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[2] = m_dDistanceMin3;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[2] = m_bEnableMax3;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[2] = m_bEnableMin3;

		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnablePosPosDist = m_bEnablePosPosDist;
		((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFirstCoarsePattern = m_bEnableFirstCoarsePattern;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		//工具1搜索数据
		m_nSearchNum		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
		m_dAcceptThre		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
		m_dConfusionThre	= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

		//工具1搜索数据
		m_nSearchNum1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1;
		m_dAcceptThre1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1;	
		m_dConfusionThre1	= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1;

		//工具2搜索数据
		m_nSearchNum2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum2;
		m_dAcceptThre2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2;	
		m_dConfusionThre2	= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2;

		m_bFirstSearchToolConst		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bFirstSearchToolConst;
		m_bSecondSearchToolConst	= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bSecondSearchToolConst;
		m_dPixelRatio		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dPixelRatio ;
		m_dDistanceMax1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[0] ;
		m_dDistanceMin1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[0];
		m_bEnableMax1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[0];
		m_bEnableMin1		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[0];

		m_dDistanceMax2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[1] ;
		m_dDistanceMin2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[1];
		m_bEnableMax2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[1];
		m_bEnableMin2		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[1];

		m_dDistanceMax3		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMax[2] ;
		m_dDistanceMin3		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dDistanceMin[2];
		m_bEnableMax3		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMax[2];
		m_bEnableMin3		= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableMin[2];

		m_bEnablePosPosDist	= ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnablePosPosDist;
		m_bEnableFirstCoarsePattern = ((CPatternPosPosDistanceSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bEnableFirstCoarsePattern;
		UpdateData(FALSE);
	}

	//GetDlgItem(IDC_CHECK_FIRST_SERCH_TOOL_CONST_POINT)->EnableWindow(m_bEnablePosPosDist);
	//GetDlgItem(IDC_CHECK_SECOND_SERCH_TOOL_CONST_POINT)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_PIXEL_RATIO)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MIN_1)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MIN_1)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MAX_1)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MAX_1)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MIN_3)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MIN_3)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MAX_2)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MAX_2)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MIN_4)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MIN_4)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_CHECK_DISTANCE_MAX_3)->EnableWindow(m_bEnablePosPosDist);
	GetDlgItem(IDC_EDT_DISTANCE_MAX_3)->EnableWindow(m_bEnablePosPosDist);

}

void CDlgPatternPosPosDistanceTool::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_pDlgParent->m_strCurWorkDir;

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

		if(_T("IDD_DLG_PATTERNPOSPOSDISTATNCE_TOOL") == strDlgID)
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


//检查对话框的有效性，将无效参数设置为默认值
void CDlgPatternPosPosDistanceTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	if (m_nSearchNum1 < 1)
	{
		m_nSearchNum1 = 1;
	}
	if (m_nSearchNum2 < 1)
	{
		m_nSearchNum2 = 1;
	}

	if ((m_dAcceptThre1 < 0.0) || (m_dAcceptThre1 > 1.0))
	{
		m_dAcceptThre1 = 0.6;
	}
	if ((m_dAcceptThre2 < 0.0) || (m_dAcceptThre2 > 1.0))
	{
		m_dAcceptThre2 = 0.6;
	}

	if ((m_dConfusionThre1 < 0.0) || (m_dConfusionThre1 > 1.0))
	{
		m_dConfusionThre1 = 0.8;
	}
	if ((m_dConfusionThre2 < 0.0) || (m_dConfusionThre2 > 1.0))
	{
		m_dConfusionThre2 = 0.8;
	}

// 	if ((m_dSharpnessScore1 < 0.0) || (m_dSharpnessScore1 > 1.0))
// 	{
// 		m_dSharpnessScore1 = 0.5;
// 	}

	if (m_dAcceptThre1 > m_dConfusionThre1)
	{
		m_dAcceptThre1 = 0.6;
		m_dConfusionThre1 = 0.8;
	}
	if (m_dAcceptThre2 > m_dConfusionThre2)
	{
		m_dAcceptThre2 = 0.6;
		m_dConfusionThre2 = 0.8;
	}

	if (m_dAngleLow1 > m_dAngleHigh1)
	{
		m_dAngleLow1 = -5.0;
		m_dAngleHigh1 = 5.0;
	}
	if (m_dAngleLow2 > m_dAngleHigh2)
	{
		m_dAngleLow2 = -5.0;
		m_dAngleHigh2 = 5.0;
	}
}

//工具1精度设置
void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatAccuracyMode1 = m_comboAccuracy1.GetCurSel();
}

//工具1速度设置
void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatSpeedMode1 = m_comboSpeed1.GetCurSel();
}

//工具2精度设置
void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatAccuracyMode2 = m_comboAccuracy2.GetCurSel();
}

//工具2速度设置
void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatSpeedMode2 = m_comboSpeed2.GetCurSel();
}

//获取训练图像
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnGetImg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	UpdateData(TRUE);
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;
	if (m_bEnableMask1)
	{
		if ( (m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
			|| (m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
		{
			m_imageMask.Release();
		}
	}

	if (m_bEnableMask2)
	{
		if ( (m_imageMask2.Height() != m_pDlgParent->m_imageTrain.Height())
			|| (m_imageMask2.Width() != m_pDlgParent->m_imageTrain.Width()) )
		{
			m_imageMask2.Release();
		}
	}
 	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}

//训练
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
	if (!m_pDlgParent->m_imageTrain.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED);		//_T("训练失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_TRAIN_IMAGE);	//_T("训练图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	CheckDlgDataValidation();
	UpdateTrainData(true);

	m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);
	if(m_bEnableMask)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
	}
	if (m_bEnableMask1)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage2(m_imageMask2);
	}
	if (m_bEnableMask2)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage3(m_imageMask3);
	}

	if (m_pDlgParent->m_pSearchTool->Train())
	{
		scImage8 trainImage;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
		m_GuiPatternDisplay.SetImage(trainImage, TRUE);

		m_pDlgParent->m_pSearchTool->GetTrainResultImage2(trainImage);
		m_GuiPatternDisplay2.SetImage(trainImage, TRUE);

		m_pDlgParent->m_pSearchTool->GetTrainResultImage3(trainImage);
		m_GuiPatternDisplay3.SetImage(trainImage, TRUE);

		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED));
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}

//工具1提取标记点
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMark1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		scImageGray imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternPosPosDistanceTrainGuiParam *pTrainGuiParam = new CPatternPosPosDistanceTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint1.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint1.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect1);
		dMarkX -= pTrainGuiParam->m_TrainRect1.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect1.GetUL().GetY();

		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			pTrainGuiParam->m_ModelPoint1.SetX(dMarkX+pTrainGuiParam->m_TrainRect1.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint1.SetY(dMarkY+pTrainGuiParam->m_TrainRect1.GetUL().GetY());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
			m_pDlgParent->m_pSearchTool->Train();
			m_pDlgParent->m_comboDisplayType.SetCurSel(1);
			m_pDlgParent->UpdateGuiDisplay();
		}
		delete pTrainGuiParam;
		pTrainGuiParam = NULL;
	}
	else
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_EXTRACTION_WND_FAIL);
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NO_TRAINED);
		m_pDlgParent->UpdateStatusBar();
	}
}

//工具2提取标记点
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMark2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		scImageGray imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternPosPosDistanceTrainGuiParam *pTrainGuiParam = new CPatternPosPosDistanceTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint2.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint2.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect2);
		dMarkX -= pTrainGuiParam->m_TrainRect2.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect2.GetUL().GetY();

		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			pTrainGuiParam->m_ModelPoint2.SetX(dMarkX+pTrainGuiParam->m_TrainRect2.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint2.SetY(dMarkY+pTrainGuiParam->m_TrainRect2.GetUL().GetY());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
			m_pDlgParent->m_pSearchTool->Train();
			m_pDlgParent->m_comboDisplayType.SetCurSel(1);
			m_pDlgParent->UpdateGuiDisplay();
		}
		delete pTrainGuiParam;
		pTrainGuiParam = NULL;
	}
	else
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_EXTRACTION_WND_FAIL);
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NO_TRAINED);
		m_pDlgParent->UpdateStatusBar();
	}
}

//工具1全图搜索
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnTool1Searchall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternPosPosDistanceTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_SearchRect1.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_AND_TRAIN_MODEL_SUCCEED);
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_FAIL);
				m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RE_TRAIN_FAIL);
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}

//工具2全图搜索
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnTool2Searchall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternPosPosDistanceTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_SearchRect2.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_AND_TRAIN_MODEL_SUCCEED);
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_FAIL);
				m_pDlgParent->m_strStatusBarInfo2 = _T("模板重新训练失败");
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}

//工具1设置掩膜
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask2.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask2.Height()) )
	{
		m_imageMask2.Release();
	}

	dlgMask.m_GuiMaskEdit.SetMaskImage(m_imageMask2);
	dlgMask.SetSysInfoStringsRef(m_psaSysInfoStrings);

	if (dlgMask.DoModal() == IDOK)
	{
		dlgMask.m_GuiMaskEdit.GetMaskImage(m_imageMask2);
	}
}

//工具2设置掩膜
void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask3.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask3.Height()) )
	{
		m_imageMask3.Release();
	}

	dlgMask.m_GuiMaskEdit.SetMaskImage(m_imageMask3);
	dlgMask.SetSysInfoStringsRef(m_psaSysInfoStrings);

	if (dlgMask.DoModal() == IDOK)
	{
		dlgMask.m_GuiMaskEdit.GetMaskImage(m_imageMask3);
	}
}

//工具1启用掩膜
void CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_bEnableMask1)
	{
		GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(FALSE);
	}
}

//工具2启用掩膜
void CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnablemask2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_bEnableMask2)
	{
		GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(FALSE);
	}
}

//以下是通过编辑框启用键盘
//工具1
void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleLow1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LOW1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LOW1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LOW1, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleHigh1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_HIGH1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_HIGH1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_HIGH1, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtSearchNum1() 
{
	//TODO: Add your control notification handler code here
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_NUM1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_NUM1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_NUM1, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAcceptThre1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TOOL1_SIMILAR1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TOOL1_SIMILAR1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TOOL1_SIMILAR1, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtConfusionThre1() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TOOL1_SIMILAR2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TOOL1_SIMILAR2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TOOL1_SIMILAR2, strOutput);
		}
	}
}

//工具2
void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleLow2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LOW2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LOW2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LOW2, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAngleHigh2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_HIGH2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_HIGH2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_HIGH2, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtSearchNum2() 
{
	//TODO: Add your control notification handler code here
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_SEARCH_NUM2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_SEARCH_NUM2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_SEARCH_NUM2, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtAcceptThre2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TOOL2_SIMILAR1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TOOL2_SIMILAR1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TOOL2_SIMILAR1, strOutput);
		}
	}
}

void CDlgPatternPosPosDistanceTool::OnSetfocusEdtConfusionThre2() 
{
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TOOL2_SIMILAR2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TOOL2_SIMILAR2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TOOL2_SIMILAR2, strOutput);
		}
	}
}


BOOL CDlgPatternPosPosDistanceTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgPatternPosPosDistanceTool::UpdateSearchResult()
{
	CSearchResult searchResult;
	m_pDlgParent->m_pSearchTool->GetResult(0, searchResult);
	m_dDistance1 = searchResult.m_resultPosPosDistanceTool.m_dDistanceMM0;
	m_dDistance2 = searchResult.m_resultPosPosDistanceTool.m_dDistanceMM1;
	m_dDistance3 = searchResult.m_resultPosPosDistanceTool.m_dDistanceMM2;

	UpdateData(FALSE);
}

void CDlgPatternPosPosDistanceTool::OnBnClickedCheckFirstSerchToolConstPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckSecondSerchToolConstPoint()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtPixelRatio()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_PIXEL_RATIO)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_PIXEL_RATIO, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_PIXEL_RATIO, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MAX_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MAX_1, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MIN_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MIN_1, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMax3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckDistanceMin4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MAX_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MAX_2, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MIN_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MIN_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MIN_3, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMax3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MAX_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MAX_3, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MAX_3, strOutput);
			UpdateSearchData(TRUE);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEdtDistanceMin4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_DISTANCE_MIN_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_DISTANCE_MIN_4, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_DISTANCE_MIN_4, strOutput);
			UpdateSearchData(TRUE);
		}
	}
	
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnablePosposDist()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetMask()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask.Height()) )
	{
		m_imageMask.Release();
	}

	dlgMask.m_GuiMaskEdit.SetMaskImage(m_imageMask);
	dlgMask.SetSysInfoStringsRef(m_psaSysInfoStrings);

	if (dlgMask.DoModal() == IDOK)
	{
		dlgMask.m_GuiMaskEdit.GetMaskImage(m_imageMask);
	}
}


void CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableMask()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_bEnableMask)
	{
		GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_SET_MASK)->EnableWindow(FALSE);
	}
}


void CDlgPatternPosPosDistanceTool::OnBnClickedBtnSetCenter()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		scImageGray imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternPosPosDistanceTrainGuiParam *pTrainGuiParam = new CPatternPosPosDistanceTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect);
		dMarkX -= pTrainGuiParam->m_TrainRect.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect.GetUL().GetY();

		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
		dlgPatternCenterSet.SetSysInfoStringsRef(m_pDlgParent->m_psaSysInfoStrings);
		dlgPatternCenterSet.EnableSysKeyboard(m_pDlgParent->m_bSysKeyboardEnabled);
		if (dlgPatternCenterSet.DoModal() == IDOK)
		{
			dlgPatternCenterSet.GetPatternCenter(dMarkX, dMarkY);
			pTrainGuiParam->m_ModelPoint.SetX(dMarkX+pTrainGuiParam->m_TrainRect.GetUL().GetX());
			pTrainGuiParam->m_ModelPoint.SetY(dMarkY+pTrainGuiParam->m_TrainRect.GetUL().GetY());
			m_pDlgParent->m_pSearchTool->SetTrainGuiParam(pTrainGuiParam);
			m_pDlgParent->m_pSearchTool->Train();
			m_pDlgParent->m_comboDisplayType.SetCurSel(1);
			m_pDlgParent->UpdateGuiDisplay();
		}
		delete pTrainGuiParam;
		pTrainGuiParam = NULL;
	}
	else
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_OPEN_EXTRACTION_WND_FAIL);
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SEARCH_TOOL_NO_TRAINED);
		m_pDlgParent->UpdateStatusBar();
	}
}


void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboPrecision()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatAccuracyMode = m_comboAccuracy.GetCurSel();
}


void CDlgPatternPosPosDistanceTool::OnCbnSelchangeComboSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	m_nPatSpeedMode = m_comboSpeed.GetCurSel();
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEditLow()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LOW)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LOW, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LOW, strOutput);
		}
	}
}


void CDlgPatternPosPosDistanceTool::OnEnSetfocusEditHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_HIGH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_HIGH, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_HIGH, strOutput);
		}
	}
}



void CDlgPatternPosPosDistanceTool::OnBnClickedCheckEnableFirstCoarsePattern()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateSearchData(TRUE);
}


void CDlgPatternPosPosDistanceTool::OnBnClickedBtnFullImageSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternPosPosDistanceTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_SearchRect.Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_AND_TRAIN_MODEL_SUCCEED);
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_SET_FULL_IMAGE_FAIL);
				m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_RE_TRAIN_FAIL);
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}
