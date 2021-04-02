/*
Write a function in assembly code that matches the behavior of the function find_
range in Figure 3.51. Your code should contain only one floating-point comparison
instruction, and then it should use conditional branches to generate the correct
result.
*/

#include <stdio.h>
#include <math.h>
int find_range_branches(float x){
    asm(
    "xorl %eax,%eax;" //nega
    "vxorps %xmm1, %xmm1, %xmm1;" // xmm1 0
    "ucomiss %xmm1,%xmm0;" //compare x vs 0
    "ja .POS ;"
    "jp .NAN;"
    "je .ZERO;"
    "pop %rbp;"
    "ret;"

    ".POS:;"
    "movl $2,%eax;"
    "pop %rbp;"
    "ret;"

     ".NAN:;"
    "movl $3,%eax;"
    "pop %rbp;"
    "ret;"

     ".ZERO:;"
    "movl $1,%eax;"
    "pop %rbp;"
    "ret;"

    );

}

int main(){
    float x=NAN;
    printf("%d",find_range_branches(x));
    //0 neg 1 zero 2 pos 3 other
}
