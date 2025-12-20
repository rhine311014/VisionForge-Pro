// DlgDmCodeShutterSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibTargetShutterSetting.h"
#include "afxdialogex.h"

#include "LogFileSave.h"

// CDlgCalibTargetShutterSetting 对话框

IMPLEMENT_DYNAMIC(CDlgCalibTargetShutterSetting, CDialog)

CDlgCalibTargetShutterSetting::CDlgCalibTargetShutterSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibTargetShutterSetting::IDD, pParent)
{
   
	m_dDmCodeShutter = 0;
	m_dEnvironmentShutter = 0;
	m_nPosIndex = 0;
	m_psaSysInfoStrings = NULL;
}

CDlgCalibTargetShutterSetting::~CDlgCalibTargetShutterSetting()
{
}

void CDlgCalibTargetShutterSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PRODUCT_SHUTTER, m_sliderDmcodeShutter);
}


BEGIN_MESSAGE_MAP(CDlgCalibTargetShutterSetting, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_RESET, &CDlgCalibTargetShutterSetting::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CDlgCalibTargetShutterSetting 消息处理程序

void CDlgCalibTargetShutterSetting::OnBnClickedBtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strVal;
	m_sliderDmcodeShutter.SetPos( (int) ((m_dEnvironmentShutter+0.005) / 0.01) );
	strVal.Format(_T("%.2fms"), m_dEnvironmentShutter);
	GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);

	if(m_pVisionASM== NULL) return;

	m_pVisionASM->SwitchToSearchShutter(m_nPosIndex, m_dEnvironmentShutter);
	m_dDmCodeShutter = m_dEnvironmentShutter;
}


BOOL CDlgCalibTargetShutterSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTitle = _T("");

	strTitle.Format(_T("位置%d相机曝光设置"),m_nPosIndex+1);
	SetWindowText(strTitle);
	// TODO:  在此添加额外的初始化
	//UpdateDialogLanguage();
	if (m_pVisionASM != NULL)
	{
		// 更新曝光显示		
		// 		double dCurrent1 = 0, dMin1 = 0, dMax1 = 0;
		// 		m_pParent->m_pVisionASM->GetPosCameraShutter(m_pParent->m_nCurPositionIdx, dCurrent1, dMin1, dMax1);

		CString strVal;
		double dCurrent = 0, dMin = 0, dMax = 0;
		if (m_pVisionASM->GetPosCameraShutter(m_nPosIndex, dCurrent, dMin, dMax))
		{
			m_sliderDmcodeShutter.EnableWindow(TRUE);
			m_sliderDmcodeShutter.SetRange(ceil(dMin/0.01), floor(dMax/0.01), TRUE);
			m_sliderDmcodeShutter.SetPos( (int) ((dCurrent+0.005) / 0.01) );
			strVal.Format(_T("%.2fms"), dCurrent);
			GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);

			m_dDmCodeShutter = dCurrent;
		}
		else
		{
			m_sliderDmcodeShutter.EnableWindow(FALSE);
		}

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgCalibTargetShutterSetting::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strVal;
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_PRODUCT_SHUTTER:
		{
			if (m_pVisionASM != NULL)
			{
				// 设置相机的曝光时间
				m_dDmCodeShutter = (double)(m_sliderDmcodeShutter.GetPos())*0.01;
				strVal.Format(_T("%.2fms"), m_dDmCodeShutter);
				GetDlgItem(IDC_STATIC_SHUTTER_VAL)->SetWindowText(strVal);
				m_pVisionASM->SwitchToSearchShutter(m_nPosIndex, m_dDmCodeShutter);
				if (m_dDmCodeShutterOld != m_dDmCodeShutter)
				{
					CString strLog;
					strLog.Format(_T("修改位置%d[曝光]由%lf到%lf"),m_nPosIndex,m_dDmCodeShutterOld,m_dDmCodeShutter);
					g_opLogFile.SaveLogText(strLog,6);
				}
				m_dDmCodeShutterOld = m_dDmCodeShutter;
			}
		}
		break;
	default:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgCalibTargetShutterSetting::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
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

		if(_T("IDD_DLG_DMCODE_SHUTTER_SETTING") == strDlgID)
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

void CDlgCalibTargetShutterSetting::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	m_pVisionASM = pVisionAligner;
}