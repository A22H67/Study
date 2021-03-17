/*
Write code for the function with the following prototype:

* Return 1 when x can be represented as an n-bit, 2’s-complement
* number; 0 otherwise
* Assume 1 <= n <= w

int fits_bits(int x, int n);
*/
#include <stdio.h>
int fits_bit(int x,int n){
   int a=( (unsigned)x >> (n-1) ) >> 1;
    return (a==0);


}
int main(){
    int x=-1;
    int n=31;

   if(fits_bit(x,n)){
    printf("FITS_BIT");
   }
   else{
    printf("DOESN'T_FITS");
   }
}
