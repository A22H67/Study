/*version get return value from thread */

#include "std_lib_facilities.h"
#include <Windows.h>
#include <process.h>
CRITICAL_SECTION critical;
struct number {
	int start;
	int end;
};

DWORD WINAPI is_perfect(LPVOID  nz) {
	number* b = (number*)nz;
	int n = b->end;
	int c = b->start;
	
	int a = 0;
	for (int i = c; i < n; i++) {
		int check;
		check = n % i;
		if (check == 0) a += i;	
	}//end for
	return a;
}
int main() {
	HANDLE myhandleA, myhandleB;
	number para;
	int n;
	int i;
	cout << "Input number:";
	cin >> n;

	//set parameter to the struct
	para.start = 1;
	para.end = n / 2;
	DWORD  myThreadID;
	//pass parameter by struct pointer
	myhandleA = CreateThread(0, 0, &is_perfect, &para , 0, &myThreadID);//tính nửa đầu
	//set parameter to the struct
	para.start = n-(n/2);
	para.end = n ;
	myhandleB = CreateThread(0, 0, &is_perfect, &para, 0, &myThreadID);//tính nửa sau
	//wait for two thread until it finish
	WaitForSingleObject(myhandleA, INFINITE);
	WaitForSingleObject(myhandleB, INFINITE);

	//get return value handleA
	DWORD dwResult;
	GetExitCodeThread(myhandleA, &dwResult);
	//get return value handleB
	DWORD dwResult2;
	GetExitCodeThread(myhandleB, &dwResult2);

	//close two handle
	CloseHandle(myhandleA);
	CloseHandle(myhandleB);

	
	if (dwResult + dwResult2 == n) cout <<n<< " is perfect number\n";
	else cout <<n << " is not perfect number\n";

	return 0;
}
