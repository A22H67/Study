/*Write code to implement the following function:
Return 1 when any odd bit of x equals 1; 0 otherwise.
Assume w=32 
int any_odd_one(unsigned x);
Your function should follow the bit-level integer coding rules (page 164),
except that you may assume that data type int has w = 32 bits.*/
#include <stdio.h>
int any_odd_one(unsigned x){
    int mask=0x55555555;
    int result=mask&x;
    return!!(result);
}
int main(){
    int x=any_odd_one(0xAAA);
 if(x==1){
    printf("Odd bit");
 }
 else{
    printf("Doesn't have odd bit");
 }
