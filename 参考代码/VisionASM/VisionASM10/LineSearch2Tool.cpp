// LineSearch2Tool.cpp: implementation of the CLineSearch2Tool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineSearch2Tool.h"
#include "svGuiLine.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineSearch2Tool::CLineSearch2Tool()
{
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bTrainResultGuiShow					= false;
	m_bSearchResultGuiShow					= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(250, 200, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiFindLine.SetStartXYEndXY(100, 300, 200, 200);
	//m_GuiFindLine.SetLabel(_T("找线工具"));
	m_GuiFindLine.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_dGrau					= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre			= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag				= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable			= false;
	m_FindLineTool.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, 
										m_tmpTrainDataParam.m_lFilterHalfWidth);
	m_tmpTrainDataParam.m_nOutLineNum			= 0;

	m_tmpTrainGuiParam.m_nCaliperNum			= m_GuiFindLine.GetCaliperNum();
	m_tmpTrainGuiParam.m_TrainAffineRect		= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();
	m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX,
									m_tmpTrainGuiParam.m_dEndY);
	m_tmpTrainGuiParam.m_dCaliperProjectLen		= m_GuiFindLine.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();

	m_tmpSearchDataParam.m_dAcceptThre			= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow			= -10.0;
	m_tmpSearchDataParam.m_dAngleHigh			= 10.0;
	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CLineSearch2Tool::CLineSearch2Tool(CLineSearch2Tool &cLineSearch2Tool)
{
	if (cLineSearch2Tool.GetSearchToolType() != eLineSearch2Tool)
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
	m_bTrainResultGuiShow					= false;
	m_bSearchResultGuiShow					= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(250, 200, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiFindLine.SetStartXYEndXY(100, 300, 200, 200);
	//m_GuiFindLine.SetLabel(_T("找线工具"));
	m_GuiFindLine.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cLineSearch2Tool.m_CamHeight;
	m_CamWidth								= cLineSearch2Tool.m_CamWidth;
	m_TrainDataParam						= cLineSearch2Tool.m_TrainDataParam;
	m_TrainGuiParam							= cLineSearch2Tool.m_TrainGuiParam;
	m_SearchDataParam						= cLineSearch2Tool.m_SearchDataParam;
	m_SearchGuiParam						= cLineSearch2Tool.m_SearchGuiParam;
	m_TrainResult							= cLineSearch2Tool.m_TrainResult;
	m_ImageTrain							= cLineSearch2Tool.m_ImageTrain;
	m_ImageMask								= cLineSearch2Tool.m_ImageMask;
	m_bTrainGuiShow							= cLineSearch2Tool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cLineSearch2Tool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cLineSearch2Tool.m_bSearchResultGuiShow;
	m_bTrainOK								= cLineSearch2Tool.m_bTrainOK;
	m_pGuiDisplay							= cLineSearch2Tool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cLineSearch2Tool.UpdateSearchGuiData(true);
	cLineSearch2Tool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cLineSearch2Tool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cLineSearch2Tool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cLineSearch2Tool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cLineSearch2Tool.m_tmpSearchGuiParam);
	SetTrainImage(cLineSearch2Tool.m_tmpImageTrain);
	SetMaskImage(cLineSearch2Tool.m_tmpImageMask);
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
		
		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
	}		
}

CLineSearch2Tool& CLineSearch2Tool::operator=(CLineSearch2Tool &cLineSearch2Tool)
{
	if (this == &cLineSearch2Tool)
	{
		return *this;
	}

	if (cLineSearch2Tool.GetSearchToolType() != eLineSearch2Tool)
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
	m_bTrainResultGuiShow					= false;
	m_bSearchResultGuiShow					= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(250, 200, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiFindLine.SetStartXYEndXY(100, 300, 200, 200);
	//m_GuiFindLine.SetLabel(_T("找线工具"));
	m_GuiFindLine.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cLineSearch2Tool.m_CamHeight;
	m_CamWidth								= cLineSearch2Tool.m_CamWidth;
	m_TrainDataParam						= cLineSearch2Tool.m_TrainDataParam;
	m_TrainGuiParam							= cLineSearch2Tool.m_TrainGuiParam;
	m_SearchDataParam						= cLineSearch2Tool.m_SearchDataParam;
	m_SearchGuiParam						= cLineSearch2Tool.m_SearchGuiParam;
	m_TrainResult							= cLineSearch2Tool.m_TrainResult;
	m_ImageTrain							= cLineSearch2Tool.m_ImageTrain;
	m_ImageMask								= cLineSearch2Tool.m_ImageMask;
	m_bTrainGuiShow							= cLineSearch2Tool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cLineSearch2Tool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cLineSearch2Tool.m_bSearchResultGuiShow;
	m_bTrainOK								= cLineSearch2Tool.m_bTrainOK;
	m_pGuiDisplay							= cLineSearch2Tool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cLineSearch2Tool.UpdateSearchGuiData(true);
	cLineSearch2Tool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cLineSearch2Tool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cLineSearch2Tool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cLineSearch2Tool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cLineSearch2Tool.m_tmpSearchGuiParam);
	SetTrainImage(cLineSearch2Tool.m_tmpImageTrain);
	SetMaskImage(cLineSearch2Tool.m_tmpImageMask);
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
		
		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
	}		

	return *this;
}

CLineSearch2Tool::~CLineSearch2Tool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CLineSearch2Tool::GetSearchToolType()
{
	return eLineSearch2Tool;
}										   

// 设置显示控件指针		
bool CLineSearch2Tool::SetDisplay(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();
	
	m_pGuiDisplay = pGuiDisplay;
	m_hDispWnd = NULL;
	if (m_pGuiDisplay)
	{
		m_hDispWnd = m_pGuiDisplay->GetSafeHwnd();
	}
	m_bSearchResultGuiShow = FALSE;
	m_bTrainResultGuiShow  = FALSE;
	m_bTrainGuiShow        = FALSE;

	// 设置Gui控件
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	// 显示控件
	UpdateTrainGuiDisplay();
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 获取显示控件指针										
scGuiDisplay* CLineSearch2Tool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CLineSearch2Tool::SetGuiKeyText(CString strGuiKeyText)									
{
	if ((strGuiKeyText == _T("")) || (strGuiKeyText == m_strKeyText))
	{
		return false;
	}
	
	ClearAllGui();
	
	m_strKeyText = strGuiKeyText;
	
	UpdateTrainGuiDisplay();
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 获取Gui图形的关键字
bool CLineSearch2Tool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CLineSearch2Tool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CLineSearch2Tool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;
	
	UpdateTrainResultGuiDisplay();
	
	return true;
}	

// 更新搜索界面显示									
bool CLineSearch2Tool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}


// 更新训练界面显示					
bool CLineSearch2Tool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiFindLine.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine, m_strKeyText+_T("_FindLineTool"));	
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine, m_strKeyText+_T("_FindLineTool"));	

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新训练结果Gui控件显示
bool CLineSearch2Tool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine.SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine, m_strKeyText+_T("_TrainResultLine"));
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}														

// 更新搜索界面显示									
bool CLineSearch2Tool::UpdateSearchResultGuiDisplay()						
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
	
	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
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
		
		for (i=0; i<m_pGuiSearchLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
		}
		
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CLineSearch2Tool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

//	UpdateTrainResultGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CLineSearch2Tool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CLineSearch2TrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CLineSearch2Tool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	*((CLineSearch2TrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CLineSearch2Tool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CLineSearch2TrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CLineSearch2Tool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CLineSearch2TrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CLineSearch2Tool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CLineSearch2Tool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CLineSearch2Tool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CLineSearch2Tool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CLineSearch2Tool::Train()
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

	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		bool bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
								m_tmpTrainGuiParam.m_ModelPoint.GetY());
		if (!bRet)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		
		// 找线
		m_FindLineTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindLineTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchRotation, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindLineTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum, eRansacRobust);
		m_FindLineTool.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, 
			m_tmpTrainGuiParam.m_dEndX, m_tmpTrainGuiParam.m_dEndY);

		scLineSeg cLineSeg;
		double dRMS;
		bRet = m_FindLineTool.Execute(m_tmpImageTrain, cLineSeg, dRMS);
		if (!bRet)
		{
			m_nResultErrNumber=-2;       //找线失败
			return false;
		}
		m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
		m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
		m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
		m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();

		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult	 = m_tmpTrainResult;
		m_GuiTrainResultLine.SetLine(cLineSeg.GetLine());

		return true;
	}
	else
	{
		bool bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
			m_tmpTrainGuiParam.m_ModelPoint.GetY());
		if (!bRet)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		
		// 找线
		m_FindLineTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindLineTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchRotation, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindLineTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum, eRansacRobust);
		m_FindLineTool.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, 
			m_tmpTrainGuiParam.m_dEndX, m_tmpTrainGuiParam.m_dEndY);
		scLineSeg cLineSeg;
		double dRMS;
		bRet = m_FindLineTool.Execute(m_tmpImageTrain, cLineSeg, dRMS);
		if (!bRet)
		{
			m_nResultErrNumber=-2;       //找线失败
			return false;
		}
		m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
		m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
		m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
		m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();

		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult	 = m_tmpTrainResult;
		m_GuiTrainResultLine.SetLine(cLineSeg.GetLine());

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CLineSearch2Tool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool.GetTrainResultImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}
	
// 获取训练结果
bool CLineSearch2Tool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	*((CLineSearch2TrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}							

// 是否训练成功
bool CLineSearch2Tool::IsTrained()
{
	return m_bTrainOK;
}
		
// 设置搜索Data参数	
bool CLineSearch2Tool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CLineSearch2SearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CLineSearch2Tool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}

	*((CLineSearch2SearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CLineSearch2Tool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CLineSearch2SearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CLineSearch2Tool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eLineSearch2Tool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CLineSearch2SearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CLineSearch2Tool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 训练时的线段1
	scLineSeg modelLineSeg;
	modelLineSeg.SetP1(sc2Vector(m_TrainResult.m_dStartX, m_TrainResult.m_dStartY));
	modelLineSeg.SetP2(sc2Vector(m_TrainResult.m_dEndX, m_TrainResult.m_dEndY));

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

	m_FindLineTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
	m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchRotation, m_TrainGuiParam.m_dCaliperSearchLen
									,m_TrainGuiParam.m_dCaliperProjectLen);
	m_FindLineTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
	m_FindLineTool.SetOutLineNum(m_TrainDataParam.m_nOutLineNum, eRansacRobust);

	// 执行搜索
	bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_QuickSearchTool.GetResultNum();
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
		
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
		
			// 求找线工具变换后的位置,找线
			double dTransStartX, dTransStartY;
			double dTransEndX, dTransEndY;
			dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dStartX  - sin(dRotation) * m_TrainGuiParam.m_dStartY + dTransX;
			dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dStartX  + cos(dRotation) * m_TrainGuiParam.m_dStartY + dTransY;
			dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dEndX  - sin(dRotation) * m_TrainGuiParam.m_dEndY + dTransX;
			dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dEndX  + cos(dRotation) * m_TrainGuiParam.m_dEndY + dTransY;
			m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, /*scRadian(dRotation)+*/m_TrainGuiParam.m_CaliperSearchRotation, m_TrainGuiParam.m_dCaliperSearchLen
				,m_TrainGuiParam.m_dCaliperProjectLen);
			m_FindLineTool.SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);

			// 执行找线
			scLineSeg ResLineSeg;
			double dRMS;
			if (m_FindLineTool.Execute(ImageSearch, ResLineSeg, dRMS)) // 成功
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = ResLineSeg.GetP1().GetX();	// 结果直线上端点X
				searchResult.m_dPosY = ResLineSeg.GetP1().GetY();	// 结果直线上端点Y
				searchResult.m_dAngle = scDegree(ResLineSeg.GetRotation()).ToDouble(); // 结果直线角度
				searchResult.m_dScore = tmpQuickResult.GetScore();

				// 设置训练矩形
				CResultSearchRect resRect;
				resRect.m_searchRect = tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);
				// line
				CResultLine line;
				line.m_vLineX[0] = ResLineSeg.GetP1().GetX();
				line.m_vLineY[0] = ResLineSeg.GetP1().GetY();
				line.m_vLineX[1] = ResLineSeg.GetP2().GetX();
				line.m_vLineY[1] = ResLineSeg.GetP2().GetY();
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
				pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray.push_back(pTempCross);
				

				strTmpKey.Format(_T("%s_ResultLine%d"), m_strKeyText, i);
				//scGuiLine *pTemLine = new scGuiLine;
				scGuiLine *pTemLine = m_GuiSearchLine+i;
				pTemLine->SetLine(ResLineSeg.GetLine());
				pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemLine->SetTipTextEnabled(true);
				pTemLine->SetTipText(strTmpKey);

				CString st;
				st.Format(_T("%d X:%.1f;Y:%.1f;D:.3f"), i, ResLineSeg.GetLine().GetPos().GetX(), ResLineSeg.GetLine().GetPos().GetY(), 
					scDegree(ResLineSeg.GetLine().GetRotation()).ToDouble());
				pTemLine->SetLabel(st);
				m_pGuiSearchLineArray.push_back(pTemLine);

			}
			else
			{
				m_nResultErrNumber=-2;       //找线失败
				return false;
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

	return true;
}

// 获取搜索结果个数										
int CLineSearch2Tool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CLineSearch2Tool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
				
// 加载模板文件
bool CLineSearch2Tool::LoadModelFromFile(CString strDir)
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
	CLineSearch2TrainDataParam tmpTrainDataParam;
	CLineSearch2TrainGuiParam tmpTrainGuiParam;
	CLineSearch2SearchDataParam tmpSearchDataParam;
	CLineSearch2SearchGuiParam tmpSearchGuiParam;
	CLineSearch2TrainResult    tmpTrainResult;

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
	if (cSearchToolType != eLineSearch2Tool)
	{
		return false;
	}

	/////////////TrainDataParam/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	// contrastthre
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	// outlinenum
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 	
// 	//////////////TrainGuiParam/////////////
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
	// CaliperNum
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchRotation = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	
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
// 	// Line visible 
// 	bRet = cXMLConfigurator.FindElem(_T("LineVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bLineVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	}
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam/////////////
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
// 	/////////////SearchGuiParam/////////////
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
// 	// Line visible 
// 	bRet = cXMLConfigurator.FindElem(_T("LineVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bLineVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	bRet = cXMLConfigurator.FindElem(_T("StartX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("StartY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dStartX = vPos.GetX();
	tmpTrainResult.m_dStartY = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("EndY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dEndX = vPos.GetX();
	tmpTrainResult.m_dEndY = vPos.GetY();
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
	m_tmpImageTrain = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
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
	m_ImageTrain		= tmpImageModel;
	m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}
	
// 保存模板文件							
bool CLineSearch2Tool::SaveModelToFile(CString strDir)
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


	/////////////TrainDataParam/////////////
	cXMLConfigurator.AddElem(_T("TrainParam"));
	cXMLConfigurator.EnterElem();
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
	// contrastthre
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre);
	cXMLConfigurator.AddElem(_T("ContrastThre"), strInfo);
	// fileterhalfwidth
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth"), strInfo);
	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum);
	cXMLConfigurator.AddElem(_T("OutLineNum"), strInfo);
	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty);
	cXMLConfigurator.AddElem(_T("EdgeProperty"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 
// 	//////////////TrainGuiParam/////////////
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

	/////////////search rect/////////////
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
	// Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum);
	cXMLConfigurator.AddElem(_T("CaliperNum"), strInfo);
	// startX
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX);
	cXMLConfigurator.AddElem(_T("StartX"), strInfo);
	// startY
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY);
	cXMLConfigurator.AddElem(_T("StartY"), strInfo);
	// EndX
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX);
	cXMLConfigurator.AddElem(_T("EndX"), strInfo);
	// EndY
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY);
	cXMLConfigurator.AddElem(_T("EndY"), strInfo);
	// Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen);
	cXMLConfigurator.AddElem(_T("ProjectLen"), strInfo);
	// Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen);
	cXMLConfigurator.AddElem(_T("SearchLen"), strInfo);
	// Search Rotation
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation.ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation"), strInfo);
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// search rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	// findcircle visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineVisible);
// 	cXMLConfigurator.AddElem(_T("LineVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam/////////////
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
// 	/////////////SearchGuiParam/////////////
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// line visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bLineVisible);
// 	cXMLConfigurator.AddElem(_T("LineVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX);
	cXMLConfigurator.AddElem(_T("StartX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY);
	cXMLConfigurator.AddElem(_T("StartY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX);
	cXMLConfigurator.AddElem(_T("EndX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY);
	cXMLConfigurator.AddElem(_T("EndY"), strInfo);
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
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	
	
// 更新训练控件数据信息	
bool CLineSearch2Tool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nCaliperNum		= m_GuiFindLine.GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen	= m_GuiFindLine.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();
		m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX,
			m_tmpTrainGuiParam.m_dEndY);
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

		m_GuiFindLine.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindLine.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_GuiFindLine.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX,
									m_tmpTrainGuiParam.m_dEndY);
		m_GuiFindLine.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_GuiFindLine.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen);
		m_GuiFindLine.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation);
	}

	return true;
}

bool CLineSearch2Tool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CLineSearch2Tool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}
																		
// 清除定位工具在Display上的控件			
bool CLineSearch2Tool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CLineSearch2Tool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	
	return true;
}
	
bool CLineSearch2Tool::SetGuiByCam(double m_Width,double m_Height)
{
	//训练区域
// 	double centerx,centery, xlen,ylen, rot,skew;
// 
// 	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
// 	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_lastCamWidth)*m_CamWidth, (centery/m_lastCamWidth)*m_CamWidth, (xlen/m_lastCamWidth)*m_CamWidth, (ylen/m_lastCamWidth)*m_CamWidth,rot,skew);
// 
// 	//搜索区域
// 
// 	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
// 	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_lastCamWidth)*m_CamWidth, (centery/m_lastCamWidth)*m_CamWidth, (xlen/m_lastCamWidth)*m_CamWidth, (ylen/m_lastCamWidth)*m_CamWidth);
// 
// 	//参考点
// 	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
// 	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_lastCamWidth)*m_CamWidth, (centery/m_lastCamWidth)*m_CamWidth, (xlen/m_lastCamWidth)*m_CamWidth, (ylen/m_lastCamWidth)*m_CamWidth);
// 	//m_GuiModelPoint.SetLabel(_T("参考点"));
// 
// 	//卡尺
// 	double sx,sy,ex,ey;
// 	m_GuiFindLine.GetStartXYEndXY(sx,sy,ex,ey);
// 	m_GuiFindLine.SetStartXYEndXY((sx/m_lastCamWidth)*m_CamWidth, (sy/m_lastCamWidth)*m_CamWidth,(ex/m_lastCamWidth)*m_CamWidth,(ey/m_lastCamWidth)*m_CamWidth);
// 
// 
// 	double plength,slength;
// 	plength = m_GuiFindLine.GetCaliperProjectionLength();
// 	slength = m_GuiFindLine.GetCaliperSearchLength();
// 	m_GuiFindLine.SetCaliperProjectionLength(plength/m_lastCamWidth*m_CamWidth);
// 	m_GuiFindLine.SetCaliperSearchLength(slength/m_lastCamWidth*m_CamWidth);

	return true;
}

bool CLineSearch2Tool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindLineTool"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine"));
	}	
	
	return true;
}	

bool CLineSearch2Tool::ClearResultGui()
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
		
		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}
	}	
	
	return true;
}	

// 获取训练模板原始图像
bool CLineSearch2Tool::GetPatternImage(cpImage &ImagePattern)
{
	bool bRet = m_QuickSearchTool.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}
	
	return true;
}	

// 恢复训练参数
bool CLineSearch2Tool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CLineSearch2Tool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CLineSearch2Tool::SetLanguage(int nLanguage)
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
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}
CString	CLineSearch2Tool::GetResultErrInfo()
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
			case -2:							//找线失败
				strErrInfo += _T("Find line failed");
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
			case -2:							//找线失败
				strErrInfo += _T("找线失败");
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
			case -2:							//找线失败
				strErrInfo += _T("找线失败");
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