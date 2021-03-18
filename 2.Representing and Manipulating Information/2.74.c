/*
Write a function with the following prototype:
 Determine whether arguments can be subtracted without overflow 
int tsub_ok(int x, int y);
This function should return 1 if the computation x-y does not overflow.
*/
#include <stdio.h>
#include <limits.h> // chứa INT_MIN,INT_MAX

int tsub_ok(int x,int y){
    int result=x-y;

   unsigned sign_x=(unsigned)x>>31;
   unsigned sign_y=(unsigned)y>>31;
   unsigned sign_result=(unsigned)result>>31;

   int nega_over= (sign_x==1)&&(sign_y==0)&&(sign_result==0);//x<0 y>0 result >0
   int posi_over= (sign_x==0)&&(sign_y==1)&&(sign_result==1); //x>0 y<0
   return !(nega_over||posi_over);

}
int main(){
    int x=INT_MAX;
    int y=-5;
    if(tsub_ok(x,y)){
        printf("DOESN'T OVERFLOW");
    }else{
        printf("OVERFLOW");
    }
}
