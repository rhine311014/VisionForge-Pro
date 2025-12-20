#pragma once

#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"

// CDlgSysKindAlignCalibTarget 对话框

class CDlgSysKindAlignCalibTarget : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindAlignCalibTarget)

public:
	CDlgSysKindAlignCalibTarget(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindAlignCalibTarget();

// 对话框数据
	enum { IDD = IDD_SYS_KIND_ALIGN_CALIB_TARGET };

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
	std::vector<VpGuiDisplay>	m_vvpObjectGuiDisplay;
	std::vector<CLabel*>		m_vpLabelPos;
	std::vector<CXPStyleButtonST*>		m_vpBtnCopyObjectToCalib;
	CString m_strModuleDir;
	BOOL m_bProdPatternChanged;
	BOOL m_bProdNameChanged;
	BOOL m_bProdBenchMarkChanged;
	BOOL m_bProdNewCreate;
	BOOL m_bCalibInfoChanged;

	CString m_strProductNameOld;

	BOOL m_bCopyObjectToCalib;

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

	void CopyObjectToCalib(int nObjectPosIndex); // 拷贝到校正

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

public:
	int		m_nPlatformIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nPrevPlatformIdx;
	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;
	std::vector<CTotalProductData*> m_vpTotalProductData;

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
	CXPStyleButtonST	m_btnCopyObjectToCalib0;
	CXPStyleButtonST	m_btnCopyObjectToCalib1;
	CXPStyleButtonST	m_btnCopyObjectToCalib2;
	CXPStyleButtonST	m_btnCopyObjectToCalib3;
	CXPStyleButtonST	m_btnCopyObjectToCalib4;
	CXPStyleButtonST	m_btnCopyObjectToCalib5;
	CXPStyleButtonST	m_btnCopyObjectToCalib6;
	CXPStyleButtonST	m_btnCopyObjectToCalib7;
	CComboBox	m_comboAlignerOffsetCoordType;
	CString	m_strAlignerPrecisionX;
	CString	m_strAlignerPrecisionY;
	CString	m_strAlignerPrecisionD;
	CString	m_strAlignerOffsetX;
	CString	m_strAlignerOffsetY;
	CString	m_strAlignerOffsetD;


	CString	m_strObjectCandidateOffsetX1;
	CString	m_strObjectCandidateOffsetY1;
	CString	m_strObjectCandidateOffsetD1;
	CString	m_strObjectCandidateOffsetX2;
	CString	m_strObjectCandidateOffsetY2;
	CString	m_strObjectCandidateOffsetD2;
	CString	m_strObjectCandidateOffsetX3;
	CString	m_strObjectCandidateOffsetY3;
	CString	m_strObjectCandidateOffsetD3;
	CString	m_strObjectCandidateOffsetX4;
	CString	m_strObjectCandidateOffsetY4;
	CString	m_strObjectCandidateOffsetD4;

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
	afx_msg void OnSetfocusEditObjectCandidateDOffset1();
	afx_msg void OnSetfocusEditObjectCandidateXOffset1();
	afx_msg void OnSetfocusEditObjectCandidateYOffset1();
	afx_msg void OnSetfocusEditObjectCandidateDOffset2();
	afx_msg void OnSetfocusEditObjectCandidateXOffset2();
	afx_msg void OnSetfocusEditObjectCandidateYOffset2();
	afx_msg void OnSetfocusEditObjectCandidateDOffset3();
	afx_msg void OnSetfocusEditObjectCandidateXOffset3();
	afx_msg void OnSetfocusEditObjectCandidateYOffset3();
	afx_msg void OnSetfocusEditObjectCandidateDOffset4();
	afx_msg void OnSetfocusEditObjectCandidateXOffset4();
	afx_msg void OnSetfocusEditObjectCandidateYOffset4();
	afx_msg void OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditAlignerXPrecision();
	afx_msg void OnKillfocusEditAlignerYPrecision();
	afx_msg void OnKillfocusEditAlignerDPrecision();
	afx_msg void OnKillfocusEditAlignerDOffset();
	afx_msg void OnKillfocusEditAlignerXOffset();
	afx_msg void OnKillfocusEditAlignerYOffset();
	afx_msg void OnKillfocusEditObjectCandidateDOffset1();
	afx_msg void OnKillfocusEditObjectCandidateXOffset1();
	afx_msg void OnKillfocusEditObjectCandidateYOffset1();
	afx_msg void OnKillfocusEditObjectCandidateDOffset2();
	afx_msg void OnKillfocusEditObjectCandidateXOffset2();
	afx_msg void OnKillfocusEditObjectCandidateYOffset2();
	afx_msg void OnKillfocusEditObjectCandidateDOffset3();
	afx_msg void OnKillfocusEditObjectCandidateXOffset3();
	afx_msg void OnKillfocusEditObjectCandidateYOffset3();
	afx_msg void OnKillfocusEditObjectCandidateDOffset4();
	afx_msg void OnKillfocusEditObjectCandidateXOffset4();
	afx_msg void OnKillfocusEditObjectCandidateYOffset4();
	afx_msg void OnKillfocusEditAlignerMaxTimes();
	afx_msg void OnClose();
	afx_msg void OnBtnCloneProduct();
	//int m_nVirtualObjectCandidateEnable;
	int m_nCalibTargetMarkEnable;
	CComboBox	m_comboCalibTargetMarkPatternMode;
	int m_nObjectCandidateEnable;
	int m_nObjectCandidateSync;
	BOOL m_bObjectCandidateOffsetEnable1;
	BOOL m_bObjectCandidateOffsetEnable2;
	BOOL m_bObjectCandidateOffsetEnable3;
	BOOL m_bObjectCandidateOffsetEnable4;
	CLabel	m_labelPos0;
	CLabel	m_labelPos1;
	CLabel	m_labelPos2;
	CLabel	m_labelPos3;
	CLabel	m_labelPos4;
	CLabel	m_labelPos5;
	CLabel	m_labelPos6;
	CLabel	m_labelPos7;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnCopyObjectToCalib0();
	afx_msg void OnBnClickedBtnCopyObjectToCalib1();
	afx_msg void OnBnClickedBtnCopyObjectToCalib2();
	afx_msg void OnBnClickedBtnCopyObjectToCalib3();
	afx_msg void OnBnClickedBtnCopyObjectToCalib4();
	afx_msg void OnBnClickedBtnCopyObjectToCalib5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnCopyObjectToCalib6();
	afx_msg void OnBnClickedBtnCopyObjectToCalib7();


	BOOL m_bObjectShutterEnable0;
	BOOL m_bObjectShutterEnable1;
	BOOL m_bObjectShutterEnable2;
	BOOL m_bObjectShutterEnable3;
	BOOL m_bObjectShutterEnable4;

	BOOL m_bObjectGainEnable0;
	BOOL m_bObjectGainEnable1;

	BOOL m_bObjectGammaEnable0;
	BOOL m_bObjectGammaEnable1;

	afx_msg void OnBnClickedCheckAlignerObjectcGainEnable();
	afx_msg void OnBnClickedCheckAlignerObjectcGammaEnable();
	void EnableAndShowControl(bool bEnable,bool bShow);

	afx_msg void OnBnClickedCheckAlignerObjectcShutterEnable();
	afx_msg void OnBnClickedRadioCalibTargetMarkChange();
	afx_msg void OnBnClickedRadioObjectcSearchModeChange();
	afx_msg void OnBnClickedRadioObjectcSearchSyncChange();
	afx_msg void OnCbnSelchangeComboCalibTargetMarkPatternMode();
	CString strCalibTargetMarkPatternModeOld;
	afx_msg void OnBnClickedCheckAlignerObjectcOffsetEnable1();
	afx_msg void OnBnClickedCheckAlignerObjectcOffsetEnable2();
	afx_msg void OnBnClickedCheckAlignerObjectcOffsetEnable3();
	afx_msg void OnBnClickedCheckAlignerObjectcOffsetEnable4();
	afx_msg void OnBnClickedBtnDmcodeParamSetting();
	BOOL m_bEnableOutLine; //是否允许二维码关联时忽略点
	int m_nOutLinePercent; //二维码关联时忽略点百分比
	afx_msg void OnBnClickedChkEnableOutline();
	afx_msg void OnEnSetfocusEditOutlinePercent();
	afx_msg void OnEnKillfocusEditOutlinePercent();

	void EnableWindowByAuthorizeset(bool bEnable);
};
