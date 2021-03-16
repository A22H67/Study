/*
Write code to implement the following function:
 Return 1 when x contains an odd number of 1s; 0 otherwise.
Assume w=32 
int odd_ones(unsigned x);
Your function should follow the bit-level integer coding rules (page 164),
except that you may assume that data type int has w = 32 bits.
Your code should contain a total of at most 12 arithmetic, bitwise, and logical
operations.
*/
#include <stdio.h>
int odd_ones(unsigned x){
 unsigned x_left,x_right;
 
 x_left=x>>16;
 x=x^x_left;//16 vs 16

 x_left=x>>8;
 x=x^x_left; //8 vs 8

 x_left=x>>4;
 x=x^x_left; //4 vs 4

 x_left=x>>2;
 x=x^x_left; //2 vs 2

 x_left=x>>1;
 x=x^x_left; //1 vs 1

 return x&0x1;
}
int main(){
    if(odd_ones(0xFFFFFFFA)){
        printf("ODD_ONES");
    }else{
        printf("EVEN_ONES");
    }
}
