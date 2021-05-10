/*
Using the example in Figure 8.23 as a starting point, write a shell program that
supports job control. Your shell should have the following features:
.
The command line typed by the user consists of a name and zero or more argu-
ments, all separated by one or more spaces. If name is a built-in command, the
shell handles it immediately and waits for the next command line. Otherwise,
the shell assumes that name is an executable file, which it loads and runs in the
context of an initial child process (job). The process group ID for the job is
identical to the PID of the child.

+Each job is identified by either a process ID (PID) or a job ID (JID), which
is a small arbitrary positive integer assigned by the shell. JIDs are denoted on
the command line by the prefix ‘%’. For example, ‘%5’ denotes JID 5, and ‘5’
denotes PID 5.
+If the command line ends with an ampersand, then the shell runs the job in
the background. Otherwise, the shell runs the job in the foreground.
+Typing Ctrl+C (Ctrl+Z) causes the kernel to send a SIGINT (SIGTSTP) signal
to your shell, which then forwards it to every process in the foreground process
group
+The jobs built-in command lists all background jobs.
+The bg job built-in command restarts job by sending it a SIGCONT signal
and then runs it in the background. The job argument can be either a PID or
a JID.
+The fg job built-in command restarts job by sending it a SIGCONT signal and
then runs it in the foreground.
+The shell reaps all of its zombie children. If any job terminates because it
receives a signal that was not caught, then the shell prints a message to the
terminal with the job’s PID and a description of the offending signal.

*/
#include "csapp.h"
#define MAXARGS 128

char *bgjob[MAXARGS];
int  listid[MAXARGS];
volatile int jobs=-1;
volatile int indexx;

void deleteJob(int index){
    free(bgjob[index]);
    for(index;index+1<=jobs;index++){
        bgjob[index]=bgjob[index+1];
        listid[index]=listid[index+1];
    }
    jobs--;
}
void addjob(int pid,char *cmdline){
    char *p=malloc(strlen(cmdline));
            jobs++;
            strcpy(p,cmdline);
            bgjob[jobs]=p;
            listid[jobs]=pid;

}
void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf,cmdline);
    bg=parseline(buf,argv);

    if(argv[0]==NULL)
        return;

    if(!builtin_command(argv))
    {
        if((pid=Fork())==0) // child runs user
        {
            if(execve(argv[0],argv,environ)<0)
            {
                printf("%s: Command not found.\n",argv[0]);
                //pause();
                exit(0);
            }

        }//end fork if
        //Parent wait for foreground job to terminate
        if(!bg)
        {
            int status;
            if(waitpid(pid,&status,0)<0)
            {
                unix_error("waitfg:waitpid error");
            }
            else
            {
                if(WIFSIGNALED(status))
                {
                    fprintf(stderr,"Job %d terminated by signal: ",pid);
                    psignal(status,NULL);
                }
            }//end else
        }//end if bg
        else
        {
            //back ground
            addjob(pid,cmdline);
            printf("PID:%d-%s\n",pid,cmdline);
        }

    }//end if builin

    return;


}
int parseline(char *buf,char **argv)
{
    char *delim;
    int argc;
    int bg;
    buf[strlen(buf)-1]=' ';
    while(*buf&&(*buf==' ')) //ignore leading spaces
        buf++;
    // build argv list
    argc=0;
    while((delim=strchr(buf,' ')))
    {
        argv[argc++]=buf;
        *delim='\0';
        buf=delim+1;
        while(*buf&&(*buf==' '))//ignore spaces
            buf++;
    }
    argv[argc]=NULL;
    if(argc==0)
    {
        return 1;
    }

    if((bg=(*argv[argc-1]=='&'))!=0)
    {

        argv[--argc]=NULL;
    }
    return bg;
}
int builtin_command(char **argv)
{
    if(!strcmp(argv[0],"ctrl+c")){

         kill((0-getpid()),SIGINT);
         //kill(0,SIGINT);
         return 1;
     }
     if(!strcmp(argv[0],"ctrl+z")){

         kill((0-getpid()),SIGTSTP);
         return 1;
     }

    if(!strcmp(argv[0],"quit"))
        exit(0);
    if(!strcmp(argv[0],"&"))
        return 1;
    if(!strcmp(argv[0],"jobs"))
    {
        int i=0;
        int status;

        if(i<=jobs)
        {
            while(i<=jobs)
            {

                printf("[%d] %d stop %s\n",i,listid[i],bgjob[i]);
                i++;
            }

        }
        else
        {
            printf("No such background list to print\n");
        }
        return 1;
    }//end jobs


    if(!strcmp(argv[0],"bg"))
    {
        if(argv[1]==NULL){
            printf("cmd types: bg [JID/PID]\n");
            return 1;
        }
       int i=parseJob(argv[1]);
       if(i<0){
        return 1;
       }

      indexx=i;
      kill(getpid(),SIGCONT);


        return 1;
    }//end bg
    if(!strcmp(argv[0],"fg"))
    {
        if(argv[1]==NULL){
            printf("cmd types: fg [JID/PID]\n");
            return 1;
        }
        int i= parseJob(argv[1]);
        if(i<0){
        return 1;
        }
    int n=strlen(bgjob[i]);
    (bgjob[i])[n-2]='\0';

     indexx=i;
    kill(getpid(),SIGCONT);



    return 1;
    }//end fg


    return 0;

}
int parseJob(char *job_n){
     char *c;
        c=job_n;
        int i;
        //int inde;
        if(c[0]=='%')
        {
            char *job_num;
            job_num=c+1;


            if(!(isdigit(*job_num)))
            {
                printf("Job ID must digit\n");
                return 0;
            }

            i=atoi(job_num);
            if(i>jobs)
            {
                printf("No such Job ID in list\n");
                return -1;
            }

        }//end if
        else
        {
            //not %

            if(!(isdigit(*c)))
            {
                printf("Process must be digit \n");
                return 1;
            }
            i=atoi(c);

            int flag=0;
            int inde;
            for( inde=0;inde<=jobs;inde++){
                if(listid[inde]==i){
                    flag=1;
                    break;
                }
            }
            if(!flag){
                printf("No such that process\n");
                return -1;
            }
            i=inde;
        }//end else

    return i;

}
void handlerCon(){
  sigset_t mask,prev_mask;
  sigfillset(&mask);
  sigprocmask(SIG_BLOCK,&mask,&prev_mask);
  eval(bgjob[indexx]);
  deleteJob(indexx);
  sigprocmask(SIG_SETMASK,&prev_mask,NULL);
  return;
}


int main()
{
    char cmdline[MAXLINE];
    signal(SIGCONT,handlerCon);
    while(1)
    {
        printf("> ");
        fgets(cmdline,MAXLINE,stdin);
        if(feof(stdin))
        {
            exit(0);
        }//end if


        eval(cmdline);


    }//end while

}
