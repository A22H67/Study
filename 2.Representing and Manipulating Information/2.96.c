/*

*/
#include <stdio.h>
typedef unsigned float_bits;
float_bits float_f2i(float_bits f){
    unsigned exp=(f>>23) & 0xff ;
    unsigned frac=f&0x7fffff;
    unsigned s=f>>31;
    unsigned bias=127;
    if(exp==0xff && frac!=0){//NAN
        return 0x80000000;
    }

    exp=exp-127;
    if(exp>32){//overflow
        return 0x80000000;
    }else if(exp>23){
        exp=23;
    }
    unsigned k=23-exp;

    frac=frac >> k;
    return (1<<(exp))|frac;
}
int main()
{
    unsigned x=0x4A564504;
    printf("0x%x convert ",x);
    printf("0x%x\n",float_f2i(x));

    x=0x4640E400;
    printf("0x%x convert ",x);
    printf("0x%x\n",float_f2i(x));

}
