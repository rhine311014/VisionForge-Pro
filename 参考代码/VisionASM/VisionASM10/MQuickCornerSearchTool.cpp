// CMQuickCornerSearchTool.cpp: implementation of the CMQuickCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MQuickCornerSearchTool.h"
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

CMQuickCornerSearchTool::CMQuickCornerSearchTool()
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
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

	m_GuiFindLine[0].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[1].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[2].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetStartXYEndXY(425, 350, 275, 350);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);
	
	int i=0;
	for (i=0; i<LINES_NUM; i++)
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

	m_tmpTrainDataParam.m_dGrau							= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre					= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag						= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable					= false;
	m_tmpTrainGuiParam.m_TrainAffineRect				= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint						= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect						= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth						= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor						= m_GuiTrainRect.GetLineColor();

	for (i=0; i<LINES_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
			m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_bEnableSortByScore[i]		= TRUE;
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 0;
		m_tmpTrainGuiParam.m_nCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
			m_tmpTrainGuiParam.m_dEndY[i]);
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();

		m_tmpTrainGuiParam.m_bEnableLineMask[i]			= false;
	}

	m_tmpSearchDataParam.m_dAcceptThre					= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow					= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh					= 5.0;
	m_tmpSearchDataParam.m_nSearchNum					= 1;
	m_tmpSearchDataParam.m_dSizeLow			= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh		= 1.0;
	m_tmpSearchDataParam.m_bCoordSortEnable		= false;
	m_tmpSearchDataParam.m_bGetFourResultEnable = false;
	m_tmpSearchDataParam.m_nCoordSortMethod		= eCoordXOnly;
	m_tmpSearchDataParam.m_nCoordSortOrderX		= eAscending;
	m_tmpSearchDataParam.m_nCoordSortOrderY		= eAscending;;
	m_tmpSearchDataParam.m_dCoordSortGap		= 64;

	m_tmpSearchGuiParam.m_nLineWidth					= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor						= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam									= m_tmpTrainDataParam;
	m_TrainGuiParam										= m_tmpTrainGuiParam;
	m_SearchDataParam									= m_tmpSearchDataParam;
	m_SearchGuiParam									= m_tmpSearchGuiParam;
}

CMQuickCornerSearchTool::CMQuickCornerSearchTool(CMQuickCornerSearchTool &cMQuickCornerSearchTool)
{
	if (cMQuickCornerSearchTool.GetSearchToolType() != eMQuickCornerSearchTool)
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

	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
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


	m_GuiFindLine[0].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[1].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[2].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetStartXYEndXY(425, 350, 275, 350);
	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);

	for (int i=0; i<LINES_NUM; i++)
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
	m_CamHeight								= cMQuickCornerSearchTool.m_CamHeight;
	m_CamWidth								= cMQuickCornerSearchTool.m_CamWidth;
	m_TrainDataParam						= cMQuickCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cMQuickCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cMQuickCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cMQuickCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cMQuickCornerSearchTool.m_TrainResult;
	m_ImageTrain							= cMQuickCornerSearchTool.m_ImageTrain;
	m_ImageMask								= cMQuickCornerSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cMQuickCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cMQuickCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cMQuickCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cMQuickCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cMQuickCornerSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cMQuickCornerSearchTool.UpdateSearchGuiData(true);
	cMQuickCornerSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cMQuickCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cMQuickCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cMQuickCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cMQuickCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cMQuickCornerSearchTool.m_tmpImageTrain);
	SetMaskImage(cMQuickCornerSearchTool.m_tmpImageMask);
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

CMQuickCornerSearchTool& CMQuickCornerSearchTool::operator=(CMQuickCornerSearchTool &cMQuickCornerSearchTool)
{
	if (this == &cMQuickCornerSearchTool)
	{
		return *this;
	}

	if (cMQuickCornerSearchTool.GetSearchToolType() != eMQuickCornerSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
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


	m_GuiFindLine[0].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[1].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[2].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetStartXYEndXY(425, 350, 275, 350);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[2].SetCaliperVisible(FALSE);
	m_GuiFindLine[2].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[3].SetCaliperVisible(FALSE);
	m_GuiFindLine[3].SetAffineRectFindLineVisible(TRUE);
	for (int i=0; i<LINES_NUM; i++)
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
	m_CamHeight								= cMQuickCornerSearchTool.m_CamHeight;
	m_CamWidth								= cMQuickCornerSearchTool.m_CamWidth;
	m_TrainDataParam						= cMQuickCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cMQuickCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cMQuickCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cMQuickCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cMQuickCornerSearchTool.m_TrainResult;
	m_ImageTrain							= cMQuickCornerSearchTool.m_ImageTrain;
	m_ImageMask								= cMQuickCornerSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cMQuickCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cMQuickCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cMQuickCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cMQuickCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cMQuickCornerSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cMQuickCornerSearchTool.UpdateSearchGuiData(true);
	cMQuickCornerSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cMQuickCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cMQuickCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cMQuickCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cMQuickCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cMQuickCornerSearchTool.m_tmpImageTrain);
	SetMaskImage(cMQuickCornerSearchTool.m_tmpImageMask);
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

CMQuickCornerSearchTool::~CMQuickCornerSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CMQuickCornerSearchTool::GetSearchToolType()
{
	return eMQuickCornerSearchTool;
}										   

// 设置显示控件指针		
bool CMQuickCornerSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CMQuickCornerSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CMQuickCornerSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CMQuickCornerSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CMQuickCornerSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CMQuickCornerSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CMQuickCornerSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CMQuickCornerSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;
	
	UpdateTrainResultGuiDisplay();
	
	return true;
}	

// 更新搜索界面显示									
bool CMQuickCornerSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CMQuickCornerSearchTool::UpdateTrainGuiDisplay()
{
	// 更新
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	for (int i=0; i<LINES_NUM; i++)
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
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));

		for (int i=0; i<LINES_NUM; i++)
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
bool CMQuickCornerSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);
    m_GuiTrainResultLine[2].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[3].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
	//	m_pGuiStaticContainer->Clear();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[2], m_strKeyText+_T("_TrainResultLine3"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[3], m_strKeyText+_T("_TrainResultLine4"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}				

// 更新搜索Gui控件显示														
bool CMQuickCornerSearchTool::UpdateSearchResultGuiDisplay()
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

	for (i=0; i<m_pGuiSearchResultFitPointArray_3.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_3[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint2);
	}

	for (i=0; i<m_pGuiSearchResultFitPointArray_4.size(); i++)
	{
		m_pGuiSearchResultFitPointArray_4[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint3);
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

			for (i=0; i<m_pGuiSearchResultFitPointArray_3.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_3[i], m_pGuiSearchResultFitPointArray_3[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPointArray_4.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPointArray_4[i], m_pGuiSearchResultFitPointArray_4[i]->GetTipText());
			}

		}
		
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}


	
	return true;
}												

// 更新界面显示
bool CMQuickCornerSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

//	UpdateTrainResultGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CMQuickCornerSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CMQuickCornerSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CMQuickCornerSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	*((CMQuickCornerSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CMQuickCornerSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CMQuickCornerSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CMQuickCornerSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CMQuickCornerSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CMQuickCornerSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CMQuickCornerSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 执行训练											
bool CMQuickCornerSearchTool::Train()
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
	if (m_tmpTrainDataParam.m_bMaskEnable)//使用掩膜
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
		// 找线0
		m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
		m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
		m_FindLineTool[0].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[0]);
//		m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0]);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[0])
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

			m_FindLineTool[0].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		} 
		else
		{
			m_FindLineTool[0].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		}

		m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);

		
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
			m_nResultErrNumber=-2;       //找线1失败
			return false;
		}
		// 找线1
		m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
		m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
		m_FindLineTool[1].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[1]);
//		m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1]);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
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

			m_FindLineTool[1].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		} 
		else
		{
			m_FindLineTool[1].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		}

		m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);

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
			m_nResultErrNumber=-3;       //找线2失败
			return false;
		}
		
		// 找线2
		m_FindLineTool[2].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[2]);
		m_FindLineTool[2].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[2], m_tmpTrainGuiParam.m_CaliperSearchRotation[2], m_tmpTrainGuiParam.m_dCaliperSearchLen[2]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[2]);
		m_FindLineTool[2].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[2]);
//		m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		m_FindLineTool[2].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[2]);
		m_FindLineTool[2].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[2], m_tmpTrainGuiParam.m_dStartY[2], 
			m_tmpTrainGuiParam.m_dEndX[2], m_tmpTrainGuiParam.m_dEndY[2]);


		if (m_tmpTrainDataParam.m_bEnableDualEdge[2])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[2] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[2] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}

			m_FindLineTool[2].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[2], 
				m_tmpTrainDataParam.m_edgeProperty[2],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		} 
		else
		{
			m_FindLineTool[2].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[2], 
				m_tmpTrainDataParam.m_edgeProperty[2], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		}

		m_FindLineTool[2].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[2]);
		
		scLineSeg cLineSeg3;
		double dRMS3;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[2].Execute(tmpImage, cLineSeg3, dRMS3);
		}
		else
		{
			bRet = m_FindLineTool[2].Execute(m_tmpImageTrain, cLineSeg3, dRMS3);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-4;       //找线3失败
			return false;
		}
		
		// 找线3
		m_FindLineTool[3].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[3]);
		m_FindLineTool[3].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[3], m_tmpTrainGuiParam.m_CaliperSearchRotation[3], m_tmpTrainGuiParam.m_dCaliperSearchLen[3]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[3]);
		m_FindLineTool[3].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[3]);
//		m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		m_FindLineTool[3].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[3]);
		m_FindLineTool[3].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[3], m_tmpTrainGuiParam.m_dStartY[3], 
			m_tmpTrainGuiParam.m_dEndX[3], m_tmpTrainGuiParam.m_dEndY[3]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[3])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[3] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[3] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}

			m_FindLineTool[3].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[3], 
				m_tmpTrainDataParam.m_edgeProperty[3],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		} 
		else
		{
			m_FindLineTool[3].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[3], 
				m_tmpTrainDataParam.m_edgeProperty[3], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		}

		m_FindLineTool[3].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[3]);
	
		scLineSeg cLineSeg4;
		double dRMS4;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[3].Execute(tmpImage, cLineSeg4, dRMS4);
		}
		else
		{
			bRet = m_FindLineTool[3].Execute(m_tmpImageTrain, cLineSeg4, dRMS4);
		}
		if (!bRet)
		{
			m_nResultErrNumber=-5;       //找线4失败
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
		
		m_tmpTrainResult.m_dStartX[2] = cLineSeg3.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[2] = cLineSeg3.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[2] = cLineSeg3.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[2] = cLineSeg3.GetP2().GetY();
		m_tmpTrainResult.m_dStartX[3] = cLineSeg4.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[3] = cLineSeg4.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[3] = cLineSeg4.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[3] = cLineSeg4.GetP2().GetY();
		// 求交点1(1,2交点)
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		std::vector<sc2Vector> vectPoint;
		
		
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-6;       //线1线2求交点失败
			return false;
		}
		
		m_tmpTrainResult.m_dModelX0 = vectArray[0].GetX();
		m_tmpTrainResult.m_dModelY0 = vectArray[0].GetY();
		vectPoint.push_back(vectArray.at(0));
		
		
		// 求交点2(2,3交点)
// 		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		
// 		m_tmpTrainResult.m_dModelX1 = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY1 = vectArray[0].GetY();
// 		vectPoint.push_back(vectArray.at(0));
		
		// 求交点3(3,4交点)
		bRet = sfInterSectLineLine(cLineSeg3.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-7;       //线3线4求交点失败
			return false;
		}
		
		m_tmpTrainResult.m_dModelX2 = vectArray[0].GetX();
		m_tmpTrainResult.m_dModelY2 = vectArray[0].GetY();
		vectPoint.push_back(vectArray.at(0));
		
		// 求交点4(4,1交点)
// 		bRet = sfInterSectLineLine(cLineSeg4.GetLine(), cLineSeg1.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		
// 		m_tmpTrainResult.m_dModelX3 = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY3 = vectArray[0].GetY();
// 		vectPoint.push_back(vectArray.at(0));
		
// 		scLineSeg cLineSeg02(vectPoint.at(0),vectPoint.at(2));
// 		scLineSeg cLineSeg13(vectPoint.at(1),vectPoint.at(3));
		
		// 求中心交点
// 		bRet = sfInterSectLineLine(cLineSeg02.GetLine(), cLineSeg13.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		m_tmpTrainResult.m_dModelX = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY = vectArray[0].GetY();
		
		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
		m_GuiTrainResultLine[2].SetLine(cLineSeg3.GetLine());
		m_GuiTrainResultLine[3].SetLine(cLineSeg4.GetLine());
		
		return true;
		
	}
	else  //非掩膜
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
			m_nResultErrNumber=-1;       //几何定位失败
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
		m_FindLineTool[0].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[0]);
//		m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0]);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[0])
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

			m_FindLineTool[0].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		} 
		else
		{
			m_FindLineTool[0].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[0], 
				m_tmpTrainDataParam.m_edgeProperty[0], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
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
			m_nResultErrNumber=-2;       //找线1失败
			return false;
		}
		// 找线1
		m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
		m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
		m_FindLineTool[1].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[1]);
//		m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1]);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[1])
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

			m_FindLineTool[1].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		} 
		else
		{
			m_FindLineTool[1].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[1], 
				m_tmpTrainDataParam.m_edgeProperty[1], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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
			m_nResultErrNumber=-3;       //找线2失败
			return false;
		}
		
		// 找线2
		m_FindLineTool[2].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[2]);
		m_FindLineTool[2].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[2], m_tmpTrainGuiParam.m_CaliperSearchRotation[2], m_tmpTrainGuiParam.m_dCaliperSearchLen[2]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[2]);
		m_FindLineTool[2].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[2]);
//		m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		m_FindLineTool[2].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[2]);
		m_FindLineTool[2].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[2], m_tmpTrainGuiParam.m_dStartY[2], 
			m_tmpTrainGuiParam.m_dEndX[2], m_tmpTrainGuiParam.m_dEndY[2]);


		if (m_tmpTrainDataParam.m_bEnableDualEdge[2])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[2] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[2] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}

			m_FindLineTool[2].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[2], 
				m_tmpTrainDataParam.m_edgeProperty[2],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		} 
		else
		{
			m_FindLineTool[2].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[2], 
				m_tmpTrainDataParam.m_edgeProperty[2], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		}
		
		scLineSeg cLineSeg3;
		double dRMS3;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[2].Execute(tmpImage, cLineSeg3, dRMS3);
		}
		else
		{
			bRet = m_FindLineTool[2].Execute(m_tmpImageTrain, cLineSeg3, dRMS3);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-4;       //找线3失败
			return false;
		}
		
		// 找线3
		m_FindLineTool[3].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[3]);
		m_FindLineTool[3].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[3], m_tmpTrainGuiParam.m_CaliperSearchRotation[3], m_tmpTrainGuiParam.m_dCaliperSearchLen[3]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[3]);
		m_FindLineTool[3].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[3]);
//		m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		m_FindLineTool[3].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[3]);
		m_FindLineTool[3].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[3], m_tmpTrainGuiParam.m_dStartY[3], 
			m_tmpTrainGuiParam.m_dEndX[3], m_tmpTrainGuiParam.m_dEndY[3]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[3])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeProperty[3] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeProperty[3] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}

			m_FindLineTool[3].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[3], 
				m_tmpTrainDataParam.m_edgeProperty[3],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		} 
		else
		{
			m_FindLineTool[3].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[3], 
				m_tmpTrainDataParam.m_edgeProperty[3], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		}
		
		scLineSeg cLineSeg4;
		double dRMS4;
		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[3].Execute(tmpImage, cLineSeg4, dRMS4);
		}
		else
		{
			bRet = m_FindLineTool[3].Execute(m_tmpImageTrain, cLineSeg4, dRMS4);
		}
		
		if (!bRet)
		{
			m_nResultErrNumber=-5;       //找线4失败
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
		
		m_tmpTrainResult.m_dStartX[2] = cLineSeg3.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[2] = cLineSeg3.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[2] = cLineSeg3.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[2] = cLineSeg3.GetP2().GetY();
		m_tmpTrainResult.m_dStartX[3] = cLineSeg4.GetP1().GetX();
		m_tmpTrainResult.m_dStartY[3] = cLineSeg4.GetP1().GetY();
		m_tmpTrainResult.m_dEndX[3] = cLineSeg4.GetP2().GetX();
		m_tmpTrainResult.m_dEndY[3] = cLineSeg4.GetP2().GetY();
		// 求交点1(1,2交点)
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		std::vector<sc2Vector> vectPoint;
		
		
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-6;       //线1线2求交点失败
			return false;
		}
		
		m_tmpTrainResult.m_dModelX0 = vectArray[0].GetX();
		m_tmpTrainResult.m_dModelY0 = vectArray[0].GetY();
		vectPoint.push_back(vectArray.at(0));
		
		
		// 求交点2(2,3交点)
// 		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		
// 		m_tmpTrainResult.m_dModelX1 = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY1 = vectArray[0].GetY();
// 		vectPoint.push_back(vectArray.at(0));
		
		// 求交点3(3,4交点)
		bRet = sfInterSectLineLine(cLineSeg3.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-7;       //线3线4求交点失败
			return false;
		}
		
		m_tmpTrainResult.m_dModelX2 = vectArray[0].GetX();
		m_tmpTrainResult.m_dModelY2 = vectArray[0].GetY();
		vectPoint.push_back(vectArray.at(0));
		
		// 求交点4(4,1交点)
// 		bRet = sfInterSectLineLine(cLineSeg4.GetLine(), cLineSeg1.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		
// 		m_tmpTrainResult.m_dModelX3 = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY3 = vectArray[0].GetY();
// 		vectPoint.push_back(vectArray.at(0));
// 		
// 		scLineSeg cLineSeg02(vectPoint.at(0),vectPoint.at(2));
// 		scLineSeg cLineSeg13(vectPoint.at(1),vectPoint.at(3));
		
		// 求中心交点
// 		bRet = sfInterSectLineLine(cLineSeg02.GetLine(), cLineSeg13.GetLine(), bInterSect, vectArray);
// 		if (!bRet || !bInterSect || vectArray.size() <= 0)
// 		{
// 			return false;
// 		}
// 		m_tmpTrainResult.m_dModelX = vectArray[0].GetX();
// 		m_tmpTrainResult.m_dModelY = vectArray[0].GetY();
		
		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_ImageMask      = m_tmpImageMask;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
		m_GuiTrainResultLine[2].SetLine(cLineSeg3.GetLine());
		m_GuiTrainResultLine[3].SetLine(cLineSeg4.GetLine());
		
		return true;

	}

}

// 获取训练结果图像														
bool CMQuickCornerSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CMQuickCornerSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	*((CMQuickCornerSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}	

// 是否训练成功
bool CMQuickCornerSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CMQuickCornerSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}

	//*((CMQuickCornerSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;
	m_tmpSearchDataParam = *((CMQuickCornerSearchDataParam*)pSearchDataParam);
	return true;
}

// 获取搜索Data参数				
bool CMQuickCornerSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}

	*((CMQuickCornerSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CMQuickCornerSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CMQuickCornerSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CMQuickCornerSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eMQuickCornerSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CMQuickCornerSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CMQuickCornerSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	scLineSeg modelLineSeg3; // 训练时的线段3
	scLineSeg modelLineSeg4; // 训练时的线段4
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));
	modelLineSeg3.SetP1(sc2Vector(m_TrainResult.m_dStartX[2], m_TrainResult.m_dStartY[2]));
	modelLineSeg3.SetP2(sc2Vector(m_TrainResult.m_dEndX[2], m_TrainResult.m_dEndY[2]));
	modelLineSeg4.SetP1(sc2Vector(m_TrainResult.m_dStartX[3], m_TrainResult.m_dStartY[3]));
	modelLineSeg4.SetP2(sc2Vector(m_TrainResult.m_dEndX[3], m_TrainResult.m_dEndY[3]));

	//m_QuickSearchTool.SetOverlapThreshold(0.1);
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

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber=-1;       //几何定位失败
		return false;
	}

	for (int i=0; i<LINES_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
			,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		m_FindLineTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bEnableSortByScore[i]);
//		m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i]);

		if (m_tmpTrainDataParam.m_bEnableDualEdge[i])
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

			m_FindLineTool[i].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[i], 
				m_tmpTrainDataParam.m_edgeProperty[i],
				tmpEdgePolarity, 
				m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		} 
		else
		{
			m_FindLineTool[i].SetCaliperProperty(
				m_tmpTrainDataParam.m_dContrastThre[i], 
				m_tmpTrainDataParam.m_edgeProperty[i], 
				m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		}

		m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
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


			// 执行找线
			scLineSeg resLineSeg[LINES_NUM];
			double dRMS[LINES_NUM] = { 0 };
			bool bFindLine[LINES_NUM] = { false };
			for (int j=0; j<LINES_NUM; j++)
			{
				// 		m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[j], m_TrainGuiParam.m_CaliperSearchRotation[j], m_TrainGuiParam.m_dCaliperSearchLen[j]
				// 			,m_TrainGuiParam.m_dCaliperProjectLen[j]);
				// 		m_FindLineTool[j].SetStartXYEndXY(m_TrainGuiParam.m_dStartX[j], m_TrainGuiParam.m_dStartY[j], m_TrainGuiParam.m_dEndX[j], m_TrainGuiParam.m_dEndY[j]);
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
				if(bIsProcessImage)
				{
					// 执行找线
					bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, resLineSeg[j], dRMS[j]); // 成功
				}
				else
				{
					// 执行找线
					bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, resLineSeg[j], dRMS[j]); // 成功
				}
				
			}
			if (!bFindLine[0])
			{
				m_nResultErrNumber=-2;       //找线1失败
			}
			if (!bFindLine[1])
			{
				m_nResultErrNumber=-3;       //找线2失败
			}
			if (!bFindLine[2])
			{
				m_nResultErrNumber=-4;       //找线3失败
			}
			if (!bFindLine[3])
			{
				m_nResultErrNumber=-5;       //找线4失败
			}

			// 求交点
			if (bFindLine[0] && bFindLine[1]&& bFindLine[2] &&bFindLine[3])
			{
				sc2Vector pointCenter[4];
				scLine resLine[4];
				resLine[0] = resLineSeg[0].GetLine();
				resLine[1] = resLineSeg[1].GetLine();
				resLine[2] = resLineSeg[2].GetLine();
				resLine[3] = resLineSeg[3].GetLine();
				//求四线中点
				{
					sc2Vector pointS;
					sc2Vector pointE;

					svStd vector<sc2Vector> pointResult;	// 探测点
					svStd vector<bool> used;				// 参与拟合标志
					for (int j=0; j<4; j++)
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
							pointCenter[j] = (pointS+pointE)/2;


						}
						else
						{
							return false;
						}
					}
				}
				if(m_tmpSearchDataParam.m_bEnable24CorLineCombine)
				{
					scLineSeg lineSeg(pointCenter[1],pointCenter[3]);
					resLine[1] = lineSeg.GetLine();
					resLine[3] = lineSeg.GetLine();
				}
				if(m_tmpSearchDataParam.m_bEnable13ProjectTo24Combine)
				{
					resLine[0].SetPos(pointCenter[0]);
					resLine[2].SetPos(pointCenter[2]);
					resLine[0].SetRotation(resLine[1].GetRotation() + scRadian(skPI_2));
					resLine[2].SetRotation(resLine[3].GetRotation() + scRadian(skPI_2));
				}
				sc2Vector vLineCrossPoint;
				bool bIntersect;
				bool bres;
				std::vector<sc2Vector> crossPointArray; 
				std::vector<sc2Vector> veccrossPoint; 
				//求四个交点
				bres = sfInterSectLineLine(resLine[0], resLine[1], bIntersect, crossPointArray);		
				if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				{
					m_nResultErrNumber=-6;       //线1线2求交点失败
					return false;
				}
				veccrossPoint.push_back(crossPointArray.at(0));

				//	 			bres = sfInterSectLineLine(resLineSeg[1].GetLine(), resLineSeg[2].GetLine(), bIntersect, crossPointArray);		
				//	 			if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				//	 			{
				//	 				return false;
				//	 			}
				//	 			veccrossPoint.push_back(crossPointArray.at(0));

				bres = sfInterSectLineLine(resLine[2], resLine[3], bIntersect, crossPointArray);		
				if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				{
					m_nResultErrNumber=-7;       //线3线4求交点失败
					return false;
				}
				veccrossPoint.push_back(crossPointArray.at(0));

				if (m_tmpSearchDataParam.m_bGetFourResultEnable || m_tmpSearchDataParam.m_bEnableOutput13MidCombine)
				{
					bres = sfInterSectLineLine(resLine[2], resLine[1], bIntersect, crossPointArray);		
					if (!bres|| !bIntersect || crossPointArray.size() <= 0)
					{
						m_nResultErrNumber=-8;       //线2线3求交点失败
						return false;
					}
					veccrossPoint.push_back(crossPointArray.at(0));

					bres = sfInterSectLineLine(resLine[0], resLine[1], bIntersect, crossPointArray);		
					if (!bres|| !bIntersect || crossPointArray.size() <= 0)
					{
						m_nResultErrNumber=-6;       //线1线2求交点失败
						return false;
					}
					veccrossPoint.push_back(crossPointArray.at(0));

					bres = sfInterSectLineLine(resLine[0], resLine[3], bIntersect, crossPointArray);		
					if (!bres|| !bIntersect || crossPointArray.size() <= 0)
					{
						m_nResultErrNumber=-9;       //线1线4求交点失败
						return false;
					}
					veccrossPoint.push_back(crossPointArray.at(0));
				}

				// 			bres = sfInterSectLineLine(resLineSeg[3].GetLine(), resLineSeg[0].GetLine(), bIntersect, crossPointArray);		
				// 			if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				// 			{
				// 				return false;
				// 			}
				// 			veccrossPoint.push_back(crossPointArray.at(0));
				//////////////////////////////////////////////////////////////////////////
				//对角连接线，交叉点
				// 			//交叉线，交叉点
				// 			scLineSeg cLineSeg02(veccrossPoint.at(0),veccrossPoint.at(2));
				// 			scLineSeg cLineSeg13(veccrossPoint.at(1),veccrossPoint.at(3));
				// 			
				// 			// 求中心交点
				// 			bres = sfInterSectLineLine(cLineSeg02.GetLine(), cLineSeg13.GetLine(), bIntersect, crossPointArray);
				// 			if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				// 			{
				// 				return false;
				// 			}
				//////////////////////////////////////////////////////////////////////////
				//中心点连线，交叉点
				// 		//交叉线，交叉点
				// 		std::vector<sc2Vector> vecMiddlePoint;
				// 		sc2Vector vMiddlePoint;
				// 		vMiddlePoint.SetX((veccrossPoint.at(0).GetX()+veccrossPoint.at(1).GetX())/2.0);
				// 		vMiddlePoint.SetY((veccrossPoint.at(0).GetY()+veccrossPoint.at(1).GetY())/2.0);
				// 		vecMiddlePoint.push_back(vMiddlePoint);
				// 
				// 		vMiddlePoint.SetX((veccrossPoint.at(1).GetX()+veccrossPoint.at(2).GetX())/2.0);
				// 		vMiddlePoint.SetY((veccrossPoint.at(1).GetY()+veccrossPoint.at(2).GetY())/2.0);
				// 		vecMiddlePoint.push_back(vMiddlePoint);
				// 
				// 		vMiddlePoint.SetX((veccrossPoint.at(2).GetX()+veccrossPoint.at(3).GetX())/2.0);
				// 		vMiddlePoint.SetY((veccrossPoint.at(2).GetY()+veccrossPoint.at(3).GetY())/2.0);
				// 		vecMiddlePoint.push_back(vMiddlePoint);
				// 
				// 		vMiddlePoint.SetX((veccrossPoint.at(3).GetX()+veccrossPoint.at(0).GetX())/2.0);
				// 		vMiddlePoint.SetY((veccrossPoint.at(3).GetY()+veccrossPoint.at(0).GetY())/2.0);
				// 		vecMiddlePoint.push_back(vMiddlePoint);
				// 
				// 		scLineSeg cLineSeg02(vecMiddlePoint.at(0),vecMiddlePoint.at(2));
				// 		scLineSeg cLineSeg13(vecMiddlePoint.at(1),vecMiddlePoint.at(3));
				// 		
				// 		// 求中心交点
				// 		bres = sfInterSectLineLine(cLineSeg02.GetLine(), cLineSeg13.GetLine(), bIntersect, crossPointArray);
				// 		if (!bres|| !bIntersect || crossPointArray.size() <= 0)
				// 		{
				// 			return false;
				// 		}
				//////////////////////////////////////////////////////////////////////////			

				{
					CSearchResult searchResult;


					searchResult.m_dPosX = veccrossPoint[0].GetX();
					searchResult.m_dPosY = veccrossPoint[0].GetY();

					searchResult.m_vdAuxiliaryPosX.at(0) = veccrossPoint[1].GetX();
					searchResult.m_vdAuxiliaryPosY.at(0) = veccrossPoint[1].GetY();

					if(m_tmpSearchDataParam.m_bEnableOutput13MidCombine)
					{
						//输出结果第一点变为线2/4与线1交点的中点
						searchResult.m_dPosX = 0.5 * (veccrossPoint[0].GetX() + veccrossPoint[4].GetX());
						searchResult.m_dPosY = 0.5 * (veccrossPoint[0].GetY() + veccrossPoint[4].GetY());
						//输出结果第二点变为线2/4与线3交点的中点
						searchResult.m_vdAuxiliaryPosX.at(0) = 0.5 * (veccrossPoint[1].GetX() + veccrossPoint[2].GetX());
						searchResult.m_vdAuxiliaryPosY.at(0) = 0.5 * (veccrossPoint[1].GetY() + veccrossPoint[2].GetY());
					}

					if (m_tmpSearchDataParam.m_bGetFourResultEnable)
					{
						searchResult.m_vdAuxiliaryPosX.at(1) = veccrossPoint[2].GetX();
						searchResult.m_vdAuxiliaryPosY.at(1) = veccrossPoint[2].GetY();

						searchResult.m_vdAuxiliaryPosX.at(2) = veccrossPoint[3].GetX();
						searchResult.m_vdAuxiliaryPosY.at(2) = veccrossPoint[3].GetY();

						searchResult.m_vdAuxiliaryPosX.at(3) = veccrossPoint[4].GetX();
						searchResult.m_vdAuxiliaryPosY.at(3) = veccrossPoint[4].GetY();
					}


					if (0)
					{
						// 角度计算，两条件的旋转量的平均
						scLineSeg modelLineSeg1; // 训练时的线段1
						scLineSeg modelLineSeg2; // 训练时的线段2
						scLineSeg modelLineSeg3; // 训练时的线段3
						scLineSeg modelLineSeg4; // 训练时的线段4

						modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
						modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
						modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
						modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));
						modelLineSeg3.SetP1(sc2Vector(m_TrainResult.m_dStartX[2], m_TrainResult.m_dStartY[2]));
						modelLineSeg3.SetP2(sc2Vector(m_TrainResult.m_dEndX[2], m_TrainResult.m_dEndY[2]));
						modelLineSeg4.SetP1(sc2Vector(m_TrainResult.m_dStartX[3], m_TrainResult.m_dStartY[3]));
						modelLineSeg4.SetP2(sc2Vector(m_TrainResult.m_dEndX[3], m_TrainResult.m_dEndY[3]));

						int nIdx[4] = {-1, -1,-1,-1 };	// [0]存放水平直线索引，[1]存放竖起直线索引
						double dTmpAngle1 = scDegree(modelLineSeg1.GetRotation()).NormMod180().ToDouble();
						if (fabs(dTmpAngle1-90) < 45)
						{
							nIdx[1] = 0;	// 找线工具1竖起方向搜索
						}
						else
						{
							nIdx[0] = 0;	// 找线工具1水平方向搜索
						}
						double dTmpAngle2 = scDegree(modelLineSeg2.GetRotation()).NormMod180().ToDouble();
						if (fabs(dTmpAngle2-90) < 45)
						{
							nIdx[1] = 1;	// 找线工具2竖起方向搜索
						}
						else
						{
							nIdx[0] = 1;	// 找线工具2水平方向搜索
						}

						double dTmpAngle3 = scDegree(modelLineSeg3.GetRotation()).NormMod180().ToDouble();
						if (fabs(dTmpAngle3-90) < 45)
						{
							nIdx[3] = 2;	// 找线工具3竖起方向搜索
						}
						else
						{
							nIdx[2] = 2;	// 找线工具3水平方向搜索
						}
						double dTmpAngle4 = scDegree(modelLineSeg4.GetRotation()).NormMod180().ToDouble();
						if (fabs(dTmpAngle4-90) < 45)
						{
							nIdx[3] = 3;	// 找线工具4竖起方向搜索
						}
						else
						{
							nIdx[2] = 3;	// 找线工具4水平方向搜索
						}


						if (!(((nIdx[0] == 0) && (nIdx[1] == 1)) || ((nIdx[0] == 1) && (nIdx[1] == 0))) )
						{
							return FALSE;
						}
						if (!(((nIdx[2] == 2) && (nIdx[3] == 3)) || ((nIdx[2] == 3) && (nIdx[3] == 2))) )
						{
							return FALSE;
						}
						// dRotH: 从X轴到水平找线工具结果的角度偏移量
						double dRotH = scDegree(scLine(sc2Vector(1, 0)).GetAngle(resLineSeg[nIdx[0]].GetLine())).NormMod180().ToDouble(); // 获取与X轴方向的夹角，并规范化到[0, 180)
						dRotH = (dRotH > 90) ? (dRotH - 180) : dRotH;	// 规范化到(-90, 90]
						// dRotV: 从Y轴到竖起找线工具结果的角度偏移量
						double dRotV = scDegree(scLine(sc2Vector(0, 1)).GetAngle(resLineSeg[nIdx[1]].GetLine())).NormMod180().ToDouble(); // 获取与Y轴方向的夹角，并规范化到[0, 180)
						dRotV = (dRotV > 90) ? (dRotV - 180) : dRotV;	// 规范化到(-90, 90]

						// dRotH: 从X轴到水平找线工具结果的角度偏移量
						double dRotH1 = scDegree(scLine(sc2Vector(1, 0)).GetAngle(resLineSeg[nIdx[2]].GetLine())).NormMod180().ToDouble(); // 获取与X轴方向的夹角，并规范化到[0, 180)
						dRotH1 = (dRotH1 > 90) ? (dRotH1 - 180) : dRotH1;	// 规范化到(-90, 90]
						// dRotV: 从Y轴到竖起找线工具结果的角度偏移量
						double dRotV1 = scDegree(scLine(sc2Vector(0, 1)).GetAngle(resLineSeg[nIdx[3]].GetLine())).NormMod180().ToDouble(); // 获取与Y轴方向的夹角，并规范化到[0, 180)
						dRotV1 = (dRotV1 > 90) ? (dRotV1 - 180) : dRotV1;	// 规范化到(-90, 90]
						// dRotAngle: 取dRotH和dRotV的平均
						double dRotAngle = (dRotH + dRotV + dRotH1 + dRotV1) / 4;		// 取两条线角度偏移量的平均值
						searchResult.m_dAngle = dRotAngle;
						searchResult.m_vdAuxiliaryAngle.at(0)= dRotation;

						// 相对位姿


						scRadian rdAngle1 = modelLineSeg1.GetLine().GetAngle(resLineSeg[0].GetLine());
						scRadian rdAngle2 = modelLineSeg2.GetLine().GetAngle(resLineSeg[1].GetLine());
						scRadian rdAngle3 = modelLineSeg3.GetLine().GetAngle(resLineSeg[2].GetLine());
						scRadian rdAngle4 = modelLineSeg4.GetLine().GetAngle(resLineSeg[3].GetLine());
						dRotAngle = scDegree((rdAngle1+rdAngle2+rdAngle3+rdAngle4)/4.0).ToDouble();

					}

					double dRotAngle = scDegree(scRadian(dRotation)).ToDouble();
					searchResult.m_dAngle = dRotAngle;
					searchResult.m_vdAuxiliaryAngle.at(0)= dRotAngle;

					CResultSearchRect resRect;
					resRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					// 设置找线结果
					CResultLine line;
					line.m_vLineX[0] = resLineSeg[0].GetP1().GetX();
					line.m_vLineY[0] = resLineSeg[0].GetP1().GetY();
					line.m_vLineX[1] = resLineSeg[0].GetP2().GetX();
					line.m_vLineY[1] = resLineSeg[0].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);

					line.m_vLineX[0] = resLineSeg[1].GetP1().GetX();
					line.m_vLineY[0] = resLineSeg[1].GetP1().GetY();
					line.m_vLineX[1] = resLineSeg[1].GetP2().GetX();
					line.m_vLineY[1] = resLineSeg[1].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);

					line.m_vLineX[0] = resLineSeg[2].GetP1().GetX();
					line.m_vLineY[0] = resLineSeg[2].GetP1().GetY();
					line.m_vLineX[1] = resLineSeg[2].GetP2().GetX();
					line.m_vLineY[1] = resLineSeg[2].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);

					line.m_vLineX[0] = resLineSeg[3].GetP1().GetX();
					line.m_vLineY[0] = resLineSeg[3].GetP1().GetY();
					line.m_vLineX[1] = resLineSeg[3].GetP2().GetX();
					line.m_vLineY[1] = resLineSeg[3].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);


					m_SearchResultArray.push_back(searchResult);

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
					for (int l=0; l<4; l++)
					{
						strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
						//scGuiLine *pTemLine = new scGuiLine;
						scGuiLine *pTemLine = m_GuiSearchLine+l+i*4;
						pTemLine->SetLine(resLine[l]);
						pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTemLine->SetTipTextEnabled(true);
						pTemLine->SetTipText(strTmpKey);
						m_pGuiSearchLineArray.push_back(pTemLine);
					}

					if (m_tmpSearchDataParam.m_bGetFourResultEnable)
					{

						int ii=0;
						strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
						//scGuiCross *pTempLineCross = new scGuiCross;
						scGuiCross *pTempCross = m_GuiSearchCross1+i;
						pTempCross->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii)), sc2Vector(20,20));
						pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross->SetTipTextEnabled(true);
						pTempCross->SetTipText(strTmpKey);

						CString st;
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii), searchResult.m_vdAuxiliaryAngle.at(ii),tmpQuickResult.GetScore());
						pTempCross->SetLabel(st);
						pTempCross->SetLabelVisible(TRUE);

						m_pGuiSearchLineCrossArray.push_back(pTempCross);
						ii=1;
						strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
						//scGuiCross *pTempCross2 = new scGuiCross;
						scGuiCross *pTempCross2 = m_GuiSearchCross2+i;
						pTempCross2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii)), sc2Vector(20,20));
						pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross2->SetTipTextEnabled(true);
						pTempCross2->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii), searchResult.m_vdAuxiliaryAngle.at(ii),tmpQuickResult.GetScore());
						pTempCross2->SetLabel(st);
						pTempCross2->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempCross2);

						ii=2;
						strTmpKey.Format(_T("%s_ResultCross3%d"), m_strKeyText, i);
						//scGuiCross *pTempCross2 = new scGuiCross;
						scGuiCross *pTempCross3 = m_GuiSearchCross3+i;
						pTempCross3->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii)), sc2Vector(20,20));
						pTempCross3->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross3->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross3->SetTipTextEnabled(true);
						pTempCross3->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii), searchResult.m_vdAuxiliaryAngle.at(ii),tmpQuickResult.GetScore());
						pTempCross3->SetLabel(st);
						pTempCross3->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempCross3);

						ii=3;
						strTmpKey.Format(_T("%s_ResultCross4%d"), m_strKeyText, i);
						//scGuiCross *pTempCross2 = new scGuiCross;
						scGuiCross *pTempCross4 = m_GuiSearchCross4+i;
						pTempCross4->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii)), sc2Vector(20,20));
						pTempCross4->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross4->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross4->SetTipTextEnabled(true);
						pTempCross4->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_vdAuxiliaryPosX.at(ii), searchResult.m_vdAuxiliaryPosY.at(ii), searchResult.m_vdAuxiliaryAngle.at(ii),tmpQuickResult.GetScore());
						pTempCross4->SetLabel(st);
						pTempCross4->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempCross4);
					}
					else
					{
						strTmpKey.Format(_T("%s_ResultCrossReal%d"), m_strKeyText, i);
						//scGuiCross *pTempLineCross = new scGuiCross;
						scGuiCross *pTempCross = m_GuiSearchCross1+i;
						pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), sc2Vector(20,20));
						pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross->SetTipTextEnabled(true);
						pTempCross->SetTipText(strTmpKey);

						CString st;
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle,tmpQuickResult.GetScore());
						pTempCross->SetLabel(st);
						pTempCross->SetLabelVisible(TRUE);

						m_pGuiSearchLineCrossArray.push_back(pTempCross);

						strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
						//scGuiCross *pTempCross2 = new scGuiCross;
						scGuiCross *pTempCross2 = m_GuiSearchCross2+i;
						pTempCross2->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0), searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
						pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempCross2->SetTipTextEnabled(true);
						pTempCross2->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, searchResult.m_vdAuxiliaryPosX.at(0), searchResult.m_vdAuxiliaryPosY.at(0), searchResult.m_vdAuxiliaryAngle.at(0),tmpQuickResult.GetScore());
						pTempCross2->SetLabel(st);
						pTempCross2->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempCross2);
					}



					// 找线0拟合点
					{
						CString	strTmpKey;
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
						CString	strTmpKey;
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



					// 找线2拟合点
					{
						CString	strTmpKey;
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindLineTool[2].GetUsed(bUsed);
						m_FindLineTool[2].GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							int nIndex = m_pGuiSearchResultFitPointArray_3.size();
							strTmpKey.Format(_T("%s_FitPoint1_2_%d"), m_strKeyText, nIndex);
							scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_3 + nIndex;
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
							m_pGuiSearchResultFitPointArray_3.push_back(pTempFitPoint1);	
						}
					}

					// 找线3拟合点
					{
						CString	strTmpKey;
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindLineTool[3].GetUsed(bUsed);
						m_FindLineTool[3].GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							int nIndex = m_pGuiSearchResultFitPointArray_4.size();
							strTmpKey.Format(_T("%s_FitPoint1_3_%d"), m_strKeyText, nIndex);
							scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultFitPoint_4 + nIndex;
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
							m_pGuiSearchResultFitPointArray_4.push_back(pTempFitPoint1);	
						}
					}



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
		m_nResultErrNumber=-1;       //几何定位失败
		return false;

	}



	return true;
}

// 获取搜索结果个数										
int CMQuickCornerSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CMQuickCornerSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像文件
bool CMQuickCornerSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eMQuickCornerSearchTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
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
bool CMQuickCornerSearchTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Pattern.xml
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
	CMQuickCornerSearchTrainDataParam tmpTrainDataParam;
	CMQuickCornerSearchTrainGuiParam tmpTrainGuiParam;
	CMQuickCornerSearchDataParam tmpSearchDataParam;
	CMQuickCornerSearchGuiParam tmpSearchGuiParam;
	CMQuickCornerSearchTrainResult    tmpTrainResult;
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
	if (cSearchToolType != eMQuickCornerSearchTool)
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
		return false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableSortByScore[0] = (BOOL)_ttol(cXMLConfigurator.GetElemData());
	}
	
	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());
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
		return false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableSortByScore[1] = (BOOL)_ttol(cXMLConfigurator.GetElemData());
	}

	// DualEdge2
	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// contrastthre3
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth3
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth3"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth[2] = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth[2] = _ttol(cXMLConfigurator.GetElemData());
	}
	
	// outlinenum3
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum[2] = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty3
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty[2] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// SortByScore3
	bRet = cXMLConfigurator.FindElem(_T("SortByScore3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableSortByScore[2] = (BOOL)_ttol(cXMLConfigurator.GetElemData());

	// DualEdge3
	bRet = cXMLConfigurator.FindElem(_T("DualEdge3"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[2] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[2] = _ttoi(cXMLConfigurator.GetElemData());


	// contrastthre4
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth4
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth4"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth[3] = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth[3] = _ttol(cXMLConfigurator.GetElemData());
	}
	
	// outlinenum4
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum[3] = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty4
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty[3] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// SortByScore4
	bRet = cXMLConfigurator.FindElem(_T("SortByScore4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableSortByScore[3] = (BOOL)_ttol(cXMLConfigurator.GetElemData());

	// DualEdge4
	bRet = cXMLConfigurator.FindElem(_T("DualEdge4"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[3] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[3] = _ttoi(cXMLConfigurator.GetElemData());

	cXMLConfigurator.LeaveElem();
	
	//////////////TrainGuiParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainGuiParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

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

	// model point2
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPoint2"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint2.GetX());
// 	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
// 	tmpTrainGuiParam.m_ModelPoint2 = vPos;
// 	cXMLConfigurator.LeaveElem();
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

	// Caliper1 Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[0]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
	}

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
		tmpTrainGuiParam.m_vdCaliperMaskData[0].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData[0].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}

			tmpTrainGuiParam.m_vdCaliperMaskData[0].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}


		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask[0].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask[0].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask[0].at(j) = _ttoi(cXMLConfigurator.GetElemData());
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

	// Caliper2 Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible2"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[1]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[1] = _ttoi(cXMLConfigurator.GetElemData());
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
		tmpTrainGuiParam.m_vdCaliperMaskData[1].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData[1].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vdCaliperMaskData[1].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum1"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask[1].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask[1].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask[1].at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}



	//
	// CaliperNum3
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum[2] = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartX[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartY[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndX[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndY[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen[2] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation3"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchRotation[2] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	// Caliper3 Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible3"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[2]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[2] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable2"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask[2] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask[2] = false;
	}

	bRet = cXMLConfigurator.FindElem(_T("MaskParam2"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum2"));
		if (!bRet) return false;		
		tmpTrainGuiParam.m_vdCaliperMaskData[2].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData[2].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vdCaliperMaskData[2].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum2"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask[2].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask[2].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask[2].at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}


	// CaliperNum4
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum[3] = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("StartX4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartX[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("StartY4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dStartY[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("EndX4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndX[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("EndY4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dEndY[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen[3] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("SearchRotation4"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchRotation[3] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	// Caliper4 Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible4"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[3]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[3] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable3"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask[3] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask[3] = false;
	}

	bRet = cXMLConfigurator.FindElem(_T("MaskParam3"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum3"));
		if (!bRet) return false;		
		tmpTrainGuiParam.m_vdCaliperMaskData[3].resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCaliperMaskData[3].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vdCaliperMaskData[3].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum3"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask[3].resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask[3].size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCaliperMask[3].at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}


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
	
	// ModelPoint2Visible
// 	bRet = cXMLConfigurator.FindElem(_T("ModelPoint2Visible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bModelPoint2Visible = false;
// 	}
// 	tmpTrainGuiParam.m_bModelPoint2Visible = _ttoi(cXMLConfigurator.GetElemData());
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

	bRet = cXMLConfigurator.FindElem(_T("GetFourResultEnable"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bGetFourResultEnable = false;
	}
	else
	{
		tmpSearchDataParam.m_bGetFourResultEnable = _ttoi(cXMLConfigurator.GetElemData());;
	}

	bRet = cXMLConfigurator.FindElem(_T("Enable24CorLineCombine"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bEnable24CorLineCombine = false;
	}
	else
	{
		tmpSearchDataParam.m_bEnable24CorLineCombine = _ttoi(cXMLConfigurator.GetElemData());;
	}

	bRet = cXMLConfigurator.FindElem(_T("Enable13ProjectTo24Combine"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bEnable13ProjectTo24Combine = false;
	}
	else
	{
		tmpSearchDataParam.m_bEnable13ProjectTo24Combine = _ttoi(cXMLConfigurator.GetElemData());;
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableOutput13MidCombine"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bEnableOutput13MidCombine = false;
	}
	else
	{
		tmpSearchDataParam.m_bEnableOutput13MidCombine = _ttoi(cXMLConfigurator.GetElemData());;
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
//	tmpSearchGuiParam.m_bLineCrossVisible = _ttoi(cXMLConfigurator.GetElemData());

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


	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible2"));
	if (bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint2 = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint2 = false;
	}

	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible3"));
	if (bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint3 = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint3 = false;
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

	// line 2
	bRet = cXMLConfigurator.FindElem(_T("StartX3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("StartY3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dStartX[2] = vPos.GetX();
	tmpTrainResult.m_dStartY[2] = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("EndY3"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dEndX[2] = vPos.GetX();
	tmpTrainResult.m_dEndY[2] = vPos.GetY();
	// line1
	bRet = cXMLConfigurator.FindElem(_T("StartX4"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("StartY4"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dStartX[3] = vPos.GetX();
	tmpTrainResult.m_dStartY[3] = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("EndX4"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("EndY4"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainResult.m_dEndX[3] = vPos.GetX();
	tmpTrainResult.m_dEndY[3] = vPos.GetY();

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

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	


	return true;
}
	
// 保存模板文件							
bool CMQuickCornerSearchTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Pattern.xml
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
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableSortByScore[0]);
	cXMLConfigurator.AddElem(_T("SortByScore1"), strInfo);
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[0]);
	cXMLConfigurator.AddElem(_T("DualEdge1"), strInfo);

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
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableSortByScore[1]);
	cXMLConfigurator.AddElem(_T("SortByScore2"), strInfo);
	// DualEdge2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[1]);
	cXMLConfigurator.AddElem(_T("DualEdge2"), strInfo);

	// contrastthre3
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[2]);
	cXMLConfigurator.AddElem(_T("ContrastThre3"), strInfo);
	// fileterhalfwidth3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[2]);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth3"), strInfo);
	// outlinenum3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[2]);
	cXMLConfigurator.AddElem(_T("OutLineNum3"), strInfo);
	// edgeproperty3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[2]);
	cXMLConfigurator.AddElem(_T("EdgeProperty3"), strInfo);
	// SortByScore3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableSortByScore[2]);
	cXMLConfigurator.AddElem(_T("SortByScore3"), strInfo);
	// DualEdge3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[2]);
	cXMLConfigurator.AddElem(_T("DualEdge3"), strInfo);

	// contrastthre4
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[3]);
	cXMLConfigurator.AddElem(_T("ContrastThre4"), strInfo);
	// fileterhalfwidth4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[3]);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth4"), strInfo);
	// outlinenum4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[3]);
	cXMLConfigurator.AddElem(_T("OutLineNum4"), strInfo);
	// edgeproperty4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[3]);
	cXMLConfigurator.AddElem(_T("EdgeProperty4"), strInfo);
	// SortByScore4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableSortByScore[3]);
	cXMLConfigurator.AddElem(_T("SortByScore4"), strInfo);
	// DualEdge4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[3]);
	cXMLConfigurator.AddElem(_T("DualEdge4"), strInfo);

	cXMLConfigurator.LeaveElem();

	//////////////TrainGuiParam//////////////
	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	cXMLConfigurator.EnterElem();
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
	// model point 2
// 	cXMLConfigurator.AddElem(_T("ModelPoint2"));
// 	cXMLConfigurator.EnterElem();
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
// 	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
// 	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
// 	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
// 	cXMLConfigurator.LeaveElem();
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

	//Caliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible[0]);
	cXMLConfigurator.AddElem(_T("CaliperVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[0]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable"), strInfo);

	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[0].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[0].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[0].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[0].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[0].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	// LineMask
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[1]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable1"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam1"));
	cXMLConfigurator.AddElem(_T("MaskParam1"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[1].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum1"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[1].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[1].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[1].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum1"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[1].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[1].at(j));
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

	//Caliper Visible2
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible[1]);
	cXMLConfigurator.AddElem(_T("CaliperVisible2"), strInfo);
	// LineMask
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[2]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable2"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam2"));
	cXMLConfigurator.AddElem(_T("MaskParam2"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[2].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum2"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[2].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[2].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[2].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum2"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[2].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[2].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	// Caliper Num3
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[2]);
	cXMLConfigurator.AddElem(_T("CaliperNum3"), strInfo);
	// startX3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX[2]);
	cXMLConfigurator.AddElem(_T("StartX3"), strInfo);
	// startY3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY[2]);
	cXMLConfigurator.AddElem(_T("StartY3"), strInfo);
	// EndX3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX[2]);
	cXMLConfigurator.AddElem(_T("EndX3"), strInfo);
	// EndY3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY[2]);
	cXMLConfigurator.AddElem(_T("EndY3"), strInfo);
	// Project Len3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[2]);
	cXMLConfigurator.AddElem(_T("ProjectLen3"), strInfo);
	// Search Len3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[2]);
	cXMLConfigurator.AddElem(_T("SearchLen3"), strInfo);
	// Search Rotation3
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation[2].ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation3"), strInfo);
	//Caliper Visible3
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible[2]);
	cXMLConfigurator.AddElem(_T("CaliperVisible3"), strInfo);

	// Caliper Num4 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[3]);
	cXMLConfigurator.AddElem(_T("CaliperNum4"), strInfo);
	// startX4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartX[3]);
	cXMLConfigurator.AddElem(_T("StartX4"), strInfo);
	// startY4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dStartY[3]);
	cXMLConfigurator.AddElem(_T("StartY4"), strInfo);
	// EndX4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndX[3]);
	cXMLConfigurator.AddElem(_T("EndX4"), strInfo);
	// EndY4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dEndY[3]);
	cXMLConfigurator.AddElem(_T("EndY4"), strInfo);
	// Project Len4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[3]);
	cXMLConfigurator.AddElem(_T("ProjectLen4"), strInfo);
	// Search Len4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[3]);
	cXMLConfigurator.AddElem(_T("SearchLen4"), strInfo);
	// Search Rotation4
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CaliperSearchRotation[3].ToDouble());
	cXMLConfigurator.AddElem(_T("SearchRotation4"), strInfo);
	//Caliper Visible4
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible[3]);
	cXMLConfigurator.AddElem(_T("CaliperVisible4"), strInfo);
	// LineMask
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask[3]);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable3"), strInfo);

	cXMLConfigurator.DeleteElem(_T("MaskParam3"));
	cXMLConfigurator.AddElem(_T("MaskParam3"));
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[3].size());
	cXMLConfigurator.AddElem(_T("MaskDataNum3"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[3].size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[3].at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[3].size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum3"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[3].size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[3].at(j));
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
	// width 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nLineWidth);
	cXMLConfigurator.AddElem(_T("LineWidth"), strInfo);
	// model point visible 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
	// search rect visible 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
	// affine rect visible
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
	// findcircle visible 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineVisible);
	cXMLConfigurator.AddElem(_T("LineVisible"), strInfo);
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

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bGetFourResultEnable);
	cXMLConfigurator.AddElem(_T("GetFourResultEnable"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnable24CorLineCombine);
	cXMLConfigurator.AddElem(_T("Enable24CorLineCombine"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnable13ProjectTo24Combine);
	cXMLConfigurator.AddElem(_T("Enable13ProjectTo24Combine"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableOutput13MidCombine);
	cXMLConfigurator.AddElem(_T("EnableOutput13MidCombine"), strInfo);

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

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint1);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint2);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible2"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint3);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible3"), strInfo);

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
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
	cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
	cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);
//	cXMLConfigurator.LeaveElem();
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

	// line2
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[2]);
	cXMLConfigurator.AddElem(_T("StartX3"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[2]);
	cXMLConfigurator.AddElem(_T("StartY3"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[2]);
	cXMLConfigurator.AddElem(_T("EndX3"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[2]);
	cXMLConfigurator.AddElem(_T("EndY3"), strInfo);
	// line 3
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[3]);
	cXMLConfigurator.AddElem(_T("StartX4"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[3]);
	cXMLConfigurator.AddElem(_T("StartY4"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[3]);
	cXMLConfigurator.AddElem(_T("EndX4"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[3]);
	cXMLConfigurator.AddElem(_T("EndY4"), strInfo);

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
	//bRet = m_QuickSearchTool.SaveTrainModelToFile(strDir + _T("Model.ptq"));
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}
	

	
	return true;
}	
	
// 更新训练控件数据信息	
bool CMQuickCornerSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
//		m_tmpTrainGuiParam.m_ModelPoint2        = m_GuiModelPoint2.GetCenter();	
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		for (int i=0; i<LINES_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);

			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			m_tmpTrainGuiParam.m_bCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();
		}
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
		for (int i=0; i<LINES_NUM; i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchRotation[i]);

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);//显示/隐藏卡尺，隐藏/显示矩形框
		}

	}

	return true;
}

bool CMQuickCornerSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
		


	}
	
	return true;
}

// 清除控件及其显示					
bool CMQuickCornerSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{	
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件				
bool CMQuickCornerSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CMQuickCornerSearchTool::ClearResult()
{
	

	ClearResultGui();

	m_SearchResultArray.clear();
	m_pGuiSearchAffineRectArray.clear();
	m_pGuiSearchCrossArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_1);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_2);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_3);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_4);


	return true;
}

bool CMQuickCornerSearchTool::SetGuiByCam(double nWidth,double nHeight)
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
	m_GuiFindLine[0].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[0].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine[1].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[1].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine[2].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[2].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
	m_GuiFindLine[3].GetStartXYEndXY(sx,sy,ex,ey);
	m_GuiFindLine[3].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);


	double plength,slength;
	plength = m_GuiFindLine[0].GetCaliperProjectionLength();
	slength = m_GuiFindLine[0].GetCaliperSearchLength();
	m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindLine[1].GetCaliperProjectionLength();
	slength = m_GuiFindLine[1].GetCaliperSearchLength();
	m_GuiFindLine[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindLine[2].GetCaliperProjectionLength();
	slength = m_GuiFindLine[2].GetCaliperSearchLength();
	m_GuiFindLine[2].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[2].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindLine[3].GetCaliperProjectionLength();
	slength = m_GuiFindLine[3].GetCaliperSearchLength();
	m_GuiFindLine[3].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[3].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CMQuickCornerSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem( m_strKeyText+_T("_ModelPoint"));
    	//m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		for (j=0; j<LINES_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}
		
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine3"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine4"));

	}
	
	if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FeatureChain"));
	}
	return true;
}

bool CMQuickCornerSearchTool::ClearResultGui()
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

		for (j=0;j<m_pGuiSearchLineArray.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		for (j=0;j<m_pGuiSearchLineCrossArray.size();j++)
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

		for (j=0; j<m_pGuiSearchResultFitPointArray_3.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_3[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPointArray_4.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPointArray_4[j]->GetTipText());
		}
	}
	
	return true;	
}

// 获取训练模板原始图像
bool CMQuickCornerSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CMQuickCornerSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CMQuickCornerSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

bool CMQuickCornerSearchTool::SetMaskImage( const cpImage &ImageMask )
{
	m_tmpImageMask = ImageMask;

	return true;
}

bool CMQuickCornerSearchTool::GetMaskImage( cpImage &ImageMask )
{
	ImageMask = m_tmpImageMask;

	return true;
}

// bool CMQuickCornerSearchTool::GetResultRelative( int nIndex, CSearchResult &searchResult )
// {
// 	if (nIndex < 0 || nIndex >= m_SearchResultArrayRelative.size())
// 	{
// 		return false;
// 	}
// 	
// 	searchResult = m_SearchResultArrayRelative[nIndex];
// 	
// 	return true;
// }

// CString CMQuickCornerSearchTool::GetCurPatDir() const
// {
// 	return m_strPatDirTmp;
// }

void CMQuickCornerSearchTool::SetLanguage(int nLanguage)
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

	int i=0;
	for (i=0; i<LINES_NUM; i++)
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
CString	CMQuickCornerSearchTool::GetResultErrInfo()
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
			case -2:							//找线1失败
				strErrInfo += _T("Find line1 failed");
				break;
			case -3:							//找线2失败
				strErrInfo += _T("Find line2 failed");
				break;										
			case -4:							//找线3失败
				strErrInfo += _T("Find line3 failed");
				break;
			case -5:							//找线4失败
				strErrInfo += _T("Find line4 failed");
				break;
			case -6:							//线1线2求交点失败
				strErrInfo += _T("Find the intersection of line1 and line2 failed");
				break;
			case -7:							//线3线4求交点失败
				strErrInfo += _T("Find the intersection of line3 and line4 failed");
				break;
			case -8:							//线2线3求交点失败
				strErrInfo += _T("Find the intersection of line2 and line3 failed");
				break;
			case -9:							//线1线4求交点失败
				strErrInfo += _T("Find the intersection of line1 and line4 failed");
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
			case -2:							//找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -3:							//找线2失败
				strErrInfo += _T("找线2失败");
				break;										
			case -4:							//找线3失败
				strErrInfo += _T("找线3失败");
				break;
			case -5:							//找线4失败
				strErrInfo += _T("找线4失败");
				break;
			case -6:							//线1线2求交点失败
				strErrInfo += _T("线1线2求交点失败");
				break;
			case -7:							//线3线4求交点失败
				strErrInfo += _T("线3线4求交点失败");
				break;
			case -8:							//线2线3求交点失败
				strErrInfo += _T("线2线3求交点失败");
				break;
			case -9:							//线1线4求交点失败
				strErrInfo += _T("线1线4求交点失败");
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
			case -2:							//找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -3:							//找线2失败
				strErrInfo += _T("找线2失败");
				break;										
			case -4:							//找线3失败
				strErrInfo += _T("找线3失败");
				break;
			case -5:							//找线4失败
				strErrInfo += _T("找线4失败");
				break;
			case -6:							//线1线2求交点失败
				strErrInfo += _T("线1线2求交点失败");
				break;
			case -7:							//线3线4求交点失败
				strErrInfo += _T("线3线4求交点失败");
				break;
			case -8:							//线2线3求交点失败
				strErrInfo += _T("线2线3求交点失败");
				break;
			case -9:							//线1线4求交点失败
				strErrInfo += _T("线1线4求交点失败");
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