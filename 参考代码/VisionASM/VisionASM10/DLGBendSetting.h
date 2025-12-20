#pragma once

#include "KeyBoard.h"


// CDlgBendSetting 对话框

class CDlgBendSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgBendSetting)

public:
	CDlgBendSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBendSetting();

// 对话框数据
	enum { IDD = IDD_DLG_BENDING_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	BOOL			m_bKillFocusEnabled;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	BOOL IsStringNumerical(LPCTSTR lpszVal);

	int m_nPlatformIndex;

public:
	BOOL m_bEnableMultiCalibExtension;
	BOOL m_bBendAlignInspect;

	BOOL m_bBendType;
	CString m_strPixelSize;
	BOOL m_bBendAlignDmCode;

	BOOL m_bBendDisOffset;
	CString m_strTargetDisOffset;
	CString m_strObjectDisOffset;

	BOOL m_bBendResultOffset;
	CString m_strLeftOffsetX;
	CString m_strLeftOffsetY;
	CString m_strRightOffsetX;
	CString m_strRightOffsetY;

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

	BOOL m_bBendingInspect;
	BOOL m_bBendingProductID;
	BOOL m_bBendInspectDmCode;

	BOOL m_bBendInspectSnCodeInvert;//高低位取反
	long m_bBendInspectSnCodeLength;//SN码长度
	BOOL m_bBendInspectGetSnOnce;//多次对位只获取一次SN码
	

	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditBendPixelSize();
	afx_msg void OnKillfocusEditBendPixelSize();
	afx_msg void OnSetfocusEditTargetDistanceOffset();
	afx_msg void OnKillfocusEditTargetDistanceOffset();
	afx_msg void OnSetfocusEditObjectDistanceOffset();
	afx_msg void OnKillfocusEditObjectDistanceOffset();
	afx_msg void OnSetfocusEditLeftOffsetX();
	afx_msg void OnKillfocusEditLeftOffsetX();
	afx_msg void OnSetfocusEditLeftOffsetY();
	afx_msg void OnKillfocusEditLeftOffsetY();
	afx_msg void OnSetfocusEditRightOffsetX();
	afx_msg void OnKillfocusEditRightOffsetX();
	afx_msg void OnSetfocusEditRightOffsetY();
	afx_msg void OnKillfocusEditRightOffsetY();
	afx_msg void OnSetfocusEditThreOffsetX();
	afx_msg void OnKillfocusEditThreOffsetX();
	afx_msg void OnSetfocusEditThreOffsetY();
	afx_msg void OnKillfocusEditThreOffsetY();
	afx_msg void OnSetfocusEditPressOkMinY();
	afx_msg void OnKillfocusEditPressOkMinY();
	afx_msg void OnSetfocusEditPressOkMaxY();
	afx_msg void OnKillfocusEditPressOkMaxY();
	afx_msg void OnSetfocusEditPressNgMinY();
	afx_msg void OnKillfocusEditPressNgMinY();
	afx_msg void OnSetfocusEditPressNgMaxY();
	afx_msg void OnKillfocusEditPressNgMaxY();
	afx_msg void OnKillfocusEditBendLoadX();
	afx_msg void OnSetfocusEditBendLoadX();
	afx_msg void OnKillfocusEditBendLoadY();
	afx_msg void OnSetfocusEditBendLoadY();

//	afx_msg void OnEnSetfocusEditSnCode();

	BOOL m_bEnableMoveSnap;
	int m_nPosIndexFisrt;
	afx_msg void OnBnClickedCheckBendingIns();
	BOOL m_bEnableAxisMoveLimit;
	double m_dAxisAllowMoveMaxX;
	double m_dAxisAllowMoveMaxY;
	double m_dAxisAllowMoveMaxD;
	afx_msg void OnEnKillfocusEditOffsetMaxX();
	afx_msg void OnEnSetfocusEditOffsetMaxX();
	afx_msg void OnEnKillfocusEditOffsetMaxY();
	afx_msg void OnEnSetfocusEditOffsetMaxY();
	afx_msg void OnEnKillfocusEditOffsetMaxD();
	afx_msg void OnEnSetfocusEditOffsetMaxD();

//	afx_msg void OnEnSetfocusEditBengNumOfInspectResults();
	afx_msg void OnEnKillfocusEditBengSncodeLength();
	afx_msg void OnEnSetfocusEditBengSncodeLength();
	// 取消Y向绝对值使用
	// 取消Y向绝对值使用
	BOOL m_bEnableFbsLRY;
	afx_msg void OnBnClickedCheckBendPressParameterWrite();
	afx_msg void OnEnKillfocusEditIdcBendPressParameterWriteAddre();
	afx_msg void OnEnSetfocusEditIdcBendPressParameterWriteAddre();
	BOOL m_bBendPressParaWrite;
	long m_lBendPressParaWriteAddre;
	afx_msg void OnBnClickedCheckBendInspect();
	afx_msg void OnBnClickedCheckBendAlignDmcode();
	afx_msg void OnBnClickedCheckBendDistanceOffset();
	afx_msg void OnBnClickedCheckBendResultOffset();
	afx_msg void OnBnClickedCheckBendThreOffset();
	afx_msg void OnBnClickedCheckBendPressParameter();
	afx_msg void OnBnClickedCheckBendLoadThre();
	afx_msg void OnBnClickedCheckEnableSendOffset();
	afx_msg void OnBnClickedCheckBendInspectDmcode();
	afx_msg void OnBnClickedCheckEnableMoveSnap();
	afx_msg void OnBnClickedCheckFbsLry();
	afx_msg void OnBnClickedCheckBendingProductId();
	afx_msg void OnBnClickedCheckBendingProductGetSnOnes();
	afx_msg void OnBnClickedCheckBendingProductSnInvert();
	afx_msg void OnBnClickedRadioBendPlatformInspect();
	afx_msg void OnBnClickedRadioBendImageInspect();
	afx_msg void OnBnClickedRadioPostion1Frist();
	afx_msg void OnBnClickedRadioPostion2Frist();
	BOOL m_bEnableReadWsInfo;
	afx_msg void OnBnClickedCheckReadWsInfo();
	afx_msg void OnBnClickedCheckReadWsInfo2();
	afx_msg void OnBnClickedCheckReadWsInfo3();
	afx_msg void OnBnClickedCheckReadWsInfo4();
	afx_msg void OnBnClickedCheckReadWsInfo5();
	BOOL m_bEnableReadInspectWsInfo;
	BOOL m_bEnableReadStartTimeInfo;
	BOOL m_bEnableReadEndTimeInfo;
	BOOL m_bEnableReadProsessingTimeInfo;
	afx_msg void OnEnSetfocusEditLeftDisOffset();
	afx_msg void OnEnKillfocusEditLeftDisOffset();
	afx_msg void OnEnSetfocusEditRightDisOffset();
	afx_msg void OnEnKillfocusEditRightDisOffset();
	CString m_strLeftDisOffset;
	CString m_strRightDisOffset;
	BOOL m_bEnableUseMathCaluLRDis;
	afx_msg void OnBnClickedCheckUseMathCaluLrdis();
	afx_msg void OnBnClickedCheckFbisTarObc();
	CString m_strPlatformName1;
	CString m_strPlatformName2;
	BOOL m_bEnableFbisTarObc;
};
