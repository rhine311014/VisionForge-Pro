#include "stdafx.h"
#include "AreaInspectTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 
#include "svTimer.h"
#include "svBlob.h"
#include "svBlobSceneDescription.h"
#include "svHistogram.h"
//#include "PositionInspectTool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAreaInspectTool::CAreaInspectTool()
{
	m_strName								= _T("ACF检测工具");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabel(_T("ACF检测工具"));
	m_GuiSearchRect.SetLabelVisible(TRUE);

    m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
    m_GuiTrainRect.SetLabel(_T("训练区域"));
    m_GuiTrainRect.SetLabelVisible(TRUE);
    m_GuiBaseSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
    m_GuiBaseSearchRect.SetLabel(_T("搜索区域"));
    m_GuiBaseSearchRect.SetLabelVisible(TRUE);
    m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
    m_GuiModelPoint.SetLabel(_T("参考点"));
    m_GuiModelPoint.SetLabelVisible(TRUE);


	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	m_tmpTrainDataParam.m_dGrau				= m_AreaInspectTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre		= m_AreaInspectTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag			= m_AreaInspectTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像

    m_tmpTrainDataParam.m_bEnableBaseSearch		= false; 

	m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
    m_tmpTrainGuiParam.m_BaseSearchRect			= m_GuiBaseSearchRect.GetRect();
    m_tmpTrainGuiParam.m_TrainAffineRect			= m_GuiTrainRect.GetAffineRect();
    m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_AreaInspectTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;
	
	m_tmpSearchGuiParam.m_nLineWidth		= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor			= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;

	m_nMaxGreyVal = 0;
	m_dDefectArea = 0.0;
}

CAreaInspectTool::CAreaInspectTool(CAreaInspectTool &cAreaInspectTool)
{
	if (cAreaInspectTool.GetSearchToolType() != eAreaInspectTool)
	{
		return;
	}

	// 清空
// 	ClearAllGui();
// 	ClearResult();

	// 初始化
	m_strName								= _T("ACF检测工具");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabel(_T("ACF检测工具"));
	m_GuiSearchRect.SetLabelVisible(TRUE);

    m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
    m_GuiTrainRect.SetLabel(_T("训练区域"));
    m_GuiTrainRect.SetLabelVisible(TRUE);
    m_GuiBaseSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
    m_GuiBaseSearchRect.SetLabel(_T("搜索区域"));
    m_GuiBaseSearchRect.SetLabelVisible(TRUE);
    m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
    m_GuiModelPoint.SetLabel(_T("参考点"));
    m_GuiModelPoint.SetLabelVisible(TRUE);
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								= cAreaInspectTool.m_CamHeight;
	m_CamWidth								= cAreaInspectTool.m_CamWidth;
	m_strName								= cAreaInspectTool.m_strName;

	m_TrainDataParam						= cAreaInspectTool.m_TrainDataParam;
	m_TrainGuiParam							= cAreaInspectTool.m_TrainGuiParam;
	m_SearchDataParam						= cAreaInspectTool.m_SearchDataParam;
	m_SearchGuiParam						= cAreaInspectTool.m_SearchGuiParam;
	m_TrainResult                           = cAreaInspectTool.m_TrainResult;
	m_ImageTrain							= cAreaInspectTool.m_ImageTrain;
	m_ImageMask								= cAreaInspectTool.m_ImageMask;
	m_bTrainGuiShow							= cAreaInspectTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cAreaInspectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cAreaInspectTool.m_bTrainOK;
	m_pGuiDisplay							= cAreaInspectTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cAreaInspectTool.UpdateSearchGuiData(true);
	cAreaInspectTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cAreaInspectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cAreaInspectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cAreaInspectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cAreaInspectTool.m_tmpSearchGuiParam);
	SetTrainImage(cAreaInspectTool.m_tmpImageTrain);
	SetMaskImage(cAreaInspectTool.m_tmpImageMask);

    m_nMaxGreyVal = 0;
    m_dDefectArea = 0.0;

	if (m_bTrainOK && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		m_bTrainOK = false;
				
		// 设置训练参数
		if (!m_AreaInspectTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return;
		}
		
		if (!m_AreaInspectTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return;
		}
		
		if (!m_AreaInspectTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
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
			if(bIsProcessImage)
			{
				bRet = m_AreaInspectTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
            m_bTrainOK = bRet;
        }
        else
        {
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_AreaInspectTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
            m_bTrainOK = bRet;
        }
	}

	UpdateTrainGuiDisplay();
}

CAreaInspectTool& CAreaInspectTool::operator=(CAreaInspectTool& cAreaInspectTool)
{
	if (this == &cAreaInspectTool)
	{
		return *this;
	}

	if (cAreaInspectTool.GetSearchToolType() != eAreaInspectTool)
	{
		return *this;
	}
	
	// 清空
	ClearAllGui();
	ClearResult();
	
	// 初始化
	m_strName								= _T("ACF检测工具");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabel(_T("ACF检测工具"));
	m_GuiSearchRect.SetLabelVisible(TRUE);

    m_GuiTrainRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
    m_GuiTrainRect.SetLabel(_T("训练区域"));
    m_GuiTrainRect.SetLabelVisible(TRUE);
    m_GuiBaseSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
    m_GuiBaseSearchRect.SetLabel(_T("搜索区域"));
    m_GuiBaseSearchRect.SetLabelVisible(TRUE);
    m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
    m_GuiModelPoint.SetLabel(_T("参考点"));
    m_GuiModelPoint.SetLabelVisible(TRUE);
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识
	
	// 赋值
	m_CamHeight								= cAreaInspectTool.m_CamHeight;
	m_CamWidth								= cAreaInspectTool.m_CamWidth;
	m_strName								= cAreaInspectTool.m_strName;

	m_TrainDataParam						= cAreaInspectTool.m_TrainDataParam;
	m_TrainGuiParam							= cAreaInspectTool.m_TrainGuiParam;
	m_SearchDataParam						= cAreaInspectTool.m_SearchDataParam;
	m_SearchGuiParam						= cAreaInspectTool.m_SearchGuiParam;
	m_TrainResult							= cAreaInspectTool.m_TrainResult;
	m_ImageTrain							= cAreaInspectTool.m_ImageTrain;
	m_ImageMask								= cAreaInspectTool.m_ImageMask;
	m_bTrainGuiShow							= cAreaInspectTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cAreaInspectTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cAreaInspectTool.m_bTrainOK;
	m_pGuiDisplay							= cAreaInspectTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cAreaInspectTool.UpdateSearchGuiData(true);
	cAreaInspectTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cAreaInspectTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cAreaInspectTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cAreaInspectTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cAreaInspectTool.m_tmpSearchGuiParam);
	SetTrainImage(cAreaInspectTool.m_tmpImageTrain);
	SetMaskImage(cAreaInspectTool.m_tmpImageMask);
    m_nMaxGreyVal = 0;
    m_dDefectArea = 0.0;
	if (m_bTrainOK && m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		m_bTrainOK = false;
		
		// 设置训练参数
		if (!m_AreaInspectTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return *this;
		}
		
		if (!m_AreaInspectTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return *this;
		}
		
		if (!m_AreaInspectTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
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
				bRet = m_AreaInspectTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_AreaInspectTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
					m_TrainGuiParam.m_ModelPoint.GetY());
			}
			m_bTrainOK = bRet;
		}
	}

	return *this;
}

CAreaInspectTool::~CAreaInspectTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CAreaInspectTool::GetSearchToolType()
{
	return eAreaInspectTool;
}					

void CAreaInspectTool::SetSearchToolName(CString strName)
{
	m_strName = strName;
}

CString CAreaInspectTool::GetSearchToolName()
{
	return m_strName;
}

// 设置显示控件指针		
bool CAreaInspectTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

// 获取显示控件指针										
scGuiDisplay* CAreaInspectTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CAreaInspectTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CAreaInspectTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CAreaInspectTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CAreaInspectTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CAreaInspectTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CAreaInspectTool::UpdateTrainGuiDisplay()
{
    ClearTrainGui();
	m_GuiSearchRect.SetCenterCrossVisible(FALSE);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	if (m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
        m_GuiBaseSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bBaseSearchRectVisible);
        m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
        m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
        if (m_tmpTrainDataParam.m_bEnableBaseSearch)
        {
            m_pGuiInteractiveContainer->AddItem(&m_GuiBaseSearchRect, m_strKeyText+_T("_BaseSearchRect"));
            m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
            m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
        }
		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新搜索界面显示									
bool CAreaInspectTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

	m_GuiContour.SetVisible(m_bInspectFinish && m_bSearchResultGuiShow);
	
    if (m_tmpTrainDataParam.m_bEnableBaseSearch)
    {
        for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
        {
            m_pGuiSearchAffineRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
        }
        for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
        {
            m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
        }

    }

	for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
	{
		m_pGuiSearchResultRectArray[i]->SetVisible(m_bInspectFinish &&m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible);
	}
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiContour,"blob result");
		
		int i=0;

		for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultRectArray[i], m_pGuiSearchResultRectArray[i]->GetTipText());
		}
        if (m_tmpTrainDataParam.m_bEnableBaseSearch)
        {
            for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
            {
                m_pGuiStaticContainer->AddItem(m_pGuiSearchAffineRectArray[i], m_pGuiSearchAffineRectArray[i]->GetTipText());
            }
            for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
            {
                m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
            }
        }
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

bool CAreaInspectTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CAreaInspectTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CAreaInspectTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CAreaInspectTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}
	
	*((CAreaInspectTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CAreaInspectTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CAreaInspectTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CAreaInspectTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CAreaInspectTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CAreaInspectTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CAreaInspectTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_ImageTrain;

	return true;
}

// 设置训练原始图像									
bool CAreaInspectTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CAreaInspectTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CAreaInspectTool::Train()
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

    if (!m_tmpTrainDataParam.m_bEnableBaseSearch)
    {
        m_ImageTrain     = m_tmpImageTrain;
        m_ImageMask      = m_tmpImageMask;
        m_TrainDataParam = m_tmpTrainDataParam;
        m_TrainGuiParam  = m_tmpTrainGuiParam;
        m_TrainResult    = m_tmpTrainResult;
        m_bTrainOK = true;
        return true;
    }
    else
    {
        // 设置训练参数
        if (!m_AreaInspectTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            return false;
        }

        if (!m_AreaInspectTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            return false;
        }

        if (!m_AreaInspectTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            return false;
        }

        // 设置掩模图像
        if (m_tmpTrainDataParam.m_bMaskEnable)
        {
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_AreaInspectTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
					m_tmpTrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
					m_tmpTrainGuiParam.m_ModelPoint.GetY());
			}
            m_bTrainOK = bRet;
            if (m_bTrainOK)
            {
                sc2Vector cModelOrigin;
                m_AreaInspectTool.GetModelOrigin(cModelOrigin);
                m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
                m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

                m_ImageTrain     = m_tmpImageTrain;
                m_ImageMask      = m_tmpImageMask;
                m_TrainDataParam = m_tmpTrainDataParam;
                m_TrainGuiParam  = m_tmpTrainGuiParam;
                m_TrainResult    = m_tmpTrainResult;
            }
			else
			{
				m_nResultErrNumber=-1;       //几何定位失败
				return false;
			}
            return bRet;
        }
        else
        {
			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_AreaInspectTool.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
					m_tmpTrainGuiParam.m_ModelPoint.GetY());
			}
			else
			{
				bRet = m_AreaInspectTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
					m_tmpTrainGuiParam.m_ModelPoint.GetY());
			}

            m_bTrainOK = bRet;
            if (m_bTrainOK)
            {
                sc2Vector cModelOrigin;
                m_AreaInspectTool.GetModelOrigin(cModelOrigin);
                m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
                m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

                m_ImageTrain     = m_tmpImageTrain;
                m_TrainDataParam = m_tmpTrainDataParam;
                m_TrainGuiParam  = m_tmpTrainGuiParam;
                m_TrainResult    = m_tmpTrainResult;
            }
			else
			{
				m_nResultErrNumber=-1;       //几何定位失败
				return false;
			}

            return bRet;
        }

        return false;
    }

}

// 获取训练结果图像														
bool CAreaInspectTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_AreaInspectTool.GetTrainResultImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CAreaInspectTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}
	
	*((CAreaInspectTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}						

// 是否训练成功
bool CAreaInspectTool::IsTrained()
{
	return m_bTrainOK;
}															
			
// 设置搜索Data参数	
bool CAreaInspectTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CAreaInspectSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CAreaInspectTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}

	*((CAreaInspectSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CAreaInspectTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CAreaInspectSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CAreaInspectTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eAreaInspectTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CAreaInspectSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CAreaInspectTool::Search(cpImage &ImageSearch)
{

    if (!m_bTrainOK)
    {
		m_nResultErrNumber=-99;       //训练未成功
        m_bInspectFinish = false;
        m_bInspectOK = false;
        return false;
    }	

	// 清空
	ClearResult();

	// 位置修正
// 	double dOffsetX = 0, dOffsetY = 0;
// 	if ( (m_pParentTool != NULL) && (m_tmpTrainDataParam.m_strPositionToolName != _T("")) )
// 	{
// 		if (m_pParentTool->GetSearchToolType() == eACFInspectTool)
// 		{
// 			CACFInspectTool* pParentTool = (CACFInspectTool*)m_pParentTool;
// 			CPositionInspectTool* pPositionTool = (CPositionInspectTool*)(pParentTool->GetPositionTool(m_tmpTrainDataParam.m_strPositionToolName));
// 
// 			if (pPositionTool != NULL)
// 			{
// 				if ( (pPositionTool->m_bInspectFinish) && (pPositionTool->m_bInspectOK) )
// 				{
// 					CSearchResult searchResult;
// 					pPositionTool->GetResult(0, searchResult);
// 
// 					CPositionInspectTrainResult trainResult;
// 					pPositionTool->GetTrainResult((CBaseTrainResult*)(&trainResult));
// 
// 					dOffsetX = searchResult.m_dPosX - trainResult.m_dModelX;
// 					dOffsetY = searchResult.m_dPosY - trainResult.m_dModelY;
// 				}
// 				else
// 				{
// 					m_bInspectFinish = true;
// 					m_bInspectOK = false;
// 					return false;
// 				}
// 			}
// 		}
// 	}  

	std::vector<CResultSearchRect> vBlobRect;

	m_nResultErrNumber = 0;

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

	// Blob分析
	scBlobTool blobTool;
	blobTool.SetSegmentationHardThresh(m_tmpTrainDataParam.m_nBinaryThre,(m_tmpTrainDataParam.m_nDefectType == 2));
	blobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
	blobTool.ConnectivityCleanup(scBlobSceneDescription::eFill);
	blobTool.ConnectivityMinPels(m_tmpTrainDataParam.m_nMinAcceptArea);
	blobTool.EnableComputeInertia(true);
	blobTool.EnableComputeContour(true);

    if (m_TrainDataParam.m_bEnableBaseSearch)
    {
        // 设置搜索个数
        if (!m_AreaInspectTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            m_bInspectFinish = false;
            m_bInspectOK = false;
            return false;
        }

        // 设置接收阈值
        if (!m_AreaInspectTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            m_bInspectFinish = false;
            m_bInspectOK = false;
            return false;
        }

        // 设置搜索角度范围
        if (!m_AreaInspectTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            m_bInspectFinish = false;
            m_bInspectOK = false;
            return false;
        }

        // 设置缩放比例
        if (!m_AreaInspectTool.SetDofNominalSize(1.0))
        {
			m_nResultErrNumber=-1;       //几何定位失败
            m_bInspectFinish = false;
            m_bInspectOK = false;
            return false;
        }

        m_vbInspectOK.clear();
		bool bRet = false;
       if(bIsProcessImage)
	   {
		   // 执行搜索
		   bRet = m_AreaInspectTool.Execute(tmpImage, m_TrainGuiParam.m_BaseSearchRect);
	   }
	   else
	   {
		   // 执行搜索
		   bRet = m_AreaInspectTool.Execute(ImageSearch, m_TrainGuiParam.m_BaseSearchRect);
	   }
		
        if (bRet)
        {
            // 获取结果
            int nResultNum = m_AreaInspectTool.GetResultNum();
            if (nResultNum <= 0)
            {
                m_bInspectFinish = false;
                m_bInspectOK = false;
                return false;
            }

            for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
            {
                scQuickSearchResult tmpQuickResult = m_AreaInspectTool.GetResult(i);

                CString	strTmpKey;
                strTmpKey.Format(_T("%s_ResultBaseRect%d"), m_strKeyText, i);
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
                CString st;
                st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpQuickResult.GetPosition().GetX(), tmpQuickResult.GetPosition().GetY(), tmpQuickResult.GetRotate().ToDouble(),tmpQuickResult.GetScore());
                pTempCross->SetLabel(st);
                pTempCross->SetLabelVisible(TRUE);
                m_pGuiSearchCrossArray.push_back(pTempCross);

                // 求变换矩阵
                sc2Vector pos = tmpQuickResult.GetPosition();
                sc2Vector tmModelPoint;
                double dRotation;
                double dTransX;
                double dTransY;
                m_AreaInspectTool.GetModelOrigin(tmModelPoint);
                dRotation = scRadian(tmpQuickResult.GetRotate()).ToDouble();
                dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
                dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

                double dTransCenterX, dTransCenterY;
                double dOriCenterX, dOriCenterY;
                dOriCenterX = (m_TrainGuiParam.m_SearchRect.GetLeft()+m_TrainGuiParam.m_SearchRect.GetRight())/2;
                dOriCenterY = (m_TrainGuiParam.m_SearchRect.GetTop()+m_TrainGuiParam.m_SearchRect.GetBottom())/2;
                dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
                dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;

                scAffineRect affRect;
                affRect.SetCenter(sc2Vector(dTransCenterX,dTransCenterY));
                affRect.SetXLength(m_TrainGuiParam.m_SearchRect.GetWidth());
                affRect.SetYLength(m_TrainGuiParam.m_SearchRect.GetHeight());
                affRect.SetXRotation(scRadian(dRotation));
                affRect.SetSkew(scRadian(0));

               scRect rect = affRect.BoundingBox();
               rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));

               //////////////////////////////////////////////////////////////////////////
               // 特征计算
               HStatus		nHStatus;
               svStd vector<long> histogramResult(256, 0);

               nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
               if (nHStatus!=HSuccess)
               {
                   m_nMaxGreyVal =0.0;
               }
               else
               {
                   scHistoStats histoStats(histogramResult);
                   m_nMaxGreyVal = histoStats.HistMax();
               }
               //////////////////////////////////////////////////////////////////////////
               m_bInspectFinish = true;
               m_bInspectOK = true;
			   if(bIsProcessImage)
			   {
				  bRet =  blobTool.Execute(tmpImage,&rect);
			   }
			   else
			   {
				   bRet = blobTool.Execute(ImageSearch,&rect);
			   }
               if (!bRet)
               {
				   m_nResultErrNumber=-2;         //
                   m_bInspectFinish = false;
                   m_bInspectOK = false;
               }
               scBlobSceneDescription* pBSD = blobTool.GetBSD();
               if (pBSD == NULL)
               {
                   m_bInspectFinish = false;
                   m_bInspectOK = false;
               }	

               //计算Blob个数
               int nResultBlobNum = 0;
               if (m_bInspectFinish == true)
               {
                   pBSD->filter(eArea,m_tmpTrainDataParam.m_nMinAcceptArea,m_tmpTrainDataParam.m_nMaxAcceptArea,0);
                   scBlob * const *pBlobB = pBSD->allBlobs(false);

                   nResultBlobNum = pBSD->numBlobs(true); 
                   if (nResultBlobNum>=1)
                   {
                       //计算缺陷面积
                       pBSD->sort(eArea,eDescending);
                       const scBlob* pBlob = pBSD->getBlob(1);


                       m_dDefectArea = pBlob->area;			

                       if (m_dDefectArea>m_tmpTrainDataParam.m_dMaxDefectArea || m_dDefectArea<0.0)
                       {
						   m_nResultErrNumber=-3;  //超出缺陷面积
                           m_bInspectFinish = true;
                           m_bInspectOK = false;
                       }
                       else
                       {
                           m_bInspectFinish = true;
                           m_bInspectOK = true;
                       }			

                   }
				   else
				   {
					   m_nResultErrNumber=-2;  //没有满足有效面积的Blob
				   }

                   if (m_bInspectOK)
                   {
                       m_GuiContour.SetLineColor(RGB(0,255,0));
                   }
                   else
                   {
                       m_GuiContour.SetLineColor(RGB(255,0,0));

                   }	

				   for (int nBlob = 1; nBlob < nResultBlobNum+1; nBlob++)
				   {
					   const scBlob* pBlob = pBSD->getBlob(nBlob);
					   CResultSearchRect blobRect;
					   blobRect.m_bFindRect = m_bInspectOK;
					   blobRect.m_searchRect.SetCenterLengthsRotAndSkew(
						   sc2Vector(pBlob->boundPrincipalCenterX,pBlob->boundPrincipalCenterY), pBlob->boundPrincipalWidth, pBlob->boundPrincipalHeight,scRadian(scDegree(pBlob->angle)), scRadian(0));
					   vBlobRect.push_back(blobRect);
				   }

                   m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
                   m_GuiContour.SetTipTextEnabled(TRUE);

               }

               //if (m_bInspectFinish == false)
               //{		
               //    m_bInspectFinish = true;
               //    m_bInspectOK = true;
               //}

               CSearchResult searchResult;
               sc2Vector resultLeftPos;
               resultLeftPos = rect.GetUL();
               searchResult.m_dPosX = resultLeftPos.GetX();
               searchResult.m_dPosY = resultLeftPos.GetY();
               searchResult.m_vdAuxiliaryPosX.at(0) = rect.GetWidth();
               searchResult.m_vdAuxiliaryPosY.at(0) = rect.GetHeight();
               searchResult.m_resultAreaInspectTool.m_dLeftUpX = resultLeftPos.GetX();
               searchResult.m_resultAreaInspectTool.m_dLeftUpY = resultLeftPos.GetY();
               searchResult.m_resultAreaInspectTool.m_dWidth = rect.GetWidth();
               searchResult.m_resultAreaInspectTool.m_dHeight = rect.GetHeight();
               searchResult.m_resultAreaInspectTool.m_nResult = (int)m_bInspectOK;
			   searchResult.m_vACFSearchRect = vBlobRect;

			   // 检测矩形
			   CResultSearchRect resRect;
			   resRect.m_bFindRect = (int)m_bInspectOK;
			   resRect.m_searchRect = affRect;
			   searchResult.m_vACFSearchRect.push_back(resRect);

			   // 粗定位矩形
			   resRect.m_searchRect = tmpQuickResult.MatchRegion();
			   searchResult.m_vTrainSearchRect.push_back(resRect);

               m_SearchResultArray.push_back(searchResult);


               int j=0;
               //CString	strTmpKey;
               strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, j);
               scGuiRect *pTempRect = m_GuiSearchResultRect+j;
               pTempRect->SetRect(rect);
               pTempRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
               if (!m_bInspectOK)
               {
                   pTempRect->SetLineColor(RGB(255,0,0));
               }
               else
               {
                   pTempRect->SetLineColor(RGB(0,255,0));
               }
               pTempRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
               pTempRect->SetCenterCrossVisible(FALSE);
               pTempRect->SetTipTextEnabled(true);
               pTempRect->SetTipText((LPCTSTR)strTmpKey);
               pTempRect->SetLabel(GetSearchToolName());
               pTempRect->SetLabelVisible(TRUE);
               m_pGuiSearchResultRectArray.push_back(pTempRect);

               m_vbInspectOK.push_back(m_bInspectOK);
            }		


            if (m_SearchResultArray.size() <= 0)
            {
                m_bInspectFinish = false;
                m_bInspectOK = false;
                return false;
            }

            m_SearchDataParam = m_tmpSearchDataParam;
            m_SearchGuiParam  = m_tmpSearchGuiParam;
            
            BOOL bOK=true;
            for (int i=0;i<m_vbInspectOK.size();i++)
            {
                bOK = bOK && m_vbInspectOK.at(i);
            }
            return bOK;
        }
        else
        {
			m_nResultErrNumber=-1;       //几何定位失败
            m_bInspectFinish = false;
            m_bInspectOK = false;
            return false;
        }

    }

	scRect rect = m_tmpTrainGuiParam.m_SearchRect;
	//rect.SetOrigion(rect.GetOrigin()/* + sc2Vector(dOffsetX,dOffsetY)*/);
	//rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));
	if(bIsProcessImage)
	{
		rect = rect.Intersect(scRect(0,0,tmpImage.Width(),ImageSearch.Height()));
	}
	else
	{
		rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));
	}
//////////////////////////////////////////////////////////////////////////
	// 特征计算
	HStatus		nHStatus;
	svStd vector<long> histogramResult(256, 0);

	//nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
	if(bIsProcessImage)
	{
		nHStatus = sfHistogram(tmpImage,rect,histogramResult);
	}
	else
	{
		nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
	}

	if (nHStatus!=HSuccess)
	{
		m_nMaxGreyVal =0.0;
	}
	else
	{
		scHistoStats histoStats(histogramResult);
		m_nMaxGreyVal = histoStats.HistMax();
	}
//////////////////////////////////////////////////////////////////////////		
    m_bInspectFinish = true;
    m_bInspectOK = true;
	//if (!blobTool.Execute(ImageSearch,&rect))
	//{
	//	m_bInspectFinish = false;
	//	m_bInspectOK = false;
	//}
	if(bIsProcessImage)
	{
		if (!blobTool.Execute(tmpImage,&rect))
		{
			m_bInspectFinish = false;
			m_bInspectOK = false;
		}
	}
	else
	{
		if (!blobTool.Execute(ImageSearch,&rect))
		{
			m_bInspectFinish = false;
			m_bInspectOK = false;
		}
	}


	scBlobSceneDescription* pBSD = blobTool.GetBSD();
	if (pBSD == NULL)
	{
		m_bInspectFinish = false;
		m_bInspectOK = false;
	}	
	
	//计算Blob个数
	int nResultBlobNum = 0;
	if (m_bInspectFinish == true)
	{
		pBSD->filter(eArea,m_tmpTrainDataParam.m_nMinAcceptArea,m_tmpTrainDataParam.m_nMaxAcceptArea,0);
		scBlob * const *pBlobB = pBSD->allBlobs(false);

		nResultBlobNum = pBSD->numBlobs(true); 

		if (nResultBlobNum>=1)
		{

			//计算缺陷面积
			pBSD->sort(eArea,eDescending);
			const scBlob* pBlob = pBSD->getBlob(1);
			

			m_dDefectArea = pBlob->area;			

			if (m_dDefectArea>m_tmpTrainDataParam.m_dMaxDefectArea || m_dDefectArea<0.0)
			{
				m_nResultErrNumber=-3;  //超出缺陷面积
				m_bInspectFinish = true;
				m_bInspectOK = false;
			}
			else
			{
				//m_dDefectArea = 0;
				m_bInspectFinish = true;
				m_bInspectOK = true;

			}			
	

		}
		else
		{
			m_nResultErrNumber=-2;  //没有满足有效面积的Blob
		}

		if (m_bInspectOK)
		{
			m_GuiContour.SetLineColor(RGB(0,255,0));
		}
		else
		{
			m_GuiContour.SetLineColor(RGB(255,0,0));

		}			
		for (int nBlob = 1; nBlob < nResultBlobNum+1; nBlob++)
		{
			const scBlob* pBlob = pBSD->getBlob(nBlob);
			CResultSearchRect blobRect;
			blobRect.m_bFindRect = m_bInspectOK;
			blobRect.m_searchRect.SetCenterLengthsRotAndSkew(
				sc2Vector(pBlob->boundPrincipalCenterX,pBlob->boundPrincipalCenterY), pBlob->boundPrincipalWidth, pBlob->boundPrincipalHeight,scRadian(scDegree(pBlob->angle)), scRadian(0));
			vBlobRect.push_back(blobRect);
		}

		m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
		m_GuiContour.SetTipTextEnabled(TRUE);

// 		else if (nResultBlobNum==1)	
// 		{
// 			//计算缺陷面积
// 			pBSD->sort(eArea,eDescending);
// 			const scBlob* pBlob = pBSD->getBlob(1);
// 
// 			double dblobArea = pBlob->area;
// 			double rectArea = (double)(rect.GetWidth()*rect.GetHeight());
// 
// 			m_dDefectArea = rectArea - dblobArea;
// 
// 			if (m_dDefectArea>m_tmpTrainDataParam.m_dMaxDefectArea || m_dDefectArea<0.0)
// 			{
// 				m_bInspectFinish = true;
// 				m_bInspectOK = false;
// 			}
// 		}


	}

	//计算Hole个数
// 	int BlobCount = 0;
// 	int HoleCount = 0;
// 	if (m_bInspectFinish == false)
// 	{
// 		pBSD->clearFilters();
// 
// 		for(int i=1;i<=nResultBlobNum;i++)
// 		{
// 			const scBlob* pBlob = pBSD->getBlob(i);
// 			if (pBlob->label==1)
// 			{
// 				BlobCount += 1;
// 				HoleCount += pBlob->lChildNum;
// 			}
// 		}
// 
// 		if (BlobCount!=1 || HoleCount !=0)
// 		{
// 			m_bInspectFinish = true;
// 			m_bInspectOK = false;
// 		}
// 	}

	//最终结果判定
	//if (m_bInspectFinish == false)
	//{		
	//	m_bInspectFinish = true;
	//	m_bInspectOK = true;
	//}

	CSearchResult searchResult;
	sc2Vector resultLeftPos;
	resultLeftPos = rect.GetUL();
	searchResult.m_dPosX = resultLeftPos.GetX();
	searchResult.m_dPosY = resultLeftPos.GetY();
	searchResult.m_vdAuxiliaryPosX.at(0) = rect.GetWidth();
	searchResult.m_vdAuxiliaryPosY.at(0) = rect.GetHeight();
	searchResult.m_resultAreaInspectTool.m_dLeftUpX = resultLeftPos.GetX();
	searchResult.m_resultAreaInspectTool.m_dLeftUpY = resultLeftPos.GetY();
	searchResult.m_resultAreaInspectTool.m_dWidth = rect.GetWidth();
	searchResult.m_resultAreaInspectTool.m_dHeight = rect.GetHeight();
	searchResult.m_resultAreaInspectTool.m_nResult = (int)m_bInspectOK;
	searchResult.m_vACFSearchRect = vBlobRect;


	scAffineRect affRect;
	affRect.SetCornerPoLengthsRotAndSkew(rect.GetOrigin(), rect.GetWidth(), rect.GetHeight());
	CResultSearchRect insRect;
	insRect.m_bFindRect = m_bInspectOK;
	insRect.m_searchRect = affRect;
	searchResult.m_vACFSearchRect.push_back(insRect);

	m_SearchResultArray.push_back(searchResult);
	

	int i=0;
	CString	strTmpKey;
	strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
	scGuiRect *pTempAffineRect = m_GuiSearchResultRect+i;
	pTempAffineRect->SetRect(rect);
	pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
	if (!m_bInspectOK)
	{
		pTempAffineRect->SetLineColor(RGB(255,0,0));
	}
	else
	{
		pTempAffineRect->SetLineColor(RGB(0,255,0));
	}
	pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
	pTempAffineRect->SetCenterCrossVisible(FALSE);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	pTempAffineRect->SetLabel(GetSearchToolName());
	pTempAffineRect->SetLabelVisible(TRUE);
	m_pGuiSearchResultRectArray.push_back(pTempAffineRect);

	return m_bInspectOK;

}

bool CAreaInspectTool::SearchStatistic(cpImage &ImageSearch)
{
	// 清空
	ClearResult();

	// 位置修正
// 	double dOffsetX = 0, dOffsetY = 0;
// 	if ( (m_pParentTool != NULL) && (m_tmpTrainDataParam.m_strPositionToolName != _T("")) )
// 	{
// 		if (m_pParentTool->GetSearchToolType() == eACFInspectTool)
// 		{
// 			CACFInspectTool* pParentTool = (CACFInspectTool*)m_pParentTool;
// 			CPositionInspectTool* pPositionTool = (CPositionInspectTool*)(pParentTool->GetPositionTool(m_tmpTrainDataParam.m_strPositionToolName));
// 
// 			if (pPositionTool != NULL)
// 			{
// 				if ( (pPositionTool->m_bInspectFinish) && (pPositionTool->m_bInspectOK) )
// 				{
// 					CSearchResult searchResult;
// 					pPositionTool->GetResult(0, searchResult);
// 
// 					CPositionInspectTrainResult trainResult;
// 					pPositionTool->GetTrainResult((CBaseTrainResult*)(&trainResult));
// 
// 					dOffsetX = searchResult.m_dPosX - trainResult.m_dModelX;
// 					dOffsetY = searchResult.m_dPosY - trainResult.m_dModelY;
// 				}
// 				else
// 				{
// 					m_bInspectFinish = true;
// 					m_bInspectOK = false;
// 					return false;
// 				}
// 			}
// 		}
// 	}

	// Blob分析
	scBlobTool blobTool;
	blobTool.SetSegmentationHardThresh(m_tmpTrainDataParam.m_nBinaryThre,(m_tmpTrainDataParam.m_nDefectType == 1));
	blobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
	blobTool.ConnectivityCleanup(scBlobSceneDescription::eFill);
	blobTool.ConnectivityMinPels(10);

	scRect rect = m_tmpTrainGuiParam.m_SearchRect;
	rect.SetOrigion(rect.GetOrigin()/* + sc2Vector(dOffsetX,dOffsetY)*/);
	rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));
	if (!blobTool.Execute(ImageSearch,&rect))
	{
		m_bInspectFinish = true;
		m_bInspectOK = false;
	}
	scBlobSceneDescription* pBSD = blobTool.GetBSD();
	if (pBSD == NULL)
	{
		m_bInspectFinish = true;
		m_bInspectOK = false;
	}	

	// 数据统计
	if ( (pBSD->numBlobs() > 0) && (m_bInspectFinish  == false) )
	{
		pBSD->sort(eArea,eDescending);

		//Blob
		pBSD->clearFilters();
		pBSD->filter(eLabel,1,1,false);
		double dBlobArea = 0;
		if (pBSD->numBlobs() > 0)
		{
			const scBlob* pBlob = pBSD->getBlob(1);
			dBlobArea = pBlob->area;

		}

		//Hole
		double dHoleArea = 0;
		pBSD->clearFilters();
		pBSD->filter(eLabel,1,1,true);
		if (pBSD->numBlobs() > 0)
		{
			const scBlob* pBlob = pBSD->getBlob(1);
			dHoleArea = pBlob->area;
		}

// 		m_tmpTrainDataParam.m_lStastisticNumber = m_tmpTrainDataParam.m_lStastisticNumber + 1;
// 		m_tmpTrainDataParam.m_lStatisticHoleAreaMax = max(m_tmpTrainDataParam.m_lStatisticHoleAreaMax, dHoleArea);
// 		m_tmpTrainDataParam.m_lStatisticACFAreaMin = min(m_tmpTrainDataParam.m_lStatisticACFAreaMin, dBlobArea);
// 		m_tmpTrainDataParam.m_lStatisticACFAreaMax = max(m_tmpTrainDataParam.m_lStatisticACFAreaMax, dBlobArea);
	}

	//最终结果判定
	if (m_bInspectFinish == false)
	{
		m_bInspectFinish = true;
		m_bInspectOK = true;
	}

	int i=0;
	CString	strTmpKey;
	strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
	scGuiRect *pTempAffineRect = m_GuiSearchResultRect+i;
	pTempAffineRect->SetRect(rect);
	pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
	if (!m_bInspectOK)
	{
		pTempAffineRect->SetLineColor(RGB(255,0,0));
	}
	pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
	pTempAffineRect->SetCenterCrossVisible(FALSE);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	pTempAffineRect->SetLabel(GetSearchToolName());
	pTempAffineRect->SetLabelVisible(TRUE);
	m_pGuiSearchResultRectArray.push_back(pTempAffineRect);

	return true;
/*
	if (!m_bTrainOK)
	{
		return false;
	}	

	// 清空
	ClearResult();

	// 设置搜索个数
	if (!m_AreaInspectTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		return false;
	}

	// 设置接收阈值
	if (!m_AreaInspectTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		return false;
	}

	// 设置搜索角度范围
	if (!m_AreaInspectTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		return false;
	}

	// 设置缩放比例
	if (!m_AreaInspectTool.SetDofNominalSize(1.0))
	{
		return false;
	}

	// 执行搜索
	bool bRet = m_AreaInspectTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_AreaInspectTool.GetResultNum();
		if (nResultNum <= 0)
		{
			return false;
		}

		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scQuickSearchResult tmpQuickResult = m_AreaInspectTool.GetResult(i);
			
			CSearchResult searchResult;
			searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
			searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
			searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
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
			pTempCross->SetCenterExtent(tmpQuickResult.GetPosition());
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);

			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, tmpQuickResult.GetPosition().GetX(), tmpQuickResult.GetPosition().GetY(), tmpQuickResult.GetRotate().ToDouble());
			pTempCross->SetLabel(st);
			pTempCross->SetLabelVisible(TRUE);

			m_pGuiSearchCrossArray.push_back(pTempCross);
		}		

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}

	return bRet;
*/
}

// 获取搜索结果个数										
int CAreaInspectTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CAreaInspectTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CAreaInspectTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eAreaInspectTool)
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
	// train affine rect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (bRet)
	{
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
		rRect.SetOrigion(sc2Vector(vPos.GetX()-rRect.GetWidth()/2,vPos.GetY()-rRect.GetHeight()/2));
		cXMLConfigurator.LeaveElem();
	}
	else
	{
		vPos = sc2Vector(250,140);
		rRect.SetOrigion(vPos);
		rRect.SetWidth(100);
		rRect.SetHeight(100);	
	}

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
bool CAreaInspectTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CAreaInspectTrainDataParam tmpTrainDataParam;
	CAreaInspectTrainGuiParam tmpTrainGuiParam;
	CAreaInspectSearchDataParam tmpSearchDataParam;
	CAreaInspectSearchGuiParam tmpSearchGuiParam;
	CAreaInspectTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eAreaInspectTool)
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("ToolName"));
	if (!bRet)
	{
		return false;
	}
	m_strName = cXMLConfigurator.GetElemData();

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

	bRet = cXMLConfigurator.FindElem(_T("DefectType"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nDefectType = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("BinaryThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nBinaryThre = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MinAcceptArea"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nMinAcceptArea = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MaxAcceptArea"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nMaxAcceptArea = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MaxDefectArea"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dMaxDefectArea = _ttof(cXMLConfigurator.GetElemData());

    // grau
    bRet = cXMLConfigurator.FindElem(_T("Grau"));
    if (!bRet)
    {
        tmpTrainDataParam.m_dGrau = 2.0;
        //return false;
    }
    else
    {
        tmpTrainDataParam.m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
    }
    
    // noise thre
    bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
    if (!bRet)
    {
        tmpTrainDataParam.m_lNoiseThre = 10;
        //return false;
    }
    else
    {
        tmpTrainDataParam.m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
    }
    // total mag
    bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
    if (!bRet)
    {
        tmpTrainDataParam.m_lTotalMag = 500;
        //return false;
    }
    else
    {
        tmpTrainDataParam.m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
    }
    

    // mask enable
    bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
    if (!bRet)
    {
        tmpTrainDataParam.m_bMaskEnable = false;
       // return false;
    }
    else
    {
        tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
    }


    // basesearch enable
    bRet = cXMLConfigurator.FindElem(_T("BaseSearchEnable"));
    if (!bRet)
    {
        tmpTrainDataParam.m_bEnableBaseSearch = false;
        //return false;
    }
    else
    {
        tmpTrainDataParam.m_bEnableBaseSearch = _ttoi(cXMLConfigurator.GetElemData());
    }
    
// 	bRet = cXMLConfigurator.FindElem(_T("StatisticACFAreaMin"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lStatisticACFAreaMin = _ttol(cXMLConfigurator.GetElemData());
// 
// 	bRet = cXMLConfigurator.FindElem(_T("StatisticACFAreaMax"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lStatisticACFAreaMax = _ttol(cXMLConfigurator.GetElemData());
// 
// 	bRet = cXMLConfigurator.FindElem(_T("StatisticHoleAreaMax"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lStatisticHoleAreaMax = _ttol(cXMLConfigurator.GetElemData());
// 
// 	bRet = cXMLConfigurator.FindElem(_T("StastisticNumber"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_lStastisticNumber = _ttol(cXMLConfigurator.GetElemData());

// 	bRet = cXMLConfigurator.FindElem(_T("PositionToolName"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpTrainDataParam.m_strPositionToolName = cXMLConfigurator.GetElemData();

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

    // train affine rect
    bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
    if (bRet)
    {
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
    }
    else
    {
        vPos = sc2Vector(350,240);
        tmpTrainGuiParam.m_TrainAffineRect.SetCenter(vPos);
        tmpTrainGuiParam.m_TrainAffineRect.SetXLength(100);
        tmpTrainGuiParam.m_TrainAffineRect.SetYLength(100);
        tmpTrainGuiParam.m_TrainAffineRect.SetXRotation(scRadian(0));
        tmpTrainGuiParam.m_TrainAffineRect.SetSkew(scRadian(0));	
    }

    // model point
    bRet = cXMLConfigurator.FindElem(_T("ModelPoint"));
    if (bRet)
    {
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
    }
    else
    {
        vPos = sc2Vector(350,240);
        tmpTrainGuiParam.m_ModelPoint = vPos;
    }

    // search rect
    bRet = cXMLConfigurator.FindElem(_T("BaseSearchRect"));
    if (bRet)
    {
        cXMLConfigurator.EnterElem();
        strInfo.Format(_T("%f"), tmpTrainGuiParam.m_BaseSearchRect.GetOrigin().GetX());
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
        tmpTrainGuiParam.m_BaseSearchRect.SetOrigion(vPos);
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
        tmpTrainGuiParam.m_BaseSearchRect.SetSize(vPos);
        cXMLConfigurator.LeaveElem();
    }
    else
    {
        vPos = sc2Vector(350,240);
        tmpTrainGuiParam.m_BaseSearchRect.SetOrigion(vPos);
        vPos = sc2Vector(652,380);
        tmpTrainGuiParam.m_BaseSearchRect.SetSize(vPos);
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

	////////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpIImageModel;
	
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
	if (tmpTrainDataParam.m_bEnableBaseSearch)
	{
        cpImage tmpImageMask;
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
        scQuickSearchTool tmpAreaInspectTool;
        //bRet = tmpAreaInspectTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
        bRet = tmpAreaInspectTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
        if (!bRet)
        {
        	return false;
        }

        //数据赋值
        m_tmpImageTrain = tmpIImageModel;
        if (tmpTrainDataParam.m_bMaskEnable)
        {
        	m_tmpImageMask = tmpImageMask;
        }
        //m_AreaInspectTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
        m_AreaInspectTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
        m_ImageMask			= tmpImageMask;
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
	

	m_bTrainOK = true;
	
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CAreaInspectTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	cXMLConfigurator.AddElem(_T("ToolName"), GetSearchToolName());

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

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nDefectType);
	cXMLConfigurator.AddElem(_T("DefectType"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nBinaryThre);
	cXMLConfigurator.AddElem(_T("BinaryThre"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nMinAcceptArea);
	cXMLConfigurator.AddElem(_T("MinAcceptArea"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nMaxAcceptArea);
	cXMLConfigurator.AddElem(_T("MaxAcceptArea"), strInfo);

	strInfo.Format(_T("%f"), m_TrainDataParam.m_dMaxDefectArea);
	cXMLConfigurator.AddElem(_T("MaxDefectArea"), strInfo);

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
    // basesearch enable
    strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableBaseSearch);
    cXMLConfigurator.AddElem(_T("BaseSearchEnable"), strInfo);
// 	strInfo.Format(_T("%d"), m_tmpTrainDataParam.m_lStatisticACFAreaMin);
// 	cXMLConfigurator.AddElem(_T("StatisticACFAreaMin"), strInfo);
// 
// 	strInfo.Format(_T("%d"), m_tmpTrainDataParam.m_lStatisticACFAreaMax);
// 	cXMLConfigurator.AddElem(_T("StatisticACFAreaMax"), strInfo);
// 
// 	strInfo.Format(_T("%d"), m_tmpTrainDataParam.m_lStatisticHoleAreaMax);
// 	cXMLConfigurator.AddElem(_T("StatisticHoleAreaMax"), strInfo);
// 
// 	strInfo.Format(_T("%d"), m_tmpTrainDataParam.m_lStastisticNumber);
// 	cXMLConfigurator.AddElem(_T("StastisticNumber"), strInfo);
// 
// 	cXMLConfigurator.AddElem(_T("PositionToolName"), m_TrainDataParam.m_strPositionToolName);

	//// grau
	//strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau);
	//cXMLConfigurator.AddElem(_T("Grau"), strInfo);
	//// noise thre
	//strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre);
	//cXMLConfigurator.AddElem(_T("NoiseThre"), strInfo);
	//// total mag
	//strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag);
	//cXMLConfigurator.AddElem(_T("TotalMag"), strInfo);
	//// mask enable
	//strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	//cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();


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
    cXMLConfigurator.AddElem(_T("BaseSearchRect"));
    cXMLConfigurator.EnterElem();
    strInfo.Format(_T("%f"), m_TrainGuiParam.m_BaseSearchRect.GetOrigin().GetX());
    cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
    strInfo.Format(_T("%f"), m_TrainGuiParam.m_BaseSearchRect.GetOrigin().GetY());
    cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
    strInfo.Format(_T("%f"), m_TrainGuiParam.m_BaseSearchRect.GetSize().GetX());
    cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
    strInfo.Format(_T("%f"), m_TrainGuiParam.m_BaseSearchRect.GetSize().GetY());
    cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

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

	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	////////////////////////////存储模板图像//////////////////////////
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
	if (m_TrainDataParam.m_bEnableBaseSearch)
	{
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
        //bRet = m_AreaInspectTool.SaveTrainModelToFile(strDir + _T("Model.ptq"));
        bRet = m_AreaInspectTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
        if (!bRet)
        {
        	return false;
        }
	}
	
	return true;
}	
	
// 更新训练控件数据信息	
bool CAreaInspectTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
        m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
        m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
        m_tmpTrainGuiParam.m_BaseSearchRect			= m_GuiBaseSearchRect.GetRect();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

        m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
        m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
        m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

        m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
        m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
        m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

        m_GuiBaseSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
        m_GuiBaseSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
        m_GuiBaseSearchRect.SetRect(m_tmpTrainGuiParam.m_BaseSearchRect);
	}

	return true;
}

bool CAreaInspectTool::SaveTrainGuiData()
{
	return UpdateTrainGuiData(true);
}

bool CAreaInspectTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiSearchResultRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchResultRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
	}
	
	return true;
}

// 清除控件及其显示					
bool CAreaInspectTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CAreaInspectTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CAreaInspectTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultRectArray);
    ClearGraphicsArray(m_pGuiSearchAffineRectArray);
    ClearGraphicsArray(m_pGuiSearchCrossArray);

	m_nMaxGreyVal = 0;
	m_dDefectArea = 0;
	m_bInspectFinish	= false;
	m_bInspectOK		= false;

	return true;
}

bool CAreaInspectTool::SetGuiByCam(double nWidth,double nHeight)
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

    //搜索区域

    m_GuiBaseSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiBaseSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//m_GuiModelPoint.SetLabel(_T("参考点"));


	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
}

bool CAreaInspectTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
        m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_BaseSearchRect"));	
	}

	return true;
}

bool CAreaInspectTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultRectArray[j]->GetTipText());
		}
        for (j=0; j<m_pGuiSearchAffineRectArray.size(); j++)
        {
            m_pGuiStaticContainer->RemoveItem(m_pGuiSearchAffineRectArray[j]->GetTipText());
        }
        for (j=0; j<m_pGuiSearchCrossArray.size(); j++)
        {
            m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArray[j]->GetTipText());
        }

		m_pGuiStaticContainer->RemoveItem("blob result");
	}


	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultRectArray);
    ClearGraphicsArray(m_pGuiSearchAffineRectArray);
    ClearGraphicsArray(m_pGuiSearchCrossArray);

	return true;
}

// 获取训练模板原始图像
bool CAreaInspectTool::GetPatternImage(cpImage &ImagePattern)
{
	/*if (m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
	bool bRet = m_AreaInspectTool.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
	return false;
	}
	}
	else
	{
	ImagePattern = m_ImageTrain;
	}
	return true;*/

	if (m_tmpTrainDataParam.m_bEnableBaseSearch)
	{
		if (m_tmpImageTrain.IsValid() == false)
		{
			return false;
		}

		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_AreaInspectTool.GetTrainRectRegion()))
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
bool CAreaInspectTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}
											
// 恢复搜索参数		
bool CAreaInspectTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}


bool CAreaInspectTool::ApplyStatisticData()
{
// 	if (m_tmpTrainDataParam.m_lStastisticNumber < 3)
// 	{
// 		return false;
// 	}
// 
// 	m_tmpTrainDataParam.m_nMinAcceptArea = m_tmpTrainDataParam.m_lStatisticACFAreaMin;
// 	m_tmpTrainDataParam.m_nMaxAcceptArea = m_tmpTrainDataParam.m_lStatisticACFAreaMax;
// 	m_tmpTrainDataParam.m_dMaxDefectArea = m_tmpTrainDataParam.m_lStatisticHoleAreaMax;

	return true;
}

bool CAreaInspectTool::ResetStatisticData()
{
// 	m_tmpTrainDataParam.m_lStatisticACFAreaMin = LONG_MAX;
// 	m_tmpTrainDataParam.m_lStatisticACFAreaMax = 0;
// 	m_tmpTrainDataParam.m_lStatisticHoleAreaMax = 10;
// 	m_tmpTrainDataParam.m_lStastisticNumber = 0;

	return true;
}

void CAreaInspectTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Rect"));
		m_GuiSearchRect.SetLabel(_T("ACF Inspect"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("ACF检测工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("ACF检测工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}	
CString	CAreaInspectTool::GetResultErrInfo()
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
			case -2:							//没有满足有效面积的Blob
				strErrInfo += _T("No valid Blob");
				break;
			case -3:							//超出缺陷面积
				strErrInfo += _T("Exceed the defect area limit");
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
			case -2:							//几何定位失败
				strErrInfo += _T("没有满足有效面积的Blob");
				break;
			case -3:							//几何定位失败
				strErrInfo += _T("超出缺陷面积");
				break;
			case -99:							//训练未成功
				strErrInfo += _T("训练未成功");
				break;
			default:
				strErrInfo += _T("Ok");
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
			case -2:							//几何定位失败
				strErrInfo += _T("没有满足有效面积的Blob");
				break;
			case -3:							//几何定位失败
				strErrInfo += _T("超出缺陷面积");
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