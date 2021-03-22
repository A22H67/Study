/*
Following the bit-level floating-point coding rules, implement the function with
the following prototype:
 Compute |f|. If f is NaN, then return f. 
float_bits float_absval(float_bits f);
For floating-point number f , this function computes |f |. If f is NaN, your
function should simply return f .
Test your function by evaluating it for all 2 32 values of argument f and com-
paring the result to what would be obtained using your machine’s floating-point
operations.
*/
typedef unsigned float_bits;

float_bits float_absval(float_bits f){
    unsigned exp=(f>>23)&0xff;
    unsigned frac=f&0x7fffff;

    if(exp==0xff && frac!=0){ //check NAN
        return f;
    }
    return (exp<<23)|frac;

}
int main(){
    unsigned x=0xFF7FFFFF;
    printf("0x%x\n",float_absval(x));
}
