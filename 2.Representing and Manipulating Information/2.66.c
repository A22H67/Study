/*
Write code to implement the following function:

* Generate mask indicating leftmost 1 in x. Assume w=32.
* For example, 0xFF00 -> 0x8000, and 0x6600 --> 0x4000.
* If x = 0, then return 0.

int leftmost_one(unsigned x);
Your function should follow the bit-level integer coding rules (page 164),
except that you may assume that data type int has w = 32 bits.
Your code should contain a total of at most 15 arithmetic, bitwise, and logical
operations.
Hint: First transform x into a bit vector of the form [0 . . . 011 . . . 1].
*/
#include <stdio.h>
int leftmost_one(unsigned x){
    unsigned k;
    unsigned y=0;
    unsigned a;
    unsigned shift_val=0;
    ((a=x&0xFFFF0000)!=0)&&(x=a); //2     check 16 bit
    ((a=x&0xFF00FF00)!=0)&&(x=a);//4      check 8 bit
    ((a=x&0xF0F0F0F0)!=0)&&(x=a);//6      check 4 bit
    ((a=x&0x88888888)!=0)&&(x=a);//8      check 1 bit 1000
    ((a=x&0x44444444)!=0)&&(x=a); //10    check 1 bit 0100
    ((a=x&0x22222222)!=0)&&(x=a);//12     check 1 bit 0010
     ((a=x&0x11111111)!=0)&&(x=a);//14    check 1 bit 0001
    return x;
}
int main(){
    unsigned x=0x6600;
    printf("\nMost bit:0x%x",leftmost_one(x));
    //printf("0x%x",(0xffff<<16) );
}
