/*
Write a function int_shifts_are_arithmetic() that yields 1 when run on a
machine that uses arithmetic right shifts for data type int and yields 0 otherwise.
Your code should work on a machine with any word size. Test your code on several
machines. 

*/
int int_shifts_are(int x){
    int shift_val=((sizeof(int)) <<3)-1;
    x=(x >> shift_val) & 0x80000000; // lấy bit most
    return  !!x;

}
int main(){
 int x=0x80000000;
 if(int_shifts_are(x)){
    printf("Arthimetic right shifts");
  }
  else{
    printf("Logical right shifts");
  }
}
