#pragma once

#include "Keyboard.h"
#include "vsVisionAlignDef.h"
#include "afxwin.h"

// CDlgCommTypeSetting 对话框
class CDlgSysEnvironSetting;
class CDlgCommTypeSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommTypeSetting)

public:
	CDlgCommTypeSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCommTypeSetting();

// 对话框数据
	enum { IDD = IDD_SYS_COMM_PARAM_SETTING };

	BOOL	m_bSysKeyboardEnabled;
	int		m_nLanguage;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }

	CCommParamInfo GetCCommParamInfo()const;
	void SetCCommParamInfo(const CCommParamInfo &commParamInfo);

	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM){m_pVisionASM = pVisionASM;}
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CStringArray *m_psaSysInfoStrings;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CDlgSysEnvironSetting	*m_pParent;

	void UpdateDialogLanguage();

	void InitDlgItem();

	CCommParamInfo m_commParamInfo;
	vcBaseVisionAlign *m_pVisionASM;

	std::vector<pair<int,CString>> strArrayProtocolTypeVal;

	void ShowPLCProtocolType(BOOL bVisible); // 非寄存器协议时，部分控件隐藏

	void ShowShareCommControl(BOOL bVisible); // 共享通信工位控件显示隐藏
public:
	int m_nCommType;
	BOOL m_bRegiterType;
	CComboBox m_ComboProtoccolType;
	CComboBox m_ComboExProtoccolType;
	int m_nRegDataTypeIndex;
	int m_nCmdParamOrderType;
	CComboBox m_ComboMotionControlType;
	BOOL m_bUseSpecialComm;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboProtocolType();
	afx_msg void OnBnClickedRadSerialType();
	afx_msg void OnBnClickedRadSocketType();

	BOOL ChangeCommType(const CommType eCommType);

	void SetGroupBoxStatus(int nID,BOOL bEnable,BOOL bShow);
	void UpdateDlgByAuthorize();	// 根据权限刷新界面控件显示隐藏、启用禁用

	bool m_bshowCommTypeSet;
	bool m_bEnableCommTypeSet;

	bool m_bshowCommOtherSet;
	bool m_bEnableCommOtherSet;
	CComboBox m_ComboOrderType;
};
