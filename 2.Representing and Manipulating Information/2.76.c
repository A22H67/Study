/*
The library function calloc has the following declaration:

void *calloc(size_t nmemb, size_t size);
According to the library documentation, “The calloc function allocates memory
for an array of nmemb elements of size bytes each. The memory is set to zero. If
nmemb or size is zero, then calloc returns NULL.”

Write an implementation of calloc that performs the allocation by a call to
malloc and sets the memory to zero via memset. Your code should not have any
vulnerabilities due to arithmetic overflow, and it should work correctly regardless
of the number of bits used to represent data of type size_t.
As a reference, functions malloc and memset have the following declarations:

void *malloc(size_t size);
void *memset(void *s, int c, size_t n);
*/
#include <stdio.h>
void *my_calloc(size_t nmemb,size_t size){

    uint64_t require_size=nmemb*size;
    size_t request_size=(size_t) require_size;
    if(request_size!=require_size){
        return NULL;
    }
    void *mem=malloc(request_size);
    memset(mem,0,request_size);
    return mem;
}
int main() 
{       
  int *a=my_calloc(8,sizeof(int));
  for(int i=0;i<8;i++){
    printf("a[%d]:%x\n",i,a[i]);
  }
}
