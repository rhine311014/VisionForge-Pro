
#pragma once

#ifdef VS_KEYBOARD_EXPORTS
#define VSKEYBOARD_API __declspec(dllexport)
#else
#define VSKEYBOARD_API __declspec(dllimport)
#endif

class VSKEYBOARD_API vcKeyboard
{
public:
	vcKeyboard();
	virtual ~vcKeyboard();

	BOOL ShowNormal(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowNum(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowDotNum(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowSignDotNum(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowPasswordNormal(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowStringNormal(CRect& rcItem, CString& strIn, CString& strOut);
	BOOL ShowAllNormal(CRect& rcItem, CString& strIn, CString& strOut);

	void SetShowTitle(BOOL bShow = true);
	BOOL GetShowTitle();

private:
	BOOL m_bShowTitle;
};


