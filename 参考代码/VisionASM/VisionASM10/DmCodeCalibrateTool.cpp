#include "stdafx.h"
#include "DmCodeCalibrateTool.h"
#include "svXMLConfigurator.h"

CDmCodeCalibrateTool::CDmCodeCalibrateTool(void)
{
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_strError                                  =_T("");
	m_GuiDmCodeSearchRect.SetCenterWidthHeight(376,240,652,380);
	m_GuiDmCodeSearchRect.SetLabelVisible(TRUE);
	m_GuiCornersSearchRect.SetCenterWidthHeight(376,240,732,460);
	m_GuiCornersSearchRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiDmCodeSearchRect.SetLabel(_T("DmCode Search Area"));
		m_GuiCornersSearchRect.SetLabel(_T("Corners Search Area"));
		break;
	case 1:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	default:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	}

	CreateGUID(m_strKeyText);
	m_tmpTrainGuiParam.m_DmCodeSearchRect = m_GuiDmCodeSearchRect.GetRect();
	m_tmpTrainGuiParam.m_CornersSearchRect = m_GuiCornersSearchRect.GetRect();
	m_TrainDataParam							= m_tmpTrainDataParam;
	m_TrainGuiParam								= m_tmpTrainGuiParam;
	m_SearchDataParam							= m_tmpSearchDataParam;
	m_SearchGuiParam							= m_tmpSearchGuiParam;
}

CDmCodeCalibrateTool::CDmCodeCalibrateTool(CDmCodeCalibrateTool& cDmCodeCalibrateTool)
{
	if (cDmCodeCalibrateTool.GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return;
	}
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_strError                                  =_T("");
	m_GuiDmCodeSearchRect.SetCenterWidthHeight(350,240,100,100);
	m_GuiDmCodeSearchRect.SetLabelVisible(TRUE);
	m_GuiCornersSearchRect.SetCenterWidthHeight(350,240,652,380);
	m_GuiCornersSearchRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiDmCodeSearchRect.SetLabel(_T("DmCode Search Area"));
		m_GuiCornersSearchRect.SetLabel(_T("Corners Search Area"));
		break;
	case 1:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	default:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	}
	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cDmCodeCalibrateTool.m_CamHeight;
	m_CamWidth								= cDmCodeCalibrateTool.m_CamWidth;

	m_TrainDataParam						= cDmCodeCalibrateTool.m_TrainDataParam;
	m_TrainGuiParam							= cDmCodeCalibrateTool.m_TrainGuiParam;
	m_SearchDataParam						= cDmCodeCalibrateTool.m_SearchDataParam;
	m_SearchGuiParam						= cDmCodeCalibrateTool.m_SearchGuiParam;
	m_TrainResult							= cDmCodeCalibrateTool.m_TrainResult;
	m_ImageTrain							= cDmCodeCalibrateTool.m_ImageTrain;
	m_ImageMask								= cDmCodeCalibrateTool.m_ImageMask;
	m_bTrainGuiShow							= cDmCodeCalibrateTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cDmCodeCalibrateTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cDmCodeCalibrateTool.m_bTrainOK;
	m_pGuiDisplay							= cDmCodeCalibrateTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cDmCodeCalibrateTool.UpdateSearchGuiData(true);
	cDmCodeCalibrateTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cDmCodeCalibrateTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cDmCodeCalibrateTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cDmCodeCalibrateTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cDmCodeCalibrateTool.m_tmpSearchGuiParam);
	SetTrainImage(cDmCodeCalibrateTool.m_tmpImageTrain);
	SetMaskImage(cDmCodeCalibrateTool.m_tmpImageMask);

}
CDmCodeCalibrateTool& CDmCodeCalibrateTool::operator=(CDmCodeCalibrateTool& cDmCodeCalibrateTool)
{
	if (this == &cDmCodeCalibrateTool)
	{
		return *this;	
	}

	if (cDmCodeCalibrateTool.GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return *this;
	}
	// 清空
	ClearAllGui();
	ClearResult();
	m_pGuiDisplay								= NULL;
	m_hDispWnd									= NULL;
	m_pGuiInteractiveContainer					= NULL;
	m_pGuiStaticContainer						= NULL;
	m_strKeyText								= _T("");
	m_bTrainOK									= FALSE;
	m_bTrainGuiShow								= true;
	m_bSearchResultGuiShow						= false;
	m_strError                                  =_T("");
	m_GuiDmCodeSearchRect.SetCenterWidthHeight(350,240,100,100);
	m_GuiDmCodeSearchRect.SetLabelVisible(TRUE);
	m_GuiCornersSearchRect.SetCenterWidthHeight(350,240,652,380);
	m_GuiCornersSearchRect.SetLabelVisible(TRUE);

	switch (m_iLanguage)
	{
	case 0:
		m_GuiDmCodeSearchRect.SetLabel(_T("DmCode Search Area"));
		m_GuiCornersSearchRect.SetLabel(_T("Corners Search Area"));
		break;
	case 1:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	default:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	}
	CreateGUID(m_strKeyText);

	// 赋值
	m_CamHeight								= cDmCodeCalibrateTool.m_CamHeight;
	m_CamWidth								= cDmCodeCalibrateTool.m_CamWidth;

	m_TrainDataParam						= cDmCodeCalibrateTool.m_TrainDataParam;
	m_TrainGuiParam							= cDmCodeCalibrateTool.m_TrainGuiParam;
	m_SearchDataParam						= cDmCodeCalibrateTool.m_SearchDataParam;
	m_SearchGuiParam						= cDmCodeCalibrateTool.m_SearchGuiParam;
	m_TrainResult							= cDmCodeCalibrateTool.m_TrainResult;
	m_ImageTrain							= cDmCodeCalibrateTool.m_ImageTrain;
	m_ImageMask								= cDmCodeCalibrateTool.m_ImageMask;
	m_bTrainGuiShow							= cDmCodeCalibrateTool.m_bTrainGuiShow;
	m_bSearchResultGuiShow					= cDmCodeCalibrateTool.m_bSearchResultGuiShow;
	m_bTrainOK								= cDmCodeCalibrateTool.m_bTrainOK;
	m_pGuiDisplay							= cDmCodeCalibrateTool.m_pGuiDisplay;
	if (m_pGuiDisplay)
	{
		SetDisplay(m_pGuiDisplay);
	}
	cDmCodeCalibrateTool.UpdateSearchGuiData(true);
	cDmCodeCalibrateTool.UpdateTrainGuiData(true);
	SetTrainDataParam(&cDmCodeCalibrateTool.m_tmpTrainDataParam);
	SetTrainGuiParam(&cDmCodeCalibrateTool.m_tmpTrainGuiParam);
	SetSearchDataParam(&cDmCodeCalibrateTool.m_tmpSearchDataParam);
	SetSearchGuiParam(&cDmCodeCalibrateTool.m_tmpSearchGuiParam);
	SetTrainImage(cDmCodeCalibrateTool.m_tmpImageTrain);
	SetMaskImage(cDmCodeCalibrateTool.m_tmpImageMask);
	return *this;	
}
CDmCodeCalibrateTool::~CDmCodeCalibrateTool(void)
{
	// 清空
	ClearAllGui();
	ClearResult();
}
// 获取工具类型
CSearchToolType CDmCodeCalibrateTool::GetSearchToolType()
{
	return eDmCodeCalibrateTool;
}										   

// 设置显示控件指针		
bool CDmCodeCalibrateTool::SetDisplay(scGuiDisplay *pGuiDisplay)
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
scGuiDisplay* CDmCodeCalibrateTool::GetDisplay()
{
	return m_pGuiDisplay;
}

// 设置Gui图形的关键字														
bool CDmCodeCalibrateTool::SetGuiKeyText(CString strGuiKeyText)									
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
bool CDmCodeCalibrateTool::GetGuiKeyText(CString &strGuiKeyText)
{
	strGuiKeyText = m_strKeyText;

	return true;
}

// 更新训练界面显示					
bool CDmCodeCalibrateTool::ShowTrainGui(bool bShow)
{
	m_bTrainGuiShow = bShow;

	UpdateTrainGuiDisplay();

	return true;
}

// 是否显示训练结果界面
bool CDmCodeCalibrateTool::ShowTrainResultGui(bool bShow)
{
	return false;
}	

// 更新搜索界面显示									
bool CDmCodeCalibrateTool::ShowSearchResultGui(bool bShow)							
{
	m_bSearchResultGuiShow = bShow;

	UpdateSearchResultGuiDisplay();

	return true;
}

// 更新训练界面显示					
bool CDmCodeCalibrateTool::UpdateTrainGuiDisplay()
{	
	m_GuiDmCodeSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bDmCodeSearchRectVisible);
	m_GuiCornersSearchRect.SetVisible(m_bTrainGuiShow && m_tmpTrainGuiParam.m_bCornersSearchRectVisible);

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();
		m_pGuiInteractiveContainer->AddItem(&m_GuiDmCodeSearchRect, m_strKeyText+_T("_DmCodeSearchRect"));
		m_pGuiInteractiveContainer->AddItem(&m_GuiCornersSearchRect, m_strKeyText+_T("_CornersSearchRect"));	

		m_pGuiDisplay->SetInteractiveContainer(m_pGuiInteractiveContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新搜索界面显示									
bool CDmCodeCalibrateTool::UpdateSearchResultGuiDisplay()							
{
	int i=0;

	for (i=0; i<m_pGuiSearchPolylineArray.size(); i++)
	{
		m_pGuiSearchPolylineArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_DmCodePolylineVisible);
	}

	for (i=0; i<m_pGuiSearchCoordCrossArray.size(); i++)
	{
		m_pGuiSearchCoordCrossArray[i]->SetVisible(m_bSearchResultGuiShow && m_tmpSearchGuiParam.m_bCornersCoordCrossVisible);
	}

	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{	
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		int i=0;

		for (i=0; i<m_pGuiSearchPolylineArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchPolylineArray[i], m_pGuiSearchPolylineArray[i]->GetTipText());
		}

		for (i=0; i<m_pGuiSearchCoordCrossArray.size(); i++)
		{
			m_pGuiStaticContainer->AddItem(m_pGuiSearchCoordCrossArray[i], m_pGuiSearchCoordCrossArray[i]->GetTipText());
		}	

		m_pGuiDisplay->SetStaticContainer(m_pGuiStaticContainer);
		m_pGuiDisplay->MyInvalidate();			
	}

	return true;
}

// 更新界面显示
bool CDmCodeCalibrateTool::UpdateGuiDisplay()
{
	UpdateTrainGuiDisplay();

	UpdateSearchResultGuiDisplay();

	return true;
}

// 设置训练Data参数	
bool CDmCodeCalibrateTool::SetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	m_tmpTrainDataParam = *((CDmCodeCalibrateTrainDataParam*)pTrainDataParam);

	return true;
}

// 获取训练Data参数									
bool CDmCodeCalibrateTool::GetTrainDataParam(CBaseTrainDataParam *pTrainDataParam)
{
	if (!pTrainDataParam)
	{
		return false;
	}

	if (pTrainDataParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	*((CDmCodeCalibrateTrainDataParam*)pTrainDataParam) = m_tmpTrainDataParam;

	return true;
}

// 获取搜索Gui参数						
bool CDmCodeCalibrateTool::SetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	m_tmpTrainGuiParam = *((CDmCodeCalibrateTrainGuiParam*)pTrainGuiParam);

	UpdateTrainGuiData(false);

	return true;
}

// 获取搜索Gui参数							
bool CDmCodeCalibrateTool::GetTrainGuiParam(CBaseTrainGuiParam *pTrainGuiParam)
{
	if (!pTrainGuiParam)
	{
		return false;
	}

	if (pTrainGuiParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	UpdateTrainGuiData(true);

	*((CDmCodeCalibrateTrainGuiParam*)pTrainGuiParam) = m_tmpTrainGuiParam;

	return true;
}

// 设置训练原始图像				
bool CDmCodeCalibrateTool::SetTrainImage(const cpImage &ImageTrain)
{
	m_tmpImageTrain = ImageTrain;

	return true;
}

// 获取训练原始图像									
bool CDmCodeCalibrateTool::GetTrainImage(cpImage &ImageTrain)
{
	ImageTrain = m_tmpImageTrain;

	return true;
}

// 设置训练原始图像									
bool CDmCodeCalibrateTool::SetMaskImage(const cpImage &ImageMask)
{
	m_tmpImageMask = ImageMask;

	return true;
}

// 获取训练原始图像								
bool CDmCodeCalibrateTool::GetMaskImage(cpImage &ImageMask)
{
	ImageMask = m_tmpImageMask;

	return true;
}

// 执行训练											
bool CDmCodeCalibrateTool::Train()
{
	m_bTrainOK =true;
	return true;
}

// 获取训练结果图像														
bool CDmCodeCalibrateTool::GetTrainResultImage(cpImage &ImageTrainResult)
{

	ImageTrainResult = m_tmpImageTrain;

	return true;
}	

// 获取训练结果
bool CDmCodeCalibrateTool::GetTrainResult(CBaseTrainResult *pTrainResult)
{
	if (!pTrainResult)
	{
		return false;
	}

	if (pTrainResult->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	*((CDmCodeCalibrateTrainResult*)pTrainResult) = m_tmpTrainResult;

	return true;
}							

// 是否训练成功
bool CDmCodeCalibrateTool::IsTrained()
{
	return m_bTrainOK;
}

// 设置搜索Data参数	
bool CDmCodeCalibrateTool::SetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{	
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	m_tmpSearchDataParam = *((CDmCodeCalibrateSearchDataParam*)pSearchDataParam);

	return true;
}

// 获取搜索Data参数				
bool CDmCodeCalibrateTool::GetSearchDataParam(CBaseSearchDataParam *pSearchDataParam)
{
	if (!pSearchDataParam)
	{
		return false;
	}

	if (pSearchDataParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	*((CDmCodeCalibrateSearchDataParam*)pSearchDataParam) = m_tmpSearchDataParam;

	return true;
}

// 设置搜索Gui参数					
bool CDmCodeCalibrateTool::SetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	m_tmpSearchGuiParam = *((CDmCodeCalibrateSearchGuiParam*)pSearchGuiParam);

	UpdateSearchGuiData(false);

	return true;
}

// 获取搜索Gui参数			
bool CDmCodeCalibrateTool::GetSearchGuiParam(CBaseSearchGuiParam *pSearchGuiParam)
{
	if (!pSearchGuiParam)
	{
		return false;
	}

	if (pSearchGuiParam->GetSearchToolType() != eDmCodeCalibrateTool)
	{
		return false;
	}

	UpdateSearchGuiData(true);

	*((CDmCodeCalibrateSearchGuiParam*)pSearchGuiParam) = m_tmpSearchGuiParam;

	return true;
}

// 执行搜索 					
bool CDmCodeCalibrateTool::Search(cpImage &ImageSearch)
{
	UpdateTrainGuiData(TRUE);
	if (!m_bTrainOK)
	{
		return false;
	}
	// 清空
	ClearResult();
	m_nResultErrNumber=0;
	/*m_DmCodeCalibrateTool.SetFindCornerPara(m_tmpSearchDataParam.m_DmCodeSearchParam.m_dGridSize
																	,m_tmpSearchDataParam.m_DmCodeSearchParam.m_dGridThre
																	,m_tmpSearchDataParam.m_DmCodeSearchParam.m_bOutputOutPoints);
*/
	m_DmCodeCalibrateTool.SetFindCornerPara(m_tmpSearchDataParam.m_DmCodeSearchParam.m_FindCornerPara);
	/*m_DmCodeCalibrateTool.SetFindCodePara(m_tmpSearchDataParam.m_DmCodeSearchParam.m_nDMCodeMaxNum
										,m_tmpSearchDataParam.m_DmCodeSearchParam.m_nMaxDecodeTime
										,m_tmpSearchDataParam.m_DmCodeSearchParam.m_bDecodeImageMirror
										,m_tmpSearchDataParam.m_DmCodeSearchParam.m_bEnableReDecode);*/
	m_DmCodeCalibrateTool.SetFindDMCodePara(m_tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara);
	m_DmCodeCalibrateTool.EnableReDecode(m_tmpSearchDataParam.m_DmCodeSearchParam.m_bEnableReDecode);

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
	int nErrorCode;
	bool bRet = false;
	if(bIsProcessImage)
	{
		bRet = m_DmCodeCalibrateTool.Execute(tmpImage,m_tmpTrainGuiParam.m_CornersSearchRect,m_tmpTrainGuiParam.m_DmCodeSearchRect
			,m_vImgDecodeCorners,m_vImgDecodeCornersLabel,m_vArrayDMCodesResult,nErrorCode);
	}
	else
	{
		bRet = m_DmCodeCalibrateTool.Execute(ImageSearch,m_tmpTrainGuiParam.m_CornersSearchRect,m_tmpTrainGuiParam.m_DmCodeSearchRect
			,m_vImgDecodeCorners,m_vImgDecodeCornersLabel,m_vArrayDMCodesResult,nErrorCode);
	}
	
	CSearchResult searchResult;
	if (bRet)
	{
		
		// 获取结果
		for (int i=0;i<m_vArrayDMCodesResult.size();i++)
		{
			searchResult.m_dPosX = m_vArrayDMCodesResult.at(i).m_CodeCenterPixelCoordinate.GetX();
			searchResult.m_dPosY = m_vArrayDMCodesResult.at(i).m_CodeCenterPixelCoordinate.GetY();
			searchResult.m_dAngle = 0;
			searchResult.m_dScore = 1;
			m_SearchResultArray.push_back(searchResult);

			CString	strTmpKey;
			strTmpKey.Format(_T("%s_ResultPolyline%d"), m_strKeyText, i);
			scGuiPolyline *pTempPolyline = m_GuiSearchDmCodePolyline+i;
			pTempPolyline->SetPolyline(m_vArrayDMCodesResult.at(i).m_CodePolyline);
			pTempPolyline->SetLineColor(RGB(255,255,0));
			pTempPolyline->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
			pTempPolyline->SetTipTextEnabled(true);
			pTempPolyline->SetTipText((LPCTSTR)strTmpKey);
			pTempPolyline->SetLabel(m_vArrayDMCodesResult.at(i).m_CodeString);
			pTempPolyline->SetLabelVisible(TRUE);
			m_pGuiSearchPolylineArray.push_back(pTempPolyline);
		}
		for (int i=0;i<m_vImgDecodeCorners.size() && i<MAXCORNERSNUM;i++)
		{
			CString	strTmpKey;
			strTmpKey.Format(_T("%s_ResultCoordCross%d"), m_strKeyText, i);
			scGuiCoordCross *pTempCoorCross = m_GuiSearchCornersCoordCross+i;
			pTempCoorCross->SetCenterRotationLengths(m_vImgDecodeCorners.at(i),scRadian(0),50,50);
			pTempCoorCross->SetLineColor(RGB(0,255,0));
			pTempCoorCross->SetLineWidth(2);
			pTempCoorCross->SetTipTextEnabled(true);
			pTempCoorCross->SetTipText((LPCTSTR)strTmpKey);
			
			m_pGuiSearchCoordCrossArray.push_back(pTempCoorCross);
		}

		m_SearchDataParam = m_tmpSearchDataParam;
		m_TrainDataParam  = m_tmpTrainDataParam;
		m_SearchGuiParam  = m_tmpSearchGuiParam;
		m_TrainGuiParam   = m_tmpTrainGuiParam;
		m_ImageTrain      = m_tmpImageTrain;
	}
	else
	{
		
		switch(m_iLanguage)
		{
		case 0:
			{
				switch(nErrorCode)
				{
				case -1:
					{					// 输入图像无效
						m_strError = _T("The input image is invalid");
					}
					break;
				case -2:
					{					// 输入棋盘格大小无效（<=0）
						m_strError = _T("The chessboard size is invalid(<=0)");
					}
					break;
				case -3:
					{					// 获取标定子图像失败
						m_strError = _T("Get the calibrate roi image failed");
					}
					break;
				case -4:
					{					// 检测标定角点失败
						m_strError = _T("Find the calibrate corner point failed");
					}
					break;
				case -5:
					{					// 检测到的标定角点数量少于5"
						m_strError = _T("Find the calibrate corner point num less than 5");
					}
					break;
				case -6:
					{					// 检测二维码失败
						m_strError = _T("Find 2D barcode failed");
					}
					break;
				case -7:
					{					// 检测到的二维码数量为0"
						m_strError = _T("Find 2D barcode num is 0");
					}
					break;
				case -8:
					{					// 重新检测二维码失败
						m_strError = _T("Refind 2D barcode failed");
					}
					break;
				case -9:
					{					// 重新检测到的二维码数量为0
						m_strError = _T("Refind 2D barcode num is 0");
					}
					break;
				case -10:
					{					// 重新检测到的二维码数量错误
						m_strError = _T("Refind 2D barcode num is error");
					}
					break;
				case -11:
					{					// 多个二维码的XY坐标系不一致
						m_strError = _T("The CoordinateXY of some 2D barcodes is not identical ");
					}
					break;
				case -12:
					{					// 多个二维码的X方向不一致
						m_strError = _T("The CoordinateX of some 2D barcodes is not identical ");
					}
					break;
				case -13:
					{					// 多个二维码的Y方向不一致
						m_strError = _T("The CoordinateY of some 2D barcodes is not identical ");
					}
					break;
				case -14:
					{					// 多个二维码的偏移值不一致
						m_strError = _T("The Offset of some 2D barcodes is not identical ");
					}
					break;
				default:
					{					// 搜索二维码失败
						m_strError = _T("Find 2D barcode failed");
					}
					break;
				}
			}
			break;
		case 1:
			{
				switch(nErrorCode)
				{
				case -1:
					{
						m_strError = _T("输入图像无效");
					}
					break;
				case -2:
					{
						m_strError = _T("输入棋盘格大小无效（<=0）");
					}
					break;
				case -3:
					{
						m_strError = _T("获取标定子图像失败");
					}
					break;
				case -4:
					{
						m_strError = _T("检测标定角点失败");
					}
					break;
				case -5:
					{
						m_strError = _T("检测到的标定角点数量少于5");
					}
					break;
				case -6:
					{
						m_strError = _T("检测二维码失败");
					}
					break;
				case -7:
					{
						m_strError = _T("检测到的二维码数量为0");
					}
					break;
				case -8:
					{
						m_strError = _T("重新检测二维码失败");
					}
					break;
				case -9:
					{
						m_strError = _T("重新检测到的二维码数量为0");
					}
					break;
				case -10:
					{
						m_strError = _T("重新检测到的二维码数量错误");
					}
					break;
				case -11:
					{
						m_strError = _T("多个二维码的XY坐标系不一致");
					}
					break;
				case -12:
					{
						m_strError = _T("多个二维码的X方向不一致");
					}
					break;
				case -13:
					{
						m_strError = _T("多个二维码的Y方向不一致");
					}
					break;
				case -14:
					{
						m_strError = _T("多个二维码的偏移值不一致");
					}
					break;
				default:
					{
						m_strError = _T("搜索二维码失败");
					}
					break;
				}
			}
			break;
		default:
			{
				switch(nErrorCode)
				{
				case -1:
					{
						m_strError = _T("输入图像无效");
					}
					break;
				case -2:
					{
						m_strError = _T("输入棋盘格大小无效（<=0）");
					}
					break;
				case -3:
					{
						m_strError = _T("获取标定子图像失败");
					}
					break;
				case -4:
					{
						m_strError = _T("检测标定角点失败");
					}
					break;
				case -5:
					{
						m_strError = _T("检测到的标定角点数量少于5");
					}
					break;
				case -6:
					{
						m_strError = _T("检测二维码失败");
					}
					break;
				case -7:
					{
						m_strError = _T("检测到的二维码数量为0");
					}
					break;
				case -8:
					{
						m_strError = _T("重新检测二维码失败");
					}
					break;
				case -9:
					{
						m_strError = _T("重新检测到的二维码数量为0");
					}
					break;
				case -10:
					{
						m_strError = _T("重新检测到的二维码数量错误");
					}
					break;
				case -11:
					{
						m_strError = _T("多个二维码的XY坐标系不一致");
					}
					break;
				case -12:
					{
						m_strError = _T("多个二维码的X方向不一致");
					}
					break;
				case -13:
					{
						m_strError = _T("多个二维码的Y方向不一致");
					}
					break;
				case -14:
					{
						m_strError = _T("多个二维码的偏移值不一致");
					}
					break;
				default:
					{
						m_strError = _T("搜索二维码失败");
					}
					break;
				}
			}
			break;
		}

	}

	return bRet;
}

// 获取搜索结果个数										
int CDmCodeCalibrateTool::GetResultNum()
{
	return m_SearchResultArray.size();
}

// 获取搜索结果														
bool CDmCodeCalibrateTool::GetResult(int nIndex, CSearchResult &searchResult)
{
	if (nIndex < 0 || nIndex >= m_SearchResultArray.size())
	{
		return false;
	}

	searchResult = m_SearchResultArray[nIndex];

	return true;
}	

// 加载模板训练图像
bool CDmCodeCalibrateTool::LoadModelTrainPatternImage(CString strDir ,cpImage &ImagePattern)
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
	if (cSearchToolType != eDmCodeCalibrateTool)
	{
		return false;
	}

	/////////////TrainDataParam//////////////
	cXMLConfigurator.ResetCurElemPos();
	//bRet = cXMLConfigurator.FindElem(_T("TrainParam"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//cXMLConfigurator.EnterElem();
	//// Train rect
	//bRet = cXMLConfigurator.FindElem(_T("DmCodeSearchRect"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//cXMLConfigurator.EnterElem();
	//bRet = cXMLConfigurator.FindElem(_T("OriginX"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	//bRet = cXMLConfigurator.FindElem(_T("OriginY"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	//rRect.SetOrigion(vPos);
	//bRet = cXMLConfigurator.FindElem(_T("SizeX"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//vPos.SetX(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	//bRet = cXMLConfigurator.FindElem(_T("SizeY"));
	//if (!bRet)
	//{
	//	return false;
	//}
	//vPos.SetY(_tcstod(cXMLConfigurator.GetElemData(), NULL));
	//rRect.SetSize(vPos);
	//cXMLConfigurator.LeaveElem();

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
	/*if (!IsSuccess(hStatus))
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
	}*/
	return true;
}

// 加载模板文件
bool CDmCodeCalibrateTool::LoadModelFromFile(CString strDir)
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
	CDmCodeCalibrateTrainDataParam tmpTrainDataParam;
	CDmCodeCalibrateTrainGuiParam tmpTrainGuiParam;
	CDmCodeCalibrateSearchDataParam tmpSearchDataParam;
	CDmCodeCalibrateSearchGuiParam tmpSearchGuiParam;	
	CDmCodeCalibrateTrainResult    tmpTrainResult;

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
	if (cSearchToolType != eDmCodeCalibrateTool)
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

	

	bRet = cXMLConfigurator.FindElem(_T("DmCodeSearchRect"));
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
	tmpTrainGuiParam.m_DmCodeSearchRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_DmCodeSearchRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();
	// search rect
	bRet = cXMLConfigurator.FindElem(_T("CornersSearchRect"));
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
	tmpTrainGuiParam.m_CornersSearchRect.SetOrigion(vPos);
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
	tmpTrainGuiParam.m_CornersSearchRect.SetSize(vPos);
	cXMLConfigurator.LeaveElem();

	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	bRet = cXMLConfigurator.FindElem(_T("SearchParam"));
	if (!bRet)
	{
		return false;
	}
	cXMLConfigurator.EnterElem();
	bRet = cXMLConfigurator.FindElem(_T("DmCodeMaxNum"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxCodeNum = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("MaxDecodeTime"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nWaitingTime = _ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("EnableImageMirror"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modeMirror = (MirrorMode)_ttoi(cXMLConfigurator.GetElemData());
	
	bRet = cXMLConfigurator.FindElem(_T("DecodeMethod"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_methodDecode = (DecodeMethod)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AppMode"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modeApp = (AppMode)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DiscreteFlag"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagDiscrete = (DiscreteFlag)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DistortionFlag"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagDistortion = (DistortionFlag)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("PolarityMode"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modePolarity = (PolarityMode)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("RectangleFlag"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagRectangle = (RectangleFlag)_ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("MinBarSize"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMinBarSize = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("MaxBarSize"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxBarSize = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("SampleLevel"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nSampleLevel = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("DebugFlag"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nDebugFlag = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AdvanceParam"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nAdvanceParam = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("AdvanceParam2"));
	if (bRet)
	{
		tmpSearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nAdvanceParam2 = _ttoi(cXMLConfigurator.GetElemData());
	}

	bRet = cXMLConfigurator.FindElem(_T("EnableReDecode"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_bEnableReDecode = (BOOL)_ttoi(cXMLConfigurator.GetElemData());
	bRet = cXMLConfigurator.FindElem(_T("GridThre"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_dGridThre = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("GridSize"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_dGridSize = _tcstod(cXMLConfigurator.GetElemData(), NULL);
	bRet = cXMLConfigurator.FindElem(_T("EnableShowOutputPoints"));
	if (!bRet)
	{
		return false;
	}
	tmpSearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_bOutputOutPoints = (BOOL)_ttoi(cXMLConfigurator.GetElemData());

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

	// 数据赋值
	m_tmpImageTrain = tmpImageModel;

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
	m_bTrainOK = true;

	////////////////////////显示更新///////////////////////////////
	UpdateTrainGuiData(false);
	UpdateSearchGuiData(false);
	m_bTrainOK = true;
	return true;
}

// 保存模板文件							
bool CDmCodeCalibrateTool::SaveModelToFile(CString strDir)
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
	cXMLConfigurator.AddElem(_T("DmCodeSearchRect"));
	cXMLConfigurator.EnterElem();
	// ImageChannel
	strInfo.Format(_T("%d"), m_TrainDataParam.m_nImageChannel);
	cXMLConfigurator.AddElem(_T("ImageChannel"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_DmCodeSearchRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_DmCodeSearchRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_DmCodeSearchRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_DmCodeSearchRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();
	// search rect
	cXMLConfigurator.AddElem(_T("CornersSearchRect"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CornersSearchRect.GetOrigin().GetX());
	cXMLConfigurator.AddElem(_T("OriginX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CornersSearchRect.GetOrigin().GetY());
	cXMLConfigurator.AddElem(_T("OriginY"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CornersSearchRect.GetSize().GetX());
	cXMLConfigurator.AddElem(_T("SizeX"), strInfo);
	strInfo.Format(_T("%f"), m_TrainGuiParam.m_CornersSearchRect.GetSize().GetY());
	cXMLConfigurator.AddElem(_T("SizeY"), strInfo);
	cXMLConfigurator.LeaveElem();
	cXMLConfigurator.LeaveElem();

	//////////////SearchDataParam//////////////
	cXMLConfigurator.AddElem(_T("SearchParam"));
	cXMLConfigurator.EnterElem();
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxCodeNum);
	cXMLConfigurator.AddElem(_T("DmCodeMaxNum"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nWaitingTime);
	cXMLConfigurator.AddElem(_T("MaxDecodeTime"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modeMirror);
	cXMLConfigurator.AddElem(_T("EnableImageMirror"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_methodDecode);
	cXMLConfigurator.AddElem(_T("DecodeMethod"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modeApp);
	cXMLConfigurator.AddElem(_T("AppMode"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagDiscrete);
	cXMLConfigurator.AddElem(_T("DiscreteFlag"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagDistortion);
	cXMLConfigurator.AddElem(_T("DistortionFlag"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_modePolarity);
	cXMLConfigurator.AddElem(_T("m_modePolarity"), strInfo);
	strInfo.Format(_T("%d"), (int)m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_flagRectangle);
	cXMLConfigurator.AddElem(_T("RectangleFlag"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMinBarSize);
	cXMLConfigurator.AddElem(_T("MinBarSize"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nMaxBarSize);
	cXMLConfigurator.AddElem(_T("MaxBarSize"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nSampleLevel);
	cXMLConfigurator.AddElem(_T("SampleLevel"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nDebugFlag);
	cXMLConfigurator.AddElem(_T("DebugFlag"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nAdvanceParam);
	cXMLConfigurator.AddElem(_T("AdvanceParam"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindDMCodePara.m_nAdvanceParam2);
	cXMLConfigurator.AddElem(_T("AdvanceParam2"), strInfo);

	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_bEnableReDecode);
	cXMLConfigurator.AddElem(_T("EnableReDecode"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_dGridThre);
	cXMLConfigurator.AddElem(_T("GridThre"), strInfo);
	strInfo.Format(_T("%f"), m_SearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_dGridSize);
	cXMLConfigurator.AddElem(_T("GridSize"), strInfo);
	strInfo.Format(_T("%d"), m_SearchDataParam.m_DmCodeSearchParam.m_FindCornerPara.m_bOutputOutPoints);
	cXMLConfigurator.AddElem(_T("EnableShowOutputPoints"), strInfo);
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
	return true;
}	

// 更新训练控件数据信息	
bool CDmCodeCalibrateTool::UpdateTrainGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{
		m_tmpTrainGuiParam.m_DmCodeSearchRect	= m_GuiDmCodeSearchRect.GetRect();
		m_tmpTrainGuiParam.m_CornersSearchRect	= m_GuiCornersSearchRect.GetRect();
	}
	else // 将数据传递到Gui控件上
	{	

		m_GuiDmCodeSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiDmCodeSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiDmCodeSearchRect.SetRect(m_tmpTrainGuiParam.m_DmCodeSearchRect);

		m_GuiCornersSearchRect.SetLineColor(m_tmpTrainGuiParam.m_LineColor);
		m_GuiCornersSearchRect.SetLineWidth(m_tmpTrainGuiParam.m_nLineWidth);
		m_GuiCornersSearchRect.SetRect(m_tmpTrainGuiParam.m_CornersSearchRect);
	}

	return true;
}

bool CDmCodeCalibrateTool::UpdateSearchGuiData(bool bSaveAndValidate)
{
	if (bSaveAndValidate)	// 从Gui控件获取数据
	{

	}
	else // 将数据传递到Gui控件上
	{
		int j=0;

		for (j=0; j<m_pGuiSearchPolylineArray.size(); j++)
		{
			m_pGuiSearchPolylineArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchPolylineArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

		for (j=0; j<m_pGuiSearchCoordCrossArray.size(); j++)
		{
			m_pGuiSearchCoordCrossArray[j]->SetLineColor(m_tmpSearchGuiParam.m_LineColor);
			m_pGuiSearchCoordCrossArray[j]->SetLineWidth(m_tmpSearchGuiParam.m_nLineWidth);
		}

	}

	return true;
}

// 清除控件及其显示					
bool CDmCodeCalibrateTool::ClearGraphicsArray(std::vector<scGuiGraphic*> &pGraphicsArray)
{
	// 	for (int j=0; j<pGraphicsArray.size(); j++)
	// 	{
	// 		delete pGraphicsArray[j];
	// 	}

	pGraphicsArray.clear();

	return true;
}																		

// 清除定位工具在Display上的控件
bool CDmCodeCalibrateTool::ClearAllGui()
{
	ClearTrainGui();

	ClearResultGui();

	return true;
}															    

// 清除搜索结果，需先清除结果GUI
bool CDmCodeCalibrateTool::ClearResult()
{
	ClearResultGui();

	m_SearchResultArray.clear();
	ClearGraphicsArray(m_pGuiSearchPolylineArray);
	ClearGraphicsArray(m_pGuiSearchCoordCrossArray);

	return true;
}

bool CDmCodeCalibrateTool::SetGuiByCam(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_CamWidth) < 10 || abs(nHeight - m_CamHeight) < 10)return true;

	//训练区域
	double centerx,centery, xlen,ylen;
	m_GuiDmCodeSearchRect.GetCenterWidthHeight(centerx,centery, xlen,ylen);
	m_GuiDmCodeSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	//搜索区域

	m_GuiCornersSearchRect.GetCenterWidthHeight(centerx,centery,xlen,ylen);
	m_GuiCornersSearchRect.SetCenterWidthHeight((centerx/m_CamWidth)*nWidth, (centery/m_CamHeight)*nHeight, (xlen/m_CamWidth)*nWidth, (ylen/m_CamHeight)*nHeight);

	m_CamHeight = nHeight;
	m_CamWidth = nWidth;
	return true;
}

bool CDmCodeCalibrateTool::ClearTrainGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		m_pGuiInteractiveContainer = m_pGuiDisplay->GetInteractiveContainer();

		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_DmCodeSearchRect"));
		m_pGuiInteractiveContainer->RemoveItem(m_strKeyText+_T("_CornersSearchRect"));
		
	}

	return true;
}

bool CDmCodeCalibrateTool::ClearResultGui()
{
	if (m_pGuiDisplay && ::IsWindow(m_hDispWnd))
	{
		int j=0;
		m_pGuiStaticContainer = m_pGuiDisplay->GetStaticContainer();

		for (j=0; j<m_pGuiSearchPolylineArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchPolylineArray[j]->GetTipText());
		}

		for (j=0; j<m_pGuiSearchCoordCrossArray.size(); j++)
		{
			m_pGuiStaticContainer->RemoveItem(m_pGuiSearchCoordCrossArray[j]->GetTipText());
		}

	}

	return true;
}

// 获取训练模板原始图像
bool CDmCodeCalibrateTool::GetPatternImage(cpImage &ImagePattern)
{
	ImagePattern = m_tmpImageTrain;
	return true;
}	

// 恢复训练参数
bool CDmCodeCalibrateTool::RestoreTrainParam()
{ 
	m_tmpTrainDataParam = m_TrainDataParam;
	m_tmpTrainGuiParam = m_TrainGuiParam;
	UpdateTrainGuiData(false);
	return true;
}

// 恢复搜索参数		
bool CDmCodeCalibrateTool::RestoreSearchParam()
{
	m_tmpSearchDataParam = m_SearchDataParam;
	m_tmpSearchGuiParam = m_SearchGuiParam;
	UpdateSearchGuiData(false);
	return true;
}

void CDmCodeCalibrateTool::SetLanguage(int nLanguage)
{
	if (nLanguage<0 || nLanguage>1)
	{
		return;
	}
	m_iLanguage = nLanguage;
	switch (m_iLanguage)
	{
	case 0:
		m_GuiDmCodeSearchRect.SetLabel(_T("DmCode Search Area"));
		m_GuiCornersSearchRect.SetLabel(_T("Corners Search Area"));
		break;
	case 1:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	default:
		m_GuiDmCodeSearchRect.SetLabel(_T("二维码搜索区域"));
		m_GuiCornersSearchRect.SetLabel(_T("角点搜索区域"));
		break;
	}
}
CString CDmCodeCalibrateTool::GetResultErrInfo()
{
	return m_strError;
}
