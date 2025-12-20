#pragma once

#include "vsBaseVisionAlign.h"
#include "KeyBoard.h"
#include "afxwin.h"
// CDlgCalibCheckBySize 对话框
class CDlgVisionRun;
class CDlgCalibCheckBySize : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibCheckBySize)

public:
	CDlgCalibCheckBySize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibCheckBySize();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_SIZE_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;
	BOOL			m_bKillFocusEnabled;

	CListCtrl m_ListObjecSize;
	//CRect m_RectDisPlay;
	CListCtrl m_ListObjectSizeStatistics;
	CListCtrl m_ListImagePos;
	CListCtrl m_ListImagePosStatistics;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void UpdateDialogLanguage();
	void InitDialogBtnAndEditControl();

	int m_nTypeIndex;  // 标定还是关联    0 -标定验证   1--关联验证

	// 获取轴方式 0-PLC   1-输入
	int m_nGetAxisOption;
	void InitAxisData();  // 初始化轴数据 采用标定数据进行初始化
	void InitAxisTextInfo();	//初始化轴的标签名称
	// 根据位置数对象目标等确定 搜索位置数据

	int m_nSeachNums;

	// 尺寸列表初始化
	void InitDlgListControl(int nCols);
	void ClearListCtrl(CListCtrl * m_pList);

	// 更新数据项列表
	void UpdateImagePosDataList(const int nPos,const std::vector<double> vData);
	// 根据数据项列表更新统计列表
	void UpdateStatisticsList(CListCtrl * m_pList,CListCtrl * m_pListStatistics);
	// 更新list序号列
	void UpdateListColsOrder(CListCtrl * m_pList);

	// 当前搜索位置序号
	int m_nCurSearchIndex;

	// 当前搜索位置序号
	int m_nOldSearchIndex;

    std::vector<BOOL> m_vbSearchSuss;
	afx_msg void OnBnClickedBtnObjectSearchPos1();
	afx_msg void OnBnClickedBtnObjectSearchPos2();

	afx_msg void OnNMRClickListObjectSizeShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListDelItem();
	afx_msg void OnNMRClickListImagepos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListDelItem2();
	afx_msg void OnListClearAllItem2();
	afx_msg void OnListClearAllItem();
	virtual BOOL OnInitDialog();

	vcBaseVisionAlign *m_pVisionASM;
	void SetVisionAlignerRef( vcBaseVisionAlign *pVisionAligner);
	virtual void OnCancel();

	CDlgVisionRunAlignInfo *m_pParent;
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	AlignerTargetSizeInfo m_AlignerTargetSizeInfo;	// 对位目标距离信息
	AlignerObjectSizeInfo m_AlignerObjectSizeInfo;  // 对位对象距离信息
	void	UpdateAlignerTargetSizeInfo();		// 更新对位目标距离信息
	void  UpdateAlignerObjectSizeInfo();		// 更新对位对象距离信息
	afx_msg void OnBnClickedRadioGetAxisByComm();
	afx_msg void OnBnClickedRadioGetAxisByInput();

	void ExeSearch(int nPos); // 执行位置nPos的搜索
	AlignerAxisInfo	 m_AlignerAxisInfo;             //  轴信息
	void UpdateAxisInfo();
	vector<double> m_vImagePosAndPlatformPos;  // 图像坐标和平台坐标
	void UpdateImagePosAndPlatformPosInfo();
	afx_msg void OnBnClickedBtnObjectSearchPos3();
	afx_msg void OnBnClickedBtnObjectSearchPos4();
	afx_msg void OnBnClickedBtnObjectSizeCalc();
	// 扩展产品序号
	CComboBox m_CComboBox_EX_Product;
	int m_nExProductIndex;
	int m_nDistanceNums;		// 距离个数
	afx_msg void OnEnSetfocusEditValPosAxisX1();

	CString m_strInfoOld;
	void OnEnSetfocusEdit_Keyboard(int nID);
	BOOL m_bIsReadOnly;
	void OnEnKillfocusEdit(int nID);
	BOOL IsStringNumerical( LPCTSTR lpszVal );

	afx_msg void OnEnSetfocusEditValPosAxisY1();
	afx_msg void OnEnSetfocusEditValPosAxisD1();
	afx_msg void OnEnSetfocusEditValPosAxisX2();
	afx_msg void OnEnSetfocusEditValPosAxisY2();
	afx_msg void OnEnSetfocusEditValPosAxisD2();

	afx_msg void OnEnSetfocusEditValPosAxisX3();
	afx_msg void OnEnSetfocusEditValPosAxisY3();
	afx_msg void OnEnSetfocusEditValPosAxisD3();

	afx_msg void OnEnSetfocusEditValPosAxisX4();
	afx_msg void OnEnSetfocusEditValPosAxisY4();
	afx_msg void OnEnSetfocusEditValPosAxisD4();
	afx_msg void OnCbnSelchangeComboExproductCheckBySize();

	afx_msg void OnEnKillfocusEditValPosAxisX1();
	afx_msg void OnEnKillfocusEditValPosAxisY1();
	afx_msg void OnEnKillfocusEditValPosAxisD1();
	afx_msg void OnEnKillfocusEditValPosAxisX2();
	afx_msg void OnEnKillfocusEditValPosAxisY2();
	afx_msg void OnEnKillfocusEditValPosAxisD2();
	afx_msg void OnEnKillfocusEditValPosAxisX3();
	afx_msg void OnEnKillfocusEditValPosAxisY3();
	afx_msg void OnEnKillfocusEditValPosAxisD3();
	afx_msg void OnEnKillfocusEditValPosAxisX4();
	afx_msg void OnEnKillfocusEditValPosAxisY4();
	afx_msg void OnEnKillfocusEditValPosAxisD4();

};
