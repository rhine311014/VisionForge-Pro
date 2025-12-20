// DlgSearchToolModelLibrary.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSearchToolModelLibrary.h"
#include "afxdialogex.h"
#include "DlgSearch.h"



// CDlgSearchToolModelLibrary 对话框

IMPLEMENT_DYNAMIC(CDlgSearchToolModelLibrary, CDialog)

CDlgSearchToolModelLibrary::CDlgSearchToolModelLibrary(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchToolModelLibrary::IDD, pParent)
	, m_bUsingCalib(FALSE)
	, m_bUsingTarget(FALSE)
	, m_bUsingObject(FALSE)
	, m_bUsingCalibTarget(FALSE)
	, m_bUsingInspect(FALSE)
	, m_bUsingVirtual(FALSE)
	, m_strPreToolType(_T("----------"))
	, m_strPrePlatformIndex(_T("----------"))
	, m_strPreProductName(_T("----------"))
	, m_strPreProductIndex(_T("----------"))
	, m_strPreModelType(_T("----------"))
	, m_strPreModelPosition(_T("----------"))
	, m_bIsLoadAllProduct(TRUE)
{
	m_nCurPlatformIndex= -1;
	m_nCurPageIndex = 1;
	m_nMaxPageNum = 1;
	m_nCurSelectImageIndex = -1;
	m_nImageNum  = 0;
	m_bUsingCalib = TRUE;
	m_bUsingCalibTarget = TRUE;
	m_bUsingTarget = TRUE;
	m_bUsingObject = TRUE;
	m_bUsingInspect = TRUE;
	m_bUsingVirtual = TRUE;
	
	m_bSysKeyboardEnabled = FALSE;
	colorSelect = RGB(82,128,201);
	colorNormal = RGB(128,255,128);
	m_pBaseSearchTool = NULL;
	for (int i = 0;i<sizeof(m_bIsLoadPlatformTool)/sizeof(m_bIsLoadPlatformTool[0]);i++)
	{
		m_bIsLoadPlatformTool[i] = FALSE;
	}

	m_vbLoadAllProduct.resize(6,true);
	m_nComboSelectPlatformIndex = 0;
	m_vvnComboSelectProductIndex.resize(6);
	for (int i =0;i<6;i++)
	{
		m_vvnComboSelectProductIndex[i].resize(2,0);
	}

	m_nSearchModeFrom = eNoSearch; 
	m_nPosIndexFrom = -1;
	m_nCandidatePosFrom = -1;
}

CDlgSearchToolModelLibrary::~CDlgSearchToolModelLibrary()
{
	ClearcpImage();
	ClearGuiDisplay();

	if (m_pStatusBar != NULL)
	{
		delete m_pStatusBar;
		m_pStatusBar = NULL;
	}
	if (m_hIconSucceed != NULL)
	{
		DestroyIcon(m_hIconSucceed);
		m_hIconSucceed = NULL;
	}
	if (m_hIconFailed != NULL)
	{
		DestroyIcon(m_hIconFailed);
		m_hIconFailed = NULL;
	}
	if(m_pBaseSearchTool!= NULL)
	{
		delete m_pBaseSearchTool;
		m_pBaseSearchTool = NULL;
	}
	
}
// 释放GuiDisplay
bool CDlgSearchToolModelLibrary::ClearGuiDisplay()
{
	for(int i = 0; i < m_vpGuiDisplay.size(); i++)
	{
		scGuiStaticContainer* pCContainer = m_vpGuiDisplay.at(i)->GetConstContainer();
		CString strKey;
		strKey.Format(_T("GuiText%d"),i);
		pCContainer->RemoveItem(strKey);
	}
	for(int i = 0; i < m_vpGuiText.size(); i++)
	{
		if(m_vpGuiText[i]!=NULL)
		{
			delete m_vpGuiText[i];
			m_vpGuiText[i] = NULL;
		}
	}
	
	for(int i = 0; i < m_vpGuiDisplay.size(); i++)
	{
		delete m_vpGuiDisplay[i];
		m_vpGuiDisplay[i] = NULL;
	}
	if(m_pGuiDisplayPreview!= NULL)
	{
		delete m_pGuiDisplayPreview;
		m_pGuiDisplayPreview = NULL;
	}


	
	return true;
}
// 释放所有模板图像信息
bool CDlgSearchToolModelLibrary::ClearcpImage()
{
	for(int i = 0; i < m_vcpImage.size(); i++)
	{
		m_vcpImage[i].Release();
	}
	return true;
}

void CDlgSearchToolModelLibrary::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_STATIC_GUICONTENT0, m_labelGuiContent0);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT1, m_labelGuiContent1);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT2, m_labelGuiContent2);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT3, m_labelGuiContent3);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT4, m_labelGuiContent4);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT5, m_labelGuiContent5);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT6, m_labelGuiContent6);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT7, m_labelGuiContent7);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT8, m_labelGuiContent8);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT9, m_labelGuiContent9);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT10, m_labelGuiContent10);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT11, m_labelGuiContent11);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT12, m_labelGuiContent12);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT13, m_labelGuiContent13);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT14, m_labelGuiContent14);
	DDX_Control(pDX, IDC_STATIC_GUICONTENT15, m_labelGuiContent15);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_PREVIOUS_PAGE, m_btnPreviouPage);
	DDX_Control(pDX, IDC_BTN_NEXT_PAGE, m_btnNextPage);
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CALIB, m_bUsingCalib);
	DDX_Check(pDX, IDC_CHECK_TARGET, m_bUsingTarget);
	DDX_Check(pDX, IDC_CHECK_OBJECT, m_bUsingObject);
	DDX_Check(pDX, IDC_CHECK_CALIBTARGET, m_bUsingCalibTarget);
	DDX_Check(pDX, IDC_CHECK_INSPECT, m_bUsingInspect);
	DDX_Check(pDX, IDC_CHECK_VIRTUAL, m_bUsingVirtual);
	DDX_Text(pDX, IDC_STATIC_TOOL_NAME_PREVIOU, m_strPreToolType);
	DDX_Text(pDX, IDC_STATIC_PLATFORM_PREVIOU, m_strPrePlatformIndex);
	DDX_Text(pDX, IDC_STATIC_PRODUCT_NAME_PREVIOU, m_strPreProductName);
	DDX_Text(pDX, IDC_STATIC_PRODUCT_INDEX_PREVIOU, m_strPreProductIndex);
	DDX_Text(pDX, IDC_STATIC_MODEL_POS_PREVIOU, m_strPreModelType);
	DDX_Text(pDX, IDC_STATIC_DETAIL_POS_PREVIOU, m_strPreModelPosition);
	//DDX_Radio(pDX, IDC_RADIO_PREVIEW_TEXT_SETTING0, m_nPreviewTextType);
	DDX_Control(pDX, IDC_COMBO_LOADPRODUCT_PLATFORM, m_ComboLoadPlatform);
	DDX_Control(pDX, IDC_COMBO_LOADPRODUCT_BIGPRODUCT, m_ComboLoadBigProduct);
	DDX_Control(pDX, IDC_COMBO_LOADPRODUCT_SMALLPRODUCT, m_ComboLoadSmallProduct);
	DDX_Radio(pDX, IDC_RADIO_LOAD_SINGLEPRODUCT, m_bIsLoadAllProduct);
}


BEGIN_MESSAGE_MAP(CDlgSearchToolModelLibrary, CDialog)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_MESSAGE(WM_SV_DISPLAY_CLICK, OnGuiDisplayClk)
	ON_BN_CLICKED(IDC_BTN_PREVIOUS_PAGE, &CDlgSearchToolModelLibrary::OnBnClickedBtnPreviousPage)
	ON_BN_CLICKED(IDC_BTN_NEXT_PAGE, &CDlgSearchToolModelLibrary::OnBnClickedBtnNextPage)
	ON_BN_CLICKED(IDOK, &CDlgSearchToolModelLibrary::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_CALIB, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_CALIBTARGET, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_INSPECT, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_TARGET, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_OBJECT, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_VIRTUAL, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM1, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM2, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM3, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM4, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM5, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PLATFORM6, &CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData)

	ON_CBN_SELCHANGE(IDC_COMBO_LOADPRODUCT_PLATFORM, OnSelchangeCmbLoadProductPlatform)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT, OnSelchangeCmbLoadProductChProduct)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADPRODUCT_BIGPRODUCT, OnSelchangeCmbLoadProductFaProduct)
	ON_BN_CLICKED(IDC_RADIO_LOAD_ALLPRODUCT, &CDlgSearchToolModelLibrary::OnBnClickedRadioLoadAllproduct)
	ON_BN_CLICKED(IDC_RADIO_LOAD_SINGLEPRODUCT, &CDlgSearchToolModelLibrary::OnBnClickedRadioLoadAllproduct)
END_MESSAGE_MAP()


// CDlgSearchToolModelLibrary 消息处理程序


BOOL CDlgSearchToolModelLibrary::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	// 创建状态栏
	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.top = rcClient.bottom - 20;
	int nPartWidth[3] = {20, 300, -1};
	m_pStatusBar = new CStatusBarCtrl;
	m_pStatusBar->Create(WS_VISIBLE|CBRS_BOTTOM, rcClient, this, IDC_STATUS_BAR_CTRL);
	m_pStatusBar->SetParts(3, nPartWidth);
	m_pStatusBar->ShowWindow(SW_SHOW);

	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), RT_GROUP_ICON);
	m_hIconSucceed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_SUCCEED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	m_hIconFailed = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_FAILED_16), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	// 刷新状态栏
	UpdateStatusBar();
	// 刷新语言工具
	UpdateDialogLanguage();
	// 获取工具路径
	CString strModuelDir;
	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
	strModuelDir.ReleaseBuffer();
	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\')); 


	InitDlgItem();//初始化Gui界面
	LoadAllToolTrainImage();//加载所有定位工具训练图像
	UpdateDlgData();//刷新界面数据
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSearchToolModelLibrary::UpdateDialogLanguage()
{
	m_nlgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		m_nlgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

	switch (m_nlgidLanguage)
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

		if(_T("IDD_DLG_SEARCH_TOOL_MODEL_LIBRARY") == strDlgID)
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
//初始化Gui界面
void CDlgSearchToolModelLibrary::InitDlgItem()
{
	// 初始化图片预览Gui
	scGuiDisplay *pGUIDisplay = NULL;
	for (int j = 0; j < 16; j++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_MODEL_DISPLAY0+  j, this, WS_BORDER);
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_CLICK );
		m_vpGuiDisplay.push_back(pGUIDisplay);
		pGUIDisplay = NULL;
	}
	// 初始化“模板信息预览”模块的Gui
	m_pGuiDisplayPreview = new scGuiDisplay;
	m_pGuiDisplayPreview->Create(IDC_STATIC_MODEL_DISPLAY_PREVIOU,this,WS_BORDER);
	m_pGuiDisplayPreview->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
	// 初始化显示模板信息的Label
	m_vpLabelImageContent.push_back(&m_labelGuiContent0);
	m_vpLabelImageContent.push_back(&m_labelGuiContent1);
	m_vpLabelImageContent.push_back(&m_labelGuiContent2);
	m_vpLabelImageContent.push_back(&m_labelGuiContent3);
	m_vpLabelImageContent.push_back(&m_labelGuiContent4);
	m_vpLabelImageContent.push_back(&m_labelGuiContent5);
	m_vpLabelImageContent.push_back(&m_labelGuiContent6);
	m_vpLabelImageContent.push_back(&m_labelGuiContent7);
	m_vpLabelImageContent.push_back(&m_labelGuiContent8);
	m_vpLabelImageContent.push_back(&m_labelGuiContent9);
	m_vpLabelImageContent.push_back(&m_labelGuiContent10);
	m_vpLabelImageContent.push_back(&m_labelGuiContent11);
	m_vpLabelImageContent.push_back(&m_labelGuiContent12);
	m_vpLabelImageContent.push_back(&m_labelGuiContent13);
	m_vpLabelImageContent.push_back(&m_labelGuiContent14);
	m_vpLabelImageContent.push_back(&m_labelGuiContent15);
	for (int j = 0; j < 16; j++)
	{
		m_vpLabelImageContent[j]->SetTextColor(RGB(0,0,0));
		m_vpLabelImageContent[j]->SetBkColor(RGB(128,255,128));
		m_vpLabelImageContent[j]->SetBorder(TRUE);
		m_vpLabelImageContent[j]->ShowWindow(SW_SHOW);
	}

	// 初始化GuiDisplay内部的文本，主要显示工具类型，如“区域定位工具”
	m_vpGuiText.resize(16);
	for (int j = 0; j < 16; j++)
	{
		scGuiStaticContainer* pCContainer = m_vpGuiDisplay.at(j)->GetConstContainer();
		m_vpGuiText.at(j) = new scGuiText;
		sc2Vector markTextPos(2, 2);	//在中心点的右上方
		m_vpGuiText.at(j) ->SetXYRotationText(markTextPos, scRadian(0), _T("----------"));
		m_vpGuiText.at(j) ->SetVisible(TRUE);
		
		//m_vpGuiText.at(j)->SetLineColor(RGB(50,230,230));
		//m_vpGuiText.at(j)->SetLineColor(RGB(255,9,9));
		m_vpGuiText.at(j)->SetLineColor(RGB(17,232,44));
		m_vpGuiText.at(j)->SetLineWidth(100);
		
		CString strKey;
		strKey.Format(_T("GuiText%d"),j);
		pCContainer->AddItem(m_vpGuiText.at(j), strKey);
		m_vpGuiDisplay.at(j)->SetConstContainer(pCContainer);
	}
	

	m_btnOK.SetThemeHelper(&m_themeHelper);
	m_btnCancel.SetThemeHelper(&m_themeHelper);
	m_btnNextPage.SetThemeHelper(&m_themeHelper);
	m_btnPreviouPage.SetThemeHelper(&m_themeHelper);
	
	// 设置图片显示通道状态
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM.at(m_nCurPlatformIndex);
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	CPlatformOptionInfo optionInfo;
	pBaseVisionAlign->GetPlatformOptionInfo(optionInfo);
	if(eTargetOnline  != platformInfo.m_eAlignerTargetType)
	{
		m_bUsingTarget = FALSE;
	}
	if(platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		
	}
	else
	{
		m_bUsingObject = FALSE;
	}
	if(platformInfo.m_eAlignerObjectType != eObjectVirtual)
	{
		m_bUsingVirtual = FALSE;
	}
	//if(FALSE == optionInfo.m_bInspectEnable)
	//{
	//	m_bUsingInspect = FALSE;
	//}
	if(platformInfo.m_bTargetObjectCamSeparate == FALSE)
	{
		m_bUsingCalibTarget = FALSE;
	}
	

	// 显示所有平台选项
	int nPlatformNum = m_vpVisionASM.size();
	for (int i = 0;i<nPlatformNum;i++)
	{
		GetDlgItem(IDC_CHECK_SHOW_PLATFORM1+i)->ShowWindow(SW_SHOW);
		SysPlatformInfo platformInfo;
		m_vpVisionASM[i]->GetSysPlatformInfo(platformInfo);
		if(platformInfo.m_nPlatformIndex == m_nCurPlatformIndex)
		((CButton*)GetDlgItem(IDC_CHECK_SHOW_PLATFORM1+i))->SetCheck(true);
	}

	// 初始化大小产品下拉框为隐藏
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_BIGPRODUCT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->EnableWindow(FALSE);
	// 刷新工位下拉框内容
	UpdateComboLoadPlatform();
	UpdateData(FALSE);
}
//清除界面所有工具（暂时没用，无任何函数调用）
bool CDlgSearchToolModelLibrary::ClearModelLibraryVector()
{
	//初始化图像
	m_vcpImage.clear();
	m_vnPos.clear();
	m_vnCandidatePos.clear();
	m_vnProductIndex.clear();
	m_vNSearchMode.clear();
	m_vToolType.clear();//存放工具类型
	m_vnPlatformIndex.clear();
	return true;
}
// 刷新界面信息（模板图像、模板对应的文本信息等）
bool CDlgSearchToolModelLibrary::UpdateDlgData()
{
	//显示图像前16个图片与文本
	//int nImageNum = m_vcpImage.size();
	// 获取预览图像数量
	int nImageNum = m_vnPreviewImageIndex.size();
	//更新总页数
	m_nMaxPageNum =  nImageNum/16+ (nImageNum%16>0?1:0);
	CString strImageContent;
	bool bRet = false;
	int nImageIndex;
	//如果当前图像数小于当前页码应该有的图像最多总数：如当前图像数为15。如果当前页码为2，则图像最多是为32幅。如果当前页码为3，则图像最多是为48幅
	//需要自动减少当前页码数
	while(m_vnPreviewImageIndex.size()<=(m_nCurPageIndex-1)*16)
	{
		// 如果当前页码已经小于0了
		if(m_nCurPageIndex<=0)
		{
			m_nCurPageIndex = 1;//则默认页码为第一页
			break;
		}
		//当页码为1时，说明没有一张图片
		if(m_nCurPageIndex==1)
		{
			m_bStatusBarFlag = TRUE;
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				m_strStatusBarInfo1 =  _T("Update interface data failed");;
				m_strStatusBarInfo2 = _T("Get Tools Number is 0");
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				m_strStatusBarInfo1 =  _T("更新界面数据失败");;
				m_strStatusBarInfo2 = _T("获取工具数为0");
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				m_strStatusBarInfo1 =  _T("更新界面数据失败");;
				m_strStatusBarInfo2 = _T("获取工具数为0");
				break;
			}
			
			for(int i= 0; i<16;i++)
			{
				m_vpGuiDisplay[i]->RemoveImage();//清屏
				m_vpLabelImageContent[i]->SetText(_T(""));//设置初始化空文本
				m_vpLabelImageContent[i]->SetBkColor(colorNormal);//设置初始化背景色
			}
			UpdateStatusBar();
			return false;
		}
		m_nCurPageIndex--;// 减少当前页码
	}
	int i;
	//显示图片
	for(i= 0; i<16;i++)
	{
		nImageIndex = (m_nCurPageIndex-1)*16+i;//第m_nCurPageIndex页，第i幅图像对应的预览模板图像编号
		m_vpGuiDisplay[i]->RemoveImage();//首先清屏
		if(nImageIndex>=nImageNum)//当图片序号大于等于预览图片总数，则不显示后面的图片
			continue;
		if(i>=m_vcpImage.size())//如果i大于等于预览图片数，则不显示后面的图片
			continue;
		m_vpGuiDisplay[i]->SetImage(m_vcpImage[i]);// 设置图像，m_vcpImage每次翻页实时刷新，并保存当前页需要预览的16幅图像
	}
	//显示图片文本
	for(i= 0; i<16;i++)
	{
		nImageIndex = (m_nCurPageIndex-1)*16+i;//第m_nCurPageIndex页，第i幅图像对应的预览模板图像编号
		m_vpGuiText[i]->SetText(_T(""));
		m_vpLabelImageContent[i]->SetText(_T(""));//首先设置初始化空文本
		m_vpLabelImageContent[i]->SetBkColor(colorNormal);//首先设置初始化背景色
		if(nImageIndex>=nImageNum)//当图片序号大于等于预览图片总数，则不显示后面的文本
			continue;
		if(nImageIndex == m_nCurSelectImageIndex)//如果当前编号等于选择编号，文本背景设置为“选择颜色”
			m_vpLabelImageContent[i]->SetBkColor(colorSelect);
		if(i>=m_vcpImage.size()) //如果i大于等于预览图片数，则不显示后面的文本
			continue;
		bRet = GetImageContent(m_vnPreviewImageIndex[nImageIndex],strImageContent);//根据当前预览图像编号nImageIndex对应的图像编号m_vnPreviewImageIndex[nImageIndex]，获取模板文本
		if(bRet)
			m_vpLabelImageContent[i]->SetText(strImageContent);//设置文本

		m_vpGuiText[i]->SetText(GetSearchToolType(m_vToolType[m_vnPreviewImageIndex[nImageIndex]]));//根据当前预览图像编号nImageIndex对应的图像编号m_vnPreviewImageIndex[nImageIndex]，获取模板类型
	}
	for (int i= 0;i<16 ;i++)
	{
		m_vpGuiDisplay[i]->ShowImage();//刷新界面
	}
	return true;
}

void CDlgSearchToolModelLibrary::UpdateStatusBar()

{
	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}
// 加载所有工位，所有产品定位工具训练图像
bool CDlgSearchToolModelLibrary::LoadAllToolTrainImage()
{
	if(m_vpVisionASM.size() <=0)
	{
		return false;
	}
	int nPlatformNum = m_vpVisionASM.size();
	// 循环加载所有平台工具
	for (int k= 0;k<nPlatformNum;k++)
	{
		vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM[k];
	
		if(m_bIsLoadPlatformTool[k])//如果当前平台已经加载过了，则无需再加载平台工具
			continue;
		//加载所有模板图片
		if(NULL != pBaseVisionAlign)
		{
			CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();	//所有产品信息	
			SysPlatformInfo platformInfo;
			pBaseVisionAlign->GetSysPlatformInfo(platformInfo);								// 获取平台信息
			int nProductNum =  pTotalProduct->GetCount();									// 获取产品总数
			// 如果时大小产品
			if(platformInfo.m_bProductFaChd)
			{
				int nProductCountFa = 0;	//父产品数量
				int nProductCountChd = 0;	//子产品数量
				int nProductIndex = 0;		//产品序号
				nProductCountFa = pTotalProduct->GetProductCountFa();//获取父产品数量
				for (int i = 0; i<nProductCountFa; i++)
				{
					nProductCountChd = pTotalProduct->GetProductCountChd(i);//获取父产品i对应的子产品数量
					for(int j = 0 ;j<nProductCountChd; j++)
					{
						nProductIndex = pTotalProduct->GetProductIndex(i,j);//获取父产品i，子产品j对应的产品序号
						LoadSearchToolTrainImageInfo(platformInfo.m_nPlatformIndex,nProductIndex);//开始加载对应产品序号的模板信息（不加载模板图像）
					}
				}
			}
			else
			{
				//当前产品数量
				int nProductNum = pTotalProduct->GetProductCountFa();
				//挨个加载产品模板信息
				for (int i=0 ;i<nProductNum;i++)
				{
					int nProductIndex = pTotalProduct->GetProductIndex(i, 0);
					LoadSearchToolTrainImageInfo(platformInfo.m_nPlatformIndex,nProductIndex);//开始加载对应产品序号的模板信息（不加载模板图像）
				}
			}
			m_bIsLoadPlatformTool[k] = true;//设置当前平台信息加载完成
		}
	}
	// 所有模板加载完成以后，获取当前软件保存的模板总数
	m_nImageNum = m_vnProductIndex.size();
	// 刷新工位设置下拉框
	UpdateComboLoadPlatform();
	// 每次完成工具的加载，都重新分配预览图片的序号m_vnPreviewImageIndex
	UpdatePreviewImageIndex();
	// 开始加载当前页面对应的模板图像
	LoadSearchToolTrainImage(m_nCurPageIndex);
	return true;
}
// 根据平台序号、产品序号加载所有模板信息
bool CDlgSearchToolModelLibrary::LoadSearchToolTrainImageInfo(int nPlatformIndex,int nProductIndex)
{
	// 从配置文件获取定位工具类型，并加载模板
	// 加载产品信息
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM.at(nPlatformIndex);
	CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();
	CTotalCalibData *pTotalCalib = pBaseVisionAlign->GetTotalCalibData();
	int nCurrentProductIndex = pTotalProduct->GetCurProductIndex(); // 当前平台序号
	bool bIsLoadCacheModel = true;//是否加载缓存模板

	//获取平台信息
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	CPlatformOptionInfo optionInfo;
	pBaseVisionAlign->GetPlatformOptionInfo(optionInfo);
	CProductData product = pTotalProduct->GetItem(nProductIndex);// 产品信息
	CCalibData caliba = pTotalCalib->GetItem(nProductIndex);	// 标定信息
	CString strProductName,strCalibName;
	CString strMarkPatternsDir;
	CString strPatternDir; 
	// 加载模板信息存在两种形式：1.在根目录下加载模板信息，2.在软件已经缓存的m_vvpAlnObjectSearchToolRepo中加载模板信息
	// 根据大小产品、加载单个产品、加载单个大小产品信息判断，当前模板信息是否已经缓存，如果缓存则采用方式2加载模板信息，否则采用方式1在根目录下加载模板信息
	// 同样需要判断产品序号nProductIndex是否为当前产品nCurrentProductIndex，如果时则采用方式2加载模板信息，这也是为了避免用户在当前产品操作时，删除模板（未保存），在模板库依然存在模板（根目录下加载模板）信息的问题
	// 开始判定是加载缓存模板，还是根目录模板
	// 如果时大小产品
	if(platformInfo.m_bProductFaChd)
	{
		// 如果是加载单个产品
		if(platformInfo.m_bLoadSingleProductSearchToolEnable)
		{
			// 加载单个小模板时
			// 如果当前需要加载的产品序号为平台的当前产品序号
			if(nCurrentProductIndex == nProductIndex)
			{
				// 加载缓存模板,也就是m_vvpAlnObjectSearchToolRepo中的模板信息
				bIsLoadCacheModel = true;
			}
			else
			{
				// 加载根目录模板
				bIsLoadCacheModel = false;
			}
		}
		else
		{
			// 如过不是加载单个产品，说明所有产品都已经缓存了
			// 如果是加载单个大产品
			if(platformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
			{
				// 需要判断当前大产品是否包含当前加载产品序号
				int nCurProductIndexFa;
				int nCurProductIndexCh;
				pTotalProduct->GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
				int nProductChNum = pTotalProduct->GetProductCountChd(nCurProductIndexFa);//当前大产品的小产品数量
				bool tmpbIsLoadCacheModel = false;
				for (int i =0;i<nProductChNum;i++)
				{
					int Index = pTotalProduct->GetProductIndex(nCurProductIndexFa,i);
					if(Index == nProductIndex)
					{
						// 当当前大产品包含当前产品序号，则加载缓存模板
						tmpbIsLoadCacheModel = true;
						break;
					}
				}
				bIsLoadCacheModel = tmpbIsLoadCacheModel;
			}
			else
			{
				// 不是加载单个大产品时
				// 判断当前产品序号是否为nProductIndex
				if(nCurrentProductIndex == nProductIndex)
				{
					// 加载缓存模板
					bIsLoadCacheModel = true;
				}
				else
				{
					// 加载根目录模板
					bIsLoadCacheModel = false;
				}
			}
		}
	}
	else
	{
		// 如果不是大小产品
		// 如果是加载单个产品
		if(platformInfo.m_bLoadSingleProductSearchToolEnable)
		{
			// 加载单个小模板时
			// 如过当前需要加载的产品序号为平台的当前产品序号
			if(nCurrentProductIndex == nProductIndex)
			{
				// 加载缓存模板,也就是m_vvpAlnObjectSearchToolRepo中的模板信息
				bIsLoadCacheModel = true;
			}
			else
			{
				// 加载根目录模板
				bIsLoadCacheModel = false;
			}
		}
		else
		{
			// 如过不是加载单个产品，说明所有产品都已经缓存了
			// 所以仅需要加载缓存模板
			bIsLoadCacheModel = true;
		}
	}
	
	//获取工具地址路径
	strProductName = pTotalProduct->GetItemName(nProductIndex);
	strProductName.Format(_T("%03d"),nProductIndex+1);
	strCalibName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\JobCalibs\\") + strProductName ;
	strProductName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\Jobs\\") + strProductName ;
	
	//目标模板图像加载
	{
		//if(m_bUsingTarget)//是否显示目标模板
		{
			if(eTargetOnline  == platformInfo.m_eAlignerTargetType)
			{
				// 模板路径
				strMarkPatternsDir =strProductName + _T("\\TargetModels");
				//加载目标
				int Modeli = product.m_pAlignerParam->m_vTargetPosPatInfo.size();
				int Modelj = product.m_pAlignerParam->m_vTargetPosPatInfo[0].m_nPatNum;
				
				//线程
				HANDLE LoadImageThread[MAXPOSNUM];
				//线程传递参数
				CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
				// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
				for(int i =0; i<Modeli; i++)
				{
					cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
						,i,Modelj,product.m_vvpAlnTargetSearchToolRepo,bIsLoadCacheModel);//线程传递参数赋值
					LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
				}
				int nReturn = WaitForMultipleObjects(Modeli,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
				for (int j=0; j<Modeli; j++)
				{
					CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
				}
				if(nReturn==WAIT_TIMEOUT)
				{
					//保存超时
				}
				else
				{
					//开始判断哪些模板没有保存成功
					for (int i=0; i<Modeli; i++)
					{
						// 加载除模板图像外的所有信息
						for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
						{
							//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
							m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
							m_vnPos.push_back(i);//存放模板对应的位置
							m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放模板对应的候补位置
							m_vNSearchMode.push_back(eTargetSearch);////存放模板对应的模板类型--目标工具
							m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);//存放模板对应的工具类型 -- 区域定位工具
							m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);// 存放模板对应的平台编号
						}
					}
				}
			}
		}
	}
	
	//对象模板图像加载
	{
	//	if(m_bUsingObject)//是否显示对象模板
		{
			if(platformInfo.m_eAlignerObjectType != eObjectBench)
			{
				//对象路径
				strMarkPatternsDir =strProductName + _T("\\ObjectModels");
				//加载目标
				int Modeli = product.m_pAlignerParam->m_vObjectPosPatInfo.size();
				int Modelj = product.m_pAlignerParam->m_vObjectPosPatInfo[0].m_nPatNum;
				//线程
				HANDLE LoadImageThread[MAXPOSNUM];
				//线程传递参数
				CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
				// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
				for(int i =0; i<Modeli; i++)
				{
					cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
						,i,Modelj,product.m_vvpAlnObjectSearchToolRepo,bIsLoadCacheModel);//线程传递参数赋值
					LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
				}
				int nReturn = WaitForMultipleObjects(Modeli,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
				for (int j=0; j<Modeli; j++)
				{
					CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
				}
				if(nReturn==WAIT_TIMEOUT)
				{
					//保存超时
				}
				else
				{
					//开始判断哪些模板没有保存成功
					for (int i=0; i<Modeli; i++)
					{
						// 加载除模板图像外的所有信息
						for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
						{
							//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
							m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
							m_vnPos.push_back(i);//存放模板对应的位置
							m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放模板对应的候补位置
							m_vNSearchMode.push_back(eObjectSearch);////存放模板对应的模板类型--目标工具
							m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);//存放模板对应的工具类型 -- 区域定位工具
							m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);// 存放模板对应的平台编号
						}
					}
				}
			}
		}
	}

	//虚拟模板图像加载
	{
	//	if(m_bUsingVirtual)//是否显示虚拟模板
		{
			//路径
			strMarkPatternsDir =strProductName + _T("\\VirtualObjectModels");
			if(platformInfo.m_eAlignerObjectType == eObjectVirtual)
			{
				int nPatternNum = platformInfo.m_nPositionNum;
				// 加载虚拟模板的方式与其他方式不同，往往虚拟模板个数不只8个
				// 虚拟模板个数不只8个，所以采用分批加载模板的方式，每批利用线程加载最多加载8个定位工具
				int nTimes = 1;//总共需要加载的次数
				int nCurTimes = 0;//当前加载次数
				int nMaxLoadModelOneTimes = 0;//每次加载模板的最大循环次数
				if(product.m_vvpAlnVirtualObjectSearchToolRepo.size()>nPatternNum)
				{
					nTimes = product.m_vvpAlnVirtualObjectSearchToolRepo.size()/nPatternNum+
						(product.m_vvpAlnVirtualObjectSearchToolRepo.size()%nPatternNum>0?1:0);
				}
				while(nCurTimes !=nTimes)
				{
					nCurTimes++;
					if(nCurTimes==nTimes)
					{
						nMaxLoadModelOneTimes =product.m_vvpAlnVirtualObjectSearchToolRepo.size()-nPatternNum*(nCurTimes-1);
					}
					else
					{
						nMaxLoadModelOneTimes = nPatternNum;
					}
				
					//加载目标
					int Modeli = product.m_pAlignerParam->GetVirtualObjectMarkPatternNum();
					int Modelj = VIRTUALOBJECTCANDIDATENUM;
					//线程
					HANDLE LoadImageThread[MAXPOSNUM];
					//线程传递参数
					CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
					// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
					for(int i =0; i<nMaxLoadModelOneTimes; i++)
					{
						cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
							,i+nPatternNum*(nCurTimes-1),Modelj,product.m_vvpAlnVirtualObjectSearchToolRepo,bIsLoadCacheModel);
						LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
					}
					int nReturn = WaitForMultipleObjects(nMaxLoadModelOneTimes,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
					for (int j=0; j<nMaxLoadModelOneTimes; j++)
					{
						CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
					}
					if(nReturn==WAIT_TIMEOUT)
					{
						//保存超时
					}
					else
					{
						//开始判断哪些模板没有保存成功
						for (int i=0; i<nMaxLoadModelOneTimes; i++)
						{
							// 加载除模板图像外的所有信息
							for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
							{
								//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
								m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
								m_vnPos.push_back(i);//存放模板对应的位置
								m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放模板对应的候补位置
								m_vNSearchMode.push_back(eVirtualSearch);////存放模板对应的模板类型--目标工具
								m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);//存放模板对应的工具类型 -- 区域定位工具
								m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);//存放模板对应的平台编号
							}
						}
					}
				}
			
			}
		}
	}

	//关联模板图像加载
	{
	//	if(m_bUsingCalibTarget)//是否显示关联模板
		{
			if(platformInfo.m_bTargetObjectCamSeparate == TRUE)
			{
				//路径
				strMarkPatternsDir =strProductName + _T("\\CalibTargetModels");
				//加载目标
				int Modeli = product.m_pAlignerParam->GetCalibTargetMarkPatternNum();
				int Modelj = ALNCALIBTARGETCANDIDATENUM;
				//线程
				HANDLE LoadImageThread[MAXPOSNUM];
				//线程传递参数
				CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
				// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
				for(int i =0; i<Modeli; i++)
				{
					cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
						,i,Modelj,product.m_vvpAlnCalibTargetSearchToolRepo,bIsLoadCacheModel);
					LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
				}
				int nReturn = WaitForMultipleObjects(Modeli,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
				for (int j=0; j<Modeli; j++)
				{
					CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
				}
				if(nReturn==WAIT_TIMEOUT)
				{
					//保存超时
				}
				else
				{
					//开始判断哪些模板没有保存成功
					for (int i=0; i<Modeli; i++)
					{
						for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
						{
							//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
							m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
							m_vnPos.push_back(i);//存放模板对应的位置
							m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放模板对应的候补位置
							m_vNSearchMode.push_back(eCalibTargetSearch);////存放模板对应的工具类型--目标工具
							m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);//存放模板对应的工具类型 -- 区域定位工具
							m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);//存放模板对应的平台编号
						}
					}
				}
			}
		}
	}

	//检测模板图像加载
	{
	//	if(m_bUsingInspect)//是否显示检测模板
		{
			//if(TRUE == optionInfo.m_bInspectEnable)
			{
				//对象路径
				strMarkPatternsDir =strProductName + _T("\\InspectModels");
				//加载目标
				int Modeli = product.m_pAlignerParam->GetInspectMarkPatternNum();
				int Modelj = INSPECTCANDIDATENUM;
				//线程
				HANDLE LoadImageThread[MAXPOSNUM];
				//线程传递参数
				CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
				// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
				for(int i =0; i<Modeli; i++)
				{
					cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
						,i,Modelj,product.m_vvpAlnInspectSearchToolRepo,bIsLoadCacheModel);
					LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
				}
				int nReturn = WaitForMultipleObjects(Modeli,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
				for (int j=0; j<Modeli; j++)
				{
					CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
				}
				if(nReturn==WAIT_TIMEOUT)
				{
					//保存超时
				}
				else
				{
					//开始判断哪些模板没有保存成功
					for (int i=0; i<Modeli; i++)
					{
						for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
						{
							//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
							m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
							m_vnPos.push_back(i);//存放模板对应的位置
							m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放Moab你对应的候补位置
							m_vNSearchMode.push_back(eInspectSearch);////存放模板对应的工具类型--目标工具
							m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);//存放模板对应的工具类型 -- 区域定位工具
							m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);//存放模板对应的平台编号
						}
					}
				}
				
			}
		}
	}

	//标定模板图像加载
	{
	//	if(m_bUsingCalib)//是否显示标定模板
		{
			//对象路径
			strMarkPatternsDir =strCalibName + _T("\\CalibModels");
			//加载目标
			int Modeli = platformInfo.m_nPositionNum;
			int Modelj = 1;
			//线程
			HANDLE LoadImageThread[MAXPOSNUM];
			//线程传递参数
			CLoadSearchToolTrainImageInfoThreadParam cThreadParam[MAXPOSNUM];
			// 以位置Modeli为基础，同时创建8个线程，每个线程加载当前位置上所有候补模板
			for(int i =0; i<Modeli; i++)
			{
				cThreadParam[i] = CLoadSearchToolTrainImageInfoThreadParam(pBaseVisionAlign,strMarkPatternsDir
					,i,Modelj,caliba.m_vvpCalibObjectSearchToolRepo,bIsLoadCacheModel);
				LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageInfoThread,&cThreadParam[i],0,NULL);//创建线程
			}
			int nReturn = WaitForMultipleObjects(Modeli,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
			for (int j=0; j<Modeli; j++)
			{
				CloseHandle(LoadImageThread[j]);    // 关闭所有线程句柄并释放内存
			}
			if(nReturn==WAIT_TIMEOUT)
			{
				//保存超时
			}
			else
			{
				//开始判断哪些模板没有保存成功
				for (int i=0; i<Modeli; i++)
				{
					for (int j= 0; j<cThreadParam[i].m_vToolType.size();j++)
					{
						//m_vcpImage.push_back(cThreadParam[i].m_vcpImage[j]);//存放图片
						m_vnProductIndex.push_back(nProductIndex);//存放模板对应的产品号
						m_vnPos.push_back(i);//存放模板对应的位置
						m_vnCandidatePos.push_back(cThreadParam[i].m_vnCandidatePos[j]);//存放模板对应的候补位置
						m_vNSearchMode.push_back(eCalibSearch);////存放模板对应的工具类型--目标工具
						m_vToolType.push_back(cThreadParam[i].m_vToolType[j]);
						m_vnPlatformIndex.push_back(platformInfo.m_nPlatformIndex);
					}
				}
			}
		
		}

	}
	return true;
}

DWORD WINAPI CDlgSearchToolModelLibrary::LoadSearchToolTrainImageInfoThread(LPVOID lpParam)
{
	CLoadSearchToolTrainImageInfoThreadParam* pThreadParam = (CLoadSearchToolTrainImageInfoThreadParam*)lpParam;
	CString strPatternDir;					//模板根目录路径
	CBaseSearchTool* pBaseSearchTool;		//声明工具

	for (int j=0;j<pThreadParam->m_nModelCandidateNum;j++)
	{
		strPatternDir.Format(_T("\\Pos%d_M%d"),pThreadParam->m_nModelPosIndex,j);
		strPatternDir = pThreadParam->m_strPatternsDir + strPatternDir;
		// 如果是加载缓存模板，则从缓存中直接读取
		if(pThreadParam->m_bIsLoadCacheModel)
		{
			// 只有缓存模板m_vvpSearchToolRepo数组的长度大于模板位置m_nModelPosIndex，且m_vvpSearchToolRepo[m_nModelPosIndex]长度大于候补位置j
			// 且位置m_nModelPosIndex，j对应的模板位置空，则获取模板信息
			if(pThreadParam->m_vvpSearchToolRepo.size()>pThreadParam->m_nModelPosIndex &&pThreadParam->m_vvpSearchToolRepo[pThreadParam->m_nModelPosIndex].size()>j&&
				pThreadParam->m_vvpSearchToolRepo[pThreadParam->m_nModelPosIndex][j] !=NULL)
			{
				//pThreadParam->m_vvpSearchToolRepo[pThreadParam->m_nModelPosIndex][j]->GetPatternImage(tmpImage);
				pThreadParam->m_vnCandidatePos.push_back(j);//存放模板对应的候补位置
				pThreadParam->m_vToolType.push_back(pThreadParam->m_vvpSearchToolRepo[pThreadParam->m_nModelPosIndex][j]->GetSearchToolType());//存放工具类型
				continue;
			}
		}
		else//否则从根目录下读取
		{
			// 从XML文件中获取定位工具类型
			CSearchToolType cSearchToolType = CBaseSearchTool::GetSearchToolTypeFromFile(strPatternDir);
			// 根据定位工具类型创建定位工具
			pBaseSearchTool = NULL;
			pThreadParam->m_pBaseVisionAlign->CreateSearchTool(cSearchToolType , &pBaseSearchTool);
			// 如果工具存在且，对应的模板图像存在，就保存候补位置信息与工具类型信息
			//if ((NULL == pBaseSearchTool) || (FALSE == pBaseSearchTool->LoadModelTrainPatternImage(strPatternDir,tmpImage)))
			if ((NULL == pBaseSearchTool) || (FALSE == pThreadParam->IsImageExist(strPatternDir)))
			{
				continue;
			}
			else
			{
				//pThreadParam->m_vcpImage.push_back(tmpImage);//存放图片
				pThreadParam->m_vnCandidatePos.push_back(j);//存放图片对应的候补位置
				pThreadParam->m_vToolType.push_back(cSearchToolType);//存放工具类型
			}
			// 释放工具
			if (pBaseSearchTool!= NULL)
			{
				delete pBaseSearchTool;
				pBaseSearchTool = NULL;
			}
		}
		
	}
	pThreadParam->SetIsInitTimeOut(false);//当前线程运行完成，未超时
	return 0;
}

//根据当前页面编号加载指定图像（16幅图像）
bool CDlgSearchToolModelLibrary::LoadSearchToolTrainImage(int nCurPageIndex)
{
	// 清空当前图像
	m_vcpImage.clear();
	if(nCurPageIndex<-1||nCurPageIndex>m_nMaxPageNum)
	{
		return false;
	}

	vcBaseVisionAlign* pBaseVisionAlign;
	int nProductIndex;
	SysPlatformInfo platformInfo;
	CTotalProductData *pTotalProduct;
	CProductData* product;
	CCalibData *calibe;
	CString strProductName;
	CString strMarkPatternsDir;
	
	int Modeli,Modelj;
	int nImageIndex;
	// 因为只需要加载16幅图像，所以需要设置好16幅图像需要加载参数即可
	vector<vector<CString>> vvstrPatternDir;//模板路径，4X4数组
	vvstrPatternDir.resize(4);
	vector<vector<bool>> vvbLoadTrainImage;//标识是否需要在根目录下加载图像，如果当前缓存文件中已经拥有模板工具了则标识为false，否则标识为ture
	vvbLoadTrainImage.resize(4);
	int nLoadtime = 4;//加载16幅图像，创建的线程数
	std::vector<VpSearchTool> vvpSearchTool;//如果时缓存模板，则将缓存模板信息，存入其中，4X4数组
	vvpSearchTool.resize(4);
	std::vector<vector<bool>> vvbIsLoadCacheModel;//是否加载当前缓存模板，4x4数组
	vvbIsLoadCacheModel.resize(4);
	for (int i =0 ;i<4;i++)
	{
		vvstrPatternDir[i].resize(4);
		vvpSearchTool[i].resize(4);
		vvbLoadTrainImage[i].resize(4);
		vvbIsLoadCacheModel[i].resize(4);
	}
	//线程
	HANDLE LoadImageThread[4];
	//线程传递参数
	CLoadSearchToolTrainImageThreadParam cThreadParam[4];
	//以4X4循环的方式，获取运行线程需要的参数
	for(int i =0; i<nLoadtime; i++)
	{
		for (int k = 0;k<4;k++)
		{
			int n =16*(nCurPageIndex-1)+(i)*4+k;//当前图片预览编号
			// 如果预览编号小于预览数组vector长度
			if(n<m_vnPreviewImageIndex.size())
			{
				nImageIndex =m_vnPreviewImageIndex[n];// 根据预览数组，获取图像的实际编号
			}
			else
			{
				nImageIndex = -1;//设置为无效编号
			}
			{
				nProductIndex = GetImageProduceIndex(nImageIndex);//根据图像编号获取对应产品信息
				// 产品序号小于0，则直接退出
				if(nProductIndex <0)
				{
					vvpSearchTool[i][k] = NULL;//设置当前工具信息为空
					continue;
				}
				// 从配置文件获取定位工具类型，并加载模板
				pBaseVisionAlign= m_vpVisionASM.at(GetImagePlatformIndex(nImageIndex));
				//加载产品信息
				pTotalProduct = pBaseVisionAlign->GetTotalProuductData();
				int nCurrentProductIndex = pTotalProduct->GetCurProductIndex();
				//获取平台信息
				pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
				product = pTotalProduct->GetItemPtr(nProductIndex);
				calibe = pBaseVisionAlign->GetTotalCalibData()->GetItemPtr(nProductIndex);
				//获取工具地址路径
				strProductName = pTotalProduct->GetItemName(nProductIndex);
				strProductName.Format(_T("%03d"),nProductIndex+1);
				strProductName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\Jobs\\") + strProductName ;
				//开始加载模板
				Modeli = GetImageSearchToolPosIndex(nImageIndex);
				Modelj = GetImageSearchToolCandidatePosIndex(nImageIndex);
				// 加载模板信息存在两种形式：1.在根目录下加载模板信息，2.在软件已经缓存的m_vvpAlnObjectSearchToolRepo中加载模板信息
				// 根据大小产品、加载单个产品、加载单个大小产品信息判断，当前模板信息是否已经缓存，如果缓存则采用方式2加载模板信息，否则采用方式1在根目录下加载模板信息
				// 同样需要判断产品序号nProductIndex是否为当前产品nCurrentProductIndex，如果时则采用方式2加载模板信息，这也是为了避免用户在当前产品操作时，删除模板（未保存），在模板库依然存在模板（根目录下加载模板）信息的问题
				// 开始判定是加载缓存模板，还是根目录模板
				if(platformInfo.m_bProductFaChd)
				{
					// 如果大小产品
					// 如果是加载单个产品
					if(platformInfo.m_bLoadSingleProductSearchToolEnable)
					{
						// 加载单个小模板时
						// 如过当前需要加载的产品序号为平台的当前产品序号
						if(nCurrentProductIndex == nProductIndex)
						{
							// 加载根缓存模板
							vvbIsLoadCacheModel[i][k] = true;
						}
						else
						{
							// 加载根目录模板
							vvbIsLoadCacheModel[i][k] = false;
						}
					}
					else
					{
						//如过不是加载单个产品，说明所有产品都已经缓存了
						// 如果是加载单个大产品
						if(platformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
						{
							// 需要判断当前大产品是否包含当前加载产品序号
							int nCurProductIndexFa;
							int nCurProductIndexCh;
							pTotalProduct->GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
							int nProductChNum = pTotalProduct->GetProductCountChd(nCurProductIndexFa);//当前大产品的小产品数量
							bool tmpbIsLoadCacheModel = false;
							for (int j =0;j<nProductChNum;j++)
							{
								int Index = pTotalProduct->GetProductIndex(nCurProductIndexFa,j);
								if(Index == nProductIndex)
								{
									// 加载根缓存模板,
									tmpbIsLoadCacheModel = true;
									break;
								}
							}
							vvbIsLoadCacheModel[i][k] = tmpbIsLoadCacheModel;
						}
						else
						{
							// 加载不是加载单个大产品时
							// 根据系统初始化时的加载逻辑，仅加载当前产品下的模板
							if(nCurrentProductIndex == nProductIndex)
							{
								// 加载根缓存模板
								vvbIsLoadCacheModel[i][k] = true;
							}
							else
							{
								// 加载根目录模板
								vvbIsLoadCacheModel[i][k] = false;
							}
							
						}
					}
				}
				else
				{
					// 如果不是大小产品
					// 如果是加载单个产品
					if(platformInfo.m_bLoadSingleProductSearchToolEnable)
					{
						// 加载单个小模板时
						// 如过当前需要加载的产品序号为平台的当前产品序号
						if(nCurrentProductIndex == nProductIndex)
						{
							// 加载根缓存模板
							vvbIsLoadCacheModel[i][k] = true;
						}
						else
						{
							// 加载根目录模板
							vvbIsLoadCacheModel[i][k] = false;
						}
					}
					else
					{
						//如过不是加载单个产品，说明所有产品都已经缓存了
						// 所以仅需要加载缓存模板
						vvbIsLoadCacheModel[i][k] = true;
					}
				}
				// 获取模板工具类型
				switch(GetImageSearchToolMode(nImageIndex))
				{
				case eObjectSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(product->m_vvpAlnObjectSearchToolRepo.size()>Modeli&&
						product->m_vvpAlnObjectSearchToolRepo[Modeli].size()>Modelj&&
						product->m_vvpAlnObjectSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = product->m_vvpAlnObjectSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;// 设置不用再加载训练图像了
						vvstrPatternDir[i][k] = "";//设置路径为空
						continue;
					}
					strMarkPatternsDir =strProductName + _T("\\ObjectModels");
					break;
				case eTargetSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(product->m_vvpAlnTargetSearchToolRepo.size()>Modeli&&
						product->m_vvpAlnTargetSearchToolRepo[Modeli].size()>Modelj&&
						product->m_vvpAlnTargetSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = product->m_vvpAlnTargetSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;
						vvstrPatternDir[i][k] = "";
						continue;
					}
					strMarkPatternsDir =strProductName + _T("\\TargetModels");
					break;
				case eInspectSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(product->m_vvpAlnInspectSearchToolRepo.size()>Modeli&&
						product->m_vvpAlnInspectSearchToolRepo[Modeli].size()>Modelj&&
						product->m_vvpAlnInspectSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = product->m_vvpAlnInspectSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;
						vvstrPatternDir[i][k] = "";
						continue;
					}
					strMarkPatternsDir =strProductName + _T("\\InspectModels");
					break;
				case eVirtualSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(product->m_vvpAlnVirtualObjectSearchToolRepo.size()>Modeli&&
						product->m_vvpAlnVirtualObjectSearchToolRepo[Modeli].size()>Modelj&&
						product->m_vvpAlnVirtualObjectSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = product->m_vvpAlnVirtualObjectSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;
						vvstrPatternDir[i][k] = "";
						continue;
					}
					strMarkPatternsDir =strProductName + _T("\\VirtualObjectModels");
					break;
				case eCalibTargetSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(product->m_vvpAlnCalibTargetSearchToolRepo.size()>Modeli&&
						product->m_vvpAlnCalibTargetSearchToolRepo[Modeli].size()>Modelj&&
						product->m_vvpAlnCalibTargetSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = product->m_vvpAlnCalibTargetSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;
						vvstrPatternDir[i][k] = "";
						continue;
					}
					strMarkPatternsDir =strProductName + _T("\\CalibTargetModels");
					break;
				case eCalibSearch:
					// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
					if(calibe->m_vvpCalibObjectSearchToolRepo.size()>Modeli&&
						calibe->m_vvpCalibObjectSearchToolRepo[Modeli].size()>Modelj&&
						calibe->m_vvpCalibObjectSearchToolRepo[Modeli][Modelj] != NULL)
					{
						vvpSearchTool[i][k] = calibe->m_vvpCalibObjectSearchToolRepo[Modeli][Modelj];
						vvbLoadTrainImage[i][k] = false;
						vvstrPatternDir[i][k] = "";
						continue;
					}
					strProductName = pBaseVisionAlign->GetTotalCalibData()->GetItemName(nProductIndex);
					strProductName.Format(_T("%03d"), nProductIndex+1);
					strProductName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\JobCalibs\\") + strProductName;
					strMarkPatternsDir =strProductName + _T("\\CalibModels");
					break;
				default:
					continue;
				}
				CSearchToolType cSearchToolType = GetImageSearchToolType(nImageIndex);
				vvstrPatternDir[i][k].Format(_T("\\Pos%d_M%d"), Modeli, Modelj);
				vvstrPatternDir[i][k] =strMarkPatternsDir + vvstrPatternDir[i][k];
				pBaseVisionAlign->CreateSearchTool(cSearchToolType , &vvpSearchTool[i][k]);
				vvbLoadTrainImage[i][k] = TRUE;
			}
		}
	}
	// 4个线程，循环4次加载16幅模板图像
	for(int i =0; i<nLoadtime; i++)
	{
		cThreadParam[i] = CLoadSearchToolTrainImageThreadParam(vvpSearchTool[i],vvstrPatternDir[i],vvbLoadTrainImage[i],vvbIsLoadCacheModel[i]);
		LoadImageThread[i] = CreateThread(NULL,0,LoadSearchToolTrainImageThread,&cThreadParam[i],0,NULL);//创建线程
	}
	m_time.GetTimeMilli(FALSE);
	int nReturn = WaitForMultipleObjects(nLoadtime,LoadImageThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
	for (int i=0; i<nLoadtime; i++)
	{
		CloseHandle(LoadImageThread[i]);    // 关闭所有线程句柄并释放内存
	}
	m_strtime.Format("Load:%.1fms",m_time.GetTimeMilli(TRUE));
	if(nReturn==WAIT_TIMEOUT)
	{
		//保存超时
	}
	else
	{
		//开始判断哪些模板没有保存成功
		for (int i=0; i<nLoadtime; i++)
		{
			for (int k=0; k<cThreadParam[i].m_vcpImage.size(); k++)
			{
				m_vcpImage.push_back(cThreadParam[i].m_vcpImage[k]);//存放图片
			}
		}
	}
	return TRUE;
}

DWORD WINAPI CDlgSearchToolModelLibrary::LoadSearchToolTrainImageThread(LPVOID lpParam)
{
	CLoadSearchToolTrainImageThreadParam* pThreadParam = (CLoadSearchToolTrainImageThreadParam*)lpParam;
	// 循环加载4幅模板图像
	for (int i =0;i < 4; i++)
	{
		cpImage patternImage;
		bool bRet =false;
		// 初始定位工具m_vpSearchTool不能为空
		if(pThreadParam->m_vpSearchTool[i]!=NULL)
		{
			// 已经标识不需要在根目录下加载训练图像
			if(!pThreadParam->m_vbLoadTrainImage[i])
			{
				// 必须是加载缓存模板，才能获取训练图像
				if(pThreadParam->m_vbLoadCacheModel[i])
				{
					pThreadParam->m_vpSearchTool[i]->GetPatternImage(patternImage);//直接从缓存工具中获取训练图像
					pThreadParam->m_vcpImage.push_back(patternImage);
				}
			}
			else
			{
				// 必须不是加载缓存模板，才能加载根目录训练图像
				if(!pThreadParam->m_vbLoadCacheModel[i])
				{
					// 在根目录下加载寻来你图像
					if((pThreadParam->m_vpSearchTool[i]!=NULL)&&(TRUE == pThreadParam->m_vpSearchTool[i]->LoadModelTrainPatternImage(pThreadParam->m_vstrPatternDir[i],patternImage)))
					{
						pThreadParam->m_vcpImage.push_back(patternImage);
					}
				}
			}
		}
	}
	pThreadParam->SetIsInitTimeOut(false);//当前线程运行完成，未超时
	return 0;
}

// 双击Display控件响应
LRESULT CDlgSearchToolModelLibrary::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	CBaseSearchTool *pSearchTool;//临时工具
	pSearchTool = GetSelectedSearchTool();//获取当前选择预览图像对应的定位工具
	if(pSearchTool != NULL)
	{
		CDlgSearch dlgSearch(this,pSearchTool);
		dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
		dlgSearch.EnableSysKeyboard(false);
		dlgSearch.SetVisionASMRef(m_vpVisionASM.at(m_vnPlatformIndex[GetSelectImageIndex()]),0);
		dlgSearch.SetIsPreviewTool(true);//标识dlgSearch界面仅提供预览，不支持保存修改功能
		dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
		if (dlgSearch.DoModal() == IDOK)
		{

		}
	}
	// 释放m_pBaseSearchTool，避免内存泄漏
	// 不释放pSearchTool，而是释放m_pBaseSearchTool，避免释放pSearchTool后，m_pBaseSearchTool不为空，但是内存释放了，导致当前析构时再次释放m_pBaseSearchTool出错
	if (m_pBaseSearchTool != NULL)
	{
		delete m_pBaseSearchTool;
		m_pBaseSearchTool = NULL;
	}
	return S_OK ;
}

// 点击Display控件响应
LRESULT CDlgSearchToolModelLibrary::OnGuiDisplayClk(WPARAM wParam, LPARAM lParam)
{
	for (int j=0; j < 16; j++)
	{
		if (wParam == m_vpGuiDisplay[j]->GetID())
		{
			int nChooseIndex = j+(m_nCurPageIndex-1)*16;//获取选择图片的预览编号
			CString text = "";
			if(nChooseIndex >= /*m_vcpImage.size()*/m_vnPreviewImageIndex.size())//如果预览编号大于等于预览数组m_vnPreviewImageIndex的长度
			{
				m_bStatusBarFlag = FALSE;
				m_strStatusBarInfo1 = "";
				m_strStatusBarInfo2 =_T("当前选择:----");
				switch (m_nlgidLanguage)
				{
				case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
					m_strStatusBarInfo2 =_T("The currently selected:----");
					break;
				case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
					m_strStatusBarInfo2 =_T("当前选择:----");
					break;
				case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
				default:
					m_strStatusBarInfo2 =_T("当前选择:----");
					break;
				}
				UpdateStatusBar();
				m_nCurSelectImageIndex = -1;
				UpdateToolPreviewInfo(GetSelectImageIndex());
			}
			else
			{
				m_nCurSelectImageIndex = nChooseIndex;//设置当前预览图片选择序号
				int nImageIndex = m_vnPreviewImageIndex[nChooseIndex];// 通过预览序号获取当前模板在模板库的序号
				GetImageContent(nImageIndex,text,true);// 根据在模板库的序号获取模板对应的文本
				m_bStatusBarFlag = TRUE;
				m_strStatusBarInfo1 = "";
				m_strStatusBarInfo2 =text;
				UpdateStatusBar();
				m_vpLabelImageContent[j]->SetBkColor(colorSelect);//设置选择图片标题颜色
				UpdateToolPreviewInfo(GetSelectImageIndex());		// 刷新选择图片预览信息
			}
		}
		else
		{
			m_vpLabelImageContent[j]->SetBkColor(colorNormal);
		}
	}
	return S_OK ;
}
// 上一页
void CDlgSearchToolModelLibrary::OnBnClickedBtnPreviousPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurPageIndex<=1)
	{
		m_bStatusBarFlag = FALSE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_strStatusBarInfo1 =  _T("Flip Failure");;
			m_strStatusBarInfo2 = _T("This is the first page!");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_strStatusBarInfo1 =  _T("翻页失败");;
			m_strStatusBarInfo2 = _T("当前已是第一页!");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_strStatusBarInfo1 =  _T("翻页失败");;
			m_strStatusBarInfo2 = _T("当前已是第一页!");
			break;
		}
		UpdateStatusBar();
		return;
	}
	CString strtime;
	scTimer t;
	m_nCurPageIndex--;//当前页码减少一页
	LoadSearchToolTrainImage(m_nCurPageIndex);// 根据页码加载16幅模板图像
	
	CString strImageContent;
	bool bRet = false;
	// 清空界面信息
	for (int i= 0;i<16 ;i++)
	{
		m_vpGuiDisplay[i]->RemoveImage();
		m_vpLabelImageContent[i]->SetText(_T(""));
		m_vpGuiText[i]->SetText(_T(""));
	}
	
	for (int i= 0;i<16 ;i++)
	{
		int nImageIndex =(m_nCurPageIndex-1)*16+i;//当前模板预览编号
		m_vpLabelImageContent[i]->SetBkColor(colorNormal);
	
		if(nImageIndex>m_vnPreviewImageIndex.size())// 预览编号大于预览数组
		{
			continue;
		}
		if(nImageIndex == m_nCurSelectImageIndex)//如果当前预览图片编号等于选中编号，则设置选中颜色
		{	
			m_vpLabelImageContent[i]->SetBkColor(colorSelect);
		}
		if(i>=m_vcpImage.size())
			continue;;
		m_vpGuiDisplay[i]->SetImage(m_vcpImage[i]);//设置图片
		bRet = GetImageContent(m_vnPreviewImageIndex[nImageIndex],strImageContent);//根据图像预览编号获取对应在图像库中的编号，根据图像库编号获取图片对应的文本
		if(bRet)
			m_vpLabelImageContent[i]->SetText(strImageContent);

		m_vpGuiText[i]->SetText(GetSearchToolType(m_vToolType[m_vnPreviewImageIndex[nImageIndex]]));//根据图像预览编号获取对应在图像库中的编号，根据图像库编号获取工具类型信息
	}

	for (int i= 0;i<16 ;i++)
	{
		m_vpGuiDisplay[i]->ShowImage();//显示界面图像
	}
	CString strText;
	
	
	switch (m_nlgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strText.Format(_T("Current Page %d，A Total Of %d Pages"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("Flip Success，Loading Tool Time:%.1fms"),t.GetTimeMilli(TRUE));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strText.Format(_T("当前第 %d 页，总共 %d 页"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("翻页成功，加载工具用时:%.1fms"),t.GetTimeMilli(TRUE));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strText.Format(_T("当前第 %d 页，总共 %d 页"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("翻页成功，加载工具用时:%.1fms"),t.GetTimeMilli(TRUE));
		break;
		break;
	}
	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = strtime;
	m_strStatusBarInfo2 =strText;
	Sleep(50);
	UpdateStatusBar();
}
// 下一页
void CDlgSearchToolModelLibrary::OnBnClickedBtnNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurPageIndex>=m_nMaxPageNum)
	{
		m_bStatusBarFlag = FALSE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_strStatusBarInfo1 =  _T("Flip Failure");;
			m_strStatusBarInfo2 = _T("This is the last page!");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_strStatusBarInfo1 =  _T("翻页失败");;
			m_strStatusBarInfo2 = _T("当前已是最后一页!");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_strStatusBarInfo1 =  _T("翻页失败");;
			m_strStatusBarInfo2 = _T("当前已是最后一页!");
			break;
		}
		UpdateStatusBar();
		return;
	}
	CString strtime;
	scTimer t;
	m_nCurPageIndex++;//当前页码增加一页
	LoadSearchToolTrainImage(m_nCurPageIndex); // 根据页码加载16幅模板图像
	CString strImageContent;
	bool bRet = false;
	// 清空界面显示
	for (int i= 0;i<16 ;i++)
	{
		m_vpGuiDisplay[i]->RemoveImage();
		m_vpLabelImageContent[i]->SetText(_T(""));
		m_vpGuiText[i]->SetText(_T(""));
	}
	
	for (int i= 0;i<16 ;i++)
	{
		int nImageIndex =(m_nCurPageIndex-1)*16+i;//当前模板预览编号
		m_vpLabelImageContent[i]->SetBkColor(colorNormal);
		
		if(nImageIndex>=m_vnPreviewImageIndex.size()) //预览编号大于预览数组
		{
			continue;
		}
		if(nImageIndex == m_nCurSelectImageIndex)//如果当前预览图片编号等于选中编号，则设置选中颜色
		{	
			m_vpLabelImageContent[i]->SetBkColor(colorSelect);
		}
		if(i>=m_vcpImage.size())
			continue;;
		m_vpGuiDisplay[i]->SetImage(m_vcpImage[i]);//设置图片
		bRet = GetImageContent(m_vnPreviewImageIndex[nImageIndex],strImageContent);////根据图像预览编号获取对应在图像库中的编号，根据图像库编号获取图片对应的文本
		if(bRet)
			m_vpLabelImageContent[i]->SetText(strImageContent);

		m_vpGuiText[i]->SetText(GetSearchToolType(m_vToolType[m_vnPreviewImageIndex[nImageIndex]]));//根据图像预览编号获取对应在图像库中的编号，根据图像库编号获取工具类型信息
	}
	for (int i= 0;i<16 ;i++)
	{
		m_vpGuiDisplay[i]->ShowImage();//刷新界面
	}
	CString strText;

	switch (m_nlgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strText.Format(_T("Current Page %d，A Total Of %d Pages"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("Flip Success，Loading Tool Time:%.1fms"),t.GetTimeMilli(TRUE));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strText.Format(_T("当前第 %d 页，总共 %d 页"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("翻页成功，加载工具用时:%.1fms"),t.GetTimeMilli(TRUE));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strText.Format(_T("当前第 %d 页，总共 %d 页"),m_nCurPageIndex,m_nMaxPageNum);
		strtime.Format(_T("翻页成功，加载工具用时:%.1fms"),t.GetTimeMilli(TRUE));
		break;
		break;
	}

	m_bStatusBarFlag = TRUE;
	m_strStatusBarInfo1 = strtime;
	m_strStatusBarInfo2 =strText;
	UpdateStatusBar();
}
// 获取选择的模板在模板库中的序号（并非预览序号）
int	CDlgSearchToolModelLibrary::GetSelectImageIndex()
{
	if(m_vnPreviewImageIndex.size()<m_nCurSelectImageIndex)
	{
		return -1;
	}
	return m_vnPreviewImageIndex[m_nCurSelectImageIndex];
}
// 获取选择的模板的预览序号
int CDlgSearchToolModelLibrary::GetSelectPreviewImageIndex()
{
	return m_nCurSelectImageIndex;
}
// 获取选中的图像
bool CDlgSearchToolModelLibrary::GetSelectedImage(cpImage &ImagePattern)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	//ImagePattern = m_vcpImage[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	ImagePattern = m_vcpImage[m_nCurSelectImageIndex-(m_nCurPageIndex-1)*16];
	return true;
}
// 获取选中模板对应的平台序号
bool CDlgSearchToolModelLibrary::GetSelectedPlatformIndex(int &nPlatformIndex)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	nPlatformIndex = m_vnPlatformIndex[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中模板对应的产品序号
bool CDlgSearchToolModelLibrary::GetSelectedProduceIndex(int &nProductIndex)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	nProductIndex = m_vnProductIndex[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中模板对应的工具类型
bool CDlgSearchToolModelLibrary::GetSelectedSearchToolMode(SearchMode &sSelectMode)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	sSelectMode = m_vNSearchMode[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中模板的位置编号
bool CDlgSearchToolModelLibrary::GetSelectedSearchToolPosIndex(int &nPosIndex)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	nPosIndex = m_vnPos[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中模板对应的候补位置
bool CDlgSearchToolModelLibrary::GetSelectedSearchToolCandidatePosIndex(int &nCandidatePosIndex)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	nCandidatePosIndex = m_vnCandidatePos[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中图像对应的模板类型
bool CDlgSearchToolModelLibrary::GetSelectedSearchToolType(CSearchToolType &SearchToolType)
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vToolType.size())
	{
		return false;
	}
	SearchToolType = m_vToolType[m_vnPreviewImageIndex[m_nCurSelectImageIndex]];
	return true;
}
// 获取选中模板的模板信息m_pBaseSearchTool
CBaseSearchTool* CDlgSearchToolModelLibrary::GetSelectedSearchTool()
{
	if(m_nCurSelectImageIndex<-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		return false;
	}
	// 获取当前图片在图片库的序号
	int nImageIndex = m_vnPreviewImageIndex[m_nCurSelectImageIndex];
	// 从配置文件获取定位工具类型，并加载模板
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM.at(m_vnPlatformIndex[nImageIndex]);
	//加载产品信息
	CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();
	
	//获取平台信息
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	CProductData product = pTotalProduct->GetItem(m_vnProductIndex[nImageIndex]);
	CCalibData calibe = pBaseVisionAlign->GetTotalCalibData()->GetItem(m_vnProductIndex[nImageIndex]);
	CString strProductName;
	CString strMarkPatternsDir;
	CString strPatternDir; 
	//获取工具地址路径
	strProductName = pTotalProduct->GetItemName(m_vnProductIndex[nImageIndex]);
	strProductName.Format(_T("%03d"),m_vnProductIndex[nImageIndex]+1);
	strProductName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\Jobs\\") + strProductName ;
	//开始加载模板
	int i = m_vnPos[nImageIndex];
	int j = m_vnCandidatePos[nImageIndex];
	
	// 避免自身拷贝到自身崩溃
	int nTmpCurProductIndex = 0;
	if (NULL != m_vpVisionASM.at(m_nCurPlatformIndex))
	{
		CTotalProductData *pTotalProduct2 = m_vpVisionASM.at(m_nCurPlatformIndex)->GetTotalProuductData();
		if (NULL !=pTotalProduct2)
		{
			nTmpCurProductIndex = pTotalProduct2->GetCurProductIndex();
		}
	}
	if (i== m_nPosIndexFrom && j== m_nCandidatePosFrom && m_vNSearchMode[nImageIndex]== m_nSearchModeFrom  
		&& m_nCurPlatformIndex ==m_vnPlatformIndex[nImageIndex] &&  m_vnProductIndex[nImageIndex]==nTmpCurProductIndex)
	{
		return NULL;
	}

	
	


	//根据类型获取路径
	switch(m_vNSearchMode[nImageIndex])
	{
	case eObjectSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(product.m_vvpAlnObjectSearchToolRepo.size()>i&&
			product.m_vvpAlnObjectSearchToolRepo[i].size()>j&&
			product.m_vvpAlnObjectSearchToolRepo[i][j] != NULL)
		{
			if (CheckSearchToolIsValid(product.m_vvpAlnObjectSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return product.m_vvpAlnObjectSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(product.m_vvpAlnObjectSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
		}
		strMarkPatternsDir =strProductName + _T("\\ObjectModels");
		break;
	case eTargetSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(product.m_vvpAlnTargetSearchToolRepo.size()>i&&
			product.m_vvpAlnTargetSearchToolRepo[i].size()>j&&
			product.m_vvpAlnTargetSearchToolRepo[i][j] != NULL)
		{
			//return product.m_vvpAlnTargetSearchToolRepo[i][j];
			if (CheckSearchToolIsValid(product.m_vvpAlnTargetSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return product.m_vvpAlnTargetSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(product.m_vvpAlnTargetSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
			
		}
		strMarkPatternsDir =strProductName + _T("\\TargetModels");
		break;
	case eInspectSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(product.m_vvpAlnInspectSearchToolRepo.size()>i&&
			product.m_vvpAlnInspectSearchToolRepo[i].size()>j&&
			product.m_vvpAlnInspectSearchToolRepo[i][j] != NULL)
		{
			//return product.m_vvpAlnInspectSearchToolRepo[i][j];
			if (CheckSearchToolIsValid(product.m_vvpAlnInspectSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return product.m_vvpAlnInspectSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(product.m_vvpAlnInspectSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
		}
		strMarkPatternsDir =strProductName + _T("\\InspectModels");
		break;
	case eVirtualSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(product.m_vvpAlnVirtualObjectSearchToolRepo.size()>i&&
			product.m_vvpAlnVirtualObjectSearchToolRepo[i].size()>j&&
			product.m_vvpAlnVirtualObjectSearchToolRepo[i][j] != NULL)
		{
			//return product.m_vvpAlnVirtualObjectSearchToolRepo[i][j];
			if (CheckSearchToolIsValid(product.m_vvpAlnVirtualObjectSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return product.m_vvpAlnVirtualObjectSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(product.m_vvpAlnVirtualObjectSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
		}
		strMarkPatternsDir =strProductName + _T("\\VirtualObjectModels");
		break;
	case eCalibTargetSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(product.m_vvpAlnCalibTargetSearchToolRepo.size()>i&&
			product.m_vvpAlnCalibTargetSearchToolRepo[i].size()>j&&
			product.m_vvpAlnCalibTargetSearchToolRepo[i][j] != NULL)
		{
			//return product.m_vvpAlnCalibTargetSearchToolRepo[i][j];
			if (CheckSearchToolIsValid(product.m_vvpAlnCalibTargetSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return product.m_vvpAlnCalibTargetSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(product.m_vvpAlnCalibTargetSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
		}
		strMarkPatternsDir =strProductName + _T("\\CalibTargetModels");
		break;
	case eCalibSearch:
		// 首先从产品中加载模板，如果没有，则LoadModelFromFile模板
		if(calibe.m_vvpCalibObjectSearchToolRepo.size()>i&&
			calibe.m_vvpCalibObjectSearchToolRepo[i].size()>j&&
			calibe.m_vvpCalibObjectSearchToolRepo[i][j] != NULL)
		{
			//return calibe.m_vvpCalibObjectSearchToolRepo[i][j];
			if (CheckSearchToolIsValid(calibe.m_vvpCalibObjectSearchToolRepo[i][j]->GetSearchToolType()))
			{
				return calibe.m_vvpCalibObjectSearchToolRepo[i][j];
			}
			else
			{
				CString strTmp = _T("");
				strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(calibe.m_vvpCalibObjectSearchToolRepo[i][j]->GetSearchToolType()),m_nCurPlatformIndex+1);
				AfxMessageBox(strTmp);
				return NULL;
			}
		}
		strProductName = pBaseVisionAlign->GetTotalCalibData()->GetItemName(m_vnProductIndex[m_nCurSelectImageIndex]);
		strProductName.Format(_T("%03d"), m_vnProductIndex[m_nCurSelectImageIndex]+1);
		strProductName = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath+ _T("\\JobCalibs\\") + strProductName;
		strMarkPatternsDir =strProductName + _T("\\CalibModels");
		break;
	default:
		return NULL;
	}
	//CBaseSearchTool* pBaseSearchTool = NULL;		//声明工具
	strPatternDir.Format(_T("\\Pos%d_M%d"), m_vnPos[nImageIndex], m_vnCandidatePos[nImageIndex]);
	strPatternDir =strMarkPatternsDir + strPatternDir;
	// 从XML文件中获取定位工具类型
	CSearchToolType cSearchToolType = CBaseSearchTool::GetSearchToolTypeFromFile(strPatternDir);

	// 对当前工位的平台选项-定位工具可用的类型判断 如果拷贝源定位工具当前工位平台选项-定位工具不可用时 不执行拷贝
	{
		if (!CheckSearchToolIsValid(cSearchToolType))
		{
			CString strTmp = _T("");
			strTmp.Format(_T("%s在工位%d平台选项-定位工具设置中没有勾选，不执行拷贝"),GetSearchToolType(cSearchToolType),m_nCurPlatformIndex+1);
			AfxMessageBox(strTmp);
			return NULL;
		}

	}


	// 根据定位工具类型创建定位工具
	pBaseVisionAlign->CreateSearchTool(cSearchToolType , &m_pBaseSearchTool);
	if((NULL == m_pBaseSearchTool) ||(FALSE == m_pBaseSearchTool->LoadModelFromFile(strPatternDir)))
	{
		if(m_pBaseSearchTool != NULL)
		{
			delete m_pBaseSearchTool;
			m_pBaseSearchTool = NULL;
		}
		return false;
	}
	else
	{
		return m_pBaseSearchTool;
	}
}

// 根据模板序号(模板库序号，并非预览序号)返回图片信息
// bIsCompleteInfo = true返回完整信息用于状态栏
// bIsCompleteInfo = false否则返回不完整信息用于工具说明Label
bool CDlgSearchToolModelLibrary::GetImageContent(int nImageIndex,CString &strImageContent,bool bIsCompleteInfo)
{
	if(nImageIndex < 0||nImageIndex > m_nImageNum)
	{
		return false;
	}
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM.at(m_vnPlatformIndex[nImageIndex]);
	if(pBaseVisionAlign== NULL)
	{
		return false;
	}
	CString strContent;
	CString strModelType;
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();//获取完整产品信息
	//获取模板类型信息
	switch (m_nlgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		switch(m_vNSearchMode[nImageIndex])
		{
		case eObjectSearch:
			strModelType = _T("Object");
			break;
		case eTargetSearch:
			strModelType = _T("Target");
			break;
		case eInspectSearch:
			strModelType = _T("Inspect");
			break;
		case eVirtualSearch:
			strModelType = _T("virtual");
			break;
		case eCalibTargetSearch:
			strModelType = _T("CalibTarget");
			break;
		case eCalibSearch:
			strModelType = _T("Calib");
			break;
		default:
			strModelType = _T("Object");
			break;
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		switch(m_vNSearchMode[nImageIndex])
		{
		case eObjectSearch:
			strModelType = _T("对象");
			break;
		case eTargetSearch:
			strModelType = _T("目标");
			break;
		case eInspectSearch:
			strModelType = _T("检测");
			break;
		case eVirtualSearch:
			strModelType = _T("虚拟");
			break;
		case eCalibTargetSearch:
			strModelType = _T("关联");
			break;
		case eCalibSearch:
			strModelType = _T("标定");
			break;
		default:
			strModelType = _T("目标");
			break;
		}
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		switch(m_vNSearchMode[nImageIndex])
		{
		case eObjectSearch:
			strModelType = _T("对象");
			break;
		case eTargetSearch:
			strModelType = _T("目标");
			break;
		case eInspectSearch:
			strModelType = _T("检测");
			break;
		case eVirtualSearch:
			strModelType = _T("虚拟");
			break;
		case eCalibTargetSearch:
			strModelType = _T("关联");
			break;
		case eCalibSearch:
			strModelType = _T("标定");
			break;
		default:
			strModelType = _T("目标");
			break;
		}
		break;
	}
	
	// 获取平台序号+产品序号以及产品名
	if(bIsCompleteInfo)
	{
		CString strPreProductName;
		if(platformInfo.m_bProductFaChd)
		{
			int nProductFa;
			int nProductChd;
			CString strProductFaName;
			CString strProductChName;
			pTotalProduct->GetDlgProductIndex(m_vnProductIndex[nImageIndex],nProductFa,nProductChd);
			strProductFaName =  pTotalProduct->GetProductNameFa(nProductFa);	// 获取平台父名称
			strProductChName =  pTotalProduct->GetItemName(m_vnProductIndex[nImageIndex]);		// 获取平台子名称
			strPreProductName.Format(_T("%s-%s"),strProductFaName,strProductChName);
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.Format(_T("Platform%d ProducIndext:%d-%d Name:%s "),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1,strPreProductName);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.Format(_T("工位%d 产品序号:%d-%d 产品名称:%s "),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1,strPreProductName);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.Format(_T("工位%d 产品序号:%d-%d 产品名称:%s "),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1,strPreProductName);
				break;
			}
		}
		else
		{
			strPreProductName =  pTotalProduct->GetProductNameFa(m_vnProductIndex[nImageIndex]);	// 获取平台父名称
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.Format(_T("Platform%d ProducIndext%d Name%s "),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1,strPreProductName);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.Format(_T("工位%d 产品序号:%d 产品名称:%s "),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1,strPreProductName);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.Format(_T("工位%d 产品序号:%d 产品名称:%s "),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1,strPreProductName);
				break;
			}
		}
	}
	else
	{
		// 不完整工具信息
		// 平台+产品序号
		if(platformInfo.m_bProductFaChd)
		{
			int nProductFa;
			int nProductChd;
			pTotalProduct->GetDlgProductIndex(m_vnProductIndex[nImageIndex],nProductFa,nProductChd);
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.Format(_T("Platform%dProduct%d-%d"),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.Format(_T("工位%d产品%d-%d"),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.Format(_T("工位%d产品%d-%d"),m_vnPlatformIndex[nImageIndex]+1,nProductFa+1,nProductChd+1);
				break;
			}
		}
		else
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.Format(_T("Platform%dProduct%d"),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.Format(_T("工位%d产品%d"),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.Format(_T("工位%d产品%d"),m_vnPlatformIndex[nImageIndex]+1,m_vnProductIndex[nImageIndex]+1);
				break;
			}
		}
	}
	
	if(bIsCompleteInfo)
	{
		if(m_vnCandidatePos[nImageIndex]==0)
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.AppendFormat(_T("%sModel Pos%dStandard"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.AppendFormat(_T("%s模板 Pos%d基准"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.AppendFormat(_T("%s模板 Pos%d基准"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			}
		}
		else
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.AppendFormat(_T("%sModel Pos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.AppendFormat(_T("%s模板 Pos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.AppendFormat(_T("%s模板 Pos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			}
		}
	}
	else
	{
		if(m_vnCandidatePos[nImageIndex]==0)
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.AppendFormat(_T("%sPos%dStandard"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.AppendFormat(_T("%sPos%d基准"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.AppendFormat(_T("%sPos%d基准"),strModelType,m_vnPos[nImageIndex]+1);
				break;
			}
		}
		else
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strContent.AppendFormat(_T("%sPos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strContent.AppendFormat(_T("%sPos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strContent.AppendFormat(_T("%sPos%d_M%d"),strModelType,m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			}
		}
	}

	// 如果时完整类型，则添加工具类型
	if(bIsCompleteInfo)
	{
		strImageContent = GetSearchToolType(m_vToolType[nImageIndex]);
		strImageContent += _T(" ");
		strImageContent += strContent;
	}
	else
	{
		strImageContent = strContent;
	}
	return true;
}
// 根据工具类型获取工具类新对用的名称信息，如：“区域定位工具”名称
CString CDlgSearchToolModelLibrary::GetSearchToolType(CSearchToolType csType)
{
	CString strSelText;
	switch(csType)
	{
	case eSearchToolUnknown:
		strSelText =_T("未知类型");
		break;
	case eQuickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_TOOL);
		break;
	case ePatternSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_TOOL);
		break;
	case eLineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_LINE_SEARCH_TOOL);
		break;								  		
	case eCornerSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_TOOL);
		break;
	case eCircleSearchTool:					
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLE_SEARCH_TOOL);
		break;								
	case eFixedPosSearchTool:	
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_FIXEDPOS_SEARCH_TOOL);
		break;								  		
	case eLineSearch2Tool:			
		strSelText =_T("找线工具2");		
		break;						
	case eCrossSearchTool:				  		
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CROSS_SEARCH_TOOL);
		break;								  
	case ePatCornerSearchTool:					
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATCORNER_SEARCH_TOOL);
		break;								  
	case eProCircleSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PROCIRCLE_SEARCH_TOOL);
		break;								  
	case ePatternCornerSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERNCORNER_SEARCH_TOOL);
		break;								  
	case eCombinedQuickSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_QUICK_SEARCH_TOOL);
		break;								  
	case eCombinedPatternSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_PATTERN_SEARCH_TOOL);
		break;								  
	case eBaseCornerSearchTool:						
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_BASE_CORNER_SEARCH_TOOL);
		break;								  
	case ePatternSearchCheckTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_TOOL);
		break;								  
	case eQuickSearchCheckTool:				 
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_TOOL);
		break;								  
	case eQuickSearchObroundTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_OBROUND_TOOL);
		break;								  
	case ePatternSearchObroundTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_OBROUND_TOOL);
		break;								  
	case eCircleSearchTool2:				  
		strSelText =_T("圆定位工具2");		  
		break;								  
	case eGCCPatternCornerSearchTool:			
		strSelText =_T("GCC专用角定位工具");
		break;
	case eMPatternSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_SEARCH_TOOL);
		break;
	case eMQuickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_SEARCH_TOOL);
		break;
	case eMQuickCornerSearchTool:	
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_CORNER_SEARCH_TOOL);
		break;		
	case eMQuickPickCornerSearchTool:	
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_CORNER_SEARCH_TOOL);
		break;
	case eQuickSearchGenRectTool:					
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_GENRECT_TOOL);
		break;									
	case ePatternSearchGenRectTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_GENRECT_TOOL);
		break;									 
	case ePatternLineSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_SEARCH_TOOL);
		break;									 
	case e2PatternSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_2PATTERN_SEARCH_TOOL);
		break;									 	
	case eQuickLineCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_LINE_CIRCLE_SEARCH_TOOL);
		break;
	case ePatternLineCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_LINE_CIRCLE_SEARCH_TOOL);
		break;
	case ePatternCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_SEARCH_TOOL);
		break;
	case eQuickSearchCheckLineTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_SEARCH_CHECK_LINE_TOOL);
		break;
	case ePatternSearchCheckLineTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_SEARCH_CHECK_LINE_TOOL);
		break;
	case ePatternBlobSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_BLOB_SEARCH_TOOL);
		break;
	case eCornerSearchCheckTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNER_SEARCH_CHECK_TOOL);
		break;
	case eRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_RECT_SEARCH_TOOL);
		break;
	case ePatternRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_RECT_SEARCH_TOOL);
		break;
	case eAreaInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECT_TOOL);
		break;
	case eAreaInspectToolEx:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AREA_INSPECTEX_TOOL);
		break;
	case eChordSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CHORD_SEARCH_TOOL);
		break;								  	
	case eTwoPatModelSearchTool:				  	
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_PAT_MODEL_SEARCH_TOOL);
		break;								
	case eMPatternCornerSearchTool:				 	
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNCORNER_SEARCH_TOOL);
		break;
	case eMPatternPickCornerSearchTool:				 	
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_MPATTERNPICKCORNER_SEARCH_TOOL);
		break;
	case ePatternCornerCircleSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings, IDS_CMB_PATTERN_CORNER_CIRCLE_SEARCH_TOOL);
		break;								      
	case eQuickCornerCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_CIRCLE_SEARCH_TOOL);
		break;
	case ePatternPosLineDistanceTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_POSLINEDISTANCE_TOOL);
		break;
	case eCornerSearchCheckInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CORNERSEARCH_CHECKINSPECT_TOOL);
		break;
	case ePatternDouleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_DOUBLE_SEARCH_TOOL);
		break;
	case ePatternChordSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CHORD_SEARCH_TOOL);
		break;
	case eQuickMulCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_MULCIRCLE_SEARCH_TOOL);
		break;
	case eDmCodeCalibrateTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_DMCOLDE_CALIBRATE_TOOL);
		break;
	case ePatternCircle4LineSearchTool:			
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CIRCLE_4LINESEARCH_TOOL);
		break;									
	case eQuickCircle4LineSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CIRCLE_4LINESEARCH_TOOL);
		break;									
	case eSpecialPickSearchTool:				
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_SPECIAL_PICK_SEARCH_TOOL);
		break;
	case ePatternCornerLineSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_LINE_SEARCH_TOOL);
		break;
	case ePatternMulCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_MULCIRCLE_SEARCH_TOOL);
		break;
	case ePatternCornerMulCircleSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNERMULCIRCLE_SEARCH_TOOL);
		break;
	case ePatternCornerMulObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_CORNER_MULOBROUND_TOOL);
		break;
	case eQuickCornerMulObroundTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_CORNER_MULOBROUND_TOOL);
		break;
	case eCircleCornerSearchCheckInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_CIRCLECORNERSEARCH_CHECKINSPECT_TOOL);break;
	case eACFInspectTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_ACFINSPECT_TOOL);break;
	case ePatternVirtualRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_PATTERN_VIRTUAL_RECT_SEARCH_TOOL);break;
	case eVirtualRectSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_VIRTUAL_RECT_SEARCH_TOOL);break;
	case eTwoQuickModelSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_TWO_QUICK_MODEL_SEARCH_TOOL);
		break;
	case eIdReaderDmCodeTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_DMCODE_TOOL);
		break;
	case eIdReaderBarCodeTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_IDREADER_BARCODE_TOOL);
		break;
	case eQuickPosLineDistanceTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_QUICK_POSLINEDISTANCE_TOOL);
		break;
	case eCombinationCornerSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_COMBINED_CORNER_TOOL);
		break;
	case eMQuickPickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MQUICK_PICK_SEARCH_TOOL);
		break;
	case eMPatternPickSearchTool:
		strSelText = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_MPATTERN_PICK_SEARCH_TOOL);
		break;
	default:
		strSelText = _T("未知类型");
		break;
	}
	return strSelText;		
}
// 确定按钮
void CDlgSearchToolModelLibrary::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	CString strTitle;
	// 如果未选中模板，获取选择非模板区域
	if(m_nCurSelectImageIndex<=-1||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			strMsg.Format(_T("No Model are currently selected，Are you still sure?"));
			strTitle.Format(_T("Prompt"));
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			strMsg.Format(_T("当前未选择任何模板，是否依然确定？"));
			strTitle.Format(_T("提示"));
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			strMsg.Format(_T("当前未选择任何模板，是否依然确定？"));
			strTitle.Format(_T("提示"));
			break;
		}
		int nRet = MessageBox(strMsg, strTitle ,MB_YESNO);
		if(nRet == IDYES)
		{
			CDialog::OnOK();
		}
		else if(nRet == IDNO)
		{

		}
		return ;
	}
	CString strContent;
	int nImageIndex;
	if(m_nCurSelectImageIndex<0||m_nCurSelectImageIndex>m_vnPreviewImageIndex.size())
	{
		nImageIndex = -1;
	}
	else
	{
		nImageIndex = m_vnPreviewImageIndex[m_nCurSelectImageIndex];// 获取当前预览图片在模板库的显示序号
	}
	 
	GetImageContent(nImageIndex,strContent,true);//根据模板库的显示序号，获取工具的描述文本
	switch (m_nlgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strMsg.Format(_T("Whether to copy the current template[%s]？"),strContent);
		strTitle.Format(_T("Prompt"));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strMsg.Format(_T("是否拷贝当前模板[%s]？"),strContent);
		strTitle.Format(_T("提示"));
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strMsg.Format(_T("是否拷贝当前模板[%s]？"),strContent);
		strTitle.Format(_T("提示"));
		break;
	}
	int nRet = MessageBox(strMsg, strTitle,MB_YESNO);
	if(nRet == IDYES)
	{
		CDialog::OnOK();
	}
	else if(nRet == IDNO)
	{
		
	}
	
	return;
}

// 选择工具类型通道
void CDlgSearchToolModelLibrary::OnBnClickedCheckUpdateDlgData()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_nCurSelectImageIndex = -1;//刷新以后之前选择图片序号重置
	scTimer time;
	CString strtime;
	LoadAllToolTrainImage();//加载所有模板图像
	UpdateComboLoadPlatform();//刷新工位下拉列表（包括大小产品下拉列表）

	bool bRet = UpdateDlgData();//刷新界面
	if(bRet)
	{
		m_bStatusBarFlag = TRUE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			strtime.Format("LoadImageTimer:%.1fms;",time.GetTimeMilli(TRUE));
			m_strStatusBarInfo1 = strtime;
			m_strStatusBarInfo2 =_T("Model Loading Completed");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			strtime.Format("加载工具用时:%.1fms;",time.GetTimeMilli(TRUE));
			m_strStatusBarInfo1 = strtime;
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			strtime.Format("加载工具用时:%.1fms;",time.GetTimeMilli(TRUE));
			m_strStatusBarInfo1 = strtime;
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		}
		UpdateStatusBar();
	}
	UpdateToolPreviewInfo(-1);//清空选择的预览信息
	
	Sleep(50);
}

//刷新“模板信息预览”信息根据图片的图像库的序号
void CDlgSearchToolModelLibrary::UpdateToolPreviewInfo(int nImageIndex)
{
	int nSize = m_vcpImage.size();
	if(nImageIndex<0 || nSize<1 || (m_nCurSelectImageIndex-(m_nCurPageIndex-1)*16<0 || m_nCurSelectImageIndex-(m_nCurPageIndex-1)*16>nSize))	
	{
		// 如果没由选择，则初始化预览
		m_pGuiDisplayPreview->RemoveImage();
		m_strPrePlatformIndex = _T("----------");
		m_strPreProductName = _T("----------");
		m_strPreProductIndex = _T("----------");
		m_strPreToolType = _T("----------");
		m_strPreModelType = _T("----------");
		m_strPreModelPosition = _T("----------");
		UpdateData(FALSE);
	}
	else
	{
		vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM.at(m_vnPlatformIndex[nImageIndex]);
		if(pBaseVisionAlign== NULL)
		{
			return ;
		}
		CString strText;
		CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();
		//获取平台信息
		SysPlatformInfo platformInfo;
		pBaseVisionAlign->GetSysPlatformInfo(platformInfo);

		m_pGuiDisplayPreview->RemoveImage();
		m_pGuiDisplayPreview->SetImage(m_vcpImage[m_nCurSelectImageIndex-(m_nCurPageIndex-1)*16]);					// 设置图片
		m_strPreToolType = GetSearchToolType(m_vToolType[nImageIndex]);			// 设置工具名称
		strText.Format("%d",m_vnPlatformIndex[nImageIndex]+1);
		m_strPrePlatformIndex = strText;												// 设置平台序号
		
		
		if(platformInfo.m_bProductFaChd)
		{
		
			int nProductFa;
			int nProductCh;
			pTotalProduct->GetDlgProductIndex(m_vnProductIndex[nImageIndex],nProductFa,nProductCh);
			CString strProductFaName;
			CString strProductChName;
			strProductFaName = pTotalProduct->GetProductNameFa(nProductFa);	// 获取平台父产品名称
			strProductChName	=  pTotalProduct->GetItemName(m_vnProductIndex[nImageIndex]);	// 获取平台子品名称
			m_strPreProductName.Format(_T("%s-%s"),strProductFaName,strProductChName);	// 设置平台名称

			strText.Format("%0.3d-%0.3d",nProductFa+1,nProductCh+1);
			m_strPreProductIndex = strText;													// 设置产品序号
		}
		else
		{
			m_strPreProductName =  pTotalProduct->GetProductNameFa(m_vnProductIndex[nImageIndex]);	// 设置平台名称
			strText.Format("%0.3d",m_vnProductIndex[nImageIndex]+1);
			m_strPreProductIndex = strText;													// 设置产品序号
		}
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			switch(m_vNSearchMode[nImageIndex])
			{
			case eObjectSearch:
				strText = _T("ObjectModel");	 
				break;
			case eTargetSearch:
				strText = _T("TargetModel");	 
					break;
			case eInspectSearch:
				strText = _T("InspectModel");	 
					break;
			case eVirtualSearch:
				strText = _T("virtualModel");	 
					break;
			case eCalibTargetSearch:		   
				strText = _T("CalibTargetModel");	 
					break;
			case eCalibSearch:
				strText = _T("CalibModel");	
					break;
			default:
				strText = _T("TargetModel");	
				break;
			}
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			switch(m_vNSearchMode[nImageIndex])
			{
			case eObjectSearch:
				strText = _T("对象模板");
				break;
			case eTargetSearch:
				strText = _T("目标模板");
				break;
			case eInspectSearch:
				strText = _T("检测模板");
				break;
			case eVirtualSearch:
				strText = _T("虚拟模板");
				break;
			case eCalibTargetSearch:
				strText = _T("关联模板");
				break;
			case eCalibSearch:
				strText = _T("标定模板");
				break;
			default:
				strText = _T("目标模板");
				break;
			}
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			switch(m_vNSearchMode[nImageIndex])
			{
			case eObjectSearch:
				strText = _T("对象模板");
				break;
			case eTargetSearch:
				strText = _T("目标模板");
				break;
			case eInspectSearch:
				strText = _T("检测模板");
				break;
			case eVirtualSearch:
				strText = _T("虚拟模板");
				break;
			case eCalibTargetSearch:
				strText = _T("关联模板");
				break;
			case eCalibSearch:
				strText = _T("标定模板");
				break;
			default:
				strText = _T("目标模板");
				break;
			}
			break;
		}
		
		m_strPreModelType = strText;														// 设置模板位置
		
		if(m_vnCandidatePos[nImageIndex]==0)
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strText.Format(_T("Pos%dStandard"),m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strText.Format(_T("位置%d基准"),m_vnPos[nImageIndex]+1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strText.Format(_T("位置%d基准"),m_vnPos[nImageIndex]+1);
				break;
			}
		}
		else
		{
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strText.Format(_T("Pos%d_M%d"),m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strText.Format(_T("位置%d候补%d"),m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strText.Format(_T("位置%d候补%d"),m_vnPos[nImageIndex]+1,m_vnCandidatePos[nImageIndex]);
				break;
			}
		}
		m_strPreModelPosition = strText;													// 设置模板详细位置
		UpdateData(FALSE);
	}
}

//计算所有需要预览的图片编号，存放入m_vnPreviewImageIndex内,并重新计算最大页数，以及当前页码
bool CDlgSearchToolModelLibrary::UpdatePreviewImageIndex()
{
	if(m_nImageNum<=0)//模板小于0时
	{return false;}
	m_vnPreviewImageIndex.clear();//首先清空预览图片序号
	vcBaseVisionAlign* pBaseVisionAlign;
	SysPlatformInfo platformInfo;
	bool bIsToolDisplay;
	int nProductIndex;
	CString strComboText;
	for (int i = 0;i<m_nImageNum;i++)
	{
		//如果当前工具所在平台未加载，则不添加模板序号
		if(!m_bIsLoadPlatformTool[m_vnPlatformIndex[i]])
		{
			continue;
		}
		//如果当前工具所在平台未启用，则不添加模板序号
		if(!((CButton*)GetDlgItem(IDC_CHECK_SHOW_PLATFORM1+m_vnPlatformIndex[i]))->GetCheck())
		{
			continue;
		}
		// 如果当前单选按钮选择的时加载全部产品
		if(m_vbLoadAllProduct[m_vnPlatformIndex[i]])
		{

		}
		else
		{
			// 当仅加载当前工位下，单个产品时
			pBaseVisionAlign= m_vpVisionASM[m_vnPlatformIndex[i]];
			pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
			// 获取模板对应的产品序号
			if(platformInfo.m_bProductFaChd)//如果时大小产品
			{
				nProductIndex = pBaseVisionAlign->GetTotalProuductData()->GetProductIndex(m_vvnComboSelectProductIndex[m_vnPlatformIndex[i]][0],
					m_vvnComboSelectProductIndex[m_vnPlatformIndex[i]][1]);
			}
			else
			{
				nProductIndex = m_vvnComboSelectProductIndex[m_vnPlatformIndex[i]][0];
			}
			// 当当前模板产品序号不为产品下拉框选择的序号时，当前模板序号不添加到预览序号中，直接continue
			if(nProductIndex != m_vnProductIndex[i])
			{
				continue;
			}
		}

		// 判断当前工具图片通道是否启用
		switch(m_vNSearchMode[i])
		{
		case eObjectSearch:
			bIsToolDisplay = m_bUsingObject;
			break;
		case eTargetSearch:
			bIsToolDisplay = m_bUsingTarget;
			break;
		case eInspectSearch:
			bIsToolDisplay = m_bUsingInspect;
			break;
		case eVirtualSearch:
			bIsToolDisplay = m_bUsingVirtual;
			break;
		case eCalibTargetSearch:
			bIsToolDisplay = m_bUsingCalibTarget;
			break;
		case eCalibSearch:
			bIsToolDisplay = m_bUsingCalib;
			break;
		default:
			bIsToolDisplay = false;
			break;
		}
		// 如果当前工具图片通道启用，则添加图像工具序号
		if(bIsToolDisplay)
		{
			m_vnPreviewImageIndex.push_back(i);
		}
	}
	// 计算最大页数
	m_nMaxPageNum = (m_vnPreviewImageIndex.size()/16)+(m_vnPreviewImageIndex.size()%16>0?1:0);
	// 判断当前页码是否大于最大页码
	if(m_nCurPageIndex>m_nMaxPageNum)
		m_nCurPageIndex = m_nMaxPageNum;
	return true;
}

// 根据模板在模板库的序号获取图片的平台序号
int	CDlgSearchToolModelLibrary::GetImagePlatformIndex(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vnPlatformIndex.size())
	{
		return -1;
	}
	return m_vnPlatformIndex[nImageIndex];
}
// 根据模板在模板库的序号获取图片的产品序号
int CDlgSearchToolModelLibrary::GetImageProduceIndex(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vnProductIndex.size())
	{
		return -1;
	}
	return m_vnProductIndex[nImageIndex];
}
// 根据模板在模板库的序获取图片的工具类型
SearchMode	CDlgSearchToolModelLibrary::GetImageSearchToolMode(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vNSearchMode.size())
	{
		return eNoSearch;
	}
	return m_vNSearchMode[nImageIndex];
}
// 根据模板在模板库的序号获取图片的工具位置
int CDlgSearchToolModelLibrary::GetImageSearchToolPosIndex(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vnPos.size())
	{
		return -1;
	}
	return m_vnPos[nImageIndex];
}
// 根据模板在模板库的序号获取图片的工具候补位置
int CDlgSearchToolModelLibrary::GetImageSearchToolCandidatePosIndex(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vnCandidatePos.size())
	{
		return -1;
	}
	return m_vnCandidatePos[nImageIndex];
}
// 根据模板在模板库的序号获取图片的工具模板类新
CSearchToolType CDlgSearchToolModelLibrary::GetImageSearchToolType(int nImageIndex)
{
	//图片序号小于0或者大于图片总数
	if(nImageIndex<0||nImageIndex>=m_nImageNum||nImageIndex>m_vToolType.size())
	{
		return eSearchToolUnknown;
	}
	return m_vToolType[nImageIndex];
}
// 刷新工位下拉框信息
void CDlgSearchToolModelLibrary::UpdateComboLoadPlatform()
{
	m_ComboLoadPlatform.ResetContent();
	int nCheckNum = 0;
	CString strtmp;
	// ComboPlatform 加载下拉列表
	for (int i = 0;i<6;i++)
	{
		bool bcheck = ((CButton*)GetDlgItem(IDC_CHECK_SHOW_PLATFORM1+i))->GetCheck();
		if(bcheck)
		{
			nCheckNum++;
			switch (m_nlgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				strtmp.Format(_T("%0.3d|Platform%d"),i+1,i+1);
				m_ComboLoadPlatform.AddString(strtmp);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				strtmp.Format(_T("%0.3d|工位%d"),i+1,i+1);
				m_ComboLoadPlatform.AddString(strtmp);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				strtmp.Format(_T("%0.3d|工位%d"),i+1,i+1);
				m_ComboLoadPlatform.AddString(strtmp);
				break;
			}
		}
	}
	
	// 如果下拉列表为空
	if(nCheckNum==0)
	{
		// 清空大小产品列表
		m_ComboLoadBigProduct.ResetContent();
		m_ComboLoadSmallProduct.ResetContent();
	}
	else
	{
		// 判断当前列表是否 允许被加载
		bool bcheck = ((CButton*)GetDlgItem(IDC_CHECK_SHOW_PLATFORM1+m_nComboSelectPlatformIndex))->GetCheck();
		if(bcheck)
		{
			//  如果允许被加载 则获取平台编号 加载大小产品下拉列表
			CString strText;
			int nCount = m_ComboLoadPlatform.GetCount();
			for(int i =0;i<nCount ;i++)
			{
				m_ComboLoadPlatform.GetLBText(i,strText);
				int nIndex = strText.Find('|');
				strText = strText.Left(nIndex);
				int nPlatformIndex = _ttoi(strText)-1;
				if(nPlatformIndex ==m_nComboSelectPlatformIndex)
				{
					m_ComboLoadPlatform.SetCurSel(i);
					UpdateComboLoadProduct(m_nComboSelectPlatformIndex);//加载大小产品下拉列表
					continue;
				}
			}
		}
		else
		{
			//  如果不允许被加载 则重新判断需要加载 下拉列表的第一个平台号
			CString strText;
			m_ComboLoadPlatform.GetLBText(0,strText);
			int nIndex = strText.Find('|');
			strText = strText.Left(nIndex);
			int nPlatformIndex = _ttoi(strText)-1;
			m_ComboLoadPlatform.SetCurSel(0);
			m_nComboSelectPlatformIndex = nPlatformIndex;
			UpdateComboLoadProduct(nPlatformIndex);//加载大小产品下拉列表
			
		}
		
	}
}
// 根据当前选择的平台序号nPlatform，加载大小产品下拉列表
void CDlgSearchToolModelLibrary::UpdateComboLoadProduct(int nPlatform)
{
	// 清空大小产品列表
	if(nPlatform<0)
	{
		m_ComboLoadSmallProduct.ResetContent();
		m_ComboLoadBigProduct.ResetContent();
		return;
	}
	m_ComboLoadSmallProduct.ResetContent();
	m_ComboLoadBigProduct.ResetContent();
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM[nPlatform];
	CTotalProductData *pTotalProduct = pBaseVisionAlign->GetTotalProuductData();	
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	CString strtmp;
	if(platformInfo.m_bProductFaChd)//如果是大小产品
	{
		int nProductFaNum = pTotalProduct->GetProductCountFa();
		CString strProductFaName;
		CString strProductChName;
		//添加信息到大产品列表
		for (int i=0;i<nProductFaNum;i++)
		{
			strProductFaName = pTotalProduct->GetProductNameFa(i);	// 获取平台父产品名称
			strtmp.Format(_T("%0.3d|%s"),i+1,strProductFaName);
			m_ComboLoadBigProduct.AddString(strtmp);
		}
		// 如果当前没有选择的任何大产品
		if(m_vvnComboSelectProductIndex[nPlatform][0]==-1)
		{
			m_ComboLoadBigProduct.SetCurSel(0);//大产品选择为0
		}
		else// 如果当前选择了产品
		{
			m_ComboLoadBigProduct.SetCurSel(m_vvnComboSelectProductIndex[nPlatform][0]);//大产品下拉框获取选择的焦点
		}
		int nProductFaIndex;
		int nProductChNum;
		// 如果当前没有选择的任何产品
		if(m_vvnComboSelectProductIndex[nPlatform][0]==-1)
		{
			nProductChNum = pTotalProduct->GetProductCountChd(0);//获取大产品对应的子产品总数
			nProductFaIndex = 0;//大产品选择为0
		}
		else// 如果当前选择了产品
		{
			nProductChNum = pTotalProduct->GetProductCountChd(m_vvnComboSelectProductIndex[nPlatform][0]);//获取大产品对应的子产品总数
			nProductFaIndex = m_vvnComboSelectProductIndex[nPlatform][0];//赋值当前大产品选择序号
		}
		// 添加信息到小产品列表
		for (int i=0;i<nProductChNum;i++)
		{
			int nProductIndex =pTotalProduct->GetProductIndex(nProductFaIndex,i);//更具大产品序号nProductFaIndex与小产品序号i，获取产品序号
			strProductChName = pTotalProduct->GetItemName(nProductIndex);//获取对应产品名称
			strtmp.Format(_T("%0.3d|%s"),i+1,strProductChName);
			m_ComboLoadSmallProduct.AddString(strtmp);
		}
		// 如果当前没有选择的任何小产品
		if(m_vvnComboSelectProductIndex[nPlatform][1]==-1)
		{
			m_ComboLoadSmallProduct.SetCurSel(0);//小产品下拉列表选择0
		}
		else// 如果当前选择了小产品
		{
			m_ComboLoadSmallProduct.SetCurSel(m_vvnComboSelectProductIndex[nPlatform][1]);//设置小产品选择的序号
		}
	}
	else
	{
		m_ComboLoadSmallProduct.EnableWindow(FALSE);//如果当前平台不为大小品种，则小产品下拉列表隐藏
		int nProductFaNum = pTotalProduct->GetProductCountFa();//获取大产品数量
		CString strProductFaName;
		// 设置大产品下拉列表内容
		for (int i = 0;i<nProductFaNum;i++)
		{
			strProductFaName = pTotalProduct->GetProductNameFa(i);	// 获取平台父产品名称
			strtmp.Format(_T("%0.3d|%s"),i+1,strProductFaName);
			m_ComboLoadBigProduct.AddString(strtmp);
		}
		// 如果当前平台没有选择任何大产品
		if(m_vvnComboSelectProductIndex[nPlatform][0]==-1)
		{
			m_ComboLoadBigProduct.SetCurSel(0);// 大产品下拉列表选择0
		}
		else// 如果当前平台选择了大产品
		{
			m_ComboLoadBigProduct.SetCurSel(m_vvnComboSelectProductIndex[nPlatform][0]);//大产品下拉列表设置选择的大产品
		}
	}
	// 设置加载全部产品按钮选择状态  大小产品是否可用状态
	//((CButton*)GetDlgItem(IDC_CHECK_LOAD_ALLPRODUCT))->SetCheck(m_vbLoadAllProduct[nPlatform]);
	m_bIsLoadAllProduct = m_vbLoadAllProduct[nPlatform];//获取平台是否选择加载全部模板
	// 如果加载全部模板，以下全部隐藏，否则加载单个模板，下面全部显示
	((CButton*)GetDlgItem(IDC_STATIC_LOADPRODUCT_BIGPRODUCT))->ShowWindow(!m_vbLoadAllProduct[nPlatform]);
	((CButton*)GetDlgItem(IDC_STATIC_LOADPRODUCT_SMALLPRODUCT))->ShowWindow(!m_vbLoadAllProduct[nPlatform]);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_BIGPRODUCT))->ShowWindow(!m_vbLoadAllProduct[nPlatform]);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->ShowWindow(!m_vbLoadAllProduct[nPlatform]);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_BIGPRODUCT))->EnableWindow(!m_vbLoadAllProduct[nPlatform]);
	if(platformInfo.m_bProductFaChd)
	{
		((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->EnableWindow(!m_vbLoadAllProduct[nPlatform]);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->EnableWindow(false);//非大小品种则隐藏
	}
	UpdateData(FALSE);
}
// 工位下拉列表选择改变事件
void CDlgSearchToolModelLibrary::OnSelchangeCmbLoadProductPlatform()
{
	int nCurSelectIndex = m_ComboLoadPlatform.GetCurSel();//获取当前选项
	CString strText;
	m_ComboLoadPlatform.GetLBText(nCurSelectIndex,strText);//获取选项内容
	int nIndex = strText.Find('|');
	strText = strText.Left(nIndex);
	int nPlatformIndex = _ttoi(strText)-1;//获取内容对应的平台序号
	//如果序号与当前选择的平台序号一样则reture，否则更新大小产品下拉列表
	if(nPlatformIndex == m_nComboSelectPlatformIndex)
	{
		return;
	}
	m_nComboSelectPlatformIndex = nPlatformIndex;//设置选择的产品序号
	UpdateComboLoadProduct(nPlatformIndex);//更新大小产品下拉列表
}
// 大产品下拉列表选择改变事件
void CDlgSearchToolModelLibrary::OnSelchangeCmbLoadProductFaProduct()
{
	int nCurSelectIndex = m_ComboLoadBigProduct.GetCurSel();//获取选择序号
	CString strText;
	m_ComboLoadBigProduct.GetLBText(nCurSelectIndex,strText);//获取选择文本
	int nIndex = strText.Find('|');
	strText = strText.Left(nIndex);
	int nProductIndex = _ttoi(strText)-1;//获取文本对应产品序号
	// 如果大产品序号与当前工位选怎的大产品序号相同，则reture，否则更新界面
	if(nProductIndex == m_vvnComboSelectProductIndex[m_nComboSelectPlatformIndex][0])
	{
		return;
	}
	m_vvnComboSelectProductIndex[m_nComboSelectPlatformIndex][0] = nProductIndex;//设置当前选择的大产品序号

	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM[m_nComboSelectPlatformIndex];
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	//如果大小产品，则刷新小产品
	if(platformInfo.m_bProductFaChd)
	{
		m_vvnComboSelectProductIndex[m_nComboSelectPlatformIndex][1] = 0;//默认小产品选择为0
		UpdateComboLoadProduct(m_nComboSelectPlatformIndex);//更新大小产品下拉列表
	}
	
	//开始刷新界面工具
	m_nCurSelectImageIndex = -1;//刷新以后之前选择图片序号重置
	UpdatePreviewImageIndex(); // 刷新所有需要预览的图片编号
	/*if(m_vcpImage.size()<=0)
	{*/
		LoadSearchToolTrainImage(m_nCurPageIndex);// 刷新完预览编号以后，根据当前页码加载界面模板图像
	/*}*/
	bool bRet = UpdateDlgData();//记载完图像，开始刷新界面图像与图像文本
	if(bRet)
	{
		m_bStatusBarFlag = TRUE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("Model Loading Completed");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		}
		UpdateStatusBar();
	}
	UpdateToolPreviewInfo(-1);//重置选择预览信息
}
// 小产品下拉列表选择改变事件
void CDlgSearchToolModelLibrary::OnSelchangeCmbLoadProductChProduct()
{
	int nCurSelectIndex = m_ComboLoadSmallProduct.GetCurSel();//获取选择编号
	CString strText;
	m_ComboLoadSmallProduct.GetLBText(nCurSelectIndex,strText);//获取文本
	int nIndex = strText.Find('|');
	strText = strText.Left(nIndex);
	int nProductIndex = _ttoi(strText)-1;//根据文本获取选择的产品序号
	//如果小产品序号与当前选择的小产品序号相同则return，否则更新界面
	if(nProductIndex == m_vvnComboSelectProductIndex[m_nComboSelectPlatformIndex][1])
	{
		return;
	}
	m_vvnComboSelectProductIndex[m_nComboSelectPlatformIndex][1] = nProductIndex;//设置小产品序号
	//否则刷新界面工具
	m_nCurSelectImageIndex = -1;//刷新以后之前选择图片序号重置
	UpdatePreviewImageIndex();	//更新预览图像序号
	/*if(m_vcpImage.size()<=0)
	{*/
		LoadSearchToolTrainImage(m_nCurPageIndex);//跟新序号后，重新加载预览图像
	/*}*/
	bool bRet = UpdateDlgData();//刷新界面图像与图像文本
	if(bRet)
	{
		m_bStatusBarFlag = TRUE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("Model Loading Completed");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		}
		UpdateStatusBar();
	}
	UpdateToolPreviewInfo(-1);//重置选择预览图像信息
}
// 点击加载全部产品radio按钮
void CDlgSearchToolModelLibrary::OnBnClickedRadioLoadAllproduct()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	bool bRct ;
	if(m_bIsLoadAllProduct == 1)//如果是选择加载全部产品
	{
		bRct = true;
	}
	else//如果是选择加载单个产品
	{
		bRct = false;
	}
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_BIGPRODUCT))->EnableWindow(!bRct);//设置大小产品是否可用
	vcBaseVisionAlign* pBaseVisionAlign= m_vpVisionASM[m_nComboSelectPlatformIndex];
	SysPlatformInfo platformInfo;
	pBaseVisionAlign->GetSysPlatformInfo(platformInfo);
	//设置小产品是否可用
	if(platformInfo.m_bProductFaChd)
	{
		((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->EnableWindow(!bRct);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->EnableWindow(false);
	}
	//如果加载全部产品，大小产品下拉列表都营隐藏，加载单个产品则可用
	((CButton*)GetDlgItem(IDC_STATIC_LOADPRODUCT_BIGPRODUCT))->ShowWindow(!bRct);
	((CButton*)GetDlgItem(IDC_STATIC_LOADPRODUCT_SMALLPRODUCT))->ShowWindow(!bRct);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_BIGPRODUCT))->ShowWindow(!bRct);
	((CButton*)GetDlgItem(IDC_COMBO_LOADPRODUCT_SMALLPRODUCT))->ShowWindow(!bRct);

	m_vbLoadAllProduct[m_nComboSelectPlatformIndex] = bRct;//记录是否加载全部产品
	UpdateData(TRUE);


	//否则刷新界面工具
	m_nCurSelectImageIndex = -1;//刷新以后之前选择图片序号重置
	UpdatePreviewImageIndex(); // 刷新预览图象序号

	LoadSearchToolTrainImage(m_nCurPageIndex);//刷新预览图象序号后，根据序号加载模板图片

	bool bRet = UpdateDlgData();//加载模板图片后，跟新界面
	if(bRet)
	{
		m_bStatusBarFlag = TRUE;
		switch (m_nlgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("Model Loading Completed");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			m_strStatusBarInfo1 = _T("");
			m_strStatusBarInfo2 =_T("模板加载完成");
			break;
		}
		UpdateStatusBar();
	}
	UpdateToolPreviewInfo(-1);//重置预览选择模板信息
}

BOOL CDlgSearchToolModelLibrary::CheckSearchToolIsValid(CSearchToolType csType)
{
	BOOL bValid = FALSE;
	DlgSearchToolInfo dlgSearchToolInfo = m_vpVisionASM.at(m_nCurPlatformIndex)->GetDlgSearchToolData();
	for (auto iterDlg = dlgSearchToolInfo.m_mapDlgSearchToolInfo.begin();iterDlg != dlgSearchToolInfo.m_mapDlgSearchToolInfo.end();iterDlg++)
	{
		if(GetSearchToolType(csType) == GetSysInfoString(m_psaSysInfoStrings,iterDlg->first.first) && iterDlg->second)
		{
			bValid = TRUE;
			break;
		}
	}

	return bValid;
}