/*

 Simple example of scans  directories in a UNIX file system

*/

#include <dirent.h>
#include <errno.h>
#include<stdio.h>
#include <sys/stat.h>
#include <string.h>
void read_d(char *filename){
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(filename)) == NULL) return;
    else
    {
        printf("contents of %s:\n",filename);
        while((entry=readdir(dir)) !=NULL )
        {

            struct stat st;
            stat(entry->d_name,&st);
            char *file=entry->d_name;
            //skip if filename is current or parent directory(prevent loop)
            if(strcmp(file,".")==0) continue; 
            if(strcmp(file,"..")==0) continue;
            
            printf("%s:%d bytes  \n\n",file,st.st_size);
            read_d(entry->d_name);
        }//end while

    }//end else
    printf("end content of %s\n\n",filename);
    closedir(dir);

}
main(int argc,char *argv[])
{
   read_d("/home/tr/Desktop");

}
