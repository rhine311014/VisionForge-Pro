#pragma once
#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"
#include "LogFileSave.h"
// CDlgSysKindAlignTarget 对话框
class CDlgSysKind;
class CDlgSysKindAlignTarget : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindAlignTarget)

public:
	CDlgSysKindAlignTarget(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindAlignTarget();

// 对话框数据
	enum { IDD = IDD_SYS_KIND_ALIGN_TARGET };
public:
	CThemeHelperST		m_themeHelper;
	
// 	// 状态栏相关
// 	CStatusBarCtrl *m_pStatusBar;
// 	BOOL m_bStatusBarFlag;
// 	CString m_strStatusBarInfo1;
// 	CString m_strStatusBarInfo2;
// 	HICON m_hIconSucceed;
// 	HICON m_hIconFailed;

	std::vector<scGuiDisplay*> m_vpTargetGuiDisplay;
	std::vector<scGuiDisplay*> m_vpObjectGuiDisplay;
	std::vector<scGuiDisplay*> m_vpVirtualObjectGuiDisplay;
	std::vector<VpGuiDisplay>	m_vvpTargetGuiDisplay;
	std::vector<CLabel*>		m_vpLabelPos;
	CString m_strModuleDir;
	BOOL m_bProdPatternChanged;
	BOOL m_bProdNameChanged;
	BOOL m_bProdBenchMarkChanged;
	BOOL m_bProdNewCreate;
	BOOL m_bCalibInfoChanged;

	CString m_strProductNameOld;

	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	void CheckDlgData();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void GetDlgTempData(CProductData& product);
 	void InitDlgItem();
	void MoveDlgItem();
	void UpdateProductListDisplay();

	CString GetNewDefName();
	BOOL IsNameLegal(LPCTSTR lpszName);
	BOOL IsStringNumerical(LPCTSTR lpszVal);

	/************************************************************************************
	/* Function name	: CreateDirectoryPro
	/* Description	    : 创建目录，父目录不要求一定存在。如果父目录不存在，则依次创建之。
	/* Return type		: BOOL 
	/* Argument
	/* [in] --  LPCTSTR lpszDir:	目录名
	/************************************************************************************/
	BOOL CreateDirectoryPro(LPCTSTR lpszDir);
	BOOL ClearDirectoryPro(LPCTSTR lpszDir);	// 清空目录
	BOOL RemoveDirectoryPro(LPCTSTR lpszDir);	// 删除非空目录
	BOOL RenameDirecotryPro(LPCTSTR lpszOldDir, LPCTSTR lpszNewDir);

	BOOL SavePrevProduct(); // 保存前一个产品的设置
	void UpdateStatusBar();

	BOOL IsCalibratedInfosEqual(std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2);
	BOOL IsProductChanged();	// 检查
	BOOL IsSearchToolSelected(CBaseSearchTool* pSearchTool);  //判断定位工具是否选中，若否，则不显示使用该定位工具制作模板时所保存的图片
	void UpdateDialogLanguage();
// 
public:
	int		m_nPlatformIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nPrevPlatformIdx;
	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;
//	std::vector<CTotalProductData*> m_vpTotalProductData;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL IsNameUnique(LPCTSTR lpszName);	// 检查产品名称是否唯一
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CListCtrl	m_listctrlProduct;
	// 	CXPStyleButtonST	m_btnOK;
	// 	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnNewProduct;
	CXPStyleButtonST	m_btnDeleteProduct;
	CXPStyleButtonST	m_btnRename;
	CXPStyleButtonST	m_btnAmendBenchMark;
	// 	CXPStyleButtonST	m_btnShowPlatform1;
	// 	CXPStyleButtonST	m_btnShowPlatform2;
	// 	CXPStyleButtonST	m_btnShowPlatform3;
	// 	CXPStyleButtonST	m_btnShowPlatform4;
	//	CXPStyleButtonST	m_btnPanel;
	CComboBox	m_comboAlignerOffsetCoordType;
	CString	m_strAlignerPrecisionX;
	CString	m_strAlignerPrecisionY;
	CString	m_strAlignerPrecisionD;
	CString	m_strAlignerOffsetX;
	CString	m_strAlignerOffsetY;
	CString	m_strAlignerOffsetD;

	CString	m_strTargetCandidateOffsetX1;
	CString	m_strTargetCandidateOffsetY1;
	CString	m_strTargetCandidateOffsetD1;
	CString	m_strTargetCandidateOffsetX2;
	CString	m_strTargetCandidateOffsetY2;
	CString	m_strTargetCandidateOffsetD2;
	CString	m_strTargetCandidateOffsetX3;
	CString	m_strTargetCandidateOffsetY3;
	CString	m_strTargetCandidateOffsetD3;
	CString	m_strTargetCandidateOffsetX4;
	CString	m_strTargetCandidateOffsetY4;
	CString	m_strTargetCandidateOffsetD4;

	int		m_nAlignerMaxTimes;
	CString	m_strCurProductName;
public:
	virtual BOOL OnInitDialog();
 	virtual BOOL DestroyWindow();
 	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnProductRename();
	afx_msg void OnBtnAmendBenchmark();
	afx_msg void OnPlatformChange();
	afx_msg void OnButtonNewProduct();
	afx_msg void OnButtonDeleteCurProduct();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetfocusEditAlignerMaxTimes();
	afx_msg void OnSetfocusEditAlignerXPrecision();
	afx_msg void OnSetfocusEditAlignerYPrecision();
	afx_msg void OnSetfocusEditAlignerDPrecision();
	afx_msg void OnSetfocusEditAlignerDOffset();
	afx_msg void OnSetfocusEditAlignerXOffset();
	afx_msg void OnSetfocusEditAlignerYOffset();
	afx_msg void OnSetfocusEditTargetCandidateDOffset1();
	afx_msg void OnSetfocusEditTargetCandidateXOffset1();
	afx_msg void OnSetfocusEditTargetCandidateYOffset1();
	afx_msg void OnSetfocusEditTargetCandidateDOffset2();
	afx_msg void OnSetfocusEditTargetCandidateXOffset2();
	afx_msg void OnSetfocusEditTargetCandidateYOffset2();
	afx_msg void OnSetfocusEditTargetCandidateDOffset3();
	afx_msg void OnSetfocusEditTargetCandidateXOffset3();
	afx_msg void OnSetfocusEditTargetCandidateYOffset3();
	afx_msg void OnSetfocusEditTargetCandidateDOffset4();
	afx_msg void OnSetfocusEditTargetCandidateXOffset4();
	afx_msg void OnSetfocusEditTargetCandidateYOffset4();
	afx_msg void OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditAlignerXPrecision();
	afx_msg void OnKillfocusEditAlignerYPrecision();
	afx_msg void OnKillfocusEditAlignerDPrecision();
	afx_msg void OnKillfocusEditAlignerDOffset();
	afx_msg void OnKillfocusEditAlignerXOffset();
	afx_msg void OnKillfocusEditAlignerYOffset();
	afx_msg void OnKillfocusEditTargetCandidateDOffset1();
	afx_msg void OnKillfocusEditTargetCandidateXOffset1();
	afx_msg void OnKillfocusEditTargetCandidateYOffset1();
	afx_msg void OnKillfocusEditTargetCandidateDOffset2();
	afx_msg void OnKillfocusEditTargetCandidateXOffset2();
	afx_msg void OnKillfocusEditTargetCandidateYOffset2();
	afx_msg void OnKillfocusEditTargetCandidateDOffset3();
	afx_msg void OnKillfocusEditTargetCandidateXOffset3();
	afx_msg void OnKillfocusEditTargetCandidateYOffset3();
	afx_msg void OnKillfocusEditTargetCandidateDOffset4();
	afx_msg void OnKillfocusEditTargetCandidateXOffset4();
	afx_msg void OnKillfocusEditTargetCandidateYOffset4();
	afx_msg void OnKillfocusEditAlignerMaxTimes();
	afx_msg void OnClose();
	afx_msg void OnBtnCloneProduct();
	int m_nTargetCandidateEnable;
	int m_nTargetCandidateSync;
	BOOL m_bTargetCandidateOffsetEnable1;
	BOOL m_bTargetCandidateOffsetEnable2;
	BOOL m_bTargetCandidateOffsetEnable3;
	BOOL m_bTargetCandidateOffsetEnable4;
	CLabel	m_labelPos0;
	CLabel	m_labelPos1;
	CLabel	m_labelPos2;
	CLabel	m_labelPos3;
	CLabel	m_labelPos4;
	CLabel	m_labelPos5;
	CLabel	m_labelPos6;
	CLabel	m_labelPos7;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL m_bTargetShutterEnable0;
	BOOL m_bTargetShutterEnable1;
	BOOL m_bTargetShutterEnable2;
	BOOL m_bTargetShutterEnable3;
	BOOL m_bTargetShutterEnable4;
	
	BOOL m_bTargetGainEnable0;
	BOOL m_bTargetGainEnable1;
	BOOL m_bTargetGainEnable2;
	BOOL m_bTargetGainEnable3;
	BOOL m_bTargetGainEnable4;

	BOOL m_bTargetGammaEnable0;
	BOOL m_bTargetGammaEnable1;
	BOOL m_bTargetGammaEnable2;
	BOOL m_bTargetGammaEnable3;
	BOOL m_bTargetGammaEnable4;

	afx_msg void OnBnClickedCheckAlignerTargetcShutterEnable();
	afx_msg void OnBnClickedRadioTargetcSearchModeChange();
	afx_msg void OnBnClickedRadioTargetcSearchSyncChange();
	afx_msg void OnBnClickedCheckAlignerTargetcOffsetEnable1();
	afx_msg void OnBnClickedCheckAlignerTargetcOffsetEnable2();
	afx_msg void OnBnClickedCheckAlignerTargetcOffsetEnable3();
	afx_msg void OnBnClickedCheckAlignerTargetcOffsetEnable4();

	afx_msg void OnBnClickedCheckAlignerTargetcGainEnable();
	afx_msg void OnBnClickedCheckAlignerTargetcGammaEnable();

	void EnableAndShowControl(bool bEnable,bool bShow);

	void EnableWindowByAuthorizeset(bool bEnable);
};
