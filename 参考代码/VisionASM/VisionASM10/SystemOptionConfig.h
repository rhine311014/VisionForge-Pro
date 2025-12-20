// SystemOptionConfig.h: interface for the CSystemOptionConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMCONFIG_H__EEDA05F9_919B_47C8_91F6_E259FCBA8BF8__INCLUDED_)
#define AFX_SYSTEMCONFIG_H__EEDA05F9_919B_47C8_91F6_E259FCBA8BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vsAlignToolDef.h"
#define MaxPlatformCustomNameNum	6	// 自定义平台名称数量
#define MAXLightControl 8
#define MAXLightChannelNum 8

struct SearchResultCrossSize
{
	int nLineWidth;
	int nLineLength;
	COLORREF nLineColor;
	double nRotation;	//旋转角度

	int nTrainRectLineWidth;
	COLORREF nTrainRectLineColor;

	int nOtherUILineWidth;
	COLORREF nOtherGuiLineColor;

	BOOL m_bShowDefortGui;
	BOOL m_bShowOtherGui;
	BOOL m_bShowRectGui;

	SearchResultCrossSize()
	{
		nLineWidth = 1;
		nLineLength = 100;
		nLineColor = RGB(0,255,0);//默认绿色
		nRotation = 0;


		nTrainRectLineWidth = 1;
		nTrainRectLineColor = RGB(0, 0, 255); // 默认蓝色 

		nOtherUILineWidth = 1;
		nOtherGuiLineColor = RGB(0, 0, 255); // 默认蓝色 

		m_bShowDefortGui = TRUE;
		m_bShowOtherGui = FALSE;
		m_bShowRectGui = FALSE;
	}
};

struct AlignOffsetByAreaGui
{
	int m_nLineWidth;				// 正常线宽
	int m_nLightLineWidth;		// 高亮线宽
	COLORREF m_clrNormal;		// 正常颜色
	COLORREF m_clrLight;		// 高亮颜色
	AlignOffsetByAreaGui()
	{
		m_nLineWidth = 1;
		m_nLightLineWidth = 2;
		m_clrNormal = RGB(0, 0, 255);
		m_clrLight = RGB(255, 170, 0);
	}
};

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

// 光源控制器参数
typedef struct tagLightCtrlParam
{
	int m_nCommType;    // 与光源控制器通信类型 0-串口; 1-网口;
	int m_nSerialPort;	// 串口号
	int m_nBaundRate;   // 波特率
	unsigned char m_szIP[4];	// IP地址
	int m_nConnectTimeOut; // 链接超时时间(ms)
	int m_nCommTimeOut;  // 通信超时时间(ms)
	int m_nPort;        // 网络端口
	int m_nLightType;   // 光源控制类型
	int m_nMaxChannels; // 最大通道数
	int m_nChannelValue[MAXLightChannelNum]; // 每个通道对应的亮度值
	BOOL m_bChannelIsOpen[MAXLightChannelNum]; // 每个通道是否处于打开状态, 有的光源控制器不支持该功能
	CString m_strName[MAXLightChannelNum]; // Summer0214 每个通道的名称

	tagLightCtrlParam()
	{
		m_nCommType = 0;
		m_nSerialPort = 0;
		m_nBaundRate = 19200;
		m_szIP[0] = 192;
		m_szIP[1] = 168;
		m_szIP[2] = 0;
		m_szIP[3] = 10;
		m_nPort = 2000;
		m_nConnectTimeOut = 1000;
		m_nCommTimeOut = 100;
		m_nLightType = -1;
		m_nMaxChannels = 8;
		for (int i =0; i < MAXLightChannelNum; i++)
		{
			m_nChannelValue[i] = 0;

			m_bChannelIsOpen[i] = FALSE;

			CString str;
			str.Format(_T("光源%d"),i+1);
			m_strName[i] = str;
		}
	}

	tagLightCtrlParam(const tagLightCtrlParam& rhs)
	{
		m_nCommType = rhs.m_nCommType;
		m_nSerialPort = rhs.m_nSerialPort;
		m_nBaundRate = rhs.m_nBaundRate;
		m_szIP[0] = rhs.m_szIP[0];
		m_szIP[1] = rhs.m_szIP[1];
		m_szIP[2] = rhs.m_szIP[2];
		m_szIP[3] = rhs.m_szIP[3];
		m_nPort = rhs.m_nPort;
		m_nConnectTimeOut = rhs.m_nConnectTimeOut;
		m_nCommTimeOut = rhs.m_nCommTimeOut;
		m_nLightType = rhs.m_nLightType;
		m_nMaxChannels = rhs.m_nMaxChannels;
		for (int i =0; i < MAXLightChannelNum; i++)
		{
			m_nChannelValue[i] = rhs.m_nChannelValue[i];
			m_bChannelIsOpen[i] = rhs.m_bChannelIsOpen[i];
			m_strName[i] = rhs.m_strName[i];
		}
	}


	tagLightCtrlParam& operator=(const tagLightCtrlParam& rhs)
	{
		m_nCommType = rhs.m_nCommType;
		m_nSerialPort = rhs.m_nSerialPort;
		m_nBaundRate = rhs.m_nBaundRate;
		m_szIP[0] = rhs.m_szIP[0];
		m_szIP[1] = rhs.m_szIP[1];
		m_szIP[2] = rhs.m_szIP[2];
		m_szIP[3] = rhs.m_szIP[3];
		m_nPort = rhs.m_nPort;
		m_nConnectTimeOut = rhs.m_nConnectTimeOut;
		m_nCommTimeOut = rhs.m_nCommTimeOut;
		m_nLightType = rhs.m_nLightType;
		m_nMaxChannels = rhs.m_nMaxChannels;
		for (int i =0; i < MAXLightChannelNum; i++)
		{
			m_nChannelValue[i] = rhs.m_nChannelValue[i];
			m_bChannelIsOpen[i] = rhs.m_bChannelIsOpen[i];
			m_strName[i] = rhs.m_strName[i];
		}
		return *this;
	}

	bool operator!=(const tagLightCtrlParam& rhs)
	{
		if (m_nCommType != rhs.m_nCommType) return true;
		if (m_nSerialPort != rhs.m_nSerialPort) return true;
		if (m_nBaundRate != rhs.m_nBaundRate) return true;
		if (m_szIP[0] != rhs.m_szIP[0]) return true;
		if (m_szIP[1] != rhs.m_szIP[1]) return true;
		if (m_szIP[2] != rhs.m_szIP[2]) return true;
		if (m_szIP[3] != rhs.m_szIP[3]) return true;
		if (m_nPort != rhs.m_nPort) return true;
		if (m_nConnectTimeOut != rhs.m_nConnectTimeOut) return true;
		if (m_nCommTimeOut != rhs.m_nCommTimeOut) return true;
		if (m_nLightType != rhs.m_nLightType) return true;
		if (m_nMaxChannels != rhs.m_nMaxChannels) return true;
		for (int i =0; i < MAXLightChannelNum; i++)
		{
			if(m_nChannelValue[i] != rhs.m_nChannelValue[i])
			{
				return true;
			}

			if (m_bChannelIsOpen[i] != rhs.m_bChannelIsOpen[i])
			{
				return true;
			}

			if (m_strName[i] != rhs.m_strName[i])
			{
				return true;
			}
		}
		return false;
	}

	bool operator==(const tagLightCtrlParam& rhs)
	{
		return !(*this != rhs);
	}

}LightCtrlParam;

// 光源控制配置
typedef struct tagSystemOptionLightControlConfig
{
	bool		   m_bLightCtrlUsed[MAXLightControl];	// 标识光源控制是否存在并使用
	LightCtrlParam m_lightCtrlParam[MAXLightControl];   // 光源控制器参数
	CString        m_strLightCtrlName[MAXLightControl];  //光源控制器名称

	tagSystemOptionLightControlConfig()
	{
		for (int i = 0; i < MAXLightControl; i++)
		{
			m_bLightCtrlUsed[i] = false;

			CString str;
			str.Format(_T("光源控制器%d"),i+1);
			m_strLightCtrlName[i] = str;
		}
	}

	tagSystemOptionLightControlConfig(const tagSystemOptionLightControlConfig& rhs)
	{
		for (int i = 0; i < MAXLightControl; i++)
		{
			m_bLightCtrlUsed[i] = rhs.m_bLightCtrlUsed[i];
			m_lightCtrlParam[i] = rhs.m_lightCtrlParam[i];
			m_strLightCtrlName[i] = rhs.m_strLightCtrlName[i];
		}
	}

	tagSystemOptionLightControlConfig& operator=(const tagSystemOptionLightControlConfig& rhs)
	{
		for (int i = 0; i < MAXLightControl; i++)
		{
			m_bLightCtrlUsed[i] = rhs.m_bLightCtrlUsed[i];
			m_lightCtrlParam[i] = rhs.m_lightCtrlParam[i];
			m_strLightCtrlName[i] = rhs.m_strLightCtrlName[i];
		}

		return *this;
	}

	bool operator!=(const tagSystemOptionLightControlConfig& rhs)
	{
		for (int i = 0; i < MAXLightControl; i++)
		{
			if (m_bLightCtrlUsed[i] != rhs.m_bLightCtrlUsed[i])
			{
				return true;	
			}

			if (m_lightCtrlParam[i] != rhs.m_lightCtrlParam[i])
			{
				return true;
			}
		}	

		return false;
	}

	bool operator==(const tagSystemOptionLightControlConfig& rhs)
	{
		return !(*this != rhs);
	}

}SystemOptionLightConfig;


// 系统参数
typedef struct tagSystemOptionConfig
{
	CString m_strSysConfigFilePath;		// 系统配置文件全路径
	CString	m_strAdminPassword;			// 管理员密码
	CString	m_strAdminPassword2;		// 管理员密码
	CString m_strLastRunDate;           // 最后一次运行日期
	CString m_strSysExpireDate;         // 系统过期日期
	CString m_strSysRegID;              // 系统注册ID
	CString m_strRestDate;              // 剩余时间
	CString m_strRestSecond;              // 剩余时间
	LANGID	m_lgidLanguage;				// 语言：暂支持简体中文，英文，繁体中文
	BOOL	m_bSysKeyboardEnabled;		// 启用系统虚拟键盘
	BOOL	m_bStartUpShowCamSNSetting;		// 

	int		m_nCurSystemIndex;			// 当前系统的编号(0、1、2……)
	
	//int		m_nCameraMode;				// 相机模式：0，4：3；1，3：4；2，6：5；3，5：6
	int		m_nDisplayMode;				// 图像显示模式：0，实时显示；1，
	int		m_nDisplayIndex;			// 实时显示时显示第n帧
	int		m_nCommXYUnitExp;			// 串口XY单位幂4
	int		m_nCommDUnitExp;			// 串口D单位幂5
	int		m_nCommDataLenType;			// 串口数据长度：0：实际长度；1：固定长度；
	int		m_nCommDelay;				// 发送延时
	int		m_nCommTimeOut;				// 串口超时
	int     m_nCommReSendTimes;			// 串口重复次数
	int		m_nShowRunCommandDetail;	// 命令记录：0，不记录命令；

	int		m_nMinute;				    // 开通权限起始时间
	int		m_nMinuteFPMI;				    // 权限时间

	BOOL	m_bMainframeSetAlignParam;	// 允许主界面设置对位参数
	BOOL    m_bShowSepTargetPos;        // 目标对象分离时显示映射目标位置

	BOOL    m_bMainframeSetBendParam;   //主界面允许设置折弯参数

	// 串口接收延时
	BOOL    m_bCommRecDelayEnable;		// 是否通信延时
	int		m_nCommRecDelayTime;		// 通信延时时间毫秒
	//int		m_nShowMode;				// 主界面显示方式：2行3列，3行2列

	//BOOL    m_bShowProductInfo;         //显示产品信息
	//BOOL    m_bShowOffsetDirection;         //主界面显示补偿方向

	//BOOL    m_bShowSearchRect;            //主界面显示搜索结果矩形
	//BOOL	m_bShowCircleTool;			// 显示找角工具的两条直线
	//BOOL	m_bShowCornerTool;			// 显示找角工具的两条直线
 //   BOOL    m_bShowLineTool;            // 显示找线工具的直线

	BOOL	m_bChange2um;
    BOOL	m_bChangeProductSize2um;
	// 将数据文件和VDB图片分别整合到一个文件夹下
	BOOL m_bAllSteoage;

	BOOL	m_bShowSearchResult;		// 主界面显示定位OK、NG

	BOOL	m_bShowGamma;				// 显示Gamma设置
	BOOL	m_bCheckModeOnly;			// 产品尺寸只有检查模式，没有输入模式
	BOOL	m_bCheckFPMI;			// 是否启用FPMI指令

	// 目标和对象相机分离时，关联标定目标位置所用的模板是来自目标模板还是对象模板。
	// 0：用目标模板关联标定目标位置，默认值
	// 1：用对象模板关联标定目标位置
	// 2: 用标定模板关联标定目标位置
	// 3: 用关联标定模板关联标定目标位置
	int     m_nSepCamCalibTargetModelType;   

	BOOL m_bGetSysResourceUse;//是否在主界面显示统计信息
	BOOL m_bThreValid;     //阈值是否有效
	double m_dMaxCpuUseThre;
	double m_dMaxDiskUseThre;
	double m_dMaxMemUseThre;
	int m_nMaxHandleUseThre;
	int m_nMaxGDIUseThre;

	double m_dCpuThreMaxValue;
	double m_dDiskThreMaxValue;
	double m_dMemThreMaxValue;
	int m_nHandleThreMaxValue;
	int m_nGDIThreMaxValue;
	
	PlatformType m_eTempPlatformType;		// 平台类型
	CPlatformInfo	*m_pTempPlatformInfo;	// 平台信息

	BOOL m_bPlatformCustomNameEnable;
	CString m_strPlatformCustomName[MaxPlatformCustomNameNum];
	SearchResultCrossSize m_TarSearchResultCrossSize;
	SearchResultCrossSize m_ObjSearchResultCrossSize;

	AlignOffsetByAreaGui m_AlignOffsetAreaGui;
	BOOL m_bIsShowCalibrateEvaluate; // 标定结果显示为标定评价界面

	int m_nPlatformLayoutType;
	
	bool m_bShowFullScreen;

	int m_nLoginTimeSet;// 正确输入密码后 m_nLoginTimeSet s秒内再次登录不用输入密码

	int m_bSaveVDBPostMessage;	// 通过消息发送来存储VDB个数文件;

	// 光源配置
	SystemOptionLightConfig m_sysOptLightConfig;

	tagSystemOptionConfig();
// 	{
// 		m_strSysConfigFilePath = _T("");
// 		m_strAdminPassword = _T("123456");
// 		m_lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
// 		m_bSysKeyboardEnabled = FALSE;
// 		m_bStartUpShowCamSNSetting = FALSE;
// 
// 		m_nCameraMode = 0;					// 相机模式：0，4：3；1，3：4；2，6：5；3，5：6
// 		m_nDisplayMode = 0;					// 图像显示模式：0，实时显示；1，帧显示
// 		m_nCommXYUnitExp = 4;				// 串口XY单位幂4
// 		m_nCommDUnitExp = 5;				// 串口D单位幂5
// 		m_nCommDataLenType = 0;				// 串口数据长度：0：实际长度；1：固定长度；
// 		m_nCommTimeOut = 30;				// 串口超时
// 		m_nCommReSendTimes = 3;				// 串口重复次数
// 		m_nShowRunCommandDetail = 0;		// 命令记录：0，不记录命令；
// 		m_bAskStagePosEachMove = TRUE;		// 每次平台移动前询问平台位置（不选择此项，对位速度加快）
// 		m_bAlignOneTime = FALSE;			// 执行一次对位
// 		m_bTargetPosSame = FALSE;			// 拍摄目标位置重复
// 		m_bObjectPosSame = FALSE;			// 拍摄对象位置重复
// 		m_bSaveAlignPos = TRUE;				// 保存对位位置
// 		m_dOffsetMaxX = 1;					// 补偿最大值X
// 		m_dOffsetMaxY = 1;					// 补偿最大值Y
// 		m_dOffsetMaxD = 0.5;				// 补偿最大值D
// 		m_dAxisMoveMaxX = 5;				// 防止撞击最大移动量X
// 		m_dAxisMoveMaxY = 5;				// 防止撞击最大移动量Y
// 		m_dAxisMoveMaxD = 5;				// 防止撞击最大移动量D
// 
// 		m_eTempPlatformType = ePlatformXY1Y2;
// 		m_pTempPlatformInfo = NULL;
// 		switch(m_eTempPlatformType)
// 		{
// 		case ePlatformXY1Y2:
// 			{
// 				m_pTempPlatformInfo = new CPlatformXY1Y2Info;
// 			}
// 			break;
// 		case ePlatformX1X2Y:
// 			{
// 				m_pTempPlatformInfo = new CPlatformX1X2YInfo;
// 			}
// 			break;
// 		default:
// 			{
// 		
// 			}
// 		}
// 
// 	}
	virtual ~tagSystemOptionConfig();
	tagSystemOptionConfig(const tagSystemOptionConfig& systemOptionConfig);
	tagSystemOptionConfig& operator = (const tagSystemOptionConfig& systemOptionConfig);
}SystemOptionConfig;


// 系统参数配置文件管理类
class CSystemOptionConfig : public SystemOptionConfig
{
public:
	CSystemOptionConfig();
	virtual ~CSystemOptionConfig();

	BOOL LoadAdminPassword();
	BOOL SaveAdminPassword();
	BOOL LoadSystemOption();
	BOOL SaveSystemOption();
};

#endif // !defined(AFX_SYSTEMCONFIG_H__EEDA05F9_919B_47C8_91F6_E259FCBA8BF8__INCLUDED_)
