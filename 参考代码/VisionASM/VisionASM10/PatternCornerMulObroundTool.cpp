// PatternSearchObroundTool.cpp: implementation of the CPatternCornerMulObroundTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PatternCornerMulObroundTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternCornerMulObroundTool::CPatternCornerMulObroundTool()
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	
	m_GuiFindLine[1].SetStartXYEndXY(130, 300, 130, 200);
	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(120, 150, 270, 150);
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	//找线卡尺隐藏，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(false);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(false);
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

	m_GuiModelPoint.SetLabelVisible(TRUE);

	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_GuiFindObround[i].SetCenterLengthsRotation(sc2Vector(300+100*i,240), 200, 100, scRadian(0));
		m_GuiFindObround[i].SetCaliperParams(20, eInward, 50, 20);
		m_GuiFindObround[i].SetLabelVisible(TRUE);

	}
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);


	for (int i=0; i<OBROUND_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindObround%d"), i+1);
			break;
		case 1:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		}

		m_GuiFindObround[i].SetLabel(str);
	}

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
	
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_FindObroundTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], 
			m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_nOutLineNum[i]			= 10;
	}


	for(int i =0; i< LINE_NUM; i++)
	{
		m_FindLineTool[i].GetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
			m_tmpTrainDataParam.m_edgeLineProperty[i],
			m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
		m_tmpTrainDataParam.m_nLineOutLineNum[i] = 10;
		m_tmpTrainDataParam.m_bLineSortByScore[i] = false;
		m_tmpTrainDataParam.m_bLineEnableDualEdge[i] = false;
		
	}

	
	m_tmpTrainGuiParam.m_TrainRect		= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_tmpTrainGuiParam.m_nCaliperNum[i]			= m_GuiFindObround[i].GetCaliperNum();
		m_tmpTrainGuiParam.m_Obround[i]				= m_GuiFindObround[i].GetObround();
		m_tmpTrainGuiParam.m_dCaliperProjectLen[i]		= m_GuiFindObround[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen[i]		= m_GuiFindObround[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchDirection[i] = (CaliperSearchDirection)m_GuiFindObround[i].GetCaliperSearchDirection();
	}
	
	for(int i =0; i < LINE_NUM; i++)
	{
		m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i],
			m_tmpTrainGuiParam.m_dLineEndX[i], m_tmpTrainGuiParam.m_dLineEndY[i]);

		m_tmpTrainGuiParam.m_nLineCaliperNum[i]				= m_GuiFindLine[i].GetCaliperNum();
		m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]	    = m_GuiFindLine[i].GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();;
		m_tmpTrainGuiParam.m_bLineEnableLineMask[i]			= false;
		m_tmpTrainGuiParam.m_bLineCaliperVisible[i]			= m_GuiFindLine[i].IsCaliperVisible();
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

CPatternCornerMulObroundTool::CPatternCornerMulObroundTool(CPatternCornerMulObroundTool &CPatternCornerMulObroundTool)
{
	if (CPatternCornerMulObroundTool.GetSearchToolType() != ePatternCornerMulObroundTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 200, 100, 100);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);

	m_GuiModelPoint.SetLabelVisible(TRUE);
	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_GuiFindObround[i].SetCenterLengthsRotation(sc2Vector(300+100*i,240), 200, 100, scRadian(0));
		m_GuiFindObround[i].SetCaliperParams(20, eInward, 50, 20);
		m_GuiFindObround[i].SetLabelVisible(TRUE);
	}

	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);


	for (int i=0; i<OBROUND_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindObround%d"), i+1);
			break;
		case 1:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		}
		m_GuiFindObround[i].SetLabel(str);
	}
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
	m_GuiFindLine[0].SetStartXYEndXY(120, 150, 270, 150);
	//找线卡尺隐藏，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(false);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(false);
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
	
	// 赋值
	m_CamHeight								= CPatternCornerMulObroundTool.m_CamHeight;
	m_CamWidth								= CPatternCornerMulObroundTool.m_CamWidth;

	m_TrainDataParam						= CPatternCornerMulObroundTool.m_TrainDataParam;
	m_TrainGuiParam							= CPatternCornerMulObroundTool.m_TrainGuiParam;
	m_SearchDataParam						= CPatternCornerMulObroundTool.m_SearchDataParam;
	m_SearchGuiParam						= CPatternCornerMulObroundTool.m_SearchGuiParam;
	m_TrainResult							= CPatternCornerMulObroundTool.m_TrainResult;
	m_ImageTrain							= CPatternCornerMulObroundTool.m_ImageTrain;
	m_ImageMask								= CPatternCornerMulObroundTool.m_ImageMask;
	m_bTrainGuiShow							= CPatternCornerMulObroundTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = CPatternCornerMulObroundTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CPatternCornerMulObroundTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CPatternCornerMulObroundTool.m_bTrainOK;
	m_pGuiDisplay							= CPatternCornerMulObroundTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CPatternCornerMulObroundTool.UpdateSearchGuiData(true);
	CPatternCornerMulObroundTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CPatternCornerMulObroundTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CPatternCornerMulObroundTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CPatternCornerMulObroundTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CPatternCornerMulObroundTool.m_tmpSearchGuiParam);
	SetTrainImage(CPatternCornerMulObroundTool.m_tmpImageTrain);
	SetMaskImage(CPatternCornerMulObroundTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = CPatternCornerMulObroundTool.m_PatternSearchTool;
	}
}

CPatternCornerMulObroundTool& CPatternCornerMulObroundTool::operator=(CPatternCornerMulObroundTool &CPatternCornerMulObroundTool)
{
	if (this == &CPatternCornerMulObroundTool)
	{
		return *this;
	}

	if (CPatternCornerMulObroundTool.GetSearchToolType() != ePatternCornerMulObroundTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);

	m_GuiModelPoint.SetLabelVisible(TRUE);
	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_GuiFindObround[i].SetCenterLengthsRotation(sc2Vector(300+100*i,240), 200, 100, scRadian(0));
		m_GuiFindObround[i].SetCaliperParams(20, eInward, 50, 20);
		m_GuiFindObround[i].SetLabelVisible(TRUE);
	}

	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);


	for (int i=0; i<OBROUND_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindObround%d"), i+1);
			break;
		case 1:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		}
		m_GuiFindObround[i].SetLabel(str);
	}
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
	m_GuiFindLine[0].SetStartXYEndXY(120, 150, 270, 150);
	//找线卡尺隐藏，矩形区域可见
	m_GuiFindLine[0].SetCaliperVisible(false);
	m_GuiFindLine[0].SetAffineRectFindLineVisible(TRUE);
	m_GuiFindLine[1].SetCaliperVisible(false);
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
	
	// 赋值
	m_CamHeight								= CPatternCornerMulObroundTool.m_CamHeight;
	m_CamWidth								= CPatternCornerMulObroundTool.m_CamWidth;

	m_TrainDataParam						= CPatternCornerMulObroundTool.m_TrainDataParam;
	m_TrainGuiParam							= CPatternCornerMulObroundTool.m_TrainGuiParam;
	m_SearchDataParam						= CPatternCornerMulObroundTool.m_SearchDataParam;
	m_SearchGuiParam						= CPatternCornerMulObroundTool.m_SearchGuiParam;
	m_TrainResult							= CPatternCornerMulObroundTool.m_TrainResult;
	m_ImageTrain							= CPatternCornerMulObroundTool.m_ImageTrain;
	m_ImageMask								= CPatternCornerMulObroundTool.m_ImageMask;
	m_bTrainGuiShow							= CPatternCornerMulObroundTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = CPatternCornerMulObroundTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CPatternCornerMulObroundTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CPatternCornerMulObroundTool.m_bTrainOK;
	m_pGuiDisplay							= CPatternCornerMulObroundTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CPatternCornerMulObroundTool.UpdateSearchGuiData(true);
	CPatternCornerMulObroundTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CPatternCornerMulObroundTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CPatternCornerMulObroundTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CPatternCornerMulObroundTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CPatternCornerMulObroundTool.m_tmpSearchGuiParam);
	SetTrainImage(CPatternCornerMulObroundTool.m_tmpImageTrain);
	SetMaskImage(CPatternCornerMulObroundTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = CPatternCornerMulObroundTool.m_PatternSearchTool;
	}

	return *this;
}

CPatternCornerMulObroundTool::~CPatternCornerMulObroundTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CPatternCornerMulObroundTool::GetSearchToolType()
{
	return ePatternCornerMulObroundTool;
}										   

// 设置显示控件指针		
bool CPatternCornerMulObroundTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

// 获取显示控件指针										
scGuiDisplay* CPatternCornerMulObroundTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternCornerMulObroundTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CPatternCornerMulObroundTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternCornerMulObroundTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;
	
	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternCornerMulObroundTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CPatternCornerMulObroundTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 更新训练Gui控件显示
bool CPatternCornerMulObroundTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	//for (int i=0;i<OBROUND_NUM;i++)
	//{
	//	if (m_tmpTrainDataParam.m_bEnableMulObround)
	//	{
	//		m_GuiFindObround[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bObroundVisible);
	//	}
	//	else
	//	{
			m_GuiFindObround[0].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bObroundVisible);
			m_GuiFindObround[1].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bObroundVisible&&m_tmpTrainDataParam.m_bEnableMulObround);
					
	/*	}
	}*/
		
	for(int i = 0; i < LINE_NUM; i++)
	{
		if (m_tmpTrainGuiParam.m_bLineEnableLineMask[i])
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine[i].EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
		m_GuiFindLine[i].SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible[i]);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
	
		CString strTemp = _T("");
		for (int i=0;i<OBROUND_NUM;i++)
		{
			strTemp.Format(_T("_FindObround%d"),i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindObround[i], m_strKeyText+strTemp);
		}


		for (int i =0; i < LINE_NUM; i++)
		{
			strTemp.Format(_T("_FindLine%d"), i);
			m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine[i], m_strKeyText + strTemp);
		}

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}	

// 更新训练结果Gui控件显示
bool CPatternCornerMulObroundTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultObround[0].SetVisible(m_bTrainResultGuiShow);
	m_GuiTrainResultObround[1].SetVisible(m_bTrainResultGuiShow);
	
	for(int i =0 ; i < LINE_NUM; i++)
	{
		m_GuiTrainResultLine[i].SetVisible(m_bTrainResultGuiShow);
	}
	for (int i=0;i<OBROUND_NUM;i++)
	{
		if (m_TrainDataParam.m_bEnableMulObround)
		{
			m_GuiTrainResultLine[i].SetVisible(m_bTrainResultGuiShow);
		}
		else
		{
			if (i==0)
			{
				m_GuiTrainResultLine[0].SetVisible(m_bTrainResultGuiShow);
			}	
		}	
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();

		CString strTemp = _T("");
		for(int i = 0; i< LINE_NUM; i++)
		{
			strTemp.Format(_T("_TrainResultLine%d"), i);
			m_pGuiStaticContainer->AddItem(&m_GuiTrainResultLine[i], m_strKeyText + strTemp);
		}

		for (int i=0;i<OBROUND_NUM;i++)
		{
			strTemp.Format(_T("_TrainResultObround%d"),i);
			m_pGuiStaticContainer->AddItem(&m_GuiTrainResultObround[i], m_strKeyText+strTemp);
		}
				
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}		

// 更新搜索Gui控件显示														
bool CPatternCornerMulObroundTool::UpdateSearchResultGuiDisplay()
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
	
	for (i=0; i<m_pGuiSearchObroundArray.size(); i++)
	{
		m_pGuiSearchObroundArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundVisible);
	}
	
	for (i=0; i<m_pGuiSearchObroundCenterArray.size(); i++)
	{
		m_pGuiSearchObroundCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundCenterVisible);
	}
	for (i=0; i<m_pGuiSearchObroundResultArray.size(); i++)
	{
		m_pGuiSearchObroundResultArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundCenterVisible);
	}
	for (i=0; i<m_pGuiSearchObroundRightCircleArray.size(); i++)
	{
		m_pGuiSearchObroundRightCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bObroundCenterVisible);
	}
	for (i = 0; i < m_pGuiSearchFindLineArray.size(); i++)
	{
		m_pGuiSearchFindLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible[i]);
	}
	
	for (i = 0; i< m_pGuiSearchLineCornerCrossArray.size(); i++)
	{
		m_pGuiSearchLineCornerCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow /*&& m_tmpSearchGuiParam.m_bCircleCenterVisible*/);
	}

	for(i=0; i < m_pGuiSearchLineCenterCrossArray.size(); i++)
	{
		m_pGuiSearchLineCenterCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineCrossVisible);
	}

	for (i = 0; i < m_pGuiSearchResultLineFitPointArray1.size(); i++)
	{
		m_pGuiSearchResultLineFitPointArray1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint[0]);
	}

	for (i = 0; i< m_pGuiSearchResultLineFitPointArray2.size(); i++)
	{
		m_pGuiSearchResultLineFitPointArray2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint[1]);
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
		
		for (i=0; i<m_pGuiSearchObroundArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundArray[i], m_pGuiSearchObroundArray[i]->GetTipText());
		}
		
		for (i=0; i<m_pGuiSearchObroundCenterArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundCenterArray[i], m_pGuiSearchObroundCenterArray[i]->GetTipText());
		}
		for (i=0; i<m_pGuiSearchObroundResultArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundResultArray[i], m_pGuiSearchObroundResultArray[i]->GetTipText());
		}
		for (i=0; i<m_pGuiSearchObroundRightCircleArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchObroundRightCircleArray[i], m_pGuiSearchObroundRightCircleArray[i]->GetTipText());
		}
		for (i =0; i < m_pGuiSearchFindLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchFindLineArray[i], m_pGuiSearchFindLineArray[i]->GetTipText());
		}

		for (i = 0; i < m_pGuiSearchLineCornerCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCornerCrossArray[i], m_pGuiSearchLineCornerCrossArray[i]->GetTipText());
		}
		for(i = 0; i < m_pGuiSearchLineCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
		}

		for(i = 0; i < m_pGuiSearchLineCenterCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCenterCrossArray[i], m_pGuiSearchLineCenterCrossArray[i]->GetTipText());
		}

		for (i = 0; i < m_pGuiSearchResultLineFitPointArray1.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultLineFitPointArray1[i], m_pGuiSearchResultLineFitPointArray1[i]->GetTipText());
		}

		for (i = 0; i < m_pGuiSearchResultLineFitPointArray2.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultLineFitPointArray2[i], m_pGuiSearchResultLineFitPointArray2[i]->GetTipText());
		}
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CPatternCornerMulObroundTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternCornerMulObroundTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CPatternCornerMulObroundTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CPatternCornerMulObroundTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	*((CPatternCornerMulObroundTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CPatternCornerMulObroundTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CPatternCornerMulObroundTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CPatternCornerMulObroundTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CPatternCornerMulObroundTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CPatternCornerMulObroundTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternCornerMulObroundTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CPatternCornerMulObroundTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternCornerMulObroundTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CPatternCornerMulObroundTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置训练参数
	if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber = -1;
			return false;
		}
	}
	else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
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

		// 找长圆

		scObround cObround[OBROUND_NUM];
		double   dRMS[OBROUND_NUM];
		bool bRet1[OBROUND_NUM];
		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_FindObroundTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_FindObroundTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[i], m_tmpTrainGuiParam.m_CaliperSearchDirection[i], m_tmpTrainGuiParam.m_dCaliperSearchLen[i]
				,m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_FindObroundTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
			m_FindObroundTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[i]);
			m_FindObroundTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore[i]);
			m_FindObroundTool[i].SetObround(m_tmpTrainGuiParam.m_Obround[i]);
			if(bIsProcessImage)
			{
				bRet1[i] = m_FindObroundTool[i].Execute(tmpImage, cObround[i], dRMS[i]);
			}
			else
			{
				bRet1[i] = m_FindObroundTool[i].Execute(m_tmpImageTrain, cObround[i], dRMS[i]);
			}
		}
	
		if (m_tmpTrainDataParam.m_bEnableMulObround)
		{
			if (!bRet1[0]&&!bRet1[1])
			{
				m_nResultErrNumber=-4;return false;
			}
			else if (!bRet1[0])
			{
				m_nResultErrNumber=-2;return false;
			}
			else if (!bRet1[1])
			{
				m_nResultErrNumber=-3;return false;
			}
		}
		else
		{
			if (!bRet1[0])
			{
				m_nResultErrNumber=-2;return false;
			}
		}

	
		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_tmpTrainResult.m_dCenterX[i] = cObround[i].GetCenter().GetX();
			m_tmpTrainResult.m_dCenterY[i] = cObround[i].GetCenter().GetY();
			m_GuiTrainResultObround[i].SetObround(cObround[i]);
		}

		bool bFindLine[LINE_NUM];
		for (int i = 0; i < LINE_NUM; i++)
		{
			// 找线
			m_FindLineTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[i]);
			m_FindLineTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum[i], 
				m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i], 
				m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i], 
				m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]);

		/*	m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
				m_tmpTrainDataParam.m_edgeLineProperty[i], 
				m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);*/

			m_FindLineTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum[i]);
			m_FindLineTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bLineSortByScore[i]);
			m_FindLineTool[i].SetStartXYEndXY(
				m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], 
				m_tmpTrainGuiParam.m_dLineEndX[i], m_tmpTrainGuiParam.m_dLineEndY[i]);
			m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			if (m_tmpTrainDataParam.m_bLineEnableDualEdge[i])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
					m_tmpTrainDataParam.m_edgeLineProperty[i], tmpEdgePolarity,
					m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
			} 
			else
			{
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
					m_tmpTrainDataParam.m_edgeLineProperty[i], 
					m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
			}
			scLineSeg cLineSeg[LINE_NUM];
			double   dRMS[LINE_NUM];
			if(bIsProcessImage)
			{
				bFindLine[i] = m_FindLineTool[i].Execute(tmpImage, cLineSeg[i], dRMS[i]);
			}
			else
			{
				bFindLine[i] = m_FindLineTool[i].Execute(m_tmpImageTrain, cLineSeg[i], dRMS[i]);
			}
			
			if (!bFindLine[i])
			{
				continue;
			}
			m_tmpTrainResult.m_dStartX[i] = cLineSeg[i].GetP1().GetX();
			m_tmpTrainResult.m_dStartY[i] = cLineSeg[i].GetP1().GetY();

			m_tmpTrainResult.m_dEndX[i] = cLineSeg[i].GetP2().GetX();
			m_tmpTrainResult.m_dEndY[i] = cLineSeg[i].GetP2().GetY();

			m_GuiTrainResultLine[i].SetLine(cLineSeg[i].GetLine());
		}
		if (!bFindLine[0]&&!bFindLine[1])
		{
			m_nResultErrNumber=-7;return false;
		}
		else if (!bFindLine[0])
		{
			m_nResultErrNumber=-5;return false;
		}
		else if (!bFindLine[1])
		{
			m_nResultErrNumber=-6;return false;
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
			m_nResultErrNumber=-1;      
			return false;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
		
		// 找圆
		scObround cObround[OBROUND_NUM];
		double   dRMS[OBROUND_NUM];
		bool bRet1[OBROUND_NUM];
		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_FindObroundTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_FindObroundTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum[i], m_tmpTrainGuiParam.m_CaliperSearchDirection[i], m_tmpTrainGuiParam.m_dCaliperSearchLen[i]
			,m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_FindObroundTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[i], m_tmpTrainDataParam.m_edgeProperty[i], m_tmpTrainDataParam.m_lFilterHalfWidth[i]);
			m_FindObroundTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum[i]);
			m_FindObroundTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore[i]);
			m_FindObroundTool[i].SetObround(m_tmpTrainGuiParam.m_Obround[i]);
			if(bIsProcessImage)
			{
				bRet1[i] = m_FindObroundTool[i].Execute(tmpImage, cObround[i], dRMS[i]);
			}
			else
			{
				bRet1[i] = m_FindObroundTool[i].Execute(m_tmpImageTrain, cObround[i], dRMS[i]);
			}
		}

		if (m_tmpTrainDataParam.m_bEnableMulObround)
		{
			if (!bRet1[0]&&!bRet1[1])
			{
				m_nResultErrNumber=-4;return false;
			}
			else if (!bRet1[0])
			{
				m_nResultErrNumber=-2;return false;
			}
			else if (!bRet1[1])
			{
				m_nResultErrNumber=-3;return false;
			}
		}
		else
		{
			if (!bRet1[0])
			{
				m_nResultErrNumber=-2;return false;
			}
		}
		

		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_tmpTrainResult.m_dCenterX[i] = cObround[i].GetCenter().GetX();
			m_tmpTrainResult.m_dCenterY[i] = cObround[i].GetCenter().GetY();
			m_GuiTrainResultObround[i].SetObround(cObround[i]);
		}
		bool bFindLine[LINE_NUM];
		for (int i = 0; i < LINE_NUM; i++)
		{
			// 找线
			m_FindLineTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[i]);
			m_FindLineTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum[i], 
				m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i], 
				m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i], 
				m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]);

		 /*m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
				m_tmpTrainDataParam.m_edgeLineProperty[i], 
				m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);*/

			m_FindLineTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum[i]);
			m_FindLineTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bLineSortByScore[i]);
			m_FindLineTool[i].SetStartXYEndXY(
				m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], 
				m_tmpTrainGuiParam.m_dLineEndX[i], m_tmpTrainGuiParam.m_dLineEndY[i]);
			m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);

			if (m_tmpTrainDataParam.m_bLineEnableDualEdge[i])
			{
				EdgePolarity tmpEdgePolarity = eDontCare;
				if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eDarkToLight)
				{
					tmpEdgePolarity = eLightToDark;
				} 
				else if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eLightToDark)
				{
					tmpEdgePolarity = eDarkToLight;
				}
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
					m_tmpTrainDataParam.m_edgeLineProperty[i], tmpEdgePolarity,
					m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
			} 
			else
			{
				m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
					m_tmpTrainDataParam.m_edgeLineProperty[i], 
					m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
			}
			scLineSeg cLineSeg[LINE_NUM];
			double   dRMS[LINE_NUM];
			if(bIsProcessImage)
			{
				bFindLine[i] = m_FindLineTool[i].Execute(tmpImage, cLineSeg[i], dRMS[i]);
			}
			else
			{
				bFindLine[i] = m_FindLineTool[i].Execute(m_tmpImageTrain, cLineSeg[i], dRMS[i]);
			}
			if (!bFindLine[i])
			{
				continue;
			}
			m_tmpTrainResult.m_dStartX[i] = cLineSeg[i].GetP1().GetX();
			m_tmpTrainResult.m_dStartY[i] = cLineSeg[i].GetP1().GetY();

			m_tmpTrainResult.m_dEndX[i] = cLineSeg[i].GetP2().GetX();
			m_tmpTrainResult.m_dEndY[i] = cLineSeg[i].GetP2().GetY();

			m_GuiTrainResultLine[i].SetLine(cLineSeg[i].GetLine());
		}
		if (!bFindLine[0]&&!bFindLine[1])
		{
			m_nResultErrNumber=-7;return false;
		}
		else if (!bFindLine[0])
		{
			m_nResultErrNumber=-5;return false;
		}
		else if (!bFindLine[1])
		{
			m_nResultErrNumber=-6;return false;
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
bool CPatternCornerMulObroundTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CPatternCornerMulObroundTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	*((CPatternCornerMulObroundTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}	

// 是否训练成功
bool CPatternCornerMulObroundTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CPatternCornerMulObroundTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternCornerMulObroundSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternCornerMulObroundTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}

	*((CPatternCornerMulObroundSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternCornerMulObroundTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CPatternCornerMulObroundSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CPatternCornerMulObroundTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != ePatternCornerMulObroundTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CPatternCornerMulObroundSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CPatternCornerMulObroundTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber =- 99;
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
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber = -1;
		return false;
	}

	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_FindObroundTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindObroundTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchDirection[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
			,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		m_FindObroundTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
		m_FindObroundTool[i].SetOutLineNum(m_TrainDataParam.m_nOutLineNum[i]);
		m_FindObroundTool[i].EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore[i]);
	}


	for (int i = 0; i < LINE_NUM; i++)
	{
		// 找线
		m_FindLineTool[i].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum[i], 
			m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i], 
			m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i], 
			m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]);

		/*m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
			m_tmpTrainDataParam.m_edgeLineProperty[i], 
			m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);*/

		m_FindLineTool[i].SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum[i]);
		m_FindLineTool[i].EnableCaliperSortByScore(m_tmpTrainDataParam.m_bLineSortByScore[i]);
		m_FindLineTool[i].SetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
		// 
		// 		m_FindLineTool[i].SetStartXYEndXY(
		// 			m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i], 
		// 			m_tmpTrainGuiParam.m_dLineEndX[i], m_tmpTrainGuiParam.m_dLineEndY[i]);

		if (m_tmpTrainDataParam.m_bLineEnableDualEdge[i])
		{
			EdgePolarity tmpEdgePolarity = eDontCare;
			if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eDarkToLight)
			{
				tmpEdgePolarity = eLightToDark;
			} 
			else if (m_tmpTrainDataParam.m_edgeLineProperty[i] == eLightToDark)
			{
				tmpEdgePolarity = eDarkToLight;
			}
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
				m_tmpTrainDataParam.m_edgeLineProperty[i], tmpEdgePolarity,
				m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
		} 
		else
		{
			m_FindLineTool[i].SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre[i], 
				m_tmpTrainDataParam.m_edgeLineProperty[i], 
				m_tmpTrainDataParam.m_lLineFilterHalfWidth[i]);
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
		if(nResultNum<=0)
		{
			m_nResultErrNumber=-1;
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
			
			// 求找线工具变换后的位置,找线
			double dTransStartX[LINE_NUM], dTransStartY[LINE_NUM];
			double dTransEndX[LINE_NUM], dTransEndY[LINE_NUM];
			for (int j=0; j<LINE_NUM; j++)
			{
				dTransStartX[j] = cos(dRotation) * m_TrainGuiParam.m_dLineStartX[j]  - sin(dRotation) * m_TrainGuiParam.m_dLineStartY[j] + dTransX;
				dTransStartY[j] = sin(dRotation) * m_TrainGuiParam.m_dLineStartX[j]  + cos(dRotation) * m_TrainGuiParam.m_dLineStartY[j] + dTransY;
				dTransEndX[j] = cos(dRotation) * m_TrainGuiParam.m_dLineEndX[j]  - sin(dRotation) * m_TrainGuiParam.m_dLineEndY[j] + dTransX;
				dTransEndY[j] = sin(dRotation) * m_TrainGuiParam.m_dLineEndX[j]  + cos(dRotation) * m_TrainGuiParam.m_dLineEndY[j] + dTransY;
				m_FindLineTool[j].SetCaliperParams(m_TrainGuiParam.m_nLineCaliperNum[j], /*scRadian(dRotation)+*/m_TrainGuiParam.m_LineCaliperSearchRotation[j],
					m_TrainGuiParam.m_dLineCaliperSearchLen[j],m_TrainGuiParam.m_dLineCaliperProjectLen[j]);
				m_FindLineTool[j].SetStartXYEndXY(dTransStartX[j], dTransStartY[j], dTransEndX[j], dTransEndY[j]);
			}

			// 求找线工具变换后的位置,找线
			double dTransCenterX[OBROUND_NUM], dTransCenterY[OBROUND_NUM];
			double dOriCenterX[OBROUND_NUM], dOriCenterY[OBROUND_NUM];

			for (int j=0;j<OBROUND_NUM;j++)
			{
				dOriCenterX[j] = m_TrainGuiParam.m_Obround[j].GetCenter().GetX();
				dOriCenterY[j] = m_TrainGuiParam.m_Obround[j].GetCenter().GetY();
				dTransCenterX[j] = cos(dRotation) * dOriCenterX[j]  - sin(dRotation) * dOriCenterY[j] + dTransX;
				dTransCenterY[j] = sin(dRotation) * dOriCenterX[j]  + cos(dRotation) * dOriCenterY[j] + dTransY;
				m_FindObroundTool[j].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[j], m_TrainGuiParam.m_CaliperSearchDirection[j], m_TrainGuiParam.m_dCaliperSearchLen[j]
					,m_TrainGuiParam.m_dCaliperProjectLen[j]);
				m_FindObroundTool[j].SetCenterLengthsRotation(sc2Vector(dTransCenterX[j], dTransCenterY[j]), m_TrainGuiParam.m_Obround[j].GetXLength(),
					m_TrainGuiParam.m_Obround[j].GetYLength(), scRadian(dRotation)+m_TrainGuiParam.m_Obround[j].GetRotation());
			}
			
			// 执行找线
			scLineSeg cLineSeg[LINE_NUM];
			double dLineRMS[LINE_NUM];
			bool bFindLine[LINE_NUM];
			for(int k = 0; k < LINE_NUM; k++)
			{
				if(bIsProcessImage)
				{
					bFindLine[k] = m_FindLineTool[k].Execute(tmpImage, cLineSeg[k], dLineRMS[k]);
				}
				else
				{
					bFindLine[k] = m_FindLineTool[k].Execute(ImageSearch, cLineSeg[k], dLineRMS[k]);
				}

				/*if (!m_FindLineTool[k].Execute(ImageSearch, cLineSeg[k], dLineRMS[k]))
				{
					bFindLine[k]=false;continue;
				}
				bFindLine[k]=true;*/
			}
			if(!bFindLine[0]&&!bFindLine[1])
			{
				m_nResultErrNumber=-7;return false;
			}
			if(!bFindLine[0])
			{
				m_nResultErrNumber=-5;return false ;
			}
			if(!bFindLine[1])
			{
				m_nResultErrNumber=-6;return false;
			}

			// 执行找圆
			scObround ResObround[OBROUND_NUM];
			double dRMS[OBROUND_NUM];
			bool bFindObround[OBROUND_NUM];

			for (int j=0;j<OBROUND_NUM;j++)
			{
				if (m_TrainDataParam.m_bEnableMulObround)
				{
					if(bIsProcessImage)
					{
						bFindObround[j] = m_FindObroundTool[j].Execute(tmpImage, ResObround[j], dRMS[j]);
					}
					else
					{
						bFindObround[j] = m_FindObroundTool[j].Execute(ImageSearch, ResObround[j], dRMS[j]);
					}
					/*if (!m_FindObroundTool[j].Execute(ImageSearch, ResObround[j], dRMS[j]))
					{
					bFindObround[j]=false;continue;
					}
					bFindObround[j]=true;*/
				}
				else
				{
					if (j==0)
					{
						bool bObroundExecute = false;
						if(bIsProcessImage)
						{
							bObroundExecute = m_FindObroundTool[0].Execute(tmpImage, ResObround[j], dRMS[j]);
						}
						else
						{
							bObroundExecute = m_FindObroundTool[0].Execute(ImageSearch, ResObround[j], dRMS[j]);
						}
						if (!bObroundExecute)
						{
							bFindObround[0]=false;
							bFindObround[1]=true;
							continue;
						}
					}
// 					bFindObround[0]=true;
// 					bFindObround[1]=true;
					break;
				}
			}
			if(!bFindObround[0]&&!bFindObround[1])
			{
				m_nResultErrNumber=-4;return false;
			}
			if(!bFindObround[0])
			{
				m_nResultErrNumber=-2;return false ;
			}
			if(!bFindObround[1])
			{
				m_nResultErrNumber=-3;return false;
			}

			// 成功
			{
				CSearchResult searchResult;

				// 计算交点
				sc2Vector vLineCrossPoint;
				std::vector<sc2Vector> crossPointArray; 
				sc2Vector pointCenter1;
				sc2Vector pointCenter2;

				if (!CalcLineCross(cLineSeg[0], cLineSeg[1], pointCenter1, pointCenter2, crossPointArray))
					return false;

				searchResult.m_dPosX = crossPointArray[0].GetX();
				searchResult.m_dPosY = crossPointArray[0].GetY();
				double dRotAngle = scDegree((cLineSeg[0].GetRotation().SignedNormMod180()-modelLineSeg1.GetRotation().SignedNormMod180()
					+ cLineSeg[1].GetRotation().SignedNormMod180()-modelLineSeg2.GetRotation().SignedNormMod180())/2.0).ToDouble(); 
				searchResult.m_dAngle = dRotAngle;
				searchResult.m_dScore = tmpPatternResult.GetScore();

				searchResult.m_dLineAngle1 = scDegree(cLineSeg[0].GetRotation().SignedNormMod180()).ToDouble();
				searchResult.m_dLineAngle2 = scDegree(cLineSeg[1].GetRotation().SignedNormMod180()).ToDouble();

				// 辅助点1-卡尺的中点
				double dAngleLine0 = scDegree(cLineSeg[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()).ToDouble();
				searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
				searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();
				searchResult.m_vdAuxiliaryAngle[0] = dAngleLine0;

				// 辅助点2-卡尺的中点
				double dAngleLine1 = scDegree(cLineSeg[1].GetRotation().NormMod180()-modelLineSeg2.GetRotation().NormMod180()).ToDouble();
				searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
				searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();
				searchResult.m_vdAuxiliaryAngle[1] = dAngleLine1;

				sc2Vector pointResultLeft;
				sc2Vector pointResultCenter;
				sc2Vector pointResultRight;
				// 辅助点3-左圆；中点；右圆
				if (m_TrainDataParam.m_bEnableMulObround)
				{		
					pointResultLeft=(ResObround[0].GetCircularArcXO().GetCircleCenter()+ResObround[1].GetCircularArcXO().GetCircleCenter())/2;
					pointResultCenter.SetX((ResObround[0].GetCenter().GetX()+ResObround[1].GetCenter().GetX())/2.0);
					pointResultCenter.SetY((ResObround[0].GetCenter().GetY()+ResObround[1].GetCenter().GetY())/2.0);
					pointResultRight=(ResObround[0].GetCircularArcYOPP().GetCircleCenter()+ResObround[1].GetCircularArcYOPP().GetCircleCenter())/2;
					switch(m_tmpSearchDataParam.m_nObroundOutputResult)
					{
					case 0://左圆
						searchResult.m_vdAuxiliaryPosX[2] = pointResultLeft.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultLeft.GetY();

						break;
					case 1://中点	
						searchResult.m_vdAuxiliaryPosX[2] = pointResultCenter.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultCenter.GetY();

						break;
					case 2://右圆
						searchResult.m_vdAuxiliaryPosX[2] = pointResultRight.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultRight.GetY();
						break;
					default:
						searchResult.m_vdAuxiliaryPosX[2] = pointResultCenter.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultCenter.GetY();
						break;
					}
				}
				else
				{
					pointResultLeft=ResObround[0].GetCircularArcXO().GetCircleCenter();
					pointResultCenter=ResObround[0].GetCenter();
					pointResultRight=ResObround[0].GetCircularArcYOPP().GetCircleCenter();

					switch(m_tmpSearchDataParam.m_nObroundOutputResult)
					{
					case 0://左圆
						searchResult.m_vdAuxiliaryPosX[2] = pointResultLeft.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultLeft.GetY();

						break;
					case 1://中点	
						searchResult.m_vdAuxiliaryPosX[2] = pointResultCenter.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultCenter.GetY();

						break;
					case 2://右圆
						searchResult.m_vdAuxiliaryPosX[2] = pointResultRight.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultRight.GetY();

						break;
					default:
						searchResult.m_vdAuxiliaryPosX[2] = pointResultCenter.GetX();
						searchResult.m_vdAuxiliaryPosY[2] = pointResultCenter.GetY();
						break;
					}
				}
							
				searchResult.m_bFindLine1 = true;
				searchResult.m_bFindLine2 = true;

				// 设置结果
				CResultSearchRect resRect;
				resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);
				// 线
				for (int nLine = 0; nLine < LINE_NUM; nLine++)
				{
					CResultLine line;
					line.m_vLineX[0] = cLineSeg[nLine].GetP1().GetX();
					line.m_vLineY[0] = cLineSeg[nLine].GetP1().GetY();

					line.m_vLineX[1] = cLineSeg[nLine].GetP2().GetX();
					line.m_vLineY[1] = cLineSeg[nLine].GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);
				}
				// 长圆
				for (int nObround = 0; nObround < OBROUND_NUM; nObround++)
				{
					if (nObround !=1 || m_tmpTrainDataParam.m_bEnableMulObround)
					{
						 CResultObround obround;
						 obround.m_scObround = ResObround[nObround];
						 searchResult.m_vObroundSearchResult.push_back(obround);
					}
				}


				m_SearchResultArray.push_back(searchResult);
				
				
				//训练区域
				CString	strTmpKey;
				strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
				//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);
				
				// 找线中心点
				scGuiCross *pTempCross = m_GuiSearchCross+i;
				strTmpKey.Format(_T("%s_ResultCross1_%d"), m_strKeyText, i);
				pTempCross = m_GuiSearchLineCenterCross+i*2;
				pTempCross->SetCenterExtent(pointCenter1, sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);			
				m_pGuiSearchLineCenterCrossArray.push_back(pTempCross);

				strTmpKey.Format(_T("%s_ResultCross2_%d"), m_strKeyText, i);
				pTempCross = m_GuiSearchLineCenterCross+i*2+1;
				pTempCross->SetCenterExtent(pointCenter2, sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);			
				m_pGuiSearchLineCenterCrossArray.push_back(pTempCross);

				//直线1

				for (int k=0; k< LINE_NUM; k++)
				{
					strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, k, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchFindLine+i*2+k;
					pTemLine->SetLine(cLineSeg[k].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchFindLineArray.push_back(pTemLine);
				}
				//角主点
				strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
				pTempCross = m_GuiSearchLineCross+i;
				pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY), sc2Vector(60,60));
				pTempCross->SetLineColor(RGB(255, 0 ,0));
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				CString st;
				st.Format(_T("%d X:%.1f;Y:%.1f;D:%.1f"),i, searchResult.m_dPosX, searchResult.m_dPosY,searchResult.m_dAngle);
				pTempCross->SetLabel(st);
				pTempCross->SetLabelVisible(TRUE);
				m_pGuiSearchLineCrossArray.push_back(pTempCross);

				//标记点
				strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
				pTempCross = m_GuiSearchCross+i;
				pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
				pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);
				m_pGuiSearchCrossArray.push_back(pTempCross);
				
				//长圆
				if (m_TrainDataParam.m_bEnableMulObround)
				{
					for (int k=0;k<OBROUND_NUM;k++)
					{
						strTmpKey.Format(_T("%s_ResultObround%d_%d"), m_strKeyText,k, i);
						scGuiObround *pTemObround = m_GuiSearchObround+i*2+k;
						pTemObround->SetObround(ResObround[k]);
						pTemObround->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
						pTemObround->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTemObround->SetTipTextEnabled(true);
						pTemObround->SetTipText(strTmpKey);
						m_pGuiSearchObroundArray.push_back(pTemObround);
					}
					switch(m_tmpSearchDataParam.m_nObroundOutputResult)
					{
						scGuiCross *pTempObroundCenter;
						scGuiCross *pTempObroundResult;
					case 0://左圆
						for (int k=0;k<OBROUND_NUM;k++)
						{
							strTmpKey.Format(_T("%s_ResultObroundCenter%d_%d"), m_strKeyText,k,i);
							pTempObroundCenter = m_GuiSearchObroundCenter+i*2+k;
							pTempObroundCenter->SetCenterExtent(ResObround[k].GetCircularArcXO().GetCircleCenter(), sc2Vector(40,40));
							pTempObroundCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempObroundCenter->SetTipTextEnabled(true);
							pTempObroundCenter->SetTipText(strTmpKey);
							pTempObroundCenter->SetLabelVisible(false);
							m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);	
						}
						strTmpKey.Format(_T("%s_ResultObroundResult%d"), m_strKeyText,i);
						pTempObroundResult = m_GuiSearchObroundResult+i;
						pTempObroundResult->SetCenterExtent(pointResultLeft, sc2Vector(60,60));
						pTempObroundResult->SetLineColor(RGB(255,0,0));
						pTempObroundResult->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundResult->SetTipTextEnabled(true);
						pTempObroundResult->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultLeft.GetX(), pointResultLeft.GetY());
						pTempObroundResult->SetLabel(st);
						pTempObroundResult->SetLabelVisible(TRUE);
						m_pGuiSearchObroundResultArray.push_back(pTempObroundResult);

						break;
					case 1://左圆
						for (int k=0;k<OBROUND_NUM;k++)
						{
							strTmpKey.Format(_T("%s_ResultObroundCenter%d_%d"), m_strKeyText,k,i);
							pTempObroundCenter = m_GuiSearchObroundCenter+i*2+k;
							pTempObroundCenter->SetCenterExtent(ResObround[k].GetCenter(), sc2Vector(40,40));
							pTempObroundCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempObroundCenter->SetTipTextEnabled(true);
							pTempObroundCenter->SetTipText(strTmpKey);
							pTempObroundCenter->SetLabelVisible(false);
							m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);	
						}
						strTmpKey.Format(_T("%s_ResultObroundResult%d"), m_strKeyText,i);
						pTempObroundResult = m_GuiSearchObroundResult+i;
						pTempObroundResult->SetCenterExtent(pointResultCenter, sc2Vector(60,60));
						pTempObroundResult->SetLineColor(RGB(255,0,0));
						pTempObroundResult->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundResult->SetTipTextEnabled(true);
						pTempObroundResult->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultCenter.GetX(), pointResultCenter.GetY());
						pTempObroundResult->SetLabel(st);
						pTempObroundResult->SetLabelVisible(TRUE);
						m_pGuiSearchObroundResultArray.push_back(pTempObroundResult);


						break;
					case 2://左圆
						for (int k=0;k<OBROUND_NUM;k++)
						{
							strTmpKey.Format(_T("%s_ResultObroundCenter%d_%d"), m_strKeyText,k,i);
							pTempObroundCenter = m_GuiSearchObroundCenter+i*2+k;
							pTempObroundCenter->SetCenterExtent(ResObround[k].GetCircularArcYOPP().GetCircleCenter(), sc2Vector(40,40));
							pTempObroundCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
							pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
							pTempObroundCenter->SetTipTextEnabled(true);
							pTempObroundCenter->SetTipText(strTmpKey);
							pTempObroundCenter->SetLabelVisible(false);
							m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);	
						}
						strTmpKey.Format(_T("%s_ResultObroundResult%d"), m_strKeyText,i);
						pTempObroundResult = m_GuiSearchObroundResult+i;
						pTempObroundResult->SetCenterExtent(pointResultRight, sc2Vector(60,60));
						pTempObroundResult->SetLineColor(RGB(255,0,0));
						pTempObroundResult->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundResult->SetTipTextEnabled(true);
						pTempObroundResult->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultRight.GetX(), pointResultRight.GetY());
						pTempObroundResult->SetLabel(st);
						pTempObroundResult->SetLabelVisible(TRUE);
						m_pGuiSearchObroundResultArray.push_back(pTempObroundResult);


						break;
					default:
						break;
					}
				}
				else
				{
					strTmpKey.Format(_T("%s_ResultObround%d"), m_strKeyText, i);
					scGuiObround *pTemObround = m_GuiSearchObround+i;
					pTemObround->SetObround(ResObround[0]);
					pTemObround->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemObround->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemObround->SetTipTextEnabled(true);
					pTemObround->SetTipText(strTmpKey);
					m_pGuiSearchObroundArray.push_back(pTemObround);


					switch(m_tmpSearchDataParam.m_nObroundOutputResult)
					{
						scGuiCross *pTempObroundCenter;
					case 0://左圆
						strTmpKey.Format(_T("%s_ResultObroundCenter%d"), m_strKeyText,i);
						pTempObroundCenter = m_GuiSearchObroundCenter+i;
						pTempObroundCenter->SetCenterExtent(pointResultLeft, sc2Vector(60,60));
						pTempObroundCenter->SetLineColor(RGB(255,0,0));
						pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundCenter->SetTipTextEnabled(true);
						pTempObroundCenter->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultLeft.GetX(), pointResultLeft.GetY());
						pTempObroundCenter->SetLabel(st);
						pTempObroundCenter->SetLabelVisible(TRUE);
						m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);
						break;
					case 1://左圆
						strTmpKey.Format(_T("%s_ResultObroundCenter%d"), m_strKeyText,i);
						pTempObroundCenter = m_GuiSearchObroundCenter+i;
						pTempObroundCenter->SetCenterExtent(pointResultCenter, sc2Vector(60,60));
						pTempObroundCenter->SetLineColor(RGB(255,0,0));
						pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundCenter->SetTipTextEnabled(true);
						pTempObroundCenter->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultCenter.GetX(), pointResultCenter.GetY());
						pTempObroundCenter->SetLabel(st);
						pTempObroundCenter->SetLabelVisible(TRUE);
						m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);
						break;
					case 2://左圆
						strTmpKey.Format(_T("%s_ResultObroundCenter%d"), m_strKeyText,i);
						pTempObroundCenter = m_GuiSearchObroundCenter+i;
						pTempObroundCenter->SetCenterExtent(pointResultRight, sc2Vector(60,60));
						pTempObroundCenter->SetLineColor(RGB(255,0,0));
						pTempObroundCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
						pTempObroundCenter->SetTipTextEnabled(true);
						pTempObroundCenter->SetTipText(strTmpKey);
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, pointResultRight.GetX(), pointResultRight.GetY());
						pTempObroundCenter->SetLabel(st);
						pTempObroundCenter->SetLabelVisible(TRUE);
						m_pGuiSearchObroundCenterArray.push_back(pTempObroundCenter);
						break;
					default:
						break;
					}

				}

				
	

				//////////////////////////////////////////////////////////////////////////
				{
					std::vector<bool> bUsed;
					std::vector<sc2Vector> points;
					m_FindLineTool[0].GetUsed(bUsed);
					m_FindLineTool[0].GetFitData(points);
					for (long m=0; m<bUsed.size(); m++)
					{
						bool bUse = bUsed[m];
						sc2Vector pt = points[m];

						int nIndex = m_pGuiSearchResultLineFitPointArray1.size();
						strTmpKey.Format(_T("%s_FitPoint1_%d"), m_strKeyText, nIndex);
						scGuiCoordCross *pTempFitPoint1 = m_GuiSearchResultLineFitPoint1 + nIndex;
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
						m_pGuiSearchResultLineFitPointArray1.push_back(pTempFitPoint1);	
					}
				}
				{
					std::vector<bool> bUsed;
					std::vector<sc2Vector> points;
					m_FindLineTool[1].GetUsed(bUsed);
					m_FindLineTool[1].GetFitData(points);
					for (long m=0; m<bUsed.size(); m++)
					{
						bool bUse = bUsed[m];
						sc2Vector pt = points[m];

						int nIndex = m_pGuiSearchResultLineFitPointArray2.size();
						strTmpKey.Format(_T("%s_FitPoint2_%d"), m_strKeyText, nIndex);
						scGuiCoordCross *pTempFitPoint2 = m_GuiSearchResultLineFitPoint2 + nIndex;
						pTempFitPoint2->SetXDirectionAdornment(scGuiCoordCross::eNone);
						pTempFitPoint2->SetYDirectionAdornment(scGuiCoordCross::eNone);
						pTempFitPoint2->SetRulerVisible(false);
						pTempFitPoint2->SetCenterRotationLengths(pt,scRadian(scDegree(45)),13,13);
						if (bUse)
						{
							pTempFitPoint2->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint1);
						}
						else
						{
							pTempFitPoint2->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint1);
						}
						pTempFitPoint2->SetTipTextEnabled(true);
						pTempFitPoint2->SetTipText(strTmpKey);
						m_pGuiSearchResultLineFitPointArray2.push_back(pTempFitPoint2);	
					}
				}
			}	

		}		

		if (m_SearchResultArray.size() <= 0)
		{
			m_nResultErrNumber=-1;
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

	return true;
}

// 获取搜索结果个数										
int CPatternCornerMulObroundTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternCornerMulObroundTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
			
// 加载模板训练图像
bool CPatternCornerMulObroundTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternCornerMulObroundTool)
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
bool CPatternCornerMulObroundTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) PatternSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CPatternCornerMulObroundTrainDataParam tmpTrainDataParam;
	CPatternCornerMulObroundTrainGuiParam tmpTrainGuiParam;
	CPatternCornerMulObroundSearchDataParam tmpSearchDataParam;
	CPatternCornerMulObroundSearchGuiParam tmpSearchGuiParam;
	CPatternCornerMulObroundTrainResult  tmpTrainResult;
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

	/////////////CameraParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("CamParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	{
		bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
		if (!bRet)
		{
			return false;
		}
		cXMLConfigurator.GetElemData(m_CamHeight);

		bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
		if (!bRet)
		{
			return false;
		}
		cXMLConfigurator.GetElemData(m_CamWidth);
	}
	cXMLConfigurator.LeaveElem();


	/////////////工具类型/////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("ToolType"));
	if (!bRet)
	{
		return false;
	}
	CSearchToolType cSearchToolType = (CSearchToolType)_ttoi(cXMLConfigurator.GetElemData());
	if (cSearchToolType != ePatternCornerMulObroundTool)
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

	// contrast thre
	
	//liuzhikuan
	for (int i=0;i<OBROUND_NUM;i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("ContrastThre%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dContrastThre[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// FilterHalfWidth
		strTemp.Format(_T("FilterHalfWidth%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_lFilterHalfWidth[i] = 1;
		}
		else
		{
			tmpTrainDataParam.m_lFilterHalfWidth[i] = _ttol(cXMLConfigurator.GetElemData());
		}
		// outlinenum
		strTemp.Format(_T("OutLineNum%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_nOutLineNum[i] = _ttol(cXMLConfigurator.GetElemData());
		// edgeproperty
		strTemp.Format(_T("EdgeProperty%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_edgeProperty[i] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());

		// SortByScore
		strTemp.Format(_T("SortByScore%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_bSortByScore[i] = true;
		}
		tmpTrainDataParam.m_bSortByScore[i] = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableMulObround"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bEnableMulObround = _ttoi(cXMLConfigurator.GetElemData());
	

	for(int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		//constrast thre
		strTemp.Format(_T("LineContrastThre%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}

		tmpTrainDataParam.m_dLineContrastThre[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);

		// FilterHalfWidth
		strTemp.Format(_T("LineFilterHalfWidth%d"), i);

		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_lLineFilterHalfWidth[i] = 1;
		}
		else
		{
			tmpTrainDataParam.m_lLineFilterHalfWidth[i] = _ttol(cXMLConfigurator.GetElemData());
		}

		// outlinenum
		strTemp.Format(_T("LineOutLineNum%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_nLineOutLineNum[i] = _ttol(cXMLConfigurator.GetElemData());

		// edgeproperty
		strTemp.Format(_T("LineEdgeProperty%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_edgeLineProperty[i] = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());

		// SortByScore
		strTemp.Format(_T("LineSortByScore%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bLineSortByScore[i] = _ttoi(cXMLConfigurator.GetElemData());

		// DualEdge1
		strTemp.Format(_T("LineDualEdge%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainDataParam.m_bLineEnableDualEdge[i] = false;
		}
		else
		{
			tmpTrainDataParam.m_bLineEnableDualEdge[i] = _ttoi(cXMLConfigurator.GetElemData());
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

	for (int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp =  _T("");
		strTemp.Format(_T("LineCaliperNum%d"), i);
		// CaliperNum1
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_nLineCaliperNum[i] = _ttoi(cXMLConfigurator.GetElemData());
		// startX
		strTemp.Format(_T("LineStartX%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineStartX[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// startY
		strTemp.Format(_T("LineStartY%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineStartY[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// EndX
		strTemp.Format(_T("LineEndX%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineEndX[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// EndY
		strTemp.Format(_T("LineEndY%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineEndY[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Project Len
		strTemp.Format(_T("LineProjectLen%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineCaliperProjectLen[0] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Len
		strTemp.Format(_T("LineSearchLen%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dLineCaliperSearchLen[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Rotation
		strTemp.Format(_T("LineSearchRotation%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_LineCaliperSearchRotation[i] = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		// Caliper Visible
		strTemp.Format(_T("LineCaliperVisible%d"),i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			tmpTrainGuiParam.m_bLineCaliperVisible[i]=true;
		}
		else
		{
			tmpTrainGuiParam.m_bLineCaliperVisible[i] = _ttoi(cXMLConfigurator.GetElemData());
		}

		strTemp.Format(_T("LineCaliperMaskEnable%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (bRet)
		{
			tmpTrainGuiParam.m_bLineEnableLineMask[i] = _ttol(cXMLConfigurator.GetElemData());
		}
		else
		{
			tmpTrainGuiParam.m_bLineEnableLineMask[i] = false;
		}

		strTemp.Format(_T("LineMaskParam%d"), i);

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

	// Obround
	bRet = cXMLConfigurator.FindElem(_T("Obround"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();

	//liuzhikuan
	for (int i=0;i<OBROUND_NUM;i++)
	{
		CString StrTemp=_T("");
		StrTemp.Format("CenterX%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}	
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		StrTemp.Format("CenterY%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}	
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_Obround[i].SetCenter(vPos);

		StrTemp.Format("XLength%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_Obround[i].SetXLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		StrTemp.Format("YLength%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_Obround[i].SetYLength(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		StrTemp.Format("XRotation%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_Obround[i].SetRotation(scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL)));
		
		// Caliper Num 
		StrTemp.Format("CaliperNum%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_nCaliperNum[i] = _ttoi(cXMLConfigurator.GetElemData());
		// Project Len
		StrTemp.Format("ProjectLen%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCaliperProjectLen[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Len
		StrTemp.Format("SearchLen%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_dCaliperSearchLen[i] = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		// Search Dirction
		StrTemp.Format("SearchDir%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		tmpTrainGuiParam.m_CaliperSearchDirection[i] = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());
	}
	cXMLConfigurator.LeaveElem();

// 	// color
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
// 	//  search rect visible 
// 	bRet = cXMLConfigurator.FindElem(_T("SearchRectVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bSearchRectVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	}
// 	// circle visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleVisible"));
// 	if (!bRet)
// 	{
// 		tmpTrainGuiParam.m_bCircleVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	bRet = cXMLConfigurator.FindElem(_T("ObroundOutputResult"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_nObroundOutputResult = _ttoi(cXMLConfigurator.GetElemData());

	for (int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("LineFitPointVisible%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		tmpSearchGuiParam.m_bEnableShowLinePoint[i] = _ttoi(cXMLConfigurator.GetElemData());
	}
// 
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
// 	if (!bRet)
// 	{
// 		return false;
// 	}
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
// 	// circle visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bCircleVisible = _ttoi(cXMLConfigurator.GetElemData());
// 	// circle center visible 
// 	bRet = cXMLConfigurator.FindElem(_T("CircleCenterVisible"));
// 	if (!bRet)
// 	{
// 		return false;
// 	}
// 	tmpSearchGuiParam.m_bCircleCenterVisible = _ttoi(cXMLConfigurator.GetElemData());
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

	//liuzhikuan
	for (int i=0;i<OBROUND_NUM;i++)
	{
		CString StrTemp=_T("");
		StrTemp.Format("CenterX%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		StrTemp.Format("CenterY%d",i);
		bRet = cXMLConfigurator.FindElem(StrTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainResult.m_dCenterX[i] = vPos.GetX();
		tmpTrainResult.m_dCenterY[i] = vPos.GetY();
	}


	for(int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		// line 0
		strTemp.Format(_T("StartX%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		strTemp.Format(_T("StartY%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainResult.m_dStartX[i] = vPos.GetX();
		tmpTrainResult.m_dStartY[i] = vPos.GetY();

		strTemp.Format(_T("EndX%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

		strTemp.Format(_T("EndY%d"), i);
		bRet = cXMLConfigurator.FindElem(strTemp);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainResult.m_dEndX[i] = vPos.GetX();
		tmpTrainResult.m_dEndY[i] = vPos.GetY();
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
	m_tmpImageTrain = tmpIImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	m_PatternSearchTool = tmpPatternSearchTool;

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
bool CPatternCornerMulObroundTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) PatternSearch模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	////////////CameraParam//////////
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

	// contrastthre
	for (int j=0;j<OBROUND_NUM;j++)
	{
		CString strTemp=_T("");
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre[j]);
		strTemp.Format(_T("ContrastThre%d"),j);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// fileterhalfwidth
		strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth[j]);
		strTemp.Format(_T("FilterHalfWidth%d"),j);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// outlinenum
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum[j]);
		strTemp.Format(_T("OutLineNum%d"),j);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// edgeproperty
		strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty[j]);
		strTemp.Format(_T("EdgeProperty%d"),j);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// SortByScore
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore[j]);
		strTemp.Format(_T("SortByScore%d"),j);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		
	}
	
	//
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableMulObround);
	cXMLConfigurator.AddElem(_T("EnableMulObround"), strInfo);


	for (int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		// contrastthre1
		strTemp.Format(_T("LineContrastThre%d"), i);
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dLineContrastThre[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// fileterhalfwidth1
		strTemp.Format(_T("LineFilterHalfWidth%d"), i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_lLineFilterHalfWidth[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		// outlinenum1
		strTemp.Format(_T("LineOutLineNum%d"), i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineOutLineNum[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// edgeproperty1
		strTemp.Format(_T("LineEdgeProperty%d"), i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeLineProperty[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// SortByScore1
		strTemp.Format(_T("LineSortByScore%d"), i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bLineSortByScore[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// DualEdge1
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bLineEnableDualEdge[i]);
		strTemp.Format(_T("LineDualEdge%d"),i);
		cXMLConfigurator.AddElem(strTemp, strInfo);
	}
//  //将TrainGuiParam的参数，放在TrainDataParam括号中 	
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam/////////////
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

	for (int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		// Caliper Num1
		strTemp.Format(_T("LineCaliperNum%d"), i);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_nLineCaliperNum[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// startX1
		strTemp.Format(_T("LineStartX%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartX[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// startY1
		strTemp.Format(_T("LineStartY%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartY[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// EndX1
		strTemp.Format(_T("LineEndX%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndX[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// EndY1
		strTemp.Format(_T("LineEndY%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndY[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// Project Len1
		strTemp.Format(_T("LineProjectLen%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperProjectLen[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// Search Len1
		strTemp.Format(_T("LineSearchLen%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperSearchLen[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
		// Search Rotation1
		strTemp.Format(_T("LineSearchRotation%d"), i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_LineCaliperSearchRotation[i].ToDouble());
		cXMLConfigurator.AddElem(strTemp, strInfo);

		//Caliper Visible1
		strTemp.Format(_T("LineCaliperVisible%d"),i);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("LineCaliperMaskEnable%d"), i);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineEnableLineMask[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("LineMaskParam%d"), i);
		cXMLConfigurator.DeleteElem(strTemp);
		cXMLConfigurator.AddElem(strTemp);
		cXMLConfigurator.EnterElem();

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCaliperMaskData[i].size());
		cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vdCaliperMaskData[i].size(); j++)
		{
			strTemp.Format(_T("MaskData%d"), j);
			strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCaliperMaskData[i].at(j));
			cXMLConfigurator.AddElem(strTemp, strInfo);
		}

		strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCaliperMask[i].size());
		cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

		for (int j=0; j<m_TrainGuiParam.m_vbCaliperMask[i].size(); j++)
		{
			strTemp.Format(_T("EnableMaskData%d"), j);
			strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCaliperMask[i].at(j));
			cXMLConfigurator.AddElem(strTemp, strInfo);
		}
		cXMLConfigurator.LeaveElem();
	}
	// Obround
	cXMLConfigurator.AddElem(_T("Obround"));
	cXMLConfigurator.EnterElem();
	for (int i=0;i<OBROUND_NUM;i++)
	{
		CString StrTemp=_T("");
		StrTemp.Format("CenterX%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround[i].GetCenter().GetX());
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		StrTemp.Format("CenterY%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround[i].GetCenter().GetY());
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		StrTemp.Format("XLength%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround[i].GetXLength());
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		StrTemp.Format("YLength%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround[i].GetYLength());
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		StrTemp.Format("XRotation%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_Obround[i].GetRotation().ToDouble());
		cXMLConfigurator.AddElem(StrTemp, strInfo);

		// Caliper Num 
		StrTemp.Format("CaliperNum%d",i);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum[i]);
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		// Project Len
		StrTemp.Format("ProjectLen%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen[i]);
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		// Search Len
		StrTemp.Format("SearchLen%d",i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen[i]);
		cXMLConfigurator.AddElem(StrTemp, strInfo);
		// Search Dircetion
		StrTemp.Format("SearchDir%d",i);
		strInfo.Format(_T("%d"), m_TrainGuiParam.m_CaliperSearchDirection[i]);
		cXMLConfigurator.AddElem(StrTemp, strInfo);
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
// 	// findcircle visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleVisible);
// 	cXMLConfigurator.AddElem(_T("CircleVisible"), strInfo);
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
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nObroundOutputResult);
	cXMLConfigurator.AddElem(_T("ObroundOutputResult"), strInfo);
	for (int i = 0;i  < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("LineFitPointVisible%d"), i);
		strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);
	}
//  // 将SearchGuiParam的参数，放在SearchDataParam括号中
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////SearchGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();


	// color
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
// 	// circle visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bCircleVisible);
// 	cXMLConfigurator.AddElem(_T("CircleVisible"), strInfo);
// 	// circle center visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bCircleCenterVisible);
// 	cXMLConfigurator.AddElem(_T("CircleCenterVisible"), strInfo);
	cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
	cXMLConfigurator.AddElem(_T("ModelX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
	cXMLConfigurator.AddElem(_T("ModelY"), strInfo);
	for (int j=0;j<OBROUND_NUM;j++)
	{
		CString strTemp=_T("");
		strTemp.Format(_T("CenterX%d"),j);
		strInfo.Format(_T("%f"), m_TrainResult.m_dModelX);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("CenterY%d"),j);
		strInfo.Format(_T("%f"), m_TrainResult.m_dModelY);
		cXMLConfigurator.AddElem(strTemp, strInfo);
	}

	for (int i = 0; i < LINE_NUM; i++)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("StartX%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dStartX[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("StartY%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dStartY[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("EndX%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dEndX[i]);
		cXMLConfigurator.AddElem(strTemp, strInfo);

		strTemp.Format(_T("EndY%d"), i);
		strInfo.Format(_T("%f"), m_TrainResult.m_dEndY[i]);
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
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	
	
// 更新训练控件数据信息	
bool CPatternCornerMulObroundTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		
		for (int i =0 ;i < LINE_NUM; i++)
		{
			m_tmpTrainGuiParam.m_nLineCaliperNum[i]		= m_GuiFindLine[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]	= m_GuiFindLine[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]		= m_GuiFindLine[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]	= m_GuiFindLine[i].GetCaliperSearchDirection();
			m_GuiFindLine[i].GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i],
				m_tmpTrainGuiParam.m_dLineEndX[i],m_tmpTrainGuiParam.m_dLineEndY[i]);

			m_GuiFindLine[i].GetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].GetCaliperMasked(m_tmpTrainGuiParam.m_vbCaliperMask[i]);
			m_tmpTrainGuiParam.m_bLineCaliperVisible[i]		= m_GuiFindLine[i].IsCaliperVisible();
		}
		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_tmpTrainGuiParam.m_nCaliperNum[i]		= m_GuiFindObround[i].GetCaliperNum();
			m_tmpTrainGuiParam.m_Obround[i]			= m_GuiFindObround[i].GetObround();
			m_tmpTrainGuiParam.m_dCaliperProjectLen[i] = m_GuiFindObround[i].GetCaliperProjectionLength();
			m_tmpTrainGuiParam.m_dCaliperSearchLen[i]	= m_GuiFindObround[i].GetCaliperSearchLength();
			m_tmpTrainGuiParam.m_CaliperSearchDirection[i] = (CaliperSearchDirection)m_GuiFindObround[i].GetCaliperSearchDirection();
		}
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
		
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

		for (int i = 0;i < LINE_NUM;i++)
		{
			m_GuiFindLine[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindLine[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindLine[i].SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum[i]);
			m_GuiFindLine[i].SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX[i], m_tmpTrainGuiParam.m_dLineStartY[i],
				m_tmpTrainGuiParam.m_dLineEndX[i], m_tmpTrainGuiParam.m_dLineEndY[i]);
			m_GuiFindLine[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dLineCaliperProjectLen[i]);
			m_GuiFindLine[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dLineCaliperSearchLen[i]);
			m_GuiFindLine[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_LineCaliperSearchRotation[i]);

			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bLineCaliperVisible[i]);
		}
		for (int i=0;i<OBROUND_NUM;i++)
		{
			m_GuiFindObround[i].SetLineColor(m_tmpTrainGuiParam.m_LineColor);
			m_GuiFindObround[i].SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
			m_GuiFindObround[i].SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum[i]);
			m_GuiFindObround[i].SetObround(m_tmpTrainGuiParam.m_Obround[i]);
			m_GuiFindObround[i].SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen[i]);
			m_GuiFindObround[i].SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen[i]);
			m_GuiFindObround[i].SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchDirection[i]);
		}
	}

	return true;
}

bool CPatternCornerMulObroundTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
		for (j=0; j<m_pGuiSearchObroundResultArray.size(); j++)
		{
			m_pGuiSearchObroundResultArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchObroundResultArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
		for (j=0; j <m_pGuiSearchFindLineArray.size(); j++)
		{
			m_pGuiSearchFindLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchFindLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}
	
	return true;
}

// 清除控件及其显示					
bool CPatternCornerMulObroundTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

bool CPatternCornerMulObroundTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CPatternCornerMulObroundTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchObroundArray);
	ClearGraphicsArray(m_pGuiSearchObroundCenterArray);
	ClearGraphicsArray(m_pGuiSearchObroundResultArray);


	//ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	ClearGraphicsArray(m_pGuiSearchFindLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCornerCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineCenterCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultLineFitPointArray1);
	ClearGraphicsArray(m_pGuiSearchResultLineFitPointArray2);
	return true;
}															

bool CPatternCornerMulObroundTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen,rot;
	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//长圆
	for (int i=0;i<OBROUND_NUM;i++)
	{
		m_GuiFindObround[i].GetCenterLengthsRotation(centerx,centery, xlen,ylen, rot);
		m_GuiFindObround[i].SetCenterLengthsRotation((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot);
		double plength,slength;
		plength = m_GuiFindObround[i].GetCaliperProjectionLength();
		slength = m_GuiFindObround[i].GetCaliperSearchLength();
		m_GuiFindObround[i].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
		m_GuiFindObround[i].SetCaliperSearchLength(slength/m_CamWidth*nWidth);
	}


	double sx,sy,ex,ey;
	double plength,slength;

	for (int i=0;i<LINE_NUM;i++)
	{
		m_GuiFindLine[i].GetStartXYEndXY(sx,sy,ex,ey);
		m_GuiFindLine[i].SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);
		plength = m_GuiFindLine[i].GetCaliperProjectionLength();
		slength = m_GuiFindLine[i].GetCaliperSearchLength();
		m_GuiFindLine[i].SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
		m_GuiFindLine[i].SetCaliperSearchLength(slength/m_CamWidth*nWidth);
	}

	//搜索区域
	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);


	//参考点位置
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;

	return true;
	return true;
}

bool CPatternCornerMulObroundTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		CString strTemp = _T("");
		for (int i=0;i<OBROUND_NUM;i++)
		{
			strTemp.Format(_T("_FindObround%d"),i);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+strTemp);

			strTemp.Format(_T("_TrainResultObround%d"),i);
			m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
			m_pGuiStaticContainer->RemoveItem(m_strKeyText +strTemp);
		}
		for (int i = 0; i < LINE_NUM; i++)
		{
			strTemp.Format(_T("_FindLine%d"),i);
			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText + strTemp);

			strTemp.Format(_T("_TrainResultLine%d"),i);
			m_pGuiStaticContainer->RemoveItem(m_strKeyText + strTemp);
		}

	}
	
	return true;	
}

bool CPatternCornerMulObroundTool::ClearResultGui()
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
		for (j=0; j<m_pGuiSearchObroundArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchObroundArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchObroundResultArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchObroundResultArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchObroundCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchObroundCenterArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchFindLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchFindLineArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchLineCornerCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCornerCrossArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchLineCenterCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCenterCrossArray[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchResultLineFitPointArray1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultLineFitPointArray1[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchResultLineFitPointArray2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultLineFitPointArray2[j]->GetTipText());
		}
	}
	
	return true;	
}

// 获取训练模板原始图像
bool CPatternCornerMulObroundTool::GetPatternImage(cpImage &ImagePattern)
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
bool CPatternCornerMulObroundTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternCornerMulObroundTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternCornerMulObroundTool::SetLanguage(int nLanguage)
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
		m_GuiFindLine[0].SetLabel(_T("FindLine1"));
		m_GuiFindLine[1].SetLabel(_T("FindLine2"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine[0].SetLabel(_T("找线工具1"));
		m_GuiFindLine[1].SetLabel(_T("找线工具2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiFindLine[0].SetLabel(_T("找线工具1"));
		m_GuiFindLine[1].SetLabel(_T("找线工具2"));
		break;
	}
	for (int i=0; i<OBROUND_NUM; i++)
	{
		CString str;
		switch (m_iLanguage)
		{
		case 0:
			str.Format(_T("FindObround%d"), i+1);
			break;
		case 1:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		default:
			str.Format(_T("找长圆工具%d"), i+1);
			break;
		}

		m_GuiFindObround[i].SetLabel(str);
	}
}
bool CPatternCornerMulObroundTool::CalcLineCross(scLineSeg cLineSeg1, scLineSeg cLineSeg2, sc2Vector& centerLine1, sc2Vector& centerLine2, std::vector<sc2Vector>& vtCrossArray)
{
	bool bIntersect;
	if (!sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bIntersect, vtCrossArray) || vtCrossArray.size() <= 0)
	{
		return false;
	}
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
				centerLine1 = (pointS+pointE)/2;

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
				centerLine2 = (pointS+pointE)/2;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}


CString CPatternCornerMulObroundTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:			//	区域定位失败			
				strErrInfo += _T("Pattern search failed");
				break;
			case -2:			//	找长圆1失败			
				strErrInfo += _T("Find Obround1 failed");
				break;
			case -3:			//	找长圆2失败			
				strErrInfo += _T("Find Obround2 failed");
				break;
			case -4:			//	找长圆1 2失败			
				strErrInfo += _T("Find Obround1 2 failed");
					break;
			case -5:			//	找线1失败			
				strErrInfo += _T("Find Line1 failed");
					break;
			case -6:			//	找线2失败			
				strErrInfo += _T("Find Line2 failed");
				break;
			case -7:			//	找线1，2失败		
				strErrInfo += _T("Find Line1 2 failed");
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
			case -1:							
				strErrInfo += _T("区域定位失败");
				break;
			case -2:							
				strErrInfo += _T("找长圆1失败");
				break;
			case -3:							
				strErrInfo += _T("找长圆2失败");
				break;
			case -4:							
				strErrInfo += _T("找长圆1、2失败");
				break;
			case -5:							
				strErrInfo += _T("找线1失败");
				break;
			case -6:							
				strErrInfo += _T("找线2失败");
				break;
			case -7:							
				strErrInfo += _T("找线1、2失败");
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
				strErrInfo += _T("找长圆1失败");
				break;
			case -3:							
				strErrInfo += _T("找长圆2失败");
				break;
			case -4:							
				strErrInfo += _T("找长圆1、2失败");
				break;
			case -5:							
				strErrInfo += _T("找线1失败");
				break;
			case -6:							
				strErrInfo += _T("找线2失败");
				break;
			case -7:							
				strErrInfo += _T("找线1、2失败");
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
