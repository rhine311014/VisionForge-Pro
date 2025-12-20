#pragma once
#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"
#include "afxwin.h"
#include "afxcmn.h"

//#define IDS_MB_SAVE_CHANGE_TO_CALIB_OR_NOT _T("是否保存标定数据的修改？")

// CDlgSysKindCalibSetting 对话框
class CDlgSysKind;
class CDlgSysKindCalibSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindCalibSetting)

public:
	CDlgSysKindCalibSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindCalibSetting();

// 对话框数据
	enum { IDD = IDD_SYS_KIND_CALIB_SETTING };

	
protected:
	CThemeHelperST		m_themeHelper;
	
// 	// 状态栏相关
// 	CStatusBarCtrl *m_pStatusBar;
// 	BOOL m_bStatusBarFlag;
// 	CString m_strStatusBarInfo1;
// 	CString m_strStatusBarInfo2;
// 	HICON m_hIconSucceed;
// 	HICON m_hIconFailed;

//	std::vector<scGuiDisplay*> m_vpTargetGuiDisplay;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;
//	std::vector<scGuiDisplay*> m_vpVirtualObjectGuiDisplay;

// 	std::vector<VpGuiRect>		m_vvpGuiSelectRect;
// 	std::vector<VpGuiDisplay>	m_vvpObjectGuiDisplay;
// 	std::vector<VpGuiDisplay>	m_vvpTargetGuiDisplay;
// 	std::vector<int>			m_vpObjectPatSel;
// 	std::vector<int>			m_vpTargetPatSel;
	std::vector<CLabel*>		m_vpLabelPos;
	std::vector<CButton*>		m_vpRadioPos;
	std::vector<CButton*>		m_vpCheckPos;

	CString m_strModuleDir;
	BOOL m_bProdPatternChanged;
	BOOL m_bProdNameChanged;
	BOOL m_bProdBenchMarkChanged;
	BOOL m_bProdNewCreate;
	BOOL m_bCalibInfoChanged;

	CString m_strProductNameOld;
	CString m_strProductNameOldFa;

	std::vector<CString*> m_vstrCalibMovementX;
	std::vector<CString*> m_vstrCalibMovementY;
	std::vector<CString*> m_vstrCalibMovementD;
	std::vector<CString*> m_vstrCamCalibMovementX;
	std::vector<CString*> m_vstrCamCalibMovementY;
	std::vector<CString*> m_vstrCalibRefineMoveD;
	std::vector<CString*> m_vstrCalibRefineTime;
	std::vector<CString*> m_vstrSearchDelayTime;

	std::vector<int*> m_vnEnableDisCor;
	std::vector<double> m_vdCalibTileX;
	std::vector<double> m_vdCalibTileY;
	std::vector<bool> m_vbEnableOutPoints;
	std::vector<int> m_vnThre;
	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	void CheckDlgData();

	void InitDlgItem();
	void MoveDlgItem();
	//void UpdateProductListDisplay();
	void UpdateProductComboDisplay();



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

public:
	BOOL SavePrevProduct(); // 保存前一个产品的设置
	void UpdateStatusBar();
	BOOL SaveSearchToolInfo();
public:
	void UpdateProductListDisplay();
	BOOL IsCalibratedInfosEqual(std::vector<CCalibratedInfo*> vpCalibratedInfo1, std::vector<CCalibratedInfo*> vpCalibratedInfo2);
	BOOL IsProductChanged();	// 检查
	BOOL IsSearchToolSelected(CBaseSearchTool* pSearchTool);  //判断定位工具是否选中，若否，则不显示使用该定位工具制作模板时所保存的图片
	void UpdateDialogLanguage();

	void SaveCalibSearchImage(CString strImageFileDirt,CString strImageName,cpImage & imageSave);
	bool CreateMultipleDirectory(CString& szPath, char split);
	bool SaveCSVFile(CString strImageFileDirt,CString strFileName,CString& strSaveData );

public:
	int		m_nPlatformIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nPrevPlatformIdx;
	int m_nSrcProductIndex;
	int m_nDstProductIndex;
	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;
	std::vector<CTotalCalibData*> m_vpTotalCalibData;

	int m_nSrcProductIndexFa;
	int m_nDstProductIndexFa;
	map<int,CRect> m_InitPosition;

	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void UpdateDlgTempData(BOOL bSaveAndValidate = TRUE);
	void UpdateDlgPatternData(BOOL bSaveAndValidate = TRUE);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL IsNameUnique(LPCTSTR lpszName);	// 检查产品名称是否唯一

	void SetCurProductIndex(int nCurProdcutIndex);
	void CreateNewProduct(int nNewProductIndexFa, int nNewProductIndexChd);
	void CreateNewProductMulitple(int nNewProductIndex, int nNewProductNum);
	void DeleteCurProduct(int nCurProductIndex);
	void ClearCurProduct(int nCurProductIndex);
	void SetComboCalibCopySrcIndex(int nSrc);
	void SetComboCalibCopyDstIndex(int nDst);

	BOOL OnMyOK();
	BOOL OnMyCancel();
	BOOL CancelProductChange();

	void SetProductListSelect(int nProductFa, int nProductChd);
	void ResetProductChangeStatus();
	void RestoreProductModify();
	void FreePreProductMem(int nCurProductIndex, int nPreProductIndex, int nPlatformIndex);

	void SetCurProductInfo();

	void SetProductChangeTipInfoStart(const CString& strTipInfo);
	void SetProductChangeTipInfoEnd(const CString& strTipInfo);
	void SetCalibProductName(const CString& strName);
	void SetCalibProductNameFa(const CString& strName);



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
//	CComboBox	m_comboAlignerOffsetCoordType;
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
// 	CString	m_strCalibMovementX;
// 	CString	m_strCalibMovementY;
// 	CString	m_strCalibMovementD;
// 	CString	m_strCamCalibMovementX;
// 	CString	m_strCamCalibMovementY;
// 	CString	m_strCalibRefineMoveD;
// 	CString	m_strCalibRefineTime;
// 	CString	m_strSearchDelayTime;
	CString	m_strCalibMovementX1;
	CString	m_strCalibMovementY1;
	CString	m_strCalibMovementD1;
	CString	m_strCamCalibMovementX1;
	CString	m_strCamCalibMovementY1;
	CString	m_strCalibRefineMoveD1;
	CString	m_strCalibRefineTime1;
	CString	m_strSearchDelayTime1;
	CString	m_strCalibMovementX2;
	CString	m_strCalibMovementY2;
	CString	m_strCalibMovementD2;
	CString	m_strCamCalibMovementX2;
	CString	m_strCamCalibMovementY2;
	CString	m_strCalibRefineMoveD2;
	CString	m_strCalibRefineTime2;
	CString	m_strSearchDelayTime2;
	CString	m_strCalibMovementX3;
	CString	m_strCalibMovementY3;
	CString	m_strCalibMovementD3;
	CString	m_strCamCalibMovementX3;
	CString	m_strCamCalibMovementY3;
	CString	m_strCalibRefineMoveD3;
	CString	m_strCalibRefineTime3;
	CString	m_strSearchDelayTime3;
	CString	m_strCalibMovementX4;
	CString	m_strCalibMovementY4;
	CString	m_strCalibMovementD4;
	CString	m_strCamCalibMovementX4;
	CString	m_strCamCalibMovementY4;
	CString	m_strCalibRefineMoveD4;
	CString	m_strCalibRefineTime4;
	CString	m_strSearchDelayTime4;
	CString	m_strCalibMovementX5;
	CString	m_strCalibMovementY5;
	CString	m_strCalibMovementD5;
	CString	m_strCamCalibMovementX5;
	CString	m_strCamCalibMovementY5;
	CString	m_strCalibRefineMoveD5;
	CString	m_strCalibRefineTime5;
	CString	m_strSearchDelayTime5;
	CString	m_strCalibMovementX6;
	CString	m_strCalibMovementY6;
	CString	m_strCalibMovementD6;
	CString	m_strCamCalibMovementX6;
	CString	m_strCamCalibMovementY6;
	CString	m_strCalibRefineMoveD6;
	CString	m_strCalibRefineTime6;
	CString	m_strSearchDelayTime6;
	CString	m_strCalibMovementX7;
	CString	m_strCalibMovementY7;
	CString	m_strCalibMovementD7;
	CString	m_strCamCalibMovementX7;
	CString	m_strCamCalibMovementY7;
	CString	m_strCalibRefineMoveD7;
	CString	m_strCalibRefineTime7;
	CString	m_strSearchDelayTime7;
	CString	m_strCalibMovementX8;
	CString	m_strCalibMovementY8;
	CString	m_strCalibMovementD8;
	CString	m_strCamCalibMovementX8;
	CString	m_strCamCalibMovementY8;
	CString	m_strCalibRefineMoveD8;
	CString	m_strCalibRefineTime8;
	CString	m_strSearchDelayTime8;

	int m_nEnableDisCorIndex1;
	int m_nEnableDisCorIndex2;
	int m_nEnableDisCorIndex3;
	int m_nEnableDisCorIndex4;
	int m_nEnableDisCorIndex5;
	int m_nEnableDisCorIndex6;
	int m_nEnableDisCorIndex7;
	int m_nEnableDisCorIndex8;

	BOOL	m_bCalibRefine;
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
	afx_msg void OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditAlignerXPrecision();
	afx_msg void OnKillfocusEditAlignerYPrecision();
	afx_msg void OnKillfocusEditAlignerDPrecision();
	afx_msg void OnKillfocusEditAlignerDOffset();
	afx_msg void OnKillfocusEditAlignerXOffset();
	afx_msg void OnKillfocusEditAlignerYOffset();
	afx_msg void OnKillfocusEditAlignerMaxTimes();

	afx_msg void OnSetfocusEditXAxisMovement1();
	afx_msg void OnSetfocusEditYAxisMovement1();
	afx_msg void OnSetfocusEditDAxisMovement1();
	afx_msg void OnSetfocusEditCameraXAxisMovement1();
	afx_msg void OnSetfocusEditCameraYAxisMovement1();
	afx_msg void OnSetfocusEditRefineDAxisMovement1();
	afx_msg void OnSetfocusEditRefineTime1();
	afx_msg void OnSetfocusEditSearchDelayTime1();
	afx_msg void OnSetfocusEditXAxisMovement2();
	afx_msg void OnSetfocusEditYAxisMovement2();
	afx_msg void OnSetfocusEditDAxisMovement2();
	afx_msg void OnSetfocusEditCameraXAxisMovement2();
	afx_msg void OnSetfocusEditCameraYAxisMovement2();
	afx_msg void OnSetfocusEditRefineDAxisMovement2();
	afx_msg void OnSetfocusEditRefineTime2();
	afx_msg void OnSetfocusEditSearchDelayTime2();
	afx_msg void OnSetfocusEditXAxisMovement3();
	afx_msg void OnSetfocusEditYAxisMovement3();
	afx_msg void OnSetfocusEditDAxisMovement3();
	afx_msg void OnSetfocusEditCameraXAxisMovement3();
	afx_msg void OnSetfocusEditCameraYAxisMovement3();
	afx_msg void OnSetfocusEditRefineDAxisMovement3();
	afx_msg void OnSetfocusEditRefineTime3();
	afx_msg void OnSetfocusEditSearchDelayTime3();
	afx_msg void OnSetfocusEditXAxisMovement4();
	afx_msg void OnSetfocusEditYAxisMovement4();
	afx_msg void OnSetfocusEditDAxisMovement4();
	afx_msg void OnSetfocusEditCameraXAxisMovement4();
	afx_msg void OnSetfocusEditCameraYAxisMovement4();
	afx_msg void OnSetfocusEditRefineDAxisMovement4();
	afx_msg void OnSetfocusEditRefineTime4();
	afx_msg void OnSetfocusEditSearchDelayTime4();
	afx_msg void OnSetfocusEditXAxisMovement5();
	afx_msg void OnSetfocusEditYAxisMovement5();
	afx_msg void OnSetfocusEditDAxisMovement5();
	afx_msg void OnSetfocusEditCameraXAxisMovement5();
	afx_msg void OnSetfocusEditCameraYAxisMovement5();
	afx_msg void OnSetfocusEditRefineDAxisMovement5();
	afx_msg void OnSetfocusEditRefineTime5();
	afx_msg void OnSetfocusEditSearchDelayTime5();
	afx_msg void OnSetfocusEditXAxisMovement6();
	afx_msg void OnSetfocusEditYAxisMovement6();
	afx_msg void OnSetfocusEditDAxisMovement6();
	afx_msg void OnSetfocusEditCameraXAxisMovement6();
	afx_msg void OnSetfocusEditCameraYAxisMovement6();
	afx_msg void OnSetfocusEditRefineDAxisMovement6();
	afx_msg void OnSetfocusEditRefineTime6();
	afx_msg void OnSetfocusEditSearchDelayTime6();
	afx_msg void OnSetfocusEditXAxisMovement7();
	afx_msg void OnSetfocusEditYAxisMovement7();
	afx_msg void OnSetfocusEditDAxisMovement7();
	afx_msg void OnSetfocusEditCameraXAxisMovement7();
	afx_msg void OnSetfocusEditCameraYAxisMovement7();
	afx_msg void OnSetfocusEditRefineDAxisMovement7();
	afx_msg void OnSetfocusEditRefineTime7();
	afx_msg void OnSetfocusEditSearchDelayTime7();
	afx_msg void OnSetfocusEditXAxisMovement8();
	afx_msg void OnSetfocusEditYAxisMovement8();
	afx_msg void OnSetfocusEditDAxisMovement8();
	afx_msg void OnSetfocusEditCameraXAxisMovement8();
	afx_msg void OnSetfocusEditCameraYAxisMovement8();
	afx_msg void OnSetfocusEditRefineDAxisMovement8();
	afx_msg void OnSetfocusEditRefineTime8();
	afx_msg void OnSetfocusEditSearchDelayTime8();

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
	afx_msg void OnBnClickedBtnCopyCalibData();
	afx_msg void OnCbnSelchangeComboCalibSrc();
	afx_msg void OnCbnSelchangeComboCalibDst();
	afx_msg void OnBnClickedBtnCopyPattern();
	CButton m_radioPos1;
	CButton m_radioPos2;
	CButton m_radioPos3;
	CButton m_radioPos4;
	CButton m_radioPos5;	
	CButton m_radioPos6;
	CButton m_radioPos7;
	CButton m_radioPos8;
	int m_nRadioPosIndex;
	CButton m_checkPos1;
	CButton m_checkPos2;
	CButton m_checkPos3;
	CButton m_checkPos4;
	CButton m_checkPos5;
	CButton m_checkPos6;
	CButton m_checkPos7;
	CButton m_checkPos8;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnDblclkStaticPos0();
	afx_msg void OnDblclkStaticPos1();
	afx_msg void OnDblclkStaticPos2();
	afx_msg void OnDblclkStaticPos3();
	afx_msg void OnDblclkStaticPos4();
	afx_msg void OnDblclkStaticPos5();
	afx_msg void OnDblclkStaticPos6();
	afx_msg void OnDblclkStaticPos7();

	static bool GetCalibratedResultDetailDescription(CString &strDescription,vcBaseVisionAlign *pVisionASM,int nPosIndex);
	void ShowCalibratedResult_Original(int nPosIndex);
	void ShowCalibratedResult(int nPosIndex);
	afx_msg void OnBnClickedRadioPosChange();
	afx_msg void OnBnClickedCheckPos1();
	afx_msg void OnBnClickedCheckPos2();
	afx_msg void OnBnClickedCheckPos3();
	afx_msg void OnBnClickedCheckPos4();
	afx_msg void OnBnClickedCheckPos5();
	afx_msg void OnBnClickedCheckPos6();
	afx_msg void OnBnClickedCheckPos7();
	afx_msg void OnBnClickedCheckPos8();
	BOOL m_bPos1;
	BOOL m_bPos2;
	BOOL m_bPos3;
	BOOL m_bPos4;
	BOOL m_bPos5;
	BOOL m_bPos6;
	BOOL m_bPos7;
	BOOL m_bPos8;

	void CopyCalibData(int nSrcCalibIndex, int nDstCalibIndex);
	CListCtrl m_listctrlProductFa;
	afx_msg void OnClickListProductFa(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnRClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnRClickListFaProduct(NMHDR* pNMHDR, LRESULT* pResult);
	CComboBox m_comboProductSrcFa;
	CComboBox m_comboProductDstFa;
	afx_msg void OnCbnSelchangeComboCalibSrcFa();
	afx_msg void OnCbnSelchangeComboCalibDstFa();
	afx_msg void OnBtnProductRenameFa();
	afx_msg void OnBnClickedBtnCopyCalibDataFa();
	CString m_strCurProductNameFa;
	afx_msg void OnBnClickedBtnDmcodeSetting();//设置二维码标定时的二维码搜索参数
	CComboBox m_comboCalibMarkPatternMode;//标定模式选择
	afx_msg void OnCbnSelchangeCmbCalibmode();
	
	void DistorCorrImage(int nPosIndex);
	afx_msg void OnBnClickedBtnDistorcorrIndex1();
	afx_msg void OnBnClickedBtnDistorcorrIndex2();
	afx_msg void OnBnClickedBtnDistorcorrIndex3();
	afx_msg void OnBnClickedBtnDistorcorrIndex4();
	afx_msg void OnBnClickedBtnDistorcorrIndex5();
	afx_msg void OnBnClickedBtnDistorcorrIndex6();
	afx_msg void OnBnClickedBtnDistorcorrIndex7();
	afx_msg void OnBnClickedBtnDistorcorrIndex8();

	void ClipMouseCursor(BOOL bClip);
	CFont  m_fontChangeProduct;//提示字体
	CComboBox m_comboHomographyDOF;
	afx_msg void OnCbnSelchangeComboHDof();
	CComboBox m_cmbWholeCalibCacuPos;
	afx_msg void OnBnClickedCheckDistorcorrIndex1();
	afx_msg void OnBnClickedCheckDistorcorrIndex2();
	afx_msg void OnBnClickedCheckDistorcorrIndex3();
	afx_msg void OnBnClickedCheckDistorcorrIndex4();
	afx_msg void OnBnClickedCheckDistorcorrIndex5();
	afx_msg void OnBnClickedCheckDistorcorrIndex6();
	afx_msg void OnBnClickedCheckDistorcorrIndex7();
	afx_msg void OnBnClickedCheckDistorcorrIndex8();
	BOOL m_bEnableWholeCalibCacuPos;
	afx_msg void OnBnClickedChkWholeCalibCacuPos();
	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	BOOL IsStringPositiveIntNumerical(LPCTSTR lpszVal);
	afx_msg void OnEnKillfocusEditXAxisMovement1();
	afx_msg void OnEnKillfocusEditXAxisMovement2();
	afx_msg void OnEnKillfocusEditXAxisMovement3();
	afx_msg void OnEnKillfocusEditXAxisMovement4();
	afx_msg void OnEnKillfocusEditXAxisMovement5();
	afx_msg void OnEnKillfocusEditXAxisMovement6();
	afx_msg void OnEnKillfocusEditXAxisMovement7();
	afx_msg void OnEnKillfocusEditXAxisMovement8();
	afx_msg void OnEnKillfocusEditYAxisMovement1();
	afx_msg void OnEnKillfocusEditYAxisMovement2();
	afx_msg void OnEnKillfocusEditYAxisMovement3();
	afx_msg void OnEnKillfocusEditYAxisMovement4();
	afx_msg void OnEnKillfocusEditYAxisMovement5();
	afx_msg void OnEnKillfocusEditYAxisMovement6();
	afx_msg void OnEnKillfocusEditYAxisMovement7();
	afx_msg void OnEnKillfocusEditYAxisMovement8();
	afx_msg void OnEnKillfocusEditDAxisMovement1();
	afx_msg void OnEnKillfocusEditDAxisMovement2();
	afx_msg void OnEnKillfocusEditDAxisMovement3();
	afx_msg void OnEnKillfocusEditDAxisMovement4();
	afx_msg void OnEnKillfocusEditDAxisMovement5();
	afx_msg void OnEnKillfocusEditDAxisMovement6();
	afx_msg void OnEnKillfocusEditDAxisMovement7();
	afx_msg void OnEnKillfocusEditDAxisMovement8();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement1();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement2();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement3();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement4();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement5();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement6();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement7();
	afx_msg void OnEnKillfocusEditRefineAxisDMovement8();
	afx_msg void OnEnKillfocusEditRefineTime1();
	afx_msg void OnEnKillfocusEditRefineTime2();
	afx_msg void OnEnKillfocusEditRefineTime3();
	afx_msg void OnEnKillfocusEditRefineTime4();
	afx_msg void OnEnKillfocusEditRefineTime5();
	afx_msg void OnEnKillfocusEditRefineTime6();
	afx_msg void OnEnKillfocusEditRefineTime7();
	afx_msg void OnEnKillfocusEditRefineTime8();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement1();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement2();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement3();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement4();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement5();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement6();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement7();
	afx_msg void OnEnKillfocusEditCameraXAxisMovement8();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement1();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement2();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement3();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement4();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement5();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement6();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement7();
	afx_msg void OnEnKillfocusEditCameraYAxisMovement8();
	afx_msg void OnEnKillfocusEditSearchDelayTime1();
	afx_msg void OnEnKillfocusEditSearchDelayTime2();
	afx_msg void OnEnKillfocusEditSearchDelayTime3();
	afx_msg void OnEnKillfocusEditSearchDelayTime4();
	afx_msg void OnEnKillfocusEditSearchDelayTime5();
	afx_msg void OnEnKillfocusEditSearchDelayTime6();
	afx_msg void OnEnKillfocusEditSearchDelayTime7();
	afx_msg void OnEnKillfocusEditSearchDelayTime8();
	BOOL LoadSearchToolInfo();

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bShowXYD;
	bool m_bEnableXYD;

	bool m_bShowDistortionCorrection;
	bool m_bEnableDistortionCorrectionD;

	bool m_bShowTemplateSet;
	bool m_bEnableTemplateSet;

	bool m_bShowCalibrationAlgorithm;
	bool m_bEnableCalibrationAlgorithm;

	bool m_bShowDmCodeSet;
	bool m_bEnableDmCodeSet;

	bool m_bShowCopyCalib;
	bool m_bEnableCopyCalib;

	bool m_bShowCalibList;
	bool m_bEnableCalibList;
};
