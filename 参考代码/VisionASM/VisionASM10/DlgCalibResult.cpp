// DlgCalibResult.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibResult.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "DlgSysKindCalibSetting.h"

bool tfGet3PointCenter(sc2Vector p1, sc2Vector p2, sc2Vector p3,sc2Vector &Center)  //求三点圆心
{
	double a, b, c, d, e, f;
	sc2Vector p(0,0);
	a = 2*(p2.GetX()-p1.GetX());
	b = 2*(p2.GetY()-p1.GetY());
	c = p2.GetX()*p2.GetX()+p2.GetY()*p2.GetY()-p1.GetX()*p1.GetX()-p1.GetY()*p1.GetY();
	d = 2*(p3.GetX()-p2.GetX());
	e = 2*(p3.GetY()-p2.GetY());
	f = p3.GetX()*p3.GetX()+p3.GetY()*p3.GetY()-p2.GetX()*p2.GetX()-p2.GetY()*p2.GetY();
	double g=b*d-e*a;
	if(g==0)
	{
		return false;
	}
	p.SetX((b*f-e*c)/g);
	p.SetY((d*c-a*f)/g);
	Center=p;
	//r = sqrt((p.GetX()-p1.GetX())*(p.GetX()-p1.GetX())+(p.GetY()-p1.GetY())*(p.GetY()-p1.GetY()));//半径
	return true;
}


// CDlgCalibResult 对话框

IMPLEMENT_DYNAMIC(CDlgCalibResult, CDialogEx)
CDlgCalibResult::CDlgCalibResult(CWnd* pParent)
	: CDialogEx(CDlgCalibResult::IDD, pParent)
{
}

CDlgCalibResult::CDlgCalibResult(vcBaseVisionAlign *pVisionASM,int nPosIndex,CWnd* pParent)
	: CDialogEx(CDlgCalibResult::IDD, pParent)
	,m_nExIndex(0)
{
	if(pVisionASM != NULL)
	{
		m_pVisionASM=pVisionASM;
	}
	m_nPosIndex=nPosIndex;
	m_pCalibratedInfo = NULL;
	m_pCalibSearchTool = NULL;	
}

CDlgCalibResult::~CDlgCalibResult()
{
	m_pVisionASM = NULL;
}

void CDlgCalibResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CALIB_RESULT_EXTENT, m_ComboCalibExtentIndex);
	DDX_Control(pDX, IDC_COMBO_CALIB_RESULT_POS, m_ComboCalibPosIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibResult, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCalibResult::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_RESULT_EXTENT, &CDlgCalibResult::OnCbnSelchangeComboCalibResultExtent)
	ON_BN_CLICKED(IDC_BTN_CALIB_RESULT_DETAIL, &CDlgCalibResult::OnBnClickedBtnCalibResultDetail)
	ON_BN_CLICKED(IDC_BTN_CALIB_RESULT_DIAGNOSE, &CDlgCalibResult::OnBnClickedBtnCalibResultDiagnose)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_CALIB_RESULT_POS, &CDlgCalibResult::OnCbnSelchangeComboCalibResultPos)
END_MESSAGE_MAP()


// CDlgCalibResult 消息处理程序


void CDlgCalibResult::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL CDlgCalibResult::OnInitDialog()
{
	CString strTemp;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_GuiDisplay.Create(IDC_STATIC_CALIB_DISPLAY,this);
	m_pVisionASM->GetSysPlatformInfo(m_sysPlatformInfo);
	
	m_nExIndex = m_nPosIndex / m_sysPlatformInfo.m_nPositionNum;
	m_nPosIndex = m_nPosIndex % m_sysPlatformInfo.m_nPositionNum;
	strTemp.Format(_T("CalibratedResult%d"),m_nPosIndex+1);
	
	//AfxSetWindowText(GetSafeHwnd(),strTemp);
	
	m_ComboCalibExtentIndex.InsertString(0,_T("基本标定(扩展标定1)"));
	
	if(m_sysPlatformInfo.m_bEnableMultiCalibExtension)
	{
		m_ComboCalibExtentIndex.ShowWindow(TRUE);
		m_ComboCalibExtentIndex.EnableWindow(TRUE);
		for(int n = 1; n <=m_sysPlatformInfo.m_nMultiCalibExtensionMaxNum; n++)
		{
			strTemp.Format(_T("扩展标定%d"), n + 1);
			m_ComboCalibExtentIndex.InsertString(n,strTemp);
		}
	}
	else
	{
		m_ComboCalibExtentIndex.ShowWindow(TRUE);
		m_ComboCalibExtentIndex.EnableWindow(FALSE);
	}
	m_ComboCalibExtentIndex.SetCurSel(m_nExIndex);

	for(int i = 0;i< m_sysPlatformInfo.m_nPositionNum; i++)
	{
		if(i >= m_sysPlatformInfo.m_nPositionNum / 2 && m_sysPlatformInfo.m_bTargetObjectCamSeparate)
		{
			break;
		}
		strTemp.Format(_T("位置%d"), i + 1);
		m_ComboCalibPosIndex.InsertString(i,strTemp);
	}
	m_ComboCalibPosIndex.SetCurSel(m_nPosIndex);

	ShowCalibResult();
	//OnBnClickedBtnCalibResultDetail();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCalibResult::OnCbnSelchangeComboCalibResultExtent()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nExIndex = m_ComboCalibExtentIndex.GetCurSel();
	ShowCalibResult();
}

void CDlgCalibResult::OnCbnSelchangeComboCalibResultPos()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nPosIndex = m_ComboCalibPosIndex.GetCurSel();
	ShowCalibResult();
}

void CDlgCalibResult::OnBnClickedBtnCalibResultDetail()
{
	// TODO: 在此添加控件通知处理程序代码
	// 如果当前位置超限，则直接返回不做处理
	CString	strDescription,strCaption;
	//获取对标定信息的文本描述
	if(!CDlgSysKindCalibSetting::GetCalibratedResultDetailDescription(strDescription,m_pVisionASM,m_nPosIndex))
	{
		return;
	}

	strCaption.Format(_T("CalibratedResult%d"),m_nPosIndex + 1);
	
	MessageBox(strDescription, strCaption);
}

bool CDlgCalibResult::CalibResultDiagnose(CString &strOut,int &nKeyProblem)
{
	if(m_pCalibratedInfo==NULL)
	{
		return false;
	}
	int nKeyItem=0,nItem=0;
	sc2Vector pSearchMark[CALIBMOVEPOSNUM];//标定九宫格坐标
	sc2Vector pSearchMarkBench[CALIBMOVEPOSNUM];//标定九宫格坐标(相对于起始点)
	sc2Vector pSearchMarkRelative[CALIBMOVEPOSNUM];//标定九宫格坐标(相对于前一点)
	for(int i=0;i<CALIBMOVEPOSNUM;i++)
	{
		pSearchMark[i]=m_pCalibratedInfo->m_cpObjectMarkImgCoordPos[i].GetPos();
		pSearchMarkBench[i]=pSearchMark[i]-pSearchMark[0];
		if(i!=0)
		{
			pSearchMarkRelative[i]=pSearchMark[i]-pSearchMark[i-1];
		}
	}
	CString strKeyItem,strItem,str;
	strKeyItem+=_T("异常信息：\n\n");
	strItem+=_T("\n详细信息：\n\n");
	{
		scRadian radian;
		if((pSearchMark[1]-pSearchMark[0]).Angle(radian)!=HSuccess)
		{
			nKeyItem++;
			str.Format(_T("%d 没有标定信息；\n"),nKeyItem);
			strKeyItem+=str;
		}
		else
		{
			double degree=scDegree(radian).ToDouble();
			if(degree<=0||degree>=90)
			{
				nKeyItem++;
				str.Format(_T("%d 九宫格移动方向错误，可能是由于图像镜像、旋转角度、平台轴正方向设置有误，请检查上述设置；\n"),nKeyItem);
				strKeyItem+=str;
			}
			nItem++;
			str.Format(_T("%d 九宫格初始移动方向角度为%.1f°；\n"),nItem,degree);
			strItem+=str;
		}
	}
	{//检查移动量是否过小
		m_CalibImage.Release();
		if(m_pCalibSearchTool!=NULL&&m_pCalibSearchTool->GetSearchToolType()!=eSearchToolUnknown)
		{
			m_pCalibSearchTool->GetTrainImage(m_CalibImage);
		}
		if(m_CalibImage.IsValid() && m_CalibImage.Width()!=0 &&  m_CalibImage.Height()!=0)
		{
			double dMinX(10000),dMinY(10000),dMaxX(0),dMaxY(0);
			for(int i =0;i<9;i++)
			{
				dMaxX=max(pSearchMark[i].GetX(),dMaxX);
				dMaxY=max(pSearchMark[i].GetY(),dMaxY);
				dMinX=min(pSearchMark[i].GetX(),dMinX);
				dMinY=min(pSearchMark[i].GetY(),dMinY);
			}
			double dxk=(dMaxX-dMinX)/(double)m_CalibImage.Width();
			double dyk=(dMaxY-dMinY)/(double)m_CalibImage.Height();
			if(dxk<0.25||dyk<0.25)
			{
				nKeyItem++;
				str.Format(_T("%d 标定横向或纵向移动范围小于视野 25%% ,可能造成标定不准确，请考虑扩大标定范围；\n"),nKeyItem);
				strKeyItem+=str;
			}

			nItem++;
			str.Format(_T("%d 标定横向移动范围%d%% ,纵向移动范围%d%%；\n"),nItem,(int)(dxk*100),(int)(dyk*100));
			strItem+=str;
		}
	}
	if(!(m_sysPlatformInfo.m_bPlatformPick && m_sysPlatformInfo.m_ePickCamPlatformType ==ePickCamPlatformShareXY))
	{//检查首尾点重合度

		sc2Vector pStart = pSearchMark[0];
		sc2Vector pEnd = pSearchMark[11];
		if(pSearchMark[0].Distance(pSearchMark[11])>4)
		{
			nKeyItem++;
			str.Format(_T("%d 标定前后位置相差超过4像素，可能是由于移动精度不足或者设备存在抖动或者模版不佳；\n"),nKeyItem);
			strKeyItem+=str;
		}
		nItem++;
		str.Format(_T("%d 标定终点相对原点距离为%.1f像素 ,水平距离%.1f像素，垂直距离%.1f像素；\n"),nItem,pSearchMark[0].Distance(pSearchMark[11]),
			(pSearchMark[11]-pSearchMark[0]).GetX(),(pSearchMark[11]-pSearchMark[0]).GetY());
		strItem+=str;
	}

	{//九宫格奇异点检查
		
		/*
		             5-----4-----3
					 丨          丨
					 6     0     2
					 丨       \  丨
					 7-----8     1
 		*/                    
		double dXOffsetMax=0;//横向移动的最大值
		double dYOffsetMax=0;//纵向移动的最小值
		double dXOffsetMin=0;//横向移动的最大值
		double dYOffsetMin=0;//纵向移动的最小值
		dXOffsetMax =max(max(abs(pSearchMark[1].GetX()-pSearchMark[8].GetX()),abs(pSearchMarkRelative[4].GetX())),
			max(abs(pSearchMarkRelative[5].GetX()),abs(pSearchMarkRelative[8].GetX()))
			);
		dXOffsetMin =min(min(abs(pSearchMark[1].GetX()-pSearchMark[8].GetX()),abs(pSearchMarkRelative[4].GetX())),
			min(abs(pSearchMarkRelative[5].GetX()),abs(pSearchMarkRelative[8].GetX()))
			);
		dYOffsetMax =max(max(abs(pSearchMarkRelative[2].GetY()),abs(pSearchMarkRelative[3].GetY())),
			max(abs(pSearchMarkRelative[6].GetY()),abs(pSearchMarkRelative[7].GetY()))
			);
		dYOffsetMin =min(min(abs(pSearchMarkRelative[2].GetY()),abs(pSearchMarkRelative[3].GetY())),
			min(abs(pSearchMarkRelative[6].GetY()),abs(pSearchMarkRelative[7].GetY()))
			);
         sc2Vector p(0,0);
		 double dTh=3;
		if(/*abs(pSearchMarkBench[2].Distance(p)-pSearchMarkBench[6].Distance(p))>dTh 
			||abs(pSearchMarkBench[4].Distance(p)-pSearchMarkBench[8].Distance(p))>dTh 
			||abs(pSearchMarkBench[3].Distance(p)-pSearchMarkBench[1].Distance(p))>dTh 
			||abs(pSearchMarkBench[5].Distance(p)-pSearchMarkBench[1].Distance(p))>dTh 
			||abs(pSearchMarkBench[7].Distance(p)-pSearchMarkBench[1].Distance(p))>dTh 
			||*/dXOffsetMax-dXOffsetMin>dTh
			||dYOffsetMax-dYOffsetMin>dTh
			)
		{
			nKeyItem++;
			str.Format(_T("%d 九宫格步长误差超过3像素，可能是由于移动精度不足或者设备存在抖动或者模版不佳，请排查原因并重新标定；\n"),nKeyItem);
			strKeyItem+=str;
		}

		nItem++;
		str.Format(_T("%d 横向移动的最大误差%.1f像素 ,纵向移动的最大误差%.1f像素；\n"),nItem,dXOffsetMax-dXOffsetMin,dYOffsetMax-dYOffsetMin);
		strItem+=str;
	}
	{//检查旋转角度是否和参数一致
		

		std::vector<CCalibrateParam*> vpCalibrateParam;
		vpCalibrateParam = m_pVisionASM->GetAlignTool()->GetCalibParams();
		if(vpCalibrateParam.size()>m_nPosIndex)
		{
			double dAngleParam=0;
			dAngleParam=abs(vpCalibrateParam.at(m_nPosIndex)->GetPlatformCalibMovement().GetAngle());
			sc2Vector center3Point;//三点中心
			if(tfGet3PointCenter(pSearchMark[9],pSearchMark[10],pSearchMark[11],center3Point))
			{
				scLine line0(center3Point,pSearchMark[11]-center3Point);
				scLine line1(center3Point,pSearchMark[9]-center3Point);
				scLine line2(center3Point,pSearchMark[10]-center3Point);
				double dAngle1,dAngle2;
				dAngle1=scDegree(line0.GetAngle(line1)).SignedNorm().ToDouble();
				dAngle2=scDegree(line0.GetAngle(line2)).SignedNorm().ToDouble();
				if(dAngle1<-1||dAngle2>1)
				{
					nKeyItem++;
					str.Format(_T("%d 旋转角度过小或旋转方向有误，请检查标定设置的旋转角度是否过小或者平台设置的旋转轴正方向是否正确；\n"),nKeyItem);
					strKeyItem+=str;
				}
				else
				{
					scLine line0(m_GuiCrossCenter.GetCenter(),pSearchMark[11]-m_GuiCrossCenter.GetCenter());
					scLine line1(m_GuiCrossCenter.GetCenter(),pSearchMark[9]-m_GuiCrossCenter.GetCenter());
					scLine line2(m_GuiCrossCenter.GetCenter(),pSearchMark[10]-m_GuiCrossCenter.GetCenter());
					double dAngle1,dAngle2;
					dAngle1=scDegree(line0.GetAngle(line1)).SignedNorm().ToDouble();
					dAngle2=scDegree(line0.GetAngle(line2)).SignedNorm().ToDouble();
					if(abs(abs(dAngle1)-dAngleParam)>0.2||abs(abs(dAngle2)-dAngleParam)>0.2)
					{
						nKeyItem++;
						str.Format(_T("%d 旋转角度与标定参数差异超过0.2°，可能是平台旋转不准确或者模版有问题；\n"),nKeyItem);
						strKeyItem+=str;
					}
					nItem++;
					str.Format(_T("%d 正向旋转角度为%.2f ,反向旋转角度为%.2f ,设定旋转角度为%.2f；\n"),nItem,dAngle1,dAngle2,dAngleParam);
					strItem+=str;
				}
			}


		}


	}
	if(nKeyItem==0)
	{
		str.Format(_T("\n没有发现明显异常\n"),nKeyItem);
	}
	else
	{
		str.Format(_T("\n共发现 %d 项异常,建议排查原因并重新标定\n"),nKeyItem);
	}
	strOut=strKeyItem+str+strItem;
	nKeyProblem=nKeyItem;
	return true;
	
}

void CDlgCalibResult::OnBnClickedBtnCalibResultDiagnose()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_pVisionASM->m_pSystempOptionConfig->m_bIsShowCalibrateEvaluate)
	{
		CDlgCalibrateEvaluate dlgCalibrateEvaluate;
		dlgCalibrateEvaluate.m_nCurProductIdx = m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
		dlgCalibrateEvaluate.SetVisionAlignerRef(m_pVisionASM);
		dlgCalibrateEvaluate.DoModal();
	}
	else
	{
		CString str;
		int nKeyProblem = 0;
		CalibResultDiagnose(str,nKeyProblem);
		MessageBox(str, _T("标定评估"));
	}

}


void CDlgCalibResult::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
}

void CDlgCalibResult::ShowCalibResult()
{
	m_GuiDisplay.ClearScreen(0);
	m_GuiDisplay.MyInvalidate();
	int nPosIndex = m_nPosIndex + m_nExIndex * m_pVisionASM->GetPosNum();

	CString str;
	if(m_nExIndex == 0)
	{
		str.Format(_T("基本标定位置 %d 尚未标定！"),m_nPosIndex + 1);
	}
	else
	{
		str.Format(_T("扩展标定 %d 位置 %d 尚未标定！"),m_nExIndex + 1,m_nPosIndex + 1);
	}
	int nCurProductIndex=m_pVisionASM->GetTotalProuductData()->GetCurProductIndex();
	// 获取当前平台对应的全部标定数据
	CTotalCalibData *pTotalCalibData= NULL;
	pTotalCalibData = m_pVisionASM->GetTotalCalibData();
	if (pTotalCalibData == NULL)
	{
		return ;
	}
	// 获取当前产品对应的标定数据
	CCalibData* pCalibData = NULL;
	pCalibData = pTotalCalibData->GetItemPtr(nCurProductIndex);
	if (pCalibData == NULL)
	{
		return ;
	}

	m_pCalibratedInfo = NULL;
	m_pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nPosIndex);
	if (m_pCalibratedInfo == NULL)
	{
		return ;
	}
	if(!m_pCalibratedInfo->IsValid())
	{
		AfxMessageBox(str);
		return;
	}
	m_pCalibSearchTool=NULL;

	
	m_pCalibSearchTool = m_pVisionASM->GetRepoCalibObjectSearchTool(pTotalCalibData->GetCurCalibIndex(), m_nPosIndex, 0);
	
	//if (m_pCalibSearchTool == NULL)
	//{
	//	return ;
	//}
	m_CalibImage.Release();
	if(m_pCalibSearchTool != NULL && m_pCalibSearchTool->GetSearchToolType() != eSearchToolUnknown)
	{
		m_pCalibSearchTool->GetTrainImage(m_CalibImage);
	}
	if(!m_CalibImage.IsValid())
	{	
		if(m_pVisionASM->GetAlignTool() && m_pVisionASM->GetAlignTool()->GetImageSize(m_nPosIndex).GetPosX() > 1
			&& m_pVisionASM->GetAlignTool()->GetImageSize(m_nPosIndex).GetPosY() > 1)
		{
			m_CalibImage.CreateImageBuffer((long)m_pVisionASM->GetAlignTool()->GetImageSize(m_nPosIndex).GetPosX(),
				(long)m_pVisionASM->GetAlignTool()->GetImageSize(m_nPosIndex).GetPosY(),epGray8);
		}
		else
		{
			double dMinX(10000),dMinY(10000),dMaxX(0),dMaxY(0);
			for(int i =0;i<12;i++)
			{
				dMaxX=max((m_pCalibratedInfo->m_cpObjectMarkImgCoordPos+i)->GetPosX(),dMaxX);
				dMaxY=max((m_pCalibratedInfo->m_cpObjectMarkImgCoordPos+i)->GetPosY(),dMaxY);
				dMinX=min((m_pCalibratedInfo->m_cpObjectMarkImgCoordPos+i)->GetPosX(),dMinX);
				dMinY=min((m_pCalibratedInfo->m_cpObjectMarkImgCoordPos+i)->GetPosY(),dMinY);
			}
			m_CalibImage.Release();
			m_CalibImage.CreateImageBuffer((long)(dMaxX+dMinX),(long)(dMaxY+dMinY),epGray8);
		}

	}
	m_GuiDisplay.SetImage(m_CalibImage);
	scGuiStaticContainer *pStaticContainer = m_GuiDisplay.GetStaticContainer();
	pStaticContainer->Clear();
	str.Format(_T("位置 %d"),m_nPosIndex + 1);
	m_GuiPosText.SetText(str);
	m_GuiPosText.SetXYRotation(0,0,0);
	m_GuiPosText.SetLineColor(RGB(255,255,0));
	m_GuiPosText.SetVisible(TRUE);
	pStaticContainer->AddItem(&m_GuiPosText,_T("位置号"));

	CString strLable;
	CCoordPos *pCoordPos=m_pCalibratedInfo->m_cpObjectMarkImgCoordPos;
	m_GuiCrossCenter.SetCenter(-1.0*m_pCalibratedInfo->GetMarkImgCoordPos().GetPos());
	double h[9];
	memset(h,0,sizeof(double)*9);
	m_pCalibratedInfo->GetPlatformTransferH(h);
	sc2Vector center=sfHomographyInverseProject(-1.0*m_pCalibratedInfo->GetMarkPlatformCoordPos().GetPos(),
		m_pCalibratedInfo->m_PlatformTranferH);
	center=center+pCoordPos[0].GetPos();
	m_GuiCrossCenter.SetCenterRotationLengths(center,scRadian(0),30,30);
	strLable.Format(_T("旋转中心%.1f,%.1f"),center.GetX(),center.GetY());
	m_GuiCrossCenter.SetLabel(strLable);
	m_GuiCrossCenter.SetLabelVisible(1);
	m_GuiCrossCenter.SetVisible(1);
	pStaticContainer->AddItem(&m_GuiCrossCenter,_T("旋转中心"));
	for(int i =0;i<12;i++)
	{

		if(i!=0)
		{
			scLineSeg lineSeg(sc2Vector((pCoordPos+i-1)->GetPosX(),(pCoordPos+i-1)->GetPosY()),
				sc2Vector((pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY())
				);
			m_GuiLineSeg[i].SetLineSeg(lineSeg);
			m_GuiLineSeg[i].SetVisible(1);
			strLable.Format(_T("Step%d:[%.1f, %.1f]"),i,
				(pCoordPos+i)->GetPosX()-(pCoordPos+i-1)->GetPosX(),
				(pCoordPos+i)->GetPosY()-(pCoordPos+i-1)->GetPosY()
				);
			m_GuiLineSeg[i].SetLabel(strLable);
			//m_GuiLineSeg[i].SetLabelVisible(1);

			m_GuiLineSeg[i].SetLineColor(RGB(0,0,255));
			m_GuiLineSeg[i].SetLineWidth(2);
			if(m_sysPlatformInfo.m_bPlatformPick && m_sysPlatformInfo.m_ePickCamPlatformType ==ePickCamPlatformShareXY && i== 9)
			{

			}
			else
			{
				pStaticContainer->AddItem(m_GuiLineSeg+i,strLable);
			}

			if(i>=9)
			{
				lineSeg.Set(center,pCoordPos[i].GetPos());
				m_GuiLineSegRaduis[i-9].SetLineSeg(lineSeg);				
				m_GuiLineSegRaduis[i-9].SetLineColor(RGB(0,255,255));
				m_GuiLineSegRaduis[i-9].SetVisible(1);
				strLable.Format(_T("幅%d"),i-9);
				m_GuiLineSegRaduis[i-9].SetLabel(strLable);
				m_GuiLineSegRaduis[i-9].SetLineSegArrowVisible(0);
				//m_GuiLineSegRaduis[i-9].SetLineWidth(2);
				m_GuiLineSegRaduis[i-9].SetLineStyle(scGuiGraphic::dashLine);
				pStaticContainer->AddItem(m_GuiLineSegRaduis+i-9,strLable);
			}
		}

		m_GuiCross[i].SetCenterRotationLengths((pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY(),scRadian(scDegree(0)).ToDouble(),16,16);
		//m_GuiCross[i].SetCenterXYWidthHeight((pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY());
		m_GuiCross[i].SetLineColor(RGB(255,0,0));
		m_GuiCross[i].SetVisible(1);
		//if(i!=0)
		//{
		//	strLable.Format(_T("P%d:[%.3f, %.3f];\nV:[%.3f, %.3f]"),i,(pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY(),
		//		(pCoordPos+i)->GetPosX()-(pCoordPos+i-1)->GetPosX(),
		//		(pCoordPos+i)->GetPosY()-(pCoordPos+i-1)->GetPosY());
		//}
		//else
		//{
		//	strLable.Format(_T("P%d:[%.3f, %.3f];\nV:[%.3f, %.3f]"),i,(pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY(),
		//		0,
		//		0);
		//}
		strLable.Format(_T("P%d:[%.1f, %.1f]"),i,(pCoordPos+i)->GetPosX(),(pCoordPos+i)->GetPosY(),
			0,
			0);
		m_GuiCross[i].SetLabel(strLable);
		m_GuiCross[i].SetLabelVisible(1);
		pStaticContainer->AddItem(m_GuiCross+i,strLable);
	}

	m_GuiDisplay.SetStaticContainer(pStaticContainer);
	m_GuiDisplay.Invalidate();

	int nKeyProblem=0;
	CalibResultDiagnose(str,nKeyProblem);
	if(nKeyProblem>0)
	{
		//MessageBox(str, _T("标定评估"));
	}
}



