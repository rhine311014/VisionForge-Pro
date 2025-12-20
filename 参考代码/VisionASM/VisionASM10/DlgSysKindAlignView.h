#pragma once
#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"
#include "LogFileSave.h"
#include "afxcmn.h"
#include "afxwin.h"
#define WM_HOTKEY_CTL_P WM_USER+6000
#include "vsBaseVisionAlign.h"
// CDlgSysKindAlignView 对话框

class CDlgSysKind;
class CDlgSysKindAlignView : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindAlignView)

public:
	CDlgSysKindAlignView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindAlignView();

// 对话框数据
	enum { IDD = IDD_SYS_KIND_ALIGN_VIEW };

protected:
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

	std::vector<VpGuiRect>		m_vvpGuiSelectRect;
	std::vector<VpGuiDisplay>	m_vvpObjectGuiDisplay;
	std::vector<VpGuiDisplay>	m_vvpTargetGuiDisplay;
	std::vector<int>			m_vpObjectPatSel;
	std::vector<int>			m_vpTargetPatSel;
	std::vector<CLabel*>		m_vpLabelPos;

	CString m_strModuleDir;
	BOOL m_bProdPatternChanged;
	BOOL m_bProdNameChanged;
	BOOL m_bProdBenchMarkChanged;
	BOOL m_bProdNewCreate;
	BOOL m_bCalibInfoChanged;

	CString m_strProductNameOld;
	CString m_strProductNameOldFa;	//zzc初始化得到的产品名称，当大小产品时，为大产品名称

	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CFont           m_fontChangeProduct;

	void CheckDlgData();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);


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
	BOOL SaveSearchToolInfo();
	
public:
	BOOL IsCalibratedInfosEqual(std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2);
	BOOL IsProductChanged();	// 检查
	BOOL IsSearchToolSelected(CBaseSearchTool* pSearchTool);  //判断定位工具是否选中，若否，则不显示使用该定位工具制作模板时所保存的图片
	void UpdateDialogLanguage();
	BOOL SaveSearchTool(SearchMode searchMode,CString strDir, std::vector<VpSearchTool> vvpSearchTool,std::vector<VbSearchToolChanged> vvbSearchToolChanged);

public:
	int		m_nPlatformIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nOldProductIdx;
	int m_nPrevPlatformIdx;
	int m_nSrcProductIndex;
	int m_nDstProductIndex;
	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;
	std::vector<CTotalProductData*> m_vpTotalProductData;
	int m_nSrcProductIndexFa;
	int m_nDstProductIndexFa;
	map<int,CRect> m_InitPosition;	//一部分控件的初始位置
		
	void UpdateCalibDlgData(BOOL bSaveAndValidate = TRUE);
	void UpdateDlgTempData(BOOL bSaveAndValidate = TRUE);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL IsNameUnique(LPCTSTR lpszName);	// 检查产品名称是否唯一

	BOOL OnMyOK();
	BOOL OnMyCancel();
	BOOL CancelProductChange();
	void SetCurProductIndex(int nCurProdcutIndex, bool bToastInfo=true, bool bSaveProduct=true);
	BOOL CheckMemUseRateWarning(int nDstProductIndex);

	void SetProductListSelect(int nProductFa, int nProductChd);		// 设置产品列表选择
	void ResetProductChangeStatus();							    // 重置产品修改标志
	void RestoreProductModify();									// 恢复产品修改

	void FreePreProductMem(int nCurProductIndex, int nPreProductIndex, int nPlatformIndex);		// 释放上一个产品内存
	CString GetProductChangeTipText(int nCurProIndex, int nPreProIndex, bool bStart);	        // 获取产品切换时提示文字

	void SetCurPlatformIndex(int nCurPlatformIndex);      // 设置当前产品索引

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CListCtrl	m_listctrlProduct;
// 	CXPStyleButtonST	m_btnOK;
// 	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnNewProduct;
	CXPStyleButtonST	m_btnNewProductMultiple;
	CXPStyleButtonST	m_btnDeleteProduct;
	CXPStyleButtonST    m_btnClearProduct;
	CXPStyleButtonST	m_btnRename;
	CXPStyleButtonST	m_btnAmendBenchMark;
// 	CXPStyleButtonST	m_btnShowPlatform1;
// 	CXPStyleButtonST	m_btnShowPlatform2;
// 	CXPStyleButtonST	m_btnShowPlatform3;
// 	CXPStyleButtonST	m_btnShowPlatform4;
//	CXPStyleButtonST	m_btnPanel;
	CComboBox	m_comboAlignerOffsetCoordType;
	CComboBox	m_comboProductSrc;
	CComboBox	m_comboProductDst;
	CString	m_strAlignerPrecisionX;
	CString	m_strAlignerPrecisionY;
	CString	m_strAlignerPrecisionD;
	CString	m_strAlignerOffsetX;
	CString	m_strAlignerOffsetY;
	CString	m_strAlignerOffsetD;
	int		m_nAlignerMaxTimes;
	CString	m_strCurProductName;
	CString m_strCurCalibName;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnProductRename();
	afx_msg void OnBtnAmendBenchmark();
	afx_msg void OnPlatformChange();
	afx_msg void OnButtonNewProduct();
	afx_msg void OnButtonDeleteCurProduct();
	void OnButtonDeleteCurProduct(int nIndex);
	void OnBnClickedButtonClearProduct(int nIndex);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetfocusEditNewProductNum();
	afx_msg void OnKillfocusEditNewProductNum();
	afx_msg void OnSetfocusEditAlignerMaxTimes();
	afx_msg void OnSetfocusEditAlignerXPrecision();
	afx_msg void OnSetfocusEditAlignerYPrecision();
	afx_msg void OnSetfocusEditAlignerDPrecision();
	afx_msg void OnSetfocusEditAlignerDOffset();
	afx_msg void OnSetfocusEditAlignerXOffset();
	afx_msg void OnSetfocusEditAlignerYOffset();
	afx_msg void OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditAlignerXPrecision();
	afx_msg void OnKillfocusEditAlignerYPrecision();
	afx_msg void OnKillfocusEditAlignerDPrecision();
	afx_msg void OnKillfocusEditAlignerDOffset();
	afx_msg void OnKillfocusEditAlignerXOffset();
	afx_msg void OnKillfocusEditAlignerYOffset();
	afx_msg void OnKillfocusEditAlignerMaxTimes();
	afx_msg void OnClose();
	afx_msg void OnBtnCloneProduct();
	int m_nSearchDelayTime;
	CLabel	m_labelPos0;
	CLabel	m_labelPos1;
	CLabel	m_labelPos2;
	CLabel	m_labelPos3;
	CLabel	m_labelPos4;
	CLabel	m_labelPos5;
	CLabel	m_labelPos6;
	CLabel	m_labelPos7;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void CopyProductData(int nSrcProductIndex, int nDstProductIndex);
	void CopyProductCalibData(int nSrcProductIndex,int nDstProductIndex);
	void NewProduct(int nNewProductIndexFa, int nNewProductIndexChd);
	//拷贝产品时同时拷贝SearchData.xml文件和Camera.xml文件
	BOOL CopyXMLDataFile(int nSrcProductIndex,int nDstProductIndex);
	BOOL CopyCalibXMLDataFile(int nSrcProductIndex,int nDstProductIndex);
	afx_msg void OnBnClickedBtnCopyProductData();
	afx_msg void OnCbnSelchangeComboProductSrc();
	afx_msg void OnCbnSelchangeComboProductDst();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nNewProductNum;
	afx_msg void OnBnClickedButtonNewProductMultiple();
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonClearProduct();

	CListCtrl m_listctrlProductFa;
	afx_msg void OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult);

	CComboBox m_comboProductSrcFa;
	CComboBox m_comboProductDstFa;
	afx_msg void OnCbnSelchangeComboProductSrcFa();
	afx_msg void OnCbnSelchangeComboProductDstFa();

	afx_msg void OnBtnProductRenameFa();
	afx_msg void OnButtonNewProductFa();
	afx_msg void OnBnClickedBtnCopyProductDataFa();

	void ClipMouseCursor(BOOL bClip);

	CString m_strCurProductNameFa;
	CString m_strCurCalibNameFa;
	BOOL LoadSearchToolInfo();

	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用
	bool m_bShowProductList;
	bool m_bEnableProductList;
};
