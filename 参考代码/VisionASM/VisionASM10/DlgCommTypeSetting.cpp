// DlgCommTypeSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCommTypeSetting.h"
#include "afxdialogex.h"
#include "DlgSysCommSetting.h"
#include "DlgSysEnvironSetting.h"

// CDlgCommTypeSetting 对话框

IMPLEMENT_DYNAMIC(CDlgCommTypeSetting, CDialogEx)

CDlgCommTypeSetting::CDlgCommTypeSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommTypeSetting::IDD, pParent)
	, m_nCommType(0)
	, m_bRegiterType(FALSE)
	, m_nRegDataTypeIndex(0)
	, m_nCmdParamOrderType(0)
	, m_bUseSpecialComm(FALSE)
{
	strArrayProtocolTypeVal.clear();
	m_bshowCommTypeSet = true;
	m_bEnableCommTypeSet = true;

	m_bshowCommOtherSet = true;
	m_bEnableCommOtherSet = true;
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdFast, _T("标准通用无协议Fast")));													//eProCmdFast							= 0,				// 通用指令Fast通用无协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdVS, _T("标准通用无协议Vs")));														//eProCmdVS							= 1,				// 通用指令VisionSmart通用无协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdYamaha, _T("雅马哈机器人无协议")));												//eProCmdYamaha						= 2,				// 通用指令雅马哈机器人无协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdEpson, _T("通用无协议预留1")));													//eProCmdEpson						= 3,				// 通用指令通用无协议预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdReserved2, _T("通用无协议预留2")));										        //eProCmdReserved2					= 4,				// 通用指令通用无协议预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdReserved3, _T("通用无协议预留3")));										        //eProCmdReserved3					= 5,				// 通用指令通用无协议预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegBase, _T("PLC链接")));															//eProRegBase							= 10,				// PLC链接，大于该枚举值的都是PLC链接协议，直接读取存储PLC寄存器
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSerialAscii, _T("Modbus寄存器串口ASCII协议")));				//eProRegModbusSerialAscii			= 11,				// Modbus的寄存器串口ASCII协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSerialRtu, _T("Modbus寄存器串口RTU协议")));				//eProRegModbusSerialRtu				= 12,				// Modbus的寄存器串口RTU协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSocketTcp, _T("Modbus寄存器网口TCP协议")));			//eProRegModbusSocketTcp				= 13,				// Modbus的寄存器网口TCP协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusReserved1, _T("Modbus的寄存器预留1")));			//eProRegModbusReserved1				= 14,				// Modbus的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusReserved2, _T("Modbus的寄存器预留2")));			//eProRegModbusReserved2				= 15,				// Modbus的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicSerialMewtocol, _T("松下FP系列PLC的寄存器串口Mewtocol协议")));					//eProRegPanasonicSerialMewtocol		= 21,				// 松下FP系列PLC的寄存器串口Mewtocol协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicSocketMewtocol, _T("松下FP系列PLC的寄存器网口Mewtocol协议")));		//eProRegPanasonicSocketMewtocol		= 22,				// 松下FP系列PLC的寄存器网口Mewtocol协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved1, _T("松下FP系列PLC的寄存器预留1")));				//eProRegPanasonicReserved1			= 23,				// 松下FP系列PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved2, _T("松下FP系列PLC的寄存器预留2")));			//eProRegPanasonicReserved2			= 24,				// 松下FP系列PLC的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved3, _T("松下FP系列PLC的寄存器预留3")));			//eProRegPanasonicReserved3			= 25,				// 松下FP系列PLC的寄存器预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSerialAscii, _T("三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议")));				//eProRegMitsubishiQSerialAscii		= 31,				// 三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSerialBinary, _T("三菱Q系列PLC的寄存器串口MC(格式5)二进制协议")));		//eProRegMitsubishiQSerialBinary		= 32,				// 三菱Q系列PLC的寄存器串口MC(格式5)二进制协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketAscii, _T("三菱Q系列PLC的寄存器网口MC（E3）ASCII协议")));		//eProRegMitsubishiQSocketAscii		= 33,				// 三菱Q系列PLC的寄存器网口MC（E3）ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketBinary, _T("三菱Q系列PLC的寄存器网口MC（E3）二进制协议")));				//eProRegMitsubishiQSocketBinary		= 34,				// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketAsciiUDP, _T("三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议")));		//eProRegMitsubishiQSocketAsciiUDP		= 35,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketBinaryUDP, _T("三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议")));				//eProRegMitsubishiQSocketBinaryUDP		= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXSerialAscii, _T("三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议")));				//eProRegMitsubishiFXSerialAscii		= 41,				// 三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXSocketBinaryMC1E, _T("三菱FX系列PLC的寄存器网口MC（A兼容1E帧）二进制协议(FX-3U-ENET-ADP网口模块使用)")));			//eProRegMitsubishiFXReserved1		= 42,				// 三菱FX系列PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved2, _T("三菱FX系列PLC的寄存器预留2")));			//eProRegMitsubishiFXReserved2		= 43,				// 三菱FX系列PLC的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved3, _T("三菱FX系列PLC的寄存器预留3")));			//eProRegMitsubishiFXReserved3		= 44,				// 三菱FX系列PLC的寄存器预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved4, _T("三菱FX系列PLC的寄存器预留4")));					//eProRegMitsubishiFXReserved4		= 45,				// 三菱FX系列PLC的寄存器预留4
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaSocketAscii, _T("横河PLC的寄存器网口ASCII协议")));		//eProRegYokogawaSocketAscii			= 51,				// 横河PLC的寄存器网口ASCII协议（PC-Link(以太网ASCII码)，一般用这种）
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaSocketBinary, _T("横河PLC的寄存器网口二进制协议")));				//eProRegYokogawaSocketBinary			= 52,				// 横河PLC的寄存器网口Binary协议（PC-Link(以太网二进制)）
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaPCLinkAscii, _T("横河PLC的寄存器网口PC-Link(ASCII码)")));			//eProRegYokogawaPCLinkAscii			= 53,				// 横河PLC的寄存器网口PC-Link(ASCII码)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaReserved1, _T("横河PLC的寄存器预留1")));			//eProRegYokogawaReserved1			= 54,				// 横河PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaReserved2, _T("横河PLC的寄存器预留2")));				//eProRegYokogawaReserved2			= 55,				// 横河PLC的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSerialAscii, _T("基恩士KV系列PLC的寄存器串口ASCII协议")));		//eProRegKeyenceSerialAscii			= 61,				// 基恩士KV系列PLC的寄存器串口ASCII协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSerialPCLink, _T("基恩士KV系列PLC的寄存器串口上位链路协议")));		//eProRegKeyenceSerialPCLink			= 62,				// 基恩士KV系列PLC的寄存器串口上位链路协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketAscii, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)")));				//eProRegKeyenceSocketAscii			= 63,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketBinary, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)")));				//eProRegKeyenceSocketBinary			= 64,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketPCLink, _T("基恩士KV系列PLC的寄存器网口上位链路协议")));				//eProRegKeyenceSocketPCLink			= 65,				// 基恩士KV系列PLC的寄存器网口上位链路协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceReserved1, _T("基恩士PLC的寄存器预留1")));			//eProRegKeyenceReserved1				= 66,				// 基恩士PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYaskawa, _T("安川PLC的寄存器协议1")));			//eProRegYaskawa						= 71,				// 安川PLC的寄存器协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved1, _T("PLC的寄存器操作协议预留1")));			//eProRegReserved1					= 81,				// PLC的寄存器操作协议预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved2, _T("PLC的寄存器操作协议预留2")));					//eProRegReserved2					= 82,				// PLC的寄存器操作协议预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved3, _T("PLC的寄存器操作协议预留3")));		//eProRegReserved3					= 83,				// PLC的寄存器操作协议预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved4, _T("PLC的寄存器操作协议预留4")));				//eProRegReserved4					= 84,				// PLC的寄存器操作协议预留4
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved5, _T("PLC的寄存器操作协议预留5")));			//eProRegReserved5					= 85,				// PLC的寄存器操作协议预留5
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegFujiSPH, _T("富士MICREX-SX SPH系列 PLC的寄存器协议")));			//eProRegFujiSPH                      = 91,               // 富士MICREX-SX SPH系列 PLC的寄存器协议                                         
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegOmronSerialHostLink, _T("欧姆龙PLC的寄存器串口HostLink协议")));	//		eProRegOmronSerialHostLink = 101, _T("欧姆龙PLC的寄存器串口HostLink协议")                                         
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegOmronSocketFins, _T("欧姆龙PLC的寄存器网口Fins协议")));			//eProRegOmronSocketFins = 102, _T("欧姆龙PLC的寄存器网口Fins协议")                                         
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSerialAscii, _T("Delta(台达)Modbus寄存器串口ASCII协议")));	// Delta(台达)Modbus的寄存器串口ASCII协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSerialRtu, _T("Delta(台达)Modbus寄存器串口RTU协议")));		// Delta(台达)Modbus的寄存器串口RTU协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSocketTcp, _T("Delta(台达)Modbus寄存器网口TCP协议")));		// Delta(台达)Modbus的寄存器网口TCP协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusReserved1, _T("Delta(台达)Modbus的寄存器预留1")));			// Delta(台达)Modbus的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusReserved2, _T("Delta(台达)Modbus的寄存器预留2")));			// Delta(台达)Modbus的寄存器预留2

}

CDlgCommTypeSetting::~CDlgCommTypeSetting()
{
}

void CDlgCommTypeSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RAD_SERIAL_TYPE, m_nCommType);
	DDX_Radio(pDX, IDC_RADIO_SINGLE_WORD, m_bRegiterType);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE, m_ComboProtoccolType);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE2, m_ComboExProtoccolType);
	DDX_Radio(pDX, IDC_RADIO_DM_TYPE, m_nRegDataTypeIndex);
	//DDX_Radio(pDX, IDC_RADIO_SEND_AXIS_ORDER_XYD, m_nCmdParamOrderType);
	DDX_Control(pDX, IDC_COMBO_MOTION_CONTROL_TYPE, m_ComboMotionControlType);
	DDX_Check(pDX, IDC_CHECK_USE_SPECIAL_COMM, m_bUseSpecialComm);
	DDX_Control(pDX, IDC_COMBO_SEND_AXIS_ORDER, m_ComboOrderType);
}


BEGIN_MESSAGE_MAP(CDlgCommTypeSetting, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL_TYPE, &CDlgCommTypeSetting::OnCbnSelchangeComboProtocolType)
	ON_BN_CLICKED(IDC_RAD_SERIAL_TYPE, &CDlgCommTypeSetting::OnBnClickedRadSerialType)
	ON_BN_CLICKED(IDC_RAD_SOCKET_TYPE, &CDlgCommTypeSetting::OnBnClickedRadSocketType)
END_MESSAGE_MAP()


// CDlgCommTypeSetting 消息处理程序

CCommParamInfo CDlgCommTypeSetting::GetCCommParamInfo() const
{
	return m_commParamInfo;
}

void CDlgCommTypeSetting::SetCCommParamInfo(const CCommParamInfo &commParamInfo)
{
	m_commParamInfo = commParamInfo;
}

void CDlgCommTypeSetting::UpdateDialogLanguage()
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

		if(_T("IDD_SYS_COMM_PARAM_SETTING") == strDlgID)
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

void CDlgCommTypeSetting::InitDlgItem()
{

	strArrayProtocolTypeVal.push_back(make_pair(eProCmdFast, _T("标准通用无协议Fast")));													//eProCmdFast							= 0,				// 通用指令Fast通用无协议
	
	// 剔除协议不常用
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdVS, _T("标准通用无协议Vs")));														//eProCmdVS							= 1,				// 通用指令VisionSmart通用无协议
	strArrayProtocolTypeVal.push_back(make_pair(eProCmdYamaha, _T("雅马哈机器人无协议")));												//eProCmdYamaha						= 2,				// 通用指令雅马哈机器人无协议
	strArrayProtocolTypeVal.push_back(make_pair(eProCmdEpson, _T("Epson无协议")));													//eProCmdEpson						= 3,				// 通用指令通用无协议预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdReserved2, _T("通用无协议预留2")));										        //eProCmdReserved2					= 4,				// 通用指令通用无协议预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProCmdReserved3, _T("通用无协议预留3")));										        //eProCmdReserved3					= 5,				// 通用指令通用无协议预留3

	if (NULL != m_pVisionASM)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

		if (sysPlatformInfo.m_bShareCommEnable==TRUE)
		{
			// 共享通信时 不支持无协议；只能为读写寄存器协议
			strArrayProtocolTypeVal.clear();
		}
		// 如果该工位为共享通信则 不可编辑
		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			ShowShareCommControl(FALSE);
		}

	}

	// 剔除协议不常用													
	strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSerialAscii, _T("Modbus串口ASCII协议")));				//eProRegModbusSerialAscii			= 11,				// Modbus的寄存器串口ASCII协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSerialRtu, _T("Modbus串口RTU协议")));				//eProRegModbusSerialRtu				= 12,				// Modbus的寄存器串口RTU协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusSocketTcp, _T("Modbus网口TCP协议")));			//eProRegModbusSocketTcp				= 13,				// Modbus的寄存器网口TCP协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusReserved1, _T("Modbus的寄存器预留1")));			//eProRegModbusReserved1				= 14,				// Modbus的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegModbusReserved2, _T("Modbus的寄存器预留2")));			//eProRegModbusReserved2				= 15,				// Modbus的寄存器预留2
	strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicSerialMewtocol, _T("松下FP串口Mewtocol协议")));					//eProRegPanasonicSerialMewtocol		= 21,				// 松下FP系列PLC的寄存器串口Mewtocol协议(一般使用这种)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicSocketMewtocol, _T("松下FP网口Mewtocol协议")));		//eProRegPanasonicSocketMewtocol		= 22,				// 松下FP系列PLC的寄存器网口Mewtocol协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved1, _T("松下FP预留1")));				//eProRegPanasonicReserved1			= 23,				// 松下FP系列PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved2, _T("松下FP预留2")));			//eProRegPanasonicReserved2			= 24,				// 松下FP系列PLC的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegPanasonicReserved3, _T("松下FP预留3")));			//eProRegPanasonicReserved3			= 25,				// 松下FP系列PLC的寄存器预留3
	strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSerialAscii, _T("三菱Q系列MC串口ASCII协议")));				//eProRegMitsubishiQSerialAscii		= 31,				// 三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSerialBinary, _T("三菱Q系列MC串口Binary协议")));		//eProRegMitsubishiQSerialBinary		= 32,				// 三菱Q系列PLC的寄存器串口MC(格式5)二进制协议(一般使用这种)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketAscii, _T("三菱Q系列MC网口ASCII协议")));		//eProRegMitsubishiQSocketAscii		= 33,				// 三菱Q系列PLC的寄存器网口MC（E3）ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketBinary, _T("三菱Q系列MC网口Binary协议")));				//eProRegMitsubishiQSocketBinary		= 34,				// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketAsciiUDP, _T("三菱Q系列网口UDP-ASCII协议")));		//eProRegMitsubishiQSocketAsciiUDP		= 35,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议(一般使用这种)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiQSocketBinaryUDP, _T("三菱Q系列网口UDP-Binary协议")));				//eProRegMitsubishiQSocketBinaryUDP		= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXSerialAscii, _T("三菱FX系列MC串口ASCII协议")));				//eProRegMitsubishiFXSerialAscii		= 41,				// 三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXSocketBinaryMC1E, _T("三菱FX系列MC网口Binary协议")));			//eProRegMitsubishiFXReserved1		= 42,				// 三菱FX系列PLC的寄存器预留1   (FX-3U-ENET-ADP网口模块使用)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved2, _T("三菱FX系列预留2")));			//eProRegMitsubishiFXReserved2		= 43,				// 三菱FX系列PLC的寄存器预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved3, _T("三菱FX系列预留3")));			//eProRegMitsubishiFXReserved3		= 44,				// 三菱FX系列PLC的寄存器预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegMitsubishiFXReserved4, _T("三菱FX系列预留4")));					//eProRegMitsubishiFXReserved4		= 45,				// 三菱FX系列PLC的寄存器预留4
	strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaSocketAscii, _T("横河网口ASCII协议")));		//eProRegYokogawaSocketAscii			= 51,				// 横河PLC的寄存器网口ASCII协议（PC-Link(以太网ASCII码)，一般用这种）
	strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaSocketBinary, _T("横河网口Binary协议")));				//eProRegYokogawaSocketBinary			= 52,				// 横河PLC的寄存器网口Binary协议（PC-Link(以太网二进制)）
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaPCLinkAscii, _T("横河网口PC-Link(ASCII码)")));			//eProRegYokogawaPCLinkAscii			= 53,				// 横河PLC的寄存器网口PC-Link(ASCII码)
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaReserved1, _T("横河PLC的寄存器预留1")));			//eProRegYokogawaReserved1			= 54,				// 横河PLC的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYokogawaReserved2, _T("横河PLC的寄存器预留2")));				//eProRegYokogawaReserved2			= 55,				// 横河PLC的寄存器预留2
	strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSerialAscii, _T("基恩士KV系列串口ASCII协议")));		//eProRegKeyenceSerialAscii			= 61,				// 基恩士KV系列PLC的寄存器串口ASCII协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSerialPCLink, _T("基恩士KV系列串口PC-Link")));		//eProRegKeyenceSerialPCLink			= 62,				// 基恩士KV系列PLC的寄存器串口上位链路协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketAscii, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)")));				//eProRegKeyenceSocketAscii			= 63,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketBinary, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)")));				//eProRegKeyenceSocketBinary			= 64,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)
	strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceSocketPCLink, _T("基恩士KV系列网口PC-Link协议")));				//eProRegKeyenceSocketPCLink			= 65,				// 基恩士KV系列PLC的寄存器网口上位链路协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegKeyenceReserved1, _T("基恩士PLC预留1")));			//eProRegKeyenceReserved1				= 66,				// 基恩士PLC的寄存器预留1

	//strArrayProtocolTypeVal.push_back(make_pair(eProRegYaskawa, _T("安川寄存器协议1")));			//eProRegYaskawa						= 71,				// 安川PLC的寄存器协议
	
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved1, _T("PLC的寄存器操作协议预留1")));			//eProRegReserved1					= 81,				// PLC的寄存器操作协议预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved2, _T("PLC的寄存器操作协议预留2")));					//eProRegReserved2					= 82,				// PLC的寄存器操作协议预留2
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved3, _T("PLC的寄存器操作协议预留3")));		//eProRegReserved3					= 83,				// PLC的寄存器操作协议预留3
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved4, _T("PLC的寄存器操作协议预留4")));				//eProRegReserved4					= 84,				// PLC的寄存器操作协议预留4
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegReserved5, _T("PLC的寄存器操作协议预留5")));			//eProRegReserved5					= 85,				// PLC的寄存器操作协议预留5
	strArrayProtocolTypeVal.push_back(make_pair(eProRegFujiSPH, _T("富士SPH系列网口寄存器协议")));			//eProRegFujiSPH                      = 91,               // 富士MICREX-SX SPH系列 PLC的寄存器协议                                         
	strArrayProtocolTypeVal.push_back(make_pair(eProRegOmronSerialHostLink, _T("欧姆龙串口HostLink协议")));	//		eProRegOmronSerialHostLink = 101, _T("欧姆龙PLC的寄存器串口HostLink协议")                                         
	strArrayProtocolTypeVal.push_back(make_pair(eProRegOmronSocketFins, _T("欧姆龙网口Fins协议")));			//eProRegOmronSocketFins = 102, _T("欧姆龙PLC的寄存器网口Fins协议")                                         
	strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSerialAscii, _T("Delta(台达)Modbus寄存器串口ASCII协议")));	// Delta(台达)Modbus的寄存器串口ASCII协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSerialRtu, _T("Delta(台达)Modbus寄存器串口RTU协议")));		// Delta(台达)Modbus的寄存器串口RTU协议
	strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusSocketTcp, _T("Delta(台达)Modbus寄存器网口TCP协议")));		// Delta(台达)Modbus的寄存器网口TCP协议
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusReserved1, _T("Delta(台达)Modbus的寄存器预留1")));			// Delta(台达)Modbus的寄存器预留1
	//strArrayProtocolTypeVal.push_back(make_pair(eProRegDeltaModbusReserved2, _T("Delta(台达)Modbus的寄存器预留2")));			// Delta(台达)Modbus的寄存器预留2

	// 通信协议
	m_ComboProtoccolType.ResetContent();
	for (int it=0;it<strArrayProtocolTypeVal.size();it++)
	{
		m_ComboProtoccolType.AddString(strArrayProtocolTypeVal.at(it).second);
	}
	m_ComboProtoccolType.SetCurSel(0);

	BOOL bIsPlcProtocolType = (strArrayProtocolTypeVal.at(0).first > eProRegBase);
	ShowPLCProtocolType(bIsPlcProtocolType);
	m_pParent->UpdateControlShow(bIsPlcProtocolType);


	// 扩展通信协议
	m_ComboExProtoccolType.ResetContent();
	m_ComboExProtoccolType.AddString(_T("原通信协议"));
	for (int it=0;it<5;it++)
	{
		CString str("");
		str.Format(_T("扩展通用指令%d"),it+1);
		m_ComboExProtoccolType.AddString(str);
	}
	m_ComboExProtoccolType.SetCurSel(0);

	if (NULL != m_pVisionASM)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

		if (sysPlatformInfo.m_bEnableMultiCalibExtension==TRUE)
		{
			m_ComboExProtoccolType.ShowWindow(m_bshowCommTypeSet);
		}
		else
		{
			m_ComboExProtoccolType.ShowWindow(SW_HIDE);
		}
	}

	// 运动控制
	m_ComboMotionControlType.ResetContent();
	m_ComboMotionControlType.AddString(_T("无运动控制"));
	m_ComboMotionControlType.AddString(_T("雅马哈机器人"));
	m_ComboMotionControlType.AddString(_T("雅马哈机器人340"));
	m_ComboMotionControlType.SetCurSel(0);

	// 发送轴顺序
	m_ComboOrderType.ResetContent();
	
	vector<CString> StrXYD;
	StrXYD.resize(6);
	StrXYD[0] = _T("X-Y-D");
	StrXYD[1] = _T("Y-X-D");
	StrXYD[2] = _T("X-D-Y");
	StrXYD[3] = _T("D-X-Y");
	StrXYD[4] = _T("Y-D-X");
	StrXYD[5] = _T("D-Y-X");

	if (NULL != m_pVisionASM)
	{
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		if(ePlatformX1X2Y ==sysPlatformInfo.m_eMidPlatformType)
		{
			StrXYD[0] = _T("X1-X2-Y");
			StrXYD[1] = _T("X2-X1-Y");
			StrXYD[2] = _T("X1-Y-X2");
			StrXYD[3] = _T("Y-X1-X2");
			StrXYD[4] = _T("X2-Y-X1");
			StrXYD[5] = _T("Y-X2-X1");
		}
		else if(ePlatformXY1Y2 ==sysPlatformInfo.m_eMidPlatformType)
		{
			StrXYD[0] = _T("X-Y1-Y2");
			StrXYD[1] = _T("Y1-X-Y2");
			StrXYD[2] = _T("X-Y2-Y1");
			StrXYD[3] = _T("Y2-X-Y1");
			StrXYD[4] = _T("Y1-Y2-X");
			StrXYD[5] = _T("Y2-Y1-X");
		}

	}

	for (int it=0;it<StrXYD.size();it++)
	{
		m_ComboOrderType.AddString(StrXYD[it]);
	}


	m_ComboOrderType.SetCurSel(0);



	UpdateDlgByAuthorize();

}

void CDlgCommTypeSetting::UpdateDlgData(BOOL bSaveAndValidate)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	UpdateDlgByAuthorize();
	if (bSaveAndValidate)
	{
		UpdateData(TRUE);
		m_commParamInfo.m_eCommType = (CommType)m_nCommType;
		//m_commParamInfo.m_eCmdParamOrderType	= (CmdParamOrderType)m_nCmdParamOrderType;
		m_commParamInfo.m_eCmdParamOrderType	= (CmdParamOrderType)m_ComboOrderType.GetCurSel();
		int nSel = m_ComboProtoccolType.GetCurSel();

		if (nSel>=0 && nSel<strArrayProtocolTypeVal.size())
		{
			m_commParamInfo.m_eProtocolType			= (ProtocolType)strArrayProtocolTypeVal.at(nSel).first;
		}

		m_commParamInfo.m_eExProtocolType		= (ExProtocolType)(m_ComboExProtoccolType.GetCurSel());
		m_commParamInfo.m_eMotionControlType	= (MotionControlType)(m_ComboMotionControlType.GetCurSel());
		m_commParamInfo.m_bRegiterType			= m_bRegiterType;
		m_commParamInfo.m_nRegDataTypeIndex		= m_nRegDataTypeIndex;
		m_commParamInfo.m_bUseSpecialComm		= m_bUseSpecialComm;
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		vsConfig->m_CommParamInfo = m_commParamInfo;
		m_pVisionASM->SetParamToXYDCommProtocol();
	}
	else
	{
		// 共享通信时,共享的通信需要同步 源通信的参数设置
		if (eShareCommTar == m_pVisionASM->GetShareCommInfoArray()->GetShareCommType(m_pVisionASM->GetSysPlatformIndex()))
		{
			if ((m_pVisionASM->GetVisionAlignOther() != NULL))
			{
				CVisionASMConfig *vsConfig;
				m_pVisionASM->GetVisionAlignOther()->GetVSconfig(vsConfig);
				if (NULL != vsConfig)
				{
					// 修改轴顺序，不与主共享工位同步
					CmdParamOrderType cmdParamOrderTemp = m_commParamInfo.m_eCmdParamOrderType;
					m_commParamInfo = vsConfig->m_CommParamInfo;
					m_commParamInfo.m_eCmdParamOrderType = cmdParamOrderTemp;
				}
			}

			ShowShareCommControl(FALSE);

		}
		
		
		m_nCommType = m_commParamInfo.m_eCommType;
		//m_nCmdParamOrderType = m_commParamInfo.m_eCmdParamOrderType;
		m_ComboOrderType.SetCurSel(m_commParamInfo.m_eCmdParamOrderType);
		int nSel = 0;
		for (int it = 0;it<strArrayProtocolTypeVal.size();it++)
		{
			if (strArrayProtocolTypeVal.at(it).first == m_commParamInfo.m_eProtocolType)
			{
				nSel = it;
				break;
			}
		}

		// 目前以下5个协议支持DM、EM(ZR)；其他不支持EM
		if(m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || 
			m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink|| 
			m_commParamInfo.m_eProtocolType ==eProRegOmronSerialHostLink|| 
			m_commParamInfo.m_eProtocolType ==eProRegOmronSocketFins ||
			m_commParamInfo.m_eProtocolType ==eProRegMitsubishiQSocketBinary)
		{
			GetDlgItem(IDC_RADIO_EM_TYPE)->ShowWindow(m_bshowCommOtherSet);
		}
		else
		{
			GetDlgItem(IDC_RADIO_EM_TYPE)->ShowWindow(SW_HIDE);
			m_nRegDataTypeIndex = 0;
			m_commParamInfo.m_nRegDataTypeIndex = m_nRegDataTypeIndex;
		}

		if(m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || 
			m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink|| 
			m_commParamInfo.m_eProtocolType ==eProRegOmronSocketFins|| 
			m_commParamInfo.m_eProtocolType ==eProRegOmronSerialHostLink)
		{
			GetDlgItem(IDC_RADIO_DM_TYPE)->SetWindowTextA(_T("DM"));
		}
		else
		{
			GetDlgItem(IDC_RADIO_DM_TYPE)->SetWindowTextA(_T("D"));
		}


		// eProRegMitsubishiQSocketBinary 为ZR
		if(m_commParamInfo.m_eProtocolType ==eProRegMitsubishiQSocketBinary)
		{
			GetDlgItem(IDC_RADIO_EM_TYPE)->SetWindowTextA(_T("ZR"));
		}
		else
		{
			GetDlgItem(IDC_RADIO_EM_TYPE)->SetWindowTextA(_T("EM"));
		}
		// 只有eProRegKeyenceSerialPCLink eProRegKeyenceSocketPCLink协议支持ZF
		bool bShowZF = (m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink);
		GetDlgItem(IDC_RADIO_ZF_TYPE)->ShowWindow(m_bshowCommOtherSet && bShowZF);

		m_ComboProtoccolType.SetCurSel(nSel);
		BOOL bIsPlcProtocolType = (m_commParamInfo.m_eProtocolType> eProRegBase);


		ShowPLCProtocolType(bIsPlcProtocolType);
		m_pParent->UpdateControlShow(bIsPlcProtocolType);

		m_ComboExProtoccolType.SetCurSel(m_commParamInfo.m_eExProtocolType);
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);
		if (sysPlatformInfo.m_bEnableMultiCalibExtension && sysPlatformInfo.m_nMultiCalibExtensionMaxNum>0)
		{
			m_ComboExProtoccolType.SetCurSel(eExProReserved1);
		}

		m_ComboMotionControlType.SetCurSel(m_commParamInfo.m_eMotionControlType);
		m_bRegiterType = m_commParamInfo.m_bRegiterType;
		m_nRegDataTypeIndex = m_commParamInfo.m_nRegDataTypeIndex;
		m_bUseSpecialComm = m_commParamInfo.m_bUseSpecialComm;
		
		UpdateData(FALSE);
	}
}

BOOL CDlgCommTypeSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitDlgItem();
	UpdateDialogLanguage();
	UpdateDlgData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgCommTypeSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgCommTypeSetting::ShowPLCProtocolType(BOOL bVisible)
{
	

	GetDlgItem(IDC_STATIC_PLC_MEMORY_TYPE)->ShowWindow(bVisible && m_bshowCommOtherSet);
	GetDlgItem(IDC_RADIO_DM_TYPE)->ShowWindow(bVisible && m_bshowCommOtherSet);
	//GetDlgItem(IDC_RADIO_EM_TYPE)->ShowWindow(bVisible && m_bshowCommOtherSet);

	GetDlgItem(IDC_STATIC_TEST_REG)->ShowWindow(bVisible && m_bshowCommTypeSet);
	GetDlgItem(IDC_RADIO_SINGLE_WORD)->ShowWindow(bVisible && m_bshowCommTypeSet);
	GetDlgItem(IDC_RADIO_DOUBLE_WORD)->ShowWindow(bVisible && m_bshowCommTypeSet);

}

void CDlgCommTypeSetting::OnCbnSelchangeComboProtocolType()
{
	// TODO: 在此添加控件通知处理程序代码
	// 通信协议切换
	UpdateData(TRUE);
	int nSel = m_ComboProtoccolType.GetCurSel();

	if (nSel< 0 && nSel>=strArrayProtocolTypeVal.size())
	{
		return;
	}

	// 当前为PLC读写寄存器协议
	BOOL bIsPlcProtocolType = (strArrayProtocolTypeVal.at(nSel).first > eProRegBase);
	
	if (nSel>=0 && nSel<strArrayProtocolTypeVal.size())
	{
		m_commParamInfo.m_eProtocolType	= (ProtocolType)strArrayProtocolTypeVal.at(nSel).first;
	}
	// 显示寄存器相关需要显示的控件

	ShowPLCProtocolType(bIsPlcProtocolType);
	if (NULL != m_pParent)
	{
		m_pParent->UpdateControlShow(bIsPlcProtocolType);
	}
	
	// 目前以下5个协议支持DM、EM(ZR)；其他不支持EM
	if(m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || 
		m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink|| 
		m_commParamInfo.m_eProtocolType ==eProRegOmronSerialHostLink|| 
		m_commParamInfo.m_eProtocolType ==eProRegOmronSocketFins ||
		m_commParamInfo.m_eProtocolType ==eProRegMitsubishiQSocketBinary)
	{
		GetDlgItem(IDC_RADIO_EM_TYPE)->ShowWindow(m_bshowCommOtherSet);
	}
	else
	{
		GetDlgItem(IDC_RADIO_EM_TYPE)->ShowWindow(SW_HIDE);
		m_nRegDataTypeIndex = 0;
		m_commParamInfo.m_nRegDataTypeIndex = m_nRegDataTypeIndex;
	}

	if(m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || 
		m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink|| 
		m_commParamInfo.m_eProtocolType ==eProRegOmronSocketFins|| 
		m_commParamInfo.m_eProtocolType ==eProRegOmronSerialHostLink)
	{
		GetDlgItem(IDC_RADIO_DM_TYPE)->SetWindowTextA(_T("DM"));
	}
	else
	{
		GetDlgItem(IDC_RADIO_DM_TYPE)->SetWindowTextA(_T("D"));
	}

	// eProRegMitsubishiQSocketBinary 为ZR
	if(m_commParamInfo.m_eProtocolType ==eProRegMitsubishiQSocketBinary)
	{
		GetDlgItem(IDC_RADIO_EM_TYPE)->SetWindowTextA(_T("ZR"));
	}
	else
	{
		GetDlgItem(IDC_RADIO_EM_TYPE)->SetWindowTextA(_T("EM"));
	}
	// 只有eProRegKeyenceSerialPCLink eProRegKeyenceSocketPCLink协议支持ZF
	bool bShowZF = (m_commParamInfo.m_eProtocolType == eProRegKeyenceSerialPCLink || m_commParamInfo.m_eProtocolType ==eProRegKeyenceSocketPCLink);
	GetDlgItem(IDC_RADIO_ZF_TYPE)->ShowWindow(m_bshowCommOtherSet && bShowZF);

	if (NULL != m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		vsConfig->m_CommParamInfo = m_commParamInfo;
		m_pVisionASM->SetParamToXYDCommProtocol();
		m_pVisionASM->UpdateCommTool();
	}

	UpdateData(FALSE);

}


void CDlgCommTypeSetting::ShowShareCommControl(BOOL bVisible)
{
	CWnd *pWnd;
	pWnd = GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		pWnd->EnableWindow(bVisible);
		// 修改轴顺序控件不受共享通信控制
		if(GetDlgItem(IDC_STATIC_SEND_AXIS_ORDER) == pWnd || GetDlgItem(IDC_RADIO_SEND_AXIS_ORDER_XYD) == pWnd 
			|| GetDlgItem(IDC_RADIO_SEND_AXIS_ORDER_YXD) == pWnd || GetDlgItem(IDC_COMBO_SEND_AXIS_ORDER) == pWnd)
		{
			pWnd->EnableWindow(TRUE);
		}
		pWnd = pWnd->GetNextWindow();
	}
}

void CDlgCommTypeSetting::OnBnClickedRadSerialType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ChangeCommType((CommType)m_nCommType);
}


void CDlgCommTypeSetting::OnBnClickedRadSocketType()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ChangeCommType((CommType)m_nCommType);
}

BOOL CDlgCommTypeSetting::ChangeCommType(const CommType eCommType)
{

	if (NULL == m_pVisionASM )
	{
		return FALSE;
	}

	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);
	if (NULL == vsConfig)
	{
		return FALSE;
	}
	m_commParamInfo.m_eCommType = (CommType)m_nCommType;
	if (eCommType == vsConfig->m_CommParamInfo.m_eCommType)
	{
		return TRUE;
	}
	else // 改变通信连接类型
	{
		vsConfig->m_CommParamInfo = m_commParamInfo;
		m_pVisionASM->ChangeCommType(eCommType);
		m_pVisionASM->SetParamToXYDCommProtocol();
		if (NULL != m_pParent)
		{
			m_pParent->ChangeCommType(eCommType);
		}
		return TRUE;

	}


}

void CDlgCommTypeSetting::SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow)
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

void CDlgCommTypeSetting::UpdateDlgByAuthorize()
{
	if (NULL !=m_pVisionASM)
	{
		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		if (NULL !=vsConfig)
		{
			CString strNodeName = _T("");
			// 通信连接类型
			bool bFind = vsConfig->GetCheckedByItem(eCommAuthorize_CommTypeSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCommTypeSet,m_bEnableCommTypeSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_COMM_TYPE_SETTING,m_bEnableCommTypeSet,m_bshowCommTypeSet);
			}

			bFind = vsConfig->GetCheckedByItem(eCommAuthorize_OtherSet,vsConfig->m_cTreeAuthorizeManagement,strNodeName,m_bshowCommOtherSet,m_bEnableCommOtherSet);
			if (bFind)
			{
				SetGroupBoxStatus(IDC_STATIC_COMM_TYPE_OTHER_SETTING,m_bEnableCommOtherSet,m_bshowCommOtherSet);
			}
		}
	}
}