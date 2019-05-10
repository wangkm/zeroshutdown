#pragma once


// DlgAlarm dialog

class DlgAlarm : public CDialog
{
	DECLARE_DYNAMIC(DlgAlarm)

public:
	DlgAlarm(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgAlarm();

// Dialog Data
	enum { IDD = IDD_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CString txtAlarmMsg;
	CString strWavFileName;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
};
