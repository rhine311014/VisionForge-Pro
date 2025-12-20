// PatternBlobSearchTool.cpp: implementation of the CPatternBlobSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatternBlobSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"



CPatternBlobSearchTool::CPatternBlobSearchTool(void)
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
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
		

	{
		CString strLabel;
		strLabel.Format("Blob工具");
		m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew(200, 240, 100, 100, 0, 0);
		m_GuiTrainBlobAffineRect.SetLabel(strLabel);
		m_GuiTrainBlobAffineRect.SetLabelVisible(TRUE);
	}
	m_GuiSearchRect.SetLabelVisible(TRUE);
	//m_GuiModelPoint.SetCenterXYWidthHeight(250, 200);
	m_GuiModelPoint.SetLabel(_T("参考点"));
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像


	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();


	
	m_tmpTrainGuiParam.m_BlobAffineRect= m_GuiTrainBlobAffineRect.GetAffineRect();
	


	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;
	m_tmpSearchDataParam.m_bCoordSortEnable		= false;
	m_tmpSearchDataParam.m_nCoordSortMethod		= eCoordXOnly;
	m_tmpSearchDataParam.m_nCoordSortOrderX		= eAscending;
	m_tmpSearchDataParam.m_nCoordSortOrderY		= eAscending;;
	m_tmpSearchDataParam.m_dCoordSortGap		= 64;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CPatternBlobSearchTool::CPatternBlobSearchTool(CPatternBlobSearchTool &cPatternBlobSearchTool)
{
	if (cPatternBlobSearchTool.GetSearchToolType() != ePatternBlobSearchTool)
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
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(250, 240, 50, 50);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(500, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
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



	{
		CString strLabel;
		strLabel.Format("Blob工具");
		m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew(100+50, 100+50, 50, 50, 0, 0);
		m_GuiTrainBlobAffineRect.SetLabel(strLabel);
		m_GuiTrainBlobAffineRect.SetLabelVisible(TRUE);
	}
	m_GuiSearchRect.SetLabelVisible(TRUE);
	//m_GuiModelPoint.SetCenterXYWidthHeight(250, 200);
	m_GuiModelPoint.SetLabel(_T("参考点"));

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cPatternBlobSearchTool.m_CamHeight;
	m_CamWidth								= cPatternBlobSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternBlobSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternBlobSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternBlobSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternBlobSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternBlobSearchTool.m_TrainResult;
	m_ImageTrain							= cPatternBlobSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternBlobSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternBlobSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternBlobSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternBlobSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternBlobSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternBlobSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cPatternBlobSearchTool.UpdateSearchGuiData(true);
	cPatternBlobSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternBlobSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternBlobSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternBlobSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternBlobSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternBlobSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternBlobSearchTool.m_tmpImageMask);

	if (cPatternBlobSearchTool.m_tmpTrainDataParam.m_bPatternSearchEnable)
	{
		if (m_bTrainOK)
		{
			m_PatternSearchTool = cPatternBlobSearchTool.m_PatternSearchTool;
		}
	}
	
}

CPatternBlobSearchTool& CPatternBlobSearchTool::operator=(CPatternBlobSearchTool &cPatternBlobSearchTool)
{
	if (this == &cPatternBlobSearchTool)
	{
		return *this;	
	}

	if (cPatternBlobSearchTool.GetSearchToolType() != ePatternBlobSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(250, 200, 50, 50);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
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

	m_GuiModelPoint.SetLabelVisible(TRUE);

	{
		CString strLabel;
		strLabel.Format("Blob工具");
		m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew(100+50, 100+50, 50, 50, 0, 0);
		m_GuiTrainBlobAffineRect.SetLabel(strLabel);
		m_GuiTrainBlobAffineRect.SetLabelVisible(TRUE);
	}
	m_GuiSearchRect.SetLabelVisible(TRUE);
	//m_GuiModelPoint.SetCenterXYWidthHeight(250, 200);
	m_GuiModelPoint.SetLabel(_T("参考点"));

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cPatternBlobSearchTool.m_CamHeight;
	m_CamWidth								= cPatternBlobSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternBlobSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternBlobSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternBlobSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternBlobSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternBlobSearchTool.m_TrainResult;

	m_ImageTrain							= cPatternBlobSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternBlobSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternBlobSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternBlobSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternBlobSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternBlobSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternBlobSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternBlobSearchTool.UpdateSearchGuiData(true);
	cPatternBlobSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternBlobSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternBlobSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternBlobSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternBlobSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternBlobSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternBlobSearchTool.m_tmpImageMask);

	if (cPatternBlobSearchTool.m_tmpTrainDataParam.m_bPatternSearchEnable)
	{
		if (m_bTrainOK)
		{
			m_PatternSearchTool = cPatternBlobSearchTool.m_PatternSearchTool;	
		}
	}
	

 	return *this;	
}

CPatternBlobSearchTool::~CPatternBlobSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CPatternBlobSearchTool::GetSearchToolType()
{
	return ePatternBlobSearchTool;
}

// 设置显示控件指针		
bool CPatternBlobSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
	m_bTrainResultGuiShow  = FALSE;

	// 设置Gui控件
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	// 显示控件
	UpdateTrainGuiDisplay();
	//UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 获取显示控件指针										
scGuiDisplay* CPatternBlobSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternBlobSearchTool::SetGuiKeyText(CString strGuiKeyText)									
{
	if ((strGuiKeyText == _T("")) || (strGuiKeyText == m_strKeyText))
	{
		return false;
	}

	ClearAllGui();

	m_strKeyText = strGuiKeyText;

	UpdateTrainGuiDisplay();
	//UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 获取Gui图形的关键字
bool CPatternBlobSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternBlobSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternBlobSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CPatternBlobSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CPatternBlobSearchTool::UpdateTrainGuiDisplay()
{	
	if (m_tmpTrainDataParam.m_bPatternSearchEnable)
	{
		m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
		m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
		m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);	
	}
	else
	{
		m_GuiTrainRect.SetVisible(FALSE);
		m_GuiModelPoint.SetVisible(FALSE);
		m_GuiSearchRect.SetVisible(FALSE);	
	}
	

	m_GuiTrainBlobAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bBlobVisible);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		if (m_tmpTrainDataParam.m_bPatternSearchEnable)
		{
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
			m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
			m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		}

		CString str;
		str.Format("_BlobRegion");
		m_pGuiInteractiveContainer->AddItem(&(m_GuiTrainBlobAffineRect), m_strKeyText+str);

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

bool CPatternBlobSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultBlob.SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		//m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		CString str;
		str.Format("_TrainResultBlob");
		m_pGuiStaticContainer->AddItem(&(m_GuiTrainResultBlob), m_strKeyText+str);

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CPatternBlobSearchTool::UpdateSearchResultGuiDisplay()							
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

	for (i=0;i<m_pGuiSearchBlobArray.size();i++)
	{
		m_pGuiSearchBlobArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bBlobVisible);
	}

	m_GuiContour.SetVisible(m_bSearchResultGuiShow);


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

		for (i=0;i<m_pGuiSearchBlobArray.size();i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchBlobArray[i], m_pGuiSearchBlobArray[i]->GetTipText());
		}


		m_pGuiStaticContainer->AddItem(&m_GuiContour,"blob result");
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}


	return true;
}

// 更新界面显示
bool CPatternBlobSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternBlobSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CPatternBlobSearchTrainDataParam*)pTrainDataParam);
	m_TrainDataParam = m_tmpTrainDataParam;

	return true;
}

// 获取训练Data参数									
bool CPatternBlobSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	*((CPatternBlobSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CPatternBlobSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CPatternBlobSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CPatternBlobSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CPatternBlobSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CPatternBlobSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternBlobSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	if (m_tmpTrainDataParam.m_bPatternSearchEnable)
	{	
		ImageTrain = m_tmpImageTrain;
	}
	else
	{
		ImageTrain = m_ImageTrain;
	}


	return true;
}

// 设置训练原始图像									
bool CPatternBlobSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternBlobSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CPatternBlobSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);
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
	if (m_tmpTrainDataParam.m_bPatternSearchEnable)
	{

		// 设置训练参数
		if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		// 必须在训练时设置角度
		if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
		{
			if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
			{
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}
		}
		else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
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
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();


			m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
			m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
			m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
			m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lBlobAreaMinThre);
			bool bSuccess = false;
			if(bIsProcessImage)
			{
				bSuccess = m_BlobTool.Execute(tmpImage,&(m_tmpTrainGuiParam.m_BlobAffineRect));
			}
			else
			{
				bSuccess = m_BlobTool.Execute(m_tmpImageTrain,&(m_tmpTrainGuiParam.m_BlobAffineRect));
			}
			
			if (!bSuccess)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}

			scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
			if (pBSD == NULL)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}

			pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
			scBlob * const *pBlobB = pBSD->allBlobs(false);

			int nResultBlobNum = pBSD->numBlobs(true); 

			if (pBSD->numBlobs() < 1)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}
			else
			{
				m_GuiContour.SetLineColor(RGB(0,255,0));
			}
	
			m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
			m_GuiContour.SetTipTextEnabled(TRUE);


			m_bTrainOK       = true;
			m_ImageTrain     = m_tmpImageTrain;
			m_ImageMask      = m_tmpImageMask;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam	 = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;

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
				m_nResultErrNumber=-1;       //区域定位失败
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
			m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
			m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
			m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lBlobAreaMinThre);
			bool bSuccess = false;
			if(bIsProcessImage)
			{
				bSuccess = m_BlobTool.Execute(tmpImage,&(m_tmpTrainGuiParam.m_BlobAffineRect));
			}
			else
			{
				bSuccess = m_BlobTool.Execute(m_tmpImageTrain,&(m_tmpTrainGuiParam.m_BlobAffineRect));
			}
			
			if (!bSuccess)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}

			scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
			if (pBSD == NULL)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}

			pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
			scBlob * const *pBlobB = pBSD->allBlobs(false);

			int nResultBlobNum = pBSD->numBlobs(true); 

			if (pBSD->numBlobs() < 1)
			{	
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}
			else
			{
				m_GuiContour.SetLineColor(RGB(0,255,0));
			}

			m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
			m_GuiContour.SetTipTextEnabled(TRUE);

// 			std::vector<sc2Vector> vResultArray;
// 			for (int j=1;j<pBSD->numBlobs()+1;j++ )
// 			{
// 				const scBlob* pBlob = pBSD->getBlob(j);
// 
//                 sc2Vector vResult;
// 				vResult.SetX(pBlob->centerMassX);
// 				vResult.SetY(pBlob->centerMassY);
// 				vResultArray.push_back(vResult);
// 
// 			}
// 
// 			if (m_tmpSearchDataParam.m_bCoordSortEnable)
// 			{
// 				SortByCoordXY(vResultArray,m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
// 					m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
// 			}			
// 
// 			for (int j=1;j<pBSD->numBlobs()+1;j++)
// 			{
// 				const scBlob* pBlob = pBSD->getBlob(j);
// 				CSearchResult searchResult;
// 				searchResult.m_dPosX = vResultArray.at(j).GetX();
// 				searchResult.m_dPosY = vResultArray.at(j).GetY();
// 				searchResult.m_dAngle = pBlob->angle;
// 				m_SearchResultArray.push_back(searchResult);
// 			}

			m_bTrainOK       = true;
			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam	 = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;

			return true;

		}
	}
	else
	{
		m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
		m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
		m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
		m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lBlobAreaMinThre);
		bool bSuccess = false;
		if(bIsProcessImage)
		{
			bSuccess = m_BlobTool.Execute(tmpImage,&(m_tmpTrainGuiParam.m_BlobAffineRect));
		}
		else
		{
			bSuccess = m_BlobTool.Execute(m_tmpImageTrain,&(m_tmpTrainGuiParam.m_BlobAffineRect));
		}
		
		if (!bSuccess)
		{
			m_nResultErrNumber=-2;       //Blob失败
			return false;
		}

		scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
		if (pBSD == NULL)
		{
			m_nResultErrNumber=-2;       //Blob失败
			return false;
		}

		pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
		scBlob * const *pBlobB = pBSD->allBlobs(false);

		int nResultBlobNum = pBSD->numBlobs(true); 

		if (pBSD->numBlobs() < 1)
		{
			m_nResultErrNumber=-2;       //Blob失败
			return false;
		}
		else
		{
			m_GuiContour.SetLineColor(RGB(0,255,0));
		}

		m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
		m_GuiContour.SetTipTextEnabled(TRUE);

// 		std::vector<sc2Vector> vResultArray;
// 		for (int j=1;j<pBSD->numBlobs()+1;j++ )
// 		{
// 			const scBlob* pBlob = pBSD->getBlob(j);
// 
// 			sc2Vector vResult;
// 			vResult.SetX(pBlob->centerMassX);
// 			vResult.SetY(pBlob->centerMassY);
// 			vResultArray.push_back(vResult);
// 
// 		}
// 
// 		if (m_tmpSearchDataParam.m_bCoordSortEnable)
// 		{
// 			SortByCoordXY(vResultArray,m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
// 				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
// 		}			
// 
// 		for (int j=1;j<pBSD->numBlobs()+1;j++)
// 		{
// 			const scBlob* pBlob = pBSD->getBlob(j);
// 			CSearchResult searchResult;
// 			searchResult.m_dPosX = vResultArray.at(j).GetX();
// 			searchResult.m_dPosY = vResultArray.at(j).GetY();
// 			searchResult.m_dAngle = pBlob->angle;
// 			m_SearchResultArray.push_back(searchResult);
// 		}


		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;

		return true;
     }


	return false;
}

// 获取训练结果图像														
bool CPatternBlobSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CPatternBlobSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	*((CPatternBlobSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CPatternBlobSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CPatternBlobSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternBlobSearchSearchDataParam*)pSearchDataParam);
    m_SearchDataParam = m_tmpSearchDataParam;
	return true;
}

// 获取搜索Data参数				
bool CPatternBlobSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	*((CPatternBlobSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternBlobSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CPatternBlobSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CPatternBlobSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternBlobSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CPatternBlobSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CPatternBlobSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	bool bRet= false;
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

	if (m_tmpSearchDataParam.m_bPatternSearchEnable)
	{
		// 设置搜索个数
		if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		// 设置接收阈值
		if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}
		if(bIsProcessImage)
		{
			// 执行搜索
			bRet = m_PatternSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);
		}
		else
		{
			// 执行搜索
			bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
		}
		if (bRet)
		{
			// 获取结果
			int nResultNum = m_PatternSearchTool.GetResultNum();
			if (nResultNum <= 0)
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

				CString	strTmpKey;
				bool bBlobOK = true;
				bool bBlobStatus = true;
				scAffineRect affBlobRect;


				{
					double dTransCenterX, dTransCenterY;
					double dOriCenterX, dOriCenterY;
					dOriCenterX = m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetX();
					dOriCenterY = m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetY();
					dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
					dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;

					scAffineRect affRect;
					affRect.SetCenter(sc2Vector(dTransCenterX,dTransCenterY));
					affRect.SetXLength(m_TrainGuiParam.m_BlobAffineRect.GetXLength());
					affRect.SetYLength(m_TrainGuiParam.m_BlobAffineRect.GetYLength());
					affRect.SetXRotation(m_TrainGuiParam.m_BlobAffineRect.GetXRotation() + scRadian(dRotation));
					affRect.SetSkew(m_TrainGuiParam.m_BlobAffineRect.GetSkew());

					m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
					m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
					m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
					m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lBlobAreaMinThre);
					m_BlobTool.EnableComputeInertia(true);
					if(bIsProcessImage)
					{
						bRet = m_BlobTool.Execute(tmpImage, &affRect);
					}
					else
					{
						bRet = m_BlobTool.Execute(ImageSearch, &affRect);
					}
					if (bRet)
					{						

						scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
						if (pBSD == NULL)
						{
							m_nResultErrNumber=-2;       //Blob失败
							return false;
						}

						pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
						scBlob * const *pBlobB = pBSD->allBlobs(false);

						int nResultBlobNum = pBSD->numBlobs(true); 

						if (pBSD->numBlobs() < 1)
						{		
							m_nResultErrNumber=-2;       //Blob失败
							return false;
						}
						else
						{
							m_GuiContour.SetLineColor(RGB(0,255,0));
						}

						m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
						m_GuiContour.SetTipTextEnabled(TRUE);

						std::vector<sc2Vector> vResultArray;
						
						for (int j=1;j<pBSD->numBlobs()+1;j++ )
						{
							const scBlob* pBlob = pBSD->getBlob(j);

							sc2Vector vResult;
							vResult.SetX(pBlob->centerMassX);
							vResult.SetY(pBlob->centerMassY);
							vResultArray.push_back(vResult);

						}

						if (m_tmpSearchDataParam.m_bCoordSortEnable)
						{
							sfSortByCoordXY(vResultArray,m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
								m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
						}			

						for (int j=1;j<pBSD->numBlobs()+1;j++)
						{
							
							CSearchResult searchResult;
							searchResult.m_dPosX = vResultArray.at(j-1).GetX();
							searchResult.m_dPosY = vResultArray.at(j-1).GetY();
							for (int k=1;k<pBSD->numBlobs()+1;k++)
							{
								const scBlob* pBlob = pBSD->getBlob(k);
								if ((pBlob->centerMassX ==searchResult.m_dPosX)&&(pBlob->centerMassY ==searchResult.m_dPosY))
								{
									searchResult.m_dAngle = pBlob->angle;
									if (j==1)
									{
										affBlobRect.SetCenter(sc2Vector(pBlob->imageBoundCenterX,pBlob->imageBoundCenterY));
										affBlobRect.SetXLength(pBlob->imageBoundWidth);
										affBlobRect.SetYLength(pBlob->imageBoundHeight);
										//affBlobRect.SetXRotation(scRadian(pBlob->angle));
									}
								}

							}

							// 设置粗定位矩形
							CResultSearchRect resRect;
							resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
							searchResult.m_vTrainSearchRect.push_back(resRect);

							// blob
							CResultSearchRect resultRect;
							resultRect.m_bFindRect = TRUE;
							resultRect.m_searchRect = affBlobRect;
							searchResult.m_vACFSearchRect.push_back(resultRect);
							
							m_SearchResultArray.push_back(searchResult);
						}


					}
					else
					{
						m_nResultErrNumber=-2;       //Blob失败
						return false;
					}


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
					pTempCross->SetCenterExtent(sc2Vector(tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY()), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore());
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);

					strTmpKey.Format(_T("%s_Result%d"), m_strKeyText, i);
					//scGuiAffineRect *pTemAffineRect = new scGuiAffineRect;
					scGuiAffineRect *pTemAffineRect = m_GuiSearchAffineRectBlob+i;
					pTemAffineRect->SetAffineRect(affBlobRect);
					pTemAffineRect->SetLineColor(RGB(0,0,255));
					pTemAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemAffineRect->SetTipTextEnabled(true);
					pTemAffineRect->SetTipText(strTmpKey);					
					st.Format(_T("Result%d"), i+1);
					pTemAffineRect->SetLabel(st);
					pTemAffineRect->SetLabelVisible(TRUE);
					m_pGuiSearchBlobArray.push_back(pTemAffineRect);

					m_SearchDataParam = m_tmpSearchDataParam;
					m_SearchGuiParam  = m_tmpSearchGuiParam;

				}
			}
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

	}
	else
	{
		scAffineRect affBlobRect;

		m_BlobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_lBlobGrayThre,m_tmpSearchDataParam.m_bBlobMode);
		m_BlobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
		m_BlobTool.ConnectivityCleanup(scBlobSceneDescription::ePrune);
		m_BlobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_lBlobAreaMinThre);
		m_BlobTool.EnableComputeInertia(true);
		if(bIsProcessImage)
		{
			bRet = m_BlobTool.Execute(tmpImage, &m_TrainGuiParam.m_BlobAffineRect);
		}
		else
		{
			bRet = m_BlobTool.Execute(ImageSearch, &m_TrainGuiParam.m_BlobAffineRect);
		}
		if (bRet)
		{						

			scBlobSceneDescription* pBSD = m_BlobTool.GetBSD();
			if (pBSD == NULL)
			{
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}

			pBSD->filter(eArea,m_tmpSearchDataParam.m_lBlobAreaMinThre,m_tmpSearchDataParam.m_lBlobAreaMaxThre,0);
			scBlob * const *pBlobB = pBSD->allBlobs(false);

			int nResultBlobNum = pBSD->numBlobs(true); 

			if (pBSD->numBlobs() < 1)
			{			
				m_nResultErrNumber=-2;       //Blob失败
				return false;
			}
			else
			{
				m_GuiContour.SetLineColor(RGB(0,255,0));
			}

			m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
			m_GuiContour.SetTipTextEnabled(TRUE);

			std::vector<sc2Vector> vResultArray;
			for (int j=1;j<pBSD->numBlobs()+1;j++ )
			{
				const scBlob* pBlob = pBSD->getBlob(j);

				sc2Vector vResult;
				vResult.SetX(pBlob->centerMassX);
				vResult.SetY(pBlob->centerMassY);
				vResultArray.push_back(vResult);

			}

			if (m_tmpSearchDataParam.m_bCoordSortEnable)
			{
				sfSortByCoordXY(vResultArray,m_tmpSearchDataParam.m_dCoordSortGap,m_tmpSearchDataParam.m_nCoordSortMethod,
					m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
			}			

			for (int j=1;j<pBSD->numBlobs()+1;j++)
			{
				const scBlob* pBlob = pBSD->getBlob(j);
				CSearchResult searchResult;
				searchResult.m_dPosX = vResultArray.at(j-1).GetX();
				searchResult.m_dPosY = vResultArray.at(j-1).GetY();
				for (int k=1;k<pBSD->numBlobs()+1;k++)
				{
					const scBlob* pBlob = pBSD->getBlob(k);
					if ((pBlob->centerMassX ==searchResult.m_dPosX)&&(pBlob->centerMassY ==searchResult.m_dPosY))
					{
						searchResult.m_dAngle = pBlob->angle;
						if (j==1)
						{
							affBlobRect.SetCenter(sc2Vector(pBlob->imageBoundCenterX,pBlob->imageBoundCenterY));
							affBlobRect.SetXLength(pBlob->imageBoundWidth);
							affBlobRect.SetYLength(pBlob->imageBoundHeight);
							//affBlobRect.SetXRotation(scRadian(pBlob->angle));
						}
					}

				}

				CResultSearchRect resultRect;
				resultRect.m_bFindRect = TRUE;
				resultRect.m_searchRect = affBlobRect;
				searchResult.m_vACFSearchRect.push_back(resultRect);

				m_SearchResultArray.push_back(searchResult);
			}

			CString strTmpKey,st;
			strTmpKey.Format(_T("%s_Result1"), m_strKeyText);
			//scGuiAffineRect *pTemAffineRect = new scGuiAffineRect;
			scGuiAffineRect *pTemAffineRect = m_GuiSearchAffineRectBlob;
			pTemAffineRect->SetAffineRect(affBlobRect);
			pTemAffineRect->SetLineColor(RGB(0,0,255));
			pTemAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemAffineRect->SetTipTextEnabled(true);
			pTemAffineRect->SetTipText(strTmpKey);					
			st.Format(_T("Result%d"), 1);
			pTemAffineRect->SetLabel(st);
			pTemAffineRect->SetLabelVisible(TRUE);
			m_pGuiSearchBlobArray.push_back(pTemAffineRect);


			return true;
		}
		else
		{
			m_nResultErrNumber=-2;       //Blob失败
			return false;
		}

    }

	return bRet;
}

// 获取搜索结果个数										
int CPatternBlobSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternBlobSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternBlobSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternBlobSearchTool)
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
bool CPatternBlobSearchTool::LoadModelFromFile(CString strDir)
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
	CPatternBlobSearchTrainDataParam tmpTrainDataParam;
	CPatternBlobSearchTrainGuiParam tmpTrainGuiParam;
	CPatternBlobSearchSearchDataParam tmpSearchDataParam;
	CPatternBlobSearchSearchGuiParam tmpSearchGuiParam;	
	CPatternBlobSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != ePatternBlobSearchTool)
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

	// PatternSearchEnable
	bRet = cXMLConfigurator.FindElem(_T("PatternSearchEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bPatternSearchEnable = _ttoi(cXMLConfigurator.GetElemData());
	




	// Train GUI rect
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


	{
		CString str;

		str.Format(_T("BlobAffineRect"));
		bRet = cXMLConfigurator.FindElem(str);
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
		tmpTrainGuiParam.m_BlobAffineRect.SetCenter(vPos);
		bRet = cXMLConfigurator.FindElem(_T("XLength"));
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_BlobAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("YLength"));
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_BlobAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("XRotation"));
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_BlobAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
		bRet = cXMLConfigurator.FindElem(_T("Skew"));
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_BlobAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));

		cXMLConfigurator.LeaveElem();
	}
	

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
	bRet = cXMLConfigurator.FindElem(_T("PatternSearchEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_bPatternSearchEnable= _ttoi(cXMLConfigurator.GetElemData());

	// BlobMode
	bRet = cXMLConfigurator.FindElem(_T("BlobMode"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_bBlobMode = _ttol(cXMLConfigurator.GetElemData());
	// BlobGrayThrehold
	bRet = cXMLConfigurator.FindElem(_T("BlobGrayThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_lBlobGrayThre = _ttol(cXMLConfigurator.GetElemData());
	// BlobAreaMinThre
	bRet = cXMLConfigurator.FindElem(_T("BlobAreaMinThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_lBlobAreaMinThre = _ttol(cXMLConfigurator.GetElemData());
	// BlobAreaMaxThre
	bRet = cXMLConfigurator.FindElem(_T("BlobAreaMaxThre"));
	if (!bRet)
	{	
		return false;	
	}
	tmpSearchDataParam.m_lBlobAreaMaxThre = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("CoordSortEnable"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bCoordSortEnable = false;
	}
	else
	{
		tmpSearchDataParam.m_bCoordSortEnable = _ttoi(cXMLConfigurator.GetElemData());;
	}

	bRet = cXMLConfigurator.FindElem(_T("CoordSortMethod"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortMethod = eCoordXOnly;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortMethod = (CoordSortMethod)_ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderX"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortOrderX = eAscending;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortOrderX = (SortOrder)_ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderY"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortOrderY = eAscending;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortOrderY = (SortOrder)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CoordSortGap"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCoordSortGap = 64;
	}
	else
	{
		tmpSearchDataParam.m_dCoordSortGap =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}



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
	// 	bRet = cXMLConfigurator.FindElem(_T("RectVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bRectVisible = _ttoi(cXMLConfigurator.GetElemData());
	// 	// model point visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bPointVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	

//	cXMLConfigurator.LeaveElem();

	if(tmpTrainDataParam.m_bPatternSearchEnable)
	{
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
		scPatternSearchRot tmpPatternSearchTool;
		bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
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
		m_ImageTrain		= tmpImageModel;
		m_ImageMask			= tmpImageMask;
	}
	else
	{
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
	}


	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CPatternBlobSearchTool::SaveModelToFile(CString strDir)
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

	// contrastthre1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bPatternSearchEnable);
	cXMLConfigurator.AddElem(_T("PatternSearchEnable"), strInfo);
	


	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam//////////////
	// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	// 	cXMLConfigurator.EnterElem();


	// train GUI　rect
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

	CString str;
	str.Format(_T("BlobAffineRect"));
	cXMLConfigurator.AddElem(str);
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetXRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_BlobAffineRect.GetSkew().ToDouble());
	cXMLConfigurator.AddElem(_T("Skew"), strInfo);

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
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainRectVisible);
	// 	cXMLConfigurator.AddElem(_T("TrainRectVisible"), strInfo);
	// 	// Search  rect visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
	// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
	// 	//Model point visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
	// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
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
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bPatternSearchEnable);
	cXMLConfigurator.AddElem(_T("PatternSearchEnable"), strInfo);

	// fileterhalfwidth1
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bBlobMode);
	cXMLConfigurator.AddElem(_T("BlobMode"), strInfo);
	// outlinenum1
	strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobGrayThre);
	cXMLConfigurator.AddElem(_T("BlobGrayThre"), strInfo);
	// edgeproperty1
	strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobAreaMinThre);
	cXMLConfigurator.AddElem(_T("BlobAreaMinThre"), strInfo);
	// SortByScore1
	strInfo.Format(_T("%d"), m_SearchDataParam.m_lBlobAreaMaxThre);
	cXMLConfigurator.AddElem(_T("BlobAreaMaxThre"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bCoordSortEnable);
	cXMLConfigurator.AddElem(_T("CoordSortEnable"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortMethod);
	cXMLConfigurator.AddElem(_T("CoordSortMethod"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderX);
	cXMLConfigurator.AddElem(_T("CoordSortOrderX"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderY);
	cXMLConfigurator.AddElem(_T("CoordSortOrderY"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCoordSortGap);
	cXMLConfigurator.AddElem(_T("CoordSortGap"), strInfo);

	// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	///////////////SearchGuiParam//////////////
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
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bRectVisible);
	// 	cXMLConfigurator.AddElem(_T("RectVisible"), strInfo);
	// 	// model point visible 
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bPointVisible);
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


	if (m_TrainDataParam.m_bPatternSearchEnable)
	{
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

		//////////////////////////存储模板pts文件//////////////////////////
		bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
		if (!bRet)
		{
			return false;
		}
	}
	else
	{
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
	}

	return true;
}	

// 更新训练控件数据信息	
bool CPatternBlobSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();

		m_tmpTrainGuiParam.m_BlobAffineRect = m_GuiTrainBlobAffineRect.GetAffineRect();


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

		m_GuiTrainBlobAffineRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainBlobAffineRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainBlobAffineRect.SetAffineRect(m_tmpTrainGuiParam.m_BlobAffineRect);


	}

	return true;
}

bool CPatternBlobSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchBlobArray.size(); j++)
		{
			m_pGuiSearchBlobArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchBlobArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		

	}

	return true;
}

// 清除控件及其显示					
bool CPatternBlobSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		if (pGraphicsArray[j]!=NULL)
// 		{
// 			delete pGraphicsArray[j];
// 		}
// 		
// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CPatternBlobSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CPatternBlobSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchBlobArray);

	return true;
}

bool CPatternBlobSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen,rot,skew;
	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//Blob区域
	m_GuiTrainBlobAffineRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainBlobAffineRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	double xx = nWidth/m_CamWidth;
	//m_GuiModelPoint.SetLabel(_T("参考点"));


	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CPatternBlobSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		//if (m_tmpTrainDataParam.m_bPatternSearchEnable)
		{
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		}
		

		CString str;
		str.Format("_BlobRegion");
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+str);
		str.Format("_TrainResultBlob");
		m_pGuiStaticContainer->RemoveItem(m_strKeyText+str);
		

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
	}

	return true;
}

bool CPatternBlobSearchTool::ClearResultGui()
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


		for (j=0; j<m_pGuiSearchBlobArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchBlobArray[j]->GetTipText());
		}
	
		m_pGuiStaticContainer->RemoveItem("blob result");

	}

	return true;
}

// 获取训练模板原始图像
bool CPatternBlobSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	/*if (m_tmpTrainDataParam.m_bPatternSearchEnable)
	{
		bool bRet = m_PatternSearchTool.GetPatternImage(ImagePattern);
		if (!bRet)
		{
			return false;
		}
	}
	else
	{
		ImagePattern = m_ImageTrain;
	}*/
	if (m_tmpTrainDataParam.m_bPatternSearchEnable)
	{
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
	else
	{
		ImagePattern = m_ImageTrain;
	}
	

	return true;
}	

// 恢复训练参数
bool CPatternBlobSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternBlobSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternBlobSearchTool::SetLanguage(int nLanguage)
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
CString	CPatternBlobSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							//区域定位失败
				strErrInfo += _T("Pattern search failed");
				break;
			case -2:							//Blob失败
				strErrInfo += _T("Blob failed");
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
				case -1:							//区域定位失败
					strErrInfo += _T("区域定位失败");
					break;
				case -2:							//Blob失败
					strErrInfo += _T("Blob失败");
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
			case -1:							//区域定位失败
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							//Blob失败
				strErrInfo += _T("Blob失败");
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