// DlgCalibrateEvaluate.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibrateEvaluate.h"
#include "afxdialogex.h"
#include <algorithm>

// CDlgCalibrateEvaluate 对话框

IMPLEMENT_DYNAMIC(CDlgCalibrateEvaluate, CDialogEx)

CDlgCalibrateEvaluate::CDlgCalibrateEvaluate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibrateEvaluate::IDD, pParent)
{
	m_pVisionASM = NULL;
	m_nCurProductIdx = 0;
}

CDlgCalibrateEvaluate::~CDlgCalibrateEvaluate()
{
	m_pVisionASM = NULL;
}

void CDlgCalibrateEvaluate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CALIB_EVALUATE, m_ListCalibrateEvaluate);
}


BEGIN_MESSAGE_MAP(CDlgCalibrateEvaluate, CDialogEx)
END_MESSAGE_MAP()


// CDlgCalibrateEvaluate 消息处理程序


void CDlgCalibrateEvaluate::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	m_pVisionASM = pVisionAligner;
}


BOOL CDlgCalibrateEvaluate::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_ImageList.Create(16,16, 1, 10, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));

	m_ListCalibrateEvaluate.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES ); 
	CRect rcListRect;
	GetDlgItem(IDC_LIST_CALIB_EVALUATE)->GetWindowRect(rcListRect);
	ScreenToClient(rcListRect);		  
	m_ListCalibrateEvaluate.SetImageList(&m_ImageList,LVSIL_SMALL); 

	if (NULL !=m_pVisionASM)
	{
		SysPlatformInfo platformInfo;
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		
		
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		int nShowNums = 1;
		int nRealShowNums = 1;
		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			if(optionInfo.m_bAutoCopyCalibData)
			{
				if (platformInfo.m_bEnableMultiCalibExtension)
				{
					nShowNums = platformInfo.m_nMultiCalibExtensionMaxNum+1;
				}
				else
				{
					nShowNums = 1;
				}
				nRealShowNums = 1;
			}
			else
			{

				if (platformInfo.m_bEnableMultiCalibExtension)
				{
					nShowNums = (platformInfo.m_nMultiCalibExtensionMaxNum+1)*platformInfo.m_nPositionNum/2;
				}
				else
				{
					nShowNums = platformInfo.m_nPositionNum/2;
				}
				nRealShowNums = platformInfo.m_nPositionNum/2;
				
			}

		}
		else
		{
			if(optionInfo.m_bAutoCopyCalibData)
			{
				if (platformInfo.m_bEnableMultiCalibExtension)
				{
					nShowNums = platformInfo.m_nMultiCalibExtensionMaxNum+1;
				}
				else
				{
					nShowNums = 1;
				}
				nRealShowNums = 1;
			}
			else
			{
				if (platformInfo.m_bEnableMultiCalibExtension)
				{
					nShowNums = (platformInfo.m_nMultiCalibExtensionMaxNum+1)*platformInfo.m_nPositionNum;
				}
				else
				{
					nShowNums = platformInfo.m_nPositionNum;
				}
				nRealShowNums = platformInfo.m_nPositionNum;
			}
		}

		// 初始化标定结果显示列表;
		int nRow =0;
		int k = 34;
		//m_ListCalibrateEvaluate.InsertColumn(0,_T("标定评价项"), LVCFMT_CENTER, rcListRect.Width()*3/k);
		//m_ListCalibrateEvaluate.InsertColumn(1, _T("具体值"), LVCFMT_CENTER,  rcListRect.Width()*13/k);
		//m_ListCalibrateEvaluate.InsertColumn(2,_T( "评价阈值"), LVCFMT_CENTER,  rcListRect.Width()*2/k);
		//m_ListCalibrateEvaluate.InsertColumn(3, _T("评价结果"), LVCFMT_CENTER, rcListRect.Width()*2/k);
		CString strTmp = _T("");
		m_ListCalibrateEvaluate.InsertColumn(0,_T("标定评价项"), LVCFMT_CENTER, rcListRect.Width()*10/k);


		int nImageIndex = 0;
		nRow ++;  //1
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("九宫格走位分析"),0,0,nImageIndex,0);

		nImageIndex =0;
		nRow ++;//2
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("角度顺逆时针分析"),0,0,nImageIndex,0);
		nRow ++;//3
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("X方向移动图像距离差"),0,0,nImageIndex,0);
		nRow ++;///4
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("Y方向移动图像距离差"),0,0,nImageIndex,0);
		nRow ++;//5
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("XY方向移动图像距离差"),0,0,nImageIndex,0);
		nRow ++;//6
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("D方向移动图像距离差"),0,0,nImageIndex,0);
		nRow ++;//7
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("起点终点偏差: "),0,0,nImageIndex,0);
		nRow ++;//8
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("XY走位步长比: "),0,0,nImageIndex,0);
		nRow ++;//9
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("标定像素范围XY: "),0,0,nImageIndex,0);

		nRow++; //10
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("转换矩阵h0,h4: "),0,0,nImageIndex,0);

		nRow++; //11
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("基准平台坐标："),0,0,nImageIndex,0);

		nRow++;//12
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("基准图像坐标："),0,0,nImageIndex,0);

		nRow++;//13
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("轴位置"),0,0,nImageIndex,0);

		nRow++;//14
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("旋转中心误差"),0,0,nImageIndex,0);

		nRow++;//15
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("像素分辨率"),0,0,nImageIndex,0);

		nRow++;//16
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("轴夹角"),0,0,nImageIndex,0);

		nRow++;//17
		m_ListCalibrateEvaluate.InsertItem(LVIF_TEXT|LVIF_IMAGE,nRow,_T("视野X*Y"),0,0,nImageIndex,0);

		int nColsWidth = 24/2;
		int nW =  rcListRect.Width()*nColsWidth/k;
		for (int it= 0;it<nShowNums;it++)
		{
			if (platformInfo.m_bEnableMultiCalibExtension)
			{
				if (platformInfo.m_bTargetObjectCamSeparate)
				{
					
					if (platformInfo.m_eMidPlatformType ==ePlatformXYPD)
					{
						strTmp.Format(_T("扩展%d位置%d具体值"),it/(platformInfo.m_nPositionNum/2)+1,it%(platformInfo.m_nPositionNum/2)+ platformInfo.m_nPositionNum/2+1);
					}
					else
					{
						strTmp.Format(_T("扩展%d位置%d具体值"),it/(platformInfo.m_nPositionNum/2)+1,it%(platformInfo.m_nPositionNum/2)+1);
					}
					
				}
				else
				{
					strTmp.Format(_T("扩展%d位置%d具体值"),it/(platformInfo.m_nPositionNum)+1,it%(platformInfo.m_nPositionNum)+1);
				}

			}
			else
			{
				int nShowPosIndex = it;
				if (platformInfo.m_bTargetObjectCamSeparate && platformInfo.m_eMidPlatformType ==ePlatformXYPD)
				{
					nShowPosIndex =it + platformInfo.m_nPositionNum/2;
				}

				{
					strTmp.Format(_T("位置%d具体值"),nShowPosIndex + 1);

				}

			}
			

			m_ListCalibrateEvaluate.InsertColumn(2*it+1, strTmp, LVCFMT_CENTER, nW*8/5);
			//strTmp.Format(_T("位置%d评价结果"),it+1);
			strTmp.Format(_T("评价结果"));
			m_ListCalibrateEvaluate.InsertColumn(2*it+2, strTmp, LVCFMT_CENTER,  nW*2/5);



			if (platformInfo.m_bEnableMultiCalibExtension)
			{
				if (platformInfo.m_bTargetObjectCamSeparate)
				{
					ShowCalibrateResult(m_pVisionASM,&m_ListCalibrateEvaluate,(int)(it/(platformInfo.m_nPositionNum/2))*platformInfo.m_nPositionNum +it%(platformInfo.m_nPositionNum/2) );
				}

				{
					ShowCalibrateResult(m_pVisionASM,&m_ListCalibrateEvaluate,it);
				}

			}
			else
			{
				ShowCalibrateResult(m_pVisionASM,&m_ListCalibrateEvaluate,it);
			}
			


		}
		
	}


	return TRUE;  // return TRUE unless you set the focus to a control

}

void CDlgCalibrateEvaluate::FixCalibrateResultImageAnaysisData(CCoordPos cpObjectMarkImgCoordPos[CALIBMOVEPOSNUM],CListCtrl * m_pList,int nPosIndex)
{

	if (m_pVisionASM==NULL) return;

	SysPlatformInfo platformInfo;
	m_pVisionASM->GetSysPlatformInfo(platformInfo);
	double MaxDistance = 0;
	vector<double> distance;	
	CString strStatus[2];
	strStatus[0]= _T("OK");
	strStatus[1] = _T("NG");
	int nLimit = 20;  // 最大20个像素
	int nImageIndex = 0; // OK/NG 图示
	CString strTemp = _T("");
	double z = 0;

	double dCalibMoveX = 1;
	double dCalibMoveY = 1;
	double dCalibMoveD = 1;
	vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();

	if (NULL != tmpAlignTool)
	{
		vector<CCalibrateParam*> TmpCalibrateParam = tmpAlignTool->GetCalibParams();
		{
			if (platformInfo.m_bEnableMultiCalibExtension)
			{
				if (nPosIndex%platformInfo.m_nPositionNum<TmpCalibrateParam.size())
				{
					if (TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]!=NULL)
					{
						dCalibMoveX = TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]->GetPlatformCalibMovement().GetPosX();
						dCalibMoveY = TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]->GetPlatformCalibMovement().GetPosY();
						dCalibMoveD = TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]->GetPlatformCalibMovement().GetAngle();
					}
				}
			}
			else
			{
				if (nPosIndex<TmpCalibrateParam.size())
				{
					if (TmpCalibrateParam[nPosIndex]!=NULL)
					{
						dCalibMoveX = TmpCalibrateParam[nPosIndex]->GetPlatformCalibMovement().GetPosX();
						dCalibMoveY = TmpCalibrateParam[nPosIndex]->GetPlatformCalibMovement().GetPosY();
						dCalibMoveD = TmpCalibrateParam[nPosIndex]->GetPlatformCalibMovement().GetAngle();
					}
				}
			}

		}
	
	}

	// 判断不同平台的处理方式
	switch(platformInfo.m_eMidPlatformType)
	{
	case ePlatformXYD:
	case ePlatformXY1Y2:
	case ePlatformX1X2Y:
	case ePlatformXYPD:
		{
			//  运用向量叉乘，判断九宫格顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY));
			
			z = distance[0]*distance[3] - distance[1]*distance[2];
			if (z<0)
			{
				nImageIndex=1;
				strTemp.Format(_T("逆时针"));
			} 
			else
			{
				nImageIndex=2;
				strTemp.Format(_T("顺时针"));
			}

			int nColsIndex = nPosIndex;
			if (platformInfo.m_bEnableMultiCalibExtension && platformInfo.m_bTargetObjectCamSeparate)
			{
				nColsIndex = nPosIndex%(platformInfo.m_nPositionNum)+(nPosIndex/(platformInfo.m_nPositionNum))*(platformInfo.m_nPositionNum/2);
			}


			int nRow =0;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//  运用向量叉乘，判断角度旋转顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[10].m_dPosX - cpObjectMarkImgCoordPos[11].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[10].m_dPosY - cpObjectMarkImgCoordPos[11].m_dPosY));
			distance.push_back((cpObjectMarkImgCoordPos[11].m_dPosX - cpObjectMarkImgCoordPos[9].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[11].m_dPosY - cpObjectMarkImgCoordPos[9].m_dPosY));
			z = distance[0]*distance[3] - distance[1]*distance[2];
			if (z>0)
			{
				strTemp.Format(_T("先顺后逆"));
				nImageIndex=1;
			} 
			else
			{
				strTemp.Format(_T("先逆后顺"));
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//X图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[3].m_dPosX - cpObjectMarkImgCoordPos[4].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[7].m_dPosX - cpObjectMarkImgCoordPos[8].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[8].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[6].m_dPosX));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//Y图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[2].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[6].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[6].m_dPosY - cpObjectMarkImgCoordPos[7].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[4].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[8].m_dPosY));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//XY图像移动距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[3].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[7].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[7].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());

			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//D方向移动图像距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[10].m_dPosX - cpObjectMarkImgCoordPos[11].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[10].m_dPosY - cpObjectMarkImgCoordPos[11].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[9].m_dPosX - cpObjectMarkImgCoordPos[11].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[9].m_dPosY - cpObjectMarkImgCoordPos[11].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			

			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
				// 根据向量911与向量1110的夹角是否与设定的轴移动角度步长一致来确定是否存在角度走位不准问题
				{
					scLineSeg L911;
					scLineSeg L1110;

					if ((cpObjectMarkImgCoordPos[9]==cpObjectMarkImgCoordPos[11]) || (cpObjectMarkImgCoordPos[10]==cpObjectMarkImgCoordPos[11]))
					{

					}
					else
					{
						L911.Set(sc2Vector(cpObjectMarkImgCoordPos[9].m_dPosX,cpObjectMarkImgCoordPos[9].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY));
						L1110.Set(sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[10].m_dPosX,cpObjectMarkImgCoordPos[10].m_dPosY));
						scRadian R;
						sfAngleLine2Line(L1110.GetLine(),L911.GetLine(),R);
						scDegree D = scDegree(R);
						if(abs(fabs(D.ToDouble()) -dCalibMoveD)>2)
						{
							strTemp.AppendFormat(_T("旋转夹角%.3f：角度步长：%.3f,偏差：%.3f"),fabs(D.ToDouble()),dCalibMoveD,abs(fabs(D.ToDouble()) -dCalibMoveD));
							nImageIndex=2;
						}
						else
						{
							strTemp.AppendFormat(_T("旋转夹角%.3f：角度步长：%.3f"),fabs(D.ToDouble()),dCalibMoveD);
						}
					}


				}

				
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);



			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[11].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[11].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));

			if (fabs(distance[0])<nLimit &&fabs(distance[1])<nLimit)
			{
				strTemp.Format(_T("X:%.1f:Y:%.1f,偏差%.1f"),distance[0],distance[1],max(fabs(distance[0]),fabs(distance[1])));
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),max(fabs(distance[0]),fabs(distance[1])),nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//比例对应分析结果


			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			if (distance[1]!=0 && dCalibMoveY!=0)
			{
				distance.push_back(distance[0]/distance[1]);
				strTemp.Format(_T("比例对应: 距离02/12比值: %.2f; 标定步长比值X/Y: %.2f; 差值: %.2f;\n"),distance[2],dCalibMoveX/dCalibMoveY,fabs(dCalibMoveX/dCalibMoveY - distance[2]));

				if (fabs(dCalibMoveX/dCalibMoveY - distance[2])<=0.05)
				{
					strTemp.Format(_T("图像比: %.3f; 步长比: %.3f; 差值: %.3f;\n"),distance[2],dCalibMoveX/dCalibMoveY,fabs(dCalibMoveX/dCalibMoveY - distance[2]));
					nImageIndex=1;
				}
				else
				{
					strTemp.Format(_T("偏差超过%f"),0.05);
					nImageIndex=2;
				}
			}
			else
			{
				strTemp.Format(_T("从1位置移动到2位置移动范围过小"));
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//// 标定移动最大范围比较
			distance.clear();
			sc2Vector CalibrateMoveRange;
			CalibrateMoveRange = sc2Vector(0,0);
			for (int i=0;i<CALIBMOVEPOSNUM;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosX);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetX(MaxDistance);
			distance.clear();
			for (int i=0;i<CALIBMOVEPOSNUM;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosY);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetY(MaxDistance);

			strTemp.Format(_T("X:%.1f,Y%.1f"),CalibrateMoveRange.GetX(),CalibrateMoveRange.GetY());

			nImageIndex=1;
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);
		}
		break;
	case ePlatformXD:
		{
			//  运用向量叉乘，判断九宫格顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));

			if (distance[0] > 0 && distance[1] < 0)
			{
				nImageIndex=1;
				strTemp.Format(_T("先右后左"));
			} 
			else
			{
				nImageIndex=2;
				strTemp.Format(_T("先左后右"));
			}

			int nColsIndex = nPosIndex;
			if (platformInfo.m_bEnableMultiCalibExtension && platformInfo.m_bTargetObjectCamSeparate)
			{
				nColsIndex = nPosIndex%platformInfo.m_nPositionNum+nPosIndex/(platformInfo.m_nPositionNum/2);
			}

			int nRow =0;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//  运用向量叉乘，判断角度旋转顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[4].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY));
			distance.push_back((cpObjectMarkImgCoordPos[5].m_dPosX - cpObjectMarkImgCoordPos[3].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY));
			z = distance[0]*distance[3] - distance[1]*distance[2];
			if (z>0)
			{
				strTemp.Format(_T("先顺后逆"));
				nImageIndex=1;
			} 
			else
			{
				strTemp.Format(_T("先逆后顺"));
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//X图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//Y图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//XY图像移动距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//D方向移动图像距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[4].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[3].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[3].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
				// 根据向量911与向量1110的夹角是否与设定的轴移动角度步长一致来确定是否存在角度走位不准问题
				{
					scLineSeg L911;
					scLineSeg L1110;
					if ((cpObjectMarkImgCoordPos[9]==cpObjectMarkImgCoordPos[11]) || (cpObjectMarkImgCoordPos[10]==cpObjectMarkImgCoordPos[11]))
					{

					}
					else
					{
						L911.Set(sc2Vector(cpObjectMarkImgCoordPos[9].m_dPosX,cpObjectMarkImgCoordPos[9].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY));
						L1110.Set(sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[10].m_dPosX,cpObjectMarkImgCoordPos[10].m_dPosY));
						scRadian R;
						sfAngleLine2Line(L1110.GetLine(),L911.GetLine(),R);
						scDegree D = scDegree(R);
						if(abs(fabs(D.ToDouble()) -dCalibMoveD)>0.05)
						{
							strTemp.AppendFormat(_T("角度走位不准：偏差：%.3f"),abs(fabs(D.ToDouble()) -dCalibMoveD));
							nImageIndex=2;
						}
					}

				}

			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			// 起点终点
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));

			if (fabs(distance[0])<nLimit &&fabs(distance[1])<nLimit)
			{
				strTemp.Format(_T("X:%.1f:Y:%.1f,偏差%.1f"),distance[0],distance[1],max(fabs(distance[0]),fabs(distance[1])));
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),max(fabs(distance[0]),fabs(distance[1])),nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//比例对应分析结果 无
			strTemp.Format(_T("图像比: %.3f; 步长比: %.3f; 差值: %.3f;\n"),1,1,0);
			nImageIndex=1;

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//标定移动最大范围比较
			distance.clear();
			sc2Vector CalibrateMoveRange;
			CalibrateMoveRange = sc2Vector(0,0);
			for (int i=0;i<CALIBMOVEPOSNUM && i<6;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosX);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetX(MaxDistance);
			CalibrateMoveRange.SetY(MaxDistance);

			strTemp.Format(_T("X:%.1f,Y%.1f"),CalibrateMoveRange.GetX(),CalibrateMoveRange.GetY());

			nImageIndex=1;
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);
		}
		break;
	case ePlatformYD:
		{
			//  运用向量叉乘，判断九宫格顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));

			if (distance[0] > 0 && distance[1] < 0)
			{
				nImageIndex=1;
				strTemp.Format(_T("先下后上"));
			} 
			else
			{
				nImageIndex=2;
				strTemp.Format(_T("先上后下"));
			}

			int nColsIndex = nPosIndex;
			if (platformInfo.m_bEnableMultiCalibExtension && platformInfo.m_bTargetObjectCamSeparate)
			{
				nColsIndex = nPosIndex%platformInfo.m_nPositionNum+nPosIndex/(platformInfo.m_nPositionNum/2);
			}

			int nRow =0;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//  运用向量叉乘，判断角度旋转顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[4].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY));
			distance.push_back((cpObjectMarkImgCoordPos[5].m_dPosX - cpObjectMarkImgCoordPos[3].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY));
			z = distance[0]*distance[3] - distance[1]*distance[2];
			if (z>0)
			{
				strTemp.Format(_T("先顺后逆"));
				nImageIndex=1;
			} 
			else
			{
				strTemp.Format(_T("先逆后顺"));
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//X图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//Y图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//XY图像移动距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//D方向移动图像距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[4].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[3].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[3].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
				// 根据向量911与向量1110的夹角是否与设定的轴移动角度步长一致来确定是否存在角度走位不准问题
				{
					scLineSeg L911;
					scLineSeg L1110;
					if ((cpObjectMarkImgCoordPos[9]==cpObjectMarkImgCoordPos[11]) || (cpObjectMarkImgCoordPos[10]==cpObjectMarkImgCoordPos[11]))
					{

					}
					else
					{
						L911.Set(sc2Vector(cpObjectMarkImgCoordPos[9].m_dPosX,cpObjectMarkImgCoordPos[9].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY));
						L1110.Set(sc2Vector(cpObjectMarkImgCoordPos[11].m_dPosX,cpObjectMarkImgCoordPos[11].m_dPosY),sc2Vector(cpObjectMarkImgCoordPos[10].m_dPosX,cpObjectMarkImgCoordPos[10].m_dPosY));
						scRadian R;
						sfAngleLine2Line(L1110.GetLine(),L911.GetLine(),R);
						scDegree D = scDegree(R);
						if(abs(fabs(D.ToDouble()) -dCalibMoveD)>0.05)
						{
							strTemp.AppendFormat(_T("角度走位不准：偏差：%.3f"),abs(fabs(D.ToDouble()) -dCalibMoveD));
							nImageIndex=2;
						}
					}

				}
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			// 起点终点
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));

			if (fabs(distance[0])<nLimit &&fabs(distance[1])<nLimit)
			{
				strTemp.Format(_T("X:%.1f:Y:%.1f,偏差%.1f"),distance[0],distance[1],max(fabs(distance[0]),fabs(distance[1])));
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),max(fabs(distance[0]),fabs(distance[1])),nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//比例对应分析结果 无
			strTemp.Format(_T("图像比: %.3f; 步长比: %.3f; 差值: %.3f;\n"),1,1,0);
			nImageIndex=1;

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//标定移动最大范围比较
			distance.clear();
			sc2Vector CalibrateMoveRange;
			CalibrateMoveRange = sc2Vector(0,0);
			for (int i=0;i<CALIBMOVEPOSNUM && i<6;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosY);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetX(MaxDistance);
			CalibrateMoveRange.SetY(MaxDistance);

			strTemp.Format(_T("X:%.1f,Y%.1f"),CalibrateMoveRange.GetX(),CalibrateMoveRange.GetY());

			nImageIndex=1;
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);
		}
		break;
	case ePlatformXY:
		{
			//  运用向量叉乘，判断九宫格顺逆
			distance.clear();
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back((cpObjectMarkImgCoordPos[2].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY));
			double z = 0;
			z = distance[0]*distance[3] - distance[1]*distance[2];
			if (z<0)
			{
				nImageIndex=1;
				strTemp.Format(_T("逆时针"));
			} 
			else
			{
				nImageIndex=2;
				strTemp.Format(_T("顺时针"));
			}

			int nColsIndex = nPosIndex;
			if (platformInfo.m_bEnableMultiCalibExtension && platformInfo.m_bTargetObjectCamSeparate)
			{
				nColsIndex = nPosIndex%platformInfo.m_nPositionNum+nPosIndex/(platformInfo.m_nPositionNum/2);
			}

			int nRow =0;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//  运用向量叉乘，判断角度旋转顺逆--无
			distance.clear();
			strTemp.Format(_T("无"));
			nImageIndex=1;

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//X图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[3].m_dPosX - cpObjectMarkImgCoordPos[4].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[4].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[7].m_dPosX - cpObjectMarkImgCoordPos[8].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[8].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[6].m_dPosX));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());

			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			//Y图像偏差
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[2].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[5].m_dPosY - cpObjectMarkImgCoordPos[6].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[6].m_dPosY - cpObjectMarkImgCoordPos[7].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[4].m_dPosY));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[8].m_dPosY));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//XY图像移动距离
			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[1].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[1].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[3].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[3].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[5].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[5].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[7].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[7].m_dPosY,2)));
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());

			if (MaxDistance<=nLimit)
			{
				strTemp.Format(_T("Max:%.1f:Min:%.1f,偏差%.1f"),*max_element(distance.begin(),distance.end()),*min_element(distance.begin(),distance.end()),MaxDistance);
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),MaxDistance,nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//D方向移动图像距离
			distance.clear();
			strTemp.Format(_T("无角度走位"));
			nImageIndex=1;

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			// 起点终点
			distance.clear();
			distance.push_back(fabs(cpObjectMarkImgCoordPos[9].m_dPosX - cpObjectMarkImgCoordPos[0].m_dPosX));
			distance.push_back(fabs(cpObjectMarkImgCoordPos[9].m_dPosY - cpObjectMarkImgCoordPos[0].m_dPosY));

			if (fabs(distance[0])<nLimit &&fabs(distance[1])<nLimit)
			{
				strTemp.Format(_T("X:%.1f:Y:%.1f,偏差%.1f"),distance[0],distance[1],max(fabs(distance[0]),fabs(distance[1])));
				nImageIndex=1;
			}
			else
			{
				strTemp.Format(_T("最大偏差%.1f超过%d"),max(fabs(distance[0]),fabs(distance[1])),nLimit);
				nImageIndex=2;
			}
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//比例对应分析结果
			double dCalibMoveX = 1;
			double dCalibMoveY = 1;
			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();

			if (NULL != tmpAlignTool)
			{
				vector<CCalibrateParam*> TmpCalibrateParam = tmpAlignTool->GetCalibParams();
				{
					if (platformInfo.m_bEnableMultiCalibExtension)
					{
						if (nPosIndex%platformInfo.m_nPositionNum<TmpCalibrateParam.size())
						{
							if (TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]!=NULL)
							{
								dCalibMoveX = TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]->GetPlatformCalibMovement().GetPosX();
								dCalibMoveY = TmpCalibrateParam[nPosIndex%platformInfo.m_nPositionNum]->GetPlatformCalibMovement().GetPosY();
							}
						}
					}
					else
					{
						if (nPosIndex<TmpCalibrateParam.size())
						{
							if (TmpCalibrateParam[nPosIndex]!=NULL)
							{
								dCalibMoveX = TmpCalibrateParam[nPosIndex]->GetPlatformCalibMovement().GetPosX();
								dCalibMoveY = TmpCalibrateParam[nPosIndex]->GetPlatformCalibMovement().GetPosY();
							}
						}
					}

				}
			}

			distance.clear();
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[0].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[0].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			distance.push_back(sqrt(pow(cpObjectMarkImgCoordPos[1].m_dPosX - cpObjectMarkImgCoordPos[2].m_dPosX,2)+pow(cpObjectMarkImgCoordPos[1].m_dPosY - cpObjectMarkImgCoordPos[2].m_dPosY,2)));
			if (distance[1]!=0 && dCalibMoveY!=0)
			{
				distance.push_back(distance[0]/distance[1]);
				strTemp.Format(_T("比例对应: 距离02/12比值: %.2f; 标定步长比值X/Y: %.2f; 差值: %.2f;\n"),distance[2],dCalibMoveX/dCalibMoveY,fabs(dCalibMoveX/dCalibMoveY - distance[2]));

				if (fabs(dCalibMoveX/dCalibMoveY - distance[2])<=0.05)
				{
					strTemp.Format(_T("图像比: %.3f; 步长比: %.3f; 差值: %.3f;\n"),distance[2],dCalibMoveX/dCalibMoveY,fabs(dCalibMoveX/dCalibMoveY - distance[2]));
					nImageIndex=1;
				}
				else
				{
					strTemp.Format(_T("偏差超过%f"),0.05);
					nImageIndex=2;
				}
			}
			else
			{
				strTemp.Format(_T("从1位置移动到2位置移动范围过小"));
				nImageIndex=2;
			}

			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			//// 标定移动最大范围比较
			distance.clear();
			sc2Vector CalibrateMoveRange;
			CalibrateMoveRange = sc2Vector(0,0);
			for (int i=0;i<CALIBMOVEPOSNUM && i<10;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosX);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetX(MaxDistance);
			distance.clear();
			for (int i=0;i<CALIBMOVEPOSNUM && i<10;i++)
			{
				distance.push_back(cpObjectMarkImgCoordPos[i].m_dPosY);
			}
			MaxDistance = *max_element(distance.begin(),distance.end())-*min_element(distance.begin(),distance.end());
			CalibrateMoveRange.SetY(MaxDistance);

			strTemp.Format(_T("X:%.1f,Y%.1f"),CalibrateMoveRange.GetX(),CalibrateMoveRange.GetY());

			nImageIndex=1;
			nRow++;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);
		}
		break;
	default:
		{
			return;
		}
		break;
	}

}

void CDlgCalibrateEvaluate::ShowCalibrateResult(vcBaseVisionAlign *pVisionASM,CListCtrl * m_pList,int nPosIndex)
{
	if (NULL !=pVisionASM && m_pList!=NULL)
	{

		//int nRowCount = m_pList->GetItemCount();
		//for (int it=1;it<nRowCount;it++)
		//{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);


			CTotalCalibData *pTotalCalibData;
			pTotalCalibData = pVisionASM->GetTotalCalibData();
			if (pTotalCalibData == NULL)
			{
				return;
			}

			// 获取当前产品对应的标定数据
			CCalibData* pCalibData;
			pCalibData = pTotalCalibData->GetItemPtr(m_nCurProductIdx);
			if (pCalibData == NULL)
			{
				return;
			}

			int nShowPosIndex = nPosIndex;
			if (platformInfo.m_bTargetObjectCamSeparate && platformInfo.m_eMidPlatformType ==ePlatformXYPD)
			{
				nShowPosIndex =nPosIndex + platformInfo.m_nPositionNum/2;
			}

			CCalibratedInfo* pCalibratedInfo;
			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(nShowPosIndex);
			if (pCalibratedInfo == NULL)
			{
				return;
			}

			FixCalibrateResultImageAnaysisData(pCalibratedInfo->m_cpObjectMarkImgCoordPos,m_pList,nPosIndex);


			int nColsIndex = nPosIndex;
			if (platformInfo.m_bEnableMultiCalibExtension && platformInfo.m_bTargetObjectCamSeparate)
			{
				if (platformInfo.m_eMidPlatformType==ePlatformXYPD)
				{
					int nTmp = nShowPosIndex-platformInfo.m_nPositionNum/2;
					nColsIndex = nTmp%(platformInfo.m_nPositionNum)+(nTmp/(platformInfo.m_nPositionNum))*(platformInfo.m_nPositionNum/2);
				}
				else
				{
					nColsIndex = nPosIndex%(platformInfo.m_nPositionNum)+(nPosIndex/(platformInfo.m_nPositionNum))*(platformInfo.m_nPositionNum/2);
				}
			}


			CString strStatus[2];
			strStatus[0]= _T("OK");
			strStatus[1] = _T("NG");
			CString strTemp = _T("");
			// 3*3矩阵
			double h[9];
			memset(h, 0.0, sizeof(double)*9);
			h[0] = 1.0;
			h[4] = 1.0;
			h[8] = 1.0;
			pCalibratedInfo->GetPlatformTransferH(h);

			int nImageIndex = 1;
			int nRow = 9; 
			strTemp.Format(_T("h0:%.6f,h4:%.6f,偏差:%.6f"),h[0],h[4],fabs(h[0]-h[4]));
			if (h[0]/h[4]<0.95 || h[0]/h[4]>1.05)
			{
				nImageIndex = 2;
			}
			// h0 /h4
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			CCoordPos pos = pCalibratedInfo->GetMarkPlatformCoordPos();
			nRow++; 
			// 平台坐标
			strTemp.Format(_T("X:%.4f,Y:%.4f"),pos.GetPosX(),pos.GetPosY());
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			 pos =pCalibratedInfo->GetMarkImgCoordPos();
			nRow++;
			strTemp.Format(_T("X:%.4f,Y:%.4f"),pos.GetPosX(),pos.GetPosY());
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
			nRow++;
			strTemp.Format(_T("X:%.4f,Y:%.4f,D:%.4f"),pCalibPlatformAxisPos->m_dPosX,pCalibPlatformAxisPos->m_dPosY,pCalibPlatformAxisPos->m_dAngle);
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			pos = pCalibratedInfo->GetMarkPlatformCoordPosRefineOffset();
			nRow++;
			strTemp.Format(_T("X:%.6f,Y:%.6f"),pos.GetPosX(),pos.GetPosY());
			nImageIndex = 1;
			if (fabs(pos.GetPosX())>5*fabs(h[0]) || fabs(pos.GetPosY())>5*fabs(h[4]) )
			{
				nImageIndex = 2;
			}
			
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);


			// 像素分辨率
			nRow++;
			strTemp.Format(_T("%.6f"),((sqrt(h[0]*h[0]+h[1]*h[1]))+(sqrt(h[3]*h[3]+h[4]*h[4])))/2);
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			// 轴夹角
			nRow++;
			strTemp.Format(_T("%.6f"),((atan2(h[0],h[1]))+(atan2(h[4],-h[3])))/2);
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

			// 视野
			nRow++;
			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
			  // 图像范围
			double dImageSizeX = 640;
			double dImageSizeY = 480;
			 // 视野范围
			double dFieldSizeX = 0;
			double dFieldSizeY = 0;

			if (NULL != tmpAlignTool)
			{
				CCoordPos TmpCoord = tmpAlignTool->GetImageSize(nPosIndex%platformInfo.m_nPositionNum);
				dImageSizeX = TmpCoord.GetPosX();
				dImageSizeY = TmpCoord.GetPosY();
				dFieldSizeX =dImageSizeX*((sqrt(h[0]*h[0]+h[1]*h[1]))+(sqrt(h[3]*h[3]+h[4]*h[4])))/2;
				dFieldSizeY =dImageSizeY*((sqrt(h[0]*h[0]+h[1]*h[1]))+(sqrt(h[3]*h[3]+h[4]*h[4])))/2;
			}

			strTemp.Format(_T("%.3f*%.3f"),dFieldSizeX,dFieldSizeY);
			nImageIndex = 1;
			m_pList->SetItemText(nRow,2*nColsIndex+1,strTemp);
			m_pList->SetItem(nRow,2*nColsIndex+2,LVIF_TEXT|LVIF_IMAGE,strStatus[nImageIndex-1],nImageIndex,0,0,0);

		//}
	}
}