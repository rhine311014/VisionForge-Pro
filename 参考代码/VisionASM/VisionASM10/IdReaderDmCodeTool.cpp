// IdReaderDmCodeTool.cpp: implementation of the CIdReaderDmCodeTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IdReaderDmCodeTool.h"
#include "svDIB.h"
#include "svXMLConfigurator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void getPICSampling(int a,cpImage & imageInput)
{
	for (int m=0;m<imageInput.Height()/a;++m)
	{
		for (int n =0;n<imageInput.Width()/a;++n)
		{
			for (int j=a*m;j<a*m+a;j++)
			{
				for (int k=a*n;k<a*n+a;k++)
				{
					if ((j == a*m)&&(k==a*n))
					{
						unsigned char ug;
						ug = imageInput.GetPixelGray8(k,j);
						imageInput.SetPixelGray8(n,m,ug);
					}
				}
			}
		}
	}

	cpImage tempImage;
	scRect Recting(imageInput.Width()/a,imageInput.Height()/a);
	imageInput.GetSubImage(tempImage,Recting);
	imageInput.Release();
	tempImage.Clone(imageInput);
	tempImage.Release();
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIdReaderDmCodeTool::CIdReaderDmCodeTool()
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

	switch (m_iLanguage)
	{
	case 0:
		m_GuiTrainRect.SetLabel(_T("Train Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));

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

CIdReaderDmCodeTool::CIdReaderDmCodeTool(CIdReaderDmCodeTool &cIdReaderDmCodeTool)
{
	if (cIdReaderDmCodeTool.GetSearchToolType() != eIdReaderDmCodeTool)
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
		m_GuiTrainRect.SetLabel(_T("Train Area"));
		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		break;
	}

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cIdReaderDmCodeTool.m_CamHeight;
	m_CamWidth								= cIdReaderDmCodeTool.m_CamWidth;

	m_TrainDataParam						= cIdReaderDmCodeTool.m_TrainDataParam;
	m_TrainGuiParam							= cIdReaderDmCodeTool.m_TrainGuiParam;
	m_SearchDataParam						= cIdReaderDmCodeTool.m_SearchDataParam;
	m_SearchGuiParam						= cIdReaderDmCodeTool.m_SearchGuiParam;
	m_TrainResult							= cIdReaderDmCodeTool.m_TrainResult;
	m_ImageTrain							= cIdReaderDmCodeTool.m_ImageTrain;
	m_ImageMask								= cIdReaderDmCodeTool.m_ImageMask;
	m_bTrainGuiShow							= cIdReaderDmCodeTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cIdReaderDmCodeTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cIdReaderDmCodeTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cIdReaderDmCodeTool.m_bTrainOK;
	m_pGuiDisplay							= cIdReaderDmCodeTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cIdReaderDmCodeTool.UpdateSearchGuiData(true);
	cIdReaderDmCodeTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cIdReaderDmCodeTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cIdReaderDmCodeTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cIdReaderDmCodeTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cIdReaderDmCodeTool.m_tmpSearchGuiParam);
	SetTrainImage(cIdReaderDmCodeTool.m_tmpImageTrain);
	SetMaskImage(cIdReaderDmCodeTool.m_tmpImageMask);	
}

CIdReaderDmCodeTool& CIdReaderDmCodeTool::operator=(CIdReaderDmCodeTool &cIdReaderDmCodeTool)
{
	if (this == &cIdReaderDmCodeTool)
	{
		return *this;
	}

	if (cIdReaderDmCodeTool.GetSearchToolType() != eIdReaderDmCodeTool)
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
		m_GuiTrainRect.SetLabel(_T("Train Area"));

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));

		break;
	}

	CreateGUID(m_strKeyText);
	
	// 赋值
	m_CamHeight								= cIdReaderDmCodeTool.m_CamHeight;
	m_CamWidth								= cIdReaderDmCodeTool.m_CamWidth;
	m_TrainDataParam						= cIdReaderDmCodeTool.m_TrainDataParam;
	m_TrainGuiParam							= cIdReaderDmCodeTool.m_TrainGuiParam;
	m_SearchDataParam						= cIdReaderDmCodeTool.m_SearchDataParam;
	m_SearchGuiParam						= cIdReaderDmCodeTool.m_SearchGuiParam;
	m_TrainResult							= cIdReaderDmCodeTool.m_TrainResult;
	m_ImageTrain							= cIdReaderDmCodeTool.m_ImageTrain;
	m_ImageMask								= cIdReaderDmCodeTool.m_ImageMask;
	m_bTrainGuiShow							= cIdReaderDmCodeTool.m_bTrainGuiShow;
	m_bTrainResultGuiShow					= cIdReaderDmCodeTool.m_bTrainResultGuiShow;
	m_bSearchResultGuiShow					= cIdReaderDmCodeTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cIdReaderDmCodeTool.m_bTrainOK;
	m_pGuiDisplay							= cIdReaderDmCodeTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cIdReaderDmCodeTool.UpdateSearchGuiData(true);
	cIdReaderDmCodeTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cIdReaderDmCodeTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cIdReaderDmCodeTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cIdReaderDmCodeTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cIdReaderDmCodeTool.m_tmpSearchGuiParam);
	SetTrainImage(cIdReaderDmCodeTool.m_tmpImageTrain);
	SetMaskImage(cIdReaderDmCodeTool.m_tmpImageMask);

	return *this;
}

CIdReaderDmCodeTool::~CIdReaderDmCodeTool()
{
	// 清空
	ClearAllGui();
	ClearResult();
}

// 获取工具类型
CSearchToolType CIdReaderDmCodeTool::GetSearchToolType()
{
	return eIdReaderDmCodeTool;
}										   

 bool CIdReaderDmCodeTool::SetGuiByCam(double nWidth,double nHeight)
 {
	 if(nWidth<1||nHeight<1)return false;
	 if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	 //训练区域
	 double centerx,centery,xlen,ylen;
	 
	 m_GuiTrainRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	 m_GuiTrainRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);
	
	 m_CamHeight = nHeight;
	 m_CamWidth = nWidth;
	 return true;
 }

// 设置显示控件指针		
bool CIdReaderDmCodeTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CIdReaderDmCodeTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CIdReaderDmCodeTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CIdReaderDmCodeTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CIdReaderDmCodeTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CIdReaderDmCodeTool::ShowTrainResultGui(bool bShow)
{
	m_bTrainResultGuiShow = bShow;
	
	UpdateTrainResultGuiDisplay();
	
	return true;
}	

// 更新搜索界面显示									
bool CIdReaderDmCodeTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练Gui控件显示
bool CIdReaderDmCodeTool::UpdateTrainGuiDisplay()
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
bool CIdReaderDmCodeTool::UpdateTrainResultGuiDisplay()
{
	return true;
}				

// 更新搜索Gui控件显示														
bool CIdReaderDmCodeTool::UpdateSearchResultGuiDisplay()
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
bool CIdReaderDmCodeTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();
	
	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CIdReaderDmCodeTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	m_tmpTrainDataParam = *((CIdReaderDmCodeTrainDataParam*)pTrainDataParam);
	
	return true;
}

// 获取训练Data参数									
bool CIdReaderDmCodeTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}
	
	if (pTrainDataParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	*((CIdReaderDmCodeTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;
	
	return true;
}

// 获取搜索Gui参数						
bool CIdReaderDmCodeTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	m_tmpTrainGuiParam = *((CIdReaderDmCodeTrainGuiParam*)pTrainGuiParam);
	
	UpdateTrainGuiData(false);
	
	return true;
}

// 获取搜索Gui参数							
bool CIdReaderDmCodeTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}
	
	if (pTrainGuiParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	UpdateTrainGuiData(true);

	*((CIdReaderDmCodeTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;
	
	return true;
}

// 设置训练原始图像				
bool CIdReaderDmCodeTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CIdReaderDmCodeTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CIdReaderDmCodeTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CIdReaderDmCodeTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CIdReaderDmCodeTool::Train()
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
bool CIdReaderDmCodeTool::GetTrainResultImage(cpImage &ImageTrainResult)
{
	if(m_tmpImageTrain.IsValid())
	{
		m_ImageTrainResult = m_tmpImageTrain;
	}
	ImageTrainResult = m_ImageTrainResult;
	return true;
}	

// 获取训练结果
bool CIdReaderDmCodeTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}
	
	if (pTrainResult->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	*((CIdReaderDmCodeTrainResult*)pTrainResult) = m_tmpTrainResult;
	
	return true;
}	

// 是否训练成功
bool CIdReaderDmCodeTool::IsTrained()
{
	return m_bTrainOK;
}
				
// 设置搜索Data参数	
bool CIdReaderDmCodeTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CIdReaderDmCodeSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CIdReaderDmCodeTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}

	*((CIdReaderDmCodeSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CIdReaderDmCodeTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}
	
	m_tmpSearchGuiParam = *((CIdReaderDmCodeSearchGuiParam*)pSearchGuiParam);
	
	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CIdReaderDmCodeTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}
	
	if (pSearchGuiParam->GetSearchToolType() != eIdReaderDmCodeTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);
	
	*((CIdReaderDmCodeSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;
	
	return true;
}

// 执行搜索 					
bool CIdReaderDmCodeTool::Search(cpImage &ImageSearch)
{
	if (!m_bTrainOK)
	{
		m_nResultErrNumber=-99;
		return false;
	}	
	m_nResultErrNumber=0;
	m_StrInspectInfor = _T("");
	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	scRect rect;
	rect = m_GuiTrainRect.GetRect();
	rect.SetOrigion(sc2Vector(rect.GetOrigin().GetX()/pow(2.0, m_TrainDataParam.m_CoarseScale - 1),rect.GetOrigin().GetY()/pow(2.0, m_TrainDataParam.m_CoarseScale - 1)));
	rect.SetSize(sc2Vector(rect.GetSize().GetX()/pow(2.0, m_TrainDataParam.m_CoarseScale - 1),rect.GetSize().GetY()/pow(2.0, m_TrainDataParam.m_CoarseScale - 1)));

	svStd vector<DecodeMatrixResult> m_DMcodeResultArray;
	svStd vector<DecodeQRResult> m_QRcodeResultArray;
	bool bSuccess = false;
	m_toolDMCode.SetFindDMCodePara(m_TrainDataParam.m_FindDMCodePara);
	m_toolQRCode.SetFindQRCodePara(m_TrainDataParam.m_FindQRCodePara);
	
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

	//====================================================================
	if (bIsProcessImage)
	{
		getPICSampling(pow(2.0, m_TrainDataParam.m_CoarseScale - 1),tmpImage);
		tmpImage.Clone(ImageSearch);
	}
	else
	{
		getPICSampling(pow(2.0, m_TrainDataParam.m_CoarseScale - 1),ImageSearch);
	}
	
	if(!ImageSearch.IsValid())
	{
		m_nResultErrNumber = -1;
		return false;
	}

	//ImageTemp = ImageSearch;
	//if (HSuccess != sfAffineSampling(ImageTemp,ImageSearch,AffineSamplingParams))
	//{
	//	return false;
	//}

	// 进行图像缩放
	//if (HSuccess != sfAffineSampling(ImageSearch,ImageTemp,AffineSamplingParams))
	//{
	//	return false;
	//}
	//ImageSearch.Release();
	//ImageTemp.Clone(ImageSearch);
	//ImageTemp.Release();
	//====================================================================
	// DM读码
	if(m_TrainDataParam.m_nCodeType==0)
	{
		if(bIsProcessImage)
		{
			bSuccess = m_toolDMCode.Execute(tmpImage,rect,m_DMcodeResultArray);
		}
		else
		{
			bSuccess = m_toolDMCode.Execute(ImageSearch,rect,m_DMcodeResultArray);
		}
		

	}
	else if(m_TrainDataParam.m_nCodeType==1)
	{
		if(bIsProcessImage)
		{
			bSuccess = m_toolQRCode.Execute(tmpImage,rect,m_QRcodeResultArray);
		}
		else
		{
			bSuccess = m_toolQRCode.Execute(ImageSearch,rect,m_QRcodeResultArray);
		}
		
	}

	if (!bSuccess)
	{
		if(m_TrainDataParam.m_nCodeType==0)
		{
			m_nResultErrNumber = -2;
		}
		else if(m_TrainDataParam.m_nCodeType==1)
		{
			m_nResultErrNumber = -3;
		}
		return false;
	}

	// DM 码
	CSearchResult searchResult;
	
	if(m_TrainDataParam.m_nCodeType==0)
	{
		ConvertImagePostion(m_DMcodeResultArray,m_TrainDataParam);
		if(m_DMcodeResultArray.size()>0)
		{
			searchResult.m_dPosX = m_DMcodeResultArray[0].m_pt[0].GetX();
			searchResult.m_dPosY = m_DMcodeResultArray[0].m_pt[0].GetY();

			if(m_DMcodeResultArray[0].m_pt[0] != m_DMcodeResultArray[0].m_pt[1])
			{
				scLineSeg ResLineSeg;
				ResLineSeg.SetP1(m_DMcodeResultArray[0].m_pt[0]);
				ResLineSeg.SetP2(m_DMcodeResultArray[0].m_pt[1]);
				scLine ResLine;
				ResLine.SetFromLineSeg(ResLineSeg);
				searchResult.m_dAngle = scDegree(ResLine.GetRotation()).ToDouble();
			}

			for(int it=0;it<MaxAuxiliarySearchResultNum && it<4;it++)
			{
				searchResult.m_vdAuxiliaryPosX[it] = m_DMcodeResultArray[0].m_pt[it].GetX();
				searchResult.m_vdAuxiliaryPosY[it] = m_DMcodeResultArray[0].m_pt[it].GetY();
			}
		}

		for (int i = 0;i < m_DMcodeResultArray.size()&&i<DmCodeTool_MaxResultNum;i++)
		{
			searchResult.m_vcodeDMResult.push_back(m_DMcodeResultArray[i]);
			m_StrInspectInfor.Format(_T("%s"),m_DMcodeResultArray[0].m_OutputData);
			CResultSearchRect resRect;
			resRect.m_bFindRect = TRUE;
			resRect.m_searchRect.SetCornersPoPxPy(sc2Vector(m_DMcodeResultArray[i].m_pt[0].GetX(),m_DMcodeResultArray[i].m_pt[0].GetY()),
												sc2Vector(m_DMcodeResultArray[i].m_pt[1].GetX(),m_DMcodeResultArray[i].m_pt[1].GetY()),
												sc2Vector(m_DMcodeResultArray[i].m_pt[3].GetX(),m_DMcodeResultArray[i].m_pt[3].GetY()));
			searchResult.m_vTrainSearchRect.push_back(resRect);

			CString	strTmpKey;

			strTmpKey.Format(_T("%s_ResultPolyline%d"), m_strKeyText, i+1);
			m_pGuiPolylineArray[i].RemoveVertices();
			m_pGuiPolylineArray[i].AddVertex(m_DMcodeResultArray[i].m_pt[0].GetX(),m_DMcodeResultArray[i].m_pt[0].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_DMcodeResultArray[i].m_pt[1].GetX(),m_DMcodeResultArray[i].m_pt[1].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_DMcodeResultArray[i].m_pt[2].GetX(),m_DMcodeResultArray[i].m_pt[2].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_DMcodeResultArray[i].m_pt[3].GetX(),m_DMcodeResultArray[i].m_pt[3].GetY());
			m_pGuiPolylineArray[i].SetTipTextEnabled(TRUE);
			m_pGuiPolylineArray[i].SetTipText(strTmpKey);
			m_pGuiPolylineArray[i].SetVisible();
			m_pGuiSearchResultArray.push_back(m_pGuiPolylineArray + i);

			strTmpKey.Format(_T("%s_ResultTextArray%d"), m_strKeyText, i+1);
			CString strCode;
			strCode.Format("%d---%s",i+1,m_DMcodeResultArray[i].m_OutputData);
			m_pGuiTextArray[i].SetXYRotationText(m_DMcodeResultArray[i].m_pt[0].GetX(),m_DMcodeResultArray[i].m_pt[0].GetY(),0,strCode);
			m_pGuiTextArray[i].SetTipTextEnabled(TRUE);
			m_pGuiTextArray[i].SetTipText(strTmpKey);
			m_pGuiTextArray[i].SetVisible();
			m_pGuiTextArray[i].SetTextFont(&m_LogFont);
			m_pGuiSearchResultArray.push_back(m_pGuiTextArray + i);

			for (int j = 0;j < 4;j++)
			{
				strTmpKey.Format(_T("%s_ResultCircleArray%d"), m_strKeyText, i*4+j+1);
				m_pGuiCircleArray[i][j].SetTipTextEnabled(TRUE);
				m_pGuiCircleArray[i][j].SetTipText(strTmpKey);
				m_pGuiCircleArray[i][j].SetCenterRadius(m_DMcodeResultArray[i].m_pt[j],5*(j+1));
				m_pGuiCircleArray[i][j].SetVisible();
				m_pGuiSearchResultArray.push_back(m_pGuiCircleArray[i] + j);
			}

			
		}

		m_SearchResultArray.push_back(searchResult);
	}
	else if(m_TrainDataParam.m_nCodeType==1)// QR码
	{
		ConvertImagePostion(m_QRcodeResultArray,m_TrainDataParam);
		if(m_QRcodeResultArray.size()>0)
		{
			searchResult.m_dPosX = m_QRcodeResultArray[0].m_pt[0].GetX();
			searchResult.m_dPosY = m_QRcodeResultArray[0].m_pt[0].GetY();

			if(m_QRcodeResultArray[0].m_pt[0] != m_QRcodeResultArray[0].m_pt[1])
			{
				scLineSeg ResLineSeg;
				ResLineSeg.SetP1(m_QRcodeResultArray[0].m_pt[0]);
				ResLineSeg.SetP2(m_QRcodeResultArray[0].m_pt[1]);
				scLine ResLine;
				ResLine.SetFromLineSeg(ResLineSeg);
				searchResult.m_dAngle = scDegree(ResLine.GetRotation()).ToDouble();
			}

			for(int it=0;it<MaxAuxiliarySearchResultNum && it<4;it++)
			{
				searchResult.m_vdAuxiliaryPosX[it] = m_QRcodeResultArray[0].m_pt[it].GetX();
				searchResult.m_vdAuxiliaryPosY[it] = m_QRcodeResultArray[0].m_pt[it].GetY();
			}
		}

		for (int i = 0;i < m_QRcodeResultArray.size()&&i<DmCodeTool_MaxResultNum;i++)
		{
			searchResult.m_vcodeQRResult.push_back(m_QRcodeResultArray[i]);
			m_StrInspectInfor.Format(_T("%s"),m_QRcodeResultArray[0].m_OutputData);
			CResultSearchRect resRect;
			resRect.m_bFindRect = TRUE;
			resRect.m_searchRect.SetCornersPoPxPy(sc2Vector(m_QRcodeResultArray[i].m_pt[0].GetX(),m_QRcodeResultArray[i].m_pt[0].GetY()),
				sc2Vector(m_QRcodeResultArray[i].m_pt[1].GetX(),m_QRcodeResultArray[i].m_pt[1].GetY()),
				sc2Vector(m_QRcodeResultArray[i].m_pt[3].GetX(),m_QRcodeResultArray[i].m_pt[3].GetY()));
			searchResult.m_vTrainSearchRect.push_back(resRect);

			CString	strTmpKey;

			strTmpKey.Format(_T("%s_ResultPolyline%d"), m_strKeyText, i+1);
			m_pGuiPolylineArray[i].RemoveVertices();
			m_pGuiPolylineArray[i].AddVertex(m_QRcodeResultArray[i].m_pt[0].GetX(),m_QRcodeResultArray[i].m_pt[0].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_QRcodeResultArray[i].m_pt[1].GetX(),m_QRcodeResultArray[i].m_pt[1].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_QRcodeResultArray[i].m_pt[2].GetX(),m_QRcodeResultArray[i].m_pt[2].GetY());
			m_pGuiPolylineArray[i].AddVertex(m_QRcodeResultArray[i].m_pt[3].GetX(),m_QRcodeResultArray[i].m_pt[3].GetY());
			m_pGuiPolylineArray[i].SetTipTextEnabled(TRUE);
			m_pGuiPolylineArray[i].SetTipText(strTmpKey);
			m_pGuiPolylineArray[i].SetVisible();
			m_pGuiSearchResultArray.push_back(m_pGuiPolylineArray + i);

			strTmpKey.Format(_T("%s_ResultTextArray%d"), m_strKeyText, i+1);
			CString strCode;
			strCode.Format("%d---%s",i+1,m_QRcodeResultArray[i].m_OutputData);
			m_pGuiTextArray[i].SetXYRotationText(m_QRcodeResultArray[i].m_pt[0].GetX(),m_QRcodeResultArray[i].m_pt[0].GetY(),0,strCode);
			m_pGuiTextArray[i].SetTipTextEnabled(TRUE);
			m_pGuiTextArray[i].SetTipText(strTmpKey);
			m_pGuiTextArray[i].SetVisible();
			m_pGuiTextArray[i].SetTextFont(&m_LogFont);
			m_pGuiSearchResultArray.push_back(m_pGuiTextArray + i);

			for (int j = 0;j < 4;j++)
			{
				strTmpKey.Format(_T("%s_ResultCircleArray%d"), m_strKeyText, i*4+j+1);
				m_pGuiCircleArray[i][j].SetTipTextEnabled(TRUE);
				m_pGuiCircleArray[i][j].SetTipText(strTmpKey);
				m_pGuiCircleArray[i][j].SetCenterRadius(m_QRcodeResultArray[i].m_pt[j],5*(j+1));
				m_pGuiCircleArray[i][j].SetVisible();
				m_pGuiSearchResultArray.push_back(m_pGuiCircleArray[i] + j);
			}
		}
		
		m_SearchResultArray.push_back(searchResult);
	}

	return true;
}

// 获取搜索结果个数										
int CIdReaderDmCodeTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CIdReaderDmCodeTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	
				
// 加载模板文件
bool CIdReaderDmCodeTool::LoadModelFromFile(CString strDir)
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
	CIdReaderDmCodeTrainDataParam tmpTrainDataParam;
	CIdReaderDmCodeTrainGuiParam tmpTrainGuiParam;
	CIdReaderDmCodeSearchDataParam tmpSearchDataParam;
	CIdReaderDmCodeSearchGuiParam tmpSearchGuiParam;
	CIdReaderDmCodeTrainResult    tmpTrainResult;
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
			// 采样缩放系数
			bRet = cXMLConfigurator.FindElem(_T("CoarseScale"));
			if (!bRet){return false;}
			tmpTrainDataParam.m_CoarseScale = (CoarseScale)_ttoi(cXMLConfigurator.GetElemData());

			// 读码类型 0-DM；1-QR
			bRet = cXMLConfigurator.FindElem(_T("nReadCodeType"));
			if (bRet)
			{
				tmpTrainDataParam.m_nCodeType = _ttoi(cXMLConfigurator.GetElemData());
			}
			else
			{
				tmpTrainDataParam.m_nCodeType = 0;
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
			FindDMCodePara &findDMCodePara = tmpTrainDataParam.m_FindDMCodePara;
			bRet = cXMLConfigurator.FindElem(_T("FindDMCodePara"));
			if (!bRet)
			{
				return false;
			}
			cXMLConfigurator.EnterElem();
			{
				bRet = cXMLConfigurator.FindElem(_T("methodDecode"));
				if (!bRet){return false;}
				findDMCodePara.m_methodDecode = (DecodeMethod)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modeApp"));
				if (!bRet){return false;}
				findDMCodePara.m_modeApp = (AppMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagDiscrete"));
				if (!bRet){return false;}
				findDMCodePara.m_flagDiscrete = (DiscreteFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagDistortion"));
				if (!bRet){return false;}
				findDMCodePara.m_flagDistortion = (DistortionFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modeMirror"));
				if (!bRet){return false;}
				findDMCodePara.m_modeMirror = (MirrorMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modePolarity"));
				if (!bRet){return false;}
				findDMCodePara.m_modePolarity = (PolarityMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagRectangle"));
				if (!bRet){return false;}
				findDMCodePara.m_flagRectangle = (RectangleFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxCodeNum"));
				if (!bRet){return false;}
				findDMCodePara.m_nMaxCodeNum = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MinBarSize"));
				if (!bRet){return false;}
				findDMCodePara.m_nMinBarSize = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxBarSize"));
				if (!bRet){return false;}
				findDMCodePara.m_nMaxBarSize = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("SampleLevel"));
				if (!bRet){return false;}
				findDMCodePara.m_nSampleLevel = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("WaitingTime"));
				if (!bRet){return false;}
				findDMCodePara.m_nWaitingTime = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("DebugFlag"));
				if (!bRet){return false;}
				findDMCodePara.m_nDebugFlag = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("AdvanceParam"));
				if (!bRet){return false;}
				findDMCodePara.m_nAdvanceParam = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("AdvanceParam2"));
				if (!bRet){return false;}
				findDMCodePara.m_nAdvanceParam2 = _ttoi(cXMLConfigurator.GetElemData());
			}
			cXMLConfigurator.LeaveElem();

			FindQRCodePara &findQRCodePara = tmpTrainDataParam.m_FindQRCodePara;
			bRet = cXMLConfigurator.FindElem(_T("FindQRCodePara"));
			if (!bRet)
			{
				return false;
			}
			cXMLConfigurator.EnterElem();
			{
				bRet = cXMLConfigurator.FindElem(_T("methodDecode"));
				if (!bRet){return false;}
				findQRCodePara.m_methodDecode = (DecodeMethod)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modeApp"));
				if (!bRet){return false;}
				findQRCodePara.m_modeApp = (AppMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagDiscrete"));
				if (!bRet){return false;}
				findQRCodePara.m_flagDiscrete = (DiscreteFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagDistortion"));
				if (!bRet){return false;}
				findQRCodePara.m_flagDistortion = (DistortionFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modeMirror"));
				if (!bRet){return false;}
				findQRCodePara.m_modeMirror = (MirrorMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("modePolarity"));
				if (!bRet){return false;}
				findQRCodePara.m_modePolarity = (PolarityMode)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("flagRectangle"));
				if (!bRet){return false;}
				findQRCodePara.m_flagRectangle = (RectangleFlag)_ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxCodeNum"));
				if (!bRet){return false;}
				findQRCodePara.m_nMaxCodeNum = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MinBarSize"));
				if (!bRet){return false;}
				findQRCodePara.m_nMinBarSize = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("MaxBarSize"));
				if (!bRet){return false;}
				findQRCodePara.m_nMaxBarSize = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("SampleLevel"));
				if (!bRet){return false;}
				findQRCodePara.m_nSampleLevel = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("WaitingTime"));
				if (!bRet){return false;}
				findQRCodePara.m_nWaitingTime = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("DebugFlag"));
				if (!bRet){return false;}
				findQRCodePara.m_nDebugFlag = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("AdvanceParam"));
				if (!bRet){return false;}
				findQRCodePara.m_nAdvanceParam = _ttoi(cXMLConfigurator.GetElemData());

				bRet = cXMLConfigurator.FindElem(_T("AdvanceParam2"));
				if (!bRet){return false;}
				findQRCodePara.m_nAdvanceParam2 = _ttoi(cXMLConfigurator.GetElemData());
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
bool CIdReaderDmCodeTool::SaveModelToFile(CString strDir)
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

			// 采样缩放系数
			cXMLConfigurator.AddElem(_T("CoarseScale"), m_TrainDataParam.m_CoarseScale);

			// 读码类型 0-DM；1-QR
			cXMLConfigurator.AddElem(_T("nReadCodeType"), m_TrainDataParam.m_nCodeType);

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

			FindDMCodePara &findDMCodePara = m_TrainDataParam.m_FindDMCodePara;
			cXMLConfigurator.AddElem(_T("FindDMCodePara"));
			cXMLConfigurator.EnterElem();
			{
				cXMLConfigurator.AddElem(_T("methodDecode"), findDMCodePara.m_methodDecode);
				cXMLConfigurator.AddElem(_T("modeApp"), findDMCodePara.m_modeApp);
				cXMLConfigurator.AddElem(_T("flagDiscrete"), findDMCodePara.m_flagDiscrete);
				cXMLConfigurator.AddElem(_T("flagDistortion"), findDMCodePara.m_flagDistortion);
				cXMLConfigurator.AddElem(_T("modeMirror"), findDMCodePara.m_modeMirror);
				cXMLConfigurator.AddElem(_T("modePolarity"), findDMCodePara.m_modePolarity);
				cXMLConfigurator.AddElem(_T("flagRectangle"), findDMCodePara.m_flagRectangle);
				cXMLConfigurator.AddElem(_T("MaxCodeNum"), findDMCodePara.m_nMaxCodeNum);
				cXMLConfigurator.AddElem(_T("MinBarSize"), findDMCodePara.m_nMinBarSize);
				cXMLConfigurator.AddElem(_T("MaxBarSize"), findDMCodePara.m_nMaxBarSize);
				cXMLConfigurator.AddElem(_T("SampleLevel"), findDMCodePara.m_nSampleLevel);
				cXMLConfigurator.AddElem(_T("WaitingTime"), findDMCodePara.m_nWaitingTime);
				cXMLConfigurator.AddElem(_T("DebugFlag"), findDMCodePara.m_nDebugFlag);
				cXMLConfigurator.AddElem(_T("AdvanceParam"), findDMCodePara.m_nAdvanceParam);
				cXMLConfigurator.AddElem(_T("AdvanceParam2"), findDMCodePara.m_nAdvanceParam2);
			}
			cXMLConfigurator.LeaveElem();
			
			FindQRCodePara &findQRCodePara = m_TrainDataParam.m_FindQRCodePara;
			cXMLConfigurator.AddElem(_T("FindQRCodePara"));
			cXMLConfigurator.EnterElem();
			{
				cXMLConfigurator.AddElem(_T("methodDecode"), findQRCodePara.m_methodDecode);
				cXMLConfigurator.AddElem(_T("modeApp"), findQRCodePara.m_modeApp);
				cXMLConfigurator.AddElem(_T("flagDiscrete"), findQRCodePara.m_flagDiscrete);
				cXMLConfigurator.AddElem(_T("flagDistortion"), findQRCodePara.m_flagDistortion);
				cXMLConfigurator.AddElem(_T("modeMirror"), findQRCodePara.m_modeMirror);
				cXMLConfigurator.AddElem(_T("modePolarity"), findQRCodePara.m_modePolarity);
				cXMLConfigurator.AddElem(_T("flagRectangle"), findQRCodePara.m_flagRectangle);
				cXMLConfigurator.AddElem(_T("MaxCodeNum"), findQRCodePara.m_nMaxCodeNum);
				cXMLConfigurator.AddElem(_T("MinBarSize"), findQRCodePara.m_nMinBarSize);
				cXMLConfigurator.AddElem(_T("MaxBarSize"), findQRCodePara.m_nMaxBarSize);
				cXMLConfigurator.AddElem(_T("SampleLevel"), findQRCodePara.m_nSampleLevel);
				cXMLConfigurator.AddElem(_T("WaitingTime"), findQRCodePara.m_nWaitingTime);
				cXMLConfigurator.AddElem(_T("DebugFlag"), findQRCodePara.m_nDebugFlag);
				cXMLConfigurator.AddElem(_T("AdvanceParam"), findQRCodePara.m_nAdvanceParam);
				cXMLConfigurator.AddElem(_T("AdvanceParam2"), findQRCodePara.m_nAdvanceParam2);
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
bool CIdReaderDmCodeTool::UpdateTrainGuiData(bool bSaveAndValidate)
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

bool CIdReaderDmCodeTool::UpdateSearchGuiData(bool bSaveAndValidate)
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
bool CIdReaderDmCodeTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{		
	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件				
bool CIdReaderDmCodeTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CIdReaderDmCodeTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchResultArray);

	return true;
}

bool CIdReaderDmCodeTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_TrainRect"));


	}
	
	return true;
}

bool CIdReaderDmCodeTool::ClearResultGui()
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
bool CIdReaderDmCodeTool::GetPatternImage(cpImage &ImagePattern)
{
	if(m_tmpImageTrain.IsValid())
	{
		ImagePattern = m_tmpImageTrain;
		return true;
	}
	return false;
}	

// 恢复训练参数
bool CIdReaderDmCodeTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CIdReaderDmCodeTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CIdReaderDmCodeTool::SetLanguage(int nLanguage)
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

		break;
	case 1:
		m_GuiTrainRect.SetLabel(_T("训练区域"));

		break;
	default:
		m_GuiTrainRect.SetLabel(_T("训练区域"));
		break;
	}
}

CString CIdReaderDmCodeTool::GetResultErrInfo()
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
				strErrInfo = m_StrInspectInfor;
				break;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("Image Conversion failed");
				break;
			case -2:			//	DM码搜索失败			
				strErrInfo += _T("DM Code Search failed");
				break;
			case -3:			//	QR码搜索失败			
				strErrInfo += _T("QR Code Search failed");
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
				strErrInfo = m_StrInspectInfor;
				break;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("彩色图像转换失败");
				break;
			case -2:			//	DM码搜索失败			
				strErrInfo += _T("DM码搜索失败");
				break;
			case -3:			//	QR码搜索失败			
				strErrInfo += _T("QR码搜索失败");
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
				break;
			case -1:			//	彩色图像转换失败			
				strErrInfo += _T("彩色图像转换失败");
				break;
			case -2:			//	DM码搜索失败			
				strErrInfo += _T("DM码搜索失败");
				break;
			case -3:			//	QR码搜索失败			
				strErrInfo += _T("QR码搜索失败");
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

void CIdReaderDmCodeTool::ConvertImagePostion(svStd vector<DecodeMatrixResult> &m_DMcodeResultArray,CIdReaderDmCodeTrainDataParam m_TrainDataParam)
{
	double dPow = pow(2.0, m_TrainDataParam.m_CoarseScale - 1);
	for(int it=0;it<m_DMcodeResultArray.size();it++)
	{
		for(int jt=0;jt<4;jt++)
		{
			double dX = m_DMcodeResultArray.at(it).m_pt[jt].GetX();
			double dY = m_DMcodeResultArray.at(it).m_pt[jt].GetY();
			m_DMcodeResultArray.at(it).m_pt[jt] = sc2Vector(dX*dPow,dY*dPow);
		}
	}
}
void CIdReaderDmCodeTool::ConvertImagePostion(svStd vector<DecodeQRResult> &m_QRcodeResultArray,CIdReaderDmCodeTrainDataParam m_TrainDataParam)
{
	double dPow = pow(2.0, m_TrainDataParam.m_CoarseScale - 1);
	for(int it=0;it<m_QRcodeResultArray.size();it++)
	{
		for(int jt=0;jt<4;jt++)
		{
			double dX = m_QRcodeResultArray.at(it).m_pt[jt].GetX();
			double dY = m_QRcodeResultArray.at(it).m_pt[jt].GetY();
			m_QRcodeResultArray.at(it).m_pt[jt] = sc2Vector(dX*dPow,dY*dPow);
		}
	}
}