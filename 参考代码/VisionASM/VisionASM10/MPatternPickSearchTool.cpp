// PatternSearchTool.cpp: implementation of the CMPatternSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPatternPickSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPatternPickSearchTool::CMPatternPickSearchTool()
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
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 240, 40, 40);
//	m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);

	m_GuiSearch1HideRect.SetCenterWidthHeight(350, 240, 352, 180);
	m_GuiSearch1HideRect.SetLabelVisible(TRUE);
	m_GuiSearch2HideRect.SetCenterWidthHeight(390, 240, 352, 180);
	m_GuiSearch2HideRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		m_GuiHand1SearchRect.SetLabel(_T("Hand1 Train Area"));
		m_GuiHand2SearchRect.SetLabel(_T("Hand2 Train Area"));
		m_GuiSearch1HideRect.SetLabel(_T("Search1 Hide Area"));
		m_GuiSearch2HideRect.SetLabel(_T("Search2 Hide Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	}
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy				= m_PatternSearchTool.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale			= m_PatternSearchTool.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow				= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainRect				= m_GuiTrainRect.GetRect();
	m_tmpTrainGuiParam.m_Hand1SearchRect    = m_GuiHand1SearchRect.GetRect();
	m_tmpTrainGuiParam.m_Hand2SearchRect    = m_GuiHand2SearchRect.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint				= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect				= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect.GetLineColor();

	m_tmpTrainGuiParam.m_Search1HideRect					= m_GuiSearch1HideRect.GetRect();
	m_tmpTrainGuiParam.m_Search2HideRect					= m_GuiSearch2HideRect.GetRect();

	m_tmpSearchDataParam.m_nSearchNum			= 1;
	m_tmpSearchDataParam.m_dAcceptThre			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre		= 0.8;
	m_tmpSearchDataParam.m_bCoordSortEnable		= false;
	m_tmpSearchDataParam.m_nCoordSortMethod		= eCoordXOnly;
	m_tmpSearchDataParam.m_nCoordSortOrderX		= eAscending;
	m_tmpSearchDataParam.m_nCoordSortOrderY		= eAscending;;
	m_tmpSearchDataParam.m_dCoordSortGap		= 64;
	
	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CMPatternPickSearchTool::CMPatternPickSearchTool(CMPatternPickSearchTool &CMPatternPickSearchTool)
{
	if (CMPatternPickSearchTool.GetSearchToolType() != eMPatternPickSearchTool)
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
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 240, 40, 40);
	//	m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);

	m_GuiSearch1HideRect.SetCenterWidthHeight(350, 240, 352, 180);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearch1HideRect.SetLabelVisible(TRUE);

	m_GuiSearch2HideRect.SetCenterWidthHeight(390, 240, 352, 180);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearch2HideRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		m_GuiHand1SearchRect.SetLabel(_T("Hand1 Train Area"));
		m_GuiHand2SearchRect.SetLabel(_T("Hand2 Train Area"));
		m_GuiSearch1HideRect.SetLabel(_T("Search1 Hide Area"));
		m_GuiSearch2HideRect.SetLabel(_T("Search2 Hide Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= CMPatternPickSearchTool.m_CamHeight;
	m_CamWidth								= CMPatternPickSearchTool.m_CamWidth;
	m_TrainDataParam						= CMPatternPickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CMPatternPickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CMPatternPickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CMPatternPickSearchTool.m_SearchGuiParam;
	m_TrainResult							= CMPatternPickSearchTool.m_TrainResult;
	m_ImageTrain							= CMPatternPickSearchTool.m_ImageTrain;
	m_ImageMask								= CMPatternPickSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CMPatternPickSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= CMPatternPickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CMPatternPickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CMPatternPickSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CMPatternPickSearchTool.UpdateSearchGuiData(true);
	CMPatternPickSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CMPatternPickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CMPatternPickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CMPatternPickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CMPatternPickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CMPatternPickSearchTool.m_tmpImageTrain);
	SetMaskImage(CMPatternPickSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = CMPatternPickSearchTool.m_PatternSearchTool;
	}
}

CMPatternPickSearchTool& CMPatternPickSearchTool::operator=(CMPatternPickSearchTool &CMPatternPickSearchTool)
{
	if (this == &CMPatternPickSearchTool)
	{
		return *this;	
	}

	if (CMPatternPickSearchTool.GetSearchToolType() != eMPatternPickSearchTool)
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
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//	m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(350, 240, 40, 40);
	//	m_GuiModelPoint.SetLabel(_T("参考点"));
	m_GuiModelPoint.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(200, 240, 40, 40);
	//	m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);

	m_GuiSearch1HideRect.SetCenterWidthHeight(350, 240, 352, 180);
	m_GuiSearch1HideRect.SetLabelVisible(TRUE);

	m_GuiSearch2HideRect.SetCenterWidthHeight(390, 240, 352, 180);
	m_GuiSearch2HideRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		m_GuiSearchRect.SetLabel(_T("Search Area"));
		m_GuiModelPoint.SetLabel(_T("Reference Point"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		m_GuiHand1SearchRect.SetLabel(_T("Hand1 Train Area"));
		m_GuiHand2SearchRect.SetLabel(_T("Hand2 Train Area"));
		m_GuiSearch1HideRect.SetLabel(_T("Search1 Hide Area"));
		m_GuiSearch2HideRect.SetLabel(_T("Search1 Hide Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	}
	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= CMPatternPickSearchTool.m_CamHeight;
	m_CamWidth								= CMPatternPickSearchTool.m_CamWidth;
	m_TrainDataParam						= CMPatternPickSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CMPatternPickSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CMPatternPickSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CMPatternPickSearchTool.m_SearchGuiParam;
	m_TrainResult							= CMPatternPickSearchTool.m_TrainResult;

	m_ImageTrain							= CMPatternPickSearchTool.m_ImageTrain;
	m_ImageMask								= CMPatternPickSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CMPatternPickSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= CMPatternPickSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CMPatternPickSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CMPatternPickSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CMPatternPickSearchTool.UpdateSearchGuiData(true);
	CMPatternPickSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CMPatternPickSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CMPatternPickSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CMPatternPickSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CMPatternPickSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CMPatternPickSearchTool.m_tmpImageTrain);
	SetMaskImage(CMPatternPickSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_PatternSearchTool = CMPatternPickSearchTool.m_PatternSearchTool;
	}

	return *this;	
}

CMPatternPickSearchTool::~CMPatternPickSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CMPatternPickSearchTool::GetSearchToolType()
{
	return eMPatternPickSearchTool;
}										   

// 设置显示控件指针		
bool CMPatternPickSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CMPatternPickSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CMPatternPickSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CMPatternPickSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CMPatternPickSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CMPatternPickSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CMPatternPickSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CMPatternPickSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPoint2Visible);
	m_GuiSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);
	m_GuiHand1SearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiHand2SearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);

	m_GuiSearch1HideRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchHideRectVisible1);
	m_GuiSearch2HideRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchHideRectVisible2);
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiHand1SearchRect, m_strKeyText+_T("_Hand1SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiHand2SearchRect, m_strKeyText+_T("_Hand2SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint, m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect, m_strKeyText+_T("_SearchRect"));	
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearch1HideRect, m_strKeyText+_T("_Search1HideRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearch2HideRect, m_strKeyText+_T("_Search2HideRect"));

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新搜索界面显示									
bool CMPatternPickSearchTool::UpdateSearchResultGuiDisplay()							
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
	
	for (i=0; i<m_pGuiSearchCross2Array.size(); i++)
	{
		m_pGuiSearchCross2Array[i]->SetVisible(m_bSearchResultGuiShow && m_TrainGuiParam.m_bModelPoint2Visible);
	}
	for (i=0; i<m_pGuiSearchCross3Array.size(); i++)
	{
		m_pGuiSearchCross3Array[i]->SetVisible(m_bSearchResultGuiShow && m_TrainGuiParam.m_bModelPoint2Visible);
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

		for (i=0; i<m_pGuiSearchCross2Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCross2Array[i], m_pGuiSearchCross2Array[i]->GetTipText());
		}	

		for (i=0; i<m_pGuiSearchCross3Array.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCross3Array[i], m_pGuiSearchCross3Array[i]->GetTipText());
		}	

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

// 更新界面显示
bool CMPatternPickSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();
	
	return true;
}

// 设置训练Data参数	
bool CMPatternPickSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CMPatternPickSearchTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CMPatternPickSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	*((CMPatternPickSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CMPatternPickSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CMPatternPickSearchTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CMPatternPickSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CMPatternPickSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CMPatternPickSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CMPatternPickSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CMPatternPickSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CMPatternPickSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CMPatternPickSearchTool::Train()
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
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			m_tmpTrainResult.m_dModelX2 = m_tmpTrainGuiParam.m_ModelPoint2.GetX();
			m_tmpTrainResult.m_dModelY2 = m_tmpTrainGuiParam.m_ModelPoint2.GetY();

			m_ImageTrain     = m_tmpImageTrain;
			m_ImageMask      = m_tmpImageMask;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		return bRet;
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
			sc2Vector cModelOrigin;
			m_PatternSearchTool.GetPatternOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			m_tmpTrainResult.m_dModelX2 = m_tmpTrainGuiParam.m_ModelPoint2.GetX();
			m_tmpTrainResult.m_dModelY2 = m_tmpTrainGuiParam.m_ModelPoint2.GetY();
			
			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位失败
			return false;
		}

		return bRet;
	}

	return false;
}

// 获取训练结果图像														
bool CMPatternPickSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
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
bool CMPatternPickSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	*((CMPatternPickSearchTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}							

// 是否训练成功
bool CMPatternPickSearchTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CMPatternPickSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CMPatternPickSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CMPatternPickSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}

	*((CMPatternPickSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CMPatternPickSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CMPatternPickSearchSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CMPatternPickSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eMPatternPickSearchTool)
	{
		return false;
	}
	
	UpdateSearchGuiData(true);

	*((CMPatternPickSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CMPatternPickSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		return false;
	}	

	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 三钻 设置重叠阈值0.1
	//m_PatternSearchTool.SetOverlapThreshold(0.1);

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
		// 按坐标排序
		if (m_tmpSearchDataParam.m_bCoordSortEnable)
		{
			m_PatternSearchTool.SortByCoordXY(m_tmpSearchDataParam.m_dCoordSortGap,	m_tmpSearchDataParam.m_nCoordSortMethod,
				m_tmpSearchDataParam.m_nCoordSortOrderX,m_tmpSearchDataParam.m_nCoordSortOrderY);
		}

		// 获取结果
		int nResultNum = m_PatternSearchTool.GetResultNum();
		if (nResultNum <= 0)
		{
			m_nResultErrNumber = -1;
			return false;
		}

		CSearchResult searchResultHand1;
		CSearchResult searchResultHand2;
		bool hand1flag=false;//吸头1有值
		bool hand2flag=false;//吸头2有值

		for (int j=0, i=0; i<nResultNum && i<MAXRESNUM; i++)
		{
			scPatternSearchRotResult tmpPatternResult = m_PatternSearchTool.GetResult(i);
			
			CSearchResult searchResult;
			searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
			searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
			searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
			searchResult.m_dScore = tmpPatternResult.GetScore();

			//计算第二参考点坐标
			sc2Vector pos = tmpPatternResult.GetPosition();
			sc2Vector tmModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_PatternSearchTool.GetPatternOrigin(tmModelPoint);
			dRotation = scRadian(tmpPatternResult.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmModelPoint.GetX()  - sin(dRotation) * tmModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmModelPoint.GetX()  + cos(dRotation) * tmModelPoint.GetY());
			double dTransModelX, dTransModelY;
			dTransModelX = cos(dRotation) * m_TrainResult.m_dModelX2  - sin(dRotation) * m_TrainResult.m_dModelY2 + dTransX;
			dTransModelY = sin(dRotation) * m_TrainResult.m_dModelX2  + cos(dRotation) * m_TrainResult.m_dModelY2 + dTransY;

			sc2Vector center= tmpPatternResult.MatchRegion().GetCenter();
			
			CString	strTmpKey;
			strTmpKey.Format(_T("%s_ResultCross3%d"), m_strKeyText, i);
			scGuiCross *pTempCross = m_GuiSearchCross3+i;
			pTempCross->SetCenterExtent(center, sc2Vector(60,60));
			pTempCross->SetLineColor(RGB(255,0,0));
			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempCross->SetTipTextEnabled(true);
			pTempCross->SetTipText(strTmpKey);

			CString st;
			st.Format(_T("%d  S:%.2f"),i,tmpPatternResult.GetScore());
			pTempCross->SetLabel(st);
			pTempCross->SetLabelVisible(TRUE);
			m_pGuiSearchCross3Array.push_back(pTempCross);

			//如果两个地方都有数值
			//if(j>1)break;//两个手爪都有数据
			//吸头1
			if(PointInRect(center,m_tmpTrainGuiParam.m_Hand1SearchRect)&&!hand1flag)
			{
				bool bflag = true;

				if (m_tmpTrainGuiParam.m_bSearchHideRectVisible1)
				{
					scAffineRect matchRect =  tmpPatternResult.MatchRegion();
					sc2Vector pos = matchRect.GetCenter();
					//scRect tempHideRect =  m_tmpTrainGuiParam.m_SearchHideRect;
					sc2Vector UpLeftPos = m_tmpTrainGuiParam.m_Search1HideRect.GetUL();   //左上角
					sc2Vector DownRightPos = m_tmpTrainGuiParam.m_Search1HideRect.GetLR();//右下角

					if( (UpLeftPos.GetX() < pos.GetX()) && (pos.GetX() < DownRightPos.GetX()) &&
						(UpLeftPos.GetY() < pos.GetY()) && (pos.GetY() < DownRightPos.GetY()) )
					{
						bflag = false;
					}
				}
				
				if (bflag)
				{
					hand1flag=true;
					searchResultHand1.m_bResultPickIndex=1;

					searchResultHand1.m_dPosX = tmpPatternResult.GetPosition().GetX();
					searchResultHand1.m_dPosY = tmpPatternResult.GetPosition().GetY();
					searchResultHand1.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
					searchResultHand1.m_dScore = tmpPatternResult.GetScore();


					searchResultHand1.m_vdAuxiliaryPosX.at(0) = dTransModelX;
					searchResultHand1.m_vdAuxiliaryPosY.at(0) = dTransModelY;
					searchResultHand1.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;


					// 设置粗定位矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = tmpPatternResult.MatchRegion();
					searchResultHand1.m_vTrainSearchRect.push_back(resRect);

					CString	strTmpKey;
					strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
					//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
					scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+j;
					pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
					pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect->SetTipTextEnabled(true);
					pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchRectArray.push_back(pTempAffineRect);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+j;
					pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("hand1 p0   X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore());
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);

					strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
					//scGuiCross *pTempCross2 = new scGuiCross;
					scGuiCross *pTempCross2 = m_GuiSearchCross2+j;
					pTempCross2->SetCenterExtent(sc2Vector(searchResultHand1.m_vdAuxiliaryPosX.at(0), searchResultHand1.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
					pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross2->SetTipTextEnabled(true);
					pTempCross2->SetTipText(strTmpKey);
					st.Format(_T("hand1 p1     X:%.1f;Y:%.1f"), searchResultHand1.m_vdAuxiliaryPosX.at(0), searchResultHand1.m_vdAuxiliaryPosY.at(0));
					pTempCross2->SetLabel(st);
					pTempCross2->SetLabelVisible(TRUE);
					m_pGuiSearchCross2Array.push_back(pTempCross2);
					//m_SearchResultArray.push_back(searchResultHand1);
					j++;
					continue;;
				}
			}

			//吸头2
			if(PointInRect(center,m_tmpTrainGuiParam.m_Hand2SearchRect)&&!hand2flag)
			{
				bool bflag = true;

				if (m_tmpTrainGuiParam.m_bSearchHideRectVisible2)
				{
					scAffineRect matchRect =  tmpPatternResult.MatchRegion();
					sc2Vector pos = matchRect.GetCenter();
					//scRect tempHideRect =  m_tmpTrainGuiParam.m_SearchHideRect;
					sc2Vector UpLeftPos = m_tmpTrainGuiParam.m_Search2HideRect.GetUL();   //左上角
					sc2Vector DownRightPos = m_tmpTrainGuiParam.m_Search2HideRect.GetLR();//右下角

					if( (UpLeftPos.GetX() < pos.GetX()) && (pos.GetX() < DownRightPos.GetX()) &&
						(UpLeftPos.GetY() < pos.GetY()) && (pos.GetY() < DownRightPos.GetY()) )
					{
						bflag = false;
					}
				} 

				if (bflag)
				{
					hand2flag=true;
					searchResultHand2.m_bResultPickIndex=2;

					searchResultHand2.m_dPosX = tmpPatternResult.GetPosition().GetX();
					searchResultHand2.m_dPosY = tmpPatternResult.GetPosition().GetY();
					searchResultHand2.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
					searchResultHand2.m_dScore = tmpPatternResult.GetScore();

					searchResultHand2.m_vdAuxiliaryPosX.at(0) = dTransModelX;
					searchResultHand2.m_vdAuxiliaryPosY.at(0) = dTransModelY;
					searchResultHand2.m_vdAuxiliaryAngle.at(0) = searchResult.m_dAngle;


					// 设置粗定位矩形
					CResultSearchRect resRect;
					resRect.m_searchRect = tmpPatternResult.MatchRegion();
					searchResultHand2.m_vTrainSearchRect.push_back(resRect);



					CString	strTmpKey;
					strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
					//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
					scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+j;
					pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
					pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect->SetTipTextEnabled(true);
					pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchRectArray.push_back(pTempAffineRect);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+j;
					pTempCross->SetCenterExtent(tmpPatternResult.GetPosition(), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("hand2  p0  X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore());
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);

					strTmpKey.Format(_T("%s_ResultCross2%d"), m_strKeyText, i);
					//scGuiCross *pTempCross2 = new scGuiCross;
					scGuiCross *pTempCross2 = m_GuiSearchCross2+j;
					pTempCross2->SetCenterExtent(sc2Vector(searchResultHand2.m_vdAuxiliaryPosX.at(0), searchResultHand2.m_vdAuxiliaryPosY.at(0)), sc2Vector(20,20));
					pTempCross2->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross2->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross2->SetTipTextEnabled(true);
					pTempCross2->SetTipText(strTmpKey);
					st.Format(_T("hand2  p1   X:%.1f;Y:%.1f"), searchResultHand2.m_vdAuxiliaryPosX.at(0), searchResultHand2.m_vdAuxiliaryPosY.at(0));
					pTempCross2->SetLabel(st);
					pTempCross2->SetLabelVisible(TRUE);
					m_pGuiSearchCross2Array.push_back(pTempCross2);
					j++;
					//m_SearchResultArray.push_back(searchResultHand2);
					continue;;
				}
			}
		}	

		if(searchResultHand1.m_bResultPickIndex>0)m_SearchResultArray.push_back(searchResultHand1);
		if(searchResultHand2.m_bResultPickIndex>0)m_SearchResultArray.push_back(searchResultHand2);

		if(m_SearchResultArray.size()==0)
		{
			m_nResultErrNumber=-3;
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

	return bRet;
}

// 获取搜索结果个数										
int CMPatternPickSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CMPatternPickSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像
bool CMPatternPickSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eMPatternPickSearchTool)
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
bool CMPatternPickSearchTool::LoadModelFromFile(CString strDir)
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
	CMPatternPickSearchTrainDataParam tmpTrainDataParam;
	CMPatternPickSearchTrainGuiParam tmpTrainGuiParam;
	CMPatternPickSearchSearchDataParam tmpSearchDataParam;
	CMPatternPickSearchSearchGuiParam tmpSearchGuiParam;	
	CMPatternPickSearchTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eMPatternPickSearchTool)
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

	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchHideRectVisible1"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bSearchHideRectVisible1 = _ttoi(cXMLConfigurator.GetElemData());
	}
	// search Hide rect
	bRet = cXMLConfigurator.FindElem(_T("Search1HideRect"));
	/*if (!bRet)
	{
		return false;
	}*/
	if(bRet)
	{
		cXMLConfigurator.EnterElem();
		strInfo.Format(_T("%f"), tmpTrainGuiParam.m_Search1HideRect.GetOrigin().GetX());
		bRet = cXMLConfigurator.FindElem(_T("OriginX"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("OriginY"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_Search1HideRect.SetOrigion(vPos);
		bRet = cXMLConfigurator.FindElem(_T("SizeX"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("SizeY"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_Search1HideRect.SetSize(vPos);
		cXMLConfigurator.LeaveElem();
	}

	// search rect2
	bRet = cXMLConfigurator.FindElem(_T("SearchHideRectVisible2"));
	if (bRet)
	{
		tmpTrainGuiParam.m_bSearchHideRectVisible2 = _ttoi(cXMLConfigurator.GetElemData());
	}
	// search Hide rect
	bRet = cXMLConfigurator.FindElem(_T("Search2HideRect"));
	/*if (!bRet)
	{
		return false;
	}*/
	if(bRet)
	{
		cXMLConfigurator.EnterElem();
		strInfo.Format(_T("%f"), tmpTrainGuiParam.m_Search2HideRect.GetOrigin().GetX());
		bRet = cXMLConfigurator.FindElem(_T("OriginX"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("OriginY"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_Search2HideRect.SetOrigion(vPos);
		bRet = cXMLConfigurator.FindElem(_T("SizeX"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		bRet = cXMLConfigurator.FindElem(_T("SizeY"));
		//if (!bRet)
		//{
		//	return false;
		//}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_Search2HideRect.SetSize(vPos);
		cXMLConfigurator.LeaveElem();
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

	// hand1 rect
	bRet = cXMLConfigurator.FindElem(_T("Hand1Rect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_Hand1SearchRect.GetOrigin().GetX());
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
	tmpTrainGuiParam.m_Hand1SearchRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_Hand1SearchRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

	// hand2 rect
	bRet = cXMLConfigurator.FindElem(_T("Hand2Rect"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_Hand2SearchRect.GetOrigin().GetX());
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
	tmpTrainGuiParam.m_Hand2SearchRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_Hand2SearchRect.SetSize(vPos);
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
	// model point 2
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

	// ModelPoint2Visible
	bRet = cXMLConfigurator.FindElem(_T("ModelPoint2Visible"));
	if (!bRet)
	{
		tmpTrainGuiParam.m_bModelPoint2Visible = false;
	}
	tmpTrainGuiParam.m_bModelPoint2Visible = _ttoi(cXMLConfigurator.GetElemData());

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


	bRet = cXMLConfigurator.FindElem(_T("CoordSortEnable"));
	if (!bRet)
	{
		tmpSearchDataParam.m_bCoordSortEnable = false;
	}
	else
	{
		tmpSearchDataParam.m_bCoordSortEnable = _ttoi(cXMLConfigurator.GetElemData());;
	}

	bRet = cXMLConfigurator.FindElem(_T("CoordSortMethod"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortMethod = eCoordXOnly;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortMethod = (CoordSortMethod)_ttoi(cXMLConfigurator.GetElemData());
	}


	bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderX"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortOrderX = eAscending;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortOrderX = (SortOrder)_ttoi(cXMLConfigurator.GetElemData());
	}
	

	bRet = cXMLConfigurator.FindElem(_T("CoordSortOrderY"));
	if (!bRet)
	{
		tmpSearchDataParam.m_nCoordSortOrderY = eAscending;
	}
	else
	{
		tmpSearchDataParam.m_nCoordSortOrderY = (SortOrder)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("CoordSortGap"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dCoordSortGap = 64;
	}
	else
	{
		tmpSearchDataParam.m_dCoordSortGap =  _tcstod(cXMLConfigurator.GetElemData(), NULL);
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
bool CMPatternPickSearchTool::SaveModelToFile(CString strDir)
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

//  //将TrainGuiParam的参数，放在TrainDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	//////////////TrainGuiParam//////////////
// 	cXMLConfigurator.AddElem(_T("TrainGuiParam"));
// 	cXMLConfigurator.EnterElem();

	// search hide rect visible1 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchHideRectVisible1);
	cXMLConfigurator.AddElem(_T("SearchHideRectVisible1"), strInfo);

	// search rect
	cXMLConfigurator.AddElem(_T("Search1HideRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search1HideRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search1HideRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search1HideRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search1HideRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();

	// search hide rect visible2 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchHideRectVisible2);
	cXMLConfigurator.AddElem(_T("SearchHideRectVisible2"), strInfo);

	// search rect
	cXMLConfigurator.AddElem(_T("Search2HideRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search2HideRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search2HideRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search2HideRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Search2HideRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();

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
	// hand1 rect
	cXMLConfigurator.AddElem(_T("Hand1Rect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand1SearchRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand1SearchRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand1SearchRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand1SearchRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();

	// hand2 rect
	cXMLConfigurator.AddElem(_T("Hand2Rect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand2SearchRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand2SearchRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand2SearchRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_Hand2SearchRect.GetSize().GetY());
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
	// model point 2
	cXMLConfigurator.AddElem(_T("ModelPoint2"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint2.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
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

	//ModelPoint2Visible 
	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPoint2Visible);
	cXMLConfigurator.AddElem(_T("ModelPoint2Visible"), strInfo);

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
	strInfo.Format(_T("%d"), m_SearchDataParam.m_bCoordSortEnable);
	cXMLConfigurator.AddElem(_T("CoordSortEnable"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortMethod);
	cXMLConfigurator.AddElem(_T("CoordSortMethod"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderX);
	cXMLConfigurator.AddElem(_T("CoordSortOrderX"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nCoordSortOrderY);
	cXMLConfigurator.AddElem(_T("CoordSortOrderY"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dCoordSortGap);
	cXMLConfigurator.AddElem(_T("CoordSortGap"), strInfo);


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
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
	cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
	cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);
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
bool CMPatternPickSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_TrainRect			= m_GuiTrainRect.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_ModelPoint2		= m_GuiModelPoint2.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect			= m_GuiSearchRect.GetRect();
		m_tmpTrainGuiParam.m_Hand1SearchRect	= m_GuiHand1SearchRect.GetRect();
		m_tmpTrainGuiParam.m_Hand2SearchRect	= m_GuiHand2SearchRect.GetRect();
		m_tmpTrainGuiParam.m_Search1HideRect			= m_GuiSearch1HideRect.GetRect();
		m_tmpTrainGuiParam.m_Search2HideRect			= m_GuiSearch2HideRect.GetRect();
	}
	else // 将数据传递到Gui控件上
	{	
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变
		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());//尺寸不变
		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);
	
		m_GuiSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect.SetRect(m_tmpTrainGuiParam.m_SearchRect);

		m_GuiHand1SearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiHand1SearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiHand1SearchRect.SetRect(m_tmpTrainGuiParam.m_Hand1SearchRect);

		m_GuiHand2SearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiHand2SearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiHand2SearchRect.SetRect(m_tmpTrainGuiParam.m_Hand2SearchRect);

		m_GuiSearch1HideRect.SetLineColor(m_tmpTrainGuiParam.m_HideRectColor);
		m_GuiSearch1HideRect.SetRect(m_tmpTrainGuiParam.m_Search1HideRect);

		m_GuiSearch2HideRect.SetLineColor(m_tmpTrainGuiParam.m_HideRectColor);
		m_GuiSearch2HideRect.SetRect(m_tmpTrainGuiParam.m_Search2HideRect);
	}

	return true;
}

bool CMPatternPickSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

		for (j=0; j<m_pGuiSearchCross2Array.size(); j++)
		{
			m_pGuiSearchCross2Array[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCross2Array[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCross3Array.size(); j++)
		{
			m_pGuiSearchCross3Array[j]->SetLineColor(RGB(255,0,0));
			m_pGuiSearchCross3Array[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}
	}
	
	return true;
}

// 清除控件及其显示					
bool CMPatternPickSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CMPatternPickSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CMPatternPickSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchCross2Array);
	ClearGraphicsArray(m_pGuiSearchCross3Array);

	return true;
}

bool CMPatternPickSearchTool::SetGuiByCam(double nWidth,double nHeight)
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

	m_GuiHand1SearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiHand1SearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiHand2SearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiHand2SearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
	//参考点2
	m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);

	m_GuiSearch1HideRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearch1HideRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiSearch2HideRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearch2HideRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CMPatternPickSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect"));	
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Hand1SearchRect"));		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Hand2SearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Search1HideRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Search2HideRect"));
	}
	
	return true;
}

bool CMPatternPickSearchTool::ClearResultGui()
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

		for (j=0; j<m_pGuiSearchCross2Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCross2Array[j]->GetTipText());
		}
		for (j=0; j<m_pGuiSearchCross3Array.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCross3Array[j]->GetTipText());
		}
	}

	return true;
}

// 获取训练模板原始图像
bool CMPatternPickSearchTool::GetPatternImage(cpImage &ImagePattern)
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
bool CMPatternPickSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CMPatternPickSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CMPatternPickSearchTool::SetLanguage(int nLanguage)
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
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		m_GuiHand1SearchRect.SetLabel(_T("Hand1 Train Area"));
		m_GuiHand2SearchRect.SetLabel(_T("Hand2 Train Area"));
		m_GuiSearch1HideRect.SetLabel(_T("Search1 Hide Area"));
		m_GuiSearch2HideRect.SetLabel(_T("Search2 Hide Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		m_GuiSearchRect.SetLabel(_T("搜索区域"));
		m_GuiModelPoint.SetLabel(_T("参考点"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		m_GuiSearch1HideRect.SetLabel(_T("手爪1 屏蔽区域"));
		m_GuiSearch2HideRect.SetLabel(_T("手爪2 屏蔽区域"));
		break;
	}


}
CString	CMPatternPickSearchTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());
	
	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -1:							//区域定位失败
				strErrInfo += _T("Pattern search failed");
				break;
			case -3:							//找线1失败
				strErrInfo += _T("There are no products in the hand area");
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
			case -1:							//区域定位失败
				strErrInfo += _T("区域定位失败");
				break;
			case -3:							//
				strErrInfo += _T("手爪区域1和2中没有产品");
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
			case -1:							//区域定位失败
				strErrInfo += _T("区域定位失败");
				break;
			case -3:							//
				strErrInfo += _T("手爪区域1和2中没有产品");
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

BOOL	CMPatternPickSearchTool::PointInRect(sc2Vector point,scRect rect)
{
	if(point.GetX()>rect.GetUL().GetX()//左上
		&&point.GetX()<rect.GetUR().GetX()//右上
		&&point.GetY()>rect.GetUL().GetY()//左上
		&&point.GetY()<rect.GetLL().GetY()//左下
		)	
		return TRUE;
	return FALSE;
}