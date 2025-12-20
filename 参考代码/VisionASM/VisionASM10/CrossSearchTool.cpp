// CrossSearchTool.cpp: implementation of the CCrossSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrossSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 

#include "svIntersect.h"
#include "svFind.h"
#include "svGuiLine.h"
#include "svGuiFindLine.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossSearchTool::CCrossSearchTool()
{
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	m_tmpTrainDataParam.m_dGrau				= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow		= -10.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 10.0;
	
	m_tmpSearchGuiParam.m_nLineWidth		= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor			= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;
}

CCrossSearchTool::CCrossSearchTool(CCrossSearchTool &cCrossSearchTool)
{
	if (cCrossSearchTool.GetSearchToolType() != eCrossSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cCrossSearchTool.m_CamHeight;
	m_CamWidth								= cCrossSearchTool.m_CamWidth;

	m_TrainDataParam						= cCrossSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCrossSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCrossSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCrossSearchTool.m_SearchGuiParam;
	m_TrainResult                           = cCrossSearchTool.m_TrainResult;
	m_ImageTrain							= cCrossSearchTool.m_ImageTrain;
	m_ImageMask								= cCrossSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCrossSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cCrossSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCrossSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCrossSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCrossSearchTool.UpdateSearchGuiData(true);
	cCrossSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCrossSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCrossSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCrossSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCrossSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCrossSearchTool.m_tmpImageTrain);
	SetMaskImage(cCrossSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;
				
		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return;
		}
		
		if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return;
		}
		
		if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
		{
			return;
		}
		
		// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
		BOOL bIsProcessImage = FALSE;
		if (1 != m_ImageTrain.PixelSize())
		{
			bIsProcessImage = TRUE;
		}

		cpImage tmpImage;
		cpImage tmpImageMask;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
		}		

		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
	}		
}

CCrossSearchTool& CCrossSearchTool::operator=(CCrossSearchTool& cCrossSearchTool)
{
	if (this == &cCrossSearchTool)
	{
		return *this;
	}

	if (cCrossSearchTool.GetSearchToolType() != eCrossSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识
	
	// 赋值
	m_CamHeight								= cCrossSearchTool.m_CamHeight;
	m_CamWidth								= cCrossSearchTool.m_CamWidth;

	m_TrainDataParam						= cCrossSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCrossSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCrossSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCrossSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCrossSearchTool.m_TrainResult;
	m_ImageTrain							= cCrossSearchTool.m_ImageTrain;
	m_ImageMask								= cCrossSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCrossSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cCrossSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCrossSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCrossSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCrossSearchTool.UpdateSearchGuiData(true);
	cCrossSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCrossSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCrossSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCrossSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCrossSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCrossSearchTool.m_tmpImageTrain);
	SetMaskImage(cCrossSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;
		
		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return *this;
		}
		
		if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return *this;
		}
		
		if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
		{
			return *this;
		}
		
		// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
		BOOL bIsProcessImage = FALSE;
		if (1 != m_ImageTrain.PixelSize())
		{
			bIsProcessImage = TRUE;
		}

		cpImage tmpImage;
		cpImage tmpImageMask;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
		}		

		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
	}

	return *this;
}

CCrossSearchTool::~CCrossSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CCrossSearchTool::GetSearchToolType()
{
	return eCrossSearchTool;
}										   

// 设置显示控件指针		
bool CCrossSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

// 设置训练结果显示控件指针
bool CCrossSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay = pGuiDisplay;
	m_hTrainResultDispWnd = NULL;
	if(m_pGuiTrainResultDisplay)
	{
		m_hTrainResultDispWnd = m_pGuiTrainResultDisplay->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}


// 获取显示控件指针										
scGuiDisplay* CCrossSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CCrossSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CCrossSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CCrossSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CCrossSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CCrossSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CCrossSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CCrossSearchTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
	
		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	scFeatureletChainSet tmpscFLCS;
	bool bflag=m_QuickSearchTool.GetTrainFeatures(tmpscFLCS);
	if(bflag)
	{
		m_GuiTrainFeaturelet.SetFeaturelet(tmpscFLCS);
		m_GuiTrainFeaturelet.SetVisible(m_bTrainOK);

		if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
		{
			m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet,m_strKeyText+_T("_FeatureChain"));

			m_pGuiTrainResultDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
			m_pGuiTrainResultDisplay->MyInvalidate();
		}
	}
	return true;
}

// 更新搜索界面显示									
bool CCrossSearchTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;
	
	for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
	{
		m_pGuiSearchAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}
	
	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		int i=0;
		
		for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchAffineRectArray[i], m_pGuiSearchAffineRectArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
		}
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

bool CCrossSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CCrossSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CCrossSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CCrossSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}
	
	*((CCrossSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CCrossSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CCrossSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CCrossSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CCrossSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CCrossSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CCrossSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CCrossSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CCrossSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CCrossSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置训练参数
	if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	BOOL bIsProcessImage = FALSE;
	if (1 != m_tmpImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	cpImage tmpImage;
	cpImage tmpImageMask;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_tmpImageMask.Width(),m_tmpImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask,tmpImageMask);
	}		


	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		bool bRet = false;
		if (bIsProcessImage)
		{
			bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());	
		}
		else
		{
			bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());	
		}

		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			
			m_ImageTrain     = m_tmpImageTrain;
			m_ImageMask      = m_tmpImageMask;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}
		return bRet;
	}
	else
	{
		bool bRet = false;
		if (bIsProcessImage)
		{
			bRet = m_QuickSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());
		}
		else
		{
			bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());
		}

		
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}

		return bRet;
	}

	return false;
}

// 获取训练结果图像														
bool CCrossSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CCrossSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}
	
	*((CCrossSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}						

// 是否训练成功
bool CCrossSearchTool::IsTrained()
{
	return m_bTrainOK;
}															
			
// 设置搜索Data参数	
bool CCrossSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CCrossSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CCrossSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}

	*((CCrossSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CCrossSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CCrossSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CCrossSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eCrossSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CCrossSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CCrossSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       // 训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 设置搜索个数
	if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

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


	// 执行搜索
	//bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	bool bRet = false;
	if (bIsProcessImage)
	{
		bRet = m_QuickSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
	}
	else
	{
		bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	}

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_QuickSearchTool.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -1;
			return false;
		}

		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
			
			scFindLine m_FindLineTool[CROSS_LINE_NUM];
			int nCaliperNum = 15;

			// 准备工作
			double dStartX[CROSS_LINE_NUM];
			double dStartY[CROSS_LINE_NUM];

			double dEndX[CROSS_LINE_NUM];
			double dEndY[CROSS_LINE_NUM];

			scAffineRect ar = m_TrainGuiParam.m_TrainAffineRect;

			// 水平线
			m_FindLineTool[0].SetCaliperNum(nCaliperNum);
			m_FindLineTool[0].SetCaliperParams(nCaliperNum, scDegree(90), ar.GetYLength(), ar.GetXLength()/nCaliperNum);
			m_FindLineTool[0].SetCaliperProperty(5, eDarkToLight, eLightToDark, 1);
			m_FindLineTool[0].SetOutLineNum(nCaliperNum/3, eRansacRobust);

			dStartX[0] = (ar.GetCornerPo().GetX() + ar.GetCornerPy().GetX())/2;
			dStartY[0] = (ar.GetCornerPo().GetY() + ar.GetCornerPy().GetY())/2;

			dEndX[0] = (ar.GetCornerPx().GetX() + ar.GetCornerPopp().GetX())/2;
			dEndY[0] = (ar.GetCornerPx().GetY() + ar.GetCornerPopp().GetY())/2;


			// 垂直线
			m_FindLineTool[1].SetCaliperNum(nCaliperNum);
			m_FindLineTool[1].SetCaliperParams(nCaliperNum, scDegree(90), ar.GetXLength(), ar.GetYLength()/nCaliperNum);
			m_FindLineTool[1].SetCaliperProperty(5, eDarkToLight, eLightToDark, 1);
			m_FindLineTool[1].SetOutLineNum(nCaliperNum/3, eRansacRobust);

			dStartX[1] = (ar.GetCornerPo().GetX() + ar.GetCornerPx().GetX())/2;
			dStartY[1] = (ar.GetCornerPo().GetY() + ar.GetCornerPx().GetY())/2;

			dEndX[1] = (ar.GetCornerPy().GetX() + ar.GetCornerPopp().GetX())/2;
			dEndY[1] = (ar.GetCornerPy().GetY() + ar.GetCornerPopp().GetY())/2;
			

			// 求变换矩阵
			sc2Vector pos = tmpQuickResult.GetPosition();
			sc2Vector tmModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_QuickSearchTool.GetModelOrigin(tmModelPoint);
			dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

			scLineSeg ResLine[CROSS_LINE_NUM];
			double dRMS[CROSS_LINE_NUM];
			bool   bFindLine[CROSS_LINE_NUM];
			for (int k=0; k<CROSS_LINE_NUM; k++)
			{
				bFindLine[k] = 0;
			}

			for (int j=0; j<CROSS_LINE_NUM; j++)
			{
				// 求找线工具变换后的位置,找线
				double dTransStartX, dTransStartY;
				double dTransEndX, dTransEndY;
				dTransStartX = cos(dRotation) * dStartX[j]  - sin(dRotation) * dStartY[j] + dTransX;
				dTransStartY = sin(dRotation) * dStartX[j]  + cos(dRotation) * dStartY[j] + dTransY;
				dTransEndX = cos(dRotation) * dEndX[j]  - sin(dRotation) * dEndY[j] + dTransX;
				dTransEndY = sin(dRotation) * dEndX[j]  + cos(dRotation) * dEndY[j] + dTransY;
				m_FindLineTool[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);

				// 执行找线
				if(bIsProcessImage)
				{
					bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
				}
				else
				{
					bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
				}
			}
			if (!bFindLine[0] || !bFindLine[1])
			{
				m_nResultErrNumber=-2;       //十字失败
				return false;
			}

			// 求交点
			if (bFindLine[0] && bFindLine[1])
			{
				sc2Vector vLineCrossPoint;
				bool bIntersect;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
					&& crossPointArray.size() > 0)
				{
					CSearchResult searchResult;
					searchResult.m_dPosX = crossPointArray[0].GetX();
					searchResult.m_dPosY = crossPointArray[0].GetY();
					searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
					searchResult.m_dScore = tmpQuickResult.GetScore();

					// 设置训练矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);
				    //line
					CResultLine line;
					line.m_vLineX[0] = ResLine[0].GetP1().GetX();
					line.m_vLineY[0] = ResLine[0].GetP1().GetY();
					line.m_vLineX[1] = ResLine[0].GetP2().GetX();
					line.m_vLineY[1] = ResLine[0].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);
					line.m_vLineX[0] = ResLine[1].GetP1().GetX();
					line.m_vLineY[0] = ResLine[1].GetP1().GetY();
					line.m_vLineX[1] = ResLine[1].GetP2().GetX();
					line.m_vLineY[1] = ResLine[1].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);


					m_SearchResultArray.push_back(searchResult);

					CString	strTmpKey;
					strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
					//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
					scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
					pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
					pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect->SetTipTextEnabled(true);
					pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);
					m_pGuiSearchCrossArray.push_back(pTempCross);
				}
			}	
		}	

		if (m_SearchResultArray.size() <= 0)
		{
			return false;
		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	return bRet;
}

// 获取搜索结果个数										
int CCrossSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CCrossSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
// 				
// // 加载模板文件
// bool CCrossSearchTool::LoadModelFromFile(CString strDir)
// {
// 	// strDir为文件夹路径
// 	// 配置文件包括以下部分:
// 	//    (1) 配置文件Pattern.xml
// 	//    (2) 模板图像ModelImage.bmp
// 	//    (3) QuickSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
// 	//    (4) 掩模图像MaskImage.bmp(视定位工具是否采用掩模训练而定)
// 	if (strDir == _T(""))
// 	{
// 		return false;
// 	}
// 	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
// 	{
// 		strDir.Delete(strDir.GetLength()-1);
// 	}
// 	DWORD dwRet = GetFileAttributes(strDir);
// 	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 	{
// 		return false;
// 	}
// 	strDir += _T('\\');
// 	
// 	//////////////////////////读xml文件//////////////////////////
// 	CString strXmlPath = strDir + _T("Pattern.xml");
// 	vcXMLConfigurator cXMLConfigurator;
// 	CString strInfo;
// 	bool bRet;
// 	sc2Vector vPos;
// 	BYTE byR, byG, byB;
// 	CCrossSearchTrainDataParam tmpTrainDataParam;
// 	CCrossSearchTrainGuiParam tmpTrainGuiParam;
// 	CCrossSearchSearchDataParam tmpSearchDataParam;
// 	CCrossSearchSearchGuiParam tmpSearchGuiParam;
// 	CCrossSearchTrainResult    tmpTrainResult;
// 
// 	if (!cXMLConfigurator.Load(strXmlPath))
// 	{
// 		return false;
// 	}
// 
// 	bRet = cXMLConfigurator.FindElem(_T("Configuration"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 
// 	/////////////工具类型/////////////
// 	cXMLConfigurator.ResetCurElemPos();
// 	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
// 	if (cSearchToolType != eCrossSearchTool)
// 	{
// 		return false;
// 	}
// 
// 	/////////////TrainDataParam//////////////
// 	cXMLConfigurator.ResetCurElemPos();
// 	bRet = cXMLConfigurator.FindElem(_T("TrainDataParam"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	// grau
// 	bRet = cXMLConfigurator.FindElem(_T("Grau"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 	// noise thre
// 	bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
// 	// total mag
// 	bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
// 
// 	// mask enable
// 	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	// train affine rect
// 	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}	
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}	
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainGuiParam.m_TrainAffineRect.SetCenter(vPos);
// 	bRet = cXMLConfigurator.FindElem(_T("XLength"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_TrainAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("YLength"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_TrainAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_TrainAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
// 	bRet = cXMLConfigurator.FindElem(_T("Skew"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_TrainAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
// 	cXMLConfigurator.LeaveElem();
// 	// model point
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPoint"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint.GetX());
// 	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainGuiParam.m_ModelPoint = vPos;
// 	cXMLConfigurator.LeaveElem();
// 	// search rect
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRect"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect.GetOrigin().GetX());
// 	bRet = cXMLConfigurator.FindElem(_T("OriginX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("OriginY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainGuiParam.m_SearchRect.SetOrigion(vPos);
// 	bRet = cXMLConfigurator.FindElem(_T("SizeX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("SizeY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainGuiParam.m_SearchRect.SetSize(vPos);
// 	cXMLConfigurator.LeaveElem();
// 	// color
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
// 	strInfo.Format(_T("%d"), tmpTrainGuiParam.m_nLineWidth);
// 	bRet = cXMLConfigurator.FindElem(_T("LineWidth"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_nLineWidth = _ttoi(cXMLConfigurator.GetElemData());
// 	// affine rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRectVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_bTrainAffineRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// model point visible
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// search rect visible  
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bSearchRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	}
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchDataParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("SearchDataParam"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	bRet = cXMLConfigurator.FindElem(_T("SearchNum"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchDataParam.m_nSearchNum = _ttoi(cXMLConfigurator.GetElemData());
// 	bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchDataParam.m_dAcceptThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchGuiParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();
// 	// color
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
// 	// affine rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bAffineRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// model point visible 
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	cXMLConfigurator.LeaveElem();
// 
// 	/////////////TrainResult//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("TrainResult"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	bRet = cXMLConfigurator.FindElem(_T("ModelX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("ModelY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainResult.m_dModelX = vPos.GetX();
// 	tmpTrainResult.m_dModelY = vPos.GetY();
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////////////////读取模板图像//////////////////////////
// 	scDIB cDIB;
// 	HStatus hStatus;
// 	scImageGray tmpIImageModel;
// 	scImageGray tmpImageMask;
// 	hStatus = cDIB.Init(strDir + _T("ModelImage.bmp"));
// 	if (!IsSuccess(hStatus))
// 	{
// 		return false;
// 	}
// 	hStatus = cDIB.GetImage(tmpIImageModel);
// 	if (!IsSuccess(hStatus))
// 	{
// 		return false;
// 	}
// 	
// 	//////////////////////////读取掩模图像//////////////////////////
// 	if (tmpTrainDataParam.m_bMaskEnable)
// 	{
// 		hStatus = cDIB.Init(strDir + _T("MaskImage.bmp"));
// 		if (!IsSuccess(hStatus))
// 		{
// 			tmpTrainDataParam.m_bMaskEnable = false;
// 		}
// 		else
// 		{
// 			hStatus = cDIB.GetImage(tmpImageMask);
// 			if (!IsSuccess(hStatus))
// 			{
// 				tmpTrainDataParam.m_bMaskEnable = false;
// 			}
// 		}
// 	}
// 	
// 	//////////////////////////读取ptq文件//////////////////////////
// 	scQuickSearchTool tmpQuickSearchTool;
// 	bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Pattern.ptq"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	
// 	// 数据赋值
// 	m_tmpImageTrain = tmpIImageModel;
// 	if (tmpTrainDataParam.m_bMaskEnable)
// 	{
// 		m_tmpImageMask = tmpImageMask;
// 	}
// 	m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Pattern.ptq"));
// 	m_tmpTrainDataParam = tmpTrainDataParam;
// 	m_tmpTrainGuiParam  = tmpTrainGuiParam;
// 	m_tmpSearchDataParam= tmpSearchDataParam;
// 	m_tmpSearchGuiParam = tmpSearchGuiParam;
// 	m_TrainDataParam	= tmpTrainDataParam;
// 	m_TrainGuiParam		= tmpTrainGuiParam;
// 	m_SearchDataParam	= tmpSearchDataParam;
// 	m_SearchGuiParam	= tmpSearchGuiParam;
// 	m_TrainResult       = tmpTrainResult;
// 	m_ImageTrain		= tmpIImageModel;
// 	m_ImageMask			= tmpImageMask;
// 
// 	m_bTrainOK = true;
// 	
// 	////////////////////////显示更新///////////////////////////////
// 	UpdateTrainGuiData(false);
// 	UpdateSearchGuiData(false);
// 	
// 	return true;
// }
// 	
// // 保存模板文件							
// bool CCrossSearchTool::SaveModelToFile(CString strDir)
// {
// 	// strDir为文件夹路径，文件夹如果不存在会自动创建
// 	// 配置文件包括以下部分:
// 	//    (1) 配置文件Pattern.xml
// 	//    (2) 模板图像ModelImage.bmp
// 	//    (3) QuickSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
// 	//    (4) 掩模图像MaskImage.bmp(视定位工具是否采用掩模训练而定)
// 	
// 	if (strDir == _T(""))
// 	{
// 		return false;
// 	}
// 	if (strDir.GetAt(strDir.GetLength()-1) == _T('\\'))
// 	{
// 		strDir.Delete(strDir.GetLength()-1);
// 	}
// 	DWORD dwRet = GetFileAttributes(strDir);
// 	if ((dwRet == 0xFFFFFFFF) || ((dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0)) // 路径不存在或不是路径
// 	{
// 		if (!CreateDirectory(strDir, NULL))
// 		{	
// 			return false;
// 		}
// 	}
// 	strDir += _T('\\');
// 	 
//  
// 	//////////////////////////写xml文件//////////////////////////
// 	vcXMLConfigurator cXMLConfigurator;
// 	CString strInfo;
// 	cXMLConfigurator.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n")); 
// 	cXMLConfigurator.AddElem(_T("Configuration"));
// 	cXMLConfigurator.EnterElem();
// 	/////////////工具类型/////////////
// 	strInfo.Format(_T("%d"), GetSearchToolType());
// 	cXMLConfigurator.AddElem(_T("ToolType"), strInfo);
// 
// 	/////////////TrainDataParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainDataParam"));
// 	cXMLConfigurator.EnterElem();
// 	// grau
// 	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau);
// 	cXMLConfigurator.AddElem(_T("Grau"), strInfo);
// 	// noise thre
// 	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre);
// 	cXMLConfigurator.AddElem(_T("NoiseThre"), strInfo);
// 	// total mag
// 	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag);
// 	cXMLConfigurator.AddElem(_T("TotalMag"), strInfo);
// 	// mask enable
// 	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
// 	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();
// 	// train affine rect
// 	cXMLConfigurator.AddElem(_T("TrainAffineRect"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetCenter().GetX());
// 	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetCenter().GetY());
// 	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetXLength());
// 	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetYLength());
// 	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetXRotation().ToDouble());
// 	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetSkew().ToDouble());
// 	cXMLConfigurator.AddElem(_T("Skew"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	// model point
// 	cXMLConfigurator.AddElem(_T("ModelPoint"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetX());
// 	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetY());
// 	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	// search rect
// 	cXMLConfigurator.AddElem(_T("SearchRect"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetOrigin().GetX());
// 	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetOrigin().GetY());
// 	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetSize().GetX());
// 	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetSize().GetY());
// 	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	// color
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
// 	// model point visible  
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// search rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchDataParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchDataParam"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
// 	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
// 	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
// 	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
// 	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
// 	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
// 	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
// 	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 
// 	/////////////SearchGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();
// 	// color
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 	
// 	/////////////TrainResult//////////////
// 	cXMLConfigurator.AddElem(_T("TrainResult"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
// 	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
// 	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);
// 	cXMLConfigurator.LeaveElem();
// 
// 	cXMLConfigurator.LeaveElem();
// 	bool bRet = cXMLConfigurator.Save(strDir+_T("Pattern.xml"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 
// 	//////////////////////////存储模板图像//////////////////////////
// 	scDIB cDIB;
// 	HStatus hStatus;
// 	hStatus = cDIB.Init(m_ImageTrain);
// 	if (!IsSuccess(hStatus))
// 	{
// 		return false;
// 	}
// 	hStatus = cDIB.Write(strDir + _T("ModelImage.bmp"));
// 	if (!IsSuccess(hStatus))
// 	{
// 		return false;
// 	}
// 	
// 	//////////////////////////存储掩模图像//////////////////////////
// 	if (m_TrainDataParam.m_bMaskEnable && m_ImageMask.IsValid())
// 	{
// 		hStatus = cDIB.Init(m_ImageMask);
// 		if (!IsSuccess(hStatus))
// 		{
// 			return false;
// 		}
// 		hStatus = cDIB.Write(strDir + _T("MaskImage.bmp"));
// 		if (!IsSuccess(hStatus))
// 		{
// 			return false;
// 		}
// 	}
// 
// 	//////////////////////////存储模板ptq文件//////////////////////////
// 	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Pattern.ptq"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 
// 	return true;
// }	

bool CCrossSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eCrossSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	// Train rect
	// train affine rect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	rRect.SetWidth(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	rRect.SetHeight(_tcstod(cXMLConfigurator.GetElemData(), NULL));
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
	rRect.SetOrigion(sc2Vector(vPos.GetX()-rRect.GetSize().GetX()/2,vPos.GetY()-rRect.GetSize().GetY()/2));
	cXMLConfigurator.LeaveElem();
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
	if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}


// 加载模板文件
bool CCrossSearchTool::LoadModelFromFile(CString strDir)
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
	CCrossSearchTrainDataParam tmpTrainDataParam;
	CCrossSearchTrainGuiParam tmpTrainGuiParam;
	CCrossSearchSearchDataParam tmpSearchDataParam;
	CCrossSearchSearchGuiParam tmpSearchGuiParam;
	CCrossSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eCrossSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	// grau
	bRet = cXMLConfigurator.FindElem(_T("Grau"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// noise thre
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
	// total mag
	bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());

	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());

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


	// train affine rect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	tmpTrainGuiParam.m_TrainAffineRect.SetCenter(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
	bRet = cXMLConfigurator.FindElem(_T("Skew"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
	cXMLConfigurator.LeaveElem();
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
	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect.GetOrigin().GetX());
	bRet = cXMLConfigurator.FindElem(_T("OriginX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("OriginY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_SearchRect.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("SizeX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("SizeY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_SearchRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();


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
// 	strInfo.Format(_T("%d"), tmpTrainGuiParam.m_nLineWidth);
// 	bRet = cXMLConfigurator.FindElem(_T("LineWidth"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_nLineWidth = _ttoi(cXMLConfigurator.GetElemData());
// 	// affine rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRectVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_bTrainAffineRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// model point visible
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// search rect visible  
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bSearchRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	}
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("SearchNum"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nSearchNum = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAcceptThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);

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
// 	// affine rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bAffineRectVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpIImageModel;
	cpImage tmpImageMask;
	hStatus = cDIB.Init(strDir + _T("Model.bmp"));
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	hStatus = cDIB.GetImage(tmpIImageModel);
	if (!IsSuccess(hStatus))
	{
		return false;
	}
	/////////////CameraParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("CamParam"));
	if (!bRet)
	{
		m_CamHeight = tmpIImageModel.Height();
		m_CamWidth = tmpIImageModel.Width();
	}
	else
	{
		cXMLConfigurator.EnterElem();
		{
			bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
			if (!bRet)
			{
				m_CamHeight = tmpIImageModel.Height();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamHeight);
			}
			bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
			if (!bRet)
			{
				m_CamWidth = tmpIImageModel.Width();
			}
			else
			{
				cXMLConfigurator.GetElemData(m_CamWidth);
			}		
		}
		cXMLConfigurator.LeaveElem();
	}
	//////////////////////////读取掩模图像//////////////////////////
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
		if (!IsSuccess(hStatus))
		{
			tmpTrainDataParam.m_bMaskEnable = false;
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask);
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_bMaskEnable = false;
			}
		}
	}
	
	//////////////////////////读取ptq文件//////////////////////////
	scQuickSearchTool tmpQuickSearchTool;
	//bRet = tmpQuickSearchTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
	bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
#ifdef VISIONPLUS_IMGPROC
		{
			if (tmpTrainDataParam.m_bMaskEnable)
			{
				bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"), strDir + _T("Model.bmp"), strDir + _T("Mask.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
					if (!bRet)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"), strDir + _T("Model.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
					if (!bRet)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}

#else
		{
			return false;
		}
#endif
	}
	
	// 数据赋值
	m_tmpImageTrain = tmpIImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	//m_QuickSearchTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
	bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}
	m_tmpTrainDataParam = tmpTrainDataParam;
	m_tmpTrainGuiParam  = tmpTrainGuiParam;
	m_tmpSearchDataParam= tmpSearchDataParam;
	m_tmpSearchGuiParam = tmpSearchGuiParam;
	m_TrainDataParam	= tmpTrainDataParam;
	m_TrainGuiParam		= tmpTrainGuiParam;
	m_SearchDataParam	= tmpSearchDataParam;
	m_SearchGuiParam	= tmpSearchGuiParam;
	m_TrainResult       = tmpTrainResult;
	m_ImageTrain		= tmpIImageModel;
	m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;
	
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CCrossSearchTool::SaveModelToFile(CString strDir)
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
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
	// grau
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau);
	cXMLConfigurator.AddElem(_T("Grau"), strInfo);
	// noise thre
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre);
	cXMLConfigurator.AddElem(_T("NoiseThre"), strInfo);
	// total mag
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag);
	cXMLConfigurator.AddElem(_T("TotalMag"), strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();


	// train affine rect
	cXMLConfigurator.AddElem(_T("TrainAffineRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetXRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect.GetSkew().ToDouble());
	cXMLConfigurator.AddElem(_T("Skew"), strInfo);
	cXMLConfigurator.LeaveElem();
	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// search rect
	cXMLConfigurator.AddElem(_T("SearchRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();
// 	// color
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
// 	// model point visible  
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// search rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	/////////////SearchGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();


// 	// color
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
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
	
	//////////////////////////存储掩模图像//////////////////////////
	if (m_TrainDataParam.m_bMaskEnable && m_ImageMask.IsValid())
	{
		hStatus = cDIB.Init(m_ImageMask);
		if (!IsSuccess(hStatus))
		{
			return false;
		}
		hStatus = cDIB.Write(strDir + _T("Mask.bmp"));
		if (!IsSuccess(hStatus))
		{
			return false;
		}
	}
	
	//////////////////////////存储模板ptq文件//////////////////////////
	//bRet = m_QuickSearchTool.SaveTrainModelToFile(strDir + _T("Model.ptq"));
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}


	return true;
}
	
// 更新训练控件数据信息	
bool CCrossSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变
	
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);
	
		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);
	}

	return true;
}

bool CCrossSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
		{
			m_pGuiSearchAffineRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchAffineRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiSearchCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CCrossSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CCrossSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CCrossSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	return true;
}

bool CCrossSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen, rot,skew;

	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
	
	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CCrossSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;
}

bool CCrossSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchAffineRectArray[j]->GetTipText());
		}
		
		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
		}
		
	}

	return true;
}

// 获取训练模板原始图像
bool CCrossSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	// 	bool bRet = m_QuickSearchTool.GetTrainModelImage(ImagePattern);
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 		
	// 	return true;


	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}

	if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 恢复训练参数
bool CCrossSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}
											
// 恢复搜索参数		
bool CCrossSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}
void CCrossSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}	
CString	CCrossSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							//几何定位失败
				strErrInfo += _T("Quick search failed");
				break;
			case -2:							//十字失败
				strErrInfo += _T("Cross search failed");
				break;
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
			case -1:							//几何定位失败
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							//区域定位2失败
				strErrInfo += _T("十字失败");
				break;
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
			case -1:							//几何定位失败
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							//区域定位2失败
				strErrInfo += _T("十字失败");
				break;
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