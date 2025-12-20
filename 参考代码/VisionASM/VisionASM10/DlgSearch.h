#if !defined(AFX_DLGSEARCH_H__1688B18B_E0E1_4CA3_9EC6_76E3414C0FDA__INCLUDED_)
#define AFX_DLGSEARCH_H__1688B18B_E0E1_4CA3_9EC6_76E3414C0FDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSearch.h : header file
//
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "svXMLConfigurator.h"
#include "VisionASM.h"
#include "vsBaseVisionAlign.h"

#include "BaseSearchTool.h"
#include "CircleSearchTool.h"
#include "CornerSearchTool.h"
#include "FixedPosSearchTool.h"
#include "LineSearchTool.h"
#include "PatternSearchTool.h"
#include "QuickSearchTool.h"
#include "CrossSearchTool.h"
#include "PatCornerSearchTool.h"
#include "PatternCornerSearchTool.h"
#include "CombinedQuickSearch.h"
#include "CombinedPatternSearchTool.h"
#include "BaseCornerSearchTool.h"
#include "PatternSearchCheckTool.h"
#include "QuickSearchCheckTool.h"
#include "QuickSearchObroundTool.h"
#include "PatternSearchObroundTool.h"
#include "MQuickSearchTool.h"
#include "MPatternSearchTool.h"
#include "MQuickCornerSearchTool.h"
#include "MQuickPickCornerSearchTool.h"
#include "QuickSearchGenRectTool.h"
#include "PatternSearchGenRectTool.h"
#include "PatternLineSearchTool.h"
#include "2PatternSearchTool.h"
#include "QuickLineCircleTool.h"
#include "PatternLineCircleTool.h"
#include "PatternCircleSearchTool.h"
#include "QuickSearchCheckLineTool.h"
#include "QuickSearchCheckLineTool.h"
#include "PatternBlobSearchTool.h"
#include "CornerSearchCheckTool.h"
#include "RectSearchTool.h"
#include "VirtualRectSearchTool.h"
#include "PatternRectSearchTool.h"
#include "PatternVirtualRectSearchTool.h"
#include "AreaInspectTool.h"
#include "AreaInspectToolEx.h"
#include "ChordSearchTool.h"
#include "TwoPatModelSearchTool.h"
#include "MPatternCornerSearchTool.h"
#include "MPatternPickCornerSearchTool.h"
#include "PatternCornerCircleSearchTool.h"
#include "QuickCornerCircleSearchTool.h"
#include "PatternPosLineDistanceTool.h"
#include "CornerSearchCheckInspectTool.h"
#include "PatternDoubleSearchTool.h"
#include "PatternChordSearchTool.h"
#include "QuickMulCircleSearchTool.h"
#include "DmCodeCalibrateTool.h"
#include "PatternCircle4LineSearchTool.h"
#include "QuickCircle4LineSearchTool.h"
#include "SpecialPickSearchTool.h"
#include "PatternCornerLineSearchTool.h"
#include "PatternMulCircleSearchTool.h"
#include "PatternCornerMulCircleSearchTool.h"
#include "PatternCornerMulObroundTool.h"
#include "QuickCornerMulObroundTool.h"
#include "CircleCornerSearchCheckInspectTool.h"
#include "ACFInspectTool.h"
#include "TwoQuickModelSearchTool.h"
#include "IdReaderBarCodeTool.h"
#include "DlgQuickSearchTool.h"
#include "DlgPatternSearchTool.h"
#include "DlgLineSearchTool.h"
#include "DlgCornerSearchTool.h"
#include "DlgCircleSearchTool.h"
#include "DlgFixedPosSearchTool.h"
#include "DlgCrossSearchTool.h"
#include "DlgPatCornerSearchTool.h"
#include "DlgProCircleSearchTool.h"
#include "DlgPatternCornerSearchTool.h"
#include "DlgCombinedQuickSearchTool.h"
#include "DlgCombinedPatternSearchTool.h"
#include "DlgBaseCornerSearchTool.h"
#include "DlgPatternSearchCheckTool.h"
#include "DlgQuickSearchCheckTool.h"
#include "DlgQuickSearchObroundTool.h"
#include "DlgPatternSearchObroundTool.h"
#include "DlgMPatternSearchTool.h"
#include "DlgMQuickSearchTool.h"
#include "DlgMQuickCornerSearchTool.h"
#include "DlgMQuickPickCornerSearchTool.h"
#include "DlgQuickSearchGenRectTool.h"
#include "DlgPatternSearchGenRectTool.h"
#include "DlgPatternLineSearchTool.h"
#include "Dlg2PatternSearchTool.h"
#include "DlgPatternCircleSearchTool.h"
#include "DlgQuickLineCircleSearchTool.h"
#include "DlgPatternLineCircleTool.h"
#include "DlgPatternCircleSearchTool.h"
#include "DlgQuickSearchCheckLineTool.h"
#include "DlgPatternSearchCheckLineTool.h"
#include "DlgPatternBlobSearchTool.h"
#include "DlgCornerSearchCheckTool.h"
#include "DlgRectSearchTool.h"
#include "DlgVirtualRectSearchTool.h"
#include "DlgPatternRectSearchTool.h"
#include "DlgPatternVirtualRectSearchTool.h"
#include "DlgAreaInspect.h"
#include "DlgAreaInspectEx.h"
#include "DlgChordSearchTool.h"
#include "DlgTwoPatModelSearchTool.h"
#include "DlgMPatternCornerSearchTool.h"
#include "DlgMPatternPickCornerSearchTool.h"
#include "DlgPatternCornerCircleSearchTool.h"
#include "DlgQuickCornerCircleSearchTool.h"
#include "DlgPatternPosLineDistanceTool.h"
#include "DlgCornerSearchCheckInspectTool.h"
#include "DlgPatternDoubleSearchTool.h"
#include "DlgPatternChordSearchTool.h"
#include "DlgQuickMulCircleSearchTool.h"
#include "DlgDmCodeCalibrateTool.h"
#include "DlgPatternCircle4LineSearchTool.h"
#include "DlgQuickCircle4LineSearchTool.h"
#include "DlgSpecialPickSearchTool.h"
#include "DlgPatternCornerLineSearchTool.h"
#include "DlgPatternMulCircleSearchTool.h"
#include "DlgPatternCornerMulCircleSearchTool.h"
#include "DlgPatternCornerMulObroundTool.h"
#include "DlgQuickCornerMulObroundTool.h"
#include "DlgCircleCornerSearchCheckInspectTool.h"
#include "DlgACFInspectTool.h"
#include "DlgTwoQuickModelSearchTool.h"
#include "DlgQuickPosLineDistanceTool.h"
#include "QuickPosLineDistanceTool.h"
#include "DlgCombinedCornerSearchTool.h"
#include "CombinationCornerSearchTool.h"

#include "MPatternPickSearchTool.h"
#include "DlgMPatternPickSearchTool.h"
#include "AreaInspectPickTool.h"
#include "DlgAreaInspectPickTool.h"

#include "DlgIdReaderDmCodeTool.h"
#include "DlgIdReaderBarCodeToolSetting.h"
#include "KeyBoard.h"

#include "LogFileSave.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSearch dialog

//#define IDS_CMB_CROSS_SEARCH_TOOL  _T("十字定位工具")
//#define IDS_CMB_PATCORNER_SEARCH_TOOL  _T("组合区域角定位工具")
//#define IDS_CMB_PROCIRCLE_SEARCH_TOOL  _T("专用圆定位工具")
//#define IDS_CMB_PATTERNCORNER_SEARCH_TOOL _T("区域角定位工具")

class CDlgSearch : public CDialog
{
// Construction
public:
	CDlgSearch(	CWnd* pParent = NULL,
				CBaseSearchTool* pDefSearchTool = NULL);
	virtual ~CDlgSearch();
// Dialog Data
	//{{AFX_DATA(CDlgSearch)
	enum { IDD = IDD_DLG_SEARCH };
	CXPStyleButtonST	m_btnOK;
	CXPStyleButtonST	m_btnCancel;
	CXPStyleButtonST	m_btnSearch;
	CXPStyleButtonST	m_btnLoadImage;
	CXPStyleButtonST	m_btnGrabImage;
	CComboBox	m_comboDisplayType;
	CComboBox	m_comboSearchToolType;
	CTabCtrl	m_ctrlTabControl;
	int		m_nDisplayType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbSearchToolType();
	afx_msg void OnSelchangeCmbDisplayType();
	afx_msg void OnBtnLoadImage();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnBtnSearchExe();
	afx_msg void OnBtnGrabImage();
	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	vector<CString> m_vectorTest1FileNames;
	int m_nTestImage1total ;
	int m_nTestImage1Index ;
	int m_nTestImageIndexStatus;// 0，未进行向前、回溯;1，向前；2，回溯

	CThemeHelperST		m_themeHelper;
	
	CWnd *m_pVAPrevMainWnd;
	scGuiDisplay *m_pPrevGuiDisplay;
	int m_nCurPositionIdx;
	CString m_strTitleName;
	scGuiDisplay m_guiDisplay;
	cpImage m_imageInput;
	cpImage m_imageTrain;
	cpImage m_imageResult;

	BOOL m_bTrainDataUpdated;
	BOOL m_bSearchDataUpdated;

	int	m_nCurSelSearchToolIdx;
	CStringArray*	m_psaSysInfoStrings;
	CString			m_strCurWorkDir;
	LANGID			m_lgidLanguage;
	BOOL			m_bSysKeyboardEnabled;
	CKeyBoard		m_ctrlKeyboard;

	vcBaseVisionAlign *m_pVisionASM;
	CameraParam m_CameraParam;

	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	CameraParamEnable   m_bEnableSearchShutter;			// 产品设置中曝光
	CameraParamSet m_dProductSearchShutter;			// 实时模板曝光
	CameraParamSet m_dEnvironmentShutter;			// 环境设置曝光

	// 定位工具
	CBaseSearchTool 		*m_pSearchTool;
	CBaseSearchTool			*m_pDefSearchTool;
	CBaseTrainDataParam		*m_pTrainDataParam;
	CBaseTrainGuiParam		*m_pTrainGuiParam;
	CBaseSearchDataParam	*m_pSearchDataParam;
	CBaseSearchGuiParam		*m_pSearchGuiParam;

	// 定位工具界面
	CDlgQuickSearchTool						*m_pDlgQuickSearchTool;
	CDlgPatternSearchTool					*m_pDlgPatternSearchTool;
	CDlgLineSearchTool						*m_pDlgLineSearchTool;
	CDlgCornerSearchTool					*m_pDlgCornerSearchTool;
	CDlgCircleSearchTool					*m_pDlgCircleSearchTool;
	CDlgCrossSearchTool						*m_pDlgCrossSearchTool;
	CDlgPatCornerSearchTool					*m_pDlgPatCornerSearchTool;
	CDlgProCircleSearchTool					*m_pDlgProCircleSearchTool;
	CDlgFixedPosSearchTool					*m_pDlgFixedPosSearchTool;
	CDlgPatternCornerSearchTool				*m_pDlgPatternCornerSearchTool;
	CDlgCombinedQuickSearchTool				*m_pDlgCombinedQuickSearchTool;
	CDlgCombinedPatternSearchTool			*m_pDlgCombinedPatternSearchTool;
	CDlgBaseCornerSearchTool				*m_pDlgBaseCornerSearchTool;
	CDlgPatternSearchCheckTool				*m_pDlgPatternSearchCheckTool;
	CDlgQuickSearchCheckTool				*m_pDlgQuickSearchCheckTool;
	CDlgQuickSearchObroundTool				*m_pDlgQuickSearchObroundTool;
	CDlgPatternSearchObroundTool			*m_pDlgPatternSearchObroundTool;
	CDlgMPatternSearchTool					*m_pDlgMPatternSearchTool;
	CDlgMQuickSearchTool					*m_pDlgMQuickSearchTool;
	CDlgMQuickCornerSearchTool				*m_pDlgMQuickCornerSearchTool;
	CDlgMQuickPickCornerSearchTool			*m_pDlgMQuickPickCornerSearchTool;
	CDlgQuickSearchGenRectTool				*m_pDlgQuickSearchGenRectTool;
	CDlgPatternSearchGenRectTool			*m_pDlgPatternSearchGenRectTool;
	CDlgPatternLineSearchTool				*m_pDlgPatternLineSearchTool;
	CDlg2PatternSearchTool					*m_pDlg2PatternSearchTool;
	CDlgQuickLineCircleTool					*m_pDlgQuickLineCircleSearchTool;
	CDlgPatternLineCircleTool				*m_pDlgPatternLineCircleSearchTool;
	CDlgPatternCircleSearchTool				*m_pDlgPatternCircleSearchTool;
	CDlgQuickSearchCheckLineTool			*m_pDlgQuickSearchCheckLineTool;
	CDlgPatternSearchCheckLineTool			*m_pDlgPatternSearchCheckLineTool;
	CDlgPatternBlobSearchTool				*m_pDlgPatternBlobSearchTool;
	CDlgCornerSearchCheckTool				*m_pDlgCornerSearchCheckTool;
	CDlgRectSearchTool						*m_pDlgRectSearchTool;
	CDlgPatternRectSearchTool				*m_pDlgPatternRectSearchTool;
	CDlgAreaInspect							*m_pDlgAreaInspectTool;
	CDlgAreaInspectEx						*m_pDlgAreaInspectExTool;
	CDlgChordSearchTool						*m_pDlgChordSearchTool;
	CDlgTwoPatModelSearchTool				*m_pDlgTwoPatModelSearchTool;
	CDlgMPatternCornerSearchTool			*m_pDlgMPatternCornerSearchTool;
	CDlgMPatternPickCornerSearchTool			*m_pDlgMPatternPickCornerSearchTool;
	CDlgPatternCornerCircleSearchTool		*m_pDlgPatternCornerCircleSearchTool;
	CDlgQuickCornerCircleSearchTool			*m_pDlgQuickCornerCircleSearchTool;
	CDlgPatternPosLineDistanceTool			*m_pDlgPatternPosLineDistanceTool;
    CDlgCornerSearchCheckInspectTool		*m_pDlgCornerSearchCheckInspectTool;
    CDlgPatternDouleSearchTool				*m_pDlgPatternDoubleSearchTool;
	CDlgPatternChordSearchTool				*m_pDlgPatternChordSearchTool;
    CDlgQuickMulCircleSearchTool			*m_pDlgQuickMulCircleSearchTool;
	CDlgDmCodeCalibrateTool					*m_pDlgDmCodeCalibrateTool;
	CDlgPatternCircle4LineSearchTool		*m_pDlgPatternCircle4LineSearchTool;//区域四弦定位工具
	CDlgQuickCircle4LineSearchTool			*m_pDlgQuickCircle4LineSearchTool;//几何四弦定位工具
	CDlgSpecialPickSearchTool				*m_pDlgSpecialPickSearchTool;
	CDlgPatternCornerLineSearchTool			*m_pDlgPatternCornerLineSearchTool;
	CDlgPatternMulCircleSearchTool			*m_pDlgPatternMulCircleSearchTool;
	CDlgPatternCornerMulCircleSearchTool	*m_pDlgPatternCornerMulCircleSearchTool;
	CDlgPatternCornerMulObroundTool			*m_pDlgPatternCornerMulObroundTool;
	CDlgQuickCornerMulObroundTool			*m_pDlgQuickCornerMulObroundTool;
	CDlgQuickPosLineDistanceTool			*m_pDlgQuickPosLineDistanceTool;
	CDlgCombinedCornerSearchTool			*m_pDlgCombinedCornerSearchTool;
	CDlgMQuickPickSearchTool			    *m_pDlgMQuickPickSearchTool;
	CDlgMPatternPickSearchTool				*m_pDlgMPatternPickSearchTool;
	CDlgAreaInspectPickTool					*m_pDlgAreaInspectPickTool;



	
	CDlgCircleCornerSearchCheckInspectTool  *m_pDlgCircleCornerSearchCheckInspectTool;
	CDlgACFInspectTool									  *m_pDlgACFInspectTool;
	
	CDlgVirtualRectSearchTool    *m_pDlgVirtualRectSearchTool;
	CDlgPatternVirtualRectSearchTool    *m_pDlgPatternVirtualRectSearchTool;
	
	CDlgTwoQuickModelSearchTool				*m_pDlgTwoQuickModelSearchTool;
	CDlgIdReaderBarCodeToolSetting *m_pDlgIdReaderBarCodeTool;
	CDlgIdReaderDmCodeTool					*m_pDlgIdReaderDmCodeTool;
	

	// 判定当前界面是否是预览界面
	// false为模板制作界面
	// true为模板预览界面，仅提供预览，不支持保存
	bool m_bIsPreviewTool;
	void SetIsPreviewTool(bool bIsPreviewTool){m_bIsPreviewTool = bIsPreviewTool;};//设置当前界面是否仅提供预览
public:
	void UpdateStatusBar();
	void UpdateStatusBar(bool bSucceed, LPCTSTR strInfo = NULL, LPCTSTR strInfoEx = NULL);
	void UpdateGuiDisplay();
	void UpdateSearchToolDisplay();	
	CBaseSearchTool* GetSearchTool();
	void DestoryItems();
	void UpdateDialogLanguage();
	
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings);
	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM, int nPositionIdx);

protected:
	void InitDlgItem();
public:
	afx_msg void OnBnClickedBtnLoadImage2();
	afx_msg void OnBnClickedBtnSearchExe2();
//	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSearchExe3();
	afx_msg void OnBnClickedBtnGrabSearchImage();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnProductShutterSetting();

	virtual BOOL DestroyWindow();

// 	double GetProductSearchShutter();
// 	void   SetProductSearchShutter(double dShutter);

// 	void SetEnableSearchShutter(BOOL bEnableSearchShutter);
// 	BOOL GetEnableSearchShutter();

	CameraParamSet GetProductSearchShutterInfoShutter();
	CameraParamEnable   GetProductSearchShutterInfoEnable();
	void   SetProductSearchShutterInfo(CameraParamEnable bEnable, CameraParamSet dShutter);



	void DestroyAllDlg();//摧毁所有对话框
	void UpdateSearchToolParam(CString strSelText);//更新搜索工具参数
	void InitSearchTooParam(CString strSelText);//创建搜索工具
	void CreateToolDlg(CString strSelText);//创建所需对话框
	void UpdateToolDlgControlDisplay(CString strSelText);//更新TOOL对话框控件显示
	void UpdateDlgSearchToolData(CString strSelText);//更新TOOL对话框数据

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEARCH_H__1688B18B_E0E1_4CA3_9EC6_76E3414C0FDA__INCLUDED_)
