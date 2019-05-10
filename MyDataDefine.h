#define TOLOGOFF	0	// logoff
#define TOLOCK		1	// lock
#define TOSUSPEND	2	// suspend
#define TOHIBERNATE	3	// hibernate
#define TOSHUTDOWN	4	// shutdown
#define TOREBOOT	5	// reboot
#define ALARMONLY	6	// alarm only

#define TARGET_LOCALHOST	0
#define TARGET_REMOTECOMP	1

extern "C" DWORD APIENTRY
WNetAddConnection2A(
     IN LPNETRESOURCEA	lpNetResource,
     IN LPCSTR			lpPassword,
     IN LPCSTR			lpUserName,
     IN DWORD			dwFlags
    );

