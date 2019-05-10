// MyShutDown.h : main header file for the MYSHUTDOWN application
//

#if !defined(AFX_MYSHUTDOWN_H__B7B4007E_FE8E_41E6_823A_7B2657A6A4EA__INCLUDED_)
#define AFX_MYSHUTDOWN_H__B7B4007E_FE8E_41E6_823A_7B2657A6A4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyShutDownApp:
// See MyShutDown.cpp for the implementation of this class
//

class CMyShutDownApp : public CWinApp
{
public:
	CMyShutDownApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyShutDownApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyShutDownApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSHUTDOWN_H__B7B4007E_FE8E_41E6_823A_7B2657A6A4EA__INCLUDED_)
