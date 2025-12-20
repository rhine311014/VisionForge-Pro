
#pragma once

class CKeyBoard
{
public:
	CKeyBoard();
	virtual ~CKeyBoard();

	BOOL ShowNumeralKeyBoard(CRect& rcAppend, CString& strInput, CString& strOutput);
	BOOL ShowStandardKeyBoard(CRect& rcAppend, CString& strInput, CString& strOutput, BOOL bSingalBtnActive=TRUE);
	BOOL ShowPasswordStandardKeyBoard(CRect& rcAppend, CString& strInput, CString& strOutput);
	BOOL ShowNumeralDotKeyBoard(CRect& rcAppend, CString& strInput, CString& strOutput);
	BOOL ShowNumeralDotSymbolKeyBoard(CRect& rcAppend, CString& strInput, CString& strOutput);
};


