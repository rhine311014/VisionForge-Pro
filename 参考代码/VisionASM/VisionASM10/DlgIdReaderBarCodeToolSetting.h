#pragma once
#include "svGuiDisplay.h"
#include "svImage.h"
#include "BtnST.h"
#include "afxwin.h"

// CDlgIdReaderBarCodeToolSetting 对话框
class CDlgSearch;
class CDlgIdReaderBarCodeToolSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgIdReaderBarCodeToolSetting)

public:
	CDlgIdReaderBarCodeToolSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgIdReaderBarCodeToolSetting();

	CDlgSearch	*m_pDlgParent;
	CStringArray*	m_psaSysInfoStrings;

// 对话框数据
	enum { IDD = IDD_TOOL_ID_READER_BAR_CODE_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnBtnGetTrainImage();
	afx_msg void OnBtnTrain();
	afx_msg void OnBtnFullImageSearch();
	DECLARE_MESSAGE_MAP()

	CThemeHelperST		m_themeHelper;
	void UpdateDialogLanguage();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// 更新控件显示
	void UpdateControlDisplay();			
	// 定位工具训练数据与界面之间的数据获取，
	// bValidate:true,定位工具从界面获取参数;false,界面从定位工具获取训练参数值
	void UpdateTrainData(bool bValidate);	
	// 定位工具搜索数据与界面之间的数据获取，
	// bValidate:true,定位工具从界面获取参数;false,界面从定位工具获取搜索参数值
	void UpdateSearchData(bool bValidate);
	// 检查对话框上的参数设置的有效性，并将无效参数设置成默认值
	void CheckDlgDataValidation();

	int m_nDecodeMethod;
	int m_nAppMode;
	BOOL m_bCodaBarFlag;
	BOOL m_bCode128Flag;
	BOOL m_bCode39Flag;
	BOOL m_bCode93Flag;
	BOOL m_bCodeEANFlag;
	BOOL m_bCodeITF25Flag;
	int m_nMinSizeWidth;
	int m_nMaxSizeWidth;
	int m_nRegionNum;
	int m_nMinSizeHeight;
	int m_nSampleLevel;
	int m_nMaxSizeHeight;
	int m_nStaticAreaWidth;
	int m_nWindowSize;
	int m_nElemMinWidth;
	int m_nElemMaxWidth;
	int m_nWaitingTime;
	int m_nBarcodeBalanceLevel;
	int m_bErrorDetectFlag;
	int m_bFrameCheckOut;
	int m_nImageMorph;
	int m_nNumScanlines;
	int m_nPromodeDistortion;
	int m_nPromodeSpot;
	int m_nPromodeWhiteGap;
	int m_nSavePicLevel;
	int m_nSegmentationMethod;
	int m_nBoxDisplayMode;

	int m_nImageChannel;
	afx_msg void OnBnClickedRadio();

	afx_msg void OnBnClickedRadioVsb();
	afx_msg void OnBnClickedRadioAppModeNormal();
	afx_msg void OnBnClickedCheckCode128();
	afx_msg void OnBnClickedCheckCode39();
	afx_msg void OnBnClickedCheckCode93();
	afx_msg void OnBnClickedCheckCodeBar();
	afx_msg void OnBnClickedCheckCodeEan();
	afx_msg void OnBnClickedCheckCodeItf25();
	afx_msg void OnEnSetfocusEditSizeminwidth();
	afx_msg void OnEnKillfocusEditSizeminwidth();
	afx_msg void OnEnSetfocusEditSizemaxwidth();
	afx_msg void OnEnKillfocusEditSizemaxwidth();
	afx_msg void OnEnSetfocusEditMaxcodenum();
	afx_msg void OnEnKillfocusEditMaxcodenum();
	afx_msg void OnEnSetfocusEditNumscanlines();
	afx_msg void OnEnKillfocusEditNumscanlines();
	afx_msg void OnEnSetfocusEditSamplelevel();
	afx_msg void OnEnKillfocusEditSamplelevel();
	afx_msg void OnEnSetfocusEditSizeminheight();
	afx_msg void OnEnKillfocusEditSizeminheight();
	afx_msg void OnEnSetfocusEditSizemaxheight();
	afx_msg void OnEnKillfocusEditSizemaxheight();
	afx_msg void OnEnSetfocusEditStaticareaWidth();
	afx_msg void OnEnKillfocusEditStaticareaWidth();
	afx_msg void OnEnSetfocusEditWindowsize();
	afx_msg void OnEnKillfocusEditWindowsize();
	afx_msg void OnEnSetfocusEditWaittime();
	afx_msg void OnEnKillfocusEditWaittime();
	afx_msg void OnEnSetfocusEditElemminwidth();
	afx_msg void OnEnKillfocusEditElemminwidth();
	afx_msg void OnEnSetfocusEditElemmaxwidth();
	afx_msg void OnEnKillfocusEditElemmaxwidth();
	afx_msg void OnEnSetfocusEditErrordetectflag();
	afx_msg void OnEnKillfocusEditErrordetectflag();
	afx_msg void OnEnSetfocusEditBarcodebalancelevel();
	afx_msg void OnEnKillfocusEditBarcodebalancelevel();
	afx_msg void OnEnSetfocusEditImagemorph();
	afx_msg void OnEnKillfocusEditImagemorph();
	afx_msg void OnEnSetfocusEditFramecheckout();
	afx_msg void OnEnKillfocusEditFramecheckout();
	afx_msg void OnEnSetfocusEditPromodedistortion();
	afx_msg void OnEnKillfocusEditPromodedistortion();
	afx_msg void OnEnSetfocusEditPromodespot();
	afx_msg void OnEnKillfocusEditPromodespot();
	afx_msg void OnEnSetfocusEditPromodewhitegap();
	afx_msg void OnEnKillfocusEditPromodewhitegap();
	afx_msg void OnEnSetfocusEditSavepiclevel();
	afx_msg void OnEnKillfocusEditSavepiclevel();
	afx_msg void OnBnClickedButtonFullImage();
	afx_msg void OnBnClickedCheckReferPositionTool();
	afx_msg void OnCbnSelchangeComboPositionTool();
	afx_msg void OnBnClickedRadioSegmentationmethodLocal();
	afx_msg void OnBnClickedRadioRaw();

	void ControEnaleWindow();
	CString m_strInput;
	CString m_strOutput;
};
