#pragma once
#include "afxwin.h"
#include "vsAlignToolParam.h"

// CDlgPhotoLightCtrlSet 对话框
class CSystemOptionConfig;
class CDlgPhotoLightCtrlSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPhotoLightCtrlSet)

public:
	CDlgPhotoLightCtrlSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPhotoLightCtrlSet();

	void UpdateDlgData(int CMDLightIndex,int nCMDLightCtrlIndex, BOOL bSaveAndValidate = TRUE );
	PhotoLightCtrlSet m_cPhotoLightCtrl;

// 对话框数据
	enum { IDD = IDD_DLG_PHOTO_LIGHT_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboCmdLightIndex();
	afx_msg void OnCbnSelchangeComboCmdLightCtrlIndex();
	int m_nCMDLightIndex;
	int m_nCMDLightCtrlIndex;
	virtual BOOL OnInitDialog();
	CComboBox m_CombCMDLightIndex;
	CComboBox m_CombCMDLightCtrlIndex;
	CButton m_BtnEnableCurLightCtrl;
	afx_msg void OnBnClickedOk();

	BOOL m_bEnableChannel1;
	BOOL m_bEnableChannel2;
	BOOL m_bEnableChannel3;
	BOOL m_bEnableChannel4;
	BOOL m_bEnableChannel5;
	BOOL m_bEnableChannel6;
	BOOL m_bEnableChannel7;
	BOOL m_bEnableChannel8;

	BOOL m_bChannelState1;
	BOOL m_bChannelState2;
	BOOL m_bChannelState3;
	BOOL m_bChannelState4;
	BOOL m_bChannelState5;
	BOOL m_bChannelState6;
	BOOL m_bChannelState7;
	BOOL m_bChannelState8;

	CSystemOptionConfig*	m_pSystempOptionConfig;						// 系统选项
	void SetSystemOptionConfig(CSystemOptionConfig* pSystemOptionConfig)
	{
		m_pSystempOptionConfig = pSystemOptionConfig;
	}
};
