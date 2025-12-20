#pragma once

#include "svGuiDisplay.h"
#include "vsBaseVisionAlign.h"
#include "vsXYDVisionAlign.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"
#include "Label.h"

#include "svDef.h"
#include "svDIB.h"
#include "svPolyline.h"
#include "svGuiCross.h"
#include "svGuiPolyline.h"
#include "svGuiDisplay.h"
#include "svCalibrate.h"
#include "svImageWarp.h"
#include "VisionASMDlg.h"
// CDlgDistorCorrImage 对话框

class CDlgDistorCorrImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgDistorCorrImage)

public:
	CDlgDistorCorrImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDistorCorrImage();

// 对话框数据
	enum { IDD = IDD_DLG_DISTORCORR_IMAGE };

protected:
	// 状态栏相关
	CStatusBarCtrl *m_pStatusBar;
	HICON m_hIconSucceed;
	HICON m_hIconFailed;
	BOOL m_bStatusBarFlag;
	CString m_strStatusBarInfo1;
	CString m_strStatusBarInfo2;

	//// 状态栏显示
	//HICON			m_hIcon;
	//HICON			m_hIcon1;
	//HICON			m_hIcon2;
	//CStatusBarCtrl*	m_StatusBar;
	
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL	OnInitDialog();
	
	DECLARE_MESSAGE_MAP()

	void			UpdateDisplay(void);
	void			UpdateResultDate(void);
	afx_msg LRESULT OnGuiDisplayMouseMove(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnBnClickedBtnLoadImage();
	afx_msg void	OnBnClickedBtnCalibrate();
	afx_msg void	OnBnClickedButtonImageWarp();
	afx_msg void	OnBnClickedCheckFiducial();
	afx_msg void	OnBnClickedBtnGrabImage();
	afx_msg void	OnEnSetfocusEditTileX();
	afx_msg void	OnEnSetfocusEditTileY();
	afx_msg void	OnEnSetfocusEditThre();
	afx_msg void	OnEnKillfocusEditTileX();
	afx_msg void	OnEnKillfocusEditTileY();
	afx_msg void	OnEnKillfocusEditThre();
	afx_msg void	OnCbnSelchangeComboImageList();
	afx_msg void	OnBnClickedOk();
	afx_msg void	OnBnClickedCancel();
	BOOL			IsStringNumerical( LPCTSTR lpszVal );
	BOOL			IsStringPositiveNumerical( LPCTSTR lpszVal );

public:
	cpImage				m_ImageInPut;
	cpImage				m_ImagePoint;
	cpImage				m_ImageResult;
protected:
	CComboBox				m_ComboImageList;
	int						m_nFindCornersMode;
	int						m_nCurIndex;
	scGuiDisplay			m_GuiDisplay;
	scGuiCross*				m_pGuiCrossArray;
	svStd vector<sc2Vector> m_imgcorners;
	svStd vector<sc2Vector> m_objcorners;
	scGuiInteractiveContainer*	m_pInteractiveContainer;
	scGuiStaticContainer*		m_pStaticContainer;
	scImageWarp*			m_pImageWarpTool;
	CWnd*m_pVAPrevMainWnd;		
	scGuiDisplay*			m_pPrevGuiDisplay;
	CStringArray*			m_psaSysInfoStrings;
	LANGID					m_lgidLanguage;

public:	
	int						m_nThre;
	double					m_dTileX;
	double					m_dTileY;
	BOOL					m_bOutputOutpoints;
	BOOL					m_bSysKeyboardEnabled;
	BOOL					m_bKillFocusEnabled;
	CComboBox				m_ComboModeList;
	CKeyBoard				m_ctrlKeyboard;
	scCalibrateResult		m_result;
	vcBaseVisionAlign *		m_pVisionASM;

	void EnableSysKeyboard(BOOL bEnable) { m_bSysKeyboardEnabled = bEnable; }
	void SetVisionASMRef(vcBaseVisionAlign *pVisionASM, int nPositionIdx);
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	void InitDlgItem();
	void UpdateStatusBar();

	// 当前下拉框选中的扩展序号
	int m_nCurExProductIndex;
	CComboBox m_comboExProductIndex;

public:
	int m_nPosNum;
	bool m_bTargetObjectCamSeparate;
	bool m_bCopyResult;
	std::vector<BOOL>	m_vbCopyPos;
public:
	std::vector<CButton*>		m_vpCheckPos;
	CButton m_checkPos1;
	CButton m_checkPos2;
	CButton m_checkPos3;
	CButton m_checkPos4;
	CButton m_checkPos5;
	CButton m_checkPos6;
	CButton m_checkPos7;
	CButton m_checkPos8;
};
