; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyShutDownDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MyShutDown.h"

ClassCount=4
Class1=CMyShutDownApp
Class2=CMyShutDownDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MYSHUTDOWN_DIALOG
Class4=MyConfirmDlg
Resource4=IDD_confirm
Resource5=IDR_RIGHT_MENU

[CLS:CMyShutDownApp]
Type=0
HeaderFile=MyShutDown.h
ImplementationFile=MyShutDown.cpp
Filter=N

[CLS:CMyShutDownDlg]
Type=0
HeaderFile=MyShutDownDlg.h
ImplementationFile=MyShutDownDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMyShutDownDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=MyShutDownDlg.h
ImplementationFile=MyShutDownDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYSHUTDOWN_DIALOG]
Type=1
Class=CMyShutDownDlg
ControlCount=25
Control1=IDC_STATIC,button,1342177287
Control2=IDC_ATTIME,button,1342308361
Control3=IDC_AFTERTIME,button,1342177289
Control4=IDC_HALT_NOW,button,1342177289
Control5=IDC_FORCE,button,1342373891
Control6=IDC_RADIO3,button,1342308361
Control7=IDC_POWERDOWN,button,1342177289
Control8=IDC_REBOOT,button,1342177289
Control9=IDOK,button,1342275585
Control10=IDCANCEL,button,1342275584
Control11=IDC_CSL,button,1342275584
Control12=IDC_HIDE,button,1342275584
Control13=IDC_ADVSET,button,1342275584
Control14=ID_APP_ABOUT,button,1342275584
Control15=IDC_DATETIMEPICKER2,SysDateTimePick32,1476591648
Control16=IDC_DATETIMEPICKER1,SysDateTimePick32,1476460601
Control17=IDC_EDIT1,edit,1484849280
Control18=IDC_EDIT2,edit,1484849280
Control19=IDC_EDIT3,edit,1484849280
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352

[DLG:IDD_confirm]
Type=1
Class=MyConfirmDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_Confirm,static,1342308865
Control4=IDC_TIMECNTR,static,1342312961
Control5=IDC_STATIC,button,1342177287

[CLS:MyConfirmDlg]
Type=0
HeaderFile=MyConfirmDlg.h
ImplementationFile=MyConfirmDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=MyConfirmDlg

[MNU:IDR_RIGHT_MENU]
Type=1
Class=CMyShutDownDlg
Command1=ID_SHOWWND
Command2=ID_APP_ABOUT
Command3=ID_END
CommandCount=3

