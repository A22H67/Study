/*
Suppose we want to compute the complete 2w-bit representation of x . y, where
both x and y are unsigned, on a machine for which data type unsigned is w bits.
The low-order w bits of the product can be computed with the expression x*y, so
we only require a procedure with prototype
unsigned unsigned_high_prod(unsigned x, unsigned y);
that computes the high-order w bits of x . y for unsigned variables.
We have access to a library function with prototype
int signed_high_prod(int x, int y);
that computes the high-order w bits of x . y for the case where x and y are in two’s-
complement form. Write code calling this procedure to implement the function
for unsigned arguments. Justify the correctness of your solution.
Hint: Look at the relationship between the signed product x . y and the un-
signed product x  . y  in the derivation of Equation 2.18.
*/
#include<stdio.h>
#include <stdint.h>

int signed_high_prod(int x,int y)
{
    int64_t x_64=x;
    int64_t y_64=y;
    return (x_64*y_64)>>32;
}
unsigned unsigned_high_prod(unsigned x,unsigned y){
    int high_pro_int=signed_high_prod(x,y);
    
    unsigned w=sizeof(unsigned)*8;
    unsigned sign_x= x>>(w-1);
    unsigned sign_y=y>>(w-1);

    return high_pro_int + (sign_x*y+sign_y*x)  ;
    //equation 2.18 (x*y + (sign_x*y+sign_y*x)*2^w+ sign_x*sign_y*(1<<(2w)))mod 2^w
    //                                        

}
int main() 
{        
    int x=0x54789632;
    int y=0x7875ABCD;
    uint64_t x_64=x;
    uint64_t y_64=y;
    printf("32:0x%x\n",unsigned_high_prod(x,y));
    printf("64:0x%lx\n",(x_64*y_64));
}
