// IdReaderBarCodeTool.cpp: implementation of the CIdReaderBarCodeTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IdReaderBarCodeTool.h"
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

CIdReaderBarCodeTool::CIdReaderBarCodeTool()
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_LogFont.lfHeight = 24;
	m_LogFont.lfWidth = 0;
	m_LogFont.lfEscapement = 0;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfWeight = FW_BOLD;
	m_LogFont.lfItalic = 0;
	m_LogFont.lfUnderline = 0;
	m_LogFont.lfStrikeOut = 0;
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_LogFont.lfQuality = PROOF_QUALITY;
	m_LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(m_LogFont.lfFaceName, "Times New Roman");
	m_StrInspectInfor = _T("");
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Seach Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));

		break;
	}
	CreateGUID(m_strKeyText);

	m_tmpTrainGuiParam.m_nLineWidth						= m_GuiTrainRect.GetLineWidth();
	m_tmpTrainGuiParam.m_LineColor						= m_GuiTrainRect.GetLineColor();

	m_TrainDataParam									= m_tmpTrainDataParam;
	m_TrainGuiParam										= m_tmpTrainGuiParam;
	m_SearchDataParam									= m_tmpSearchDataParam;
	m_SearchGuiParam									= m_tmpSearchGuiParam;
}

CIdReaderBarCodeTool::CIdReaderBarCodeTool(CIdReaderBarCodeTool &cIdReaderBarCodeTool)
{
	if (cIdReaderBarCodeTool.GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return;
	}

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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_LogFont.lfHeight = 24;
	m_LogFont.lfWidth = 0;
	m_LogFont.lfEscapement = 0;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfWeight = FW_BOLD;
	m_LogFont.lfItalic = 0;
	m_LogFont.lfUnderline = 0;
	m_LogFont.lfStrikeOut = 0;
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_LogFont.lfQuality = PROOF_QUALITY;
	m_LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(m_LogFont.lfFaceName, "Times New Roman");

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Seach Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));
		break;
	}

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cIdReaderBarCodeTool.m_CamHeight;
	m_CamWidth								= cIdReaderBarCodeTool.m_CamWidth;

	m_TrainDataParam						= cIdReaderBarCodeTool.m_TrainDataParam;
	m_TrainGuiParam							= cIdReaderBarCodeTool.m_TrainGuiParam;
	m_SearchDataParam						= cIdReaderBarCodeTool.m_SearchDataParam;
	m_SearchGuiParam						= cIdReaderBarCodeTool.m_SearchGuiParam;
	m_TrainResult							= cIdReaderBarCodeTool.m_TrainResult;
	m_ImageTrain							= cIdReaderBarCodeTool.m_ImageTrain;
	m_ImageMask								= cIdReaderBarCodeTool.m_ImageMask;
	m_bTrainGuiShow							= cIdReaderBarCodeTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cIdReaderBarCodeTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cIdReaderBarCodeTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cIdReaderBarCodeTool.m_bTrainOK;
	m_pGuiDisplay							= cIdReaderBarCodeTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cIdReaderBarCodeTool.UpdateSearchGuiData(true);
	cIdReaderBarCodeTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cIdReaderBarCodeTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cIdReaderBarCodeTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cIdReaderBarCodeTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cIdReaderBarCodeTool.m_tmpSearchGuiParam);
	SetTrainImage(cIdReaderBarCodeTool.m_tmpImageTrain);
	SetMaskImage(cIdReaderBarCodeTool.m_tmpImageMask);	
}

CIdReaderBarCodeTool& CIdReaderBarCodeTool::operator=(CIdReaderBarCodeTool &cIdReaderBarCodeTool)
{
	if (this == &cIdReaderBarCodeTool)
	{
		return *this;
	}

	if (cIdReaderBarCodeTool.GetSearchToolType() != eIdReaderBarCodeTool)
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
	m_GuiTrainRect.SetCenterWidthHeight(350, 240, 100, 100);
	m_GuiTrainRect.SetLabelVisible(TRUE);

	m_LogFont.lfHeight = 24;
	m_LogFont.lfWidth = 0;
	m_LogFont.lfEscapement = 0;
	m_LogFont.lfOrientation = 0;
	m_LogFont.lfWeight = FW_BOLD;
	m_LogFont.lfItalic = 0;
	m_LogFont.lfUnderline = 0;
	m_LogFont.lfStrikeOut = 0;
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_LogFont.lfQuality = PROOF_QUALITY;
	m_LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(m_LogFont.lfFaceName, "Times New Roman");

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Seach Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));

		break;
	}

	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cIdReaderBarCodeTool.m_CamHeight;
	m_CamWidth								= cIdReaderBarCodeTool.m_CamWidth;
	m_TrainDataParam						= cIdReaderBarCodeTool.m_TrainDataParam;
	m_TrainGuiParam							= cIdReaderBarCodeTool.m_TrainGuiParam;
	m_SearchDataParam						= cIdReaderBarCodeTool.m_SearchDataParam;
	m_SearchGuiParam						= cIdReaderBarCodeTool.m_SearchGuiParam;
	m_TrainResult							= cIdReaderBarCodeTool.m_TrainResult;
	m_ImageTrain							= cIdReaderBarCodeTool.m_ImageTrain;
	m_ImageMask								= cIdReaderBarCodeTool.m_ImageMask;
	m_bTrainGuiShow							= cIdReaderBarCodeTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cIdReaderBarCodeTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cIdReaderBarCodeTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cIdReaderBarCodeTool.m_bTrainOK;
	m_pGuiDisplay							= cIdReaderBarCodeTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cIdReaderBarCodeTool.UpdateSearchGuiData(true);
	cIdReaderBarCodeTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cIdReaderBarCodeTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cIdReaderBarCodeTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cIdReaderBarCodeTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cIdReaderBarCodeTool.m_tmpSearchGuiParam);
	SetTrainImage(cIdReaderBarCodeTool.m_tmpImageTrain);
	SetMaskImage(cIdReaderBarCodeTool.m_tmpImageMask);

	return *this;
}

CIdReaderBarCodeTool::~CIdReaderBarCodeTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CIdReaderBarCodeTool::GetSearchToolType()
{
	return eIdReaderBarCodeTool;
}										   

bool CIdReaderBarCodeTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//搜索区域
	double centerx,centery,xlen,ylen;

	m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

// 设置显示控件指针		
bool CIdReaderBarCodeTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CIdReaderBarCodeTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CIdReaderBarCodeTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CIdReaderBarCodeTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CIdReaderBarCodeTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CIdReaderBarCodeTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;

	UpdateTrainResultGuiDisplay();

	return true;
}	

// 更新搜索界面显示									
bool CIdReaderBarCodeTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CIdReaderBarCodeTool::UpdateTrainGuiDisplay()
{
	// 更新
	m_GuiTrainRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bTrainRectVisible);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiTrainRect, m_strKeyText+_T("_TrainRect"));

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}	

// 更新训练结果Gui控件显示
bool CIdReaderBarCodeTool::UpdateTrainResultGuiDisplay()
{
	return true;
}				

// 更新搜索Gui控件显示														
bool CIdReaderBarCodeTool::UpdateSearchResultGuiDisplay()
{
	for (int i=0; i<m_pGuiSearchResultArray.size(); i++)
	{
		m_pGuiSearchResultArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bVisible);
	}
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		for (int i=0; i<m_pGuiSearchResultArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchResultArray[i],m_pGuiSearchResultArray[i]->GetTipText());
		}

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}												

// 更新界面显示
bool CIdReaderBarCodeTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CIdReaderBarCodeTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CIdReaderBarCodeTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CIdReaderBarCodeTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	*((CIdReaderBarCodeTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CIdReaderBarCodeTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CIdReaderBarCodeTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CIdReaderBarCodeTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CIdReaderBarCodeTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CIdReaderBarCodeTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CIdReaderBarCodeTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CIdReaderBarCodeTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CIdReaderBarCodeTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CIdReaderBarCodeTool::Train()
{
	m_bTrainOK = false;
	// 获取Gui控件数据
	UpdateTrainGuiData(true);

	m_bTrainOK       = true;
	m_ImageTrain     = m_tmpImageTrain;
	m_TrainDataParam = m_tmpTrainDataParam;
	m_TrainGuiParam	 = m_tmpTrainGuiParam;
	m_TrainResult    = m_tmpTrainResult;
	return true;
}

// 获取训练结果图像														
bool CIdReaderBarCodeTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	if(m_tmpImageTrain.IsValid())
	{
		m_ImageTrainResult = m_tmpImageTrain;
	}
	ImageTrainResult = m_ImageTrainResult;
	return true;
}	

// 获取训练结果
bool CIdReaderBarCodeTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	*((CIdReaderBarCodeTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}	

// 是否训练成功
bool CIdReaderBarCodeTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CIdReaderBarCodeTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CIdReaderBarCodeSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CIdReaderBarCodeTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	*((CIdReaderBarCodeSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CIdReaderBarCodeTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CIdReaderBarCodeSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CIdReaderBarCodeTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eIdReaderBarCodeTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CIdReaderBarCodeSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CIdReaderBarCodeTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;
		return false;
	}	
	m_nResultErrNumber=0;
	// 清空
	ClearResult();

	scRect rect;
	rect = m_GuiTrainRect.GetRect();

	svStd vector<Code1DResult> m_BarcodeResultArray;
	bool bSuccess = false;

	m_toolBarCode.SetFind1DCodePara(m_TrainDataParam.m_Find1DCodePara);

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

	if(bIsProcessImage)
	{
		bSuccess = m_toolBarCode.Execute(tmpImage,rect,m_BarcodeResultArray);
	}
	else
	{
		bSuccess = m_toolBarCode.Execute(ImageSearch,rect,m_BarcodeResultArray);
	}


	if (!bSuccess)
	{

		m_nResultErrNumber = -2;
		return false;
	}

	CSearchResult searchResult;

	for (int i = 0;i < m_BarcodeResultArray.size()&&i<BarCodeTool_MaxResultNum;i++)
	{
		searchResult.m_vcode1DResult.push_back(m_BarcodeResultArray[i]);
		m_StrInspectInfor.Format(_T("读码结果：%s"),m_BarcodeResultArray[i].m_OutputData);
		CResultSearchRect resRect;
		resRect.m_bFindRect = TRUE;
		resRect.m_searchRect.SetCornersPoPxPy(sc2Vector(m_BarcodeResultArray[i].m_pt[0].GetX(),m_BarcodeResultArray[i].m_pt[0].GetY()),
			sc2Vector(m_BarcodeResultArray[i].m_pt[1].GetX(),m_BarcodeResultArray[i].m_pt[1].GetY()),
			sc2Vector(m_BarcodeResultArray[i].m_pt[3].GetX(),m_BarcodeResultArray[i].m_pt[3].GetY()));
		searchResult.m_vTrainSearchRect.push_back(resRect);

		searchResult.m_dPosX = m_BarcodeResultArray[0].m_pt[0].GetX();
		searchResult.m_dPosY = m_BarcodeResultArray[0].m_pt[0].GetY();
		searchResult.m_dAngle = m_BarcodeResultArray[0].m_dAngle;
		for(int it=0;it<MaxAuxiliarySearchResultNum && it<4;it++)
		{
			searchResult.m_vdAuxiliaryPosX[it] = m_BarcodeResultArray[0].m_pt[it].GetX();
			searchResult.m_vdAuxiliaryPosY[it] = m_BarcodeResultArray[0].m_pt[it].GetY();
		}

		CString	strTmpKey;

		strTmpKey.Format(_T("%s_ResultPolyline%d"), m_strKeyText, i+1);
		m_pGuiPolylineArray[i].RemoveVertices();
		m_pGuiPolylineArray[i].AddVertex(m_BarcodeResultArray[i].m_pt[0].GetX(),m_BarcodeResultArray[i].m_pt[0].GetY());
		m_pGuiPolylineArray[i].AddVertex(m_BarcodeResultArray[i].m_pt[1].GetX(),m_BarcodeResultArray[i].m_pt[1].GetY());
		m_pGuiPolylineArray[i].AddVertex(m_BarcodeResultArray[i].m_pt[2].GetX(),m_BarcodeResultArray[i].m_pt[2].GetY());
		m_pGuiPolylineArray[i].AddVertex(m_BarcodeResultArray[i].m_pt[3].GetX(),m_BarcodeResultArray[i].m_pt[3].GetY());
		m_pGuiPolylineArray[i].SetTipTextEnabled(TRUE);
		m_pGuiPolylineArray[i].SetTipText(strTmpKey);
		m_pGuiPolylineArray[i].SetVisible();
		m_pGuiSearchResultArray.push_back(m_pGuiPolylineArray + i);

		strTmpKey.Format(_T("%s_ResultTextArray%d"), m_strKeyText, i+1);
		CString strCode;
		strCode.Format("%d---%s",i+1,m_BarcodeResultArray[i].m_OutputData);
		m_pGuiTextArray[i].SetXYRotationText(m_BarcodeResultArray[i].m_pt[0].GetX(),m_BarcodeResultArray[i].m_pt[0].GetY(),0,strCode);
		m_pGuiTextArray[i].SetTipTextEnabled(TRUE);
		m_pGuiTextArray[i].SetTipText(strTmpKey);
		m_pGuiTextArray[i].SetVisible();
		m_pGuiTextArray[i].SetTextFont(&m_LogFont);
		m_pGuiSearchResultArray.push_back(m_pGuiTextArray + i);
	}

	m_SearchResultArray.push_back(searchResult);
	return true;
}

// 获取搜索结果个数										
int CIdReaderBarCodeTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CIdReaderBarCodeTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板文件
bool CIdReaderBarCodeTool::LoadModelFromFile(CString strDir)
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
	CIdReaderBarCodeTrainDataParam tmpTrainDataParam;
	CIdReaderBarCodeTrainGuiParam tmpTrainGuiParam;
	CIdReaderBarCodeSearchDataParam tmpSearchDataParam;
	CIdReaderBarCodeSearchGuiParam tmpSearchGuiParam;
	CIdReaderBarCodeTrainResult    tmpTrainResult;
	if (!cXMLConfigurator.Load(strXmlPath))
	{
		return false;
	}

	bRet = cXMLConfigurator.FindElem(_T("Config"));
	if (!bRet){return false;}
	cXMLConfigurator.EnterElem();
	{
		/////////////TrainDataParam//////////////
		cXMLConfigurator.ResetCurElemPos();
		/////////////CameraParam//////////////
		bRet = cXMLConfigurator.FindElem(_T("CamParam"));
		if (!bRet)
		{
			m_CamHeight = 0;
			m_CamWidth = 0;
		}
		else
		{
			cXMLConfigurator.EnterElem();
			{
				bRet = cXMLConfigurator.FindElem(_T("CamHeight"));
				if (!bRet)
				{
					m_CamHeight = 0;
				}
				else
				{
					cXMLConfigurator.GetElemData(m_CamHeight);
				}
				bRet = cXMLConfigurator.FindElem(_T("CamWidth"));
				if (!bRet)
				{
					m_CamWidth = 0;
				}
				else
				{
					cXMLConfigurator.GetElemData(m_CamWidth);
				}		
			}
			cXMLConfigurator.LeaveElem();
		}
		bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
		if (!bRet)
		{
			return false;
		}
		cXMLConfigurator.EnterElem();
		{
			// ImageChannel
			bRet = cXMLConfigurator.FindElem(_T("ImageChannel"));				
			if (!bRet)
			{
				tmpTrainDataParam.m_nImageChannel = 0;
			}
			else
			{
				tmpTrainDataParam.m_nImageChannel = _ttol(cXMLConfigurator.GetElemData());
			}

			// search rect
			bRet = cXMLConfigurator.FindElem(_T("TrainRect"));
			if (!bRet)
			{
				return false;
			}
			cXMLConfigurator.EnterElem();
			{
				strInfo.Format(_T("%f"), tmpTrainGuiParam.m_TrainRect.GetOrigin().GetX());
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
			}
			cXMLConfigurator.LeaveElem();

			Find1DCodePara find1DCodePara = tmpTrainDataParam.m_Find1DCodePara;
			bRet = cXMLConfigurator.FindElem(_T("Find1DCodePara"));
			if (!bRet)
			{
				return false;
			}
			cXMLConfigurator.EnterElem();
			{
				bRet = cXMLConfigurator.FindElem(_T("DecodeMethod"));
				if (!bRet){return false;}
				find1DCodePara.m_methodDecode = (DecodeMethod)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("AppMode"));
				if (!bRet){return false;}
				find1DCodePara.m_modeApp = (AppMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("CodaBarFlag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCodaBarFlag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("Code128Flag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCode128Flag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("Code39Flag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCode39Flag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("Code93Flag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCode93Flag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("CodeEANFlag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCodeEANFlag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("CodeITF25Flag"));
				if (!bRet){return false;}
				find1DCodePara.m_bCodeITF25Flag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("RegionNum"));
				if (!bRet){return false;}
				find1DCodePara.m_nRegionNum = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxSizeWidth"));
				if (!bRet){return false;}
				find1DCodePara.m_nMaxSizeWidth = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MinSizeWidth"));
				if (!bRet){return false;}
				find1DCodePara.m_nMinSizeWidth = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("ErrorDetectFlag"));
				if (!bRet){return false;}
				find1DCodePara.m_bErrorDetectFlag = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("ImageMorph"));
				if (!bRet){return false;}
				find1DCodePara.m_nImageMorph = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("PromodeDistortion"));
				if (!bRet){return false;}
				find1DCodePara.m_nPromodeDistortion = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("PromodeSpot"));
				if (!bRet){return false;}
				find1DCodePara.m_nPromodeSpot = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("PromodeWhiteGap"));
				if (!bRet){return false;}
				find1DCodePara.m_nPromodeWhiteGap = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("SampleLevel"));
				if (!bRet){return false;}
				find1DCodePara.m_nSampleLevel = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("SavePicLevel"));
				if (!bRet){return false;}
				find1DCodePara.m_nSavePicLevel = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("StaticAreaWidth"));
				if (!bRet){return false;}
				find1DCodePara.m_nStaticAreaWidth = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("WaitingTime"));
				if (!bRet){return false;}
				find1DCodePara.m_nWaitingTime = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("WindowSize"));
				if (!bRet){return false;}
				find1DCodePara.m_nWindowSize = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxSizeHeight"));
				if (!bRet){return false;}
				find1DCodePara.m_nMaxSizeHeight = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MinSizeHeight"));
				if (!bRet){return false;}
				find1DCodePara.m_nMinSizeHeight = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("ElemMaxWidth"));
				if (!bRet){return false;}
				find1DCodePara.m_nElemMaxWidth = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("ElemMinWidth"));
				if (!bRet){return false;}
				find1DCodePara.m_nElemMinWidth = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("NumScanlines"));
				if (!bRet){return false;}
				find1DCodePara.m_nNumScanlines = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("BarcodeBalanceLevel"));
				if (!bRet){return false;}
				find1DCodePara.m_nBarcodeBalanceLevel = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("SegmentationMethod"));
				if (!bRet){return false;}
				find1DCodePara.m_methodSegmentation = (SegmentationMethod)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("RectangleFlag"));
				if (!bRet){return false;}
				find1DCodePara.m_modeBoxDisplay = (BoxDisplayMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("FrameCheckOut"));
				if (!bRet){return false;}
				find1DCodePara.m_bFrameCheckOut = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

			}
			cXMLConfigurator.LeaveElem();

		}
		cXMLConfigurator.LeaveElem();
	}
	cXMLConfigurator.LeaveElem();

	//////////////////////////读取模板图像//////////////////////////
	scDIB cDIB;
	HStatus hStatus;
	scImageGray tmpIImageModel;
	scImageGray tmpImageMask;
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

	if (!bRet)
	{
		return false;
	}
	// 数据赋值
	m_tmpImageTrain = tmpIImageModel;
	m_ImageTrain = m_tmpImageTrain;

	m_tmpTrainDataParam = tmpTrainDataParam;
	m_tmpTrainGuiParam  = tmpTrainGuiParam;
	m_tmpSearchDataParam= tmpSearchDataParam;
	m_tmpSearchGuiParam = tmpSearchGuiParam;
	m_TrainDataParam	= tmpTrainDataParam;
	m_TrainGuiParam		= tmpTrainGuiParam;
	m_SearchDataParam	= tmpSearchDataParam;
	m_SearchGuiParam	= tmpSearchGuiParam;
	m_TrainResult       = tmpTrainResult;


	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);

	return true;
}

// 保存模板文件							
bool CIdReaderBarCodeTool::SaveModelToFile(CString strDir)
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
	{
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
		{
			// ImageChannel
			strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
			cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);

			// search rect
			cXMLConfigurator.AddElem(_T("TrainRect"));
			cXMLConfigurator.EnterElem();
			{
				strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetX());
				cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
				strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetOrigin().GetY());
				cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
				strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetX());
				cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
				strInfo.Format(_T("%f"), m_TrainGuiParam.m_TrainRect.GetSize().GetY());
				cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
			}
			cXMLConfigurator.LeaveElem();

			Find1DCodePara &find1DCodePara = m_TrainDataParam.m_Find1DCodePara;
			cXMLConfigurator.AddElem(_T("Find1DCodePara"));
			cXMLConfigurator.EnterElem();
			{
				cXMLConfigurator.AddElem(_T("DecodeMethod"), find1DCodePara.m_methodDecode);
				cXMLConfigurator.AddElem(_T("AppMode"), find1DCodePara.m_modeApp);
				cXMLConfigurator.AddElem(_T("CodaBarFlag"), find1DCodePara.m_bCodaBarFlag);
				cXMLConfigurator.AddElem(_T("Code128Flag"), find1DCodePara.m_bCode128Flag);
				cXMLConfigurator.AddElem(_T("Code39Flag"), find1DCodePara.m_bCode39Flag);
				cXMLConfigurator.AddElem(_T("Code93Flag"), find1DCodePara.m_bCode93Flag);
				cXMLConfigurator.AddElem(_T("CodeEANFlag"), find1DCodePara.m_bCodeEANFlag);
				cXMLConfigurator.AddElem(_T("CodeITF25Flag"), find1DCodePara.m_bCodeITF25Flag);
				cXMLConfigurator.AddElem(_T("RegionNum"), find1DCodePara.m_nRegionNum);
				cXMLConfigurator.AddElem(_T("MaxSizeWidth"), find1DCodePara.m_nMaxSizeWidth);
				cXMLConfigurator.AddElem(_T("MinSizeWidth"), find1DCodePara.m_nMinSizeWidth);
				cXMLConfigurator.AddElem(_T("ErrorDetectFlag"), find1DCodePara.m_bErrorDetectFlag);
				cXMLConfigurator.AddElem(_T("ImageMorph"), find1DCodePara.m_nImageMorph);
				cXMLConfigurator.AddElem(_T("PromodeDistortion"), find1DCodePara.m_nPromodeDistortion);
				cXMLConfigurator.AddElem(_T("PromodeSpot"), find1DCodePara.m_nPromodeSpot);
				cXMLConfigurator.AddElem(_T("PromodeWhiteGap"), find1DCodePara.m_nPromodeWhiteGap);
				cXMLConfigurator.AddElem(_T("SampleLevel"), find1DCodePara.m_nSampleLevel);
				cXMLConfigurator.AddElem(_T("SavePicLevel"), find1DCodePara.m_nSavePicLevel);
				cXMLConfigurator.AddElem(_T("StaticAreaWidth"), find1DCodePara.m_nStaticAreaWidth);
				cXMLConfigurator.AddElem(_T("WaitingTime"), find1DCodePara.m_nWaitingTime);
				cXMLConfigurator.AddElem(_T("WindowSize"), find1DCodePara.m_nWindowSize);
				cXMLConfigurator.AddElem(_T("MaxSizeHeight"), find1DCodePara.m_nMaxSizeHeight);
				cXMLConfigurator.AddElem(_T("MinSizeHeight"), find1DCodePara.m_nMinSizeHeight);
				cXMLConfigurator.AddElem(_T("ElemMaxWidth"), find1DCodePara.m_nElemMaxWidth);
				cXMLConfigurator.AddElem(_T("ElemMinWidth"), find1DCodePara.m_nElemMinWidth);
				cXMLConfigurator.AddElem(_T("NumScanlines"), find1DCodePara.m_nNumScanlines);
				cXMLConfigurator.AddElem(_T("BarcodeBalanceLevel"), find1DCodePara.m_nBarcodeBalanceLevel);
				cXMLConfigurator.AddElem(_T("SegmentationMethod"), find1DCodePara.m_methodSegmentation);
				cXMLConfigurator.AddElem(_T("RectangleFlag"), find1DCodePara.m_modeBoxDisplay);
				cXMLConfigurator.AddElem(_T("FrameCheckOut"), find1DCodePara.m_bFrameCheckOut);
	
			}
			cXMLConfigurator.LeaveElem();
		}
		cXMLConfigurator.LeaveElem();
	}
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
	return true;
}	

// 更新训练控件数据信息	
bool CIdReaderBarCodeTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

		m_tmpTrainGuiParam.m_TrainRect	= m_GuiTrainRect.GetRect();

	}
	else // 将数据传递到Gui控件上
	{

		m_GuiTrainRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiTrainRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiTrainRect.SetRect(m_tmpTrainGuiParam.m_TrainRect);

	}

	return true;
}

bool CIdReaderBarCodeTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
	}

	return true;
}

// 清除控件及其显示					
bool CIdReaderBarCodeTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{		
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件				
bool CIdReaderBarCodeTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CIdReaderBarCodeTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultArray);

	return true;
}

bool CIdReaderBarCodeTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));


	}

	return true;
}

bool CIdReaderBarCodeTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();
		for (j=0; j<m_pGuiSearchResultArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchResultArray[j]->GetTipText());
		}
	}

	return true;	
}

// 获取训练模板原始图像
bool CIdReaderBarCodeTool::GetPatternImage(cpImage &ImagePattern)
{
	if(m_tmpImageTrain.IsValid())
	{
		ImagePattern = m_tmpImageTrain;
		return true;
	}
	return false;
}	

// 恢复训练参数
bool CIdReaderBarCodeTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CIdReaderBarCodeTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CIdReaderBarCodeTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Seach Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("搜索区域"));
		break;
	}
}

CString CIdReaderBarCodeTool::GetResultErrInfo()
{
	CString strErrInfo;
	strErrInfo.Format(_T("Err:%d"), GetResultErrNumber());

	switch(m_iLanguage)
	{
	case 0:
		{
			switch (m_nResultErrNumber)
			{
			case 0:
				strErrInfo =  m_StrInspectInfor;
				break;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("Image Conversion failed");
				break;
			case -2:			//	条码搜索失败			
				strErrInfo += _T("1D Code Search failed");
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
			case 0:
				strErrInfo =  m_StrInspectInfor;
				break;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("彩色图像转换失败");
				break;
			case -2:			//	条码搜索失败			
				strErrInfo += _T("条码搜索失败");
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
			case 0:
				strErrInfo =  m_StrInspectInfor;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("彩色图像转换失败");
				break;
			case -2:			//	条码搜索失败			
				strErrInfo += _T("条码搜索失败");
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