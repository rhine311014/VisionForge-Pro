// QuickSearchCheckLineTool.cpp: implementation of the CQuickSearchCheckLineTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickSearchCheckLineTool.h"

#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 
#include "svTimer.h"


CQuickSearchCheckLineTool::CQuickSearchCheckLineTool()
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(230, 240, 50, 50, 0, 0);
//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect2.SetCenterLengthsRotationSkew(500, 240, 50, 50, 0, 0);
//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,255,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,255,0));

	m_GuiFindLine.SetStartXYEndXY(120, 320, 120, 170);
	m_GuiFindLine.SetLabelVisible(TRUE);

	m_GuiFindLine1.SetStartXYEndXY(170, 120, 310, 120);
	m_GuiFindLine1.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet1.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet1.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine1.SetCaliperVisible(FALSE);
	m_GuiFindLine1.SetAffineRectFindLineVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiFindLine1.SetLabel(_T("FindLine1"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
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
	m_tmpTrainDataParam.m_bEnableCheckRegion1	= true;
	m_tmpTrainDataParam.m_bEnableCheckRegion2	= false;

	m_FindLineTool.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, 
		m_tmpTrainDataParam.m_lFilterHalfWidth);
	m_tmpTrainDataParam.m_nOutLineNum			= 0;
	m_tmpTrainDataParam.m_bSortByScore			= true;
	m_tmpTrainDataParam.m_nLineAngleType		= 0;

	m_FindLineTool1.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1, 
		m_tmpTrainDataParam.m_lFilterHalfWidth1);
	m_tmpTrainDataParam.m_nOutLineNum1			= 0;
	m_tmpTrainDataParam.m_bSortByScore1			= true;
	m_tmpTrainDataParam.m_nLineAngleType1		= 0;

	m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_TrainAffineRect1	= m_GuiTrainRect1.GetAffineRect();
	m_tmpTrainGuiParam.m_TrainAffineRect2	= m_GuiTrainRect2.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint1		= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint2		= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();

	m_tmpTrainGuiParam.m_nCaliperNum			= m_GuiFindLine.GetCaliperNum();
	m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX,m_tmpTrainGuiParam.m_dEndY);
	m_tmpTrainGuiParam.m_dCaliperProjectLen		= m_GuiFindLine.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bEnableLineMask		= false;

	m_tmpTrainGuiParam.m_nCaliperNum1			= m_GuiFindLine1.GetCaliperNum();
	m_GuiFindLine1.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX1, m_tmpTrainGuiParam.m_dStartY1, m_tmpTrainGuiParam.m_dEndX1,
		m_tmpTrainGuiParam.m_dEndY1);
	m_tmpTrainGuiParam.m_dCaliperProjectLen1		= m_GuiFindLine1.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCaliperSearchLen1		= m_GuiFindLine1.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CaliperSearchRotation1	= m_GuiFindLine1.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bEnableLineMask1		= false;

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAcceptThre2		= m_QuickSearchTool1.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;
	m_tmpSearchDataParam.m_dSizeLow			= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh		= 1.0;

	m_tmpSearchGuiParam.m_nLineWidth		= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor			= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;
}

CQuickSearchCheckLineTool::CQuickSearchCheckLineTool(CQuickSearchCheckLineTool &cQuickSearchCheckLineTool)
{
	if (cQuickSearchCheckLineTool.GetSearchToolType() != eQuickSearchCheckLineTool)
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(230, 240, 50, 50, 0, 0);
	//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect2.SetCenterLengthsRotationSkew(500, 240, 50, 50, 0, 0);
	//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,255,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,255,0));

	m_GuiFindLine.SetStartXYEndXY(120, 320, 120, 170);
	m_GuiFindLine.SetLabelVisible(TRUE);

	m_GuiFindLine1.SetStartXYEndXY(170, 120, 310, 120);
	m_GuiFindLine1.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet1.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet1.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine1.SetCaliperVisible(FALSE);
	m_GuiFindLine1.SetAffineRectFindLineVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiFindLine1.SetLabel(_T("FindLine1"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	}

	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cQuickSearchCheckLineTool.m_CamHeight;
	m_CamWidth								= cQuickSearchCheckLineTool.m_CamWidth;

	m_TrainDataParam						= cQuickSearchCheckLineTool.m_TrainDataParam;
	m_TrainGuiParam							= cQuickSearchCheckLineTool.m_TrainGuiParam;
	m_SearchDataParam						= cQuickSearchCheckLineTool.m_SearchDataParam;
	m_SearchGuiParam						= cQuickSearchCheckLineTool.m_SearchGuiParam;
	m_TrainResult                           = cQuickSearchCheckLineTool.m_TrainResult;
	m_ImageTrain							= cQuickSearchCheckLineTool.m_ImageTrain;
	m_ImageMask								= cQuickSearchCheckLineTool.m_ImageMask;
	m_bTrainGuiShow							= cQuickSearchCheckLineTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cQuickSearchCheckLineTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cQuickSearchCheckLineTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cQuickSearchCheckLineTool.m_bTrainOK;
	m_pGuiDisplay							= cQuickSearchCheckLineTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cQuickSearchCheckLineTool.UpdateSearchGuiData(true);
	cQuickSearchCheckLineTool.UpdateTrainGuiData(true);

	SetTrainDataParam(&cQuickSearchCheckLineTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cQuickSearchCheckLineTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cQuickSearchCheckLineTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cQuickSearchCheckLineTool.m_tmpSearchGuiParam);
	SetTrainImage(cQuickSearchCheckLineTool.m_tmpImageTrain);
	SetMaskImage(cQuickSearchCheckLineTool.m_tmpImageMask);

	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return;
		}
		if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
		{
			return;
		}
		if (!m_QuickSearchTool2.SetGranularity(m_TrainDataParam.m_dGrau1))
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
		if (!m_QuickSearchTool2.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
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
		if (!m_QuickSearchTool2.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
		{
			return;
		}

		//// 设置掩模图像
		//if (m_TrainDataParam.m_bMaskEnable)
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(), m_TrainGuiParam.m_ModelPoint.GetY());
		//	if (m_TrainDataParam.m_bEnableCheckRegion1)
		//	{
		//		bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
		//	}
		//	if (m_TrainDataParam.m_bEnableCheckRegion2)
		//	{
		//		bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
		//	}
		//	m_bTrainOK = bRet;
		//}
		//else
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(), m_TrainGuiParam.m_ModelPoint.GetY());
		//	if (m_TrainDataParam.m_bEnableCheckRegion1)
		//	{
		//		bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
		//	}
		//	if (m_TrainDataParam.m_bEnableCheckRegion2)
		//	{
		//		bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
		//	}			
		//	m_bTrainOK = bRet;
		//}

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
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			m_bTrainOK = bRet;
		}
	}		
}

CQuickSearchCheckLineTool& CQuickSearchCheckLineTool::operator=(CQuickSearchCheckLineTool& cQuickSearchCheckLineTool)
{
	if (this == &cQuickSearchCheckLineTool)
	{
		return *this;
	}

	if (cQuickSearchCheckLineTool.GetSearchToolType() != eQuickSearchCheckLineTool)
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect1.SetCenterLengthsRotationSkew(230, 240, 50, 50, 0, 0);
	//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,255,0));

	m_GuiTrainRect2.SetCenterLengthsRotationSkew(500, 240, 50, 50, 0, 0);
	//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,255,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,255,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,255,0));

	m_GuiFindLine.SetStartXYEndXY(120, 320, 120, 170);
	m_GuiFindLine.SetLabelVisible(TRUE);

	m_GuiFindLine1.SetStartXYEndXY(170, 120, 310, 120);
	m_GuiFindLine1.SetLabelVisible(TRUE);
	
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	m_GuiTrainFeaturelet1.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet1.SetLineWidth(2);
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine1.SetCaliperVisible(FALSE);
	m_GuiFindLine1.SetAffineRectFindLineVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiFindLine1.SetLabel(_T("FindLine1"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	}

	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cQuickSearchCheckLineTool.m_CamHeight;
	m_CamWidth								= cQuickSearchCheckLineTool.m_CamWidth;

	m_TrainDataParam						= cQuickSearchCheckLineTool.m_TrainDataParam;
	m_TrainGuiParam							= cQuickSearchCheckLineTool.m_TrainGuiParam;
	m_SearchDataParam						= cQuickSearchCheckLineTool.m_SearchDataParam;
	m_SearchGuiParam						= cQuickSearchCheckLineTool.m_SearchGuiParam;
	m_TrainResult							= cQuickSearchCheckLineTool.m_TrainResult;
	m_ImageTrain							= cQuickSearchCheckLineTool.m_ImageTrain;
	m_ImageMask								= cQuickSearchCheckLineTool.m_ImageMask;

	m_bTrainGuiShow							= cQuickSearchCheckLineTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cQuickSearchCheckLineTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cQuickSearchCheckLineTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cQuickSearchCheckLineTool.m_bTrainOK;
	m_pGuiDisplay							= cQuickSearchCheckLineTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cQuickSearchCheckLineTool.UpdateSearchGuiData(true);
	cQuickSearchCheckLineTool.UpdateTrainGuiData(true);

	SetTrainDataParam(&cQuickSearchCheckLineTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cQuickSearchCheckLineTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cQuickSearchCheckLineTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cQuickSearchCheckLineTool.m_tmpSearchGuiParam);
	SetTrainImage(cQuickSearchCheckLineTool.m_tmpImageTrain);
	SetMaskImage(cQuickSearchCheckLineTool.m_tmpImageMask);

	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return *this;
		}
		if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
		{
			return *this;
		}
		if (!m_QuickSearchTool2.SetGranularity(m_TrainDataParam.m_dGrau1))
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
		if (!m_QuickSearchTool2.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
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
		if (!m_QuickSearchTool2.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
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
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			else
			{
				bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
				if (m_TrainDataParam.m_bEnableCheckRegion1)
				{
					bRet = bRet && m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(), m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				if (m_TrainDataParam.m_bEnableCheckRegion2)
				{
					bRet = bRet && m_QuickSearchTool2.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(), m_TrainGuiParam.m_ModelPoint2.GetY());
				}
			}
			m_bTrainOK = bRet;
		}
	}

	return *this;
}

CQuickSearchCheckLineTool::~CQuickSearchCheckLineTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CQuickSearchCheckLineTool::GetSearchToolType()
{
	return eQuickSearchCheckLineTool;
}										   

// 设置显示控件指针		
bool CQuickSearchCheckLineTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CQuickSearchCheckLineTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
bool CQuickSearchCheckLineTool::SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay1 = pGuiDisplay;
	m_hTrainResultDispWnd1 = NULL;
	if(m_pGuiTrainResultDisplay1)
	{
		m_hTrainResultDispWnd1 = m_pGuiTrainResultDisplay1->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}

// 设置训练结果显示控件指针
bool CQuickSearchCheckLineTool::SetTrainResultDisplay3(scGuiDisplay *pGuiDisplay)
{
	ClearAllGui();

	m_pGuiTrainResultDisplay2 = pGuiDisplay;
	m_hTrainResultDispWnd2 = NULL;
	if(m_pGuiTrainResultDisplay2)
	{
		m_hTrainResultDispWnd2 = m_pGuiTrainResultDisplay2->GetSafeHwnd();
	}

	UpdateTrainGuiDisplay();
	return true;
}

// 获取显示控件指针										
scGuiDisplay* CQuickSearchCheckLineTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickSearchCheckLineTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickSearchCheckLineTool::GetTrainResultDisplay2()
{
	return m_pGuiTrainResultDisplay1;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickSearchCheckLineTool::GetTrainResultDisplay3()
{
	return m_pGuiTrainResultDisplay2;
}

// 设置Gui图形的关键字														
bool CQuickSearchCheckLineTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CQuickSearchCheckLineTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CQuickSearchCheckLineTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CQuickSearchCheckLineTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}

// 更新搜索界面显示									
bool CQuickSearchCheckLineTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CQuickSearchCheckLineTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiTrainRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible && m_tmpTrainDataParam.m_bEnableCheckRegion1);
	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible && m_tmpTrainDataParam.m_bEnableCheckRegion2);

	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);

	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	m_GuiFindLine.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible && m_tmpTrainDataParam.m_bEnableCheckRegion1);
	m_GuiFindLine1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible && m_tmpTrainDataParam.m_bEnableCheckRegion2);

	if (m_tmpTrainGuiParam.m_bEnableLineMask)
	{
		m_GuiFindLine.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
	}
	else
	{
		m_GuiFindLine.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
	}


	if (m_tmpTrainGuiParam.m_bEnableLineMask1)
	{
		m_GuiFindLine1.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
	}
	else
	{
		m_GuiFindLine1.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect1, m_strKeyText+_T("_CheckTrainRect1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect2, m_strKeyText+_T("_CheckTrainRect2"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine, m_strKeyText+_T("_FindLineTool"));	
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine1, m_strKeyText+_T("_FindLineTool1"));	

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
		bool bflag=m_QuickSearchTool.GetTrainFeatures(tmpscFLCS);
		if(bflag)
		{
			m_GuiTrainFeaturelet.SetFeaturelet(tmpscFLCS);
			m_GuiTrainFeaturelet.SetVisible(TRUE);

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
			m_GuiTrainFeaturelet1.SetFeaturelet(tmpscFLCS);
			m_GuiTrainFeaturelet1.SetVisible(TRUE);

			if(m_pGuiTrainResultDisplay1 && ::IsWindow(m_hTrainResultDispWnd1))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay1->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet1,m_strKeyText+_T("_FeatureChain2"));

				m_pGuiTrainResultDisplay1->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay1->MyInvalidate();
			}
		}
	}

	{
		scFeatureletChainSet tmpscFLCS;
		bool bflag=m_QuickSearchTool2.GetTrainFeatures(tmpscFLCS);
		if(bflag)
		{
			m_GuiTrainFeaturelet2.SetFeaturelet(tmpscFLCS);
			m_GuiTrainFeaturelet2.SetVisible(TRUE);

			if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet2,m_strKeyText+_T("_FeatureChain3"));

				m_pGuiTrainResultDisplay2->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay2->MyInvalidate();
			}
		}
	}
	return true;
}

// 更新训练结果Gui控件显示
bool CQuickSearchCheckLineTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine.SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine1.SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine, m_strKeyText+_T("_TrainResultLine"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine1, m_strKeyText+_T("_TrainResultLine1"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}				
// 更新搜索界面显示									
bool CQuickSearchCheckLineTool::UpdateSearchResultGuiDisplay()							
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

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchLineArray1.size(); i++)
	{
		m_pGuiSearchLineArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray1.size(); i++)
	{
		m_pGuiSearchLineCrossArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary1_1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary1_1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary2_1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary2_1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array_1.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array_1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint1);
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

		for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary1[i], m_pGuiSearchCrossArrayAuxilary1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary2[i], m_pGuiSearchCrossArrayAuxilary2[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array[i], m_pGuiSearchResultFitPoint1Array[i]->GetTipText());
		}


		for (i=0; i<m_pGuiSearchLineArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray1[i], m_pGuiSearchLineArray1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchLineCrossArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray1[i], m_pGuiSearchLineCrossArray1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArrayAuxilary1_1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary1_1[i], m_pGuiSearchCrossArrayAuxilary1_1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArrayAuxilary2_1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary2_1[i], m_pGuiSearchCrossArrayAuxilary2_1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultFitPoint1Array_1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array_1[i], m_pGuiSearchResultFitPoint1Array_1[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

bool CQuickSearchCheckLineTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CQuickSearchCheckLineTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CQuickSearchCheckLineTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CQuickSearchCheckLineTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	*((CQuickSearchCheckLineTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CQuickSearchCheckLineTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CQuickSearchCheckLineTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CQuickSearchCheckLineTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CQuickSearchCheckLineTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CQuickSearchCheckLineTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CQuickSearchCheckLineTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CQuickSearchCheckLineTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CQuickSearchCheckLineTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像1									
bool CQuickSearchCheckLineTool::SetMaskImage2(const cpImage &ImageMask)
{
	return true;
}

// 获取训练原始图像1								
bool CQuickSearchCheckLineTool::GetMaskImage2(cpImage &ImageMask)
{
	return true;
}

// 执行训练											
bool CQuickSearchCheckLineTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置训练参数
	if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetGranularity(m_tmpTrainDataParam.m_dGrau1))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetGranularity(m_tmpTrainDataParam.m_dGrau1))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre1))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre1))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag1))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag1))
	{
		m_nResultErrNumber = -3;
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
			m_nResultErrNumber=-1;//几何定位失败
			return false;
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion1)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_QuickSearchTool1.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = bRet && m_QuickSearchTool1.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-2;//几何检查工具1失败
				return false;
			}
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion2)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_QuickSearchTool2.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(), m_tmpTrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = bRet && m_QuickSearchTool2.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(), m_tmpTrainGuiParam.m_ModelPoint2.GetY());
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-3;//几何检查工具2失败
				return false;
			}
		}

		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			return m_bTrainOK;
		}

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
		if (!bRet)
		{
			m_nResultErrNumber=-1;//几何定位失败
			return false;
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion1)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_QuickSearchTool1.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
			}
			else
			{
				bRet = bRet && m_QuickSearchTool1.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(), m_tmpTrainGuiParam.m_ModelPoint1.GetY());
			}
			if (!bRet)
			{
				m_nResultErrNumber=-2;//几何检查工具1失败
				return false;
			}
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion2)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_QuickSearchTool2.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(), m_tmpTrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = bRet && m_QuickSearchTool2.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(), m_tmpTrainGuiParam.m_ModelPoint2.GetY());
			}
			if (!bRet)
			{
				m_nResultErrNumber=-3;//几何检查工具2失败
				return false;
			}
		}

		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			return m_bTrainOK;
		}

	}

	if (m_bTrainOK)
	{
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		if (m_tmpTrainDataParam.m_bMaskEnable)
		{
			m_ImageMask      = m_tmpImageMask;
		}

		if (m_tmpTrainDataParam.m_bEnableCheckRegion1)
		{
			// 找线
			m_FindLineTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
			m_FindLineTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchRotation, m_tmpTrainGuiParam.m_dCaliperSearchLen
				,m_tmpTrainGuiParam.m_dCaliperProjectLen);
		//	m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
			m_FindLineTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum, eRansacRobust);
			m_FindLineTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
			m_FindLineTool.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX, m_tmpTrainGuiParam.m_dEndY);
			m_FindLineTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine);
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
				m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
			} 
			else
			{
				m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
			}

			scLineSeg cLineSeg;
			double dRMS;
			if(bIsProcessImage)
			{
				m_bTrainOK = m_FindLineTool.Execute(tmpImage, cLineSeg, dRMS);
			}
			else
			{
				m_bTrainOK = m_FindLineTool.Execute(m_tmpImageTrain, cLineSeg, dRMS);
			}
			
			if (!m_bTrainOK)
			{
				m_nResultErrNumber=-4;//找线工具1失败
				return false;
			}

			if (m_tmpTrainDataParam.m_nLineAngleType == 0)	// 任意角度
			{
				m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
				m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
				m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
				m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType == 1)	//水平线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg.GetRotation()).SignedNorm();

				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
					m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
					m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
					m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX = cLineSeg.GetP2().GetX();
					m_tmpTrainResult.m_dStartY = cLineSeg.GetP2().GetY();
					m_tmpTrainResult.m_dEndX = cLineSeg.GetP1().GetX();
					m_tmpTrainResult.m_dEndY = cLineSeg.GetP1().GetY();
				}
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType == 2)	//垂直线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg.GetRotation()).Norm();

				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
					m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
					m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
					m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX = cLineSeg.GetP2().GetX();
					m_tmpTrainResult.m_dStartY = cLineSeg.GetP2().GetY();
					m_tmpTrainResult.m_dEndX = cLineSeg.GetP1().GetX();
					m_tmpTrainResult.m_dEndY = cLineSeg.GetP1().GetY();
				}
			}
			else
			{
				m_tmpTrainResult.m_dStartX = cLineSeg.GetP1().GetX();
				m_tmpTrainResult.m_dStartY = cLineSeg.GetP1().GetY();
				m_tmpTrainResult.m_dEndX = cLineSeg.GetP2().GetX();
				m_tmpTrainResult.m_dEndY = cLineSeg.GetP2().GetY();
			}

			m_GuiTrainResultLine.SetLine(cLineSeg.GetLine());
		}



		if (m_tmpTrainDataParam.m_bEnableCheckRegion2)
		{
			// 找线1
			m_FindLineTool1.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum1);
			m_FindLineTool1.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum1, m_tmpTrainGuiParam.m_CaliperSearchRotation1, m_tmpTrainGuiParam.m_dCaliperSearchLen1
				,m_tmpTrainGuiParam.m_dCaliperProjectLen1);
			//m_FindLineTool1.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1, m_tmpTrainDataParam.m_lFilterHalfWidth1);
			m_FindLineTool1.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum1, eRansacRobust);
			m_FindLineTool1.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore1);
			m_FindLineTool1.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX1, m_tmpTrainGuiParam.m_dStartY1, 
				m_tmpTrainGuiParam.m_dEndX1, m_tmpTrainGuiParam.m_dEndY1);
			m_FindLineTool1.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine1);
			if (m_tmpTrainDataParam.m_bEnableDualEdge1)
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeProperty1 == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeProperty1 == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool1.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth1);
			} 
			else
			{
				m_FindLineTool1.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1, m_tmpTrainDataParam.m_lFilterHalfWidth1);
			}
			scLineSeg cLineSeg1;
			double dRMS1;
			if(bIsProcessImage)
			{
				m_bTrainOK = m_FindLineTool1.Execute(tmpImage, cLineSeg1, dRMS1);
			}
			else
			{
				m_bTrainOK = m_FindLineTool1.Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
			}
			if (!m_bTrainOK)
			{
				m_nResultErrNumber=-5;//找线工具2失败
				return false;
			}

			if (m_tmpTrainDataParam.m_nLineAngleType1 == 0)	// 任意角度
			{
				m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP2().GetY();
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType1 == 1)	//水平线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();

				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP1().GetY();
				}
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType1 == 2)	//垂直线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();

				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP1().GetY();
				}
			}
			else
			{
				m_tmpTrainResult.m_dStartX1 = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY1 = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX1 = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY1 = cLineSeg1.GetP2().GetY();
			}

			m_GuiTrainResultLine1.SetLine(cLineSeg1.GetLine());
		}

		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;

	}

	return m_bTrainOK;
}

// 获取训练结果图像														
bool CQuickSearchCheckLineTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CQuickSearchCheckLineTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	if (!m_TrainDataParam.m_bEnableCheckRegion1)
	{
		return false;
	}

	bool bRet = m_QuickSearchTool1.GetTrainModelImage(m_ImageTrainResult1);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult1;

	return true;
}	

// 获取训练结果图像														
bool CQuickSearchCheckLineTool::GetTrainResultImage3(cpImage &ImageTrainResult)
{
	if (!m_TrainDataParam.m_bEnableCheckRegion2)
	{
		return false;
	}

	bool bRet = m_QuickSearchTool2.GetTrainModelImage(m_ImageTrainResult2);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult2;

	return true;
}	

// 获取训练结果
bool CQuickSearchCheckLineTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	*((CQuickSearchCheckLineTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}						

// 是否训练成功
bool CQuickSearchCheckLineTool::IsTrained()
{
	return m_bTrainOK;
}															

// 设置搜索Data参数	
bool CQuickSearchCheckLineTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CQuickSearchCheckLineSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CQuickSearchCheckLineTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	*((CQuickSearchCheckLineSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CQuickSearchCheckLineTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CQuickSearchCheckLineSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CQuickSearchCheckLineTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickSearchCheckLineTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CQuickSearchCheckLineSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

bool CQuickSearchCheckLineTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;

	// 设置搜索个数
	if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber = -3;
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber = -1;
		return false;
	}
	if (!m_QuickSearchTool1.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber = -2;
		return false;
	}
	if (!m_QuickSearchTool2.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber = -3;
		return false;
	}


	m_FindLineTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
	m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchRotation, m_TrainGuiParam.m_dCaliperSearchLen
		,m_TrainGuiParam.m_dCaliperProjectLen);
	//m_FindLineTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
	m_FindLineTool.SetOutLineNum(m_TrainDataParam.m_nOutLineNum, eRansacRobust);
	m_FindLineTool.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore);
	// 找线双极性
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
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
	} 
	else
	{
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
	}
	m_FindLineTool1.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum1);
	m_FindLineTool1.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum1, m_TrainGuiParam.m_CaliperSearchRotation1, m_TrainGuiParam.m_dCaliperSearchLen1
		,m_TrainGuiParam.m_dCaliperProjectLen1);
	//m_FindLineTool1.SetCaliperProperty(m_TrainDataParam.m_dContrastThre1, m_TrainDataParam.m_edgeProperty1, m_TrainDataParam.m_lFilterHalfWidth1);
	m_FindLineTool1.SetOutLineNum(m_TrainDataParam.m_nOutLineNum1, eRansacRobust);
	m_FindLineTool1.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore1);
	// 找线双极性
	if (m_tmpTrainDataParam.m_bEnableDualEdge1)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeProperty1 == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeProperty1 == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindLineTool1.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth1);
	} 
	else
	{
		m_FindLineTool1.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre1, m_tmpTrainDataParam.m_edgeProperty1, m_tmpTrainDataParam.m_lFilterHalfWidth1);
	}

	// 执行搜索
	bool bRetCheck1 = true;
	bool bRetCheck2 = true;
	CSearchResult searchResult0;
	CSearchResult searchResult1;
	CSearchResult searchResult2;
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
	if (!bRet)
	{
		m_nResultErrNumber=-1;//几何定位失败
		return false;
	}
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_QuickSearchTool.GetResultNum();
		if (nResultNum <= 0) 
		{
			m_nResultErrNumber=-1;///几何定位失败
			return false;
		}

		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);

			searchResult0.m_dPosX = tmpQuickResult.GetPosition().GetX();
			searchResult0.m_dPosY = tmpQuickResult.GetPosition().GetY();
			searchResult0.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
			searchResult0.m_dScore = tmpQuickResult.GetScore();

			CResultSearchRect resRect;
			resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
			searchResult0.m_vTrainSearchRect.push_back(resRect);

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


			//////////////////////////辅助矩形实时搜索区域计算1//////////////////////////////
			if (m_TrainDataParam.m_bEnableCheckRegion1)
			{
				// 搜索点
//				scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
				sc2Vector pos = tmpQuickResult.GetPosition();
				double dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
				// 训练点
				sc2Vector tmModelPoint;
				m_QuickSearchTool.GetModelOrigin(tmModelPoint);

				scAffineRect tmpTrainRect1 = m_QuickSearchTool1.GetTrainAffineRectRegion();
				sc2Vector tmTrainRectCenter1 = tmpTrainRect1.GetCenter();

				//旋转后搜索区域中心
				tmTrainRectCenter1 = tmTrainRectCenter1 - tmModelPoint;
				double dXRotated1 = cos(dRotation) * tmTrainRectCenter1.GetX() - sin(dRotation) * tmTrainRectCenter1.GetY() + pos.GetX();
				double dYRotated1 = sin(dRotation) * tmTrainRectCenter1.GetX() + cos(dRotation) * tmTrainRectCenter1.GetY() + pos.GetY();
				tmTrainRectCenter1 = sc2Vector(dXRotated1, dYRotated1);

				//旋转后搜索仿射区域
				scAffineRect tmAffineRect1;
				tmAffineRect1.SetCenter(tmTrainRectCenter1);
				tmAffineRect1.SetXLength(tmpTrainRect1.GetXLength()*1.3);
				tmAffineRect1.SetYLength(tmpTrainRect1.GetYLength()*1.1);
				tmAffineRect1.SetXRotation(tmpTrainRect1.GetXRotation() + scRadian(dRotation));
				tmAffineRect1.SetSkew(tmpTrainRect1.GetSkew());

				//旋转后搜索区域
				scRect tmpSearchRect1, SearchRect1;
				tmpSearchRect1 = tmAffineRect1.BoundingBox();
				SearchRect1 = tmpSearchRect1;
				if(bIsProcessImage)
				{
					bRetCheck1 = m_QuickSearchTool1.Execute(tmpImage, SearchRect1);
				}
				else
				{
					bRetCheck1 = m_QuickSearchTool1.Execute(ImageSearch, SearchRect1);
				}
				if (!bRetCheck1)
				{
					m_nResultErrNumber=-2;//几何检查工具1失败
					return false;
				}
				if (bRetCheck1)
				{
					// 获取结果
					int nResultNum1 = m_QuickSearchTool1.GetResultNum();
					if (nResultNum1 > 0)
					{
						for (int i=0; i<1 && i<MAXRESNUM; i++)
						{
							scQuickSearchResult tmpQuickResult1 = m_QuickSearchTool1.GetResult(i);
							sc2Vector pos1 = tmpQuickResult1.GetPosition();
							double dRotation1 = scRadian(tmpQuickResult1.GetRotate()).ToDouble();

							CResultSearchRect resRect;
							resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
							searchResult1.m_vTrainSearchRect.push_back(resRect);

							resRect.m_searchRect = 	tmpQuickResult1.MatchRegion();
							searchResult1.m_vACFSearchRect.push_back(resRect);

							sc2Vector tmModelPoint1;
							double dTransX;
							double dTransY;
							m_QuickSearchTool1.GetModelOrigin(tmModelPoint1);
							
							dTransX = pos1.GetX() - (cos(dRotation1) * tmModelPoint1.GetX()  - sin(dRotation1) * tmModelPoint1.GetY());
							dTransY = pos1.GetY() - (sin(dRotation1) * tmModelPoint1.GetX()  + cos(dRotation1) * tmModelPoint1.GetY());

							// 求找线工具变换后的位置,找线
							double dTransStartX, dTransStartY;
							double dTransEndX, dTransEndY;
							dTransStartX = cos(dRotation1) * m_TrainGuiParam.m_dStartX  - sin(dRotation1) * m_TrainGuiParam.m_dStartY + dTransX;
							dTransStartY = sin(dRotation1) * m_TrainGuiParam.m_dStartX  + cos(dRotation1) * m_TrainGuiParam.m_dStartY + dTransY;
							dTransEndX = cos(dRotation1) * m_TrainGuiParam.m_dEndX  - sin(dRotation1) * m_TrainGuiParam.m_dEndY + dTransX;
							dTransEndY = sin(dRotation1) * m_TrainGuiParam.m_dEndX  + cos(dRotation1) * m_TrainGuiParam.m_dEndY + dTransY;

							m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum,m_TrainGuiParam.m_CaliperSearchRotation, m_TrainGuiParam.m_dCaliperSearchLen
								,m_TrainGuiParam.m_dCaliperProjectLen);
							m_FindLineTool.SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
							m_FindLineTool.SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask_FindLine);

							// 执行找线
							scLineSeg ResLineSeg;
							double dRMS;
							if(bIsProcessImage)
							{
								bRet = m_FindLineTool.Execute(tmpImage, ResLineSeg, dRMS);
							}
							else
							{
								bRet = m_FindLineTool.Execute(ImageSearch, ResLineSeg, dRMS);
							}
							if (!bRet)
							{
								m_nResultErrNumber=-4;///找线工具1失败
								return false;
							}
							else //(m_FindLineTool.Execute(ImageSearch, ResLineSeg, dRMS)) // 成功
							{
//								CSearchResult searchResult;

								searchResult1.m_dPosX = (ResLineSeg.GetP1().GetX() + ResLineSeg.GetP2().GetX())/2;
								searchResult1.m_dPosY = (ResLineSeg.GetP1().GetY() + ResLineSeg.GetP2().GetY())/2;
								searchResult1.m_dScore = tmpQuickResult.GetScore();

								if (m_TrainDataParam.m_nLineAngleType == 0)	// 任意角度
								{
									searchResult1.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

									searchResult1.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
									searchResult1.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
									searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
									searchResult1.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
									searchResult1.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
									searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
								}
								else if (m_TrainDataParam.m_nLineAngleType == 1)	//水平线
								{
									searchResult1.m_dAngle = scDegree(ResLineSeg.GetRotation().SignedNormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg.GetRotation()).SignedNorm();
									if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
									{
										searchResult1.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
										searchResult1.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
									}
									else
									{
										searchResult1.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
										searchResult1.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
									}
								}
								else if (m_TrainDataParam.m_nLineAngleType == 2)	//垂直线
								{
									searchResult1.m_dAngle = scDegree(ResLineSeg.GetRotation().NormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg.GetRotation()).Norm();
									if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
									{
										searchResult1.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
										searchResult1.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
									}
									else
									{
										searchResult1.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
										searchResult1.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
										searchResult1.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
										searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
									}
								}
								else
								{
									searchResult1.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

									searchResult1.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
									searchResult1.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
									searchResult1.m_vdAuxiliaryAngle.at(0) = searchResult1.m_dAngle;
									searchResult1.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
									searchResult1.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
									searchResult1.m_vdAuxiliaryAngle.at(1) = searchResult1.m_dAngle;
								}

								CResultLine line;
								line.m_vLineX[0] =ResLineSeg.GetP1().GetX();
								line.m_vLineY[0] =ResLineSeg.GetP1().GetY();
								line.m_vLineX[1] =ResLineSeg.GetP2().GetX();
								line.m_vLineY[1] =ResLineSeg.GetP2().GetY();
								searchResult1.m_vLineSearchResult.push_back(line);
//								m_SearchResultArray.push_back(searchResult);

 								CString	strTmpKey;
// 								strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
// 								scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
// 								pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
// 								pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 								pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 								pTempAffineRect->SetTipTextEnabled(true);
// 								pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
// 								m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
// 
// 								strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
// 								scGuiCross *pTempCross = m_GuiSearchCross+i;
// 								pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
// 								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 								pTempCross->SetTipTextEnabled(true);
// 								pTempCross->SetTipText(strTmpKey);
// 								m_pGuiSearchCrossArray.push_back(pTempCross);

								strTmpKey.Format(_T("%s_Check1ResultRect%d"), m_strKeyText, i);
								scGuiAffineRect *pTempAffineRect1 = m_GuiSearchAffineRect1+i;
								pTempAffineRect1->SetAffineRect(tmpQuickResult1.MatchRegion());
								pTempAffineRect1->SetLineColor(RGB(255,0,0));
								pTempAffineRect1->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempAffineRect1->SetTipTextEnabled(true);
								pTempAffineRect1->SetTipText((LPCTSTR)strTmpKey);
								m_pGuiSearchAffineRectArray.push_back(pTempAffineRect1);

								strTmpKey.Format(_T("%s_Check1SearchRect%d"), m_strKeyText, i);
								scGuiRect  *pTempRect = m_GuiSearchRect1+i;
								pTempRect->SetRect(SearchRect1);
								pTempRect->SetLineColor(RGB(255,255,255));
								pTempRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempRect->SetTipTextEnabled(true);
								pTempRect->SetTipText((LPCTSTR)strTmpKey);
								m_pGuiSearchAffineRectArray.push_back(pTempRect);

								strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchLineCross+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult1.m_dPosX,searchResult1.m_dPosY), sc2Vector(60,60));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchLineCrossArray.push_back(pTempCross);


								strTmpKey.Format(_T("%s_ResultCrossAuxiliary1%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchCrossAuxilary1+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult1.m_vdAuxiliaryPosX.at(0),searchResult1.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary1.push_back(pTempCross);

								strTmpKey.Format(_T("%s_ResultCrossAuxiliary2%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchCrossAuxilary2+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult1.m_vdAuxiliaryPosX.at(1),searchResult1.m_vdAuxiliaryPosY.at(1)), sc2Vector(30,30));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary2.push_back(pTempCross);


								strTmpKey.Format(_T("%s_ResultLine%d"), m_strKeyText, i);
								scGuiLine *pTemLine = m_GuiSearchLine+i;
								pTemLine->SetXYRotation(sc2Vector(searchResult1.m_dPosX, searchResult1.m_dPosY), scDegree(searchResult1.m_dAngle));
								pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTemLine->SetTipTextEnabled(true);
								pTemLine->SetTipText(strTmpKey);

								m_pGuiSearchLineArray.push_back(pTemLine);


								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool.GetUsed(bUsed);
								m_FindLineTool.GetFitData(points);
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
										pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint1->SetTipTextEnabled(true);
									pTempFitPoint1->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPoint1Array.push_back(pTempFitPoint1);	
								}

							}	

						}	
					}
					else
					{
						m_nResultErrNumber=-2;///几何检查工具1失败
						bRetCheck1 = false;
					}
				}
			}


			if (m_TrainDataParam.m_bEnableCheckRegion2)
			{
				// 搜索点
//				scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
				sc2Vector pos = tmpQuickResult.GetPosition();
				double dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
				// 训练点
				sc2Vector tmModelPoint;
				m_QuickSearchTool.GetModelOrigin(tmModelPoint);

				scAffineRect tmpTrainRect2 = m_QuickSearchTool2.GetTrainAffineRectRegion();
				sc2Vector tmTrainRectCenter2 = tmpTrainRect2.GetCenter();

				//旋转后搜索区域中心
				tmTrainRectCenter2 = tmTrainRectCenter2 - tmModelPoint;
				double dXRotated2 = cos(dRotation) * tmTrainRectCenter2.GetX() - sin(dRotation) * tmTrainRectCenter2.GetY() + pos.GetX();
				double dYRotated2 = sin(dRotation) * tmTrainRectCenter2.GetX() + cos(dRotation) * tmTrainRectCenter2.GetY() + pos.GetY();
				tmTrainRectCenter2 = sc2Vector(dXRotated2, dYRotated2);

				//旋转后搜索仿射区域
				scAffineRect tmAffineRect2;
				tmAffineRect2.SetCenter(tmTrainRectCenter2);
				tmAffineRect2.SetXLength(tmpTrainRect2.GetXLength()*1.3);
				tmAffineRect2.SetYLength(tmpTrainRect2.GetYLength()*1.1);
				tmAffineRect2.SetXRotation(tmpTrainRect2.GetXRotation() + scRadian(dRotation));
				tmAffineRect2.SetSkew(tmpTrainRect2.GetSkew());

				//旋转后搜索区域
				scRect tmpSearchRect2, SearchRect2;
				tmpSearchRect2 = tmAffineRect2.BoundingBox();
				SearchRect2 = tmpSearchRect2;
				if(bIsProcessImage)
				{
					bRetCheck2 = m_QuickSearchTool2.Execute(tmpImage, SearchRect2);
				}
				else
				{
					bRetCheck2 = m_QuickSearchTool2.Execute(ImageSearch, SearchRect2);
				}
				if (!bRetCheck2)
				{
					m_nResultErrNumber=-3;///几何检查工具2失败
					return false;
				}
				if (bRetCheck2)
				{
					// 获取结果
					int nResultNum = m_QuickSearchTool2.GetResultNum();
					if (nResultNum > 0)
					{
						for (int i=0; i<1 && i<MAXRESNUM; i++)
						{
							scQuickSearchResult tmpQuickResult2 = m_QuickSearchTool2.GetResult(i);
							sc2Vector pos2 = tmpQuickResult2.GetPosition();							
							double dRotation2 = scRadian(tmpQuickResult2.GetRotate()).ToDouble();

							CResultSearchRect resRect;
							resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
							searchResult2.m_vTrainSearchRect.push_back(resRect);

							resRect.m_searchRect = 	tmpQuickResult2.MatchRegion();
							searchResult2.m_vACFSearchRect.push_back(resRect);


							double dTransX;
							double dTransY;
							sc2Vector tmModelPoint2;
							m_QuickSearchTool2.GetModelOrigin(tmModelPoint2);
							
							dTransX = pos2.GetX() - (cos(dRotation2) * tmModelPoint2.GetX()  - sin(dRotation2) * tmModelPoint2.GetY());
							dTransY = pos2.GetY() - (sin(dRotation2) * tmModelPoint2.GetX()  + cos(dRotation2) * tmModelPoint2.GetY());

							// 求找线工具变换后的位置,找线
							double dTransStartX, dTransStartY;
							double dTransEndX, dTransEndY;
							dTransStartX = cos(dRotation2) * m_TrainGuiParam.m_dStartX1  - sin(dRotation2) * m_TrainGuiParam.m_dStartY1 + dTransX;
							dTransStartY = sin(dRotation2) * m_TrainGuiParam.m_dStartX1  + cos(dRotation2) * m_TrainGuiParam.m_dStartY1 + dTransY;
							dTransEndX = cos(dRotation2) * m_TrainGuiParam.m_dEndX1  - sin(dRotation2) * m_TrainGuiParam.m_dEndY1 + dTransX;
							dTransEndY = sin(dRotation2) * m_TrainGuiParam.m_dEndX1  + cos(dRotation2) * m_TrainGuiParam.m_dEndY1 + dTransY;

							m_FindLineTool1.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum1, m_TrainGuiParam.m_CaliperSearchRotation1, m_TrainGuiParam.m_dCaliperSearchLen1
								,m_TrainGuiParam.m_dCaliperProjectLen1);
							m_FindLineTool1.SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
							m_FindLineTool1.SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask_FindLine1);

							// 执行找线
							scLineSeg ResLineSeg;
							double dRMS;
							if(bIsProcessImage)
							{
								bRet = m_FindLineTool1.Execute(tmpImage, ResLineSeg, dRMS);
							}
							else
							{
								bRet = m_FindLineTool1.Execute(ImageSearch, ResLineSeg, dRMS);
							}
							if (!bRet) // 成功
							{
								m_nResultErrNumber=-5;///找线工具2失败
								return false;
							}
							else
							{
//								CSearchResult searchResult;

								searchResult2.m_dPosX = (ResLineSeg.GetP1().GetX() + ResLineSeg.GetP2().GetX())/2;
								searchResult2.m_dPosY = (ResLineSeg.GetP1().GetY() + ResLineSeg.GetP2().GetY())/2;
								searchResult2.m_dScore = tmpQuickResult.GetScore();

								if (m_TrainDataParam.m_nLineAngleType1 == 0)	// 任意角度
								{
									searchResult2.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

									searchResult2.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
									searchResult2.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
									searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
									searchResult2.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
									searchResult2.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
									searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
								}
								else if (m_TrainDataParam.m_nLineAngleType1 == 1)	//水平线
								{
									searchResult2.m_dAngle = scDegree(ResLineSeg.GetRotation().SignedNormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg.GetRotation()).SignedNorm();
									if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
									{
										searchResult2.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
										searchResult2.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
									}
									else
									{
										searchResult2.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
										searchResult2.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
									}
								}
								else if (m_TrainDataParam.m_nLineAngleType1 == 2)	//垂直线
								{
									searchResult2.m_dAngle = scDegree(ResLineSeg.GetRotation().NormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg.GetRotation()).Norm();
									if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
									{
										searchResult2.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
										searchResult2.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
									}
									else
									{
										searchResult2.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
										searchResult2.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
										searchResult2.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
										searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
									}
								}
								else
								{
									searchResult2.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

									searchResult2.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
									searchResult2.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
									searchResult2.m_vdAuxiliaryAngle.at(0) = searchResult2.m_dAngle;
									searchResult2.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
									searchResult2.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
									searchResult2.m_vdAuxiliaryAngle.at(1) = searchResult2.m_dAngle;
								}

								CResultLine line;
								line.m_vLineX[0] =ResLineSeg.GetP1().GetX();
								line.m_vLineY[0] =ResLineSeg.GetP1().GetY();
								line.m_vLineX[1] =ResLineSeg.GetP2().GetX();
								line.m_vLineY[1] =ResLineSeg.GetP2().GetY();
								searchResult2.m_vLineSearchResult.push_back(line);

//								m_SearchResultArray.push_back(searchResult);

								CString	strTmpKey;
// 								strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
// 								scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
// 								pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
// 								pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 								pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 								pTempAffineRect->SetTipTextEnabled(true);
// 								pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
// 								m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

// 								strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
// 								scGuiCross *pTempCross = m_GuiSearchCross+i;
// 								pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
// 								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 								pTempCross->SetTipTextEnabled(true);
// 								pTempCross->SetTipText(strTmpKey);
// 								m_pGuiSearchCrossArray.push_back(pTempCross);

								strTmpKey.Format(_T("%s_Check2ResultRect%d"), m_strKeyText, i);
								scGuiAffineRect *pTempAffineRect1 = m_GuiSearchAffineRect2+i;
								pTempAffineRect1->SetAffineRect(tmpQuickResult2.MatchRegion());
								pTempAffineRect1->SetLineColor(RGB(255,0,0));
								pTempAffineRect1->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempAffineRect1->SetTipTextEnabled(true);
								pTempAffineRect1->SetTipText((LPCTSTR)strTmpKey);
								m_pGuiSearchAffineRectArray.push_back(pTempAffineRect1);

								strTmpKey.Format(_T("%s_Check2SearchRect%d"), m_strKeyText, i);
								scGuiRect  *pTempRect = m_GuiSearchRect2+i;
								pTempRect->SetRect(SearchRect2);
								pTempRect->SetLineColor(RGB(255,255,255));
								pTempRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempRect->SetTipTextEnabled(true);
								pTempRect->SetTipText((LPCTSTR)strTmpKey);
								m_pGuiSearchAffineRectArray.push_back(pTempRect);

								strTmpKey.Format(_T("%s_ResultLineCross1_%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchLineCross1+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult2.m_dPosX,searchResult2.m_dPosY), sc2Vector(60,60));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchLineCrossArray1.push_back(pTempCross);


								strTmpKey.Format(_T("%s_ResultCrossAuxiliary1_1%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchCrossAuxilary1_1+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult2.m_vdAuxiliaryPosX.at(0),searchResult2.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary1_1.push_back(pTempCross);

								strTmpKey.Format(_T("%s_ResultCrossAuxiliary2_1%d"), m_strKeyText, i);
								pTempCross = m_GuiSearchCrossAuxilary2_1+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult2.m_vdAuxiliaryPosX.at(1),searchResult2.m_vdAuxiliaryPosY.at(1)), sc2Vector(30,30));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary2_1.push_back(pTempCross);


								strTmpKey.Format(_T("%s_ResultLine1_%d"), m_strKeyText, i);
								scGuiLine *pTemLine = m_GuiSearchLine1+i;
								pTemLine->SetXYRotation(sc2Vector(searchResult2.m_dPosX, searchResult2.m_dPosY), scDegree(searchResult2.m_dAngle));
								pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTemLine->SetTipTextEnabled(true);
								pTemLine->SetTipText(strTmpKey);

								m_pGuiSearchLineArray1.push_back(pTemLine);

								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool1.GetUsed(bUsed);
								m_FindLineTool1.GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPoint1Array_1.size();
									strTmpKey.Format(_T("%s_FitPoint1_1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint1_1 + nIndex;
									pTempFitPoint1->SetXDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint1->SetYDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint1->SetRulerVisible(false);
									pTempFitPoint1->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
									if (bUse)
									{
										pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint1->SetTipTextEnabled(true);
									pTempFitPoint1->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPoint1Array_1.push_back(pTempFitPoint1);	
								}
							}	

						}	
					}
					else
					{
						m_nResultErrNumber=-3;//几何检查工2失败
						bRetCheck2 = false;
					}
				}
			}
		}	


		if (m_TrainDataParam.m_bEnableCheckRegion1 && m_TrainDataParam.m_bEnableCheckRegion2)
		{
			if (bRet)
			{
				if(bRetCheck1 && bRetCheck2)
				{
								
					double dAngle = (searchResult1.m_dAngle + searchResult2.m_dAngle)/2.0;
					scLine TempLine(sc2Vector(searchResult1.m_dPosX, searchResult1.m_dPosY), scRadian(scDegree(dAngle)));					
					sc2Vector AuxiliaryPos = TempLine.Project(sc2Vector(searchResult1.m_vdAuxiliaryPosX.at(1),searchResult1.m_vdAuxiliaryPosY.at(1)));

					CSearchResult searchResult;	
					searchResult.m_dPosX = searchResult1.m_dPosX;
					searchResult.m_dPosY = searchResult1.m_dPosY;
					searchResult.m_dAngle = dAngle;
					searchResult.m_dScore = (searchResult1.m_dScore + searchResult1.m_dScore) / 2.0;

					searchResult.m_vdAuxiliaryPosX.at(0) = searchResult1.m_vdAuxiliaryPosX.at(0);
					searchResult.m_vdAuxiliaryPosY.at(0) = searchResult1.m_vdAuxiliaryPosY.at(0);
					searchResult.m_vdAuxiliaryAngle.at(0) = dAngle;
					searchResult.m_vdAuxiliaryPosX.at(1) = AuxiliaryPos.GetX();
					searchResult.m_vdAuxiliaryPosY.at(1) = AuxiliaryPos.GetY();
					searchResult.m_vdAuxiliaryAngle.at(1) = dAngle;
					searchResult.m_vLineSearchResult.push_back(searchResult1.m_vLineSearchResult[0]);
					searchResult.m_vLineSearchResult.push_back(searchResult2.m_vLineSearchResult[0]);

					searchResult.m_vTrainSearchRect = searchResult0.m_vTrainSearchRect;
					searchResult.m_vACFSearchRect.push_back(searchResult1.m_vACFSearchRect[0]);
					searchResult.m_vACFSearchRect.push_back(searchResult2.m_vACFSearchRect[0]);

					m_SearchResultArray.push_back(searchResult);

				}
			}
		}
		else if((!m_TrainDataParam.m_bEnableCheckRegion1) && m_TrainDataParam.m_bEnableCheckRegion2)
		{
			if (bRet)
			{
				if(bRetCheck1 && bRetCheck2)
				{				
					m_SearchResultArray.push_back(searchResult2);
				}
			}
		}
		else if((m_TrainDataParam.m_bEnableCheckRegion1) && (!m_TrainDataParam.m_bEnableCheckRegion2))
		{
			if (bRet)
			{
				if(bRetCheck1 && bRetCheck2)
				{
					m_SearchResultArray.push_back(searchResult1);
				}
			}
		}
		else if((!m_TrainDataParam.m_bEnableCheckRegion1) && (!m_TrainDataParam.m_bEnableCheckRegion2))
		{
			if (bRet)
			{
				if(bRetCheck1 && bRetCheck2)
				{
					m_SearchResultArray.push_back(searchResult0);
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
		m_nResultErrNumber=-1;//几何定位失败
		return bRet;
	}


	return (bRet && bRetCheck1 && bRetCheck2);
}

//获取搜索结果个数										
int CQuickSearchCheckLineTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CQuickSearchCheckLineTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CQuickSearchCheckLineTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eQuickSearchCheckLineTool)
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
bool CQuickSearchCheckLineTool::LoadModelFromFile(CString strDir)
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
	CQuickSearchCheckLineTrainDataParam tmpTrainDataParam;
	CQuickSearchCheckLineTrainGuiParam tmpTrainGuiParam;
	CQuickSearchCheckLineSearchDataParam tmpSearchDataParam;
	CQuickSearchCheckLineSearchGuiParam tmpSearchGuiParam;
	CQuickSearchCheckLineTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eQuickSearchCheckLineTool)
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

	// EnableCheckRegion1
	bRet = cXMLConfigurator.FindElem(_T("EnableCheckRegion1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableCheckRegion1 = _ttoi(cXMLConfigurator.GetElemData());
	// EnableCheckRegion2
	bRet = cXMLConfigurator.FindElem(_T("EnableCheckRegion2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableCheckRegion2 = _ttoi(cXMLConfigurator.GetElemData());




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
		tmpTrainDataParam.m_bSortByScore = true;	
	}
	else
	{
		tmpTrainDataParam.m_bSortByScore = _ttoi(cXMLConfigurator.GetElemData());
	}
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
	// LineAngleType
	bRet = cXMLConfigurator.FindElem(_T("LineAngleType"));
	if (!bRet)
	{
		tmpTrainDataParam.m_nLineAngleType = 0;
	}
	else
	{
		tmpTrainDataParam.m_nLineAngleType = _ttol(cXMLConfigurator.GetElemData());
	}





	// FilterHalfWidth
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth1 = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth1 = _ttol(cXMLConfigurator.GetElemData());
	}
	
	// outlinenum
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_nOutLineNum1 = 10;
	}
	else
	{
		tmpTrainDataParam.m_nOutLineNum1 = _ttol(cXMLConfigurator.GetElemData());
	}
	
	// edgeproperty
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_edgeProperty1 = eDontCare;
	}
	else
	{
		tmpTrainDataParam.m_edgeProperty1 = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	}
	

	// SortByScore
	bRet = cXMLConfigurator.FindElem(_T("SortByScore1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore1 = true;		
	}
	else
	{
		tmpTrainDataParam.m_bSortByScore1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge1 = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	// LineAngleType
	bRet = cXMLConfigurator.FindElem(_T("LineAngleType1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_nLineAngleType1 = 0;		
	}
	else
	{
		tmpTrainDataParam.m_nLineAngleType1 = _ttol(cXMLConfigurator.GetElemData());
	}

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

	// CheckTrainAffineRect1
	bRet = cXMLConfigurator.FindElem(_T("CheckTrainAffineRect1"));
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
	tmpTrainGuiParam.m_TrainAffineRect1.SetCenter(vPos);

	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));

	bRet = cXMLConfigurator.FindElem(_T("Skew"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect1.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
	cXMLConfigurator.LeaveElem();

	// CheckTrainAffineRect2
	bRet = cXMLConfigurator.FindElem(_T("CheckTrainAffineRect2"));
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
	tmpTrainGuiParam.m_TrainAffineRect2.SetCenter(vPos);

	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect2.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect2.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect2.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));

	bRet = cXMLConfigurator.FindElem(_T("Skew"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_TrainAffineRect2.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
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

	// CheckModelPoint1
	bRet = cXMLConfigurator.FindElem(_T("CheckModelPoint1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
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
	tmpTrainGuiParam.m_ModelPoint1 = vPos;
	cXMLConfigurator.LeaveElem();

	// CheckModelPoint2
	bRet = cXMLConfigurator.FindElem(_T("CheckModelPoint2"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
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

	// Caliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask = false;
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask = _ttol(cXMLConfigurator.GetElemData());
	}
	
	bRet = cXMLConfigurator.FindElem(_T("MaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet)
		{
			return false;
		}
			
		tmpTrainGuiParam.m_vdMaskData_FindLine.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}
				
			tmpTrainGuiParam.m_vdMaskData_FindLine.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask_FindLine.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine.size(); j++)
		{
			strTmp.Format(_T("CaliperMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask_FindLine.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}





	// CaliperNum1
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_nCaliperNum1 = 3;
	}
	else
	{
		tmpTrainGuiParam.m_nCaliperNum1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dStartX1 = 100;
	}
	else
	{
		tmpTrainGuiParam.m_dStartX1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dStartY1 = 300;
	}
	else
	{
		tmpTrainGuiParam.m_dStartY1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dEndX1 = 200;
	}
	else
	{
		tmpTrainGuiParam.m_dEndX1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dEndY1 = 200;
	}
	else
	{
		tmpTrainGuiParam.m_dEndY1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dCaliperProjectLen1 = 20;
	}
	else
	{
		tmpTrainGuiParam.m_dCaliperProjectLen1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_dCaliperSearchLen1 = 100;
	}
	else
	{
		tmpTrainGuiParam.m_dCaliperSearchLen1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_CaliperSearchRotation1 = scRadian(skPI/2.0);
	}
	else
	{
		tmpTrainGuiParam.m_CaliperSearchRotation1 = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}

	// Caliper Visible2
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible1=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask1 = false;
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask1 = _ttol(cXMLConfigurator.GetElemData());
	}
	

	bRet = cXMLConfigurator.FindElem(_T("MaskParam1"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum1"));
		if (!bRet)
		{
			return false;
		}			
		tmpTrainGuiParam.m_vdMaskData_FindLine1.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine1.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vdMaskData_FindLine1.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum1"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask_FindLine1.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine1.size(); j++)
		{
			strTmp.Format(_T("CaliperMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask_FindLine1.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
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
	if (!bRet)
	{
		tmpSearchDataParam.m_dSizeLow = 1.0;		
	}
	else
	{
		tmpSearchDataParam.m_dSizeLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeHigh"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dSizeHigh = 1.0;		
	}
	else
	{
		tmpSearchDataParam.m_dSizeHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
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


	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible1"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint1 = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint1 = _ttoi(cXMLConfigurator.GetElemData());
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


	bRet = cXMLConfigurator.FindElem(_T("StartX1"));
	if (!bRet)
	{
		vPos.SetX(0);
	}
	else
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("StartY1"));
	if (!bRet)
	{
		vPos.SetY(0);
	}
	else
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainResult.m_dStartX1 = vPos.GetX();
	tmpTrainResult.m_dStartY1 = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX1"));
	if (!bRet)
	{
		vPos.SetX(0);
	}
	else
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("EndY1"));
	if (!bRet)
	{
		vPos.SetY(0);
	}
	else
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainResult.m_dEndX1 = vPos.GetX();
	tmpTrainResult.m_dEndY1 = vPos.GetY();

	cXMLConfigurator.LeaveElem();

	//cXMLConfigurator.LeaveElem();


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

	if (tmpTrainDataParam.m_bEnableCheckRegion1)
	{
		bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model1.ptq"));;
		if (!bRet)
		{
			return false;
		}
	}
	if (tmpTrainDataParam.m_bEnableCheckRegion2)
	{
		bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"));;
		if (!bRet)
		{
			return false;
		}
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
	if (tmpTrainDataParam.m_bEnableCheckRegion1)
	{
		bRet = m_QuickSearchTool1.LoadTrainModelFromBinaryFile(strDir + _T("Model1.ptq"));;
		if (!bRet)
		{
			return false;
		}
	}
	if (tmpTrainDataParam.m_bEnableCheckRegion2)
	{
		bRet = m_QuickSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"));;
		if (!bRet)
		{
			return false;
		}
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
bool CQuickSearchCheckLineTool::SaveModelToFile(CString strDir)
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

	// EnableCheckRegion1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCheckRegion1);
	cXMLConfigurator.AddElem(_T("EnableCheckRegion1"), strInfo);
	// EnableCheckRegion2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCheckRegion2);
	cXMLConfigurator.AddElem(_T("EnableCheckRegion2"), strInfo);

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
	// LineAngleType
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType);
	cXMLConfigurator.AddElem(_T("LineAngleType"), strInfo);


	// fileterhalfwidth1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth1);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth1"), strInfo);
	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum1);
	cXMLConfigurator.AddElem(_T("OutLineNum1"), strInfo);
	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty1);
	cXMLConfigurator.AddElem(_T("EdgeProperty1"), strInfo);
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore1);
	cXMLConfigurator.AddElem(_T("SortByScore1"), strInfo);
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge1);
	cXMLConfigurator.AddElem(_T("DualEdge1"), strInfo);
	// LineAngleType
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType1);
	cXMLConfigurator.AddElem(_T("LineAngleType1"), strInfo);

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

	// CheckTrainAffineRect1
	cXMLConfigurator.AddElem(_T("CheckTrainAffineRect1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetXRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect1.GetSkew().ToDouble());
	cXMLConfigurator.AddElem(_T("Skew"), strInfo);
	cXMLConfigurator.LeaveElem();

	// CheckTrainAffineRect2
	cXMLConfigurator.AddElem(_T("CheckTrainAffineRect2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetXRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainAffineRect2.GetSkew().ToDouble());
	cXMLConfigurator.AddElem(_T("Skew"), strInfo);
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

	// CheckModelPoint1
	cXMLConfigurator.AddElem(_T("CheckModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();

	// CheckModelPoint2
	cXMLConfigurator.AddElem(_T("CheckModelPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
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
	//Caliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible);
	cXMLConfigurator.AddElem(_T("CaliperVisible"), strInfo);

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable"), strInfo);

	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdMaskData_FindLine.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vdMaskData_FindLine.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdMaskData_FindLine.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask_FindLine.size());
	cXMLConfigurator.AddElem(_T("CaliperMaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask_FindLine.size(); j++)
	{
		strTmp.Format(_T("CaliperMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask_FindLine.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();




	// Caliper Num1 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum1);
	cXMLConfigurator.AddElem(_T("CaliperNum1"), strInfo);
	// startX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX1);
	cXMLConfigurator.AddElem(_T("StartX1"), strInfo);
	// startY1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY1);
	cXMLConfigurator.AddElem(_T("StartY1"), strInfo);
	// EndX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX1);
	cXMLConfigurator.AddElem(_T("EndX1"), strInfo);
	// EndY
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY1);
	cXMLConfigurator.AddElem(_T("EndY1"), strInfo);
	// Project Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen1);
	cXMLConfigurator.AddElem(_T("ProjectLen1"), strInfo);
	// Search Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen1);
	cXMLConfigurator.AddElem(_T("SearchLen1"), strInfo);
	// Search Rotation1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation1.ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation1"), strInfo);
	//Caliper Visible2
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible1);
	cXMLConfigurator.AddElem(_T("CaliperVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask1);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable1"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam1"));
	cXMLConfigurator.AddElem(_T("MaskParam1"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdMaskData_FindLine1.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum1"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vdMaskData_FindLine1.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdMaskData_FindLine1.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask_FindLine1.size());
	cXMLConfigurator.AddElem(_T("CaliperMaskDataNum1"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask_FindLine1.size(); j++)
	{
		strTmp.Format(_T("CaliperMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask_FindLine1.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
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

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint1);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible1"), strInfo);

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
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX);
	cXMLConfigurator.AddElem(_T("StartX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY);
	cXMLConfigurator.AddElem(_T("StartY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX);
	cXMLConfigurator.AddElem(_T("EndX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY);
	cXMLConfigurator.AddElem(_T("EndY"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX1);
	cXMLConfigurator.AddElem(_T("StartX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY1);
	cXMLConfigurator.AddElem(_T("StartY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX1);
	cXMLConfigurator.AddElem(_T("EndX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY1);
	cXMLConfigurator.AddElem(_T("EndY1"), strInfo);
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

	if (m_TrainDataParam.m_bEnableCheckRegion1)
	{
		bRet = m_QuickSearchTool1.SaveTrainModelToBinaryFile(strDir + _T("Model1.ptq"));
		if (!bRet)
		{
			return false;
		}
	}

	if (m_TrainDataParam.m_bEnableCheckRegion2)
	{
		bRet = m_QuickSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.ptq"));
		if (!bRet)
		{
			return false;
		}
	}

	return true;
}	

// 更新训练控件数据信息	
bool CQuickSearchCheckLineTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_TrainAffineRect1	= m_GuiTrainRect1.GetAffineRect();
		m_tmpTrainGuiParam.m_TrainAffineRect2	= m_GuiTrainRect2.GetAffineRect();

		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint1		= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint2		= m_GuiModelPoint.GetCenter();

		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nCaliperNum		= m_GuiFindLine.GetCaliperNum();
		m_tmpTrainGuiParam.m_dCaliperProjectLen	= m_GuiFindLine.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();
		m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX,m_tmpTrainGuiParam.m_dEndY);

		m_GuiFindLine.GetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine);
		m_GuiFindLine.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine);


		m_tmpTrainGuiParam.m_nCaliperNum1		= m_GuiFindLine1.GetCaliperNum();
		m_tmpTrainGuiParam.m_dCaliperProjectLen1	= m_GuiFindLine1.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen1		= m_GuiFindLine1.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation1	= m_GuiFindLine1.GetCaliperSearchDirection();
		m_GuiFindLine1.GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX1, m_tmpTrainGuiParam.m_dStartY1, m_tmpTrainGuiParam.m_dEndX1,
			m_tmpTrainGuiParam.m_dEndY1);

		m_GuiFindLine1.GetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine1);
		m_GuiFindLine1.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine1);
		m_tmpTrainGuiParam.m_bCaliperVisible=m_GuiFindLine.IsCaliperVisible();
		m_tmpTrainGuiParam.m_bCaliperVisible1=m_GuiFindLine1.IsCaliperVisible();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

		m_GuiTrainRect1.SetLineColor(RGB(255, 0, 0));
		m_GuiTrainRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect1.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect1);

		m_GuiTrainRect2.SetLineColor(RGB(255, 0, 0));
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect2);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiFindLine.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindLine.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_GuiFindLine.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX, m_tmpTrainGuiParam.m_dStartY, m_tmpTrainGuiParam.m_dEndX, m_tmpTrainGuiParam.m_dEndY);
		m_GuiFindLine.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_GuiFindLine.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen);
		m_GuiFindLine.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation);
		m_GuiFindLine.SetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine);
		m_GuiFindLine.SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible);
		m_GuiFindLine.SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible);

		m_GuiFindLine1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindLine1.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum1);
		m_GuiFindLine1.SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX1, m_tmpTrainGuiParam.m_dStartY1, m_tmpTrainGuiParam.m_dEndX1,
			m_tmpTrainGuiParam.m_dEndY1);
		m_GuiFindLine1.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen1);
		m_GuiFindLine1.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen1);
		m_GuiFindLine1.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation1);
		m_GuiFindLine1.SetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine1);
		m_GuiFindLine1.SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible1);
		m_GuiFindLine1.SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible1);
	}

	return true;
}

bool CQuickSearchCheckLineTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiSearchLineCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}




		for (j=0; j<m_pGuiSearchLineArray1.size(); j++)
		{
			m_pGuiSearchLineArray1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArray1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchLineCrossArray1.size(); j++)
		{
			m_pGuiSearchLineCrossArray1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1_1.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary1_1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary1_1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2_1.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary2_1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary2_1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
	}

	return true;
}

// 清除控件及其显示					
bool CQuickSearchCheckLineTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CQuickSearchCheckLineTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CQuickSearchCheckLineTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary1);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary2);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);


	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary1_1);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary2_1);
	ClearGraphicsArray(m_pGuiSearchLineArray1);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray1);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array_1);

	return true;
}

bool CQuickSearchCheckLineTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen, rot,skew;

	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
	m_GuiTrainRect1.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect1.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
	m_GuiTrainRect2.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect2.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	//搜索区域
	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	
	double plength,slength;


	//卡尺
	double sx,sy,ex,ey;
	m_GuiFindLine.GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine.SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine1.GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine1.SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

	plength = m_GuiFindLine.GetCaliperProjectionLength();
	slength = m_GuiFindLine.GetCaliperSearchLength();
	m_GuiFindLine.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine.SetCaliperSearchLength(slength/m_CamWidth*nWidth);
	plength = m_GuiFindLine1.GetCaliperProjectionLength();
	slength = m_GuiFindLine1.GetCaliperSearchLength();
	m_GuiFindLine1.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine1.SetCaliperSearchLength(slength/m_CamWidth*nWidth);



	//参考点位置
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
}

bool CQuickSearchCheckLineTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_CheckTrainRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_CheckTrainRect2"));

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	


		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindLineTool"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindLineTool1"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	if(m_pGuiTrainResultDisplay1 && ::IsWindow(m_hTrainResultDispWnd1))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay1->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain2"));
	}
	if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain3"));
	}
	return true;
}

bool CQuickSearchCheckLineTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint1Array[j]->GetTipText());
		}




		for (j=0; j<m_pGuiSearchLineArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1_1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary1_1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2_1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary2_1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint1Array_1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint1Array_1[j]->GetTipText());
		}
	}

	return true;
}

// 获取训练模板原始图像
bool CQuickSearchCheckLineTool::GetPatternImage(cpImage &ImagePattern)
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
bool CQuickSearchCheckLineTool::GetPatternImage2(cpImage &ImagePattern)
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

// 获取训练模板原始图像
bool CQuickSearchCheckLineTool::GetPatternImage3(cpImage &ImagePattern)
{
	/*bool bRet = m_QuickSearchTool2.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}

	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}

	if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool2.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 恢复训练参数
bool CQuickSearchCheckLineTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CQuickSearchCheckLineTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CQuickSearchCheckLineTool::SetLanguage(int nLanguage)
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
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiFindLine1.SetLabel(_T("FindLine1"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine.SetLabel(_T("找线工具1"));
		m_GuiFindLine1.SetLabel(_T("找线工具2"));
		break;
	}
}
CString CQuickSearchCheckLineTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:			//几何定位失败				
				strErrInfo += _T("Quick search failed");
				break;
			case -2:			//几何检查1失败				
				strErrInfo += _T("Quick search Check1 failed");
				break;
			case -3:			//几何检查2失败				
				strErrInfo += _T("Quick search Check2 failed");
				break;
			case -4:			//找线1失败				
				strErrInfo += _T("Find line1 failed");
				break;
			case -5:			//找线2失败				
				strErrInfo += _T("Find line2 failed");
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
			case -1:							
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							
				strErrInfo += _T("几何检查1失败");
				break;
			case -3:							
				strErrInfo += _T("几何检查2失败");
				break;
			case -4:							
				strErrInfo += _T("找线1失败");
				break;
			case -5:							
				strErrInfo += _T("找线2失败");
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
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							
				strErrInfo += _T("几何检查1失败");
				break;
			case -3:							
				strErrInfo += _T("几何检查2失败");
				break;
			case -4:							
				strErrInfo += _T("找线1失败");
				break;
			case -5:							
				strErrInfo += _T("找线2失败");
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