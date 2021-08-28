//use mutex to prevent race condition 

#include "std_lib_facilities.h"
#include <Windows.h>
#include <process.h>
CRITICAL_SECTION critical;
HANDLE mutex;
int share_buf = 0;
struct number {
	int start;
	int end;
};


DWORD WINAPI is_perfect(LPVOID  nz) {
	number* b = (number*)nz;
	int n = b->end;
	int c = b->start;
	
	for (int i = c; i < n; i++) {
		int check;
		check = n % i;
		if (check == 0) {
			WaitForSingleObject(mutex, INFINITE);//Enter critical section
			share_buf += i;
			ReleaseMutex(mutex);//()  Leave critical section
		}
	}
	return 0;
}


int main() {
	mutex = CreateMutex(0, 0, 0);//create mutex
	HANDLE myhandleA, myhandleB;
	number para;
	int n;
	int i;
	cout << "Input number:";
	cin >> n;

	//set parameter to the struct
	para.start = 1;
	para.end = n / 2;
	//a.end = n;
	DWORD  myThreadID;
	//pass parameter by struct pointer
	myhandleA = CreateThread(0, 0, &is_perfect, &para, 0, &myThreadID);
	//set parameter to the struct
	para.start = n-(n/2);
	para.end = n ;
	myhandleB = CreateThread(0, 0, &is_perfect, &para, 0, &myThreadID);

	//wait for two thread until it finish
	WaitForSingleObject(myhandleA, INFINITE);
	WaitForSingleObject(myhandleB, INFINITE);

	//close two handle
	CloseHandle(myhandleA);
	CloseHandle(myhandleB);

	CloseHandle(mutex);
	
	if (share_buf == n) cout <<n<< " is perfect number\n";
	else cout <<n << " is not perfect number\n";

	return 0;
}
