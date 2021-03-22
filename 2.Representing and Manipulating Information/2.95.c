/*
Following the bit-level floating-point coding rules, implement the function with
the following prototype:
 Compute 0.5*f. If f is NaN, then return f. 
float_bits float_half(float_bits f);
For floating-point number f , this function computes 0.5 . f . If f is NaN, your
function should simply return f .
*/
#include <stdio.h>
typedef unsigned float_bits;
float_bits float_half(float_bits f){
    unsigned exp=(f>>23) & 0xff ;
    unsigned frac=f&0x7ffff;
    unsigned s=f>>31;
    if(exp==0xFF && frac!=0) //check NAN
    {
        return f;
    }
    if(exp|=0){
        exp=exp-1;//normalize
    }else{
        frac=frac >> 1;//denormalize
    }
    return (s<<31)|(exp<<23)|frac;

}
int main()
{
    unsigned x=0x3F800000;
    unsigned y=float_twice(x);
    printf("x    =0x%x\n",x);
    printf("y=x*2=0x%x\n",y);
    unsigned z=float_half(y);
    printf("z=y/2=0x%x\n",z);
}
