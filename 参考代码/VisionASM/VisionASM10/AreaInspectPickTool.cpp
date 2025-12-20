#include "stdafx.h"
#include "AreaInspectPickTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "Shlwapi.h" 
#include "svTimer.h"
#include "svBlob.h"
#include "svBlobSceneDescription.h"
#include "svHistogram.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAreaInspectPickTool::CAreaInspectPickTool()
{
	m_strName								= _T("ACF检测");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;



	CString str;

	for (int i=0;i<MAXAREATOOLNUM;i++)
	{
		scGuiAffineRect *tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);

		scGuiRect* tempGuiSearchRect = new scGuiRect;
		tempGuiSearchRect->SetCenterWidthHeight(250+i*100, 200, 50, 50);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiSearchRect->SetLabel(str);
		tempGuiSearchRect->SetLabelVisible(TRUE);
		m_GuiSearchRect.push_back(tempGuiSearchRect);

		scGuiCross* tempGuiModelPoint = new scGuiCross;
		tempGuiModelPoint->SetCenterXYWidthHeight(250+i*100, 200);
		str.Format(_T("参考点%d"),i+1);
		tempGuiModelPoint->SetLabel(str);
		tempGuiModelPoint->SetLabelVisible(TRUE);
		m_GuiModelPoint.push_back(tempGuiModelPoint);
	}
	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);
	m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
	m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
	
	
	
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	//m_tmpTrainDataParam.m_dGrau				= m_AreaInspectTool.GetGranularity();
	//m_tmpTrainDataParam.m_lNoiseThre		= m_AreaInspectTool.GetNoiseThre();
	//m_tmpTrainDataParam.m_lTotalMag			= m_AreaInspectTool.GetTotalGradMagThre();
	//m_tmpTrainDataParam.m_bMaskEnable		= false; // 默认不采用掩模图像

	for (int i=0;i<MAXAREATOOLNUM;i++)
	{
// 		scRect* tempRect = new scRect;
// 		m_tmpTrainGuiParam.m_SearchRect.push_back(tempRect);
		*(m_tmpTrainGuiParam.m_SearchRect.at(i))			= m_GuiSearchRect.at(i)->GetRect();
		m_tmpTrainGuiParam.m_nLineWidth.at(i)			= m_GuiTrainRect.at(i)->GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor.at(i)			= m_GuiTrainRect.at(i)->GetLineColor();

		m_tmpSearchGuiParam.m_nLineWidth.at(i)		= m_GuiSearchRect.at(i)->GetLineWidth();
		m_tmpSearchGuiParam.m_LineColor.at(i)			= m_GuiSearchRect.at(i)->GetLineColor();
	}
	
	m_tmpTrainGuiParam.m_Hand1SearchRect    = m_GuiHand1SearchRect.GetRect();
	m_tmpTrainGuiParam.m_Hand2SearchRect    = m_GuiHand2SearchRect.GetRect();
	m_tmpSearchDataParam.m_nSearchNum		= 1;
	m_tmpSearchDataParam.m_dAcceptThre		= m_AreaInspectTool.GetAcceptThreshold();
	m_tmpSearchDataParam.m_dAngleLow		= -5.0;
	m_tmpSearchDataParam.m_dAngleHigh		= 5.0;


	m_TrainDataParam						= m_tmpTrainDataParam;
	m_TrainGuiParam							= m_tmpTrainGuiParam;
	m_SearchDataParam						= m_tmpSearchDataParam;
	m_SearchGuiParam						= m_tmpSearchGuiParam;

	m_dCurMean.resize(MAXAREATOOLNUM,0);
	m_dCurStdDev.resize(MAXAREATOOLNUM,0);

// 	m_nMaxGreyVal = 0;
// 	m_dDefectArea = 0.0;
}

CAreaInspectPickTool::CAreaInspectPickTool(CAreaInspectPickTool &cAreaInspectPickTool)
{
	if (cAreaInspectPickTool.GetSearchToolType() != eAreaInspectPickTool)
	{
		return;
	}

	// 清空
//  	ClearAllGui();
//  	ClearResult();

	// 初始化
	m_strName								= _T("ACF检测");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		if (m_GuiTrainRect.at(i)!=NULL)
		{
			delete m_GuiTrainRect.at(i);
			m_GuiTrainRect.at(i) = NULL;
		}
	}
	m_GuiTrainRect.clear();
	for (int i=0;i<m_GuiSearchRect.size();i++)
	{
		if (m_GuiSearchRect.at(i)!=NULL)
		{
			delete m_GuiSearchRect.at(i);
			m_GuiSearchRect.at(i) = NULL;
		}
	}
	m_GuiSearchRect.clear();
	
	for (int i=0;i<m_GuiModelPoint.size();i++)
	{
		if (m_GuiModelPoint.at(i)!=NULL)
		{
			delete m_GuiModelPoint.at(i);
			m_GuiModelPoint.at(i) = NULL;
		}
	}
	m_GuiModelPoint.clear();

	for (int i=0;i<m_tmpTrainGuiParam.m_SearchRect.size();i++)
	{
		if (m_tmpTrainGuiParam.m_SearchRect.at(i)!=NULL)
		{
			delete m_tmpTrainGuiParam.m_SearchRect.at(i);
			m_tmpTrainGuiParam.m_SearchRect.at(i) = NULL;
		}
	}
	m_tmpTrainGuiParam.m_SearchRect.clear();

	CString str;
	for (int i=0;i<cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{
		scGuiAffineRect* tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);

		scGuiRect* tempGuiSearchRect = new scGuiRect;
		scRect* tempRect = new scRect;
		tempGuiSearchRect->SetCenterWidthHeight(250+i*100, 200, 50, 50);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiSearchRect->SetLabel(str);
		tempGuiSearchRect->SetLabelVisible(TRUE);
		m_GuiSearchRect.push_back(tempGuiSearchRect);
		m_tmpTrainGuiParam.m_SearchRect.push_back(tempRect);
		*(m_tmpTrainGuiParam.m_SearchRect.at(i))= m_GuiSearchRect.at(i)->GetRect();

		scGuiCross* tempGuiModelPoint = new scGuiCross;
		tempGuiModelPoint->SetCenterXYWidthHeight(250+i*100, 200);
		str.Format(_T("参考点%d"),i+1);
		tempGuiModelPoint->SetLabel(str);
		tempGuiModelPoint->SetLabelVisible(TRUE);
		m_GuiModelPoint.push_back(tempGuiModelPoint);
	}
	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);
	m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
	m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));

	m_tmpTrainGuiParam.m_LineColor.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	m_tmpTrainGuiParam.m_nLineWidth.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,1);
	m_tmpTrainGuiParam.m_bSearchRectVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);

	m_tmpSearchGuiParam.m_LineColor.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	m_tmpSearchGuiParam.m_nLineWidth.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,1);
	m_tmpSearchGuiParam.m_bAffineRectVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);
	m_tmpSearchGuiParam.m_bModelPointVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);
	m_tmpTrainDataParam.m_nInsAreaNum = cAreaInspectPickTool.m_tmpTrainDataParam.m_nInsAreaNum;
	
	for (int i=0;i<cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{
		
		m_tmpTrainGuiParam.m_nLineWidth.at(i)			= m_GuiTrainRect.at(i)->GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor.at(i)			= m_GuiTrainRect.at(i)->GetLineColor();	


		m_tmpSearchGuiParam.m_nLineWidth.at(i)		= m_GuiSearchRect.at(i)->GetLineWidth();
		m_tmpSearchGuiParam.m_LineColor.at(i)			= m_GuiSearchRect.at(i)->GetLineColor();
	}

	m_dCurMean.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurStdDev.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,0);

	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识

	// 赋值
	m_CamHeight								=  cAreaInspectPickTool.m_CamHeight;
	m_CamWidth								=  cAreaInspectPickTool.m_CamWidth;
	m_strName								= cAreaInspectPickTool.m_strName;

	m_TrainDataParam						= cAreaInspectPickTool.m_TrainDataParam;
	m_TrainGuiParam							= cAreaInspectPickTool.m_TrainGuiParam;
	m_SearchDataParam						= cAreaInspectPickTool.m_SearchDataParam;
	m_SearchGuiParam						= cAreaInspectPickTool.m_SearchGuiParam;
	m_TrainResult                           = cAreaInspectPickTool.m_TrainResult;
	m_ImageTrain							= cAreaInspectPickTool.m_ImageTrain;
	m_ImageMask								= cAreaInspectPickTool.m_ImageMask;
	m_bTrainGuiShow							= cAreaInspectPickTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cAreaInspectPickTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cAreaInspectPickTool.m_bTrainOK;
	m_pGuiDisplay							= cAreaInspectPickTool.m_pGuiDisplay;
	
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}

	cAreaInspectPickTool.UpdateSearchGuiData(true);
	cAreaInspectPickTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cAreaInspectPickTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cAreaInspectPickTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cAreaInspectPickTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cAreaInspectPickTool.m_tmpSearchGuiParam);
	SetTrainImage(cAreaInspectPickTool.m_tmpImageTrain);
	SetMaskImage(cAreaInspectPickTool.m_tmpImageMask);
	//if (m_bTrainOK)
	//{
	//	m_bTrainOK = false;

	//}	

	UpdateTrainGuiDisplay();
}

CAreaInspectPickTool& CAreaInspectPickTool::operator=(CAreaInspectPickTool& cAreaInspectPickTool)
{
	if (this == &cAreaInspectPickTool)
	{
		return *this;
	}

	if (cAreaInspectPickTool.GetSearchToolType() != eAreaInspectPickTool)
	{
		return *this;
	}
	
	// 清空
	ClearAllGui();
	ClearResult();
	
	// 初始化
	m_strName								= _T("ACF检测");
	m_bInspectFinish						= false;
	m_bInspectOK							= false;
//	m_pParentTool							= NULL;

	m_pGuiDisplay							= NULL;
	m_hDispWnd                              = NULL;
	m_pGuiInteractiveContainer				= NULL;
	m_pGuiStaticContainer					= NULL;
	m_strKeyText							= _T("");
	m_bTrainOK								= FALSE;
	m_bTrainGuiShow							= true;
	m_bSearchResultGuiShow					= false;

	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		if (m_GuiTrainRect.at(i)!=NULL)
		{
			delete m_GuiTrainRect.at(i);
			m_GuiTrainRect.at(i) = NULL;
		}
	}
	m_GuiTrainRect.clear();
	for (int i=0;i<m_GuiSearchRect.size();i++)
	{
		if (m_GuiSearchRect.at(i)!=NULL)
		{
			delete m_GuiSearchRect.at(i);
			m_GuiSearchRect.at(i) = NULL;
		}
	}
	m_GuiSearchRect.clear();
	for (int i=0;i<m_GuiModelPoint.size();i++)
	{
		if (m_GuiModelPoint.at(i)!=NULL)
		{
			delete m_GuiModelPoint.at(i);
			m_GuiModelPoint.at(i) = NULL;
		}
	}
	m_GuiModelPoint.clear();

	for (int i=0;i<m_tmpTrainGuiParam.m_SearchRect.size();i++)
	{
		if (m_tmpTrainGuiParam.m_SearchRect.at(i)!=NULL)
		{
			delete m_tmpTrainGuiParam.m_SearchRect.at(i);
			m_tmpTrainGuiParam.m_SearchRect.at(i) = NULL;
		}
	}
	m_tmpTrainGuiParam.m_SearchRect.clear();

	CString str;
	for (int i=0;i<cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{
		scGuiAffineRect* tempGuiTrainRect = new scGuiAffineRect;
		tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiTrainRect->SetLabel(str);
		tempGuiTrainRect->SetLabelVisible(TRUE);
		m_GuiTrainRect.push_back(tempGuiTrainRect);

		scGuiRect* tempGuiSearchRect = new scGuiRect;
		scRect* tempRect = new scRect;
		tempGuiSearchRect->SetCenterWidthHeight(250+i*100, 200, 50, 50);
		str.Format(_T("ACF检测%d"),i+1);
		tempGuiSearchRect->SetLabel(str);
		tempGuiSearchRect->SetLabelVisible(TRUE);
		m_GuiSearchRect.push_back(tempGuiSearchRect);
		m_tmpTrainGuiParam.m_SearchRect.push_back(tempRect);
		*(m_tmpTrainGuiParam.m_SearchRect.at(i))= m_GuiSearchRect.at(i)->GetRect();

		scGuiCross* tempGuiModelPoint = new scGuiCross;
		tempGuiModelPoint->SetCenterXYWidthHeight(250+i*100, 200);
		str.Format(_T("参考点%d"),i+1);
		tempGuiModelPoint->SetLabel(str);
		tempGuiModelPoint->SetLabelVisible(TRUE);
		m_GuiModelPoint.push_back(tempGuiModelPoint);
	}
	m_GuiHand1SearchRect.SetCenterWidthHeight(200, 300, 200, 200);	
	m_GuiHand1SearchRect.SetLabelVisible(TRUE);
	m_GuiHand2SearchRect.SetCenterWidthHeight(400, 300, 200, 200);	
	m_GuiHand2SearchRect.SetLabelVisible(TRUE);
	m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
	m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
	CreateGUID(m_strKeyText);				// 创建guid作为Key的唯一标识
	

	m_tmpTrainGuiParam.m_LineColor.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	m_tmpTrainGuiParam.m_nLineWidth.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,1);
	m_tmpTrainGuiParam.m_bSearchRectVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);

	m_tmpSearchGuiParam.m_LineColor.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	m_tmpSearchGuiParam.m_nLineWidth.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,1);
	m_tmpSearchGuiParam.m_bAffineRectVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);
	m_tmpSearchGuiParam.m_bModelPointVisible.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,TRUE);
	m_tmpTrainDataParam.m_nInsAreaNum = cAreaInspectPickTool.m_tmpTrainDataParam.m_nInsAreaNum;
	for (int i=0;i<cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum;i++)
	{

		m_tmpTrainGuiParam.m_nLineWidth.at(i)			= m_GuiTrainRect.at(i)->GetLineWidth();
		m_tmpTrainGuiParam.m_LineColor.at(i)			= m_GuiTrainRect.at(i)->GetLineColor();	


		m_tmpSearchGuiParam.m_nLineWidth.at(i)		= m_GuiSearchRect.at(i)->GetLineWidth();
		m_tmpSearchGuiParam.m_LineColor.at(i)			= m_GuiSearchRect.at(i)->GetLineColor();
	}
	m_dCurMean.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,0);
	m_dCurStdDev.resize(cAreaInspectPickTool.m_TrainDataParam.m_nInsAreaNum,0);
	// 赋值
	m_CamHeight								=  cAreaInspectPickTool.m_CamHeight;
	m_CamWidth								=  cAreaInspectPickTool.m_CamWidth;
	m_strName								= cAreaInspectPickTool.m_strName;

	m_TrainDataParam						= cAreaInspectPickTool.m_TrainDataParam;
	m_TrainGuiParam							= cAreaInspectPickTool.m_TrainGuiParam;
	m_SearchDataParam						= cAreaInspectPickTool.m_SearchDataParam;
	m_SearchGuiParam						= cAreaInspectPickTool.m_SearchGuiParam;
	m_TrainResult							= cAreaInspectPickTool.m_TrainResult;
	m_ImageTrain							= cAreaInspectPickTool.m_ImageTrain;
	m_ImageMask								= cAreaInspectPickTool.m_ImageMask;
	m_bTrainGuiShow							= cAreaInspectPickTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cAreaInspectPickTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cAreaInspectPickTool.m_bTrainOK;
	m_pGuiDisplay							= cAreaInspectPickTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cAreaInspectPickTool.UpdateSearchGuiData(true);
	cAreaInspectPickTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cAreaInspectPickTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cAreaInspectPickTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cAreaInspectPickTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cAreaInspectPickTool.m_tmpSearchGuiParam);
	SetTrainImage(cAreaInspectPickTool.m_tmpImageTrain);
	SetMaskImage(cAreaInspectPickTool.m_tmpImageMask);
	if (m_bTrainOK)
	{
		m_bTrainOK = false;
		

	}

	return *this;
}

CAreaInspectPickTool::~CAreaInspectPickTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
	ClearTrainVectorGui();
}

// 获取工具类型
CSearchToolType CAreaInspectPickTool::GetSearchToolType()
{
	return eAreaInspectPickTool;
}					

void CAreaInspectPickTool::SetSearchToolName(CString strName)
{
	m_strName = strName;
}

CString CAreaInspectPickTool::GetSearchToolName()
{
	return m_strName;
}

// 设置显示控件指针		
bool CAreaInspectPickTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CAreaInspectPickTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CAreaInspectPickTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CAreaInspectPickTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CAreaInspectPickTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CAreaInspectPickTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CAreaInspectPickTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CAreaInspectPickTool::UpdateTrainGuiDisplay()
{
	m_GuiHand1SearchRect.SetVisible(m_bTrainGuiShow);
	m_GuiHand2SearchRect.SetVisible(m_bTrainGuiShow);

	for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		m_GuiSearchRect.at(i)->SetCenterCrossVisible(FALSE);
		m_GuiSearchRect.at(i)->SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible.at(i));

		CString str;
		if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
		{
			m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
			str.Format("_SearchRect%d",i);
			m_pGuiInteractiveContainer->AddItem(m_GuiSearchRect.at(i), m_strKeyText+str);

// 			m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
// 			m_pGuiDisplay->MyInvalidate();			
		}

	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer->AddItem(&m_GuiHand1SearchRect, m_strKeyText+_T("_Hand1SearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiHand2SearchRect, m_strKeyText+_T("_Hand2SearchRect"));
		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	
	
	
	return true;
}

// 更新搜索界面显示									
bool CAreaInspectPickTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

//	m_GuiContour.SetVisible(m_bSearchResultGuiShow);
	
	for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
	{
		m_pGuiSearchResultRectArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bAffineRectVisible.at(i));
	}
	
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
// 		m_pGuiStaticContainer->AddItem(&m_GuiContour,"blob result");
		
		int i=0;
		
		for (i=0; i<m_pGuiSearchResultRectArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultRectArray[i], m_pGuiSearchResultRectArray[i]->GetTipText());
		}
	
		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}
	
	return true;
}

bool CAreaInspectPickTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CAreaInspectPickTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CAreaInspectPickTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CAreaInspectPickTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}
	
	*((CAreaInspectPickTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CAreaInspectPickTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CAreaInspectPickTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CAreaInspectPickTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);
	
	*((CAreaInspectPickTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CAreaInspectPickTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CAreaInspectPickTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_ImageTrain;

	return true;
}

// 设置训练原始图像									
bool CAreaInspectPickTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CAreaInspectPickTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CAreaInspectPickTool::Train()
{
	m_bTrainOK = true;
	m_nResultErrNumber=0;
	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	m_ImageTrain     = m_tmpImageTrain;
	m_ImageMask      = m_tmpImageMask;
	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam  = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;

	return true;
/*
	// 设置训练参数
	if (!m_AreaInspectTool.SetGranularity(m_tmpTrainDataParam.m_dGrau))
	{
		return false;
	}

	if (!m_AreaInspectTool.SetNoiseThre(m_tmpTrainDataParam.m_lNoiseThre))
	{
		return false;
	}

	if (!m_AreaInspectTool.SetTotalGradMagThre(m_tmpTrainDataParam.m_lTotalMag))
	{
		return false;
	}

	// 设置掩模图像
	if (m_tmpTrainDataParam.m_bMaskEnable)
	{
		bool bRet = m_AreaInspectTool.Train(m_tmpImageTrain, m_tmpImageMask, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
								m_tmpTrainGuiParam.m_ModelPoint.GetY());	
		
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
			sc2Vector cModelOrigin;
			m_AreaInspectTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();
			
			m_ImageTrain     = m_tmpImageTrain;
			m_ImageMask      = m_tmpImageMask;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}

		return bRet;
	}
	else
	{
		bool bRet = m_AreaInspectTool.Train(m_tmpImageTrain, m_tmpTrainGuiParam.m_TrainAffineRect, m_tmpTrainGuiParam.m_ModelPoint.GetX(),
			m_tmpTrainGuiParam.m_ModelPoint.GetY());
		
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
			sc2Vector cModelOrigin;
			m_AreaInspectTool.GetModelOrigin(cModelOrigin);
			m_tmpTrainResult.m_dModelX = cModelOrigin.GetX();
			m_tmpTrainResult.m_dModelY = cModelOrigin.GetY();

			m_ImageTrain     = m_tmpImageTrain;
			m_TrainDataParam = m_tmpTrainDataParam;
			m_TrainGuiParam  = m_tmpTrainGuiParam;
			m_TrainResult    = m_tmpTrainResult;
		}

		return bRet;
	}

	return false;
*/}

// 获取训练结果图像														
bool CAreaInspectPickTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_AreaInspectTool.GetTrainResultImage(m_ImageTrainResult);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult;

	return true;
}	

// 获取训练结果
bool CAreaInspectPickTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}
	
	*((CAreaInspectPickTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}						

// 是否训练成功
bool CAreaInspectPickTool::IsTrained()
{
	return m_bTrainOK;
}															
			
// 设置搜索Data参数	
bool CAreaInspectPickTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CAreaInspectPickSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CAreaInspectPickTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}

	*((CAreaInspectPickSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CAreaInspectPickTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CAreaInspectPickSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CAreaInspectPickTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eAreaInspectPickTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CAreaInspectPickSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CAreaInspectPickTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       //训练未成功
		//return false;
	}
	
	// 清空
	ClearResult();

	// 位置修正
// 	double dOffsetX = 0, dOffsetY = 0;
// 	if ( (m_pParentTool != NULL) && (m_tmpTrainDataParam.m_strPositionToolName != _T("")) )
// 	{
// 		if (m_pParentTool->GetSearchToolType() == eACFInspectTool)
// 		{
// 			CACFInspectTool* pParentTool = (CACFInspectTool*)m_pParentTool;
// 			CPositionInspectTool* pPositionTool = (CPositionInspectTool*)(pParentTool->GetPositionTool(m_tmpTrainDataParam.m_strPositionToolName));
// 
// 			if (pPositionTool != NULL)
// 			{
// 				if ( (pPositionTool->m_bInspectFinish) && (pPositionTool->m_bInspectOK) )
// 				{
// 					CSearchResult searchResult;
// 					pPositionTool->GetResult(0, searchResult);
// 
// 					CPositionInspectTrainResult trainResult;
// 					pPositionTool->GetTrainResult((CBaseTrainResult*)(&trainResult));
// 
// 					dOffsetX = searchResult.m_dPosX - trainResult.m_dModelX;
// 					dOffsetY = searchResult.m_dPosY - trainResult.m_dModelY;
// 				}
// 				else
// 				{
// 					m_bInspectFinish = true;
// 					m_bInspectOK = false;
// 					return false;
// 				}
// 			}
// 		}
// 	}
	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
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
	// Blob分析
	std::vector<bool> bInspectResult;
	m_dCurMean.clear();
	m_dCurStdDev.clear();



	for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		scRect rect = *(m_tmpTrainGuiParam.m_SearchRect.at(i));
		if(bIsProcessImage)
		{
			rect = rect.Intersect(scRect(0,0,tmpImage.Width(),ImageSearch.Height()));
		}
		else
		{
			rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));
		}

		
		//////////////////////////////////////////////////////////////////////////
		// 特征计算
		HStatus		nHStatus;
		svStd vector<long> histogramResult(256, 0);
		if(bIsProcessImage)
		{
			nHStatus = sfHistogram(tmpImage,rect,histogramResult);
		}
		else
		{
			nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
		}
		if (nHStatus!=HSuccess)
		{
			bInspectResult.push_back(false);
			m_dCurMean.push_back(-1.0);
			m_dCurStdDev.push_back(-1.0);
			
		}
		else
		{
			scHistoStats histoStats(histogramResult);
			m_dCurMean.push_back(histoStats.Mean());
			m_dCurStdDev.push_back(histoStats.StdDev());

			bool m_bmean=true;
			bool m_bstddev=true;

			if (m_tmpTrainDataParam.m_bMeanInvalid.at(i))
			{
				if ((m_dCurMean.at(i)<m_tmpTrainDataParam.m_dMinMean.at(i)) || (m_dCurMean.at(i)>m_tmpTrainDataParam.m_dMaxMean.at(i)))
				{
					m_bmean=false;

				}
			}

			if (m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
			{
				if ((m_dCurStdDev.at(i)<m_tmpTrainDataParam.m_dMinStdDev.at(i))||(m_dCurStdDev.at(i)>m_tmpTrainDataParam.m_dMaxStdDev.at(i)))
				{
					m_bstddev=false;

				}
			}

			
			if (m_tmpTrainDataParam.m_bMeanInvert.at(i) && m_tmpTrainDataParam.m_bMeanInvalid.at(i))
			{
				m_bmean = !m_bmean;
			}

			if (m_tmpTrainDataParam.m_bStdDevInvert.at(i) && m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
			{
				m_bstddev = !m_bstddev;
			}

			bInspectResult.push_back(m_bmean && m_bstddev);

	
		}
		//////////////////////////////////////////////////////////////////////////	
	}

	
	int nValidResultNum=m_tmpTrainDataParam.m_nInsAreaNum;
	bool hand1flag=false;//吸头1有值
	bool hand2flag=false;//吸头2有值
	CSearchResult searchResultHand1;
	CSearchResult searchResultHand2;
	vector<bool> ResinRect;//结果在矩形里面

	for (int i=0,j=0;i<nValidResultNum;i++)
	{
		
		scRect rect = *(m_tmpTrainGuiParam.m_SearchRect.at(i));


		sc2Vector pos1 = rect.GetUL();//左上
		sc2Vector pos2 = rect.GetUR();//右上
		sc2Vector pos3 = rect.GetLR();//右下
		sc2Vector center;	
		center.SetX((pos1.GetX()+pos3.GetX())/2);
		center.SetY((pos1.GetY()+pos3.GetY())/2);
		//hand1
		if(PointInRect(center,m_tmpTrainGuiParam.m_Hand1SearchRect)&&!hand1flag&&bInspectResult.at(i))
		{
			hand1flag=true;
			searchResultHand1.m_bResultPickIndex=1;
		
			CResultSearchRect resRect;
			resRect.m_searchRect.SetCornerPoLengthsRotAndSkew(rect.GetUL(), rect.GetWidth(), rect.GetHeight());
			searchResultHand1.m_vTrainSearchRect.push_back(resRect);

			searchResultHand1.m_dPosX=pos1.GetX();
			searchResultHand1.m_dPosY=pos1.GetY();
			searchResultHand1.m_dAngle=0;
			searchResultHand1.m_vdAuxiliaryPosX[0]=pos2.GetX();
			searchResultHand1.m_vdAuxiliaryPosY[0]=pos2.GetY();
			searchResultHand1.m_vdAuxiliaryAngle[0]=0;
			j++;
			ResinRect.push_back(true);
			continue;;
		}

		//hand2
		if(PointInRect(center,m_tmpTrainGuiParam.m_Hand2SearchRect)&&!hand2flag&&bInspectResult.at(i))
		{
			hand2flag=true;
			searchResultHand2.m_bResultPickIndex=2;
			CResultSearchRect resRect;
			resRect.m_searchRect.SetCornerPoLengthsRotAndSkew(rect.GetUL(), rect.GetWidth(), rect.GetHeight());
			searchResultHand2.m_vTrainSearchRect.push_back(resRect);
			searchResultHand2.m_dPosX=pos1.GetX();
			searchResultHand2.m_dPosY=pos1.GetY();
			searchResultHand2.m_dAngle=0;
			searchResultHand2.m_vdAuxiliaryPosX[0]=pos2.GetX();
			searchResultHand2.m_vdAuxiliaryPosY[0]=pos2.GetY();
			searchResultHand2.m_vdAuxiliaryAngle[0]=0;
			j++;
			ResinRect.push_back(true);
			continue;;
		}

		ResinRect.push_back(false);
		

		
	}
	if(searchResultHand1.m_bResultPickIndex>0)m_SearchResultArray.push_back(searchResultHand1);
	if(searchResultHand2.m_bResultPickIndex>0)m_SearchResultArray.push_back(searchResultHand2);


	switch(m_tmpTrainDataParam.m_nLogicIndex)
	{
	case 0:  //与
		m_bInspectOK = TRUE;
		break;
	case 1: //或
		m_bInspectOK = FALSE;
		break;
	default:
		m_bInspectOK = TRUE;		

	}
	for (int i=0;i<bInspectResult.size();i++)
	{
		switch(m_tmpTrainDataParam.m_nLogicIndex)
		{
		case 0:  //与
			m_bInspectOK = m_bInspectOK && bInspectResult.at(i);
			break;
		case 1: //或
			m_bInspectOK = m_bInspectOK || bInspectResult.at(i);
			break;
		default:
			m_bInspectOK = m_bInspectOK && bInspectResult.at(i);
			//break;
		}
	}
	
	if(!m_bInspectOK) m_nResultErrNumber = -2;

	CString	strTmpKey;
	CString strLable;
	for (int i=0;i<bInspectResult.size();i++)
	{
		strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
		scGuiRect *pTempAffineRect = m_GuiSearchResultRect+i;
		pTempAffineRect->SetRect((*m_tmpTrainGuiParam.m_SearchRect.at(i)));
		pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor.at(i));
		if ((bInspectResult.at(i))&&(ResinRect[i]))
		{
			pTempAffineRect->SetLineColor(RGB(0,255,0));
		}
		else
		{
			
			pTempAffineRect->SetLineColor(RGB(255,0,0));
		}
		pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth.at(i));
		pTempAffineRect->SetCenterCrossVisible(FALSE);
		pTempAffineRect->SetTipTextEnabled(true);
		pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
		
		strLable.Format(_T("%s_%d 亮:%.2f 均匀:%.2f"),bInspectResult.at(i)? "ACF_OK":"ACF_NG", i+1,m_dCurMean.at(i),m_dCurStdDev.at(i));
		pTempAffineRect->SetLabel(strLable);
		pTempAffineRect->SetLabelVisible(TRUE);
		m_pGuiSearchResultRectArray.push_back(pTempAffineRect);
	}
	//m_SearchDataParam = m_tmpSearchDataParam;
	//m_SearchGuiParam  = m_tmpSearchGuiParam;

	return m_bInspectOK;

}

bool CAreaInspectPickTool::SearchStatistic(cpImage &ImageSearch)
{
	// 清空
	ClearResult();

	// 位置修正
	// 	double dOffsetX = 0, dOffsetY = 0;
	// 	if ( (m_pParentTool != NULL) && (m_tmpTrainDataParam.m_strPositionToolName != _T("")) )
	// 	{
	// 		if (m_pParentTool->GetSearchToolType() == eACFInspectTool)
	// 		{
	// 			CACFInspectTool* pParentTool = (CACFInspectTool*)m_pParentTool;
	// 			CPositionInspectTool* pPositionTool = (CPositionInspectTool*)(pParentTool->GetPositionTool(m_tmpTrainDataParam.m_strPositionToolName));
	// 
	// 			if (pPositionTool != NULL)
	// 			{
	// 				if ( (pPositionTool->m_bInspectFinish) && (pPositionTool->m_bInspectOK) )
	// 				{
	// 					CSearchResult searchResult;
	// 					pPositionTool->GetResult(0, searchResult);
	// 
	// 					CPositionInspectTrainResult trainResult;
	// 					pPositionTool->GetTrainResult((CBaseTrainResult*)(&trainResult));
	// 
	// 					dOffsetX = searchResult.m_dPosX - trainResult.m_dModelX;
	// 					dOffsetY = searchResult.m_dPosY - trainResult.m_dModelY;
	// 				}
	// 				else
	// 				{
	// 					m_bInspectFinish = true;
	// 					m_bInspectOK = false;
	// 					return false;
	// 				}
	// 			}
	// 		}
	// 	}

	// Blob分析
	std::vector<bool> bInspectResult;
	m_dCurMean.clear();
	m_dCurStdDev.clear();

	for (int i=0;i<m_tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		scRect rect = (*m_tmpTrainGuiParam.m_SearchRect.at(i));
		rect = rect.Intersect(scRect(0,0,ImageSearch.Width(),ImageSearch.Height()));

		//////////////////////////////////////////////////////////////////////////
		// 特征计算
		HStatus		nHStatus;
		svStd vector<long> histogramResult(256, 0);

		nHStatus = sfHistogram(ImageSearch,rect,histogramResult);
		if (nHStatus!=HSuccess)
		{
			bInspectResult.push_back(false);
			m_dCurMean.push_back(-1.0);
			m_dCurStdDev.push_back(-1.0);

		}
		else
		{
			scHistoStats histoStats(histogramResult);
			m_dCurMean.push_back(histoStats.Mean());
			m_dCurStdDev.push_back(histoStats.StdDev());

			bool m_bmean=true;
			bool m_bstddev=true;

			if (m_tmpTrainDataParam.m_bMeanInvalid.at(i))
			{
				if ((m_dCurMean.at(i)<m_tmpTrainDataParam.m_dMinMean.at(i)) || (m_dCurMean.at(i)>m_tmpTrainDataParam.m_dMaxMean.at(i)))
				{
					m_bmean=false;

				}
			}

			if (m_tmpTrainDataParam.m_bStdDevInvalid.at(i))
			{
				if ((m_dCurStdDev.at(i)<m_tmpTrainDataParam.m_dMinStdDev.at(i))||(m_dCurStdDev.at(i)>m_tmpTrainDataParam.m_dMaxStdDev.at(i)))
				{
					m_bstddev=false;

				}
			}


			if (m_tmpTrainDataParam.m_bMeanInvert.at(i))
			{
				m_bmean = !m_bmean;
			}

			if (m_tmpTrainDataParam.m_bStdDevInvert.at(i))
			{
				m_bstddev = !m_bstddev;
			}

			bInspectResult.push_back(m_bmean && m_bstddev);



		}

		//////////////////////////////////////////////////////////////////////////	
	}

	switch(m_tmpTrainDataParam.m_nLogicIndex)
	{
	case 0:  //与
		m_bInspectOK = TRUE;
		break;
	case 1: //或
		m_bInspectOK = FALSE;
		break;
	default:
		m_bInspectOK = TRUE;
		break;

	}
	for (int i=0;i<bInspectResult.size();i++)
	{
		switch(m_tmpTrainDataParam.m_nLogicIndex)
		{
		case 0:  //与
			m_bInspectOK = m_bInspectOK && bInspectResult.at(i);
			break;
		case 1: //或
			m_bInspectOK = m_bInspectOK || bInspectResult.at(i);
			break;
		default:
			m_bInspectOK = m_bInspectOK && bInspectResult.at(i);
			break;
		}
	}


	CString	strTmpKey;
	for (int i=0;i<bInspectResult.size();i++)
	{
		strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
		scGuiRect *pTempAffineRect = m_GuiSearchResultRect+i;
		pTempAffineRect->SetRect(*m_tmpTrainGuiParam.m_SearchRect.at(i));
		pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor.at(i));
		if (!bInspectResult.at(i))
		{
			pTempAffineRect->SetLineColor(RGB(255,0,0));
		}
		pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth.at(i));
		pTempAffineRect->SetCenterCrossVisible(FALSE);
		pTempAffineRect->SetTipTextEnabled(true);
		pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
		pTempAffineRect->SetLabel(GetSearchToolName());
		pTempAffineRect->SetLabelVisible(TRUE);
		m_pGuiSearchResultRectArray.push_back(pTempAffineRect);
	}


	return true;

}

// 获取搜索结果个数										
int CAreaInspectPickTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CAreaInspectPickTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
	
// 加载模板训练图像
bool CAreaInspectPickTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eAreaInspectPickTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();

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
	ImagePattern = tmpImageModel;

	return true;
}


// 加载模板文件
bool CAreaInspectPickTool::LoadModelFromFile(CString strDir)
{
	// strDir为文件夹路径
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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
	CAreaInspectPickTrainDataParam tmpTrainDataParam;
	CAreaInspectPickTrainGuiParam tmpTrainGuiParam;
	CAreaInspectPickSearchDataParam tmpSearchDataParam;
	CAreaInspectPickSearchGuiParam tmpSearchGuiParam;
	CAreaInspectPickTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eAreaInspectPickTool)
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("ToolName"));
	if (!bRet)
	{
		return false;
	}
	m_strName = cXMLConfigurator.GetElemData();


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

	bRet = cXMLConfigurator.FindElem(_T("MaxInspectToolNum"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nInsAreaNum= _ttoi(cXMLConfigurator.GetElemData());
	
	bRet = cXMLConfigurator.FindElem(_T("LogicIndex"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_nLogicIndex= _ttoi(cXMLConfigurator.GetElemData());

	bRet = cXMLConfigurator.FindElem(_T("nColNum"));
	if (!bRet)
	{
		tmpTrainDataParam.m_nColNum = 1;
	}
	else
	{
		tmpTrainDataParam.m_nColNum = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("nRowNum"));
	if (!bRet)
	{
		tmpTrainDataParam.m_nRowNum = 1;
	}
	else
	{
		tmpTrainDataParam.m_nRowNum = _ttoi(cXMLConfigurator.GetElemData());
	}

	tmpTrainDataParam.m_dMinMean.resize(tmpTrainDataParam.m_nInsAreaNum);
	tmpTrainDataParam.m_dMaxMean.resize(tmpTrainDataParam.m_nInsAreaNum);
	tmpTrainDataParam.m_dMinStdDev.resize(tmpTrainDataParam.m_nInsAreaNum);
	tmpTrainDataParam.m_dMaxStdDev.resize(tmpTrainDataParam.m_nInsAreaNum);

	tmpTrainDataParam.m_bMeanInvalid.resize(tmpTrainDataParam.m_nInsAreaNum);
	tmpTrainDataParam.m_bMeanInvert.resize(tmpTrainDataParam.m_nInsAreaNum);

	tmpTrainDataParam.m_bStdDevInvalid.resize(tmpTrainDataParam.m_nInsAreaNum);
	tmpTrainDataParam.m_bStdDevInvert.resize(tmpTrainDataParam.m_nInsAreaNum);

	tmpSearchGuiParam.m_LineColor.resize(tmpTrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	tmpSearchGuiParam.m_nLineWidth.resize(tmpTrainDataParam.m_nInsAreaNum,1);
	tmpSearchGuiParam.m_bAffineRectVisible.resize(tmpTrainDataParam.m_nInsAreaNum,TRUE);
	tmpSearchGuiParam.m_bModelPointVisible.resize(tmpTrainDataParam.m_nInsAreaNum,TRUE);


	for (int i=0;i<tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		CString strElem;
		strElem.Format(_T("MinMean%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dMinMean.at(i) = _ttof(cXMLConfigurator.GetElemData());

	
		strElem.Format(_T("MaxMean%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dMaxMean.at(i) = _ttof(cXMLConfigurator.GetElemData());

		strElem.Format(_T("MeanInvalid%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bMeanInvalid.at(i) = _ttoi(cXMLConfigurator.GetElemData());

		strElem.Format(_T("MeanInvert%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bMeanInvert.at(i) = _ttoi(cXMLConfigurator.GetElemData());

		strElem.Format(_T("MinStdDev%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dMinStdDev.at(i) = _ttof(cXMLConfigurator.GetElemData());


		strElem.Format(_T("MaxStdDev%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_dMaxStdDev.at(i) = _ttof(cXMLConfigurator.GetElemData());

		strElem.Format(_T("StdDevInvalid%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bStdDevInvalid.at(i) = _ttoi(cXMLConfigurator.GetElemData());

		strElem.Format(_T("StdDevInvert%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		tmpTrainDataParam.m_bStdDevInvert.at(i) = _ttoi(cXMLConfigurator.GetElemData());
	}

	for (int j=0;j<tmpTrainGuiParam.m_SearchRect.size();j++)
	{
		if (tmpTrainGuiParam.m_SearchRect.at(j)!=NULL)
		{
			delete tmpTrainGuiParam.m_SearchRect.at(j);
			tmpTrainGuiParam.m_SearchRect.at(j) = NULL;

		}
	}
	tmpTrainGuiParam.m_SearchRect.clear();

	for (int i=0;i<tmpTrainDataParam.m_nInsAreaNum;i++)
	{
		scRect* tempRect = new scRect;
		tmpTrainGuiParam.m_SearchRect.push_back(tempRect);
		
	}
	
	tmpTrainGuiParam.m_LineColor.resize(tmpTrainDataParam.m_nInsAreaNum,RGB(0, 0, 255));
	tmpTrainGuiParam.m_nLineWidth.resize(tmpTrainDataParam.m_nInsAreaNum,1);
	tmpTrainGuiParam.m_bSearchRectVisible.resize(tmpTrainDataParam.m_nInsAreaNum,TRUE);

	for (int i=0;i<tmpTrainGuiParam.m_SearchRect.size();i++)
	{
		CString strElem;
		// search rect
		strElem.Format(_T("SearchRect%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		cXMLConfigurator.EnterElem();

		strElem.Format(_T("OriginX%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		strElem.Format(_T("OriginY%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_SearchRect.at(i)->SetOrigion(vPos);

		strElem.Format(_T("SizeX%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		strElem.Format(_T("SizeY%d"),i);
		bRet = cXMLConfigurator.FindElem(strElem);
		if (!bRet)
		{
			return false;
		}
		vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
		tmpTrainGuiParam.m_SearchRect.at(i)->SetSize(vPos);
		cXMLConfigurator.LeaveElem();
	}


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
	cXMLConfigurator.LeaveElem();//hand2

	cXMLConfigurator.LeaveElem();//TrainDataParam

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

	////////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	cpImage tmpIImageModel;
	//scImageGray tmpImageMask;
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

	////////////////////////////读取掩模图像//////////////////////////
	//if (tmpTrainDataParam.m_bMaskEnable)
	//{
	//	hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
	//	if (!IsSuccess(hStatus))
	//	{
	//		tmpTrainDataParam.m_bMaskEnable = false;
	//	}
	//	else
	//	{
	//		hStatus = cDIB.GetImage(tmpImageMask);
	//		if (!IsSuccess(hStatus))
	//		{
	//			tmpTrainDataParam.m_bMaskEnable = false;
	//		}
	//	}
	//}
	
	////////////////////////////读取ptq文件//////////////////////////
	//scQuickSearchTool tmpAreaInspectTool;
	////bRet = tmpAreaInspectTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
	//bRet = tmpAreaInspectTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
	//if (!bRet)
	//{
	//	return false;
	//}
	
	// 数据赋值
	//m_tmpImageTrain = tmpIImageModel;
	//if (tmpTrainDataParam.m_bMaskEnable)
	//{
	//	m_tmpImageMask = tmpImageMask;
	//}
	//m_AreaInspectTool.LoadTrainModelFromFile(strDir + _T("Model.ptq"));
	//m_AreaInspectTool.LoadTrainModelFromBinaryFile(strDir + _T("Model.ptq"));
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
	//m_ImageMask			= tmpImageMask;

	m_bTrainOK = true;
	
	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	
	return true;
}
	
// 保存模板文件							
bool CAreaInspectPickTool::SaveModelToFile(CString strDir)
{
	// strDir为文件夹路径，文件夹如果不存在会自动创建
	// 配置文件包括以下部分:
	//    (1) 配置文件Model.xml
	//    (2) 模板图像ModelImage.bmp
	//    (3) AreaInspect模板文件Pattern.ptq或PatternSearch模板文件Pattern.pts或者没有(视定位工具而定)
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

	cXMLConfigurator.AddElem(_T("ToolName"), GetSearchToolName());

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

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nInsAreaNum);
	cXMLConfigurator.AddElem(_T("MaxInspectToolNum"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nLogicIndex);
	cXMLConfigurator.AddElem(_T("LogicIndex"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nColNum);
	cXMLConfigurator.AddElem(_T("nColNum"), strInfo);

	strInfo.Format(_T("%d"), m_TrainDataParam.m_nRowNum);
	cXMLConfigurator.AddElem(_T("nRowNum"), strInfo);


	for (int i=0;i<m_TrainDataParam.m_nInsAreaNum;i++)
	{
		CString strElem;
		strElem.Format(_T("MinMean%d"),i);
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dMinMean.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("MaxMean%d"),i);
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dMaxMean.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);
		
		strElem.Format(_T("MeanInvalid%d"),i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bMeanInvalid.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("MeanInvert%d"),i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bMeanInvert.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("MinStdDev%d"),i);
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dMinStdDev.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("MaxStdDev%d"),i);
		strInfo.Format(_T("%f"), m_TrainDataParam.m_dMaxStdDev.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("StdDevInvalid%d"),i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bStdDevInvalid.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("StdDevInvert%d"),i);
		strInfo.Format(_T("%d"), m_TrainDataParam.m_bStdDevInvert.at(i));
		cXMLConfigurator.AddElem(strElem, strInfo);

		
	}

	for (int i=0;i<m_TrainGuiParam.m_SearchRect.size();i++)
	{
		CString strElem;
		// search rect
		strElem.Format(_T("SearchRect%d"),i);
		cXMLConfigurator.AddElem(strElem);
		cXMLConfigurator.EnterElem();

		strElem.Format(_T("OriginX%d"),i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.at(i)->GetOrigin().GetX());
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("OriginY%d"),i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.at(i)->GetOrigin().GetY());
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("SizeX%d"),i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.at(i)->GetSize().GetX());
		cXMLConfigurator.AddElem(strElem, strInfo);

		strElem.Format(_T("SizeY%d"),i);
		strInfo.Format(_T("%f"), m_TrainGuiParam.m_SearchRect.at(i)->GetSize().GetY());
		cXMLConfigurator.AddElem(strElem, strInfo);
		cXMLConfigurator.LeaveElem();
	
	}
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
	// search rect
	
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
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bTrainAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("TrainAffineRectVisible"), strInfo);
// 	// model point visible  
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bModelPointVisible);
// 	cXMLConfigurator.AddElem(_T("ModelPointVisible"), strInfo);
// 	// search rect visible 
// 	strInfo.Format(_T("%d"), m_TrainGuiParam.m_bSearchRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
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

// 	// 将SearchGuiParam的参数，放在SearchDataParam括号中 
// 	cXMLConfigurator.LeaveElem();
// 
// 	/////////////SearchGuiParam//////////////
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
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bAffineRectVisible);
// 	cXMLConfigurator.AddElem(_T("SearchRectVisible"), strInfo);
// 	// model point visible 
// 	strInfo.Format(_T("%d"), m_SearchGuiParam.m_bModelPointVisible);
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

	cXMLConfigurator.LeaveElem();
	bool bRet = cXMLConfigurator.Save(strDir+_T("Model.xml"));
	if (!bRet)
	{
		return false;
	}

	////////////////////////////存储模板图像//////////////////////////
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
	
	////////////////////////////存储掩模图像//////////////////////////
	//if (m_TrainDataParam.m_bMaskEnable && m_ImageMask.IsValid())
	//{
	//	hStatus = cDIB.Init(m_ImageMask);
	//	if (!IsSuccess(hStatus))
	//	{
	//		return false;
	//	}
	//	hStatus = cDIB.Write(strDir + _T("Mask.bmp"));
	//	if (!IsSuccess(hStatus))
	//	{
	//		return false;
	//	}
	//}
	
	////////////////////////////存储模板ptq文件//////////////////////////
	////bRet = m_AreaInspectTool.SaveTrainModelToFile(strDir + _T("Model.ptq"));
	//bRet = m_AreaInspectTool.SaveTrainModelToBinaryFile(strDir + _T("Model.ptq"));
	//if (!bRet)
	//{
	//	return false;
	//}


	return true;
}	
	
// 更新训练控件数据信息	
bool CAreaInspectPickTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
// 	m_tmpTrainGuiParam.m_SearchRect.resize(m_tmpTrainDataParam.m_nInsAreaNum);
// 	m_GuiSearchRect.resize(m_tmpTrainDataParam.m_nInsAreaNum);

	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_Hand1SearchRect	= m_GuiHand1SearchRect.GetRect();
		m_tmpTrainGuiParam.m_Hand2SearchRect	= m_GuiHand2SearchRect.GetRect();
		for (int i=0;i<m_GuiSearchRect.size();i++)
		{
			*m_tmpTrainGuiParam.m_SearchRect.at(i)= m_GuiSearchRect.at(i)->GetRect();
		}


	}
	else // 将数据传递到Gui控件上
	{
		ClearTrainGui();
		m_GuiHand1SearchRect.SetLineColor(RGB(0, 0, 255));
		m_GuiHand1SearchRect.SetLineWidth(1);
		m_GuiHand1SearchRect.SetRect(m_tmpTrainGuiParam.m_Hand1SearchRect);

		m_GuiHand2SearchRect.SetLineColor(RGB(0, 0, 255));
		m_GuiHand2SearchRect.SetLineWidth(1);
		m_GuiHand2SearchRect.SetRect(m_tmpTrainGuiParam.m_Hand2SearchRect);

		for (int i=0;i<m_GuiSearchRect.size();i++)
		{
			if (m_GuiSearchRect.at(i)!=NULL)
			{
				delete m_GuiSearchRect.at(i);
				m_GuiSearchRect.at(i)= NULL;
			}
		}
		m_GuiSearchRect.clear();
		
		

		for (int i=0;i<m_tmpTrainGuiParam.m_SearchRect.size();i++)
		{
			scGuiRect * tempGuiRect = new scGuiRect;
			CString str;
			tempGuiRect->SetCenterWidthHeight(250+i*100, 200, 50, 50);
			str.Format(_T("ACF检测%d"),i+1);
			tempGuiRect->SetLabel(str);
			tempGuiRect->SetLabelVisible(TRUE);
			m_GuiSearchRect.push_back(tempGuiRect);
			m_GuiSearchRect.at(i)->SetLineColor(m_tmpTrainGuiParam.m_LineColor.at(i));
			m_GuiSearchRect.at(i)->SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth.at(i));
			m_GuiSearchRect.at(i)->SetRect(*m_tmpTrainGuiParam.m_SearchRect.at(i));
		}

	}

	return true;
}

bool CAreaInspectPickTool::SaveTrainGuiData()
{
	return UpdateTrainGuiData(true);
}

bool CAreaInspectPickTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiSearchResultRectArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor.at(j));
			m_pGuiSearchResultRectArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth.at(j));
		}
	}
	
	return true;
}

// 清除控件及其显示					
bool CAreaInspectPickTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
// 	for (int j=0; j<pGraphicsArray.size(); j++)
// 	{
// 		delete pGraphicsArray[j];
// 	}
	
	pGraphicsArray.clear();

	return true;
}	

// 清除定位工具在Display上的控件
bool CAreaInspectPickTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}

// 清除搜索结果，需先清除结果GUI
bool CAreaInspectPickTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultRectArray);

	m_bInspectFinish	= false;
	m_bInspectOK		= false;

	return true;
}

bool CAreaInspectPickTool::SetGuiByCam(double nWidth,double nHeight)
{

	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;
	double centerx,centery, xlen,ylen;
	m_GuiHand1SearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiHand1SearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_GuiHand2SearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiHand2SearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);


	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		//训练区域
		double centerx,centery, xlen,ylen, rot,skew;
		if (i<m_GuiTrainRect.size())
		{
			m_GuiTrainRect[i]->GetCenterLengthsRotationSkew(centerx,centery, xlen,ylen, rot,skew);
			m_GuiTrainRect[i]->SetCenterLengthsRotationSkew((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight,rot,skew);
		}

		//搜索区域
		if (i<m_GuiSearchRect.size())
		{
			m_GuiSearchRect[i]->GetCenterWidthHeight(centerx,centery,xlen,ylen);
			m_GuiSearchRect[i]->SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

		}
		//参考点
		if (i<m_GuiModelPoint.size())
		{
			m_GuiModelPoint[i]->GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
			m_GuiModelPoint[i]->SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
		}

	}
	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CAreaInspectPickTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
 		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
 		
 		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));
 		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Hand1SearchRect"));		
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_Hand2SearchRect"));
 
 		for (int i=0;i<m_GuiSearchRect.size();i++)
 		{
 			CString str;
 			str.Format(_T("_SearchRect%d"),i);
 			m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+str);	
 
 		}
	
			
	}

	return true;
}

bool CAreaInspectPickTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		
		for (j=0; j<m_pGuiSearchResultRectArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultRectArray[j]->GetTipText());
		}
	}

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultRectArray);
// 	for (int i=0;i<m_pGuiSearchResultRectArray.size();i++)
// 	{
// 		if (m_pGuiSearchResultRectArray.at(i)!=NULL)
// 		{
// 			delete m_pGuiSearchResultRectArray.at(i);
// 			m_pGuiSearchResultRectArray.at(i)=NULL;
// 		}
// 	}
// 	m_pGuiSearchResultRectArray.clear();
	return true;
}
// 清除vector中的Gui
bool CAreaInspectPickTool::ClearTrainVectorGui()
{
	for (int i=0;i<m_GuiSearchRect.size();i++)
	{
		if (m_GuiSearchRect.at(i)!=NULL)
		{
			delete m_GuiSearchRect.at(i);
			m_GuiSearchRect.at(i)= NULL;
		}
	}
	m_GuiSearchRect.clear();

	for (int i=0;i<m_GuiTrainRect.size();i++)
	{
		if (m_GuiTrainRect.at(i)!=NULL)
		{
			delete m_GuiTrainRect.at(i);
			m_GuiTrainRect.at(i)= NULL;
		}
	}
	m_GuiTrainRect.clear();

	for (int i=0;i<m_GuiModelPoint.size();i++)
	{
		if (m_GuiModelPoint.at(i)!=NULL)
		{
			delete m_GuiModelPoint.at(i);
			m_GuiModelPoint.at(i)= NULL;
		}
	}
	m_GuiModelPoint.clear();
	return true;
}

// 获取训练模板原始图像
bool CAreaInspectPickTool::GetPatternImage(cpImage &ImagePattern)
{
// 	bool bRet = m_AreaInspectTool.GetTrainModelImage(ImagePattern);
// 	if (!bRet)
// 	{
// 		return false;
// 	}
	ImagePattern = m_ImageTrain;
	return true;
		

}

// 恢复训练参数
bool CAreaInspectPickTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}
											
// 恢复搜索参数		
bool CAreaInspectPickTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}


bool CAreaInspectPickTool::ApplyStatisticData()
{
// 	if (m_tmpTrainDataParam.m_lStastisticNumber < 3)
// 	{
// 		return false;
// 	}
// 
// 	m_tmpTrainDataParam.m_nMinAcceptArea = m_tmpTrainDataParam.m_lStatisticACFAreaMin;
// 	m_tmpTrainDataParam.m_nMaxAcceptArea = m_tmpTrainDataParam.m_lStatisticACFAreaMax;
// 	m_tmpTrainDataParam.m_dMaxDefectArea = m_tmpTrainDataParam.m_lStatisticHoleAreaMax;

	return true;
}

bool CAreaInspectPickTool::ResetStatisticData()
{
// 	m_tmpTrainDataParam.m_lStatisticACFAreaMin = LONG_MAX;
// 	m_tmpTrainDataParam.m_lStatisticACFAreaMax = 0;
// 	m_tmpTrainDataParam.m_lStatisticHoleAreaMax = 10;
// 	m_tmpTrainDataParam.m_lStastisticNumber = 0;

	return true;
}

void CAreaInspectPickTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF Inspect_%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiSearchRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF Inspect_%d"),i+1);
			m_GuiSearchRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiModelPoint.size();i++)
		{
			CString str;
			str.Format(_T("Reference Point_%d"),i+1);
			m_GuiModelPoint.at(i)->SetLabel(str);
		}
		m_GuiHand1SearchRect.SetLabel(_T("Hand1 Train Area"));
		m_GuiHand2SearchRect.SetLabel(_T("Hand2 Train Area"));
		break;
	case 1:
		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiSearchRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiSearchRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiModelPoint.size();i++)
		{
			CString str;
			str.Format(_T("参考点_%d"),i+1);
			m_GuiModelPoint.at(i)->SetLabel(str);
		}
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		break;
	default:
		for (int i=0;i<m_GuiTrainRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiTrainRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiSearchRect.size();i++)
		{
			CString str;
			str.Format(_T("ACF检测%d"),i+1);
			m_GuiSearchRect.at(i)->SetLabel(str);
		}
		for (int i=0;i<m_GuiModelPoint.size();i++)
		{
			CString str;
			str.Format(_T("参考点_%d"),i+1);
			m_GuiModelPoint.at(i)->SetLabel(str);
		}
		m_GuiHand1SearchRect.SetLabel(_T("手爪1 搜索区域"));
		m_GuiHand2SearchRect.SetLabel(_T("手爪2 搜索区域"));
		break;
	}


}	

void  CAreaInspectPickTool::ResizeToolsAllParam(int nNewToolNum)
{
	if (nNewToolNum<0 || (nNewToolNum == m_tmpTrainDataParam.m_nInsAreaNum))
	{
		return;
	}
	//trainDataParam
	
	m_tmpTrainDataParam.m_dMinMean.resize(nNewToolNum,0.0);
	m_tmpTrainDataParam.m_dMaxMean.resize(nNewToolNum,255.0);
	m_tmpTrainDataParam.m_dMinStdDev.resize(nNewToolNum,0.0);
	m_tmpTrainDataParam.m_dMaxStdDev.resize(nNewToolNum,255.0);
	m_tmpTrainDataParam.m_bMeanInvalid.resize(nNewToolNum,TRUE);
	m_tmpTrainDataParam.m_bStdDevInvalid.resize(nNewToolNum,TRUE);
	m_tmpTrainDataParam.m_bMeanInvert.resize(nNewToolNum,FALSE);
	m_tmpTrainDataParam.m_bStdDevInvert.resize(nNewToolNum,FALSE);
	m_tmpTrainDataParam.m_strPositionToolName.resize(nNewToolNum,_T(""));

	//m_tmpTrainGuiParam

	m_tmpTrainGuiParam.m_LineColor.resize(nNewToolNum,RGB(0, 0, 255));
	m_tmpTrainGuiParam.m_nLineWidth.resize(nNewToolNum,1);
	m_tmpTrainGuiParam.m_bSearchRectVisible.resize(nNewToolNum,TRUE);

	//m_tmpSearchGuiParam
	m_tmpSearchGuiParam.m_LineColor.resize(nNewToolNum,RGB(0, 0, 255));
	m_tmpSearchGuiParam.m_nLineWidth.resize(nNewToolNum,1);
	m_tmpSearchGuiParam.m_bAffineRectVisible.resize(nNewToolNum,TRUE);
	m_tmpSearchGuiParam.m_bModelPointVisible.resize(nNewToolNum,TRUE);

	m_dCurMean.resize(nNewToolNum,0);
	m_dCurStdDev.resize(nNewToolNum,0);

	if (nNewToolNum< m_tmpTrainDataParam.m_nInsAreaNum)
	{
		for (int i=m_tmpTrainDataParam.m_nInsAreaNum-1;i>=nNewToolNum;i-- )
		{
			if (m_GuiTrainRect.at(i)!=NULL)
			{
				delete m_GuiTrainRect.at(i);
				m_GuiTrainRect.at(i) = NULL;
			}
			m_GuiTrainRect.erase(m_GuiTrainRect.begin()+i);

			if (m_GuiSearchRect.at(i)!=NULL)
			{
				delete m_GuiSearchRect.at(i);
				m_GuiSearchRect.at(i) = NULL;
			}
			m_GuiSearchRect.erase(m_GuiSearchRect.begin()+i);

			if (m_GuiModelPoint.at(i)!=NULL)
			{
				delete m_GuiModelPoint.at(i);
				m_GuiModelPoint.at(i) = NULL;
			}
			m_GuiModelPoint.erase(m_GuiModelPoint.begin()+i);

	
			if (m_tmpTrainGuiParam.m_SearchRect.at(i)!=NULL)
			{
				delete m_tmpTrainGuiParam.m_SearchRect.at(i);
				m_tmpTrainGuiParam.m_SearchRect.at(i) = NULL;
			}
			m_tmpTrainGuiParam.m_SearchRect.erase(m_tmpTrainGuiParam.m_SearchRect.begin()+i);
		}


	}
	else
	{
		CString str;
		for (int i=0;i<nNewToolNum;i++)
		{
			if (i>=m_tmpTrainDataParam.m_nInsAreaNum)
			{
				scGuiAffineRect *tempGuiTrainRect = new scGuiAffineRect;							
				tempGuiTrainRect->SetCenterLengthsRotationSkew(250+i*100, 200, 50, 50, 0, 0);
				str.Format(_T("ACF检测%d"),i+1);
				tempGuiTrainRect->SetLabel(str);
				tempGuiTrainRect->SetLabelVisible(TRUE);
				m_GuiTrainRect.push_back(tempGuiTrainRect);
				

				scGuiRect* tempGuiSearchRect = new scGuiRect;
				scRect* tempRect = new scRect;	
				tempGuiSearchRect->SetCenterWidthHeight(250+i*100, 200, 50, 50);
				str.Format(_T("ACF检测%d"),i+1);
				tempGuiSearchRect->SetLabel(str);
				tempGuiSearchRect->SetLabelVisible(TRUE);
				m_GuiSearchRect.push_back(tempGuiSearchRect);
				m_tmpTrainGuiParam.m_SearchRect.push_back(tempRect);
				*(m_tmpTrainGuiParam.m_SearchRect.at(i))= m_GuiSearchRect.at(i)->GetRect();


				scGuiCross* tempGuiModelPoint = new scGuiCross;
				tempGuiModelPoint->SetCenterXYWidthHeight(250+i*100, 200);
				str.Format(_T("参考点%d"),i+1);
				tempGuiModelPoint->SetLabel(str);
				tempGuiModelPoint->SetLabelVisible(TRUE);
				m_GuiModelPoint.push_back(tempGuiModelPoint);
			}
		
		}
		
	}

	 m_tmpTrainDataParam.m_nInsAreaNum = nNewToolNum;

	 m_TrainDataParam						= m_tmpTrainDataParam;
	 m_TrainGuiParam							= m_tmpTrainGuiParam;
	 m_SearchDataParam						= m_tmpSearchDataParam;
	 m_SearchGuiParam						= m_tmpSearchGuiParam;

	
}

CString	CAreaInspectPickTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d "), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case -99:							//训练未成功
				strErrInfo += _T("The Train is NG");
				break;
			case -1:
				strErrInfo += _T("GetProcessImage NG");
				break;
			case -2:
				strErrInfo += _T("Inspect NG");
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
			case -99:							//训练未成功
				strErrInfo += _T("训练未成功");
				break;
			case -1:
				strErrInfo += _T("图像转换失败");
				break;
			case -2:
				strErrInfo += _T("检测失败");
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
BOOL	CAreaInspectPickTool::PointInRect(sc2Vector point,scRect rect)
{
	if(point.GetX()>rect.GetUL().GetX()//左上
		&&point.GetX()<rect.GetUR().GetX()//右上
		&&point.GetY()>rect.GetUL().GetY()//左上
		&&point.GetY()<rect.GetLL().GetY()//左下
		)	
		return TRUE;
	return FALSE;
}