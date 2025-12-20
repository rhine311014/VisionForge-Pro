#if !defined(AFX_DLGPATTERNCENTERSET_H__99BF7C4A_8DF9_4EB5_ACD7_E69689BEE8C6__INCLUDED_)
#define AFX_DLGPATTERNCENTERSET_H__99BF7C4A_8DF9_4EB5_ACD7_E69689BEE8C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPatternCenterSet.h : header file
//
#include "svGuiDisplay.h"
#include "svGuiInteractiveContainer.h"
#include "svGuiCross.h"
#include "svImage.h"
#include "BtnST.h"
#include "KeyBoard.h"
#include "LogFileSave.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPatternCenterSet dialog

class CDlgPatternCenterSet : public CDialog
{
// Construction
public:
	CDlgPatternCenterSet(cpImage& imageModel, double dMarkX, double dMarkY, CWnd* pParent = NULL);   // standard constructor
	CDlgPatternCenterSet(cpImage& imageModel, std::vector<sc2Vector> vecRoiMarkXY, cpImage& imageTrain, 
		scRect rcTrain, bool boIsMultipleModelPoint = FALSE, CWnd* pParent = NULL);	//多标记点工具调用此构造函数
// Dialog Data
	//{{AFX_DATA(CDlgPatternCenterSet)
	enum { IDD = IDD_DLG_PATTERN_CENTER_SET };
//	CComboBox	m_comboBlobMode;
	CString	m_strStepLength;
	CXPStyleButtonST m_btnMoveUp;
	CXPStyleButtonST m_btnMoveDown;
	CXPStyleButtonST m_btnMoveLeft;
	CXPStyleButtonST m_btnMoveRight;
//	CXPStyleButtonST m_btnCenterAbstract;
	CXPStyleButtonST m_btnOK;
	CXPStyleButtonST m_btnCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPatternCenterSet)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPatternCenterSet)
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnLeft();
	afx_msg void OnBtnRight();
	afx_msg LRESULT OnGuiDisplayDrapStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayDrapStart(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusEditStepLength();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CThemeHelperST		m_themeHelper;
	
	scGuiDisplay m_GuiDisplay;
	scGuiCross m_GuiMarkCross;
	scGuiInteractiveContainer *m_pGuiInteractiveContainer;

	cpImage m_imageModel;
	cpImage m_imageTrain;	//全图 zzc
	double m_dMarkX;			//显示的坐标信息X
	double m_dMarkY;			//显示的坐标信息Y
	double m_dStepLength;
	int		m_nMarkType;		//标记点选点的区域类型 zzc
	std::vector<sc2Vector> m_vecMarkXY;		//标记点结果 zzc

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	CStringArray	*m_psaSysInfoStrings;

	void UpdateDialogLanguage();

public:
	void InitDlgItem();
	void UpdateStatusBar();
	void GetPatternCenter(double& dMarkX, double& dMarkY);
	BOOL CheckMoveStepValid();

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }


	void OnUp();
	void OnDown();
	void OnLeft();
	void OnRight();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	BOOL		m_boIsMultipleModelPoint;	// 标识是否为多标记点工具 zzc
	int			m_nMoveMode;				// 点击上下左右按钮移动方式：连续模式和单动模式
	CComboBox	m_comboMarkTypeSel;			// 提取标记点类型，ROI或者是全图 zzc
	int			m_nMarkNum;					// 标记点数量zzc
	int			m_nMarkPosIndex;			// 标记点位置【1-4】,对应值【0-3】 zzc
	int			m_nMarkPosIndexOld;			// 上一次操作的标记点的位置序号 zzc
	scRect		m_rcTrain;					//训练区域大小 zzc
	std::vector<sc2Vector>		m_vecCurrentRoiMarkXY;	//当前标记点ROI位置 zzc
	std::vector<sc2Vector>		m_vecCurrentModelMarkXY;//当前标记点全图位置 zzc
	std::vector<scGuiCross*>	m_vpGuiMarkCross;		//标记点十字线容器，多标记点工具时使用 zzc
	std::vector<scGuiText*>		m_vpGuiMarkText;		//标记点文本容器，因标记点十字线长度固定，在ROI选点情况下，难以看到十字线的label，zzc
	afx_msg void OnCbnSelchangeComboMarkTypeSel();		//根据选择的标记点类型，显示不同的图像区域 zzc
	afx_msg void OnBnClickedRadioIndex1();				//标记点序号 zzc
	void GetPatternCenter(std::vector<sc2Vector>& vecMarkXY, int &nMarkType);	//对于多标记点工具，重载此函数，非多标记点工具不用 zzc
	void UpdateRadioButtonShow();						//根据标记点数量更新Radiobutton的显示 zzc
	afx_msg void OnBnClickedBtnShowAll();				//如果是多个标记点，显示全部十字线 zzc
	void MarkPosUpdate(double dMarkX,double dMarkY,int nPosIndex = 0);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPATTERNCENTERSET_H__99BF7C4A_8DF9_4EB5_ACD7_E69689BEE8C6__INCLUDED_)
