// VisionASM.h : main header file for the VisionASM application
//

#if !defined(AFX_VisionASM_H__677B4C0B_5C4E_4A57_B34A_1AF90F0D02AE__INCLUDED_)
#define AFX_VisionASM_H__677B4C0B_5C4E_4A57_B34A_1AF90F0D02AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVisionASMApp:
// See VisionASM.cpp for the implementation of this class
//
typedef int ( * lpFunc)(void);
#define vas1000 1001
#define vas2000 2002
class CVisionASMApp : public CWinApp
{
public:
	CVisionASMApp();
	~CVisionASMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionASMApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVisionASMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HANDLE m_hMutexAppRunning;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VisionASM_H__677B4C0B_5C4E_4A57_B34A_1AF90F0D02AE__INCLUDED_)
