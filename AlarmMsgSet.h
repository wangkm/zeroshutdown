#pragma once


// AlarmMsgSet dialog

class AlarmMsgSet : public CDialog
{
	DECLARE_DYNAMIC(AlarmMsgSet)

public:
	AlarmMsgSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~AlarmMsgSet();

// Dialog Data
	enum { IDD = IDD_ALARMMSGSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString TxtAlarmMsg;
};
