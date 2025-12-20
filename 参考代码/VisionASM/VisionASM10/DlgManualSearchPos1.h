#pragma once

#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "afxwin.h"
#include "Label.h"
#include "BtnST.h"
#include "KeyBoard.h"

// CDlgManualSearchPos1 对话框

class CDlgManualSearchPos1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualSearchPos1)

public:
	CDlgManualSearchPos1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgManualSearchPos1();

// 对话框数据
	enum { IDD = IDD_DLG_MANUAL_SEARCH_POS1 };

	vcBaseVisionAlign *m_pVisionASM;
	int	m_nCurPlatformIndex;
	int m_nMarkType;	//0对象,1目标

	CWnd *m_pVAPrevMainWnd;
	std::vector<scGuiDisplay*> m_vpPrevGuiDisplay;
	std::vector<scGuiDisplay*> m_vpGuiDisplay;		// GUI显示控件
	std::vector<scGuiCross*>   m_vpGuiCross;		// GUI显示控件
	std::vector<CCoordPos>	   m_vCoordPos;			// 手动选标结果
	std::vector<int>		   m_vnPosIndex;		// 手动选标位置序号

	void SetVisionAlignerRef(vcBaseVisionAlign *pVisionAligner, std::vector<int> vnPosIndex);

	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;

	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void UpdateDialogLanguage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void InitDlgItem();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnGuiDisplayClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
};
