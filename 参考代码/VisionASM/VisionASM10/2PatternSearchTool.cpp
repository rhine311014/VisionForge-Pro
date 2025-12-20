// 2PatternSearchTool.cpp: implementation of the C2PatternSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "2PatternSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"
#include "svLineSeg.h"
#include "svGuiLine.h"
#include "svIntersect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2PatternSearchTool::C2PatternSearchTool()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1-1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiModelPoint3.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
	m_GuiModelPoint3.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area1"));
		m_GuiModelPoint.SetLabel(_T("Reference Point1-1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1-2"));
		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2-1"));
		m_GuiModelPoint3.SetLabel(_T("Reference Point2-2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	}
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(130, 150, 230, 150);
	m_GuiFindLine[1].SetStartXYEndXY(480, 150, 580, 150);

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

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Accuracy2				= m_PatternSearchTool2.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale2			= m_PatternSearchTool2.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow2				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh2			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable2			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint1				= m_GuiModelPoint1.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_TrainRect2				= m_GuiTrainRect2.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint2				= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint3				= m_GuiModelPoint3.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect2				= m_GuiSearchRect2.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

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
	m_tmpSearchDataParam.m_dAcceptThre			= 0.8;
	
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

C2PatternSearchTool::C2PatternSearchTool(C2PatternSearchTool &c2PatternSearchTool)
{
	if (c2PatternSearchTool.GetSearchToolType() != e2PatternSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1-1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiModelPoint3.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
	m_GuiModelPoint3.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area1"));
		m_GuiModelPoint.SetLabel(_T("Reference Point1-1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1-2"));
		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2-1"));
		m_GuiModelPoint3.SetLabel(_T("Reference Point2-2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	}
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(130, 150, 230, 150);
	m_GuiFindLine[1].SetStartXYEndXY(480, 150, 580, 150);

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
	m_CamHeight								= c2PatternSearchTool.m_CamHeight;
	m_CamWidth								= c2PatternSearchTool.m_CamWidth;
	m_TrainDataParam						= c2PatternSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= c2PatternSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= c2PatternSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= c2PatternSearchTool.m_SearchGuiParam;
	m_TrainResult							= c2PatternSearchTool.m_TrainResult;
	m_ImageTrain							= c2PatternSearchTool.m_ImageTrain;
	m_ImageMask								= c2PatternSearchTool.m_ImageMask;
	m_ImageMask2							= c2PatternSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= c2PatternSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= c2PatternSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= c2PatternSearchTool.m_bTrainOK;
	m_pGuiDisplay							= c2PatternSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	c2PatternSearchTool.UpdateSearchGuiData(true);
	c2PatternSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&c2PatternSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&c2PatternSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&c2PatternSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&c2PatternSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(c2PatternSearchTool.m_tmpImageTrain);
	SetMaskImage(c2PatternSearchTool.m_tmpImageMask);
	SetMaskImage2(c2PatternSearchTool.m_tmpImageMask2);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = c2PatternSearchTool.m_PatternSearchTool;
		m_PatternSearchTool2 = c2PatternSearchTool.m_PatternSearchTool2;
	}
}

C2PatternSearchTool& C2PatternSearchTool::operator=(C2PatternSearchTool &c2PatternSearchTool)
{
	if (this == &c2PatternSearchTool)
	{
		return *this;	
	}

	if (c2PatternSearchTool.GetSearchToolType() != e2PatternSearchTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(180, 240, 100, 100);
	//m_GuiTrainRect.SetLabel(_T("训练区域1"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(180, 240, 300, 400);
	//m_GuiSearchRect.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点1-1"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(180, 240, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	m_GuiTrainRect2.SetCenterWidthHeight(530, 240, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(530, 240, 300, 400);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiModelPoint3.SetCenterXYWidthHeight(530, 240, 40, 40);
	//m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
	m_GuiModelPoint3.SetLabelVisible(TRUE);
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area1"));
		m_GuiSearchRect.SetLabel(_T("Search Area1"));
		m_GuiModelPoint.SetLabel(_T("Reference Point1-1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1-2"));
		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2-1"));
		m_GuiModelPoint3.SetLabel(_T("Reference Point2-2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	}
	m_GuiFindLine[0].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);

	m_GuiFindLine[1].SetCaliperParams(20,scDegree(90), 50, 5, TRUE);
	m_GuiFindLine[0].SetStartXYEndXY(130, 150, 230, 150);
	m_GuiFindLine[1].SetStartXYEndXY(480, 150, 580, 150);
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
	m_CamHeight								= c2PatternSearchTool.m_CamHeight;
	m_CamWidth								= c2PatternSearchTool.m_CamWidth;
	m_TrainDataParam						= c2PatternSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= c2PatternSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= c2PatternSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= c2PatternSearchTool.m_SearchGuiParam;
	m_TrainResult							= c2PatternSearchTool.m_TrainResult;

	m_ImageTrain							= c2PatternSearchTool.m_ImageTrain;
	m_ImageMask								= c2PatternSearchTool.m_ImageMask;
	m_ImageMask2							= c2PatternSearchTool.m_ImageMask2;
	m_bTrainGuiShow							= c2PatternSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= c2PatternSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= c2PatternSearchTool.m_bTrainOK;
	m_pGuiDisplay							= c2PatternSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	c2PatternSearchTool.UpdateSearchGuiData(true);
	c2PatternSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&c2PatternSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&c2PatternSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&c2PatternSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&c2PatternSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(c2PatternSearchTool.m_tmpImageTrain);
	SetMaskImage(c2PatternSearchTool.m_tmpImageMask);
	SetMaskImage2(c2PatternSearchTool.m_tmpImageMask2);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = c2PatternSearchTool.m_PatternSearchTool;
		m_PatternSearchTool2 = c2PatternSearchTool.m_PatternSearchTool2;
	}
	return *this;	
}



C2PatternSearchTool::~C2PatternSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType C2PatternSearchTool::GetSearchToolType()
{
	return e2PatternSearchTool;
}										   

// 设置显示控件指针		
bool C2PatternSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* C2PatternSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool C2PatternSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool C2PatternSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool C2PatternSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool C2PatternSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool C2PatternSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool C2PatternSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	//m_GuiModelPoint1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	//m_GuiModelPoint3.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	
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
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		//m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint1, m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	

		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect2, m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));
		//m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint3, m_strKeyText+_T("_ModelPoint3"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect2, m_strKeyText+_T("_SearchRect2"));
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

// 更新搜索界面显示									
bool C2PatternSearchTool::UpdateSearchResultGuiDisplay()							
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
bool C2PatternSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 设置训练Data参数	
bool C2PatternSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((C2PatternSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool C2PatternSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	*((C2PatternSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool C2PatternSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((C2PatternSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool C2PatternSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((C2PatternSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool C2PatternSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool C2PatternSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool C2PatternSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool C2PatternSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 设置训练原始图像									
bool C2PatternSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask2 = ImageMask;
	
	return true;
}

// 获取训练原始图像								
bool C2PatternSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask2;
	
	return true;
}

// 执行训练											
bool C2PatternSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置训练参数
	if (!m_PatternSearchTool.SetAccuracy(m_tmpTrainDataParam.m_Accuracy))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	if (!m_PatternSearchTool.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(m_tmpTrainDataParam.m_dAngleLow - m_tmpTrainDataParam.m_dAngleHigh) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow)))
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return false;
		}
	}
 	else if (!m_PatternSearchTool.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow), scDegree(m_tmpTrainDataParam.m_dAngleHigh), scDegree(360)))
 	{
		m_nResultErrNumber=-1;       //区域定位1失败
 		return false;
 	}

	// 设置训练参数
	if (!m_PatternSearchTool2.SetAccuracy(m_tmpTrainDataParam.m_Accuracy2))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return false;
	}
	
	if (!m_PatternSearchTool2.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale2))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return false;
	}
	
	// 必须在训练时设置角度
	if (m_tmpTrainDataParam.m_dAngleLow2 == m_tmpTrainDataParam.m_dAngleHigh2) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool2.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2)))
		{
			m_nResultErrNumber=-2;       //区域定位2失败
			return false;
		}
	}
	if (!m_PatternSearchTool2.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2), scDegree(m_tmpTrainDataParam.m_dAngleHigh2), scDegree(360)))
	{
		m_nResultErrNumber=-2;			//区域定位2失败
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
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
// 			sc2Vector cModelOrigin;
// 			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
// 			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
// 			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
// 
// 			m_ImageTrain     = m_tmpImageTrain;
// 			m_ImageMask      = m_tmpImageMask;
// 			m_TrainDataParam = m_tmpTrainDataParam;
// 			m_TrainGuiParam  = m_tmpTrainGuiParam;
// 			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return m_bTrainOK;
		}

		//return bRet;
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
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
// 			sc2Vector cModelOrigin;
// 			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
// 			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
// 			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
//			
// 			m_ImageTrain     = m_tmpImageTrain;
// 			m_TrainDataParam = m_tmpTrainDataParam;
// 			m_TrainGuiParam  = m_tmpTrainGuiParam;
// 			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;      //区域定位1失败
			return m_bTrainOK;
		}

		//return bRet;
	}


	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable2)
	{
		bool bRet = false;
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool2.Train(tmpImage, tmpImageMask2, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		else
		{
			bRet = m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpImageMask2, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
// 			sc2Vector cModelOrigin;
// 			m_PatternSearchTool2.GetPatternOrigin(cModelOrigin);
// 			m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
// 			m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();
			
// 			m_ImageTrain     = m_tmpImageTrain;
// 			m_ImageMask2      = m_tmpImageMask2;
// 			m_TrainDataParam = m_tmpTrainDataParam;
// 			m_TrainGuiParam  = m_tmpTrainGuiParam;
// 			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-2;      //区域定位2失败
			return m_bTrainOK;
		}
		
		//return bRet;
	}
	else
	{
		bool bRet = false;
		if(bIsProcessImage)
		{
			bRet = m_PatternSearchTool2.Train(tmpImage, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		else
		{
			bRet = m_PatternSearchTool2.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_ModelPoint2,
				&m_tmpTrainGuiParam.m_TrainRect2);
		}
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
// 			sc2Vector cModelOrigin;
// 			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
// 			m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
// 			m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();
//			
// 			m_ImageTrain     = m_tmpImageTrain;
// 			m_TrainDataParam = m_tmpTrainDataParam;
// 			m_TrainGuiParam  = m_tmpTrainGuiParam;
// 			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-2;      //区域定位2失败
			return m_bTrainOK;
		}
		
		//return bRet;
	}

	if (m_bTrainOK)
	{
		if (m_tmpTrainDataParam.m_bMaskEnable)
		{
			m_ImageMask      = m_tmpImageMask;
		}

		if (m_tmpTrainDataParam.m_bMaskEnable2)
		{
			m_ImageMask2      = m_tmpImageMask2;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

		m_tmpTrainResult.m_dModelX1 = m_tmpTrainGuiParam.m_ModelPoint1.GetX();
		m_tmpTrainResult.m_dModelY1 = m_tmpTrainGuiParam.m_ModelPoint1.GetY();

		m_PatternSearchTool2.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();
		m_tmpTrainResult.m_dModelX3 = m_tmpTrainGuiParam.m_ModelPoint3.GetX();
		m_tmpTrainResult.m_dModelY3 = m_tmpTrainGuiParam.m_ModelPoint3.GetY();

		m_bTrainOK = false;
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
			m_nResultErrNumber=-3;      //找线1失败
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
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1],tmpEdgePolarity, m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
		} 
		else
		{
			m_FindLineTool[0].SetCaliperProperty(m_tmpTrainDataParam.m_dContrastThre[1], m_tmpTrainDataParam.m_edgeProperty[1], m_tmpTrainDataParam.m_lFilterHalfWidth[1]);
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
			m_nResultErrNumber=-4;      //找线2失败
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg2.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			m_nResultErrNumber=-6;      //求交点失败
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

		m_bTrainOK = true;
		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
	}

	return m_bTrainOK;
}

// 获取训练结果图像														
bool C2PatternSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool C2PatternSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool2.GetPatternImage(m_ImageTrainResult2);
	if (!bRet)
	{
		return false;
	}
	
	ImageTrainResult = m_ImageTrainResult2;
	
	return true;
}	

// 获取训练结果
bool C2PatternSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	*((C2PatternSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}							

// 是否训练成功
bool C2PatternSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool C2PatternSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((C2PatternSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool C2PatternSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}

	*((C2PatternSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool C2PatternSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((C2PatternSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool C2PatternSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != e2PatternSearchTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((C2PatternSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool C2PatternSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber = -99; // 其他失败
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber = 0;
	m_tmpSearchDataParam.m_nSearchNum = 1;
	scLineSeg modelLineSeg1; // 训练时的线段1
	scLineSeg modelLineSeg2; // 训练时的线段2
	modelLineSeg1.SetP1(sc2Vector(m_TrainResult.m_dStartX[0], m_TrainResult.m_dStartY[0]));
	modelLineSeg1.SetP2(sc2Vector(m_TrainResult.m_dEndX[0], m_TrainResult.m_dEndY[0]));
	modelLineSeg2.SetP1(sc2Vector(m_TrainResult.m_dStartX[1], m_TrainResult.m_dStartY[1]));
	modelLineSeg2.SetP2(sc2Vector(m_TrainResult.m_dEndX[1], m_TrainResult.m_dEndY[1]));
	
	// 设置搜索个数
	if (!m_PatternSearchTool.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -1;//区域定位1失败
		return false;
	}

	// 设置接收阈值
	if (!m_PatternSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -1;//区域定位1失败
		return false;
	}


	// 设置搜索个数
	if (!m_PatternSearchTool2.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum))
	{
		m_nResultErrNumber = -2;//区域定位2失败
		return false;
	}
	
	// 设置接收阈值
	if (!m_PatternSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		m_nResultErrNumber = -2;//区域定位2失败
		return false;
	}
	for (int i=0; i<LINE_NUM; i++)
	{
		m_FindLineTool[i].SetCaliperNum(m_TrainGuiParam.m_nCaliperNum[i]);
		m_FindLineTool[i].SetCaliperParams(m_TrainGuiParam.m_nCaliperNum[i], m_TrainGuiParam.m_CaliperSearchRotation[i], m_TrainGuiParam.m_dCaliperSearchLen[i]
		,m_TrainGuiParam.m_dCaliperProjectLen[i]);
		m_FindLineTool[i].SetCaliperProperty(m_TrainDataParam.m_dContrastThre[i], m_TrainDataParam.m_edgeProperty[i], m_TrainDataParam.m_lFilterHalfWidth[i]);
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
			m_nResultErrNumber=-1;       //区域定位1失败
			return false;
		}
	}
	else
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return bRet;
	}
	if(bIsProcessImage)
	{
		bRet = m_PatternSearchTool2.Execute(tmpImage, m_TrainGuiParam.m_SearchRect2);
	}
	else
	{
		bRet = m_PatternSearchTool2.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect2);
	}
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool2.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber=-2;       //区域定位2失败
			return false;
		}
	}
	else
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return bRet;
	}


	// 求直线交点
	bool   bFindLine[LINE_NUM];
	scLineSeg ResLine[LINE_NUM];
	double dRMS[LINE_NUM];
	scPatternSearchRotResult tmpPatternResult;
	scPatternSearchRotResult tmpPatternResult2;
	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			tmpPatternResult = m_PatternSearchTool.GetResult(i);

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

		
			
			for (int k=0; k<1; k++)
			{
				bFindLine[k] = 0;
			}

			for (int j=0; j<1; j++)
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
				if (!bFindLine[j])
				{
					m_nResultErrNumber = -3; // 找线1失败
					return false;
				}
			}

		}

	}
	else
	{
		m_nResultErrNumber = -2; // 区域定位2失败
		return bRet;
	}

	if (bRet)
	{
		// 获取结果
		int nResultNum = m_PatternSearchTool2.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -2; // 区域定位2失败
			return false;
		}
		for (int i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			tmpPatternResult2 = m_PatternSearchTool2.GetResult(i);

			// 求变换矩阵
			sc2Vector pos = tmpPatternResult2.GetPosition();
			sc2Vector tmModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_PatternSearchTool2.GetPatternOrigin(tmModelPoint);
			dRotation = scRadian(tmpPatternResult2.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());

			
			for (int k=1; k<LINE_NUM; k++)
			{
				bFindLine[k] = 0;
			}

			for (int j=1; j<LINE_NUM; j++)
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
				if (!bFindLine[j])
				{
					m_nResultErrNumber = -4; // 找线2失败
					return false;
				}
			}

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
				//double dRotAngle = scDegree((ResLine[0].GetRotation()-modelLineSeg1.GetRotation()+ResLine[0].GetRotation()-modelLineSeg1.GetRotation())/2.0).ToDouble(); 
				double dRotAngle = scDegree((ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180()+ResLine[0].GetRotation().NormMod180()-modelLineSeg1.GetRotation().NormMod180())/2.0).ToDouble(); 
				searchResult.m_dAngle = dRotAngle;
				searchResult.m_dScore = tmpPatternResult.GetScore();

				searchResult.m_vdAuxiliaryPosX[0] = pointCenter1.GetX();
				searchResult.m_vdAuxiliaryPosY[0] = pointCenter1.GetY();

				searchResult.m_vdAuxiliaryPosX[1] = pointCenter2.GetX();
				searchResult.m_vdAuxiliaryPosY[1] = pointCenter2.GetY();

				// 添加线结果
				CResultLine line1;
				CResultLine line2;

				line1.m_nLineNum = 1;
				line1.m_vLineX[0] = ResLine[0].GetP1().GetX();
				line1.m_vLineY[0] = ResLine[0].GetP1().GetY();
				line1.m_vLineX[1] = ResLine[0].GetP2().GetX();
				line1.m_vLineY[1] = ResLine[0].GetP2().GetY();
				
				line2.m_nLineNum = 1;
				line2.m_vLineX[0] = ResLine[1].GetP1().GetX();
				line2.m_vLineY[0] = ResLine[1].GetP1().GetY();
				line2.m_vLineX[1] = ResLine[1].GetP2().GetX();
				line2.m_vLineY[1] = ResLine[1].GetP2().GetY();

				searchResult.m_vLineSearchResult.push_back(line1);
				searchResult.m_vLineSearchResult.push_back(line2);

				CResultSearchRect resRect;
				resRect.m_searchRect = tmpPatternResult.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);
				resRect.m_searchRect = tmpPatternResult2.MatchRegion();
				searchResult.m_vTrainSearchRect.push_back(resRect);

				m_SearchResultArray.push_back(searchResult);

				CString	strTmpKey;
				int i=0;
				strTmpKey.Format(_T("工具1%s_ResultRect%d"), m_strKeyText, i);
				//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
				scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
				pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
				pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect->SetTipTextEnabled(true);
				pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchRectArray.push_back(pTempAffineRect);

				strTmpKey.Format(_T("工具2%s_ResultRect%d"), m_strKeyText, i);
				//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
				scGuiAffineRect *pTempAffineRect2 = m_GuiSearchAffineRect2+i;
				pTempAffineRect2->SetAffineRect(tmpPatternResult2.MatchRegion());
				pTempAffineRect2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempAffineRect2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempAffineRect2->SetTipTextEnabled(true);
				pTempAffineRect2->SetTipText((LPCTSTR)strTmpKey);
				m_pGuiSearchRectArray.push_back(pTempAffineRect2);

				for (int l=0; l<2; l++)
				{
					strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, l, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*2+l;
					pTemLine->SetLine(ResLine[l].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);
				}

				strTmpKey.Format(_T("%s_ResultLineCross%d"), m_strKeyText, i);
				//scGuiCross *pTempLineCross = new scGuiCross;
				scGuiCross *pTempLineCross = m_GuiSearchLineCross+i;
				pTempLineCross->SetCenterExtent(crossPointArray[0], sc2Vector(40,40));
				pTempLineCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
				pTempLineCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempLineCross->SetTipTextEnabled(true);
				pTempLineCross->SetTipText(strTmpKey);


				strTmpKey.Format(_T("工具1%s_ResultCross%d"), m_strKeyText, i);
				scGuiCross *pTempCross = m_GuiSearchCross2+i*2+1;
				pTempCross->SetCenterExtent(pointCenter1, sc2Vector(40,40));
				pTempCross->SetLineColor(RGB(255,0,0));
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);			
				m_pGuiSearchCrossArray.push_back(pTempCross);

				strTmpKey.Format(_T("工具2%s_ResultCross%d"), m_strKeyText, i);
				pTempCross = m_GuiSearchCross2+i*2+2;
				pTempCross->SetCenterExtent(pointCenter2, sc2Vector(40,40));
				pTempCross->SetLineColor(RGB(255,0,0));
				pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
				pTempCross->SetTipTextEnabled(true);
				pTempCross->SetTipText(strTmpKey);			
				m_pGuiSearchCrossArray.push_back(pTempCross);

				CString st;
				st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
				pTempLineCross->SetLabel(st);
				pTempLineCross->SetLabelVisible(TRUE);
				m_pGuiSearchLineCrossArray.push_back(pTempLineCross);

			}

		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	else
	{
		m_nResultErrNumber = -2; // 区域定位2失败
		return bRet;
	}

	return bRet;
}

// 获取搜索结果个数										
int C2PatternSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool C2PatternSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool C2PatternSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != e2PatternSearchTool)
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
bool C2PatternSearchTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或2PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	C2PatternSearchTrainDataParam tmpTrainDataParam;
	C2PatternSearchTrainGuiParam tmpTrainGuiParam;
	C2PatternSearchSearchDataParam tmpSearchDataParam;
	C2PatternSearchSearchGuiParam tmpSearchGuiParam;	
	C2PatternSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != e2PatternSearchTool)
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

	// coarse scale
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale2 = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy2 = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable2"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable2 = _ttoi(cXMLConfigurator.GetElemData());
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

	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint1"));
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




	// Train rect
	bRet = cXMLConfigurator.FindElem(_T("TrainRect2"));
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
	bRet = cXMLConfigurator.FindElem(_T("SearchRect2"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect2.GetOrigin().GetX());
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
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint2.GetX());
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


	// model point
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint3"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_ModelPoint3.GetX());
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
	tmpTrainGuiParam.m_ModelPoint3 = vPos;
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
	bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAcceptThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);


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
	scPatternSearchRot tmpPatternSearchTool;
	bRet = tmpPatternSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	scPatternSearchRot tmpPatternSearchTool2;
	bRet = tmpPatternSearchTool2.LoadTrainModelFromBinaryFile(strDir + _T("Model2.pts"));
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
	if (tmpTrainDataParam.m_bMaskEnable2)
	{
		m_tmpImageMask2 = tmpImageMask2;
	}
	m_PatternSearchTool = tmpPatternSearchTool;
	m_PatternSearchTool2 = tmpPatternSearchTool2;
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
bool C2PatternSearchTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) QuickSearch模板文件Pattern.ptq或2PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	// CoarseScale
	strInfo.Format(_T("%d"), m_TrainDataParam.m_CoarseScale2);
	cXMLConfigurator.AddElem(_T("CoarseScale2"), strInfo);
	// Accuracy
	strInfo.Format(_T("%d"), m_TrainDataParam.m_Accuracy2);
	cXMLConfigurator.AddElem(_T("Accuracy2"), strInfo);
	// Angle Low
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleLow2);
	cXMLConfigurator.AddElem(_T("AngleLow2"), strInfo);
	// Angle High
	strInfo.Format(_T("%f"), m_TrainDataParam.m_dAngleHigh2);
	cXMLConfigurator.AddElem(_T("AngleHigh2"), strInfo);
	// mask enable
	strInfo.Format(_T("%d"), m_TrainDataParam.m_bMaskEnable2);
	cXMLConfigurator.AddElem(_T("MaskEnable2"), strInfo);

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

	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();


	
	// train rect
	cXMLConfigurator.AddElem(_T("TrainRect2"));
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

	// model point
	cXMLConfigurator.AddElem(_T("ModelPoint3"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint3.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint3.GetY());
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
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中
// 	cXMLConfigurator.LeaveElem();
// 
// 	///////////////SearchGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("SearchGuiParam"));
// 	cXMLConfigurator.EnterElem();

// 
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

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
	cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
	cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);

	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX3);
	cXMLConfigurator.AddElem(_T("ModelX3"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY3);
	cXMLConfigurator.AddElem(_T("ModelY3"), strInfo);

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
	bRet = m_PatternSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
	if (!bRet)
	{
		return false;
	}

	bRet = m_PatternSearchTool2.SaveTrainModelToBinaryFile(strDir + _T("Model2.pts"));
	if (!bRet)
	{
		return false;
	}
	
	return true;
}	
	
// 更新训练控件数据信息	
bool C2PatternSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint1.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();


		m_tmpTrainGuiParam.m_TrainRect2			= m_GuiTrainRect2.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint3			= m_GuiModelPoint3.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect2			= m_GuiSearchRect2.GetRect();
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
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变
		m_GuiModelPoint1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint1.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint1, m_GuiModelPoint1.GetExtent());//尺寸不变
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);
	
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

			m_GuiFindLine[i].SetCaliperVisible(m_tmpTrainGuiParam.m_bCaliperVisible[i]);
			m_GuiFindLine[i].SetAffineRectFindLineVisible(!m_tmpTrainGuiParam.m_bCaliperVisible[i]);
			m_GuiFindLine[i].SetMaskData(m_tmpTrainGuiParam.m_vdCaliperMaskData[i]);
		}

		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变
		m_GuiModelPoint3.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint3.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint3.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint3, m_GuiModelPoint3.GetExtent());//尺寸不变
		m_GuiTrainRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetRect(m_tmpTrainGuiParam.m_TrainRect2);
		
		m_GuiSearchRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect2.SetRect(m_tmpTrainGuiParam.m_SearchRect2);
	}

	return true;
}

bool C2PatternSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
bool C2PatternSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool C2PatternSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool C2PatternSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);

	return true;
}

bool C2PatternSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		
		for (int j=0; j<LINE_NUM; j++)
		{
			CString strKey;
			strKey.Format(_T("%s_FindLineTool%d"), m_strKeyText, j);
			m_pGuiInteractiveContainer->RemoveItem(strKey);
		}

		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine1"));
		m_pGuiStaticContainer->RemoveItem(m_strKeyText +_T("_TrainResultLine2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint3"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect2"));	
	}
	
	return true;
}

bool C2PatternSearchTool::ClearResultGui()
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

	}

	return true;
}

bool C2PatternSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;
	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	m_GuiTrainRect2.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	
	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	m_GuiSearchRect2.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点1
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//参考点2
	m_GuiModelPoint1.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint1.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//参考点3
	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//参考点4
	m_GuiModelPoint3.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint3.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

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

// 获取训练模板原始图像
bool C2PatternSearchTool::GetPatternImage(cpImage &ImagePattern)
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
}	

// 获取训练模板原始图像
bool C2PatternSearchTool::GetPatternImage2(cpImage &ImagePattern)
{
	/*bool bRet = m_PatternSearchTool2.GetPatternImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}
	
	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if(cpImage::statusSuccess == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool2.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
}	

// 恢复训练参数
bool C2PatternSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool C2PatternSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void C2PatternSearchTool::SetLanguage(int nLanguage)
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
		m_GuiModelPoint.SetLabel(_T("Reference Point1-1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1-2"));
		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2-1"));
		m_GuiModelPoint3.SetLabel(_T("Reference Point2-2"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域1"));
		m_GuiSearchRect.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint.SetLabel(_T("参考点1-1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1-2"));
		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2-1"));
		m_GuiModelPoint3.SetLabel(_T("参考点2-2"));
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

CString	C2PatternSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							//区域定位1失败
				strErrInfo += _T("Pattern search 1 failed");
				break;
			case -2:							//区域定位2失败
				strErrInfo += _T("Pattern search 2 failed");
				break;
			case -3:							//找线1失败
				strErrInfo += _T("Find line1 failed");
				break;
			case -4:							//找线2失败
				strErrInfo += _T("Find line2 failed");
				break;										
			case -5:							//找线1失败，找线2失败
				strErrInfo += _T("Find line1 failed，Find line2 failed");
				break;
			case -6:							//求交点失败
				strErrInfo += _T("Find the intersection of line1 and line2 failed");
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
			case -1:							//区域定位1失败
				strErrInfo += _T("区域定位1失败");
				break;
			case -2:							//区域定位2失败
				strErrInfo += _T("区域定位2失败");
				break;
			case -3:							//找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -4:							//找线2失败
				strErrInfo += _T("找线2失败");
				break;										
			case -5:							//找线1失败，找线2失败
				strErrInfo += _T("找线1失败，找线2失败");
				break;
			case -6:							//求交点失败
				strErrInfo += _T("求交点失败");
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
			case -1:							//区域定位1失败
				strErrInfo += _T("区域定位1失败");
				break;
			case -2:							//区域定位2失败
				strErrInfo += _T("区域定位2失败");
				break;
			case -3:							//找线1失败
				strErrInfo += _T("找线1失败");
				break;
			case -4:							//找线2失败
				strErrInfo += _T("找线2失败");
				break;										
			case -5:							//找线1失败，找线2失败
				strErrInfo += _T("找线1失败，找线2失败");
				break;
			case -6:							//求交点失败
				strErrInfo += _T("求交点失败");
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