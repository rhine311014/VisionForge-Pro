#include "stdafx.h"
#include "SystemCompanyInfo.h"
#include "svXMLConfigurator.h"

CSystemCompanyInfo::CSystemCompanyInfo(void)
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath = szPath;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	strPath += _T("\\Company.xml");
	m_strSysCompanyFilePath = strPath;
}


CSystemCompanyInfo::~CSystemCompanyInfo(void)
{
}

BOOL CSystemCompanyInfo::LoadCompanyInfo()
{
	BOOL bSucceed = TRUE;
	vcXMLConfigurator xml;
	int nData = 0;

	if (xml.Load(m_strSysCompanyFilePath))
	{
		xml.ResetDocElemPos();
		if (xml.FindElem(_T("Config")))
		{			
			xml.EnterElem();
			
			if (xml.FindElem(_T("Logo")))
			{
				m_strCompanyLogo = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("Title")))
			{
				m_strCompanyTitle = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			
			if (xml.FindElem(_T("SoftwareTitle")))
			{
				m_strSoftwareTitle = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("VersionDateFront")))
			{
				m_strVersionDateFront = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("VersionDateCustomEnable")))
			{
				xml.GetElemData(nData);
				m_bVersionDateCustomEnable = (BOOL)nData;
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("VersionDate")))
			{
				m_strVersionDate = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("VersionDateBack")))
			{
				m_strVersionDateBack = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}
			
			if (xml.FindElem(_T("Note")))
			{
				m_strCompanyNote = xml.GetElemData();
			}
			else
			{
				bSucceed = FALSE;
			}

			if (xml.FindElem(_T("FontSize")))
			{
				xml.GetElemData(m_nFontSize);
			}
			else
			{
				bSucceed = FALSE;
			}

			xml.LeaveElem();
		}
		else
		{
			bSucceed = FALSE;
		}
	}
	else
	{
		bSucceed = FALSE;
	}

	return bSucceed;
}

BOOL CSystemCompanyInfo::SaveCompanyInfo()
{
	vcXMLConfigurator xml;
	xml.Load(m_strSysCompanyFilePath);
	if (!xml.FindElem(_T("Config")))
	{
		xml.AddElem(_T("Config"));
	}
	xml.EnterElem();
	
	if(!xml.FindElem(_T("Logo")))
	{
		xml.AddElem(_T("Logo"));
	}
	xml.SetElemData(m_strCompanyLogo);

	if (!xml.FindElem(_T("Title")))
	{
		xml.AddElem(_T("Title"));
	}
	xml.SetElemData(m_strCompanyTitle);
	
	if(!xml.FindElem(_T("SoftwareTitle")))
	{
		xml.AddElem(_T("SoftwareTitle"));
	}
	xml.SetElemData(m_strSoftwareTitle);

	if(!xml.FindElem(_T("VersionDateFront")))
	{
		xml.AddElem(_T("VersionDateFront"));
	}
	xml.SetElemData(m_strVersionDateFront);

	if(!xml.FindElem(_T("VersionDateCustomEnable")))
	{
		xml.AddElem(_T("VersionDateCustomEnable"));
	}
	xml.SetElemData((int)m_bVersionDateCustomEnable);
	
	if(!xml.FindElem(_T("VersionDate")))
	{
		xml.AddElem(_T("VersionDate"));
	}
	xml.SetElemData(m_strVersionDate);

	if(!xml.FindElem(_T("VersionDateBack")))
	{
		xml.AddElem(_T("VersionDateBack"));
	}
	xml.SetElemData(m_strVersionDateBack);

	if(!xml.FindElem(_T("Note")))
	{
		xml.AddElem(_T("Note"));
	}
	xml.SetElemData(m_strCompanyNote);

	if(!xml.FindElem(_T("FontSize")))
	{
		xml.AddElem(_T("FontSize"));
	}
	xml.SetElemData(m_nFontSize);

	xml.LeaveElem();

	return xml.Save(m_strSysCompanyFilePath);
}
