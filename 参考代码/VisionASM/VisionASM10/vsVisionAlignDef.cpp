// vsVisionAlignDef.cpp

#include "StdAfx.h"
#include "vsVisionAlignDef.h"


CCalibData::CCalibData()
{
	m_nCalibIndex = -1;
	m_strUserName = _T("");

	m_bCalibObjectSearchToolInited = FALSE;
	m_eCalibMarkPatternMode = eTraditionalCalib;
	m_eCalibHomographyDOF = eAuto;
	m_nWholeCalibCacuPos = 0;
	m_bEnableWholeCalibCacuPos = FALSE;
}

CCalibData::~CCalibData()
{
	int i=0; 
	int nSize = 0;

	nSize = m_vpCalibrateParam.size();
	for ( i=0; i<nSize; i++)
	{
		if (m_vpCalibrateParam.at(i) != NULL)
		{
			delete m_vpCalibrateParam.at(i);
			m_vpCalibrateParam.at(i) = NULL;
		}
	}
	m_vpCalibrateParam.clear();

	nSize = m_vpCalibratedInfo.size();
	for ( i=0; i<nSize; i++)
	{
		if (m_vpCalibratedInfo.at(i) != NULL)
		{
			delete m_vpCalibratedInfo.at(i);
			m_vpCalibratedInfo.at(i) = NULL;
		}
	}
	m_vpCalibratedInfo.clear();

	m_bCalibObjectSearchToolInited = FALSE;
}

CCalibData::CCalibData( const CCalibData& lCData )
{
	m_nCalibIndex = lCData.m_nCalibIndex;
	m_strUserName = lCData.m_strUserName;
	m_eCalibMarkPatternMode = lCData.m_eCalibMarkPatternMode;
	m_eCalibHomographyDOF = lCData.m_eCalibHomographyDOF;
	m_nWholeCalibCacuPos = lCData.m_nWholeCalibCacuPos;
	m_bEnableWholeCalibCacuPos = lCData.m_bEnableWholeCalibCacuPos;

	int j=0; 
	int nSrcSize = 0;

	nSrcSize = lCData.m_vpCalibrateParam.size();
	CCalibrateParam *pCalibrateParam = NULL;
	for (j=0; j<nSrcSize; j++)
	{
		pCalibrateParam = NULL;
		if (lCData.m_vpCalibrateParam.at(j) != NULL)
		{
			pCalibrateParam = new CCalibrateParam(*(lCData.m_vpCalibrateParam.at(j)));
		}
		m_vpCalibrateParam.push_back(pCalibrateParam);
	}

	nSrcSize = lCData.m_vpCalibratedInfo.size();
	CCalibratedInfo *pCalibratedInfo = NULL;
	for (j=0; j<nSrcSize; j++)
	{
		pCalibratedInfo = NULL;
		if (lCData.m_vpCalibratedInfo.at(j) != NULL)
		{
			pCalibratedInfo = new CCalibratedInfo(*(lCData.m_vpCalibratedInfo.at(j)));
		}
		m_vpCalibratedInfo.push_back(pCalibratedInfo);
	}

	m_bCalibObjectSearchToolInited = lCData.m_bCalibObjectSearchToolInited;
	m_vvpCalibObjectSearchToolRepo = lCData.m_vvpCalibObjectSearchToolRepo;
}



CCalibData::CCalibData( const SysPlatformInfo& platformInfo )
{
	m_nCalibIndex = -1;
	m_strUserName = _T("");

	m_eCalibMarkPatternMode = eTraditionalCalib;
	m_eCalibHomographyDOF = eAuto;
	m_bEnableWholeCalibCacuPos = FALSE;
	m_nWholeCalibCacuPos = 0;
	

//	int nPositionNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
	int nPositionNum = platformInfo.m_nPositionNum;
	int i=0;
	for (i=0; i<nPositionNum; i++)
	{
		m_vpCalibrateParam.push_back(new CCalibrateParam());
	}

	int nExPosNum = 0;
	if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
	{
		// 启用单产品多标定扩展
		nExPosNum = nPositionNum + nPositionNum*platformInfo.m_nMultiCalibExtensionMaxNum;
	}
	else
	{
		nExPosNum = nPositionNum;
	}
	//for (i=0; i<nPositionNum; i++)
	for (i=0; i<nExPosNum; i++)
	{
		m_vpCalibratedInfo.push_back(new CCalibratedInfo(platformInfo.m_ePlatformType, platformInfo.m_eCamPlatformType));
	}
	
	m_bCalibObjectSearchToolInited = FALSE;			// 标定定位工具是否初始化
	m_vvpCalibObjectSearchToolRepo.resize(nPositionNum);
	for (i=0; i<m_vvpCalibObjectSearchToolRepo.size(); i++)
	{
		m_vvpCalibObjectSearchToolRepo[i].resize(CALIBOBJECTCANDIDATENUM, NULL);
	}

}

CCalibData& CCalibData::operator=( const CCalibData& lCData )
{
	if (this == &lCData)
	{
		return *this;
	}

	m_nCalibIndex = lCData.m_nCalibIndex;
	m_strUserName = lCData.m_strUserName;

	m_eCalibMarkPatternMode = lCData.m_eCalibMarkPatternMode;
	m_eCalibHomographyDOF = lCData.m_eCalibHomographyDOF;
	m_nWholeCalibCacuPos = lCData.m_nWholeCalibCacuPos;
	m_bEnableWholeCalibCacuPos = lCData.m_bEnableWholeCalibCacuPos;

	int nSize = 0;
	int nSrcSize = 0;
	int i=0;
	nSize = m_vpCalibrateParam.size();
	for (i=0; i<nSize; i++)
	{
		if (m_vpCalibrateParam.at(i) != NULL)
		{
			delete m_vpCalibrateParam.at(i);
			m_vpCalibrateParam.at(i) = NULL;
		}
	}
	m_vpCalibrateParam.clear();

	nSrcSize = lCData.m_vpCalibrateParam.size();
	CCalibrateParam *pCalibrateParam = NULL;
	for (i=0; i<nSrcSize; i++)
	{
		if (lCData.m_vpCalibrateParam.at(i) != NULL)
		{
			pCalibrateParam = new CCalibrateParam(*(lCData.m_vpCalibrateParam.at(i)));
		}
		m_vpCalibrateParam.push_back(pCalibrateParam);
		pCalibrateParam = NULL;
	}


	nSize = m_vpCalibratedInfo.size();
	for (i=0; i<nSize; i++)
	{
		if (m_vpCalibratedInfo.at(i) != NULL)
		{
			delete m_vpCalibratedInfo.at(i);
			m_vpCalibratedInfo.at(i) = NULL;
		}
	}
	m_vpCalibratedInfo.clear();

	nSrcSize = lCData.m_vpCalibratedInfo.size();
	CCalibratedInfo *pCalibratedInfo = NULL;
	for (i=0; i<nSrcSize; i++)
	{
		if (lCData.m_vpCalibratedInfo.at(i) != NULL)
		{
			pCalibratedInfo = new CCalibratedInfo(*(lCData.m_vpCalibratedInfo.at(i)));
		}
		m_vpCalibratedInfo.push_back(pCalibratedInfo);
		pCalibratedInfo = NULL;
	}

	m_bCalibObjectSearchToolInited = lCData.m_bCalibObjectSearchToolInited;
	m_vvpCalibObjectSearchToolRepo = lCData.m_vvpCalibObjectSearchToolRepo;

	return *this;
}

BOOL CCalibData::IsValid()
{
	int nSize = m_vpCalibrateParam.size();
	if (nSize <= 0)
	{
		return FALSE;
	}

	int nSizeCalibInfo = m_vpCalibratedInfo.size();
	if (nSizeCalibInfo <= 0)
	{
		return FALSE;
	}

	int i = 0;
	//for (i=0; i<nSizeCalibInfo; i++)
	//{
	//	if (m_vpCalibratedInfo.at(i) == NULL || (m_vpCalibratedInfo.at(i)->IsValid()==FALSE))
	//	{
	//		return FALSE;
	//	}
	//}

	return TRUE;
}


BOOL CCalibData::IsCalibrateInfoValid()
{
	int nSizeCalibInfo = m_vpCalibratedInfo.size();
	if (nSizeCalibInfo <= 0)
	{
		return FALSE;
	}

	int i = 0;
	for (i=0; i<nSizeCalibInfo; i++)
	{
		if (m_vpCalibratedInfo.at(i) == NULL || (m_vpCalibratedInfo.at(i)->IsValid()==FALSE))
		{
			return FALSE;
		}
	}

	return TRUE;
}

int	CCalibData::GetCalibrateParamNum()
{
	return m_vpCalibrateParam.size();
}
int CCalibData::GetpCalibratedInfoNum()
{
	return m_vpCalibratedInfo.size();
}


// 设置标定参数的索引值
void CCalibData::SetCalibIndex(int nIndex)
{
	m_nCalibIndex = nIndex;
}

// 获得标定参数的索引值
int CCalibData::GetCalibIndex()
{
	return m_nCalibIndex;
}

//// 设置标定参数的名称
//void CCalibData::SetCalibUserName(BSTR bstrUserName)
//{
//	m_strUserName = (CString)bstrUserName;;
//}
//
//// 获取标定模板的名称
//BSTR CCalibData::GetCalibUserName()
//
//{
//	return m_strUserName.AllocSysString();
//}

// 设置标定参数的名称
void CCalibData::SetCalibUserName(CString strUserName)
{
	m_strUserName = strUserName;;
}

// 获取标定模板的名称
CString CCalibData::GetCalibUserName()

{
	return m_strUserName;
}



// 释放所有标定对象定位工具
void CCalibData::DeleteObjectSearchTools()
{
	int i=0, j = 0;
	m_bCalibObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	for (i =0; i<m_vvpCalibObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpCalibObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpCalibObjectSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpCalibObjectSearchToolRepo[i][j];
				m_vvpCalibObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}
// 赋空所有对象定位工具
void CCalibData::ResetObjectSearchTools()
{
	int i=0, j = 0;
	m_bCalibObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	for (i =0; i<m_vvpCalibObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpCalibObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpCalibObjectSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpCalibObjectSearchToolRepo[i][j];
				m_vvpCalibObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}

// 设置、获取关联标定模板类型
void CCalibData::SetCalibMarkPatternMode(CalibMarkPatternMode mode)
{
	m_eCalibMarkPatternMode = mode;
}
CalibMarkPatternMode CCalibData::GetCalibMarkPatternMode()
{
	return m_eCalibMarkPatternMode;
}

void CCalibData::SetCalibHomographyDOF(HomographyDOF CalibHomographyDOF)
{
	m_eCalibHomographyDOF = CalibHomographyDOF;
}
HomographyDOF CCalibData::GetCalibHomographyDOF()
{
	return m_eCalibHomographyDOF;
}

//设置、获取全局二维码靶标标定旋转中心计算位置
void CCalibData::SetWholeCalibCacuPos(int nPosIndex)
{
	m_nWholeCalibCacuPos = nPosIndex;
}
int CCalibData::GetWholeCalibCacuPos()
{
	return m_nWholeCalibCacuPos;
}

//设置、获取全局二维码靶标标定旋转中心计算位置
void CCalibData::SetWholeCalibCacuPosEnable(BOOL bEnable)
{
	m_bEnableWholeCalibCacuPos = bEnable;
}
BOOL CCalibData::GetWholeCalibCacuPosEnable()
{
	return m_bEnableWholeCalibCacuPos;
}



CTotalCalibData::CTotalCalibData()
{
	m_nCurCalibIndex = 0;
}

CTotalCalibData::~CTotalCalibData()
{
	int i = 0;
	for(i = 0; i < m_vCalibData.size(); i++)
	{
		if (m_vCalibData.at(i) != NULL)
		{
			m_vCalibData.at(i)->DeleteObjectSearchTools();
			delete m_vCalibData.at(i);
			m_vCalibData.at(i) = NULL;
		}
	}
	m_vCalibData.clear();
	m_vCalibIndexFaChd.clear();
	m_vCalibNameFa.clear();
	m_vCalibLastChdIndex.clear();
}

int CTotalCalibData::GetCount() const
{
	return m_vCalibData.size();
}

BOOL CTotalCalibData::SetItem( int nIndex, const CCalibData& item )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vCalibData.size()));
	if ((nIndex < 0) || (nIndex >= m_vCalibData.size()))
	{
		return FALSE;
	}
	//item.m_nCalibIndex = nIndex;
	if (m_vCalibData.at(nIndex) != NULL)
	{
		*m_vCalibData.at(nIndex) = item;
	}
	else
	{
		m_vCalibData.at(nIndex) = new CCalibData(item);
	}
	m_vCalibData.at(nIndex)->m_nCalibIndex = nIndex;
	return TRUE;
}

BOOL CTotalCalibData::SetItemGrow( int nIndex, const CCalibData& item )
{
	ASSERT(nIndex >= 0);
	if (nIndex < 0)
	{
		return FALSE;
	}
	if (nIndex >= m_vCalibData.size())
	{
		int nOldSize = m_vCalibData.size();
		m_vCalibData.resize(nIndex+1);
	}

	if (m_vCalibData.at(nIndex) != NULL)
	{
		*m_vCalibData.at(nIndex) = item;
	}
	else
	{
		m_vCalibData.at(nIndex) = new CCalibData(item);
	}
	m_vCalibData.at(nIndex)->m_nCalibIndex = nIndex;

	return TRUE;
}

BOOL CTotalCalibData::AddItem( const CCalibData& item )
{
	m_vCalibData.push_back(new CCalibData(item));
	m_vCalibData.at(m_vCalibData.size()-1)->m_nCalibIndex = m_vCalibData.size() - 1;
	return TRUE;
}

CCalibData CTotalCalibData::GetItem( int nIndex ) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_vCalibData.size()));
	if ((nIndex < 0) || (nIndex >= m_vCalibData.size()))
	{
		return CCalibData();
	}
	return *m_vCalibData.at(nIndex);
}

// 获取产品标定信息指针
CCalibData*	CTotalCalibData::GetItemPtr(int nIndex) const
{
//	ASSERT((nIndex >= 0) && (nIndex < m_vCalibData.size()));
	if ((nIndex < 0) || (nIndex >= m_vCalibData.size()))
	{
		return NULL;
	}
	return m_vCalibData.at(nIndex);
}

BOOL CTotalCalibData::RemoveItem( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vCalibData.size()));
	if ((nIndex < 0) || (nIndex >= m_vCalibData.size()))
	{
		return FALSE;
	}
	std::vector<CCalibData*>::iterator iter = m_vCalibData.begin();
	BOOL bRemoved = FALSE;
	int nNewIdx = 0;
	for (int i = 0; iter != m_vCalibData.end(); i++, iter++)
	{
		if (i == nIndex)
		{
			if (m_vCalibData.at(i) != NULL)
			{
				m_vCalibData.at(i)->DeleteObjectSearchTools();
				delete m_vCalibData.at(i);
				m_vCalibData.at(i) = NULL;
			}
			m_vCalibData.erase(iter);
			m_nCurCalibIndex = (m_nCurCalibIndex >= m_vCalibData.size()) ? (m_vCalibData.size()-1) : m_nCurCalibIndex;
			m_nCurCalibIndex = (m_nCurCalibIndex < 0) ? 0 : m_nCurCalibIndex;

			// 更改所删除产品以后的所有产品的索引号
			for (int j=i; j < m_vCalibData.size(); j++)
			{
				m_vCalibData.at(j)->m_nCalibIndex = j;
			}
			return TRUE;
		}
	}
	return TRUE;
}

BOOL CTotalCalibData::ClearItem( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vCalibData.size()));
	if ((nIndex < 0) || (nIndex >= m_vCalibData.size()))
	{
		return FALSE;
	}
	std::vector<CCalibData*>::iterator iter = m_vCalibData.begin();
	BOOL bRemoved = FALSE;
	int nNewIdx = 0;
	for (int i = 0; iter != m_vCalibData.end(); i++, iter++)
	{
		if (i == nIndex)
		{
			if (m_vCalibData.at(i) != NULL)
			{
				m_vCalibData.at(i)->DeleteObjectSearchTools();
			}
			//m_vCalibData.erase(iter);
			return TRUE;
		}
	}
	return TRUE;
}

void CTotalCalibData::Clear()
{
	for (int i = 0; i < m_vCalibData.size(); i++)
	{
		if (m_vCalibData.at(i) != NULL)
		{
			m_vCalibData.at(i)->DeleteObjectSearchTools();
			delete m_vCalibData.at(i);
			m_vCalibData.at(i) = NULL;
		}
	}
	m_vCalibData.clear();
}

BOOL CTotalCalibData::CopyItem( int nFromIndex, int nToIndex )
{
	int nSize = m_vCalibData.size();
	ASSERT((nFromIndex >= 0) && (nFromIndex < nSize));
	ASSERT((nToIndex >= 0) && (nToIndex < (nSize+1)));

	if ((nFromIndex < 0) || (nFromIndex >= nSize))
	{
		return FALSE;
	}
	if ((nToIndex < 0) || (nToIndex >= (nSize+1)))
	{
		return FALSE;
	}

	SetItemGrow(nToIndex, *m_vCalibData.at(nFromIndex));
// 
// 	if (nToIndex == nSize)
// 	{
// 		m_vCalibData.push_back(m_vCalibData.at(nFromIndex));
// 	}
// 	else
// 	{
// 		m_vCalibData.at(nToIndex) = m_vCalibData.at(nFromIndex);
// 	}
// 	m_vCalibData.at(nToIndex).m_pAlignerParam->m_nAlignerIndex = nToIndex;

	return TRUE;
}

BOOL CTotalCalibData::SetItemName( int nIdx, LPCTSTR lpszNewName )
{
	ASSERT((nIdx >= 0) && (nIdx < m_vCalibData.size()));
	if ((nIdx < 0) || (nIdx >= m_vCalibData.size()))
	{
		return FALSE;
	}
// 	CString strName = lpszNewName;
// 	BSTR bstrName = strName.AllocSysString();
// 	m_vCalibData.at(nIdx)->SetCalibUserName(bstrName);
// 	SysFreeString(bstrName);
// 	bstrName = NULL;

	CString strName = lpszNewName;
	m_vCalibData.at(nIdx)->SetCalibUserName(strName);

	return TRUE;
}

CString CTotalCalibData::GetItemName( int nIdx )
{
//	ASSERT((nIdx >= 0) && (nIdx < m_vCalibData.size()));
	if ((nIdx < 0) || (nIdx >= m_vCalibData.size()))
	{
		return _T("");
	}
// 	BSTR bstrName = m_vCalibData.at(nIdx)->GetCalibUserName();
// 	CString strName = (CString)bstrName;
// 	SysFreeString(bstrName);
	CString strName =  m_vCalibData.at(nIdx)->GetCalibUserName();
	return strName;
}

void CTotalCalibData::RemoveInvalidItems()
{
	std::vector<CCalibData*>::iterator iter = m_vCalibData.begin();
	for (; iter != m_vCalibData.end();)
	{
		if (!(*iter)->IsValid())
		{
			if ((*iter) != NULL)
			{
				(*iter)->DeleteObjectSearchTools();
				delete (*iter);
				(*iter) = NULL;
			}
			iter = m_vCalibData.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	int nSize = m_vCalibData.size();
	for (int i=0; i<nSize; i++)
	{
		m_vCalibData.at(i)->m_nCalibIndex = i;
	}
}

bool CTotalCalibData::SetCalibIndex(int nCalibIndexFa, int nCalibIndexChd, int nCalibIndex)
{
	if(nCalibIndexFa < 0 || nCalibIndexFa > m_vCalibIndexFaChd.size())
	{
		return false;
	}
	else if(nCalibIndexFa == m_vCalibIndexFaChd.size())
	{
		m_vCalibIndexFaChd.resize(nCalibIndexFa + 1);
		CString str;
		str.Format(_T("------"));
		m_vCalibNameFa.push_back(str);
	}

	if(nCalibIndexChd < 0 || nCalibIndexChd > m_vCalibIndexFaChd[nCalibIndexFa].size())
	{
		return false;
	}
	else if(nCalibIndexChd == m_vCalibIndexFaChd[nCalibIndexFa].size())
	{
		m_vCalibIndexFaChd[nCalibIndexFa].resize(nCalibIndexChd + 1);
	}

	m_vCalibIndexFaChd[nCalibIndexFa][nCalibIndexChd] = nCalibIndex;
	return true;
}

int CTotalCalibData::GetCalibIndex(int nCalibIndexFa, int nCalibIndexChd)
{
	if(nCalibIndexFa < 0 || nCalibIndexFa >= m_vCalibIndexFaChd.size())
	{
		return -1;
	}

	if(nCalibIndexChd < 0 || nCalibIndexChd >= m_vCalibIndexFaChd[nCalibIndexFa].size())
	{
		return -1;
	}

	return m_vCalibIndexFaChd[nCalibIndexFa][nCalibIndexChd];
}

int CTotalCalibData::GetCalibCountChd(int nCalibIndexFa)
{
	if(nCalibIndexFa < 0 || nCalibIndexFa >= m_vCalibIndexFaChd.size())
	{
		return 0;
	}

	return m_vCalibIndexFaChd[nCalibIndexFa].size();
}

bool CTotalCalibData::GetDlgCalibIndex(int nCalibIndex, int& nCalibIndexFa, int& nCalibIndexChd)
{
	bool bFind = false;
	for(nCalibIndexFa = 0; nCalibIndexFa < m_vCalibIndexFaChd.size(); nCalibIndexFa++)
	{
		bFind = false;
		for(nCalibIndexChd = 0; nCalibIndexChd < m_vCalibIndexFaChd[nCalibIndexFa].size(); nCalibIndexChd++)
		{
			if(m_vCalibIndexFaChd[nCalibIndexFa][nCalibIndexChd] == nCalibIndex)
			{
				bFind = true;
				break;
			}
		}

		if(bFind)
		{
			break;
		}
	}

	if(bFind)
	{
		return true;
	}
	else
	{
		nCalibIndexChd = 0;
		nCalibIndexFa = 0;
	}
	return false;
}

bool CTotalCalibData::SetCalibNameFa(int nCalibIndexFa, LPCTSTR lpszNewName)
{
	if (nCalibIndexFa < 0 || nCalibIndexFa >= m_vCalibNameFa.size())
	{
		return false;
	}

	m_vCalibNameFa[nCalibIndexFa] = lpszNewName;
	return true;
}

CString CTotalCalibData::GetCalibNameFa(int nCalibIndexFa)
{
	if (nCalibIndexFa < 0 || nCalibIndexFa >= m_vCalibNameFa.size())
	{
		return _T("");
	}

	return m_vCalibNameFa[nCalibIndexFa];
}

void CTotalCalibData::UpdateCalibLastChdIndex(int nCalibIndexFa, int nLastChdIndex)
{
	if(nCalibIndexFa >= m_vCalibLastChdIndex.size())
	{
		m_vCalibLastChdIndex.resize(nCalibIndexFa+1);
	}
	m_vCalibLastChdIndex[nCalibIndexFa] = nLastChdIndex;
}

int CTotalCalibData::GetCalibLastChdIndex(int nCalibIndexFa)
{
	if(nCalibIndexFa == m_vCalibLastChdIndex.size())
	{
		m_vCalibLastChdIndex.push_back(0);
	}
	return m_vCalibLastChdIndex[nCalibIndexFa];
}

CProductData::CProductData()
{
	m_pAlignerParam = NULL;

	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	m_bAlnVirtualObjectSearchToolInited = FALSE;		// 虚拟对象定位工具是否初始化
	m_bAlnCalibTargetSearchToolInited = FALSE;		// 关联标定定位工具是否初始化
	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化

}

CProductData::~CProductData()
{
	if (m_pAlignerParam != NULL)
	{
		delete m_pAlignerParam;
		m_pAlignerParam = NULL;
	}

	int nSize = m_vpCalibratedInfo.size();
	for (int i=0; i<nSize; i++)
	{
		if (m_vpCalibratedInfo.at(i) != NULL)
		{
			delete m_vpCalibratedInfo.at(i);
			m_vpCalibratedInfo.at(i) = NULL;
		}
	}
	m_vpCalibratedInfo.clear();


	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	m_bAlnVirtualObjectSearchToolInited = FALSE;		// 虚拟对象定位工具是否初始化
	m_bAlnCalibTargetSearchToolInited = FALSE;			// 关联标定定位工具是否初始化
	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化
}

CProductData::CProductData( const CProductData& lPData )
{
	m_pAlignerParam = (lPData.m_pAlignerParam == NULL) ? NULL : new CAlignerParam(*(lPData.m_pAlignerParam));

	int nSrcSize = lPData.m_vpCalibratedInfo.size();
	CCalibratedInfo *pCalibratedInfo = NULL;
	for (int j=0; j<nSrcSize; j++)
	{
		pCalibratedInfo = NULL;
		if (lPData.m_vpCalibratedInfo.at(j) != NULL)
		{
			pCalibratedInfo = new CCalibratedInfo(*(lPData.m_vpCalibratedInfo.at(j)));
		}
		m_vpCalibratedInfo.push_back(pCalibratedInfo);
	}


	m_bAlnTargetSearchToolInited = lPData.m_bAlnTargetSearchToolInited;
	m_vvpAlnTargetSearchToolRepo = lPData.m_vvpAlnTargetSearchToolRepo;

	m_bAlnObjectSearchToolInited = lPData.m_bAlnObjectSearchToolInited;
	m_vvpAlnObjectSearchToolRepo = lPData.m_vvpAlnObjectSearchToolRepo;

	m_bAlnVirtualObjectSearchToolInited = lPData.m_bAlnVirtualObjectSearchToolInited;
	m_vvpAlnVirtualObjectSearchToolRepo = lPData.m_vvpAlnVirtualObjectSearchToolRepo;

	m_bAlnCalibTargetSearchToolInited = lPData.m_bAlnCalibTargetSearchToolInited;
	m_vvpAlnCalibTargetSearchToolRepo = lPData.m_vvpAlnCalibTargetSearchToolRepo;

	m_bAlnInspectSearchToolInited = lPData.m_bAlnInspectSearchToolInited;
	m_vvpAlnInspectSearchToolRepo = lPData.m_vvpAlnInspectSearchToolRepo;

	m_RobotTeachInfo = lPData.m_RobotTeachInfo;

	m_vDMcodeCalib = lPData.m_vDMcodeCalib;
}

CProductData::CProductData( const SysPlatformInfo& platformInfo )
{
	m_pAlignerParam = new CAlignerParam();
	m_pAlignerParam->SetTargetMarkType(platformInfo.m_eAlignerTargetType);
	m_pAlignerParam->SetObjectMarkType(platformInfo.m_eAlignerObjectType);
	//m_pAlignerParam->SetAlignerMode(eDotToDotAligner);
	m_pAlignerParam->SetBenchTargetMarkFixEnable(platformInfo.m_bBenchMarkAmend);
	m_pAlignerParam->SetVirtualObjectMarkEnable(FALSE);
	m_pAlignerParam->SetTargetObjectCamSeparateEnable(FALSE);
	m_pAlignerParam->SetTargetCalibUseBoardEnable(FALSE);
	m_pAlignerParam->SetMultiCalibExtensionMaxNum(platformInfo.m_nMultiCalibExtensionMaxNum);


	int nPositionNum = platformInfo.m_nPositionNum;
	int nExPosNum = 0;
	if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
	{
		nExPosNum = nPositionNum + nPositionNum*platformInfo.m_nMultiCalibExtensionMaxNum;
	}
	else
	{
		nExPosNum = nPositionNum;
	}
	
	for (int i=0; i<nExPosNum; i++)
	{
		m_vpCalibratedInfo.push_back(new CCalibratedInfo(platformInfo.m_ePlatformType, platformInfo.m_eCamPlatformType));
	}



	// 目标Mark类型
	if (platformInfo.m_eAlignerTargetType == eTargetBench)
	{
		if (platformInfo.m_bBenchMarkAmend)
		{
			CBenchTargetMarkFixInfo *pBenchTargetMarkFixInfo = new CBenchTargetMarkFixInfo(nPositionNum, platformInfo.m_ePlatformType);
			m_pAlignerParam->SetBenchTargetMarkFixInfo(pBenchTargetMarkFixInfo);
			delete pBenchTargetMarkFixInfo;
			pBenchTargetMarkFixInfo = NULL;
		}
	}
	else
	{
		m_pAlignerParam->SetTargetMarkPatternNum(nPositionNum);
	}


	// 对象Mark类型
	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
	{
		if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
		{
			m_pAlignerParam->SetVirtualObjectMarkPatternNum(nPositionNum + nPositionNum*platformInfo.m_nMultiCalibExtensionMaxNum);
		}
		else
		{
			m_pAlignerParam->SetVirtualObjectMarkPatternNum(nPositionNum);
		}		
	}

	if (m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	{
		m_pAlignerParam->SetObjectMarkPatternNum(nPositionNum);
	}

	// 关联标定
	if (platformInfo.m_bTargetObjectCamSeparate == TRUE)
	{
		m_pAlignerParam->SetTargetObjectCamSeparateEnable(TRUE);
		m_pAlignerParam->SetCalibTargetMarkPatternNum(nPositionNum);
	}

	if (platformInfo.m_bTargetCalibUseBoard == TRUE)
	{
		m_pAlignerParam->SetTargetCalibUseBoardEnable(TRUE);
	}


	if (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE)
	{
		m_pAlignerParam->SetTargetObjectCamSeparateEnable(FALSE);
		m_pAlignerParam->SetCalibTargetMarkPatternNum(nPositionNum);
	}

	// 检测
	//if (m_SysOptionConfig.m_bInspectEnable == TRUE)
	{
		m_pAlignerParam->SetSysPlatformInspectEnable(TRUE);
		m_pAlignerParam->SetInspectMarkPatternNum(nPositionNum);
	}





	int k=0;
	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	m_vvpAlnTargetSearchToolRepo.resize(m_pAlignerParam->GetTargetMarkPatternNum());
	for (k=0; k<m_vvpAlnTargetSearchToolRepo.size(); k++)
	{
		m_vvpAlnTargetSearchToolRepo[k].resize(TARGETCANDIDATENUM, NULL);
	}


	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	m_vvpAlnObjectSearchToolRepo.resize(m_pAlignerParam->m_vObjectPosPatInfo.size());
	for (k=0; k<m_vvpAlnObjectSearchToolRepo.size(); k++)
	{
		m_vvpAlnObjectSearchToolRepo[k].resize(OBJECTCANDIDATENUM, NULL);
	}


	m_bAlnVirtualObjectSearchToolInited = FALSE;			// 虚拟对象定位工具是否初始化
	m_vvpAlnVirtualObjectSearchToolRepo.resize(m_pAlignerParam->GetObjectMarkPatternNum()/*m_pAlignerParam->GetVirtualObjectMarkPatternNum()*/);
	for (k=0; k<m_vvpAlnVirtualObjectSearchToolRepo.size(); k++)
	{
		m_vvpAlnVirtualObjectSearchToolRepo[k].resize(VIRTUALOBJECTCANDIDATENUM, NULL);
	}


	m_bAlnCalibTargetSearchToolInited = FALSE;			// 关联标定定位工具是否初始化
	m_vvpAlnCalibTargetSearchToolRepo.resize(m_pAlignerParam->GetCalibTargetMarkPatternNum());
	for (k=0; k<m_vvpAlnCalibTargetSearchToolRepo.size(); k++)
	{
		m_vvpAlnCalibTargetSearchToolRepo[k].resize(ALNCALIBTARGETCANDIDATENUM, NULL);
	}

	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化
	m_vvpAlnInspectSearchToolRepo.resize(m_pAlignerParam->GetInspectMarkPatternNum());
	for (k=0; k<m_vvpAlnInspectSearchToolRepo.size(); k++)
	{
		m_vvpAlnInspectSearchToolRepo[k].resize(INSPECTCANDIDATENUM, NULL);
	}

	m_vDMcodeCalib.resize(nPositionNum);

}

// CProductData::CProductData( const SysPlatformInfo& platformInfo )
// {
// 	m_pAlignerParam = new CAlignerParam();
// 	m_pAlignerParam->SetTargetMarkType(platformInfo.m_eAlignerTargetType);
// 	m_pAlignerParam->SetObjectMarkType(platformInfo.m_eAlignerObjectType);
// 	m_pAlignerParam->SetAlignerMode(platformInfo.m_eAlignerMode);
// 	m_pAlignerParam->SetBenchTargetMarkFixEnable(platformInfo.m_bBenchMarkAmend);
// 	m_pAlignerParam->SetVirtualObjectMarkEnable(FALSE);
// 	m_pAlignerParam->SetTargetObjectCamSeparateEnable(FALSE);
// 	m_pAlignerParam->SetTargetCalibUseBoardEnable(FALSE);
// 	m_pAlignerParam->SetMultiCalibExtensionMaxNum(platformInfo.m_nMultiCalibExtensionMaxNum);
// //	m_pAlignerParam->SetAlignInspectEnable(FALSE);
// //	int nPositionNum = (platformInfo.m_nCamNum == 1) ? platformInfo.m_nPositionNum : platformInfo.m_nCamNum;
// 	int nPositionNum = platformInfo.m_nPositionNum;
// 	int nExPosNum = 0;
// 	if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
// 	{
// 		nExPosNum = nPositionNum + nPositionNum*platformInfo.m_nMultiCalibExtensionMaxNum;
// 	}
// 	else
// 	{
// 		nExPosNum = nPositionNum;
// 	}
// 	//for (int i=0; i<nPositionNum; i++)
// 	for (int i=0; i<nExPosNum; i++)
// 	{
// 		m_vpCalibratedInfo.push_back(new CCalibratedInfo(platformInfo.m_ePlatformType, platformInfo.m_eCamPlatformType));
// 	}
// 
// 	// 目标Mark类型
// 	if (platformInfo.m_eAlignerTargetType == eTargetBench)
// 	{
// 		if (platformInfo.m_bBenchMarkAmend)
// 		{
// 			CBenchTargetMarkFixInfo *pBenchTargetMarkFixInfo = new CBenchTargetMarkFixInfo(nPositionNum, platformInfo.m_ePlatformType);
// 			m_pAlignerParam->SetBenchTargetMarkFixInfo(pBenchTargetMarkFixInfo);
// 			delete pBenchTargetMarkFixInfo;
// 			pBenchTargetMarkFixInfo = NULL;
// 		}
// 	}
// 	else
// 	{
// 		m_pAlignerParam->SetTargetMarkPatternNum(nPositionNum);
// 	}
// 	// 对象Mark类型
// 	if (platformInfo.m_eAlignerObjectType == eObjectVirtual)
// 	{
// 		if (platformInfo.m_bEnableMultiCalibExtension == TRUE)
// 		{
// 			m_pAlignerParam->SetVirtualObjectMarkPatternNum(nPositionNum + nPositionNum*platformInfo.m_nMultiCalibExtensionMaxNum);
// 		}
// 		else
// 		{
// 			m_pAlignerParam->SetVirtualObjectMarkPatternNum(nPositionNum);
// 		}		
// 	}
// 
// 	if (m_pAlignerParam->GetObjectMarkType() != eObjectBench)
// 	{
// 		m_pAlignerParam->SetObjectMarkPatternNum(nPositionNum);
// 	}
// 
// 	// 关联标定
// 	if (platformInfo.m_bTargetObjectCamSeparate == TRUE)
// 	{
// 		m_pAlignerParam->SetTargetObjectCamSeparateEnable(TRUE);
// 		m_pAlignerParam->SetCalibTargetMarkPatternNum(nPositionNum);
// 	}
// 
// 	if (platformInfo.m_bTargetCalibUseBoard == TRUE)
// 	{
// 		m_pAlignerParam->SetTargetCalibUseBoardEnable(TRUE);
// 	}
// 
// 
// 	if (platformInfo.m_bTargetObjectCamSeparate == FALSE && platformInfo.m_bTargetCalibUseBoard == TRUE)
// 	{
// 		m_pAlignerParam->SetTargetObjectCamSeparateEnable(FALSE);
// 		m_pAlignerParam->SetCalibTargetMarkPatternNum(nPositionNum);
// 	}
// 
// 	// 检测
// 	//if (m_SysOptionConfig.m_bInspectEnable == TRUE)
// 	{
// 		m_pAlignerParam->SetSysPlatformInspectEnable(TRUE);
// 		m_pAlignerParam->SetInspectMarkPatternNum(nPositionNum);
// 	}
// 
// 	int k=0;
// 
// 	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
// 	m_vvpAlnTargetSearchToolRepo.resize(m_pAlignerParam->GetTargetMarkPatternNum());
// 	for (k=0; k<m_vvpAlnTargetSearchToolRepo.size(); k++)
// 	{
// 		m_vvpAlnTargetSearchToolRepo[k].resize(TARGETCANDIDATENUM, NULL);
// 	}
// 
// 
// 	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
// 	m_vvpAlnObjectSearchToolRepo.resize(m_pAlignerParam->m_vObjectPosPatInfo.size());
// 	for (k=0; k<m_vvpAlnObjectSearchToolRepo.size(); k++)
// 	{
// 		m_vvpAlnObjectSearchToolRepo[k].resize(OBJECTCANDIDATENUM, NULL);
// 	}
// 	
// 
// 	m_bAlnVirtualObjectSearchToolInited = FALSE;			// 虚拟对象定位工具是否初始化
// 	m_vvpAlnVirtualObjectSearchToolRepo.resize(m_pAlignerParam->GetObjectMarkPatternNum()/*m_pAlignerParam->GetVirtualObjectMarkPatternNum()*/);
// 	for (k=0; k<m_vvpAlnVirtualObjectSearchToolRepo.size(); k++)
// 	{
// 		m_vvpAlnVirtualObjectSearchToolRepo[k].resize(VIRTUALOBJECTCANDIDATENUM, NULL);
// 	}
// 
// 
// 	m_bAlnCalibTargetSearchToolInited = FALSE;			// 关联标定定位工具是否初始化
// 	m_vvpAlnCalibTargetSearchToolRepo.resize(m_pAlignerParam->GetCalibTargetMarkPatternNum());
// 	for (k=0; k<m_vvpAlnCalibTargetSearchToolRepo.size(); k++)
// 	{
// 		m_vvpAlnCalibTargetSearchToolRepo[k].resize(ALNCALIBTARGETCANDIDATENUM, NULL);
// 	}
// 
// 	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化
// 	m_vvpAlnInspectSearchToolRepo.resize(m_pAlignerParam->GetInspectMarkPatternNum());
// 	for (k=0; k<m_vvpAlnInspectSearchToolRepo.size(); k++)
// 	{
// 		m_vvpAlnInspectSearchToolRepo[k].resize(INSPECTCANDIDATENUM, NULL);
// 	}
// 
// 	m_vDMcodeCalib.resize(nPositionNum);
// 
// }

CProductData& CProductData::operator=( const CProductData& lPData )
{
	if (this == &lPData)
	{
		return *this;
	}
	if (m_pAlignerParam != NULL)
	{
		delete m_pAlignerParam;
		m_pAlignerParam = NULL;
	}
	if (lPData.m_pAlignerParam != NULL)
	{
		m_pAlignerParam = new CAlignerParam(*(lPData.m_pAlignerParam));
	}
	int nSize = m_vpCalibratedInfo.size();
	for (int i=0; i<nSize; i++)
	{
		if (m_vpCalibratedInfo.at(i) != NULL)
		{
			delete m_vpCalibratedInfo.at(i);
			m_vpCalibratedInfo.at(i) = NULL;
		}
	}
	m_vpCalibratedInfo.clear();

	int nSrcSize = lPData.m_vpCalibratedInfo.size();
	CCalibratedInfo *pCalibratedInfo = NULL;
	for (int j=0; j<nSrcSize; j++)
	{
		if (lPData.m_vpCalibratedInfo.at(j) != NULL)
		{
			pCalibratedInfo = new CCalibratedInfo(*(lPData.m_vpCalibratedInfo.at(j)));
		}
		m_vpCalibratedInfo.push_back(pCalibratedInfo);
		pCalibratedInfo = NULL;
	}


	m_bAlnTargetSearchToolInited = lPData.m_bAlnTargetSearchToolInited;
	m_vvpAlnTargetSearchToolRepo = lPData.m_vvpAlnTargetSearchToolRepo;

	m_bAlnObjectSearchToolInited = lPData.m_bAlnObjectSearchToolInited;
	m_vvpAlnObjectSearchToolRepo = lPData.m_vvpAlnObjectSearchToolRepo;

	m_bAlnVirtualObjectSearchToolInited = lPData.m_bAlnVirtualObjectSearchToolInited;
	m_vvpAlnVirtualObjectSearchToolRepo = lPData.m_vvpAlnVirtualObjectSearchToolRepo;

	m_bAlnCalibTargetSearchToolInited = lPData.m_bAlnCalibTargetSearchToolInited;
	m_vvpAlnCalibTargetSearchToolRepo = lPData.m_vvpAlnCalibTargetSearchToolRepo;

	m_bAlnInspectSearchToolInited = lPData.m_bAlnInspectSearchToolInited;
	m_vvpAlnInspectSearchToolRepo = lPData.m_vvpAlnInspectSearchToolRepo;

	m_RobotTeachInfo = lPData.m_RobotTeachInfo;
	m_vDMcodeCalib = lPData.m_vDMcodeCalib;
	return *this;
}

BOOL CProductData::IsValid()
{
	return TRUE;

	int nSizeCalibInfo = m_vpCalibratedInfo.size();
	if (nSizeCalibInfo <= 0)
	{
		return FALSE;
	}
	if (m_pAlignerParam == NULL)
	{
		return FALSE;
	}
	
	// 目标Mark参数有效性检查
	if (m_pAlignerParam->GetTargetMarkType() == eTargetOnline)
	{
		if (m_pAlignerParam->GetTargetMarkPatternNum() != nSizeCalibInfo)
		{
			return FALSE;
		}
	}
	else if (m_pAlignerParam->GetBenchTargetMarkFixEnable())
	{
		CBenchTargetMarkFixInfo *pInfo = m_pAlignerParam->GetBenchTargetMarkFixInfo();
		if ((pInfo == NULL) || (!pInfo->IsValid()))
		{
			return FALSE;
		}
	}

	// 对象Mark参数有效性检查
	if (m_pAlignerParam->GetObjectMarkType() != eObjectBench)
	{
		if (m_pAlignerParam->GetObjectMarkPatternNum() != nSizeCalibInfo)
		{
			return FALSE;
		}
		else if (m_pAlignerParam->GetVirtualObjectMarkEnable())
		{
			if (m_pAlignerParam->GetVirtualObjectMarkPatternNum() != nSizeCalibInfo)
			{
				return FALSE;
			}
		}
	}
// 	if (m_pAlignerParam->GetObjectMarkPatternNum() != nSizeCalibInfo)
// 	{
// 		return FALSE;
// 	}
// 	else if (m_pAlignerParam->GetVirtualObjectMarkEnable())
// 	{
// 		if (m_pAlignerParam->GetVirtualObjectMarkPatternNum() != nSizeCalibInfo)
// 		{
// 			return FALSE;
// 		}
// 	}
	return TRUE;
}


void CProductData::DeleteTargetSearchTools()
{
	int i=0, j = 0;
	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	for (i =0; i<m_vvpAlnTargetSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnTargetSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnTargetSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpAlnTargetSearchToolRepo[i][j];
				m_vvpAlnTargetSearchToolRepo[i][j] = NULL;
			}
		}
	}

}

void CProductData::DeleteObjectSearchTools()
{
	int i=0, j = 0;
	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	for (i =0; i<m_vvpAlnObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnObjectSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpAlnObjectSearchToolRepo[i][j];
				m_vvpAlnObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}


void CProductData::DeleteVirtualObjectSearchTools()
{
	int i=0, j = 0;
	m_bAlnVirtualObjectSearchToolInited = FALSE;			// 虚拟对象定位工具是否初始化
	for (i =0; i<m_vvpAlnVirtualObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnVirtualObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnVirtualObjectSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpAlnVirtualObjectSearchToolRepo[i][j];
				m_vvpAlnVirtualObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}


// 释放所有关联标定定位工具
void CProductData::DeleteCalibTargetSearchTools()
{
	int i=0, j = 0;
	m_bAlnCalibTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	for (i =0; i<m_vvpAlnCalibTargetSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnCalibTargetSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnCalibTargetSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpAlnCalibTargetSearchToolRepo[i][j];
				m_vvpAlnCalibTargetSearchToolRepo[i][j] = NULL;
			}
		}
	}
}

void CProductData::DeleteInspectSearchTools()
{
	int i=0, j = 0;
	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化
	for (i =0; i<m_vvpAlnInspectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnInspectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnInspectSearchToolRepo[i][j] != NULL)
			{
				delete m_vvpAlnInspectSearchToolRepo[i][j];
				m_vvpAlnInspectSearchToolRepo[i][j] = NULL;
			}
		}
	}

}


// 赋空所有目标定位工具
void CProductData::ResetTargetSearchTools()
{
	int i=0, j = 0;
	m_bAlnTargetSearchToolInited = FALSE;			// 目标定位工具是否初始化
	for (i =0; i<m_vvpAlnTargetSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnTargetSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnTargetSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpAlnTargetSearchToolRepo[i][j];
				m_vvpAlnTargetSearchToolRepo[i][j] = NULL;
			}
		}
	}

}

// 赋空所有对象定位工具			
void CProductData::ResetObjectSearchTools()
{
	int i=0, j = 0;
	m_bAlnObjectSearchToolInited = FALSE;			// 对象定位工具是否初始化
	for (i =0; i<m_vvpAlnObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnObjectSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpAlnObjectSearchToolRepo[i][j];
				m_vvpAlnObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}

// 赋空所有虚拟对象定位工具			
void CProductData::ResetVirtualObjectSearchTools()
{
	int i=0, j = 0;
	m_bAlnVirtualObjectSearchToolInited = FALSE;			// 虚拟对象定位工具是否初始化
	for (i =0; i<m_vvpAlnVirtualObjectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnVirtualObjectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnVirtualObjectSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpAlnVirtualObjectSearchToolRepo[i][j];
				m_vvpAlnVirtualObjectSearchToolRepo[i][j] = NULL;
			}
		}
	}
}

// 赋空所有关联标定定位工具
void CProductData::ResetCalibTargetSearchTools()
{
	int i=0, j = 0;
	m_bAlnCalibTargetSearchToolInited = FALSE;			// 关联标定定位工具是否初始化
	for (i =0; i<m_vvpAlnCalibTargetSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnCalibTargetSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnCalibTargetSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpAlnCalibTargetSearchToolRepo[i][j];
				m_vvpAlnCalibTargetSearchToolRepo[i][j] = NULL;
			}
		}
	}

}

// 赋空所有检测定位工具
void CProductData::ResetInspectSearchTools()
{
	int i=0, j = 0;
	m_bAlnInspectSearchToolInited = FALSE;			// 检测定位工具是否初始化
	for (i =0; i<m_vvpAlnInspectSearchToolRepo.size(); i++)
	{
		for (j = 0; j < m_vvpAlnInspectSearchToolRepo[i].size(); j++)
		{
			if (m_vvpAlnInspectSearchToolRepo[i][j] != NULL)
			{
				//delete m_vvpAlnInspectSearchToolRepo[i][j];
				m_vvpAlnInspectSearchToolRepo[i][j] = NULL;
			}
		}
	}

}

CTotalProductData::CTotalProductData()
{
	m_nCurProductIndex = 0;
}

CTotalProductData::~CTotalProductData()
{
	int i = 0;
	for(i = 0; i < m_vProductData.size(); i++)
	{
		if (m_vProductData.at(i) != NULL)
		{
			m_vProductData.at(i)->DeleteTargetSearchTools();
			m_vProductData.at(i)->DeleteObjectSearchTools();
			m_vProductData.at(i)->DeleteVirtualObjectSearchTools();
			m_vProductData.at(i)->DeleteCalibTargetSearchTools();
			m_vProductData.at(i)->DeleteInspectSearchTools();

			delete m_vProductData.at(i);
			m_vProductData.at(i) = NULL;
		}
	}
	m_vProductData.clear();
	m_vProductIndexFaChd.clear();
	m_vProductNameFa.clear();
	m_vProductLastChdIndex.clear();
}

int CTotalProductData::GetCount() const
{
	return m_vProductData.size();
}

BOOL CTotalProductData::SetItem( int nIndex, const CProductData& item )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vProductData.size()));
	if ((nIndex < 0) || (nIndex >= m_vProductData.size()))
	{
		return FALSE;
	}
 	//item.m_pAlignerParam->m_nAlignerIndex = nIndex;
	if (m_vProductData.at(nIndex) != NULL)
	{
		*m_vProductData.at(nIndex) = item;
	}
	else
	{
		m_vProductData.at(nIndex) = new CProductData(item);
	}

	m_vProductData.at(nIndex)->m_pAlignerParam->m_nAlignerIndex = nIndex;
	return TRUE;
}

BOOL CTotalProductData::SetItemGrow( int nIndex, const CProductData& item )
{
	ASSERT(nIndex >= 0);
	if (nIndex < 0)
	{
		return FALSE;
	}
	if (nIndex >= m_vProductData.size())
	{
		int nOldSize = m_vProductData.size();
		m_vProductData.resize(nIndex+1);
	}

	//item.m_pAlignerParam->m_nAlignerIndex = nIndex;
	if (m_vProductData.at(nIndex) != NULL)
	{
		*m_vProductData.at(nIndex) = item;
	}
	else
	{
		m_vProductData.at(nIndex) = new CProductData(item);
	}
	m_vProductData.at(nIndex)->m_pAlignerParam->m_nAlignerIndex = nIndex;
	return TRUE;
}

BOOL CTotalProductData::AddItem( const CProductData& item )
{
	//item.m_pAlignerParam->m_nAlignerIndex = m_vProductData.size();
	m_vProductData.push_back(new CProductData(item));
	m_vProductData.at(m_vProductData.size() - 1)->m_pAlignerParam->m_nAlignerIndex = m_vProductData.size() - 1;
	return TRUE;
}

CProductData CTotalProductData::GetItem( int nIndex ) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_vProductData.size()));
	if ((nIndex < 0) || (nIndex >= m_vProductData.size()))
	{
		return CProductData();
	}
	return *m_vProductData.at(nIndex);
}

CProductData*	CTotalProductData::GetItemPtr(int nIndex) const				// 获取产品信息指针
{
//	ASSERT((nIndex >= 0) && (nIndex < m_vProductData.size()));
	if ((nIndex < 0) || (nIndex >= m_vProductData.size()))
	{
		return NULL;
	}
	return m_vProductData.at(nIndex);
}

BOOL CTotalProductData::RemoveItem( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vProductData.size()));
	if ((nIndex < 0) || (nIndex >= m_vProductData.size()))
	{
		return FALSE;
	}
	std::vector<CProductData*>::iterator iter = m_vProductData.begin();
	BOOL bRemoved = FALSE;
	int nNewIdx = 0;
	for (int i = 0; iter != m_vProductData.end(); i++, iter++)
	{
		if (i == nIndex)
		{
			if (m_vProductData.at(i) != NULL)
			{
				m_vProductData.at(i)->DeleteTargetSearchTools();
				m_vProductData.at(i)->DeleteObjectSearchTools();
				m_vProductData.at(i)->DeleteVirtualObjectSearchTools();
				m_vProductData.at(i)->DeleteCalibTargetSearchTools();
				m_vProductData.at(i)->DeleteInspectSearchTools();
				delete m_vProductData.at(i);
				m_vProductData.at(i) = NULL;
			}

			m_vProductData.erase(iter);
			m_nCurProductIndex = (m_nCurProductIndex >= m_vProductData.size()) ? (m_vProductData.size()-1) : m_nCurProductIndex;
			m_nCurProductIndex = (m_nCurProductIndex < 0) ? 0 : m_nCurProductIndex;

			// 更改所删除产品以后的所有产品的索引号
			for (int j=i; j < m_vProductData.size(); j++)
			{
				if (m_vProductData.at(j)->m_pAlignerParam != NULL)
				{
					m_vProductData.at(j)->m_pAlignerParam->m_nAlignerIndex = j;
				}
			}
			return TRUE;
		}
	}
	return TRUE;
}

BOOL CTotalProductData::ClearItem( int nIndex )
{
	ASSERT((nIndex >= 0) && (nIndex < m_vProductData.size()));
	if ((nIndex < 0) || (nIndex >= m_vProductData.size()))
	{
		return FALSE;
	}
	std::vector<CProductData*>::iterator iter = m_vProductData.begin();
	BOOL bRemoved = FALSE;
	int nNewIdx = 0;
	for (int i = 0; iter != m_vProductData.end(); i++, iter++)
	{
		if (i == nIndex)
		{
			if (m_vProductData.at(i) != NULL)
			{
				m_vProductData.at(i)->DeleteTargetSearchTools();
				m_vProductData.at(i)->DeleteObjectSearchTools();
				m_vProductData.at(i)->DeleteVirtualObjectSearchTools();
				m_vProductData.at(i)->DeleteCalibTargetSearchTools();
				m_vProductData.at(i)->DeleteInspectSearchTools();
			}
			//m_vProductData.erase(iter);
			return TRUE;
		}
	}
	return TRUE;
}

void CTotalProductData::Clear()
{
	int i = 0;
	for(i = 0; i < m_vProductData.size(); i++)
	{
		if (m_vProductData.at(i) != NULL)
		{
			m_vProductData.at(i)->DeleteTargetSearchTools();
			m_vProductData.at(i)->DeleteObjectSearchTools();
			m_vProductData.at(i)->DeleteVirtualObjectSearchTools();
			m_vProductData.at(i)->DeleteCalibTargetSearchTools();
			m_vProductData.at(i)->DeleteInspectSearchTools();

			delete m_vProductData.at(i);
			m_vProductData.at(i) = NULL;
		}
	}
	m_vProductData.clear();
}

BOOL CTotalProductData::CopyItem( int nFromIndex, int nToIndex )
{
	int nSize = m_vProductData.size();
	ASSERT((nFromIndex >= 0) && (nFromIndex < nSize));
	ASSERT((nToIndex >= 0) && (nToIndex < (nSize+1)));

	if ((nFromIndex < 0) || (nFromIndex >= nSize))
	{
		return FALSE;
	}
	if ((nToIndex < 0)/* || (nToIndex >= (nSize+1))*/)
	{
		return FALSE;
	}

	SetItemGrow(nToIndex, *m_vProductData.at(nFromIndex));
// 
// 	if (nToIndex == nSize)
// 	{
// 		m_vProductData.push_back(m_vProductData.at(nFromIndex));
// 	}
// 	else
// 	{
// 		m_vProductData.at(nToIndex) = m_vProductData.at(nFromIndex);
// 	}
// 	m_vProductData.at(nToIndex).m_pAlignerParam->m_nAlignerIndex = nToIndex;

	return TRUE;
}

BOOL CTotalProductData::SetItemName( int nIdx, LPCTSTR lpszNewName )
{
	ASSERT((nIdx >= 0) && (nIdx < m_vProductData.size()));
	if ((nIdx < 0) || (nIdx >= m_vProductData.size()))
	{
		return FALSE;
	}
//	CString strName = lpszNewName;
// 	BSTR bstrName = strName.AllocSysString();
// 	m_vProductData.at(nIdx)->m_pAlignerParam->SetProductUserName(bstrName);
// 	SysFreeString(bstrName);
// 	bstrName = NULL;
	CString strName = lpszNewName;
	m_vProductData.at(nIdx)->m_pAlignerParam->SetProductUserName(strName);
	return TRUE;
}

CString CTotalProductData::GetItemName( int nIdx )
{
	ASSERT((nIdx >= 0) && (nIdx < m_vProductData.size()));
	if ((nIdx < 0) || (nIdx >= m_vProductData.size()))
	{
		return _T("------");
	}
// 	BSTR bstrName = m_vProductData.at(nIdx)->m_pAlignerParam->GetProductUserName();
// 	CString strName = (CString)bstrName;
// 	SysFreeString(bstrName);
	CString strName = m_vProductData.at(nIdx)->m_pAlignerParam->GetProductUserName();
	return strName;
}

void CTotalProductData::RemoveInvalidItems()
{
	std::vector<CProductData*>::iterator iter = m_vProductData.begin();
	for (; iter != m_vProductData.end();)
	{
		if (!(*iter)->IsValid())
		{
			if ((*iter) != NULL)
			{
				(*iter)->DeleteTargetSearchTools();
				(*iter)->DeleteObjectSearchTools();
				(*iter)->DeleteVirtualObjectSearchTools();
				(*iter)->DeleteCalibTargetSearchTools();
				(*iter)->DeleteInspectSearchTools();
				delete (*iter);
				(*iter) = NULL;
			}
			iter = m_vProductData.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	int nSize = m_vProductData.size();
	for (int i=0; i<nSize; i++)
	{
		m_vProductData.at(i)->m_pAlignerParam->m_nAlignerIndex = i;
	}
}

bool CTotalProductData::SetProductIndex(int nProductIndexFa, int nProductIndexChd, int nProductIndex)
{
	if(nProductIndexFa < 0 || nProductIndexFa > m_vProductIndexFaChd.size())
	{
		return false;
	}
	else if(nProductIndexFa == m_vProductIndexFaChd.size())
	{
		m_vProductIndexFaChd.resize(nProductIndexFa + 1);
		CString str;
		str.Format(_T("------"));
		m_vProductNameFa.push_back(str);
	}

	if(nProductIndexChd <0 || nProductIndexChd > m_vProductIndexFaChd[nProductIndexFa].size())
	{
		return false;
	}
	else if(nProductIndexChd == m_vProductIndexFaChd[nProductIndexFa].size())
	{
		m_vProductIndexFaChd[nProductIndexFa].resize(nProductIndexChd + 1);
	}

	m_vProductIndexFaChd[nProductIndexFa][nProductIndexChd] = nProductIndex;
	return true;
}

int CTotalProductData::GetProductIndex(int nProductIndexFa, int nProductIndexChd)
{
	if(nProductIndexFa < 0 || nProductIndexFa >= m_vProductIndexFaChd.size())
	{
		return -1;
	}
	
	if(nProductIndexChd <0 || nProductIndexChd >= m_vProductIndexFaChd[nProductIndexFa].size())
	{
		return -1;
	}

	return m_vProductIndexFaChd[nProductIndexFa][nProductIndexChd];
}

int CTotalProductData::GetProductCountChd(int nFaProductIndex)
{
	if(nFaProductIndex < 0 || nFaProductIndex >= m_vProductIndexFaChd.size())
	{	
		return 0;
	}

	return m_vProductIndexFaChd[nFaProductIndex].size();
}

bool CTotalProductData::GetDlgProductIndex(int nProductIndex, int& nFaProductIndex, int& nChdProductIndex)
{
	bool bFind = false;
	for(nFaProductIndex = 0; nFaProductIndex < m_vProductIndexFaChd.size(); nFaProductIndex++)
	{
		bFind = false;
		for(nChdProductIndex = 0; nChdProductIndex < m_vProductIndexFaChd[nFaProductIndex].size(); nChdProductIndex++)
		{
			if(m_vProductIndexFaChd[nFaProductIndex][nChdProductIndex] == nProductIndex)
			{
				bFind = true;
				break;
			}
		}

		if(bFind)
		{
			break;
		}
	}

	if(bFind)
	{
		return true;
	}
	else
	{
		nFaProductIndex = 0;
		nChdProductIndex = 0;
	}
	return false;
}

bool CTotalProductData::SetProductNameFa(int nProductIndexFa, LPCTSTR lpszNewName)
{
	if(nProductIndexFa < 0 || nProductIndexFa >= m_vProductNameFa.size())
	{
		return false;
	}

	m_vProductNameFa[nProductIndexFa] = lpszNewName;
	return true;
}

CString CTotalProductData::GetProductNameFa(int nProductIndexFa)
{
	if (nProductIndexFa < 0 || nProductIndexFa >= m_vProductNameFa.size())
	{
		return _T("------");
	}

	return m_vProductNameFa[nProductIndexFa];
}

void CTotalProductData::UpdateLastChdIndex(int nProductIndexFa, int nLastChdIndex)
{
	if(nProductIndexFa >= m_vProductLastChdIndex.size())
	{
		m_vProductLastChdIndex.resize(nProductIndexFa+1);
	}
	m_vProductLastChdIndex[nProductIndexFa] = nLastChdIndex;
}

int CTotalProductData::GetLastChdIndex(int nProductIndexFa)
{
	if(nProductIndexFa == m_vProductLastChdIndex.size())
	{
		m_vProductLastChdIndex.push_back(0);
	}
	return m_vProductLastChdIndex[nProductIndexFa];
}

CVisionASMConfig::CVisionASMConfig()
{
	m_pPlatformInfo = NULL;//new CPlatformXYDInfo();
	m_pCamPlatformInfo = NULL;//new CCamPlatformInfo();
	m_pCommParam = NULL;//new scSerialCommInfo();
	m_pMidPlatformInfo = NULL;

	m_MapAuthorizeItem.insert(make_pair(eCamremaAuthorize_Node,make_pair(_T("相机设置"),make_pair(IDS_CMB_CAMREMAAUTHORIZE_NODE,_T("CamremaAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eCamremaAuthorize_ParamSet,make_pair(_T("参数设置"),make_pair(IDS_CMB_CAMREMAAUTHORIZE_PARAMSET,_T("CamremaAuthorizeParamSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCamremaAuthorize_ShowSet,make_pair(_T("显示设置"),make_pair(IDS_CMB_CAMREMAAUTHORIZE_SHOWSET,_T("CamremaAuthorizeShowSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCamremaAuthorize_ColorSet,make_pair(_T("彩色相机设置"),make_pair(IDS_CMB_CAMREMAAUTHORIZE_COLORSET,_T("CamremaAuthorizeColorSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCamremaAuthorize_OtherItem,make_pair(_T("其他项"),make_pair(IDS_CMB_CAMREMAAUTHORIZE_OTHERITEM,_T("CamremaAuthorizeOtherItem")))));

	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_Node,make_pair(_T("通信设置"),make_pair(IDS_CMB_COMMAUTHORIZE_NODE,_T("CommAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_ParamSet,make_pair(_T("通信参数"),make_pair(IDS_CMB_COMMAUTHORIZE_PARAMSET,_T("CommAuthorizeParamSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_CommTypeSet,make_pair(_T("通信连接类型"),make_pair(IDS_CMB_COMMAUTHORIZE_COMMTYPESET,_T("CommAuthorizeCommTypeSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_TestSet,make_pair(_T("连接读写测试"),make_pair(IDS_CMB_COMMAUTHORIZE_TESTSET,_T("CommAuthorizeTestSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_OtherSet,make_pair(_T("其他通信设置"),make_pair(IDS_CMB_COMMAUTHORIZE_OTHER_SET,_T("CommAuthorizeOtherSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCommAuthorize_CommDelaySet,make_pair(_T("通信延时超时设置"),make_pair(IDS_CMB_COMMAUTHORIZE_COMMDELAYSET,_T("CommAuthorizeCommDelaySet")))));

	m_MapAuthorizeItem.insert(make_pair(ePlatformAuthorize_Node,make_pair(_T("平台设置"),make_pair(IDS_CMB_PLATFORMAUTHORIZE_NODE,_T("PlatformAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformAuthorize_ParamSet,make_pair(_T("对位平台参数"),make_pair(IDS_CMB_PLATFORMAUTHORIZE_PARAMSET,_T("PlatformAuthorizeParamSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformAuthorize_Camplatform,make_pair(_T("相机平台参数"),make_pair(IDS_CMB_PLATFORMAUTHORIZE_CAMPLATFORM,_T("PlatformAuthorizeCamplatform")))));

	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_Node,make_pair(_T("日志设置"),make_pair(IDS_CMB_LOGAUTHORIZE_NODE,_T("LogAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_Content,make_pair(_T("日志内容"),make_pair(IDS_CMB_LOGAUTHORIZE_CONTENT,_T("LogAuthorizeContent")))));
	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_SaveImg,make_pair(_T("存图文件"),make_pair(IDS_CMB_LOGAUTHORIZE_SAVEIMG,_T("LogAuthorizeSaveImgFile")))));
	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_PathSet,make_pair(_T("路径设置"),make_pair(IDS_CMB_LOGAUTHORIZE_PATHSET,_T("LogAuthorizePathSet")))));
	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_SaveByHour,make_pair(_T("按小时存图"),make_pair(IDS_CMB_LOGAUTHORIZE_SAVEBYHOUR,_T("LogAuthorizeSaveByHour")))));
	m_MapAuthorizeItem.insert(make_pair(eLogAuthorize_SaveVDB,make_pair(_T("vdb文件"),make_pair(IDS_CMB_LOGAUTHORIZE_SAVEVDB,_T("LogAuthorizeSaveVdbFile")))));

	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_Node,make_pair(_T("平台选项"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_NODE,_T("PlatformOptionAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_SearchTool,make_pair(_T("定位工具"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_SEARCHTOOL,_T("PlatformOptionAuthorizeSearchTool")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_OffSet,make_pair(_T("补偿设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_OFFSET,_T("PlatformOptionAuthorizeOffSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_AlignType,make_pair(_T("对位方式设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_ALIGNTYPE,_T("PlatformOptionAuthorizeAlignType")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_AlignOption,make_pair(_T("对位参数设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_ALIGNOPTION,_T("PlatformOptionAuthorizeAlignOption")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_PlatformComm,make_pair(_T("平台通信设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_PLATFORMCOMM,_T("PlatformOptionAuthorizePlatformComm")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_SnapSet,make_pair(_T("拍照设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_SNAPSET,_T("PlatformOptionAuthorizeSnapSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_PickSet,make_pair(_T("拾取设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_PICKSET,_T("PlatformOptionAuthorizePickSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_CalibrationSet,make_pair(_T("标定关联设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_CALIBRATIONSET,_T("PlatformOptionAuthorizeCalibrationSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_Inspect,make_pair(_T("检测设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_INSPECTSET,_T("PlatformOptionAuthorizeInspectSet")))));
	m_MapAuthorizeItem.insert(make_pair(ePlatformOptionAuthorize_More,make_pair(_T("更多设置"),make_pair(IDS_CMB_PLATFORMOPTIONAUTHORIZE_MORE_SET,_T("PlatformOptionAuthorizeMoreSet")))));

	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_Node,make_pair(_T("软件选项"),make_pair(IDS_CMB_OPTIONAUTHORIZE_NODE,_T("OptionAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_Language,make_pair(_T("语言设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_LANGUAGE,_T("OptionAuthorizeLanguage")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_StartSet,make_pair(_T("启动设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_STARTSET,_T("OptionAuthorizeStartSet")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_KeyBoard,make_pair(_T("键盘设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_KEYBOARD,_T("OptionAuthorizeKeyBoard")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_LogShowSet,make_pair(_T("信息显示设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_LOGSHOWSET,_T("OptionAuthorizeLogShowSet")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_SystemSwitch,make_pair(_T("系统切换"),make_pair(IDS_CMB_OPTIONAUTHORIZE_SYSTEMSWITCH,_T("OptionAuthorizeSystemSwitch")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_CheckModeSet,make_pair(_T("检查模式"),make_pair(IDS_CMB_OPTIONAUTHORIZE_CHECKMODESET,_T("OptionAuthorizeCheckModeSet")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_Interface,make_pair(_T("界面显示设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_INTERFACE,_T("OptionAuthorizeInterface")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_WSlayout,make_pair(_T("工位布局设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_WSLAYOUT,_T("OptionAuthorizeWSlayout")))));
	m_MapAuthorizeItem.insert(make_pair(eOptionAuthorize_OtherSet,make_pair(_T("其他设置"),make_pair(IDS_CMB_OPTIONAUTHORIZE_OTHEROPTIONSET,_T("OptionAuthorizeOtherSet")))));
	
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_Node,make_pair(_T("标定设置"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_NODE,_T("CalibrateAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_XYDMove,make_pair(_T("标定输入参数"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_XYDMOVE,_T("CalibrateAuthorizeXYDMove")))));
	//m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_DelayTime,make_pair(_T("标定延时"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_DELAYTIME,_T("CalibrateAuthorizeDelayTime")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_DistortionCorrection,make_pair(_T("畸变矫正"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_DISTORTIONCORRECTION,_T("CalibrateAuthorizeDistortionCorrection")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_TemplateSet,make_pair(_T("模板设置"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_TEMPLATESET,_T("CalibrateAuthorizeTemplateSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_CalibrationAlgorithm,make_pair(_T("标定算法"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_CALIBRATIONALGORITHM,_T("CalibrateAuthorizeCalibrationAlgorithm")))));
	//m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_CalibWhole,make_pair(_T("全局算法标定计算设置"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_CALIBWHOLE,_T("CalibrateAuthorizeCalibWhole")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_DmCodeSet,make_pair(_T("二维码标定设置"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_DMCODESET,_T("CalibrateAuthorizeDmCodeSet")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_CopyCalib,make_pair(_T("拷贝标定"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_COPYCALIB,_T("CalibrateAuthorizeCopyCalib")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_CalibList,make_pair(_T("标定产品列表"),make_pair(IDS_CMB_CALIBRATEAUTHORIZE_CALIBLIST,_T("CalibrateAuthorizeCalibList")))));

	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_Node,make_pair(_T("产品设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_NODE,_T("ProductAuthorizeNode")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_CheckMode,make_pair(_T("检查模式"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_CHECKMODE,_T("ProductAuthorizeCheckMode")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_Offset,make_pair(_T("对位补偿"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_OFFSET,_T("ProductAuthorizeOffset")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_AlignToolParamSet,make_pair(_T("对位延时、精度"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_ALIGNTOOLPARAMSET,_T("ProductAuthorizeAlignToolParamSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_AlignTypeSet,make_pair(_T("对位模式切换设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_ALIGNTYPESET,_T("ProductAuthorizeAlignTypeSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_AlignOther,make_pair(_T("其他对位设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_ALIGNOTHER,_T("ProductAuthorizeAlignOther")))));
	m_MapAuthorizeItem.insert(make_pair(eCalibrateAuthorize_ProudctList,make_pair(_T("产品列表设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_PROLISTSET,_T("ProductAuthorizeProductListSet")))));
	
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_TargetTemplateSet,make_pair(_T("目标模板设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_TARGET_TEMPLATESET,_T("ProductAuthorizeTargetTemplateSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_ObjectTemplateSet,make_pair(_T("对象模板设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_OBJECT_TEMPLATESET,_T("ProductAuthorizeObjectTemplateSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_VirtualObjectTemplateSet,make_pair(_T("虚拟对象模板设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_VIRTUALOBJCT_TEMPLATESET,_T("ProductAuthorizeVirtualObjectTemplateSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_CalibTargetTemplateSet,make_pair(_T("关联模板设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_CALIBTARGET_TEMPLATESET,_T("ProductAuthorizeCalibTargetTemplateSet")))));
	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_InspectTemplateSet,make_pair(_T("检测模板设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_INSPECT_TEMPLATESET,_T("ProductAuthorizeInspectTemplateSet")))));

	m_MapAuthorizeItem.insert(make_pair(eProductAuthorize_SpecialApplicationSet,make_pair(_T("特殊类应用设置"),make_pair(IDS_CMB_PRODUCTAUTHORIZE_SPECIAL_APPLICATION_SET,_T("ProductAuthorizeSpecialApplicationSet")))));

	m_MapAuthorizeItem.insert(make_pair(eOtherAuthorize_Node,make_pair(_T("其他设置"),make_pair(IDS_CMB_OTHERAUTHORIZE_NODE,_T("OtherAuthorizeNode")))));

	DeleteTree();
	CPermissionEntries* Root= new CPermissionEntries();
	Root->m_StrName = _T("权限管理");//根节点
	m_cTreeAuthorizeManagement = CTree<CPermissionEntries*>(Root);

	for (int iNode =eCamremaAuthorize_Node; iNode<eAuthorizeGroup_Other;iNode+=Group_Max_Num )
	{
		CTree<CPermissionEntries*> LayeriNode = CreateAuthorizeTree((CAuthorizeItem)iNode);
		m_cTreeAuthorizeManagement.AddChild(LayeriNode);
	}

	m_vKeyenceCodeParam.resize(MaxCodeNums);

	m_vKeyenceCodeParam[0].m_strCmd = _T("FAAL");
	m_vKeyenceCodeParam[0].m_nCmdVal = (int)eKeyenceCmdFAAL;
	m_vKeyenceCodeParam[0].P1[0] = 1;
	m_vKeyenceCodeParam[0].P2[0] = 1;

	// 扩展标定
	m_vKeyenceCodeParam[1].m_strCmd = _T("FCLB");
	m_vKeyenceCodeParam[1].m_nCmdVal = (int)eKeyenceCmdCalibrate;
	m_vKeyenceCodeParam[1].P1[0] = 1;
	m_vKeyenceCodeParam[1].P2[0] = 1;
	m_vKeyenceCodeParam[1].nP3[0] = 1;

	m_vKeyenceCodeParam[2].m_strCmd = _T("FOBC");
	m_vKeyenceCodeParam[2].m_nCmdVal = (int)eKeyenceCmdFOBC;

	//m_vKeyenceCodeParam[2].P1.resize(2);
	//m_vKeyenceCodeParam[2].P2.resize(2,1);
	//m_vKeyenceCodeParam[2].nP3.resize(2,0);
	//m_vKeyenceCodeParam[2].nP4.resize(2,0);
	//m_vKeyenceCodeParam[2].nP5.resize(2,0);

	m_vKeyenceCodeParam[2].P1[0] = 5;
	m_vKeyenceCodeParam[2].P2[0] = 1;

	m_vKeyenceCodeParam[3].m_strCmd = _T("FTGC");
	m_vKeyenceCodeParam[3].m_nCmdVal = (int)eKeyenceCmdFTGC;
	m_vKeyenceCodeParam[3].P1[0] = 5;
	m_vKeyenceCodeParam[3].P2[0] = 1;

	m_vKeyenceCodeParam[4].m_strCmd = _T("FSPC");
	m_vKeyenceCodeParam[4].m_nCmdVal = (int)eKeyenceCmdFSPC;
	m_vKeyenceCodeParam[4].P1[0] = 1;
	m_vKeyenceCodeParam[4].P2[0] = 1;

	m_vKeyenceCodeParam[5].m_strCmd = _T("FCHB");
	m_vKeyenceCodeParam[5].m_nCmdVal = (int)eKeyenceCmdFCHB;
	m_vKeyenceCodeParam[5].P1[0] = 1;
	m_vKeyenceCodeParam[5].P2[0] = 1;

	m_vKeyenceCodeParam[6].m_strCmd = _T("FALP");
	m_vKeyenceCodeParam[6].m_nCmdVal = (int)eKeyenceCmdFALP;;
	m_vKeyenceCodeParam[6].P1[0] = 1;
	m_vKeyenceCodeParam[6].P2[0] = 1;

	m_vKeyenceCodeParam[7].m_strCmd = _T("FAAC");
	m_vKeyenceCodeParam[7].m_nCmdVal = (int)eKeyenceCmdFAAC;
	m_vKeyenceCodeParam[7].P1[0] = 1;
	m_vKeyenceCodeParam[7].P2[0] = 1;

	m_vKeyenceCodeParam[8].m_strCmd = _T("FMCO");
	m_vKeyenceCodeParam[8].m_nCmdVal = (int)eKeyenceCmdFMCO;
	m_vKeyenceCodeParam[8].P1[0] = 1;
	m_vKeyenceCodeParam[8].P2[0] = 1;

	m_vKeyenceCodeParam[9].m_strCmd = _T("FCBP");
	m_vKeyenceCodeParam[9].m_nCmdVal = (int)eKeyenceCmdFCBP;

	//
	int nProcNums = 4;
	m_vKeyenceCodeParam[9].P1.resize(nProcNums);
	m_vKeyenceCodeParam[9].P2.resize(nProcNums,1);
	m_vKeyenceCodeParam[9].nP3.resize(nProcNums,0);
	m_vKeyenceCodeParam[9].nP4.resize(nProcNums,0);
	m_vKeyenceCodeParam[9].nP5.resize(nProcNums,0);
	
	// 轴获取
	m_vKeyenceCodeParam[9].P1[0] = 1;
	m_vKeyenceCodeParam[9].P2[0] = 0;
	
	// 对象拍照
	m_vKeyenceCodeParam[9].P1[1] = 0;
	m_vKeyenceCodeParam[9].P2[1] = 12;
	// 目标拍照
	m_vKeyenceCodeParam[9].P1[2] = 0;
	m_vKeyenceCodeParam[9].P2[2] = 13;

	// 执行关联
	m_vKeyenceCodeParam[9].P1[nProcNums-1] = 2;
	m_vKeyenceCodeParam[9].P2[nProcNums-1] = 0;


	// FCBS
	m_vKeyenceCodeParam[10].m_strCmd = _T("FCBS");
	m_vKeyenceCodeParam[10].m_nCmdVal = (int)eKeyenceCmdFCBS;
	nProcNums = 3;
	m_vKeyenceCodeParam[10].P1.resize(nProcNums);
	m_vKeyenceCodeParam[10].P2.resize(nProcNums,0);
	m_vKeyenceCodeParam[10].nP3.resize(nProcNums,0);
	m_vKeyenceCodeParam[10].nP4.resize(nProcNums,0);
	m_vKeyenceCodeParam[10].nP5.resize(nProcNums,0);
	// 标定XY
	m_vKeyenceCodeParam[10].P1[0] = 0;
	// 标定D
	m_vKeyenceCodeParam[10].P1[1] = 1;
	// 执行标定
	m_vKeyenceCodeParam[10].P1[2] = 2;


	m_vKeyenceCodeParam[11].m_strCmd = _T("FACO");
	m_vKeyenceCodeParam[11].m_nCmdVal = (int)eKeyenceCmdFACO;
	m_vKeyenceCodeParam[11].P1[0] = 1;
	m_vKeyenceCodeParam[11].P2[0] = 1;

	m_vKeyenceCodeParam[12].m_strCmd = _T("FPMI");
	m_vKeyenceCodeParam[12].m_nCmdVal = (int)eKeyenceCmdFPMI;
	m_vKeyenceCodeParam[12].P1[0] = 1;
	m_vKeyenceCodeParam[12].P2[0] = 1;

	m_vKeyenceCodeParam[13].m_strCmd = _T("FBIS");
	m_vKeyenceCodeParam[13].m_nCmdVal = (int)eKeyenceCmdFBIS;
	m_vKeyenceCodeParam[13].P1[0] = 5;
	m_vKeyenceCodeParam[13].P2[0] = 101;


}
BOOL CVisionASMConfig::CheckProductDataCalibDataConsistency()
{
	// 检查总数目个数一致性
	if (m_TotalProductData.GetCount() != m_TotalCalibData.GetCount())
	{
		return FALSE;
	}

	// 检查当前产品索引
	if (m_TotalProductData.GetCurProductIndex()!= m_TotalCalibData.GetCurCalibIndex())
	{
		return FALSE;
	}

	// 检查父产品个数一致性
	if (m_TotalProductData.GetProductCountFa() != m_TotalCalibData.GetCalibCountFa())
	{
		return FALSE;
	}

	// 检查每个父产品下子产品个数一致性
	for (int it=0;it<m_TotalProductData.GetProductCountFa();it++)
	{
		if (m_TotalProductData.GetProductCountChd(it) != m_TotalCalibData.GetCalibCountChd(it))
		{
			return FALSE;
		}
	}

	// 检查产品的大小品种索引的一致性
	for (int it=0;it<m_TotalProductData.GetCount();it++)
	{
		int nProFa = 0;
		int nProCh = 0;
		int nCaliFa = 0;
		int nCaliCh= 0;
		if (!m_TotalProductData.GetDlgProductIndex(it,nProFa,nProCh))
		{
			return FALSE;
		}

		if (!m_TotalCalibData.GetDlgCalibIndex(it,nCaliFa,nCaliCh))
		{
			return FALSE;
		}

		if (nProFa!= nCaliFa || nProCh !=nCaliCh)
		{
			return FALSE;
		}
	}

	return TRUE;


}


CTree<CPermissionEntries*> CVisionASMConfig::CreateAuthorizeTree(CAuthorizeItem eItem )
{
	auto itFind = m_MapAuthorizeItem.find(eItem);
	if (itFind != m_MapAuthorizeItem.end())
	{
		CPermissionEntries* eNode= new CPermissionEntries();
		eNode->m_StrName = itFind->second.first;
		eNode->m_bGroup= true;
		eNode->m_nIndex = (int)eItem;
		CTree<CPermissionEntries*> Layer = CTree<CPermissionEntries*>(eNode);
		for (auto it = m_MapAuthorizeItem.begin();it != m_MapAuthorizeItem.end();it++)
		{
			if (it->first > eItem && it->first <eItem + Group_Max_Num)
			{
				CPermissionEntries* childItem = new CPermissionEntries();
				childItem->m_StrName = it->second.first;
				childItem->m_nIndex = (int)it->first;
				// 默认隐藏并禁用：平台选项中不常用项\通信类型设置的不常用项\标定畸变矫正\标定算法选择\标定产品列表\其他通信设置\ 其他对位设置 \特殊类应用设置
				if (ePlatformOptionAuthorize_More == it->first ||
					eCommAuthorize_OtherSet == it->first ||
					eCalibrateAuthorize_DistortionCorrection == it->first ||
					eCalibrateAuthorize_CalibrationAlgorithm == it->first ||
					eCalibrateAuthorize_CalibList == it->first  ||
					eCommAuthorize_OtherSet == it->first  ||
					eCommAuthorize_CommDelaySet == it->first ||
					eProductAuthorize_AlignOther == it->first ||
					eProductAuthorize_SpecialApplicationSet == it->first ||
					eProductAuthorize_InspectTemplateSet== it->first)
				{
					childItem->m_bShow =false;
					childItem->m_bEnable =false;
				}

				Layer.AddChild(childItem);
			}
		}

		return Layer;
	}
	else
	{
		return NULL;
	}
}


void CVisionASMConfig::DeleteTree()
{
	// 首先清理子节点
	DeleteTreeChild(m_cTreeAuthorizeManagement.children);
	if (NULL != m_cTreeAuthorizeManagement.node)
	{
		delete m_cTreeAuthorizeManagement.node;
		m_cTreeAuthorizeManagement.node = NULL;
	}
}

void CVisionASMConfig::DeleteTreeChild(std::vector<CTree<CPermissionEntries*>> &children)
{
	for (UINT it=0;it < children.size();it++)
	{
		if (children.at(it).node != NULL)
		{
			DeleteTreeChild(children.at(it).children);
			delete children.at(it).node;
			children.at(it).node = NULL;
		}
	}

	children.clear();
}

bool CVisionASMConfig::FindItem(const CAuthorizeItem eItem,std::vector<CTree<CPermissionEntries*>> &children,CString &str,bool &bShow,bool &bEenable)
{
	if (children.size()>0)
	{
		for (UINT it=0;it < children.size();it++)
		{
			if (children.at(it).node == NULL)
			{
				continue;
			}

			if (eItem == children.at(it).node->m_nIndex)
			{
				bShow = children.at(it).node->m_bShow;
				bEenable = children.at(it).node->m_bEnable;
				str = children.at(it).node->m_StrName;
				return true;
			}
			else
			{
				bool bfind =  FindItem(eItem,children.at(it).children,str,bShow,bEenable);
				if (bfind)
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool CVisionASMConfig::GetCheckedByItem(const CAuthorizeItem eItem,CTree<CPermissionEntries*> m_cTreepair,CString &str,bool &bShow,bool &bEenable)
{
	return FindItem(eItem,m_cTreepair.children,str,bShow,bEenable);
}


void CVisionASMConfig::SaveToXML(vcXMLConfigurator& cXMLConfigurator,std::vector<CTree<CPermissionEntries*>> &children)
{

	if (children.size()>0)
	{
		CString StrFather = _T("");
		for (UINT it = 0;it<children.size();it++)
		{

			if (children.at(it).node == NULL)
			{
				continue;
			}

			int nIndex2 = children.at(it).node->m_nIndex;
			auto iterFind2 = m_MapAuthorizeItem.find(nIndex2);
			if (iterFind2 !=m_MapAuthorizeItem.end())
			{
				CString StrChild = iterFind2->second.second.second;
				if(FALSE == cXMLConfigurator.FindElem(StrChild))
				{
					cXMLConfigurator.AddElem(StrChild);
				}
				cXMLConfigurator.EnterElem();
				children.at(it).node->SaveModelToFile(cXMLConfigurator);
				{
					SaveToXML(cXMLConfigurator,children.at(it).children);
				}
				cXMLConfigurator.LeaveElem();
			}


		}
	}

}

void CVisionASMConfig::LoadFromXML(vcXMLConfigurator& cXMLConfigurator,std::vector<CTree<CPermissionEntries*>> &children)
{

	if (children.size()>0)
	{
		CString StrFather = _T("");
		for (UINT it = 0;it<children.size();it++)
		{

			if (children.at(it).node == NULL)
			{
				continue;
			}

			int nIndex2 = children.at(it).node->m_nIndex;
			auto iterFind2 = m_MapAuthorizeItem.find(nIndex2);
			if (iterFind2 !=m_MapAuthorizeItem.end())
			{
				CString StrChild = iterFind2->second.second.second;
				if(cXMLConfigurator.FindElem(StrChild))
				{
					cXMLConfigurator.EnterElem();
					children.at(it).node->LoadModelFromFile(cXMLConfigurator);
					{
						LoadFromXML(cXMLConfigurator,children.at(it).children);
					}
					cXMLConfigurator.LeaveElem();

				}
			}
			
		}
	}

}


CGuiGraphics::CGuiGraphics()
{
	m_vGuiObjectCross.resize(DMCODECORNERSNUM);
	for (int i=0;i<DMCODECORNERSNUM;i++)
	{
		m_vGuiObjectCross.at(i)= new scGuiCross;
		m_vGuiObjectCross.at(i)->SetLineColor(RGB(0,255,0));
		m_vGuiObjectCross.at(i)->SetLineWidth(2);
	}
	m_vbShowObjectCross.resize(DMCODECORNERSNUM,FALSE);

	//m_vGuiTargetCross.resize(DMCODECORNERSNUM);
	//for (int i=0;i<DMCODECORNERSNUM;i++)
	//{
	//	m_vGuiTargetCross.at(i)= new scGuiCross;
	//	m_vGuiTargetCross.at(i)->SetLineColor(RGB(0,255,0));
	//	m_vGuiTargetCross.at(i)->SetLineWidth(2);
	//}
	//m_vbShowTargetCross.resize(DMCODECORNERSNUM,FALSE);

	m_vGuiObjectCoordCross.resize(DMCODECORNERSNUM);
	for (int i=0;i<DMCODECORNERSNUM;i++)
	{
		m_vGuiObjectCoordCross.at(i)= new scGuiCoordCross;
		m_vGuiObjectCoordCross.at(i)->SetLineColor(RGB(128,0,255));
		m_vGuiObjectCoordCross.at(i)->SetLineWidth(1);
	}
	m_vbShowObjectCoordCross.resize(DMCODECORNERSNUM,FALSE);

	//m_vGuiTargetCoordCross.resize(DMCODECORNERSNUM);
	//for (int i=0;i<DMCODECORNERSNUM;i++)
	//{
	//	m_vGuiTargetCoordCross.at(i)= new scGuiCoordCross;
	//	m_vGuiTargetCoordCross.at(i)->SetLineColor(RGB(128,0,255));
	//	m_vGuiTargetCoordCross.at(i)->SetLineWidth(1);
	//}
	//m_vbShowTargetCoordCross.resize(DMCODECORNERSNUM,FALSE);

	m_vGuiObjectPolyline.resize(1);
	for (int i=0;i<1;i++)
	{
		m_vGuiObjectPolyline.at(i)= new scGuiPolyline;
		m_vGuiObjectPolyline.at(i)->SetLineColor(RGB(255,0,255));
	}
	m_vbShowObjectPolyline.resize(1,FALSE);

	//m_vGuiTargetPolyline.resize(1);
	//for (int i=0;i<1;i++)
	//{
	//	m_vGuiTargetPolyline.at(i)= new scGuiPolyline;
	//	m_vGuiTargetPolyline.at(i)->SetLineColor(RGB(255,0,255));
	//}
	//m_vbShowTargetPolyline.resize(1,FALSE);

}
CGuiGraphics::~CGuiGraphics()
{
	for (int i=0;i<m_vGuiObjectCross.size();i++)
	{
		if (m_vGuiObjectCross.at(i)!=NULL)
		{
			delete m_vGuiObjectCross.at(i);
			m_vGuiObjectCross.at(i) = NULL;
		}	
	}

	for (int i=0;i<m_vGuiTargetCross.size();i++)
	{
		if (m_vGuiTargetCross.at(i)!=NULL)
		{
			delete m_vGuiTargetCross.at(i);
			m_vGuiTargetCross.at(i) = NULL;
		}
	}

	for (int i=0;i<m_vGuiObjectCoordCross.size();i++)
	{
		if (m_vGuiObjectCoordCross.at(i)!=NULL)
		{
			delete m_vGuiObjectCoordCross.at(i);
			m_vGuiObjectCoordCross.at(i) = NULL;
		}	
	}
	for (int i=0;i<m_vGuiTargetCoordCross.size();i++)
	{
		if (m_vGuiTargetCoordCross.at(i)!=NULL)
		{
			delete m_vGuiTargetCoordCross.at(i);
			m_vGuiTargetCoordCross.at(i) = NULL;
		}	
	}

	for (int i=0;i<m_vGuiObjectPolyline.size();i++)
	{
		if (m_vGuiObjectPolyline.at(i)!=NULL)
		{
			delete m_vGuiObjectPolyline.at(i);
			m_vGuiObjectPolyline.at(i) = NULL;
		}
	}

	for (int i=0;i<m_vGuiTargetPolyline.size();i++)
	{
		if (m_vGuiTargetPolyline.at(i)!=NULL)
		{
			delete m_vGuiTargetPolyline.at(i);
			m_vGuiTargetPolyline.at(i) = NULL;
		}
	}
}
BOOL CreateDirectoryPro( LPCTSTR lpszDir )
{
	CString strDir = lpszDir;
	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{	
		// 创建多级目录
		int nLen = 0;
		DWORD dwLevelXAttrib = 0;
		int nSlashIdx = strDir.Find('\\');
		CString strDirLevelX = _T("");
		CString strTemp = strDir;
		while (nSlashIdx != -1)
		{
			nLen = strDirLevelX.GetLength();
			nSlashIdx += (nLen == 0) ? 0 : (nLen + 1);
			strDirLevelX = strDir.Left(nSlashIdx);
			strTemp = strDir.Mid(nSlashIdx + 1);
			dwLevelXAttrib = GetFileAttributes(strDirLevelX);
			if ((dwLevelXAttrib == 0xFFFFFFFF) || ((dwLevelXAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0))
			{
				if (!CreateDirectory(strDirLevelX, NULL))
				{
					return FALSE;
				}
			}
			nSlashIdx = strTemp.Find('\\');
		}
		if (!CreateDirectory(strDir, NULL))
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ClearDirectoryPro( LPCTSTR lpszDir )
{
	CFileFind fileFind; 
	CString strDir(lpszDir);
	CString strPath = strDir + _T("\\*.*");
	BOOL bFound = fileFind.FindFile(strPath); 
	while (bFound) 
	{ 
		bFound = fileFind.FindNextFile(); 

		if (!fileFind.IsDots()) 
		{ 
			CString strFileName = fileFind.GetFileName();
			CString strSubFile = strDir + _T("\\") + strFileName;
			if (fileFind.IsDirectory()) 
			{ 
				if (!ClearDirectoryPro(strSubFile)) //先删除文件夹下边的所有文件
				{
					return FALSE;
				}
				if (!RemoveDirectory(strSubFile)) // 删除该文件夹
				{
					return FALSE;
				}
			} 
			else 
			{ 
				if (!DeleteFile(strSubFile))	//将该文件夹删除
				{
					return FALSE;
				}
			} 
		}
	} 
	fileFind.Close(); 
	return TRUE; 
}

BOOL RemoveDirectoryPro( LPCTSTR lpszDir )
{
	DWORD dwAttributes = GetFileAttributes(lpszDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return TRUE;
	}

	if (!ClearDirectoryPro(lpszDir))
	{
		return FALSE;
	}
	if (!RemoveDirectory(lpszDir))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL RenameDirecotryPro( LPCTSTR lpszOldDir, LPCTSTR lpszNewDir )
{
	CString strOldDir = lpszOldDir;
	CString strNewDir = lpszNewDir;
	if (strOldDir.IsEmpty() || strNewDir.IsEmpty())
	{
		return FALSE;
	}

	// 如果文件夹不存在，则返回TRUE
	DWORD dwAttributes = GetFileAttributes(strOldDir);
	if ((dwAttributes == 0xFFFFFFFF) || ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0))
	{
		return FALSE;
	}

	// 文件夹存在，则重命名文件夹
	SHFILEOPSTRUCT shFileOp={0};
	shFileOp.fFlags = FOF_NOCONFIRMATION; // 不出现确认对话框
	TCHAR pszzDir[MAX_PATH] = {0};
	TCHAR *p = strOldDir.GetBuffer(strOldDir.GetLength());
	_tcscat(pszzDir, p);
	shFileOp.pFrom	= pszzDir; 
	shFileOp.pTo	= lpszNewDir;	
	shFileOp.wFunc	= FO_RENAME;	//重命名操作
	if (SHFileOperation(&shFileOp) != 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


void AlignToolLogRecord(CString strlog,int nPlatformIndex)
{
	if (1)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString t;
		t.Format("[%02d_%02d_%02d_%02d_%02d_%03d]",sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
		CString strPath;


		CString strTemp = "";
		strTemp += t;
		strTemp += strlog;
		strTemp+= "\n";
		FILE *stream;

		CString strDir;
		CFileFind findFile;
		BOOL IsFild, IsDir;
		IsFild = FALSE;
		IsDir = FALSE;

		strDir  = "D:\\VisionASMLog";

		IsFild = findFile.FindFile(strDir);
		while (IsFild)
		{
			IsFild = findFile.FindNextFile();
			IsDir = findFile.IsDirectory();
			if(IsDir) break;
		}	
		if (!IsDir)
		{
			if (!CreateDirectory(strDir, NULL))// 创建文件夹
			{

				return ;
			}
		}


		strPath.Format("D:\\VisionASMLog\\VisionASMInfo_Platform%d.txt",nPlatformIndex);
		stream = fopen(strPath, _T("a+t"));
		if (stream != NULL)
		{
			fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
			fclose(stream);	
		}

		CFileStatus fs;
		CFile::GetStatus(strPath,fs);
		if (fs.m_size > 5000000)//1048576)
		{
			::DeleteFile(strPath);
		}
	}
}
//
////20160510增加对角补偿
//bool AngleOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, int nPosMode, scDegree& angleOffset)
//{
//	if ( (nPosMode < 0) || (nPosMode > 1) )
//	{
//		return false;
//	}
//
//	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) ||(dTargetL2 <= 0) ||(dTargetW2 <= 0) )
//	{
//		return false;
//	}
//
//	sc2Vector objectP1, objectP2, targetP1, targetP2;
//	if (nPosMode == 0)
//	{
//		objectP1 = sc2Vector(-0.5 * dObjectL1, -0.5 * dObjectW1);
//		objectP2 = sc2Vector( 0.5 * dObjectL1,  0.5 * dObjectW1);
//		targetP1 = sc2Vector(-0.5 * dTargetL2, -0.5 * dTargetW2);
//		targetP2 = sc2Vector( 0.5 * dTargetL2,  0.5 * dTargetW2);	
//	}
//	else if (nPosMode == 1)
//	{
//		objectP1 = sc2Vector( 0.5 * dObjectL1, -0.5 * dObjectW1);
//		objectP2 = sc2Vector(-0.5 * dObjectL1,  0.5 * dObjectW1);
//		targetP1 = sc2Vector( 0.5 * dTargetL2, -0.5 * dTargetW2);
//		targetP2 = sc2Vector(-0.5 * dTargetL2,  0.5 * dTargetW2);	
//	}
//
//	scLine objectLine, targetLine;
//	objectLine.SetFromStartEnd(objectP1, objectP2);
//	targetLine.SetFromStartEnd(targetP1, targetP2);
//
//	scRadian radianAngle;
//	sfAngleLine2Line(objectLine, targetLine, radianAngle);
//
//	angleOffset = scDegree(radianAngle);
//	angleOffset.SignedNormMod180();
//
//	return true;
//}
//
////20160510增加对角补偿
//bool XYOffsetCalculate(double dObjectL1, double dObjectW1, double dTargetL2, double dTargetW2, double dF1, double dF2, int nPosMode, double& dOffsetX, double& dOffsetY)
//{
//	if ( (nPosMode < 0) || (nPosMode > 1) )
//	{
//		return false;
//	}
//
//	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) ||(dTargetL2 <= 0) ||(dTargetW2 <= 0) )
//	{
//		return false;
//	}
//
//	double dLeft = dF1;
//	double dRight = dTargetL2 - dObjectL1 - dF1;
//	dOffsetX = (dRight - dLeft)*0.5;
//
//	double dTop = dF2;
//	double dBottom = dTargetW2 - dObjectW1 - dF2;
//	dOffsetY = (dBottom - dTop)*0.5;
//
//	return true;
//}
//
////20160510增加对角补偿
//bool AngleDiagonalCalculate(double dObjectL1, double dObjectW1, int nPosMode, scDegree& angleDiagonal)
//{
//	if ( (nPosMode < 0) || (nPosMode > 1) )
//	{
//		return false;
//	}
//
//	if ( (dObjectL1 <= 0) ||(dObjectW1 <= 0) )
//	{
//		return false;
//	}
//
//	sc2Vector objectP1, objectP2;
//	if (nPosMode == 0)
//	{
//		objectP1 = sc2Vector(-0.5 * dObjectL1, -0.5 * dObjectW1);
//		objectP2 = sc2Vector( 0.5 * dObjectL1,  0.5 * dObjectW1);
//	}
//	else if (nPosMode == 1)
//	{
//		objectP1 = sc2Vector( 0.5 * dObjectL1, -0.5 * dObjectW1);
//		objectP2 = sc2Vector(-0.5 * dObjectL1,  0.5 * dObjectW1);
//	}
//
//	scLine objectLine;
//	objectLine.SetFromStartEnd(objectP1, objectP2);
//
//	angleDiagonal = scDegree(objectLine.GetRotation()).SignedNormMod180();
//
//	return true;
//}

//20161020从字符串数组获取字符串
CString GetSysInfoString(CStringArray *psaSysInfoStrings, int nIndex)
{
	if (psaSysInfoStrings == NULL)
	{
		return _T("");
	}

	if ( (nIndex < 0) || (nIndex >= psaSysInfoStrings->GetSize()) )
	{
		return _T("");
	}

	return psaSysInfoStrings->GetAt(nIndex);
}

CString GetLogDirecotryPath()
{
	///////////////////////////////////////////////////////////////////////////
	// 	// 获取当前EXE文件夹的路径
	CString strEXEDir, m_strEXEDir, strDir;
	GetModuleFileName(NULL, strEXEDir.GetBuffer(MAX_PATH), MAX_PATH);    
	strEXEDir.ReleaseBuffer();

	m_strEXEDir = strEXEDir.Left(strEXEDir.ReverseFind('\\'));   

	///////////////////////////////////////////////////////////////////////////
	strDir = m_strEXEDir + _T("\\VSLog");

	return strDir;
}


CShareCommInfoArray::CShareCommInfoArray()
{

}
CShareCommInfoArray::~CShareCommInfoArray()
{
	ClearPlatShareCommInfo();
}

void CShareCommInfoArray::ClearPlatShareCommInfo()
{
	for (int i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		m_vpShareCommInfos[i]->m_vShareComms.clear();
		delete m_vpShareCommInfos[i];
		m_vpShareCommInfos[i] = NULL;
	}
	m_vpShareCommInfos.clear();
}

// 添加平台共享通信信息，即设置所有的共享通信信息
void CShareCommInfoArray::AddPlatShareComm(int nPlatformIndex, int nShareCommIndex, BOOL bShareCommEnable)
{
	if (!bShareCommEnable)
	{
		return ;
	}
	int i = 0;
	int j = 0;

	BOOL bShareCommIndexExist = FALSE;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		if (nShareCommIndex == m_vpShareCommInfos[i]->m_nShareCommIndex)
		{
			CPlatShareComm platShareComm;
			platShareComm.m_nShareCommIndex = nShareCommIndex;
			platShareComm.m_nPlatformIndex = nPlatformIndex;
			m_vpShareCommInfos[i]->m_vShareComms.push_back(platShareComm);
			bShareCommIndexExist = TRUE;
			break;
		}
	}

	if (!bShareCommIndexExist)
	{
		CPlatShareComm platShareComm;
		platShareComm.m_nShareCommIndex = nShareCommIndex;
		platShareComm.m_nPlatformIndex = nPlatformIndex;

		CShareCommInfo* pShareCommInfo = new CShareCommInfo;
		pShareCommInfo->m_nShareCommIndex = nShareCommIndex;
		pShareCommInfo->m_vShareComms.push_back(platShareComm);

		m_vpShareCommInfos.push_back(pShareCommInfo);
	}

	// 更新nShareCommIndex对应的ShareCommInfos
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		if (nShareCommIndex == m_vpShareCommInfos[i]->m_nShareCommIndex)
		{			
			for(j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
			{
				if (j == 0)
				{
					m_vpShareCommInfos[i]->m_vShareComms[j].m_eCommShareType = eShareCommSrc;
				}
				else
				{
					m_vpShareCommInfos[i]->m_vShareComms[j].m_eCommShareType = eShareCommTar;
				}				
			}
			m_vpShareCommInfos[i]->m_nCurCommPlatformIndex = m_vpShareCommInfos[i]->m_vShareComms[0].m_nPlatformIndex;
			m_vpShareCommInfos[i]->m_nShareCommPlatformNum = m_vpShareCommInfos[i]->m_vShareComms.size();
			
			return;
		}
	}
}


// 通过平台序号，查询其是否源通信接口
ShareCommType CShareCommInfoArray::GetShareCommType(int nPlatformIndex)
{
	int i = 0;
	int j = 0;

	ShareCommType eShareCommType = eShareCommNo;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				eShareCommType = m_vpShareCommInfos[i]->m_vShareComms[j].m_eCommShareType;
				
				return eShareCommType;
			}
		}
	}

	return eShareCommType;
}

// 通过平台序号，查询其对应的共享通信接口序号
int CShareCommInfoArray::GetShareCommIndex(int nPlatformIndex)
{
	int i = 0;
	int j = 0;

	int nShareCommIndex = -1;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				nShareCommIndex = m_vpShareCommInfos[i]->m_nShareCommIndex;	
				
				return nShareCommIndex;
			}
		}
	}

	return nShareCommIndex;
}

// 通过平台序号，查询其对应的共享通信接口序号
int CShareCommInfoArray::GetShareCommSrcPlatformIndex(int nPlatformIndex)
{
	int i = 0;
	int j = 0;

	int nShareCommSrcIndex = -1;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				if (m_vpShareCommInfos[i]->m_vShareComms.size() > 0)
				{
					nShareCommSrcIndex = m_vpShareCommInfos[i]->m_vShareComms[0].m_nPlatformIndex;	
				}
				
				return nShareCommSrcIndex;
			}
		}
	}

	return nShareCommSrcIndex;
}

// 通过平台序号，查询当前允许通信的平台序号
int CShareCommInfoArray::GetCurCommPlatformIndex(int nPlatformIndex)
{
	int i = 0;
	int j = 0;

	int nCurCommPlatformIndex = -1;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				nCurCommPlatformIndex = m_vpShareCommInfos[i]->m_nCurCommPlatformIndex;		
				return nCurCommPlatformIndex;
			}
		}
	}

	return nCurCommPlatformIndex;
}

// 通过平台序号，查询当前允许通信的平台序号
int CShareCommInfoArray::GetNextCommPlatformIndex(int nPlatformIndex)
{
	int i = 0;
	int j = 0;
	int nCurCommPlatformIndex = -1;
	nCurCommPlatformIndex = GetCurCommPlatformIndex(nPlatformIndex);

	int nNextCommPlatformIndex = -1;
	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nCurCommPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				if (j ==  m_vpShareCommInfos[i]->m_vShareComms.size()-1)
				{
					nNextCommPlatformIndex = m_vpShareCommInfos[i]->m_vShareComms[0].m_nPlatformIndex;	
				}
				else
				{
					int nNextIndex = j;
					nNextIndex++;
					nNextCommPlatformIndex = m_vpShareCommInfos[i]->m_vShareComms[nNextIndex].m_nPlatformIndex;
				}
					
				return nNextCommPlatformIndex;
			}
		}
	}

	return nNextCommPlatformIndex;
}

// 通过平台序号，设置当前允许通信的平台序号
BOOL CShareCommInfoArray::SetCurCommPlatformIndex(int nPlatformIndex, int nCurCommPlatformIndex, BOOL bChangeEnable/* = TRUE*/)
{
	int i = 0;
	int j = 0;

	if (GetShareCommIndex(nPlatformIndex) != GetShareCommIndex(nCurCommPlatformIndex))
	{
		return FALSE;
	}

	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				m_vpShareCommInfos[i]->Lock();
				
				// 只有源，可以修改写入状态
				if (j == 0)
				{
					m_vpShareCommInfos[i]->m_bCurCommChangeEnable = bChangeEnable;
				}	

				// 只有源，或是处于可写入状态，才可以修改当前平台
				if (j == 0 || m_vpShareCommInfos[i]->m_bCurCommChangeEnable)
				{
					m_vpShareCommInfos[i]->m_nCurCommPlatformIndex = nCurCommPlatformIndex;
				}
			
				m_vpShareCommInfos[i]->Unlock();
				return TRUE;
			}
		}
	}

	return FALSE;
}


// 加锁当前平台通信 
BOOL CShareCommInfoArray::LockCurPlatformIndex(int nCurCommPlatformIndex)
{

	if (GetShareCommType(nCurCommPlatformIndex) == eShareCommNo)
	{
		return TRUE;
	}

	int i = 0;
	int j = 0;

	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nCurCommPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				m_vpShareCommInfos[i]->Lock();
								
				return TRUE;
			}
		}
	}

	return TRUE;
}


// 解锁当前平台通信
BOOL CShareCommInfoArray::UnlockCurPlatformIndex(int nCurCommPlatformIndex)
{
	if (GetShareCommType(nCurCommPlatformIndex) == eShareCommNo)
	{
		return TRUE;
	}

	int i = 0;
	int j = 0;

	for (i = 0; i < m_vpShareCommInfos.size(); i++)
	{
		for (j = 0; j < m_vpShareCommInfos[i]->m_vShareComms.size(); j++)
		{
			if (nCurCommPlatformIndex == m_vpShareCommInfos[i]->m_vShareComms[j].m_nPlatformIndex)
			{
				m_vpShareCommInfos[i]->Unlock();

				return TRUE;
			}
		}
	}

	return TRUE;
}