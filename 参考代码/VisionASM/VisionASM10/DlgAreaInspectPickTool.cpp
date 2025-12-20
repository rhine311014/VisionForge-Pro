// DlgAreaInspectPickTool.cpp : 实现文件
//
#include "stdafx.h"
#include "AreaInspectPickTool.h"
#include "VisionASM.h"
#include <IO.H>
#include "DlgAreaInspectPickTool.h"

#include "afxdialogex.h"
#include "DlgSearch.h"
#include "DlgMaskEdit.h"

// CDlgAreaInspectPickTool 对话框

IMPLEMENT_DYNAMIC(CDlgAreaInspectPickTool, CDialog)

CDlgAreaInspectPickTool::CDlgAreaInspectPickTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAreaInspectPickTool::IDD, pParent)
	, m_nInsToolNum(0)
	, m_dCurMean(0)
	, m_dCurStdDev(0)
	, m_dCurMinMean(0)
	, m_dCurMaxMean(255)
	, m_bMeanNotInvalid(FALSE)
	, m_bCurMeanInvert(FALSE)
	, m_dCurMinStdDev(0)
	, m_dCurMaxStdDev(255)
	, m_bCurStdDevNotInvalid(FALSE)
	, m_bCurStdDevInvert(FALSE)
	, m_nCurLogicIndex(0)
	, m_nImageChannel(0)
	, m_nColNum(1)
	, m_nRowNum(1)
{
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;

	m_nInsToolNum = 6;
	m_dCurMean = 0.0;
	m_dCurStdDev= 0.0;
	m_dCurMinMean= 0.0;
	m_dCurMaxMean= 255.0;
	m_bMeanNotInvalid = FALSE;
	m_bCurMeanInvert = FALSE;
	m_dCurMinStdDev= 0.0;
	m_dCurMaxStdDev= 255.0;
	m_bCurStdDevNotInvalid = FALSE;
	m_bCurStdDevInvert = FALSE;
	m_nCurLogicIndex = 0;

	m_nCurToolIndex = 0;
}

CDlgAreaInspectPickTool::~CDlgAreaInspectPickTool()
{
}

void CDlgAreaInspectPickTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAreaInspectPickTool)
	DDX_Control(pDX, IDC_BTN_TRAIN, m_btnTrain);
	DDX_Control(pDX, IDC_BTN_GET_TRAIN_IMAGE, m_btnGetTrainImage);
	DDX_Control(pDX, IDC_COMBO_AREA_INDEX, m_SelAreaIndex);
	DDX_Text(pDX, IDC_EDT_INSTOOL_NUM, m_nInsToolNum);
	DDX_Text(pDX, IDC_EDT_CUR_MEAN, m_dCurMean);
	DDX_Text(pDX, IDC_EDT_CUR_STDDEV, m_dCurStdDev);
	DDX_Text(pDX, IDC_EDT_MIN_MEAN, m_dCurMinMean);
	DDV_MinMaxDouble(pDX, m_dCurMinMean, 0, 255);
	DDX_Text(pDX, IDC_EDT_MAX_MEAN, m_dCurMaxMean);
	DDV_MinMaxDouble(pDX, m_dCurMaxMean, 0, 255);
	DDX_Check(pDX, IDC_CHK_MEAN_NOTINVALID, m_bMeanNotInvalid);
	DDX_Check(pDX, IDC_CHK_MEAN_INVERT, m_bCurMeanInvert);
	DDX_Text(pDX, IDC_EDT_MIN_STDDEV, m_dCurMinStdDev);
	DDV_MinMaxDouble(pDX, m_dCurMinStdDev, 0, 255);
	DDX_Text(pDX, IDC_EDT_MAX_STDDEV, m_dCurMaxStdDev);
	DDV_MinMaxDouble(pDX, m_dCurMaxStdDev, 0, 255);
	DDX_Check(pDX, IDC_CHK_STDDEV_NOTINVALID, m_bCurStdDevNotInvalid);
	DDX_Check(pDX, IDC_CHK_STDDEV_INVERT, m_bCurStdDevInvert);
	DDX_Radio(pDX, IDC_RADIO_LOGIC_AND, m_nCurLogicIndex);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_CHANNEL_GRAY, m_nImageChannel);
	DDX_Text(pDX, IDC_EDT_COL_NUM, m_nColNum);
	DDX_Text(pDX, IDC_EDT_ROW_NUM, m_nRowNum);
}


BEGIN_MESSAGE_MAP(CDlgAreaInspectPickTool, CDialog)
	//{{AFX_MSG_MAP(CDlgAreaInspectPickTool)
	ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_AREA_INDEX, &CDlgAreaInspectPickTool::OnCbnSelchangeComboAreaIndex)
	ON_EN_SETFOCUS(IDC_EDT_INSTOOL_NUM, &CDlgAreaInspectPickTool::OnEnSetfocusEdtInstoolNum)
	ON_EN_SETFOCUS(IDC_EDT_MIN_MEAN, &CDlgAreaInspectPickTool::OnEnSetfocusEdtMinMean)
	ON_EN_SETFOCUS(IDC_EDT_MAX_MEAN, &CDlgAreaInspectPickTool::OnEnSetfocusEdtMaxMean)
	ON_EN_SETFOCUS(IDC_EDT_MIN_STDDEV, &CDlgAreaInspectPickTool::OnEnSetfocusEdtMinStddev)
	ON_EN_SETFOCUS(IDC_EDT_MAX_STDDEV, &CDlgAreaInspectPickTool::OnEnSetfocusEdtMaxStddev)
	ON_EN_KILLFOCUS(IDC_EDT_INSTOOL_NUM, &CDlgAreaInspectPickTool::OnEnKillfocusEdtInstoolNum)
	ON_EN_CHANGE(IDC_EDT_INSTOOL_NUM, &CDlgAreaInspectPickTool::OnEnChangeEdtInstoolNum)
	ON_BN_CLICKED(IDC_BTN_SET_CUR_TO_ALL, &CDlgAreaInspectPickTool::OnBtnSetCurToAll)
	ON_EN_SETFOCUS(IDC_EDT_COL_NUM, &CDlgAreaInspectPickTool::OnEnSetfocusEdtColNum)
	ON_EN_SETFOCUS(IDC_EDT_ROW_NUM, &CDlgAreaInspectPickTool::OnEnSetfocusEdtRowNum)
	ON_BN_CLICKED(IDC_BTN_AUTO_ARRANGE, &CDlgAreaInspectPickTool::OnBtnAutoArrange)
END_MESSAGE_MAP()


// CDlgAreaInspectPickTool 消息处理程序
/////////////////////////////////////////////////////////////////////////////
// CDlgAreaInspectPickTool message handlers
void CDlgAreaInspectPickTool::UpdateControlDisplay()
{	
	UpdateData(TRUE);

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_AREA_INDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_INSTOOL_NUM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_AREA_INDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_INSTOOL_NUM)->EnableWindow(TRUE);
	}

	if (m_pDlgParent->m_pSearchTool != NULL)
	{
		if (m_pDlgParent->m_imageTrain.IsValid())
		{
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
		GetDlgItem(IDC_RADIO_CHANNEL_GRAY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_R)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_G)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_CHANNEL_B)->ShowWindow(SW_HIDE);
	}
}

void CDlgAreaInspectPickTool::UpdateTrainData(bool bValidate)
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
		//		m_nDefectType = m_comboDefectType.GetCurSel()+1;
		//		m_nCurToolIndex = m_SelAreaIndex.GetCurSel();
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel= m_nImageChannel;
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLogicIndex = m_nCurLogicIndex;
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum = m_nColNum;
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum = m_nRowNum;

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.resize(m_nInsToolNum,0.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.resize(m_nInsToolNum,255.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.resize(m_nInsToolNum,false);

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.resize(m_nInsToolNum,0.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.resize(m_nInsToolNum,false);

		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bSearchRectVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_LineColor.resize(m_nInsToolNum,RGB(0, 0, 255));
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nLineWidth.resize(m_nInsToolNum,1);
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.resize(m_nInsToolNum/*,scRect(250, 200, 50, 50)*/);

		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bAffineRectVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bModelPointVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_LineColor.resize(m_nInsToolNum,RGB(0, 0, 255));
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_nLineWidth.resize(m_nInsToolNum,1);

		m_dMinMean.resize(m_nInsToolNum,0.0);
		m_dMaxMean.resize(m_nInsToolNum,255.0);
		m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
		m_bMeanInvert.resize(m_nInsToolNum,FALSE);
		m_dMinStdDev.resize(m_nInsToolNum,0.0);
		m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
		m_bStdDevInvert.resize(m_nInsToolNum,FALSE);

		m_dMinMean.at(m_nCurToolIndex)    = m_dCurMinMean;
		m_dMaxMean.at(m_nCurToolIndex)    = m_dCurMaxMean;
		m_bMeanInvalid.at(m_nCurToolIndex)= !m_bMeanNotInvalid;
		m_bMeanInvert.at(m_nCurToolIndex) = m_bCurMeanInvert;

		m_dMinStdDev.at(m_nCurToolIndex)  = m_dCurMinStdDev;
		m_dMaxStdDev.at(m_nCurToolIndex)  = m_dCurMaxStdDev;
		m_bStdDevInvalid.at(m_nCurToolIndex) = !m_bCurStdDevNotInvalid;
		m_bStdDevInvert.at(m_nCurToolIndex)  = m_bCurStdDevInvert;

		for (int i=0;i<m_nInsToolNum;i++)
		{
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_dMinMean.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_dMaxMean.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_bMeanInvalid.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_bMeanInvert.at(i);

			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_dMinStdDev.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_dMaxStdDev.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_bStdDevInvalid.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_bStdDevInvert.at(i);
		}



		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		//		m_nCurToolIndex = m_SelAreaIndex.GetCurSel();
		m_nImageChannel = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
		m_nInsToolNum = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;
		m_nCurLogicIndex = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLogicIndex;
		m_nColNum = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum;
		m_nRowNum = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum;

		m_dMinMean.resize(m_nInsToolNum,0.0);
		m_dMaxMean.resize(m_nInsToolNum,255.0);
		m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
		m_bMeanInvert.resize(m_nInsToolNum,FALSE);

		m_dMinStdDev.resize(m_nInsToolNum,0.0);
		m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
		m_bStdDevInvert.resize(m_nInsToolNum,FALSE);


		for (int i=0;i<m_nInsToolNum;i++)
		{
			m_dMinMean.at(i) = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
			m_dMaxMean.at(i)=	((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
			m_bMeanInvalid.at(i)=((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
			m_bMeanInvert.at(i)=((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

			m_dMinStdDev.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
			m_dMaxStdDev.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
			m_bStdDevInvalid.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
			m_bStdDevInvert.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
		}

		m_dCurMinMean = m_dMinMean.at(m_nCurToolIndex) ;
		m_dCurMaxMean = m_dMaxMean.at(m_nCurToolIndex)    ;
		m_bMeanNotInvalid = !m_bMeanInvalid.at(m_nCurToolIndex);
		m_bCurMeanInvert = m_bMeanInvert.at(m_nCurToolIndex) ;

		m_dCurMinStdDev = m_dMinStdDev.at(m_nCurToolIndex) ;
		m_dCurMaxStdDev = m_dMaxStdDev.at(m_nCurToolIndex) ;
		m_bCurStdDevNotInvalid = !m_bStdDevInvalid.at(m_nCurToolIndex);
		m_bCurStdDevInvert = m_bStdDevInvert.at(m_nCurToolIndex) ;

		if (((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
		}
		else
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
		}

		m_dCurMean =  floor(((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
		m_dCurStdDev = floor(((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;

		//		m_comboDefectType.SetCurSel(m_nDefectType-1);
		UpdateData(FALSE);
	}
}

void CDlgAreaInspectPickTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		// 		UpdateData(TRUE);
		// 		m_nDefectType = m_comboDefectType.GetCurSel()+1;
		// 		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType= m_nDefectType;
		// 		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre = m_nBinaryThre;
		// 		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea = m_nMinAcceptArea/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
		// 		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea  =  m_nMaxAcceptArea;
		// 		((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea = m_dMaxDefectArea;
		// 		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		// 		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		// 		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		// 		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		// 		m_pDlgParent->m_bSearchDataUpdated = FALSE;
		m_nCurToolIndex = m_SelAreaIndex.GetCurSel();
		if (m_nCurToolIndex <=0)
		{
			m_nCurToolIndex = 0;
		}
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLogicIndex = m_nCurLogicIndex;

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.resize(m_nInsToolNum,0.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.resize(m_nInsToolNum,255.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.resize(m_nInsToolNum,false);

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.resize(m_nInsToolNum,0.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.resize(m_nInsToolNum,false);

		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bSearchRectVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_LineColor.resize(m_nInsToolNum,RGB(0, 0, 255));
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nLineWidth.resize(m_nInsToolNum,1);
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.resize(m_nInsToolNum/*,scRect(250, 200, 50, 50)*/);

		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bAffineRectVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bModelPointVisible.resize(m_nInsToolNum,true);
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_LineColor.resize(m_nInsToolNum,RGB(0, 0, 255));
		((CAreaInspectPickSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_nLineWidth.resize(m_nInsToolNum,1);

		m_dMinMean.at(m_nCurToolIndex)    = m_dCurMinMean;
		m_dMaxMean.at(m_nCurToolIndex)    = m_dCurMaxMean;
		m_bMeanInvalid.at(m_nCurToolIndex)= !m_bMeanNotInvalid;
		m_bMeanInvert.at(m_nCurToolIndex) = m_bCurMeanInvert;

		m_dMinStdDev.at(m_nCurToolIndex)  = m_dCurMinStdDev;
		m_dMaxStdDev.at(m_nCurToolIndex)  = m_dCurMaxStdDev;
		m_bStdDevInvalid.at(m_nCurToolIndex) = !m_bCurStdDevNotInvalid;
		m_bStdDevInvert.at(m_nCurToolIndex)  = m_bCurStdDevInvert;

		for (int i=0;i<m_nInsToolNum;i++)
		{
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_dMinMean.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_dMaxMean.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_bMeanInvalid.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_bMeanInvert.at(i);

			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_dMinStdDev.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_dMaxStdDev.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_bStdDevInvalid.at(i);
			((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_bStdDevInvert.at(i);
		}



		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		// 		m_nDefectType = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType;
		// 		m_nBinaryThre = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre;
		// 		m_nMinAcceptArea	    = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea;
		// 		m_nMaxAcceptArea	= ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea;
		// 		m_dMaxDefectArea = ((CAreaInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea;
		//		m_nCurToolIndex = m_SelAreaIndex.GetCurSel();
		m_nInsToolNum = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;
		m_nCurLogicIndex = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nLogicIndex;

		m_dMinMean.resize(m_nInsToolNum,0.0);
		m_dMaxMean.resize(m_nInsToolNum,255.0);
		m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
		m_bMeanInvert.resize(m_nInsToolNum,FALSE);

		m_dMinStdDev.resize(m_nInsToolNum,0.0);
		m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
		m_bStdDevInvert.resize(m_nInsToolNum,FALSE);


		for (int i=0;i<m_nInsToolNum;i++)
		{
			m_dMinMean.at(i) = ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
			m_dMaxMean.at(i)=	((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
			m_bMeanInvalid.at(i)=((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
			m_bMeanInvert.at(i)=((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

			m_dMinStdDev.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
			m_dMaxStdDev.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
			m_bStdDevInvalid.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
			m_bStdDevInvert.at(i)= ((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
		}

		m_dCurMinMean = m_dMinMean.at(m_nCurToolIndex) ;
		m_dCurMaxMean = m_dMaxMean.at(m_nCurToolIndex)    ;
		m_bMeanNotInvalid = !m_bMeanInvalid.at(m_nCurToolIndex);
		m_bCurMeanInvert = m_bMeanInvert.at(m_nCurToolIndex) ;

		m_dCurMinStdDev = m_dMinStdDev.at(m_nCurToolIndex) ;
		m_dCurMaxStdDev = m_dMaxStdDev.at(m_nCurToolIndex) ;
		m_bCurStdDevNotInvalid = !m_bStdDevInvalid.at(m_nCurToolIndex);
		m_bCurStdDevInvert = m_bStdDevInvert.at(m_nCurToolIndex) ;

		//m_nInsToolNum = MAXTOOLNUM;
		//	m_nCurToolIndex = MAXTOOLNUM-1;
		CString str;
		m_SelAreaIndex.ResetContent();
		for (int i=0;i<m_nInsToolNum;i++)
		{
			str.Format("%d",i+1);
			m_SelAreaIndex.InsertString(i,str);
		}
		if (m_nCurToolIndex>=m_SelAreaIndex.GetCount())
		{
			m_nCurToolIndex = 0;
		}

		m_SelAreaIndex.SetCurSel(m_nCurToolIndex);

		// 		if (((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
		// 		{
		// 			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
		// 		}
		// 		else
		// 		{
		// 			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
		// 		}
		// 
		// 		m_dDefectArea = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_dDefectArea;
		// 		m_nMaxGreyVal = ((CAreaInspectTool*)m_pDlgParent->m_pSearchTool)->m_nMaxGreyVal;

		//		m_comboDefectType.SetCurSel(m_nDefectType-1);
		UpdateData(FALSE);
	}

}

void CDlgAreaInspectPickTool::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
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

		if(_T("IDD_DLG_AREAINSPECT_TOOLEX") == strDlgID)
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

void CDlgAreaInspectPickTool::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
	if (m_nInsToolNum < 1 || m_nInsToolNum > 100)
	{
		m_nInsToolNum = 1;
	}
	if ((m_dCurMinMean < 0) || (m_dCurMinMean > 255))
	{
		m_dCurMinMean = 0.0;
	}
	if ((m_dCurMaxMean < 0) || (m_dCurMaxMean > 255))
	{
		m_dCurMaxMean = 255.0;
	}
	if ((m_dCurMinStdDev < 0) || (m_dCurMinStdDev > 255))
	{
		m_dCurMinStdDev = 0.0;
	}
	if ((m_dCurMaxStdDev < 0) || (m_dCurMaxStdDev > 255))
	{
		m_dCurMaxStdDev = 255.0;
	}


	UpdateData(FALSE);
}

void CDlgAreaInspectPickTool::OnBtnGetTrainImage() 
{
	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	UpdateData(TRUE);
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;

	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}

void CDlgAreaInspectPickTool::OnBtnTrain() 
{
	// TODO: Add your control notification handler code here
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}
	if (!m_pDlgParent->m_imageTrain.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_FAILED);		//_T("训练失败");
		m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_INVALID_TRAIN_IMAGE);	//_T("训练图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}

	CheckDlgDataValidation();
	UpdateTrainData(true);

	m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);


	if (m_pDlgParent->m_pSearchTool->Train())
	{
		cpImage trainImage;
		m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
		//m_GuiPatternDisplay.SetImage(trainImage, TRUE);
		m_pDlgParent->UpdateStatusBar(true, m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_SUCCEED));
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else
	{
		m_pDlgParent->UpdateStatusBar(false, m_psaSysInfoStrings->GetAt(IDS_SB_TRAIN_FAILED));
		m_pDlgParent->m_bTrainDataUpdated = FALSE;
	}
}






BOOL CDlgAreaInspectPickTool::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置按钮

	m_btnGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings; 

	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
	}



	//m_GuiPatternDisplay.Create(IDC_STC_PATTERN_DISPLAY, this, 0);
	//m_GuiPatternDisplay.SetBackgroundParam(bgDispBGPure,RGB(10,10,10));

	// 	switch (lgidLanguage)
	// 	{
	// 	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
	// 		m_GuiPatternDisplay.SetLanguage(0);
	// 		break;
	// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	// 		m_GuiPatternDisplay.SetLanguage(1);
	// 		break;
	// 	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	// 	default:
	// 		m_GuiPatternDisplay.SetLanguage(1);
	// 		break;
	//	}
	//	m_GuiPatternDisplay.SetBackgroundColor(RGB(0,0,0));

	//	m_comboDefectType.InsertString(0, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_WHITE));
	//	m_comboDefectType.InsertString(1, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_BLACK));
	//	m_comboDefectType.SetCurSel(m_nDefectType-1);
	// 	m_nInsToolNum = MAXTOOLNUM;
	// //	m_nCurToolIndex = MAXTOOLNUM-1;
	CString str;
	for (int i=0;i<m_nInsToolNum;i++)
	{
		str.Format("%d",i+1);
		m_SelAreaIndex.InsertString(i,str);
	}
	m_nCurToolIndex = 0;
	m_SelAreaIndex.SetCurSel(m_nCurToolIndex);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}






void CDlgAreaInspectPickTool::OnBtnFullImageSearch() 
{
	if (m_pDlgParent->m_imageTrain.IsValid())
	{
		CAreaInspectPickTrainGuiParam trainGuiParam;
		m_pDlgParent->m_pSearchTool->GetTrainGuiParam(&trainGuiParam);
		trainGuiParam.m_SearchRect.at(m_nCurToolIndex)->Set(0, 0, m_pDlgParent->m_imageTrain.Width(), m_pDlgParent->m_imageTrain.Height());
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(&trainGuiParam);
		m_pDlgParent->UpdateGuiDisplay();
		// 如果定位工具已经训练，则自动重新训练；否则，不训练，等待用户执行训练操作。
		if (m_pDlgParent->m_pSearchTool->IsTrained())
		{
			if (m_pDlgParent->m_pSearchTool->Train())
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_FULL_IMAGE_SEARCH_SUCCEED_AND_RETRAINED);	//_T("全图搜索设置成功，模板已重新训练");
				m_pDlgParent->m_strStatusBarInfo2 = _T("");
			}
			else
			{
				m_pDlgParent->m_bStatusBarFlag = TRUE;
				m_pDlgParent->m_strStatusBarInfo1 = m_psaSysInfoStrings->GetAt(IDS_SB_FULL_IMAGE_SEARCH_FAILED);	//_T("全图搜索设置失败");
				m_pDlgParent->m_strStatusBarInfo2 = m_psaSysInfoStrings->GetAt(IDS_SB_PATTERN_RETRAIN_FAILED);		//_T("模板重新训练失败");
			}
			m_pDlgParent->UpdateStatusBar();
		}
	}
}

BOOL CDlgAreaInspectPickTool::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAreaInspectPickTool::UpdateSearchResult()
{

	if (((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
	}
	else
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
	}

	m_nCurToolIndex = m_SelAreaIndex.GetCurSel();

	m_dCurMean = floor(((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.0f+0.5)/100.0f;
	m_dCurStdDev =floor( ((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.0f+0.5)/100.0f;


	UpdateData(FALSE);
}


void CDlgAreaInspectPickTool::OnCbnSelchangeComboAreaIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);

	m_nCurToolIndex = m_SelAreaIndex.GetCurSel();

	UpdateTrainData(false);
}


void CDlgAreaInspectPickTool::OnEnSetfocusEdtInstoolNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_INSTOOL_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_INSTOOL_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			if (atoi(strOutput)<1 || atoi(strOutput)>100)
			{
				strOutput.Format("%d",1);
			}
			SetDlgItemText(IDC_EDT_INSTOOL_NUM, strOutput);	
		}		

		if (strOutput!=_T("") && atoi(strOutput)!=m_nInsToolNum)
		{
			(CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool->ClearAllGui();
			m_nInsToolNum = atoi(strOutput);
			((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->ResizeToolsAllParam(m_nInsToolNum);

			m_dMinMean.resize(m_nInsToolNum,0.0); 
			m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_dMinStdDev.resize(m_nInsToolNum,0.0); 
			m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_bMeanInvert.resize(m_nInsToolNum,FALSE);    
			m_bStdDevInvert.resize(m_nInsToolNum,FALSE);  

			m_SelAreaIndex.ResetContent();
			CString str;
			for (int i=0;i<m_nInsToolNum;i++)
			{
				str.Format("%d",i+1);
				m_SelAreaIndex.InsertString(i,str);
			}
			if (m_nCurToolIndex>=m_SelAreaIndex.GetCount())
			{
				m_nCurToolIndex = 0;

			}
			m_SelAreaIndex.SetCurSel(m_nCurToolIndex);	

			UpdateTrainData(false);

			(CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		}
	}

}


void CDlgAreaInspectPickTool::OnEnSetfocusEdtMinMean()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MIN_MEAN)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MIN_MEAN, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MIN_MEAN, strOutput);
		}
	}
}


void CDlgAreaInspectPickTool::OnEnSetfocusEdtMaxMean()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_MEAN)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_MEAN, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_MEAN, strOutput);
		}
	}
}


void CDlgAreaInspectPickTool::OnEnSetfocusEdtMinStddev()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MIN_STDDEV)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MIN_STDDEV, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MIN_STDDEV, strOutput);
		}
	}
}


void CDlgAreaInspectPickTool::OnEnSetfocusEdtMaxStddev()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_STDDEV)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_STDDEV, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_STDDEV, strOutput);
		}
	}
}


void CDlgAreaInspectPickTool::OnEnKillfocusEdtInstoolNum()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CDlgAreaInspectPickTool::OnEnChangeEdtInstoolNum()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{

		CString strOutput;
		GetDlgItemText(IDC_EDT_INSTOOL_NUM,strOutput);
		if (atoi(strOutput)<=0 || atoi(strOutput)>100)
		{
			strOutput.Format("%d",1);
		}

		if (strOutput!=_T("") && atoi(strOutput)!=m_nInsToolNum)
		{
			(CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool->ClearAllGui();
			m_nInsToolNum = atoi(strOutput);
			((CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool)->ResizeToolsAllParam(m_nInsToolNum);

			m_dMinMean.resize(m_nInsToolNum,0.0); 
			m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_dMinStdDev.resize(m_nInsToolNum,0.0); 
			m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_bMeanInvert.resize(m_nInsToolNum,FALSE);    
			m_bStdDevInvert.resize(m_nInsToolNum,FALSE);  

			m_SelAreaIndex.ResetContent();
			CString str;
			for (int i=0;i<m_nInsToolNum;i++)
			{
				str.Format("%d",i+1);
				m_SelAreaIndex.InsertString(i,str);
			}
			if (m_nCurToolIndex>=m_SelAreaIndex.GetCount())
			{
				m_nCurToolIndex = 0;

			}
			m_SelAreaIndex.SetCurSel(m_nCurToolIndex);	

			UpdateTrainData(false);

			(CAreaInspectToolEx*)m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

			SetDlgItemText(IDC_EDT_INSTOOL_NUM, strOutput);	
		}

	}
}


void CDlgAreaInspectPickTool::OnBtnSetCurToAll() 
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	// 将当前区域参数设置给全部区域
	UpdateData(TRUE);
	for (int i=0;i<m_nInsToolNum;i++)
	{
		m_dMinMean.at(i) = m_dCurMinMean;
		m_dMaxMean.at(i) = m_dCurMaxMean;
		m_bMeanInvalid.at(i) = !m_bMeanNotInvalid;
		m_bMeanInvert.at(i) = m_bCurMeanInvert;

		m_dMinStdDev.at(i) = m_dCurMinStdDev;
		m_dMaxStdDev.at(i)  = m_dCurMaxStdDev;
		m_bStdDevInvalid.at(i) = !m_bCurStdDevNotInvalid;
		m_bStdDevInvert.at(i) = m_bCurStdDevInvert;

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i) = m_dMinMean.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i) = m_dMaxMean.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i) = m_bMeanInvalid.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =	m_bMeanInvert.at(i);

		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) = m_dMinStdDev.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) = m_dMaxStdDev.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) = m_bStdDevInvalid.at(i);
		((CAreaInspectPickTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) = m_bStdDevInvert.at(i);
	}

	// 将当前矩形大小设置给全部矩形
	sc2Vector tempRectSize = ((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(m_nCurToolIndex)->GetSize();
	for (int i=0;i<m_nInsToolNum;i++)
	{
		((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i)->SetSize(tempRectSize);
	}

	m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	m_pDlgParent->m_bTrainDataUpdated = TRUE;
	m_pDlgParent->m_bSearchDataUpdated = FALSE;
}

void CDlgAreaInspectPickTool::OnEnSetfocusEdtColNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_COL_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_COL_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			if (atoi(strOutput)<1 || atoi(strOutput)>100)
			{
				strOutput.Format("%d",1);
			}
			SetDlgItemText(IDC_EDT_COL_NUM, strOutput);
		}
	}
}

void CDlgAreaInspectPickTool::OnEnSetfocusEdtRowNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_ROW_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_ROW_NUM, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			if (atoi(strOutput)<1 || atoi(strOutput)>100)
			{
				strOutput.Format("%d",1);
			}
			SetDlgItemText(IDC_EDT_ROW_NUM, strOutput);
		}
	}
}

void CDlgAreaInspectPickTool::OnBtnAutoArrange() 
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	UpdateData(TRUE);

	if (m_nColNum<1 || m_nRowNum<1)
	{
		return;
	}

	if (m_nColNum * m_nRowNum != m_nInsToolNum)
	{
		return;
	}

	if (m_nColNum * m_nRowNum == 1)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	sc2Vector tempRectOriginFirst = ((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(0)->GetOrigin();
	sc2Vector tempRectOriginEnd = ((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(m_nInsToolNum-1)->GetOrigin();

	double dSeparatorX = 0.0;
	double dSeparatorY = 0.0;
	if (m_nRowNum == 1)
	{
		dSeparatorX = (tempRectOriginEnd.GetX() - tempRectOriginFirst.GetX())/(m_nColNum - 1);
		dSeparatorY = 0;
	}
	else if (m_nColNum == 1)
	{
		dSeparatorX = 0;
		dSeparatorY = (tempRectOriginEnd.GetY() - tempRectOriginFirst.GetY())/(m_nRowNum - 1);
	}
	else
	{
		dSeparatorX = (tempRectOriginEnd.GetX() - tempRectOriginFirst.GetX())/(m_nColNum - 1);
		dSeparatorY = (tempRectOriginEnd.GetY() - tempRectOriginFirst.GetY())/(m_nRowNum - 1);
	}

	if (m_nRowNum == 1)
	{
		for (int i=0;i<m_nColNum; i++)
		{
			double dOriginX = tempRectOriginFirst.GetX() + dSeparatorX*i;
			double dOriginY = tempRectOriginFirst.GetY();
			((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i)->SetOrigion(sc2Vector(dOriginX, dOriginY));
		}
	}
	else if (m_nColNum == 1)
	{
		for (int i=0;i<m_nRowNum;i++)
		{
			double dOriginX = tempRectOriginFirst.GetX();
			double dOriginY = tempRectOriginFirst.GetY() + dSeparatorY*i;
			//((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i*m_nColNum + j)->SetSize(((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(0).GetSize());
			auto size= ((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(0)->GetSize();
			((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i)->SetSize(size);
			((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i)->SetOrigion(sc2Vector(dOriginX, dOriginY));
		}
	}
	else
	{
		for (int i=0;i<m_nRowNum;i++)
		{
			for (int j=0;j<m_nColNum; j++)
			{
				double dOriginX = tempRectOriginFirst.GetX() + dSeparatorX*j;
				double dOriginY = tempRectOriginFirst.GetY() + dSeparatorY*i;
				auto size= ((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(0)->GetSize();
				((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i*m_nColNum + j)->SetSize(size);

				((CAreaInspectPickTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_SearchRect.at(i*m_nColNum + j)->SetOrigion(sc2Vector(dOriginX, dOriginY));
			}
		}
	}

	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	m_pDlgParent->m_bTrainDataUpdated = TRUE;
	m_pDlgParent->m_bSearchDataUpdated = FALSE;
}