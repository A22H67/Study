/*
Write C expressions that evaluate to 1 when the following conditions are true and
to 0 when they are false. Assume x is of type int.
A. Any bit of x equals 1.
B. Any bit of x equals 0.
C. Any bit in the least significant byte of x equals 1.
D. Any bit in the most significant byte of x equals 0.
*/
#include<stdio.h>

int A(int x){
    return x && 1;
}

int B(int x){
    // trường hợp bit không có 0 là 0xFFFFFFFF khi nghịch đảo bit của nó lại ta được 0
    return !(~x==0);
}

int C(int x){
    return (x &0xff) && 1;
}

int D(int x){
    
    int shift_val=(sizeof(int)-1)<<3;
    int x_most=x >> shift_val;//lấy bit most
    return!(~x_most==0);
}
