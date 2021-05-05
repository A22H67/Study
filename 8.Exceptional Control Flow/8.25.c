#include "csapp.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

sigjmp_buf buf;

int kbhit(void)
{
    //code from https://stackoverflow.com/questions/32390617/get-keyboard-interrupt-in-c
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
void handler_arlam()
{

    siglongjmp(buf,1);
}
void handler_interrupt()
{
    siglongjmp(buf,2);
}
char *tfgets(char *str,int n,FILE *stream)
{

    switch(sigsetjmp(buf,1))
    {
    case 0:
        signal(SIGINT,handler_interrupt);
        signal(SIGALRM,handler_arlam);
        alarm(5);

        while(!kbhit())
            ;
        kill(0,SIGINT);
        break;
    case 1:   // alarm
        return NULL;
    case 2: //interrup
        return fgets(str,n,stream);

    }
}
int main()
{
    char line[10];
    //alarm 1 interrupt 2
    if(tfgets(line,10,stdin)!=NULL )
    {
        puts(line);
    }
    else{
         printf("Time Out\n");
    }
}
