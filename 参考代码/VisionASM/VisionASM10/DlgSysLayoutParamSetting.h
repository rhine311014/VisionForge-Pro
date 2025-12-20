#pragma once
#include "vsVisionAlignDef.h"
#include "KeyBoard.h"
#include "vsBaseVisionAlign.h"
#include "DlgViewPosLayoutMode.h"

// CDlgSysLayoutParamSetting 对话框

class CDlgSysEnvironSetting;
class CDlgSysLayoutParamSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysLayoutParamSetting)

public:
	CDlgSysLayoutParamSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysLayoutParamSetting();

// 对话框数据
	enum { IDD = IDD_DLG_SYS_LAYOUT_PARAM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) {  m_psaSysInfoStrings = psaSysInfoStrings; };
	void SetParentWnd(CDlgSysEnvironSetting *pParent) { m_pParent = pParent; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetPlatformLayoutInfo(CPlatformLayoutInfo layoutInfo);
	CPlatformLayoutInfo GetPlatformLayoutInfo();
	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);
	void UpdateDlgData(BOOL bValidUpdate=TRUE);
	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	

private:
	CStringArray* m_psaSysInfoStrings;
	CDlgSysEnvironSetting	*m_pParent;
	CPlatformLayoutInfo	m_PlatformLayoutInfo;
	vcBaseVisionAlign* m_pVisionASM;
	BOOL m_bSysKeyboardEnabled;
	BOOL m_bKillFocusEnabled;
	CKeyBoard m_ctrlKeyboard;

	// 布局模式显示
protected:
	void InitViewPosLayoutMode();

private:
	CDlgViewPosLayoutMode* m_pDlgViewPosLayoutMode;
	std::vector<CLayout*>  m_vpViewPosLayout;

public:
	afx_msg void OnEnSetfocusEditVisionRunDlgWidth();
	afx_msg void OnEnKillfocusEditVisionRunDlgWidth();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nRdViewLayoutType;
	CString m_strVisionRunDlgWidth;

	void UpdateDialogLanguage();			// 更新语言包
	int m_nRdViewRunShowType;
};
