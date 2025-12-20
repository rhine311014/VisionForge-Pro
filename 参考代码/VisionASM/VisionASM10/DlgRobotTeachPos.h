#pragma once
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "KeyBoard.h"

// CDlgRobotTeachPos 对话框
class CDlgRobotTeach;
class CDlgRobotTeachPos : public CDialog
{
	DECLARE_DYNAMIC(CDlgRobotTeachPos)

public:
	CDlgRobotTeachPos(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotTeachPos();

// 对话框数据
	enum { IDD = IDD_DLG_ROBOT_TEACH_POS };

public:
	CDlgRobotTeach  *m_pParent;
	int				m_nPageIndex;
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;


	void UpdateDlgData(BOOL bSave);									// 更新数据
	void UpdateDlgDataTeachPosPos(BOOL bSave, int nPosIndex);		// 更新示教点位置
	void EnableDlgItem(BOOL bEnable);

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();

public:
	// 机器人点位信息字符串队列
	std::vector<CString*> m_vstrPosX;
	std::vector<CString*> m_vstrPosY;
	std::vector<CString*> m_vstrPosR;
	std::vector<CString*> m_vstrPosZ;

	CFont				m_MyFont;
	CFont				m_MyBtnFont;
	void SetItemFont();				// 设置Edit和Button控件字体显示

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strPosX1;
	CString m_strPosY1;
	CString m_strPosR1;
	CString m_strPosZ1;
	CString m_strPosX2;
	CString m_strPosY2;
	CString m_strPosR2;
	CString m_strPosZ2;
	CString m_strPosX3;
	CString m_strPosY3;
	CString m_strPosR3;
	CString m_strPosZ3;
	CString m_strPosX4;
	CString m_strPosY4;
	CString m_strPosR4;
	CString m_strPosZ4;
	CString m_strPosX5;
	CString m_strPosY5;
	CString m_strPosR5;
	CString m_strPosZ5;
	afx_msg void OnBnClickedBtnSavePos1();
	afx_msg void OnBnClickedBtnSavePos2();
	afx_msg void OnBnClickedBtnSavePos3();
	afx_msg void OnBnClickedBtnSavePos4();
	afx_msg void OnBnClickedBtnSavePos5();
	afx_msg void OnBnClickedBtnMovePos1();
	afx_msg void OnBnClickedBtnMovePos2();
	afx_msg void OnBnClickedBtnMovePos3();
	afx_msg void OnBnClickedBtnMovePos4();
	afx_msg void OnBnClickedBtnMovePos5();
};
