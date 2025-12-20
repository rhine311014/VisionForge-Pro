#include "stdafx.h"
#include "PatternSearchCheckLineTool.h"

#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"


CPatternSearchCheckLineTool::CPatternSearchCheckLineTool(void)
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
//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterWidthHeight(200, 240, 50, 50);
//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect2.SetCenterWidthHeight(400, 240, 50, 50);
//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}


	m_GuiFindLine[0].SetStartXYEndXY(300, 150, 400, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	int i=0;
	for (i=0; i<PATTERN_LINE_NUM; i++)
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

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像
	m_tmpTrainDataParam.m_bEnableCheckRegion1	= true;
	m_tmpTrainDataParam.m_bEnableCheckRegion2	= false;

	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_TrainRect1				= m_GuiTrainRect1.GetRect();
	m_tmpTrainGuiParam.m_TrainRect2				= m_GuiTrainRect2.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();
	m_tmpTrainGuiParam.m_bEnableLineMask1		= false;

	for (i=0; i<PATTERN_LINE_NUM; i++)
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
	}



	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;
	m_tmpSearchDataParam.m_dAcceptThre2			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre2		= 0.8;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CPatternSearchCheckLineTool::CPatternSearchCheckLineTool(CPatternSearchCheckLineTool &cCombinePatternSearchTool)
{
	if (cCombinePatternSearchTool.GetSearchToolType() != ePatternSearchCheckLineTool)
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
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterWidthHeight(200, 240, 50, 50);
	//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect2.SetCenterWidthHeight(400, 240, 50, 50);
	//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	m_GuiFindLine[0].SetStartXYEndXY(300, 150, 400, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	for (int i=0; i<PATTERN_LINE_NUM; i++)
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
	m_CamHeight								= cCombinePatternSearchTool.m_CamHeight;
	m_CamWidth								= cCombinePatternSearchTool.m_CamWidth;
	m_TrainDataParam						= cCombinePatternSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinePatternSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinePatternSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinePatternSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCombinePatternSearchTool.m_TrainResult;
	m_ImageTrain							= cCombinePatternSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinePatternSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCombinePatternSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cCombinePatternSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCombinePatternSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinePatternSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinePatternSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cCombinePatternSearchTool.UpdateSearchGuiData(true);
	cCombinePatternSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCombinePatternSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinePatternSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinePatternSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinePatternSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinePatternSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinePatternSearchTool.m_tmpImageMask);

	if (m_bTrainOK)
	{
		m_PatternSearchTool = cCombinePatternSearchTool.m_PatternSearchTool;
		if (m_TrainDataParam.m_bEnableCheckRegion1)
		{
			m_PatternSearchTool1 = cCombinePatternSearchTool.m_PatternSearchTool1;
		}
		if (m_TrainDataParam.m_bEnableCheckRegion2)
		{
			m_PatternSearchTool2 = cCombinePatternSearchTool.m_PatternSearchTool2;
		}
	}
}

CPatternSearchCheckLineTool& CPatternSearchCheckLineTool::operator=(CPatternSearchCheckLineTool &cCombinePatternSearchTool)
{
	if (this == &cCombinePatternSearchTool)
	{
		return *this;	
	}

	if (cCombinePatternSearchTool.GetSearchToolType() != ePatternSearchCheckLineTool)
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
	//	m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiTrainRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect1.SetCenterWidthHeight(200, 240, 50, 50);
	//	m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiTrainRect1.SetSelectedLineColor(RGB(255,0,0));

	m_GuiTrainRect2.SetCenterWidthHeight(400, 240, 50, 50);
	//	m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiTrainRect2.SetSelectedLineColor(RGB(255,0,0));

	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetSelectedLineColor(RGB(255,0,0));

	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetSelectedLineColor(RGB(255,0,0));

	//卡尺不可见，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(FALSE);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}

	m_GuiFindLine[0].SetStartXYEndXY(300, 150, 400, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	for (int i=0; i<PATTERN_LINE_NUM; i++)
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
	m_CamHeight								= cCombinePatternSearchTool.m_CamHeight;
	m_CamWidth								= cCombinePatternSearchTool.m_CamWidth;

	m_TrainDataParam						= cCombinePatternSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cCombinePatternSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cCombinePatternSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cCombinePatternSearchTool.m_SearchGuiParam;
	m_TrainResult							= cCombinePatternSearchTool.m_TrainResult;
	m_ImageTrain							= cCombinePatternSearchTool.m_ImageTrain;
	m_ImageMask								= cCombinePatternSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cCombinePatternSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cCombinePatternSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cCombinePatternSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cCombinePatternSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cCombinePatternSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cCombinePatternSearchTool.UpdateSearchGuiData(true);
	cCombinePatternSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cCombinePatternSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cCombinePatternSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cCombinePatternSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cCombinePatternSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cCombinePatternSearchTool.m_tmpImageTrain);
	SetMaskImage(cCombinePatternSearchTool.m_tmpImageMask);

	if (m_bTrainOK)
	{
		m_PatternSearchTool = cCombinePatternSearchTool.m_PatternSearchTool;
		if (m_TrainDataParam.m_bEnableCheckRegion1)
		{
			m_PatternSearchTool1 = cCombinePatternSearchTool.m_PatternSearchTool1;
		}
		if (m_TrainDataParam.m_bEnableCheckRegion2)
		{
			m_PatternSearchTool2 = cCombinePatternSearchTool.m_PatternSearchTool2;
		}
	}

	return *this;	
}

CPatternSearchCheckLineTool::~CPatternSearchCheckLineTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CPatternSearchCheckLineTool::GetSearchToolType()
{
	return ePatternSearchCheckLineTool;
}										   

// 设置显示控件指针		
bool CPatternSearchCheckLineTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CPatternSearchCheckLineTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternSearchCheckLineTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CPatternSearchCheckLineTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternSearchCheckLineTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternSearchCheckLineTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CPatternSearchCheckLineTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CPatternSearchCheckLineTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiTrainRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableCheckRegion1);
	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible && m_tmpTrainDataParam.m_bEnableCheckRegion2);

	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);

	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);


	for (int i=0; i<PATTERN_LINE_NUM; i++)
	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, i);
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);
	}

	if (m_tmpTrainGuiParam.m_bEnableLineMask1)
	{
		m_GuiFindLine[0].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
	}
	else
	{
		m_GuiFindLine[0].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect1, m_strKeyText+_T("_CheckTrainRect1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect2, m_strKeyText+_T("_CheckTrainRect2"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));

		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	


		for (int i=0; i<PATTERN_LINE_NUM; i++)
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

bool CPatternSearchCheckLineTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[0], m_strKeyText+_T("_TrainResultLine1"));

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CPatternSearchCheckLineTool::UpdateSearchResultGuiDisplay()							
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


	for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		int i=0;
		if (m_bSearchResultGuiShow) // 立即更新
		{
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

			for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary1[i], m_pGuiSearchCrossArrayAuxilary1[i]->GetTipText());
			}

			for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArrayAuxilary2[i], m_pGuiSearchCrossArrayAuxilary2[i]->GetTipText());
			}

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
bool CPatternSearchCheckLineTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternSearchCheckLineTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CPatternSearchCheckLineTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CPatternSearchCheckLineTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	*((CPatternSearchCheckLineTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CPatternSearchCheckLineTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CPatternSearchCheckLineTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CPatternSearchCheckLineTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CPatternSearchCheckLineTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CPatternSearchCheckLineTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternSearchCheckLineTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CPatternSearchCheckLineTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternSearchCheckLineTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像1									
bool CPatternSearchCheckLineTool::SetMaskImage2(const cpImage &ImageMask)
{
	return true;
}

// 获取训练原始图像1								
bool CPatternSearchCheckLineTool::GetMaskImage2(cpImage &ImageMask)
{
	return true;
}

// 执行训练											
bool CPatternSearchCheckLineTool::Train()
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
	if (!m_PatternSearchTool1.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber=-2;       //检查工具1失败
		return false;
	}
	if (!m_PatternSearchTool2.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber=-3;       //检查工具2失败
		return false;
	}

	if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}
	if (!m_PatternSearchTool1.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber=-2;       //检查工具1失败
		return false;
	}
	if (!m_PatternSearchTool2.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber=-3;       //检查工具2失败
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
	// 必须在训练时设置角度1
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool1.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber=-2;       //检查工具1失败
			return false;
		}
	}
	else if (!m_PatternSearchTool1.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-2;       //检查工具1失败
		return false;
	}
	// 必须在训练时设置角度1
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool2.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber=-3;       //检查工具2失败
			return false;
		}
	}
	else if (!m_PatternSearchTool2.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
	{
		m_nResultErrNumber=-3;       //检查工具2失败
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
		if (m_tmpTrainDataParam.m_bEnableCheckRegion1)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_PatternSearchTool1.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint1, &m_tmpTrainGuiParam.m_TrainRect1);
			}
			else
			{
				bRet = bRet && m_PatternSearchTool1.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint1, &m_tmpTrainGuiParam.m_TrainRect1);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-2;       //检查工具1失败
				return false;
			}
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion2)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_PatternSearchTool2.Train(tmpImage, tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint2, &m_tmpTrainGuiParam.m_TrainRect2);
			}
			else
			{
				bRet = bRet && m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_ModelPoint2, &m_tmpTrainGuiParam.m_TrainRect2);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-3;       //检查工具2失败
				return false;
			}
		}

		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			return m_bTrainOK;
		}
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
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}
		if (m_tmpTrainDataParam.m_bEnableCheckRegion1)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_PatternSearchTool1.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint1, &m_tmpTrainGuiParam.m_TrainRect1);
			}
			else
			{
				bRet = bRet && m_PatternSearchTool1.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint1, &m_tmpTrainGuiParam.m_TrainRect1);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-2;       //检查工具1失败
				return false;
			}
		}

		if (m_tmpTrainDataParam.m_bEnableCheckRegion2)
		{
			if(bIsProcessImage)
			{
				bRet = bRet && m_PatternSearchTool2.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint2, &m_tmpTrainGuiParam.m_TrainRect2);
			}
			else
			{
				bRet = bRet && m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint2, &m_tmpTrainGuiParam.m_TrainRect2);
			}
			if (!bRet)
			{
				m_nResultErrNumber=-3;       //检查工具2失败
				return false;
			}
		}		

		m_bTrainOK = bRet;

		if (false == m_bTrainOK)
		{
			return m_bTrainOK;
		}
	}

	if (m_bTrainOK)
	{
		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		// 找线0
		m_FindLineTool[0].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[0]);
		m_FindLineTool[0].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[0], m_tmpTrainGuiParam.m_CaliperSearchRotation[0], m_tmpTrainGuiParam.m_dCaliperSearchLen[0]
		,m_tmpTrainGuiParam.m_dCaliperProjectLen[0]);
		//m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[0], m_tmpTrainDataParam.m_edgeProperty[0], m_tmpTrainDataParam.m_lFilterHalfWidth[0]);
		m_FindLineTool[0].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[0], eRansacRobust);
		m_FindLineTool[0].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore[0]);
		m_FindLineTool[0].SetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[0], m_tmpTrainGuiParam.m_dStartY[0], 
			m_tmpTrainGuiParam.m_dEndX[0], m_tmpTrainGuiParam.m_dEndY[0]);
		m_FindLineTool[0].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine1);
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
		if(bIsProcessImage)
		{
			m_bTrainOK = m_FindLineTool[0].Execute(tmpImage, cLineSeg1, dRMS1);
		}
		else
		{
			m_bTrainOK = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
		}
		if (!m_bTrainOK)
		{
			m_nResultErrNumber=-4;       //找线1失败
			return false;
		}

		if (m_tmpTrainDataParam.m_nLineAngleType[0] == 0)	// 任意角度
		{
			m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
			m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
			m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
			m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
		}
		else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 1)	//水平线
		{
			scDegree degLineAngle;
			degLineAngle = scDegree(cLineSeg1.GetRotation()).SignedNorm();

			if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
			{
				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
			}
			else
			{
				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP1().GetY();
			}
		}
		else if (m_tmpTrainDataParam.m_nLineAngleType[0] == 2)	//垂直线
		{
			scDegree degLineAngle;
			degLineAngle = scDegree(cLineSeg1.GetRotation()).Norm();

			if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
			{
				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
			}
			else
			{
				m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP2().GetX();
				m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP2().GetY();
				m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP1().GetX();
				m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP1().GetY();
			}
		}
		else
		{
			m_tmpTrainResult.m_dStartX[0] = cLineSeg1.GetP1().GetX();
			m_tmpTrainResult.m_dStartY[0] = cLineSeg1.GetP1().GetY();
			m_tmpTrainResult.m_dEndX[0] = cLineSeg1.GetP2().GetX();
			m_tmpTrainResult.m_dEndY[0] = cLineSeg1.GetP2().GetY();
		}

		if (m_tmpTrainDataParam.m_bMaskEnable)
		{
			m_ImageMask      = m_tmpImageMask;
		}

		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
	}

	return m_bTrainOK;
}

// 获取训练结果图像														
bool CPatternSearchCheckLineTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool.GetPatternImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果图像														
bool CPatternSearchCheckLineTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	if (!m_TrainDataParam.m_bEnableCheckRegion1)
	{
		return false;
	}

	bool bRet = m_PatternSearchTool1.GetPatternImage(m_ImageTrainResult1);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult1;

	return true;
}	

// 获取训练结果图像														
bool CPatternSearchCheckLineTool::GetTrainResultImage3(cpImage &ImageTrainResult)
{
	if (!m_TrainDataParam.m_bEnableCheckRegion2)
	{
		return false;
	}

	bool bRet = m_PatternSearchTool2.GetPatternImage(m_ImageTrainResult2);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult2;

	return true;
}

// 获取训练结果
bool CPatternSearchCheckLineTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	*((CPatternSearchCheckLineTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}							

// 是否训练成功
bool CPatternSearchCheckLineTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CPatternSearchCheckLineTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternSearchCheckLineSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternSearchCheckLineTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	*((CPatternSearchCheckLineSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternSearchCheckLineTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CPatternSearchCheckLineSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CPatternSearchCheckLineTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternSearchCheckLineTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CPatternSearchCheckLineSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CPatternSearchCheckLineTool::Search(cpImage &ImageSearch)
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
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));


	// 设置搜索个数
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}
	if (!m_PatternSearchTool1.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-2;       //检查工具1失败
		return false;
	}
	if (!m_PatternSearchTool2.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-3;       //检查工具2失败
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}
	if (!m_PatternSearchTool1.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber=-2;       //检查工具1失败
		return false;
	}
	if (!m_PatternSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber=-3;       //检查工具2失败
		return false;
	}

	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}
	if (!m_PatternSearchTool1.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre2>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre2))
	{
		m_nResultErrNumber=-2;
		return false;
	}
	if (!m_PatternSearchTool2.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre2>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre2))
	{
		m_nResultErrNumber=-3;
		return false;
	}


	for (int i=0; i<PATTERN_LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		//m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindLineTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i], eRansacRobust);
		m_FindLineTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
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
	}


	// 执行搜索
	bool bRetCheck1 = true;
	bool bRetCheck2 = true;cpImage tmpImage;
	scAffineRect	m_trainResultRect;
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
			m_nResultErrNumber=-1;
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);
			m_trainResultRect = tmpPatternResult.MatchRegion();
//			CSearchResult searchResult;
// 			searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
// 			searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
// 			searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
// 			searchResult.m_dScore = tmpPatternResult.GetScore();
// 			m_SearchResultArray.push_back(searchResult);

			CString	strTmpKey;
			strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
			pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchRectArray.push_back(pTempAffineRect);

			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross+i;
			pTempCross->SetCenterExtent(sc2Vector(tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY()), sc2Vector(40,40));
			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);
			CString st;
			st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore());
			pTempCross->SetLabel(st);
			pTempCross->SetLabelVisible(TRUE);
			m_pGuiSearchCrossArray.push_back(pTempCross);

			//////////////////////////辅助矩形实时搜索区域计算1//////////////////////////////
			if (m_TrainDataParam.m_bEnableCheckRegion1)
			{
				sc2Vector tmModelPoint;
				m_PatternSearchTool.GetPatternOrigin(tmModelPoint);

				scRect tmpTrainRect1 = m_PatternSearchTool1.GetTrainRectRegion();
				sc2Vector tmTrainRectCenter;
				tmTrainRectCenter.SetX((tmpTrainRect1.GetLeft() + tmpTrainRect1.GetRight())*0.5);
				tmTrainRectCenter.SetY((tmpTrainRect1.GetTop() + tmpTrainRect1.GetBottom())*0.5);

				double dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
				sc2Vector pos = tmpPatternResult.GetPosition();

				//旋转后搜索区域中心
				tmTrainRectCenter = tmTrainRectCenter - tmModelPoint;
				double dXRotated = cos(dRotation) * tmTrainRectCenter.GetX() - sin(dRotation) * tmTrainRectCenter.GetY() + pos.GetX();
				double dYRotated = sin(dRotation) * tmTrainRectCenter.GetX() + cos(dRotation) * tmTrainRectCenter.GetY() + pos.GetY();
				tmTrainRectCenter = sc2Vector(dXRotated, dYRotated);

				//旋转后搜索仿射区域
				scAffineRect tmAffineRect;
				tmAffineRect.SetCenter(tmTrainRectCenter);
				tmAffineRect.SetXLength(tmpTrainRect1.GetWidth()*1.5);
				tmAffineRect.SetYLength(tmpTrainRect1.GetHeight()*1.5);
				tmAffineRect.SetXRotation(scRadian(dRotation));
				tmAffineRect.SetSkew(scRadian(0));

				//旋转后搜索区域
				scRect tmpSearchRect, SearchRect;
				tmpSearchRect = tmAffineRect.BoundingBox();
				//SearchRect = tmpSearchRect.Intersect(m_TrainGuiParam.m_SearchRect);
				SearchRect = tmpSearchRect;
				if(bIsProcessImage)
				{
					bRetCheck1 = m_PatternSearchTool1.Execute(tmpImage, SearchRect);

				}
				else
				{
					bRetCheck1 = m_PatternSearchTool1.Execute(ImageSearch, SearchRect);

				}
				if (bRetCheck1)
				{
					// 获取结果
					int nResultNum = m_PatternSearchTool1.GetResultNum();
					if (nResultNum > 0)
					{
						for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
						{
							scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool1.GetResult(i);

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

							scLineSeg ResLineSeg[PATTERN_LINE_NUM];
							double dRMS[PATTERN_LINE_NUM];
							bool   bFindLine[PATTERN_LINE_NUM];
							for (int k=0; k<PATTERN_LINE_NUM; k++)
							{
								bFindLine[k] = 0;
							}

							for (int j=0; j<PATTERN_LINE_NUM; j++)
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
								m_FindLineTool[j].SetCaliperMasked(m_TrainGuiParam.m_vbCaliperMask_FindLine1);
								if(bIsProcessImage)
								{
									// 执行找线
									bFindLine[j] = m_FindLineTool[j].Execute(tmpImage, ResLineSeg[j], dRMS[j]); // 成功
								}
								else
								{
									// 执行找线
									bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLineSeg[j], dRMS[j]); // 成功
								}
								
							}

							// 求交点
							if (bFindLine[0])
							{
								CSearchResult searchResult;

								searchResult.m_dPosX = (ResLineSeg[0].GetP1().GetX() + ResLineSeg[0].GetP2().GetX())/2;
								searchResult.m_dPosY = (ResLineSeg[0].GetP1().GetY() + ResLineSeg[0].GetP2().GetY())/2;
								searchResult.m_dScore = tmpPatternResult.GetScore();

								CResultSearchRect resRect;
								resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
								searchResult.m_vTrainSearchRect.push_back(resRect);

								resRect.m_searchRect = m_trainResultRect;
								searchResult.m_vACFSearchRect.push_back(resRect);

								if (m_TrainDataParam.m_nLineAngleType[0] == 0)	// 任意角度
								{
									searchResult.m_dAngle = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();

									searchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg[0].GetP1().GetX();
									searchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg[0].GetP1().GetY();
									searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
									searchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg[0].GetP2().GetX();
									searchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg[0].GetP2().GetY();
									searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
								}
								else if (m_TrainDataParam.m_nLineAngleType[0] == 1)	//水平线
								{
									searchResult.m_dAngle = scDegree(ResLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg[0].GetRotation()).SignedNorm();
									if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
									{
										searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP1().GetX();
										searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP1().GetY();
										searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
										searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP2().GetX();
										searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP2().GetY();
										searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
									}
									else
									{
										searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP2().GetX();
										searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP2().GetY();
										searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
										searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP1().GetX();
										searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP1().GetY();
										searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
									}
								}
								else if (m_TrainDataParam.m_nLineAngleType[0] == 2)	//垂直线
								{
									searchResult.m_dAngle = scDegree(ResLineSeg[0].GetRotation().NormMod180()).ToDouble();

									scDegree degLineAngle;
									degLineAngle = scDegree(ResLineSeg[0].GetRotation()).Norm();
									if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
									{
										searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP1().GetX();
										searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP1().GetY();
										searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
										searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP2().GetX();
										searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP2().GetY();
										searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
									}
									else
									{
										searchResult.m_vdAuxiliaryPosX.at(0) = ResLineSeg[0].GetP2().GetX();
										searchResult.m_vdAuxiliaryPosY.at(0) = ResLineSeg[0].GetP2().GetY();
										searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
										searchResult.m_vdAuxiliaryPosX.at(1) = ResLineSeg[0].GetP1().GetX();
										searchResult.m_vdAuxiliaryPosY.at(1) = ResLineSeg[0].GetP1().GetY();
										searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
									}
								}
								else
								{
									searchResult.m_dAngle = scDegree(ResLineSeg[0].GetRotation().Norm()).ToDouble();

									searchResult.m_vdAuxiliaryPosX.at(0)  = ResLineSeg[0].GetP1().GetX();
									searchResult.m_vdAuxiliaryPosY.at(0)  = ResLineSeg[0].GetP1().GetY();
									searchResult.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;
									searchResult.m_vdAuxiliaryPosX.at(1)  = ResLineSeg[0].GetP2().GetX();
									searchResult.m_vdAuxiliaryPosY.at(1)  = ResLineSeg[0].GetP2().GetY();
									searchResult.m_vdAuxiliaryAngle.at(1) = searchResult.m_dAngle;
								}

								// 设置线
								CResultLine resline;
								resline.m_vLineX[0] = ResLineSeg[0].GetP1().GetX();
								resline.m_vLineY[0] = ResLineSeg[0].GetP1().GetY();
								resline.m_vLineX[1] = ResLineSeg[0].GetP2().GetX();
								resline.m_vLineY[1] = ResLineSeg[0].GetP2().GetY();
								searchResult.m_vLineSearchResult.push_back(resline);


								m_SearchResultArray.push_back(searchResult);

								for (int l=0; l<PATTERN_LINE_NUM; l++)
								{
									strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
									//scGuiLine *pTemLine = new scGuiLine;
									scGuiLine *pTemLine = m_GuiSearchLine+i*2+l;
									pTemLine->SetXYRotation(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), scDegree(searchResult.m_dAngle));
									pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
									pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
									pTemLine->SetTipTextEnabled(true);
									pTemLine->SetTipText(strTmpKey);

									CString st;
									st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle);
									pTemLine->SetLabel(st);
									pTemLine->SetLabelVisible(TRUE);

									m_pGuiSearchLineArray.push_back(pTemLine);
								}

								strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
								//scGuiCross *pTempLineCross = new scGuiCross;
								scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
								pTempLineCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(60,60));
								pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempLineCross->SetTipTextEnabled(true);
								pTempLineCross->SetTipText(strTmpKey);
								m_pGuiSearchLineCrossArray.push_back(pTempLineCross);


								//////////////////////////////////////////////////////////////////////////
								std::vector<bool> bUsed;
								std::vector<sc2Vector> points;
								m_FindLineTool[0].GetUsed(bUsed);
								m_FindLineTool[0].GetFitData(points);
								for (long m=0; m<bUsed.size(); m++)
								{
									bool bUse = bUsed[m];
									sc2Vector pt = points[m];

									int nIndex = m_pGuiSearchResultFitPoint1Array.size();
									strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
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
								//////////////////////////////////////////////////////////////////////////


								strTmpKey.Format(_T("%s_ResultCrossAuxiliary1%d"), m_strKeyText, i);
								//scGuiCross *pTempCross = new scGuiCross;
								pTempCross = m_GuiSearchCrossAuxilary1+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary1.push_back(pTempCross);

								strTmpKey.Format(_T("%s_ResultCrossAuxiliary2%d"), m_strKeyText, i);
								//scGuiCross *pTempCross = new scGuiCross;
								pTempCross = m_GuiSearchCrossAuxilary2+i;
								pTempCross->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(1),searchResult.m_vdAuxiliaryPosY.at(1)), sc2Vector(30,30));
								pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
								pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
								pTempCross->SetTipTextEnabled(true);
								pTempCross->SetTipText(strTmpKey);
								m_pGuiSearchCrossArrayAuxilary2.push_back(pTempCross);

							}
							else
							{
								m_nResultErrNumber=-4;  //找线1失败
								return false;
							}

							CString	strTmpKey;
							strTmpKey.Format(_T("%s_Check1ResultRect%d"), m_strKeyText, i);
							scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect1+i;
							pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
							pTempAffineRect->SetLineColor(RGB(255,0,0));
							pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempAffineRect->SetTipTextEnabled(true);
							pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchRectArray.push_back(pTempAffineRect);

							strTmpKey.Format(_T("%s_Check1SearchRect%d"), m_strKeyText, i);
							scGuiRect  *pTempRect = m_GuiSearchRect1+i;
							pTempRect->SetRect(SearchRect);
							pTempRect->SetLineColor(RGB(255,255,255));
							pTempRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempRect->SetTipTextEnabled(true);
							pTempRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchRectArray.push_back(pTempRect);

						}	
					}
					else
					{
						
						bRetCheck1 = false;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////

			//////////////////////////辅助矩形实时搜索区域计算2//////////////////////////////
			if (m_TrainDataParam.m_bEnableCheckRegion2)
			{
				sc2Vector tmModelPoint;
				m_PatternSearchTool.GetPatternOrigin(tmModelPoint);

				scRect tmpTrainRect1 = m_PatternSearchTool2.GetTrainRectRegion();
				sc2Vector tmTrainRectCenter;
				tmTrainRectCenter.SetX((tmpTrainRect1.GetLeft() + tmpTrainRect1.GetRight())*0.5);
				tmTrainRectCenter.SetY((tmpTrainRect1.GetTop() + tmpTrainRect1.GetBottom())*0.5);

				double dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
				sc2Vector pos = tmpPatternResult.GetPosition();

				//旋转后搜索区域中心
				tmTrainRectCenter = tmTrainRectCenter - tmModelPoint;
				double dXRotated = cos(dRotation) * tmTrainRectCenter.GetX() - sin(dRotation) * tmTrainRectCenter.GetY() + pos.GetX();
				double dYRotated = sin(dRotation) * tmTrainRectCenter.GetX() + cos(dRotation) * tmTrainRectCenter.GetY() + pos.GetY();
				tmTrainRectCenter = sc2Vector(dXRotated, dYRotated);

				//旋转后搜索仿射区域
				scAffineRect tmAffineRect;
				tmAffineRect.SetCenter(tmTrainRectCenter);
				tmAffineRect.SetXLength(tmpTrainRect1.GetWidth()*1.5);
				tmAffineRect.SetYLength(tmpTrainRect1.GetHeight()*1.5);
				tmAffineRect.SetXRotation(scRadian(dRotation));
				tmAffineRect.SetSkew(scRadian(0));

				//旋转后搜索区域
				scRect tmpSearchRect, SearchRect;
				tmpSearchRect = tmAffineRect.BoundingBox();
				//SearchRect = tmpSearchRect.Intersect(m_TrainGuiParam.m_SearchRect);
				SearchRect = tmpSearchRect;
				if(bIsProcessImage)
				{
					bRetCheck2 = m_PatternSearchTool2.Execute(tmpImage, SearchRect);
				}
				else
				{
					bRetCheck2 = m_PatternSearchTool2.Execute(ImageSearch, SearchRect);
				}
				
				if (bRetCheck2)
				{
					// 获取结果
					int nResultNum = m_PatternSearchTool2.GetResultNum();
					if (nResultNum > 0)
					{
						for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
						{
							scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool2.GetResult(i);

							CString	strTmpKey;
							strTmpKey.Format(_T("%s_Check2ResultRect%d"), m_strKeyText, i);
							scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect2+i;
							pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
							pTempAffineRect->SetLineColor(RGB(255,0,0));
							pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempAffineRect->SetTipTextEnabled(true);
							pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchRectArray.push_back(pTempAffineRect);

							strTmpKey.Format(_T("%s_Check2SearchRect%d"), m_strKeyText, i);
							scGuiRect  *pTempRect = m_GuiSearchRect2+i;
							pTempRect->SetRect(SearchRect);
							pTempRect->SetLineColor(RGB(255,255,255));
							pTempRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempRect->SetTipTextEnabled(true);
							pTempRect->SetTipText((LPCTSTR)strTmpKey);
							m_pGuiSearchRectArray.push_back(pTempRect);
						}	
					}
					else
					{
						bRetCheck2 =  false;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////

		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		m_nResultErrNumber=-1;   //区域定位失败
		return bRet;
	}
	if (!bRetCheck1)
	{
		m_nResultErrNumber=-2;   //检查1失败
		return false;
	}
	if (!bRetCheck2)
	{
		m_nResultErrNumber=-3;   //检查2失败
		return false;
	}

	return (bRet && bRetCheck1 && bRetCheck2);
}

// 获取搜索结果个数										
int CPatternSearchCheckLineTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternSearchCheckLineTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternSearchCheckLineTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternSearchCheckLineTool)
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
bool CPatternSearchCheckLineTool::LoadModelFromFile(CString strDir)
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
	CPatternSearchCheckLineTrainDataParam tmpTrainDataParam;
	CPatternSearchCheckLineTrainGuiParam tmpTrainGuiParam;
	CPatternSearchCheckLineSearchDataParam tmpSearchDataParam;
	CPatternSearchCheckLineSearchGuiParam tmpSearchGuiParam;	
	CPatternSearchCheckLineTrainResult    tmpTrainResult;

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
	if (cSearchToolType != ePatternSearchCheckLineTool)
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
	// EnableCheckRegion1
	bRet = cXMLConfigurator.FindElem(_T("EnableCheckRegion1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableCheckRegion1 = _ttoi(cXMLConfigurator.GetElemData());
	// EnableCheckRegion2
	bRet = cXMLConfigurator.FindElem(_T("EnableCheckRegion2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableCheckRegion2 = _ttoi(cXMLConfigurator.GetElemData());

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
	if (bRet)
	{
		tmpTrainDataParam.m_bSortByScore[0] = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainDataParam.m_bSortByScore[0] = true;	
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
	// LineAngleType1
	bRet = cXMLConfigurator.FindElem(_T("LineAngleType1"));
	if (bRet)
	{
		tmpTrainDataParam.m_nLineAngleType[0] = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainDataParam.m_nLineAngleType[0] = 0 ;
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


	// CheckTrainrect1
	bRet = cXMLConfigurator.FindElem(_T("CheckTrainRect1"));
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
	tmpTrainGuiParam.m_TrainRect1.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_TrainRect1.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

	// CheckTrainrect2
	bRet = cXMLConfigurator.FindElem(_T("CheckTrainRect2"));
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
	tmpTrainGuiParam.m_TrainRect2.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_TrainRect2.SetSize(vPos);
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

	// CheckModelPoint1
	bRet = cXMLConfigurator.FindElem(_T("CheckModelPoint1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
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
	tmpTrainGuiParam.m_ModelPoint1 = vPos;
	cXMLConfigurator.LeaveElem();

	// CheckModelPoint2
	bRet = cXMLConfigurator.FindElem(_T("CheckModelPoint2"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint1.GetX());
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
	
	//////////////////////////////////////////////////////////////////////////
	bRet = cXMLConfigurator.FindElem(_T("CaliperMaskEnable"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bEnableLineMask1 = false;
	}
	tmpTrainGuiParam.m_bEnableLineMask1 = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("MaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();
		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdMaskData_FindLine1.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vdMaskData_FindLine1.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet) return false;
			tmpTrainGuiParam.m_vdMaskData_FindLine1.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}
		bRet = cXMLConfigurator.FindElem(_T("CaliperMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCaliperMask_FindLine1.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCaliperMask_FindLine1.size(); j++)
		{
			strTmp.Format(_T("CaliperMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet) return false;
			tmpTrainGuiParam.m_vbCaliperMask_FindLine1.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}
	//////////////////////////////////////////////////////////////////////////

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

	bRet = cXMLConfigurator.FindElem(_T("AcceptThre2"));
	if (!bRet)
	{
		return false;
	}
	else
	{
		tmpSearchDataParam.m_dAcceptThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}
	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre2"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dConfusionThre2 = (tmpSearchDataParam.m_dAcceptThre2 + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre2 + 0.1);
	}
	else
	{
		tmpSearchDataParam.m_dConfusionThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = false;
	}
	tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());

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
	scPatternSearchRot tmpPatternSearchTool;
	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}
	m_PatternSearchTool = tmpPatternSearchTool;
	if (tmpTrainDataParam.m_bEnableCheckRegion1)
	{
		bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model1.pts"));
		if (!bRet)
		{
			return false;
		}
		m_PatternSearchTool1 = tmpPatternSearchTool;
	}
	if (tmpTrainDataParam.m_bEnableCheckRegion2)
	{
		bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model2.pts"));
		if (!bRet)
		{
			return false;
		}
		m_PatternSearchTool2 = tmpPatternSearchTool;
	}

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
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
bool CPatternSearchCheckLineTool::SaveModelToFile(CString strDir)
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
	// EnableCheckRegion1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCheckRegion1);
	cXMLConfigurator.AddElem(_T("EnableCheckRegion1"), strInfo);
	// EnableCheckRegion2
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableCheckRegion2);
	cXMLConfigurator.AddElem(_T("EnableCheckRegion2"), strInfo);

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
	// LineAngleType1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType[0]);
	cXMLConfigurator.AddElem(_T("LineAngleType1"), strInfo);


	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam//////////////
	// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	// 	cXMLConfigurator.EnterElem();


	// train rect
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

	// CheckTrainRect1
	cXMLConfigurator.AddElem(_T("CheckTrainRect1"));
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

	// CheckTrainRect2
	cXMLConfigurator.AddElem(_T("CheckTrainRect2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect2.GetSize().GetY());
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
	// CheckModelPoint1
	cXMLConfigurator.AddElem(_T("CheckModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// CheckModelPoint2
	cXMLConfigurator.AddElem(_T("CheckModelPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
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
	//////////////////////////////////////////////////////////////////////////
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bEnableLineMask1);
	cXMLConfigurator.AddElem(_T("CaliperMaskEnable"), strInfo);

	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("MaskParam"));
	cXMLConfigurator.AddElem(_T("MaskParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdMaskData_FindLine1.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vdMaskData_FindLine1.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdMaskData_FindLine1.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask_FindLine1.size());
	cXMLConfigurator.AddElem(_T("CaliperMaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask_FindLine1.size(); j++)
	{
		strTmp.Format(_T("CaliperMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask_FindLine1.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();
	//////////////////////////////////////////////////////////////////////////

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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre2);
	cXMLConfigurator.AddElem(_T("AcceptThre2"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre2);
	cXMLConfigurator.AddElem(_T("ConfusionThre2"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

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

	// line0
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[0]);
	cXMLConfigurator.AddElem(_T("StartX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[0]);
	cXMLConfigurator.AddElem(_T("StartY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[0]);
	cXMLConfigurator.AddElem(_T("EndX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[0]);
	cXMLConfigurator.AddElem(_T("EndY1"), strInfo);

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

	if (m_TrainDataParam.m_bEnableCheckRegion1)
	{
		bRet = m_PatternSearchTool1.SaveTrainModelToBinaryFile(strDir + _T("Model1.pts"));
		if (!bRet)
		{
			return false;
		}
	}

	if (m_TrainDataParam.m_bEnableCheckRegion2)
	{
		bRet = m_PatternSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.pts"));
		if (!bRet)
		{
			return false;
		}
	}

	return true;
}	

// 更新训练控件数据信息	
bool CPatternSearchCheckLineTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_TrainRect1			= m_GuiTrainRect1.GetRect();
		m_tmpTrainGuiParam.m_TrainRect2			= m_GuiTrainRect2.GetRect();

		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint1		= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint2		= m_GuiModelPoint.GetCenter();

		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();


		for (int i=0; i<PATTERN_LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dStartX[i], m_tmpTrainGuiParam.m_dStartY[i], m_tmpTrainGuiParam.m_dEndX[i],
				m_tmpTrainGuiParam.m_dEndY[i]);
			m_tmpTrainGuiParam.m_bCaliperVisible[i]=m_GuiFindLine[i].IsCaliperVisible();
		}

		m_GuiFindLine[0].GetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine1);
		m_GuiFindLine[0].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask_FindLine1);
	}
	else // 将数据传递到Gui控件上
	{	
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);

		m_GuiTrainRect1.SetLineColor(RGB(255, 0, 0));
		m_GuiTrainRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect1.SetRect(m_tmpTrainGuiParam.m_TrainRect1);

		m_GuiTrainRect2.SetLineColor(RGB(255, 0, 0));
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetRect(m_tmpTrainGuiParam.m_TrainRect2);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);


		for (int i=0; i<PATTERN_LINE_NUM; i++)
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
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);
		}

		m_GuiFindLine[0].SetMaskData(m_tmpTrainGuiParam.m_vdMaskData_FindLine1);
	}

	return true;
}

bool CPatternSearchCheckLineTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}

	return true;
}

// 清除控件及其显示					
bool CPatternSearchCheckLineTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CPatternSearchCheckLineTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CPatternSearchCheckLineTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary1);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary2);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	return true;
}

bool CPatternSearchCheckLineTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;

	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	m_GuiTrainRect1.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect1.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	m_GuiTrainRect2.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

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
	
	double plength,slength;
	plength = m_GuiFindLine[0].GetCaliperProjectionLength();
	slength = m_GuiFindLine[0].GetCaliperSearchLength();
	m_GuiFindLine[0].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
	m_GuiFindLine[0].SetCaliperSearchLength(slength/m_CamWidth*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CPatternSearchCheckLineTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_CheckTrainRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_CheckTrainRect2"));

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));		

		int j=0;
		for (j=0; j<PATTERN_LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
	}

	return true;
}

bool CPatternSearchCheckLineTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary2[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint1Array[j]->GetTipText());
		}

	}

	return true;
}

// 获取训练模板原始图像
bool CPatternSearchCheckLineTool::GetPatternImage(cpImage &ImagePattern)
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
bool CPatternSearchCheckLineTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternSearchCheckLineTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternSearchCheckLineTool::SetLanguage(int nLanguage)
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
		m_GuiTrainRect1.SetLabel(_T("Check Train Area1"));
		m_GuiTrainRect2.SetLabel(_T("Check Train Area2"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiTrainRect1.SetLabel(_T("检查训练区域1"));
		m_GuiTrainRect2.SetLabel(_T("检查训练区域2"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}

	for (int i=0; i<PATTERN_LINE_NUM; i++)
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

CString	CPatternSearchCheckLineTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
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
				strErrInfo += _T("Check1 failed");
				break;
			case -3:							
				strErrInfo += _T("Check2 failed");
				break;
			case -4:							
				strErrInfo += _T("Find line1 failed");
				break;										
			case -99:							
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
			case -1:							
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("检查1失败");
				break;
			case -3:							
				strErrInfo += _T("检查2失败");
				break;
			case -4:							
				strErrInfo += _T("找线1失败");
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
			case -1:							
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("检查1失败");
				break;
			case -3:							
				strErrInfo += _T("检查2失败");
				break;
			case -4:							
				strErrInfo += _T("找线1失败");
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
