// PatternSearchGenRectTool.cpp: implementation of the CPatternSearchGenRectTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatternSearchGenRectTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
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

CPatternSearchGenRectTool::CPatternSearchGenRectTool()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bTrainResultGuiShow                       = false;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	
	
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindGenRect.SetCenterLengthsRotationRound(sc2Vector(350,240), 200, 200, scRadian(0), 30);
	m_GuiFindGenRect.SetCaliperParams(20, eInward, 50, 20);


	m_GuiFindGenRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindGenRect.SetLabel(_T("FindGenRect"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_FindGenRectTool.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, 
		m_tmpTrainDataParam.m_lFilterHalfWidth);
	m_tmpTrainDataParam.m_nOutLineNum			= 10;
	
	m_tmpTrainGuiParam.m_nCaliperNum			= m_GuiFindGenRect.GetCaliperNumLineX();
	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_GenRect				= m_GuiFindGenRect.GetGenRect();
	m_tmpTrainGuiParam.m_dCaliperProjectLen		= m_GuiFindGenRect.GetCaliperProjectionLengthLineX();
	m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindGenRect.GetCaliperSearchLengthLineX();
	m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindGenRect.GetCaliperSearchDirectionLineX();

	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;
	m_tmpSearchDataParam.m_nMethod				= 0;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CPatternSearchGenRectTool::CPatternSearchGenRectTool(CPatternSearchGenRectTool &cPatternSearchGenRectTool)
{
	if (cPatternSearchGenRectTool.GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return;
	}
	
	// 清空
// 	ClearAllGui();
// 	ClearResult();
	
	// 初始化
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bTrainResultGuiShow						= false;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindGenRect.SetCenterLengthsRotationRound(sc2Vector(350,240), 200, 200, scRadian(0), 30);
	m_GuiFindGenRect.SetCaliperParams(20, eInward, 50, 20);

	m_GuiFindGenRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindGenRect.SetLabel(_T("FindGenRect"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cPatternSearchGenRectTool.m_CamHeight;
	m_CamWidth								= cPatternSearchGenRectTool.m_CamWidth;

	m_TrainDataParam						= cPatternSearchGenRectTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternSearchGenRectTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternSearchGenRectTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternSearchGenRectTool.m_SearchGuiParam;
	m_TrainResult							= cPatternSearchGenRectTool.m_TrainResult;
	m_ImageTrain							= cPatternSearchGenRectTool.m_ImageTrain;
	m_ImageMask								= cPatternSearchGenRectTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternSearchGenRectTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cPatternSearchGenRectTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternSearchGenRectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternSearchGenRectTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternSearchGenRectTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternSearchGenRectTool.UpdateSearchGuiData(true);
	cPatternSearchGenRectTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternSearchGenRectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternSearchGenRectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternSearchGenRectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternSearchGenRectTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternSearchGenRectTool.m_tmpImageTrain);
	SetMaskImage(cPatternSearchGenRectTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = cPatternSearchGenRectTool.m_PatternSearchTool;
	}
}

CPatternSearchGenRectTool& CPatternSearchGenRectTool::operator=(CPatternSearchGenRectTool &cPatternSearchGenRectTool)
{
	if (this == &cPatternSearchGenRectTool)
	{
		return *this;
	}

	if (cPatternSearchGenRectTool.GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return *this;
	}
	
	// 清空
	ClearAllGui();
	ClearResult();
	
	// 初始化
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bTrainResultGuiShow						= false;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindGenRect.SetCenterLengthsRotationRound(sc2Vector(350,240), 200, 200, scRadian(0), 30);
	m_GuiFindGenRect.SetCaliperParams(20, eInward, 50, 20);

	m_GuiFindGenRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindGenRect.SetLabel(_T("FindGenRect"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cPatternSearchGenRectTool.m_CamHeight;
	m_CamWidth								= cPatternSearchGenRectTool.m_CamWidth;

	m_TrainDataParam						= cPatternSearchGenRectTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternSearchGenRectTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternSearchGenRectTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternSearchGenRectTool.m_SearchGuiParam;
	m_TrainResult							= cPatternSearchGenRectTool.m_TrainResult;
	m_ImageTrain							= cPatternSearchGenRectTool.m_ImageTrain;
	m_ImageMask								= cPatternSearchGenRectTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternSearchGenRectTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cPatternSearchGenRectTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternSearchGenRectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternSearchGenRectTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternSearchGenRectTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternSearchGenRectTool.UpdateSearchGuiData(true);
	cPatternSearchGenRectTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternSearchGenRectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternSearchGenRectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternSearchGenRectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternSearchGenRectTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternSearchGenRectTool.m_tmpImageTrain);
	SetMaskImage(cPatternSearchGenRectTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = cPatternSearchGenRectTool.m_PatternSearchTool;
	}

	return *this;
}

CPatternSearchGenRectTool::~CPatternSearchGenRectTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CPatternSearchGenRectTool::GetSearchToolType()
{
	return ePatternSearchGenRectTool;
}										   

// 设置显示控件指针		
bool CPatternSearchGenRectTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CPatternSearchGenRectTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternSearchGenRectTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CPatternSearchGenRectTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternSearchGenRectTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;
	
	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternSearchGenRectTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CPatternSearchGenRectTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 更新训练Gui控件显示
bool CPatternSearchGenRectTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiFindGenRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bGenRectVisible);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindGenRect, m_strKeyText+_T("_FindGenRect"));

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}	

// 更新训练结果Gui控件显示
bool CPatternSearchGenRectTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultGenRect.SetVisible(m_bTrainResultGuiShow);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultGenRect, m_strKeyText+_T("_TrainResultGenRect"));
		
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}		

// 更新搜索Gui控件显示														
bool CPatternSearchGenRectTool::UpdateSearchResultGuiDisplay()
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
	
	for (i=0; i<m_pGuiSearchGenRectArray.size(); i++)
	{
		m_pGuiSearchGenRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bGenRectVisible);
	}
	
	for (i=0; i<m_pGuiSearchGenRectCenterArray.size(); i++)
	{
		m_pGuiSearchGenRectCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bGenRectCenterVisible);
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
		
		for (i=0; i<m_pGuiSearchGenRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchGenRectArray[i], m_pGuiSearchGenRectArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchGenRectCenterArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchGenRectCenterArray[i], m_pGuiSearchGenRectCenterArray[i]->GetTipText());
		}
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CPatternSearchGenRectTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

//	UpdateTrainResultGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternSearchGenRectTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CPatternSearchGenRectTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CPatternSearchGenRectTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	*((CPatternSearchGenRectTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CPatternSearchGenRectTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CPatternSearchGenRectTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CPatternSearchGenRectTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CPatternSearchGenRectTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CPatternSearchGenRectTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternSearchGenRectTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CPatternSearchGenRectTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternSearchGenRectTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CPatternSearchGenRectTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);
	m_nResultErrNumber=0;
	// 设置训练参数
	if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber = -1;
			return false;
		}
	}
	else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber = -1;
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
			bRet = m_PatternSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
				&m_tmpTrainGuiParam.m_TrainRect);
		}
		else
		{
			bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
				&m_tmpTrainGuiParam.m_TrainRect);
		}

		if (!bRet)
		{
			m_nResultErrNumber=-1;////区域定位失败
			return false;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		// 找圆
		m_FindGenRectTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindGenRectTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchDirection, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_FindGenRectTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindGenRectTool.SetOutLinePrecent(m_tmpTrainDataParam.m_nOutLineNum);
		m_FindGenRectTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
		m_FindGenRectTool.SetGenRect(m_tmpTrainGuiParam.m_GenRect);
		scGenRect cGenRect;
		double   dRMS;
		if(bIsProcessImage)
		{
			bRet = m_FindGenRectTool.Execute(tmpImage, cGenRect, dRMS);
		}
		else
		{
			bRet = m_FindGenRectTool.Execute(m_tmpImageTrain, cGenRect, dRMS);
		}
		if (!bRet)
		{
			m_nResultErrNumber=-2;////找圆工具失败
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cGenRect.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cGenRect.GetCenter().GetY();

		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultGenRect.SetGenRect(cGenRect);

		return true;
	}
	else
	{
		bool bRet = false;
		if (bIsProcessImage)
		{
			bRet = m_PatternSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint,
				&m_tmpTrainGuiParam.m_TrainRect);
		}
		else
		{
			bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
				&m_tmpTrainGuiParam.m_TrainRect);
		}

		if (!bRet)
		{
			m_nResultErrNumber=-1;////区域定位失败
			return false;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		
		// 找圆
		m_FindGenRectTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindGenRectTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchDirection, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_FindGenRectTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindGenRectTool.SetOutLinePrecent(m_tmpTrainDataParam.m_nOutLineNum);
		m_FindGenRectTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
		m_FindGenRectTool.SetGenRect(m_tmpTrainGuiParam.m_GenRect);
		scGenRect cGenRect;
		double   dRMS;
		if(bIsProcessImage)
		{
			bRet = m_FindGenRectTool.Execute(tmpImage, cGenRect, dRMS);
		}
		else
		{
			bRet = m_FindGenRectTool.Execute(m_tmpImageTrain, cGenRect, dRMS);
		}
			
		if (!bRet)
		{
			m_nResultErrNumber=-2;////区域定位失败
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cGenRect.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cGenRect.GetCenter().GetY();


		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultGenRect.SetGenRect(cGenRect);

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CPatternSearchGenRectTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);

	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CPatternSearchGenRectTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	*((CPatternSearchGenRectTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}	

// 是否训练成功
bool CPatternSearchGenRectTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CPatternSearchGenRectTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternSearchGenRectSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternSearchGenRectTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}

	*((CPatternSearchGenRectSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternSearchGenRectTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CPatternSearchGenRectSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CPatternSearchGenRectTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != ePatternSearchGenRectTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CPatternSearchGenRectSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CPatternSearchGenRectTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;////训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 设置搜索个数
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	m_FindGenRectTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
	m_FindGenRectTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
		,m_TrainGuiParam.m_dCaliperProjectLen);
	m_FindGenRectTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
	m_FindGenRectTool.SetOutLinePrecent(m_TrainDataParam.m_nOutLineNum);
	m_FindGenRectTool.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore);

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
	//bool bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	bool bRet = false;
	if (bIsProcessImage)
	{
		bRet = m_PatternSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
	}
	else
	{
		bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	}
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool.GetResultNum();
		if(nResultNum<=0)
		{
			m_nResultErrNumber = -1;
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);
			
			// 求变换矩阵
			sc2Vector pos = tmpPatternResult.GetPosition();
			sc2Vector tmModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_PatternSearchTool.GetPatternOrigin(tmModelPoint);
			dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
			
			// 求找线工具变换后的位置,找线
			double dTransCenterX, dTransCenterY;
			double dOriCenterX, dOriCenterY;
			dOriCenterX = m_TrainGuiParam.m_GenRect.GetCenter().GetX();
			dOriCenterY = m_TrainGuiParam.m_GenRect.GetCenter().GetY();
			dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
			dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
			m_FindGenRectTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
				,m_TrainGuiParam.m_dCaliperProjectLen);
			m_FindGenRectTool.SetCenterLengthsRotationRound(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_GenRect.GetXLength(),
				m_TrainGuiParam.m_GenRect.GetYLength(), scRadian(dRotation)+m_TrainGuiParam.m_GenRect.GetRotation(), m_TrainGuiParam.m_GenRect.GetRound());

			// 执行找圆
			scGenRect ResGenRect;
			double dRMS;
			if(bIsProcessImage)
			{
				bRet = m_FindGenRectTool.Execute(tmpImage, ResGenRect, dRMS, (scFindGenRect::FindMethod)(m_tmpSearchDataParam.m_nMethod+1));
			}
			else
			{
				bRet = m_FindGenRectTool.Execute(ImageSearch, ResGenRect, dRMS, (scFindGenRect::FindMethod)(m_tmpSearchDataParam.m_nMethod+1));
			}
			if (bRet) // 成功
			{
				CSearchResult searchResult;
				searchResult.m_dPosX = ResGenRect.GetCenter().GetX();
				searchResult.m_dPosY = ResGenRect.GetCenter().GetY();
				searchResult.m_dAngle = 0;
				searchResult.m_dScore = tmpPatternResult.GetScore();

				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				// 设置圆角
				CResultGenRect genRect;
				genRect.m_scGenRect = ResGenRect;
				searchResult.m_vGenRectSearchResult.push_back(genRect);

				m_SearchResultArray.push_back(searchResult);
				
				CString	strTmpKey;
				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
				//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
				
				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				//scGuiCross *pTempCross = new scGuiCross;
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray.push_back(pTempCross);
				
				strTmpKey.Format(_T("%s_ResultGenRect%d"), m_strKeyText, i);
				//scGuiCircle *pTemCircle = new scGuiCircle;
				scGuiGenRect *pTemGenRect = m_GuiSearchGenRect+i;
				pTemGenRect->SetGenRect(ResGenRect);
				pTemGenRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTemGenRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemGenRect->SetTipTextEnabled(true);
				pTemGenRect->SetTipText(strTmpKey);
				CString st;
				st.Format(_T("%d X:%.1f;Y:%.1f"), i, ResGenRect.GetCenter().GetX(), ResGenRect.GetCenter().GetY());
				pTemGenRect->SetLabel(st);
				pTemGenRect->SetLabelVisible(TRUE);
				m_pGuiSearchGenRectArray.push_back(pTemGenRect);

				strTmpKey.Format(_T("%s_ResultGenRectCenter%d"), m_strKeyText, i);
				//scGuiCross *pTempCircleCenter = new scGuiCross;
				scGuiCross *pTempGenRectCenter = m_GuiSearchGenRectCenter+i;
				pTempGenRectCenter->SetCenterExtent(ResGenRect.GetCenter(), sc2Vector(40,40));
				pTempGenRectCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempGenRectCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempGenRectCenter->SetTipTextEnabled(true);
				pTempGenRectCenter->SetTipText(strTmpKey);
				m_pGuiSearchGenRectCenterArray.push_back(pTempGenRectCenter);	
			}	

		}		

		if (m_SearchResultArray.size() <= 0)
		{
			m_nResultErrNumber=-2;////找圆工具失败
			return false;
		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		m_nResultErrNumber=-1;////区域定位失败
		return false;
	}

	return true;
}

// 获取搜索结果个数										
int CPatternSearchGenRectTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternSearchGenRectTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternSearchGenRectTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternSearchGenRectTool)
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
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	rRect.SetOrigion(vPos);
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
	rRect.SetSize(vPos);
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
bool CPatternSearchGenRectTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) PatternSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CPatternSearchGenRectTrainDataParam tmpTrainDataParam;
	CPatternSearchGenRectTrainGuiParam tmpTrainGuiParam;
	CPatternSearchGenRectSearchDataParam tmpSearchDataParam;
	CPatternSearchGenRectSearchGuiParam tmpSearchGuiParam;
	CPatternSearchGenRectTrainResult  tmpTrainResult;
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
	if (cSearchToolType != ePatternSearchGenRectTool)
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
	// coarse scale
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());

	// contrast thre
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

	// SortByScore
	bRet = cXMLConfigurator.FindElem(_T("SortByScore"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore = true;
	}
	tmpTrainDataParam.m_bSortByScore = _ttoi(cXMLConfigurator.GetElemData());


//  //将TrainGuiParam的参数，放在TrainDataParam括号中
//	cXMLConfigurator.LeaveElem();
//	
// 	//////////////TrainGuiParam//////////////
// 	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();


	// Train rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	tmpTrainGuiParam.m_TrainRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_TrainRect.SetSize(vPos);
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

	// GenRect
	bRet = cXMLConfigurator.FindElem(_T("GenRect"));
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
	tmpTrainGuiParam.m_GenRect.SetCenter(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_GenRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_GenRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_GenRect.SetRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
	bRet = cXMLConfigurator.FindElem(_T("Round"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_GenRect.SetRound(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	cXMLConfigurator.LeaveElem();


	// Caliper Num 
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
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
	// Search Dirction
	bRet = cXMLConfigurator.FindElem(_T("SearchDir"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());
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
// 	//  search rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bSearchRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	}
// 	// circle visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bCircleVisible = _ttoi(cXMLConfigurator.GetElemData());
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

	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dConfusionThre = (tmpSearchDataParam.m_dAcceptThre + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre + 0.1);
	}
	else
	{
		tmpSearchDataParam.m_dConfusionThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("SearchMethod"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nMethod = 0;
	}
	tmpSearchDataParam.m_nMethod = _ttoi(cXMLConfigurator.GetElemData());
// 
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
// 	if (!bRet)
// 	{
// 		return false;
// 	}
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
// 	// circle visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bCircleVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// circle center visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleCenterVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bCircleCenterVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	tmpTrainResult.m_dCenterX = vPos.GetX();
	tmpTrainResult.m_dCenterY = vPos.GetY();
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
	scPatternSearchRot tmpPatternSearchTool;
	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	// 数据赋值
	m_tmpImageTrain = tmpIImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	m_PatternSearchTool = tmpPatternSearchTool;

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
bool CPatternSearchGenRectTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) PatternSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	//////////////////////////存储xml文件//////////////////////////
	vcXMLConfigurator cXMLConfigurator;
	CString strInfo;
	cXMLConfigurator.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n")); 
	cXMLConfigurator.AddElem(_T("Config"));
	cXMLConfigurator.EnterElem();

	/////////////工具类型/////////////
	strInfo.Format(_T("%d"), GetSearchToolType());
	cXMLConfigurator.AddElem(_T("ToolType"), strInfo);

	////////////CameraParam//////////
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
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
	// CoarseScale
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CoarseScale);
	cXMLConfigurator.AddElem(_T("CoarseScale"), strInfo);
	// Accuracy
	strInfo.Format(_T("%d"), m_TrainDataParam.m_Accuracy);
	cXMLConfigurator.AddElem(_T("Accuracy"), strInfo);
	// Angle Low
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	// Angle High
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);
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
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore);
	cXMLConfigurator.AddElem(_T("SortByScore"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 	
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam/////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();

	// train rect
	cXMLConfigurator.AddElem(_T("TrainRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
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

	// GenRect
	cXMLConfigurator.AddElem(_T("GenRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_GenRect.GetRound());
	cXMLConfigurator.AddElem(_T("Round"), strInfo);
	cXMLConfigurator.LeaveElem();


	// Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum);
	cXMLConfigurator.AddElem(_T("CaliperNum"), strInfo);
	// Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen);
	cXMLConfigurator.AddElem(_T("ProjectLen"), strInfo);
	// Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen);
	cXMLConfigurator.AddElem(_T("SearchLen"), strInfo);
	// Search Dircetion
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_CaliperSearchDirection);
	cXMLConfigurator.AddElem(_T("SearchDir"), strInfo);

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
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleVisible);
// 	cXMLConfigurator.AddElem(_T("CircleVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre);
	cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nMethod);
	cXMLConfigurator.AddElem(_T("SearchMethod"), strInfo);

//  // 将SearchGuiParam的参数，放在SearchDataParam括号中
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
// 	// affine rect visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// circle visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bCircleVisible);
// 	cXMLConfigurator.AddElem(_T("CircleVisible"), strInfo);
// 	// circle center visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bCircleCenterVisible);
// 	cXMLConfigurator.AddElem(_T("CircleCenterVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	
	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	//////////////////////////存储模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus = cDIB.Init(m_ImageTrain);
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
		HStatus hStatus = cDIB.Init(m_ImageMask);
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
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	
	
// 更新训练控件数据信息	
bool CPatternSearchGenRectTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		
		m_tmpTrainGuiParam.m_nCaliperNum		= m_GuiFindGenRect.GetCaliperNumLineX();
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
		m_tmpTrainGuiParam.m_GenRect			= m_GuiFindGenRect.GetGenRect();
		m_tmpTrainGuiParam.m_dCaliperProjectLen = m_GuiFindGenRect.GetCaliperProjectionLengthLineX();
		m_tmpTrainGuiParam.m_dCaliperSearchLen	= m_GuiFindGenRect.GetCaliperSearchLengthLineX();
		m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindGenRect.GetCaliperSearchDirectionLineX();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变
	
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);
	
		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiFindGenRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindGenRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindGenRect.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_GuiFindGenRect.SetGenRect(m_tmpTrainGuiParam.m_GenRect);
		m_GuiFindGenRect.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_GuiFindGenRect.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen);
		m_GuiFindGenRect.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchDirection);
	}

	return true;
}

bool CPatternSearchGenRectTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchGenRectArray.size(); j++)
		{
			m_pGuiSearchGenRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchGenRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchGenRectCenterArray.size(); j++)
		{
			m_pGuiSearchGenRectCenterArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchGenRectCenterArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CPatternSearchGenRectTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

bool CPatternSearchGenRectTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CPatternSearchGenRectTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchGenRectArray);
	ClearGraphicsArray(m_pGuiSearchGenRectCenterArray);

	return true;
}															

bool CPatternSearchGenRectTool::SetGuiByCam(double nWidth,double nHeight)
{
	//训练区域
	double centerx,centery, xlen,ylen,rot,skew;

	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiFindGenRect.GetCenterLengthsRotationRound(centerx,centery, xlen,ylen, rot,skew);
	m_GuiFindGenRect.SetCenterLengthsRotationRound((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	double plength,slength;
	plength = m_GuiFindGenRect.GetCaliperProjectionLengthLineX();
	slength = m_GuiFindGenRect.GetCaliperSearchLengthLineX();
	m_GuiFindGenRect.SetCaliperProjectionLengthLineX(plength/m_CamWidth*nWidth);
	m_GuiFindGenRect.SetCaliperSearchLengthLineX(slength/m_CamWidth*nWidth);

	plength = m_GuiFindGenRect.GetCaliperProjectionLengthLineY();
	slength = m_GuiFindGenRect.GetCaliperSearchLengthLineY();
	m_GuiFindGenRect.SetCaliperProjectionLengthLineY(plength/m_CamWidth*nWidth);
	m_GuiFindGenRect.SetCaliperSearchLengthLineY(slength/m_CamWidth*nWidth);

	plength = m_GuiFindGenRect.GetCaliperProjectionLengthCircle();
	slength = m_GuiFindGenRect.GetCaliperSearchLengthCircle();
	m_GuiFindGenRect.SetCaliperProjectionLengthCircle(plength/m_CamWidth*nWidth);
	m_GuiFindGenRect.SetCaliperSearchLengthCircle(slength/m_CamWidth*nWidth);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
	return true;
}

bool CPatternSearchGenRectTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindGenRect"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultGenRect"));
	}
	
	return true;	
}

bool CPatternSearchGenRectTool::ClearResultGui()
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
		
		for (j=0; j<m_pGuiSearchGenRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchGenRectArray[j]->GetTipText());
		}
		
		for (j=0; j<m_pGuiSearchGenRectCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchGenRectCenterArray[j]->GetTipText());
		}
	}
	
	return true;	
}

// 获取训练模板原始图像
bool CPatternSearchGenRectTool::GetPatternImage(cpImage &ImagePattern)
{
	/*bool bRet = m_PatternSearchTool.GetPatternImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}
	
	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if(cpImage::statusSuccess == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
}	

// 恢复训练参数
bool CPatternSearchGenRectTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternSearchGenRectTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternSearchGenRectTool::SetLanguage(int nLanguage)
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
		m_GuiFindGenRect.SetLabel(_T("FindGenRect"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindGenRect.SetLabel(_T("找圆角矩形工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}
CString CPatternSearchGenRectTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:			//	区域定位失败			
				strErrInfo += _T("Pattern search failed");
				break;
			case -2:			//	找圆角矩形失败			
				strErrInfo += _T("Find general rect failed");
				break;
			case -99:			// 训练未成功				
				strErrInfo += _T("The Train is NG");
				break;
			default:
				strErrInfo += _T(" ");
				break;
			}
		}
		break;
	case 1:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("找圆角矩形失败");
				break;
			case -99:							
				strErrInfo += _T("训练未成功");
				break;
			default:
				strErrInfo += _T(" ");
				break;
			}
		}
		break;
	default:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("找圆角矩形失败");
				break;
			case -99:							
				strErrInfo += _T("训练未成功");
				break;
			default:
				strErrInfo += _T(" ");
				break;
			}
		}
		break;
	}
	return strErrInfo;
}