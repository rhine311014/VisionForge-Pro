// FixedPosSearchTool.cpp: implementation of the CFixedPosSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FixedPosSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFixedPosSearchTool::CFixedPosSearchTool()
{
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	CreateGUID(m_strKeyText);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}

	m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiModelPoint.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiModelPoint.GetLineColor();
	m_tmpTrainGuiParam.m_FixedPosMode		= eAnyPoint;
	m_tmpTrainGuiParam.m_ModelPoint2		= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_FixedPosMode2		= eAnyPoint;

	m_tmpSearchGuiParam.m_nLineWidth		= m_GuiModelPoint.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor			= m_GuiModelPoint.GetLineColor();
			
	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;
}

CFixedPosSearchTool::CFixedPosSearchTool(CFixedPosSearchTool &cFixedPosSearchTool)
{
	if (cFixedPosSearchTool.GetSearchToolType() != eFixedPosSearchTool)
	{
		return;
	}
	
	// 清空
// 	ClearAllGui();
// 	ClearResult();
	
	// 初始化
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cFixedPosSearchTool.m_CamHeight;
	m_CamWidth								= cFixedPosSearchTool.m_CamWidth;

	m_TrainDataParam						= cFixedPosSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cFixedPosSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cFixedPosSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cFixedPosSearchTool.m_SearchGuiParam;
	m_TrainResult							= cFixedPosSearchTool.m_TrainResult;
	m_ImageTrain							= cFixedPosSearchTool.m_ImageTrain;
	m_ImageMask								= cFixedPosSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cFixedPosSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cFixedPosSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cFixedPosSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cFixedPosSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cFixedPosSearchTool.UpdateSearchGuiData(true);
	cFixedPosSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cFixedPosSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cFixedPosSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cFixedPosSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cFixedPosSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cFixedPosSearchTool.m_tmpImageTrain);
	SetMaskImage(cFixedPosSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;
		
		if (!m_ImageTrain.IsValid())
		{
			return;
		}
				
		m_bTrainOK = true;
	}		
}

CFixedPosSearchTool& CFixedPosSearchTool::operator=(CFixedPosSearchTool &cFixedPosSearchTool)
{
	if (this == &cFixedPosSearchTool)
	{
		return *this;
	}

	if (cFixedPosSearchTool.GetSearchToolType() != eFixedPosSearchTool)
	{
		return *this;
	}
	
	// 清空
	ClearAllGui();
	ClearResult();
	
	// 初始化
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cFixedPosSearchTool.m_CamHeight;
	m_CamWidth								= cFixedPosSearchTool.m_CamWidth;
	m_TrainDataParam						= cFixedPosSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cFixedPosSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cFixedPosSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cFixedPosSearchTool.m_SearchGuiParam;
	m_TrainResult							= cFixedPosSearchTool.m_TrainResult;
	m_ImageTrain							= cFixedPosSearchTool.m_ImageTrain;
	m_ImageMask								= cFixedPosSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cFixedPosSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cFixedPosSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cFixedPosSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cFixedPosSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cFixedPosSearchTool.UpdateSearchGuiData(true);
	cFixedPosSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cFixedPosSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cFixedPosSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cFixedPosSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cFixedPosSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cFixedPosSearchTool.m_tmpImageTrain);
	SetMaskImage(cFixedPosSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;
		
		if (!m_ImageTrain.IsValid())
		{
			return *this;
		}
		
		m_bTrainOK = true;
	}	
	
	return *this;
}

CFixedPosSearchTool::~CFixedPosSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CFixedPosSearchTool::GetSearchToolType()
{
	return eFixedPosSearchTool;
}										   

// 设置显示控件指针		
bool CFixedPosSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();
	
	m_pGuiDisplay = pGuiDisplay;
	m_hDispWnd = NULL;
	if (m_pGuiDisplay)
	{
		m_hDispWnd = m_pGuiDisplay->GetSafeHwnd();
	}
	m_bSearchResultGuiShow = FALSE;
	m_bTrainGuiShow        = FALSE;

	// 设置Gui控件
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	// 显示控件
	UpdateTrainGuiDisplay();
	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 获取显示控件指针										
scGuiDisplay* CFixedPosSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CFixedPosSearchTool::SetGuiKeyText(CString strGuiKeyText)									
{
	if ((strGuiKeyText == _T("")) || (strGuiKeyText == m_strKeyText))
	{
		return false;
	}
	
	ClearAllGui();
	
	m_strKeyText = strGuiKeyText;
	
	UpdateTrainGuiDisplay();
	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 获取Gui图形的关键字
bool CFixedPosSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CFixedPosSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CFixedPosSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CFixedPosSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示					
bool CFixedPosSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible && m_tmpTrainGuiParam.m_bEnableSecondModelPoint);
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();	
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));	
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));	

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新搜索Gui控件显示									
bool CFixedPosSearchTool::UpdateSearchResultGuiDisplay()							
{	
	int i=0;
	
	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新界面显示
bool CFixedPosSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CFixedPosSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CFixedPosSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CFixedPosSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	*((CFixedPosSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CFixedPosSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CFixedPosSearchTrainGuiParam*)pTrainGuiParam);

	if (m_tmpTrainGuiParam.m_FixedPosMode == eImageCenter)
	{
		if (m_tmpImageTrain.IsValid())
		{
			m_tmpTrainGuiParam.m_ModelPoint = sc2Vector(m_tmpImageTrain.Width()/2, m_tmpImageTrain.Height()/2);
		}
	}
	if(m_tmpTrainGuiParam.m_FixedPosMode2 == eImageCenter)
	{
		if(m_tmpImageTrain.IsValid())
		{
			m_tmpTrainGuiParam.m_ModelPoint2= sc2Vector(m_tmpImageTrain.Width()/2, m_tmpImageTrain.Height()/2);
		}
	}
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CFixedPosSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CFixedPosSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CFixedPosSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	SetTrainGuiParam(&m_tmpTrainGuiParam);

	return true;
}

// 获取训练原始图像									
bool CFixedPosSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CFixedPosSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CFixedPosSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CFixedPosSearchTool::Train()
{
	m_bTrainOK = false;

	if (!m_tmpImageTrain.IsValid())
	{
		m_nResultErrNumber = -99;
		return false;
	}

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	m_bTrainOK		 = true;

	m_tmpTrainResult.m_dModelX = m_tmpTrainGuiParam.m_ModelPoint.GetX();
	m_tmpTrainResult.m_dModelY = m_tmpTrainGuiParam.m_ModelPoint.GetY();

	m_tmpTrainResult.m_dModel2X = m_tmpTrainGuiParam.m_ModelPoint2.GetX();
	m_tmpTrainResult.m_dModel2Y = m_tmpTrainGuiParam.m_ModelPoint2.GetY();

	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam	 = m_tmpTrainGuiParam;
	m_ImageTrain     = m_tmpImageTrain;
	m_ImageMask      = m_tmpImageMask;
	m_TrainResult	 = m_tmpTrainResult;

	return true;
}

// 获取训练结果图像														
bool CFixedPosSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	ImageTrainResult = m_tmpImageTrain;

	return true;
}	

// 获取训练结果	
bool CFixedPosSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{	
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	*((CFixedPosSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}
	
// 是否训练成功
bool CFixedPosSearchTool::IsTrained()
{
	return m_bTrainOK;
}
		
// 设置搜索Data参数	
bool CFixedPosSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CFixedPosSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CFixedPosSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}

	*((CFixedPosSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CFixedPosSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CFixedPosSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CFixedPosSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eFixedPosSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CFixedPosSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CFixedPosSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	if (!ImageSearch.IsValid())
	{
		return false;
	}
	m_nResultErrNumber=0;
	// 清空
	ClearResult();
	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	cpImage tmpImage;
	BOOL bIsProcessImage = FALSE;
	if (1 != ImageSearch.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(ImageSearch.Width(),ImageSearch.Height(),epGray8);
		if (!GetProcessImage(&m_tmpTrainDataParam,ImageSearch,tmpImage))
		{
			m_nResultErrNumber = -1;
			return false;
		}
	}
	int nResultNum = 1;
	// 获取结果
	for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
	{		
		CSearchResult searchResult;
		if (m_TrainGuiParam.m_FixedPosMode == eImageCenter) // 图像中心
		{
			if(bIsProcessImage)
			{
				searchResult.m_dPosX = tmpImage.Width()/2;
				searchResult.m_dPosY = tmpImage.Height()/2;
			}
			else
			{
				searchResult.m_dPosX = ImageSearch.Width()/2;
				searchResult.m_dPosY = ImageSearch.Height()/2;
			}
		}
		else
		{
			searchResult.m_dPosX = m_TrainGuiParam.m_ModelPoint.GetX();
			searchResult.m_dPosY = m_TrainGuiParam.m_ModelPoint.GetY();
		}
		searchResult.m_dAngle = 0;
		searchResult.m_dScore = 1;
		
		
		if(m_tmpTrainGuiParam.m_bEnableSecondModelPoint)
		{
			if (m_TrainGuiParam.m_FixedPosMode2 == eImageCenter) // 图像中心
			{
				if(bIsProcessImage)
				{
					searchResult.m_vdAuxiliaryPosX[0] = tmpImage.Width()/2;
					searchResult.m_vdAuxiliaryPosY[0] = tmpImage.Height()/2;
				}
				else
				{
					searchResult.m_vdAuxiliaryPosX[0] = ImageSearch.Width()/2;
					searchResult.m_vdAuxiliaryPosY[0] = ImageSearch.Height()/2;
				}
			}
			else
			{
				searchResult.m_vdAuxiliaryPosX[0] = m_TrainGuiParam.m_ModelPoint2.GetX();
				searchResult.m_vdAuxiliaryPosY[0] = m_TrainGuiParam.m_ModelPoint2.GetY();
			}
			searchResult.m_vdAuxiliaryAngle[0] = 0;
		}
		m_SearchResultArray.push_back(searchResult);
				
		CString strTmpKey;
		strTmpKey.Format(_T("%s_ResultCross1_%d"), m_strKeyText, i);
		//scGuiCross *pTempCross = new scGuiCross;
		scGuiCross *pTempCross = m_GuiSearchCross+2*i;
 		pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), m_GuiModelPoint.GetExtent());
		pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
		pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		pTempCross->SetTipTextEnabled(true);
		CString st;
		st.Format(_T("参考点%d X:%.1f;Y:%.1f;D:%.3f;"), i+1, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
		pTempCross->SetLabel(st);
		pTempCross->SetLabelVisible(TRUE);
		pTempCross->SetTipText(strTmpKey);
		m_pGuiSearchCrossArray.push_back(pTempCross);

		if(m_tmpTrainGuiParam.m_bEnableSecondModelPoint)
		{
			strTmpKey.Format(_T("%s_s_ResultCross2_%d"),m_strKeyText,i);
			scGuiCross *pTempCross2 = m_GuiSearchCross + 2*i+1;
			pTempCross2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX[0], searchResult.m_vdAuxiliaryPosY[0]), m_GuiModelPoint2.GetExtent());
			pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross2->SetTipTextEnabled(true);
			st.Format(_T("参考点%d X:%.1f;Y:%.1f;D:%.3f;"), 2*i+2, searchResult.m_vdAuxiliaryPosX[0],searchResult.m_vdAuxiliaryPosY[0], searchResult.m_vdAuxiliaryAngle);
			pTempCross2->SetLabel(st);
			pTempCross2->SetTipText(strTmpKey);
			pTempCross2->SetLabelVisible(TRUE);
			m_pGuiSearchCrossArray.push_back(pTempCross2);
		}
		
	}

	m_SearchDataParam = m_tmpSearchDataParam;
	m_SearchGuiParam  = m_tmpSearchGuiParam;

	return true;
}

// 获取搜索结果个数										
int CFixedPosSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CFixedPosSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CFixedPosSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
{
	// strDir为文件夹路径
	// 仅配置ModelImage.bmp
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
		return false;
	}
	strDir += _T('\\');

	//////////////////////////读xml文件//////////////////////////
	CString strXmlPath = strDir + _T("Model.xml");
	vcXMLConfigurator cXMLConfigurator;
	sc2Vector vPos;
	scRect rRect;
	bool bRet;

	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (!bRet)
	{
		return false;
	}
	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
	if (cSearchToolType != eFixedPosSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	
	cXMLConfigurator.LeaveElem();
	cXMLConfigurator.LeaveElem();
	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpImageModel);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	ImagePattern = tmpImageModel;
	return true;
}

				
// 加载模板文件
bool CFixedPosSearchTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
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
		return false;
	}
	strDir += _T('\\');

	//////////////////////////读xml文件//////////////////////////
	CString strXmlPath = strDir + _T("Model.xml");
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	bool bRet;
	sc2Vector vPos;
//	BYTE byR, byG, byB;
	CFixedPosSearchTrainDataParam tmpTrainDataParam;
	CFixedPosSearchTrainGuiParam tmpTrainGuiParam;
	CFixedPosSearchSearchDataParam tmpSearchDataParam;
	CFixedPosSearchSearchGuiParam tmpSearchGuiParam;
	CFixedPosSearchTrainResult    tmpTrainResult;

	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (!bRet)
	{
		return false;
	}
	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
	if (cSearchToolType != eFixedPosSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	cXMLConfigurator.EnterElem();

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();

	// ImageChannel
	bRet = cXMLConfigurator.FindElem(_T("ImageChannel"));				
	if (!bRet)
	{
		//return false;
		tmpTrainDataParam.m_nImageChannel = 0;
	}
	else
	{
		tmpTrainDataParam.m_nImageChannel = _ttol(cXMLConfigurator.GetElemData());
	}
	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint.GetX());
	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_ModelPoint = vPos;
	cXMLConfigurator.LeaveElem();
	// FixedPos Mode
	// width 
	bRet = cXMLConfigurator.FindElem(_T("FixedPosMode"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_FixedPosMode = (CFixedPosMode)_ttoi(cXMLConfigurator.GetElemData());

	//ZhangRui
	{
		// 兼容老版本模板XML
		// EnableSecondModelPoint
		bRet = cXMLConfigurator.FindElem(_T("EnableSecondModelPoint"));
		if(!bRet)
		{
			tmpTrainGuiParam.m_bEnableSecondModelPoint = false;
		}
		else
		{
			tmpTrainGuiParam.m_bEnableSecondModelPoint = _ttoi(cXMLConfigurator.GetElemData());
		}

		// model point2
		bRet = cXMLConfigurator.FindElem(_T("ModelPoint2"));
		if (bRet)
		{
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("Center2X"));
			if (!bRet)
			{
				vPos.SetX(50);
			}
			else
			{
				vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}
		
			bRet = cXMLConfigurator.FindElem(_T("Center2Y"));
			if (!bRet)
			{
				vPos.SetY(50);
			}
			else
			{
				vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
			}
			tmpTrainGuiParam.m_ModelPoint2 = vPos;
			cXMLConfigurator.LeaveElem();
		}
		else
		{
			vPos.SetX(50);
			vPos.SetY(50);
			tmpTrainGuiParam.m_ModelPoint2=vPos;
		}
		// FixedPos Mode2
		bRet = cXMLConfigurator.FindElem(_T("FixedPosMode2"));
		if (!bRet)
		{
			tmpTrainGuiParam.m_FixedPosMode2 = eAnyPoint;
		}
		else
		{
			tmpTrainGuiParam.m_FixedPosMode2 = (CFixedPosMode)_ttoi(cXMLConfigurator.GetElemData());
		}
	}
	// color
// 	bRet = cXMLConfigurator.FindElem(_T("LineColor"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	bRet = cXMLConfigurator.FindElem(_T("R"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byR = _ttoi(cXMLConfigurator.GetElemData());
// 	bRet = cXMLConfigurator.FindElem(_T("G"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byG = _ttoi(cXMLConfigurator.GetElemData());
// 	bRet = cXMLConfigurator.FindElem(_T("B"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byB = _ttoi(cXMLConfigurator.GetElemData());
// 	tmpTrainGuiParam.m_LineColor = RGB(byR, byG, byB);
// 	cXMLConfigurator.LeaveElem();
// 	// width 
// 	bRet = cXMLConfigurator.FindElem(_T("LineWidth"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_nLineWidth = _ttoi(cXMLConfigurator.GetElemData());
// 	// model point visible 
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchGuiParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();


	// color
// 	bRet = cXMLConfigurator.FindElem(_T("LineColor"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	bRet = cXMLConfigurator.FindElem(_T("R"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byR = _ttoi(cXMLConfigurator.GetElemData());
// 	bRet = cXMLConfigurator.FindElem(_T("G"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byG = _ttoi(cXMLConfigurator.GetElemData());
// 	bRet = cXMLConfigurator.FindElem(_T("B"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	byB = _ttoi(cXMLConfigurator.GetElemData());
// 	tmpSearchGuiParam.m_LineColor = RGB(byR, byG, byB);
// 	cXMLConfigurator.LeaveElem();
// 	// width 
// 	bRet = cXMLConfigurator.FindElem(_T("LineWidth"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_nLineWidth = _ttoi(cXMLConfigurator.GetElemData());
// 	// model point visible 
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainResult"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("ModelX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("ModelY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dModelX = vPos.GetX();
	tmpTrainResult.m_dModelY = vPos.GetY();
	//ZhanRui
	{
		bRet = cXMLConfigurator.FindElem(_T("Model2X"));
		if (!bRet)
		{
			vPos.SetX(50);
		}
		else
		{
			vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}
		bRet = cXMLConfigurator.FindElem(_T("Model2Y"));
		if (!bRet)
		{
			vPos.SetY(50);
		}
		else
		{
			vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		}
		tmpTrainResult.m_dModel2X = vPos.GetX();
		tmpTrainResult.m_dModel2Y = vPos.GetY();
	}
	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	cpImage tmpImageMask;
	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpImageModel);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	/////////////CameraParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("CamParam"));
	if (!bRet)
	{
		m_CamHeight = tmpImageModel.Height();
		m_CamWidth = tmpImageModel.Width();
	}
	else
	{
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
			if (!bRet)
			{
				m_CamHeight = tmpImageModel.Height();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamHeight);
			}
			bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
			if (!bRet)
			{
				m_CamWidth = tmpImageModel.Width();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamWidth);
			}		
		}
		cXMLConfigurator.LeaveElem();
	}

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	m_tmpTrainDataParam = tmpTrainDataParam;
	m_tmpTrainGuiParam  = tmpTrainGuiParam;
	m_tmpSearchDataParam= tmpSearchDataParam;
	m_tmpSearchGuiParam = tmpSearchGuiParam;
	m_TrainDataParam	= tmpTrainDataParam;
	m_TrainGuiParam		= tmpTrainGuiParam;
	m_SearchDataParam	= tmpSearchDataParam;
	m_SearchGuiParam	= tmpSearchGuiParam;
	m_TrainResult       = tmpTrainResult;
	m_ImageTrain		= tmpImageModel;
	m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}
	
// 保存模板文件							
bool CFixedPosSearchTool::SaveModelToFile(CString strDir)
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
	strInfo.Format(_T("%d"), GetSearchToolType());
	cXMLConfigurator.AddElem(_T("ToolType"), strInfo);

	////////////CaneraParam//////////
	cXMLConfigurator.AddElem(_T("CamParam"));
	cXMLConfigurator.EnterElem();
	{
		cXMLConfigurator.AddElem(_T("CamHeight"), m_CamHeight);
		cXMLConfigurator.AddElem(_T("CamWidth"), m_CamWidth);
	}
	cXMLConfigurator.LeaveElem();



	/////////////TrainDataParam//////////////
	cXMLConfigurator.AddElem(_T("TrainParam"));
	cXMLConfigurator.EnterElem();

//  //将TrainGuiParam的参数，放在TrainDataParam括号中
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);

	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// FixedPosMode
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_FixedPosMode);
	cXMLConfigurator.AddElem(_T("FixedPosMode"), strInfo);

	{
		//EnableSecondModelPoint
		strInfo.Format(_T("%d"),m_TrainGuiParam.m_bEnableSecondModelPoint);
		cXMLConfigurator.AddElem(_T("EnableSecondModelPoint"),strInfo);
		// model point2
		cXMLConfigurator.AddElem(_T("ModelPoint2"));
		cXMLConfigurator.EnterElem();
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
		cXMLConfigurator.AddElem(_T("Center2X"), strInfo);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
		cXMLConfigurator.AddElem(_T("Center2Y"), strInfo);
		cXMLConfigurator.LeaveElem();
		// FixedPosMode2
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_FixedPosMode2);
		cXMLConfigurator.AddElem(_T("FixedPosMode2"), strInfo);
	}
	// color
// 	cXMLConfigurator.AddElem(_T("LineColor"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%d"), GetRValue(m_TrainGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("R"), strInfo);
// 	strInfo.Format(_T("%d"), GetGValue(m_TrainGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("G"), strInfo);
// 	strInfo.Format(_T("%d"), GetBValue(m_TrainGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("B"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	// width 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nLineWidth);
// 	cXMLConfigurator.AddElem(_T("LineWidth"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();


	// color
// 	cXMLConfigurator.AddElem(_T("LineColor"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%d"), GetRValue(m_SearchGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("R"), strInfo);
// 	strInfo.Format(_T("%d"), GetGValue(m_SearchGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("G"), strInfo);
// 	strInfo.Format(_T("%d"), GetBValue(m_SearchGuiParam.m_LineColor));
// 	cXMLConfigurator.AddElem(_T("B"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	// width 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_nLineWidth);
// 	cXMLConfigurator.AddElem(_T("LineWidth"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dModel2X);
	cXMLConfigurator.AddElem(_T("Model2X"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModel2Y);
	cXMLConfigurator.AddElem(_T("Model2Y"), strInfo);
	cXMLConfigurator.LeaveElem();
	
	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	//////////////////////////存储模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	hStatus = cDIB.Init(m_ImageTrain);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.Write(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	
	return true;
}	
	
// 更新训练控件数据信息	
bool CFixedPosSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint2 = m_GuiModelPoint2.GetCenter();
	}
	else // 将数据传递到Gui控件上
	{
		if (m_tmpTrainGuiParam.m_FixedPosMode == eImageCenter) // 图像中心
		{
			m_GuiModelPoint.SetInteractive(FALSE);
		}
		else
		{
			m_GuiModelPoint.SetInteractive(TRUE);
		}
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

		if (m_tmpTrainGuiParam.m_FixedPosMode2 == eImageCenter) // 图像中心
		{
			m_GuiModelPoint2.SetInteractive(FALSE);
		}
		else
		{
			m_GuiModelPoint2.SetInteractive(TRUE);
		}
		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变
	}

	return true;
}

bool CFixedPosSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiSearchCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CFixedPosSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{	
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}	
																	
// 清除定位工具在Display上的控件
bool CFixedPosSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CFixedPosSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	return true;
}

bool CFixedPosSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;


	double centerx,centery, xlen,ylen;

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CFixedPosSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();	
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));	
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));	
	}
	
	return true;
}

bool CFixedPosSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
				
		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
		}
	}
	
	return true;	
}

// 获取训练模板原始图像
bool CFixedPosSearchTool::GetPatternImage(cpImage &ImagePattern)
{	
	ImagePattern = m_ImageTrain;
	return true;
}	

// 恢复训练参数
bool CFixedPosSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CFixedPosSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CFixedPosSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
	
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
}
CString	CFixedPosSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -99:							//训练未成功
				strErrInfo += _T("The Train is NG");
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	case 1:
		{
			switch (m_nResultErrNumber)
			{
			case -99:							//训练未成功
				strErrInfo += _T("训练未成功");
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	default:
		{
			switch (m_nResultErrNumber)
			{
			case -99:							//训练未成功
				strErrInfo += _T("训练未成功");
				break;
			default:
				strErrInfo += _T("");
				break;
			}
		}
		break;
	}
	return strErrInfo;
}
