// PatCornerSearchTool.cpp: implementation of the CPatCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatCornerSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"
#include "svLineSeg.h"
#include "svGuiLine.h"
#include "svIntersect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTwoPatModelSearchTool::CTwoPatModelSearchTool()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Accuracy2				= m_PatternSearchTool2.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale2			= m_PatternSearchTool2.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow2				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh2			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable2			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_TrainRect2				= m_GuiTrainRect2.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint2				= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect2				= m_GuiSearchRect2.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.8;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;
	
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CTwoPatModelSearchTool::CTwoPatModelSearchTool(CTwoPatModelSearchTool &cTwoPatModelSearchTool)
{
	if (cTwoPatModelSearchTool.GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return;
	}
	
	// 初始化
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cTwoPatModelSearchTool.m_CamHeight;
	m_CamWidth								= cTwoPatModelSearchTool.m_CamWidth;

	m_TrainDataParam						= cTwoPatModelSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cTwoPatModelSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cTwoPatModelSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cTwoPatModelSearchTool.m_SearchGuiParam;
	m_TrainResult							= cTwoPatModelSearchTool.m_TrainResult;
	m_ImageTrain							= cTwoPatModelSearchTool.m_ImageTrain;
	m_ImageMask								= cTwoPatModelSearchTool.m_ImageMask;
	m_ImageMask2								= cTwoPatModelSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= cTwoPatModelSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cTwoPatModelSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cTwoPatModelSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cTwoPatModelSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cTwoPatModelSearchTool.UpdateSearchGuiData(true);
	cTwoPatModelSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cTwoPatModelSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cTwoPatModelSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cTwoPatModelSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cTwoPatModelSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cTwoPatModelSearchTool.m_tmpImageTrain);
	SetMaskImage(cTwoPatModelSearchTool.m_tmpImageMask);
	SetMaskImage2(cTwoPatModelSearchTool.m_tmpImageMask2);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = cTwoPatModelSearchTool.m_PatternSearchTool;
		m_PatternSearchTool2 = cTwoPatModelSearchTool.m_PatternSearchTool2;
	}
}

CTwoPatModelSearchTool& CTwoPatModelSearchTool::operator=(CTwoPatModelSearchTool &cTwoPatModelSearchTool)
{
	if (this == &cTwoPatModelSearchTool)
	{
		return *this;	
	}

	if (cTwoPatModelSearchTool.GetSearchToolType() != eTwoPatModelSearchTool)
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
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cTwoPatModelSearchTool.m_CamHeight;
	m_CamWidth								= cTwoPatModelSearchTool.m_CamWidth;

	m_TrainDataParam						= cTwoPatModelSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cTwoPatModelSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cTwoPatModelSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cTwoPatModelSearchTool.m_SearchGuiParam;
	m_TrainResult							= cTwoPatModelSearchTool.m_TrainResult;

	m_ImageTrain							= cTwoPatModelSearchTool.m_ImageTrain;
	m_ImageMask								= cTwoPatModelSearchTool.m_ImageMask;
	m_ImageMask2								= cTwoPatModelSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= cTwoPatModelSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cTwoPatModelSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cTwoPatModelSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cTwoPatModelSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cTwoPatModelSearchTool.UpdateSearchGuiData(true);
	cTwoPatModelSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cTwoPatModelSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cTwoPatModelSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cTwoPatModelSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cTwoPatModelSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cTwoPatModelSearchTool.m_tmpImageTrain);
	SetMaskImage(cTwoPatModelSearchTool.m_tmpImageMask);
	SetMaskImage2(cTwoPatModelSearchTool.m_tmpImageMask2);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = cTwoPatModelSearchTool.m_PatternSearchTool;
		m_PatternSearchTool2 = cTwoPatModelSearchTool.m_PatternSearchTool2;
	}

	return *this;	
}



CTwoPatModelSearchTool::~CTwoPatModelSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CTwoPatModelSearchTool::GetSearchToolType()
{
	return eTwoPatModelSearchTool;
}										   

// 设置显示控件指针		
bool CTwoPatModelSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CTwoPatModelSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CTwoPatModelSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CTwoPatModelSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CTwoPatModelSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CTwoPatModelSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CTwoPatModelSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CTwoPatModelSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	

		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect2, m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect2, m_strKeyText+_T("_SearchRect2"));

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新搜索界面显示									
bool CTwoPatModelSearchTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;
	
	for (i=0; i<m_pGuiSearchRectArray.size(); i++)
	{
		m_pGuiSearchRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible);
	}
	
	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bPointVisible);
	}

	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}
	
	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		int i=0;
		
		for (i=0; i<m_pGuiSearchRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchRectArray[i], m_pGuiSearchRectArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
		}	

		for (i=0; i<m_pGuiSearchLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新界面显示
bool CTwoPatModelSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 设置训练Data参数	
bool CTwoPatModelSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CTwoPatModelSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CTwoPatModelSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	*((CTwoPatModelSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CTwoPatModelSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CTwoPatModelSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CTwoPatModelSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CTwoPatModelSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CTwoPatModelSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CTwoPatModelSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CTwoPatModelSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CTwoPatModelSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像									
bool CTwoPatModelSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask2 = ImageMask;
	
	return true;
}

// 获取训练原始图像								
bool CTwoPatModelSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask2;
	
	return true;
}

// 执行训练											
bool CTwoPatModelSearchTool::Train()
{
	m_bTrainOK = false;
	m_nResultErrNumber=0;
	// 获取Gui控件数据
	UpdateTrainGuiData(true);

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

	// 设置训练参数
	if (!m_PatternSearchTool2.SetAccuracy(m_tmpTrainDataParam.m_Accuracy2))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	
	if (!m_PatternSearchTool2.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale2))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	
	// 必须在训练时设置角度
	if (m_tmpTrainDataParam.m_dAngleLow2 == m_tmpTrainDataParam.m_dAngleHigh2) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool2.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2)))
		{
			m_nResultErrNumber = -2;
			return false;
		}
	}
	if (!m_PatternSearchTool2.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2), scDegree(m_tmpTrainDataParam.m_dAngleHigh2), scDegree(360)))
	{
		m_nResultErrNumber = -2;
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
	cpImage tmpImageMask2;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_tmpImageMask.Width(),m_tmpImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask,tmpImageMask);

		tmpImageMask2.CreateImageBuffer(m_tmpImageMask2.Width(),m_tmpImageMask2.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask2,tmpImageMask2);
	}		

	bool bRet = false;
	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
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
	}
	else
	{
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
	}

	if (!bRet)
	{
		m_nResultErrNumber=-1; /// 区域1训练失败
		return bRet;
	}




	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable2)
	{
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool2.Train(tmpImage, tmpImageMask2, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		else
		{
			bRet = m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpImageMask2, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
	}
	else
	{
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool2.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		else
		{
			bRet = m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
	}

	if (!bRet)
	{
	  m_nResultErrNumber=-2; /// 区域2训练失败
		return bRet;
	}

	m_bTrainOK = bRet;

	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		m_ImageMask = m_tmpImageMask;
	}

	if (m_tmpTrainDataParam.m_bMaskEnable2)
	{
		m_ImageMask2 = m_tmpImageMask2;
	}

	sc2Vector cModelOrigin;
	m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
	m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
	m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

	m_PatternSearchTool2.GetPatternOrigin(cModelOrigin);
	m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
	m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();

	m_ImageTrain     = m_tmpImageTrain;
	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam  = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;

	return m_bTrainOK;
}

// 获取训练结果图像														
bool CTwoPatModelSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果图像														
bool CTwoPatModelSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool2.GetPatternImage(m_ImageTrainResult2);
	if (!bRet)
	{
		return false;
	}
	
	ImageTrainResult = m_ImageTrainResult2;
	
	return true;
}	

// 获取训练结果
bool CTwoPatModelSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	*((CTwoPatModelSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}							

// 是否训练成功
bool CTwoPatModelSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CTwoPatModelSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CTwoPatModelSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CTwoPatModelSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}

	*((CTwoPatModelSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CTwoPatModelSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CTwoPatModelSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CTwoPatModelSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eTwoPatModelSearchTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CTwoPatModelSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CTwoPatModelSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;///训练未成功
		return false;
	}	
	m_nResultErrNumber=0;
	// 清空
	ClearResult();

	m_strShowInspectInfo= _T("");
	m_tmpSearchDataParam.m_nSearchNum = 1;
	BOOL bSizeCheck = TRUE; // 检查成功
	
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

	// 设置混淆接受阈值
	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}


	// 设置搜索个数
	if (!m_PatternSearchTool2.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	
	// 设置接收阈值
	if (!m_PatternSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	// 设置混淆接受阈值
	if (!m_PatternSearchTool2.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber = -2;
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
		if (nResultNum <= 0)
		{
			m_nResultErrNumber=-1;
			return false;
		}
	}
	else
	{
		m_nResultErrNumber=-1;//区域定位1失败
		return bRet;
	}





	if(bIsProcessImage)
	{
		bRet = m_PatternSearchTool2.Execute(tmpImage, m_TrainGuiParam.m_SearchRect2);
	}
	else
	{
		bRet = m_PatternSearchTool2.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect2);
	}

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool2.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber=-2;//区域定位2失败
			return false;
		}
	}
	else
	{
		m_nResultErrNumber=-2;//区域定位2失败
		return bRet;
	}

	// 求两个参考点
	sc2Vector searchPoint1;
	sc2Vector searchPoint2;

	scAffineRect affRect1;

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -1;
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM && i < m_PatternSearchTool2.GetResultNum(); i++)
		{
			scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);

			affRect1 = tmpPatternResult.MatchRegion();
			// 求变换矩阵
			sc2Vector pos = tmpPatternResult.GetPosition();
			searchPoint1 = pos;

			CString	strTmpKey;
			strTmpKey.Format(_T("工具1%s_ResultRect%d"), m_strKeyText, i);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
			pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchRectArray.push_back(pTempAffineRect);

			strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross+i*2;
			pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			m_pGuiSearchCrossArray.push_back(pTempCross);

// 			strTmpKey.Format(_T("工具1%s_ResultSearchCross%d"), m_strKeyText, i);
// 			pTempCross = m_GuiSearchCross+i*2+1;
// 			pTempCross->SetCenterExtent(searchPoint1, sc2Vector(40,40));
// 			pTempCross->SetLineColor(RGB(0, 0, 255));
// 			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempCross->SetTipTextEnabled(true);
// 			pTempCross->SetTipText(strTmpKey);
// 			m_pGuiSearchCrossArray.push_back(pTempCross);
		}	
	}
	else
	{
		return bRet;
	}



	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool2.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -2;
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM && i<m_PatternSearchTool.GetResultNum(); i++)
		{
			scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool2.GetResult(i);
			sc2Vector pos = tmpPatternResult.GetPosition();
			searchPoint2 = pos;

			CSearchResult searchResult;
			searchResult.m_dPosX = (searchPoint1.GetX() + searchPoint2.GetX()) / 2.0;
			searchResult.m_dPosY = (searchPoint1.GetY() + searchPoint2.GetY()) / 2.0;
			searchResult.m_dAngle = 0;

			searchResult.m_vdAuxiliaryPosX.at(0) = searchPoint1.GetX();
			searchResult.m_vdAuxiliaryPosY.at(0) = searchPoint1.GetY();

			searchResult.m_vdAuxiliaryPosX.at(1) = searchPoint2.GetX();
			searchResult.m_vdAuxiliaryPosY.at(1) = searchPoint2.GetY();

			// 点1到点2的距离
			double dDistance12 = 0.0;
			sfDistancePoint2Point(searchPoint1,searchPoint2,dDistance12);
			dDistance12 = dDistance12*m_tmpSearchDataParam.m_dPixelRatio;

			if(m_tmpSearchDataParam.m_bEnableSizeCheck)
			{
				if(m_tmpSearchDataParam.m_bEnableMax && dDistance12>m_tmpSearchDataParam.m_dDistanceMax)
				{
					m_nResultErrNumber = -3; // 尺寸检查失败：超过最大值;
					bSizeCheck = FALSE;
				}

				if(m_tmpSearchDataParam.m_bEnableMin && dDistance12<m_tmpSearchDataParam.m_dDistanceMin)
				{
					m_nResultErrNumber = -4; // 尺寸检查失败：低于最小值;
					bSizeCheck = FALSE;
				}
			}

			
			CResultSearchRect resRect;
			resRect.m_searchRect = 	affRect1;
			searchResult.m_vTrainSearchRect.push_back(resRect);

			resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
			searchResult.m_vTrainSearchRect.push_back(resRect);

			CResultLine line;
			line.m_vLineX[0] = searchPoint1.GetX();
			line.m_vLineY[0] = searchPoint1.GetY();
			line.m_vLineX[1] = searchPoint2.GetX();
			line.m_vLineY[1] = searchPoint2.GetY();
			searchResult.m_vLineSegSearchResult.push_back(line);

			m_SearchResultArray.push_back(searchResult);
			
			CString	strTmpKey;
			strTmpKey.Format(_T("工具2%s_ResultRect%d"), m_strKeyText, i);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect2+i;
			pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchRectArray.push_back(pTempAffineRect);
			
			strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross2+i*2;
			pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			m_pGuiSearchCrossArray.push_back(pTempCross);

// 			strTmpKey.Format(_T("工具2%s_ResultSearchCross%d"), m_strKeyText, i);
// 			pTempCross = m_GuiSearchCross2+i*2+1;
// 			pTempCross->SetCenterExtent(searchPoint2, sc2Vector(40,40));
// 			pTempCross->SetLineColor(RGB(0, 0, 255));
// 			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempCross->SetTipTextEnabled(true);
// 			pTempCross->SetTipText(strTmpKey);			
// 			m_pGuiSearchCrossArray.push_back(pTempCross);

			scLineSeg lineSeg(searchPoint1, searchPoint2);
			int L = 2;
			strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, L, i);
			scGuiLineSeg *pTemLine = m_GuiSearchLineSeg2+i;
			pTemLine->SetLineSeg(lineSeg);
			pTemLine->SetLineColor(RGB(0, 0, 255));
			pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemLine->SetTipTextEnabled(true);
			pTemLine->SetTipText(strTmpKey);
			m_pGuiSearchLineArray.push_back(pTemLine);

			strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
			scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
			pTempLineCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), sc2Vector(40,40));
			pTempLineCross->SetLineColor(RGB(255, 0, 0));
			pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempLineCross->SetTipTextEnabled(true);
			pTempLineCross->SetTipText(strTmpKey);
			
			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
			pTempLineCross->SetLabel(st);
			pTempLineCross->SetLabelVisible(TRUE);
			m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
		}	
		
		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		return bRet;
	}

	return bRet;
}

// 获取搜索结果个数										
int CTwoPatModelSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CTwoPatModelSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像
bool CTwoPatModelSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eTwoPatModelSearchTool)
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
bool CTwoPatModelSearchTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或PatCornerSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CTwoPatModelSearchTrainDataParam tmpTrainDataParam;
	CTwoPatModelSearchTrainGuiParam tmpTrainGuiParam;
	CTwoPatModelSearchSearchDataParam tmpSearchDataParam;
	CTwoPatModelSearchSearchGuiParam tmpSearchGuiParam;	
	CTwoPatModelSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eTwoPatModelSearchTool)
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

	// coarse scale
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale2 = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy2 = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable2 = _ttoi(cXMLConfigurator.GetElemData());

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 
// 	//////////////TrainGuiParam//////////////


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

	// Train rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect2"));
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
	tmpTrainGuiParam.m_TrainRect2.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_TrainRect2.SetSize(vPos);
	cXMLConfigurator.LeaveElem();
	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchRect2"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect2.GetOrigin().GetX());
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
	tmpTrainGuiParam.m_SearchRect2.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_SearchRect2.SetSize(vPos);
	cXMLConfigurator.LeaveElem();
	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint2"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint2.GetX());
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
	tmpTrainGuiParam.m_ModelPoint2 = vPos;
	cXMLConfigurator.LeaveElem();

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
		return false;
	}
	tmpSearchDataParam.m_dConfusionThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	bRet = cXMLConfigurator.FindElem(_T("EnableSizeCheck"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableSizeCheck = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dPixelRatio"));
	if (bRet)
	{
		tmpSearchDataParam.m_dPixelRatio = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMax"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMax = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMin"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMin = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMax"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMax = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMin"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMin = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中 

// 	//////////////SearchGuiParam//////////////

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


	bRet = cXMLConfigurator.FindElem(_T("ModelX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("ModelY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dModelX1 = vPos.GetX();
	tmpTrainResult.m_dModelY1 = vPos.GetY();

	bRet = cXMLConfigurator.FindElem(_T("ModelX2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("ModelY2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dModelX2 = vPos.GetX();
	tmpTrainResult.m_dModelY2 = vPos.GetY();

	bRet = cXMLConfigurator.FindElem(_T("ModelX3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("ModelY3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dModelX3 = vPos.GetX();
	tmpTrainResult.m_dModelY3 = vPos.GetY();

	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	cpImage tmpImageMask;
	cpImage tmpImageMask2;
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

	if (tmpTrainDataParam.m_bMaskEnable2)
	{
		hStatus = cDIB.Init(strDir + _T("Mask2.bmp"));
		if (!IsSuccess(hStatus))
		{
			tmpTrainDataParam.m_bMaskEnable2 = false;
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask2);
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_bMaskEnable2 = false;
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

	scPatternSearchRot tmpPatternSearchTool2;
	bRet = tmpPatternSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.pts"));
	if (!bRet)
	{
		return false;
	}
	
	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	if (tmpTrainDataParam.m_bMaskEnable2)
	{
		m_tmpImageMask2 = tmpImageMask2;
	}
	m_PatternSearchTool = tmpPatternSearchTool;
	m_PatternSearchTool2 = tmpPatternSearchTool2;
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
	m_ImageMask2			= tmpImageMask2;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CTwoPatModelSearchTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或PatCornerSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	
	////////////CameraParam//////////
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


	// CoarseScale
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CoarseScale2);
	cXMLConfigurator.AddElem(_T("CoarseScale2"), strInfo);
	// Accuracy
	strInfo.Format(_T("%d"), m_TrainDataParam.m_Accuracy2);
	cXMLConfigurator.AddElem(_T("Accuracy2"), strInfo);
	// Angle Low
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleLow2);
	cXMLConfigurator.AddElem(_T("AngleLow2"), strInfo);
	// Angle High
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleHigh2);
	cXMLConfigurator.AddElem(_T("AngleHigh2"), strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable2);
	cXMLConfigurator.AddElem(_T("MaskEnable2"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
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
	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	
	// train rect
	cXMLConfigurator.AddElem(_T("TrainRect2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// search rect
	cXMLConfigurator.AddElem(_T("SearchRect2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect2.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect2.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect2.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect2.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();

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


	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableSizeCheck);
	cXMLConfigurator.AddElem(_T("EnableSizeCheck"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dPixelRatio);
	cXMLConfigurator.AddElem(_T("dPixelRatio"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMax);
	cXMLConfigurator.AddElem(_T("bEnableMax"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMin);
	cXMLConfigurator.AddElem(_T("bEnableMin"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMax);
	cXMLConfigurator.AddElem(_T("dDistanceMax"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMin);
	cXMLConfigurator.AddElem(_T("dDistanceMin"), strInfo);

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中

// 	///////////////SearchGuiParam//////////////
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX1);
	cXMLConfigurator.AddElem(_T("ModelX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY1);
	cXMLConfigurator.AddElem(_T("ModelY1"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
	cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
	cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX3);
	cXMLConfigurator.AddElem(_T("ModelX3"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY3);
	cXMLConfigurator.AddElem(_T("ModelY3"), strInfo);

	cXMLConfigurator.LeaveElem();

	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	//////////////////////////存储模板图像文//////////////////////////
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

	if (m_TrainDataParam.m_bMaskEnable2 && m_ImageMask2.IsValid())
	{
		hStatus = cDIB.Init(m_ImageMask2);
		if (!IsSuccess(hStatus))
		{
			return false;
		}
		hStatus = cDIB.Write(strDir + _T("Mask2.bmp"));
		if (!IsSuccess(hStatus))
		{
			return false;
		}
	}

	//////////////////////////存储模板pts文件//////////////////////////
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	bRet = m_PatternSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.pts"));
	if (!bRet)
	{
		return false;
	}
	
	return true;
}	
	
// 更新训练控件数据信息	
bool CTwoPatModelSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_TrainRect2			= m_GuiTrainRect2.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect2			= m_GuiSearchRect2.GetRect();
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


		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变
		m_GuiTrainRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetRect(m_tmpTrainGuiParam.m_TrainRect2);
		
		m_GuiSearchRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect2.SetRect(m_tmpTrainGuiParam.m_SearchRect2);
	}

	return true;
}

bool CTwoPatModelSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchRectArray.size(); j++)
		{
			m_pGuiSearchRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
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
bool CTwoPatModelSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CTwoPatModelSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CTwoPatModelSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	return true;
}

bool CTwoPatModelSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;
	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainRect2.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiSearchRect2.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//参考点2
	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	
	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CTwoPatModelSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint3"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect2"));	
	}
	
	return true;
}

bool CTwoPatModelSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
			
		for (j=0; j<m_pGuiSearchRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchRectArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}
		
		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

	}

	return true;
}

// 获取训练模板原始图像
bool CTwoPatModelSearchTool::GetPatternImage(cpImage &ImagePattern)
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
	return true;
}	

// 获取训练模板原始图像
bool CTwoPatModelSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
	/*bool bRet = m_PatternSearchTool2.GetPatternImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}
	
	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if(cpImage::statusSuccess == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool2.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}	

// 恢复训练参数
bool CTwoPatModelSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CTwoPatModelSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CTwoPatModelSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area1"));
		m_GuiModelPoint.SetLabel(_T("Reference Point1"));
		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
}	
CString CTwoPatModelSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case 0:
				{
					strErrInfo = m_strShowInspectInfo;
				}
				break;
			case -1:			//区域定位1失败				
				strErrInfo += _T("Pattern search 1 failed");
				break;
			case -2:			//区域定位2失败				
				strErrInfo += _T("Pattern search 2 failed");
				break;
			case -99:			//训练未成功				
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
			case 0:
				{
					strErrInfo = m_strShowInspectInfo;
				}
				break;
			case -1:							
				strErrInfo += _T("区域定位1失败");
				break;
			case -2:							
				strErrInfo += _T("区域定位2失败");
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
			case 0:
				{
					strErrInfo = m_strShowInspectInfo;
				}
				break;
			case -1:							
				strErrInfo += _T("区域定位1失败");
				break;
			case -2:							
				strErrInfo += _T("区域定位2失败");
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