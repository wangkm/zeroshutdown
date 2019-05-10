// DlgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "MyShutDown.h"
#include "DlgAlarm.h"
#include "Mmsystem.h"

// DlgAlarm dialog

IMPLEMENT_DYNAMIC(DlgAlarm, CDialog)
DlgAlarm::DlgAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAlarm::IDD, pParent)
	, txtAlarmMsg(_T(""))
{
}

DlgAlarm::~DlgAlarm()
{
}

void DlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, txtAlarmMsg);
}


BEGIN_MESSAGE_MAP(DlgAlarm, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// DlgAlarm message handlers

void DlgAlarm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CPen	*oldPen;
	CBrush	*oldBrush;
	CFont	*oldFont;
	RECT rectAlarmMsg;
	rectAlarmMsg.top = 10;
	rectAlarmMsg.left = 10;
	rectAlarmMsg.right =  270;
	rectAlarmMsg.bottom = 90;


	CPen myPen(PS_SOLID, 2, RGB(255,255,0));
	CBrush myBrush(RGB(0, 255, 255));
	oldPen = dc.SelectObject(&myPen);
	oldBrush = dc.SelectObject(&myBrush);
	dc.Rectangle(&rectAlarmMsg);
	CFont myFont;
	VERIFY(myFont.CreateFont(
				24,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial"))); 			   // lpszFacename

	oldFont = dc.SelectObject(&myFont);
	dc.SetTextColor(RGB(255, 0, 0));
	dc.SetBkMode(TRANSPARENT);
	
	dc.DrawText("Alarm！设定时间已到！", &rectAlarmMsg, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_EXPANDTABS | DT_NOCLIP);
	

	// restore the old things
	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldFont);
	myPen.DeleteObject();
	myBrush.DeleteObject();
	myFont.DeleteObject();
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

void DlgAlarm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	//---------------- 有声音文件则播放，否则静音 ---------
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
//	char strWaveFileName[] = "alarm.WAV";
	hFind = FindFirstFile(strWavFileName, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
		sndPlaySound(strWavFileName, SND_ASYNC | SND_LOOP);
	//-----------------------------------------------------
}

void DlgAlarm::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	sndPlaySound(NULL, SND_ASYNC);
}
