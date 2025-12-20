#pragma once


// DlgCalibAxisDirection 对话框

class CDlgCalibAxisDirection : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibAxisDirection)

public:
	CDlgCalibAxisDirection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibAxisDirection();

// 对话框数据
	enum { IDD = IDD_DLG_CALIB_AXIS_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	BOOL m_bReverseX;
	BOOL m_bReverseY;
	BOOL m_bReverseD;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
