// TwoQuickModelSearchTool.cpp: implementation of the TwoQuickModelSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TwoQuickModelSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"
#include "svLineSeg.h"
#include "svGuiLine.h"
#include "svIntersect.h"
#include "Shlwapi.h" 
#include "svTimer.h"
#include "svImageSharpness.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CTwoQuickModelSearchTool::CTwoQuickModelSearchTool()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(180, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterLengthsRotationSkew(530, 240, 100, 100, 0, 0);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);
	
	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_dGrau				= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_dGrau2				= m_QuickSearchTool2.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre2		= m_QuickSearchTool2.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag2			= m_QuickSearchTool2.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable2			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainAffineRect				= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_TrainAffineRect2				= m_GuiTrainRect2.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint2				= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect2				= m_GuiSearchRect2.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_QuickSearchTool.GetAcceptThreshold();

	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;
	m_tmpSearchDataParam.m_dSizeLow			= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh		= 1.0;

	m_tmpSearchDataParam.m_dAngleLow1		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh1		= 5.0;
	m_tmpSearchDataParam.m_dSizeLow1		= 1.0;
	m_tmpSearchDataParam.m_dSizeHigh1		= 1.0;

	
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CTwoQuickModelSearchTool::CTwoQuickModelSearchTool(CTwoQuickModelSearchTool &cTwoQuickModelSearchTool)
{
	if (cTwoQuickModelSearchTool.GetSearchToolType() != eTwoQuickModelSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(180, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterLengthsRotationSkew(530, 240, 100, 100, 0, 0);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);
	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cTwoQuickModelSearchTool.m_CamHeight;
	m_CamWidth								= cTwoQuickModelSearchTool.m_CamWidth;

	m_TrainDataParam						= cTwoQuickModelSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cTwoQuickModelSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cTwoQuickModelSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cTwoQuickModelSearchTool.m_SearchGuiParam;
	m_TrainResult							= cTwoQuickModelSearchTool.m_TrainResult;
	m_ImageTrain							= cTwoQuickModelSearchTool.m_ImageTrain;
	m_ImageMask								= cTwoQuickModelSearchTool.m_ImageMask;
	m_ImageMask2								= cTwoQuickModelSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= cTwoQuickModelSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cTwoQuickModelSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cTwoQuickModelSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cTwoQuickModelSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cTwoQuickModelSearchTool.UpdateSearchGuiData(true);
	cTwoQuickModelSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cTwoQuickModelSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cTwoQuickModelSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cTwoQuickModelSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cTwoQuickModelSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cTwoQuickModelSearchTool.m_tmpImageTrain);
	SetMaskImage(cTwoQuickModelSearchTool.m_tmpImageMask);
	SetMaskImage2(cTwoQuickModelSearchTool.m_tmpImageMask2);
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
		
		if (!m_QuickSearchTool2.SetGranularity(m_TrainDataParam.m_dGrau2))
		{
			return;
		}
		
		if (!m_QuickSearchTool2.SetNoiseThre(m_TrainDataParam.m_lNoiseThre2))
		{
			return;
		}
		
		if (!m_QuickSearchTool2.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag2))
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
		cpImage tmpImageMask2;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
			
			tmpImageMask2.CreateImageBuffer(m_ImageMask2.Width(),m_ImageMask2.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask2,tmpImageMask2);
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
		
		
		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable2)
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool2.Train(tmpImage, tmpImageMask2, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool2.Train(m_ImageTrain, m_ImageMask2, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool2.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool2.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			m_bTrainOK = bRet;
		}
		
	}
}

CTwoQuickModelSearchTool& CTwoQuickModelSearchTool::operator=(CTwoQuickModelSearchTool &cTwoQuickModelSearchTool)
{
	if (this == &cTwoQuickModelSearchTool)
	{
		return *this;	
	}

	if (cTwoQuickModelSearchTool.GetSearchToolType() != eTwoQuickModelSearchTool)
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
	m_GuiTrainRect.SetCenterLengthsRotationSkew(180, 240, 100, 100, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect2.SetCenterLengthsRotationSkew(530, 240, 100, 100, 0, 0);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	
	m_GuiTrainFeaturelet2.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet2.SetLineWidth(2);

	SetLanguage(m_iLanguage);

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cTwoQuickModelSearchTool.m_CamHeight;
	m_CamWidth								= cTwoQuickModelSearchTool.m_CamWidth;

	m_TrainDataParam						= cTwoQuickModelSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cTwoQuickModelSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cTwoQuickModelSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cTwoQuickModelSearchTool.m_SearchGuiParam;
	m_TrainResult							= cTwoQuickModelSearchTool.m_TrainResult;

	m_ImageTrain							= cTwoQuickModelSearchTool.m_ImageTrain;
	m_ImageMask								= cTwoQuickModelSearchTool.m_ImageMask;
	m_ImageMask2								= cTwoQuickModelSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= cTwoQuickModelSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cTwoQuickModelSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cTwoQuickModelSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cTwoQuickModelSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cTwoQuickModelSearchTool.UpdateSearchGuiData(true);
	cTwoQuickModelSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cTwoQuickModelSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cTwoQuickModelSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cTwoQuickModelSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cTwoQuickModelSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cTwoQuickModelSearchTool.m_tmpImageTrain);
	SetMaskImage(cTwoQuickModelSearchTool.m_tmpImageMask);
	SetMaskImage2(cTwoQuickModelSearchTool.m_tmpImageMask2);
	if (m_bTrainOK)
	{
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
		
		if (!m_QuickSearchTool2.SetGranularity(m_TrainDataParam.m_dGrau2))
		{
			return *this;
		}
		
		if (!m_QuickSearchTool2.SetNoiseThre(m_TrainDataParam.m_lNoiseThre2))
		{
			return *this;
		}
		
		if (!m_QuickSearchTool2.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag2))
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
		cpImage tmpImageMask2;
		if (bIsProcessImage)
		{
			tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageTrain,tmpImage);

			tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask,tmpImageMask);
			
			tmpImageMask2.CreateImageBuffer(m_ImageMask2.Width(),m_ImageMask2.Height(),epGray8);
			GetProcessImage(&m_tmpTrainDataParam,m_ImageMask2,tmpImageMask2);
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
		
		
		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable2)
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool2.Train(tmpImage, tmpImageMask2, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool2.Train(m_ImageTrain, m_ImageMask2, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if (bIsProcessImage)
			{
				bRet = m_QuickSearchTool2.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			else
			{
				bRet = m_QuickSearchTool2.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect2, m_TrainGuiParam.m_ModelPoint2.GetX(),
					m_TrainGuiParam.m_ModelPoint2.GetY());
			}
			m_bTrainOK = bRet;
		}
		
	}
	

	return *this;	
}



CTwoQuickModelSearchTool::~CTwoQuickModelSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CTwoQuickModelSearchTool::GetSearchToolType()
{
	return eTwoQuickModelSearchTool;
}										   

// 设置显示控件指针		
bool CTwoQuickModelSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CTwoQuickModelSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CTwoQuickModelSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}
// 获取训练结果显示控件指针
scGuiDisplay* CTwoQuickModelSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}


// 设置训练结果显示控件指针
bool CTwoQuickModelSearchTool::SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay)
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

// 获取训练结果显示控件指针
scGuiDisplay* CTwoQuickModelSearchTool::GetTrainResultDisplay2()
{
	return m_pGuiTrainResultDisplay2;
}

// 设置Gui图形的关键字														
bool CTwoQuickModelSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CTwoQuickModelSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CTwoQuickModelSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CTwoQuickModelSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CTwoQuickModelSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CTwoQuickModelSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
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
	
	scFeatureletChainSet tmpscFLCS2;
	bool bflag2=m_QuickSearchTool2.GetTrainFeatures(tmpscFLCS2);
	if(bflag2)
	{
		m_GuiTrainFeaturelet2.SetFeaturelet(tmpscFLCS2);
		m_GuiTrainFeaturelet2.SetVisible(m_bTrainOK);

		if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
		{
			m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
			m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet2,m_strKeyText+_T("_FeatureChain2"));
			
			m_pGuiTrainResultDisplay2->SetInteractiveContainer(m_pGuiInteractiveContainer);
			m_pGuiTrainResultDisplay2->MyInvalidate();
		}
	}
	return true;
}

// 更新搜索界面显示									
bool CTwoQuickModelSearchTool::UpdateSearchResultGuiDisplay()							
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

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新界面显示
bool CTwoQuickModelSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 设置训练Data参数	
bool CTwoQuickModelSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CTwoQuickModelSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CTwoQuickModelSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	*((CTwoQuickModelSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CTwoQuickModelSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CTwoQuickModelSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CTwoQuickModelSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CTwoQuickModelSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CTwoQuickModelSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CTwoQuickModelSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CTwoQuickModelSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CTwoQuickModelSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像									
bool CTwoQuickModelSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask2 = ImageMask;
	
	return true;
}

// 获取训练原始图像								
bool CTwoQuickModelSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask2;
	
	return true;
}

// 执行训练											
bool CTwoQuickModelSearchTool::Train()
{
	m_bTrainOK = false;
	m_nResultErrNumber=0;
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


	// 设置训练参数
	if (!m_QuickSearchTool2.SetGranularity(m_tmpTrainDataParam.m_dGrau2))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	if (!m_QuickSearchTool2.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre2))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	if (!m_QuickSearchTool2.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag2))
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
			bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint.GetY());	
		}
		else
		{
			bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask,m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());	
		}
	}
	else
	{
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
	}

	if (!bRet)
	{
		m_nResultErrNumber=-1; /// 区域1训练失败
		return bRet;
	}





	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable2)
	{
		if (bIsProcessImage)
		{
			bRet = m_QuickSearchTool2.Train(tmpImage, tmpImageMask2, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint2.GetY());	
		}
		else
		{
			bRet = m_QuickSearchTool2.Train(m_tmpImageTrain, m_tmpImageMask2,m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint2.GetY());	
		}
	}
	else
	{
		if (bIsProcessImage)
		{
			bRet = m_QuickSearchTool2.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint.GetY());

		}
		else
		{
			bRet = m_QuickSearchTool2.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect2, m_tmpTrainGuiParam.m_ModelPoint2.GetX(),
				m_tmpTrainGuiParam.m_ModelPoint2.GetY());

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
	m_QuickSearchTool.GetModelOrigin(cModelOrigin);
	m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
	m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();


	m_QuickSearchTool2.GetModelOrigin(cModelOrigin);
	m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
	m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();


	m_ImageTrain     = m_tmpImageTrain;
	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam  = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;

	return m_bTrainOK;
}

// 获取训练结果图像														
bool CTwoQuickModelSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CTwoQuickModelSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool2.GetTrainModelImage(m_ImageTrainResult2);
	if (!bRet)
	{
		return false;
	}
	
	ImageTrainResult = m_ImageTrainResult2;
	
	return true;
}	

// 获取训练结果
bool CTwoQuickModelSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	*((CTwoQuickModelSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}							

// 是否训练成功
bool CTwoQuickModelSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CTwoQuickModelSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CTwoQuickModelSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CTwoQuickModelSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}

	*((CTwoQuickModelSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CTwoQuickModelSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CTwoQuickModelSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CTwoQuickModelSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eTwoQuickModelSearchTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CTwoQuickModelSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CTwoQuickModelSearchTool::Search(cpImage &ImageSearch)
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

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow,m_tmpSearchDataParam.m_dSizeHigh,5))
	{
		m_nResultErrNumber = -1;
		return false;
	}



	// 设置搜索个数
	if (!m_QuickSearchTool2.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool2.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow1), scDegree(m_tmpSearchDataParam.m_dAngleHigh1), scDegree(360)))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool2.SetDofNominalSize(1.0))
	{
		m_nResultErrNumber = -2;
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool2.SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow1, m_tmpSearchDataParam.m_dSizeHigh1, 5))
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
		bRet = m_QuickSearchTool2.Execute(tmpImage, m_TrainGuiParam.m_SearchRect2);
	}
	else
	{
		bRet = m_QuickSearchTool2.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect2);
	}

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_QuickSearchTool2.GetResultNum();
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

	// 限制多个结果

	// 求两个参考点
	sc2Vector searchPoint1;
	sc2Vector searchPoint2;
	scAffineRect affRect1;

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_QuickSearchTool.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -1;
			return false;
		}

		for (int i=0; i<nResultNum && i<MAXRESNUM && i < m_QuickSearchTool2.GetResultNum(); i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool.GetResult(i);
			affRect1 = tmpQuickResult.MatchRegion();
			// 求变换矩阵
			sc2Vector pos = tmpQuickResult.GetPosition();
			searchPoint1 = pos;

			CString	strTmpKey;
			strTmpKey.Format(_T("工具1%s_ResultRect%d"), m_strKeyText, i);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
			pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);

			strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross+i*2;
			pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
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
		int nResultNum = m_QuickSearchTool2.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -2;
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM && i<m_QuickSearchTool.GetResultNum(); i++)
		{
			scQuickSearchResult tmpQuickResult = m_QuickSearchTool2.GetResult(i);
			sc2Vector pos = tmpQuickResult.GetPosition();
			searchPoint2 = pos;

			CSearchResult searchResult;
			searchResult.m_dPosX = (searchPoint1.GetX() + searchPoint2.GetX()) / 2.0;
			searchResult.m_dPosY = (searchPoint1.GetY() + searchPoint2.GetY()) / 2.0;
			scLineSeg lineSeg(searchPoint1, searchPoint2);
			searchResult.m_dAngle =scDegree(lineSeg.GetRotation().Norm()).ToDouble();


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

			resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
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
			pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
			
			strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross2+i*2;
			pTempCross->SetCenterExtent(tmpQuickResult.GetPosition(), sc2Vector(40,40));
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

			//scLineSeg lineSeg(searchPoint1, searchPoint2);
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
			st.Format(_T("%d X:%.1f;Y:%.1f(Dis:%.4fmm)"), i, searchResult.m_dPosX, searchResult.m_dPosY,dDistance12);
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

	bRet = bRet && bSizeCheck;

	return bRet;
}

// 获取搜索结果个数										
int CTwoQuickModelSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CTwoQuickModelSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
				
// 加载模板文件
bool CTwoQuickModelSearchTool::LoadModelFromFile(CString strDir)
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
	CTwoQuickModelSearchTrainDataParam tmpTrainDataParam;
	CTwoQuickModelSearchTrainGuiParam tmpTrainGuiParam;
	CTwoQuickModelSearchSearchDataParam tmpSearchDataParam;
	CTwoQuickModelSearchSearchGuiParam tmpSearchGuiParam;	
	CTwoQuickModelSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eTwoQuickModelSearchTool)
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
	//tmpTrainDataParam.LoadModelFromFile(cXMLConfigurator);
	bRet = cXMLConfigurator.FindElem(_T("ImageChannel"));				
	if (!bRet)
	{
		tmpTrainDataParam.m_nImageChannel = 0;
	}
	else
	{
		tmpTrainDataParam.m_nImageChannel = _ttol(cXMLConfigurator.GetElemData());
	}
	// grau
	bRet = cXMLConfigurator.FindElem(_T("Grau"));
	if (bRet)
	{
		tmpTrainDataParam.m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// noise thre
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
	if (bRet)
	{
		tmpTrainDataParam.m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
	}

	// total mag
	bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
	if (bRet)
	{
		tmpTrainDataParam.m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
	}
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (bRet)
	{
		tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
	}

	// grau2
	bRet = cXMLConfigurator.FindElem(_T("Grau2"));
	if (bRet)
	{
		tmpTrainDataParam.m_dGrau2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// noise thre2
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre2"));
	if (bRet)
	{
		tmpTrainDataParam.m_lNoiseThre2 = _ttol(cXMLConfigurator.GetElemData());
	}

	// total mag2
	bRet = cXMLConfigurator.FindElem(_T("TotalMag2"));
	if (bRet)
	{
		tmpTrainDataParam.m_lTotalMag2 = _ttol(cXMLConfigurator.GetElemData());
	}
	// mask enable2
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable2"));
	if (bRet)
	{
		tmpTrainDataParam.m_bMaskEnable2 = _ttoi(cXMLConfigurator.GetElemData());
	}
//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 
// 	//////////////TrainGuiParam//////////////


	// Train rect
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
	
	
	// Train rect2
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect2"));
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
	// model point1
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
	bRet = cXMLConfigurator.FindElem(_T("SearchRect2"));
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

	bRet = cXMLConfigurator.FindElem(_T("AcceptThre2"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAcceptThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	

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




	bRet = cXMLConfigurator.FindElem(_T("AngleLow1"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAngleLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh1"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAngleHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	bRet = cXMLConfigurator.FindElem(_T("SizeLow1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		tmpSearchDataParam.m_dSizeLow1 = 1.0;
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeHigh1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	else
	{
		tmpSearchDataParam.m_dSizeHigh1 = 1.0;
	}



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


	scQuickSearchTool tmpQuickSearchTool2;
	bRet = tmpQuickSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"));

	if (!bRet)
{
#ifdef VISIONPLUS_IMGPROC
		{
			if (tmpTrainDataParam.m_bMaskEnable2)
			{
				bRet = tmpQuickSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"), strDir + _T("Model.bmp"), strDir + _T("Mask2.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.ptq"));
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
				bRet = tmpQuickSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"), strDir + _T("Model.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.ptq"));
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
	if (tmpTrainDataParam.m_bMaskEnable2)
	{
		m_tmpImageMask2 = tmpImageMask2;
	}
	
	bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}
	
	bRet = m_QuickSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.ptq"));
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
	m_ImageMask2			= tmpImageMask2;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CTwoQuickModelSearchTool::SaveModelToFile(CString strDir)
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



	// grau2
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau2);
	cXMLConfigurator.AddElem(_T("Grau2"), strInfo);
	// noise thre2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre2);
	cXMLConfigurator.AddElem(_T("NoiseThre2"), strInfo);
	// total mag2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag2);
	cXMLConfigurator.AddElem(_T("TotalMag2"), strInfo);
	// mask enable2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable2);
	cXMLConfigurator.AddElem(_T("MaskEnable2"), strInfo);


//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();


	// train rect
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
	
	// train rect2
	cXMLConfigurator.AddElem(_T("TrainAffineRect2"));
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

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow1);
	cXMLConfigurator.AddElem(_T("AngleLow1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh1);
	cXMLConfigurator.AddElem(_T("AngleHigh1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeLow1);
	cXMLConfigurator.AddElem(_T("SizeLow1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeHigh1);
	cXMLConfigurator.AddElem(_T("SizeHigh1"), strInfo);

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
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}

	bRet = m_QuickSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.ptq"));
	if (!bRet)
	{
		return false;
	}
	
	return true;
}	
	
// 更新训练控件数据信息	
bool CTwoQuickModelSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

	m_tmpTrainGuiParam.m_TrainAffineRect2	= m_GuiTrainRect2.GetAffineRect();
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
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);
	
		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变
		m_GuiTrainRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect2);
		
		m_GuiSearchRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect2.SetRect(m_tmpTrainGuiParam.m_SearchRect2);
	}

	return true;
}

bool CTwoQuickModelSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
bool CTwoQuickModelSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CTwoQuickModelSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CTwoQuickModelSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	return true;
}

bool CTwoQuickModelSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen, rot,skew;

	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiTrainRect2.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainRect2.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

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

bool CTwoQuickModelSearchTool::ClearTrainGui()
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

bool CTwoQuickModelSearchTool::ClearResultGui()
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

	}

	return true;
}

// 获取训练模板原始图像
bool CTwoQuickModelSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CTwoQuickModelSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
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
bool CTwoQuickModelSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CTwoQuickModelSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CTwoQuickModelSearchTool::SetLanguage(int nLanguage)
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
CString CTwoQuickModelSearchTool::GetResultErrInfo()
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
				strErrInfo += _T("Quick search 1 failed");
				break;
			case -2:			//区域定位2失败				
				strErrInfo += _T("Quick search 2 failed");
				break;
			case -3:			//尺寸检查失败：超过最大值		
				strErrInfo += _T("Size Check Failed:Over Max");
				break;
			case -4:			//尺寸检查失败：低于最小值		
				strErrInfo += _T("Size Check Failed:Below Min");
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
				strErrInfo += _T("几何定位1失败");
				break;
			case -2:							
				strErrInfo += _T("几何定位2失败");
				break;
			case -3:			//尺寸检查失败：超过最大值		
				strErrInfo += _T("尺寸检查失败：超过最大值");
				break;
			case -4:			//尺寸检查失败：低于最小值		
				strErrInfo += _T("尺寸检查失败：低于最小值");
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
				strErrInfo += _T("几何定位1失败");
				break;
			case -2:							
				strErrInfo += _T("几何定位2失败");
				break;
			case -3:			//尺寸检查失败：超过最大值		
				strErrInfo += _T("尺寸检查失败：超过最大值");
				break;
			case -4:			//尺寸检查失败：低于最小值		
				strErrInfo += _T("尺寸检查失败：低于最小值");
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

// 加载模板训练图像
// 加载模板训练图像
bool CTwoQuickModelSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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

