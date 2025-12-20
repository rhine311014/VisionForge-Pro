// DlgSpecialPickSearchTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSpecialPickSearchTool.h"
#include "afxdialogex.h"
#include "DlgSearch.h"

// CDlgSpecialPickSearchTool 对话框

IMPLEMENT_DYNAMIC(CDlgSpecialPickSearchTool, CDialogEx)

CDlgSpecialPickSearchTool::CDlgSpecialPickSearchTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSpecialPickSearchTool::IDD, pParent)
	, m_nRoughSearchToolTypeIndex(0)
	, m_nAccurateSearchToolTypeIndex(0)
	, m_nHeadTailCheckToolTypeIndex(2)
	, m_bEnableAccurateLoation(TRUE)
	, m_bEnableHeadTailCheck(FALSE)
	, m_nHeadTailFailTpye(2)
	, m_nImageChannel(0)
{
	m_TabIndex=0;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;

	m_bCoordSortEnable = FALSE;
	m_dCoordSortGap = 64;
	m_nCoordSortMethod = 0;
	m_nCoordSortOrderX = 0;
	m_nCoordSortOrderY = 0;

}

CDlgSpecialPickSearchTool::~CDlgSpecialPickSearchTool()
{
}

void CDlgSpecialPickSearchTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB, m_cTabCtl);
	DDX_Control(pDX, IDC_BTN_MARK_TRAIN, m_btnMarkTrain);
	DDX_Control(pDX, IDC_BTN_MARK_GETTRAINIMAGE, m_btnMarkGetTrainImage);

	DDX_CBIndex(pDX, IDC_CMB_ROUGH_SEARCHTOOL_TYPE, m_nRoughSearchToolTypeIndex);
	DDX_CBIndex(pDX, IDC_CMB_ACCURATE_SEARCHTOOL_TYPE, m_nAccurateSearchToolTypeIndex);
	DDX_CBIndex(pDX, IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE, m_nHeadTailCheckToolTypeIndex);


	DDX_Control(pDX, IDC_CMB_ROUGH_SEARCHTOOL_TYPE, m_cmbRouchSearchtoolType);
	DDX_Control(pDX, IDC_CMB_ACCURATE_SEARCHTOOL_TYPE, m_cmbAccurateSearchType);
	DDX_Control(pDX, IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE, m_cmbHeadTailCheckToolType);

	DDX_Check(pDX, IDC_CHK_ENABLE_ACCURATE_LOCATION, m_bEnableAccurateLoation);
	DDX_Check(pDX, IDC_CHK_ENABLE_HEADTAIL_CHECK, m_bEnableHeadTailCheck);

	DDX_Check(pDX, IDC_CHK_COORD_SORT_ENABLE, m_bCoordSortEnable);
	DDX_Text(pDX, IDC_EDT_COORD_SORT_GAP2, m_dCoordSortGap);
	DDX_Control(pDX, IDC_COMBO_COORD_SORT_METHOD, m_comboCoordSortMethod);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_X_DE, m_nCoordSortOrderX);
	DDX_Radio(pDX, IDC_RADIO_COORD_SORT_ORDER_Y_DE, m_nCoordSortOrderY);

	//DDX_Radio(pDX, IDC_RADIO_HEADTAIL_HANDLE_TPYE1, m_nHeadTailFailTpye);
	DDX_CBIndex(pDX,IDC_COMBO_HEADTAILFAILTYPE,m_nHeadTailFailTpye);
	DDX_Control(pDX,IDC_COMBO_HEADTAILFAILTYPE,m_comboHeadTailFailTpe);
}


BEGIN_MESSAGE_MAP(CDlgSpecialPickSearchTool, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgSpecialPickSearchTool::OnTcnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_CMB_ROUGH_SEARCHTOOL_TYPE, &CDlgSpecialPickSearchTool::OnCbnSelchangeCmbRouchSearchtoolType)
	ON_CBN_SELCHANGE(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE, &CDlgSpecialPickSearchTool::OnCbnSelchangeCmbAccurateSearchToolType)
	ON_CBN_SELCHANGE(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE, &CDlgSpecialPickSearchTool::OnCbnSelchangeCmbHeadTailCheckToolType)
	ON_BN_CLICKED(IDC_BTN_MARK_GETTRAINIMAGE, &CDlgSpecialPickSearchTool::OnBnClickedBtnMarkGettrainimage)
	ON_BN_CLICKED(IDC_BTN_MARK_TRAIN, &CDlgSpecialPickSearchTool::OnBnClickedBtnMarkTrain)
	ON_BN_CLICKED(IDC_CHK_ENABLE_ACCURATE_LOCATION, &CDlgSpecialPickSearchTool::OnBnClickedChkEnableAccurateLocation)
	ON_BN_CLICKED(IDC_CHK_ENABLE_HEADTAIL_CHECK, &CDlgSpecialPickSearchTool::OnBnClickedChkEnableHeadTailCheck)

	ON_CBN_SELCHANGE(IDC_COMBO_COORD_SORT_METHOD, &CDlgSpecialPickSearchTool::OnCbnSelchangeComboCoordSortMethod)
	ON_EN_SETFOCUS(IDC_EDT_COORD_SORT_GAP2, &CDlgSpecialPickSearchTool::OnEnSetfocusEdtCoordSortGap2)

END_MESSAGE_MAP()


// CDlgSpecialPickSearchTool 消息处理程序


BOOL CDlgSpecialPickSearchTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_btnMarkGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnMarkTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	InitDlgItem();

	return TRUE;  // return TRUE unless you set the focus to a control

}


void CDlgSpecialPickSearchTool::RemoveImage()
{
	m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay.RemoveImage();
	m_DlgPatternSearchTool_Rough.m_GuiPatternDisplay.RemoveImage();

	m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay.RemoveImage();
	m_DlgPatternSearchTool_HeadTail.m_GuiPatternDisplay.RemoveImage();
}

void CDlgSpecialPickSearchTool::SetGuiPatternDisplayImage(BOOL bCopy)
{

}


void CDlgSpecialPickSearchTool::ClearScreen()
{
	m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay.ClearScreen();
	m_DlgPatternSearchTool_Rough.m_GuiPatternDisplay.ClearScreen();
	m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay.ClearScreen();
	m_DlgPatternSearchTool_HeadTail.m_GuiPatternDisplay.ClearScreen();
}

void CDlgSpecialPickSearchTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_SPECIAL_PICK_SEARCH_TOOL") == strDlgID)
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


void  CDlgSpecialPickSearchTool::UpdateDlgItem()
{
	UpdateData(TRUE);

	m_TabIndex=m_cTabCtl.GetCurSel();

	switch(m_TabIndex)
	{
	case 0:  // tab0  粗定位
		{
			// 粗定位
			GetDlgItem(IDC_STATIC_ROUGH_SEARCHTOOL)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->ShowWindow(TRUE);

			GetDlgItem(IDC_STATIC_GRP_COORD_SORT)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_METHOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_COMBO_COORD_SORT_METHOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_X)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_DE)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_AS)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_DE)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_AS)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_GAP)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHK_COORD_SORT_ENABLE)->ShowWindow(TRUE);


			// 精定位
			GetDlgItem(IDC_STATIC_ACCURATE_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
			// 头尾
			GetDlgItem(IDC_STATIC_HEADTAIL_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(FALSE);

			GetDlgItem(IDC_STATIC_HEADTAIL_TPYE)->ShowWindow(FALSE);
			
			GetDlgItem(IDC_COMBO_HEADTAILFAILTYPE)->ShowWindow(FALSE);

			if (m_nRoughSearchToolTypeIndex==0) // 粗定位采用区域定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(TRUE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
			}
			else if(m_nRoughSearchToolTypeIndex == 1)// 粗定位采用几何定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(TRUE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else if (m_nRoughSearchToolTypeIndex == 2)// 粗定位采用Blob
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(TRUE);
			}

		}
		break;

	case 1: // tab1 精定位
		{
			// 粗定位
			GetDlgItem(IDC_STATIC_ROUGH_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->ShowWindow(FALSE);

			GetDlgItem(IDC_STATIC_GRP_COORD_SORT)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_METHOD)->ShowWindow(FALSE);
			GetDlgItem(IDC_COMBO_COORD_SORT_METHOD)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_X)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_Y)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_DE)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_AS)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_DE)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_AS)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_GAP)->ShowWindow(FALSE);
			GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_COORD_SORT_ENABLE)->ShowWindow(FALSE);

			// 精定位
			GetDlgItem(IDC_STATIC_ACCURATE_SEARCHTOOL)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
			//GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
			// 头尾
			GetDlgItem(IDC_STATIC_HEADTAIL_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_HEADTAIL_TPYE)->ShowWindow(FALSE);
			
			
			GetDlgItem(IDC_COMBO_HEADTAILFAILTYPE)->ShowWindow(FALSE);

			if (m_nAccurateSearchToolTypeIndex==0)  // 精定位采用 矩形定位 找4线
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(TRUE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);

			}
			else if (m_nAccurateSearchToolTypeIndex == 1) // 精定位采用 Blob
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(TRUE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}

		}
		break;
	case 2: // tab2 头尾检查
		{

			// 粗定位
			GetDlgItem(IDC_STATIC_ROUGH_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->ShowWindow(FALSE);

			GetDlgItem(IDC_STATIC_GRP_COORD_SORT)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_METHOD)->ShowWindow(FALSE);
			GetDlgItem(IDC_COMBO_COORD_SORT_METHOD)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_X)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_Y)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_DE)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_AS)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_DE)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_AS)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_COORD_SORT_GAP)->ShowWindow(FALSE);
			GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_COORD_SORT_ENABLE)->ShowWindow(FALSE);
			// 精定位
			GetDlgItem(IDC_STATIC_ACCURATE_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
			// 头尾
			GetDlgItem(IDC_STATIC_HEADTAIL_SEARCHTOOL)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_HEADTAIL_TPYE)->ShowWindow(TRUE);
			GetDlgItem(IDC_COMBO_HEADTAILFAILTYPE)->ShowWindow(TRUE);

			if (m_nHeadTailCheckToolTypeIndex==0) // 头尾采用区域定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(TRUE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else if(m_nHeadTailCheckToolTypeIndex == 1)// 头尾采用几何定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(TRUE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else if(m_nHeadTailCheckToolTypeIndex == 2) // 头尾采用直方图
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(TRUE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
		}
		break;
	default:
		{
			// 粗定位
			GetDlgItem(IDC_STATIC_ROUGH_SEARCHTOOL)->ShowWindow(TRUE);
			GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_GRP_COORD_SORT)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_METHOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_COMBO_COORD_SORT_METHOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_X)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_ORDER_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_DE)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_X_AS)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_DE)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_COORD_SORT_ORDER_Y_AS)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC_COORD_SORT_GAP)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->ShowWindow(TRUE);
			GetDlgItem(IDC_CHK_COORD_SORT_ENABLE)->ShowWindow(TRUE);
			// 精定位
			GetDlgItem(IDC_STATIC_ACCURATE_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
			// 头尾
			GetDlgItem(IDC_STATIC_HEADTAIL_SEARCHTOOL)->ShowWindow(FALSE);
			GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC_HEADTAIL_TPYE)->ShowWindow(FALSE);
			
			
			GetDlgItem(IDC_COMBO_HEADTAILFAILTYPE)->ShowWindow(FALSE);

			if (m_nRoughSearchToolTypeIndex==0) // 粗定位采用区域定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(TRUE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else if(m_nRoughSearchToolTypeIndex == 1)// 粗定位采用几何定位
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(TRUE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
			}
			else if (m_nRoughSearchToolTypeIndex == 2)// 粗定位采用Blob
			{
				m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
				m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
				m_DlgFind4LineParamSet.ShowWindow(FALSE);
				m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);
				m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet.ShowWindow(FALSE);
				m_DlgBlobSearchParamSet_Rough.ShowWindow(TRUE);
			}
		}
		break;

	}



}
void  CDlgSpecialPickSearchTool::InitDlgItem()
{

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	
	}// tab 页插入
	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		{
			//Tab框
			m_cTabCtl.InsertItem(0,_T("ToughSearchTool"));
			m_cTabCtl.InsertItem(1,_T("AccurateSearchTool"));
			m_cTabCtl.InsertItem(2,_T("HeadTailInspect"));
			//m_cTabCtl.InsertItem(3,_T("正反检查"));
			m_cTabCtl.SetCurSel(0);
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		{
			//Tab框
			m_cTabCtl.InsertItem(0,_T("粗定位"));
			m_cTabCtl.InsertItem(1,_T("精定位"));
			m_cTabCtl.InsertItem(2,_T("头尾检查"));
			//m_cTabCtl.InsertItem(3,_T("正反检查"));
			m_cTabCtl.SetCurSel(0);
		}
		break;
	}
	

	m_cmbRouchSearchtoolType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL)/*_T("区域定位工具")*/);
	m_cmbRouchSearchtoolType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL)/*_T("几何定位工具")*/);
	m_cmbRouchSearchtoolType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BLOB_SEARCH_TOOL)/*_T("Blob定位工具")*/);
	m_cmbRouchSearchtoolType.SetCurSel(0);

	m_cmbAccurateSearchType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL)/*_T("矩形定位工具")*/);
	m_cmbAccurateSearchType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BLOB_SEARCH_TOOL)/*_T("Blob定位工具")*/);
	m_cmbAccurateSearchType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MMARK_SEARCH_TOOL)/*_T("多标记点")*/);
	m_cmbAccurateSearchType.SetCurSel(0);

	m_cmbHeadTailCheckToolType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL)/*_T("区域定位工具")*/);
	m_cmbHeadTailCheckToolType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL)/*_T("几何定位工具")*/);
	m_cmbHeadTailCheckToolType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_GREY_LEVEL_HISTOGRAM_SEARCH_TOOL)/*_T("灰度直方图工具")*/);
	m_cmbHeadTailCheckToolType.SetCurSel(0);
	////粗定位
	//m_cmbRouchSearchtoolType.AddString(_T("区域定位工具"));
	//m_cmbRouchSearchtoolType.AddString(_T("几何定位工具"));
	//m_cmbRouchSearchtoolType.AddString(_T("Blob定位工具"));
	//m_cmbRouchSearchtoolType.SetCurSel(0);
	//IDS_CMB_RECT_SEARCH_TOOL  IDS_CMB_BLOB_SEARCH_TOOL
	////精定位
	//m_cmbAccurateSearchType.AddString(_T("矩形定位工具"));
	//m_cmbAccurateSearchType.AddString(_T("Blob定位工具"));
	//m_cmbAccurateSearchType.AddString(_T("多标记点"));
	//m_cmbAccurateSearchType.SetCurSel(0);
	////IDS_CMB_PATTERN_SEARCH_TOOL  IDS_CMB_QUICK_SEARCH_TOOL IDS_CMB_GREY_LEVEL_HISTOGRAM_SEARCH_TOOL
	//// 头尾检查
	//m_cmbHeadTailCheckToolType.AddString(_T("区域定位工具"));
	//m_cmbHeadTailCheckToolType.AddString(_T("几何定位工具"));
	//m_cmbHeadTailCheckToolType.AddString(_T("灰度直方图工具"));
	//m_cmbHeadTailCheckToolType.SetCurSel(2);

	CRect rect;
	m_cTabCtl.GetClientRect(rect);

	// 粗定位
	GetDlgItem(IDC_STATIC_ROUGH_SEARCHTOOL)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
	// 精定位
	GetDlgItem(IDC_STATIC_ACCURATE_SEARCHTOOL)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
	// 头尾
	GetDlgItem(IDC_STATIC_HEADTAIL_SEARCHTOOL)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(FALSE);

	int nIntendY = 47;
	rect.top+=nIntendY;
	rect.left-=2;
	rect.right+=2;
	//rect.bottom -=1;
	CRect rect2;
	rect2 = rect;
	/*rect2.top +=20;*/

	// 粗定位
	//几何
	m_DlgQuickSearchTool_Rough.m_pDlgParent=m_pDlgParent;
	m_DlgQuickSearchTool_Rough.Create(IDD_DLG_QUICKSEARCH_TOOL,&m_cTabCtl);
	m_DlgQuickSearchTool_Rough.MoveWindow(rect2);
	m_DlgQuickSearchTool_Rough.ShowWindow(FALSE);
	//区域
	m_DlgPatternSearchTool_Rough.m_pDlgParent=m_pDlgParent;
	m_DlgPatternSearchTool_Rough.Create(IDD_DLG_PATTERNSEARCH_TOOL,&m_cTabCtl);
	m_DlgPatternSearchTool_Rough.MoveWindow(rect2);
	m_DlgPatternSearchTool_Rough.ShowWindow(FALSE);
	//几何工具隐藏不需要控件
	(m_DlgQuickSearchTool_Rough.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_Rough.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_Rough.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	//区域隐藏不需要控件
	(m_DlgPatternSearchTool_Rough.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool_Rough.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool_Rough.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	
	// 粗定位Blob
	m_DlgBlobSearchParamSet_Rough.m_pDlgParent=m_pDlgParent;
	m_DlgBlobSearchParamSet_Rough.Create(IDD_DLG_BLOB_SEARCH_TOOL_FOR_SPECIAL_PICK,&m_cTabCtl);
	m_DlgBlobSearchParamSet_Rough.MoveWindow(rect);
	m_DlgBlobSearchParamSet_Rough.ShowWindow(FALSE);
	//m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_STATIC_BLOB_MIN_AREA2)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_STATIC_POINT001)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_STATIC_POINT002)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_STATIC_GROUP_FOR_OUTPUT)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_COMBO_OUTPUTPOINT1)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet_Rough.GetDlgItem(IDC_COMBO_OUTPUTPOINT2)->ShowWindow(FALSE);
	

	//rect.bottom +=80;
	//rect2.top -=40;
	// 找线
	m_DlgFind4LineParamSet.m_pDlgParent=m_pDlgParent;
	m_DlgFind4LineParamSet.Create(IDD_DLG_RECT_FOR_SPECIAL_PICK,&m_cTabCtl);
	m_DlgFind4LineParamSet.MoveWindow(rect2);
	m_DlgFind4LineParamSet.ShowWindow(FALSE);

	//rect2.top +=40;


	// Blob
	m_DlgBlobSearchParamSet.m_pDlgParent=m_pDlgParent;
	m_DlgBlobSearchParamSet.Create(IDD_DLG_BLOB_SEARCH_TOOL_FOR_SPECIAL_PICK,&m_cTabCtl);
	m_DlgBlobSearchParamSet.MoveWindow(rect);
	m_DlgBlobSearchParamSet.ShowWindow(FALSE);
	m_DlgBlobSearchParamSet.GetDlgItem(IDC_STATIC_SEARCH_NUMS)->ShowWindow(FALSE);
	m_DlgBlobSearchParamSet.GetDlgItem(IDC_EDT_BLOB_SEARCH_NUMS)->ShowWindow(FALSE);


	// 头尾
	m_DlgQuickSearchTool_HeadTail.m_pDlgParent=m_pDlgParent;
	m_DlgQuickSearchTool_HeadTail.Create(IDD_DLG_QUICKSEARCH_TOOL,&m_cTabCtl);
	m_DlgQuickSearchTool_HeadTail.MoveWindow(rect2);
	m_DlgQuickSearchTool_HeadTail.ShowWindow(FALSE);

	m_DlgPatternSearchTool_HeadTail.m_pDlgParent=m_pDlgParent;
	m_DlgPatternSearchTool_HeadTail.Create(IDD_DLG_PATTERNSEARCH_TOOL,&m_cTabCtl);
	m_DlgPatternSearchTool_HeadTail.MoveWindow(rect2);
	m_DlgPatternSearchTool_HeadTail.ShowWindow(FALSE);

	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	// 头尾检查不需要设置搜索个数
	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);

	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	//头尾检查 不需要提取
	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_CENTER))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_CENTER))->ShowWindow(FALSE);
	// 头尾检查不需要设置搜索个数
	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);

	m_DlgHistogramForSpecicalPick.m_pDlgParent=m_pDlgParent;
	m_DlgHistogramForSpecicalPick.Create(IDD_DLG_HISTOGRAM_FOR_SPECIAL_PICK,&m_cTabCtl);
	m_DlgHistogramForSpecicalPick.MoveWindow(rect2);
	m_DlgHistogramForSpecicalPick.ShowWindow(FALSE);

	/*rect.bottom +=60;*/
	//rect.bottom +=100;  
	m_comboHeadTailFailTpe.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_FAIL)/*_T("定位失败")*/);
	m_comboHeadTailFailTpe.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCHSUCCEED_PICKANDROTATE)/*_T("定位成功,拾取后旋转180度")*/);
	m_comboHeadTailFailTpe.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCHSUCCEED_ROTATEANDPICK)/*_T("定位成功,旋转180度后拾取")*/);
	m_comboHeadTailFailTpe.SetCurSel(m_nCoordSortMethod);
	/*m_comboHeadTailFailTpe.InsertString(0,_T("定位失败"));
	m_comboHeadTailFailTpe.InsertString(1,_T("定位成功,拾取后旋转180度"));
	m_comboHeadTailFailTpe.InsertString(2,_T("定位成功,旋转180度后拾取"));
	m_comboHeadTailFailTpe.SetCurSel(0);*/

	m_comboCoordSortMethod.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_X)/*_T("按X坐标排序")*/);
	m_comboCoordSortMethod.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_Y)/*_T("按Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_XFIRST_Y)/*_T("先X后Y坐标排序")*/);
	m_comboCoordSortMethod.InsertString(3, GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SORT_BY_YFIRST_X)/*_T("先Y后X坐标排序")*/);
	m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);

}
void CDlgSpecialPickSearchTool::CheckDlgDataValidation()
{
	m_DlgFind4LineParamSet.CheckDlgDataValidation();

	m_DlgPatternSearchTool_Rough.CheckDlgDataValidation();
	m_DlgQuickSearchTool_Rough.CheckDlgDataValidation();
	m_DlgPatternSearchTool_HeadTail.CheckDlgDataValidation();
	m_DlgQuickSearchTool_HeadTail.CheckDlgDataValidation();
	m_DlgHistogramForSpecicalPick.CheckDlgDataValidation();
}
void CDlgSpecialPickSearchTool::UpdateControlDisplay()
{

	m_DlgFind4LineParamSet.UpdateControlDisplay();
	m_DlgPatternSearchTool_Rough.UpdateControlDisplay();
	m_DlgQuickSearchTool_Rough.UpdateControlDisplay();

	m_DlgPatternSearchTool_HeadTail.UpdateControlDisplay();
	m_DlgQuickSearchTool_HeadTail.UpdateControlDisplay();

	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMB_ROUGH_SEARCHTOOL_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_ACCURATE_SEARCHTOOL_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_HEADTAIL_SEARCHTOOL_TYPE)->EnableWindow(TRUE);
	}
	m_DlgPatternSearchTool_Rough.UpdateRadioButtonDisplay();
	m_DlgQuickSearchTool_Rough.UpdateRadioButtonDisplay();
	m_DlgBlobSearchParamSet_Rough.UpdateRadioButtonDisplay();
}

// 

void CDlgSpecialPickSearchTool::OnCbnSelchangeComboCoordSortMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCoordSortMethod = m_comboCoordSortMethod.GetCurSel();
}


void CDlgSpecialPickSearchTool::OnEnSetfocusEdtCoordSortGap2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pDlgParent->m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pDlgParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDT_COORD_SORT_GAP2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDT_COORD_SORT_GAP2, strInput);

		if (TRUE == m_pDlgParent->m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDT_COORD_SORT_GAP2, strOutput);
		}
	}
}


void CDlgSpecialPickSearchTool::OnBnClickedChkEnableAccurateLocation()
{
	UpdateData(TRUE);
	OnBnClickedBtnMarkGettrainimage();

	/*m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableAccurateLocation)
	{
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHK_INSPECTAFTERCORNER)->ShowWindow(FALSE);
	}*/
}
void CDlgSpecialPickSearchTool::OnBnClickedChkEnableHeadTailCheck()
{
	UpdateData(TRUE);
	OnBnClickedBtnMarkGettrainimage();
}


void CDlgSpecialPickSearchTool::UpdateTrainData(bool bValidate)
{

	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		CSearchToolType tRoughSearchToolType;
		CSearchToolType tAccurateSearchToolType;
		CSearchToolType tHeadTailCheckToolType;

		if (m_nRoughSearchToolTypeIndex==0)
		{
			tRoughSearchToolType=ePatternSearchTool;
		}
		else if (m_nRoughSearchToolTypeIndex==1)
		{
			tRoughSearchToolType=eQuickSearchTool;
		}
		else if (m_nRoughSearchToolTypeIndex==2)
		{
			tRoughSearchToolType=ePatternBlobSearchTool;
		}
		// 精定位
		if (m_nAccurateSearchToolTypeIndex==0) // 矩形定位工具
		{
			tAccurateSearchToolType=ePatternRectSearchTool;
		}
		else if (m_nAccurateSearchToolTypeIndex==1)
		{
			tAccurateSearchToolType=ePatternBlobSearchTool;
		}
		else if(m_nAccurateSearchToolTypeIndex == 2)
		{
			tAccurateSearchToolType=eMPatternSearchTool;
		}

		// 头尾检查
		if (m_nHeadTailCheckToolTypeIndex==0)
		{
			tHeadTailCheckToolType=ePatternSearchTool;
		}
		else if (m_nHeadTailCheckToolTypeIndex==1)
		{
			tHeadTailCheckToolType=eQuickSearchTool;
		}
		else if (m_nHeadTailCheckToolTypeIndex==2)
		{
			tHeadTailCheckToolType=eAreaInspectToolEx;
		}

		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetRoughSearchToolType(tRoughSearchToolType);
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetAccurateSearchToolType(tAccurateSearchToolType);
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetHeadTailCheckToolType(tHeadTailCheckToolType);

		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableAccurateLocation=m_bEnableAccurateLoation;
		((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHeadTail=m_bEnableHeadTailCheck;
		
		// 粗定位
		switch(m_nRoughSearchToolTypeIndex)
		{
		case 0: // 区域
			{
				m_DlgPatternSearchTool_Rough.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgPatternSearchTool_Rough.m_nImageChannel;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_Accuracy = (Accuracy)(1<<m_DlgPatternSearchTool_Rough.m_nPatAccuracyMode);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_CoarseScale = (CoarseScale)(m_DlgPatternSearchTool_Rough.m_nPatSpeedMode+1);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable = m_DlgPatternSearchTool_Rough.m_bEnableMask;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_dAngleLow  =  m_DlgPatternSearchTool_Rough.m_dAngleLow;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_dAngleHigh = m_DlgPatternSearchTool_Rough.m_dAngleHigh;
			}
			break;
		case 1:// 几何
			{
				m_DlgQuickSearchTool_Rough.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgQuickSearchTool_Rough.m_nImageChannel; 
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_dGrau = m_DlgQuickSearchTool_Rough.m_dGrau; 
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_lNoiseThre = m_DlgQuickSearchTool_Rough.m_lNoiseThre;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_lTotalMag = m_DlgQuickSearchTool_Rough.m_lTotalMag;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable = m_DlgQuickSearchTool_Rough.m_bEnableMask;
			}
			break;
		case 2:
			{
				m_DlgBlobSearchParamSet_Rough.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel			= m_DlgBlobSearchParamSet_Rough.m_nImageChannel;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_bBlobMode				= (m_DlgBlobSearchParamSet_Rough.m_nBlobMode==0?false:true);
				//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode = m_DlgBlobSearchParamSet_Rough.m_nBlobOutMode;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobGrayThre			= m_DlgBlobSearchParamSet_Rough.m_lBlobGrayThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lConnectivityMinPels	= m_DlgBlobSearchParamSet_Rough.m_lConnectivityMinPels;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre		= m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMinThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre		= m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMaxThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_nBlobSearchNums		= m_DlgBlobSearchParamSet_Rough.m_nBlobSearchNums;

			}
			break;
		}

		// 精定位
		switch(m_nAccurateSearchToolTypeIndex)
		{
		case 0: // 矩形
			{
				m_DlgFind4LineParamSet.UpdateData(TRUE);
				//m_DlgFind4LineParamSet.UpdateTrainData(bValidate);
				//找线1
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_nOutLineNum  =m_DlgFind4LineParamSet.m_nLineOutLineNum1;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_dContrastThre = m_DlgFind4LineParamSet.m_dLineContrastThre1;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_lFilterHalfWidth = m_DlgFind4LineParamSet.m_lLineFilterHalfWidth1;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_edgeProperty = (EdgePolarity)m_DlgFind4LineParamSet.m_nLinePolarityMode1;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bSortByScore = m_DlgFind4LineParamSet.m_bSortByScore1;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_nCaliperNum = m_DlgFind4LineParamSet.m_nLineCaliperNum1;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bEnableCornerLineMask = m_DlgFind4LineParamSet.m_bEnableLineMask_1;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bCaliperVisible = m_DlgFind4LineParamSet.m_bShowCaliper_1;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bEnableDualEdge = m_DlgFind4LineParamSet.m_bEnableDualEdge_1;
				
				//找线2
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_nOutLineNum  = m_DlgFind4LineParamSet.m_nLineOutLineNum2;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_dContrastThre = m_DlgFind4LineParamSet.m_dLineContrastThre2;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_lFilterHalfWidth = m_DlgFind4LineParamSet.m_lLineFilterHalfWidth2;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_edgeProperty = (EdgePolarity)m_DlgFind4LineParamSet.m_nLinePolarityMode2;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bSortByScore = m_DlgFind4LineParamSet.m_bSortByScore2;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_nCaliperNum = m_DlgFind4LineParamSet.m_nLineCaliperNum2;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bEnableCornerLineMask = m_DlgFind4LineParamSet.m_bEnableLineMask_2;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bCaliperVisible = m_DlgFind4LineParamSet.m_bShowCaliper_2;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bEnableDualEdge = m_DlgFind4LineParamSet.m_bEnableDualEdge_2;

				//找线3
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_nOutLineNum  = m_DlgFind4LineParamSet.m_nLineOutLineNum3;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_dContrastThre = m_DlgFind4LineParamSet.m_dLineContrastThre3;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_lFilterHalfWidth = m_DlgFind4LineParamSet.m_lLineFilterHalfWidth3;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_edgeProperty = (EdgePolarity)m_DlgFind4LineParamSet.m_nLinePolarityMode3;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bSortByScore = m_DlgFind4LineParamSet.m_bSortByScore3;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_nCaliperNum = m_DlgFind4LineParamSet.m_nLineCaliperNum3;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bEnableCornerLineMask = m_DlgFind4LineParamSet.m_bEnableLineMask_3;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bCaliperVisible = m_DlgFind4LineParamSet.m_bShowCaliper_3;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bEnableDualEdge = m_DlgFind4LineParamSet.m_bEnableDualEdge_3;
				//找线4
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_nOutLineNum  = m_DlgFind4LineParamSet.m_nLineOutLineNum4;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_dContrastThre = m_DlgFind4LineParamSet.m_dLineContrastThre4;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_lFilterHalfWidth = m_DlgFind4LineParamSet.m_lLineFilterHalfWidth4;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_edgeProperty = (EdgePolarity)m_DlgFind4LineParamSet.m_nLinePolarityMode4;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bSortByScore = m_DlgFind4LineParamSet.m_bSortByScore4;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_nCaliperNum = m_DlgFind4LineParamSet.m_nLineCaliperNum4;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bEnableCornerLineMask = m_DlgFind4LineParamSet.m_bEnableLineMask_4;
				((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bCaliperVisible = m_DlgFind4LineParamSet.m_bShowCaliper_4;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bEnableDualEdge = m_DlgFind4LineParamSet.m_bEnableDualEdge_4;
			}
			break;
		case 1: // Blob
			{

			}
			break;
		}

		// 头尾检查
		switch(m_nHeadTailCheckToolTypeIndex)
		{
		case 0: // 区域
			{
				m_DlgPatternSearchTool_HeadTail.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_Accuracy = (Accuracy)(1<<m_DlgPatternSearchTool_HeadTail.m_nPatAccuracyMode);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_CoarseScale = (CoarseScale)(m_DlgPatternSearchTool_HeadTail.m_nPatSpeedMode+1);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable = m_DlgPatternSearchTool_HeadTail.m_bEnableMask;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_dAngleLow  =  m_DlgPatternSearchTool_HeadTail.m_dAngleLow;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_dAngleHigh = m_DlgPatternSearchTool_HeadTail.m_dAngleHigh;
			}
			break;
		case 1:// 几何
			{
				m_DlgQuickSearchTool_Rough.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_dGrau = m_DlgQuickSearchTool_HeadTail.m_dGrau; 
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_lNoiseThre = m_DlgQuickSearchTool_HeadTail.m_lNoiseThre;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_lTotalMag = m_DlgQuickSearchTool_HeadTail.m_lTotalMag;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable = m_DlgQuickSearchTool_HeadTail.m_bEnableMask;
			}
			break;
		case 2:
			{
				m_DlgHistogramForSpecicalPick.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset = m_DlgHistogramForSpecicalPick.m_dHeadXoffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadYoffset = m_DlgHistogramForSpecicalPick.m_dHeadYOffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadLength= m_DlgHistogramForSpecicalPick.m_nHeadLength;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadWidth = m_DlgHistogramForSpecicalPick.m_nHeadWidth;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailXoffset = m_DlgHistogramForSpecicalPick.m_dTailXoffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailYoffset = m_DlgHistogramForSpecicalPick.m_dTailYOffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailLength= m_DlgHistogramForSpecicalPick.m_nTailLength;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailWidth= m_DlgHistogramForSpecicalPick.m_nTailWidth;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset = m_DlgHistogramForSpecicalPick.m_dHeadXoffset;

				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead = m_DlgHistogramForSpecicalPick.m_MeanHighHead;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = m_DlgHistogramForSpecicalPick.m_nHeadGreyValue;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = m_DlgHistogramForSpecicalPick.m_nTailGreyValue;
			}
			break;
		}


		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==ePatternSearchTool)
		{
			m_nRoughSearchToolTypeIndex=0;
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==eQuickSearchTool)
		{
			m_nRoughSearchToolTypeIndex=1;
		}
		else if(((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==ePatternBlobSearchTool)
		{
			m_nRoughSearchToolTypeIndex=2;
		}


		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetAccurateSearchToolType()==ePatternRectSearchTool)
		{
			m_nAccurateSearchToolTypeIndex = 0;
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetAccurateSearchToolType()==ePatternBlobSearchTool)
		{
			m_nAccurateSearchToolTypeIndex = 1;
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetAccurateSearchToolType()==eMPatternSearchTool)
		{
			m_nAccurateSearchToolTypeIndex = 2;
		}

		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetHeadTailCheckToolType()==ePatternSearchTool)
		{
			m_nHeadTailCheckToolTypeIndex=0;
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetHeadTailCheckToolType()==eQuickSearchTool)
		{
			m_nHeadTailCheckToolTypeIndex=1;
		}
		else if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetHeadTailCheckToolType()==eAreaInspectToolEx)
		{
			m_nHeadTailCheckToolTypeIndex = 2;
		}
		m_bEnableAccurateLoation = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableAccurateLocation;
		m_bEnableHeadTailCheck = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHeadTail;
		
		UpdateData(FALSE);

		// 粗定位
		switch(m_nRoughSearchToolTypeIndex)
		{
		case 0:// 粗定位 区域
			{
				m_DlgPatternSearchTool_Rough.m_nImageChannel = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
				m_DlgPatternSearchTool_Rough.m_nPatAccuracyMode = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_Accuracy/2;
				m_DlgPatternSearchTool_Rough.m_nPatSpeedMode = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_CoarseScale -1;
				m_DlgPatternSearchTool_Rough.m_bEnableMask = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_bMaskEnable;
				m_DlgPatternSearchTool_Rough.m_dAngleLow = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_dAngleLow;
				m_DlgPatternSearchTool_Rough.m_dAngleHigh = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_PatternSearchTrainData.m_dAngleHigh ;



				if (m_DlgPatternSearchTool_Rough.m_bEnableMask)
				{
					m_DlgPatternSearchTool_Rough.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternSearchTool_Rough.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}

				m_DlgPatternSearchTool_Rough.m_comboAccuracy.SetCurSel(m_DlgPatternSearchTool_Rough.m_nPatAccuracyMode);
				m_DlgPatternSearchTool_Rough.m_comboSpeed.SetCurSel(m_DlgPatternSearchTool_Rough.m_nPatSpeedMode);

				m_DlgPatternSearchTool_Rough.UpdateData(FALSE);

			}
			break;
		case 1:// 粗定位 几何
			{

				m_DlgQuickSearchTool_Rough.m_nImageChannel = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel; 
				m_DlgQuickSearchTool_Rough.m_dGrau = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_dGrau; 
				m_DlgQuickSearchTool_Rough.m_lNoiseThre = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_lNoiseThre;
				m_DlgQuickSearchTool_Rough.m_lTotalMag = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_lTotalMag;
				m_DlgQuickSearchTool_Rough.m_bEnableMask = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_Rough_QuickSearchTrainData.m_bMaskEnable;

				if (m_DlgQuickSearchTool_Rough.m_bEnableMask)
				{
					m_DlgQuickSearchTool_Rough.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgQuickSearchTool_Rough.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}

				m_DlgQuickSearchTool_Rough.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				m_DlgBlobSearchParamSet_Rough.m_nImageChannel = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel;
				m_DlgBlobSearchParamSet_Rough.m_nBlobMode = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_bBlobMode;
				//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode = m_DlgBlobSearchParamSet_Rough.m_nBlobOutMode;
				m_DlgBlobSearchParamSet_Rough.m_lBlobGrayThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobGrayThre;
				m_DlgBlobSearchParamSet_Rough.m_lConnectivityMinPels = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lConnectivityMinPels;
				m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMinThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre;
				m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMaxThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre;
				m_DlgBlobSearchParamSet_Rough.m_nBlobSearchNums = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_nBlobSearchNums;
				m_DlgBlobSearchParamSet_Rough.UpdateData(FALSE);
			}
			break;
		}

		// 精定位
		switch(m_nAccurateSearchToolTypeIndex)
		{
		case 0: // 矩形
			{
				
				//m_DlgFind4LineParamSet.UpdateTrainData(bValidate);
				//找线1
				m_DlgFind4LineParamSet.m_nLineOutLineNum1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_nOutLineNum;
				m_DlgFind4LineParamSet.m_dLineContrastThre1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_dContrastThre;
				m_DlgFind4LineParamSet.m_lLineFilterHalfWidth1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_lFilterHalfWidth;
				m_DlgFind4LineParamSet.m_nLinePolarityMode1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_edgeProperty;
				m_DlgFind4LineParamSet.m_bSortByScore1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bSortByScore;
				m_DlgFind4LineParamSet.m_nLineCaliperNum1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_nCaliperNum;
				m_DlgFind4LineParamSet.m_bEnableLineMask_1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bEnableCornerLineMask;
				m_DlgFind4LineParamSet.m_bShowCaliper_1 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[0].m_bCaliperVisible;
				m_DlgFind4LineParamSet.m_bEnableDualEdge_1 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[0].m_bEnableDualEdge;

				m_DlgFind4LineParamSet.m_nLineOutLineNum2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_nOutLineNum;
				m_DlgFind4LineParamSet.m_dLineContrastThre2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_dContrastThre;
				m_DlgFind4LineParamSet.m_lLineFilterHalfWidth2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_lFilterHalfWidth;
				m_DlgFind4LineParamSet.m_nLinePolarityMode2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_edgeProperty;
				m_DlgFind4LineParamSet.m_bSortByScore2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bSortByScore;
				m_DlgFind4LineParamSet.m_nLineCaliperNum2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_nCaliperNum;
				m_DlgFind4LineParamSet.m_bEnableLineMask_2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bEnableCornerLineMask;
				m_DlgFind4LineParamSet.m_bShowCaliper_2 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[1].m_bCaliperVisible;
				m_DlgFind4LineParamSet.m_bEnableDualEdge_2 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[1].m_bEnableDualEdge;

				m_DlgFind4LineParamSet.m_nLineOutLineNum3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_nOutLineNum;
				m_DlgFind4LineParamSet.m_dLineContrastThre3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_dContrastThre;
				m_DlgFind4LineParamSet.m_lLineFilterHalfWidth3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_lFilterHalfWidth;
				m_DlgFind4LineParamSet.m_nLinePolarityMode3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_edgeProperty;
				m_DlgFind4LineParamSet.m_bSortByScore3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bSortByScore;
				m_DlgFind4LineParamSet.m_nLineCaliperNum3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_nCaliperNum;
				m_DlgFind4LineParamSet.m_bEnableLineMask_3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bEnableCornerLineMask;
				m_DlgFind4LineParamSet.m_bShowCaliper_3 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[2].m_bCaliperVisible;
				m_DlgFind4LineParamSet.m_bEnableDualEdge_3 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[2].m_bEnableDualEdge;

				m_DlgFind4LineParamSet.m_nLineOutLineNum4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_nOutLineNum;
				m_DlgFind4LineParamSet.m_dLineContrastThre4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_dContrastThre;
				m_DlgFind4LineParamSet.m_lLineFilterHalfWidth4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_lFilterHalfWidth;
				m_DlgFind4LineParamSet.m_nLinePolarityMode4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_edgeProperty;
				m_DlgFind4LineParamSet.m_bSortByScore4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bSortByScore;
				m_DlgFind4LineParamSet.m_nLineCaliperNum4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_nCaliperNum;
				m_DlgFind4LineParamSet.m_bEnableLineMask_4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bEnableCornerLineMask;
				m_DlgFind4LineParamSet.m_bShowCaliper_4 = ((CSpecialPickSearchTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_FindLineTrainGuiParam[3].m_bCaliperVisible;
				m_DlgFind4LineParamSet.m_bEnableDualEdge_4 = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_FindLineCaliperTrainDataParam[3].m_bEnableDualEdge;
				m_DlgFind4LineParamSet.m_checkSortByScore1.SetCheck(m_DlgFind4LineParamSet.m_bSortByScore1);
				m_DlgFind4LineParamSet.m_checkSortByScore2.SetCheck(m_DlgFind4LineParamSet.m_bSortByScore2);
				m_DlgFind4LineParamSet.m_checkSortByScore3.SetCheck(m_DlgFind4LineParamSet.m_bSortByScore3);
				m_DlgFind4LineParamSet.m_checkSortByScore4.SetCheck(m_DlgFind4LineParamSet.m_bSortByScore4);

				m_DlgFind4LineParamSet.UpdateData(FALSE);

			}
			break;
		}

		// 头尾检查
		switch(m_nHeadTailCheckToolTypeIndex)
		{
		case 0:
			{

				m_DlgPatternSearchTool_HeadTail.m_nPatAccuracyMode = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_Accuracy/2;
				m_DlgPatternSearchTool_HeadTail.m_nPatSpeedMode = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_CoarseScale-1;
				m_DlgPatternSearchTool_HeadTail.m_bEnableMask = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_bMaskEnable;
				m_DlgPatternSearchTool_HeadTail.m_dAngleLow = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_dAngleLow;
				m_DlgPatternSearchTool_HeadTail.m_dAngleHigh = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_PatternSearchTrainData.m_dAngleHigh;

				
				
				if (m_DlgPatternSearchTool_HeadTail.m_bEnableMask)
				{
					m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgPatternSearchTool_HeadTail.m_comboAccuracy.SetCurSel(m_DlgPatternSearchTool_HeadTail.m_nPatAccuracyMode);
				m_DlgPatternSearchTool_HeadTail.m_comboSpeed.SetCurSel(m_DlgPatternSearchTool_HeadTail.m_nPatSpeedMode);
				m_DlgPatternSearchTool_HeadTail.UpdateData(FALSE);

			}
			break;
		case 1:
			{

				m_DlgQuickSearchTool_HeadTail.m_dGrau = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_dGrau; 
				m_DlgQuickSearchTool_HeadTail.m_lNoiseThre = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_lNoiseThre;
				m_DlgQuickSearchTool_HeadTail.m_lTotalMag = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_lTotalMag;
				m_DlgQuickSearchTool_HeadTail.m_bEnableMask = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_QuickSearchTrainData.m_bMaskEnable;

				if (m_DlgQuickSearchTool_HeadTail.m_bEnableMask)
				{
					m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgQuickSearchTool_HeadTail.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgQuickSearchTool_HeadTail.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				 m_DlgHistogramForSpecicalPick.m_dHeadXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset;
				 m_DlgHistogramForSpecicalPick.m_dHeadYOffset =((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadYoffset;
				 m_DlgHistogramForSpecicalPick.m_nHeadLength = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadLength;
				 m_DlgHistogramForSpecicalPick.m_nHeadWidth =((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadWidth;
				 m_DlgHistogramForSpecicalPick.m_dTailXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailXoffset;
				 m_DlgHistogramForSpecicalPick.m_dTailYOffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailYoffset;
				m_DlgHistogramForSpecicalPick.m_nTailLength = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailLength;
				m_DlgHistogramForSpecicalPick.m_nTailWidth = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailWidth;
				m_DlgHistogramForSpecicalPick.m_dHeadXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset;

				m_DlgHistogramForSpecicalPick.m_MeanHighHead = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead;
				m_DlgHistogramForSpecicalPick.m_nHeadGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue;
				m_DlgHistogramForSpecicalPick.m_nTailGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue;
				m_DlgHistogramForSpecicalPick.UpdateData(FALSE);
			}
			break;
		}


	}

}
void CDlgSpecialPickSearchTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCheckAfterSearch=m_bInspectAfterCorner;

		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable = m_bCoordSortEnable;
		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod = (CoordSortMethod)m_nCoordSortMethod;
		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap = m_dCoordSortGap;
		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX = (SortOrder)m_nCoordSortOrderX;
		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY =  (SortOrder)m_nCoordSortOrderY;

		((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nHeadTail_FailType = m_nHeadTailFailTpye;


		// 粗定位
		switch(m_nRoughSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool_Rough.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_nSearchNum = m_DlgPatternSearchTool_Rough.m_nSearchNum;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre = m_DlgPatternSearchTool_Rough.m_dAcceptThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre = m_DlgPatternSearchTool_Rough.m_dConfusionThre;
				
			
			}
				break;
		case 1:
			{
				m_DlgQuickSearchTool_Rough.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_nSearchNum = m_DlgQuickSearchTool_Rough.m_nSearchNum;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAcceptThre = m_DlgQuickSearchTool_Rough.m_dAcceptThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAngleLow = m_DlgQuickSearchTool_Rough.m_dAngleLow;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh = m_DlgQuickSearchTool_Rough.m_dAngleHigh;

			}
			break;
		case 2:
			{
				m_DlgBlobSearchParamSet_Rough.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_bBlobMode				= (m_DlgBlobSearchParamSet_Rough.m_nBlobMode==0?false:true);
				//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode = m_DlgBlobSearchParamSet_Rough.m_nBlobOutMode;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobGrayThre			= m_DlgBlobSearchParamSet_Rough.m_lBlobGrayThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lConnectivityMinPels	= m_DlgBlobSearchParamSet_Rough.m_lConnectivityMinPels;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre		= m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMinThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre		= m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMaxThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_nBlobSearchNums		= m_DlgBlobSearchParamSet_Rough.m_nBlobSearchNums;

			}
			break;
		}

		// 精定位
		switch(m_nAccurateSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgFind4LineParamSet.UpdateData(TRUE);
				//四条线
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[0] = m_DlgFind4LineParamSet.m_bShowFitPoint_1;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[1] = m_DlgFind4LineParamSet.m_bShowFitPoint_2;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[2] = m_DlgFind4LineParamSet.m_bShowFitPoint_3;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[3] = m_DlgFind4LineParamSet.m_bShowFitPoint_4;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode1 = m_DlgFind4LineParamSet.m_nResultOutMode1;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode2 = m_DlgFind4LineParamSet.m_nResultOutMode2;
			}
			break;
		case 1:
			{
				m_DlgBlobSearchParamSet.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobMode				= (m_DlgBlobSearchParamSet.m_nBlobMode==0?false:true);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode1 = m_DlgBlobSearchParamSet.m_nOutPutPoint1;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode2 = m_DlgBlobSearchParamSet.m_nOutPutPoint2;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobGrayThre			= m_DlgBlobSearchParamSet.m_lBlobGrayThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lConnectivityMinPels	= m_DlgBlobSearchParamSet.m_lConnectivityMinPels;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobAreaMinThre		= m_DlgBlobSearchParamSet.m_lBlobAreaMinThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobAreaMaxThre		= m_DlgBlobSearchParamSet.m_lBlobAreaMaxThre;

			}
			break;
		}

		// 头尾检查
		switch(m_nHeadTailCheckToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool_HeadTail.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_nSearchNum = m_DlgPatternSearchTool_HeadTail.m_nSearchNum;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_dAcceptThre = m_DlgPatternSearchTool_HeadTail.m_dAcceptThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_dConfusionThre = m_DlgPatternSearchTool_HeadTail.m_dConfusionThre;

			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool_HeadTail.UpdateData(TRUE);
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_nSearchNum = m_DlgQuickSearchTool_HeadTail.m_nSearchNum;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAcceptThre = m_DlgQuickSearchTool_HeadTail.m_dAcceptThre;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAngleLow = m_DlgQuickSearchTool_HeadTail.m_dAngleLow;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAngleHigh = m_DlgQuickSearchTool_HeadTail.m_dAngleHigh;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dSizeLow = m_DlgQuickSearchTool_HeadTail.m_dSizeLow;
				((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dSizeHigh = m_DlgQuickSearchTool_HeadTail.m_dSizeHigh;

			}
			break;
		case 2:
			{
				m_DlgHistogramForSpecicalPick.UpdateData(TRUE);
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset = m_DlgHistogramForSpecicalPick.m_dHeadXoffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadYoffset = m_DlgHistogramForSpecicalPick.m_dHeadYOffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadLength= m_DlgHistogramForSpecicalPick.m_nHeadLength;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadWidth = m_DlgHistogramForSpecicalPick.m_nHeadWidth;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailXoffset = m_DlgHistogramForSpecicalPick.m_dTailXoffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailYoffset = m_DlgHistogramForSpecicalPick.m_dTailYOffset;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailLength= m_DlgHistogramForSpecicalPick.m_nTailLength;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailWidth= m_DlgHistogramForSpecicalPick.m_nTailWidth;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset = m_DlgHistogramForSpecicalPick.m_dHeadXoffset;

				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead = m_DlgHistogramForSpecicalPick.m_MeanHighHead;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue = m_DlgHistogramForSpecicalPick.m_nHeadGreyValue;
				((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue = m_DlgHistogramForSpecicalPick.m_nTailGreyValue;
			}
			break;
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{
	
		
		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableAccurateLocation)
		{
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHK_ENABLE_ACCURATE_LOCATION)->ShowWindow(FALSE);
		}

		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHeadTail)
		{
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHK_ENABLE_HEADTAIL_CHECK)->ShowWindow(FALSE);
		}


		//粗定位
		m_bCoordSortEnable = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bCoordSortEnable;
		m_nCoordSortMethod = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortMethod;
		m_dCoordSortGap = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dCoordSortGap;
		m_nCoordSortOrderX = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderX;
		m_nCoordSortOrderY = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nCoordSortOrderY;

		m_nHeadTailFailTpye = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nHeadTail_FailType;
		m_comboCoordSortMethod.SetCurSel(m_nCoordSortMethod);

		switch(m_nRoughSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool_Rough.m_nSearchNum	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_nSearchNum;
				m_DlgPatternSearchTool_Rough.m_dAcceptThre	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_dAcceptThre;	
				m_DlgPatternSearchTool_Rough.m_dConfusionThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_PatternSearchSearchData.m_dConfusionThre;
				m_DlgPatternSearchTool_Rough.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool_Rough.m_nSearchNum	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_nSearchNum;
				m_DlgQuickSearchTool_Rough.m_dAcceptThre	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAcceptThre;
				m_DlgQuickSearchTool_Rough.m_dAngleLow		= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAngleLow;
				m_DlgQuickSearchTool_Rough.m_dAngleHigh	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dAngleHigh;
				m_DlgQuickSearchTool_Rough.m_dSizeLow		= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dSizeLow;
				m_DlgQuickSearchTool_Rough.m_dSizeHigh	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Struct_Rough_QuickSearchSearchData.m_dSizeHigh;

				m_DlgQuickSearchTool_Rough.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				m_DlgBlobSearchParamSet_Rough.m_nBlobMode = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_bBlobMode;
				//((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode = m_DlgBlobSearchParamSet_Rough.m_nBlobOutMode;
				 m_DlgBlobSearchParamSet_Rough.m_lBlobGrayThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobGrayThre;
				m_DlgBlobSearchParamSet_Rough.m_lConnectivityMinPels = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lConnectivityMinPels;
				m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMinThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMinThre;
				m_DlgBlobSearchParamSet_Rough.m_lBlobAreaMaxThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_lBlobAreaMaxThre;
				 m_DlgBlobSearchParamSet_Rough.m_nBlobSearchNums = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_Rough_BlobSearchTrainData.m_nBlobSearchNums;

				m_DlgBlobSearchParamSet_Rough.UpdateData(FALSE);
			}
			break;
		}
		
		// 精定位
		switch(m_nAccurateSearchToolTypeIndex)
		{
		case 0:
			{
				//四条线
				m_DlgFind4LineParamSet.m_bShowFitPoint_1 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[0];
				m_DlgFind4LineParamSet.m_bShowFitPoint_2 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[1];
				m_DlgFind4LineParamSet.m_bShowFitPoint_3 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[2];
				m_DlgFind4LineParamSet.m_bShowFitPoint_4 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bVisibleFitPoint_Line[3];
				m_DlgFind4LineParamSet.m_nResultOutMode1 =((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode1;
				m_DlgFind4LineParamSet.m_nResultOutMode2 =((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nResultOutMode2;
				m_DlgFind4LineParamSet.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgBlobSearchParamSet.m_nBlobMode = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_bBlobMode;
				m_DlgBlobSearchParamSet.m_nOutPutPoint1 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode1;
				m_DlgBlobSearchParamSet.m_nOutPutPoint2 = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nBlobOutMode2;
				m_DlgBlobSearchParamSet.m_lBlobGrayThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobGrayThre;
				m_DlgBlobSearchParamSet.m_lConnectivityMinPels = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lConnectivityMinPels;
				m_DlgBlobSearchParamSet.m_lBlobAreaMinThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobAreaMinThre;
				m_DlgBlobSearchParamSet.m_lBlobAreaMaxThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_lBlobAreaMaxThre;
				m_DlgBlobSearchParamSet.UpdateData(FALSE);
			}
			break;
		}

		// 头尾
		switch(m_nHeadTailCheckToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool_HeadTail.m_nSearchNum	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_nSearchNum;
				m_DlgPatternSearchTool_HeadTail.m_dAcceptThre	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_dAcceptThre;	
				m_DlgPatternSearchTool_HeadTail.m_dConfusionThre = ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_PatternSearchSearchData.m_dConfusionThre;
				m_DlgPatternSearchTool_HeadTail.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool_HeadTail.m_nSearchNum	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_nSearchNum;
				m_DlgQuickSearchTool_HeadTail.m_dAcceptThre	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAcceptThre;
				m_DlgQuickSearchTool_HeadTail.m_dAngleLow		= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAngleLow;
				m_DlgQuickSearchTool_HeadTail.m_dAngleHigh	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dAngleHigh;
				m_DlgQuickSearchTool_HeadTail.m_dSizeLow		= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dSizeLow;
				m_DlgQuickSearchTool_HeadTail.m_dSizeHigh	= ((CSpecialPickSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_HeadTail_QuickSearchSearchData.m_dSizeHigh;

				m_DlgQuickSearchTool_HeadTail.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				m_DlgHistogramForSpecicalPick.m_dHeadXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset;
				m_DlgHistogramForSpecicalPick.m_dHeadYOffset =((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadYoffset;
				m_DlgHistogramForSpecicalPick.m_nHeadLength = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadLength;
				m_DlgHistogramForSpecicalPick.m_nHeadWidth =((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadWidth;
				m_DlgHistogramForSpecicalPick.m_dTailXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailXoffset;
				m_DlgHistogramForSpecicalPick.m_dTailYOffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dTailYoffset;
				m_DlgHistogramForSpecicalPick.m_nTailLength = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailLength;
				m_DlgHistogramForSpecicalPick.m_nTailWidth = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailWidth;
				m_DlgHistogramForSpecicalPick.m_dHeadXoffset = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_dHeadXoffset;

				m_DlgHistogramForSpecicalPick.m_MeanHighHead = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_MeanHighHead;
				m_DlgHistogramForSpecicalPick.m_nHeadGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue;
				m_DlgHistogramForSpecicalPick.m_nTailGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue;
				m_DlgHistogramForSpecicalPick.UpdateData(FALSE);
			}
			break;
		}

		UpdateData(FALSE);
	}
}

void CDlgSpecialPickSearchTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	*pResult = 0;
}

void CDlgSpecialPickSearchTool::OnCbnSelchangeCmbRouchSearchtoolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}

void CDlgSpecialPickSearchTool::OnCbnSelchangeCmbAccurateSearchToolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}

void CDlgSpecialPickSearchTool::OnCbnSelchangeCmbHeadTailCheckToolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}

void CDlgSpecialPickSearchTool::OnBnClickedBtnMarkGettrainimage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateTrainData(TRUE);

	if (!m_pDlgParent->m_imageInput.IsValid())
	{
		m_pDlgParent->m_bStatusBarFlag = FALSE;
		m_pDlgParent->m_strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_GET_TRAIN_IMAGE_FAILED);	//_T("获取训练图像失败");
		m_pDlgParent->m_strStatusBarInfo2 = GetSysInfoString(m_psaSysInfoStrings,IDS_SB_INVALID_INPUT_IMAGE);		//_T("输入图像数据为空");
		m_pDlgParent->UpdateStatusBar();
		return;
	}
	m_pDlgParent->m_imageTrain = m_pDlgParent->m_imageInput;

	switch(m_nRoughSearchToolTypeIndex)
	{
	case 0:
		{
			if (m_DlgPatternSearchTool_Rough.m_bEnableMask)
			{
				if ( (m_DlgPatternSearchTool_Rough.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternSearchTool_Rough.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternSearchTool_Rough.m_imageMask.Release();
				}
			}
		}
		break;
	case 1:
		{
			if (m_DlgQuickSearchTool_Rough.m_bEnableMask)
			{
				if ( (m_DlgQuickSearchTool_Rough.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgQuickSearchTool_Rough.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgQuickSearchTool_Rough.m_imageMask.Release();
				}
			}

		}
		break;
	}

	switch(m_nHeadTailCheckToolTypeIndex)
	{
	case 0:
		{
			if (m_DlgPatternSearchTool_HeadTail.m_bEnableMask)
			{
				if ( (m_DlgPatternSearchTool_HeadTail.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternSearchTool_HeadTail.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternSearchTool_HeadTail.m_imageMask.Release();
				}
			}
		}
		break;
	case 1:
		{
			if (m_DlgQuickSearchTool_HeadTail.m_bEnableMask)
			{
				if ( (m_DlgQuickSearchTool_HeadTail.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgQuickSearchTool_HeadTail.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgQuickSearchTool_HeadTail.m_imageMask.Release();
				}
			}

		}
		break;
	}


	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}

void CDlgSpecialPickSearchTool::OnBnClickedBtnMarkTrain()
{
	// TODO: 在此添加控件通知处理程序代码

	{
		// TODO: Add your control notification handler code here
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
		UpdateTrainData(TRUE);
		UpdateSearchData(TRUE);

		m_pDlgParent->m_pSearchTool->SetTrainImage(m_pDlgParent->m_imageTrain);

		switch(m_nRoughSearchToolTypeIndex)
		{
		case 0:
			if (m_DlgPatternSearchTool_Rough.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgPatternSearchTool_Rough.m_imageMask);
			}
			break;
		case 1:
			if (m_DlgQuickSearchTool_Rough.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgQuickSearchTool_Rough.m_imageMask);
			}
			break;

		}


		// 启用头尾检查
		if (((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHeadTail)
		{
			switch(m_nHeadTailCheckToolTypeIndex)
			{
			case 0:
				if (m_DlgPatternSearchTool_HeadTail.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage_HeadTail(m_DlgPatternSearchTool_HeadTail.m_imageMask);
				}
				break;
			case 1:
				if (m_DlgQuickSearchTool_HeadTail.m_bEnableMask)
				{
					m_pDlgParent->m_pSearchTool->SetMaskImage_HeadTail(m_DlgQuickSearchTool_HeadTail.m_imageMask);
				}
				break;
			}
		}

		//启用正反 下一步实现
		//启用混料 下一步实现



		if (m_pDlgParent->m_pSearchTool->Train())
		{
			cpImage trainImage;
			
			switch(m_nRoughSearchToolTypeIndex)
			{
			case 0:
				m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
				m_DlgPatternSearchTool_Rough.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
				m_DlgQuickSearchTool_Rough.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			}
			trainImage.Release();

			
			switch(m_nHeadTailCheckToolTypeIndex)
			{
			case 0:
				((CSpecialPickSearchTool*)m_pDlgParent->m_pSearchTool)->GetTrainResultImage_HeadTail(trainImage);
				m_DlgPatternSearchTool_HeadTail.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				((CSpecialPickSearchTool*)m_pDlgParent->m_pSearchTool)->GetTrainResultImage_HeadTail(trainImage);
				m_DlgQuickSearchTool_HeadTail.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;

			}
			m_pDlgParent->UpdateStatusBar(true, GetSysInfoString(m_psaSysInfoStrings,IDS_SB_TRAIN_SUCCEED));
			((CSpecialPickSearchTool*)m_pDlgParent->m_pSearchTool)->UpdateGuiDisplay();
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
}

void CDlgSpecialPickSearchTool::UpdateSearchResult()
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	switch(m_nHeadTailCheckToolTypeIndex)
	{
		case 2:
		{
			m_DlgHistogramForSpecicalPick.m_nHeadGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nHeadGreyValue;
			m_DlgHistogramForSpecicalPick.m_nTailGreyValue = ((CSpecialPickSearchTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Struct_HeadTail_HistogramSearchTrainData.m_nTailGreyValue;
			m_DlgHistogramForSpecicalPick.UpdateData(FALSE);
		}
		break;
	}
}

BOOL CDlgSpecialPickSearchTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
