// CornerSearchTool.cpp: implementation of the CQuickCornerCircleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuickCornerCircleSearchTool.h"
#include "svIntersect.h"
#include "svGuiLine.h"
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

CQuickCornerCircleSearchTool::CQuickCornerCircleSearchTool()
{
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100,0,0);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
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

	m_GuiFindLine[1].SetStartXYEndXY(130, 300, 130, 200);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(120, 150, 270, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	

	int i=0;
	for (i=0; i<LINE_NUM; i++)
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

	// 找圆工具
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		break;
	case 1:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	default:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	}

	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_dGrau							= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre					= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag						= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable					= false;
	m_tmpTrainGuiParam.m_TrainAffineRect				= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint						= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect						= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth						= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor						= m_GuiTrainRect.GetLineColor();

	for (i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
		m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 0;
		m_tmpTrainGuiParam.m_nCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
			m_tmpTrainGuiParam.m_dEndY[i]);
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bEnableLineMask[i]			= false;
		m_tmpTrainGuiParam.m_bLineCaliperVisible[i]		= m_GuiFindLine[i].IsCaliperVisible();
		m_tmpTrainGuiParam.m_bEnableLineMask[i]			= false;
	}

	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dCircleRadius,
		m_tmpTrainGuiParam.m_CircleArcStart, m_tmpTrainGuiParam.m_CircleArcSpan);
	m_FindCircleTool.GetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre,m_tmpTrainDataParam.m_CircleedgeProperty,
		m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
	m_tmpTrainGuiParam.m_dCircleCaliperProjectLen		= m_GuiFindCircle.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCircleCaliperSearchLen		= m_GuiFindCircle.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CircleCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bCircleCaliperVisible			= m_GuiFindCircle.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bEnableCircleMask				= false;

	m_tmpSearchDataParam.m_dAcceptThre					= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow					= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh					= 5.0;
	m_tmpSearchDataParam.m_nSearchNum					= 1;
	m_tmpSearchGuiParam.m_nLineWidth					= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor						= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam									= m_tmpTrainDataParam;
	m_TrainGuiParam										= m_tmpTrainGuiParam;
	m_SearchDataParam									= m_tmpSearchDataParam;
	m_SearchGuiParam									= m_tmpSearchGuiParam;
}

CQuickCornerCircleSearchTool::CQuickCornerCircleSearchTool(CQuickCornerCircleSearchTool &CQuickCornerCircleSearchTool)
{
	if (CQuickCornerCircleSearchTool.GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return;
	}

	// 清空
	// 	ClearAllGui();
	// 	ClearResult();

	// 初始化
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(300, 240, 50, 50,0,0);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(200, 240, 40, 40);
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

	m_GuiFindLine[1].SetStartXYEndXY(100, 300, 100, 200);
	m_GuiFindLine[0].SetStartXYEndXY(150, 150, 300, 150);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	
	for (int i=0; i<LINE_NUM; i++)
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

	// 找圆工具
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(480, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);
	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		break;
	case 1:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	default:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	}

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= CQuickCornerCircleSearchTool.m_CamHeight;
	m_CamWidth								= CQuickCornerCircleSearchTool.m_CamWidth;

	m_TrainDataParam						= CQuickCornerCircleSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CQuickCornerCircleSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CQuickCornerCircleSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CQuickCornerCircleSearchTool.m_SearchGuiParam;
	m_TrainResult							= CQuickCornerCircleSearchTool.m_TrainResult;
	m_ImageTrain							= CQuickCornerCircleSearchTool.m_ImageTrain;
	m_ImageMask								= CQuickCornerCircleSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CQuickCornerCircleSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CQuickCornerCircleSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CQuickCornerCircleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CQuickCornerCircleSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CQuickCornerCircleSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CQuickCornerCircleSearchTool.UpdateSearchGuiData(true);
	CQuickCornerCircleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CQuickCornerCircleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CQuickCornerCircleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CQuickCornerCircleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CQuickCornerCircleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CQuickCornerCircleSearchTool.m_tmpImageTrain);
	SetMaskImage(CQuickCornerCircleSearchTool.m_tmpImageMask);

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

CQuickCornerCircleSearchTool& CQuickCornerCircleSearchTool::operator=(CQuickCornerCircleSearchTool &CQuickCornerCircleSearchTool)
{
	if (this == &CQuickCornerCircleSearchTool)
	{
		return *this;
	}

	if (CQuickCornerCircleSearchTool.GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	// 初始化
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(300, 240, 50, 50,0,0);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(200, 240, 40, 40);
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

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(100, 300, 100, 200);
	m_GuiFindLine[0].SetStartXYEndXY(150, 150, 300, 150);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	
	for (int i=0; i<LINE_NUM; i++)
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
	// 找圆工具
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(480, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle.SetLabelVisible(TRUE);
	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		break;
	case 1:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	default:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	}

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= CQuickCornerCircleSearchTool.m_CamHeight;
	m_CamWidth								= CQuickCornerCircleSearchTool.m_CamWidth;

	m_TrainDataParam						= CQuickCornerCircleSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CQuickCornerCircleSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CQuickCornerCircleSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CQuickCornerCircleSearchTool.m_SearchGuiParam;
	m_TrainResult							= CQuickCornerCircleSearchTool.m_TrainResult;
	m_ImageTrain							= CQuickCornerCircleSearchTool.m_ImageTrain;
	m_ImageMask								= CQuickCornerCircleSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CQuickCornerCircleSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CQuickCornerCircleSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CQuickCornerCircleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CQuickCornerCircleSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CQuickCornerCircleSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CQuickCornerCircleSearchTool.UpdateSearchGuiData(true);
	CQuickCornerCircleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CQuickCornerCircleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CQuickCornerCircleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CQuickCornerCircleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CQuickCornerCircleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CQuickCornerCircleSearchTool.m_tmpImageTrain);
	SetMaskImage(CQuickCornerCircleSearchTool.m_tmpImageMask);
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

CQuickCornerCircleSearchTool::~CQuickCornerCircleSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CQuickCornerCircleSearchTool::GetSearchToolType()
{
	return eQuickCornerCircleSearchTool;
}										   

// 设置显示控件指针		
bool CQuickCornerCircleSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CQuickCornerCircleSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CQuickCornerCircleSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickCornerCircleSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CQuickCornerCircleSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CQuickCornerCircleSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CQuickCornerCircleSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CQuickCornerCircleSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}	

// 更新搜索界面显示									
bool CQuickCornerCircleSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CQuickCornerCircleSearchTool::UpdateTrainGuiDisplay()
{
	// 更新
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	for (int i=0; i<LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);

		if (m_tmpTrainGuiParam.m_bEnableLineMask[i])
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
	}

	{
		CString strKey;
		strKey.Format(_T("%s_FindCircleTool"),m_strKeyText);
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
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	
		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

		{
			CString strKey;
			strKey.Format(_T("%s_FindCircleTool"),m_strKeyText);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle,strKey);
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
bool CQuickCornerCircleSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultCircle.SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle,m_strKeyText+_T("_TrainResultCircle"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}				

// 更新搜索Gui控件显示														
bool CQuickCornerCircleSearchTool::UpdateSearchResultGuiDisplay()
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
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPointArray_1.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchResultFitPointArray_2.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint1);
	}

	for (i=0; i<m_pGuiSearchResultCircleFitPointArray.size(); i++)
	{
		m_pGuiSearchResultCircleFitPointArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint);
	}

	for(i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible);
	}

	for(i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		if (m_bSearchResultGuiShow) // 立即更新
		{
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

			for (i=0; i<m_pGuiSearchResultFitPointArray_1.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_1[i], m_pGuiSearchResultFitPointArray_1[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPointArray_2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_2[i], m_pGuiSearchResultFitPointArray_2[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultCircleFitPointArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultCircleFitPointArray[i], m_pGuiSearchResultCircleFitPointArray[i]->GetTipText());
			}

			for(i=0;i<m_pGuiSearchCircleArray.size();i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleArray[i],m_pGuiSearchCircleArray[i]->GetTipText());
			}

			for(i=0;i<m_pGuiSearchCircleCenterArray.size();i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterArray[i],m_pGuiSearchCircleCenterArray[i]->GetTipText());
			}
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CQuickCornerCircleSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	//	UpdateTrainResultGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CQuickCornerCircleSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CQuickCornerCircleSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CQuickCornerCircleSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	*((CQuickCornerCircleSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CQuickCornerCircleSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CQuickCornerCircleSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CQuickCornerCircleSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CQuickCornerCircleSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CQuickCornerCircleSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CQuickCornerCircleSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CQuickCornerCircleSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CQuickCornerCircleSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CQuickCornerCircleSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);
	m_nResultErrNumber=0;
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
			m_nResultErrNumber=-1;///几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		// 找线0
		m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
		m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
		m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
		//掩膜与双极性
		m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[0]);
		if (m_tmpTrainDataParam.m_bEnableLineDualEdge[0])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], tmpEdgePolarity,m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		} 
		else
		{
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		}

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
			m_nResultErrNumber=-2;///找线工具1失败
			return false;
		}
		// 找线1
		m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
		m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
		m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
		//掩膜与双极性
		m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[1]);
		if (m_tmpTrainDataParam.m_bEnableLineDualEdge[1])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1], tmpEdgePolarity,m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		} 
		else
		{
			m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1],m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		}

		scLineSeg cLineSeg2;
		double dRMS2;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
		}
		else
		{
			bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
		}
		if (!bRet)
		{
			m_nResultErrNumber = -3;
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber = -4;
			return false;
		}

		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
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
			m_nResultErrNumber=-5;///找圆失败
			return false;
		}

		m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
		m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

		m_tmpTrainResult.m_dCircleCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCircleCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dCircleRadius  = cCircle.GetRadius();

		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

		return true;
	}
	else
	{
		bool bRet =false;
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
			m_nResultErrNumber=-1;///几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		// 找线0
		m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
		m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
		//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
		//掩膜与双极性
		m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[0]);
		if (m_tmpTrainDataParam.m_bEnableLineDualEdge[0])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[0] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[0] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], tmpEdgePolarity,m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		} 
		else
		{
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		}

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
			m_nResultErrNumber=-2;///找线工具1失败
			return false;
		}

		// 找线1
		m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
		m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
		m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
		//掩膜与双极性
		m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[1]);
		if (m_tmpTrainDataParam.m_bEnableLineDualEdge[1])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[1] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[1] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1], tmpEdgePolarity,m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		} 
		else
		{
			m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1],m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		}

		scLineSeg cLineSeg2;
		double dRMS2;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[1].Execute(tmpImage, cLineSeg2, dRMS2);
		}
		else
		{
			bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-3;///找线工具1失败
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-4;///求交点失败
			return false;
		}

		// 找圆
		m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum);
		m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum, m_tmpTrainGuiParam.m_CircleCaliperSearchDirection, m_tmpTrainGuiParam.m_dCircleCaliperSearchLen
			,m_tmpTrainGuiParam.m_dCircleCaliperProjectLen);
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre, m_tmpTrainDataParam.m_CircleedgeProperty, m_tmpTrainDataParam.m_lCircleFilterHalfWidth);
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
			m_nResultErrNumber=-5;///找圆工具失败
			return false;
		}

		m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
		m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();

		m_tmpTrainResult.m_dCircleCenterX = cCircle.GetCenter().GetX();
		m_tmpTrainResult.m_dCircleCenterY = cCircle.GetCenter().GetY();
		m_tmpTrainResult.m_dCircleRadius  = cCircle.GetRadius();

		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CQuickCornerCircleSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CQuickCornerCircleSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	*((CQuickCornerCircleSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CQuickCornerCircleSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CQuickCornerCircleSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CQuickCornerCircleSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CQuickCornerCircleSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	*((CQuickCornerCircleSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CQuickCornerCircleSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CQuickCornerCircleSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CQuickCornerCircleSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickCornerCircleSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CQuickCornerCircleSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CQuickCornerCircleSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;///训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));

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

	for (int i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
	}

	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCircleCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum, m_TrainGuiParam.m_CircleCaliperSearchDirection, m_TrainGuiParam.m_dCircleCaliperSearchLen
		,m_TrainGuiParam.m_dCircleCaliperProjectLen);
	m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre, m_TrainDataParam.m_CircleedgeProperty, m_TrainDataParam.m_lCircleFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nCircleOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bCircleSortByScore);
	

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

			scLineSeg ResLine[LINE_NUM];
			double dRMS[LINE_NUM];
			bool   bFindLine[LINE_NUM];
			for (int k=0; k<LINE_NUM; k++)
			{
				bFindLine[k] = 0;
			}

			for (int j=0; j<LINE_NUM; j++)
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
				//掩膜与双极性
				m_FindLineTool[j].SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[j]);
				if (m_tmpTrainDataParam.m_bEnableLineDualEdge[j])
				{
					EdgePolarity tmpEdgePolarity = eDontCare;
					if (m_tmpTrainDataParam.m_edgeProperty[j] == eDarkToLight)
					{
						tmpEdgePolarity = eLightToDark;
					} 
					else if (m_tmpTrainDataParam.m_edgeProperty[j] == eLightToDark)
					{
						tmpEdgePolarity = eDarkToLight;
					}
					m_FindLineTool[j].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[j], 
						m_tmpTrainDataParam.m_edgeProperty[j], tmpEdgePolarity,m_tmpTrainDataParam.m_lFilterHalfWidth[j]);
				} 
				else
				{
					m_FindLineTool[j].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[j], 
						m_tmpTrainDataParam.m_edgeProperty[j],m_tmpTrainDataParam.m_lFilterHalfWidth[j]);
				}
				if(bIsProcessImage)
				{
					// 执行找线
					bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); 
				}
				else
				{
					// 执行找线
					bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]);
				}

				
				if (!bFindLine[j])
				{
				 m_nResultErrNumber=-2-j;///找线工具失败
				 return false;
				}
			}

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
			//掩膜与双极性
			m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);
			if (m_tmpTrainDataParam.m_bEnableCircleDualEdge)
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_dCircleContrastThre == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_dCircleContrastThre == eLightToDark)
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
			// 执行找圆
			scCircle ResCircle;
			double dCircleRMS;
			bool   bFindCircle = false;
			if(bIsProcessImage)
			{
				bFindCircle = m_FindCircleTool.Execute(tmpImage, ResCircle, dCircleRMS);
			}
			else
			{
				bFindCircle = m_FindCircleTool.Execute(ImageSearch, ResCircle, dCircleRMS);
			}
			
			if (!bFindCircle)
			{
				m_nResultErrNumber=-5;/////找圆工具失败
				return false;
			}

			// 求交点和圆心
			if (bFindLine[0] && bFindLine[1] && bFindCircle)
			{
				sc2Vector vLineCrossPoint;
				bool bIntersect;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
					&& crossPointArray.size() > 0)
				{

					sc2Vector pointCenter1;
					sc2Vector pointCenter2;

					//计算找线1的中点
					{
						sc2Vector pointS;
						sc2Vector pointE;

						svStd vector<sc2Vector> pointResult;	// 探测点
						svStd vector<bool> used;				// 参与拟合标志
						for (int j=0; j<1; j++)
						{
							m_FindLineTool[j].GetFitData(pointResult);
							m_FindLineTool[j].GetUsed(used);

							int nNum = pointResult.size();
							if (nNum > 0)
							{

								int i = 0;
								svStd vector<sc2Vector>::iterator iter;
								for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
								{
									if (used.at(i) == true)
									{
										pointS = pointResult.at(i);
										break;
									}

								}
								for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
								{
									if (used.at(i) == true)
									{
										pointE = pointResult.at(i);
									}

								}
								pointCenter1 = (pointS+pointE)/2;


							}
							else
							{
								return false;
							}
						}
					}


					//计算找线2的中点
					{
						sc2Vector pointS;
						sc2Vector pointE;
						sc2Vector pointCenter;
						svStd vector<sc2Vector> pointResult;	// 探测点
						svStd vector<bool> used;				// 参与拟合标志
						for (int j=1; j<LINE_NUM; j++)
						{
							m_FindLineTool[j].GetFitData(pointResult);
							m_FindLineTool[j].GetUsed(used);

							int nNum = pointResult.size();
							if (nNum > 0)
							{

								int i = 0;
								svStd vector<sc2Vector>::iterator iter;
								for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
								{
									if (used.at(i) == true)
									{
										pointS = pointResult.at(i);
										break;
									}

								}
								for( iter = pointResult.begin(), i = 0; iter != pointResult.end(); iter++, i++)
								{
									if (used.at(i) == true)
									{
										pointE = pointResult.at(i);
									}

								}
								pointCenter2 = (pointS+pointE)/2;
							}
							else
							{
								return false;
							}
						}
					}

					CSearchResult searchResult;
					searchResult.m_dPosX = crossPointArray[0].GetX();
					searchResult.m_dPosY = crossPointArray[0].GetY();
					// 角度
					//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
					double dRotAngle = scDegree((ResLine[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180()
						+ ResLine[1].GetRotation().SignedNormMod180()-modelLineSeg2.GetRotation().SignedNormMod180())/2.0).ToDouble(); 
					searchResult.m_dAngle = dRotAngle;
					searchResult.m_dScore = tmpQuickResult.GetScore();

					// 辅助点1
					double dAngleLine0 = scDegree(ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()).ToDouble();
					searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
					searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
					searchResult.m_vdAuxiliaryAngle[0] = dAngleLine0;

					// 辅助点2
					double dAngleLine1 = scDegree(ResLine[1].GetRotation().NormMod180()-modelLineSeg2.GetRotation().NormMod180()).ToDouble();
					searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
					searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
					searchResult.m_vdAuxiliaryAngle[1] = dAngleLine1;
					
					//辅助点3
					searchResult.m_vdAuxiliaryPosX.at(2) = ResCircle.GetCenter().GetX();
					searchResult.m_vdAuxiliaryPosY.at(2) = ResCircle.GetCenter().GetY();
					searchResult.m_vdAuxiliaryAngle.at(2) = dRotAngle;

					searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
					searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

					// searchrect
					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					// line
					for (int nLine = 0; nLine < LINE_NUM; nLine++)
					{
						CResultLine line;
						line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
						line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
						line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
						line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
						searchResult.m_vLineSearchResult.push_back(line);
					}

					// circle

					CResultCircleSearchTool circle;
					circle.m_scCircleCircle = ResCircle.GetCenter();
					circle.m_dRadius = ResCircle.GetRadius();
					searchResult.m_vCircleSearchResult.push_back(circle);



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

					strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
					pTempCross = m_GuiSearchCross2+i*2+1;
					pTempCross->SetCenterExtent(pointCenter1, sc2Vector(40,40));
					pTempCross->SetLineColor(RGB(255,0,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);			
					m_pGuiSearchCrossArray.push_back(pTempCross);

					strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
					pTempCross = m_GuiSearchCross2+i*2+2;
					pTempCross->SetCenterExtent(pointCenter2, sc2Vector(40,40));
					pTempCross->SetLineColor(RGB(255,0,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);			
					m_pGuiSearchCrossArray.push_back(pTempCross);

					for (int l=0; l< LINE_NUM; l++)
					{
						strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
						//scGuiLine *pTemLine = new scGuiLine;
						scGuiLine *pTemLine = m_GuiSearchLine+i*2+l;
						pTemLine->SetLine(ResLine[l].GetLine());
						pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTemLine->SetTipTextEnabled(true);
						pTemLine->SetTipText(strTmpKey);
						m_pGuiSearchLineArray.push_back(pTemLine);
					}

					strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
					//scGuiCross *pTempLineCross = new scGuiCross;
					scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
					pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
					//					pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempLineCross->SetLineColor(RGB(255, 0, 0));
					pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempLineCross->SetTipTextEnabled(true);
					pTempLineCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
					pTempLineCross->SetLabel(st);
					pTempLineCross->SetLabelVisible(TRUE);
					m_pGuiSearchLineCrossArray.push_back(pTempLineCross);

					// 找线0拟合点
					{
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindLineTool[0].GetUsed(bUsed);
						m_FindLineTool[0].GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							int nIndex = m_pGuiSearchResultFitPointArray_1.size();
							strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
							scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_1 + nIndex;
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
							m_pGuiSearchResultFitPointArray_1.push_back(pTempFitPoint1);	
						}
					}

					// 找线1拟合点
					{
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindLineTool[1].GetUsed(bUsed);
						m_FindLineTool[1].GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							int nIndex = m_pGuiSearchResultFitPointArray_2.size();
							strTmpKey.Format(_T("%s_FitPoint1_1_%d"), m_strKeyText, nIndex);
							scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_2 + nIndex;
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
							m_pGuiSearchResultFitPointArray_2.push_back(pTempFitPoint1);	
						}
					}

					strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
					scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
					pTemCircle->SetCircle(ResCircle);
					pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemCircle->SetTipTextEnabled(true);
					pTemCircle->SetTipText(strTmpKey);
					m_pGuiSearchCircleArray.push_back(pTemCircle);

					strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, i);
					scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+i;
					pTempCircleCenter->SetCenterExtent(ResCircle.GetCenter(), sc2Vector(40,40));
					pTempCircleCenter->SetLineColor(RGB(255,0,0));
					pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCircleCenter->SetTipTextEnabled(true);
					pTempCircleCenter->SetTipText(strTmpKey);

					st.Format(_T("%d X:%.1f;Y:%.1f"), i, ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY());
					pTempCircleCenter->SetLabel(st);
					pTempCircleCenter->SetLabelVisible(TRUE);

					m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);	

					{
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindCircleTool.GetUsed(bUsed);
						m_FindCircleTool.GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							int nIndex = m_pGuiSearchResultCircleFitPointArray.size();
							strTmpKey.Format(_T("%s_CircleFitPoint0_%d"), m_strKeyText, nIndex);
							scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultCircleFitPoint + nIndex;
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
							m_pGuiSearchResultCircleFitPointArray.push_back(pTempFitPoint1);	
						}
					}

				}
				else 
				{
					m_nResultErrNumber=-4;///求交点失败
					return false;
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
		m_nResultErrNumber=-1;///几何定位失败
		return false;
	}

	return true;
}

// 获取搜索结果个数										
int CQuickCornerCircleSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CQuickCornerCircleSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CQuickCornerCircleSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eQuickCornerCircleSearchTool)
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
bool CQuickCornerCircleSearchTool::LoadModelFromFile(CString strDir)
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
	CQuickCornerCircleSearchTrainDataParam tmpTrainDataParam;
	CQuickCornerCircleSearchTrainGuiParam tmpTrainGuiParam;
	CQuickCornerCircleSearchSearchDataParam tmpSearchDataParam;
	CQuickCornerCircleSearchSearchGuiParam tmpSearchGuiParam;
	CQuickCornerCircleSearchTrainResult    tmpTrainResult;
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
	if (cSearchToolType != eQuickCornerCircleSearchTool)
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



	// contrastthre1
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth1
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth[0] = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	// outlinenum1
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum[0] = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty1
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty[0] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// SortByScore1
	bRet = cXMLConfigurator.FindElem(_T("SortByScore1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[0] = true;
	}
	tmpTrainDataParam.m_bSortByScore[0] = _ttoi(cXMLConfigurator.GetElemData());
	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("LineDualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableLineDualEdge[0] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableLineDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// contrastthre2
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth2
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth[1] = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth[1] = _ttol(cXMLConfigurator.GetElemData());
	}
	// outlinenum2
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum[1] = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty2
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty[1] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// SortByScore2
	bRet = cXMLConfigurator.FindElem(_T("SortByScore2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[1] = true;
	}
	tmpTrainDataParam.m_bSortByScore[1] = _ttoi(cXMLConfigurator.GetElemData());
	// DualEdge2
	bRet = cXMLConfigurator.FindElem(_T("LineDualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableLineDualEdge[1] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableLineDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	//Circle
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
		return false;
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


	// CaliperNum1
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum[0] = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndX[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndY[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchRotation[0] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	// LineCaliper Visible1
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[0]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
	}
	// LineMaskParam
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask[0] = false;
	}
	bRet = cXMLConfigurator.FindElem(_T("MaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdLineCaliperMaskData[0].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdLineCaliperMaskData[0].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}

			tmpTrainGuiParam.m_vdLineCaliperMaskData[0].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbLineCaliperMask[0].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbLineCaliperMask[0].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbLineCaliperMask[0].at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}


	// CaliperNum2
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum[1] = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartX[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartY[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndX[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndY[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen[1] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchRotation[1] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	// LineCaliper Visible2
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperVisible2"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[1]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bLineCaliperVisible[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable1"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask[1] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask[1] = false;
	}


	bRet = cXMLConfigurator.FindElem(_T("MaskParam1"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum1"));
		if (!bRet) return false;		
		tmpTrainGuiParam.m_vdLineCaliperMaskData[1].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdLineCaliperMaskData[1].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vdLineCaliperMaskData[1].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum1"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbLineCaliperMask[1].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbLineCaliperMask[1].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbLineCaliperMask[1].at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}
	// Circle

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
	if (bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = false;
	}



	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible1"));
	if (bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint1 = false;
	}

	// Circle
	bRet = cXMLConfigurator.FindElem(_T("CircleFitPointVisible"));
	if (!bRet)
	{
		return false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowCirclePoint = _ttoi(cXMLConfigurator.GetElemData());
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


	// line 0
	bRet = cXMLConfigurator.FindElem(_T("StartX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("StartY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dStartX[0] = vPos.GetX();
	tmpTrainResult.m_dStartY[0] = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("EndY1"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dEndX[0] = vPos.GetX();
	tmpTrainResult.m_dEndY[0] = vPos.GetY();


	// line1
	bRet = cXMLConfigurator.FindElem(_T("StartX2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("StartY2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dStartX[1] = vPos.GetX();
	tmpTrainResult.m_dStartY[1] = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("EndY2"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dEndX[1] = vPos.GetX();
	tmpTrainResult.m_dEndY[1] = vPos.GetY();


	// Circle
	bRet = cXMLConfigurator.FindElem(_T("CircleCenterX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("CircleCenterY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dCircleCenterX = vPos.GetX();
	tmpTrainResult.m_dCircleCenterY = vPos.GetY();

	bRet = cXMLConfigurator.FindElem(_T("CircleRadius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dCircleRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	cXMLConfigurator.LeaveElem();

	//cXMLConfigurator.LeaveElem();

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
bool CQuickCornerCircleSearchTool::SaveModelToFile(CString strDir)
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
	int i=0;
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
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);


	// contrastthre1
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[0]);
	cXMLConfigurator.AddElem(_T("ContrastThre1"), strInfo);
	// fileterhalfwidth1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[0]);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth1"), strInfo);
	// outlinenum1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[0]);
	cXMLConfigurator.AddElem(_T("OutLineNum1"), strInfo);
	// edgeproperty1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[0]);
	cXMLConfigurator.AddElem(_T("EdgeProperty1"), strInfo);
	// SortByScore1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[0]);
	cXMLConfigurator.AddElem(_T("SortByScore1"), strInfo);
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableLineDualEdge[0]);
	cXMLConfigurator.AddElem(_T("LineDualEdge1"), strInfo);

	// contrastthre2
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[1]);
	cXMLConfigurator.AddElem(_T("ContrastThre2"), strInfo);
	// fileterhalfwidth2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[1]);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth2"), strInfo);
	// outlinenum2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[1]);
	cXMLConfigurator.AddElem(_T("OutLineNum2"), strInfo);
	// edgeproperty2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[1]);
	cXMLConfigurator.AddElem(_T("EdgeProperty2"), strInfo);
	// SortByScore2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[1]);
	cXMLConfigurator.AddElem(_T("SortByScore2"), strInfo);
	// DualEdge2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableLineDualEdge[1]);
	cXMLConfigurator.AddElem(_T("LineDualEdge2"), strInfo);
	// Circle
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

	// Caliper Num1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[0]);
	cXMLConfigurator.AddElem(_T("CaliperNum1"), strInfo);
	// startX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX[0]);
	cXMLConfigurator.AddElem(_T("StartX1"), strInfo);
	// startY1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY[0]);
	cXMLConfigurator.AddElem(_T("StartY1"), strInfo);
	// EndX1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX[0]);
	cXMLConfigurator.AddElem(_T("EndX1"), strInfo);
	// EndY1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY[0]);
	cXMLConfigurator.AddElem(_T("EndY1"), strInfo);
	// Project Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[0]);
	cXMLConfigurator.AddElem(_T("ProjectLen1"), strInfo);
	// Search Len1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[0]);
	cXMLConfigurator.AddElem(_T("SearchLen1"), strInfo);
	// Search Rotation1
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation[0].ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation1"), strInfo);

	//LineCaliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible[0]);
	cXMLConfigurator.AddElem(_T("LineCaliperVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[0]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable"), strInfo);

	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdLineCaliperMaskData[0].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdLineCaliperMaskData[0].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdLineCaliperMaskData[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbLineCaliperMask[0].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbLineCaliperMask[0].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbLineCaliperMask[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();


	// Caliper Num2 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[1]);
	cXMLConfigurator.AddElem(_T("CaliperNum2"), strInfo);
	// startX2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX[1]);
	cXMLConfigurator.AddElem(_T("StartX2"), strInfo);
	// startY2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY[1]);
	cXMLConfigurator.AddElem(_T("StartY2"), strInfo);
	// EndX
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX[1]);
	cXMLConfigurator.AddElem(_T("EndX2"), strInfo);
	// EndY2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY[1]);
	cXMLConfigurator.AddElem(_T("EndY2"), strInfo);
	// Project Len2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[1]);
	cXMLConfigurator.AddElem(_T("ProjectLen2"), strInfo);
	// Search Len2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[1]);
	cXMLConfigurator.AddElem(_T("SearchLen2"), strInfo);
	// Search Rotation2
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation[1].ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation2"), strInfo);
	//LineCaliper Visible2
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible[1]);
	cXMLConfigurator.AddElem(_T("LineCaliperVisible2"), strInfo);

	// LineMask
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[1]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable1"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam1"));
	cXMLConfigurator.AddElem(_T("MaskParam1"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdLineCaliperMaskData[1].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum1"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdLineCaliperMaskData[1].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdLineCaliperMaskData[1].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbLineCaliperMask[1].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum1"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbLineCaliperMask[1].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbLineCaliperMask[1].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	// Circle
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

	// CircleCaliperMaskEnable
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableCircleMask);
	cXMLConfigurator.AddElem(_T("CircleCaliperMaskEnable"), strInfo);
	cXMLConfigurator.DeleteElem(_T("CircleMaskParam"));
	cXMLConfigurator.AddElem(_T("CircleMaskParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCircleCaliperMaskData.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
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

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint1);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowCirclePoint);
	cXMLConfigurator.AddElem(_T("CircleFitPointVisible"), strInfo);

	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);


	// line0
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[0]);
	cXMLConfigurator.AddElem(_T("StartX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[0]);
	cXMLConfigurator.AddElem(_T("StartY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[0]);
	cXMLConfigurator.AddElem(_T("EndX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[0]);
	cXMLConfigurator.AddElem(_T("EndY1"), strInfo);
	// line 1
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[1]);
	cXMLConfigurator.AddElem(_T("StartX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[1]);
	cXMLConfigurator.AddElem(_T("StartY2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[1]);
	cXMLConfigurator.AddElem(_T("EndX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[1]);
	cXMLConfigurator.AddElem(_T("EndY2"), strInfo);

	// Circle
	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterX);
	cXMLConfigurator.AddElem(_T("CircleCenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterY);
	cXMLConfigurator.AddElem(_T("CircleCenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dCircleRadius);
	cXMLConfigurator.AddElem(_T("CircleRadius"), strInfo);	
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
bool CQuickCornerCircleSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		for (int i=0; i<LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask[i]);
			m_tmpTrainGuiParam.m_bLineCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();

		}

		m_tmpTrainGuiParam.m_nCircleCaliperNum		= m_GuiFindCircle.GetCaliperNum();
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
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
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		for (int i=0; i<LINE_NUM; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation[i]);

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData[i]);

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
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

bool CQuickCornerCircleSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		// Circle
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

	}

	return true;
}

// 清除控件及其显示					
bool CQuickCornerCircleSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{	
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件				
bool CQuickCornerCircleSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CQuickCornerCircleSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_1);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_2);
	ClearGraphicsArray(m_pGuiSearchResultCircleFitPointArray);

	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	

	return true;
}

bool CQuickCornerCircleSearchTool::SetGuiByCam(double nWidth,double nHeight)
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
	m_GuiFindLine[1].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[1].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

	plength = m_GuiFindLine[0].GetCaliperProjectionLength();
	slength = m_GuiFindLine[0].GetCaliperSearchLength();
	m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindLine[1].GetCaliperProjectionLength();
	slength = m_GuiFindLine[1].GetCaliperSearchLength();
	m_GuiFindLine[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);


	//参考点位置
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;


	return true;
}

bool CQuickCornerCircleSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem( m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		for (j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindCircleTool"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));

	}

	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;
}

bool CQuickCornerCircleSearchTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchResultFitPointArray_1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPointArray_2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_2[j]->GetTipText());
		}

		// Circle
		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultCircleFitPointArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultCircleFitPointArray[j]->GetTipText());
		}

	}

	return true;	
}

// 获取训练模板原始图像
bool CQuickCornerCircleSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CQuickCornerCircleSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CQuickCornerCircleSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CQuickCornerCircleSearchTool::SetLanguage(int nLanguage)
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

	for (int i=0; i<LINE_NUM; i++)
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

	switch (m_iLanguage)
	{
	case 0:
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		break;
	case 1:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	default:
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		break;
	}
}
CString CQuickCornerCircleSearchTool::GetResultErrInfo()
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
			case -2:			//找线1失败				
				strErrInfo += _T("Find line1 failed");
				break;
			case -3:			//找线2失败				
				strErrInfo += _T("Find line2 failed");
				break;
			case -4:			//求交点失败
				strErrInfo +=_T("Find the intersection of line1 and line2 failed");
				break;
			case -5:			//找圆失败				
				strErrInfo += _T("Find circle failed");
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
				strErrInfo += _T("找线1失败");
				break;
			case -3:							
				strErrInfo += _T("找线2失败");
				break;
			case -4:
				strErrInfo +=_T("求交点失败");
				break;
			case -5:							
				strErrInfo += _T("找圆失败");
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
				strErrInfo += _T("找线1失败");
				break;
			case -3:							
				strErrInfo += _T("找线2失败");
				break;
			case -4:
				strErrInfo +=_T("求交点失败");
				break;
			case -5:							
				strErrInfo += _T("找圆失败");
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