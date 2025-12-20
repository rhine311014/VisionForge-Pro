// DlgLightCtrlSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLightCtrlSetting.h"
#include "afxdialogex.h"
#include "LightController.h"

// CDlgLightCtrlSetting 对话框

IMPLEMENT_DYNAMIC(CDlgLightCtrlSetting, CDialogEx)

	CDlgLightCtrlSetting::CDlgLightCtrlSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLightCtrlSetting::IDD, pParent)
	, m_nRdLightControl(0)
{

}

CDlgLightCtrlSetting::~CDlgLightCtrlSetting()
{
}

void CDlgLightCtrlSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RD_LIGHT_CONTROL1, m_nRdLightControl);

	DDX_Control(pDX, IDC_SLIDER_CHANNEL1, m_sliderChannel[0]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL2, m_sliderChannel[1]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL3, m_sliderChannel[2]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL4, m_sliderChannel[3]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL5, m_sliderChannel[4]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL6, m_sliderChannel[5]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL7, m_sliderChannel[6]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL8, m_sliderChannel[7]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL1, m_stCurChannel[0]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL2, m_stCurChannel[1]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL3, m_stCurChannel[2]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL4, m_stCurChannel[3]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL5, m_stCurChannel[4]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL6, m_stCurChannel[5]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL7, m_stCurChannel[6]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL8, m_stCurChannel[7]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL1, m_stMaxChannel[0]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL2, m_stMaxChannel[1]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL3, m_stMaxChannel[2]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL4, m_stMaxChannel[3]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL5, m_stMaxChannel[4]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL6, m_stMaxChannel[5]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL7, m_stMaxChannel[6]);
	DDX_Control(pDX, IDC_STATIC_MAX_CHANNEL8, m_stMaxChannel[7]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL1, m_stMinChannel[0]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL2, m_stMinChannel[1]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL3, m_stMinChannel[2]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL4, m_stMinChannel[3]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL5, m_stMinChannel[4]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL6, m_stMinChannel[5]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL7, m_stMinChannel[6]);
	DDX_Control(pDX, IDC_STATIC_MIN_CHANNEL8, m_stMinChannel[7]);
	DDX_Text(pDX, IDC_EDIT_CH1, m_strEditName[0]);
	DDX_Text(pDX, IDC_EDIT_CH2, m_strEditName[1]);
	DDX_Text(pDX, IDC_EDIT_CH3, m_strEditName[2]);
	DDX_Text(pDX, IDC_EDIT_CH4, m_strEditName[3]);
	DDX_Text(pDX, IDC_EDIT_CH5, m_strEditName[4]);
	DDX_Text(pDX, IDC_EDIT_CH6, m_strEditName[5]);
	DDX_Text(pDX, IDC_EDIT_CH7, m_strEditName[6]);
	DDX_Text(pDX, IDC_EDIT_CH8, m_strEditName[7]);
}


BEGIN_MESSAGE_MAP(CDlgLightCtrlSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLightCtrlSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLightCtrlSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL1, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL2, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL3, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL4, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL5, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL6, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL7, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL8, &CDlgLightCtrlSetting::OnBnClickedRdLightControl1)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CDlgLightCtrlSetting 消息处理程序

void CDlgLightCtrlSetting::SetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig)
{
	m_SystemOptionLightConfig = systemOptionLightConfig;
}


void CDlgLightCtrlSetting::GetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig)
{
	systemOptionLightConfig = m_SystemOptionLightConfig;
}

void CDlgLightCtrlSetting::InitDlgItem()
{
	int nCtrlId[] = {IDC_RD_LIGHT_CONTROL1, IDC_RD_LIGHT_CONTROL2, IDC_RD_LIGHT_CONTROL3,
		IDC_RD_LIGHT_CONTROL4, IDC_RD_LIGHT_CONTROL5, IDC_RD_LIGHT_CONTROL6, IDC_RD_LIGHT_CONTROL7,
		IDC_RD_LIGHT_CONTROL8};

	for (int i = 0; i < MAXLightControl; i++)
	{
		bool bEnable = m_SystemOptionLightConfig.m_bLightCtrlUsed[i] && CLightController::GetInstance().IsOpen(i);
		GetDlgItem(nCtrlId[i])->EnableWindow(bEnable);
	}

	//控制器名称
	GetDlgItem(IDC_RD_LIGHT_CONTROL1)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[0]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL2)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[1]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL3)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[2]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL4)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[3]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL5)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[4]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL6)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[5]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL7)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[6]);
	GetDlgItem(IDC_RD_LIGHT_CONTROL8)->SetWindowText(m_SystemOptionLightConfig.m_strLightCtrlName[7]);
}

void CDlgLightCtrlSetting::UpdateDlgData(int nIndex, bool bSaveAndValidate)
{
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		for (int i = 0; i < MAXLightChannelNum; i++)
		{
			int nPos = m_sliderChannel[i].GetPos();
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nChannelValue[i] = nPos;
			//m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nChannelValue[i] = nPos;
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_strName[i] = m_strEditName[i];//Summer0214
		}
	}
	else
	{
		///
		int nLightType = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nLightType;
		int nMinIntensity = 0;
		int nMaxIntensity = 0;
		CLightController::GetIntensityRange((LightControlType)nLightType, nMinIntensity, nMaxIntensity);

		CString strMin;
		CString strMax;
		CString strCur;

		strMin.Format(_T("%d"), nMinIntensity);
		strMax.Format(_T("%d"), nMaxIntensity);

		for (int i = 0; i < MAXLightChannelNum; i++)
		{
			strCur.Format(_T("%d"), m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nChannelValue[i]);

			m_stMinChannel[i].SetWindowText(strMin);
			m_stMaxChannel[i].SetWindowText(strMax);
			m_stCurChannel[i].SetWindowText(strCur);

			m_sliderChannel[i].SetRange(nMinIntensity, nMaxIntensity);
			m_sliderChannel[i].SetPos(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nChannelValue[i]);

			m_strEditName[i] = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_strName[i];//Summer0214
		}

		UpdateData(FALSE);
	}

	UpdateShowStatus(nIndex);
}

void CDlgLightCtrlSetting::UpdateShowStatus(int nIndex)
{
	bool bOpened = CLightController::GetInstance().IsOpen(nIndex);
	bool bEnable = m_SystemOptionLightConfig.m_bLightCtrlUsed[nIndex];
	int nMaxChannel = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nMaxChannels;

	GetDlgItem(IDC_STATIC_GROUP_CHANNEL_SETTING)->EnableWindow(bEnable && bOpened);

	GetDlgItem(IDC_SLIDER_CHANNEL1)->EnableWindow(bEnable && bOpened && nMaxChannel >= 1 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[0]);
	GetDlgItem(IDC_SLIDER_CHANNEL2)->EnableWindow(bEnable && bOpened && nMaxChannel >= 2 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[1]);
	GetDlgItem(IDC_SLIDER_CHANNEL3)->EnableWindow(bEnable && bOpened && nMaxChannel >= 3 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[2]);
	GetDlgItem(IDC_SLIDER_CHANNEL4)->EnableWindow(bEnable && bOpened && nMaxChannel >= 4 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[3]);
	GetDlgItem(IDC_SLIDER_CHANNEL5)->EnableWindow(bEnable && bOpened && nMaxChannel >= 5 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[4]);
	GetDlgItem(IDC_SLIDER_CHANNEL6)->EnableWindow(bEnable && bOpened && nMaxChannel >= 6 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[5]);
	GetDlgItem(IDC_SLIDER_CHANNEL7)->EnableWindow(bEnable && bOpened && nMaxChannel >= 7 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[6]);
	GetDlgItem(IDC_SLIDER_CHANNEL8)->EnableWindow(bEnable && bOpened && nMaxChannel >= 8 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[7]);

	GetDlgItem(IDC_STATIC_MIN_CHANNEL1)->EnableWindow(bEnable && bOpened && nMaxChannel >= 1 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[0]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL2)->EnableWindow(bEnable && bOpened && nMaxChannel >= 2 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[1]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL3)->EnableWindow(bEnable && bOpened && nMaxChannel >= 3 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[2]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL4)->EnableWindow(bEnable && bOpened && nMaxChannel >= 4 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[3]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL5)->EnableWindow(bEnable && bOpened && nMaxChannel >= 5 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[4]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL6)->EnableWindow(bEnable && bOpened && nMaxChannel >= 6 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[5]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL7)->EnableWindow(bEnable && bOpened && nMaxChannel >= 7 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[6]);
	GetDlgItem(IDC_STATIC_MIN_CHANNEL8)->EnableWindow(bEnable && bOpened && nMaxChannel >= 8 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[7]);

	GetDlgItem(IDC_STATIC_MAX_CHANNEL1)->EnableWindow(bEnable && bOpened && nMaxChannel >= 1 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[0]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL2)->EnableWindow(bEnable && bOpened && nMaxChannel >= 2 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[1]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL3)->EnableWindow(bEnable && bOpened && nMaxChannel >= 3 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[2]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL4)->EnableWindow(bEnable && bOpened && nMaxChannel >= 4 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[3]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL5)->EnableWindow(bEnable && bOpened && nMaxChannel >= 5 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[4]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL6)->EnableWindow(bEnable && bOpened && nMaxChannel >= 6 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[5]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL7)->EnableWindow(bEnable && bOpened && nMaxChannel >= 7 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[6]);
	GetDlgItem(IDC_STATIC_MAX_CHANNEL8)->EnableWindow(bEnable && bOpened && nMaxChannel >= 8 && m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[7]);
}


BOOL CDlgLightCtrlSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitDlgItem(); 

	UpdateDlgData(m_nRdLightControl, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgLightCtrlSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDlgData(m_nRdLightControl, TRUE);

	CDialogEx::OnOK();
}


void CDlgLightCtrlSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CDlgLightCtrlSetting::OnBnClickedRdLightControl1()
{
	int nPreSel = m_nRdLightControl;
	UpdateData(TRUE);
	if (nPreSel == m_nRdLightControl)
	{
		return;
	}
	UpdateDlgData(nPreSel, TRUE);
	UpdateDlgData(m_nRdLightControl, FALSE);
}

void CDlgLightCtrlSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nCtrlId = pScrollBar->GetDlgCtrlID();

	switch (nCtrlId)
	{
	case IDC_SLIDER_CHANNEL1:
		{
			int nPos = m_sliderChannel[0].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[0].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 0, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL2:
		{
			int nPos = m_sliderChannel[1].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[1].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 1, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL3:
		{
			int nPos = m_sliderChannel[2].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[2].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 2, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL4:
		{
			int nPos = m_sliderChannel[3].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[3].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 3, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL5:
		{
			int nPos = m_sliderChannel[4].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[4].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 4, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL6:
		{
			int nPos = m_sliderChannel[5].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[5].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 5, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL7:
		{
			int nPos = m_sliderChannel[6].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[6].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 6, nPos);
		}
		return;
	case IDC_SLIDER_CHANNEL8:
		{
			int nPos = m_sliderChannel[7].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[7].SetWindowText(strCur);
			CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 7, nPos);
		}
		return;
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
