// MyShutDownDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyShutDown.h"
#include "MyShutDownDlg.h"
#include ".\myshutdowndlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedHelptopic();
	afx_msg void OnStnClickedHomepage();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HELPTOPIC, OnBnClickedHelptopic)
	ON_STN_CLICKED(IDC_HOMEPAGE, OnStnClickedHomepage)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyShutDownDlg dialog

CMyShutDownDlg::CMyShutDownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyShutDownDlg::IDD, pParent)
	, m_Remote_IP(_T(""))
	, m_localhost_IP(_T(""))
	, m_isForce_remote(true)
	, m_isReset_remote(true)
	, m_TargetCompterType(0)
	, m_TimeType(2)
	, m_Date(CTime::GetCurrentTime())
	, m_Time(CTime::GetCurrentTime())
	, m_Hour(0)
	, m_Minute(0)
	, m_Second(0)
	, isDecided(false)
	, hasShowCfmDlg(false)
	, m_isForce(true)
{
	//{{AFX_DATA_INIT(CMyShutDownDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//������ϵͳ�汾��Ŀǰֻ֧��NT/2000/XPϵ��
	dwVersion = GetVersion();
 	if ((DWORD)(LOBYTE(LOWORD(dwVersion))) < 4){      // is Win32s
		AfxMessageBox("��Ǹ����������ʱ��֧��Win32s(win3.x��win9x��)ϵͳ");
		exit(0);
	}

	//---------------------- �趨ini�ļ���,��ͬ·����,ͬ��ִ�г�������Ӧ --
	char szAppPath[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),szAppPath,MAX_PATH);
	int i=strlen(szAppPath);
	while(szAppPath[i]!=92) //92��\��Ascii��
		i--;
	szAppPath[i+1]='\0';
	strIniFileName = _T(szAppPath);
	strIniFileName += AfxGetApp()->m_pszAppName;
	strIniFileName += ".ini";
	//--------------------- ini�ļ����趨��� ----------------------------
	//---------------------�趨wave�ļ�����for alarm ---------------------
	strWavFileName = _T(szAppPath);
	strWavFileName += "alarm.wav";
	dlgAlarm.strWavFileName = strWavFileName;
	//---------------------wave�ļ����趨���-----------------------------
}

void CMyShutDownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyShutDownDlg)
	DDX_Radio(pDX, IDC_ATTIME, m_TimeType);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_Date);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Time);
	DDX_Text(pDX, IDC_EDIT1, m_Hour);
	DDV_MinMaxInt(pDX, m_Hour, 0, 24000);
	DDX_Text(pDX, IDC_EDIT2, m_Minute);
	DDV_MinMaxInt(pDX, m_Minute, 0, 59);
	DDX_Text(pDX, IDC_EDIT3, m_Second);
	DDV_MinMaxInt(pDX, m_Second, 0, 59);
	DDX_Check(pDX, IDC_FORCE, m_isForce);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO1, m_cmbSelType);
	DDX_Text(pDX, IDC_EDIT5, m_Remote_IP);
	DDX_Text(pDX, IDC_EDIT4, m_localhost_IP);
	DDX_Check(pDX, IDC_Force_remote, m_isForce_remote);
	DDX_Check(pDX, IDC_Reset_remote, m_isReset_remote);
}

BEGIN_MESSAGE_MAP(CMyShutDownDlg, CDialog)
	//{{AFX_MSG_MAP(CMyShutDownDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ATTIME, OnAtTime)
	ON_BN_CLICKED(IDC_AFTERTIME, OnAfterTime)
	ON_BN_CLICKED(IDC_HALT_NOW, OnHaltNow)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_END, OnEnd)
	ON_BN_CLICKED(IDC_CSL, OnCsl)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_BN_CLICKED(IDC_ADVSET, OnAdvset)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_APP_ABOUT, OnBnClickedAppAbout)
	ON_COMMAND(ID_SHOWWND, OnShowwnd)
	ON_BN_CLICKED(IDC_LOCALHOST, OnBnClickedLocalhost)
	ON_BN_CLICKED(IDC_REMOTE, OnBnClickedRemote)
	ON_BN_CLICKED(IDCLOSE, OnBnClickedClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyShutDownDlg message handlers

BOOL CMyShutDownDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimer(2, 1000, NULL);	//�趨ȡ��ϵͳʱ��ļ�ʱ��
	//////////////////////////////////////////////////////
	//
	// ��ʾ������ͼ��
	//
	//////////////////////////////////////////////////////

	mynid.cbSize = sizeof(NOTIFYICONDATA);
	mynid.hWnd = this->m_hWnd;
	mynid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	mynid.uCallbackMessage = MYWM_NOTIFYICON;

	CString szToolTip;
	szToolTip = _T("ZeroShutDown-δ�趨");
	_tcscpy(mynid.szTip, szToolTip);
	mynid.uID = IDR_MAINFRAME;
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mynid.hIcon = hIcon;

	::Shell_NotifyIcon(NIM_ADD, &mynid);

	if(hIcon)
		::DestroyIcon(hIcon);

	/////////////////////////////////////////////////////////
	// ������ͼ�괴�����
	////////////////////////////////////////////////////////


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//---------- set title ----------------
	SetWindowText("ZeroShutDown-δ�趨");
	//-------------------------------------

	//---------- initial list box ------------
	// Add  items to the list box.
	DWORD  buffsize = 128;
	TCHAR  buff[128];
	LPTSTR lpszUserName = buff;
	if(GetUserName(lpszUserName, &buffsize)){
		strTmp = "ע�� ";
		strTmp += _T(lpszUserName);
	}else
		strTmp = "ע����ǰ�û�";
	m_cmbSelType.InsertString(TOLOGOFF,	strTmp);
	m_cmbSelType.InsertString(TOLOCK,	"���������");
	m_cmbSelType.InsertString(TOSUSPEND,"����");
	m_cmbSelType.InsertString(TOHIBERNATE, "����");
	m_cmbSelType.InsertString(TOSHUTDOWN, "�ػ�");
	m_cmbSelType.InsertString(TOREBOOT,	"��������");
	m_cmbSelType.InsertString(ALARMONLY,"С����");
	//-----------------------------------------

	////////////////////////////////////////////////////////
	//
	// ���������в���
	//
	////////////////////////////////////////////////////////
	if(__argc >= 2){	//have parameter
		int i;
		CString strTmp;

		// ����-h����
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-h") == 0)	//Ѱ��-h����
				break;
		}
		if(i < __argc){		//�Ѿ��ҵ�-h����
			ShowUsage();
			exit(0);
		}
		
		// ����-t����
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-t") == 0)	//Ѱ��-t����
				break;
		}
		if(i == __argc){	//û���趨-t����
			AfxMessageBox("δ�趨�ػ�ʱ�̣������趨ȡ��");
			return true;
		}else{				//�Ѿ��趨��-t����
			if((i + 2) >= __argc){	//��������
				AfxMessageBox("-t������������鿴����");
				return true;
			}else{
				int nYear, nMonth, nDay, nHour, nMin, nSec;
				i++;
				if(strcmp(__argv[i], "at") == 0){	//at some time
					i++;
					if(strcmp(__argv[i], "now") == 0){	//at now
						timeToShutDown = CTime::GetCurrentTime();
					}else{
						int test = sscanf(__argv[i++], "%d/%d/%d,%d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
						if(test != 6){	//ʱ���ʽ����
							AfxMessageBox("ʱ���ʽ����������");
							return true;
						}
						timeToShutDown = CTime::CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
					}
				}else if(strcmp(__argv[i], "after") == 0){	//after some time
					i++;
					int test = sscanf(__argv[i++], "%d/%d/%d,%d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
					if(test != 6){	//ʱ���ʽ����
						AfxMessageBox("ʱ���ʽ����������");
						return true;
					}
					timeToShutDown = CTime::GetCurrentTime();
					timeToShutDown = CTime::CTime(timeToShutDown.GetYear() + nYear, timeToShutDown.GetMonth() + nMonth, timeToShutDown.GetDay() + nDay, timeToShutDown.GetHour() + nHour, timeToShutDown.GetMinute() + nMin, timeToShutDown.GetSecond() + nSec);
				}else{
					strTmp = "δ֪����:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		// ����-o����
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-o") == 0)	//Ѱ��-o����
				break;
		}
		if(i < __argc){				//�ҵ�-o����
			if(i + 1 >= __argc){	//��������
				AfxMessageBox("-o������������鿴����");
				return true;
			}else{
				i++;
				if(strcmp(__argv[i], "logout") == 0){	//at some time
					m_cmbSelType.SetCurSel(TOLOGOFF);
					shutDownType = TOLOGOFF;
				}else if(strcmp(__argv[i], "shutdown") == 0){	//at some time
					m_cmbSelType.SetCurSel(TOSHUTDOWN);
					shutDownType = TOSHUTDOWN;
				}else if(strcmp(__argv[i], "suspend") == 0){	//at some time
					m_cmbSelType.SetCurSel(TOSUSPEND);
					shutDownType = TOSUSPEND;
				}else if(strcmp(__argv[i], "hibernate") == 0){	//at some time
					m_cmbSelType.SetCurSel(TOHIBERNATE);
					shutDownType = TOHIBERNATE;
				}else if(strcmp(__argv[i], "reboot") == 0){		//at some time
					m_cmbSelType.SetCurSel(TOREBOOT);
					shutDownType = TOREBOOT;
				}else if(strcmp(__argv[i], "lock") == 0){		//at some time
					m_cmbSelType.SetCurSel(TOLOCK);
					shutDownType = TOLOCK;
				}else{
					strTmp = "δ֪����:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		//����-x����
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-x") == 0)	//Ѱ��-x����
				break;
		}
		if(i < __argc){				//�ҵ�-x����
			if(i + 1 >= __argc){	//��������
				AfxMessageBox("-x������������鿴����");
				return true;
			}else{
				i++;
				if(strcmp(__argv[i], "force") == 0){	//at some time
					m_isForce = true;
				}else if(strcmp(__argv[i], "unforce") == 0){
					m_isForce = false;
				}else{
					strTmp = "δ֪����:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		UpdateData(false);
		isDecided = true;			//ȷ���趨
	}

	//////////////////////////////////////////////////////
	// �����в����������
	//////////////////////////////////////////////////////

	//----------------- �������в�������ȡini�ļ� -------
	else{
		// ��ȡ�������ͣ����ؼ������Զ�̼����
		strTmp.Format("%d", m_TargetCompterType);
		m_TargetCompterType = GetPrivateProfileInt("normal", "TargetType", TARGET_LOCALHOST, strIniFileName);
		if ( m_TargetCompterType == TARGET_LOCALHOST )
		{
			this->GetDlgItem(IDC_LOCALHOST)->SendMessage(BM_CLICK);
		}
		else
		{
			this->GetDlgItem(IDC_REMOTE)->SendMessage(BM_CLICK);
		}

		// �趨ʱ�䷽ʽ
		// 0: at some time
		// 1: after some time
		// 2: right now
		m_TimeType = GetPrivateProfileInt("normal", "TimeType", 2, strIniFileName);	
		switch ( m_TimeType )
		{
		case 0:
			GetDlgItem(IDC_ATTIME)->SendMessage(BM_CLICK);
			break;
		case 1:
			GetDlgItem(IDC_AFTERTIME)->SendMessage(BM_CLICK);
			break;
		case 2:
			GetDlgItem(IDC_HALT_NOW)->SendMessage(BM_CLICK);
			break;
		}
																					
		// �趨���عػ���ʽ
		m_cmbSelType.SetCurSel(GetPrivateProfileInt("normal", "ShutDownType", TOSHUTDOWN, strIniFileName));

		// �趨�Ƿ�ǿ�ƹرձ���Ӧ�ó���
		if(GetPrivateProfileInt("normal", "isForced", 1, strIniFileName) == 1)
			m_isForce = true;
		else
			m_isForce = false;

		// �趨Զ�̹ػ���ʽ
		if(GetPrivateProfileInt("normal", "isReset_remote", 1, strIniFileName) == 1)
			m_isReset_remote = true;
		else
			m_isReset_remote = false;

		// �趨�Ƿ�ǿ�ƹر�Զ��Ӧ�ó���
		if(GetPrivateProfileInt("normal", "isForce_remote", 1, strIniFileName) == 1)
			m_isForce_remote = true;
		else
			m_isForce_remote = false;

		UpdateData(false);
	}

	//�����������
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1))->SetFormat(_T("HH:mm:ss"));	//��ʾʱ��
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2))->SetFormat(_T("yyyy-MM-dd"));	//��ʾ����

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyShutDownDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyShutDownDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyShutDownDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CMyShutDownDlg::OnAtTime() 
{
//	UpdateData(true);	// ����UpdateData����Ϊ�˱�����û�а�ȷ����ʱ����ֶԻ�������У�����ʵ������ʱ���������Ӧ���������
	m_TimeType = 0;
	TypeSet();
}

void CMyShutDownDlg::OnAfterTime() 
{
//	UpdateData(true);
	m_TimeType = 1;
	TypeSet();
}

void CMyShutDownDlg::OnHaltNow() 
{
//	UpdateData(true);
	m_TimeType = 2;
	TypeSet();	
}

void CMyShutDownDlg::OnOK() 
{
	CString strTmp;
	MyConfirmDlg cfmDlg;
	CTimeSpan timeLeft;


	if(isDecided && AfxMessageBox("ȷʵҪ�����ϴ��趨��", MB_OKCANCEL) != IDOK)
		return;
	if(UpdateData(true) == 0)	//������Ϣ����
		return;

	//��ȡ�ػ�ʱ��
	switch(m_TimeType){		
		
		case 0:	// ��ָ��ʱ�̹ػ�
			timeToShutDown = CTime::CTime(m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(), m_Time.GetHour(), m_Time.GetMinute(), m_Time.GetSecond());
			break;
		
		case 1: // ��һ��ʱ���ػ�
			//ȡ�ùػ�ʱ��
			timeNow = CTime::GetCurrentTime();
			timeToShutDown = CTime::CTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), timeNow.GetHour() + m_Hour, timeNow.GetMinute() + m_Minute, timeNow.GetSecond() + m_Second);
			break;

		case 2: // ���̹ػ�
			this->SetWindowText("ZeroShutDown-���̹ر�");
			timeToShutDown = CTime::GetCurrentTime();
			break;

		default:
			break;
	}

	//------------------------------
	// ���ؼ����
	//------------------------------
	if(m_TargetCompterType == TARGET_LOCALHOST){

		// ��ȡ���عػ�Ȩ�ޡ�ע��AdjustPrivileges ���÷����Լ���εõ�tokenhandle
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		//�õ���ǰ���̵�����
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
		//�õ��ػ���Ȩ��Luid
		LookupPrivilegeValue(NULL,TEXT("SeShutdownPrivilege"),&tkp.Privileges[0].Luid);
		//ָ��һ�����õ���Ȩ���Եĸ���
		tkp.PrivilegeCount=1;
		//��Ȼ�û������йػ���Ȩ,����״̬ȱʡ��disable��
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		if(AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0) == 0){
			AfxMessageBox("�趨Ȩ��ʧ��");
			exit(0);
		}

		//��ȡ�ػ���������
		shutDownType = m_cmbSelType.GetCurSel();

		//////////////////////////////////////////////////////////////
		// if��ʱ����ʾ�û��������������Ϣ
		/////////////////////////////////////////////////////////////
		if(shutDownType == ALARMONLY){
			if(dlgAlarmMsgSet.DoModal() == IDOK)
				dlgAlarm.txtAlarmMsg = dlgAlarmMsgSet.TxtAlarmMsg;
			else
				return;
		}


		// �õ���ǰ��������
		switch(shutDownType){
			case TOLOGOFF:
				strExecType = "[ע��]";
				break;
			case TOLOCK:
				strExecType = "[����]";
				break;
			case TOSUSPEND:
				strExecType = "[����]";
				break;
			case TOHIBERNATE:
				strExecType = "[����]";
				break;
			case TOSHUTDOWN:
				strExecType = "[�ر�]";
				break;
			case TOREBOOT:
				strExecType = "[����]";
				break;
			case ALARMONLY:
				strExecType = "[����]";
				break;
			default:
				strExecType = "";
				break;
		}
		isDecided = true;			//ȷ���趨
		ShowWindow(SW_HIDE);

	}

	//------------------------------
	// Զ�̼����
	//------------------------------
	else{
		//����Զ�̼����
		DWORD dwResult;
		NETRESOURCE nr;
		nr.dwType = RESOURCETYPE_ANY;
		nr.lpLocalName = (LPSTR)NULL;
		nr.lpRemoteName = m_Remote_IP.GetBuffer(0);
		nr.lpProvider = (LPSTR)NULL;

		dwResult = WNetAddConnection2(&nr, NULL, NULL, CONNECT_INTERACTIVE);
		switch(dwResult){
			case NO_ERROR:	//���ӳɹ�
				break;
			case ERROR_CANCELLED:	//�û�ȡ������
				Reset();
				return;
			case ERROR_SESSION_CREDENTIAL_CONFLICT:	//�Ѵ�������
				AfxMessageBox("�Ѿ�����ͬԶ�̼���������ӣ�������������������IPC���ӣ�������net use����鿴���޸ģ���\nΪ��ȫ��������ֶ�ȡ�����Ӻ�����");
				Reset();
				return;
			case ERROR_ACCESS_DENIED:
				AfxMessageBox("����Զ�̼����ʧ�ܣ��޷���Ȩ��");
				Reset();
				return;
			case ERROR_BAD_NET_NAME:
				AfxMessageBox("����Զ�̼����ʧ�ܣ�û���ҵ�Ŀ������");
				Reset();
				return;
			case ERROR_BAD_NETPATH:
				AfxMessageBox("����Զ�̼����ʧ�ܣ�δ�ҵ�����·��");
				Reset();
				return;
			case ERROR_NO_NETWORK:
				AfxMessageBox("����Զ�̼����ʧ�ܣ����粻����");
				Reset();
				return;
			case ERROR_INVALID_PASSWORD:
				AfxMessageBox("����Զ�̼����ʧ�ܣ��������");
				Reset();
				return;
			default:
				CString strTmp;
				strTmp.Format("%d", dwResult);
				AfxMessageBox("����Զ�̼����ʧ�ܣ��������:" + strTmp);
				Reset();
				return;
		}

		//��ȡȨ��
		HANDLE token;
		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&token)){
			AfxMessageBox(GetLastError());
			abort();
		}
		TOKEN_PRIVILEGES priv[2];
		priv[0].PrivilegeCount=2;
		if(!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&priv[0].Privileges[0].Luid)){
			AfxMessageBox(GetLastError());
			CloseHandle(token);
			abort();
		}
		priv[0].Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

		if(!LookupPrivilegeValue(NULL,SE_REMOTE_SHUTDOWN_NAME,&priv[0].Privileges[1].Luid)){
			AfxMessageBox(GetLastError());
			CloseHandle(token);
			abort();
		}
		priv[0].Privileges[1].Attributes=SE_PRIVILEGE_ENABLED;

		if(!AdjustTokenPrivileges(token,false,priv,sizeof priv,NULL,NULL)){
			AfxMessageBox(GetLastError());
			CloseHandle(token);
			abort();
		}
		CloseHandle(token);

		if(m_isReset_remote)
			strExecType = "[����Զ�̼����]";
		else
			strExecType = "[�ر�Զ�̼����]";
		isDecided = true;
		ShowWindow(SW_HIDE);
	}
}

void CMyShutDownDlg::TypeSet()
{
	if(m_TimeType == 0){
		GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(true);
		GetDlgItem(IDC_EDIT1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	}
	else if(m_TimeType == 1){
		GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT1)->EnableWindow(true);
		GetDlgItem(IDC_EDIT2)->EnableWindow(true);
		GetDlgItem(IDC_EDIT3)->EnableWindow(true);
	}
	else if(m_TimeType == 2){
		GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
	}
}



void CMyShutDownDlg::OnTimer(UINT nIDEvent) 
{
	timeTmp = CTime::GetCurrentTime();	// �õ�ÿ��һ����ȡ��һ��ϵͳ��ǰʱ��
	
	//�ѵ��ػ�ʱ��
	if(isDecided && timeTmp >= timeToShutDown)
		ToShutDown(shutDownType, m_TargetCompterType);

	//�Ѿ��趨
	if(isDecided){
		CTimeSpan timeLeft = timeToShutDown - timeTmp;

		//��ʾ������
		if(timeLeft > 0)
			strTitleTmp = timeLeft.Format("��ִ�л���:%D��%HСʱ%M��%S��");
		else
			strTitleTmp = "����ִ��";
		strTitleTmp = "ZeroShutDown-" + strTitleTmp;
		strTitleTmp += strExecType;
		this->SetWindowText(strTitleTmp);
		HICON hIcon;	//�ı�ͼ��
		hIcon = AfxGetApp()->LoadIcon(IDR_ICONRUN);
		this->SetIcon(hIcon, false);
		
		//��ʾ��������ʾ
		CString szToolTip = _T(strTitleTmp);
		_tcscpy(mynid.szTip, szToolTip);
		mynid.uID = IDR_MAINFRAME;
		mynid.hIcon = hIcon;

		::Shell_NotifyIcon(NIM_MODIFY, &mynid);

		if(hIcon)
			::DestroyIcon(hIcon);
	}
	//δ�趨
	else{
		//��ʾ������
		this->SetWindowText("ZeroShutDown-δ�趨");
		HICON hIcon;	//�ı�ͼ��
		hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		this->SetIcon(hIcon, false);

		
		//��ʾ��������ʾ
		CString szToolTip = _T("ZeroShutDown-δ�趨");
		_tcscpy(mynid.szTip, szToolTip);
		mynid.uID = IDR_MAINFRAME;
		mynid.hIcon = hIcon;

		::Shell_NotifyIcon(NIM_MODIFY, &mynid);

		if(hIcon)
			::DestroyIcon(hIcon);
	}

}

void CMyShutDownDlg::ToShutDown(int ShutDownType, bool target_type)
{
	if(!hasShowCfmDlg){
		hasShowCfmDlg = true;			// ����Ѿ���ʾ��ȷ�ϴ���
		MyConfirmDlg	cfmDlg;			// ȷ�ϴ���

		if(target_type == TARGET_LOCALHOST){	//���ؼ����
			switch(ShutDownType){
			case TOLOCK:
				LockWorkStation();
				break;

			case TOLOGOFF:
				cfmDlg.SetCfmMsg("׼��ע����ǰ�û�...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_LOGOFF;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("��������ʧ�ܣ��������: " + strTmp);
					}
				}
				else{
					AfxMessageBox("�����Ѿ�ȡ�������������趨");
					ShowWindow(SW_SHOW);		//��ʾ������
				}
				break;

			case TOSUSPEND:
				cfmDlg.SetCfmMsg("׼������...");
				if(cfmDlg.DoModal() == IDOK){
					if(!SetSystemPowerState(true, m_isForce)){
						AfxMessageBox("��������ʧ�ܣ��������: " + strTmp);
					}
				}
				else{
					AfxMessageBox("�����Ѿ�ȡ�������������趨");
					ShowWindow(SW_SHOW);		//��ʾ������
				}
				break;

			case TOHIBERNATE:
				cfmDlg.SetCfmMsg("׼������...");
				if(cfmDlg.DoModal() == IDOK){
					if(!SetSystemPowerState(false, m_isForce)){
						AfxMessageBox("��������ʧ�ܣ��������: " + strTmp);
					}
				}
				else{
					AfxMessageBox("�����Ѿ�ȡ�����������趨");
					ShowWindow(SW_SHOW);		//��ʾ������
				}
				break;

			case TOSHUTDOWN:
				cfmDlg.SetCfmMsg("׼���رձ��ؼ����...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_POWEROFF;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("��������ʧ�ܣ��������: " + strTmp);
					}
				}
				else{
					AfxMessageBox("�����Ѿ�ȡ�����������趨");
					ShowWindow(SW_SHOW);		//��ʾ������
				}
				break;
			case TOREBOOT:
				cfmDlg.SetCfmMsg("׼���������ؼ����...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_REBOOT;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("��������ʧ�ܣ��������: " + strTmp);
					}
				}
				else{
					AfxMessageBox("�����Ѿ�ȡ�����������趨");
					ShowWindow(SW_SHOW);		//��ʾ������
				}
				break;
			case ALARMONLY:
				// ��ʾ��ʱ��ʾ��Ϣ
				dlgAlarm.DoModal();
				break;
			
			default:
				break;
			}
			Reset();
		}
		else{	//Զ�̼����
			if(m_isReset_remote)
				cfmDlg.SetCfmMsg("׼������Զ�̼����(" + m_Remote_IP + ")...");
			else
				cfmDlg.SetCfmMsg("׼���ر�Զ�̼����(" + m_Remote_IP + ")...");
			if(cfmDlg.DoModal() == IDOK){
				if(InitiateSystemShutdown(m_Remote_IP.GetBuffer(0), "", 0, m_isForce_remote, m_isReset_remote) == 0){
					AfxMessageBox("����ʧ��");
				}
				else{
					AfxMessageBox("�����ɹ�");
				}
				ShowWindow(SW_SHOW);
			}
			else{
				Reset();
				AfxMessageBox("�����Ѿ�ȡ�����������趨");
				ShowWindow(SW_SHOW);		//��ʾ������
			}
			Reset();
		}
	}
}

BOOL CMyShutDownDlg::PreCreateWindow(CREATESTRUCT& cs) 
{

	return CDialog::PreCreateWindow(cs);
	
}

LRESULT CMyShutDownDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	BOOL isWndShow;
	switch(message){ 
	case MYWM_NOTIFYICON:	//������û��������Ϣ 
		if(lParam == WM_LBUTTONDBLCLK){	//���˫��ʱ
			isWndShow = this->IsWindowVisible();
			if(this->IsWindowVisible())	//���ڿɼ�������
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			else{						//������������ʾ
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
				this->ActivateTopParent();
			}
		} 
		else if(lParam == WM_RBUTTONDOWN){ //����Ҽ���������ѡ�� 
			CMenu menu; 
			menu.LoadMenu(IDR_RIGHT_MENU); //�������ȶ����ѡ�� 
			CMenu *pMenu = menu.GetSubMenu(0);
			CPoint pos; 
			GetCursorPos(&pos); 
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd()); 
		} 
		break; 
	case WM_SYSCOMMAND:		//�����ϵͳ��Ϣ 
		if(wParam == SC_CLOSE){	//���ر�ͼ������������
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); 
			return 0; 
		} 
		break; 

	} 
	return CWnd::WindowProc(message, wParam, lParam); 
} 

void CMyShutDownDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CMyShutDownDlg::OnEnd() 
{
	OnCancel();
}

void CMyShutDownDlg::OnCsl() 
{
	if(isDecided){			//if has set the shutdown time
		if(AfxMessageBox("ȷʵҪȡ���ϴ��趨��", MB_OKCANCEL) == IDOK){	//let the user affirm
			isDecided = false;	
			AfxMessageBox("�ϴ��趨�Ѿ�ȡ��");
		}
	}
	else
		AfxMessageBox("����û�н����κ��趨");
}

void CMyShutDownDlg::OnCancel() 
{
	ShowWindow(SW_HIDE);
}

void CMyShutDownDlg::OnAppAbout() 
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMyShutDownDlg::OnHide() 
{
	ShowWindow(SW_HIDE);
}

void CMyShutDownDlg::OnAdvset() 
{
	// --------------------����ǰ�趨������ini�ļ��� ------------------------------
	UpdateData(true);


	// �趨�������ͣ����ؼ������Զ�̼����
	strTmp.Format("%d", m_TargetCompterType);
	WritePrivateProfileString("normal", "TargetType", strTmp, strIniFileName);

	// �趨ʱ�䷽ʽ
	strTmp.Format("%d", m_TimeType);
	WritePrivateProfileString("normal", "TimeType", strTmp, strIniFileName);	// 0: at some time
																				// 1: after some time
																				// 2: at now
	// �趨���عػ���ʽ
	strTmp.Format("%d", m_cmbSelType.GetCurSel());
	WritePrivateProfileString("normal", "ShutDownType", strTmp, strIniFileName);
	// �趨�Ƿ�ǿ�ƹرձ���Ӧ�ó���
	if(m_isForce)
		WritePrivateProfileString("normal", "isForced", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isForced", "0", strIniFileName);

	// �趨Զ�̹ػ���ʽ
	if(m_isReset_remote)
		WritePrivateProfileString("normal", "isReset_remote", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isReset_remote", "0", strIniFileName);

	// �趨�Ƿ�ǿ�ƹر�Զ��Ӧ�ó���
	if(m_isForce_remote)
		WritePrivateProfileString("normal", "isForce_remote", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isForce_remote", "0", strIniFileName);

	//-------------------- ������� ------------------------------------------------
	//// �趨ini�ļ�����Ϊ����
	//SetFileAttributes(strIniFileName, FILE_ATTRIBUTE_HIDDEN);
}

void CMyShutDownDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_ESCAPE){	//��Escape����������
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); 
		return; 
	} 
			
//	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyShutDownDlg::OnBnClickedAppAbout()
{
	// TODO: Add your control notification handler code here
}

void CMyShutDownDlg::ShowUsage(void)
{
	CString strTmp = "�������÷���\nZeroShutDown [��h] [-t [at {\"time\"}] | [after {\"time\"}]] [-o {\"option\"}] [-x {\"extend option\"}]";
	strTmp += "\n\n-h��ӡ����ʾ��Ϣ����ʱ�����������ԣ�������Ӧ�ó���";
	strTmp += "\n\n-tΪ�����������������������Ч";
	strTmp += "\n\ntime������Ϊʱ�䣬����now��������at֮��";
	strTmp += "\ntime��ʱ���ʽΪ: yyyy/mm/dd,hh:mm:ss ��ע�⣬�м��޿ո�";
	strTmp += "\ntime��Ϊnow�����ʾ�����˳�";
	strTmp += "\n\noption����Ϊlogout��lock��suspend��hibernate��shutdown��reboot��\n���е�һ�����ֵĲ�����Ч��ȱʡΪlogout";
	strTmp += "\n\nextend optionĿǰ��ѡ����Ϊforce��unforce����ʾ�Ƿ�ǿ�ƹر�Ӧ�ó���ȱʡΪforce";
	strTmp += "\n\n����\tZeroShutDown -t at now -o reboot -x force";
	strTmp += "\n\tZeroShutDown -t after 0/0/0,0:30:0 -o reboot -x force";
	strTmp += "\n\nĿǰ�����н����Բ������ؼ����";
	AfxMessageBox(strTmp);
}

void CMyShutDownDlg::OnShowwnd()
{
	//��ʾ����
	this->ShowWindow(SW_SHOW);
}

void CMyShutDownDlg::Reset(void)
{
	isDecided = false;
	hasShowCfmDlg = false;

	//ȡ���Ѿ���������������
	if(m_TargetCompterType == TARGET_REMOTECOMP){

		DWORD dwResult; 		
		dwResult = WNetCancelConnection2(m_Remote_IP, 0, false); 

		//��Ҫ��dwResult���м�⣬�˰汾��
	}

	
}


void CAboutDlg::OnBnClickedHelptopic()
{
	CMyShutDownDlg::ShowUsage();
}

void CMyShutDownDlg::OnBnClickedLocalhost()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_TargetCompterType = TARGET_LOCALHOST;
	GetDlgItem(IDC_EDIT4)->EnableWindow(true);
	GetDlgItem(IDC_COMBO1)->EnableWindow(true);
	GetDlgItem(IDC_FORCE)->EnableWindow(true);

	GetDlgItem(IDC_EDIT5)->EnableWindow(false);
	GetDlgItem(IDC_Reset_remote)->EnableWindow(false);
	GetDlgItem(IDC_Force_remote)->EnableWindow(false);

	m_localhost_IP = GetLocalhostIP();
	UpdateData(false);
}

void CMyShutDownDlg::OnBnClickedRemote()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_TargetCompterType = TARGET_REMOTECOMP;
	GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	GetDlgItem(IDC_COMBO1)->EnableWindow(false);
	GetDlgItem(IDC_FORCE)->EnableWindow(false);

	GetDlgItem(IDC_EDIT5)->EnableWindow(true);
	GetDlgItem(IDC_Reset_remote)->EnableWindow(true);
	GetDlgItem(IDC_Force_remote)->EnableWindow(true);
	GetDlgItem(IDC_EDIT5)->SetFocus();
	UpdateData(false);

}

//Get the IP address of local host.
//If there are more than one IP, Display the last one
CString CMyShutDownDlg::GetLocalhostIP(void)
{
	char szHostName[128]; 
 	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	CString strIP = "";
	 
	wVersionRequested = MAKEWORD( 2, 2 );
	 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		AfxMessageBox("could not find a usable Winsock DLL.");
		return "error";
	}

    if( gethostname(szHostName, 128) == 0 ){ 
       struct hostent * pHost;
       pHost = gethostbyname(szHostName); 
       for( int i = 0; pHost!= NULL && pHost->h_addr_list[i]!= NULL; i++ ){ 
			char *ip;
			ip = inet_ntoa(*((in_addr *)pHost->h_addr_list[i]));
			if(strIP == "")
				strIP = _T(ip);
			else{
				strIP += _T("; ");
				strIP += _T(ip);
			}
       } 
    } 

	return strIP;
}

void CAboutDlg::OnStnClickedHomepage()
{
	ShellExecute(NULL, "open", "http://www.zeroapp.com", NULL, NULL, SW_SHOWNORMAL);
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rect; 
	CPoint ptCursor; 
	CWnd *pStatic = GetDlgItem(IDC_HOMEPAGE); 
	pStatic->GetWindowRect(&rect); 
	GetCursorPos(&ptCursor); 
	if (rect.PtInRect (ptCursor)) 
	{ 
		CWinApp *pApp = AfxGetApp(); 
		HICON hIconBang=pApp->LoadCursor(IDC_PointToWeb); 
		SetCursor(hIconBang); 
		return TRUE; 
	} 
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CMyShutDownDlg::OnBnClickedClose()
{
	if(isDecided){		//if has set the shutdown time
		if(AfxMessageBox("ȷʵҪ�˳��������⽫ȡ���ػ��趨", MB_OKCANCEL) == IDOK){	//let the user affirm
			::Shell_NotifyIcon(NIM_DELETE, &mynid);
			CDialog::OnCancel();
		}
	}
	else{
		::Shell_NotifyIcon(NIM_DELETE, &mynid);
		CDialog::OnCancel();
	}
}
