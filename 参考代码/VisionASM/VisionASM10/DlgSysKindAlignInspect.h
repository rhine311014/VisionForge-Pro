#pragma once

#include "svGuiDisplay.h"
#include "KeyBoard.h"
#include "VisionASMDlg.h"
#include "vsVisionAlignDef.h"
#include "Label.h"

// CDlgSysKindAlignInspect 对话框

class CDlgSysKind;
class CDlgSysKindAlignInspect : public CDialog
{
	DECLARE_DYNAMIC(CDlgSysKindAlignInspect)

public:
	CDlgSysKindAlignInspect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysKindAlignInspect();

// 对话框数据
	enum { IDD = IDD_SYS_KIND_ALIGN_INSPECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGuiDisplayDblClk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGuiDisplayMenuCustom3(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	

public:
	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	void InitDlgItem();
	void MoveDlgItem();
	void UpdateDlgData(BOOL bSaveAndValidate = TRUE);
	void OnPlatformChange();
	BOOL SavePrevProduct();
	void OnButtonNewProduct();
	BOOL IsProductChanged();
	BOOL IsSearchToolSelected(CBaseSearchTool* pSearchTool);  //判断定位工具是否选中，若否，则不显示使用该定位工具制作模板时所保存的图片
	void GetDlgTempData(CProductData& product);

public:
	CStringArray	*m_psaSysInfoStrings;
	BOOL			m_bSysKeyboardEnabled;

	CVisionASMDlg *m_pParent;
	CDlgSysKind	*m_pParentKind;

	int		m_nPlatformIndex;
	int		m_nPrevPlatformIdx;

	int		m_nPrevProductIdx;
	int		m_nCurProductIdx;

	CString m_strModuleDir;
	CString m_strProductNameOld;

public:
	std::vector<CLabel*>		m_vpLabelPos;
	std::vector<VpGuiDisplay>	m_vvpInspectGuiDisplay;

//	int m_nInspectMarkEnable;			//是否启用检测搜索
	int m_nInspectCandidateEnable;		//是否启用候补搜索
	int m_nInspectCandidateSync;		//是否启用成对搜索
	BOOL m_bProdPatternChanged;

	CLabel	m_labelPos0;
	CLabel	m_labelPos1;
	CLabel	m_labelPos2;
	CLabel	m_labelPos3;
	CLabel	m_labelPos4;
	CLabel	m_labelPos5;
	CLabel	m_labelPos6;
	CLabel	m_labelPos7;
	
	BOOL m_bInspectCandidateOffsetEnable1;
	BOOL m_bInspectCandidateOffsetEnable2;
	BOOL m_bInspectCandidateOffsetEnable3;
	BOOL m_bInspectCandidateOffsetEnable4;

	BOOL m_bObjectShutterEnable0;
	BOOL m_bObjectShutterEnable1;
	BOOL m_bObjectShutterEnable2;
	BOOL m_bObjectShutterEnable3;
	BOOL m_bObjectShutterEnable4;

	BOOL m_bObjectGainEnable0;
	BOOL m_bObjectGainEnable1;
	BOOL m_bObjectGainEnable2;
	BOOL m_bObjectGainEnable3;
	BOOL m_bObjectGainEnable4;

	BOOL m_bObjectGammaEnable0;
	BOOL m_bObjectGammaEnable1;
	BOOL m_bObjectGammaEnable2;
	BOOL m_bObjectGammaEnable3;
	BOOL m_bObjectGammaEnable4;

	afx_msg void OnBnClickedCheckAlignerObjectcShutterEnable();
	afx_msg void OnBnClickedCheckAlignerObjectcGainEnable();
	afx_msg void OnBnClickedCheckAlignerObjectcGammaEnable();
	void EnableAndShowControl(bool bEnable,bool bShow);

	afx_msg void OnBnClickedRadioInspectSearchMode0();
	afx_msg void OnBnClickedRadioInspectcSearchMode1();
	afx_msg void OnBnClickedRadioInspectcSearchSync0();

	void EnableWindowByAuthorizeset(bool bEnable);

private:
	void ShowAndEnableControl(BOOL bShow);
public:
	afx_msg void OnBnClickedCheckInspectSinglesearch();
	int m_nInspectSingleSearch;
};
