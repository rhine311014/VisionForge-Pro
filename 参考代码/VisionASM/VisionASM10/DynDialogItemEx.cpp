// DynDialogItemEx.cpp: implementation of the CDynDialogItemEx class.
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DynDialogItemEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static UINT glb_nNextID = WM_USER+4096;  // We have to start somewhere...

UINT GetNewUniqueID(void)
{
	glb_nNextID++;
	return glb_nNextID - 1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynDialogItemEx::CDynDialogItemEx()
	: CWnd()
{
	m_eTypeControl = NOCONTROL;
	m_strClassName = _T("");
	m_dwStyle = 0;
	m_dwStyleEx = 0;
	m_strCaption = _T("");
	m_ControlID = 0;
	m_pData = NULL;
	m_bSubclassed = FALSE;
}

void CDynDialogItemEx::DoDataExchange(CDataExchange *pDX)
{
	if (m_pData != NULL) {
		switch(m_eTypeControl) {
		case BUTTON:
			if ((m_dwStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) {
				DDX_Radio(pDX, m_ControlID, *(int*)m_pData);
			}
			else if ((m_dwStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX) {
				DDX_Check(pDX, m_ControlID, *(int*)m_pData);
			}
			else {
				DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			}
			break;
		case EDITCONTROL:
			DDX_Text(pDX, m_ControlID, *(CString*)m_pData);
			break;
		case STATICTEXT:
			DDX_Text(pDX, m_ControlID, *(CString*)m_pData);
			break;
		case LISTBOX:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case HSCROLL:
			DDX_Scroll(pDX, m_ControlID, *(int*)m_pData);
			break;
		case COMBOBOX:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case SPIN:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case PROGRES:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case SLIDER:
			DDX_Slider(pDX, m_ControlID, *(int*)m_pData);
			break;
		case HOTKEY:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case LISTCTRL:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case TREECTRL:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case TABCTRL:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case ANIMATE:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case RICHEDIT:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case DATETIMEPICKER:
			if ((m_dwStyle & DTS_TIMEFORMAT) == DTS_TIMEFORMAT) {
				DDX_DateTimeCtrl(pDX, m_ControlID, *(CTime*)m_pData);
			}
			else {
				DDX_DateTimeCtrl(pDX, m_ControlID, *(COleDateTime*)m_pData);
			}
			break;
		case MONTHCALENDER:
			DDX_MonthCalCtrl(pDX, m_ControlID, *(COleDateTime*)m_pData);
			break;
		case IPADRESS:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		case COMBOBOXEX:
			DDX_Control(pDX, m_ControlID, *(CWnd*)m_pData);
			break;
		default:
			break;
		}
	}
	CWnd::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDynDialogItemEx, CWnd)
	//{{AFX_MSG_MAP(CDynDialogItemEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

char* CDynDialogItemEx::GetClassNameByType(DLGITEMTEMPLATECONTROLS TypeControl)
{
	switch(TypeControl) {
	case BUTTON:
		return _T("BUTTON");
	case EDITCONTROL:
		return _T("EDIT");
	case STATICTEXT:
		return _T("STATIC");
	case LISTBOX:
		return _T("LISTBOX");
	case HSCROLL:
		return _T("SCROLLBAR");
	case COMBOBOX:
		return _T("COMBOBOX");
	case SPIN:
		return _T("msctls_updown32");
	case PROGRES:
		return _T("msctls_progress32");
	case SLIDER:
		return _T("msctls_trackbar32");
	case HOTKEY:
		return _T("msctls_hotkey32");
	case LISTCTRL:
		return _T("SysListView32");
	case TREECTRL:
		return _T("SysTreeView32");
	case TABCTRL:
		return _T("SysTabControl32");
	case ANIMATE:
		return _T("SysAnimate32");
	case RICHEDIT:
		return _T("RICHEDIT");
	case DATETIMEPICKER:
		return _T("SysDateTimePick32");
	case MONTHCALENDER:
		return _T("SysMonthCal32");
	case IPADRESS:
		return _T("SysIPAddress32");
	case COMBOBOXEX:
		return _T("ComboBoxEx32");
	}
	return _T("");
}

DLGITEMTEMPLATECONTROLS CDynDialogItemEx::GetClassTypeByName(LPCSTR lpszClassName)
{
	if (memcmp(lpszClassName, _T("BUTTON"), 6) == 0) {
		return BUTTON;
	}
	else if (memcmp(lpszClassName, _T("EDIT"), 4) == 0) {
		return EDITCONTROL;
	}
	else if (memcmp(lpszClassName, _T("STATIC"), 6) == 0) {
		return STATICTEXT;
	}
	else if (memcmp(lpszClassName, _T("LISTBOX"), 7) == 0) {
		return LISTBOX;
	}
	else if (memcmp(lpszClassName, _T("SCROLLBAR"), 9) == 0) {
		return HSCROLL;
	}
	else if (memcmp(lpszClassName, _T("COMBOBOX"), 8) == 0) {
		return COMBOBOX;
	}
	else if (memcmp(lpszClassName, _T("msctls_updown32"), 15) == 0) {
		return SPIN;
	}
	else if (memcmp(lpszClassName, _T("msctls_progress32"), 17) == 0) {
		return PROGRES;
	}
	else if (memcmp(lpszClassName, _T("msctls_trackbar32"), 17) == 0) {
		return SLIDER;
	}
	else if (memcmp(lpszClassName, _T("msctls_hotkey32"), 15) == 0) {
		return HOTKEY;
	}
	else if (memcmp(lpszClassName, _T("SysListView32"), 13) == 0) {
		return LISTCTRL;
	}
	else if (memcmp(lpszClassName, _T("SysTreeView32"), 13) == 0) {
		return TREECTRL;
	}
	else if (memcmp(lpszClassName, _T("SysTabControl32"), 15) == 0) {
		return TABCTRL;
	}
	else if (memcmp(lpszClassName, _T("SysAnimate32"), 12) == 0) {
		return ANIMATE;
	}
	else if (memcmp(lpszClassName, _T("RICHEDIT"), 8) == 0) {
		return RICHEDIT;
	}
	else if (memcmp(lpszClassName, _T("SysDateTimePick32"), 17) == 0) {
		return DATETIMEPICKER;
	}
	else if (memcmp(lpszClassName, _T("SysMonthCal32"), 13) == 0) {
		return MONTHCALENDER;
	}
	else if (memcmp(lpszClassName, _T("SysIPAddress32"), 14) == 0) {
		return IPADRESS;
	}
	else if (memcmp(lpszClassName, _T("ComboBoxEx32"), 12) == 0) {
		return COMBOBOXEX;
	}

	return NOCONTROL;
}

UINT CDynDialogItemEx::InitDialogItem(DLGITEMTEMPLATECONTROLS TypeControl,
											   DWORD dwStyle,
												DWORD dwExtendedStyle,
												LPRECT pRect,
												LPCTSTR lpszCaption,
												UINT nID /*= 0*/,
												BOOL bSubclassed /*= FALSE*/,
												void	*pData /*= NULL*/)
{
	m_eTypeControl = TypeControl;
	m_strClassName = GetClassNameByType(m_eTypeControl);
	m_dwStyle = dwStyle;
	m_dwStyleEx = dwExtendedStyle;
	m_Rect = pRect;
	m_strCaption = lpszCaption;
	m_bSubclassed = bSubclassed;
	m_pData = pData;
	if (nID == 0) {
		m_ControlID = ::GetNewUniqueID();
	}
	else {
		m_ControlID = nID;
	}
	return m_ControlID;
}

UINT CDynDialogItemEx::InitDialogItem(LPCSTR lpszClassName,
												DWORD dwStyle,
												DWORD dwExtendedStyle,
												LPRECT pRect,
												LPCTSTR lpszCaption,
												UINT nID /*= 0*/,
												BOOL bSubclassed /*= FALSE*/,
												void	*pData /*= NULL*/)
{
	m_strClassName = lpszClassName;
	m_eTypeControl = GetClassTypeByName(lpszClassName);
	m_dwStyle = dwStyle;
	m_dwStyleEx = dwExtendedStyle;
	m_Rect = pRect;
	m_strCaption = lpszCaption;
	m_bSubclassed = bSubclassed;
	m_pData = pData;
	if (nID == 0) {
		m_ControlID = ::GetNewUniqueID();
	}
	else {
		m_ControlID = nID;
	}
	return m_ControlID;
}

BOOL CDynDialogItemEx::CreateEx(CWnd *pParent)
{
	BOOL bRet = FALSE;
	if (m_eTypeControl == NOCONTROL) { //It will probably be an OCX...
		//
		// Create the control later....
		// if it's created here then the rectangle is not OK and SetWindowPos doesn't work on OCX's????
		//
		bRet = TRUE;
	}
	else if (m_pData != NULL && IsDataMemberPointerToWnd()) {
		bRet = ((CWnd*)m_pData)->CreateEx(m_dwStyleEx, m_strClassName, m_strCaption, m_dwStyle, m_Rect, pParent, m_ControlID);
	}
	else  {
		bRet = CWnd::CreateEx(m_dwStyleEx, m_strClassName, m_strCaption, m_dwStyle, m_Rect, pParent, m_ControlID);
	}

	return bRet;
}

BOOL CDynDialogItemEx::SetWindowPos(CWnd *pParent)
{
	BOOL bRet = FALSE;
	//Conversion of Dialog units to screenunits
	CRect rect(m_Rect);
	((CDialog *)pParent)->MapDialogRect(&rect);
	ASSERT(rect.IsRectEmpty() == FALSE);

	if (m_eTypeControl == NOCONTROL) 
	{
		return bRet;
	}
	else if (m_pData != NULL && IsDataMemberPointerToWnd()) {
		bRet = ((CWnd*)m_pData)->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
	}
	else {
		bRet = CWnd::SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
	}

	return bRet;
}

void CDynDialogItemEx::SetFont(CFont* pFont, BOOL bRedraw /*= TRUE*/)
{
	if (m_pData != NULL && IsDataMemberPointerToWnd()) {
		((CWnd*)m_pData)->SetFont(pFont, bRedraw);
	}
	else {
		CWnd::SetFont(pFont, bRedraw);
	}
}

PBYTE CDynDialogItemEx::FillBufferWithItemTemplate(BYTE *pdest)
{
	pdest = (BYTE*)(((DWORD)pdest + 3) & ~3);  // make the pointer DWORD aligned

	DLGITEMTEMPLATE dlgItemTemplate;
	dlgItemTemplate.x = (short)m_Rect.left;
	dlgItemTemplate.y = (short)m_Rect.top;
	dlgItemTemplate.cx = (short)m_Rect.Width();
	dlgItemTemplate.cy = (short)m_Rect.Height();
	dlgItemTemplate.style = m_dwStyle;
	dlgItemTemplate.dwExtendedStyle = m_dwStyleEx;
	dlgItemTemplate.id = (USHORT)m_ControlID;

	memcpy(pdest, (void *)&dlgItemTemplate, sizeof(DLGITEMTEMPLATE));
	pdest += sizeof(DLGITEMTEMPLATE);
	*(WORD*)pdest = 0xFFFF;  // indicating atom value
	pdest += sizeof(WORD);
	*(WORD*)pdest = (USHORT)m_eTypeControl;    // atom value for the control
	pdest += sizeof(WORD);

	// transfer the caption even when it is an empty string
	WCHAR*  pchCaption;
	int     nChars, nActualChars;

	nChars = m_strCaption.GetLength() + 1;
	pchCaption = new WCHAR[nChars];
	nActualChars = MultiByteToWideChar(CP_ACP, 0, m_strCaption, -1, pchCaption, nChars);
	ASSERT(nActualChars > 0);
	memcpy(pdest, pchCaption, nActualChars * sizeof(WCHAR));
	pdest += nActualChars * sizeof(WCHAR);
	delete pchCaption;

	*(WORD*)pdest = 0;  // How many bytes in data for control
	pdest += sizeof(WORD);

	return pdest;
}

BOOL CDynDialogItemEx::IsDataMemberPointerToWnd()
{
	BOOL bRet = TRUE;
	switch(m_eTypeControl) {
	case BUTTON:
		if ((m_dwStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON) {
			bRet = FALSE;
		}
		else if ((m_dwStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX) {
			bRet = FALSE;
		}
		break;
	case EDITCONTROL:
		bRet = FALSE;
		break;
	case STATICTEXT:
		bRet = FALSE;
		break;
	case HSCROLL:
		bRet = FALSE;
		break;
	case SLIDER:
		bRet = FALSE;
		break;
	case DATETIMEPICKER:
		bRet = FALSE;
		break;
	case MONTHCALENDER:
		bRet = FALSE;
		break;
	default:
		break;
	}
	return bRet;
}
