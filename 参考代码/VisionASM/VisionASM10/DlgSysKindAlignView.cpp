// DlgSysKindAlignView.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindAlignView.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"

#include "DlgSysKind.h"


// CDlgSysKindAlignView 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindAlignView, CDialog)

	CDlgSysKindAlignView::CDlgSysKindAlignView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindAlignView::IDD, pParent)
	, m_nSearchDelayTime(0)
	, m_nNewProductNum(0)
{
	//{{AFX_DATA_INIT(CDlgSysKindAlignView)
	m_strAlignerOffsetD = _T("0.000000");
	m_strAlignerOffsetX = _T("0.000000");
	m_strAlignerOffsetY = _T("0.000000");
	m_strAlignerPrecisionX = _T("0.010000");
	m_strAlignerPrecisionY = _T("0.010000");
	m_strAlignerPrecisionD = _T("0.010000");
	m_nAlignerMaxTimes = 1;
	m_nPlatformIndex = 0;
	m_strCurProductName = _T("");
	m_strCurCalibName = _T("");
	//}}AFX_DATA_INIT
	// 	m_pStatusBar = NULL;
	// 	m_bStatusBarFlag = TRUE;
	// 	m_hIconSucceed = NULL;
	// 	m_hIconFailed = NULL;

	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;
	m_nPrevProductIdx = 0;
	m_nCurProductIdx = 0;
	m_nOldProductIdx = 0;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_strProductNameOld.Empty();
	m_strProductNameOldFa.Empty();
	//	m_pParent = (CVisionASMDlg *)pParent;
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalProductData.push_back(pTotalProduct);
	// 	}

	m_nSrcProductIndex = 0;
	m_nDstProductIndex = 0;

	m_strCurProductNameFa = _T("");
	m_strCurCalibNameFa = _T("");
	m_nSrcProductIndexFa = 0;
	m_nDstProductIndexFa = 0;

	m_bShowProductList = true;
	m_bEnableProductList = true;
}

CDlgSysKindAlignView::~CDlgSysKindAlignView()
{
	DestroyWindow();
}

void CDlgSysKindAlignView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysKindAlignView)
	//	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrlProduct);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT, m_btnNewProduct);
	DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT_MULTIPLE, m_btnNewProductMultiple);
	DDX_Control(pDX, IDC_BUTTON_DELETE_PRODUCT, m_btnDeleteProduct);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_PRODUCT, m_btnClearProduct);
	DDX_Control(pDX, IDC_BTN_PRODUCT_RENAME, m_btnRename);
	DDX_Control(pDX, IDC_BTN_AMEND_BENCHMARK, m_btnAmendBenchMark);
	DDX_Control(pDX, IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, m_comboAlignerOffsetCoordType);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_SRC, m_comboProductSrc);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_DST, m_comboProductDst);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	// 	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_MAX_TIMES, m_nAlignerMaxTimes);
	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME, m_strCurProductName);
	DDX_Text(pDX, IDC_EDIT_CALIB_NAME, m_strCurCalibName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SEARCH_DELAY_TIME, m_nSearchDelayTime);
	DDX_Control(pDX, IDC_STATIC_POS0, m_labelPos0);
	DDX_Control(pDX, IDC_STATIC_POS1, m_labelPos1);
	DDX_Control(pDX, IDC_STATIC_POS2, m_labelPos2);
	DDX_Control(pDX, IDC_STATIC_POS3, m_labelPos3);
	DDX_Control(pDX, IDC_STATIC_POS4, m_labelPos4);
	DDX_Control(pDX, IDC_STATIC_POS5, m_labelPos5);
	DDX_Control(pDX, IDC_STATIC_POS6, m_labelPos6);
	DDX_Control(pDX, IDC_STATIC_POS7, m_labelPos7);
	DDX_Text(pDX, IDC_EDIT_NEW_PRODUCT_NUM, m_nNewProductNum);
	DDX_Control(pDX, IDC_LIST_PRODUCT_FA, m_listctrlProductFa);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_SRC_FA, m_comboProductSrcFa);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_DST_FA, m_comboProductDstFa);
	DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME_FA, m_strCurProductNameFa);
	DDX_Text(pDX, IDC_EDIT_CALIB_NAME_FA, m_strCurCalibNameFa);
}


BEGIN_MESSAGE_MAP(CDlgSysKindAlignView, CDialog)
	//{{AFX_MSG_MAP(CDlgSysKindAlignView)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME, OnBtnProductRename)
	ON_BN_CLICKED(IDC_BTN_AMEND_BENCHMARK, OnBtnAmendBenchmark)
	//	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT, OnButtonNewProduct)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRODUCT, OnButtonDeleteCurProduct)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_PRODUCT, OnBnClickedButtonClearProduct)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnSetfocusEditAlignerMaxTimes)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnSetfocusEditAlignerXPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnSetfocusEditAlignerYPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnSetfocusEditAlignerDPrecision)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnSetfocusEditAlignerDOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnSetfocusEditAlignerXOffset)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnSetfocusEditAlignerYOffset)
	ON_EN_SETFOCUS(IDC_EDIT_NEW_PRODUCT_NUM, OnSetfocusEditNewProductNum)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduct)
	ON_NOTIFY(NM_RCLICK,IDC_LIST_PRODUCT,OnRClickListProduct)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnKillfocusEditAlignerXPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnKillfocusEditAlignerYPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnKillfocusEditAlignerDPrecision)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnKillfocusEditAlignerDOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnKillfocusEditAlignerXOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnKillfocusEditAlignerYOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnKillfocusEditAlignerMaxTimes)
	ON_EN_KILLFOCUS(IDC_EDIT_NEW_PRODUCT_NUM, OnKillfocusEditNewProductNum)
	//	ON_WM_CLOSE()
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnClickListProduct)
	ON_BN_CLICKED(IDC_BTN_CLONE_PRODUCT, OnBtnCloneProduct)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_COPY_PRODUCT_DATA, &CDlgSysKindAlignView::OnBnClickedBtnCopyProductData)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_SRC, &CDlgSysKindAlignView::OnCbnSelchangeComboProductSrc)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_DST, &CDlgSysKindAlignView::OnCbnSelchangeComboProductDst)
	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT_MULTIPLE, &CDlgSysKindAlignView::OnBnClickedButtonNewProductMultiple)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT_FA, OnClickListProductFa)
	ON_NOTIFY(NM_RCLICK,IDC_LIST_PRODUCT_FA,OnRClickListProductFa)
	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME_FA, &CDlgSysKindAlignView::OnBtnProductRenameFa)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_SRC_FA, &CDlgSysKindAlignView::OnCbnSelchangeComboProductSrcFa)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_DST_FA, &CDlgSysKindAlignView::OnCbnSelchangeComboProductDstFa)
	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT_FA, &CDlgSysKindAlignView::OnButtonNewProductFa)
	ON_BN_CLICKED(IDC_BTN_COPY_PRODUCT_DATA_FA, &CDlgSysKindAlignView::OnBnClickedBtnCopyProductDataFa)
END_MESSAGE_MAP()


// CDlgSysKindAlignView 消息处理程序


BOOL CDlgSysKindAlignView::OnInitDialog()
{
	CDialog::OnInitDialog();
//	m_bSysKeyboardEnabled = FALSE;

	RegisterHotKey(this->m_hWnd, WM_HOTKEY_CTL_P, MOD_CONTROL, 'P');

	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	CTotalProductData *pTotalProduct = NULL;
	for (int i=0; i<nPlatformNum; i++)
	{
		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
		m_vpTotalProductData.push_back(pTotalProduct);
	}

	m_nPlatformIndex = m_pParentKind->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;
	// TODO:  在此添加额外的初始化
	CString strModuelDir;
	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
	strModuelDir.ReleaseBuffer();
	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   
	UpdateDialogLanguage();
	UpdateDlgByAuthorize();
	InitDlgItem();

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();
	int nPlatformNumNow = m_pParentKind->m_nPlatformIndex;
	int nCurProductIdx = m_vpTotalProductData.at(nPlatformNumNow)->GetCurProductIndex();
	
	int nCurProductIdxCh = 0;	//zzc add 在大小产品情况下，nCurProductIdx代表一共有多少产品，需通过函数获取具体大\小产品序号
	int nCurProductIdxFa = 0;
	 m_vpTotalProductData.at(nPlatformNumNow)->GetDlgProductIndex(nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);

	m_strProductNameOld = m_vpTotalProductData.at(nPlatformNumNow)->GetItemName(nCurProductIdx);
	m_strProductNameOldFa = m_vpTotalProductData.at(nPlatformNumNow)->GetProductNameFa(nCurProductIdxFa);	//zzc大产品名称

	m_nOldProductIdx = nCurProductIdx;
	CString strLog;
	strLog.Format(_T("打开[工位%d][产品]"),m_nPlatformIndex+1);
	g_opLogFile.SaveLogText(strLog,2);
	strLog.Format(_T("打开产品%d(大品种%d_小品种%d)[产品浏览]"),nCurProductIdx+1,nCurProductIdxFa+1,nCurProductIdxCh+1);
	g_opLogFile.SaveLogText(strLog,3);

	// jjh 1211
	m_fontChangeProduct.CreatePointFont(180, _T("宋体")); 
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetFont(&m_fontChangeProduct); 
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_HIDE); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSysKindAlignView::InitDlgItem()
{
	int i = 0, j = 0;

	// 创建产品列表
	CRect rcListRect;
	m_listctrlProduct.GetClientRect(rcListRect);
	m_listctrlProduct.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
	while(m_listctrlProduct.DeleteColumn(0));
	m_listctrlProduct.InsertColumn(0, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_INDEX)/*_T("序号")*/, LVCFMT_LEFT, rcListRect.Width()/4);
	m_listctrlProduct.InsertColumn(1, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME)/*_T("产品名称")*/, LVCFMT_LEFT, rcListRect.Width()*3/4);
	m_listctrlProduct.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_listctrlProductFa.GetClientRect(rcListRect);
	m_listctrlProductFa.ClientToScreen(rcListRect);
	ScreenToClient(rcListRect);
	while(m_listctrlProductFa.DeleteColumn(0));
	m_listctrlProductFa.InsertColumn(0, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_INDEX)/*_T("序号")*/, LVCFMT_LEFT, rcListRect.Width()/4);
	m_listctrlProductFa.InsertColumn(1, GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME_FA)/*_T("产品名称大")*/, LVCFMT_LEFT, rcListRect.Width()*3/4);
	m_listctrlProductFa.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	// Mali 2013.8.16
	m_comboAlignerOffsetCoordType.InsertString(0, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_COORD));
	m_comboAlignerOffsetCoordType.InsertString(1, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_PLTFM_COORD));
	m_comboAlignerOffsetCoordType.InsertString(2, GetSysInfoString(m_psaSysInfoStrings,IDS_COM_STR_MARK_PLATFORM_COORD));

	// 
	m_vpLabelPos.push_back(&m_labelPos0);
	m_vpLabelPos.push_back(&m_labelPos1);
	m_vpLabelPos.push_back(&m_labelPos2);
	m_vpLabelPos.push_back(&m_labelPos3);
	m_vpLabelPos.push_back(&m_labelPos4);
	m_vpLabelPos.push_back(&m_labelPos5);
	m_vpLabelPos.push_back(&m_labelPos6);
	m_vpLabelPos.push_back(&m_labelPos7);


	// 调整控制板
	int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
	ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

	// 设置按钮
	m_btnNewProduct.SetThemeHelper(&m_themeHelper);
	// 	m_btnNewProduct.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnNewProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnNewProductMultiple.SetThemeHelper(&m_themeHelper);
	// 	m_btnNewProduct.SetIcon(IDI_ICON_CREATE_NEW_PRODUCT_48);
	// 	m_btnNewProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnDeleteProduct.SetThemeHelper(&m_themeHelper);
	// 	m_btnDeleteProduct.SetIcon(IDI_ICON_DELETE_PRODUCT_48);
	// 	m_btnDeleteProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnClearProduct.SetThemeHelper(&m_themeHelper);
	// 	m_btnClearProduct.SetIcon(IDI_ICON_DELETE_PRODUCT_48);
	// 	m_btnClearProduct.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnRename.SetThemeHelper(&m_themeHelper);
	// 	m_btnRename.SetIcon(IDI_ICON_RENAME_PRODUCT_16);
	// 	m_btnRename.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	m_btnAmendBenchMark.SetThemeHelper(&m_themeHelper);
	// 	m_btnAmendBenchMark.SetIcon(IDI_ICON_BENCHFIXTURE_40);
	// 	m_btnAmendBenchMark.SetAlign(CButtonST::ST_ALIGN_VERT);


	int nColorOffset = -20;
	CString strText;
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnPanel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM1, strText);
	// 	//	m_btnShowPlatform1.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform1.SetIcon(m_pParent->m_anIDPlatformIcon[0], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform1.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	//	m_btnShowPlatform1.SetFlat(TRUE);
	// 	m_btnShowPlatform1.SetWindowText(strText);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	//  	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM2, strText);
	// 	// 	m_btnShowPlatform2.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform2.SetIcon(m_pParent->m_anIDPlatformIcon[1], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform2.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform2.SetFlat(FALSE);
	// 	m_btnShowPlatform2.SetWindowText(strText);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform2.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM3, strText);
	// 	// 	m_btnShowPlatform3.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform3.SetIcon(m_pParent->m_anIDPlatformIcon[2], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform3.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform3.SetFlat(FALSE);
	// 	m_btnShowPlatform3.SetWindowText(strText);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform3.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	GetDlgItemText(IDC_RADIO_SHOW_PLATFORM4, strText);
	// 	// 	m_btnShowPlatform4.SetThemeHelper(&m_themeHelper);
	// 	// 	m_btnShowPlatform4.SetIcon(m_pParent->m_anIDPlatformIcon[3], (int)BTNST_AUTO_DARKER);
	// 	// 	m_btnShowPlatform4.SetAlign(CButtonST::ST_ALIGN_VERT);
	// 	// // 	m_btnShowPlatform4.SetFlat(FALSE);
	// 	m_btnShowPlatform4.SetWindowText(strText);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(172, 168, 153), TRUE);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(172+nColorOffset, 168+nColorOffset, 153+nColorOffset), TRUE);
	// 	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(172, 168, 153), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(170, 206, 227), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(231, 242, 249), TRUE);
	// 	m_btnShowPlatform4.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(199, 224, 239), TRUE);
	// 
	// 	m_btnShowPlatform1.SetCheck(1);

	// 创建scGUIDisplay
	m_vpTargetGuiDisplay.clear();
	m_vpObjectGuiDisplay.clear();
	m_vpVirtualObjectGuiDisplay.clear();
	m_vvpTargetGuiDisplay.resize(MAXPOSNUM);
	m_vvpObjectGuiDisplay.resize(MAXPOSNUM);
	m_vpObjectPatSel.resize(MAXPOSNUM, 0);
	m_vpTargetPatSel.resize(MAXPOSNUM, 0);
	scGuiDisplay *pGUIDisplay = NULL;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	for (i=0; i<MAXPOSNUM; i++)
	{
		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_TARGET_DISPLAY0+i*2, this, WS_BORDER);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}
		//		pGUIDisplay->SetBackgroundColor(RGB(0,0,0));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		m_vpTargetGuiDisplay.push_back(pGUIDisplay);
		pGUIDisplay = NULL;


		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_OBJECT_DISPLAY0+i*2, this, WS_BORDER);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}
		//		pGUIDisplay->SetBackgroundColor(RGB(0,0,0));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		m_vpObjectGuiDisplay.push_back(pGUIDisplay);
		pGUIDisplay = NULL;


		pGUIDisplay = new scGuiDisplay;
		pGUIDisplay->Create(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2, this, WS_BORDER);
		pGUIDisplay->SetBackgroundParam(bgDispBGPure,RGB(10,10,10));
		switch (lgidLanguage)
		{
		case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
			pGUIDisplay->SetLanguage(0);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
			pGUIDisplay->SetLanguage(1);
			break;
		case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
		default:
			pGUIDisplay->SetLanguage(1);
			break;
		}
		//		pGUIDisplay->SetBackgroundColor(RGB(0,0,0));
		pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK);
		pGUIDisplay->ShowWindow(SW_HIDE);
		m_vpVirtualObjectGuiDisplay.push_back(pGUIDisplay);
		pGUIDisplay = NULL;



		for (j = 0; j < TARGETCANDIDATENUM; j++)
		{
			pGUIDisplay = new scGuiDisplay;
			pGUIDisplay->Create(IDC_STATIC_TARGETC_DISPLAY0+ i*TARGETCANDIDATENUM + j, this, WS_BORDER);
			pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CLICK);
			
			switch (lgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				pGUIDisplay->SetLanguage(0);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				pGUIDisplay->SetLanguage(1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				pGUIDisplay->SetLanguage(1);
				break;
			}
			m_vvpTargetGuiDisplay[i].push_back(pGUIDisplay);
			pGUIDisplay = NULL;
		}

		for (j = 0; j < OBJECTCANDIDATENUM; j++)
		{
			pGUIDisplay = new scGuiDisplay;
			pGUIDisplay->Create(IDC_STATIC_OBJECTC_DISPLAY0+ i*OBJECTCANDIDATENUM + j, this, WS_BORDER);
			pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CLICK);
			
			switch (lgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				pGUIDisplay->SetLanguage(0);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				pGUIDisplay->SetLanguage(1);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				pGUIDisplay->SetLanguage(1);
				break;
			}
			m_vvpObjectGuiDisplay[i].push_back(pGUIDisplay);
			pGUIDisplay = NULL;
		}

	}


	// 动态计算并调整对话框大小
	int nMaxCamNum = 0;
	int nClientWidth = 0, nClientHeight = 0;
	int nDispInterval = 0, nGRPIndent = 6;
	int nFrame = 6;
	int nStatusBarHeight = 0;	// 状态栏高度
	int nListWidth = 30;
	CRect rcGRP, rcItem1, rcItem2;

	int nPosNum = 0;
	BOOL bVirtualObjUsed = FALSE;
	for (i=0; i<nPlatformNum; i++)
	{
		nPosNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
		SysPlatformInfo platformInfo;
		m_pParent->m_vpVisionASM.at(i)->GetSysPlatformInfo(platformInfo);
		//if (nPosNum == 1)	// 单相机时，以拍摄位置数为模板数
		{
			nPosNum = platformInfo.m_nPositionNum;
		}
		nMaxCamNum = max(nMaxCamNum, nPosNum);
		bVirtualObjUsed |= (platformInfo.m_eAlignerObjectType == eObjectVirtual);
	}

	CRect rcWnd, rcClient;
	// 	GetWindowRect(rcWnd);
	// 	GetClientRect(rcClient);
	// 	rcWnd.right = rcWnd.left + (rcWnd.Width() - rcClient.Width()) + nClientWidth;
	// 	rcWnd.bottom = rcWnd.top + (rcWnd.Height() - rcClient.Height()) + nClientHeight;
	// 	MoveWindow(rcWnd);
	// 	GetClientRect(rcClient);

	// 调整Groups
	int nIntendY = 10, nIntervalY = 10;
	// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->GetWindowRect(rcGRP);
	// 	ScreenToClient(rcGRP);
	// 	GetClientRect(rcClient);
	// 	rcGRP.right = rcClient.right - nFrame - nIntendY;
	// 	rcGRP.bottom = rcClient.bottom-nStatusBarHeight-nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->MoveWindow(rcGRP);

	GetClientRect(rcClient);
	// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->MoveWindow(rcItem1);
	// 
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->MoveWindow(rcItem1);
	// 
	// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->GetWindowRect(rcItem1);
	// 	ScreenToClient(rcItem1);
	// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
	// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
	// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->MoveWindow(rcItem1);


	// 调整 Control Panel
	int nButtonSize = 75;

	// 调整 Control Panel
	int nButtonSizeWidth = 75;
	int nButtonSizeHeight = 30;
	nIntendY = 0, nIntervalY = 0; 
	int nIntendYOK = 6,nIntervalYOK = 6;

	// 调整产品管理中的按钮大小
	CRect rcRect;
	nIntervalY = 10; 
	
	// 调整按钮新建
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->MoveWindow(rcRect);

	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->EnableWindow(m_bEnableProductList);

	// 调整按钮新建大
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->MoveWindow(rcRect);

	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->EnableWindow(m_bEnableProductList);

	GetDlgItem(IDC_BUTTON_CLEAR_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_CLEAR_PRODUCT)->MoveWindow(rcRect);

	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->MoveWindow(rcRect);


	GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->MoveWindow(rcRect);

	GetDlgItem(IDC_BTN_CLONE_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BTN_CLONE_PRODUCT)->MoveWindow(rcRect);

	GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcRect = rcItem1;
	rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
	rcRect.bottom = rcRect.top + rcItem1.Height();
	GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->MoveWindow(rcRect);


	// 调整产品列表大
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.bottom = (rcItem1.top + rcItem2.top - nIntervalY)/2 - nIntervalY/4;
	int tempTop = rcItem1.top;
	GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem1);

	GetDlgItem(IDC_LIST_PRODUCT_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_LIST_PRODUCT_FA)->EnableWindow(m_bEnableProductList);

	// 调整产品列表
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.bottom = rcItem2.top - nIntervalY;
	rcItem1.top = (tempTop + rcItem2.top - nIntervalY)/2 + nIntervalY/4;
	GetDlgItem(IDC_LIST_PRODUCT)->MoveWindow(rcItem1);
	GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowProductList);

	GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.top = rcClient.top;
	rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
	GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem1);

	// 调整产品信息相关
	GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.right = rcClient.right - nFrame;
	GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->MoveWindow(rcItem1);


	GetDlgItem(IDC_STATIC_SEPARATOR3)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.right = rcClient.right;
	GetDlgItem(IDC_STATIC_SEPARATOR3)->MoveWindow(rcItem1);


	GetDlgItem(IDC_STATIC_SEPARATOR11)->GetWindowRect(rcItem1);
	ScreenToClient(rcItem1);
	rcItem1.right = rcClient.right;
	GetDlgItem(IDC_STATIC_SEPARATOR11)->MoveWindow(rcItem1);

// 	GetDlgItem(IDC_STATIC_SEPARATOR9)->GetWindowRect(rcItem1);
// 	ScreenToClient(rcItem1);
// 	rcItem1.right = rcClient.right;
// 	GetDlgItem(IDC_STATIC_SEPARATOR9)->MoveWindow(rcItem1);

	//////////////////////////////////////////////////////////////////////////
	// 根据默认平台调整界面
	SysPlatformInfo platformInfo;
	//	int nPosNum = 0, i = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;
	// 目标Mark界面修改
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);


				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	}
	else
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < TARGETCANDIDATENUM; j++)
			{
				m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
		if (platformInfo.m_bBenchMarkAmend)
		{
			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
	}
// 
// 	// 虚拟对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		//GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
// 	}

	// 对象Mark界面修改
	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
	{
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
		for (i=0; i<MAXPOSNUM; i++)
		{
			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
	}
	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
	// 	for (i=0; i<6; i++)
	// 	{
	// 		if (i < nPosNum)
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
	// 		}
	// 		else
	// 		{
	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
	// 		}
	// 	}

	// 	m_nPrevPlatformIdx = m_nPlatformIndex;
	// 	m_nCurProductIdx = m_comboCurProduct.GetCurSel();
	// 	m_nPrevProductIdx = m_nCurProductIdx;

	// 保存一部分控件的位置信息，以便于单双层结构之间的切换
	CRect tmpRect;

	// 按钮
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BUTTON_NEW_PRODUCT_FA,tmpRect));

	GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BUTTON_NEW_PRODUCT,tmpRect));

	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_PRODUCT_RENAME_FA,tmpRect));

	GetDlgItem(IDC_BTN_PRODUCT_RENAME)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_PRODUCT_RENAME,tmpRect));

	GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_COPY_PRODUCT_DATA_FA,tmpRect));

	GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_BTN_COPY_PRODUCT_DATA,tmpRect));

	// 下拉框
	GetDlgItem(IDC_COMBO_PRODUCT_SRC_FA)->GetWindowRect(tmpRect);

	GetDlgItem(IDC_COMBO_PRODUCT_SRC_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_COMBO_PRODUCT_SRC_FA)->EnableWindow(m_bEnableProductList);

	GetDlgItem(IDC_COMBO_PRODUCT_DST_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_COMBO_PRODUCT_DST_FA)->EnableWindow(m_bEnableProductList);

	GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->EnableWindow(m_bEnableProductList);
	
	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->EnableWindow(m_bEnableProductList);
	
	GetDlgItem(IDC_STATIC_COPY_TO_PRO)->ShowWindow(m_bShowProductList);
	GetDlgItem(IDC_STATIC_COPY_TO_PRO)->EnableWindow(m_bEnableProductList);
	
	
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_PRODUCT_SRC_FA,tmpRect));

	GetDlgItem(IDC_COMBO_PRODUCT_SRC)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_PRODUCT_SRC,tmpRect));

	GetDlgItem(IDC_COMBO_PRODUCT_DST_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_PRODUCT_DST_FA,tmpRect));

	GetDlgItem(IDC_COMBO_PRODUCT_DST)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_COMBO_PRODUCT_DST,tmpRect));

	// 编辑框
	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CUR_PRODUCT_NAME_FA,tmpRect));

	GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CUR_PRODUCT_NAME,tmpRect));

	GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CALIB_NAME_FA,tmpRect));

	GetDlgItem(IDC_EDIT_CALIB_NAME)->GetWindowRect(tmpRect);
	ScreenToClient(tmpRect);
	m_InitPosition.insert(pair<int,CRect>(IDC_EDIT_CALIB_NAME,tmpRect));

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignView::MoveDlgItem()
{
	if (GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->GetSafeHwnd() != NULL)
	{
		UpdateDlgByAuthorize();
		int i = 0, j = 0;

		// 创建产品列表
		CRect rcListRect;

		// 调整控制板
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

		int nColorOffset = -20;
		CString strText;

		// 动态计算并调整对话框大小
		int nMaxCamNum = 0;
		int nClientWidth = 0, nClientHeight = 0;
		int nDispInterval = 0, nGRPIndent = 6;
		int nFrame = 6;
		int nStatusBarHeight = 0;	// 状态栏高度
		int nListWidth = 30;
		CRect rcGRP, rcItem1, rcItem2;

		int nPosNum = 0;
		BOOL bVirtualObjUsed = FALSE;
		for (i=0; i<nPlatformNum; i++)
		{
			nPosNum = m_pParent->m_vpVisionASM.at(i)->GetCameraNum();
			SysPlatformInfo platformInfo;
			m_pParent->m_vpVisionASM.at(i)->GetSysPlatformInfo(platformInfo);
			//if (nPosNum == 1)	// 单相机时，以拍摄位置数为模板数
			{
				nPosNum = platformInfo.m_nPositionNum;
			}
			nMaxCamNum = max(nMaxCamNum, nPosNum);
			bVirtualObjUsed |= (platformInfo.m_eAlignerObjectType == eObjectVirtual);
		}
		CRect rcWnd, rcClient;
		// 	GetWindowRect(rcWnd);
		// 	GetClientRect(rcClient);
		// 	rcWnd.right = rcWnd.left + (rcWnd.Width() - rcClient.Width()) + nClientWidth;
		// 	rcWnd.bottom = rcWnd.top + (rcWnd.Height() - rcClient.Height()) + nClientHeight;
		// 	MoveWindow(rcWnd);
		// 	GetClientRect(rcClient);

		// 调整Groups
		int nIntendY = 10, nIntervalY = 10;
		// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->GetWindowRect(rcGRP);
		// 	ScreenToClient(rcGRP);
		// 	GetClientRect(rcClient);
		// 	rcGRP.right = rcClient.right - nFrame - nIntendY;
		// 	rcGRP.bottom = rcClient.bottom-nStatusBarHeight-nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_PRODUCT_INFO)->MoveWindow(rcGRP);

		GetClientRect(rcClient);
		// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_TARGET_PATTERN)->MoveWindow(rcItem1);
		// 
		// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->MoveWindow(rcItem1);
		// 
		// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->GetWindowRect(rcItem1);
		// 	ScreenToClient(rcItem1);
		// 	//	rcItem1.right = rcGRP.right - nGRPIndent;
		// 	rcItem1.right = rcClient.right - nFrame - nIntendY - nGRPIndent;
		// 	GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->MoveWindow(rcItem1);


		// 调整 Control Panel
		int nButtonSize = 75;

		// 调整 Control Panel
		int nButtonSizeWidth = 75;
		int nButtonSizeHeight = 30;
		nIntendY = 0, nIntervalY = 0; 
		int nIntendYOK = 6,nIntervalYOK = 6;

		// 调整产品管理中的按钮大小
		CRect rcRect;
		nIntervalY = 10; 
		
		// 调整按钮新建
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->MoveWindow(rcRect);

		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->EnableWindow(m_bEnableProductList);

		// 调整按钮新建大
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->MoveWindow(rcRect);

		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->EnableWindow(m_bEnableProductList);

		GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->EnableWindow(m_bEnableProductList);

		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->EnableWindow(m_bEnableProductList);
		
		GetDlgItem(IDC_STATIC_COPY_TO_PRO)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_STATIC_COPY_TO_PRO)->EnableWindow(m_bEnableProductList);

		GetDlgItem(IDC_BUTTON_CLEAR_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_CLEAR_PRODUCT)->MoveWindow(rcRect);

		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->MoveWindow(rcRect);


		GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->MoveWindow(rcRect);


		GetDlgItem(IDC_BTN_CLONE_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BTN_CLONE_PRODUCT)->MoveWindow(rcRect);

		GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcRect = rcItem1;
		rcRect.top = rcClient.bottom-nStatusBarHeight - rcItem1.Height() - nIntervalY;
		rcRect.bottom = rcRect.top + rcItem1.Height();
		GetDlgItem(IDC_BUTTON_DELETE_PRODUCT)->MoveWindow(rcRect);


		// 调整产品列表大
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
		ScreenToClient(rcItem2);
		GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.bottom = (rcItem1.top + rcItem2.top - nIntervalY)/2 - nIntervalY/4;
		int tempTop = rcItem1.top;
		GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem1);
		
		GetDlgItem(IDC_LIST_PRODUCT_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_LIST_PRODUCT_FA)->EnableWindow(m_bEnableProductList);

		// 调整产品列表
		GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowRect(rcItem2);
		ScreenToClient(rcItem2);
		GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		//rcItem1.bottom = rcClient.bottom-nStatusBarHeight - nButtonSize - nIntervalY*2;
		rcItem1.bottom = rcItem2.top - nIntervalY;
		rcItem1.top = (tempTop + rcItem2.top - nIntervalY)/2 + nIntervalY/4;
		GetDlgItem(IDC_LIST_PRODUCT)->MoveWindow(rcItem1);
		GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(TRUE);


		GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableProductList);

		GetDlgItem(IDC_COMBO_PRODUCT_SRC_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_COMBO_PRODUCT_SRC_FA)->EnableWindow(m_bEnableProductList);
		GetDlgItem(IDC_COMBO_PRODUCT_DST_FA)->ShowWindow(m_bShowProductList);
		GetDlgItem(IDC_COMBO_PRODUCT_DST_FA)->EnableWindow(m_bEnableProductList);
		

		SysPlatformInfo platformInfo;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		if(platformInfo.m_bProductFaChd)
		{
			// 双层结构

			// 显示小产品控件
			GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_LIST_PRODUCT)->EnableWindow(m_bEnableProductList);

			GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(m_bShowProductList);
			
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(TRUE);

			GetDlgItem(IDC_COMBO_PRODUCT_SRC)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_COMBO_PRODUCT_SRC)->EnableWindow(m_bEnableProductList);

			GetDlgItem(IDC_COMBO_PRODUCT_DST)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_COMBO_PRODUCT_DST)->EnableWindow(m_bEnableProductList);

			GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA)->EnableWindow(m_bEnableProductList);

			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(m_bShowProductList);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->EnableWindow(m_bEnableProductList);

			// 修改产品列表大的列名
			LVCOLUMN m_vcolumn;
			CString strText = GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME_FA)/*_T("产品名称大")*/;
			m_vcolumn.mask = LVCF_TEXT;
			m_vcolumn.pszText =strText.GetBuffer(0);
			m_vcolumn.cchTextMax = strText.GetLength();
			m_listctrlProductFa.SetColumn(1,&m_vcolumn);

			map<int, CRect>::iterator iter;
			iter = m_InitPosition.find(IDC_BUTTON_NEW_PRODUCT_FA);
			if(iter != m_InitPosition.end())
			{
				GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->GetWindowRect(rcItem1);
				ScreenToClient(rcItem1);
				int temDst = iter->second.left - rcItem1.left;
				rcItem1.left+=temDst;
				rcItem1.right+=temDst;
				GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->MoveWindow(rcItem1);
			}

			iter = m_InitPosition.find(IDC_BTN_PRODUCT_RENAME_FA);
			if(iter != m_InitPosition.end())
			{
				GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->MoveWindow(iter->second);
			}

			iter = m_InitPosition.find(IDC_EDIT_CALIB_NAME_FA);
			if(iter != m_InitPosition.end())
			{
				GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->MoveWindow(iter->second);
			}

			UpdateDialogLanguage();
		}
		else
		{
			// 单层结构

			// 隐藏小产品控件，只保留大产品控件
			GetDlgItem(IDC_LIST_PRODUCT)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_COMBO_PRODUCT_SRC)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_COMBO_PRODUCT_DST)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->ShowWindow(HIDE_WINDOW);
			GetDlgItem(IDC_EDIT_CALIB_NAME)->ShowWindow(HIDE_WINDOW);

			// 把小产品控件名称赋值给大产品控件
			CString tempString;
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT)->GetWindowTextA(tempString);
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->SetWindowText(tempString);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME)->GetWindowTextA(tempString);
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->SetWindowText(tempString);
			GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA)->GetWindowTextA(tempString);
			GetDlgItem(IDC_BTN_COPY_PRODUCT_DATA_FA)->SetWindowText(tempString);

			// 修改产品列表大的列名
			LVCOLUMN m_vcolumn;
			CString strText = GetSysInfoString(m_psaSysInfoStrings,IDS_LIST_PRODUCT_NAME)/*_T("产品名称")*/;
			m_vcolumn.mask = LVCF_TEXT;
			m_vcolumn.pszText =strText.GetBuffer(0);
			m_vcolumn.cchTextMax = strText.GetLength();
			m_listctrlProductFa.SetColumn(1,&m_vcolumn);

			// 调整产品列表大
			GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			rcItem2.bottom = rcItem1.bottom;
			GetDlgItem(IDC_LIST_PRODUCT_FA)->MoveWindow(rcItem2);

			// 调整按钮新建大
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			GetDlgItem(IDC_LIST_PRODUCT_FA)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			int temDst = ((rcItem2.right + rcItem2.left)/2 - (rcItem1.right + rcItem1.left)/2);
			rcItem1.left+=temDst;
			rcItem1.right+=temDst;
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT_FA)->MoveWindow(rcItem1);

			// 调整按钮重命名大
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			GetDlgItem(IDC_EDIT_CUR_PRODUCT_NAME)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			temDst = rcItem2.left - rcItem1.left;
			rcItem1.left+=temDst;
			rcItem1.right+=temDst;
			GetDlgItem(IDC_BTN_PRODUCT_RENAME_FA)->MoveWindow(rcItem1);
			
			// 编辑框
			GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			GetDlgItem(IDC_EDIT_CALIB_NAME)->GetWindowRect(rcItem2);
			ScreenToClient(rcItem2);
			rcItem1.right = rcItem2.right;
			GetDlgItem(IDC_EDIT_CALIB_NAME_FA)->MoveWindow(rcItem1);
		}

		GetDlgItem(IDC_STATIC_SEPARATOR2)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.top = rcClient.top;
		rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_STATIC_SEPARATOR2)->MoveWindow(rcItem1);

		// 调整产品信息相关
		GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.right = rcClient.right - nFrame;
		GetDlgItem(IDC_STATIC_GRP_ALIGN_PARAM)->MoveWindow(rcItem1);


		GetDlgItem(IDC_STATIC_SEPARATOR3)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.right = rcClient.right;
		GetDlgItem(IDC_STATIC_SEPARATOR3)->MoveWindow(rcItem1);

		GetDlgItem(IDC_STATIC_SEPARATOR11)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.right = rcClient.right;
		GetDlgItem(IDC_STATIC_SEPARATOR11)->MoveWindow(rcItem1);


// 		GetDlgItem(IDC_STATIC_SEPARATOR9)->GetWindowRect(rcItem1);
// 		ScreenToClient(rcItem1);
// 		rcItem1.right = rcClient.right;
// 		GetDlgItem(IDC_STATIC_SEPARATOR9)->MoveWindow(rcItem1);

		//////////////////////////////////////////////////////////////////////////
		// 根据默认平台调整界面
//		SysPlatformInfo platformInfo;
		//	int nPosNum = 0, i = 0;
//		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
		nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
//		nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		nPosNum = platformInfo.m_nPositionNum;
		// 目标Mark界面修改
		if (platformInfo.m_eAlignerTargetType == eTargetOnline)
		{
			for (i=0; i<MAXPOSNUM; i++)
			{
				if (i < nPosNum)
				{
					//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
					m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
					m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);


					for(j = 0; j < TARGETCANDIDATENUM; j++)
					{
						m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_SHOW);
					}
				}
				else
				{
					//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
					m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

					for(j = 0; j < TARGETCANDIDATENUM; j++)
					{
						m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}
				}
			}
			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
		else
		{
			for (i=0; i<MAXPOSNUM; i++)
			{
				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
			if (platformInfo.m_bBenchMarkAmend)
			{
				GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
			}
			else
			{
				GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
			}
		}

// 		// 虚拟对象Mark界面修改
// 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 		{
// 			//GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
// 			for (i=0; i<MAXPOSNUM; i++)
// 			{
// 				if (i < nPosNum)
// 				{
// 					//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 					GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 					m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 				}
// 				else
// 				{
// 					//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 					GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 					m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
// 			for (i=0; i<MAXPOSNUM; i++)
// 			{
// 				//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}

		// 对象Mark界面修改
		if (platformInfo.m_eAlignerObjectType != eObjectBench)
		{
			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
			for (i=0; i<MAXPOSNUM; i++)
			{
				if (i < nPosNum)
				{
					//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
					m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
					m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);

					for(j = 0; j < OBJECTCANDIDATENUM; j++)
					{
						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
					}
				}
				else
				{
					//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
					m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

					for(j = 0; j < OBJECTCANDIDATENUM; j++)
					{
						m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}
				}
			}
		}
		else
		{
			//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
			for (i=0; i<MAXPOSNUM; i++)
			{
				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignView::CheckDlgData()
{
	UpdateData(TRUE);

	// 对位次数
	m_nAlignerMaxTimes  = (m_nAlignerMaxTimes > 10) ? 10 : ((m_nAlignerMaxTimes < 1) ? 1 : m_nAlignerMaxTimes);
}

void CDlgSysKindAlignView::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	// UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CProductData product;

    //小数位数格式化
    CString strXY,strD;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);

	if (bSaveAndValidate) // 从界面获取信息
	{

		// 从界面获取精度、补偿、检测等信息

		// 从界面获取目标搜索信息

		// 从界面获取对象搜索信息
		UpdateData(TRUE);
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		pTotalProduct->SetCurProductIndex(nCurProdIdx);
		product = pTotalProduct->GetItem(nCurProdIdx);
		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
		CCoordPos pos;
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX)/PRECISIONXYUNIT;
			pos.m_dPosY = atof(m_strAlignerPrecisionY)/PRECISIONXYUNIT;
			pos.m_dAngle = atof(m_strAlignerPrecisionD)/PRECISIONDUNIT;
		}
		else
		{
			pos.m_dPosX = atof(m_strAlignerPrecisionX);
			pos.m_dPosY = atof(m_strAlignerPrecisionY);
			pos.m_dAngle = atof(m_strAlignerPrecisionD);
		}
		
		product.m_pAlignerParam->SetAlignerPrecision(pos);
		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
		pos.m_dPosX = atof(m_strAlignerOffsetX);
		pos.m_dPosY = atof(m_strAlignerOffsetY);
		pos.m_dAngle = atof(m_strAlignerOffsetD);		
		product.m_pAlignerParam->SetAlignerOffset(pos);
// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// 		{
// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// 		}
		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else //从存储的数据更新界面
	{
		BeginWaitCursor();

		// 用存储的数据精度、补偿、检测更新界面显示

		// 用存储的数据目标搜索信息更新界面显示

		// 用存储的数据对象搜索信息更新界面显示

		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nProductNum > 0)
		{
			if (nCurProdIdx < 0)
			{
				nCurProdIdx = 0;
			}
			else if (nCurProdIdx >= nProductNum)
			{
				nCurProdIdx = nProductNum - 1;
			}
		}
		else
		{
			nCurProdIdx = -1;
		}
		pTotalProduct->SetCurProductIndex(nCurProdIdx);
		// 从总产品信息中获取当前产品信息，如果总产品数为0，则增加一个空产品
		if (nCurProdIdx < 0)
		{
			SysPlatformInfo platformInfo;
			m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
			product = CProductData(platformInfo);
			CString strNewName = GetNewDefName();
			pTotalProduct->AddItem(product);
			pTotalProduct->SetItemName(0, strNewName);
			pTotalProduct->SetCurProductIndex(0);
			pTotalProduct->SetProductIndex(0,0,0);
			nCurProdIdx = pTotalProduct->GetCurProductIndex();

			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(nCurProdIdx);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveJobCameraInfo(); // 保存随产品的切换曝光增益配置文件

			m_bProdNewCreate = TRUE;
			m_strProductNameOld = strNewName;
			m_strProductNameOldFa = strNewName;	//zzc新增一个产品项，也增加一个大产品
		}
		product = pTotalProduct->GetItem(nCurProdIdx);


		//m_bLoadSingleProductSearchToolEnable优先级高于m_bLoadSingleProductSearchToolEnableFaChd，所以在单产品下，不会加载整个大产品
		// 启用大小品种且只加载单个大产品时，加载当前品种对应大品种下的其他全部小品种
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd && !sysPlatformInfo.m_bLoadSingleProductSearchToolEnable)
		{
			int nProductIndexFa = 0;
			int nProductIndexChd = 0;
			pTotalProduct->GetDlgProductIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);

			int nProductCountChd = pTotalProduct->GetProductCountChd(nProductIndexFa);
			for (int i =0; i<nProductCountChd; i++)
			{
				int nProductIndex = pTotalProduct->GetProductIndex(nProductIndexFa, i);
				if (nProductIndex == nCurProdIdx) continue;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nProductIndex, FALSE, TRUE);
			}
		}

		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateVisionASMTool(nCurProdIdx, FALSE, TRUE); // 更新当前视觉对位工具（加载定位工具）
		// m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);

		int nProductIndexFa, nProductIndexChd;
		pTotalProduct->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);	//zzc mark 先根据当前所有产品信息，得到当前大产品与小产品序号
		m_strCurProductNameFa.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalProduct->GetProductNameFa(nProductIndexFa));
		m_strCurProductName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalProduct->GetItemName(nCurProdIdx));

		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
		m_strCurCalibNameFa.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalCalibData->GetCalibNameFa(nProductIndexFa));
		m_strCurCalibName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalCalibData->GetItemName(nCurProdIdx));

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;
		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();

		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerPrecisionX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerPrecisionY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerPrecisionD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("Precision X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("Precision Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("设定精度X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("设定精度Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("设定精度θ(°)"));
                break;
            }

            GetDlgItem(IDC_STATIC_PX)->SetWindowText(_T("um"));
            GetDlgItem(IDC_STATIC_PY)->SetWindowText(_T("um"));
            GetDlgItem(IDC_STATIC_PD)->SetWindowText(_T("°"));
		}
		else
		{
			m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("Precision X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("Precision Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("设定精度X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("设定精度Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("设定精度θ(°)"));
                break;
            }
            GetDlgItem(IDC_STATIC_PX)->SetWindowText(_T("mm"));
            GetDlgItem(IDC_STATIC_PY)->SetWindowText(_T("mm"));
            GetDlgItem(IDC_STATIC_PD)->SetWindowText(_T("°"));
		}

		
		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
		pos = product.m_pAlignerParam->GetAlignerOffset();
		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
		m_nSearchDelayTime = product.m_pAlignerParam->GetSearchDelayTime();

		nProductNum = pTotalProduct->GetCount();
// 
// 		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
// 		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
// 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
// 		{
// 			int nMBReturn = IDNO;
// 			if (!m_bProdNewCreate)
// 			{
// 				CString strInfo = _T("该产品的平台标定结果与系统的标定结果不同。");
// 				strInfo += _T("\n使用该产品的标定结果更新系统标定结果，请点击“是”；");
// 				strInfo += _T("\n使用系统标定结果更新该产品的标定结果，请点击“否”。");
// 				nMBReturn = AfxMessageBox(strInfo, MB_YESNO);
// 			}
// 
// 			if (nMBReturn == IDYES)	// 用产品标定结果代替当前平台标定结果
// 			{
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(product.m_vpCalibratedInfo);
// 			}
// 			else if (nMBReturn == IDNO)	// 用当前平台标定结果代替产品标定结果
// 			{
// 				int j = 0;
// 				for (j=0; j<product.m_vpCalibratedInfo.size(); j++)
// 				{
// 					if (product.m_vpCalibratedInfo.at(j) != NULL)
// 					{
// 						delete product.m_vpCalibratedInfo.at(j);
// 						product.m_vpCalibratedInfo.at(j) = NULL;
// 					}
// 				}
// 				int nNewSize = vpCurCalibratedInfo.size();
// 				product.m_vpCalibratedInfo.resize(nNewSize, NULL);
// 				for (j=0; j<nNewSize; j++)
// 				{
// 					if (vpCurCalibratedInfo.at(j) != NULL)
// 					{
// 						product.m_vpCalibratedInfo.at(j) = new CCalibratedInfo(*vpCurCalibratedInfo.at(j));
// 					}
// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
// 
// 				m_bCalibInfoChanged = TRUE;
// 			}
// 		}

		if (product.m_pAlignerParam->GetTargetCandidateEnable())
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		if (product.m_pAlignerParam->GetTargetCandidateSyncEnable())
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		if (product.m_pAlignerParam->GetObjectCandidateEnable())
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		if (product.m_pAlignerParam->GetObjectCandidateSyncEnable())
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}


		// 目标Mark模板图像显示
		int i = 0, j = 0;
		for (i=0; i<MAXPOSNUM; i++)
		{
			for (j = 0; j < TARGETCANDIDATENUM; j++)
			{
				m_vvpTargetGuiDisplay[i][j]->ClearScreen();
			}
		}

		if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
		{
			CBaseSearchTool* pTargetSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
			{
				j = m_vpTargetPatSel.at(i);
				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);

				BOOL bExistSearchTool = IsSearchToolSelected(pTargetSearchTool);
				if (pTargetSearchTool != NULL  && bExistSearchTool)
				{
					image.Release();
					pTargetSearchTool->GetPatternImage(image);
					m_vpTargetGuiDisplay.at(i)->ClearScreen();
					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
				}
				else
				{
					m_vpTargetGuiDisplay.at(i)->ClearScreen();
				}

				for(j = 0; j<TARGETCANDIDATENUM; j++)
				{
					pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);

					/////////.................判断定位工具是否选中，若否，则不显示使用该定位工具制作模板所保存的图片
					bExistSearchTool = IsSearchToolSelected(pTargetSearchTool);

					if (pTargetSearchTool != NULL && bExistSearchTool)
					{
						image.Release();
						pTargetSearchTool->GetPatternImage(image);
						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
						m_vvpTargetGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}

// 		// 虚拟对象Mark模板图像显示
// 		if (product.m_pAlignerParam->GetVirtualObjectMarkEnable())
// 		{
// 			CBaseSearchTool* pVirtualObjSearchTool = NULL;
// 			scImageGray image;
// 			for (i=0; i<product.m_pAlignerParam->GetVirtualObjectMarkPatternNum(); i++)
// 			{
// 				pVirtualObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
// 				if (pVirtualObjSearchTool != NULL)
// 				{
// 					image.Release();
// 					pVirtualObjSearchTool->GetPatternImage(image);
// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 					m_vpVirtualObjectGuiDisplay.at(i)->SetImage(image, TRUE);
// 				}
// 				else
// 				{
// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 				}
// 			}
// 		}

		// 对象Mark模板图像显示
		for (i=0; i<MAXPOSNUM; i++)
		{
			for (j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				m_vvpObjectGuiDisplay[i][j]->ClearScreen();
			}
		}
		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
		{
			CBaseSearchTool* pObjSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
			{
				j = m_vpObjectPatSel.at(i);
				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx, i, j);

				/////////.................判断定位工具是否选中，若否，则不显示使用该定位工具制作模板所保存的图片
				BOOL bExistSearchTool = IsSearchToolSelected(pObjSearchTool);

				if (pObjSearchTool != NULL && bExistSearchTool)
				{
					image.Release();
					pObjSearchTool->GetPatternImage(image);
					m_vpObjectGuiDisplay.at(i)->ClearScreen();
					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
				}
				else
				{
					m_vpObjectGuiDisplay.at(i)->ClearScreen();
				}

				for(j = 0; j<OBJECTCANDIDATENUM; j++)
				{
					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx, i, j);


					/////////.................判断定位工具是否选中，若否，则不显示使用该定位工具制作模板所保存的图片
					bExistSearchTool = IsSearchToolSelected(pObjSearchTool);				

					if (pObjSearchTool != NULL  && bExistSearchTool)
					{
						image.Release();
						pObjSearchTool->GetPatternImage(image);
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
						m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}
		// 		CBaseSearchTool* pObjSearchTool = NULL;
		// 		scImageGray image;
		// 		for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
		// 		{
		// 			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
		// 			if (pObjSearchTool != NULL)
		// 			{
		// 				image.Release();
		// 				pObjSearchTool->GetPatternImage(image);
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 				m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
		// 			}
		// 			else
		// 			{
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 			}
		// 		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}


void CDlgSysKindAlignView::UpdateCalibDlgData(BOOL bSaveAndValidate /*= TRUE*/)
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	if (bSaveAndValidate) // 从界面获取信息
	{
		UpdateData(TRUE);
	}
	else //从存储的数据更新界面
	{
		BeginWaitCursor();

		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();

		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalCalibData();
		int nProductIndexFa, nProductIndexChd;
		CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
		pTotalProduct->GetDlgProductIndex(nCurProdIdx, nProductIndexFa, nProductIndexChd);
		m_strCurCalibNameFa.Format(_T("%03d|%s"), nProductIndexFa+1, pTotalCalibData->GetCalibNameFa(nProductIndexFa));
		m_strCurCalibName.Format(_T("%03d|%s"), nProductIndexChd+1, pTotalCalibData->GetItemName(nCurProdIdx));


		UpdateData(FALSE);
		EndWaitCursor();
	}
}


void CDlgSysKindAlignView::UpdateDlgTempData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
    //小数位数格式化
    CString strXY,strD;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);

	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CProductData product;
	if (bSaveAndValidate) // 从界面获取信息
	{
		// 
		// 		// 从界面获取精度、补偿、检测等信息
		// 
		// 		// 从界面获取目标搜索信息
		// 
		// 		// 从界面获取对象搜索信息
		// 
		// 		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		// 		if (nCurProdIdx >= pTotalProduct->GetCount())
		// 		{
		// 			return;
		// 		}
		// 		pTotalProduct->SetCurProductIndex(nCurProdIdx);
		// 		product = pTotalProduct->GetItem(nCurProdIdx);
		// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
		// 		CCoordPos pos;
		// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
		// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
		// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
		// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
		// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
		// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
		// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
		// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
		// 		product.m_pAlignerParam->SetAlignerOffset(pos);
		// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
		// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
		// 		{
		// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
		// 		}
		// 		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else //从存储的数据更新界面
	{
		BeginWaitCursor();

		m_pParentKind->CheckAllDlgIsCreated();



		// 用存储的数据对象搜索信息更新界面显示

		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
		product = pTotalProduct->GetItem(nCurProdIdx);

		// 用存储的数据精度、补偿、检测更新界面显示
		m_pParentKind->m_pDlgKindAlignSetting->GetDlgTempData(product);	
		m_pParentKind->m_pDlgKindAlignTarget->GetDlgTempData(product);	
		m_pParentKind->m_pDlgKindAlignObject->GetDlgTempData(product);	
		m_pParentKind->m_pDlgKindAlignVirtualObject->GetDlgTempData(product);	
		m_pParentKind->m_pDlgKindAlignCalibTarget->GetDlgTempData(product);
		m_pParentKind->m_pDlgKindAlignInspect->GetDlgTempData(product);

		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
		if (m_pParent->m_SysOptionConfig.m_bChange2um)
		{
			m_strAlignerPrecisionX.Format(strXY, pos.GetPosX()*PRECISIONXYUNIT);
			m_strAlignerPrecisionY.Format(strXY, pos.GetPosY()*PRECISIONXYUNIT);
			m_strAlignerPrecisionD.Format(strD, pos.GetAngle()*PRECISIONDUNIT);

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("Precision X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("Precision Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("设定精度X(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("设定精度Y(um)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("设定精度θ(°)"));
                break;
            }
            GetDlgItem(IDC_STATIC_PX)->SetWindowText(_T("um"));
            GetDlgItem(IDC_STATIC_PY)->SetWindowText(_T("um"));
            GetDlgItem(IDC_STATIC_PD)->SetWindowText(_T("°"));
		}
		else
		{
			m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
			m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
			m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());

            LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
            if (m_psaSysInfoStrings != NULL)
            {
                lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
            }
            switch(lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("Precision X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("Precision Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("Precision θ(°)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_PRECISION_X)->SetWindowText(_T("设定精度X(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_Y)->SetWindowText(_T("设定精度Y(mm)"));
                GetDlgItem(IDC_STATIC_PRECISION_D)->SetWindowText(_T("设定精度θ(°)"));
                break;
            }
            GetDlgItem(IDC_STATIC_PX)->SetWindowText(_T("mm"));
            GetDlgItem(IDC_STATIC_PY)->SetWindowText(_T("mm"));
            GetDlgItem(IDC_STATIC_PD)->SetWindowText(_T("°"));
		}

		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
		pos = product.m_pAlignerParam->GetAlignerOffset();
		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());
		m_nSearchDelayTime = product.m_pAlignerParam->GetSearchDelayTime();

		if (product.m_pAlignerParam->GetTargetCandidateEnable())
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		if (product.m_pAlignerParam->GetTargetCandidateSyncEnable())
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_TARGETC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}


		if (product.m_pAlignerParam->GetObjectCandidateEnable())
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_ENABLE,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		if (product.m_pAlignerParam->GetObjectCandidateSyncEnable())
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_VALID)/*_T("有效")*/);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_OBJECTC_SEARCH_SYNC,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID)/*_T("无效")*/);
		}

		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
		// 		std::vector<CCalibratedInfo*> vpCurCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCalibratedInfo();
		// 		if (!IsCalibratedInfosEqual(vpCurCalibratedInfo, product.m_vpCalibratedInfo))
		// 		{
		// 			int nMBReturn = IDNO;
		// 			if (!m_bProdNewCreate)
		// 			{
		// 				// 				CString strInfo = _T("该产品所使用的平台标定结果与当前平台的标定结果不同。");
		// 				// 				strInfo += _T("\n仍然要使用该产品的标定结果，请点击“是”；");
		// 				// 				strInfo += _T("\n应用当前平台的标定结果到该产品，请点击“否”。");
		// 				nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIBRATED_INFO_VARIED_PLEASE_SELECT), MB_YESNO);
		// 			}
		// 
		// 			if (nMBReturn == IDYES)	// 用产品标定结果代替当前平台标定结果
		// 			{
		// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetCurCalibratedInfo(product.m_vpCalibratedInfo);
		// 			}
		// 			else if (nMBReturn == IDNO)	// 用当前平台标定结果代替产品标定结果
		// 			{
		// 				int j = 0;
		// 				for (j=0; j<product.m_vpCalibratedInfo.size(); j++)
		// 				{
		// 					if (product.m_vpCalibratedInfo.at(j) != NULL)
		// 					{
		// 						delete product.m_vpCalibratedInfo.at(j);
		// 						product.m_vpCalibratedInfo.at(j) = NULL;
		// 					}
		// 				}
		// 				int nNewSize = vpCurCalibratedInfo.size();
		// 				product.m_vpCalibratedInfo.resize(nNewSize, NULL);
		// 				for (j=0; j<nNewSize; j++)
		// 				{
		// 					if (vpCurCalibratedInfo.at(j) != NULL)
		// 					{
		// 						product.m_vpCalibratedInfo.at(j) = new CCalibratedInfo(*vpCurCalibratedInfo.at(j));
		// 					}
		// 				}
		// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItem(m_nCurProductIdx, product);	
		// 
		// 				m_bCalibInfoChanged = TRUE;
		// 			}
		// 		}

		// 目标Mark模板图像显示
		int i = 0, j = 0;
		if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
		{
			CBaseSearchTool* pTargetSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
			{
				j = m_vpTargetPatSel.at(i);
				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);
				BOOL bExistSearchTool = IsSearchToolSelected(pTargetSearchTool);
				if (pTargetSearchTool != NULL && bExistSearchTool)
				{
					image.Release();
					pTargetSearchTool->GetPatternImage(image);
					m_vpTargetGuiDisplay.at(i)->ClearScreen();
					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
				}
				else
				{
					m_vpTargetGuiDisplay.at(i)->ClearScreen();
				}

				for(j = 0; j<TARGETCANDIDATENUM; j++)
				{
					pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);
					bExistSearchTool = IsSearchToolSelected(pTargetSearchTool);
					
					if (pTargetSearchTool != NULL && bExistSearchTool)
					{
						image.Release();
						pTargetSearchTool->GetPatternImage(image);
						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
						m_vvpTargetGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpTargetGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}

// 		// 虚拟对象Mark模板图像显示
// 		if (product.m_pAlignerParam->GetVirtualObjectMarkEnable())
// 		{
// 			CBaseSearchTool* pVirtualObjSearchTool = NULL;
// 			scImageGray image;
// 			for (i=0; i<product.m_pAlignerParam->GetVirtualObjectMarkPatternNum(); i++)
// 			{
// 				pVirtualObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
// 				if (pVirtualObjSearchTool != NULL)
// 				{
// 					image.Release();
// 					pVirtualObjSearchTool->GetPatternImage(image);
// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 					m_vpVirtualObjectGuiDisplay.at(i)->SetImage(image, TRUE);
// 				}
// 				else
// 				{
// 					m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 				}
// 			}
// 		}

		// 对象Mark模板图像显示
		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
		{
			CBaseSearchTool* pObjSearchTool = NULL;
			cpImage image;
			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
			{
				j = m_vpObjectPatSel.at(i);
				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx, i, j);
				BOOL bExistSearchTool = IsSearchToolSelected(pObjSearchTool);
				
				if (pObjSearchTool != NULL && bExistSearchTool)
				{
					image.Release();
					pObjSearchTool->GetPatternImage(image);
					m_vpObjectGuiDisplay.at(i)->ClearScreen();
					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
				}
				else
				{
					m_vpObjectGuiDisplay.at(i)->ClearScreen();
				}

				for(j = 0; j<OBJECTCANDIDATENUM; j++)
				{
					pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx, i, j);
					bExistSearchTool = IsSearchToolSelected(pObjSearchTool);
					
					if (pObjSearchTool != NULL && bExistSearchTool)
					{
						image.Release();
						pObjSearchTool->GetPatternImage(image);
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
						m_vvpObjectGuiDisplay[i][j]->SetImage(image, TRUE);
					}
					else
					{
						m_vvpObjectGuiDisplay[i][j]->ClearScreen();
					}
				}
			}
		}
		// 		CBaseSearchTool* pObjSearchTool = NULL;
		// 		scImageGray image;
		// 		for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
		// 		{
		// 			pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
		// 			if (pObjSearchTool != NULL)
		// 			{
		// 				image.Release();
		// 				pObjSearchTool->GetPatternImage(image);
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 				m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
		// 			}
		// 			else
		// 			{
		// 				m_vpObjectGuiDisplay.at(i)->ClearScreen();
		// 			}
		// 		}

		UpdateData(FALSE);
		EndWaitCursor();
	}
}

void CDlgSysKindAlignView::UpdateStatusBar()
{
	// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
	// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysKindAlignView::UpdateProductListDisplay()
{
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	if (pTotalProduct == NULL)
	{
		return;
	}

	CString strTemp;
	CString strTemp1;

	int nProductIndexFa, nProductIndexChd;
	pTotalProduct->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);

	// 大品种刷新列表
	m_listctrlProductFa.DeleteAllItems();
	m_comboProductSrcFa.ResetContent();
	m_comboProductDstFa.ResetContent();
	for (int i=0; i<pTotalProduct->GetProductCountFa(); i++)
	{
		// 序号
		strTemp.Format(_T("%03d"), i+1);
		m_listctrlProductFa.InsertItem(i, strTemp);

		// 产品名称
		strTemp1 = pTotalProduct->GetProductNameFa(i);
		m_listctrlProductFa.SetItemText(i, 1, strTemp1);
		m_comboProductSrcFa.InsertString(i, strTemp + _T("|") + strTemp1);
		m_comboProductDstFa.InsertString(i, strTemp + _T("|") + strTemp1);

		if (i == nProductIndexFa)
		{
			m_listctrlProductFa.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
		}
	}

	// 小品种刷新列表
	m_listctrlProduct.DeleteAllItems();
	m_comboProductSrc.ResetContent();
	m_comboProductDst.ResetContent();
	for (int i=0; i<pTotalProduct->GetProductCountChd(nProductIndexFa); i++)
	{
		// 序号
		strTemp.Format(_T("%03d"), i+1);
		m_listctrlProduct.InsertItem(i, strTemp);

		// 产品名称
		int nProductIndex =  pTotalProduct->GetProductIndex(nProductIndexFa,i);
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_listctrlProduct.SetItemText(i, 1, strTemp1);
		m_comboProductSrc.InsertString(i, strTemp + _T("|") + strTemp1);
		m_comboProductDst.InsertString(i, strTemp + _T("|") + strTemp1);

		if (i == nProductIndexChd)
		{
			m_listctrlProduct.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
		}
	}
	if (m_nSrcProductIndex >= m_comboProductSrc.GetCount())
	{
		m_nSrcProductIndex = 0;
	}
	m_comboProductSrc.SetCurSel(m_nSrcProductIndex);

	if (m_nDstProductIndex >= m_comboProductDst.GetCount())
	{
		m_nDstProductIndex = 0;
	}
	m_comboProductDst.SetCurSel(m_nDstProductIndex);

	if(m_nSrcProductIndexFa >= m_comboProductSrcFa.GetCount())
	{
		m_nSrcProductIndexFa = 0;
	}
	m_comboProductSrcFa.SetCurSel(m_nSrcProductIndexFa);

	if(m_nDstProductIndexFa >= m_comboProductDstFa.GetCount())
	{
		m_nDstProductIndexFa = 0;
	}
	m_comboProductDstFa.SetCurSel(m_nDstProductIndexFa);
}

void CDlgSysKindAlignView::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_KIND_ALIGN_VIEW") == strDlgID)
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

CString CDlgSysKindAlignView::GetNewDefName()
{
	CString strName;
	strName = _T("------");
	return strName;

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	int nNum = pTotalProduct->GetCount();
//	CString strName;
	for (int i=0; i<=nNum; i++)
	{
		strName.Format(_T("Job%d"), i+1);
		if (IsNameUnique(strName))
		{
			return strName;
		}
	}
	return strName;
	// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	// 	int nNum = pTotalProduct->GetCount();
	// 	CString strName;
	// 	strName.Format(_T("Product%d"), nNum-1);
	// 	if (!IsNameUnique(strName))
	// 	{
	// 		for (int i=0; i<=nNum; i++)
	// 		{
	// 			strName.Format(_T("Product%d"), i);
	// 			if (IsNameUnique(strName))
	// 			{
	// 				return strName;
	// 			}
	// 		}
	// 	}
	// 	return strName;
}

BOOL CDlgSysKindAlignView::IsNameUnique( LPCTSTR lpszName )
{
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nProductNum = pTotalProduct->GetCount();
	CString strName = lpszName;
	if (strName.IsEmpty())
	{
		return FALSE;
	}
	for (int i=0; i<nProductNum; i++)
	{
		if (strName == pTotalProduct->GetItemName(i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignView::IsNameLegal( LPCTSTR lpszName )
{
	CString strName = lpszName;
	if (strName.IsEmpty())
	{
		return FALSE;
	}

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	int nNum = pTotalProduct->GetCount();
	for (int i=0; i<nNum; i++)
	{
		if (strName == pTotalProduct->GetItemName(i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignView::IsCalibratedInfosEqual( std::vector<CCalibratedInfo*> vpCalibratedInfo1, 
	std::vector<CCalibratedInfo*> vpCalibratedInfo2 )
{
	int nSize1 = vpCalibratedInfo1.size();
	int nSize2 = vpCalibratedInfo2.size();
	if (nSize1 != nSize2)
	{
		return FALSE;
	}
	for (int i=0; i<nSize1; i++)
	{
		if(!(*(vpCalibratedInfo1.at(i)) == *(vpCalibratedInfo2.at(i))))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgSysKindAlignView::IsProductChanged()
{

// 	m_bProdPatternChanged = (m_pParentKind->m_pDlgKindAlignTarget->IsProductChanged() 
// 		|| m_pParentKind->m_pDlgKindAlignObject->IsProductChanged()
// 		|| m_pParentKind->m_pDlgKindAlignVirtualObject->IsProductChanged()
// 		|| m_pParentKind->m_pDlgKindAlignCalibTarget->IsProductChanged());

	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
	{
		m_bProdPatternChanged = m_bProdPatternChanged || m_pParentKind->m_pDlgKindAlignTarget->IsProductChanged();
	}
	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
	{
		m_bProdPatternChanged = m_bProdPatternChanged || m_pParentKind->m_pDlgKindAlignObject->IsProductChanged();
	}
	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
	{
		m_bProdPatternChanged = m_bProdPatternChanged || m_pParentKind->m_pDlgKindAlignVirtualObject->IsProductChanged();
	}
	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
	{
		m_bProdPatternChanged = m_bProdPatternChanged || m_pParentKind->m_pDlgKindAlignCalibTarget->IsProductChanged();
	}
	
	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
	{
		m_bProdPatternChanged = m_bProdPatternChanged || m_pParentKind->m_pDlgKindAlignInspect->IsProductChanged();
	}

	if (m_bProdPatternChanged || m_bProdNameChanged || m_bProdBenchMarkChanged || m_bProdNewCreate || m_bCalibInfoChanged)
	{
		return TRUE;
	}

	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
	{
		if (m_pParentKind->m_pDlgKindAlignSetting->IsProductChanged())
		{
			return TRUE;
		}
	}

	// 
	// 	if (m_pParentKind->m_pDlgKindAlignTarget->IsProductChanged())
	// 	{	m_bProdPatternChanged = 
	// 
	// 		return TRUE;
	// 	}
	// 
	// 	if (m_pParentKind->m_pDlgKindAlignObject->IsProductChanged())
	// 	{
	// 		return TRUE;
	// 	}

	// 
	// 	UpdateData(TRUE);
	// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx); // 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	// 	ASSERT(pTotalProduct != NULL);
	// 	CProductData product;
	// 	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	// 	{
	// 		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	// 	}
	// 	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	// 	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	// 	if (m_nAlignerMaxTimes != product.m_pAlignerParam->GetAlignerMaxTime())
	// 	{
	// 		return TRUE;
	// 	}
	// 	if (m_comboAlignerOffsetCoordType.GetCurSel() != (int)product.m_pAlignerParam->GetAlignerOffsetCoordType())
	// 	{
	// 		return TRUE;
	// 	}
	// 	CCoordPos pos;
	// 	pos.m_dPosX = atof(m_strAlignerOffsetX);
	// 	pos.m_dPosY = atof(m_strAlignerOffsetY);
	// 	pos.m_dAngle = atof(m_strAlignerOffsetD);
	// 	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerOffset().m_dPosX 
	// 		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerOffset().m_dPosY 
	// 		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerOffset().m_dAngle)
	// 	{
	// 		return TRUE;
	// 	}
	// 	pos.m_dPosX = atof(m_strAlignerPrecisionX);
	// 	pos.m_dPosY = atof(m_strAlignerPrecisionY);
	// 	pos.m_dAngle = atof(m_strAlignerPrecisionD);
	// 	if (pos.m_dPosX != product.m_pAlignerParam->GetAlignerPrecision().m_dPosX 
	// 		|| pos.m_dPosY != product.m_pAlignerParam->GetAlignerPrecision().m_dPosY 
	// 		|| pos.m_dAngle != product.m_pAlignerParam->GetAlignerPrecision().m_dAngle)
	// 	{
	// 		return TRUE;
	// 	}
	return FALSE;
}

BOOL CDlgSysKindAlignView::IsSearchToolSelected(CBaseSearchTool* pSearchTool)
{
	//UpdateData(TRUE);
	BOOL bExistSearchTool =TRUE;
	SysPlatformInfo sysPlat;
	DlgSearchToolInfo dlgSearchTool;
	CSearchToolType nSearchToolType = eSearchToolUnknown;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlat);
	dlgSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetDlgSearchToolData();
	if (pSearchTool!= NULL)
	{
		nSearchToolType = pSearchTool->GetSearchToolType();
		vector<map<pair<int,int>,BOOL>>::iterator iterSys;
		map<pair<int,CString>,BOOL>::iterator iterDlg;
		iterSys = sysPlat.m_vmapSysSearchToolInfo.begin();
		for (;iterSys != sysPlat.m_vmapSysSearchToolInfo.end();iterSys++)
		{
			if (nSearchToolType == (*iterSys).begin()->first.second)
			{
				for (iterDlg = dlgSearchTool.m_mapDlgSearchToolInfo.begin();iterDlg != dlgSearchTool.m_mapDlgSearchToolInfo.end();iterDlg++)
				{
					if ((*iterSys).begin()->first.first == iterDlg->first.first)
					{
						bExistSearchTool = iterDlg->second;
						break;
					}
				}
				break;
			}
		}
	}

	return bExistSearchTool;
}

BOOL CDlgSysKindAlignView::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgSysKindAlignView::CreateDirectoryPro( LPCTSTR lpszDir )
{
	CString strDir = lpszDir;
	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{	
		// 创建多级目录
		int nLen = 0;
		DWORD dwLevelXAttrib = 0;
		int nSlashIdx = strDir.Find('\\');
		CString strDirLevelX = _T("");
		CString strTemp = strDir;
		while (nSlashIdx != -1)
		{
			nLen = strDirLevelX.GetLength();
			nSlashIdx += (nLen == 0) ? 0 : (nLen + 1);
			strDirLevelX = strDir.Left(nSlashIdx);
			strTemp = strDir.Mid(nSlashIdx + 1);
			dwLevelXAttrib = GetFileAttributes(strDirLevelX);
			if ((dwLevelXAttrib == 0xFFFFFFFF) || ((dwLevelXAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0))
			{
				if (!CreateDirectory(strDirLevelX, NULL))
				{
					return FALSE;
				}
			}
			nSlashIdx = strTemp.Find('\\');
		}
		if (!CreateDirectory(strDir, NULL))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CDlgSysKindAlignView::ClearDirectoryPro( LPCTSTR lpszDir )
{
	CFileFind fileFind; 
	CString strDir(lpszDir);
	CString strPath = strDir + _T("\\*.*");
	BOOL bFound = fileFind.FindFile(strPath); 
	while (bFound) 
	{ 
		bFound = fileFind.FindNextFile(); 

		if (!fileFind.IsDots()) 
		{ 
			CString strFileName = fileFind.GetFileName();
			CString strSubFile = strDir + _T("\\") + strFileName;
			if (fileFind.IsDirectory()) 
			{ 
				if (!ClearDirectoryPro(strSubFile)) //先删除文件夹下边的所有文件
				{
					return FALSE;
				}
				if (!RemoveDirectory(strSubFile)) // 删除该文件夹
				{
					return FALSE;
				}
			} 
			else 
			{ 
				if (!DeleteFile(strSubFile))	//将该文件夹删除
				{
					return FALSE;
				}
			} 
		}
	} 
	fileFind.Close(); 
	return TRUE; 
}

BOOL CDlgSysKindAlignView::RemoveDirectoryPro( LPCTSTR lpszDir )
{
	AlignToolLogRecord(lpszDir, m_nPrevPlatformIdx);

	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return TRUE;
	}

	if (!ClearDirectoryPro(lpszDir))
	{
		return FALSE;
	}
	if (!RemoveDirectory(lpszDir))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CDlgSysKindAlignView::RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
{
	CString strOldDir = lpszOldDir;
	CString strNewDir = lpszNewDir;
	if (strOldDir.IsEmpty() || strNewDir.IsEmpty())
	{
		return FALSE;
	}

	// 如果文件夹不存在，则返回TRUE
	DWORD dwAttributes = GetFileAttributes(strOldDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return FALSE;
	}

	// 文件夹存在，则重命名文件夹
	SHFILEOPSTRUCT shFileOp={0};
	shFileOp.fFlags = FOF_NOCONFIRMATION; // 不出现确认对话框
	TCHAR pszzDir[MAX_PATH] = {0};
	TCHAR *p = strOldDir.GetBuffer(strOldDir.GetLength());
	_tcscat(pszzDir, p);
	shFileOp.pFrom	= pszzDir; 
	shFileOp.pTo	= lpszNewDir;	
	shFileOp.wFunc	= FO_RENAME;	//重命名操作
	if (SHFileOperation(&shFileOp) != 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CDlgSysKindAlignView::SaveSearchToolInfo()
{
	BOOL bSucceed = TRUE;
	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx < 0) && (m_nPrevProductIdx >= pTotalProduct->GetCount()))
	{
		return FALSE;
	}
	// 保存产品信息到磁盘
	CString strDir, strTemp;
	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);
	CPlatformOptionInfo optionInfo;
	pVisionASM->GetPlatformOptionInfo(optionInfo);
	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	strTemp.Format(_T("\\Jobs\\%03d"), m_nPrevProductIdx+1);
	strDir += strTemp;        

	// 保存模板到磁盘

	CProductData *pProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);

	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		// 保存目标Mark模板
		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
		{
			bSucceed =bSucceed && SaveSearchTool(eTargetSearch,strDir,pProduct->m_vvpAlnTargetSearchToolRepo,m_pParentKind->m_vvbAlnTargetSearchToolChanged);
		}
	}
	else
	{
		RemoveDirectoryPro(strDir + _T("\\TargetModels"));
	}

	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
		// 保存虚拟对象模板
		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
		{
			bSucceed = bSucceed && SaveSearchTool(eVirtualSearch,strDir,pProduct->m_vvpAlnVirtualObjectSearchToolRepo,m_pParentKind->m_vvbAlnVirtualObjectSearchToolChanged);
		}
	}
	else
	{
		RemoveDirectoryPro(strDir + _T("\\VirtualObjectModels"));
	}

	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		// 保存对象模板
		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
		{
			bSucceed = bSucceed && SaveSearchTool(eObjectSearch,strDir,pProduct->m_vvpAlnObjectSearchToolRepo,m_pParentKind->m_vvbAlnObjectSearchToolChanged);
		}
	}
	else
	{
		RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
	}
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE
		|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
	{
		// 保存关联模板
		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
		{
			bSucceed = bSucceed && SaveSearchTool(eCalibTargetSearch,strDir,pProduct->m_vvpAlnCalibTargetSearchToolRepo,m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged);
		}
	}
	else
	{
		RemoveDirectoryPro(strDir + _T("\\CalibTargetModels"));
	}
	//if (TRUE == optionInfo.m_bInspectEnable)
	{
		// 保存检测模板
		if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
		{
			bSucceed = bSucceed && SaveSearchTool(eInspectSearch,strDir,pProduct->m_vvpAlnInspectSearchToolRepo,m_pParentKind->m_vvbAlnInspectSearchToolChanged);
		}
	}
	//else
	//{
	//	RemoveDirectoryPro(strDir + _T("\\InspectModels"));
	//}
	return bSucceed;
}

// 保存前一个产品的设置
BOOL CDlgSysKindAlignView::SavePrevProduct()
{
	BOOL bSucceed = TRUE;
	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	{
		m_pParentKind->CheckAllDlgIsCreated();

		// 保存对位参数
		// 保存产品对位设置信息：精度、补偿、检测等
		m_pParentKind->m_pDlgKindAlignSetting->SavePrevProduct();
		m_pParentKind->m_pDlgKindAlignTarget->SavePrevProduct();
		m_pParentKind->m_pDlgKindAlignObject->SavePrevProduct();
		m_pParentKind->m_pDlgKindAlignVirtualObject->SavePrevProduct();
		m_pParentKind->m_pDlgKindAlignCalibTarget->SavePrevProduct();
		m_pParentKind->m_pDlgKindAlignInspect->SavePrevProduct();

		m_pParentKind->m_pDlgKindCalibSetting->SavePrevProduct();

		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nPrevProductIdx);
		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
		}

		bSucceed = SaveSearchToolInfo();

		m_strProductNameOld.Empty();
		m_strProductNameOldFa.Empty();	//zzc保存产品时，清空当前产品名称
	}

	return bSucceed;
}
// BOOL CDlgSysKindAlignView::SavePrevProduct()
// {
// 	BOOL bSucceed = TRUE;
// 	UpdateData(TRUE);
// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// 	{
// 		m_pParentKind->CheckAllDlgIsCreated();
// 
// 		// 保存对位参数
// 		// 保存产品对位设置信息：精度、补偿、检测等
// 		m_pParentKind->m_pDlgKindAlignSetting->SavePrevProduct();
// 		m_pParentKind->m_pDlgKindAlignTarget->SavePrevProduct();
// 		m_pParentKind->m_pDlgKindAlignObject->SavePrevProduct();
// 		m_pParentKind->m_pDlgKindAlignVirtualObject->SavePrevProduct();
// 		m_pParentKind->m_pDlgKindAlignCalibTarget->SavePrevProduct();
// 		m_pParentKind->m_pDlgKindAlignInspect->SavePrevProduct();
// 		// 		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
// 		// 		product.m_pAlignerParam->SetAlignerMaxTime(m_nAlignerMaxTimes);
// 		// 		CCoordPos pos;
// 		// 		pos.m_dPosX = atof(m_strAlignerPrecisionX);
// 		// 		pos.m_dPosY = atof(m_strAlignerPrecisionY);
// 		// 		pos.m_dAngle = atof(m_strAlignerPrecisionD);
// 		// 		product.m_pAlignerParam->SetAlignerPrecision(pos);
// 		// 		product.m_pAlignerParam->SetAlignerOffsetCoordType((AlignerOffsetCoordType)m_comboAlignerOffsetCoordType.GetCurSel());
// 		// 		pos.m_dPosX = atof(m_strAlignerOffsetX);
// 		// 		pos.m_dPosY = atof(m_strAlignerOffsetY);
// 		// 		pos.m_dAngle = atof(m_strAlignerOffsetD);		
// 		// 		product.m_pAlignerParam->SetAlignerOffset(pos);
// 		// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
// 		// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// 		// 		{
// 		// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// 		// 		}
// 		// 
// 		// 		// 		// by adk
// 		// 		// 		CBaseSearchTool *pSearchTool = NULL;
// 		// 		// 		SysPlatformInfo platformInfo;
// 		// 		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 		// 		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
// 		// 		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		// 		// 		for (int j=0; j<nPatternNum; j++)
// 		// 		// 		{
// 		// 		// 			// 			int nPatSel = m_vnCurSelPat[j];
// 		// 		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
// 		// 		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
// 		// 		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
// 		// 		// 			
// 		// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
// 		// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
// 		// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
// 		// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
// 		// 		// 		} // end by
// 		// 
// 		// 		pTotalProduct->SetItem(m_nPrevProductIdx, product);
// 
// 
// 		// 保存产品信息到磁盘
// 		CString strDir, strTemp;
// 		SysPlatformInfo platformInfo;
// 		vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 		pVisionASM->GetSysPlatformInfo(platformInfo);
// 
// 		CPlatformOptionInfo optionInfo;
// 		pVisionASM->GetPlatformOptionInfo(optionInfo);
// 		// 如果产品被重命名，则删除旧文件架
// 		// 		if ((m_bProdNameChanged) && (!m_strProductNameOld.IsEmpty()))
// 		// 		{
// 		// 			strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 		// 			strTemp.Format(_T("\\Jobs\\%s"), m_strProductNameOld);
// 		// 			strDir += strTemp;
// 		// 			RemoveDirectoryPro(strDir);
// 		// 		}
// 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 		//		strTemp.Format(_T("\\Jobs\\%s"), pTotalProduct->GetItemName(m_nPrevProductIdx));
// 		strTemp.Format(_T("\\Jobs\\%03d"), m_nPrevProductIdx+1);
// 		strDir += strTemp;        
// 		//ClearDirectoryPro(strDir); // Mali 2013.9.5 为提升效率
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nPrevProductIdx);
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 		}
// 
// 		//// 检查存储空间是否超过阈值()
// 		if(1)
// 		{
// 			if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
// 			{
// 				if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
// 				{
// 					double dUsedRate = 0.0;
// 					CString StrTmp = m_strModuleDir;
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);
// 
// 					// 进行新产品创建时 ，不进行定位工具的保存
// 					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) && m_bProdNewCreate)
// 					{
// 						CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
// 						CString strTmp2;
// 						strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
// 						CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);
// 						
// 						AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！"));
// 
// 						CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！"));
// 						// 避免后续无法保存的情况
// 						m_strProductNameOld.Empty();
// 						m_strProductNameOldFa.Empty();	//zzc保存产品时，清空当前产品名称
// 						return TRUE;
// 			
// 					}
// 				}
// 			}
// 		}
// 
// 		// 保存模板到磁盘
// //		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		int nPatternNum = platformInfo.m_nPositionNum;
// 		CBaseSearchTool *pSearchTool = NULL;
// 
// 		int j = 0, i = 0;
// 		CProductData *pProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 		
// 		//// 保存目标Mark模板
// 		//if (platformInfo.m_eAlignerTargetType == eTargetOnline)
// 		//{
// 		//	if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//	{
// 		//		CreateDirectoryPro(strDir + _T("\\TargetModels"));
// 		//		for (j=0; j<nPatternNum; j++)
// 		//		{
// 		//			for(i = 0; i<TARGETCANDIDATENUM; i++)
// 		//			{
// 		//				strTemp.Format(_T("\\TargetModels\\Pos%d_M%d"), j, i);
// 		//				pSearchTool = pVisionASM->GetRepoAlnTargetSearchTool(m_nPrevProductIdx, j, i);
// 		//				if (pSearchTool != NULL) 
// 		//				{						
// 		//					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		//					{
// 		//						//strTemp.Format(_T("CH%d目标Mark模板保存失败！"), j);
// 		//						strTemp.Format(_T("Pos%d_M%d%s"), j, i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTEN_SAVE_FAILED));
// 		//						AfxMessageBox(strTemp);
// 		//						bSucceed = FALSE;
// 		//					}						
// 
// 		//				}
// 		//				else
// 		//				{
// 		//					RemoveDirectoryPro(strDir+strTemp);
// 		//				}
// 		//			}
// 		//		}
// 
// 		//	}		
// 		//}
// 		//else
// 		//{
// 		//	RemoveDirectoryPro(strDir + _T("\\TargetModels"));
// 		//}
// 
// 		//目标线程保存模板
// 		if (platformInfo.m_eAlignerTargetType == eTargetOnline)
// 		{
// 			// 保存目标Mark模板
// 			if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 			{
// 				bSucceed =bSucceed && SaveSearchTool(eTargetSearch,strDir,pProduct->m_vvpAlnTargetSearchToolRepo,m_pParentKind->m_vvbAlnTargetSearchToolChanged);
// 			}
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\TargetModels"));
// 		}
// 		
// 		//if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 		//{
// 		//	// 保存虚拟对象Mark模板
// 		//	if (m_bProdNewCreate || m_bProdPatternChanged/* || m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//	{
// 		//		CreateDirectoryPro(strDir + _T("\\VirtualObjectModels"));
// 		//		for (j=0; j<nPatternNum; j++)
// 		//		{
// 		//			for (i=0; i<VIRTUALOBJECTCANDIDATENUM;i++)
// 		//			{
// 		//				strTemp.Format(_T("\\VirtualObjectModels\\Pos%d_M%d"), j, i);
// 		//				pSearchTool = pVisionASM->GetRepoAlnVirtualObjectSearchTool(m_nPrevProductIdx, j, i);
// 		//				if (pSearchTool != NULL)
// 		//				{					
// 		//					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		//					{
// 		//						//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// 		//						strTemp.Format(_T("Pos%d_M%d%s"), j, i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_FAILED));
// 		//						AfxMessageBox(strTemp);
// 		//						bSucceed = FALSE;
// 		//					}	
// 		//				}
// 		//				else
// 		//				{
// 		//					RemoveDirectoryPro(strDir+strTemp);
// 		//				}
// 		//			}
// 
// 		//		}
// 		//	}
// 		//}
// 		//else
// 		//{
// 		//	RemoveDirectoryPro(strDir + _T("\\VirtualObjectModels"));
// 		//}
// 
// 		//线程保存虚拟模板
// 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 		{
// 			// 保存虚拟对象模板
// 			if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 			{
// 				bSucceed = bSucceed && SaveSearchTool(eVirtualSearch,strDir,pProduct->m_vvpAlnVirtualObjectSearchToolRepo,m_pParentKind->m_vvbAlnVirtualObjectSearchToolChanged);
// 			}
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\VirtualObjectModels"));
// 		}
// 		
// 
// 
// 		//if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 		//{
// 		//	// 保存对象Mark模板
// 		//	if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//	{
// 		//		CreateDirectoryPro(strDir + _T("\\ObjectModels"));
// 		//		for (j=0; j<nPatternNum; j++)
// 		//		{
// 		//			for (i=0; i<OBJECTCANDIDATENUM;i++)
// 		//			{
// 		//				strTemp.Format(_T("\\ObjectModels\\Pos%d_M%d"), j, i);
// 		//				pSearchTool = pVisionASM->GetRepoAlnObjectSearchTool(m_nPrevProductIdx, j, i);
// 		//				if (pSearchTool != NULL)
// 		//				{					
// 		//					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		//					{
// 		//						//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// 		//						strTemp.Format(_T("Pos%d_M%d%s"), j, i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
// 		//						AfxMessageBox(strTemp);
// 		//						bSucceed = FALSE;
// 		//					}	
// 		//				}
// 		//				else
// 		//				{
// 		//					RemoveDirectoryPro(strDir+strTemp);
// 		//				}
// 		//			}
// 
// 		//		}
// 		//	}
// 		//}
// 		//else
// 		//{
// 		//	RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
// 		//}
// 
// 		
// 		//线程保存对象模板
// 		if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 		{
// 			// 保存对象模板
// 			if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 			{
// 				bSucceed = bSucceed && SaveSearchTool(eObjectSearch,strDir,pProduct->m_vvpAlnObjectSearchToolRepo,m_pParentKind->m_vvbAlnObjectSearchToolChanged);
// 			}
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
// 		}
// 		
// 		//// 保存关联对象Mark模板
// 		//if (platformInfo.m_bTargetObjectCamSeparate == TRUE)
// 		//{
// 		//	if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//	{
// 		//		CreateDirectoryPro(strDir + _T("\\CalibTargetModels"));
// 		//		for (j=0; j<nPatternNum; j++)
// 		//		{
// 		//			for(i = 0; i<ALNCALIBTARGETCANDIDATENUM; i++)
// 		//			{
// 		//				strTemp.Format(_T("\\CalibTargetModels\\Pos%d_M%d"), j, i);
// 		//				pSearchTool = pVisionASM->GetRepoAlnCalibTargetSearchTool(m_nPrevProductIdx, j, i);
// 		//				if (pSearchTool != NULL) 
// 		//				{						
// 		//					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		//					{
// 		//						//strTemp.Format(_T("CH%d关联Mark模板保存失败！"), j);
// 		//						strTemp.Format(_T("Pos%d_M%d%s"), j, i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_TARGET_MARK_PATTEN_SAVE_FAILED));
// 		//						AfxMessageBox(strTemp);
// 		//						bSucceed = FALSE;
// 		//					}						
// 
// 		//				}
// 		//				else
// 		//				{
// 		//					RemoveDirectoryPro(strDir+strTemp);
// 		//				}
// 		//			}
// 		//		}
// 
// 		//	}		
// 		//}
// 		//else
// 		//{
// 		//	RemoveDirectoryPro(strDir + _T("\\CalibTargetModels"));
// 		//}
// 
// 		//线程保存关联模板
// 		if (platformInfo.m_bTargetObjectCamSeparate == TRUE
// 			|| (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE))
// 		{
// 			// 保存关联模板
// 			if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 			{
// 				bSucceed = bSucceed && SaveSearchTool(eCalibTargetSearch,strDir,pProduct->m_vvpAlnCalibTargetSearchToolRepo,m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged);
// 			}
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\CalibTargetModels"));
// 		}
// 
// 		//// 保存检测模板
// 		//if (TRUE == platformInfo.m_bInspectEnable)
// 		//{
// 		//	if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 		//	{
// 		//		CreateDirectoryPro(strDir + _T("\\InspectModels"));
// 		//		for (j=0; j<nPatternNum; j++)
// 		//		{
// 		//			for(i = 0; i<INSPECTCANDIDATENUM; i++)
// 		//			{
// 		//				strTemp.Format(_T("\\InspectModels\\Pos%d_M%d"), j, i);
// 		//				pSearchTool = pVisionASM->GetRepoAlnInspectSearchTool(m_nPrevProductIdx, j, i);
// 		//				if (pSearchTool != NULL) 
// 		//				{						
// 		//					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		//					{
// 		//						//strTemp.Format(_T("CH%d检测Mark模板保存失败！"), j);
// 		//						strTemp.Format(_T("Pos%d_M%d%s"), j, i, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INSPECT_MARK_PATTEN_SAVE_FAILED));
// 		//						AfxMessageBox(strTemp);
// 		//						bSucceed = FALSE;
// 		//					}						
// 
// 		//				}
// 		//				else
// 		//				{
// 		//					RemoveDirectoryPro(strDir+strTemp);
// 		//				}
// 		//			}
// 		//		}
// 
// 		//	}		
// 		//}
// 		//else
// 		//{
// 		//	RemoveDirectoryPro(strDir + _T("\\InspectModels"));
// 		//}
// 		// 保存检测模板
// 		if (TRUE == optionInfo.m_bInspectEnable)
// 		{
// 			// 保存检测模板
// 			if (m_bProdNewCreate || m_bProdPatternChanged /*|| m_bProdNameChanged*/) // Mali 2013.9.5 为提升效率//2020.1.6提升效率取消产品名变化再次保存产品
// 			{
// 				bSucceed = bSucceed && SaveSearchTool(eInspectSearch,strDir,pProduct->m_vvpAlnInspectSearchToolRepo,m_pParentKind->m_vvbAlnInspectSearchToolChanged);
// 			}
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\InspectModels"));
// 		}
// 	
// 		m_strProductNameOld.Empty();
// 		m_strProductNameOldFa.Empty();	//zzc保存产品时，清空当前产品名称
// 	}
// 
// 	return bSucceed;
// }

BOOL CDlgSysKindAlignView::SaveSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> vvpSearchTool,std::vector<VbSearchToolChanged> vvbSearchToolChanged)
{
	if(vvpSearchTool.size() != vvbSearchToolChanged.size())
	{
		return false;//工具数量应该与工具修改标志数量一致
	}
	else
	{
		for (int i = 0;i < vvbSearchToolChanged.size();i++)
		{
			if(vvbSearchToolChanged[i].size() != vvpSearchTool[i].size())
				return false;//工具数量应该与工具修改标志数量一致
		}
	}
	bool bSucceed =true;
	//工具路径
	CString strPatternDir;
	//工具保存失败
	CString strSaveFail;
	//工具保存超时
	CString strSaveTimeout;
	//临时字符串
	CString strTemp;
	//当前工具的位置数
	int nPatternNum;
	int i,j;
	switch(searchMode)
	{
	case eTargetSearch:
		{
			strPatternDir = strDir+ _T("\\TargetModels");
			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTERN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTEN_SAVE_FAILED);
		}
		break;
	case eVirtualSearch:
		{
			strPatternDir = strDir+ _T("\\VirtualObjectModels");
			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_FAILED);
		}
		break;
	case eObjectSearch:
		{
			strPatternDir = strDir+ _T("\\ObjectModels");
			strSaveTimeout =  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED);
		}
		break;
	case eCalibTargetSearch:
		{
			strPatternDir = strDir+ _T("\\CalibTargetModels");
			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_TARGET_MARK_PATTEN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_TARGET_MARK_PATTEN_SAVE_FAILED);
		}
		break;
	case  eInspectSearch:
		{
			strPatternDir = strDir+ _T("\\InspectModels");
			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INSPECT_MARK_PATTERN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_INSPECT_MARK_PATTEN_SAVE_FAILED);
		}
		break;
	case  eCalibSearch:
		{
			strPatternDir = strDir+ _T("\\CalibModels");
			strSaveTimeout = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_TIMEOUT);
			strSaveFail = GetSysInfoString(m_psaSysInfoStrings,IDS_MB_CALIB_PATTERN_SAVE_FAILED);
		}
		break;
	default:
			return false;
	}
	// 赋值临时变量
	nPatternNum = vvpSearchTool.size();
	CreateDirectoryPro(strPatternDir);

	//线程
	HANDLE SearchToolSaveModelThread[MAXPOSNUM];
	//线程传递函数
	CSaveSearchToolRepositoryThreadParam SaveSearchToolThreadParam[MAXPOSNUM];
	for (j=0; j<nPatternNum; j++)
	{
		SaveSearchToolThreadParam[j] =CSaveSearchToolRepositoryThreadParam(strPatternDir,j,vvpSearchTool[j],vvbSearchToolChanged[j]);//赋值线程所需的传递函数
		SearchToolSaveModelThread[j] = CreateThread(NULL,0,vcBaseVisionAlign::SaveSearchToolRepositoryThread,&SaveSearchToolThreadParam[j],0,NULL);//创建线程
	}
	int nReturn = WaitForMultipleObjects(nPatternNum,SearchToolSaveModelThread,TRUE,SAVESEARCHTOOLTHREAD_WAITINGTIME);//等待所有模板保存完成
	for (j=0; j<nPatternNum; j++)
	{
		CloseHandle(SearchToolSaveModelThread[j]);    // 关闭所有线程句柄并释放内存
	}
	if(nReturn==WAIT_TIMEOUT)
	{
		//保存超时
		bSucceed = false;
		for (j=0; j<nPatternNum; j++)
		{
			if(SaveSearchToolThreadParam[j].GetIsSaveTimeOut())
			{
				strTemp.Format(_T("Pos%d_Thread%s"),j,strSaveTimeout);
				AfxMessageBox(strTemp);
			}
		}
		
	}
	else
	{
		//开始判断哪些模板没有保存成功
		for (j=0; j<vvpSearchTool.size(); j++)
		{
			for(i = 0; i<vvpSearchTool[j].size(); i++)
			{
				if(SaveSearchToolThreadParam[j].GetSearchToolSaveResult(i)==FALSE)
				{
					strTemp.Format(_T("Pos%d_M%d%s"), j, i, strSaveFail);
					AfxMessageBox(strTemp);
					bSucceed = FALSE;
				}
			}
		}
	}
	return bSucceed;
}

void CDlgSysKindAlignView::OnButtonNewProductFa()
{
	CString strLog;
	strLog.Format(_T("点击[新建]新建大产品\n"));
	g_opLogFile.SaveLogText(strLog,4);

	int nProductIndexFa, nProductIndexChd;
	nProductIndexFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountFa();
	nProductIndexChd = 0;
	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		strLog.Format(_T("点击[新建] 内存不足\n"));
		g_opLogFile.SaveLogText(strLog,4);

		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			CString strWarning;
			strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CREATE_PRODUCT_FAIL_OF_MEM_OUT));
			strWarning = strWarning + 
				_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
				_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
			MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
		
			return ;
			

		}

	}
	NewProduct(nProductIndexFa, nProductIndexChd);
}

void CDlgSysKindAlignView::OnButtonNewProduct()
{
	CString strLog;
	strLog.Format(_T("点击[新建]新建小产品\n"));
	g_opLogFile.SaveLogText(strLog,4);

	int nProductIndexFa, nProductIndexChd;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);
	nProductIndexChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nProductIndexFa);
	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else
		{

			CString strWarning;
			strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CREATE_PRODUCT_FAIL_OF_MEM_OUT));
			strWarning = strWarning + 
				_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
				_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
			MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
			return ;

		}

	}
	NewProduct(nProductIndexFa, nProductIndexChd);
}

void CDlgSysKindAlignView::NewProduct(int nNewProductIndexFa, int nNewProductIndexChd)
{
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	CString strLog;
	strLog.Format(_T("点击[新建]新建产品\n"));

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();
	SysPlatformInfo platformInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	CProductData product(platformInfo);
	pTotalProduct->AddItem(product);
	pTotalProduct->SetItemName(nCount, strName);
	pTotalProduct->SetProductIndex(nNewProductIndexFa, nNewProductIndexChd, nCount);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(nCount);
	m_pParentKind->m_pDlgKindCalibSetting->CreateNewProduct(nNewProductIndexFa, nNewProductIndexChd);

	SetCurProductIndex(nCount, false);

	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveJobCameraInfo(); // 保存随产品的切换曝光增益配置文件
	strLog.Format(_T("新建产品%d：产品大%d,产品小%d\n"),m_nCurProductIdx+1,nNewProductIndexFa+1, nNewProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,5);
	strLog.Format(_T("新建产品完成\n"));
	g_opLogFile.SaveLogText(strLog,4);
	strLog.Format(_T("切换到新建产品%d：大品种%d,小品种%d\n"),m_nCurProductIdx+1,nNewProductIndexFa+1, nNewProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,3);
}


// void CDlgSysKindAlignView::NewProduct(int nNewProductIndexFa, int nNewProductIndexChd)
// {
// 	// 如果前一个产品已修改，提示是否保存前一个产品的设置
// 	CString strLog;
// 	strLog.Format(_T("点击[新建]新建产品\n"));
// 	g_opLogFile.SaveLogText(strLog,4);
// 	int nPrevProductIdx = m_nPrevProductIdx;
// 	if (IsProductChanged())
// 	{
// 		strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT) + "\n";
// 		g_opLogFile.SaveLogText(strLog,5);
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 		if (nMBReturn == IDYES)
// 		{
// 			strLog = _T("弹窗选择：是\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 
// 			if (SavePrevProduct())
// 			{
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		else if (nMBReturn == IDCANCEL)
// 		{
// 			strLog = _T("弹窗选择：取消\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			return;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			strLog = _T("弹窗选择：否\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 
// 			if (m_bProdNameChanged)
// 			{
// 				// 				CString strOldDir, strNewDir;
// 				// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				// 				{
// 				// 					SysPlatformInfo platformInfo;
// 				// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 				// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 				// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 				// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 				// 				}
// 				// 
// 				// 				// 如果文件夹存在，则重命名文件夹
// 				// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// 				// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 				// 				{
// 				// 					if (strCurName != m_strProductNameOld)
// 				// 					{						
// 				// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// 				// 						{
// 				// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
// 				// 						}
// 				// 					}
// 				// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 
// 				int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 				int nPrevProductIdxFa = 0;
// 				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);
// 				strLog.Format(_T("当前产品%d(大品种%d_小品种%d)[产品浏览]"),m_nPrevProductIdx+1,nPrevProductIdxFa+1,nPrevProductIdxCh+1);
// 				g_opLogFile.SaveLogText(strLog,6);
// 				UpdateDlgData(FALSE);
// 
// 				if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 				{
// 					m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 				}
// 
// 				if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 				{
// 					m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 				}
// 
// 				if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 				{	
// 					m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 				}
// 
// 				if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 				{
// 					m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 				}
// 
// 				if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 				{
// 					m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 				}
// 
// 				if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 				{
// 					m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 				}
// 
// 			}
// 
// 			if (m_bProdNewCreate)
// 			{
// // 				CString strDir;
// // 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 					strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 				}
// // 				strDir = m_strModuleDir + strDir;
// // 				if (!RemoveDirectoryPro(strDir))
// // 				{
// // 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// // 				}
// // 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 			}
// 			if (m_bProdNameChanged || m_bProdNewCreate)
// 			{
// 				UpdateProductListDisplay();
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 			}
// 			LoadSearchToolInfo();
// 
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 		}
// 	}
// 
// 
// 	// 释放原来产品的定位工具20170516
// 	{
// 		SysPlatformInfo sysPlatformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 		{
// 			CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 			CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 			if (pPreProduct != NULL)
// 			{
// 				pPreProduct->DeleteObjectSearchTools();
// 				pPreProduct->DeleteTargetSearchTools();
// 				pPreProduct->DeleteVirtualObjectSearchTools();
// 				pPreProduct->DeleteCalibTargetSearchTools();
// 				pPreProduct->DeleteInspectSearchTools();
// 			}
// 		}
// 		// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 		{
// 			CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 			for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 			{
// 				if(i == nNewProductIndexFa) continue;
// 
// 				for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 				{
// 					int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 					CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 					if (pPreProductData != NULL)
// 					{
// 						pPreProductData->DeleteObjectSearchTools();
// 						pPreProductData->DeleteTargetSearchTools();
// 						pPreProductData->DeleteVirtualObjectSearchTools();
// 						pPreProductData->DeleteCalibTargetSearchTools();
// 						pPreProductData->DeleteInspectSearchTools();
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	int nCount = pTotalProduct->GetCount();
// 	CString strName = GetNewDefName();
// 
// 	SysPlatformInfo platformInfo;
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
// 	CProductData product(platformInfo);
// 	pTotalProduct->AddItem(product);
// 	pTotalProduct->SetItemName(nCount, strName);
// 	pTotalProduct->SetProductIndex(nNewProductIndexFa, nNewProductIndexChd, nCount);
// 	pTotalProduct->SetCurProductIndex(nCount);
// 
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(nCount);
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveJobCameraInfo(); // 保存随产品的切换曝光增益配置文件
// 
// 	m_strProductNameOld = strName;
// 	m_strProductNameOldFa = strName;
// 	m_bProdNewCreate = TRUE;
// 
// 	UpdateDlgData(FALSE);
// 
// 	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 	{	
// 		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 	}
// 	UpdateProductListDisplay();
// 
// 	m_pParentKind->m_pDlgKindCalibSetting->CreateNewProduct(nNewProductIndexFa, nNewProductIndexChd);
// 	// 新建产品后，切换至新的产品界面，所有标识符被重新赋值
// 	m_pParentKind->SetProductSearchToolChanged(false);
// 	m_pParentKind->SetCalibObjectSearchToolChanged(false);
// 
// 	strLog.Format(_T("新建产品%d：产品大%d,产品小%d\n"),m_nCurProductIdx+1,nNewProductIndexFa+1, nNewProductIndexChd+1);
// 	g_opLogFile.SaveLogText(strLog,5);
// 	strLog.Format(_T("新建产品完成\n"));
// 	g_opLogFile.SaveLogText(strLog,4);
// 	//strLog.Format(_T("关闭产品%d[产品浏览]\n"),nPrevProductIdx+1);
// 	//g_opLogFile.SaveLogText(strLog,3);
// 	//strLog.Format(_T("打开产品%d[产品浏览]\n"),m_nCurProductIdx+1);
// 	strLog.Format(_T("切换到新建产品%d：大品种%d,小品种%d\n"),m_nCurProductIdx+1,nNewProductIndexFa+1, nNewProductIndexChd+1);
// 	g_opLogFile.SaveLogText(strLog,3);
// }
// 

void CDlgSysKindAlignView::OnBnClickedButtonNewProductMultiple()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CString strLog;
	strLog.Format(_T("点击[批量]按钮\n"));
	g_opLogFile.SaveLogText(strLog,4);

	if (m_nNewProductNum <= 0)
	{
		return;
	}

	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_BATCH_NEW_PRODUCT_OR_NOT)/*_T("是否批量新建产品?")*/, MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			strLog.Format(_T("点击[是]\n"));
			g_opLogFile.SaveLogText(strLog,4);		
		}
		else
		{
			strLog.Format(_T("点击[否]\n"));
			g_opLogFile.SaveLogText(strLog,4);
			return;
		}
	}


	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	if (IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			return;
		}
		else // 不保存对产品的修改
		{
			if (m_bProdNameChanged)
			{
				// 				CString strOldDir, strNewDir;
				// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
				// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
				// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
				// 				{
				// 					SysPlatformInfo platformInfo;
				// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
				// 					pVisionASM->GetSysPlatformInfo(platformInfo);
				// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
				// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
				// 				}
				// 
				// 				// 如果文件夹存在，则重命名文件夹
				// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
				// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
				// 				{
				// 					if (strCurName != m_strProductNameOld)
				// 					{						
				// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
				// 						{
				// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
				// 						}
				// 					}
				// 				}

				int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
				int nPrevProductIdxFa = 0;
				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
				m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);

				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);

				UpdateDlgData(FALSE);
				if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
				{
					m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
				{
					m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignObject != NULL)
				{
					m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
				{
					m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
				{
					m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
				{
					m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
				}
			}
			if (m_bProdNewCreate)
			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
			}
			if (m_bProdNameChanged || m_bProdNewCreate)
			{
				UpdateProductListDisplay();
			}

			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
			{                        
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
			}
			LoadSearchToolInfo();
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();
	// 	if (nCount < 1)
	// 	{

	SysPlatformInfo platformInfo;
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	CProductData product(platformInfo);

	int nIndex = 0;
	for (nIndex = nCount; nIndex < nCount + m_nNewProductNum; nIndex++)
	{
		pTotalProduct->AddItem(product);
		pTotalProduct->SetItemName(nIndex, strName);
		pTotalProduct->SetCurProductIndex(nIndex);
	}
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(TRUE);

	// 	}
	// 	else
	// 	{
	// //		UpdateDlgData(TRUE);
	// 		int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	// 		if (pTotalProduct->CopyItem(nFormIdx, nCount))
	// 		{
	// 			pTotalProduct->SetItemName(nCount, strName);
	// 			pTotalProduct->SetCurProductIndex(nCount);
	// 		}
	// 	}
	m_strProductNameOld = strName;
	m_bProdNewCreate = TRUE;

	UpdateDlgData(FALSE);
	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
	{
		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
	{
		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
	{
		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
	{
		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
	{
		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
	{
		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
	}
	UpdateProductListDisplay();

	m_pParentKind->m_pDlgKindCalibSetting->CreateNewProductMulitple(m_nCurProductIdx, m_nNewProductNum);
}


void CDlgSysKindAlignView::OnBtnCloneProduct() 
{
	// TODO: Add your control notification handler code here
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
	if (IsProductChanged())
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}
		}
		else if (nMBReturn == IDCANCEL)
		{
			return;
		}
		else // 不保存对产品的修改
		{
			if (m_bProdNameChanged)
			{
				// 				CString strOldDir, strNewDir;
				// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
				// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
				// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
				// 				{
				// 					SysPlatformInfo platformInfo;
				// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
				// 					pVisionASM->GetSysPlatformInfo(platformInfo);
				// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
				// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
				// 				}
				// 
				// 				// 如果文件夹存在，则重命名文件夹
				// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
				// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
				// 				{
				// 					if (strCurName != m_strProductNameOld)
				// 					{						
				// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
				// 						{
				// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));	//_T("重命名失败！")
				// 						}
				// 					}
				// 				}

				int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
				int nPrevProductIdxFa = 0;
				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
				m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);

				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
				UpdateDlgData(FALSE);
				if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
				{
					m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
				{
					m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignObject != NULL)
				{
					m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
				{
					m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
				{
					m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
				}
				if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
				{
					m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
				}
			}
			if (m_bProdNewCreate)
			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
			}
			if (m_bProdNameChanged || m_bProdNewCreate)
			{
				UpdateProductListDisplay();
			}

			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
			{                        
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
			}
			LoadSearchToolInfo();
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();
	CString strName = GetNewDefName();
	// 	if (nCount < 1)
	// 	{
	// 		SysPlatformInfo platformInfo;
	// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
	// 		CProductData product(platformInfo);
	// 		pTotalProduct->AddItem(product);
	// 		pTotalProduct->SetItemName(0, strName);
	// 		pTotalProduct->SetCurProductIndex(0);
	// 	}
	// 	else
	// 	{
	//		UpdateDlgData(TRUE);
	int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
	if (pTotalProduct->CopyItem(nFormIdx, nCount))
	{
		pTotalProduct->SetItemName(nCount, strName);
		pTotalProduct->SetCurProductIndex(nCount);
	}
	//	}


	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(nCount);

	m_strProductNameOld = strName;
	m_bProdNewCreate = TRUE;

	// 拷贝模板：保存模板到新产品的模板文件夹下以达到拷贝模板的目的
	int nPrevProdIdxTmp = m_nPrevProductIdx;
	m_nPrevProductIdx = nCount;
	SavePrevProduct();
	m_nPrevProductIdx = nPrevProdIdxTmp;

	UpdateDlgData(FALSE);
	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
	{
		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
	{
		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
	{
		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
	{
		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
	{
		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
	}
	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
	{
		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
	}
	UpdateProductListDisplay();
}

void CDlgSysKindAlignView::OnButtonDeleteCurProduct() 
{
	// 用户删除操作确认
	CString strLog;
	strLog.Format(_T("点击[删除产品]按钮\n"));
	g_opLogFile.SaveLogText(strLog,4);
	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_ROMOVE_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要删除当前产品？")
	{
		strLog.Format(_T("点击[否]按钮\n"));
		g_opLogFile.SaveLogText(strLog,4);
		return;
	}
	strLog.Format(_T("点击[是]按钮\n"));
	g_opLogFile.SaveLogText(strLog,4);

	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();

	SysPlatformInfo platformInfo;

	if (nCount > 0)
	{
		CString strDir;
		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{
			SysPlatformInfo platformInfo;
			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!RemoveDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("删除产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
		}

		for(int i = m_nCurProductIdx+1; i < nCount; i++)
		{
			CString strOldDir , strNewDir;
			strOldDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i+1);
			strOldDir = m_strModuleDir + strOldDir;
			strNewDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i);
			strNewDir = m_strModuleDir + strNewDir;
			RenameDirecotryPro(strOldDir,strNewDir);
		}
	
		if (pTotalProduct->RemoveItem(m_nCurProductIdx))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;

			for(int i = m_nCurProductIdx;i < pTotalProduct->GetCount(); i++)
			{
				pTotalProduct->GetItem(i).m_pAlignerParam->SetProductIndex(i);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(i);
			}

			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
			pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
			UpdateDlgData(FALSE);
			if (m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}			
			if (m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}
			UpdateProductListDisplay();
			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurProductIndex());
		
			m_pParentKind->m_pDlgKindCalibSetting->DeleteCurProduct(m_nCurProductIdx);
		}
		else
		{
			//AfxMessageBox(_T("删除产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
}

void CDlgSysKindAlignView::OnButtonDeleteCurProduct(int nIndex)
{
	CString strLog;
	strLog.Format(_T("点击[删除产品]%d\n"));
	g_opLogFile.SaveLogText(strLog,4);
	// 用户删除操作确认
	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_ROMOVE_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要删除当前产品？")
	{
		strLog.Format(_T("点击[否]\n"));
		g_opLogFile.SaveLogText(strLog,5);
		return;
	}
	strLog.Format(_T("点击[是]\n"));
	g_opLogFile.SaveLogText(strLog,5);
	if(nIndex < 0)
		return ;

	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();

	SysPlatformInfo platformInfo;

	if (nCount > 0)
	{
		CString strDir;
		CString strProductName = pTotalProduct->GetItemName(/*m_nCurProductIdx*/nIndex);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{
			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, /*m_nCurProductIdx*/nIndex+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!RemoveDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("删除产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
		}

		for(int i = /*m_nCurProductIdx*/nIndex+1; i < nCount; i++)
		{
			CString strOldDir , strNewDir;
			strOldDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i+1);
			strOldDir = m_strModuleDir + strOldDir;
			strNewDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, i);
			strNewDir = m_strModuleDir + strNewDir;
			RenameDirecotryPro(strOldDir,strNewDir);
		}

		if (pTotalProduct->RemoveItem(/*m_nCurProductIdx*/nIndex))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;

			for(int i = /*m_nCurProductIdx*/nIndex;i < pTotalProduct->GetCount(); i++)
			{
				pTotalProduct->GetItem(i).m_pAlignerParam->SetProductIndex(i);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(i);
			}
			/*
			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
			pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
			*/
			m_nCurProductIdx = (m_nCurProductIdx > nIndex) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
			pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
			UpdateDlgData(FALSE);
			if (m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindCalibSetting != NULL)
			{
				m_pParentKind->m_pDlgKindCalibSetting -> UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}

			UpdateProductListDisplay();
			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurProductIndex());

			m_pParentKind->m_pDlgKindCalibSetting->DeleteCurProduct(/*m_nCurProductIdx*/nIndex);
		}
		else
		{
			//AfxMessageBox(_T("删除产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
	return ;
}

void CDlgSysKindAlignView::OnBnClickedButtonClearProduct(int nIndex)
{
	CString strLog;
	strLog.Format(_T("点击[清空]产品%d\n"));
	g_opLogFile.SaveLogText(strLog,4);
	// 用户清空操作确认
	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_CLEAR_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要清空当前产品？")
	{
		strLog.Format(_T("点击[否]\n"));
		g_opLogFile.SaveLogText(strLog,5);
		return;
	}
	strLog.Format(_T("点击[是]\n"));
	g_opLogFile.SaveLogText(strLog,5);
	if(nIndex < 0)
		return ;

	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();

	if (nCount > 0)
	{
		CString strDir;
		SysPlatformInfo platformInfo;
		CString strProductName = pTotalProduct->GetItemName(/*m_nCurProductIdx*/nIndex);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{
			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, /*m_nCurProductIdx*/nIndex+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!ClearDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("清空产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT_DATA_FILE));
		}
		CreateDirectoryPro(strDir + _T("\\ObjectModels"));
		CreateDirectoryPro(strDir + _T("\\TargetModels"));

		CString strName = GetNewDefName();
		pTotalProduct->SetItemName(/*m_nCurProductIdx*/nIndex, strName);

		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(/*m_nCurProductIdx*/nIndex);
		UpdateDlgData(FALSE);
		if (pTotalProduct->ClearItem(/*m_nCurProductIdx*/nIndex))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;
			//如果清除产品为当前产品，需要设置为false，否则，不做任何操作
			if(pTotalProduct->GetCurProductIndex() ==nIndex)
			{
				m_pParentKind->SetProductSearchToolChanged(false);
				m_pParentKind->SetCalibObjectSearchToolChanged(false);
			}
			if (m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}

			UpdateDlgTempData(FALSE);

			UpdateProductListDisplay();

			m_pParentKind->m_pDlgKindCalibSetting->ClearCurProduct(/*m_nCurProductIdx*/nIndex);
			if(m_pParentKind->m_pDlgKindCalibSetting != NULL)
			{
				m_pParentKind->m_pDlgKindCalibSetting->UpdateDlgData(FALSE);
			}
		}
		else
		{
			//AfxMessageBox(_T("清空产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
	return ;
}

void CDlgSysKindAlignView::OnBnClickedButtonClearProduct()
{
	CString strLog;
	strLog.Format(_T("点击[清空]按钮\n"));
	g_opLogFile.SaveLogText(strLog,4);

	// 用户清空操作确认
	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_CLEAR_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要清空当前产品？")
	{
		strLog.Format(_T("点击[否]\n"));
		g_opLogFile.SaveLogText(strLog,5);
		return;
	}
	strLog.Format(_T("点击[是]\n"));
	g_opLogFile.SaveLogText(strLog,5);

	UpdateData(TRUE);
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	int nCount = pTotalProduct->GetCount();

	if (nCount > 0)
	{
		CString strDir;
		SysPlatformInfo platformInfo;
		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
		{

			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			pVisionASM->GetSysPlatformInfo(platformInfo);
			//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
			strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
		}
		strDir = m_strModuleDir + strDir;
		if (!ClearDirectoryPro(strDir))
		{
			//AfxMessageBox(_T("清空产品数据文件失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT_DATA_FILE));
		}
		CreateDirectoryPro(strDir + _T("\\ObjectModels"));
		CreateDirectoryPro(strDir + _T("\\TargetModels"));
		
		CString strName = GetNewDefName();
		pTotalProduct->SetItemName(m_nCurProductIdx, strName);

		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
		UpdateDlgData(FALSE);
		if (pTotalProduct->ClearItem(m_nCurProductIdx))
		{
			// 恢复修改标志
			m_bProdPatternChanged = FALSE;
			m_bProdNameChanged = FALSE;
			m_bProdBenchMarkChanged = FALSE;
			m_bProdNewCreate = FALSE;
			m_bCalibInfoChanged = FALSE;

			if (m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if (m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}			

			UpdateDlgTempData(FALSE);

			UpdateProductListDisplay();

			m_pParentKind->m_pDlgKindCalibSetting->ClearCurProduct(m_nCurProductIdx);
			if(m_pParentKind->m_pDlgKindCalibSetting != NULL)
			{
				m_pParentKind->m_pDlgKindCalibSetting->UpdateDlgData(FALSE);
			}
		}
		else
		{
			//AfxMessageBox(_T("清空产品失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_CLEAR_PRODUCT));
		}
	}
	else
	{
		//AfxMessageBox(_T("当前没有任何产品！"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
	}
}

void CDlgSysKindAlignView::OnBtnProductRename()
{
	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
	{
		//AfxMessageBox(_T("请选择当前产品"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
	}
	CString strOldName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

	CDlgProductRename  dlgProductRename(strOldName, this);
	dlgProductRename.m_nParentType = 1;
	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgProductRename.DoModal() == IDOK)
	{
		CString strNewName = dlgProductRename.GetNewName();
		if (strNewName != strOldName)
		{
			// 			CString strOldDir, strNewDir;
			// 			// 			strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strOldName);
			// 			// 			strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strNewName);
			// 			{
			// 				SysPlatformInfo platformInfo;
			// 				vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
			// 				pVisionASM->GetSysPlatformInfo(platformInfo);
			// 				strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strOldName);
			// 				strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strNewName);
			// 			}
			// 			// 如果新命名的文件夹存在，则删除该文件夹
			// 			DWORD dwAttributes = GetFileAttributes(strNewDir);
			// 			if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
			// 			{
			// 				if (!RemoveDirectoryPro(strNewDir))
			// 				{
			// 					//AfxMessageBox(_T("重命名失败！"));
			// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
			// 					return;
			// 				}
			// 			}
			// 			// 如果文件夹存在，则重命名文件夹
			// 			dwAttributes = GetFileAttributes(strOldDir);
			// 			if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
			// 			{
			// 				if (!RenameDirecotryPro(strOldDir, strNewDir))
			// 				{
			// 					//AfxMessageBox(_T("重命名失败！"));
			// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
			// 					return;
			// 				}
			// 			}		

			m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, strNewName);
			//m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
			m_bProdNameChanged = TRUE;
			UpdateDlgData(FALSE);
			if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}

			m_pParentKind->m_pDlgKindCalibSetting->SetCalibProductName(strNewName);


			UpdateProductListDisplay();
		}
	}
	CString strLog;
	strLog.Format(_T("修改[重命名]由%s到%s\n"),strOldName,dlgProductRename.GetNewName());
	g_opLogFile.SaveLogText(strLog,4);
}

void CDlgSysKindAlignView::OnBtnProductRenameFa()
{
	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
	{
		//AfxMessageBox(_T("请选择当前产品"));
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
	}
	int nProductIndexFa, nProductIndexChd;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);
	CString strOldName = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nProductIndexFa);

	CDlgProductRename dlgProductRename(strOldName, this);
	dlgProductRename.m_nParentType = 1;
	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgProductRename.DoModal() == IDOK)
	{
		CString strNewName = dlgProductRename.GetNewName();
		if (strNewName != strOldName)
		{
			m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nProductIndexFa,strNewName);
			m_bProdNameChanged = TRUE;
			UpdateDlgData(FALSE);
			if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
			{
				m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
			{
				m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
			{
				m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
			}
			if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
			{
				m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
			}

			m_pParentKind->m_pDlgKindCalibSetting->SetCalibProductNameFa(strNewName);


			UpdateProductListDisplay();
		}
	}
}

void CDlgSysKindAlignView::OnBtnAmendBenchmark() 
{
	CDlgBenchMarkAmend dlgBenchMarkAmend;
	UpdateData(TRUE);
	dlgBenchMarkAmend.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex));
	dlgBenchMarkAmend.SetSysInfoStringsRef(m_psaSysInfoStrings);
	if (dlgBenchMarkAmend.DoModal() == IDOK)
	{
		// 标记基准修正信息已修改
		m_bProdBenchMarkChanged = TRUE;
	}
}

// 双击Display控件响应
LRESULT CDlgSysKindAlignView::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	if (m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		NewProduct(0,0);
	}
	
	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j =0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
//	int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPatternNum = platformInfo.m_nPositionNum;

	// Target mark check
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		for (i=0; i<nPatternNum; i++)
		{
			for (j=0; j < TARGETCANDIDATENUM; j++)
			{
				if (wParam == m_vvpTargetGuiDisplay[i][j]->GetID())
				{
					m_vpTargetGuiDisplay.at(i)->ClearScreen();
					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);
					if (pSearchTool!=NULL)
					{
						cpImage imagePattern;
						pSearchTool->GetPatternImage(imagePattern);
						m_vpTargetGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
					}
					return S_OK;
				}

				// 				if (wParam == m_vpTargetGuiDisplay[i]->GetID())
				// 				{
				// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
				// 					CDlgSearch dlgSearch(this, pSearchTool);
				// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
				// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
				// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
				// 					if (dlgSearch.DoModal() == IDOK)
				// 					{
				// 						pSearchTool = dlgSearch.GetSearchTool();
				// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnTargetSearchToolParam(i, pSearchTool);
				// 
				// 						// 显示模板训练图像
				// 						if (pSearchTool != NULL)
				// 						{
				// 							scImageGray imagePattern;
				// 							pSearchTool->GetPatternImage(imagePattern);
				// 							m_vpTargetGuiDisplay.at(i)->ClearScreen();
				// 							m_vpTargetGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
				// 						}
				// 						else
				// 						{
				// 							m_vpTargetGuiDisplay.at(i)->ClearScreen();
				// 						}
				// 						// 标记产品模板已修改
				// 						m_bProdPatternChanged = TRUE;
				// 					}
				// 					return S_OK;
				// 				}
			}
		}

	}

	// Virtual Object mark check
	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
// 		for (i=0; i<nPatternNum; i++)
// 		{
// 			if (wParam == m_vpVirtualObjectGuiDisplay.at(i)->GetID())
// 			{
// 				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnVirtualSearchToolParam(i);
// 				CDlgSearch dlgSearch(this, pSearchTool);
// 				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 				if (dlgSearch.DoModal() == IDOK)
// 				{
// 					pSearchTool = dlgSearch.GetSearchTool();
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnVirtualSearchToolParam(i, pSearchTool);
// 
// 					// 显示模板训练图像
// 					if (pSearchTool != NULL)
// 					{
// 						cpImage imagePattern;
// 						pSearchTool->GetPatternImage(imagePattern);
// 						m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 						m_vpVirtualObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 					}
// 					else
// 					{
// 						m_vpVirtualObjectGuiDisplay.at(i)->ClearScreen();
// 					}
// 					// 标记产品模板已修改
// 					m_bProdPatternChanged = TRUE;
// 				}
// 				return S_OK;
// 			}
// 		}
	}

	// Object mark check
	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		for (i=0; i<nPatternNum; i++)
		{
			for (j=0; j < OBJECTCANDIDATENUM; j++)
			{
				if (wParam == m_vvpObjectGuiDisplay[i][j]->GetID())
				{
					m_vpObjectGuiDisplay.at(i)->ClearScreen();
					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nCurProdIdx, i, j);
					if (pSearchTool!=NULL)
					{
						cpImage imagePattern;
						pSearchTool->GetPatternImage(imagePattern);
						m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
					}

					return S_OK;
				}
				// 				if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
				// 				{
				// 					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
				// 					CDlgSearch dlgSearch(this, pSearchTool);
				// 					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
				// 					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
				// 					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
				// 					if (dlgSearch.DoModal() == IDOK)
				// 					{
				// 						pSearchTool = dlgSearch.GetSearchTool();
				// 						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
				// 
				// 						// 显示模板训练图像
				// 						if (pSearchTool != NULL)
				// 						{
				// 							scImageGray imagePattern;
				// 							pSearchTool->GetPatternImage(imagePattern);
				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 							m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
				// 						}
				// 						else
				// 						{
				// 							m_vpObjectGuiDisplay.at(i)->ClearScreen();
				// 						}
				// 						// 标记产品模板已修改
				// 						m_bProdPatternChanged = TRUE;
				// 					}
				// 					return S_OK;
				// 				}
			}
		}
	}
	// 	for (i=0; i<nPatternNum; i++)
	// 	{
	// 		if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
	// 		{
	// 			pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	// 			CDlgSearch dlgSearch(this, pSearchTool);
	// 			dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
	// 			dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
	// 			dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
	// 			if (dlgSearch.DoModal() == IDOK)
	// 			{
	// 				pSearchTool = dlgSearch.GetSearchTool();
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
	// 				
	// 				// 显示模板训练图像
	// 				if (pSearchTool != NULL)
	// 				{
	// 					scImageGray imagePattern;
	// 					pSearchTool->GetPatternImage(imagePattern);
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 					m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 				// 标记产品模板已修改
	// 				m_bProdPatternChanged = TRUE;
	// 			}
	// 			return S_OK;
	// 		}
	// 	}


	return S_OK ;
}

void CDlgSysKindAlignView::SetCurPlatformIndex(int nCurPlatformIndex)
{

	// 保存前一个产品的设置
	m_nPlatformIndex = m_nPrevPlatformIdx;

	BOOL bProductChanged = IsProductChanged();
	BOOL bCalibChanged = m_pParentKind->m_pDlgKindCalibSetting->IsProductChanged();

	if (bProductChanged || bCalibChanged)
	{
		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
		if (nMBReturn == IDYES)
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				ResetProductChangeStatus();
			}
		}
		else // 不保存对产品的修改
		{
			RestoreProductModify();

			if(m_nCurProductIdx != m_nOldProductIdx)
			{
				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
				ASSERT(pTotalProduct != NULL);
				pTotalProduct->SetCurProductIndex(m_nOldProductIdx);
				SetCurProductIndex(m_nOldProductIdx, FALSE, FALSE);	  // FALSE 不在弹框


				// 保存产品概要信息到磁盘
				if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
				{
					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
					m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
				}	

				if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
				{
					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
					m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
				}

				//m_pParentKind->m_pDlgKindCalibSetting->SetCurProductIndex(m_nOldProductIdx);

				//FreePreProductMem(m_nOldProductIdx, m_nCurProductIdx, m_nPrevPlatformIdx);
			}
			//ResetProductChangeStatus();
		}
	}
	else
	{
		if(m_nCurProductIdx != m_nOldProductIdx)
		{
			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")	
			if(nMBReturn == IDNO)
			{

				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
				ASSERT(pTotalProduct != NULL);
				pTotalProduct->SetCurProductIndex(m_nOldProductIdx);

				// 设置切换原来产品
				SetCurProductIndex(m_nOldProductIdx);
			}

		}
		// 保存产品概要信息到磁盘
		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
		}	

		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
		}
	}



	//////////////////////////////////////////////////////////////////////////
	// 修改新选择的平台的显示界面
	// 此段代码在MoveItem中存在， 应该不需要
#if 0
	SysPlatformInfo platformInfo;
	int nPosNum = 0, i = 0, j = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
	//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);


				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	}
	else
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < TARGETCANDIDATENUM; j++)
			{
				m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
		if (platformInfo.m_bBenchMarkAmend)
		{
			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
	}



	// 对象Mark界面修改
	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < OBJECTCANDIDATENUM; j++)
				{
					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
	{
		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
		for (i=0; i<MAXPOSNUM; i++)
		{
			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < OBJECTCANDIDATENUM; j++)
			{
				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
	}

#endif

	m_nPlatformIndex = nCurPlatformIndex;
	// 根据大小品种调整对话框控件
	MoveDlgItem();

	UpdateDlgData(FALSE);
	UpdateProductListDisplay();
	//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SetMutiTarget();
	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_nPrevProductIdx = m_nCurProductIdx;
	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);

	m_nOldProductIdx = m_nCurProductIdx;

	int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
	int nCurProductIdxFa = 0;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
	m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nCurProductIdxFa);
}


void CDlgSysKindAlignView::OnPlatformChange() 
{
	UpdateData(TRUE);
	if (m_nPlatformIndex == m_nPrevPlatformIdx)
	{
		return;
	}

	SetCurPlatformIndex(m_nPlatformIndex);
}


// void CDlgSysKindAlignView::OnPlatformChange() 
// {
// 	UpdateData(TRUE);
// 	if (m_nPlatformIndex == m_nPrevPlatformIdx)
// 	{
// 		return;
// 	}
// 
// 	// 保存前一个产品的设置
// 	if (IsProductChanged())
// 	{
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 		if (nMBReturn == IDYES)
// 		{
// 			if (SavePrevProduct())
// 			{
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		else if (nMBReturn == IDCANCEL)
// 		{
// 			m_nPlatformIndex = m_nPrevPlatformIdx;
// 			UpdateData(FALSE);
// 			return;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			if (m_bProdNameChanged)
// 			{
// 				// 				CString strOldDir, strNewDir;
// 				// 				CString strCurName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// 				// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, m_strProductNameOld);
// 				// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, strCurName);
// 				// 				{
// 				// 					SysPlatformInfo platformInfo;
// 				// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 				// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 				// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 				// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 				// 				}
// 				// 				// 如果文件夹存在，则重命名文件夹
// 				// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// 				// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 				// 				{
// 				// 					if (strCurName != m_strProductNameOld)
// 				// 					{						
// 				// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// 				// 						{
// 				// 							//AfxMessageBox(_T("重命名失败！"));
// 				// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 				// 						}
// 				// 					}
// 				// 				}
// 
// 				int nCurProductIdxCh = 0;//zzc根据m_nCurProductIdx，推算fa和ch的序号
// 				int nCurProductIdxFa = 0;
// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetDlgProductIndex(m_nCurProductIdx, nCurProductIdxFa, nCurProductIdxCh);
// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->SetProductNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 
// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nCurProductIdx);
// 
// 			}
// 			if (m_bProdNewCreate)
// 			{
// // 				CString strDir;
// // 				CString strProductName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// // 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPrevPlatformIdx, strProductName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 					strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 				}
// // 				strDir = m_strModuleDir + strDir;
// // 				if (!RemoveDirectoryPro(strDir))
// // 				{
// // 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 				}
// // 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->RemoveItem(m_nPrevProductIdx);
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->LoadProductDataInfo(m_nCurProductIdx, m_strProductNameOld);           
// 			}
// 
// 			LoadSearchToolInfo();
// 
// 			if(m_nCurProductIdx != m_nOldProductIdx)
// 			{
// 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 				ASSERT(pTotalProduct != NULL);
// 				pTotalProduct->SetCurProductIndex(m_nOldProductIdx);
// 				m_pParentKind->m_pDlgKindCalibSetting->SetCurProductIndex(m_nOldProductIdx);
// 
// 				/////////////////释放空间/////////////////////
// 				{
// 					SysPlatformInfo sysPlatformInfo;
// 					m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetSysPlatformInfo(sysPlatformInfo);
// 					if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 					{
// 						CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 						CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nCurProductIdx);
// 						if (pPreProduct != NULL)
// 						{
// 							pPreProduct->DeleteObjectSearchTools();
// 							pPreProduct->DeleteTargetSearchTools();
// 							pPreProduct->DeleteVirtualObjectSearchTools();
// 							pPreProduct->DeleteCalibTargetSearchTools();
// 							pPreProduct->DeleteInspectSearchTools();
// 						}
// 					}
// 					// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 					else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 					{
// 						CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 
// 						int nOldProductIndexFa = 0;
// 						int nOldProductIndexChd = 0;
// 						pTotalProduct->GetDlgProductIndex(m_nOldProductIdx, nOldProductIndexFa, nOldProductIndexChd);
// 
// 						for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 						{
// 							if(i == nOldProductIndexFa) continue;
// 
// 							for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 							{
// 								int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 								CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 								if (pPreProductData != NULL)
// 								{
// 									pPreProductData->DeleteObjectSearchTools();
// 									pPreProductData->DeleteTargetSearchTools();
// 									pPreProductData->DeleteVirtualObjectSearchTools();
// 									pPreProductData->DeleteCalibTargetSearchTools();
// 									pPreProductData->DeleteInspectSearchTools();
// 								}
// 							}
// 						}
// 					}
// 				}	
// 				/////////////////释放空间/////////////////////
// 			}
// 
// 
// 
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 		}
// 	}
// 	else
// 	{
// 		if(m_nCurProductIdx != m_nOldProductIdx)
// 		{
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")	
// 			if(nMBReturn == IDNO)
// 			{
// 
// 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 				ASSERT(pTotalProduct != NULL);
// 				pTotalProduct->SetCurProductIndex(m_nOldProductIdx);
// 				m_pParentKind->m_pDlgKindCalibSetting->SetCurProductIndex(m_nOldProductIdx);
// 
// 				/////////////////释放空间/////////////////////
// 				{
// 					SysPlatformInfo sysPlatformInfo;
// 					m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetSysPlatformInfo(sysPlatformInfo);
// 					if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 					{
// 						CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 						CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nCurProductIdx);
// 						if (pPreProduct != NULL)
// 						{
// 							pPreProduct->DeleteObjectSearchTools();
// 							pPreProduct->DeleteTargetSearchTools();
// 							pPreProduct->DeleteVirtualObjectSearchTools();
// 							pPreProduct->DeleteCalibTargetSearchTools();
// 							pPreProduct->DeleteInspectSearchTools();
// 						}
// 					}
// 					// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 					else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 					{
// 						CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPrevPlatformIdx);
// 
// 						int nOldProductIndexFa = 0;
// 						int nOldProductIndexChd = 0;
// 						pTotalProduct->GetDlgProductIndex(m_nOldProductIdx, nOldProductIndexFa, nOldProductIndexChd);
// 
// 						for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 						{
// 							if(i == nOldProductIndexFa) continue;
// 
// 							for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 							{
// 								int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 								CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 								if (pPreProductData != NULL)
// 								{
// 									pPreProductData->DeleteObjectSearchTools();
// 									pPreProductData->DeleteTargetSearchTools();
// 									pPreProductData->DeleteVirtualObjectSearchTools();
// 									pPreProductData->DeleteCalibTargetSearchTools();
// 									pPreProductData->DeleteInspectSearchTools();
// 								}
// 							}
// 						}
// 					}
// 				}	
// 				/////////////////释放空间/////////////////////
// 
// 			}
// 
// 		}
// 		// 保存产品概要信息到磁盘
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 		}	
// 	}
// 
// 	UpdateDlgByAuthorize();
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 修改新选择的平台的显示界面
// 	SysPlatformInfo platformInfo;
// 	int nPosNum = 0, i = 0, j = 0;
// 	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
// 	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
// //	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	nPosNum = platformInfo.m_nPositionNum;
// 	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
// 	{
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_SHOW);
// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 
// 
// 				for(j = 0; j < TARGETCANDIDATENUM; j++)
// 				{
// 					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 
// 				for(j = 0; j < TARGETCANDIDATENUM; j++)
// 				{
// 					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
// 	}
// 	else
// 	{
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			//GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			for(j = 0; j < TARGETCANDIDATENUM; j++)
// 			{
// 				m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 			}
// 		}
// 		if (platformInfo.m_bBenchMarkAmend)
// 		{
// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// // 	// 虚拟对象Mark界面修改
// // 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// // 	{
// // 		//GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
// // 		for (i=0; i<MAXPOSNUM; i++)
// // 		{
// // 			if (i < nPosNum)
// // 			{
// // 				//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// // 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// // 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// // 			}
// // 			else
// // 			{
// // 				//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// // 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// // 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// // 			}
// // 		}
// // 	}
// // 	else
// // 	{
// // 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
// // 		for (i=0; i<MAXPOSNUM; i++)
// // 		{
// // 			//GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// // 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// // 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// // 		}
// // 	}
// 
// 	// 对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_SHOW);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_SHOW);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_SHOW);
// 				}
// 			}
// 			else
// 			{
// 				//GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 				if (i < MAXPOSNUM -1)
// 				{
// 					GetDlgItem(IDC_STATIC_SEPARATOR4+ i)->ShowWindow(SW_HIDE);
// 				}
// 				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
// 				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);
// 
// 				for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 				{
// 					m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		//GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<MAXPOSNUM; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 
// 			for(j = 0; j < OBJECTCANDIDATENUM; j++)
// 			{
// 				m_vvpObjectGuiDisplay[i][j]->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	// 	GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 	// 	for (i=0; i<6; i++)
// 	// 	{
// 	// 		if (i < nPosNum)
// 	// 		{
// 	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 	// 		}
// 	// 		else
// 	// 		{
// 	// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 	// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 	// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 	// 		}
// 	// 	}
// 
// 	// 根据大小品种调整对话框控件
// 	MoveDlgItem();
// 
// 	UpdateDlgData(FALSE);
// 	UpdateProductListDisplay();
// 	//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SetMutiTarget();
// 	m_nPrevPlatformIdx = m_nPlatformIndex;
// 	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
// 	m_nPrevProductIdx = m_nCurProductIdx;
// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 
// 	m_nOldProductIdx = m_nCurProductIdx;
// 
// 	int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 	int nCurProductIdxFa = 0;
// 	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
// 	m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nCurProductIdxFa);
// 
// }

void CDlgSysKindAlignView::OnSetfocusEditAlignerMaxTimes() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_MAX_TIMES)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerMaxTimes();
	}	
}


void CDlgSysKindAlignView::OnSetfocusEditNewProductNum() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_NEW_PRODUCT_NUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_NEW_PRODUCT_NUM, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditNewProductNum();
	}	
}


void CDlgSysKindAlignView::OnSetfocusEditAlignerXPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXPrecision();
	}	
}

void CDlgSysKindAlignView::OnSetfocusEditAlignerYPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYPrecision();
	}	
}

void CDlgSysKindAlignView::OnSetfocusEditAlignerDPrecision() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_PRECISION)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDPrecision();
	}	
}

void CDlgSysKindAlignView::OnSetfocusEditAlignerXOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerXOffset();
	}	
}

void CDlgSysKindAlignView::OnSetfocusEditAlignerYOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerYOffset();
	}	
}

void CDlgSysKindAlignView::OnSetfocusEditAlignerDOffset() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strOutput);
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditAlignerDOffset();
	}	
}


void CDlgSysKindAlignView::OnKillfocusEditAlignerMaxTimes() 
{
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nAlignerMaxTimes = (m_nAlignerMaxTimes<1) ? 1 : ((m_nAlignerMaxTimes>10) ? 10 : m_nAlignerMaxTimes);
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignView::OnKillfocusEditNewProductNum() 
{
	if (m_bKillFocusEnabled)
	{
		UpdateData(TRUE);
		m_nNewProductNum = (m_nNewProductNum<1) ? 1 : ((m_nNewProductNum>200) ? 200 : m_nNewProductNum);
		UpdateData(FALSE);
	}
}


void CDlgSysKindAlignView::OnKillfocusEditAlignerXPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
		}
		else
		{
			m_strAlignerPrecisionX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
		}
	}	
}

void CDlgSysKindAlignView::OnKillfocusEditAlignerYPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
		}
		else
		{
			m_strAlignerPrecisionY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
		}
	}	
}

void CDlgSysKindAlignView::OnKillfocusEditAlignerDPrecision() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
		}
		else
		{
			m_strAlignerPrecisionD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
		}
	}	
}

void CDlgSysKindAlignView::OnKillfocusEditAlignerXOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
		}
		else
		{
			m_strAlignerOffsetX.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
		}
	}	
}

void CDlgSysKindAlignView::OnKillfocusEditAlignerYOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
		}
		else
		{
			m_strAlignerOffsetY.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
		}
	}	
}

void CDlgSysKindAlignView::OnKillfocusEditAlignerDOffset() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
		}
		else
		{
			m_strAlignerOffsetD.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
		}
	}	
}

/************************************************************************************
/* Function name	: OnClickListProduct
/* Description	    : 变更当前产品的消息响应，包括保存前一个当前产品的所有设置和加载
/*					  显示新当前产品的信息，包括模板信息。注：模板信息保存直接写到磁
/*					  盘，点产品管理“取消”时不能取消对模板信息的修改。
/* Return type		: void 
/************************************************************************************/
void CDlgSysKindAlignView::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
{

	CString strLog;
	int nProductIndexFa = 0;
	int nProductIndexChd = 0;

	int nPreProductIndexFa= 0;
	int nPreProductIndexChd = 0;

	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);

	nPreProductIndexFa = nProductIndexFa;
	nPreProductIndexChd = nProductIndexChd;

	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);

	if (nSelected < 0)
	{
		SetProductListSelect(nProductIndexFa, nProductIndexChd);
		return;
	}

	int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,nSelected);
	if (CheckMemUseRateWarning(nDstProductIndex))
	{
		SetProductListSelect(nPreProductIndexFa, nPreProductIndexChd);
		return;
	}

	if ((nDstProductIndex >= 0) && (nDstProductIndex != m_nCurProductIdx))
	{
		strLog.Format(_T("点击大品种%d小品种列表：小品种%d切换到小品种%d\n"),nProductIndexFa+1,nProductIndexChd+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,5);
		SetCurProductIndex(nDstProductIndex);
	}

	*pResult = 0;
}


void CDlgSysKindAlignView::OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString strLog;
	int nProductIndexFa = 0;
	int nProductIndexChd = 0;

	int nPreProductIndexFa = 0;
	int nPreProductIndexChd = 0;

	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);

	nPreProductIndexFa = nProductIndexFa;
	nPreProductIndexChd = nProductIndexChd;

	POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);

	if (nSelected < 0)
	{
		SetProductListSelect(nProductIndexFa, nProductIndexChd);
		return;
	}

	if(nSelected == nProductIndexFa)
	{
		return;
	}

	nProductIndexChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetLastChdIndex(nSelected);
	int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSelected, nProductIndexChd);
	if (CheckMemUseRateWarning(nDstProductIndex))
	{
		SetProductListSelect(nPreProductIndexFa, nPreProductIndexChd);
		return;
	}	 

	if ((nDstProductIndex >= 0) && (nDstProductIndex != m_nCurProductIdx))
	{
		strLog.Format(_T("点击大品种列表：大品种%d切换到大品种%d\n"),nProductIndexFa+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,5);
		SetCurProductIndex(nDstProductIndex);
	}

}


// void CDlgSysKindAlignView::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	
// 	int nProductIndexFa, nProductIndexChd;
// 	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);
// 
// 	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
// 	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
// 
// 	if (nSelected>=0)
// 	{
// 		if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
// 			int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
// 			int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,nSelected);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 
// 			}
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 
// 
// 			}
// 			else
// 			{
// 				if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
// 				{
// 					CString strWarning;
// 					strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANGE_PRODUCT_FAIL_OF_MEM_OUT));
// 					strWarning = strWarning + 
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
// 					MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
// 					m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 					return ;
// 				}
// 
// 			}
// 
// 		}
// 	}
// 		
// 	if (nSelected < 0)
// 	{
// 		m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 	}
// 	else
// 	{
// 		m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa, nSelected);
// 	}
// 
// 	CString strLog;
// 
// 	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
// 	{
// 		strLog.Format(_T("点击大品种%d小品种列表：小品种%d切换到小品种%d\n"),nProductIndexFa+1,nProductIndexChd+1,nSelected+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		strLog.Format(_T("产品序号%d切换到产品序号%d\n"),m_nPrevProductIdx+1,m_nCurProductIdx+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		// jjh 1211
// 		CString strProductChange;
// 		strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_CHANGING);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
// 
// 		//CString strLog;
// 		//strLog.Format(_T("关闭产品%d[产品浏览]\n"),m_nPrevProductIdx+1);
// 		//g_opLogFile.SaveLogText(strLog,3);
// 		//strLog.Format(_T("打开产品%d[产品浏览]\n"),m_nCurProductIdx+1);
// 		//g_opLogFile.SaveLogText(strLog,3);
// 
// 		// 保存前一个产品的设置
// 		if (IsProductChanged())
// 		{
// 			strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT) + "\n";
// 			g_opLogFile.SaveLogText(strLog,5);
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 			if (nMBReturn == IDYES)
// 			{
// 				strLog = _T("弹窗选择：是\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				if (SavePrevProduct())
// 				{
// 					// 恢复修改标志
// 					m_bProdPatternChanged = FALSE;
// 					m_bProdNameChanged = FALSE;
// 					m_bProdBenchMarkChanged = FALSE;
// 					m_bProdNewCreate = FALSE;
// 					m_bCalibInfoChanged = FALSE;
// 				}
// 			}
// 			else if (nMBReturn == IDCANCEL)
// 			{
// 				strLog = _T("弹窗选择：取消\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
// 				m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 				m_nCurProductIdx = m_nPrevProductIdx;
// 				return;
// 			}
// 			else // 不保存对产品的修改
// 			{
// 				strLog = _T("弹窗选择：否\n");
// 				g_opLogFile.SaveLogText(strLog,5);
// 				if (m_bProdNameChanged)
// 				{
// 					// 					CString strOldDir, strNewDir;
// 					// 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					// 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 					// 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 					// 					{
// 					// 						SysPlatformInfo platformInfo;
// 					// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 					// 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					// 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 					// 					}
// 					// 					// 如果文件夹存在，则重命名文件夹
// 					// 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// 					// 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 					// 					{
// 					// 						if (strCurName != m_strProductNameOld)
// 					// 						{						
// 					// 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// 					// 							{
// 					// 								//AfxMessageBox(_T("重命名失败！"));
// 					// 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 					// 							}
// 					// 						}
// 					// 					}
// 
// 					m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 					//					UpdateDlgData(FALSE);
// 				}
// 				if (m_bProdNewCreate)
// 				{
// // 					CString strDir;
// // 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 					//strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%03d"), m_nPlatformIndex, m_nPrevProductIdx+1);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 						strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 					}
// // 					strDir = m_strModuleDir + strDir;
// // 					if (!RemoveDirectoryPro(strDir))
// // 					{
// // 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 					}
// // 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 				}
// 				if (m_bProdNewCreate || m_bProdNameChanged)
// 				{
// 					UpdateProductListDisplay();
// 				}
// 
// 				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 				{                        
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 				}
// 				LoadSearchToolInfo();
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		ClipMouseCursor(TRUE);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetProductCameraShutter(m_nCurProductIdx); 
// 		// 释放原来产品的定位工具20170516
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 				CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 				if (pPreProduct != NULL)
// 				{
// 					pPreProduct->DeleteObjectSearchTools();
// 					pPreProduct->DeleteTargetSearchTools();
// 					pPreProduct->DeleteVirtualObjectSearchTools();
// 					pPreProduct->DeleteCalibTargetSearchTools();
// 					pPreProduct->DeleteInspectSearchTools();
// 				}
// 			}
// 		}
// 
// 		m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
// 		m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 
// 		int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 		int nCurProductIdxFa = 0;
// 		m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
// 
// 		strLog.Format(_T("当前产品%d：大品种%d,小品种%d\n"),m_nCurProductIdx+1,nCurProductIdxFa+1, nCurProductIdxCh+1);
// 		g_opLogFile.SaveLogText(strLog,5);
// 
// 		m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nCurProductIdxFa);
// 
// //		m_nPrevProductIdx = m_nCurProductIdx;
// 
// 		UpdateDlgData(FALSE);
// 
// 		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 		}
// 		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 		}
// 
// 		m_listctrlProduct.SetItemState(nSelected, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 
// 		ClipMouseCursor(FALSE);
//         m_pParentKind->m_pDlgKindCalibSetting->SetCurProductIndex(m_nCurProductIdx);
// 
// 		// jjh 1211
// 		strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_CHANGE_SUCCEED);
// 		GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
// 		// 切换产品，界面更新，所有标识符被重新赋值
// 		m_pParentKind->SetProductSearchToolChanged(false);
// 		m_pParentKind->SetCalibObjectSearchToolChanged(false);
//     }
// 
// 
// 	*pResult = 0;
// }

// void CDlgSysKindAlignView::OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	CString strLog;
// 	int nProductIndexFa, nProductIndexChd;
// 	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);
// 
// 	POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
// 	int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);
// 
// 	if (nSelected >= 0)
// 	{
// 		if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
// 			int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);
// 			// 读取当前大品种的上一次选中的小品种
// 			nProductIndexChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetLastChdIndex(nSelected);
// 
// 			// 将界面上的索引转换成内部索引，并更新为当前索引
// 			int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSelected,nProductIndexChd);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 
// 			}
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 
// 
// 			}
// 			else
// 			{
// 				if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
// 				{
// 					CString strWarning;
// 					strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANGE_PRODUCT_FAIL_OF_MEM_OUT));
// 					strWarning = strWarning + 
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
// 						_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
// 					MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
// 					m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 					return ;
// 				}
// 
// 			}
// 
// 		}
// 	}
// 	
// 	if (nSelected < 0)
// 	{
// 		m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 		return;
// 	}
// 
// 	if(nSelected == nProductIndexFa)
// 	{
// 		return;
// 	}
// 
// 	// jjh 1211
// 	
// 	CString strProductChange;
// 	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_CHANGING);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
// 	strLog.Format(_T("点击大品种列表：大品种%d切换到大品种%d\n"),nProductIndexFa+1,nSelected+1);
// 	g_opLogFile.SaveLogText(strLog,5);
// 	if (IsProductChanged())
// 	{
// 		strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT) + "\n";
// 		g_opLogFile.SaveLogText(strLog,5);
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 		if (nMBReturn == IDYES)
// 		{
// 			strLog = _T("弹窗选择：是\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			if (SavePrevProduct())
// 			{
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 		else if (nMBReturn == IDCANCEL)
// 		{
// 			strLog = _T("弹窗选择：取消\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
// 			m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 			m_nCurProductIdx = m_nPrevProductIdx;
// 			return;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			strLog = _T("弹窗选择：否\n");
// 			g_opLogFile.SaveLogText(strLog,5);
// 			if (m_bProdNameChanged)
// 			{
// 				// 					CString strOldDir, strNewDir;
// 				// 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 				// 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				// 					{
// 				// 						SysPlatformInfo platformInfo;
// 				// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 				// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 				// 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 				// 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 				// 					}
// 				// 					// 如果文件夹存在，则重命名文件夹
// 				// 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// 				// 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 				// 					{
// 				// 						if (strCurName != m_strProductNameOld)
// 				// 						{						
// 				// 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// 				// 							{
// 				// 								//AfxMessageBox(_T("重命名失败！"));
// 				// 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 				// 							}
// 				// 						}
// 				// 					}
// 
// 				int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 				int nPrevProductIdxFa = 0;
// 				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);
// 
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 				//					UpdateDlgData(FALSE);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// // 					CString strDir;
// // 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// // 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 					//strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%03d"), m_nPlatformIndex, m_nPrevProductIdx+1);
// // 					{
// // 						SysPlatformInfo platformInfo;
// // 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 						pVisionASM->GetSysPlatformInfo(platformInfo);
// // 						//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 						strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// // 					}
// // 					strDir = m_strModuleDir + strDir;
// // 					if (!RemoveDirectoryPro(strDir))
// // 					{
// // 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 					}
// // 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 			}
// 			if (m_bProdNewCreate || m_bProdNameChanged)
// 			{
// 				UpdateProductListDisplay();
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 			}
// 			LoadSearchToolInfo();
// 
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 		}
// 	}
// 
// 	ClipMouseCursor(TRUE);//锁定鼠标
// 
// 	// 释放原来产品的定位工具20170516
// 	{
// 		SysPlatformInfo sysPlatformInfo;
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 		{
// 			CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 			CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 			if (pPreProduct != NULL)
// 			{
// 				pPreProduct->DeleteObjectSearchTools();
// 				pPreProduct->DeleteTargetSearchTools();
// 				pPreProduct->DeleteVirtualObjectSearchTools();
// 				pPreProduct->DeleteCalibTargetSearchTools();
// 				pPreProduct->DeleteInspectSearchTools();
// 			}
// 		}
// 		// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 		{
// 			CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 			for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 			{
// 				if(i == nSelected) continue;
// 
// 				for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 				{
// 					int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 					CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 					if (pPreProductData != NULL)
// 					{
// 						pPreProductData->DeleteObjectSearchTools();
// 						pPreProductData->DeleteTargetSearchTools();
// 						pPreProductData->DeleteVirtualObjectSearchTools();
// 						pPreProductData->DeleteCalibTargetSearchTools();
// 						pPreProductData->DeleteInspectSearchTools();
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	// 保存记录
// 	m_vpTotalProductData.at(m_nPlatformIndex)->UpdateLastChdIndex(nProductIndexFa, nProductIndexChd);
// 
// 	// 读取当前大品种的上一次选中的小品种
// 	nProductIndexChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetLastChdIndex(nSelected);
// 
// 	// 将界面上的索引转换成内部索引，并更新为当前索引
// 	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSelected,nProductIndexChd);
// 
// 	strLog.Format(_T("当前产品%d：大品种%d,小品种%d\n"),m_nCurProductIdx+1,nSelected+1, nProductIndexChd+1);
// 	g_opLogFile.SaveLogText(strLog,5);
// 
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetProductCameraShutter(m_nCurProductIdx); 
// 
// 	// 设置当前索引
// 	m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
// 
// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 	m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nSelected);	//zzc大产品名称
// 
// 	UpdateDlgData(FALSE);
// 
// 	if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 	}
// 	if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 	{
// 		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 	}
// 
// 	//m_listctrlProductFa.SetItemState(nSelected, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 	UpdateProductListDisplay();
// 
// 	ClipMouseCursor(FALSE);//接触鼠标锁定
// 
// 	m_pParentKind->m_pDlgKindCalibSetting->SetCurProductIndex(m_nCurProductIdx);
// 
// 	// jjh 1211
// 	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_CHANGE_SUCCEED);
// 	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
// 	// 切换产品，界面更新，所有标识符被重新赋值
// 	m_pParentKind->SetProductSearchToolChanged(false);
// 	m_pParentKind->SetCalibObjectSearchToolChanged(false);
// }

void CDlgSysKindAlignView::SetCurProductIndex(int nCurProdcutIndex, bool bToastInfo/*=true*/, bool bSaveProduct/*=true*/)
{

	CString strLog;
	int nCurProductIndexFa, nCurProductIndexChd;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIndexFa, nCurProductIndexChd);

	if (nCurProdcutIndex < 0)
	{
		SetProductListSelect(nCurProductIndexFa, nCurProductIndexChd);
	}
	else
	{
		m_nCurProductIdx = nCurProdcutIndex;
		m_pParentKind->m_pDlgKindCalibSetting->m_nCurProductIdx = nCurProdcutIndex;
	}


	int nCurProductIndex = nCurProdcutIndex;
	int nPreProductIndex = m_nPrevProductIdx;

	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
	{

		if (bToastInfo)
		{
			CString strProductChange;
			strProductChange = GetProductChangeTipText(nCurProductIndex, nPreProductIndex, true);
			GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
			m_pParentKind->m_pDlgKindCalibSetting->SetProductChangeTipInfoStart(strProductChange);
		}


		// 保存前一个产品的设置
		BOOL bProductChanged = IsProductChanged();
		BOOL bCalibChanged = m_pParentKind->m_pDlgKindCalibSetting->IsProductChanged();

		if ((bProductChanged || bCalibChanged) && bSaveProduct )
		{
			strLog = _T("弹窗提示：")  +  GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT) + "\n";
			g_opLogFile.SaveLogText(strLog,5);
			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
			if (nMBReturn == IDYES)
			{
				strLog = _T("弹窗选择：是\n");
				g_opLogFile.SaveLogText(strLog,5);
				if (SavePrevProduct())
				{
					// 恢复修改标志
					ResetProductChangeStatus();
				}
			}
			else // 不保存对产品的修改
			{
				strLog = _T("弹窗选择：否\n");
				g_opLogFile.SaveLogText(strLog,5);
				RestoreProductModify();
			}
		}

		ClipMouseCursor(TRUE);//鼠标锁定

		// 释放上一个产品内存
		FreePreProductMem(m_nCurProductIdx, m_nPrevProductIdx, m_nPlatformIndex);


		m_vpTotalProductData.at(m_nPlatformIndex)->UpdateLastChdIndex(nCurProductIndexFa, nCurProductIndexChd);

		m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
		m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
		int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
		int nCurProductIdxFa = 0;

		m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
		m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nCurProductIdxFa);
		m_vpTotalProductData.at(m_nPlatformIndex)->UpdateLastChdIndex(nCurProductIdxFa, nCurProductIdxCh);

		//		m_nPrevProductIdx = m_nCurProductIdx;

		m_pParentKind->m_pDlgKindCalibSetting->SetCurProductInfo();

		UpdateDlgData(FALSE);

		m_pParentKind->m_pDlgKindCalibSetting->UpdateDlgData(FALSE);

		// ShiYue 20190228
		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
		{
			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
		}

		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
		{
			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
		}


		UpdateProductListDisplay();
		m_pParentKind->m_pDlgKindCalibSetting->UpdateProductListDisplay();

		if (bToastInfo)
		{
			CString  strProductChange = GetProductChangeTipText(nCurProductIndex, nPreProductIndex, false);
			GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
			m_pParentKind->m_pDlgKindCalibSetting->SetProductChangeTipInfoEnd(strProductChange);
		}


		// 切换产品，界面更新，所有标识符被重新赋值
		m_pParentKind->SetProductSearchToolChanged(false);
		m_pParentKind->SetCalibObjectSearchToolChanged(false);
	}


	ClipMouseCursor(FALSE);//接触鼠标锁定
}


BOOL CDlgSysKindAlignView::CheckMemUseRateWarning(int nDstProductIndex)
{
	if (nDstProductIndex <= 0)
	{
		return FALSE;
	}

	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);

		// 将界面上的索引转换成内部索引，并更新为当前索引
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			return FALSE;
		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{

			return FALSE;
		}
		else
		{
			if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHANGE_PRODUCT_FAIL_OF_MEM_OUT));
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return TRUE;
			}

		}
	}

	return FALSE;
}

void CDlgSysKindAlignView::SetProductListSelect(int nProductFa, int nProductChd)
{
	// 设置产品浏览列表选择
	m_listctrlProduct.SetItemState(nProductChd, LVIS_SELECTED, LVIS_SELECTED);
	m_listctrlProductFa.SetItemState(nProductFa, LVIS_SELECTED, LVIS_SELECTED);

	// 设置标定产品列表List选择 保持同步
	m_pParentKind->m_pDlgKindCalibSetting->SetProductListSelect(nProductFa, nProductChd);
}


void CDlgSysKindAlignView::ResetProductChangeStatus()
{
	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;

	m_pParentKind->m_pDlgKindCalibSetting->ResetProductChangeStatus();

	m_pParentKind->m_pDlgKindAlignSetting->m_bPhotoLightCtrlInfoChange = FALSE;
}

void CDlgSysKindAlignView::RestoreProductModify()
{
	// 恢复标定	(标定内容少)
	m_pParentKind->m_pDlgKindCalibSetting->RestoreProductModify();

	if (m_bProdNameChanged)
	{
		int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
		int nPrevProductIdxFa = 0;
		m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
		m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);
		m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);	
	}
	if (m_bProdNewCreate)
	{

	}
	if (m_bProdNewCreate || m_bProdNameChanged)
	{
		UpdateProductListDisplay();
	}

	if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	{                        
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	}
	LoadSearchToolInfo();
	// 恢复修改标志
	ResetProductChangeStatus();

}


void CDlgSysKindAlignView::FreePreProductMem(int nCurProductIndex, int nPreProductIndex, int nPlatformIndex)
{
	int nCurProductIndexFa = 0;
	int nCurProductIndexChd = 0;
	int nPreProductIndexFa = 0;
	int nPreProductIndexChd = 0;
	SysPlatformInfo sysPlatformInfo;

	// 获取产品索引对应的大小品种序号
	m_vpTotalProductData.at(nPlatformIndex)->GetDlgProductIndex(nCurProductIndex, nCurProductIndexFa, nCurProductIndexChd);
	m_vpTotalProductData.at(nPlatformIndex)->GetDlgProductIndex(nPreProductIndex, nPreProductIndexFa, nPreProductIndexChd);

	// 获取平台信息
	m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);


	if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
	{
		// 产品数据
		CTotalProductData *pTotalProduct = NULL;
		pTotalProduct = m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetTotalProuductData();
		CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
		if (pPreProduct != NULL)
		{
			pPreProduct->DeleteObjectSearchTools();
			pPreProduct->DeleteTargetSearchTools();
			pPreProduct->DeleteVirtualObjectSearchTools();
			pPreProduct->DeleteCalibTargetSearchTools();
			pPreProduct->DeleteInspectSearchTools();
		}
	}
	// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
	else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
	{
		//根据当前产品序号，获得当前应该修改的大产品序号
		// 如果上一个大品种和当前大品种不一致，则说明切换的是大品种 需要释放其他大品种内存
		if(nPreProductIndexFa != nCurProductIndexFa) 
		{
			CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(nPlatformIndex)->GetTotalProuductData();
			for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
			{
				if(i == nCurProductIndexFa) continue;

				for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
				{
					int nProductIndex = pTotalProduct->GetProductIndex(i, j);
					CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
					if (pPreProductData != NULL)
					{
						pPreProductData->DeleteObjectSearchTools();
						pPreProductData->DeleteTargetSearchTools();
						pPreProductData->DeleteVirtualObjectSearchTools();
						pPreProductData->DeleteCalibTargetSearchTools();
						pPreProductData->DeleteInspectSearchTools();
					}
				}
			}
		}
	}


	m_pParentKind->m_pDlgKindCalibSetting->FreePreProductMem(nCurProductIndex, nPreProductIndex, nPlatformIndex);
}

CString CDlgSysKindAlignView::GetProductChangeTipText(int nCurProIndex, int nPreProIndex, bool bStart)
{
	int nCurProductIndexFa = 0;
	int nCurProductIndexChd = 0;
	int nPreProductIndexFa = 0;
	int nPreProductIndexChd = 0;

	// 获取产品索引对应的大小品种序号
	m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(nCurProIndex, nCurProductIndexFa, nCurProductIndexChd);
	m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(nPreProIndex, nPreProductIndexFa, nPreProductIndexChd);

	DWORD dwStrID = 0;
	if (nCurProductIndexFa != nPreProductIndexFa)
	{
		dwStrID = bStart ? IDS_STR_DAD_PRODUCNT_CHANGING : IDS_STR_DAD_PRODUCNT_CHANGE_SUCCEED;
	}
	else
	{
		dwStrID = bStart ? IDS_STR_CHILD_PRODUCNT_CHANGING : IDS_STR_CHILD_PRODUCNT_CHANGE_SUCCEED;
	}

	return GetSysInfoString(m_psaSysInfoStrings, dwStrID);
}



// void CDlgSysKindAlignView::SetCurProductIndex(int nCurProdcutIndex)
// {
// 	int nCurProductIndexFa, nCurProductIndexChd;
// 	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIndexFa, nCurProductIndexChd);
// 
// 	if (nCurProdcutIndex < 0)
// 	{
// 		m_listctrlProduct.SetItemState(nCurProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 		m_listctrlProductFa.SetItemState(nCurProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 	}
// 	else
// 	{
// 		m_nCurProductIdx = nCurProdcutIndex;
// 	}
// 	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
// 	{
// 		// 保存前一个产品的设置
// 		if (IsProductChanged())
// 		{
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 			if (nMBReturn == IDYES)
// 			{
// 				if (SavePrevProduct())
// 				{
// 					// 恢复修改标志
// 					m_bProdPatternChanged = FALSE;
// 					m_bProdNameChanged = FALSE;
// 					m_bProdBenchMarkChanged = FALSE;
// 					m_bProdNewCreate = FALSE;
// 					m_bCalibInfoChanged = FALSE;
// 				}
// 			}
// 			else if (nMBReturn == IDCANCEL)
// 			{
// 				m_listctrlProduct.SetItemState(nCurProductIndexChd, 0x0000, LVIS_SELECTED);
// 				m_listctrlProductFa.SetItemState(nCurProductIndexFa, 0x0000, LVIS_SELECTED);
// 
// 				int nPrevProductIndexFa, nPrevProductIndexChd;
// 				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx,nPrevProductIndexFa, nPrevProductIndexChd);
// 				m_listctrlProduct.SetItemState(nPrevProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
// 				m_listctrlProductFa.SetItemState(nPrevProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
// 
// 				m_nCurProductIdx = m_nPrevProductIdx;
// 				return;
// 			}
// 			else // 不保存对产品的修改
// 			{
// 				if (m_bProdNameChanged)
// 				{
// 					// 					CString strOldDir, strNewDir;
// 					// 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					// 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 					// 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 					// 					{
// 					// 						SysPlatformInfo platformInfo;
// 					// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 					// 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 					// 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 					// 					}
// 					// 					// 如果文件夹存在，则重命名文件夹
// 					// 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// 					// 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 					// 					{
// 					// 						if (strCurName != m_strProductNameOld)
// 					// 						{						
// 					// 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// 					// 							{
// 					// 								//AfxMessageBox(_T("重命名失败！"));
// 					// 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 					// 							}
// 					// 						}
// 					// 					}
// 
// 					int nPrevProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 					int nPrevProductIdxFa = 0;
// 					m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(m_nPrevProductIdx, nPrevProductIdxFa, nPrevProductIdxCh);
// 					m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nPrevProductIdxFa, m_strProductNameOldFa);
// 
// 					m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 					//					UpdateDlgData(FALSE);
// 				}
// 				if (m_bProdNewCreate)
// 				{
// 					// 					CString strDir;
// 					// 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					// 					//strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 					// 					//strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%03d"), m_nPlatformIndex, m_nPrevProductIdx+1);
// 					// 					{
// 					// 						SysPlatformInfo platformInfo;
// 					// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 					// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 					// 						//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 					// 						strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nPrevProductIdx+1);
// 					// 					}
// 					// 					strDir = m_strModuleDir + strDir;
// 					// 					if (!RemoveDirectoryPro(strDir))
// 					// 					{
// 					// 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// 					// 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 					// 					}
// 					// 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
// 				}
// 				if (m_bProdNewCreate || m_bProdNameChanged)
// 				{
// 					UpdateProductListDisplay();
// 				}
// 
// 				if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 				{                        
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 				}
// 				LoadSearchToolInfo();
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 
// 
// 		// ShiYue 20190228
// 		// 释放原来产品的定位工具20170516
// // 		{
// // 			SysPlatformInfo sysPlatformInfo;
// // 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// // 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// // 			{
// // 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// // 				CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// // 				if (pPreProduct != NULL)
// // 				{
// // 					if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// // 					}
// // 					if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// // 					}
// // 					if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// // 					}
// // 					if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// // 					}
// // 					if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// // 					}
// // 					if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// // 					{
// // 						m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// // 					}
// // 				}
// // 			}
// // 		}
// 
// 		// ShiYue 20190228
// 		{
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 				CTotalProductData *pTotalProduct = NULL;
// 				pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
// 				CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);
// 				if (pPreProduct != NULL)
// 				{
// 					pPreProduct->DeleteObjectSearchTools();
// 					pPreProduct->DeleteTargetSearchTools();
// 					pPreProduct->DeleteVirtualObjectSearchTools();
// 					pPreProduct->DeleteCalibTargetSearchTools();
// 					pPreProduct->DeleteInspectSearchTools();
// 				}
// 			}
// 			// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 				int nTempCurProductIndexFa, nTempCurProductIndexChd;
// 				//根据当前产品序号，获得当前应该修改的大产品序号
// 				m_vpTotalProductData.at(m_nPlatformIndex)->GetDlgProductIndex(nCurProdcutIndex,nTempCurProductIndexFa,nTempCurProductIndexChd);
// 				if(nCurProductIndexFa != nTempCurProductIndexFa)
// 				{
// 					CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
// 					for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 					{
// 						if(i == nTempCurProductIndexFa) continue;
// 
// 						for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 						{
// 							int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 							CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 							if (pPreProductData != NULL)
// 							{
// 								pPreProductData->DeleteObjectSearchTools();
// 								pPreProductData->DeleteTargetSearchTools();
// 								pPreProductData->DeleteVirtualObjectSearchTools();
// 								pPreProductData->DeleteCalibTargetSearchTools();
// 								pPreProductData->DeleteInspectSearchTools();
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
// 		m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 		int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 		int nCurProductIdxFa = 0;
// 		m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
// 		m_strProductNameOldFa = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductNameFa(nCurProductIdxFa);
// //		m_nPrevProductIdx = m_nCurProductIdx;
// 
// 		UpdateDlgData(FALSE);
// 
// 		// ShiYue 20190228
// 		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// 		}
// 
// 		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// 		}
// 
// 		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// 		}
// 
// 		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// 		}
// 
// 		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// 		}
// 
// 		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
// 		{
// 			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 		}
// 
// // 		m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
// // 		m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
// // 		m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
// // 		m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
// // 		m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
// // 		m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
// 
// //		m_listctrlProduct.SetItemState(m_nCurProductIdx, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 		UpdateProductListDisplay();
// 	}
// 
// }


void CDlgSysKindAlignView::OnRClickListProduct(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nProductIndexFa, nProductIndexChd;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa,nProductIndexChd);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if( index == -1 )
	{
		m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
		return;
	}
	
	HMENU hMenu = ::CreatePopupMenu();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	switch(lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	}
	
	int nIndex = -1;
	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu( hMenu, TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );//弹出菜单

	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);

	if(nSelected >= 0)
	{
		nIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,nSelected);
	}
	else
	{
		m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
		DestroyMenu(hMenu);

		return;
	}
	CString strLog;
	strLog.Format(_T("右键大品种%d小品种列表的小品种%d\n"),nProductIndexFa+1,nSelected+1);
	g_opLogFile.SaveLogText(strLog,5);

	strLog.Format(_T("当前产品%d：大品种%d,小品种%d\n"),m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,nProductIndexChd)+1,nProductIndexFa+1, nProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,5);

	switch( Cmd )//响应点击的菜单
	{
	case 10001:
		strLog.Format(_T("右键-新建\n"));
		g_opLogFile.SaveLogText(strLog,6);
		OnButtonNewProduct();
		break;
	case 10002:
		strLog.Format(_T("右键-删除-大品种%d小品种列表的小品种%d\n"),nProductIndexFa+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,6);
		OnButtonDeleteCurProduct(nIndex);
		break;
	case 10003:
		strLog.Format(_T("右键-清空-大品种%d小品种列表的小品种%d\n"),nProductIndexFa+1,nSelected+1);
		g_opLogFile.SaveLogText(strLog,6);
		OnBnClickedButtonClearProduct(nIndex);
		break;
	default:
		break;
	}

	m_listctrlProduct.SetItemState(nProductIndexChd, LVIS_SELECTED, LVIS_SELECTED);
	DestroyMenu(hMenu);
	
	return;
}

void CDlgSysKindAlignView::OnRClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nProductIndexFa, nProductIndexChd;
	m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if( index == -1 )
	{
		m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
		return;
	}

	HMENU hMenu = ::CreatePopupMenu();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	switch(lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
//		AppendMenu( hMenu, MF_STRING, 10001, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_NEW_PRODUCT) );
//		AppendMenu( hMenu, MF_STRING, 10002, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_DELETE_PRODUCT) );
		AppendMenu( hMenu, MF_STRING, 10003, GetSysInfoString(m_psaSysInfoStrings,IDS_BUTTON_CLEAR_PRODUCT) );
		break;
	}

	int nIndex = -1;
	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu( hMenu, TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );//弹出菜单

	POSITION posSel = m_listctrlProductFa.GetFirstSelectedItemPosition();
	int nSelected = m_listctrlProductFa.GetNextSelectedItem(posSel);

	if(nSelected < 0)
	{
		m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
		DestroyMenu(hMenu);

		return;
	}
	CString strLog;
	strLog.Format(_T("右键大品种列表的大品种%d\n"),nSelected+1);
	g_opLogFile.SaveLogText(strLog,5);
	strLog.Format(_T("当前产品%d：大品种%d,小品种%d\n"),m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,nProductIndexChd)+1,nProductIndexFa+1, nProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,5);

	switch( Cmd )//响应点击的菜单
	{
	case 10001:
		strLog.Format(_T("右键-新建\n"));
		g_opLogFile.SaveLogText(strLog,6);
		OnButtonNewProductFa();
		break;
	case 10002:
		{
			strLog.Format(_T("右键-删除-大品种%d\n"),nProductIndexFa+1);
			g_opLogFile.SaveLogText(strLog,6);
			int nProductCountChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nProductIndexFa);
			for(int i=0; i<nProductCountChd; i++)
			{
				nIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nProductIndexFa,i);
				OnButtonDeleteCurProduct(nIndex);
			}
		}
		break;
	case 10003:
		{
			strLog.Format(_T("右键-清空-大品种%d\n"),nSelected+1);
			g_opLogFile.SaveLogText(strLog,6);
			int nProductCountChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nSelected);
			for(int i=0; i<nProductCountChd; i++)
			{
				nIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSelected,i);
				OnBnClickedButtonClearProduct(nIndex);
			}

			// 清空产品界面的大品种名称
			CString strNewName = _T("------");
			m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nProductIndexFa, strNewName);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(FALSE);
			UpdateProductListDisplay();

			// 清空标定界面的大品种名称
			m_pParentKind->m_pDlgKindCalibSetting->m_vpTotalCalibData.at(m_nPlatformIndex)->SetCalibNameFa(nProductIndexFa, strNewName);
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalCalibDataInfo(FALSE);
			m_pParentKind->m_pDlgKindCalibSetting->UpdateProductListDisplay();
		}
		break;
	default:
		break;
	}

	m_listctrlProductFa.SetItemState(nProductIndexFa, LVIS_SELECTED, LVIS_SELECTED);
	DestroyMenu(hMenu);
	
	return ;
}


BOOL CDlgSysKindAlignView::DestroyWindow() 
{
	int i = 0, j = 0;
	int nSize = m_vpTargetGuiDisplay.size();
	for (i=0; i<nSize; i++)
	{
		delete m_vpTargetGuiDisplay.at(i);
		m_vpTargetGuiDisplay.at(i) = NULL;
	}
	m_vpTargetGuiDisplay.clear();

	nSize = m_vpObjectGuiDisplay.size();
	for (i=0; i<nSize; i++)
	{
		delete m_vpObjectGuiDisplay.at(i);
		m_vpObjectGuiDisplay.at(i) = NULL;
	}
	m_vpObjectGuiDisplay.clear();

	nSize = m_vpVirtualObjectGuiDisplay.size();
	for (i=0; i<nSize; i++)
	{
		delete m_vpVirtualObjectGuiDisplay.at(i);
		m_vpVirtualObjectGuiDisplay.at(i) = NULL;
	}
	m_vpVirtualObjectGuiDisplay.clear();

	for(i = 0; i < m_vvpTargetGuiDisplay.size(); i++)
	{
		for(j = 0; j < m_vvpTargetGuiDisplay[i].size(); j++)
		{
			delete m_vvpTargetGuiDisplay[i][j];
			m_vvpTargetGuiDisplay[i][j] = NULL;
		}
		m_vvpTargetGuiDisplay[i].clear();
	}
	m_vvpTargetGuiDisplay.clear();

	for(i = 0; i < m_vvpObjectGuiDisplay.size(); i++)
	{
		for(j = 0; j < m_vvpObjectGuiDisplay[i].size(); j++)
		{
			delete m_vvpObjectGuiDisplay[i][j];
			m_vvpObjectGuiDisplay[i][j] = NULL;
		}
		m_vvpObjectGuiDisplay[i].clear();
	}
	m_vvpObjectGuiDisplay.clear();


	// 	if (m_pStatusBar != NULL)
	// 	{
	// 		delete m_pStatusBar;
	// 		m_pStatusBar = NULL;
	// 	}
	// 	if (m_hIconSucceed != NULL)
	// 	{
	// 		DestroyIcon(m_hIconSucceed);
	// 		m_hIconSucceed = NULL;
	// 	}
	// 	if (m_hIconFailed != NULL)
	// 	{
	// 		DestroyIcon(m_hIconFailed);
	// 		m_hIconFailed = NULL;
	// 	}
	m_fontChangeProduct.DeleteObject();
	return CDialog::DestroyWindow();
}


// BOOL CDlgSysKindAlignView::OnMyOK()
// {
// 	if (IsProductChanged())
// 	{
// 		if (SavePrevProduct())
// 		{
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 			m_pParentKind->SetProductSearchToolChanged(FALSE);
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("产品保存失败！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_SAVE_PRODUCT));
// 			return FALSE;
// 		}	
// 	}
// 	else
// 	{
// 		// 保存产品概要信息到磁盘
// 		//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
// 		{
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
// 			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 		}
// 
// 	}
// 	//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SetMutiTarget();
// 	return TRUE;
// }

BOOL CDlgSysKindAlignView::OnMyOK()
{
	bool bAlignProductChanged = IsProductChanged();
	bool bCalibProductChanged = m_pParentKind->m_pDlgKindCalibSetting->IsProductChanged();
	if (bAlignProductChanged || bCalibProductChanged)
	{
		if (SavePrevProduct())
		{
			// 恢复修改标志
			ResetProductChangeStatus();
			m_pParentKind->SetProductSearchToolChanged(FALSE);
		}
		else
		{
			//AfxMessageBox(_T("产品保存失败！"));
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_SAVE_PRODUCT));
			return FALSE;
		}	
	}
	else
	{
		// 保存产品概要信息到磁盘
		//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
		}

		if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
		{
			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
			m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
		}

	}
	//m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SetMutiTarget();
	return TRUE;
}

BOOL CDlgSysKindAlignView::CancelProductChange()
{
	if (m_bProdNameChanged)
	{
		// 				CString strOldDir, strNewDir;
		// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
		// 
		// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
		// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
		// 				{
		// 					SysPlatformInfo platformInfo;
		// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
		// 					pVisionASM->GetSysPlatformInfo(platformInfo);
		// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
		// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
		// 				}
		// 				// 如果文件夹存在，则重命名文件夹
		// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
		// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
		// 				{
		// 					if (strCurName != m_strProductNameOld)
		// 					{						
		// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
		// 						{
		// 							//AfxMessageBox(_T("重命名失败！"));
		// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
		// 						}
		// 					}
		// 				}

		int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
		int nCurProductIdxFa = 0;
		m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
		m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nCurProductIdxFa, m_strProductNameOldFa);

		m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);

	}
	if (m_bProdNewCreate)
	{
// 		CString strDir;
// 		CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 		//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 		{
// 			SysPlatformInfo platformInfo;
// 			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 			pVisionASM->GetSysPlatformInfo(platformInfo);
// 			//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 			strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
// 		}
// 		strDir = m_strModuleDir + strDir;
// 		if (!RemoveDirectoryPro(strDir))
// 		{
// 			//AfxMessageBox(_T("删除产品数据文件失败！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 		}
// 		m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
	}

	if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	{                        
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	}

	// 恢复修改标志
	m_bProdPatternChanged = FALSE;
	m_bProdNameChanged = FALSE;
	m_bProdBenchMarkChanged = FALSE;
	m_bProdNewCreate = FALSE;
	m_bCalibInfoChanged = FALSE;

	m_nCurProductIdx = m_nOldProductIdx;
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
	return TRUE;
}

BOOL CDlgSysKindAlignView::OnMyCancel()
{
	// 如果当前产品已修改，提示是否保存当前产品的设置

	if (m_nOldProductIdx != m_nCurProductIdx)
	{
		SetCurProductIndex(m_nOldProductIdx, false);
	}

	// 保存产品概要信息到磁盘
	if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE))
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
	}	

	if (!m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE))
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INCALID_CALI_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保标定名称正确！")*/);
		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalCalibDataInfo(FALSE);
	}

	return TRUE;
}

// BOOL CDlgSysKindAlignView::OnMyCancel()
// {
// 	// 如果当前产品已修改，提示是否保存当前产品的设置
// 	if (IsProductChanged())
// 	{
// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNO);	//_T("是否保存对当前产品的修改？")
// 		if (nMBReturn == IDYES)
// 		{
// 			if (SavePrevProduct())
// 			{
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 				m_pParentKind->SetProductSearchToolChanged(FALSE);
// 			}
// 		}
// 		else if (nMBReturn == IDCANCEL)
// 		{
// 			return FALSE;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			if (m_bProdNameChanged)
// 			{
// 				// 				CString strOldDir, strNewDir;
// 				// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 				// 
// 				// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 				// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 				// 				{
// 				// 					SysPlatformInfo platformInfo;
// 				// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 				// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 				// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 				// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 				// 				}
// 				// 				// 如果文件夹存在，则重命名文件夹
// 				// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
// 				// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 				// 				{
// 				// 					if (strCurName != m_strProductNameOld)
// 				// 					{						
// 				// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
// 				// 						{
// 				// 							//AfxMessageBox(_T("重命名失败！"));
// 				// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 				// 						}
// 				// 					}
// 				// 				}
// 
// 				int nCurProductIdxCh = 0;//zzc根据m_nPrevProductIdx，推算fa和ch的序号
// 				int nCurProductIdxFa = 0;
// 				m_vpTotalProductData.at(m_nPlatformIndex)->GetCurDlgProductIndex(nCurProductIdxFa, nCurProductIdxCh);
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetProductNameFa(nCurProductIdxFa, m_strProductNameOldFa);
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
// 
// 			}
// 			if (m_bProdNewCreate)
// 			{
// // 				CString strDir;
// // 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// // 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// // 				{
// // 					SysPlatformInfo platformInfo;
// // 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// // 					pVisionASM->GetSysPlatformInfo(platformInfo);
// // 					//strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// // 					strDir.Format(_T("%s\\Jobs\\%03d"), platformInfo.m_strPlatformDetailSavePath, m_nCurProductIdx+1);
// // 				}
// // 				strDir = m_strModuleDir + strDir;
// // 				if (!RemoveDirectoryPro(strDir))
// // 				{
// // 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// // 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// // 				}
// // 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
// 			}
// 
// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
// 			{                        
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
// 			}
// 			LoadSearchToolInfo();
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 		}
// 	}
// 	// 释放原来产品的定位工具20181204-mfq
// 	{
//         if (m_nCurProductIdx != m_nOldProductIdx)
//         {
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
// 			if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
// 			{
// 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 				CProductData *pPreProduct = pTotalProduct->GetItemPtr(m_nCurProductIdx);
// 				if (pPreProduct != NULL)
// 				{
// 					pPreProduct->DeleteObjectSearchTools();
// 					pPreProduct->DeleteTargetSearchTools();
// 					pPreProduct->DeleteVirtualObjectSearchTools();
// 					pPreProduct->DeleteCalibTargetSearchTools();
// 					pPreProduct->DeleteInspectSearchTools();
// 				}
// 			}
// 			// 启用大小品种且只加载单个大产品时，释放其他全部大品种的全部小品种
// 			else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
// 			{
// 				CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 
// 				int nOldProductIndexFa = 0;
// 				int nOldProductIndexChd = 0;
// 				pTotalProduct->GetDlgProductIndex(m_nOldProductIdx, nOldProductIndexFa, nOldProductIndexChd);
// 
// 				for (int i=0; i<pTotalProduct->GetProductCountFa(); i++ )
// 				{
// 					if(i == nOldProductIndexFa) continue;
// 
// 					for (int j =0; j< pTotalProduct->GetProductCountChd(i); j++)
// 					{
// 						int nProductIndex = pTotalProduct->GetProductIndex(i, j);
// 						CProductData *pPreProductData  = pTotalProduct->GetItemPtr(nProductIndex);
// 						if (pPreProductData != NULL)
// 						{
// 							pPreProductData->DeleteObjectSearchTools();
// 							pPreProductData->DeleteTargetSearchTools();
// 							pPreProductData->DeleteVirtualObjectSearchTools();
// 							pPreProductData->DeleteCalibTargetSearchTools();
// 							pPreProductData->DeleteInspectSearchTools();
// 						}
// 					}
// 				}
// 			}
//         }
// 	}
// 	m_nCurProductIdx = m_nOldProductIdx;
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	ASSERT(pTotalProduct != NULL);
// 	pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
// 	return TRUE;
// }

void CDlgSysKindAlignView::OnOK() 
{
	if (!OnMyOK())
	{
		return;
	}
	// 	if (IsProductChanged())
	// 	{
	// 		if (SavePrevProduct())
	// 		{
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 		else
	// 		{
	// 			//AfxMessageBox(_T("产品保存失败！"));
	// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_SAVE_PRODUCT));
	// 			return;
	// 		}	
	// 	}
	// 	else
	// 	{
	// 		// 保存产品概要信息到磁盘
	// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
	// 	}

	CDialog::OnOK();
}

void CDlgSysKindAlignView::OnCancel() 
{
	if (!OnMyCancel())
	{
		return;
	}
	// 	// 如果当前产品已修改，提示是否保存当前产品的设置
	// 	if (IsProductChanged())
	// 	{
	// 		int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
	// 		if (nMBReturn == IDYES)
	// 		{
	// 			if (SavePrevProduct())
	// 			{
	// 				// 恢复修改标志
	// 				m_bProdPatternChanged = FALSE;
	// 				m_bProdNameChanged = FALSE;
	// 				m_bProdBenchMarkChanged = FALSE;
	// 				m_bProdNewCreate = FALSE;
	// 				m_bCalibInfoChanged = FALSE;
	// 			}
	// 		}
	// 		else if (nMBReturn == IDCANCEL)
	// 		{
	// 			return;
	// 		}
	// 		else // 不保存对产品的修改
	// 		{
	// 			if (m_bProdNameChanged)
	// 			{
	// 				CString strOldDir, strNewDir;
	// 				CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 
	// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
	// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
	// 					strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
	// 				}
	// 				// 如果文件夹存在，则重命名文件夹
	// 				DWORD dwAttributes = GetFileAttributes(strNewDir);
	// 				if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
	// 				{
	// 					if (strCurName != m_strProductNameOld)
	// 					{						
	// 						if (!RenameDirecotryPro(strNewDir, strOldDir))
	// 						{
	// 							//AfxMessageBox(_T("重命名失败！"));
	// 							AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
	// 						}
	// 					}
	// 				}
	// 
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, m_strProductNameOld);
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
	// 			}
	// 			if (m_bProdNewCreate)
	// 			{
	// 				CString strDir;
	// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
	// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
	// 				{
	// 					SysPlatformInfo platformInfo;
	// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
	// 					pVisionASM->GetSysPlatformInfo(platformInfo);
	// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
	// 				}
	// 				strDir = m_strModuleDir + strDir;
	// 				if (!RemoveDirectoryPro(strDir))
	// 				{
	// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
	// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
	// 				}
	// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nCurProductIdx);
	// 			}
	// 
	// 			if (m_bCalibInfoChanged) //	Mali 2013.9.24 恢复当前产品的标定后信息
	// 			{                        
	// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->LoadProductDataInfo(m_nPrevProductIdx, m_strProductNameOld);           
	// 			}
	// 
	// 			// 恢复修改标志
	// 			m_bProdPatternChanged = FALSE;
	// 			m_bProdNameChanged = FALSE;
	// 			m_bProdBenchMarkChanged = FALSE;
	// 			m_bProdNewCreate = FALSE;
	// 			m_bCalibInfoChanged = FALSE;
	// 		}
	// 	}

	CDialog::OnCancel();
}

void CDlgSysKindAlignView::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(this->m_hWnd, WM_HOTKEY_CTL_P);
	CDialog::OnClose();
}


void CDlgSysKindAlignView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}

//拷贝产品标定数据
void CDlgSysKindAlignView::CopyProductCalibData(int nSrcProductIndex,int nDstProductIndex)
{
	if (m_pParentKind == NULL || m_pParentKind->m_pDlgKindCalibSetting == NULL)
	{
		return;
	}
	CDlgSysKindCalibSetting* pDlgKindCalibSetting=m_pParentKind->m_pDlgKindCalibSetting;
	//pDlgKindCalibSetting->SetComboCalibCopySrcIndex(nSrcProductIndex);
	//pDlgKindCalibSetting->SetComboCalibCopyDstIndex(nDstProductIndex);
	//pDlgKindCalibSetting->OnBnClickedBtnCopyCalibData();
	pDlgKindCalibSetting->CopyCalibData(nSrcProductIndex, nDstProductIndex);
	CopyCalibXMLDataFile(nSrcProductIndex,nDstProductIndex);
}

void CDlgSysKindAlignView::OnBnClickedBtnCopyProductDataFa()
{

	int nSrcFaProductIndex = m_comboProductSrcFa.GetCurSel();
	int nDstFaProductIndex = m_comboProductDstFa.GetCurSel();
	
	CString strLog; 
	strLog.Format(_T("点击大品种拷贝：大品种%d拷贝到大品种%d\n"),nSrcFaProductIndex+1,nDstFaProductIndex+1);
	g_opLogFile.SaveLogText(strLog,5);

	//源小品种数量和目标小品种数量
	int nSrcProductNumChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nSrcFaProductIndex);
	int nDstProductNumChd = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nDstFaProductIndex);

	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		int nSrcProductIndexChild = m_vpTotalProductData.at(m_nPlatformIndex)->GetLastChdIndex(nSrcFaProductIndex);
		int nSrcProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSrcFaProductIndex,nSrcProductIndexChild);//获取源大产品上一个小产品序号
		int nDstProductIndexChild = m_vpTotalProductData.at(m_nPlatformIndex)->GetLastChdIndex(nDstFaProductIndex);
		int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nDstFaProductIndex,nDstProductIndexChild);//获取目标大产品第一个小产品序号
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nSrcProductIndex)
				|| !m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_PRODUCT_FAIL_OF_MEM_OUT));
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return ;
			}

		}

	}

	if(1)
	{
		if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
		{
			if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
			{
				double dUsedRate = 0.0;
				CString StrTmp = m_strModuleDir;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);
				if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp))
				{

					CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
					CString strTmp2;
					strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
					CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

					GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！产品拷贝失败"));

					CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！产品拷贝失败"));

					// 拷贝
					return;

				}
			}
		}
	}
	// jjh
	ClipMouseCursor(TRUE);
	CString strProductChange;
	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_COPYING);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);	

	ClipMouseCursor(FALSE);
	if(nSrcProductNumChd > nDstProductNumChd)//目标小品种不够，先补齐不够的小产品
	{
		for(int i=0; i<nSrcProductNumChd - nDstProductNumChd ;i++)
		{
			NewProduct(nDstFaProductIndex,nDstProductNumChd + i);
		}
	}
	else if(nSrcProductNumChd < nDstProductNumChd)//目标小品种多，则清空多余的
	{
		for(int i=nSrcProductNumChd;i<nDstProductNumChd;i++)
		{
			int nIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nDstFaProductIndex,i);
			OnBnClickedButtonClearProduct(nIndex);
		}
	}
	ClipMouseCursor(TRUE);
	for(int i=0; i<nSrcProductNumChd; i++)
	{
		int nSrcProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSrcFaProductIndex,i);
		int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nDstFaProductIndex,i);
		CopyProductData(nSrcProductIndex, nDstProductIndex);
	}

	// jjh 1211
	//Sleep(10000);
	strProductChange =  GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DAD_PRODUCNT_COPY_SUCCEED);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);	
	ClipMouseCursor(FALSE);
}

void CDlgSysKindAlignView::OnBnClickedBtnCopyProductData()
{
	// TODO: 在此添加控件通知处理程序代码
	

	int nSrcProductIndexChd = m_comboProductSrc.GetCurSel();
	int nSrcProductIndexFa = m_comboProductSrcFa.GetCurSel();
	int nSrcProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nSrcProductIndexFa, nSrcProductIndexChd);

	int nDstProductIndexChd = m_comboProductDst.GetCurSel();
	int nDstProductIndexFa = m_comboProductDstFa.GetCurSel();
	int nDstProductIndex = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductIndex(nDstProductIndexFa, nDstProductIndexChd);

	CString strLog; 
	strLog.Format(_T("点击小品种拷贝：大品种%d小品种%d拷贝到大品种%d小品种%d\n"),nSrcProductIndexChd+1,nSrcProductIndexFa+1,nDstProductIndexFa+1,nDstProductIndexChd+1);
	g_opLogFile.SaveLogText(strLog,5);

	if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{

		}
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{


		}
		else
		{
			if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nSrcProductIndex)
				|| !m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->ProductSearchToolLoadState(nDstProductIndex))
			{
				CString strWarning;
				strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_PRODUCT_FAIL_OF_MEM_OUT));
				strWarning = strWarning + 
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
					_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
				MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
				return ;
			}
			
		}
		
	}

	if(1)
	{
		if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
		{
			if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
			{
				double dUsedRate = 0.0;
				CString StrTmp = m_strModuleDir;
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);
				if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp))
				{

					CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
					CString strTmp2;
					strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
					CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

					GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！产品拷贝失败"));

					CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n模板未保存到磁盘！产品拷贝失败"));

					// 拷贝
					return;

				}
			}
		}
	}
	// jjh 1211
	ClipMouseCursor(TRUE);
	CString strProductChange;
	strProductChange = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_COPYING);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);

	CopyProductData(nSrcProductIndex, nDstProductIndex);

	// jjh 1211
	strProductChange =  GetSysInfoString(m_psaSysInfoStrings,IDS_STR_CHILD_PRODUCNT_COPY_SUCCEED);
	GetDlgItem(IDC_STATICPRODUCTCHANGE)->SetWindowText(strProductChange);
	ClipMouseCursor(FALSE);
}

void CDlgSysKindAlignView::CopyProductData(int nSrcProductIndex, int nDstProductIndex)
{
	if (nDstProductIndex < 0 || nSrcProductIndex < 0)
	{
		return;
	}

	if (nDstProductIndex == nSrcProductIndex)
	{
		return;
	}

	if (nSrcProductIndex == m_nCurProductIdx)
	{
		if (IsProductChanged())
		{
			if (SavePrevProduct())
			{
				// 恢复修改标志
				m_bProdPatternChanged = FALSE;
				m_bProdNameChanged = FALSE;
				m_bProdBenchMarkChanged = FALSE;
				m_bProdNewCreate = FALSE;
				m_bCalibInfoChanged = FALSE;
			}	
		}
	}

	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);

	CProductData *pSrcProduct = pTotalProduct->GetItemPtr(nSrcProductIndex);
	if (pSrcProduct == NULL)
	{
		return;
	}	

// 	if (nSrcProductIndex != m_nCurProductIdx)
// 	{
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnTargetSearchToolRepository(nSrcProductIndex);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnObjectSearchToolRepository(nSrcProductIndex);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnVirtualObjectSearchToolRepository(nSrcProductIndex);
// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnCalibTargetSearchToolRepository(nSrcProductIndex);
// 	}

	CProductData *pDstProduct = pTotalProduct->GetItemPtr(nDstProductIndex);
	if (pDstProduct != NULL)
	{
		pDstProduct->DeleteObjectSearchTools();
		pDstProduct->DeleteTargetSearchTools();
		pDstProduct->DeleteVirtualObjectSearchTools();
		pDstProduct->DeleteCalibTargetSearchTools();
		pDstProduct->DeleteInspectSearchTools();
	}

	CString strSrcProductName;
	strSrcProductName = pTotalProduct->GetItemName(nDstProductIndex);
	if (pTotalProduct->CopyItem(nSrcProductIndex, nDstProductIndex))
	{
		pTotalProduct->SetItemName(nDstProductIndex, strSrcProductName);
	}
	// m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(TRUE);
	if (!m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(TRUE))
	{
		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_INVALID_NAME_SAVE_DATA_FAILE)/*_T("保存数据失败,请确保产品名称正确！")*/);
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveTotalProductDataInfo(TRUE);
	}


	pDstProduct = pTotalProduct->GetItemPtr(nDstProductIndex);
	if (pDstProduct != NULL)
	{
		pDstProduct->ResetObjectSearchTools();
		pDstProduct->ResetTargetSearchTools();
		pDstProduct->ResetVirtualObjectSearchTools();
		pDstProduct->ResetCalibTargetSearchTools();
		pDstProduct->ResetInspectSearchTools();
	}

	int nSrcProductIndexFa = 0;
	int nSrcProductIndexChd = 0;
	int nDstProductIndexFa = 0;
	int nDstProductIndexChd = 0;
	int nCurProductIndexFa = 0;
	int nCurProductIndexChd = 0;
	pTotalProduct->GetDlgProductIndex(nSrcProductIndex, nSrcProductIndexFa, nSrcProductIndexChd);
	pTotalProduct->GetDlgProductIndex(nDstProductIndex, nDstProductIndexFa, nDstProductIndexChd);
	pTotalProduct->GetDlgProductIndex(m_nCurProductIdx, nCurProductIndexFa, nCurProductIndexChd);


	// 1. 目标Mark模板工具设置
	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnTargetSearchToolRepository(nSrcProductIndex);
	}
	
	int i = 0, j = 0;
	if (pSrcProduct->m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
	{
		CBaseSearchTool* pTargetSearchTool = NULL;
		for (i=0; i<pSrcProduct->m_pAlignerParam->GetTargetMarkPatternNum(); i++)
		{
			for(j = 0; j<TARGETCANDIDATENUM; j++)
			{
				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nSrcProductIndex, i, j);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnTargetSearchTool(nDstProductIndex, i, j, pTargetSearchTool);
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveAlnTargetSearchToolRepository(nDstProductIndex);
		pDstProduct->m_bAlnTargetSearchToolInited = TRUE;
	}
	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteTargetSearchTools();
			}
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteTargetSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteTargetSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteTargetSearchTools();
			}
		}
	}


	// 2. 对象Mark模板工具设置
	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnObjectSearchToolRepository(nSrcProductIndex);
	}
	
	if (pSrcProduct->m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	{
		CBaseSearchTool* pObjSearchTool = NULL;
		cpImage image;
		for (i=0; i<pSrcProduct->m_pAlignerParam->GetObjectMarkPatternNum(); i++)
		{
			for(j = 0; j<OBJECTCANDIDATENUM; j++)
			{
				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnObjectSearchTool(nSrcProductIndex, i, j);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnObjectSearchTool(nDstProductIndex, i, j, pObjSearchTool);
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveAlnObjectSearchToolRepository(nDstProductIndex);
		pDstProduct->m_bAlnObjectSearchToolInited = TRUE;
	}
	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteObjectSearchTools();
			}
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteObjectSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteObjectSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteObjectSearchTools();
			}
		}
	}


	// 3. 虚拟对象Mark模板设置
	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnVirtualObjectSearchToolRepository(nSrcProductIndex);
	}
	
	if (pSrcProduct->m_pAlignerParam->GetObjectMarkType() == eObjectVirtual)
	{
		CBaseSearchTool* pVirtualObjSearchTool = NULL;
		cpImage image;
		for (i=0; i<pSrcProduct->m_pAlignerParam->GetObjectMarkPatternNum()/*GetVirtualObjectMarkPatternNum()*/; i++)
		{
			for(j = 0; j<VIRTUALOBJECTCANDIDATENUM; j++)
			{
				pVirtualObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnVirtualObjectSearchTool(nSrcProductIndex, i, j);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnVirtualObjectSearchTool(nDstProductIndex, i, j, pVirtualObjSearchTool);
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveAlnVirtualObjectSearchToolRepository(nDstProductIndex);
		pDstProduct->m_bAlnVirtualObjectSearchToolInited = TRUE;
	}
	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteVirtualObjectSearchTools();
			}
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteVirtualObjectSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteVirtualObjectSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteVirtualObjectSearchTools();
			}
		}
	}

	// 4. 关联标定Mark模板工具设置
	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnCalibTargetSearchToolRepository(nSrcProductIndex);
	}
	
	if (pSrcProduct->m_pAlignerParam->GetTargetObjectCamSeparateEnable() == TRUE ||
		(pSrcProduct->m_pAlignerParam->GetTargetObjectCamSeparateEnable() == FALSE && pSrcProduct->m_pAlignerParam->GetTargetCalibUseBoardEnable() == TRUE))
	{
		CBaseSearchTool* pCalibTargetSearchTool = NULL;
		for (i=0; i<pSrcProduct->m_pAlignerParam->GetCalibTargetMarkPatternNum(); i++)
		{
			for(j = 0; j<ALNCALIBTARGETCANDIDATENUM; j++)
			{
				pCalibTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnCalibTargetSearchTool(nSrcProductIndex, i, j);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnCalibTargetSearchTool(nDstProductIndex, i, j, pCalibTargetSearchTool);
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveAlnCalibTargetSearchToolRepository(nDstProductIndex);
		pDstProduct->m_bAlnCalibTargetSearchToolInited = TRUE;
	}
	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteCalibTargetSearchTools();
			}
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteCalibTargetSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteCalibTargetSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteCalibTargetSearchTools();
			}
		}
	}


	// 5. 检测Mark模板工具设置
	if (nSrcProductIndex != m_nCurProductIdx)
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->InitAlnInspectSearchToolRepository(nSrcProductIndex);
	}
	
	if (pSrcProduct->m_pAlignerParam->GetSysPlatformInspectEnable() == TRUE)
	{
		CBaseSearchTool* pInspectSearchTool = NULL;
		for (i=0; i<pSrcProduct->m_pAlignerParam->GetInspectMarkPatternNum(); i++)
		{
			for(j = 0; j<INSPECTCANDIDATENUM; j++)
			{
				pInspectSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnInspectSearchTool(nSrcProductIndex, i, j);
				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnInspectSearchTool(nDstProductIndex, i, j, pInspectSearchTool);
			}
		}
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveAlnInspectSearchToolRepository(nDstProductIndex);
		pDstProduct->m_bAlnInspectSearchToolInited = TRUE;
	}
	// 释放原来产品的定位工具20170516
	{
		SysPlatformInfo sysPlatformInfo;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bLoadSingleProductSearchToolEnable == TRUE)
		{
			if (nSrcProductIndex != m_nCurProductIdx)
			{
				pSrcProduct->DeleteInspectSearchTools();
			}
			if (nDstProductIndex != m_nCurProductIdx && pDstProduct != NULL)
			{
				pDstProduct->DeleteInspectSearchTools();
			}
		}
		// 启用大小品种且只加载单个大产品时，如果Src/Dst和Cur的大品种不同，则释放Src/Dst品种
		else if (sysPlatformInfo.m_bProductFaChd && sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd)
		{
			if(nSrcProductIndexFa != nCurProductIndexFa)
			{
				pSrcProduct->DeleteInspectSearchTools();
			}

			if(nDstProductIndexFa != nCurProductIndexFa && pDstProduct != NULL)
			{
				pDstProduct->DeleteInspectSearchTools();
			}
		}
	}


	if (nDstProductIndex == m_nCurProductIdx)
	{
		UpdateDlgData(FALSE);
		if(m_pParentKind->m_pDlgKindAlignSetting != NULL)
		{
			m_pParentKind->m_pDlgKindAlignSetting->UpdateDlgData(FALSE);
		}
		if(m_pParentKind->m_pDlgKindAlignTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignTarget->UpdateDlgData(FALSE);
		}
		if(m_pParentKind->m_pDlgKindAlignObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignObject->UpdateDlgData(FALSE);
		}
		if(m_pParentKind->m_pDlgKindAlignVirtualObject != NULL)
		{
			m_pParentKind->m_pDlgKindAlignVirtualObject->UpdateDlgData(FALSE);
		}
		if(m_pParentKind->m_pDlgKindAlignCalibTarget != NULL)
		{
			m_pParentKind->m_pDlgKindAlignCalibTarget->UpdateDlgData(FALSE);
		}
		if(m_pParentKind->m_pDlgKindAlignInspect != NULL)
		{
			m_pParentKind->m_pDlgKindAlignInspect->UpdateDlgData(FALSE);
		}
		UpdateProductListDisplay();
		//复制的所有产品中，当目标产品为当前产品时，当前产品都发生了改变，所有工具都需要进行保存，故设为true
		//如果目标产品中不存在当前产品时，工具改变标识符不发生改变
		m_pParentKind->SetProductSearchToolChanged(true);
		m_pParentKind->SetCalibObjectSearchToolChanged(true);
	}

	CopyXMLDataFile(nSrcProductIndex,nDstProductIndex);//拷贝产品的SearchData.xml文件
	CopyProductCalibData(nSrcProductIndex,nDstProductIndex);

}

void CDlgSysKindAlignView::ClipMouseCursor(BOOL bClip)
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

//拷贝产品的SearchData.xml文件和Camera.xml文件
BOOL CDlgSysKindAlignView::CopyXMLDataFile(int nSrcProductIndex, int nDstProductIndex)
{
	CString strEXEDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\')); 
	CFileFind findFile;
	BOOL IsFild, IsDir;
	SysPlatformInfo platInfo;
	CString strSrcName;
	CString strDstName;
	strSrcName.Format(_T("%03d"),nSrcProductIndex+1);
	strDstName.Format(_T("%03d"),nDstProductIndex+1);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platInfo);
	CString strSrcDir = strEXEDir + platInfo.m_strPlatformDetailSavePath 
		+ _T("\\Jobs\\") + strSrcName;
	CString strDstDir = strEXEDir + platInfo.m_strPlatformDetailSavePath 
		+ _T("\\Jobs\\") + strDstName;

	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strSrcDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (!IsDir)
	{
		return FALSE;
	}
	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strDstDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (!IsDir)
	{
		return FALSE;
	}

	CString strSrcSearchDataDir = strSrcDir + _T("\\SearchData.xml");
	CString strDstSearchDataDir = strDstDir + _T("\\SearchData.xml");
	CString strSrcCameraDataDir = strSrcDir + _T("\\Camera.xml");
	CString strDstCameraDataDir = strDstDir + _T("\\Camera.xml");

	BOOL bResult = CopyFile(strSrcSearchDataDir,strDstSearchDataDir,FALSE);
	// bug修复： bResult为false时CopyFile(strSrcCameraDataDir,strDstCameraDataDir,FALSE)并不执行的bug
	bResult = CopyFile(strSrcCameraDataDir,strDstCameraDataDir,FALSE) && bResult;

	return bResult; 
}
BOOL CDlgSysKindAlignView::CopyCalibXMLDataFile(int nSrcProductIndex, int nDstProductIndex)
{
	CString strEXEDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\')); 
	CFileFind findFile;
	BOOL IsFild, IsDir;
	SysPlatformInfo platInfo;
	CString strSrcName;
	CString strDstName;
	strSrcName.Format(_T("%03d"),nSrcProductIndex+1);
	strDstName.Format(_T("%03d"),nDstProductIndex+1);
	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platInfo);
	CString strSrcDir = strEXEDir + platInfo.m_strPlatformDetailSavePath 
		+ _T("\\JobCalibs\\") + strSrcName;
	CString strDstDir = strEXEDir + platInfo.m_strPlatformDetailSavePath 
		+ _T("\\JobCalibs\\") + strDstName;

	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strSrcDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (!IsDir)
	{
		return FALSE;
	}
	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strDstDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (!IsDir)
	{
		return FALSE;
	}

	CString strSrcDataDir = strSrcDir + _T("\\CalibMoveImagePos.xml");
	CString strDstDataDir = strDstDir + _T("\\CalibMoveImagePos.xml");

	BOOL bResult = CopyFile(strSrcDataDir,strDstDataDir,FALSE);

	return bResult; 
}
void CDlgSysKindAlignView::OnCbnSelchangeComboProductSrc()
{
	// TODO: 在此添加控件通知处理程序代码
	int m_nSrcProductIndexOld = m_nSrcProductIndex;
	m_nSrcProductIndex = m_comboProductSrc.GetCurSel();
	if (m_nSrcProductIndexOld != m_nSrcProductIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[复制产品]中被复制对象由产品%d到产品%d\n"),m_nSrcProductIndexOld+1,m_nSrcProductIndex+1);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignView::OnCbnSelchangeComboProductSrcFa()
{
	int nSrcFaProductIndex = m_comboProductSrcFa.GetCurSel();
	int nCount = m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nSrcFaProductIndex);
	m_comboProductSrc.ResetContent();
	CTotalProductData*  pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	for(int i=0; i < nCount; i++)
	{
		CString strTemp,strTemp1;
		// 序号
		strTemp.Format(_T("%03d"), i+1);

		int nProductIndex =  pTotalProduct->GetProductIndex(nSrcFaProductIndex,i);
		// 产品名称
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_comboProductSrc.InsertString(i, strTemp + _T("|") + strTemp1);
	}
	m_comboProductSrc.SetCurSel(0);
	m_nSrcProductIndexFa = nSrcFaProductIndex;
}


void CDlgSysKindAlignView::OnCbnSelchangeComboProductDst()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_nDstProductIndex = m_comboProductDst.GetCurSel();
	int m_nDstProductIndexOld = m_nDstProductIndex;
	m_nDstProductIndex = m_comboProductDst.GetCurSel();
	if (m_nDstProductIndexOld != m_nDstProductIndex)
	{
		CString strLog;
		strLog.Format(_T("修改[复制产品]中复制对象由产品%d到产品%d\n"),m_nDstProductIndexOld+1,m_nDstProductIndex+1);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignView::OnCbnSelchangeComboProductDstFa()
{
	int nDstFaProductIndex = m_comboProductDstFa.GetCurSel();
	int nCount =  m_vpTotalProductData.at(m_nPlatformIndex)->GetProductCountChd(nDstFaProductIndex);
	m_comboProductDst.ResetContent();
	CTotalProductData*  pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
	for(int i=0; i < nCount; i++)
	{
		CString strTemp,strTemp1;
		// 序号
		strTemp.Format(_T("%03d"), i+1);

		int nProductIndex =  pTotalProduct->GetProductIndex(nDstFaProductIndex,i);
		// 产品名称
		strTemp1 = pTotalProduct->GetItemName(nProductIndex);
		m_comboProductDst.InsertString(i, strTemp + _T("|") + strTemp1);
	}
	m_comboProductDst.SetCurSel(0);
	m_nDstProductIndexFa = nDstFaProductIndex;
}

BOOL CDlgSysKindAlignView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN) 
	{
		if (pMsg->wParam == VK_F8)
		{
			GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->ShowWindow(SW_SHOW);
		}

		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}



LRESULT CDlgSysKindAlignView::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetDlgItem(IDC_BUTTON_NEW_PRODUCT_MULTIPLE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_NEW_PRODUCT_NUM)->ShowWindow(SW_SHOW);

	return 0;

}

BOOL CDlgSysKindAlignView::LoadSearchToolInfo()
{
	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx < 0) || (m_nPrevProductIdx >= pTotalProduct->GetCount()))
	{
		return FALSE;
	}
	BOOL bSucceed = TRUE;
	// 保存产品信息到磁盘
	CString strDir, strTemp;
	SysPlatformInfo platformInfo;
	vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
	pVisionASM->GetSysPlatformInfo(platformInfo);
	CPlatformOptionInfo optionInfo;
	pVisionASM->GetPlatformOptionInfo(optionInfo);
	strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
	strTemp.Format(_T("\\Jobs\\%03d"), m_nPrevProductIdx+1);
	strDir += strTemp;  

	CProductData *pProduct = pTotalProduct->GetItemPtr(m_nPrevProductIdx);

	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		// 保存目标Mark模板
		bSucceed =bSucceed && m_pParentKind->LoadSearchTool(eTargetSearch,strDir,pProduct->m_vvpAlnTargetSearchToolRepo,m_pParentKind->m_vvbAlnTargetSearchToolChanged);
	}
	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
		// 保存虚拟对象模板
		bSucceed = bSucceed && m_pParentKind->LoadSearchTool(eVirtualSearch,strDir,pProduct->m_vvpAlnVirtualObjectSearchToolRepo,m_pParentKind->m_vvbAlnVirtualObjectSearchToolChanged);
	}
	if (platformInfo.m_eAlignerObjectType != eObjectBench)
	{
		// 保存对象模板
		bSucceed = bSucceed && m_pParentKind->LoadSearchTool(eObjectSearch,strDir,pProduct->m_vvpAlnObjectSearchToolRepo,m_pParentKind->m_vvbAlnObjectSearchToolChanged);
	}
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE)
	{
		// 保存关联模板
		bSucceed = bSucceed && m_pParentKind->LoadSearchTool(eCalibTargetSearch,strDir,pProduct->m_vvpAlnCalibTargetSearchToolRepo,m_pParentKind->m_vvbAlnCalibTargetSearchToolChanged);

	}
	//if (TRUE == optionInfo.m_bInspectEnable)
	{
		// 保存检测模板
		bSucceed = bSucceed && m_pParentKind->LoadSearchTool(eInspectSearch,strDir,pProduct->m_vvpAlnInspectSearchToolRepo,m_pParentKind->m_vvbAlnInspectSearchToolChanged);

	}
	return bSucceed;
}

void CDlgSysKindAlignView::UpdateDlgByAuthorize()
{
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && (m_nPlatformIndex<  m_pParent->m_vpVisionASM.size()) && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{

			CString strNodeName = _T("");
			// 产品列表
			vsConfig->GetCheckedByItem(eCalibrateAuthorize_ProudctList,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowProductList,m_bEnableProductList);

		}
	}
}