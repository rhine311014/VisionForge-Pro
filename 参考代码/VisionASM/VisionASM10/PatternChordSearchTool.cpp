#include "stdafx.h"
#include "PatternChordSearchTool.h"
#include "svGuiCircle.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"
#include "svIntersect.h"

#include <map>
#include "svMeasure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternChordSearchTool::CPatternChordSearchTool()
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


	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(350, 240, 100, 0, sk2PI);
	m_GuiFindCircle.SetCaliperParams(20, eInward, 50, 20, true);

	m_GuiFindLine.SetStartXYEndXY(150, 300, 150, 200);
	//m_GuiFindLine.SetLabel(_T("找线工具"));
	m_GuiFindLine.SetLabelVisible(TRUE);


	m_GuiFindCircle.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(FALSE);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}

	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_CoarseScale           = m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_Accuracy              = m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_dAngleLow             = -5.0;
	m_tmpTrainDataParam.m_dAngleHigh            = 5.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false;

	m_FindCircleTool.GetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, 
		m_tmpTrainDataParam.m_lFilterHalfWidth);

	m_FindLineTool.GetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre, m_tmpTrainDataParam.m_edgeLineProperty, 
		m_tmpTrainDataParam.m_lLineFilterHalfWidth);

	m_tmpTrainDataParam.m_nOutLineNum			= 10;


	m_tmpTrainGuiParam.m_TrainRect		        = m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	m_tmpTrainGuiParam.m_nCaliperNum			= m_GuiFindCircle.GetCaliperNum();
	m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius,
		m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
	m_tmpTrainGuiParam.m_dCaliperProjectLen		= m_GuiFindCircle.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dCaliperSearchLen		= m_GuiFindCircle.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
	m_tmpTrainGuiParam.m_bCircleCaliperVisible	= m_GuiFindCircle.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bCircleEnableMask		= false;

	m_tmpTrainGuiParam.m_nLineCaliperNum		= m_GuiFindLine.GetCaliperNum();
	m_tmpTrainGuiParam.m_dLineCaliperProjectLen		= m_GuiFindLine.GetCaliperProjectionLength();
	m_tmpTrainGuiParam.m_dLineCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
	m_tmpTrainGuiParam.m_LineCaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();
	m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX, m_tmpTrainGuiParam.m_dLineStartY, m_tmpTrainGuiParam.m_dLineEndX,
		m_tmpTrainGuiParam.m_dLineEndY);
	m_tmpTrainGuiParam.m_bLineCaliperVisible	= m_GuiFindLine.IsCaliperVisible();
	m_tmpTrainGuiParam.m_bCircleEnableMask		= false;

	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre       = 0.8;
	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CPatternChordSearchTool::CPatternChordSearchTool(CPatternChordSearchTool &cPatternChordSearchTool)
{
	if (cPatternChordSearchTool.GetSearchToolType() != ePatternChordSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(250,200,50,50);

	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);

	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, skPI);
	m_GuiFindCircle.SetLabelVisible(TRUE);


	m_GuiFindLine.SetStartXYEndXY(100, 300, 200, 200);
	m_GuiFindLine.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetLabelVisible(TRUE);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(false);
		m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);

	// 赋值
    m_CamHeight								= cPatternChordSearchTool.m_CamHeight;
    m_CamWidth								= cPatternChordSearchTool.m_CamWidth;
	m_TrainDataParam						= cPatternChordSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternChordSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternChordSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternChordSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternChordSearchTool.m_TrainResult;
	m_ImageTrain							= cPatternChordSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternChordSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternChordSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cPatternChordSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternChordSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternChordSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternChordSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternChordSearchTool.UpdateSearchGuiData(true);
	cPatternChordSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternChordSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternChordSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternChordSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternChordSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternChordSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternChordSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool =cPatternChordSearchTool.m_PatternSearchTool;
	}
}

CPatternChordSearchTool& CPatternChordSearchTool::operator=(CPatternChordSearchTool &cPatternChordSearchTool)
{
	if (this == &cPatternChordSearchTool)
	{
		return *this;
	}

	if (cPatternChordSearchTool.GetSearchToolType() != ePatternChordSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(250,200,50,50);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_GuiSearchRect.SetCenterWidthHeight(250, 200, 400, 300);
	m_GuiSearchRect.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetLabelVisible(TRUE);

	m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(250, 200, 100, 0, skPI);
	m_GuiFindCircle.SetLabelVisible(TRUE);

	m_GuiFindLine.SetStartXYEndXY(100, 300, 200, 200);
	m_GuiFindLine.SetLabelVisible(TRUE);

	m_GuiModelPoint.SetCenterXYWidthHeight(250, 200, 40, 40);

	//找线、找圆卡尺隐藏，矩形框可见
	m_GuiFindLine.SetCaliperVisible(FALSE);
	m_GuiFindLine.SetAffineRectFindLineVisible(TRUE);
	m_GuiFindCircle.SetCaliperVisible(false);
	m_GuiFindCircle.SetAnnulusFindCircleVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
	CreateGUID(m_strKeyText);

	// 赋值
    m_CamHeight								= cPatternChordSearchTool.m_CamHeight;
    m_CamWidth								= cPatternChordSearchTool.m_CamWidth;
	m_TrainDataParam						= cPatternChordSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= cPatternChordSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= cPatternChordSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= cPatternChordSearchTool.m_SearchGuiParam;
	m_TrainResult							= cPatternChordSearchTool.m_TrainResult;
	m_ImageTrain							= cPatternChordSearchTool.m_ImageTrain;
	m_ImageMask								= cPatternChordSearchTool.m_ImageMask;
	m_bTrainGuiShow							= cPatternChordSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow                   = cPatternChordSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cPatternChordSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cPatternChordSearchTool.m_bTrainOK;
	m_pGuiDisplay							= cPatternChordSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternChordSearchTool.UpdateSearchGuiData(true);
	cPatternChordSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternChordSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternChordSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternChordSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternChordSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternChordSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternChordSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool =cPatternChordSearchTool.m_PatternSearchTool;
	}

	return *this;
}

CPatternChordSearchTool::~CPatternChordSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CPatternChordSearchTool::GetSearchToolType()
{
	return ePatternChordSearchTool;
}										   

// 设置显示控件指针		
bool CPatternChordSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CPatternChordSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CPatternChordSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CPatternChordSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CPatternChordSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CPatternChordSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}											

// 更新搜索界面显示									
bool CPatternChordSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CPatternChordSearchTool::UpdateTrainGuiDisplay()
{
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiFindCircle.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCircleVisible);
	m_GuiFindLine.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);

	{
		CString strKey;
		strKey.Format(_T("%s_FindLineTool"), m_strKeyText);
		m_GuiFindLine.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bLineVisible);

		if (m_tmpTrainGuiParam.m_bLineEnableMask)
		{
			m_GuiFindLine.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition|scGuiFindLine::eGuiDOFMask);
		}
		else
		{
			m_GuiFindLine.EnableGuiDOF(scGuiFindLine::eGuiDOFStart|scGuiFindLine::eGuiDOFEnd|scGuiFindLine::eGuiDOFMarker|scGuiFindLine::eGuiDOFPosition);
		}
		if (m_tmpTrainGuiParam.m_bCircleEnableMask)
		{
			m_GuiFindCircle.EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
				scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition|scGuiFindCircle::eGuiDOFMask);
		}
		else
		{
			m_GuiFindCircle.EnableGuiDOF(scGuiFindCircle::eGuiDOFStartAngle|scGuiFindCircle::eGuiDOFStart|scGuiFindCircle::eGuiDOFEndAngle|
				scGuiFindCircle::eGuiDOFEnd|scGuiFindCircle::eGuiDOFBend|scGuiFindCircle::eGuiDOFMarker|scGuiFindCircle::eGuiDOFPosition);
		}
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindCircle, m_strKeyText+_T("_FindCircle"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiFindLine, m_strKeyText+_T("_FindLineTool"));	

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}	

// 更新训练结果Gui控件显示
bool CPatternChordSearchTool::UpdateTrainResultGuiDisplay()
{
	// 训练结果
	m_GuiTrainResultCircle.SetVisible(m_bTrainResultGuiShow);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiStaticContainer    = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->AddItem(&m_GuiTrainResultCircle, m_strKeyText+_T("_TrainResultCircle"));
		m_pGuiStaticContainer->AddItem(&(m_GuiSearchAffineRect[0]), m_strKeyText + _T("_TrainResultRect"));
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}		

// 更新搜索Gui控件显示														
bool CPatternChordSearchTool::UpdateSearchResultGuiDisplay()
{
	int i=0;

	for (i=0; i<m_pGuiSearchRectArray.size(); i++)
	{
		m_pGuiSearchRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bRectVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
	{
		m_pGuiSearchCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}

	for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
	{
		m_pGuiSearchCircleArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleVisible);
	}

	for (i=0; i<m_pGuiCircleSearchResultFitPointArray.size(); i++)
	{
		m_pGuiCircleSearchResultFitPointArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowCirclePoint);
	}

	for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
	{
		m_pGuiSearchCircleCenterArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCircleCenterVisible);
	}

	for (i=0; i<m_pGuiSearchLineArray.size(); i++)
	{
		m_pGuiSearchLineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
	{
		m_pGuiSearchResultFitPoint1Array[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bEnableShowLinePoint);
	}

	for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
	{
		m_pGuiSearchLineCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bLineVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary1.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary1[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}

	for (i=0; i<m_pGuiSearchCrossArrayAuxilary2.size(); i++)
	{
		m_pGuiSearchCrossArrayAuxilary2[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}


	for (i=0; i<m_pGuiSearchCircleCenterPointAndLinePointCrossArray.size(); i++)
	{
		m_pGuiSearchCircleCenterPointAndLinePointCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
	}

	for (i=0; i<m_pGuiSearchCircleCenterPointAndLinePointSegArray.size(); i++)
	{
		if (m_tmpSearchDataParam.m_bEnableLineCircleCross)
		{
			m_pGuiSearchCircleCenterPointAndLinePointSegArray[i]->SetVisible(false);
		}
		else
		{
			m_pGuiSearchCircleCenterPointAndLinePointSegArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bModelPointVisible);
		}
	}


	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		int i=0;

		for (i=0; i<m_pGuiSearchRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchRectArray[i], m_pGuiSearchRectArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCrossArray[i], m_pGuiSearchCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCircleArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleArray[i], m_pGuiSearchCircleArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiCircleSearchResultFitPointArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiCircleSearchResultFitPointArray[i], m_pGuiCircleSearchResultFitPointArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCircleCenterArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterArray[i], m_pGuiSearchCircleCenterArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchLineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineArray[i], m_pGuiSearchLineArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchLineCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchLineCrossArray[i], m_pGuiSearchLineCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchResultFitPoint1Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultFitPoint1Array[i], m_pGuiSearchResultFitPoint1Array[i]->GetTipText());
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
		for (i=0; i<m_pGuiSearchCircleCenterPointAndLinePointCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterPointAndLinePointCrossArray[i], m_pGuiSearchCircleCenterPointAndLinePointCrossArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCircleCenterPointAndLinePointSegArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCircleCenterPointAndLinePointSegArray[i], m_pGuiSearchCircleCenterPointAndLinePointSegArray[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CPatternChordSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	//	UpdateTrainResultGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CPatternChordSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CPatternChordSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CPatternChordSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	*((CPatternChordSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CPatternChordSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CPatternChordSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CPatternChordSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CPatternChordSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CPatternChordSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CPatternChordSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CPatternChordSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CPatternChordSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CPatternChordSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	//设置训练参数
	if(!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}
	if(!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
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
	bool bRet = false;
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
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
	}
	else
	{
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

	// 找圆
	m_FindCircleTool.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_tmpTrainGuiParam.m_nCaliperNum, m_tmpTrainGuiParam.m_CaliperSearchDirection, m_tmpTrainGuiParam.m_dCaliperSearchLen
		,m_tmpTrainGuiParam.m_dCaliperProjectLen);
	//m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_tmpTrainDataParam.m_nOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bSortByScore);
	m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius
		,m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
	m_FindCircleTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);
	if (m_tmpTrainDataParam.m_bCircleEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
	} 
	else
	{
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
	}
	scCircle cCircle;
	double   dRMS;
	if(bIsProcessImage)
	{
		bRet = m_FindCircleTool.Execute(tmpImage, cCircle, dRMS);
	}
	else
	{
		bRet = m_FindCircleTool.Execute(m_tmpImageTrain, cCircle, dRMS);
	}
	
	if (!bRet)
	{
		m_nResultErrNumber=-2;       //找圆失败
		return false;
	}
	m_tmpTrainResult.m_dCenterX = cCircle.GetCenter().GetX();
	m_tmpTrainResult.m_dCenterY = cCircle.GetCenter().GetY();
	m_tmpTrainResult.m_dRadius  = cCircle.GetRadius();



	// 	std::vector<sc2Vector> vPoints;
	// 	m_FindCircleTool.GetFitData(vPoints);
	// 	sc2Vector vPointNearCircleCenter;
	// 	bRet = GetPointNearestCircleCenter(vPointNearCircleCenter, cCircle, vPoints);
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	m_tmpTrainResult.m_pointPatternChord = vPointNearCircleCenter;



	// 找线
	m_FindLineTool.SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum);
	m_FindLineTool.SetCaliperParams(m_tmpTrainGuiParam.m_nLineCaliperNum, m_tmpTrainGuiParam.m_LineCaliperSearchRotation, m_tmpTrainGuiParam.m_dLineCaliperSearchLen
		,m_tmpTrainGuiParam.m_dLineCaliperProjectLen);
	m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre, m_tmpTrainDataParam.m_edgeLineProperty, m_tmpTrainDataParam.m_lLineFilterHalfWidth);
	m_FindLineTool.SetOutLineNum(m_tmpTrainDataParam.m_nLineOutLineNum, eRansacRobust);
	m_FindLineTool.EnableCaliperSortByScore(m_tmpTrainDataParam.m_bLineSortByScore);
	m_FindLineTool.SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX, m_tmpTrainGuiParam.m_dLineStartY, 
		m_tmpTrainGuiParam.m_dLineEndX, m_tmpTrainGuiParam.m_dLineEndY);
	m_FindLineTool.SetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask);
	if (m_tmpTrainDataParam.m_bLineEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeLineProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeLineProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre,
			m_tmpTrainDataParam.m_edgeLineProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lLineFilterHalfWidth);
	} 
	else
	{
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre,
			m_tmpTrainDataParam.m_edgeLineProperty, m_tmpTrainDataParam.m_lLineFilterHalfWidth);
	}

	scLineSeg cLineSeg;
	//double dRMS;
	if(bIsProcessImage)
	{
		bRet = m_FindLineTool.Execute(tmpImage, cLineSeg, dRMS);
	}
	else
	{
		bRet = m_FindLineTool.Execute(m_tmpImageTrain, cLineSeg, dRMS);
	}
	if (!bRet)
	{
		m_nResultErrNumber=-3;       //找线失败
		return false;
	}

	if (m_tmpTrainDataParam.m_nLineAngleType == 0)	// 任意角度
	{
		m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP1().GetX();
		m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP1().GetY();
		m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP2().GetX();
		m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP2().GetY();
	}
	else if (m_tmpTrainDataParam.m_nLineAngleType == 1)	//水平线
	{
		scDegree degLineAngle;
		degLineAngle = scDegree(cLineSeg.GetRotation()).SignedNorm();

		if (degLineAngle.ToDouble() >= -90 && degLineAngle.ToDouble() < 90)
		{
			m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP1().GetX();
			m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP1().GetY();
			m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP2().GetX();
			m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP2().GetY();
		}
		else
		{
			m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP2().GetX();
			m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP2().GetY();
			m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP1().GetX();
			m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP1().GetY();
		}
	}
	else if (m_tmpTrainDataParam.m_nLineAngleType == 2)	//垂直线
	{
		scDegree degLineAngle;
		degLineAngle = scDegree(cLineSeg.GetRotation()).Norm();

		if (degLineAngle.ToDouble() >= 0 && degLineAngle.ToDouble() < 180)
		{
			m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP1().GetX();
			m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP1().GetY();
			m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP2().GetX();
			m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP2().GetY();
		}
		else
		{
			m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP2().GetX();
			m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP2().GetY();
			m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP1().GetX();
			m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP1().GetY();
		}
	}
	else
	{
		m_tmpTrainResult.m_dLineStartX = cLineSeg.GetP1().GetX();
		m_tmpTrainResult.m_dLineStartY = cLineSeg.GetP1().GetY();
		m_tmpTrainResult.m_dLineEndX = cLineSeg.GetP2().GetX();
		m_tmpTrainResult.m_dLineEndY = cLineSeg.GetP2().GetY();
	}

	m_bTrainOK		 = true;
	m_ImageTrain     = m_tmpImageTrain;
	// 启用掩模时的操作
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		m_ImageMask      = m_tmpImageMask;
	}

	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam	 = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;
	m_GuiTrainResultCircle.SetCircle(cCircle);

	return true;
}

// 获取训练结果图像														
bool CPatternChordSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CPatternChordSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	*((CPatternChordSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CPatternChordSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CPatternChordSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternChordSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternChordSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	*((CPatternChordSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternChordSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CPatternChordSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CPatternChordSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternChordSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CPatternChordSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CPatternChordSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 设置搜索个数
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	if (!m_PatternSearchTool.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre))
	{
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	// 找圆
	m_FindCircleTool.SetCaliperNum(m_TrainGuiParam.m_nCaliperNum);
	m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
		,m_TrainGuiParam.m_dCaliperProjectLen);
	m_FindCircleTool.SetCaliperProperty(m_TrainDataParam.m_dContrastThre, m_TrainDataParam.m_edgeProperty, m_TrainDataParam.m_lFilterHalfWidth);
	m_FindCircleTool.SetOutLineNum(m_TrainDataParam.m_nOutLineNum);
	m_FindCircleTool.EnableCaliperSortByScore(m_TrainDataParam.m_bSortByScore);
	m_FindCircleTool.SetCaliperMasked(m_TrainGuiParam.m_vbCircleCaliperMask);
	if (m_tmpTrainDataParam.m_bCircleEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth);
	} 
	else
	{
		m_FindCircleTool.SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre, m_tmpTrainDataParam.m_edgeProperty, m_tmpTrainDataParam.m_lFilterHalfWidth);
	}

	// 找线
	m_FindLineTool.SetCaliperNum(m_TrainGuiParam.m_nLineCaliperNum);
	m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nLineCaliperNum, m_TrainGuiParam.m_LineCaliperSearchRotation, m_TrainGuiParam.m_dLineCaliperSearchLen
		,m_TrainGuiParam.m_dLineCaliperProjectLen);
	m_FindLineTool.SetCaliperProperty(m_TrainDataParam.m_dLineContrastThre, m_TrainDataParam.m_edgeLineProperty, m_TrainDataParam.m_lLineFilterHalfWidth);
	m_FindLineTool.SetOutLineNum(m_TrainDataParam.m_nLineOutLineNum, eRansacRobust);
	m_FindLineTool.EnableCaliperSortByScore(m_TrainDataParam.m_bLineSortByScore);
	m_FindLineTool.SetCaliperMasked(m_TrainGuiParam.m_vbLineCaliperMask);
	if (m_tmpTrainDataParam.m_bLineEnableDualEdge)
	{
		EdgePolarity tmpEdgePolarity = eDontCare;
		if (m_tmpTrainDataParam.m_edgeLineProperty == eDarkToLight)
		{
			tmpEdgePolarity = eLightToDark;
		} 
		else if (m_tmpTrainDataParam.m_edgeLineProperty == eLightToDark)
		{
			tmpEdgePolarity = eDarkToLight;
		}
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre,
			m_tmpTrainDataParam.m_edgeLineProperty,tmpEdgePolarity, m_tmpTrainDataParam.m_lLineFilterHalfWidth);
	} 
	else
	{
		m_FindLineTool.SetCaliperProperty(m_tmpTrainDataParam.m_dLineContrastThre,
			m_tmpTrainDataParam.m_edgeLineProperty, m_tmpTrainDataParam.m_lLineFilterHalfWidth);
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
			sc2Vector tmModelPoint = sc2Vector(0,0);
			double dRotation = 0;
			double dTransX = 0;
			double dTransY = 0;
			m_PatternSearchTool.GetPatternOrigin(tmModelPoint);
			dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

			// 求找圆工具变换后的位置
			double dTransCenterX, dTransCenterY;
			double dOriCenterX, dOriCenterY;
			dOriCenterX = m_TrainGuiParam.m_CircleCenter.GetX();
			dOriCenterY = m_TrainGuiParam.m_CircleCenter.GetY();
			dTransCenterX = cos(dRotation) * dOriCenterX  - sin(dRotation) * dOriCenterY + dTransX;
			dTransCenterY = sin(dRotation) * dOriCenterX  + cos(dRotation) * dOriCenterY + dTransY;
			m_FindCircleTool.SetCaliperParams(m_TrainGuiParam.m_nCaliperNum, m_TrainGuiParam.m_CaliperSearchDirection, m_TrainGuiParam.m_dCaliperSearchLen
				,m_TrainGuiParam.m_dCaliperProjectLen);
			m_FindCircleTool.SetCenterRadiusAngleStartAngleSpan(sc2Vector(dTransCenterX, dTransCenterY), m_TrainGuiParam.m_dRadius
				,scRadian(dRotation)+m_TrainGuiParam.m_ArcStart, m_TrainGuiParam.m_ArcSpan);

			// 执行找圆
			scCircle ResCircle;
			double dRMS;
			if(bIsProcessImage)
			{
				bRet = m_FindCircleTool.Execute(tmpImage, ResCircle, dRMS);
			}
			else
			{
				bRet = m_FindCircleTool.Execute(ImageSearch, ResCircle, dRMS);
			}
			if (bRet) // 成功
			{
				// 				std::vector<sc2Vector> vPoints;
				// 				sc2Vector vNearestPoint;
				// 计算矫正角度之后的角度和偏移距离
				if (m_TrainGuiParam.m_bCorrectAngle)
				{
					// 					m_FindCircleTool.GetFitData(vPoints);					
					// 					if (GetPointNearestCircleCenter(vNearestPoint, ResCircle, vPoints))
					// 					{
					// 						scLineSeg newLineSeg(ResCircle.GetCenter(), vNearestPoint);
					// 						scLineSeg trainLineSeg(sc2Vector(m_TrainResult.m_dCenterX, m_TrainResult.m_dCenterY), m_TrainResult.m_pointPatternChord);
					// 						scRadian lineAngle;
					// 						sfAngleLine2Line(trainLineSeg.GetLine(), newLineSeg.GetLine(), lineAngle);
					// 						dRotation = lineAngle.Norm().ToDouble();
					// 						dTransX = vNearestPoint.GetX() - (cos(dRotation) * m_TrainResult.m_pointPatternChord.GetX()  - sin(dRotation) * m_TrainResult.m_pointPatternChord.GetY());
					// 						dTransY = vNearestPoint.GetY() - (sin(dRotation) * m_TrainResult.m_pointPatternChord.GetX()  + cos(dRotation) * m_TrainResult.m_pointPatternChord.GetY());
					// 					}

					// 根据找圆结果修正找线工具的X和Y
					dTransX = ResCircle.GetCenter().GetX() - (cos(dRotation) * m_TrainResult.m_dCenterX  - sin(dRotation) * m_TrainResult.m_dCenterY);
					dTransY = ResCircle.GetCenter().GetY() - (sin(dRotation) * m_TrainResult.m_dCenterX  + cos(dRotation) * m_TrainResult.m_dCenterY);

				}

				// 求找线工具变换后的位置,找线
				double dTransStartX, dTransStartY;
				double dTransEndX, dTransEndY;
				dTransStartX = cos(dRotation) * m_TrainGuiParam.m_dLineStartX  - sin(dRotation) * m_TrainGuiParam.m_dLineStartY + dTransX;
				dTransStartY = sin(dRotation) * m_TrainGuiParam.m_dLineStartX  + cos(dRotation) * m_TrainGuiParam.m_dLineStartY + dTransY;
				dTransEndX = cos(dRotation) * m_TrainGuiParam.m_dLineEndX  - sin(dRotation) * m_TrainGuiParam.m_dLineEndY + dTransX;
				dTransEndY = sin(dRotation) * m_TrainGuiParam.m_dLineEndX  + cos(dRotation) * m_TrainGuiParam.m_dLineEndY + dTransY;

				m_FindLineTool.SetCaliperParams(m_TrainGuiParam.m_nLineCaliperNum, m_TrainGuiParam.m_LineCaliperSearchRotation, m_TrainGuiParam.m_dLineCaliperSearchLen
					,m_TrainGuiParam.m_dLineCaliperProjectLen);
				m_FindLineTool.SetStartXYEndXY(dTransStartX, dTransStartY, dTransEndX, dTransEndY);
				m_FindLineTool.SetCaliperMasked(m_TrainGuiParam.m_vbLineCaliperMask);

				// 执行找线
				scLineSeg ResLineSeg;
				if(bIsProcessImage)
				{
					bRet = m_FindLineTool.Execute(tmpImage, ResLineSeg, dRMS);
				}
				else
				{
					bRet = m_FindLineTool.Execute(ImageSearch, ResLineSeg, dRMS);
				}
				if (bRet) // 成功
				{
				
										// 直线拟合系数判断
					if (m_tmpTrainDataParam.m_bEnableFitLineDMS)
					{
						if ( dRMS > m_tmpTrainDataParam.m_dFitLineDMS)
						{
							m_nResultErrNumber = -6;//线拟合残差超上限
							return false;
						}
					}
					
					// 圆心到直线的最短距离测量
					sc2Vector nearestPoint;
					sc2Vector circleCenter = ResCircle.GetCenter();
					double dDistance = 0.0f;
					sfDistancePoint2Line(circleCenter, ResLineSeg.GetLine(), dDistance, nearestPoint);

					CSearchResult searchResult;
					
					searchResult.m_dPosX = nearestPoint.GetX();
					searchResult.m_dPosY = nearestPoint.GetY();
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
					// 圆心到直线的最短距离测量
					sc2Vector  vLineCirleCross;
					dDistance = 0.0;
					double dXDisOffset = 0.0, dYDisOffset = 0.0;
					sfDistancePoint2Line(circleCenter, ResLineSeg.GetLine(), dDistance, nearestPoint);

					if (m_tmpSearchDataParam.m_bEnableLineCircleCross)
					{
						double dResultAngle = searchResult.m_dAngle;
						//计算圆心到直线X和Y方向的偏移量
						double dOffsetRatio = m_tmpSearchDataParam.m_dOffsetRatio;

						dXDisOffset = (circleCenter.GetX() - nearestPoint.GetX())*dOffsetRatio;
						dYDisOffset = (circleCenter.GetY() - nearestPoint.GetY())*dOffsetRatio;
						sc2Vector vP1,vP2;
						vP1.SetX(ResLineSeg.GetP1().GetX() + dXDisOffset);
						vP1.SetY(ResLineSeg.GetP1().GetY() + dYDisOffset);
						vP2.SetX(ResLineSeg.GetP2().GetX() + dXDisOffset);
						vP2.SetY(ResLineSeg.GetP2().GetY() + dYDisOffset);
						ResLineSeg.SetP1(vP1);
						ResLineSeg.SetP2(vP2);

						//返回平移之后线段的所在的直线
						scLine ResLine; 
						ResLine = ResLineSeg.GetLine();

						//增加判断输出的规则检查
						//double Radius2 = ResCircle.GetRadius()*sqrt(2.0)/2;

						//求直线和圆的交点
						bool bIntersect = false;
						std::vector<sc2Vector> vCrossPointArray; 
						if (sfInterSectLineCircle(ResLine,ResCircle,bIntersect,vCrossPointArray) && vCrossPointArray.size()>1)
						{
							switch (m_tmpSearchDataParam.m_nLineCircleCrossMode)
							{
							case 0:
								if (vCrossPointArray[0].GetX() < vCrossPointArray[1].GetX())
								{
									vLineCirleCross = vCrossPointArray[0];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								else
								{
									vLineCirleCross = vCrossPointArray[1];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								break;
							case 1:
								if (vCrossPointArray[0].GetX() < vCrossPointArray[1].GetX())
								{
									vLineCirleCross = vCrossPointArray[1];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								else
								{
									vLineCirleCross = vCrossPointArray[0];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								break;
							case 2:
								if (vCrossPointArray[0].GetY() < vCrossPointArray[1].GetY())
								{
									vLineCirleCross = vCrossPointArray[0];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								else
								{
									vLineCirleCross = vCrossPointArray[1];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								break;
							case 3:
								if (vCrossPointArray[0].GetY() < vCrossPointArray[1].GetY())
								{
									vLineCirleCross = vCrossPointArray[1];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								else
								{
									vLineCirleCross = vCrossPointArray[0];

									searchResult.m_vdAuxiliaryPosX[0] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[0] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[0] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[1] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[1] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[1] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[2] = vCrossPointArray[0].GetX();
									searchResult.m_vdAuxiliaryPosY[2] = vCrossPointArray[0].GetY();
									searchResult.m_vdAuxiliaryAngle[2] = dResultAngle;

									searchResult.m_vdAuxiliaryPosX[3] = vCrossPointArray[1].GetX();
									searchResult.m_vdAuxiliaryPosY[3] = vCrossPointArray[1].GetY();
									searchResult.m_vdAuxiliaryAngle[3] = dResultAngle;
								}
								break;
							}
						}else
						{
							m_nResultErrNumber = -4;
							return false;
						}
						//获取直线的的位置

						searchResult.m_dPosX = vLineCirleCross.GetX();
						searchResult.m_dPosY = vLineCirleCross.GetY();
					}
					else
					{
						searchResult.m_dPosX = nearestPoint.GetX();
						searchResult.m_dPosY = nearestPoint.GetY();
					}
					//获取搜索圆定位工具的圆心和半径
					searchResult.m_resultCircleSearchTool.m_scCircleCircle  = ResCircle.GetCenter();
					searchResult.m_resultCircleSearchTool.m_dRadius         = ResCircle.GetRadius();

					searchResult.m_dScore = tmpPatternResult.GetScore();
					// 存储搜索结果

					// 设置粗定位矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpPatternResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					// 圆
					CResultCircleSearchTool circle;
					circle.m_scCircleCircle = ResCircle.GetCenter();
					circle.m_dRadius = ResCircle.GetRadius();
					searchResult.m_vCircleSearchResult.push_back(circle);
					// 线
					CResultLine line;
					line.m_vLineX[0] = ResLineSeg.GetP1().GetX();
					line.m_vLineY[0] = ResLineSeg.GetP1().GetY();
					line.m_vLineX[1] = ResLineSeg.GetP2().GetX();
					line.m_vLineY[1] = ResLineSeg.GetP2().GetY();
					searchResult.m_vLineSearchResult.push_back(line);
					// 线段
					line.m_vLineX[0] = searchResult.m_dPosX;
					line.m_vLineY[0] = searchResult.m_dPosY;
					line.m_vLineX[1] = ResCircle.GetCenter().GetX();
					line.m_vLineY[1] = ResCircle.GetCenter().GetY();
					searchResult.m_vLineSegSearchResult.push_back(line);

					m_SearchResultArray.push_back(searchResult);


					// 几何定位工具
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
					pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					m_pGuiSearchCrossArray.push_back(pTempCross);

					// 找圆工具
					strTmpKey.Format(_T("%s_ResultCircle%d"), m_strKeyText, i);
					scGuiCircle *pTemCircle = m_GuiSearchCircle+i;
					pTemCircle->SetCircle(ResCircle);
					pTemCircle->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemCircle->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemCircle->SetTipTextEnabled(true);
					pTemCircle->SetTipText(strTmpKey);
					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f;R:%.1f"), i, ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY(),ResCircle.GetRadius());
					pTemCircle->SetLabel(st);
					pTemCircle->SetLabelVisible(TRUE);
					m_pGuiSearchCircleArray.push_back(pTemCircle);

					strTmpKey.Format(_T("%s_ResultCircleCenter%d"), m_strKeyText, i);
					scGuiCross *pTempCircleCenter = m_GuiSearchCircleCenter+i;
					pTempCircleCenter->SetCenterExtent(ResCircle.GetCenter(), sc2Vector(40,40));
					pTempCircleCenter->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCircleCenter->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCircleCenter->SetTipTextEnabled(true);
					pTempCircleCenter->SetTipText(strTmpKey);
					// 					CString st;
					// 					st.Format(_T("%d X:%.1f;Y:%.1f"), i, ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY());
					// 					pTempCircleCenter->SetLabel(st);
					// 					pTempCircleCenter->SetLabelVisible(TRUE);
					m_pGuiSearchCircleCenterArray.push_back(pTempCircleCenter);

					// 找线工具
					strTmpKey.Format(_T("%s_ResultLine%d"), m_strKeyText, i);
					scGuiLine *pTemLine = m_GuiSearchLine+i;
					pTemLine->SetXYRotation(ResLineSeg.GetLine().GetPos(), scDegree(searchResult.m_dAngle));
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f,RMS:%.3f"), i, searchResult.m_dPosX, searchResult.m_dPosY, searchResult.m_dAngle,dRMS);
					pTemLine->SetLabel(st);
					pTemLine->SetLabelVisible(TRUE);
					m_pGuiSearchLineArray.push_back(pTemLine);

					// 					strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
					// 					pTempCross = m_GuiSearchLineCross+i;
					// 					sc2Vector vPoint1 = ResLineSeg.GetP1();
					// 					sc2Vector vPoint2 = ResLineSeg.GetP2();
					// 					sc2Vector vMidPoint((vPoint1.GetX() + vPoint2.GetX())/2.0, (vPoint1.GetY() + vPoint2.GetY()) / 2.0);
					// 					pTempCross->SetCenterExtent(nearestPoint, sc2Vector(60,60));
					// 					pTempCross->SetLineColor(RGB(255, 0, 0));
					// 					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					// 					pTempCross->SetTipTextEnabled(true);
					// 					pTempCross->SetTipText(strTmpKey);
					// 					m_pGuiSearchLineCrossArray.push_back(pTempCross);
					// 
					// 					strTmpKey.Format(_T("%s_ResultCrossAuxiliary1%d"), m_strKeyText, i);
					// 					pTempCross = m_GuiSearchCrossAuxilary1+i;
					// 					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(0),searchResult.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
					// 					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					// 					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					// 					pTempCross->SetTipTextEnabled(true);
					// 					pTempCross->SetTipText(strTmpKey);
					// 					m_pGuiSearchCrossArrayAuxilary1.push_back(pTempCross);
					// 
					// 					strTmpKey.Format(_T("%s_ResultCrossAuxiliary2%d"), m_strKeyText, i);
					// 					pTempCross = m_GuiSearchCrossAuxilary2+i;
					// 					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_vdAuxiliaryPosX.at(1),searchResult.m_vdAuxiliaryPosY.at(1)), sc2Vector(30,30));
					// 					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					// 					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					// 					pTempCross->SetTipTextEnabled(true);
					// 					pTempCross->SetTipText(strTmpKey);
					// 					m_pGuiSearchCrossArrayAuxilary2.push_back(pTempCross);

					// 圆心到直线垂线最短距离交点
					strTmpKey.Format(_T("%s_ResultCircleCenterPointAndLinePointCross%d"), m_strKeyText, i);
					pTempCross = m_GuiSearchCircleCenterPointAndLinePointCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), sc2Vector(30,30));
					pTempCross->SetLineColor(RGB(255,0,0)/*m_tmpSearchGuiParam.m_LineColor*/);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					m_pGuiSearchCircleCenterPointAndLinePointCrossArray.push_back(pTempCross);

					// 圆心、直线垂线最短距离交点构成的线段
					strTmpKey.Format(_T("%s_ResultCircleCenterPointAndLinePointSeg%d"), m_strKeyText, i);
					scGuiLineSeg *pTemLineSeg = m_GuiSearchCircleCenterPointAndLinePointSeg+i;
					pTemLineSeg->SetStartXYEndXY(sc2Vector(searchResult.m_dPosX, searchResult.m_dPosY), ResCircle.GetCenter());
					pTemLineSeg->SetLineColor(RGB(255,0,0)/*m_tmpSearchGuiParam.m_LineColor*/);
					pTemLineSeg->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLineSeg->SetTipTextEnabled(true);
					pTemLineSeg->SetTipText(strTmpKey);
					m_pGuiSearchCircleCenterPointAndLinePointSegArray.push_back(pTemLineSeg);


					//////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////
					// 找圆工具拟合点
					std::vector<bool> vbCirclePointUsed;
					std::vector<sc2Vector> vCirclePoints;
					m_FindCircleTool.GetUsed(vbCirclePointUsed);
					m_FindCircleTool.GetFitData(vCirclePoints);

					for (int nIndex = 0; nIndex < vbCirclePointUsed.size(); nIndex++)
					{
						bool bUsed = vbCirclePointUsed[nIndex];
						sc2Vector pointFit = vCirclePoints[nIndex];
						strTmpKey.Format(_T("%s_CircleFitPoint1_%d"), m_strKeyText, nIndex);
						scGuiCoordCross *pTempFitPoint1 = m_GuiCircleSearchResultFitPoint + nIndex;
						pTempFitPoint1->SetXDirectionAdornment(scGuiCoordCross::eNone);
						pTempFitPoint1->SetYDirectionAdornment(scGuiCoordCross::eNone);
						pTempFitPoint1->SetRulerVisible(false);
						pTempFitPoint1->SetCenterRotationLengths(pointFit, scRadian(scDegree(45)),13,13);
						if (bUsed)
						{
							pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorOKFitPoint1);
						}
						else
						{
							pTempFitPoint1->SetLineColor(m_SearchGuiParam.m_cLineColorNGFitPoint1);
						}
						pTempFitPoint1->SetTipTextEnabled(true);
						pTempFitPoint1->SetTipText(strTmpKey);
						m_pGuiCircleSearchResultFitPointArray.push_back(pTempFitPoint1);	
					}


					//////////////////////////////////////////////////////////////////////////
					//////////////////////////////////////////////////////////////////////////

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
					//////////////////////////////////////////////////////////////////////////
					// 找线工具拟合点
					std::vector<bool> bUsed;
					std::vector<sc2Vector> points;
					m_FindLineTool.GetUsed(bUsed);
					m_FindLineTool.GetFitData(points);
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
					m_nResultErrNumber=-3;       // 找线失败
				}
			}
			else
			{
				m_nResultErrNumber=-2;       // 找圆失败
				return false;
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
		m_nResultErrNumber=-1;       //区域定位失败
		return false;
	}

	return true;
}

// 获取搜索结果个数										
int CPatternChordSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternChordSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternChordSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternChordSearchTool)
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
	rRect.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
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
bool CPatternChordSearchTool::LoadModelFromFile(CString strDir)
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
	CPatternChordSearchTrainDataParam tmpTrainDataParam;
	CPatternChordSearchTrainGuiParam tmpTrainGuiParam;
	CPatternChordSearchSearchDataParam tmpSearchDataParam;
	CPatternChordSearchSearchGuiParam tmpSearchGuiParam;
	CPatternChordSearchTrainResult  tmpTrainResult;
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
	if (cSearchToolType != ePatternChordSearchTool)
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

	bRet = cXMLConfigurator.FindElem(_T("CoarseScale"));
	if(!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale = (CoarseScale)_ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("Accuracy"));
	if(!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy = (Accuracy)_ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow"));
	if(!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow = _tcstod(cXMLConfigurator.GetElemData(),NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh"));
	if(!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh = _tcstod(cXMLConfigurator.GetElemData(),NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable = _ttoi(cXMLConfigurator.GetElemData());
	// contrast thre
	bRet = cXMLConfigurator.FindElem(_T("ContrastThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dContrastThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// FilterHalfWidth
	bRet = cXMLConfigurator.FindElem(_T("FilterHalfWidth"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lFilterHalfWidth = 1;
	}
	else
	{
		tmpTrainDataParam.m_lFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	}
	// outlinenum
	bRet = cXMLConfigurator.FindElem(_T("OutLineNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nOutLineNum = _ttol(cXMLConfigurator.GetElemData());
	// edgeproperty
	bRet = cXMLConfigurator.FindElem(_T("EdgeProperty"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeProperty = (EdgePolarity)_ttol(cXMLConfigurator.GetElemData());
	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("CircleDualEdge"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bCircleEnableDualEdge = false;
	}
	else
	{
		tmpTrainDataParam.m_bCircleEnableDualEdge = _ttoi(cXMLConfigurator.GetElemData());
	}

	// SortByScore
	bRet = cXMLConfigurator.FindElem(_T("SortByScore"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bSortByScore = true;
	}
	tmpTrainDataParam.m_bSortByScore = _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("LineContrastThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dLineContrastThre = _ttol(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("LineFilterHalfWidth"));
	if (!bRet)
	{
		tmpTrainDataParam.m_lLineFilterHalfWidth = 1;
	}
	else
	{
		tmpTrainDataParam.m_lLineFilterHalfWidth = _ttol(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("LineOutLineNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLineOutLineNum = _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("LineEdgeProperty"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_edgeLineProperty = (EdgePolarity)_ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("LineSortByScore"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bLineSortByScore = _ttoi(cXMLConfigurator.GetElemData());

	// DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("LineDualEdge"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bLineEnableDualEdge = false;
	}
	else
	{
		tmpTrainDataParam.m_bLineEnableDualEdge = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableFitLineDMS"));
	if (bRet)
	{
		tmpTrainDataParam.m_bEnableFitLineDMS = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("FitLineDMS"));
	if (bRet)
	{
		tmpTrainDataParam.m_dFitLineDMS = _ttof(cXMLConfigurator.GetElemData());
	}




	bRet = cXMLConfigurator.FindElem(_T("LineAngleType"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLineAngleType = _ttoi(cXMLConfigurator.GetElemData());

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
	bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
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
	tmpTrainGuiParam.m_TrainRect.SetOrigion(vPos);
	bRet = cXMLConfigurator.FindElem(_T("XLength"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(),NULL));
	bRet = cXMLConfigurator.FindElem(_T("YLength"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(),NULL));
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
	// circle center
	bRet = cXMLConfigurator.FindElem(_T("CircleCenter"));
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
	tmpTrainGuiParam.m_CircleCenter = vPos;
	cXMLConfigurator.LeaveElem();
	// circle radius  
	bRet = cXMLConfigurator.FindElem(_T("CircleRadius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Arc start   
	bRet = cXMLConfigurator.FindElem(_T("ArcStart"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_ArcStart = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	// Arc span   
	bRet = cXMLConfigurator.FindElem(_T("ArcSpan"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_ArcSpan = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	// Caliper Num 
	bRet = cXMLConfigurator.FindElem(_T("CaliperNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("ProjectLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("SearchLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Dirction
	bRet = cXMLConfigurator.FindElem(_T("SearchDir"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)_ttoi(cXMLConfigurator.GetElemData());
	// CircleCaliper Visible
	bRet = cXMLConfigurator.FindElem(_T("CircleCaliperVisible"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bCircleCaliperVisible=true;
	}
	else
	{
		tmpTrainGuiParam.m_bCircleCaliperVisible = _ttoi(cXMLConfigurator.GetElemData());
	}

	// CircleCaliperMaskEnable
	bRet = cXMLConfigurator.FindElem(_T("CircleCaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bCircleEnableMask = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bCircleEnableMask = false;
	}
	bRet = cXMLConfigurator.FindElem(_T("CircleMaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdCircleCaliperMaskData.resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdCircleCaliperMaskData.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}

			tmpTrainGuiParam.m_vdCircleCaliperMaskData.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbCircleCaliperMask.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbCircleCaliperMask.size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbCircleCaliperMask.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}


	// CaliperNum
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_nLineCaliperNum = _ttoi(cXMLConfigurator.GetElemData());
	// startX
	bRet = cXMLConfigurator.FindElem(_T("LineStartX"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineStartX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// startY
	bRet = cXMLConfigurator.FindElem(_T("LineStartY"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineStartY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndX
	bRet = cXMLConfigurator.FindElem(_T("LineEndX"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineEndX = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// EndY
	bRet = cXMLConfigurator.FindElem(_T("LineEndY"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineEndY = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Project Len
	bRet = cXMLConfigurator.FindElem(_T("LineProjectLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineCaliperProjectLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Len
	bRet = cXMLConfigurator.FindElem(_T("LineSearchLen"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_dLineCaliperSearchLen = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// Search Rotation
	bRet = cXMLConfigurator.FindElem(_T("LineSearchRotation"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_LineCaliperSearchRotation = scRadian(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	
	// LineCaliper Visible
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperVisible"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bLineCaliperVisible=true;
	}
	else
	{
		tmpTrainGuiParam.m_bLineCaliperVisible = _ttoi(cXMLConfigurator.GetElemData());
	}

	// LineMaskEnable
	bRet = cXMLConfigurator.FindElem(_T("LineCaliperMaskEnable"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bLineEnableMask = _ttol(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpTrainGuiParam.m_bLineEnableMask = false;
	}
	bRet = cXMLConfigurator.FindElem(_T("LineMaskParam"));
	if (bRet)
	{
		CString strTmp;
		cXMLConfigurator.EnterElem();

		bRet = cXMLConfigurator.FindElem(_T("MaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vdLineCaliperMaskData.resize(_ttoi(cXMLConfigurator.GetElemData()));

		for (int j=0; j<tmpTrainGuiParam.m_vdLineCaliperMaskData.size(); j++)
		{
			strTmp.Format(_T("MaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}
			tmpTrainGuiParam.m_vdLineCaliperMaskData.at(j) = _tcstod(cXMLConfigurator.GetElemData(), NULL);
		}

		bRet = cXMLConfigurator.FindElem(_T("EnableMaskDataNum"));
		if (!bRet) return false;
		tmpTrainGuiParam.m_vbLineCaliperMask.resize(_ttoi(cXMLConfigurator.GetElemData()));
		for (int j=0; j<tmpTrainGuiParam.m_vbLineCaliperMask.size(); j++)
		{
			strTmp.Format(_T("EnableMaskData%d"), j);
			bRet = cXMLConfigurator.FindElem(strTmp);
			if (!bRet)
			{
				return false;
			}			
			tmpTrainGuiParam.m_vbLineCaliperMask.at(j) = _ttoi(cXMLConfigurator.GetElemData());
		}
		cXMLConfigurator.LeaveElem();
	}

	// Correct Angle
	bRet = cXMLConfigurator.FindElem(_T("CorrectAngle"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainGuiParam.m_bCorrectAngle = _ttoi(cXMLConfigurator.GetElemData());

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

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
	if(!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dConfusionThre = _tcstod(cXMLConfigurator.GetElemData(),NULL);

	bRet = cXMLConfigurator.FindElem(_T("LineFitPointVisible"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowLinePoint = _ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("CircleFitPointVisible"));
	if (!bRet)
	{
		tmpSearchGuiParam.m_bEnableShowCirclePoint = false;
	}
	else
	{
		tmpSearchGuiParam.m_bEnableShowCirclePoint = _ttoi(cXMLConfigurator.GetElemData());
	}
	
	bRet = cXMLConfigurator.FindElem(_T("EnableLineCircleCross"));
	if(!bRet)
	{
		tmpSearchDataParam.m_bEnableLineCircleCross = false;
	}
	tmpSearchDataParam.m_bEnableLineCircleCross = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("LineCircleCrossMode"));
	if(!bRet)
	{
		tmpSearchDataParam.m_nLineCircleCrossMode = 0;
	}
	tmpSearchDataParam.m_nLineCircleCrossMode = _ttoi(cXMLConfigurator.GetElemData());
	
	bRet = cXMLConfigurator.FindElem(_T("OffsetRatio"));
	if (bRet)
	{
		tmpSearchDataParam.m_dOffsetRatio = _ttof(cXMLConfigurator.GetElemData());
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
	tmpTrainResult.m_dCenterX = vPos.GetX();
	tmpTrainResult.m_dCenterY = vPos.GetY();
	bRet = cXMLConfigurator.FindElem(_T("Radius"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainResult.m_dRadius = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	bRet = cXMLConfigurator.FindElem(_T("PatternChordPoint"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("PatternChordPointX"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	bRet = cXMLConfigurator.FindElem(_T("PatternChordPointY"));
	if (!bRet)
	{
		return false;
	}
	vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));

	tmpTrainResult.m_pointPatternChord = vPos;
	cXMLConfigurator.LeaveElem();

	//cXMLConfigurator.LeaveElem();




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
	scPatternSearchRot tmpPatternSearchTool;
	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
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
bool CPatternChordSearchTool::SaveModelToFile(CString strDir)
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
	strInfo.Format(_T("%d"),m_TrainDataParam.m_CoarseScale);
	cXMLConfigurator.AddElem(_T("CoarseScale"),strInfo);
	strInfo.Format(_T("%d"),m_TrainDataParam.m_Accuracy);
	cXMLConfigurator.AddElem(_T("Accuracy"),strInfo);
	strInfo.Format(_T("%f"),m_TrainDataParam.m_dAngleLow);
	cXMLConfigurator.AddElem(_T("AngleLow"),strInfo);
	strInfo.Format(_T("%f"),m_TrainDataParam.m_dAngleHigh);
	cXMLConfigurator.AddElem(_T("AngleHigh"),strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable);
	cXMLConfigurator.AddElem(_T("MaskEnable"), strInfo);
	// contrastthre
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dContrastThre);
	cXMLConfigurator.AddElem(_T("ContrastThre"), strInfo);
	// fileterhalfwidth
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lFilterHalfWidth);
	cXMLConfigurator.AddElem(_T("FilterHalfWidth"), strInfo);
	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nOutLineNum);
	cXMLConfigurator.AddElem(_T("OutLineNum"), strInfo);
	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeProperty);
	cXMLConfigurator.AddElem(_T("EdgeProperty"), strInfo);
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bSortByScore);
	cXMLConfigurator.AddElem(_T("SortByScore"), strInfo);
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bCircleEnableDualEdge);
	cXMLConfigurator.AddElem(_T("CircleDualEdge"), strInfo);

	// contrastthre
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dLineContrastThre);
	cXMLConfigurator.AddElem(_T("LineContrastThre"), strInfo);
	// fileterhalfwidth
	strInfo.Format(_T("%d"), m_TrainDataParam.m_lLineFilterHalfWidth);
	cXMLConfigurator.AddElem(_T("LineFilterHalfWidth"), strInfo);
	// outlinenum
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineOutLineNum);
	cXMLConfigurator.AddElem(_T("LineOutLineNum"), strInfo);
	// edgeproperty
	strInfo.Format(_T("%d"), m_TrainDataParam.m_edgeLineProperty);
	cXMLConfigurator.AddElem(_T("LineEdgeProperty"), strInfo);
	// SortByScore
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bLineSortByScore);
	cXMLConfigurator.AddElem(_T("LineSortByScore"), strInfo);
	// DualEdge1
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bLineEnableDualEdge);
	cXMLConfigurator.AddElem(_T("LineDualEdge"), strInfo);
	// EnableFitLineDMS
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bEnableFitLineDMS);
	cXMLConfigurator.AddElem(_T("EnableFitLineDMS"), strInfo);
	// FitLineDMS
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dFitLineDMS);
	cXMLConfigurator.AddElem(_T("FitLineDMS"), strInfo);
	// LineAngleType
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLineAngleType);
	cXMLConfigurator.AddElem(_T("LineAngleType"), strInfo);

	//  //将TrainGuiParam的参数，放在TrainDataParam括号中 	
	// 	cXMLConfigurator.LeaveElem();
	// 
	// 	//////////////TrainGuiParam/////////////
	// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
	// 	cXMLConfigurator.EnterElem();

	// train affine rect
	cXMLConfigurator.AddElem(_T("TrainRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetWidth());
	cXMLConfigurator.AddElem(_T("XLength"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetHeight());
	cXMLConfigurator.AddElem(_T("YLength"), strInfo);
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
	// circle center
	cXMLConfigurator.AddElem(_T("CircleCenter"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CircleCenter.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// circle radius 
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dRadius);
	cXMLConfigurator.AddElem(_T("CircleRadius"), strInfo);
	// Arc start 
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ArcStart.ToDouble());
	cXMLConfigurator.AddElem(_T("ArcStart"), strInfo);
	// Arc Span
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ArcSpan.ToDouble());
	cXMLConfigurator.AddElem(_T("ArcSpan"), strInfo);
	// Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nCaliperNum);
	cXMLConfigurator.AddElem(_T("CaliperNum"), strInfo);
	// Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperProjectLen);
	cXMLConfigurator.AddElem(_T("ProjectLen"), strInfo);
	// Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dCaliperSearchLen);
	cXMLConfigurator.AddElem(_T("SearchLen"), strInfo);
	// Search Dircetion
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_CaliperSearchDirection);
	cXMLConfigurator.AddElem(_T("SearchDir"), strInfo);
	//CircleCaliper Visible
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleCaliperVisible);
	cXMLConfigurator.AddElem(_T("CircleCaliperVisible"), strInfo);

	// CircleCaliperMaskEnable
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCircleEnableMask);
	cXMLConfigurator.AddElem(_T("CircleCaliperMaskEnable"), strInfo);
	CString strTmp;
	cXMLConfigurator.DeleteElem(_T("CircleMaskParam"));
	cXMLConfigurator.AddElem(_T("CircleMaskParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdCircleCaliperMaskData.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vdCircleCaliperMaskData.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdCircleCaliperMaskData.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbCircleCaliperMask.size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbCircleCaliperMask.size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbCircleCaliperMask.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	// Caliper Num 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_nLineCaliperNum);
	cXMLConfigurator.AddElem(_T("LineCaliperNum"), strInfo);
	// startX
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartX);
	cXMLConfigurator.AddElem(_T("LineStartX"), strInfo);
	// startY
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineStartY);
	cXMLConfigurator.AddElem(_T("LineStartY"), strInfo);
	// EndX
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndX);
	cXMLConfigurator.AddElem(_T("LineEndX"), strInfo);
	// EndY
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineEndY);
	cXMLConfigurator.AddElem(_T("LineEndY"), strInfo);
	// Project Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperProjectLen);
	cXMLConfigurator.AddElem(_T("LineProjectLen"), strInfo);
	// Search Len
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_dLineCaliperSearchLen);
	cXMLConfigurator.AddElem(_T("LineSearchLen"), strInfo);
	// Search Rotation
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_LineCaliperSearchRotation.ToDouble());
	cXMLConfigurator.AddElem(_T("LineSearchRotation"), strInfo);
	//LineCaliper Visible1
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineCaliperVisible);
	cXMLConfigurator.AddElem(_T("LineCaliperVisible"), strInfo);

	// LineCaliperMaskEnable
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineEnableMask);
	cXMLConfigurator.AddElem(_T("LineCaliperMaskEnable"), strInfo);
	cXMLConfigurator.DeleteElem(_T("LineMaskParam"));
	cXMLConfigurator.AddElem(_T("LineMaskParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vdLineCaliperMaskData.size());
	cXMLConfigurator.AddElem(_T("MaskDataNum"), strInfo);
	for (int j=0; j<m_TrainGuiParam.m_vdLineCaliperMaskData.size(); j++)
	{
		strTmp.Format(_T("MaskData%d"), j);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_vdLineCaliperMaskData.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_vbLineCaliperMask.size());
	cXMLConfigurator.AddElem(_T("EnableMaskDataNum"), strInfo);

	for (int j=0; j<m_TrainGuiParam.m_vbLineCaliperMask.size(); j++)
	{
		strTmp.Format(_T("EnableMaskData%d"), j);
		strInfo.Format(_T("%d"), (int)m_TrainGuiParam.m_vbLineCaliperMask.at(j));
		cXMLConfigurator.AddElem(strTmp, strInfo);
	}
	cXMLConfigurator.LeaveElem();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bCorrectAngle);
	cXMLConfigurator.AddElem(_T("CorrectAngle"), strInfo);

	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"),m_SearchDataParam.m_dConfusionThre);
	cXMLConfigurator.AddElem(_T("ConfusionThre"),strInfo);


	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowLinePoint);
	cXMLConfigurator.AddElem(_T("LineFitPointVisible"), strInfo);

	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bEnableShowCirclePoint);
	cXMLConfigurator.AddElem(_T("CircleFitPointVisible"), strInfo);

	strInfo.Format(_T("%d"),m_SearchDataParam.m_bEnableLineCircleCross);
	cXMLConfigurator.AddElem(_T("EnableLineCircleCross"),strInfo);
	strInfo.Format(_T("%d"),m_SearchDataParam.m_nLineCircleCrossMode);
	cXMLConfigurator.AddElem(_T("LineCircleCrossMode"),strInfo);
	
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dOffsetRatio);
	cXMLConfigurator.AddElem(_T("OffsetRatio"), strInfo);

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
	strInfo.Format(_T("%f"), m_TrainResult.m_dCenterX);
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dCenterY);
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dRadius);
	cXMLConfigurator.AddElem(_T("Radius"), strInfo);	

	cXMLConfigurator.AddElem(_T("PatternChordPoint"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_pointPatternChord.GetX());
	cXMLConfigurator.AddElem(_T("PatternChordPointX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_pointPatternChord.GetY());
	cXMLConfigurator.AddElem(_T("PatternChordPointY"), strInfo);
	cXMLConfigurator.LeaveElem();

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
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 更新训练控件数据信息	
bool CPatternChordSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainRect	        = m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();

		m_tmpTrainGuiParam.m_nCaliperNum		= m_GuiFindCircle.GetCaliperNum();
		m_tmpTrainGuiParam.m_nLineWidth			= m_GuiTrainRect.GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor			= m_GuiTrainRect.GetLineColor();
		m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius,
			m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_tmpTrainGuiParam.m_dCaliperProjectLen = m_GuiFindCircle.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dCaliperSearchLen	= m_GuiFindCircle.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_CaliperSearchDirection = (CaliperSearchDirection)m_GuiFindCircle.GetCaliperSearchDirection();
		m_tmpTrainGuiParam.m_bCircleCaliperVisible=m_GuiFindCircle.IsCaliperVisible();
		m_GuiFindCircle.GetMaskData(m_tmpTrainGuiParam.m_vdCircleCaliperMaskData);
		m_GuiFindCircle.GetCaliperMasked(m_tmpTrainGuiParam.m_vbCircleCaliperMask);

		m_tmpTrainGuiParam.m_nLineCaliperNum		= m_GuiFindLine.GetCaliperNum();
		m_tmpTrainGuiParam.m_dLineCaliperProjectLen	= m_GuiFindLine.GetCaliperProjectionLength();
		m_tmpTrainGuiParam.m_dLineCaliperSearchLen		= m_GuiFindLine.GetCaliperSearchLength();
		m_tmpTrainGuiParam.m_LineCaliperSearchRotation	= m_GuiFindLine.GetCaliperSearchDirection();
		m_GuiFindLine.GetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX, m_tmpTrainGuiParam.m_dLineStartY, m_tmpTrainGuiParam.m_dLineEndX,
			m_tmpTrainGuiParam.m_dLineEndY);
		m_GuiFindLine.GetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData);
		m_GuiFindLine.GetCaliperMasked(m_tmpTrainGuiParam.m_vbLineCaliperMask);
		m_tmpTrainGuiParam.m_bLineCaliperVisible=m_GuiFindLine.IsCaliperVisible();
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);

		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiFindCircle.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindCircle.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindCircle.SetCaliperNum(m_tmpTrainGuiParam.m_nCaliperNum);
		m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan(m_tmpTrainGuiParam.m_CircleCenter, m_tmpTrainGuiParam.m_dRadius
			,m_tmpTrainGuiParam.m_ArcStart, m_tmpTrainGuiParam.m_ArcSpan);
		m_GuiFindCircle.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dCaliperProjectLen);
		m_GuiFindCircle.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dCaliperSearchLen);
		m_GuiFindCircle.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_CaliperSearchDirection);
		m_GuiFindCircle.SetCaliperVisible(m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetAnnulusFindCircleVisible(!m_tmpTrainGuiParam.m_bCircleCaliperVisible);
		m_GuiFindCircle.SetMaskData(m_tmpTrainGuiParam.m_vdCircleCaliperMaskData);

		m_GuiFindLine.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiFindLine.SetCaliperNum(m_tmpTrainGuiParam.m_nLineCaliperNum);
		m_GuiFindLine.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiFindLine.SetStartXYEndXY(m_tmpTrainGuiParam.m_dLineStartX, m_tmpTrainGuiParam.m_dLineStartY, m_tmpTrainGuiParam.m_dLineEndX,
			m_tmpTrainGuiParam.m_dLineEndY);
		m_GuiFindLine.SetCaliperProjectionLength(m_tmpTrainGuiParam.m_dLineCaliperProjectLen);
		m_GuiFindLine.SetCaliperSearchLength(m_tmpTrainGuiParam.m_dLineCaliperSearchLen);
		m_GuiFindLine.SetCaliperSearchDirection(m_tmpTrainGuiParam.m_LineCaliperSearchRotation);

		m_GuiFindLine.SetCaliperVisible(m_tmpTrainGuiParam.m_bLineCaliperVisible);
		m_GuiFindLine.SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bLineCaliperVisible);
		m_GuiFindLine.SetMaskData(m_tmpTrainGuiParam.m_vdLineCaliperMaskData);
	}

	return true;
}

bool CPatternChordSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiSearchCircleArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiCircleSearchResultFitPointArray.size(); j++)
		{
			//m_pGuiCircleSearchResultFitPointArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiCircleSearchResultFitPointArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiSearchCircleCenterArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleCenterArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiSearchLineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiSearchLineCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchLineCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			//m_pGuiSearchResultFitPoint1Array[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchResultFitPoint1Array[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary1[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCrossArrayAuxilary2[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}


		for (j = 0; j < m_pGuiSearchCircleCenterPointAndLinePointCrossArray.size(); j++)
		{
			m_pGuiSearchCircleCenterPointAndLinePointCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleCenterPointAndLinePointCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j = 0; j < m_pGuiSearchCircleCenterPointAndLinePointSegArray.size(); j++)
		{
			m_pGuiSearchCircleCenterPointAndLinePointSegArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCircleCenterPointAndLinePointSegArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}

	return true;
}

// 清除控件及其显示					
bool CPatternChordSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

bool CPatternChordSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CPatternChordSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchCircleArray);
	ClearGraphicsArray(m_pGuiCircleSearchResultFitPointArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPoint1Array);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary1);
	ClearGraphicsArray(m_pGuiSearchCrossArrayAuxilary2);

	ClearGraphicsArray(m_pGuiSearchCircleCenterPointAndLinePointCrossArray);
	ClearGraphicsArray(m_pGuiSearchCircleCenterPointAndLinePointSegArray);

	return true;
}															

bool CPatternChordSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindCircle"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_FindLineTool"));

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultCircle"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultRect"));
	}

	return true;	
}

bool CPatternChordSearchTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchCircleArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCircleCenterArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiCircleSearchResultFitPointArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiCircleSearchResultFitPointArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchLineCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchLineCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchResultFitPoint1Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultFitPoint1Array[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary1.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary1[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCrossArrayAuxilary2.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCrossArrayAuxilary2[j]->GetTipText());
		}


		for (j=0; j<m_pGuiSearchCircleCenterPointAndLinePointCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterPointAndLinePointCrossArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCircleCenterPointAndLinePointSegArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCircleCenterPointAndLinePointSegArray[j]->GetTipText());
		}
	}

	return true;	
}

// 获取训练模板原始图像
bool CPatternChordSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CPatternChordSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternChordSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternChordSearchTool::SetLanguage(int nLanguage)
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
		m_GuiFindCircle.SetLabel(_T("FindCircle"));
		m_GuiFindLine.SetLabel(_T("FindLine"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiFindCircle.SetLabel(_T("找圆工具"));
		m_GuiFindLine.SetLabel(_T("找线工具"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		break;
	}
}

int CPatternChordSearchTool::GetPointYOnCircle(double dCenterX, double dCenterY, double dRadius, int nXOnCircle)
{
	double dYOnCircle = -sqrt(dRadius*dRadius - (nXOnCircle - dCenterX) * (nXOnCircle - dCenterX));
	if (_isnan(dYOnCircle))
	{
		dYOnCircle = 0.0f;
	}
	dYOnCircle += dCenterY;
	double dAfterPoint = dYOnCircle - (int)dYOnCircle;
	int nResult = 0;
	if (dAfterPoint >= 0.5f)
	{
		nResult = (int)dYOnCircle + 1;
	}
	else
	{
		nResult = (int)dYOnCircle;
	}

	return nResult;
}

bool CPatternChordSearchTool::GetPatternChordPoint(cpImage &ImageSearch, const scCircle& ResCircle, sc2Vector& outputPoint)
{
	sc2Vector circleCenter = ResCircle.GetCenter();
	double dRadius = ResCircle.GetRadius();
	//ResCircle.GetPoint()
	std::vector<double> vDistances;
	std::vector<std::pair<sc2Vector, sc2Vector> > vPoints;
	int nXMin = -cos(scRadian(scDegree(30))) * dRadius;
	int nXMax = -nXMin;
	for (int nXOnCircle = nXMin; nXOnCircle < nXMax; ++nXOnCircle)
	{
		double dXOnCircle = nXOnCircle + circleCenter.GetX();
		double dYOnCircle = GetPointYOnCircle(circleCenter.GetX(), circleCenter.GetY(), dRadius, dXOnCircle);
		sc2Vector point0(circleCenter.GetX(), circleCenter.GetY() - dRadius);
		sc2Vector point1(dXOnCircle, dYOnCircle);
		scLineSeg lineSeg0(point0, ResCircle.GetCenter());
		scLineSeg lineSeg1(point1, ResCircle.GetCenter());
		scRadian radianLine;
		sfAngleLine2Line(lineSeg0.GetLine(), lineSeg1.GetLine(), radianLine);
		scAffineRect affineRect(sc2Vector(dXOnCircle, dYOnCircle), 400.0f, 1.0f, radianLine + scRadian(scDegree(90)), scRadian(0.0f));
		scCaliperTwoEdge caliperTwoEdge;
		caliperTwoEdge.SetEdgePolarity(eLightToDark, eDarkToLight);
		caliperTwoEdge.SetEdgePairWidth(10);
		caliperTwoEdge.SetContrastThreshold(10);
		caliperTwoEdge.SetFilterHalfSize(1);
		caliperTwoEdge.SetMaxResultNum(1);
		caliperTwoEdge.SetSampleAffineRect(affineRect);

		bool bRet = caliperTwoEdge.Execute(ImageSearch);
		if (bRet)
		{
			int resultNum = caliperTwoEdge.GetResultNum();
			if (resultNum <= 0)
			{
				continue;
			}
			scCaliperTwoEdgeResult* pCaliperResult = caliperTwoEdge.GetCaliperResult();
			if (NULL == pCaliperResult)
			{
				continue;
			}
			sc2Vector resultPoint1(pCaliperResult->m_dEdgePoint1X, pCaliperResult->m_dEdgePoint1Y);
			sc2Vector resultPoint2(pCaliperResult->m_dEdgePoint2X, pCaliperResult->m_dEdgePoint2Y);
			double dDistance = 0.0f;
			sfDistancePoint2Point(resultPoint1, resultPoint2, dDistance);
			vDistances.push_back(dDistance);
			vPoints.push_back(std::make_pair(resultPoint1, resultPoint2));
		}
	}

	if (vDistances.size() > 3)
	{
		int nMinPos = 0;
		double dMinDistance = vDistances[0];
		for (int nPos = 1; nPos < vDistances.size() - 1; ++nPos)
		{
			// 判断是否为极值点
			double dPreDistance = vDistances[nPos - 1];
			double dCurDistance = vDistances[nPos];
			double dNexDistance = vDistances[nPos + 1];
			if ((dCurDistance - dPreDistance) * (dCurDistance - dNexDistance) >= 0 && dCurDistance < dPreDistance)
			{
				// 是否为最小值
				if (dCurDistance < dMinDistance)
				{
					dMinDistance = dCurDistance;
					nMinPos = nPos;
				}
			}

		}

		outputPoint = vPoints[nMinPos].first;
		scLineSeg lineZeroDegree(sc2Vector(ResCircle.GetCenter().GetX(), ResCircle.GetCenter().GetY() - ResCircle.GetRadius()), ResCircle.GetCenter());
		scLineSeg lineResult(outputPoint, ResCircle.GetCenter());
		scRadian radianResult;
		sfAngleLine2Line(lineZeroDegree.GetLine(), lineResult.GetLine(), radianResult);
		scDegree degreeResult(radianResult.SignedNormMod180());
		return true;
	}
	return false;
}

bool CPatternChordSearchTool::GetPointNearestCircleCenter(sc2Vector& vPoint, scCircle& ResCircle, std::vector<sc2Vector>&  vPoints)
{
	if (vPoints.size() <= 0)
	{
		return false;
	}

	if (vPoints.size() < 2)
	{
		vPoint = vPoints[0];
		return true;
	}
	double dMinDistance = 0.0f;
	sfDistancePoint2Point(vPoints[0], ResCircle.GetCenter(),  dMinDistance);
	CString strTmpKey = _T("KingFires");
	for (int i = 1; i < vPoints.size(); ++i)
	{
		double dDistance = 0.0f;
		sfDistancePoint2Point(vPoints[i], ResCircle.GetCenter(), dDistance);
		if (dMinDistance > dDistance)
		{
			dMinDistance = dDistance;
			vPoint = vPoints[i];
		}

		//scGuiCross* pCross = new scGuiCross;
		//strTmpKey.Format(_T("%s_FitCross%d"), m_strKeyText, i);
		////scGuiCross *pTempCross = m_GuiSearchCross+i;
		//pCross->SetCenterExtent(vPoints[i], sc2Vector(40,40));
		//pCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
		//pCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		//pCross->SetTipTextEnabled(true);
		//pCross->SetTipText(strTmpKey);
		//m_pGuiSearchCrossArray.push_back(pCross);

		//m_pGuiSearchCrossArray.push_back();
	}
	//scGuiCross* pCross = new scGuiCross;
	//strTmpKey.Format(_T("%s_FitCrossTarget"), m_strKeyText);
	////scGuiCross *pTempCross = m_GuiSearchCross+i;
	//pCross->SetCenterExtent(vPoint, sc2Vector(40,40));
	//pCross->SetLineColor(COLORREF(RGB(255,0,0)));
	//pCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
	//pCross->SetTipTextEnabled(true);
	//pCross->SetTipText(strTmpKey);
	//m_pGuiSearchCrossArray.push_back(pCross);
	return true;
}

bool CPatternChordSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
    if(nWidth<1||nHeight<1)return false;
    if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

    //训练区域
    double centerx,centery, xlen,ylen;

    m_GuiTrainRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //搜索区域
    m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //找圆
    double rad,asta,aspan;
    m_GuiFindCircle.GetCenterRadiusAngleStartAngleSpan(centerx,centery,rad,asta,aspan);
    m_GuiFindCircle.SetCenterRadiusAngleStartAngleSpan((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight,(rad/m_CamWidth)*nWidth,asta,aspan);

    double plength,slength;
    plength = m_GuiFindCircle.GetCaliperProjectionLength();
    slength = m_GuiFindCircle.GetCaliperSearchLength();
    m_GuiFindCircle.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindCircle.SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    //卡尺
    double sx,sy,ex,ey;
    m_GuiFindLine.GetStartXYEndXY(sx,sy,ex,ey);
    m_GuiFindLine.SetStartXYEndXY((sx/m_CamWidth)*nWidth, (sy/m_CamHeight)*nHeight,(ex/m_CamWidth)*nWidth,(ey/m_CamHeight)*nHeight);

    plength = m_GuiFindLine.GetCaliperProjectionLength();
    slength = m_GuiFindLine.GetCaliperSearchLength();
    m_GuiFindLine.SetCaliperProjectionLength(plength/m_CamWidth*nWidth);
    m_GuiFindLine.SetCaliperSearchLength(slength/m_CamWidth*nWidth);

    //参考点位置
    m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
    m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

    m_CamHeight = nHeight;
    m_CamWidth = nWidth;

    return true;

}
CString	CPatternChordSearchTool::GetResultErrInfo()
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
				strErrInfo += _T("Find circle failed");
				break;
			case -3:							
				strErrInfo += _T("Find line failed");
				break;
			case -6:
				strErrInfo += _T("Line Fit DMS failed");
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
				strErrInfo += _T("找圆失败");
				break;
			case -3:							
				strErrInfo += _T("找线失败");
				break;
			case -6:
				strErrInfo += _T("线拟合残差超上限");
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
				strErrInfo += _T("找圆失败");
				break;
			case -3:							
				strErrInfo += _T("找线失败");
				break;
			case -6:
				strErrInfo += _T("线拟合残差超上限");
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
