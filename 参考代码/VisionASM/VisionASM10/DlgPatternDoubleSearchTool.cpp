// DlgDonleQuicksearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgPatternDoubleSearchTool.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"
#include "DlgPatternCenterSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// DlgDonleQuicksearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgPatternDouleSearchTool, CDialogEx)

CDlgPatternDouleSearchTool::CDlgPatternDouleSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPatternDouleSearchTool::IDD, pParent)
	, m_nImageChannel(0)
{
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

CDlgPatternDouleSearchTool::~CDlgPatternDouleSearchTool()
{
}

void CDlgPatternDouleSearchTool::DoDataExchange(CDataExchange* pDX)
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
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
}


BEGIN_MESSAGE_MAP(CDlgPatternDouleSearchTool, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PRECISION1, &CDlgPatternDouleSearchTool::OnCbnSelchangeComboPrecision1)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED1, &CDlgPatternDouleSearchTool::OnCbnSelchangeComboSpeed1)
	ON_CBN_SELCHANGE(IDC_COMBO_PRECISION2, &CDlgPatternDouleSearchTool::OnCbnSelchangeComboPrecision2)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED2, &CDlgPatternDouleSearchTool::OnCbnSelchangeComboSpeed2)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, &CDlgPatternDouleSearchTool::OnBnClickedBtnGetImg)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CDlgPatternDouleSearchTool::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER1, &CDlgPatternDouleSearchTool::OnBnClickedBtnSetMark1)
	ON_BN_CLICKED(IDC_BTN_SET_CENTER2, &CDlgPatternDouleSearchTool::OnBnClickedBtnSetMark2)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH1, &CDlgPatternDouleSearchTool::OnBnClickedBtnTool1Searchall)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH2, &CDlgPatternDouleSearchTool::OnBnClickedBtnTool2Searchall)
	ON_BN_CLICKED(IDC_BTN_SET_MASK1, &CDlgPatternDouleSearchTool::OnBnClickedBtnSetMask1)
	ON_BN_CLICKED(IDC_BTN_SET_MASK2, &CDlgPatternDouleSearchTool::OnBnClickedBtnSetMask2)

	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASK1, &CDlgPatternDouleSearchTool::OnBnClickedCheckEnableMask1)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_MASK2, &CDlgPatternDouleSearchTool::OnBnClickedCheckEnablemask2)

	ON_EN_SETFOCUS(IDC_EDIT_LOW1, &CDlgPatternDouleSearchTool::OnSetfocusEdtAngleLow1)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH1, &CDlgPatternDouleSearchTool::OnSetfocusEdtAngleHigh1)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL1_SIMILAR1, &CDlgPatternDouleSearchTool::OnSetfocusEdtAcceptThre1)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL1_SIMILAR2, &CDlgPatternDouleSearchTool::OnSetfocusEdtConfusionThre1)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_NUM1, &CDlgPatternDouleSearchTool::OnSetfocusEdtSearchNum1)
	ON_EN_SETFOCUS(IDC_EDIT_LOW2, &CDlgPatternDouleSearchTool::OnSetfocusEdtAngleLow2)
	ON_EN_SETFOCUS(IDC_EDIT_HIGH2, &CDlgPatternDouleSearchTool::OnSetfocusEdtAngleHigh2)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL2_SIMILAR1, &CDlgPatternDouleSearchTool::OnSetfocusEdtAcceptThre2)
	ON_EN_SETFOCUS(IDC_EDIT_TOOL2_SIMILAR2, &CDlgPatternDouleSearchTool::OnSetfocusEdtConfusionThre2)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH_NUM2, &CDlgPatternDouleSearchTool::OnSetfocusEdtSearchNum2)
END_MESSAGE_MAP()


// DlgDonleQuicksearchTool 消息处理程序
BOOL CDlgPatternDouleSearchTool::OnInitDialog()
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
	m_GuiPatternDisplay1.Create(IDC_STATIC_IMG1, this, 0);
	m_GuiPatternDisplay1.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
	
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		m_GuiPatternDisplay1.SetLanguage(0);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		m_GuiPatternDisplay1.SetLanguage(1);
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		m_GuiPatternDisplay1.SetLanguage(1);
		break;
	}
	m_GuiPatternDisplay2.Create(IDC_STATIC_IMG2, this, 0);
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
void CDlgPatternDouleSearchTool::UpdateControlDisplay()
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
			GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(m_bEnableMask1);
			GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(m_bEnableMask2);

			if (m_pDlgParent->m_imageTrain.IsRGB24() || m_pDlgParent->m_imageTrain.IsRGB32())
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(FALSE);

			cpImage imageTrain;
			m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
			if (imageTrain.IsValid() && 
				(imageTrain.IsRGB24() || imageTrain.IsRGB32()))
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_ENABLE_MASK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK1)->EnableWindow(m_bEnableMask1);
		GetDlgItem(IDC_CHECK_ENABLE_MASK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SET_MASK2)->EnableWindow(m_bEnableMask2);

		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
}

//定位工具训练数据与界面之间的数据获取
void CDlgPatternDouleSearchTool::UpdateTrainData(bool bValidate)
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

		//工具1参数传递
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_nImageChannel;
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1 = (Accuracy)(1<<m_nPatAccuracyMode1);
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1 = (CoarseScale)(m_nPatSpeedMode1+1);
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1 = m_bEnableMask1/*m_imageMask1.IsValid() ? m_bEnableMask : false*/;
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1  =  m_dAngleLow1;
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1 = m_dAngleHigh1;

		//工具2参数传递
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy2 = (Accuracy)(1<<m_nPatAccuracyMode2);
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale2 = (CoarseScale)(m_nPatSpeedMode2+1);
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable2 = m_bEnableMask2/*m_imageMask1.IsValid() ? m_bEnableMask : false*/;
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow2  =  m_dAngleLow2;
		((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh2 = m_dAngleHigh2;
		
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		//工具1界面设置
		m_nImageChannel = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nPatAccuracyMode1 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy1/2;
		m_nPatSpeedMode1 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale1-1;
		m_dAngleLow1	    = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow1;
		m_dAngleHigh1	= ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh1;
		m_bEnableMask1 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable1;	
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
		m_nPatAccuracyMode2 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy2/2;
		m_nPatSpeedMode2 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale2-1;
		m_dAngleLow2	    = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow2;
		m_dAngleHigh2	= ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh2;
		m_bEnableMask2 = ((CPatternDoubleSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable2;
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
void CDlgPatternDouleSearchTool::UpdateSearchData(bool bValidate)
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
		//工具1搜索数据
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1 = m_nSearchNum1;
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1 = m_dAcceptThre1;
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1 = m_dConfusionThre1;

		//工具2搜索数据
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum2 = m_nSearchNum2;
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2 = m_dAcceptThre2;
		((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2 = m_dConfusionThre2;

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
		//工具1搜索数据
		m_nSearchNum1		= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum1;
		m_dAcceptThre1		= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre1;	
		m_dConfusionThre1	= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre1;

		//工具2搜索数据
		m_nSearchNum2		= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum2;
		m_dAcceptThre2		= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre2;	
		m_dConfusionThre2	= ((CPatternDoubleSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre2;
		UpdateData(FALSE);
	}
}

void CDlgPatternDouleSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_PATTERNDOUBLESEARCH_TOOL") == strDlgID)
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
void CDlgPatternDouleSearchTool::CheckDlgDataValidation()
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
void CDlgPatternDouleSearchTool::OnCbnSelchangeComboPrecision1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatAccuracyMode1 = m_comboAccuracy1.GetCurSel();
}

//工具1速度设置
void CDlgPatternDouleSearchTool::OnCbnSelchangeComboSpeed1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatSpeedMode1 = m_comboSpeed1.GetCurSel();
}

//工具2精度设置
void CDlgPatternDouleSearchTool::OnCbnSelchangeComboPrecision2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatAccuracyMode2 = m_comboAccuracy2.GetCurSel();
}

//工具2速度设置
void CDlgPatternDouleSearchTool::OnCbnSelchangeComboSpeed2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPatSpeedMode2 = m_comboSpeed2.GetCurSel();
}

//获取训练图像
void CDlgPatternDouleSearchTool::OnBnClickedBtnGetImg()
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
void CDlgPatternDouleSearchTool::OnBnClickedBtnTrain()
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
	if (m_bEnableMask1)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage(m_imageMask);
	}
	if (m_bEnableMask2)
	{
		m_pDlgParent->m_pSearchTool->SetMaskImage2(m_imageMask2);
	}

	if (m_pDlgParent->m_pSearchTool->Train())
	{
		cpImage trainImage;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
		m_GuiPatternDisplay1.SetImage(trainImage, TRUE);

		m_pDlgParent->m_pSearchTool->GetTrainResultImage2(trainImage);
		m_GuiPatternDisplay2.SetImage(trainImage, TRUE);

		m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		CString strErrInfo = m_pDlgParent->m_pSearchTool->GetResultErrInfo();
		m_pDlgParent->UpdateStatusBar(false, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_FAILED),strErrInfo);
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}

//工具1提取标记点
void CDlgPatternDouleSearchTool::OnBnClickedBtnSetMark1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternDoubleSearchTrainGuiParam *pTrainGuiParam = new CPatternDoubleSearchTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint1.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint1.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect1);
		dMarkX -= pTrainGuiParam->m_TrainRect1.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect1.GetUL().GetY();

		scRect rcTrain = pTrainGuiParam->m_TrainRect1;
		sc2Vector modelPointRoi(dMarkX, dMarkY);	//ROI参考点1相对坐标
		std::vector<sc2Vector> vecModelPointRoi;
		vecModelPointRoi.push_back(modelPointRoi);
		if (!imagePattern.IsValid() || !imageTrain.IsValid() || vecModelPointRoi.empty() || rcTrain.IsEmpty())
		{
			return;
		} 
		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, vecModelPointRoi, imageTrain, rcTrain, FALSE, this);
		//CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
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
void CDlgPatternDouleSearchTool::OnBnClickedBtnSetMark2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_pSearchTool->IsTrained())
	{
		cpImage imageTrain, imagePattern;
		m_pDlgParent->m_pSearchTool->GetTrainImage(imageTrain);
		CPatternDoubleSearchTrainGuiParam *pTrainGuiParam = new CPatternDoubleSearchTrainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(pTrainGuiParam);
		double dMarkX = pTrainGuiParam->m_ModelPoint2.GetX();
		double dMarkY = pTrainGuiParam->m_ModelPoint2.GetY();
		imageTrain.GetSubImage(imagePattern, pTrainGuiParam->m_TrainRect2);
		dMarkX -= pTrainGuiParam->m_TrainRect2.GetUL().GetX();
		dMarkY -= pTrainGuiParam->m_TrainRect2.GetUL().GetY();


		scRect rcTrain = pTrainGuiParam->m_TrainRect2;
		sc2Vector modelPointRoi(dMarkX, dMarkY);	//ROI参考点1相对坐标
		std::vector<sc2Vector> vecModelPointRoi;
		vecModelPointRoi.push_back(modelPointRoi);
		if (!imagePattern.IsValid() || !imageTrain.IsValid() || vecModelPointRoi.empty() || rcTrain.IsEmpty())
		{
			return;
		} 
		CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, vecModelPointRoi, imageTrain, rcTrain, FALSE, this);
		//CDlgPatternCenterSet dlgPatternCenterSet(imagePattern, dMarkX, dMarkY, this);
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
void CDlgPatternDouleSearchTool::OnBnClickedBtnTool1Searchall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternDoubleSearchTrainGuiParam trainGuiParam;
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
void CDlgPatternDouleSearchTool::OnBnClickedBtnTool2Searchall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CPatternDoubleSearchTrainGuiParam trainGuiParam;
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
void CDlgPatternDouleSearchTool::OnBnClickedBtnSetMask1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	/*CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask.Height()) )
	{
		m_imageMask.Release();
	}*/
	cpImage tmpImage(m_pDlgParent->m_imageTrain.Width(),m_pDlgParent->m_imageTrain.Height(),epGray8);
	m_pDlgParent->m_pSearchTool->GetProcessImage(m_pDlgParent->m_pTrainDataParam,m_pDlgParent->m_imageTrain,tmpImage);

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(tmpImage);

	if ( (tmpImage.Width() != m_imageMask.Width())
		|| (tmpImage.Height() != m_imageMask.Height()) )
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

//工具2设置掩膜
void CDlgPatternDouleSearchTool::OnBnClickedBtnSetMask2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	/*CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(m_pDlgParent->m_imageTrain);

	if ( (m_pDlgParent->m_imageTrain.Width() != m_imageMask2.Width())
		|| (m_pDlgParent->m_imageTrain.Height() != m_imageMask2.Height()) )
	{
		m_imageMask2.Release();
	}*/
	cpImage tmpImage(m_pDlgParent->m_imageTrain.Width(),m_pDlgParent->m_imageTrain.Height(),epGray8);
	m_pDlgParent->m_pSearchTool->GetProcessImage(m_pDlgParent->m_pTrainDataParam,m_pDlgParent->m_imageTrain,tmpImage);

	CDlgMaskEdit dlgMask;
	dlgMask.m_GuiMaskEdit.SetRefImage(tmpImage);

	if ( (tmpImage.Width() != m_imageMask2.Width())
		|| (tmpImage.Height() != m_imageMask2.Height()) )
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

//工具1启用掩膜
void CDlgPatternDouleSearchTool::OnBnClickedCheckEnableMask1()
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
void CDlgPatternDouleSearchTool::OnBnClickedCheckEnablemask2()
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
void CDlgPatternDouleSearchTool::OnSetfocusEdtAngleLow1() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtAngleHigh1() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtSearchNum1() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtAcceptThre1() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtConfusionThre1() 
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
void CDlgPatternDouleSearchTool::OnSetfocusEdtAngleLow2() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtAngleHigh2() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtSearchNum2() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtAcceptThre2() 
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

void CDlgPatternDouleSearchTool::OnSetfocusEdtConfusionThre2() 
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


BOOL CDlgPatternDouleSearchTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}