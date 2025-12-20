#pragma once
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "KeyBoard.h"

// CDlgRobotTeachSG 对话框
class CDlgRobotTeach;
class CDlgRobotTeachSG : public CDialog
{
	DECLARE_DYNAMIC(CDlgRobotTeachSG)

public:
	CDlgRobotTeachSG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotTeachSG();

// 对话框数据
	enum { IDD = IDD_DLG_ROBOT_TEACH_SG };


public:
	CDlgRobotTeach  *m_pParent;
	int				m_nPageIndex;
	CStringArray	*m_psaSysInfoStrings;
	void UpdateDlgData(BOOL bSave);									// 更新数据
	void UpdateDlgDataTeachSGI(BOOL bSave, int nPosIndex);		// 更新示教点位置
	void UpdateDlgDataTeachSGR(BOOL bSave, int nPosIndex);		// 更新示教点位置
	void EnableDlgItem(BOOL bEnable);

	//void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
public:
	// 机器人点位信息字符串队列
	std::vector<CString*> m_vstrPosX;
	std::vector<CString*> m_vstrPosY;
	std::vector<CString*> m_vstrPosR;
	std::vector<CString*> m_vstrPosZ;

	// 机器人点位信息字符串队列
	std::vector<CString*> m_vstrSGInt;
	std::vector<CString*> m_vstrSGReal;

	CFont				m_MyFont;
	CFont				m_MyBtnFont;
	void SetItemFont();				// 设置Edit和Button控件字体显示

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }

	BOOL IsStringNumerical(LPCTSTR lpszVal);
	BOOL IsSGIOutRange(LPCTSTR lpszVal, int nPosIndex);	 // 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围
	BOOL IsSGROutRange(LPCTSTR lpszVal, int nPosIndex);	 // 根据机器人当前位置以及预先设定好的范围，判断当前位置是否超出了范围
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strSGInt1;
	CString m_strSGInt2;
	CString m_strSGInt3;
	CString m_strSGInt4;
	CString m_strSGInt5;
	CString m_strSGReal1;
	CString m_strSGReal2;
	CString m_strSGReal3;
	CString m_strSGReal4;
	CString m_strSGReal5;
	CString m_strPosZ3;
	CString m_strPosX4;
	CString m_strPosY4;
	CString m_strPosR4;
	CString m_strPosZ4;
	CString m_strPosX5;
	CString m_strPosY5;
	CString m_strPosR5;
	CString m_strPosZ5;
	afx_msg void OnBnClickedBtnSaveSG();
	afx_msg void OnEnSetfocusEditSgi1();
	afx_msg void OnEnKillfocusEditSgi1();
	afx_msg void OnEnSetfocusEditSgi2();
	afx_msg void OnEnKillfocusEditSgi2();
	afx_msg void OnEnSetfocusEditSgi3();
	afx_msg void OnEnKillfocusEditSgi3();
	afx_msg void OnEnSetfocusEditSgi4();
	afx_msg void OnEnKillfocusEditSgi4();
	afx_msg void OnEnSetfocusEditSgi5();
	afx_msg void OnEnKillfocusEditSgi5();

	afx_msg void OnEnSetfocusEditSgr1();
	afx_msg void OnEnKillfocusEditSgr1();
	afx_msg void OnEnSetfocusEditSgr2();
	afx_msg void OnEnKillfocusEditSgr2();
	afx_msg void OnEnSetfocusEditSgr3();
	afx_msg void OnEnKillfocusEditSgr3();
	afx_msg void OnEnSetfocusEditSgr4();
	afx_msg void OnEnKillfocusEditSgr4();
	afx_msg void OnEnSetfocusEditSgr5();
	afx_msg void OnEnKillfocusEditSgr5();
	afx_msg void OnBnClickedCheckEnableSgi1();
	afx_msg void OnBnClickedCheckEnableSgi2();
	afx_msg void OnBnClickedCheckEnableSgi3();
	afx_msg void OnBnClickedCheckEnableSgi4();
	afx_msg void OnBnClickedCheckEnableSgi5();
	BOOL m_bCheckEnableSGI1;
	BOOL m_bCheckEnableSGI2;
	BOOL m_bCheckEnableSGI3;
	BOOL m_bCheckEnableSGI4;
	BOOL m_bCheckEnableSGI5;
};
