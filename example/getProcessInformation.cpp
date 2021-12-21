#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include<winver.h>
#include<winnt.h>
#include <tlhelp32.h>
using namespace std;

typedef struct {
    WORD  wLength;
    WORD  wValueLength;
    WORD  wType;
    WCHAR szKey;
    WORD  Padding;
    DWORD Value;
} Var;

void getProcessIntegrity(HANDLE hProcess) {
    HANDLE hToken;
    DWORD dwLengthNeeded;
    DWORD dwError = ERROR_SUCCESS;
    PTOKEN_MANDATORY_LABEL pTIL = NULL;
    LPWSTR pStringSid;
    DWORD dwIntegrityLevel;
   
    if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken)) {
        if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwLengthNeeded)) {
            dwError = GetLastError();
            if (dwError == ERROR_INSUFFICIENT_BUFFER) {
                pTIL = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwLengthNeeded);
                if (pTIL != NULL) {
                    if (GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwLengthNeeded, &dwLengthNeeded)) {
                        dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid,
                            (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));

                        switch (dwIntegrityLevel) {
                        case SECURITY_MANDATORY_UNTRUSTED_RID:cout << "integrity level:Untrusted level\n";
                            break;
                        case SECURITY_MANDATORY_LOW_RID:cout << "Integrity level:Low \n";
                            break;
                        case SECURITY_MANDATORY_MEDIUM_RID:cout << "Integrity level:Medium \n";
                            break;
                        case SECURITY_MANDATORY_HIGH_RID:cout << "Integrity level:High \n";
                            break;
                        case SECURITY_MANDATORY_SYSTEM_RID:cout << "Integrity level:System \n";
                            break;
                        }//end switch
                    }//end if getToken
               }
         }

        }//end if !getToken

    }//end openProcess
    CloseHandle(hToken);
}//end function
void getImageType(HANDLE hProcess) {
    cout << "Image type:";
    BOOL result;
    IsWow64Process(hProcess, &result);
    if (result) {
        cout << "32 bit\n";
    }
    else cout << "64 bit\n";
}//======================
void getStringName(LPCSTR stringName, LPSTR verData) {
    char str[200];

    LPBYTE lpBuffer = NULL;
    UINT   size = 0;
  
    if (VerQueryValueA(verData, "\\VarFileInfo\\Translation", (LPVOID*)&lpBuffer, &size)) {
        Var* var = (Var*)lpBuffer;
        sprintf_s(str, "\\StringFileInfo\\%04x%04x\\%s",
            var->wLength, var->wValueLength,
            stringName);
        VerQueryValueA(verData, str, (LPVOID*)&lpBuffer, &size);
        if (lpBuffer != 0) {
            cout << stringName << ':' << lpBuffer << '\n';
        }
    }
 
  
   
}//===========================
void getModuleType(LPSTR verData) {
    LPBYTE lpBuffer = NULL;
    UINT   size = 0;
    if (VerQueryValue(verData, (LPCWSTR)"\\", (LPVOID*)&lpBuffer, &size)) {
        VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
        switch (verInfo->dwFileType) {
        case VFT_APP: cout << "Type:application\n";
            break;
        case VFT_DLL: cout << "Type:dll\n";
            break;
        case VFT_DRV: cout << "Type:device driver\n";
            break;
        case VFT_FONT: cout << "Type:font\n";
            break;
        case VFT_STATIC_LIB: cout << "Type:static link library\n";
            break;
        case VFT_UNKNOWN: cout << "Type:unknown\n";
            break;
        case VFT_VXD: cout << "Type:virtual device\n";
            break;
        }
    }
}
void printInformation(DWORD processID, WCHAR* szProcessName)
{
    //TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION , 
        FALSE, processID);
    
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;
      //  if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
           //  GetModuleBaseName(hProcess, hMod, szProcessName,sizeof(szProcessName)/sizeof(TCHAR));//getName
             DWORD  verHandle = 0;
             wchar_t buf[MAX_PATH];
             GetModuleFileNameEx(hProcess,NULL,buf,sizeof(buf));//get path
             DWORD    verSize = GetFileVersionInfoSize(buf, &verHandle);
             if (verSize!=0) {
                 wcout << szProcessName << "(PID:" << processID << ")\n";
                 LPSTR verData = new char[verSize];
                 if (GetFileVersionInfo(buf, verHandle, verSize, verData)) {
                     getModuleType(verData);
                         getStringName("FileDescription", verData);
                         getStringName("CompanyName", verData);
                         getImageType(hProcess);
                         getProcessIntegrity(hProcess);
                         wcout << "Path:" << buf << '\n' << "=======================\n";
                 }
             }  
       // }
    }
    else {
        wcout << szProcessName << "   (PID:" << processID << ")\n";
            cout<<"Err:"<<GetLastError() <<"\n============================\n";
    }
    CloseHandle(hProcess);
}


BOOL EnableDebugPrivilege()
{
    HANDLE hToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return FALSE;

    LUID luid;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
        return FALSE;

    TOKEN_PRIVILEGES tokenPriv;
    tokenPriv.PrivilegeCount = 1;
    tokenPriv.Privileges[0].Luid = luid;
    tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
        return FALSE;

    return TRUE;
}
DWORD isAdminMode() {
    PSID pSid = NULL;
    
    SID_IDENTIFIER_AUTHORITY sid_admin = SECURITY_NT_AUTHORITY;
    BOOL result=FALSE;
    if (!AllocateAndInitializeSid(&sid_admin, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &pSid)) {
        cout << "Err AllocateAndInitializeSid:" << GetLastError() << '\n';     
    }
    if (!CheckTokenMembership(NULL,pSid,&result)) {
        cout << "Err CheckTokenMembership:" << GetLastError() << '\n';
    }
    if (pSid)
    {
        FreeSid(pSid);
        pSid = NULL;
    }
    return result;
}
void requestAdmin() {
    wchar_t path[MAX_PATH];
    if (GetModuleFileName(NULL,path,sizeof(path) ) ) {
        SHELLEXECUTEINFO shell_info;
        shell_info.cbSize = { sizeof(shell_info) };
        shell_info.lpVerb = L"runas";
        shell_info.lpFile = path;
        shell_info.hwnd = NULL;
        shell_info.nShow = SW_NORMAL;
        shell_info.fMask = SEE_MASK_DEFAULT;
        shell_info.lpParameters = NULL;
        shell_info.lpDirectory = NULL;
        shell_info.hInstApp = NULL;
        if (!ShellExecuteEx(&shell_info)) {
            cout << "Err ShellExecute:" << GetLastError() << '\n';
           
        }
    }
    exit(0);
}
int main()
{
   
    unsigned int i=0;
    BOOL isadmin = isAdminMode();
    if (isadmin){
        EnableDebugPrivilege();// only run when run as administrator

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        BOOL cont = Process32FirstW(hSnapshot, &pe32);

        while (cont)
        {
            if (pe32.th32ProcessID)  printInformation(pe32.th32ProcessID, pe32.szExeFile);

            cont = Process32NextW(hSnapshot, &pe32);
            i++;
        }
        cout << "Total Process:" << i << '\n';
        CloseHandle(hSnapshot);
       
    }
    else {
        requestAdmin();
      
    }
    getchar();
    return 0;
}

