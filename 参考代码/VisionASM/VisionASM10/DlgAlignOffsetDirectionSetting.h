#pragma once


// CDlgAlignOffsetDirectionSetting 对话框

class CDlgAlignOffsetDirectionSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlignOffsetDirectionSetting)

public:
	CDlgAlignOffsetDirectionSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlignOffsetDirectionSetting();

// 对话框数据
	enum { IDD = IDD_DLG_ALIGN_OFFSET_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStringArray	*m_psaSysInfoStrings;
	void SetSysInfoStringsRef(CStringArray *psaSysInfoStrings) { m_psaSysInfoStrings = psaSysInfoStrings; }
	void UpdateDialogLanguage();
	BOOL m_bReverseX;
	BOOL m_bReverseY;
	BOOL m_bReverseD;
	virtual BOOL OnInitDialog();
};
