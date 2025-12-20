// DlgProductOffsetSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgProductOffsetSetting.h"
#include "afxdialogex.h"
#include "LogFileSave.h"
#include "svXMLConfigurator.h"

// CDlgProductOffsetSetting 对话框

IMPLEMENT_DYNAMIC(CDlgProductOffsetSetting, CDialog)

CDlgProductOffsetSetting::CDlgProductOffsetSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductOffsetSetting::IDD, pParent)
{

// 	m_nProductOffsetXDirection = 0;
// 	m_nProductOffsetYDirection = 0;
// 	m_nProductOffsetDDirection = 0;
// 
// 	m_nProductOffsetXSetting = 0;
// 	m_nProductOffsetYSetting = 0;
// 	m_nProductOffsetDSetting = 0;

	m_bEnableProductOffsetXDirection = FALSE;
	m_bEnableProductOffsetYDirection = FALSE;
	m_bEnableProductOffsetDDirection = FALSE;

	m_bEnableProductOffsetXSetting = FALSE;
	m_bEnableProductOffsetYSetting = FALSE;
	m_bEnableProductOffsetDSetting = FALSE;


}

CDlgProductOffsetSetting::~CDlgProductOffsetSetting()
{
}

void CDlgProductOffsetSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_X_DIRECTION, m_ProductOffsetXDirection);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_Y_DIRECTION, m_ProductOffsetYDirection);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_D_DIRECTION, m_ProductOffsetDDirection);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_D_SETTING, m_ProductOffsetDSetting);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_X_SETTING, m_ProductOffsetXSetting);
// 	DDX_Control(pDX, IDC_COMBO_PRODUCT_OFFSET_Y_SETTING, m_ProductOffsetYSetting);

	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_X_DIRECTION, m_bEnableProductOffsetXDirection);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_Y_DIRECTION, m_bEnableProductOffsetYDirection);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_D_DIRECTION, m_bEnableProductOffsetDDirection);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_X_SETTING, m_bEnableProductOffsetXSetting);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_Y_SETTING, m_bEnableProductOffsetYSetting);
	DDX_Check(pDX, IDC_CHECK_PRODUCT_OFFSET_D_SETTING, m_bEnableProductOffsetDSetting);
}


BEGIN_MESSAGE_MAP(CDlgProductOffsetSetting, CDialog)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_X_DIRECTION, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetXDirection)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_Y_DIRECTION, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetYDirection)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_D_DIRECTION, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetDDirection)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_X_SETTING, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetXSetting)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_Y_SETTING, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetYSetting)
// 	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_OFFSET_D_SETTING, &CDlgProductOffsetSetting::OnSelchangeComboProductOffsetDSetting)
	ON_BN_CLICKED(IDCANCEL, &CDlgProductOffsetSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgProductOffsetSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_X_DIRECTION, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetXDirection)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_Y_DIRECTION, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetYDirection)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_D_DIRECTION, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetDDirection)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_X_SETTING, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetXSetting)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_Y_SETTING, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetYSetting)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_OFFSET_D_SETTING, &CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetDSetting)
END_MESSAGE_MAP()


// CDlgProductOffsetSetting 消息处理程序


BOOL CDlgProductOffsetSetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strLog;
	strLog.Format(_T("进入[对象补偿设置][补偿方向设置]\n"));
	g_opLogFile.SaveLogText(strLog,4);
	// TODO:  在此添加额外的初始化

// 	m_ProductOffsetXDirection.ResetContent();
// 	m_ProductOffsetXDirection.AddString(_T("向右"));	//_T("向右")
// 	m_ProductOffsetXDirection.AddString(_T("向左"));	//_T("向左")

// 	m_ProductOffsetYDirection.ResetContent();
// 	m_ProductOffsetYDirection.AddString(_T("向下"));	//_T("向下")
// 	m_ProductOffsetYDirection.AddString(_T("向上"));		//_T("向上")

// 	m_ProductOffsetDDirection.ResetContent();
// 	m_ProductOffsetDDirection.AddString(_T("顺时针"));		//_T("顺时针")
// 	m_ProductOffsetDDirection.AddString(_T("逆时针"));//_T("逆时针")



// 	m_ProductOffsetXSetting.ResetContent();
// 	m_ProductOffsetXSetting.AddString(_T("X"));			//_T("向右")
// 	m_ProductOffsetXSetting.AddString(_T("Y"));			//_T("向左")
// 
// 	m_ProductOffsetYSetting.ResetContent();
// 	m_ProductOffsetYSetting.AddString(_T("Y"));			//_T("向下")
// 	m_ProductOffsetYSetting.AddString(_T("X"));			//_T("向上")
// 
// 	m_ProductOffsetDSetting.ResetContent();
// 	m_ProductOffsetDSetting.AddString(_T("θ"));		//_T("顺时针")
// 	m_ProductOffsetDSetting.AddString(_T("θ"));		//_T("逆时针")
    UpdateDialogLanguage();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgProductOffsetSetting::UpdateDialogLanguage()
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

        if(_T("IDD_DLG_PRODUCT_OFFSET_SETTING") == strDlgID)
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


// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetXDirection()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetXDirection = m_ProductOffsetXDirection.GetCurSel();
// }


// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetYDirection()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetYDirection = m_ProductOffsetYDirection.GetCurSel();
// }


// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetDDirection()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetDDirection = m_ProductOffsetDDirection.GetCurSel();
// }


// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetXSetting()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetXSetting = m_ProductOffsetXSetting.GetCurSel();
// }


// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetYSetting()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetYSetting = m_ProductOffsetYSetting.GetCurSel();
// }

// 
// void CDlgProductOffsetSetting::OnSelchangeComboProductOffsetDSetting()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 
// 	m_nProductOffsetDSetting = m_ProductOffsetDSetting.GetCurSel();
// 
// }


void CDlgProductOffsetSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[取消]退出"));
	g_opLogFile.SaveLogText(strLog,4);
	CDialog::OnCancel();
}


void CDlgProductOffsetSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("[保存]退出"));
	g_opLogFile.SaveLogText(strLog,4);

	CDialog::OnOK();
	
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetXDirection()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetXDirectionOld != m_bEnableProductOffsetXDirection)
	{
		CString strValue = m_bEnableProductOffsetXDirection ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[X方向取反]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetXDirectionOld = m_bEnableProductOffsetXDirection;
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetYDirection()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetYDirectionOld != m_bEnableProductOffsetYDirection)
	{
		CString strValue = m_bEnableProductOffsetYDirection ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[Y方向取反]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetYDirectionOld = m_bEnableProductOffsetYDirection;
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetDDirection()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetDDirectionOld != m_bEnableProductOffsetDDirection)
	{
		CString strValue = m_bEnableProductOffsetDDirection ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[D方向取反]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetDDirectionOld = m_bEnableProductOffsetDDirection;
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetXSetting()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetXSettingOld != m_bEnableProductOffsetXSetting)
	{
		CString strValue = m_bEnableProductOffsetXSetting ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[X方向与Y方向对调]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetXSettingOld = m_bEnableProductOffsetXSetting;
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetYSetting()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetYSettingOld != m_bEnableProductOffsetYSetting)
	{
		CString strValue = m_bEnableProductOffsetYSetting ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[补偿值为Y]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetYSettingOld = m_bEnableProductOffsetYSetting;
}


void CDlgProductOffsetSetting::OnBnClickedCheckProductOffsetDSetting()
{
	// TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
	CString strLog;
	if (bEnableProductOffsetDSettingOld != m_bEnableProductOffsetDSetting)
	{
		CString strValue = m_bEnableProductOffsetDSetting ? _T("选中状态"):_T("未选中状态");
		strLog.Format(_T("修改[补偿值为D]为%s\n"),strValue);
		g_opLogFile.SaveLogText(strLog,5);
	}
	bEnableProductOffsetDSettingOld = m_bEnableProductOffsetDSetting;
}
