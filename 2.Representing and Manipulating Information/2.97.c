/*
Following the bit-level floating-point coding rules, implement the function with
the following prototype:
 Compute (float) i 
float_bits float_i2f(int i);
For argument i, this function computes the bit-level representation of
(float) i.
Test your function by evaluating it for all 2 32 values of argument f and com-
paring the result to what would be obtained using your machine’s floating-point
operations.
*/
#include <stdio.h>
typedef unsigned float_bits;
float_bits float_i2f(int i)
{
    int k,a;
    int s=(unsigned)i>>31;
    unsigned exp;
    unsigned frac;
    if(i==0){

        return 0;
    }

    if (i == INT_MIN) {
        exp = 158;
        frac = 0;
        return s << 31 | exp << 23 | frac;
    }
    if(s){
        i=~i+1;
    }

    for(k=0;k<32;k++){
        a=(unsigned)i>>k;
        if(a==1){
            break;
        }
    }//end for
     exp=k+127;
     frac=(1<<k)^i;//drop leading bit 1


    if(k>23){
        frac=frac >> (k-23);
    }
    else{
        frac=frac <<(23-k);
    }

    return (s<<31)|(exp<<23)|frac;
}
int main()
{
    int a=0x3039;//12345
    printf("int x:0x%x convert float",a);
    printf(" 0x%x\n",float_i2f(a));

     a=0xffffcfc7;//-12345
    printf("int x:0x%x convert float",a);
    printf(" 0x%x\n",float_i2f(a));

}
