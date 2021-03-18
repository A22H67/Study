/*
Write code for a function with the following prototype:
 Addition that saturates to TMin or TMax 
int saturating_add(int x, int y);
Instead of overflowing the way normal two’s-complement addition does, sat-
urating addition returns TMax when there would be positive overflow, and TMin
when there would be negative overflow. Saturating arithmetic is commonly used
in programs that perform digital signal processing.
*/

#include<stdio.h>
#include <limits.h> // chứa INT_MAX INT_
int saturating_add(int x,int y){
   int result=x+y;

   unsigned sign_x=(unsigned)x>>31;
   unsigned sign_y=(unsigned)y>>31;
   unsigned sign_result=(unsigned)result>>31;

   (sign_x==0)&&(sign_y==0)&&(sign_result==1)&&(result=INT_MAX);//x>0 y>0 result<0 result=MAX
   (sign_x==1)&&(sign_y==1)&&(sign_result==0)&&(result=INT_MIN);//x<0 y<0 result>0 result=MIN

    return result;

}
int main(){
    int x=INT_MIN;
    int y=-5;
    printf("%d",saturating_add(x,y));
}
