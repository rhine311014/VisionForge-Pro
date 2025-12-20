#pragma once

#include "vsVisionAlignDef.h"
#include "afxcmn.h"
// CDlgAuthorizeSet 对话框
class CDlgSysEnvironSetting;
class CDlgAuthorizeSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAuthorizeSet)

public:
	CDlgAuthorizeSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAuthorizeSet();

// 对话框数据
	enum { IDD = IDD_SYS_AUTHORIZE_SET };
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickTreeAuthorizeSetInfo(NMHDR *pNMHDR, LRESULT *pResult);

	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM){m_pVisionASM = pVisionASM;}
	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();

	void SetChildCheck(HTREEITEM item, BOOL bCheck); // 统一子节点下的全部子节点状态
	void SetParentCheck(HTREEITEM item, BOOL bCheck);// 根据子节点状态变更父节点状态
	void BandingTreeCtrl(CTreeCtrl & ctreeCtrl,HTREEITEM Root,std::vector<CTree<CPermissionEntries*>> &children);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CStringArray *m_psaSysInfoStrings;
	
	vcBaseVisionAlign *m_pVisionASM;
	CDlgSysEnvironSetting	*m_pParent;

	//CImageList *ImageListTree;
	//HICON m_icon[1];
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeAuthorizeView;
	CTree<CPermissionEntries*> m_cTreeAuthorizeShow;
	afx_msg void OnDestroy();
};
