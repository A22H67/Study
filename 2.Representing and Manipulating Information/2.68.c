/*

* Mask with least signficant n bits set to 1
* Examples: n = 6 --> 0x3F, n = 17 --> 0x1FFFF
* Assume 1 <= n <= w

int lower_one_mask(int n);
Your function should follow the bit-level integer coding rules (page 164). Be
careful of the case n = w.
*/
#include <stdio.h>
int lower_one_mask(int n){
    return (1<<n)-1;
}
int main(){
    unsigned x=0x12345678;
    printf("0x%x",lower_one_mask(17));
}
