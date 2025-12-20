// CombinedQuickSearch.cpp: implementation of the CCombinedQuickSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombinedQuickSearch.h"

#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 
#include "svTimer.h"


CCombinedQuickSearch::CCombinedQuickSearch()
{
	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100,0,0);
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(150, 240, 50, 50,0,0);
	//	m_GuiTrainRect1.SetLabel(_T("辅助训练区域"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect1.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect1.SetLabel(_T("辅助搜索区域"));
	m_GuiSearchRect1.SetLabelVisible(FALSE);
	m_GuiSearchRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

// 	m_GuiModelPoint1.SetCenterXYWidthHeight(350, 300, 20, 20);
// 	m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
// 	m_GuiModelPoint1.SetLabelVisible(TRUE);
// 	m_GuiModelPoint1.SetSelectedLineColor(RGB(255,255,0));
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
//		m_GuiModelPoint1.SetLabel(_T("Check Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	m_tmpTrainDataParam.m_dGrau				= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_dGrau1			= m_QuickSearchTool1.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lNoiseThre1		= m_QuickSearchTool1.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_lTotalMag1		= m_QuickSearchTool1.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像
	m_tmpTrainDataParam.m_bMaskEnable1		= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_TrainAffineRect1	= m_GuiTrainRect1.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint1		= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_SearchRect1		= m_GuiSearchRect1.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAcceptThre2		= m_QuickSearchTool1.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;
	m_tmpSearchDataParam.m_dSizeLow			= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh		= 1.0;
	m_tmpSearchDataParam.m_nPosIndex		= 0;

	m_tmpSearchGuiParam.m_nLineWidth		= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor			= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;
}

CCombinedQuickSearch::CCombinedQuickSearch(CCombinedQuickSearch &cCombinedQuickSearchTool)
{
	if (cCombinedQuickSearchTool.GetSearchToolType() != eCombinedQuickSearchTool)
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100,0,0);
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(150, 240, 50, 50,0,0);
	//	m_GuiTrainRect1.SetLabel(_T("辅助训练区域"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect1.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect1.SetLabel(_T("辅助搜索区域"));
	m_GuiSearchRect1.SetLabelVisible(FALSE);
	m_GuiSearchRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

	// 	m_GuiModelPoint1.SetCenterXYWidthHeight(350, 300, 20, 20);
	// 	m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
	// 	m_GuiModelPoint1.SetLabelVisible(TRUE);
	// 	m_GuiModelPoint1.SetSelectedLineColor(RGB(255,255,0));
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
//		m_GuiModelPoint1.SetLabel(_T("Check Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cCombinedQuickSearchTool.m_CamHeight;
	m_CamWidth								= cCombinedQuickSearchTool.m_CamWidth;
	
	m_TrainDataParam						= cCombinedQuickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinedQuickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinedQuickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinedQuickSearchTool.m_SearchGuiParam;
	m_TrainResult                           = cCombinedQuickSearchTool.m_TrainResult;
	m_ImageTrain							= cCombinedQuickSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinedQuickSearchTool.m_ImageMask;
	m_ImageMask1							= cCombinedQuickSearchTool.m_ImageMask1;
	m_bTrainGuiShow							= cCombinedQuickSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cCombinedQuickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinedQuickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinedQuickSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cCombinedQuickSearchTool.UpdateSearchGuiData(true);
	cCombinedQuickSearchTool.UpdateTrainGuiData(true);

	SetTrainDataParam(&cCombinedQuickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinedQuickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinedQuickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinedQuickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinedQuickSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinedQuickSearchTool.m_tmpImageMask);
	SetMaskImage2(cCombinedQuickSearchTool.m_tmpImageMask1);

	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return;
		}
		// 设置训练参数1
		if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
		{
			return;
		}


		if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return;
		}
		if (!m_QuickSearchTool1.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
		{
			return;
		}

		if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
		{
			return;
		}
		if (!m_QuickSearchTool1.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
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
		cpImage tmpImageMask1;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);

			tmpImageMask1.CreateImageBuffer(m_ImageMask1.Width(),m_ImageMask1.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask1,tmpImageMask1);
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


		// 设置掩模图像1
		if (m_TrainDataParam.m_bMaskEnable1)
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool1.Train(tmpImage, tmpImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			m_bTrainOK = bRet && m_bTrainOK;
		}
		else
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool1.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			
			m_bTrainOK = bRet && m_bTrainOK;
		}
	}		
}

CCombinedQuickSearch& CCombinedQuickSearch::operator=(CCombinedQuickSearch& cCombinedQuickSearchTool)
{
	if (this == &cCombinedQuickSearchTool)
	{
		return *this;
	}

	if (cCombinedQuickSearchTool.GetSearchToolType() != eCombinedQuickSearchTool)
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100,0,0);
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(150, 240, 50, 50,0,0);
	//	m_GuiTrainRect1.SetLabel(_T("辅助训练区域"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect1.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect1.SetLabel(_T("辅助搜索区域"));
	m_GuiSearchRect1.SetLabelVisible(FALSE);
	m_GuiSearchRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

//	m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
// 	m_GuiModelPoint1.SetLabelVisible(TRUE);
// 	m_GuiModelPoint1.SetSelectedLineColor(RGB(255,255,0));
// 	m_GuiModelPoint1.SetCenterXYWidthHeight(350, 300, 20, 20);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
//		m_GuiModelPoint1.SetLabel(_T("Check Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	}
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cCombinedQuickSearchTool.m_CamHeight;
	m_CamWidth								= cCombinedQuickSearchTool.m_CamWidth;

	m_TrainDataParam						= cCombinedQuickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinedQuickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinedQuickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinedQuickSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCombinedQuickSearchTool.m_TrainResult;
	m_ImageTrain							= cCombinedQuickSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinedQuickSearchTool.m_ImageMask;
	m_ImageMask1							= cCombinedQuickSearchTool.m_ImageMask1;
	m_bTrainGuiShow							= cCombinedQuickSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cCombinedQuickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinedQuickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinedQuickSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCombinedQuickSearchTool.UpdateSearchGuiData(true);
	cCombinedQuickSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCombinedQuickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinedQuickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinedQuickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinedQuickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinedQuickSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinedQuickSearchTool.m_tmpImageMask);
	SetMaskImage2(cCombinedQuickSearchTool.m_tmpImageMask1);

	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return *this;
		}
		// 设置训练参数1
		if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
		{
			return *this;
		}

		if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return *this;
		}
		if (!m_QuickSearchTool1.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
		{
			return *this;
		}

		if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
		{
			return *this;
		}
		if (!m_QuickSearchTool1.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
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
		cpImage tmpImageMask1;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);

			tmpImageMask1.CreateImageBuffer(m_ImageMask1.Width(),m_ImageMask1.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask1,tmpImageMask1);
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


		// 设置掩模图像1
		if (m_TrainDataParam.m_bMaskEnable1)
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool1.Train(tmpImage, tmpImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			m_bTrainOK = bRet && m_bTrainOK;
		}
		else
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool1.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
					m_TrainGuiParam.m_ModelPoint1.GetY());
			}

			m_bTrainOK = bRet && m_bTrainOK;
		}

	}

	return *this;
}

CCombinedQuickSearch::~CCombinedQuickSearch()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CCombinedQuickSearch::GetSearchToolType()
{
	return eCombinedQuickSearchTool;
}										   

// 设置显示控件指针		
bool CCombinedQuickSearch::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CCombinedQuickSearch::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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

// 设置训练结果显示控件指针
bool CCombinedQuickSearch::SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay2 = pGuiDisplay;
	m_hTrainResultDispWnd2= NULL;
	if(m_pGuiTrainResultDisplay2)
	{
		m_hTrainResultDispWnd2 = m_pGuiTrainResultDisplay2->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}

// 获取显示控件指针										
scGuiDisplay* CCombinedQuickSearch::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CCombinedQuickSearch::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CCombinedQuickSearch::GetTrainResultDisplay2()
{
	return m_pGuiTrainResultDisplay2;
}

// 设置Gui图形的关键字														
bool CCombinedQuickSearch::SetGuiKeyText(CString strGuiKeyText)									
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
bool CCombinedQuickSearch::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CCombinedQuickSearch::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CCombinedQuickSearch::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CCombinedQuickSearch::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CCombinedQuickSearch::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiTrainRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);

	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
//	m_GuiModelPoint1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);

	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
//	m_GuiSearchRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiSearchRect1.SetVisible(FALSE);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect1, m_strKeyText+_T("_TrainRect1"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
//		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint1, m_strKeyText+_T("_ModelPoint1"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect1, m_strKeyText+_T("_SearchRect1"));

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	{
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
	}
	
	{
		scFeatureletChainSet tmpscFLCS;
		bool bflag=m_QuickSearchTool1.GetTrainFeatures(tmpscFLCS);
		if(bflag)
		{
			m_GuiTrainFeaturelet2.SetFeaturelet(tmpscFLCS);
			m_GuiTrainFeaturelet2.SetVisible(m_bTrainOK);

			if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet2,m_strKeyText+_T("_FeatureChain2"));

				m_pGuiTrainResultDisplay2->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay2->MyInvalidate();
			}
		}
	}
	return true;
}

// 更新搜索界面显示									
bool CCombinedQuickSearch::UpdateSearchResultGuiDisplay()							
{
	int i=0;

	for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
	{
		m_pGuiSearchAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
//		m_pGuiSearchAffineRectArray[i]->SetVisible(TRUE);
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

bool CCombinedQuickSearch::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CCombinedQuickSearch::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CCombinedQuickSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CCombinedQuickSearch::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	*((CCombinedQuickSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CCombinedQuickSearch::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CCombinedQuickSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CCombinedQuickSearch::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CCombinedQuickSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CCombinedQuickSearch::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CCombinedQuickSearch::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CCombinedQuickSearch::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CCombinedQuickSearch::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像1									
bool CCombinedQuickSearch::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask1 = ImageMask;

	return true;
}

// 获取训练原始图像1								
bool CCombinedQuickSearch::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask1;

	return true;
}

// 执行训练											
bool CCombinedQuickSearch::Train()
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
	if (!m_QuickSearchTool1.SetGranularity(m_tmpTrainDataParam.m_dGrau1))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre1))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag1))
	{
		m_nResultErrNumber=-2;       //检查失败
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
	cpImage tmpImageMask1;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_tmpImageMask.Width(),m_tmpImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask,tmpImageMask);

		tmpImageMask1.CreateImageBuffer(m_tmpImageMask1.Width(),m_tmpImageMask1.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask1,tmpImageMask1);
	}		

	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		bool bRet =false;
		if(bIsProcessImage)
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

		if (false == m_bTrainOK)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return m_bTrainOK;
		}

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
		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			m_nResultErrNumber=-1;       //几何定位失败
			return m_bTrainOK;
		}

	}

	// 设置掩模图像1
	if (m_tmpTrainDataParam.m_bMaskEnable1)
	{
		bool bRet = false;
		if(bIsProcessImage)
		{
			bRet = m_QuickSearchTool1.Train(tmpImage, tmpImageMask1, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint1.GetY());	
		}
		else
		{
			bRet = m_QuickSearchTool1.Train(m_tmpImageTrain, m_tmpImageMask1, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint1.GetY());	
		}
		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			m_nResultErrNumber=-2;       //检查失败
			return m_bTrainOK;
		}

	}
	else
	{
		bool bRet = false;
		if(bIsProcessImage)
		{
			bRet = m_QuickSearchTool1.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect1, 
				m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
		}
		else
		{
			bRet = m_QuickSearchTool1.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect1, 
				m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
		}
		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			m_nResultErrNumber=-2;       //检查失败
			return m_bTrainOK;
		}
	}

	if (m_bTrainOK)
	{
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		sc2Vector cModelOrigin1;
		m_QuickSearchTool1.GetModelOrigin(cModelOrigin1);
		m_tmpTrainResult.m_dModelX1 = cModelOrigin1.GetX();
		m_tmpTrainResult.m_dModelY1 = cModelOrigin1.GetY();


		if (m_tmpTrainDataParam.m_bMaskEnable)
		{
			m_ImageMask      = m_tmpImageMask;
		}

		if (m_tmpTrainDataParam.m_bMaskEnable1)
		{
			m_ImageMask1      = m_tmpImageMask1;
		}
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;

	}

	return m_bTrainOK;

}

// 获取训练结果图像														
bool CCombinedQuickSearch::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果图像														
bool CCombinedQuickSearch::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool1.GetTrainModelImage(m_ImageTrainResult1);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult1;

	return true;
}	

// 获取训练结果
bool CCombinedQuickSearch::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	*((CCombinedQuickSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}						

// 是否训练成功
bool CCombinedQuickSearch::IsTrained()
{
	return m_bTrainOK;
}															

// 设置搜索Data参数	
bool CCombinedQuickSearch::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CCombinedQuickSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CCombinedQuickSearch::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	*((CCombinedQuickSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CCombinedQuickSearch::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CCombinedQuickSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CCombinedQuickSearch::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCombinedQuickSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CCombinedQuickSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CCombinedQuickSearch::Search(cpImage &ImageSearch)
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
	if (!m_QuickSearchTool1.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber=-2;       //检查失败
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}
	if (!m_QuickSearchTool1.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber=-2;       //检查失败
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
			return false;
		}

		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);


			/////////////////////////////辅助仿射矩形区域定位/////////////////////////////
			sc2Vector pos = tmpQuickResult.GetPosition();
			sc2Vector tmModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_QuickSearchTool.GetModelOrigin(tmModelPoint);
			dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

// 			sc2Vector pos = tmpQuickResult.MatchRegion().GetCornerPo();  //搜索结果放射矩形原点
// 			sc2Vector tmModelPoint;
// 			double dRotation;
// 			double dTransX;
// 			double dTransY;
// //			m_QuickSearchTool.GetModelOrigin(tmModelPoint);
// 			tmModelPoint = m_QuickSearchTool.GetTrainAffineRectRegion().GetCornerPo();  // 训练结果放射矩形原点
// 			dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
// 			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
// 			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

			// 辅助仿射矩形训练区域变量（训练区域1最小外接矩形）
			scAffineRect tmpTrainAffineRect = m_QuickSearchTool1.GetTrainAffineRectRegion();
// 			double dWidth = tmpTrainAffineRect.GetXLength();
// 			double dHeight = tmpTrainAffineRect.GetYLength(); 
			double dLeftX = tmpTrainAffineRect.GetCornerPo().GetX();
			double dLeftY = tmpTrainAffineRect.GetCornerPo().GetY();
			double dRightX = tmpTrainAffineRect.GetCornerPopp().GetX();
			double dRightY = tmpTrainAffineRect.GetCornerPopp().GetY();

			double dWidth  = dRightX - dLeftX;
			double dHeight = dRightY - dLeftY;

			double dTransStartX, dTransStartY;
			dTransStartX = cos(dRotation) * dLeftX  - sin(dRotation) * dLeftY + dTransX;
			dTransStartY = sin(dRotation) * dLeftX  + cos(dRotation) * dLeftY + dTransY;

// 			sc2Vector dCornerPo = tmpTrainAffineRect.GetCornerPo() + sc2Vector(dTransX,dTransY) + sc2Vector(-10,-10);
// 			sc2Vector dPx = tmpTrainAffineRect.GetCornerPx() + sc2Vector(dTransX,dTransY) + sc2Vector(10,-10);
// 			sc2Vector dPy = tmpTrainAffineRect.GetCornerPy() + sc2Vector(dTransX,dTransY) + sc2Vector(-10,10);


			sc2Vector dCornerPo, dPx, dPy;
			if (dWidth > 0 && dHeight > 0)
			{
				dCornerPo = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(-10,-10);
				dPx = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(dWidth,0) + sc2Vector(10,-10);
				dPy = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(0,dHeight) + sc2Vector(-10,10);
			}
			else if (dWidth < 0 && dHeight < 0)
			{
				dCornerPo = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(10,10);
				dPx = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(dWidth,0) + sc2Vector(-10,10);
				dPy = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(0,dHeight) + sc2Vector(10,-10);
			}
			else if (dWidth < 0 && dHeight > 0)
			{
				dCornerPo = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(10,-10);
				dPx = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(dWidth,0) + sc2Vector(-10,-10);
				dPy = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(0,dHeight) + sc2Vector(10,10);
			}
			else if (dWidth > 0 && dHeight < 0)
			{
				dCornerPo = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(-10,10);
				dPx = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(dWidth,0) + sc2Vector(10,10);
				dPy = sc2Vector(dTransStartX, dTransStartY) + sc2Vector(0,dHeight) + sc2Vector(-10,-10);
			}
			else
			{
				return false;
			}

			// 设置搜索区域
			scAffineRect tmpAffineSearchRect(dCornerPo, dPx, dPy);
			scRect tmpSearchRect = tmpAffineSearchRect.BoundingBox();


			// 辅助仿射矩形训练区域变量（界面设置搜索区域）
// 			double dWidth = m_TrainGuiParam.m_SearchRect1.GetWidth();
// 			double dHeight = m_TrainGuiParam.m_SearchRect1.GetHeight();
// 			double dLeftX = m_TrainGuiParam.m_SearchRect1.GetUL().GetX();
// 			double dLeftY = m_TrainGuiParam.m_SearchRect1.GetUL().GetY();
// 
// 			sc2Vector dCornerPo = m_TrainGuiParam.m_TrainAffineRect1.GetCornerPo();
// 			sc2Vector dPx = m_TrainGuiParam.m_TrainAffineRect1.GetCornerPx();
// 			sc2Vector dPy = m_TrainGuiParam.m_TrainAffineRect1.GetCornerPy();
// 
// 			double dTransStartX, dTransStartY;
// 			dTransStartX = cos(dRotation) * dLeftX  - sin(dRotation) * dLeftY + dTransX;
// 			dTransStartY = sin(dRotation) * dLeftX  + cos(dRotation) * dLeftY + dTransY;
// 
// 			// 设置辅助仿射矩形搜索区域
// 			scRect tmpSearchRect;
// 			tmpSearchRect.Set(dTransStartX, dTransStartY, dWidth, dHeight);

			
			// 执行搜索1
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool1.Execute(tmpImage, tmpSearchRect);
			}
			else
			{
				bRet = m_QuickSearchTool1.Execute(ImageSearch, tmpSearchRect);
			}
			

			scAffineRect resultInspectRect;
			if (bRet)
			{
				// 获取结果
				int nResultNum1 = m_QuickSearchTool1.GetResultNum();

				if (nResultNum1 <= 0)
				{
					return false;
				}

				for (int i=0; i<nResultNum1 && i<MAXRESNUM; i++)
				{
					scQuickSearchResult tmpQuickResult1 = m_QuickSearchTool1.GetResult(i);

					resultInspectRect = tmpQuickResult1.MatchRegion();

					CString	strTmpKey;
					strTmpKey.Format(_T("%s_Result1Rect%d"), m_strKeyText, i);
					//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
					scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect1+i;
					pTempAffineRect->SetAffineRect(tmpQuickResult1.MatchRegion());
					//pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect->SetLineColor(RGB(255,0,255));
					pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect->SetTipTextEnabled(true);
					pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

					strTmpKey.Format(_T("%s_SearchRect%d"), m_strKeyText, i);
//					scGuiAffineRect *pTempAffineRect1 = new scGuiAffineRect;
					scGuiAffineRect *pTempAffineRect1 = m_GuiSearchAffineRect1+i+1;
					pTempAffineRect1->SetAffineRect(tmpAffineSearchRect);
					//pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect1->SetLineColor(RGB(255,255,255));
					pTempAffineRect1->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect1->SetTipTextEnabled(true);
					pTempAffineRect1->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchAffineRectArray.push_back(pTempAffineRect1);

					strTmpKey.Format(_T("%s_Result1Cross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross1+i;
					pTempCross->SetCenterExtent(tmpQuickResult1.GetPosition(), sc2Vector(20,20));
					//pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineColor(RGB(255,0,255));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					m_pGuiSearchCrossArray.push_back(pTempCross);
				}	
			}
			else
			{
				m_nResultErrNumber=-2;       //检查失败
				return bRet;
			}

			/////////////////////////////////////////////////////////////////////

			CSearchResult searchResult;
			searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
			searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
			searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
			searchResult.m_dScore = tmpQuickResult.GetScore();

			// 粗定位矩形
			CResultSearchRect resRect;
			resRect.m_searchRect = tmpQuickResult.MatchRegion();
			searchResult.m_vTrainSearchRect.push_back(resRect);

			// 设置检测矩形
			CResultSearchRect resultRect;
			resultRect.m_bFindRect = TRUE;
			resultRect.m_searchRect = resultInspectRect;
			searchResult.m_vACFSearchRect.push_back(resultRect);

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

			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpQuickResult.GetPosition().GetX(), tmpQuickResult.GetPosition().GetY(), tmpQuickResult.GetRotate().ToDouble(),tmpQuickResult.GetScore());
			pTempCross->SetLabel(st);
			pTempCross->SetLabelVisible(TRUE);

			m_pGuiSearchCrossArray.push_back(pTempCross);
		}	

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;

	}
	else
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return bRet;
	}


// 	scRect tmpSearchRect;
// 	if (0 == m_tmpSearchDataParam.m_nPosIndex)
// 	{
// 		double width = m_SearchResultArray.at(0).m_dPosX - m_TrainGuiParam.m_SearchRect.GetUL().GetX();
// 		double heigh = m_SearchResultArray.at(0).m_dPosY - m_TrainGuiParam.m_SearchRect.GetUL().GetY();
// 
// 		tmpSearchRect.Set(m_TrainGuiParam.m_SearchRect.GetUL().GetX(), m_TrainGuiParam.m_SearchRect.GetUL().GetY(), width, heigh);
// 	}
// 	else if (1 == m_tmpSearchDataParam.m_nPosIndex)
// 	{
// 		double width = m_TrainGuiParam.m_SearchRect.GetLR().GetX() - m_SearchResultArray.at(0).m_dPosX;
// 		double heigh =  m_SearchResultArray.at(0).m_dPosY - m_TrainGuiParam.m_SearchRect.GetUL().GetY();
// 		
// 		tmpSearchRect.Set(m_SearchResultArray.at(0).m_dPosX, m_TrainGuiParam.m_SearchRect.GetUL().GetY(), width, heigh);
// 	}
// 	else if (2 == m_tmpSearchDataParam.m_nPosIndex)
// 	{
// 		double width = m_SearchResultArray.at(0).m_dPosX - m_TrainGuiParam.m_SearchRect.GetUL().GetX();
// 		double heigh =  m_TrainGuiParam.m_SearchRect.GetLR().GetY() - m_SearchResultArray.at(0).m_dPosY;
// 		
// 		tmpSearchRect.Set(m_TrainGuiParam.m_SearchRect.GetUL().GetX(), m_SearchResultArray.at(0).m_dPosY, width, heigh);
// 	}
// 	else if (3 == m_tmpSearchDataParam.m_nPosIndex)
// 	{
// 		double width = m_TrainGuiParam.m_SearchRect.GetLR().GetX() - m_SearchResultArray.at(0).m_dPosX;
// 		double heigh = m_TrainGuiParam.m_SearchRect.GetLR().GetY() - m_SearchResultArray.at(0).m_dPosY;
// 		
// 		tmpSearchRect.Set(m_SearchResultArray.at(0).m_dPosX, m_SearchResultArray.at(0).m_dPosY, width, heigh);
// 	}
// 
// 	// 执行搜索1
// //	bool bRet1 = m_QuickSearchTool1.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
// 	bool bRet1 = m_QuickSearchTool1.Execute(ImageSearch, tmpSearchRect);
// 	if (bRet1)
// 	{
// 		// 获取结果
// 		int nResultNum1 = m_QuickSearchTool1.GetResultNum();
// 
// 		if (nResultNum1 <= 0)
// 		{
// 			return false;
// 		}
// 
// 		for (int i=0; i<nResultNum1 && i<MAXRESNUM; i++)
// 		{
// 			scQuickSearchResult tmpQuickResult1 = m_QuickSearchTool1.GetResult(i);
// 
// // 			CSearchResult searchResult;
// // 			searchResult.m_dPosX = tmpQuickResult1.GetPosition().GetX();
// // 			searchResult.m_dPosY = tmpQuickResult1.GetPosition().GetY();
// // 			searchResult.m_dAngle = tmpQuickResult1.GetRotate().ToDouble();
// // 			searchResult.m_dScore = tmpQuickResult1.GetScore();
// // 			m_SearchResultArray.push_back(searchResult);
// 
// 			CString	strTmpKey;
// 			strTmpKey.Format(_T("%s_Result1Rect%d"), m_strKeyText, i);
// 			//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
// 			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect1+i;
// 			pTempAffineRect->SetAffineRect(tmpQuickResult1.MatchRegion());
// //			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			pTempAffineRect->SetLineColor(RGB(255,0,0));
// 			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempAffineRect->SetTipTextEnabled(true);
// 			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
// 			m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
// 
// 			strTmpKey.Format(_T("%s_Result1Cross%d"), m_strKeyText, i);
// 			//scGuiCross *pTempCross = new scGuiCross;
// 			scGuiCross *pTempCross = m_GuiSearchCross1+i;
// 			pTempCross->SetCenterExtent(tmpQuickResult1.GetPosition(), sc2Vector(40,40));
// //			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			pTempCross->SetLineColor(RGB(255,0,0));
// 			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempCross->SetTipTextEnabled(true);
// 			pTempCross->SetTipText(strTmpKey);
// 
// // 			CString st;
// // 			st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpQuickResult1.GetPosition().GetX(), tmpQuickResult.GetPosition().GetY(), tmpQuickResult.GetRotate().ToDouble(),tmpQuickResult.GetScore());
// // 			pTempCross->SetLabel(st);
// // 			pTempCross->SetLabelVisible(TRUE);
// 
// 			m_pGuiSearchCrossArray.push_back(pTempCross);
// 		}	
// 
// // 		m_SearchDataParam = m_tmpSearchDataParam;
// // 		m_SearchGuiParam  = m_tmpSearchGuiParam;
// 	}
// 	else
// 	{
// 		return bRet1;
// 	}
// 
// 	if (bRet && bRet1)
// 	{
// 		m_SearchDataParam = m_tmpSearchDataParam;
// 		m_SearchGuiParam  = m_tmpSearchGuiParam;
// 	}

	return bRet;
}

// 获取搜索结果个数										
int CCombinedQuickSearch::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CCombinedQuickSearch::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CCombinedQuickSearch::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eCombinedQuickSearchTool)
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
bool CCombinedQuickSearch::LoadModelFromFile(CString strDir)
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
	CCombinedQuickSearchTrainDataParam tmpTrainDataParam;
	CCombinedQuickSearchTrainGuiParam tmpTrainGuiParam;
	CCombinedQuickSearchDataParam tmpSearchDataParam;
	CCombinedQuickSearchGuiParam tmpSearchGuiParam;
	CCombinedQuickSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eCombinedQuickSearchTool)
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

	// grau1
	bRet = cXMLConfigurator.FindElem(_T("Grau1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dGrau1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// noise thre1
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lNoiseThre1 = _ttol(cXMLConfigurator.GetElemData());
	// total mag1
	bRet = cXMLConfigurator.FindElem(_T("TotalMag1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lTotalMag1 = _ttol(cXMLConfigurator.GetElemData());

	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());

	// mask enable1
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable1 = _ttoi(cXMLConfigurator.GetElemData());

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


	// train affine rect1
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("CenterX1"));
	if (!bRet)
	{
		return false;
	}	
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("CenterY1"));
	if (!bRet)
	{
		return false;
	}	
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_TrainAffineRect1.SetCenter(vPos);

	bRet = cXMLConfigurator.FindElem(_T("XLength1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("YLength1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("XRotation1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));

	bRet = cXMLConfigurator.FindElem(_T("Skew1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
	cXMLConfigurator.LeaveElem();


	// model point1
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
	bRet = cXMLConfigurator.FindElem(_T("CenterX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("CenterY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_ModelPoint1 = vPos;

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

	// search rect1
	bRet = cXMLConfigurator.FindElem(_T("SearchRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
//	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect1.GetOrigin().GetX());
	bRet = cXMLConfigurator.FindElem(_T("OriginX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("OriginY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_SearchRect1.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("SizeX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("SizeY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_SearchRect1.SetSize(vPos);
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

	bRet = cXMLConfigurator.FindElem(_T("AcceptThre2"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAcceptThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);

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

	bRet = cXMLConfigurator.FindElem(_T("SizeLow"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		tmpSearchDataParam.m_dSizeLow = 1.0;
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeHigh"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		tmpSearchDataParam.m_dSizeHigh = 1.0;
	}

	bRet = cXMLConfigurator.FindElem(_T("PosIndex"));
	if (bRet)
	{
		tmpSearchDataParam.m_nPosIndex = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		tmpSearchDataParam.m_nPosIndex = 0;
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


	/////////////TrainResult1//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainResult1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpIImageModel;
	cpImage tmpImageMask;
	cpImage tmpImageMask1;
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

	//////////////////////////读取掩模图像1//////////////////////////
	if (tmpTrainDataParam.m_bMaskEnable1)
	{
		hStatus = cDIB.Init(strDir + _T("Mask1.bmp"));
		if (!IsSuccess(hStatus))
		{
			tmpTrainDataParam.m_bMaskEnable1 = false;
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask1);
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_bMaskEnable1 = false;
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
	if (tmpTrainDataParam.m_bMaskEnable1)
	{
		m_tmpImageMask1 = tmpImageMask1;
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
	m_ImageMask1		= tmpImageMask1;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CCombinedQuickSearch::SaveModelToFile(CString strDir)
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

	// grau1
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau1);
	cXMLConfigurator.AddElem(_T("Grau1"), strInfo);
	// noise thre1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre1);
	cXMLConfigurator.AddElem(_T("NoiseThre1"), strInfo);
	// total mag1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag1);
	cXMLConfigurator.AddElem(_T("TotalMag1"), strInfo);

	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);
	// mask enable1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable1);
	cXMLConfigurator.AddElem(_T("MaskEnable1"), strInfo);

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


	// train affine rect1
	cXMLConfigurator.AddElem(_T("TrainAffineRect1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetXRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetSkew().ToDouble());
	cXMLConfigurator.AddElem(_T("Skew1"), strInfo);
	cXMLConfigurator.LeaveElem();
	// model point1
	cXMLConfigurator.AddElem(_T("ModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY1"), strInfo);
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

	// search rect1
	cXMLConfigurator.AddElem(_T("SearchRect1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY1"), strInfo);
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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre2);
	cXMLConfigurator.AddElem(_T("AcceptThre2"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeLow);
	cXMLConfigurator.AddElem(_T("SizeLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeHigh);
	cXMLConfigurator.AddElem(_T("SizeHigh"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nPosIndex);
	cXMLConfigurator.AddElem(_T("PosIndex"), strInfo);

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

	/////////////TrainResult1//////////////
	cXMLConfigurator.AddElem(_T("TrainResult1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX1);
	cXMLConfigurator.AddElem(_T("ModelX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY1);
	cXMLConfigurator.AddElem(_T("ModelY1"), strInfo);
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
	//////////////////////////存储掩模图像1//////////////////////////
	if (m_TrainDataParam.m_bMaskEnable1 && m_ImageMask1.IsValid())
	{
		hStatus = cDIB.Init(m_ImageMask1);
		if (!IsSuccess(hStatus))
		{
			return false;
		}
		hStatus = cDIB.Write(strDir + _T("Mask1.bmp"));
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
bool CCombinedQuickSearch::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint1		= m_GuiModelPoint.GetCenter();

		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_TrainAffineRect1	= m_GuiTrainRect1.GetAffineRect();

		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_SearchRect1		= m_GuiSearchRect1.GetRect();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

//		m_GuiModelPoint1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
// 		m_GuiModelPoint1.SetLineColor(RGB(255, 0, 0));
// 		m_GuiModelPoint1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
// 		m_GuiModelPoint1.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint1, sc2Vector(20,20));

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

//		m_GuiTrainRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect1.SetLineColor(RGB(255, 0, 0));
		m_GuiTrainRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect1.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect1);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiSearchRect1.SetLineColor(RGB(255, 0, 0));
		m_GuiSearchRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect1.SetRect(m_tmpTrainGuiParam.m_SearchRect1);
	}

	return true;
}

bool CCombinedQuickSearch::UpdateSearchGuiData(bool bSaveAndValidate)
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
bool CCombinedQuickSearch::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CCombinedQuickSearch::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CCombinedQuickSearch::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	return true;
}

bool CCombinedQuickSearch::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen, rot,skew;

	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
	m_GuiTrainRect1.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect1.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//m_GuiModelPoint.SetLabel(_T("参考点"));


	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CCombinedQuickSearch::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect1"));		
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain2"));
	}
	return true;
}

bool CCombinedQuickSearch::ClearResultGui()
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
bool CCombinedQuickSearch::GetPatternImage(cpImage &ImagePattern)
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

// 获取训练模板原始图像
bool CCombinedQuickSearch::GetPatternImage2(cpImage &ImagePattern)
{
	/*bool bRet = m_QuickSearchTool1.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}

	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}

	if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool1.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 恢复训练参数
bool CCombinedQuickSearch::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CCombinedQuickSearch::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CCombinedQuickSearch::SetLanguage(int nLanguage)
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
		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
//		m_GuiModelPoint1.SetLabel(_T("Check Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiSearchRect1.SetLabel(_T("检查搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
//		m_GuiModelPoint1.SetLabel(_T("辅助参考点"));
		break;
	}
}
CString	CCombinedQuickSearch::GetResultErrInfo()
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
			case -2:							//检查失败
				strErrInfo += _T("Check failed");
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
			case -2:							//检查失败
				strErrInfo += _T("检查失败");
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
			case -2:							//检查失败
				strErrInfo += _T("检查失败");
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