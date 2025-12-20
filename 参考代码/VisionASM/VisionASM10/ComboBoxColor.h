#pragma once
//#include <vector>
#include <map>

// CComboBoxColor

class CComboBoxColor : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxColor)

public:
	CComboBoxColor();
	virtual ~CComboBoxColor();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	int AddString(LPCTSTR lpszString,COLORREF nColor);
	int InsertString(int nIndex, LPCTSTR lpszString,COLORREF nColor);
	void ResetContent();
private:
	//std::vector<COLORREF> colorItem;
	std::map<CString,COLORREF> colorItem;
public:
	afx_msg void OnDestroy();
	virtual void PreSubclassWindow();
};


