/*
Write code for a function with the following prototype:

* Do rotating left shift. Assume 0 <= n < w
* Examples when x = 0x12345678 and w = 32:
*
n=4 -> 0x23456781, n=20 -> 0x67812345

unsigned rotate_left(unsigned x, int n);
Your function should follow the bit-level integer coding rules (page 164). Be
careful of the case n = 0.
*/

#include <stdio.h>
unsigned rotate_left(unsigned x,int n){
     x=(x<<n)| ((x>>(31-n))>>1 )  ;
    return x;
}
int main(){
    unsigned x=0x12345678;
    printf("0x%x",rotate_left(x,20));

}
