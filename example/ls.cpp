/*
Like ls program in linux
Example of how to use opendir and readdir to scan directory
*/

#include <dirent.h>
#include <errno.h>
#include<stdio.h>
#include <sys/stat.h>
main(int argc,char *argv[])
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/home/tr/Desktop")) == NULL) perror("opendir() error");
    else
    {
        puts("contents of root:");
        while((entry=readdir(dir)) !=NULL )
        {
            struct stat st;
            stat(entry->d_name,&st);
            printf("%s:%d bytes \n\n",entry->d_name,st.st_size);
        }//end while

    }//end else

}
