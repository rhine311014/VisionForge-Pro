// DlgCrossSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCrossSetting.h"
#include "afxdialogex.h"
#include "svXMLConfigurator.h"

#include "DlgSearchGrapchSetting.h"
#include "LogFileSave.h"
// CDlgCrossSetting 对话框

IMPLEMENT_DYNAMIC(CDlgCrossSetting, CDialog)

CDlgCrossSetting::CDlgCrossSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCrossSetting::IDD, pParent)
	, m_ShowLength(0)
	, m_ShowWidth(0)
    , m_ShowColor(0)
	, m_ShowStyle(FALSE)
	, m_bShowTrainRect(FALSE)
	, m_bShowOtherGui(FALSE)
	, m_nTrainRectShowColor(0)
	, m_nTrainRectShowWidth(0)
	, m_nOtherUIShowColor(0)
	, m_nOtherUIShowWidth(0)
{
	m_bShowDefortGui = TRUE;
    m_nOldLineColor = m_ShowColor;
    m_bOldShowStyle = m_ShowStyle;
    m_nOldShowLength = m_ShowLength;
    m_nOldShowWidth = m_ShowWidth;

	m_nOldTrainRectShowColor = m_nTrainRectShowColor;
	m_nOldTrainRectShowWidth = m_nTrainRectShowWidth;

	m_nOldOtherUIShowColor	= m_nOtherUIShowColor;
	m_nOldOtherUIShowWidth	= m_nOtherUIShowWidth;
}

CDlgCrossSetting::~CDlgCrossSetting()
{
}

void CDlgCrossSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// CrossSetting
	DDX_Radio(pDX, IDC_RAD_RED, m_ShowColor);
	DDX_Text(pDX, IDC_EDIT_LINE_LENGTH, m_ShowLength);
	DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_ShowWidth);
	DDX_Radio(pDX, IDC_RAD_CrossZero, m_ShowStyle);
	DDX_Check(pDX, IDC_CHECK_SHOW_DEFORT_GUI, m_bShowDefortGui);
	DDX_Check(pDX, IDC_CHECK_SHOW_SEARCH_RECT, m_bShowTrainRect);
	DDX_Check(pDX, IDC_CHECK_SHOW_OTHER_GUI, m_bShowOtherGui);
	// TrainRectSetting
	DDX_Radio(pDX, IDC_RAD_TRAIN_RECT_RED, m_nTrainRectShowColor);
	DDX_Text(pDX, IDC_EDIT_TRAIN_RECT_LINE_WIDTH, m_nTrainRectShowWidth);
	// OtherUISetting
	DDX_Radio(pDX, IDC_RAD_OTHER_UI_RED, m_nOtherUIShowColor);
	DDX_Text(pDX, IDC_EDIT_OTHER_UI_LINE_WIDTH, m_nOtherUIShowWidth);
}

BEGIN_MESSAGE_MAP(CDlgCrossSetting, CDialog)
	ON_BN_CLICKED(IDC_RAD_RED, &CDlgCrossSetting::OnBnClickedRadRed)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RAD_GREEN, &CDlgCrossSetting::OnBnClickedRadGreen)
	ON_BN_CLICKED(IDC_RAD_BLUE, &CDlgCrossSetting::OnBnClickedRadBlue)
	ON_BN_CLICKED(IDC_RAD_COLOR_SETTING, &CDlgCrossSetting::OnBnClickedRadColorSetting)
	ON_BN_CLICKED(IDOK, &CDlgCrossSetting::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_LINE_WIDTH, &CDlgCrossSetting::OnEnChangeEditLineWidth)
	ON_EN_CHANGE(IDC_EDIT_LINE_LENGTH, &CDlgCrossSetting::OnEnChangeEditLineLength)
	ON_EN_SETFOCUS(IDC_EDIT_LINE_WIDTH, &CDlgCrossSetting::OnEnSetfocusEditLineWidth)
	ON_EN_SETFOCUS(IDC_EDIT_LINE_LENGTH, &CDlgCrossSetting::OnEnSetfocusEditLineLength)
    ON_BN_CLICKED(IDC_RAD_CrossZero, &CDlgCrossSetting::OnBnClickedRadCrosszero)
    ON_BN_CLICKED(IDC_RAD_CROSS45, &CDlgCrossSetting::OnBnClickedRadCross45)
    ON_BN_CLICKED(IDC_CHECK_SHOW_DEFORT_GUI, &CDlgCrossSetting::OnBnClickedCheckShowDefortGui)

	ON_BN_CLICKED(IDC_RAD_TRAIN_RECT_RED, &CDlgCrossSetting::OnBnClickedRadTrainRectRed)
	ON_BN_CLICKED(IDC_RAD_TRAIN_RECT_GREEN, &CDlgCrossSetting::OnBnClickedRadTrainRectGreen)
	ON_BN_CLICKED(IDC_RAD_TRAIN_RECT_BLUE, &CDlgCrossSetting::OnBnClickedRadTrainRectBlue)
	ON_BN_CLICKED(IDC_RAD_TRAIN_RECT_COLOR_SETTING, &CDlgCrossSetting::OnBnClickedRadTrainRectColorSetting)
	ON_EN_CHANGE(IDC_EDIT_TRAIN_RECT_LINE_WIDTH, &CDlgCrossSetting::OnEnChangeEditTrainRectLineWidth)

	ON_BN_CLICKED(IDC_RAD_OTHER_UI_RED, &CDlgCrossSetting::OnBnClickedRadOtherUIRed)
	ON_BN_CLICKED(IDC_RAD_OTHER_UI_GREEN, &CDlgCrossSetting::OnBnClickedRadOtherUIGreen)
	ON_BN_CLICKED(IDC_RAD_OTHER_UI_BLUE, &CDlgCrossSetting::OnBnClickedRadOtherUIBlue)
	ON_BN_CLICKED(IDC_RAD_OTHER_UI_COLOR_SETTING, &CDlgCrossSetting::OnBnClickedRadOtherUIColorSetting)
	ON_EN_CHANGE(IDC_EDIT_OTHER_UI_LINE_WIDTH, &CDlgCrossSetting::OnEnChangeEditOtherUILineWidth)


	ON_EN_SETFOCUS(IDC_EDIT_TRAIN_RECT_LINE_WIDTH, &CDlgCrossSetting::OnEnSetfocusEditTrainRectLineWidth)
	ON_EN_SETFOCUS(IDC_EDIT_OTHER_UI_LINE_WIDTH, &CDlgCrossSetting::OnEnSetfocusEditOtherUILineWidth)


END_MESSAGE_MAP()

void CDlgCrossSetting::UpdateDlgdata(bool flag)
{
	if(flag)
	{
		UpdateData(TRUE);
		m_Cross.nLineColor = m_ColorDisplay;
		m_Cross.nLineLength = m_ShowLength;
		m_Cross.nLineWidth = m_ShowWidth;
		m_Cross.nRotation = m_ShowStyle == 0?0:45;

		m_Cross.nTrainRectLineColor = m_TrainRectColorDisplay;
		m_Cross.nTrainRectLineWidth = m_nTrainRectShowWidth;


		m_Cross.nOtherGuiLineColor = m_OtherUIColorDisplay;
		m_Cross.nOtherUILineWidth = m_nOtherUIShowWidth;


		m_Cross.m_bShowDefortGui = m_bShowDefortGui;
		m_Cross.m_bShowRectGui = m_bShowTrainRect;
		m_Cross.m_bShowOtherGui = m_bShowOtherGui;
	}
	else
	{
		//颜色
		if(m_Cross.nLineColor == RGB(255,0,0))
		{
			m_ShowColor = 0;
			m_ColorDisplay = m_Cross.nLineColor;
		}
		else if(m_Cross.nLineColor == RGB(0,255,0))
		{
			m_ShowColor = 1;
			m_ColorDisplay = m_Cross.nLineColor;
		}
		else if(m_Cross.nLineColor == RGB(0,0,255))
		{
			m_ShowColor = 2;
			m_ColorDisplay = m_Cross.nLineColor;
		}
		else
		{
			m_ShowColor = 3;
			m_ColorDisplay = m_Cross.nLineColor;
		}

		

		//线长
		m_ShowLength = m_Cross.nLineLength;
		/*m_SliderLength .SetPos(m_ShowLength);*/

		//线宽
		m_ShowWidth = m_Cross.nLineWidth;
		/*m_SliderWidth.SetPos( m_ShowWidth);*/

		//角度
		if(m_Cross.nRotation == 0)
			m_ShowStyle = 0;
		else
			m_ShowStyle = 1;

		//颜色
		if(m_Cross.nTrainRectLineColor == RGB(255,0,0))
		{
			m_nTrainRectShowColor = 0;
			m_TrainRectColorDisplay = m_Cross.nTrainRectLineColor;
		}
		else if(m_Cross.nTrainRectLineColor == RGB(0,255,0))
		{
			m_nTrainRectShowColor = 1;
			m_TrainRectColorDisplay = m_Cross.nTrainRectLineColor;
		}
		else if(m_Cross.nTrainRectLineColor == RGB(0,0,255))
		{
			m_nTrainRectShowColor = 2;
			m_TrainRectColorDisplay = m_Cross.nTrainRectLineColor;
		}
		else
		{
			m_nTrainRectShowColor = 3;
			m_TrainRectColorDisplay = m_Cross.nTrainRectLineColor;
		}
		//线宽
		m_nTrainRectShowWidth = m_Cross.nTrainRectLineWidth;


		//颜色
		if(m_Cross.nOtherGuiLineColor == RGB(255,0,0))
		{
			m_nOtherUIShowColor = 0;
			m_OtherUIColorDisplay = m_Cross.nOtherGuiLineColor;
		}
		else if(m_Cross.nOtherGuiLineColor == RGB(0,255,0))
		{
			m_nOtherUIShowColor = 1;
			m_OtherUIColorDisplay = m_Cross.nOtherGuiLineColor;
		}
		else if(m_Cross.nOtherGuiLineColor == RGB(0,0,255))
		{
			m_nOtherUIShowColor = 2;
			m_OtherUIColorDisplay = m_Cross.nOtherGuiLineColor;
		}
		else
		{
			m_nOtherUIShowColor = 3;
			m_OtherUIColorDisplay = m_Cross.nOtherGuiLineColor;
		}
		//线宽
		m_nOtherUIShowWidth = m_Cross.nOtherUILineWidth;




		m_bShowDefortGui = m_Cross.m_bShowDefortGui;

		m_bShowTrainRect = m_Cross.m_bShowRectGui;
		m_bShowOtherGui = m_Cross.m_bShowOtherGui;

		UpdateData(FALSE);
		InvalidateRect(&m_RectDisPlay);
		UpdateWindow();
	}
}

void CDlgCrossSetting::SetCrossSize(SearchResultCrossSize nCrossSize)
{
	m_Cross = nCrossSize;
}


void CDlgCrossSetting::UpdateDialogLanguage()
{
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

		if(_T("IDD_DLG_CROSS_SETTING") == strDlgID)
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

BOOL CDlgCrossSetting::IsStringNumerical(LPCTSTR lpszVal)
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

// CDlgCrossSetting 消息处理程序


BOOL CDlgCrossSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateDialogLanguage();

	// TODO:  在此添加额外的初始化
	//获取DISPLAY的矩形
	CRect rect;
	CWnd* pWnd = NULL;

	//1
	pWnd = GetDlgItem(IDC_STATIC_DISPLAY);
	pWnd->GetClientRect(&rect);//得到控件客户端区域坐标
	pWnd->ClientToScreen(rect);//将区域坐标由 控件客户区转成对话框区
	this->ScreenToClient(rect); //将区域坐标由 对话框区转成对话框客户区坐标
	m_RectDisPlay = rect;

	//2
	pWnd = GetDlgItem(IDC_STATIC_TRAIN_RECT_DISPLAY);
	pWnd->GetClientRect(&rect);//得到控件客户端区域坐标
	pWnd->ClientToScreen(rect);//将区域坐标由 控件客户区转成对话框区
	this->ScreenToClient(rect); //将区域坐标由 对话框区转成对话框客户区坐标
	m_TrainRectRectDisPlay = rect;

	//3
	pWnd = GetDlgItem(IDC_STATIC_OTHERUI_DISPLAY);
	pWnd->GetClientRect(&rect);//得到控件客户端区域坐标
	pWnd->ClientToScreen(rect);//将区域坐标由 控件客户区转成对话框区
	this->ScreenToClient(rect); //将区域坐标由 对话框区转成对话框客户区坐标
	m_OtherUIRectDisPlay = rect;

	UpdateDlgdata(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCrossSetting::OnBnClickedRadRed()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_ColorDisplay = RGB(255,0,0);
	InvalidateRect(&m_RectDisPlay);
	UpdateWindow();
    OnLineColorChange();
}


HBRUSH CDlgCrossSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DISPLAY)// IDC_Display为所选文本框ID
	{

		//pDC->SetTextColor(RGB(255, 0, 0));//设置字体颜色

		pDC->SetBkColor(m_ColorDisplay);//设置背景颜色
		HBRUSH b = CreateSolidBrush(m_ColorDisplay);
		//pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return b;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TRAIN_RECT_DISPLAY)// IDC_Display为所选文本框ID
	{

		//pDC->SetTextColor(RGB(255, 0, 0));//设置字体颜色

		pDC->SetBkColor(m_TrainRectColorDisplay);//设置背景颜色
		HBRUSH b = CreateSolidBrush(m_TrainRectColorDisplay);
		//pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return b;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_OTHERUI_DISPLAY)// IDC_Display为所选文本框ID
	{

		//pDC->SetTextColor(RGB(255, 0, 0));//设置字体颜色

		pDC->SetBkColor(m_OtherUIColorDisplay);//设置背景颜色
		HBRUSH b = CreateSolidBrush(m_OtherUIColorDisplay);
		//pDC->SetBkMode(TRANSPARENT);//设置背景透明
		return b;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgCrossSetting::OnBnClickedRadGreen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_ColorDisplay = RGB(0,255,0);
	InvalidateRect(&m_RectDisPlay);
	UpdateWindow();
    OnLineColorChange();
}


void CDlgCrossSetting::OnBnClickedRadBlue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_ColorDisplay = RGB(0,0,255);
	InvalidateRect(&m_RectDisPlay);
	UpdateWindow();
    OnLineColorChange();
}


void CDlgCrossSetting::OnBnClickedRadColorSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    OnLineColorChange();
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_ColorDisplay = dlg.GetColor();
		InvalidateRect(&m_RectDisPlay);
		UpdateWindow();
	}
}


void CDlgCrossSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialog::OnOK();
}


void CDlgCrossSetting::OnEnChangeEditLineWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if(m_ShowWidth > 5)m_ShowWidth = 5;
	if(m_ShowWidth <= 0)m_ShowWidth = 1;
	UpdateData(FALSE);
// 	// TODO:  在此添加控件通知处理程序代码
}


void CDlgCrossSetting::OnEnChangeEditLineLength()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	if(m_ShowLength > 20000)m_ShowLength = 20000;
	if(m_ShowLength <= 1)m_ShowLength = 1;
	UpdateData(FALSE);

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgCrossSetting::OnEnSetfocusEditLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
	
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LINE_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LINE_WIDTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LINE_WIDTH, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[线条粗细]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,4);
            }
		}
	
	}
}


void CDlgCrossSetting::OnEnSetfocusEditLineLength()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_LINE_LENGTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_LINE_LENGTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_LINE_LENGTH, strOutput);
            if (strInput != strOutput)
            {
                CString strLog;
                strLog.Format(_T("修改[线条长短]由%s到%s\n"),strInput,strOutput);
                g_opLogFile.SaveLogText(strLog,4);
            }
		}
	}	
}




BOOL CDlgCrossSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CDlgCrossSetting::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::DestroyWindow();
}

void CDlgCrossSetting::OnLineColorChange()
{
    UpdateData(TRUE);
    if (m_nOldLineColor!= m_ShowColor)
    {
        CString strLog;
        CString strVal[] = {_T("红"),_T("绿"),_T("蓝"),_T("自定义")};
        strLog.Format(_T("修改[线条颜色][%s]为[%s]\n"),strVal[m_nOldLineColor],strVal[m_ShowColor]);
        g_opLogFile.SaveLogText(strLog,4);
    }
    m_nOldLineColor = m_ShowColor;
}


void CDlgCrossSetting::OnTrainRectLineColorChange()
{
	UpdateData(TRUE);
	if (m_nOldTrainRectShowColor!= m_nTrainRectShowColor)
	{
		CString strLog;
		CString strVal[] = {_T("红"),_T("绿"),_T("蓝"),_T("自定义")};
		strLog.Format(_T("修改[线条颜色][%s]为[%s]\n"),strVal[m_nOldTrainRectShowColor],strVal[m_nTrainRectShowColor]);
		g_opLogFile.SaveLogText(strLog,4);
	}
	m_nOldTrainRectShowColor = m_nTrainRectShowColor;
}


void CDlgCrossSetting::OnOtherUILineColorChange()
{
	UpdateData(TRUE);
	if (m_nOldOtherUIShowColor!= m_nOtherUIShowColor)
	{
		CString strLog;
		CString strVal[] = {_T("红"),_T("绿"),_T("蓝"),_T("自定义")};
		strLog.Format(_T("修改[线条颜色][%s]为[%s]\n"),strVal[m_nOldOtherUIShowColor],strVal[m_nOtherUIShowColor]);
		g_opLogFile.SaveLogText(strLog,4);
	}
	m_nOldOtherUIShowColor = m_nOtherUIShowColor;
}

void CDlgCrossSetting::OnBnClickedRadCrosszero()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bOldShowStyle!= m_ShowStyle)
    {
        CString strLog;
        strLog.Format(_T("修改[十字类型][斜十字]为[正十字]\n"));
        g_opLogFile.SaveLogText(strLog,4);
    }
    m_bOldShowStyle = m_ShowStyle;
}


void CDlgCrossSetting::OnBnClickedRadCross45()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bOldShowStyle!= m_ShowStyle)
    {
        CString strLog;
        strLog.Format(_T("修改[十字类型][正十字]为[斜十字]\n"));
        g_opLogFile.SaveLogText(strLog,4);
    }
    m_bOldShowStyle = m_ShowStyle;
}


void CDlgCrossSetting::OnBnClickedCheckShowDefortGui()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CString strLog;
    CString strValue = m_bShowDefortGui ? _T("选中状态"):_T("未选中状态");
    strLog.Format(_T("修改[主界面显示]为%s\n"),strValue);
    g_opLogFile.SaveLogText(strLog,4);
}



void CDlgCrossSetting::OnBnClickedRadTrainRectRed()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_TrainRectColorDisplay = RGB(255,0,0);
	InvalidateRect(&m_TrainRectRectDisPlay);
	UpdateWindow();
	OnTrainRectLineColorChange();
}

void CDlgCrossSetting::OnBnClickedRadTrainRectGreen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_TrainRectColorDisplay = RGB(0,255,0);
	InvalidateRect(&m_TrainRectRectDisPlay);
	UpdateWindow();
	OnTrainRectLineColorChange();
}


void CDlgCrossSetting::OnBnClickedRadTrainRectBlue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_TrainRectColorDisplay = RGB(0,0,255);
	InvalidateRect(&m_TrainRectRectDisPlay);
	UpdateWindow();
	OnTrainRectLineColorChange();
}


void CDlgCrossSetting::OnBnClickedRadTrainRectColorSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnTrainRectLineColorChange();
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_TrainRectColorDisplay = dlg.GetColor();
		InvalidateRect(&m_TrainRectRectDisPlay);
		UpdateWindow();
	}
}


void CDlgCrossSetting::OnEnChangeEditTrainRectLineWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if(m_nTrainRectShowWidth > 5) m_nTrainRectShowWidth = 5;
	if(m_nTrainRectShowWidth <= 0) m_nTrainRectShowWidth = 1;
	UpdateData(FALSE);
	// 	// TODO:  在此添加控件通知处理程序代码
}

void CDlgCrossSetting::OnEnSetfocusEditTrainRectLineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{

		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_TRAIN_RECT_LINE_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_TRAIN_RECT_LINE_WIDTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_TRAIN_RECT_LINE_WIDTH, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[训练矩形线条粗细]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}

	}
}



void CDlgCrossSetting::OnBnClickedRadOtherUIRed()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_OtherUIColorDisplay = RGB(255,0,0);
	InvalidateRect(&m_OtherUIRectDisPlay);
	UpdateWindow();
	OnOtherUILineColorChange();
}

void CDlgCrossSetting::OnBnClickedRadOtherUIGreen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_OtherUIColorDisplay = RGB(0,255,0);
	InvalidateRect(&m_OtherUIRectDisPlay);
	UpdateWindow();
	OnOtherUILineColorChange();
}

void CDlgCrossSetting::OnBnClickedRadOtherUIBlue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_OtherUIColorDisplay = RGB(0,0,255);
	InvalidateRect(&m_OtherUIRectDisPlay);
	UpdateWindow();
	OnOtherUILineColorChange();
}


void CDlgCrossSetting::OnBnClickedRadOtherUIColorSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOtherUILineColorChange();
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_OtherUIColorDisplay = dlg.GetColor();
		InvalidateRect(&m_OtherUIRectDisPlay);
		UpdateWindow();
	}
}


void CDlgCrossSetting::OnEnChangeEditOtherUILineWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	if(m_nOtherUIShowWidth > 5) m_nOtherUIShowWidth = 5;
	if(m_nOtherUIShowWidth <= 0) m_nOtherUIShowWidth = 1;
	UpdateData(FALSE);
	// 	// TODO:  在此添加控件通知处理程序代码
}


void CDlgCrossSetting::OnEnSetfocusEditOtherUILineWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{

		::PostMessage(m_pParent->GetSafeHwnd(),WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_OTHER_UI_LINE_WIDTH)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_OTHER_UI_LINE_WIDTH, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_OTHER_UI_LINE_WIDTH, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[其他图形线条粗细]由%s到%s\n"),strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,4);
			}
		}

	}
}