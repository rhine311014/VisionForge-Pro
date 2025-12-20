#pragma once

#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsBaseVisionAlign.h"
#include "vsVisionAlignDef.h"
#include "afxwin.h"

#include "DlgSysKindAlignView.h"
#include "DlgSysKindAlignSetting.h"
#include "DlgSysKindAlignTarget.h"
#include "DlgSysKindAlignObject.h"
#include "DlgSysKindAlignVirtualObject.h"
#include "DlgSysKindAlignCalibTarget.h"

#include "DlgSysKindCalibSetting.h"
#include "DlgSysKindAlignInspect.h"

#include "LogFileSave.h"
//#define TABSHEET_ITEM_ALIGN_VIEW		_T("产品浏览")
//#define TABSHEET_ITEM_ALIGN_SETTING		_T("产品设置")
//#define TABSHEET_ITEM_ALIGN_TARGET		_T("目标模板设置")
//#define TABSHEET_ITEM_ALIGN_OBJECT		_T("对象模板设置")
//#define TABSHEET_ITEM_ALIGN_VIRTUAL_OBJECT	_T("虚拟对象模板设置")

//#define IDS_COM_STR_MARK_PLATFORM_COORD _T("Mark及平台坐标")

// #define IDS_COM_STR_ALIGNER_4VS4_TYPE_POS1234VS5678		_T("位置1234对5678")
// #define IDS_COM_STR_ALIGNER_4VS4_TYPE_POS123VS567		_T("位置123对567")
// #define IDS_COM_STR_ALIGNER_4VS4_TYPE_POS124VS568		_T("位置124对568")
// #define IDS_COM_STR_ALIGNER_4VS4_TYPE_POS134VS578		_T("位置134对578")
// #define IDS_COM_STR_ALIGNER_4VS4_TYPE_POS234VS678		_T("位置234对678")

#define MaxPlatformNum	6	// 最大的平台数量

// CDlgSysKind 对话框

class CDlgSysKind : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKind)

public:
	CDlgSysKind(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKind();

// 对话框数据
	enum { IDD = IDD_SYS_KIND };


public:
	CThemeHelperST		m_themeHelper;
	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;

	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CString m_strModuleDir;

public:
	int m_nPrevTabIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nPrevPlatformIdx;
	CVisionASMDlg *m_pParent;
	std::vector<CTotalProductData*> m_vpTotalProductData;
	std::vector<CTotalCalibData*> m_vpTotalCalibData;



	BOOL	m_bSinglePlatformProductManageEnable;

	//记录当前产品模板数据是否被改变（暂时未添加标定模板修改标识符）
	std::vector<VbSearchToolChanged> m_vvbAlnObjectSearchToolChanged;			// 对象定位工具是否被改变
	std::vector<VbSearchToolChanged> m_vvbAlnTargetSearchToolChanged;			// 目标定位工具是否被改变
	std::vector<VbSearchToolChanged> m_vvbAlnVirtualObjectSearchToolChanged;	// 虚拟定位工具是否被改变
	std::vector<VbSearchToolChanged> m_vvbAlnCalibTargetSearchToolChanged;		// 关联定位工具是否被改变
	std::vector<VbSearchToolChanged> m_vvbAlnInspectSearchToolChanged;			// 检测定位工具是否被改变
	std::vector<VbSearchToolChanged> m_vvbAlnCalibObjectSearchToolChanged;		// 标定定位工具是否被改变

	bool InitSearchToolChanged();											// 初始化所有定位工具修改标识符
	bool SetProductSearchToolChanged(bool bSetState);						// 成批设置所有定位工具修改标识符状态
	bool SetCalibObjectSearchToolChanged(bool bSetState);					// 成批设置标定定位工具修改标识符状态
	
	
	void InitDlgItem();
	void MoveDlgItem();
	void UpdateStatusBar();
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();

public:
	CDlgSysKindAlignView			*m_pDlgKindAlignView;
	CDlgSysKindAlignSetting			*m_pDlgKindAlignSetting;
	CDlgSysKindAlignTarget			*m_pDlgKindAlignTarget;
	CDlgSysKindAlignObject			*m_pDlgKindAlignObject;
	CDlgSysKindAlignVirtualObject	*m_pDlgKindAlignVirtualObject;
	CDlgSysKindAlignCalibTarget		*m_pDlgKindAlignCalibTarget;
	CDlgSysKindAlignInspect			*m_pDlgKindAlignInspect;
	CDlgSysKindCalibSetting			*m_pDlgKindCalibSetting;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnApply;
	CXPStyleButtonST	m_btnShowPlatform1;
	CXPStyleButtonST	m_btnShowPlatform2;
	CXPStyleButtonST	m_btnShowPlatform3;
	CXPStyleButtonST	m_btnShowPlatform4;
	CXPStyleButtonST	m_btnShowPlatform5;
	CXPStyleButtonST	m_btnShowPlatform6;
	CXPStyleButtonST	m_btnPanel;
	CXPStyleButtonST	m_btnAlign;
	CXPStyleButtonST	m_btnCalib;
	CXPStyleButtonST	m_btnKindSetting;
	CXPStyleButtonST	m_btnKindTarget;
	CXPStyleButtonST	m_btnKindObject;
	CXPStyleButtonST	m_btnKindVirtualObject;
	CXPStyleButtonST	m_btnKindCalibTarget;
	CXPStyleButtonST	m_btnKindInspect;


	CTabCtrl			m_tabSheet;
	afx_msg void OnTcnSelchangeTabKindAlign(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioAlign();
	afx_msg void OnBnClickedRadioCalib();
	afx_msg void OnBnClickedRadioKind();

	afx_msg void OnPlatformChange();
	int m_nRadioKind;
	int m_nPreRadioKind;
	int m_nPlatformIndex;
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnBnClickedBtnPanel();
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedBtnApply();

	//检测子对话框是否创建，如果未创建，进行创建
	void CheckDlgAlignSettingIsCreated();
	void CheckDlgAlignTargetIsCreated();
	void CheckDlgAlignObjectIsCreated();
	void CheckDlgAlignVirtualObjectIsCreated();
	void CheckDlgAlignCalibTargetIsCreated();
	void CheckDlgAlignInspectIsCreated();
	void CheckAllDlgIsCreated();
	BOOL m_bInitCreateDlg;

	int flag1,flag2;
	BOOL LoadSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> &vvpSearchTool,std::vector<VbSearchToolChanged> vvbSearchToolChanged);
	BOOL LoadSearchToolInfo();
	CString GetOptionText(int nOptIndex);

	bool m_bShowTargetTemplateSet;
	bool m_bEnableTargetTemplateSet;

	bool m_bShowObjectTemplateSet;
	bool m_bEnableObjectTemplateSet;

	bool m_bShowVirtualObjectTemplateSet;
	bool m_bEnableVirtualObjectTemplateSet;

	bool m_bShowCalibTargetTemplateSet;
	bool m_bEnableCalibTargetTemplateSet;

	bool m_bShowInspectTemplateSet;
	bool m_bEnableInspectTemplateSet;

	CRect m_rtView;


public:
	
// 	// 释放产品内存
// 	bool FreeProductMem(int nPlatformIdx, int nProductIdx);
// 
// 
// 	//// 保存产品数据+标定数据+概要信息
// 	bool SaveProduct(int nPlatformIdx, int nProductIdx);
// 	// 保存产品数据
// 	bool SaveAlignProduct(int nPlatformIdx, int nProductIdx);
// 	// 保存产品参数数据
// 	bool  SaveAlignProductData(int nPlatformIdx, int nProductIdx);
// 	// 保存产品模板数据
// 	bool  SaveAlignProductSearchTool(int nPlatformIdx, int nProductIdx);
// 	// 保存所有产品概要文件
// 	bool SaveAlignProductTotalData(int nPlatfromIdx, int nCurProductIdx); 
// 	// 保存产品数据
// 	bool SaveCalibProduct(int nPlatformIdx, int nProductIdx);
// 	// 保存产品参数数据
// 	bool  SaveCalibProductData(int nPlatformIdx, int nProductIdx);
// 	// 保存产品模板数据
// 	bool  SaveCalibProductSearchTool(int nPlatformIdx, int nProductIdx);
// 	// 保存所有产品概要文件
// 	bool SaveCalibProductTotalData(int nPlatfromIdx, int nCurProductIdx); 
// 	// 保存定位工具
// 	bool SaveSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> vvpSearchTool,std::vector<VbSearchToolChanged> vvbSearchToolChanged);
	afx_msg void OnBnClickedOk();
};
