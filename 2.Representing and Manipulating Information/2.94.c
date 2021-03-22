/*
Following the bit-level floating-point coding rules, implement the function with
the following prototype:
 Compute 2*f. If f is NaN, then return f. 
float_bits float_twice(float_bits f);
For floating-point number f , this function computes 2.0 . f . If f is NaN, your
function should simply return f .
Test your function by evaluating it for all 2 32 values of argument f and com-
paring the result to what would be obtained using your machine’s floating-point
operations.
*/

#include <stdio.h>
float_bits float_twice(float_bits f)
{
    unsigned exp=(f>>23) & 0xff ;
    unsigned frac=f&0x7ffff;
    unsigned s=f>>31;
    if(exp==0xFF && frac!=0) //check NAN
    {
        return f;
    }
    if(exp!=0)
    {
        exp=(exp+1);//normalize
    }
    else
    {
        frac=frac << 1;//denormalize
    }

    return (s<<31)|(exp<<23)|frac;
}
int main()
{
    unsigned x=0x3F800000;
    printf("x*2=0x%x\n",float_twice(x));
}
