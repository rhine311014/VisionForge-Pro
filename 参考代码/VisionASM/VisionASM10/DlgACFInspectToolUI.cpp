#include "stdafx.h"
#include "VisionASM.h"
#include <IO.H>

#include "DlgACFInspectToolUI.h "
#include "DlgSearch.h"
#include "DlgMaskEdit.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgACFInspectToolUI::CDlgACFInspectToolUI(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgACFInspectToolUI::IDD, pParent)
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
	, m_nColNum(1)
	, m_nRowNum(1)
	,m_bCurEnableGrayInspect(TRUE)
	,m_bCurEnableBlobInspect(FALSE)
	, m_nDefectType(0)
	, m_nAllowedNG(0)
	, m_dmaxGrayValue(0)
{
	m_pDlgParent = NULL;

	m_psaSysInfoStrings = NULL;

	m_nInsToolNum = 4;
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


	m_nCurToolIndex = 0;
	m_bCurEnableGrayInspect = TRUE; 
	m_bCurEnableBlobInspect = FALSE;
	m_nDefectType = 0;
	m_nBinaryThre = 80;
	m_nMinAcceptArea = 3;
	m_nMaxAcceptArea = 100000;
	m_dMaxDefectArea = 50.0;
	m_dDefectArea    = 0.0;

}

CDlgACFInspectToolUI::~CDlgACFInspectToolUI()
{
	m_pDlgParent = NULL;
	m_psaSysInfoStrings = NULL;
}

void CDlgACFInspectToolUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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

	DDX_Text(pDX, IDC_EDT_COL_NUM, m_nColNum);
	DDX_Text(pDX, IDC_EDT_ROW_NUM, m_nRowNum);

	DDX_Check(pDX, IDC_CHK_ENABLE_GRAY_INSPECT, m_bCurEnableGrayInspect);
	DDX_Check(pDX, IDC_CHK_ENABLE_BLOB_INSPECT, m_bCurEnableBlobInspect);

	DDX_Control(pDX, IDC_COMBO_DEFECT_COLOR_TYPE2, m_comboDefectType);
	DDX_Text(pDX, IDC_EDT_BINARY_THRE2, m_nBinaryThre);
	DDX_Text(pDX, IDC_EDT_MIN_AREA2, m_nMinAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_AREA2, m_nMaxAcceptArea);
	DDX_Text(pDX, IDC_EDT_MAX_DEFECT_AREA2, m_dMaxDefectArea);
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA2, m_dDefectArea);
	DDX_CBIndex(pDX, IDC_COMBO_DEFECT_COLOR_TYPE2, m_nDefectType);
	DDX_Text(pDX, IDC_EDT_COL_NUM2, m_nAllowedNG);
	DDX_Text(pDX, IDC_EDT_DEFECT_AREA3, m_dmaxGrayValue);
}


BEGIN_MESSAGE_MAP(CDlgACFInspectToolUI, CDialog)
	//{{AFX_MSG_MAP(CDlgACFInspectToolUI)
	//ON_BN_CLICKED(IDC_BTN_GET_TRAIN_IMAGE, OnBtnGetTrainImage)
	//ON_BN_CLICKED(IDC_BTN_TRAIN, OnBtnTrain)
	//ON_BN_CLICKED(IDC_BTN_FULL_IMAGE_SEARCH, OnBtnFullImageSearch)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_AREA_INDEX, &CDlgACFInspectToolUI::OnCbnSelchangeComboAreaIndex)
	ON_EN_SETFOCUS(IDC_EDT_INSTOOL_NUM, &CDlgACFInspectToolUI::OnEnSetfocusEdtInstoolNum)
	ON_EN_SETFOCUS(IDC_EDT_MIN_MEAN, &CDlgACFInspectToolUI::OnEnSetfocusEdtMinMean)
	ON_EN_SETFOCUS(IDC_EDT_MAX_MEAN, &CDlgACFInspectToolUI::OnEnSetfocusEdtMaxMean)
	ON_EN_SETFOCUS(IDC_EDT_MIN_STDDEV, &CDlgACFInspectToolUI::OnEnSetfocusEdtMinStddev)
	ON_EN_SETFOCUS(IDC_EDT_MAX_STDDEV, &CDlgACFInspectToolUI::OnEnSetfocusEdtMaxStddev)
	ON_EN_KILLFOCUS(IDC_EDT_INSTOOL_NUM, &CDlgACFInspectToolUI::OnEnKillfocusEdtInstoolNum)
	ON_EN_CHANGE(IDC_EDT_INSTOOL_NUM, &CDlgACFInspectToolUI::OnEnChangeEdtInstoolNum)
	ON_BN_CLICKED(IDC_BTN_SET_CUR_TO_ALL, &CDlgACFInspectToolUI::OnBtnSetCurToAll)
	ON_EN_SETFOCUS(IDC_EDT_COL_NUM, &CDlgACFInspectToolUI::OnEnSetfocusEdtColNum)
	ON_EN_SETFOCUS(IDC_EDT_ROW_NUM, &CDlgACFInspectToolUI::OnEnSetfocusEdtRowNum)


	ON_CBN_SELCHANGE(IDC_COMBO_DEFECT_COLOR_TYPE2, OnSelchangeComboDefectType)
	ON_EN_SETFOCUS(IDC_EDT_BINARY_THRE2, &CDlgACFInspectToolUI::OnEnSetfocusEdtBinaryThre)
	ON_EN_SETFOCUS(IDC_EDT_MIN_AREA2, &CDlgACFInspectToolUI::OnEnSetfocusEdtMinArea)
	ON_EN_SETFOCUS(IDC_EDT_MAX_AREA2, &CDlgACFInspectToolUI::OnEnSetfocusEdtMaxArea)
	ON_EN_SETFOCUS(IDC_EDT_MAX_DEFECT_AREA2, &CDlgACFInspectToolUI::OnEnSetfocusEdtMaxDefectArea)
	//}}AFX_MSG_MAP


	ON_EN_SETFOCUS(IDC_EDT_COL_NUM2, &CDlgACFInspectToolUI::OnEnSetfocusEdtColNum2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgACFInspectToolUI message handlers
void CDlgACFInspectToolUI::UpdateControlDisplay()
{	
	UpdateData(TRUE);
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_COMBO_AREA_INDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDT_INSTOOL_NUM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_AREA_INDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDT_INSTOOL_NUM)->EnableWindow(TRUE);
	}
}

void CDlgACFInspectToolUI::UpdateTrainData(bool bValidate)
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
		
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.resize(m_nInsToolNum,1);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.resize(m_nInsToolNum,99);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.resize(m_nInsToolNum,TRUE);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.resize(m_nInsToolNum,FALSE);

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.resize(m_nInsToolNum,0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.resize(m_nInsToolNum,80);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.resize(m_nInsToolNum,3);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.resize(m_nInsToolNum,100000);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.resize(m_nInsToolNum,50.0);


		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.resize(m_nInsToolNum,0.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.resize(m_nInsToolNum,255.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.resize(m_nInsToolNum,true);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.resize(m_nInsToolNum,false);

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.resize(m_nInsToolNum,0.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.resize(m_nInsToolNum,true);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.resize(m_nInsToolNum,false);

		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nInsAreaNum= m_nInsToolNum;
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.resize(m_nInsToolNum);

		m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
		m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);
		m_vnRowNum.resize(m_nInsToolNum,1);
		m_vnColNum.resize(m_nInsToolNum,99);
		m_vnAllowNGNum.resize(m_nInsToolNum,0);
		m_vnDefectType.resize(m_nInsToolNum,0);
		m_vnBinaryThre.resize(m_nInsToolNum,80);
		m_vnMinAcceptArea.resize(m_nInsToolNum,3);
		m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
		m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);

		m_dMinMean.resize(m_nInsToolNum,0.0);
		m_dMaxMean.resize(m_nInsToolNum,255.0);
		m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
		m_bMeanInvert.resize(m_nInsToolNum,FALSE);
		m_dMinStdDev.resize(m_nInsToolNum,0.0);
		m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
		m_bStdDevInvert.resize(m_nInsToolNum,FALSE);

		m_vbCurEnableGrayInspect.at(m_nCurToolIndex) = m_bCurEnableGrayInspect;
		m_vbCurEnableBlobInspect.at(m_nCurToolIndex) = m_bCurEnableBlobInspect;
		m_vnRowNum.at(m_nCurToolIndex) = m_nRowNum;
		m_vnColNum.at(m_nCurToolIndex) = m_nColNum;
		m_vnAllowNGNum.at(m_nCurToolIndex) = m_nAllowedNG;
		m_vnDefectType.at(m_nCurToolIndex) =m_nDefectType ;
		m_vnBinaryThre.at(m_nCurToolIndex) = m_nBinaryThre;
		m_vnMinAcceptArea.at(m_nCurToolIndex) = m_nMinAcceptArea;
		m_vnMaxAcceptArea.at(m_nCurToolIndex) = m_nMaxAcceptArea;
		m_vdMaxDefectArea.at(m_nCurToolIndex) = m_dMaxDefectArea;

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
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i) = m_vbCurEnableGrayInspect.at(i) ;
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i) = m_vbCurEnableBlobInspect.at(i);

			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i) = m_vnRowNum.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i) = m_vnColNum.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i) =m_vnAllowNGNum.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i) = m_vnDefectType.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i) = m_vnBinaryThre.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i) = m_vnMinAcceptArea.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i) = m_vnMaxAcceptArea.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i) = m_vdMaxDefectArea.at(i);

			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_dMinMean.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_dMaxMean.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_bMeanInvalid.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_bMeanInvert.at(i);

			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_dMinStdDev.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_dMaxStdDev.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_bStdDevInvalid.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_bStdDevInvert.at(i);
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_nInsToolNum = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;

		m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
		m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);

		m_vnRowNum.resize(m_nInsToolNum,1);
		m_vnColNum.resize(m_nInsToolNum,99);
		m_vnAllowNGNum.resize(m_nInsToolNum,0);
		m_vnDefectType.resize(m_nInsToolNum,0);
		m_vnBinaryThre.resize(m_nInsToolNum,80);
		m_vnMinAcceptArea.resize(m_nInsToolNum,3);
		m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
		m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);


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
			m_vbCurEnableGrayInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i);
			m_vbCurEnableBlobInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i);

			m_vnRowNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i);
			m_vnColNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i);
			m_vnAllowNGNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i);

			m_vnDefectType.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i);
			m_vnBinaryThre.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i);
			m_vnMinAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i);
			m_vnMaxAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i);
			m_vdMaxDefectArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i);

			m_dMinMean.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
			m_dMaxMean.at(i)=	((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
			m_bMeanInvalid.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
			m_bMeanInvert.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

			m_dMinStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
			m_dMaxStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
			m_bStdDevInvalid.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
			m_bStdDevInvert.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
		}

		m_bCurEnableGrayInspect = m_vbCurEnableGrayInspect.at(m_nCurToolIndex);
		m_bCurEnableBlobInspect = m_vbCurEnableBlobInspect.at(m_nCurToolIndex);

		m_nRowNum = m_vnRowNum.at(m_nCurToolIndex);
		m_nColNum = m_vnColNum.at(m_nCurToolIndex);
		m_nAllowedNG = m_vnAllowNGNum.at(m_nCurToolIndex);

		m_nDefectType = m_vnDefectType.at(m_nCurToolIndex);
		m_nBinaryThre = m_vnBinaryThre.at(m_nCurToolIndex);
		m_nMinAcceptArea = m_vnMinAcceptArea.at(m_nCurToolIndex);
		m_nMaxAcceptArea = m_vnMaxAcceptArea.at(m_nCurToolIndex);
		m_dMaxDefectArea = m_vdMaxDefectArea.at(m_nCurToolIndex);

		m_dCurMinMean = m_dMinMean.at(m_nCurToolIndex) ;
		m_dCurMaxMean = m_dMaxMean.at(m_nCurToolIndex)    ;
		m_bMeanNotInvalid = !m_bMeanInvalid.at(m_nCurToolIndex);
		m_bCurMeanInvert = m_bMeanInvert.at(m_nCurToolIndex) ;

		m_dCurMinStdDev = m_dMinStdDev.at(m_nCurToolIndex) ;
		m_dCurMaxStdDev = m_dMaxStdDev.at(m_nCurToolIndex) ;
		m_bCurStdDevNotInvalid = !m_bStdDevInvalid.at(m_nCurToolIndex);
		m_bCurStdDevInvert = m_bStdDevInvert.at(m_nCurToolIndex) ;

		if (((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
		}
		else
		{
			GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
		}

		m_dCurMean =  floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
		m_dCurStdDev = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
		m_dDefectArea = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurDefArea.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
		m_dmaxGrayValue = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dmaxGrayValue.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
		//m_comboDefectType.SetCurSel(m_nDefectType);
		UpdateData(FALSE);
	}
}

void CDlgACFInspectToolUI::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		m_nCurToolIndex = m_SelAreaIndex.GetCurSel();
		if (m_nCurToolIndex <=0)
		{
			m_nCurToolIndex = 0;
		}
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;


		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.resize(m_nInsToolNum,0.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.resize(m_nInsToolNum,255.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.resize(m_nInsToolNum,true);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.resize(m_nInsToolNum,false);

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.resize(m_nInsToolNum,0.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.resize(m_nInsToolNum,255.0);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.resize(m_nInsToolNum,true);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.resize(m_nInsToolNum,false);

		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nInsAreaNum= m_nInsToolNum;
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.resize(m_nInsToolNum/*,scRect(250, 200, 50, 50)*/);


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
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_dMinMean.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_dMaxMean.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_bMeanInvalid.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_bMeanInvert.at(i);

			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_dMinStdDev.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_dMaxStdDev.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_bStdDevInvalid.at(i);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_bStdDevInvert.at(i);
		}



		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{

		m_nInsToolNum = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;
		

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
			m_dMinMean.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
			m_dMaxMean.at(i)=	((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
			m_bMeanInvalid.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
			m_bMeanInvert.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

			m_dMinStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
			m_dMaxStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
			m_bStdDevInvalid.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
			m_bStdDevInvert.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
		}

		m_dCurMinMean = m_dMinMean.at(m_nCurToolIndex) ;
		m_dCurMaxMean = m_dMaxMean.at(m_nCurToolIndex)    ;
		m_bMeanNotInvalid = !m_bMeanInvalid.at(m_nCurToolIndex);
		m_bCurMeanInvert = m_bMeanInvert.at(m_nCurToolIndex) ;

		m_dCurMinStdDev = m_dMinStdDev.at(m_nCurToolIndex) ;
		m_dCurMaxStdDev = m_dMaxStdDev.at(m_nCurToolIndex) ;
		m_bCurStdDevNotInvalid = !m_bStdDevInvalid.at(m_nCurToolIndex);
		m_bCurStdDevInvert = m_bStdDevInvert.at(m_nCurToolIndex) ;

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

		UpdateData(FALSE);
	}

}

void CDlgACFInspectToolUI::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_ACF_GRAY_BLOB_INSPECT_TOOL") == strDlgID)
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

void CDlgACFInspectToolUI::CheckDlgDataValidation()
{
	UpdateData(TRUE);
	//add your validation case here
	if (m_nInsToolNum < 1 || m_nInsToolNum > MAXALLOWTOOLNUM)
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

	if (m_nDefectType < 0 || m_nDefectType>1)
	{
		m_nDefectType = 0;
	}
	if ((m_nBinaryThre < 0) || (m_nBinaryThre > 255))
	{
		m_nBinaryThre = 80;
	}
	if (m_nMinAcceptArea <0 || m_nMinAcceptArea>=m_nMaxAcceptArea || m_dMaxDefectArea < m_nMinAcceptArea)
	{
		m_nMinAcceptArea = 3;
	}

	if (m_nMaxAcceptArea <0 || m_nMinAcceptArea>=m_nMaxAcceptArea)
	{
		m_nMaxAcceptArea = 100000;
	}
	if (m_dMaxDefectArea < m_nMinAcceptArea || m_dMaxDefectArea < 0)
	{
		m_dMaxDefectArea = 50.0;
	}


	if (m_nRowNum<1 || m_nRowNum>=MAXROWNUM)
	{
		m_nRowNum = 1;
	}

	if (m_nColNum<1 || m_nColNum>=MAXCOLNUM)
	{
		m_nColNum = 1;
	}

	if (m_nAllowedNG>m_nRowNum*m_nColNum || m_nAllowedNG<0)
	{
		m_nAllowedNG = 0;
	}


	UpdateData(FALSE);
}


BOOL CDlgACFInspectToolUI::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 设置按钮

	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings; 

	UpdateDialogLanguage();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(m_psaSysInfoStrings->GetAt(0));
	}

	CString str;
	for (int i=0;i<m_nInsToolNum;i++)
	{
		str.Format("%d",i+1);
		m_SelAreaIndex.InsertString(i,str);
	}
	m_nCurToolIndex = 0;
	m_SelAreaIndex.SetCurSel(m_nCurToolIndex);


	m_comboDefectType.InsertString(0, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_WHITE));
	m_comboDefectType.InsertString(1, m_psaSysInfoStrings->GetAt(IDS_INSPCT_ACF_COLOR_BLACK));
	m_comboDefectType.SetCurSel(m_nDefectType);

	::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgACFInspectToolUI::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgACFInspectToolUI::UpdateSearchResult()
{

	if (((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
	}
	else
	{
		GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
	}

	m_nCurToolIndex = m_SelAreaIndex.GetCurSel();

	m_dCurMean = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
	m_dCurStdDev =floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
	m_dDefectArea = ((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurDefArea.at(m_nCurToolIndex);
	m_dmaxGrayValue = ((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dmaxGrayValue.at(m_nCurToolIndex);
	UpdateData(FALSE);
}


void CDlgACFInspectToolUI::OnCbnSelchangeComboAreaIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateTrainData(true);

	m_nCurToolIndex = m_SelAreaIndex.GetCurSel();

	UpdateTrainData(false);
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtInstoolNum()
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
			if (atoi(strOutput)<1 || atoi(strOutput)>MAXALLOWTOOLNUM)
			{
				strOutput.Format("%d",1);
			}
			SetDlgItemText(IDC_EDT_INSTOOL_NUM, strOutput);	
		}

		if (atoi(strOutput)<1 || atoi(strOutput)>MAXALLOWTOOLNUM)
		{
			strOutput.Format("%d",1);
		}

		if (strOutput!=_T("") && atoi(strOutput)!=m_nInsToolNum )
		{
			(CACFInspectTool*)m_pDlgParent->m_pSearchTool->ClearAllGui();
			m_nInsToolNum = atoi(strOutput);
			((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->ResizeToolsAllParam(m_nInsToolNum);

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

			(CACFInspectTool*)m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		}
	}

}


void CDlgACFInspectToolUI::OnEnSetfocusEdtMinMean()
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


void CDlgACFInspectToolUI::OnEnSetfocusEdtMaxMean()
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


void CDlgACFInspectToolUI::OnEnSetfocusEdtMinStddev()
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


void CDlgACFInspectToolUI::OnEnSetfocusEdtMaxStddev()
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


void CDlgACFInspectToolUI::OnEnKillfocusEdtInstoolNum()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CDlgACFInspectToolUI::OnEnChangeEdtInstoolNum()
{
	if (!m_pDlgParent->m_bSysKeyboardEnabled)
	{

		CString strOutput;
		GetDlgItemText(IDC_EDT_INSTOOL_NUM,strOutput);
		if (atoi(strOutput)<=0 || atoi(strOutput)>MAXALLOWTOOLNUM)
		{
			strOutput.Format("%d",1);
		}

		if (strOutput!=_T("") && atoi(strOutput)!=m_nInsToolNum)
		{
			(CACFInspectTool*)m_pDlgParent->m_pSearchTool->ClearAllGui();
			m_nInsToolNum = atoi(strOutput);
			((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->ResizeToolsAllParam(m_nInsToolNum);

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
			(CACFInspectTool*)m_pDlgParent->m_pSearchTool->ShowSearchResultGui(false);
			(CACFInspectTool*)m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();

			SetDlgItemText(IDC_EDT_INSTOOL_NUM, strOutput);	
		}

	}
}


void CDlgACFInspectToolUI::OnBtnSetCurToAll() 
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

		m_vbCurEnableGrayInspect.at(i) = m_bCurEnableGrayInspect;
		m_vbCurEnableBlobInspect.at(i) = m_bCurEnableBlobInspect;
		m_vnRowNum.at(i) = m_nRowNum;
		m_vnColNum.at(i) = m_nColNum;
		m_vnAllowNGNum.at(i) = m_nAllowedNG;
		m_vnDefectType.at(i) =m_nDefectType ;
		m_vnBinaryThre.at(i) = m_nBinaryThre;
		m_vnMinAcceptArea.at(i) = m_nMinAcceptArea;
		m_vnMaxAcceptArea.at(i) = m_nMaxAcceptArea;
		m_vdMaxDefectArea.at(i) = m_dMaxDefectArea;


		m_dMinMean.at(i) = m_dCurMinMean;
		m_dMaxMean.at(i) = m_dCurMaxMean;
		m_bMeanInvalid.at(i) = !m_bMeanNotInvalid;
		m_bMeanInvert.at(i) = m_bCurMeanInvert;

		m_dMinStdDev.at(i) = m_dCurMinStdDev;
		m_dMaxStdDev.at(i)  = m_dCurMaxStdDev;
		m_bStdDevInvalid.at(i) = !m_bCurStdDevNotInvalid;
		m_bStdDevInvert.at(i) = m_bCurStdDevInvert;

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i) = m_vbCurEnableGrayInspect.at(i) ;
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i) = m_vbCurEnableBlobInspect.at(i);

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i) = m_vnRowNum.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i) = m_vnColNum.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i) =m_vnAllowNGNum.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i) = m_vnDefectType.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i) = m_vnBinaryThre.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i) = m_vnMinAcceptArea.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i) = m_vnMaxAcceptArea.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i) = m_vdMaxDefectArea.at(i);


		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i) = m_dMinMean.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i) = m_dMaxMean.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i) = m_bMeanInvalid.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =	m_bMeanInvert.at(i);

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) = m_dMinStdDev.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) = m_dMaxStdDev.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) = m_bStdDevInvalid.at(i);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) = m_bStdDevInvert.at(i);
	}

	// 将当前仿射矩形大小设置给全部矩形 而中心点不变
	
	double tmpXL = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(m_nCurToolIndex)->GetXLength();
	double tmpYL = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(m_nCurToolIndex)->GetYLength();
	scRadian tmpR = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(m_nCurToolIndex)->GetXRotation();
	scRadian tmpskew=  ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(m_nCurToolIndex)->GetSkew();

	for (int i=0;i<m_nInsToolNum;i++)
	{
		sc2Vector tempCenter = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(i)->GetCenter();
		((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.at(i)->SetCenterLengthsRotAndSkew(tempCenter,tmpXL,tmpYL,tmpR,tmpskew);
	}

	m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
	m_pDlgParent->m_bTrainDataUpdated = TRUE;
	m_pDlgParent->m_bSearchDataUpdated = FALSE;
}

void CDlgACFInspectToolUI::OnEnSetfocusEdtColNum()
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

void CDlgACFInspectToolUI::OnEnSetfocusEdtRowNum()
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


void CDlgACFInspectToolUI::OnSelchangeComboDefectType() 
{
	m_nDefectType = m_comboDefectType.GetCurSel();	
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtBinaryThre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_BINARY_THRE2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_BINARY_THRE2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_BINARY_THRE2, strOutput);
		}
	}
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtMinArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MIN_AREA2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MIN_AREA2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MIN_AREA2, strOutput);
		}
	}
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtMaxArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_AREA2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_AREA2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_AREA2, strOutput);
		}
	}
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtMaxDefectArea()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_MAX_DEFECT_AREA2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_MAX_DEFECT_AREA2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_MAX_DEFECT_AREA2, strOutput);
		}
	}
}


void CDlgACFInspectToolUI::OnEnSetfocusEdtColNum2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_COL_NUM2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_COL_NUM2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			if (atoi(strOutput)<0 || atoi(strOutput)>100)
			{
				strOutput.Format("%d",0);
			}
			SetDlgItemText(IDC_EDT_COL_NUM2, strOutput);
		}
	}
}
