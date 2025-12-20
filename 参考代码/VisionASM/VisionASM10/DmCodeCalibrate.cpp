#include "stdafx.h"
#include "DmCodeCalibrate.h"


CDmCodeCalibrate::CDmCodeCalibrate(void)
{
    m_bImageValid = FALSE;
    m_bIsSearchValid = FALSE;
    m_strError = _T("");
    m_vImgDecodeCorners.clear();
    m_vImgDecodeCornersLabel.clear();
    //m_pGuiDisplay = NULL;
	m_pGuiDmCodeGraphics = NULL;
    //m_pStaticContainer						= NULL;
    //m_GuiPolyline = NULL;
    //m_GuiQRCodeText = NULL;
    //m_vGuiCodeCross.clear();
    //m_vpResultGuiCodeCross.clear();
    m_vImgDecodecornersResult.clear();
	m_roiRectCalibrate.Set(0,0,752,480);
	m_roiRectDecode.Set(25,25,702,430);
}


CDmCodeCalibrate::~CDmCodeCalibrate(void)
{
    //ClearSearchGui();
    ClearSearchResult();
    //ClearRelationGui();
    ClearRelationResult();

}
BOOL CDmCodeCalibrate::SetGuiGraphics(CGuiGraphics* pGuiGraphics)
{
	if (pGuiGraphics == NULL)
	{
		m_strError = _T("设置GUI失败");
		return FALSE;
	}
	m_pGuiDmCodeGraphics = pGuiGraphics;
	return TRUE;
}
//BOOL CDmCodeCalibrate::SetGuiDisplay(scGuiDisplay* pGuiDisplay)
//{
//    ClearSearchGui();
//    if (pGuiDisplay == NULL)
//    {
//        m_strError = _T("设置显示控件失败");
//        return FALSE;
//    }
//
//    m_pGuiDisplay = pGuiDisplay;
//    return TRUE;
//}

bool CDmCodeCalibrate::ClearSearchResult()
{
   m_vImgDecodeCorners.clear();
   m_vImgDecodeCornersLabel.clear();
   m_vArrayDMCodesResult.clear();
   return true;
}

//bool CDmCodeCalibrate::ClearSearchGui()
//{
//    if (m_pGuiDisplay == NULL)
//    {
//        return FALSE;
//    }
//    m_pStaticContainer = m_pGuiDisplay->GetStaticContainer();
//    CString str;
//    str.Format(_T("_DmCodePolyline"));
//    m_pStaticContainer->RemoveItem(str);
//    str.Format(_T("_DmCodeText"));
//    m_pStaticContainer->RemoveItem(str);
//    for (int i=0;i<m_vGuiCodeCross.size();i++)
//    {
//        str.Format(_T("_DmCodeCorners%d"),i);
//        m_pStaticContainer->RemoveItem(str);
//    }
//   // m_pGuiDisplay->MyInvalidate();
//    for (int i=0;i<m_vGuiCodeCross.size();i++)
//    {
//        if(m_vGuiCodeCross.at(i)!=NULL)
//        {
//            delete m_vGuiCodeCross.at(i);
//            m_vGuiCodeCross.at(i) = NULL;
//        }
//    }
//    m_vGuiCodeCross.clear();
//    if (m_GuiPolyline!=NULL)
//    {
//        delete m_GuiPolyline;
//        m_GuiPolyline = NULL;
//    }
//    if (m_GuiQRCodeText!=NULL)
//    {
//        delete m_GuiQRCodeText;
//        m_GuiQRCodeText = NULL;
//    }
//    return true;
//}
void CDmCodeCalibrate::SetSearchValid(BOOL bValid)
{
	m_bIsSearchValid = bValid;
}
BOOL CDmCodeCalibrate::SetSearchImage(cpImage &image)
{
    m_bImageValid = FALSE;
    if (!image.IsValid())
    {
        m_strError = _T("设置图像为空");
        return FALSE;
    }
    else
    {
        m_imageInput = image; 
        m_bImageValid = TRUE;
    }
    return TRUE;
}

//BOOL CDmCodeCalibrate::ShowGuiSearchResult(BOOL bShow)
//{
//    if (m_pGuiDisplay == NULL)
//    {
//        return FALSE;
//    }
//    if (bShow)
//    {
//        m_GuiPolyline->SetVisible(bShow);	
//        m_GuiPolyline->SetLineColor(RGB(255,0,255));
//        m_GuiPolyline->SetLabelVisible(bShow);
//
//        m_GuiQRCodeText->SetLineColor(RGB(255,0,0));
//        m_GuiQRCodeText->SetVisible(bShow);
//
//        for (int i = 0;i<m_vGuiCodeCross.size();i++)
//        {
//            m_vGuiCodeCross.at(i)->SetLineColor(RGB(0,255,0));
//            m_vGuiCodeCross.at(i)->SetLineWidth(2);
//            m_vGuiCodeCross.at(i)->SetVisible(bShow);
//        }
//
//        m_pStaticContainer = m_pGuiDisplay->GetStaticContainer();
//        CString str;
//        str.Format(_T("_DmCodePolyline"));
//        m_pStaticContainer->AddItem(m_GuiPolyline,str);
//        str.Format(_T("_DmCodeText"));
//        m_pStaticContainer->AddItem(m_GuiQRCodeText,str);
//        for (int i=0;i<m_vGuiCodeCross.size();i++)
//        {
//            str.Format(_T("_DmCodeCorners%d"),i);
//            m_pStaticContainer->AddItem(m_vGuiCodeCross.at(i),str);
//        }
//        m_pGuiDisplay->SetStaticContainer(m_pStaticContainer);
//        m_pGuiDisplay->MyInvalidate();
//    }
//    else
//    {
//        ClearSearchGui();
//        ClearSearchResult();
//        ClearRelationGui();
//        ClearRelationResult();
//    }
//    return TRUE;
//}

BOOL CDmCodeCalibrate::SetDmCodeParam(scDMCodeParam& dmCodeParam)
{
    // 设置DmCodeParam结构体参数
    m_scDmCodeParam = dmCodeParam;
    return TRUE;   
}

BOOL CDmCodeCalibrate::SearchDmCode()
{
    m_bIsSearchValid = FALSE;
    //if (m_pGuiDisplay == NULL)
    //{
    //    m_strError = _T("未设置显示控件");
    //    return FALSE;
    //}
    if (!UpdateSearchParam())
    {
        return FALSE;
    }
    //ShowGuiSearchResult(FALSE);
	ClearSearchResult();
    // 开始搜索
    int nErrorCode;
    bool bResult = false;
	
	if (m_imageInput.PixelSize() != 1)// 若是彩色图像，则转换成灰度图像，因为二维码靶标基本都是黑白的，无需使用彩色
	{
		cpImage tmpImage;
		m_imageInput.GetGrayImage(tmpImage);
		bResult = m_scDmCodeCalibrate.Execute(tmpImage,
			m_roiRectCalibrate,
			m_roiRectDecode,
			m_vImgDecodeCorners,
			m_vImgDecodeCornersLabel,
			m_vArrayDMCodesResult,nErrorCode);
	}
	else
	{
		bResult = m_scDmCodeCalibrate.Execute(m_imageInput,
			m_roiRectCalibrate,
			m_roiRectDecode,
			m_vImgDecodeCorners,
			m_vImgDecodeCornersLabel,
			m_vArrayDMCodesResult,nErrorCode);
	}
	
   /* bResult = m_scDmCodeCalibrate.Execute(m_imageInput,
        m_roiRectCalibrate,
        m_roiRectDecode,
        m_vImgDecodeCorners,
        m_vImgDecodeCornersLabel,
        m_vArrayDMCodesResult,nErrorCode);*/
    if (!bResult)
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
        
        return FALSE;
    }
    UpdateSearchGuiData();
    /*ShowGuiSearchResult(TRUE);*/
    m_bIsSearchValid = TRUE;
    return TRUE;
}

bool CDmCodeCalibrate::UpdateSearchGuiData()
{
	if (m_pGuiDmCodeGraphics != NULL)
	{
		for (int i=0;i<m_pGuiDmCodeGraphics->m_vGuiObjectPolyline.size();i++)
		{
			m_pGuiDmCodeGraphics->m_vbShowObjectPolyline.at(0) = FALSE;
		}
		for (int i=0;i<m_pGuiDmCodeGraphics->m_vGuiObjectCross.size();i++)
		{
			m_pGuiDmCodeGraphics->m_vbShowObjectCross.at(i) = FALSE;
		}
		Code2DResult tmpCodeResult = m_vArrayDMCodesResult.at(0);
		//m_GuiPolyline = new scGuiPolyline;
		//m_GuiPolyline->SetPolyline(tmpCodeResult.m_CodePolyline);
		//m_GuiPolyline->SetLabel(tmpCodeResult.m_CodeString);
		m_pGuiDmCodeGraphics->m_vGuiObjectPolyline.at(0)->SetPolyline(tmpCodeResult.m_CodePolyline);
		m_pGuiDmCodeGraphics->m_vbShowObjectPolyline.at(0) = TRUE;
		m_pGuiDmCodeGraphics->m_vGuiObjectPolyline.at(0)->SetLabel(tmpCodeResult.m_CodeString);
		m_pGuiDmCodeGraphics->m_vGuiObjectPolyline.at(0)->SetLabelVisible(TRUE);
		//CString str = _T("二维码：");
		//str = str + tmpCodeResult.m_CodeString;
	   /* m_GuiQRCodeText = new scGuiText;
		m_GuiQRCodeText->SetXYRotationText(sc2Vector(10,50),scRadian(0),str);*/

		for (int i=0;i<m_vImgDecodeCorners.size() && i<m_pGuiDmCodeGraphics->m_vGuiObjectCross.size();i++)
		{
			m_pGuiDmCodeGraphics->m_vGuiObjectCross.at(i)->SetCenterXYWidthHeight(
															m_vImgDecodeCorners.at(i).GetX(),
															m_vImgDecodeCorners.at(i).GetY(),
															40,40);
			m_pGuiDmCodeGraphics->m_vGuiObjectCross.at(i)->SetLineColor(RGB(0,255,0));
		    m_pGuiDmCodeGraphics->m_vGuiObjectCross.at(i)->SetLineWidth(2);
			m_pGuiDmCodeGraphics->m_vbShowObjectCross.at(i)= TRUE;
		}
		//m_vGuiCodeCross.resize(m_vImgDecodeCorners.size());
		//for (int i=0;i<m_vImgDecodeCorners.size();i++ )
		//{
		//	m_vGuiCodeCross.at(i)=new scGuiCross;
		//	m_vGuiCodeCross.at(i)->SetCenterXYWidthHeight(
		//		m_vImgDecodeCorners.at(i).GetX(),
		//		m_vImgDecodeCorners.at(i).GetY(),
		//		40,40);
		//}
	}

    return true;
}

BOOL CDmCodeCalibrate::UpdateSearchParam()
{
    if (!m_bImageValid)
    {
        m_strError = _T("搜索图像未设置");
        return FALSE;
    }
    //m_scDmCodeCalibrate.SetFindCornerPara(m_scDmCodeParam.m_dGridSize,m_scDmCodeParam.m_dGridThre,m_scDmCodeParam.m_bOutputOutPoints);  //棋盘格单元大小\棋盘格强度阈值\输出外围点
	m_scDmCodeCalibrate.SetFindCornerPara(m_scDmCodeParam.m_FindCornerPara);
    /*m_scDmCodeCalibrate.SetFindCodePara(m_scDmCodeParam.m_nDMCodeMaxNum,m_scDmCodeParam.m_nMaxDecodeTime,m_scDmCodeParam.m_bDecodeImageMirror,m_scDmCodeParam.m_bEnableReDecode);*/
	m_scDmCodeCalibrate.SetFindDMCodePara(m_scDmCodeParam.m_FindDMCodePara);

	m_scDmCodeCalibrate.EnableReDecode(m_scDmCodeParam.m_bEnableReDecode);

	m_roiRectCalibrate = m_scDmCodeParam.m_CornersSearchRect;
	m_roiRectDecode = m_scDmCodeParam.m_DmCodeSearchRect;

	SetRoiByImage(m_imageInput.Width(),m_imageInput.Height());
    return TRUE;
}

bool CDmCodeCalibrate::SetRoiByImage(double nWidth,double nHeight)
{
	if(nWidth<1||nHeight<1)return false;
	if(abs(nWidth - m_scDmCodeParam.m_dImageWidth) < 10 || abs(nHeight - m_scDmCodeParam.m_dImageHeight) < 10)return true;

	//训练区域
	sc2Vector ul,sz,curUL,curSZ; double centerx,centery,curCenterx,curCentery;
	ul = m_roiRectCalibrate.GetUL();
	sz = m_roiRectCalibrate.GetSize();
	centerx = ul.GetX()+sz.GetX()/2;
	centery = ul.GetY()+sz.GetY()/2;
	curCenterx = (centerx/m_scDmCodeParam.m_dImageWidth)*nWidth;
	curCentery = (centery/m_scDmCodeParam.m_dImageHeight)*nHeight;
	curSZ.SetX((sz.GetX()/m_scDmCodeParam.m_dImageWidth)*nWidth);
	curSZ.SetY((sz.GetY()/m_scDmCodeParam.m_dImageHeight)*nHeight);
	curUL.SetX(curCenterx - curSZ.GetX()/2);
	curUL.SetY(curCentery - curSZ.GetY()/2);
	m_roiRectCalibrate.Set(curUL,curSZ);

	ul = m_roiRectDecode.GetUL();
	sz = m_roiRectDecode.GetSize();
	centerx = ul.GetX()+sz.GetX()/2;
	centery = ul.GetY()+sz.GetY()/2;
	curCenterx = (centerx/m_scDmCodeParam.m_dImageWidth)*nWidth;
	curCentery = (centery/m_scDmCodeParam.m_dImageHeight)*nHeight;
	curSZ.SetX((sz.GetX()/m_scDmCodeParam.m_dImageWidth)*nWidth);
	curSZ.SetY((sz.GetY()/m_scDmCodeParam.m_dImageHeight)*nHeight);
	curUL.SetX(curCenterx - curSZ.GetX()/2);
	curUL.SetY(curCentery - curSZ.GetY()/2);
	m_roiRectDecode.Set(curUL,curSZ);
	return true;
}

CString CDmCodeCalibrate::GetErrorString()
{
    return m_strError;
}
int CDmCodeCalibrate::GetCornersNum()
{
    return m_vImgDecodeCorners.size();
}
bool CDmCodeCalibrate::GetCornersImagePos(std::vector<sc2Vector>& vCornersImagePos)
{
	vCornersImagePos.clear();
	vCornersImagePos = m_vImgDecodeCorners;
	return true;
}
bool CDmCodeCalibrate::GetCornersLabelPos(std::vector<sc2Vector>& vCornersLabelPos)
{
	vCornersLabelPos.clear();
	vCornersLabelPos = m_vImgDecodeCornersLabel;
	return true;
}

BOOL CDmCodeCalibrate::GetDmCodeResult(Code2DResult& result)
{
    if(m_vArrayDMCodesResult.size()<=0)
        return FALSE;
    else
    {
        result = m_vArrayDMCodesResult.at(0);
        return TRUE;
    }
    
}
BOOL CDmCodeCalibrate::IsSearchValid()
{
    return m_bIsSearchValid;
}

BOOL CDmCodeCalibrate::RelatedTo(CDmCodeCalibrate& dmCodeTo)
{
    if (!dmCodeTo.IsSearchValid()||!IsSearchValid())
    {
        m_strError = _T("关联二维码无效");
        return FALSE;
    }
    //ShowGuiRelationResult(FALSE);
    //dmCodeTo.ShowGuiRelationResult(FALSE);
	ClearRelationResult();
	dmCodeTo.ClearRelationResult();
    BOOL bResult = FALSE;
    double h[9]={0};
    bResult = sfGetHomography(m_vImgDecodeCorners,
        m_vImgDecodeCornersLabel,
        dmCodeTo.m_vImgDecodeCorners,
        dmCodeTo.m_vImgDecodeCornersLabel,
        h,
        m_vImgDecodecornersResult,
        dmCodeTo.m_vImgDecodecornersResult);
    if (!bResult)
    {
        m_strError = _T("关联失败");
        return FALSE;
    }
 //   CString strLog;
 //   strLog.Format(_T("h0:%.5f ; h1:%.5f ; h2:%.5f ; h3:%.5f ; h4:%.5f ; h5:%.5f ; h6:%.5f ; h7:%.5f ; h8:%.5f;\n")
 //                   ,h[0],h[1],h[2],h[3],h[4],h[5],h[6],h[7],h[8]);

 //   VisionAlignLogRecord(_T("D:\\DmCodeTest.txt"),strLog,FALSE);

	//CString strAllCorners;
	//for(int i = 0;i<m_vImgDecodecornersResult.size();i++)
	//{
	//	strLog.Format(_T("%f ;"),m_vImgDecodecornersResult.at(i).GetX());
	//	strAllCorners +=strLog;
	//	strLog.Format(_T("%f \n"),m_vImgDecodecornersResult.at(i).GetY());
	//	strAllCorners +=strLog;
	//}
	//VisionAlignLogRecord(_T("D:\\DmCodeTest.txt"),strAllCorners,FALSE);
    UpdateRelationGuiData();
    dmCodeTo.UpdateRelationGuiData();
    //ClearSearchGui();
    //dmCodeTo.ClearSearchGui();
    //ShowGuiRelationResult(TRUE);
    //dmCodeTo.ShowGuiRelationResult(TRUE);
    return TRUE;
}
bool CDmCodeCalibrate::ClearRelationResult()
{
    m_vImgDecodecornersResult.clear();
    return true;
}

//bool CDmCodeCalibrate::ClearRelationGui()
//{
//    if (m_pGuiDisplay == NULL)
//    {
//        return FALSE;
//    }
//    m_pStaticContainer = m_pGuiDisplay->GetStaticContainer();
//    CString str;
//    for (int i=0;i<m_vpResultGuiCodeCross.size();i++)
//    {
//        str.Format(_T("_DmCodeResultCorners%d"),i);
//        m_pStaticContainer->RemoveItem(str);
//    }
//   // m_pGuiDisplay->MyInvalidate();
//    for (int i=0;i<m_vpResultGuiCodeCross.size();i++)
//    {
//        if(m_vpResultGuiCodeCross.at(i)!=NULL)
//        {
//            delete m_vpResultGuiCodeCross.at(i);
//            m_vpResultGuiCodeCross.at(i) = NULL;
//        }
//    }
//    m_vpResultGuiCodeCross.clear();
//    return true;
//}
bool CDmCodeCalibrate::UpdateRelationGuiData()
{
    //m_vpResultGuiCodeCross.resize(m_vImgDecodecornersResult.size());
    //for (int i=0;i<m_vpResultGuiCodeCross.size();i++ )
    //{
    //    m_vpResultGuiCodeCross.at(i)=new scGuiCoordCross;
    //    m_vpResultGuiCodeCross.at(i)->SetCenterRotationLengths(
    //        m_vImgDecodecornersResult.at(i).GetX(),
    //        m_vImgDecodecornersResult.at(i).GetY(),
    //        45,50,50);
    //}
	if (m_pGuiDmCodeGraphics != NULL)
	{
		for (int i=0;i<m_pGuiDmCodeGraphics->m_vGuiObjectCoordCross.size();i++)
		{
			m_pGuiDmCodeGraphics->m_vbShowObjectCoordCross.at(i) = FALSE;
		}

		for (int i=0;i<m_vImgDecodecornersResult.size() && i<m_pGuiDmCodeGraphics->m_vGuiObjectCoordCross.size();i++)
		{
			m_pGuiDmCodeGraphics->m_vGuiObjectCoordCross.at(i)->SetCenterRotationLengths(
				        m_vImgDecodecornersResult.at(i).GetX(),
				        m_vImgDecodecornersResult.at(i).GetY(),
				        45,50,50);
			m_pGuiDmCodeGraphics->m_vGuiObjectCoordCross.at(i)->SetLineColor(RGB(128,0,255));
			m_pGuiDmCodeGraphics->m_vGuiObjectCoordCross.at(i)->SetLineWidth(1);
			m_pGuiDmCodeGraphics->m_vbShowObjectCoordCross.at(i) = TRUE;
		}
	}

    return true;
}
//BOOL CDmCodeCalibrate::ShowGuiRelationResult(BOOL bShow)
//{
//    if (m_pGuiDisplay == NULL)
//    {
//        return FALSE;
//    }
//    if (bShow)
//    {
//
//        for (int i = 0;i<m_vpResultGuiCodeCross.size();i++)
//        {
//            m_vpResultGuiCodeCross.at(i)->SetLineColor(RGB(128,0,255));
//            m_vpResultGuiCodeCross.at(i)->SetLineWidth(1);
//            m_vpResultGuiCodeCross.at(i)->SetVisible(bShow);
//        }
//
//        m_pStaticContainer = m_pGuiDisplay->GetStaticContainer();
//        CString str;
//        for (int i=0;i<m_vpResultGuiCodeCross.size();i++)
//        {
//            str.Format(_T("_DmCodeResultCorners%d"),i);
//            m_pStaticContainer->AddItem(m_vpResultGuiCodeCross.at(i),str);
//        }
//        m_pGuiDisplay->SetStaticContainer(m_pStaticContainer);
//        m_pGuiDisplay->MyInvalidate();
//    }
//    else
//    {
//        ClearRelationGui();
//        ClearRelationResult();
//    }
//    return TRUE;
//}

void CDmCodeCalibrate::VisionAlignLogRecord(CString strPathFileName, CString strLog, BOOL bLogTimeEnable/* = TRUE*/)
{
        SYSTEMTIME sys; 
        GetLocalTime( &sys ); 
        CString t;
        t.Format("[%02d_%02d_%02d_%02d_%02d_%03d]",sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);

        CString strTemp = "";
        if (bLogTimeEnable)
        {
            strTemp += t;
        }

        strTemp += strLog;
        strTemp += "\n";
        FILE *stream;

        CString strPath;
        strPath = strPathFileName;
        stream = fopen(strPath, _T("a+t"));
        if (stream != NULL)
        {
            fwrite(strTemp, sizeof(TCHAR), strTemp.GetLength(), stream);
            fclose(stream);	
        }
}
