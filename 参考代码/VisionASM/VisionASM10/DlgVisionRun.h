#pragma once
#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "Label.h"
#include "svGuiText.h"
#include "svGuiCross.h"
#include "afxwin.h"
#include "KeyBoard.h"
#include "LogFileSave.h"
#include "Layout.h"
#include "afxcmn.h"

#define WM_DLGDESTROY  WM_USER + 300     // 销毁非模态窗体
#define WM_CALCSIZE    WM_USER + 301     // 计算产品尺寸


// CDlgVisionRun 对话框
class CDlgVisionRunAlignInfo;
class CDlgVisionRunProductInfo;
class CDlgVisionRunCommInfo;
class CDlgVisionRunBendInspectInfo;

class CDlgCalibCheckBySize;
class CDlgCalibCheckByFixedTargetAligner;

class CDlgVisionRun : public CDialog
{
	DECLARE_DYNAMIC(CDlgVisionRun)

public:
	CDlgVisionRun(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionRun();


// 对话框数据
	enum { IDD = IDD_DLG_VISION_RUN };



protected:
	void InitUILayout();				// 初始化布局显示
	void InitUIShow();					// 初始化控件显示
	void InitTabUI();					// 初始化Tab页
	void UpdateUIShow();				// 更新控件显示
	void SetItemFont();				    // 设置Edit和Button控件字体显示

private:
	CLayout* m_pMainLayout;				// 布局管理器
	CLayout* m_pTabLayout;				// Tab页布局

public:
	int	m_nPlatformID;
	CVisionASMDlg*		m_pParent;				// 父窗口
	vcBaseVisionAlign*	m_pVisionASM;		// 视觉对位工具

	// TAB标签页对话框
    CDlgVisionRunProductInfo*		m_pDlgVisionRunProductInfo;			// 对位中的产品尺寸信息
	CDlgVisionRunAlignInfo*			m_pDlgVisionRunAlignInfo;			// 对位信息窗口
	CDlgVisionRunCommInfo*			m_pDlgVisionRunCommInfo;			// 运行通信信息
	CDlgVisionRunBendInspectInfo*	m_pDlgVisionRunBendInspectInfo;		// 折弯检测信息

public:
	// 基础信息
	void EnableSysKeyboard(BOOL bEnable);
	void UpdateDialogLanguage();
	void UpdateDlgProductInfo();
	CWnd* SetParentWnd(CWnd* pParent);
	void SetVisionASMRef(vcBaseVisionAlign* pVisionASM);

	void UpdateAlignerProcessInfo();
	void GetCommCommandInfo();

	// 更新产品
	void UpdateCurProductAlignInfo();
	void UpdateDlgData();

	// 对位信息相关
	void UpdateAlignPrecisionInfo();
	void UpdateAlignOffsetInfo();


	// 通信信息相关
	// 产品信息相关
	// 检测信息相关

	void SetRunCommInfoShow();

protected:
	CString			m_strPlatformID;		// platform ID
	int				m_nCombocProductIndex;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;
	CFont			m_MyFont;

public:
	//加载补偿图片
	void LoadOffsetDirectionPicture();
	void SetPlatformID(LPCTSTR strID);
	void SetPlatformID(int nID);
	int  GetPlatformID();
	void EnableDlgItem(BOOL bEnable);
	void UpdateUserType(int nUserType, BOOL bEnable);
	void UpdateDlgParamItem();
	CString GetDlgCtrlString(int nCtrlID);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CLabel	m_labelProductName;
	CLabel	m_labelProductIndex;

	CComboBox	m_comboProduct;
	CComboBox m_comboProductFa;

	CString	m_strProductName;
	CString	m_strProductIndex;
	CTabCtrl m_tabVisionRunInfo;
	//int m_nRadioCommandMode;
	//CEdit m_editLog;

// 	CDlgCalibCheckBySize* m_pDlgCalibCheckBySize;
// 	CDlgCalibCheckByFixedTargetAligner* m_pDlgCalibCheckByFixedTargetAligner;

	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboProduct();
	afx_msg void OnCbnSelchangeComboProductFa();
	afx_msg void OnBnClickedBtnLoadProduct();
	afx_msg void OnBnClickedBtnSinglePlatformProductManage();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioProductInfo();
	afx_msg void OnBnClickedRadioOffsetDirection();
	afx_msg void OnTcnSelchangeTabVisionRunInfo(NMHDR *pNMHDR, LRESULT *pResult);

};
