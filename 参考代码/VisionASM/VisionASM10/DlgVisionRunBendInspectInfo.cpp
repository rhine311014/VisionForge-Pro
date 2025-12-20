// DlgVisionRunBendInspectInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVisionRunBendInspectInfo.h"
#include "afxdialogex.h"


// CDlgVisionRunBendInspectInfo 对话框

IMPLEMENT_DYNAMIC(CDlgVisionRunBendInspectInfo, CDialog)

CDlgVisionRunBendInspectInfo::CDlgVisionRunBendInspectInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVisionRunBendInspectInfo::IDD, pParent)
{
	m_pParent = NULL;
	m_pVisionASM = NULL;

	m_bShowBendDlg = FALSE;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = TRUE;

	m_nBendShowInfo = 0;

	for(int i = 0;i<MAX_ITEM_NUM;i++)
	{
		for(int j = 0;j<MAX_COL_NUM;j++)
		{
			m_strListDataSimple[i][j]	= _T("0.000");
			m_strListDataCircle[i][j]	= _T("0.000");
			m_strListDataDetal[i][j]	= _T("0.000");

			m_bSimpleOK[i][j] = TRUE;
			m_bCircleOK[i][j] = TRUE;
		}
	}

	// 折弯检测
	m_strBendInsBase = _T("0.000");
	m_bSimpleStanderShow = FALSE;
	m_nListSimpleItemNum = 0;
	m_LastSimpleItem = 0;
	m_LastSimpleSubItem = 0;


	// 圆检测
	m_strCircleInsBase = _T("0.000");
	m_bCircleStanderShow = FALSE;
	m_nListCircleItemNum = 0;
	m_LastCircleItem = 0;
	m_LastCircleSubItem = 0;

	// 详细信息
	m_nListDetaItemNum = 0;
	m_nRecordNumMax = 100;

}

CDlgVisionRunBendInspectInfo::~CDlgVisionRunBendInspectInfo()
{
}

void CDlgVisionRunBendInspectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BEND_SIMPLE, m_ListBendSimple);
	DDX_Control(pDX, IDC_LIST_BEND_CIRCLE, m_ListBendCircle);
	DDX_Radio(pDX, IDC_RADIO_BEND_INSPECT, m_nBendShowInfo);
	DDX_Text(pDX, IDC_EDIT_BEND_SHOW, m_strBendInsBase);
	DDX_Control(pDX, IDC_EDIT_BEND_SHOW, m_editBendInsBase);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_SHOW, m_strCircleInsBase);
	DDX_Control(pDX, IDC_EDIT_CIRCLE_SHOW, m_editCircleInsBase);
	DDX_Control(pDX, IDC_LIST_BEND_DETAL, m_ListBendDetal);
	DDX_Text(pDX, IDC_EDIT_MAX_RECORD_NUM, m_nRecordNumMax);
}


BEGIN_MESSAGE_MAP(CDlgVisionRunBendInspectInfo, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_BEND_SHOW, &CDlgVisionRunBendInspectInfo::OnKillfocusEditBendShow)
	ON_EN_SETFOCUS(IDC_EDIT_BEND_SHOW, &CDlgVisionRunBendInspectInfo::OnSetfocusEditBendShow)
	ON_EN_KILLFOCUS(IDC_EDIT_CIRCLE_SHOW, &CDlgVisionRunBendInspectInfo::OnKillfocusEditCircleShow)
	ON_EN_SETFOCUS(IDC_EDIT_CIRCLE_SHOW, &CDlgVisionRunBendInspectInfo::OnSetfocusEditCircleShow)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BEND_SIMPLE, &CDlgVisionRunBendInspectInfo::OnCustomdrawListBendSimple)  //NM_CUSTOMDRAW
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BEND_SIMPLE, &CDlgVisionRunBendInspectInfo::OnDblclkListBendSimple)  //NM_DBLICK
	ON_NOTIFY(NM_CLICK, IDC_LIST_BEND_SIMPLE, &CDlgVisionRunBendInspectInfo::OnClickListBendSimple)	   //Nm_Click
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BEND_CIRCLE, &CDlgVisionRunBendInspectInfo::OnCustomdrawListBendCircle)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BEND_CIRCLE, &CDlgVisionRunBendInspectInfo::OnDblclkListBendCircle)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BEND_CIRCLE, &CDlgVisionRunBendInspectInfo::OnClickListBendCircle)
	ON_BN_CLICKED(IDC_RADIO_BEND_INSPECT, &CDlgVisionRunBendInspectInfo::OnBnClickedRadioBendInspect)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_INSPECT, &CDlgVisionRunBendInspectInfo::OnBnClickedRadioBendInspect)
	ON_BN_CLICKED(IDC_RADIO_BEND_DETAL, &CDlgVisionRunBendInspectInfo::OnBnClickedRadioBendInspect)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_RECORD_NUM, &CDlgVisionRunBendInspectInfo::OnKillfocusEditMaxRecordNum)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_RECORD_NUM, &CDlgVisionRunBendInspectInfo::OnSetfocusEditMaxRecordNum)
	ON_BN_CLICKED(IDC_BTN_DETAL_CLEAR, &CDlgVisionRunBendInspectInfo::OnBnClickedBtnDetalClear)
END_MESSAGE_MAP()


// CDlgVisionRunBendInspectInfo 消息处理程序

CWnd* CDlgVisionRunBendInspectInfo::SetParentWnd( CDlgVisionRun* pParent )
{
	CWnd* pPrevParent = (CWnd*)m_pParent;
	m_pParent = pParent;
	return pPrevParent;
}

void CDlgVisionRunBendInspectInfo::SetVisionASMRef( vcBaseVisionAlign* pVisionASM )
{
	m_pVisionASM = pVisionASM;
}

void CDlgVisionRunBendInspectInfo::UpdateDlgParamItem()
{
	//if (m_pParent->m_pParent->m_SysOptionConfig.m_bMainframeSetBendParam == TRUE)
	//{
	//	GetDlgItem(IDC_LIST_BEND_SIMPLE)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_LIST_BEND_CIRCLE)->EnableWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(IDC_LIST_BEND_SIMPLE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_LIST_BEND_CIRCLE)->EnableWindow(FALSE);
	//}

	if (m_pVisionASM != NULL)
	{
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);

		m_bShowBendDlg = optionInfo.m_bShowBend;
	}
}

void CDlgVisionRunBendInspectInfo::UpdateDialogLanguage()
{
//	return;


	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent && m_pParent->m_pParent &&m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,0));
	}

	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

	vcXMLConfigurator xml;
	CString strXMLPath;

	switch (lgidLanguage)
	{
	case MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\ENG.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL):
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHT.xml");
		break;
	case MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED):
	default:
		strXMLPath = strCurWorkDir + _T("\\VSConfig\\Language\\CHS.xml");
		break;
	}
	xml.Load(strXMLPath);
	xml.FindElem(_T("ResourceBundle")); 
	xml.EnterElem();

	while(xml.FindElem(_T("")))
	{
		CString strDlgID = xml.GetAttribute(_T("Key"));

		if(_T("IDD_DLG_VISION_RUN_BEND_INSPECT_INFO") == strDlgID)
		{
			int nID;
			CString strValue = xml.GetAttribute(_T("Value"));
			SetWindowText(strValue);

			xml.EnterElem();	   
			while(xml.FindElem())
			{
				xml.GetAttribute(_T("Id"),  nID);
				strValue = xml.GetAttribute(_T("Value"));

				if(GetDlgItem(nID) != NULL)
				{
					GetDlgItem(nID)->SetWindowText(strValue);
				}	  
			}
			xml.LeaveElem();
		}  
	}
	xml.LeaveElem();	
}

void CDlgVisionRunBendInspectInfo::SetItemFont()
{

}

BOOL CDlgVisionRunBendInspectInfo::IsStringPositiveNumerical(LPCTSTR lpszVal)
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if (((ch > '9') || ((ch < '0'))) && (ch != '.'))
		{
			if ( (i == 0) && ((ch == '+')) || ((ch == '-')) )
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgVisionRunBendInspectInfo::IsStringNumerical( LPCTSTR lpszVal )
{
	CString strVal = lpszVal;
	if (strVal.IsEmpty())
	{
		return FALSE;
	}
	int nLen = strVal.GetLength();
	TCHAR ch = 0;
	for (int i=0; i<nLen; i++)
	{
		ch = strVal.GetAt(i);
		if ((ch > '9') || ((ch < '0') && (ch != '.')))
		{
			if ((i == 0) && ((ch == '-') || (ch == '+')))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgVisionRunBendInspectInfo::UpdateBendBaseInfo(BOOL bUpdate)
{
	CBendInfo BendInfo;
	CTotalProductData* pTotalProduct = m_pVisionASM->GetTotalProuductData();
	int nCurProductIndex = pTotalProduct->GetCurProductIndex();

	CProductData* pProductData  = pTotalProduct->GetItemPtr(nCurProductIndex);
	if (pProductData != NULL)
	{
		BendInfo = pProductData->m_pAlignerParam->GetBendInfo();
	}

	if(bUpdate)
	{
		if (pProductData == NULL)
		{
			return;
		}

		for(int i =0;i<3;i++)// 012分别对应 基准值、最小值、最大值
		{
			double dData;
			CString str;
			int nItem = 0;

			// 0LX
			str = m_ListBendSimple.GetItemText(0,i+1);
			m_strListDataSimple[0][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeLX[i] = dData;

			// 1LY
			str = m_ListBendSimple.GetItemText(1,i+1);
			m_strListDataSimple[1][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeLY[i] = dData;

			// 2LDis
			str = m_ListBendSimple.GetItemText(2,i+1);
			m_strListDataSimple[2][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeLDis[i] = dData;

			// 3RX
			str = m_ListBendSimple.GetItemText(3,i+1);
			m_strListDataSimple[3][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeRX[i] = dData;

			// 4RY
			str = m_ListBendSimple.GetItemText(4,i+1);
			m_strListDataSimple[4][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeRY[i] = dData;

			// 5RDis
			str = m_ListBendSimple.GetItemText(5,i+1);
			m_strListDataSimple[5][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeRDis[i] = dData;

			// 6TargetDis
			str = m_ListBendSimple.GetItemText(6,i+1);
			m_strListDataSimple[6][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeTargetDis[i] = dData;

			// 7ObjectDis
			str = m_ListBendSimple.GetItemText(7,i+1);
			m_strListDataSimple[7][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeObjectDis[i] = dData;

			// 8LX-RX
			str = m_ListBendSimple.GetItemText(8,i+1);
			m_strListDataSimple[8][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeSubtractX[i] = dData;


			// 9LY-RY
			str = m_ListBendSimple.GetItemText(9,i+1);
			m_strListDataSimple[9][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeSubtractY[i] = dData;

			//DeltaY
			str = m_ListBendSimple.GetItemText(10,i+1);
			m_strListDataSimple[10][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dDeltaY[i] = dData;

		}

		pProductData->m_pAlignerParam->SetBendInfo(BendInfo);
		m_pVisionASM->SaveProductDataInfo(nCurProductIndex);

	}
	else
	{
		for(int i =0;i<3;i++)// 012分别对应 基准值、最小值、最大值
		{
			double dData;
			CString str;
			int nItem = 0;

			// 0LX
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeLX[i];		
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(0,i+1,str);
			m_strListDataSimple[0][i+1] = str;

			// 1LY
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeLY[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(1,i+1,str);
			m_strListDataSimple[1][i+1] = str;

			// 2LDis
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeLDis[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(2,i+1,str);
			m_strListDataSimple[2][i+1] = str;

			// 3RX
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeRX[i];		
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(3,i+1,str);
			m_strListDataSimple[3][i+1] = str;

			// 4RY
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeRY[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(4,i+1,str);
			m_strListDataSimple[4][i+1] = str;

			// 5RDis
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeRDis[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(5,i+1,str);
			m_strListDataSimple[5][i+1] = str;


			// 6TargetDis
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeTargetDis[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(6,i+1,str);
			m_strListDataSimple[6][i+1] = str;

			// 7ObjectDis
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeObjectDis[i];		
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(7,i+1,str);
			m_strListDataSimple[7][i+1] = str;

			// 8SubtrancX
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeSubtractX[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(8,i+1,str);
			m_strListDataSimple[8][i+1] = str;

			// 9SubtrancY
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeSubtractY[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(9,i+1,str);
			m_strListDataSimple[9][i+1] = str;

			// DeltaY
			dData = BendInfo.m_BendInspectRangeInfo.m_dDeltaY[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendSimple.SetItemText(10,i+1,str);
			m_strListDataSimple[10][i+1] = str;
		}
 	}

	UpdateBendBaseListColor();
}

void CDlgVisionRunBendInspectInfo::UpdateBendBaseResult(BOOL bUpdate)
{
	if (FALSE == bUpdate)
	{
		BendInspectResultInfo	TempBendInspectResult;
		m_BendInspectResult = TempBendInspectResult;
	}

	CString str;
	int nItem = 0, nSubItem = 4;

	// LX
	nItem = 0, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dLx);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// LY
	nItem = 1, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dLy);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// LDIS
	nItem = 2, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dDistanceL);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// RX
	nItem = 3, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dRx);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// RY
	nItem = 4, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dRy);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// RDIS
	nItem = 5, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dDistanceR);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// TargetDistance
	nItem = 6, nSubItem = 6;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dTargetDistance);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// ObjectDistance
	nItem = 7, nSubItem = 6;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dObjectDistance);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// LX-RX
	nItem = 8, nSubItem = 6;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dDistanceSubX);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// LY-RY
	nItem = 9, nSubItem = 6;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dDistanceSubY);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// L-DeltaY
	nItem = 10, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dLDeltaY);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// R-DeltaY
	nItem = 10, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dRDeltaY);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// LDeltaY-RDeltaY
	nItem = 10, nSubItem = 6;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dDeltaY);
	m_ListBendSimple.SetItemText(nItem,nSubItem,str);
	m_strListDataSimple[nItem][nSubItem] = str;

	// 更新检测值时，刷新一下颜色状态
	UpdateBendBaseListColor();
}

void CDlgVisionRunBendInspectInfo::UpdateBendBaseListColor()
{
	for(int i = 0;i<m_nListSimpleItemNum;i++)
	{
		// 测量值Left Right Other 分别对应第4列和 第5列 第6列
		for(int j = 4; j < 7;j++)
		{
			if( atof(m_strListDataSimple[i][j]) >= atof(m_strListDataSimple[i][1]) - atof(m_strListDataSimple[i][2]) 
				&& atof(m_strListDataSimple[i][j]) <= atof(m_strListDataSimple[i][1]) + atof(m_strListDataSimple[i][3]))
			{
				m_bSimpleOK[i][j] = TRUE;
			}
			else
			{
				m_bSimpleOK[i][j] = FALSE;
			}

		}
	}

	// 产品长度都是显示在Right这一列的，所以Left这样一列不作处理

	m_bSimpleOK[0][5] = TRUE;
	m_bSimpleOK[1][5] = TRUE;
	m_bSimpleOK[2][5] = TRUE;
	m_bSimpleOK[3][4] = TRUE;
	m_bSimpleOK[4][4] = TRUE;
	m_bSimpleOK[5][4] = TRUE;


	m_bSimpleOK[0][6] = TRUE;
	m_bSimpleOK[1][6] = TRUE;
	m_bSimpleOK[2][6] = TRUE;
	m_bSimpleOK[3][6] = TRUE;
	m_bSimpleOK[4][6] = TRUE;
	m_bSimpleOK[5][6] = TRUE;

	m_bSimpleOK[6][4] = TRUE;
	m_bSimpleOK[6][5] = TRUE;

	m_bSimpleOK[7][4] = TRUE;
	m_bSimpleOK[7][5] = TRUE;

	m_bSimpleOK[8][4] = TRUE;
	m_bSimpleOK[8][5] = TRUE;

	m_bSimpleOK[9][4] = TRUE;
	m_bSimpleOK[9][5] = TRUE;

	m_bSimpleOK[10][4] = TRUE;
	m_bSimpleOK[10][5] = TRUE;

	Invalidate(TRUE);

}

void CDlgVisionRunBendInspectInfo::UpdateCircleBaseInfo(BOOL bUpdate)
{
	CBendInfo BendInfo;
	CTotalProductData* pTotalProduct = m_pVisionASM->GetTotalProuductData();
	int nCurProductIndex = pTotalProduct->GetCurProductIndex();

	CProductData* pProductData  = pTotalProduct->GetItemPtr(nCurProductIndex);
	if (pProductData != NULL)
	{
		BendInfo = pProductData->m_pAlignerParam->GetBendInfo();
	}

	if(bUpdate)
	{
		for(int i =0;i<3;i++)// 012分别对应 基准值、最小值、最大值
		{
			double dData;
			CString str;
			int nItem = 0;

			// BaseCircleRadius
			str = m_ListBendCircle.GetItemText(0,i+1);
			m_strListDataCircle[0][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadius[i] = dData;



			// 8LX-RX
			str = m_ListBendCircle.GetItemText(1,i+1);
			m_strListDataCircle[1][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadiusX[i] = dData;


			// 9LY-RY
			str = m_ListBendCircle.GetItemText(2,i+1);
			m_strListDataCircle[2][i+1] = str;
			dData = atof(str);
			BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadiusY[i] = dData;


		}

		pProductData->m_pAlignerParam->SetBendInfo(BendInfo);
		m_pVisionASM->SaveProductDataInfo(nCurProductIndex);
	}
	else
	{

		for(int i =0;i<3;i++)// 012分别对应 基准值、最小值、最大值
		{
			double dData;
			CString str;
			int nItem = 0;

			// BaseCircleRadius
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadius[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendCircle.SetItemText(0,i+1,str);
			m_strListDataCircle[0][i+1] = str;

			// 8SubtrancX
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadiusX[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendCircle.SetItemText(1,i+1,str);
			m_strListDataCircle[1][i+1] = str;

			// 9SubtrancY
			dData = BendInfo.m_BendInspectRangeInfo.m_dRangeCircleRadiusY[i];
			str.Format(_T("%.3f"),dData);
			m_ListBendCircle.SetItemText(2,i+1,str);
			m_strListDataCircle[2][i+1] = str;

		}
	}

	UpdateCircleBaseListColor();
}

void CDlgVisionRunBendInspectInfo::UpdateCircleResultSimple(BOOL bUpdate)
{
	if (FALSE == bUpdate)
	{
		BendInspectResultInfo	TempBendInspectResult;
		m_BendInspectResult = TempBendInspectResult;
	}

	CString str;
	int nItem = 0, nSubItem = 4;

	nItem = 0, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadius[0]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;

	nItem = 0, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadius[1]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;

	nItem = 1, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadiusX[0]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;
	nItem = 1, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadiusX[1]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;

	nItem = 2, nSubItem = 4;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadiusY[0]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;
	nItem = 2, nSubItem = 5;
	str.Format(_T("%.4f"),m_BendInspectResult.m_dCircleRadiusY[1]);
	m_ListBendCircle.SetItemText(nItem,nSubItem,str);
	m_strListDataCircle[nItem][nSubItem] = str;

	// 更新检测值时，刷新一下颜色状态
	UpdateCircleBaseListColor();
}

void CDlgVisionRunBendInspectInfo::UpdateCircleBaseListColor()
{
	for(int i = 0;i<m_nListCircleItemNum;i++)
	{
		// 测量值Left Right Other 分别对应第4列和 第5列 第6列
		for(int j = 4; j < 6;j++)
		{
			if( atof(m_strListDataCircle[i][j]) >= atof(m_strListDataCircle[i][1]) - atof(m_strListDataCircle[i][2]) 
				&& atof(m_strListDataCircle[i][j]) <= atof(m_strListDataCircle[i][1]) + atof(m_strListDataCircle[i][3]))
			{
				m_bCircleOK[i][j] = TRUE;
			}
			else
			{
				m_bCircleOK[i][j] = FALSE;
			}

		}
	}


	Invalidate(TRUE);
}

void CDlgVisionRunBendInspectInfo::UpdateBendResultDetal(BOOL bUpdate)
{
	if (FALSE == bUpdate)
	{
		BendInspectResultInfo	TempBendInspectResult;
		m_BendInspectResult = TempBendInspectResult;
	}


	SYSTEMTIME st;
	CString strTmp;
	GetLocalTime(&st);

	// 将已有的数据下移一行
	for(int i = m_nListDetaItemNum; i > 0; i--)
	{
		for(int j = MAX_COL_NUM; j>=0; j--)
		{
			m_strListDataDetal[i][j] = m_strListDataDetal[i - 1][j];
		}
	}


	// 0时间
	int nIndex = 0;
	strTmp.Format("%2d:%2d:%2d",st.wHour,st.wMinute,st.wSecond);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 1检测ID
	nIndex++;
	strTmp.Format(_T("%s"),m_BendInspectResult.m_strProductID);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 1检测工位位置
	nIndex++;
	strTmp.Format(_T("%s"),m_BendInspectResult.m_strProductSTATION);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 1本压工位位置
	nIndex++;
	strTmp.Format(_T("%s"),m_BendInspectResult.m_strInspectStation);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 2对位次数
	nIndex++;
	strTmp.Format(_T("%d"),m_apAlignerProcesInfo.nTimes);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 9Lx
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLx);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 10Ly
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLy);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 11Rx
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRx);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 12Ry
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRy);
	m_strListDataDetal[0][nIndex] = strTmp;


	// 13DisL
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceL);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 14DisR
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceR);
	m_strListDataDetal[0][nIndex] = strTmp;



	// 3MoveX
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dXMove);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 4MoveY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dYMove);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 5MoveD
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dDMove);
	m_strListDataDetal[0][nIndex] = strTmp;


	// 6ProductX
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dXOffset);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 7ProductY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dYOffset);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 8dDOffset
	nIndex++;
	strTmp.Format(_T("%.3f"),m_apAlignerProcesInfo.dDOffset);
	m_strListDataDetal[0][nIndex] = strTmp;

	//// 9Lx
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLx);
	//m_strListDataDetal[0][nIndex] = strTmp;

	//// 10Ly
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLy);
	//m_strListDataDetal[0][nIndex] = strTmp;

	//// 11Rx
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRx);
	//m_strListDataDetal[0][nIndex] = strTmp;

	//// 12Ry
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRy);
	//m_strListDataDetal[0][nIndex] = strTmp;


	//// 13DisL
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceL);
	//m_strListDataDetal[0][nIndex] = strTmp;

	//// 14DisR
	//nIndex++;
	//strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceR);
	//m_strListDataDetal[0][nIndex] = strTmp;

	// 15目标距离
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dTargetDistance);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 16对象距离
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dObjectDistance);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 17Lx-Rx
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceSubX);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 18Ly-Ry
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDistanceSubY);
	m_strListDataDetal[0][nIndex] = strTmp;

	//添加m_dLDeltaY;m_dRDeltaY;m_dDeltaY三项
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLDeltaY);
	m_strListDataDetal[0][nIndex] = strTmp;
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRDeltaY);
	m_strListDataDetal[0][nIndex] = strTmp;
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dDeltaY);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 19LCenterX
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLCenterx);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 20LCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dLCentery);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 21RCenterX
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRCenterx);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 22RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dRCentery);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 23RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadius[0]);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 24RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadius[1]);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 25RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadiusX[0]);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 26RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadiusX[1]);
	m_strListDataDetal[0][nIndex] = strTmp;

	// 27RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadiusY[0]);
	m_strListDataDetal[0][nIndex] = strTmp;


	// 28RCenterY
	nIndex++;
	strTmp.Format(_T("%.3f"),m_BendInspectResult.m_dCircleRadiusY[1]);
	m_strListDataDetal[0][nIndex] = strTmp;


	m_nListDetaItemNum++ ;
	if(m_nListDetaItemNum > m_nRecordNumMax)
	{
		ClearListDetalAllItem();
		m_ListBendDetal.InsertItem(m_nListDetaItemNum,_T(""));
	}
	else
	{
		m_ListBendDetal.InsertItem(m_nListDetaItemNum,_T(""));
	}


	// 刷新整个表格
	for(int i = 0; i < m_nListDetaItemNum; i++)
	{
		for(int j = 0; j<MAX_COL_NUM; j++)
		{
			m_ListBendDetal.SetItemText(i,j,m_strListDataDetal[i][j]);
		}
	}

	// 第一行高亮
	if(m_nListDetaItemNum > 0)
	{
		m_ListBendDetal.SetFocus();
		m_ListBendDetal.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	Invalidate(TRUE);
}

void CDlgVisionRunBendInspectInfo::ClearListDetalAllItem()
{
	m_ListBendDetal.DeleteAllItems();
	m_nListDetaItemNum = 0;
}

// BOOL CDlgVisionRunBendInspectInfo::OnInitDialog()
// {
// 	CDialog::OnInitDialog();
// 
// 	// TODO:  在此添加额外的初始化
// 
// 	CRect rcListRectSimple;
// 	m_ListBendSimple.GetClientRect(rcListRectSimple);
// 	m_ListBendSimple.ClientToScreen(rcListRectSimple);
// 	ScreenToClient(rcListRectSimple);
// 
// 
// 	// 折弯检测
// 	m_ListBendSimple.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(1, _T("基准"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(2, _T("最小"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(3, _T("最大"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(4, _T("左边"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(5, _T("右边"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendSimple.InsertColumn(6, _T("其它"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 
// 	m_ListBendSimple.InsertItem(0, _T("X差值(L)"));
// 	m_ListBendSimple.InsertItem(1, _T("Y差值(L)"));
// 	m_ListBendSimple.InsertItem(2, _T("距离(L)"));
// 	m_ListBendSimple.InsertItem(3, _T("X差值(R)"));
// 	m_ListBendSimple.InsertItem(4, _T("Y差值(R)"));
// 	m_ListBendSimple.InsertItem(5, _T("距离(R)"));
// 	m_ListBendSimple.InsertItem(6, _T("目标距离"));
// 	m_ListBendSimple.InsertItem(7, _T("对象距离"));
// 	m_ListBendSimple.InsertItem(8, _T("X向偏差"));
// 	m_ListBendSimple.InsertItem(9, _T("Y向差值"));
// 
// 	m_nListSimpleItemNum = 10;
// 	m_ListBendSimple.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
// 
// 
//  	// 圆检测
// 	m_ListBendCircle.InsertColumn(0, _T("序号"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(1, _T("基准"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(2, _T("最小"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(3, _T("最大"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(4, _T("左边"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(5, _T("右边"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 	m_ListBendCircle.InsertColumn(6, _T("其它"), LVCFMT_CENTER, rcListRectSimple.Width()/7);
// 
// 	m_ListBendCircle.InsertItem(0, _T("半径"));
// 
// 	m_nListCircleItemNum = 1;
// 	m_ListBendCircle.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
// 
// 
// 
// 	// 折弯检测过程详细信息
// 	m_ListBendDetal.InsertColumn(0, _T("Time"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 时间
// 	m_ListBendDetal.InsertColumn(1, _T("ID"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 产品ID
// 	m_ListBendDetal.InsertColumn(2, _T("Times"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 对位次数
// 
// 	m_ListBendDetal.InsertColumn(3, _T("MoveX"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(4, _T("MoveY"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(5, _T("MoveD"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(6, _T("ProductX"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(7, _T("ProductY"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(8, _T("ProductD"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(9, _T("Lx"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(10, _T("Ly"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(11, _T("Rx"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(12, _T("Ry"),			LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(13, _T("Left_Dis"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(14, _T("Right_Dis"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(15, _T("Tar_Dis"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(16, _T("Obj_Dis"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(17, _T("X_Sub"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(18, _T("Y_Sub"),		LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 	m_ListBendDetal.InsertColumn(19, _T("LCenterX"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(20, _T("LCenterY"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(21, _T("RCenterX"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 	m_ListBendDetal.InsertColumn(22, _T("RCenterY"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
// 
// 
// 	m_ListBendDetal.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
// 	m_ListBendDetal.InsertItem(m_nListDetaItemNum,_T(""));
// 
// 	switch(m_nBendShowInfo)
// 	{
// 	case 0:
// 		{
// 			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);
// 
// 			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
// 		}
// 		break;
// 
// 	case 1:
// 		{
// 			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);
// 
// 			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
// 		}
// 		break;
// 
// 
// 	case 2:
// 		{
// 			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_SHOW);
// 
// 			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
// 		}
// 		break;
// 
// 	default:
// 		break;
// 
// 	}
// 
// 	return TRUE;  // return TRUE unless you set the focus to a control
// 	// 异常: OCX 属性页应返回 FALSE
// }

BOOL CDlgVisionRunBendInspectInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CRect rcListRectSimple;
	m_ListBendSimple.GetClientRect(rcListRectSimple);
	m_ListBendSimple.ClientToScreen(rcListRectSimple);
	ScreenToClient(rcListRectSimple);


	// 折弯检测
	m_ListBendSimple.InsertColumn(0, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_INDEX)/*_T("序号")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(1, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_BASE)/*_T("基准")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(2, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MIN)/*_T("最小")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(3, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MAX)/*_T("最大")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(4, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT)/*_T("左边")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(5, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT)/*_T("右边")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendSimple.InsertColumn(6, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_OTHER)/*_T("其它")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);

	m_ListBendSimple.InsertItem(0, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_X_LEFT)/*_T("X差值(L)")*/);
	m_ListBendSimple.InsertItem(1, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_Y_LEFT)/*_T("Y差值(L)")*/);
	m_ListBendSimple.InsertItem(2, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_DISTANCE_LEFT)/*_T("距离(L)")*/);
	m_ListBendSimple.InsertItem(3, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_X_RIGHT)/*_T("X差值(R)")*/);
	m_ListBendSimple.InsertItem(4, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_Y_RIGHT)/*_T("Y差值(R)")*/);
	m_ListBendSimple.InsertItem(5, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_DISTANCE_RIGHT)/*_T("距离(R)")*/);
	m_ListBendSimple.InsertItem(6, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_DISTANCE_TARGET)/*_T("目标距离")*/);
	m_ListBendSimple.InsertItem(7, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_DISTANCE_OBJECT)/*_T("对象距离")*/);
	m_ListBendSimple.InsertItem(8, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_X_SUB)/*_T("X向偏差")*/);
	m_ListBendSimple.InsertItem(9, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_Y_SUB)/*_T("Y向差值")*/);
	m_ListBendSimple.InsertItem(10,_T("DeltaY"));  //增加DeltaY用于查看两侧Y向的变化趋势

	m_nListSimpleItemNum = 11;
	m_ListBendSimple.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);



	// 圆检测
	m_ListBendCircle.InsertColumn(0, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_INDEX)/*_T("序号")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(1, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_BASE)/*_T("基准")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(2, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MIN)/*_T("最小")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(3, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MAX)/*_T("最大")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(4, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT)/*_T("左边")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(5, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT)/*_T("右边")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);
	m_ListBendCircle.InsertColumn(6, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_OTHER)/*_T("其它")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/7);

	m_ListBendCircle.InsertItem(0, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_CIRCLE_RADIUS)/* _T("半径")*/);
	///////////黄新宇
	m_ListBendCircle.InsertItem(1, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_CIRCLE_X_SUB)/*_T("X向偏差")*/);
	m_ListBendCircle.InsertItem(2, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_ITEM_CIRCLE_Y_SUB)/*_T("Y向差值")*/);
	m_nListCircleItemNum = 3;
	m_ListBendCircle.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);



	// 折弯检测过程详细信息
	int nColsIndex = 0;
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_TIME)/*_T("Time")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 时间
	nColsIndex++;  //1
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_ID)/*_T("ID")*/,				LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 产品ID	 IDS_BEND_INSPECT_LIST_COLUMN_STATION
	nColsIndex++;  //2
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_STATION)/*_T("ID")*/,				LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 产品工位	 IDS_BEND_INSPECT_LIST_COLUMN_STATION
	nColsIndex++;  //2
	m_ListBendDetal.InsertColumn(nColsIndex, _T("平台号2")/*GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_STATION2)*//*_T("ID")*/,				LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 本压工位	 IDS_BEND_INSPECT_LIST_COLUMN_STATION2
	nColsIndex++;  //2
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_TIMES)/*_T("Times")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);			// 对位次数
	nColsIndex++;  //3
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_X)/*_T("Lx")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //4
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_Y)/*_T("Ly")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //5
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_X)/*_T("Rx")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //6
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_Y)/*_T("Ry")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //7
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_DISTANCE)/*_T("Left_Dis")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //8
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_DISTANCE)/*_T("Right_Dis")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //9
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MOVE_X)/*_T("MoveX")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //10
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MOVE_Y)/*_T("MoveY")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //11
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_MOVE_D)/*_T("MoveD")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //12
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_PRODUCT_X)/*_T("ProductX")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //13
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_PRODUCT_Y)/*_T("ProductY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //14
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_PRODUCT_D)/*_T("ProductD")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);

	//m_ListBendDetal.InsertColumn(9, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_X)/*_T("Lx")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	//m_ListBendDetal.InsertColumn(10, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_Y)/*_T("Ly")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	//m_ListBendDetal.InsertColumn(11, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_X)/*_T("Rx")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);
	//m_ListBendDetal.InsertColumn(12, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_Y)/*_T("Ry")*/,			LVCFMT_CENTER, rcListRectSimple.Width()/5);

	//m_ListBendDetal.InsertColumn(13, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_DISTANCE)/*_T("Left_Dis")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	//m_ListBendDetal.InsertColumn(14, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_DISTANCE)/*_T("Right_Dis")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //15
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_TARGET_DISTANCE)/*_T("Tar_Dis")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //16
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_OBJECT_DISTANCE)/*_T("Obj_Dis")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //17
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_SUB_X)/*_T("X_Sub")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //18
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_SUB_Y)/*_T("Y_Sub")*/,		LVCFMT_CENTER, rcListRectSimple.Width()/5);
	
	//添加m_dLDeltaY;m_dRDeltaY;m_dDeltaY三项
	nColsIndex++;  //m_dLDeltaY
	m_ListBendDetal.InsertColumn(nColsIndex,_T("LDeltaY"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //m_dRDeltaY
	m_ListBendDetal.InsertColumn(nColsIndex,_T("RDeltaY"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //m_dDeltaY
	m_ListBendDetal.InsertColumn(nColsIndex,_T("DeltaY"),	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	
	nColsIndex++;  //19
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_CENTER_X)/*_T("LCenterX")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //20
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_LEFT_CENTER_Y)/*_T("LCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //21
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_CENTER_X)/*_T("RCenterX")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //22
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_RIGHT_CENTER_Y)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);


	nColsIndex++;  //23																					
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_LIEFT)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //24
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_RIGHT)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //25
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_LIEFT_X)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //26
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_RIGHT_X)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //27
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_LIEFT_Y)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);
	nColsIndex++;  //28
	m_ListBendDetal.InsertColumn(nColsIndex, GetSysInfoString(m_pParent->m_pParent->m_psaSysInfoStrings,IDS_BEND_INSPECT_LIST_COLUMN_CIRCLE_RIGHT_Y)/*_T("RCenterY")*/,	LVCFMT_CENTER, rcListRectSimple.Width()/5);

	m_ListBendDetal.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListBendDetal.InsertItem(m_nListDetaItemNum,_T(""));

	switch(m_nBendShowInfo)
	{
	case 0:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
		}
		break;

	case 1:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
		}
		break;

	case 2:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
		}
		break;

	default:
		break;

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgVisionRunBendInspectInfo::OnCustomdrawListBendSimple(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// 	// TODO: 在此添加控件通知处理程序代码
	// 	*pResult = 0;

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
 	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
 	{
 		int nItem=(int)pLVCD->nmcd.dwItemSpec;
 		int nSubitem= pLVCD->iSubItem;
 
 		// 默认白底黑字
 		COLORREF clrNewTextColor = RGB(0,0,0), clrNewBkColor = RGB(255,255,255);
 
 		if (!m_bSimpleOK[nItem][nSubitem] ) // 如果NG则标红
 		{
 			clrNewTextColor = RGB(0,0,0);		
 			clrNewBkColor = RGB(255,0,0);		
 		}
 		pLVCD->clrText = clrNewTextColor;
 		pLVCD->clrTextBk = clrNewBkColor;
 
 		*pResult = CDRF_DODEFAULT;
 	}
}


void CDlgVisionRunBendInspectInfo::OnDblclkListBendSimple(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 	// TODO: 在此添加控件通知处理程序代码
	// 	*pResult = 0;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;

	int nItem = pEditCtrl->iItem;
	int nSubItem = pEditCtrl->iSubItem;
	CRect EditRect;
	CString strTmp;

	//禁止修改
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bMainframeSetBendParam == FALSE)
	{
		return;
	}


	// 只允许更改基准
	if(nSubItem>3 || nSubItem==0)
	{
		return;
	}


	if (pEditCtrl->iItem==-1)//点击到非工作区
	{
		if(m_bSimpleStanderShow)
		{
			m_editBendInsBase.ShowWindow(SW_HIDE);
			m_bSimpleStanderShow = FALSE;
		}
	}
	else
	{
		if(!m_bSimpleStanderShow)
		{
			strTmp = m_ListBendSimple.GetItemText(nItem,nSubItem);
			m_ListBendSimple.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
			m_ListBendSimple.ClientToScreen(EditRect);
			ScreenToClient(EditRect);

			m_editBendInsBase.MoveWindow(EditRect);
			m_editBendInsBase.SetWindowText(strTmp);
			m_editBendInsBase.ShowWindow(SW_SHOW);
			m_editBendInsBase.SetFocus();

			m_LastSimpleItem = nItem;
			m_LastSimpleSubItem = nSubItem;

			m_bSimpleStanderShow = TRUE;
		}
	}

}


void CDlgVisionRunBendInspectInfo::OnClickListBendSimple(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// 	// TODO: 在此添加控件通知处理程序代码
	// 	*pResult = 0;


	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	int nItem = pEditCtrl->iItem;
	int nSubItem = pEditCtrl->iSubItem;
	CRect EditRect;
	CString strTmp;

	//点击到了其他地方如果此时已经处于编辑状态，则更新数据。
	if(m_bSimpleStanderShow)
	{
		CString strOld  = m_ListBendSimple.GetItemText(m_LastSimpleItem,m_LastSimpleSubItem);
		
		m_editBendInsBase.GetWindowText(strTmp);
		m_ListBendSimple.SetItemText(m_LastSimpleItem,m_LastSimpleSubItem,strTmp);


		m_editBendInsBase.ShowWindow(SW_HIDE);
		m_bSimpleStanderShow = FALSE;

		UpdateBendBaseInfo(TRUE);

		for(int i = 0;i<m_nListSimpleItemNum;i++)
		{
			if( atof(m_strListDataSimple[i][2]) > atof(m_strListDataSimple[i][3]))
			{
				//10-30测试修改最小值
				//m_ListBendSimple.SetItemText(i,2,m_strListDataSimple[i][3]);

				UpdateBendBaseInfo(TRUE);
			}
		}

		if (strTmp != strOld)
		{
			CString strLog,strSubName;
			if (m_LastSimpleSubItem == 1)	strSubName = _T("基准值");
			if (m_LastSimpleSubItem == 2)	strSubName = _T("最小值");
			if (m_LastSimpleSubItem == 3)	strSubName = _T("最大值");
			strLog.Format(_T("主界面修改工位%d折弯检测参数[%s%s]由%s到%s"),m_pVisionASM->GetSysPlatformInfoEx().m_nPlatformIndex+1,
				m_ListBendSimple.GetItemText(m_LastSimpleItem,0),strSubName,strOld,strTmp);
			g_opBendLogFile.SaveLogText(strLog,3);
		}

//		UpdateBendBaseInfo(TRUE);
	}

}
void CDlgVisionRunBendInspectInfo::OnKillfocusEditBendShow()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_BEND_SHOW, strVal);

		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_BEND_SHOW, m_strBendInsBase);
		}
		else
		{
			m_strBendInsBase.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_BEND_SHOW, m_strBendInsBase);
		}
	}
}

void CDlgVisionRunBendInspectInfo::OnSetfocusEditBendShow()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_BEND_SHOW)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_BEND_SHOW, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_BEND_SHOW, strOutput);
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditBendShow();

	}	

}


void CDlgVisionRunBendInspectInfo::OnCustomdrawListBendCircle(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		int nItem=(int)pLVCD->nmcd.dwItemSpec;
		int nSubitem= pLVCD->iSubItem;

		// 默认白底黑字
		COLORREF clrNewTextColor = RGB(0,0,0), clrNewBkColor = RGB(255,255,255);

		if (!m_bCircleOK[nItem][nSubitem] ) // 如果NG则标红
		{
			clrNewTextColor = RGB(0,0,0);		
			clrNewBkColor = RGB(255,0,0);		
		}
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}

}

void CDlgVisionRunBendInspectInfo::OnDblclkListBendCircle(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;

	int nItem = pEditCtrl->iItem;
	int nSubItem = pEditCtrl->iSubItem;
	CRect EditRect;
	CString strTmp;

	//禁止修改
	if (m_pParent->m_pParent->m_SysOptionConfig.m_bMainframeSetBendParam == FALSE)
	{
		return;
	}

	// 只允许更改基准
	if(nSubItem>3 || nSubItem==0)
	{
		return ;
	}


	if (pEditCtrl->iItem==-1)//点击到非工作区
	{
		if(m_bCircleStanderShow)
		{
			m_editCircleInsBase.ShowWindow(SW_HIDE);
			m_bCircleStanderShow = FALSE;
		}
	}
	else
	{
		if(!m_bCircleStanderShow)
		{
			strTmp = m_ListBendCircle.GetItemText(nItem,nSubItem);
			m_ListBendCircle.GetSubItemRect(nItem, nSubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
			m_ListBendCircle.ClientToScreen(EditRect);
			ScreenToClient(EditRect);

			m_editCircleInsBase.MoveWindow(EditRect);
			m_editCircleInsBase.SetWindowText(strTmp);
			m_editCircleInsBase.ShowWindow(SW_SHOW);
			m_editCircleInsBase.SetFocus();

			m_LastCircleItem = nItem;
			m_LastCircleSubItem = nSubItem;

			m_bCircleStanderShow = TRUE;
		}
	}
}

void CDlgVisionRunBendInspectInfo::OnClickListBendCircle(NMHDR *pNMHDR, LRESULT *pResult)
{
// 	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	int nItem = pEditCtrl->iItem;
	int nSubItem = pEditCtrl->iSubItem;
	CRect EditRect;
	CString strTmp;

	//点击到了其他地方如果此时已经处于编辑状态，则更新数据。
	if(m_bCircleStanderShow)
	{
		CString strOld  = m_ListBendCircle.GetItemText(m_LastCircleItem,m_LastCircleSubItem);

		m_editCircleInsBase.GetWindowText(strTmp);
		m_ListBendCircle.SetItemText(m_LastCircleItem,m_LastCircleSubItem,strTmp);


		m_editCircleInsBase.ShowWindow(SW_HIDE);
		m_bCircleStanderShow = FALSE;

		UpdateCircleBaseInfo(TRUE);

		for(int i = 0;i<m_nListCircleItemNum;i++)
		{
			if( atof(m_strListDataCircle[i][2]) > atof(m_strListDataCircle[i][3]))
			{
				//m_ListBendCircle.SetItemText(i,2,m_strListDataCircle[i][3]);

				UpdateCircleBaseInfo(TRUE);
			}
		}

		if (strTmp != strOld)
		{
			CString strLog,strSubName;
			if (m_LastCircleSubItem == 1)	strSubName = _T("基准值");
			if (m_LastCircleSubItem == 2)	strSubName = _T("最小值");
			if (m_LastCircleSubItem == 3)	strSubName = _T("最大值");
			strLog.Format(_T("主界面修改工位%d圆检测参数[%s%s]由%s到%s"),m_pVisionASM->GetSysPlatformInfoEx().m_nPlatformIndex+1,
				m_ListBendCircle.GetItemText(m_LastCircleItem,0),strSubName,strOld,strTmp);
			g_opBendLogFile.SaveLogText(strLog,3);
		}

//		UpdateCircleBaseInfo(TRUE);
	}

}


void CDlgVisionRunBendInspectInfo::OnKillfocusEditCircleShow()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_CIRCLE_SHOW, strVal);

		if (!IsStringNumerical(strVal))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_SHOW, m_strCircleInsBase);
		}
		else
		{
			m_strCircleInsBase.Format(_T("%.3f"), atof(strVal));
			SetDlgItemText(IDC_EDIT_CIRCLE_SHOW, m_strCircleInsBase);

		}
	}

}


void CDlgVisionRunBendInspectInfo::OnSetfocusEditCircleShow()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_CIRCLE_SHOW)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_CIRCLE_SHOW, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_CIRCLE_SHOW, strOutput);
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditCircleShow();

	}	
}

void CDlgVisionRunBendInspectInfo::OnKillfocusEditMaxRecordNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bKillFocusEnabled)
	{
		CString strVal;
		GetDlgItemText(IDC_EDIT_MAX_RECORD_NUM, strVal);
		int nOldValue = m_nRecordNumMax;

		// 判断数据是否合法
		if(atoi(strVal) < 0 ||atoi(strVal) >= MAX_ITEM_NUM)
		{
			strVal.Format(_T("%d"),m_nRecordNumMax);
			SetDlgItemText(IDC_EDIT_MAX_RECORD_NUM, strVal);
		}
		UpdateData(TRUE);

		if (nOldValue != m_nRecordNumMax)
		{
			CString strLog;
			strLog.Format(_T("主界面修改工位%d折弯检测参数[记录上限]由%d到%d"),m_pVisionASM->GetSysPlatformInfoEx().m_nPlatformIndex+1,nOldValue,m_nRecordNumMax);
			g_opBendLogFile.SaveLogText(strLog,3);
		}
	}

}


void CDlgVisionRunBendInspectInfo::OnSetfocusEditMaxRecordNum()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_bSysKeyboardEnabled)
	{
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->GetWindowRect(&rcWnd);
		GetDlgItemText(IDC_EDIT_MAX_RECORD_NUM, strInput);

		if (TRUE == m_ctrlKeyboard.ShowNumeralKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(IDC_EDIT_MAX_RECORD_NUM, strOutput);
		}

		m_bKillFocusEnabled = TRUE;
		OnKillfocusEditMaxRecordNum();

	}	
}

void CDlgVisionRunBendInspectInfo::OnBnClickedBtnDetalClear()
{
	// TODO: 在此添加控件通知处理程序代码

	ClearListDetalAllItem();
}



void CDlgVisionRunBendInspectInfo::OnBnClickedRadioBendInspect()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);


	switch(m_nBendShowInfo)
	{
	case 0:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
 			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
 			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
 			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
		}
		break;

	case 1:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_HIDE);
		}
		break;


	case 2:
		{
			GetDlgItem(IDC_LIST_BEND_SIMPLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_CIRCLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_LIST_BEND_DETAL)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_BTN_DETAL_CLEAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_MAX_RECORD_NUM)->ShowWindow(SW_SHOW);
		}
		break;

	default:
		break;

	}
}
