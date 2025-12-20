#pragma once
#include "vsVisionAlignDef.h"
#include "svDIB.h"
#include "vsBaseVisionAlign.h"
#include "DlgCalibrateEvaluate.h"
#include "afxwin.h"
// CDlgCalibResult 对话框

class CDlgCalibResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibResult)

private:
	vcBaseVisionAlign * m_pVisionASM;
	SysPlatformInfo m_sysPlatformInfo;
	int m_nPosIndex;
	int m_nExIndex;
	CString m_strOut;//标定信息
	CString m_strCaption;//标题
	CCalibratedInfo* m_pCalibratedInfo;
	CBaseSearchTool* m_pCalibSearchTool;
	scGuiDisplay m_GuiDisplay;//九宫格显示
	scDIB m_DIB;
	cpImage m_CalibImage;
	scGuiLineSeg m_GuiLineSeg[12];
	scGuiCoordCross m_GuiCross[12];
	scGuiCoordCross m_GuiCrossCenter;
	scGuiLineSeg m_GuiLineSegRaduis[3];
	scGuiText m_GuiText[12];
	scGuiText m_GuiPosText;
public:
	CDlgCalibResult(CWnd* pParent = NULL);   // 标准构造函数
	CDlgCalibResult(vcBaseVisionAlign *pVisionASM,int nPosIndex,CWnd* pParent);
	virtual ~CDlgCalibResult();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool CalibResultDiagnose(CString &strOut,int &nKeyProblem);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_ComboCalibExtentIndex;
	afx_msg void OnCbnSelchangeComboCalibResultExtent();
	afx_msg void OnBnClickedBtnCalibResultDetail();
	afx_msg void OnBnClickedBtnCalibResultDiagnose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void ShowCalibResult();
	CComboBox m_ComboCalibPosIndex;
	afx_msg void OnCbnSelchangeComboCalibResultPos();
};
