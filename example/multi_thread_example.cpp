#include "std_lib_facilities.h"
#include <Windows.h>
#include <process.h>

CRITICAL_SECTION critical;
struct number {
	int start;
	int end;
};
#define THREADCOUNT 2
HANDLE mutex;

DWORD WINAPI is_perfect(LPVOID  nz) {
	number* b = (number*)nz;
	int n = b->end;
	int c = b->start;
	
	int a = 0;
	for (int i = c; i < n; i++) {
		int check;
		check = n % i;
		if (check == 0) {

			WaitForSingleObject(mutex, INFINITE); //mutex
			a += i;
			ReleaseMutex(mutex);//unlock mutex
		}
	}//end for
	return a;
}
int main() {
	HANDLE myhandleA, myhandleB;
	number a;
	int n;
	int i;
	cout << "Input number:";
	cin >> n;

	//set parameter to the struct
	a.start = 1;
	a.end = n / 2;
	DWORD  myThreadID;
	//pass parameter by struct pointer
	myhandleA = CreateThread(0, 0, &is_perfect, &a , 0, &myThreadID);
	//set parameter to the struct
	a.start = n-(n/2);
	a.end = n ;
	myhandleB = CreateThread(0, 0, &is_perfect, &a, 0, &myThreadID);
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

	DeleteCriticalSection(&critical);
	
	if (dwResult + dwResult2 == n) cout <<n<< " is perfect number\n";
	else cout <<n << " is not perfect number\n";

	return 0;
}
