// PatternRectSearchTool.cpp: implementation of the CPatternRectSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatternRectSearchTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"


CPatternRectSearchTool::CPatternRectSearchTool(void)
{
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
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);
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
	m_GuiFindLine[0].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(425, 350, 275, 350);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[2].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[2].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);	
	m_GuiFindLine[3].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

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
	CString strDirZh[4] = {"上","下","左","右"};
	CString strDirEn[4] = {"Up","Down","Left","Right"};
	for (i=0; i<RECT_LINE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d_%s"), i+1, strDirEn[i]);
			break;
		case 1:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		default:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	for (i=0; i<RECT_LINE_NUM; i++)
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
		m_tmpTrainDataParam.m_bEnableDualEdge[i]		= false;
	}


	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CPatternRectSearchTool::CPatternRectSearchTool(CPatternRectSearchTool &cPatternRectSearchTool)
{
	if (cPatternRectSearchTool.GetSearchToolType() != ePatternRectSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);
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

	m_GuiFindLine[0].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(425, 350, 275, 350);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[2].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[2].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);	
	m_GuiFindLine[3].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

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
	CString strDirZh[4] = {"上","下","左","右"};
	CString strDirEn[4] = {"Up","Down","Left","Right"};
	for (i=0; i<RECT_LINE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d_%s"), i+1, strDirEn[i]);
			break;
		case 1:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		default:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}
	CreateGUID(m_strKeyText);


	// 赋值
	m_CamHeight								=  cPatternRectSearchTool.m_CamHeight;
	m_CamWidth								=  cPatternRectSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternRectSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternRectSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternRectSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternRectSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternRectSearchTool.m_TrainResult;
	m_ImageTrain							= cPatternRectSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternRectSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternRectSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternRectSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternRectSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternRectSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternRectSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cPatternRectSearchTool.UpdateSearchGuiData(true);
	cPatternRectSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternRectSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternRectSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternRectSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternRectSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternRectSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternRectSearchTool.m_tmpImageMask);

	if (m_bTrainOK)
	{
		m_PatternSearchTool = cPatternRectSearchTool.m_PatternSearchTool;
	}
}

CPatternRectSearchTool& CPatternRectSearchTool::operator=(CPatternRectSearchTool &cPatternRectSearchTool)
{
	if (this == &cPatternRectSearchTool)
	{
		return *this;	
	}

	if (cPatternRectSearchTool.GetSearchToolType() != ePatternRectSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	m_GuiModelPoint.SetLabelVisible(TRUE);
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

	m_GuiFindLine[0].SetStartXYEndXY(275, 150, 425, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[1].SetStartXYEndXY(425, 350, 275, 350);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[2].SetStartXYEndXY(200, 300, 200, 150);
	m_GuiFindLine[2].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);	
	m_GuiFindLine[3].SetStartXYEndXY(500, 150, 500, 300);
	m_GuiFindLine[3].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

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
	CString strDirZh[4] = {"上","下","左","右"};
	CString strDirEn[4] = {"Up","Down","Left","Right"};
	for (i=0; i<RECT_LINE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d_%s"), i+1, strDirEn[i]);
			break;
		case 1:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		default:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}
	CreateGUID(m_strKeyText);


	// 赋值
	m_CamHeight								=  cPatternRectSearchTool.m_CamHeight;
	m_CamWidth								=  cPatternRectSearchTool.m_CamWidth;

	m_TrainDataParam						= cPatternRectSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternRectSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternRectSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternRectSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternRectSearchTool.m_TrainResult;

	m_ImageTrain							= cPatternRectSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternRectSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternRectSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cPatternRectSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternRectSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternRectSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternRectSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternRectSearchTool.UpdateSearchGuiData(true);
	cPatternRectSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternRectSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternRectSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternRectSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternRectSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternRectSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternRectSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = cPatternRectSearchTool.m_PatternSearchTool;
	}

 	return *this;	
}

CPatternRectSearchTool::~CPatternRectSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

CSearchToolType CPatternRectSearchTool::GetSearchToolType()
{
	return ePatternRectSearchTool;
}

// 设置显示控件指针		
bool CPatternRectSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CPatternRectSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternRectSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CPatternRectSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternRectSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternRectSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;
	
	UpdateTrainResultGuiDisplay();
	return false;
}

// 更新搜索界面显示									
bool CPatternRectSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CPatternRectSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	for (int i=0; i<RECT_LINE_NUM; i++)
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

		for (int i=0; i<RECT_LINE_NUM; i++)
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

bool CPatternRectSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[1].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[2].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultLine[3].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[1], m_strKeyText+_T("_TrainResultLine2"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[2], m_strKeyText+_T("_TrainResultLine3"));
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[3], m_strKeyText+_T("_TrainResultLine4"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CPatternRectSearchTool::UpdateSearchResultGuiDisplay()							
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
bool CPatternRectSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternRectSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CPatternRectSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CPatternRectSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	*((CPatternRectSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CPatternRectSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CPatternRectSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CPatternRectSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CPatternRectSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CPatternRectSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternRectSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CPatternRectSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternRectSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CPatternRectSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置训练参数
	if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}
	}
	else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-1;       //区域定位失败
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
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

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
		m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
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
		m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
			m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		} 
		else
		{
			m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		}
		//m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		m_FindLineTool[2].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[2], eRansacRobust);
		m_FindLineTool[2].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[2], m_tmpTrainGuiParam.m_dStartY[2], 
			m_tmpTrainGuiParam.m_dEndX[2], m_tmpTrainGuiParam.m_dEndY[2]);
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
			m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		} 
		else
		{
			m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		}
		//m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		m_FindLineTool[3].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[3], eRansacRobust);
		m_FindLineTool[3].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[3], m_tmpTrainGuiParam.m_dStartY[3], 
			m_tmpTrainGuiParam.m_dEndX[3], m_tmpTrainGuiParam.m_dEndY[3]);
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
		
		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-6;//线1、线3求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-7;//线1、线4求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-8;//线2、线3求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-9;//线2、线4求交点失败
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
	else
	{
		bool bRet = false;
		if (bIsProcessImage)
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
			m_nResultErrNumber=-1;//定位失败
			return false;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

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
		//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
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
			m_nResultErrNumber=-2;//找线工具1失败
			return false;
		}

		// 找线1
		m_FindLineTool[1].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[1]);
		m_FindLineTool[1].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[1], m_tmpTrainGuiParam.m_CaliperSearchRotation[1], m_tmpTrainGuiParam.m_dCaliperSearchLen[1]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[1]);
		
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
		//m_FindLineTool[1].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		m_FindLineTool[1].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[1], eRansacRobust);
		m_FindLineTool[1].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[1], m_tmpTrainGuiParam.m_dStartY[1], 
			m_tmpTrainGuiParam.m_dEndX[1], m_tmpTrainGuiParam.m_dEndY[1]);
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
			m_nResultErrNumber=-3;//找线工具2失败
			return false;
		}
		// 找线2
		m_FindLineTool[2].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[2]);
		m_FindLineTool[2].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[2], m_tmpTrainGuiParam.m_CaliperSearchRotation[2], m_tmpTrainGuiParam.m_dCaliperSearchLen[2]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[2]);
		
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
			m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		} 
		else
		{
			m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		}
		//m_FindLineTool[2].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[2], m_tmpTrainDataParam.m_edgeProperty[2], m_tmpTrainDataParam.m_lFilterHalfWidth[2]);
		m_FindLineTool[2].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[2], eRansacRobust);
		m_FindLineTool[2].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[2], m_tmpTrainGuiParam.m_dStartY[2], 
			m_tmpTrainGuiParam.m_dEndX[2], m_tmpTrainGuiParam.m_dEndY[2]);
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
			m_nResultErrNumber=-4;//找线工具3失败
			return false;
		}
		// 找线3
		m_FindLineTool[3].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[3]);
		m_FindLineTool[3].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[3], m_tmpTrainGuiParam.m_CaliperSearchRotation[3], m_tmpTrainGuiParam.m_dCaliperSearchLen[3]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[3]);
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
			m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		} 
		else
		{
			m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		}
		//m_FindLineTool[3].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[3], m_tmpTrainDataParam.m_edgeProperty[3], m_tmpTrainDataParam.m_lFilterHalfWidth[3]);
		m_FindLineTool[3].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[3], eRansacRobust);
		m_FindLineTool[3].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[3], m_tmpTrainGuiParam.m_dStartY[3], 
			m_tmpTrainGuiParam.m_dEndX[3], m_tmpTrainGuiParam.m_dEndY[3]);
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
			m_nResultErrNumber=-5;//找线工具4失败
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-6;//线1、线3求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-7;//线1、线4求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-8;//线2、线3求交点失败
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-9;//线2、线4求交点失败
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

		m_bTrainOK       = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam	 = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_GuiTrainResultLine[0].SetLine(cLineSeg1.GetLine());
		m_GuiTrainResultLine[1].SetLine(cLineSeg2.GetLine());
		m_GuiTrainResultLine[2].SetLine(cLineSeg3.GetLine());
		m_GuiTrainResultLine[3].SetLine(cLineSeg4.GetLine());

		return true;
	}

	return false;
}

// 获取训练结果图像														
bool CPatternRectSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}

// 获取训练结果
bool CPatternRectSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	*((CPatternRectSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CPatternRectSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CPatternRectSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternRectSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternRectSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	*((CPatternRectSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternRectSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CPatternRectSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}


// 获取搜索Gui参数			
bool CPatternRectSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternRectSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CPatternRectSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CPatternRectSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;
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

	// 设置搜索个数
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-1;
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber=-1;
		return false;
	}

	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber=-1;
		return false;
	}

	for (int i=0; i<RECT_LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);
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
		//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
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
	//bool bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
	bool bRet = false;
	if (bIsProcessImage)
	{
		bRet = m_PatternSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
	}
	else
	{
		bRet = m_PatternSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
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

			scLineSeg ResLine[RECT_LINE_NUM];
			double dRMS[RECT_LINE_NUM];
			bool   bFindLine[RECT_LINE_NUM];
			for (int k=0; k<RECT_LINE_NUM; k++)
			{
				bFindLine[k] = 0;
			}

			CString	strTmpKey;

			//定位结果显示
			strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
			//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
			pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchRectArray.push_back(pTempAffineRect);

// 			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
// 			//scGuiCross *pTempCross = new scGuiCross;
// 			scGuiCross *pTempCross = m_GuiSearchCross+i;
// 			pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
// 			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempCross->SetTipTextEnabled(true);
// 			pTempCross->SetTipText(strTmpKey);
// 			m_pGuiSearchCrossArray.push_back(pTempCross);

			for (int j=0; j<RECT_LINE_NUM; j++)
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
				m_FindLineTool[j].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask[j]);

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
				

				// 找线结果显示
				if (bFindLine[j])
				{
					strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, j, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+j;
					pTemLine->SetLine(ResLine[j].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					// 找线拟合点
					{
						std::vector<bool> bUsed;
						std::vector<sc2Vector> points;
						m_FindLineTool[j].GetUsed(bUsed);
						m_FindLineTool[j].GetFitData(points);
						for (long m=0; m<bUsed.size(); m++)
						{
							bool bUse = bUsed[m];
							sc2Vector pt = points[m];

							switch (j)
							{
							case 0:
								{
									int nIndex = m_pGuiSearchResultFitPointArray_1.size();
									strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint = m_GuiSearchResultFitPoint_1 + nIndex;
									pTempFitPoint->SetXDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetYDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetRulerVisible(false);
									pTempFitPoint->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
									if (bUse)
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint->SetTipTextEnabled(true);
									pTempFitPoint->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPointArray_1.push_back(pTempFitPoint);	
								}
								break;
							case 1:
								{
									int nIndex = m_pGuiSearchResultFitPointArray_2.size();
									strTmpKey.Format(_T("%s_FitPoint2_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint = m_GuiSearchResultFitPoint_2 + nIndex;
									pTempFitPoint->SetXDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetYDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetRulerVisible(false);
									pTempFitPoint->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
									if (bUse)
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint->SetTipTextEnabled(true);
									pTempFitPoint->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPointArray_2.push_back(pTempFitPoint);	
								}
								break;
							case 2:
								{
									int nIndex = m_pGuiSearchResultFitPointArray_3.size();
									strTmpKey.Format(_T("%s_FitPoint3_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint = m_GuiSearchResultFitPoint_3 + nIndex;
									pTempFitPoint->SetXDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetYDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetRulerVisible(false);
									pTempFitPoint->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
									if (bUse)
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint->SetTipTextEnabled(true);
									pTempFitPoint->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPointArray_3.push_back(pTempFitPoint);	
								}
								break;
							case 3:
								{
									int nIndex = m_pGuiSearchResultFitPointArray_4.size();
									strTmpKey.Format(_T("%s_FitPoint4_%d"), m_strKeyText, nIndex);
									scGuiCoordCross *pTempFitPoint = m_GuiSearchResultFitPoint_4 + nIndex;
									pTempFitPoint->SetXDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetYDirectionAdornment(scGuiCoordCross::eNone);
									pTempFitPoint->SetRulerVisible(false);
									pTempFitPoint->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
									if (bUse)
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint);
									}
									else
									{
										pTempFitPoint->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint);
									}
									pTempFitPoint->SetTipTextEnabled(true);
									pTempFitPoint->SetTipText(strTmpKey);
									m_pGuiSearchResultFitPointArray_4.push_back(pTempFitPoint);	
								}
								break;
							default:
								break;
							}
						}
					}
				}
			}

			if ((bFindLine[0]==false)&&(bFindLine[1]==false)&&(bFindLine[2]==false)&&(bFindLine[3]==false))
			{
				m_nResultErrNumber = -5;
				return false;
			} 
			else if (bFindLine[0]==false)
			{
				m_nResultErrNumber = -1;
				return false;
			}
			else if (bFindLine[1]==false)
			{
				m_nResultErrNumber = -2;
				return false;
			}
			else if (bFindLine[2]==false)
			{
				m_nResultErrNumber = -3;
				return false;
			}
			else if (bFindLine[3]==false)
			{
				m_nResultErrNumber = -4;
				return false;
			}
			else
			{

			}

			bool bIntersect[4] = {false,false,false,false};
			sc2Vector vLineCrossPoint[4];

			// 求交点左上
			if (bFindLine[0] && bFindLine[2])
			{
				int nIntersectIndex = 0;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
					&& crossPointArray.size() > 0)
				{
					vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
					strTmpKey.Format(_T("%s_ResultLineCross左上_%d"), m_strKeyText, i);
					//scGuiCross *pTempLineCross = new scGuiCross;
					scGuiCross *pTempLineCross = m_GuiSearchLineCross+i*RECT_LINE_NUM+nIntersectIndex;
					pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
					//pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempLineCross->SetLineColor(RGB(255, 0, 0));
					pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempLineCross->SetTipTextEnabled(true);
					pTempLineCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("左上%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
					pTempLineCross->SetLabel(st);
					pTempLineCross->SetLabelVisible(TRUE);
					m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
				}
			}	

			// 求交点右上
			if (bFindLine[0] && bFindLine[3])
			{
				int nIntersectIndex = 1;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[0].GetLine(), ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
					&& crossPointArray.size() > 0)
				{
					vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
					strTmpKey.Format(_T("%s_ResultLineCross右上_%d"), m_strKeyText, i);
					//scGuiCross *pTempLineCross = new scGuiCross;
					scGuiCross *pTempLineCross = m_GuiSearchLineCross+i*RECT_LINE_NUM+nIntersectIndex;
					pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
					//pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempLineCross->SetLineColor(RGB(255, 0, 0));
					pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempLineCross->SetTipTextEnabled(true);
					pTempLineCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("右上%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
					pTempLineCross->SetLabel(st);
					pTempLineCross->SetLabelVisible(TRUE);
					m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
				}
			}	

			// 求交点左下
			if (bFindLine[1] && bFindLine[2])
			{
				int nIntersectIndex = 2;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[1].GetLine(), ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
					&& crossPointArray.size() > 0)
				{
					vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
					strTmpKey.Format(_T("%s_ResultLineCross左下_%d"), m_strKeyText, i);
					//scGuiCross *pTempLineCross = new scGuiCross;
					scGuiCross *pTempLineCross = m_GuiSearchLineCross+i*RECT_LINE_NUM+nIntersectIndex;
					pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
					//pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempLineCross->SetLineColor(RGB(255, 0, 0));
					pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempLineCross->SetTipTextEnabled(true);
					pTempLineCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("左下%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
					pTempLineCross->SetLabel(st);
					pTempLineCross->SetLabelVisible(TRUE);
					m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
				}
			}

			// 求交点右下
			if (bFindLine[1] && bFindLine[3])
			{
				int nIntersectIndex = 3;
				std::vector<sc2Vector> crossPointArray; 
				if (sfInterSectLineLine(ResLine[1].GetLine(), ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
					&& crossPointArray.size() > 0)
				{
					vLineCrossPoint[nIntersectIndex] = crossPointArray[0];
					strTmpKey.Format(_T("%s_ResultLineCross右下_%d"), m_strKeyText, i);
					//scGuiCross *pTempLineCross = new scGuiCross;
					scGuiCross *pTempLineCross = m_GuiSearchLineCross+i*RECT_LINE_NUM+nIntersectIndex;
					pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
					//pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempLineCross->SetLineColor(RGB(255, 0, 0));
					pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempLineCross->SetTipTextEnabled(true);
					pTempLineCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("右下%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
					pTempLineCross->SetLabel(st);
					pTempLineCross->SetLabelVisible(TRUE);
					m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
				}
			}

			// 如果4个交点都找到
			if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
			{
				double dPosX = 0.0,dPosY = 0.0;
				double dCenterX = 0.0,dCenterY = 0.0/*, dAngel = 0.0, dAngelAuxiliary = 0.0*/;
				scRadian dAngel,dAngelAuxiliary;
				for (long m=0; m<RECT_LINE_NUM; m++)
				{
					dPosX = dPosX + vLineCrossPoint[m].GetX();
					dPosY = dPosY + vLineCrossPoint[m].GetY();
				}

				// 求矩形中点
				dCenterX = dPosX/4;
				dCenterY = dPosY/4;

				//求长边的角度
				double d1,d2;
				if(false == sfDistancePoint2Point(vLineCrossPoint[0],vLineCrossPoint[1],d1))
					return FALSE;
				if(false == sfDistancePoint2Point(vLineCrossPoint[0],vLineCrossPoint[2],d2))
					return FALSE;

				if(d1>d2)
				{
					scLineSeg lineSeg01(vLineCrossPoint[0],vLineCrossPoint[1]);
					scLineSeg lineSeg23(vLineCrossPoint[2],vLineCrossPoint[3]);

					scLine line01,line23;

					line01.SetFromLineSeg(lineSeg01);
					line23.SetFromLineSeg(lineSeg23);

					scLineSeg lineSeg13(vLineCrossPoint[1],vLineCrossPoint[3]);
					scLineSeg lineSeg02(vLineCrossPoint[0],vLineCrossPoint[2]);

					scLine line13,line02;

					line13.SetFromLineSeg(lineSeg13);
					line02.SetFromLineSeg(lineSeg02);

// 					dAngel = (scDegree((line01.GetRotation() + line23.GetRotation())/2)).SignedNorm().ToDouble();
// 					dAngelAuxiliary = (scDegree((line13.GetRotation() + line02.GetRotation())/2)).SignedNorm().ToDouble();
					dAngel = (line01.GetRotation() + line23.GetRotation())/2;
					dAngelAuxiliary = (line13.GetRotation() + line02.GetRotation())/2;
				}
				else
				{
					scLineSeg lineSeg13(vLineCrossPoint[1],vLineCrossPoint[3]);
					scLineSeg lineSeg02(vLineCrossPoint[0],vLineCrossPoint[2]);

					scLine line13,line02;

					line13.SetFromLineSeg(lineSeg13);
					line02.SetFromLineSeg(lineSeg02);

					scLineSeg lineSeg01(vLineCrossPoint[0],vLineCrossPoint[1]);
					scLineSeg lineSeg23(vLineCrossPoint[2],vLineCrossPoint[3]);

					scLine line01,line23;

					line01.SetFromLineSeg(lineSeg01);
					line23.SetFromLineSeg(lineSeg23);

// 					dAngel = (scDegree((line13.GetRotation() + line02.GetRotation())/2)).SignedNorm().ToDouble();
// 					dAngelAuxiliary = (scDegree((line01.GetRotation() + line23.GetRotation())/2)).SignedNorm().ToDouble();
					dAngel = (line13.GetRotation() + line02.GetRotation())/2;
					dAngelAuxiliary = (line01.GetRotation() + line23.GetRotation())/2;
				}

				// 根据长边角度及中心点构造新的直线
				scLine AcrossLine,AcrossLineAuxiliary;
				AcrossLine.SetPosRotation(sc2Vector(dCenterX,dCenterY),dAngel);
				AcrossLineAuxiliary.SetPosRotation(sc2Vector(dCenterX,dCenterY),dAngelAuxiliary);

				CSearchResult searchResult;
				searchResult.m_dScore = tmpPatternResult.GetScore();
				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				std::vector<sc2Vector> NewcrossPointArray; 
				bool bIntersect = false;
// 				searchResult.m_dPosX = tmpQuickResult.GetPosition().GetX();
// 				searchResult.m_dPosY = tmpQuickResult.GetPosition().GetY();
// 				searchResult.m_dAngle = tmpQuickResult.GetRotate().ToDouble();
// 				searchResult.m_dScore = tmpQuickResult.GetScore();
				double RectX1,RectY1, RectX2,RectY2;
				RectX1 = vLineCrossPoint[0].GetX();
				RectY1 = vLineCrossPoint[0].GetY();
				RectX2 = vLineCrossPoint[3].GetX();
				RectY2 = vLineCrossPoint[3].GetY();
				///////////////////5----------1----------6//////////////////////////////////////
				///////////////////3		  0			 4////////////////////////////////////
				///////////////////7----------2----------8/////////////////////////////////////
				switch(m_tmpSearchGuiParam.m_nResultOutMode)
				{
				case 0:	//输出中心点
					searchResult.m_dPosX	= dCenterX;
					searchResult.m_dPosY	= dCenterY;
					//searchResult.m_dAngle	= dAngel;
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					break;
				case 1: //输出上交点
					if (sfInterSectLineLine(ResLine[0].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
					{
						if (NewcrossPointArray[0].GetX() < RectX1 || NewcrossPointArray[0].GetX() > RectX2)
						{
							if (sfInterSectLineLine(ResLine[0].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
							{
								searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
								searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
								//searchResult.m_dAngle	= dAngel;
								searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
							}
						}
						else
						{
							searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
							searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
							//searchResult.m_dAngle	= dAngel;
							searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
						}
					}		
					break;
				case 2: //输出下交点
					if (sfInterSectLineLine(ResLine[1].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
					{
						if (NewcrossPointArray[0].GetX() < RectX1 || NewcrossPointArray[0].GetX() > RectX2)
						{
							if (sfInterSectLineLine(ResLine[1].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
							{
								searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
								searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
								//searchResult.m_dAngle	= dAngel;
								searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
							}
						}
						else
						{
							searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
							searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
							//searchResult.m_dAngle	= dAngel;
							searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
						}
					}
					break;
				case 3: //输出左交点
					if (sfInterSectLineLine(ResLine[2].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
					{
						if (NewcrossPointArray[0].GetY() < RectY1 || NewcrossPointArray[0].GetY() > RectY2)
						{
							if (sfInterSectLineLine(ResLine[2].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
							{
								searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
								searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
								//searchResult.m_dAngle	= dAngel;
								searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
							}
						}
						else
						{
							searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
							searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
							//searchResult.m_dAngle	= dAngel;
							searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
						}
					}
					break;
				case 4: //输出右交点
					if (sfInterSectLineLine(ResLine[3].GetLine(), AcrossLine, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
					{
						if (NewcrossPointArray[0].GetY() < RectY1 || NewcrossPointArray[0].GetY() > RectY2)
						{
							if (sfInterSectLineLine(ResLine[3].GetLine(), AcrossLineAuxiliary, bIntersect, NewcrossPointArray) && NewcrossPointArray.size() > 0)
							{
								searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
								searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
								//searchResult.m_dAngle	= dAngel;
								searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
							}
						}
						else
						{
							searchResult.m_dPosX	= NewcrossPointArray[0].GetX();
							searchResult.m_dPosY	= NewcrossPointArray[0].GetY();
							//searchResult.m_dAngle	= dAngel;
							searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
						}
					}
					break;
				case 5://左上方
					searchResult.m_dPosX = vLineCrossPoint[0].GetX();
					searchResult.m_dPosY = vLineCrossPoint[0].GetY();
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();
					break;
				case 6://右上方
					searchResult.m_dPosX = vLineCrossPoint[1].GetX();
					searchResult.m_dPosY = vLineCrossPoint[1].GetY();
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();
					break;
				case 7://左下方
					searchResult.m_dPosX = vLineCrossPoint[2].GetX();
					searchResult.m_dPosY = vLineCrossPoint[2].GetY();
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();
					break;
				case 8://右下方
					searchResult.m_dPosX = vLineCrossPoint[3].GetX();
					searchResult.m_dPosY = vLineCrossPoint[3].GetY();
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					searchResult.m_dScore = tmpPatternResult.GetScore();
					break;
				default:
					searchResult.m_dPosX	= dCenterX;
					searchResult.m_dPosY	= dCenterY;
					//searchResult.m_dAngle	= dAngel;
					searchResult.m_dAngle	= scDegree(dAngel).SignedNorm().ToDouble();
					break;
				}

				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				//scGuiCross *pTempCross = new scGuiCross;
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY),sc2Vector(50,50));
				pTempCross->SetLineColor(RGB(255,0,0));
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipText(strTmpKey);
				pTempCross->SetTipTextEnabled(true);
				CString st;
				st.Format(_T("OutPut%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i,searchResult.m_dPosX, searchResult.m_dPosY,searchResult.m_dAngle,searchResult.m_dScore);
				pTempCross->SetLabel(st);
				pTempCross->SetLabelVisible(TRUE);
				m_pGuiSearchCrossArray.push_back(pTempCross);
				for (long k=0; k<RECT_LINE_NUM; k++)
				{
					searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
					searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
					searchResult.m_vdAuxiliaryAngle[k] = 0;
				}

				// 设置结果 
				for (int nLine = 0; nLine < RECT_LINE_NUM; nLine++)
				{
					CResultLine line;
					line.m_vLineX[0] = ResLine[nLine].GetP1().GetX();
					line.m_vLineY[0] = ResLine[nLine].GetP1().GetY();
					line.m_vLineX[1] = ResLine[nLine].GetP2().GetX();
					line.m_vLineY[1] = ResLine[nLine].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);
				}


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
		m_nResultErrNumber=-1;
		return false;
	}

	return bRet;
}

// 获取搜索结果个数										
int CPatternRectSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternRectSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternRectSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternRectSearchTool)
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
bool CPatternRectSearchTool::LoadModelFromFile(CString strDir)
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
	CPatternRectSearchTrainDataParam tmpTrainDataParam;
	CPatternRectSearchTrainGuiParam tmpTrainGuiParam;
	CPatternRectSearchSearchDataParam tmpSearchDataParam;
	CPatternRectSearchSearchGuiParam tmpSearchGuiParam;	
	CPatternRectSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != ePatternRectSearchTool)
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

	//DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());
	}
	

	// SortByScore1
	bRet = cXMLConfigurator.FindElem(_T("SortByScore1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[0] = true;
	}
	tmpTrainDataParam.m_bSortByScore[0] = _ttoi(cXMLConfigurator.GetElemData());

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

	//DualEdge2
	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// SortByScore2
	bRet = cXMLConfigurator.FindElem(_T("SortByScore2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[1] = true;
	}
	tmpTrainDataParam.m_bSortByScore[1] = _ttoi(cXMLConfigurator.GetElemData());


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
	//DualEdge3
	bRet = cXMLConfigurator.FindElem(_T("DualEdge3"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[2] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[2] = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// SortByScore3
	bRet = cXMLConfigurator.FindElem(_T("SortByScore3"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[2] = true;
	}
	tmpTrainDataParam.m_bSortByScore[2] = _ttoi(cXMLConfigurator.GetElemData());



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
	//DualEdge4
	bRet = cXMLConfigurator.FindElem(_T("DualEdge4"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[3] = false;
	}
	else
	{
		tmpTrainDataParam.m_bEnableDualEdge[3] = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	// SortByScore4
	bRet = cXMLConfigurator.FindElem(_T("SortByScore4"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore[3] = true;
	}
	tmpTrainDataParam.m_bSortByScore[3] = _ttoi(cXMLConfigurator.GetElemData());



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

	// Caliper Visible3
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

	// Caliper Visible4
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

	bRet = cXMLConfigurator.FindElem(_T("ResultOutMode"));
	if (bRet)
	{
		tmpSearchGuiParam.m_nResultOutMode = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchGuiParam.m_nResultOutMode = 0;
	}

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
	scPatternSearchRot tmpPatternSearchTool;
	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	m_PatternSearchTool = tmpPatternSearchTool;

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
bool CPatternRectSearchTool::SaveModelToFile(CString strDir)
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
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
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
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[0]);
	cXMLConfigurator.AddElem(_T("DualEdge1"), strInfo);
	// SortByScore1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[0]);
	cXMLConfigurator.AddElem(_T("SortByScore1"), strInfo);
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
	// DualEdge2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[1]);
	cXMLConfigurator.AddElem(_T("DualEdge2"), strInfo);
	// SortByScore2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[1]);
	cXMLConfigurator.AddElem(_T("SortByScore2"), strInfo);



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
	// DualEdge3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[2]);
	cXMLConfigurator.AddElem(_T("DualEdge3"), strInfo);
	// SortByScore3
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[2]);
	cXMLConfigurator.AddElem(_T("SortByScore3"), strInfo);
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
	// DualEdge4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableDualEdge[3]);
	cXMLConfigurator.AddElem(_T("DualEdge4"), strInfo);
	// SortByScore4
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[3]);
	cXMLConfigurator.AddElem(_T("SortByScore4"), strInfo);



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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre);
	cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint1);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible1"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint2);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible2"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint3);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible3"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_nResultOutMode);
	cXMLConfigurator.AddElem(_T("ResultOutMode"), strInfo);
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
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 更新训练控件数据信息	
bool CPatternRectSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		for (int i=0; i<RECT_LINE_NUM; i++)
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
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		for (int i=0; i<RECT_LINE_NUM; i++)
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
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);

		}
	}

	return true;
}

bool CPatternRectSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

	}

	return true;
}

// 清除控件及其显示					
bool CPatternRectSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CPatternRectSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CPatternRectSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_1);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_2);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_3);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_4);

	return true;
}

bool CPatternRectSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;
	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

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

bool CPatternRectSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		

		for (j=0; j<RECT_LINE_NUM; j++)
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

	return true;
}

bool CPatternRectSearchTool::ClearResultGui()
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
bool CPatternRectSearchTool::GetPatternImage(cpImage &ImagePattern)
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
	if(cpImage::statusSuccess == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}	

// 恢复训练参数
bool CPatternRectSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternRectSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternRectSearchTool::SetLanguage(int nLanguage)
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

	CString strDirZh[4] = {"上","下","左","右"};
	CString strDirEn[4] = {"Up","Down","Left","Right"};
	for (int i=0; i<RECT_LINE_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindLine%d_%s"), i+1, strDirEn[i]);
			break;
		case 1:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		default:
			str.Format(_T("找线工具%d_%s"), i+1, strDirZh[i]);
			break;
		}

		m_GuiFindLine[i].SetLabel(str);
		m_GuiFindLine[i].SetLabelVisible(TRUE);
	}
}
CString CPatternRectSearchTool::GetResultErrInfo()
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
				strErrInfo += _T("Pattern search failed");
				break;
			case -2:							
				strErrInfo += _T("Find line1 failed");
				break;
			case -3:							
				strErrInfo += _T("Find line2 failed");
				break;
			case -4:							
				strErrInfo += _T("Find line3 failed");
				break;
			case -5:							
				strErrInfo += _T("Find line4 failed");
				break;
			case -6:			//线1、线3求交点失败				
				strErrInfo += _T("Find the intersection of line1 and line3 failed");
				break;
			case -7:			//线1、线4求交点失败				
				strErrInfo += _T("Find the intersection of line1 and line4 failed");
				break;
			case -8:			//线2、线3求交点失败				
				strErrInfo += _T("Find the intersection of line2 and line3 failed");
				break;
			case -9:			//线2、线4求交点失败				
				strErrInfo += _T("Find the intersection of line2 and line4 failed");
				break;
			case -99:							
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
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("找线1失败");
				break;
			case -3:							
				strErrInfo += _T("找线2失败");
				break;
			case -4:							
				strErrInfo += _T("找线3失败");
				break;
			case -5:							
				strErrInfo += _T("找线4失败");
				break;
			case -6:							
				strErrInfo += _T("线1、线3求交点失败");
				break;
			case -7:							
				strErrInfo += _T("线1、线4求交点失败");
				break;
			case -8:							
				strErrInfo += _T("线2、线3求交点失败");
				break;
			case -9:							
				strErrInfo += _T("线2、线4求交点失败");
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
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("找线1失败");
				break;
			case -3:							
				strErrInfo += _T("找线2失败");
				break;
			case -4:							
				strErrInfo += _T("找线3失败");
				break;
			case -5:							
				strErrInfo += _T("找线4失败");
				break;
			case -6:							
				strErrInfo += _T("线1、线3求交点失败");
				break;
			case -7:							
				strErrInfo += _T("线1、线4求交点失败");
				break;
			case -8:							
				strErrInfo += _T("线2、线3求交点失败");
				break;
			case -9:							
				strErrInfo += _T("线2、线4求交点失败");
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