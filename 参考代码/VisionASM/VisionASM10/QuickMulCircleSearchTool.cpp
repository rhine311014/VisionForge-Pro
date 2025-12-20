// CircleSearchTool.cpp: implementation of the CQuickMulCircleSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
 #include "QuickMulCircleSearchTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#include "svLineSeg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuickMulCircleSearchTool::CQuickMulCircleSearchTool()
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

	m_dCenterDistance = 0.0;
	m_dRadius1 = 0.0;
	m_dRadius2 = 0.0;
	m_strInspectResultShow = _T("");
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle_1[0].SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle_1[0].SetLabelVisible(TRUE);

	m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan(400, 300, 100, 0, sk2PI);
	m_GuiFindCircle_1[1].SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle_1[1].SetLabelVisible(TRUE);


	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	// 找圆工具卡尺隐藏，矩形区域可见
	m_GuiFindCircle_1[0].SetCaliperVisible(false);
	m_GuiFindCircle_1[0].SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindCircle_1[1].SetCaliperVisible(false);
	m_GuiFindCircle_1[1].SetAnnulusFindCircleVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		m_GuiFindCircle_1[0].SetLabel(_T("FindCircle1"));
		m_GuiFindCircle_1[1].SetLabel(_T("FindCircle2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	}

	CreateGUID(m_strKeyText);

	// TrainData
	m_tmpTrainDataParam.m_dGrau					= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre			= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag				= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable			= false;

	m_tmpTrainDataParam.m_nLineAngleType		= 0;

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		m_FindCircleTool_1[i].GetCaliperProperty(
			m_tmpTrainDataParam.m_dCircleContrastThre[i], 
			m_tmpTrainDataParam.m_edgeCircleProperty[i], 
			m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);

		m_tmpTrainDataParam.m_nCircleOutLineNum[i]			= 10;
		m_tmpTrainDataParam.m_bCircleSortByScore[i]			= false;
	}

	// TrainGui
	m_tmpTrainGuiParam.m_TrainAffineRect		= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
// 	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
// 	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_nLineWidth				= 1;
	m_tmpTrainGuiParam.m_LineColor				= RGB(0, 0, 255);

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		m_GuiFindCircle_1[i].GetCenterRadiusAngleStartAngleSpan(
			m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i],
			m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);
		m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i]		= m_GuiFindCircle_1[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i]			= m_GuiFindCircle_1[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i]	= (CaliperSearchDirection)m_GuiFindCircle_1[i].GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_nCircleCaliperNum[i]				= m_GuiFindCircle_1[i].GetCaliperNum();
		m_tmpTrainGuiParam.m_bCircleCaliperVisible[i]			= m_GuiFindCircle_1[i].IsCaliperVisible();
		m_tmpTrainGuiParam.m_bEnableCircleMask[i]				= false;
	}

	// SearchData
	m_tmpSearchDataParam.m_dAcceptThre			= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow			= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh			= 5.0;
	m_tmpSearchDataParam.m_nSearchNum			= 1;
// 	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
// 	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();
	m_tmpSearchGuiParam.m_nLineWidth			= 1;
	m_tmpSearchGuiParam.m_LineColor				= RGB(0, 255, 0);

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CQuickMulCircleSearchTool::CQuickMulCircleSearchTool(CQuickMulCircleSearchTool &cCircleSearchTool)
{
	if (cCircleSearchTool.GetSearchToolType() != eQuickMulCircleSearchTool)
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

	m_dCenterDistance = 0.0;
	m_dRadius1 = 0.0;
	m_dRadius2 = 0.0;

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);

	m_GuiTrainRect.SetCenterLengthsRotationSkew(250, 200, 50, 50, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	
	m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, skPI);
	m_GuiFindCircle_1[0].SetLabelVisible(TRUE);

	m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan(500, 400, 100, 0, skPI);
	m_GuiFindCircle_1[1].SetLabelVisible(TRUE);

	
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);

	m_GuiFindCircle_1[0].SetCaliperVisible(false);
	m_GuiFindCircle_1[0].SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindCircle_1[1].SetCaliperVisible(false);
	m_GuiFindCircle_1[1].SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		m_GuiFindCircle_1[0].SetLabel(_T("FindCircle1"));
		m_GuiFindCircle_1[1].SetLabel(_T("FindCircle2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
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

CQuickMulCircleSearchTool& CQuickMulCircleSearchTool::operator=(CQuickMulCircleSearchTool &cCircleSearchTool)
{
	if (this == &cCircleSearchTool)
	{
		return *this;
	}

	if (cCircleSearchTool.GetSearchToolType() != eQuickMulCircleSearchTool)
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

	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiTrainRect.SetCenterLengthsRotationSkew(250, 200, 50, 50, 0, 0);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, skPI);
	m_GuiFindCircle_1[0].SetLabelVisible(TRUE);

	m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan(500, 400, 100, 0, skPI);
	m_GuiFindCircle_1[1].SetLabelVisible(TRUE);

	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	
	m_GuiFindCircle_1[0].SetCaliperVisible(false);
	m_GuiFindCircle_1[0].SetAnnulusFindCircleVisible(TRUE);
	m_GuiFindCircle_1[1].SetCaliperVisible(false);
	m_GuiFindCircle_1[1].SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));

		m_GuiFindCircle_1[0].SetLabel(_T("FindCircle1"));
		m_GuiFindCircle_1[1].SetLabel(_T("FindCircle2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
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

CQuickMulCircleSearchTool::~CQuickMulCircleSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CQuickMulCircleSearchTool::GetSearchToolType()
{
	return eQuickMulCircleSearchTool;
}										   

// 设置显示控件指针		
bool CQuickMulCircleSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CQuickMulCircleSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CQuickMulCircleSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CQuickMulCircleSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CQuickMulCircleSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CQuickMulCircleSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CQuickMulCircleSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CQuickMulCircleSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CQuickMulCircleSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CQuickMulCircleSearchTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		m_GuiFindCircle_1[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCircleVisible_1[i]);
		if (m_tmpTrainGuiParam.m_bEnableCircleMask[i])
		{
			m_GuiFindCircle_1[i].EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
				scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition|scGuiFindCircle::eGuiDOFMask);
		}
		else
		{
			m_GuiFindCircle_1[i].EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
				scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition);
		}
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));

		CString strTemp = _T("");
		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			strTemp.Format(_T("_FindCircle%d"), i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle_1[i], m_strKeyText + strTemp);
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
bool CQuickMulCircleSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		m_GuiTrainResultCircle_1[i].SetVisible(m_bTrainResultGuiShow);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();

		CString strTemp = _T("");
		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			strTemp.Format(_T("_TrainResultCircle%d"), i);
			m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle_1[i], m_strKeyText + strTemp);
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}		

// 更新搜索Gui控件显示														
bool CQuickMulCircleSearchTool::UpdateSearchResultGuiDisplay()
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
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible_1[i]);
	}

	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible_1[i]);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow/* && m_tmpSearchGuiParam.m_bCircleCenterVisible*/);
	}

	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow/* && m_tmpSearchGuiParam.m_bCircleCenterVisible*/);
	}


	for (i=0; i<m_pGuiSearchResultCircleFitPointArray1.size(); i++)
	{
		m_pGuiSearchResultCircleFitPointArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint[0]);
	}

	for (i=0; i<m_pGuiSearchResultCircleFitPointArray2.size(); i++)
	{
		m_pGuiSearchResultCircleFitPointArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint[1]);
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

		for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultCircleFitPointArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultCircleFitPointArray1[i], m_pGuiSearchResultCircleFitPointArray1[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultCircleFitPointArray2.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultCircleFitPointArray2[i], m_pGuiSearchResultCircleFitPointArray2[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CQuickMulCircleSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	//	UpdateTrainResultGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CQuickMulCircleSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CQuickMulCircleSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CQuickMulCircleSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	*((CQuickMulCircleSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CQuickMulCircleSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CQuickMulCircleSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CQuickMulCircleSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CQuickMulCircleSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CQuickMulCircleSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CQuickMulCircleSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CQuickMulCircleSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CQuickMulCircleSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CQuickMulCircleSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);
	m_nResultErrNumber=0;
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
			m_nResultErrNumber=-1;///几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			// 找圆
			m_FindCircleTool_1[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum[i]);
			m_FindCircleTool_1[i].SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum[i], 
												   m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i], 
												   m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i], 
												   m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i]);
			/*m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], 
													 m_tmpTrainDataParam.m_edgeCircleProperty[i], 
													 m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);*/
			m_FindCircleTool_1[i].SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum[i]);
			m_FindCircleTool_1[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore[i]);
			m_FindCircleTool_1[i].SetCenterRadiusAngleStartAngleSpan(
				 m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i], 
				 m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);
			m_FindCircleTool_1[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], m_tmpTrainDataParam.m_edgeCircleProperty[i],
					tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);
			} 
			else
			{
				m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], m_tmpTrainDataParam.m_edgeCircleProperty[i],
					 m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);
			}

			scCircle cCircle[QUICK_CIRCLE_NUM];
			double   dRMS[QUICK_CIRCLE_NUM];
			if(bIsProcessImage)
			{
				bRet = m_FindCircleTool_1[i].Execute(tmpImage, cCircle[i], dRMS[i]);
			}
			else
			{
				bRet = m_FindCircleTool_1[i].Execute(m_tmpImageTrain, cCircle[i], dRMS[i]);
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-2-i;///找圆工具失败
				return false;
			}
			m_tmpTrainResult.m_dCircleCenterX[i] = cCircle[i].GetCenter().GetX();
			m_tmpTrainResult.m_dCircleCenterY[i] = cCircle[i].GetCenter().GetY();
			m_tmpTrainResult.m_dCircleRadius[i]  = cCircle[i].GetRadius();

			m_GuiTrainResultCircle_1[i].SetCircle(cCircle[i]);
		}

		m_bTrainOK		 = true;
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
			m_nResultErrNumber=-1;///几何定位失败
			return false;
		}
		sc2Vector cModelOrigin;
		m_QuickSearchTool.GetModelOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			// 找圆
			m_FindCircleTool_1[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum[i]);
			m_FindCircleTool_1[i].SetCaliperParams( m_tmpTrainGuiParam.m_nCircleCaliperNum[i], 
													m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i], 
													m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i], 
													m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i]);
			/*m_FindCircleTool_1[i].SetCaliperProperty( m_tmpTrainDataParam.m_dCircleContrastThre[i], 
													  m_tmpTrainDataParam.m_edgeCircleProperty[i], 
													  m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);*/
			m_FindCircleTool_1[i].SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum[i]);
			m_FindCircleTool_1[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore[i]);
			m_FindCircleTool_1[i].SetCenterRadiusAngleStartAngleSpan(
				m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i], 
				m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);
			m_FindCircleTool_1[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], m_tmpTrainDataParam.m_edgeCircleProperty[i],
					tmpEdgePolarity, m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);
			} 
			else
			{
				m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], m_tmpTrainDataParam.m_edgeCircleProperty[i],
					m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);
			}

			scCircle cCircle[QUICK_CIRCLE_NUM];
			double   dRMS[QUICK_CIRCLE_NUM];
			if(bIsProcessImage)
			{
				bRet = m_FindCircleTool_1[i].Execute(tmpImage, cCircle[i], dRMS[i]);
			}
			else
			{
				bRet = m_FindCircleTool_1[i].Execute(m_tmpImageTrain, cCircle[i], dRMS[i]);
			}
			
			if (!bRet)
			{
				m_nResultErrNumber=-2-i;///找圆工具失败
				return false;
			}

			m_tmpTrainResult.m_dCircleCenterX[i] = cCircle[i].GetCenter().GetX();
			m_tmpTrainResult.m_dCircleCenterY[i] = cCircle[i].GetCenter().GetY();
			m_tmpTrainResult.m_dCircleRadius[i]  = cCircle[i].GetRadius();

			m_GuiTrainResultCircle_1[i].SetCircle(cCircle[i]);
		}


		m_bTrainOK		 = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CQuickMulCircleSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CQuickMulCircleSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	*((CQuickMulCircleSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CQuickMulCircleSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CQuickMulCircleSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CQuickMulCircleSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CQuickMulCircleSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	*((CQuickMulCircleSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CQuickMulCircleSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CQuickMulCircleSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CQuickMulCircleSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eQuickMulCircleSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CQuickMulCircleSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CQuickMulCircleSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;///几何定位失败
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

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		m_FindCircleTool_1[i].SetCaliperNum(m_TrainGuiParam.m_nCircleCaliperNum[i]);
		m_FindCircleTool_1[i].SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum[i], 
											   m_TrainGuiParam.m_CircleCaliperSearchDirection[i], 
											   m_TrainGuiParam.m_dCircleCaliperSearchLen[i], 
											   m_TrainGuiParam.m_dCircleCaliperProjectLen[i]);
		/*m_FindCircleTool_1[i].SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre[i], 
												 m_TrainDataParam.m_edgeCircleProperty[i], 
												 m_TrainDataParam.m_lCircleFilterHalfWidth[i]);*/
		m_FindCircleTool_1[i].SetOutLineNum(m_TrainDataParam.m_nCircleOutLineNum[i]);
		m_FindCircleTool_1[i].EnableCaliperSortByScore(m_TrainDataParam.m_bCircleSortByScore[i]);
		m_FindCircleTool_1[i].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask[i]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeCircleProperty[i] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindCircleTool_1[i].SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre[i], m_TrainDataParam.m_edgeCircleProperty[i], 
				tmpEdgePolarity,m_TrainDataParam.m_lCircleFilterHalfWidth[i]);
		} 
		else
		{
			m_FindCircleTool_1[i].SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre[i], 
				m_TrainDataParam.m_edgeCircleProperty[i], 
				m_TrainDataParam.m_lCircleFilterHalfWidth[i]);
		}
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
//		for (int j=0; j<nResultNum && j<MAXRESNUM; j++)
		for (int j=0; j<nResultNum && j<1; j++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(j);

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

			// 求找线工具变换后的位置,找Circle
			double dTransCenterX_1[QUICK_CIRCLE_NUM], dTransCenterY_1[QUICK_CIRCLE_NUM];
			double dOriCenterX_1[QUICK_CIRCLE_NUM], dOriCenterY_1[QUICK_CIRCLE_NUM];

			for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
			{
				dOriCenterX_1[i] = m_TrainGuiParam.m_CircleCenter_1[i].GetX();
				dOriCenterY_1[i] = m_TrainGuiParam.m_CircleCenter_1[i].GetY();

				dTransCenterX_1[i] = cos(dRotation) * dOriCenterX_1[i] - sin(dRotation) * dOriCenterY_1[i] + dTransX;
				dTransCenterY_1[i] = sin(dRotation) * dOriCenterX_1[i] + cos(dRotation) * dOriCenterY_1[i] + dTransY;

				m_FindCircleTool_1[i].SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum[i], 
													   m_TrainGuiParam.m_CircleCaliperSearchDirection[i], 
													   m_TrainGuiParam.m_dCircleCaliperSearchLen[i], 
													   m_TrainGuiParam.m_dCircleCaliperProjectLen[i]);
				m_FindCircleTool_1[i].SetCenterRadiusAngleStartAngleSpan(
					sc2Vector(dTransCenterX_1[i], dTransCenterY_1[i]), 
					m_TrainGuiParam.m_dCircleRadius[i], 
					scRadian(dRotation)+m_TrainGuiParam.m_CircleArcStart[i], 
					m_TrainGuiParam.m_CircleArcSpan[i]);
			}




			scCircle ResCircle_1[QUICK_CIRCLE_NUM];
			double dRMS_1[QUICK_CIRCLE_NUM];
			sc2Vector scCircleCenter[QUICK_CIRCLE_NUM];
		
			for (int k = 0; k < QUICK_CIRCLE_NUM; k++)
			{
				bool  bFindCircle = false;
				if(bIsProcessImage)
				{
					bFindCircle = m_FindCircleTool_1[k].Execute(tmpImage, ResCircle_1[k], dRMS_1[k]);
				}
				else
				{
					bFindCircle = m_FindCircleTool_1[k].Execute(ImageSearch, ResCircle_1[k], dRMS_1[k]);
				}
				if (!bFindCircle) 
				{
					m_nResultErrNumber=-2-k;///找圆工具失败
					return false;
				}

				scCircleCenter[k] = ResCircle_1[k].GetCenter();
			}


			CSearchResult searchResult;

			

			scLineSeg ResLineSeg;
			ResLineSeg.Set(scCircleCenter[0], scCircleCenter[1]);

			searchResult.m_dPosX = (ResLineSeg.GetP1().GetX() + ResLineSeg.GetP2().GetX())/2;
			searchResult.m_dPosY = (ResLineSeg.GetP1().GetY() + ResLineSeg.GetP2().GetY())/2;
			searchResult.m_dScore = tmpQuickResult.GetScore();

			searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle_1[0].GetRadius();
			searchResult.m_resultCircleSearchTool.m_scCircleCircle = sc2Vector(ResCircle_1[0].GetCenter().GetX(),ResCircle_1[0].GetCenter().GetY());
			searchResult.m_resultCircleSearchTool2.m_dRadius = ResCircle_1[1].GetRadius();
			searchResult.m_resultCircleSearchTool2.m_scCircleCircle = sc2Vector(ResCircle_1[1].GetCenter().GetX(),ResCircle_1[1].GetCenter().GetY());

			// 设置GUI显示参数
			CResultSearchRect resRect;
			resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
			searchResult.m_vTrainSearchRect.push_back(resRect);
			CResultCircleSearchTool circle;
			circle.m_dRadius = ResCircle_1[0].GetRadius();
			circle.m_scCircleCircle = ResCircle_1[0].GetCenter();
			searchResult.m_vCircleSearchResult.push_back(circle);
			circle.m_dRadius = ResCircle_1[1].GetRadius();
			circle.m_scCircleCircle = ResCircle_1[1].GetCenter();
			searchResult.m_vCircleSearchResult.push_back(circle);


			if (m_TrainDataParam.m_nLineAngleType == 0)	// 任意角度
			{
				searchResult.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

				searchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
				searchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
				searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
				searchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
				searchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
				searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
			}
			else if (m_TrainDataParam.m_nLineAngleType == 1)	//水平线
			{
				searchResult.m_dAngle = scDegree(ResLineSeg.GetRotation().SignedNormMod180()).ToDouble();

				scDegree degLineAngle;
				degLineAngle = scDegree(ResLineSeg.GetRotation()).SignedNorm();
				if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
				{
					searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
					searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
					searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
					searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
					searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
					searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
				}
				else
				{
					searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
					searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
					searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
					searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
					searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
					searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
				}
			}
			else if (m_TrainDataParam.m_nLineAngleType == 2)	//垂直线
			{
				searchResult.m_dAngle = scDegree(ResLineSeg.GetRotation().NormMod180()).ToDouble();

				scDegree degLineAngle;
				degLineAngle = scDegree(ResLineSeg.GetRotation()).Norm();
				if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
				{
					searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP1().GetX();
					searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP1().GetY();
					searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
					searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP2().GetX();
					searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP2().GetY();
					searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
				}
				else
				{
					searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg.GetP2().GetX();
					searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg.GetP2().GetY();
					searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
					searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg.GetP1().GetX();
					searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg.GetP1().GetY();
					searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
				}
			}
			else
			{
				searchResult.m_dAngle = scDegree(ResLineSeg.GetRotation().Norm()).ToDouble();

				searchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg.GetP1().GetX();
				searchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg.GetP1().GetY();
				searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
				searchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg.GetP2().GetX();
				searchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg.GetP2().GetY();
				searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
			}

			double dCenterDistanceX = 0.0;
			double dCenterDistanceY = 0.0;
			{
				m_dRadius1 = ResCircle_1[0].GetRadius()*m_tmpSearchDataParam.m_dPixelRatio;
				m_dRadius2 = ResCircle_1[1].GetRadius()*m_tmpSearchDataParam.m_dPixelRatio;
				sfDistancePoint2Point(scCircleCenter[0], scCircleCenter[1],m_dCenterDistance);
				m_dCenterDistance = m_dCenterDistance*m_tmpSearchDataParam.m_dPixelRatio;
				m_dCenterDistance = m_tmpSearchDataParam.m_dFactor_a*m_dCenterDistance + m_tmpSearchDataParam.m_dFactor_b;
				dCenterDistanceX = m_tmpSearchDataParam.m_dPixelRatio*(double)(scCircleCenter[1].GetX()-scCircleCenter[0].GetX());
				dCenterDistanceY = m_tmpSearchDataParam.m_dPixelRatio*(double)(scCircleCenter[1].GetY()-scCircleCenter[0].GetY());
				searchResult.m_vdAuxiliaryPosX.at(3) = dCenterDistanceX;
				searchResult.m_vdAuxiliaryPosY.at(3) = dCenterDistanceY;
				searchResult.m_vdAuxiliaryAngle.at(3) = m_dCenterDistance;
				m_strInspectResultShow.Format(_T("同心度CD:%.3fmm;CX:%.3fmm;CY:%.3fmm"),m_dCenterDistance,dCenterDistanceX,dCenterDistanceY);
				if(m_tmpSearchDataParam.m_bEnableInspect)
				{
					bool bInspectStatus = true;
					if(m_tmpSearchDataParam.m_bEnableMax1 && m_dRadius1>m_tmpSearchDataParam.m_dDistanceMax1)
					{
						m_nResultErrNumber = -4;
						bInspectStatus = false;
					}
					if(m_tmpSearchDataParam.m_bEnableMin1 && m_dRadius1<m_tmpSearchDataParam.m_dDistanceMin1)
					{
						m_nResultErrNumber = -4;
						bInspectStatus = false;
					}
					if(m_tmpSearchDataParam.m_bEnableMax2 && m_dRadius2>m_tmpSearchDataParam.m_dDistanceMax2)
					{
						m_nResultErrNumber = -5;
						bInspectStatus = false;
					}
					if(m_tmpSearchDataParam.m_bEnableMin2 && m_dRadius2<m_tmpSearchDataParam.m_dDistanceMin2)
					{
						m_nResultErrNumber = -5;
						bInspectStatus = false;
					}
					if(m_tmpSearchDataParam.m_bEnableMax3 && m_dCenterDistance>m_tmpSearchDataParam.m_dDistanceMax3)
					{
						m_nResultErrNumber = -6;
						bInspectStatus = false;
					}
					if(m_tmpSearchDataParam.m_bEnableMin3 && m_dCenterDistance<m_tmpSearchDataParam.m_dDistanceMin3)
					{
						m_nResultErrNumber = -6;
						bInspectStatus = false;
					}
			m_SearchResultArray.push_back(searchResult);
					if(bInspectStatus==false) return bInspectStatus;
				}
				else
				{
					m_SearchResultArray.push_back(searchResult);
				}
			}


			CString	strTmpKey;
			strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, j);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+j;
			pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, j);
			scGuiCross *pTempCross = m_GuiSearchCross+j;
			pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			m_pGuiSearchCrossArray.push_back(pTempCross);


			for (int k = 0; k < QUICK_CIRCLE_NUM; k++)
			{
				strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, k);
				scGuiCircle *pTemCircle = m_GuiSearchCircle+k;
				pTemCircle->SetCircle(ResCircle_1[k]);
				pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemCircle->SetTipTextEnabled(true);
				pTemCircle->SetTipText(strTmpKey);
				CString st;
				if(k==1)
				{
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f;同心度CD:%.3f;CX:%.3f;CY:%.3f"),k, ResCircle_1[k].GetCenter().GetX(), ResCircle_1[k].GetCenter().GetY(), ResCircle_1[k].GetRadius(),m_dCenterDistance,dCenterDistanceX,dCenterDistanceY);
				}
				else
				{
				st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), k, ResCircle_1[k].GetCenter().GetX(), ResCircle_1[k].GetCenter().GetY(), ResCircle_1[k].GetRadius());
				}
				pTemCircle->SetLabel(st);
				pTemCircle->SetLabelVisible(TRUE);
				m_pGuiSearchCircleArray.push_back(pTemCircle);


				strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, k);
				scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+k;
				pTempCircleCenter->SetCenterExtent(ResCircle_1[k].GetCenter(), sc2Vector(40,40));
				pTempCircleCenter->SetLineColor(RGB(255, 0, 0)/*m_tmpSearchGuiParam.m_LineColor*/);
				pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCircleCenter->SetTipTextEnabled(true);
				pTempCircleCenter->SetTipText(strTmpKey);
				m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);	

			}


			strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, j);
			pTempCross = m_GuiSearchLineCross+j;
			pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(60,60));
			pTempCross->SetLineColor(RGB(255, 0 ,0));
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			m_pGuiSearchLineCrossArray.push_back(pTempCross);

			strTmpKey.Format(_T("%s_ResultLine%d"), m_strKeyText, j);
			scGuiLineSeg *pTemLineSeg = m_GuiSearchLineSeg+j;
			pTemLineSeg->SetStartXYEndXY(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), 
										 sc2Vector(searchResult.m_vdAuxiliaryPosX.at(1),searchResult.m_vdAuxiliaryPosY.at(1)));
			pTemLineSeg->SetLineColor(RGB(255, 0, 0));
			pTemLineSeg->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemLineSeg->SetTipTextEnabled(true);
			pTemLineSeg->SetTipText(strTmpKey);

			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), j, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
			pTemLineSeg->SetLabel(st);
			pTemLineSeg->SetLabelVisible(TRUE);

			m_pGuiSearchLineArray.push_back(pTemLineSeg);



			//////////////////////////////////////////////////////////////////////////
			{
				CString strTmpKey;
				std::vector<bool> bUsed;
				std::vector<sc2Vector> points;
				m_FindCircleTool_1[0].GetUsed(bUsed);
				m_FindCircleTool_1[0].GetFitData(points);
				for (long m=0; m<bUsed.size() && m<points.size(); m++)
				{
					bool bUse = bUsed[m];
					sc2Vector pt = points[m];

					int nIndex = m_pGuiSearchResultCircleFitPointArray1.size();
					strTmpKey.Format(_T("%s_CircleFitPoint1_%d"), m_strKeyText, nIndex);
					scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultCircleFitPoint1 + nIndex;

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

					m_pGuiSearchResultCircleFitPointArray1.push_back(pTempFitPoint1);	

				}
			}


			{
				CString strTmpKey;
				std::vector<bool> bUsed;
				std::vector<sc2Vector> points;
				m_FindCircleTool_1[1].GetUsed(bUsed);
				m_FindCircleTool_1[1].GetFitData(points);
				for (long m=0; m<bUsed.size() && m<points.size(); m++)
				{
					bool bUse = bUsed[m];
					sc2Vector pt = points[m];

					int nIndex = m_pGuiSearchResultCircleFitPointArray2.size();
					strTmpKey.Format(_T("%s_CircleFitPoint2_%d"), m_strKeyText, nIndex);
					scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultCircleFitPoint2 + nIndex;

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

					m_pGuiSearchResultCircleFitPointArray2.push_back(pTempFitPoint1);	

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
int CQuickMulCircleSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CQuickMulCircleSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CQuickMulCircleSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eQuickMulCircleSearchTool)
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
bool CQuickMulCircleSearchTool::LoadModelFromFile(CString strDir)
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
	CQuickMulCircleSearchTrainDataParam tmpTrainDataParam;
	CQuickMulCircleSearchTrainGuiParam tmpTrainGuiParam;
	CQuickMulCircleSearchSearchDataParam tmpSearchDataParam;
	CQuickMulCircleSearchSearchGuiParam tmpSearchGuiParam;
	CQuickMulCircleSearchTrainResult  tmpTrainResult;
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
	if (cSearchToolType != eQuickMulCircleSearchTool)
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


	// LineAngle
	bRet = cXMLConfigurator.FindElem(_T("LineAngle"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLineAngleType = _ttoi(cXMLConfigurator.GetElemData());

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		// contrast thre
		strTemp.Format(_T("ContrastThre%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dCircleContrastThre[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);

		// FilterHalfWidth
		strTemp.Format(_T("FilterHalfWidth%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_lCircleFilterHalfWidth[i] = 1;
		}
		else
		{
			tmpTrainDataParam.m_lCircleFilterHalfWidth[i] = _ttol(cXMLConfigurator.GetElemData());
		}


		// outlinenum
		strTemp.Format(_T("OutLineNum%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_nCircleOutLineNum[i] = _ttol(cXMLConfigurator.GetElemData());


		// edgeproperty
		strTemp.Format(_T("EdgeProperty%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_edgeCircleProperty[i] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());


		// SortByScore
		strTemp.Format(_T("SortByScore%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bCircleSortByScore[i] = _ttoi(cXMLConfigurator.GetElemData());

		// DualEdge
		strTemp.Format(_T("DualEdge%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_bEnableDualEdge[i] = false;
		}
		else
		{
			tmpTrainDataParam.m_bEnableDualEdge[i] = _ttoi(cXMLConfigurator.GetElemData());
		}
		
	}


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

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		strInfo.Format(_T("CircleCenter%d"), i);
		// circle center
		bRet = cXMLConfigurator.FindElem(strTemp);
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
		tmpTrainGuiParam.m_CircleCenter_1[i] = vPos;
		cXMLConfigurator.LeaveElem();


		// circle radius  
		strTemp.Format(_T("CircleRadius%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCircleRadius[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);

		// Arc start   
		strTemp.Format(_T("ArcStart%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CircleArcStart[i] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));


		// Arc span   
		strTemp.Format(_T("ArcSpan%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CircleArcSpan[i] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));


		// Caliper Num 
		strTemp.Format(_T("CaliperNum%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_nCircleCaliperNum[i] = _ttoi(cXMLConfigurator.GetElemData());

		// Project Len
		strTemp.Format(_T("ProjectLen%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCircleCaliperProjectLen[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);


		// Search Len
		strTemp.Format(_T("SearchLen%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCircleCaliperSearchLen[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);

		// CircleCaliper Visible
		strTemp.Format(_T("CircleCaliperVisible%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainGuiParam.m_bCircleCaliperVisible[i] = true;
		}
		else
		{
			tmpTrainGuiParam.m_bCircleCaliperVisible[i] = _ttoi(cXMLConfigurator.GetElemData());
		}
		
		{
			//Mask
			strTemp.Format(_T("CaliperMaskEnable%d"),i);
			bRet = cXMLConfigurator.FindElem(strTemp);
			if (bRet)
			{
				tmpTrainGuiParam.m_bEnableCircleMask[i] = _ttol(cXMLConfigurator.GetElemData());
			}
			else
			{
				tmpTrainGuiParam.m_bEnableCircleMask[i] = false;
			}
			strTemp.Format(_T("MaskParam%d"),i);
			bRet = cXMLConfigurator.FindElem(strTemp);
			if (bRet)
			{
				CString strTmp;
				cXMLConfigurator.EnterElem();

				bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
				if (!bRet) return false;
				tmpTrainGuiParam.m_vdCaliperMaskData[i].resize(_ttoi(cXMLConfigurator.GetElemData()));

				for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData[i].size(); j++)
				{
					strTmp.Format(_T("MaskData%d"), j);
					bRet = cXMLConfigurator.FindElem(strTmp);
					if (!bRet)
					{
						return false;
					}
					tmpTrainGuiParam.m_vdCaliperMaskData[i].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
				}
				bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
				if (!bRet) return false;
				tmpTrainGuiParam.m_vbCaliperMask[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
				for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask[i].size(); j++)
				{
					strTmp.Format(_T("EnableMaskData%d"), j);
					bRet = cXMLConfigurator.FindElem(strTmp);
					if (!bRet)
					{
						return false;
					}			
					tmpTrainGuiParam.m_vbCaliperMask[i].at(j) = _ttoi(cXMLConfigurator.GetElemData());
				}

				cXMLConfigurator.LeaveElem();
			}
		}


		// Search Dirction
		strTemp.Format(_T("SearchDir%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());
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

	bRet = cXMLConfigurator.FindElem(_T("EnableInspect"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableInspect = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dPixelRatio"));
	if (bRet)
	{
		tmpSearchDataParam.m_dPixelRatio = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMax1"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMax1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMin1"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMin1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMax1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMax1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMin1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMin1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("bEnableMax2"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMax2 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMin2"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMin2 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMax2"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMax2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMin2"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMin2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMax3"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMax3 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("bEnableMin3"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableMin3 = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMax3"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMax3 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dDistanceMin3"));
	if (bRet)
	{
		tmpSearchDataParam.m_dDistanceMin3 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dFactor_a"));
	if (bRet)
	{
		tmpSearchDataParam.m_dFactor_a = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("dFactor_b"));
	if (bRet)
	{
		tmpSearchDataParam.m_dFactor_b = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
// 	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("CircleFitPointVisible%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpSearchGuiParam.m_bEnableShowCirclePoint[i] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中
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

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		strTemp.Format(_T("CenterX%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		strTemp.Format(_T("CenterY%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		tmpTrainResult.m_dCircleCenterX[i] = vPos.GetX();
		tmpTrainResult.m_dCircleCenterY[i] = vPos.GetY();

		strTemp.Format(_T("Radius%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainResult.m_dCircleRadius[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


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
bool CQuickMulCircleSearchTool::SaveModelToFile(CString strDir)
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

	// LineAngle
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType);
	cXMLConfigurator.AddElem(_T("LineAngle"), strInfo);

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");
		
		// contrastthre
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dCircleContrastThre[i]);
		strTemp.Format(_T("ContrastThre%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// fileterhalfwidth
		strInfo.Format(_T("%d"), m_TrainDataParam.m_lCircleFilterHalfWidth[i]);
		strTemp.Format(_T("FilterHalfWidth%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// outlinenum
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nCircleOutLineNum[i]);
		strTemp.Format(_T("OutLineNum%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// edgeproperty
		strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeCircleProperty[i]);
		strTemp.Format(_T("EdgeProperty%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// SortByScore
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bCircleSortByScore[i]);
		strTemp.Format(_T("SortByScore%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// DualEdge
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[i]);
		strTemp.Format(_T("DualEdge%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

	}

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

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		// circle center		
		strTemp.Format(_T("CircleCenter%d"), i);
		cXMLConfigurator.AddElem(strTemp);
		cXMLConfigurator.EnterElem();

		strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter_1[i].GetX());
		cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter_1[i].GetY());
		cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
		cXMLConfigurator.LeaveElem();

		// circle radius 
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleRadius[i]);
		strTemp.Format(_T("CircleRadius%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// Arc start 
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleArcStart[i].ToDouble());
		strTemp.Format(_T("ArcStart%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// Arc Span
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleArcSpan[i].ToDouble());
		strTemp.Format(_T("ArcSpan%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// Caliper Num 
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCircleCaliperNum[i]);
		strTemp.Format(_T("CaliperNum%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// Project Len
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleCaliperProjectLen[i]);
		strTemp.Format(_T("ProjectLen%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// Search Len
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCircleCaliperSearchLen[i]);
		strTemp.Format(_T("SearchLen%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		//CircleCaliper Visible1
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleCaliperVisible[i]);
		strTemp.Format(_T("CircleCaliperVisible%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);


		//CircleMask
		CString strTmp;
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableCircleMask[i]);
		strTemp.Format(_T("CaliperMaskEnable%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("MaskParam%d"),i);
		cXMLConfigurator.DeleteElem(strTemp);
		cXMLConfigurator.AddElem(strTemp);
		cXMLConfigurator.EnterElem();

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[i].size());
		cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[i].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[i].at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[i].size());
		cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[i].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[i].at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}
		cXMLConfigurator.LeaveElem();

		// Search Dircetion
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_CircleCaliperSearchDirection[i]);
		strTemp.Format(_T("SearchDir%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);

	
		
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

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableInspect);
	cXMLConfigurator.AddElem(_T("EnableInspect"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dPixelRatio);
	cXMLConfigurator.AddElem(_T("dPixelRatio"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMax1);
	cXMLConfigurator.AddElem(_T("bEnableMax1"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMin1);
	cXMLConfigurator.AddElem(_T("bEnableMin1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMax1);
	cXMLConfigurator.AddElem(_T("dDistanceMax1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMin1);
	cXMLConfigurator.AddElem(_T("dDistanceMin1"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMax2);
	cXMLConfigurator.AddElem(_T("bEnableMax2"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMin2);
	cXMLConfigurator.AddElem(_T("bEnableMin2"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMax2);
	cXMLConfigurator.AddElem(_T("dDistanceMax2"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMin2);
	cXMLConfigurator.AddElem(_T("dDistanceMin2"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMax3);
	cXMLConfigurator.AddElem(_T("bEnableMax3"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableMin3);
	cXMLConfigurator.AddElem(_T("bEnableMin3"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMax3);
	cXMLConfigurator.AddElem(_T("dDistanceMax3"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dDistanceMin3);
	cXMLConfigurator.AddElem(_T("dDistanceMin3"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dFactor_a);
	cXMLConfigurator.AddElem(_T("dFactor_a"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dFactor_b);
	cXMLConfigurator.AddElem(_T("dFactor_b"), strInfo);

// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
// 	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		strTemp.Format(_T("CircleFitPointVisible%d"), i);
		strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowCirclePoint[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
	}

	//  // 将SearchGuiParam的参数，放在SearchDataParam括号中
	cXMLConfigurator.LeaveElem();


	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);	

	for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
	{
		CString strTemp = _T("");

		strTemp.Format(_T("CenterX%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterX[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("CenterY%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dCircleCenterY[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("Radius%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dCircleRadius[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);	
	}
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
bool CQuickMulCircleSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCircleCaliperNum[i]		= m_GuiFindCircle_1[i].GetCaliperNum();

			m_GuiFindCircle_1[i].GetCenterRadiusAngleStartAngleSpan(
				m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i],
				m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);

			m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i] = m_GuiFindCircle_1[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i]	= m_GuiFindCircle_1[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)m_GuiFindCircle_1[i].GetCaliperSearchDirection();
			m_tmpTrainGuiParam.m_bCircleCaliperVisible[i]=m_GuiFindCircle_1[i].IsCaliperVisible();
			m_GuiFindCircle_1[i].GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindCircle_1[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
		}
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

		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			m_GuiFindCircle_1[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindCircle_1[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);

			m_GuiFindCircle_1[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum[i]);

			m_GuiFindCircle_1[i].SetCenterRadiusAngleStartAngleSpan(
				m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i], 
				m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);

			m_GuiFindCircle_1[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i]);
			m_GuiFindCircle_1[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i]);
			m_GuiFindCircle_1[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i]);
			m_GuiFindCircle_1[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCircleCaliperVisible[i]);
			m_GuiFindCircle_1[i].SetAnnulusFindCircleVisible(!m_tmpTrainGuiParam.m_bCircleCaliperVisible[i]);
			m_GuiFindCircle_1[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
		}
	}

	return true;
}

bool CQuickMulCircleSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

// 		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
// 		{
// 			m_pGuiSearchCircleCenterArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			m_pGuiSearchCircleCenterArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 		}

	}

	return true;
}

// 清除控件及其显示					
bool CQuickMulCircleSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

bool CQuickMulCircleSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CQuickMulCircleSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultCircleFitPointArray1);
	ClearGraphicsArray(m_pGuiSearchResultCircleFitPointArray2);

	return true;
}															

bool CQuickMulCircleSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));

		CString strTemp = _T("");
		for (int i = 0; i < QUICK_CIRCLE_NUM; i++)
		{
			strTemp.Format(_T("_FindCircle%d"),i);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText + strTemp);

			strTemp.Format(_T("_TrainResultCircle%d"),i);
			m_pGuiStaticContainer->RemoveItem(m_strKeyText + strTemp);
		}
	}

	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;	
}

bool CQuickMulCircleSearchTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultCircleFitPointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultCircleFitPointArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultCircleFitPointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultCircleFitPointArray2[j]->GetTipText());
		}
	}

	return true;	
}

// 获取训练模板原始图像
bool CQuickMulCircleSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CQuickMulCircleSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CQuickMulCircleSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CQuickMulCircleSearchTool::SetLanguage(int nLanguage)
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

		m_GuiFindCircle_1[0].SetLabel(_T("FindCircle1"));
		m_GuiFindCircle_1[1].SetLabel(_T("FindCircle2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));

		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		break;
	}
}

bool CQuickMulCircleSearchTool::SetGuiByCam(double nWidth,double nHeight)
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
    m_GuiFindCircle_1[0].GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
    m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

    m_GuiFindCircle_1[1].GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
    m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

    double plength,slength;
    plength = m_GuiFindCircle_1[0].GetCaliperProjectionLength();
    slength = m_GuiFindCircle_1[0].GetCaliperSearchLength();
    m_GuiFindCircle_1[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindCircle_1[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    plength = m_GuiFindCircle_1[1].GetCaliperProjectionLength();
    slength = m_GuiFindCircle_1[1].GetCaliperSearchLength();
    m_GuiFindCircle_1[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindCircle_1[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    //参考点位置
    m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
    m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

    m_CamHeight = nHeight;
    m_CamWidth = nWidth;

    return true;
}

CString CQuickMulCircleSearchTool::GetResultErrInfo()
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
					strErrInfo = m_strInspectResultShow;
				}
				break;
			case -1:			//几何定位失败				
				strErrInfo += _T("Quick search failed");
				break;
			case -2:			//找圆1失败				
				strErrInfo += _T("Find circle1 failed");
				break;
			case -3:			//找圆2失败				
				strErrInfo += _T("Find circle2 failed");
				break;
			case -4:							
				strErrInfo += _T("circle1 Radius Overrun");
				break;
			case -5:							
				strErrInfo += _T("circle2 Radius Overrun");
				break;
			case -6:							
				strErrInfo += _T("Center Distance Overrun");
				break;
			case -99:			//	训练未成功			
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
					strErrInfo = m_strInspectResultShow;
				}
				break;
			case -1:							
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							
				strErrInfo += _T("找圆1失败");
				break;
			case -3:							
				strErrInfo += _T("找圆2失败");
				break;
			case -4:							
				strErrInfo += _T("圆1半径超限");
				break;
			case -5:							
				strErrInfo += _T("圆2半径超限");
				break;
			case -6:							
				strErrInfo += _T("圆心距超限");
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
					strErrInfo = m_strInspectResultShow;
				}
				break;
			case -1:							
				strErrInfo += _T("几何定位失败");
				break;
			case -2:							
				strErrInfo += _T("找圆1失败");
				break;
			case -3:							
				strErrInfo += _T("找圆2失败");
				break;
			case -4:							
				strErrInfo += _T("圆1半径超限");
				break;
			case -5:							
				strErrInfo += _T("圆2半径超限");
				break;
			case -6:							
				strErrInfo += _T("圆心距超限");
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