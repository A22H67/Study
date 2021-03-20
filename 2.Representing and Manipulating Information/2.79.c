/*
Write code for a function mul3div4 that, for integer argument x, computes 3 ∗
x/4 but follows the bit-level integer coding rules (page 164). Your code should
replicate the fact that the computation 3*x can cause overflow
*/
#include<stdio.h>
int mul3div4(int x){
  //x*3/4
  int bias=3;
   int size=sizeof(int)*8;
  int sign_x=(unsigned)x>>(size-1);
  x=((x<<1)+x) ; //x*3
  (sign_x==1)&&(x=x+bias);
  return x>>2;

}
int main(){
    printf("%d\n",mul3div4(-5));
}
