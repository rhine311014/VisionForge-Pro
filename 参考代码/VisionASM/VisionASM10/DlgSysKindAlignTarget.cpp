// DlgSysKindAlignTarget.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysKindAlignTarget.h"
#include "afxdialogex.h"

#include "DlgSearch.h"
#include "DlgProductRename.h"
#include "DlgBenchMarkAmend.h"


#include "DlgSysKind.h"
#include "DlgSearchToolCopyPos.h"
#include "DlgSearchToolModelLibrary.h"
// CDlgSysKindAlignTarget 对话框

IMPLEMENT_DYNAMIC(CDlgSysKindAlignTarget, CDialog)

CDlgSysKindAlignTarget::CDlgSysKindAlignTarget(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysKindAlignTarget::IDD, pParent)
	, m_nTargetCandidateEnable(0)
	, m_nTargetCandidateSync(0)
	, m_bTargetCandidateOffsetEnable1(FALSE)
	, m_bTargetCandidateOffsetEnable2(FALSE)
	, m_bTargetCandidateOffsetEnable3(FALSE)
	, m_bTargetCandidateOffsetEnable4(FALSE)
{
	//{{AFX_DATA_INIT(CDlgSysKindAlignTarget)
	m_strAlignerOffsetD = _T("0.000000");
	m_strAlignerOffsetX = _T("0.000000");
	m_strAlignerOffsetY = _T("0.000000");
	m_strTargetCandidateOffsetD1 = _T("0.000000");
	m_strTargetCandidateOffsetX1 = _T("0.000000");
	m_strTargetCandidateOffsetY1 = _T("0.000000");
	m_strTargetCandidateOffsetD2 = _T("0.000000");
	m_strTargetCandidateOffsetX2 = _T("0.000000");
	m_strTargetCandidateOffsetY2 = _T("0.000000");
	m_strTargetCandidateOffsetD3 = _T("0.000000");
	m_strTargetCandidateOffsetX3 = _T("0.000000");
	m_strTargetCandidateOffsetY3 = _T("0.000000");
	m_strTargetCandidateOffsetD4 = _T("0.000000");
	m_strTargetCandidateOffsetX4 = _T("0.000000");
	m_strTargetCandidateOffsetY4 = _T("0.000000");

	m_strAlignerPrecisionX = _T("0.010000");
	m_strAlignerPrecisionY = _T("0.010000");
	m_strAlignerPrecisionD = _T("0.010000");
	m_nAlignerMaxTimes = 1;
	m_nPlatformIndex = 0;
	m_strCurProductName = _T("");
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
	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_strProductNameOld.Empty();

	m_bTargetShutterEnable0 = FALSE;
	m_bTargetShutterEnable1 = FALSE;
	m_bTargetShutterEnable2 = FALSE;
	m_bTargetShutterEnable3 = FALSE;
	m_bTargetShutterEnable4 = FALSE;

	//	m_pParent = (CVisionASMDlg *)pParent;
	// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
	// 	CTotalProductData *pTotalProduct = NULL;
	// 	for (int i=0; i<nPlatformNum; i++)
	// 	{
	// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
	// 		m_vpTotalProductData.push_back(pTotalProduct);
	// 	}

}

CDlgSysKindAlignTarget::~CDlgSysKindAlignTarget()
{
	DestroyWindow();
}

void CDlgSysKindAlignTarget::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysKindAlignTarget)
	//	DDX_Radio(pDX, IDC_RADIO_SHOW_PLATFORM1, m_nPlatformIndex);
	//  DDX_Control(pDX, IDC_LIST_PRODUCT, m_listctrlProduct);
	// 	DDX_Control(pDX, IDOK, m_btnOK);
	// 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//DDX_Control(pDX, IDC_BUTTON_NEW_PRODUCT, m_btnNewProduct);
	//DDX_Control(pDX, IDC_BUTTON_DELETE_PRODUCT, m_btnDeleteProduct);
	//DDX_Control(pDX, IDC_BTN_PRODUCT_RENAME, m_btnRename);
	//DDX_Control(pDX, IDC_BTN_AMEND_BENCHMARK, m_btnAmendBenchMark);
	//DDX_Control(pDX, IDC_COMBO_ALIGNER_OFFSET_COORD_TYPE, m_comboAlignerOffsetCoordType);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM1, m_btnShowPlatform1);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM2, m_btnShowPlatform2);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM3, m_btnShowPlatform3);
	// 	DDX_Control(pDX, IDC_RADIO_SHOW_PLATFORM4, m_btnShowPlatform4);
	// 	DDX_Control(pDX, IDC_BTN_PANEL, m_btnPanel);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
	//DDX_Text(pDX, IDC_EDIT_ALIGNER_MAX_TIMES, m_nAlignerMaxTimes);
	//DDX_Text(pDX, IDC_EDIT_CUR_PRODUCT_NAME, m_strCurProductName);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_TARGETC_SEARCH_MODE0, m_nTargetCandidateEnable);
	DDX_Radio(pDX, IDC_RADIO_TARGETC_SEARCH_SYNC0, m_nTargetCandidateSync);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, m_strTargetCandidateOffsetD1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, m_strTargetCandidateOffsetX1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, m_strTargetCandidateOffsetY1);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, m_strTargetCandidateOffsetD2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, m_strTargetCandidateOffsetX2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, m_strTargetCandidateOffsetY2);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, m_strTargetCandidateOffsetD3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, m_strTargetCandidateOffsetX3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, m_strTargetCandidateOffsetY3);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, m_strTargetCandidateOffsetD4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, m_strTargetCandidateOffsetX4);
	DDX_Text(pDX, IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, m_strTargetCandidateOffsetY4);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1, m_bTargetCandidateOffsetEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2, m_bTargetCandidateOffsetEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3, m_bTargetCandidateOffsetEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4, m_bTargetCandidateOffsetEnable4);
	DDX_Control(pDX, IDC_STATIC_POS0, m_labelPos0);
	DDX_Control(pDX, IDC_STATIC_POS1, m_labelPos1);
	DDX_Control(pDX, IDC_STATIC_POS2, m_labelPos2);
	DDX_Control(pDX, IDC_STATIC_POS3, m_labelPos3);
	DDX_Control(pDX, IDC_STATIC_POS4, m_labelPos4);
	DDX_Control(pDX, IDC_STATIC_POS5, m_labelPos5);
	DDX_Control(pDX, IDC_STATIC_POS6, m_labelPos6);
	DDX_Control(pDX, IDC_STATIC_POS7, m_labelPos7);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE,  m_bTargetShutterEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE1, m_bTargetShutterEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE2, m_bTargetShutterEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE3, m_bTargetShutterEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE4, m_bTargetShutterEnable4);
	
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE, m_bTargetGainEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE1,m_bTargetGainEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE2,m_bTargetGainEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE3,m_bTargetGainEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE4,m_bTargetGainEnable4);

	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE, m_bTargetGammaEnable0);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE1,m_bTargetGammaEnable1);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE2,m_bTargetGammaEnable2);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE3,m_bTargetGammaEnable3);
	DDX_Check(pDX, IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE4,m_bTargetGammaEnable4);

}


BEGIN_MESSAGE_MAP(CDlgSysKindAlignTarget, CDialog)
	//{{AFX_MSG_MAP(CDlgSysKindAlignTarget)
	ON_MESSAGE(WM_SV_DISPLAY_DBLCLICK, OnGuiDisplayDblClk)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM1, OnGuiDisplayMenuCustom1)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM2, OnGuiDisplayMenuCustom2)
	ON_MESSAGE(WM_SV_DISPLAY_MENU_CUSTOM3, OnGuiDisplayMenuCustom3)
// 	ON_BN_CLICKED(IDC_BTN_PRODUCT_RENAME, OnBtnProductRename)
// 	ON_BN_CLICKED(IDC_BTN_AMEND_BENCHMARK, OnBtnAmendBenchmark)
	//	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM1, OnPlatformChange)
// 	ON_BN_CLICKED(IDC_BUTTON_NEW_PRODUCT, OnButtonNewProduct)
// 	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRODUCT, OnButtonDeleteCurProduct)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnSetfocusEditAlignerMaxTimes)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnSetfocusEditAlignerXPrecision)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnSetfocusEditAlignerYPrecision)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnSetfocusEditAlignerDPrecision)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnSetfocusEditAlignerDOffset)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnSetfocusEditAlignerXOffset)
// 	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnSetfocusEditAlignerYOffset)
// 	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, OnClickListProduct)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_PRECISION, OnKillfocusEditAlignerXPrecision)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_PRECISION, OnKillfocusEditAlignerYPrecision)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_PRECISION, OnKillfocusEditAlignerDPrecision)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_D_OFFSET, OnKillfocusEditAlignerDOffset)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_X_OFFSET, OnKillfocusEditAlignerXOffset)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_Y_OFFSET, OnKillfocusEditAlignerYOffset)
// 	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_MAX_TIMES, OnKillfocusEditAlignerMaxTimes)
	//	ON_WM_CLOSE()
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM2, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM3, OnPlatformChange)
	// 	ON_BN_CLICKED(IDC_RADIO_SHOW_PLATFORM4, OnPlatformChange)
// 	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, OnClickListProduct)
// 	ON_BN_CLICKED(IDC_BTN_CLONE_PRODUCT, OnBtnCloneProduct)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, OnSetfocusEditTargetCandidateDOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, OnSetfocusEditTargetCandidateXOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, OnSetfocusEditTargetCandidateYOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, OnSetfocusEditTargetCandidateDOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, OnSetfocusEditTargetCandidateXOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, OnSetfocusEditTargetCandidateYOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, OnSetfocusEditTargetCandidateDOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, OnSetfocusEditTargetCandidateXOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, OnSetfocusEditTargetCandidateYOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, OnSetfocusEditTargetCandidateDOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, OnSetfocusEditTargetCandidateXOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, OnSetfocusEditTargetCandidateYOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, OnKillfocusEditTargetCandidateDOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, OnKillfocusEditTargetCandidateXOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, OnKillfocusEditTargetCandidateYOffset1)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, OnKillfocusEditTargetCandidateDOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, OnKillfocusEditTargetCandidateXOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, OnKillfocusEditTargetCandidateYOffset2)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, OnKillfocusEditTargetCandidateDOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, OnKillfocusEditTargetCandidateXOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, OnKillfocusEditTargetCandidateYOffset3)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, OnKillfocusEditTargetCandidateDOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, OnKillfocusEditTargetCandidateXOffset4)
	ON_EN_KILLFOCUS(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, OnKillfocusEditTargetCandidateYOffset4)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE1, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE2, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE3, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE4, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE1, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE2, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE3, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE4, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable)

	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE1, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE2, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE3, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE4, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable)

	ON_BN_CLICKED(IDC_RADIO_TARGETC_SEARCH_MODE0, &CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchModeChange)
	ON_BN_CLICKED(IDC_RADIO_TARGETC_SEARCH_MODE1, &CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchModeChange)
	ON_BN_CLICKED(IDC_RADIO_TARGETC_SEARCH_SYNC0, &CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchSyncChange)
	ON_BN_CLICKED(IDC_RADIO_TARGETC_SEARCH_SYNC1, &CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchSyncChange)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE1, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable1)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE2, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable2)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE3, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable3)
	ON_BN_CLICKED(IDC_CHECK_ALIGNER_TARGETC_OFFSET_ENABLE4, &CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable4)

END_MESSAGE_MAP()


// CDlgSysKindAlignTarget 消息处理程序


BOOL CDlgSysKindAlignTarget::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	m_bSysKeyboardEnabled = FALSE;
// 	int nPlatformNum = m_pParent->m_vpVisionASM.size();
// 	CTotalProductData *pTotalProduct = NULL;
// 	for (int i=0; i<nPlatformNum; i++)
// 	{
// 		pTotalProduct = m_pParent->m_vpVisionASM.at(i)->GetTotalProuductData();
// 		m_vpTotalProductData.push_back(pTotalProduct);
// 	}
// 
// 
// 
// 	// TODO:  在此添加额外的初始化
// 	CString strModuelDir;
// 	GetModuleFileName(NULL, strModuelDir.GetBuffer(MAX_PATH), MAX_PATH);
// 	strModuelDir.ReleaseBuffer();
// 	m_strModuleDir = strModuelDir.Left(strModuelDir.ReverseFind('\\'));   


	m_strModuleDir = m_pParentKind->m_strModuleDir;

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = m_nPlatformIndex;//不加这句会崩溃
	int nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_strProductNameOld = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);

	UpdateDialogLanguage();
	InitDlgItem();
	
	UpdateDlgData(FALSE);
	UpdateProductListDisplay();
// 
//  	int nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
//  	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(nCurProductIdx);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlgSysKindAlignTarget::InitDlgItem()
{
	int i = 0, j = 0;

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

	// 创建scGUIDisplay
//	m_vpTargetGuiDisplay.clear();
//	m_vpObjectGuiDisplay.clear();
//	m_vpVirtualObjectGuiDisplay.clear();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}
	
	m_vvpTargetGuiDisplay.resize(MAXPOSNUM);
	scGuiDisplay *pGUIDisplay = NULL;
	for (i=0; i<MAXPOSNUM; i++)
	{
		for (j = 0; j < TARGETCANDIDATENUM; j++)
		{
			pGUIDisplay = new scGuiDisplay;
			pGUIDisplay->Create(IDC_STATIC_TARGETC_DISPLAY0+ i*TARGETCANDIDATENUM + j, this, WS_BORDER);
			pGUIDisplay->RegisterWndMsg(this, SV_DISPLAY_DBLCLICK | SV_DISPLAY_CLICK | SV_DISPLAY_CUSTOM1 | SV_DISPLAY_CUSTOM2 | SV_DISPLAY_CUSTOM3);
			switch (lgidLanguage)
			{
			case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
				pGUIDisplay->SetLanguage(0);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("Clear Search Tool"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("Copy Search Tool To ..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("Get Search Tool From Model library"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
			default:
				pGUIDisplay->SetLanguage(1);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom1, _T("清空定位工具"), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom2, _T("拷贝定位工具到..."), TRUE);
				pGUIDisplay->SetCustomMenuVisableAndName(cmGuiDisplayCustom3, _T("获取模板库定位工具"), TRUE);
				break;
			}
			m_vvpTargetGuiDisplay[i].push_back(pGUIDisplay);
			pGUIDisplay = NULL;
		}
	}

// 	// 动态计算并调整对话框大小
 	int nMaxCamNum = 0;
	int nClientWidth = 0, nClientHeight = 0;
	int nDispInterval = 0, nGRPIndent = 6;
	int nFrame = 0;
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

	CRect rcClient;
	GetClientRect(rcClient);
	for(i = 0; i < 6; i++)
	{
		GetDlgItem(IDC_STATIC_SEPARATOR2+i)->GetWindowRect(rcItem1);
		ScreenToClient(rcItem1);
		rcItem1.top = rcClient.top;
		rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
		GetDlgItem(IDC_STATIC_SEPARATOR2+i)->MoveWindow(rcItem1);
	}


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
// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);


				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);


				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
				}
			}
			else
			{
// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
 				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
//		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
	}
	else
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
// 			GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
 			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);


			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < TARGETCANDIDATENUM; j++)
			{
				GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
				m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
// 		if (platformInfo.m_bBenchMarkAmend)
// 		{
// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
// 		}
	}

// 	// 虚拟对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<6; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<6; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 	// 对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<6; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<6; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
// 	}
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

	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}

	EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignTarget::EnableAndShowControl(bool bEnable,bool bShow)
{
	bool bShowTemplateSet = false;
	bool bEnableTemplateSet = false;
	if(NULL != m_pParentKind)
	{
		bShowTemplateSet = m_pParentKind->m_bShowTargetTemplateSet;
		bEnableTemplateSet = m_pParentKind->m_bEnableTargetTemplateSet;
	}
	bEnable = bEnable && bEnableTemplateSet;
	bShow = bShow && bShowTemplateSet;

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE1)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_SHUTTER_ENABLE4)->ShowWindow(bShow);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE1)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAIN_ENABLE4)->ShowWindow(bShow);

	//暂时禁用Gamma实时拍照切换
	{
		bEnable = false;
		bShow = false;
	}
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE4)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE1)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE2)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE3)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_ALIGNER_TARGETC_GAMMA_ENABLE4)->ShowWindow(bShow);

}

void CDlgSysKindAlignTarget::MoveDlgItem()
{
	if (GetDlgItem(IDC_STATIC_POS0)->GetSafeHwnd() != NULL)
	{
		int i = 0, j = 0;

		// 调整控制板
		int nPlatformNum = m_pParent->m_SystemConfigure.GetPlatformNum();
		ASSERT(nPlatformNum == m_pParent->m_vpVisionASM.size());

		// 创建scGUIDisplay
		//	m_vpTargetGuiDisplay.clear();
		//	m_vpObjectGuiDisplay.clear();
		//	m_vpVirtualObjectGuiDisplay.clear();

		// 	// 动态计算并调整对话框大小
		int nMaxCamNum = 0;
		int nClientWidth = 0, nClientHeight = 0;
		int nDispInterval = 0, nGRPIndent = 6;
		int nFrame = 0;
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

		CRect rcClient;
		GetClientRect(rcClient);
		for(i = 0; i < 8; i++)
		{
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->GetWindowRect(rcItem1);
			ScreenToClient(rcItem1);
			rcItem1.top = rcClient.top;
			rcItem1.bottom = rcClient.bottom - nStatusBarHeight;
			GetDlgItem(IDC_STATIC_SEPARATOR2+i)->MoveWindow(rcItem1);
		}


		//////////////////////////////////////////////////////////////////////////
		// 根据默认平台调整界面
		SysPlatformInfo platformInfo;
		//	int nPosNum = 0, i = 0;
		m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
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
					// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
					// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);


					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
					m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);


					for(j = 0; j < TARGETCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
						m_vvpTargetGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
					}
				}
				else
				{
					// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
					// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

					if (i < MAXPOSNUM -1)
					{
						GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
					}
					m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
					m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

					for(j = 0; j < TARGETCANDIDATENUM; j++)
					{
						GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
						m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
					}
				}
			}
			//		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		}
		else
		{
			for (i=0; i<MAXPOSNUM; i++)
			{
				// 			GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);


				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
			// 		if (platformInfo.m_bBenchMarkAmend)
			// 		{
			// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
			// 		}
			// 		else
			// 		{
			// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
			// 		}
		}

		CPlatformOptionInfo optionInfo;
		if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
		{
			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
		}

		EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);
	

	}

	::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
}

void CDlgSysKindAlignTarget::CheckDlgData()
{
	UpdateData(TRUE);

	// 对位次数
	m_nAlignerMaxTimes  = (m_nAlignerMaxTimes > 10) ? 10 : ((m_nAlignerMaxTimes < 1) ? 1 : m_nAlignerMaxTimes);
}

void CDlgSysKindAlignTarget::UpdateDlgData( BOOL bSaveAndValidate /*= TRUE*/ )
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	UpdateData(TRUE);

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPlatformIndex);
	ASSERT(pTotalProduct != NULL);
	int nCurProdIdx = 0;
	CProductData product;
	if (bSaveAndValidate)
	{
		nCurProdIdx = pTotalProduct->GetCurProductIndex();
		if (nCurProdIdx >= pTotalProduct->GetCount())
		{
			return;
		}
		product = pTotalProduct->GetItem(nCurProdIdx);
		product.m_pAlignerParam->SetTargetCandidateEnable(m_nTargetCandidateEnable);
		product.m_pAlignerParam->SetTargetCandidateSyncEnable(m_nTargetCandidateSync);
		
		CCoordPos pos;		
		int i=0; 

		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable0;

		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX1);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX1);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX1);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable1);

		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable1;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX2);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX2);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX2);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable2);
		
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable2;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable2;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable2;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX3);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX3);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX3);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable3);

		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable3;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable3;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable3;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX4);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX4);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX4);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable4);

		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable4;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable4;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable4;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);


		pTotalProduct->SetItem(nCurProdIdx, product);
	}
	else
	{
		BeginWaitCursor();
		int nProductNum = pTotalProduct->GetCount();
		nCurProdIdx = pTotalProduct->GetCurProductIndex();		
		product = pTotalProduct->GetItem(nCurProdIdx);

		m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);

		m_nCurProductIdx = nCurProdIdx;
		m_nPrevProductIdx = m_nCurProductIdx;
// 
// 		m_nCurProductIdx = nCurProdIdx;
// 		m_nPrevProductIdx = m_nCurProductIdx;
// 		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
// 		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
// 		m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
// 		m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
// 		m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());
// 		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
// 		pos = product.m_pAlignerParam->GetAlignerOffset();
// 		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
// 		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
// 		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());

		m_nTargetCandidateEnable = (int)product.m_pAlignerParam->GetTargetCandidateEnable();
		m_nTargetCandidateSync = (int)product.m_pAlignerParam->GetTargetCandidateSyncEnable();
		
		CCoordPos pos;
		int i = 0;

		m_bTargetShutterEnable0 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable;
		m_bTargetGainEnable0 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable;
		m_bTargetGammaEnable0 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable;

		i++;
		pos = product.m_pAlignerParam->GetTargetCandidateOffset(i);
		m_strTargetCandidateOffsetX1.Format(_T("%.6f"), pos.GetPosX());
		m_strTargetCandidateOffsetY1.Format(_T("%.6f"), pos.GetPosY());
		m_strTargetCandidateOffsetD1.Format(_T("%.6f"), pos.GetAngle());
		m_bTargetCandidateOffsetEnable1 = product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i);

		m_bTargetShutterEnable1 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable;
		m_bTargetGainEnable1 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable;
		m_bTargetGammaEnable1 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable;

		i++;
		pos = product.m_pAlignerParam->GetTargetCandidateOffset(i);
		m_strTargetCandidateOffsetX2.Format(_T("%.6f"), pos.GetPosX());
		m_strTargetCandidateOffsetY2.Format(_T("%.6f"), pos.GetPosY());
		m_strTargetCandidateOffsetD2.Format(_T("%.6f"), pos.GetAngle());
		m_bTargetCandidateOffsetEnable2 = product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i);
		m_bTargetShutterEnable2 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable;
		m_bTargetGainEnable2 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable;
		m_bTargetGammaEnable2 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable;

		i++;
		pos = product.m_pAlignerParam->GetTargetCandidateOffset(i);
		m_strTargetCandidateOffsetX3.Format(_T("%.6f"), pos.GetPosX());
		m_strTargetCandidateOffsetY3.Format(_T("%.6f"), pos.GetPosY());
		m_strTargetCandidateOffsetD3.Format(_T("%.6f"), pos.GetAngle());
		m_bTargetCandidateOffsetEnable3 = product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i);

		m_bTargetShutterEnable3 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable;
		m_bTargetGainEnable3 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable;
		m_bTargetGammaEnable3 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable;

		i++;
		pos = product.m_pAlignerParam->GetTargetCandidateOffset(i);
		m_strTargetCandidateOffsetX4.Format(_T("%.6f"), pos.GetPosX());
		m_strTargetCandidateOffsetY4.Format(_T("%.6f"), pos.GetPosY());
		m_strTargetCandidateOffsetD4.Format(_T("%.6f"), pos.GetAngle());
		m_bTargetCandidateOffsetEnable4 = product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i);

		m_bTargetShutterEnable4 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable;
		m_bTargetGainEnable4 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable;
		m_bTargetGammaEnable4 = product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable;


		// 目标Mark模板图像显示
		int j = 0;
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
// 				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
// 				if (pTargetSearchTool != NULL)
// 				{
// 					image.Release();
// 					pTargetSearchTool->GetPatternImage(image);
// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
// 					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
// 				}
// 				else
// 				{
// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
// 				}

				for(j = 0; j<TARGETCANDIDATENUM; j++)
				{
					pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx, i, j);

					BOOL bExistSearchTool = IsSearchToolSelected(pTargetSearchTool);

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
// 
// 		// 对象Mark模板图像显示
// 		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
// 		{
// 			CBaseSearchTool* pObjSearchTool = NULL;
// 			scImageGray image;
// 			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
// 			{
// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				if (pObjSearchTool != NULL)
// 				{
// 					image.Release();
// 					pObjSearchTool->GetPatternImage(image);
// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
// 				}
// 				else
// 				{
// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 				}
// 			}
// 		}
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


void CDlgSysKindAlignTarget::GetDlgTempData(CProductData& product)
{
	ASSERT(GetSafeHwnd() != NULL);
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	UpdateData(TRUE);

	product.m_pAlignerParam->SetTargetCandidateEnable(m_nTargetCandidateEnable);
	product.m_pAlignerParam->SetTargetCandidateSyncEnable(m_nTargetCandidateSync);

	CCoordPos pos;		
	int i=0; 

	CameraParamEnable cameraParamEnable;
	cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable0;
	cameraParamEnable.m_bGainEnable = m_bTargetGainEnable0;
	cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable0;
	product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

	i++;		
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX1);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetX1);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetX1);
	product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
	product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable1);
	cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable1;
	cameraParamEnable.m_bGainEnable = m_bTargetGainEnable1;
	cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable1;
	product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

	i++;		
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX2);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetX2);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetX2);
	product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
	product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable2);
	cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable2;
	cameraParamEnable.m_bGainEnable = m_bTargetGainEnable2;
	cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable2;
	product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

	i++;		
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX3);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetX3);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetX3);
	product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
	product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable3);
	cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable3;
	cameraParamEnable.m_bGainEnable = m_bTargetGainEnable3;
	cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable3;
	product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

	i++;		
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX4);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetX4);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetX4);
	product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
	product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable4);
	cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable4;
	cameraParamEnable.m_bGainEnable = m_bTargetGainEnable4;
	cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable4;
	product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

	//		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->UpdateCurVisionASMTool(FALSE); // 更新当前视觉对位工具（加载定位工具）
	// 		m_strCurProductName = pTotalProduct->GetItemName(nCurProdIdx);
	// 
	// 		m_nCurProductIdx = nCurProdIdx;
	// 		m_nPrevProductIdx = m_nCurProductIdx;
	// 		m_nAlignerMaxTimes = product.m_pAlignerParam->GetAlignerMaxTime();
	// 		CCoordPos pos = product.m_pAlignerParam->GetAlignerPrecision();
	// 		m_strAlignerPrecisionX.Format(_T("%.6f"), pos.GetPosX());
	// 		m_strAlignerPrecisionY.Format(_T("%.6f"), pos.GetPosY());
	// 		m_strAlignerPrecisionD.Format(_T("%.6f"), pos.GetAngle());
	// 		m_comboAlignerOffsetCoordType.SetCurSel((int)product.m_pAlignerParam->GetAlignerOffsetCoordType());
	// 		pos = product.m_pAlignerParam->GetAlignerOffset();
	// 		m_strAlignerOffsetX.Format(_T("%.6f"), pos.GetPosX());
	// 		m_strAlignerOffsetY.Format(_T("%.6f"), pos.GetPosY());
	// 		m_strAlignerOffsetD.Format(_T("%.6f"), pos.GetAngle());

	// 		// 产品标定后参数检查，如果与当前平台标定后参数不同，需要给出提示信息，并做相应处理
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
	// 
	//// 目标Mark模板图像显示
	//int i = 0, j = 0;
	//if (product.m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
	//{
	//	CBaseSearchTool* pTargetSearchTool = NULL;
	//	scImageGray image;
	//	for (i=0; i<product.m_pAlignerParam->GetTargetMarkPatternNum(); i++)
	//	{
	//		// 				pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnTargetSearchToolParam(i);
	//		// 				if (pTargetSearchTool != NULL)
	//		// 				{
	//		// 					image.Release();
	//		// 					pTargetSearchTool->GetPatternImage(image);
	//		// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
	//		// 					m_vpTargetGuiDisplay.at(i)->SetImage(image, TRUE);
	//		// 				}
	//		// 				else
	//		// 				{
	//		// 					m_vpTargetGuiDisplay.at(i)->ClearScreen();
	//		// 				}

	//		for(j = 0; j<TARGETCANDIDATENUM; j++)
	//		{
	//			pTargetSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(i, j);
	//			if (pTargetSearchTool != NULL)
	//			{
	//				image.Release();
	//				pTargetSearchTool->GetPatternImage(image);
	//				m_vvpTargetGuiDisplay[i][j]->ClearScreen();
	//				m_vvpTargetGuiDisplay[i][j]->SetImage(image, TRUE);
	//			}
	//			else
	//			{
	//				m_vvpTargetGuiDisplay[i][j]->ClearScreen();
	//			}
	//		}
	//	}
	//}

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
	// 
	// 		// 对象Mark模板图像显示
	// 		if (product.m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	// 		{
	// 			CBaseSearchTool* pObjSearchTool = NULL;
	// 			scImageGray image;
	// 			for (i=0; i<product.m_pAlignerParam->GetObjectMarkPatternNum(); i++)
	// 			{
	// 				pObjSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
	// 				if (pObjSearchTool != NULL)
	// 				{
	// 					image.Release();
	// 					pObjSearchTool->GetPatternImage(image);
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 					m_vpObjectGuiDisplay.at(i)->SetImage(image, TRUE);
	// 				}
	// 				else
	// 				{
	// 					m_vpObjectGuiDisplay.at(i)->ClearScreen();
	// 				}
	// 			}
	// 		}
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

}

void CDlgSysKindAlignTarget::UpdateStatusBar()
{
// 	m_pStatusBar->SetIcon(0, m_bStatusBarFlag ? m_hIconSucceed : m_hIconFailed);
// 	m_pStatusBar->SetText(m_strStatusBarInfo1, 1, 0);
// 	m_pStatusBar->SetText(m_strStatusBarInfo2, 2, 0);
}

void CDlgSysKindAlignTarget::UpdateProductListDisplay()
{
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	if (pTotalProduct == NULL)
// 	{
// 		return;
// 	}
// 
// 	CString strTemp;
// 	int nProductNum = pTotalProduct->GetCount();
// 	m_listctrlProduct.DeleteAllItems();
// 	for (int i=0; i<nProductNum; i++)
// 	{
// 		// 序号
// 		strTemp.Format(_T("%d"), i);
// 		m_listctrlProduct.InsertItem(i, strTemp);
// 
// 		// 产品名称
// 		strTemp = pTotalProduct->GetItemName(i);
// 		m_listctrlProduct.SetItemText(i, 1, strTemp);
// 
// 		if (i == pTotalProduct->GetCurProductIndex())
// 		{
// 			m_listctrlProduct.SetItemState(i, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 		}
// 	}
}

void CDlgSysKindAlignTarget::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_KIND_ALIGN_TARGET") == strDlgID)
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

CString CDlgSysKindAlignTarget::GetNewDefName()
{
	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
	int nNum = pTotalProduct->GetCount();
	CString strName;
	for (int i=0; i<=nNum; i++)
	{
		strName.Format(_T("Job%d"), i);
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

BOOL CDlgSysKindAlignTarget::IsNameUnique( LPCTSTR lpszName )
{
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	int nProductNum = pTotalProduct->GetCount();
// 	CString strName = lpszName;
// 	if (strName.IsEmpty())
// 	{
// 		return FALSE;
// 	}
// 	for (int i=0; i<nProductNum; i++)
// 	{
// 		if (strName == pTotalProduct->GetItemName(i))
// 		{
// 			return FALSE;
// 		}
// 	}
	return TRUE;
}

BOOL CDlgSysKindAlignTarget::IsNameLegal( LPCTSTR lpszName )
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

BOOL CDlgSysKindAlignTarget::IsCalibratedInfosEqual( std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2 )
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

BOOL CDlgSysKindAlignTarget::IsProductChanged()
{
	if (m_bProdPatternChanged)
	{
		return TRUE;
	}

	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
//	m_nPlatformIndex = pView->m_nPlatformIndex;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	CTotalProductData *pTotalProduct = pView->m_vpTotalProductData.at(m_nPrevPlatformIdx);// 当平台未更改时，m_nPlatformIdx == m_nPrevPlatformIdx
	ASSERT(pTotalProduct != NULL);
	CProductData product;
	if (m_nPrevProductIdx >= pTotalProduct->GetCount() || (m_nPrevProductIdx < 0))
	{
		return TRUE;	// 当前产品设置不正确，则认为产品未被修改
	}
	pTotalProduct->SetCurProductIndex(m_nPrevProductIdx);
	product = pTotalProduct->GetItem(m_nPrevProductIdx);
	if (m_nTargetCandidateEnable != (int)product.m_pAlignerParam->GetTargetCandidateEnable())
	{
		return TRUE;
	}

	if (m_nTargetCandidateSync != (int)product.m_pAlignerParam->GetTargetCandidateSyncEnable())
	{
		return TRUE;
	}

	CCoordPos pos;
	int i = 0;

	if (m_bTargetShutterEnable0 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable
		|| m_bTargetGainEnable0 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable
		|| m_bTargetGammaEnable0 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}
	
	i++;
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX1);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetY1);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetD1);
	if (pos.m_dPosX != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dAngle
		|| m_bTargetCandidateOffsetEnable1 != product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i)
		|| m_bTargetShutterEnable1 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable
		|| m_bTargetGainEnable1 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable
		|| m_bTargetGammaEnable1 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}


	i++;
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX2);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetY2);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetD2);
	if (pos.m_dPosX != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dAngle
		|| m_bTargetCandidateOffsetEnable2 != product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i)
		|| m_bTargetShutterEnable2 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable
		|| m_bTargetGainEnable2 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable
		|| m_bTargetGammaEnable2 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}

	i++;
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX3);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetY3);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetD3);
	if (pos.m_dPosX != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dAngle
		|| m_bTargetCandidateOffsetEnable3 != product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i)
		|| m_bTargetShutterEnable3 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable
		|| m_bTargetGainEnable3 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable
		|| m_bTargetGammaEnable3 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}

	i++;
	pos.m_dPosX = atof(m_strTargetCandidateOffsetX4);
	pos.m_dPosY = atof(m_strTargetCandidateOffsetY4);
	pos.m_dAngle = atof(m_strTargetCandidateOffsetD4);
	if (pos.m_dPosX != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosX 
		|| pos.m_dPosY != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dPosY 
		|| pos.m_dAngle != product.m_pAlignerParam->GetTargetCandidateOffset(i).m_dAngle
		|| m_bTargetCandidateOffsetEnable4 != product.m_pAlignerParam->GetTargetCandidateOffsetEnable(i)
		|| m_bTargetShutterEnable4 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bShutterEnable
		|| m_bTargetGainEnable4 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGainEnable
		|| m_bTargetGammaEnable4 != product.m_pAlignerParam->GetEnableTargetSearchShutter(i).m_bGammaEnable)
	{
		return TRUE;
	}

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

BOOL CDlgSysKindAlignTarget::IsSearchToolSelected(CBaseSearchTool* pSearchTool)
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

BOOL CDlgSysKindAlignTarget::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgSysKindAlignTarget::CreateDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindAlignTarget::ClearDirectoryPro( LPCTSTR lpszDir )
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

BOOL CDlgSysKindAlignTarget::RemoveDirectoryPro( LPCTSTR lpszDir )
{
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

BOOL CDlgSysKindAlignTarget::RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
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
// 保存前一个产品的设置
BOOL CDlgSysKindAlignTarget::SavePrevProduct()
{
	BOOL bSucceed = TRUE;
	UpdateData(TRUE);
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPrevPlatformIdx = pView->m_nPrevPlatformIdx;
	m_nPrevProductIdx = pView->m_nPrevProductIdx;

	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
	{
		// 保存对位参数
		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
		product.m_pAlignerParam->SetTargetCandidateEnable(m_nTargetCandidateEnable);
		product.m_pAlignerParam->SetTargetCandidateSyncEnable(m_nTargetCandidateSync);

		CCoordPos pos;		
		int i=0; 

		CameraParamEnable cameraParamEnable;
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable0;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable0;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable0;

		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);

		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX1);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX1);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX1);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable1);
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable1;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable1;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable1;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);


		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX2);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX2);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX2);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable2);
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable2;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable2;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable2;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);


		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX3);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX3);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX3);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable3);
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable3;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable3;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable3;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);


		i++;		
		pos.m_dPosX = atof(m_strTargetCandidateOffsetX4);
		pos.m_dPosY = atof(m_strTargetCandidateOffsetX4);
		pos.m_dAngle = atof(m_strTargetCandidateOffsetX4);
		product.m_pAlignerParam->SetTargetCandidateOffset(i, pos);
		product.m_pAlignerParam->SetTargetCandidateOffsetEnable(i, m_bTargetCandidateOffsetEnable4);
		cameraParamEnable.m_bShutterEnable = m_bTargetShutterEnable4;
		cameraParamEnable.m_bGainEnable = m_bTargetGainEnable4;
		cameraParamEnable.m_bGammaEnable = m_bTargetGammaEnable4;
		product.m_pAlignerParam->SetEnableTargetSearchShutter(i, cameraParamEnable);


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
// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// 		{
// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// 		}

		// 		// by adk
		// 		CBaseSearchTool *pSearchTool = NULL;
		// 		SysPlatformInfo platformInfo;
		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
		// 		for (int j=0; j<nPatternNum; j++)
		// 		{
		// 			// 			int nPatSel = m_vnCurSelPat[j];
		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
		// 			
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
		// 		} // end by

		pTotalProduct->SetItem(m_nPrevProductIdx, product);
		m_bProdPatternChanged = FALSE;
	}

// 	UpdateData(TRUE);
// 	CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetTotalProuductData();
// 	if ((m_nPrevProductIdx >= 0) && (m_nPrevProductIdx < pTotalProduct->GetCount()))
// 	{
// 		// 保存对位参数
// 		CProductData product = pTotalProduct->GetItem(m_nPrevProductIdx);
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
// 		std::vector<CCalibratedInfo*> vpCalibratedInfo = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->GetCalibratedInfo();
// 		for (int i=0; i<product.m_vpCalibratedInfo.size(); i++)
// 		{
// 			*(product.m_vpCalibratedInfo.at(i)) = *(vpCalibratedInfo.at(i));
// 		}
// 
// 		// 		// by adk
// 		// 		CBaseSearchTool *pSearchTool = NULL;
// 		// 		SysPlatformInfo platformInfo;
// 		// 		vcBaseVisionAlign *pVisionAligner = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 		// 		pVisionAligner->GetSysPlatformInfo(platformInfo);
// 		// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		// 		for (int j=0; j<nPatternNum; j++)
// 		// 		{
// 		// 			// 			int nPatSel = m_vnCurSelPat[j];
// 		// 			int nPatSel = m_vCamWorkInfo[j].m_nPatSelected;
// 		// 			pSearchTool = pVisionAligner->GetRepoSearchTool(j, nPatSel);
// 		// 			pVisionAligner->SetAlnObjectSearchToolParam(j, pSearchTool, FALSE);
// 		// 			
// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nIndex = j;
// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatNum = pVisionAligner->GetRepoSearchToolCount(j);
// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_nPatSelected = nPatSel;
// 		// 			product.m_pAlignerParam->m_vCamWorkInfo[j].m_vdShutter = m_vCamWorkInfo[j].m_vdShutter;
// 		// 		} // end by
// 
// 		pTotalProduct->SetItem(m_nPrevProductIdx, product);
// 
// 
// 		// 保存产品信息到磁盘
// 		CString strDir, strTemp;
// 		SysPlatformInfo platformInfo;
// 		vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 		pVisionASM->GetSysPlatformInfo(platformInfo);
// 		// 如果产品被重命名，则删除旧文件架
// 		if ((m_bProdNameChanged) && (!m_strProductNameOld.IsEmpty()))
// 		{
// 			strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 			strTemp.Format(_T("\\Jobs\\%s"), m_strProductNameOld);
// 			strDir += strTemp;
// 			RemoveDirectoryPro(strDir);
// 		}
// 		strDir = m_strModuleDir + platformInfo.m_strPlatformDetailSavePath;
// 		strTemp.Format(_T("\\Jobs\\%s"), pTotalProduct->GetItemName(m_nPrevProductIdx));
// 		strDir += strTemp;        
// 		//ClearDirectoryPro(strDir); // Mali 2013.9.5 为提升效率
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveProductDataInfo(m_nPrevProductIdx);
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 
// 
// 		// 保存模板到磁盘
// 		int nPatternNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 		CBaseSearchTool *pSearchTool = NULL;
// 
// 		int j = 0;
// 		// 保存目标Mark模板
// 		if (platformInfo.m_eAlignerTargetType == eTargetOnline)
// 		{
// 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// 			{
// 				CreateDirectoryPro(strDir + _T("\\TargetModels"));
// 				for (j=0; j<nPatternNum; j++)
// 				{
// 					strTemp.Format(_T("\\TargetModels\\M%d"), j);
// 					pSearchTool = pVisionASM->GetAlnTargetSearchToolParam(j);
// 					if (pSearchTool != NULL) 
// 					{						
// 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 						{
// 							//strTemp.Format(_T("CH%d目标Mark模板保存失败！"), j);
// 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_TARGET_MARK_PATTEN_SAVE_FAILED));
// 							AfxMessageBox(strTemp);
// 							bSucceed = FALSE;
// 						}						
// 
// 					}
// 				}
// 
// 			}		
// 		}
// 		else
// 		{
// 			RemoveDirectoryPro(strDir + _T("\\TargetModels"));
// 		}
// 
// // 		// 保存虚拟对象Mark模板
// // 		if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// // 		{
// // 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// // 			{
// // 				CreateDirectoryPro(strDir + _T("\\ObjectExModels"));
// // 				for (j=0; j<nPatternNum; j++)
// // 				{
// // 					strTemp.Format(_T("\\ObjectExModels\\M%d"), j);
// // 					pSearchTool = pVisionASM->GetAlnVirtualSearchToolParam(j);
// // 					if (pSearchTool != NULL)
// // 					{
// // 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// // 						{
// // 							//strTemp.Format(_T("CH%d虚拟对象Mark模板保存失败！"), j);
// // 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_VIRTUAL_OBJ_MARK_PATTERN_SAVE_FAILED));
// // 							AfxMessageBox(strTemp);
// // 							bSucceed = FALSE;
// // 						}
// // 					}
// // 				}
// // 
// // 			}
// // 		}
// // 		else
// // 		{
// // 			RemoveDirectoryPro(strDir + _T("\\ObjectExModels"));
// // 		}
// // 
// // 
// // 		if (platformInfo.m_eAlignerObjectType != eObjectBench)
// // 		{
// // 			// 保存对象Mark模板
// // 			if (m_bProdNewCreate || m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// // 			{
// // 				CreateDirectoryPro(strDir + _T("\\ObjectModels"));
// // 				for (j=0; j<nPatternNum; j++)
// // 				{
// // 					strTemp.Format(_T("\\ObjectModels\\M%d"), j);
// // 					pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
// // 					if (pSearchTool != NULL)
// // 					{					
// // 						if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// // 						{
// // 							//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// // 							strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
// // 							AfxMessageBox(strTemp);
// // 							bSucceed = FALSE;
// // 						}	
// // 					}
// // 				}
// // 			}
// // 		}
// // 		else
// // 		{
// // 			RemoveDirectoryPro(strDir + _T("\\ObjectModels"));
// // 		}
// 
// 		// 		// 保存对象Mark模板
// 		// 		if (m_bProdPatternChanged || m_bProdNameChanged) // Mali 2013.9.5 为提升效率
// 		// 		{
// 		// 			CreateDirectoryPro(strDir + _T("\\ObjectModels"));
// 		// 			for (j=0; j<nPatternNum; j++)
// 		// 			{
// 		// 				strTemp.Format(_T("\\ObjectModels\\M%d"), j);
// 		// 				pSearchTool = pVisionASM->GetAlnObjectSearchToolParam(j);
// 		// 				if (pSearchTool != NULL)
// 		// 				{					
// 		// 					if (!pSearchTool->SaveModelToFile(strDir+strTemp))
// 		// 					{
// 		// 						//strTemp.Format(_T("CH%d对象Mark模板保存失败！"), j);
// 		// 						strTemp.Format(_T("CH%d%s"), j, GetSysInfoString(m_psaSysInfoStrings,IDS_MB_OBJECT_MARK_PATTERN_SAVE_FAILED));
// 		// 						AfxMessageBox(strTemp);
// 		// 						bSucceed = FALSE;
// 		// 					}	
// 		// 				}
// 		// 			}
// 		// 		}
// 
// 		m_strProductNameOld.Empty();
// 	}

	return bSucceed;
}
void CDlgSysKindAlignTarget::OnButtonNewProduct()
{
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
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
// 
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 				UpdateDlgData(FALSE);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
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
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
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
// 	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	int nCount = pTotalProduct->GetCount();
// 	CString strName = GetNewDefName();
// 	// 	if (nCount < 1)
// 	// 	{
// 	SysPlatformInfo platformInfo;
// 	m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
// 	CProductData product(platformInfo);
// 	pTotalProduct->AddItem(product);
// 	pTotalProduct->SetItemName(nCount, strName);
// 	pTotalProduct->SetCurProductIndex(nCount);
// 	// 	}
// 	// 	else
// 	// 	{
// 	// //		UpdateDlgData(TRUE);
// 	// 		int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
// 	// 		if (pTotalProduct->CopyItem(nFormIdx, nCount))
// 	// 		{
// 	// 			pTotalProduct->SetItemName(nCount, strName);
// 	// 			pTotalProduct->SetCurProductIndex(nCount);
// 	// 		}
// 	// 	}
// 	m_strProductNameOld = strName;
// 	m_bProdNewCreate = TRUE;

	UpdateDlgData(FALSE);
//	UpdateProductListDisplay();
}


void CDlgSysKindAlignTarget::OnBtnCloneProduct() 
{
	// TODO: Add your control notification handler code here
	// 如果前一个产品已修改，提示是否保存前一个产品的设置
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
// 
// 				m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 				UpdateDlgData(FALSE);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
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
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));	//_T("删除产品数据文件失败！")
// 				}
// 				m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
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
// 	// 如果当前平台的产品数量为零，则创建空产品；否则，将当前产品的信息拷备给新产品。
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	int nCount = pTotalProduct->GetCount();
// 	CString strName = GetNewDefName();
// 	// 	if (nCount < 1)
// 	// 	{
// 	// 		SysPlatformInfo platformInfo;
// 	// 		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetSysPlatformInfo(platformInfo);
// 	// 		CProductData product(platformInfo);
// 	// 		pTotalProduct->AddItem(product);
// 	// 		pTotalProduct->SetItemName(0, strName);
// 	// 		pTotalProduct->SetCurProductIndex(0);
// 	// 	}
// 	// 	else
// 	// 	{
// 	//		UpdateDlgData(TRUE);
// 	int nFormIdx = (nCount > m_nCurProductIdx) ? m_nCurProductIdx : nCount-1;
// 	if (pTotalProduct->CopyItem(nFormIdx, nCount))
// 	{
// 		pTotalProduct->SetItemName(nCount, strName);
// 		pTotalProduct->SetCurProductIndex(nCount);
// 	}
// 	//	}
// 	m_strProductNameOld = strName;
// 	m_bProdNewCreate = TRUE;
// 
// 	// 拷贝模板：保存模板到新产品的模板文件夹下以达到拷贝模板的目的
// 	int nPrevProdIdxTmp = m_nPrevProductIdx;
// 	m_nPrevProductIdx = nCount;
// 	SavePrevProduct();
// 	m_nPrevProductIdx = nPrevProdIdxTmp;

	UpdateDlgData(FALSE);
//	UpdateProductListDisplay();
}

void CDlgSysKindAlignTarget::OnButtonDeleteCurProduct() 
{
// 	// 用户删除操作确认
// 	if (AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PRODUCT_ROMOVE_CONFIRM), MB_YESNO) == IDNO)	//_T("确定要删除当前产品？")
// 	{
// 		return;
// 	}
// 
// 	UpdateData(TRUE);
// 	CTotalProductData *pTotalProduct = m_vpTotalProductData.at(m_nPlatformIndex);
// 	int nCount = pTotalProduct->GetCount();
// 	if (nCount > 0)
// 	{
// 		CString strDir;
// 		CString strProductName = pTotalProduct->GetItemName(m_nCurProductIdx);
// 		//		strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 		{
// 			SysPlatformInfo platformInfo;
// 			vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 			pVisionASM->GetSysPlatformInfo(platformInfo);
// 			strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 		}
// 		strDir = m_strModuleDir + strDir;
// 		if (!RemoveDirectoryPro(strDir))
// 		{
// 			//AfxMessageBox(_T("删除产品数据文件失败！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 		}
// 		if (pTotalProduct->RemoveItem(m_nCurProductIdx))
// 		{
// 			// 恢复修改标志
// 			m_bProdPatternChanged = FALSE;
// 			m_bProdNameChanged = FALSE;
// 			m_bProdBenchMarkChanged = FALSE;
// 			m_bProdNewCreate = FALSE;
// 			m_bCalibInfoChanged = FALSE;
// 
// 			m_nCurProductIdx = (m_nCurProductIdx > 0) ? (m_nCurProductIdx - 1) : m_nCurProductIdx;
// 			pTotalProduct->SetCurProductIndex(m_nCurProductIdx);
// 			UpdateDlgData(FALSE);
// 			UpdateProductListDisplay();
// 			m_strProductNameOld = pTotalProduct->GetItemName(pTotalProduct->GetCurProductIndex());
// 		}
// 		else
// 		{
// 			//AfxMessageBox(_T("删除产品失败！"));
// 			AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT));
// 		}
// 	}
// 	else
// 	{
// 		//AfxMessageBox(_T("当前没有任何产品！"));
// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_NO_PRODUCT_AVAILABLE));
// 	}
}

void CDlgSysKindAlignTarget::OnBtnProductRename()
{
// 	if ((m_nCurProductIdx < 0) || (m_nCurProductIdx >= m_vpTotalProductData.at(m_nPlatformIndex)->GetCount()))
// 	{
// 		//AfxMessageBox(_T("请选择当前产品"));
// 		AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_PLEASE_SELECT_CUR_PRODUCT));
// 	}
// 	CString strOldName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 
// 	CDlgProductRename  dlgProductRename(strOldName, this);
// 	dlgProductRename.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 	dlgProductRename.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 	if (dlgProductRename.DoModal() == IDOK)
// 	{
// 		CString strNewName = dlgProductRename.GetNewName();
// 		if (strNewName != strOldName)
// 		{
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
// 
// 			m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nCurProductIdx, strNewName);
// 			m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nCurProductIdx);
// 			m_bProdNameChanged = TRUE;
// 			UpdateDlgData(FALSE);
// 			UpdateProductListDisplay();
// 		}
// 	}
}

void CDlgSysKindAlignTarget::OnBtnAmendBenchmark() 
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


// 自定义菜单响应
LRESULT CDlgSysKindAlignTarget::OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < TARGETCANDIDATENUM; j++)
		{
			if (wParam == m_vvpTargetGuiDisplay[i][j]->GetID())
			{
				CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
				CProductData product = pTotalProduct->GetItem(nCurProdIdx);

				CameraParamEnable cameraParamEnable;
				cameraParamEnable = product.m_pAlignerParam->GetEnableTargetSearchShutter(j);

				CPlatformOptionInfo optionInfo;
				if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
				{
					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
				}
				BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

				if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
				{
					CameraParamSet tmpameraParamSet;
					product.m_pAlignerParam->SetTargetSearchShutter(i, j, tmpameraParamSet);
					pTotalProduct->SetItem(nCurProdIdx, product);
				}


				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx,i,j);
				
				CString strLog;
				if (j == 0)
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d目标模板\n"),i+1);
					g_opLogFile.SaveLogText(strLog,4);
				}
				else
				{
					strLog.Format(_T("点击右键选择[清空定位工具]清空位置%d候选模板%d\n"),i+1,j);
					g_opLogFile.SaveLogText(strLog,4);
				}
				
				if (pSearchTool == NULL)
				{
					return S_OK;
				}

				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnTargetSearchTool(nCurProdIdx,i, j);
				m_vvpTargetGuiDisplay[i][j]->ClearScreen();

				// 标记产品模板已修改
				m_bProdPatternChanged = TRUE;
				// 标记指定位置目标模板已经被修改了
				m_pParentKind->m_vvbAlnTargetSearchToolChanged[i][j] = TRUE;
				return S_OK;
			}
		}
	}

	return S_OK ;
}

LRESULT CDlgSysKindAlignTarget::OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0,nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;
	
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}

	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < TARGETCANDIDATENUM; j++)
		{
			if (wParam == m_vvpTargetGuiDisplay[i][j]->GetID())
			{
				CString strLog;
				strLog.Format(_T("点击右键选择[拷贝定位工具到...]\n"));
				g_opLogFile.SaveLogText(strLog,4);

				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx,i,j);

				CDlgSearchToolCopyPos dlgSearchToolCopyPos;
				dlgSearchToolCopyPos.m_nPosNum = nPatternNum;
				dlgSearchToolCopyPos.m_nPosCNum = TARGETCANDIDATENUM;
				dlgSearchToolCopyPos.m_nSrcCandidateIndex = j;
				dlgSearchToolCopyPos.SetSysInfoStringsRef(m_psaSysInfoStrings);

				if (dlgSearchToolCopyPos.DoModal() == IDOK)
				{
					nDstCandidateIndex = dlgSearchToolCopyPos.m_nCandidateIndex;
					int k = 0;
					for (k = 0; k < MAXPOSNUM && k < dlgSearchToolCopyPos.m_vbCopyPos.size(); k++)
					{
						if (dlgSearchToolCopyPos.m_vbCopyPos.at(k) == TRUE && ((k != i)||(nDstCandidateIndex != j)))
						{
							nDstCandidateIndex = dlgSearchToolCopyPos.m_nCandidateIndex;
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
							{
								CString strWarning;
								strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),k+1,nDstCandidateIndex);
								strWarning = strWarning + 
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
									_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
								MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
								return S_OK;

							}

							// 检查存储空间是否超过阈值
							if(1)
							{
								if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
								{
									if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
									{
										double dUsedRate = 0.0;
										CString StrTmp = m_strModuleDir;
										m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

										if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchTool != NULL)
										{
											CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
											CString strTmp2;
											strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
											CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

											AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

											CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
											m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
											// 避免后续无法保存的情况

											return S_OK;

										}
									}
								}
							}

							CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
							CProductData product = pTotalProduct->GetItem(nCurProdIdx);

							// 拷贝源为空模板时，将拷贝目标位置的产品执行清空操作
							if (pSearchTool == NULL)
							{
								CameraParamEnable cameraParamEnable = product.m_pAlignerParam->GetEnableTargetSearchShutter(nDstCandidateIndex);
								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

								if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable)  && bSystemOptionShutter)
								{
									CameraParamSet tmpameraParamSet;
									product.m_pAlignerParam->SetTargetSearchShutter(k,nDstCandidateIndex, tmpameraParamSet);
									pTotalProduct->SetItem(nCurProdIdx, product);
								}

								CBaseSearchTool *pSearchToolDst = NULL;
								pSearchToolDst = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								if (pSearchToolDst == NULL)
								{
									continue;
								}

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->DeleteRepoAlnTargetSearchTool(nCurProdIdx,k,nDstCandidateIndex);
								m_vvpTargetGuiDisplay[k][nDstCandidateIndex]->ClearScreen();

							}
							else
							{
								CameraParamEnable cameraParamEnable = product.m_pAlignerParam->GetEnableTargetSearchShutter(j);
								BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

								if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable)   && bSystemOptionShutter)
								{
									product.m_pAlignerParam->SetTargetSearchShutter(k,nDstCandidateIndex, product.m_pAlignerParam->GetTargetSearchShutter(i,j));
									pTotalProduct->SetItem(nCurProdIdx, product);
								}



								//获得拷贝之前的窗口 
								scGuiDisplay* pOldDisPlay = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPosDisplay(k);

								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnTargetSearchTool(nCurProdIdx, k, nDstCandidateIndex, pSearchTool);

								// 显示模板训练图像
								if (pSearchTool != NULL)
								{
									cpImage imagePattern;
									pSearchTool->GetPatternImage(imagePattern);



									m_vvpTargetGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
									m_vvpTargetGuiDisplay[k][nDstCandidateIndex]->SetImage(imagePattern, TRUE);
								}
								else
								{
									m_vvpTargetGuiDisplay[k][nDstCandidateIndex]->ClearScreen();
								}

								//设置拷贝之前的窗口 
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetPosDisplay(k, pOldDisPlay); 
							}

							
							// 标记指定位置目标模板已经被修改了
							m_pParentKind->m_vvbAlnTargetSearchToolChanged[k][nDstCandidateIndex] = TRUE;
							///////////////////////////////////////////////////////////////////////
						
						
							strLog.Format(_T("将位置%d候选%d目标模板复制到位置%d候选%d\n"),i+1,j,k+1,nDstCandidateIndex);
							g_opLogFile.SaveLogText(strLog,5);


							/////////////////////////////////////////////////////////////////////////
							
						}
					}


					m_bProdPatternChanged = TRUE;
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}

LRESULT CDlgSysKindAlignTarget::OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		return S_OK;
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0,nDstCandidateIndex = 0;
	SysPlatformInfo platformInfo;
	CBaseSearchTool *pSearchTool = NULL;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	int nPatternNum = platformInfo.m_nPositionNum;


	for (i=0; i<nPatternNum; i++)
	{
		for (j=0; j < TARGETCANDIDATENUM; j++)
		{
			if (wParam == m_vvpTargetGuiDisplay[i][j]->GetID())
			{
				CString strLog,strText;
				strLog.Format(_T("点击右键选择[获取定位工具从模板库]\n"));
				g_opLogFile.SaveLogText(strLog,4);
			
				CDlgSearchToolModelLibrary dlgSearchToolModelLibrary;
				dlgSearchToolModelLibrary.m_vpVisionASM= m_pParent->m_vpVisionASM;
				dlgSearchToolModelLibrary.m_nCurPlatformIndex = m_pParentKind->m_nPlatformIndex;
				dlgSearchToolModelLibrary.SetSysInfoStringsRef(m_psaSysInfoStrings);
				dlgSearchToolModelLibrary.EnableSysKeyboard(m_bSysKeyboardEnabled);

				dlgSearchToolModelLibrary.m_nSearchModeFrom = eTargetSearch;
				dlgSearchToolModelLibrary.m_nPosIndexFrom = i;
				dlgSearchToolModelLibrary.m_nCandidatePosFrom = j;

				if (dlgSearchToolModelLibrary.DoModal() == IDOK)
				{
					CBaseSearchTool *pSearchTool =NULL;
					pSearchTool = dlgSearchToolModelLibrary.GetSelectedSearchTool();
					if(NULL == pSearchTool)
					{
						return S_FALSE;
					}

					if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsSysMemoryUsedRateWarning())
					{
						CString strWarning;
						strWarning.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_COPY_SEARCHTOOL_FAIL_OF_MEM_OUT),i+1,j);
						strWarning = strWarning + 
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_REASON) +
							_T("\n\n")+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_SOLUTION1);
						MessageBox(strWarning,GetSysInfoString(m_psaSysInfoStrings,IDS_STR_MEM_OUT_WARNING),MB_ICONEXCLAMATION);
						return S_FALSE;

					}

					// 检查存储空间是否超过阈值
					if(1)
					{
						if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
							{
								double dUsedRate = 0.0;
								CString StrTmp = m_strModuleDir;
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

								// 从其他模板拷贝到空产品时，需要进行资源管控
								CBaseSearchTool *pSearchToolDstTmp = NULL;
								pSearchToolDstTmp = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx,i,j);
								if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) &&  pSearchToolDstTmp == NULL)
								{
									CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
									CString strTmp2;
									strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
									CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

									AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));

									CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
									m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法拷贝模板！"));
									// 避免后续无法保存的情况

									return S_OK;

								}
							}
						}
					}

					bool bRet = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnTargetSearchTool(nCurProdIdx,i,j,pSearchTool);
					if(!bRet)
					{
						return S_FALSE;
					}
					cpImage imagePattern;
					bRet = dlgSearchToolModelLibrary.GetSelectedImage(imagePattern);
					if(!bRet)
					{
						return S_FALSE;
					}
					m_vvpTargetGuiDisplay[i][j]->ClearScreen();
					m_vvpTargetGuiDisplay[i][j]->SetImage(imagePattern);
					
					// 标记产品模板已修改
					m_bProdPatternChanged = TRUE;
					// 标记指定位置目标模板已经被修改了
					m_pParentKind->m_vvbAlnTargetSearchToolChanged[i][j] = TRUE;

					///////////////////////////////////////////////////////////////////////
					dlgSearchToolModelLibrary.GetImageContent(dlgSearchToolModelLibrary.GetSelectImageIndex(),strLog);
					
					strText.Format(_T("将%s复制到产品%d位置%d候选%d\n"),strLog, m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData()->GetCurProductIndex()
						,i+1,j);
					g_opLogFile.SaveLogText(strText,5);


					/////////////////////////////////////////////////////////////////////////
					return S_OK;
				}
				else
				{
					return S_OK;
				}
			}
		}
	}

	return S_OK ;
}

// 双击Display控件响应
LRESULT CDlgSysKindAlignTarget::OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam)
{
	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;
	if (pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCount() <= 0)
	{
		OnButtonNewProduct();
	}

	int nCurProdIdx = m_nCurProductIdx;

	int i = 0, j = 0;
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
					CString strLog;
					if (j == 0)
					{
						strLog.Format(_T("双击[位置%d]目标模板"),i+1);
						g_opLogFile.SaveLogText(strLog,4);
					}
					else
					{
						strLog.Format(_T("双击[位置%d]候选模板%d"),i+1,j);
						g_opLogFile.SaveLogText(strLog,4);
					}

					pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetRepoAlnTargetSearchTool(nCurProdIdx,i,j);

					// 检查存储空间是否超过阈值
					if(1)
					{
						if (m_pParent && m_nPlatformIndex<m_pParent->m_vpVisionASM.size() && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
						{
							if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo!=NULL)
							{
								double dUsedRate = 0.0;
								CString StrTmp = m_strModuleDir;
								m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->m_pSystemStatusInfo->GetDiskUsedRate(StrTmp,dUsedRate);

								if (m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->IsDiskUsedRateWarning(StrTmp) && pSearchTool==NULL)
								{
									CString strStatusBarInfo1 = GetSysInfoString(m_psaSysInfoStrings,IDS_STR_ATTENTION);
									CString strTmp2;
									strTmp2 = StrTmp.Left(StrTmp.Find('\\')+1);
									CString strStatusBarInfo2 = strTmp2+GetSysInfoString(m_psaSysInfoStrings,IDS_STR_DISK_OUT);

									AfxMessageBox(strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！"));

									CString strTempWarningType = GetSysInfoString(m_psaSysInfoStrings, IDS_WARNING_PROCESSING_SEARCHTOOLINVALID);
									m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->AddWarningInfo(strTempWarningType,strStatusBarInfo1 + strStatusBarInfo2 + _T("\n无法新建模板！！"));
									// 避免后续无法保存的情况

									return S_OK;

								}
							}
						}
					}

					LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
					if (m_psaSysInfoStrings != NULL)
					{
						lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
					}
					switch (lgidLanguage)
					{
					case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(0);
						break;
					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
						break;
					case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
					default:
						if(pSearchTool!=NULL) pSearchTool->SetLanguage(1);
						break;
					}

					CDlgSearch dlgSearch(this, pSearchTool);
					dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
					dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
					dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);

					CTotalProductData *pTotalProduct = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetTotalProuductData();
					CProductData product = pTotalProduct->GetItem(nCurProdIdx);

					CameraParamEnable cameraParamEnable = product.m_pAlignerParam->GetEnableTargetSearchShutter(j);
					CPlatformOptionInfo optionInfo;
					if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
					{
						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
					}
					BOOL bSystemOptionShutter = optionInfo.m_bEnableSearchShutter;

					if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
					{
						dlgSearch.SetProductSearchShutterInfo(product.m_pAlignerParam->GetEnableTargetSearchShutter(j) ,product.m_pAlignerParam->GetTargetSearchShutter(i,j));
					}
					
					if (dlgSearch.DoModal() == IDOK)
					{
						if ((cameraParamEnable.m_bShutterEnable || cameraParamEnable.m_bGainEnable || cameraParamEnable.m_bGammaEnable) && bSystemOptionShutter)
						{
							product.m_pAlignerParam->SetTargetSearchShutter(i, j, dlgSearch.GetProductSearchShutterInfoShutter());
							pTotalProduct->SetItem(nCurProdIdx, product);
						}

						pSearchTool = dlgSearch.GetSearchTool();
						m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetRepoAlnTargetSearchTool(nCurProdIdx,i, j, pSearchTool);

						// 显示模板训练图像
						if (pSearchTool != NULL)
						{
							cpImage imagePattern;
							pSearchTool->GetPatternImage(imagePattern);
							
							m_vvpTargetGuiDisplay[i][j]->ClearScreen();
							m_vvpTargetGuiDisplay[i][j]->SetImage(imagePattern, TRUE);
						}
						else
						{
							m_vvpTargetGuiDisplay[i][j]->ClearScreen();
						}
						// 标记产品模板已修改
						m_bProdPatternChanged = TRUE;
						// 标记指定位置目标模板已经被修改了
						m_pParentKind->m_vvbAlnTargetSearchToolChanged[i][j] = TRUE;
					}
					return S_OK;
				}
// 				if (wParam == m_vpTargetGuiDisplay.at(i)->GetID())
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

// 	// Virtual Object mark check
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
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
// 						scImageGray imagePattern;
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
// 	}
// 
// 	// Object mark check
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		for (i=0; i<nPatternNum; i++)
// 		{
// 			if (wParam == m_vpObjectGuiDisplay.at(i)->GetID())
// 			{
// 				pSearchTool = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetAlnObjectSearchToolParam(i);
// 				CDlgSearch dlgSearch(this, pSearchTool);
// 				dlgSearch.SetSysInfoStringsRef(m_psaSysInfoStrings);
// 				dlgSearch.EnableSysKeyboard(m_bSysKeyboardEnabled);
// 				dlgSearch.SetVisionASMRef(m_pParent->m_vpVisionASM.at(m_nPlatformIndex), i);
// 				if (dlgSearch.DoModal() == IDOK)
// 				{
// 					pSearchTool = dlgSearch.GetSearchTool();
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SetAlnObjectSearchToolParam(i, pSearchTool);
// 
// 					// 显示模板训练图像
// 					if (pSearchTool != NULL)
// 					{
// 						scImageGray imagePattern;
// 						pSearchTool->GetPatternImage(imagePattern);
// 						m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 						m_vpObjectGuiDisplay.at(i)->SetImage(imagePattern, TRUE);
// 					}
// 					else
// 					{
// 						m_vpObjectGuiDisplay.at(i)->ClearScreen();
// 					}
// 					// 标记产品模板已修改
// 					m_bProdPatternChanged = TRUE;
// 				}
// 				return S_OK;
// 			}
// 		}
// 
// 	}
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

void CDlgSysKindAlignTarget::OnPlatformChange() 
{
	UpdateData(TRUE);
// 	if (m_nPlatformIndex == m_nPrevPlatformIdx)
// 	{
// 		return;
// 	}
// 
// 	// 保存前一个产品的设置
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
// 			m_nPlatformIndex = m_nPrevPlatformIdx;
// 			UpdateData(FALSE);
// 			return;
// 		}
// 		else // 不保存对产品的修改
// 		{
// 			if (m_bProdNameChanged)
// 			{
// 				CString strOldDir, strNewDir;
// 				CString strCurName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// 				// 				strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, m_strProductNameOld);
// 				// 				strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPrevPlatformIdx, strCurName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
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
// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 				m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 			}
// 			if (m_bProdNewCreate)
// 			{
// 				CString strDir;
// 				CString strProductName = m_vpTotalProductData.at(m_nPrevPlatformIdx)->GetItemName(m_nPrevProductIdx);
// 				//				strDir.Format(_T("\\VSConfig\\WS%d\\Jobs\\%s"), m_nPrevPlatformIdx, strProductName);
// 				{
// 					SysPlatformInfo platformInfo;
// 					vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx);
// 					pVisionASM->GetSysPlatformInfo(platformInfo);
// 					strDir.Format(_T("%s\\Jobs\\%s"), platformInfo.m_strPlatformDetailSavePath, strProductName);
// 				}
// 				strDir = m_strModuleDir + strDir;
// 				if (!RemoveDirectoryPro(strDir))
// 				{
// 					//AfxMessageBox(_T("删除产品数据文件失败！"));
// 					AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 				}
// 				m_vpTotalProductData.at(m_nPrevPlatformIdx)->RemoveItem(m_nPrevProductIdx);
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
// 	else
// 	{
// 		// 保存产品概要信息到磁盘
// 		m_pParent->m_vpVisionASM.at(m_nPrevPlatformIdx)->SaveTotalProductDataInfo(FALSE);
// 	}

	//////////////////////////////////////////////////////////////////////////
	// 修改新选择的平台的显示界面
	SysPlatformInfo platformInfo;
	int nPosNum = 0, i = 0, j = 0;
	m_pParent->m_SystemConfigure.GetSysPlatformInfo(m_nPlatformIndex, platformInfo);
	nPosNum = m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetCameraNum();
	CPlatformOptionInfo optionInfo;
	if (m_pParent && m_pParent->m_vpVisionASM.at(m_nPlatformIndex))
	{
		m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->GetPlatformOptionInfo(optionInfo);
	}
//	nPosNum = (nPosNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	nPosNum = platformInfo.m_nPositionNum;
	// 目标Mark界面修改
	if (platformInfo.m_eAlignerTargetType == eTargetOnline)
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			if (i < nPosNum)
			{
				// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
				// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_SHOW);


				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(0,255,0));
				m_vpLabelPos.at(i)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);


				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(m_pParentKind->m_bShowTargetTemplateSet);
				}
			}
			else
			{
				// 				GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
				// 				m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);

				if (i < MAXPOSNUM -1)
				{
					GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
				}
				m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
				m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

				for(j = 0; j < TARGETCANDIDATENUM; j++)
				{
					GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
					m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
				}
			}
		}
		//		GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		EnableAndShowControl(optionInfo.m_bEnableSearchShutter,optionInfo.m_bEnableSearchShutter);
	}
	else
	{
		for (i=0; i<MAXPOSNUM; i++)
		{
			// 			GetDlgItem(IDC_STATIC_TARGET_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TARGET_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
			// 			m_vpTargetGuiDisplay.at(i)->ShowWindow(SW_HIDE);


			if (i < MAXPOSNUM -1)
			{
				GetDlgItem(IDC_STATIC_SEPARATOR3+ i)->ShowWindow(SW_HIDE);
			}
			m_vpLabelPos.at(i)->SetBkColor(RGB(100,100,100));
			m_vpLabelPos.at(i)->ShowWindow(SW_HIDE);

			for(j = 0; j < TARGETCANDIDATENUM; j++)
			{
				GetDlgItem(IDC_STATIC_TARGETC_PATTERN0+i*TARGETCANDIDATENUM+j)->ShowWindow(SW_HIDE);
				m_vvpTargetGuiDisplay[i][j]->ShowWindow(SW_HIDE);
			}
		}
		// 		if (platformInfo.m_bBenchMarkAmend)
		// 		{
		// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		// 		}
		// 		else
		// 		{
		// 			GetDlgItem(IDC_BTN_AMEND_BENCHMARK)->ShowWindow(SW_HIDE);
		// 		}

		EnableAndShowControl(false,false);
	}
// 
// 	// 虚拟对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<6; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_VIRTUAL_OBJ_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<6; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_VIRTUAL_OBJ_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 			m_vpVirtualObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
// 	}
// 
// 	// 对象Mark界面修改
// 	if (platformInfo.m_eAlignerObjectType != eObjectBench)
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_SHOW);
// 		for (i=0; i<6; i++)
// 		{
// 			if (i < nPosNum)
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_SHOW);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_SHOW);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_SHOW);
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+i*2)->ShowWindow(SW_HIDE);
// 				GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+i*2)->ShowWindow(SW_HIDE);
// 				m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_STATIC_GRP_OBJECT_PATTERN)->ShowWindow(SW_HIDE);
// 		for (i=0; i<6; i++)
// 		{
// 			GetDlgItem(IDC_STATIC_OBJECT_DISPLAY0+(i*2))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_OBJECT_PATTERN0+(i*2))->ShowWindow(SW_HIDE);
// 			m_vpObjectGuiDisplay.at(i)->ShowWindow(SW_HIDE);
// 		}
// 	}
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

	UpdateDlgData(FALSE);
//	UpdateProductListDisplay();

	CDlgSysKindAlignView *pView = m_pParentKind->m_pDlgKindAlignView;;
	m_nPlatformIndex = pView->m_nPlatformIndex;

	m_nPrevPlatformIdx = m_nPlatformIndex;
	m_nCurProductIdx = pView->m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
	m_nPrevProductIdx = m_nCurProductIdx;


// 
// 	m_nPrevPlatformIdx = m_nPlatformIndex;
// 	m_nCurProductIdx = m_vpTotalProductData.at(m_nPlatformIndex)->GetCurProductIndex();
// 	m_nPrevProductIdx = m_nCurProductIdx;
// 	m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerMaxTimes() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_MAX_TIMES)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_MAX_TIMES, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerMaxTimes();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerXPrecision() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_X_PRECISION)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerXPrecision();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerYPrecision() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_Y_PRECISION)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerYPrecision();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerDPrecision() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_D_PRECISION)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerDPrecision();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerXOffset() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_X_OFFSET)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerXOffset();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerYOffset() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_Y_OFFSET)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerYOffset();
// 	}	
}

void CDlgSysKindAlignTarget::OnSetfocusEditAlignerDOffset() 
{
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_ALIGNER_D_OFFSET)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strOutput);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnKillfocusEditAlignerDOffset();
// 	}	
}



void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateXOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateXOffset1();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateYOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateYOffset1();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateDOffset1() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选1][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateDOffset1();
	}	
}



void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateXOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateXOffset2();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateYOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateYOffset2();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateDOffset2() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选2][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateDOffset2();
	}	
}



void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateXOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateXOffset3();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateYOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateYOffset3();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateDOffset3() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选3][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateDOffset3();
	}	
}




void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateXOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][X]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateXOffset4();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateYOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][Y]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateYOffset4();
	}	
}


void CDlgSysKindAlignTarget::OnSetfocusEditTargetCandidateDOffset4() 
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParentKind->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[候选4][D]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditTargetCandidateDOffset4();
	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerMaxTimes() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		UpdateData(TRUE);
// 		m_nAlignerMaxTimes = (m_nAlignerMaxTimes<1) ? 1 : ((m_nAlignerMaxTimes>10) ? 10 : m_nAlignerMaxTimes);
// 		UpdateData(FALSE);
// 	}
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerXPrecision() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
// 		}
// 		else
// 		{
// 			m_strAlignerPrecisionX.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_PRECISION, m_strAlignerPrecisionX);
// 		}
// 	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerYPrecision() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
// 		}
// 		else
// 		{
// 			m_strAlignerPrecisionY.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_PRECISION, m_strAlignerPrecisionY);
// 		}
// 	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerDPrecision() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
// 		}
// 		else
// 		{
// 			m_strAlignerPrecisionD.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_PRECISION, m_strAlignerPrecisionD);
// 		}
// 	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerXOffset() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
// 		}
// 		else
// 		{
// 			m_strAlignerOffsetX.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_X_OFFSET, m_strAlignerOffsetX);
// 		}
// 	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerYOffset() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
// 		}
// 		else
// 		{
// 			m_strAlignerOffsetY.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_Y_OFFSET, m_strAlignerOffsetY);
// 		}
// 	}	
}

void CDlgSysKindAlignTarget::OnKillfocusEditAlignerDOffset() 
{
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, strVal);
// 		if (!IsStringNumerical(strVal))
// 		{
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
// 		}
// 		else
// 		{
// 			m_strAlignerOffsetD.Format(_T("%.6f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_ALIGNER_D_OFFSET, m_strAlignerOffsetD);
// 		}
// 	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateXOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, m_strTargetCandidateOffsetX1);
		}
		else
		{
			m_strTargetCandidateOffsetX1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET1, m_strTargetCandidateOffsetX1);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateYOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, m_strTargetCandidateOffsetY1);
		}
		else
		{
			m_strTargetCandidateOffsetY1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET1, m_strTargetCandidateOffsetY1);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateDOffset1() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, m_strTargetCandidateOffsetD1);
		}
		else
		{
			m_strTargetCandidateOffsetD1.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET1, m_strTargetCandidateOffsetD1);
		}
	}	
}



void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateXOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, m_strTargetCandidateOffsetX2);
		}
		else
		{
			m_strTargetCandidateOffsetX2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET2, m_strTargetCandidateOffsetX2);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateYOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, m_strTargetCandidateOffsetY2);
		}
		else
		{
			m_strTargetCandidateOffsetY2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET2, m_strTargetCandidateOffsetY2);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateDOffset2() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, m_strTargetCandidateOffsetD2);
		}
		else
		{
			m_strTargetCandidateOffsetD2.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET2, m_strTargetCandidateOffsetD2);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateXOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, m_strTargetCandidateOffsetX3);
		}
		else
		{
			m_strTargetCandidateOffsetX3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET3, m_strTargetCandidateOffsetX3);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateYOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, m_strTargetCandidateOffsetY3);
		}
		else
		{
			m_strTargetCandidateOffsetY3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET3, m_strTargetCandidateOffsetY3);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateDOffset3() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, m_strTargetCandidateOffsetD3);
		}
		else
		{
			m_strTargetCandidateOffsetD3.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET3, m_strTargetCandidateOffsetD3);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateXOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, m_strTargetCandidateOffsetX4);
		}
		else
		{
			m_strTargetCandidateOffsetX4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_X_OFFSET4, m_strTargetCandidateOffsetX4);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateYOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, m_strTargetCandidateOffsetY4);
		}
		else
		{
			m_strTargetCandidateOffsetY4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_Y_OFFSET4, m_strTargetCandidateOffsetY4);
		}
	}	
}


void CDlgSysKindAlignTarget::OnKillfocusEditTargetCandidateDOffset4() 
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, strVal);
		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, m_strTargetCandidateOffsetD4);
		}
		else
		{
			m_strTargetCandidateOffsetD4.Format(_T("%.6f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_ALIGNER_TARGETC_D_OFFSET4, m_strTargetCandidateOffsetD4);
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
void CDlgSysKindAlignTarget::OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult) 
{
// 	POSITION posSel = m_listctrlProduct.GetFirstSelectedItemPosition();
// 	int nSelected = m_listctrlProduct.GetNextSelectedItem(posSel);
// 	if (nSelected < 0)
// 	{
// 		m_listctrlProduct.SetItemState(m_nCurProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 	}
// 	else
// 	{
// 		m_nCurProductIdx = nSelected;
// 	}
// 
// 	if ((m_nCurProductIdx >= 0) && (m_nCurProductIdx != m_nPrevProductIdx))
// 	{
// 		// 保存前一个产品的设置
// 		if (IsProductChanged())
// 		{
// 			int nMBReturn = AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_SAVE_CHANGE_TO_PRODUCT_OR_NOT), MB_YESNOCANCEL);	//_T("是否保存对当前产品的修改？")
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
// 				m_listctrlProduct.SetItemState(m_nCurProductIdx, 0x0000, LVIS_SELECTED);
// 				m_listctrlProduct.SetItemState(m_nPrevProductIdx, LVIS_SELECTED, LVIS_SELECTED);
// 				m_nCurProductIdx = m_nPrevProductIdx;
// 				return;
// 			}
// 			else // 不保存对产品的修改
// 			{
// 				if (m_bProdNameChanged)
// 				{
// 					CString strOldDir, strNewDir;
// 					CString strCurName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					// 					strOldDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, m_strProductNameOld);
// 					// 					strNewDir.Format(_T("%s\\VSConfig\\WS%d\\Jobs\\%s"), m_strModuleDir, m_nPlatformIndex, strCurName);
// 					{
// 						SysPlatformInfo platformInfo;
// 						vcBaseVisionAlign *pVisionASM = m_pParent->m_vpVisionASM.at(m_nPlatformIndex);
// 						pVisionASM->GetSysPlatformInfo(platformInfo);
// 						strOldDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, m_strProductNameOld);
// 						strNewDir.Format(_T("%s%s\\Jobs\\%s"), m_strModuleDir, platformInfo.m_strPlatformDetailSavePath, strCurName);
// 					}
// 					// 如果文件夹存在，则重命名文件夹
// 					DWORD dwAttributes = GetFileAttributes(strNewDir);
// 					if ((dwAttributes != 0xFFFFFFFF) && ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
// 					{
// 						if (strCurName != m_strProductNameOld)
// 						{						
// 							if (!RenameDirecotryPro(strNewDir, strOldDir))
// 							{
// 								//AfxMessageBox(_T("重命名失败！"));
// 								AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_RENAME_FAILED));
// 							}
// 						}
// 					}
// 
// 					m_vpTotalProductData.at(m_nPlatformIndex)->SetItemName(m_nPrevProductIdx, m_strProductNameOld);
// 					m_pParent->m_vpVisionASM.at(m_nPlatformIndex)->SaveProductDataInfo(m_nPrevProductIdx);
// 					//					UpdateDlgData(FALSE);
// 				}
// 				if (m_bProdNewCreate)
// 				{
// 					CString strDir;
// 					CString strProductName = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nPrevProductIdx);
// 					strDir.Format(_T("\\VSConfig\\Platform%d\\Jobs\\%s"), m_nPlatformIndex, strProductName);
// 					strDir = m_strModuleDir + strDir;
// 					if (!RemoveDirectoryPro(strDir))
// 					{
// 						//AfxMessageBox(_T("删除产品数据文件失败！"));
// 						AfxMessageBox(GetSysInfoString(m_psaSysInfoStrings,IDS_MB_FAIL_TO_DELETE_PRODUCT_DATA_FILE));
// 					}
// 					m_vpTotalProductData.at(m_nPlatformIndex)->RemoveItem(m_nPrevProductIdx);
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
// 
// 				// 恢复修改标志
// 				m_bProdPatternChanged = FALSE;
// 				m_bProdNameChanged = FALSE;
// 				m_bProdBenchMarkChanged = FALSE;
// 				m_bProdNewCreate = FALSE;
// 				m_bCalibInfoChanged = FALSE;
// 			}
// 		}
// 
// 		m_vpTotalProductData.at(m_nPlatformIndex)->SetCurProductIndex(m_nCurProductIdx);
// 		m_strProductNameOld = m_vpTotalProductData.at(m_nPlatformIndex)->GetItemName(m_nCurProductIdx);
// 		m_nPrevProductIdx = m_nCurProductIdx;
// 		UpdateDlgData(FALSE);
// 		m_listctrlProduct.SetItemState(m_nCurProductIdx, /*LVIS_FOCUSED | */LVIS_SELECTED, /*LVIS_FOCUSED|*/LVIS_SELECTED);
// 	}

	*pResult = 0;
}

BOOL CDlgSysKindAlignTarget::DestroyWindow() 
{
	int i = 0 , j = 0;
//	int nSize = m_vpTargetGuiDisplay.size();
// 	for (i=0; i<nSize; i++)
// 	{
// 		delete m_vpTargetGuiDisplay.at(i);
// 		m_vpTargetGuiDisplay.at(i) = NULL;
// 	}
// 	m_vpTargetGuiDisplay.clear();

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

// 	nSize = m_vpObjectGuiDisplay.size();
// 	for (i=0; i<nSize; i++)
// 	{
// 		delete m_vpObjectGuiDisplay.at(i);
// 		m_vpObjectGuiDisplay.at(i) = NULL;
// 	}
// 	m_vpObjectGuiDisplay.clear();
// 
// 	nSize = m_vpVirtualObjectGuiDisplay.size();
// 	for (i=0; i<nSize; i++)
// 	{
// 		delete m_vpVirtualObjectGuiDisplay.at(i);
// 		m_vpVirtualObjectGuiDisplay.at(i) = NULL;
// 	}
// 	m_vpVirtualObjectGuiDisplay.clear();
// 
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

	return CDialog::DestroyWindow();
}

void CDlgSysKindAlignTarget::OnOK() 
{
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

void CDlgSysKindAlignTarget::OnCancel() 
{
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

void CDlgSysKindAlignTarget::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}


void CDlgSysKindAlignTarget::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}


BOOL CDlgSysKindAlignTarget::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchModeChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nTargetCandidateEnableOld = m_nTargetCandidateEnable;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nTargetCandidateEnableOld != m_nTargetCandidateEnable)
	{
		CString strLog;
		strLog.Format(_T("修改[候选搜索]由%s到%s\n"),strVal[nTargetCandidateEnableOld],strVal[m_nTargetCandidateEnable]);
		g_opLogFile.SaveLogText(strLog,4);
	}

}

void CDlgSysKindAlignTarget::OnBnClickedRadioTargetcSearchSyncChange()
{
	// TODO: 在此添加控件通知处理程序代码
	int nTargetCandidateSyncOld = m_nTargetCandidateSync;
	UpdateData(TRUE);

	CString strVal[] = {_T("关"), _T("开")};
	if (nTargetCandidateSyncOld != m_nTargetCandidateSync)
	{
		CString strLog;
		strLog.Format(_T("修改[成对搜索]由%s到%s\n"),strVal[nTargetCandidateSyncOld],strVal[m_nTargetCandidateSync]);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable1()
{
	// TODO: 在此添加控件通知处理程序代码m_bTargetCandidateOffsetEnable1
	BOOL bTargetCandidateOffsetEnable1Old = m_bTargetCandidateOffsetEnable1;

	UpdateData(TRUE);
	if (bTargetCandidateOffsetEnable1Old != m_bTargetCandidateOffsetEnable1)
	{
		CString strLog;
		CString strValue = m_bTargetCandidateOffsetEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable2()
{
	// TODO: 在此添加控件通知处理程序代码m_bTargetCandidateOffsetEnable2
	BOOL bTargetCandidateOffsetEnable2Old = m_bTargetCandidateOffsetEnable2;

	UpdateData(TRUE);
	if (bTargetCandidateOffsetEnable2Old != m_bTargetCandidateOffsetEnable2)
	{
		CString strLog;
		CString strValue = m_bTargetCandidateOffsetEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable3()
{
	// TODO: 在此添加控件通知处理程序代码m_bTargetCandidateOffsetEnable3
	BOOL bTargetCandidateOffsetEnable3Old = m_bTargetCandidateOffsetEnable3;

	UpdateData(TRUE);
	if (bTargetCandidateOffsetEnable3Old != m_bTargetCandidateOffsetEnable3)
	{
		CString strLog;
		CString strValue = m_bTargetCandidateOffsetEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}


void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcOffsetEnable4()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bTargetCandidateOffsetEnable4Old = m_bTargetCandidateOffsetEnable4;

	UpdateData(TRUE);
	if (bTargetCandidateOffsetEnable4Old != m_bTargetCandidateOffsetEnable4)
	{
		CString strLog;
		CString strValue = m_bTargetCandidateOffsetEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[候选4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcShutterEnable()
{
	// TODO: 在此添加控件通知处理程序代码
    BOOL bAlignerTargetShutterEnable0Old = m_bTargetShutterEnable0;
    BOOL bAlignerTargetShutterEnable1Old = m_bTargetShutterEnable1;
    BOOL bAlignerTargetShutterEnable2Old = m_bTargetShutterEnable2;
    BOOL bAlignerTargetShutterEnable3Old = m_bTargetShutterEnable3;
    BOOL bAlignerTargetShutterEnable4Old = m_bTargetShutterEnable4;
	UpdateData(TRUE);
    if (bAlignerTargetShutterEnable0Old != m_bTargetShutterEnable0)
    {
        CString strLog;
        CString strValue = m_bTargetShutterEnable0 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerTargetShutterEnable1Old != m_bTargetShutterEnable1)
    {
        CString strLog;
        CString strValue = m_bTargetShutterEnable1 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置1]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerTargetShutterEnable2Old != m_bTargetShutterEnable2)
    {
        CString strLog;
        CString strValue = m_bTargetShutterEnable2 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置2]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerTargetShutterEnable3Old != m_bTargetShutterEnable3)
    {
        CString strLog;
        CString strValue = m_bTargetShutterEnable3 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置3]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
    if (bAlignerTargetShutterEnable4Old != m_bTargetShutterEnable4)
    {
        CString strLog;
        CString strValue = m_bTargetShutterEnable4 ? _T("选中状态"):_T("未选中状态");
        strLog.Format(_T("修改[曝光设置4]为%s\n"),strValue);
        g_opLogFile.SaveLogText(strLog,4);
    }
}

void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGainEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerTargetGainEnable0Old = m_bTargetGainEnable0;
	BOOL bAlignerTargetGainEnable1Old = m_bTargetGainEnable1;
	BOOL bAlignerTargetGainEnable2Old = m_bTargetGainEnable2;
	BOOL bAlignerTargetGainEnable3Old = m_bTargetGainEnable3;
	BOOL bAlignerTargetGainEnable4Old = m_bTargetGainEnable4;
	UpdateData(TRUE);
	if (bAlignerTargetGainEnable0Old != m_bTargetGainEnable0)
	{
		CString strLog;
		CString strValue = m_bTargetGainEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGainEnable1Old != m_bTargetGainEnable1)
	{
		CString strLog;
		CString strValue = m_bTargetGainEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGainEnable2Old != m_bTargetGainEnable2)
	{
		CString strLog;
		CString strValue = m_bTargetGainEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGainEnable3Old != m_bTargetGainEnable3)
	{
		CString strLog;
		CString strValue = m_bTargetGainEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGainEnable4Old != m_bTargetGainEnable4)
	{
		CString strLog;
		CString strValue = m_bTargetGainEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[增益设置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignTarget::OnBnClickedCheckAlignerTargetcGammaEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bAlignerTargetGammaEnable0Old = m_bTargetGammaEnable0;
	BOOL bAlignerTargetGammaEnable1Old = m_bTargetGammaEnable1;
	BOOL bAlignerTargetGammaEnable2Old = m_bTargetGammaEnable2;
	BOOL bAlignerTargetGammaEnable3Old = m_bTargetGammaEnable3;
	BOOL bAlignerTargetGammaEnable4Old = m_bTargetGammaEnable4;
	UpdateData(TRUE);
	if (bAlignerTargetGammaEnable0Old != m_bTargetGammaEnable0)
	{
		CString strLog;
		CString strValue = m_bTargetGammaEnable0 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGammaEnable1Old != m_bTargetGammaEnable1)
	{
		CString strLog;
		CString strValue = m_bTargetGammaEnable1 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置1]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGammaEnable2Old != m_bTargetGammaEnable2)
	{
		CString strLog;
		CString strValue = m_bTargetGammaEnable2 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置2]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGammaEnable3Old != m_bTargetGammaEnable3)
	{
		CString strLog;
		CString strValue = m_bTargetGammaEnable3 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置3]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
	if (bAlignerTargetGammaEnable4Old != m_bTargetGammaEnable4)
	{
		CString strLog;
		CString strValue = m_bTargetGammaEnable4 ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Gamma设置4]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,4);
	}
}

void CDlgSysKindAlignTarget::EnableWindowByAuthorizeset(bool bEnable)
{
	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		pWnd->EnableWindow(bEnable);
		pWnd = pWnd->GetNextWindow();
	}
}