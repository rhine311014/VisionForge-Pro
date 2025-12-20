
#include "stdafx.h"
 #include "QuickLineCircleTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#include "svIntersect.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuickLineCircleTool::CQuickLineCircleTool()
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
	m_GuiTrainRect.SetLineColor(RGB(0, 0, 255));	// 蓝色

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLineColor(RGB(0, 255, 0));  // 绿色

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);
	
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(500, 240, 40, 40);
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}

	m_GuiFindLine[0].SetStartXYEndXY(100, 250, 100, 100);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);

	int i=0;
	for (i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d"), i+1);
			break;
		case 1:
			str.Format(_T("找线工具%d"), i+1);
			break;
		default:
			str.Format(_T("找线工具%d"), i+1);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}

	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_dGrau					= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre			= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag				= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable			= false;

	m_tmpTrainGuiParam.m_TrainAffineRect		= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	// Circle
	m_FindCircleTool.GetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty, 
		m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
	m_tmpTrainDataParam.m_nCircleOutLineNum			= 10;
	m_tmpTrainDataParam.m_bCircleSortByScore		= true;
	m_tmpTrainDataParam.m_bEnableCircle				= true;
	m_tmpTrainGuiParam.m_nCircleCaliperNum			= m_GuiFindCircle.GetCaliperNum();

	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius,
		m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
	m_tmpTrainGuiParam.m_dCircleCaliperProjectLen		= m_GuiFindCircle.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCircleCaliperSearchLen		= m_GuiFindCircle.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bCircleCaliperVisible			= m_GuiFindCircle.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bEnableCircleMask				= false;
	// Line
	for (i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], m_tmpTrainDataParam.m_eLinedgeProperty[i], 
			m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_nLineOutLineNum[i]			= 10;
		m_tmpTrainDataParam.m_bLineSortByScore[i]			= true;
		m_tmpTrainDataParam.m_nLineAngleType[i]				= 0;
		m_tmpTrainDataParam.m_bEnableLine[i]				= false;

		m_tmpTrainGuiParam.m_nLineCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], m_tmpTrainGuiParam.m_dLineEndX[i],
			m_tmpTrainGuiParam.m_dLineEndY[i]);
		m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bLineCaliperVisible[i]			= m_GuiFindLine[i].IsCaliperVisible();
	}


	m_tmpSearchDataParam.m_dAcceptThre			= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow			= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh			= 5.0;
	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CQuickLineCircleTool::CQuickLineCircleTool(CQuickLineCircleTool &cCircleSearchTool)
{
	if (cCircleSearchTool.GetSearchToolType() != eQuickLineCircleSearchTool)
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
	m_GuiTrainRect.SetLineColor(RGB(0, 0, 255));	// 蓝色

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLineColor(RGB(0, 255, 0));  // 绿色

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(500, 240, 40, 40);
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}

	for (int i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d"), i+1);
			break;
		case 1:
			str.Format(_T("找线工具%d"), i+1);
			break;
		default:
			str.Format(_T("找线工具%d"), i+1);
			break;
		}
		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
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

		//// 设置掩模图像
		//if (m_TrainDataParam.m_bMaskEnable)
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
		//		m_TrainGuiParam.m_ModelPoint.GetY());
		//	m_bTrainOK = bRet;
		//}
		//else
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
		//		m_TrainGuiParam.m_ModelPoint.GetY());
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

CQuickLineCircleTool& CQuickLineCircleTool::operator=(CQuickLineCircleTool &cCircleSearchTool)
{
	if (this == &cCircleSearchTool)
	{
		return *this;
	}

	if (cCircleSearchTool.GetSearchToolType() != eQuickLineCircleSearchTool)
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
	m_GuiTrainRect.SetLineColor(RGB(0, 0, 255));	// 蓝色

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLineColor(RGB(0, 255, 0));  // 绿色

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiModelPoint2.SetCenterXYWidthHeight(500, 240, 40, 40);
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}

	for (int i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d"), i+1);
			break;
		case 1:
			str.Format(_T("找线工具%d"), i+1);
			break;
		default:
			str.Format(_T("找线工具%d"), i+1);
			break;
		}
		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
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

		//// 设置掩模图像
		//if (m_TrainDataParam.m_bMaskEnable)
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
		//		m_TrainGuiParam.m_ModelPoint.GetY());
		//	m_bTrainOK = bRet;
		//}
		//else
		//{
		//	bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
		//		m_TrainGuiParam.m_ModelPoint.GetY());
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

CQuickLineCircleTool::~CQuickLineCircleTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CQuickLineCircleTool::GetSearchToolType()
{
	return eQuickLineCircleSearchTool;
}										   

// 设置显示控件指针		
bool CQuickLineCircleTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CQuickLineCircleTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CQuickLineCircleTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickLineCircleTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CQuickLineCircleTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CQuickLineCircleTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CQuickLineCircleTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CQuickLineCircleTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CQuickLineCircleTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CQuickLineCircleTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPoint2Visible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiFindCircle.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCircleVisible && m_tmpTrainDataParam.m_bEnableCircle);

	for (int i=0; i<PATTERN_LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible && m_tmpTrainDataParam.m_bEnableLine[i]);
	}
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
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle, m_strKeyText+_T("_FindCircle"));

		for (int i=0; i<PATTERN_LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

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
bool CQuickLineCircleTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultCircle.SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle, m_strKeyText+_T("_TrainResultCircle"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}		

// 更新搜索Gui控件显示														
bool CQuickLineCircleTool::UpdateSearchResultGuiDisplay()
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
	for (i=0; i<m_pGuiSearchCrossArray1.size(); i++)
	{
		m_pGuiSearchCrossArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible && m_tmpTrainGuiParam.m_bModelPoint2Visible);
	}

	for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible);
	}

	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible);
	}



	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	// 虚拟线段
	for (i=0; i<m_pGuiSearchLineArrayModelPoint.size(); i++)
	{
		m_pGuiSearchLineArrayModelPoint[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}
	for (i=0; i<m_pGuiSearchLineArrayVirtural.size(); i++)
	{
		m_pGuiSearchLineArrayVirtural[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}


	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint2Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint2);
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
		for (i=0; i<m_pGuiSearchCrossArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray1[i], m_pGuiSearchCrossArray1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleArray[i], m_pGuiSearchCircleArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterArray[i], m_pGuiSearchCircleCenterArray[i]->GetTipText());
		}




		for (i=0; i<m_pGuiSearchLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
		}

		// 两条虚拟线段
		for (i=0; i<m_pGuiSearchLineArrayModelPoint.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArrayModelPoint[i], m_pGuiSearchLineArrayModelPoint[i]->GetTipText());
		}
		for (i=0; i<m_pGuiSearchLineArrayVirtural.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArrayVirtural[i], m_pGuiSearchLineArrayVirtural[i]->GetTipText());
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

		for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint2Array[i], m_pGuiSearchResultFitPoint2Array[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CQuickLineCircleTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	//	UpdateTrainResultGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CQuickLineCircleTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CQuickLineCircleSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CQuickLineCircleTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	*((CQuickLineCircleSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CQuickLineCircleTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CQuickLineCircleSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CQuickLineCircleTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CQuickLineCircleSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CQuickLineCircleTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CQuickLineCircleTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CQuickLineCircleTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CQuickLineCircleTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CQuickLineCircleTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	//设置训练参数
	if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
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
			m_nResultErrNumber=-1;
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		m_tmpTrainResult.m_dModelX2 = m_tmpTrainGuiParam.m_ModelPoint2.GetX();
		m_tmpTrainResult.m_dModelY2 = m_tmpTrainGuiParam.m_ModelPoint2.GetY();

		if (m_tmpTrainDataParam.m_bEnableCircle)
		{
			// 找圆
			m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
			m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
				,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
			//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
			m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum);
			m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore);
			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius
				,m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
			m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
			if (m_tmpTrainDataParam.m_bEnableDualEdge)
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_eCircledgeProperty == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_eCircledgeProperty == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
			} 
			else
			{
				m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
			}
			scCircle cCircle;
			double   dRMS;
			if(bIsProcessImage)
			{
				bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS);
			}
			else
			{
				bRet = m_FindCircleTool.Execute(m_ImageTrain, cCircle, dRMS);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-2;
				return false;
			}
			m_tmpTrainResult.m_dCircleCenterX = cCircle.GetCenter().GetX();
			m_tmpTrainResult.m_dCircleCenterY = cCircle.GetCenter().GetY();
			m_tmpTrainResult.m_dCircleRadius  = cCircle.GetRadius();

			m_GuiTrainResultCircle.SetCircle(cCircle);
		}


// 		if (m_tmpTrainDataParam.m_bEnableLine[0])
// 		{
// 			// 找线0
// 			m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[0]);
// 			m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum[0], m_tmpTrainGuiParam.m_LineCaliperSearchRotation[0], m_tmpTrainGuiParam.m_dLineCaliperSearchLen[0]
// 			,m_tmpTrainGuiParam.m_dLineCaliperProjectLen[0]);
// 			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[0], m_tmpTrainDataParam.m_eLinedgeProperty[0], m_tmpTrainDataParam.m_lLineFilterHalfWidth[0]);
// 			m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum[0], eRansacRobust);
// 			m_FindLineTool[0].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bLineSortByScore[0]);
// 			m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[0], m_tmpTrainGuiParam.m_dLineStartY[0], 
// 				m_tmpTrainGuiParam.m_dLineEndX[0], m_tmpTrainGuiParam.m_dLineEndY[0]);
// 			scLineSeg cLineSeg1;
// 			double dRMS1;
// 			bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
// 			if (!bRet)
// 			{
// 				return false;
// 			}
// 
// 
// 			if (m_tmpTrainDataParam.m_nLineAngleType[0] == 0)	// 任意角度
// 			{
// 				m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
// 				m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
// 				m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
// 				m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
// 			}
// 			else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 1)	//水平线
// 			{
// 				scDegree degLineAngle;
// 				degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();
// 
// 				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
// 				{
// 					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
// 					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
// 					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
// 					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
// 				}
// 				else
// 				{
// 					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP2().GetX();
// 					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP2().GetY();
// 					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP1().GetX();
// 					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP1().GetY();
// 				}
// 			}
// 			else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 2)	//垂直线
// 			{
// 				scDegree degLineAngle;
// 				degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();
// 
// 				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
// 				{
// 					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
// 					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
// 					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
// 					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
// 				}
// 				else
// 				{
// 					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP2().GetX();
// 					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP2().GetY();
// 					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP1().GetX();
// 					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP1().GetY();
// 				}
// 			}
// 			else
// 			{
// 				m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
// 				m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
// 				m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
// 				m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
// 			}
// 
// 			m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
// 		}


		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
// 		m_GuiTrainResultCircle.SetCircle(cCircle);
// 		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());

		return true;
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
			m_nResultErrNumber=-1;
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		m_tmpTrainResult.m_dModelX2 = m_tmpTrainGuiParam.m_ModelPoint2.GetX();
		m_tmpTrainResult.m_dModelY2 = m_tmpTrainGuiParam.m_ModelPoint2.GetY();

//		if (m_tmpTrainDataParam.m_bEnableCircle)
		{
			// 找圆
			m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
			m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
				,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
			//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
			m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum);
			m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore);
			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius
				,m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
			m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
			if (m_tmpTrainDataParam.m_bEnableDualEdge)
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_eCircledgeProperty == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_eCircledgeProperty == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
			} 
			else
			{
				m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_eCircledgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
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
				m_nResultErrNumber=-2;
				return false;
			}
			m_tmpTrainResult.m_dCircleCenterX = cCircle.GetCenter().GetX();
			m_tmpTrainResult.m_dCircleCenterY = cCircle.GetCenter().GetY();
			m_tmpTrainResult.m_dCircleRadius  = cCircle.GetRadius();

			m_GuiTrainResultCircle.SetCircle(cCircle);
		}


		if (m_tmpTrainDataParam.m_bEnableLine[0])
		{
			// 找线0
			m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[0]);
			m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum[0], m_tmpTrainGuiParam.m_LineCaliperSearchRotation[0], m_tmpTrainGuiParam.m_dLineCaliperSearchLen[0]
			,m_tmpTrainGuiParam.m_dLineCaliperProjectLen[0]);
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[0], m_tmpTrainDataParam.m_eLinedgeProperty[0], m_tmpTrainDataParam.m_lLineFilterHalfWidth[0]);
			m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum[0], eRansacRobust);
			m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[0], m_tmpTrainGuiParam.m_dLineStartY[0], 
				m_tmpTrainGuiParam.m_dLineEndX[0], m_tmpTrainGuiParam.m_dLineEndY[0]);
			scLineSeg cLineSeg1;
			double dRMS1;
			if(bIsProcessImage)
			{
				bRet = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
			}
			else
			{
				bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-3;//找线工具失败
				return false;
			}

			if (m_tmpTrainDataParam.m_nLineAngleType[0] == 0)	// 任意角度
			{
				m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 1)	//水平线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();

				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP1().GetY();
				}
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 2)	//垂直线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();

				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP1().GetY();
				}
			}
			else
			{
				m_tmpTrainResult.m_dLineStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dLineStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dLineEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dLineEndY[0] = cLineSeg1.GetP2().GetY();
			}

			m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		}



		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
// 		m_GuiTrainResultCircle.SetCircle(cCircle);
// 		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CQuickLineCircleTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CQuickLineCircleTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	*((CQuickLineCircleSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CQuickLineCircleTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CQuickLineCircleTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CQuickLineCircleSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CQuickLineCircleTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	*((CQuickLineCircleSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CQuickLineCircleTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CQuickLineCircleSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CQuickLineCircleTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickLineCircleSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CQuickLineCircleSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CQuickLineCircleTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;///训练未成功
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

	// 设置接收阈值
	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// Circle
	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCircleCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum, m_TrainGuiParam.m_CircleCaliperSearchDirection, m_TrainGuiParam.m_dCircleCaliperSearchLen
		,m_TrainGuiParam.m_dCircleCaliperProjectLen);
	//m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre, m_TrainDataParam.m_eCircledgeProperty, m_TrainDataParam.m_lCircleFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nCircleOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bCircleSortByScore);
	m_FindCircleTool.SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask);
	if (m_tmpTrainDataParam.m_bEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_eCircledgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_eCircledgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre, m_TrainDataParam.m_eCircledgeProperty, tmpEdgePolarity,m_TrainDataParam.m_lCircleFilterHalfWidth);
	} 
	else
	{
		m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre, m_TrainDataParam.m_eCircledgeProperty, m_TrainDataParam.m_lCircleFilterHalfWidth);
	}
	// Line
	for (int i=0; i<PATTERN_LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nLineCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nLineCaliperNum[i], m_TrainGuiParam.m_LineCaliperSearchRotation[i], m_TrainGuiParam.m_dLineCaliperSearchLen[i]
		,m_TrainGuiParam.m_dLineCaliperProjectLen[i]);
		m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dLineContrastThre[i], m_TrainDataParam.m_eLinedgeProperty[i], m_TrainDataParam.m_lLineFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nLineOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bLineSortByScore[i]);
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
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);

			// 第一参考点位置
			CSearchResult SearchResultModel0;
			SearchResultModel0.m_dPosX = tmpQuickResult.GetPosition().GetX();
			SearchResultModel0.m_dPosY = tmpQuickResult.GetPosition().GetY();
			SearchResultModel0.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
//			SearchResultModel0.m_dAngle = tmpQuickResult.GetRotate().Norm().ToDouble();
			SearchResultModel0.m_dScore = tmpQuickResult.GetScore();

	

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

			// 第二参考点位置
			double dTransModelX, dTransModelY;
			dTransModelX = cos(dRotation) * m_TrainResult.m_dModelX2  - sin(dRotation) * m_TrainResult.m_dModelY2 + dTransX;
			dTransModelY = sin(dRotation) * m_TrainResult.m_dModelX2  + cos(dRotation) * m_TrainResult.m_dModelY2 + dTransY;

			CSearchResult SearchResultModel1;
			SearchResultModel1.m_dPosX = dTransModelX;
			SearchResultModel1.m_dPosY = dTransModelY;
			SearchResultModel1.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
//			SearchResultModel1.m_dAngle = tmpQuickResult.GetRotate().Norm().ToDouble();
			SearchResultModel1.m_dScore = tmpQuickResult.GetScore();


			// 求找圆工具变换后的位置,找圆
			double dTransCenterX, dTransCenterY;
			double dOriCenterX, dOriCenterY;
			dOriCenterX = m_TrainGuiParam.m_CircleCenter.GetX();
			dOriCenterY = m_TrainGuiParam.m_CircleCenter.GetY();
			dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
			dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
			m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum, m_TrainGuiParam.m_CircleCaliperSearchDirection, m_TrainGuiParam.m_dCircleCaliperSearchLen
				,m_TrainGuiParam.m_dCircleCaliperProjectLen);
			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_dCircleRadius
				,scRadian(dRotation)+m_TrainGuiParam.m_CircleArcStart, m_TrainGuiParam.m_CircleArcSpan);


			// 执行找圆
			CSearchResult searchResultCircle;	
			scCircle ResCircle;
			double dCircleRMS;
			bool bCircle = false;
			if (m_TrainDataParam.m_bEnableCircle)
			{
				if(bIsProcessImage)
				{
					bCircle = m_FindCircleTool.Execute(tmpImage, ResCircle, dCircleRMS);
				}
				else
				{
					bCircle = m_FindCircleTool.Execute(ImageSearch, ResCircle, dCircleRMS);
				}
				if (bCircle)
				{
					searchResultCircle.m_dPosX = ResCircle.GetCenter().GetX();
					searchResultCircle.m_dPosY = ResCircle.GetCenter().GetY();
// 					searchResultCircle.m_dAngle = 0;
// 					searchResultCircle.m_dScore = tmpQuickResult.GetScore();
				}
				else
				{
					m_nResultErrNumber=-2;//找圆工具失败
					return false;
				}
			}



			//////////////////////////////////////////////////////////////////////////
// 			CSearchResult LinesearchResult;
// 			scLineSeg TempLineSeg;
// 			scLineSeg ResLineSeg[QUICK_LINE_CIRCLE_NUM];
// 			double dLineRMS[QUICK_LINE_CIRCLE_NUM];
// 			bool   bFindLine[QUICK_LINE_CIRCLE_NUM];
// 			for (int k=0; k<QUICK_LINE_CIRCLE_NUM; k++)
// 			{
// 				bFindLine[k] = 0;
// 			}
// 
// 			if (m_TrainDataParam.m_bEnableLine[0])
// 			{
// 				for (int j=0; j<QUICK_LINE_CIRCLE_NUM; j++)
// 				{
// 					// 求找线工具变换后的位置,找线
// 					double dTransStartX, dTransStartY;
// 					double dTransEndX, dTransEndY;
// 					dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dLineStartX[j]  - sin(dRotation) * m_TrainGuiParam.m_dLineStartY[j] + dTransX;
// 					dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dLineStartX[j]  + cos(dRotation) * m_TrainGuiParam.m_dLineStartY[j] + dTransY;
// 					dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dLineEndX[j]  - sin(dRotation) * m_TrainGuiParam.m_dLineEndY[j] + dTransX;
// 					dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dLineEndX[j]  + cos(dRotation) * m_TrainGuiParam.m_dLineEndY[j] + dTransY;
// 					m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nLineCaliperNum[j], /*scRadian(dRotation)+*/m_TrainGuiParam.m_LineCaliperSearchRotation[j], m_TrainGuiParam.m_dLineCaliperSearchLen[j]
// 					,m_TrainGuiParam.m_dLineCaliperProjectLen[j]);
// 					m_FindLineTool[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
// 
// 					bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLineSeg[j], dLineRMS[j]); // 成功
// 				}
// 
// 				// 求交点
// 				if (bFindLine[0])
// 				{
// 					LinesearchResult.m_dPosX = (ResLineSeg[0].GetP1().GetX() + ResLineSeg[0].GetP2().GetX())/2;
// 					LinesearchResult.m_dPosY = (ResLineSeg[0].GetP1().GetY() + ResLineSeg[0].GetP2().GetY())/2;
// 
// 					if (m_TrainDataParam.m_nLineAngleType[0] == 0)	// 任意角度
// 					{
// 						LinesearchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg[0].GetP1().GetX();
// 						LinesearchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg[0].GetP1().GetY();
// 						LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();
// 						LinesearchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg[0].GetP2().GetX();
// 						LinesearchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg[0].GetP2().GetY();
// 						LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();
// 
// 						TempLineSeg.SetP1(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(0), LinesearchResult.m_vdAuxiliaryPosY.at(0)));
// 						TempLineSeg.SetP2(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(1), LinesearchResult.m_vdAuxiliaryPosY.at(1)));
// 
// 					}
// 					else if (m_TrainDataParam.m_nLineAngleType[0] == 1)	//水平线
// 					{
// 						scDegree degLineAngle;
// 						degLineAngle = scDegree(ResLineSeg[0].GetRotation()).SignedNorm();
// 						if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
// 						{
// 							LinesearchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP1().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP1().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();
// 							LinesearchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP2().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP2().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();
// 						}
// 						else
// 						{
// 							LinesearchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP2().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP2().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();
// 							LinesearchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP1().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP1().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();
// 						}
// 
// 						TempLineSeg.SetP1(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(0), LinesearchResult.m_vdAuxiliaryPosY.at(0)));
// 						TempLineSeg.SetP2(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(1), LinesearchResult.m_vdAuxiliaryPosY.at(1)));
// 
// 					}
// 					else if (m_TrainDataParam.m_nLineAngleType[0] == 2)	//垂直线
// 					{
// 
// 						scDegree degLineAngle;
// 						degLineAngle = scDegree(ResLineSeg[0].GetRotation()).Norm();
// 						if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
// 						{
// 							LinesearchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP1().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP1().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().NormMod180()).ToDouble();
// 							LinesearchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP2().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP2().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().NormMod180()).ToDouble();
// 						}
// 						else
// 						{
// 							LinesearchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP2().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP2().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().NormMod180()).ToDouble();
// 							LinesearchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP1().GetX();
// 							LinesearchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP1().GetY();
// 							LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().NormMod180()).ToDouble();
// 						}
// 
// 						TempLineSeg.SetP1(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(0), LinesearchResult.m_vdAuxiliaryPosY.at(0)));
// 						TempLineSeg.SetP2(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(1), LinesearchResult.m_vdAuxiliaryPosY.at(1)));
// 
// 					}
// 					else
// 					{
// 						LinesearchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg[0].GetP1().GetX();
// 						LinesearchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg[0].GetP1().GetY();
// 						LinesearchResult.m_vdAuxiliaryAngle.at(0) = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();
// 						LinesearchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg[0].GetP2().GetX();
// 						LinesearchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg[0].GetP2().GetY();
// 						LinesearchResult.m_vdAuxiliaryAngle.at(1) = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();
// 
// 						TempLineSeg.SetP1(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(0), LinesearchResult.m_vdAuxiliaryPosY.at(0)));
// 						TempLineSeg.SetP2(sc2Vector(LinesearchResult.m_vdAuxiliaryPosX.at(1), LinesearchResult.m_vdAuxiliaryPosY.at(1)));
// 
// 					}
// 				}
// 				else
// 				{
// 					return false;
// 				}
// 			}

			


			CSearchResult searchResult;
			if ((m_TrainGuiParam.m_bModelPoint2Visible && bCircle && m_TrainDataParam.m_bEnableCircle))
			{
				scLineSeg LineSegModelPoint;
				LineSegModelPoint.SetP1(sc2Vector(SearchResultModel0.m_dPosX, SearchResultModel0.m_dPosY));
				LineSegModelPoint.SetP2(sc2Vector(SearchResultModel1.m_dPosX, SearchResultModel1.m_dPosY));
				// 返回角度归一化到（0-360）度
//				double dAngleLineSegModelPoint = scDegree(scRadian(LineSegModelPoint.GetRotation())).Norm().ToDouble();
//				double dAngleLineSegModelPoint = scDegree(scRadian(LineSegModelPoint.GetRotation())).ToDouble();
				double dAngleLineSegModelPoint = tmpQuickResult.GetRotate().ToDouble();

				scLine scVirturalline(sc2Vector(searchResultCircle.m_dPosX, searchResultCircle.m_dPosY),LineSegModelPoint.GetRotation());

				sc2Vector scTempPointLeft  = sc2Vector(0, 0);
				sc2Vector scTempPointRight = sc2Vector(0, 0);

				bool bIntersect;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineCircle(scVirturalline, ResCircle, bIntersect, crossPointArray) && crossPointArray.size() > 0)
				{
					if (crossPointArray.size() == 2)
					{
						scTempPointLeft = crossPointArray.at(0);
						scTempPointRight = crossPointArray.at(1);
					}
					else
					{
						return false;
					}
				}

				// 几何定位点与圆上点之间的距离
				sc2Vector scPointLeft  = sc2Vector(0, 0);
				sc2Vector scPointRight = sc2Vector(0, 0);
				int nIndexLeft = 0;

				// 第一标记点
				sc2Vector ModelPointLeft = sc2Vector(SearchResultModel0.m_dPosX, SearchResultModel0.m_dPosY);
				double dDistanceLeft1 = ModelPointLeft.Distance(scTempPointLeft);
				double dDistanceRight1 = ModelPointLeft.Distance(scTempPointRight);

				if (dDistanceLeft1 < dDistanceRight1)
				{
					scPointLeft = scTempPointLeft;
					scPointRight = scTempPointRight;

					nIndexLeft = 1;
				}
				else
				{
					scPointLeft = scTempPointRight;
					scPointRight = scTempPointLeft;

					nIndexLeft = 2;
				}


				//////////////////////////////////////////////////////////////////////////
				// 第二标记点
				int nIndexRight = 0;
				sc2Vector ModelPointRight = sc2Vector(SearchResultModel1.m_dPosX, SearchResultModel1.m_dPosY);
				double dDistanceLeft2 = ModelPointRight.Distance(scTempPointLeft);
				double dDistanceRight2 = ModelPointRight.Distance(scTempPointRight);

				if (dDistanceLeft2 > dDistanceRight2)
				{
					scPointLeft = scTempPointLeft;
					scPointRight = scTempPointRight;

					nIndexRight = 1;
				}
				else
				{
					scPointLeft = scTempPointRight;
					scPointRight = scTempPointLeft;

					nIndexRight = 2;
				}

				if (nIndexLeft != nIndexRight )
				{
					//AfxMessageBox(_T("impossable"));
					m_nResultErrNumber = -4;
					return false;
				}


				searchResult.m_dPosX = scPointLeft.GetX();
				searchResult.m_dPosY = scPointLeft.GetY();
				searchResult.m_dAngle = dAngleLineSegModelPoint;

				searchResult.m_vdAuxiliaryPosX.at(0) = scPointRight.GetX();
				searchResult.m_vdAuxiliaryPosY.at(0) = scPointRight.GetY();
				searchResult.m_vdAuxiliaryAngle.at(0) = dAngleLineSegModelPoint;

				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				CResultCircleSearchTool circle;
				circle.m_dRadius = ResCircle.GetRadius();
				circle.m_scCircleCircle = ResCircle.GetCenter();
				searchResult.m_vCircleSearchResult.push_back(circle);

				CResultLine line;
				line.m_vLineX[0] = scPointLeft.GetX();
				line.m_vLineY[0] = scPointLeft.GetY();
				line.m_vLineX[1] = scPointRight.GetX();
				line.m_vLineY[1] = scPointRight.GetY();
				searchResult.m_vLineSearchResult.push_back(line);

				m_SearchResultArray.push_back(searchResult);

				// 几何定位搜索仿射矩形
				CString	strTmpKey;
				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

				// 几何定位工具十字标记点
				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				//scGuiCross *pTempCross = new scGuiCross;
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray.push_back(pTempCross);

				strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
				scGuiCross *pTempCross2 = m_GuiSearchCross1+i;
				pTempCross2->SetCenterExtent(sc2Vector(SearchResultModel1.m_dPosX,SearchResultModel1.m_dPosY), sc2Vector(40,40));
				pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross2->SetTipTextEnabled(true);
				pTempCross2->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray1.push_back(pTempCross2);

				// 搜索到的圆形
				strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
				//scGuiCircle *pTemCircle = new scGuiCircle;
				scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
				pTemCircle->SetCircle(ResCircle);
				pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemCircle->SetTipTextEnabled(true);
				pTemCircle->SetTipText(strTmpKey);
				m_pGuiSearchCircleArray.push_back(pTemCircle);

				// 圆心
				strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, i);
				scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+i;
				pTempCircleCenter->SetCenterExtent(ResCircle.GetCenter(), sc2Vector(40,40));
				pTempCircleCenter->SetLineColor(RGB(255,0,0));
				pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCircleCenter->SetTipTextEnabled(true);
				pTempCircleCenter->SetTipText(strTmpKey);
				m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);	

				strTmpKey.Format(_T("%s_ResultModelPointLine%d"), m_strKeyText, i);
				scGuiLine *pModelPointLine = m_GuiSearchLineModelPoint+i;
				pModelPointLine->SetLine(LineSegModelPoint.GetLine());
				pModelPointLine->SetLineColor(RGB(255, 0, 0));
				pModelPointLine->SetLineWidth(1);
				pModelPointLine->SetTipTextEnabled(true);
				pModelPointLine->SetTipText(strTmpKey);
				pModelPointLine->SetLineStyle(scGuiGraphic::dashLine);
				m_pGuiSearchLineArrayModelPoint.push_back(pModelPointLine);

				strTmpKey.Format(_T("%s_ResultVirturalLine%d"), m_strKeyText, i);
				scGuiLine *pTempVirturalLine = m_GuiSearchLineVirtucal+i;
				pTempVirturalLine->SetLine(scVirturalline);
				pTempVirturalLine->SetLineColor(RGB(255, 0, 0));
				pTempVirturalLine->SetLineWidth(1);
				pTempVirturalLine->SetTipTextEnabled(true);
				pTempVirturalLine->SetTipText(strTmpKey);
				pTempVirturalLine->SetLineStyle(scGuiGraphic::dashLine);
				m_pGuiSearchLineArrayVirtural.push_back(pTempVirturalLine);

				strTmpKey.Format(_T("%s_ResultCrossAuxiliary1%d"), m_strKeyText, i);
				scGuiCross *pTempCrossAuxiliary1 = m_GuiSearchCrossAuxilary1+i;
//				pTempCrossAuxiliary1->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(30,30));
				pTempCrossAuxiliary1->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(30,30));
				pTempCrossAuxiliary1->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCrossAuxiliary1->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCrossAuxiliary1->SetTipTextEnabled(true);
				pTempCrossAuxiliary1->SetTipText(strTmpKey);
				CString strTempCrossAuxiliary1;
				strTempCrossAuxiliary1.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
				pTempCrossAuxiliary1->SetLabel(strTempCrossAuxiliary1);
				pTempCrossAuxiliary1->SetLabelVisible(TRUE);
				m_pGuiSearchCrossArrayAuxilary1.push_back(pTempCrossAuxiliary1);

				strTmpKey.Format(_T("%s_ResultCrossAuxiliary2%d"), m_strKeyText, i);
				scGuiCross *pTempCrossAuxiliary2 = m_GuiSearchCrossAuxilary2+i;
				pTempCrossAuxiliary2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(30,30));
				pTempCrossAuxiliary2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCrossAuxiliary2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCrossAuxiliary2->SetTipTextEnabled(true);
				pTempCrossAuxiliary2->SetTipText(strTmpKey);
				CString strTempCrossAuxiliary2;
				strTempCrossAuxiliary2.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_vdAuxiliaryPosX.at(0), searchResult.m_vdAuxiliaryPosY.at(0), searchResult.m_dAngle);
				pTempCrossAuxiliary2->SetLabel(strTempCrossAuxiliary2);
				pTempCrossAuxiliary2->SetLabelVisible(TRUE);
				m_pGuiSearchCrossArrayAuxilary2.push_back(pTempCrossAuxiliary2);


				//////////////////////////////////////////////////////////////////////////
				{
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
				}
				/////////////////////////////////////////////////////////////////////////

			}
			else
			{
				searchResult.m_dPosX = SearchResultModel0.m_dPosX;
				searchResult.m_dPosY = SearchResultModel0.m_dPosY;
				searchResult.m_dAngle = SearchResultModel0.m_dAngle;
				searchResult.m_dScore = SearchResultModel0.m_dScore;

				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				searchResult.m_vdAuxiliaryPosX.at(0) = SearchResultModel1.m_dPosX;
				searchResult.m_vdAuxiliaryPosY.at(0) = SearchResultModel1.m_dPosY;
				searchResult.m_vdAuxiliaryAngle.at(0) = SearchResultModel1.m_dAngle;

				m_SearchResultArray.push_back(searchResult);


				// 几何定位搜索仿射矩形
				CString	strTmpKey;
				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

				// 几何定位工具十字标记点
				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				CString strTempCross;
				strTempCross.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
				pTempCross->SetLabel(strTempCross);
				pTempCross->SetLabelVisible(TRUE);
				m_pGuiSearchCrossArray.push_back(pTempCross);

				strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
				scGuiCross *pTempCross2 = m_GuiSearchCross1+i;
				pTempCross2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(40,40));
				pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross2->SetTipTextEnabled(true);
				pTempCross2->SetTipText(strTmpKey);
				CString strTempCross2;
				strTempCross2.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_vdAuxiliaryPosX.at(0), searchResult.m_vdAuxiliaryPosY.at(0), searchResult.m_dAngle);
				pTempCross2->SetLabel(strTempCross2);
				pTempCross2->SetLabelVisible(TRUE);
				m_pGuiSearchCrossArray1.push_back(pTempCross2);
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
		return false;
	}

	return true;
}

// 获取搜索结果个数										
int CQuickLineCircleTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CQuickLineCircleTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CQuickLineCircleTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eQuickLineCircleSearchTool)
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
bool CQuickLineCircleTool::LoadModelFromFile(CString strDir)
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
	CQuickLineCircleSearchTrainDataParam tmpTrainDataParam;
	CQuickLineCircleSearchTrainGuiParam tmpTrainGuiParam;
	CQuickLineCircleSearchSearchDataParam tmpSearchDataParam;
	CQuickLineCircleSearchSearchGuiParam tmpSearchGuiParam;
	CQuickLineCircleSearchTrainResult  tmpTrainResult;
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
	if (cSearchToolType != eQuickLineCircleSearchTool)
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
	bRet = cXMLConfigurator.FindElem(_T("CircleContrastThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dCircleContrastThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth
	bRet = cXMLConfigurator.FindElem(_T("CircleFilterHalfWidth"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lCircleFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	// outlinenum
	bRet = cXMLConfigurator.FindElem(_T("CircleOutLineNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nCircleOutLineNum = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty
	bRet = cXMLConfigurator.FindElem(_T("CircleEdgeProperty"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_eCircledgeProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());


	// SortByScore
	bRet = cXMLConfigurator.FindElem(_T("CircleSortByScore"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bCircleSortByScore = _ttoi(cXMLConfigurator.GetElemData());

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
	

	// EnableCircle
	bRet = cXMLConfigurator.FindElem(_T("EnableCircle"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableCircle = _ttoi(cXMLConfigurator.GetElemData());

	//////////////////////////////////////////////////////////////////////////
	// contrastthre1
	bRet = cXMLConfigurator.FindElem(_T("LineContrastThre1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dLineContrastThre[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth1
	bRet = cXMLConfigurator.FindElem(_T("LineFilterHalfWidth1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lLineFilterHalfWidth[0] = 1;
	}
	else
	{
		tmpTrainDataParam.m_lLineFilterHalfWidth[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	// outlinenum1
	bRet = cXMLConfigurator.FindElem(_T("LineOutLineNum1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLineOutLineNum[0] = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty1
	bRet = cXMLConfigurator.FindElem(_T("LineEdgeProperty1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_eLinedgeProperty[0] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// SortByScore1
	bRet = cXMLConfigurator.FindElem(_T("LineSortByScore1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bLineSortByScore[0] = _ttoi(cXMLConfigurator.GetElemData());

	// LineAngleType1
	bRet = cXMLConfigurator.FindElem(_T("LineAngleType1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLineAngleType[0] = _ttol(cXMLConfigurator.GetElemData());
	// EnableLine
	bRet = cXMLConfigurator.FindElem(_T("EnableLine"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableLine[0] = _ttoi(cXMLConfigurator.GetElemData());

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

	// model point2 visible
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint2Visible"));
	if (!bRet)
	{
 		tmpTrainGuiParam.m_bModelPoint2Visible = true;
 	}
	else
	{
		tmpTrainGuiParam.m_bModelPoint2Visible = _ttoi(cXMLConfigurator.GetElemData());
	}
 	

	// model point2
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
	tmpTrainGuiParam.m_dCircleRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Arc start   
	bRet = cXMLConfigurator.FindElem(_T("CircleArcStart"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CircleArcStart = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	// Arc span   
	bRet = cXMLConfigurator.FindElem(_T("CircleArcSpan"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CircleArcSpan = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	// Caliper Num 
	bRet = cXMLConfigurator.FindElem(_T("CircleCaliperNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCircleCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("CircleProjectLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCircleCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("CircleSearchLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCircleCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Dirction
	bRet = cXMLConfigurator.FindElem(_T("CircleSearchDir"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());

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

	// CaliperNum1
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperNum1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nLineCaliperNum[0] = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("LineStartX1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineStartX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("LineStartY1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineStartY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("LineEndX1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineEndX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("LineEndY1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineEndY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("LineProjectLen1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineCaliperProjectLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("LineSearchLen1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineCaliperSearchLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("LineSearchRotation1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_LineCaliperSearchRotation[0] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	// LineCaliper Visible
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[0]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
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



	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible2"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint2 = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint2 = _ttoi(cXMLConfigurator.GetElemData());
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

	// Circle
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
	tmpTrainResult.m_dCircleCenterX = vPos.GetX();
	tmpTrainResult.m_dCircleCenterY = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("Radius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dCircleRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
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
bool CQuickLineCircleTool::SaveModelToFile(CString strDir)
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
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dCircleContrastThre);
	cXMLConfigurator.AddElem(_T("CircleContrastThre"), strInfo);
	// fileterhalfwidth
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lCircleFilterHalfWidth);
	cXMLConfigurator.AddElem(_T("CircleFilterHalfWidth"), strInfo);
	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nCircleOutLineNum);
	cXMLConfigurator.AddElem(_T("CircleOutLineNum"), strInfo);
	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_eCircledgeProperty);
	cXMLConfigurator.AddElem(_T("CircleEdgeProperty"), strInfo);
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bCircleSortByScore);
	cXMLConfigurator.AddElem(_T("CircleSortByScore"), strInfo);
	// DualEdge
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge);
	cXMLConfigurator.AddElem(_T("DualEdge"), strInfo);
	// EnableCircle
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCircle);
	cXMLConfigurator.AddElem(_T("EnableCircle"), strInfo);

	//////////////////////////////////////////////////////////////////////////
	// contrastthre1
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dLineContrastThre[0]);
	cXMLConfigurator.AddElem(_T("LineContrastThre1"), strInfo);
	// fileterhalfwidth1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lLineFilterHalfWidth[0]);
	cXMLConfigurator.AddElem(_T("LineFilterHalfWidth1"), strInfo);
	// outlinenum1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineOutLineNum[0]);
	cXMLConfigurator.AddElem(_T("LineOutLineNum1"), strInfo);
	// edgeproperty1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_eLinedgeProperty[0]);
	cXMLConfigurator.AddElem(_T("LineEdgeProperty1"), strInfo);
	// SortByScore1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bLineSortByScore[0]);
	cXMLConfigurator.AddElem(_T("LineSortByScore1"), strInfo);
	// LineAngleType1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType[0]);
	cXMLConfigurator.AddElem(_T("LineAngleType1"), strInfo);
	// EnableLine
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableLine[0]);
	cXMLConfigurator.AddElem(_T("EnableLine"), strInfo);


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
	// model point2 visible
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPoint2Visible);
	cXMLConfigurator.AddElem(_T("ModelPoint2Visible"), strInfo);
	// model point 2
	cXMLConfigurator.AddElem(_T("ModelPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
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
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleRadius);
	cXMLConfigurator.AddElem(_T("CircleRadius"), strInfo);
	// Arc start 
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleArcStart.ToDouble());
	cXMLConfigurator.AddElem(_T("CircleArcStart"), strInfo);
	// Arc Span
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleArcSpan.ToDouble());
	cXMLConfigurator.AddElem(_T("CircleArcSpan"), strInfo);
	// Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCircleCaliperNum);
	cXMLConfigurator.AddElem(_T("CircleCaliperNum"), strInfo);
	// Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleCaliperProjectLen);
	cXMLConfigurator.AddElem(_T("CircleProjectLen"), strInfo);
	// Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleCaliperSearchLen);
	cXMLConfigurator.AddElem(_T("CircleSearchLen"), strInfo);
	// Search Dircetion
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_CircleCaliperSearchDirection);
	cXMLConfigurator.AddElem(_T("CircleSearchDir"), strInfo);
	//CircleCaliper Visible1
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
	// Caliper Num1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nLineCaliperNum[0]);
	cXMLConfigurator.AddElem(_T("LineCaliperNum1"), strInfo);
	// startX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartX[0]);
	cXMLConfigurator.AddElem(_T("LineStartX1"), strInfo);
	// startY1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartY[0]);
	cXMLConfigurator.AddElem(_T("LineStartY1"), strInfo);
	// EndX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndX[0]);
	cXMLConfigurator.AddElem(_T("LineEndX1"), strInfo);
	// EndY1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndY[0]);
	cXMLConfigurator.AddElem(_T("LineEndY1"), strInfo);
	// Project Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperProjectLen[0]);
	cXMLConfigurator.AddElem(_T("LineProjectLen1"), strInfo);
	// Search Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperSearchLen[0]);
	cXMLConfigurator.AddElem(_T("LineSearchLen1"), strInfo);
	// Search Rotation1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_LineCaliperSearchRotation[0].ToDouble());
	cXMLConfigurator.AddElem(_T("LineSearchRotation1"), strInfo);
	//LineCaliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible[0]);
	cXMLConfigurator.AddElem(_T("LineCaliperVisible1"), strInfo);
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

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint2);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible2"), strInfo);
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

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
	cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
	cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterX);
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterY);
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleRadius);
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
bool CQuickLineCircleTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint2        = m_GuiModelPoint2.GetCenter();	
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nCircleCaliperNum		= m_GuiFindCircle.GetCaliperNum();
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
		m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius,
			m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
		m_tmpTrainGuiParam.m_dCircleCaliperProjectLen = m_GuiFindCircle.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCircleCaliperSearchLen	= m_GuiFindCircle.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bCircleCaliperVisible		= m_GuiFindCircle.IsCaliperVisible();
		m_GuiFindCircle.GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData);
		m_GuiFindCircle.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask);
		// Line
		for (int i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nLineCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], m_tmpTrainGuiParam.m_dLineEndX[i],
				m_tmpTrainGuiParam.m_dLineEndY[i]);
			m_tmpTrainGuiParam.m_bLineCaliperVisible[i]			= m_GuiFindLine[i].IsCaliperVisible();
		}
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变
		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiFindCircle.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindCircle.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindCircle.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
		m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius
			,m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
		m_GuiFindCircle.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
		m_GuiFindCircle.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCircleCaliperSearchLen);
		m_GuiFindCircle.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CircleCaliperSearchDirection);

		m_GuiFindCircle.SetCaliperVisible(m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetAnnulusFindCircleVisible(!m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData);
		for (int i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], m_tmpTrainGuiParam.m_dLineEndX[i],
				m_tmpTrainGuiParam.m_dLineEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]);

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
		}
	}

	return true;
}

bool CQuickLineCircleTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
		for (j=0; j<m_pGuiSearchCrossArray1.size(); j++)
		{
			m_pGuiSearchCrossArray1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArray1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
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

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		// 两条虚拟线段
		for (j=0; j<m_pGuiSearchLineArrayModelPoint.size(); j++)
		{
			m_pGuiSearchLineArrayModelPoint[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArrayModelPoint[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
		for (j=0; j<m_pGuiSearchLineArrayVirtural.size(); j++)
		{
			m_pGuiSearchLineArrayVirtural[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArrayVirtural[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
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

	}

	return true;
}

// 清除控件及其显示					
bool CQuickLineCircleTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

bool CQuickLineCircleTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CQuickLineCircleTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray1);

	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);

	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArrayModelPoint);
	ClearGraphicsArray(m_pGuiSearchLineArrayVirtural);

	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary1);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary2);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint2Array);

	return true;
}															

bool CQuickLineCircleTool::SetGuiByCam(double nWidth,double nHeight)
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

	//卡尺
	double sx,sy,ex,ey;
	m_GuiFindLine[0].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[0].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

	plength = m_GuiFindLine[0].GetCaliperProjectionLength();
	slength = m_GuiFindLine[0].GetCaliperSearchLength();
	m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	

	//参考点位置
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
}

bool CQuickLineCircleTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindCircle"));

		for (int j=0; j<QUICK_LINE_CIRCLE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;	
}

bool CQuickLineCircleTool::ClearResultGui()
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
		for (j=0; j<m_pGuiSearchCrossArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		// 两条虚拟线段
		for (j=0; j<m_pGuiSearchLineArrayModelPoint.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArrayModelPoint[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchLineArrayVirtural.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArrayVirtural[j]->GetTipText());
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

		for (j=0; j<m_pGuiSearchResultFitPoint2Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint2Array[j]->GetTipText());
		}
	}

	return true;	
}

// 获取训练模板原始图像
bool CQuickLineCircleTool::GetPatternImage(cpImage &ImagePattern)
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
bool CQuickLineCircleTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CQuickLineCircleTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CQuickLineCircleTool::SetLanguage(int nLanguage)
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
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}

	for (int i=0; i<QUICK_LINE_CIRCLE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d"), i+1);
			break;
		case 1:
			str.Format(_T("找线工具%d"), i+1);
			break;
		default:
			str.Format(_T("找线工具%d"), i+1);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}
}
CString CQuickLineCircleTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:			//	几何定位失败			
				strErrInfo += _T("Quick search failed");
				break;
			case -2:			// 	找圆失败			
				strErrInfo += _T("Find circle failed");
				break;
			case -3:			// 找线失败				
				strErrInfo += _T("Find line failed");
				break;
			case -4:							
				strErrInfo += _T("Intersect failed");
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
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							
				strErrInfo += _T("找圆失败");
				break;
			case -3:							
				strErrInfo += _T("找线失败");
				break;
			case -4:							
				strErrInfo += _T("交点距离异常");
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
				strErrInfo += _T("找圆失败");
				break;
			case -3:							
				strErrInfo += _T("找线失败");
				break;
			case -4:							
				strErrInfo += _T("交点距离异常");
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