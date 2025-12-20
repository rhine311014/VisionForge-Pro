#pragma once

#include "KeyBoard.h"

// CDlgCalibCopy 对话框

class CDlgCalibCopy : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibCopy)

public:
	CDlgCalibCopy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibCopy();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_COPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nSourcePosCalib;
	int m_nTargetPosCalib;
	afx_msg void OnEnSetfocusEditSourcePosCalib();
	afx_msg void OnEnSetfocusEditTargetPosCalib();

	BOOL		m_bSysKeyboardEnabled;	  // 是否开启虚拟键盘
	CKeyBoard	m_ctrlKeyboard;		  // 虚拟键盘
	afx_msg void OnBnClickedCheckAllCalibCopy();
	BOOL m_bEnableALLCalibCopy;
	virtual BOOL OnInitDialog();
};
