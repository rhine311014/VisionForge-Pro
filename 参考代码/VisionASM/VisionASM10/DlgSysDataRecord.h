#if !defined(AFX_DLGSYSDATARECORD_H__125B9AA9_A872_4A56_997A_BC1800386A5F__INCLUDED_)
#define AFX_DLGSYSDATARECORD_H__125B9AA9_A872_4A56_997A_BC1800386A5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSysDataRecord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSysDataRecord dialog
#include "vsVisionAlignDef.h"
#include "KeyBoard.h"
#include "afxwin.h"
class CDlgSysEnvironSetting;
class CDlgSysDataRecord : public CDialog
{
// Construction
public:
	CDlgSysDataRecord(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSysDataRecord)
	enum { IDD = IDD_SYS_DATA_RECORD };
	
	BOOL	m_bAlignerPrecision;
	BOOL	m_bAlignerTime;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSysDataRecord)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSysDataRecord)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	SysDataRecord m_sysDataRecord;
	CStringArray  *m_psaSysInfoStrings;
	CDlgSysEnvironSetting *m_pParent;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	void UpdateDialogLanguage();


public:

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	SysDataRecord GetSysDataRecord() const;
	void SetSysDataRecord(const SysDataRecord& dataRecord);

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }

public:
	BOOL m_bTempStartUpShowCamSN;
	BOOL m_bTempSysKeyboard;
	


public:

	int m_nPlatformIndex;
	void SetPlatformIndex(int nPlatformIndex);

	bool m_bEnableContent;
	bool m_bEnableSaveImg;		// 存图
	bool m_bEnableSaveVdb;
	bool m_bEnablePathSet;		
	bool m_bEnableSaveByHour;

	bool m_bShowContent;
	bool m_bShowSaveImg;
	bool m_bShowSaveVdb;
	bool m_bShowPathSet;
	bool m_bShowSaveByHour;


	BOOL m_bCustomLogPath;
	CString m_strDataRecordPath;
	BOOL m_bDataRecordTime;
	int m_nDataRecordTimeNum;
	double m_dDiskSpaceMaxUsePercent;

	afx_msg void OnBnClickedCheckDataRecordTime();	
	afx_msg void OnEnSetfocusEditDataRecordTimeNum();
	afx_msg void OnEnKillfocusEditDataRecordTimeNum();
	afx_msg void OnEnSetfocusEditDiskSpaceMaxUsePercent();
	afx_msg void OnEnKillfocusEditDiskSpaceMaxUsePercent();
	afx_msg void OnBnClickedBtnSetLogPath();	
	afx_msg void OnBnClickedChkCustomLogPath();

//	BOOL m_bUseThreadSaveImageMode;


	// VDB文件
	BOOL m_bSaveVDBFile;
	int m_nVBDImageType;
	int m_nSaveVDBMaxNums;
	afx_msg void OnBnClickedCheckSaveVdbFile();	
	afx_msg void OnEnSetfocusEditSaveVdbNum2();
	afx_msg void OnEnKillfocusEditSaveVdbNum2();


	BOOL m_bAlignerSearchPltfmInfo;	// 流程日志
	BOOL m_bCommunication;			// 通信日志
	BOOL m_bAlignToolLog;			// 对位工具日志

	BOOL m_bImageName;				// 产品ID命名图像
	BOOL m_bSaveNGImage;			// 存储NG图像
	int  m_nSaveFailedImageNum;		// 存储NG图像数量
	BOOL m_bSaveAllImage;			// 存储所有图像
	int m_nSaveAllImageNum;			// 存储所有图像数量
	BOOL m_bSaveAllImageType;		// 存储所有图像的方式：0原始存图方式；1线程存图方式
	CComboBox m_CmbLogImageSaveFormat;
	CString m_strJpgQuality;
	BOOL	m_bVDBNameByID;				// 产品ID命名VDB
	CString m_strVDBJpgQuality;
	BOOL m_bSaveTargetSnapVDBFile;//目标拍照存VDB
	BOOL m_bSaveObjectSnapVDBFile;//对象拍照存VDB
	// 存储流程拍照VDB文件
	BOOL m_bSaveProcessSnapVDBFile;

	afx_msg void OnCbnSelchangeCmbImageFormat();
	afx_msg void OnEnSetfocusEditQuality();
	afx_msg void OnEnKillfocusEditQuality();
	afx_msg void OnEnSetfocusEditSaveFailedImageNum();
	afx_msg void OnEnKillfocusEditSaveFailedImageNum();
	afx_msg void OnBnClickedCheckImageSave();
	afx_msg void OnBnClickedCheckAlignerSearchPltfmInfo();
	afx_msg void OnBnClickedCheckCommunication();

	afx_msg void OnSetfocusEditSaveAllImageNum();
	afx_msg void OnKillfocusEditSaveAllImageNum();
	afx_msg void OnBnClickedCheckAllImageSave();
	afx_msg void OnEnKillfocusEditQualityVdb();
	afx_msg void OnEnSetfocusEditQualityVdb();
	afx_msg void OnBnClickedRadioVdbImageType3();
	afx_msg void OnBnClickedCheckVdbIdName();
	afx_msg void OnBnClickedCheckSaveVdbFileObj();
	afx_msg void OnBnClickedCheckSaveVdbFileTarget();
	afx_msg void OnBnClickedCheckSaveVdbFileProcess();
	
	afx_msg void OnEnSetfocusEditSaveGuitextWeight();
	afx_msg void OnEnKillfocusEditSaveGuitextWeight();
	// 字体大小
	int m_GuitextWeight;
	afx_msg void OnEnSetfocusEditSaveGuiLineWidth();
	afx_msg void OnEnKillfocusEditSaveGuiLineWidth();
	// //VDB线宽
	int m_GuiLineWidth;
	afx_msg void OnBnClickedCheckSvaeInspectResultVdb();
	BOOL m_bSaveInsepectResultVDBFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSDATARECORD_H__125B9AA9_A872_4A56_997A_BC1800386A5F__INCLUDED_)
