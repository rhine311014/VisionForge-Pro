// DlgSystemInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSystemInfo.h"
#include "afxdialogex.h"


// CDlgSystemInfo 对话框

IMPLEMENT_DYNAMIC(CDlgSystemInfo, CDialog)

CDlgSystemInfo::CDlgSystemInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSystemInfo::IDD, pParent)
{
	m_psaSysInfoStrings = NULL;
	m_bSysKeyboardEnabled = FALSE;
	m_pParent = NULL;

	m_strSimpleSystemInfo += _T("");
}

CDlgSystemInfo::~CDlgSystemInfo()
{
}

void CDlgSystemInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SYSTEM_INFO, m_ctrlProductTree);
}


BEGIN_MESSAGE_MAP(CDlgSystemInfo, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE_SYSTEM_INFO, &CDlgSystemInfo::OnBnClickedBtnSaveSystemInfo)
	ON_BN_CLICKED(IDC_BTN_SAVE_SIMPLE_SYSTEM_INFO, &CDlgSystemInfo::OnBnClickedBtnSaveSimpleSystemInfo)
	ON_BN_CLICKED(IDOK, &CDlgSystemInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSystemInfo::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSystemInfo 消息处理程序


BOOL CDlgSystemInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdateDialogLanguage();

	UpdateDlgSystemInfoTree();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgSystemInfo::UpdateDialogLanguage()
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

		if(_T("IDD_DLG_SYSTEM_INFO") == strDlgID)
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
		}  
	}
	xml.LeaveElem();
}
// 
// CString GetSysInfoString(CStringArray* pStringArray, int nIndex)
// {
// 	CString str;
// 	if (pStringArray == NULL)
// 	{
// 		return str;
// 	}
// 
// 	if (nIndex < 0 || nIndex >= pStringArray->GetSize())
// 	{
// 		return str;
// 	}
// 
// 	str = pStringArray->GetAt(nIndex);
// 
// 	return str;
// }

CString GetLevelString(int nLevel, int nSpaceStep, int nIndex)
{
	CString strLevelString = _T("");
	if (nLevel <= 0 || nLevel > 3)
	{
		return strLevelString;
	}

	CStringArray strArraySpaceVal;
	strArraySpaceVal.SetSize(12);
	strArraySpaceVal.SetAtGrow(0, _T(""));
	strArraySpaceVal.SetAtGrow(1, _T(" "));
	strArraySpaceVal.SetAtGrow(2, _T("  "));
	strArraySpaceVal.SetAtGrow(3, _T("   "));
	strArraySpaceVal.SetAtGrow(4, _T("    "));
	strArraySpaceVal.SetAtGrow(5, _T("     "));
	strArraySpaceVal.SetAtGrow(6, _T("      "));
	strArraySpaceVal.SetAtGrow(7, _T("       "));
	strArraySpaceVal.SetAtGrow(8, _T("        "));
	strArraySpaceVal.SetAtGrow(9, _T("         "));
	strArraySpaceVal.SetAtGrow(10, _T("          "));
	strArraySpaceVal.SetAtGrow(11, _T("           "));

	int nSpaceNum = 0;
	nSpaceNum = nSpaceStep*(nLevel - 1);
	if (nSpaceNum >= strArraySpaceVal.GetSize())
	{
		return strLevelString;
	}

	CString strIndexSign;
	switch(nLevel)
	{
	case 1:
		{
			strIndexSign.Format(_T(""));
			strLevelString = GetSysInfoString(&strArraySpaceVal, nSpaceNum) + strIndexSign;
		}
		break;
	case 2:
		{
			strIndexSign.Format(_T("%d."), nIndex);
			strLevelString = GetSysInfoString(&strArraySpaceVal, nSpaceNum) + strIndexSign;
		}
		break;
	case 3:
		{
			strIndexSign.Format(_T("(%d)"), nIndex);
			strLevelString = GetSysInfoString(&strArraySpaceVal, nSpaceNum) + strIndexSign;
		}
		break;
	default:
		{
			strIndexSign = _T("");
		}
	}

	return strLevelString;
}

CString GetOpenDir(HWND hwndOwner, LPCTSTR lpszTitle)
{
	TCHAR szDir[MAX_PATH + 1];
	ITEMIDLIST *pidl;
	BROWSEINFO bi;

	memset(&bi, 0, sizeof(BROWSEINFO));
	bi.hwndOwner = hwndOwner;
	bi.lpszTitle = lpszTitle;
	bi.ulFlags	 = BIF_RETURNONLYFSDIRS;

	//弹出选择目录对话框。
	return ((pidl = SHBrowseForFolder(&bi)) && SHGetPathFromIDList(pidl, szDir)) ? szDir : _T("");
}

CString	GetSystemBuildTime()
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	
	// 获取系统文件修改时间
	CString strTime;
	CFileStatus fs;
	if (CFile::GetStatus(strPath,fs))
	{
		strTime = fs.m_mtime.Format("%Y-%m-%d %H:%M:%S");		
	}

	return strTime;
}





void CDlgSystemInfo::UpdateDlgSystemInfoTree()
{
	// 软件版本
	CString strSoftwareVersionName = _T("软件版本");
	CString strWin = _T("(Win32)");
#ifdef _WIN64
	strWin = _T("(Win64)");
#else
	strWin = _T("(Win32)");
#endif
	
	// 编译时间
	CString strBuildTimeName = _T("编译时间");

	// 系统名称
	CString strSystemNameName = _T("系统");

	// 平台名称
	CString strPlatformNameName = _T("平台");

	// 平台类型
	CString strPlatformTypeName = _T("平台类型");
	CStringArray strArrayPlatformTypeVal;
	strArrayPlatformTypeVal.SetSize(100);
	strArrayPlatformTypeVal.SetAtGrow(ePlatformUnknown + 1, _T("Unknown"));		// 未知
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXYD + 1, _T("XYD"));				// XYD
	strArrayPlatformTypeVal.SetAtGrow(ePlatformX1X2Y + 1, _T("X1X2Y"));
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXY1Y2 + 1, _T("XY1Y2"));
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXY + 1, _T("XY"));				// XY
	strArrayPlatformTypeVal.SetAtGrow(ePlatformYX1X2PD + 1, _T("YX1X2PD"));		// YX+D
	strArrayPlatformTypeVal.SetAtGrow(ePlatformYDPX1X2 + 1, _T("YDPX1X2"));		// YD+X
	strArrayPlatformTypeVal.SetAtGrow(ePlatformDXY + 1, _T("DXY"));				// DXY
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXD + 1, _T("XD"));				// XD
	strArrayPlatformTypeVal.SetAtGrow(ePlatformYD + 1, _T("YD"));				// YD
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXDPY + 1, _T("XDPY"));			// XD + Y Y方向标定时使用相机轴移动，适用于三力设备
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXDPY1 + 1, _T("XDPY1"));			// XD + Y Y方向标定时，在Y平台进行吸放动作，适用于风华设备
	strArrayPlatformTypeVal.SetAtGrow(ePlatformXYPD + 1, _T("XYPD"));			// XY + D
	strArrayPlatformTypeVal.SetAtGrow(ePlatformX + 1, _T("X"));					// 只有X平台，四维焊接项目特殊使用
	strArrayPlatformTypeVal.SetAtGrow(ePlatformY + 1, _T("Y"));					// 只有Y平台


	// 相机数量
	CString strCamNumName = _T("相机数量");

	// 位置数量
	CString strPositionNumName = _T("位置数量");

	// 通信接口
	CString strCommTypeName = _T("通信接口");
	CStringArray strArrayCommTypeVal;
	strArrayCommTypeVal.SetSize(10);
	strArrayCommTypeVal.SetAtGrow(ePlatformUnknown + 1, _T("Unknown"));		// 	eCommUnknown = -1,	// 未知类型
	strArrayCommTypeVal.SetAtGrow(eSerial + 1, _T("串口"));					// 	eSerial,			// 串口
	strArrayCommTypeVal.SetAtGrow(eSocket + 1, _T("网口"));					// 	eSocket				// 网络

	// 通信协议
	CString strProtocolTypeName = _T("通信协议");
	CStringArray strArrayProtocolTypeVal;
	strArrayProtocolTypeVal.SetSize(100);
	strArrayProtocolTypeVal.SetAtGrow(eProUnknown + 1, _T("Unknown"));															//eProUnknown							= -1,				// 未知的通信协议
	strArrayProtocolTypeVal.SetAtGrow(eProCmdFast + 1, _T("标准通用无协议"));													//eProCmdFast							= 0,				// 通用指令Fast通用无协议
	strArrayProtocolTypeVal.SetAtGrow(eProCmdVS + 1, _T("标准通用无协议"));														//eProCmdVS							= 1,				// 通用指令VisionSmart通用无协议
	strArrayProtocolTypeVal.SetAtGrow(eProCmdYamaha + 1, _T("雅马哈机器人无协议"));												//eProCmdYamaha						= 2,				// 通用指令雅马哈机器人无协议
	strArrayProtocolTypeVal.SetAtGrow(eProCmdEpson + 1, _T("通用无协议预留1"));													//eProCmdEpson						= 3,				// 通用指令通用无协议预留1
	strArrayProtocolTypeVal.SetAtGrow(eProCmdReserved2 + 1, _T("通用无协议预留2"));										        //eProCmdReserved2					= 4,				// 通用指令通用无协议预留2
	strArrayProtocolTypeVal.SetAtGrow(eProCmdReserved3 + 1, _T("通用无协议预留3"));										        //eProCmdReserved3					= 5,				// 通用指令通用无协议预留3
	strArrayProtocolTypeVal.SetAtGrow(eProRegBase + 1, _T("PLC链接"));															//eProRegBase							= 10,				// PLC链接，大于该枚举值的都是PLC链接协议，直接读取存储PLC寄存器
	strArrayProtocolTypeVal.SetAtGrow(eProRegModbusSerialAscii + 1, _T("Modbus寄存器串口ASCII协议"));				//eProRegModbusSerialAscii			= 11,				// Modbus的寄存器串口ASCII协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegModbusSerialRtu + 1, _T("Modbus寄存器串口RTU协议"));				//eProRegModbusSerialRtu				= 12,				// Modbus的寄存器串口RTU协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegModbusSocketTcp + 1, _T("Modbus寄存器网口TCP协议"));			//eProRegModbusSocketTcp				= 13,				// Modbus的寄存器网口TCP协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegModbusReserved1 + 1, _T("Modbus的寄存器预留1"));			//eProRegModbusReserved1				= 14,				// Modbus的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegModbusReserved2 + 1, _T("Modbus的寄存器预留2"));			//eProRegModbusReserved2				= 15,				// Modbus的寄存器预留2
	strArrayProtocolTypeVal.SetAtGrow(eProRegPanasonicSerialMewtocol + 1, _T("松下FP系列PLC的寄存器串口Mewtocol协议"));					//eProRegPanasonicSerialMewtocol		= 21,				// 松下FP系列PLC的寄存器串口Mewtocol协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegPanasonicSocketMewtocol + 1, _T("松下FP系列PLC的寄存器网口Mewtocol协议"));		//eProRegPanasonicSocketMewtocol		= 22,				// 松下FP系列PLC的寄存器网口Mewtocol协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegPanasonicReserved1 + 1, _T("松下FP系列PLC的寄存器预留1"));				//eProRegPanasonicReserved1			= 23,				// 松下FP系列PLC的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegPanasonicReserved2 + 1, _T("松下FP系列PLC的寄存器预留2"));			//eProRegPanasonicReserved2			= 24,				// 松下FP系列PLC的寄存器预留2
	strArrayProtocolTypeVal.SetAtGrow(eProRegPanasonicReserved3 + 1, _T("松下FP系列PLC的寄存器预留3"));			//eProRegPanasonicReserved3			= 25,				// 松下FP系列PLC的寄存器预留3
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSerialAscii + 1, _T("三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议"));				//eProRegMitsubishiQSerialAscii		= 31,				// 三菱Q系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSerialBinary + 1, _T("三菱Q系列PLC的寄存器串口MC(格式5)二进制协议"));		//eProRegMitsubishiQSerialBinary		= 32,				// 三菱Q系列PLC的寄存器串口MC(格式5)二进制协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSocketAscii + 1, _T("三菱Q系列PLC的寄存器网口MC（E3）ASCII协议"));		//eProRegMitsubishiQSocketAscii		= 33,				// 三菱Q系列PLC的寄存器网口MC（E3）ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSocketBinary + 1, _T("三菱Q系列PLC的寄存器网口MC（E3）二进制协议"));				//eProRegMitsubishiQSocketBinary		= 34,				// 三菱Q系列PLC的寄存器网口MC（E3）二进制协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSocketAsciiUDP + 1, _T("三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议"));		//eProRegMitsubishiQSocketAsciiUDP		= 35,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiQSocketBinaryUDP + 1, _T("三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议"));				//eProRegMitsubishiQSocketBinaryUDP		= 36,				// 三菱Q系列PLC的寄存器网口UDP MC（E3）二进制协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiFXSerialAscii + 1, _T("三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议"));				//eProRegMitsubishiFXSerialAscii		= 41,				// 三菱FX系列PLC的寄存器串口MC(格式4)ASCII协议(一般使用这种)
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiFXSocketBinaryMC1E + 1, _T("三菱FX系列PLC的寄存器网口MC（A兼容1E帧）二进制协议(FX-3U-ENET-ADP网口模块使用)"));			//eProRegMitsubishiFXReserved1		= 42,				// 三菱FX系列PLC的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiFXReserved2 + 1, _T("三菱FX系列PLC的寄存器预留2"));			//eProRegMitsubishiFXReserved2		= 43,				// 三菱FX系列PLC的寄存器预留2
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiFXReserved3 + 1, _T("三菱FX系列PLC的寄存器预留3"));			//eProRegMitsubishiFXReserved3		= 44,				// 三菱FX系列PLC的寄存器预留3
	strArrayProtocolTypeVal.SetAtGrow(eProRegMitsubishiFXReserved4 + 1, _T("三菱FX系列PLC的寄存器预留4"));					//eProRegMitsubishiFXReserved4		= 45,				// 三菱FX系列PLC的寄存器预留4
	strArrayProtocolTypeVal.SetAtGrow(eProRegYokogawaSocketAscii + 1, _T("横河PLC的寄存器网口ASCII协议"));		//eProRegYokogawaSocketAscii			= 51,				// 横河PLC的寄存器网口ASCII协议（PC-Link(以太网ASCII码)，一般用这种）
	strArrayProtocolTypeVal.SetAtGrow(eProRegYokogawaSocketBinary + 1, _T("横河PLC的寄存器网口二进制协议"));				//eProRegYokogawaSocketBinary			= 52,				// 横河PLC的寄存器网口Binary协议（PC-Link(以太网二进制)）
	strArrayProtocolTypeVal.SetAtGrow(eProRegYokogawaPCLinkAscii + 1, _T("横河PLC的寄存器网口PC-Link(ASCII码)"));			//eProRegYokogawaPCLinkAscii			= 53,				// 横河PLC的寄存器网口PC-Link(ASCII码)
	strArrayProtocolTypeVal.SetAtGrow(eProRegYokogawaReserved1 + 1, _T("横河PLC的寄存器预留1"));			//eProRegYokogawaReserved1			= 54,				// 横河PLC的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegYokogawaReserved2 + 1, _T("横河PLC的寄存器预留2"));				//eProRegYokogawaReserved2			= 55,				// 横河PLC的寄存器预留2
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceSerialAscii + 1, _T("基恩士KV系列PLC的寄存器串口ASCII协议"));		//eProRegKeyenceSerialAscii			= 61,				// 基恩士KV系列PLC的寄存器串口ASCII协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceSerialPCLink + 1, _T("基恩士KV系列PLC的寄存器串口上位链路协议"));		//eProRegKeyenceSerialPCLink			= 62,				// 基恩士KV系列PLC的寄存器串口上位链路协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceSocketAscii + 1, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)"));				//eProRegKeyenceSocketAscii			= 63,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）ASCII协议   (QnA兼容3E)
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceSocketBinary + 1, _T("基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)"));				//eProRegKeyenceSocketBinary			= 64,				// 基恩士KV系列使用三菱Q系列PLC的寄存器网口MC（3E）二进制协议  (QnA兼容3E)
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceSocketPCLink + 1, _T("基恩士KV系列PLC的寄存器网口上位链路协议"));				//eProRegKeyenceSocketPCLink			= 65,				// 基恩士KV系列PLC的寄存器网口上位链路协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegKeyenceReserved1 + 1, _T("基恩士PLC的寄存器预留1"));			//eProRegKeyenceReserved1				= 66,				// 基恩士PLC的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegYaskawa + 1, _T("安川PLC的寄存器协议1"));			//eProRegYaskawa						= 71,				// 安川PLC的寄存器协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegReserved1 + 1, _T("PLC的寄存器操作协议预留1"));			//eProRegReserved1					= 81,				// PLC的寄存器操作协议预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegReserved2 + 1, _T("PLC的寄存器操作协议预留2"));					//eProRegReserved2					= 82,				// PLC的寄存器操作协议预留2
	strArrayProtocolTypeVal.SetAtGrow(eProRegReserved3 + 1, _T("PLC的寄存器操作协议预留3"));		//eProRegReserved3					= 83,				// PLC的寄存器操作协议预留3
	strArrayProtocolTypeVal.SetAtGrow(eProRegReserved4 + 1, _T("PLC的寄存器操作协议预留4"));				//eProRegReserved4					= 84,				// PLC的寄存器操作协议预留4
	strArrayProtocolTypeVal.SetAtGrow(eProRegReserved5 + 1, _T("PLC的寄存器操作协议预留5"));			//eProRegReserved5					= 85,				// PLC的寄存器操作协议预留5
	strArrayProtocolTypeVal.SetAtGrow(eProRegFujiSPH + 1, _T("富士MICREX-SX SPH系列 PLC的寄存器协议"));			//eProRegFujiSPH                      = 91,               // 富士MICREX-SX SPH系列 PLC的寄存器协议                                         
	strArrayProtocolTypeVal.SetAtGrow(eProRegOmronSerialHostLink + 1, _T("欧姆龙PLC的寄存器串口HostLink协议"));	//		eProRegOmronSerialHostLink = 101, _T("欧姆龙PLC的寄存器串口HostLink协议")                                         
	strArrayProtocolTypeVal.SetAtGrow(eProRegOmronSocketFins + 1, _T("欧姆龙PLC的寄存器网口Fins协议"));			//eProRegOmronSocketFins = 102, _T("欧姆龙PLC的寄存器网口Fins协议")                                         
	strArrayProtocolTypeVal.SetAtGrow(eProRegDeltaModbusSerialAscii + 1, _T("Delta(台达)Modbus寄存器串口ASCII协议"));	// Delta(台达)Modbus的寄存器串口ASCII协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegDeltaModbusSerialRtu + 1, _T("Delta(台达)Modbus寄存器串口RTU协议"));		// Delta(台达)Modbus的寄存器串口RTU协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegDeltaModbusSocketTcp + 1, _T("Delta(台达)Modbus寄存器网口TCP协议"));		// Delta(台达)Modbus的寄存器网口TCP协议
	strArrayProtocolTypeVal.SetAtGrow(eProRegDeltaModbusReserved1 + 1, _T("Delta(台达)Modbus的寄存器预留1"));			// Delta(台达)Modbus的寄存器预留1
	strArrayProtocolTypeVal.SetAtGrow(eProRegDeltaModbusReserved2 + 1, _T("Delta(台达)Modbus的寄存器预留2"));			// Delta(台达)Modbus的寄存器预留2

	// 寄存器类型
	CString strRegiterTypeName = _T("寄存器类型");
	CStringArray strArrayRegiterTypeVal;
	strArrayRegiterTypeVal.SetSize(2);
	strArrayRegiterTypeVal.SetAtGrow(0, _T("单字"));			// 	false,	单字
	strArrayRegiterTypeVal.SetAtGrow(1, _T("双字"));			// 	true,	双字
	
	// 共享通信
	CString strShareCommEnableName = _T("共享通信");
	CStringArray strShareCommEnableVal;
	strShareCommEnableVal.SetSize(2);
	strShareCommEnableVal.SetAtGrow(0, _T("否"));		// 	false,	否
	strShareCommEnableVal.SetAtGrow(1, _T("是"));		// 	true,	是

	// 共享通信接口号
	CString strShareCommIndexName = _T("共享通信接口号");

	// 共享通信并行
	CString strShareCommPlatformParallelName = _T("共享通信并行");
	CStringArray strShareCommPlatformParallelVal;
	strShareCommPlatformParallelVal.SetSize(2);
	strShareCommPlatformParallelVal.SetAtGrow(0, _T("否"));		// 	false,	否
	strShareCommPlatformParallelVal.SetAtGrow(1, _T("是"));		// 	true,	是

	// 目标对象相机独立
	CString strTargetObjectCamSeparateName = _T("目标对象相机独立");
	CStringArray strArrayTargetObjectCamSeparateVal;
	strArrayTargetObjectCamSeparateVal.SetSize(2);
	strArrayTargetObjectCamSeparateVal.SetAtGrow(0, _T("否"));			// 	false,	否
	strArrayTargetObjectCamSeparateVal.SetAtGrow(1, _T("是"));			// 	true,	是

	// 目标相机关联映射类型
	CString strTarObjCamSepNoAxisCalibName = _T("目标相机关联映射类型");
	CStringArray strArrayTarObjCamSepNoAxisCalibVal;
	strArrayTarObjCamSepNoAxisCalibVal.SetSize(2);
	strArrayTarObjCamSepNoAxisCalibVal.SetAtGrow(0, _T("读取平台轴位置"));	// 	false,	否
	strArrayTarObjCamSepNoAxisCalibVal.SetAtGrow(1, _T("相机直接映射"));	// 	true,	是

	// 目标相机关联映射(使用标定板)
	CString strTargetCalibUseBoardName = _T("目标相机关联映射(使用标定板)");
	CStringArray strArrayTargetCalibUseBoardVal;
	strArrayTargetCalibUseBoardVal.SetSize(2);
	strArrayTargetCalibUseBoardVal.SetAtGrow(0, _T("否"));				// 	false,	否
	strArrayTargetCalibUseBoardVal.SetAtGrow(1, _T("是"));				// 	true,	是

	// 拾取功能
	CString strbPlatformPickName = _T("拾取功能");
	CStringArray strArrayPlatformPickVal;
	strArrayPlatformPickVal.SetSize(2);
	strArrayPlatformPickVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArrayPlatformPickVal.SetAtGrow(1, _T("是"));				// 	true,	


	// 拾取平台类型
	CString strbPickCamPlatformTypeName = _T("拾取平台类型");
	CStringArray strArrayPickCamPlatformTypeVal;
	strArrayPickCamPlatformTypeVal.SetSize(5);

	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformUnknown + 1, _T("XYD平台固定安装"));	// 未知
	strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepFix, _T("XYD平台固定安装"));	// 独立固定安装，只针对XYD、UVW、DXY平台
	strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformShareXY, _T("共享XYD平台的XY"));	// 共享运动平台的XY，即相机随着拾取头和运动平台一起XY方向运动
	strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepX, _T("独立X轴"));				// 独立X轴，只针对XYD、UVW、DXY平台
	strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepY, _T("独立Y轴"));				// 独立Y轴，只针对XYD、UVW、DXY平台
	strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepXY, _T("独立XY轴"));			// 独立XY轴，只针对XYD、UVW、DXY平台

	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformUnknown + 1, _T("XYD平台固定安装"));	// 未知
	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepFix + 1, _T("共享XYD平台的XY"));	// 独立固定安装，只针对XYD、UVW、DXY平台
	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformShareXY + 1, _T("独立X轴"));			// 共享运动平台的XY，即相机随着拾取头和运动平台一起XY方向运动
	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepX + 1, _T("独立Y轴"));				// 独立X轴，只针对XYD、UVW、DXY平台
	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepY + 1, _T("独立Y轴"));				// 独立Y轴，只针对XYD、UVW、DXY平台
	//strArrayPickCamPlatformTypeVal.SetAtGrow(ePickCamPlatformSepXY + 1, _T("独立XY轴"));			// 独立XY轴，只针对XYD、UVW、DXY平台


	// 相机平台类型
	CString strCamPlatformTypeName = _T("相机平台类型");
	CStringArray strCamPlatformTypeVal;
	strCamPlatformTypeVal.SetSize(4);
	strCamPlatformTypeVal.SetAtGrow(eCamPlatformSepFix , _T("独立固定安装"));			
	strCamPlatformTypeVal.SetAtGrow(eCamPlatformSepX, _T("独立X轴"));	
	strCamPlatformTypeVal.SetAtGrow(eCamPlatformSepXY, _T("独立XY轴"));			
	strCamPlatformTypeVal.SetAtGrow(eCamPlatformShareX, _T("共享XY轴"));				

	// 窗口显示模式 m_nShowMode
	CString strnShowModeName = _T("窗口显示模式");
	CStringArray strArraynShowModeVal;
	strArraynShowModeVal.SetSize(2);
	strArraynShowModeVal.SetAtGrow(0, _T("按位置数目显示"));				// 	0,	
	strArraynShowModeVal.SetAtGrow(1, _T("按相机数目显示"));				// 	1,	

	// 启用大小品种
	CString strbProductFaChdName = _T("启用大小品种");
	CStringArray strArrayProductFaChdVal;
	strArrayProductFaChdVal.SetSize(2);
	strArrayProductFaChdVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArrayProductFaChdVal.SetAtGrow(1, _T("是"));				// 	true,	

	// 启用单产品载入
	CString strbLoadSingleProductSearchToolEnableName = _T("启用单产品载入");
	CStringArray strLoadSingleProductSearchToolEnableVal;
	strLoadSingleProductSearchToolEnableVal.SetSize(2);
	strLoadSingleProductSearchToolEnableVal.SetAtGrow(0, _T("否"));				// 	false,	
	strLoadSingleProductSearchToolEnableVal.SetAtGrow(1, _T("是"));				// 	true,	

	// 单个大产品载入
	CString strbLoadSingleProductSearchToolEnableFaChdName = _T("单个大产品载入");
	CStringArray strbLoadSingleProductSearchToolEnableFaChdVal;
	strbLoadSingleProductSearchToolEnableFaChdVal.SetSize(2);
	strbLoadSingleProductSearchToolEnableFaChdVal.SetAtGrow(0, _T("否"));				// 	false,	
	strbLoadSingleProductSearchToolEnableFaChdVal.SetAtGrow(1, _T("是"));				// 	true,

	// 启用相机共享
	CString strbShareCamSystemName = _T("启用相机共享");
	CStringArray strArraybShareCamSystemVal;
	strArraybShareCamSystemVal.SetSize(2);
	strArraybShareCamSystemVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybShareCamSystemVal.SetAtGrow(1, _T("是"));				// 	true,	

	// 启用目标标定不关联
	CString strbTarObjCamSepAutoCalibTarName = _T("启用目标标定不关联");
	CStringArray strArraybTarObjCamSepAutoCalibTarVal;
	strArraybTarObjCamSepAutoCalibTarVal.SetSize(2);
	strArraybTarObjCamSepAutoCalibTarVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybTarObjCamSepAutoCalibTarVal.SetAtGrow(1, _T("是"));				// 	true,	

	// 启用单产品扩展
	CString strbEnableMultiCalibExtensionName = _T("启用单产品扩展");
	CStringArray strArraybEnableMultiCalibExtensionVal;
	strArraybEnableMultiCalibExtensionVal.SetSize(2);
	strArraybEnableMultiCalibExtensionVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybEnableMultiCalibExtensionVal.SetAtGrow(1, _T("是"));				// 	true,

	// 扩展数量
	CString strnMultiCalibExtensionMaxNumName = _T("扩展数量");


	// 对象标定时是否选择一个位置为基准
	CString strbEnableObjectCalibBenchName = _T("启用对象选定标定基准位置");
	CStringArray strArraybEnableObjectCalibBenchVal;
	strArraybEnableObjectCalibBenchVal.SetSize(2);
	strArraybEnableObjectCalibBenchVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybEnableObjectCalibBenchVal.SetAtGrow(1, _T("是"));				// 	true,

	// 基准位置序号 
	CString strnObjectCalibBenchIndexName = _T("基准位置序号");

	// 跨控制器对位-目标端工位
	CString strbTargetInDifferentComputerName = _T("跨控制器对位-目标端工位");
	CStringArray strArraybTargetInDifferentComputerVal;
	strArraybTargetInDifferentComputerVal.SetSize(2);
	strArraybTargetInDifferentComputerVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybTargetInDifferentComputerVal.SetAtGrow(1, _T("是"));				// 	true,
	
	// 跨控制器对位-对象端工位
	CString strbObjectInDifferentComputerName = _T("跨控制器对位-对象端工位");
	CStringArray strArraybObjectInDifferentComputerVal;
	strArraybObjectInDifferentComputerVal.SetSize(2);
	strArraybObjectInDifferentComputerVal.SetAtGrow(0, _T("否"));				// 	false,	
	strArraybObjectInDifferentComputerVal.SetAtGrow(1, _T("是"));				// 	true,

	//////////////////////////////////////////////////////////////////////////
	m_ctrlProductTree.DeleteAllItems();
	m_strSimpleSystemInfo += _T("");
	int nLevel = 0;
	int nSpaceStep = 2;
	int nLevelIndex1 = 0;
	int nLevelIndex2 = 0;
	int nLevelIndex3 = 0;

	HTREEITEM hParentOfLevelOne = NULL, hParentOfLevelTwo = NULL, hParentOfLevelThree = NULL;
	CString strName;
	CString strValue;

	CSystemConfigure& systemConfig = m_pParent->m_SystemConfigure; 

	// Level 1 tree item: 产品名
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = NULL;
	tvInsert.item.mask = TVIF_TEXT | TVIF_STATE;
	nLevel = 1;
	nLevelIndex1 = 0;
	// 软件版本
	strName.Format(_T("%s"), strSoftwareVersionName);
	strValue.Format(_T("%s：%s"), strName, systemConfig.GetSoftwareVersion() + strWin);
	m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex1) + strValue + _T("\n");
	tvInsert.item.state = INDEXTOSTATEIMAGEMASK(1);
	tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
	tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
	tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
	strValue.ReleaseBuffer();

	// 编译时间
	strName.Format(_T("%s"), strBuildTimeName);
	strValue.Format(_T("%s：%s"), strName, GetSystemBuildTime());
	m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex1) + strValue + _T("\n");
	tvInsert.item.state = INDEXTOSTATEIMAGEMASK(1);
	tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
	tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
	tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
	strValue.ReleaseBuffer();

	int nSystemIndex = 0;
	int nPlatformIndex = 0;
	CSystemInfo systemInfo;
	SysPlatformInfo sysPlatformInfo;
	for (nSystemIndex = 0; nSystemIndex < systemConfig.GetSystemNum(); nSystemIndex++)
	{
		systemConfig.GetSystemInfo(nSystemIndex, systemInfo);
		nLevel = 1;
	
		// 系统名称
		tvInsert.hParent = NULL;
		strName.Format(_T("%s%d"), strSystemNameName, nSystemIndex + 1);
		strValue.Format(_T("%s：%s"), strName, systemInfo.m_strSystemName);
		m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex1) + strValue + _T("\n");
		if (nSystemIndex == systemConfig.GetCurSystemIndex())
		{
			tvInsert.item.state = TVIS_BOLD | INDEXTOSTATEIMAGEMASK(10);
			tvInsert.item.stateMask = TVIS_BOLD | TVIS_STATEIMAGEMASK;
		}
		else
		{
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(1);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
		}
		tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
		hParentOfLevelOne = m_ctrlProductTree.InsertItem(&tvInsert);
		strValue.ReleaseBuffer();

		// Level 2 tree items
		nLevelIndex2 = 0;
		// 平台信息
		for (nPlatformIndex = 0; nPlatformIndex < systemInfo.m_nSystemPlatformNum; nPlatformIndex++)
		{
			nLevel = 2;
			
			systemConfig.GetSysPlatformInfo(nSystemIndex, nPlatformIndex, sysPlatformInfo);

			// 平台名称
			tvInsert.hParent = hParentOfLevelOne;
			//tvInsert.hInsertAfter = tvInsert.hInsertAfter;
			strName.Format(_T("%s%d"), strPlatformNameName, nPlatformIndex + 1);
			strValue.Format(_T("%s：%s"), strName, sysPlatformInfo.m_strPlatformName);
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex2) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(5);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			hParentOfLevelTwo = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// Level 3 tree items
			nLevel = 3;
			nLevelIndex3 = 0;
			// 平台类型
			tvInsert.hParent = hParentOfLevelTwo;
			tvInsert.hInsertAfter = hParentOfLevelTwo;
			strName.Format(_T("%s"), strPlatformTypeName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayPlatformTypeVal, sysPlatformInfo.m_eMidPlatformType+1));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 相机数量
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strCamNumName);
			strValue.Format(_T("%s：%d"), strName, sysPlatformInfo.m_nCamNum);
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 位置数量
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strPositionNumName);
			strValue.Format(_T("%s：%d"), strName, sysPlatformInfo.m_nPositionNum);
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();


			//// 通信接口
			//tvInsert.hParent = hParentOfLevelTwo;
			//strName.Format(_T("%s"), strCommTypeName);
			//strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayCommTypeVal, sysPlatformInfo.m_eCommType+1));
			//m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			//tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			//tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			//tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			//tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			//strValue.ReleaseBuffer();

			//// 通信协议
			//tvInsert.hParent = hParentOfLevelTwo;
			//strName.Format(_T("%s"), strProtocolTypeName);
			//strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayProtocolTypeVal, sysPlatformInfo.m_eProtocolType+1));
			//m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			//tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			//tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			//tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			//tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			//strValue.ReleaseBuffer();

			//// 寄存器类型
			//tvInsert.hParent = hParentOfLevelTwo;
			//strName.Format(_T("%s"), strRegiterTypeName);
			//strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayRegiterTypeVal, sysPlatformInfo.m_bRegiterType));
			//m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			//tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			//tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			//tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			//tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			//strValue.ReleaseBuffer();

			// 共享通信
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strShareCommEnableName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strShareCommEnableVal, sysPlatformInfo.m_bShareCommEnable));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 启用共享通信才显示
			if(sysPlatformInfo.m_bShareCommEnable)
			{
				// 共享通信接口号
				tvInsert.hParent = hParentOfLevelTwo;
				strName.Format(_T("%s"), strShareCommIndexName);
				strValue.Format(_T("%s：%d"), strName, sysPlatformInfo.m_nShareCommIndex);
				m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
				tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
				tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
				tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
				tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
				strValue.ReleaseBuffer();

				// 共享通信并行
				tvInsert.hParent = hParentOfLevelTwo;
				strName.Format(_T("%s"), strShareCommPlatformParallelName);
				strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strShareCommPlatformParallelVal, sysPlatformInfo.m_bShareCommPlatformParallel));
				m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
				tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
				tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
				tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
				tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
				strValue.ReleaseBuffer();
			}

			// 目标对象相机独立
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strTargetObjectCamSeparateName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayTargetObjectCamSeparateVal, sysPlatformInfo.m_bTargetObjectCamSeparate));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 目标相机关联映射类型
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strTarObjCamSepNoAxisCalibName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayTarObjCamSepNoAxisCalibVal, sysPlatformInfo.m_bTarObjCamSepNoAxisCalib));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 目标相机关联映射(使用标定板)
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strTargetCalibUseBoardName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayTargetCalibUseBoardVal, sysPlatformInfo.m_bTargetCalibUseBoard));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();
			
			// 拾取功能
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbPlatformPickName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayPlatformPickVal, sysPlatformInfo.m_bPlatformPick));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();
			if(sysPlatformInfo.m_bPlatformPick)
			{
				// 拾取平台类型
				tvInsert.hParent = hParentOfLevelTwo;
				strName.Format(_T("%s"), strbPickCamPlatformTypeName);
				strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayPickCamPlatformTypeVal, sysPlatformInfo.m_ePickCamPlatformType));
				m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
				tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
				tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
				tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
				tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
				strValue.ReleaseBuffer();
			}
			

			// 相机平台类型
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strCamPlatformTypeName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strCamPlatformTypeVal, sysPlatformInfo.m_eCamPlatformType));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 窗口显示模式
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strnShowModeName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraynShowModeVal, sysPlatformInfo.m_nShowMode));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);



			// 启用大小品种
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbProductFaChdName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArrayProductFaChdVal, sysPlatformInfo.m_bProductFaChd));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 启用单产品载入
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbLoadSingleProductSearchToolEnableName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strLoadSingleProductSearchToolEnableVal, sysPlatformInfo.m_bLoadSingleProductSearchToolEnable));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 单个大产品载入
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbLoadSingleProductSearchToolEnableFaChdName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strbLoadSingleProductSearchToolEnableFaChdVal, sysPlatformInfo.m_bLoadSingleProductSearchToolEnableFaChd));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();


			// 启用相机共享
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbShareCamSystemName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybShareCamSystemVal, sysPlatformInfo.m_bShareCamSystem));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 启用目标标定不关联
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbTarObjCamSepAutoCalibTarName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybTarObjCamSepAutoCalibTarVal, sysPlatformInfo.m_bTarObjCamSepAutoCalibTar));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 启用单产品扩展
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbEnableMultiCalibExtensionName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybEnableMultiCalibExtensionVal, sysPlatformInfo.m_bEnableMultiCalibExtension));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			if(sysPlatformInfo.m_bEnableMultiCalibExtension)
			{
				// 扩展数量
				tvInsert.hParent = hParentOfLevelTwo;
				strName.Format(_T("%s"), strnMultiCalibExtensionMaxNumName);
				strValue.Format(_T("%s：%d"), strName, sysPlatformInfo.m_nMultiCalibExtensionMaxNum);
				m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
				tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
				tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
				tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
				tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
				strValue.ReleaseBuffer();
			}
			

			// 对象标定时是否选择一个位置为基准
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbEnableObjectCalibBenchName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybEnableObjectCalibBenchVal, sysPlatformInfo.m_bEnableObjectCalibBench));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			if(sysPlatformInfo.m_bEnableObjectCalibBench)
			{
				// 基准位置序号
				tvInsert.hParent = hParentOfLevelTwo;
				strName.Format(_T("%s"), strnObjectCalibBenchIndexName);
				strValue.Format(_T("%s：%d"), strName, sysPlatformInfo.m_nObjectCalibBenchIndex);
				m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
				tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
				tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
				tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
				tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
				strValue.ReleaseBuffer();
			}

			// 跨控制器对位-目标端工位
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbTargetInDifferentComputerName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybTargetInDifferentComputerVal, sysPlatformInfo.m_bTargetInDifferentComputer));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();

			// 跨控制器对位-对象端工位
			tvInsert.hParent = hParentOfLevelTwo;
			strName.Format(_T("%s"), strbObjectInDifferentComputerName);
			strValue.Format(_T("%s：%s"), strName, GetSysInfoString(&strArraybObjectInDifferentComputerVal, sysPlatformInfo.m_bObjectInDifferentComputer));
			m_strSimpleSystemInfo += GetLevelString(nLevel,nSpaceStep,++nLevelIndex3) + strValue + _T("\n");
			tvInsert.item.state = INDEXTOSTATEIMAGEMASK(9);
			tvInsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvInsert.item.pszText = strValue.GetBuffer(strValue.GetLength());
			tvInsert.hInsertAfter = m_ctrlProductTree.InsertItem(&tvInsert);
			strValue.ReleaseBuffer();
		}
	}


}

void CDlgSystemInfo::OnBnClickedBtnSaveSystemInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	strLog.Format(_T("点击[保存详细系统信息]"));
	g_opLogFile.SaveLogText(strLog,3);
	CString strDir;
	CString strTitle = _T("");
	strDir = GetOpenDir(GetSafeHwnd(), strTitle);
	
	CString strFileName;
	strFileName.Format(_T("\\SysConfigs(%s).xml"), m_pParent->m_SystemConfigure.GetSoftwareVersion());
	CString strFilePath = strDir + strFileName;
	m_pParent->m_SystemConfigure.SaveSystemConfigInfo(strFilePath);
	strLog.Format(_T("保存详细系统信息到%s\n"),strFilePath);
	g_opLogFile.SaveLogText(strLog,4);
	strLog.Format(_T("退出[保存详细系统信息]"));
	g_opLogFile.SaveLogText(strLog,3);
}


void CDlgSystemInfo::OnBnClickedBtnSaveSimpleSystemInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (1)
	{	
		CString strLog;
		strLog.Format(_T("点击[保存系统信息]"));
		g_opLogFile.SaveLogText(strLog,3);
		CString strDir;
		CString strTitle = _T("");
		strDir = GetOpenDir(GetSafeHwnd(), strTitle);

		CString strFileName;
		strFileName.Format(_T("\\SimpleSysConfigs(%s).txt"), m_pParent->m_SystemConfigure.GetSoftwareVersion());
		CString strFilePath = strDir + strFileName;
		FILE *stream;
		stream = fopen(strFilePath, _T("w+"));
		if (stream != NULL)
		{
			fwrite(m_strSimpleSystemInfo, sizeof(TCHAR), m_strSimpleSystemInfo.GetLength(), stream);
			fclose(stream);	
			strLog.Format(_T("保存系统信息到%s\n"),strFilePath);
			g_opLogFile.SaveLogText(strLog,4);
		}
		strLog.Format(_T("退出[保存系统信息]"));
		g_opLogFile.SaveLogText(strLog,3);
	}

}


void CDlgSystemInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	//strLog.Format(_T("点击[确定]"));
	//g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("点击[确定]退出[系统信息]"));
	g_opLogFile.SaveLogText(strLog,2);

	CDialog::OnOK();
}


void CDlgSystemInfo::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLog;
	//strLog.Format(_T("点击[取消]"));
	//g_opLogFile.SaveLogText(strLog,3);
	strLog.Format(_T("点击[取消]退出[系统信息]"));
	g_opLogFile.SaveLogText(strLog,2);
	CDialog::OnCancel();
}
