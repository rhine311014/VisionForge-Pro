#pragma once

#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "Label.h"
#include "KeyBoard.h"
// CDlgVisionRunProductInfo 对话框

class CDlgVisionRunProductInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVisionRunProductInfo)

public:
	CDlgVisionRunProductInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVisionRunProductInfo();

// 对话框数据
	enum { IDD = IDD_DLG_VISION_RUN_PRODUCT_INFO };

public:
	CDlgVisionRun*		m_pParent;			// 父窗口
	vcBaseVisionAlign*	m_pVisionASM;		// 视觉对位工具

	AlignerTargetSizeInfo m_AlignerTargetSizeInfo;		// 对位目标距离信息
	AlignerObjectSizeInfo m_AlignerObjectSizeInfo;		// 对位对象距离信息
	AlignerTargetAngleInfo m_AlignerTargetAngleInfo;	// 对位目标角度信息
	AlignerObjectAngleInfo m_AlignerObjectAngleInfo;	// 对位对象角度信息

	CAlignCheckInfo		m_AlignCheckInfo;

public:
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	BOOL IsStringPositiveNumerical(LPCTSTR lpszVal);
	BOOL IsStringNumerical(LPCTSTR lpszVal);
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
    void UpdateDialogLanguage();
	CFont				m_MyFont;
	void SetItemFont();				// 设置Edit和Button控件字体显示
public:
	CWnd*	SetParentWnd(CDlgVisionRun* pParent);
	void	SetVisionASMRef(vcBaseVisionAlign* pVisionASM);

	void	MoveDlgItem(BOOL bMoveViewOnly = FALSE);

	void	UpdateCurProductInfo();
	void	UpdateAlignerTargetSizeInfo();			// 更新对位目标距离信息
	void    UpdateAlignerObjectSizeInfo();			// 更新对位对象距离信息
	void	UpdateAlignerTargetStdSizeInfo();		// 更新对位目标标准距离信息
	void    UpdateAlignerObjectStdSizeInfo();		// 更新对位对象标准距离信息

	void	UpdateAlignerTargetStdAngleInfo();		// 更新对位目标标准距离信息
	void    UpdateAlignerObjectStdAngleInfo();		// 更新对位对象标准距离信息
	void	UpdateAlignerTargetAngleInfo();			// 更新对位目标距离信息
	void    UpdateAlignerObjectAngleInfo();			// 更新对位对象距离信息



	void    UpdateDlgItem();
	void    UpdateDlgParamItem();
	void    UpdateDlgItemText();

	BOOL    SetAlignerTargetSizeStr(CString strSizeName,int nSizeIndex);   
	BOOL    SetAlignerObjectSizeStr(CString strSizeName,int nSizeIndex);  
	BOOL    SetAlignerTargetAngleStr(CString strSizeName,int nSizeIndex);   
	BOOL    SetAlignerObjectAngleStr(CString strSizeName,int nSizeIndex);  

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CLabel	m_labelCurObjectSizeCheckStatus0;
	CLabel	m_labelCurObjectSizeCheckStatus1;
	CLabel	m_labelCurObjectSizeCheckStatus2;
	CLabel	m_labelCurObjectSizeCheckStatus3;
	CLabel	m_labelCurObjectSizeCheckStatus4;
	CLabel	m_labelCurObjectSizeCheckStatus5;
	CLabel	m_labelCurObjectAngleCheckStatus0;
	CLabel	m_labelCurObjectAngleCheckStatus1;
	CLabel	m_labelCurObjectAngleCheckStatus2;
	CLabel	m_labelCurObjectAngleCheckStatus3;
// 	CLabel	m_labelCurObjectAngleCheckStatus4;
// 	CLabel	m_labelCurObjectAngleCheckStatus5;

	CLabel	m_labelCurTargetSizeCheckStatus0;
	CLabel	m_labelCurTargetSizeCheckStatus1;
	CLabel	m_labelCurTargetSizeCheckStatus2;
	CLabel	m_labelCurTargetSizeCheckStatus3;
	CLabel	m_labelCurTargetSizeCheckStatus4;
	CLabel	m_labelCurTargetSizeCheckStatus5;
	CLabel	m_labelCurTargetAngleCheckStatus0;
	CLabel	m_labelCurTargetAngleCheckStatus1;
	CLabel	m_labelCurTargetAngleCheckStatus2;
	CLabel	m_labelCurTargetAngleCheckStatus3;
// 	CLabel	m_labelCurTargetAngleCheckStatus4;
// 	CLabel	m_labelCurTargetAngleCheckStatus5;


	// 标准值
	CString	m_strEditObjectStdSize0;
	CString	m_strEditObjectStdSize1;
	CString	m_strEditObjectStdSize2;
	CString	m_strEditObjectStdSize3;
	CString	m_strEditObjectStdSize4;
	CString	m_strEditObjectStdSize5;
	CString	m_strEditObjectStdAngle0;
	CString	m_strEditObjectStdAngle1;
	CString	m_strEditObjectStdAngle2;
	CString	m_strEditObjectStdAngle3;
// 	CString	m_strEditObjectStdAngle4;
// 	CString	m_strEditObjectStdAngle5;


	CString	m_strEditTargetStdSize0;
	CString	m_strEditTargetStdSize1;
	CString	m_strEditTargetStdSize2;
	CString	m_strEditTargetStdSize3;
	CString	m_strEditTargetStdSize4;
	CString	m_strEditTargetStdSize5;
	CString	m_strEditTargetStdAngle0;
	CString	m_strEditTargetStdAngle1;
	CString	m_strEditTargetStdAngle2;
	CString	m_strEditTargetStdAngle3;
// 	CString	m_strEditTargetStdAngle4;
// 	CString	m_strEditTargetStdAngle5;




	// 当前值
	CEdit	m_editCurObjectSize0;
	CEdit	m_editCurObjectSize1;
	CEdit	m_editCurObjectSize2;
	CEdit	m_editCurObjectSize3;
	CEdit	m_editCurObjectSize4;
	CEdit	m_editCurObjectSize5;
	CEdit	m_editCurObjectAngle0;
	CEdit	m_editCurObjectAngle1;
	CEdit	m_editCurObjectAngle2;
	CEdit	m_editCurObjectAngle3;
// 	CEdit	m_editCurObjectAngle4;
// 	CEdit	m_editCurObjectAngle5;

	CEdit	m_editCurTargetSize0;
	CEdit	m_editCurTargetSize1;
	CEdit	m_editCurTargetSize2;
	CEdit	m_editCurTargetSize3;
	CEdit	m_editCurTargetSize4;
	CEdit	m_editCurTargetSize5;
	CEdit	m_editCurTargetAngle0;
	CEdit	m_editCurTargetAngle1;
	CEdit	m_editCurTargetAngle2;
	CEdit	m_editCurTargetAngle3;
// 	CEdit	m_editCurTargetAngle4;
// 	CEdit	m_editCurTargetAngle5;




	// 当前值
	CString	m_strEditCurObjectSize0;
	CString	m_strEditCurObjectSize1;
	CString	m_strEditCurObjectSize2;
	CString	m_strEditCurObjectSize3;
	CString	m_strEditCurObjectSize4;
	CString	m_strEditCurObjectSize5;
	CString	m_strEditCurObjectAngle0;
	CString	m_strEditCurObjectAngle1;
	CString	m_strEditCurObjectAngle2;
	CString	m_strEditCurObjectAngle3;
// 	CString	m_strEditCurObjectAngle4;
// 	CString	m_strEditCurObjectAngle5;

	CString	m_strEditCurTargetSize0;
	CString	m_strEditCurTargetSize1;
	CString	m_strEditCurTargetSize2;
	CString	m_strEditCurTargetSize3;
	CString	m_strEditCurTargetSize4;
	CString	m_strEditCurTargetSize5;
	CString	m_strEditCurTargetAngle0;
	CString	m_strEditCurTargetAngle1;
	CString	m_strEditCurTargetAngle2;
	CString	m_strEditCurTargetAngle3;
// 	CString	m_strEditCurTargetAngle4;
// 	CString	m_strEditCurTargetAngle5;





public:
	virtual BOOL OnInitDialog();

	afx_msg void OnEnSetfocusEditValTargetStdSize0();
	afx_msg void OnEnSetfocusEditValTargetStdSize1();
	afx_msg void OnEnSetfocusEditValTargetStdSize2();
	afx_msg void OnEnSetfocusEditValTargetStdSize3();
	afx_msg void OnEnSetfocusEditValTargetStdSize4();
	afx_msg void OnEnSetfocusEditValTargetStdSize5();
	afx_msg void OnEnSetfocusEditValTargetStdAngle0();
	afx_msg void OnEnSetfocusEditValTargetStdAngle1();
	afx_msg void OnEnSetfocusEditValTargetStdAngle2();
	afx_msg void OnEnSetfocusEditValTargetStdAngle3();

	afx_msg void OnEnKillfocusEditValTargetStdSize0();
	afx_msg void OnEnKillfocusEditValTargetStdSize1();
	afx_msg void OnEnKillfocusEditValTargetStdSize2();
	afx_msg void OnEnKillfocusEditValTargetStdSize3();
	afx_msg void OnEnKillfocusEditValTargetStdSize4();
	afx_msg void OnEnKillfocusEditValTargetStdSize5();
	afx_msg void OnEnKillfocusEditValTargetStdAngle0();
	afx_msg void OnEnKillfocusEditValTargetStdAngle1();
	afx_msg void OnEnKillfocusEditValTargetStdAngle2();
	afx_msg void OnEnKillfocusEditValTargetStdAngle3();



	afx_msg void OnEnSetfocusEditValObjectStdSize0();
	afx_msg void OnEnSetfocusEditValObjectStdSize1();
	afx_msg void OnEnSetfocusEditValObjectStdSize2();
	afx_msg void OnEnSetfocusEditValObjectStdSize3();
	afx_msg void OnEnSetfocusEditValObjectStdSize4();
	afx_msg void OnEnSetfocusEditValObjectStdSize5();
	afx_msg void OnEnSetfocusEditValObjectStdAngle0();
	afx_msg void OnEnSetfocusEditValObjectStdAngle1();
	afx_msg void OnEnSetfocusEditValObjectStdAngle2();
	afx_msg void OnEnSetfocusEditValObjectStdAngle3();



	afx_msg void OnEnKillfocusEditValObjectStdSize0();
	afx_msg void OnEnKillfocusEditValObjectStdSize1();
	afx_msg void OnEnKillfocusEditValObjectStdSize2();
	afx_msg void OnEnKillfocusEditValObjectStdSize3();
	afx_msg void OnEnKillfocusEditValObjectStdSize4();
	afx_msg void OnEnKillfocusEditValObjectStdSize5();
	afx_msg void OnEnKillfocusEditValObjectStdAngle0();
	afx_msg void OnEnKillfocusEditValObjectStdAngle1();
	afx_msg void OnEnKillfocusEditValObjectStdAngle2();
	afx_msg void OnEnKillfocusEditValObjectStdAngle3();



	int m_nMultiCalibExtensionIndex;//扩展产品序号
	CComboBox m_comboMultiCalibExtension;
	afx_msg void OnCbnSelchangeCombo1();
	void UpdateStdSizeExProduct();//切换扩展产品序号时，更新扩展产品的检查模式标准值
};
