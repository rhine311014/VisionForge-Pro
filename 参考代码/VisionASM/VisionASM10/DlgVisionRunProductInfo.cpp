// DlgVisionRunProductInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionRunProductInfo.h"
#include "afxdialogex.h"
#include "DlgVisionRun.h"


// CDlgVisionRunProductInfo 对话框

IMPLEMENT_DYNAMIC(CDlgVisionRunProductInfo, CDialogEx)

CDlgVisionRunProductInfo::CDlgVisionRunProductInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVisionRunProductInfo::IDD, pParent)

	,m_strEditObjectStdSize0("0.0000")
	,m_strEditObjectStdSize1("0.0000")
	,m_strEditObjectStdSize2("0.0000")
	,m_strEditObjectStdSize3("0.0000")
	,m_strEditObjectStdSize4("0.0000")
	,m_strEditObjectStdSize5("0.0000")
	,m_strEditObjectStdAngle0("0.0000")
	,m_strEditObjectStdAngle1("0.0000")
	,m_strEditObjectStdAngle2("0.0000")
	,m_strEditObjectStdAngle3("0.0000")

	,m_strEditTargetStdSize0("0.0000")
	,m_strEditTargetStdSize1("0.0000")
	,m_strEditTargetStdSize2("0.0000")
	,m_strEditTargetStdSize3("0.0000")
	,m_strEditTargetStdSize4("0.0000")
	,m_strEditTargetStdSize5("0.0000")
	,m_strEditTargetStdAngle0("0.0000")
	,m_strEditTargetStdAngle1("0.0000")
	,m_strEditTargetStdAngle2("0.0000")
	,m_strEditTargetStdAngle3("0.0000")


	,m_strEditCurObjectSize0("0.0000")
	,m_strEditCurObjectSize1("0.0000")
	,m_strEditCurObjectSize2("0.0000")
	,m_strEditCurObjectSize3("0.0000")
	,m_strEditCurObjectSize4("0.0000")
	,m_strEditCurObjectSize5("0.0000")
	,m_strEditCurObjectAngle0("0.0000")
	,m_strEditCurObjectAngle1("0.0000")
	,m_strEditCurObjectAngle2("0.0000")
	,m_strEditCurObjectAngle3("0.0000")

	,m_strEditCurTargetSize0("0.0000")
	,m_strEditCurTargetSize1("0.0000")
	,m_strEditCurTargetSize2("0.0000")
	,m_strEditCurTargetSize3("0.0000")
	,m_strEditCurTargetSize4("0.0000")
	,m_strEditCurTargetSize5("0.0000")
	,m_strEditCurTargetAngle0("0.0000")
	,m_strEditCurTargetAngle1("0.0000")
	,m_strEditCurTargetAngle2("0.0000")
	,m_strEditCurTargetAngle3("0.0000")


{
	m_pParent = NULL;
	m_pVisionASM = NULL;

	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_nMultiCalibExtensionIndex = 0;
}

CDlgVisionRunProductInfo::~CDlgVisionRunProductInfo()
{
}

void CDlgVisionRunProductInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// 状态
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_0, m_labelCurObjectSizeCheckStatus0);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_1, m_labelCurObjectSizeCheckStatus1);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_2, m_labelCurObjectSizeCheckStatus2);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_3, m_labelCurObjectSizeCheckStatus3);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_4, m_labelCurObjectSizeCheckStatus4);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_5, m_labelCurObjectSizeCheckStatus5);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_ANGLE_CHECK_STATUS_0, m_labelCurObjectAngleCheckStatus0);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_ANGLE_CHECK_STATUS_1, m_labelCurObjectAngleCheckStatus1);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_ANGLE_CHECK_STATUS_2, m_labelCurObjectAngleCheckStatus2);
	DDX_Control(pDX, IDC_STATIC_CUR_OBJECT_ANGLE_CHECK_STATUS_3, m_labelCurObjectAngleCheckStatus3);

	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_0, m_labelCurTargetSizeCheckStatus0);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_1, m_labelCurTargetSizeCheckStatus1);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_2, m_labelCurTargetSizeCheckStatus2);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_3, m_labelCurTargetSizeCheckStatus3);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_4, m_labelCurTargetSizeCheckStatus4);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_5, m_labelCurTargetSizeCheckStatus5);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_ANGLE_CHECK_STATUS_0, m_labelCurTargetAngleCheckStatus0);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_ANGLE_CHECK_STATUS_1, m_labelCurTargetAngleCheckStatus1);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_ANGLE_CHECK_STATUS_2, m_labelCurTargetAngleCheckStatus2);
	DDX_Control(pDX, IDC_STATIC_CUR_TARGET_ANGLE_CHECK_STATUS_3, m_labelCurTargetAngleCheckStatus3);



	// 标准值
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_0, m_strEditObjectStdSize0);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_1, m_strEditObjectStdSize1);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_2, m_strEditObjectStdSize2);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_3, m_strEditObjectStdSize3);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_4, m_strEditObjectStdSize4);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_SIZE_5, m_strEditObjectStdSize5);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, m_strEditObjectStdAngle0);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, m_strEditObjectStdAngle1);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, m_strEditObjectStdAngle2);
	DDX_Text(pDX, IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, m_strEditObjectStdAngle3);

	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_0, m_strEditTargetStdSize0);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_1, m_strEditTargetStdSize1);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_2, m_strEditTargetStdSize2);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_3, m_strEditTargetStdSize3);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_4, m_strEditTargetStdSize4);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_SIZE_5, m_strEditTargetStdSize5);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_ANGLE_0, m_strEditTargetStdAngle0);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_ANGLE_1, m_strEditTargetStdAngle1);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_ANGLE_2, m_strEditTargetStdAngle2);
	DDX_Text(pDX, IDC_EDIT_VAL_TARGET_STD_ANGLE_3, m_strEditTargetStdAngle3);


	// 当前值1
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_0, m_editCurObjectSize0);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_1, m_editCurObjectSize1);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_2, m_editCurObjectSize2);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_3, m_editCurObjectSize3);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_4, m_editCurObjectSize4);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_5, m_editCurObjectSize5);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_0, m_editCurObjectAngle0);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_1, m_editCurObjectAngle1);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_2, m_editCurObjectAngle2);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_3, m_editCurObjectAngle3);

	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_0, m_editCurTargetSize0);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_1, m_editCurTargetSize1);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_2, m_editCurTargetSize2);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_3, m_editCurTargetSize3);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_4, m_editCurTargetSize4);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_5, m_editCurTargetSize5);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_0, m_editCurTargetAngle0);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_1, m_editCurTargetAngle1);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_2, m_editCurTargetAngle2);
	DDX_Control(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_3, m_editCurTargetAngle3);


	// 当前值2
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_0, m_strEditCurObjectSize0);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_1, m_strEditCurObjectSize1);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_2, m_strEditCurObjectSize2);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_3, m_strEditCurObjectSize3);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_4, m_strEditCurObjectSize4);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_SIZE_5, m_strEditCurObjectSize5);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_0, m_strEditCurObjectAngle0);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_1, m_strEditCurObjectAngle1);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_2, m_strEditCurObjectAngle2);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_OBJECT_ANGLE_3, m_strEditCurObjectAngle3);

	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_0, m_strEditCurTargetSize0);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_1, m_strEditCurTargetSize1);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_2, m_strEditCurTargetSize2);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_3, m_strEditCurTargetSize3);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_4, m_strEditCurTargetSize4);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_SIZE_5, m_strEditCurTargetSize5);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_0, m_strEditCurTargetAngle0);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_1, m_strEditCurTargetAngle1);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_2, m_strEditCurTargetAngle2);
	DDX_Text(pDX, IDC_EDIT_VAL_CUR_TARGET_ANGLE_3, m_strEditCurTargetAngle3);
	

	DDX_CBIndex(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_nMultiCalibExtensionIndex);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_INDEX, m_comboMultiCalibExtension);
}


BEGIN_MESSAGE_MAP(CDlgVisionRunProductInfo, CDialogEx)
	ON_WM_SIZE()

	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_0, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_1, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_2, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_3, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_4, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_5, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize5)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, &CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle3)

	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_0, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_1, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_2, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_3, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_4, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_SIZE_5, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize5)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, &CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle3)



	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize5)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle0)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, &CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle3)

	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize5)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle0)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, &CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle3)

	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_INDEX, &CDlgVisionRunProductInfo::OnCbnSelchangeCombo1)

END_MESSAGE_MAP()


// CDlgVisionRunProductInfo 消息处理程序
CWnd* CDlgVisionRunProductInfo::SetParentWnd( CDlgVisionRun* pParent )
{
	CWnd* pPrevParent = (CWnd*)m_pParent;
	m_pParent = pParent;
	return pPrevParent;
}

void CDlgVisionRunProductInfo::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	m_pVisionASM = pVisionASM;
}


void CDlgVisionRunProductInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	MoveDlgItem();
}


BOOL CDlgVisionRunProductInfo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgVisionRunProductInfo::MoveDlgItem(BOOL bMoveViewOnly /*= FALSE*/)
{
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
	}
}


void CDlgVisionRunProductInfo::UpdateCurProductInfo()
{
	m_strEditObjectStdSize0 = _T("0.0000");
	m_strEditObjectStdSize1 = _T("0.0000");
	m_strEditObjectStdSize2 = _T("0.0000");
	m_strEditObjectStdSize3 = _T("0.0000");
	m_strEditObjectStdSize4 = _T("0.0000");
	m_strEditObjectStdSize5 = _T("0.0000");
	m_strEditObjectStdAngle0 = _T("0.0000");
	m_strEditObjectStdAngle1 = _T("0.0000");
	m_strEditObjectStdAngle2 = _T("0.0000");
	m_strEditObjectStdAngle3 = _T("0.0000");

	m_strEditTargetStdSize0 = _T("0.0000");
	m_strEditTargetStdSize1 = _T("0.0000");
	m_strEditTargetStdSize2 = _T("0.0000");
	m_strEditTargetStdSize3 = _T("0.0000");
	m_strEditTargetStdSize4 = _T("0.0000");
	m_strEditTargetStdSize5 = _T("0.0000");
	m_strEditTargetStdAngle0 = _T("0.0000");
	m_strEditTargetStdAngle1 = _T("0.0000");
	m_strEditTargetStdAngle2 = _T("0.0000");
	m_strEditTargetStdAngle3 = _T("0.0000");



	m_strEditCurObjectSize0 = _T("0.0000");
	m_strEditCurObjectSize1 = _T("0.0000");
	m_strEditCurObjectSize2 = _T("0.0000");
	m_strEditCurObjectSize3 = _T("0.0000");
	m_strEditCurObjectSize4 = _T("0.0000");
	m_strEditCurObjectSize5 = _T("0.0000");
	m_strEditCurObjectAngle0 = _T("0.0000");
	m_strEditCurObjectAngle1 = _T("0.0000");
	m_strEditCurObjectAngle2 = _T("0.0000");
	m_strEditCurObjectAngle3 = _T("0.0000");

	m_strEditCurTargetSize0 = _T("0.0000");
	m_strEditCurTargetSize1 = _T("0.0000");
	m_strEditCurTargetSize2 = _T("0.0000");
	m_strEditCurTargetSize3 = _T("0.0000");
	m_strEditCurTargetSize4 = _T("0.0000");
	m_strEditCurTargetSize5 = _T("0.0000");
	m_strEditCurTargetAngle0 = _T("0.0000");
	m_strEditCurTargetAngle1 = _T("0.0000");
	m_strEditCurTargetAngle2 = _T("0.0000");
	m_strEditCurTargetAngle3 = _T("0.0000");


	COLORREF crUnCheck = RGB(255,255,0);

    //小数位数格式化
    CString strXY;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		int nCurProIdx = pTotalProdData->GetCurProductIndex();

        LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
        if (m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
        {
            lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,0));
        }
        if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
        {
            switch (lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_OBJECT_STD_SIZE)->SetWindowTextA(_T("Std Value(um)"));
                GetDlgItem(IDC_STATIC_TARGET_STD_SIZE)->SetWindowTextA(_T("Std Value(um)"));
                GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE)->SetWindowTextA(_T("Cur Value(um)"));
                GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE)->SetWindowTextA(_T("Cur Value(um)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
                
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_OBJECT_STD_SIZE)->SetWindowTextA(_T("标准值(um)"));
                GetDlgItem(IDC_STATIC_TARGET_STD_SIZE)->SetWindowTextA(_T("标准值(um)"));
                GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE)->SetWindowTextA(_T("当前值(um)"));
                GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE)->SetWindowTextA(_T("当前值(um)"));
                break;
            }   
        }
        else
        {
            switch (lgidLanguage)
            {
            case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
                GetDlgItem(IDC_STATIC_OBJECT_STD_SIZE)->SetWindowTextA(_T("Std Value(mm)"));
                GetDlgItem(IDC_STATIC_TARGET_STD_SIZE)->SetWindowTextA(_T("Std Value(mm)"));
                GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE)->SetWindowTextA(_T("Cur Value(mm)"));
                GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE)->SetWindowTextA(_T("Cur Value(mm)"));
                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):

                break;
            case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
            default:
                GetDlgItem(IDC_STATIC_OBJECT_STD_SIZE)->SetWindowTextA(_T("标准值(mm)"));
                GetDlgItem(IDC_STATIC_TARGET_STD_SIZE)->SetWindowTextA(_T("标准值(mm)"));
                GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE)->SetWindowTextA(_T("当前值(mm)"));
                GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE)->SetWindowTextA(_T("当前值(mm)"));
                break;
            }  
        }

		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
		{
			{
				if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
				{					
				}
				else
				{
				}

				CProductData curProduct = pTotalProdData->GetItem(nCurProIdx);
				vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
				int nShowNums = 0;
				if( tmpAlignTool != NULL)
				{
					nShowNums = tmpAlignTool->GetAlignExProductIndex();
				}
				m_AlignCheckInfo = curProduct.m_pAlignerParam->GetAlignCheckInfo(nShowNums);

                if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
                {
					m_strEditObjectStdSize0.Format(strXY, m_AlignCheckInfo.m_dObjectDis0*PRECISIONXYUNIT);
					m_strEditObjectStdSize1.Format(strXY, m_AlignCheckInfo.m_dObjectDis1*PRECISIONXYUNIT);
					m_strEditObjectStdSize2.Format(strXY, m_AlignCheckInfo.m_dObjectDis2*PRECISIONXYUNIT);
					m_strEditObjectStdSize3.Format(strXY, m_AlignCheckInfo.m_dObjectDis3*PRECISIONXYUNIT);
					m_strEditObjectStdSize4.Format(strXY, m_AlignCheckInfo.m_dObjectDis4*PRECISIONXYUNIT);
					m_strEditObjectStdSize5.Format(strXY, m_AlignCheckInfo.m_dObjectDis5*PRECISIONXYUNIT);
					m_strEditObjectStdAngle0.Format(strXY, m_AlignCheckInfo.m_dObjectAngle0*PRECISIONXYUNIT);
					m_strEditObjectStdAngle1.Format(strXY, m_AlignCheckInfo.m_dObjectAngle1*PRECISIONXYUNIT);
					m_strEditObjectStdAngle2.Format(strXY, m_AlignCheckInfo.m_dObjectAngle2*PRECISIONXYUNIT);
					m_strEditObjectStdAngle3.Format(strXY, m_AlignCheckInfo.m_dObjectAngle3*PRECISIONXYUNIT);

                    m_strEditTargetStdSize0.Format(strXY, m_AlignCheckInfo.m_dTargetDis0*PRECISIONXYUNIT);
                    m_strEditTargetStdSize1.Format(strXY, m_AlignCheckInfo.m_dTargetDis1*PRECISIONXYUNIT);
                    m_strEditTargetStdSize2.Format(strXY, m_AlignCheckInfo.m_dTargetDis2*PRECISIONXYUNIT);
                    m_strEditTargetStdSize3.Format(strXY, m_AlignCheckInfo.m_dTargetDis3*PRECISIONXYUNIT);
                    m_strEditTargetStdSize4.Format(strXY, m_AlignCheckInfo.m_dTargetDis4*PRECISIONXYUNIT);
                    m_strEditTargetStdSize5.Format(strXY, m_AlignCheckInfo.m_dTargetDis5*PRECISIONXYUNIT);
					m_strEditTargetStdAngle0.Format(strXY, m_AlignCheckInfo.m_dTargetAngle0*PRECISIONXYUNIT);
					m_strEditTargetStdAngle1.Format(strXY, m_AlignCheckInfo.m_dTargetAngle1*PRECISIONXYUNIT);
					m_strEditTargetStdAngle2.Format(strXY, m_AlignCheckInfo.m_dTargetAngle2*PRECISIONXYUNIT);
					m_strEditTargetStdAngle3.Format(strXY, m_AlignCheckInfo.m_dTargetAngle3*PRECISIONXYUNIT);


                }
                else
                {
                    m_strEditTargetStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis0);
                    m_strEditTargetStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis1);
                    m_strEditTargetStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis2);
                    m_strEditTargetStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis3);
                    m_strEditTargetStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis4);
                    m_strEditTargetStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis5);
					m_strEditTargetStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle0);
					m_strEditTargetStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle1);
					m_strEditTargetStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle2);
					m_strEditTargetStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle3);

                    m_strEditObjectStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis0);
                    m_strEditObjectStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis1);
                    m_strEditObjectStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis2);
                    m_strEditObjectStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis3);
                    m_strEditObjectStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis4);
                    m_strEditObjectStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis5);
					m_strEditObjectStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle0);
					m_strEditObjectStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle1);
					m_strEditObjectStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle2);
					m_strEditObjectStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle3);

                }

				m_comboMultiCalibExtension.SetCurSel(nShowNums);
			}
		}

		m_editCurTargetSize0.SetWindowText(m_strEditCurTargetSize0);
		m_editCurTargetSize1.SetWindowText(m_strEditCurTargetSize1);
		m_editCurTargetSize2.SetWindowText(m_strEditCurTargetSize2);
		m_editCurTargetSize3.SetWindowText(m_strEditCurTargetSize3);
		m_editCurTargetSize4.SetWindowText(m_strEditCurTargetSize4);
		m_editCurTargetSize5.SetWindowText(m_strEditCurTargetSize5);
		m_editCurTargetAngle0.SetWindowText(m_strEditCurTargetAngle0);
		m_editCurTargetAngle1.SetWindowText(m_strEditCurTargetAngle1);
		m_editCurTargetAngle2.SetWindowText(m_strEditCurTargetAngle2);
		m_editCurTargetAngle3.SetWindowText(m_strEditCurTargetAngle3);

		m_editCurObjectSize0.SetWindowText(m_strEditCurObjectSize0);
		m_editCurObjectSize1.SetWindowText(m_strEditCurObjectSize1);
		m_editCurObjectSize2.SetWindowText(m_strEditCurObjectSize2);
		m_editCurObjectSize3.SetWindowText(m_strEditCurObjectSize3);
		m_editCurObjectSize4.SetWindowText(m_strEditCurObjectSize4);
		m_editCurObjectSize5.SetWindowText(m_strEditCurObjectSize5);
		m_editCurObjectAngle0.SetWindowText(m_strEditCurObjectAngle0);
		m_editCurObjectAngle1.SetWindowText(m_strEditCurObjectAngle1);
		m_editCurObjectAngle2.SetWindowText(m_strEditCurObjectAngle2);
		m_editCurObjectAngle3.SetWindowText(m_strEditCurObjectAngle3);



		m_labelCurTargetSizeCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus3.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus4.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus5.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus3.SetBkColor(crUnCheck);

		m_labelCurObjectSizeCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus3.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus4.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus5.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus3.SetBkColor(crUnCheck);
		
	}

	UpdateData(FALSE);
}

void CDlgVisionRunProductInfo::UpdateDialogLanguage()
{
    //	return;
    LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
    if (m_pParent && m_pParent->m_pParent &&m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
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

        if(_T("IDD_DLG_VISION_RUN_PRODUCT_INFO") == strDlgID)
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


// 更新对位目标距离信息
void CDlgVisionRunProductInfo::UpdateAlignerTargetSizeInfo()
{
	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	//切换扩展产品时，更新到扩展产品的尺寸标准值
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditCurTargetSize0 = _T("0.0000");
	m_strEditCurTargetSize1 = _T("0.0000");
	m_strEditCurTargetSize2 = _T("0.0000");
	m_strEditCurTargetSize3 = _T("0.0000");
	m_strEditCurTargetSize4 = _T("0.0000");
	m_strEditCurTargetSize5 = _T("0.0000");

    //小数位数格式化
    CString strXY;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);

    if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
    {
        m_strEditCurTargetSize0.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[0]*PRECISIONXYUNIT);
        m_strEditCurTargetSize1.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[1]*PRECISIONXYUNIT);
        m_strEditCurTargetSize2.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[2]*PRECISIONXYUNIT);
        m_strEditCurTargetSize3.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[3]*PRECISIONXYUNIT);
        m_strEditCurTargetSize4.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[4]*PRECISIONXYUNIT);
        m_strEditCurTargetSize5.Format(strXY, m_AlignerTargetSizeInfo.m_vdTargetSize[5]*PRECISIONXYUNIT);
    }
    else
    {
	    m_strEditCurTargetSize0.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[0]);
	    m_strEditCurTargetSize1.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[1]);
	    m_strEditCurTargetSize2.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[2]);
	    m_strEditCurTargetSize3.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[3]);
	    m_strEditCurTargetSize4.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[4]);
	    m_strEditCurTargetSize5.Format(_T("%.4f"), m_AlignerTargetSizeInfo.m_vdTargetSize[5]);
    }

	m_editCurTargetSize0.SetWindowText(m_strEditCurTargetSize0);
	m_editCurTargetSize1.SetWindowText(m_strEditCurTargetSize1);
	m_editCurTargetSize2.SetWindowText(m_strEditCurTargetSize2);
	m_editCurTargetSize3.SetWindowText(m_strEditCurTargetSize3);
	m_editCurTargetSize4.SetWindowText(m_strEditCurTargetSize4);
	m_editCurTargetSize5.SetWindowText(m_strEditCurTargetSize5);

	std::vector<COLORREF> vCOLORREFs;
	vCOLORREFs.push_back(RGB(255,255,0));	// UnCheck;
	vCOLORREFs.push_back(RGB(0,255,0));		// CheckOK;
	vCOLORREFs.push_back(RGB(255,0,0));		// CheckNG;

	m_labelCurTargetSizeCheckStatus0.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[0]]);
	m_labelCurTargetSizeCheckStatus1.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[1]]);
	m_labelCurTargetSizeCheckStatus2.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[2]]);
	m_labelCurTargetSizeCheckStatus3.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[3]]);
	m_labelCurTargetSizeCheckStatus4.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[4]]);
	m_labelCurTargetSizeCheckStatus5.SetBkColor(vCOLORREFs[m_AlignerTargetSizeInfo.m_vnCheckStatus[5]]);

	//m_nMultiCalibExtensionIndex = m_AlignerTargetSizeInfo.m_nExProductIndex;
	//m_comboMultiCalibExtension.SetCurSel(m_nMultiCalibExtensionIndex);
}

// 更新对位对象距离信息
void CDlgVisionRunProductInfo::UpdateAlignerObjectSizeInfo()
{
	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditCurObjectSize0 = _T("0.0000");
	m_strEditCurObjectSize1 = _T("0.0000");
	m_strEditCurObjectSize2 = _T("0.0000");
	m_strEditCurObjectSize3 = _T("0.0000");
	m_strEditCurObjectSize4 = _T("0.0000");
	m_strEditCurObjectSize5 = _T("0.0000");

    //小数位数格式化
    CString strXY;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
    {
        m_strEditCurObjectSize0.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[0]*PRECISIONXYUNIT);
        m_strEditCurObjectSize1.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[1]*PRECISIONXYUNIT);
        m_strEditCurObjectSize2.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[2]*PRECISIONXYUNIT);
        m_strEditCurObjectSize3.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[3]*PRECISIONXYUNIT);
        m_strEditCurObjectSize4.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[4]*PRECISIONXYUNIT);
        m_strEditCurObjectSize5.Format(strXY, m_AlignerObjectSizeInfo.m_vdObjectSize[5]*PRECISIONXYUNIT);
    }
    else
    {
        m_strEditCurObjectSize0.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[0]);
        m_strEditCurObjectSize1.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[1]);
        m_strEditCurObjectSize2.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[2]);
        m_strEditCurObjectSize3.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[3]);
        m_strEditCurObjectSize4.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[4]);
        m_strEditCurObjectSize5.Format(_T("%.4f"), m_AlignerObjectSizeInfo.m_vdObjectSize[5]);
    }

	m_editCurObjectSize0.SetWindowText(m_strEditCurObjectSize0);
	m_editCurObjectSize1.SetWindowText(m_strEditCurObjectSize1);
	m_editCurObjectSize2.SetWindowText(m_strEditCurObjectSize2);
	m_editCurObjectSize3.SetWindowText(m_strEditCurObjectSize3);
	m_editCurObjectSize4.SetWindowText(m_strEditCurObjectSize4);
	m_editCurObjectSize5.SetWindowText(m_strEditCurObjectSize5);


	std::vector<COLORREF> vCOLORREFs;
	vCOLORREFs.push_back(RGB(255,255,0));	// UnCheck;
	vCOLORREFs.push_back(RGB(0,255,0));		// CheckOK;
	vCOLORREFs.push_back(RGB(255,0,0));		// CheckNG;

	m_labelCurObjectSizeCheckStatus0.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[0]]);
	m_labelCurObjectSizeCheckStatus1.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[1]]);
	m_labelCurObjectSizeCheckStatus2.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[2]]);
	m_labelCurObjectSizeCheckStatus3.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[3]]);
	m_labelCurObjectSizeCheckStatus4.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[4]]);
	m_labelCurObjectSizeCheckStatus5.SetBkColor(vCOLORREFs[m_AlignerObjectSizeInfo.m_vnCheckStatus[5]]);

}

// 更新对位目标标准距离信息
void CDlgVisionRunProductInfo::UpdateAlignerTargetStdSizeInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditTargetStdSize0 = _T("0.0000");
	m_strEditTargetStdSize1 = _T("0.0000");
	m_strEditTargetStdSize2 = _T("0.0000");
	m_strEditTargetStdSize3 = _T("0.0000");
	m_strEditTargetStdSize4 = _T("0.0000");
	m_strEditTargetStdSize5 = _T("0.0000");

    //小数位数格式化
    CString strXY;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
    {
        m_strEditTargetStdSize0.Format(strXY, m_AlignCheckInfo.m_dTargetDis0*PRECISIONXYUNIT);
        m_strEditTargetStdSize1.Format(strXY, m_AlignCheckInfo.m_dTargetDis1*PRECISIONXYUNIT);
        m_strEditTargetStdSize2.Format(strXY, m_AlignCheckInfo.m_dTargetDis2*PRECISIONXYUNIT);
        m_strEditTargetStdSize3.Format(strXY, m_AlignCheckInfo.m_dTargetDis3*PRECISIONXYUNIT);
        m_strEditTargetStdSize4.Format(strXY, m_AlignCheckInfo.m_dTargetDis4*PRECISIONXYUNIT);
        m_strEditTargetStdSize5.Format(strXY, m_AlignCheckInfo.m_dTargetDis5*PRECISIONXYUNIT);
    }
    else
    {
        m_strEditTargetStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis0);
        m_strEditTargetStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis1);
        m_strEditTargetStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis2);
        m_strEditTargetStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis3);
        m_strEditTargetStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis4);
        m_strEditTargetStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis5);
    }

	UpdateData(FALSE);
}

// 更新对位对象标准距离信息
void CDlgVisionRunProductInfo::UpdateAlignerObjectStdSizeInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditObjectStdSize0 = _T("0.0000");
	m_strEditObjectStdSize1 = _T("0.0000");
	m_strEditObjectStdSize2 = _T("0.0000");
	m_strEditObjectStdSize3 = _T("0.0000");
	m_strEditObjectStdSize4 = _T("0.0000");
	m_strEditObjectStdSize5 = _T("0.0000");

    //小数位数格式化
    CString strXY;
    strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
    if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
    {
        m_strEditObjectStdSize0.Format(strXY, m_AlignCheckInfo.m_dObjectDis0*PRECISIONXYUNIT);
        m_strEditObjectStdSize1.Format(strXY, m_AlignCheckInfo.m_dObjectDis1*PRECISIONXYUNIT);
        m_strEditObjectStdSize2.Format(strXY, m_AlignCheckInfo.m_dObjectDis2*PRECISIONXYUNIT);
        m_strEditObjectStdSize3.Format(strXY, m_AlignCheckInfo.m_dObjectDis3*PRECISIONXYUNIT);
        m_strEditObjectStdSize4.Format(strXY, m_AlignCheckInfo.m_dObjectDis4*PRECISIONXYUNIT);
        m_strEditObjectStdSize5.Format(strXY, m_AlignCheckInfo.m_dObjectDis5*PRECISIONXYUNIT);
    }
    else
    {
	    m_strEditObjectStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis0);
	    m_strEditObjectStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis1);
	    m_strEditObjectStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis2);
	    m_strEditObjectStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis3);
	    m_strEditObjectStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis4);
	    m_strEditObjectStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis5);
    }
	
	UpdateData(FALSE);

}

void CDlgVisionRunProductInfo::UpdateAlignerTargetStdAngleInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditTargetStdAngle0 = _T("0.0000");
	m_strEditTargetStdAngle1 = _T("0.0000");
	m_strEditTargetStdAngle2 = _T("0.0000");
	m_strEditTargetStdAngle3 = _T("0.0000");
// 	m_strEditTargetStdAngle4 = _T("0.0000");
// 	m_strEditTargetStdAngle5 = _T("0.0000");

	//小数位数格式化
	CString strXY;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
	{
		m_strEditTargetStdAngle0.Format(strXY, m_AlignCheckInfo.m_dTargetAngle0*PRECISIONXYUNIT);
		m_strEditTargetStdAngle1.Format(strXY, m_AlignCheckInfo.m_dTargetAngle1*PRECISIONXYUNIT);
		m_strEditTargetStdAngle2.Format(strXY, m_AlignCheckInfo.m_dTargetAngle2*PRECISIONXYUNIT);
		m_strEditTargetStdAngle3.Format(strXY, m_AlignCheckInfo.m_dTargetAngle3*PRECISIONXYUNIT);
// 		m_strEditTargetStdAngle4.Format(strXY, m_AlignCheckInfo.m_dTargetAngle4*PRECISIONXYUNIT);
// 		m_strEditTargetStdAngle5.Format(strXY, m_AlignCheckInfo.m_dTargetAngle5*PRECISIONXYUNIT);
	}
	else
	{
		m_strEditTargetStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle0);
		m_strEditTargetStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle1);
		m_strEditTargetStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle2);
		m_strEditTargetStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle3);
// 		m_strEditTargetStdAngle4.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle4);
// 		m_strEditTargetStdAngle5.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle5);
	}

	UpdateData(FALSE);
}

void CDlgVisionRunProductInfo::UpdateAlignerObjectStdAngleInfo()
{
	if (!m_pVisionASM)
	{
		return;
	}

	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditObjectStdAngle0 = _T("0.0000");
	m_strEditObjectStdAngle1 = _T("0.0000");
	m_strEditObjectStdAngle2 = _T("0.0000");
	m_strEditObjectStdAngle3 = _T("0.0000");
// 	m_strEditObjectStdAngle4 = _T("0.0000");
// 	m_strEditObjectStdAngle5 = _T("0.0000");

	//小数位数格式化
	CString strXY;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
	{
		m_strEditObjectStdAngle0.Format(strXY, m_AlignCheckInfo.m_dObjectAngle0*PRECISIONXYUNIT);
		m_strEditObjectStdAngle1.Format(strXY, m_AlignCheckInfo.m_dObjectAngle1*PRECISIONXYUNIT);
		m_strEditObjectStdAngle2.Format(strXY, m_AlignCheckInfo.m_dObjectAngle2*PRECISIONXYUNIT);
		m_strEditObjectStdAngle3.Format(strXY, m_AlignCheckInfo.m_dObjectAngle3*PRECISIONXYUNIT);
// 		m_strEditObjectStdAngle4.Format(strXY, m_AlignCheckInfo.m_dObjectAngle4*PRECISIONXYUNIT);
// 		m_strEditObjectStdAngle5.Format(strXY, m_AlignCheckInfo.m_dObjectAngle5*PRECISIONXYUNIT);
	}
	else
	{
		m_strEditObjectStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle0);
		m_strEditObjectStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle1);
		m_strEditObjectStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle2);
		m_strEditObjectStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle3);
// 		m_strEditObjectStdAngle4.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle4);
// 		m_strEditObjectStdAngle5.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle5);
	}

	UpdateData(FALSE);
}

void CDlgVisionRunProductInfo::UpdateAlignerTargetAngleInfo()
{
	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	//切换扩展产品时，更新到扩展产品的尺寸标准值
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditCurTargetAngle0 = _T("0.0000");
	m_strEditCurTargetAngle1 = _T("0.0000");
	m_strEditCurTargetAngle2 = _T("0.0000");
	m_strEditCurTargetAngle3 = _T("0.0000");
// 	m_strEditCurTargetAngle4 = _T("0.0000");
// 	m_strEditCurTargetAngle5 = _T("0.0000");

	//小数位数格式化
	CString strXY;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);

	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
	{
		m_strEditCurTargetAngle0.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[0]*PRECISIONXYUNIT);
		m_strEditCurTargetAngle1.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[1]*PRECISIONXYUNIT);
		m_strEditCurTargetAngle2.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[2]*PRECISIONXYUNIT);
		m_strEditCurTargetAngle3.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[3]*PRECISIONXYUNIT);
// 		m_strEditCurTargetAngle4.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[4]*PRECISIONXYUNIT);
// 		m_strEditCurTargetAngle5.Format(strXY, m_AlignerTargetAngleInfo.m_vdTargetAngle[5]*PRECISIONXYUNIT);
	}
	else
	{
		m_strEditCurTargetAngle0.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[0]);
		m_strEditCurTargetAngle1.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[1]);
		m_strEditCurTargetAngle2.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[2]);
		m_strEditCurTargetAngle3.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[3]);
// 		m_strEditCurTargetAngle4.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[4]);
// 		m_strEditCurTargetAngle5.Format(_T("%.4f"), m_AlignerTargetAngleInfo.m_vdTargetAngle[5]);
	}

	m_editCurTargetAngle0.SetWindowText(m_strEditCurTargetAngle0);
	m_editCurTargetAngle1.SetWindowText(m_strEditCurTargetAngle1);
	m_editCurTargetAngle2.SetWindowText(m_strEditCurTargetAngle2);
	m_editCurTargetAngle3.SetWindowText(m_strEditCurTargetAngle3);
// 	m_editCurTargetAngle4.SetWindowText(m_strEditCurTargetAngle4);
// 	m_editCurTargetAngle5.SetWindowText(m_strEditCurTargetAngle5);

	std::vector<COLORREF> vCOLORREFs;
	vCOLORREFs.push_back(RGB(255,255,0));	// UnCheck;
	vCOLORREFs.push_back(RGB(0,255,0));		// CheckOK;
	vCOLORREFs.push_back(RGB(255,0,0));		// CheckNG;

	m_labelCurTargetAngleCheckStatus0.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[0]]);
	m_labelCurTargetAngleCheckStatus1.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[1]]);
	m_labelCurTargetAngleCheckStatus2.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[2]]);
	m_labelCurTargetAngleCheckStatus3.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[3]]);
// 	m_labelCurTargetAngleCheckStatus4.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[4]]);
// 	m_labelCurTargetAngleCheckStatus5.SetBkColor(vCOLORREFs[m_AlignerTargetAngleInfo.m_vnCheckStatus[5]]);

	//m_nMultiCalibExtensionIndex = m_AlignerTargetSizeInfo.m_nExProductIndex;
	//m_comboMultiCalibExtension.SetCurSel(m_nMultiCalibExtensionIndex);
}

void CDlgVisionRunProductInfo::UpdateAlignerObjectAngleInfo()
{
	SysPlatformInfo sysInfo;
	m_pVisionASM->GetSysPlatformInfo(sysInfo);
	if (sysInfo.m_bEnableMultiCalibExtension)
	{
		vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
		int nShowNums = 0;
		if( tmpAlignTool != NULL)
		{
			nShowNums = tmpAlignTool->GetAlignExProductIndex();
		}
		m_comboMultiCalibExtension.SetCurSel(nShowNums);
		UpdateStdSizeExProduct();
	}

	m_strEditCurObjectAngle0 = _T("0.0000");
	m_strEditCurObjectAngle1 = _T("0.0000");
	m_strEditCurObjectAngle2 = _T("0.0000");
	m_strEditCurObjectAngle3 = _T("0.0000");
// 	m_strEditCurObjectAngle4 = _T("0.0000");
// 	m_strEditCurObjectAngle5 = _T("0.0000");

	//小数位数格式化
	CString strXY;
	strXY.Format(_T("%%.%df"),PRECISIONXYDOTNUM);
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
	{
		m_strEditCurObjectAngle0.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[0]*PRECISIONXYUNIT);
		m_strEditCurObjectAngle1.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[1]*PRECISIONXYUNIT);
		m_strEditCurObjectAngle2.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[2]*PRECISIONXYUNIT);
		m_strEditCurObjectAngle3.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[3]*PRECISIONXYUNIT);
// 		m_strEditCurObjectAngle4.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[4]*PRECISIONXYUNIT);
// 		m_strEditCurObjectAngle5.Format(strXY, m_AlignerObjectAngleInfo.m_vdObjectAngle[5]*PRECISIONXYUNIT);
	}
	else
	{
		m_strEditCurObjectAngle0.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[0]);
		m_strEditCurObjectAngle1.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[1]);
		m_strEditCurObjectAngle2.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[2]);
		m_strEditCurObjectAngle3.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[3]);
// 		m_strEditCurObjectAngle4.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[4]);
// 		m_strEditCurObjectAngle5.Format(_T("%.4f"), m_AlignerObjectAngleInfo.m_vdObjectAngle[5]);
	}

	m_editCurObjectAngle0.SetWindowText(m_strEditCurObjectAngle0);
	m_editCurObjectAngle1.SetWindowText(m_strEditCurObjectAngle1);
	m_editCurObjectAngle2.SetWindowText(m_strEditCurObjectAngle2);
	m_editCurObjectAngle3.SetWindowText(m_strEditCurObjectAngle3);
// 	m_editCurObjectAngle4.SetWindowText(m_strEditCurObjectAngle4);
// 	m_editCurObjectAngle5.SetWindowText(m_strEditCurObjectAngle5);


	std::vector<COLORREF> vCOLORREFs;
	vCOLORREFs.push_back(RGB(255,255,0));	// UnCheck;
	vCOLORREFs.push_back(RGB(0,255,0));		// CheckOK;
	vCOLORREFs.push_back(RGB(255,0,0));		// CheckNG;

	m_labelCurObjectAngleCheckStatus0.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[0]]);
	m_labelCurObjectAngleCheckStatus1.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[1]]);
	m_labelCurObjectAngleCheckStatus2.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[2]]);
	m_labelCurObjectAngleCheckStatus3.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[3]]);
// 	m_labelCurObjectAngleCheckStatus4.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[4]]);
// 	m_labelCurObjectAngleCheckStatus5.SetBkColor(vCOLORREFs[m_AlignerObjectAngleInfo.m_vnCheckStatus[5]]);
}


void CDlgVisionRunProductInfo::UpdateDlgItem()
{
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		// 不是位数而是测量尺寸数
		int nPosNum = 0;

		switch (platformInfo.m_nPositionNum)
		{
		case 2:
			nPosNum= 1;
			break;
		case 3:  //  jjh0730
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				//nPosNum = 2;
			}
			else
			{
				nPosNum = 3;						
			}
			break;
		case 4:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				nPosNum = 1;
			}
			else
			{
				nPosNum = 4;			
			}
			break;
		case 6:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				nPosNum = 3;
			}
			else
			{
				//nPosNum = 2;						
			}
			break;
		case 8:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				nPosNum = 4;
			}
			else
			{
				//nPosNum = 2;						
			}
			break;
		default:
			nPosNum= 1;
			break;
		}

		

		for (int i=0;i<nPosNum;i++)
		{
			GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE_0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VAL_CUR_OBJECT_SIZE_0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE_CHECK_STATUS_0+i)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE_0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_0+i)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VAL_CUR_TARGET_SIZE_0+i)->ShowWindow(SW_SHOW);		
			GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE_CHECK_STATUS_0+i)->ShowWindow(SW_SHOW);


			GetDlgItem(IDC_STATIC_CUR_OBJECT_SIZE_0+i)->SetWindowText(m_AlignerObjectSizeInfo.m_vstrObjectSizeName.at(i));
			GetDlgItem(IDC_STATIC_CUR_TARGET_SIZE_0+i)->SetWindowText(m_AlignerTargetSizeInfo.m_vstrTargetSizeName.at(i));

			if ( (4 == nPosNum) && ( ((8 == platformInfo.m_nCamNum) && (8 == platformInfo.m_nPositionNum)) || ((4 == platformInfo.m_nCamNum) && (4 == platformInfo.m_nPositionNum)) ) )
			{
				GetDlgItem(IDC_STATIC_CUR_OBJECT_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_VAL_CUR_OBJECT_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CUR_OBJECT_ANGLE_CHECK_STATUS_0+i)->ShowWindow(SW_SHOW);

				GetDlgItem(IDC_STATIC_CUR_TARGET_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_VAL_CUR_TARGET_ANGLE_0+i)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_STATIC_CUR_TARGET_ANGLE_CHECK_STATUS_0+i)->ShowWindow(SW_SHOW);

				GetDlgItem(IDC_STATIC_CUR_OBJECT_ANGLE_0+i)->SetWindowText(m_AlignerObjectAngleInfo.m_vstrObjectAngleName.at(i));
				GetDlgItem(IDC_STATIC_CUR_TARGET_ANGLE_0+i)->SetWindowText(m_AlignerTargetAngleInfo.m_vstrTargetAngleName.at(i));
			}
			
		}

		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			 GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
			 GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_SHOW);
		}
		else
		{
			 GetDlgItem(IDC_STATIC_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
			 GetDlgItem(IDC_COMBO_EXPRODUCT_INDEX)->ShowWindow(SW_HIDE);
		}

		m_comboMultiCalibExtension.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_comboMultiCalibExtension.InsertString(it,(LPCTSTR)strNum);
		}
		m_comboMultiCalibExtension.SetCurSel(0);
	}

}

void CDlgVisionRunProductInfo::UpdateDlgParamItem()
{
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bMainframeSetAlignParam == TRUE)
	{
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_3)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_5)->EnableWindow(TRUE);	
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3)->EnableWindow(TRUE);


	}
	else
	{
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_3)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3)->EnableWindow(FALSE);
	}
}

void CDlgVisionRunProductInfo::UpdateDlgItemText()
{
	if (m_pVisionASM && m_pParent)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		switch (platformInfo.m_nPositionNum)
		{
		case 2:
			SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_OBJECTSIZE0),0);
			SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_TARGETSIZE0),0);
			break;
		case 3:
			{
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE0),0);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE1),1);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE2),2);

				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE0),0);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE1),1);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE2),2);


			}
			break;
		case 4:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P2V2_OBJECTSIZE0),0);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P2V2_TARGETSIZE0),0);	
			}
			else
			{
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE0),0);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE0),0);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE1),1);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE1),1);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE2),2);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE2),2);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE3),3);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE3),3);

				SetAlignerObjectAngleStr(_T("角度12-24"),0);
				SetAlignerTargetAngleStr(_T("角度12-24"),0);
				SetAlignerObjectAngleStr(_T("角度24-43"),1);
				SetAlignerTargetAngleStr(_T("角度24-43"),1);
				SetAlignerObjectAngleStr(_T("角度43-31"),2);
				SetAlignerTargetAngleStr(_T("角度43-31"),2);
				SetAlignerObjectAngleStr(_T("角度31-12"),3);
				SetAlignerTargetAngleStr(_T("角度31-12"),3);
			}
			break;
		case 6:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE0),0);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE0),0);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE1),1);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE1),1);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE2),2);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE2),2);
			}
			else
			{

			}
			break;
		case 8:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE0),0);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE0),0);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE1),1);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE1),1);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE2),2);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE2),2);
				SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE3),3);
				SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE3),3);

				SetAlignerObjectAngleStr(_T("角度12-24"),0);
				SetAlignerTargetAngleStr(_T("角度56-68"),0);
				SetAlignerObjectAngleStr(_T("角度24-43"),1);
				SetAlignerTargetAngleStr(_T("角度68-87"),1);
				SetAlignerObjectAngleStr(_T("角度43-31"),2);
				SetAlignerTargetAngleStr(_T("角度87-75"),2);
				SetAlignerObjectAngleStr(_T("角度31-12"),3);
				SetAlignerTargetAngleStr(_T("角度75-56"),3);

			}
			else
			{

			}
			break;
		default:
			SetAlignerObjectSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_OBJECTSIZE0),0);
			SetAlignerTargetSizeStr(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_TARGETSIZE0),0);
			break;
		}

		UpdateDlgItem();
	}

}


BOOL CDlgVisionRunProductInfo::SetAlignerObjectSizeStr(CString strSizeName,int nSizeIndex)
{
	if (nSizeIndex<0 || nSizeIndex>=m_AlignerObjectSizeInfo.m_vstrObjectSizeName.size())
	{
		return FALSE;
	}

	m_AlignerObjectSizeInfo.m_vstrObjectSizeName.at(nSizeIndex) = strSizeName;

	return TRUE;
}

BOOL CDlgVisionRunProductInfo::SetAlignerTargetSizeStr(CString strSizeName,int nSizeIndex)
{
	if (nSizeIndex<0 || nSizeIndex>=m_AlignerTargetSizeInfo.m_vstrTargetSizeName.size())
	{
		return FALSE;
	}

	m_AlignerTargetSizeInfo.m_vstrTargetSizeName.at(nSizeIndex) = strSizeName;


	return TRUE;
}


BOOL CDlgVisionRunProductInfo::SetAlignerObjectAngleStr(CString strSizeName,int nSizeIndex)
{
	if (nSizeIndex < 0 || nSizeIndex >= m_AlignerObjectAngleInfo.m_vstrObjectAngleName.size())
	{
		return FALSE;
	}

	m_AlignerObjectAngleInfo.m_vstrObjectAngleName.at(nSizeIndex) = strSizeName;

	return TRUE;
}

BOOL CDlgVisionRunProductInfo::SetAlignerTargetAngleStr(CString strSizeName,int nSizeIndex)
{
	if (nSizeIndex < 0 || nSizeIndex >= m_AlignerTargetAngleInfo.m_vstrTargetAngleName.size())
	{
		return FALSE;
	}

	m_AlignerTargetAngleInfo.m_vstrTargetAngleName.at(nSizeIndex) = strSizeName;

	return TRUE;
}


BOOL CDlgVisionRunProductInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//SetItemFont();
    UpdateDialogLanguage();
	UpdateCurProductInfo();
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgVisionRunProductInfo::SetItemFont()
{
// 	m_MyFont.CreateFont(
// 		12,                        // nHeight
// 		0,                         // nWidth
// 		0,                         // nEscapement
// 		0,                         // nOrientation
// 		FW_SEMIBOLD,                  // nWeight
// 		FALSE,                     // bItalic
// 		FALSE,                     // bUnderline
// 		0,                         // cStrikeOut
// 		ANSI_CHARSET,              // nCharSet
// 		OUT_DEFAULT_PRECIS,        // nOutPrecision
// 		CLIP_DEFAULT_PRECIS,       // nClipPrecision
// 		DEFAULT_QUALITY,           // nQuality
// 		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
// 		_T("宋体"));					// lpszFacename
// 
// 	for (int i=0;i<6;i++)
// 	{
// 		GetDlgItem(IDC_EDIT_VAL_CUR_OBJECT_SIZE_0+i)->SetFont(&m_MyFont);
// 		GetDlgItem(IDC_EDIT_VAL_CUR_TARGET_SIZE_0+i)->SetFont(&m_MyFont);
// 
// 		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_0+i)->SetFont(&m_MyFont);
// 		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_0+i)->SetFont(&m_MyFont);
// 
// 	}
}

BOOL CDlgVisionRunProductInfo::IsStringPositiveNumerical(LPCTSTR lpszVal)
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
		if (((ch > '9') || ((ch < '0'))) && (ch != '.'))
		{
			if ( (i == 0) && ((ch == '+')) || ((ch == '-')) )
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgVisionRunProductInfo::IsStringNumerical( LPCTSTR lpszVal )
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

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸12]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize0();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_0, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_0, m_strEditTargetStdSize0);
		}
		else
		{
			m_strEditTargetStdSize0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_0, m_strEditTargetStdSize0);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis0 = atof(m_strEditTargetStdSize0)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis0 = atof(m_strEditTargetStdSize0);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸34]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize1();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_1, m_strEditTargetStdSize1);
		}
		else
		{
			m_strEditTargetStdSize1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_1, m_strEditTargetStdSize1);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis1 = atof(m_strEditTargetStdSize1)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis1 = atof(m_strEditTargetStdSize1);
				}
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸13]由%s到%s\n"),m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1, strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize2();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_2, m_strEditTargetStdSize2);
		}
		else
		{
			m_strEditTargetStdSize2.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_2, m_strEditTargetStdSize2);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis2 = atof(m_strEditTargetStdSize2)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis2 = atof(m_strEditTargetStdSize2);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸24]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize3();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_3, m_strEditTargetStdSize3);
		}
		else
		{
			m_strEditTargetStdSize3.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_3, m_strEditTargetStdSize3);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis3 = atof(m_strEditTargetStdSize3)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis3 = atof(m_strEditTargetStdSize3);
				}
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸4]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize4();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_4, m_strEditTargetStdSize4);
		}
		else
		{
			m_strEditTargetStdSize4.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_4, m_strEditTargetStdSize4);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis4 = atof(m_strEditTargetStdSize4)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis4 = atof(m_strEditTargetStdSize4);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdSize5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_SIZE_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标尺寸5]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdSize5();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdSize5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_5, m_strEditTargetStdSize5);
		}
		else
		{
			m_strEditTargetStdSize5.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_SIZE_5, m_strEditTargetStdSize5);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dTargetDis5 = atof(m_strEditTargetStdSize5)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dTargetDis5 = atof(m_strEditTargetStdSize5);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}


void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle0()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标角度1]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdAngle0();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle0()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, m_strEditTargetStdAngle0);
		}
		else
		{
			m_strEditTargetStdAngle0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_0, m_strEditTargetStdAngle0);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dTargetAngle0 = atof(m_strEditTargetStdAngle0);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle1()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标角度2]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdAngle1();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle1()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, m_strEditTargetStdAngle1);
		}
		else
		{
			m_strEditTargetStdAngle1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_1, m_strEditTargetStdAngle1);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dTargetAngle1 = atof(m_strEditTargetStdAngle1);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle2()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标角度3]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdAngle2();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle2()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, m_strEditTargetStdAngle2);
		}
		else
		{
			m_strEditTargetStdAngle2.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_2, m_strEditTargetStdAngle2);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dTargetAngle2 = atof(m_strEditTargetStdAngle2);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValTargetStdAngle3()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_TARGET_STD_ANGLE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][目标角度4]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValTargetStdAngle3();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValTargetStdAngle3()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, m_strEditTargetStdAngle3);
		}
		else
		{
			m_strEditTargetStdAngle3.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_TARGET_STD_ANGLE_3, m_strEditTargetStdAngle3);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dTargetAngle3 = atof(m_strEditTargetStdAngle3);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}






void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸12]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize0();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize0()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, m_strEditObjectStdSize0);
		}
		else
		{
			m_strEditObjectStdSize0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_0, m_strEditObjectStdSize0);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis0 = atof(m_strEditObjectStdSize0)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis0 = atof(m_strEditObjectStdSize0);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸34]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize1();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, m_strEditObjectStdSize1);
		}
		else
		{
			m_strEditObjectStdSize1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_1, m_strEditObjectStdSize1);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis1 = atof(m_strEditObjectStdSize1)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis1 = atof(m_strEditObjectStdSize1);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸13]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize2();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, m_strEditObjectStdSize2);
		}
		else
		{
			m_strEditObjectStdSize2.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_2, m_strEditObjectStdSize2);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis2 = atof(m_strEditObjectStdSize2)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis2 = atof(m_strEditObjectStdSize2);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸24]由%s到%s\n"),m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize3();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, m_strEditObjectStdSize3);
		}
		else
		{
			m_strEditObjectStdSize3.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_3, m_strEditObjectStdSize3);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis3 = atof(m_strEditObjectStdSize3)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis3 = atof(m_strEditObjectStdSize3);
				}

				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_4)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸4]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize4();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, m_strEditObjectStdSize4);
		}
		else
		{
			m_strEditObjectStdSize4.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_4, m_strEditObjectStdSize4);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis4 = atof(m_strEditObjectStdSize4)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis4 = atof(m_strEditObjectStdSize4);
				}
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdSize5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_SIZE_5)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象尺寸5]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdSize5();
	}	
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdSize5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, m_strEditObjectStdSize5);
		}
		else
		{
			m_strEditObjectStdSize5.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_SIZE_5, m_strEditObjectStdSize5);

			if (m_pVisionASM != NULL)
			{
				if(m_pParent->m_pParent->m_SysOptionConfig.m_bChangeProductSize2um)
				{
					m_AlignCheckInfo.m_dObjectDis5 = atof(m_strEditObjectStdSize5)/PRECISIONXYUNIT;
				}
				else
				{
					m_AlignCheckInfo.m_dObjectDis5 = atof(m_strEditObjectStdSize5);
				}
\
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}


void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle0()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象角度1]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdAngle0();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle0()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, m_strEditObjectStdAngle0);
		}
		else
		{
			m_strEditObjectStdAngle0.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_0, m_strEditObjectStdAngle0);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dObjectAngle0 = atof(m_strEditObjectStdAngle0);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}


void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle1()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象角度2]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdAngle1();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle1()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, m_strEditObjectStdAngle1);
		}
		else
		{
			m_strEditObjectStdAngle1.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_1, m_strEditObjectStdAngle1);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dObjectAngle1 = atof(m_strEditObjectStdAngle1);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle2()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象角度3]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdAngle2();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle2()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, m_strEditObjectStdAngle2);
		}
		else
		{
			m_strEditObjectStdAngle2.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_2, m_strEditObjectStdAngle2);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dObjectAngle2 = atof(m_strEditObjectStdAngle2);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}

void CDlgVisionRunProductInfo::OnEnSetfocusEditValObjectStdAngle3()
{
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[工位%d][产品%d][对象角度4]由%s到%s\n"), m_pParent->GetPlatformID()+1,m_pParent->m_comboProduct.GetCurSel()+1,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
		m_bKillFocusEnabled = TRUE;
		OnEnKillfocusEditValObjectStdAngle3();
	}
}

void CDlgVisionRunProductInfo::OnEnKillfocusEditValObjectStdAngle3()
{
	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, strVal);
		if (!IsStringPositiveNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, m_strEditObjectStdAngle3);
		}
		else
		{
			m_strEditObjectStdAngle3.Format(_T("%.4f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_VAL_OBJECT_STD_ANGLE_3, m_strEditObjectStdAngle3);

			if (m_pVisionASM != NULL)
			{
				m_AlignCheckInfo.m_dObjectAngle3 = atof(m_strEditObjectStdAngle3);
				m_pVisionASM->SetAlignCheckInfo(m_AlignCheckInfo,m_nMultiCalibExtensionIndex);
			}
		}
	}
}


void CDlgVisionRunProductInfo::OnCbnSelchangeCombo1()
{
	UpdateData(TRUE);
	int nSel = m_comboMultiCalibExtension.GetCurSel();
	if (nSel < 0)
	{
		return;
	}
	COLORREF crUnCheck = RGB(255,255,0);
	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		int nCurProIdx = pTotalProdData->GetCurProductIndex();
		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
		{

			CProductData curProduct = pTotalProdData->GetItem(nCurProIdx);
			m_AlignCheckInfo = curProduct.m_pAlignerParam->GetAlignCheckInfo(nSel);

			m_strEditTargetStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis0);
			m_strEditTargetStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis1);
			m_strEditTargetStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis2);
			m_strEditTargetStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis3);
			m_strEditTargetStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis4);
			m_strEditTargetStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis5);
			m_strEditTargetStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle0);
			m_strEditTargetStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle1);
			m_strEditTargetStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle2);
			m_strEditTargetStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle3);


			m_strEditObjectStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis0);
			m_strEditObjectStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis1);
			m_strEditObjectStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis2);
			m_strEditObjectStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis3);
			m_strEditObjectStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis4);
			m_strEditObjectStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis5);
			m_strEditObjectStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle0);
			m_strEditObjectStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle1);
			m_strEditObjectStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle2);
			m_strEditObjectStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle3);

		}

		m_editCurTargetSize0.SetWindowText(m_strEditCurTargetSize0);
		m_editCurTargetSize1.SetWindowText(m_strEditCurTargetSize1);
		m_editCurTargetSize2.SetWindowText(m_strEditCurTargetSize2);
		m_editCurTargetSize3.SetWindowText(m_strEditCurTargetSize3);
		m_editCurTargetSize4.SetWindowText(m_strEditCurTargetSize4);
		m_editCurTargetSize5.SetWindowText(m_strEditCurTargetSize5);
		m_editCurTargetAngle0.SetWindowText(m_strEditCurTargetAngle0);
		m_editCurTargetAngle1.SetWindowText(m_strEditCurTargetAngle1);
		m_editCurTargetAngle2.SetWindowText(m_strEditCurTargetAngle2);
		m_editCurTargetAngle3.SetWindowText(m_strEditCurTargetAngle3);

		m_editCurObjectSize0.SetWindowText(m_strEditCurObjectSize0);
		m_editCurObjectSize1.SetWindowText(m_strEditCurObjectSize1);
		m_editCurObjectSize2.SetWindowText(m_strEditCurObjectSize2);
		m_editCurObjectSize3.SetWindowText(m_strEditCurObjectSize3);
		m_editCurObjectSize4.SetWindowText(m_strEditCurObjectSize4);
		m_editCurObjectSize5.SetWindowText(m_strEditCurObjectSize5);
		m_editCurObjectAngle0.SetWindowText(m_strEditCurObjectAngle0);
		m_editCurObjectAngle1.SetWindowText(m_strEditCurObjectAngle1);
		m_editCurObjectAngle2.SetWindowText(m_strEditCurObjectAngle2);
		m_editCurObjectAngle3.SetWindowText(m_strEditCurObjectAngle3);



		m_labelCurTargetSizeCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus3.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus4.SetBkColor(crUnCheck);
		m_labelCurTargetSizeCheckStatus5.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurTargetAngleCheckStatus3.SetBkColor(crUnCheck);

		m_labelCurObjectSizeCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus3.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus4.SetBkColor(crUnCheck);
		m_labelCurObjectSizeCheckStatus5.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus0.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus1.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus2.SetBkColor(crUnCheck);
		m_labelCurObjectAngleCheckStatus3.SetBkColor(crUnCheck);


	}

	UpdateData(FALSE);
}
void CDlgVisionRunProductInfo::UpdateStdSizeExProduct()
{
	UpdateData(TRUE);
	int nSel = m_comboMultiCalibExtension.GetCurSel();
	if (nSel < 0)
	{
		return;
	}

	if (m_pVisionASM != NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		CTotalProductData *pTotalProdData = m_pVisionASM->GetTotalProuductData();
		int nCurProIdx = pTotalProdData->GetCurProductIndex();
		if ((nCurProIdx < pTotalProdData->GetCount()) && (nCurProIdx >= 0))
		{

			CProductData curProduct = pTotalProdData->GetItem(nCurProIdx);
			m_AlignCheckInfo = curProduct.m_pAlignerParam->GetAlignCheckInfo(nSel);

			m_strEditTargetStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis0);
			m_strEditTargetStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis1);
			m_strEditTargetStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis2);
			m_strEditTargetStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis3);
			m_strEditTargetStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis4);
			m_strEditTargetStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetDis5);
			m_strEditTargetStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle0);
			m_strEditTargetStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle1);
			m_strEditTargetStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle2);
			m_strEditTargetStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dTargetAngle3);

			m_strEditObjectStdSize0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis0);
			m_strEditObjectStdSize1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis1);
			m_strEditObjectStdSize2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis2);
			m_strEditObjectStdSize3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis3);
			m_strEditObjectStdSize4.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis4);
			m_strEditObjectStdSize5.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectDis5);
			m_strEditObjectStdAngle0.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle0);
			m_strEditObjectStdAngle1.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle1);
			m_strEditObjectStdAngle2.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle2);
			m_strEditObjectStdAngle3.Format(_T("%.4f"), m_AlignCheckInfo.m_dObjectAngle3);

		}

		m_editCurTargetSize0.SetWindowText(m_strEditCurTargetSize0);
		m_editCurTargetSize1.SetWindowText(m_strEditCurTargetSize1);
		m_editCurTargetSize2.SetWindowText(m_strEditCurTargetSize2);
		m_editCurTargetSize3.SetWindowText(m_strEditCurTargetSize3);
		m_editCurTargetSize4.SetWindowText(m_strEditCurTargetSize4);
		m_editCurTargetSize5.SetWindowText(m_strEditCurTargetSize5);
		m_editCurTargetAngle0.SetWindowText(m_strEditCurTargetAngle0);
		m_editCurTargetAngle1.SetWindowText(m_strEditCurTargetAngle1);
		m_editCurTargetAngle2.SetWindowText(m_strEditCurTargetAngle2);
		m_editCurTargetAngle3.SetWindowText(m_strEditCurTargetAngle3);

		m_editCurObjectSize0.SetWindowText(m_strEditCurObjectSize0);
		m_editCurObjectSize1.SetWindowText(m_strEditCurObjectSize1);
		m_editCurObjectSize2.SetWindowText(m_strEditCurObjectSize2);
		m_editCurObjectSize3.SetWindowText(m_strEditCurObjectSize3);
		m_editCurObjectSize4.SetWindowText(m_strEditCurObjectSize4);
		m_editCurObjectSize5.SetWindowText(m_strEditCurObjectSize5);
		m_editCurObjectAngle0.SetWindowText(m_strEditCurObjectAngle0);
		m_editCurObjectAngle1.SetWindowText(m_strEditCurObjectAngle1);
		m_editCurObjectAngle2.SetWindowText(m_strEditCurObjectAngle2);
		m_editCurObjectAngle3.SetWindowText(m_strEditCurObjectAngle3);

	}

	UpdateData(FALSE);
}

