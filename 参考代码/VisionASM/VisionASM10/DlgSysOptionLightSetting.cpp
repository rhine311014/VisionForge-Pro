// DlgSysOptionLightSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSysOptionLightSetting.h"
#include "afxdialogex.h"
#include "LightController.h"
#include "VisionASMDLG.h"
#include "vsBaseVisionAlign.h"
#include "DlgLightCtrlRename.h"

// CDlgSysOptionLightSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSysOptionLightSetting, CDialogEx)

	CDlgSysOptionLightSetting::CDlgSysOptionLightSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysOptionLightSetting::IDD, pParent)
	, m_nRdLightControl(0)
	, m_nEditPort(0)
	, m_nMaxChannelSum(8)
{
	m_bEnableLightControl = TRUE;
	m_bShowLightControl = TRUE;
	m_bSysKeyboardEnabled = FALSE;

}

CDlgSysOptionLightSetting::~CDlgSysOptionLightSetting()
{
}

void CDlgSysOptionLightSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RD_LIGHT_CONTROL1, m_nRdLightControl);
	DDX_Control(pDX, IDC_COMBO_LIGHT_TYPE, m_cbbLightType);
	DDX_Control(pDX, IDC_COMBO_COMM_TYPE, m_cbbCommType);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_cbbBaudRate);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nEditPort);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipAddressCtrl);
	DDX_Check(pDX, IDC_CHK_ENABLE, m_bChkEnable);
	DDX_Control(pDX, IDC_COMBO_SERIAL_PORT, m_cbbSerialPort);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL1, m_sliderChannel[0]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL2, m_sliderChannel[1]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL3, m_sliderChannel[2]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL4, m_sliderChannel[3]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL5, m_sliderChannel[4]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL6, m_sliderChannel[5]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL7, m_sliderChannel[6]);
	DDX_Control(pDX, IDC_SLIDER_CHANNEL8, m_sliderChannel[7]);
	DDX_Control(pDX, IDC_STATIC_BAUDRATE, m_stBaudRate);
	//  DDX_Control(pDX, IDC_STATIC_COMM_TYPE, m_stCommType);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL1, m_stCurChannel[0]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL2, m_stCurChannel[1]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL3, m_stCurChannel[2]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL4, m_stCurChannel[3]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL5, m_stCurChannel[4]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL6, m_stCurChannel[5]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL7, m_stCurChannel[6]);
	DDX_Control(pDX, IDC_STATIC_CUR_CHANNEL8, m_stCurChannel[7]);
	DDX_Control(pDX, IDC_STATIC_IP_ADDR, m_stIPAddr);
	DDX_Control(pDX, IDC_STATIC_SERIAL_PORT, m_stSerialPort);
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
	DDX_Control(pDX, IDC_STATIC_PORT, m_stPort);
	DDX_Text(pDX, IDC_EDIT_NAME_1, m_strEditName[0]);
	DDX_Text(pDX, IDC_EDIT_NAME_2, m_strEditName[1]);
	DDX_Text(pDX, IDC_EDIT_NAME_3, m_strEditName[2]);
	DDX_Text(pDX, IDC_EDIT_NAME_4, m_strEditName[3]);
	DDX_Text(pDX, IDC_EDIT_NAME_5, m_strEditName[4]);
	DDX_Text(pDX, IDC_EDIT_NAME_6, m_strEditName[5]);
	DDX_Text(pDX, IDC_EDIT_NAME_7, m_strEditName[6]);
	DDX_Text(pDX, IDC_EDIT_NAME_8, m_strEditName[7]);
	DDX_Text(pDX, IDC_EDIT_MAX_CHANNEL_SUM, m_nMaxChannelSum);
}


BEGIN_MESSAGE_MAP(CDlgSysOptionLightSetting, CDialogEx)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL1, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL2, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL3, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL4, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL5, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL6, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL7, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_BN_CLICKED(IDC_RD_LIGHT_CONTROL8, &CDlgSysOptionLightSetting::OnBnClickedRdLightControl1)
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT_TYPE, &CDlgSysOptionLightSetting::OnCbnSelchangeComboLightType)
	ON_CBN_SELCHANGE(IDC_COMBO_COMM_TYPE, &CDlgSysOptionLightSetting::OnCbnSelchangeComboCommType)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CDlgSysOptionLightSetting::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_CHK_ENABLE, &CDlgSysOptionLightSetting::OnBnClickedChkEnable)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_OPEN_ALL, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenAll)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL1, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel1)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL2, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel2)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL3, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel3)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL4, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel4)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL5, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel5)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL6, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel6)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL7, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel7)
	ON_BN_CLICKED(IDC_BTN_OPEN_CHANNEL8, &CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel8)
	ON_BN_CLICKED(IDC_BTN_CLOSE_ALL, &CDlgSysOptionLightSetting::OnBnClickedBtnCloseAll)
	ON_EN_SETFOCUS(IDC_EDIT_PORT, &CDlgSysOptionLightSetting::OnEnSetfocusEditPort)
	ON_EN_SETFOCUS(IDC_IPADDRESS, &CDlgSysOptionLightSetting::OnSetfocusIpaddress)
	ON_EN_KILLFOCUS(IDC_EDIT_PORT, &CDlgSysOptionLightSetting::OnEnKillfocusEditPort)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_CTRL_RENAME, &CDlgSysOptionLightSetting::OnBnClickedButtonLightCtrlRename)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_CHANNEL_SUM, &CDlgSysOptionLightSetting::OnEnSetfocusEditMaxChannelSum)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_CHANNEL_SUM, &CDlgSysOptionLightSetting::OnEnKillfocusEditMaxChannelSum)
END_MESSAGE_MAP()


// CDlgSysOptionLightSetting 消息处理程序


BOOL CDlgSysOptionLightSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 初始化窗口控件
	InitDlgItem();
	UpdateDlgByAuthorize();
	UpdateDlgData(0, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgSysOptionLightSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 初始化GUI
// eLightControlUnknown = -1,	// 未知
// 	eLightControlLTS = 0,       // 乐视
// 	eLightControlLTS2 =1,       // 乐视2
// 	eLightControlCST = 10,       // 康视达
// 	eLightControlCST2 = 11,      // 康视达2
// 	eLightControlFST = 20,       // 菲视特
// 	eLightControlFST2 = 21,      // 菲视特2
// 	eLightControlFST3 = 22,      // 菲视特3
// 	eLightControlOPT  = 30,      // 奥普特
// 	eLightControlOPT2 = 31,      // 奥普特2
// 	eLightControlLKS  = 40,      // 蓝科胜
void CDlgSysOptionLightSetting::InitDlgItem()
{
	// 初始化ComboBox
	// 1.光源类型
	{
		const char* pszString[] = {"乐视", "乐视2", "康视达", "康视达2", "菲视特", "菲视特2", "菲视特3", "奥普特","奥普特2","蓝科胜"};
		int nLightTypes[] = {eLightControlLTS, eLightControlLTS2, eLightControlCST, eLightControlCST2, 
			eLightControlFST, eLightControlFST2,eLightControlFST3,eLightControlOPT, eLightControlOPT2,
			eLightControlLKS};

		for (int i = 0; i < sizeof(pszString)/sizeof(char*); i++)
		{
			m_cbbLightType.AddString(pszString[i]);
			m_cbbLightType.SetItemData(i, nLightTypes[i]);
		}

		m_cbbLightType.SetCurSel(0);
	}
	// 2.通信类型
	{
		m_cbbCommType.AddString(_T("串口"));
		m_cbbCommType.AddString(_T("网口"));
		m_cbbCommType.SetCurSel(0);
	}

	// 3. 串口号
	{
		CString strSerialComm;
		for (int i = 0; i < 8; i++)
		{
			strSerialComm.Format(_T("COM%d"), i+1);
			m_cbbSerialPort.AddString(strSerialComm);
		}

		m_cbbSerialPort.SetCurSel(0);
	}

	// 4.波特率
	{
		const char* pszString[] = {"300", "600","1200","2400","4800","9600","14400","19200","38400","56000",
			"57600", "115200"};
		for (int i = 0; i < sizeof(pszString)/sizeof(char*); i++)
		{
			m_cbbBaudRate.AddString(pszString[i]);
		}
		m_cbbBaudRate.SetCurSel(8);
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

// 更新界面数据
void CDlgSysOptionLightSetting::UpdateDlgData(int nIndex, bool bSaveAndValidate)
{
	if (nIndex < 0 || nIndex > MAXLightControl)
	{
		return;
	}

	//m_SystemOptionLightConfig.m_bLightCtrlUsed[nIndex];
	//m_SystemOptionLightConfig.m_lightCtrlParam[nIndex];

	if (bSaveAndValidate)
	{
		UpdateData(TRUE);

		m_SystemOptionLightConfig.m_bLightCtrlUsed[nIndex] = m_bChkEnable;

		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nLightType = m_cbbLightType.GetItemData(m_cbbLightType.GetCurSel());

		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nCommType = m_cbbCommType.GetCurSel();

		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nSerialPort = m_cbbSerialPort.GetCurSel();

		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nMaxChannels = m_nMaxChannelSum;

		CString strBaudRate;
		m_cbbBaudRate.GetWindowText(strBaudRate);
		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nBaundRate = atoi(strBaudRate);

		BYTE feild0 = 0, feild1 = 0, feild2 = 0, feild3 = 0;
		m_ipAddressCtrl.GetAddress(feild0, feild1, feild2, feild3);
		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[0] = feild0;
		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[1] = feild1;
		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[2] = feild2;
		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[3] = feild3;

		m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nPort = m_nEditPort;

		CString strChannel;
		for (int i = 0; i < MAXLightChannelNum; i++)
		{
			m_stCurChannel[i].GetWindowText(strChannel);
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nChannelValue[i] = atoi(strChannel);
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_strName[i] = m_strEditName[i];//Summer0214
		}
	}
	else
	{
		m_bChkEnable = m_SystemOptionLightConfig.m_bLightCtrlUsed[nIndex];
		int nItemCount = m_cbbLightType.GetCount();
		for (int i = 0; i < nItemCount; i++)
		{
			if (m_cbbLightType.GetItemData(i) == m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nLightType)
			{
				m_cbbLightType.SetCurSel(i);
				break;
			}
		}

		m_cbbCommType.SetCurSel(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nCommType);

		m_cbbSerialPort.SetCurSel(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nSerialPort);

		CString strBaudRate;
		strBaudRate.Format(_T("%d"), m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nBaundRate);
		int nFind = m_cbbBaudRate.FindString(-1, strBaudRate);
		if (nFind != -1) m_cbbBaudRate.SetCurSel(nFind);

		m_ipAddressCtrl.SetAddress(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[0],
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[1],
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[2],
			m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_szIP[3]);

		m_nEditPort = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nPort;


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

		m_nMaxChannelSum = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nMaxChannels;

		UpdateData(FALSE);

	}

	UpdateShowStatus(nIndex);
}

// 保存数据
void CDlgSysOptionLightSetting::GetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig)
{
	UpdateDlgData(m_nRdLightControl, TRUE);
	systemOptionLightConfig = m_SystemOptionLightConfig;
}

void CDlgSysOptionLightSetting::SetSystemOptionLightConfig(SystemOptionLightConfig& systemOptionLightConfig)
{
	m_SystemOptionLightConfig = systemOptionLightConfig;
}

void CDlgSysOptionLightSetting::UpdateShowStatus(int nIndex)
{
	bool bEnable = m_SystemOptionLightConfig.m_bLightCtrlUsed[nIndex];
	int nMaxChannel = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nMaxChannels;

	bool bOpened = CLightController::GetInstance().IsOpen(nIndex);
	if (m_bEnableLightControl)
	{
		GetDlgItem(IDC_STATIC_GROUP_CONNECT_SETTING)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_STATIC_LIGHT_TYPE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_COMBO_LIGHT_TYPE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_STATIC_COMM_TYPE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_COMBO_COMM_TYPE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_STATIC_IP_ADDR)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(bEnable && !bOpened);//IP地址
		GetDlgItem(IDC_STATIC_PORT)->EnableWindow(bEnable && !bOpened);//端口号
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(bEnable && !bOpened);//端口号输入
		GetDlgItem(IDC_STATIC_SERIAL_PORT)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_COMBO_SERIAL_PORT)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_STATIC_BAUDRATE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(bEnable && !bOpened);
		GetDlgItem(IDC_EDIT_MAX_CHANNEL_SUM)->EnableWindow(bEnable && !bOpened);	

		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(bEnable);
		GetDlgItem(IDC_BTN_OPEN)->SetWindowText(bOpened?_T("关闭"):_T("打开"));

		GetDlgItem(IDC_STATIC_GROUP_CHANNEL_SETTING)->EnableWindow(bEnable && bOpened);
		GetDlgItem(IDC_BTN_OPEN_ALL)->EnableWindow(bEnable && bOpened);
		GetDlgItem(IDC_BTN_CLOSE_ALL)->EnableWindow(bEnable && bOpened);//Summer0214
		GetDlgItem(IDC_BTN_OPEN_CHANNEL1)->EnableWindow(bEnable && bOpened && nMaxChannel >= 1);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL2)->EnableWindow(bEnable && bOpened && nMaxChannel >= 2);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL3)->EnableWindow(bEnable && bOpened && nMaxChannel >= 3);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL4)->EnableWindow(bEnable && bOpened && nMaxChannel >= 4);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL5)->EnableWindow(bEnable && bOpened && nMaxChannel >= 5);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL6)->EnableWindow(bEnable && bOpened && nMaxChannel >= 6);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL7)->EnableWindow(bEnable && bOpened && nMaxChannel >= 7);
		GetDlgItem(IDC_BTN_OPEN_CHANNEL8)->EnableWindow(bEnable && bOpened && nMaxChannel >= 8);


		GetDlgItem(IDC_BTN_OPEN_CHANNEL1)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[0] ? "关闭-CH1" : "打开-CH1");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL2)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[1] ? "关闭-CH2" : "打开-CH2");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL3)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[2] ? "关闭-CH3" : "打开-CH3");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL4)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[3] ? "关闭-CH4" : "打开-CH4");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL5)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[4] ? "关闭-CH5" : "打开-CH5");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL6)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[5] ? "关闭-CH6" : "打开-CH6");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL7)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[6] ? "关闭-CH7" : "打开-CH7");
		GetDlgItem(IDC_BTN_OPEN_CHANNEL8)->SetWindowText(m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_bChannelIsOpen[7] ? "关闭-CH8" : "打开-CH8");


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

	///

	bool bSerialComm = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nCommType == 0 ? true : false;

	if (m_bShowLightControl)
	{
		GetDlgItem(IDC_STATIC_IP_ADDR)->ShowWindow((bSerialComm) ? SW_HIDE : SW_SHOW);
		GetDlgItem(IDC_IPADDRESS)->ShowWindow((bSerialComm) ? SW_HIDE : SW_SHOW);
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow((bSerialComm) ? SW_HIDE : SW_SHOW);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow((bSerialComm) ? SW_HIDE : SW_SHOW);

		GetDlgItem(IDC_STATIC_SERIAL_PORT)->ShowWindow((bSerialComm) ? SW_SHOW : SW_HIDE);
		GetDlgItem(IDC_COMBO_SERIAL_PORT)->ShowWindow((bSerialComm) ? SW_SHOW : SW_HIDE);
		GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow((bSerialComm) ? SW_SHOW : SW_HIDE);
		GetDlgItem(IDC_COMBO_BAUDRATE)->ShowWindow((bSerialComm) ? SW_SHOW : SW_HIDE);
	}

}

void CDlgSysOptionLightSetting::OnBnClickedRdLightControl1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPreSel = m_nRdLightControl;
	UpdateData(TRUE);
	if (nPreSel == m_nRdLightControl)
	{
		return;
	}
	UpdateDlgData(nPreSel, TRUE);
	UpdateDlgData(m_nRdLightControl, FALSE);
}


void CDlgSysOptionLightSetting::OnCbnSelchangeComboLightType()
{
	// TODO: 在此添加控件通知处理程序代码
	///
	int nCurSel = m_cbbLightType.GetCurSel();
	if (nCurSel == -1) return;

	int nLightType = m_cbbLightType.GetItemData(nCurSel);

	int nMinIntensity = 0;
	int nMaxIntensity = 0;
	CLightController::GetIntensityRange((LightControlType)nLightType, nMinIntensity, nMaxIntensity);

	CString strMin;
	CString strMax;

	strMin.Format(_T("%d"), nMinIntensity);
	strMax.Format(_T("%d"), nMaxIntensity);
	CString strTmp;
	for (int i = 0; i < MAXLightChannelNum; i++)
	{
		m_stMinChannel[i].SetWindowText(strMin);
		m_stMaxChannel[i].SetWindowText(strMax);
		m_sliderChannel[i].SetRange(nMinIntensity, nMaxIntensity);
		////Summer0228 根据进度条修改当前值
		//nPos = m_sliderChannel[i].GetPos();
		//strTmp.Format("%d",nPos);
		//m_stCurChannel[i].SetWindowText(strTmp);
		////根据当前控件值  调整getpos窗体

	}
	//UpdateData(FALSE);
}


void CDlgSysOptionLightSetting::OnCbnSelchangeComboCommType()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = m_cbbCommType.GetCurSel();
	if (nCurSel == -1) return;

	bool bSerialComm = nCurSel == 0 ? true : false;

	GetDlgItem(IDC_STATIC_IP_ADDR)->ShowWindow(bSerialComm ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_IPADDRESS)->ShowWindow(bSerialComm ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_STATIC_PORT)->ShowWindow(bSerialComm ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_EDIT_PORT)->ShowWindow(bSerialComm ? SW_HIDE : SW_SHOW);

	GetDlgItem(IDC_STATIC_SERIAL_PORT)->ShowWindow(bSerialComm ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COMBO_SERIAL_PORT)->ShowWindow(bSerialComm ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow(bSerialComm ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_COMBO_BAUDRATE)->ShowWindow(bSerialComm ? SW_SHOW : SW_HIDE);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bOpened = CLightController::GetInstance().IsOpen(m_nRdLightControl);
	if (bOpened)
	{
		CLightController::GetInstance().CloseConnect(m_nRdLightControl);
		GetDlgItem(IDC_BTN_OPEN)->SetWindowText(_T("打开"));
		UpdateShowStatus(m_nRdLightControl);
	}
	else
	{
		UpdateDlgData(m_nRdLightControl, TRUE);
		//////////////////////////////////////////////////////////////////////////
		//Summer0228 打开光源控制器之前，需要先进行一下检查    防止光源控制器被多个连接
		for (int i = 0; i < MAXLightControl; i++)
		{
			if (m_nRdLightControl == i)
			{
				continue;
			}

			//确认将要打开的光源控制器 与 其他要验证的光源控制器类型一致    在进行验证
			int nTmpCommType = m_cbbCommType.GetCurSel();
			if (nTmpCommType == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_nCommType)
			{
				if (CLightController::GetInstance().IsOpen(i))
				{
					//已经打开的    则与当前将要打开的进行对比
					if (nTmpCommType == 1)//网口
					{
						BYTE feild0 = 0, feild1 = 0, feild2 = 0, feild3 = 0;
						m_ipAddressCtrl.GetAddress(feild0, feild1, feild2, feild3);
						if (feild0 == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_szIP[0]
						||feild1 == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_szIP[1]
						||feild2 == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_szIP[2]
						||feild3 == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_szIP[3]
						)
						{
							if (m_nEditPort == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_nPort)
							{
								AfxMessageBox(_T("打开失败，端口号重复！"));
								return ;
							}
							continue;
						}
					}
					else//串口
					{
						int nTmpSer = m_cbbSerialPort.GetCurSel();
						if (nTmpSer == m_SystemOptionLightConfig.m_lightCtrlParam[i].m_nSerialPort)
						{
							AfxMessageBox(_T("打开失败，串口号重复！"));
							return ;
						}
						continue;
					}

				}
			}



		}
		//////////////////////////////////////////////////////////////////////////
		bool bRet = CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl]);
		if (bRet)
		{
			GetDlgItem(IDC_BTN_OPEN)->SetWindowText(_T("关闭"));
			UpdateShowStatus(m_nRdLightControl);
		}
		else
		{
			MessageBox(_T("打开光源控制器失败, 请检测光源控制器是否正确安装!"));
		}
	}
}


void CDlgSysOptionLightSetting::OnBnClickedChkEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_SystemOptionLightConfig.m_bLightCtrlUsed[m_nRdLightControl] = m_bChkEnable;
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 0, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 0, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[0] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL2:
		{
			int nPos = m_sliderChannel[1].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[1].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 1, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 1, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[1] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL3:
		{
			int nPos = m_sliderChannel[2].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[2].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 2, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 2, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[2] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL4:
		{
			int nPos = m_sliderChannel[3].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[3].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 3, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 3, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[3] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL5:
		{
			int nPos = m_sliderChannel[4].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[4].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 4, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 4, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[4] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL6:
		{
			int nPos = m_sliderChannel[5].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[5].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 5, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 5, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[5] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL7:
		{
			int nPos = m_sliderChannel[6].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[6].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 6, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 6, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[6] = nPos;
			}
		}
		return;
	case IDC_SLIDER_CHANNEL8:
		{
			int nPos = m_sliderChannel[7].GetPos();
			CString strCur;
			strCur.Format("%d", nPos);
			m_stCurChannel[7].SetWindowText(strCur);
			//CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 7, nPos);
			if (CLightController::GetInstance().SetChannelIntensityValue(m_nRdLightControl, 7, nPos))
			{
				m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nChannelValue[7] = nPos;
			}
		}
		return;
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenAll()
{
	// TODO: 在此添加控件通知处理程序代码
	int nMaxChannel = m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nMaxChannels;
	for (int i = 0; i < nMaxChannel; i++)
	{
		m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[i] = TRUE;
	}

	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl]);

	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[0] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[0];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],1);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[1] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[1];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],2);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[2] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[2];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],3);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[3] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[3];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],4);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[4] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[4];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],5);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[5] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[5];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],6);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel7()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[6] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[6];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],7);//Summer0216
	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnBnClickedBtnOpenChannel8()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[7] = !m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[7];
	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],8);//Summer0216;
	UpdateShowStatus(m_nRdLightControl);
}

void CDlgSysOptionLightSetting::UpdateDlgByAuthorize()
{
	// 系统选项仅仅通过第一工位的权限管理进行设置
	if (NULL !=m_pParent && m_pParent->m_vpVisionASM.size()>0 && m_pParent->m_vpVisionASM.at(0))
	{
		CVisionASMConfig *vsConfig;
		m_pParent->m_vpVisionASM.at(0)->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			//bool bShow = false;
			//bool bEnable = false;
			CString strNodeName = _T("");
			// 光源控制
			//bool bFind = vsConfig->GetCheckedByItem(eOptionAuthorize_LightControl,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bShowLightControl,m_bEnableLightControl);
			if (1/*bFind*/)
			{
				GetDlgItem(IDC_RD_LIGHT_CONTROL1)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL1)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL2)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL2)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL3)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL3)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL4)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL4)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL5)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL5)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL6)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL6)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL7)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL7)->ShowWindow(m_bShowLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL8)->EnableWindow(m_bEnableLightControl);
				GetDlgItem(IDC_RD_LIGHT_CONTROL8)->ShowWindow(m_bShowLightControl);
				SetGroupBoxStatus(IDC_STATIC_GROUP_CONNECT_SETTING,m_bEnableLightControl,m_bShowLightControl);
				SetGroupBoxStatus(IDC_STATIC_GROUP_CHANNEL_SETTING,m_bEnableLightControl,m_bShowLightControl);

				//if(pWnd->GetDlgCtrlID() == IDC_BUTTON_TRIGGER_MODE_SET)
				//{
				//	bool bEnable = m_SystemOptionLightConfig.m_bLightCtrlUsed[0];
				//	int nMaxChannel = m_SystemOptionLightConfig.m_lightCtrlParam[0].m_nMaxChannels;
				//	bool bOpened = CLightController::GetInstance().IsOpen(0);
				//	GetDlgItem(IDC_STATIC_IP_ADDR)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);//IP地址
				//	GetDlgItem(IDC_IPADDRESS)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);//IP地址输入
				//	GetDlgItem(IDC_STATIC_PORT)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);//端口号
				//	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);//端口号输入
				//	GetDlgItem(IDC_STATIC_SERIAL_PORT)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);
				//	GetDlgItem(IDC_COMBO_SERIAL_PORT)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);
				//	GetDlgItem(IDC_STATIC_BAUDRATE)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);
				//	GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(bEnable && !bOpened && m_bEnableLightControl);
				//
				//	bool bSerialComm = m_SystemOptionLightConfig.m_lightCtrlParam[nIndex].m_nCommType == 0 ? true : false;

				//	GetDlgItem(IDC_STATIC_IP_ADDR)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_HIDE : SW_SHOW);
				//	GetDlgItem(IDC_IPADDRESS)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_HIDE : SW_SHOW);
				//	GetDlgItem(IDC_STATIC_PORT)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_HIDE : SW_SHOW);
				//	GetDlgItem(IDC_EDIT_PORT)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_HIDE : SW_SHOW);

				//	GetDlgItem(IDC_STATIC_SERIAL_PORT)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_SHOW : SW_HIDE);
				//	GetDlgItem(IDC_COMBO_SERIAL_PORT)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_SHOW : SW_HIDE);
				//	GetDlgItem(IDC_STATIC_BAUDRATE)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_SHOW : SW_HIDE);
				//	GetDlgItem(IDC_COMBO_BAUDRATE)->ShowWindow((bSerialComm && m_bShowLightControl) ? SW_SHOW : SW_HIDE);
				//}



			}

		}
	}
}

void CDlgSysOptionLightSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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
			pWnd->EnableWindow(bEnable);
			pWnd->ShowWindow(nShow);
		}
		pWnd = pWnd->GetNextWindow();
	}
}


void CDlgSysOptionLightSetting::OnBnClickedBtnCloseAll()//Summer0214
{
	// TODO: 在此添加控件通知处理程序代码
	int nMaxChannel = m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_nMaxChannels;
	for (int i = 0; i < nMaxChannel; i++)
	{
		m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl].m_bChannelIsOpen[i] = FALSE;
	}

	CLightController::GetInstance().OpenConnect(m_nRdLightControl, true, m_SystemOptionLightConfig.m_lightCtrlParam[m_nRdLightControl],9);//Summer0216

	UpdateShowStatus(m_nRdLightControl);
}


void CDlgSysOptionLightSetting::OnEnSetfocusEditPort()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_PORT)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_PORT, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_PORT, strOutput);
			if (strInput != strOutput)
			{
				CString strLog;
				strLog.Format(_T("修改[光源控制器%d][通道1名称]由%s到%s\n"),m_nRdLightControl,strInput,strOutput);
				g_opLogFile.SaveLogText(strLog,2);
			}
		}
	}	
}

void CDlgSysOptionLightSetting::OnEnKillfocusEditPort()
{
	// TODO: 在此添加控件通知处理程序代码
	int nTmp;
	nTmp = m_nEditPort;
	//GetDlgItemText(IDC_EDIT_PORT, strTmp);
	UpdateData(TRUE);
	if (m_nEditPort < 0)
	{
		m_nEditPort = nTmp;
	}
	UpdateData(FALSE);
}

void CDlgSysOptionLightSetting::OnSetfocusIpaddress() 
{
	if (m_bSysKeyboardEnabled)
	{
		unsigned char  m_byAddr[4];
		m_byAddr[0] = 0;
		m_byAddr[1] = 0; 
		m_byAddr[2] = 0; 
		m_byAddr[3] = 0;
		m_ipAddressCtrl.GetAddress(m_byAddr[0], m_byAddr[1], m_byAddr[2], m_byAddr[3]);

		CString strLog,strPre,strCur;
		strPre.Format(_T("%d %d %d %d"), m_byAddr[0],m_byAddr[1],m_byAddr[2],m_byAddr[3]);
		CRect rcWnd;
		CString strInput, strOutput;

		GetDlgItem(IDC_IPADDRESS)->GetWindowRect(&rcWnd);
		unsigned char byAddr = 0;
		int i = 0;
		for(i = 0; i < 4; i++)
		{
			strInput.Format("%d", m_byAddr[i]);
			if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
			{
				byAddr = _tcstoul(strOutput, NULL, 10);
				if (byAddr >=0 || byAddr <= 255)
				{
					m_byAddr[i] = byAddr;
				}
				m_ipAddressCtrl.SetAddress(m_byAddr[0], m_byAddr[1], m_byAddr[2], m_byAddr[3]);
			}
			else
			{
				return;
			}
		}

		strCur.Format(_T("%d %d %d %d"), m_byAddr[0],m_byAddr[1],m_byAddr[2],m_byAddr[3]);
		if (strPre != strCur)
		{
			CString strLog;
			strLog.Format(_T("修改[光源控制器%d][IP]%s到%s"),m_nRdLightControl,strPre,strCur);
			g_opLogFile.SaveLogText(strLog,3);
		}
	}
}



void CDlgSysOptionLightSetting::OnBnClickedButtonLightCtrlRename()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgLightCtrlRename dlg;
	dlg.SetSystemOptionLightName(m_SystemOptionLightConfig);
	if (dlg.DoModal()==IDOK)
	{
		dlg.GetSystemOptionLightName(m_SystemOptionLightConfig);

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
}


void CDlgSysOptionLightSetting::OnEnSetfocusEditMaxChannelSum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bSysKeyboardEnabled)
	{
		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MAX_CHANNEL_SUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MAX_CHANNEL_SUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotKeyBoard(rcWnd, strInput, strOutput))
		{
			if (atoi(strOutput) > 0 && atoi(strOutput)<= MAXLightChannelNum)
			{
				SetDlgItemText(IDC_EDIT_MAX_CHANNEL_SUM, strOutput);
			}	
		}
	}
}


void CDlgSysOptionLightSetting::OnEnKillfocusEditMaxChannelSum()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strInput;
	GetDlgItemText(IDC_EDIT_MAX_CHANNEL_SUM, strInput);
	if (atoi(strInput) < 1 || atoi(strInput) > MAXLightChannelNum)
	{
		SetDlgItemText(IDC_EDIT_MAX_CHANNEL_SUM, _T("8"));
	}	
}
