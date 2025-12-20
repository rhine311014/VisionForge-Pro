
// PatternCornerSearchTool.cpp: implementation of the CPatternCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CircleCornerSearchCheckInspectTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"
#include "svBlob.h"
#include "svBlobSceneDescription.h"
#include "svHistogram.h"

void CCircleCornerCheckSearchTool::Initialize_Member_Variables()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_bBlobInspectFinsh							= false;
	m_strErrorDetail = _T("");
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area"));
		m_GuiFindCircle_1[0].SetLabel(_T("FindCircle1"));
		m_GuiFindCircle_1[1].SetLabel(_T("FindCircle2"));
		m_GuiFindObround.SetLabel(_T("FindObround"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));
		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		m_GuiFindObround.SetLabel(_T("找长圆工具"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));
		m_GuiFindCircle_1[0].SetLabel(_T("找圆工具1"));
		m_GuiFindCircle_1[1].SetLabel(_T("找圆工具2"));
		m_GuiFindObround.SetLabel(_T("找长圆工具"));
		break;
	}

	m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(240, 320, 240, 170);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
    for(int i =0;i<LINE_NUM;i++)
    {
        if (m_tmpTrainGuiParam.m_bEnableCornerLineMask[i])
        {
            m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
        }
        else
        {
            m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
        }
    }
	

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

	m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle_1[0].SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle_1[0].SetLabelVisible(TRUE);

	m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan(400, 300, 100, 0, sk2PI);
	m_GuiFindCircle_1[1].SetCaliperParams(20, eInward, 50, 20, true);
	m_GuiFindCircle_1[1].SetLabelVisible(TRUE);

	m_GuiFindObround.SetCenterLengthsRotation(sc2Vector(350,240), 200, 100, scRadian(0));
	m_GuiFindObround.SetCaliperParams(20, eInward, 50, 20);
	m_GuiFindObround.SetLabelVisible(TRUE);

	CreateGUID(m_strKeyText);

	m_GuiContour.SetTipText(m_strKeyText + _T("Blob Result Gui"));
	m_GuiContour.SetTipTextEnabled(TRUE);
}

CCircleCornerCheckSearchTool::CCircleCornerCheckSearchTool(void)
{
	
	Initialize_Member_Variables();

	//训练参数与GUI一致
	// 的粗定位训练参数
	m_tmpTrainDataParam.m_Accuracy = m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_dGrau	= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre = m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag = m_QuickSearchTool.GetTotalGradMagThre();


	//训练区GUI参数
	m_tmpTrainGuiParam.m_TrainRect = m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect	= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth	= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor = m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();

	
	for (int i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
			m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 20;
		m_tmpTrainGuiParam.m_nCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
			m_tmpTrainGuiParam.m_dEndY[i]);
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
		m_tmpTrainDataParam.m_bEnableDualEdge[i]		= false;
	}

	if(m_tmpTrainDataParam.m_nCircleToolType < 3)
	{
		for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
		{
			m_FindCircleTool_1[i].GetCaliperProperty(
				m_tmpTrainDataParam.m_dCircleContrastThre[i], 
				m_tmpTrainDataParam.m_edgeCircleProperty[i], 
				m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);

			m_tmpTrainDataParam.m_nCircleOutLineNum[i]			= 10;
			m_tmpTrainDataParam.m_bCircleSortByScore[i]			= false;
		}
	}else
	{
		m_FindObroundTool.GetCaliperProperty(m_tmpTrainDataParam.m_dObroundContrastThre, m_tmpTrainDataParam.m_edgeObroundProperty, 
			m_tmpTrainDataParam.m_lObroundFilterHalfWidth);
		m_tmpTrainDataParam.m_nObroundOutLineNum			= 10;
		m_tmpTrainDataParam.m_bObroundSortByScore			= false;
	}




	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= INI_ACCEPTTHRE;
	m_tmpSearchDataParam.m_dConfusionThre		= INI_CONTRASTTHRE;
	m_tmpSearchDataParam.m_dAngleLow			= -INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_dAngleHigh			= INI_SEARCH_ANGLE;

	m_tmpSearchDataParam.m_nSearchNum1			= 1;
	m_tmpSearchDataParam.m_dAcceptThre1			= INI_ACCEPTTHRE;
	m_tmpSearchDataParam.m_dConfusionThre1		= INI_CONTRASTTHRE;
	m_tmpSearchDataParam.m_dAngleLow1			= -INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_dAngleHigh1			= INI_SEARCH_ANGLE;
	m_tmpSearchDataParam.m_dSizeLow1			= 1;
	m_tmpSearchDataParam.m_dSizeHigh1			= 1;
	m_tmpSearchDataParam.m_bEnableDistCheck = true;

	m_tmpSearchGuiParam.m_nLineWidth	= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor	= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CCircleCornerCheckSearchTool::CCircleCornerCheckSearchTool(CCircleCornerCheckSearchTool &CCircleCornerCheckSearchTool)
{
	if (CCircleCornerCheckSearchTool.GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return;
	}

	Initialize_Member_Variables();


	// 赋值
    m_CamHeight								= CCircleCornerCheckSearchTool.m_CamHeight;
    m_CamWidth								= CCircleCornerCheckSearchTool.m_CamWidth;
	m_TrainDataParam						= CCircleCornerCheckSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CCircleCornerCheckSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CCircleCornerCheckSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CCircleCornerCheckSearchTool.m_SearchGuiParam;
	m_TrainResult							= CCircleCornerCheckSearchTool.m_TrainResult;
	m_ImageTrain							= CCircleCornerCheckSearchTool.m_ImageTrain;
	m_ImageMask								= CCircleCornerCheckSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CCircleCornerCheckSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CCircleCornerCheckSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CCircleCornerCheckSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CCircleCornerCheckSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CCircleCornerCheckSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	CCircleCornerCheckSearchTool.UpdateSearchGuiData(true);
	CCircleCornerCheckSearchTool.UpdateTrainGuiData(true);
    

	SetTrainDataParam(&CCircleCornerCheckSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CCircleCornerCheckSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CCircleCornerCheckSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CCircleCornerCheckSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CCircleCornerCheckSearchTool.m_tmpImageTrain);
	SetMaskImage(CCircleCornerCheckSearchTool.m_tmpImageMask);
	//SetMaskImage2(CCircleCornerCheckSearchTool.m_tmpImageMask1);


	if (m_bTrainOK)
	{
		bool bRet=false;
		m_bTrainOK = false;
		if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		{
			// 设置训练参数
			if (!m_PatternSearchTool.SetAccuracy(m_TrainDataParam.m_Accuracy))
			{
				return;
			}

			if (!m_PatternSearchTool.SetCoarsestScale(m_TrainDataParam.m_CoarseScale))
			{
				return;
			}

			// 必须在训练时设置角度
			if (fabs(m_TrainDataParam.m_dAngleLow - m_TrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
			{
				if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_TrainDataParam.m_dAngleLow)))
				{
					return;
				}
			}
			else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_TrainDataParam.m_dAngleLow), scDegree(m_TrainDataParam.m_dAngleHigh), scDegree(360)))
			{
				return;
			}

			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				bRet = m_PatternSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_ModelPoint,
					&m_TrainGuiParam.m_TrainRect);
				m_bTrainOK = bRet;
			}
			else
			{
				bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
				m_bTrainOK = bRet;			
			}
		}
		
		if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
		{
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
			if (1 != m_tmpImageTrain.PixelSize())
			{
				bIsProcessImage = TRUE;
			}

			cpImage tmpImage;
			cpImage tmpImageMask;
			if (bIsProcessImage)
			{
				tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
				GetProcessImage(&m_TrainDataParam,m_ImageTrain,tmpImage);

				tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
				GetProcessImage(&m_TrainDataParam,m_ImageMask,tmpImageMask);
			}

			//////////////////////////////////////////////////////////////////////////
			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
			//////////////////////////////////////////////////////////////////////////

		}
		
	}
}

CCircleCornerCheckSearchTool& CCircleCornerCheckSearchTool::operator=(CCircleCornerCheckSearchTool &CCircleCornerCheckSearchTool)
{
	if (this == &CCircleCornerCheckSearchTool)
	{
		return *this;	
	}

	if (CCircleCornerCheckSearchTool.GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	Initialize_Member_Variables();

	// 赋值
    m_CamHeight								= CCircleCornerCheckSearchTool.m_CamHeight;
    m_CamWidth								= CCircleCornerCheckSearchTool.m_CamWidth;
	m_TrainDataParam						= CCircleCornerCheckSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CCircleCornerCheckSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CCircleCornerCheckSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CCircleCornerCheckSearchTool.m_SearchGuiParam;
	m_TrainResult							= CCircleCornerCheckSearchTool.m_TrainResult;

	m_ImageTrain							= CCircleCornerCheckSearchTool.m_ImageTrain;
	m_ImageMask								= CCircleCornerCheckSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CCircleCornerCheckSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CCircleCornerCheckSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CCircleCornerCheckSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CCircleCornerCheckSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CCircleCornerCheckSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CCircleCornerCheckSearchTool.UpdateSearchGuiData(true);
	CCircleCornerCheckSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CCircleCornerCheckSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CCircleCornerCheckSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CCircleCornerCheckSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CCircleCornerCheckSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CCircleCornerCheckSearchTool.m_tmpImageTrain);
	SetMaskImage(CCircleCornerCheckSearchTool.m_tmpImageMask);

    
	if (m_bTrainOK)
	
		
		m_bTrainOK = false;

		if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		{
			// 设置训练参数
			if (!m_PatternSearchTool.SetAccuracy(m_TrainDataParam.m_Accuracy))
			{
				return *this;;
			}

			if (!m_PatternSearchTool.SetCoarsestScale(m_TrainDataParam.m_CoarseScale))
			{
				return *this;;
			}

			// 必须在训练时设置角度
			if (fabs(m_TrainDataParam.m_dAngleLow - m_TrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
			{
				if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_TrainDataParam.m_dAngleLow)))
				{
					return *this;;
				}
			}
			else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_TrainDataParam.m_dAngleLow), scDegree(m_TrainDataParam.m_dAngleHigh), scDegree(360)))
			{
				return *this;;
			}

			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				bool bRet = m_PatternSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_ModelPoint,
					&m_TrainGuiParam.m_TrainRect);
				m_bTrainOK = bRet;
			}
			else
			{
				bool bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
				m_bTrainOK = bRet;			
			}
		}

		if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
			{
				return *this;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
			{
				return *this;;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
			{
				return *this;;
			}

			bool bRet=false;
			BOOL bIsProcessImage = FALSE;
			if (1 != m_tmpImageTrain.PixelSize())
			{
				bIsProcessImage = TRUE;
			}

			cpImage tmpImage;
			cpImage tmpImageMask;
			if (bIsProcessImage)
			{
				tmpImage.CreateImageBuffer(m_ImageTrain.Width(),m_ImageTrain.Height(),epGray8);
				GetProcessImage(&m_TrainDataParam,m_ImageTrain,tmpImage);

				tmpImageMask.CreateImageBuffer(m_ImageMask.Width(),m_ImageMask.Height(),epGray8);
				GetProcessImage(&m_TrainDataParam,m_ImageMask,tmpImageMask);
			}		

			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
						m_TrainGuiParam.m_ModelPoint.GetY());
				}
				m_bTrainOK = bRet;
			}
		}

	

	return *this;	
}

CCircleCornerCheckSearchTool::~CCircleCornerCheckSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CCircleCornerCheckSearchTool::GetSearchToolType()
{
	return eCircleCornerSearchCheckInspectTool;
}

// 设置显示控件指针
bool CCircleCornerCheckSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

// 获取显示控件指针										
scGuiDisplay* CCircleCornerCheckSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

bool CCircleCornerCheckSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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

scGuiDisplay* CCircleCornerCheckSearchTool::GetTrainResultDisplay()
{
   return m_pGuiTrainResultDisplay;
}

// 设置Gui图形的关键字														
bool CCircleCornerCheckSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CCircleCornerCheckSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CCircleCornerCheckSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CCircleCornerCheckSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CCircleCornerCheckSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CCircleCornerCheckSearchTool::UpdateTrainGuiDisplay()
{	
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
		m_GuiTrainAffineRect.SetVisible(false);
	}

	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		m_GuiTrainRect.SetVisible(false);
		m_GuiTrainAffineRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);


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

	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);


	for (int i=0; i<LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);
        if (m_tmpTrainGuiParam.m_bEnableCornerLineMask[i])
        {
            m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
        }
        else
        {
            m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
        }
	}

	if (m_tmpTrainDataParam.m_nCircleToolType < 3)
	{
		for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
		{
			m_GuiFindCircle_1[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCircleVisible_1[i]);
		}
	}else
	{
		m_GuiFindObround.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bObroundVisible);
	}


	

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect, m_strKeyText+_T("_TrainAffRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	

		//显示直线的GUI
		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

		//显示圆的GUI
		CString strTemp = _T("");
		if(m_tmpTrainDataParam.m_nCircleToolType < 3)//圆
		{
			for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
			{
				strTemp.Format(_T("_FindCircle%d"), i);
				m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle_1[i], m_strKeyText + strTemp);
			}
		}else//长圆
		{
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindObround, m_strKeyText+_T("_FindObround"));
		}


		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}





	return true;
}

bool CCircleCornerCheckSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);

	if(m_tmpTrainDataParam.m_nCircleToolType < 3)//圆
	{
		for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
		{
			m_GuiTrainResultCircle_1[i].SetVisible(m_bTrainResultGuiShow);
		}
	}else if(m_tmpTrainDataParam.m_nCircleToolType)//长圆
	{
		m_GuiTrainResultObround.SetVisible(m_bTrainResultGuiShow);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));

		CString strTemp;
		if(m_tmpTrainDataParam.m_nCircleToolType < 3)		//圆
		{
			for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
			{
				strTemp.Format(_T("_TrainResultCircle%d"), i);
				m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle_1[i], m_strKeyText + strTemp);
			}
		}else//长圆
		{
			m_pGuiStaticContainer->AddItem(&m_GuiTrainResultObround, m_strKeyText+_T("_TrainResultObround"));
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CCircleCornerCheckSearchTool::UpdateSearchResultGuiDisplay()							
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

	for (i=0; i<m_pGuiSearchLineSegArray.size(); i++)
	{
		m_pGuiSearchLineSegArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
	{
		m_pGuiSearchLineTextArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible_1[i]);
	}

	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible_1[i]);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint2Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bVisibleFitPoint2);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bVisibleFitPoint1);
	}

	for (i=0; i<m_pGuiSearchResultCircleFitPointArray1.size(); i++)
	{
		m_pGuiSearchResultCircleFitPointArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint[0]);
	}

	for (i=0; i<m_pGuiSearchResultCircleFitPointArray2.size(); i++)
	{
		m_pGuiSearchResultCircleFitPointArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint[1]);
	}

	for (i=0; i<m_pGuiSearchObroundArray.size(); i++)
	{
		m_pGuiSearchObroundArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundVisible);
	}
	for (i=0; i<m_pGuiSearchObroundCenterArray.size(); i++)
	{
		m_pGuiSearchObroundCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundCenterVisible);
	}


	m_GuiContour.SetVisible(m_bBlobInspectFinsh && m_bSearchResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiContour,m_GuiContour.GetTipText());

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

			for (i=0; i<m_pGuiSearchLineSegArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineSegArray[i], m_pGuiSearchLineSegArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineTextArray[i], m_pGuiSearchLineTextArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
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

			for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint2Array[i], m_pGuiSearchResultFitPoint2Array[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultCircleFitPointArray1.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultCircleFitPointArray1[i], m_pGuiSearchResultCircleFitPointArray1[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultCircleFitPointArray2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultCircleFitPointArray2[i], m_pGuiSearchResultCircleFitPointArray2[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchObroundArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundArray[i], m_pGuiSearchObroundArray[i]->GetTipText());
			}for (i=0; i<m_pGuiSearchObroundCenterArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundCenterArray[i], m_pGuiSearchObroundCenterArray[i]->GetTipText());
			}
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CCircleCornerCheckSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CCircleCornerCheckSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CCircleCornerCheckSearchTrainDataParam*)pTrainDataParam);

	return true;
}

bool CCircleCornerCheckSearchTool::SetTrainDataParamForce(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_TrainDataParam = *((CCircleCornerCheckSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CCircleCornerCheckSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	*((CCircleCornerCheckSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CCircleCornerCheckSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CCircleCornerCheckSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CCircleCornerCheckSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CCircleCornerCheckSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CCircleCornerCheckSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CCircleCornerCheckSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CCircleCornerCheckSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}
bool CCircleCornerCheckSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	//m_tmpImageMask1 = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CCircleCornerCheckSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

bool CCircleCornerCheckSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	//ImageMask = m_tmpImageMask1;

	return true;
}

// 执行训练											
bool CCircleCornerCheckSearchTool::Train()
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
	cpImage tmpImageMask1;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask.CreateImageBuffer(m_tmpImageMask.Width(),m_tmpImageMask.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask,tmpImageMask);

	}

	// 设置训练参数
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
		{
			return false;
		}

		if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
		{
			return false;
		}

		// 必须在训练时设置角度
		if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
		{
			if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
			{
				return false;
			}
		}
		else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
		{
			return false;
		}

		// 设置掩模图像
		if (m_tmpTrainDataParam.m_bMaskEnable)
		{

			bool bRet = false;
			if(bIsProcessImage)
			{
				bRet = m_PatternSearchTool.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
			}
			else
			{
				bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
			}


			if (!bRet)
			{
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			if(!GetTrainLineCircleSearchResult())//训练时找线和找圆的结果
				return false;


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
			if(bIsProcessImage)
			{
				bRet = m_PatternSearchTool.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
			}
			else
			{
				bRet = m_PatternSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint,
					&m_tmpTrainGuiParam.m_TrainRect);
			}

			if (!bRet)
			{
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			if(!GetTrainLineCircleSearchResult())//训练时找线和找圆的结果
				return false;

			m_bTrainOK       = true;
			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam	 = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;

			return true;
		}

	}


	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
		{
			return false;
		}

		if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
		{
			return false;
		}

		if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
		{
			return false;
		}

		// 设置掩模图像
		if (m_tmpTrainDataParam.m_bMaskEnable)
		{

			bool bRet = false;
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


			if (!bRet)
			{
				return false;
			}
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			if(!GetTrainLineCircleSearchResult())//训练时找线和找圆的结果
				return false;

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
				return false;
			}
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			if(!GetTrainLineCircleSearchResult())//训练时找线和找圆的结果
				return false;

			m_bTrainOK       = true;
			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam	 = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;

			return true;
		}
	}
	
	return false;
}

//训练时找线和找圆返回的结果
bool CCircleCornerCheckSearchTool::GetTrainLineCircleSearchResult()
{
	// 找线
	bool bRet = false; 
	scLineSeg cLineSeg[LINE_NUM];
	double dRMS[LINE_NUM];

	BOOL bIsProcessImage = FALSE;
	if (1 != m_tmpImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}
	cpImage tmpImage;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);
	}
	for( int i =0 ; i < LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[i], m_tmpTrainGuiParam.m_CaliperSearchRotation[i], m_tmpTrainGuiParam.m_dCaliperSearchLen[i]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);

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
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		} 
		else
		{
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		}
		m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[i]);
		m_FindLineTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], 
			m_tmpTrainGuiParam.m_dEndX[i], m_tmpTrainGuiParam.m_dEndY[i]);

		if(bIsProcessImage)
		{
			bRet = m_FindLineTool[i].Execute(tmpImage, cLineSeg[i], dRMS[i]);
		}
		else
		{
			bRet = m_FindLineTool[i].Execute(m_tmpImageTrain, cLineSeg[i], dRMS[i]);
		}

		if (!bRet)
		{
			return false;
		}

	}

	// 求交点
	bool bInterSect = false;
	std::vector<sc2Vector> vectArray;
	bRet = sfInterSectLineLine(cLineSeg[0].GetLine(), cLineSeg[1].GetLine(), bInterSect, vectArray);
	if (!bRet || !bInterSect || vectArray.size() <= 0)
	{
		return false;
	}

	m_tmpTrainResult.m_dModelX = vectArray.at(0).GetX();
	m_tmpTrainResult.m_dModelY = vectArray.at(0).GetY();
	for(int i = 0; i < LINE_NUM; i++)
	{
		m_tmpTrainResult.m_dStartX[i] = cLineSeg[i].GetP1().GetX();
		m_tmpTrainResult.m_dStartY[i] = cLineSeg[i].GetP1().GetY();
		m_tmpTrainResult.m_dEndX[i]	  = cLineSeg[i].GetP2().GetX();
		m_tmpTrainResult.m_dEndY[i]   = cLineSeg[i].GetP2().GetY();
	}

	m_GuiTrainResultLine[0].SetLine(cLineSeg[0].GetLine());
	m_GuiTrainResultLine[1].SetLine(cLineSeg[0].GetLine());

	// 找圆
	if(m_tmpTrainDataParam.m_nCircleToolType < 3)		//圆
	{
		for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
		{	
			m_FindCircleTool_1[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCircleCaliperNum[i]);
			m_FindCircleTool_1[i].SetCaliperParams(m_tmpTrainGuiParam.m_nCircleCaliperNum[i], 
				m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i], 
				m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i], 
				m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i]);
			m_FindCircleTool_1[i].SetCaliperProperty(m_tmpTrainDataParam.m_dCircleContrastThre[i], 
				m_tmpTrainDataParam.m_edgeCircleProperty[i], 
				m_tmpTrainDataParam.m_lCircleFilterHalfWidth[i]);
			m_FindCircleTool_1[i].SetOutLineNum(m_tmpTrainDataParam.m_nCircleOutLineNum[i]);
			m_FindCircleTool_1[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bCircleSortByScore[i]);
			m_FindCircleTool_1[i].SetCenterRadiusAngleStartAngleSpan(
				m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i], 
				m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);

			scCircle cCircle[CIRCLE_NUM];
			double   dRMS[CIRCLE_NUM];

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
				return false;
			}
			m_tmpTrainResult.m_dCircleCenterX[i] = cCircle[i].GetCenter().GetX();
			m_tmpTrainResult.m_dCircleCenterY[i] = cCircle[i].GetCenter().GetY();
			m_tmpTrainResult.m_dCircleRadius[i]  = cCircle[i].GetRadius();

			m_GuiTrainResultCircle_1[i].SetCircle(cCircle[i]);
		}
	}
	else//长圆
	{
		m_FindObroundTool.SetCaliperNum(m_tmpTrainGuiParam.m_nObroundCaliperNum);
		m_FindObroundTool.SetCaliperParams(m_tmpTrainGuiParam.m_nObroundCaliperNum, 
			m_tmpTrainGuiParam.m_ObroundCaliperSearchDirection, 
			m_tmpTrainGuiParam.m_dObroundCaliperSearchLen, 
			m_tmpTrainGuiParam.m_dObroundCaliperProjectLen);
		m_FindObroundTool.SetCaliperProperty(m_tmpTrainDataParam.m_dObroundContrastThre, 
			m_tmpTrainDataParam.m_edgeObroundProperty, 
			m_tmpTrainDataParam.m_lObroundFilterHalfWidth);
		m_FindObroundTool.SetOutLineNum(m_tmpTrainDataParam.m_nObroundOutLineNum);
		m_FindObroundTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bObroundSortByScore);
		m_FindObroundTool.SetCenterLengthsRotation(
			m_tmpTrainGuiParam.m_ObroundCenter, m_tmpTrainGuiParam.m_dObroundSideXLen, 
			m_tmpTrainGuiParam.m_dObroundSideYLen, scRadian(m_tmpTrainGuiParam.m_dObroundRotation));
		m_FindObroundTool.SetObround(m_tmpTrainGuiParam.m_Obround);

		scObround cObround;
		double   dRMS;

		if(bIsProcessImage)
		{
			bRet = m_FindObroundTool.Execute(tmpImage, cObround, dRMS);
		}
		else
		{
			bRet = m_FindObroundTool.Execute(m_tmpImageTrain, cObround, dRMS);
		}

		if (!bRet)
		{
			return false;
		}

		m_tmpTrainResult.m_dObroundCenterX = cObround.GetCenter().GetX();
		m_tmpTrainResult.m_dObroundCenterY = cObround.GetCenter().GetY();
		m_tmpTrainResult.m_dObroundRotation = scDegree(cObround.GetRotation()).ToDouble();
		m_tmpTrainResult.m_dObroundSideXLen = cObround.GetXLength();
		m_tmpTrainResult.m_dObroundSideYLen = cObround.GetYLength();

		m_GuiTrainResultObround.SetObround(cObround);
	}

	return true;
}

// 获取训练结果图像														
bool CCircleCornerCheckSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
		//bRet = m_QuickSearchTool.GetTrainResultImage(m_ImageTrainResult);
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}


// 获取训练结果
bool CCircleCornerCheckSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	*((CCircleCornerCheckSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CCircleCornerCheckSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CCircleCornerCheckSearchTool::SetSearchDataParamForce(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_SearchDataParam = *((CCircleCornerCheckSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 设置搜索Data参数	
bool CCircleCornerCheckSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CCircleCornerCheckSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CCircleCornerCheckSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	*((CCircleCornerCheckSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CCircleCornerCheckSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CCircleCornerCheckSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CCircleCornerCheckSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCircleCornerSearchCheckInspectTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CCircleCornerCheckSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}	


//训练时设置找线和找圆的参数
bool CCircleCornerCheckSearchTool::SetSearchLineCircleParam()
{
	for (int i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);

		m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);

		// 找线双极性
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
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		} 
		else
		{
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		}


		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);

		m_FindLineTool[i].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask_FindLine[i]);
	}

	CString strTemp;
	if(m_tmpTrainDataParam.m_nCircleToolType < 3)		//圆
	{
		for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
		{
			m_FindCircleTool_1[i].SetCaliperNum(m_TrainGuiParam.m_nCircleCaliperNum[i]);
			m_FindCircleTool_1[i].SetCaliperParams(m_TrainGuiParam.m_nCircleCaliperNum[i], 
				m_TrainGuiParam.m_CircleCaliperSearchDirection[i], 
				m_TrainGuiParam.m_dCircleCaliperSearchLen[i], 
				m_TrainGuiParam.m_dCircleCaliperProjectLen[i]);
			m_FindCircleTool_1[i].SetCaliperProperty(m_TrainDataParam.m_dCircleContrastThre[i], 
				m_TrainDataParam.m_edgeCircleProperty[i], 
				m_TrainDataParam.m_lCircleFilterHalfWidth[i]);
			m_FindCircleTool_1[i].SetOutLineNum(m_TrainDataParam.m_nCircleOutLineNum[i]);
			m_FindCircleTool_1[i].EnableCaliperSortByScore(m_TrainDataParam.m_bCircleSortByScore[i]);
		}
	}else//长圆
	{
		m_FindObroundTool.SetCaliperNum(m_TrainGuiParam.m_nObroundCaliperNum);
		m_FindObroundTool.SetCaliperParams(m_TrainGuiParam.m_nObroundCaliperNum, m_TrainGuiParam.m_ObroundCaliperSearchDirection, m_TrainGuiParam.m_dObroundCaliperSearchLen
			,m_TrainGuiParam.m_dObroundCaliperProjectLen);
		m_FindObroundTool.SetCaliperProperty(m_TrainDataParam.m_dObroundContrastThre, m_TrainDataParam.m_edgeObroundProperty, m_TrainDataParam.m_lObroundFilterHalfWidth);
		m_FindObroundTool.SetOutLineNum(m_TrainDataParam.m_nObroundOutLineNum);
		m_FindObroundTool.EnableCaliperSortByScore(m_TrainDataParam.m_bObroundSortByScore);
		m_FindObroundTool.SetObround(m_tmpTrainGuiParam.m_Obround);
	}


	return true;
}

//搜索时找直线的函数
bool CCircleCornerCheckSearchTool::SearchFindLineFunc(int nIndex,cpImage &ImageSearch,double dRotation,double dTransX,double dTransY,CSearchResult& searchResult,scLineSeg & ResultLine1,scLineSeg & ResultLine2)
{
	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));

	// 执行角定位找线
	scLineSeg ResLine[LINE_NUM];
	double		dRMS[LINE_NUM];
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

		// 执行找线
		bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
	}

	//拟合点
	if (bFindLine[0] && m_tmpSearchGuiParam.m_bVisibleFitPoint1)
	{
		CString str = _T("");
		str.Format(_T("%d_FitPoint1_%d"),1,1);
		ShowFitPoint(m_pGuiSearchResultFitPoint1Array,m_GuiSearchResultFitPoint1,&m_FindLineTool[0],str);

	}
	if (bFindLine[1]&& m_tmpSearchGuiParam.m_bVisibleFitPoint2)
	{

		CString str = _T("");
		str.Format(_T("%d_FitPoint2_%d"),2,2);
		ShowFitPoint(m_pGuiSearchResultFitPoint2Array,m_GuiSearchResultFitPoint2,&m_FindLineTool[1],str);
	}

	if (!bFindLine[0] && bFindLine[1])
	{
		m_nResultErrNumber=-2;
		sc2Vector pointCenter2;
		//计算找线2的中点
		GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);

		searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
		searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
		searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
		searchResult.m_bFindLine2 = TRUE;
		searchResult.m_bFindLine1 = FALSE;
		m_SearchResultArray.push_back(searchResult);
		return false;
	}
	if (bFindLine[0] && !bFindLine[1])
	{
		m_nResultErrNumber=-3;
		//	CSearchResult searchResult;
		sc2Vector pointCenter1;
		//计算找线1的中点
		GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);
		searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
		searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
		searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
		searchResult.m_bFindLine1=TRUE;
		searchResult.m_bFindLine2=FALSE;
		m_SearchResultArray.push_back(searchResult);
		return false;
	}
	if (!bFindLine[0] && !bFindLine[1])
	{
		m_nResultErrNumber=-4;
		//	CSearchResult searchResult;
		searchResult.m_bFindLine1=FALSE;
		searchResult.m_bFindLine2=FALSE;
		m_SearchResultArray.push_back(searchResult);
		return false;
	}
	// 求交点
	if (bFindLine[0] && bFindLine[1])
	{
		sc2Vector pointCenter1;
		sc2Vector pointCenter2;
		sc2Vector vLineCrossPoint;
		bool bIntersect;
		std::vector<sc2Vector> crossPointArray; 
		if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
			&& crossPointArray.size() > 0)
		{

			//计算找线1的中点
			GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);

			//计算找线2的中点
			GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);

			ResultLine1 = ResLine[0];
			ResultLine2 = ResLine[1];

			//		CSearchResult searchResult;
			searchResult.m_dPosX = crossPointArray[0].GetX();
			searchResult.m_dPosY = crossPointArray[0].GetY();
			// 角度
			//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
			double dRotAngle = scDegree((ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()+ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180())/2.0).ToDouble(); 
			searchResult.m_dAngle = dRotAngle;


			searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
			searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

			searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
			searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

			searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
			searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

			searchResult.m_bFindLine1 = TRUE;
			searchResult.m_bFindLine2 = TRUE;

			CResultLine line;
			line.m_vLineX[0] = ResLine[0].GetP1().GetX();
			line.m_vLineY[0] = ResLine[0].GetP1().GetY();
			line.m_vLineX[1] = ResLine[0].GetP2().GetX();
			line.m_vLineY[1] = ResLine[0].GetP2().GetY();
			searchResult.m_vLineSearchResult.push_back(line);
			line.m_vLineX[0] = ResLine[1].GetP1().GetX();
			line.m_vLineY[0] = ResLine[1].GetP1().GetY();
			line.m_vLineX[1] = ResLine[1].GetP2().GetX();
			line.m_vLineY[1] = ResLine[1].GetP2().GetY();
			searchResult.m_vLineSearchResult.push_back(line);

			////显示交点
			//ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

			//显示线1中点
			ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,nIndex * 2 + 1,pointCenter1,_T("_ResultCross1_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

			//显示线2中点
			ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,nIndex * 2 + 2,pointCenter2,_T("_ResultCross2_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

			for (int l=0; l<2; l++)
			{
				ShowResultLine(m_pGuiSearchLineArray,m_GuiSearchLine,nIndex * 2 + l,&(ResLine[l].GetLine()),_T("_ResultLine_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
			}

			//显示交点及详细信息
			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f"), nIndex, crossPointArray[0].GetX(), crossPointArray[0].GetY());
			ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,nIndex ,crossPointArray[0],_T("_ResultLineCross_"),st,m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
		}
	}
	return true;
}

bool CCircleCornerCheckSearchTool::SearchOneCircleDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scCircle & ResultCircle,int nOffset,CSearchResult& searchResult)
{
	if(!m_tmpSearchDataParam.m_bEnableSizeCheck && !m_tmpSearchDataParam.m_bEnableDistCheck)
	{
		return true;
	}

	bool bMeasureSucces = true;

	// 判断距离结果
	// 计算点到直线距离
	sc2Vector PosModel[DIST_MEASURE_NUMS];
	scLineSeg LineSegDistance[DIST_MEASURE_NUMS];
	sc2Vector PosProject[DIST_MEASURE_NUMS];
	double dDistancePixel[DIST_MEASURE_NUMS];
	double dDistanceMM[DIST_MEASURE_NUMS];
	
	if(m_tmpSearchDataParam.m_bEnableSizeCheck)//启用尺寸检查
	{
		// 1. 圆半径的距离
		dDistancePixel[0] = ResultCircle.GetRadius();
		dDistanceMM[0]	  = dDistancePixel[0] * m_tmpSearchDataParam.m_dPixelRatio;
		LineSegDistance[0].SetP1(ResultCircle.GetCenter());
		LineSegDistance[0].SetP2(sc2Vector(ResultCircle.GetCenter().GetX() ,ResultCircle.GetCenter().GetY()+ ResultCircle.GetRadius()));

		COLORREF lineColor = RGB(0,0,255);
		searchResult.m_CircleCornerDistance.m_vSizeMM.push_back(dDistanceMM[0]);
		if ((true == m_tmpSearchDataParam.m_bEnableFlu[0] && fabs(dDistanceMM[0]-m_tmpSearchDataParam.m_dDistanceBase[0]) > m_tmpSearchDataParam.m_dDistanceFlu[0]))
		{
			lineColor = RGB(255,0,0);
			searchResult.m_CircleCornerDistance.m_nSizeControlResult = -1;
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);
			m_nResultErrNumber  = -30;
			if ((dDistanceMM[0]-m_tmpSearchDataParam.m_dDistanceBase[0]) > m_tmpSearchDataParam.m_dDistanceFlu[0]) //上限
			{
				m_strErrorDetail.Format(_T("-%.3f,超上限"),dDistanceMM[0]);
			}
 			else//下限
 			{
 				m_strErrorDetail.Format(_T("-%.3f,超下限"),dDistanceMM[0]);
 			}
			if(m_tmpSearchDataParam.m_bSizeCheckUseToTool)
			{
				bMeasureSucces		= false;
			}
			
		}else
		{
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
		}
		if(m_tmpSearchDataParam.m_bEnableDispLineSeg[0])
		{
			searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[0]);
			CString StrTmpDis1 = _T("");
			StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[0]);
			StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[0] + StrTmpDis1;
			searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);

			//显示GUI
			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,0,&LineSegDistance[0],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,0,&LineSegDistance[0],dDistanceMM[0],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
		}	
	}

	if(!bMeasureSucces)
	{
		return false;
	}

	if(!m_tmpSearchDataParam.m_bEnableDistCheck)//启用距离判断
	{
		return true;
	}

	// 2. 圆心到直线1的距离
	PosModel[1] = sc2Vector(ResultCircle.GetCenter().GetX(), ResultCircle.GetCenter().GetY());
	PosProject[1] = ResultLine1.GetLine().Project(PosModel[1]);
	LineSegDistance[1].SetP1(sc2Vector(PosModel[1].GetX(), PosModel[1].GetY()));
	LineSegDistance[1].SetP2(sc2Vector(PosProject[1].GetX(), PosProject[1].GetY()));
	dDistancePixel[1] = PosModel[1].Distance(PosProject[1]);
	dDistanceMM[1] = dDistancePixel[1] * m_tmpSearchDataParam.m_dPixelRatio;

	// 3. 圆心到直线2的距离
	PosModel[2] = sc2Vector(ResultCircle.GetCenter().GetX(), ResultCircle.GetCenter().GetY());
	PosProject[2] = ResultLine2.GetLine().Project(PosModel[2]);
	LineSegDistance[2].SetP1(sc2Vector(PosModel[2].GetX(), PosModel[2].GetY()));
	LineSegDistance[2].SetP2(sc2Vector(PosProject[2].GetX(), PosProject[2].GetY()));
	dDistancePixel[2] = PosModel[2].Distance(PosProject[2]);
	dDistanceMM[2] = dDistancePixel[2] * m_tmpSearchDataParam.m_dPixelRatio;

	// 循环判断
	for (int i =1;i < 3;i++)
	{
		COLORREF lineColor = RGB(0,0,255);
		searchResult.m_CircleCornerDistance.m_vDistMM.push_back(dDistanceMM[i]);
		if ((true == m_tmpSearchDataParam.m_bEnableFlu[i] && fabs(dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]))
		{
			lineColor           = RGB(255,0,0);
			m_nResultErrNumber  = -30 - i;
			if ((dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]) //上限
			{
				m_strErrorDetail.Format(_T("-%.3f,超上限"),dDistanceMM[i]);
			}
			else//下限
 			{
 				m_strErrorDetail.Format(_T("-%.3f,超下限"),dDistanceMM[i]);
 			}
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);	
			searchResult.m_CircleCornerDistance.m_nDistControlResult = -1;
			if(m_tmpSearchDataParam.m_bDistCheckUseToTool)
			{
				bMeasureSucces		= false;
			}
		}
		else
		{
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
		}

		if(m_tmpSearchDataParam.m_bEnableDispLineSeg[1])
		{
			searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[i]);
			CString StrTmpDis1 = _T("");
			StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[i]);
			StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[i] + StrTmpDis1;
			searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);
			//显示GUI
			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],dDistanceMM[i],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
		}	
	}
	return bMeasureSucces;
}

bool CCircleCornerCheckSearchTool::SearchTwoCircleDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scCircle & ResultCircle1,scCircle & ResultCircle2,int nOffset,CSearchResult& searchResult)
{
	if(!m_tmpSearchDataParam.m_bEnableSizeCheck && !m_tmpSearchDataParam.m_bEnableDistCheck)
	{
		return true;
	}

	bool bMeasureSucces = true;
	m_strErrorDetail = _T("");
	// 判断距离结果
	// 计算点到直线距离
	sc2Vector PosModel[DIST_MEASURE_NUMS];
	scLineSeg LineSegDistance[DIST_MEASURE_NUMS];
	sc2Vector PosProject[DIST_MEASURE_NUMS];
	double dDistancePixel[DIST_MEASURE_NUMS];
	double dDistanceMM[DIST_MEASURE_NUMS];

	if(m_tmpSearchDataParam.m_bEnableSizeCheck)//启用距离判断
	{
		// 1. 圆1半径的距离	
		dDistancePixel[0] = ResultCircle1.GetRadius();
		dDistanceMM[0]	  = dDistancePixel[0] * m_tmpSearchDataParam.m_dPixelRatio;
		LineSegDistance[0].SetP1(ResultCircle1.GetCenter());
		LineSegDistance[0].SetP2(sc2Vector(ResultCircle1.GetCenter().GetX() ,ResultCircle1.GetCenter().GetY()- ResultCircle1.GetRadius()));

		// 2. 圆2半径的距离
		dDistancePixel[1] = ResultCircle2.GetRadius();
		dDistanceMM[1]	  = dDistancePixel[1] * m_tmpSearchDataParam.m_dPixelRatio;
		LineSegDistance[1].SetP1(ResultCircle2.GetCenter());
		LineSegDistance[1].SetP2(sc2Vector(ResultCircle2.GetCenter().GetX() ,ResultCircle2.GetCenter().GetY()- ResultCircle2.GetRadius()));


		// 3. 两个圆的距离
		dDistancePixel[2] = ResultCircle2.GetCenter().Distance(ResultCircle1.GetCenter());
		dDistanceMM[2]	  = dDistancePixel[2] * m_tmpSearchDataParam.m_dPixelRatio;

		double dDisXPixel = ResultCircle1.GetCenter().GetX()-ResultCircle2.GetCenter().GetX();
		double dDisYPixel = ResultCircle1.GetCenter().GetY()-ResultCircle2.GetCenter().GetY();
		double dDisXMM = dDisXPixel* m_tmpSearchDataParam.m_dPixelRatio;
		double dDisYMM = dDisYPixel* m_tmpSearchDataParam.m_dPixelRatio;

		scLineSeg LineSegDistanceTmpX;//同心度X分解线段
		scLineSeg LineSegDistanceTmpY;//同心度X分解线段
		LineSegDistanceTmpX.SetP1(sc2Vector(ResultCircle1.GetCenter().GetX(),ResultCircle1.GetCenter().GetY()));
		LineSegDistanceTmpX.SetP2(sc2Vector(ResultCircle2.GetCenter().GetX(),ResultCircle1.GetCenter().GetY()));

		LineSegDistanceTmpY.SetP1(sc2Vector(ResultCircle1.GetCenter().GetX(),ResultCircle1.GetCenter().GetY()));
		LineSegDistanceTmpY.SetP2(sc2Vector(ResultCircle1.GetCenter().GetX(),ResultCircle2.GetCenter().GetY()));
		searchResult.m_CircleCornerDistance.m_dConcentricityX =dDisXMM; 
		searchResult.m_CircleCornerDistance.m_dConcentricityY =dDisYMM; 

		LineSegDistance[2].SetP1(ResultCircle1.GetCenter());
		LineSegDistance[2].SetP2(ResultCircle2.GetCenter());
		COLORREF lineColor2 = RGB(0,0,255);

		bool bTmpLineSegResult = true;

		for (int i =0;i < 3;i++)
		{
			COLORREF lineColor = RGB(0,0,255);


			bool bCheckFluRes = true;
			searchResult.m_CircleCornerDistance.m_vSizeMM.push_back(dDistanceMM[i]);
			if ((true == m_tmpSearchDataParam.m_bEnableFlu[i] && fabs(dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]))
			{
				bCheckFluRes = false;
				lineColor = RGB(255,0,0);

				if (i==2)
				{
					lineColor2  = RGB(255,0,0);
					bTmpLineSegResult = false;
				}

				if ((dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]) //上限
				{
					m_strErrorDetail.Format(_T("-%.3f,超上限"),dDistanceMM[i]);
				}
 				else//下限
 				{
 					m_strErrorDetail.Format(_T("-%.3f,超下限"),dDistanceMM[i]);
 				}
				m_nResultErrNumber  = -13 - i;

				searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);	
				searchResult.m_CircleCornerDistance.m_nSizeControlResult = -1;
				if(m_tmpSearchDataParam.m_bSizeCheckUseToTool)
				{
					bMeasureSucces		= false;
				}
			}
			else
			{
				searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
			}

			if(m_tmpSearchDataParam.m_bEnableDispLineSeg[0])
			{
				searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[i]);
				CString StrTmpDis1 = _T("");
				StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[i]);
				StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[i] + StrTmpDis1;
				searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);
				CResultLine line;
				line.m_vLineX[0] = LineSegDistance[i].GetP1().GetX();
				line.m_vLineY[0] = LineSegDistance[i].GetP1().GetY();
				line.m_vLineX[1] = LineSegDistance[i].GetP2().GetX();
				line.m_vLineY[1] = LineSegDistance[i].GetP2().GetY();
				line.m_bFindResult = bCheckFluRes;

				searchResult.m_vLineSegSearchResult.push_back(line);


				//显示GUI
				ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
				//显示GUI
				ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],dDistanceMM[i],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			}	
		}

		if(m_tmpSearchDataParam.m_bEnableDispLineSeg[0])
		{
			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,10,&LineSegDistanceTmpX,_T("_ResultLineSeg2_"),lineColor2,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,10,&LineSegDistanceTmpX,dDisXMM,_T("_ResultLineSegText2_"),lineColor2,m_tmpSearchGuiParam.m_nLineWidth);

			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,11,&LineSegDistanceTmpY,_T("_ResultLineSeg2_"),lineColor2,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,11,&LineSegDistanceTmpY,dDisYMM,_T("_ResultLineSegText2_"),lineColor2,m_tmpSearchGuiParam.m_nLineWidth);


			CResultLine line;
			line.m_vLineX[0] = LineSegDistanceTmpX.GetP1().GetX();
			line.m_vLineY[0] = LineSegDistanceTmpX.GetP1().GetY();
			line.m_vLineX[1] = LineSegDistanceTmpX.GetP2().GetX();
			line.m_vLineY[1] = LineSegDistanceTmpX.GetP2().GetY();
			line.m_bFindResult = bTmpLineSegResult;
			searchResult.m_vLineSegSearchResult.push_back(line);

			line.m_vLineX[0] = LineSegDistanceTmpY.GetP1().GetX();
			line.m_vLineY[0] = LineSegDistanceTmpY.GetP1().GetY();
			line.m_vLineX[1] = LineSegDistanceTmpY.GetP2().GetX();
			line.m_vLineY[1] = LineSegDistanceTmpY.GetP2().GetY();
			line.m_bFindResult = bTmpLineSegResult;
			searchResult.m_vLineSegSearchResult.push_back(line);
		}


	}
	if(!bMeasureSucces)
		return false;

	if(!m_tmpSearchDataParam.m_bEnableDistCheck)//启用距离判断
	{
		return true;
	}

	// 连个圆的中心点
	double dCircleCenterX = 0.0;
	double dCircleCenterY = 0.0;
	double dCircleCenterX1 = 0.0;
	double dCircleCenterY1 = 0.0;
	double dCircleCenterX2 = 0.0;
	double dCircleCenterY2 = 0.0;

	if (m_tmpSearchDataParam.m_nDistanceType==0)  // 两圆心-连线中点
	{
		dCircleCenterX = (ResultCircle2.GetCenter().GetX() + ResultCircle1.GetCenter().GetX())/2.0;
		dCircleCenterY = (ResultCircle2.GetCenter().GetY() + ResultCircle1.GetCenter().GetY())/2.0;
	}
	else if (m_tmpSearchDataParam.m_nDistanceType==1)  // 圆1圆心
	{
		dCircleCenterX = ResultCircle1.GetCenter().GetX();
		dCircleCenterY = ResultCircle1.GetCenter().GetY();
	}
	else if (m_tmpSearchDataParam.m_nDistanceType==2)  //  圆2圆心
	{
		dCircleCenterX = ResultCircle2.GetCenter().GetX();
		dCircleCenterY = ResultCircle2.GetCenter().GetY();
	}
	else if  (m_tmpSearchDataParam.m_nDistanceType==3)  // 同时管控所有
	{
		dCircleCenterX = (ResultCircle1.GetCenter().GetX()+ResultCircle2.GetCenter().GetX())/2;
		dCircleCenterY = (ResultCircle1.GetCenter().GetY()+ResultCircle2.GetCenter().GetY())/2;

		dCircleCenterX1 = ResultCircle1.GetCenter().GetX();
		dCircleCenterY1 = ResultCircle1.GetCenter().GetY();

		dCircleCenterX2= ResultCircle2.GetCenter().GetX();
		dCircleCenterY2 = ResultCircle2.GetCenter().GetY();

	}
	// 2. 两个圆的中心连线的中心到直线1的距离
	PosModel[3] = sc2Vector(dCircleCenterX, dCircleCenterY);
	PosProject[3] = ResultLine1.GetLine().Project(PosModel[3]);
	LineSegDistance[3].SetP1(sc2Vector(PosModel[3].GetX(), PosModel[3].GetY()));
	LineSegDistance[3].SetP2(sc2Vector(PosProject[3].GetX(), PosProject[3].GetY()));
	dDistancePixel[3] = PosModel[3].Distance(PosProject[3]);
	dDistanceMM[3] = dDistancePixel[3] * m_tmpSearchDataParam.m_dPixelRatio;

	// 2. 两个圆的中心连线的中心到直线2的距离
	PosModel[4] = sc2Vector(dCircleCenterX, dCircleCenterY);
	PosProject[4] = ResultLine2.GetLine().Project(PosModel[4]);
	LineSegDistance[4].SetP1(sc2Vector(PosModel[4].GetX(), PosModel[4].GetY()));
	LineSegDistance[4].SetP2(sc2Vector(PosProject[4].GetX(), PosProject[4].GetY()));
	dDistancePixel[4] = PosModel[4].Distance(PosProject[4]);
	dDistanceMM[4] = dDistancePixel[4] * m_tmpSearchDataParam.m_dPixelRatio;

	 // 圆1 圆心到直线1的距离
	PosModel[5] = sc2Vector(dCircleCenterX1, dCircleCenterY1);
	PosProject[5] = ResultLine1.GetLine().Project(PosModel[5]);
	LineSegDistance[5].SetP1(sc2Vector(PosModel[5].GetX(), PosModel[5].GetY()));
	LineSegDistance[5].SetP2(sc2Vector(PosProject[5].GetX(), PosProject[5].GetY()));
	dDistancePixel[5] = PosModel[5].Distance(PosProject[5]);
	dDistanceMM[5] = dDistancePixel[5] * m_tmpSearchDataParam.m_dPixelRatio;

	// 圆1 圆心到直线2的距离
	PosModel[6] = sc2Vector(dCircleCenterX1, dCircleCenterY1);
	PosProject[6] = ResultLine2.GetLine().Project(PosModel[6]);
	LineSegDistance[6].SetP1(sc2Vector(PosModel[6].GetX(), PosModel[6].GetY()));
	LineSegDistance[6].SetP2(sc2Vector(PosProject[6].GetX(), PosProject[6].GetY()));
	dDistancePixel[6] = PosModel[6].Distance(PosProject[6]);
	dDistanceMM[6] = dDistancePixel[6] * m_tmpSearchDataParam.m_dPixelRatio;

	// 圆2 圆心到直线1的距离
	PosModel[7] = sc2Vector(dCircleCenterX2, dCircleCenterY2);
	PosProject[7] = ResultLine1.GetLine().Project(PosModel[7]);
	LineSegDistance[7].SetP1(sc2Vector(PosModel[7].GetX(), PosModel[7].GetY()));
	LineSegDistance[7].SetP2(sc2Vector(PosProject[7].GetX(), PosProject[7].GetY()));
	dDistancePixel[7] = PosModel[7].Distance(PosProject[7]);
	dDistanceMM[7] = dDistancePixel[7] * m_tmpSearchDataParam.m_dPixelRatio;

	// 圆2 圆心到直线2的距离
	PosModel[8] = sc2Vector(dCircleCenterX2, dCircleCenterY2);
	PosProject[8] = ResultLine2.GetLine().Project(PosModel[8]);
	LineSegDistance[8].SetP1(sc2Vector(PosModel[8].GetX(), PosModel[8].GetY()));
	LineSegDistance[8].SetP2(sc2Vector(PosProject[8].GetX(), PosProject[8].GetY()));
	dDistancePixel[8] = PosModel[8].Distance(PosProject[8]);
	dDistanceMM[8] = dDistancePixel[8] * m_tmpSearchDataParam.m_dPixelRatio;


	// 循环判断
	int nLimit = 5;

	if ( (m_tmpSearchDataParam.m_nDistanceType==3) && m_tmpSearchDataParam.m_bEnableDistCheck )
	{
		nLimit = 9;
	}

	for (int i = 3 ;i < nLimit;i++)
	{
		bool bCheckResult = true;
		COLORREF lineColor = RGB(0,0,255);
		searchResult.m_CircleCornerDistance.m_vDistMM.push_back(dDistanceMM[i]);
		if ((true == m_tmpSearchDataParam.m_bEnableFlu[i] && fabs(dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]))
		{
			bCheckResult       = false;
			lineColor           = RGB(255,0,0);
			m_nResultErrNumber  = -13 - i;

			if ((dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]) //上限
			{
				m_strErrorDetail.Format(_T("-%.3f,超上限"),dDistanceMM[i]);
			}
 			else//下限
 			{
 				m_strErrorDetail.Format(_T("-%.3f,超下限"),dDistanceMM[i]);
 			}

			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);	
			searchResult.m_CircleCornerDistance.m_nDistControlResult = -1;
			if(m_tmpSearchDataParam.m_bDistCheckUseToTool)
			{
				bMeasureSucces		= false;
			}
		}
		else
		{
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
		}

		if(m_tmpSearchDataParam.m_bEnableDispLineSeg[1])
		{
			searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[i]);
			CString StrTmpDis1 = _T("");
			StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[i]);
			StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[i] + StrTmpDis1;
			searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);

			CResultLine line;
			line.m_vLineX[0] = LineSegDistance[i].GetP1().GetX();
			line.m_vLineY[0] = LineSegDistance[i].GetP1().GetY();
			line.m_vLineX[1] = LineSegDistance[i].GetP2().GetX();
			line.m_vLineY[1] = LineSegDistance[i].GetP2().GetY();
			line.m_bFindResult = bCheckResult;
			searchResult.m_vLineSegSearchResult.push_back(line);

			//显示GUI
			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],dDistanceMM[i],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
		}	
	}

	return bMeasureSucces;
}


bool CCircleCornerCheckSearchTool::SearchOneObroundDistMeasureFunc(scLineSeg & ResultLine1,scLineSeg & ResultLine2,scObround & ResultObround,int nOffset,CSearchResult& searchResult)
{
	if(!m_tmpSearchDataParam.m_bEnableSizeCheck && !m_tmpSearchDataParam.m_bEnableDistCheck)
	{
		return true;
	}

	bool bMeasureSucces = true;

	// 判断距离结果
	// 计算点到直线距离
	sc2Vector PosModel[DIST_MEASURE_NUMS];
	scLineSeg LineSegDistance[DIST_MEASURE_NUMS];
	sc2Vector PosProject[DIST_MEASURE_NUMS];
	double dDistancePixel[DIST_MEASURE_NUMS];	
	double dDistanceMM[DIST_MEASURE_NUMS];

	if(m_tmpSearchDataParam.m_bEnableSizeCheck)//启用距离判断
	{
		// 1. 圆半径的距离
		dDistancePixel[0] = ResultObround.GetYLength()/2.0;
		dDistanceMM[0]	  = dDistancePixel[0] * m_tmpSearchDataParam.m_dPixelRatio;
		LineSegDistance[0].SetP1(ResultObround.GetCenter());
		double dTempX = (ResultObround.GetCornerPo().GetX() + ResultObround.GetCornerPx().GetX())/2.0;
		double dTempY = (ResultObround.GetCornerPo().GetY() + ResultObround.GetCornerPx().GetY())/2.0;
		LineSegDistance[0].SetP2(sc2Vector(dTempX,dTempY));

		dDistancePixel[1] = ResultObround.GetXLength()/2.0;
		dDistanceMM[1]	  = dDistancePixel[1] * m_tmpSearchDataParam.m_dPixelRatio;
		PosModel[1]       = ResultObround.GetCenter();
		scLineSeg tempLine(ResultObround.GetCornerPx(),ResultObround.GetCornerPopp());
		PosProject[1] = tempLine.GetLine().Project(PosModel[1]);
		LineSegDistance[1].SetP1(ResultObround.GetCenter());
		LineSegDistance[1].SetP2(PosProject[1]);

		for (int i =0;i < 2;i++)
		{
			bool bCheckResult1 = true;
			COLORREF lineColor = RGB(0,0,255);
			searchResult.m_CircleCornerDistance.m_vSizeMM.push_back(dDistanceMM[i]);
			if ((true == m_tmpSearchDataParam.m_bEnableFlu[i] && fabs(dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]))
			{
				bCheckResult1 = false;
				lineColor           = RGB(255,0,0);
				m_nResultErrNumber  = -8 - i;
				searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);	
				searchResult.m_CircleCornerDistance.m_nSizeControlResult = -1;
				if(m_tmpSearchDataParam.m_bSizeCheckUseToTool)
				{
					bMeasureSucces		= false;
				}
			}
			else
			{
				searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
			}

			if(m_tmpSearchDataParam.m_bEnableDispLineSeg[0])
			{
				searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[i]);
				CString StrTmpDis1 = _T("");
				StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[i]);
				StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[i] + StrTmpDis1;
				searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);

				CResultLine line;
				line.m_bFindResult = bCheckResult1;
				line.m_vLineX[0] = LineSegDistance[i].GetP1().GetX();
				line.m_vLineY[0] = LineSegDistance[i].GetP1().GetY();
				line.m_vLineX[1] = LineSegDistance[i].GetP2().GetX();
				line.m_vLineY[1] = LineSegDistance[i].GetP2().GetY();
				searchResult.m_vLineSegSearchResult.push_back(line);

				//显示GUI
				ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
				//显示GUI
				ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],dDistanceMM[i],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			}	
		}
	}

	if(!bMeasureSucces)
		return false;

	if(!m_tmpSearchDataParam.m_bEnableDistCheck)//启用距离判断
	{
		return true;
	}

	// 2. 圆心到直线1的距离
	PosModel[2] = sc2Vector(ResultObround.GetCenter().GetX(), ResultObround.GetCenter().GetY());
	PosProject[2] = ResultLine1.GetLine().Project(PosModel[2]);
	LineSegDistance[2].SetP1(sc2Vector(PosModel[2].GetX(), PosModel[2].GetY()));
	LineSegDistance[2].SetP2(sc2Vector(PosProject[2].GetX(), PosProject[2].GetY()));
	dDistancePixel[2] = PosModel[2].Distance(PosProject[2]);
	dDistanceMM[2] = dDistancePixel[2] * m_tmpSearchDataParam.m_dPixelRatio;

	// 2. 圆心到直线2的距离
	PosModel[3] = sc2Vector(ResultObround.GetCenter().GetX(), ResultObround.GetCenter().GetY());
	PosProject[3] = ResultLine2.GetLine().Project(PosModel[3]);
	LineSegDistance[3].SetP1(sc2Vector(PosModel[3].GetX(), PosModel[3].GetY()));
	LineSegDistance[3].SetP2(sc2Vector(PosProject[3].GetX(), PosProject[3].GetY()));
	dDistancePixel[3] = PosModel[3].Distance(PosProject[3]);
	dDistanceMM[3] = dDistancePixel[3] * m_tmpSearchDataParam.m_dPixelRatio;

	// 循环判断
	for (int i = 2;i < 4;i++)
	{
		bool bCheckResult2 = true;
		COLORREF lineColor = RGB(0,0,255);
		searchResult.m_CircleCornerDistance.m_vDistMM.push_back(dDistanceMM[i]);
		if ((true == m_tmpSearchDataParam.m_bEnableFlu[i] && fabs(dDistanceMM[i]-m_tmpSearchDataParam.m_dDistanceBase[i]) > m_tmpSearchDataParam.m_dDistanceFlu[i]))
		{
			bCheckResult2 = false;
			lineColor           = RGB(255,0,0);
			m_nResultErrNumber  = -8 - i;
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(-1);	
			searchResult.m_CircleCornerDistance.m_nDistControlResult = -1;
			if(m_tmpSearchDataParam.m_bDistCheckUseToTool)
			{
				bMeasureSucces		= false;
			}
		}
		else
		{
			searchResult.m_CircleCornerDistance.m_vLineSegResult.push_back(0);
		}

		if(m_tmpSearchDataParam.m_bEnableDispLineSeg[1])
		{
			searchResult.m_CircleCornerDistance.m_vLineSeg.push_back(LineSegDistance[i]);
			CString StrTmpDis1 = _T("");
			StrTmpDis1.Format(_T("%.3fmm"),dDistanceMM[i]);
			StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[i] + StrTmpDis1;
			searchResult.m_CircleCornerDistance.m_vLineSegName.push_back(StrTmpDis1);

			CResultLine line;
			line.m_bFindResult = bCheckResult2;
			line.m_vLineX[0] = LineSegDistance[i].GetP1().GetX();
			line.m_vLineY[0] = LineSegDistance[i].GetP1().GetY();
			line.m_vLineX[1] = LineSegDistance[i].GetP2().GetX();
			line.m_vLineY[1] = LineSegDistance[i].GetP2().GetY();
			searchResult.m_vLineSegSearchResult.push_back(line);

			//显示GUI
			ShowResultLineSeg(m_pGuiSearchLineSegArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],_T("_ResultLineSeg_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
			//显示GUI
			ShowResultLineSegText(m_pGuiSearchLineTextArray,m_GuiSearchLineSeg,i,&LineSegDistance[i],dDistanceMM[i],_T("_ResultLineSegText_"),lineColor,m_tmpSearchGuiParam.m_nLineWidth);
		}	
	}
	return bMeasureSucces;
}

//搜索是找圆的函数
bool CCircleCornerCheckSearchTool::SearchFindCircleFunc(int nIndex,cpImage &ImageSearch,double dRotation,double dTransX,double dTransY,scLineSeg & ResultLine1,scLineSeg & ResultLine2,CSearchResult& searchResult)
{

	CString strTemp;
	if(m_tmpTrainDataParam.m_nCircleToolType < 3)		//圆
	{
		int nCircleNum = m_tmpTrainDataParam.m_nCircleToolType;
		// 求找线工具变换后的位置,找Circle
		double dTransCenterX_1[CIRCLE_NUM], dTransCenterY_1[CIRCLE_NUM];
		double dOriCenterX_1[CIRCLE_NUM], dOriCenterY_1[CIRCLE_NUM];

		for (int i = 0; i < nCircleNum; i++)
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

		scCircle ResCircle_1[CIRCLE_NUM];
		double dRMS_1[CIRCLE_NUM];
		sc2Vector scCircleCenter[CIRCLE_NUM];

		for (int k = 0; k < nCircleNum; k++)
		{
			if (!m_FindCircleTool_1[k].Execute(ImageSearch, ResCircle_1[k], dRMS_1[k])) // 成功
			{
				m_nResultErrNumber = -33 - k;
				return false;
			}

			scCircleCenter[k] = ResCircle_1[k].GetCenter();
		}

		//显示圆
		CString strTmpKey;
		for (int k = 0; k < nCircleNum; k++)
		{
			strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, k);
			scGuiCircle *pTemCircle = m_GuiSearchCircle+k;
			pTemCircle->SetCircle(ResCircle_1[k]);
			pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemCircle->SetTipTextEnabled(true);
			pTemCircle->SetTipText(strTmpKey);
			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), k, ResCircle_1[k].GetCenter().GetX(), ResCircle_1[k].GetCenter().GetY(), ResCircle_1[k].GetRadius());
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

		if(nCircleNum == 1)
		{
			searchResult.m_vdAuxiliaryPosX.at(2) = scCircleCenter[0].GetX();
			searchResult.m_vdAuxiliaryPosY.at(2) = scCircleCenter[0].GetY();
			searchResult.m_vdAuxiliaryAngle.at(2) = searchResult.m_dAngle;
			//
			searchResult.m_CircleCornerDistance.m_vCircle.push_back(scCircleCenter[0]);
			searchResult.m_CircleCornerDistance.m_vRadius.push_back(ResCircle_1[0].GetRadius());

			searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle_1[0].GetRadius();
			searchResult.m_resultCircleSearchTool.m_scCircleCircle = sc2Vector(scCircleCenter[0].GetX(),scCircleCenter[0].GetY());

			CResultCircleSearchTool circle;
			circle.m_dRadius = ResCircle_1[0].GetRadius();
			circle.m_scCircleCircle = ResCircle_1[0].GetCenter();
			searchResult.m_vCircleSearchResult.push_back(circle);
			

			//一个圆的距离判断
			int nCircleOffset = 0;
			if(!SearchOneCircleDistMeasureFunc(ResultLine1,ResultLine2,ResCircle_1[0],nCircleOffset,searchResult))
				return false;

			//Blob分析
			if(m_tmpSearchDataParam.m_bEanbleBlobInspect)
			{
				scCircle tempCircle = ResCircle_1[0];
				tempCircle.SetRadius(ResCircle_1[0].GetRadius() * m_tmpSearchDataParam.m_dAreaScaleRatio);
				if(!SearchOneCircleBlobInspect(ImageSearch,tempCircle,searchResult) && m_tmpSearchDataParam.m_bBlobInspectUseToTool)
				{
					m_nResultErrNumber  = - 20;
					return false;
				}
			}else
			{
				m_bBlobInspectFinsh = false;
			}
			
		}
		else if(nCircleNum == 2)
		{
			scLineSeg ResLineSeg;
			ResLineSeg.Set(scCircleCenter[0], scCircleCenter[1]);

			searchResult.m_vdAuxiliaryPosX.at(2) = (ResLineSeg.GetP1().GetX() + ResLineSeg.GetP2().GetX())/2;
			searchResult.m_vdAuxiliaryPosY.at(2) = (ResLineSeg.GetP1().GetY() + ResLineSeg.GetP2().GetY())/2;

			searchResult.m_CircleCornerDistance.m_vCircle.push_back(scCircleCenter[0]);
			searchResult.m_CircleCornerDistance.m_vRadius.push_back(ResCircle_1[0].GetRadius());
			searchResult.m_CircleCornerDistance.m_vCircle.push_back(scCircleCenter[1]);
			searchResult.m_CircleCornerDistance.m_vRadius.push_back(ResCircle_1[1].GetRadius());

			searchResult.m_resultCircleSearchTool.m_dRadius = ResCircle_1[0].GetRadius();
			searchResult.m_resultCircleSearchTool.m_scCircleCircle = sc2Vector(scCircleCenter[0].GetX(),scCircleCenter[0].GetY());

			searchResult.m_resultCircleSearchTool2.m_dRadius = ResCircle_1[1].GetRadius();
			searchResult.m_resultCircleSearchTool2.m_scCircleCircle = sc2Vector(scCircleCenter[1].GetX(),scCircleCenter[1].GetY());

			CResultCircleSearchTool circle;
			circle.m_dRadius = ResCircle_1[0].GetRadius();
			circle.m_scCircleCircle = ResCircle_1[0].GetCenter();
			searchResult.m_vCircleSearchResult.push_back(circle);

			circle.m_dRadius = ResCircle_1[1].GetRadius();
			circle.m_scCircleCircle = ResCircle_1[1].GetCenter();
			searchResult.m_vCircleSearchResult.push_back(circle);

			//一个圆的距离判断

			int nCircleOffset = 0;
			if(!SearchTwoCircleDistMeasureFunc(ResultLine1,ResultLine2,ResCircle_1[0],ResCircle_1[1],nCircleOffset,searchResult))
				return false;
	
			//Blob分析
			if(m_tmpSearchDataParam.m_bEanbleBlobInspect)
			{
				scCircle tempCircle = ResCircle_1[0];
				tempCircle.SetRadius(ResCircle_1[0].GetRadius() * m_tmpSearchDataParam.m_dAreaScaleRatio);
				if(!SearchOneCircleBlobInspect(ImageSearch,tempCircle,searchResult) && m_tmpSearchDataParam.m_bBlobInspectUseToTool)
				{
					m_nResultErrNumber  = - 20;
					return false;
				}

				tempCircle = ResCircle_1[1];
				tempCircle.SetRadius(ResCircle_1[1].GetRadius() * m_tmpSearchDataParam.m_dAreaScaleRatio);
				if(!SearchOneCircleBlobInspect(ImageSearch,tempCircle,searchResult) && m_tmpSearchDataParam.m_bBlobInspectUseToTool)
				{
					m_nResultErrNumber  = - 21;
					return false;
				}
			}else
			{
				m_bBlobInspectFinsh = false;
			}
		}



		//////////////////////////////拟合点1，2////////////////////////////////////////////
		if (m_tmpSearchGuiParam.m_bEnableShowCirclePoint[0])
		{
			CString strTmpKey;
			std::vector<bool> bUsed;
			std::vector<sc2Vector> points;
			m_FindCircleTool_1[0].GetUsed(bUsed);
			m_FindCircleTool_1[0].GetFitData(points);
			for (long m=0; m<bUsed.size(); m++)
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


		if (m_tmpSearchGuiParam.m_bEnableShowCirclePoint[1] && nCircleNum > 1)
		{
			CString strTmpKey;
			std::vector<bool> bUsed;
			std::vector<sc2Vector> points;
			m_FindCircleTool_1[1].GetUsed(bUsed);
			m_FindCircleTool_1[1].GetFitData(points);
			for (long m=0; m<bUsed.size(); m++)
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
    else if(m_tmpTrainDataParam.m_nCircleToolType == 3)//长圆
	{
		double dTransCenterX, dTransCenterY;
		double dOriCenterX, dOriCenterY;
		dOriCenterX = m_TrainGuiParam.m_Obround.GetCenter().GetX();
		dOriCenterY = m_TrainGuiParam.m_Obround.GetCenter().GetY();
		dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
		dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;

		m_FindObroundTool.SetCenterLengthsRotation(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_Obround.GetXLength(),
			m_TrainGuiParam.m_Obround.GetYLength(), scRadian(dRotation)+m_TrainGuiParam.m_Obround.GetRotation());

		scObround ResObround;
		double dRMS;
		sc2Vector scObroundCenter;
		///
		if (m_FindObroundTool.Execute(ImageSearch, ResObround, dRMS)) // 成功
		{
			//searchResult.m_dPosX = ResObround.GetCenter().GetX();
			//searchResult.m_dPosY = ResObround.GetCenter().GetY();

			scObroundCenter = ResObround.GetCenter();

			CString strTmpKey;
			strTmpKey.Format(_T("%s_ResultObround%d"), m_strKeyText, nIndex);
			//scGuiCircle *pTemCircle = new scGuiCircle;
			scGuiObround *pTemObround = m_GuiSearchObround + nIndex;
			pTemObround->SetObround(ResObround);
			pTemObround->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTemObround->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTemObround->SetTipTextEnabled(true);
			pTemObround->SetTipText(strTmpKey);
			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f"), nIndex, ResObround.GetCenter().GetX(), ResObround.GetCenter().GetY());
			pTemObround->SetLabel(st);
			pTemObround->SetLabelVisible(TRUE);
			m_pGuiSearchObroundArray.push_back(pTemObround);

			strTmpKey.Format(_T("%s_ResultObroundCenter%d"), m_strKeyText, nIndex);
			//scGuiCross *pTempCircleCenter = new scGuiCross;
			scGuiCross *pTempObroundCenter = m_GuiSearchObroundCenter + nIndex;
			pTempObroundCenter->SetCenterExtent(ResObround.GetCenter(), sc2Vector(40,40));
			pTempObroundCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempObroundCenter->SetTipTextEnabled(true);
			pTempObroundCenter->SetTipText(strTmpKey);
			m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);	
		}
		else
		{
			m_nResultErrNumber = -35;
			return false;
		}
		searchResult.m_vdAuxiliaryPosX.at(2) = scObroundCenter.GetX();
		searchResult.m_vdAuxiliaryPosY.at(2) = scObroundCenter.GetY();
		searchResult.m_vdAuxiliaryAngle.at(2) = searchResult.m_dAngle;
		searchResult.m_CircleCornerDistance.m_vLongObroundCenter.push_back(scObroundCenter);
		searchResult.m_CircleCornerDistance.m_vObroundLenX.push_back(ResObround.GetXLength());
		searchResult.m_CircleCornerDistance.m_vObroundLenY.push_back(ResObround.GetYLength());
		searchResult.m_CircleCornerDistance.m_vObroundRadian.push_back(ResObround.GetRotation().ToDouble());

		CResultObround obround;
		obround.m_scObround = ResObround;
		searchResult.m_vObroundSearchResult.push_back(obround);
		//
		//searchResult.m_CircleCornerDistance.m_vCircle.push_back(scObroundCenter);
		//searchResult.m_CircleCornerDistance.m_vRadius.push_back(ResObround.GetYLength()/2);

		//一个长圆的距离判断
		int nCircleOffset = 0;
		if(!SearchOneObroundDistMeasureFunc(ResultLine1,ResultLine2,ResObround,nCircleOffset,searchResult))
			return false;



		//Blob分析
		if(m_tmpSearchDataParam.m_bEanbleBlobInspect)
		{
			scObround tempObround = ResObround;
			tempObround.SetXLength(ResObround.GetXLength() * m_tmpSearchDataParam.m_dAreaScaleRatio);
			tempObround.SetYLength(ResObround.GetYLength() * m_tmpSearchDataParam.m_dAreaScaleRatio);
			if(!SearchOneObroundBlobInspect(ImageSearch,tempObround,searchResult) && m_tmpSearchDataParam.m_bBlobInspectUseToTool)
			{
				m_nResultErrNumber  = - 20;
				return false;
			}
		}else
		{
			m_bBlobInspectFinsh = false;
		}


	}

	return true;
}

bool CCircleCornerCheckSearchTool::SearchOneCircleBlobInspect(cpImage &ImageSearch,scCircle & ResultCircle,CSearchResult& searchResult)
{
	// Blob分析
	scBlobTool blobTool;
	blobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_nBinaryThre,(m_tmpSearchDataParam.m_nDefectType == 2));
	blobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
	blobTool.ConnectivityCleanup(scBlobSceneDescription::eFill);
	blobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_nMinAcceptArea);
	
	bool bInspectOK = true;
	if (!blobTool.Execute(ImageSearch,&ResultCircle))
	{
		return bInspectOK;
	}
	scBlobSceneDescription* pBSD = blobTool.GetBSD();
	if (pBSD == NULL)
	{
		return bInspectOK;
	}	

	//计算Blob个数
	int nResultBlobNum = 0;
	if (bInspectOK == true)
	{
		pBSD->filter(eArea,m_tmpSearchDataParam.m_nMinAcceptArea,m_tmpSearchDataParam.m_nMaxAcceptArea,0);
		scBlob * const *pBlobB = pBSD->allBlobs(false);

		nResultBlobNum = pBSD->numBlobs(true); 
		if (nResultBlobNum>=1)
		{
			//计算缺陷面积
			pBSD->sort(eArea,eDescending);
			const scBlob* pBlob = pBSD->getBlob(1);
			double dDefectArea = pBlob->area;
			if(searchResult.m_CircleCornerDistance.m_vBlobArea.size() == 0)
			{
				searchResult.m_CircleCornerDistance.m_vBlobArea.resize(1);
			}
			searchResult.m_CircleCornerDistance.m_vBlobArea[0] = dDefectArea;
			if (dDefectArea > m_tmpSearchDataParam.m_dMaxDefectArea || dDefectArea <= 0.0)//说明有脏污
			{
				searchResult.m_CircleCornerDistance.m_nBlobResult = -1;
				bInspectOK = false;
			}
			else
			{
				bInspectOK = true;
			}
		}

		if (bInspectOK)
		{
			m_GuiContour.SetLineColor(RGB(0,255,0));
		}
		else
		{
			m_GuiContour.SetLineColor(RGB(255,0,0));

		}			
		m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
		m_GuiContour.SetTipTextEnabled(TRUE);
		m_bBlobInspectFinsh = true;
	}
	return bInspectOK;
}

bool CCircleCornerCheckSearchTool::SearchOneObroundBlobInspect(cpImage &ImageSearch,scObround & ResultObround,CSearchResult& searchResult)
{
	//第一个圆
	double dCenterX = (ResultObround.GetCornerPo().GetX() + ResultObround.GetCornerPx().GetX())/2.0;
	double dCenterY = (ResultObround.GetCornerPo().GetY() + ResultObround.GetCornerPx().GetY())/2.0;
	sc2Vector circelCenter1(dCenterX,dCenterY);
	scCircle cirlce1;
	cirlce1.SetCenterRadius(circelCenter1,ResultObround.GetXLength()/2.0);

	if(!SearchOneCircleBlobInspect(ImageSearch,cirlce1,searchResult))
	{
		m_nResultErrNumber  = - 20;
		if(m_tmpSearchDataParam.m_bBlobInspectUseToTool)
		{
			return false;
		}else
		{
			return true;
		}
	}

	//第二个圆
	dCenterX = (ResultObround.GetCornerPy().GetX() + ResultObround.GetCornerPopp().GetX())/2.0;
	dCenterY = (ResultObround.GetCornerPy().GetY() + ResultObround.GetCornerPopp().GetY())/2.0;
	sc2Vector circelCenter2(dCenterX,dCenterY);
	scCircle cirlce2;
	cirlce2.SetCenterRadius(circelCenter2,ResultObround.GetXLength()/2.0);
	if(!SearchOneCircleBlobInspect(ImageSearch,cirlce2,searchResult))
	{
		m_nResultErrNumber  = - 21;
		if(m_tmpSearchDataParam.m_bBlobInspectUseToTool)
		{
			return false;
		}else
		{
			return true;
		}

	}


	// Blob分析
	scBlobTool blobTool;
	blobTool.SetSegmentationHardThresh(m_tmpSearchDataParam.m_nBinaryThre,(m_tmpSearchDataParam.m_nDefectType == 2));
	blobTool.ConnectivityType(scBlobSceneDescription::eGreyScale);
	blobTool.ConnectivityCleanup(scBlobSceneDescription::eFill);
	blobTool.ConnectivityMinPels(m_tmpSearchDataParam.m_nMinAcceptArea);
	scAffineRect affsetRect = ResultObround.GetAffineRect();
	bool bInspectOK = true;
	if (!blobTool.Execute(ImageSearch,&affsetRect))
	{
		return bInspectOK;
	}
	scBlobSceneDescription* pBSD = blobTool.GetBSD();
	if (pBSD == NULL)
	{
		return bInspectOK;
	}	

	//计算Blob个数
	int nResultBlobNum = 0;
	if (bInspectOK == true)
	{
		pBSD->filter(eArea,m_tmpSearchDataParam.m_nMinAcceptArea,m_tmpSearchDataParam.m_nMaxAcceptArea,0);
		scBlob * const *pBlobB = pBSD->allBlobs(false);

		nResultBlobNum = pBSD->numBlobs(true); 
		if (nResultBlobNum>=1)
		{
			//计算缺陷面积
			pBSD->sort(eArea,eDescending);
			const scBlob* pBlob = pBSD->getBlob(1);
			double dDefectArea = pBlob->area;			
			if(searchResult.m_CircleCornerDistance.m_vBlobArea.size() == 0)
			{
				searchResult.m_CircleCornerDistance.m_vBlobArea.resize(1);
			}
			searchResult.m_CircleCornerDistance.m_vBlobArea[0] = dDefectArea;
			if (dDefectArea > m_tmpSearchDataParam.m_dMaxDefectArea || dDefectArea <= 0.0)//说明有脏污
			{
				searchResult.m_CircleCornerDistance.m_nBlobResult = -1;
				bInspectOK = false;
			}
			else
			{
				searchResult.m_CircleCornerDistance.m_nBlobResult = 0;
				bInspectOK = true;
			}
		}

		if (bInspectOK)
		{
			m_GuiContour.SetLineColor(RGB(0,255,0));
		}
		else
		{
			m_GuiContour.SetLineColor(RGB(255,0,0));

		}			
		m_GuiContour.SetContourBoundary(pBlobB,nResultBlobNum,true,true);
		m_GuiContour.SetTipTextEnabled(TRUE);
		m_bBlobInspectFinsh = true;
	}
	return bInspectOK;
}

bool CCircleCornerCheckSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		return false;
	}	
	bool bRet = false;
	// 清空
	ClearResult();
	m_nResultErrNumber = 0;
	SetSearchLineCircleParam();//设置找线和找圆的参数
	m_strErrorDetail = _T("");

	//处理彩图
	cpImage tmpImage;
	//是否图片处理过
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

	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)//区域定位
	{
		// 执行区域初定位搜索
		if(bIsProcessImage)
		{
			bRet = ExecutePatternSerach(tmpImage,&m_PatternSearchTool,&(m_TrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_nSearchNum,m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dConfusionThre,0,0,0);
		}
		else
		{
			bRet = ExecutePatternSerach(ImageSearch,&m_PatternSearchTool,&(m_TrainGuiParam.m_SearchRect),
				m_tmpSearchDataParam.m_nSearchNum,m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dConfusionThre,0,0,0);
		}
		if (bRet)
		{
			// 获取结果
			int nResultNum = m_PatternSearchTool.GetResultNum();
			if (nResultNum <= 0)
			{
				m_nResultErrNumber = -1;
				return false;
			}
			for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
			{
				scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);

				// 求变换矩阵
				sc2Vector pos = tmpPatternResult.GetPosition();
				sc2Vector tmModelPoint;
				double dRotation;
				double dTransX;
				double dTransY;
				m_PatternSearchTool.GetPatternOrigin(tmModelPoint);
				dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
				dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
				dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
				CSearchResult searchResult;
				searchResult.m_resultSearchRect.m_searchRect	= tmpPatternResult.MatchRegion();
				searchResult.m_resultSearchRect.m_bFindRect		= TRUE;
				searchResult.m_dScore							= tmpPatternResult.GetScore();

				CResultSearchRect trainRect;
				trainRect.m_searchRect = tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(trainRect);
					
				// 1.显示矩形
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&searchResult.m_resultSearchRect.m_searchRect,_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

				// 2.执行找线
				scLineSeg ResultLine1,ResultLine2;

				if (bIsProcessImage)
				{
					if(!SearchFindLineFunc(i,tmpImage,dRotation,dTransX,dTransY,searchResult,ResultLine1,ResultLine2))
					{
						m_SearchResultArray.push_back(searchResult);
						return false;
					}
				}
				else
				{
					if(!SearchFindLineFunc(i,ImageSearch,dRotation,dTransX,dTransY,searchResult,ResultLine1,ResultLine2))
					{
						m_SearchResultArray.push_back(searchResult);
						return false;
					}
				}



					
				// 3.执行找圆+检查+Blob

				if (bIsProcessImage)
				{
					if(!SearchFindCircleFunc(i,tmpImage,dRotation,dTransX,dTransY,ResultLine1,ResultLine2,searchResult))
					{
						m_SearchResultArray.push_back(searchResult);	
						return false;
					}
				}
				else
				{
					if(!SearchFindCircleFunc(i,ImageSearch,dRotation,dTransX,dTransY,ResultLine1,ResultLine2,searchResult))
					{
						m_SearchResultArray.push_back(searchResult);	
						return false;
					}
				}

				m_SearchResultArray.push_back(searchResult);		
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
			m_nResultErrNumber=-1;
			return false;
		}
	}

	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)//角定位
	{
		// 执行搜索
		if(bIsProcessImage)
		{
			bRet = ExecuteQuickSearch(tmpImage,&m_QuickSearchTool,&(m_TrainGuiParam.m_SearchRect),m_tmpSearchDataParam.m_nSearchNum, \
				m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dAngleLow,m_tmpSearchDataParam.m_dAngleHigh,1, \
				1,0,0,0);
		}
		else
		{
			bRet = ExecuteQuickSearch(ImageSearch,&m_QuickSearchTool,&(m_TrainGuiParam.m_SearchRect),m_tmpSearchDataParam.m_nSearchNum, \
				m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dAngleLow,m_tmpSearchDataParam.m_dAngleHigh,1, \
				1,0,0,0);
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
				CSearchResult searchResult;
				searchResult.m_resultSearchRect.m_searchRect	= tmpQuickResult.MatchRegion();
				searchResult.m_resultSearchRect.m_bFindRect		= TRUE;
				searchResult.m_dScore							= tmpQuickResult.GetScore();

				CResultSearchRect resRect;
				resRect.m_searchRect = tmpQuickResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);
	
				// 1.显示矩形
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&searchResult.m_resultSearchRect.m_searchRect,_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

				// 2.执行找线
				scLineSeg ResultLine1,ResultLine2;

				if (bIsProcessImage)
				{
					if(!SearchFindLineFunc(i,tmpImage,dRotation,dTransX,dTransY,searchResult,ResultLine1,ResultLine2))
					{
						m_SearchResultArray.push_back(searchResult);
						return false;
					}
				}
				else
				{
					if(!SearchFindLineFunc(i,ImageSearch,dRotation,dTransX,dTransY,searchResult,ResultLine1,ResultLine2))
					{
						m_SearchResultArray.push_back(searchResult);
						return false;
					}
				}

				//if(!SearchFindLineFunc(i,ImageSearch,dRotation,dTransX,dTransY,searchResult,ResultLine1,ResultLine2))
				//{
				//	m_SearchResultArray.push_back(searchResult);
				//	return false;
				//}


				// 3.执行找圆+检查+Blob
				if (bIsProcessImage)
				{
					if(!SearchFindCircleFunc(i,tmpImage,dRotation,dTransX,dTransY,ResultLine1,ResultLine2,searchResult))
					{
						m_SearchResultArray.push_back(searchResult);	
						return false;
					}
				}
				else
				{
					if(!SearchFindCircleFunc(i,ImageSearch,dRotation,dTransX,dTransY,ResultLine1,ResultLine2,searchResult))
					{
						m_SearchResultArray.push_back(searchResult);	
						return false;
					}
				}				
				m_SearchResultArray.push_back(searchResult);		
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
			m_nResultErrNumber=-1;
			return false;
		}
	}
	return bRet;
}

// 获取搜索结果个数										
int CCircleCornerCheckSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CCircleCornerCheckSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板文件
bool CCircleCornerCheckSearchTool::LoadModelFromFile(CString strDir)
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
	CCircleCornerCheckSearchTrainDataParam tmpTrainDataParam;
	CCircleCornerCheckSearchTrainGuiParam tmpTrainGuiParam;
	CCircleCornerCheckSearchSearchDataParam tmpSearchDataParam;
	CCircleCornerCheckSearchSearchGuiParam tmpSearchGuiParam;	
	CCircleCornerCheckSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eCircleCornerSearchCheckInspectTool)
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

	
	bRet = cXMLConfigurator.FindElem(_T("EnableCheck"));
	if (bRet)
	{
		tmpTrainDataParam.m_bEnableCheck = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("SubSearchToolType"));
	if (bRet)
	{
		tmpTrainDataParam.SetSubSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
	}

	bRet = cXMLConfigurator.FindElem(_T("CircleToolType"));
	if (bRet)
	{
		tmpTrainDataParam.SetSubCircleToolType(_ttoi(cXMLConfigurator.GetElemData()));
	}

	// coarse scale
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());

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
		return false;
	}
	tmpTrainDataParam.m_lFilterHalfWidth[0] = _ttol(cXMLConfigurator.GetElemData());
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
	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());

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
		return false;
	}
	tmpTrainDataParam.m_lFilterHalfWidth[1] = _ttol(cXMLConfigurator.GetElemData());
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
	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());


    for (int i=0;i<2;i++)
    {
        CString tmpStr = _T("");
        tmpStr.Format(_T("CornerLineMaskEnable%d"),i);
        bRet = cXMLConfigurator.FindElem(tmpStr);
        if (bRet)
        {
            tmpTrainGuiParam.m_bEnableCornerLineMask[i] = _ttoi(cXMLConfigurator.GetElemData());
        }
    }
	for (int i=0;i<2;i++)
	{
		CString strTmp;
		strTmp.Format(_T("MaskParam%d"),i);
		bRet = cXMLConfigurator.FindElem(strTmp);
		if (bRet)
		{
			cXMLConfigurator.EnterElem();
			bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
			if (!bRet) return false;
			tmpTrainGuiParam.m_vdMaskData_FindLine[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
			for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine[i].size(); j++)
			{
				strTmp.Format(_T("MaskData%d"), j);
				bRet = cXMLConfigurator.FindElem(strTmp);
				if (!bRet) return false;
				tmpTrainGuiParam.m_vdMaskData_FindLine[i].at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
			}
			bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
			if (!bRet) return false;
			tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].resize(_ttoi(cXMLConfigurator.GetElemData()));
			for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].size(); j++)
			{
				strTmp.Format(_T("CaliperMaskData%d"), j);
				bRet = cXMLConfigurator.FindElem(strTmp);
				if (!bRet) return false;
				tmpTrainGuiParam.m_vbCaliperMask_FindLine[i].at(j) = _ttoi(cXMLConfigurator.GetElemData());
			}
			cXMLConfigurator.LeaveElem();
		}
	}

	for (int i = 0; i < CIRCLE_NUM; i++)
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
			return false;
		}
		tmpTrainDataParam.m_lCircleFilterHalfWidth[i] = _ttol(cXMLConfigurator.GetElemData());


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
	}

	CString strTemp = _T("");

	// Obround contrast thre
	strTemp.Format(_T("ObroundContrastThre"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dObroundContrastThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	//Obround FilterHalfWidth
	strTemp.Format(_T("ObroundFilterHalfWidth"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lObroundFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());


	// Obroundoutlinenum
	strTemp.Format(_T("ObroundOutLineNum"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nObroundOutLineNum = _ttol(cXMLConfigurator.GetElemData());


	// Obroundedgeproperty
	strTemp.Format(_T("ObroundEdgeProperty"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeObroundProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());


	// ObroundSortByScore
	strTemp.Format(_T("ObroundSortByScore"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bObroundSortByScore = _ttoi(cXMLConfigurator.GetElemData());


	// Train GUI rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
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
	tmpTrainGuiParam.m_TrainRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_TrainRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

	bRet = cXMLConfigurator.FindElem(_T("TrainRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("OriginX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("OriginY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainGuiParam.m_TrainRect1.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("SizeX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("SizeY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainGuiParam.m_TrainRect1.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}	
	
	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}	
	
	tmpTrainGuiParam.m_TrainAffineRect.SetCenter(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("Skew"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
	}
	
	cXMLConfigurator.LeaveElem();


	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}	

	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}	

	tmpTrainGuiParam.m_TrainAffineRect1.SetCenter(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect1.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}

	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect1.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}

	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect1.SetXRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
	}

	bRet = cXMLConfigurator.FindElem(_T("Skew"));
	if (bRet)
	{
		tmpTrainGuiParam.m_TrainAffineRect1.SetSkew(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));	
	}

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

	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect1.GetOrigin().GetX());
	bRet = cXMLConfigurator.FindElem(_T("OriginX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("OriginY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainGuiParam.m_SearchRect1.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("SizeX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("SizeY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainGuiParam.m_SearchRect1.SetSize(vPos);
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


	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
	bRet = cXMLConfigurator.FindElem(_T("CenterX"));
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("CenterY"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainGuiParam.m_ModelPoint1 = vPos;
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

	for (int i = 0; i < CIRCLE_NUM; i++)
	{
		CString strTemp;

		strTemp.Format(_T("CircleCenter%d"), i);
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


		// Search Dirction
		strTemp.Format(_T("SearchDir%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());
	}


	strTemp.Format(_T("ObroundCenter"));
	// Obround center
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("ObroundCenterX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("ObroundCenterY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	tmpTrainGuiParam.m_ObroundCenter = vPos;
	cXMLConfigurator.LeaveElem();


	// Obround Rotation  
	strTemp.Format(_T("ObroundRotation"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dObroundRotation = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	// Obround SideXLen  
	strTemp.Format(_T("ObroundSideXLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dObroundSideXLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	// ObroundSideYLen  
	strTemp.Format(_T("ObroundSideYLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dObroundSideYLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	//Obround Caliper Num 
	strTemp.Format(_T("ObroundCaliperNum"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nObroundCaliperNum = _ttoi(cXMLConfigurator.GetElemData());

	//Obround Project Len
	strTemp.Format(_T("ObroundProjectLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dObroundCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	//Obround Search Len
	strTemp.Format(_T("ObroundSearchLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dObroundCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	//Obround Search Dirction
	strTemp.Format(_T("ObroundSearchDir"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_ObroundCaliperSearchDirection = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());

	// Obround
	bRet = cXMLConfigurator.FindElem(_T("Obround"));
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
	tmpTrainGuiParam.m_Obround.SetCenter(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_Obround.SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_Obround.SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("XRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_Obround.SetRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
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

	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dConfusionThre = (tmpSearchDataParam.m_dAcceptThre + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre + 0.1);
	}
	else
	{
		tmpSearchDataParam.m_dConfusionThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	

	bRet = cXMLConfigurator.FindElem(_T("SearchNum1"));
	if (bRet)
	{
		tmpSearchDataParam.m_nSearchNum1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	bRet = cXMLConfigurator.FindElem(_T("AcceptThre1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAcceptThre1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	

	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre1"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dConfusionThre1 = (tmpSearchDataParam.m_dAcceptThre1 + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre1 + 0.1);
	}
	else
	{
		tmpSearchDataParam.m_dConfusionThre1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}


	bRet = cXMLConfigurator.FindElem(_T("AngleLow1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAngleLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleHigh1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dAngleHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeLow1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeHigh1"));
	if (bRet)
	{
		tmpSearchDataParam.m_dSizeHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("SizeCheckUseToTool"));
	if (bRet)
	{
		tmpSearchDataParam.m_bSizeCheckUseToTool = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableSizeCheck"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableSizeCheck = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DistCheckUseToTool"));
	if (bRet)
	{
		tmpSearchDataParam.m_bDistCheckUseToTool = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableDistCheck"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableDistCheck = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableDispLineSeg"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableDispLineSeg[0] = _ttoi(cXMLConfigurator.GetElemData());
	}
	bRet = cXMLConfigurator.FindElem(_T("EnableDispLineSeg1"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEnableDispLineSeg[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// FitPoint1
	BYTE byR, byG, byB;
	bRet = cXMLConfigurator.FindElem(_T("FitPoint1"));
	if (!bRet)	return false;
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("Visible1"));
	if (!bRet)	return false;
	tmpSearchGuiParam.m_bVisibleFitPoint1 = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("Visible2"));
	if (!bRet)	return false;
	tmpSearchGuiParam.m_bVisibleFitPoint2 = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorOK_R"));
	if (!bRet)	return false;
	byR = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorOK_G"));
	if (!bRet)	return false;
	byG = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorOK_B"));
	if (!bRet)	return false;
	byB = _ttoi(cXMLConfigurator.GetElemData());
	tmpSearchGuiParam.m_cLineColorOKFitPoint1 = RGB(byR, byG, byB);
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_R"));
	if (!bRet)	return false;
	byR = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_G"));
	if (!bRet)	return false;
	byG = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_B"));
	if (!bRet)	return false;
	byB = _ttoi(cXMLConfigurator.GetElemData());
	tmpSearchGuiParam.m_cLineColorNGFitPoint1 = RGB(byR, byG, byB);
	cXMLConfigurator.LeaveElem();

	bRet = cXMLConfigurator.FindElem(_T("DistMeasureSearchDataParam"));
	if (bRet)
	{
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("PixelRatio"));
		if (bRet)
		{
			tmpSearchDataParam.m_dPixelRatio = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("nDistanceType"));
		if (bRet)
		{
			tmpSearchDataParam.m_nDistanceType =  _ttoi(cXMLConfigurator.GetElemData());
		}

		for (int i=0;i < DIST_MEASURE_NUMS; i++)
		{
			CString tmpStr = _T("");
			tmpStr.Format(_T("Tool%d"),i);
			bRet = cXMLConfigurator.FindElem(tmpStr);
			if(bRet)
			{
				cXMLConfigurator.EnterElem();
				/////////////////////////
				bRet = cXMLConfigurator.FindElem(_T("EnableFlu"));
				if (bRet)
				{
					tmpSearchDataParam.m_bEnableFlu[i] = _ttoi(cXMLConfigurator.GetElemData());
				}
				else
				{
					tmpSearchDataParam.m_bEnableFlu[i] = false;
				}

				bRet = cXMLConfigurator.FindElem(_T("DistanceBase"));
				if (bRet)
				{
					tmpSearchDataParam.m_dDistanceBase[i] = _tcstod(cXMLConfigurator.GetElemData(),NULL);
				}
				else
				{
					tmpSearchDataParam.m_dDistanceBase[i] = 1;
				}

				bRet = cXMLConfigurator.FindElem(_T("DistanceFlu"));
				if (bRet)
				{
					tmpSearchDataParam.m_dDistanceFlu[i] = _tcstod(cXMLConfigurator.GetElemData(),NULL);
				}
				else
				{
					tmpSearchDataParam.m_dDistanceFlu[i] = 1;
				}
				////////////////////
// 				bRet = cXMLConfigurator.FindElem(_T("EnableMax"));
// 				if (bRet)
// 				{
// 					tmpSearchDataParam.m_bEnableMax1[i] = _ttoi(cXMLConfigurator.GetElemData());
// 				}
// 
// 				bRet = cXMLConfigurator.FindElem(_T("DistanceMax"));
// 				if (bRet)
// 				{
// 					tmpSearchDataParam.m_dDistanceMax1[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 				}
// 
// 				bRet = cXMLConfigurator.FindElem(_T("EnableMin"));
// 				if (bRet)
// 				{
// 					tmpSearchDataParam.m_bEnableMin1[i] = _ttoi(cXMLConfigurator.GetElemData());
// 				}
// 
// 				bRet = cXMLConfigurator.FindElem(_T("DistanceMin"));
// 				if (bRet)
// 				{
// 					tmpSearchDataParam.m_dDistanceMin1[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 				}

				bRet = cXMLConfigurator.FindElem(_T("EnableOKNGDecision"));
				if (bRet)
				{
					tmpSearchDataParam.m_bEnableInspectOKNG[i] = _ttoi(cXMLConfigurator.GetElemData());
				}

				bRet = cXMLConfigurator.FindElem(_T("InspectDistanceValueLabel"));
				if (bRet)
				{
					tmpSearchDataParam.m_strInspectDistanceValueLabel[i] = CString(cXMLConfigurator.GetElemData());
				}

				cXMLConfigurator.LeaveElem();
			}
		}

		cXMLConfigurator.LeaveElem();
	}

	bRet = cXMLConfigurator.FindElem(_T("BlobInspectUseToTool"));
	if (bRet)
	{
		tmpSearchDataParam.m_bBlobInspectUseToTool = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EanbleBlobInspect"));
	if (bRet)
	{
		tmpSearchDataParam.m_bEanbleBlobInspect = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DefectType"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nDefectType = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("BinaryThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nBinaryThre = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MinAcceptArea"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nMinAcceptArea = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MaxAcceptArea"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nMaxAcceptArea = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MaxDefectArea"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dMaxDefectArea = _ttof(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("AreaScaleRatio"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAreaScaleRatio = _ttof(cXMLConfigurator.GetElemData());

	for(int i = 0;i < CIRCLE_NUM;i++)
	{
		CString tmpStr = _T("");
		tmpStr.Format(_T("EnableShowCirclePoint%d"),i);
		bRet = cXMLConfigurator.FindElem(tmpStr);
		if (!bRet)
		{
			return false;
		}
		tmpSearchGuiParam.m_bEnableShowCirclePoint[i] = _ttol(cXMLConfigurator.GetElemData());
	}


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
	if (bRet)
	{
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	bRet = cXMLConfigurator.FindElem(_T("ModelY1"));
	if (bRet)
	{
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	}
	
	tmpTrainResult.m_dModelX1 = vPos.GetX();
	tmpTrainResult.m_dModelY1 = vPos.GetY();

	

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

	for (int i = 0; i < CIRCLE_NUM; i++)
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



	strTemp.Format(_T("ObroundCenterX"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	strTemp.Format(_T("ObroundCenterY"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	tmpTrainResult.m_dObroundCenterX = vPos.GetX();
	tmpTrainResult.m_dObroundCenterY = vPos.GetY();

	strTemp.Format(_T("ObroundRotation"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dObroundRotation = _tcstod(cXMLConfigurator.GetElemData(), NULL);


	strTemp.Format(_T("ObroundSideXLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dObroundSideXLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	strTemp.Format(_T("ObroundSideYLen"));
	bRet = cXMLConfigurator.FindElem(strTemp);
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dObroundSideYLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	scImageGray tmpImageModel;
	scImageGray tmpImageMask;
	//scImageGray tmpImageMask1;
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

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	//m_tmpImageTrain1 = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	
	//m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
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
	//m_ImageMask1		=tmpImageMask1;
	m_bTrainOK = true;
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	}

	if (!bRet)
	{
		return false;
	}
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CCircleCornerCheckSearchTool::SaveModelToFile(CString strDir)
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

	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCheck);
	cXMLConfigurator.AddElem(_T("EnableCheck"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.GetSubSearchToolType());
	cXMLConfigurator.AddElem(_T("SubSearchToolType"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.GetCircleToolType());
	cXMLConfigurator.AddElem(_T("CircleToolType"), strInfo);

	// CoarseScale
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CoarseScale);
	cXMLConfigurator.AddElem(_T("CoarseScale"), strInfo);
	// Accuracy
	strInfo.Format(_T("%d"), m_TrainDataParam.m_Accuracy);
	cXMLConfigurator.AddElem(_T("Accuracy"), strInfo);
	// Angle Low
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	// Angle High
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);

	// grau
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau);
	cXMLConfigurator.AddElem(_T("Grau"), strInfo);
	// noise thre
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre);
	cXMLConfigurator.AddElem(_T("NoiseThre"), strInfo);
	// total mag
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag);
	cXMLConfigurator.AddElem(_T("TotalMag"), strInfo);

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
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[1]);
	cXMLConfigurator.AddElem(_T("SortByScore2"), strInfo);
	// DualEdge2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[1]);
	cXMLConfigurator.AddElem(_T("DualEdge2"), strInfo);

	//圆的训练参数
	for (int i = 0; i < CIRCLE_NUM; i++)
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
	}

	CString strTemp = _T("");
	// contrastthre
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dObroundContrastThre);
	strTemp.Format(_T("ObroundContrastThre"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// fileterhalfwidth
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lObroundFilterHalfWidth);
	strTemp.Format(_T("ObroundFilterHalfWidth"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nObroundOutLineNum);
	strTemp.Format(_T("ObroundOutLineNum"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeObroundProperty);
	strTemp.Format(_T("ObroundEdgeProperty"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bObroundSortByScore);
	strTemp.Format(_T("ObroundSortByScore"));
	cXMLConfigurator.AddElem(strTemp, strInfo);



	// TrainGui
    for (int i=0;i<2;i++)
    {
        strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableCornerLineMask[i]);
        CString tmpStr = _T("");
        tmpStr.Format(_T("CornerLineMaskEnable%d"),i);
        cXMLConfigurator.AddElem(tmpStr, strInfo);
    }
	for (int i=0; i<2; i++)
	{
		CString strTmp;
		strTmp.Format(_T("MaskParam%d"),i);
		cXMLConfigurator.DeleteElem(strTmp);
		cXMLConfigurator.AddElem(strTmp);
		cXMLConfigurator.EnterElem();
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdMaskData_FindLine[i].size());
		cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vdMaskData_FindLine[i].size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdMaskData_FindLine[i].at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask_FindLine[i].size());
		cXMLConfigurator.AddElem(_T("CaliperMaskDataNum"), strInfo);
		for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask_FindLine[i].size(); j++)
		{
			strTmp.Format(_T("CaliperMaskData%d"), j);
			strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask_FindLine[i].at(j));
			cXMLConfigurator.AddElem(strTmp, strInfo);
		}
		cXMLConfigurator.LeaveElem();
	}


	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam//////////////
	// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	// 	cXMLConfigurator.EnterElem();


	// train GUI　rect
	cXMLConfigurator.AddElem(_T("TrainRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();


	cXMLConfigurator.AddElem(_T("TrainRect1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect1.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect1.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect1.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect1.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();

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

	// train affine rect
	cXMLConfigurator.AddElem(_T("TrainAffineRect1"));
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

	// search rect
	cXMLConfigurator.AddElem(_T("SearchRect1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect1.GetSize().GetY());
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

	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
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

	//圆训练GUI参数
	for (int i = 0; i < CIRCLE_NUM; i++)
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

		// Search Dircetion
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_CircleCaliperSearchDirection[i]);
		strTemp.Format(_T("SearchDir%d"), i);
		cXMLConfigurator.AddElem(strTemp, strInfo);
	}



	// Obround center		
	strTemp.Format(_T("ObroundCenter"));
	cXMLConfigurator.AddElem(strTemp);
	cXMLConfigurator.EnterElem();

	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ObroundCenter.GetX());
	cXMLConfigurator.AddElem(_T("ObroundCenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ObroundCenter.GetY());
	cXMLConfigurator.AddElem(_T("ObroundCenterY"), strInfo);
	cXMLConfigurator.LeaveElem();

	// Obround Rotation
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dObroundRotation);
	strTemp.Format(_T("ObroundRotation"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// X边长度
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dObroundSideXLen);
	strTemp.Format(_T("ObroundSideXLen"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// Y边长度
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dObroundSideYLen);
	strTemp.Format(_T("ObroundSideYLen"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// Obround Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nObroundCaliperNum);
	strTemp.Format(_T("ObroundCaliperNum"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	//Obround Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dObroundCaliperProjectLen);
	strTemp.Format(_T("ObroundProjectLen"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// Obround Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dObroundCaliperSearchLen);
	strTemp.Format(_T("ObroundSearchLen"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// Obround Search Dircetion
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_ObroundCaliperSearchDirection);
	strTemp.Format(_T("ObroundSearchDir"));
	cXMLConfigurator.AddElem(strTemp, strInfo);

	// Obround
	cXMLConfigurator.AddElem(_T("Obround"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround.GetCenter().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround.GetCenter().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround.GetXLength());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround.GetYLength());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround.GetRotation().ToDouble());
	cXMLConfigurator.AddElem(_T("XRotation"), strInfo);
	cXMLConfigurator.LeaveElem();

	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre);
	cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum1);
	cXMLConfigurator.AddElem(_T("SearchNum1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre1);
	cXMLConfigurator.AddElem(_T("AcceptThre1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre1);
	cXMLConfigurator.AddElem(_T("ConfusionThre1"), strInfo);

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleLow1);
	cXMLConfigurator.AddElem(_T("AngleLow1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAngleHigh1);
	cXMLConfigurator.AddElem(_T("AngleHigh1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeLow1);
	cXMLConfigurator.AddElem(_T("SizeLow1"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSizeHigh1);
	cXMLConfigurator.AddElem(_T("SizeHigh1"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bSizeCheckUseToTool);
	cXMLConfigurator.AddElem(_T("SizeCheckUseToTool"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableSizeCheck);
	cXMLConfigurator.AddElem(_T("EnableSizeCheck"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bDistCheckUseToTool);
	cXMLConfigurator.AddElem(_T("DistCheckUseToTool"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableDistCheck);
	cXMLConfigurator.AddElem(_T("EnableDistCheck"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableDispLineSeg[0]);
	cXMLConfigurator.AddElem(_T("EnableDispLineSeg"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableDispLineSeg[1]);
	cXMLConfigurator.AddElem(_T("EnableDispLineSeg1"), strInfo);

	// FitPoint1
	cXMLConfigurator.AddElem(_T("FitPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bVisibleFitPoint1);
	cXMLConfigurator.AddElem(_T("Visible1"), strInfo);
	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bVisibleFitPoint2);
	cXMLConfigurator.AddElem(_T("Visible2"), strInfo);
	strInfo.Format(_T("%d"), GetRValue(m_SearchGuiParam.m_cLineColorOKFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorOK_R"), strInfo);
	strInfo.Format(_T("%d"), GetGValue(m_SearchGuiParam.m_cLineColorOKFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorOK_G"), strInfo);
	strInfo.Format(_T("%d"), GetBValue(m_SearchGuiParam.m_cLineColorOKFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorOK_B"), strInfo);
	strInfo.Format(_T("%d"), GetRValue(m_SearchGuiParam.m_cLineColorNGFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorNG_R"), strInfo);
	strInfo.Format(_T("%d"), GetGValue(m_SearchGuiParam.m_cLineColorNGFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorNG_G"), strInfo);
	strInfo.Format(_T("%d"), GetBValue(m_SearchGuiParam.m_cLineColorNGFitPoint1));
	cXMLConfigurator.AddElem(_T("LineColorNG_B"), strInfo);
	cXMLConfigurator.LeaveElem();

	//测量工具
	cXMLConfigurator.AddElem(_T("DistMeasureSearchDataParam"));
	cXMLConfigurator.EnterElem();

	strInfo.Format("%f",m_SearchDataParam.m_dPixelRatio);
	cXMLConfigurator.AddElem(_T("PixelRatio"), strInfo);

	strInfo.Format("%d",m_SearchDataParam.m_nDistanceType);
	cXMLConfigurator.AddElem(_T("nDistanceType"), strInfo);

	for (int i=0;i < DIST_MEASURE_NUMS;i++)
	{
		CString tmpStr = _T("");
		tmpStr.Format(_T("Tool%d"),i);
		cXMLConfigurator.AddElem(tmpStr);
		cXMLConfigurator.EnterElem();
		/////////////////////////
		strInfo.Format("%d",m_SearchDataParam.m_bEnableFlu[i]);
		cXMLConfigurator.AddElem(_T("EnableFlu"), strInfo);

		strInfo.Format("%f",m_SearchDataParam.m_dDistanceBase[i]);
		cXMLConfigurator.AddElem(_T("DistanceBase"), strInfo);

		strInfo.Format("%f",m_SearchDataParam.m_dDistanceFlu[i]);
		cXMLConfigurator.AddElem(_T("DistanceFlu"), strInfo);
		/////////////////////////
// 		strInfo.Format("%d",m_SearchDataParam.m_bEnableMax1[i]);
// 		cXMLConfigurator.AddElem(_T("EnableMax"), strInfo);
// 
// 		strInfo.Format("%f",m_SearchDataParam.m_dDistanceMax1[i]);
// 		cXMLConfigurator.AddElem(_T("DistanceMax"), strInfo);
// 
// 		strInfo.Format("%d",m_SearchDataParam.m_bEnableMin1[i]);
// 		cXMLConfigurator.AddElem(_T("EnableMin"), strInfo);
// 
// 		strInfo.Format("%f",m_SearchDataParam.m_dDistanceMin1[i]);
// 		cXMLConfigurator.AddElem(_T("DistanceMin"), strInfo);

		strInfo.Format("%d",m_SearchDataParam.m_bEnableInspectOKNG[i]);
		cXMLConfigurator.AddElem(_T("EnableOKNGDecision"), strInfo);

		cXMLConfigurator.AddElem(_T("InspectDistanceValueLabel"), m_SearchDataParam.m_strInspectDistanceValueLabel[i]);

		cXMLConfigurator.LeaveElem();

	}
	cXMLConfigurator.LeaveElem();

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bBlobInspectUseToTool);
	cXMLConfigurator.AddElem(_T("BlobInspectUseToTool"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEanbleBlobInspect);
	cXMLConfigurator.AddElem(_T("EanbleBlobInspect"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nDefectType);
	cXMLConfigurator.AddElem(_T("DefectType"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nBinaryThre);
	cXMLConfigurator.AddElem(_T("BinaryThre"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nMinAcceptArea);
	cXMLConfigurator.AddElem(_T("MinAcceptArea"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nMaxAcceptArea);
	cXMLConfigurator.AddElem(_T("MaxAcceptArea"), strInfo);

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dMaxDefectArea);
	cXMLConfigurator.AddElem(_T("MaxDefectArea"), strInfo);

	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAreaScaleRatio);
	cXMLConfigurator.AddElem(_T("AreaScaleRatio"), strInfo);

	for(int i = 0;i < CIRCLE_NUM;i++)
	{
		CString tmpStr = _T("");
		tmpStr.Format(_T("EnableShowCirclePoint%d"),i);
		strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowCirclePoint[i]);
		cXMLConfigurator.AddElem(tmpStr, strInfo);
	}

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


	for (int i = 0; i < CIRCLE_NUM; i++)
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

	/*CString strTemp = _T("");*/

	strTemp.Format(_T("ObroundCenterX"));
	strInfo.Format(_T("%f"), m_TrainResult.m_dObroundCenterX);
	cXMLConfigurator.AddElem(strTemp, strInfo);

	strTemp.Format(_T("ObroundCenterY"));
	strInfo.Format(_T("%f"), m_TrainResult.m_dObroundCenterY);
	cXMLConfigurator.AddElem(strTemp, strInfo);

	strTemp.Format(_T("ObroundSideXLen"));
	strInfo.Format(_T("%f"), m_TrainResult.m_dObroundSideXLen);
	cXMLConfigurator.AddElem(strTemp, strInfo);	

	strTemp.Format(_T("ObroundSideYLen"));
	strInfo.Format(_T("%f"), m_TrainResult.m_dObroundSideYLen);
	cXMLConfigurator.AddElem(strTemp, strInfo);	

	strTemp.Format(_T("ObroundRotation"));
	strInfo.Format(_T("%f"), m_TrainResult.m_dObroundRotation);
	cXMLConfigurator.AddElem(strTemp, strInfo);	



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
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	}

	if (!bRet)
	{
		return false;
	}

	return true;
}	

bool CCircleCornerCheckSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
    if(nWidth<1||nHeight<1)return false;
    if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

    //训练区域
    double centerx,centery, xlen,ylen, rot,skew;

    m_GuiTrainRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    m_GuiTrainAffineRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
    m_GuiTrainAffineRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

    m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
    m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);


    //卡尺
    double sx,sy,ex,ey;
    m_GuiFindLine[0].GetStartXYEndXY(sx,sy,ex,ey);
    m_GuiFindLine[0].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
    m_GuiFindLine[1].GetStartXYEndXY(sx,sy,ex,ey);
    m_GuiFindLine[1].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

    double plength,slength;
    plength = m_GuiFindLine[0].GetCaliperProjectionLength();
    slength = m_GuiFindLine[0].GetCaliperSearchLength();
    m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    plength = m_GuiFindLine[1].GetCaliperProjectionLength();
    slength = m_GuiFindLine[1].GetCaliperSearchLength();
    m_GuiFindLine[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindLine[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);


	//找圆
	double rad,asta,aspan;
	m_GuiFindCircle_1[0].GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
	m_GuiFindCircle_1[0].SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

	m_GuiFindCircle_1[1].GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
	m_GuiFindCircle_1[1].SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

	//double plength,slength;
	plength = m_GuiFindCircle_1[0].GetCaliperProjectionLength();
	slength = m_GuiFindCircle_1[0].GetCaliperSearchLength();
	m_GuiFindCircle_1[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindCircle_1[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	plength = m_GuiFindCircle_1[1].GetCaliperProjectionLength();
	slength = m_GuiFindCircle_1[1].GetCaliperSearchLength();
	m_GuiFindCircle_1[1].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindCircle_1[1].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	//长圆
	m_GuiFindObround.GetCenterLengthsRotation(centerx,centery, xlen,ylen, rot);
	m_GuiFindObround.SetCenterLengthsRotation((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot);
	//double plength,slength;
	plength = m_GuiFindObround.GetCaliperProjectionLength();
	slength = m_GuiFindObround.GetCaliperSearchLength();
	m_GuiFindObround.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindObround.SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    m_CamHeight = nHeight;
    m_CamWidth = nWidth;
    return true;
}

// 更新训练控件数据信息	
bool CCircleCornerCheckSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect.GetAffineRect();


		for (int i=0; i<LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);

			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[i]);

		}

				CString strTemp = _T("");
		if(m_tmpTrainDataParam.m_nCircleToolType < 3)//圆
		{
			for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
			{
				m_tmpTrainGuiParam.m_nCircleCaliperNum[i]		= m_GuiFindCircle_1[i].GetCaliperNum();

				m_GuiFindCircle_1[i].GetCenterRadiusAngleStartAngleSpan(
					m_tmpTrainGuiParam.m_CircleCenter_1[i], m_tmpTrainGuiParam.m_dCircleRadius[i],
					m_tmpTrainGuiParam.m_CircleArcStart[i], m_tmpTrainGuiParam.m_CircleArcSpan[i]);

				m_tmpTrainGuiParam.m_dCircleCaliperProjectLen[i] = m_GuiFindCircle_1[i].GetCaliperProjectionLength();
				m_tmpTrainGuiParam.m_dCircleCaliperSearchLen[i]	= m_GuiFindCircle_1[i].GetCaliperSearchLength();
				m_tmpTrainGuiParam.m_CircleCaliperSearchDirection[i] = (CaliperSearchDirection)m_GuiFindCircle_1[i].GetCaliperSearchDirection();
			}
		}
		else if(m_tmpTrainDataParam.m_nCircleToolType == 3)//长圆
		{
			m_tmpTrainGuiParam.m_nObroundCaliperNum	= m_GuiFindObround.GetCaliperNum();
			m_GuiFindObround.GetCenterLengthsRotation(
				m_tmpTrainGuiParam.m_ObroundCenter,m_tmpTrainGuiParam.m_dObroundSideXLen,
				m_tmpTrainGuiParam.m_dObroundSideYLen,scRadian(m_tmpTrainGuiParam.m_dObroundRotation));
			m_tmpTrainGuiParam.m_dObroundCaliperProjectLen	= m_GuiFindObround.GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dObroundCaliperSearchLen	= m_GuiFindObround.GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_ObroundCaliperSearchDirection = (CaliperSearchDirection)m_GuiFindObround.GetCaliperSearchDirection();
			m_tmpTrainGuiParam.m_Obround						= m_GuiFindObround.GetObround();
		}
	}
	else // 将数据传递到Gui控件上
	{	
		m_tmpTrainGuiParam.m_LineColor = RGB(0,0,255);
		m_tmpTrainGuiParam.m_LineColor1 = RGB(0,0,255);
		m_tmpTrainGuiParam.m_nLineWidth = 1;
		m_tmpTrainGuiParam.m_nLineWidth1 = 1;

		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);

		m_GuiTrainAffineRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainAffineRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainAffineRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

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

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine[i]);

		}


		if(m_tmpTrainDataParam.m_nCircleToolType < 3)//圆
		{
			for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
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
			}
		}else if(m_tmpTrainDataParam.m_nCircleToolType == 3)//长圆
		{
			m_GuiFindObround.SetCaliperNum(m_tmpTrainGuiParam.m_nObroundCaliperNum);
			m_GuiFindObround.SetCenterLengthsRotation(
				m_tmpTrainGuiParam.m_ObroundCenter,m_tmpTrainGuiParam.m_dObroundSideXLen,
				m_tmpTrainGuiParam.m_dObroundSideYLen,scRadian(m_tmpTrainGuiParam.m_dObroundRotation));
			m_GuiFindObround.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dObroundCaliperProjectLen);
			m_GuiFindObround.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dObroundCaliperSearchLen);
			m_GuiFindObround.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_ObroundCaliperSearchDirection);
			m_GuiFindObround.SetObround(m_tmpTrainGuiParam.m_Obround);
			m_GuiFindObround.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindObround.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		}

	}

	return true;
}

bool CCircleCornerCheckSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
			//m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			//m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
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

		for (j=0; j<m_pGuiSearchObroundArray.size(); j++)
		{
			m_pGuiSearchObroundArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchObroundArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
		for (j=0; j<m_pGuiSearchObroundCenterArray.size(); j++)
		{
			m_pGuiSearchObroundCenterArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchObroundCenterArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}

	return true;
}

// 清除控件及其显示					
bool CCircleCornerCheckSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CCircleCornerCheckSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CCircleCornerCheckSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineSegArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineTextArray);

	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint2Array);
	ClearGraphicsArray(m_pGuiSearchResultCircleFitPointArray1);
	ClearGraphicsArray(m_pGuiSearchResultCircleFitPointArray2);

	ClearGraphicsArray(m_pGuiSearchObroundArray);
	ClearGraphicsArray(m_pGuiSearchObroundCenterArray);
	

	m_nResultErrNumber = 0;
	return true;
}

bool CCircleCornerCheckSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindObround"));

		
		int j=0;
		for (j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		CString strTemp = _T("");
		for (int i = 0; i < CIRCLE_NUM; i++)
		{
			strTemp.Format(_T("_FindCircle%d"),i);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText + strTemp);

			strTemp.Format(_T("_TrainResultCircle%d"),i);
			m_pGuiStaticContainer->RemoveItem(m_strKeyText + strTemp);
		}


		m_pGuiStaticContainer->RemoveItem(m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem( m_strKeyText+_T("_TrainResultLine2"));
		if(m_tmpTrainDataParam.m_nCircleToolType < 3)		//圆
		{
			for (int i = 0; i < m_tmpTrainDataParam.m_nCircleToolType; i++)
			{
				strTemp.Format(_T("_TrainResultCircle%d"), i);
				m_pGuiStaticContainer->RemoveItem(m_strKeyText + strTemp);
			}
		}else if(m_tmpTrainDataParam.m_nCircleToolType == 3)//长圆
		{
			strTemp.Format(_T("_TrainResultObround"));
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

bool CCircleCornerCheckSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_GuiContour.GetTipText());

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

		for (j=0; j<m_pGuiSearchLineSegArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineSegArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineTextArray.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineTextArray[j]->GetTipText());
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

		for (j=0; j<m_pGuiSearchResultFitPoint2Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint2Array[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint2Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint2Array[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchResultCircleFitPointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultCircleFitPointArray1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultCircleFitPointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultCircleFitPointArray2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchObroundArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchObroundArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchObroundCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchObroundCenterArray[j]->GetTipText());
		}
		
	
	}

	return true;
}

// 获取训练模板原始图像
bool CCircleCornerCheckSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet=m_PatternSearchTool.GetPatternImage(ImagePattern);
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet=m_QuickSearchTool.GetTrainModelImage(ImagePattern);
	}


	if (!bRet)
	{
		return false;
	}

	return true;
}	
bool CCircleCornerCheckSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetCircleToolType() == ePatternSearchTool)
	{
		//bRet=m_PatternSearchTool1.GetPatternImage(ImagePattern);
	}
	if (m_tmpTrainDataParam.GetCircleToolType() == eQuickSearchTool)
	{
		//bRet=m_QuickSearchTool1.GetTrainModelImage(ImagePattern);
	}


	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 恢复训练参数
bool CCircleCornerCheckSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CCircleCornerCheckSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CCircleCornerCheckSearchTool::SetLanguage(int nLanguage)
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
		m_GuiTrainAffineRect.SetLabel(_T("Train Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

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
}

void CCircleCornerCheckSearchTool::ShowFitPoint(std::vector<scGuiGraphic*> & pGuiInspectFitPointArray,scGuiCoordCross guiInspectFitPoint[MAXPTSNUM],scFindLine* findLineTool,const CString strFitPoint)
{
	std::vector<bool> bUsed;
	std::vector<sc2Vector> points;
	findLineTool->GetUsed(bUsed);
	findLineTool->GetFitData(points);

	for (long m=0; m<bUsed.size(); m++)
	{
		bool bUse = bUsed[m];
		sc2Vector pt = points[m];
		CString strTmpKey = _T("");
		int nIndex = pGuiInspectFitPointArray.size();
		strTmpKey = m_strKeyText + strFitPoint;
		CString tmpIndex = _T("");
		tmpIndex.Format(_T("_%d"), nIndex);
		strTmpKey += tmpIndex;

		scGuiCoordCross *pTempFitPoint1 = guiInspectFitPoint + nIndex;
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
		pGuiInspectFitPointArray.push_back(pTempFitPoint1);	
	}
}

CString CCircleCornerCheckSearchTool::GetResultErrInfo()
{
	CString strInfo = _T("");
	switch(m_nResultErrNumber)
	{
	case 0:
		{
			strInfo = m_strShowInspectInfo;
		}
		break;
	case -1:
		{
			strInfo.Format(_T("Err:%d,粗定位失败"), m_nResultErrNumber);
		}
		break;
	case -2:
		{
			strInfo.Format(_T("Err:%d,找线1失败"),m_nResultErrNumber);
		}
		break;
	case -3:
		{
			strInfo.Format(_T("Err:%d,找线2失败"), m_nResultErrNumber);
		}
		break;
	case -4:
		{
			strInfo.Format(_T("Err:%d,找线1、2失败"), m_nResultErrNumber);
		}
		break;
	case -5:
		{
			strInfo.Format(_T("Err:%d,检测定位失败"), m_nResultErrNumber);
		}
		break;
	case -6:
		{
			strInfo.Format(_T("Err:%d,线距测量1找线失败"), m_nResultErrNumber);
		}
		break;
	case -7:
		{
			strInfo.Format(_T("Err:%d,线距测量1超限失败"), m_nResultErrNumber);
		}
		break;
	case -8:
		{
			strInfo.Format(_T("Err:%d,线距测量2找线失败"), m_nResultErrNumber);
		}
		break;
	case -9:
		{
			strInfo.Format(_T("Err:%d,线距测量2超限失败"), m_nResultErrNumber);
		}
		break;
	case -13:
		{
			strInfo.Format(_T("Err:%d,圆1半径%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -14:
		{
			strInfo.Format(_T("Err:%d,圆2半径%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -15:
		{
			strInfo.Format(_T("Err:%d,圆1，2间距%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -16:
		{
			strInfo.Format(_T("Err:%d,圆心中点与线1%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -17:
		{
			strInfo.Format(_T("Err:%d,圆心中点与线2距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -18:
		{
			strInfo.Format(_T("Err:%d,圆心1与线1距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -19:
		{
			strInfo.Format(_T("Err:%d,圆心1与线2距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -20:
		{
			strInfo.Format(_T("Err:%d,圆心2与线1距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -21:
		{
			strInfo.Format(_T("Err:%d,圆心2与线2距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -30:
		{
			strInfo.Format(_T("Err:%d,圆半径%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -31:
		{
			strInfo.Format(_T("Err:%d,圆心与线1距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -32:
		{
			strInfo.Format(_T("Err:%d,圆心与线2距离%s失败"), m_nResultErrNumber,m_strErrorDetail);
		}
		break;
	case -33:
		{
			strInfo.Format(_T("Err:%d,找圆1失败"), m_nResultErrNumber);
		}
		break;
	case -34:
		{
			strInfo.Format(_T("Err:%d,找圆2失败"), m_nResultErrNumber);
		}
		break;
	case -35:
		{
			strInfo.Format(_T("Err:%d,找长圆失败"), m_nResultErrNumber);
		}
		break;
	default:
		strInfo.Format(_T("Err:%d，其他错误"), m_nResultErrNumber);
		break;
	}
	return strInfo;
}

bool CCircleCornerCheckSearchTool::ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation)
{
	bool bRet = false;
	if(NULL == m_PatternSearchTool || NULL == m_SearchRect)
	{
		return bRet;
	}

	if (!m_PatternSearchTool->SetAcceptNum(m_nSearchNum))
	{
		return bRet;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool->SetAcceptThreshold(m_dAcceptThre))
	{
		return bRet;
	}

	if (!m_PatternSearchTool->SetConfusionThreshold(m_dConfusionThre>1.0 ?1.0:m_dConfusionThre))
	{
		return bRet;
	}

	double dTransCenterX, dTransCenterY;
	double dOriCenterX, dOriCenterY;
	dOriCenterX = m_SearchRect->GetOrigin().GetX();
	dOriCenterY = m_SearchRect->GetOrigin().GetY();
	dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
	dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
	scRect tmpSearch = *m_SearchRect;
	tmpSearch.SetOrigion(sc2Vector(dTransCenterX,dTransCenterY));
	bRet = m_PatternSearchTool->Execute(ImageSearch,tmpSearch);
	return bRet;
}

bool CCircleCornerCheckSearchTool::ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation)
{
	bool bRet = false;
	if(NULL == m_QuickSearchTool || NULL == m_SearchRect)
	{
		return bRet;
	}
	// 设置搜索个数
	if (!m_QuickSearchTool->SetNumToFind(m_nSearchNum))
	{
		return bRet;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool->SetAcceptThreshold(m_dAcceptThre))
	{
		return bRet;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool->SetDofZoneRotate(scDegree(m_dAngleLow), scDegree(m_dAngleHigh), scDegree(360)))
	{
		return bRet;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool->SetDofNominalSize(1.0))
	{
		return bRet;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool->SetDofZoneSize(m_tmpSearchDataParam.m_dSizeLow1,m_tmpSearchDataParam.m_dSizeHigh1,5))
	{
		return bRet;
	}

	
	double dTransCenterX, dTransCenterY;
	double dOriCenterX, dOriCenterY;
	dOriCenterX = m_SearchRect->GetOrigin().GetX();
	dOriCenterY = m_SearchRect->GetOrigin().GetY();
	dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
	dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
	scRect tmpSearch = *m_SearchRect;
	tmpSearch.SetOrigion(sc2Vector(dTransCenterX,dTransCenterY));
	// 执行搜索
	bRet = m_QuickSearchTool->Execute(ImageSearch, tmpSearch);
	return bRet;
}

bool CCircleCornerCheckSearchTool::ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine)
{
	bool bRet = false;
	if(NULL == m_FindLineTool)
	{
		return bRet;
	}

	m_FindLineTool->SetCaliperNum(m_sFindLineTrainData.m_nCaliperNum);
	/*m_FindLineTool->SetCaliperParams(m_sFindLineTrainData.m_nCaliperNum, m_sFindLineTrainData.m_CaliperSearchRotation, m_sFindLineTrainData.m_dCaliperSearchLen
	,m_sFindLineTrainData.m_dCaliperProjectLen);*/

	if (m_sFindLineTrainData.m_bEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_sFindLineTrainData.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_sFindLineTrainData.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindLineTool->SetCaliperProperty(m_sFindLineTrainData.m_dContrastThre, m_sFindLineTrainData.m_edgeProperty,tmpEdgePolarity, m_sFindLineTrainData.m_lFilterHalfWidth);
	}
	else
	{
		m_FindLineTool->SetCaliperProperty(m_sFindLineTrainData.m_dContrastThre, m_sFindLineTrainData.m_edgeProperty, m_sFindLineTrainData.m_lFilterHalfWidth);
	}

	m_FindLineTool->SetOutLineNum(m_sFindLineTrainData.m_nOutLineNum, eRansacRobust);
	m_FindLineTool->EnableCaliperSortByScore(m_sFindLineTrainData.m_bSortByScore);
	m_FindLineTool->SetCaliperMasked(m_sFindLineTrainData.m_vbCaliperMask_FindLine);

	double dTransStartX, dTransStartY;
	double dTransEndX, dTransEndY;

	dTransStartX = cos(dRotation) * m_sFindLineTrainData.m_dStartX  - sin(dRotation) * m_sFindLineTrainData.m_dStartY + dTransX;
	dTransStartY = sin(dRotation) * m_sFindLineTrainData.m_dStartX + cos(dRotation) * m_sFindLineTrainData.m_dStartY + dTransY;
	dTransEndX = cos(dRotation) * m_sFindLineTrainData.m_dEndX  - sin(dRotation) * m_sFindLineTrainData.m_dEndY + dTransX;
	dTransEndY = sin(dRotation) * m_sFindLineTrainData.m_dEndX  + cos(dRotation) * m_sFindLineTrainData.m_dEndY + dTransY;
	
	m_FindLineTool->SetCaliperParams(m_sFindLineTrainData.m_nCaliperNum,m_sFindLineTrainData.m_CaliperSearchRotation, m_sFindLineTrainData.m_dCaliperSearchLen
	,m_sFindLineTrainData.m_dCaliperProjectLen);
	m_FindLineTool->SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);

	double dRMS;
	bRet = m_FindLineTool->Execute(ImageSearch, ResultLine, dRMS);
	return bRet;
}

bool CCircleCornerCheckSearchTool::GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2)
{
	bool bRet = false;
	if(NULL == m_FindLineTool)
	{
		return bRet;
	}

	sc2Vector pointS;
	sc2Vector pointE;

	svStd vector<sc2Vector> pointResult;	// 探测点
	svStd vector<bool> used;				// 参与拟合标志

	m_FindLineTool->GetFitData(pointResult);
	m_FindLineTool->GetUsed(used);

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
		bRet = true;
	}
	else
	{
		return bRet;
	}
	return bRet;
}

void CCircleCornerCheckSearchTool::ShowResultRect(std::vector<scGuiGraphic*>& pGuiSearchRectArray,scGuiAffineRect GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* pAffineRect,const CString strRect,COLORREF color,int LineWidth)
{
	if(NULL == pAffineRect) return;
	//显示矩形
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strRect;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiAffineRect *pTempAffineRect = GuiSearchAffineRect+nOffset;
	pTempAffineRect->SetAffineRect(*pAffineRect);
	pTempAffineRect->SetLineColor(color);
	pTempAffineRect->SetLineWidth(LineWidth);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	pGuiSearchRectArray.push_back(pTempAffineRect);
}

void CCircleCornerCheckSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth)
{
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strCross;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiCross *pTempCross = m_GuiSearchCross+nOffset;
	pTempCross->SetCenterExtent(m_CrossVector, sc2Vector(40,40));
	pTempCross->SetLineColor(color);
	pTempCross->SetLineWidth(LineWidth);
	pTempCross->SetTipTextEnabled(true);
	pTempCross->SetTipText(strTmpKey);
	m_pGuiSearchCrossArray.push_back(pTempCross);
}

void CCircleCornerCheckSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth)
{
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strCross;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiCross *pTempCross = m_GuiSearchCross+nOffset;
	pTempCross->SetCenterExtent(m_CrossVector, sc2Vector(40,40));
	pTempCross->SetLineColor(color);
	pTempCross->SetLineWidth(LineWidth);
	pTempCross->SetTipTextEnabled(true);
	pTempCross->SetTipText(strTmpKey);

	pTempCross->SetLabel(strCrossLabel);
	pTempCross->SetLabelVisible(TRUE);

	m_pGuiSearchCrossArray.push_back(pTempCross);
}
void CCircleCornerCheckSearchTool::ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth)
{
	if(NULL == m_scLine) return;

	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strLine;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiLine *pTemLine = m_GuiSearchLine + nOffset;
	pTemLine->SetLine(*m_scLine);
	pTemLine->SetLineColor(color);
	pTemLine->SetLineWidth(LineWidth);
	pTemLine->SetTipTextEnabled(true);
	pTemLine->SetTipText(strTmpKey);
	m_pGuiSearchLineArray.push_back(pTemLine);
}

void CCircleCornerCheckSearchTool::ShowResultLineSeg(std::vector<scGuiGraphic*>& pGuiSearchLineArray,scGuiLineSeg m_GuiSearchLine[MAXRESNUM],const int nOffset,scLineSeg * cLineSeg,const CString strLine,COLORREF color,int LineWidth)
{
	if(NULL == cLineSeg) return;

	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strLine;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiLineSeg *pTemLine = m_GuiSearchLineSeg + nOffset;
	pTemLine->SetLineSeg(*cLineSeg);
	pTemLine->SetLineColor(color);
	pTemLine->SetLineWidth(LineWidth);
	pTemLine->SetTipTextEnabled(true);
	pTemLine->SetTipText(strTmpKey);
	pGuiSearchLineArray.push_back(pTemLine);
}

void CCircleCornerCheckSearchTool::ShowResultLineSegText(std::vector<scGuiGraphic*>& pGuiSearchLineArray,scGuiLineSeg m_GuiSearchLine[MAXRESNUM],const int nOffset,scLineSeg * cLineSeg,double dInspectDistance,const CString strLine,COLORREF color,int LineWidth)
{
	// GUI 显示测量结果
	CString StrTmpDis1 = _T("");
	StrTmpDis1.Format(_T("%.3fmm"),dInspectDistance);
	StrTmpDis1 = m_tmpSearchDataParam.m_strInspectDistanceValueLabel[nOffset] + StrTmpDis1;

	scGuiText* pGuiText = m_GuiSearchLineText + nOffset;
	pGuiText->SetInteractive(FALSE);
	double centerX = (cLineSeg->GetP1().GetX() +cLineSeg->GetP2().GetX())/2.0;
	double centerY = (cLineSeg->GetP1().GetY() +cLineSeg->GetP2().GetY())/2.0;
	CString strTmpKey;
	strTmpKey.Format(_T("%s_InspectResultDistanceText_%d"), m_strKeyText, nOffset);
	pGuiText->SetVisible(TRUE);
	pGuiText->SetXYRotation(sc2Vector(centerX,centerY),scRadian(0));
	pGuiText->SetText(StrTmpDis1);
	pGuiText->SetLineColor(color);
	pGuiText->SetTipText(strTmpKey);
	pGuiText->SetTipTextEnabled(TRUE);
	pGuiSearchLineArray.push_back(pGuiText);
}

// 加载模板训练图像
bool CCircleCornerCheckSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	scRect rRect2;
	bool bRet;
	CCornerCheckSearchTrainDataParam tmpTrainDataParam;
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
	if (cSearchToolType != eCircleCornerSearchCheckInspectTool)
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


	bRet = cXMLConfigurator.FindElem(_T("SubSearchToolType"));
	if (bRet)
	{
		tmpTrainDataParam.SetSubSearchToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
	}


	// Train rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
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
	rRect.SetOrigion(vPos);
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
	rRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

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
	rRect2.SetWidth(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	rRect2.SetHeight(_tcstod(cXMLConfigurator.GetElemData(), NULL));
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
	rRect2.SetOrigion(sc2Vector(vPos.GetX()-rRect2.GetSize().GetX()/2,vPos.GetY()-rRect2.GetSize().GetY()/2));
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


	if (tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		if(cpImage::statusSuccess == tmpImageModel.GetSubImage(ImagePattern,rRect2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	return true;
}
