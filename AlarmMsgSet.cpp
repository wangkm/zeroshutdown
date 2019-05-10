// AlarmMsgSet.cpp : implementation file
//

#include "stdafx.h"
#include "MyShutDown.h"
#include "AlarmMsgSet.h"


// AlarmMsgSet dialog

IMPLEMENT_DYNAMIC(AlarmMsgSet, CDialog)
AlarmMsgSet::AlarmMsgSet(CWnd* pParent /*=NULL*/)
	: CDialog(AlarmMsgSet::IDD, pParent)
	, TxtAlarmMsg(_T(""))
{
}

AlarmMsgSet::~AlarmMsgSet()
{
}

void AlarmMsgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, TxtAlarmMsg);
}


BEGIN_MESSAGE_MAP(AlarmMsgSet, CDialog)
END_MESSAGE_MAP()


// AlarmMsgSet message handlers
