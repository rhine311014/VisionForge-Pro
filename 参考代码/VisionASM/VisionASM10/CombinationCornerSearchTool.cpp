
// PatternCornerSearchTool.cpp: implementation of the CPatternCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombinationCornerSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"
#include <iostream>
using namespace std;


void CCombinationCornerSearchTool::Initialize_Member_Variables()
{
	m_strShowInspectInfo = _T("");
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;

	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiTrainAffineRect.SetCenterLengthsRotationSkew(350, 240, 100, 100, 0, 0);
	m_GuiTrainAffineRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);


	m_GuiTrainRect1.SetCenterWidthHeight(400, 290, 100, 100);
	m_GuiTrainRect1.SetLabelVisible(TRUE);

	m_GuiTrainAffineRect1.SetCenterLengthsRotationSkew(400, 290, 100, 100,0,0);
	m_GuiTrainAffineRect1.SetLabelVisible(TRUE);

	m_GuiSearchRect1.SetCenterWidthHeight(400, 290, 652, 380);
	m_GuiSearchRect1.SetLabelVisible(TRUE);

	m_GuiModelPoint1.SetCenterXYWidthHeight(400, 290, 40, 40);
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	m_GuiTrainFeaturelet.SetLineColor(RGB(0,255,0));
	m_GuiTrainFeaturelet.SetLineWidth(2);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiTrainAffineRect.SetLabel(_T("Train Area"));

		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint1.SetLabel(_T("Check Reference Point"));
		m_GuiTrainAffineRect1.SetLabel(_T("Check Train Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("角定位工具参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));


		m_GuiTrainRect1.SetLabel(_T("FPC Mark训练区域"));
		m_GuiSearchRect1.SetLabel(_T("FPC Mark搜索区域"));
		m_GuiModelPoint1.SetLabel(_T("FPC Mark参考点"));
		m_GuiTrainAffineRect1.SetLabel(_T("几何FPC Mark训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("角定位工具参考点" ));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

		m_GuiTrainRect1.SetLabel(_T("FPC Mark训练区域"));
		m_GuiSearchRect1.SetLabel(_T("FPC Mark搜索区域"));
		m_GuiModelPoint1.SetLabel(_T("FPC Mark参考点"));
		m_GuiTrainAffineRect1.SetLabel(_T("几何FPC Mark训练区域"));
		break;
	}

	m_GuiFindLine[0].SetStartXYEndXY(270, 130, 420, 130);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(240, 320, 240, 170);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(FALSE);
	m_GuiFindLine[1].SetAffineRectFindLineVisible(TRUE);

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

	CString str;
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	switch (m_iLanguage)
	{
	case 0:
		str.Format(_T("InspectTool%dFindLine%d"), 1,1);
		str1.Format(_T("InspectTool%dFindLine%d"), 1,2);
		str2.Format(_T("InspectTool%dFindLine%d"), 2,1);
		str3.Format(_T("InspectTool%dFindLine%d"), 2,2);
		str4.Format(_T("InspectTool%dFindLine%d"), 3,1);
		str5.Format(_T("InspectTool%dFindCircle%d"), 3,1);
		break;
	case 1:
		str.Format(_T("线距工具%d找线%d"),1,1);
		str1.Format(_T("线距工具%d找线%d"),1,2);
		str2.Format(_T("线距工具%d找线%d"),2,1);
		str3.Format(_T("线距工具%d找线%d"),2,2);
		str4.Format(_T("检测工具%d找线%d"),3,1);
		str5.Format(_T("检测工具%d找圆%d"),3,1);
		break;
	default:
		str.Format(_T("线距工具%d找线%d"),1,1);
		str1.Format(_T("线距工具%d找线%d"),1,2);
		str2.Format(_T("线距工具%d找线%d"),2,1);
		str3.Format(_T("线距工具%d找线%d"),2,2);
		str4.Format(_T("检测工具%d找线%d"),3,1);
		str5.Format(_T("检测工具%d找圆%d"),3,1);
		break;
	}


	CreateGUID(m_strKeyText);
}

CCombinationCornerSearchTool::CCombinationCornerSearchTool(void)
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

	// FPC Mark的定位训练参数
	m_tmpTrainDataParam.m_Accuracy1 = m_PatternSearchTool1.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale1			= m_PatternSearchTool1.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow1				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh1			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable1			= false; // 默认不采用掩模图像


	m_tmpTrainDataParam.m_dGrau1	= m_QuickSearchTool1.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre1 = m_QuickSearchTool1.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag1 = m_QuickSearchTool1.GetTotalGradMagThre();

	//训练区GUI参数

	m_tmpTrainGuiParam.m_TrainRect = m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint	= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect	= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth	= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor = m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_TrainAffineRect = m_GuiTrainAffineRect.GetAffineRect();

	m_tmpTrainGuiParam.m_TrainRect1 = m_GuiTrainRect1.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint1	= m_GuiModelPoint1.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect1	= m_GuiSearchRect1.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth1	= m_GuiTrainRect1.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor1 = m_GuiTrainRect1.GetLineColor();
	m_tmpTrainGuiParam.m_TrainAffineRect1 = m_GuiTrainAffineRect1.GetAffineRect();

	int i = 0;
	for (i=0; i<LINE_NUM; i++)
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
	m_tmpSearchDataParam.m_bLine2ForceVerticalLine1=FALSE;
	

	m_tmpSearchGuiParam.m_nLineWidth	= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor	= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CCombinationCornerSearchTool::CCombinationCornerSearchTool(CCombinationCornerSearchTool &cCombinationCornerSearchTool)
{
	if (cCombinationCornerSearchTool.GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return;
	}

	Initialize_Member_Variables();

	int i=0;


	// 赋值
	m_CamHeight								= cCombinationCornerSearchTool.m_CamHeight;
	m_CamWidth								= cCombinationCornerSearchTool.m_CamWidth;
	m_TrainDataParam						= cCombinationCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinationCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinationCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinationCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCombinationCornerSearchTool.m_TrainResult;
	m_ImageTrain							= cCombinationCornerSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinationCornerSearchTool.m_ImageMask;
	m_ImageMask1							= cCombinationCornerSearchTool.m_ImageMask1;
	m_bTrainGuiShow							= cCombinationCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cCombinationCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCombinationCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinationCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinationCornerSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cCombinationCornerSearchTool.UpdateSearchGuiData(true);
	cCombinationCornerSearchTool.UpdateTrainGuiData(true);


	SetTrainDataParam(&cCombinationCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinationCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinationCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinationCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinationCornerSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinationCornerSearchTool.m_tmpImageMask);
	SetMaskImage2(cCombinationCornerSearchTool.m_tmpImageMask1);


	if (m_bTrainOK)
	{
		bool bRet=false;
		m_bTrainOK = false;
		if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		{
			m_PatternSearchTool =cCombinationCornerSearchTool.m_PatternSearchTool;
			m_bTrainOK = cCombinationCornerSearchTool.m_bTrainOK;
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

			//// 设置掩模图像
			//if (m_TrainDataParam.m_bMaskEnable)
			//{
			//	bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
			//		m_TrainGuiParam.m_ModelPoint.GetY());
			//	m_bTrainOK = bRet;
			//}
			//else
			//{
			//	bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
			//		m_TrainGuiParam.m_ModelPoint.GetY());
			//	m_bTrainOK = bRet;
			//}
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

		if ( m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
		{
			m_PatternSearchTool1 = cCombinationCornerSearchTool.m_PatternSearchTool1;
			m_bTrainOK = cCombinationCornerSearchTool.m_bTrainOK;
		}

		if ( m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
			{
				return;
			}

			if (!m_QuickSearchTool1.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
			{
				return;
			}

			if (!m_QuickSearchTool1.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
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
				tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
				GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

				tmpImageMask.CreateImageBuffer(m_ImageMask1.Width(),m_ImageMask1.Height(),epGray8);
				GetProcessImage(&m_tmpTrainDataParam,m_ImageMask1,tmpImageMask);
			}		

			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable1)
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool1.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
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
				m_bTrainOK = bRet;
			}
		}
	}
}

CCombinationCornerSearchTool& CCombinationCornerSearchTool::operator=(CCombinationCornerSearchTool &cCombinationCornerSearchTool)
{
	if (this == &cCombinationCornerSearchTool)
	{
		return *this;	
	}

	if (cCombinationCornerSearchTool.GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	Initialize_Member_Variables();



	// 赋值
	m_CamHeight								= cCombinationCornerSearchTool.m_CamHeight;
	m_CamWidth								= cCombinationCornerSearchTool.m_CamWidth;
	m_TrainDataParam						= cCombinationCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinationCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinationCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinationCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCombinationCornerSearchTool.m_TrainResult;

	m_ImageTrain							= cCombinationCornerSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinationCornerSearchTool.m_ImageMask;
	m_ImageMask1							= cCombinationCornerSearchTool.m_ImageMask1;
	m_bTrainGuiShow							= cCombinationCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cCombinationCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCombinationCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinationCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinationCornerSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cCombinationCornerSearchTool.UpdateSearchGuiData(true);
	cCombinationCornerSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCombinationCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinationCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinationCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinationCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinationCornerSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinationCornerSearchTool.m_tmpImageMask);
	SetMaskImage2(cCombinationCornerSearchTool.m_tmpImageMask1);


	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		{
			m_PatternSearchTool = cCombinationCornerSearchTool.m_PatternSearchTool;
			m_bTrainOK = cCombinationCornerSearchTool.m_bTrainOK;
		}

		if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
			{
				return *this;;
			}

			if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
			{
				return *this;;
			}

			if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
			{
				return *this;;
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
			bool bRet = false;
			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable)
			{

				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool.Train(tmpImage, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				m_bTrainOK = bRet;
			}
		}

		if ( m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
		{
			m_PatternSearchTool1 = cCombinationCornerSearchTool.m_PatternSearchTool1;
			m_bTrainOK = cCombinationCornerSearchTool.m_bTrainOK;
		}

		if ( m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool1.SetGranularity(m_TrainDataParam.m_dGrau1))
			{
				return *this;;
			}

			if (!m_QuickSearchTool1.SetNoiseThre(m_TrainDataParam.m_lNoiseThre1))
			{
				return *this;;
			}

			if (!m_QuickSearchTool1.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag1))
			{
				return *this;;
			}

			//// 设置掩模图像
			//if (m_TrainDataParam.m_bMaskEnable1)
			//{
			//	bool bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
			//		m_TrainGuiParam.m_ModelPoint1.GetY());
			//	m_bTrainOK = bRet;
			//}
			//else
			//{
			//	bool bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
			//		m_TrainGuiParam.m_ModelPoint1.GetY());
			//	m_bTrainOK = bRet;
			//}
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

				tmpImageMask.CreateImageBuffer(m_ImageMask1.Width(),m_ImageMask1.Height(),epGray8);
				GetProcessImage(&m_tmpTrainDataParam,m_ImageMask1,tmpImageMask);
			}		
			bool bRet = false;
			// 设置掩模图像
			if (m_TrainDataParam.m_bMaskEnable1)
			{

				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool1.Train(tmpImage, tmpImageMask, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				else
				{
					bRet = m_QuickSearchTool1.Train(m_ImageTrain, m_ImageMask1, m_TrainGuiParam.m_TrainAffineRect1, m_TrainGuiParam.m_ModelPoint1.GetX(),
						m_TrainGuiParam.m_ModelPoint1.GetY());
				}
				m_bTrainOK = bRet;
			}
			else
			{
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
				m_bTrainOK = bRet;
			}
		}
	}

	return *this;	
}

CCombinationCornerSearchTool::~CCombinationCornerSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CCombinationCornerSearchTool::GetSearchToolType()
{
	return eCombinationCornerSearchTool;
}

// 设置显示控件指针
bool CCombinationCornerSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
bool CCombinationCornerSearchTool::SetTrainResultDisplay(scGuiDisplay *pGuiDisplay)
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
bool CCombinationCornerSearchTool::SetTrainResultDisplay2(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CCombinationCornerSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CCombinationCornerSearchTool::GetTrainResultDisplay()
{
	return m_pGuiTrainResultDisplay;
}

// 获取训练结果显示控件指针
scGuiDisplay* CCombinationCornerSearchTool::GetTrainResultDisplay2()
{
	return m_pGuiTrainResultDisplay2;
}

// 设置Gui图形的关键字														
bool CCombinationCornerSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CCombinationCornerSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CCombinationCornerSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CCombinationCornerSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CCombinationCornerSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CCombinationCornerSearchTool::UpdateTrainGuiDisplay()
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
	}
	if ( m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
	{
		m_GuiTrainRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible1  );
		m_GuiModelPoint1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible1  );
		m_GuiSearchRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible1  );
		m_GuiTrainAffineRect1.SetVisible(FALSE);
	} 
	if ( m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
	{
		m_GuiTrainAffineRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible1  );
		m_GuiModelPoint1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible1  );
		m_GuiSearchRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible1  );
		m_GuiTrainRect1.SetVisible(FALSE);
	}



	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);


	for (int i=0; i<LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible&& m_tmpTrainDataParam.m_bEnableCorner);
		if (m_tmpTrainGuiParam.m_bEnableCornerLineMask[i])
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

		//	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		//	{
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		//}

		//	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
		//{
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect, m_strKeyText+_T("_TrainAffRect"));
		//}


		//	if (  m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
		//{
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect1, m_strKeyText+_T("_TrainRect1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint1, m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect1, m_strKeyText+_T("_SearchRect1"));
		//	} 
		//	if (  m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
		//{
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainAffineRect1, m_strKeyText+_T("_TrainAffRect1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint1, m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect1, m_strKeyText+_T("_SearchRect1"));
		//}

		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	

		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}




		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	{
		scFeatureletChainSet tmpscGLCS;
		bool bFalg = m_QuickSearchTool.GetTrainFeaturesFine(tmpscGLCS);
		if(bFalg)
		{
			m_GuiTrainFeaturelet.SetFeaturelet(tmpscGLCS);
			m_GuiTrainFeaturelet.SetVisible(m_bTrainOK && (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool));
			if(m_pGuiTrainResultDisplay && ::IsWindow(m_hTrainResultDispWnd))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet ,m_strKeyText +_T("_FeatureChain"));

				m_pGuiTrainResultDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay->MyInvalidate();
			}
		}
	}
	{
		scFeatureletChainSet tmpscGLCS;
		bool bFalg = m_QuickSearchTool1.GetTrainFeaturesFine(tmpscGLCS);
		if(bFalg)
		{
			m_GuiTrainFeaturelet2.SetFeaturelet(tmpscGLCS);
			m_GuiTrainFeaturelet2.SetVisible(m_bTrainOK && (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool));
			if(m_pGuiTrainResultDisplay2 && ::IsWindow(m_hTrainResultDispWnd2))
			{
				m_pGuiInteractiveContainer = m_pGuiTrainResultDisplay2->GetInteractiveContainer();
				m_pGuiInteractiveContainer->AddItem(&m_GuiTrainFeaturelet2 ,m_strKeyText +_T("_FeatureChain2"));

				m_pGuiTrainResultDisplay2->SetInteractiveContainer(m_pGuiInteractiveContainer);
				m_pGuiTrainResultDisplay2->MyInvalidate();
			}
		}
	}
	return true;
}

bool CCombinationCornerSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow&& m_tmpTrainDataParam.m_bEnableCorner);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow&& m_tmpTrainDataParam.m_bEnableCorner);



	//if (m_tmpTrainDataParam.m_bEnableCheck)
	//{
	//	
	//}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CCombinationCornerSearchTool::UpdateSearchResultGuiDisplay()							
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





	for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
	{
		m_pGuiSearchLineTextArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint2Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bVisibleFitPoint2);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bVisibleFitPoint1);
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




			for (i=0; i<m_pGuiSearchLineTextArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineTextArray[i], m_pGuiSearchLineTextArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array[i], m_pGuiSearchResultFitPoint1Array[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchResultFitPoint2Array.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint2Array[i], m_pGuiSearchResultFitPoint2Array[i]->GetTipText());
			}


		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CCombinationCornerSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CCombinationCornerSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CCombinationCornerSearchTrainDataParam*)pTrainDataParam);

	return true;
}

bool CCombinationCornerSearchTool::SetTrainDataParamForce(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	m_TrainDataParam = *((CCombinationCornerSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CCombinationCornerSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	*((CCombinationCornerSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CCombinationCornerSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CCombinationCornerSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CCombinationCornerSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CCombinationCornerSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CCombinationCornerSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CCombinationCornerSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CCombinationCornerSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}
bool CCombinationCornerSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask1 = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CCombinationCornerSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

bool CCombinationCornerSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask1;

	return true;
}

// 执行训练											
bool CCombinationCornerSearchTool::Train()
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

		tmpImageMask1.CreateImageBuffer(m_tmpImageMask1.Width(),m_tmpImageMask1.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask1,tmpImageMask1);
	}		

	if (1)
	{
		if (m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
		{
			// 设置训练参数
			if (!m_PatternSearchTool1.SetAccuracy(m_tmpTrainDataParam.m_Accuracy1))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			if (!m_PatternSearchTool1.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale1))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 必须在训练时设置角度
			if (fabs(m_tmpTrainDataParam.m_dAngleLow1 - m_tmpTrainDataParam.m_dAngleHigh1) < DBL_EPSILON) // 相同时需设置成Nominal
			{
				if (!m_PatternSearchTool1.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow1)))
				{
					m_nResultErrNumber = -1;
					return false;
				}
			}
			else if (!m_PatternSearchTool1.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow1), scDegree(m_tmpTrainDataParam.m_dAngleHigh1), scDegree(360)))
			{
				m_nResultErrNumber = -1;
				return false;
			}

			// 设置掩模图像
			if (m_tmpTrainDataParam.m_bMaskEnable1)
			{
				bool bRet = false;
				if(bIsProcessImage)
				{
					bRet = m_PatternSearchTool1.Train(tmpImage, tmpImageMask1, m_tmpTrainGuiParam.m_ModelPoint1,
						&m_tmpTrainGuiParam.m_TrainRect1);
				}
				else
				{
					bRet = m_PatternSearchTool1.Train(m_tmpImageTrain, m_tmpImageMask1, m_tmpTrainGuiParam.m_ModelPoint1,
						&m_tmpTrainGuiParam.m_TrainRect1);
				}
				m_bTrainOK = bRet;

				if (!bRet)
				{
					m_nResultErrNumber=-1;       //检查区域定位失败
					return false;
				}
				m_ImageMask1      = m_tmpImageMask1;

			}
			else
			{
				bool bRet = false;
				if(bIsProcessImage)
				{
					bRet = m_PatternSearchTool1.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint1,
						&m_tmpTrainGuiParam.m_TrainRect1);
				}
				else
				{
					bRet = m_PatternSearchTool1.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint1,
						&m_tmpTrainGuiParam.m_TrainRect1);
				}
				/*		if (m_PatternSearchTool1.SaveTrainModelToBinaryFile( _T("E:\\天马code\\123ruanjian -1#34正用+增加功能\\Win64\\Bin\\VSConfig\\Model1.pts")))
				{
				AfxMessageBox(_T("保存成功"));
				}
				else
				{
				AfxMessageBox(_T("保存失败"));
				}*/
				m_bTrainOK = bRet;

				if (!bRet)
				{
					m_nResultErrNumber=-1;       //检查区域定位失败
					return false;
				}
			}
		}

		if (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
		{
			// 设置训练参数
			if (!m_QuickSearchTool1.SetGranularity(m_tmpTrainDataParam.m_dGrau1))
			{
				m_nResultErrNumber = -2;
				return false;
			}

			if (!m_QuickSearchTool1.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre1))
			{
				m_nResultErrNumber = -2;
				return false;
			}

			if (!m_QuickSearchTool1.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag1))
			{
				m_nResultErrNumber = -2;
				return false;
			}

			// 设置掩模图像
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
				if (!bRet)
				{
					m_nResultErrNumber=-2;       //检查几何定位失败
					return false;
				}
				m_ImageMask1      = m_tmpImageMask1;

			}
			else
			{
				bool bRet = false;
				if(bIsProcessImage)
				{
					bRet = m_QuickSearchTool1.Train(tmpImage, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint1.GetY());	
				}
				else
				{
					bRet = m_QuickSearchTool1.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect1, m_tmpTrainGuiParam.m_ModelPoint1.GetX(),
						m_tmpTrainGuiParam.m_ModelPoint1.GetY());	
				}

				m_bTrainOK = bRet;
				if (!bRet)
				{
					m_nResultErrNumber=-2;       //检查几何定位失败
					return false;
				}

			}
		}
	}


	// 设置训练参数
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
		{
			m_nResultErrNumber = -3;
			return false;
		}

		if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
		{
			m_nResultErrNumber = -3;
			return false;
		}

		// 必须在训练时设置角度
		if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
		{
			if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
			{
				m_nResultErrNumber = -3;
				return false;
			}
		}
		else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
		{
			m_nResultErrNumber = -3;
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
				m_nResultErrNumber=-3;       //角定位区域定位失败
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			if(m_tmpTrainDataParam.m_bEnableCorner)
			{
				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);

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
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
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
					m_nResultErrNumber=-5;       //角定位找线1失败
					return false;
				}

				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);

				//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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

					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
					m_nResultErrNumber=-6;       //角定位找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-7;       //角定位求交点失败
					return false;
				}

				m_tmpTrainResult.m_dModelX = vectArray.at(0).GetX();
				m_tmpTrainResult.m_dModelY = vectArray.at(0).GetY();

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();



				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_ImageMask      = m_tmpImageMask;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
			}else//不启用线定位
			{

				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_ImageMask      = m_tmpImageMask;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;

			}
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
				m_nResultErrNumber=-3;       //角定位区域定位失败
				return false;
			}

			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			if(m_tmpTrainDataParam.m_bEnableCorner)
			{

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
				//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
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
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);

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
					m_nResultErrNumber=-5;       //角定位找线1失败
					return false;
				}

				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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

					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
					m_nResultErrNumber=-6;       //角定位找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-7;       //角定位求交点失败
					return false;
				}

				// 将交点作为模型参考点
				m_tmpTrainResult.m_dModelX = vectArray.at(0).GetX();
				m_tmpTrainResult.m_dModelY =  vectArray.at(0).GetY();

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();



				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
			}
			else
			{


				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;

			}

			return true;
		}

	}


	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
		{
			m_nResultErrNumber = -4;
			return false;
		}

		if (!m_QuickSearchTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
		{
			m_nResultErrNumber = -4;
			return false;
		}

		if (!m_QuickSearchTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
		{
			m_nResultErrNumber = -4;
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
				m_nResultErrNumber=-4;       //角定位几何定位失败
				return false;
			}
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			if(m_tmpTrainDataParam.m_bEnableCorner)
			{

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
				//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
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
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				} 
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
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
					m_nResultErrNumber=-5;       //角定位找线1失败
					return false;
				}
				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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

					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
					m_nResultErrNumber=-6;       //角定位找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-7;       //角定位求交点失败
					return false;
				}
				m_tmpTrainResult.m_dModelX = vectArray.at(0).GetX();
				m_tmpTrainResult.m_dModelY = vectArray.at(0).GetY();

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();



				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_ImageMask      = m_tmpImageMask;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
			}else
			{
				m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
				m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

				//执行线距检测工具的找线训练


				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
			}
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
				m_nResultErrNumber=-4;       //角定位几何定位失败
				return false;
			}
			sc2Vector cModelOrigin;
			m_QuickSearchTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			if(m_tmpTrainDataParam.m_bEnableCorner)
			{

				// 找线0
				m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
				m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
				//	m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
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

					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				else
				{
					m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
				}
				m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[0]);
				m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
				m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
					m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);

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
					m_nResultErrNumber=-5;       //角定位找线1失败
					return false;
				}
				// 找线1
				m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
				m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
				//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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

					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				else
				{
					m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
				}
				m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine[1]);
				m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
				m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
					m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
					m_nResultErrNumber=-6;       //角定位找线2失败
					return false;
				}

				// 求交点
				bool bInterSect = false;
				std::vector<sc2Vector> vectArray;
				bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
				if (!bRet || !bInterSect || vectArray.size() <= 0)
				{
					m_nResultErrNumber=-7;       //角定位求交点失败
					return false;
				}

				m_tmpTrainResult.m_dModelX = vectArray.at(0).GetX();
				m_tmpTrainResult.m_dModelY = vectArray.at(0).GetY();

				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dStartX[1] = cLineSeg2.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[1] = cLineSeg2.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[1] = cLineSeg2.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[1] = cLineSeg2.GetP2().GetY();



				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
				m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
				m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
			}else
			{




				m_bTrainOK       = true;
				m_ImageTrain     = m_tmpImageTrain;
				m_TrainDataParam = m_tmpTrainDataParam;
				m_TrainGuiParam	 = m_tmpTrainGuiParam;
				m_TrainResult    = m_tmpTrainResult;
			}
			return true;
		}
	}

	return false;
}


// 获取训练结果图像														
bool CCombinationCornerSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.GetTrainModelImage(m_ImageTrainResult);
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

bool CCombinationCornerSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	bool bRet = false;
	if (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool  )
	{
		bRet = m_QuickSearchTool1.GetTrainModelImage(m_ImageTrainResult1);
	}
	if (m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool  )
	{
		bRet = m_PatternSearchTool1.GetPatternImage(m_ImageTrainResult1);
	}


	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult1;

	return true;
}

// 获取训练结果
bool CCombinationCornerSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	*((CCombinationCornerSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CCombinationCornerSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CCombinationCornerSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CCombinationCornerSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CCombinationCornerSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	*((CCombinationCornerSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CCombinationCornerSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CCombinationCornerSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CCombinationCornerSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eCombinationCornerSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CCombinationCornerSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}


// 执行搜索 					
bool CCombinationCornerSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	
	bool bRet = false;
	// 清空
	ClearResult();
	m_nResultErrNumber = 0;
	m_strShowInspectInfo = _T("");
	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));


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

	scAffineRect affineCheckRect;

	double Tool2PosX;//工具2的参考点
	double Tool2PosY;//工具2的参考点

	// 先防呆检查 再定位
	if (  m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
	{
		// 位置修正
		CSearchResult searchResult;
		// 执行搜索
		if(bIsProcessImage)
		{
			bRet = ExecutePatternSerach(tmpImage,&m_PatternSearchTool1,&(m_TrainGuiParam.m_SearchRect1),m_tmpSearchDataParam.m_nSearchNum,m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dConfusionThre,0,0,0);
		}
		else
		{
			bRet = ExecutePatternSerach(ImageSearch,&m_PatternSearchTool1,&(m_TrainGuiParam.m_SearchRect1),m_tmpSearchDataParam.m_nSearchNum,m_tmpSearchDataParam.m_dAcceptThre,m_tmpSearchDataParam.m_dConfusionThre,0,0,0);
		}

		if (bRet)
		{
			// 获取结果
			int nResultNum = m_PatternSearchTool1.GetResultNum();
			if (nResultNum <= 0 || nResultNum != m_tmpSearchDataParam.m_nSearchNum1)
			{
				m_nResultErrNumber=-1;
				return false;
			}
			for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
			{
				scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool1.GetResult(i);

				affineCheckRect = tmpPatternResult.MatchRegion();
				////显示矩形
				//ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect1,i,&(tmpPatternResult.MatchRegion()),_T("_InspectResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

				////显示参考点
				//ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross3,i,tmpPatternResult.GetPosition(),_T("_InspectResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

				//显示矩形
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect1,i,&(tmpPatternResult.MatchRegion()),_T("_InspectResultRect_"),RGB(0,0,255),m_tmpSearchGuiParam.m_nLineWidth);

				//显示参考点
				ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross3,i,tmpPatternResult.GetPosition(),_T("_InspectResultCross_"),RGB(0,0,255),m_tmpSearchGuiParam.m_nLineWidth);

				Tool2PosX=tmpPatternResult.GetPosition().GetX();
				Tool2PosY=tmpPatternResult.GetPosition().GetY();
				//searchResult.m_vdAuxiliaryPosX[3] = tmpQuickResult.GetPosition().GetX();
				//searchResult.m_vdAuxiliaryPosY[3] = tmpQuickResult.GetPosition().GetY();



				// 对searchResult 初步赋值
				searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
				searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
				searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
				searchResult.m_dScore = tmpPatternResult.GetScore();

				CResultSearchRect resultRect;
				resultRect.m_searchRect = affineCheckRect;
				resultRect.m_bFindRect = TRUE;
				searchResult.m_vTrainSearchRect.push_back(resultRect);
				m_SearchResultArray.push_back(searchResult);
			}	

			m_SearchDataParam = m_tmpSearchDataParam;
			m_SearchGuiParam  = m_tmpSearchGuiParam;
		}
		else
		{
			m_nResultErrNumber=-1;  //检查区域定位失败
			return false;
		}
	} 

	if (  m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
	{
		// 位置修正
		CSearchResult searchResult;
		if(bIsProcessImage)
		{
			bRet = ExecuteQuickSearch(tmpImage,&m_QuickSearchTool1,&(m_TrainGuiParam.m_SearchRect1),m_tmpSearchDataParam.m_nSearchNum1, \
				m_tmpSearchDataParam.m_dAcceptThre1,m_tmpSearchDataParam.m_dAngleLow1,m_tmpSearchDataParam.m_dAngleHigh1,m_tmpSearchDataParam.m_dSizeLow1, \
				m_tmpSearchDataParam.m_dSizeHigh1,0,0,0);
		}
		else
		{
			bRet = ExecuteQuickSearch(ImageSearch,&m_QuickSearchTool1,&(m_TrainGuiParam.m_SearchRect1),m_tmpSearchDataParam.m_nSearchNum1, \
				m_tmpSearchDataParam.m_dAcceptThre1,m_tmpSearchDataParam.m_dAngleLow1,m_tmpSearchDataParam.m_dAngleHigh1,m_tmpSearchDataParam.m_dSizeLow1, \
				m_tmpSearchDataParam.m_dSizeHigh1,0,0,0);
		}

		if (bRet)
		{
			// 获取结果
			int nResultNum = m_QuickSearchTool1.GetResultNum();
			if (nResultNum <= 0 || nResultNum != m_tmpSearchDataParam.m_nSearchNum1)
			{
				m_nResultErrNumber=-2;
				return false;
			}

			for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
			{
				scQuickSearchResult tmpQuickResult = m_QuickSearchTool1.GetResult(i);
				affineCheckRect = tmpQuickResult.MatchRegion();

				////显示矩形
				//ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect1,i,&(tmpQuickResult.MatchRegion()),_T("_InspectResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

				////显示参考点
				//ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross3,i,tmpQuickResult.GetPosition(),_T("_InspectResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
				
				
				//显示矩形
				ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect1,i,&(tmpQuickResult.MatchRegion()),_T("_InspectResultRect_"),RGB(0,0,255),m_tmpSearchGuiParam.m_nLineWidth);

				//显示参考点
				ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross3,i,tmpQuickResult.GetPosition(),_T("_InspectResultCross_"),RGB(0,0,255),m_tmpSearchGuiParam.m_nLineWidth);

				Tool2PosX=tmpQuickResult.GetPosition().GetX();
				Tool2PosY=tmpQuickResult.GetPosition().GetY();



				searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
				searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
				searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
				searchResult.m_dScore = tmpQuickResult.GetScore();

				// 设置搜索矩形/线
				CResultSearchRect resultRect;

				resultRect.m_searchRect = affineCheckRect;
				resultRect.m_bFindRect = TRUE;
				searchResult.m_vTrainSearchRect.push_back(resultRect);
				m_SearchResultArray.push_back(searchResult);

			}
			m_SearchDataParam = m_tmpSearchDataParam;
			m_SearchGuiParam  = m_tmpSearchGuiParam;
		}
		else
		{
			m_nResultErrNumber=-2;  //检查几何定位失败
			return false;
		}
	}

	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		// 位置修正
		CSearchResult searchResult;
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
				m_nResultErrNumber = -3;
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

				searchResult.m_dAngle=tmpPatternResult.GetRotate().ToDouble();
				searchResult.m_dPosX=tmpPatternResult.GetPosition().GetX();
				searchResult.m_dPosY=tmpPatternResult.GetPosition().GetY();
				searchResult.m_dScore=tmpPatternResult.GetScore();

				if(m_tmpTrainDataParam.m_bEnableCorner)
				{
				// 执行角定位找线
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

					// 执行找线
					if(bIsProcessImage)
					{
						bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
					}
					else
					{
						bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
					}
				}

				//拟合点
				if (bFindLine[0])
				{
					CString str = _T("");
					str.Format(_T("%d_FitPoint1_%d"),1,1);
					ShowFitPoint(m_pGuiSearchResultFitPoint1Array,m_GuiSearchResultFitPoint1,&m_FindLineTool[0],str);

				}
				if (bFindLine[1])
				{

					CString str = _T("");
					str.Format(_T("%d_FitPoint2_%d"),2,2);
					ShowFitPoint(m_pGuiSearchResultFitPoint2Array,m_GuiSearchResultFitPoint2,&m_FindLineTool[1],str);
				}

				if (!bFindLine[0] && bFindLine[1])
				{
					m_nResultErrNumber=-5;  //角定位找线1失败
					//	CSearchResult searchResult;
					sc2Vector pointCenter2;
					//计算找线2的中点
					GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);

					searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
					searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
					searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
					searchResult.m_bFindLine2 = TRUE;
					searchResult.m_bFindLine1 = FALSE;
					searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
					searchResult.m_resultSearchRect.m_bFindRect = TRUE;
					m_SearchResultArray.push_back(searchResult);
					return false;
				}
				if (bFindLine[0] && !bFindLine[1])
				{
					m_nResultErrNumber=-6;    //角定位找线2失败
					//	CSearchResult searchResult;
						sc2Vector pointCenter1;
						//计算找线1的中点
						GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);
						searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
						searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
						searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_bFindLine1=TRUE;
                        searchResult.m_bFindLine2=FALSE;
                        searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
                        searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						m_SearchResultArray.push_back(searchResult);
						return false;
					}
					if (!bFindLine[0] && !bFindLine[1])
					{
                        searchResult.m_bFindLine1=FALSE;
                        searchResult.m_bFindLine2=FALSE;
                        searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
                        searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						m_SearchResultArray.push_back(searchResult);
						m_nResultErrNumber=-7;       //角定位求交点失败
						return false;
					}
					// 求交点
					if (bFindLine[0] && bFindLine[1])
					{
						sc2Vector pointCenter1;
						sc2Vector pointCenter2;
						sc2Vector point3;//线2中点到线1的垂足
						sc2Vector vLineCrossPoint;
						bool bIntersect;
						std::vector<sc2Vector> crossPointArray; 

						if(m_tmpSearchDataParam.m_bLine2ForceVerticalLine1)//线1线2强制垂直
						{
							//计算找线2的中点
							GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);
							point3= ResLine[0].GetLine().Project(pointCenter2);
							ResLine[1].Set(pointCenter2,point3);
						}

						if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
							&& crossPointArray.size() > 0)
						{

						//计算找线1的中点
						GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);

						//计算找线2的中点
						GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);

						//		CSearchResult searchResult;
						searchResult.m_dPosX = crossPointArray[0].GetX();
						searchResult.m_dPosY = crossPointArray[0].GetY();
						// 角度
						//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
						double dRotAngle = scDegree((ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()+ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180())/2.0).ToDouble(); 
						searchResult.m_dAngle = dRotAngle;
						searchResult.m_dScore = tmpPatternResult.GetScore();


						searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
						searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

						searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
						searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

						searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

						searchResult.m_bFindLine1 = TRUE;
						searchResult.m_bFindLine2 = TRUE;

						searchResult.m_resultSearchRect.m_searchRect = tmpPatternResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;


						// 设置搜索矩形/线
						CResultSearchRect resultRect;
						resultRect.m_searchRect = tmpPatternResult.MatchRegion();
						resultRect.m_bFindRect = TRUE;
						searchResult.m_vTrainSearchRect.push_back(resultRect);

						/*resultRect.m_searchRect = affineCheckRect;
						searchResult.m_vACFSearchRect.push_back(resultRect);*/


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



						//m_SearchResultArray.push_back(searchResult);

						//显示矩形
						ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpPatternResult.MatchRegion()),_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

						//显示交点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

						//显示线1中点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,i*2+1,pointCenter1,_T("_ResultCross1_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

						//显示线2中点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,i*2+2,pointCenter2,_T("_ResultCross2_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

						for (int l=0; l<2; l++)
						{
							ShowResultLine(m_pGuiSearchLineArray,m_GuiSearchLine,i*2+l,&(ResLine[l].GetLine()),_T("_ResultLine_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}

						//显示交点及详细信息
						CString st;
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,i,crossPointArray[0],_T("_ResultLineCross_"),st,m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);


						//重新根据角定位最终结果 建立变换矩阵;
						sc2Vector tmModelPoint;
						pos = crossPointArray.at(0);

						tmModelPoint.SetX(m_TrainResult.m_dModelX);
						tmModelPoint.SetY(m_TrainResult.m_dModelY);


						scRadian dLine1 = ResLine[0].GetRotation().SignedNorm ()- modelLineSeg1.GetRotation().SignedNorm ();
						scRadian dLine2 = ResLine[1].GetRotation().SignedNorm ()- modelLineSeg2.GetRotation().SignedNorm ();

						//dRotation = ((dLine1).SignedNorm () + (dLine2).SignedNorm ()).ToDouble()/2;


						dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
						dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());


						searchResult.m_vdAuxiliaryPosX[3] = Tool2PosX;
						searchResult.m_vdAuxiliaryPosY[3] = Tool2PosY;




						m_SearchResultArray.push_back(searchResult);
						// 完成线距检测搜索




					}

				}
				}
				else//不启用角定位
					{

						// 设置搜索矩形/线
						CResultSearchRect resultRect;
						resultRect.m_searchRect = tmpPatternResult.MatchRegion();
						resultRect.m_bFindRect = TRUE;
						searchResult.m_vTrainSearchRect.push_back(resultRect);

						searchResult.m_vdAuxiliaryPosX[3] = Tool2PosX;
						searchResult.m_vdAuxiliaryPosY[3] = Tool2PosY;
						//searchResult.m_vdAuxiliaryPosX[3] = 15.5;
						//searchResult.m_vdAuxiliaryPosY[3] = 15.5;
						//显示矩形
						ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpPatternResult.MatchRegion()),_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpPatternResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						
						m_SearchResultArray.push_back(searchResult);
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
			m_nResultErrNumber=-3;  //角定位区域定位失败
			return false;
		}
	}

	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		// 位置修正
		CSearchResult searchResult;
		for (int i=0; i<LINE_NUM; i++)
		{
			m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
			m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
			,m_TrainGuiParam.m_dCaliperProjectLen[i]);

			//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
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
				searchResult.m_dAngle=tmpQuickResult.GetRotate().ToDouble();
				searchResult.m_dPosX=tmpQuickResult.GetPosition().GetX();
				searchResult.m_dPosY=tmpQuickResult.GetPosition().GetY();
				searchResult.m_dScore=tmpQuickResult.GetScore();
				
				if(m_tmpTrainDataParam.m_bEnableCorner)
				{

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

					// 执行找线
					if(bIsProcessImage)
					{
						bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
					}
					else
					{
						bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
					}
				}

				//拟合点
				if (bFindLine[0])
				{
					CString str = _T("");
					str.Format(_T("%d_FitPoint1_%d"),1,1);
					ShowFitPoint(m_pGuiSearchResultFitPoint1Array,m_GuiSearchResultFitPoint1,&m_FindLineTool[0],str);
				}
				if (bFindLine[1])
				{
					CString str = _T("");
					str.Format(_T("%d_FitPoint2_%d"),2,2);
					ShowFitPoint(m_pGuiSearchResultFitPoint2Array,m_GuiSearchResultFitPoint2,&m_FindLineTool[1],str);
				}
				if (!bFindLine[0] && bFindLine[1])
				{
					m_nResultErrNumber=-5;  //角定位找线1失败
					//	CSearchResult searchResult;
					sc2Vector pointCenter2;
					//计算找线2的中点
					GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);
					searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
					searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
					searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
					searchResult.m_bFindLine2 = TRUE;
					searchResult.m_bFindLine1 = FALSE;
					searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_resultSearchRect.m_bFindRect = TRUE;
					m_SearchResultArray.push_back(searchResult);
					return false;
				}
				if (bFindLine[0] && !bFindLine[1])
				{
					m_nResultErrNumber=-6;  //角定位找线2失败
					//	CSearchResult searchResult;
					sc2Vector pointCenter1;
					//计算找线1的中点
					GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);
					searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
					searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
					searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
					searchResult.m_bFindLine1=TRUE;
					searchResult.m_bFindLine2=FALSE;
					searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
					searchResult.m_resultSearchRect.m_bFindRect = TRUE;
					m_SearchResultArray.push_back(searchResult);
					return false;
				}
				if (!bFindLine[0] && !bFindLine[1])
				{
					m_nResultErrNumber=-5;  //角定位找线1失败
					//	CSearchResult searchResult;
                        searchResult.m_bFindLine1=FALSE;
                        searchResult.m_bFindLine2=FALSE;
                        searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
                        searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						m_SearchResultArray.push_back(searchResult);
						return false;
					}

						// 求交点
						if (bFindLine[0] && bFindLine[1])
						{
							sc2Vector pointCenter1;
							sc2Vector pointCenter2;
							sc2Vector point3;//线2中点到线1的垂足


							sc2Vector vLineCrossPoint;
							bool bIntersect;
							std::vector<sc2Vector> crossPointArray; 
							//重构线2
							if(m_tmpSearchDataParam.m_bLine2ForceVerticalLine1)//线1线2强制垂直
							{
								//计算找线2的中点
								GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);
								point3= ResLine[0].GetLine().Project(pointCenter2);
								ResLine[1].Set(pointCenter2,point3);
							}

							if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[1].GetLine(), bIntersect, crossPointArray)
								&& crossPointArray.size() > 0)
							{
								//计算找线1的中点
								GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);

						//计算找线2的中点
						GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);
						//	CSearchResult searchResult;
						searchResult.m_dPosX = crossPointArray[0].GetX();
						searchResult.m_dPosY = crossPointArray[0].GetY();

						// 角度
						//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
						double dRotAngle = scDegree((ResLine[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180()+ResLine[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180())/2.0).ToDouble(); 
						searchResult.m_dAngle = dRotAngle;
						searchResult.m_dScore = tmpQuickResult.GetScore();

						searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
						searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

						searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
						searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

						searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
						searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();

						searchResult.m_bFindLine1 = TRUE;
						searchResult.m_bFindLine2 = TRUE;

						searchResult.m_resultSearchRect.m_searchRect = tmpQuickResult.MatchRegion();
						searchResult.m_resultSearchRect.m_bFindRect = TRUE;
						//m_SearchResultArray.push_back(searchResult);

						// 设置搜索矩形/线
						CResultSearchRect resultRect;
						resultRect.m_searchRect = tmpQuickResult.MatchRegion();
						resultRect.m_bFindRect = TRUE;
						searchResult.m_vTrainSearchRect.push_back(resultRect);


						/*resultRect.m_searchRect = affineCheckRect;
						searchResult.m_vACFSearchRect.push_back(resultRect);*/


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



						//显示矩形
						ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpQuickResult.MatchRegion()),_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

						//显示粗定位参考点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpQuickResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);


						for (int l=0; l<2; l++)
						{
							ShowResultLine(m_pGuiSearchLineArray,m_GuiSearchLine,i*2+l,&(ResLine[l].GetLine()),_T("_ResultLine_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);
						}

						//显示线1中点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,i*2+1,pointCenter1,_T("_ResultCross1_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

						//显示线2中点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross2,i*2+2,pointCenter2,_T("_ResultCross2_"),RGB(255,0,0),m_tmpSearchGuiParam.m_nLineWidth);

						//显示交点及详细信息
						CString st;
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						ShowResultCross(m_pGuiSearchLineCrossArray,m_GuiSearchLineCross,i,crossPointArray[0],_T("_ResultLineCross_"),st,m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

						//重新根据角定位最终结果 建立变换矩阵;
						sc2Vector tmModelPoint;
						pos = crossPointArray.at(0);

						tmModelPoint.SetX(m_TrainResult.m_dModelX);
						tmModelPoint.SetY(m_TrainResult.m_dModelY);

						scRadian dLine1 = ResLine[0].GetRotation().SignedNorm ()- modelLineSeg1.GetRotation().SignedNorm ();
						scRadian dLine2 = ResLine[1].GetRotation().SignedNorm ()- modelLineSeg2.GetRotation().SignedNorm ();

						//dRotation = ((dLine1).SignedNorm () + (dLine2).SignedNorm ()).ToDouble()/2;
						dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
						dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

						searchResult.m_vdAuxiliaryPosX[3] = Tool2PosX;
						searchResult.m_vdAuxiliaryPosY[3] = Tool2PosY;

						m_SearchResultArray.push_back(searchResult);
						// 完成线距检测搜索
					}

				}	
			}else//启用角定位
					{
						// 设置搜索矩形/线
						CResultSearchRect resultRect;
						resultRect.m_searchRect = tmpQuickResult.MatchRegion();
						resultRect.m_bFindRect = TRUE;
						searchResult.m_vTrainSearchRect.push_back(resultRect);

						ShowResultRect(m_pGuiSearchRectArray,m_GuiSearchAffineRect,i,&(tmpQuickResult.MatchRegion()),_T("_ResultRect_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);

						//显示粗定位参考点
						ShowResultCross(m_pGuiSearchCrossArray,m_GuiSearchCross,i,tmpQuickResult.GetPosition(),_T("_ResultCross_"),m_tmpSearchGuiParam.m_LineColor,m_tmpSearchGuiParam.m_nLineWidth);



						searchResult.m_vdAuxiliaryPosX[3] = Tool2PosX;
						searchResult.m_vdAuxiliaryPosY[3] = Tool2PosY;
						m_SearchResultArray.push_back(searchResult);
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
			m_nResultErrNumber=-4;  //角定位几何定位失败
			return false;
		}
	}
	std::cout<<"点1   "<<m_SearchResultArray.at(0).m_dPosX<<"      "<< m_SearchResultArray.at(0).m_dPosY<<endl;
	std::cout<<"点2   "<<m_SearchResultArray.at(0).m_vdAuxiliaryPosX[3]<<"      "<< m_SearchResultArray.at(0).m_vdAuxiliaryPosY[3]<<endl;
	std::cout<<" -----------------------"<<endl;
	return bRet;
}

// 获取搜索结果个数										
int CCombinationCornerSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CCombinationCornerSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}

// 加载模板训练图像
bool CCombinationCornerSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	CCombinationCornerSearchTrainDataParam tmpTrainDataParam;
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
	if (cSearchToolType != eCombinationCornerSearchTool)
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

	bRet = cXMLConfigurator.FindElem(_T("EnableCorner"));
	if (bRet)
	{
		tmpTrainDataParam.m_bEnableCorner = _ttoi(cXMLConfigurator.GetElemData());
	}


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


// 加载模板文件
bool CCombinationCornerSearchTool::LoadModelFromFile(CString strDir)
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
	CCombinationCornerSearchTrainDataParam tmpTrainDataParam;
	CCombinationCornerSearchTrainGuiParam tmpTrainGuiParam;
	CCombinationCornerSearchSearchDataParam tmpSearchDataParam;
	CCombinationCornerSearchSearchGuiParam tmpSearchGuiParam;	
	CCombinationCornerSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eCombinationCornerSearchTool)
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

	bRet = cXMLConfigurator.FindElem(_T("InspectToolType"));
	if (bRet)
	{
		tmpTrainDataParam.SetSubInspectToolType((CSearchToolType)_ttoi(cXMLConfigurator.GetElemData()));
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

	bRet = cXMLConfigurator.FindElem(_T("EnableCorner"));
	if (bRet)
	{
		tmpTrainDataParam.m_bEnableCorner = _ttoi(cXMLConfigurator.GetElemData());
	}


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

	// coarse scale
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale1"));
	if (bRet)
	{
		tmpTrainDataParam.m_CoarseScale1 = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	}

	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy1"));
	if (bRet)
	{
		tmpTrainDataParam.m_Accuracy1 = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleLow1"));
	if (bRet)
	{
		tmpTrainDataParam.m_dAngleLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("AngleHigh1"));
	if (bRet)
	{
		tmpTrainDataParam.m_dAngleHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable1"));
	if (bRet)
	{
		tmpTrainDataParam.m_bMaskEnable1 = _ttoi(cXMLConfigurator.GetElemData());
	}


	// grau
	bRet = cXMLConfigurator.FindElem(_T("Grau1"));
	if (bRet)
	{
		tmpTrainDataParam.m_dGrau1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	// noise thre
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre1"));
	if (bRet)
	{
		tmpTrainDataParam.m_lNoiseThre1 = _ttol(cXMLConfigurator.GetElemData());
	}

	// total mag
	bRet = cXMLConfigurator.FindElem(_T("TotalMag1"));
	if (bRet)
	{
		tmpTrainDataParam.m_lTotalMag1 = _ttol(cXMLConfigurator.GetElemData());
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

	// Caliper Visible1
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[0]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
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

	// Caliper Visible2
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible2"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[1]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[1] = _ttoi(cXMLConfigurator.GetElemData());
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
	// 	bRet = cXMLConfigurator.FindElem(_T("TrainRectVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpTrainGuiParam.m_bTrainRectVisible = _ttoi(cXMLConfigurator.GetElemData());
	// 	// search rect visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpTrainGuiParam.m_bModelPointVisible = _ttoi(cXMLConfigurator.GetElemData());
	// 	// model point visible 
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

	bRet = cXMLConfigurator.FindElem(_T("Line2ForceVerticalLine1"));
	if (bRet)
	{
		tmpSearchDataParam.m_bLine2ForceVerticalLine1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// FitPoint1
	BYTE byR, byG, byB;
	bRet = cXMLConfigurator.FindElem(_T("FitPoint1"));
	if (!bRet)	return false;
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("Visible"));
	if (!bRet)	return false;
	tmpSearchGuiParam.m_bVisibleFitPoint1 = _ttoi(cXMLConfigurator.GetElemData());
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

	// FitPoint2
	bRet = cXMLConfigurator.FindElem(_T("FitPoint2"));
	if (!bRet)	return false;
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("Visible"));
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
	tmpSearchGuiParam.m_cLineColorOKFitPoint2 = RGB(byR, byG, byB);
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_R"));
	if (!bRet)	return false;
	byR = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_G"));
	if (!bRet)	return false;
	byG = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineColorNG_B"));
	if (!bRet)	return false;
	byB = _ttoi(cXMLConfigurator.GetElemData());
	tmpSearchGuiParam.m_cLineColorNGFitPoint2 = RGB(byR, byG, byB);
	cXMLConfigurator.LeaveElem();



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
	// 	bRet = cXMLConfigurator.FindElem(_T("RectVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bRectVisible = _ttoi(cXMLConfigurator.GetElemData());
	// 	// model point visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("ModelPointVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bPointVisible = _ttoi(cXMLConfigurator.GetElemData());
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



	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpImageModel;
	cpImage tmpImageMask;
	cpImage tmpImageMask1;
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

	if (tmpTrainDataParam.m_bMaskEnable1  )
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
	//scPatternSearchRot tmpPatternSearchTool;
	//scQuickSearchTool tmpQuickSearchTool;
	//if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	//{
	//	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	//}
	//if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	//{
	//	bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	//}
	//if (!bRet)
	//{
	//	return false;
	//}

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	m_tmpImageTrain1 = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	if (tmpTrainDataParam.m_bMaskEnable1)
	{
		m_tmpImageMask1 = tmpImageMask1;
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
	m_ImageMask1		=tmpImageMask1;
	m_bTrainOK = true;
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	}

	if (m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool  )
	{
		bRet = m_PatternSearchTool1.LoadTrainModelFromBinaryFile(strDir + _T("Model1.pts"));
		//bRet = m_PatternSearchTool1.LoadTrainModelFromBinaryFile( _T("E:\\天马code\\123ruanjian -1#34正用+增加功能\\Win64\\Bin\\VSConfig\\Model1.pts"));

	}
	if (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool  )
	{
		bRet = m_QuickSearchTool1.LoadTrainModelFromBinaryFile(strDir + _T("Model1.ptq"));
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
bool CCombinationCornerSearchTool::SaveModelToFile(CString strDir)
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




	strInfo.Format(_T("%d"), m_TrainDataParam.GetSubSearchToolType());
	cXMLConfigurator.AddElem(_T("SubSearchToolType"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.GetInspectToolType());
	cXMLConfigurator.AddElem(_T("InspectToolType"), strInfo);

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

	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCorner);
	cXMLConfigurator.AddElem(_T("EnableCorner"), strInfo);

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

	// CoarseScale
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CoarseScale1);
	cXMLConfigurator.AddElem(_T("CoarseScale1"), strInfo);
	// Accuracy
	strInfo.Format(_T("%d"), m_TrainDataParam.m_Accuracy1);
	cXMLConfigurator.AddElem(_T("Accuracy1"), strInfo);
	// Angle Low
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleLow1);
	cXMLConfigurator.AddElem(_T("AngleLow1"), strInfo);
	// Angle High
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleHigh1);
	cXMLConfigurator.AddElem(_T("AngleHigh1"), strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable1);
	cXMLConfigurator.AddElem(_T("MaskEnable1"), strInfo);

	// grau
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dGrau1);
	cXMLConfigurator.AddElem(_T("Grau1"), strInfo);
	// noise thre
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lNoiseThre1);
	cXMLConfigurator.AddElem(_T("NoiseThre1"), strInfo);
	// total mag
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lTotalMag1);
	cXMLConfigurator.AddElem(_T("TotalMag1"), strInfo);

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



	//////////////////////////////////////////////////////////////////////////
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
	//Caliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCaliperVisible[0]);
	cXMLConfigurator.AddElem(_T("CaliperVisible1"), strInfo);

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


	//////////////////////////////////////////////////////////////////////////


	//cXMLConfigurator.LeaveElem();




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

	strInfo.Format(_T("%d"), m_SearchDataParam.m_bLine2ForceVerticalLine1);
	cXMLConfigurator.AddElem(_T("Line2ForceVerticalLine1"), strInfo);

	// FitPoint1
	cXMLConfigurator.AddElem(_T("FitPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bVisibleFitPoint1);
	cXMLConfigurator.AddElem(_T("Visible"), strInfo);
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

	// FitPoint2
	cXMLConfigurator.AddElem(_T("FitPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bVisibleFitPoint2);
	cXMLConfigurator.AddElem(_T("Visible"), strInfo);
	strInfo.Format(_T("%d"), GetRValue(m_SearchGuiParam.m_cLineColorOKFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorOK_R"), strInfo);
	strInfo.Format(_T("%d"), GetGValue(m_SearchGuiParam.m_cLineColorOKFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorOK_G"), strInfo);
	strInfo.Format(_T("%d"), GetBValue(m_SearchGuiParam.m_cLineColorOKFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorOK_B"), strInfo);
	strInfo.Format(_T("%d"), GetRValue(m_SearchGuiParam.m_cLineColorNGFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorNG_R"), strInfo);
	strInfo.Format(_T("%d"), GetGValue(m_SearchGuiParam.m_cLineColorNGFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorNG_G"), strInfo);
	strInfo.Format(_T("%d"), GetBValue(m_SearchGuiParam.m_cLineColorNGFitPoint2));
	cXMLConfigurator.AddElem(_T("LineColorNG_B"), strInfo);
	cXMLConfigurator.LeaveElem();



	// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	///////////////SearchGuiParam//////////////
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
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bRectVisible);
	// 	cXMLConfigurator.AddElem(_T("RectVisible"), strInfo);
	// 	// model point visible 
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bPointVisible);
	// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
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

	if (m_TrainDataParam.m_bMaskEnable1 && m_ImageMask1.IsValid()   )
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

	//////////////////////////存储模板pts文件//////////////////////////
	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	}

	if (m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool  )
	{
		bRet = m_PatternSearchTool1.SaveTrainModelToBinaryFile(strDir + _T("Model1.pts"));
	}
	if (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool  )
	{
		bRet = m_QuickSearchTool1.SaveTrainModelToBinaryFile(strDir + _T("Model1.ptq"));
	}

	if (!bRet)
	{
		return false;
	}

	return true;
}	

bool CCombinationCornerSearchTool::SetGuiByCam(double nWidth,double nHeight)
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
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);


	m_GuiTrainRect1.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiTrainRect1.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiTrainAffineRect1.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
	m_GuiTrainAffineRect1.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	m_GuiSearchRect1.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect1.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiModelPoint1.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint1.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

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




	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

// 更新训练控件数据信息	
bool CCombinationCornerSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainAffineRect.GetAffineRect();

		//m_tmpTrainGuiParam.m_TrainRect1			= m_GuiTrainRect1.GetRect();
		//m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint1.GetCenter();
		//m_tmpTrainGuiParam.m_SearchRect1			= m_GuiSearchRect1.GetRect();
		//m_tmpTrainGuiParam.m_TrainAffineRect1	= m_GuiTrainAffineRect1.GetAffineRect();

		//if (m_tmpTrainDataParam.m_bEnableCheck)
		{
			m_tmpTrainGuiParam.m_TrainRect1			= m_GuiTrainRect1.GetRect();
			m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint1.GetCenter();
			m_tmpTrainGuiParam.m_SearchRect1		= m_GuiSearchRect1.GetRect();

			m_tmpTrainGuiParam.m_TrainAffineRect1 = m_GuiTrainAffineRect1.GetAffineRect();
		}



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
			m_tmpTrainGuiParam.m_bCaliperVisible[i]		=m_GuiFindLine[i].IsCaliperVisible();
		}



	}
	else // 将数据传递到Gui控件上
	{	
		m_tmpTrainGuiParam.m_LineColor = RGB(0,255,0);
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

		//	if (m_tmpTrainDataParam.m_bEnableCheck)
		{
			m_GuiModelPoint1.SetLineColor(m_tmpTrainGuiParam.m_LineColor1);
			m_GuiModelPoint1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth1);
			m_GuiModelPoint1.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint1, m_GuiModelPoint1.GetExtent());
			m_GuiTrainRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor1);
			m_GuiTrainRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth1);
			m_GuiTrainRect1.SetRect(m_tmpTrainGuiParam.m_TrainRect1);

			m_GuiSearchRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor1);
			m_GuiSearchRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth1);
			m_GuiSearchRect1.SetRect(m_tmpTrainGuiParam.m_SearchRect1);

			m_GuiTrainAffineRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor1);
			m_GuiTrainAffineRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth1);
			m_GuiTrainAffineRect1.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect1);
		}


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

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);
		}


	}

	return true;
}

bool CCombinationCornerSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

	}

	return true;
}

// 清除控件及其显示					
bool CCombinationCornerSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CCombinationCornerSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CCombinationCornerSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineTextArray);

	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint2Array);




	m_nResultErrNumber = 0;
	return true;
}

bool CCombinationCornerSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		//	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
		//	{
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		//	}
		//	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
		//	{
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect"));
		//}
		//	if (  m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
		//	{
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect1"));		
		//	}
		//	if (  m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
		//	{
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainAffRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect1"));		
		//	}
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		

		for (j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		for (int i=0;i<INSPCET_TOOL_LINE_NUMS/2;i++)
		{
			CString strKey;
			strKey.Format(_T("%s_InspectTool__FindLine%d"), m_strKeyText, i);
			CString strKey1;
			strKey1.Format(_T("%s_InspectTool__FindLine%d"), m_strKeyText, i+2);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
			m_pGuiInteractiveContainer->RemoveItem(strKey1);
		}
		{
			CString strKey;
			strKey.Format(_T("%s_InspectTool__FindLine%d"), m_strKeyText, 4);
			CString strKey1;
			strKey1.Format(_T("%s_InspectTool__FindCircle"), m_strKeyText);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
			m_pGuiInteractiveContainer->RemoveItem(strKey1);
		}
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));

		for (int i=0;i<INSPCET_TOOL_LINE_NUMS+1;i++)
		{
			CString str = _T("");
			str.Format(_T("%s_TrainResultInspectLine_%d"),m_strKeyText,i);
			m_pGuiStaticContainer->RemoveItem(str);
			if (i == 4)
			{
				m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));
			}
		}

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

bool CCombinationCornerSearchTool::ClearResultGui()
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


		for (j=0; j<m_pGuiSearchLineTextArray.size();j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineTextArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
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
bool CCombinationCornerSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	/*bool bRet = false;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}

	if (m_tmpTrainDataParam.GetSubSearchToolType() == ePatternCornerSearchTool)
	{
		/*bRet=m_PatternSearchTool.GetPatternImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool.GetTrainRectRegion()))
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	if (m_tmpTrainDataParam.GetSubSearchToolType() == eCornerSearchTool)
	{
		//bRet=m_QuickSearchTool.GetTrainModelImage(ImagePattern);
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool.GetTrainRectRegion()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/*if (!bRet)
	{
	return false;
	}*/

	return true;
}	
bool CCombinationCornerSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
	//bool bRet = false;
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}

	if (m_tmpTrainDataParam.GetInspectToolType() == ePatternSearchTool)
	{
		/*bRet=m_PatternSearchTool1.GetPatternImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool1.GetTrainRectRegion()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (m_tmpTrainDataParam.GetInspectToolType() == eQuickSearchTool)
	{
		/*bRet=m_QuickSearchTool1.GetTrainModelImage(ImagePattern);*/
		if(0 == m_tmpImageTrain.GetSubImage(ImagePattern,m_QuickSearchTool1.GetTrainRectRegion()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/*if (!bRet)
	{
	return false;
	}*/

	return true;
}	

// 恢复训练参数
bool CCombinationCornerSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CCombinationCornerSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CCombinationCornerSearchTool::SetLanguage(int nLanguage)
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

		m_GuiTrainRect1.SetLabel(_T("Check Train Area"));
		m_GuiSearchRect1.SetLabel(_T("Check Search Area"));
		m_GuiModelPoint1.SetLabel(_T("Check Reference Point"));
		m_GuiTrainAffineRect1.SetLabel(_T("Check Train Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("屏体 Mark训练区域"));
		m_GuiSearchRect.SetLabel(_T("屏体 Mark搜索区域"));
		m_GuiModelPoint.SetLabel(_T("屏体 Mark角定位工具参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("屏体 Mark训练区域"));


		m_GuiTrainRect1.SetLabel(_T("FPC Mark训练区域"));
		m_GuiSearchRect1.SetLabel(_T("FPC Mark搜索区域"));
		m_GuiModelPoint1.SetLabel(_T("FPC Mark参考点"));
		m_GuiTrainAffineRect1.SetLabel(_T("FPC Mark训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("角定位工具参考点"));
		m_GuiTrainAffineRect.SetLabel(_T("训练区域"));

		m_GuiTrainRect1.SetLabel(_T("FPC Mark训练区域"));
		m_GuiSearchRect1.SetLabel(_T("FPC Mark搜索区域"));
		m_GuiModelPoint1.SetLabel(_T("FPC Mark参考点"));
		m_GuiTrainAffineRect1.SetLabel(_T("几何FPC Mark训练区域"));
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
//scFindCircle
void CCombinationCornerSearchTool::ShowFitPoint(std::vector<scGuiGraphic*> & m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindCircle* m_FindCircleTool,const CString strFitPoint)
{
	std::vector<bool> bUsed;
	std::vector<sc2Vector> points;
	m_FindCircleTool->GetUsed(bUsed);
	m_FindCircleTool->GetFitData(points);
	for (long m=0; m<bUsed.size(); m++)
	{
		bool bUse = bUsed[m];
		sc2Vector pt = points[m];

		CString strTmpKey = _T("");
		int nIndex = m_pGuiInspectFitPointArray.size();
		strTmpKey = m_strKeyText + strFitPoint;
		CString tmpIndex = _T("");
		tmpIndex.Format(_T("_%d"), nIndex);
		strTmpKey += tmpIndex;

		scGuiCoordCross *pTempFitPoint1 = m_GuiInspectFitPoint + nIndex;
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
		m_pGuiInspectFitPointArray.push_back(pTempFitPoint1);	
	}
}
//scFindLine
void CCombinationCornerSearchTool::ShowFitPoint(std::vector<scGuiGraphic*> & m_pGuiInspectFitPointArray,scGuiCoordCross m_GuiInspectFitPoint[MAXPTSNUM],scFindLine* m_FindLineTool,const CString strFitPoint)
{
	std::vector<bool> bUsed;
	std::vector<sc2Vector> points;
	m_FindLineTool->GetUsed(bUsed);
	m_FindLineTool->GetFitData(points);

	for (long m=0; m<bUsed.size(); m++)
	{
		bool bUse = bUsed[m];
		sc2Vector pt = points[m];

		CString strTmpKey = _T("");
		int nIndex = m_pGuiInspectFitPointArray.size();
		strTmpKey = m_strKeyText + strFitPoint;
		CString tmpIndex = _T("");
		tmpIndex.Format(_T("_%d"), nIndex);
		strTmpKey += tmpIndex;

		scGuiCoordCross *pTempFitPoint1 = m_GuiInspectFitPoint + nIndex;
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
		m_pGuiInspectFitPointArray.push_back(pTempFitPoint1);	
	}
}

CString CCombinationCornerSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());

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
			case -1:			// 检查区域定位失败				
				strErrInfo += _T("Check Pattern search failed");
				break;
			case -2:			// 检查几何定位失败				
				strErrInfo += _T("Check Quick search failed");
				break;
			case -3:			// 角定位区域定位失败					
				strErrInfo += _T("Corner Pattern search failed");
				break;
			case -4:			// 角定位几何定位失败				
				strErrInfo += _T("Corner Quick search failed");
				break;										
			case -5:			// 角定位找线1失败				
				strErrInfo += _T("Corner find line1 failed");
				break;
			case -6:			// 角定位找线2失败				
				strErrInfo += _T("Corner find line2 failed");
				break;
			case -7:			// 角定位求交点失败				
				strErrInfo += _T("Corner find the intersection of line1 and line2 failed");
				break;
			case -8:			// 线距检测1找线失败				
				strErrInfo += _T("Line distance1 find line failed");
				break;										
			case -9:			// 线距检测1超限失败				
				strErrInfo += (m_strShowInspectInfo +_T("Line distance1 exceed the limit"));
				break;
			case -10:			// 线距检测2找线失败				
				strErrInfo += _T("Line distance2 find line failed");
				break;
			case -11:			// 线距检测2超限失败				
				strErrInfo += (m_strShowInspectInfo +_T("Line distance2 exceed the limit"));
				break;
			case -12:			// 线距检测1找线1失败				
				strErrInfo += _T("Line distance1 find line1 failed");
				break;
			case -13:			// 线距检测1找线2失败				
				strErrInfo += _T("Line distance1 find line2 failed");
				break;
			case -14:			// 线距检测2找线1失败				
				strErrInfo += _T("Line distance2 find line1 failed");
				break;
			case -15:			// 线距检测2找线2失败				
				strErrInfo += _T("Line distance2 find line2 failed");
				break;
			case -16:			//线圆距离检测找线失败			
				strErrInfo += _T("Line Circle find line failed");
				break;
			case -17:			//线圆距离检测找圆失败				
				strErrInfo += _T("Line Circle find Circle failed");
				break;
			case -18:			// 线距检测找线失败				
				strErrInfo += _T("CircleLine distance find line failed");
				break;
			case -19:			// 圆线距检测超限失败				
				strErrInfo += (m_strShowInspectInfo +_T("ClrcleLine distance exceed the limit"));
				break;
			case -99:			// 训练未成功				
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
			case 0:
				{
					strErrInfo = m_strShowInspectInfo;
				}
				break;
			case -1:							
				strErrInfo += _T("FPC Mark区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("FPC Mark几何定位失败");
				break;
			case -3:							
				strErrInfo += _T("屏体MarK角定位区域定位失败");
				break;
			case -4:							
				strErrInfo += _T("屏体MarK角定位几何定位失败");
				break;										
			case -5:							
				strErrInfo += _T("屏体MarK角定位找线1失败");
				break;
			case -6:							
				strErrInfo += _T("屏体MarK角定位找线2失败");
				break;
			case -7:							
				strErrInfo += _T("屏体MarK角定位求交点失败");
				break;
			case -8:							
				strErrInfo += _T("线距检测1找线失败");
				break;										
			case -9:							
				strErrInfo +=  (m_strShowInspectInfo + _T("线距1超限"));
				break;
			case -10:							
				strErrInfo += _T("线距检测2找线失败");
				break;
			case -11:							
				strErrInfo += (m_strShowInspectInfo +_T("线距2超限"));
				break;
			case -12:							
				strErrInfo += _T("线距检测1找线1失败");
				break;
			case -13:							
				strErrInfo += _T("线距检测1找线2失败");
				break;
			case -14:							
				strErrInfo += _T("线距检测2找线1失败");
				break;
			case -15:							
				strErrInfo += _T("线距检测2找线2失败");
				break;
			case -16:							
				strErrInfo += _T("圆线距离检测找线失败");
				break;
			case -17:							
				strErrInfo += _T("圆线距离检测找圆失败");
				break;
			case -18:		 			
				strErrInfo += _T("圆距检测找线失败");
				break;
			case -19:			 		
				strErrInfo += (m_strShowInspectInfo + _T("圆线距超限"));
				break;
			case -99:							
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
			case 0:
				{
					strErrInfo = m_strShowInspectInfo;
				}
				break;
			case -1:							
				strErrInfo += _T("FPC Mark区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("FPC Mark几何定位失败");
				break;
			case -3:							
				strErrInfo += _T("角定位区域定位失败");
				break;
			case -4:							
				strErrInfo += _T("角定位几何定位失败");
				break;										
			case -5:							
				strErrInfo += _T("角定位找线1失败");
				break;
			case -6:							
				strErrInfo += _T("角定位找线2失败");
				break;
			case -7:							
				strErrInfo += _T("角定位求交点失败");
				break;
			case -8:							
				strErrInfo += _T("线距检测1找线失败");
				break;										
			case -9:							
				strErrInfo +=  (m_strShowInspectInfo + _T("线距1超限"));
				break;
			case -10:							
				strErrInfo += _T("线距检测2找线失败");
				break;
			case -11:							
				strErrInfo += (m_strShowInspectInfo +_T("线距2超限"));
				break;
			case -12:							
				strErrInfo += _T("线距检测1找线1失败");
				break;
			case -13:							
				strErrInfo += _T("线距检测1找线2失败");
				break;
			case -14:							
				strErrInfo += _T("线距检测2找线1失败");
				break;
			case -15:							
				strErrInfo += _T("线距检测2找线2失败");
				break;
			case -16:							
				strErrInfo += _T("圆线距离检测找线失败");
				break;
			case -17:							
				strErrInfo += _T("圆线距离检测找圆失败");
				break;
			case -18:		 			
				strErrInfo += _T("圆线距检测找线失败");
				break;
			case -19:			 		
				strErrInfo += (m_strShowInspectInfo + _T("圆线距超限"));
				break;
			case -99:							
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

bool CCombinationCornerSearchTool::ExecutePatternSerach(cpImage &ImageSearch,scPatternSearchRot* m_PatternSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dConfusionThre,double dTransX,double dTransY,double dRotation)
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

bool CCombinationCornerSearchTool::ExecuteQuickSearch(cpImage &ImageSearch,scQuickSearchTool* m_QuickSearchTool,scRect* m_SearchRect,int m_nSearchNum,double m_dAcceptThre,double m_dAngleLow,double m_dAngleHigh,double m_dSizeLow1,double m_dSizeHigh1,double dTransX,double dTransY,double dRotation)
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

bool CCombinationCornerSearchTool::ExecuteFindLine(cpImage &ImageSearch,scFindLine* m_FindLineTool,StructFindLineTrainData m_sFindLineTrainData,double dTransX,double dTransY,double dRotation,scLineSeg &ResultLine)
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

bool CCombinationCornerSearchTool::GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2)
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

void CCombinationCornerSearchTool::ShowResultRect(std::vector<scGuiGraphic*>& m_pGuiSearchRectArray,scGuiAffineRect m_GuiSearchAffineRect[MAXRESNUM],const int nOffset,scAffineRect* m_AffineRect,const CString strRect,COLORREF color,int LineWidth)
{
	if(NULL == m_AffineRect) return;
	//显示矩形
	CString strTmpKey = _T("");
	strTmpKey = m_strKeyText + strRect;
	CString tmpIndex = _T("");
	tmpIndex.Format(_T("_%d"), nOffset);
	strTmpKey += tmpIndex;

	scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+nOffset;
	pTempAffineRect->SetAffineRect(*m_AffineRect);
	pTempAffineRect->SetLineColor(color);
	pTempAffineRect->SetLineWidth(LineWidth);
	pTempAffineRect->SetTipTextEnabled(true);
	pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
	m_pGuiSearchRectArray.push_back(pTempAffineRect);
}

void CCombinationCornerSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,COLORREF color,int LineWidth)
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

void CCombinationCornerSearchTool::ShowResultCross(std::vector<scGuiGraphic*>& m_pGuiSearchCrossArray,scGuiCross m_GuiSearchCross[MAXRESNUM],const int nOffset,const sc2Vector &m_CrossVector,const CString strCross,const CString strCrossLabel,COLORREF color,int LineWidth)
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
void CCombinationCornerSearchTool::ShowResultLine(std::vector<scGuiGraphic*>& m_pGuiSearchLineArray,scGuiLine m_GuiSearchLine[MAXRESNUM],const int nOffset,scLine *m_scLine,const CString strLine,COLORREF color,int LineWidth)
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



