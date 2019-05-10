// MyConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyShutDown.h"
#include "MyConfirmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MyConfirmDlg dialog


MyConfirmDlg::MyConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MyConfirmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MyConfirmDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MyConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MyConfirmDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS, m_pg);
}


BEGIN_MESSAGE_MAP(MyConfirmDlg, CDialog)
	//{{AFX_MSG_MAP(MyConfirmDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyConfirmDlg message handlers

void MyConfirmDlg::SetCfmMsg(CString theCfmMsg)
{
	cfmMsg = theCfmMsg;
	timeCounter = 10;		// delay 10 seconds, then auto shutdown
	timeMsg.Format("%d", timeCounter);
}

BOOL MyConfirmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetTimer(1, 1000, NULL);
	// TODO: Add extra initialization here
	GetDlgItem(IDC_Confirm)->SetWindowText(cfmMsg);
	m_pg.SetRange(0, 10);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MyConfirmDlg::OnTimer(UINT nIDEvent) 
{
	if(timeCounter > 0){		// reduce the time counter
		timeCounter--;
		m_pg.SetPos(10 - timeCounter);
	}
	else{
		EndDialog(IDOK);		// exit, and return IDOK
	}
//	CDialog::OnTimer(nIDEvent);
}

void MyConfirmDlg::OnCancel() 
{
	CDialog::OnCancel();
}
