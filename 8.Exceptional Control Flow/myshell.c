#include "csapp.h"
#define MAXARGS 128

char *bgjob[MAXARGS];
int  listid[MAXARGS];
int jobs=-1;
volatile int indexx;

void deleteJob(int index){
    free(bgjob[index]);
    for(index;index+1<=jobs;index++){
        bgjob[index]=bgjob[index+1];
        listid[index]=listid[index+1];
    }
    jobs--;
  //  bgjob[jobs]=NULL;
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
          //  kill(pid,SIGSTOP);
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

                waitpid(listid[i],&status,WNOHANG);
                if(WIFEXITED(status)) //=========================================Here===========================================================================================================
                {
                    printf("[%d] %d stop %s\n",i,listid[i],bgjob[i]);
                }
                else
                {
                    printf("[%d] %d running %s\n",i,listid[i],bgjob[i]);

                }//=============================================================Here=======================================================================


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
                //printf("S:%d\n",listid[inde]);
                if(listid[inde]==i){
                    //printf("PID:%d",listid[inde]);
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
  //  printf("PID:%d\n",listid[indexx]);
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
    // jobs ctrl+c ctrl+z bg fg
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
