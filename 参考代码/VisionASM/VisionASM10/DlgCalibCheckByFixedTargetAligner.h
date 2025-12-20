#pragma once

#include "vsBaseVisionAlign.h"
#include "KeyBoard.h"
// CDlgCalibCheckByFixedTargetAligner 对话框
class CDlgVisionRunAlignInfo;
class CDlgCalibCheckByFixedTargetAligner : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibCheckByFixedTargetAligner)

public:
	CDlgCalibCheckByFixedTargetAligner(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibCheckByFixedTargetAligner();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_CHECK_BY_FIXED_TARGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	vcBaseVisionAlign *m_pVisionASM;
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	CDlgVisionRunAlignInfo *m_pParent;

	CListCtrl m_ListImagePos;
	CListCtrl m_ListImagePosStatistics;
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void InitDialogBtnAndEditControl();

	// 获取轴方式 0-PLC   1-输入
	int m_nGetAxisOption;
	void InitAxisData();  // 初始化轴数据 采用标定数据进行初始化
	void InitAxisTextInfo();	//初始化轴的标签名称,主要是第三段

	// 根据位置数对象目标等确定 搜索位置数据
	int m_nSeachNums;

	// 扩展产品序号
	CComboBox m_CComboBox_EX_Product;
	int m_nExProductIndex;

	// 列表初始化
	void InitDlgListControl(int nCols);
	void ClearListCtrl(CListCtrl * m_pList);

	// 根据数据项列表更新统计列表
	void UpdateStatisticsList(CListCtrl * m_pList,CListCtrl * m_pListStatistics);
	// 更新list序号列
	void UpdateListColsOrder(CListCtrl * m_pList);
	afx_msg void OnBnClickedRadioGetAxisByComm();
	afx_msg void OnBnClickedRadioGetAxisByInput();
	afx_msg void OnBnClickedBtnTargetSearchPos1();
	afx_msg void OnBnClickedBtnTargetSearchPos2();
	afx_msg void OnBnClickedBtnTargetSearchPos3();
	afx_msg void OnBnClickedBtnTargetSearchPos4();
	afx_msg void OnBnClickedBtnObject2SearchPos1();
	afx_msg void OnBnClickedBtnObject2SearchPos2();
	afx_msg void OnBnClickedBtnObject2SearchPos3();
	afx_msg void OnBnClickedBtnObject2SearchPos4();
	afx_msg void OnBnClickedBtnObjectAlignerCalc();
	afx_msg void OnBnClickedBtnCandidate5ObjectSearchPos1();
	afx_msg void OnBnClickedBtnCandidate5ObjectSearchPos2();
	afx_msg void OnBnClickedBtnCandidate5ObjectSearchPos3();
	afx_msg void OnBnClickedBtnCandidate5ObjectSearchPos4();
	afx_msg void OnNMRClickListObjectSearchImagepos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListDelItem2();
	afx_msg void OnListClearAllItem2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	AlignerAxisInfo	 m_AlignerAxisInfo;             //  轴信息

	void UpdateTargetAxisInfo();
	void UpdateObjectAxisInfo();
	// 当前搜索位置序号
	int m_nCurTargetSearchIndex;

	int m_nCurObjectSearchIndex;

	BOOL m_bCandidateSearch; // 候补搜索

	void ExeObjectSearch(int nPos); // 执行对象位置nPos的搜索

	void ExeTargetSearch(int nPos); // 执行目标位置nPos的搜索
	std::vector<BOOL> m_vbSearchObjectSuss;
	std::vector<BOOL> m_vbSearchTargetSuss;
	
	AlignerProcessInfo		m_AlignerProcessInfo;	// 实时对位信息
	void UpdateProcessInfo();

	std::vector<BOOL> m_vbIsUpdateAllPos; // 是否全部位置的图像坐标均已刷新
	void UpdateImagePosDataList(const std::vector<double> vData);
	void UpdateDialogLanguage();

	CString m_strInfoOld;
	void OnEnSetfocusEdit_Keyboard(int nID);
	void OnEnKillfocusEdit(int nID);
	BOOL IsStringNumerical( LPCTSTR lpszVal );
	BOOL m_bIsReadOnly;
	afx_msg void OnEnSetfocusEditValPosTargetAxisX1();
	afx_msg void OnEnSetfocusEditValPosTargetAxisY1();
	afx_msg void OnEnSetfocusEditValPosTargetAxisD1();
	afx_msg void OnEnSetfocusEditValPosTargetAxisX2();
	afx_msg void OnEnSetfocusEditValPosTargetAxisY2();
	afx_msg void OnEnSetfocusEditValPosTargetAxisD2();
	afx_msg void OnEnSetfocusEditValPosTargetAxisX3();
	afx_msg void OnEnSetfocusEditValPosTargetAxisY3();
	afx_msg void OnEnSetfocusEditValPosTargetAxisD3();

	afx_msg void OnEnSetfocusEditValPosTargetAxisX4();
	afx_msg void OnEnSetfocusEditValPosTargetAxisY4();
	afx_msg void OnEnSetfocusEditValPosTargetAxisD4();

	afx_msg void OnEnSetfocusEditValPosObjectAxisX1();
	afx_msg void OnEnSetfocusEditValPosObjectAxisY1();
	afx_msg void OnEnSetfocusEditValPosObjectAxisD1();

	afx_msg void OnEnSetfocusEditValPosObjectAxisX2();
	afx_msg void OnEnSetfocusEditValPosObjectAxisY2();
	afx_msg void OnEnSetfocusEditValPosObjectAxisD2();

	afx_msg void OnEnSetfocusEditValPosObjectAxisX3();
	afx_msg void OnEnSetfocusEditValPosObjectAxisY3();
	afx_msg void OnEnSetfocusEditValPosObjectAxisD3();

	afx_msg void OnEnSetfocusEditValPosObjectAxisX4();
	afx_msg void OnEnSetfocusEditValPosObjectAxisY4();
	afx_msg void OnEnSetfocusEditValPosObjectAxisD4();

	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisX1();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisY1();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisD1();

	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisX2();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisY2();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisD2();

	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisX3();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisY3();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisD3();

	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisX4();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisY4();
	afx_msg void OnEnSetfocusEditValPosCandidateObjectAxisD4();
	afx_msg void OnCbnSelchangeComboExproductCheckBySize();


	afx_msg void OnEnKillfocusEditValPosTargetAxisX1();
	afx_msg void OnEnKillfocusEditValPosTargetAxisY1();
	afx_msg void OnEnKillfocusEditValPosTargetAxisD1();
	afx_msg void OnEnKillfocusEditValPosTargetAxisX2();
	afx_msg void OnEnKillfocusEditValPosTargetAxisY2();
	afx_msg void OnEnKillfocusEditValPosTargetAxisD2();
	afx_msg void OnEnKillfocusEditValPosTargetAxisX3();
	afx_msg void OnEnKillfocusEditValPosTargetAxisY3();
	afx_msg void OnEnKillfocusEditValPosTargetAxisD3();
	afx_msg void OnEnKillfocusEditValPosTargetAxisX4();
	afx_msg void OnEnKillfocusEditValPosTargetAxisY4();
	afx_msg void OnEnKillfocusEditValPosTargetAxisD4();
	afx_msg void OnEnKillfocusEditValPosObjectAxisX1();
	afx_msg void OnEnKillfocusEditValPosObjectAxisY1();
	afx_msg void OnEnKillfocusEditValPosObjectAxisD1();
	afx_msg void OnEnKillfocusEditValPosObjectAxisX2();
	afx_msg void OnEnKillfocusEditValPosObjectAxisY2();
	afx_msg void OnEnKillfocusEditValPosObjectAxisD2();
	afx_msg void OnEnKillfocusEditValPosObjectAxisX3();
	afx_msg void OnEnKillfocusEditValPosObjectAxisY3();
	afx_msg void OnEnKillfocusEditValPosObjectAxisD3();
	afx_msg void OnEnKillfocusEditValPosObjectAxisX4();
	afx_msg void OnEnKillfocusEditValPosObjectAxisY4();
	afx_msg void OnEnKillfocusEditValPosObjectAxisD4();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisX1();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisY1();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisD1();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisX2();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisY2();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisD2();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisX3();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisY3();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisD3();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisX4();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisY4();
	afx_msg void OnEnKillfocusEditValPosCandidateObjectAxisD4();
};
