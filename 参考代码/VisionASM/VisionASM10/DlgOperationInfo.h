#pragma once
#include "vsBaseVisionAlign.h"
#include "VisionASMDlg.h"
#include "DlgVisionRun.h"
#include "VisionASMDLG.h"


// CDlgOperationInfo 对话框

class CDlgOperationInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOperationInfo)

public:
	CDlgOperationInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOperationInfo();

// 对话框数据
	enum { IDD = IDD_DLG_OPERATION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CWnd*	SetParentWnd(CDlgVisionRun* pParent);							//设置父窗口
	CWnd*	SetParentWndPointer(CVisionASMDlg* pParent);					//设置父窗口指针
	void	SetVisionASMRef(std::vector<vcBaseVisionAlign*> vpVisionASM);	//获取视觉指针
	void	SetPlatformNum(int nPlatformNum);								//设置平台数量
	void	InitDlgItem();													//初始化窗口控件
	void	InitInfoList();													//初始化信息列表
	void	InitListType();													//初始化列表控件的样式
	void	InitListContent();												//初始化列表控件的内容
	void	UpdateDialogLanguage();											//根据语言选择更改界面信息
	void	UpdateDlgItem(CRect rcCommCommand, int nIntendX);				//刷新界面显示控件
	void	UpdateCurDlgLanguage();											//刷新当前界面文字 中/英
	void	UpdateCurShowInfo(deque<CString> saShowInfo);					//刷新信息（根据与上一次字符串的区别刷新）
	void	UpdateCurShowInfoAll(deque<CString> saShowInfo);				//刷新信息（根据全部的信息刷新,切换信息类型时刷新用这个）
	void	LoadWarningInfoFromFile();										//从文件加载报警信息
	void	SetWarningInfoFilePath(CString strFilePath);					//设置当前工作路径
	void	SetCurrentWorkPath(CString strFilePath);						//设置报警文件路径
	void	SetOperationLogFilePath(CString strFilePath);					//设置报警文件路径
	void	SetCurrentShowPlatform(int nCurrentShowPlatform);				//设置当前要显示的平台信息，在显示窗口上点击statubar的工位的信息会显示出来
	int		GetCurrentShowPlatform();										//获取当前正在显示的平台的序号
	int		GetCurrentShowInfoType();										//获取当前正在显示的信息类型，0-报警，1-操作
	void	ClearItemContent();												//清空m_editCurWarningInfo内容
	void	CheckChange(int& nPos);											//检查新旧内容区别，因为旧内容和新内容之前除前后端点位置，都是重合的，因此检查新内容第一条在旧内容的位置
	int		Split(CString strSource, const char split, CStringArray &strArrayRes);	//字符串分割，将报警信息分割为 时间+内容

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL DestroyWindow();

	afx_msg void OnBnClickedBtnClearInfo();					//清空当前信息，清空内容，清空字符串的值
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnCbnSelchangeComboPlatformIndex();		//平台切换，需更新窗口显示的内容
	afx_msg void OnBnClickedRadioShowInfoType();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CVisionASMDlg*				m_pParentDlg;				// 父窗口
	std::vector<vcBaseVisionAlign*> m_vpVisionASM;			// 视觉对位工具
	CStringArray*				m_psaSysInfoStrings;		// 语言信息
	int							m_nPlatformNum;				// 平台数量
	int							m_nCurrentPlatformIndex;	// 当前平台序号
	CString						m_strWarningInfo;
	CComboBox					m_comboPlatformSel;			// 当前平台的信息
	int							m_nInfoType;				// 当前显示的信息类型，0：报警信息，1：操作日志
	int							m_nInfoTypeOld;				// 上一次的信息类型
	CListCtrl					m_listctrlInfo;				// 信息列表，当前Radio为报警时，显示报警信息，为操作日志时，显示操作记录
	CString						m_strCurWorkDir;			// 当前执行路径
	CString						m_strWarningFilePath;		// 报警文件路径
	CString						m_strOperationLogFilePath;	// 操作日志文件路径
	std::vector<CString>		m_vstrWarningInfo;			// 报警信息
	BOOL						m_boIsCleaningInfo;			// 是否正在清空信息，如果正在清空信息，则界面不可刷新
	deque<CString>				m_saWarningInfo;			// 报警信息
	deque<CString>				m_saWarningInfoOld;			// 上一次的报警信息
	deque<CString>				m_saOperationLogInfo;		// 操作记录信息
	deque<CString>				m_saOperationLogInfoOld;	// 上一次获取的操作记录信息

};
