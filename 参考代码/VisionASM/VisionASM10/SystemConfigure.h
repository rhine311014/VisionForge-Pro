// SystemConfigure.h: interface for the CSystemConfigure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMCONFIGURE_H__76F6CA2D_DB90_4EFD_A5E1_F949AC5CB1B2__INCLUDED_)
#define AFX_SYSTEMCONFIGURE_H__76F6CA2D_DB90_4EFD_A5E1_F949AC5CB1B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vsAlignToolDef.h"
#include "vsBaseAlignTool.h"
#include "svXMLConfigurator.h"
#include "SysPlatformInfo.h"

// 系统信息
// 说明：该结构为了在一个软件中兼容几种配置，比如兼容以下2个系统：
//       可选系统1（3相机系统）：平台1为单相机上料，平台2为双相机预压对位；
//       可选系统2（5相机系统)：平台1为单相机上料，平台2为4相机预压对位；
typedef struct tagSystemInfo
{
	CString m_strSystemName;	// 系统名称
	int m_nSystemIndex;			// 系统编号（0，1，2，……）
	int m_nSystemOffsetIndex;	// 系统偏移量编号
	int m_nSystemPlatformNum;	// 系统包含的平台数
	tagSystemInfo()
	{
		m_strSystemName = _T("系统1");
		m_nSystemIndex = 0;
		m_nSystemOffsetIndex = 0;
		m_nSystemPlatformNum = 1;
	}
} CSystemInfo;

class CSystemConfigure
{
public:
	CSystemConfigure();
	virtual ~CSystemConfigure();

	BOOL LoadSystemConfigInfo();	// 加载所有系统配置信息，暂时废弃
	BOOL SaveSystemConfigInfo();	// 保存所有系统配置信息，暂时废弃

	BOOL LoadSystemConfigInfo(CString strFilePathName);	// 加载所有系统配置信息
	BOOL SaveSystemConfigInfo(CString strFilePathName);	// 保存所有系统配置信息到指定目录

	CString	GetSoftwareVersion();			// 软件版本 
	CString GetSoftwareStandardVersion();	// 标准软件版本
	CString GetSoftwareProjectVersion();	// 项目软件版本


	// 获取系统数量
	int GetSystemNum() const;
	// 获取系统信息
	BOOL GetSystemInfo(int nIndex, CSystemInfo& systemInfo);
	// 获取当前系统编号
	int GetCurSystemIndex();


	// 获取平台数
	int GetPlatformNum();
	// 设置平台数
	void SetPlatformNum(int nNum);

	// 获取指定的平台配置信息（从0开始）
	BOOL GetSysPlatformInfo(int nIndex, SysPlatformInfo& sysPlatformInfo);
	// 设置指定的平台配置信息（从0开始）
	BOOL SetSysPlatformInfo(int nIndex, SysPlatformInfo sysPlatformInfo);

	// 获取指定系统的指定平台配置信息
	BOOL GetSysPlatformInfo(int nSystemIndex, int nPlatformIndex, SysPlatformInfo& sysPlatformInfo);

	CString GetSavePath() const { return m_strXMLFilePath; }
	void SetSavePath(LPCTSTR lpszPath) { m_strXMLFilePath = lpszPath; }

private:
	//BOOL LoadPlatformInfo(LPCTSTR lpszPlatformName, SysPlatformInfo& sysPlatformInfo);	//暂时废弃
	//BOOL SavePlatformInfo(LPCTSTR lpszPlatformName, SysPlatformInfo sysPlatformInfo);	//暂时废弃
	void ConfigCustomPlatformInfo();			// 根据项目，进行定制性的初始化

	std::vector<CSystemInfo> m_vSystemInfo;		// 系统信息（包含可兼容的系统）
	int  m_nSystemNum;							// 系统数量（包含可兼容的系统数量）
	int  m_nCurSystemIndex;						// 当前系统的序号

	CString	m_strSoftwareVersion;				// 软件版本 = 标准软件版本 + "." + 项目软件版本；
	CString m_strSoftwareStandardVersion;		// 标准软件版本（例子：1.97.12,"大版本.小版本.标准软件修改版本"）
	CString m_strSoftwareProjectVersion;		// 项目软件版本(例子：1.1.5.1.1，"客户公司编号.客户设备编号.项目软件修改版本,组号（前端项目分组编号），组内编号（具体配置软件人员的组内编号）")
	

	std::vector<SysPlatformInfo> m_vSysPlatformInfo;
	std::vector<DlgSearchToolInfo> m_vDlgSearchToolInfo;
	int m_nPlatformNum;
	vcXMLConfigurator m_XMLConfigurator;
	CString m_strXMLFilePath;
};

#endif // !defined(AFX_SYSTEMCONFIGURE_H__76F6CA2D_DB90_4EFD_A5E1_F949AC5CB1B2__INCLUDED_)
