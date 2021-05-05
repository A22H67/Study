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

char *tfgets(char *str,int n,FILE *stream)
{

    switch(sigsetjmp(buf,1))
    {
    case 0:

        signal(SIGALRM,handler_arlam);
        sigset_t mask_one;
        sigaddset(&mask_one,SIGALRM);

        alarm(5);
        while(!kbhit()) //checks the keyboard buffer and returns a nonzero value if the buffer has any keypress otherwise 0 is returned.
            ;
        sigprocmask(SIG_BLOCK,&mask_one,NULL);//block signal from alarm

        return fgets(str,n,stream);
    case 1:   // alarm
        return NULL;
    }
}
int main()
{
    char line[10];
    if(tfgets(line,10,stdin)!=NULL )
    {
        puts(line);
    }
    else{
         printf("Time Out\n");
    }
    return 0;
}
