// CircleSearchTool.cpp: implementation of the CCircleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CircleSearchTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#include "svIntersect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircleSearchTool::CCircleSearchTool()
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	
	
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);


	m_GuiFindCircle.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	// 找圆工具卡尺隐藏，矩形区域可见
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_dGrau					= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre			= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag				= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable			= false;
	m_tmpTrainDataParam.m_nAuxiliaryPosType		= 0;

	m_FindCircleTool.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, 
		m_tmpTrainDataParam.m_lFilterHalfWidth);
	m_tmpTrainDataParam.m_nOutLineNum			= 10;
	
	m_tmpTrainGuiParam.m_nCaliperNum			= m_GuiFindCircle.GetCaliperNum();
	m_tmpTrainGuiParam.m_TrainAffineRect		= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
// 	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
// 	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_nLineWidth				= 1;
	m_tmpTrainGuiParam.m_LineColor				= RGB(0, 0, 255);
	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius,
		m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
	m_tmpTrainGuiParam.m_dCaliperProjectLen		= m_GuiFindCircle.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindCircle.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bCircleCaliperVisible  =m_GuiFindCircle.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bEnableCircleMask		= false;

	m_tmpSearchDataParam.m_dAcceptThre			= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow			= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh			= 5.0;
	m_tmpSearchDataParam.m_nSearchNum			= 1;
// 	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
// 	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();
	m_tmpSearchDataParam.m_bCoordSortEnable		= false;
	m_tmpSearchDataParam.m_nCoordSortMethod		= eCoordXOnly;
	m_tmpSearchDataParam.m_nCoordSortOrderX		= eAscending;
	m_tmpSearchDataParam.m_nCoordSortOrderY		= eAscending;;
	m_tmpSearchDataParam.m_dCoordSortGap		= 64;

	m_tmpSearchGuiParam.m_nLineWidth			= 1;
	m_tmpSearchGuiParam.m_LineColor				= RGB(0, 255, 0);

	m_tmpSearchDataParam.m_bEnableCircleCenterMove		= FALSE;
	m_tmpSearchDataParam.m_nCircleCenterMoveMode		= 0;
	m_tmpSearchDataParam.m_nCircleCenterMoveModeX		= 0;
	m_tmpSearchDataParam.m_nCircleCenterMoveModeY		= 0;
	m_tmpSearchDataParam.m_dCircleCenterMoveLengthX		= 0.0;
	m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMulX	= 0.0;
	m_tmpSearchDataParam.m_dCircleCenterMoveLengthY		= 0.0;
	m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMulY	= 0.0;
	m_tmpSearchDataParam.m_dCircleCenterMoveAngle		= 0.0;
	m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMul	= 0.0;
	
	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CCircleSearchTool::CCircleSearchTool(CCircleSearchTool &cCircleSearchTool)
{
	if (cCircleSearchTool.GetSearchToolType() != eCircleSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);


	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);

	m_GuiFindCircle.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	// 找圆工具卡尺隐藏，矩形区域可见
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cCircleSearchTool.m_CamHeight;
	m_CamWidth								= cCircleSearchTool.m_CamWidth;

	m_TrainDataParam						= cCircleSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCircleSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCircleSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCircleSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCircleSearchTool.m_TrainResult;
	m_ImageTrain							= cCircleSearchTool.m_ImageTrain;
	m_ImageMask								= cCircleSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCircleSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cCircleSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCircleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCircleSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCircleSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCircleSearchTool.UpdateSearchGuiData(true);
	cCircleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCircleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCircleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCircleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCircleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCircleSearchTool.m_tmpImageTrain);
	SetMaskImage(cCircleSearchTool.m_tmpImageMask);
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

CCircleSearchTool& CCircleSearchTool::operator=(CCircleSearchTool &cCircleSearchTool)
{
	if (this == &cCircleSearchTool)
	{
		return *this;
	}

	if (cCircleSearchTool.GetSearchToolType() != eCircleSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);


	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);


	m_GuiFindCircle.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	// 找圆工具卡尺隐藏，矩形区域可见
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cCircleSearchTool.m_CamHeight;
	m_CamWidth								= cCircleSearchTool.m_CamWidth;

	m_TrainDataParam						= cCircleSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCircleSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCircleSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCircleSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCircleSearchTool.m_TrainResult;
	m_ImageTrain							= cCircleSearchTool.m_ImageTrain;
	m_ImageMask								= cCircleSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCircleSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cCircleSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCircleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCircleSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCircleSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCircleSearchTool.UpdateSearchGuiData(true);
	cCircleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCircleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCircleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCircleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCircleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCircleSearchTool.m_tmpImageTrain);
	SetMaskImage(cCircleSearchTool.m_tmpImageMask);
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

CCircleSearchTool::~CCircleSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CCircleSearchTool::GetSearchToolType()
{
	return eCircleSearchTool;
}										   

// 设置显示控件指针		
bool CCircleSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

// 设置训练结果显示控件指针
bool CCircleSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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

// 获取训练结果显示控件指针
scGuiDisplay* CCircleSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 获取显示控件指针										
scGuiDisplay* CCircleSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CCircleSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CCircleSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CCircleSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;
	
	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CCircleSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CCircleSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 更新训练Gui控件显示
bool CCircleSearchTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiFindCircle.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCircleVisible);
	
	if (m_tmpTrainGuiParam.m_bEnableCircleMask)
	{
		m_GuiFindCircle.EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
			scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition|scGuiFindCircle::eGuiDOFMask);
	}
	else
	{
		m_GuiFindCircle.EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
			scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle, m_strKeyText+_T("_FindCircle"));

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

// 更新训练结果Gui控件显示
bool CCircleSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultCircle.SetVisible(m_bTrainResultGuiShow);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle, m_strKeyText+_T("_TrainResultCircle"));
		
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}		

// 更新搜索Gui控件显示														
bool CCircleSearchTool::UpdateSearchResultGuiDisplay()
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
	
	for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible);
	}
	
	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchCirclePointArray1.size(); i++)
	{
		BOOL bShow = FALSE;
		int nShow = m_tmpTrainDataParam.m_nAuxiliaryPosType;
		if (nShow == 0)
		{
			bShow = FALSE;
		}
		else
		{
			bShow = TRUE;
		}
		m_pGuiSearchCirclePointArray1[i]->SetVisible(m_bSearchResultGuiShow && bShow);
	}

	for (i=0; i<m_pGuiSearchCirclePointArray2.size(); i++)
	{
		BOOL bShow = FALSE;
		int nShow = m_tmpTrainDataParam.m_nAuxiliaryPosType;
		if (nShow == 0)
		{
			bShow = FALSE;
		}
		else
		{
			bShow = TRUE;
		}
		m_pGuiSearchCirclePointArray2[i]->SetVisible(m_bSearchResultGuiShow && bShow);
	}

	for (i=0; i<m_pGuiSearchLineSegArray.size(); i++)
	{
		BOOL bShow = FALSE;
		int nShow = m_tmpTrainDataParam.m_nAuxiliaryPosType;
		if (nShow == 0)
		{
			bShow = FALSE;
		}
		else
		{
			bShow = TRUE;
		}

		m_pGuiSearchLineSegArray[i]->SetVisible(m_bSearchResultGuiShow && bShow);
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
		
		for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleArray[i], m_pGuiSearchCircleArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterArray[i], m_pGuiSearchCircleCenterArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array[i], m_pGuiSearchResultFitPoint1Array[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCirclePointArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCirclePointArray1[i], m_pGuiSearchCirclePointArray1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCirclePointArray2.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCirclePointArray2[i], m_pGuiSearchCirclePointArray2[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchLineSegArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineSegArray[i], m_pGuiSearchLineSegArray[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CCircleSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

//	UpdateTrainResultGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CCircleSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CCircleSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CCircleSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	*((CCircleSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CCircleSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CCircleSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CCircleSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CCircleSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CCircleSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CCircleSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CCircleSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CCircleSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CCircleSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	//设置训练参数
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

		if (!bRet)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		// 找圆
		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchDirection, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum);
		m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
		m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius
			,m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
		if (m_tmpTrainDataParam.m_bEnableDualEdge)
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
		} 
		else
		{
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		}
		scCircle cCircle;
		double   dRMS;
		if(bIsProcessImage)
		{
			bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS);
		}
		else
		{
			bRet = m_FindCircleTool.Execute(m_tmpImageTrain, cCircle, dRMS);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-2;       //找圆失败
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dRadius  = cCircle.GetRadius();

		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultCircle.SetCircle(cCircle);

		return true;
	}
	else
	{
		bool bRet = false;
		if(bIsProcessImage)
		{
			bRet = m_QuickSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());
		}
		else
		{
			bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		
		// 找圆
		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchDirection, m_tmpTrainGuiParam.m_dCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum);
		m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
		m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius
			,m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
		if (m_tmpTrainDataParam.m_bEnableDualEdge)
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
		} 
		else
		{
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
		}
		scCircle cCircle;
		double   dRMS;
		if(bIsProcessImage)
		{
			bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS);
		}
		else
		{
			bRet = m_FindCircleTool.Execute(m_tmpImageTrain, cCircle, dRMS);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-2;       //找圆失败
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dRadius  = cCircle.GetRadius();

		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultCircle.SetCircle(cCircle);

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CCircleSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CCircleSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	*((CCircleSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}	

// 是否训练成功
bool CCircleSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CCircleSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CCircleSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CCircleSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}

	*((CCircleSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CCircleSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CCircleSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CCircleSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eCircleSearchTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CCircleSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
// bool CCircleSearchTool::Search(const scImage8 &ImageSearch)
// {
// 	if (!m_bTrainOK)
// 	{
// 		return false;
// 	}	
// 
// 	// 清空
// 	ClearResult();
// 
// 	// 设置搜索个数
// 	if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
// 	{
// 		return false;
// 	}
// 
// 	// 设置接收阈值
// 	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
// 	{
// 		return false;
// 	}
// 
// 	// 设置搜索角度范围
// 	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
// 	{
// 		return false;
// 	}
// 
// 	// 设置缩放比例
// 	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
// 	{
// 		return false;
// 	}
// 
// 	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
// 	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
// 		,m_TrainGuiParam.m_dCaliperProjectLen);
// 	m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
// 	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nOutLineNum);
// 	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore);
// 
// 	// 执行搜索
// 	bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
// 	if (bRet)
// 	{
// 		// 获取结果
// 		int nResultNum = m_QuickSearchTool.GetResultNum();
// 		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
// 		{
// 			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
// 			
// 			// 求变换矩阵
// 			sc2Vector pos = tmpQuickResult.GetPosition();
// 			sc2Vector tmModelPoint;
// 			double dRotation;
// 			double dTransX;
// 			double dTransY;
// 			m_QuickSearchTool.GetModelOrigin(tmModelPoint);
// 			dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
// 			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
// 			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
// 			
// 			// 求找线工具变换后的位置,找线
// 			double dTransCenterX, dTransCenterY;
// 			double dOriCenterX, dOriCenterY;
// 			dOriCenterX = m_TrainGuiParam.m_CircleCenter.GetX();
// 			dOriCenterY = m_TrainGuiParam.m_CircleCenter.GetY();
// 			dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
// 			dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
// 			m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
// 				,m_TrainGuiParam.m_dCaliperProjectLen);
// 			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_dRadius
// 				,scRadian(dRotation)+m_TrainGuiParam.m_ArcStart, m_TrainGuiParam.m_ArcSpan);
// 
// 			// 执行找圆
// 			scCircle ResCircle;
// 			double dRMS;
// 			if (m_FindCircleTool.Execute(ImageSearch, ResCircle, dRMS)) // 成功
// 			{
// 				CSearchResult searchResult;
// 				searchResult.m_dPosX = ResCircle.GetCenter().GetX();
// 				searchResult.m_dPosY = ResCircle.GetCenter().GetY();
// 				searchResult.m_dAngle = 0;
// 				searchResult.m_dScore = tmpQuickResult.GetScore();
// 				m_SearchResultArray.push_back(searchResult);
// 				
// 				CString	strTmpKey;
// 				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
// 				//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
// 				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
// 				pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
// 				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 				pTempAffineRect->SetTipTextEnabled(true);
// 				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
// 				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
// 				
// 				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
// 				//scGuiCross *pTempCross = new scGuiCross;
// 				scGuiCross *pTempCross = m_GuiSearchCross+i;
// 				pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
// 				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 				pTempCross->SetTipTextEnabled(true);
// 				pTempCross->SetTipText(strTmpKey);
// 				m_pGuiSearchCrossArray.push_back(pTempCross);
// 				
// 				strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
// 				//scGuiCircle *pTemCircle = new scGuiCircle;
// 				scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
// 				pTemCircle->SetCircle(ResCircle);
// 				pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 				pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 				pTemCircle->SetTipTextEnabled(true);
// 				pTemCircle->SetTipText(strTmpKey);
// 				CString st;
// 				st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY(), ResCircle.GetRadius());
// 				pTemCircle->SetLabel(st);
// 				pTemCircle->SetLabelVisible(TRUE);
// 				m_pGuiSearchCircleArray.push_back(pTemCircle);
// 
// 				strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, i);
// 				//scGuiCross *pTempCircleCenter = new scGuiCross;
// 				scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+i;
// 				pTempCircleCenter->SetCenterExtent(ResCircle.GetCenter(), sc2Vector(40,40));
// 				pTempCircleCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 				pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 				pTempCircleCenter->SetTipTextEnabled(true);
// 				pTempCircleCenter->SetTipText(strTmpKey);
// 				m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);	
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				std::vector<bool> bUsed;
// 				std::vector<sc2Vector> points;
// 				m_FindCircleTool.GetUsed(bUsed);
// 				m_FindCircleTool.GetFitData(points);
// 				for (long m=0; m<bUsed.size(); m++)
// 				{
// 					bool bUse = bUsed[m];
// 					sc2Vector pt = points[m];
// 
// 					int nIndex = m_pGuiSearchResultFitPoint1Array.size();
// 					strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
// 					scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint1 + nIndex;
// 					pTempFitPoint1->SetXDirectionAdornment(scGuiCoordCross::eNone);
// 					pTempFitPoint1->SetYDirectionAdornment(scGuiCoordCross::eNone);
// 					pTempFitPoint1->SetRulerVisible(false);
// 					pTempFitPoint1->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
// 					if (bUse)
// 					{
// 						pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint1);
// 					}
// 					else
// 					{
// 						pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint1);
// 					}
// 					pTempFitPoint1->SetTipTextEnabled(true);
// 					pTempFitPoint1->SetTipText(strTmpKey);
// 					m_pGuiSearchResultFitPoint1Array.push_back(pTempFitPoint1);	
// 				}
// 				//////////////////////////////////////////////////////////////////////////
// 			}	
// 
// 		}		
// 
// 		if (m_SearchResultArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 
// 		m_SearchDataParam = m_tmpSearchDataParam;
// 		m_SearchGuiParam  = m_tmpSearchGuiParam;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 
// 	return true;
// }

bool CCircleSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
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

	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
		,m_TrainGuiParam.m_dCaliperProjectLen);
	//m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore);
	m_FindCircleTool.SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask);
	
	if (m_tmpTrainDataParam.m_bEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
	} 
	else
	{
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
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
	/*bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);*/
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
		// 按坐标排序
		if (m_tmpSearchDataParam.m_bCoordSortEnable)
		{
			m_QuickSearchTool.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,	m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
		}

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
			double dTransCenterX, dTransCenterY;
			double dOriCenterX, dOriCenterY;
			dOriCenterX = m_TrainGuiParam.m_CircleCenter.GetX();
			dOriCenterY = m_TrainGuiParam.m_CircleCenter.GetY();
			dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
			dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
			m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
				,m_TrainGuiParam.m_dCaliperProjectLen);
			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_dRadius
				,scRadian(dRotation)+m_TrainGuiParam.m_ArcStart, m_TrainGuiParam.m_ArcSpan);

			// 执行找圆
			scCircle ResCircle;
			double dRMS;
			if(bIsProcessImage)
			{
				bRet = m_FindCircleTool.Execute(tmpImage, ResCircle, dRMS);
			}
			else
			{
				bRet = m_FindCircleTool.Execute(ImageSearch, ResCircle, dRMS);
			}
			if(bRet)
			{
				CSearchResult searchResult;
				CResultSearchRect resRect;
				resRect.m_searchRect = tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);


				if (m_TrainDataParam.m_nAuxiliaryPosType == 1)
				{
					// 过圆心的水平直线
					scLine Line;
					Line.SetPos(sc2Vector(ResCircle.GetCenter().GetX(),ResCircle.GetCenter().GetY()));
					Line.SetRotation(scRadian(0));

					// 过圆心的水平直线与圆的交点
					bool bIntersect = false;
					std::vector<sc2Vector> crossPointArray; 
					sfInterSectLineCircle(Line,ResCircle,bIntersect,crossPointArray);

					if (crossPointArray.size() == 2)
					{
						double dLeftX = crossPointArray.at(0).GetX();
						double dLeftY = crossPointArray.at(0).GetY();

						double dRightX = crossPointArray.at(1).GetX();
						double dRightY = crossPointArray.at(1).GetY();

						if (dLeftX < dRightX)
						{
							searchResult.m_dPosX = dLeftX;
							searchResult.m_dPosY = dLeftY;
							searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_vdAuxiliaryPosX.at(0) = dRightX;
							searchResult.m_vdAuxiliaryPosY.at(0) = dRightY;
							searchResult.m_vdAuxiliaryAngle.at(0) = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_dScore = tmpQuickResult.GetScore();

							searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
							searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

							CResultCircleSearchTool resultCirle;
							resultCirle.m_scCircleCircle = ResCircle.GetCenter();
							resultCirle.m_dRadius = ResCircle.GetRadius();
							searchResult.m_vCircleSearchResult.push_back(resultCirle);

							m_SearchResultArray.push_back(searchResult);

						}
						else if (dLeftX > dRightX)
						{
							searchResult.m_dPosX = dRightX;
							searchResult.m_dPosY = dRightY;
							searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_vdAuxiliaryPosX.at(0) = dLeftX;
							searchResult.m_vdAuxiliaryPosY.at(0) = dLeftY;
							searchResult.m_vdAuxiliaryAngle.at(0) = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_dScore = tmpQuickResult.GetScore();

							searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
							searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

							CResultCircleSearchTool resultCirle;
							resultCirle.m_scCircleCircle = ResCircle.GetCenter();
							resultCirle.m_dRadius = ResCircle.GetRadius();
							searchResult.m_vCircleSearchResult.push_back(resultCirle);

							m_SearchResultArray.push_back(searchResult);
						}
						else
						{
							return false;
						}
					}
					else
					{
						searchResult.m_dPosX = ResCircle.GetCenter().GetX();
						searchResult.m_dPosY = ResCircle.GetCenter().GetY();
						searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();

						searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
						searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

						CResultCircleSearchTool resultCirle;
						resultCirle.m_scCircleCircle = ResCircle.GetCenter();
						resultCirle.m_dRadius = ResCircle.GetRadius();
						searchResult.m_vCircleSearchResult.push_back(resultCirle);

						m_SearchResultArray.push_back(searchResult);
					}
				}
				else if (m_TrainDataParam.m_nAuxiliaryPosType == 2)
				{
					// 过圆心的垂直直线
					scLine Line;
					Line.SetPos(sc2Vector(ResCircle.GetCenter().GetX(),ResCircle.GetCenter().GetY()));
					Line.SetRotation(scRadian(scDegree(90)));

					// 过圆心的垂直直线与圆的交点
					bool bIntersect = false;
					std::vector<sc2Vector> crossPointArray; 
					sfInterSectLineCircle(Line,ResCircle,bIntersect,crossPointArray);

					if (crossPointArray.size() == 2)
					{
						double dUpX = crossPointArray.at(0).GetX();
						double dUpY = crossPointArray.at(0).GetY();

						double dDownX = crossPointArray.at(1).GetX();
						double dDownY = crossPointArray.at(1).GetY();

						if (dUpY < dDownY)
						{
							searchResult.m_dPosX = dUpX;
							searchResult.m_dPosY = dUpY;
							searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_vdAuxiliaryPosX.at(0) = dDownX;
							searchResult.m_vdAuxiliaryPosY.at(0) = dDownY;
							searchResult.m_vdAuxiliaryAngle.at(0) = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_dScore = tmpQuickResult.GetScore();

							searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
							searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

							CResultCircleSearchTool resultCirle;
							resultCirle.m_scCircleCircle = ResCircle.GetCenter();
							resultCirle.m_dRadius = ResCircle.GetRadius();
							searchResult.m_vCircleSearchResult.push_back(resultCirle);

							m_SearchResultArray.push_back(searchResult);

						}
						else if (dUpY > dDownY)
						{
							searchResult.m_dPosX = dDownX;
							searchResult.m_dPosY = dDownY;
							searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_vdAuxiliaryPosX.at(0) = dUpX;
							searchResult.m_vdAuxiliaryPosY.at(0) = dUpY;
							searchResult.m_vdAuxiliaryAngle.at(0) = tmpQuickResult.GetRotate().ToDouble();

							searchResult.m_dScore = tmpQuickResult.GetScore();

							searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
							searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

							CResultCircleSearchTool resultCirle;
							resultCirle.m_scCircleCircle = ResCircle.GetCenter();
							resultCirle.m_dRadius = ResCircle.GetRadius();
							searchResult.m_vCircleSearchResult.push_back(resultCirle);

							m_SearchResultArray.push_back(searchResult);
						}
						else
						{
							return false;
						}
					}
					else
					{
						searchResult.m_dPosX = ResCircle.GetCenter().GetX();
						searchResult.m_dPosY = ResCircle.GetCenter().GetY();
						searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();

						searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
						searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

						CResultCircleSearchTool resultCirle;
						resultCirle.m_scCircleCircle = ResCircle.GetCenter();
						resultCirle.m_dRadius = ResCircle.GetRadius();
						searchResult.m_vCircleSearchResult.push_back(resultCirle);

						m_SearchResultArray.push_back(searchResult);
					}
				}
				else
				{
					if(m_tmpSearchDataParam.m_bEnableCircleCenterMove)
					{
						if (m_tmpSearchDataParam.m_nCircleCenterMoveMode == 0)
						{
							switch(m_tmpSearchDataParam.m_nCircleCenterMoveModeX)
							{
							case 0:
								searchResult.m_dPosX = ResCircle.GetCenter().GetX() + m_tmpSearchDataParam.m_dCircleCenterMoveLengthX;
								break;
							case 1:
								searchResult.m_dPosX = ResCircle.GetCenter().GetX() + m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMulX * ResCircle.GetRadius();
								break;
							default:
								break;
							}
							switch(m_tmpSearchDataParam.m_nCircleCenterMoveModeY)
							{
							case 0:
								searchResult.m_dPosY = ResCircle.GetCenter().GetY() + m_tmpSearchDataParam.m_dCircleCenterMoveLengthY;
								break;
							case 1:
								searchResult.m_dPosY = ResCircle.GetCenter().GetY() + m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMulY * ResCircle.GetRadius();
								break;
							default:
								break;
							}
						}
						else if (m_tmpSearchDataParam.m_nCircleCenterMoveMode == 1)
						{
							searchResult.m_dPosX = ResCircle.GetCenter().GetX() + (m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMul * ResCircle.GetRadius())*cos(scDegree(m_tmpSearchDataParam.m_dCircleCenterMoveAngle));
							searchResult.m_dPosY = ResCircle.GetCenter().GetY() + (m_tmpSearchDataParam.m_dCircleCenterMoveRadiusMul * ResCircle.GetRadius())*sin(scDegree(m_tmpSearchDataParam.m_dCircleCenterMoveAngle));
						}
						searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
						searchResult.m_resultCircleSearchTool.m_scCircleCircle = sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY);
						searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();

						CResultCircleSearchTool resultCirle;
						resultCirle.m_scCircleCircle = sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY);
						resultCirle.m_dRadius = ResCircle.GetRadius();
						searchResult.m_vCircleSearchResult.push_back(resultCirle);
					}
					else
					{
						searchResult.m_dPosX = ResCircle.GetCenter().GetX();
						searchResult.m_dPosY = ResCircle.GetCenter().GetY();
						searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();

						searchResult.m_resultCircleSearchTool.m_scCircleCircle = ResCircle.GetCenter();
						searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle.GetRadius();


						CResultCircleSearchTool resultCirle;
						resultCirle.m_scCircleCircle = ResCircle.GetCenter();
						resultCirle.m_dRadius = ResCircle.GetRadius();
						searchResult.m_vCircleSearchResult.push_back(resultCirle);
					}


					m_SearchResultArray.push_back(searchResult);
				}

				CString	strTmpKey;
				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray.push_back(pTempCross);


				strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
				scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
				pTemCircle->SetCircle(ResCircle);
				pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);

				if (m_tmpSearchDataParam.m_bEnableRadiusControl && (ResCircle.GetRadius()< m_tmpSearchDataParam.m_dMinRadius || ResCircle.GetRadius() > m_tmpSearchDataParam.m_dMaxRadius))
				{
					pTemCircle->SetLineColor(RGB(255,0,0));
				}

				pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemCircle->SetTipTextEnabled(true);
				pTemCircle->SetTipText(strTmpKey);
				CString st;
				if(m_tmpSearchDataParam.m_bEnableCircleCenterMove)
				{
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, searchResult.m_resultCircleSearchTool.m_scCircleCircle.GetX(), searchResult.m_resultCircleSearchTool.m_scCircleCircle.GetY(),  ResCircle.GetRadius());
				}
				else
				{
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY(), ResCircle.GetRadius());
				}
				pTemCircle->SetLabel(st);
				pTemCircle->SetLabelVisible(TRUE);
				m_pGuiSearchCircleArray.push_back(pTemCircle);


				strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, i);
				scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+i;
				if(m_tmpSearchDataParam.m_bEnableCircleCenterMove)
				{
					pTempCircleCenter->SetCenterExtent(sc2Vector(searchResult.m_resultCircleSearchTool.m_scCircleCircle.GetX(),searchResult.m_resultCircleSearchTool.m_scCircleCircle.GetY()),sc2Vector(40,40));
				}
				else
				{
					pTempCircleCenter->SetCenterExtent(ResCircle.GetCenter(), sc2Vector(40,40));
				}
				pTempCircleCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCircleCenter->SetTipTextEnabled(true);
				pTempCircleCenter->SetTipText(strTmpKey);
				m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);	


				if (m_TrainDataParam.m_nAuxiliaryPosType != 0)
				{
					strTmpKey.Format(_T("%s_ResultCirclePoint1%d"), m_strKeyText, i);
					scGuiCross *pTempCirclePoint1 = m_GuiSearchCirclePoint1+i;
					pTempCirclePoint1->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(40,40));
// 					pTempCirclePoint1->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 					pTempCirclePoint1->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCirclePoint1->SetLineColor(RGB(255, 0 , 0));
					pTempCirclePoint1->SetLineWidth(1);
					pTempCirclePoint1->SetTipTextEnabled(true);
					pTempCirclePoint1->SetTipText(strTmpKey);
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, searchResult.m_dPosX,searchResult.m_dPosY, ResCircle.GetRadius());
					pTempCirclePoint1->SetLabel(st);
					pTempCirclePoint1->SetLabelVisible(TRUE);
					m_pGuiSearchCirclePointArray1.push_back(pTempCirclePoint1);



					strTmpKey.Format(_T("%s_ResultCirclePoint2%d"), m_strKeyText, i);
					scGuiCross *pTempCirclePoint2 = m_GuiSearchCirclePoint2+i;
					pTempCirclePoint2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(40,40));
// 					pTempCirclePoint2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 					pTempCirclePoint2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCirclePoint2->SetLineColor(RGB(255, 0 , 0));
					pTempCirclePoint2->SetLineWidth(1);
					pTempCirclePoint2->SetTipTextEnabled(true);
					pTempCirclePoint2->SetTipText(strTmpKey);
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0), ResCircle.GetRadius());
					pTempCirclePoint2->SetLabel(st);
					pTempCirclePoint2->SetLabelVisible(TRUE);			
					m_pGuiSearchCirclePointArray2.push_back(pTempCirclePoint2);


					strTmpKey.Format(_T("%s_ResultLineSeg%d"), m_strKeyText, i);
					scGuiLineSeg *pTemLineSeg = m_GuiSearchLineSeg+i;
					pTemLineSeg->SetStartXYEndXY(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), 
						sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)));
					pTemLineSeg->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLineSeg->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLineSeg->SetTipTextEnabled(true);
					pTemLineSeg->SetTipText(strTmpKey);
					m_pGuiSearchLineSegArray.push_back(pTemLineSeg);

				}


				//////////////////////////////////////////////////////////////////////////
				std::vector<bool> bUsed;
				std::vector<sc2Vector> points;
				m_FindCircleTool.GetUsed(bUsed);
				m_FindCircleTool.GetFitData(points);
				for (long m=0; m<bUsed.size(); m++)
				{
					bool bUse = bUsed[m];
					sc2Vector pt = points[m];

					int nIndex = m_pGuiSearchResultFitPoint1Array.size();
					strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
					scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint1 + nIndex;
					pTempFitPoint1->SetXDirectionAdornment(scGuiCoordCross::eNone);
					pTempFitPoint1->SetYDirectionAdornment(scGuiCoordCross::eNone);
					pTempFitPoint1->SetRulerVisible(false);
					pTempFitPoint1->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
					if (bUse)
					{
						pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint1);
					}
					else
					{
						pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint1);
					}
					pTempFitPoint1->SetTipTextEnabled(true);
					pTempFitPoint1->SetTipText(strTmpKey);
					m_pGuiSearchResultFitPoint1Array.push_back(pTempFitPoint1);	
				}
				//////////////////////////////////////////////////////////////////////////	


				// 圆尺寸超限
				if ( m_tmpSearchDataParam.m_bEnableRadiusControl )
				{
					double dCurRadius = ResCircle.GetRadius();
					if (dCurRadius <m_tmpSearchDataParam.m_dMinRadius )
					{
						m_nResultErrNumber = -3;
						return false;
					}
					if ( dCurRadius > m_tmpSearchDataParam.m_dMaxRadius)
					{
						m_nResultErrNumber = -3;
						return false;
					}
				}

			}	
			else
			{
				m_nResultErrNumber=-2;       //找圆失败
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
int CCircleSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CCircleSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像
bool CCircleSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eCircleSearchTool)
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
bool CCircleSearchTool::LoadModelFromFile(CString strDir)
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
	CCircleSearchTrainDataParam tmpTrainDataParam;
	CCircleSearchTrainGuiParam tmpTrainGuiParam;
	CCircleSearchSearchDataParam tmpSearchDataParam;
	CCircleSearchSearchGuiParam tmpSearchGuiParam;
	CCircleSearchTrainResult  tmpTrainResult;
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
	if (cSearchToolType != eCircleSearchTool)
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
		tmpTrainDataParam.m_lFilterHalfWidth = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	}
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
		return false;
	}
	tmpTrainDataParam.m_bSortByScore = _ttoi(cXMLConfigurator.GetElemData());

	// DualEdge
	bRet = cXMLConfigurator.FindElem(_T("DualEdge"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge = _ttoi(cXMLConfigurator.GetElemData());
	}
	

	bRet = cXMLConfigurator.FindElem(_T("AuxiliaryPosType"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nAuxiliaryPosType = _ttoi(cXMLConfigurator.GetElemData());


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
	// circle center
	bRet = cXMLConfigurator.FindElem(_T("CircleCenter"));
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
	tmpTrainGuiParam.m_CircleCenter = vPos;
	cXMLConfigurator.LeaveElem();
	// circle radius  
	bRet = cXMLConfigurator.FindElem(_T("CircleRadius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Arc start   
	bRet = cXMLConfigurator.FindElem(_T("ArcStart"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_ArcStart = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	// Arc span   
	bRet = cXMLConfigurator.FindElem(_T("ArcSpan"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_ArcSpan = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
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

	// CircleCaliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CircleCaliperVisible"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCircleCaliperVisible=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCircleCaliperVisible = _ttoi(cXMLConfigurator.GetElemData());
	}

	//CaliperMask
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableCircleMask = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableCircleMask = false;
	}
	bRet = cXMLConfigurator.FindElem(_T("MaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdCaliperMaskData.resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}

			tmpTrainGuiParam.m_vdCaliperMaskData.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask.size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}
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

	bRet = cXMLConfigurator.FindElem(_T("EnableRadiusControl"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableRadiusControl = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("MinRadius"));
	if (bRet)
	{
		tmpSearchDataParam.m_dMinRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("MaxRadius"));
	if (bRet)
	{
		tmpSearchDataParam.m_dMaxRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	//增加圆心偏移输出距离保存
	bRet = cXMLConfigurator.FindElem(_T("bEnableCircleCenterMove"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bEnableCircleCenterMove = FALSE;
	}
	else
	{
		tmpSearchDataParam.m_bEnableCircleCenterMove = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("nCircleCenterMoveMode"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCircleCenterMoveMode = 0;
	}
	else
	{
		tmpSearchDataParam.m_nCircleCenterMoveMode = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("nCircleCenterMoveModeX"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCircleCenterMoveModeX = 0;
	}
	else
	{
		tmpSearchDataParam.m_nCircleCenterMoveModeX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveLengthX"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveLengthX = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveLengthX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveRadiusMulX"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMulX = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMulX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("nCircleCenterMoveModeY"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCircleCenterMoveModeY = 0;
	}
	else
	{
		tmpSearchDataParam.m_nCircleCenterMoveModeY = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveLengthY"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveLengthY = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveLengthY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveRadiusMulY"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMulY = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMulY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveAngle"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveAngle = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveAngle = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dCircleCenterMoveRadiusMul"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMul = 0.0;
	}
	else
	{
		tmpSearchDataParam.m_dCircleCenterMoveRadiusMul = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());
	}



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
	bRet = cXMLConfigurator.FindElem(_T("Radius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
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
bool CCircleSearchTool::SaveModelToFile(CString strDir)
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

	//////////////////////////存储xml文件//////////////////////////
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
	// DualEdge
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge);
	cXMLConfigurator.AddElem(_T("DualEdge"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nAuxiliaryPosType);
	cXMLConfigurator.AddElem(_T("AuxiliaryPosType"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 	
// 	cXMLConfigurator.LeaveElem();
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
	// circle center
	cXMLConfigurator.AddElem(_T("CircleCenter"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// circle radius 
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dRadius);
	cXMLConfigurator.AddElem(_T("CircleRadius"), strInfo);
	// Arc start 
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ArcStart.ToDouble());
	cXMLConfigurator.AddElem(_T("ArcStart"), strInfo);
	// Arc Span
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ArcSpan.ToDouble());
	cXMLConfigurator.AddElem(_T("ArcSpan"), strInfo);
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
	//CircleCaliper Visible
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleCaliperVisible);
	cXMLConfigurator.AddElem(_T("CircleCaliperVisible"), strInfo);
	
	//CircleMask
	CString strTmp;
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableCircleMask);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask.size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask.size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();
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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_bEnableRadiusControl);
	cXMLConfigurator.AddElem(_T("EnableRadiusControl"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dMinRadius);
	cXMLConfigurator.AddElem(_T("MinRadius"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dMaxRadius);
	cXMLConfigurator.AddElem(_T("MaxRadius"), strInfo);

	//添加搜索设置中的偏移设置
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableCircleCenterMove);
	cXMLConfigurator.AddElem(_T("bEnableCircleCenterMove"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCircleCenterMoveMode);
	cXMLConfigurator.AddElem(_T("nCircleCenterMoveMode"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCircleCenterMoveModeX);
	cXMLConfigurator.AddElem(_T("nCircleCenterMoveModeX"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveLengthX);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveLengthX"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveRadiusMulX);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveRadiusMulX"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCircleCenterMoveModeY);
	cXMLConfigurator.AddElem(_T("nCircleCenterMoveModeY"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveLengthY);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveLengthY"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveRadiusMulY);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveRadiusMulY"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveAngle);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveAngle"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCircleCenterMoveRadiusMul);
	cXMLConfigurator.AddElem(_T("dCircleCenterMoveRadiusMul"), strInfo);


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


	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

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
	strInfo.Format(_T("%f"), m_TrainResult.m_dRadius);
	cXMLConfigurator.AddElem(_T("Radius"), strInfo);	
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
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	
	
// 更新训练控件数据信息	
bool CCircleSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nCaliperNum		= m_GuiFindCircle.GetCaliperNum();
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
		m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius,
			m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_tmpTrainGuiParam.m_dCaliperProjectLen = m_GuiFindCircle.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen	= m_GuiFindCircle.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bCircleCaliperVisible	= m_GuiFindCircle.IsCaliperVisible();
		
		m_GuiFindCircle.GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData);
		m_GuiFindCircle.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
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

		m_GuiFindCircle.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindCircle.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindCircle.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius
															,m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_GuiFindCircle.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_GuiFindCircle.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen);
		m_GuiFindCircle.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchDirection);
		m_GuiFindCircle.SetCaliperVisible(m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetAnnulusFindCircleVisible(!m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData);
	}

	return true;
}

bool CCircleSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiSearchCircleArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiSearchCircleCenterArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleCenterArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

// 		for (j=0; j<m_pGuiSearchCirclePointArray1.size(); j++)
// 		{
// 			m_pGuiSearchCirclePointArray1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			m_pGuiSearchCirclePointArray1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 		}
// 
// 		for (j=0; j<m_pGuiSearchCirclePointArray2.size(); j++)
// 		{
// 			m_pGuiSearchCirclePointArray2[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			m_pGuiSearchCirclePointArray2[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 
// 		}

		for (j=0; j<m_pGuiSearchLineSegArray.size(); j++)
		{
			m_pGuiSearchLineSegArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineSegArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CCircleSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

bool CCircleSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CCircleSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);

	ClearGraphicsArray(m_pGuiSearchCirclePointArray1);
	ClearGraphicsArray(m_pGuiSearchCirclePointArray2);
	ClearGraphicsArray(m_pGuiSearchLineSegArray);

	return true;
}															

bool CCircleSearchTool::SetGuiByCam(double nWidth,double nHeight)
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

	//找圆
	double rad,asta,aspan;
	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

	double plength,slength;
	plength = m_GuiFindCircle.GetCaliperProjectionLength();
	slength = m_GuiFindCircle.GetCaliperSearchLength();
	m_GuiFindCircle.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindCircle.SetCaliperSearchLength(slength/m_CamWidth*nWidth);
	
	//参考点位置
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
}

bool CCircleSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindCircle"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;	
}

bool CCircleSearchTool::ClearResultGui()
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
		
		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleArray[j]->GetTipText());
		}
		
		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint1Array[j]->GetTipText());
		}
    
		for (j=0; j<m_pGuiSearchCirclePointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCirclePointArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCirclePointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCirclePointArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineSegArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineSegArray[j]->GetTipText());
		}
	}
	
	return true;	
}

// 获取训练模板原始图像
bool CCircleSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	/*bool bRet = m_QuickSearchTool.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
	return false;
	}
	return true;*/
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
bool CCircleSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CCircleSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CCircleSearchTool::SetLanguage(int nLanguage)
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
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}
CString	CCircleSearchTool::GetResultErrInfo()
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
			case -2:							//找圆失败
				strErrInfo += _T("Find circle failed");
				break;
			case -3:							//圆半径超限
				strErrInfo += _T("Circle Radius Out of Range");
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
			case -2:							//找圆失败
				strErrInfo += _T("找圆失败");
				break;
			case -3:							//圆半径超限
				strErrInfo += _T("圆半径超限");
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
			case -2:							//找圆失败
				strErrInfo += _T("找圆失败");
				break;
			case -3:							//圆半径超限
				strErrInfo += _T("圆半径超限");
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
