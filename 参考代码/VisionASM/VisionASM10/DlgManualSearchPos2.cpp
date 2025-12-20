// DlgManualSearchPos2.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgManualSearchPos2.h"
#include "afxdialogex.h"


// CDlgManualSearchPos2 对话框

IMPLEMENT_DYNAMIC(CDlgManualSearchPos2, CDialog)

CDlgManualSearchPos2::CDlgManualSearchPos2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualSearchPos2::IDD, pParent)
{
	m_psaSysInfoStrings = NULL;

	m_pVisionASM = NULL;

	m_pVAPrevMainWnd = NULL;
}

CDlgManualSearchPos2::~CDlgManualSearchPos2()
{
}

void CDlgManualSearchPos2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgManualSearchPos2, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SV_DISPLAY_CLICK,OnGuiDisplayClick)
	ON_BN_CLICKED(IDOK, &CDlgManualSearchPos2::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgManualSearchPos2 消息处理程序


BOOL CDlgManualSearchPos2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDialogLanguage();

	CString strTitle;
	if (m_nMarkType == 0)
	{
		strTitle.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SELECT_MARK_OBJECT_POSITION) + "+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	//	strTitle.Format("手动选择Mark点---工位%d对象位置%d+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	}
	else if (m_nMarkType == 1)
	{
		strTitle.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SELECT_MARK_TARGET_POSITION) + "+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	//	strTitle.Format("手动选择Mark点---工位%d目标位置%d+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	}
	else
	{
		strTitle.Format(GetSysInfoString(m_psaSysInfoStrings,IDS_STR_SELECT_MARK_POSITION) + "+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	//	strTitle.Format("手动选择Mark点---工位%d位置%d+%d",m_nCurPlatformIndex+1,m_vnPosIndex.at(0)+1,m_vnPosIndex.at(1)+1);
	}
	SetWindowText(strTitle);

	InitDlgItem();

	m_pVisionASM->SetPosDisplay(m_vnPosIndex.at(0), m_vpGuiDisplay.at(0));
	m_pVisionASM->SetPosDisplay(m_vnPosIndex.at(1), m_vpGuiDisplay.at(1));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgManualSearchPos2::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pVisionASM != NULL)
	{
		// 还原旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		for (int i=0; i<nNum; i++)
		{
			m_pVisionASM->SetPosDisplay(i, m_vpPrevGuiDisplay.at(i));
		}
		m_pVisionASM->SetMainWnd(m_pVAPrevMainWnd);
	}

	for (int i=0; i<m_vpGuiDisplay.size(); i++)
	{
		scGuiDisplay* pGuiDisplay = m_vpGuiDisplay.at(i);
		if (pGuiDisplay != NULL)
		{
			pGuiDisplay->RemoveGraphics();

			delete pGuiDisplay;
			pGuiDisplay = NULL;
		}
	}

	for (int i=0; i<m_vpGuiCross.size(); i++)
	{
		scGuiCross* pGuiCross = m_vpGuiCross.at(i);
		if (pGuiCross != NULL)
		{
			delete pGuiCross;
			pGuiCross = NULL;
		}
	}
}


LRESULT CDlgManualSearchPos2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_UPDATE_DISPLAY_IMAGE:
		{
			m_pVisionASM->UpdateDisplayImage((int)wParam);

		}
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgManualSearchPos2::InitDlgItem()
{
	int nPosNum = 2;
	m_vpGuiDisplay.resize(nPosNum, NULL);
	m_vpGuiCross.resize(nPosNum, NULL);
	m_vCoordPos.resize(nPosNum);
	m_vnPosIndex.resize(nPosNum);
	for (int i=0; i<nPosNum; i++)
	{
		GetDlgItem(IDC_STATIC_CH0_DISPLAY+i)->ShowWindow(SW_SHOW);
		m_vpGuiDisplay.at(i) = new scGuiDisplay;
		m_vpGuiDisplay.at(i)->Create(IDC_STATIC_CH0_DISPLAY+i, this,WS_VISIBLE);
		m_vpGuiDisplay.at(i)->SetBackgroundParam(bgDispBGPure,RGB(222,225,231));
		m_vpGuiDisplay.at(i)->RegisterWndMsg(this, SV_DISPLAY_CLICK);

		m_vpGuiCross.at(i) = new scGuiCross;
		m_vpGuiCross.at(i)->SetCenterExtent(sc2Vector(320,240),sc2Vector(640,640));
		m_vpGuiCross.at(i)->SetVisible(TRUE);

		scGuiInteractiveContainer* pInteractiveContainer = m_vpGuiDisplay.at(i)->GetInteractiveContainer();
		pInteractiveContainer->AddItem(m_vpGuiCross.at(i), _T("定位结果"));
		m_vpGuiDisplay.at(i)->SetInteractiveContainer(pInteractiveContainer);
	}
}

void CDlgManualSearchPos2::SetVisionAlignerRef(vcBaseVisionAlign *pVisionAligner, std::vector<int> vnPosIndex)
{
	if ( (pVisionAligner == NULL) || (vnPosIndex.size() != 2) )
	{
		return;
	}

	m_pVisionASM = pVisionAligner;
	m_vnPosIndex = vnPosIndex;
	if (m_pVisionASM != NULL)
	{
		// 保存旧的GuiDisplay
		int nNum = m_pVisionASM->GetPosNum();
		m_vpPrevGuiDisplay.resize(nNum);
		for (int i=0; i<nNum; i++)
		{
			m_vpPrevGuiDisplay.at(i) = m_pVisionASM->GetPosDisplay(i);
			m_pVisionASM->SetPosDisplay(i, NULL);
		}
		m_pVAPrevMainWnd = m_pVisionASM->SetMainWnd(this);
	}
}


BOOL CDlgManualSearchPos2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
//	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_SPACE) return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDlgManualSearchPos2::OnGuiDisplayClick(WPARAM wParam, LPARAM lParam)
{
	scGuiDisplay* m_pGuiDisplay = m_vpGuiDisplay.at(0);
	scGuiCross* m_pGuiCross = m_vpGuiCross.at(0);
	if (wParam == m_pGuiDisplay->GetID())
	{
		if (m_pGuiDisplay->GetImageCP() != NULL)
		{
			if (m_pGuiDisplay->GetImageCP()->IsValid())
			{
				long nWidth  = m_pGuiDisplay->GetImageCP()->Width();
				long nHeight = m_pGuiDisplay->GetImageCP()->Height();

				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_pGuiDisplay->TransformPos(x,y,dImageX,dImageY);

				if ( (m_pGuiCross != NULL) && (dImageX>=0) && (dImageY>=0) && (dImageX<nWidth) && (dImageY<nHeight) )
				{
					m_pGuiCross->SetCenterExtent(sc2Vector(dImageX,dImageY), sc2Vector(640,480));
				}
			}
		}
	}

	m_pGuiDisplay = m_vpGuiDisplay.at(1);
	m_pGuiCross = m_vpGuiCross.at(1);
	if (wParam == m_pGuiDisplay->GetID())
	{
		if (m_pGuiDisplay->GetImageCP() != NULL)
		{
			if (m_pGuiDisplay->GetImageCP()->IsValid())
			{
				long nWidth  = m_pGuiDisplay->GetImageCP()->Width();
				long nHeight = m_pGuiDisplay->GetImageCP()->Height();

				// 获取鼠标相对于显示控件的坐标,lParam的高16位存储的是Y坐标,低16位存储的是X坐标
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				// 将相对于显示控件坐标系的坐标,转换到相对于图像坐标系的坐标
				double dImageX, dImageY;
				m_pGuiDisplay->TransformPos(x,y,dImageX,dImageY);

				if ( (m_pGuiCross != NULL) && (dImageX>=0) && (dImageY>=0) && (dImageX<nWidth) && (dImageY<nHeight) )
				{
					m_pGuiCross->SetCenterExtent(sc2Vector(dImageX,dImageY), sc2Vector(640,480));
				}
			}
		}
	}

	return 0 ;
}


void CDlgManualSearchPos2::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	for (long i=0; i<m_vpGuiCross.size(); i++)
	{
		m_vCoordPos.at(i).m_dPosX = m_vpGuiCross.at(i)->GetCenter().GetX();
		m_vCoordPos.at(i).m_dPosY = m_vpGuiCross.at(i)->GetCenter().GetY();
		m_vCoordPos.at(i).m_dAngle = 0;
	}

	CDialog::OnOK();
}

void CDlgManualSearchPos2::UpdateDialogLanguage()
{
	//	return;
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
	}

	vcXMLConfigurator xml;
	CString strXMLPath;
	CString strCurWorkDir;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	strCurWorkDir = szPath;
	strCurWorkDir = strCurWorkDir.Left(strCurWorkDir.ReverseFind('\\'));

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

		if(_T("IDD_DLG_MANUAL_SEARCH_POS2") == strDlgID)
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
