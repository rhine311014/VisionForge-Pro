// QuickCircle4LineSearchTool.cpp: implementation of the CPatternCircleLineSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickCircle4LineSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"
#include "svImageSharpness.h"


CQuickCircle4LineSearchTool::CQuickCircle4LineSearchTool(void)
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

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


	m_GuiFindLine[0].SetStartXYEndXY(100 , 100, 100 , 200);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[1].SetStartXYEndXY(300 , 100,200 , 100);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[2].SetStartXYEndXY(400 , 200, 400 , 100);
	m_GuiFindLine[2].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[3].SetStartXYEndXY(200 , 200, 300 , 200);
	m_GuiFindLine[3].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	
	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindCircle.SetCaliperVisible(FALSE);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);

	int i=0;
	for (i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
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

	m_tmpTrainDataParam.m_dGrau				= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainAffineRect	=   m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	// 找线工具参数
	for (i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
			m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_edgeProperty[i]           = eLightToDark;
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 10;
		m_tmpTrainGuiParam.m_nCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
			m_tmpTrainGuiParam.m_dEndY[i]);
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bLineCaliperVisible[i]		= m_GuiFindLine[i].IsCaliperVisible();
		m_tmpTrainGuiParam.m_bEnableLineMask[i]			= false;
	}



	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();


	// 找圆工具参数初始化
	m_FindCircleTool.GetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_CircleedgeProperty, 
		m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
	m_tmpTrainDataParam.m_nCircleOutLineNum			= 10;
	m_tmpTrainGuiParam.m_nCircleCaliperNum			= m_GuiFindCircle.GetCaliperNum();
	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius,
		m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
	m_tmpTrainGuiParam.m_dCircleCaliperProjectLen		= m_GuiFindCircle.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCircleCaliperSearchLen		= m_GuiFindCircle.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bCircleCaliperVisible			= m_GuiFindCircle.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bEnableCircleMask				= false;

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CQuickCircle4LineSearchTool::CQuickCircle4LineSearchTool(CQuickCircle4LineSearchTool &cPatternCircleLineSearchTool)
{
	if (cPatternCircleLineSearchTool.GetSearchToolType() != eQuickCircle4LineSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindCircle.SetCaliperVisible(FALSE);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);
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

	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
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
	m_CamHeight								= cPatternCircleLineSearchTool.m_CamHeight;
	m_CamWidth								= cPatternCircleLineSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternCircleLineSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternCircleLineSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternCircleLineSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternCircleLineSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternCircleLineSearchTool.m_TrainResult;
	m_ImageTrain							= cPatternCircleLineSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternCircleLineSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternCircleLineSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternCircleLineSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternCircleLineSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternCircleLineSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternCircleLineSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cPatternCircleLineSearchTool.UpdateSearchGuiData(true);
	cPatternCircleLineSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternCircleLineSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternCircleLineSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternCircleLineSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternCircleLineSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternCircleLineSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternCircleLineSearchTool.m_tmpImageMask);

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

CQuickCircle4LineSearchTool& CQuickCircle4LineSearchTool::operator=(CQuickCircle4LineSearchTool &cPatternCircleLineSearchTool)
{
	if (this == &cPatternCircleLineSearchTool)
	{
		return *this;	
	}

	if (cPatternCircleLineSearchTool.GetSearchToolType() != eQuickCircle4LineSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindCircle.SetCaliperVisible(FALSE);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
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

	m_GuiModelPoint.SetLabelVisible(TRUE);
	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
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
	m_CamHeight								= cPatternCircleLineSearchTool.m_CamHeight;
	m_CamWidth								= cPatternCircleLineSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternCircleLineSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternCircleLineSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternCircleLineSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternCircleLineSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternCircleLineSearchTool.m_TrainResult;

	m_ImageTrain							= cPatternCircleLineSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternCircleLineSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternCircleLineSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternCircleLineSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternCircleLineSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternCircleLineSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternCircleLineSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternCircleLineSearchTool.UpdateSearchGuiData(true);
	cPatternCircleLineSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternCircleLineSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternCircleLineSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternCircleLineSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternCircleLineSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternCircleLineSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternCircleLineSearchTool.m_tmpImageMask);
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

CQuickCircle4LineSearchTool::~CQuickCircle4LineSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CQuickCircle4LineSearchTool::GetSearchToolType()
{
	return eQuickCircle4LineSearchTool;
}

// 设置显示控件指针		
bool CQuickCircle4LineSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
	UpdateTrainResultGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练结果显示控件指针
bool CQuickCircle4LineSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CQuickCircle4LineSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickCircle4LineSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CQuickCircle4LineSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CQuickCircle4LineSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CQuickCircle4LineSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CQuickCircle4LineSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CQuickCircle4LineSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CQuickCircle4LineSearchTool::UpdateTrainGuiDisplay()
{	
	// 区域定位参数
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	// 卡尺参数
	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible && m_tmpTrainDataParam.m_bEnableLine[i]);
		if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
	}
	// 圆参数
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
		// 区域定位工具参数
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	

		// 卡尺参数
		for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			if(m_tmpTrainDataParam.m_bEnableLine[i])
			{
				m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
			}
			
		}

		// 圆工具参数
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

bool CQuickCircle4LineSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		// 显示直线结果
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));

		//显示圆的结果
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle, m_strKeyText+_T("_TrainResultCircle"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CQuickCircle4LineSearchTool::UpdateSearchResultGuiDisplay()							
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
		
	// 圆参数
	for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible);
	}

	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible);
	}
	//拟合参数
	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		if (m_bSearchResultGuiShow) // 立即更新
		{
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

			// 显示圆的结果
			for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleArray[i], m_pGuiSearchCircleArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterArray[i], m_pGuiSearchCircleCenterArray[i]->GetTipText());
			}
			// 拟合点
			for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array[i], m_pGuiSearchResultFitPoint1Array[i]->GetTipText());
			}
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CQuickCircle4LineSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CQuickCircle4LineSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CQuickCircle4LineSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CQuickCircle4LineSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	*((CQuickCircle4LineSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CQuickCircle4LineSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CQuickCircle4LineSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CQuickCircle4LineSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CQuickCircle4LineSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CQuickCircle4LineSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CQuickCircle4LineSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CQuickCircle4LineSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CQuickCircle4LineSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CQuickCircle4LineSearchTool::Train()
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
		m_bTrainOK = bRet;

		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		//计算清晰度
		scImageSharpness tool;
		double dScoreSharp = 0;
		bool bSharpExecute = false;
		if(bIsProcessImage)
		{
			bSharpExecute = tool.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);
		}
		else
		{
			bSharpExecute = tool.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainAffineRect.BoundingBox(),dScoreSharp);
		}
		if (bSharpExecute)
		{
			m_tmpTrainResult.m_dSharpness = dScoreSharp;
		}
		else
		{
			m_tmpTrainResult.m_dSharpness = 1;
		}


		// 找线0
		int nNumLine = 0;
		for(int nIndex = 0;nIndex < QUICK_CIRCLE_4LINE_NUM; nIndex++)
		{
			if (!m_tmpTrainDataParam.m_bEnableLine[nIndex])
			{
				continue;
			}
			m_FindLineTool[nIndex].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[nIndex]);
			m_FindLineTool[nIndex].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[nIndex], m_tmpTrainGuiParam.m_CaliperSearchRotation[nIndex], m_tmpTrainGuiParam.m_dCaliperSearchLen[nIndex]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[nIndex]);
			//m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex], m_tmpTrainDataParam.m_edgeProperty[nIndex], m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			m_FindLineTool[nIndex].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[nIndex], eRansacRobust);
			m_FindLineTool[nIndex].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore[nIndex]);
			m_FindLineTool[nIndex].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[nIndex], m_tmpTrainGuiParam.m_dStartY[nIndex], 
				m_tmpTrainGuiParam.m_dEndX[nIndex], m_tmpTrainGuiParam.m_dEndY[nIndex]);
			
			//掩膜与双极性
			m_FindLineTool[nIndex].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[nIndex]);
			if (m_tmpTrainDataParam.m_bEnableLineDualEdge[nIndex])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeProperty[nIndex] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeProperty[nIndex] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex],
					m_tmpTrainDataParam.m_edgeProperty[nIndex],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			} 
			else
			{
				m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex],
					m_tmpTrainDataParam.m_edgeProperty[nIndex], m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			}
			
			scLineSeg cLineSeg1;
			double dRMS1;
			if(bIsProcessImage)
			{
				bRet = m_FindLineTool[nIndex].Execute(tmpImage, cLineSeg1, dRMS1);
			}
			else
			{
				bRet = m_FindLineTool[nIndex].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-2-nIndex;
				return false;
			}
			nNumLine += 1;

			if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == nIndex)	// 任意角度
			{
				m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == 1)	//水平线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();

				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP1().GetY();
				}
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == 2)	//垂直线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();

				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP1().GetY();
				}
			}
			else
			{
				m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
			}
			m_GuiTrainResultLine[nIndex].SetLine(cLineSeg1.GetLine());
		}
		
		if (nNumLine == 0)
			return false;

		// 找圆
		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
		//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum);
		m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore);
		m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius
			,m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
		//掩膜与双极性
		m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);
		if (m_tmpTrainDataParam.m_bEnableCircleDualEdge)
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_CircleedgeProperty == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_CircleedgeProperty == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, 
				m_tmpTrainDataParam.m_CircleedgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		} 
		else
		{
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre,
				m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		}
		
		scCircle cCircle;
		double   dRMS2;
		if(bIsProcessImage)
		{
			bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS2);
		}
		else
		{
			bRet = m_FindCircleTool.Execute(m_tmpImageTrain, cCircle, dRMS2);
		}
		if (!bRet)
		{
			m_nResultErrNumber=-6;
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dRadius  = cCircle.GetRadius();


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

		// 找线0
		int nNumLine = 0;
		for(int nIndex = 0;nIndex < QUICK_CIRCLE_4LINE_NUM; nIndex++)
		{
			if (!m_tmpTrainDataParam.m_bEnableLine[nIndex])
			{
				continue;
			}

			m_FindLineTool[nIndex].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[nIndex]);
			m_FindLineTool[nIndex].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[nIndex], m_tmpTrainGuiParam.m_CaliperSearchRotation[nIndex], m_tmpTrainGuiParam.m_dCaliperSearchLen[nIndex]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[nIndex]);
			//m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex], m_tmpTrainDataParam.m_edgeProperty[nIndex], m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			m_FindLineTool[nIndex].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[nIndex], eRansacRobust);
			m_FindLineTool[nIndex].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore[nIndex]);
			m_FindLineTool[nIndex].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[nIndex], m_tmpTrainGuiParam.m_dStartY[nIndex], 
				m_tmpTrainGuiParam.m_dEndX[nIndex], m_tmpTrainGuiParam.m_dEndY[nIndex]);
			//掩膜与双极性
			m_FindLineTool[nIndex].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[nIndex]);
			if (m_tmpTrainDataParam.m_bEnableLineDualEdge[nIndex])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeProperty[nIndex] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeProperty[nIndex] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex],
					m_tmpTrainDataParam.m_edgeProperty[nIndex],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			} 
			else
			{
				m_FindLineTool[nIndex].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[nIndex],
					m_tmpTrainDataParam.m_edgeProperty[nIndex], m_tmpTrainDataParam.m_lFilterHalfWidth[nIndex]);
			}
			scLineSeg cLineSeg1;
			double dRMS1;
			if(bIsProcessImage)
			{
				bRet = m_FindLineTool[nIndex].Execute(tmpImage, cLineSeg1, dRMS1);
			}
			else
			{
				bRet = m_FindLineTool[nIndex].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-2-nIndex;
				return false;
			}
			nNumLine += 1;



			if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == nIndex)	// 任意角度
			{
				m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == 1)	//水平线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();

				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP1().GetY();
				}
			}
			else if (m_tmpTrainDataParam.m_nLineAngleType[nIndex] == 2)	//垂直线
			{
				scDegree degLineAngle;
				degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();

				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
				}
				else
				{
					m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP2().GetX();
					m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP2().GetY();
					m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP1().GetX();
					m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP1().GetY();
				}
			}
			else
			{
				m_tmpTrainResult.m_dStartX[nIndex] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[nIndex] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[nIndex] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[nIndex] = cLineSeg1.GetP2().GetY();
			}
			m_GuiTrainResultLine[nIndex].SetLine(cLineSeg1.GetLine());
		}

		if(nNumLine == 0)
			return false;

		// 找圆
		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
		//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum);
		m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore);
		m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius
			,m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
		//掩膜与双极性
		m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);
		if (m_tmpTrainDataParam.m_bEnableCircleDualEdge)
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_CircleedgeProperty == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_CircleedgeProperty == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, 
				m_tmpTrainDataParam.m_CircleedgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		} 
		else
		{
			m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre,
				m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
		}
		
		scCircle cCircle;
		double   dRMS2;
		if(bIsProcessImage)
		{
			bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS2);
		}
		else
		{
			bRet = m_FindCircleTool.Execute(m_tmpImageTrain, cCircle, dRMS2);
		}
		if (!bRet)
		{
			m_nResultErrNumber=-6;
			return false;
		}
		m_tmpTrainResult.m_dCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dRadius  = cCircle.GetRadius();

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
bool CQuickCircle4LineSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CQuickCircle4LineSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	*((CQuickCircle4LineSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CQuickCircle4LineSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CQuickCircle4LineSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CQuickCircle4LineSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CQuickCircle4LineSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	*((CQuickCircle4LineSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CQuickCircle4LineSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CQuickCircle4LineSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CQuickCircle4LineSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickCircle4LineSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CQuickCircle4LineSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CQuickCircle4LineSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber = -1;
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
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
	{
		if(m_tmpTrainDataParam.m_bEnableLine[i])
		{
			m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
			m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
			,m_TrainGuiParam.m_dCaliperProjectLen[i]);
			//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
			m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
			m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
			//掩膜与双极性
			m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[i]);
			if (m_tmpTrainDataParam.m_bEnableLineDualEdge[i])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeProperty[i] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeProperty[i] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i],
					m_tmpTrainDataParam.m_edgeProperty[i],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
			} 
			else
			{
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i],
					m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
			}
		}

	}

	// 设置圆的参数
	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCircleCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum, m_TrainGuiParam.m_CircleCaliperSearchDirection, m_TrainGuiParam.m_dCircleCaliperSearchLen
		,m_TrainGuiParam.m_dCircleCaliperProjectLen);
	//m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre, m_TrainDataParam.m_CircleedgeProperty, m_TrainDataParam.m_lCircleFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nCircleOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bCircleSortByScore);
	//掩膜与双极性
	m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);
	if (m_tmpTrainDataParam.m_bEnableCircleDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_CircleedgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_CircleedgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, 
			m_tmpTrainDataParam.m_CircleedgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
	} 
	else
	{
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre,
			m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
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

			// 求变换矩阵
			sc2Vector pos = tmpQuickResult.GetPosition();
			sc2Vector tmModelPoint = sc2Vector(0,0);
			double dRotation = 0;
			double dTransX = 0;
			double dTransY = 0;
			m_QuickSearchTool.GetModelOrigin(tmModelPoint);
			dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());


			// 1.求线的结果
			scLineSeg arrLineSeg[QUICK_CIRCLE_4LINE_NUM];
			double dRMS[QUICK_CIRCLE_4LINE_NUM];
			bool   bFindLine[QUICK_CIRCLE_4LINE_NUM];
			for (int k=0; k<QUICK_CIRCLE_4LINE_NUM; k++)
			{
				bFindLine[k] = 1;
			}

			for (int j=0; j<QUICK_CIRCLE_4LINE_NUM; j++)
			{
				// 求找线工具变换后的位置,找线
				double dTransStartX, dTransStartY;
				double dTransEndX, dTransEndY;
				dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dStartX[j]  - sin(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransX;
				dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dStartX[j]  + cos(dRotation) * m_TrainGuiParam.m_dStartY[j] + dTransY;
				dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dEndX[j]  - sin(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransX;
				dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dEndX[j]  + cos(dRotation) * m_TrainGuiParam.m_dEndY[j] + dTransY;
				m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[j], /*scRadian(dRotation)+*/m_TrainGuiParam.m_CaliperSearchRotation[j], m_TrainGuiParam.m_dCaliperSearchLen[j]
				,m_TrainGuiParam.m_dCaliperProjectLen[j]);
				m_FindLineTool[j].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);

				// 执行找线
				if (m_tmpTrainDataParam.m_bEnableLine[j])
				{
					//bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, arrLineSeg[j], dRMS[j]); // 成功
					if(bIsProcessImage)
					{
						bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, arrLineSeg[j], dRMS[j]); 
					}
					else
					{
						bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, arrLineSeg[j], dRMS[j]); 
					}
				}
				
			}



			// 2.找圆的结果
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

			
			scCircle ResCircle;
			double dRMS2;
			bool retCircle = false;
			//retCircle = m_FindCircleTool.Execute(ImageSearch, ResCircle, dRMS2); // 执行找圆
			if(bIsProcessImage)
			{
				retCircle = m_FindCircleTool.Execute(tmpImage, ResCircle, dRMS2); // 执行找圆
			}
			else
			{
				retCircle = m_FindCircleTool.Execute(ImageSearch, ResCircle, dRMS2); // 执行找圆
			}

			if (!bFindLine[0] )// 找线1失败
			{
				m_nResultErrNumber = -2;
				return false;
			}
			if (!bFindLine[1] )// 找线2失败
			{
				m_nResultErrNumber = -3;
				return false;
			}
			if (!bFindLine[2] )// 找线3失败
			{
				m_nResultErrNumber = -4;
				return false;
			}
			if (!bFindLine[3] )// 找线4失败
			{
				m_nResultErrNumber = -5;
				return false;
			}
			

			if(!retCircle)// 找圆失败
			{
				m_nResultErrNumber = -6;
				return false;
			}

			// 显示四条线
			CSearchResult searchResult;
			CString strTmpKey;
			int nNumLine   = 0;
			int nAuIndex   = 0;
			for (int nIndexLine = 0; nIndexLine < QUICK_CIRCLE_4LINE_NUM; nIndexLine++)
			{
				if(m_tmpTrainDataParam.m_bEnableLine[nIndexLine])
				{
					strTmpKey.Format(_T("%s_ResultChordLine%d_%d"), m_strKeyText, nIndexLine, i);
					scGuiLine *pTemLine = m_GuiSearchLine+nIndexLine;
					pTemLine->SetXYRotation(arrLineSeg[nIndexLine].GetLine().GetPos(),arrLineSeg[nIndexLine].GetRotation());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					searchResult.m_ResultLine.m_vLineX[nAuIndex] = arrLineSeg[nIndexLine].GetP1().GetX();
					searchResult.m_ResultLine.m_vLineY[nAuIndex] = arrLineSeg[nIndexLine].GetP1().GetY();
					nAuIndex +=1;
					searchResult.m_ResultLine.m_vLineX[nAuIndex] = arrLineSeg[nIndexLine].GetP2().GetX();
					searchResult.m_ResultLine.m_vLineY[nAuIndex] = arrLineSeg[nIndexLine].GetP2().GetY();
					nAuIndex +=1;
					nNumLine += 1;
				}
			}


			if(nNumLine == 0){
				m_nResultErrNumber = -7;
				return false;
			}

			searchResult.m_ResultLine.m_nLineNum = nAuIndex;
			// 圆的结果显示
			strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
			scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
			pTemCircle->SetCircle(ResCircle);
			pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemCircle->SetTipTextEnabled(true);
			pTemCircle->SetTipText(strTmpKey);
			m_pGuiSearchCircleArray.push_back(pTemCircle);

			//归一化到0度附近
			double dLine0Angle = 0;
			if (m_tmpTrainDataParam.m_bEnableLine[0] )
				dLine0Angle =scDegree(arrLineSeg[0].GetRotation().NormMod180()).ToDouble() - 90;

			double dLine1Angle = 0;
			if (m_tmpTrainDataParam.m_bEnableLine[1] )
				dLine1Angle= scDegree(arrLineSeg[1].GetRotation().SignedNormMod180()).ToDouble();

			double dLine2Angle =0 ;
			if (m_tmpTrainDataParam.m_bEnableLine[2] )
				dLine2Angle = scDegree(arrLineSeg[2].GetRotation().NormMod180()).ToDouble()- 90;

			double dLine3Angle =0 ;
			if (m_tmpTrainDataParam.m_bEnableLine[3] )
				dLine3Angle = scDegree(arrLineSeg[3].GetRotation().SignedNormMod180()).ToDouble();

			double dHoriAngle = (dLine0Angle + dLine1Angle + dLine2Angle + dLine3Angle) / nNumLine;


			// 5.4.显示直线拟合的点
			for (int Index = 0; Index < QUICK_CIRCLE_4LINE_NUM; Index++)
			{
				if (m_tmpSearchGuiParam.m_bEnableShowFitLinePoint[Index])
				{

					if(!m_tmpTrainDataParam.m_bEnableLine[Index])
						continue;

					std::vector<bool> bUsed;
					std::vector<sc2Vector> points;
					m_FindLineTool[Index].GetUsed(bUsed);
					m_FindLineTool[Index].GetFitData(points);
					for (long m=0; m<bUsed.size(); m++)
					{
						bool bUse = bUsed[m];
						sc2Vector pt = points[m];

						int nIndex = m_pGuiSearchResultFitPoint1Array.size();
						strTmpKey.Format(_T("%s_LineFitPoint1_%d"), m_strKeyText, nIndex);
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
			}


			// 启用夹角判断
			if (m_tmpSearchDataParam.m_bEnable2LineAngleJudge)
			{
				scRadian Line2Angle01(0);
				scRadian Line2Angle21(0);
				scRadian Line2Angle23(0);
				scRadian Line2Angle03(0);
				scRadian Line2Angle02(0);
				scRadian Line2Angle13(0);
				int LineNum = 0;
				for (int i =0 ; i <4 ;i++)
				{
					if (m_tmpTrainDataParam.m_bEnableLine[i])
					{
						LineNum++;
					}
				}
				switch (LineNum)
				{
				case 0:
					break;
				case 1:
					break;
				case 2:
					if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[1]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[1].GetLine(),Line2Angle01);
						if( (fabs(scDegree(Line2Angle01).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					} 
					else if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[3].GetLine(),Line2Angle03);
						if( (fabs(scDegree(Line2Angle03).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[2]==true)&&(m_tmpTrainDataParam.m_bEnableLine[1]==true))
					{
						sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[1].GetLine(),Line2Angle21);
						if( (fabs(scDegree(Line2Angle21).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[2]==true)&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[3].GetLine(),Line2Angle23);
						if( (fabs(scDegree(Line2Angle23).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[2]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[1].GetLine(),Line2Angle02);
						if( (fabs(scDegree(Line2Angle02).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[1]==true)&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[1].GetLine(),arrLineSeg[3].GetLine(),Line2Angle13);
						if( (fabs(scDegree(Line2Angle13).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					break;
				case 3:
					if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[1]==true)
						&&(m_tmpTrainDataParam.m_bEnableLine[2]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[1].GetLine(),Line2Angle01);
						sfAngleLine2Line(arrLineSeg[1].GetLine(),arrLineSeg[2].GetLine(),Line2Angle21);
						if( (fabs(scDegree(Line2Angle01).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
							(fabs(scDegree(Line2Angle21).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange) )
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[1]==true)
						&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[1].GetLine(),Line2Angle01);
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[3].GetLine(),Line2Angle03);
						if( (fabs(scDegree(Line2Angle01).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
							(fabs(scDegree(Line2Angle03).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[1]==true)&&(m_tmpTrainDataParam.m_bEnableLine[2]==true)
						&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[1].GetLine(),Line2Angle21);
						sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[3].GetLine(),Line2Angle23);
						if( (fabs(scDegree(Line2Angle21).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
							(fabs(scDegree(Line2Angle23).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					else if ((m_tmpTrainDataParam.m_bEnableLine[0]==true)&&(m_tmpTrainDataParam.m_bEnableLine[2]==true)
						&&(m_tmpTrainDataParam.m_bEnableLine[3]==true))
					{
						sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[3].GetLine(),Line2Angle03);
						sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[3].GetLine(),Line2Angle23);
						if( (fabs(scDegree(Line2Angle03).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
							(fabs(scDegree(Line2Angle23).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange))
						{
							m_nResultErrNumber = -9;
							return false;
						}
					}
					break;
				case 4:
					sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[1].GetLine(),Line2Angle01);
					sfAngleLine2Line(arrLineSeg[0].GetLine(),arrLineSeg[3].GetLine(),Line2Angle03);
					sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[1].GetLine(),Line2Angle21);
					sfAngleLine2Line(arrLineSeg[2].GetLine(),arrLineSeg[3].GetLine(),Line2Angle23);
					if( (fabs(scDegree(Line2Angle01).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
						(fabs(scDegree(Line2Angle03).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
						(fabs(scDegree(Line2Angle21).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange)||
						(fabs(scDegree(Line2Angle23).NormMod180().ToDouble()-90) > m_tmpSearchDataParam.m_dAngleRange) )
					{
						m_nResultErrNumber = -9;
						return false;
					}
					break;
				}
			}

			if (m_tmpSearchDataParam.m_bEnableLineAngleJudge)
			{
				double dAngleDiff = 0;
				if (m_tmpTrainDataParam.m_bEnableLine[0])
				{
					dAngleDiff = dHoriAngle - dLine0Angle;
					if (abs(dAngleDiff) > m_tmpSearchDataParam.m_dJudgeLineAngle)
					{
						m_nResultErrNumber = -8;
						return false;
					}
				}

				if (m_tmpTrainDataParam.m_bEnableLine[1])
				{
					dAngleDiff = dHoriAngle - dLine1Angle;
					if (abs(dAngleDiff) > m_tmpSearchDataParam.m_dJudgeLineAngle)
					{
						m_nResultErrNumber = -8;
						return false;
					}
				}

				if (m_tmpTrainDataParam.m_bEnableLine[2])
				{
					dAngleDiff = dHoriAngle - dLine2Angle;
					if (abs(dAngleDiff) > m_tmpSearchDataParam.m_dJudgeLineAngle)
					{
						m_nResultErrNumber = -8;
						return false;
					}
				}

				if (m_tmpTrainDataParam.m_bEnableLine[3])
				{
					dAngleDiff = dHoriAngle - dLine3Angle;
					if (abs(dAngleDiff) > m_tmpSearchDataParam.m_dJudgeLineAngle)
					{
						m_nResultErrNumber = -8;
						return false;
					}
				}
			}


			//输出垂直线或者水平线
			double dResultAngle = 0;
			if (m_tmpSearchDataParam.m_nSearchOutLineAngleType == 1)	//垂直线
			{
				dResultAngle = dHoriAngle + 90;
			}
			else
			{
				dResultAngle =dHoriAngle;
			}

			// 4.2根据圆形
			scLine fixLine;
			fixLine.SetPos(ResCircle.GetCenter());
			scRadian rotation(dResultAngle/180.0*skPI);
			fixLine.SetRotation(rotation);

			// 4.3修正后的直线与圆的交点
			bool bIntersect = false;
			std::vector<sc2Vector> vCrossPointArray; 
			sfInterSectLineCircle(fixLine,ResCircle,bIntersect,vCrossPointArray);
			if (vCrossPointArray.size() != 2)
				return false;

			
			if (m_tmpSearchDataParam.m_nSearchOutPointType == 0)//左边的点
			{
				if (vCrossPointArray[0].GetX() < vCrossPointArray[1].GetX())
				{
					searchResult.m_dPosX = vCrossPointArray[0].GetX();
					searchResult.m_dPosY = vCrossPointArray[0].GetY();
				}
				else
				{
					searchResult.m_dPosX = vCrossPointArray[1].GetX();
					searchResult.m_dPosY = vCrossPointArray[1].GetY();
				}
			}
			else if (m_tmpSearchDataParam.m_nSearchOutPointType == 1)//右边的点
			{
				if (vCrossPointArray[0].GetX() < vCrossPointArray[1].GetX())
				{
					searchResult.m_dPosX = vCrossPointArray[1].GetX();
					searchResult.m_dPosY = vCrossPointArray[1].GetY();
				}
				else
				{
					searchResult.m_dPosX = vCrossPointArray[0].GetX();
					searchResult.m_dPosY = vCrossPointArray[0].GetY();
				}
			}
			else if (m_tmpSearchDataParam.m_nSearchOutPointType == 2)//上边的点
			{
				if (vCrossPointArray[0].GetY() < vCrossPointArray[1].GetY())
				{
					searchResult.m_dPosX = vCrossPointArray[0].GetX();
					searchResult.m_dPosY = vCrossPointArray[0].GetY();
				}
				else
				{
					searchResult.m_dPosX = vCrossPointArray[1].GetX();
					searchResult.m_dPosY = vCrossPointArray[1].GetY();
				}
			}
			else if (m_tmpSearchDataParam.m_nSearchOutPointType == 3)//下边的点
			{
				if (vCrossPointArray[0].GetY() < vCrossPointArray[1].GetY())
				{
					searchResult.m_dPosX = vCrossPointArray[1].GetX();
					searchResult.m_dPosY = vCrossPointArray[1].GetY();
				}
				else
				{
					searchResult.m_dPosX = vCrossPointArray[0].GetX();
					searchResult.m_dPosY = vCrossPointArray[0].GetY();
				}
			}
			searchResult.m_dScore = tmpQuickResult.GetScore();
			searchResult.m_dAngle = dResultAngle;

			//获取搜索圆定位工具的圆心和半径
			searchResult.m_resultCircleSearchTool.m_scCircleCircle  = ResCircle.GetCenter();
			searchResult.m_resultCircleSearchTool.m_dRadius         = ResCircle.GetRadius();


			// 设置结果
			CResultSearchRect resRect;
			resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
			searchResult.m_vTrainSearchRect.push_back(resRect);
			// 线
			for (int nLine = 0; nLine < QUICK_CIRCLE_4LINE_NUM; nLine++)
			{
				CResultLine line;
				line.m_vLineX[0] = arrLineSeg[nLine].GetP1().GetX();
				line.m_vLineY[0] = arrLineSeg[nLine].GetP1().GetY();
				line.m_vLineX[1] = arrLineSeg[nLine].GetP2().GetX();
				line.m_vLineY[1] = arrLineSeg[nLine].GetP2().GetY();
				searchResult.m_vLineSearchResult.push_back(line);
			}
			CResultCircleSearchTool circle;
			circle.m_scCircleCircle = ResCircle.GetCenter();
			circle.m_dRadius = ResCircle.GetRadius();
			searchResult.m_vCircleSearchResult.push_back(circle);

			m_SearchResultArray.push_back(searchResult);

			//////////////////////////////////////显示结果的gui////////////////////////////
			// 5.1.显示粗定位和参考点
			strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
			pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchRectArray.push_back(pTempAffineRect);

			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross+i;
			pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			m_pGuiSearchCrossArray.push_back(pTempCross);

			// 5.2.显示输出点
			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, 1);
			scGuiCross *pTempLineCross = m_GuiSearchLineCross +1;
			pTempLineCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(60,60));
			pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_ChordLineColor);
			pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempLineCross->SetTipTextEnabled(true);
			pTempLineCross->SetTipText(strTmpKey);
			CString stCross;
			stCross.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_dPosX,searchResult.m_dPosY, searchResult.m_dAngle);
			pTempLineCross->SetLabel(stCross);
			pTempLineCross->SetLabelVisible(TRUE);
			m_pGuiSearchLineCrossArray.push_back(pTempLineCross);

			// 5.3.显示修正后的直线
			strTmpKey.Format(_T("%s_ResultFixLine%d_%d"), m_strKeyText, 4, i);
			scGuiLine *pTemLine = m_GuiSearchLine + 4;
			pTemLine->SetXYRotation(fixLine.GetPos(),fixLine.GetRotation());
			pTemLine->SetLineColor(m_tmpSearchGuiParam.m_cLineColorFixLine);
			pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemLine->SetTipTextEnabled(true);
			pTemLine->SetTipText(strTmpKey);
			m_pGuiSearchLineArray.push_back(pTemLine);

			


			
			

			// 5.7.显示圆的拟合点
			if(m_tmpSearchGuiParam.m_bEnableShowFitCirclePoint)
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
					strTmpKey.Format(_T("%s_CircleFitPoint1_%d"), m_strKeyText, nIndex);
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
		}	
		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		m_nResultErrNumber = -1;
		return false;
	}

	return bRet;
}

// 获取搜索结果个数										
int CQuickCircle4LineSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CQuickCircle4LineSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CQuickCircle4LineSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eQuickCircle4LineSearchTool)
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
bool CQuickCircle4LineSearchTool::LoadModelFromFile(CString strDir)
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
	CQuickCircle4LineSearchTrainDataParam tmpTrainDataParam;
	CQuickCircle4LineSearchTrainGuiParam tmpTrainGuiParam;
	CQuickCircle4LineSearchSearchDataParam tmpSearchDataParam;
	CQuickCircle4LineSearchSearchGuiParam tmpSearchGuiParam;	
	CQuickCircle4LineSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eQuickCircle4LineSearchTool)
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

	//圆的训练参数
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
		tmpTrainDataParam.m_lCircleFilterHalfWidth = 1;
	}
	else
	{
		tmpTrainDataParam.m_lCircleFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	}
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
	tmpTrainDataParam.m_CircleedgeProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());


	// SortByScore
	bRet = cXMLConfigurator.FindElem(_T("CircleSortByScore"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bCircleSortByScore = true;
	}
	tmpTrainDataParam.m_bCircleSortByScore = _ttoi(cXMLConfigurator.GetElemData());

	// DualEdge
	bRet = cXMLConfigurator.FindElem(_T("CircleDualEdge"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableCircleDualEdge = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableCircleDualEdge = _ttoi(cXMLConfigurator.GetElemData());
	}

	//直线的训练参数
	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM;index++)
	{
		CString strName;
		strName = "ContrastThre";
		strName.AppendFormat("%d",index + 1);
		// contrastthre1
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}

		tmpTrainDataParam.m_dContrastThre[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// FilterHalfWidth1
		strName = "FilterHalfWidth";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			tmpTrainDataParam.m_lFilterHalfWidth[index] = 1;
		}
		else
		{
			tmpTrainDataParam.m_lFilterHalfWidth[index] = _ttol(cXMLConfigurator.GetElemData());
		}
		// outlinenum1
		strName = "OutLineNum";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_nOutLineNum[index] = _ttol(cXMLConfigurator.GetElemData());
		// edgeproperty1
		strName = "EdgeProperty";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_edgeProperty[index] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
		// SortByScore1
		strName = "SortByScore";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (bRet)
		{
			tmpTrainDataParam.m_bSortByScore[index] = _ttoi(cXMLConfigurator.GetElemData());
		}
		else
		{
			tmpTrainDataParam.m_bSortByScore[index] = true;	
		}


		// LineAngleType1
		strName = "LineAngleType";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (bRet)
		{
			tmpTrainDataParam.m_nLineAngleType[index] = _ttol(cXMLConfigurator.GetElemData());
		}
		else
		{
			tmpTrainDataParam.m_nLineAngleType[index] = 0 ;
		}

		// DualEdge1
		strName = _T("LineDualEdge");
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			tmpTrainDataParam.m_bEnableLineDualEdge[index] = false;
		}
		else
		{
			tmpTrainDataParam.m_bEnableLineDualEdge[index] = _ttoi(cXMLConfigurator.GetElemData());
		}

		// EnableLine
		strName = "EnableLine";
		strName.AppendFormat("%d",index + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (bRet)
		{
			tmpTrainDataParam.m_bEnableLine[index] = _ttol(cXMLConfigurator.GetElemData());
		}
	}
	

	//Train Gui Param
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

	//圆 train gui参数
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

	// CircleCaliperMaskEnable
	bRet = cXMLConfigurator.FindElem(_T("CircleCaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableCircleMask = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableCircleMask = false;
	}
	bRet = cXMLConfigurator.FindElem(_T("CircleMaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdCircleCaliperMaskData.resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCircleCaliperMaskData.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}

			tmpTrainGuiParam.m_vdCircleCaliperMaskData.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCircleCaliperMask.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCircleCaliperMask.size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCircleCaliperMask.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}

	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM;index++)
	{
		CString strName;
		strName = "CaliperNum";
		strName.AppendFormat("%d",index+1);
		// CaliperNum1
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_nCaliperNum[index] = _ttoi(cXMLConfigurator.GetElemData());
		// startX
		strName = "StartX";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dStartX[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// startY
		strName = "StartY";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dStartY[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// EndX
		strName = "EndX";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dEndX[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// EndY
		strName = "EndY";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dEndY[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Project Len
		strName = "ProjectLen";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCaliperProjectLen[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Len
		strName = "SearchLen";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCaliperSearchLen[index] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Rotation
		strName = "SearchRotation";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CaliperSearchRotation[index] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		// LineCaliper Visible
		strName="LineCaliperVisible";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			tmpTrainGuiParam.m_bLineCaliperVisible[index]=true;
		}
		else
		{
			tmpTrainGuiParam.m_bLineCaliperVisible[index] = _ttoi(cXMLConfigurator.GetElemData());
		}

		// LineMaskParam
		strName=_T("CaliperMaskEnable");
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (bRet)
		{
			tmpTrainGuiParam.m_bEnableLineMask[index] = _ttol(cXMLConfigurator.GetElemData());
		}
		else
		{
			tmpTrainGuiParam.m_bEnableLineMask[index] = false;
		}
		strName=_T("MaskParam");
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (bRet)
		{
			CString strTmp;
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
			if (!bRet) return false;
			tmpTrainGuiParam.m_vdLineCaliperMaskData[index].resize(_ttoi(cXMLConfigurator.GetElemData()));

			for (int j=0; j<tmpTrainGuiParam.m_vdLineCaliperMaskData[index].size(); j++)
			{
				strTmp.Format(_T("MaskData%d"), j);
				bRet = cXMLConfigurator.FindElem(strTmp);
				if (!bRet)
				{
					return false;
				}
				tmpTrainGuiParam.m_vdLineCaliperMaskData[index].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
			}
			bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
			if (!bRet) return false;
			tmpTrainGuiParam.m_vbLineCaliperMask[index].resize(_ttoi(cXMLConfigurator.GetElemData()));
			for (int j=0; j<tmpTrainGuiParam.m_vbLineCaliperMask[index].size(); j++)
			{
				strTmp.Format(_T("EnableMaskData%d"), j);
				bRet = cXMLConfigurator.FindElem(strTmp);
				if (!bRet)
				{
					return false;
				}			
				tmpTrainGuiParam.m_vbLineCaliperMask[index].at(j) = _ttoi(cXMLConfigurator.GetElemData());
			}
			cXMLConfigurator.LeaveElem();
		}
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

	bRet = cXMLConfigurator.FindElem(_T("EnableSharpScore"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bEnableSharpScore =  false;
	}
	tmpSearchDataParam.m_bEnableSharpScore = _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("SharpThre"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dSharpThre = 0.5;
	}
	else
	{
		tmpSearchDataParam.m_dSharpThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("SearchOutLineAngleType"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nSearchOutLineAngleType = _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("SearchOutPointType"));
	if (bRet)
	{
		tmpSearchDataParam.m_nSearchOutPointType = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableLineAngleJudge"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableLineAngleJudge = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("JudgeLineAngle"));
	if (bRet)
	{
		tmpSearchDataParam.m_dJudgeLineAngle = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	bRet = cXMLConfigurator.FindElem(_T("Enable2LineAngleJudge"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnable2LineAngleJudge = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleRange"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAngleRange = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


	cXMLConfigurator.LeaveElem();

	/////////////////////////////SearchGuiParam///////////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchGuiParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("ShowFitCirclePoint"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchGuiParam.m_bEnableShowFitCirclePoint = _ttoi(cXMLConfigurator.GetElemData());

	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM;index++)
	{
		CString strName;
		strName = "ShowFitLinePoint";
		strName.AppendFormat("%d",index+1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			tmpSearchGuiParam.m_bEnableShowFitLinePoint[index] = false;
		}
		tmpSearchGuiParam.m_bEnableShowFitLinePoint[index] = _ttoi(cXMLConfigurator.GetElemData());
	}


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

	bRet = cXMLConfigurator.FindElem(_T("Sharpness"));
	if (!bRet)
	{
		tmpTrainResult.m_dSharpness = 1.0;
	}
	tmpTrainResult.m_dSharpness = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	//圆的参数
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

	//线的参数
	for(int nIndex = 0;nIndex < QUICK_CIRCLE_4LINE_NUM;nIndex ++)
	{
		CString strName;
		strName = "StartX";
		strName.AppendFormat("%d",nIndex + 1);
		// line 0
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		strName = "StartY";
		strName.AppendFormat("%d",nIndex + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainResult.m_dStartX[nIndex] = vPos.GetX();
		tmpTrainResult.m_dStartY[nIndex] = vPos.GetY();

		strName = "EndX";
		strName.AppendFormat("%d",nIndex + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		strName = "EndY";
		strName.AppendFormat("%d",nIndex + 1);
		bRet = cXMLConfigurator.FindElem(strName);
		if (!bRet)
		{
			return false;
		}

		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainResult.m_dEndX[nIndex] = vPos.GetX();
		tmpTrainResult.m_dEndY[nIndex] = vPos.GetY();
	}


	cXMLConfigurator.LeaveElem();
	

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

	////////////////////////////读取ptq文件//////////////////////////
	//scPatternSearchRot tmpPatternSearchTool;
	//bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	//if (!bRet)
	//{
	//	return false;
	//}

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
bool CQuickCircle4LineSearchTool::SaveModelToFile(CString strDir)
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
		cXMLConfigurator.AddElem(_T("CamWidth"),  m_CamWidth);
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


	// 圆的参数
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
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CircleedgeProperty);
	cXMLConfigurator.AddElem(_T("CircleEdgeProperty"), strInfo);
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bCircleSortByScore);
	cXMLConfigurator.AddElem(_T("CircleSortByScore"), strInfo);
	// DualEdge
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCircleDualEdge);
	cXMLConfigurator.AddElem(_T("CircleDualEdge"), strInfo);

	// 直线的参数
	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM; index ++)
	{
		CString strName;
		// contrastthre1
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[index]);
		strName = _T("ContrastThre");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
		// fileterhalfwidth1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[index]);
		strName = _T("FilterHalfWidth");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
		// outlinenum1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[index]);
		strName = _T("OutLineNum");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
		// edgeproperty1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[index]);
		strName = _T("EdgeProperty");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
		// SortByScore1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[index]);
		strName = _T("SortByScore");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
		// LineAngleType1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType[index]);
		strName = _T("LineAngleType");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);

		// DualEdge1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableLineDualEdge[index]);
		strName = _T("LineDualEdge");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);

		strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableLine[index]);
		strName = _T("EnableLine");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.AddElem(strName, strInfo);
	}
	

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

	// 圆的gui参数
	// circle center
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

	//CircleCaliper Visible
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleCaliperVisible);
	cXMLConfigurator.AddElem(_T("CircleCaliperVisible"), strInfo);

	// CircleCaliperMaskEnable
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableCircleMask);
	cXMLConfigurator.AddElem(_T("CircleCaliperMaskEnable"), strInfo);
	cXMLConfigurator.DeleteElem(_T("CircleMaskParam"));
	cXMLConfigurator.AddElem(_T("CircleMaskParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCircleCaliperMaskData.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
	CString strTmp;
	for (int j=0; j<m_TrainGuiParam.m_vdCircleCaliperMaskData.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCircleCaliperMaskData.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCircleCaliperMask.size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCircleCaliperMask.size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCircleCaliperMask.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	//直线的gui参数
	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM; index ++)
	{
		// Caliper Num1
		CString strName;
		strName = _T("CaliperNum");
		strName.AppendFormat("%d",index+1);
		
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// startX1
		strName = _T("StartX");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// startY1
		strName = _T("StartY");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// EndX1
		strName = _T("EndX");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// EndY1
		strName = _T("EndY");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// Project Len1
		strName = _T("ProjectLen");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// Search Len1
		strName = _T("SearchLen");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
		// Search Rotation1
		strName = _T("SearchRotation");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation[index].ToDouble());
		cXMLConfigurator.AddElem(strName, strInfo);
		//LineCaliper Visible1
		strName = _T("LineCaliperVisible");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible[index]);
		cXMLConfigurator.AddElem(strName, strInfo);

		// LineMask
		strName = _T("CaliperMaskEnable");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[index]);
		cXMLConfigurator.AddElem(strName, strInfo);

		strName = _T("MaskParam");
		strName.AppendFormat("%d",index+1);
		cXMLConfigurator.DeleteElem(strName);
		cXMLConfigurator.AddElem(strName);
		cXMLConfigurator.EnterElem();

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdLineCaliperMaskData[index].size());
		cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vdLineCaliperMaskData[index].size(); j++)
		{
			strName = _T("MaskData");
			strName.AppendFormat("%d",j);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdLineCaliperMaskData[index].at(j));
			cXMLConfigurator.AddElem(strName, strInfo);
		}

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbLineCaliperMask[index].size());
		cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vbLineCaliperMask[index].size(); j++)
		{
			strName = _T("EnableMaskData");
			strName.AppendFormat("%d",j);
			strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbLineCaliperMask[index].at(j));
			cXMLConfigurator.AddElem(strName, strInfo);
		}
		cXMLConfigurator.LeaveElem();
	}
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

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeLow);
	cXMLConfigurator.AddElem(_T("SizeLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeHigh);
	cXMLConfigurator.AddElem(_T("SizeHigh"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableSharpScore);
	cXMLConfigurator.AddElem(_T("EnableSharpScore"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSharpThre);
	cXMLConfigurator.AddElem(_T("SharpThre"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchOutLineAngleType);
	cXMLConfigurator.AddElem(_T("SearchOutLineAngleType"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchOutPointType);
	cXMLConfigurator.AddElem(_T("SearchOutPointType"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableLineAngleJudge);
	cXMLConfigurator.AddElem(_T("EnableLineAngleJudge"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dJudgeLineAngle);
	cXMLConfigurator.AddElem(_T("JudgeLineAngle"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnable2LineAngleJudge);
	cXMLConfigurator.AddElem(_T("Enable2LineAngleJudge"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleRange);
	cXMLConfigurator.AddElem(_T("AngleRange"), strInfo);

	cXMLConfigurator.LeaveElem();

	///////////////SearchGuiParam/////////////////////////
	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowFitCirclePoint);
	cXMLConfigurator.AddElem(_T("ShowFitCirclePoint"), strInfo);


	for(int index = 0;index < QUICK_CIRCLE_4LINE_NUM; index ++)
	{
		CString strName;
		strName = _T("ShowFitLinePoint");
		strName.AppendFormat("%d",index+1);
		strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowFitLinePoint[index]);
		cXMLConfigurator.AddElem(strName, strInfo);
	}
	cXMLConfigurator.LeaveElem();


	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dSharpness);
	cXMLConfigurator.AddElem(_T("Sharpness"), strInfo);

	//圆的训练结果参数
	strInfo.Format(_T("%f"), m_TrainResult.m_dCenterX);
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dCenterY);
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dRadius);
	cXMLConfigurator.AddElem(_T("Radius"), strInfo);	

	//直线的训练结果参数
	for(int nIndex = 0;nIndex < QUICK_CIRCLE_4LINE_NUM; nIndex++)
	{	
		CString strName;
		strName = _T("StartX");
		strName.AppendFormat("%d",nIndex+1);

		strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[nIndex]);
		cXMLConfigurator.AddElem(strName, strInfo);

		strName = _T("StartY");
		strName.AppendFormat("%d",nIndex+1);
		strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[nIndex]);
		cXMLConfigurator.AddElem(strName, strInfo);

		strName = _T("EndX");
		strName.AppendFormat("%d",nIndex+1);
		strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[nIndex]);
		cXMLConfigurator.AddElem(strName, strInfo);

		strName = _T("EndY");
		strName.AppendFormat("%d",nIndex+1);
		strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[nIndex]);
		cXMLConfigurator.AddElem(strName, strInfo);
	}

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

	//////////////////////////存储模板pts文件//////////////////////////
	bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
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
bool CQuickCircle4LineSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_tmpTrainGuiParam.m_bLineCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();
			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[i]);
		}

		m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius,
			m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
		m_tmpTrainGuiParam.m_dCircleCaliperProjectLen = m_GuiFindCircle.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCircleCaliperSearchLen	= m_GuiFindCircle.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bCircleCaliperVisible=m_GuiFindCircle.IsCaliperVisible();
		m_GuiFindCircle.GetMaskData(m_tmpTrainGuiParam.m_vdCircleCaliperMaskData);
		m_GuiFindCircle.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);

	}
	else // 将数据传递到Gui控件上
	{	
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation[i]);
			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData[i]);
		}

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
		m_GuiFindCircle.SetMaskData(m_tmpTrainGuiParam.m_vdCircleCaliperMaskData);
	}

	return true;
}

bool CQuickCircle4LineSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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


		for (j = 0; j < m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			m_pGuiSearchResultFitPoint1Array[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchResultFitPoint1Array[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
	}

	return true;
}

// 清除控件及其显示					
bool CQuickCircle4LineSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CQuickCircle4LineSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CQuickCircle4LineSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	return true;
}

bool CQuickCircle4LineSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindCircle"));



		for (j=0; j<QUICK_CIRCLE_4LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));
	}
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;
}

bool CQuickCircle4LineSearchTool::ClearResultGui()
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
	}

	return true;
}

// 获取训练模板原始图像
bool CQuickCircle4LineSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CQuickCircle4LineSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CQuickCircle4LineSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CQuickCircle4LineSearchTool::SetLanguage(int nLanguage)
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

	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
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

// 根据相机宽高设置GUI大小
bool CQuickCircle4LineSearchTool::SetGuiByCam(double nWidth,double nHeight) 
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
	//m_GuiModelPoint.SetLabel(_T("参考点"));

	//卡尺
	double sx,sy,ex,ey;
	double plength,slength;
	for (int i=0; i<QUICK_CIRCLE_4LINE_NUM; i++)
	{
		m_GuiFindLine[i].GetStartXYEndXY(sx,sy,ex,ey);
		m_GuiFindLine[i].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
		plength = m_GuiFindLine[i].GetCaliperProjectionLength();
		slength = m_GuiFindLine[i].GetCaliperSearchLength();
		m_GuiFindLine[i].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
		m_GuiFindLine[i].SetCaliperSearchLength(slength/m_CamWidth*nWidth);
	}


	//找圆
	double rad,asta,aspan;
	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
}
CString CQuickCircle4LineSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:
				strErrInfo += _T(" Quick search failed");
				break;
			case -2:
				strErrInfo += _T(" Find line1 failed");
				break;
			case -3:
				strErrInfo += _T(" Find line2 failed");
				break;
			case -4:
				strErrInfo += _T(" Find line3 failed");
				break;
			case -5:
				strErrInfo += _T(" Find line4 failed");
				break;
			case -6:
				strErrInfo += _T("  Find circle failed");
				break;
			case -7:
				strErrInfo += _T("  Find line1234 failed");
				break;
			case -8:
				strErrInfo += _T("  Line angle NG");
				break;
			case -9:
				strErrInfo += _T(" Cross angle NG");
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
				strErrInfo += _T(" 粗定位失败");
				break;
			case -2:
				strErrInfo += _T(" 找线1失败");
				break;
			case -3:
				strErrInfo += _T(" 找线2失败");
				break;
			case -4:
				strErrInfo += _T(" 找线3失败");
				break;
			case -5:
				strErrInfo += _T(" 找线4失败");
				break;
			case -6:
				strErrInfo += _T("  找圆失败");
				break;
			case -7:
				strErrInfo += _T("  找线1234失败");
				break;
			case -8:
				strErrInfo += _T("  直线角度判定超限");
				break;
			case -9:
				strErrInfo += _T(" 夹角角度判定超限");
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
				strErrInfo += _T(" 几何定位失败");
				break;
			case -2:
				strErrInfo += _T(" 找线1失败");
				break;
			case -3:
				strErrInfo += _T(" 找线2失败");
				break;
			case -4:
				strErrInfo += _T(" 找线3失败");
				break;
			case -5:
				strErrInfo += _T(" 找线4失败");
				break;
			case -6:
				strErrInfo += _T("  找圆失败");
				break;
			case -7:
				strErrInfo += _T("  找线1234失败");
				break;
			case -8:
				strErrInfo += _T("  直线角度判定超限");
				break;
			case -9:
				strErrInfo += _T(" 夹角角度判定超限");
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