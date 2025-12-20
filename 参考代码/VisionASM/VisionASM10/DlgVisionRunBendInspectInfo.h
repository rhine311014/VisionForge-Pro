#pragma once

#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "Label.h"
#include "KeyBoard.h"

// CDlgVisionRunBendInspectInfo 对话框

#define MAX_ITEM_NUM    2000		// 表一的最大行数
#define MAX_COL_NUM	    35			// 表一的最大列数

class CDlgVisionRunBendInspectInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgVisionRunBendInspectInfo)

public:
	CDlgVisionRunBendInspectInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionRunBendInspectInfo();

// 对话框数据
	enum { IDD = IDD_DLG_VISION_RUN_BEND_INSPECT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	CWnd*	SetParentWnd(CDlgVisionRun* pParent);
	void	SetVisionASMRef(vcBaseVisionAlign* pVisionASM);

	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void UpdateDlgParamItem();
	void UpdateDialogLanguage();	
	void SetItemFont();

	CDlgVisionRun*			m_pParent;					// 父窗口
	vcBaseVisionAlign*		m_pVisionASM;				// 视觉对位工具
	BendInspectResultInfo	m_BendInspectResult;
	AlignerProcessInfo      m_apAlignerProcesInfo;		// 对位过程信息

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CFont			m_MyFont;

	// 折弯检测
	void UpdateBendBaseInfo(BOOL bUpdate);
	void UpdateBendBaseResult(BOOL bUpdate = TRUE);
	void UpdateBendBaseListColor();			// 更新表格颜色,NG标红

	afx_msg void OnCustomdrawListBendSimple(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListBendSimple(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListBendSimple(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnKillfocusEditBendShow();
	afx_msg void OnSetfocusEditBendShow();

	CString		m_strListDataSimple[MAX_ITEM_NUM][MAX_COL_NUM];			// 表1的数据
	double		m_bSimpleOK[MAX_ITEM_NUM][MAX_ITEM_NUM];				// 记录是否在范围内，方便标红
	CListCtrl	m_ListBendSimple;	
	CString		m_strBendInsBase;										// 虚拟键盘显示控件
	CEdit		m_editBendInsBase;
	BOOL		m_bSimpleStanderShow;
	int			m_nListSimpleItemNum;  // 行数
	int			m_LastSimpleItem;
	int			m_LastSimpleSubItem;



	// 圆检测
	void UpdateCircleBaseInfo(BOOL bUpdate);
	void UpdateCircleResultSimple(BOOL bUpdate = TRUE);
	void UpdateCircleBaseListColor();			// 更新表格颜色,NG标红

	afx_msg void OnCustomdrawListBendCircle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListBendCircle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListBendCircle(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnKillfocusEditCircleShow();
	afx_msg void OnSetfocusEditCircleShow();

	CString		m_strListDataCircle[MAX_ITEM_NUM][MAX_COL_NUM];			// 表2的数据	
	double		m_bCircleOK[MAX_ITEM_NUM][MAX_ITEM_NUM];				// 记录是否在范围内，方便标红
	CListCtrl	m_ListBendCircle;	
	CString		m_strCircleInsBase;										// 虚拟键盘显示控件
	CEdit		m_editCircleInsBase;
	BOOL		m_bCircleStanderShow;
	int			m_nListCircleItemNum;  // 行数
	int			m_LastCircleItem;
	int			m_LastCircleSubItem;


	// 折弯检测过程详细信息
	void UpdateBendResultDetal(BOOL bUpdate = TRUE);
	afx_msg void OnBnClickedBtnDetalClear();
	afx_msg void OnKillfocusEditMaxRecordNum();
	afx_msg void OnSetfocusEditMaxRecordNum();
	void ClearListDetalAllItem();

	CString m_strListDataDetal[MAX_ITEM_NUM][MAX_COL_NUM];			// 表3的数据

	CListCtrl m_ListBendDetal;
	int m_nRecordNumMax;	
	int m_nListDetaItemNum;		// 行数

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioBendInspect();

	int m_nBendShowInfo;
	BOOL m_bShowBendDlg;

};
