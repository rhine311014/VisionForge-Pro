// DlgVisionRunAlignInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionRunAlignInfo.h"
#include "afxdialogex.h"
#include "VerticalLayout.h"
#include "HorizontalLayout.h"
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
#include "DlgCalibObjectPos2Code.h"
#include "DlgCalibCopy.h"

#include "DlgAlignOffsetByAreaSet.h"

// CDlgVisionRunAlignInfo 对话框

IMPLEMENT_DYNAMIC(CDlgVisionRunAlignInfo, CDialogEx)

CDlgVisionRunAlignInfo::CDlgVisionRunAlignInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVisionRunAlignInfo::IDD, pParent)
	, m_strEditGrip(0)
{
	m_pMainLayout = NULL;
	m_pVisionASM = NULL;
	m_pParent = NULL;
	m_pDlgCalibCheckBySize = NULL;
	m_pDlgCalibCheckByFixedTargetAligner = NULL;
	m_bKillFocusEnabled = TRUE;
	m_bSysKeyboardEnabled = FALSE;
	m_hBmpOffsetDir = NULL;


	m_strEditCurOffsetIndex = _T("1");
	m_strEditCurOffsetX = _T("0.0000");
	m_strEditCurOffsetY = _T("0.0000");
	m_strEditCurOffsetD = _T("0.0000");
	m_strEditTargetPrecisionMaxTimes = _T("1");
	m_strEditTargetPrecisionD = _T("0.0000");
	m_strEditTargetPrecisionX = _T("0.0000");
	m_strEditTargetPrecisionY = _T("0.0000");
	m_strEditTargetPrecisionD2 = _T("0.0000");
	m_strEditTargetPrecisionX2 = _T("0.0000");
	m_strEditTargetPrecisionY2 = _T("0.0000");
	m_strEditCurPrecisionD = _T("0.0000");
	m_strEditCurPrecisionX = _T("0.0000");
	m_strEditCurPrecisionY = _T("0.0000");
	m_strEditAlignTimes = _T("0");
	m_strEditAxisPosD = _T("0.0000");
	m_strEditAxisPosX = _T("0.0000");
	m_strEditAxisPosY = _T("0.0000");
	m_strEditINSMode = _T("四点独立");
	m_strEditSanDianMode = _T("不启用");
}

CDlgVisionRunAlignInfo::~CDlgVisionRunAlignInfo()
{
	if (m_pMainLayout)
	{
		delete m_pMainLayout;
		m_pMainLayout = NULL;
	}

	if (m_pDlgCalibCheckBySize)
	{
		m_pDlgCalibCheckBySize->DestroyWindow();
		delete m_pDlgCalibCheckBySize;
		m_pDlgCalibCheckBySize = NULL;
	}

	if (m_pDlgCalibCheckByFixedTargetAligner)
	{
		m_pDlgCalibCheckByFixedTargetAligner->DestroyWindow();
		delete m_pDlgCalibCheckByFixedTargetAligner;
		m_pDlgCalibCheckByFixedTargetAligner = NULL;
	}

	if (m_hBmpOffsetDir)
	{
		DeleteObject(m_hBmpOffsetDir);
		m_hBmpOffsetDir = NULL;

	}


}

void CDlgVisionRunAlignInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OFFSET_INDEX, m_strEditCurOffsetIndex);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OFFSET_X, m_strEditCurOffsetX);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OFFSET_Y, m_strEditCurOffsetY);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OFFSET_D, m_strEditCurOffsetD);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, m_strEditTargetPrecisionMaxTimes);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_D, m_strEditTargetPrecisionD);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_X, m_strEditTargetPrecisionX);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_Y, m_strEditTargetPrecisionY);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_D2, m_strEditTargetPrecisionD2);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_X2, m_strEditTargetPrecisionX2);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_PRECISION_Y2, m_strEditTargetPrecisionY2);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_PRECISION_D, m_strEditCurPrecisionD);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_PRECISION_X, m_strEditCurPrecisionX);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_PRECISION_Y, m_strEditCurPrecisionY);
	DDX_Text(pDX, IDC_EDIT_VAL_ALIGN_TIMES, m_strEditAlignTimes);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_POS_X, m_strEditAxisPosX);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_POS_Y, m_strEditAxisPosY);
	DDX_Text(pDX, IDC_EDIT_VAL_AXIS_POS_D, m_strEditAxisPosD);
	DDX_Text(pDX, IDC_EDIT_VAL_INS_MODE, m_strEditINSMode);
	DDX_Text(pDX, IDC_EDIT_VAL_SAN_DIAN_MODE, m_strEditSanDianMode);
	DDX_Radio(pDX, IDC_RADIO_GRIP1, m_strEditGrip);
}


BEGIN_MESSAGE_MAP(CDlgVisionRunAlignInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgVisionRunAlignInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgVisionRunAlignInfo::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalcuVirtualObjectPos)
	
	ON_BN_CLICKED(IDC_BTN_CALIB_TARGET_POS, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalibTargetPos)
	ON_BN_CLICKED(IDC_BTN_ROBOT_TEACH, &CDlgVisionRunAlignInfo::OnBnClickedBtnRobotTeach)
	ON_BN_CLICKED(IDC_BTN_ROBOT_TEACH_RANGE, &CDlgVisionRunAlignInfo::OnBnClickedBtnRobotTeachRange)
	ON_BN_CLICKED(IDC_BTN_CALCU_BENCH_PICK, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalcuBenchPick)
	ON_BN_CLICKED(IDC_BTN_CALIB_OBJECT_POS, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalibObjectPos)
	ON_BN_CLICKED(IDC_BTN_CALIB_CHECK_BY_SIZE, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCheckBySize)
	ON_BN_CLICKED(IDC_BTN_RELEVANCE_CHECK_BY_SIZE, &CDlgVisionRunAlignInfo::OnBnClickedBtnRelevanceCheckBySize)
	ON_BN_CLICKED(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCheckByFixedTarget)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_CUR_OFFSET_X, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetX)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CUR_OFFSET_X, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetX)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CUR_OFFSET_Y, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetY)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CUR_OFFSET_Y, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetY)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_CUR_OFFSET_D, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetD)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CUR_OFFSET_D, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetD)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_D, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionD)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_D, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionD)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_X, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionX)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_X, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionX)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_Y, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionY)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_Y, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionY)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_D2, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionD2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_D2, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionD2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_X2, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionX2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_X2, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionX2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_Y2, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionY2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_Y2, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionY2)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_CUR_OFFSET_INDEX, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetIndex)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_CUR_OFFSET_INDEX, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetIndex)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, &CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionMaxTimes)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, &CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionMaxTimes)
	ON_MESSAGE(WM_DLGDESTROY, &CDlgVisionRunAlignInfo::OnDlgdestroy)
	ON_MESSAGE(WM_CALCSIZE, &CDlgVisionRunAlignInfo::OnCalcProductSize)

	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET, &CDlgVisionRunAlignInfo::OnBnClickedBtnAlignOffsetByAreaSet)
	ON_BN_CLICKED(IDC_RADIO_GRIP1, &CDlgVisionRunAlignInfo::OnBnClickedRadEditGrip0)
	ON_BN_CLICKED(IDC_RADIO_GRIP2, &CDlgVisionRunAlignInfo::OnBnClickedRadEditGrip1)
	ON_BN_CLICKED(IDC_BTN_CALIB_COPY, &CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCopy)
END_MESSAGE_MAP()


// CDlgVisionRunAlignInfo 消息处理程序


void CDlgVisionRunAlignInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDlgVisionRunAlignInfo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}



void CDlgVisionRunAlignInfo::SetItemFont()
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


	GetDlgItem(IDC_EDIT_VAL_ALIGN_TIMES)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_AXIS_POS_X)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_AXIS_POS_Y)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_AXIS_POS_D)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_PRECISION_X)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_PRECISION_Y)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_PRECISION_D)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_INDEX)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_X)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_Y)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_D)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_INS_MODE)->SetFont(&m_MyFont);
	GetDlgItem(IDC_EDIT_VAL_SAN_DIAN_MODE)->SetFont(&m_MyFont);
}


void CDlgVisionRunAlignInfo::SetVisionASMRef(vcBaseVisionAlign* pVisionASM)
{
	m_pVisionASM = pVisionASM;
}


void CDlgVisionRunAlignInfo::SetParentWnd(CDlgVisionRun* pParentWnd)
{
	m_pParent = pParentWnd;
}

void CDlgVisionRunAlignInfo::SetKeybordEnable(bool bKeyboardEnable)
{
    m_bSysKeyboardEnabled = bKeyboardEnable;
}

void CDlgVisionRunAlignInfo::UpdateDlgParamItem()
{
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bMainframeSetAlignParam == TRUE)
	{
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_INDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_D)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_INDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_D)->EnableWindow(FALSE);
	}
	CPlatformOptionInfo optionInfo;
	if (m_pVisionASM != NULL)
	{
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	}
	//m_bEnableCommMultiCalibExtension
	
//	if (optionInfo.m_bShowPCB == TRUE)
	if (0)
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D3)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_CUR_MODE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_INS_MODE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_CUR_MODE2)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_SAN_DIAN_MODE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRP_TARGET_PRECISION3)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D3)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_CUR_MODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_INS_MODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_CUR_MODE2)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_SAN_DIAN_MODE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_GRP_TARGET_PRECISION3)->ShowWindow(FALSE);
	}

	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT)->SetWindowText(_T("°"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT)->SetWindowText(_T("°"));
	}
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->SetWindowText(_T("°"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->SetWindowText(_T("°"));
	}

	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
	{
		GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("um"));
		GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
	}
	else
	{
		GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("mm"));
		GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
	}
}


void CDlgVisionRunAlignInfo::UpdateAllAlignInfo()
{
	UpdateAlignOffsetInfo();		// 更新补偿信息
	UpdateAlignPrecisionInfo();		// 更新对位精度设置
//	UpdateAlignPrecisionInfo2();	// 更新对位精度设置2（PCB)
	UpdateAlignerProcessInfo();		// 更新平台运动量、产品偏差量
}


void CDlgVisionRunAlignInfo::UpdateAlignOffsetInfo()
{
	//小数位数格式化
	CString strXY,strD;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
	strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);

	m_strEditCurOffsetIndex= _T("1");
	m_strEditCurOffsetX = _T("0.0000");
	m_strEditCurOffsetY = _T("0.0000");
	m_strEditCurOffsetD = _T("0.0000");

	if (m_pVisionASM != NULL)
	{
		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		int nCurProIdx = pTotalProdData->GetCurProductIndex();
		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
		{
			CProductData* pProduct = pTotalProdData->GetItemPtr(nCurProIdx);
			CCoordPos pos;


			if (m_strEditGrip)
			{
			   /* if (nCurProIdx<11)
			    {
					nCurProIdx = nCurProIdx +10;
					pProduct = pTotalProdData->GetItemPtr(nCurProIdx);
			    }
				*/

				// 获取当前设置的补偿信息
				pos = pProduct->m_pAlignerParam->GetAlignerOffset();
				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
				{
					m_strEditCurOffsetX.Format(strXY, PRECISIONXYUNIT*pos.GetPosX());
					m_strEditCurOffsetY.Format(strXY, PRECISIONXYUNIT*pos.GetPosY());
					m_strEditCurOffsetD.Format(strD, PRECISIONDUNIT*pos.GetAngle());
					GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("um"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("um"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
				}
				else
				{
					m_strEditCurOffsetX.Format(_T("%.4f"), pos.GetPosX());
					m_strEditCurOffsetY.Format(_T("%.4f"), pos.GetPosY());
					m_strEditCurOffsetD.Format(_T("%.4f"), pos.GetAngle());
					GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("mm"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("mm"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
				}


				if (pProduct->m_pAlignerParam->GetCurAlignerOffsetIndex()<10)
				{
					m_strEditCurOffsetIndex.Format(_T("%d"), pProduct->m_pAlignerParam->GetCurAlignerOffsetIndex()+1);
				}
				else
				{
					m_strEditCurOffsetIndex.Format(_T("%d"), pProduct->m_pAlignerParam->GetCurAlignerOffsetIndex()-9);
				}
				
			} 
			else
			{
				
				// 获取当前设置的补偿信息
				pos = pProduct->m_pAlignerParam->GetAlignerOffset();
				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
				{
					m_strEditCurOffsetX.Format(strXY, PRECISIONXYUNIT*pos.GetPosX());
					m_strEditCurOffsetY.Format(strXY, PRECISIONXYUNIT*pos.GetPosY());
					m_strEditCurOffsetD.Format(strD, PRECISIONDUNIT*pos.GetAngle());
					GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("um"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("um"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
				}
				else
				{
					m_strEditCurOffsetX.Format(_T("%.4f"), pos.GetPosX());
					m_strEditCurOffsetY.Format(_T("%.4f"), pos.GetPosY());
					m_strEditCurOffsetD.Format(_T("%.4f"), pos.GetAngle());
					GetDlgItem(IDC_STATIC_CUR_OFFSET_X_UNIT)->SetWindowText(_T("mm"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_Y_UNIT)->SetWindowText(_T("mm"));
					GetDlgItem(IDC_STATIC_CUR_OFFSET_D_UNIT)->SetWindowText(_T("°"));
				}


				m_strEditCurOffsetIndex.Format(_T("%d"), pProduct->m_pAlignerParam->GetCurAlignerOffsetIndex()+1);
			}
			

		}
	}
	UpdateData(FALSE);
}



void CDlgVisionRunAlignInfo::UpdateAlignPrecisionInfo()
{
	//小数位数格式化
	CString strXY,strD;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
	strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);

	m_strEditTargetPrecisionX = _T("0.0000");
	m_strEditTargetPrecisionY = _T("0.0000");
	m_strEditTargetPrecisionD = _T("0.0000");

	if (m_pVisionASM != NULL)
	{
		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		int nCurProIdx = pTotalProdData->GetCurProductIndex();
		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
		{
			CProductData* pProduct = pTotalProdData->GetItemPtr(nCurProIdx);
			CCoordPos pos;

			int nMaxAlignTimes = pProduct->m_pAlignerParam->GetAlignerMaxTime();
			pos = pProduct->m_pAlignerParam->GetAlignerPrecision();

			m_strEditTargetPrecisionMaxTimes.Format(_T("%d"), nMaxAlignTimes);

			if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
			{
				m_strEditTargetPrecisionX.Format(strXY, PRECISIONXYUNIT*pos.GetPosX());
				m_strEditTargetPrecisionY.Format(strXY, PRECISIONXYUNIT*pos.GetPosY());
				m_strEditTargetPrecisionD.Format(strD, PRECISIONDUNIT*pos.GetAngle());
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT)->SetWindowText(_T("um"));
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT)->SetWindowText(_T("um"));
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT)->SetWindowText(_T("°"));
			}
			else
			{
				m_strEditTargetPrecisionX.Format(_T("%.4f"), pos.GetPosX());
				m_strEditTargetPrecisionY.Format(_T("%.4f"), pos.GetPosY());
				m_strEditTargetPrecisionD.Format(_T("%.4f"), pos.GetAngle());
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT)->SetWindowText(_T("mm"));
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT)->SetWindowText(_T("mm"));
				GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT)->SetWindowText(_T("°"));
			}

		}
	}

	UpdateData(FALSE);
}


// void CDlgVisionRunAlignInfo::UpdateAlignPrecisionInfo2()
// {
// 
// 	m_strEditTargetPrecisionX2 = _T("0.0000");
// 	m_strEditTargetPrecisionY2 = _T("0.0000");
// 	m_strEditTargetPrecisionD2 = _T("0.0000");
// 	m_strEditINSMode = _T("四点独立");
// 	m_strEditSanDianMode = _T("不启用");
// 
// 	//小数位数格式化
// 	CString strXY,strD;
// 	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
// 	strD.Format(_T("%%.%df"),PRECISIONDDOTNUM);
// 
// 	if (m_pVisionASM != NULL)
// 	{
// 		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
// 		int nCurProIdx = pTotalProdData->GetCurProductIndex();
// 		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
// 		{
// 			CProductData* pProduct = pTotalProdData->GetItemPtr(nCurProIdx);
// 			CCoordPos pos;
// 
// 			pos = pProduct->m_pAlignerParam->m_PCBInfo.m_PCBInsPre;
// 			if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
// 			{
// 				m_strEditTargetPrecisionX2.Format(strXY, PRECISIONXYUNIT*pos.GetPosX());
// 				m_strEditTargetPrecisionY2.Format(strXY, PRECISIONXYUNIT*pos.GetPosY());
// 				m_strEditTargetPrecisionD2.Format(strD, PRECISIONDUNIT*pos.GetAngle());
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->SetWindowText(_T("um"));
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->SetWindowText(_T("um"));
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->SetWindowText(_T("°"));
// 			}
// 			else
// 			{
// 				m_strEditTargetPrecisionX2.Format(_T("%.4f"), pos.GetPosX());
// 				m_strEditTargetPrecisionY2.Format(_T("%.4f"), pos.GetPosY());
// 				m_strEditTargetPrecisionD2.Format(_T("%.4f"), pos.GetAngle());
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_X_UNIT2)->SetWindowText(_T("mm"));
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_Y_UNIT2)->SetWindowText(_T("mm"));
// 				GetDlgItem(IDC_STATIC_TARGET_PRECISION_D_UNIT2)->SetWindowText(_T("°"));
// 			}
// 
// 
// 			if (pProduct->m_pAlignerParam->m_PCBInfo.m_bShowPCB4To3Align)
// 			{
// 				m_strEditSanDianMode = _T("启用");
// 			}
// 			else
// 			{
// 				m_strEditSanDianMode = _T("不启用");
// 			}
// 			if (pProduct->m_pAlignerParam->m_PCBInfo.m_nPCBInsPreType == 0)
// 			{
// 				m_strEditINSMode = _T("中心对位");
// 			}
// 			else if (pProduct->m_pAlignerParam->m_PCBInfo.m_nPCBInsPreType == 1)
// 			{
// 				m_strEditINSMode = _T("四点独立");
// 			}
// 			else
// 			{
// 				m_strEditINSMode = _T("三点独立");
// 			}
// 
// 		}
// 
// 	}
// 
// 	UpdateData(FALSE);
// }


void CDlgVisionRunAlignInfo::UpdateCurPlatformAxisPosInfo()
{
//  if (m_vdCurPlatformAxisPos.size() >= 3)
// 	{
// 		m_strAxisPosX.Format(_T("%.4f"), m_vdCurPlatformAxisPos.at(0));
// 		m_strAxisPosY.Format(_T("%.4f"), m_vdCurPlatformAxisPos.at(1));
// 		m_strAxisPosD.Format(_T("%.4f"), m_vdCurPlatformAxisPos.at(2));
// 	}
// 	else
// 	{
// 		m_strAxisPosX.Format(_T("------"));
// 		m_strAxisPosY.Format(_T("------"));
// 		m_strAxisPosD.Format(_T("------"));
// 	}
// 	m_labelAxisPosX.SetText(m_strAxisPosX);
// 	m_labelAxisPosY.SetText(m_strAxisPosY);
// 	m_labelAxisPosD.SetText(m_strAxisPosD);
}


void CDlgVisionRunAlignInfo::UpdateAlignerProcessInfo()
{
	if (m_pVisionASM)
	{
		m_pVisionASM->GetAlignerProcessInfo(m_AlignerProcessInfo);
	}

	m_strEditAlignTimes.Format(_T("%d"), m_AlignerProcessInfo.nTimes);
	m_strEditAxisPosX.Format(_T("%.4f"), m_AlignerProcessInfo.dXMove);
	m_strEditAxisPosY.Format(_T("%.4f"), m_AlignerProcessInfo.dYMove);
	m_strEditAxisPosD.Format(_T("%.4f"), m_AlignerProcessInfo.dDMove);
	m_strEditCurPrecisionX.Format(_T("%.4f"), m_AlignerProcessInfo.dXOffset);
	m_strEditCurPrecisionY.Format(_T("%.4f"), m_AlignerProcessInfo.dYOffset);
	m_strEditCurPrecisionD.Format(_T("%.4f"), m_AlignerProcessInfo.dDOffset);

	memset(&m_AlignerProcessInfo, 0, sizeof(AlignerProcessInfo));
	UpdateData(FALSE);
}


void CDlgVisionRunAlignInfo::UpdateCheckAlignerTargetSizeInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo platformInfo;
	m_pVisionASM->GetSysPlatformInfo(platformInfo); 
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	// 启用标定关联验证通过尺寸-对象目标分离-以及非模态窗体指针不为空时
	if ( optionInfo.m_bEnableRelevanceCheckBySize  &&  m_pDlgCalibCheckBySize !=NULL)
	{
		// 刷新轴数据
		if(m_pDlgCalibCheckBySize->m_nGetAxisOption ==0 && m_pDlgCalibCheckBySize->m_nTypeIndex==1)  // 获取轴数据
		{
			m_pDlgCalibCheckBySize->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
			m_pDlgCalibCheckBySize->UpdateAxisInfo();
		}

		// 刷新当前搜索位置的图像与平台坐标；
		// 图像坐标（主点）

		int nBaseIndex = 0;
		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			nBaseIndex = m_pVisionASM->GetPosNum()/2;
		} 
		else
		{
			nBaseIndex = 0;
		}
		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(0) = ((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex +nBaseIndex ).GetPosX();
		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(1) = ((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+ nBaseIndex).GetPosY();

		// 平台坐标（主点 综合重构后为重构交点）
		CMarkPlatformPos mpTargetMarkPos;

		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		if (tmpAlignTool !=NULL)
		{
			mpTargetMarkPos =tmpAlignTool->GetTargetMarkPlatformPosForCheck();
			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(2) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+ nBaseIndex).GetPosX();
			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(3) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+ nBaseIndex).GetPosY();
			m_pDlgCalibCheckBySize->UpdateImagePosDataList(m_pDlgCalibCheckBySize->m_nCurSearchIndex,m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos);
		}
	}


	if ( optionInfo.m_bEnableCalibrateCheckByFixedTargetAligner &&  m_pDlgCalibCheckByFixedTargetAligner !=NULL)
	{
		// 刷新轴数据
		if(m_pDlgCalibCheckByFixedTargetAligner->m_nGetAxisOption ==0)  // 获取轴数据
		{
			m_pDlgCalibCheckByFixedTargetAligner->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
			m_pDlgCalibCheckByFixedTargetAligner->UpdateTargetAxisInfo();
		}
	}
}

void CDlgVisionRunAlignInfo::UpdateCheckAlignerObjectSizeInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

// 	SysPlatformInfo platformInfo;
// 	m_pVisionASM->GetSysPlatformInfo(platformInfo); 

	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	// 启用标定关联验证通过尺寸以及非模态窗体指针不为空时
	if ( optionInfo.m_bEnableCalibrateCheckBySize &&  m_pDlgCalibCheckBySize !=NULL)
	{
		// 刷新轴数据
		if(m_pDlgCalibCheckBySize->m_nGetAxisOption ==0)  // 获取轴数据
		{
			m_pDlgCalibCheckBySize->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
			m_pDlgCalibCheckBySize->UpdateAxisInfo();
		}

		// 刷新当前搜索位置的图像与平台坐标；
		// 图像坐标（主点）
		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(0) = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosX();
		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(1) = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosY();

		// 重构后点
		CMarkPlatformPos mpTargetMarkPos;
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		if (tmpAlignTool !=NULL)
		{
			mpTargetMarkPos =tmpAlignTool->GetObjectMarkPlatformPos();
			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(2) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosX();
			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(3) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosY();
			m_pDlgCalibCheckBySize->UpdateImagePosDataList(m_pDlgCalibCheckBySize->m_nCurSearchIndex,m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos);
		}

	}

	if ( optionInfo.m_bEnableCalibrateCheckByFixedTargetAligner &&  m_pDlgCalibCheckByFixedTargetAligner !=NULL)
	{
		// 刷新轴数据
		if(m_pDlgCalibCheckByFixedTargetAligner->m_nGetAxisOption ==0)  // 获取轴数据
		{
			m_pDlgCalibCheckByFixedTargetAligner->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
			m_pDlgCalibCheckByFixedTargetAligner->UpdateObjectAxisInfo();
		}

		// 候补搜索时，刷新图像坐标;
		if (m_pDlgCalibCheckByFixedTargetAligner->m_bCandidateSearch)
		{
			std::vector<double> vdate;
			vdate.resize(2,0.0);
			CCoordPos tmpCCoordPos = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckByFixedTargetAligner->m_nCurObjectSearchIndex);
			vdate[0]=tmpCCoordPos.GetPosX();
			vdate[1]=tmpCCoordPos.GetPosY();
			m_pDlgCalibCheckByFixedTargetAligner->UpdateImagePosDataList(vdate);
		}

	}
}

// void CDlgVisionRunAlignInfo::UpdateCheckAlignerObjectSizeInfo()
// {
// 	if (!m_pVisionASM)
// 	{
// 		return;
// 	}
// 
// 	SysPlatformInfo platformInfo;
// 	m_pVisionASM->GetSysPlatformInfo(platformInfo); 
// 	CPlatformOptionInfo optionInfo;
// 	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
// 	// 启用标定关联验证通过尺寸以及非模态窗体指针不为空时
// 	if ( optionInfo.m_bEnableCalibrateCheckBySize &&  m_pDlgCalibCheckBySize !=NULL)
// 	{
// 		// 刷新轴数据
// 		if(m_pDlgCalibCheckBySize->m_nGetAxisOption ==0)  // 获取轴数据
// 		{
// 			m_pDlgCalibCheckBySize->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
// 			m_pDlgCalibCheckBySize->UpdateAxisInfo();
// 		}
// 
// 		// 刷新当前搜索位置的图像与平台坐标；
// 		// 图像坐标（主点）
// 		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(0) = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosX();
// 		m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(1) = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosY();
// 
// 		// 重构后点
// 		CMarkPlatformPos mpTargetMarkPos;
// 		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
// 		if (tmpAlignTool !=NULL)
// 		{
// 			mpTargetMarkPos =tmpAlignTool->GetObjectMarkPlatformPos();
// 			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(2) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosX();
// 			m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(3) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex).GetPosY();
// 			m_pDlgCalibCheckBySize->UpdateImagePosDataList(m_pDlgCalibCheckBySize->m_nCurSearchIndex,m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos);
// 		}
// 
// 	}
// 
// 	if ( optionInfo.m_bEnableCalibrateCheckByFixedTargetAligner &&  m_pDlgCalibCheckByFixedTargetAligner !=NULL)
// 	{
// 		// 刷新轴数据
// 		if(m_pDlgCalibCheckByFixedTargetAligner->m_nGetAxisOption ==0)  // 获取轴数据
// 		{
// 			m_pDlgCalibCheckByFixedTargetAligner->m_AlignerAxisInfo = m_pVisionASM->m_AlignerAxisInfo;
// 			m_pDlgCalibCheckByFixedTargetAligner->UpdateObjectAxisInfo();
// 		}
// 
// 		// 候补搜索时，刷新图像坐标;
// 		if (m_pDlgCalibCheckByFixedTargetAligner->m_bCandidateSearch)
// 		{
// 			std::vector<double> vdate;
// 			vdate.resize(2,0.0);
// 			CCoordPos tmpCCoordPos = ((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckByFixedTargetAligner->m_nCurObjectSearchIndex);
// 			vdate[0]=tmpCCoordPos.GetPosX();
// 			vdate[1]=tmpCCoordPos.GetPosY();
// 			m_pDlgCalibCheckByFixedTargetAligner->UpdateImagePosDataList(vdate);
// 
// 			// 保存日志
// 			CMarkPlatformPos mpTargetMarkPos;
// 			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
// 			if (tmpAlignTool !=NULL)
// 			{
// 				mpTargetMarkPos =tmpAlignTool->GetObjectMarkPlatformPos();
// 			}
// 
// 			CString strLog = _T("");
// 			CString StrTmp = _T(""); 
// 			strLog += _T("\n");
// 			StrTmp .Format( _T("标定固定目标对位验证--候补搜索 位置%d\n图像坐标:"),m_pDlgCalibCheckByFixedTargetAligner->m_nCurObjectSearchIndex+1);
// 			strLog += StrTmp;
// 			((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(m_pDlgCalibCheckByFixedTargetAligner->m_nCurObjectSearchIndex).ToString(StrTmp);
// 			strLog += StrTmp;
// 			StrTmp=_T("平台坐标:");
// 			strLog += StrTmp;
// 			mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckByFixedTargetAligner->m_nCurObjectSearchIndex).ToString(StrTmp);
// 			strLog += StrTmp;
// 
// 			// 日志路径
// 			CString SaveFilePath = _T("D:\\VisionASMLog\\");
// 			DWORD dwRet = GetFileAttributes(SaveFilePath);	
// 			if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 			{
// 				// 暂时不考虑创建文件夹失败情况
// 				CreateDirectory(SaveFilePath, NULL);
// 			}
// 
// 			SysPlatformInfo sysPlatformInfo;
// 			m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
// 			CVisionASMConfig *vsConfig;
// 			m_pVisionASM->GetVSconfig(vsConfig);
// 
// 			if ((NULL !=vsConfig))
// 			{
// 				if (sysPlatformInfo.m_bProductFaChd)
// 				{
// 					int nCurProductIndexFa;
// 					int nCurProductIndexCh;
// 					vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
// 					StrTmp.Format(_T("工位%d_大品种%03d_小品种%03d_FixedTargetCalibrationVerification.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 						nCurProductIndexFa+1,nCurProductIndexCh+1);
// 				}
// 				else
// 				{
// 					StrTmp.Format(_T("工位%d_品种%03d_FixedTargetCalibrationVerification.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 						vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
// 				}
// 
// 			}
// 
// 			SaveFilePath += StrTmp;
// 			m_pVisionASM->VisionAlignLogRecord(SaveFilePath,strLog);
// 		}
// 
// 	}
// }


void CDlgVisionRunAlignInfo::InitBtnLayout()
{
	int nArrBtnID[] = {IDC_BTN_CALCU_VIRTUAL_OBJECT_POS, IDC_BTN_CALCU_BENCH_PICK,
	IDC_BTN_CALIB_TARGET_POS, IDC_BTN_CALIB_OBJECT_POS, IDC_BTN_CALIB_CHECK_BY_SIZE,
	IDC_BTN_RELEVANCE_CHECK_BY_SIZE, IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET, IDC_BTN_ROBOT_TEACH,
	IDC_BTN_ROBOT_TEACH_RANGE,IDC_BTN_CALIB_COPY,IDC_BTN_ALIGN_OFFSET_BY_AREA_SET};

	CLayout* pBtnLayout = new CVerticalLayout;
	for (int i = 0; i < sizeof(nArrBtnID)/sizeof(int); i++)
	{
		pBtnLayout->AddWnd(GetDlgItem(nArrBtnID[i]), 0, 35, CLayout::eSizePolicy_Fixed);
	}

	pBtnLayout->AddWnd(GetDlgItem(IDC_STATIC_OFFSET_DIRECTION), 0, 120, CLayout::eSizePolicy_Fixed);
	CLayout* pSpring = new CHorizontalLayout;
	pBtnLayout->AddLayout(pSpring, 0,0, CLayout::eSizePolicy_Expand);

	// 将按钮显示位置设置到右侧位置
	m_pMainLayout = new CHorizontalLayout;
	pSpring = new CHorizontalLayout;
	m_pMainLayout->AddLayout(pSpring, 0,0, CLayout::eSizePolicy_Expand);
	m_pMainLayout->AddLayout(pBtnLayout, 120, 0, CLayout::eSizePolicy_Fixed);
	m_pMainLayout->SetParent(this);
	m_pMainLayout->OnSize();
}

void CDlgVisionRunAlignInfo::SetDlgItemVisible()
{
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (eObjectVirtual == platformInfo.m_eAlignerObjectType)
		{
			GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->ShowWindow(SW_HIDE);
		}

		if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
		{
			if (!platformInfo.m_bTarObjCamSepAutoCalibTar)
			{
				if (ePlatformXYPD == platformInfo.m_eMidPlatformType)
				{
					GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->ShowWindow(SW_SHOW);
				}
				else
				{
					GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->ShowWindow(SW_HIDE);
			}
		}
		else
		{			

			if (TRUE == platformInfo.m_bTargetCalibUseBoard)
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->ShowWindow(SW_HIDE);
			}
		}


		MotionControlType TmpMotionControlType = eMCNo;
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL != vsConfig)
		{
			TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
		}

		if (eMCNo == TmpMotionControlType)
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->ShowWindow(SW_HIDE);
		}
		else if (eMCYamahaRobot == TmpMotionControlType
			|| eMCYamahaRobot340 == TmpMotionControlType)
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->EnableWindow(TRUE);

			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->ShowWindow(SW_HIDE);
		}


		if (platformInfo.m_bPlatformPick)
		{
			GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->ShowWindow(SW_HIDE);
		}



		if (optionInfo.m_bEnableCalibrateCheckBySize)
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		}
		if (optionInfo.m_bEnableRelevanceCheckBySize)
		{
			GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		}

		if (optionInfo.m_bEnableCalibrateCheckByFixedTargetAligner)
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->ShowWindow(SW_SHOW);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->ShowWindow(SW_HIDE);
		}

		if (optionInfo.m_bEnableAlignOffsetMarkByAera)
		{
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->EnableWindow(TRUE);

		}
		else
		{
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->EnableWindow(FALSE);
		}

		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			GetDlgItem(IDC_BTN_CALIB_COPY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CALIB_COPY)->EnableWindow(TRUE);
		} 
		else
		{
			GetDlgItem(IDC_BTN_CALIB_COPY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CALIB_COPY)->EnableWindow(FALSE);
		}


	}
	else
	{			
		GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ROBOT_TEACH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->ShowWindow(SW_HIDE);
		
	}

	UpdateDialogLanguage();

	


	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}
}

void CDlgVisionRunAlignInfo::EnableDlgItem(bool bEnable)
{
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (eObjectVirtual == platformInfo.m_eAlignerObjectType)
		{
			GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->EnableWindow(bEnable);

		}
		else
		{
			GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->EnableWindow(FALSE);
		}



		if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
		{
			if (!platformInfo.m_bTarObjCamSepAutoCalibTar)
			{
				if (ePlatformXYPD == platformInfo.m_eMidPlatformType)
				{
					GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->EnableWindow(bEnable);
				}
				else
				{
					GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(bEnable);
				}
			}
			else
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(FALSE);
			}


		}
		else
		{			

			if (TRUE == platformInfo.m_bTargetCalibUseBoard)
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->EnableWindow(bEnable);
			}
			else
			{
				GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(FALSE);
			}
		}

		MotionControlType TmpMotionControlType = eMCNo;
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL != vsConfig)
		{
			TmpMotionControlType = vsConfig->m_CommParamInfo.m_eMotionControlType;
		}
		if (eMCNo == TmpMotionControlType)
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->EnableWindow(FALSE);
		}
		else if (eMCYamahaRobot == TmpMotionControlType
			|| eMCYamahaRobot340 == TmpMotionControlType)
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->EnableWindow(bEnable);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->EnableWindow(bEnable);
		}
		else
		{
			GetDlgItem(IDC_BTN_ROBOT_TEACH)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->EnableWindow(FALSE);
		}


		if (platformInfo.m_bPlatformPick)
		{
			GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->EnableWindow(bEnable);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->EnableWindow(FALSE);
		}

		if (optionInfo.m_bEnableAlignOffsetMarkByAera)
		{
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->EnableWindow(bEnable);
		}
		else
		{
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->EnableWindow(FALSE);
		}

		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			GetDlgItem(IDC_BTN_CALIB_COPY)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_CALIB_COPY)->EnableWindow(bEnable);
		} 
		else
		{
			GetDlgItem(IDC_BTN_CALIB_COPY)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_CALIB_COPY)->EnableWindow(FALSE);
		}

		BOOL bShowCalibrateCheck = FALSE;
		if(m_pParent && m_pParent->m_pParent)
		{
			bShowCalibrateCheck = m_pParent->m_pParent->m_bIsRunning;
		}

		if (optionInfo.m_bEnableCalibrateCheckBySize)
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(!bEnable && bShowCalibrateCheck);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(FALSE);
		}
		if (optionInfo.m_bEnableRelevanceCheckBySize)
		{
			GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(!bEnable  && bShowCalibrateCheck);
		}
		else
		{
			GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(FALSE);
		}

		if (optionInfo.m_bEnableCalibrateCheckByFixedTargetAligner)
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(!bEnable  && bShowCalibrateCheck);
		}
		else
		{
			GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(FALSE);
		}
	}
	else
	{			
		GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALIB_OBJECT_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALIB_TARGET_POS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ROBOT_TEACH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ROBOT_TEACH_RANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALCU_BENCH_PICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ALIGN_OFFSET_BY_AREA_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CALIB_COPY)->EnableWindow(FALSE);
	}
}


BOOL CDlgVisionRunAlignInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//UpdateDialogLanguage();
	InitBtnLayout();
	SetItemFont();
	SetDlgItemVisible();
	EnableDlgItem(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgVisionRunAlignInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == VK_DOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgVisionRunAlignInfo::OnBnClickedBtnCalcuVirtualObjectPos()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog,strtemp;
	GetDlgItem(IDC_BTN_CALCU_VIRTUAL_OBJECT_POS)->GetWindowText(strtemp);
	strLog.Format(_T("点击主界面[%s]，进入%s界面\n"),strtemp,strtemp);
	g_opLogFile.SaveLogText(strLog,1);


	m_pParent->SetRunCommInfoShow();

	CPlatformOptionInfo optionInfo;
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);


// 		if (sysPlatformInfo.m_bTarCamWithDiffObjCam)
// 		{
// 			CDlgCalcuVirtualObject4 dlg;
// 			if (m_pVisionASM != NULL)
// 			{
// 				dlg.SetVisionAlignerRef(m_pVisionASM);
// 			}
// 			else
// 			{
// 				return;		
// 			}
// 			dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
// 			if (dlg.DoModal() == IDOK)
// 			{
// 			}
// 
// 
// 			if (optionInfo.m_bEnableSearchShutter == TRUE)
// 			{
// 				for (int j=0; j<m_pVisionASM->GetCameraNum(); j++)
// 				{
// 					// 确保实时采集为环境设置中的曝光时间
// 					Sleep(10);
// 					m_pVisionASM->UpdateCameraShutter(j);
// 				}
// 			}
// 
// 			return;
// 		}

	}

	// 计算虚拟对象的位置，即计算虚拟对象与实际对象之间的关系
	CDlgCalcuVirtualObject dlg;
	if (m_pVisionASM != NULL)
	{
		dlg.SetVisionAlignerRef(m_pVisionASM);
	}
	else
	{
		return;		
	}
	dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	if (dlg.DoModal() == IDOK)
	{
	}


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


void CDlgVisionRunAlignInfo::OnBnClickedBtnCalibTargetPos()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[标定目标位置]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_pParent->SetRunCommInfoShow();

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
// 		////GYM-关联日志存储
// 		CString strCorrelationLogPath;
// 
// 		strCorrelationLogPath.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CorrelationLog.txt"),platformInfo.m_nPlatformIndex+1,
// 			m_pVisionASM->GetTotalProuductData()->GetCurProductIndex()+1);
// 
// 		SysPlatformInfo sysPlatformInfo;
// 		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
// 
// 		CVisionASMConfig *vsConfig;
// 		m_pVisionASM->GetVSconfig(vsConfig);
// 		CString StrTmp = _T("");
// 		if ((NULL !=vsConfig))
// 		{
// 			if (sysPlatformInfo.m_bProductFaChd)
// 			{
// 				int nCurProductIndexFa;
// 				int nCurProductIndexCh;
// 				vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
// 				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_大品种%03d_小品种%03d_CorrelationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 					nCurProductIndexFa+1,nCurProductIndexCh+1);
// 			}
// 			else
// 			{
// 				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CorrelationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 					vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
// 			}
// 
// 		}


		////GYM

		if (platformInfo.m_bTargetCalibUseBoard == TRUE)
		{
			if (platformInfo.m_bTargetObjectCamSeparate == TRUE
				&& platformInfo.m_nPositionNum == 4)
			{
				// 4相机分离，即2对2
				CDlgCalibTargetPos4Board dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}

				dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			else if (platformInfo.m_bTargetObjectCamSeparate == TRUE
				&& (platformInfo.m_nPositionNum == 6 || platformInfo.m_nPositionNum == 8))
			{
				// 8相机分离，即4对4
				CDlgCalibTargetPos8Board dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}

				dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
		}
		else
		{
			if (platformInfo.m_bTargetObjectCamSeparate == TRUE
				&& platformInfo.m_nPositionNum == 4)
			{
				// 4相机分离，即2对2
				CDlgCalibTargetPos dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}

// 				////GYM-关联日志存储
// 				CString strInfo;
// 				CString strTemp;
// 				strInfo.Empty();
// 				strTemp.Format(_T("\n品种%03d开始关联\n"),m_pVisionASM->GetTotalProuductData()->GetCurProductIndex()+1);
// 				strInfo += strTemp;
// 				strTemp.Format(_T("关联参数如下:\n"),m_pVisionASM->GetTotalProuductData()->GetCurProductIndex());
// 				strInfo += strTemp;
// 				if (platformInfo.m_bTarObjCamSepNoAxisCalib)
// 				{
// 					strTemp.Format(_T("......关联方式: 像素关联方式;\n"));
// 					strInfo += strTemp;
// 				}
// 				else
// 				{
// 					strTemp.Format(_T("......关联方式: 轴关联方式;\n"));
// 					strInfo += strTemp;
// 				}
// 
// 				if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternSingleMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 单模板单Mark;\n"));
// 					strInfo += strTemp;
// 				} 
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eDoublePatternSingleMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 双模板单Mark;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternMultiMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 单模板多Mark;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternGuanlian4Point)
// 				{
// 					strTemp.Format(_T("......关联类型: 四点关联;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eDmCodeCalib)
// 				{
// 					strTemp.Format(_T("......关联类型: 二维码关联;\n"));
// 					strInfo += strTemp;
// 				}
// 
// 				m_pVisionASM->VisionAlignLogRecord(strCorrelationLogPath,strInfo,TRUE);
// 
// 				////GYM


				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			else if (platformInfo.m_bTargetObjectCamSeparate == TRUE
				&&  (platformInfo.m_nPositionNum == 6 || platformInfo.m_nPositionNum == 8))
			{
				// 8相机分离，即4对4
				CDlgCalibTargetPos8 dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}

// 				////GYM-关联日志存储
// 				CString strInfo;
// 				CString strTemp;
// 				strInfo.Empty();
// 				strTemp.Format(_T("\n品种%03d开始关联\n"),m_pVisionASM->GetTotalProuductData()->GetCurProductIndex()+1);
// 				strInfo += strTemp;
// 				strTemp.Format(_T("关联参数如下:\n"),m_pVisionASM->GetTotalProuductData()->GetCurProductIndex());
// 				strInfo += strTemp;
// 				if (platformInfo.m_bTarObjCamSepNoAxisCalib)
// 				{
// 					strTemp.Format(_T("......关联方式: 像素关联方式;\n"));
// 					strInfo += strTemp;
// 				}
// 				else
// 				{
// 					strTemp.Format(_T("......关联方式: 轴关联方式;\n"));
// 					strInfo += strTemp;
// 				}
// 
// 				if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternSingleMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 单模板单Mark;\n"));
// 					strInfo += strTemp;
// 				} 
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eDoublePatternSingleMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 双模板单Mark;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternMultiMark)
// 				{
// 					strTemp.Format(_T("......关联类型: 单模板多Mark;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eSinglePatternGuanlian4Point)
// 				{
// 					strTemp.Format(_T("......关联类型: 四点关联;\n"));
// 					strInfo += strTemp;
// 				}
// 				else if (m_pVisionASM->GetCalibTargetMarkPatternMode()==eDmCodeCalib)
// 				{
// 					strTemp.Format(_T("......关联类型: 二维码关联;\n"));
// 					strInfo += strTemp;
// 				}
// 
// 				m_pVisionASM->VisionAlignLogRecord(strCorrelationLogPath,strInfo,TRUE);
// 				////GYM


				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			else if (platformInfo.m_bTargetObjectCamSeparate == TRUE &&  (platformInfo.m_nPositionNum == 2) && (platformInfo.m_eMidPlatformType == ePlatformXY))
			{
				// 2相机分离
				CDlgCalibTargetPos2 dlg;

				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);

				if (dlg.DoModal() == IDOK)
				{
				}
			}
		}


		if (optionInfo.m_bEnableSearchShutter == TRUE)
		{
			for (int j=0; j<m_pVisionASM->GetCameraNum(); j++)
			{
				// 确保实时采集为环境设置中的曝光时间
				Sleep(10);
				m_pVisionASM->UpdateCameraShutter(j);
			}
		}
		m_pVisionASM->AddAllGuiCalibrateGraphicsToDisplay();
	}
}


void CDlgVisionRunAlignInfo::OnBnClickedBtnRobotTeach()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[机器人示教]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_pParent->SetRunCommInfoShow();

	CDlgRobotTeach dlg;
	if (m_pVisionASM != NULL)
	{
		dlg.SetVisionAlignerRef(m_pVisionASM);
	}
	else
	{
		return;		
	}
	dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
	if (dlg.DoModal() == IDOK)
	{
	}
}


void CDlgVisionRunAlignInfo::OnBnClickedBtnRobotTeachRange()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[设置机器人示教范围]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_pParent->SetRunCommInfoShow();

	if (m_pVisionASM == NULL)
	{
		return;
	}
	CRobotTeachRangeInfo robotTeachRangeInfo;
	m_pVisionASM->GetRobotTeachRangeInfo(robotTeachRangeInfo);

	CDlgRobotUserLogin dlgLogin;
	dlgLogin.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	dlgLogin.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
	dlgLogin.SetRobotPassword(robotTeachRangeInfo.m_strPassword);
	if (dlgLogin.DoModal() == IDOK)
	{
	}
	else
	{
		return;
	}



	CDlgRobotTeachRange dlg;
	if (m_pVisionASM != NULL)
	{
		dlg.SetVisionAlignerRef(m_pVisionASM);
	}
	else
	{
		return;		
	}
	dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
	if (dlg.DoModal() == IDOK)
	{
	}
}


void CDlgVisionRunAlignInfo::OnBnClickedBtnCalcuBenchPick()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[计算基准拾取位置]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_pParent->SetRunCommInfoShow();

	CDlgCalcuBenchPick dlg;
	CPlatformOptionInfo optionInfo;
	if (m_pVisionASM != NULL)
	{
		dlg.SetVisionAlignerRef(m_pVisionASM);
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	}
	else
	{
		return;		
	}
	dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	if (dlg.DoModal() == IDOK)
	{
	}

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

void CDlgVisionRunAlignInfo::OnBnClickedBtnCalibObjectPos()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击主界面[标定对象位置]\n"));
	g_opLogFile.SaveLogText(strLog,1);

	m_pParent->SetRunCommInfoShow();

	CPlatformOptionInfo optionInfo;
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE && platformInfo.m_nPositionNum == 2)
		{
			if (eDmCodeCalib == m_pVisionASM->GetCalibTargetMarkPatternMode())
			{
				CDlgCalibObjectPos2Code ObjectCodeDlg;
				ObjectCodeDlg.SetVisionAlignerRef(m_pVisionASM);
				ObjectCodeDlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
				ObjectCodeDlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				ObjectCodeDlg.DoModal();
			}
			else
			{
				CDlgCalibObjectPos2Board ObjectBoardDlg;
				ObjectBoardDlg.SetVisionAlignerRef(m_pVisionASM);
				ObjectBoardDlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
				ObjectBoardDlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				ObjectBoardDlg.DoModal();
			}


// 			CDlgCalibObjectPos2Board dlg;
// 			if (m_pVisionASM != NULL)
// 			{
// 				dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 				dlg.SetVisionAlignerRef(m_pVisionASM);
// 			}
// 			else
// 			{
// 				return;		
// 			}
// 
// 			dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
// 			dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
// 			if (dlg.DoModal() == IDOK)
// 			{
// 			}
		}
		else
		{
			CProductData* pProduct;

			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
			{
				pProduct = pTotalProdData->GetItemPtr(nCurProIdx);		
			}				

			if (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 8
				&& eAlignerPos1234_56_4V2==pProduct->m_pAlignerParam->GetAligner4Vs4Type())
			{
				CDlgCalibObjectPos8 dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			else if (platformInfo.m_bTargetObjectCamSeparate == TRUE && platformInfo.m_nPositionNum == 8)
			{
				CDlgCalibObjectPos8 dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
			else
			{
				CDlgCalibObjectPos dlg;
				if (m_pVisionASM != NULL)
				{
					dlg.EnableSysKeyboard(m_pParent->m_pParent->m_SysOptionConfig.m_bSysKeyboardEnabled);
					dlg.SetVisionAlignerRef(m_pVisionASM);
				}
				else
				{
					return;		
				}
				dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
				if (dlg.DoModal() == IDOK)
				{
				}
			}
		}


		if (optionInfo.m_bEnableSearchShutter == TRUE)
		{
			for (int j=0; j<m_pVisionASM->GetCameraNum(); j++)
			{
				// 确保实时采集为环境设置中的曝光时间
				Sleep(10);
				m_pVisionASM->UpdateCameraShutter(j);
			}
		}
		m_pVisionASM->AddAllGuiCalibrateGraphicsToDisplay();
	}
}



void CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCheckBySize()
{
	// 标定验证
	CString strLog;
	strLog.Format(_T("点击主界面[标定验证]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(FALSE);  
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(FALSE);

	m_pParent->SetRunCommInfoShow();

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		if (NULL == m_pDlgCalibCheckBySize )   
		{  
			// 创建非模态对话框实例
			m_pDlgCalibCheckBySize = new CDlgCalibCheckBySize;   
			m_pDlgCalibCheckBySize->m_pParent = this;
			m_pDlgCalibCheckBySize->EnableSysKeyboard(m_bSysKeyboardEnabled);
			m_pVisionASM->m_bIsExecuteSizeCheck = TRUE;
			m_pDlgCalibCheckBySize->SetVisionAlignerRef(m_pVisionASM);
			//if (NULL != m_pDlgVisionRunProductInfo)
			//{
			//	m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo = m_pDlgVisionRunProductInfo->m_AlignerObjectSizeInfo;
			//}

			m_pDlgCalibCheckBySize->m_nTypeIndex = 0;
			m_pDlgCalibCheckBySize->Create(IDD_DLG_CALIB_SIZE_CHECK, this); 

			// 调整显示位置
			CRect rect,rect1;
			m_pParent->m_pParent->GetClientRect(&rect);  
			m_pParent->m_pParent->ScreenToClient(&rect);
			m_pDlgCalibCheckBySize->GetClientRect(&rect1);
			m_pDlgCalibCheckBySize->MoveWindow(rect.left+rect.Width()/2-rect1.Width()/2,rect.top+rect.Height()/2-rect1.Height()/2,rect1.Width(),1.1*rect1.Height());


			m_pDlgCalibCheckBySize->ShowWindow(SW_SHOW);
		}
	}


}


void CDlgVisionRunAlignInfo::OnBnClickedBtnRelevanceCheckBySize()
{
	// 关联验证
	CString strLog;
	strLog.Format(_T("点击主界面[关联验证]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(FALSE);  
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(FALSE);

	m_pParent->SetRunCommInfoShow();

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		if (NULL == m_pDlgCalibCheckBySize )   
		{  
			// 创建非模态对话框实例
			m_pDlgCalibCheckBySize = new CDlgCalibCheckBySize;   
			m_pDlgCalibCheckBySize->m_pParent = this;
			m_pDlgCalibCheckBySize->EnableSysKeyboard(m_bSysKeyboardEnabled);
			m_pVisionASM->m_bIsExecuteSizeCheck = TRUE;
			m_pDlgCalibCheckBySize->SetVisionAlignerRef(m_pVisionASM);
			m_pDlgCalibCheckBySize->m_nTypeIndex = 1;  // 关联验证
			m_pDlgCalibCheckBySize->Create(IDD_DLG_CALIB_SIZE_CHECK, this); 

			// 调整显示位置
			CRect rect,rect1;
			m_pParent->m_pParent->GetClientRect(&rect);  
			m_pParent->m_pParent->ScreenToClient(&rect);
			m_pDlgCalibCheckBySize->GetClientRect(&rect1);
			m_pDlgCalibCheckBySize->MoveWindow(rect.left+rect.Width()/2-rect1.Width()/2,rect.top+rect.Height()/2-rect1.Height()/2,rect1.Width(),1.1*rect1.Height());

			m_pDlgCalibCheckBySize->ShowWindow(SW_SHOW);
		}
	}
}


BOOL CDlgVisionRunAlignInfo::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
		if (((ch > '9') || (ch < '0')) && (ch != '.'))
		{
			if ((i == 0) && ((ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgVisionRunAlignInfo::IsStringNumerical( LPCTSTR lpszVal )
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

BOOL CDlgVisionRunAlignInfo::IsStringOverMaxX(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);

	CString strVal = lpszVal;	
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxX)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxX)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CDlgVisionRunAlignInfo::IsStringOverMaxY(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	CString strVal = lpszVal;
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxY)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxY)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CDlgVisionRunAlignInfo::IsStringOverMaxD(LPCTSTR lpszVal)
{
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	CString strVal = lpszVal;
	//if (fabs(atof(strVal)) > m_pParent->m_SysOptionConfig.m_dOffsetMaxD)
	if (fabs(atof(strVal)) > optionInfo.m_dOffsetMaxD)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



void CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCheckByFixedTarget()
{
	// 关联验证
	CString strLog;
	strLog.Format(_T("点击主界面[标定验证2]\n"));
	g_opLogFile.SaveLogText(strLog,1);
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(FALSE);  
	GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(FALSE);

	m_pParent->SetRunCommInfoShow();

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		if (NULL == m_pDlgCalibCheckByFixedTargetAligner )   
		{  
			// 创建非模态对话框实例
			m_pDlgCalibCheckByFixedTargetAligner = new CDlgCalibCheckByFixedTargetAligner;   
			m_pDlgCalibCheckByFixedTargetAligner->m_pParent = this;
			m_pDlgCalibCheckByFixedTargetAligner->EnableSysKeyboard(m_bSysKeyboardEnabled);
			m_pVisionASM->m_bIsExecuteFixTargetCheck = TRUE;
			m_pDlgCalibCheckByFixedTargetAligner->SetVisionAlignerRef(m_pVisionASM);

			m_pDlgCalibCheckByFixedTargetAligner->Create(IDD_DLG_CALIB_CHECK_BY_FIXED_TARGET, this); 

			// 调整显示位置
			CRect rect,rect1;
			m_pParent->m_pParent->GetClientRect(&rect);  
			m_pParent->m_pParent->ScreenToClient(&rect);
			m_pDlgCalibCheckByFixedTargetAligner->GetClientRect(&rect1);
			m_pDlgCalibCheckByFixedTargetAligner->MoveWindow(rect.left+rect.Width()/2-rect1.Width()/2,rect.top+rect.Height()/2-rect1.Height()/2,rect1.Width(),1.1*rect1.Height());

			m_pDlgCalibCheckByFixedTargetAligner->ShowWindow(SW_SHOW);
		}
	}
}





void CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_INDEX)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[补偿量][补偿序号]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCurOffsetIndex();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, strVal);
		if (!IsStringPositiveNumerical(strVal) 
			|| atoi(strVal) > ALIGNOFFSETNUM	//zzc补偿序号开放到25，原来15
			|| atoi(strVal) < 1)
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, m_strEditCurOffsetIndex);
		}
		else
		{

			//夹爪2启用
			if (m_strEditGrip)
			{
				if (atoi(strVal) > 15)
				{
					SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, m_strEditCurOffsetIndex);
				}
				m_strEditCurOffsetIndex.Format(_T("%d"), atoi(strVal));
				SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, m_strEditCurOffsetIndex);

				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;
					nCurOffsetIndex += 10; 

					m_pVisionASM->SetCurAlignerOffsetIndex(nCurOffsetIndex);
				}
			} 
			else
			{
				m_strEditCurOffsetIndex.Format(_T("%d"), atoi(strVal));
				SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, m_strEditCurOffsetIndex);

				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;

					m_pVisionASM->SetCurAlignerOffsetIndex(nCurOffsetIndex);
				}
			}


			
			UpdateAlignOffsetInfo();
		}
	}
}



void CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_X, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[补偿量][补偿序号%s][X]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,m_strEditCurOffsetIndex,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCurOffsetX();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_X, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxX(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_X, m_strEditCurOffsetX);
		}
		else
		{
			m_strEditCurOffsetX.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_X, m_strEditCurOffsetX);


			if (m_strEditGrip)
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;
				    nCurOffsetIndex += 10;
					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);

				}
			} 
			else
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;

					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);

				}
			}
			
			UpdateAlignOffsetInfo();
		}
	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_Y, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[补偿量][补偿序号%s][Y]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,m_strEditCurOffsetIndex,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCurOffsetY();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_Y, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxY(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_Y, m_strEditCurOffsetY);
		}
		else
		{
			m_strEditCurOffsetY.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_Y, m_strEditCurOffsetY);

			if (m_strEditGrip)
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;
					nCurOffsetIndex += 10;
					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);
				}
			} 
			else
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;

					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);
				}
			}

			
			UpdateAlignOffsetInfo();
		}
	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValCurOffsetD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_CUR_OFFSET_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_D, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[补偿量][补偿序号%s][D]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,m_strEditCurOffsetIndex,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValCurOffsetD();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValCurOffsetD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_D, strVal);
		if (!IsStringNumerical(strVal) || IsStringOverMaxD(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_D, m_strEditCurOffsetD);
		}
		else
		{
			m_strEditCurOffsetD.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_D, m_strEditCurOffsetD);
			if (m_strEditGrip)
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;
					nCurOffsetIndex += 10;
					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);
				}
			} 
			else
			{
				if (m_pVisionASM != NULL)
				{
					int nCurOffsetIndex = 1;
					nCurOffsetIndex = atoi(m_strEditCurOffsetIndex) - 1;

					CCoordPos pos;
					if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX)/PRECISIONXYUNIT;
						pos.m_dPosY = atof(m_strEditCurOffsetY)/PRECISIONXYUNIT;
						pos.m_dAngle = atof(m_strEditCurOffsetD)/PRECISIONDUNIT;
					}
					else
					{
						pos.m_dPosX = atof(m_strEditCurOffsetX);
						pos.m_dPosY = atof(m_strEditCurOffsetY);
						pos.m_dAngle = atof(m_strEditCurOffsetD);
					}

					m_pVisionASM->SetCurAlignerOffset(nCurOffsetIndex, pos);
				}
			}
			
			
			UpdateAlignOffsetInfo();
		}
	}
}



void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionMaxTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[最多次数]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetPrecisionMaxTimes();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionMaxTimes()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, strVal);
		if (!IsStringPositiveNumerical(strVal)
			|| atoi(strVal) > 50
			|| atoi(strVal) < 1)
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, m_strEditTargetPrecisionMaxTimes);
		}
		else
		{
			m_strEditTargetPrecisionMaxTimes.Format(_T("%d"), atoi(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_MAX_TIMES, m_strEditTargetPrecisionMaxTimes);
			if (m_pVisionASM != NULL)
			{
				int nMaxTimes = 1;
				nMaxTimes = atoi(m_strEditTargetPrecisionMaxTimes);
				m_pVisionASM->SetAlignerMaxTime(nMaxTimes);
			}
			UpdateAlignPrecisionInfo();
		}
	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))	//zzc 精度设置键盘没有负号
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[X]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetPrecisionX();
	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionX()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X, strVal);
		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X, m_strEditTargetPrecisionX);
		}
		else
		{
			m_strEditTargetPrecisionX.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X, m_strEditTargetPrecisionX);
			if (m_pVisionASM != NULL)
			{
				CCoordPos pos;
				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX)/PRECISIONXYUNIT;
					pos.m_dPosY = atof(m_strEditTargetPrecisionY)/PRECISIONXYUNIT;
					pos.m_dAngle = atof(m_strEditTargetPrecisionD)/PRECISIONDUNIT;
				}
				else
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX);
					pos.m_dPosY = atof(m_strEditTargetPrecisionY);
					pos.m_dAngle = atof(m_strEditTargetPrecisionD);
				}

				m_pVisionASM->SetAlignerPrecision(pos);				
			}
			UpdateAlignPrecisionInfo();
		}
	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[Y]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetPrecisionY();
	}
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionY()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y, strVal);
		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y, m_strEditTargetPrecisionY);
		}
		else
		{
			m_strEditTargetPrecisionY.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y, m_strEditTargetPrecisionY);
			if (m_pVisionASM != NULL)
			{
				CCoordPos pos;
				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX)/PRECISIONXYUNIT;
					pos.m_dPosY = atof(m_strEditTargetPrecisionY)/PRECISIONXYUNIT;
					pos.m_dAngle = atof(m_strEditTargetPrecisionD)/PRECISIONDUNIT;
				}
				else
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX);
					pos.m_dPosY = atof(m_strEditTargetPrecisionY);
					pos.m_dAngle = atof(m_strEditTargetPrecisionD);
				}
				m_pVisionASM->SetAlignerPrecision(pos);	
			}
			UpdateAlignPrecisionInfo();
		}
	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D, strOutput);
			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
			int nCurProIdx = pTotalProdData->GetCurProductIndex();
			CString strLog;
			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[D]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
			g_opLogFile.SaveLogText(strLog,2);
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetPrecisionD();
	}
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionD()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D, strVal);
		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D, m_strEditTargetPrecisionD);
		}
		else
		{
			m_strEditTargetPrecisionD.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D, m_strEditTargetPrecisionD);
			if (m_pVisionASM != NULL)
			{
				CCoordPos pos;
				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX)/PRECISIONXYUNIT;
					pos.m_dPosY = atof(m_strEditTargetPrecisionY)/PRECISIONXYUNIT;
					pos.m_dAngle = atof(m_strEditTargetPrecisionD)/PRECISIONDUNIT;
				}
				else
				{
					pos.m_dPosX = atof(m_strEditTargetPrecisionX);
					pos.m_dPosY = atof(m_strEditTargetPrecisionY);
					pos.m_dAngle = atof(m_strEditTargetPrecisionD);
				}
				m_pVisionASM->SetAlignerPrecision(pos);	
			}
			UpdateAlignPrecisionInfo();
		}
	}
}

void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionX2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_X2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X2, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X2, strOutput);
// 			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
// 			int nCurProIdx = pTotalProdData->GetCurProductIndex();
// 			CString strLog;
// 			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[X2]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
// 			g_opLogFile.SaveLogText(strLog,2);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditValTargetPrecisionX2();
// 	}	
}


void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionX2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X2, strVal);
// 		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X2, m_strEditTargetPrecisionX2);
// 		}
// 		else
// 		{
// 			m_strEditTargetPrecisionX2.Format(_T("%.4f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_X2, m_strEditTargetPrecisionX2);
// 			if (m_pVisionASM != NULL)
// 			{
// 				CCoordPos pos;
// 				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2)/PRECISIONXYUNIT;
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2)/PRECISIONXYUNIT;
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2)/PRECISIONDUNIT;
// 				}
// 				else
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2);
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2);
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2);
// 				}
// 				m_pVisionASM->SetAlignerPrecision2(pos);				
// 			}
// 			UpdateAlignPrecisionInfo2();
// 		}
// 	}
}


void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionY2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_Y2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y2, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y2, strOutput);
// 			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
// 			int nCurProIdx = pTotalProdData->GetCurProductIndex();
// 			CString strLog;
// 			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[Y2]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
// 			g_opLogFile.SaveLogText(strLog,2);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditValTargetPrecisionY2();
// 	}
}

void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionY2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y2, strVal);
// 		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y2, m_strEditTargetPrecisionY2);
// 		}
// 		else
// 		{
// 			m_strEditTargetPrecisionY2.Format(_T("%.4f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_Y2, m_strEditTargetPrecisionY2);
// 			if (m_pVisionASM != NULL)
// 			{
// 				CCoordPos pos;
// 				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2)/PRECISIONXYUNIT;
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2)/PRECISIONXYUNIT;
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2)/PRECISIONDUNIT;
// 				}
// 				else
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2);
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2);
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2);
// 				}
// 				m_pVisionASM->SetAlignerPrecision2(pos);	
// 			}
// 			UpdateAlignPrecisionInfo2();
// 		}
// 	}
}

void CDlgVisionRunAlignInfo::OnEnSetfocusEditValTargetPrecisionD2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bSysKeyboardEnabled)
// 	{
// 		m_bKillFocusEnabled = FALSE;
// 		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);
// 
// 		CRect rcWnd;
// 		CString strInput, strOutput;
// 		GetDlgItem(IDC_EDIT_VAL_TARGET_PRECISION_D2)->GetWindowRect(&rcWnd);
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D2, strInput);
// 
// 		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D2, strOutput);
// 			CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
// 			int nCurProIdx = pTotalProdData->GetCurProductIndex();
// 			CString strLog;
// 			strLog.Format(_T("主界面修改工位%d产品%d[设置精度]的[D2]由%s到%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInput,strOutput);
// 			g_opLogFile.SaveLogText(strLog,2);
// 		}
// 		m_bKillFocusEnabled = TRUE;
// 		OnEnKillfocusEditValTargetPrecisionD2();
// 	}
}

void CDlgVisionRunAlignInfo::OnEnKillfocusEditValTargetPrecisionD2()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_bKillFocusEnabled)
// 	{
// 		CString strVal;
// 		GetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D2, strVal);
// 		if (!IsStringPositiveNumerical(strVal))	//zzc 精度为正值才有意义
// 		{
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D2, m_strEditTargetPrecisionD2);
// 		}
// 		else
// 		{
// 			m_strEditTargetPrecisionD2.Format(_T("%.4f"), atof(strVal));
// 			SetDlgItemText(IDC_EDIT_VAL_TARGET_PRECISION_D2, m_strEditTargetPrecisionD2);
// 			if (m_pVisionASM != NULL)
// 			{
// 				CCoordPos pos;
// 				if (m_pParent->m_pParent->m_SysOptionConfig.m_bChange2um)
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2)/PRECISIONXYUNIT;
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2)/PRECISIONXYUNIT;
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2)/PRECISIONDUNIT;
// 				}
// 				else
// 				{
// 					pos.m_dPosX = atof(m_strEditTargetPrecisionX2);
// 					pos.m_dPosY = atof(m_strEditTargetPrecisionY2);
// 					pos.m_dAngle = atof(m_strEditTargetPrecisionD2);
// 				}
// 				m_pVisionASM->SetAlignerPrecision2(pos);	
// 			}
// 			UpdateAlignPrecisionInfo2();
// 		}
// 	}
}

void CDlgVisionRunAlignInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pMainLayout)
	{
		m_pMainLayout->OnSize();
	}
}


LRESULT CDlgVisionRunAlignInfo::OnDlgdestroy(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 1:
		{
			if (m_pDlgCalibCheckBySize !=NULL)
			{
				m_pVisionASM->m_bIsExecuteSizeCheck = FALSE;
				m_pVisionASM->m_nExecuteCheckTimes = 0;
				m_pDlgCalibCheckBySize->DestroyWindow();
				delete m_pDlgCalibCheckBySize;
				m_pDlgCalibCheckBySize = NULL;
			}
		};
		break;
	case 2:
		{
			if (m_pDlgCalibCheckByFixedTargetAligner !=NULL)
			{
				m_pVisionASM->m_bIsExecuteFixTargetCheck = FALSE;
				m_pVisionASM->m_nExecuteCheckTimes = 0;
				m_pDlgCalibCheckByFixedTargetAligner->DestroyWindow();
				delete m_pDlgCalibCheckByFixedTargetAligner;
				m_pDlgCalibCheckByFixedTargetAligner = NULL;
			}
		};
		break;
	default:
		{

		};
		break;
	}

	if (!GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_FIXED_TARGET)->EnableWindow(TRUE);
	}
	if (!GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BTN_CALIB_CHECK_BY_SIZE)->EnableWindow(TRUE);
	}
	if (!GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->IsWindowEnabled())
	{
		GetDlgItem(IDC_BTN_RELEVANCE_CHECK_BY_SIZE)->EnableWindow(TRUE);
	}


	return 0;
}


LRESULT CDlgVisionRunAlignInfo::OnCalcProductSize(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 1:
		{
			if (m_pDlgCalibCheckBySize == NULL)
			{
				return 0;
			}

			switch(m_pDlgCalibCheckBySize->m_nTypeIndex)
			{
			case 0:
				{
					CString strLog = _T("");
					CString strTemp = _T(""); 

					AlignerObjectSizeInfo tmp_AlignerObjectSizeInfo; 
					m_pVisionASM->GetAlignerObjectSizeInfo(tmp_AlignerObjectSizeInfo);
					m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize = tmp_AlignerObjectSizeInfo.m_vdObjectSize;
					m_pDlgCalibCheckBySize->UpdateAlignerObjectSizeInfo();

					CMarkPlatformPos mpTargetMarkPos;
					vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();

					if (tmpAlignTool !=NULL)
					{
						mpTargetMarkPos = tmpAlignTool->GetObjectMarkPlatformPosForCheck();
					}

					m_pVisionASM->m_nExecuteCheckTimes = m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount();
					strLog.Format(_T("第%d次对象尺寸验证\n"),m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount());

					// 记录距离尺寸
					for (int it=0;it<m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize.size() && it<m_pDlgCalibCheckBySize->m_nDistanceNums;it++)
					{
						strTemp.Format(_T("%s:%f\n"),m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vstrObjectSizeName.at(it),m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize.at(it));
						strLog += strTemp;
					}

					m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath4,strLog);

					// 记录图像坐标和平台坐标
					for (int it=0;it<m_pDlgCalibCheckBySize->m_nSeachNums;it++)
					{
						strLog.Empty();
						strTemp.Empty();

						strTemp .Format( _T("位置%d,图像坐标-平台坐标:"),it+1);
						strLog += strTemp;

						((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(it).ToString(strTemp);
						strLog += strTemp;

						mpTargetMarkPos.GetMarkPlatformPos(it).ToString(strTemp);
						strLog += strTemp;

						m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath4,strLog);
					}
				}
				break;

			case 1:
				{
					CString strLog = _T("");
					CString strTemp = _T(""); 

					AlignerTargetSizeInfo tmp_AlignerTargetSizeInfo; 
					m_pVisionASM->GetAlignerTargetSizeInfo(tmp_AlignerTargetSizeInfo);
					m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize = tmp_AlignerTargetSizeInfo.m_vdTargetSize;
					m_pDlgCalibCheckBySize->UpdateAlignerTargetSizeInfo();


					SysPlatformInfo platformInfo;
					m_pVisionASM->GetSysPlatformInfo(platformInfo);
					int nBaseNums = 0;
					if (platformInfo.m_bTargetObjectCamSeparate)
					{
						nBaseNums = m_pVisionASM->GetPosNum()/2;
					}

					CMarkPlatformPos mpTargetMarkPos;
					vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
					if (tmpAlignTool !=NULL)
					{
						mpTargetMarkPos = tmpAlignTool->GetTargetMarkPlatformPosForCheck();

					}

					m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(2) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+nBaseNums).GetPosX();
					m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(3) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+nBaseNums).GetPosY();


					strLog.Format(_T("第%d次目标尺寸验证\n"),m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount());

					for (int it=0;it<m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize.size() && it<m_pDlgCalibCheckBySize->m_nDistanceNums;it++)
					{
						strTemp.Format(_T("%s:%f,"),m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vstrTargetSizeName.at(it),m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize.at(it));
						strLog += strTemp;
					}
					m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath4,strLog);

					for (int it=0;it<m_pDlgCalibCheckBySize->m_nSeachNums;it++)
					{
						strLog.Empty();
						strTemp.Empty();

						strTemp .Format( _T("位置%d,图像坐标-平台坐标:"),it+m_pVisionASM->GetPosNum()/2+1);
						strLog += strTemp;

						((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(it+nBaseNums).ToString(strTemp);
						strLog += strTemp;

						mpTargetMarkPos.GetMarkPlatformPos(it+ nBaseNums).ToString(strTemp);
						strLog += strTemp;

						m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath4,strLog);

					}
				}
				break;

			default:
				{

				}
				break;
			}
		}
		break;

	case 2:
		{
			if (m_pDlgCalibCheckByFixedTargetAligner == NULL)
			{
				return 0;
			}

			CString strLog = _T("");
			CString strTemp = _T(""); 

			m_pVisionASM->GetAlignerProcessInfo(m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo);
			m_pDlgCalibCheckByFixedTargetAligner->UpdateProcessInfo();

			// 保存日志
			SysPlatformInfo platformInfo;
			m_pVisionASM->GetSysPlatformInfo(platformInfo);

			CMarkPlatformPos mpTargetMarkPos;
			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
			if (tmpAlignTool != NULL)
			{
				mpTargetMarkPos = tmpAlignTool->GetTargetMarkPlatformPosForCheck();
			}

			// 目标位置图像,平台坐标
			for (int it=0;it<m_pDlgCalibCheckByFixedTargetAligner->m_nSeachNums;it++)
			{
				strLog.Empty();
				strTemp.Empty();

				int k = it;
				if (platformInfo.m_bTargetObjectCamSeparate)
				{
					k= it + m_pVisionASM->GetPosNum()/2;
				}

				strTemp .Format( _T("目标搜索,位置%d,图像坐标-平台坐标:"),k+1);
				strLog += strTemp;

				((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(it).ToString(strTemp);
				strLog += strTemp;

				mpTargetMarkPos.GetMarkPlatformPos(it).ToString(strTemp);
				strLog += strTemp;

				m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath5,strLog);

			}

			strLog.Empty();
			strTemp.Empty();

			// 对象位置图像,平台坐标
			if (tmpAlignTool !=NULL)
			{
				mpTargetMarkPos = tmpAlignTool->GetObjectMarkPlatformPosForCheck();
			}

			for (int it=0;it<m_pDlgCalibCheckByFixedTargetAligner->m_nSeachNums;it++)
			{
				strTemp.Format( _T("对象搜索,位置%d,图像坐标-平台坐标:"),it+1);
				strLog += strTemp;

				((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(it).ToString(strTemp);
				strLog += strTemp;

				mpTargetMarkPos.GetMarkPlatformPos(it).ToString(strTemp);
				strLog += strTemp;

				m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath5,strLog);

			}

			strLog.Empty();
			strTemp.Empty();


			strTemp.Format(_T("对位平台偏移量：X:%.3f,Y:%.3f,D:%.3f"),m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dXMove,m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dYMove,m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dDMove);
			strLog += strTemp;

			m_pVisionASM->VisionAlignLogRecord(m_pVisionASM->m_strCalibrateProcessPath5,strLog);

		}
		break;

	default:
		{

		}
		break;
	}

	return 0;
}

// LRESULT CDlgVisionRunAlignInfo::OnCalcProductSize(WPARAM wParam, LPARAM lParam)
// {
// 	switch(wParam)
// 	{
// 	case 1:
// 		{
// 			if (m_pDlgCalibCheckBySize !=NULL)
// 			{
// 				if (m_pDlgCalibCheckBySize->m_nTypeIndex==0)
// 				{
// 
// 					AlignerObjectSizeInfo tmp_AlignerObjectSizeInfo; 
// 					m_pVisionASM->GetAlignerObjectSizeInfo(tmp_AlignerObjectSizeInfo);
// 					m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize = tmp_AlignerObjectSizeInfo.m_vdObjectSize;
// 
// 					m_pDlgCalibCheckBySize->UpdateAlignerObjectSizeInfo();
// 
// 					CMarkPlatformPos mpTargetMarkPos;
// 					CMarkPlatformPos mpTargetMarkPos2;
// 
// 					BOOL bISReConstruct = FALSE;
// 
// 					vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
// 					if (tmpAlignTool !=NULL)
// 					{
// 						mpTargetMarkPos = tmpAlignTool->GetObjectMarkPlatformPos(FALSE);
// 
// 						bISReConstruct = tmpAlignTool->IsObjectReConstruct();
// 						if (bISReConstruct)
// 						{
// 							mpTargetMarkPos2 =tmpAlignTool->GetObjectMarkPlatformPos(TRUE);
// 						}
// 
// 					}
// 
// 
// 					CString strLog = _T("");
// 					m_pVisionASM->m_nExecuteCheckTimes = m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount();
// 					strLog.Format(_T("第%d次对象尺寸验证\n"),m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount());
// 					CString StrTmp = _T(""); 
// 					// 记录距离尺寸
// 					for (int it=0;it<m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize.size() && it<m_pDlgCalibCheckBySize->m_nDistanceNums;it++)
// 					{
// 						StrTmp.Format(_T("%s:%f,"),m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vstrObjectSizeName.at(it),m_pDlgCalibCheckBySize->m_AlignerObjectSizeInfo.m_vdObjectSize.at(it));
// 						strLog += StrTmp;
// 					}
// 
// 					strLog += _T("\n");
// 					// 记录图像和平台坐标点含辅助点
// 					for (int it=0;it<m_pDlgCalibCheckBySize->m_nSeachNums;it++)
// 					{
// 						StrTmp .Format( _T("标定尺寸验证--位置%d\n图像坐标:"),it+1);
// 						strLog += StrTmp;
// 						((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(it).ToString(StrTmp);
// 						strLog += StrTmp;
// 
// 						if (bISReConstruct)
// 						{
// 							StrTmp=_T("重构前平台坐标:");
// 						}
// 						else
// 						{
// 							StrTmp=_T("平台坐标:");
// 						}
// 
// 						strLog += StrTmp;
// 						mpTargetMarkPos.GetMarkPlatformPos(it).ToString(StrTmp);
// 						strLog += StrTmp;
// 
// 						if (bISReConstruct)
// 						{
// 							StrTmp=_T("重构后平台坐标:");
// 							strLog += StrTmp;
// 							mpTargetMarkPos2.GetMarkPlatformPos(it).ToString(StrTmp);
// 							strLog += StrTmp;
// 						}
// 
// 					}
// 
// 					CString SaveFilePath = _T("D:\\VisionASMLog\\");
// 					DWORD dwRet = GetFileAttributes(SaveFilePath);	
// 					if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 					{
// 						// 暂时不考虑创建文件夹失败情况
// 						CreateDirectory(SaveFilePath, NULL);
// 					}
// 
// 					SysPlatformInfo sysPlatformInfo;
// 					m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
// 
// 					CVisionASMConfig *vsConfig;
// 					m_pVisionASM->GetVSconfig(vsConfig);
// 
// 					if ((NULL !=vsConfig))
// 					{
// 						if (sysPlatformInfo.m_bProductFaChd)
// 						{
// 							int nCurProductIndexFa;
// 							int nCurProductIndexCh;
// 							vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
// 							StrTmp.Format(_T("工位%d_大品种%03d_小品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 								nCurProductIndexFa+1,nCurProductIndexCh+1);
// 						}
// 						else
// 						{
// 							StrTmp.Format(_T("工位%d_品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 								vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
// 						}
// 
// 					}
// 
// 
// 					SaveFilePath += StrTmp;
// 					m_pVisionASM->VisionAlignLogRecord(SaveFilePath,strLog);
// 				}
// 				else
// 				{
// 					AlignerTargetSizeInfo tmp_AlignerTargetSizeInfo; 
// 					m_pVisionASM->GetAlignerTargetSizeInfo(tmp_AlignerTargetSizeInfo);
// 					m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize = tmp_AlignerTargetSizeInfo.m_vdTargetSize;
// 					m_pDlgCalibCheckBySize->UpdateAlignerTargetSizeInfo();
// 
// 
// 					SysPlatformInfo platformInfo;
// 					m_pVisionASM->GetSysPlatformInfo(platformInfo);
// 					int nBaseNums = 0;
// 					if (platformInfo.m_bTargetObjectCamSeparate)
// 					{
// 						nBaseNums = m_pVisionASM->GetPosNum()/2;
// 					}
// 
// 					CMarkPlatformPos mpTargetMarkPos;
// 					CMarkPlatformPos mpTargetMarkPos2;
// 					BOOL bISReConstruct = FALSE;
// 					vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
// 					if (tmpAlignTool !=NULL)
// 					{
// 						mpTargetMarkPos = tmpAlignTool->GetTargetMarkPlatformPos();
// 						bISReConstruct = tmpAlignTool->IsTargetReConstruct();
// 						if (bISReConstruct)
// 						{
// 							mpTargetMarkPos2 = tmpAlignTool->GetTargetMarkPlatformPosForCheck();
// 						}
// 
// 					}
// 
// 					m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(2) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+nBaseNums).GetPosX();
// 					m_pDlgCalibCheckBySize->m_vImagePosAndPlatformPos.at(3) = mpTargetMarkPos.GetMarkPlatformPos(m_pDlgCalibCheckBySize->m_nCurSearchIndex+nBaseNums).GetPosY();
// 
// 					CString strLog = _T("");
// 					strLog.Format(_T("第%d次目标尺寸验证\n"),m_pDlgCalibCheckBySize->m_ListObjecSize.GetItemCount());
// 					CString StrTmp = _T(""); 
// 
// 					for (int it=0;it<m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize.size() && it<m_pDlgCalibCheckBySize->m_nDistanceNums;it++)
// 					{
// 						StrTmp.Format(_T("%s:%f,"),m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vstrTargetSizeName.at(it),m_pDlgCalibCheckBySize->m_AlignerTargetSizeInfo.m_vdTargetSize.at(it));
// 						strLog += StrTmp;
// 					}
// 					strLog +=_T("\n");
// 					for (int it=0;it<m_pDlgCalibCheckBySize->m_nSeachNums;it++)
// 					{
// 						StrTmp .Format( _T("关联尺寸验证--位置%d\n图像坐标:"),it+m_pVisionASM->GetPosNum()/2+1);
// 						strLog += StrTmp;
// 						((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(it+nBaseNums).ToString(StrTmp);
// 						strLog += StrTmp;
// 
// 						if (bISReConstruct)
// 						{
// 							StrTmp=_T("重构前平台坐标:");
// 						}
// 						else
// 						{
// 							StrTmp=_T("平台坐标:");
// 						}
// 
// 						strLog += StrTmp;
// 						mpTargetMarkPos.GetMarkPlatformPos(it+ nBaseNums).ToString(StrTmp);
// 						strLog += StrTmp;
// 
// 						if (bISReConstruct)
// 						{
// 							StrTmp=_T("重构后平台坐标:");
// 							strLog += StrTmp;
// 							mpTargetMarkPos2.GetMarkPlatformPos(it+ nBaseNums).ToString(StrTmp);
// 							strLog += StrTmp;
// 						}
// 					}
// 
// 
// 
// 					// 保存日志
// 					CString SaveFilePath = _T("D:\\VisionASMLog\\");
// 					DWORD dwRet = GetFileAttributes(SaveFilePath);	
// 					if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 					{
// 						CreateDirectory(SaveFilePath, NULL);
// 					}
// 
// 
// 					SysPlatformInfo sysPlatformInfo;
// 					m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
// 
// 					CVisionASMConfig *vsConfig;
// 					m_pVisionASM->GetVSconfig(vsConfig);
// 
// 					if ((NULL !=vsConfig))
// 					{
// 						if (sysPlatformInfo.m_bProductFaChd)
// 						{
// 							int nCurProductIndexFa;
// 							int nCurProductIndexCh;
// 							vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
// 							StrTmp.Format(_T("工位%d_大品种%03d_小品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 								nCurProductIndexFa+1,nCurProductIndexCh+1);
// 						}
// 						else
// 						{
// 							StrTmp.Format(_T("工位%d_品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 								vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
// 						}
// 
// 					}
// 
// 
// 					SaveFilePath += StrTmp;
// 					m_pVisionASM->VisionAlignLogRecord(SaveFilePath,strLog);
// 
// 				}
// 			}
// 		};
// 		break;
// 	case 2:
// 		{
// 			if (m_pDlgCalibCheckByFixedTargetAligner!=NULL)
// 			{
// 
// 				m_pVisionASM->GetAlignerProcessInfo(m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo);
// 				m_pDlgCalibCheckByFixedTargetAligner->UpdateProcessInfo();
// 
// 				// 保存日志
// 
// 
// 
// 				SysPlatformInfo platformInfo;
// 				m_pVisionASM->GetSysPlatformInfo(platformInfo);
// 				CMarkPlatformPos mpTargetMarkPos;
// 				CMarkPlatformPos mpTargetMarkPos2;
// 				vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
// 				BOOL bISReConstruct = FALSE;
// 				if (tmpAlignTool !=NULL)
// 				{
// 					bISReConstruct = tmpAlignTool->IsTargetReConstruct();
// 					if (bISReConstruct)
// 					{
// 						mpTargetMarkPos = tmpAlignTool->GetTargetMarkPlatformPosForCheck();
// 					}
// 					mpTargetMarkPos2 = tmpAlignTool->GetTargetMarkPlatformPos();
// 				}
// 
// 				CString strLog = _T("");
// 				CString StrTmp = _T(""); 
// 				strLog += _T("\n");
// 				// 目标位置图像平台坐标
// 				for (int it=0;it<m_pDlgCalibCheckByFixedTargetAligner->m_nSeachNums;it++)
// 				{
// 					int k = it;
// 					if (platformInfo.m_bTargetObjectCamSeparate)
// 					{
// 						k= it + m_pVisionASM->GetPosNum()/2;
// 					}
// 
// 					StrTmp .Format( _T("标定固定目标对位验证--目标搜索 位置%d\n图像坐标:"),k+1);
// 					strLog += StrTmp;
// 					((vcXYDVisionAlign*)m_pVisionASM)->GetTargetMarkImagePos().GetMarkImagePos(it).ToString(StrTmp);
// 					strLog += StrTmp;
// 					if (bISReConstruct)
// 					{
// 						StrTmp=_T("重构前平台坐标:");
// 					}
// 					else
// 					{
// 						StrTmp=_T("平台坐标:");
// 					}
// 					strLog += StrTmp;
// 					mpTargetMarkPos2.GetMarkPlatformPos(it).ToString(StrTmp);
// 					strLog += StrTmp;
// 					if (bISReConstruct)
// 					{
// 						StrTmp=_T("重构后平台坐标:");
// 						strLog += StrTmp;
// 						mpTargetMarkPos.GetMarkPlatformPos(it).ToString(StrTmp);
// 						strLog += StrTmp;
// 					}
// 
// 				}
// 
// 				strLog += _T("\n");
// 				bISReConstruct = FALSE;
// 				// 对象位置图像平台坐标
// 				if (tmpAlignTool !=NULL)
// 				{
// 					mpTargetMarkPos = tmpAlignTool->GetObjectMarkPlatformPos(FALSE);
// 					bISReConstruct = tmpAlignTool->IsObjectReConstruct();
// 					if (bISReConstruct)
// 					{
// 						mpTargetMarkPos2 =tmpAlignTool->GetObjectMarkPlatformPos(TRUE);
// 					}
// 
// 
// 				}
// 
// 				for (int it=0;it<m_pDlgCalibCheckByFixedTargetAligner->m_nSeachNums;it++)
// 				{
// 					StrTmp .Format( _T("标定固定目标对位验证--对象搜索 位置%d\n图像坐标:"),it+1);
// 					strLog += StrTmp;
// 					((vcXYDVisionAlign*)m_pVisionASM)->GetObjectMarkImagePos().GetMarkImagePos(it).ToString(StrTmp);
// 					strLog += StrTmp;
// 					if (bISReConstruct)
// 					{
// 						StrTmp=_T("重构前平台坐标:");
// 					}
// 					else
// 					{
// 						StrTmp=_T("平台坐标:");
// 					}
// 					strLog += StrTmp;
// 					mpTargetMarkPos.GetMarkPlatformPos(it).ToString(StrTmp);
// 					strLog += StrTmp;
// 
// 					if (bISReConstruct)
// 					{
// 						StrTmp=_T("重构后平台坐标:");
// 						strLog += StrTmp;
// 						mpTargetMarkPos2.GetMarkPlatformPos(it).ToString(StrTmp);
// 						strLog += StrTmp;
// 					}
// 				}
// 				strLog += _T("\n");
// 
// 				StrTmp.Format(_T("对位平台偏移量：X:%.3f,Y:%.3f,D:%.3f"),m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dXMove,m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dYMove,m_pDlgCalibCheckByFixedTargetAligner->m_AlignerProcessInfo.dDMove);
// 				strLog += StrTmp;
// 
// 				// 日志路径
// 				CString SaveFilePath = _T("D:\\VisionASMLog\\");
// 				DWORD dwRet = GetFileAttributes(SaveFilePath);	
// 				if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 				{
// 					// 暂时不考虑创建文件夹失败情况
// 					CreateDirectory(SaveFilePath, NULL);
// 				}
// 
// 				SysPlatformInfo sysPlatformInfo;
// 				m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
// 				CVisionASMConfig *vsConfig;
// 				m_pVisionASM->GetVSconfig(vsConfig);
// 
// 				if ((NULL !=vsConfig))
// 				{
// 					if (sysPlatformInfo.m_bProductFaChd)
// 					{
// 						int nCurProductIndexFa;
// 						int nCurProductIndexCh;
// 						vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
// 						StrTmp.Format(_T("工位%d_大品种%03d_小品种%03d_FixedTargetCalibrationVerification.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 							nCurProductIndexFa+1,nCurProductIndexCh+1);
// 					}
// 					else
// 					{
// 						StrTmp.Format(_T("工位%d_品种%03d_FixedTargetCalibrationVerification.txt"),sysPlatformInfo.m_nPlatformIndex+1,
// 							vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
// 					}
// 
// 				}
// 
// 				SaveFilePath += StrTmp;
// 				m_pVisionASM->VisionAlignLogRecord(SaveFilePath,strLog);
// 
// 			}
// 		};
// 		break;
// 	default:
// 		{
// 
// 		};
// 		break;
// 	}
// 	return 0;
// }



void CDlgVisionRunAlignInfo::LoadOffsetDirectionPicture()
{
	//TCHAR szPath[MAX_PATH];
	//GetModuleFileName(NULL, szPath, MAX_PATH);

	//CString strCurWorkDir = szPath;
	//strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));
	if (m_pVisionASM == NULL)
	{
		return ;
	}
	CPlatformOptionInfo optionInfo;
	m_pVisionASM->GetPlatformOptionInfo(optionInfo);
	//HBITMAP hbmp;

	if (optionInfo.m_bEnableCommMultiCalibExtension)
	{
		GetDlgItem(IDC_RADIO_GRIP1)->ShowWindow(TRUE); 
		GetDlgItem(IDC_RADIO_GRIP2)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_GRIP1)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_GRIP2)->ShowWindow(FALSE);
	}

	CString strEXEDir;
	CString strDir;
	CFileFind findFile;
	BOOL IsFild, IsDir;
	SysPlatformInfo platInfo;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();
	strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\'));   
	m_pVisionASM->GetSysPlatformInfo(platInfo);
	strDir = strEXEDir + platInfo.m_strPlatformDetailSavePath; //Bin\VSConfig\WS0
	IsFild = FALSE;
	IsDir = FALSE;
	IsFild = findFile.FindFile(strDir);
	while (IsFild)
	{
		IsFild = findFile.FindNextFile();
		IsDir = findFile.IsDirectory();
		if(IsDir) break;
	}
	if (IsDir && optionInfo.m_strOffsetPicturePath != _T(""))
	{
		strDir = strDir + _T("\\")+optionInfo.m_strOffsetPicturePath;
		DeleteObject(m_hBmpOffsetDir);
		m_hBmpOffsetDir = NULL;
		GetDlgItem(IDC_STATIC_OFFSET_DIRECTION)->ShowWindow(SW_SHOW);
		m_pMainLayout->OnSize();
		CRect rtWnd;
		GetDlgItem(IDC_STATIC_OFFSET_DIRECTION)->GetClientRect(rtWnd);
		/*hbmp = (HBITMAP)::LoadImage(NULL,strCurWorkDir+"\\OffSet.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE);*/
		m_hBmpOffsetDir = (HBITMAP)::LoadImage(NULL,strDir,IMAGE_BITMAP,rtWnd.Width(),rtWnd.Height(),LR_LOADFROMFILE|LR_DEFAULTSIZE);
		if (m_hBmpOffsetDir)
		{
			((CStatic*)GetDlgItem(IDC_STATIC_OFFSET_DIRECTION))->SetBitmap((HBITMAP)m_hBmpOffsetDir);
		}
	}	
}

void CDlgVisionRunAlignInfo::UpdateDialogLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent && m_pParent->m_pParent && m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_VISION_RUN_ALIGN_INFO") == strDlgID)
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

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		int m_nPlatformIndex = platformInfo.m_nPlatformIndex;
		if(m_pParent && m_pParent->m_pParent && m_nPlatformIndex<6 && m_pParent->m_pParent->m_SysOptionConfig.m_bPlatformCustomNameEnable)
		{
			GetDlgItem(IDC_STATIC_GRP_ALIGNER_OFFSET)->SetWindowText(m_pParent->m_pParent->m_SysOptionConfig.m_strPlatformCustomName[m_nPlatformIndex] + _T("补偿量"));
			//SetDlgItemText(IDC_STATIC_GRP_ALIGNER_OFFSET,m_pParent->m_pParent->m_SysOptionConfig.m_strPlatformCustomName[m_nPlatformIndex]);
		}
	}
}

void CDlgVisionRunAlignInfo::OnBnClickedBtnAlignOffsetByAreaSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pVisionASM)
	{
		return;
	}

	if (m_pVisionASM->GetTotalProuductData()->GetCount() <= 0)
	{
		return;
	}



	CDlgAlignOffsetByAreaSet dlg;
	dlg.SetParentWnd(this);
	dlg.SetVisionAlignerRef(m_pVisionASM);
	dlg.SetSysInfoStringsRef(m_pParent->m_pParent->m_psaSysInfoStrings);
	dlg.SetKeyBoard(m_bSysKeyboardEnabled);
	dlg.DoModal();

	int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
	if (pProduct->m_pAlignerParam->GetAlignOffsetByArea().m_bEnable)
	{
		std::vector<sc2Vector> vvPoint;
		m_pVisionASM->m_cVisionAlignGui.SetAllPosAlignOffsetByArea(vvPoint);
		m_pVisionASM->GetAlignTool()->GetProductAlignerParam()->SetAlignOffsetByArea(pProduct->m_pAlignerParam->GetAlignOffsetByArea());
	}
	else
	{
		m_pVisionASM->m_cVisionAlignGui.SetAllPosAlignOffsetByAreaInvisible();
	}
}


void CDlgVisionRunAlignInfo::OnBnClickedRadEditGrip0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_strEditCurOffsetIndex = _T("1");
	SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, _T("1"));
	OnEnKillfocusEditValCurOffsetIndex();
}


void CDlgVisionRunAlignInfo::OnBnClickedRadEditGrip1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//m_strEditCurOffsetIndex = _T("1");
	SetDlgItemText(IDC_EDIT_VAL_CUR_OFFSET_INDEX, _T("1"));
	OnEnKillfocusEditValCurOffsetIndex();
}


void CDlgVisionRunAlignInfo::OnBnClickedBtnCalibCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pVisionASM->SetStatusBarInfo(TRUE,_T(""),_T(""));
	m_pVisionASM->PostMessageUpdateStatusBarInfo();

	CDlgCalibCopy dlg;
	dlg.m_bSysKeyboardEnabled = m_bSysKeyboardEnabled;

	if (dlg.DoModal() == IDOK)
	{
		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		if (pTotalProdData == NULL)
		{
			return;
		}
		int nCurProIdx = pTotalProdData->GetCurProductIndex();
		int nSourcePosCalib = dlg.m_nSourcePosCalib;
		int nTargetPosCalib = dlg.m_nTargetPosCalib;
		BOOL bEnableALLCalibCopy = dlg.m_bEnableALLCalibCopy;

		//拷贝标定信息
		std::vector<CCalibratedInfo*> vpAllCalibratedInfo;
		vpAllCalibratedInfo = m_pVisionASM ->GetAllCalibratedInfo();

		CString strInfo;
		BOOL bSuccess = TRUE;
		if (bEnableALLCalibCopy)  //位置解析
		{
			int nPosNum = m_pVisionASM ->GetPosNum();
			for (int i =0;i< nPosNum;++i)
			{
				std::vector<int> vnTargetPos;
				int nSourceCalibIndex = (nSourcePosCalib - 1) * nPosNum + i;
				int nTargetCalibIndex = (nTargetPosCalib - 1) * nPosNum + i;
				vnTargetPos.push_back(nTargetCalibIndex);
				bSuccess = bSuccess & m_pVisionASM ->SetSameCalibrateInfo(nSourceCalibIndex,vnTargetPos);
			}
			strInfo.Format(_T("扩展%d复制到扩展%d"),nSourcePosCalib,nTargetPosCalib);
		}
		else
		{
			std::vector<int> vnTargetPos;
			vnTargetPos.push_back(nTargetPosCalib-1);
			bSuccess = m_pVisionASM ->SetSameCalibrateInfo(nSourcePosCalib-1,vnTargetPos);
			strInfo.Format(_T("位置%d复制到位置%d"),nSourcePosCalib,nTargetPosCalib);
		}


		if (!bSuccess)
		{
			m_pVisionASM ->SetStatusBarInfo(FALSE, _T("标定复制："),strInfo +_T(" 失败"));
			m_pVisionASM ->PostMessageUpdateStatusBarInfo();
		}
		else
		{
			// 获取全部的标定信息
			vpAllCalibratedInfo = m_pVisionASM ->GetAllCalibratedInfo();
			// 设置标定后的信息到对位工具,为对位做准备，并保存到配置文件中
			m_pVisionASM ->SetCurCalibratedInfo(vpAllCalibratedInfo);	
			// 保存标定后信息到当前产品中及配置文件中
			m_pVisionASM ->SaveCalibratedInfoToCurProduct(vpAllCalibratedInfo);

			m_pVisionASM ->SetStatusBarInfo(TRUE, _T("标定复制："),strInfo + _T(" 成功"));
			m_pVisionASM ->PostMessageUpdateStatusBarInfo();
		}

		//日志记录
		CString strLog;
		strLog.Format(_T("主界面操作工位%d产品%d标定信息%s\n"),m_pParent->m_nPlatformID+1,nCurProIdx+1,strInfo);
		g_opLogFile.SaveLogText(strLog,2);
	}
	
}
