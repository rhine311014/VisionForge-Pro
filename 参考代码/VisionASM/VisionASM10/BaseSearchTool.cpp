// BaseSearchTool.cpp: implementation of the CBaseSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseSearchTool.h"
#include "svXMLConfigurator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseSearchTool::CBaseSearchTool()
{
	m_iLanguage = 1;
	m_nResultErrNumber = 0;
	m_CamWidth = 752;
	m_CamHeight = 480;
	m_strShowInspectInfo = _T("");
}

CBaseSearchTool::CBaseSearchTool(CBaseSearchTool &cBaseSearchTool)
{
    m_CamWidth = cBaseSearchTool.m_CamWidth;
    m_CamHeight = cBaseSearchTool.m_CamHeight;
}

CBaseSearchTool::~CBaseSearchTool()
{

}

/////////////////////////公共函数////////////////////////////////	
CSearchToolType CBaseSearchTool::GetSearchToolTypeFromFile(CString strDir)
{
	if (strDir == _T(""))
	{
		return eSearchToolUnknown;
	}
	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
	{
		strDir.Delete(strDir.GetLength()-1);
	}
	DWORD dwRet = GetFileAttributes(strDir);
	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
	{
		return eSearchToolUnknown;
	}
	strDir += _T('\\');

	//////////////////////////写xml文件//////////////////////////
	CString strXmlPath = strDir + _T("Model.xml");
	CSearchToolType cSearchType = eSearchToolUnknown;
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	bool bRet;
	
	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return eSearchToolUnknown;
	}
	
	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet)
	{
		return eSearchToolUnknown;
	}
	cXMLConfigurator.EnterElem();
	
	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (bRet)
	{
		cSearchType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());	
	}
	
	return cSearchType;
}

/////////////////////////公共函数////////////////////////////////	
BOOL CBaseSearchTool::SaveSearchToolTypeToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
	//    (4) 掩模图像MaskImage.bmp(视定位工具是否采用掩模训练而定)

	if (strDir == _T(""))
	{
		return false;
	}
	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
	{
		strDir.Delete(strDir.GetLength()-1);
	}
	DWORD dwRet = GetFileAttributes(strDir);	
	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
	{
		if (!CreateDirectory(strDir, NULL))
		{	
			return false;
		}
	}
	strDir += _T('\\');


	//////////////////////////写xml文件//////////////////////////
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	cXMLConfigurator.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n")); 
	cXMLConfigurator.AddElem(_T("Config"));
	cXMLConfigurator.EnterElem();
	/////////////工具类型/////////////
	strInfo.Format(_T("%d"), eSearchToolUnknown);
	cXMLConfigurator.AddElem(_T("ToolType"), strInfo);

	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	return true;
}

// 创建guid																
void CBaseSearchTool::CreateGUID(CString &strGUID)
{	
	GUID guid;
	CoCreateGuid(&guid);
	WCHAR   wstr[100]; 
	StringFromGUID2(guid,wstr,100); 
	CString   str   =   wstr; 	
	strGUID = wstr;
}

void CBaseSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
}
int CBaseSearchTool::GetResultErrNumber()
{
	return m_nResultErrNumber;
}

BOOL CBaseSearchTool::GetProcessImage(CBaseTrainDataParam* pTrainData,cpImage& src,cpImage &dst)
{
	if ( (pTrainData == NULL) || (src.IsValid() == FALSE) )
	{
		return FALSE;
	}
	if (src.PixelSize() == 1)
	{
		dst = src;
		return TRUE;
	}
	else
	{
		switch (pTrainData->m_nImageChannel)
		{
		case 0:
			{
				//scImage24 imagetmp24((unsigned char*)src.PointToPixel(0,0),src.Width(),src.Height(),1);
				//scImage8 imagetmp8(src.Width(),src.Height(),1);
				//imagetmp24.GetGrayImage(imagetmp8);
				//dest = imagetmp8;
				src.GetGrayImage(dst);

			}
			break;
		case 1:
			src.ChannelR(dst);
			break;
		case 2:
			src.ChannelG(dst);
			break;
		case 3:
			src.ChannelB(dst);
			break;
		default:
			return FALSE;
		}
	}
	return TRUE;
}

bool CBaseSearchTool::LoadModelTrainPatternImage(CString strDir , cpImage &ImagePattern)
{
	return false;
}