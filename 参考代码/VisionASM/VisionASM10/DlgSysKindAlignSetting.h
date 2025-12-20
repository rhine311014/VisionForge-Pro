#pragma once
#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"
#include "LogFileSave.h"
#include "afxwin.h"
// CDlgSysKindAlignSetting 对话框
class CDlgSysKind;
class CDlgSysKindAlignSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindAlignSetting)

public:
	CDlgSysKindAlignSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindAlignSetting();

	//20160510增加对角补偿
	double m_dObjectL1;
	double m_dObjectW1;
	double m_dTargetL2;
	double m_dTargetW2;
	double m_dF1;
	double m_dF2;
	bool   m_bOffsetXYEnable;
	int m_nPosMode;

	//lzk 0405 增减对象目标虚拟位置参数
	bool m_bEnableObjectPositionVirtualMode;
	bool m_bEnableTargetPositionVirtualMode;
	int m_nObjectPositionVirtualMode;
	int m_nTargetPositionVirtualMode;


	// 补偿设置（补偿的方向、X和Y补偿值对调）
// 	int m_nProductOffsetXDirection;
// 	int m_nProductOffsetYDirection;
// 	int m_nProductOffsetDDirection;
// 
// 	int m_nProductOffsetXSetting;
// 	int m_nProductOffsetYSetting;
// 	int m_nProductOffsetDSetting;

	BOOL m_bEnableProductOffsetXDirection;
	BOOL m_bEnableProductOffsetYDirection;
	BOOL m_bEnableProductOffsetDDirection;

	BOOL m_bEnableProductOffsetXSetting;
	BOOL m_bEnableProductOffsetYSetting;
	BOOL m_bEnableProductOffsetDSetting;


	

// 对话框数据
	enum { IDD = IDD_SYS_KIND_ALIGN_SETTING };


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
	CString m_strModuleDir;
	BOOL m_bProdPatternChanged;
	BOOL m_bProdNameChanged;
	BOOL m_bProdBenchMarkChanged;
	BOOL m_bProdNewCreate;
	BOOL m_bCalibInfoChanged;

	CString m_strProductNameOld;
	CStringArray m_saAlignerOffsetX;
	CStringArray m_saAlignerOffsetY;
	CStringArray m_saAlignerOffsetD;

	CStringArray m_saAlignerBenchOffsetX;
	CStringArray m_saAlignerBenchOffsetY;
	CStringArray m_saAlignerBenchOffsetD;

	// 系统选项相关
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	void CheckDlgData();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void UpdateCalibDlgData(BOOL bSaveAndValidate = TRUE);
	void GetDlgTempData(CProductData& product);
	void InitDlgItem();
	void MoveDlgItem();
	void UpdateProductListDisplay();

	CString GetNewDefName();
	BOOL IsNameLegal(LPCTSTR lpszName);
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	BOOL IsStringOverMaxX(LPCTSTR lpszVal);
	BOOL IsStringOverMaxY(LPCTSTR lpszVal);
	BOOL IsStringOverMaxD(LPCTSTR lpszVal);
	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
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
	void UpdateDialogLanguage();

public:
	int		m_nPlatformIndex;
	int m_nPrevProductIdx;
	int m_nCurProductIdx;
	int m_nPrevPlatformIdx;
	int m_nPrevAlignerOffsetIndex;
	int m_nPrevAlignerBenchOffsetIndex;
	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;
	map<int,CRect>m_InitPosition;
//	std::vector<CTotalProductData*> m_vpTotalProductData;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	BOOL IsNameUnique(LPCTSTR lpszName);	// 检查产品名称是否唯一
	void ShowTipSysKeyboard(CPoint point);	
	void LoadOffsetDirectionPicture();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
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
	CComboBox	m_comboAligner2Vs2Type;
	CComboBox	m_comboAligner3Vs3Type;
	CComboBox	m_comboAligner4Vs4Type;
	CComboBox   m_comboAlignerCirclePos;
	CComboBox	m_comboAlignerMoveType;
	CComboBox	m_comboAlignerOffsetCoordType;
	CComboBox	m_comboAlignerOffsetIndex;
	CComboBox	m_comboAlignerBenchOffsetIndex;
	CString	m_strAlignerPrecisionX;
	CString	m_strAlignerPrecisionY;
	CString	m_strAlignerPrecisionD;
	CString	m_strAlignerBaseOffsetX;
	CString	m_strAlignerBaseOffsetY;
	CString	m_strAlignerBaseOffsetD;
	CString	m_strAlignerOffsetX;
	CString	m_strAlignerOffsetY;
	CString	m_strAlignerOffsetD;
	CString	m_strAlignerOffsetX2;
	CString	m_strAlignerOffsetY2;
	CString	m_strAlignerOffsetD2;
	CString	m_strAlignerOffsetX3;
	CString	m_strAlignerOffsetY3;
	CString	m_strAlignerOffsetD3;
	CString	m_strAlignerOffsetX4;
	CString	m_strAlignerOffsetY4;
	CString	m_strAlignerOffsetD4;
	CString	m_strAlignerOffsetX5;
	CString	m_strAlignerOffsetY5;
	CString	m_strAlignerOffsetD5;
	CString	m_strAlignerOffsetX6;
	CString	m_strAlignerOffsetY6;
	CString	m_strAlignerOffsetD6;
	CString	m_strAlignerBenchOffsetX;
	CString	m_strAlignerBenchOffsetY;
	CString	m_strAlignerBenchOffsetD;
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
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetfocusEditAlignerMaxTimes();
	afx_msg void OnSetfocusEditAlignerXPrecision();
	afx_msg void OnSetfocusEditAlignerYPrecision();
	afx_msg void OnSetfocusEditAlignerDPrecision();
	afx_msg void OnSetfocusEditAlignerDBaseOffset();
	afx_msg void OnSetfocusEditAlignerXBaseOffset();
	afx_msg void OnSetfocusEditAlignerYBaseOffset();
	afx_msg void OnSetfocusEditAlignerDOffset();
	afx_msg void OnSetfocusEditAlignerXOffset();
	afx_msg void OnSetfocusEditAlignerYOffset();
	afx_msg void OnSetfocusEditAlignerDOffset2();
	afx_msg void OnSetfocusEditAlignerXOffset2();
	afx_msg void OnSetfocusEditAlignerYOffset2();
	afx_msg void OnSetfocusEditAlignerDOffset3();
	afx_msg void OnSetfocusEditAlignerXOffset3();
	afx_msg void OnSetfocusEditAlignerYOffset3();
	afx_msg void OnSetfocusEditAlignerDOffset4();
	afx_msg void OnSetfocusEditAlignerXOffset4();
	afx_msg void OnSetfocusEditAlignerYOffset4();
	afx_msg void OnSetfocusEditAlignerDOffset5();
	afx_msg void OnSetfocusEditAlignerXOffset5();
	afx_msg void OnSetfocusEditAlignerYOffset5();
	afx_msg void OnSetfocusEditAlignerDOffset6();
	afx_msg void OnSetfocusEditAlignerXOffset6();
	afx_msg void OnSetfocusEditAlignerYOffset6();
	afx_msg void OnSetfocusEditAlignerDBenchOffset();
	afx_msg void OnSetfocusEditAlignerXBenchOffset();
	afx_msg void OnSetfocusEditAlignerYBenchOffset();
	afx_msg void OnClickListProduct(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditAlignerXPrecision();
	afx_msg void OnKillfocusEditAlignerYPrecision();
	afx_msg void OnKillfocusEditAlignerDPrecision();
	afx_msg void OnKillfocusEditAlignerDBaseOffset();
	afx_msg void OnKillfocusEditAlignerXBaseOffset();
	afx_msg void OnKillfocusEditAlignerYBaseOffset();
	afx_msg void OnKillfocusEditAlignerDOffset();
	afx_msg void OnKillfocusEditAlignerXOffset();
	afx_msg void OnKillfocusEditAlignerYOffset();
	afx_msg void OnKillfocusEditAlignerDOffset2();
	afx_msg void OnKillfocusEditAlignerXOffset2();
	afx_msg void OnKillfocusEditAlignerYOffset2();
	afx_msg void OnKillfocusEditAlignerDOffset3();
	afx_msg void OnKillfocusEditAlignerXOffset3();
	afx_msg void OnKillfocusEditAlignerYOffset3();
	afx_msg void OnKillfocusEditAlignerDOffset4();
	afx_msg void OnKillfocusEditAlignerXOffset4();
	afx_msg void OnKillfocusEditAlignerYOffset4();
	afx_msg void OnKillfocusEditAlignerDOffset5();
	afx_msg void OnKillfocusEditAlignerXOffset5();
	afx_msg void OnKillfocusEditAlignerYOffset5();
	afx_msg void OnKillfocusEditAlignerDOffset6();
	afx_msg void OnKillfocusEditAlignerXOffset6();
	afx_msg void OnKillfocusEditAlignerYOffset6();
	afx_msg void OnKillfocusEditAlignerDBenchOffset();
	afx_msg void OnKillfocusEditAlignerXBenchOffset();
	afx_msg void OnKillfocusEditAlignerYBenchOffset();
	afx_msg void OnKillfocusEditAlignerMaxTimes();
	afx_msg void OnClose();
	afx_msg void OnBtnCloneProduct();

	afx_msg void OnEnSetfocusEditSearchDelayTime();
	afx_msg void OnEnKillfocusEditSearchDelayTime();
	afx_msg void OnEnSetfocusEditObjectSearchDelayTime();
	afx_msg void OnEnKillfocusEditObjectSearchDelayTime();
	afx_msg void OnCbnSelchangeComboAlignerOffsetIndex();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboAlignerOffsetCoordType();	//20160510增加对角补偿
	afx_msg void OnBnClickedButtonOffsetCalculate();			//20160510增加对角补偿
	afx_msg void OnKillfocusEditObjectSearchDelayTime2();
	afx_msg void OnSetfocusEditObjectSearchDelayTime2();

	int m_nSearchDelayTime;
	int m_nObjectSearchDelayTime;
	int m_nObjectSearchDelayTime2;
	BOOL m_bEnableObjectSearchDelayTime2;
//	afx_msg void OnEnSetfocusEditMovementRatioX();
//	afx_msg void OnEnKillfocusEditMovementRatioX();
//	afx_msg void OnEnSetfocusEditMovementRatioY();
//	afx_msg void OnEnKillfocusEditMovementRatioY();

	//afx_msg void OnEnSetfocusEditEighitAngle();
	//afx_msg void OnEnKillfocusEditEighitAngle();
	//BOOL m_bDiffPosWithSameSearchResultEnabled;
	CString m_strAlnTargetOffsetX;
	CString m_strAlnTargetOffsetY;
	afx_msg void OnEnSetfocusEditAlignerTargetXOffset();
	afx_msg void OnEnKillfocusEditAlignerTargetXOffset();
	afx_msg void OnEnSetfocusEditAlignerTargetYOffset();
	afx_msg void OnEnKillfocusEditAlignerTargetYOffset();
	BOOL m_bCheckTargetOffsetEnable;
	BOOL m_bAlignAngleVertical;
	int m_nAlignAngleVerticalMode;
	afx_msg void OnCbnSelchangeComboAlignerBenchOffsetIndex();
// 	BOOL m_bObjectSearchShutter;
// 	BOOL m_bTargetSearchShutter;
// 	BOOL m_bInspectSearchShutter;
	afx_msg void OnBnClickedButtonProductOffset();
	CString m_strAlignerWeight;
	afx_msg void OnKillfocusEditAlignerWieght4vs4();
	afx_msg void OnSetfocusEditAlignerWieght4vs4();
	afx_msg void OnCbnSelchangeComboAligner4vs4Type();
	//BOOL m_bEnableMutiTarget;
	CString m_strCurFaCalibName;



	//目标固定补偿
	BOOL m_bEnableTarFixOffset;			//目标采用靶标关联时，是否启用目标固定补偿
	int  m_nTarFixOffsetType;			// 0：产品计算固定位置关系；1：二维码计算固定位置关系
	CStringArray m_saTarFixOffsetX;//目标固定补偿X
	CStringArray m_saTarFixOffsetY;//目标固定补偿Y
	CStringArray m_saTarFixOffsetD;//目标固定补偿D

	BOOL m_bAlignOffsetReverseX;//发送给PLC的平台移动量X方向反置
	BOOL m_bAlignOffsetReverseY;//发送给PLC的平台移动量Y方向反置
	BOOL m_bAlignOffsetReverseD;//发送给PLC的平台移动量D方向反置

	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedCheckTargetAndObjectDis();
	afx_msg void OnBnClickedChkSalnOffset();
//	afx_msg void OnBnClickedCheckTargetAngle();
	afx_msg void OnBnClickedCheckTargetShutter();
	afx_msg void OnBnClickedCheckObjectShutter();
	//afx_msg void OnBnClickedCheckMulMarkMode();
	//afx_msg void OnBnClickedCheckEnablemutitarget();
	afx_msg void OnBnClickedCheckAlignerAngleVertical();
	//afx_msg void OnBnClickedCheckEightShapeAlign();
//	afx_msg void OnBnClickedCheckTargetDis();
//	afx_msg void OnBnClickedCheckObjectDis();
	afx_msg void OnAlignerAngleVerticalChange();
	afx_msg void OnAlignCheckModeChange();
	//afx_msg void OnAlignCheckEightModeChange();
//	afx_msg void OnBnClickedCheckPixelDis();
	afx_msg void OnCbnSelchangeComboAligner2vs2Type();
	afx_msg void OnCbnSelchangeComboAligner3vs3Type();
	afx_msg void OnCbnSelchangeComboAlignerMoveType();
	afx_msg void OnKillfocusEditAmmendMovementRatioX();
	afx_msg void OnSetfocusEditAmmendMovementRatioX();
	afx_msg void OnKillfocusEditAmmendMovementRatioY();
	afx_msg void OnSetfocusEditAmmendMovementRatioY();


	BOOL m_bEnableSendTargetOffsetToPLC;
	afx_msg void OnBnClickedCheckEnableSendOffset();
	afx_msg void OnBnClickedBtnTarFixOffset();
	afx_msg void OnBnClickedBtnAlignOffsetSetting();
	CComboBox m_comboMultiCalibExtension;
	int m_nMultiCalibExtensionIndex;//产品扩展序号




	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	vector<CString> m_vstrOffSetTip;
	// 可显示备注
	BOOL m_bShowOffsetTips;
	// 可修改备注
	BOOL m_bSetOffsetTip;
	afx_msg void OnSelchangeComboAligner4vs4Circlepos();


	HBITMAP m_hBmpOffsetDir;
	BOOL m_bAlignAccordingToPickEnable;
	int m_nPreMultiCalibExtensionIndex;//先前的扩展产品序号

	CStringArray m_saMult_CheckTargetAndObjectDis;
	CStringArray m_saMult_CheckMode;

	CStringArray m_saMult_CheckTargetDisThre;
	CStringArray m_saMult_CheckObjectDisThre;
	CStringArray m_saMult_CheckTargetAngleThre;
	CStringArray m_saMult_CheckObjectAngleThre;

	CStringArray m_saMult_TargetDisThre;
	CStringArray m_saMult_ObjectDisThre;
	CStringArray m_saMult_TargetAngleThre;
	CStringArray m_saMult_ObjectAngleThre;

	BOOL m_bCheckTargetAndObjectDis;
	BOOL m_nCheckMode;

	BOOL m_bTargetDisThre;
	BOOL m_bObjectDisThre;
	BOOL m_bTargetAngleThre;
	BOOL m_bObjectAngleThre;

	CString m_strTargetDisThre;	
	CString m_strObjectDisThre;
	CString m_strTargetAngleThre;
	CString m_strObjectAngleThre;

	
	afx_msg void OnSetfocusEditTargetDisThre();
	afx_msg void OnKillfocusEditTargetDisThre();
	afx_msg void OnSetfocusEditObjectDisThre();
	afx_msg void OnKillfocusEditObjectDisThre();
	afx_msg void OnSetfocusEditTargetAngleThre();
	afx_msg void OnKillfocusEditTargetAngleThre();
	afx_msg void OnSetfocusEditObjectAngleThre();
	afx_msg void OnKillfocusEditObjectAngleThre();

	afx_msg void OnCbnSelchangeComboExproductIndex();
	afx_msg void OnBnClickedCheckTargetDisThre();
	afx_msg void OnBnClickedCheckObjectDisThre();
	afx_msg void OnBnClickedCheckTargetAngleThre();
	afx_msg void OnBnClickedCheckObjectAngleThre();
	

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bShowCheckMode;
	bool m_bEnableCheckModeAuthoroze;

	bool m_bShowOffset;
	bool m_bEnableOffset;

	bool m_bShowAlignToolParamSet;
	bool m_bEnableAlignToolParamSet;

	bool m_bShowAlignTypeSet;
	bool m_bEnableAlignTypeSet;

	bool m_bShowAlignOther;
	bool m_bEnableAlignOther;

	bool m_bShowAlignSpecialApplicationSet;
	bool m_bEnableAlignSpecialApplicationSet;

	bool m_bShowAlignerCirclePos;


	// PCB参数
	CString	m_strPCBInsPreX;
	CString	m_strPCBInsPreY;
	CString	m_strPCBInsPreD;
	int	  m_nPCBInsPreType;
	BOOL m_bPCBShowInspectResult;
	BOOL m_bShowPCB4To3Align;

	// 专用焊接机参数
	BOOL m_bEnableWeld;
	CString m_strWeldMovementRatioX;
	CString m_strWeldMovementRatioY;
	CString m_strWeldMovementRatioD;
	CString m_strWeldAmmendMoveRatioX;
	CString m_strWeldAmmendMoveRatioY;
	int m_nWeldTargetSearchTimes;
	int m_nWeldSearchBench;
	int m_nWeldAngleBench;

	// 八字对位参数
	BOOL m_bCheckEightShapeAlign;
	int m_nEightShapeAlignMode;
	double m_dEightShapeAngle;

	// 折弯参数
	BOOL m_bBendAlignInspect;

	BOOL m_bBendType;
	CString m_strPixelSize;
	BOOL	m_bBendAlignDmCode;		// 对位过程中使用二维码标定信息进行检测，需开启产品扩展、二维码标定功能；

	BOOL m_bBendDisOffset;
	CString m_strTargetDisOffset;
	CString m_strObjectDisOffset;

	BOOL m_bBendResultOffset;
	CString m_strLeftOffsetX;
	CString m_strLeftOffsetY;
	CString m_strRightOffsetX;
	CString m_strRightOffsetY;
	CString m_strLeftDisOffset;
	CString m_strRightDisOffset;
	BOOL m_bEnableUseMathCaluLRDis;

	BOOL m_bBendThreOffset;
	CString m_strThreOffsetX;
	CString m_strThreOffsetY;

	BOOL m_bBendPressPara;
	CString m_strPressOKMinY;
	CString m_strPressOKMaxY;
	CString m_strPressNGMinY;
	CString m_strPressNGMaxY;

	BOOL m_bBendLoadThre;
	CString m_strBendLoadThreX;
	CString m_strBendLoadThreY;

	BOOL m_bBendingInspect;		// 对位过程中检测（FCHB）
	BOOL m_bBendingProductID;		// 产品ID
	BOOL	m_bBendInspectDmCode;	// 检测过程中使用二维码标定信息进行检测，需开启产品扩展、二维码标定功能；
	BOOL			m_bEnableReadWsInfo;    // 折弯平台信息
	BOOL			m_bEnableReadInspectWsInfo;    // 本压平台信息
	BOOL			m_bEnableReadStartTimeInfo;    // 生产开始时间
	BOOL			m_bEnableReadEndTimeInfo;    // 生产结束时间
	BOOL			m_bEnableReadProsessingTimeInfo;    // 生产总时间

	BOOL			m_bEnableMoveSnap;		// 启用移动拍照 移动2个位置
	int				m_nPosIndexFisrt;		// 移动拍照起始位置

	BOOL	m_bEnableAxisMoveLimit;
	double  m_dAxisAllowMoveMaxX;
	double  m_dAxisAllowMoveMaxY;
	double  m_dAxisAllowMoveMaxD;

	BOOL m_bBendInspectSnCodeInvert;//高低位取反
	long m_bBendInspectSnCodeLength;//SN码长度
	BOOL m_bBendInspectGetSnOnce;//多次对位只获取一次SN码

	// 取消Y向绝对值使用
	BOOL m_bEnableFbsLRY;

	afx_msg void OnBnClickedBtnPcbSetting();
	afx_msg void OnBnClickedBtnWeldSetting();
	afx_msg void OnBnClickedBtnEightShapeAlignSetting();
	afx_msg void OnBnClickedBtnBendSetting();
	afx_msg void OnBnClickedBtnCmdLightCtrlSet();

	PhotoLightCtrlSet m_cPhotoLightCtrl;
	BOOL m_bPhotoLightCtrlInfoChange;

	BOOL m_bBendPressParaWrite;
	long m_lBendPressParaWriteAddre;

	CString m_strPlatformName1;
	CString m_strPlatformName2;
	BOOL m_bEnableFbisTarObc;
};
