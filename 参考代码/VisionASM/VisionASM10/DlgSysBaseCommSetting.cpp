// DlgSysBaseCommSetting.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysBaseCommSetting.h"
#include "DlgSysCommSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysBaseCommSetting dialog


CDlgSysBaseCommSetting::CDlgSysBaseCommSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysBaseCommSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysBaseCommSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pVisionASM = NULL;
	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;
//	m_pParent = (CDlgSysCommSetting *)pParent;
	m_pParent = (CDlgSysEnvironSetting *)pParent;
	m_bSysKeyboardEnabled = FALSE;
	m_psaSysInfoStrings = NULL;
	m_bIsCommOpen = FALSE;
	m_hIconOpen = NULL;
	m_hIconClosed = NULL;
	m_bVisible = FALSE;
}

CDlgSysBaseCommSetting::CDlgSysBaseCommSetting( UINT nIDTemplate, CWnd* pParent /*= NULL*/ )
	: CDialog(nIDTemplate, pParent)
{
	m_pVisionASM = NULL;
	m_pThreadReceiveData = NULL;
	m_bExitThread = FALSE;
	m_hEventReceiveThreadExit = NULL;
//	m_pParent = (CDlgSysCommSetting *)pParent;
	m_pParent = (CDlgSysEnvironSetting *)pParent;
	m_bSysKeyboardEnabled = FALSE;
	m_psaSysInfoStrings = NULL;
	m_bIsCommOpen = FALSE;
	m_hIconOpen = NULL;
	m_hIconClosed = NULL;
	m_bVisible = FALSE;
}

void CDlgSysBaseCommSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysBaseCommSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSysBaseCommSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgSysBaseCommSetting)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysBaseCommSetting message handlers

 void CDlgSysBaseCommSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
 {
	 CRect CRectGroupBox;
	 // 检查是否为Group控件
	 if(NULL== GetDlgItem(nID)) return;
	 if (NULL== (CStatic*) GetDlgItem(nID)) return;
	 GetDlgItem(nID)->GetWindowRect(&CRectGroupBox);
	 ScreenToClient(&CRectGroupBox);

	 int nShow = SW_SHOW;
	 if (bShow)
	 {
		 nShow = SW_SHOW;
	 }
	 else
	 {
		 nShow = SW_HIDE;
	 }

	 // Group 控件本身也进行显示或隐藏
	 GetDlgItem(nID)->EnableWindow(bEnable);
	 GetDlgItem(nID)->ShowWindow(nShow);

	 CWnd *pWnd;
	 pWnd = GetWindow(GW_CHILD);
	 while(pWnd!=NULL)
	 {
		 CRect CRect2;
		 pWnd->GetWindowRect(&CRect2);
		 ScreenToClient(&CRect2);
		 if (CRectGroupBox.PtInRect(CRect2.TopLeft()) && CRectGroupBox.PtInRect(CRect2.BottomRight()))
		 {
			 if(pWnd->GetDlgCtrlID()== IDC_EDIT_REG_READ_VALUE || pWnd->GetDlgCtrlID()==IDC_EDIT_READ_REG_VALUE)
			 {
				 pWnd->EnableWindow(FALSE);
			 }
			 else
			 {
			 	pWnd->EnableWindow(bEnable);
			 }
			 pWnd->ShowWindow(nShow);
		 }
		 pWnd = pWnd->GetNextWindow();
	 }
 }