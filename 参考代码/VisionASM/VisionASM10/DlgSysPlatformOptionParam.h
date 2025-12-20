#pragma once
#include "vsVisionAlignDef.h"
#include "KeyBoard.h"
#include "vsBaseVisionAlign.h"
#include "afxcmn.h"
#include "afxwin.h"


class CDlgSysEnvironSetting;
// CDlgSysPlatformOptionParam 对话框

class CDlgSysPlatformOptionParam : public CDialogEx
{


public:
	CDlgSysPlatformOptionParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysPlatformOptionParam();
// 对话框数据
	enum { IDD = IDD_SYS_PLATFORM_OPTION_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void InitDlgItem();
	
	DECLARE_MESSAGE_MAP()
    void UpdateDlgSearchToolDataState();
	//lzk
	void UpdateAlignerTypeInfoState();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    
protected:
	DlgSearchToolInfo		m_dlgSearchToolInfo;	
	CPlatformOptionInfo     m_platformOptionInfo;
	CStringArray*			m_psaSysInfoStrings;
	CDlgSysEnvironSetting	*m_pParent;
	void					UpdateDialogLanguage();
	BOOL                    IsStringNumerical( LPCTSTR lpszVal );
	BOOL					IsStringPositiveNumerical(LPCTSTR lpszVal);
	BOOL					m_bSysKeyboardEnabled;
	CKeyBoard				m_ctrlKeyboard;
	BOOL					m_bKillFocusEnabled;
	int						m_nListLen;
	HBITMAP                 m_hBmpOffsetDir;
	//lzk
	AlignerTypeInfo         m_alignerTypeInfo;
	int						m_nListAligner;
public:
	void					UpdateDlgData(BOOL bSaveAndValidate = TRUE);
    void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void					SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) {  m_psaSysInfoStrings = psaSysInfoStrings; };
	void					SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }
	DlgSearchToolInfo		GetDlgSearchToolData() const;
	CPlatformOptionInfo     GetPlatformOptionInfo()const;
	void					SetDlgSearchToolData(const DlgSearchToolInfo& searchToolData);
	void					SetPlatformOptionInfo(const CPlatformOptionInfo& optionInfo);
	void					SetVisionASMRef(vcBaseVisionAlign *pVisionASM);
    BOOL                    IsAllSearchToolSelected();
	DlgSearchToolInfo		m_tmpDlgSearchToolInfo;
	CListCtrl m_listCtrlSearchTool;
	vcBaseVisionAlign *m_pVisionASM;


	afx_msg void OnNMClickSearchToolList(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bChkSelectAll;
	afx_msg void OnBnClickedCheckSelectAll();
	afx_msg void OnBnClickedButtonResetSearchTool();
    afx_msg void OnNMDblclkSearchToolList(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bSearchWithDistanceInspect;//对象或目标拍照时是否进行线距检测（目前支持FOBC/FOBJ/FTGC/FTGT/FGTW/FGTP/FCHB）
	BOOL m_bSendInsDistanceInSearchResult;//启用线距检测后是否给PLC发送线距检测详细结果
	CString m_strOffsetPicturePath;//补偿方向图示的路径
	afx_msg void OnBnClickedBtnSetOffsetPicture();
	BOOL m_bEnableDmCodeCalib;//是否允许对象二维码标定

	void ShowOffsetthumbnail(CString strDir);//显示补偿方向缩略图
	BOOL GetAndCheckOffsetPicturePath(CString &strDir);//获取并检查补偿方向图片路径
	afx_msg void OnPaint();

	BOOL m_bShowOffsetTips;
	BOOL m_bSetOffsetTip;
	int m_nTarInfoCommModeInDiffPC;//对象和目标不在同一台控制器时，目标端和对象端目标信息的交互方式(0 不交换，1 交换平台坐标和轴位置，2交换图像坐标和轴位置)
	BOOL m_bEnableAlignAfterSearch;//是否允许拍照后对位（目前支持FOBC/FOBJ/FTGC/FTGT)
	CString m_strOffsetMaxX;// 补偿最大值X
	CString m_strOffsetMaxY;// 补偿最大值Y
	CString m_strOffsetMaxD;// 补偿最大值D
	afx_msg void OnEnSetfocusEditOffsetMaxX();
	afx_msg void OnEnKillfocusEditOffsetMaxX();
	afx_msg void OnEnSetfocusEditOffsetMaxY();
	afx_msg void OnEnKillfocusEditOffsetMaxY();
	afx_msg void OnEnSetfocusEditOffsetMaxD();
	afx_msg void OnEnKillfocusEditOffsetMaxD();
	CString m_strAxisMoveMaxX;// 防止撞击最大移动量X
	CString m_strAxisMoveMaxY;// 防止撞击最大移动量Y
	CString m_strAxisMoveMaxD;// 防止撞击最大移动量D
	afx_msg void OnEnSetfocusEditAxisMoveMaxX();
	afx_msg void OnEnKillfocusEditAxisMoveMaxX();
	afx_msg void OnEnSetfocusEditAxisMoveMaxY();
	afx_msg void OnEnKillfocusEditAxisMoveMaxY();
	afx_msg void OnEnSetfocusEditAxisMoveMaxD();
	afx_msg void OnEnKillfocusEditAxisMoveMaxD();
	CString m_strAxisMoveMaxX_LowerLimit;// 防止撞击最大移动量X 下限
	CString m_strAxisMoveMaxY_LowerLimit;// 防止撞击最大移动量Y 下限
	CString m_strAxisMoveMaxD_LowerLimit;// 防止撞击最大移动量D 下限
	afx_msg void OnEnSetfocusEditAxisMoveMaxX2();
	afx_msg void OnEnKillfocusEditAxisMoveMaxX2();
	afx_msg void OnEnSetfocusEditAxisMoveMaxY2();
	afx_msg void OnEnKillfocusEditAxisMoveMaxY2();
	afx_msg void OnEnSetfocusEditAxisMoveMaxD2();
	afx_msg void OnEnKillfocusEditAxisMoveMaxD2();

	BOOL m_bOmitRePickObjectEnable;// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
	BOOL m_bOmitRePickObjectEnable_MM;// 是否启用忽略重复拾取（拾取不起来）的对象，拾取下一片。true：忽略；false：不忽略；
	CString m_strRePickOffsetMaxX;// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
	CString m_strRePickOffsetMaxY;// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取
	CString m_strRePickOffsetMaxX_MM;// 认为是重复拾取时最大的偏差X，小于（X、Y同时满足）该偏差认为是重复拾取
	CString m_strRePickOffsetMaxY_MM;// 认为是重复拾取时最大的偏差Y，小于（X、Y同时满足）该偏差认为是重复拾取
	afx_msg void OnEnSetfocusEditRepickOffsetMaxX();
	afx_msg void OnEnKillfocusEditRepickOffsetMaxX();
	afx_msg void OnEnSetfocusEditRepickOffsetMaxY();
	afx_msg void OnEnKillfocusEditRepickOffsetMaxY();
	afx_msg void OnEnSetfocusEditRepickOffsetMaxXMm();
	afx_msg void OnEnKillfocusEditRepickOffsetMaxXMm();
	afx_msg void OnEnSetfocusEditRepickOffsetMaxYMm();
	afx_msg void OnEnKillfocusEditRepickOffsetMaxYMm();
	BOOL m_bObjectSearchExProduct;//是否启用对象产品扩展，兼容老版本，老版本开启产品扩展功能时，对象拍照不用发扩展产品序号，该参数用于控制是否使用产品序号参数
	BOOL m_bTargetSearchExProduct;//是否启用目标产品扩展，兼容老版本，老版本开启产品扩展功能时，目标拍照不用发扩展产品序号，该参数用于控制是否使用产品序号参数
	BOOL m_bAlignExProduct;//是否启用对位产品扩展，该参数用于控制对位时是否使用产品序号参数
	BOOL m_bRecordCamFixMarkPos;
	BOOL m_bSaveCalibData;
	BOOL m_bAutoCopyCalibData;
	CComboBox m_cmbTarAutoCalibAlignType;
	BOOL m_bEnableCalibrateCheckByAxisMoveOption;
	double m_dCalibrateCheckAxisMoveRangeX;
	double m_dCalibrateCheckAxisMoveRangeY;
	double m_dCalibrateCheckAxisMoveRangeD;
	CString m_strCalibrateCheckAxisMoveRangeX;
	CString m_strCalibrateCheckAxisMoveRangeY;
	CString m_strCalibrateCheckAxisMoveRangeD;
	afx_msg void OnEnSetfocusEditValAxisMoveRangeX();
	afx_msg void OnEnKillfocusEditValAxisMoveRangeX();
	afx_msg void OnEnSetfocusEditValAxisMoveRangeY();
	afx_msg void OnEnKillfocusEditValAxisMoveRangeY();
	afx_msg void OnEnSetfocusEditValAxisMoveRangeD();
	afx_msg void OnEnKillfocusEditValAxisMoveRangeD();
	BOOL m_bReadWriteAxisPosWithCmd;
	BOOL m_bChkSendAlignMulResults;

	//lzk
	AlignerTypeInfo         m_tmpAlignerTypeInfo;
	AlignerTypeInfo			GetAlignerTypeInfo() const;
	void					SetAlignerTypeInfo(const AlignerTypeInfo& alignerTypeInfo);
	CListCtrl m_listCtrlAlignerType;
	afx_msg void OnNMClickListAlignerType(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL m_bSendAreaInspectSubResult;//是否发送面积检测子结果到PLC备用寄存器(仅支持FCHB指令候补模板索引大于100时)

	// 原选项中移植的部分
	BOOL m_bEnableCommMultiCalibExtension;
	afx_msg void OnBnClickedCheckEnableCommMultiCalibExtension();
	BOOL m_bVisionOnLineStatus;
	afx_msg void OnBnClickedCheckVisionStatus();

	BOOL m_bEnableAlignBenchCenter;
	afx_msg void OnBnClickedCheckEnableAlignBenchCenter();

	BOOL m_bSetVisionManualSearchToPLC;
	afx_msg void OnBnClickedCheckSetVisionManualSearchToPlc();

	//自定义回复地址
	BOOL m_bSetVisionManualSearchToPLC2;
	int m_nVisionManualSearchToPlcAddre;

	BOOL m_bAlignOneTime;
	afx_msg void OnBnClickedCheckAlignOneTime();

	BOOL m_bEnableMutiTarget;
	afx_msg void OnBnClickedCheckEnableMutiTarget();

	BOOL m_bObjMulSearchPosXYPD;
	BOOL m_bTargetAngleMulAlignerXYPD;
	afx_msg void OnBnClickedCheckObjMulSearchPosXypd();
	afx_msg void OnBnClickedCheckTargetAngleMulAlignerXypd();

	BOOL m_bLastPickSetting;
	afx_msg void OnBnClickedCheckLastPickSetting();

	BOOL m_bEnablePickPosUpdate;
	afx_msg void OnBnClickedCheckPickPosUpdate();

	BOOL m_bAskStagePosEachMove;
	afx_msg void OnBnClickedCheckAskStagePosEachMove();

	BOOL m_bTargetPosSame;
	BOOL m_bObjectPosSame;
	BOOL m_bSaveAlignPos;
	afx_msg void OnBnClickedCheckTargetPosSame();
	afx_msg void OnBnClickedCheckObjectPosSame();
	afx_msg void OnBnClickedCheckSaveAlignPos();

	BOOL m_bEnableSearchCindidate;
	afx_msg void OnBnClickedCheckSearchCindidateEnable();

	BOOL m_bSearchTimes;
	int m_nSearchTimes;
	afx_msg void OnSetfocusEditSearchtime();
	afx_msg void OnKillfocusEditSearchtime();
	afx_msg void OnBnClickedCheckSearchtime();

	BOOL m_bEnableSearchShutter;
	afx_msg void OnBnClickedCheckEnableSearchShutter();

	afx_msg void OnBnClickedCheckShowPcb();
	afx_msg void OnBnClickedCheckShowFwel();
	BOOL m_bShowFWEL;
//	BOOL m_bShowPCB;
	BOOL m_bShowEightShapeAlign;
	BOOL m_bShowBend;

	BOOL m_bManualAfTarSearchFail;
	BOOL m_bManualAfObjSearchFail;
	BOOL m_bAlignFinishClearSnapStatus;
	BOOL m_bExeObjectCheckModeWhenAlign;
	BOOL m_bExeTargetCheckModeWhenAlign;

	BOOL m_bManualAfTarSearchFailShowButton;
	BOOL m_bManualAfObjSearchFailShowButton;
	afx_msg void OnBnClickedCheckManualTargetSearchFailed();
	afx_msg void OnBnClickedCheckManualObjectSearchFailed();
	afx_msg void OnBnClickedCheckManualTargetSearchFailedShowButton();
	afx_msg void OnBnClickedCheckManualObjectSearchFailedShowButton();

	//BOOL m_bInspectEnable;

	BOOL m_bTargetUseFixedVirtualCoordinates;
	BOOL m_bEnableMultiMarkMode;// 手表贴合多标记点模式
	BOOL m_bEnableAlignOffsetMarkByAera; // 是否根据区域进行补偿
	BOOL m_bEnableTargetAndObjectPositionVirtualMode;// 是否启用-目标和对象位置虚拟模式

	BOOL m_bCalibrateChangeXDirection;	//标定过程中反置轴的X方向
	BOOL m_bCalibrateChangeYDirection;	//标定过程中反置轴的Y方向
	BOOL m_bCalibrateChangeDDirection;	//标定过程中反置轴的D方向

	BOOL m_bEnableCalibrateCheckBySize;	//启用标定验证--对象产品尺寸验证功能
	BOOL m_bEnableRelevanceCheckBySize;	//启用关联验证--目标产品尺寸验证功能
	BOOL m_bEnableCalibrateCheckByFixedTargetAligner;	//启用标定验证--通过轴移动产品验证尺寸

	BOOL m_bCalibSendAbsPos;             // 默认为ture----TRUE:标定时发送绝对轴坐标，FALSE:标定时发送相对值(相对基准轴位置)
	BOOL m_bCalibSendRelativePos;		// 默认false----TRUE:标定时发送相对位置(相对当前位置)
	//针对使用FAAL的做上料的时候，对固定点的时候，第一次拍照需要开启，后面关掉就行了，避免重复拍目标，减少对位时间
	BOOL m_bFAALAlignOneTimeTargetSeachSnap;

	afx_msg void OnBnClickedBtnCalibDirection();
	
private:
	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow); // 将nID的Group范围内的控件设置为启用禁用以及显示隐藏
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

public:
	afx_msg void OnBnClickedCheckCalibSendRelativePos();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	BOOL m_bEnableAllControl;
	void SetEnableAllControl(BOOL bEnable);

	bool m_bShowCalibrateCtl;
	bool m_bEnableCalibrateCtl;

	bool m_bShowOffsetCtl;
	bool m_bEnableOffsetCtl;
	
	afx_msg void OnBnClickedButtonOffsetOtherSet();
	afx_msg void OnBnClickedButtonInspectSet2();
	afx_msg void OnBnClickedButtonSnapSet();
	afx_msg void OnBnClickedButtonAlignerOptionSet2();
	afx_msg void OnBnClickedButtonPlatformCommSet3();
	afx_msg void OnBnClickedButtonCalibtateSet4();
	BOOL m_bEnablePickSendRelativateAxis;
	

	afx_msg void OnBnClickedChkFaalAlignOneTimeTargetSearchSnap();
	afx_msg void OnEnSetSouSuoTime();
	// 定位失败后搜索延时
	int m_nSearchDelayTime;

	BOOL m_bEnablePhotoLightCtrl;
	afx_msg void OnBnClickedCheckMoreTimeTargetSearch();
	BOOL m_bFAALAlignMoreTimeTargetSeach;
};
