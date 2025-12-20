// DlgSysPlatformParam.cpp : implementation file
//

#include "stdafx.h"
#include "VisionASM.h"
#include "DlgSysPlatformParam.h"
#include "DlgSysEnvironSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPlatformParam dialog


CDlgSysPlatformParam::CDlgSysPlatformParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSysPlatformParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysPlatformParam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPlatformInfo = NULL;
	m_pCamPlatformInfo = NULL;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;
	m_pVisionASM = NULL;
	m_bCamPlatformInfoEnable = TRUE;
}

CDlgSysPlatformParam::CDlgSysPlatformParam( UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/ )
	: CDialog(nIDTemplate, pParentWnd)
{
	m_pPlatformInfo = NULL;
	m_pCamPlatformInfo = NULL;
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;

	m_bCamPlatformInfoEnable = TRUE;
}

void CDlgSysPlatformParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysPlatformParam)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysPlatformParam, CDialog)
	//{{AFX_MSG_MAP(CDlgSysPlatformParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPlatformInfo* CDlgSysPlatformParam::GetPlatformInfo() const
{
	return m_pPlatformInfo;
}

void CDlgSysPlatformParam::SetVisionASMRef(vcBaseVisionAlign* pVisionASM)
{
	m_pVisionASM = pVisionASM;
}

CCamPlatformInfo* CDlgSysPlatformParam::GetCamPlatformInfo() const
{
	return m_pCamPlatformInfo;
}

BOOL CDlgSysPlatformParam::SetCamPlatformInfo( const CCamPlatformInfo* platformInfo )
{
	if (m_pCamPlatformInfo != NULL)
	{
		delete m_pCamPlatformInfo;
		m_pCamPlatformInfo = NULL;
	}
	if (platformInfo != NULL)
	{
		m_pCamPlatformInfo = new CCamPlatformInfo(*platformInfo);
	}
	return TRUE;
}

void CDlgSysPlatformParam::SetCamPlatformInfoEnable(BOOL bEnable)
{
	m_bCamPlatformInfoEnable = bEnable;
}

BOOL CDlgSysPlatformParam::GetCamPlatformInfoEnable()
{
	return m_bCamPlatformInfoEnable;
}

CDlgSysPlatformParam::~CDlgSysPlatformParam()
{
	if (m_pCamPlatformInfo != NULL)
	{
		delete m_pCamPlatformInfo;
		m_pCamPlatformInfo = NULL;
	}
	if (m_pPlatformInfo != NULL)
	{
		delete m_pPlatformInfo;
		m_pPlatformInfo = NULL;
	}
}

CWnd* CDlgSysPlatformParam::SetParentWnd( CDlgSysEnvironSetting *pParent )
{
	CWnd* pPrevParent = m_pParent;
	m_pParent = pParent;
	return pPrevParent;
}

BOOL CDlgSysPlatformParam::IsStringNumerical( LPCTSTR lpszVal )
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
