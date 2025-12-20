// CDlgACFInspectTool.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgACFInspectTool.h "
#include "afxdialogex.h"
#include "DlgSearch.h"


// CDlgACFInspectTool 对话框

IMPLEMENT_DYNAMIC(CDlgACFInspectTool, CDialog)

CDlgACFInspectTool::CDlgACFInspectTool(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgACFInspectTool::IDD, pParent)
	, m_nSearchToolTypeIndex(0)
{
	
	m_TabIndex=0;
	m_pDlgParent = NULL;
	m_psaSysInfoStrings=NULL;
	
}

CDlgACFInspectTool::~CDlgACFInspectTool()
{
}

void CDlgACFInspectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_cTabCtl);
	DDX_Control(pDX, IDC_BTN_MARK_TRAIN, m_btnMarkTrain);
	DDX_Control(pDX, IDC_BTN_MARK_GETTRAINIMAGE, m_btnMarkGetTrainImage);
	DDX_CBIndex(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_nSearchToolTypeIndex);
	DDX_Control(pDX, IDC_CMB_SEARCHTOOL_TYPE, m_cmbSearchToolType);

	DDX_Check(pDX, IDC_CHK_ENABLE_BASESEARCH_CHECK, m_bEnableLocation);
}


BEGIN_MESSAGE_MAP(CDlgACFInspectTool, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgACFInspectTool::OnTcnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_CMB_SEARCHTOOL_TYPE, &CDlgACFInspectTool::OnCbnSelchangeCmbSearchtoolType)
	ON_BN_CLICKED(IDC_BTN_MARK_GETTRAINIMAGE, &CDlgACFInspectTool::OnBnClickedBtnMarkGettrainimage)
	ON_BN_CLICKED(IDC_BTN_MARK_TRAIN, &CDlgACFInspectTool::OnBnClickedBtnMarkTrain)
	ON_BN_CLICKED(IDC_CHK_ENABLE_BASESEARCH_CHECK, &CDlgACFInspectTool::OnBnClickedChkEnableBasesearchCheck)
END_MESSAGE_MAP()


// CDlgACFInspectTool 消息处理程序
void CDlgACFInspectTool::RemoveImage()
{
	m_DlgPatternSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgQuickSearchTool.m_GuiPatternDisplay.RemoveImage();
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.RemoveImage();	// 从显示控件中删除图像	
	m_DlgCornerSearchTool.m_GuiPatternDisplay.RemoveImage();
}
void CDlgACFInspectTool::SetGuiPatternDisplayImage(BOOL bCopy)
{
	
}

void CDlgACFInspectTool::ClearScreen()
{
	m_DlgPatternSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgQuickSearchTool.m_GuiPatternDisplay.ClearScreen();
	m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.ClearScreen();	// 清屏
	m_DlgCornerSearchTool.m_GuiPatternDisplay.ClearScreen();
}
void CDlgACFInspectTool::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_ACF_INSPECT_TOOL") == strDlgID)
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
void  CDlgACFInspectTool::UpdateDlgItem()
{
	UpdateData(TRUE);

	m_TabIndex=m_cTabCtl.GetCurSel();

	switch(m_TabIndex)
	{
		case 0:
			{
				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(TRUE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
				GetDlgItem(IDC_CHK_ENABLE_BASESEARCH_CHECK)->ShowWindow(TRUE);

				if (m_nSearchToolTypeIndex==0)
				{
					m_DlgPatternSearchTool.ShowWindow(TRUE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==1)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(TRUE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==2)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==3)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(TRUE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
			}
			break;
		case 1:
			{
				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(FALSE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(FALSE);
				GetDlgItem(IDC_CHK_ENABLE_BASESEARCH_CHECK)->ShowWindow(FALSE);

				m_DlgPatternSearchTool.ShowWindow(FALSE);
				m_DlgQuickSearchTool.ShowWindow(FALSE);
				m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
				m_DlgCornerSearchTool.ShowWindow(FALSE);
				m_DlgGrayBlobInspectTool.ShowWindow(TRUE);
			}
			break;
		default:
			{
				GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(TRUE);
				GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(TRUE);
				GetDlgItem(IDC_CHK_ENABLE_BASESEARCH_CHECK)->ShowWindow(TRUE);

				if (m_nSearchToolTypeIndex==0)
				{
					m_DlgPatternSearchTool.ShowWindow(TRUE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==1)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(TRUE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==2)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(TRUE);
					m_DlgCornerSearchTool.ShowWindow(FALSE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
				else if (m_nSearchToolTypeIndex==3)
				{
					m_DlgPatternSearchTool.ShowWindow(FALSE);
					m_DlgQuickSearchTool.ShowWindow(FALSE);
					m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
					m_DlgCornerSearchTool.ShowWindow(TRUE);
					m_DlgGrayBlobInspectTool.ShowWindow(FALSE);
				}
			}
			break;

	}

	
	
}
void  CDlgACFInspectTool::InitDlgItem()
{

	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	m_cTabCtl.InsertItem(0,_T("初定位"));
	m_cTabCtl.InsertItem(1,_T("检测工具"));
	m_cTabCtl.SetCurSel(0);

	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL));
	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL));
	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL));
	m_cmbSearchToolType.AddString(GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL));
	m_cmbSearchToolType.SetCurSel(0);

	CRect rect;
	m_cTabCtl.GetClientRect(rect);

	GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(TRUE);
	GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->ShowWindow(TRUE);

	int nIntendY = 47;
	rect.top+=nIntendY;
	rect.left-=2;
	rect.right+=2;

	CRect rect2;
	rect2 = rect;
	//

	// 粗定位
	//区域角定位
	m_DlgPatternCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgPatternCornerSearchTool.Create(IDD_DLG_PATTERNCORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgPatternCornerSearchTool.MoveWindow(rect2);
	m_DlgPatternCornerSearchTool.ShowWindow(FALSE);
	//几何角定位
	m_DlgCornerSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgCornerSearchTool.Create(IDD_DLG_CORNERSEARCH_TOOL_FORCORNER,&m_cTabCtl);
	m_DlgCornerSearchTool.MoveWindow(rect2);
	m_DlgCornerSearchTool.ShowWindow(FALSE);
	//几何
	m_DlgQuickSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgQuickSearchTool.Create(IDD_DLG_QUICKSEARCH_TOOL,&m_cTabCtl);
	m_DlgQuickSearchTool.MoveWindow(rect2);
	m_DlgQuickSearchTool.ShowWindow(FALSE);
	//区域
	m_DlgPatternSearchTool.m_pDlgParent=m_pDlgParent;
	m_DlgPatternSearchTool.Create(IDD_DLG_PATTERNSEARCH_TOOL,&m_cTabCtl);
	m_DlgPatternSearchTool.MoveWindow(rect2);
	m_DlgPatternSearchTool.ShowWindow(FALSE);
	


	//几何工具隐藏不需要控件
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_CHK_ENABLE_SHARPNESS))->ShowWindow(FALSE);
	(m_DlgQuickSearchTool.GetDlgItem(IDC_EDT_SHARPNESS_THRE2))->ShowWindow(FALSE);
	
	//区域隐藏不需要控件
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_CHK_ENABLE_SHARPNESS))->ShowWindow(FALSE);
	(m_DlgPatternSearchTool.GetDlgItem(IDC_EDT_SHARPNESS_THRE))->ShowWindow(FALSE);

	// 区域角定位工具
	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	//(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);
	//(m_DlgPatternCornerSearchTool.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);


	// 几何角定位工具
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_GET_TRAIN_IMAGE))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_TRAIN))->ShowWindow(FALSE);
	(m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_FULL_IMAGE_SEARCH))->ShowWindow(FALSE);
	//(m_DlgCornerSearchTool.GetDlgItem(IDC_EDT_SEARCH_NUM))->ShowWindow(FALSE);
	//(m_DlgCornerSearchTool.GetDlgItem(IDC_STATIC_SEARCH_NUM))->ShowWindow(FALSE);


	//rect2.top -=20;
	//区域
	m_DlgGrayBlobInspectTool.m_pDlgParent=m_pDlgParent;
	m_DlgGrayBlobInspectTool.Create(IDD_DLG_ACF_GRAY_BLOB_INSPECT_TOOL,&m_cTabCtl);
	m_DlgGrayBlobInspectTool.MoveWindow(rect2);
	m_DlgGrayBlobInspectTool.ShowWindow(FALSE);


}
void CDlgACFInspectTool::CheckDlgDataValidation()
{
	m_DlgPatternSearchTool.CheckDlgDataValidation();
	m_DlgQuickSearchTool.CheckDlgDataValidation();
	m_DlgPatternCornerSearchTool.CheckDlgDataValidation();
	m_DlgCornerSearchTool.CheckDlgDataValidation();
	m_DlgGrayBlobInspectTool.CheckDlgDataValidation();
}
void CDlgACFInspectTool::UpdateControlDisplay()
{
	m_DlgPatternSearchTool.UpdateControlDisplay();
	m_DlgQuickSearchTool.UpdateControlDisplay();
	m_DlgPatternCornerSearchTool.UpdateControlDisplay();
	m_DlgCornerSearchTool.UpdateControlDisplay();
	m_DlgGrayBlobInspectTool.UpdateControlDisplay();
	if (m_pDlgParent->m_pVisionASM->IsPosCameraGrabbing(m_pDlgParent->m_nCurPositionIdx))
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_MARK_GETTRAINIMAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MARK_TRAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_SEARCHTOOL_TYPE)->EnableWindow(TRUE);
	}
	
}

BOOL CDlgACFInspectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnMarkGetTrainImage.SetThemeHelper(&m_themeHelper);
	m_btnMarkTrain.SetThemeHelper(&m_themeHelper);
	m_psaSysInfoStrings = m_pDlgParent->m_psaSysInfoStrings;
	UpdateDialogLanguage();
	InitDlgItem();
	return TRUE;  
}
void CDlgACFInspectTool::UpdateTrainData(bool bValidate)
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
		CSearchToolType tSearchToolType;
		if (m_nSearchToolTypeIndex==0)
		{
			tSearchToolType = ePatternSearchTool;
		}
		else if (m_nSearchToolTypeIndex==1)
		{
			tSearchToolType = eQuickSearchTool;
		}
		if (m_nSearchToolTypeIndex==2)
		{
			tSearchToolType = ePatternCornerSearchTool;
		}
		else if (m_nSearchToolTypeIndex==3)
		{
			tSearchToolType = eCornerSearchTool;
		}

		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->SetRoughSearchToolType(tSearchToolType);
		((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBaseSearch = m_bEnableLocation;
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.UpdateData(TRUE);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_DlgPatternSearchTool.m_nPatAccuracyMode);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_DlgPatternSearchTool.m_nPatSpeedMode+1);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgPatternSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  = m_DlgPatternSearchTool.m_dAngleLow;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh =m_DlgPatternSearchTool.m_dAngleHigh;
			}
			break;
		case 1:
			{

				m_DlgQuickSearchTool.UpdateData(TRUE);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgQuickSearchTool.m_nImageChannel; 
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_DlgQuickSearchTool.m_dGrau; 
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_DlgQuickSearchTool.m_lNoiseThre;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_DlgQuickSearchTool.m_lTotalMag;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgQuickSearchTool.m_bEnableMask;
			}
			break;
		case 2:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				// 区域定位
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgPatternCornerSearchTool.m_nImageChannel;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy = (Accuracy)(1<<m_DlgPatternCornerSearchTool.m_nPatAccuracyMode);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale = (CoarseScale)(m_DlgPatternCornerSearchTool.m_nPatSpeedMode+1);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgPatternCornerSearchTool.m_bEnableMask/*m_imageMask.IsValid() ? m_bEnableMask : false*/;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow  = m_DlgPatternCornerSearchTool.m_dAngleLow;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh =m_DlgPatternCornerSearchTool.m_dAngleHigh;

				// 角定位  找线工具参数设置
				// 找线工具参数设置
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgPatternCornerSearchTool.m_nLineOutLineNum1;					// 忽略个数
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgPatternCornerSearchTool.m_dLineContrastThre1;				// 对比度
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1;			// 滤波半宽
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode1;	// 极性
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgPatternCornerSearchTool.m_bSortByScore1;						// 按分数排序
				//((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[0] = m_DlgPatternCornerSearchTool.m_nFitPointIndex1;					// 拟合点序号
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum1;						// 卡尺个数
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_1;				// 找线工具掩膜
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_DlgPatternCornerSearchTool.m_bShowCaliper_1;					// 显示卡尺

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_DlgPatternCornerSearchTool.m_nLineOutLineNum2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgPatternCornerSearchTool.m_dLineContrastThre2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgPatternCornerSearchTool.m_nLinePolarityMode2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgPatternCornerSearchTool.m_bSortByScore2;
				//((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[1] = m_DlgPatternCornerSearchTool.m_nFitPointIndex2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgPatternCornerSearchTool.m_nLineCaliperNum2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_DlgPatternCornerSearchTool.m_bEnableLineMask_2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_DlgPatternCornerSearchTool.m_bShowCaliper_2;

			}
			break;
		case 3:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nImageChannel = m_DlgCornerSearchTool.m_nImageChannel; 
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau = m_DlgCornerSearchTool.m_dGrau; 
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre = m_DlgCornerSearchTool.m_lNoiseThre;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag = m_DlgCornerSearchTool.m_lTotalMag;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable = m_DlgCornerSearchTool.m_bEnableMask;

				// 角定位  找线工具参数设置
				// 找线工具参数设置
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0]  = m_DlgCornerSearchTool.m_nLineOutLineNum1;					// 忽略个数
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0] = m_DlgCornerSearchTool.m_dLineContrastThre1;				// 对比度
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth1;			// 滤波半宽
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode1;	// 极性
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0] = m_DlgCornerSearchTool.m_bSortByScore1;						// 按分数排序
				//((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[0] = m_DlgCornerSearchTool.m_nFitPointIndex1;					// 拟合点序号
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0] = m_DlgCornerSearchTool.m_nLineCaliperNum1;						// 卡尺个数
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0] = m_DlgCornerSearchTool.m_bEnableLineMask_1;				// 找线工具掩膜
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0] = m_DlgCornerSearchTool.m_bShowCaliper_1;					// 显示卡尺

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1] = m_DlgCornerSearchTool.m_nLineOutLineNum2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1] = m_DlgCornerSearchTool.m_dLineContrastThre2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1] = m_DlgCornerSearchTool.m_lLineFilterHalfWidth2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1] = (EdgePolarity)m_DlgCornerSearchTool.m_nLinePolarityMode2;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1] = m_DlgCornerSearchTool.m_bSortByScore2;
				//((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[1] = m_DlgCornerSearchTool.m_nFitPointIndex2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1] = m_DlgCornerSearchTool.m_nLineCaliperNum2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1] = m_DlgCornerSearchTool.m_bEnableLineMask_2;
				((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1] = m_DlgCornerSearchTool.m_bShowCaliper_2;
			}
			break;
		}

	    m_DlgGrayBlobInspectTool.UpdateData(TRUE);
		{
			int m_nInsToolNum = m_DlgGrayBlobInspectTool.m_nInsToolNum;
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.resize(m_nInsToolNum,1);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.resize(m_nInsToolNum,1);
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

			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);
			m_DlgGrayBlobInspectTool.m_vnRowNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnColNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.resize(m_nInsToolNum,0);

			m_DlgGrayBlobInspectTool.m_vnDefectType.resize(m_nInsToolNum,0);
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.resize(m_nInsToolNum,80);
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.resize(m_nInsToolNum,3);
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);

			m_DlgGrayBlobInspectTool.m_dMinMean.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bMeanInvert.resize(m_nInsToolNum,FALSE);
			m_DlgGrayBlobInspectTool.m_dMinStdDev.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurEnableGrayInspect;
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurEnableBlobInspect;
			m_DlgGrayBlobInspectTool.m_vnRowNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nRowNum;
			m_DlgGrayBlobInspectTool.m_vnColNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nColNum;

			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nAllowedNG;

			m_DlgGrayBlobInspectTool.m_vnDefectType.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) =m_DlgGrayBlobInspectTool.m_nDefectType ;
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nBinaryThre;
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nMinAcceptArea;
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nMaxAcceptArea;
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_dMaxDefectArea;

			m_DlgGrayBlobInspectTool.m_dMinMean.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)    = m_DlgGrayBlobInspectTool.m_dCurMinMean;
			m_DlgGrayBlobInspectTool.m_dMaxMean.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)    = m_DlgGrayBlobInspectTool.m_dCurMaxMean;
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)= !m_DlgGrayBlobInspectTool.m_bMeanNotInvalid;
			m_DlgGrayBlobInspectTool.m_bMeanInvert.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurMeanInvert;

			m_DlgGrayBlobInspectTool.m_dMinStdDev.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_dCurMinStdDev;
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_dCurMaxStdDev;
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = !m_DlgGrayBlobInspectTool.m_bCurStdDevNotInvalid;
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_bCurStdDevInvert;

			for (int i=0;i<m_nInsToolNum;i++)
			{
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i) = m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(i) ;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i) = m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i) = m_DlgGrayBlobInspectTool.m_vnRowNum.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i) = m_DlgGrayBlobInspectTool.m_vnColNum.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i) = m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i) = m_DlgGrayBlobInspectTool.m_vnDefectType.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i) = m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i) = m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i) = m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i) = m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_DlgGrayBlobInspectTool.m_dMinMean.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_DlgGrayBlobInspectTool.m_dMaxMean.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_DlgGrayBlobInspectTool.m_bMeanInvert.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_DlgGrayBlobInspectTool.m_dMinStdDev.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(i);
			}
		}

		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		m_pDlgParent->m_bTrainDataUpdated = TRUE;
		m_pDlgParent->m_bSearchDataUpdated = FALSE;
	}
	else // 设置界面值
	{
		m_bEnableLocation = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBaseSearch;

		if (((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==ePatternSearchTool)
		{
			m_nSearchToolTypeIndex=0;
		}
		else if (((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==eQuickSearchTool)
		{
			m_nSearchToolTypeIndex=1;
		}
		if (((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==ePatternCornerSearchTool)
		{
			m_nSearchToolTypeIndex = 2;
		}
		else if (((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->GetRoughSearchToolType()==eCornerSearchTool)
		{
			m_nSearchToolTypeIndex = 3;
		}
		UpdateData(FALSE);
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{

				m_DlgPatternSearchTool.m_nPatAccuracyMode = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
				m_DlgPatternSearchTool.m_nPatSpeedMode = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
				m_DlgPatternSearchTool.m_dAngleLow	    = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
				m_DlgPatternSearchTool.m_dAngleHigh	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
				m_DlgPatternSearchTool.m_bEnableMask = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

				if (m_DlgPatternSearchTool.m_bEnableMask)
				{
					m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}
				m_DlgPatternSearchTool.m_comboAccuracy.SetCurSel(m_DlgPatternSearchTool.m_nPatAccuracyMode);
				m_DlgPatternSearchTool.m_comboSpeed.SetCurSel(m_DlgPatternSearchTool.m_nPatSpeedMode);

				m_DlgPatternSearchTool.UpdateData(FALSE);

			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.m_dGrau= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
				m_DlgQuickSearchTool.m_lNoiseThre	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
				m_DlgQuickSearchTool.m_lTotalMag		= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
				m_DlgQuickSearchTool.m_bEnableMask	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

				if (m_DlgQuickSearchTool.m_bEnableMask)
				{
					m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgQuickSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}

				m_DlgQuickSearchTool.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				m_DlgPatternCornerSearchTool.m_nPatAccuracyMode = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_Accuracy/2;
				m_DlgPatternCornerSearchTool.m_nPatSpeedMode = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_CoarseScale-1;
				m_DlgPatternCornerSearchTool.m_dAngleLow	    = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleLow;
				m_DlgPatternCornerSearchTool.m_dAngleHigh	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dAngleHigh;
				m_DlgPatternCornerSearchTool.m_bEnableMask = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;	

				if (m_DlgPatternCornerSearchTool.m_bEnableMask)
				{
					m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgPatternCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}


				// 角定位  找线工具参数设置
				// 找线工具参数设置
				m_DlgPatternCornerSearchTool.m_nLineOutLineNum1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];					
				m_DlgPatternCornerSearchTool.m_dLineContrastThre1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];				
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];			
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];	
				m_DlgPatternCornerSearchTool.m_bSortByScore1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];						
				//m_DlgPatternCornerSearchTool.m_nFitPointIndex1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[0];					
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];						
				m_DlgPatternCornerSearchTool.m_bEnableLineMask_1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];			
				m_DlgPatternCornerSearchTool.m_bShowCaliper_1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];			

				m_DlgPatternCornerSearchTool.m_nLineOutLineNum2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgPatternCornerSearchTool.m_dLineContrastThre2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgPatternCornerSearchTool.m_lLineFilterHalfWidth2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgPatternCornerSearchTool.m_nLinePolarityMode2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgPatternCornerSearchTool.m_bSortByScore2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				//m_DlgPatternCornerSearchTool.m_nFitPointIndex2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[1];
				m_DlgPatternCornerSearchTool.m_nLineCaliperNum2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgPatternCornerSearchTool.m_bEnableLineMask_2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
				m_DlgPatternCornerSearchTool.m_bShowCaliper_2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];

				m_DlgPatternCornerSearchTool.m_comboAccuracy.SetCurSel(m_DlgPatternCornerSearchTool.m_nPatAccuracyMode);
				m_DlgPatternCornerSearchTool.m_comboSpeed.SetCurSel(m_DlgPatternCornerSearchTool.m_nPatSpeedMode);

				m_DlgPatternCornerSearchTool.UpdateData(FALSE);
			}
			break;

		case 3:
			{
				m_DlgCornerSearchTool.m_dGrau = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dGrau;
				m_DlgCornerSearchTool.m_lNoiseThre	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lNoiseThre;
				m_DlgCornerSearchTool.m_lTotalMag		= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lTotalMag;
				m_DlgCornerSearchTool.m_bEnableMask	= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMaskEnable;

				if (m_DlgCornerSearchTool.m_bEnableMask)
				{
					m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(TRUE);
				}
				else
				{
					m_DlgCornerSearchTool.GetDlgItem(IDC_BTN_SET_MASK_IMAGE)->EnableWindow(FALSE);
				}

				m_DlgCornerSearchTool.m_nLineOutLineNum1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[0];					
				m_DlgCornerSearchTool.m_dLineContrastThre1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[0];				
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[0];			
				m_DlgCornerSearchTool.m_nLinePolarityMode1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[0];	
				m_DlgCornerSearchTool.m_bSortByScore1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[0];						
				//m_DlgCornerSearchTool.m_nFitPointIndex1 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[0];					
				m_DlgCornerSearchTool.m_nLineCaliperNum1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[0];						
				m_DlgCornerSearchTool.m_bEnableLineMask_1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[0];			
				m_DlgCornerSearchTool.m_bShowCaliper_1 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[0];			

				m_DlgCornerSearchTool.m_nLineOutLineNum2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nOutLineNum[1];
				m_DlgCornerSearchTool.m_dLineContrastThre2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dContrastThre[1];
				m_DlgCornerSearchTool.m_lLineFilterHalfWidth2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_lFilterHalfWidth[1];
				m_DlgCornerSearchTool.m_nLinePolarityMode2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_edgeProperty[1];
				m_DlgCornerSearchTool.m_bSortByScore2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bSortByScore[1];
				//m_DlgCornerSearchTool.m_nFitPointIndex2 = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nFitPointIndex[1];
				m_DlgCornerSearchTool.m_nLineCaliperNum2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_nCaliperNum[1];
				m_DlgCornerSearchTool.m_bEnableLineMask_2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bEnableLineMask[1];
				m_DlgCornerSearchTool.m_bShowCaliper_2 = ((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_bCaliperVisible[1];

				m_DlgCornerSearchTool.UpdateData(FALSE);
			}
			break;
		}
		
		{
			m_DlgGrayBlobInspectTool.m_nInsToolNum = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;
			int m_nInsToolNum = m_DlgGrayBlobInspectTool.m_nInsToolNum;
			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_vnRowNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnColNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.resize(m_nInsToolNum,0);
			m_DlgGrayBlobInspectTool.m_vnDefectType.resize(m_nInsToolNum,0);
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.resize(m_nInsToolNum,80);
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.resize(m_nInsToolNum,3);
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);


			m_DlgGrayBlobInspectTool.m_dMinMean.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bMeanInvert.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_dMinStdDev.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.resize(m_nInsToolNum,FALSE);


			for (int i=0;i<m_nInsToolNum;i++)
			{
				m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i);
				m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i);

				m_DlgGrayBlobInspectTool.m_vnRowNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i);
				m_DlgGrayBlobInspectTool.m_vnColNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i);
				m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i);

				m_DlgGrayBlobInspectTool.m_vnDefectType.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i);
				m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i);
				m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i);
				m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i);
				m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i);

				m_DlgGrayBlobInspectTool.m_dMinMean.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
				m_DlgGrayBlobInspectTool.m_dMaxMean.at(i)=	((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
				m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
				m_DlgGrayBlobInspectTool.m_bMeanInvert.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

				m_DlgGrayBlobInspectTool.m_dMinStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
				m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
				m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
				m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
			}

			int m_nCurToolIndex = 0;
			m_DlgGrayBlobInspectTool.m_nCurToolIndex = m_nCurToolIndex;

			m_DlgGrayBlobInspectTool.m_nRowNum = m_DlgGrayBlobInspectTool.m_vnRowNum.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nColNum = m_DlgGrayBlobInspectTool.m_vnColNum.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nAllowedNG = m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(m_nCurToolIndex);

			m_DlgGrayBlobInspectTool.m_bCurEnableGrayInspect = m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurEnableBlobInspect = m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(m_nCurToolIndex);

			m_DlgGrayBlobInspectTool.m_nDefectType = m_DlgGrayBlobInspectTool.m_vnDefectType.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nBinaryThre = m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nMinAcceptArea = m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nMaxAcceptArea = m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_dMaxDefectArea = m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(m_nCurToolIndex);

			m_DlgGrayBlobInspectTool.m_dCurMinMean = m_DlgGrayBlobInspectTool.m_dMinMean.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_dCurMaxMean = m_DlgGrayBlobInspectTool.m_dMaxMean.at(m_nCurToolIndex)    ;
			m_DlgGrayBlobInspectTool.m_bMeanNotInvalid = !m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurMeanInvert = m_DlgGrayBlobInspectTool.m_bMeanInvert.at(m_nCurToolIndex) ;

			m_DlgGrayBlobInspectTool.m_dCurMinStdDev = m_DlgGrayBlobInspectTool.m_dMinStdDev.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_dCurMaxStdDev = m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_bCurStdDevNotInvalid = !m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurStdDevInvert = m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(m_nCurToolIndex) ;

			CString str;
			m_DlgGrayBlobInspectTool.m_SelAreaIndex.ResetContent();
			for (int i=0;i<m_DlgGrayBlobInspectTool.m_nInsToolNum;i++)
			{
				str.Format("%d",i+1);
				m_DlgGrayBlobInspectTool.m_SelAreaIndex.InsertString(i,str);
			}
			if (m_nCurToolIndex>=m_DlgGrayBlobInspectTool.m_SelAreaIndex.GetCount())
			{
				m_nCurToolIndex = 0;
			}
			m_DlgGrayBlobInspectTool.m_SelAreaIndex.SetCurSel(m_nCurToolIndex);

			if (((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
			{
				m_DlgGrayBlobInspectTool.GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));
			}
			else
			{
				m_DlgGrayBlobInspectTool.GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
			}

			m_DlgGrayBlobInspectTool.m_dCurMean =  floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
			m_DlgGrayBlobInspectTool.m_dCurStdDev = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;

			m_DlgGrayBlobInspectTool.UpdateData(FALSE);
		}

	}

}
void CDlgACFInspectTool::UpdateSearchData(bool bValidate)
{
	if (!m_pDlgParent->m_pSearchTool)
	{
		return;
	}

	m_pDlgParent->m_pSearchTool->GetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
	m_pDlgParent->m_pSearchTool->GetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
	m_pDlgParent->m_pSearchTool->GetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
	m_pDlgParent->m_pSearchTool->GetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);

	if (bValidate)	// 从界面获取训练参数
	{
		UpdateData(TRUE);
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.UpdateData(TRUE);
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgPatternSearchTool.m_nSearchNum;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgPatternSearchTool.m_dAcceptThre;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_DlgPatternSearchTool.m_dConfusionThre;
				
			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.UpdateData(TRUE);
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgQuickSearchTool.m_nSearchNum;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgQuickSearchTool.m_dAcceptThre;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_DlgQuickSearchTool.m_dAngleLow;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_DlgQuickSearchTool.m_dAngleHigh;

			}
			break;
		case 2:
			{
				m_DlgPatternCornerSearchTool.UpdateData(TRUE);
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgPatternCornerSearchTool.m_nSearchNum;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgPatternCornerSearchTool.m_dAcceptThre;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre = m_DlgPatternCornerSearchTool.m_dConfusionThre;

				// 角定位
				((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_DlgPatternCornerSearchTool.m_bShowFitPoint_1;
				((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_DlgPatternCornerSearchTool.m_bShowFitPoint_2;
			}
			break;
		case 3:
			{
				m_DlgCornerSearchTool.UpdateData(TRUE);
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum = m_DlgCornerSearchTool.m_nSearchNum;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre = m_DlgCornerSearchTool.m_dAcceptThre;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow = m_DlgCornerSearchTool.m_dAngleLow;
				((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh = m_DlgCornerSearchTool.m_dAngleHigh;

				
				((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint = m_DlgCornerSearchTool.m_bShowFitPoint_1;			// 拟合点是否可见
				((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1 = m_DlgCornerSearchTool.m_bShowFitPoint_2;
			}
			break;
		}

		m_DlgGrayBlobInspectTool.UpdateData(TRUE);
		{
			int m_nInsToolNum = m_DlgGrayBlobInspectTool.m_nInsToolNum;
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum= m_nInsToolNum;
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.resize(m_nInsToolNum,1);
			((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.resize(m_nInsToolNum,1);
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


			((CACFInspectTrainGuiParam*)m_pDlgParent->m_pTrainGuiParam)->m_InspectAffineRect.resize(m_nInsToolNum);

			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);
			m_DlgGrayBlobInspectTool.m_vnRowNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnColNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.resize(m_nInsToolNum,0);

			m_DlgGrayBlobInspectTool.m_vnDefectType.resize(m_nInsToolNum,0);
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.resize(m_nInsToolNum,80);
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.resize(m_nInsToolNum,3);
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);

			m_DlgGrayBlobInspectTool.m_dMinMean.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bMeanInvert.resize(m_nInsToolNum,FALSE);
			m_DlgGrayBlobInspectTool.m_dMinStdDev.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurEnableGrayInspect;
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurEnableBlobInspect;
			m_DlgGrayBlobInspectTool.m_vnRowNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nRowNum;
			m_DlgGrayBlobInspectTool.m_vnColNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nColNum;
			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nAllowedNG;

			m_DlgGrayBlobInspectTool.m_vnDefectType.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) =m_DlgGrayBlobInspectTool.m_nDefectType ;
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nBinaryThre;
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nMinAcceptArea;
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_nMaxAcceptArea;
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_dMaxDefectArea;

			m_DlgGrayBlobInspectTool.m_dMinMean.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)    = m_DlgGrayBlobInspectTool.m_dCurMinMean;
			m_DlgGrayBlobInspectTool.m_dMaxMean.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)    = m_DlgGrayBlobInspectTool.m_dCurMaxMean;
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)= !m_DlgGrayBlobInspectTool.m_bMeanNotInvalid;
			m_DlgGrayBlobInspectTool.m_bMeanInvert.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = m_DlgGrayBlobInspectTool.m_bCurMeanInvert;

			m_DlgGrayBlobInspectTool.m_dMinStdDev.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_dCurMinStdDev;
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_dCurMaxStdDev;
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex) = !m_DlgGrayBlobInspectTool.m_bCurStdDevNotInvalid;
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(m_DlgGrayBlobInspectTool.m_nCurToolIndex)  = m_DlgGrayBlobInspectTool.m_bCurStdDevInvert;

			for (int i=0;i<m_nInsToolNum;i++)
			{
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i) = m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(i) ;
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i) = m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i) = m_DlgGrayBlobInspectTool.m_vnRowNum.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i) = m_DlgGrayBlobInspectTool.m_vnColNum.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i) = m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i) = m_DlgGrayBlobInspectTool.m_vnDefectType.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i) = m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i) = m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i) = m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i) = m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i)=			m_DlgGrayBlobInspectTool.m_dMinMean.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i)=			m_DlgGrayBlobInspectTool.m_dMaxMean.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i)=		m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i) =		m_DlgGrayBlobInspectTool.m_bMeanInvert.at(i);

				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i) =        m_DlgGrayBlobInspectTool.m_dMinStdDev.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) =        m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i) =    m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(i);
				((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i) =     m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(i);
			}
		}

		m_pDlgParent->m_pSearchTool->SetSearchDataParam(m_pDlgParent->m_pSearchDataParam);
		m_pDlgParent->m_pSearchTool->SetSearchGuiParam(m_pDlgParent->m_pSearchGuiParam);
		m_pDlgParent->m_pSearchTool->SetTrainDataParam(m_pDlgParent->m_pTrainDataParam);
		m_pDlgParent->m_pSearchTool->SetTrainGuiParam(m_pDlgParent->m_pTrainGuiParam);
		m_pDlgParent->m_bSearchDataUpdated = TRUE;
	}
	else // 设置界面值
	{

		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			{
				m_DlgPatternSearchTool.m_nSearchNum	= ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgPatternSearchTool.m_dAcceptThre	= ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
				m_DlgPatternSearchTool.m_dConfusionThre = ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

				m_DlgPatternSearchTool.UpdateData(FALSE);
			}
			break;
		case 1:
			{
				m_DlgQuickSearchTool.m_nSearchNum	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgQuickSearchTool.m_dAcceptThre	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
				m_DlgQuickSearchTool.m_dAngleLow		= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
				m_DlgQuickSearchTool.m_dAngleHigh	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;
             	m_DlgQuickSearchTool.UpdateData(FALSE);
			}
			break;
		case 2:
			{
				m_DlgPatternCornerSearchTool.m_nSearchNum	= ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgPatternCornerSearchTool.m_dAcceptThre	= ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;	
				m_DlgPatternCornerSearchTool.m_dConfusionThre = ((CACFInspectSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dConfusionThre;

				m_DlgPatternCornerSearchTool.m_bShowFitPoint_1 = ((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;			// 拟合点是否可见
				m_DlgPatternCornerSearchTool.m_bShowFitPoint_2 = ((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;
				m_DlgPatternCornerSearchTool.UpdateData(FALSE);
			}
			break;
		case 3:
			{
				m_DlgCornerSearchTool.m_nSearchNum	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_nSearchNum;
				m_DlgCornerSearchTool.m_dAcceptThre	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAcceptThre;
				m_DlgCornerSearchTool.m_dAngleLow	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleLow;
				m_DlgCornerSearchTool.m_dAngleHigh	= ((CCircleCornerCheckSearchSearchDataParam*)m_pDlgParent->m_pSearchDataParam)->m_dAngleHigh;

				m_DlgCornerSearchTool.m_bShowFitPoint_1 = ((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint;			// 拟合点是否可见
				m_DlgCornerSearchTool.m_bShowFitPoint_2 = ((CACFInspectSearchGuiParam*)m_pDlgParent->m_pSearchGuiParam)->m_bEnableShowLinePoint1;
				m_DlgCornerSearchTool.UpdateData(FALSE);
			}
			break;
		}

		{
			m_DlgGrayBlobInspectTool.m_nInsToolNum = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nInsAreaNum;
			int m_nInsToolNum = m_DlgGrayBlobInspectTool.m_nInsToolNum;
			m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_vnRowNum.resize(m_nInsToolNum,1);
			m_DlgGrayBlobInspectTool.m_vnColNum.resize(m_nInsToolNum,1);

			m_DlgGrayBlobInspectTool.m_vnAllowNGNum.resize(m_nInsToolNum,0);

			m_DlgGrayBlobInspectTool.m_vnDefectType.resize(m_nInsToolNum,0);
			m_DlgGrayBlobInspectTool.m_vnBinaryThre.resize(m_nInsToolNum,80);
			m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.resize(m_nInsToolNum,3);
			m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.resize(m_nInsToolNum,100000);
			m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.resize(m_nInsToolNum,50.0);


			m_DlgGrayBlobInspectTool.m_dMinMean.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxMean.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bMeanInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bMeanInvert.resize(m_nInsToolNum,FALSE);

			m_DlgGrayBlobInspectTool.m_dMinStdDev.resize(m_nInsToolNum,0.0);
			m_DlgGrayBlobInspectTool.m_dMaxStdDev.resize(m_nInsToolNum,255.0);
			m_DlgGrayBlobInspectTool.m_bStdDevInvalid.resize(m_nInsToolNum,TRUE);
			m_DlgGrayBlobInspectTool.m_bStdDevInvert.resize(m_nInsToolNum,FALSE);


			for (int i=0;i<m_nInsToolNum;i++)
			{
				m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableHistogram.at(i);
				m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bEnableBlob.at(i);

				m_DlgGrayBlobInspectTool.m_vnRowNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nRowNum.at(i);
				m_DlgGrayBlobInspectTool.m_vnColNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nColNum.at(i);

				m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nNGAllowed.at(i);

				m_DlgGrayBlobInspectTool.m_vnDefectType.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nDefectType.at(i);
				m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nBinaryThre.at(i);
				m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMinAcceptArea.at(i);
				m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_nMaxAcceptArea.at(i);
				m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxDefectArea.at(i);

				m_DlgGrayBlobInspectTool.m_dMinMean.at(i) = ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinMean.at(i);
				m_DlgGrayBlobInspectTool.m_dMaxMean.at(i)=	((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxMean.at(i);
				m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvalid.at(i);
				m_DlgGrayBlobInspectTool.m_bMeanInvert.at(i)=((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bMeanInvert.at(i);

				m_DlgGrayBlobInspectTool.m_dMinStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMinStdDev.at(i);
				m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_dMaxStdDev.at(i) ;
				m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvalid.at(i);
				m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(i)= ((CACFInspectTrainDataParam*)m_pDlgParent->m_pTrainDataParam)->m_bStdDevInvert.at(i);
			}

			int m_nCurToolIndex = 0;
			m_DlgGrayBlobInspectTool.m_nCurToolIndex = m_nCurToolIndex;

			m_DlgGrayBlobInspectTool.m_nRowNum = m_DlgGrayBlobInspectTool.m_vnRowNum.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nColNum = m_DlgGrayBlobInspectTool.m_vnColNum.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nAllowedNG = m_DlgGrayBlobInspectTool.m_vnAllowNGNum.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurEnableGrayInspect = m_DlgGrayBlobInspectTool.m_vbCurEnableGrayInspect.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurEnableBlobInspect = m_DlgGrayBlobInspectTool.m_vbCurEnableBlobInspect.at(m_nCurToolIndex);

			m_DlgGrayBlobInspectTool.m_nDefectType = m_DlgGrayBlobInspectTool.m_vnDefectType.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nBinaryThre = m_DlgGrayBlobInspectTool.m_vnBinaryThre.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nMinAcceptArea = m_DlgGrayBlobInspectTool.m_vnMinAcceptArea.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_nMaxAcceptArea = m_DlgGrayBlobInspectTool.m_vnMaxAcceptArea.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_dMaxDefectArea = m_DlgGrayBlobInspectTool.m_vdMaxDefectArea.at(m_nCurToolIndex);

			m_DlgGrayBlobInspectTool.m_dCurMinMean = m_DlgGrayBlobInspectTool.m_dMinMean.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_dCurMaxMean = m_DlgGrayBlobInspectTool.m_dMaxMean.at(m_nCurToolIndex)    ;
			m_DlgGrayBlobInspectTool.m_bMeanNotInvalid = !m_DlgGrayBlobInspectTool.m_bMeanInvalid.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurMeanInvert = m_DlgGrayBlobInspectTool.m_bMeanInvert.at(m_nCurToolIndex) ;

			m_DlgGrayBlobInspectTool.m_dCurMinStdDev = m_DlgGrayBlobInspectTool.m_dMinStdDev.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_dCurMaxStdDev = m_DlgGrayBlobInspectTool.m_dMaxStdDev.at(m_nCurToolIndex) ;
			m_DlgGrayBlobInspectTool.m_bCurStdDevNotInvalid = !m_DlgGrayBlobInspectTool.m_bStdDevInvalid.at(m_nCurToolIndex);
			m_DlgGrayBlobInspectTool.m_bCurStdDevInvert = m_DlgGrayBlobInspectTool.m_bStdDevInvert.at(m_nCurToolIndex) ;

			CString str;
			m_DlgGrayBlobInspectTool.m_SelAreaIndex.ResetContent();
			for (int i=0;i<m_DlgGrayBlobInspectTool.m_nInsToolNum;i++)
			{
				str.Format("%d",i+1);
				m_DlgGrayBlobInspectTool.m_SelAreaIndex.InsertString(i,str);
			}
			if (m_nCurToolIndex>=m_DlgGrayBlobInspectTool.m_SelAreaIndex.GetCount())
			{
				m_nCurToolIndex = 0;
			}
			m_DlgGrayBlobInspectTool.m_SelAreaIndex.SetCurSel(m_nCurToolIndex);

			if (((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_bInspectOK)
			{
				m_DlgGrayBlobInspectTool.GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("OK"));

			}
			else
			{
				m_DlgGrayBlobInspectTool.GetDlgItem(IDC_EDT_INSPECT_RESULT)->SetWindowTextA(_T("NG"));
			}

			m_DlgGrayBlobInspectTool.m_dCurMean =  floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurMean.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;
			m_DlgGrayBlobInspectTool.m_dCurStdDev = floor(((CACFInspectTool*)m_pDlgParent->m_pSearchTool)->m_dCurStdDev.at(m_nCurToolIndex)*100.00f+0.5)/100.00f;

		}
		m_DlgGrayBlobInspectTool.UpdateData(FALSE);
		UpdateData(FALSE);
	}
}
void CDlgACFInspectTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateDlgItem();
	*pResult = 0;
}


void CDlgACFInspectTool::OnCbnSelchangeCmbSearchtoolType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgItem();
	OnBnClickedBtnMarkGettrainimage();
}

void CDlgACFInspectTool::OnBnClickedBtnMarkGettrainimage()
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

	switch(m_nSearchToolTypeIndex)
	{
	case 0:
		{
			if (m_DlgPatternSearchTool.m_bEnableMask)
			{
				if ( (m_DlgPatternSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternSearchTool.m_imageMask.Release();
				}
			}
		}
		break;
	case 1:
		{
			if (m_DlgQuickSearchTool.m_bEnableMask)
			{
				if ( (m_DlgQuickSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgQuickSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgQuickSearchTool.m_imageMask.Release();
				}
			}

		}
		break;
	case 2:
		{
			if (m_DlgPatternCornerSearchTool.m_bEnableMask)
			{
				if ( (m_DlgPatternCornerSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgPatternCornerSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgPatternCornerSearchTool.m_imageMask.Release();
				}
			}

		}
		break;
	case 3:
		{
			if (m_DlgCornerSearchTool.m_bEnableMask)
			{
				if ( (m_DlgCornerSearchTool.m_imageMask.Height() != m_pDlgParent->m_imageTrain.Height())
					|| (m_DlgCornerSearchTool.m_imageMask.Width() != m_pDlgParent->m_imageTrain.Width()) )
				{
					m_DlgCornerSearchTool.m_imageMask.Release();
				}
			}

		}
		break;
	}
	
	m_pDlgParent->m_comboDisplayType.SetCurSel(1);
	m_pDlgParent->UpdateGuiDisplay();

	UpdateControlDisplay();
}


void CDlgACFInspectTool::OnBnClickedBtnMarkTrain()
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
		
		switch(m_nSearchToolTypeIndex)
		{
		case 0:
			if (m_DlgPatternSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgPatternSearchTool.m_imageMask);
			}
			break;
		case 1:
			if (m_DlgQuickSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgQuickSearchTool.m_imageMask);
			}
			break;
		case 2:
			if (m_DlgPatternCornerSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgPatternCornerSearchTool.m_imageMask);
			}
			break;
		case 3:
			if (m_DlgCornerSearchTool.m_bEnableMask)
			{
				m_pDlgParent->m_pSearchTool->SetMaskImage(m_DlgCornerSearchTool.m_imageMask);
			}
			break;

		}
		

		if (m_pDlgParent->m_pSearchTool->Train())
		{
			cpImage trainImage;
			m_pDlgParent->m_pSearchTool->GetTrainResultImage(trainImage);
			switch(m_nSearchToolTypeIndex)
			{
			case 0:
				m_DlgPatternSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 1:
				m_DlgQuickSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 2:
				m_DlgPatternCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			case 3:
				m_DlgCornerSearchTool.m_GuiPatternDisplay.SetImage(trainImage, TRUE);
				break;
			}
            trainImage.Release();
			
			
			m_pDlgParent->m_pSearchTool->UpdateGuiDisplay();
		
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
}


//将搜索的结果更新到界面上
void CDlgACFInspectTool::UpdateSearchResult()
{
	m_DlgGrayBlobInspectTool.UpdateSearchResult();
}


void CDlgACFInspectTool::OnBnClickedChkEnableBasesearchCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnBnClickedBtnMarkGettrainimage();
}
