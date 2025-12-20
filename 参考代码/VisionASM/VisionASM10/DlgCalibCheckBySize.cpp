
#include "stdafx.h"
#include "DlgCalibCheckBySize.h"
#include "afxdialogex.h"
#include <vector>
#include<algorithm>
#include "DlgVisionRunAlignInfo.h"
#include "vsXYDVisionAlign.h"
// CDlgCalibCheckBySize 对话框

IMPLEMENT_DYNAMIC(CDlgCalibCheckBySize, CDialogEx)

CDlgCalibCheckBySize::CDlgCalibCheckBySize(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibCheckBySize::IDD, pParent)
	, m_nExProductIndex(0)
{
	m_pVisionASM = NULL;
	m_pParent = NULL;
	m_nGetAxisOption = 1;
	m_nTypeIndex = 0;
	m_bSysKeyboardEnabled = FALSE;
	m_bKillFocusEnabled = FALSE;
	m_vImagePosAndPlatformPos.resize(4);
	m_nSeachNums = 4;
	m_nDistanceNums = 4;
	m_vbSearchSuss.resize(m_nSeachNums,FALSE);
	m_nCurSearchIndex = 0;
	m_nOldSearchIndex = 0;
	m_bIsReadOnly = FALSE;
}

CDlgCalibCheckBySize::~CDlgCalibCheckBySize()
{
	m_pVisionASM = NULL;
	m_pParent = NULL;
	
}

void CDlgCalibCheckBySize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT_SIZE_SHOW, m_ListObjecSize);
	DDX_Control(pDX, IDC_LIST_OBJECT_SIZE_STATISTICS, m_ListObjectSizeStatistics);
	DDX_Control(pDX, IDC_LIST_IMAGEPOS, m_ListImagePos);
	DDX_Control(pDX, IDC_LIST_IMAGE_POS_STATISTICS, m_ListImagePosStatistics);
	DDX_Radio(pDX, IDC_RADIO_GET_AXIS_BY_COMM, m_nGetAxisOption);
	DDX_Control(pDX, IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, m_CComboBox_EX_Product);
	DDX_CBIndex(pDX, IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, m_nExProductIndex);
}


BEGIN_MESSAGE_MAP(CDlgCalibCheckBySize, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OBJECT_SEARCH_POS1, &CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos1)
	ON_BN_CLICKED(IDC_BTN_OBJECT_SEARCH_POS2, &CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_OBJECT_SIZE_SHOW, &CDlgCalibCheckBySize::OnNMRClickListObjectSizeShow)
	ON_COMMAND(ID_LIST_DEL_ITEM, &CDlgCalibCheckBySize::OnListDelItem)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_IMAGEPOS, &CDlgCalibCheckBySize::OnNMRClickListImagepos)
	ON_COMMAND(ID_LIST_DEL_ITEM2, &CDlgCalibCheckBySize::OnListDelItem2)
	ON_COMMAND(ID_LIST_CLEAR_ALL_ITEM2, &CDlgCalibCheckBySize::OnListClearAllItem2)
	ON_COMMAND(ID_LIST_CLEAR_ALL_ITEM, &CDlgCalibCheckBySize::OnListClearAllItem)
	ON_BN_CLICKED(IDC_RADIO_GET_AXIS_BY_COMM, &CDlgCalibCheckBySize::OnBnClickedRadioGetAxisByComm)
	ON_BN_CLICKED(IDC_RADIO_GET_AXIS_BY_INPUT, &CDlgCalibCheckBySize::OnBnClickedRadioGetAxisByInput)
	ON_BN_CLICKED(IDC_BTN_OBJECT_SEARCH_POS3, &CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos3)
	ON_BN_CLICKED(IDC_BTN_OBJECT_SEARCH_POS4, &CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos4)
	ON_BN_CLICKED(IDC_BTN_OBJECT_SIZE_CALC, &CDlgCalibCheckBySize::OnBnClickedBtnObjectSizeCalc)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_X1, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_Y1, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_D1, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD1)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_X2, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_Y2, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY2)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_D2, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD2)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_X3, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_Y3, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY3)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_D3, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD3)

	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_X4, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_Y4, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY4)
	ON_EN_SETFOCUS(IDC_EDIT_VAL_POS_AXIS_D4, &CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD4)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE, &CDlgCalibCheckBySize::OnCbnSelchangeComboExproductCheckBySize)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_X1, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_Y1, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_D1, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD1)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_X2, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_Y2, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_D2, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD2)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_X3, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_Y3, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_D3, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD3)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_X4, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_Y4, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY4)
	ON_EN_KILLFOCUS(IDC_EDIT_VAL_POS_AXIS_D4, &CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD4)
END_MESSAGE_MAP()


// CDlgCalibCheckBySize 消息处理程序


BOOL CDlgCalibCheckBySize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化控件显示

	UpdateDialogLanguage();
	OnBnClickedRadioGetAxisByComm();
	if (m_pVisionASM !=NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		int nPos =  m_pVisionASM->GetPosNum();
		if (nPos == 2)
		{
			m_nSeachNums =2;
		}
		else if (nPos == 4 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =2;
		}
		else if (nPos == 4 && platformInfo.m_bTargetObjectCamSeparate == FALSE)
		{
			m_nSeachNums =4;
		}
		else if (nPos == 3 && platformInfo.m_bTargetObjectCamSeparate == FALSE)
		{
			m_nSeachNums =3;
		}
		else if (nPos == 6 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =3;
		}
		else if (nPos == 8 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			m_nSeachNums =4;
		}


		// 单平台扩展
		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			GetDlgItem(IDC_STATIC_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_SHOW);

		}
		else
		{
			GetDlgItem(IDC_STATIC_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_EXPRODUCT_CHECK_BY_SIZE)->ShowWindow(SW_HIDE);
		}

		// 修复无协议时 获取轴卡顿问题
		if (!m_pVisionASM->m_XYDCommProtocol.IsPlcProtocolType())
		{
			GetDlgItem(IDC_RADIO_GET_AXIS_BY_COMM)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_GET_AXIS_BY_COMM)->ShowWindow(SW_HIDE);
		}


		m_CComboBox_EX_Product.ResetContent();
		CString strNum = _T("1");
		for (int it = 0;it < platformInfo.m_nMultiCalibExtensionMaxNum + 1;it++)
		{
			strNum.Format(_T("%d"),it+1);
			m_CComboBox_EX_Product.InsertString(it,(LPCTSTR)strNum);
		}
		m_nExProductIndex = 0;
		m_CComboBox_EX_Product.SetCurSel(0);
		
		SysPlatformInfo sysPlatformInfo;
		m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

		CVisionASMConfig *vsConfig;
		m_pVisionASM->GetVSconfig(vsConfig);
		CString StrTmp = _T("");
		if (m_nTypeIndex==0)
		{
			CString strTmp = _T("对象尺寸验证");
			if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
			{
				strTmp = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_OBJECT_SIZE_CHECK);
			}
			SetWindowText(strTmp);

			if ((NULL !=vsConfig))
			{
				if (sysPlatformInfo.m_bProductFaChd)
				{
					int nCurProductIndexFa;
					int nCurProductIndexCh;
					vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
					StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_大品种%03d_小品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
						nCurProductIndexFa+1,nCurProductIndexCh+1);
				}
				else
				{
					StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
						vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
				}

			}
		}
		else if(m_nTypeIndex==1)
		{
			CString strTmp = _T("目标尺寸验证");
			if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
			{
				strTmp = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_TARGET_SIZE_CHECK);
			}
			SetWindowText(strTmp);

			if ((NULL !=vsConfig))
			{
				if (sysPlatformInfo.m_bProductFaChd)
				{
					int nCurProductIndexFa;
					int nCurProductIndexCh;
					vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
					StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_大品种%03d_小品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
						nCurProductIndexFa+1,nCurProductIndexCh+1);
				}
				else
				{
					StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
						vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
				}

			}
		}


		if (sysPlatformInfo.m_bEnableMultiCalibExtension)
		{
			CString strTemp = _T("");
			strTemp.Format(_T("当前扩展序号: %3d;\n"),0);
			m_pVisionASM->VisionAlignLogRecord(StrTmp,strTemp,TRUE);
		}

	}
	
	m_vbSearchSuss.resize(m_nSeachNums,FALSE);

	// 初始化 按钮和轴编辑框
	InitDialogBtnAndEditControl();
	// 初始化list控件
	InitDlgListControl(m_nSeachNums);
	InitAxisData();
	InitAxisTextInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	
}



void CDlgCalibCheckBySize::InitDialogBtnAndEditControl()
{

	// 根据关联，调整按钮显示；
	if (m_nTypeIndex==1)
	{

		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		if (platformInfo.m_bTargetObjectCamSeparate)
		{
			for (int i=0;i<m_nSeachNums;i++)
			{
				// 
				CString strTmp = _T("");

				CString strTmp2 = _T("目标%d拍照");

				if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
				{
					strTmp2 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_TARGET_SNAP_POS);
				}

				strTmp.Format(strTmp2,m_nSeachNums+i+1);
				// 各按钮 显示
				GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+i*5)->SetWindowText(strTmp);
			}
		}
		else
		{
			for (int i=0;i<m_nSeachNums;i++)
			{
				// 
				CString strTmp = _T("");

				CString strTmp2 = _T("目标%d拍照");

				if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
				{
					strTmp2 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_TARGET_SNAP_POS);
				}

				strTmp.Format(strTmp2,i+1);
				// 各按钮 显示
				GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+i*5)->SetWindowText(strTmp);
			}
		}




	}


	//根据搜索位置数隐藏控件
	switch(m_nSeachNums)
	{
	case 2:
		{
			// 隐藏搜索按钮
			for (int i=0;i<10;i++)
			{
				if (NULL !=GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS3+i))
				{
					GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS3+i)->ShowWindow(SW_HIDE);
				}
			}



			


		};
		break;
	case 3:
		{
			// 隐藏搜索按钮
			for (int i=0;i<5;i++)
			{
				if (NULL !=GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS4+i))
				{
					GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS4+i)->ShowWindow(SW_HIDE);
				}

				//// 根据标定关联，调整按钮显示；
				//if (m_nTypeIndex==1)
				//{
				//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1)->SetWindowText(_T("目标位置4拍照"));
				//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS2)->SetWindowText(_T("目标位置5拍照"));
				//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS3)->SetWindowText(_T("目标位置6拍照"));
				//}
			}
		};
		break;
	case 4:
		{
			// 默认控件显示控件

			// 根据标定关联，调整按钮显示；
			//if (m_nTypeIndex==1)
			//{
			//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1)->SetWindowText(_T("目标位置5拍照"));
			//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS2)->SetWindowText(_T("目标位置6拍照"));
			//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS3)->SetWindowText(_T("目标位置7拍照"));
			//	GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS4)->SetWindowText(_T("目标位置8拍照"));
			//}
		};
		break;
	default:
		{
			// 默认
		};
		break;
	}
}

// 尺寸列表初始化
void CDlgCalibCheckBySize::InitDlgListControl(int nCols)
{
	if (nCols<1)
	{
		return;
	}

	//GetDlgItem(IDC_LIST_OBJECT_SIZE_SHOW)->GetWindowRect(m_RectDisPlay);
	//ScreenToClient(m_RectDisPlay);
	CRect rcListRect;
	GetDlgItem(IDC_LIST_OBJECT_SIZE_SHOW)->GetWindowRect(rcListRect);
	ScreenToClient(rcListRect);		  
	m_ListObjecSize.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListObjectSizeStatistics.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListImagePos.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListImagePosStatistics.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	m_ListObjecSize.DeleteAllItems();
	m_ListObjectSizeStatistics.DeleteAllItems();
	m_ListImagePos.DeleteAllItems();
	m_ListImagePosStatistics.DeleteAllItems();

	CString strTmp1 = _T("拍照次数");
	CString strTmp2 = _T("图像坐标X");
	CString strTmp3 = _T("图像坐标Y");
	CString strTmp4 = _T("平台坐标X");
	CString strTmp5 = _T("平台坐标Y");

	CString strTmp6 = _T("统计");
	CString strTmp7 = _T("最大值");
	CString strTmp8 = _T("最小值");
	CString strTmp9 = _T("极差");
	CString strTmp10 = _T("验证次数");

	if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		strTmp1 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT1);
		strTmp2 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT2);
		strTmp3 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT3);
		strTmp4 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT4);
		strTmp5 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT5);
		strTmp6 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT6);
		strTmp7 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT7);
		strTmp8 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT8);
		strTmp9 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT9);
		strTmp10 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_SIZE_CHECK_TEXT10);

	}

	// 初始化图像坐标平台坐标列表
	int nRow =0;
	int k = nCols;
	k=5;
	m_ListImagePos.InsertColumn(0,strTmp1, LVCFMT_CENTER, rcListRect.Width()/k);
	m_ListImagePos.InsertColumn(1, strTmp2, LVCFMT_CENTER,  rcListRect.Width()/k);
	m_ListImagePos.InsertColumn(2,strTmp3, LVCFMT_CENTER,  rcListRect.Width()/k);
	m_ListImagePos.InsertColumn(3,strTmp4, LVCFMT_CENTER, rcListRect.Width()/k);
	m_ListImagePos.InsertColumn(4, strTmp5, LVCFMT_CENTER, rcListRect.Width()/k);

	m_ListImagePosStatistics.InsertColumn(0,strTmp6, LVCFMT_CENTER, rcListRect.Width()/k);
	m_ListImagePosStatistics.InsertColumn(1, strTmp2, LVCFMT_CENTER,  rcListRect.Width()/k);
	m_ListImagePosStatistics.InsertColumn(2,strTmp3, LVCFMT_CENTER,  rcListRect.Width()/k);
	m_ListImagePosStatistics.InsertColumn(3, strTmp4, LVCFMT_CENTER, rcListRect.Width()/k);
	m_ListImagePosStatistics.InsertColumn(4, strTmp5, LVCFMT_CENTER, rcListRect.Width()/k);

	nRow = m_ListImagePosStatistics.InsertItem(0, strTmp7);
	nRow = m_ListImagePosStatistics.InsertItem(1, strTmp8);
	nRow = m_ListImagePosStatistics.InsertItem(2, strTmp9);


	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		switch (platformInfo.m_nPositionNum)
		{
		case 2:
			{
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_OBJECTSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_TARGETSIZE0);
			}
			break;
		case 3:
			{
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE0);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE1);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_OBJECTSIZE2);

				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE1);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN3P3V3_TARGETSIZE2);
			}
			break;
		case 4:
			{
				if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
				{
					m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P2V2_OBJECTSIZE0);
					m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P2V2_TARGETSIZE0);
				}
				else
				{
					m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE0);
					m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE1);
					m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE2);
					m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_OBJECTSIZE3);

					m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE0);
					m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE1);
					m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE2);
					m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN4P4V4_TARGETSIZE3);

				}
			}

			break;
		case 6:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{

				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE0);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE1);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE2);

				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE1);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE2);

			}
			else
			{
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE0);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE1);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_OBJECTSIZE2);

				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE1);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN6P3V3_TARGETSIZE2);

			}
			break;
		case 8:
			if (TRUE == platformInfo.m_bTargetObjectCamSeparate)
			{

				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE0);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[1]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE1);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[2]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE2);
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[3]  = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_OBJECTSIZE3);

				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[1] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE1);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[2] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE2);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[3] =GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN8P4V4_TARGETSIZE3);

			}
			else
			{

			}
			break;
		default:
			{
				m_AlignerObjectSizeInfo.m_vstrObjectSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_OBJECTSIZE0);
				m_AlignerTargetSizeInfo.m_vstrTargetSizeName[0] = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_STR_STATIC_ALN2P2V2_TARGETSIZE0);
			}
			break;
		}

	}





	// 初始化 尺寸检查列表
	switch(nCols)
	{
	case 2:
		{
			k = 2;
			m_nDistanceNums = 1;
			m_ListObjecSize.InsertColumn(0,strTmp10, LVCFMT_CENTER, rcListRect.Width()/k);
			m_ListObjectSizeStatistics.InsertColumn(0,strTmp6, LVCFMT_CENTER, rcListRect.Width()/k);

			if (m_pVisionASM!=NULL)
			{
				
				// 标定 or 关联
				if (m_nTypeIndex==0)
				{
					AlignerObjectSizeInfo TmpAlignerObjectSizeInfo ;
					TmpAlignerObjectSizeInfo = m_AlignerObjectSizeInfo;
					m_ListObjecSize.InsertColumn(1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(0), LVCFMT_CENTER, rcListRect.Width()/k);
					m_ListObjectSizeStatistics.InsertColumn(1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(0), LVCFMT_CENTER, rcListRect.Width()/k);
				} 
				else
				{
					AlignerTargetSizeInfo  TmpAlignerObjectSizeInfo;
					TmpAlignerObjectSizeInfo = m_AlignerTargetSizeInfo;
					m_ListObjecSize.InsertColumn(1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(0), LVCFMT_CENTER, rcListRect.Width()/k);
					m_ListObjectSizeStatistics.InsertColumn(1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(0), LVCFMT_CENTER, rcListRect.Width()/k);
				}

			}

			nRow = m_ListObjectSizeStatistics.InsertItem(0, strTmp7);
			nRow = m_ListObjectSizeStatistics.InsertItem(1, strTmp8);
			nRow = m_ListObjectSizeStatistics.InsertItem(2, strTmp9);
		}
		break;
	case 3:
		{
			k = 4;
			m_nDistanceNums=3;
			m_ListObjecSize.InsertColumn(0,strTmp10, LVCFMT_CENTER, rcListRect.Width()/k);
			m_ListObjectSizeStatistics.InsertColumn(0,strTmp6, LVCFMT_CENTER, rcListRect.Width()/k);
			nRow = m_ListObjectSizeStatistics.InsertItem(0, strTmp7);
			nRow = m_ListObjectSizeStatistics.InsertItem(1, strTmp8);
			nRow = m_ListObjectSizeStatistics.InsertItem(2, strTmp9);

			if (m_pVisionASM!=NULL)
			{


				// 标定 or 关联
				if (m_nTypeIndex==0)
				{
					AlignerObjectSizeInfo TmpAlignerObjectSizeInfo ;
					TmpAlignerObjectSizeInfo = m_AlignerObjectSizeInfo;

					for (int it=0;it<3 && it<TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.size();it++)
					{
						m_ListObjecSize.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
						m_ListObjectSizeStatistics.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
					}
				} 
				else
				{
					AlignerTargetSizeInfo  TmpAlignerObjectSizeInfo;
					TmpAlignerObjectSizeInfo = m_AlignerTargetSizeInfo;
					for (int it=0;it<3 && it<TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.size();it++)
					{
						m_ListObjecSize.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
						m_ListObjectSizeStatistics.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
					}
				}

			}
		}
		break;
	case 4:
		{
			k = 5;
			m_nDistanceNums = 4;
			m_ListObjecSize.InsertColumn(0,strTmp10, LVCFMT_CENTER, rcListRect.Width()/k);
			m_ListObjectSizeStatistics.InsertColumn(0,strTmp6, LVCFMT_CENTER, rcListRect.Width()/k);
			nRow = m_ListObjectSizeStatistics.InsertItem(0, strTmp7);
			nRow = m_ListObjectSizeStatistics.InsertItem(1, strTmp8);
			nRow = m_ListObjectSizeStatistics.InsertItem(2, strTmp9);

			if (m_pVisionASM!=NULL)
			{
				// 标定 or 关联
				if (m_nTypeIndex==0)
				{
					AlignerObjectSizeInfo TmpAlignerObjectSizeInfo ;
					//m_pVisionASM->GetAlignerObjectSizeInfo(TmpAlignerObjectSizeInfo);
					TmpAlignerObjectSizeInfo = m_AlignerObjectSizeInfo;

					for (int it=0;it<4 && it<TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.size();it++)
					{
						m_ListObjecSize.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
						m_ListObjectSizeStatistics.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrObjectSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
					}
				} 
				else
				{
					AlignerTargetSizeInfo  TmpAlignerObjectSizeInfo;
					//m_pVisionASM->GetAlignerTargetSizeInfo(TmpAlignerObjectSizeInfo);
					TmpAlignerObjectSizeInfo = m_AlignerTargetSizeInfo;
					for (int it=0;it<4 && it<TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.size();it++)
					{
						m_ListObjecSize.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
						m_ListObjectSizeStatistics.InsertColumn(it+1,TmpAlignerObjectSizeInfo.m_vstrTargetSizeName.at(it), LVCFMT_CENTER, rcListRect.Width()/k);
					}
				}

			}

		}
		break;
	}

}


void CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos1()
{
	m_nCurSearchIndex =0;
	ExeSearch(0);
}


void CDlgCalibCheckBySize::ClearListCtrl(CListCtrl * m_pList)
{
	if (NULL == m_pList)
	{
		return;
	}

	for (int i = 0;i<m_pList->GetItemCount();i++)
	{
		for (int j=1;j<m_pList->GetHeaderCtrl()->GetItemCount();j++)
		{
			m_pList->SetItemText(i,j,_T(""));
		}
	}

}
void CDlgCalibCheckBySize::UpdateImagePosDataList(const int nPos,const std::vector<double> vData)
{


	if (m_nCurSearchIndex !=m_nOldSearchIndex) // 如果点击位置与上一次位置不一致  清除图像列表
	{
		m_ListImagePos.DeleteAllItems();
	}


	// 更新图像坐标列表和统计列表
	CString tmpStr=_T("");

	int nRow = m_ListImagePos.GetItemCount();
	tmpStr.Format(_T("%03d"),nRow+1);
	m_ListImagePos.InsertItem(nRow,tmpStr);
	// 传入数据异常 返回
	if (vData.size() != m_ListImagePos.GetHeaderCtrl()->GetItemCount()-1)
	{
		return;
	}

	// 赋值
	for (int j = 1;j<m_ListImagePos.GetHeaderCtrl()->GetItemCount()  ;j++)
	{
		tmpStr.Format(_T("%.2f"),vData.at(j-1));
		m_ListImagePos.SetItemText(nRow,j,tmpStr);
	}

	// 更新图像坐标与
	UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);

	//搜索成功后 更新上一次搜索位置序号
	m_nOldSearchIndex = m_nCurSearchIndex;
}
void CDlgCalibCheckBySize::UpdateStatisticsList(CListCtrl * m_pList,CListCtrl * m_pListStatistics)
{
	// 空指针返回
	if (NULL == m_pList || NULL == m_pListStatistics )
	{
		return;
	}
	// 无表头 返回
	if (NULL == m_pList->GetHeaderCtrl() || NULL == m_pListStatistics->GetHeaderCtrl() )
	{
		ClearListCtrl(m_pListStatistics);
		return;
	}

	int nRowCount = m_pList->GetItemCount();
	int nColsCount = m_pList->GetHeaderCtrl()->GetItemCount();
	int nColsCountStatistics = m_pListStatistics->GetHeaderCtrl()->GetItemCount();

	// 行数不对返回
	if(nRowCount<1)
	{
		ClearListCtrl(m_pListStatistics);
		return;
	}
	// 列数不对应返回
	if (nColsCountStatistics != nColsCount)
	{

		return;
	}

	// 数字检查 返回
	{
		for (int i=0;i<nColsCount-1;i++)
		{
			for (int j=0;j<nRowCount;j++)
			{
				CString str = m_pList->GetItemText(j,i+1);
				CString tmp2= str.SpanIncluding(_T("-.0123456789"));
				if (str !=tmp2)
				{
					return;
				}
			}
		}
	}

	std::vector<std::vector<double>>  vData;
	vData.resize(nColsCount-1);

	for (unsigned  int it = 0;it<vData.size();it++)
	{
		vData.at(it).resize(nRowCount,0.00);
		for (int j=0;j<nRowCount;j++)
		{
			vData.at(it).at(j) = _ttof(m_pList->GetItemText(j,it+1));
		}

	}
	CString strTmp = _T("");
	for (unsigned int it = 0;it<vData.size();it++)
	{
		// 最大值
		std::vector<double>::iterator Tmpiter = max_element(vData.at(it).begin(),vData.at(it).end());
		double dMax =*Tmpiter;
		strTmp.Format(_T("%.3f"),dMax);
		m_pListStatistics->SetItemText(0,it+1,strTmp);

		// 最小值
		Tmpiter = min_element(vData.at(it).begin(),vData.at(it).end());
		double dMin =*Tmpiter;
		strTmp.Format(_T("%.3f"),dMin);
		m_pListStatistics->SetItemText(1,it+1,strTmp);

		// 极差
		strTmp.Format(_T("%.3f"),dMax-dMin);
		m_pListStatistics->SetItemText(2,it+1,strTmp);
	}

}



void CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos2()
{
	m_nCurSearchIndex =1;
	ExeSearch(1);
}


void CDlgCalibCheckBySize::OnNMRClickListObjectSizeShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos(); //返回表示屏幕坐标下光标位置的长整数值
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_LIST_POPUPMENU) );
		CMenu* popup = menu.GetSubMenu(0); //取得被指定菜单激活的下拉式菜单或子菜单的句柄
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x, point.y, this ); //在指定位置显示快捷菜单，并跟踪菜单项的选择
	}
	*pResult = 0;
}


void CDlgCalibCheckBySize::OnListDelItem()
{
	// 删除一行数据
	int CurSel = m_ListObjecSize.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(CurSel >= 0)
	{    
		m_ListObjecSize.DeleteItem(CurSel);
		UpdateListColsOrder(&m_ListObjecSize);

		if (m_pVisionASM!=NULL)
		{
			m_pVisionASM->m_nExecuteCheckTimes = m_ListObjecSize.GetItemCount();
		}

		UpdateStatisticsList(&m_ListObjecSize,&m_ListObjectSizeStatistics);
	}

}


void CDlgCalibCheckBySize::OnNMRClickListImagepos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos(); //返回表示屏幕坐标下光标位置的长整数值
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_LIST_POPUPMENU2) );
		CMenu* popup = menu.GetSubMenu(0); //取得被指定菜单激活的下拉式菜单或子菜单的句柄
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x, point.y, this ); //在指定位置显示快捷菜单，并跟踪菜单项的选择
	}
	*pResult = 0;
}




void CDlgCalibCheckBySize::OnListDelItem2()
{
	// 删除一行数据
	int CurSel = m_ListImagePos.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(CurSel >= 0)
	{    
		m_ListImagePos.DeleteItem(CurSel);
		UpdateListColsOrder(&m_ListImagePos);
		UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
	}
}


// // 更新list序号列
void CDlgCalibCheckBySize::UpdateListColsOrder(CListCtrl * m_pList)
{
	// 空指针返回
	if (NULL == m_pList)
	{
		return;
	}


	for (int i = 0;i<m_pList->GetItemCount();i++)
	{
		CString str = _T("");
		str.Format(_T("%03d"),i+1);
		m_pList->SetItemText(i,0,str);
	}

}


void CDlgCalibCheckBySize::OnListClearAllItem2()
{

	m_ListImagePos.DeleteAllItems();
	UpdateListColsOrder(&m_ListImagePos);
	UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
}


void CDlgCalibCheckBySize::OnListClearAllItem()
{
	m_ListObjecSize.DeleteAllItems();
	if (m_pVisionASM!=NULL)
	{
		m_pVisionASM->m_nExecuteCheckTimes = m_ListObjecSize.GetItemCount();
	}
	UpdateListColsOrder(&m_ListObjecSize);
	UpdateStatisticsList(&m_ListObjecSize,&m_ListObjectSizeStatistics);
}

void CDlgCalibCheckBySize::SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner)
{
	m_pVisionASM = pVisionAligner;
}

void CDlgCalibCheckBySize::OnCancel()
{
	// 自行销毁非模态窗体，减少内存占用以及泄露
	::PostMessageA(m_pParent->GetSafeHwnd(),WM_DLGDESTROY,1,NULL);
	CDialogEx::OnCancel();
}


void CDlgCalibCheckBySize::OnOK()
{
	
	::PostMessageA(m_pParent->GetSafeHwnd(),WM_DLGDESTROY,1,NULL);
	CDialogEx::OnOK();
}


BOOL CDlgCalibCheckBySize::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgCalibCheckBySize::UpdateAlignerObjectSizeInfo()
{
	// 更新对象尺寸
	CString tmpStr=_T("");
	int nRow = m_ListObjecSize.GetItemCount();
	tmpStr.Format(_T("%03d"),nRow+1);
	m_ListObjecSize.InsertItem(nRow,tmpStr);
	for (int i= 0;i<m_AlignerObjectSizeInfo.m_vdObjectSize.size() && m_ListObjecSize.GetHeaderCtrl()->GetItemCount() && i<m_nSeachNums;i++)
	{
		tmpStr.Format(_T("%.2f"),m_AlignerObjectSizeInfo.m_vdObjectSize.at(i));
		m_ListObjecSize.SetItemText(nRow,i+1,tmpStr);
	}

	// 更新统计信息
	UpdateStatisticsList(&m_ListObjecSize,&m_ListObjectSizeStatistics);

}

void	CDlgCalibCheckBySize::UpdateAlignerTargetSizeInfo()
{
	// 更新目标尺寸
	CString tmpStr=_T("");
	int nRow = m_ListObjecSize.GetItemCount();
	tmpStr.Format(_T("%03d"),nRow+1);
	m_ListObjecSize.InsertItem(nRow,tmpStr);
	for (int i= 0;i<m_AlignerTargetSizeInfo.m_vdTargetSize.size() && m_ListObjecSize.GetHeaderCtrl()->GetItemCount() && i<m_nSeachNums;i++)
	{
		tmpStr.Format(_T("%.2f"),m_AlignerTargetSizeInfo.m_vdTargetSize.at(i));
		m_ListObjecSize.SetItemText(nRow,i+1,tmpStr);
	}
	// 更新统计信息
	UpdateStatisticsList(&m_ListObjecSize,&m_ListObjectSizeStatistics);
}


void CDlgCalibCheckBySize::OnBnClickedRadioGetAxisByComm()
{
	
	UpdateData(TRUE);
	// 轴输入文本框 只读 供显示
	if (m_nGetAxisOption==0)
	{
		for (int i=0;i<19;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))->SetReadOnly(TRUE);
				}
				
			}
		}
		m_bIsReadOnly = TRUE;
	}
	else // 轴输入文本框 可编辑
	{
		for (int i=0;i<19;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))->SetReadOnly(FALSE);
				}

			}
		}
		m_bIsReadOnly = FALSE;
	}
	

}


void CDlgCalibCheckBySize::InitAxisData()
{
	if (NULL!=m_pVisionASM)
	{
		CTotalCalibData *pTotalCalibData;
		pTotalCalibData = m_pVisionASM->GetTotalCalibData();
		if (pTotalCalibData == NULL)
		{
			return;
		}
		int nCurProductIdx = pTotalCalibData->GetCurCalibIndex();

		// 获取当前产品对应的标定数据
		CCalibData* pCalibData;
		pCalibData = pTotalCalibData->GetItemPtr(nCurProductIdx);
		if (pCalibData == NULL)
		{
			return;
		}

		// 获取当前位置对应的标定结果
		CCalibratedInfo* pCalibratedInfo;
		//
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);

		int nEnd = m_pVisionASM->GetPosNum();
		int nStart = 0;

		if (m_nTypeIndex ==0) // 标定验证
		{
			if (platformInfo.m_bTargetObjectCamSeparate)
			{
				nEnd = m_pVisionASM->GetPosNum()/2;
			}
			else
			{
				nEnd = m_pVisionASM->GetPosNum();
			}
		}
		else // 关联验证
		{
			if (platformInfo.m_bTargetObjectCamSeparate)
			{
				nStart = m_pVisionASM->GetPosNum()/2;
				nEnd = m_pVisionASM->GetPosNum();
			}
		}

		// 考虑单平台扩展；
		int nExProBase = 0;
		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			nExProBase = m_nExProductIndex*m_pVisionASM->GetPosNum();
		}

		for (int i=nExProBase+ nStart;i<nExProBase+ nEnd;i++)
		{
			pCalibratedInfo = pCalibData->m_vpCalibratedInfo.at(i);
			if (pCalibratedInfo != NULL)
			{
				double dUVW_1 = 0.0;
				double dUVW_2 = 0.0;
				double dUVW_3 = 0.0;
				// 轴位置
				CPlatformXYDAxisPos* pCalibPlatformAxisPos = (CPlatformXYDAxisPos*)pCalibratedInfo->GetPlatformAxisPos();
				int k=(i-nStart-nExProBase)*5;
				CString str;
				if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
				{
					CPlatformXY1Y2Info platformXY1Y2Info;
					platformXY1Y2Info.ConvertXYDPltfmPosToXY1Y2PltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_2, dUVW_3, dUVW_1);//Y1、Y2、X

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
				{
					CPlatformX1X2YInfo platformX1X2YInfo;
					platformX1X2YInfo.ConvertXYDPltfmPosToX1X2YPltfmPos(pCalibPlatformAxisPos->m_dPosX, pCalibPlatformAxisPos->m_dPosY, pCalibPlatformAxisPos->m_dAngle,
						dUVW_1, dUVW_2, dUVW_3);//X1、X2、Y

					str.Format(_T("%.4f"), dUVW_1);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"), dUVW_2);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"), dUVW_3);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}
				else
				{
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosX);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2))->SetWindowText(str);
					}

					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dPosY);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3))->SetWindowText(str);
					}
					str.Format(_T("%.4f"),pCalibPlatformAxisPos->m_dAngle);
					if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4)) !=NULL)
					{
						((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4))->SetWindowText(str);
					}
				}

			}
		}

	}
}

void CDlgCalibCheckBySize::InitAxisTextInfo()
{
	if (NULL!=m_pVisionASM)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y)
		{
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA, _T("X1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA, _T("X2(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA, _T("Y(mm)"));
		}
		else if (platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
		{
			SetDlgItemText(IDC_STATIC_AXIS_X_DATA, _T("X(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_Y_DATA, _T("Y1(mm)"));
			SetDlgItemText(IDC_STATIC_AXIS_D_DATA, _T("Y2(mm)"));
		}
	}
}

void CDlgCalibCheckBySize::OnBnClickedRadioGetAxisByInput()
{
	UpdateData(TRUE);
	// 轴输入文本框 只读 供显示
	if (m_nGetAxisOption==0)
	{
		for (int i=0;i<19;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))->SetReadOnly(TRUE);
				}

			}
		}
		m_bIsReadOnly = TRUE;
	}
	else // 轴输入文本框 可编辑
	{
		for (int i=0;i<19;i++)
		{
			if (NULL !=GetDlgItem(IDC_STATIC_POS_AXIS1+i))
			{
				if (  ((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))!=NULL)
				{
					((CEdit*)GetDlgItem(IDC_STATIC_POS_AXIS1+i))->SetReadOnly(FALSE);
				}

			}
		}
		m_bIsReadOnly = FALSE;
	}
}

void CDlgCalibCheckBySize::ExeSearch(int nPos)
{
	if (NULL !=m_pVisionASM)
	{
		if (nPos<0 || nPos>=m_pVisionASM->GetPosNum())
		{
			return;
		}
		
		//
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		// 对轴进行处理  手动输入轴时，暂时只有UVW平台类型进行转换XYD处理； 
		if (m_nGetAxisOption == 1)
		{
			int k=nPos*5;
			CString strTmp = _T("");
			((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = TRUE;

			// UVW 平台
			std::vector<CString> vstrAxisPos;

			// UVW-1
			if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			// UVW-2
			if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			// UVW-3
			if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4)) !=NULL)
			{
				((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4))->GetWindowText(strTmp);
			}
			vstrAxisPos.push_back(strTmp);

			if (platformInfo.m_eMidPlatformType == ePlatformX1X2Y || platformInfo.m_eMidPlatformType == ePlatformXY1Y2)
			{	
				m_pVisionASM->m_XYDCommProtocol.ConvertToPlatformXYDAxisPos(vstrAxisPos, &(m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos));
			}
			else
			{
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosX = atof(vstrAxisPos.at(0));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dPosY = atof(vstrAxisPos.at(1));
				m_pVisionASM->m_XYDCommProtocol.m_CurPlatformXYDAxisPos.m_dAngle = atof(vstrAxisPos.at(2));
			}
		}
		else
		{
			CString strAttention;
			strAttention = m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings->GetAt(IDS_MB_PLEASE_PLC_SEND_CUR_PLATFORM_POS);
			AfxMessageBox(strAttention);
		}
		
		UpdateData(TRUE);
		if (platformInfo.m_bEnableMultiCalibExtension)
		{
			vcBaseAlignTool* tmpAlignTool = m_pVisionASM->GetAlignTool();
			if (tmpAlignTool != NULL)
			{
				m_pVisionASM->SetAlignExProductIndex(m_nExProductIndex);
				tmpAlignTool->m_bEnableMultiCombinationCode = FALSE;
			}

		}

		std::vector<int> vnPosIndex;
		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if (m_nTypeIndex ==0) //标定
		{

			if(optionInfo.m_bEnableMultiMarkMode)
			{
				if(nPos==0)
				{
					m_vbSearchSuss.at(nPos) =  ((vcXYDVisionAlign*)m_pVisionASM)->OnSingleCameraAlignerObjectSearchCandidateExFOBC(nPos, 0);
					if(m_vbSearchSuss.at(nPos))
					{
						for (int nSize=1;nSize<m_vbSearchSuss.size();nSize++)
						{
							m_vbSearchSuss.at(nSize)=TRUE;
						}
					}
				}
				else
				{
					return;
				}
			}
			else
			{
				vnPosIndex.push_back(nPos);
				m_vbSearchSuss.at(nPos) =  ((vcXYDVisionAlign*)m_pVisionASM)->OnSerialCameraAlignerObjectSearchCandidateExFOBC(vnPosIndex, 0);
			}
		} 
		else  // 目标尺寸验证
		{

			if (platformInfo.m_bTargetObjectCamSeparate) //对象目标分离
			{
				if(optionInfo.m_bEnableMultiMarkMode)
				{
					if (nPos==0)
					{
						m_vbSearchSuss.at(nPos) =  ((vcXYDVisionAlign*)m_pVisionASM)->OnSingleCameraAlignerTargetSearchCandidateExFTGC(nPos+m_pVisionASM->GetPosNum()/2, 0);
						if(m_vbSearchSuss.at(nPos))
						{
							for (int nSize=1;nSize<m_vbSearchSuss.size();nSize++)
							{
								m_vbSearchSuss.at(nSize)=TRUE;
							}
						}
					}
					else
					{
						return;
					}
				}
				else
				{
					vnPosIndex.push_back(nPos+m_pVisionASM->GetPosNum()/2);
					m_vbSearchSuss.at(nPos)  = ((vcXYDVisionAlign*)m_pVisionASM)->OnSerialCameraAlignerTargetSearchCandidateExFTGC(vnPosIndex, 0);
				}
			}
			else
			{
				if(optionInfo.m_bEnableMultiMarkMode)
				{
					if(nPos==0)
					{
						m_vbSearchSuss.at(nPos) =  ((vcXYDVisionAlign*)m_pVisionASM)->OnSingleCameraAlignerTargetSearchCandidateExFTGC(nPos, 0);
						if(m_vbSearchSuss.at(nPos))
						{
							for (int nSize=1;nSize<m_vbSearchSuss.size();nSize++)
							{
								m_vbSearchSuss.at(nSize)=TRUE;
							}
						}
					}
					else
					{
						return;
					}
				}
				else
				{
					vnPosIndex.push_back(nPos);
					m_vbSearchSuss.at(nPos)  = ((vcXYDVisionAlign*)m_pVisionASM)->OnSerialCameraAlignerTargetSearchCandidateExFTGC(vnPosIndex, 0);
				}
				

			}
			

		}

		((vcXYDVisionAlign*)m_pVisionASM)->m_bCmdParamAxisPosEanble = FALSE;
	}
}

void CDlgCalibCheckBySize::UpdateAxisInfo()
{
	int k = m_nCurSearchIndex*5;
	// X
	if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(0));
		((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+2))->SetWindowText(str);
	}

	// Y
	if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(1));
		((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+3))->SetWindowText(str);
	}
	// D
	if (((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4)) !=NULL)
	{
		CString str = _T("");
		str.Format(_T("%.4f"),m_AlignerAxisInfo.m_vdAxis.at(2));
		((CEdit*)GetDlgItem(IDC_BTN_OBJECT_SEARCH_POS1+k+4))->SetWindowText(str);
	}

}



void CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos3()
{
	m_nCurSearchIndex =2;
	ExeSearch(2);
}


void CDlgCalibCheckBySize::OnBnClickedBtnObjectSearchPos4()
{
	m_nCurSearchIndex =3;
	ExeSearch(3);
}


void CDlgCalibCheckBySize::OnBnClickedBtnObjectSizeCalc()
{
	
	if (m_pVisionASM !=NULL)
	{
		SysPlatformInfo platformInfo;
		m_pVisionASM->GetSysPlatformInfo(platformInfo);
		int nPos =  m_pVisionASM->GetPosNum();
		int nBaseindex = 0;
		if (m_nTypeIndex==1 && platformInfo.m_bTargetObjectCamSeparate == TRUE)
		{
			nBaseindex = nPos/2;
		}

		for (int it =0;it<m_vbSearchSuss.size();it++)
		{
			if(!m_vbSearchSuss.at(it))
			{
				CString strTmp = _T("");
				CString strTmp2 = _T("位置%d未完成搜索");
				
				if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
				{
					strTmp2 = GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,IDS_POS_NON_SNAP);
				}

				strTmp.Format(strTmp2,nBaseindex + it+1);
				AfxMessageBox(strTmp);
				return;
			}
		}

		CPlatformOptionInfo optionInfo;
		m_pVisionASM->GetPlatformOptionInfo(optionInfo);
		if(optionInfo.m_bEnableMultiMarkMode)
		{
			m_ListImagePos.DeleteAllItems();
			UpdateStatisticsList(&m_ListImagePos,&m_ListImagePosStatistics);
		}

		// 刷新结果
		::PostMessageA(m_pParent->GetSafeHwnd(),WM_CALCSIZE,1,NULL);


		// 是否将搜索成功置False，避免重复相同的尺寸结果进行统计
		for (int it =0;it<m_vbSearchSuss.size();it++)
		{
			m_vbSearchSuss[it] = FALSE;
		}
	}
}

void CDlgCalibCheckBySize::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_pParent->m_pParent->m_pParent->m_psaSysInfoStrings,0));
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

		if(_T("IDD_DLG_CALIB_SIZE_CHECK") == strDlgID)
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

void CDlgCalibCheckBySize::OnEnSetfocusEdit_Keyboard(int nID)
{
	if (  ((CEdit*)GetDlgItem(nID))==NULL)
	{
		return;
	}
	if (m_bIsReadOnly) // 文本框为不可输入状态时
	{
		return;
	}
	GetDlgItemText(nID, m_strInfoOld);
	if (m_bSysKeyboardEnabled)
	{
		m_bKillFocusEnabled = FALSE;
		::PostMessage(m_pParent->m_pParent->GetSafeHwnd(), WM_SETFOCUS, NULL, NULL);

		CRect rcWnd;
		CString strInput, strOutput;
		GetDlgItem(nID)->GetWindowRect(&rcWnd);
		GetDlgItemText(nID, strInput);
		if (TRUE == m_ctrlKeyboard.ShowNumeralDotSymbolKeyBoard(rcWnd, strInput, strOutput))
		{
			SetDlgItemText(nID, strOutput);
		}
	}	
}

void CDlgCalibCheckBySize::OnEnKillfocusEdit(int nID)
{
	CString strVal;
	GetDlgItemText(nID, strVal);
	if (!IsStringNumerical(strVal))
	{
		SetDlgItemText(nID, m_strInfoOld);
	}
	else
	{
		SetDlgItemText(nID, strVal);
	}
}

BOOL CDlgCalibCheckBySize::IsStringNumerical(LPCTSTR lpszVal)
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

void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_X1);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_Y1);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD1()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_D1);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_X2);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_Y2);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD2()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_D2);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_X3);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_Y3);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD3()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_D3);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisX4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_X4);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisY4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_Y4);
}


void CDlgCalibCheckBySize::OnEnSetfocusEditValPosAxisD4()
{
	OnEnSetfocusEdit_Keyboard(IDC_EDIT_VAL_POS_AXIS_D4);
}

void CDlgCalibCheckBySize::OnCbnSelchangeComboExproductCheckBySize()
{
	// 切换扩展平台序号后，需要重新初始化轴，清空尺寸列表，图像列表；之前搜索成功的对象目标位置标识
	UpdateData(TRUE);

	InitAxisData();
	OnListClearAllItem();
	OnListClearAllItem2();
	for (int it =0;it<m_vbSearchSuss.size();it++)
	{
		m_vbSearchSuss[it] = FALSE;
	}

	SysPlatformInfo sysPlatformInfo;
	m_pVisionASM->GetSysPlatformInfo(sysPlatformInfo);

	CVisionASMConfig *vsConfig;
	m_pVisionASM->GetVSconfig(vsConfig);
	CString StrTmp = _T("");
	if (m_nTypeIndex==0)
	{
		if ((NULL !=vsConfig))
		{
			if (sysPlatformInfo.m_bProductFaChd)
			{
				int nCurProductIndexFa;
				int nCurProductIndexCh;
				vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_大品种%03d_小品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
					nCurProductIndexFa+1,nCurProductIndexCh+1);
			}
			else
			{
				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CalibrationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
					vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
			}

		}
	}
	else if(m_nTypeIndex==1)
	{
		if ((NULL !=vsConfig))
		{
			if (sysPlatformInfo.m_bProductFaChd)
			{
				int nCurProductIndexFa;
				int nCurProductIndexCh;
				vsConfig->m_TotalProductData.GetCurDlgProductIndex(nCurProductIndexFa,nCurProductIndexCh);
				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_大品种%03d_小品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
					nCurProductIndexFa+1,nCurProductIndexCh+1);
			}
			else
			{
				StrTmp.Format(_T("D:\\VisionASMLog\\工位%d_品种%03d_CorrelationVerificationLog.txt"),sysPlatformInfo.m_nPlatformIndex+1,
					vsConfig->m_TotalCalibData.GetCurCalibIndex()+1);
			}

		}
	}


	if (sysPlatformInfo.m_bEnableMultiCalibExtension)
	{
		CString strTemp = _T("");
		strTemp.Format(_T("当前扩展序号: %3d;\n"),m_nExProductIndex);
		m_pVisionASM->VisionAlignLogRecord(StrTmp,strTemp,TRUE);
	}

}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_X1);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_Y1);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD1()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_D1);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_X2);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_Y2);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD2()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_D2);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_X3);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_Y3);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD3()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_D3);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisX4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_X4);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisY4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_Y4);
}


void CDlgCalibCheckBySize::OnEnKillfocusEditValPosAxisD4()
{
	OnEnKillfocusEdit(IDC_EDIT_VAL_POS_AXIS_D4);
}