// MyShutDownDlg.h : header file
//

#if !defined(AFX_MYSHUTDOWNDLG_H__D0D2E086_AA67_4646_92B0_9185BA7D016B__INCLUDED_)
#define AFX_MYSHUTDOWNDLG_H__D0D2E086_AA67_4646_92B0_9185BA7D016B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MYWM_NOTIFYICON  (WM_USER + 1)

/////////////////////////////////////////////////////////////////////////////
// CMyShutDownDlg dialog
#include "AlarmMsgSet.h"
#include "DlgAlarm.h"
#include "MyConfirmDlg.h"
#include "afxwin.h"
#include "winnetwk.h"
#include "MyDataDefine.h"


class CMyShutDownDlg : public CDialog
{
// Construction
public:
	CTime timeNow;			//设定时刻
	CTime timeToShutDown;	//需要关机的时刻
	CTime timeTmp;			//即使得到当前时刻的暂时变量
	CMyShutDownDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyShutDownDlg)
	enum { IDD = IDD_MYSHUTDOWN_DIALOG };
	int		m_TimeType;
	CTime	m_Date;
	CTime	m_Time;
	int		m_Hour;
	int		m_Minute;
	int		m_Second;
	BOOL	m_isForce;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyShutDownDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyShutDownDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAtTime();
	afx_msg void OnAfterTime();
	virtual void OnOK();
	afx_msg void OnHaltNow();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnEnd();
	afx_msg void OnCsl();
	virtual void OnCancel();
	afx_msg void OnAppAbout();
	afx_msg void OnHide();
	afx_msg void OnAdvset();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void 			ToShutDown(int ShutDownType, bool target_type);	//实际的关机函数
	bool 			hasShowCfmDlg;		//标识是否已经显示了确认对话框
	bool 			isDecided;			//标识是否已经确认了设定
	void 			TypeSet();
	void 			Reset();
	CString 		strTitleTmp;	//标题栏
	CString 		strIniFileName;	//ini file
	CString 		strWavFileName;	//wave file for alarm
	NOTIFYICONDATA	mynid;	
	DWORD			dwVersion;
	CString 		strTmp;
	CString 		strExecType;
	int				shutDownType;
	UINT			shutDownSign;
	DlgAlarm		dlgAlarm;		//定时提示窗口
	AlarmMsgSet		dlgAlarmMsgSet;	//定时待办事宜

public:
	afx_msg void OnBnClickedAppAbout();
	static void ShowUsage();
	afx_msg void OnShowwnd();
protected:
	CComboBox m_cmbSelType;
public:
	afx_msg void OnBnClickedLocalhost();
	// identify the target is localhost or a remote machine
	int m_TargetCompterType;

	CString m_Remote_IP;
	CString m_localhost_IP;
	afx_msg void OnBnClickedRemote();
	CString GetLocalhostIP(void);
	BOOL	m_isForce_remote;
	BOOL	m_isReset_remote;
	afx_msg void OnBnClickedClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSHUTDOWNDLG_H__D0D2E086_AA67_4646_92B0_9185BA7D016B__INCLUDED_)
