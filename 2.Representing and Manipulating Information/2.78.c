
/*
Write code for a function with the following prototype:
 Divide by power of 2. Assume 0 <= k < w-1 
int divide_power2(int x, int k);
The function should compute x/2 k with correct rounding, and it should follow
the bit-level integer coding rules (page 164).
*/
#include<stdio.h>
int divide_power2(int x,int k){
    int bias=(1<<k)-1;
    int size=sizeof(int)*8;

    int sign_x=(unsigned)x>>(size-1);

    (sign_x==1)&&(x=x+bias);
    return x>>k;
}
int main(){

    printf("%d\n",divide_power2(12340,8));
    printf("%d\n",divide_power2(-12340,8));

}
