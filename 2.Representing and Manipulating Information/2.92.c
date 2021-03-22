/*
Following the bit-level floating-point coding rules, implement the function with
the following prototype:
 Compute -f. If f is NaN, then return f. 
float_bits float_negate(float_bits f);
For floating-point number f , this function computes −f . If f is NaN, your
function should simply return f
*/

typedef unsigned float_bits;

float_bits float_negate(float_bits f){
    unsigned exp=(f>>23)&0xff;
    unsigned frac=f&0x7fffff;
    if(exp==0xff && frac!=0){ //check NAN
        return f;
    }
    return f^0x80000000;
}

int main(){
    unsigned x=0x7F900000;
    printf("0x%x\n",float_negate(x));
}
