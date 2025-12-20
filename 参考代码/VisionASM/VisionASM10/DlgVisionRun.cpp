// DlgVisionRun.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionRun.h"
#include "afxdialogex.h"
#include "DlgCalcuVirtualObject.h"
#include "DlgCalibTargetPos.h"
#include "DlgCalibObjectPos.h"
#include "DlgCalibObjectPos8.h"
#include "DlgRobotTeach.h"
#include "DlgRobotTeachRange.h"
#include "DlgRobotUserLogin.h"
#include "DlgCalcuBenchPick.h"
#include "DlgCalibTargetPos8.h"
#include "DlgCalibTargetPos4Board.h"
#include "DlgCalibTargetPos8Board.h"
#include "DlgCalibTargetPos2.h"
#include "DlgCalibObjectPos2Board.h"
#include "DlgCalcuVirtualObject4.h"
#include "DlgCalibCheckBySize.h"
#include "DlgCalibCheckByFixedTargetAligner.h"

#include "DlgVisionRunProductInfo.h"
#include "DlgVisionRunAlignInfo.h"
#include "DlgVisionRunCommInfo.h"
#include "DlgVisionRunBendInspectInfo.h"

// CDlgVisionRun 对话框

IMPLEMENT_DYNAMIC(CDlgVisionRun, CDialog)

CDlgVisionRun::CDlgVisionRun(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVisionRun::IDD, pParent)
	
{
	m_strProductName = _T("产品名称:");
	m_strProductIndex = _T("产品序号:");

	m_pParent = (CVisionASMDlg*)pParent;
	m_pVisionASM = NULL;


	m_nCombocProductIndex = 0;
	m_nPlatformID = -1;

	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;


	m_pDlgVisionRunAlignInfo = NULL;
	m_pDlgVisionRunCommInfo = NULL;
	m_pDlgVisionRunProductInfo = NULL;
	m_pDlgVisionRunBendInspectInfo = NULL;

	m_pMainLayout = NULL;
	m_pTabLayout = NULL;

// 	m_pDlgCalibCheckBySize = NULL;
// 	m_pDlgCalibCheckByFixedTargetAligner = NULL;

//	m_bUpdatelogCommCommand = FALSE;
// 	m_strLogCommandDetail = _T("");
// 	m_strLogCommandSimple = _T("");
//	m_bSpecial1Platform = FALSE;
//	m_pStatusBar = NULL;
// 	m_nCamNum = 2;
// 	m_nRow = 1;
// 	m_nCol = 2;
// 	m_nAlignerInfoRow = 4;
// 	m_nAlignerInfoHeight = 0;
//  m_hIconFailed = NULL;
//  m_hIconSucceed = NULL;
//  m_strTimeCost = _T("0");
	//, m_nRadioCommandMode(0)
}

CDlgVisionRun::~CDlgVisionRun()
{
	m_MyFont.DeleteObject();



	if (m_pMainLayout)
	{
		delete m_pMainLayout;
		m_pMainLayout = NULL;
	}

	if (m_pTabLayout)
	{
		delete m_pTabLayout;
		m_pTabLayout = NULL;
	}

	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->DestroyWindow();
		delete m_pDlgVisionRunAlignInfo;
		m_pDlgVisionRunAlignInfo = NULL;
	}

	if (m_pDlgVisionRunCommInfo)
	{
		m_pDlgVisionRunCommInfo->DestroyWindow();
		delete m_pDlgVisionRunCommInfo;
		m_pDlgVisionRunCommInfo = NULL;
	}

	if (m_pDlgVisionRunProductInfo != NULL)
	{
		delete m_pDlgVisionRunProductInfo;
		m_pDlgVisionRunProductInfo = NULL;
	}

	if (m_pDlgVisionRunBendInspectInfo)
	{
		m_pDlgVisionRunBendInspectInfo->DestroyWindow();
		delete m_pDlgVisionRunBendInspectInfo;
		m_pDlgVisionRunBendInspectInfo = NULL;
	}
}

void CDlgVisionRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, m_comboProduct);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_FA, m_comboProductFa);
	DDX_Text(pDX, IDC_STATIC_VAL_PRODUCT_INDEX, m_strProductIndex);
	DDX_Control(pDX, IDC_TAB_VISION_RUN_INFO, m_tabVisionRunInfo);
}


BEGIN_MESSAGE_MAP(CDlgVisionRun, CDialog)
	//ON_MESSAGE(WM_DLGDESTROY, &CDlgVisionRun::OnDlgdestroy)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT, &CDlgVisionRun::OnCbnSelchangeComboProduct)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_FA, &CDlgVisionRun::OnCbnSelchangeComboProductFa)
	ON_BN_CLICKED(IDC_BTN_LOAD_PRODUCT, &CDlgVisionRun::OnBnClickedBtnLoadProduct)
	ON_BN_CLICKED(IDC_BTN_SINGLE_PLATFORM_PRODUCT_MANAGE, &CDlgVisionRun::OnBnClickedBtnSinglePlatformProductManage)
	ON_WM_TIMER()
	//ON_MESSAGE(WM_CALCSIZE, &CDlgVisionRun::OnCalcProductSize)
	ON_BN_CLICKED(IDC_RADIO_OFFSET_DIRECTION, &CDlgVisionRun::OnBnClickedRadioOffsetDirection)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VISION_RUN_INFO, &CDlgVisionRun::OnTcnSelchangeTabVisionRunInfo)
END_MESSAGE_MAP()


// CDlgVisionRun 消息处理程序


void CDlgVisionRun::InitUILayout()
{
	m_pMainLayout = new CVerticalLayout;

	// 产品信息栏
	CLayout* pProductBar = new CHorizontalLayout;
	pProductBar->AddWnd(GetDlgItem(IDC_STATIC_VAL_PRODUCT_INDEX), 65, 0, CLayout::eSizePolicy_Fixed);
	pProductBar->AddWnd(GetDlgItem(IDC_COMBO_PRODUCT_FA), 0, 100, CLayout::eSizePolicy_Expand);
	pProductBar->AddWnd(GetDlgItem(IDC_COMBO_PRODUCT), 0, 100, CLayout::eSizePolicy_Expand);
	pProductBar->AddWnd(GetDlgItem(IDC_BTN_SINGLE_PLATFORM_PRODUCT_MANAGE), 110, 0, CLayout::eSizePolicy_Fixed);
// 	CLayout* pSpring = new CHorizontalLayout;
// 	pProductBar->AddLayout(pSpring, 0, 0, CLayout::eSizePolicy_Expand);

	m_pMainLayout->AddLayout(pProductBar, 0, 30, CLayout::eSizePolicy_Fixed);
	// 分割线
	m_pMainLayout->AddWnd(GetDlgItem(IDC_STATIC_SEPARATOR5), 0, 3, CLayout::eSizePolicy_Fixed);
	// 对位信息区
	m_pMainLayout->AddWnd(GetDlgItem(IDC_TAB_VISION_RUN_INFO), 0, 0, CLayout::eSizePolicy_Expand);

	m_pMainLayout->SetParent(this);
}

void CDlgVisionRun::InitUIShow()
{
	UpdateDialogLanguage();
	SetItemFont();
	InitTabUI();
}

void CDlgVisionRun::InitTabUI()
{

	 m_pTabLayout = new CHorizontalLayout;

	 CString strTmp1 = _T("对位信息");
	 CString strTmp2 = _T("通信信息");
	 CString strTmp3 = _T("产品信息");
	 CString strTmp4 = _T("折弯信息");

	 if(m_pParent->m_psaSysInfoStrings != NULL)
	 {
		  strTmp1 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_VISION_RUN_ALIGN_INFO);
		  strTmp2 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_VISION_RUN_COMM_INFO);
		  strTmp3 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_VISION_RUN_PRODUCT_INFO);
		  strTmp4 = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_VISION_RUN_BEND_INSPECT_INFO);
	 }
	

	 // 对位信息窗口
	 m_tabVisionRunInfo.InsertItem(0, strTmp1);
	 m_pDlgVisionRunAlignInfo = new CDlgVisionRunAlignInfo;
	 m_pDlgVisionRunAlignInfo->SetParentWnd(this);
	 m_pDlgVisionRunAlignInfo->Create(IDD_DLG_VISION_RUN_ALIGN_INFO, &m_tabVisionRunInfo);
	 m_pDlgVisionRunAlignInfo->ShowWindow(SW_SHOW);
	 m_pTabLayout->AddWnd(m_pDlgVisionRunAlignInfo, 0 , 0 ,CLayout::eSizePolicy_Expand, 3, 20, 3, 3);
	
	 // 通信信息窗口
	 m_tabVisionRunInfo.InsertItem(1, strTmp2);
	 m_pDlgVisionRunCommInfo = new CDlgVisionRunCommInfo;
	 m_pDlgVisionRunCommInfo->SetParentWnd(this);
	 m_pDlgVisionRunCommInfo->Create(IDD_DLG_VISION_RUN_COMM_INFO, &m_tabVisionRunInfo);
	 m_pDlgVisionRunCommInfo->ShowWindow(SW_HIDE);
	 m_pTabLayout->AddWnd(m_pDlgVisionRunCommInfo, 0 , 0 ,CLayout::eSizePolicy_Expand, 3, 20, 3, 3);

	 // 产品信息窗口
	 m_tabVisionRunInfo.InsertItem(2, strTmp3);
	 m_pDlgVisionRunProductInfo = new CDlgVisionRunProductInfo;
	 m_pDlgVisionRunProductInfo->SetParentWnd(this);
	 m_pDlgVisionRunProductInfo->Create(IDD_DLG_VISION_RUN_PRODUCT_INFO, &m_tabVisionRunInfo);
	 m_pDlgVisionRunProductInfo->ShowWindow(SW_HIDE);
	 m_pTabLayout->AddWnd(m_pDlgVisionRunProductInfo, 0 , 0 ,CLayout::eSizePolicy_Expand, 3, 20, 3, 3);
	
	 // 折弯检测
	 m_tabVisionRunInfo.InsertItem(3, strTmp4);
	 m_pDlgVisionRunBendInspectInfo = new CDlgVisionRunBendInspectInfo;
	 m_pDlgVisionRunBendInspectInfo->SetParentWnd(this);
	 m_pDlgVisionRunBendInspectInfo->Create(IDD_DLG_VISION_RUN_BEND_INSPECT_INFO, &m_tabVisionRunInfo);
	 m_pDlgVisionRunBendInspectInfo->ShowWindow(SW_HIDE);
	 m_pTabLayout->AddWnd(m_pDlgVisionRunBendInspectInfo, 0 , 0 ,CLayout::eSizePolicy_Expand, 3, 20, 3, 3);


	 m_pTabLayout->SetParent(&m_tabVisionRunInfo);
	 m_pTabLayout->OnSize();
}

BOOL CDlgVisionRun::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitUILayout();
	InitUIShow();

	UpdateDlgParamItem();
	UpdateDlgProductInfo();

	SetTimer(0,1000,NULL);	//hk20180130

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgVisionRun::SetItemFont()
{
	m_MyFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));					// lpszFacename


	GetDlgItem(IDC_COMBO_PRODUCT)->SetFont(&m_MyFont);
	GetDlgItem(IDC_COMBO_PRODUCT_FA)->SetFont(&m_MyFont);
}


int CDlgVisionRun::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CDlgVisionRun::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::DestroyWindow();
}


LRESULT CDlgVisionRun::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_pVisionASM != NULL)
	{
		switch (message)
		{
		case WM_UPDATE_ALIGNER_PROCESS_INFO:
			{
				UpdateAlignerProcessInfo();
			}
			break;
		case WM_UPDATE_CUR_PRODUCT_ALIGN_INFO:
			{
				UpdateCurProductAlignInfo();
			}
			break;
		case WM_UPDATE_ALIGN_PRECISION:
			{
				UpdateAlignPrecisionInfo();
			}
		case WM_UPDATE_ALIGN_OFFSET:
			{
				UpdateAlignOffsetInfo();
			}
			break;

		case WM_UPDATE_COMM_COMMAND_INFO:
			{
				GetCommCommandInfo();
			}
			break;

		case WM_UPDATE_BEND_INSPECT_RESULT:
			{
				if (m_pDlgVisionRunBendInspectInfo != NULL)
				{
					//m_pVisionASM->GetBendInspectResult(m_pDlgVisionRunBendInspectInfo->m_BendInspectResult);
					//m_pVisionASM->GetAlignerProcessInfo(m_pDlgVisionRunBendInspectInfo->m_apAlignerProcesInfo);
					m_pVisionASM->GetBendInspectResultLine(m_pDlgVisionRunBendInspectInfo->m_BendInspectResult);
					m_pVisionASM->GetAlignerProcessInfoLine(m_pDlgVisionRunBendInspectInfo->m_apAlignerProcesInfo);

					m_pDlgVisionRunBendInspectInfo->UpdateBendBaseResult();
					m_pDlgVisionRunBendInspectInfo->UpdateBendResultDetal();
				}

			}
			break;


		case WM_UPDATE_BEND_CIRCLE_RESULT:
			{
				if (m_pDlgVisionRunBendInspectInfo != NULL)
				{
					m_pVisionASM->GetBendInspectResult(m_pDlgVisionRunBendInspectInfo->m_BendInspectResult);
					m_pVisionASM->GetAlignerProcessInfo(m_pDlgVisionRunBendInspectInfo->m_apAlignerProcesInfo);
					m_pDlgVisionRunBendInspectInfo->UpdateCircleResultSimple();
					m_pDlgVisionRunBendInspectInfo->UpdateBendResultDetal();
				}

			}
			break;

		case WM_UPDATE_BEND_DETAL_RESULT:
			{
				if (m_pDlgVisionRunBendInspectInfo != NULL)
				{
					m_pVisionASM->GetBendInspectResult(m_pDlgVisionRunBendInspectInfo->m_BendInspectResult);
					m_pVisionASM->GetAlignerProcessInfo(m_pDlgVisionRunBendInspectInfo->m_apAlignerProcesInfo);
					m_pDlgVisionRunBendInspectInfo->UpdateBendResultDetal();
				}

			}
			break;


			// 检测模式
		case WM_UPDATE_ALIGNER_TARGET_SIZE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerTargetSizeInfo(m_pDlgVisionRunProductInfo->m_AlignerTargetSizeInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerTargetSizeInfo();
				}

				if (m_pDlgVisionRunAlignInfo)
				{
					m_pDlgVisionRunAlignInfo->UpdateCheckAlignerTargetSizeInfo();
				}				
			}
			break;
		case WM_UPDATE_ALIGNER_OBJECT_SIZE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerObjectSizeInfo(m_pDlgVisionRunProductInfo->m_AlignerObjectSizeInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerObjectSizeInfo();
				}

				if (m_pDlgVisionRunAlignInfo)
				{
					m_pDlgVisionRunAlignInfo->UpdateCheckAlignerObjectSizeInfo();
				}
			}
			break;

		case WM_UPDATE_ALIGNER_TARGET_ANGLE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerTargetAngleInfo(m_pDlgVisionRunProductInfo->m_AlignerTargetAngleInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerTargetAngleInfo();
				}			
			}
			break;
		case WM_UPDATE_ALIGNER_OBJECT_ANGLE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerObjectAngleInfo(m_pDlgVisionRunProductInfo->m_AlignerObjectAngleInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerObjectAngleInfo();
				}
			}
			break;

			// 输入模式
		case WM_UPDATE_ALIGNER_TARGET_STD_SIZE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerTargetStdSizeInfo(m_pDlgVisionRunProductInfo->m_AlignCheckInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerTargetStdSizeInfo();
				}
			}
			break;
		case WM_UPDATE_ALIGNER_OBJECT_STD_SIZE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerObjectStdSizeInfo(m_pDlgVisionRunProductInfo->m_AlignCheckInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerObjectStdSizeInfo();
				}
			}
			break;
		case WM_UPDATE_ALIGNER_TARGET_STD_ANGLE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerTargetStdAngleInfo(m_pDlgVisionRunProductInfo->m_AlignCheckInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerTargetStdAngleInfo();
				}
			}
			break;
		case WM_UPDATE_ALIGNER_OBJECT_STD_ANGLE_INFO:
			{				
				if(m_pDlgVisionRunProductInfo != NULL)
				{
					m_pVisionASM->GetAlignerObjectStdSizeInfo(m_pDlgVisionRunProductInfo->m_AlignCheckInfo);
					m_pDlgVisionRunProductInfo->UpdateAlignerObjectStdAngleInfo();
				}
			}
			break;
		default:
			break;
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CDlgVisionRun::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgVisionRun::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}

	if (m_pTabLayout)
	{
		m_pTabLayout->OnSize();
	}

	//MoveDlgItem();
}





CWnd* CDlgVisionRun::SetParentWnd( CWnd* pParent )
{
	CWnd* pPrevParent = m_pParent;
	m_pParent = (CVisionASMDlg*)pParent;
	return pPrevParent;
}



void CDlgVisionRun::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	m_pVisionASM = pVisionASM;

	// 设置对位信息VisionASM
	m_pDlgVisionRunAlignInfo->SetVisionASMRef(pVisionASM);
	// 设置通信信息VIsionASM
	m_pDlgVisionRunCommInfo->SetVisionASMRef(pVisionASM);
	// 设置产品信息VisionASM
	m_pDlgVisionRunProductInfo->SetVisionASMRef(pVisionASM);
	// 设置检测信息VisionASM
	m_pDlgVisionRunBendInspectInfo->SetVisionASMRef(pVisionASM);
	// 设置其他信息VisionASM

	// 更新当前产品信息
	UpdateCurProductAlignInfo();
	//UpdateCrossInfo();

	// 加载补偿提示图片
	m_pDlgVisionRunAlignInfo->LoadOffsetDirectionPicture();
}

void CDlgVisionRun::SetPlatformID( LPCTSTR strID )
{
	m_strPlatformID = strID;
	//UpdateStatusBar(m_bStatusBarFlag, NULL, NULL);
}

void CDlgVisionRun::SetPlatformID(int nID)
{
	m_nPlatformID = nID;
}

int CDlgVisionRun::GetPlatformID()
{
	return m_nPlatformID;
}




void CDlgVisionRun::EnableSysKeyboard(BOOL bEnable)
{
	m_bSysKeyboardEnabled = bEnable;

	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->SetKeybordEnable(bEnable);
	}

	if (m_pDlgVisionRunProductInfo != NULL)
	{
		m_pDlgVisionRunProductInfo->EnableSysKeyboard(bEnable);
	}

	if (m_pDlgVisionRunBendInspectInfo != NULL)
	{
		m_pDlgVisionRunBendInspectInfo->EnableSysKeyboard(bEnable);
	}

}

void CDlgVisionRun::UpdateCurProductAlignInfo()
{
	// 更新产品显示
	UpdateDlgData();


	// 更新当前产品对位信息
	if (m_pDlgVisionRunAlignInfo != NULL)
	{
		m_pDlgVisionRunAlignInfo->UpdateAllAlignInfo();
	}

	// 更新当前产品通信信息
	if (m_pDlgVisionRunCommInfo != NULL)
	{
		m_pDlgVisionRunCommInfo->UpdateLogCommCommand();
	}

	// 更新当前产品尺寸信息
	if (m_pDlgVisionRunProductInfo != NULL)
	{
		m_pDlgVisionRunProductInfo->UpdateCurProductInfo();
		m_pDlgVisionRunProductInfo->UpdateAlignerTargetStdSizeInfo();
		m_pDlgVisionRunProductInfo->UpdateAlignerObjectStdSizeInfo();
	}

	if (m_pDlgVisionRunBendInspectInfo != NULL)
	{
		m_pDlgVisionRunBendInspectInfo->UpdateBendBaseInfo(FALSE);
		m_pDlgVisionRunBendInspectInfo->UpdateBendBaseResult();

		m_pDlgVisionRunBendInspectInfo->UpdateCircleBaseInfo(FALSE);
		m_pDlgVisionRunBendInspectInfo->UpdateCircleResultSimple();
	}
}


void CDlgVisionRun::UpdateDlgData()
{
	m_strProductIndex = GetSysInfoString(m_pParent->m_psaSysInfoStrings,IDS_STR_CUR_PRODUCT);
	m_strProductName = _T("");
	if (m_pVisionASM != NULL)
	{
		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();

		CString strTemp;
		CString strTemp1;
		int nProductNum = pTotalProdData->GetCount();
		m_comboProductFa.ResetContent();
		m_comboProduct.ResetContent();

		int nProductIndexFa, nProductIndexChd;
		pTotalProdData->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);

		int nFaProductNum = pTotalProdData->GetProductCountFa();
		for(int i=0;i<nFaProductNum;i++)//第i个大产品
		{
			strTemp.Format(_T("%03d|%s"), i+1,pTotalProdData->GetProductNameFa(i));			
			m_comboProductFa.InsertString(i, strTemp);
		}
		m_comboProductFa.SetCurSel(nProductIndexFa);

		int nProductNumChd = pTotalProdData->GetProductCountChd(nProductIndexFa);
		for(int i=0;i<nProductNumChd;i++)
		{
			int nIndex = pTotalProdData->GetProductIndex(nProductIndexFa,i);
			strTemp.Format(_T("%03d|%s"), i+1,pTotalProdData->GetItemName(nIndex));			
			m_comboProduct.InsertString(i, strTemp);
		}
		m_comboProduct.SetCurSel(nProductIndexChd);

	}
	UpdateData(FALSE);
}

// 
void CDlgVisionRun::UpdateAlignPrecisionInfo()
{
	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->UpdateAlignPrecisionInfo();
	}
}

void CDlgVisionRun::UpdateAlignOffsetInfo()
{
	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->UpdateAlignOffsetInfo();
	}
}


void CDlgVisionRun::UpdateAlignerProcessInfo()
{
	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->UpdateAlignerProcessInfo();
	}
}

void CDlgVisionRun::GetCommCommandInfo()
{
	if (m_pDlgVisionRunCommInfo)
	{
		m_pDlgVisionRunCommInfo->GetCommCommandInfo();
	}
}



void CDlgVisionRun::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_VISION_RUN") == strDlgID)
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

	//m_labelProductIndex.SetText(GetDlgCtrlString(IDC_STATIC_VAL_PRODUCT_INDEX));
}

void CDlgVisionRun::UpdateDlgProductInfo()
{
	//	return;
	//	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);

	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		if (m_pVisionASM != NULL)
		{
			SysPlatformInfo platformInfo;
			m_pVisionASM->GetSysPlatformInfo(platformInfo);
			if (platformInfo.m_bProductFaChd)
			{
				GetDlgItem(IDC_COMBO_PRODUCT)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_COMBO_PRODUCT)->ShowWindow(SW_HIDE);
			}

			// 相机共享， 产品设置按钮隐藏， 原因位置
			if (m_pVisionASM->IsShareCameraPlatform())	
			{
				GetDlgItem(IDC_BTN_SINGLE_PLATFORM_PRODUCT_MANAGE)->ShowWindow(SW_HIDE);

			}
			else
			{
				GetDlgItem(IDC_BTN_SINGLE_PLATFORM_PRODUCT_MANAGE)->ShowWindow(SW_SHOW);

			}


			// 更新窗口显示
			// 更新对位信息窗口显示
			if (m_pDlgVisionRunAlignInfo)
			{
				m_pDlgVisionRunAlignInfo->SetDlgItemVisible();
				m_pDlgVisionRunAlignInfo->UpdateDialogLanguage();
			}
			// 更新通信信息窗口显示
			if(m_pDlgVisionRunCommInfo)
			{
				m_pDlgVisionRunCommInfo->UpdateDialogLanguage();
			}


			// 更新产品信息窗口显示
			if (m_pDlgVisionRunProductInfo)
			{
				m_pDlgVisionRunProductInfo->UpdateDlgParamItem();
				
				m_pDlgVisionRunProductInfo->UpdateDialogLanguage();
				m_pDlgVisionRunProductInfo->UpdateDlgItemText();
			}

			if(m_pDlgVisionRunBendInspectInfo)
			{
				if (3 == m_tabVisionRunInfo.GetCurSel())
				{
					m_pDlgVisionRunBendInspectInfo->UpdateDlgParamItem();
					m_pDlgVisionRunBendInspectInfo->ShowWindow(m_pDlgVisionRunBendInspectInfo->m_bShowBendDlg);
				}
			
				m_pDlgVisionRunBendInspectInfo->UpdateDialogLanguage();
			}

			if(m_pParent->m_psaSysInfoStrings != NULL)
			{
				int nArrID[] = {IDS_VISION_RUN_ALIGN_INFO, IDS_VISION_RUN_COMM_INFO, IDS_VISION_RUN_PRODUCT_INFO};
				TCITEM tci;
				char szTmpBuf[MAX_PATH] = {0};	
				for (int i = 0; i < m_tabVisionRunInfo.GetItemCount() && i < sizeof(nArrID)/sizeof(int); i++)
				{
					tci.mask = TCIF_TEXT;

					memset(szTmpBuf, 0, MAX_PATH);
					tci.pszText = szTmpBuf;
					tci.cchTextMax = MAX_PATH;
					m_tabVisionRunInfo.GetItem(i, &tci);

					tci.pszText = (LPSTR)(LPCSTR) GetSysInfoString(m_pParent->m_psaSysInfoStrings,nArrID[i]);
					m_tabVisionRunInfo.SetItem(i,&tci);

				}
			}
		}
	}



}


void CDlgVisionRun::OnCbnSelchangeComboProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
	int nProductIndexFa, nProductIndexChd;
	pTotalProdData->GetCurDlgProductIndex(nProductIndexFa, nProductIndexChd);
	int nCombocProductIndexOld = nProductIndexChd;	
	CString strCombocProductIndexOld, m_strCombocProductIndex;
	m_comboProduct.GetLBText(nCombocProductIndexOld, strCombocProductIndexOld);
	m_comboProduct.GetWindowText(m_strCombocProductIndex);

	m_nCombocProductIndex = m_comboProduct.GetCurSel();
	OnBnClickedBtnLoadProduct();
	if (nCombocProductIndexOld != m_nCombocProductIndex)
	{
		CString strLog;
		strLog.Format(_T("主界面修改[当前产品]工位%d产品%d由%s到%s\n"),m_nPlatformID+1,nProductIndexChd+1,strCombocProductIndexOld,m_strCombocProductIndex);
		g_opLogFile.SaveLogText(strLog,1);
	}
}

void CDlgVisionRun::OnCbnSelchangeComboProductFa()
{
	CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
	int nFaProductIndex = m_comboProductFa.GetCurSel();
	//获取该大品种上一次选中的小品种
	int nLastChdIndex = pTotalProdData->GetLastChdIndex(nFaProductIndex);
	CString strTemp;

	m_comboProduct.ResetContent();
	int nChdProductNum = pTotalProdData->GetProductCountChd(nFaProductIndex);
	for(int i=0;i<nChdProductNum;i++)
	{
		int nIndex = pTotalProdData->GetProductIndex(nFaProductIndex,i);
		strTemp.Format(_T("%03d|%s"), i+1,pTotalProdData->GetItemName(nIndex));			
		m_comboProduct.InsertString(i, strTemp);
	}
	m_comboProduct.SetCurSel(nLastChdIndex);
	OnBnClickedBtnLoadProduct();
}

void CDlgVisionRun::OnBnClickedBtnLoadProduct()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[加载新产品]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	//将m_nCombocProductIndex转化为产品的大小品种索引，再变成内存索引
	CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
	int nProductIndexFa = m_comboProductFa.GetCurSel();
	int nProductIndexChd = m_comboProduct.GetCurSel();
	int nProductIndex = pTotalProdData->GetProductIndex(nProductIndexFa, nProductIndexChd);

	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->SetCurCalibDataIndex(nProductIndex);

		if (m_pVisionASM->SetCurProductIndex(nProductIndex))
		{
			UpdateCurProductAlignInfo();
			m_pVisionASM->SetMutiTarget();
		}		
	}
	m_pVisionASM->SetProductCameraShutter(nProductIndex);
}


void CDlgVisionRun::OnBnClickedRadioProductInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[产品信息]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	UpdateData(TRUE);

	//OnRadioCommandSelChanged();
}

void CDlgVisionRun::EnableDlgItem(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_PRODUCT)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_PRODUCT_FA)->EnableWindow(bEnable);

	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->EnableDlgItem(bEnable);
	}
}
void CDlgVisionRun::UpdateUserType(int nUserType, BOOL bEnable)
{
	if (m_pDlgVisionRunAlignInfo)
	{
		m_pDlgVisionRunAlignInfo->EnableDlgItem(bEnable);
	}
}

void CDlgVisionRun::UpdateDlgParamItem()
{
	if (m_pDlgVisionRunAlignInfo != NULL)
	{
		m_pDlgVisionRunAlignInfo->UpdateDlgParamItem();
		// 加载补偿示意图
		m_pDlgVisionRunAlignInfo->LoadOffsetDirectionPicture();
	}

	if (m_pDlgVisionRunProductInfo != NULL)
	{
		m_pDlgVisionRunProductInfo->UpdateDlgParamItem();
	}

	if(m_pDlgVisionRunBendInspectInfo)
	{
		m_pDlgVisionRunBendInspectInfo->UpdateDlgParamItem();
	}
	
	Invalidate();
}



CString CDlgVisionRun::GetDlgCtrlString(int nCtrlID)
{
	CString strCtrl;
	strCtrl.Empty();
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir = m_pParent->m_strCurWorkDir;

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

		if(_T("IDD_DLG_VISION_RUN") == strDlgID)
		{

			CString strValue = xml.GetAttribute(_T("Value"));
			//SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				int nID;
				xml.GetAttribute(_T("Id"),  nID);
				if (nID == nCtrlID)
				{
					strCtrl = xml.GetAttribute(_T("Value"));

					return strCtrl;
				}


			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();

	return strCtrl;

}



void CDlgVisionRun::OnBnClickedBtnSinglePlatformProductManage()
{
	// TODO: 在此添加控件通知处理程序代码
	CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
	int nCurProIdx = pTotalProdData->GetCurProductIndex();
	CString strLog;
	strLog.Format(_T("点击主界面[产品设置]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	//strLog.Format(_T("对工位%d产品%d[产品浏览]进行设置\n"),m_nPlatformID+1,nCurProIdx+1);
	//g_opLogFile.SaveLogText(strLog,2);

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		m_pParent->ExecuteSinglePlatformProductManage(sysPlatformInfo.m_nPlatformIndex);

		if (optionInfo.m_bEnableSearchShutter == TRUE)
		{
			for (int j=0; j<m_pVisionASM->GetCameraNum(); j++)
			{
				// 确保实时采集为环境设置中的曝光时间
				Sleep(10);
				m_pVisionASM->UpdateCameraShutter(j);
			}
		}

	}
}




void CDlgVisionRun::OnTimer(UINT_PTR nIDEvent)		//hk20180130
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( (nIDEvent == 0) && (m_pVisionASM != NULL) )
	{
		// 		if (m_bUpdatelogCommCommand)	//hk20180130
		// 		{
		// 			m_bUpdatelogCommCommand = FALSE;
		// 			UpdateLogCommCommand();
		// 		}
		//m_pVisionASM->GetCommCommandInfo(m_saCommCommandSimple, m_saCommCommandDetail);
		//UpdateLogCommCommand();
	}

	CDialog::OnTimer(nIDEvent);
}




void CDlgVisionRun::OnTcnSelchangeTabVisionRunInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_tabVisionRunInfo.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		m_pDlgVisionRunProductInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunCommInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunBendInspectInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunAlignInfo->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pDlgVisionRunProductInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunAlignInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunBendInspectInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunCommInfo->ShowWindow(SW_SHOW);
		break;
	case 2:
		m_pDlgVisionRunAlignInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunCommInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunBendInspectInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunProductInfo->ShowWindow(SW_SHOW);
		break;
	case 3:
		m_pDlgVisionRunAlignInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunCommInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunProductInfo->ShowWindow(SW_HIDE);
		m_pDlgVisionRunBendInspectInfo->UpdateDlgParamItem();
		m_pDlgVisionRunBendInspectInfo->ShowWindow(m_pDlgVisionRunBendInspectInfo->m_bShowBendDlg);
		break;
	}

	if (m_pTabLayout)
	{
		m_pTabLayout->OnSize();
	}
	*pResult = 0;
}


void CDlgVisionRun::SetRunCommInfoShow()
{
	m_pDlgVisionRunProductInfo->ShowWindow(SW_HIDE);
	m_pDlgVisionRunAlignInfo->ShowWindow(SW_HIDE);
	m_pDlgVisionRunBendInspectInfo->ShowWindow(SW_HIDE);
	m_pDlgVisionRunCommInfo->ShowWindow(SW_SHOW);
	m_tabVisionRunInfo.SetCurSel(1);
	if (m_pTabLayout)
	{
		m_pTabLayout->OnSize();
	}

	m_pDlgVisionRunCommInfo->SetSimpleCommShow();
}

void CDlgVisionRun::OnBnClickedRadioOffsetDirection()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_nRadioCommandMode=3;
	//OnRadioCommandSelChanged();
}