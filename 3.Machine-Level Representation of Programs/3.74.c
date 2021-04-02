/*
Write a function in assembly code that matches the behavior of the function find_
range in Figure 3.51. Your code should contain only one floating-point comparison
instruction, and then it should use conditional moves to generate the correct result.
You might want to make use of the instruction cmovp (move if even parity).
*/
#include <stdio.h>
#include <math.h>

int find_range_cmov(float x){
    //x in xmm0
    asm
    (
    "xorl %eax,%eax;" //nega
    "movl $2, %ebx;"//pos
    "mov $3, %ecx;"//nan
    "movl $1, %edx;"//zero
    "vxorps %xmm1, %xmm1, %xmm1;" // set xmm1 to 0
    "ucomiss %xmm1,%xmm0;" //compare x vs 0
    "cmova %rbx,%rax;"//pos
    "cmove %rdx,%rax;"//zero
    "cmovp %rcx,%rax;"//nan
    "pop %rbp;"
    "ret;"
    );
}

int main(){
    float x=NAN;
    printf("%d",find_range_cmov(x));
    //0 neg 1 zero 2 pos 3 other
}
