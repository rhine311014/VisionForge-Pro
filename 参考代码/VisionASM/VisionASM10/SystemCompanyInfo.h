#pragma once

// 标题 = Logo+公司名称+软件名称+空格+版本(可自定义)+空格+注释
// 系统参数
typedef struct tagSystemCompanyInfo
{
	CString m_strSysCompanyFilePath;	// 系统配置文件全路径
	CString	m_strCompanyLogo;			// 公司Logo文件名称
	CString m_strCompanyTitle;			// 公司名称
	CString m_strSoftwareTitle;			// 软件名称
	CString m_strVersionDateFront;		// 版本日期前的站位空格
	BOOL	m_bVersionDateCustomEnable; // 自定义的版本日期
	CString m_strVersionDate;			// 版本日期
	CString m_strVersionDateBack;		// 版本日期后的占位空格
	CString	m_strCompanyNote;			// 公司注释	
	int		m_nFontSize;

	tagSystemCompanyInfo()
	{
		m_strSysCompanyFilePath = _T("");
		m_strCompanyLogo  = _T("Logo.bmp");
		m_strCompanyTitle = _T("");
		m_strSoftwareTitle = _T("视觉对位系统");
		m_strVersionDateFront = _T("");
		m_bVersionDateCustomEnable = FALSE;
		m_strVersionDate = _T("版本 V1.0.1.0.0.0");
		m_strVersionDateBack = _T("");
		m_strCompanyNote  = _T("");
		m_nFontSize = 17;
	}
}SystemCompanyInfo;


class CSystemCompanyInfo : public SystemCompanyInfo
{
public:
	CSystemCompanyInfo(void);
	~CSystemCompanyInfo(void);

	BOOL LoadCompanyInfo();
	BOOL SaveCompanyInfo();
};

