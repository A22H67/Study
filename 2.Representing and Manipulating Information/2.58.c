/*
Write a procedure is_little_endian that will return 1 when compiled and run
on a little-endian machine, and will return 0 when compiled and run on a big-
endian machine. This program should run on any machine, regardless of its word
size.
0x1234567
Little endian: 67 45 23 01
Big    endian: 01 23 45 67
*/

#include<stdio.h>
typedef unsigned char *byte_pointer;

int compare_bytes(byte_pointer byte,size_t len,int x){
    for(int i=0;i<len;i++){
    int a = x>>((i)*8)  &0xff;//get byte from least to most significant bytes
    if(a != byte[i]){
        return 0;
    }
    
    }//end for

    return 1;
}

int main(){
int x=0x1234567;
int result=compare_bytes((byte_pointer)&x,sizeof(x),x);
if(result==1){
    printf("Little endian");
}
else{
    printf("Big endian");
}

}
