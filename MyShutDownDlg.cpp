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

	//检测操作系统版本，目前只支持NT/2000/XP系列
	dwVersion = GetVersion();
 	if ((DWORD)(LOBYTE(LOWORD(dwVersion))) < 4){      // is Win32s
		AfxMessageBox("抱歉，本程序暂时不支持Win32s(win3.x、win9x等)系统");
		exit(0);
	}

	//---------------------- 设定ini文件名,相同路径下,同可执行程序名对应 --
	char szAppPath[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),szAppPath,MAX_PATH);
	int i=strlen(szAppPath);
	while(szAppPath[i]!=92) //92是\的Ascii码
		i--;
	szAppPath[i+1]='\0';
	strIniFileName = _T(szAppPath);
	strIniFileName += AfxGetApp()->m_pszAppName;
	strIniFileName += ".ini";
	//--------------------- ini文件名设定完毕 ----------------------------
	//---------------------设定wave文件名，for alarm ---------------------
	strWavFileName = _T(szAppPath);
	strWavFileName += "alarm.wav";
	dlgAlarm.strWavFileName = strWavFileName;
	//---------------------wave文件名设定完毕-----------------------------
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
	SetTimer(2, 1000, NULL);	//设定取得系统时间的计时器
	//////////////////////////////////////////////////////
	//
	// 显示任务栏图标
	//
	//////////////////////////////////////////////////////

	mynid.cbSize = sizeof(NOTIFYICONDATA);
	mynid.hWnd = this->m_hWnd;
	mynid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	mynid.uCallbackMessage = MYWM_NOTIFYICON;

	CString szToolTip;
	szToolTip = _T("ZeroShutDown-未设定");
	_tcscpy(mynid.szTip, szToolTip);
	mynid.uID = IDR_MAINFRAME;
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mynid.hIcon = hIcon;

	::Shell_NotifyIcon(NIM_ADD, &mynid);

	if(hIcon)
		::DestroyIcon(hIcon);

	/////////////////////////////////////////////////////////
	// 任务栏图标创建完毕
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
	SetWindowText("ZeroShutDown-未设定");
	//-------------------------------------

	//---------- initial list box ------------
	// Add  items to the list box.
	DWORD  buffsize = 128;
	TCHAR  buff[128];
	LPTSTR lpszUserName = buff;
	if(GetUserName(lpszUserName, &buffsize)){
		strTmp = "注销 ";
		strTmp += _T(lpszUserName);
	}else
		strTmp = "注销当前用户";
	m_cmbSelType.InsertString(TOLOGOFF,	strTmp);
	m_cmbSelType.InsertString(TOLOCK,	"锁定计算机");
	m_cmbSelType.InsertString(TOSUSPEND,"待机");
	m_cmbSelType.InsertString(TOHIBERNATE, "休眠");
	m_cmbSelType.InsertString(TOSHUTDOWN, "关机");
	m_cmbSelType.InsertString(TOREBOOT,	"重新启动");
	m_cmbSelType.InsertString(ALARMONLY,"小闹钟");
	//-----------------------------------------

	////////////////////////////////////////////////////////
	//
	// 处理命令行参数
	//
	////////////////////////////////////////////////////////
	if(__argc >= 2){	//have parameter
		int i;
		CString strTmp;

		// 处理-h参数
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-h") == 0)	//寻找-h参数
				break;
		}
		if(i < __argc){		//已经找到-h参数
			ShowUsage();
			exit(0);
		}
		
		// 处理-t参数
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-t") == 0)	//寻找-t参数
				break;
		}
		if(i == __argc){	//没有设定-t参数
			AfxMessageBox("未设定关机时刻，其他设定取消");
			return true;
		}else{				//已经设定了-t参数
			if((i + 2) >= __argc){	//参数不够
				AfxMessageBox("-t参数不够，请查看帮助");
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
						if(test != 6){	//时间格式有误
							AfxMessageBox("时间格式有误，请重试");
							return true;
						}
						timeToShutDown = CTime::CTime(nYear, nMonth, nDay, nHour, nMin, nSec);
					}
				}else if(strcmp(__argv[i], "after") == 0){	//after some time
					i++;
					int test = sscanf(__argv[i++], "%d/%d/%d,%d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);
					if(test != 6){	//时间格式有误
						AfxMessageBox("时间格式有误，请重试");
						return true;
					}
					timeToShutDown = CTime::GetCurrentTime();
					timeToShutDown = CTime::CTime(timeToShutDown.GetYear() + nYear, timeToShutDown.GetMonth() + nMonth, timeToShutDown.GetDay() + nDay, timeToShutDown.GetHour() + nHour, timeToShutDown.GetMinute() + nMin, timeToShutDown.GetSecond() + nSec);
				}else{
					strTmp = "未知参数:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		// 处理-o参数
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-o") == 0)	//寻找-o参数
				break;
		}
		if(i < __argc){				//找到-o参数
			if(i + 1 >= __argc){	//参数不够
				AfxMessageBox("-o参数不够，请查看帮助");
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
					strTmp = "未知参数:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		//处理-x参数
		for(i = 1; i < __argc; i++){
			if(strcmp(__argv[i], "-x") == 0)	//寻找-x参数
				break;
		}
		if(i < __argc){				//找到-x参数
			if(i + 1 >= __argc){	//参数不够
				AfxMessageBox("-x参数不够，请查看帮助");
				return true;
			}else{
				i++;
				if(strcmp(__argv[i], "force") == 0){	//at some time
					m_isForce = true;
				}else if(strcmp(__argv[i], "unforce") == 0){
					m_isForce = false;
				}else{
					strTmp = "未知参数:";
					strTmp += _T(__argv[i]);
					AfxMessageBox(strTmp);
					return true;
				}
			}
		}

		UpdateData(false);
		isDecided = true;			//确认设定
	}

	//////////////////////////////////////////////////////
	// 命令行参数处理完毕
	//////////////////////////////////////////////////////

	//----------------- 无命令行参数，读取ini文件 -------
	else{
		// 获取对象类型，本地计算机或远程计算机
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

		// 设定时间方式
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
																					
		// 设定本地关机方式
		m_cmbSelType.SetCurSel(GetPrivateProfileInt("normal", "ShutDownType", TOSHUTDOWN, strIniFileName));

		// 设定是否强制关闭本地应用程序
		if(GetPrivateProfileInt("normal", "isForced", 1, strIniFileName) == 1)
			m_isForce = true;
		else
			m_isForce = false;

		// 设定远程关机方式
		if(GetPrivateProfileInt("normal", "isReset_remote", 1, strIniFileName) == 1)
			m_isReset_remote = true;
		else
			m_isReset_remote = false;

		// 设定是否强制关闭远程应用程序
		if(GetPrivateProfileInt("normal", "isForce_remote", 1, strIniFileName) == 1)
			m_isForce_remote = true;
		else
			m_isForce_remote = false;

		UpdateData(false);
	}

	//定制日期组件
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1))->SetFormat(_T("HH:mm:ss"));	//显示时间
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2))->SetFormat(_T("yyyy-MM-dd"));	//显示日期

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
//	UpdateData(true);	// 不用UpdateData，是为了避免在没有按确定的时候出现对话框数据校验错误。实际上这时的输入错误应该是允许的
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


	if(isDecided && AfxMessageBox("确实要覆盖上次设定？", MB_OKCANCEL) != IDOK)
		return;
	if(UpdateData(true) == 0)	//输入信息有误
		return;

	//获取关机时刻
	switch(m_TimeType){		
		
		case 0:	// 在指定时刻关机
			timeToShutDown = CTime::CTime(m_Date.GetYear(), m_Date.GetMonth(), m_Date.GetDay(), m_Time.GetHour(), m_Time.GetMinute(), m_Time.GetSecond());
			break;
		
		case 1: // 在一段时间后关机
			//取得关机时刻
			timeNow = CTime::GetCurrentTime();
			timeToShutDown = CTime::CTime(timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), timeNow.GetHour() + m_Hour, timeNow.GetMinute() + m_Minute, timeNow.GetSecond() + m_Second);
			break;

		case 2: // 立刻关机
			this->SetWindowText("ZeroShutDown-立刻关闭");
			timeToShutDown = CTime::GetCurrentTime();
			break;

		default:
			break;
	}

	//------------------------------
	// 本地计算机
	//------------------------------
	if(m_TargetCompterType == TARGET_LOCALHOST){

		// 获取本地关机权限。注意AdjustPrivileges 的用法，以及如何得到tokenhandle
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		//得到当前进程的令牌
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
		//得到关机特权的Luid
		LookupPrivilegeValue(NULL,TEXT("SeShutdownPrivilege"),&tkp.Privileges[0].Luid);
		//指定一次设置的特权属性的个数
		tkp.PrivilegeCount=1;
		//虽然用户可能有关机特权,但其状态缺省是disable的
		tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		if(AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0) == 0){
			AfxMessageBox("设定权限失败");
			exit(0);
		}

		//获取关机操作类型
		shutDownType = m_cmbSelType.GetCurSel();

		//////////////////////////////////////////////////////////////
		// if定时，提示用户输入待办事宜信息
		/////////////////////////////////////////////////////////////
		if(shutDownType == ALARMONLY){
			if(dlgAlarmMsgSet.DoModal() == IDOK)
				dlgAlarm.txtAlarmMsg = dlgAlarmMsgSet.TxtAlarmMsg;
			else
				return;
		}


		// 得到当前操作类型
		switch(shutDownType){
			case TOLOGOFF:
				strExecType = "[注销]";
				break;
			case TOLOCK:
				strExecType = "[锁定]";
				break;
			case TOSUSPEND:
				strExecType = "[待机]";
				break;
			case TOHIBERNATE:
				strExecType = "[休眠]";
				break;
			case TOSHUTDOWN:
				strExecType = "[关闭]";
				break;
			case TOREBOOT:
				strExecType = "[重启]";
				break;
			case ALARMONLY:
				strExecType = "[闹钟]";
				break;
			default:
				strExecType = "";
				break;
		}
		isDecided = true;			//确认设定
		ShowWindow(SW_HIDE);

	}

	//------------------------------
	// 远程计算机
	//------------------------------
	else{
		//链接远程计算机
		DWORD dwResult;
		NETRESOURCE nr;
		nr.dwType = RESOURCETYPE_ANY;
		nr.lpLocalName = (LPSTR)NULL;
		nr.lpRemoteName = m_Remote_IP.GetBuffer(0);
		nr.lpProvider = (LPSTR)NULL;

		dwResult = WNetAddConnection2(&nr, NULL, NULL, CONNECT_INTERACTIVE);
		switch(dwResult){
			case NO_ERROR:	//链接成功
				break;
			case ERROR_CANCELLED:	//用户取消操作
				Reset();
				return;
			case ERROR_SESSION_CREDENTIAL_CONFLICT:	//已存在链接
				AfxMessageBox("已经存在同远程计算机的链接（可能是网络驱动器或IPC链接，可以用net use命令查看和修改），\n为安全起见，请手动取消链接后重试");
				Reset();
				return;
			case ERROR_ACCESS_DENIED:
				AfxMessageBox("链接远程计算机失败，无访问权限");
				Reset();
				return;
			case ERROR_BAD_NET_NAME:
				AfxMessageBox("链接远程计算机失败，没有找到目标计算机");
				Reset();
				return;
			case ERROR_BAD_NETPATH:
				AfxMessageBox("链接远程计算机失败，未找到网络路径");
				Reset();
				return;
			case ERROR_NO_NETWORK:
				AfxMessageBox("链接远程计算机失败，网络不可用");
				Reset();
				return;
			case ERROR_INVALID_PASSWORD:
				AfxMessageBox("链接远程计算机失败，口令错误");
				Reset();
				return;
			default:
				CString strTmp;
				strTmp.Format("%d", dwResult);
				AfxMessageBox("链接远程计算机失败，错误代码:" + strTmp);
				Reset();
				return;
		}

		//获取权限
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
			strExecType = "[重启远程计算机]";
		else
			strExecType = "[关闭远程计算机]";
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
	timeTmp = CTime::GetCurrentTime();	// 得到每隔一秒钟取得一次系统当前时间
	
	//已到关机时间
	if(isDecided && timeTmp >= timeToShutDown)
		ToShutDown(shutDownType, m_TargetCompterType);

	//已经设定
	if(isDecided){
		CTimeSpan timeLeft = timeToShutDown - timeTmp;

		//显示标题栏
		if(timeLeft > 0)
			strTitleTmp = timeLeft.Format("距执行还有:%D天%H小时%M分%S秒");
		else
			strTitleTmp = "正在执行";
		strTitleTmp = "ZeroShutDown-" + strTitleTmp;
		strTitleTmp += strExecType;
		this->SetWindowText(strTitleTmp);
		HICON hIcon;	//改变图标
		hIcon = AfxGetApp()->LoadIcon(IDR_ICONRUN);
		this->SetIcon(hIcon, false);
		
		//显示任务栏提示
		CString szToolTip = _T(strTitleTmp);
		_tcscpy(mynid.szTip, szToolTip);
		mynid.uID = IDR_MAINFRAME;
		mynid.hIcon = hIcon;

		::Shell_NotifyIcon(NIM_MODIFY, &mynid);

		if(hIcon)
			::DestroyIcon(hIcon);
	}
	//未设定
	else{
		//显示标题栏
		this->SetWindowText("ZeroShutDown-未设定");
		HICON hIcon;	//改变图标
		hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		this->SetIcon(hIcon, false);

		
		//显示任务栏提示
		CString szToolTip = _T("ZeroShutDown-未设定");
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
		hasShowCfmDlg = true;			// 标记已经显示了确认窗口
		MyConfirmDlg	cfmDlg;			// 确认窗口

		if(target_type == TARGET_LOCALHOST){	//本地计算机
			switch(ShutDownType){
			case TOLOCK:
				LockWorkStation();
				break;

			case TOLOGOFF:
				cfmDlg.SetCfmMsg("准备注销当前用户...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_LOGOFF;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("程序运行失败，错误代码: " + strTmp);
					}
				}
				else{
					AfxMessageBox("任务已经取消，请您重新设定");
					ShowWindow(SW_SHOW);		//显示主窗口
				}
				break;

			case TOSUSPEND:
				cfmDlg.SetCfmMsg("准备待机...");
				if(cfmDlg.DoModal() == IDOK){
					if(!SetSystemPowerState(true, m_isForce)){
						AfxMessageBox("程序运行失败，错误代码: " + strTmp);
					}
				}
				else{
					AfxMessageBox("任务已经取消，请您重新设定");
					ShowWindow(SW_SHOW);		//显示主窗口
				}
				break;

			case TOHIBERNATE:
				cfmDlg.SetCfmMsg("准备休眠...");
				if(cfmDlg.DoModal() == IDOK){
					if(!SetSystemPowerState(false, m_isForce)){
						AfxMessageBox("程序运行失败，错误代码: " + strTmp);
					}
				}
				else{
					AfxMessageBox("任务已经取消，请重新设定");
					ShowWindow(SW_SHOW);		//显示主窗口
				}
				break;

			case TOSHUTDOWN:
				cfmDlg.SetCfmMsg("准备关闭本地计算机...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_POWEROFF;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("程序运行失败，错误代码: " + strTmp);
					}
				}
				else{
					AfxMessageBox("任务已经取消，请重新设定");
					ShowWindow(SW_SHOW);		//显示主窗口
				}
				break;
			case TOREBOOT:
				cfmDlg.SetCfmMsg("准备重启本地计算机...");
				if(cfmDlg.DoModal() == IDOK){
					shutDownSign = EWX_REBOOT;
					if(m_isForce)
						shutDownSign |= EWX_FORCE;
					if((ExitWindowsEx(shutDownSign, 0) == 0)){
						strTmp.Format("%d", GetLastError());
						AfxMessageBox("程序运行失败，错误代码: " + strTmp);
					}
				}
				else{
					AfxMessageBox("任务已经取消，请重新设定");
					ShowWindow(SW_SHOW);		//显示主窗口
				}
				break;
			case ALARMONLY:
				// 显示定时提示消息
				dlgAlarm.DoModal();
				break;
			
			default:
				break;
			}
			Reset();
		}
		else{	//远程计算机
			if(m_isReset_remote)
				cfmDlg.SetCfmMsg("准备重启远程计算机(" + m_Remote_IP + ")...");
			else
				cfmDlg.SetCfmMsg("准备关闭远程计算机(" + m_Remote_IP + ")...");
			if(cfmDlg.DoModal() == IDOK){
				if(InitiateSystemShutdown(m_Remote_IP.GetBuffer(0), "", 0, m_isForce_remote, m_isReset_remote) == 0){
					AfxMessageBox("操作失败");
				}
				else{
					AfxMessageBox("操作成功");
				}
				ShowWindow(SW_SHOW);
			}
			else{
				Reset();
				AfxMessageBox("任务已经取消，请重新设定");
				ShowWindow(SW_SHOW);		//显示主窗口
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
	case MYWM_NOTIFYICON:	//如果是用户定义的消息 
		if(lParam == WM_LBUTTONDBLCLK){	//鼠标双击时
			isWndShow = this->IsWindowVisible();
			if(this->IsWindowVisible())	//窗口可见则隐藏
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			else{						//窗口隐藏则显示
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
				this->ActivateTopParent();
			}
		} 
		else if(lParam == WM_RBUTTONDOWN){ //鼠标右键单击弹出选单 
			CMenu menu; 
			menu.LoadMenu(IDR_RIGHT_MENU); //载入事先定义的选单 
			CMenu *pMenu = menu.GetSubMenu(0);
			CPoint pos; 
			GetCursorPos(&pos); 
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd()); 
		} 
		break; 
	case WM_SYSCOMMAND:		//如果是系统消息 
		if(wParam == SC_CLOSE){	//按关闭图标隐藏主窗口
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
		if(AfxMessageBox("确实要取消上次设定？", MB_OKCANCEL) == IDOK){	//let the user affirm
			isDecided = false;	
			AfxMessageBox("上次设定已经取消");
		}
	}
	else
		AfxMessageBox("您还没有进行任何设定");
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
	// --------------------将当前设定保存在ini文件中 ------------------------------
	UpdateData(true);


	// 设定对象类型，本地计算机或远程计算机
	strTmp.Format("%d", m_TargetCompterType);
	WritePrivateProfileString("normal", "TargetType", strTmp, strIniFileName);

	// 设定时间方式
	strTmp.Format("%d", m_TimeType);
	WritePrivateProfileString("normal", "TimeType", strTmp, strIniFileName);	// 0: at some time
																				// 1: after some time
																				// 2: at now
	// 设定本地关机方式
	strTmp.Format("%d", m_cmbSelType.GetCurSel());
	WritePrivateProfileString("normal", "ShutDownType", strTmp, strIniFileName);
	// 设定是否强制关闭本地应用程序
	if(m_isForce)
		WritePrivateProfileString("normal", "isForced", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isForced", "0", strIniFileName);

	// 设定远程关机方式
	if(m_isReset_remote)
		WritePrivateProfileString("normal", "isReset_remote", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isReset_remote", "0", strIniFileName);

	// 设定是否强制关闭远程应用程序
	if(m_isForce_remote)
		WritePrivateProfileString("normal", "isForce_remote", "1", strIniFileName);
	else
		WritePrivateProfileString("normal", "isForce_remote", "0", strIniFileName);

	//-------------------- 保存完毕 ------------------------------------------------
	//// 设定ini文件属性为隐藏
	//SetFileAttributes(strIniFileName, FILE_ATTRIBUTE_HIDDEN);
}

void CMyShutDownDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_ESCAPE){	//按Escape隐藏主窗口
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
	CString strTmp = "命令行用法：\nZeroShutDown [－h] [-t [at {\"time\"}] | [after {\"time\"}]] [-o {\"option\"}] [-x {\"extend option\"}]";
	strTmp += "\n\n-h打印本提示信息，此时其他参数忽略，不启动应用程序";
	strTmp += "\n\n-t为必设参数，否则其他参数无效";
	strTmp += "\n\ntime可以设为时间，或者now（仅用于at之后）";
	strTmp += "\ntime的时间格式为: yyyy/mm/dd,hh:mm:ss （注意，中间无空格）";
	strTmp += "\ntime若为now，则表示立即退出";
	strTmp += "\n\noption可设为logout、lock、suspend、hibernate、shutdown或reboot，\n其中第一个出现的参数有效。缺省为logout";
	strTmp += "\n\nextend option目前可选参数为force或unforce，表示是否强制关闭应用程序。缺省为force";
	strTmp += "\n\n例：\tZeroShutDown -t at now -o reboot -x force";
	strTmp += "\n\tZeroShutDown -t after 0/0/0,0:30:0 -o reboot -x force";
	strTmp += "\n\n目前命令行仅可以操作本地计算机";
	AfxMessageBox(strTmp);
}

void CMyShutDownDlg::OnShowwnd()
{
	//显示窗口
	this->ShowWindow(SW_SHOW);
}

void CMyShutDownDlg::Reset(void)
{
	isDecided = false;
	hasShowCfmDlg = false;

	//取消已经建立的网络链接
	if(m_TargetCompterType == TARGET_REMOTECOMP){

		DWORD dwResult; 		
		dwResult = WNetCancelConnection2(m_Remote_IP, 0, false); 

		//需要对dwResult进行检测，此版本略
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
		if(AfxMessageBox("确实要退出本程序？这将取消关机设定", MB_OKCANCEL) == IDOK){	//let the user affirm
			::Shell_NotifyIcon(NIM_DELETE, &mynid);
			CDialog::OnCancel();
		}
	}
	else{
		::Shell_NotifyIcon(NIM_DELETE, &mynid);
		CDialog::OnCancel();
	}
}
