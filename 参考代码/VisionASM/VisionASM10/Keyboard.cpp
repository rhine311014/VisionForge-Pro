
#include "stdafx.h"
#include "Keyboard.h"
#include "vsKeyboard.h"

CKeyBoard::CKeyBoard()
{

}

CKeyBoard::~CKeyBoard()
{

}

BOOL CKeyBoard::ShowNumeralKeyBoard( CRect& rcAppend, CString& strInput, CString& strOutput )
{
	vcKeyboard keyboard;
	return keyboard.ShowNum(rcAppend, strInput, strOutput);
}

BOOL CKeyBoard::ShowStandardKeyBoard( CRect& rcAppend, CString& strInput, CString& strOutput, BOOL bSingalBtnActive/*=TRUE*/ )
{
	vcKeyboard keyboard;
	return keyboard.ShowNormal(rcAppend, strInput, strOutput);
}

BOOL CKeyBoard::ShowPasswordStandardKeyBoard( CRect& rcAppend, CString& strInput, CString& strOutput)
{
	vcKeyboard keyboard;
	return keyboard.ShowPasswordNormal(rcAppend, strInput, strOutput);
}

BOOL CKeyBoard::ShowNumeralDotKeyBoard( CRect& rcAppend, CString& strInput, CString& strOutput )
{
	vcKeyboard keyboard;
	return keyboard.ShowDotNum(rcAppend, strInput, strOutput);	
}

BOOL CKeyBoard::ShowNumeralDotSymbolKeyBoard( CRect& rcAppend, CString& strInput, CString& strOutput )
{
	vcKeyboard keyboard;
	return keyboard.ShowSignDotNum(rcAppend, strInput, strOutput);	
}