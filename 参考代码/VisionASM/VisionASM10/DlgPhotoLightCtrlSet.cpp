// DlgPhotoLightCtrlSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPhotoLightCtrlSet.h"
#include "afxdialogex.h"
#include "SystemOptionConfig.h"


// CDlgPhotoLightCtrlSet 对话框

IMPLEMENT_DYNAMIC(CDlgPhotoLightCtrlSet, CDialogEx)

CDlgPhotoLightCtrlSet::CDlgPhotoLightCtrlSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPhotoLightCtrlSet::IDD, pParent)
	, m_nCMDLightIndex(0)
	, m_nCMDLightCtrlIndex(0)
	, m_bEnableChannel1(FALSE)
	, m_bEnableChannel2(FALSE)
	, m_bEnableChannel3(FALSE)
	, m_bEnableChannel4(FALSE)
	, m_bEnableChannel5(FALSE)
	, m_bEnableChannel6(FALSE)
	, m_bEnableChannel7(FALSE)
	, m_bEnableChannel8(FALSE)
	, m_bChannelState1(FALSE)
	, m_bChannelState2(FALSE)
	, m_bChannelState3(FALSE)
	, m_bChannelState4(FALSE)
	, m_bChannelState5(FALSE)
	, m_bChannelState6(FALSE)
	, m_bChannelState7(FALSE)
	, m_bChannelState8(FALSE)
{
	m_pSystempOptionConfig = NULL;
}

CDlgPhotoLightCtrlSet::~CDlgPhotoLightCtrlSet()
{
}

void CDlgPhotoLightCtrlSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CMD_LIGHT_INDEX, m_nCMDLightIndex);
	DDX_CBIndex(pDX, IDC_COMBO_CMD_LIGHT_CTRL_INDEX, m_nCMDLightCtrlIndex);
	DDX_Control(pDX, IDC_COMBO_CMD_LIGHT_INDEX, m_CombCMDLightIndex);
	DDX_Control(pDX, IDC_COMBO_CMD_LIGHT_CTRL_INDEX, m_CombCMDLightCtrlIndex);
	DDX_Control(pDX, IDC_CHECK_ENABLE_CUR_LIGHTCTRL, m_BtnEnableCurLightCtrl);
	DDX_Check(pDX, IDC_CHECK_CHANNEL1, m_bEnableChannel1);
	DDX_Check(pDX, IDC_CHECK_CHANNEL2, m_bEnableChannel2);
	DDX_Check(pDX, IDC_CHECK_CHANNEL3, m_bEnableChannel3);
	DDX_Check(pDX, IDC_CHECK_CHANNEL4, m_bEnableChannel4);
	DDX_Check(pDX, IDC_CHECK_CHANNEL5, m_bEnableChannel5);
	DDX_Check(pDX, IDC_CHECK_CHANNEL6, m_bEnableChannel6);
	DDX_Check(pDX, IDC_CHECK_CHANNEL7, m_bEnableChannel7);
	DDX_Check(pDX, IDC_CHECK_CHANNEL8, m_bEnableChannel8);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE1, m_bChannelState1);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE2, m_bChannelState2);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE3, m_bChannelState3);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE4, m_bChannelState4);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE5, m_bChannelState5);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE6, m_bChannelState6);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE7, m_bChannelState7);
	DDX_Check(pDX, IDC_CHECK_CHANNELSTATE8, m_bChannelState8);
}


BEGIN_MESSAGE_MAP(CDlgPhotoLightCtrlSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CMD_LIGHT_INDEX, &CDlgPhotoLightCtrlSet::OnCbnSelchangeComboCmdLightIndex)
	ON_CBN_SELCHANGE(IDC_COMBO_CMD_LIGHT_CTRL_INDEX, &CDlgPhotoLightCtrlSet::OnCbnSelchangeComboCmdLightCtrlIndex)
	ON_BN_CLICKED(IDOK, &CDlgPhotoLightCtrlSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPhotoLightCtrlSet 消息处理程序


void CDlgPhotoLightCtrlSet::OnCbnSelchangeComboCmdLightIndex()
{
	// TODO: 在此添加控件通知处理程序代码

	//1、先保存前一个流程信息
	UpdateDlgData(m_nCMDLightIndex,m_nCMDLightCtrlIndex,TRUE);

	//2、更新当前流程信息
	UpdateDlgData(m_nCMDLightIndex,0,FALSE);
}


void CDlgPhotoLightCtrlSet::OnCbnSelchangeComboCmdLightCtrlIndex()
{
	// TODO: 在此添加控件通知处理程序代码
	//1、先保存前一个控制器信息
	UpdateDlgData(m_nCMDLightIndex,m_nCMDLightCtrlIndex,TRUE);

	//2、更新当前控制器信息
	UpdateDlgData(m_nCMDLightIndex,m_nCMDLightCtrlIndex,FALSE);
}

BOOL CDlgPhotoLightCtrlSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	{
		
		//FAAC、FAAL、FALP、FTGC、FCHB、FBIS
		for (int i=0;i < m_cPhotoLightCtrl.m_nCmdSum;++i)
		{
			m_CombCMDLightIndex.AddString(m_cPhotoLightCtrl.m_vCmd.at(i));
		}
		m_CombCMDLightIndex.SetCurSel(0);//显示选中流程1

		CString str;
		if (m_pSystempOptionConfig)
		{
			for (int i=0;i<8;++i)
			{
				str = m_pSystempOptionConfig->m_sysOptLightConfig.m_strLightCtrlName[i];
				m_CombCMDLightCtrlIndex.AddString(str);
			}
		}
		else
		{
			for (int i=0;i<8;++i)
			{
				str.Format(_T("控制器%d"),i+1);
				m_CombCMDLightCtrlIndex.AddString(str);
			}
		}
		m_CombCMDLightCtrlIndex.SetCurSel(0);

		UpdateDlgData(0,0,FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPhotoLightCtrlSet::UpdateDlgData(int CMDLightIndex,int nCMDLightCtrlIndex, BOOL bSaveAndValidate /*= TRUE*/)
{
	if (bSaveAndValidate) //从界面获取
	{
		UpdateData(TRUE);

		CurCMDPhotoLightCtrlSet cObj = m_cPhotoLightCtrl.m_vCmdLightCtrlInfo[CMDLightIndex];
		std::vector<BOOL> vChannelCtrlIndex;
		std::vector<BOOL> vChannelOnorOff;

		 //每个控制器8个通道的控制情况
		{
			vChannelCtrlIndex.push_back(m_bEnableChannel1);
			vChannelCtrlIndex.push_back(m_bEnableChannel2);
			vChannelCtrlIndex.push_back(m_bEnableChannel3);
			vChannelCtrlIndex.push_back(m_bEnableChannel4);
			vChannelCtrlIndex.push_back(m_bEnableChannel5);
			vChannelCtrlIndex.push_back(m_bEnableChannel6);
			vChannelCtrlIndex.push_back(m_bEnableChannel7);
			vChannelCtrlIndex.push_back(m_bEnableChannel8);

			vChannelOnorOff.push_back(m_bChannelState1);
			vChannelOnorOff.push_back(m_bChannelState2);
			vChannelOnorOff.push_back(m_bChannelState3);
			vChannelOnorOff.push_back(m_bChannelState4);
			vChannelOnorOff.push_back(m_bChannelState5);
			vChannelOnorOff.push_back(m_bChannelState6);
			vChannelOnorOff.push_back(m_bChannelState7);
			vChannelOnorOff.push_back(m_bChannelState8);
		}

		BOOL bEnableCurCtrl = m_BtnEnableCurLightCtrl.GetCheck();
		cObj.m_vCmdLightCtrlEnable[nCMDLightCtrlIndex] = bEnableCurCtrl;
		cObj.m_vbChannelCtrlIndex[nCMDLightCtrlIndex] = vChannelCtrlIndex;
		cObj.m_vbChannelOnorOff[nCMDLightCtrlIndex] = vChannelOnorOff;
		m_cPhotoLightCtrl.m_vCmdLightCtrlInfo[CMDLightIndex] = cObj;

	} 
	else //更新到界面
	{
		CurCMDPhotoLightCtrlSet cObj = m_cPhotoLightCtrl.m_vCmdLightCtrlInfo[CMDLightIndex];
		m_BtnEnableCurLightCtrl.SetCheck(cObj.m_vCmdLightCtrlEnable[nCMDLightCtrlIndex]);
		std::vector<BOOL> vChannelCtrlIndex = cObj.m_vbChannelCtrlIndex[nCMDLightCtrlIndex];
		std::vector<BOOL> vChannelOnorOff	= cObj.m_vbChannelOnorOff[nCMDLightCtrlIndex];

		//每个控制器8个通道的控制情况
		{
			m_bEnableChannel1 = vChannelCtrlIndex.at(0);
			m_bEnableChannel2 = vChannelCtrlIndex.at(1);
			m_bEnableChannel3 = vChannelCtrlIndex.at(2);
			m_bEnableChannel4 = vChannelCtrlIndex.at(3);
			m_bEnableChannel5 = vChannelCtrlIndex.at(4);
			m_bEnableChannel6 = vChannelCtrlIndex.at(5);
			m_bEnableChannel7 = vChannelCtrlIndex.at(6);
			m_bEnableChannel8 = vChannelCtrlIndex.at(7);

			m_bChannelState1 = vChannelOnorOff.at(0);
			m_bChannelState2 = vChannelOnorOff.at(1);
			m_bChannelState3 = vChannelOnorOff.at(2);
			m_bChannelState4 = vChannelOnorOff.at(3);
			m_bChannelState5 = vChannelOnorOff.at(4);
			m_bChannelState6 = vChannelOnorOff.at(5);
			m_bChannelState7 = vChannelOnorOff.at(6);
			m_bChannelState8 = vChannelOnorOff.at(7);
		}

		m_nCMDLightIndex = CMDLightIndex;
		m_nCMDLightCtrlIndex = nCMDLightCtrlIndex;

		UpdateData(FALSE);

	}
}

void CDlgPhotoLightCtrlSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();

	UpdateDlgData(m_nCMDLightIndex,m_nCMDLightCtrlIndex,TRUE);
}
