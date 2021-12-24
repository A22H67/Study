#include<Windows.h>
#include<iostream>
using namespace std;
void CALLBACK WinEventProc(
	HWINEVENTHOOK hWinEventHook,
	DWORD event,
	HWND hwnd,
	LONG idObject,
	LONG idChild,
	DWORD dwEventThread,
	DWORD dwmsEventTime
)
{
		HWND cal_hwnd = NULL;
		cal_hwnd = FindWindowA(NULL, (LPCSTR)"Calculator");
		if (cal_hwnd != NULL) {
				
				if (SetWindowTextA(cal_hwnd, (LPCSTR)"ABCD")) {
					cout << "OK\n";
					exit(1);
				}
	}
}
void MessageLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}
int main() {
	LPCSTR  target_app = "C:\\WINDOWS\\system32\\calc.exe";
	LPSTARTUPINFOA startup_info = new STARTUPINFOA();
	LPPROCESS_INFORMATION  process_infor = new PROCESS_INFORMATION();
	
	
	if (!CreateProcessA(target_app, NULL, NULL, NULL, FALSE, 0, NULL,
		NULL, startup_info, process_infor)
		) {
		cout << "Can't CreateProcessA:" << GetLastError() << '\n';
		return 0;
	}
	//Sleep(1000);
	//HWND cal_hwnd = NULL;
		/*while (cal_hwnd == NULL) {
		cal_hwnd = FindWindowA(NULL, (LPCSTR)"Calculator");
	}*/
	HWINEVENTHOOK hEventHook = SetWinEventHook(EVENT_OBJECT_CREATE,
		EVENT_OBJECT_DESTROY, NULL, WinEventProc,process_infor->dwProcessId,
		0, 
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	);
	MessageLoop();
	

}

