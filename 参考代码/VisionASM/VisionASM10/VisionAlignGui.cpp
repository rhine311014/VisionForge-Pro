#include "stdafx.h"
#include "VisionAlignGui.h"
#include "vsBaseVisionAlign.h"


CVisionAlignGui::CVisionAlignGui()
{
	int m_nPosNum = 0;
	m_pVisionASM = NULL;
}
CVisionAlignGui::~CVisionAlignGui()
{
// 	m_nPosNum = 0;
// 	m_vpGuiDisplay.clear();	 
	for (int i = 0; i < m_vpObjectAlignGui.size(); i++)
	{
		if (m_vpObjectAlignGui[i])
		{
			delete m_vpObjectAlignGui[i];
			m_vpObjectAlignGui[i] = NULL;
		}

		if (m_vpTargetAlignGui[i])
		{
			delete m_vpTargetAlignGui[i];
			m_vpTargetAlignGui[i] = NULL;
		}
	}

	for (int i = 0; i < m_vpAlignPosTextGui.size(); i++)
	{
		if (m_vpAlignPosTextGui[i] && !m_vbIsShareGuiShow[i])
		{
			delete m_vpAlignPosTextGui[i];
			m_vpAlignPosTextGui[i] = NULL;
		}
	}

	for (int i = 0; i < m_vpObjectToTargetAlignGui.size(); i++)
	{
		if (m_vpObjectToTargetAlignGui[i])
		{
			delete m_vpObjectToTargetAlignGui[i];
			m_vpObjectToTargetAlignGui[i] = NULL;
		}
	}

	for (int i = 0; i < m_vpAlignOffsetAreaGui.size(); i++)
	{
		if (m_vpAlignOffsetAreaGui[i])
		{
			delete m_vpAlignOffsetAreaGui[i];
			m_vpAlignOffsetAreaGui[i] = NULL;
		}
	}

	m_pVisionASM = NULL;
}


void CVisionAlignGui::SetVisionRef(vcBaseVisionAlign* pVisionASM)
{

	if (!pVisionASM)
	{
		for (int i = 0; i < m_vpObjectAlignGui.size(); i++)
		{
			if (m_vpObjectAlignGui[i])
			{
				delete m_vpObjectAlignGui[i];
				m_vpObjectAlignGui[i] = NULL;
			}

			if (m_vpTargetAlignGui[i])
			{
				delete m_vpTargetAlignGui[i];
				m_vpTargetAlignGui[i] = NULL;
			}
		}

		for (int i = 0; i < m_vpAlignPosTextGui.size(); i++)
		{
			if (m_vpAlignPosTextGui[i] && !m_vbIsShareGuiShow[i])
			{
				delete m_vpAlignPosTextGui[i];
				m_vpAlignPosTextGui[i] = NULL;
			}
		}

		for (int i = 0; i < m_vpObjectToTargetAlignGui.size(); i++)
		{
			if (m_vpObjectToTargetAlignGui[i])
			{
				delete m_vpObjectToTargetAlignGui[i];
				m_vpObjectToTargetAlignGui[i] = NULL;
			}
		}

		for (int i = 0; i < m_vpAlignOffsetAreaGui.size(); i++)
		{
			if (m_vpAlignOffsetAreaGui[i])
			{
				delete m_vpAlignOffsetAreaGui[i];
				m_vpAlignOffsetAreaGui[i] = NULL;
			}
		}
	}

	m_pVisionASM = pVisionASM;

	if (m_pVisionASM)
	{
		int nPosNum  = m_pVisionASM->GetPosNum();
		m_vpObjectAlignGui.resize(nPosNum);
		m_vpTargetAlignGui.resize(nPosNum);
		m_vpAlignPosTextGui.resize(nPosNum);
		m_vpObjectToTargetAlignGui.resize(nPosNum);
		m_vpAlignOffsetAreaGui.resize(nPosNum);
		m_vbIsShareGuiShow.resize(nPosNum, false);
		CString strShowInfo;

		SysPlatformInfo spi;
		m_pVisionASM->GetSysPlatformInfo(spi);

		for (int i = 0; i < nPosNum; i++)
		{
			m_vpObjectAlignGui[i] = new CVisionAlignPosGui;
			m_vpTargetAlignGui[i] = new CVisionAlignPosGui;
			m_vpObjectToTargetAlignGui[i] = new CVisionAlignPosCombineGui;
			m_vpAlignOffsetAreaGui[i] = new CVisionAlignPosOffsetAeraGui;

			if (spi.m_bPlatformShareShow)
			{
				m_vbIsShareGuiShow[i] = true;
				CVisionASMDlg* pMainDlg=(CVisionASMDlg*)(m_pVisionASM->GetMainFrameWnd());
				m_vpAlignPosTextGui[i] = pMainDlg->m_vpVisionASM[spi.m_nShareShowPlatformIndex]->m_cVisionAlignGui.m_vpAlignPosTextGui[i];
			}
			else
			{
				if (spi.m_nShowMode == 1)  //  按相机显示
				{
					int nCameraIndex = m_pVisionASM->GetPosCameraIndex(i);
					int nFirstPos = m_pVisionASM->GetCameraFirstPosIndex(nCameraIndex);
					if (nFirstPos == i)
					{
						m_vbIsShareGuiShow[i] = false;
						m_vpAlignPosTextGui[i] = new CVisionAlignMarkPosGui;
					}
					else
					{
						m_vbIsShareGuiShow[i] = true;
						m_vpAlignPosTextGui[i] = m_vpAlignPosTextGui[nFirstPos];
					}

				}
				else
				{
					m_vpAlignPosTextGui[i] = new CVisionAlignMarkPosGui;
				}
			}


			// 对象位置%d:(---,---)
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_OBJECT_POS),i+1);
			m_vpAlignPosTextGui[i]->InitObjectGui(strShowInfo);

			// 对象位置%d:(---,---)
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_TARGET_POS),i+1);
			m_vpAlignPosTextGui[i]->InitTargetGui(strShowInfo);
		}
	}
}


void CVisionAlignGui::SetVisionDisplay()
{
	if (!m_pVisionASM) return;

	int nPosNum = m_pVisionASM->GetPosNum();
	for (int i = 0; i < nPosNum; i++)
	{
		m_vpObjectAlignGui[i]->SetGuiDisplay(m_pVisionASM->GetPosDisplay(i));
		m_vpTargetAlignGui[i]->SetGuiDisplay(m_pVisionASM->GetPosDisplay(i));
		m_vpAlignPosTextGui[i]->SetGuiDisplay(m_pVisionASM->GetPosDisplay(i));
		m_vpObjectToTargetAlignGui[i]->SetGuiDisplay(m_pVisionASM->GetPosDisplay(i));
		m_vpAlignOffsetAreaGui[i]->SetGuiDisplay(m_pVisionASM->GetPosDisplay(i));
	}
}


void CVisionAlignGui::SetAllPosGuiInvisible(int eSearchMode)
{
	if (!m_pVisionASM)	return;
	std::vector<int> vnPosIndex;
	int nPosNum = m_pVisionASM->GetPosNum();
	for (int i = 0; i < nPosNum; i++)
	{
		vnPosIndex.push_back(i);
	}
	SetPosGuiInvisible(vnPosIndex, eSearchMode);
}


void CVisionAlignGui::SetPosGuiInvisible(std::vector<int>& vnPosIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	if (vnPosIndex.size() == 0)
	{
		int nPosNum = m_pVisionASM->GetPosNum();
		for (int i = 0; i < nPosNum; i++)
		{
			vnPosIndex.push_back(i);
		}
	}

	CString strShowInfo;
	CString strSearchResult;

	switch(eSearchMode)
	{
	case eObjectSearch:
	case eInspectSearch:
	case eCalibSearch:
	case eVirtualSearch:
		{
		   for (int i =0 ; i < vnPosIndex.size(); i++)
		   {
				m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiInvisible();

				strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_OBJECT_POS),vnPosIndex[i]+1);
				//strSearchResult = _T("对象定位结果:----");
				strSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_OBJECT_SEARCH_RESULT_SHOW);

				m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
				m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, RGB(0, 255, 0));
		   }
		}
		break;
	case eTargetSearch:
	case eCalibTargetSearch:
	case eAutoCalibTargetSearch:
		{
			for (int i =0 ; i < vnPosIndex.size(); i++)
			{
				m_vpTargetAlignGui[vnPosIndex[i]]->SetGuiInvisible();
				strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_TARGET_POS),vnPosIndex[i]+1);
				//strSearchResult = _T("目标定位结果:----");
				strSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_TARGET_SEARCH_RESULT_SHOW);

				m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetMarkPos(strShowInfo);
				m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetSearchToolResult(strSearchResult, RGB(0, 255, 0));
			}
		}
		break;
	}

	PostMessagePosGuiInvisible(vnPosIndex, eSearchMode);

	SetPosObjectToTargetResultLineInvisible(vnPosIndex);
}


void CVisionAlignGui::PostMessagePosGuiInvisible(std::vector<int>& vnPosIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	if(vnPosIndex.size() == 0)
	{
		int nPosNum  = m_pVisionASM->GetPosNum();
		for (int i = 0;i < nPosNum; i++)
		{
			vnPosIndex.push_back(i);
		}
	}
	int nPosNum = vnPosIndex.size();



	for (int i = 0; i < nPosNum; i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = FALSE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = eSearchMode;
		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALIGNER_GUI_SHOW, wParam, lParam);
	}
}

void CVisionAlignGui::UpdatePosGuiInvisible(int nPosIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
	CString strShowInfo;
	CString strSearchResult;

	switch(eSearchMode)
	{
	case eObjectSearch:
	case eInspectSearch:
	case eCalibSearch:
	case eVirtualSearch:
		{
			if (nPosIndex < nPosNum)
			{
				m_vpObjectAlignGui[nPosIndex]->UpdateGuiInvisible();
			}

		}
		break;
	case eTargetSearch:
	case eCalibTargetSearch:
	case eAutoCalibTargetSearch:
		{
			if (nPosIndex < nPosNum)
			{
				m_vpTargetAlignGui[nPosIndex]->UpdateGuiInvisible();
				
			}
		}
		break;
	case eCalibWholeDmcodeBoard:
		{
			// 二维码单独设置
		}
		break;
	}

	if (nPosIndex < nPosNum)
	{
		m_vpAlignPosTextGui[nPosIndex]->UpdateGuiShow();
	}
}



void CVisionAlignGui::SetAllPosSearchResult(int eSearchMode)
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);
		
	std::vector<int> vnPosIndex;
	switch(eSearchMode)
	{
	case eObjectSearch:
		{
			if (spi.m_bTargetObjectCamSeparate)
			{
				for (int i = 0; i < m_pVisionASM->GetPosNum()/2; i++)
				{
					vnPosIndex.push_back(i);
				}
			}
			else
			{
				for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
				{
					vnPosIndex.push_back(i);
				}
			}
		}
		break;
	case eInspectSearch:
		break;
	case eCalibSearch:
		break;
	case eVirtualSearch:
		break;
	case eTargetSearch:
		{
			if (spi.m_bTargetObjectCamSeparate)
			{
				if (spi.m_bPlatformGlue)
				{
					for (int i = 0; i < m_pVisionASM->GetPosNum()/2; i++)
					{
						vnPosIndex.push_back(i);
					}
				}
				else
				{
					for (int i = m_pVisionASM->GetPosNum()/2; i < m_pVisionASM->GetPosNum(); i++)
					{
						vnPosIndex.push_back(i);
					}
				}
			}
			else
			{
				for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
				{
					vnPosIndex.push_back(i);
				}
			}
		}
		break;
	case eCalibTargetSearch:
		break;
	case eAutoCalibTargetSearch:
		{

		}
		break;
	case eCalibWholeDmcodeBoard:
		{
			// 二维码单独设置
		}
		break;
	}

	SetPosSearchResult(vnPosIndex, eSearchMode);
}


void CVisionAlignGui::SetPosSearchResult(std::vector<int> vnPosIndex,  int eSearchMode)
{
	if (!m_pVisionASM)	return;

	std::vector<int> vnPattenIndex;
	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		vnPattenIndex.push_back(m_pVisionASM->GetAlnSearchPatIndex(vnPosIndex[i], eSearchMode));
	}

	SetPosSearchResult(vnPosIndex, vnPattenIndex, eSearchMode);
}

void CVisionAlignGui::SetPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_ObjSearchResultCrossSize);
		m_vpTargetAlignGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_TarSearchResultCrossSize);
	}


	int nPosNum  = m_pVisionASM->GetPosNum();
	CString strShowInfo;

	switch(eSearchMode)
	{
	case eObjectSearch:
		SetObjectPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eInspectSearch:
		SetInspectPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eCalibSearch:
		SetCalibPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eVirtualSearch:
		SetVirtualObjectPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eTargetSearch:
		SetTargetPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eCalibTargetSearch:
		SetCalibTargetSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eAutoCalibTargetSearch:
		SetCalibTargetSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eCalibWholeDmcodeBoard:
		break;
	}


	PostMessagePosSearchResult(vnPosIndex, eSearchMode);
}

void CVisionAlignGui::SetPosSearchResult_TX(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_ObjSearchResultCrossSize);
		m_vpTargetAlignGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_TarSearchResultCrossSize);
	}


	int nPosNum  = m_pVisionASM->GetPosNum();
	CString strShowInfo;

	switch(eSearchMode)
	{
	case eObjectSearch:
		SetObjectPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eInspectSearch:
		SetInspectPosSearchResult_TX(vnPosIndex, vnPattenIndex);
		break;
	case eCalibSearch:
		SetCalibPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eVirtualSearch:
		SetVirtualObjectPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eTargetSearch:
		SetTargetPosSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eCalibTargetSearch:
		SetCalibTargetSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eAutoCalibTargetSearch:
		SetCalibTargetSearchResult(vnPosIndex, vnPattenIndex);
		break;
	case eCalibWholeDmcodeBoard:
		break;
	}


	PostMessagePosSearchResult(vnPosIndex, eSearchMode);
}

void CVisionAlignGui::SetObjectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);
	
	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		bool bResult = true;
		int nSuccessPatIndex = 	vnPattenIndex[i];
		CString strTmpSearchResult;
		CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnObjectSearchTool(vnPosIndex[i], nSuccessPatIndex);
		
		std::vector<CSearchResult> vTempSeachResults = m_pVisionASM->GetAlnObjectSearchResults(vnPosIndex[i], nSuccessPatIndex);
		if (m_pVisionASM->m_mpObjectMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpObjectMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			if (vTempSeachResults.size() > 0)
			{
				vTempSeachResults[0].m_dPosX = cp.m_dPosX;
				vTempSeachResults[0].m_dPosY = cp.m_dPosY;
			}
			else
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = cp.m_dPosX;
				searchResult.m_dPosY = cp.m_dPosY;
				vTempSeachResults.push_back(searchResult);
			}

			strTmpSearchResult = _T("OK");
			bResult = true;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_OBJECT_SEARCH_RESULT_SHOW2);  //_T("对象定位结果: %s")

			//strSearchResult.Format(_T("对象定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			colorSearchResult = RGB(0, 255, 0);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1, cp.m_dPosX, cp.m_dPosY);

		}
		else
		{
			colorSearchResult = RGB(255, 0, 0);

			if (pSearchTool) 
			{
				strTmpSearchResult = pSearchTool->GetResultErrInfo();
			}
			else
			{
				if ((spi.m_bTargetObjectCamSeparate &&  vnPosIndex[i] >= spi.m_nPositionNum/2) || nSuccessPatIndex == -1)
				{
					strTmpSearchResult = _T("----");
					colorSearchResult = RGB(0, 255, 0);
				}
				else
				{
					//strTmpSearchResult = _T("定位工具为空");IDS_SEARCH_TOOL_IS_NULL
					strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
				}
			}

			bResult = false;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_OBJECT_SEARCH_RESULT_SHOW2);  //_T("对象定位结果: %s")
			strSearchResult.Format(strTmp, strTmpSearchResult);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_OBJECT_POS),vnPosIndex[i]+1);
			//vTempSeachResults.clear();
		}
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vTempSeachResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, colorSearchResult);
	}
	
}


void CVisionAlignGui::SetVirtualObjectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		bool bResult = true;
		int nSuccessPatIndex = 	vnPattenIndex[i];
		CString strTmpSearchResult;
		std::vector<CSearchResult> vSearchResults;

		CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnVirtualObjectSearchTool(vnPosIndex[i], nSuccessPatIndex);
		if (pSearchTool) 
		{
			strTmpSearchResult = pSearchTool->GetResultErrInfo();
		}
		else
		{
			//strTmpSearchResult = _T("定位工具为空");
			strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
		}
		CSearchResult vTempSeachResult = m_pVisionASM->GetAlnVirtualObjectSearchResult(vnPosIndex[i], nSuccessPatIndex);
		vSearchResults.push_back(vTempSeachResult);
		if (m_pVisionASM->m_mpObjectMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpObjectMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			if (vSearchResults.size() > 0)
			{
				vSearchResults[0].m_dPosX = cp.m_dPosX;
				vSearchResults[0].m_dPosY = cp.m_dPosY;
			}
			else
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = cp.m_dPosX;
				searchResult.m_dPosY = cp.m_dPosY;
				vSearchResults.push_back(searchResult);
			}

			bResult = true;
			strTmpSearchResult = _T("OK");

			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_VIRTUAL_OBJECT_SEARCH_RESULT_SHOW);  //_T("虚拟对象定位结果: %s")

			//strSearchResult.Format(_T("虚拟对象定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);

			colorSearchResult = RGB(0, 255, 0);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1, cp.m_dPosX, cp.m_dPosY);

		}
		else
		{
			bResult = false;
			colorSearchResult = RGB(255, 0, 0);
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_VIRTUAL_OBJECT_SEARCH_RESULT_SHOW);  //_T("虚拟对象定位结果: %s")
			//strSearchResult.Format(_T("虚拟对象定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_OBJECT_POS),vnPosIndex[i]+1);
			vSearchResults.clear();
		}
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vSearchResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, colorSearchResult);
	}
}

void CVisionAlignGui::SetTargetPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;
	CString strTmpSearchResult;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		int nSuccessPatIndex = vnPattenIndex[i];
		bool bResult = false;
		int eCoordPosLineStyle = scGuiGraphic::solidLine;

		CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnTargetSearchTool(vnPosIndex[i], nSuccessPatIndex);
		std::vector<CSearchResult> vTempSeachResults = m_pVisionASM->GetAlnTargetSearchResults(vnPosIndex[i], nSuccessPatIndex);
		if (m_pVisionASM->m_mpTargetMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpTargetMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			if (vTempSeachResults.size() > 0)
			{
				vTempSeachResults[0].m_dPosX = cp.m_dPosX;
				vTempSeachResults[0].m_dPosY = cp.m_dPosY;
			}
			else
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = cp.m_dPosX;
				searchResult.m_dPosY = cp.m_dPosY;
				vTempSeachResults.push_back(searchResult);
				
			}

			if (spi.m_bTargetObjectCamSeparate)
			{
				if (vnPosIndex[i] < spi.m_nPositionNum/2)
				{
					eCoordPosLineStyle = scGuiGraphic::dashLine;
				}
				else
				{
					eCoordPosLineStyle = scGuiGraphic::solidLine;
				}
			}


			strTmpSearchResult = _T("OK");
			bResult = true;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_TARGET_SEARCH_RESULT_SHOW2);  //_T("目标定位结果: %s")
			//strSearchResult.Format(_T("目标定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			colorSearchResult = RGB(0, 255, 0);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_TARGET_POS), vnPosIndex[i]+1, cp.m_dPosX, cp.m_dPosY);

		}
		else
		{

			colorSearchResult = RGB(255, 0, 0);	 

			if (pSearchTool) 
			{
				strTmpSearchResult = pSearchTool->GetResultErrInfo();
			}
			else
			{
				if ((spi.m_bTargetObjectCamSeparate &&  vnPosIndex[i] < spi.m_nPositionNum/2) || nSuccessPatIndex == -1)
				{
					strTmpSearchResult = _T("----");
					colorSearchResult = RGB(0, 255, 0);
				}
				else
				{
					//strTmpSearchResult = _T("定位工具为空");
					strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
				}
			}

			bResult = false;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_TARGET_SEARCH_RESULT_SHOW2);  //_T("目标定位结果: %s")
			//strSearchResult.Format(_T("目标定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_TARGET_POS),vnPosIndex[i]+1);
			vTempSeachResults.clear();
		}
		m_vpTargetAlignGui[vnPosIndex[i]]->SetGuiData(vTempSeachResults, bResult);
		m_vpTargetAlignGui[vnPosIndex[i]]->SetCoordPosLineStyle(0, eCoordPosLineStyle);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetSearchToolResult(strSearchResult, colorSearchResult);

	}
}


void CVisionAlignGui::SetCalibPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		std::vector<CSearchResult> vTempSeachResults;
		int nSuccessPatIndex = vnPattenIndex[i];
		CString strTmpSearchResult;
		bool bResult = false;


		CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoCalibObjectSearchTool(vnPosIndex[i], nSuccessPatIndex);
		CSearchResult cTmpSearchRes = m_pVisionASM->GetCalibTargetSearchResult(vnPosIndex[i], nSuccessPatIndex);

		if (pSearchTool) 
		{
			strTmpSearchResult = pSearchTool->GetResultErrInfo();
			vTempSeachResults.push_back(cTmpSearchRes);
		}
		else
		{
			//strTmpSearchResult = _T("定位工具为空");
			strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
		}

		if (m_pVisionASM->m_mpCalibMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpCalibMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			if (vTempSeachResults.size() > 0)
			{
				vTempSeachResults[0].m_dPosX = cp.m_dPosX;
				vTempSeachResults[0].m_dPosY = cp.m_dPosY;
			}
			else
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = cp.m_dPosX;
				searchResult.m_dPosY = cp.m_dPosY;
				vTempSeachResults.push_back(searchResult);
			}

			strTmpSearchResult = _T("OK");
			bResult = true;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_CALIB_SEARCH_RESULT_SHOW);  //_T("标定定位结果: %s")
			//strSearchResult.Format(_T("标定定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			colorSearchResult = RGB(0, 255, 0);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1, cp.m_dPosX, cp.m_dPosY);

		}
		else
		{
			bResult = false;
			CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_CALIB_SEARCH_RESULT_SHOW);  //_T("标定定位结果: %s")
			//strSearchResult.Format(_T("标定定位结果: %s"), strTmpSearchResult);
			strSearchResult.Format(strTmp, strTmpSearchResult);
			colorSearchResult = RGB(255, 0, 0);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_STATIC_SHOW_OBJECT_POS),vnPosIndex[i]+1);
			vTempSeachResults.clear();
		}
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vTempSeachResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, colorSearchResult);
	}
}


void CVisionAlignGui::SetInspectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		CString strTmpSearchResult;
		bool bResult = false;
		std::vector<CSearchResult> vSearchResults;
		BOOL bSearchResult = TRUE;
		for (int j = 0; j < vnPattenIndex.size(); j++)
		{
			CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnInspectSearchTool(vnPosIndex[i], vnPattenIndex[j]);
			if(pSearchTool)
			{
				for(int index = 0; index < pSearchTool->GetResultNum(); index++)
				{
					CSearchResult vTempSeachResult;
					pSearchTool->GetResult(index,vTempSeachResult);
					strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1,vTempSeachResult.m_dPosX, vTempSeachResult.m_dPosY);
					vSearchResults.push_back(vTempSeachResult);
					strTmpSearchResult += pSearchTool->GetResultErrInfo();
				}
			}
			else
			{
				//strTmpSearchResult = _T("定位工具为空");
				strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
			}
		}

		bResult = m_pVisionASM->GetAlnSearchResult(vnPosIndex[i], eInspectSearch);

		CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_INSPECT_SEARCH_RESULT_SHOW);  //_T("检测定位结果: %s")
		//strSearchResult.Format(_T("检测定位结果: %s"), strTmpSearchResult);
		strSearchResult.Format(strTmp, strTmpSearchResult);
		
		colorSearchResult = bResult ? RGB(0, 255, 0) : RGB(255, 0, 0);

		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vSearchResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, colorSearchResult);

	}
}

void CVisionAlignGui::SetInspectPosSearchResult_TX(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		CString strTmpSearchResult;
		bool bResult = false;
		std::vector<CSearchResult> vSearchResults;
		BOOL bSearchResult = TRUE;
		/*for (int j = 0; j < vnPattenIndex.size(); j++)
		{*/

		CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnInspectSearchTool(vnPosIndex[i], vnPattenIndex[i]);
		if(pSearchTool)
		{
			for(int index = 0; index < pSearchTool->GetResultNum(); index++)
			{
				CSearchResult vTempSeachResult;
				pSearchTool->GetResult(index,vTempSeachResult);
				strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1,vTempSeachResult.m_dPosX, vTempSeachResult.m_dPosY);
				vSearchResults.push_back(vTempSeachResult);
				strTmpSearchResult += pSearchTool->GetResultErrInfo();
			}
		}
		else
		{
			//strTmpSearchResult = _T("定位工具为空");
			strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
		}
		/*}*/

		bResult = m_pVisionASM->GetAlnSearchResult(vnPosIndex[i], eInspectSearch);

		CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_INSPECT_SEARCH_RESULT_SHOW);  //_T("检测定位结果: %s")
		//strSearchResult.Format(_T("检测定位结果: %s"), strTmpSearchResult);
		strSearchResult.Format(strTmp, strTmpSearchResult);

		colorSearchResult = bResult ? RGB(0, 255, 0) : RGB(255, 0, 0);

		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vSearchResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetObjectSearchToolResult(strSearchResult, colorSearchResult);

	}
}


void CVisionAlignGui::SetCalibTargetSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex)
{
	CString strShowInfo;
	CString strSearchResult;
	COLORREF colorSearchResult;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		CString strTmpSearchResult;
		bool bResult = false;
		std::vector<CSearchResult> vSearchResults;
		BOOL bSearchResult = TRUE;
		for (int j = 0; j < vnPattenIndex.size(); j++)
		{
			CBaseSearchTool* pSearchTool = m_pVisionASM->GetRepoAlnCalibTargetSearchTool(vnPosIndex[i], vnPattenIndex[j]);
			CSearchResult vTempSeachResult = m_pVisionASM->GetAlnCalibTargetSearchResult(vnPosIndex[i], vnPattenIndex[j]);
			strShowInfo.Format(GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_STR_AUTO_SHOW_OBJECT_POS), vnPosIndex[i]+1,vTempSeachResult.m_dPosX, vTempSeachResult.m_dPosY);

			if (pSearchTool)
			{
				vSearchResults.push_back(vTempSeachResult);
				strTmpSearchResult += pSearchTool->GetResultErrInfo();
			}
			else
			{
				//strTmpSearchResult = _T("定位工具为空");
				strTmpSearchResult = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_SEARCH_TOOL_IS_NULL);
			}
		}

		bResult = m_pVisionASM->GetAlnSearchResult(vnPosIndex[i], eCalibTargetSearch);
		
		CString strTmp = GetSysInfoString(m_pVisionASM->m_psaSysInfoStrings,IDS_CALIBTARGET_SEARCH_RESULT_SHOW);  //_T("关联定位结果: %s")
		//strSearchResult.Format(_T("关联定位结果: %s"), strTmpSearchResult);
		strSearchResult.Format(strTmp, strTmpSearchResult);

		colorSearchResult = bResult ? RGB(0, 255, 0) : RGB(255, 0, 0);

		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vSearchResults, bResult);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetMarkPos(strShowInfo);
		m_vpAlignPosTextGui[vnPosIndex[i]]->SetTargetSearchToolResult(strSearchResult, colorSearchResult);

	}
}

void CVisionAlignGui::PostMessagePosSearchResult(std::vector<int> vnPosIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = TRUE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = eSearchMode;
		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALIGNER_GUI_SHOW, wParam, lParam);
	}
}

void CVisionAlignGui::UpdatePosSearchResult(int nPosIndex, int eSearchMode)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
//	m_vpAlignPosTextGui[nPosIndex]->SetGuiTextPos();

	switch(eSearchMode)
	{
	case eObjectSearch:
		
	case eInspectSearch:
	case eCalibSearch:
	case eVirtualSearch:
		{
			if (nPosIndex < nPosNum)
			{
				m_vpObjectAlignGui[nPosIndex]->UpdateGuiShow();
				m_vpAlignPosTextGui[nPosIndex]->UpdateGuiShow();
			}
		}
		break;
	case eTargetSearch:
		{
			if (nPosIndex < nPosNum)
			{
				m_vpAlignPosTextGui[nPosIndex]->UpdateGuiShow();
			}
		}
	case eCalibTargetSearch:
	case eAutoCalibTargetSearch:
		{
			if (nPosIndex < nPosNum)
			{
				m_vpTargetAlignGui[nPosIndex]->UpdateGuiShow();
			}
		}
		break;
	case eCalibWholeDmcodeBoard:
		{
			// 二维码单独设置
		}
		break;
	}
}



// 设置目标与对象定位连线
void CVisionAlignGui::SetAllPosObjectToTargetResultLine()
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);
	if (spi.m_bTargetObjectCamSeparate)
	{
		return;
	}

	std::vector<int> vnPosIndex;
	for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		vnPosIndex.push_back(i);
	}

	SetPosObjectToTargetResultLine(vnPosIndex);

	
}

void CVisionAlignGui::SetAllPosText(std::vector<BOOL> bResult,std::vector<CString> vSearchResult)
{
	  std::vector<CSearchResult> vSearchResults;
	  std::vector<CSearchResult> vSearchResults1;
	  CSearchResult vTempSeachResult;
	  vSearchResults.push_back(vTempSeachResult);
	  vSearchResults1.push_back(vTempSeachResult);
	  for (int i = 0; i < bResult.size(); i++)
	   {
		   if (i%2==0)
		   {
			   vSearchResults[0].m_vText.push_back(vSearchResult[i]);
			   vSearchResults[0].m_vTextBool.push_back(bResult[i]);
		   } 
		   else
		   {
			   vSearchResults1[0].m_vText.push_back(vSearchResult[i]);
			   vSearchResults1[0].m_vTextBool.push_back(bResult[i]);
		   }	   	   
	   }

	   m_vpObjectAlignGui[0]->ClearAlnGuiTextShow();
	   m_vpObjectAlignGui[0]->SetGuiData(vSearchResults,true);
	   m_vpObjectAlignGui[1]->ClearAlnGuiTextShow();
	   m_vpObjectAlignGui[1]->SetGuiData(vSearchResults1,true);
}

void CVisionAlignGui::SetCurPosText(std::vector<int> vnPosIndex,std::vector<BOOL>  bResult,std::vector<CString> vSearchResult)
{
	std::vector<CSearchResult> vSearchResults;
	CSearchResult vTempSeachResult;
	vSearchResults.resize(vnPosIndex.size(),vTempSeachResult);
	for (int i = 0; i < bResult.size();)
	{
		for (int j = 0;j < vnPosIndex.size();j++)
		{
			vSearchResults[j].m_vText.push_back(vSearchResult[i]);
			vSearchResults[j].m_vTextBool.push_back(bResult[i]);
			++i;
		}   	   
	}

	std::vector<CSearchResult> vTemp;
	for (int i = 0; i < vnPosIndex.size();++i)
	{
		vTemp.clear();
		vTemp.push_back(vSearchResults[i]);
		m_vpObjectAlignGui[vnPosIndex[i]]->ClearAlnGuiTextShow();
		m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData(vTemp,true);
	}
}

void CVisionAlignGui::SetPosObjectToTargetResultLine(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;


	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		m_vpObjectToTargetAlignGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_ObjSearchResultCrossSize);
	}


	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		int nObjPattenIndex = m_pVisionASM->GetAlnSearchPatIndex(vnPosIndex[i], eObjectSearch); 
		int nTarPattenIndex = m_pVisionASM->GetAlnSearchPatIndex(vnPosIndex[i], eTargetSearch);
		std::vector<sc2Vector> vTmpTarget;
		std::vector<sc2Vector> vTmpObject;

		if (m_pVisionASM->m_mpTargetMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpTargetMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			vTmpTarget.push_back(sc2Vector(cp.m_dPosX, cp.m_dPosY));
		}

		if (m_pVisionASM->m_mpObjectMarkImagePos.GetIsMarkImagePosOK(vnPosIndex[i]))
		{
			CCoordPos cp = m_pVisionASM->m_mpObjectMarkImagePos.GetMarkImagePos(vnPosIndex[i]);
			vTmpObject.push_back(sc2Vector(cp.m_dPosX, cp.m_dPosY));
		}

		// 可以添加对象到目标距离的判定结果bResult控制显示颜色
		bool bResult = true;
		//

		m_vpObjectToTargetAlignGui[vnPosIndex[i]]->SetGuiData(vTmpObject, vTmpTarget, bResult);
		 //m_vstrAlnGuiText[i];
							  
		//m_vpObjectAlignGui[vnPosIndex[i]]->SetGuiData();
		//m_vstrAlnGuiText[vnPosIndex[i]] = _T("dddd")
	}
					
	PostMessagePosObjectToTargetResultLine(vnPosIndex);

}



void CVisionAlignGui::PostMessagePosObjectToTargetResultLine(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = TRUE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = 0;
		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALIGNER_COMBINE_GUI_SHOW, wParam, lParam);
	}
}


void CVisionAlignGui::UpdatePosObjectToTargetResultLine(int nPosIndex)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
	if (nPosIndex < nPosNum)
	{
		m_vpObjectToTargetAlignGui[nPosIndex]->UpdateGuiShow();
	}
}


void CVisionAlignGui::SetAllPosObjectToTargetResultLineInvisible()
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);

	std::vector<int> vnPosIndex;
	for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		vnPosIndex.push_back(i);
	}

	SetPosObjectToTargetResultLineInvisible(vnPosIndex);
}


void CVisionAlignGui::SetPosObjectToTargetResultLineInvisible(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;

	for (int i =0 ; i < vnPosIndex.size(); i++)
	{
		m_vpObjectToTargetAlignGui[vnPosIndex[i]]->SetGuiInvisible();
	}

	PostMessagePosObjectToTargetResultLineGuiInvisible(vnPosIndex);
}


void CVisionAlignGui::PostMessagePosObjectToTargetResultLineGuiInvisible(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = FALSE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = 0;

		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALIGNER_COMBINE_GUI_SHOW, wParam, lParam);
	}
}


void CVisionAlignGui::UpdatePosObjectToTargetLineGuiInvisible(int nPosIndex)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
	if (nPosIndex < nPosNum)
	{
		m_vpObjectToTargetAlignGui[nPosIndex]->UpdateGuiInvisible();
	}
}


void CVisionAlignGui::SetAllPosAlignOffsetByArea(sc2Vector vPoint)
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);

	int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
	if (!pProduct) return;


	std::vector<int> vnPosIndex;
	for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		vnPosIndex.push_back(i);
	}

	for (int i =0 ; i < vnPosIndex.size(); i++)
	{
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetAlignOffsetByArea(pProduct->m_pAlignerParam->GetAlignOffsetByArea());
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_AlignOffsetAreaGui);
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetGuiData(vPoint);
	}

	PostMessageAlignOffsetByAreaShow(vnPosIndex);
}
void CVisionAlignGui::SetAllPosAlignOffsetByArea(std::vector<sc2Vector>& vvPoint)						
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);

	int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
	if (!pProduct) return;

	std::vector<int> vnPosIndex;
	for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		vnPosIndex.push_back(i);
	}

	for (int i =0 ; i < vnPosIndex.size(); i++)
	{
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetAlignOffsetByArea(pProduct->m_pAlignerParam->GetAlignOffsetByArea());
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetGuiProperty(m_pVisionASM->m_pSystempOptionConfig->m_AlignOffsetAreaGui);
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetGuiData(vvPoint);
	}

	PostMessageAlignOffsetByAreaShow(vnPosIndex);
}
void CVisionAlignGui::PostMessageAlignOffsetByAreaShow(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = TRUE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = 0;

		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALINGER_OFFSET_AREA_GUI_SHOW, wParam, lParam);
	}
}


void CVisionAlignGui::UpdateAllPosAlignOffsetByAreaShow(int nPosIndex)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
	if (nPosIndex < nPosNum)
	{
		m_vpAlignOffsetAreaGui[nPosIndex]->UpdateGuiShow();
	}
}

void CVisionAlignGui::SetAllPosAlignOffsetByAreaInvisible()
{
	if (!m_pVisionASM)	return;

	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);

	std::vector<int> vnPosIndex;
	for (int i = 0; i < m_pVisionASM->GetPosNum(); i++)
	{
		vnPosIndex.push_back(i);
	}

	for (int i =0 ; i < vnPosIndex.size(); i++)
	{
		m_vpAlignOffsetAreaGui[vnPosIndex[i]]->SetGuiInvisible();
	}

	PostMessageAlignOffsetByAreaInvisible(vnPosIndex);
}


void CVisionAlignGui::PostMessageAlignOffsetByAreaInvisible(std::vector<int> vnPosIndex)
{
	if (!m_pVisionASM)	return;

	for (int i = 0; i < vnPosIndex.size(); i++)
	{
		WORD nPosIndex = vnPosIndex[i];
		WORD bVisible = FALSE;
		WPARAM wParam = MAKELONG(nPosIndex, bVisible);
		LPARAM lParam = 0;

		::PostMessage(m_pVisionASM->GetMainWnd()->GetSafeHwnd(), WM_UPDATE_ALINGER_OFFSET_AREA_GUI_SHOW, wParam, lParam);
	}
}


void CVisionAlignGui::UpdateAllPosAlignOffsetByAreaInvisible(int nPosIndex)
{
	if (!m_pVisionASM)	return;

	int nPosNum  = m_pVisionASM->GetPosNum();
	if (nPosIndex < nPosNum)
	{
		m_vpAlignOffsetAreaGui[nPosIndex]->UpdateGuiInvisible();
	}
}

int  CVisionAlignGui::GetAlignOffsetByArea(sc2Vector vPoint)
{
	if (m_vpAlignOffsetAreaGui.size() > 0 && m_vpAlignOffsetAreaGui[0])
	{
		return m_vpAlignOffsetAreaGui[0]->WithIn(vPoint);
	}

	return 0;
}



void CVisionAlignGui::SetConstTextShow(std::vector<int> vnPosIndex, const CString& strInfo, int eType)
{
	switch(eType)
	{
	case 1: // 对象定位坐标结果
		break;
	case 2: //  目标定位坐标结果 
		break;
	case 3:	//  对象定位工具状态结果
		break;
	case 4:	//  目标定位工具状态结果
		break;
	case 5:	//  对位结果移动量
		break;
	}
}

void CVisionAlignGui::Changelanguage()
{
	SysPlatformInfo spi;
	m_pVisionASM->GetSysPlatformInfo(spi);
	std::vector<int> vnPosIndex;
	for (int i = 0; i < spi.m_nPositionNum; i++)
	{
		vnPosIndex.push_back(i);
	}
	SetAllPosGuiInvisible(eTargetSearch);
	SetAllPosGuiInvisible(eObjectSearch);
	SetAllPosSearchResult(eTargetSearch);
	SetAllPosSearchResult(eObjectSearch);




	int nProductIndex = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	CProductData* pProduct = m_pVisionASM->GetTotalProuductData()->GetItemPtr(nProductIndex);
	if (pProduct && pProduct->m_pAlignerParam->GetAlignOffsetByArea().m_bEnable)
	{
		std::vector<sc2Vector> vvPoint;
		m_pVisionASM->m_cVisionAlignGui.SetAllPosAlignOffsetByArea(vvPoint);
	}
	else
	{
		m_pVisionASM->m_cVisionAlignGui.SetAllPosAlignOffsetByAreaInvisible();
	}

}


CVisionAlignMarkPosGui* CVisionAlignGui::GetVisionAlignConstGui(int nPosIndex)
{
	if (nPosIndex < 0 || nPosIndex >= m_vpAlignPosTextGui.size())
	{
		return NULL;
	}
	return m_vpAlignPosTextGui[nPosIndex];
}


CVisionAlignPosGui::CVisionAlignPosGui()
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}

CVisionAlignPosGui::CVisionAlignPosGui(const CVisionAlignPosGui& rhs)
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}


CVisionAlignPosGui::~CVisionAlignPosGui()
{
	FreeGui();
	m_pGuiDisplay = NULL;
}

void CVisionAlignPosGui::InitGui()
{
	scLine line;
	scLineSeg lineSeg;
	scCircle  circle;
	scObround obround;
	scRect  rect;
	scAffineRect  affineRect;
	scGenRect genRect;
	
	//m_pGuiShowImagePosText = new scGuiText;

	for (int i = 0 ;i < MAX_GUI_COUNT_PER_POS; i++)
	{
		// 显示定位点
		scGuiCoordCross* pGuiCoordCross = m_vcGuiCoordCross + i;
		pGuiCoordCross->SetInteractive(FALSE);
		pGuiCoordCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetYDirectionAdornment(scGuiCoordCross::eNone);
		pGuiCoordCross->SetRulerVisible(FALSE);
		pGuiCoordCross->SetLineColor(RGB(0,255,0));
		m_vbVisibleAlnGuiCoordCross[i] = false;
		m_vvlnGuiCoordCross[i] = sc2Vector(0, 0);
		m_vColorAlnGuiCoordCross[i] = RGB(0, 255, 0);
		m_vGuiCoordCrossLineStyle[i] = scGuiGraphic::solidLine;
		// 显示其他文本数据

		scGuiText* pGuiText = m_vcAlnGuiText + i;
		pGuiText->SetInteractive(FALSE);
		pGuiText->SetLineColor(RGB(0,255,0));
		pGuiText->SetVisible(FALSE);
		LOGFONT* lf = new LOGFONT;
		lf->lfHeight         = 15;
		lf->lfWidth          = 15;
		lf->lfEscapement     = 0;
		lf->lfOrientation    = 0;
		lf->lfWeight         = FW_THIN;
		lf->lfItalic         = 0;
		lf->lfUnderline      = 0;
		lf->lfStrikeOut      = 0;
		lf->lfCharSet        = DEFAULT_CHARSET;
		lf->lfOutPrecision   = 0;
		lf->lfClipPrecision  = 0;
		lf->lfQuality        = 0;
		lf->lfPitchAndFamily = 0;
		//lstrcpy(lf->lfFaceName, _T("new times roman"));
		lstrcpy(lf->lfFaceName, _T("隶书"));
		pGuiText->SetTextFont(lf);
		m_vbVisibleAlnGuiText[i] = false;
		m_vstrAlnGuiText[i] = _T("");
		m_vColorAlnGuiText[i] = (RGB(0, 255, 0));
		// 显示线

		scGuiLine* pGuiLine = m_vcGuiLine + i;
		pGuiLine->SetInteractive(FALSE);
		pGuiLine->SetLineColor(RGB(0,255,0));
		m_vbVisibleAlnGuiLine[i] = false;
		m_vcAlnGuiLine[i] = line;
		m_vColorAlnGuiLine[i] = (RGB(0, 255, 0));
		// 显示线段

		scGuiLineSeg* pGuiLineSeg = m_vcGuiLineSeg + i;
		pGuiLineSeg->SetInteractive(FALSE);
		pGuiLineSeg->SetLineColor(RGB(0,255,0));
		pGuiLineSeg->SetLineSegArrowVisible(TRUE);
		m_vbVisibleAlnGuiLineSeg[i] = false;
		m_vcAlnGuiLineSeg[i] = lineSeg;
		m_vColorAlnGuiLineSeg[i] = (RGB(0, 255, 0));
		// 显示圆
		scGuiCircle* pGuiCircle = m_vcGuiCircle + i;
		pGuiCircle->SetInteractive(FALSE);
		pGuiCircle->SetLineColor(RGB(0,255,0));
		pGuiCircle->SetCenterCrossVisible(FALSE);
		m_vbVisibleAlnGuiCircle[i] = false;
		m_vcAlnGuiCircle[i] = circle;
		m_vColorAlnGuiCircle[i] = (RGB(0, 255, 0));
		// 显示长圆
		scGuiObround* pGuiObround = m_vcGuiObround + i;
		pGuiObround->SetInteractive(FALSE);
		pGuiObround->SetLineColor(RGB(0,255,0));
		pGuiObround->SetCenterCrossVisible(FALSE);
		m_vbVisibleAlnGuiObround[i] = false;
		m_vcAlnGuiObround[i] = obround;
		m_vColorAlnGuiObround[i] = (RGB(0, 255, 0));
		// 显示搜索矩形框
		scGuiAffineRect* pGuiRect = m_vcSearchGuiRect + i;
		pGuiRect->SetInteractive(FALSE);
		pGuiRect->SetLineColor(RGB(0,255,0));
		pGuiRect->SetCenterCrossVisible(FALSE);
		m_vbVisibleAlnSearchGuiRect[i] = false;
		m_vcAlnSearchGuiRect[i] = affineRect;
		m_vColorAlnSearchGuiRect[i] = (RGB(0, 255, 0));

		// 显示搜索结果框
		scGuiAffineRect* pGuiAffineRect = m_vcGuiSearchResultAffineRect+i;
		pGuiAffineRect->SetInteractive(FALSE);
		pGuiAffineRect->SetLineColor(RGB(255,255,0));
		m_vbVisibleAlnSearchResultAffineRect[i] = false;
		m_vcAlnSearchResultAffineRect[i] = affineRect;
		m_vColorAlnSearchResultAffineRect[i] = (RGB(0, 255, 0));
		// 显示圆角矩形
		scGuiGenRect* pGuiGenRect = m_vcGuiGenRect + i;
		pGuiGenRect->SetInteractive(FALSE);
		pGuiGenRect->SetLineColor(RGB(0,255,0));
		pGuiGenRect->SetCenterCrossVisible(FALSE);
		m_vbVisibleAlnGuiGenRect[i] = false;
		m_vcAlnGenRect[i] = genRect;
		m_vColorAlnGuiGenRect[i] = RGB(0, 255, 0);
	}
}


void CVisionAlignPosGui::FreeGui()
{
	RemoveGuiFromGuiDisplay();
	for (int i = 0; i < MAX_GUI_COUNT_PER_POS; i++)
	{
		LOGFONT* pFont = m_vcAlnGuiText[i].GetTextFont();
		if (pFont)
		{
			delete pFont;
		}
	}
}

// 设置显示窗口
void CVisionAlignPosGui::SetGuiDisplay(scGuiDisplay* pGuiDisplay)
{
	// 先移除原先窗口
	RemoveGuiFromGuiDisplay();
	m_pGuiDisplay = pGuiDisplay;  
	AddGuiToGuiDisplay();
}


void CVisionAlignPosGui::SetGuiProperty(SearchResultCrossSize guiProperty)
{
	m_GuiProperty = guiProperty;
}


void CVisionAlignPosGui::AddGuiToGuiDisplay()
{
	CString strKey;
	CString strTmp;
	CBaseSearchTool::CreateGUID(strKey);

	if (m_pGuiDisplay)
	{
		//scGuiStaticContainer* pGuiSC = m_pGuiDisplay->GetConstContainer();
		//pGuiSC->AddItem(m_pGuiShowImagePosText, strKey+_T("_GuiPosText"));
		//m_pGuiShowImagePosText->SetVisible(TRUE);
		//m_pGuiDisplay->SetConstContainer(pGuiSC);


		scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
		for (int i = 0; i <MAX_GUI_COUNT_PER_POS; i++)
		{
			// 显示点
			strTmp.Format(_T("%s_CoordCross%d"), strKey, i);
			m_vcGuiCoordCross[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiCoordCross[i], strTmp);
			// 显示其他文本数据
			strTmp.Format(_T("%s_GuiText%d"), strKey, i);
			m_vcAlnGuiText[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcAlnGuiText[i], strTmp);
			// 显示线
			strTmp.Format(_T("%s_GuiLine%d"), strKey, i);
			m_vcGuiLine[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiLine[i], strTmp);
			// 显示线段
			strTmp.Format(_T("%s_GuiLineSeg%d"), strKey, i);
			m_vcGuiLineSeg[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiLineSeg[i], strTmp);
			// 显示圆
			strTmp.Format(_T("%s_GuiCircle%d"), strKey, i);
			m_vcGuiCircle[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiCircle[i], strTmp);
			// 显示长圆
			strTmp.Format(_T("%s_GuiObround%d"), strKey, i);
		    m_vcGuiObround[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiObround[i], strTmp);
			// 显示搜索矩形框
			strTmp.Format(_T("%s_GuiRect%d"), strKey, i);
			m_vcSearchGuiRect[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcSearchGuiRect[i], strTmp);
			// 显示搜索结果框
			strTmp.Format(_T("%s_GuiAffineRect%d"), strKey, i);
			m_vcGuiSearchResultAffineRect[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiSearchResultAffineRect[i], strTmp);
			// 显示圆角
			strTmp.Format(_T("%s_GuiGenRect%d"), strKey, i);
			m_vcGuiGenRect[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiGenRect[i], strTmp);
		}
		m_pGuiDisplay->SetStaticContainer(pGuiSC1);
	}
}

void CVisionAlignPosGui::RemoveGuiFromGuiDisplay()
{
	if (!m_pGuiDisplay)
	{
		return;
	}

	scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
	for (int i = 0; i <MAX_GUI_COUNT_PER_POS; i++)
	{
		// 显示点
		pGuiSC1->RemoveItem(m_vcGuiCoordCross[i].GetTipText());
		//显示其他文本数据
		pGuiSC1->RemoveItem(m_vcAlnGuiText[i].GetTipText());
		// 显示线
		pGuiSC1->RemoveItem(m_vcGuiLine[i].GetTipText());
		// 显示线段
		pGuiSC1->RemoveItem(m_vcGuiLineSeg[i].GetTipText());
		// 显示圆
		pGuiSC1->RemoveItem(m_vcGuiCircle[i].GetTipText());
		// 显示长圆
		pGuiSC1->RemoveItem(m_vcGuiObround[i].GetTipText());
		// 显示搜索矩形框
		pGuiSC1->RemoveItem(m_vcSearchGuiRect[i].GetTipText());
		// 显示搜索结果框
		pGuiSC1->RemoveItem(m_vcGuiSearchResultAffineRect[i].GetTipText());
	}
	m_pGuiDisplay = NULL;
}


// void CVisionAlignPosGui::SetShowAlnMarkTextGuiPos(sc2Vector vPos)
// {
// 
// }

void CVisionAlignPosGui::ClearAlnGuiTextShow()
{
	for (int j = 0; j < MAX_GUI_COUNT_PER_POS; j++)
	{
		m_vbVisibleAlnGuiText[j] = FALSE;
	}
}

void CVisionAlignPosGui::SetGuiData(std::vector<CSearchResult>& vSearchResult, bool bResult)
{

	int nGuiLineIndex = 0;
	int nGuiLineSegIndex = 0;
	int nGuiCircleIndex = 0;
	int nGuiCoordCrossIndex = 0;
	int nGuiAffineRectIndex = 0;
	int nGuiSearchAffineRectIndex = 0;
	int nGuiObrondIndex = 0;
	int nGuiGenRectIndex = 0;
	int nGuiTextIndex = 0;

	//// 显示其他文本数据
	//scGuiText m_vcAlnGuiText[MAX_GUI_COUNT_PER_POS];
	//bool m_vbVisibleAlnGuiText[MAX_GUI_COUNT_PER_POS];
	//CString m_vstrAlnGuiText[MAX_GUI_COUNT_PER_POS];
	//COLORREF m_vColorAlnGuiText[MAX_GUI_COUNT_PER_POS];
	//// 显示线
	//scGuiLine m_vcGuiLine[MAX_GUI_COUNT_PER_POS];
	//bool m_vbVisibleAlnGuiLine[MAX_GUI_COUNT_PER_POS];
	//scLine m_vcAlnGuiLine[MAX_GUI_COUNT_PER_POS];
	//COLORREF m_vColorAlnGuiLine[MAX_GUI_COUNT_PER_POS];

	for (int i = 0; i < vSearchResult.size(); i++)
	{

		// 定位点显示
		{
			if (nGuiCoordCrossIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiCoordCross[nGuiCoordCrossIndex] = true; 
			// 检测结果
			m_vColorAlnGuiCoordCross[nGuiCoordCrossIndex] = m_GuiProperty.nLineColor;
			// 设置位置
			m_vvlnGuiCoordCross[nGuiCoordCrossIndex].SetX(vSearchResult[i].m_dPosX);
			m_vvlnGuiCoordCross[nGuiCoordCrossIndex].SetY(vSearchResult[i].m_dPosY);

			nGuiCoordCrossIndex++;
		}

		// 定位矩形显示
		for (int j = 0; j < vSearchResult[i].m_vTrainSearchRect.size(); j++)
		{
			if (nGuiSearchAffineRectIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}
			
			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnSearchGuiRect[nGuiSearchAffineRectIndex] = m_GuiProperty.m_bShowRectGui; 
			// 检测结果
			m_vColorAlnSearchGuiRect[nGuiSearchAffineRectIndex] = m_GuiProperty.nTrainRectLineColor;
			// 设置位置
			m_vcAlnSearchGuiRect[nGuiSearchAffineRectIndex] = vSearchResult[i].m_vTrainSearchRect[j].m_searchRect;

			nGuiSearchAffineRectIndex++;
		}


		// 搜索结果矩形显示
		for (int j = 0; j < vSearchResult[i].m_vACFSearchRect.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiAffineRectIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnSearchResultAffineRect[nGuiAffineRectIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 检测结果
			m_vColorAlnSearchResultAffineRect[nGuiAffineRectIndex] = vSearchResult[i].m_vACFSearchRect[j].m_bFindRect == 1 ? m_GuiProperty.nOtherGuiLineColor : m_NGColor;
			// 设置位置
			m_vcAlnSearchResultAffineRect[nGuiAffineRectIndex] = vSearchResult[i].m_vACFSearchRect[j].m_searchRect;

			nGuiAffineRectIndex++;
		}

		// 圆显示
		for (int j = 0; j < vSearchResult[i].m_vCircleSearchResult.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiCircleIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiCircle[nGuiCircleIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiCircle[nGuiCircleIndex] = m_GuiProperty.nOtherGuiLineColor;
			// 设置位置
			m_vcAlnGuiCircle[nGuiCircleIndex].SetCenterRadius(vSearchResult[i].m_vCircleSearchResult[j].m_scCircleCircle, vSearchResult[i].m_vCircleSearchResult[j].m_dRadius);
			
			nGuiCircleIndex++;
		}


		// 线显示
		for (int j = 0; j < vSearchResult[i].m_vLineSearchResult.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiLineIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiLine[nGuiLineIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiLine[nGuiLineIndex] = vSearchResult[i].m_vLineSearchResult[j].m_bFindResult == 1 ? m_GuiProperty.nOtherGuiLineColor : m_NGColor;
			// 设置位置
			m_vcAlnGuiLine[nGuiLineIndex].SetFromStartEnd(sc2Vector(vSearchResult[i].m_vLineSearchResult[j].m_vLineX[0], vSearchResult[i].m_vLineSearchResult[j].m_vLineY[0]), 
				sc2Vector(vSearchResult[i].m_vLineSearchResult[j].m_vLineX[1], vSearchResult[i].m_vLineSearchResult[j].m_vLineY[1]));

			nGuiLineIndex++;
		}

		// 线段显示
		for (int j = 0; j < vSearchResult[i].m_vLineSegSearchResult.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiLineSegIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiLineSeg[nGuiLineSegIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiLineSeg[nGuiLineSegIndex] = m_GuiProperty.nOtherGuiLineColor;
			// 设置位置
			m_vcAlnGuiLineSeg[nGuiLineSegIndex].Set(sc2Vector(vSearchResult[i].m_vLineSegSearchResult[j].m_vLineX[0], vSearchResult[i].m_vLineSegSearchResult[j].m_vLineY[0]), 
				sc2Vector(vSearchResult[i].m_vLineSegSearchResult[j].m_vLineX[1], vSearchResult[i].m_vLineSegSearchResult[j].m_vLineY[1]));

			nGuiLineSegIndex++;
		}

		// 长圆显示
		for (int j = 0; j < vSearchResult[i].m_vObroundSearchResult.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiObrondIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiObround[nGuiObrondIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiObround[nGuiObrondIndex] = m_GuiProperty.nOtherGuiLineColor;
			// 设置位置
			m_vcAlnGuiObround[nGuiObrondIndex] = vSearchResult[i].m_vObroundSearchResult[j].m_scObround;

			nGuiObrondIndex++;
		}
	
		// 圆角矩形
		for (int j = 0; j < vSearchResult[i].m_vGenRectSearchResult.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiGenRectIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiGenRect[nGuiGenRectIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiGenRect[nGuiGenRectIndex] = m_GuiProperty.nOtherGuiLineColor;
			// 设置位置
			m_vcAlnGenRect[nGuiGenRectIndex] = vSearchResult[i].m_vGenRectSearchResult[j].m_scGenRect;

			nGuiGenRectIndex++;
		}


		// 显示文本	  测试 1
		for (int j = 0; j < vSearchResult[i].m_vTextBool.size(); j++)
		{
			// 没有GUI可以显示了
			if (nGuiTextIndex >= MAX_GUI_COUNT_PER_POS)
			{
				continue;
			}

			// 是否显示(如果有开关, 需要判定)
			m_vbVisibleAlnGuiText[nGuiTextIndex] = m_GuiProperty.m_bShowOtherGui; 
			// 结果
			m_vColorAlnGuiText[nGuiTextIndex] = vSearchResult[i].m_vTextBool[j] ?RGB(0,255,0):RGB(255,0,0);
			// 设置位置
			m_vstrAlnGuiText[nGuiTextIndex] = vSearchResult[i].m_vText[j];


			//// 是否显示(如果有开关, 需要判定)
			//m_vbVisibleAlnGuiText[nGuiGenRectIndex+1] = m_GuiProperty.m_bShowOtherGui; 
			//// 结果
			//m_vColorAlnGuiText[nGuiGenRectIndex+1] = m_GuiProperty.nOtherGuiLineColor;
			//// 设置位置
			//m_vstrAlnGuiText[nGuiGenRectIndex+1] = _T("X:33\nY:33");

			nGuiTextIndex++;
		}

		//// 显示其他文本数据
		//scGuiText m_vcAlnGuiText[MAX_GUI_COUNT_PER_POS];
		//bool m_vbVisibleAlnGuiText[MAX_GUI_COUNT_PER_POS];
		//CString m_vstrAlnGuiText[MAX_GUI_COUNT_PER_POS];
		//COLORREF m_vColorAlnGuiText[MAX_GUI_COUNT_PER_POS];


	}

	//SetGuiResultText(bResult);

}

void CVisionAlignPosGui::SetCoordPosLineStyle(int nPosIndex, int eStyle)
{
	if (nPosIndex < 0 || nPosIndex >= MAX_GUI_COUNT_PER_POS)
	{
		return;
	}

	m_vGuiCoordCrossLineStyle[nPosIndex] = (scGuiGraphic::StyleFlags)eStyle;
}


void CVisionAlignPosGui::SetGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_COUNT_PER_POS; i++)
	{
		m_vbVisibleAlnGuiCoordCross[i] = false;
		m_vbVisibleAlnGuiText[i] = false;
		m_vbVisibleAlnGuiLine[i] = false;
		m_vbVisibleAlnGuiLineSeg[i] = false;
		m_vbVisibleAlnGuiCircle[i] = false;
		m_vbVisibleAlnGuiObround[i] = false;
		m_vbVisibleAlnSearchGuiRect[i] = false;
		m_vbVisibleAlnSearchResultAffineRect[i] = false;
		m_vbVisibleAlnGuiGenRect[i] = false;
	}
}

void CVisionAlignPosGui::UpdateGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_COUNT_PER_POS; i++)
	{
		m_vcGuiCoordCross[i].SetVisible(m_vbVisibleAlnGuiCoordCross[i]);
		m_vcAlnGuiText[i].SetVisible(m_vbVisibleAlnGuiText[i]);
		m_vcGuiLine[i].SetVisible(m_vbVisibleAlnGuiLine[i]);
		m_vcGuiLineSeg[i].SetVisible(m_vbVisibleAlnGuiLineSeg[i]);
		m_vcGuiCircle[i].SetVisible(m_vbVisibleAlnGuiCircle[i] );
		m_vcGuiObround[i].SetVisible(m_vbVisibleAlnGuiObround[i]);
		m_vcSearchGuiRect[i].SetVisible(m_vbVisibleAlnSearchGuiRect[i]);
		m_vcGuiSearchResultAffineRect[i].SetVisible(m_vbVisibleAlnSearchResultAffineRect[i]);
		m_vcGuiGenRect[i].SetVisible(m_vbVisibleAlnGuiGenRect[i]);
	}

	if (m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
}


void CVisionAlignPosGui::SetGuiResultText(bool bOk)
{
	m_vcAlnGuiText[0].SetText(bOk? _T("OK") : _T("NG"));
	m_vColorAlnGuiText[0] = bOk ? RGB(0, 255, 0) : RGB(255, 0, 0);
	m_vbVisibleAlnGuiText[0] = true;
}

void CVisionAlignPosGui::UpdateGuiShow()
{

	for (int i = 0; i < MAX_GUI_COUNT_PER_POS; i++)
	{
		// 显示点
		if (!m_vbVisibleAlnGuiCoordCross[i])
		{
			m_vcGuiCoordCross[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiCoordCross[i].SetVisible(TRUE);
			m_vcGuiCoordCross[i].SetLineWidth(m_GuiProperty.nLineWidth);
			m_vcGuiCoordCross[i].SetCenterRotationLengths(m_vvlnGuiCoordCross[i], scRadian(scDegree(m_GuiProperty.nRotation)), m_GuiProperty.nLineLength, m_GuiProperty.nLineLength);
			m_vcGuiCoordCross[i].SetLineColor(m_vColorAlnGuiCoordCross[i]);
			m_vcGuiCoordCross[i].SetLineStyle(m_vGuiCoordCrossLineStyle[i]);
		}
		// 显示其他文本数据
		if (!m_vbVisibleAlnGuiText[i])
		{
			m_vcAlnGuiText[i].SetVisible(FALSE);
		}
		else
		{
			int nWidth = m_pGuiDisplay->GetImageWidth();
			int nHeight = m_pGuiDisplay->GetImageHeight();

			 m_vcAlnGuiText[i].SetVisible(TRUE);
			 m_vcAlnGuiText[i].SetLineWidth(m_GuiProperty.nLineWidth);
			 m_vcAlnGuiText[i].SetXYRotationText(sc2Vector(10, (nHeight*i)/20), scRadian(0), m_vstrAlnGuiText[i]);
			 m_vcAlnGuiText[i].SetLineColor(m_vColorAlnGuiText[i]);
		}
		// 显示线
		if (!m_vbVisibleAlnGuiLine[i])
		{
			m_vcGuiLine[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiLine[i].SetVisible(TRUE);
			m_vcGuiLine[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiLine[i].SetLine(m_vcAlnGuiLine[i]);
			m_vcGuiLine[i].SetLineColor(m_vColorAlnGuiLine[i]);
		}

		// 显示线段
		if (!m_vbVisibleAlnGuiLineSeg[i])
		{
			m_vcGuiLineSeg[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiLineSeg[i].SetVisible(TRUE);
			m_vcGuiLineSeg[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiLineSeg[i].SetLineSeg(m_vcAlnGuiLineSeg[i]);
			m_vcGuiLineSeg[i].SetLineColor(m_vColorAlnGuiLineSeg[i]);
		}
		// 显示圆
		if (!m_vbVisibleAlnGuiCircle[i])
		{
			m_vcGuiCircle[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiCircle[i].SetVisible(TRUE);
			m_vcGuiCircle[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiCircle[i].SetCircle(m_vcAlnGuiCircle[i]);
			m_vcGuiCircle[i].SetLineColor(m_vColorAlnGuiCircle[i]);
		}
		// 显示长圆
		if (!m_vbVisibleAlnGuiObround[i])
		{
			m_vcGuiObround[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiObround[i].SetVisible(TRUE);
			m_vcGuiObround[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiObround[i].SetObround(m_vcAlnGuiObround[i]);
			m_vcGuiObround[i].SetLineColor(m_vColorAlnGuiObround[i]);
		}
		// 显示搜索矩形框
		if (!m_vbVisibleAlnSearchGuiRect[i])
		{
			m_vcSearchGuiRect[i].SetVisible(FALSE);
		}
		else
		{
			m_vcSearchGuiRect[i].SetVisible(TRUE);
			m_vcSearchGuiRect[i].SetLineWidth(m_GuiProperty.nTrainRectLineWidth);
			m_vcSearchGuiRect[i].SetAffineRect(m_vcAlnSearchGuiRect[i]);
			m_vcSearchGuiRect[i].SetLineColor(m_vColorAlnSearchGuiRect[i]);
		}

		// 显示搜索结果框
		if (!m_vbVisibleAlnSearchResultAffineRect[i])
		{
			m_vcGuiSearchResultAffineRect[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiSearchResultAffineRect[i].SetVisible(TRUE);
			m_vcGuiSearchResultAffineRect[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiSearchResultAffineRect[i].SetAffineRect(m_vcAlnSearchResultAffineRect[i]);
			m_vcGuiSearchResultAffineRect[i].SetLineColor(m_vColorAlnSearchResultAffineRect[i]);
		}

		// 显示搜索圆角矩形
		if (!m_vbVisibleAlnGuiGenRect[i])
		{
			m_vcGuiGenRect[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiGenRect[i].SetVisible(TRUE);
			m_vcGuiGenRect[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiGenRect[i].SetGenRect(m_vcAlnGenRect[i]);
			m_vcGuiGenRect[i].SetLineColor(m_vColorAlnSearchResultAffineRect[i]);
		}

	}

	if(m_pGuiDisplay)
	{
		 m_pGuiDisplay->MyInvalidate();
	}
	
}





CVisionAlignMarkPosGui::CVisionAlignMarkPosGui()
{
	m_pGuiDisplay = NULL;

	m_bGuiUpdating = false;

	m_colorObjectSearchToolResult = RGB(0, 255, 0);
	m_colorTargetSearchToolResult = RGB(0, 255, 0);


	int width = GetSystemMetrics ( SM_CXSCREEN ); 
	int height= GetSystemMetrics ( SM_CYSCREEN ); 

	int nFontWidth = width / 185.0;
	int nFontHigh = width / 185.0 + 5 * (height/1080.0);

	LOGFONT* pFont = new LOGFONT;
	pFont->lfHeight         = nFontHigh;
	pFont->lfWidth          = nFontWidth;
	pFont->lfEscapement     = 0;
	pFont->lfOrientation    = 0;
	pFont->lfWeight         = FW_THIN;
	pFont->lfItalic         = 0;
	pFont->lfUnderline      = 0;
	pFont->lfStrikeOut      = 0;
	pFont->lfCharSet        = DEFAULT_CHARSET;
	pFont->lfOutPrecision   = 0;
	pFont->lfClipPrecision  = 0;
	pFont->lfQuality        = 0;
	pFont->lfPitchAndFamily = 0;
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	//lstrcpy(pFont->lfFaceName, _T("隶书"));
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	m_scObjectSearchToolResult.SetTextFont(pFont);

	pFont = new LOGFONT;
	pFont->lfHeight         = nFontHigh;
	pFont->lfWidth          = nFontWidth;
	pFont->lfEscapement     = 0;
	pFont->lfOrientation    = 0;
	pFont->lfWeight         = FW_THIN;
	pFont->lfItalic         = 0;
	pFont->lfUnderline      = 0;
	pFont->lfStrikeOut      = 0;
	pFont->lfCharSet        = DEFAULT_CHARSET;
	pFont->lfOutPrecision   = 0;
	pFont->lfClipPrecision  = 0;
	pFont->lfQuality        = 0;
	pFont->lfPitchAndFamily = 0;
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	//lstrcpy(pFont->lfFaceName, _T("隶书"));
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	m_scObjectGuiText.SetTextFont(pFont);

	pFont = new LOGFONT;
	pFont->lfHeight         = nFontHigh;
	pFont->lfWidth          = nFontWidth;
	pFont->lfEscapement     = 0;
	pFont->lfOrientation    = 0;
	pFont->lfWeight         = FW_THIN;
	pFont->lfItalic         = 0;
	pFont->lfUnderline      = 0;
	pFont->lfStrikeOut      = 0;
	pFont->lfCharSet        = DEFAULT_CHARSET;
	pFont->lfOutPrecision   = 0;
	pFont->lfClipPrecision  = 0;
	pFont->lfQuality        = 0;
	pFont->lfPitchAndFamily = 0;
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	//lstrcpy(pFont->lfFaceName, _T("隶书"));
	m_scTargetGuiText.SetTextFont(pFont);
// 
	pFont = new LOGFONT;
	pFont->lfHeight         = nFontHigh;
	pFont->lfWidth          = nFontWidth;
	pFont->lfEscapement     = 0;
	pFont->lfOrientation    = 0;
	pFont->lfWeight         = FW_THIN;
	pFont->lfItalic         = 0;
	pFont->lfUnderline      = 0;
	pFont->lfStrikeOut      = 0;
	pFont->lfCharSet        = DEFAULT_CHARSET;
	pFont->lfOutPrecision   = 0;
	pFont->lfClipPrecision  = 0;
	pFont->lfQuality        = 0;
	pFont->lfPitchAndFamily = 0;
	//lstrcpy(lf->lfFaceName, _T("new times roman"));
	//lstrcpy(pFont->lfFaceName, _T("隶书"));
	m_scTargetSearchToolResult.SetTextFont(pFont);

	strcpy(m_szObjectText,"对象位置  :(---,---)");
	strcpy(m_szTargetText,"目标位置  :(---,---)");
	strcpy(m_szObjectSearchToolResult,"对象定位结果: ----");
	strcpy(m_szTargetSearchToolResult,"目标定位结果: ----");
	

}

CVisionAlignMarkPosGui::CVisionAlignMarkPosGui(const CVisionAlignMarkPosGui& rhs)
{
	m_pGuiDisplay = NULL;
	m_bGuiUpdating = false;

}


CVisionAlignMarkPosGui::~CVisionAlignMarkPosGui()
{
	FreeGui();
	m_pGuiDisplay = NULL;
}


void CVisionAlignMarkPosGui::InitTargetGui(const CString& strShowInfo)
{
	m_strTargetText = strShowInfo;
	m_scTargetGuiText.SetLineColor(RGB(0,255,0));
	m_scTargetGuiText.SetVisible(TRUE);
	m_scTargetGuiText.SetText(strShowInfo);

	m_scTargetSearchToolResult.SetLineColor(RGB(0, 255, 0));
	m_scTargetSearchToolResult.SetVisible(TRUE);
	m_strTargetSearchToolResult = _T("目标定位结果:----");
	m_scTargetSearchToolResult.SetText(m_strTargetSearchToolResult);


	
}

void CVisionAlignMarkPosGui::InitObjectGui(const CString& strShowInfo)
{
	m_strObjectText = strShowInfo;
	m_scObjectGuiText.SetLineColor(RGB(0,255,0));
	m_scObjectGuiText.SetVisible(TRUE);
	m_scObjectGuiText.SetText(strShowInfo);

	m_scObjectSearchToolResult.SetLineColor(RGB(0, 255, 0));
	m_scObjectSearchToolResult.SetVisible(TRUE);
	m_strObjectSearchToolResult = _T("对象定位结果:----");
	m_scObjectSearchToolResult.SetText(m_strObjectSearchToolResult);

	return;

}



void CVisionAlignMarkPosGui::FreeGui()
{
	RemoveGuiFromGuiDisplay();
	LOGFONT* pFont = NULL;
	pFont = m_scObjectGuiText.GetTextFont();
	if (pFont)
	{
		delete pFont;
	}


	pFont = m_scObjectSearchToolResult.GetTextFont();
	if (pFont)
	{
		delete pFont;
	}

	pFont = m_scTargetGuiText.GetTextFont();
	if (pFont)
	{
		delete pFont;
	}

	pFont = m_scTargetSearchToolResult.GetTextFont();
	if (pFont)
	{
		delete pFont;
	}
}

// 设置显示窗口
void CVisionAlignMarkPosGui::SetGuiDisplay(scGuiDisplay* pGuiDisplay)
{
	// 先移除原先窗口
	RemoveGuiFromGuiDisplay();
	m_pGuiDisplay = pGuiDisplay;  
	AddGuiToGuiDisplay();
}


void CVisionAlignMarkPosGui::AddGuiToGuiDisplay()
{
	CString strKey;
	CString strTmp;
	CBaseSearchTool::CreateGUID(strKey);
	if (m_pGuiDisplay)
	{
		scGuiStaticContainer* pGuiSC = m_pGuiDisplay->GetConstContainer();

		strTmp =  strKey+_T("_GuiObjectPosText");
		pGuiSC->AddItem(&m_scObjectGuiText, strTmp);
		m_scObjectGuiText.SetTipText(strTmp);

		strTmp =  strKey+_T("_GuiTargetPosText");
		pGuiSC->AddItem(&m_scTargetGuiText, strTmp);
		m_scTargetGuiText.SetTipText(strTmp);

		strTmp =  strKey+_T("_GuiTargetSearchToolResult");
		pGuiSC->AddItem(&m_scTargetSearchToolResult, strTmp);
		m_scTargetSearchToolResult.SetTipText(strTmp);

		strTmp =  strKey+_T("_GuiObjectSearchToolResult");
		pGuiSC->AddItem(&m_scObjectSearchToolResult, strTmp);
		m_scObjectSearchToolResult.SetTipText(strTmp);


		m_pGuiDisplay->SetConstContainer(pGuiSC);
	}

	
}

void CVisionAlignMarkPosGui::RemoveGuiFromGuiDisplay()
{
	if (!m_pGuiDisplay)
	{
		return;
	}

	scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetConstContainer();

	pGuiSC1->RemoveItem(m_scObjectGuiText.GetTipText());
	pGuiSC1->RemoveItem(m_scTargetGuiText.GetTipText());


	pGuiSC1->RemoveItem(m_scTargetSearchToolResult.GetTipText());
	pGuiSC1->RemoveItem(m_scObjectSearchToolResult.GetTipText());



	
	m_pGuiDisplay = NULL;
}


void CVisionAlignMarkPosGui::SetGuiTextPos()
{
	if (m_pGuiDisplay)
	{
		RECT rtClient;
		m_pGuiDisplay->GetClientRect(&rtClient);

		LOGFONT* pFont = m_scTargetGuiText.GetTextFont();

		double nSegHeight = pFont->lfHeight * 1.5;

		/*m_scTargetGuiText.SetXYRotation(sc2Vector(10,20),scRadian(0));
		m_scObjectGuiText.SetXYRotation(sc2Vector(10,20 + nSegHeight),scRadian(0));*/

		m_scTargetGuiText.SetXYRotation(sc2Vector(10,rtClient.bottom- nSegHeight*2),scRadian(0));
		m_scObjectGuiText.SetXYRotation(sc2Vector(10,rtClient.bottom- nSegHeight*3),scRadian(0));
		m_scTargetSearchToolResult.SetXYRotation(sc2Vector(10, rtClient.bottom- nSegHeight*4), scRadian(0));
		m_scObjectSearchToolResult.SetXYRotation(sc2Vector(10, rtClient.bottom- nSegHeight*5), scRadian(0));
		//测试 1
	}
}


void CVisionAlignMarkPosGui::SetObjectMarkPos(const CString& strPos)
{
	//m_strObjectText = strPos;
	::strncpy_s(m_szObjectText,512, (LPCSTR)strPos, min(strPos.GetLength(), 511));
}

void CVisionAlignMarkPosGui::SetTargetMarkPos(const CString& strPos)
{
	//m_strTargetText = strPos;
	strncpy_s(m_szTargetText, 512, (LPCSTR)strPos, min(strPos.GetLength(), 511));

}

void CVisionAlignMarkPosGui::SetObjectSearchToolResult(const CString& strResult, COLORREF color)
{
	//m_strObjectSearchToolResult = strResult;
	strncpy_s(m_szObjectSearchToolResult, 512, (LPCSTR)strResult, min(strResult.GetLength(), 511));

	m_colorObjectSearchToolResult = color;
}

void CVisionAlignMarkPosGui::SetTargetSearchToolResult(const CString& strResult, COLORREF color)
{
//	m_strTargetSearchToolResult = strResult;
	strncpy_s(m_szTargetSearchToolResult, 512, (LPCSTR)strResult, min(strResult.GetLength(), 511));

	m_colorTargetSearchToolResult = color;
}

void CVisionAlignMarkPosGui::UpdateGuiShow()
{
	//UpdateAllGuiVisible(true);
// 	m_scObjectGuiText.SetText(m_strObjectText);
// 	m_scTargetGuiText.SetText(m_strTargetText);
// 
// 	m_scObjectSearchToolResult.SetLineColor(m_colorObjectSearchToolResult);
// 	m_scTargetSearchToolResult.SetLineColor(m_colorTargetSearchToolResult);
// 
// 	m_scTargetSearchToolResult.SetText(m_strTargetSearchToolResult);
// 	m_scObjectSearchToolResult.SetText(m_strObjectSearchToolResult);

	m_scObjectGuiText.SetText(m_szObjectText);
	m_scTargetGuiText.SetText(m_szTargetText);

	m_scObjectSearchToolResult.SetLineColor(m_colorObjectSearchToolResult);
	m_scTargetSearchToolResult.SetLineColor(m_colorTargetSearchToolResult);

	m_scTargetSearchToolResult.SetText(m_szTargetSearchToolResult);
	m_scObjectSearchToolResult.SetText(m_szObjectSearchToolResult);



	if (m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
	
}

void CVisionAlignMarkPosGui::UpdateAllGuiVisible(bool bVisible)
{
	m_scObjectGuiText.SetVisible(bVisible);
	m_scTargetGuiText.SetVisible(bVisible);
	m_scObjectSearchToolResult.SetVisible(bVisible);
	m_scTargetSearchToolResult.SetVisible(bVisible);

	m_bGuiUpdating = false;
}


/************************************************************************/
/* 每个位置组合显示GUI类-定制显示特殊GUI                                                                     */
/************************************************************************/


CVisionAlignPosCombineGui::CVisionAlignPosCombineGui()
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}


CVisionAlignPosCombineGui::CVisionAlignPosCombineGui(const CVisionAlignPosCombineGui& rhs)
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}


CVisionAlignPosCombineGui::~CVisionAlignPosCombineGui()
{
	FreeGui();
	m_pGuiDisplay = NULL;
}

void CVisionAlignPosCombineGui::InitGui()
{
	scLineSeg lineSeg;

	for (int i = 0 ;i < MAX_GUI_COMBINE_COUNT_PER_POS; i++)
	{
		// 显示线段
		scGuiLineSeg* pGuiLineSeg = m_vcGuiLineSeg + i;
		pGuiLineSeg->SetInteractive(FALSE);
		pGuiLineSeg->SetLineColor(RGB(0,255,0));
		pGuiLineSeg->SetLineSegArrowVisible(TRUE);
		m_vbVisibleAlnGuiLineSeg[i] = false;
		m_vcAlnGuiLineSeg[i] = lineSeg;
		m_vColorAlnGuiLineSeg[i] = (RGB(0, 255, 0));
	}
}
void CVisionAlignPosCombineGui::FreeGui()
{
	RemoveGuiFromGuiDisplay();
}

void CVisionAlignPosCombineGui::SetGuiDisplay(scGuiDisplay* pGuiDisplay)
{
	// 先移除原先窗口
	RemoveGuiFromGuiDisplay();
	m_pGuiDisplay = pGuiDisplay;  
	AddGuiToGuiDisplay();
}

void CVisionAlignPosCombineGui::SetGuiProperty(SearchResultCrossSize guiProperty)
{
	m_GuiProperty = guiProperty;
}

void CVisionAlignPosCombineGui::SetGuiData(std::vector<sc2Vector> vObj, std::vector<sc2Vector> vTar, bool bResult)
{
	int nGuiLineSegIndex = 0;

	if (vObj.size() != vTar.size() || vObj.size() == 0 || vTar.size() == 0)
	{
		return;
	}

	int nResultCnt = vObj.size();

	for (int i = 0; i < nResultCnt; i++)
	{
		// 没有GUI可以显示了
		if (nGuiLineSegIndex >= MAX_GUI_COMBINE_COUNT_PER_POS)
		{
			continue;
		}

		// 是否显示(如果有开关, 需要判定)
		m_vbVisibleAlnGuiLineSeg[nGuiLineSegIndex] = true;//m_GuiProperty.m_bShowOtherGui; 
		// 结果
		m_vColorAlnGuiLineSeg[nGuiLineSegIndex] = bResult ? m_GuiProperty.nOtherGuiLineColor : m_NGColor;
		// 设置位置
		m_vcAlnGuiLineSeg[nGuiLineSegIndex].Set(vObj[i], vTar[i]);

		nGuiLineSegIndex++;
	}
}

void CVisionAlignPosCombineGui::SetGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_COMBINE_COUNT_PER_POS; i++)
	{
		m_vbVisibleAlnGuiLineSeg[i] = false;
	}
}

void CVisionAlignPosCombineGui::UpdateGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_COMBINE_COUNT_PER_POS; i++)
	{
		m_vcGuiLineSeg[i].SetVisible(m_vbVisibleAlnGuiLineSeg[i]);
	}

	if (m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
}


void CVisionAlignPosCombineGui::UpdateGuiShow()
{
	for (int i = 0; i < MAX_GUI_COMBINE_COUNT_PER_POS; i++)
	{
		
		// 显示线段
		if (!m_vbVisibleAlnGuiLineSeg[i])
		{
			m_vcGuiLineSeg[i].SetVisible(FALSE);
		}
		else
		{
			m_vcGuiLineSeg[i].SetVisible(TRUE);
			m_vcGuiLineSeg[i].SetLineWidth(m_GuiProperty.nOtherUILineWidth);
			m_vcGuiLineSeg[i].SetLineSeg(m_vcAlnGuiLineSeg[i]);
			m_vcGuiLineSeg[i].SetLineColor(m_vColorAlnGuiLineSeg[i]);
		}
	}

	if(m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
}

void CVisionAlignPosCombineGui::AddGuiToGuiDisplay()
{
	CString strKey;
	CString strTmp;
	CBaseSearchTool::CreateGUID(strKey);

	if (m_pGuiDisplay)
	{
		scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
		for (int i = 0; i <MAX_GUI_COMBINE_COUNT_PER_POS; i++)
		{
			// 显示线段
			strTmp.Format(_T("%s_GuiLineSeg%d"), strKey, i);
			m_vcGuiLineSeg[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiLineSeg[i], strTmp);
		}
		m_pGuiDisplay->SetStaticContainer(pGuiSC1);
	}
}


void CVisionAlignPosCombineGui::RemoveGuiFromGuiDisplay()
{
	if (!m_pGuiDisplay)
	{
		return;
	}

	scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
	for (int i = 0; i <MAX_GUI_COMBINE_COUNT_PER_POS; i++)
	{
		// 显示线段
		pGuiSC1->RemoveItem(m_vcGuiLineSeg[i].GetTipText());
	}
	m_pGuiDisplay = NULL;
}

/************************************************************************/
/* 分区域显示GUI类                                                       */
/************************************************************************/

CVisionAlignPosOffsetAeraGui::CVisionAlignPosOffsetAeraGui()
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}


CVisionAlignPosOffsetAeraGui::CVisionAlignPosOffsetAeraGui(const CVisionAlignPosOffsetAeraGui& rhs)
{
	m_pGuiDisplay = NULL;
	m_NGColor = RGB(255, 0, 0);
	InitGui();
}


CVisionAlignPosOffsetAeraGui::~CVisionAlignPosOffsetAeraGui()
{
	FreeGui();
	m_pGuiDisplay = NULL;
}

void CVisionAlignPosOffsetAeraGui::InitGui()
{
	scRect rect;
	CString strTmp;
	int width = GetSystemMetrics ( SM_CXSCREEN ); 
	int height= GetSystemMetrics ( SM_CYSCREEN ); 

	int nFontWidth = width / 185.0;
	int nFontHigh = width / 185.0 + 5 * (height/1080.0);

	// 字体
	//LOGFONT* pFont = new LOGFONT;
	LOGFONT* pFont = &m_font;
	pFont->lfHeight         = nFontHigh;
	pFont->lfWidth          = nFontWidth;
	pFont->lfEscapement     = 0;
	pFont->lfOrientation    = 0;
	pFont->lfWeight         = FW_BOLD;
	pFont->lfItalic         = 0;
	pFont->lfUnderline      = 0;
	pFont->lfStrikeOut      = 0;
	pFont->lfCharSet        = DEFAULT_CHARSET;
	pFont->lfOutPrecision   = 0;
	pFont->lfClipPrecision  = 0;
	pFont->lfQuality        = 0;
	pFont->lfPitchAndFamily = 0;

	for (int i = 0 ;i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		// 显示线段
		scGuiRect* pGuiRect= m_vcGuiAreaRect + i;
		pGuiRect->SetInteractive(FALSE);
		pGuiRect->SetLineColor(RGB(0,255,0));
		m_vbVisibleAlnGuiAreaRect[i] = false;
		m_vcAlnGuiAreaRect[i] = rect;
		m_vColorAlnGuiAreaRect[i] = (RGB(0, 255, 0));
		m_vnGuiLineWidth[i] = 1;
		strTmp.Format(_T("区域%d"), i+1);
		pGuiRect->SetLabel(strTmp);
		pGuiRect->SetLabelVisible(TRUE);
		pGuiRect->SetLabelFont(pFont);
		pGuiRect->SetCenterCrossExtent(sc2Vector(0,0));
	}
}
void CVisionAlignPosOffsetAeraGui::FreeGui()
{
	RemoveGuiFromGuiDisplay();
}

void CVisionAlignPosOffsetAeraGui::SetGuiDisplay(scGuiDisplay* pGuiDisplay)
{
	// 先移除原先窗口
	RemoveGuiFromGuiDisplay();
	m_pGuiDisplay = pGuiDisplay;  
	AddGuiToGuiDisplay();
}

void CVisionAlignPosOffsetAeraGui::SetGuiProperty(AlignOffsetByAreaGui guiProperty)
{
	m_GuiProperty = guiProperty;
}

void CVisionAlignPosOffsetAeraGui::SetAlignOffsetByArea(AlignOffsetByAera alignOffsetByArea)
{
	m_AlignOffsetArea = alignOffsetByArea;

	double dAreaWidth = m_AlignOffsetArea.m_dSearchWidth / m_AlignOffsetArea.m_nAreaCol;
	double dAreaHeight = m_AlignOffsetArea.m_dSearchHeight / m_AlignOffsetArea.m_nAreaRow;

	for (int iRow = 0; iRow < m_AlignOffsetArea.m_nAreaRow; iRow++)
	{
		for (int iCol = 0; iCol < m_AlignOffsetArea.m_nAreaCol; iCol++)
		{
			int nGuiIndex = iRow * m_AlignOffsetArea.m_nAreaCol + iCol;
			double dStartX = iCol * dAreaWidth + m_AlignOffsetArea.m_dSearchStartX;
			double dStartY = iRow * dAreaHeight + m_AlignOffsetArea.m_dSearchStartY;

			m_vcAlnGuiAreaRect[nGuiIndex].Set(dStartX, dStartY, dAreaWidth, dAreaHeight);
		}
	}
}


int CVisionAlignPosOffsetAeraGui::WithIn(sc2Vector vPoint)
{
	for (int i = 0; i < m_AlignOffsetArea.m_nAreaRow * m_AlignOffsetArea.m_nAreaCol; i++)
	{
		if(m_vcAlnGuiAreaRect[i].Within(vPoint))
		{
			return i;
		}
	}

	return 0;
}

void CVisionAlignPosOffsetAeraGui::SetGuiData(sc2Vector vPoint)
{
	for (int i = 0; i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		if (i < m_AlignOffsetArea.m_nAreaRow * m_AlignOffsetArea.m_nAreaCol)
		{
			m_vcGuiAreaRect[i].SetVisible(TRUE);
		}
		else
		{
			m_vcGuiAreaRect[i].SetVisible(FALSE);
		}
		m_vColorAlnGuiAreaRect[i] = m_GuiProperty.m_clrNormal;
		m_vnGuiLineWidth[i] = m_GuiProperty.m_nLineWidth;
	}

	int nRect = WithIn(vPoint);
	m_vColorAlnGuiAreaRect[nRect] = m_GuiProperty.m_clrLight;
	m_vnGuiLineWidth[nRect] = m_GuiProperty.m_nLightLineWidth;
}

void CVisionAlignPosOffsetAeraGui::SetGuiData(std::vector<sc2Vector>& vvPoint)
{
	for (int i = 0; i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		if (i < m_AlignOffsetArea.m_nAreaRow * m_AlignOffsetArea.m_nAreaCol)
		{
			m_vbVisibleAlnGuiAreaRect[i] = true;
		}
		else
		{
			m_vbVisibleAlnGuiAreaRect[i] = false;
		}
		m_vColorAlnGuiAreaRect[i] = m_GuiProperty.m_clrNormal;
		m_vnGuiLineWidth[i] = m_GuiProperty.m_nLineWidth;
	}

	for (int i = 0; i < vvPoint.size(); i++)
	{
		int nRect = WithIn(vvPoint[i]);
		m_vColorAlnGuiAreaRect[nRect] = m_GuiProperty.m_clrLight;
		m_vnGuiLineWidth[nRect] = m_GuiProperty.m_nLightLineWidth;
	}

}

void CVisionAlignPosOffsetAeraGui::SetGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		m_vbVisibleAlnGuiAreaRect[i] = false;
	}
}

void CVisionAlignPosOffsetAeraGui::UpdateGuiInvisible()
{
	for (int i = 0 ;i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		m_vcGuiAreaRect[i].SetVisible(m_vbVisibleAlnGuiAreaRect[i]);
	}

	if (m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
}


void CVisionAlignPosOffsetAeraGui::UpdateGuiShow()
{
	for (int i = 0; i < MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		// 显示矩形
		m_vcGuiAreaRect[i].SetVisible(m_vbVisibleAlnGuiAreaRect[i]);
		m_vcGuiAreaRect[i].SetLineWidth(m_vnGuiLineWidth[i]);
		m_vcGuiAreaRect[i].SetRect(m_vcAlnGuiAreaRect[i]);
		m_vcGuiAreaRect[i].SetLineColor(m_vColorAlnGuiAreaRect[i]);
	}

	if(m_pGuiDisplay)
	{
		m_pGuiDisplay->MyInvalidate();
	}
}

void CVisionAlignPosOffsetAeraGui::AddGuiToGuiDisplay()
{
	CString strKey;
	CString strTmp;
	CBaseSearchTool::CreateGUID(strKey);

	if (m_pGuiDisplay)
	{
		scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
		for (int i = 0; i <MAX_GUI_AREA_COUNT_PER_POS; i++)
		{
			// 显示线段
			strTmp.Format(_T("%s_GuiAreaRect%d"), strKey, i);
			m_vcGuiAreaRect[i].SetTipText(strTmp);
			pGuiSC1->AddItem(&m_vcGuiAreaRect[i], strTmp);
		}
		m_pGuiDisplay->SetStaticContainer(pGuiSC1);
	}
}


void CVisionAlignPosOffsetAeraGui::RemoveGuiFromGuiDisplay()
{
	if (!m_pGuiDisplay)
	{
		return;
	}

	scGuiStaticContainer* pGuiSC1 = m_pGuiDisplay->GetStaticContainer();
	for (int i = 0; i <MAX_GUI_AREA_COUNT_PER_POS; i++)
	{
		// 显示线段
		pGuiSC1->RemoveItem(m_vcGuiAreaRect[i].GetTipText());
	}
	m_pGuiDisplay = NULL;
}



