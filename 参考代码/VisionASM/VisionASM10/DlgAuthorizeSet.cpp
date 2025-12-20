// DlgAuthorizeSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAuthorizeSet.h"
#include "afxdialogex.h"
#include "LogFileSave.h"
#include "DlgSysEnvironSetting.h"
// CDlgAuthorizeSet 对话框

IMPLEMENT_DYNAMIC(CDlgAuthorizeSet, CDialogEx)

CDlgAuthorizeSet::CDlgAuthorizeSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAuthorizeSet::IDD, pParent)
{
	 //m_icon[0] = NULL;
	 //ImageListTree = NULL;
}

CDlgAuthorizeSet::~CDlgAuthorizeSet()
{


	//if (ImageListTree != NULL)
	//{
	//	ImageListTree->DeleteImageList();
	//	delete ImageListTree;
	//	ImageListTree = NULL;
	//}
	//if (m_icon[0])
	//{
	//	DestroyIcon(m_icon[0]);
	//	m_icon[0] = NULL;
	//}


}

void CDlgAuthorizeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_AUTHORIZE_SET_INFO, m_TreeAuthorizeView);
}

BEGIN_MESSAGE_MAP(CDlgAuthorizeSet, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE_AUTHORIZE_SET_INFO, &CDlgAuthorizeSet::OnNMClickTreeAuthorizeSetInfo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CDlgAuthorizeSet 消息处理程序

BOOL CDlgAuthorizeSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateDialogLanguage();
// 	HICON icon[1];
// 	icon[0]=AfxGetApp()->LoadIcon(IDI_ICON4);
	//HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON4), RT_GROUP_ICON);
	//m_icon[0] = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON4), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	//ImageListTree = new CImageList;
	//ImageListTree->Create(16,16,0,1,1); //16,16为图标分辩率，4,4为该list最多能容纳的图标数
	//
	//ImageListTree->Add(m_icon[0]); //读入图标
	//m_TreeAuthorizeView.SetImageList(ImageListTree,TVSIL_NORMAL);

	// TODO:  在此添加额外的初始化

	CString strTmp = _T("权限管理");
	if(m_psaSysInfoStrings !=NULL)
	{
		strTmp = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AUTHORIZE_MANAGEMENT);
	}

	m_TreeAuthorizeView.ModifyStyle(TVS_CHECKBOXES,0);
	m_TreeAuthorizeView.ModifyStyle(0,TVS_CHECKBOXES);

	HTREEITEM hImgNodeGlobal = m_TreeAuthorizeView.InsertItem(strTmp,0,0);
	BandingTreeCtrl(m_TreeAuthorizeView,hImgNodeGlobal,m_cTreeAuthorizeShow.children);
	m_TreeAuthorizeView.Expand(hImgNodeGlobal, TVE_EXPAND );

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgAuthorizeSet::UpdateDialogLanguage()
{
	LANGID lgidLanguage = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	if (m_psaSysInfoStrings != NULL)
	{
		lgidLanguage = (LANGID)_ttol(GetSysInfoString(m_psaSysInfoStrings,0));
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

		if(_T("IDD_SYS_AUTHORIZE_SET") == strDlgID)
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
BOOL CDlgAuthorizeSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4)) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) return TRUE;
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgAuthorizeSet::SetChildCheck(HTREEITEM item, BOOL bCheck)
{
	CString strLog = _T("");

	HTREEITEM child = m_TreeAuthorizeView.GetChildItem(item);
	while(child)
	{
		if (item !=NULL)
		{
			strLog = m_TreeAuthorizeView.GetItemText(item) + _T("---");
		}
		m_TreeAuthorizeView.SetCheck(child, bCheck);
		bool *pbNodeData = NULL;
		pbNodeData = (bool*)(m_TreeAuthorizeView.GetItemData(child));
		if (pbNodeData)
		{
			*pbNodeData = (bool)(bCheck);
			strLog += m_TreeAuthorizeView.GetItemText(child);
			CString strValue = bCheck ? _T("选中状态"):_T("未选中状态");
			CString strLog2;
			strLog2.Format(_T("修改[%s]为[%s]"),strLog,strValue);
			g_opLogFile.SaveLogText(strLog2,3);
		}
		
		SetChildCheck(child, bCheck);
		child = m_TreeAuthorizeView.GetNextItem(child, TVGN_NEXT);
	}
}

void CDlgAuthorizeSet::SetParentCheck(HTREEITEM item, BOOL bCheck)
{
	// 父节点下全部子节点为同一状态时，父节点变更为该状态
	HTREEITEM parent = m_TreeAuthorizeView.GetParentItem(item);
	if(parent == NULL)
	{
		return;
	}

	HTREEITEM bro = m_TreeAuthorizeView.GetNextItem(item, TVGN_NEXT);
	BOOL bFlag = TRUE;
	while(bro)
	{
		if(!m_TreeAuthorizeView.GetCheck(bro))
		{
			bFlag = FALSE;
			break;
		}
		bro = m_TreeAuthorizeView.GetNextItem(bro, TVGN_NEXT);
	}
	if(bFlag)
	{
		bro = m_TreeAuthorizeView.GetNextItem(item, TVGN_PREVIOUS);
		while(bro)
		{
			if(!m_TreeAuthorizeView.GetCheck(bro))
			{
				bFlag = FALSE;
				break;
			}
			bro = m_TreeAuthorizeView.GetNextItem(bro, TVGN_PREVIOUS);
		}
	}
	if(bFlag)
	{
		m_TreeAuthorizeView.SetCheck(parent, bCheck);
	}
	
	SetParentCheck(parent, m_TreeAuthorizeView.GetCheck(parent));
}

void CDlgAuthorizeSet::BandingTreeCtrl(CTreeCtrl & ctreeCtrl,HTREEITEM Root,std::vector<CTree<CPermissionEntries*>> &children)
{
	if (Root!=NULL && children.size()>0)
	{
		for (UINT it=0;it < children.size();it++)
		{
			if (children.at(it).node == NULL)
			{
				continue;
			}

			CString strTmp= children.at(it).node->m_StrName;

			if(m_psaSysInfoStrings !=NULL && NULL !=m_pVisionASM)
			{
				CVisionASMConfig *vsConfig;
				m_pVisionASM->GetVSconfig(vsConfig);
				if (NULL !=vsConfig)
				{
					
					auto iterFind = vsConfig->m_MapAuthorizeItem.find(children.at(it).node->m_nIndex);
					if(iterFind != vsConfig->m_MapAuthorizeItem.end())
					{
						strTmp = GetSysInfoString(m_psaSysInfoStrings,iterFind->second.second.first);
					}

				}
				
			}


			HTREEITEM Root1 = ctreeCtrl.InsertItem((LPCTSTR)strTmp,0,0,Root,TVI_LAST);


			////方式1： 直接通过选项控制-启用禁用
			//{
			//	bool *pbNodeData = NULL;
			//	pbNodeData = &(children.at(it).node->m_bEnable);
			//	ctreeCtrl.SetItemData(Root1,(DWORD)pbNodeData);
			//	ctreeCtrl.SetCheck(Root1,*pbNodeData);

			//	SetChildCheck(Root1, *pbNodeData);
			//	SetParentCheck(Root1, *pbNodeData);
			//}


			////方式1： 具体权限项-启用/禁用
			if (children.at(it).node->m_bEnableAllowSet && children.at(it).node->m_bGroup==false)
			{
				CString strTmp = _T("启用");
				if(m_psaSysInfoStrings !=NULL)
				{
					strTmp = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AUTHORIZE_ITEM1);
				}

				HTREEITEM RootEnable = ctreeCtrl.InsertItem(strTmp,0,0,Root1,TVI_LAST);
				bool *pbNodeData = NULL;
				pbNodeData = &(children.at(it).node->m_bEnable);
				ctreeCtrl.SetItemData(RootEnable,(DWORD)pbNodeData);
				ctreeCtrl.SetCheck(RootEnable,*pbNodeData);

				SetChildCheck(RootEnable, *pbNodeData);
				SetParentCheck(RootEnable, *pbNodeData);
			}
			// 具体权限项-显示/隐藏 
			// 暂时不隐藏产品的模板隐藏
			if (children.at(it).node->m_bShowAllowSet && children.at(it).node->m_bGroup==false)
			{
				CString strTmp = _T("显示");
				if(m_psaSysInfoStrings !=NULL)
				{
					strTmp = GetSysInfoString(m_psaSysInfoStrings,IDS_CMB_AUTHORIZE_ITEM2);
				}

				HTREEITEM RootShow = ctreeCtrl.InsertItem(strTmp,0,0,Root1,TVI_LAST);
				bool *pbNodeData = NULL;

				pbNodeData = &(children.at(it).node->m_bShow);
				ctreeCtrl.SetItemData(RootShow,(DWORD)pbNodeData);
				ctreeCtrl.SetCheck(RootShow,*pbNodeData);

				SetChildCheck(RootShow, *pbNodeData);
				SetParentCheck(RootShow, *pbNodeData);
			}

			BandingTreeCtrl(ctreeCtrl,Root1,children.at(it).children);
		}
	}
}

void CDlgAuthorizeSet::OnNMClickTreeAuthorizeSetInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	
	CPoint pt;
	UINT flag;
	HTREEITEM hCurrentItem;  
	
	GetCursorPos(&pt);
	m_TreeAuthorizeView.ScreenToClient(&pt);
	hCurrentItem = m_TreeAuthorizeView.HitTest(pt, &flag); 
	if ((hCurrentItem) && (flag & TVHT_ONITEMSTATEICON))//是点击CHECK框
	{
		
		BOOL bCheck = m_TreeAuthorizeView.GetCheck(hCurrentItem);
		CString strLog = _T("");

		HTREEITEM parent = m_TreeAuthorizeView.GetParentItem(hCurrentItem);
		if (parent !=NULL)
		{
			strLog = m_TreeAuthorizeView.GetItemText(parent)+ _T("---");;
		}
		CString strValue = bCheck ? _T("选中状态"):_T("未选中状态");
		bool *pnNodeData = NULL;
		pnNodeData = (bool*)(m_TreeAuthorizeView.GetItemData(hCurrentItem));
		if (pnNodeData)
		{
			*pnNodeData = (!bCheck);
			strLog +=m_TreeAuthorizeView.GetItemText(hCurrentItem);
			CString strValue2 = !bCheck ? _T("选中状态"):_T("未选中状态");
			CString strLog2;
			strLog2.Format(_T("修改[%s]:由[%s]到[%s]"),strLog,strValue,strValue2);
			g_opLogFile.SaveLogText(strLog2,3);
		}

		SetChildCheck(hCurrentItem, !bCheck);
		SetParentCheck(hCurrentItem, !bCheck);
	}

	*pResult = 0;

}


void CDlgAuthorizeSet::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CImageList *piml = m_TreeAuthorizeView.GetImageList(TVSIL_STATE);
	if (piml)
	{
		piml->DeleteImageList();
	}
	// TODO: 在此处添加消息处理程序代码
}
