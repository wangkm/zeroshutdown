#include "afxcmn.h"
#if !defined(AFX_MYCONFIRMDLG_H__56C78B30_68D7_452F_899C_09D8F8B2D553__INCLUDED_)
#define AFX_MYCONFIRMDLG_H__56C78B30_68D7_452F_899C_09D8F8B2D553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MyConfirmDlg dialog

class MyConfirmDlg : public CDialog
{
// Construction
public:
	void SetCfmMsg(CString theCfgMsg);
	MyConfirmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MyConfirmDlg)
	enum { IDD = IDD_confirm };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyConfirmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MyConfirmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT myTimer;
	int timeCounter;
	CString cfmMsg;
	CString timeMsg;
public:
	CProgressCtrl m_pg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCONFIRMDLG_H__56C78B30_68D7_452F_899C_09D8F8B2D553__INCLUDED_)
