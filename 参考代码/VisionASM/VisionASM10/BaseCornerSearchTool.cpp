// BaseCornerSearchTool.cpp: implementation of the CBaseCornerSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseCornerSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"


CBaseCornerSearchTool::CBaseCornerSearchTool(void)
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;

	m_GuiFindLine[1].SetStartXYEndXY(100, 250, 100, 100);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(150, 100, 300, 100);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);


	//卡尺不可见，矩形区域可见
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

	CreateGUID(m_strKeyText);

	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiFindLine[0].GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiFindLine[0].GetLineColor();

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
	}


	m_tmpSearchDataParam.m_nSearchNum			= 1;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchLine[0].GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchLine[0].GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CBaseCornerSearchTool::CBaseCornerSearchTool(CBaseCornerSearchTool &cBaseCornerSearchTool)
{
	if (cBaseCornerSearchTool.GetSearchToolType() != eBaseCornerSearchTool)
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

	m_GuiFindLine[1].SetStartXYEndXY(100, 300, 200, 200);
	m_GuiFindLine[0].SetStartXYEndXY(300, 200, 400, 300);

	//卡尺不可见，矩形区域可见
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

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cBaseCornerSearchTool.m_CamHeight;
	m_CamWidth								= cBaseCornerSearchTool.m_CamWidth;

	m_TrainDataParam						= cBaseCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cBaseCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cBaseCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cBaseCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cBaseCornerSearchTool.m_TrainResult;
	m_ImageTrain							= cBaseCornerSearchTool.m_ImageTrain;
	m_bTrainGuiShow							= cBaseCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cBaseCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cBaseCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cBaseCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cBaseCornerSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cBaseCornerSearchTool.UpdateSearchGuiData(true);
	cBaseCornerSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cBaseCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cBaseCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cBaseCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cBaseCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cBaseCornerSearchTool.m_tmpImageTrain);
}

CBaseCornerSearchTool& CBaseCornerSearchTool::operator=(CBaseCornerSearchTool &cBaseCornerSearchTool)
{
	if (this == &cBaseCornerSearchTool)
	{
		return *this;	
	}

	if (cBaseCornerSearchTool.GetSearchToolType() != eBaseCornerSearchTool)
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

	m_GuiFindLine[1].SetStartXYEndXY(100, 300, 200, 200);
	m_GuiFindLine[0].SetStartXYEndXY(300, 200, 400, 300);

	//卡尺不可见，矩形区域可见
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

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cBaseCornerSearchTool.m_CamHeight;
	m_CamWidth								= cBaseCornerSearchTool.m_CamWidth;
	m_TrainDataParam						= cBaseCornerSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cBaseCornerSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cBaseCornerSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cBaseCornerSearchTool.m_SearchGuiParam;
	m_TrainResult							= cBaseCornerSearchTool.m_TrainResult;

	m_ImageTrain							= cBaseCornerSearchTool.m_ImageTrain;
	m_bTrainGuiShow							= cBaseCornerSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cBaseCornerSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cBaseCornerSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cBaseCornerSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cBaseCornerSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cBaseCornerSearchTool.UpdateSearchGuiData(true);
	cBaseCornerSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cBaseCornerSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cBaseCornerSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cBaseCornerSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cBaseCornerSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cBaseCornerSearchTool.m_tmpImageTrain);

 	return *this;	
}

CBaseCornerSearchTool::~CBaseCornerSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CBaseCornerSearchTool::GetSearchToolType()
{
	return eBaseCornerSearchTool;
}

// 设置显示控件指针		
bool CBaseCornerSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CBaseCornerSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CBaseCornerSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CBaseCornerSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CBaseCornerSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CBaseCornerSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CBaseCornerSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CBaseCornerSearchTool::UpdateTrainGuiDisplay()
{	
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

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		for (int i=0; i<LINE_NUM; i++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], strKey);
		}

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

bool CBaseCornerSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);

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
bool CBaseCornerSearchTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

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

		if (m_bSearchResultGuiShow) // 立即更新
		{
			int i=0;

			for (i=0; i<m_pGuiSearchLineArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
			}
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CBaseCornerSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CBaseCornerSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CBaseCornerSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CBaseCornerSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	*((CBaseCornerSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CBaseCornerSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CBaseCornerSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CBaseCornerSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CBaseCornerSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CBaseCornerSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CBaseCornerSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CBaseCornerSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	return true;
}

// 获取训练原始图像								
bool CBaseCornerSearchTool::GetMaskImage(cpImage &ImageMask)
{
	return true;
}

// 执行训练											
bool CBaseCornerSearchTool::Train()
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
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);
	}		


	// 找线0
	m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
	m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
	,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
	//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
	m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
	m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
		m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
	m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[0]);
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
	scLineSeg cLineSeg1;
	double dRMS1;
	bool bRet = false;
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
		m_nResultErrNumber=-1;       //找线1失败
		return false;
	}

	// 找线1
	m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
	m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
	,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
	//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
	m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
	m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
		m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
	m_FindLineTool[1].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[1]);
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
		m_nResultErrNumber=-2;       //找线2失败
		return false;
	}

	// 求交点
	bool bInterSect = false;
	std::vector<sc2Vector> vectArray;
	bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
	if (!bRet || !bInterSect || vectArray.size() <= 0)
	{
		m_nResultErrNumber=-3;       //求交点失败
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

	m_bTrainOK       = true;
	m_ImageTrain     = m_tmpImageTrain;
	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam	 = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;
	m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
	m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());

	return true;
}

// 获取训练结果图像														
bool CBaseCornerSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	ImageTrainResult = m_ImageTrain;

	return true;
}

// 获取训练结果
bool CBaseCornerSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	*((CBaseCornerSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CBaseCornerSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CBaseCornerSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CBaseCornerSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CBaseCornerSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	*((CBaseCornerSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CBaseCornerSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CBaseCornerSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CBaseCornerSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eBaseCornerSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CBaseCornerSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CBaseCornerSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //未知失败
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber = 0;
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

	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));

	for (int i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
		m_FindLineTool[i].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask[i]);

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

		double dTransStartX, dTransStartY;
		double dTransEndX, dTransEndY;
		dTransStartX = m_TrainGuiParam.m_dStartX[i];
		dTransStartY = m_TrainGuiParam.m_dStartY[i];
		dTransEndX = m_TrainGuiParam.m_dEndX[i];
		dTransEndY =  m_TrainGuiParam.m_dEndY[i];
		m_FindLineTool[i].SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
	}


	scLineSeg ResLine[LINE_NUM];
	double dRMS[LINE_NUM];
	bool   bFindLine[LINE_NUM];
	for (int k=0; k<LINE_NUM; k++)
	{
		bFindLine[k] = 0;
	}

	for (int j=0; j<LINE_NUM; j++)
	{
		if(bIsProcessImage)
		{
			// 执行找线
			bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLine[j], dRMS[j]); // 成功
		}
		else
		{
			// 执行找线
			bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功
		}
	}
    if (!bFindLine[0] && bFindLine[1])
    {
        CSearchResult searchResult;
        sc2Vector pointCenter2;
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
        searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
        searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
        searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
        searchResult.m_bFindLine2 = TRUE;
        searchResult.m_bFindLine1 = FALSE;
        m_SearchResultArray.push_back(searchResult);

		m_nResultErrNumber=-1;       //找线1失败
        return false;

    }
    if (bFindLine[0] && !bFindLine[1])
    {
        CSearchResult searchResult;
        sc2Vector pointCenter1;
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
        searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
        searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
        searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
        searchResult.m_bFindLine1=TRUE;
        searchResult.m_bFindLine2=FALSE;
        m_SearchResultArray.push_back(searchResult);

		m_nResultErrNumber=-2;       //找线2失败
        return false;
    }
    if (!bFindLine[0] && !bFindLine[1])
    {
        CSearchResult searchResult;
        searchResult.m_bFindLine1=FALSE;
        searchResult.m_bFindLine2=FALSE;
        m_SearchResultArray.push_back(searchResult);
		//m_nResultErrNumber=-3;       //找线1失败，找线2失败
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
			double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
			searchResult.m_dAngle = dRotAngle;
            searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
            searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

            searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
            searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
			searchResult.m_dLineAngle1 = scDegree(ResLine[0].GetRotation().SignedNormMod180()).ToDouble();
			searchResult.m_dLineAngle2 = scDegree(ResLine[1].GetRotation().SignedNormMod180()).ToDouble();
            searchResult.m_bFindLine1 = TRUE;
            searchResult.m_bFindLine2 = TRUE;
			searchResult.m_dScore = 1.0;

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

			m_SearchResultArray.push_back(searchResult);

			CString	strTmpKey;
			for (int l=0; l<2; l++)
			{
				strTmpKey.Format(_T("%s_ResultLine%d"), m_strKeyText, l);
				//scGuiLine *pTemLine = new scGuiLine;
				scGuiLine *pTemLine = m_GuiSearchLine+l;
				pTemLine->SetLine(ResLine[l].GetLine());
				pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTemLine->SetTipTextEnabled(true);
				pTemLine->SetTipText(strTmpKey);
				m_pGuiSearchLineArray.push_back(pTemLine);
			}

			strTmpKey.Format(_T("%s_ResultLineCross"), m_strKeyText);
			//scGuiCross *pTempLineCross = new scGuiCross;
			scGuiCross *pTempLineCross = m_GuiSearchLineCross;
			pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
			pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempLineCross->SetTipTextEnabled(true);
			pTempLineCross->SetTipText(strTmpKey);

			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f"), 0, crossPointArray[0].GetX(), crossPointArray[0].GetY());
			pTempLineCross->SetLabel(st);
			pTempLineCross->SetLabelVisible(TRUE);
			m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
		}
	}
	else
	{
		m_nResultErrNumber=-1;       //找线1失败
		return false;
	}

	if (m_SearchResultArray.size() <= 0)
	{
		m_nResultErrNumber=-4;       //求交点失败
		return false;
	}

	m_SearchDataParam = m_tmpSearchDataParam;
	m_SearchGuiParam  = m_tmpSearchGuiParam;

	return true;
}

// 获取搜索结果个数										
int CBaseCornerSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CBaseCornerSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像文件
bool CBaseCornerSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eBaseCornerSearchTool)
	{
		return false;
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
	 ImagePattern = tmpImageModel;
	return true;
}


// 加载模板文件
bool CBaseCornerSearchTool::LoadModelFromFile(CString strDir)
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
	CBaseCornerSearchTrainDataParam tmpTrainDataParam;
	CBaseCornerSearchTrainGuiParam tmpTrainGuiParam;
	CBaseCornerSearchSearchDataParam tmpSearchDataParam;
	CBaseCornerSearchSearchGuiParam tmpSearchGuiParam;	
	CBaseCornerSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eBaseCornerSearchTool)
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
		tmpTrainDataParam.m_bSortByScore[1] = true;
	}
	tmpTrainDataParam.m_bSortByScore[1] = _ttoi(cXMLConfigurator.GetElemData());

	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());
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

	// Caliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible1"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[0]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[0] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// CaliperMaskEnable
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bEnableLineMask[0] = false;
	}
	// MaskParam
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
	// Caliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CaliperVisible2"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCaliperVisible[1]=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCaliperVisible[1] = _ttoi(cXMLConfigurator.GetElemData());
	}

	// CaliperMaskEnable1
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

	cXMLConfigurator.LeaveElem();

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
	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
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

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CBaseCornerSearchTool::SaveModelToFile(CString strDir)
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
		cXMLConfigurator.AddElem(_T("CamWidth"), m_CamWidth);
	}
	cXMLConfigurator.LeaveElem();

	/////////////TrainDataParam//////////////
	cXMLConfigurator.AddElem(_T("TrainParam"));
	cXMLConfigurator.EnterElem();
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
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
	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam//////////////
	// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	// 	cXMLConfigurator.EnterElem();


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
	// CaliperMaskEnable
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
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainRectVisible);
	// 	cXMLConfigurator.AddElem(_T("TrainRectVisible"), strInfo);
	// 	// Search  rect visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
	// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
	// 	//Model point visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
	// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);

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
	cXMLConfigurator.LeaveElem();

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

	return true;
}	

// 更新训练控件数据信息	
bool CBaseCornerSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		for (int i=0; i<LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_tmpTrainGuiParam.m_bCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();
			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);

		}
	}
	else // 将数据传递到Gui控件上
	{	
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

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);//显示/隐藏卡尺，隐藏/显示矩形框
			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);

		}
	}

	return true;
}

bool CBaseCornerSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

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

	}

	return true;
}

// 清除控件及其显示					
bool CBaseCornerSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CBaseCornerSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CBaseCornerSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	return true;
}


bool CBaseCornerSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

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

bool CBaseCornerSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		for (j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));
	}

	return true;
}

bool CBaseCornerSearchTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

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
bool CBaseCornerSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	ImagePattern = m_ImageTrain;

	return true;
}	

// 恢复训练参数
bool CBaseCornerSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CBaseCornerSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CBaseCornerSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;

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
CString	CBaseCornerSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							// 找线1失败
				strErrInfo += _T("Find line1 failed");
				break;
			case -2:							// 找线2失败
				strErrInfo += _T("Find line2 failed");
				break;
			case -3:							// 找线1失败，找线2失败
				strErrInfo += _T("Find line1 failed，Find line2 failed");
				break;
			case -4:							// 求交点失败
				strErrInfo += _T("Find the intersection of line1 and line2 failed");
				break;
			case -99:							// 求交点失败
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
			case -1:							// 找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -2:							// 找线2失败
				strErrInfo += _T("找线2失败");
				break;
			case -3:							// 找线1失败，找线2失败
				strErrInfo += _T("找线1失败,找线2失败");
				break;
			case -4:							// 求交点失败
				strErrInfo += _T("求交点失败");
				break;
			case -99:							// 求交点失败
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
			case -1:							// 找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -2:							// 找线2失败
				strErrInfo += _T("找线2失败");
				break;
			case -3:							// 找线1失败，找线2失败
				strErrInfo += _T("找线1失败,找线2失败");
				break;
			case -4:							// 求交点失败
				strErrInfo += _T("求交点失败");
				break;
			case -99:							// 求交点失败
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