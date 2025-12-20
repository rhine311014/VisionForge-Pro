#include "stdafx.h"
#include "PatternDoubleSearchTool.h"
#include "svXMLConfigurator.h"
#include "svDIB.h"
#include "svGuiAffineRect.h"
#include "svImageSharpness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////  

//无参构造函数，创建对象，对对象初始化
CPatternDoubleSearchTool::CPatternDoubleSearchTool()
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;

	//工具1
	m_GuiTrainRect1.SetCenterWidthHeight(350, 240, 100, 100);
	//m_GuiTrainRect1.SetLabel(_T("训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiSearchRect1.SetCenterWidthHeight(350, 240, 652, 380);
	//m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect1.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(350, 240, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	//工具2
	m_GuiTrainRect2.SetCenterWidthHeight(400, 250, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(400, 250, 652, 380);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(400, 250, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);
	
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect1.SetLabel(_T("Train Area1"));
		m_GuiSearchRect1.SetLabel(_T("Search Area1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1"));

		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
	CreateGUID(m_strKeyText);

	m_tmpTrainDataParam.m_Accuracy1				= m_PatternSearchTool1.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale1			= m_PatternSearchTool1.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow1			= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh1			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable1			= false; // 默认不采用掩模图像

	m_tmpTrainDataParam.m_Accuracy2				= m_PatternSearchTool2.GetAccuracy();
	m_tmpTrainDataParam.m_CoarseScale2			= m_PatternSearchTool2.GetCoarsestScale();
	m_tmpTrainDataParam.m_dAngleLow2			= -10.0;
	m_tmpTrainDataParam.m_dAngleHigh2			= 10.0;
	m_tmpTrainDataParam.m_bMaskEnable2			= false; // 默认不采用掩模图像

	m_tmpTrainGuiParam.m_TrainRect1				= m_GuiTrainRect1.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint1.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect1			= m_GuiSearchRect1.GetRect();
	
	m_tmpTrainGuiParam.m_TrainRect2				= m_GuiTrainRect2.GetRect();
	m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
	m_tmpTrainGuiParam.m_SearchRect2			= m_GuiSearchRect2.GetRect();

	m_tmpTrainGuiParam.m_nLineWidth				= m_GuiTrainRect1.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor				= m_GuiTrainRect1.GetLineColor();

	m_tmpSearchDataParam.m_nSearchNum1			= 1;
	m_tmpSearchDataParam.m_dAcceptThre1			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre1		= 0.8;
// 	m_tmpSearchDataParam.m_bEnableSharpScore1	= false;
// 	m_tmpSearchDataParam.m_dSharpThre1			= 0.5;

	m_tmpSearchDataParam.m_nSearchNum2			= 1;
	m_tmpSearchDataParam.m_dAcceptThre2			= 0.6;
	m_tmpSearchDataParam.m_dConfusionThre2		= 0.8;
// 	m_tmpSearchDataParam.m_bEnableSharpScore2	= false;
// 	m_tmpSearchDataParam.m_dSharpThre2			= 0.5;

	m_tmpSearchGuiParam.m_nLineWidth			= m_GuiSearchRect1.GetLineWidth();
	m_tmpSearchGuiParam.m_LineColor				= m_GuiSearchRect1.GetLineColor();

	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

//拷贝构造函数，根据存在的对象赋值出一个新的对象，此处完成数据成员的复制
CPatternDoubleSearchTool::CPatternDoubleSearchTool(CPatternDoubleSearchTool &CPatternDoubleSearchTool)
{
	if (CPatternDoubleSearchTool.GetSearchToolType() != ePatternDouleSearchTool)
	{
		return;
	}

	//初始化
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;

	//工具1
	m_GuiTrainRect1.SetCenterWidthHeight(250, 200, 100, 100);
	//m_GuiTrainRect1.SetLabel(_T("训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiSearchRect1.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect1.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	//工具2
	m_GuiTrainRect2.SetCenterWidthHeight(850, 200, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(850, 200, 400, 300);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(850, 200, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect1.SetLabel(_T("Train Area1"));
		m_GuiSearchRect1.SetLabel(_T("Search Area1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1"));

		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
	CreateGUID(m_strKeyText);

	//赋值
    m_CamHeight						= CPatternDoubleSearchTool.m_CamHeight;
    m_CamWidth						= CPatternDoubleSearchTool.m_CamWidth;
	m_TrainDataParam				= CPatternDoubleSearchTool.m_TrainDataParam;
	m_TrainGuiParam					= CPatternDoubleSearchTool.m_TrainGuiParam;
	m_SearchDataParam				= CPatternDoubleSearchTool.m_SearchDataParam;
	m_SearchGuiParam				= CPatternDoubleSearchTool.m_SearchGuiParam;
	m_TrainResult					= CPatternDoubleSearchTool.m_TrainResult;
	m_ImageTrain					= CPatternDoubleSearchTool.m_ImageTrain;
	m_ImageMask1					= CPatternDoubleSearchTool.m_ImageMask1;
	m_ImageMask2					= CPatternDoubleSearchTool.m_ImageMask2;
	m_bTrainGuiShow					= CPatternDoubleSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow			= CPatternDoubleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK						= CPatternDoubleSearchTool.m_bTrainOK;
	m_pGuiDisplay					= CPatternDoubleSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	CPatternDoubleSearchTool.UpdateSearchGuiData(true);
	CPatternDoubleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&CPatternDoubleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&CPatternDoubleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&CPatternDoubleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&CPatternDoubleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(CPatternDoubleSearchTool.m_tmpImageTrain);
	SetMaskImage(CPatternDoubleSearchTool.m_tmpImageMask1);
	SetMaskImage2(CPatternDoubleSearchTool.m_tmpImageMask2);

	if (m_bTrainOK)
	{
		m_PatternSearchTool1 = CPatternDoubleSearchTool.m_PatternSearchTool1;
		m_PatternSearchTool2 = CPatternDoubleSearchTool.m_PatternSearchTool2;
	}
}

//赋值构造函数，类似于拷贝构造函数，将等号右边的本类对象的值复制给等号左边的对象
CPatternDoubleSearchTool& CPatternDoubleSearchTool::operator =(CPatternDoubleSearchTool &cPatternDoubleSearchTool)
{
	// 首先检测右边对象是否就是左边对象本身，若是本身,则直接返回 
	if (this == &cPatternDoubleSearchTool)
	{
		return *this;
	}

	if (cPatternDoubleSearchTool.GetSearchToolType() != ePatternDouleSearchTool)
	{
		return *this;
	}

	// 清空
	ClearAllGui();
	ClearResult();

	//初始化
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;

	//工具1
	m_GuiTrainRect1.SetCenterWidthHeight(250, 200, 100, 100);
	//m_GuiTrainRect1.SetLabel(_T("训练区域1"));
	m_GuiTrainRect1.SetLabelVisible(TRUE);
	m_GuiSearchRect1.SetCenterWidthHeight(250, 200, 400, 300);
	//m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
	m_GuiSearchRect1.SetLabelVisible(TRUE);
	m_GuiModelPoint1.SetCenterXYWidthHeight(250, 200, 40, 40);
	//m_GuiModelPoint1.SetLabel(_T("参考点1"));
	m_GuiModelPoint1.SetLabelVisible(TRUE);

	//工具2
	m_GuiTrainRect2.SetCenterWidthHeight(850, 200, 100, 100);
	//m_GuiTrainRect2.SetLabel(_T("训练区域2"));
	m_GuiTrainRect2.SetLabelVisible(TRUE);
	m_GuiSearchRect2.SetCenterWidthHeight(850, 200, 400, 300);
	//m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
	m_GuiSearchRect2.SetLabelVisible(TRUE);
	m_GuiModelPoint2.SetCenterXYWidthHeight(850, 200, 40, 40);
	//m_GuiModelPoint2.SetLabel(_T("参考点2"));
	m_GuiModelPoint2.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect1.SetLabel(_T("Train Area1"));
		m_GuiSearchRect1.SetLabel(_T("Search Area1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1"));

		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}
	CreateGUID(m_strKeyText);

    m_CamHeight								= cPatternDoubleSearchTool.m_CamHeight;
    m_CamWidth								= cPatternDoubleSearchTool.m_CamWidth;
	//赋值
	m_TrainDataParam				= cPatternDoubleSearchTool.m_TrainDataParam;
	m_TrainGuiParam					= cPatternDoubleSearchTool.m_TrainGuiParam;
	m_SearchDataParam				= cPatternDoubleSearchTool.m_SearchDataParam;
	m_SearchGuiParam				= cPatternDoubleSearchTool.m_SearchGuiParam;
	m_TrainResult					= cPatternDoubleSearchTool.m_TrainResult;
	m_ImageTrain					= cPatternDoubleSearchTool.m_ImageTrain;
	m_ImageMask1					= cPatternDoubleSearchTool.m_ImageMask1;
	m_ImageMask2					= cPatternDoubleSearchTool.m_ImageMask2;
	m_bTrainGuiShow					= cPatternDoubleSearchTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow			= cPatternDoubleSearchTool.m_bSearchResultGuiShow;
	m_bTrainOK						= cPatternDoubleSearchTool.m_bTrainOK;
	m_pGuiDisplay					= cPatternDoubleSearchTool.m_pGuiDisplay;

	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cPatternDoubleSearchTool.UpdateSearchGuiData(true);
	cPatternDoubleSearchTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cPatternDoubleSearchTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cPatternDoubleSearchTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cPatternDoubleSearchTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cPatternDoubleSearchTool.m_tmpSearchGuiParam);
	SetTrainImage(cPatternDoubleSearchTool.m_tmpImageTrain);
	SetMaskImage(cPatternDoubleSearchTool.m_tmpImageMask1);
	SetMaskImage2(cPatternDoubleSearchTool.m_tmpImageMask2);

	if (m_bTrainOK)
	{
		m_PatternSearchTool1 = cPatternDoubleSearchTool.m_PatternSearchTool1;
		m_PatternSearchTool2 = cPatternDoubleSearchTool.m_PatternSearchTool2;
	}

	return *this;
}

//析构函数，调用对象完成之后自动执行释放资源
CPatternDoubleSearchTool::~CPatternDoubleSearchTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}

//获取工具类型
CSearchToolType CPatternDoubleSearchTool::GetSearchToolType()
{
	return ePatternDouleSearchTool;
}

//设置显示控件指针
bool CPatternDoubleSearchTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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

//获取显示控件指针
scGuiDisplay* CPatternDoubleSearchTool::GetDisplay()
{
	return m_pGuiDisplay;
}

//设置Gui图形关键字
bool CPatternDoubleSearchTool::SetGuiKeyText(CString strGuiKeyText)
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

//获取Gui图形关键字
bool CPatternDoubleSearchTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

//更新训练界面显示
bool CPatternDoubleSearchTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

//是否显示训练结果界面
bool CPatternDoubleSearchTool::ShowTrainResultGui(bool bShow)
{
	return false;
}

// 更新搜索界面显示									
bool CPatternDoubleSearchTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CPatternDoubleSearchTool::UpdateTrainGuiDisplay()
{	
	m_GuiTrainRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect1.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	m_GuiTrainRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);
	m_GuiModelPoint2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bModelPointVisible);
	m_GuiSearchRect2.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bSearchRectVisible);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect1, m_strKeyText+_T("_TrainRect1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint1, m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect1, m_strKeyText+_T("_SearchRect1"));	

		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect2, m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiModelPoint2, m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiSearchRect2, m_strKeyText+_T("_SearchRect2"));	

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CPatternDoubleSearchTool::UpdateSearchResultGuiDisplay()							
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

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CPatternDoubleSearchTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();
	UpdateSearchResultGuiDisplay();
	return true;
}

// 设置训练Data参数	
bool CPatternDoubleSearchTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}
	m_tmpTrainDataParam = *((CPatternDoubleSearchTrainDataParam*)pTrainDataParam);
	return true;
}

// 获取训练Data参数									
bool CPatternDoubleSearchTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	*((CPatternDoubleSearchTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CPatternDoubleSearchTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CPatternDoubleSearchTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CPatternDoubleSearchTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CPatternDoubleSearchTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CPatternDoubleSearchTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;
	return true;
}

// 获取训练原始图像									
bool CPatternDoubleSearchTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;
	return true;
}

// 设置工具1训练原始图像掩膜									
bool CPatternDoubleSearchTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask1 = ImageMask;
	return true;
}

// 获取工具1训练原始图像掩膜								
bool CPatternDoubleSearchTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask1;

	return true;
}

// 设置工具2训练原始图像掩膜									
bool CPatternDoubleSearchTool::SetMaskImage2(const cpImage &ImageMask)
{
	m_tmpImageMask2 = ImageMask;
	return true;
}

// 获取工具2训练原始图像掩膜								
bool CPatternDoubleSearchTool::GetMaskImage2(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask2;

	return true;
}

// 执行训练											
bool CPatternDoubleSearchTool::Train()
{
	m_bTrainOK = false;

	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	// 设置工具1训练参数
	if (!m_PatternSearchTool1.SetAccuracy(m_tmpTrainDataParam.m_Accuracy1))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	if (!m_PatternSearchTool1.SetCoarsestScale(m_tmpTrainDataParam.m_CoarseScale1))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	// 必须在训练时设置角度
	if (fabs(m_tmpTrainDataParam.m_dAngleLow1 - m_tmpTrainDataParam.m_dAngleHigh1) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool1.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow1)))
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return false;
		}
	}
	else if (!m_PatternSearchTool1.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow1), scDegree(m_tmpTrainDataParam.m_dAngleHigh1), scDegree(360)))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	// 如果是彩色图像(像素大小!=1)，则进行处理获得灰度图像
	BOOL bIsProcessImage = FALSE;
	if (1 != m_tmpImageTrain.PixelSize())
	{
		bIsProcessImage = TRUE;
	}

	cpImage tmpImage;
	cpImage tmpImageMask1;
	cpImage tmpImageMask2;
	if (bIsProcessImage)
	{
		tmpImage.CreateImageBuffer(m_tmpImageTrain.Width(),m_tmpImageTrain.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageTrain,tmpImage);

		tmpImageMask1.CreateImageBuffer(m_tmpImageMask1.Width(),m_tmpImageMask1.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask1,tmpImageMask1);

		tmpImageMask2.CreateImageBuffer(m_tmpImageMask2.Width(),m_tmpImageMask2.Height(),epGray8);
		GetProcessImage(&m_tmpTrainDataParam,m_tmpImageMask2,tmpImageMask2);
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
		if (m_bTrainOK)
		{
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return m_bTrainOK;
		}
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
		m_bTrainOK = bRet;
		if (m_bTrainOK)
		{
		}
		else
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return m_bTrainOK;
		}
		//return bRet;
	}


	// 设置工具2训练参数
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
	if (fabs(m_tmpTrainDataParam.m_dAngleLow2 - m_tmpTrainDataParam.m_dAngleHigh2) < DBL_EPSILON) // 相同时需设置成Nominal
	{
		if (!m_PatternSearchTool2.SetDofNominalRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2)))
		{
			m_nResultErrNumber=-2;       //区域定位2失败
			return false;
		}
	}
	else if (!m_PatternSearchTool2.SetDofZoneRotate(scDegree(m_tmpTrainDataParam.m_dAngleLow2), scDegree(m_tmpTrainDataParam.m_dAngleHigh2), scDegree(360)))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return false;
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
		}
		else
		{
			m_nResultErrNumber=-2;       //区域定位2失败
			return m_bTrainOK;
		}
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
		}
		else
		{
			m_nResultErrNumber=-2;       //区域定位2失败
			return m_bTrainOK;
		}
	}
	if (m_bTrainOK)
	{
		if (m_tmpTrainDataParam.m_bMaskEnable1)
		{
			m_ImageMask1      = m_tmpImageMask1;
		}

		if (m_tmpTrainDataParam.m_bMaskEnable2)
		{
			m_ImageMask2      = m_tmpImageMask2;
		}

		sc2Vector cModelOrigin;
		m_PatternSearchTool1.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX1 = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY1 = cModelOrigin.GetY();

		m_PatternSearchTool2.GetPatternOrigin(cModelOrigin);
		m_tmpTrainResult.m_dModelX2 = cModelOrigin.GetX();
		m_tmpTrainResult.m_dModelY2 = cModelOrigin.GetY();

		// 获取搜索区域的位置及尺寸并计算第一个定位点与第二个搜索区域的位置左上角点距离
		sc2Vector cSearchOriginLU;
		//m_GuiSearchRect2.GetCenterWidthHeight(cSearchOriginCenter,m_tmpTrainResult.m_scRectSearchSize);
		m_GuiSearchRect2.GetXYWidthHeight(cSearchOriginLU,m_tmpTrainResult.m_scRectSearchSize);

		m_tmpTrainResult.m_dMarkToSearch2X = cSearchOriginLU.GetX() - m_tmpTrainResult.m_dModelX1;
		m_tmpTrainResult.m_dMarkToSearch2Y = cSearchOriginLU.GetY() - m_tmpTrainResult.m_dModelY1;
		m_bTrainOK = true;

		//计算工具1清晰度
		scImageSharpness tool1;
		double dScoreSharp1 = 0;
		bool bSharpExecute =false;
		if(bIsProcessImage)
		{
			bSharpExecute = tool1.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainRect1,dScoreSharp1);
		}
		else
		{
			bSharpExecute = tool1.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect1,dScoreSharp1);
		}
		if (bSharpExecute)
		{
			m_tmpTrainResult.m_dSharpness1 = dScoreSharp1;
		}
		else
		{
			m_tmpTrainResult.m_dSharpness1 = 1;
		}
		//计算工具2清晰度
		scImageSharpness tool2;
		double dScoreSharp2 = 0;
		
		if(bIsProcessImage)
		{
			bSharpExecute = tool2.Execute(tmpImage,m_tmpTrainGuiParam.m_TrainRect2,dScoreSharp2);
		}
		else
		{
			bSharpExecute = tool2.Execute(m_tmpImageTrain,m_tmpTrainGuiParam.m_TrainRect2,dScoreSharp2);
		}
		if (bSharpExecute)
		{
			m_tmpTrainResult.m_dSharpness2 = dScoreSharp2;
		}
		else
		{
			m_tmpTrainResult.m_dSharpness2 = 1;
		}

		m_ImageTrain     = m_tmpImageTrain;
		m_TrainDataParam = m_tmpTrainDataParam;
		m_TrainGuiParam  = m_tmpTrainGuiParam;
		m_TrainResult    = m_tmpTrainResult;
		m_bTrainOK = true;
	}
	return m_bTrainOK;
}

// 获取工具1的训练结果图像														
bool CPatternDoubleSearchTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	bool bRet = m_PatternSearchTool1.GetPatternImage(m_ImageTrainResult1);
	if (!bRet)
	{
		return false;
	}

	ImageTrainResult = m_ImageTrainResult1;

	return true;
}	

// 获取工具2的训练结果图像														
bool CPatternDoubleSearchTool::GetTrainResultImage2(cpImage &ImageTrainResult)
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
bool CPatternDoubleSearchTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	*((CPatternDouleSearchTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}							

// 是否训练成功
bool CPatternDoubleSearchTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CPatternDoubleSearchTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CPatternDoubleSearchSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CPatternDoubleSearchTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	*((CPatternDoubleSearchSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CPatternDoubleSearchTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CPatternDoubleSearchSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CPatternDoubleSearchTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != ePatternDouleSearchTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CPatternDoubleSearchSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CPatternDoubleSearchTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;       // 训练未成功
		return false;
	}	
	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	// 设置工具1的搜索个数
	if (!m_PatternSearchTool1.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum1))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	// 设置工具1设置接收阈值
	if (!m_PatternSearchTool1.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre1))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}
	if (!m_PatternSearchTool1.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre1>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre1))
	{
		m_nResultErrNumber=-1;       //区域定位1失败
		return false;
	}

	// 设置工具2的搜索个数
	if (!m_PatternSearchTool2.SetAcceptNum(m_tmpSearchDataParam.m_nSearchNum2))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return false;
	}

	// 设置工具2设置接收阈值
	if (!m_PatternSearchTool2.SetAcceptThreshold(m_tmpSearchDataParam.m_dAcceptThre2))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
		return false;
	}
	if (!m_PatternSearchTool2.SetConfusionThreshold(m_tmpSearchDataParam.m_dConfusionThre2>1.0 ?1.0:m_tmpSearchDataParam.m_dConfusionThre2))
	{
		m_nResultErrNumber=-2;       //区域定位2失败
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

	bool bRet = false;
	if (bIsProcessImage)
	{
		bRet = m_PatternSearchTool1.Execute(tmpImage, m_TrainGuiParam.m_SearchRect1);		
	}
	else
	{
		bRet = m_PatternSearchTool1.Execute(ImageSearch, m_TrainGuiParam.m_SearchRect1);
	}

	if (bRet)
	{
		// 获取结果
		int nResultNum1 = m_PatternSearchTool1.GetResultNum();
		if (nResultNum1 <= 0)
		{
			m_nResultErrNumber=-1;       //区域定位1失败
			return false;
		}
		for (int i=0; i<nResultNum1 && i<MAXRESNUM; i++)
		{
			scPatternSearchRotResult tmpPatternResult1 = m_PatternSearchTool1.GetResult(i);

			//计算清晰度
// 			double dScoreSharp = 0;
// 			if (m_tmpSearchDataParam.m_bEnableSharpScore1)
// 			{
// 				scImageSharpness tool;
// 				if (tool.Execute(ImageSearch,tmpPatternResult.m_MatchRect,dScoreSharp))
// 				{
// 					dScoreSharp = dScoreSharp / m_TrainResult.m_dSharpness1;
// 
// 					if (dScoreSharp < m_tmpSearchDataParam.m_dSharpThre1)
// 					{
// 						continue;
// 					}
// 				}
// 			}

// 			CSearchResult searchResult;
// 			searchResult.m_dPosX = tmpPatternResult.GetPosition().GetX();
// 			searchResult.m_dPosY = tmpPatternResult.GetPosition().GetY();
// 			searchResult.m_dAngle = tmpPatternResult.GetRotate().ToDouble();
// 			searchResult.m_dScore = tmpPatternResult.GetScore();
// 
// 			// 将第一个工具搜索的结果通过训练参数确定第二个工具的训练区域（通过左上角的点）
// 			double dLeft, dtop;
// 			dLeft = searchResult.m_dPosX + m_TrainResult.m_dMarkToSearch2X;
// 			dtop = searchResult.m_dPosY + m_TrainResult.m_dMarkToSearch2Y;
// 
// 			m_TrainGuiParam.m_SearchRect2.Set(dLeft,dtop,m_TrainGuiParam.m_SearchRect2.GetWidth(),m_TrainGuiParam.m_SearchRect2.GetHeight());



			//////////////////////////////////////////////////////////////////////////
			// 根据工具1定位结果，调整工具2搜索区域

			// 变换矩阵
			sc2Vector pos = tmpPatternResult1.GetPosition();
			sc2Vector tmpModelPoint;
			double dRotation;
			double dTransX;
			double dTransY;
			m_PatternSearchTool1.GetPatternOrigin(tmpModelPoint);
			dRotation = scRadian(tmpPatternResult1.GetRotate()).ToDouble();
			dTransX = pos.GetX() - (cos(dRotation) * tmpModelPoint.GetX()  - sin(dRotation) * tmpModelPoint.GetY());
			dTransY = pos.GetY() - (sin(dRotation) * tmpModelPoint.GetX()  + cos(dRotation) * tmpModelPoint.GetY());

			// 变换前工具2搜索区域
			scRect tmpRectBefore = m_TrainGuiParam.m_SearchRect2;

			// 变换前工具2搜索区域中心点
			double dXBefore = (tmpRectBefore.GetLeft() + tmpRectBefore.GetRight())*0.5;
			double dYBefore = (tmpRectBefore.GetTop() + tmpRectBefore.GetBottom())*0.5;

			// 变换后工具2搜索区域中心点
			double dXAfter = cos(dRotation) * dXBefore  - sin(dRotation) * dYBefore + dTransX;
			double dYAfter = sin(dRotation) * dXBefore  + cos(dRotation) * dYBefore + dTransY;

			// 变换后工具2搜索区域（仿射矩形）
			scAffineRect tmpAffineRect;
			tmpAffineRect.SetCenter(sc2Vector(dXAfter, dYAfter));
			tmpAffineRect.SetXLength(tmpRectBefore.GetWidth());
			tmpAffineRect.SetYLength(tmpRectBefore.GetHeight());
			tmpAffineRect.SetXRotation(scRadian(dRotation));
			tmpAffineRect.SetSkew(scRadian(0));

			// 变换后工具2搜索区域（矩形）
			scRect tmpRect = tmpAffineRect.BoundingBox();

			if(bIsProcessImage)
			{
				// 第二个工具执行搜索
				bRet = m_PatternSearchTool2.Execute(tmpImage, tmpRect/*m_TrainGuiParam.m_SearchRect2*/);
			}
			else
			{
				// 第二个工具执行搜索
				bRet = m_PatternSearchTool2.Execute(ImageSearch, tmpRect/*m_TrainGuiParam.m_SearchRect2*/);
			}
			
			if (bRet)
			{
				// 获取结果
				int nResultNum2 = m_PatternSearchTool2.GetResultNum();
				if (nResultNum2 <= 0)
				{
					m_nResultErrNumber=-2;       //区域定位2失败
					return false;
				}
				for (int j=0; j<nResultNum2 && j<MAXRESNUM; j++)
				{
					scPatternSearchRotResult tmpPatternResult2 = m_PatternSearchTool2.GetResult(j);

					//计算清晰度
// 					double dScoreSharp = 0;
// 					if (m_tmpSearchDataParam.m_bEnableSharpScore2)
// 					{
// 						scImageSharpness tool;
// 						if (tool.Execute(ImageSearch,tmpPatternResult.m_MatchRect,dScoreSharp))
// 						{
// 							dScoreSharp = dScoreSharp / m_TrainResult.m_dSharpness2;
// 
// 							if (dScoreSharp < m_tmpSearchDataParam.m_dSharpThre2)
// 							{
// 								continue;
// 							}
// 						}
// 					}

					CSearchResult searchResult;
					searchResult.m_dPosX = tmpPatternResult2.GetPosition().GetX();
					searchResult.m_dPosY = tmpPatternResult2.GetPosition().GetY();
					searchResult.m_dAngle = tmpPatternResult2.GetRotate().ToDouble();
					searchResult.m_dScore = tmpPatternResult2.GetScore();

					CResultSearchRect resRect;
					resRect.m_searchRect = 	tmpPatternResult1.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);
					resRect.m_searchRect = 	tmpPatternResult2.MatchRegion();
					searchResult.m_vTrainSearchRect.push_back(resRect);

					m_SearchResultArray.push_back(searchResult);

					CString	strTmpKey;
					strTmpKey.Format(_T("%s_ResultRect%d_%d"), m_strKeyText, i, j);
					//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
					int nSize = m_pGuiSearchRectArray.size();
					scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect + nSize;
					pTempAffineRect->SetAffineRect(tmpPatternResult2.MatchRegion());
					pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempAffineRect->SetTipTextEnabled(true);
					pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
					m_pGuiSearchRectArray.push_back(pTempAffineRect);

					strTmpKey.Format(_T("%s_ResultCross%d_%d"), m_strKeyText, i, j);
					//scGuiCross *pTempCross = new scGuiCross;
					nSize = m_pGuiSearchCrossArray.size();
					scGuiCross *pTempCross = m_GuiSearchCross + nSize;
					pTempCross->SetCenterExtent(sc2Vector(tmpPatternResult2.GetPosition().GetX(), tmpPatternResult2.GetPosition().GetY()), sc2Vector(40,40));
					pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
					pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
					pTempCross->SetTipTextEnabled(true);
					pTempCross->SetTipText(strTmpKey);
					CString st;
// 					if (m_tmpSearchDataParam.m_bEnableSharpScore1)
// 					{
// 						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f;SS:%.2f"), i, tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore(),dScoreSharp);
// 					}
// 					else
// 					{
						st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpPatternResult2.GetPosition().GetX(), tmpPatternResult2.GetPosition().GetY(), tmpPatternResult2.GetRotate().ToDouble(),tmpPatternResult2.GetScore());
//					}
					pTempCross->SetLabel(st);
					pTempCross->SetLabelVisible(TRUE);

					m_pGuiSearchCrossArray.push_back(pTempCross);
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
				m_nResultErrNumber=-2;       //区域定位2失败
			}
			//m_SearchResultArray.push_back(searchResult);

// 			CString	strTmpKey;
// 			strTmpKey.Format(_T("%s_ResultRect%d"), m_strKeyText, i);
// 			//scGuiAffineRect *pTempAffineRect = new scGuiAffineRect;
// 			scGuiAffineRect *pTempAffineRect = m_GuiSearchAffineRect+i;
// 			pTempAffineRect->SetAffineRect(tmpPatternResult.MatchRegion());
// 			pTempAffineRect->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			pTempAffineRect->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempAffineRect->SetTipTextEnabled(true);
// 			pTempAffineRect->SetTipText((LPCTSTR)strTmpKey);
// 			m_pGuiSearchRectArray.push_back(pTempAffineRect);
// 
// 			strTmpKey.Format(_T("%s_ResultCross%d"), m_strKeyText, i);
// 			//scGuiCross *pTempCross = new scGuiCross;
// 			scGuiCross *pTempCross = m_GuiSearchCross+i;
// 			pTempCross->SetCenterExtent(sc2Vector(tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY()), sc2Vector(40,40));
// 			pTempCross->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
// 			pTempCross->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
// 			pTempCross->SetTipTextEnabled(true);
// 			pTempCross->SetTipText(strTmpKey);
// 			CString st;
// 			if (m_tmpSearchDataParam.m_bEnableSharpScore1)
// 			{
// 				st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f;SS:%.2f"), i, tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore(),dScoreSharp);
// 			}
// 			else
// 			{
// 				st.Format(_T("%d X:%.1f;Y:%.1f;D:%.3f;S:%.2f"), i, tmpPatternResult.GetPosition().GetX(), tmpPatternResult.GetPosition().GetY(), tmpPatternResult.GetRotate().ToDouble(),tmpPatternResult.GetScore());
// 
// 			}
// 			pTempCross->SetLabel(st);
// 			pTempCross->SetLabelVisible(TRUE);
// 
// 			m_pGuiSearchCrossArray.push_back(pTempCross);
		}	

		if (m_SearchResultArray.size() <= 0)
		{
			return false;
		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
	}
	return bRet;
}

// 获取搜索结果个数										
int CPatternDoubleSearchTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CPatternDoubleSearchTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CPatternDoubleSearchTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != ePatternDouleSearchTool)
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
	bRet = cXMLConfigurator.FindElem(_T("TrainRect1"));
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
bool CPatternDoubleSearchTool::LoadModelFromFile(CString strDir)
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
	CPatternDoubleSearchTrainDataParam	tmpTrainDataParam;
	CPatternDoubleSearchTrainGuiParam		tmpTrainGuiParam;
	CPatternDoubleSearchSearchDataParam	tmpSearchDataParam;
	CPatternDoubleSearchSearchGuiParam	tmpSearchGuiParam;	
	CPatternDouleSearchTrainResult		tmpTrainResult;

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
	if (cSearchToolType != ePatternDouleSearchTool)
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
	bRet = cXMLConfigurator.FindElem(_T("CoarseScale1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_CoarseScale1 = (CoarseScale)_ttol(cXMLConfigurator.GetElemData());
	// Accuracy
	bRet = cXMLConfigurator.FindElem(_T("Accuracy1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_Accuracy1 = (Accuracy)_ttol(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AngleLow1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleLow1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("AngleHigh1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_dAngleHigh1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	// mask enable
	bRet = cXMLConfigurator.FindElem(_T("MaskEnable1"));
	if (!bRet)
	{
		return false;
	}
	tmpTrainDataParam.m_bMaskEnable1 = _ttoi(cXMLConfigurator.GetElemData());

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
	bRet = cXMLConfigurator.FindElem(_T("TrainRect1"));
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
	// search rect
	bRet = cXMLConfigurator.FindElem(_T("SearchRect1"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), tmpTrainGuiParam.m_SearchRect1.GetOrigin().GetX());
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
	tmpTrainGuiParam.m_SearchRect1.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_SearchRect1.SetSize(vPos);
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
	bRet = cXMLConfigurator.FindElem(_T("SearchParam1"));
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
	tmpSearchDataParam.m_nSearchNum1 = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_dAcceptThre1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);

	bRet = cXMLConfigurator.FindElem(_T("ConfusionThre"));
	if (!bRet)
	{
		tmpSearchDataParam.m_dConfusionThre1 = (tmpSearchDataParam.m_dAcceptThre1 + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre1 + 0.1);
	}
	else
	{
		tmpSearchDataParam.m_dConfusionThre1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	}

// 	bRet = cXMLConfigurator.FindElem(_T("EnableSharpScore"));
// 	if (!bRet)
// 	{
// 		tmpSearchDataParam.m_bEnableSharpScore1 =  false;
// 	}
// 	tmpSearchDataParam.m_bEnableSharpScore1 = _ttoi(cXMLConfigurator.GetElemData());
// 
// 	bRet = cXMLConfigurator.FindElem(_T("SharpThre"));
// 	if (!bRet)
// 	{
// 		tmpSearchDataParam.m_dSharpThre1 = 0.5;
// 	}
// 	else
// 	{
// 		tmpSearchDataParam.m_dSharpThre1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
// 	}

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

    bRet = cXMLConfigurator.FindElem(_T("SearchParam2"));
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
    tmpSearchDataParam.m_nSearchNum2 = _ttoi(cXMLConfigurator.GetElemData());
    bRet = cXMLConfigurator.FindElem(_T("AcceptThre"));
    if (!bRet)
    {
        return false;
    }
    tmpSearchDataParam.m_dAcceptThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);

    bRet = cXMLConfigurator.FindElem(_T("ConfusionThre"));
    if (!bRet)
    {
        tmpSearchDataParam.m_dConfusionThre2 = (tmpSearchDataParam.m_dAcceptThre1 + 0.1 > 1.0 ? 1.0 : tmpSearchDataParam.m_dAcceptThre1 + 0.1);
    }
    else
    {
        tmpSearchDataParam.m_dConfusionThre2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
    }

   
    cXMLConfigurator.LeaveElem();
	/////////////TrainResult//////////////
	bRet = cXMLConfigurator.FindElem(_T("TrainResult"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
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
	bRet = cXMLConfigurator.FindElem(_T("Sharpness1"));
	if (!bRet)
	{
		tmpTrainResult.m_dSharpness1 = 1.0;
	}
	tmpTrainResult.m_dSharpness1 = _tcstod(cXMLConfigurator.GetElemData(), NULL);
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
    bRet = cXMLConfigurator.FindElem(_T("Sharpness2"));
    if (!bRet)
    {
        tmpTrainResult.m_dSharpness2 = 1.0;
    }
    tmpTrainResult.m_dSharpness2 = _tcstod(cXMLConfigurator.GetElemData(), NULL);

    bRet = cXMLConfigurator.FindElem(_T("MarkToSearch2X"));
    if (!bRet)
    {
        return false;
    }
    tmpTrainResult.m_dMarkToSearch2X = _tcstod(cXMLConfigurator.GetElemData(), NULL);

    bRet = cXMLConfigurator.FindElem(_T("MarkToSearch2Y"));
    if (!bRet)
    {
        return false;
    }
    tmpTrainResult.m_dMarkToSearch2Y = _tcstod(cXMLConfigurator.GetElemData(), NULL);

    bRet = cXMLConfigurator.FindElem(_T("RectSearchSize"));
    if (!bRet)
    {
        return false;
    }
    cXMLConfigurator.EnterElem();

    bRet = cXMLConfigurator.FindElem(_T("dX"));
    if (!bRet)
    {
        return false;
    }
    tmpTrainResult.m_scRectSearchSize.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL)) ;
    bRet = cXMLConfigurator.FindElem(_T("dY"));
    if (!bRet)
    {
        return false;
    }
    tmpTrainResult.m_scRectSearchSize.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL)) ;
	cXMLConfigurator.LeaveElem();
  //  cXMLConfigurator.LeaveElem();
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
	if (tmpTrainDataParam.m_bMaskEnable1)
	{
		hStatus = cDIB.Init(strDir + _T("Mask.bmp"));
		if (!IsSuccess(hStatus))
		{
			tmpTrainDataParam.m_bMaskEnable1 = false;
		}
		else
		{
			hStatus = cDIB.GetImage(tmpImageMask);
			if (!IsSuccess(hStatus))
			{
				tmpTrainDataParam.m_bMaskEnable1 = false;
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
	if (tmpTrainDataParam.m_bMaskEnable1)
	{
		m_tmpImageMask1 = tmpImageMask;
	}
    if (tmpTrainDataParam.m_bMaskEnable2)
    {
        m_tmpImageMask2 = tmpImageMask2;
    }
	m_PatternSearchTool1 = tmpPatternSearchTool;
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
	m_ImageMask1		= tmpImageMask;
    m_ImageMask2		= tmpImageMask2;

	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CPatternDoubleSearchTool::SaveModelToFile(CString strDir)
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
	cXMLConfigurator.AddElem(_T("ModelPoint1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetX());
	cXMLConfigurator.AddElem(_T("CenterX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_ModelPoint1.GetY());
	cXMLConfigurator.AddElem(_T("CenterY"), strInfo);
	cXMLConfigurator.LeaveElem();

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
	cXMLConfigurator.AddElem(_T("SearchParam1"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum1);
	cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre1);
	cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre1);
	cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);

// 	strInfo.Format(_T("%d"), m_SearchDataParam.m_bEnableSharpScore1);
// 	cXMLConfigurator.AddElem(_T("EnableSharpScore"), strInfo);
// 	strInfo.Format(_T("%f"), m_SearchDataParam.m_dSharpThre1);
// 	cXMLConfigurator.AddElem(_T("SharpThre"), strInfo);

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

    cXMLConfigurator.AddElem(_T("SearchParam2"));
    cXMLConfigurator.EnterElem();
    strInfo.Format(_T("%d"), m_SearchDataParam.m_nSearchNum2);
    cXMLConfigurator.AddElem(_T("SearchNum"), strInfo);
    strInfo.Format(_T("%f"), m_SearchDataParam.m_dAcceptThre2);
    cXMLConfigurator.AddElem(_T("AcceptThre"), strInfo);
    strInfo.Format(_T("%f"), m_SearchDataParam.m_dConfusionThre2);
    cXMLConfigurator.AddElem(_T("ConfusionThre"), strInfo);

    cXMLConfigurator.LeaveElem();

	/////////////TrainResult//////////////
	cXMLConfigurator.AddElem(_T("TrainResult"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelX1);
	cXMLConfigurator.AddElem(_T("ModelX1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dModelY1);
	cXMLConfigurator.AddElem(_T("ModelY1"), strInfo);
	strInfo.Format(_T("%f"), m_TrainResult.m_dSharpness1);
	cXMLConfigurator.AddElem(_T("Sharpness1"), strInfo);

    strInfo.Format(_T("%f"), m_TrainResult.m_dModelX2);
    cXMLConfigurator.AddElem(_T("ModelX2"), strInfo);
    strInfo.Format(_T("%f"), m_TrainResult.m_dModelY2);
    cXMLConfigurator.AddElem(_T("ModelY2"), strInfo);
    strInfo.Format(_T("%f"), m_TrainResult.m_dSharpness2);
    cXMLConfigurator.AddElem(_T("Sharpness2"), strInfo);

    strInfo.Format(_T("%f"), m_TrainResult.m_dMarkToSearch2X);
    cXMLConfigurator.AddElem(_T("MarkToSearch2X"), strInfo);

    strInfo.Format(_T("%f"), m_TrainResult.m_dMarkToSearch2Y);
    cXMLConfigurator.AddElem(_T("MarkToSearch2Y"), strInfo);

    cXMLConfigurator.AddElem(_T("RectSearchSize"));
    cXMLConfigurator.EnterElem();
    strInfo.Format(_T("%f"), m_TrainResult.m_scRectSearchSize.GetX());
    cXMLConfigurator.AddElem(_T("dX"), strInfo);
    strInfo.Format(_T("%f"), m_TrainResult.m_scRectSearchSize.GetY());
    cXMLConfigurator.AddElem(_T("dY"), strInfo);
    
    cXMLConfigurator.LeaveElem();

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
	if (m_TrainDataParam.m_bMaskEnable1 && m_ImageMask1.IsValid())
	{
		hStatus = cDIB.Init(m_ImageMask1);
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
	bRet = m_PatternSearchTool1.SaveTrainModelToBinaryFile(strDir + _T("Model.pts"));
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
bool CPatternDoubleSearchTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		//工具1更新界面
		m_tmpTrainGuiParam.m_TrainRect1				= m_GuiTrainRect1.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint1			= m_GuiModelPoint1.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect1			= m_GuiSearchRect1.GetRect();

		//工具2更新界面
		m_tmpTrainGuiParam.m_TrainRect2				= m_GuiTrainRect2.GetRect();
		m_tmpTrainGuiParam.m_ModelPoint2			= m_GuiModelPoint2.GetCenter();
		m_tmpTrainGuiParam.m_SearchRect2			= m_GuiSearchRect2.GetRect();
	}
	else // 将数据传递到Gui控件上
	{	
		//工具1传递
		m_GuiModelPoint1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint1.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint1, m_GuiModelPoint1.GetExtent());
		m_GuiTrainRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect1.SetRect(m_tmpTrainGuiParam.m_TrainRect1);

		m_GuiSearchRect1.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect1.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect1.SetRect(m_tmpTrainGuiParam.m_SearchRect1);

		//工具2传递
		m_GuiModelPoint2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiModelPoint2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiModelPoint2.SetCenterExtent(m_tmpTrainGuiParam.m_ModelPoint2, m_GuiModelPoint2.GetExtent());
		m_GuiTrainRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect2.SetRect(m_tmpTrainGuiParam.m_TrainRect2);

		m_GuiSearchRect2.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiSearchRect2.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiSearchRect2.SetRect(m_tmpTrainGuiParam.m_SearchRect2);
	}

	return true;
}

bool CPatternDoubleSearchTool::UpdateSearchGuiData(bool bSaveAndValidate)
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

	}

	return true;
}

// 清除控件及其显示					
bool CPatternDoubleSearchTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CPatternDoubleSearchTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CPatternDoubleSearchTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchRectArray);
	ClearGraphicsArray(m_pGuiSearchCrossArray);

	return true;
}

bool CPatternDoubleSearchTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint1"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect1"));	

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_ModelPoint2"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_SearchRect2"));
	}

	return true;
}

bool CPatternDoubleSearchTool::ClearResultGui()
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

	}

	return true;
}

// 获取训练模板原始图像
bool CPatternDoubleSearchTool::GetPatternImage(cpImage &ImagePattern)
{
	/*bool bRet = m_PatternSearchTool1.GetPatternImage(ImagePattern);
	if (!bRet)
	{
		return false;
	}
	return true;*/
	if (m_tmpImageTrain.IsValid() == false)
	{
		return false;
	}
	if(cpImage::statusSuccess == m_tmpImageTrain.GetSubImage(ImagePattern,m_PatternSearchTool1.GetTrainRectRegion()))
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}	

// 获取训练模板原始图像
bool CPatternDoubleSearchTool::GetPatternImage2(cpImage &ImagePattern)
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
	return true;
}	

// 恢复训练参数
bool CPatternDoubleSearchTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CPatternDoubleSearchTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CPatternDoubleSearchTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect1.SetLabel(_T("Train Area1"));
		m_GuiSearchRect1.SetLabel(_T("Search Area1"));
		m_GuiModelPoint1.SetLabel(_T("Reference Point1"));

		m_GuiTrainRect2.SetLabel(_T("Train Area2"));
		m_GuiSearchRect2.SetLabel(_T("Search Area2"));
		m_GuiModelPoint2.SetLabel(_T("Reference Point2"));
		break;
	case 1:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	default:
		m_GuiTrainRect1.SetLabel(_T("训练区域1"));
		m_GuiSearchRect1.SetLabel(_T("搜索区域1"));
		m_GuiModelPoint1.SetLabel(_T("参考点1"));

		m_GuiTrainRect2.SetLabel(_T("训练区域2"));
		m_GuiSearchRect2.SetLabel(_T("搜索区域2"));
		m_GuiModelPoint2.SetLabel(_T("参考点2"));
		break;
	}


}

bool CPatternDoubleSearchTool::SetGuiByCam(double nWidth,double nHeight)
{
    if(nWidth<1||nHeight<1)return false;
    if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

    //训练区域
    double centerx,centery, xlen,ylen;
    m_GuiTrainRect1.GetCenterWidthHeight(centerx,centery, xlen,ylen);
    m_GuiTrainRect1.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //搜索区域

    m_GuiSearchRect1.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiSearchRect1.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //参考点
    m_GuiModelPoint1.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
    m_GuiModelPoint1.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
    //m_GuiModelPoint.SetLabel(_T("参考点"));


    m_GuiTrainRect2.GetCenterWidthHeight(centerx,centery, xlen,ylen);
    m_GuiTrainRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //搜索区域

    m_GuiSearchRect2.GetCenterWidthHeight(centerx,centery,xlen,ylen);
    m_GuiSearchRect2.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

    //参考点
    m_GuiModelPoint2.GetCenterXYWidthHeight(centerx,centery,xlen,ylen);
    m_GuiModelPoint2.SetCenterXYWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamWidth)*nWidth);
    //m_GuiModelPoint.SetLabel(_T("参考点"));

    m_CamHeight = nHeight;
    m_CamWidth = nWidth;

    return true;
}
CString	CPatternDoubleSearchTool::GetResultErrInfo()
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



