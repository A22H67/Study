/*
Write C expressions to generate the bit patterns that follow, where a k represents
k repetitions of symbol a. Assume a w-bit data type. Your code may contain
references to parameters j and k, representing the values of j and k, but not a
parameter representing w.
A. 1(w−k) 0(k)
B. 0(w−k−j) 1(k) 0(j)
*/
#include<stdio.h>
int generate_A(int k){
    int w=sizeof(int)*8;
    int bit=(1<<(w-1-k)<<1)-1;
    return (bit<<k);
}
int generate_B(int k,int j){
     int w=sizeof(int)*8;
    int bit=((1<<k)-1)<<j;
    return bit;
}
int main(){
    unsigned a=0xFFFFFFFF>>4;
    printf("A:0x%x\n",generate_A(4));
    printf("B:0x%x\n",generate_B(4,16));
}
