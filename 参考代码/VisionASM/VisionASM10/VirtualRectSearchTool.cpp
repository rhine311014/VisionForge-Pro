// RectSearchTool.cpp: implementation of the CVirtualRectSearchTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirtualRectSearchTool.h"
#include "svIntersect.h"
#include "svGuiLine.h"
#include "svGuiCross.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualRectSearchTool::CVirtualRectSearchTool()
{
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(300, 240, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(400, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
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

	m_tmpTrainDataParam.m_dGrau							= m_QuickSearchTool.GetGranularity();
	m_tmpTrainDataParam.m_lNoiseThre					= m_QuickSearchTool.GetNoiseThre();
	m_tmpTrainDataParam.m_lTotalMag						= m_QuickSearchTool.GetTotalGradMagThre();
	m_tmpTrainDataParam.m_bMaskEnable					= false;
	m_tmpTrainGuiParam.m_TrainAffineRect				= m_GuiTrainRect.GetAffineRect();
	m_tmpTrainGuiParam.m_ModelPoint						= m_GuiModelPoint.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect						= m_GuiSearchRect.GetRect();
	m_tmpTrainGuiParam.m_nLineWidth						= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor						= m_GuiTrainRect.GetLineColor();

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
		m_tmpTrainDataParam.m_bEnableDualEdge[i]			= false;
	}

	m_tmpSearchDataParam.m_dAcceptThre					= m_QuickSearchTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow					= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh					= 5.0;
	m_tmpSearchDataParam.m_nSearchNum					= 1;
	m_tmpSearchGuiParam.m_nLineWidth					= m_GuiSearchRect.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor						= m_GuiSearchRect.GetLineColor();

	m_TrainDataParam									= m_tmpTrainDataParam;
	m_TrainGuiParam										= m_tmpTrainGuiParam;
	m_SearchDataParam									= m_tmpSearchDataParam;
	m_SearchGuiParam									= m_tmpSearchGuiParam;
}

CVirtualRectSearchTool::CVirtualRectSearchTool(CVirtualRectSearchTool &CVirtualRectSearchTool)
{
	if (CVirtualRectSearchTool.GetSearchToolType() != eVirtualRectSearchTool)
	{
		return;
	}

	// 清空
	// 	ClearAllGui();
	// 	ClearResult();

	// 初始化
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(300, 240, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(400, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
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
	m_CamHeight								= CVirtualRectSearchTool.m_CamHeight;
	m_CamWidth								= CVirtualRectSearchTool.m_CamWidth;
	m_TrainDataParam						= CVirtualRectSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CVirtualRectSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CVirtualRectSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CVirtualRectSearchTool.m_SearchGuiParam;
	m_TrainResult							= CVirtualRectSearchTool.m_TrainResult;
	m_ImageTrain							= CVirtualRectSearchTool.m_ImageTrain;
	m_ImageMask								= CVirtualRectSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CVirtualRectSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CVirtualRectSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CVirtualRectSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CVirtualRectSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CVirtualRectSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CVirtualRectSearchTool.UpdateSearchGuiData(true);
	CVirtualRectSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CVirtualRectSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CVirtualRectSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CVirtualRectSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CVirtualRectSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CVirtualRectSearchTool.m_tmpImageTrain);
	SetMaskImage(CVirtualRectSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;

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

		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
	}		
}

CVirtualRectSearchTool& CVirtualRectSearchTool::operator=(CVirtualRectSearchTool &CVirtualRectSearchTool)
{
	if (this == &CVirtualRectSearchTool)
	{
		return *this;
	}

	if (CVirtualRectSearchTool.GetSearchToolType() != eVirtualRectSearchTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	// 初始化
	m_pGuiDisplay										= NULL;
	m_hDispWnd											= NULL;
	m_pGuiInteractiveContainer							= NULL;
	m_pGuiStaticContainer								= NULL;
	m_strKeyText										= _T("");
	m_bTrainOK											= FALSE;
	m_bTrainGuiShow										= true;
	m_bTrainResultGuiShow								= false;
	m_bSearchResultGuiShow								= false;
	m_GuiTrainRect.SetCenterLengthsRotationSkew(300, 240, 50, 50, 0, 0);
	//m_GuiTrainRect.SetLabel(_T("训练区域"));
	m_GuiTrainRect.SetLabelVisible(TRUE);
	m_GuiSearchRect.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect.SetLabel(_T("搜索区域"));
	m_GuiSearchRect.SetLabelVisible(TRUE);
	m_GuiModelPoint.SetCenterXYWidthHeight(400, 240, 40, 40);
	//m_GuiModelPoint.SetLabel(_T("参考点"));
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
	m_CamHeight								= CVirtualRectSearchTool.m_CamHeight;
	m_CamWidth								= CVirtualRectSearchTool.m_CamWidth;

	m_TrainDataParam						= CVirtualRectSearchTool.m_TrainDataParam;
	m_TrainGuiParam							= CVirtualRectSearchTool.m_TrainGuiParam;
	m_SearchDataParam						= CVirtualRectSearchTool.m_SearchDataParam;
	m_SearchGuiParam						= CVirtualRectSearchTool.m_SearchGuiParam;
	m_TrainResult							= CVirtualRectSearchTool.m_TrainResult;
	m_ImageTrain							= CVirtualRectSearchTool.m_ImageTrain;
	m_ImageMask								= CVirtualRectSearchTool.m_ImageMask;
	m_bTrainGuiShow							= CVirtualRectSearchTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= CVirtualRectSearchTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= CVirtualRectSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK								= CVirtualRectSearchTool.m_bTrainOK;
	m_pGuiDisplay							= CVirtualRectSearchTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CVirtualRectSearchTool.UpdateSearchGuiData(true);
	CVirtualRectSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CVirtualRectSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CVirtualRectSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CVirtualRectSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CVirtualRectSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CVirtualRectSearchTool.m_tmpImageTrain);
	SetMaskImage(CVirtualRectSearchTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;

		// 设置训练参数
		if (!m_QuickSearchTool.SetGranularity(m_TrainDataParam.m_dGrau))
		{
			return *this;
		}

		if (!m_QuickSearchTool.SetNoiseThre(m_TrainDataParam.m_lNoiseThre))
		{
			return *this;
		}

		if (!m_QuickSearchTool.SetTotalGradMagThre(m_TrainDataParam.m_lTotalMag))
		{
			return *this;
		}

		// 设置掩模图像
		if (m_TrainDataParam.m_bMaskEnable)
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_ImageMask, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
		else
		{
			bool bRet = m_QuickSearchTool.Train(m_ImageTrain, m_TrainGuiParam.m_TrainAffineRect, m_TrainGuiParam.m_ModelPoint.GetX(),
				m_TrainGuiParam.m_ModelPoint.GetY());
			m_bTrainOK = bRet;
		}
	}	

	return *this;
}

CVirtualRectSearchTool::~CVirtualRectSearchTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CVirtualRectSearchTool::GetSearchToolType()
{
	return eVirtualRectSearchTool;
}										   

// 设置显示控件指针		
bool CVirtualRectSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CVirtualRectSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}


bool CVirtualRectSearchTool::GetFindLineCenterPoint(scFindLine* m_FindLineTool,sc2Vector &pointCenter2)
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

// 设置Gui图形的关键字														
bool CVirtualRectSearchTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CVirtualRectSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CVirtualRectSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CVirtualRectSearchTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}	

// 更新搜索界面显示									
bool CVirtualRectSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CVirtualRectSearchTool::UpdateTrainGuiDisplay()
{
	// 更新
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainAffineRectVisible);
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

// 更新训练结果Gui控件显示
bool CVirtualRectSearchTool::UpdateTrainResultGuiDisplay()
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

// 更新搜索Gui控件显示														
bool CVirtualRectSearchTool::UpdateSearchResultGuiDisplay()
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

			for (i=0; i<m_pGuiSearchAffineRectArray.size(); i++)
			{
				m_pGuiStaticContainer->AddItem(m_pGuiSearchAffineRectArray[i], m_pGuiSearchAffineRectArray[i]->GetTipText());
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
bool CVirtualRectSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	//	UpdateTrainResultGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CVirtualRectSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CVirtualRectSearchTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CVirtualRectSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	*((CVirtualRectSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CVirtualRectSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CVirtualRectSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CVirtualRectSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CVirtualRectSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CVirtualRectSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CVirtualRectSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CVirtualRectSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CVirtualRectSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CVirtualRectSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

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
		//bool bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
		//	m_tmpTrainGuiParam.m_ModelPoint.GetY());
		bool bRet = false;
		if (bIsProcessImage)
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
		bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[2].Execute(m_tmpImageTrain, cLineSeg3, dRMS3);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[3].Execute(m_tmpImageTrain, cLineSeg4, dRMS4);
		if (!bRet)
		{
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
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
		//bool bRet = m_QuickSearchTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
		//	m_tmpTrainGuiParam.m_ModelPoint.GetY());
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
		bRet = m_FindLineTool[0].Execute(m_tmpImageTrain, cLineSeg1, dRMS1);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[1].Execute(m_tmpImageTrain, cLineSeg2, dRMS2);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[2].Execute(m_tmpImageTrain, cLineSeg3, dRMS3);
		if (!bRet)
		{
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
		bRet = m_FindLineTool[3].Execute(m_tmpImageTrain, cLineSeg4, dRMS4);
		if (!bRet)
		{
			return false;
		}

		// 求交点
		bool bInterSect = false;
		std::vector<sc2Vector> vectArray;
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg1.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg3.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
			return false;
		}
		bInterSect = false;
		vectArray.clear();
		bRet = sfInterSectLineLine(cLineSeg2.GetLine(), cLineSeg4.GetLine(), bInterSect, vectArray);
		if (!bRet || !bInterSect || vectArray.size() <= 0)
		{
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
bool CVirtualRectSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_QuickSearchTool.GetTrainResultImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CVirtualRectSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	*((CVirtualRectSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CVirtualRectSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CVirtualRectSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CVirtualRectSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CVirtualRectSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	*((CVirtualRectSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CVirtualRectSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CVirtualRectSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CVirtualRectSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eVirtualRectSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CVirtualRectSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CVirtualRectSearchTool::Search(cpImage &ImageSearch)
{
	m_nResultErrNumber = 0;//重置错误码
	if (!m_bTrainOK)
	{
		return false;
	}	

	// 清空
	ClearResult();

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
	if (!m_QuickSearchTool.SetNumToFind(m_tmpSearchDataParam.m_nSearchNum))
	{
		return false;
	}

	// 设置接收阈值
	if (!m_QuickSearchTool.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre))
	{
		return false;
	}

	// 设置搜索角度范围
	if (!m_QuickSearchTool.SetDofZoneRotate(scDegree(m_tmpSearchDataParam.m_dAngleLow), scDegree(m_tmpSearchDataParam.m_dAngleHigh), scDegree(360)))
	{
		return false;
	}

	// 设置缩放比例
	if (!m_QuickSearchTool.SetDofNominalSize(1.0))
	{
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

	// 执行搜索
	//bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
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
	/*bool bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);*/
	bool bRet = false;
	if (bIsProcessImage)
	{
		bRet = m_QuickSearchTool.Execute(tmpImage, m_TrainGuiParam.m_SearchRect);		
	}
	else
	{
		bRet = m_QuickSearchTool.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect);
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
			pTempAffineRect->SetAffineRect(tmpQuickResult.MatchRegion());
			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempAffineRect->SetTipTextEnabled(true);
			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
			m_pGuiSearchAffineRectArray.push_back(pTempAffineRect);


			std::vector<sc2Vector> points1;
			std::vector<sc2Vector> points2;
			std::vector<sc2Vector> points3;
			std::vector<sc2Vector> points4; 

			sc2Vector  pointLine1;
			sc2Vector  pointLine2;

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

				// 执行找线
				bFindLine[j] = m_FindLineTool[j].Execute(ImageSearch, ResLine[j], dRMS[j]); // 成功

				// 找线结果显示
				if (bFindLine[j])
				{
// 					strTmpKey.Format(_T("%s_ResultLine%d_%d"), m_strKeyText, j, i);
// 					//scGuiLine *pTemLine = new scGuiLine;
// 					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+j;
// 					pTemLine->SetLine(ResLine[j].GetLine());
// 					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 					pTemLine->SetTipTextEnabled(true);
// 					pTemLine->SetTipText(strTmpKey);
// 					m_pGuiSearchLineArray.push_back(pTemLine);

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
										points1.push_back(points[m]);
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
										points2.push_back(points[m]);
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
										points3.push_back(points[m]);
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
										points4.push_back(points[m]);
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

			// 所做辅助线
			scLine line1,line2,line3,line4,line5,line6;
			scRadian angle1,angle2,angle3,angle4;

			// 找线工具中点
			sc2Vector pointCenter1;
			sc2Vector pointCenter2;
			sc2Vector pointCenter3;
			sc2Vector pointCenter4;
			GetFindLineCenterPoint(&m_FindLineTool[0],pointCenter1);
			GetFindLineCenterPoint(&m_FindLineTool[1],pointCenter2);
			GetFindLineCenterPoint(&m_FindLineTool[2],pointCenter3);
			GetFindLineCenterPoint(&m_FindLineTool[3],pointCenter4);
			//修正角度计算
			angle1 =(ResLine[0].GetRotation().SignedNormMod180()+
				ResLine[1].GetRotation().SignedNormMod180())/2;
			angle2 = (angle1 +scRadian(skPI_2)).SignedNormMod180();
			angle3 =(ResLine[2].GetRotation().NormMod180()+
				ResLine[3].GetRotation().NormMod180())/2;
			angle4 = (angle3+scRadian(skPI_2)).SignedNormMod180();

			//上下是直边儿时，所做辅助线
			line5.SetPosRotation(pointCenter1,angle2);
			//左右是直边儿时，所做辅助线
			line6.SetPosRotation(pointCenter3,angle4);


			// 点到直线距离的记录
			double m;
			std::vector<double> max1;
			std::vector<double> max2;
			std::vector<double> max3;
			std::vector<double> max4;


			//	对修正模式进行选择
			// 			switch (m_tmpSearchDataParam.m_nRiviseMode)
			// 			{
			// 			case 0:
			// 				m_tmpSearchDataParam.m_nRiviseMode = 0;
			// 				break;
			// 			case 1:
			// 				m_tmpSearchDataParam.m_nRiviseMode = 1;
			// 				break;
			// 			case 2:
			// 				m_tmpSearchDataParam.m_nRiviseMode = 2;
			// 				break;
			// 			case 3:
			// 				m_tmpSearchDataParam.m_nRiviseMode = 3;
			// 				break;
			// 			default:
			// 				m_tmpSearchDataParam.m_nRiviseMode = 1;
			// 				break;
			// 			}
			if (m_tmpSearchDataParam.m_nRiviseMode==0)
			{
				if (((ResLine[0].GetRotation().SignedNormMod180()-ResLine[1].GetRotation().SignedNormMod180())>scRadian(skPI/180) )||
					((ResLine[0].GetRotation().SignedNormMod180()-ResLine[1].GetRotation().SignedNormMod180())<scRadian(-skPI/180)))
				{
					m_nResultErrNumber = -6;
					return false;
				}
				//找线3拥有最大距离点points3[tmax3]
				if( (points3.size()<=0)||(points4.size()<=0))
				{
					return false;
				}
				int tmax3 = 0;
				for (size_t l=0;l < points3.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points3[l], line5, m, pointLine1);
					max3.push_back(m);
				}
				if (max3.size()<= 0)
				{
					return false;
				}
				m = max3[0];
				for (size_t l=0;l < points3.size() ;l++)
				{
					if (max3[l] > m)
					{
						m = max3[l];
						tmax3 = l;
					}
				}
				//找线4拥有最大距离点points4[tmax4]
				int tmax4 = 0;
				for (size_t l=0;l < points4.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points4[l], line5, m, pointLine1);
					max4.push_back(m);
				}
				if (max4.size()<= 0)
				{
					return false;
				}
				m = max4[0];
				for (size_t l=0;l < points4.size() ;l++)
				{
					if (max4[l] > m)
					{
						m = max4[l];
						tmax4 = l;
					}
				}

				//做线3的修正线
				line3.SetPosRotation(points3[tmax3],angle2);
				//做线4的修正线
				line4.SetPosRotation(points4[tmax4],angle2);
				// 求交点左上
				if (bFindLine[0] && bFindLine[2])
				{
					int nIntersectIndex = 0;
					std::vector<sc2Vector> crossPointArray; 
					if (sfInterSectLineLine(ResLine[0].GetLine(),line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(ResLine[0].GetLine(),line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(ResLine[1].GetLine(),line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(ResLine[1].GetLine(),line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						pTempLineCross->SetLabel(st);
						pTempLineCross->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
					}
				}

				// 如果4个交点都找到
				if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
				{

					CSearchResult searchResult;
					searchResult.m_dScore = tmpQuickResult.GetScore();
					switch(m_tmpSearchGuiParam.m_nResultOutMode)
					{
					case 0://中心点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 1://中心点上方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY())/2;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 2://中心点下方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[2].GetY()+vLineCrossPoint[3].GetY())/2;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 3://中心点左方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[2].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 4://中心点右方点
						searchResult.m_dPosX = (vLineCrossPoint[1].GetX()+vLineCrossPoint[3].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 5://左上方
						searchResult.m_dPosX = vLineCrossPoint[0].GetX();
						searchResult.m_dPosY = vLineCrossPoint[0].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 6://右上方
						searchResult.m_dPosX = vLineCrossPoint[1].GetX();
						searchResult.m_dPosY = vLineCrossPoint[1].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 7://左下方
						searchResult.m_dPosX = vLineCrossPoint[2].GetX();
						searchResult.m_dPosY = vLineCrossPoint[2].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 8://右下方
						searchResult.m_dPosX = vLineCrossPoint[3].GetX();
						searchResult.m_dPosY = vLineCrossPoint[3].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					default:
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					}

					strTmpKey.Format(_T("%s_ResultLine0_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+0;
					pTemLine->SetLine(ResLine[0].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine1_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+1;
					pTemLine->SetLine(ResLine[1].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine2_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+2;
					pTemLine->SetLine(line3);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine3_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+3;
					pTemLine->SetLine(line4);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY),sc2Vector(50,50));
					pTempCross->SetLineColor(RGB(255,255,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);

					for (long k=0; k<RECT_LINE_NUM; k++)
					{
						searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
						searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
						searchResult.m_vdAuxiliaryAngle[k] = 0;
					}
					//清空容器，释放内存
					points3.clear();
					points4.clear();
					max3.clear();
					max4.clear();

					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


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
			else if(m_tmpSearchDataParam.m_nRiviseMode==1)
			{
				if (((ResLine[0].GetRotation().SignedNormMod180()-ResLine[1].GetRotation().SignedNormMod180())>scRadian(skPI/180) )||
					((ResLine[0].GetRotation().SignedNormMod180()-ResLine[1].GetRotation().SignedNormMod180())<scRadian(-skPI/180)))
				{
					m_nResultErrNumber = -6;
					return false;
				}
				if( (points3.size()<=0)||(points4.size()<=0))
				{
					return false;
				}
				//找线3拥有最大距离点points3[tmax3]
				int tmax3 = 0;
				for (size_t l=0;l < points3.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points3[l], line5, m, pointLine1);
					max3.push_back(m);
				}
				if (max3.size()<= 0)
				{
					return false;
				}
				m = max3[0];
				for (size_t l=0;l < points3.size() ;l++)
				{
					if (max3[l] > m)
					{
						m = max3[l];
						tmax3 = l;
					}
				}
				//找线4拥有最大距离点points4[tmax4]
				int tmax4 = 0;
				for (size_t l=0;l < points4.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points4[l], line5, m, pointLine1);
					max4.push_back(m);
				}
				if (max4.size()<= 0)
				{
					return false;
				}
				m = max4[0];
				for (size_t l=0;l < points4.size() ;l++)
				{
					if (max4[l] > m)
					{
						m = max4[l];
						tmax4 = l;
					}
				}

				//做线1的修正线
				line1.SetPosRotation(pointCenter1,angle1);	
				//做线2的修正线
				line2.SetPosRotation(pointCenter2,angle1);
				//做线3的修正线
				line3.SetPosRotation(points3[tmax3],angle2);
				//做线4的修正线
				line4.SetPosRotation(points4[tmax4],angle2);
				// 求交点左上
				if (bFindLine[0] && bFindLine[2])
				{
					int nIntersectIndex = 0;
					std::vector<sc2Vector> crossPointArray; 
					if (sfInterSectLineLine(line1,line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line1,line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						pTempLineCross->SetLabel(st);
						pTempLineCross->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
					}
				}

				// 如果4个交点都找到
				if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
				{

					CSearchResult searchResult;
					switch(m_tmpSearchGuiParam.m_nResultOutMode)
					{
					case 0://中心点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 1://中心点上方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY())/2;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 2://中心点下方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[2].GetY()+vLineCrossPoint[3].GetY())/2;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 3://中心点左方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[2].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 4://中心点右方点
						searchResult.m_dPosX = (vLineCrossPoint[1].GetX()+vLineCrossPoint[3].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 5://左上方
						searchResult.m_dPosX = vLineCrossPoint[0].GetX();
						searchResult.m_dPosY = vLineCrossPoint[0].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 6://右上方
						searchResult.m_dPosX = vLineCrossPoint[1].GetX();
						searchResult.m_dPosY = vLineCrossPoint[1].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 7://左下方
						searchResult.m_dPosX = vLineCrossPoint[2].GetX();
						searchResult.m_dPosY = vLineCrossPoint[2].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 8://右下方
						searchResult.m_dPosX = vLineCrossPoint[3].GetX();
						searchResult.m_dPosY = vLineCrossPoint[3].GetY();
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					default:
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle1.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					}

					strTmpKey.Format(_T("%s_ResultLine0_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+0;
					pTemLine->SetLine(line1);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine1_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+1;
					pTemLine->SetLine(line2);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine2_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+2;
					pTemLine->SetLine(line3);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine3_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+3;
					pTemLine->SetLine(line4);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY),sc2Vector(50,50));
					pTempCross->SetLineColor(RGB(255,255,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);
					for (long k=0; k<RECT_LINE_NUM; k++)
					{
						searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
						searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
						searchResult.m_vdAuxiliaryAngle[k] = 0;
					}

					//清空容器，释放内存
					points3.clear();
					points4.clear();
					max3.clear();
					max4.clear();

					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


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
			else if(m_tmpSearchDataParam.m_nRiviseMode==2)
			{
				if (((ResLine[2].GetRotation().NormMod180()-ResLine[3].GetRotation().NormMod180())>scRadian(skPI/180) )||
					((ResLine[2].GetRotation().NormMod180()-ResLine[3].GetRotation().NormMod180())<scRadian(-skPI/180)))
				{
					m_nResultErrNumber = -7;
					return false;
				}
				if( (points1.size()<=0)||(points2.size()<=0))
				{
					return false;
				}
				// 				//做线3的修正线
				// 				line3.SetPosRotation(pointCenter3,scRadian(angle3));	
				// 				//做线4的修正线
				// 				line4.SetPosRotation(pointCenter4,scRadian(angle3));

				//找线1拥有最大距离点points1[tmax1]
				int tmax1 = 0;
				for (size_t l=0;l < points1.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points1[l], line6, m, pointLine2);
					max1.push_back(m);
				}
				if (max1.size()<= 0)
				{
					return false;
				}	
				m = max1[0];
				for (size_t l=0;l < points1.size() ;l++)
				{
					if (max1[l] > m)
					{
						m = max1[l];
						tmax1 = l;
					}
				}
				//找线2拥有最大距离点points2[tmax2]
				int tmax2 = 0;
				for (size_t l=0;l < points2.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points2[l], line6, m, pointLine2);
					max2.push_back(m);
				}
				if (max2.size()<= 0)
				{
					return false;
				}	
				m = max2[0];
				for (size_t l=0;l < points2.size() ;l++)
				{
					if (max2[l] > m)
					{
						m = max2[l];
						tmax2 = l;
					}
				}

				//做线1的修正线
				line1.SetPosRotation(points1[tmax1],angle4);
				//做线2的修正线
				line2.SetPosRotation(points2[tmax2],angle4);
				// 求交点左上
				if (bFindLine[0] && bFindLine[2])
				{
					int nIntersectIndex = 0;
					std::vector<sc2Vector> crossPointArray; 
					if (sfInterSectLineLine(line1,ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line1,ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,ResLine[2].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,ResLine[3].GetLine(), bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						pTempLineCross->SetLabel(st);
						pTempLineCross->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
					}
				}

				// 如果4个交点都找到
				if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
				{

					CSearchResult searchResult;
					switch(m_tmpSearchGuiParam.m_nResultOutMode)
					{
					case 0://中心点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 1://中心点上方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY())/2;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 2://中心点下方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[2].GetY()+vLineCrossPoint[3].GetY())/2;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 3://中心点左方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[2].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 4://中心点右方点
						searchResult.m_dPosX = (vLineCrossPoint[1].GetX()+vLineCrossPoint[3].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 5://左上方
						searchResult.m_dPosX = vLineCrossPoint[0].GetX();
						searchResult.m_dPosY = vLineCrossPoint[0].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 6://右上方
						searchResult.m_dPosX = vLineCrossPoint[1].GetX();
						searchResult.m_dPosY = vLineCrossPoint[1].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 7://左下方
						searchResult.m_dPosX = vLineCrossPoint[2].GetX();
						searchResult.m_dPosY = vLineCrossPoint[2].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 8://右下方
						searchResult.m_dPosX = vLineCrossPoint[3].GetX();
						searchResult.m_dPosY = vLineCrossPoint[3].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					default:
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					}
					strTmpKey.Format(_T("%s_ResultLine0_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+0;
					pTemLine->SetLine(line1);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine1_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+1;
					pTemLine->SetLine(line2);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine2_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+2;
					pTemLine->SetLine(ResLine[2].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine3_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+3;
					pTemLine->SetLine(ResLine[3].GetLine());
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);


					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY),sc2Vector(50,50));
					pTempCross->SetLineColor(RGB(255,255,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);

					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);
					for (long k=0; k<RECT_LINE_NUM; k++)
					{
						searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
						searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
						searchResult.m_vdAuxiliaryAngle[k] = 0;
					}

					//清空容器，释放内存
					points1.clear();
					points2.clear();
					max1.clear();
					max2.clear();

					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


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
			else if(m_tmpSearchDataParam.m_nRiviseMode==3)
			{
				if (((ResLine[2].GetRotation().NormMod180()-ResLine[3].GetRotation().NormMod180())>scRadian(skPI/180) )||
					((ResLine[2].GetRotation().NormMod180()-ResLine[3].GetRotation().NormMod180())<scRadian(-skPI/180)))
				{
					m_nResultErrNumber = -7;
					return false;
				}
				if( (points1.size()<=0)||(points2.size()<=0))
				{
					return false;
				}
				//找线1拥有最大距离点points1[tmax1]
				int tmax1 = 0;
				for (size_t l=0;l < points1.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points1[l], line6, m, pointLine2);
					max1.push_back(m);
				}
				if (max1.size()<= 0)
				{
					return false;
				}	
				m = max1[0];
				for (size_t l=0;l < points1.size() ;l++)
				{
					if (max1[l] > m)
					{
						m = max1[l];
						tmax1 = l;
					}
				}
				//找线2拥有最大距离点points2[tmax2]
				int tmax2 = 0;
				for (size_t l=0;l < points2.size() ;l++)
				{
					bool ret = sfDistancePoint2Line(points2[l], line6, m, pointLine2);
					max2.push_back(m);
				}
				if (max2.size()<= 0)
				{
					return false;
				}	
				m = max2[0];
				for (size_t l=0;l < points2.size() ;l++)
				{
					if (max2[l] > m)
					{
						m = max2[l];
						tmax2 = l;
					}
				}

				//做线3的修正线
				line3.SetPosRotation(pointCenter3,angle3);	
				//做线4的修正线
				line4.SetPosRotation(pointCenter4,angle3);

				//做线1的修正线
				line1.SetPosRotation(points1[tmax1],angle4);
				//做线2的修正线
				line2.SetPosRotation(points2[tmax2],angle4);

				// 求交点左上
				if (bFindLine[0] && bFindLine[2])
				{
					int nIntersectIndex = 0;
					std::vector<sc2Vector> crossPointArray; 
					if (sfInterSectLineLine(line1,line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line1,line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,line3, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
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
					if (sfInterSectLineLine(line2,line4, bIntersect[nIntersectIndex], crossPointArray)
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
						st.Format(_T("%d X:%.1f;Y:%.1f"), i, crossPointArray[0].GetX(), crossPointArray[0].GetY());
						pTempLineCross->SetLabel(st);
						pTempLineCross->SetLabelVisible(TRUE);
						m_pGuiSearchLineCrossArray.push_back(pTempLineCross);
					}
				}

				// 如果4个交点都找到
				if (bIntersect[0] && bIntersect[1] && bIntersect[2] && bIntersect[3])
				{

					CSearchResult searchResult;
					switch(m_tmpSearchGuiParam.m_nResultOutMode)
					{
					case 0://中心点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 1://中心点上方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY())/2;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 2://中心点下方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[2].GetY()+vLineCrossPoint[3].GetY())/2;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 3://中心点左方点
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[2].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 4://中心点右方点
						searchResult.m_dPosX = (vLineCrossPoint[1].GetX()+vLineCrossPoint[3].GetX())/2;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 5://左上方
						searchResult.m_dPosX = vLineCrossPoint[0].GetX();
						searchResult.m_dPosY = vLineCrossPoint[0].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 6://右上方
						searchResult.m_dPosX = vLineCrossPoint[1].GetX();
						searchResult.m_dPosY = vLineCrossPoint[1].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 7://左下方
						searchResult.m_dPosX = vLineCrossPoint[2].GetX();
						searchResult.m_dPosY = vLineCrossPoint[2].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					case 8://右下方
						searchResult.m_dPosX = vLineCrossPoint[3].GetX();
						searchResult.m_dPosY = vLineCrossPoint[3].GetY();
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					default:
						searchResult.m_dPosX = (vLineCrossPoint[0].GetX()+vLineCrossPoint[1].GetX()+vLineCrossPoint[2].GetX()
							+vLineCrossPoint[3].GetX())/4;
						searchResult.m_dPosY = (vLineCrossPoint[0].GetY()+vLineCrossPoint[1].GetY()+vLineCrossPoint[2].GetY()
							+vLineCrossPoint[3].GetY())/4;
						searchResult.m_dAngle = angle3.SignedNormMod180().ToDouble();
						searchResult.m_dScore = tmpQuickResult.GetScore();
						break;
					}
					strTmpKey.Format(_T("%s_ResultLine0_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					scGuiLine *pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+0;
					pTemLine->SetLine(line1);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine1_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+1;
					pTemLine->SetLine(line2);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine2_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+2;
					pTemLine->SetLine(line3);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultLine3_%d"), m_strKeyText, i);
					//scGuiLine *pTemLine = new scGuiLine;
					pTemLine = m_GuiSearchLine+i*RECT_LINE_NUM+3;
					pTemLine->SetLine(line4);
					pTemLine->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTemLine->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTemLine->SetTipTextEnabled(true);
					pTemLine->SetTipText(strTmpKey);
					m_pGuiSearchLineArray.push_back(pTemLine);

					strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
					//scGuiCross *pTempCross = new scGuiCross;
					scGuiCross *pTempCross = m_GuiSearchCross+i;
					pTempCross->SetCenterExtent(sc2Vector(searchResult.m_dPosX,searchResult.m_dPosY),sc2Vector(50,50));
					pTempCross->SetLineColor(RGB(255,255,0));
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					CString st;
					st.Format(_T("%d X:%.1f;Y:%.1f"), i, searchResult.m_dPosX, searchResult.m_dPosY);
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);
					for (long k=0; k<RECT_LINE_NUM; k++)
					{
						searchResult.m_vdAuxiliaryPosX[k] = vLineCrossPoint[k].GetX();
						searchResult.m_vdAuxiliaryPosY[k] = vLineCrossPoint[k].GetY();
						searchResult.m_vdAuxiliaryAngle[k] = 0;
					}
					//清空容器，释放内存
					points1.clear();
					points2.clear();
					max1.clear();
					max2.clear();

					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpQuickResult.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);


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
		m_nResultErrNumber = 0;
		return false;
	}

	return true;
}

// 获取搜索结果个数										
int CVirtualRectSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CVirtualRectSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板文件
bool CVirtualRectSearchTool::LoadModelFromFile(CString strDir)
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
	CVirtualRectSearchTrainDataParam tmpTrainDataParam;
	CVirtualRectSearchTrainGuiParam tmpTrainGuiParam;
	CVirtualRectSearchSearchDataParam tmpSearchDataParam;
	CVirtualRectSearchSearchGuiParam tmpSearchGuiParam;
	CVirtualRectSearchTrainResult    tmpTrainResult;
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
	if (cSearchToolType != eVirtualRectSearchTool)
	{
		return false;
	}
	////////////CameraParam//////////////
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


	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	// grau
	bRet = cXMLConfigurator.FindElem(_T("Grau"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dGrau = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// noise thre
	bRet = cXMLConfigurator.FindElem(_T("NoiseThre"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lNoiseThre = _ttol(cXMLConfigurator.GetElemData());
	// total mag
	bRet = cXMLConfigurator.FindElem(_T("TotalMag"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_lTotalMag = _ttol(cXMLConfigurator.GetElemData());
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

	//DualEdge1
	bRet = cXMLConfigurator.FindElem(_T("DualEdge1"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[0] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[0] = _ttoi(cXMLConfigurator.GetElemData());

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

	//DualEdge2
	bRet = cXMLConfigurator.FindElem(_T("DualEdge2"));
	if (!bRet)
	{
		tmpTrainDataParam.m_bEnableDualEdge[1] = false;
	}
	tmpTrainDataParam.m_bEnableDualEdge[1] = _ttoi(cXMLConfigurator.GetElemData());

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
		return false;
	}
	tmpTrainDataParam.m_lFilterHalfWidth[2] = _ttol(cXMLConfigurator.GetElemData());
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
	tmpTrainDataParam.m_bEnableDualEdge[2] = _ttoi(cXMLConfigurator.GetElemData());

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
		return false;
	}
	tmpTrainDataParam.m_lFilterHalfWidth[3] = _ttol(cXMLConfigurator.GetElemData());
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
	tmpTrainDataParam.m_bEnableDualEdge[3] = _ttoi(cXMLConfigurator.GetElemData());

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
	// train affine rect
	bRet = cXMLConfigurator.FindElem(_T("TrainAffineRect"));
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
	// 	// Line visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("LineVisible"));
	// 	if (!bRet)
	// 	{
	// 		tmpTrainGuiParam.m_bLineVisible = _ttoi(cXMLConfigurator.GetElemData());
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

	bRet = cXMLConfigurator.FindElem(_T("RiviseMode"));
	if (bRet)
	{
		tmpSearchDataParam.m_nRiviseMode = _ttoi(cXMLConfigurator.GetElemData());
	}
	else
	{
		tmpSearchDataParam.m_nRiviseMode = 1;
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
	// 
	// 
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
	// 	// Line visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("LineVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bLineVisible = _ttoi(cXMLConfigurator.GetElemData());
	// 	// Line cross visible 
	// 	bRet = cXMLConfigurator.FindElem(_T("LineCrossVisible"));
	// 	if (!bRet)
	// 	{
	// 		return false;
	// 	}
	// 	tmpSearchGuiParam.m_bLineCrossVisible = _ttoi(cXMLConfigurator.GetElemData());
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
	scImageGray tmpImageModel;
	scImageGray tmpImageMask;
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
	scQuickSearchTool tmpQuickSearchTool;
	bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
#ifdef VISIONPLUS_IMGPROC
		{
			if (tmpTrainDataParam.m_bMaskEnable)
			{
				bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"), strDir + _T("Model.bmp"), strDir + _T("Mask.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
					if (!bRet)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				bRet = tmpQuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"), strDir + _T("Model.bmp"));
				if (bRet)
				{
					bRet = tmpQuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
					if (!bRet)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}

#else
		{
			return false;
		}
#endif
	}

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;
	if (tmpTrainDataParam.m_bMaskEnable)
	{
		m_tmpImageMask = tmpImageMask;
	}
	bRet = m_QuickSearchTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
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
	m_ImageTrain		= tmpImageModel;
	m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CVirtualRectSearchTool::SaveModelToFile(CString strDir)
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
	int i=0;
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
	// 	// model point visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
	// 	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
	// 	// search rect visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
	// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
	// 	// affine rect visible
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
	// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
	// 	// findcircle visible 
	// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bLineVisible);
	// 	cXMLConfigurator.AddElem(_T("LineVisible"), strInfo);
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

	strInfo.Format(_T("%d"), m_SearchDataParam.m_nRiviseMode);
	cXMLConfigurator.AddElem(_T("RiviseMode"), strInfo);

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
	// 	// Line visible 
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bLineVisible);
	// 	cXMLConfigurator.AddElem(_T("LineVisible"), strInfo);	
	// 	// Line cross visible 
	// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bLineCrossVisible);
	// 	cXMLConfigurator.AddElem(_T("LineCrossVisible"), strInfo);
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

	//////////////////////////存储模板图像//////////////////////////
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

	//////////////////////////存储模板ptq文件//////////////////////////
	bRet = m_QuickSearchTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 更新训练控件数据信息	
bool CVirtualRectSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_ModelPoint			= m_GuiModelPoint.GetCenter();
		m_tmpTrainGuiParam.m_TrainAffineRect	= m_GuiTrainRect.GetAffineRect();
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
		}
	}
	else // 将数据传递到Gui控件上
	{
		m_GuiModelPoint.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint, m_GuiModelPoint.GetExtent());//尺寸不变

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetAffineRect(m_tmpTrainGuiParam.m_TrainAffineRect);

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

		}

	}

	return true;
}

bool CVirtualRectSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
bool CVirtualRectSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{	
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件				
bool CVirtualRectSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CVirtualRectSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchAffineRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);
	ClearGraphicsArray(m_pGuiSearchLineArray);
	ClearGraphicsArray(m_pGuiSearchLineCrossArray);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_1);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_2);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_3);
	ClearGraphicsArray(m_pGuiSearchResultFitPointArray_4);

	return true;
}

bool CVirtualRectSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen,rot,skew;
	m_GuiTrainRect.GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen,rot,skew);
	m_GuiTrainRect.SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);

	//搜索区域

	m_GuiSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//参考点
	m_GuiModelPoint.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
	m_GuiModelPoint.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
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

	return true;
}

bool CVirtualRectSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
		m_pGuiInteractiveContainer->RemoveItem( m_strKeyText+_T("_ModelPoint"));
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

bool CVirtualRectSearchTool::ClearResultGui()
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
bool CVirtualRectSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	bool bRet = m_QuickSearchTool.GetTrainModelImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}

	return true;
}	

// 恢复训练参数
bool CVirtualRectSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CVirtualRectSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CVirtualRectSearchTool::SetLanguage(int nLanguage)
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

CString CVirtualRectSearchTool::GetResultErrInfo()
{
	CString strInfo = _T("");
	switch(m_nResultErrNumber)
	{
	case 0:
		strInfo += _T("几何定位失败");
		break;
	case -1:
		strInfo += _T("找线1失败");
		break;
	case -2:
		strInfo += _T("找线2失败");
		break;
	case -3:
		strInfo += _T("找线3失败");
		break;
	case -4:
		strInfo += _T("找线4失败");
		break;
	case -5:
		strInfo += _T("找线1234失败");
		break;
	case -6:
		strInfo += _T("找线12不平行");
		break;
	case -7:
		strInfo += _T("找线34不平行");
		break;
	default:
		strInfo.Format(_T("Err:%d,其他错误"),m_nResultErrNumber);
		break;
	}
	return strInfo;
}

bool CVirtualRectSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eVirtualRectSearchTool)
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
	rRect.SetOrigion(sc2Vector(vPos.GetX()-rRect.GetSize().GetX()/2,vPos.GetY()-rRect.GetSize().GetY()/2));
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