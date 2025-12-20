#pragma once
#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "Layout.h"
#include "DlgCalibCheckBySize.h"
#include "DlgCalibCheckByFixedTargetAligner.h"


// CDlgVisionRunAlignInfo 对话框

class CDlgVisionRunAlignInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVisionRunAlignInfo)

public:
	CDlgVisionRunAlignInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionRunAlignInfo();

// 对话框数据
	enum { IDD = IDD_DLG_VISION_RUN_ALIGN_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:

	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);
	void SetParentWnd(CDlgVisionRun* pParentWnd);
	void SetKeybordEnable(bool bKeyboardEnable);
	void SetDlgItemVisible();
	void EnableDlgItem(bool bEnable);		// 设置控件可用状态
	void UpdateDlgParamItem();				// 更新窗口控件输入，单位
	void UpdateAllAlignInfo();				// 更新所有对位信息

	void UpdateAlignOffsetInfo();			// 更新补偿信息
	void UpdateAlignPrecisionInfo();		// 更新对位精度设置
//	void UpdateAlignPrecisionInfo2();		// 更新对位精度设置2（PCB)
	void UpdateAlignerProcessInfo();		// 更新平台运动量、产品偏差量
	void UpdateCheckAlignerTargetSizeInfo();  // 更新尺寸计算信息
	void UpdateCheckAlignerObjectSizeInfo();  // 更新尺寸计算信息

	void UpdateCurPlatformAxisPosInfo();	 // 更新当前轴信息， 未使用

	void LoadOffsetDirectionPicture();		 // 加载补偿示意图

	void UpdateDialogLanguage();			// 更新语言包
	
protected:
	void InitBtnLayout();		// 设置按钮显示位置
	void SetItemFont();

	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	BOOL IsStringOverMaxX(LPCTSTR lpszVal);
	BOOL IsStringOverMaxY(LPCTSTR lpszVal);
	BOOL IsStringOverMaxD(LPCTSTR lpszVal);
private:
	CLayout* m_pMainLayout;		// 按钮区域布局

public:
	vcBaseVisionAlign* m_pVisionASM;  // 对位流程
	CDlgVisionRun*		m_pParent;	  // 主窗口

	BOOL		m_bKillFocusEnabled;
	BOOL		m_bSysKeyboardEnabled;	  // 是否开启虚拟键盘
	CKeyBoard	m_ctrlKeyboard;		  // 虚拟键盘
	
	CDlgCalibCheckBySize* m_pDlgCalibCheckBySize;
	CDlgCalibCheckByFixedTargetAligner* m_pDlgCalibCheckByFixedTargetAligner;
private:
	AlignerProcessInfo m_AlignerProcessInfo;
	CFont  m_MyFont;
	HBITMAP m_hBmpOffsetDir;

public:
	CString	m_strEditCurOffsetIndex;
	CString	m_strEditCurOffsetD;
	CString	m_strEditCurOffsetX;
	CString	m_strEditCurOffsetY;
	CString	m_strEditTargetPrecisionMaxTimes;
	CString	m_strEditTargetPrecisionD;
	CString	m_strEditTargetPrecisionX;
	CString	m_strEditTargetPrecisionY;
	CString	m_strEditTargetPrecisionD2;
	CString	m_strEditTargetPrecisionX2;
	CString	m_strEditTargetPrecisionY2;
	CString	m_strEditCurPrecisionD;
	CString	m_strEditCurPrecisionX;
	CString	m_strEditCurPrecisionY;
	CString	m_strEditAlignTimes;
	CString	m_strEditAxisPosD;
	CString	m_strEditAxisPosX;
	CString	m_strEditAxisPosY;
	CString	m_strEditINSMode;
	CString m_strEditSanDianMode;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnCalcuVirtualObjectPos();
	afx_msg void OnBnClickedBtnCalibTargetPos();
	afx_msg void OnBnClickedBtnRobotTeach();
	afx_msg void OnBnClickedBtnRobotTeachRange();
	afx_msg void OnBnClickedBtnCalcuBenchPick();
	afx_msg void OnBnClickedBtnCalibObjectPos();
	afx_msg void OnBnClickedBtnCalibCheckBySize();
	afx_msg void OnBnClickedBtnRelevanceCheckBySize();
	afx_msg void OnBnClickedBtnCalibCheckByFixedTarget();

	afx_msg void OnEnSetfocusEditValCurOffsetX();
	afx_msg void OnEnKillfocusEditValCurOffsetX();
	afx_msg void OnEnSetfocusEditValCurOffsetY();
	afx_msg void OnEnKillfocusEditValCurOffsetY();
	afx_msg void OnEnSetfocusEditValCurOffsetD();
	afx_msg void OnEnKillfocusEditValCurOffsetD();
	afx_msg void OnEnSetfocusEditValTargetPrecisionD();
	afx_msg void OnEnKillfocusEditValTargetPrecisionD();
	afx_msg void OnEnSetfocusEditValTargetPrecisionX();
	afx_msg void OnEnKillfocusEditValTargetPrecisionX();
	afx_msg void OnEnSetfocusEditValTargetPrecisionY();
	afx_msg void OnEnKillfocusEditValTargetPrecisionY();

	afx_msg void OnEnSetfocusEditValTargetPrecisionD2();
	afx_msg void OnEnKillfocusEditValTargetPrecisionD2();
	afx_msg void OnEnSetfocusEditValTargetPrecisionX2();
	afx_msg void OnEnKillfocusEditValTargetPrecisionX2();
	afx_msg void OnEnSetfocusEditValTargetPrecisionY2();
	afx_msg void OnEnKillfocusEditValTargetPrecisionY2();

	afx_msg void OnEnSetfocusEditValCurOffsetIndex();
	afx_msg void OnEnKillfocusEditValCurOffsetIndex();
	afx_msg void OnEnSetfocusEditValTargetPrecisionMaxTimes();
	afx_msg void OnEnKillfocusEditValTargetPrecisionMaxTimes();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnDlgdestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCalcProductSize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnAlignOffsetByAreaSet();
	afx_msg void OnEnChangeEditValCurOffsetIndex();
	// 切换夹爪补偿设置
	int m_strEditGrip;
	afx_msg void OnBnClickedRadEditGrip0();
	afx_msg void OnBnClickedRadEditGrip1();
	afx_msg void OnBnClickedBtnCalibCopy();
};
