/*
Write a function int_shifts_are_arithmetic() that yields 1 when run on a
machine that uses arithmetic right shifts for data type int and yields 0 otherwise.
Your code should work on a machine with any word size. Test your code on several
machines. 

*/
int int_shifts_are(int x){
  // nếu là logical shift thì kết quả sẽ là 0 còn nếu là arthimetic shift thì kết quả sẽ là 0xFFFFFFFF 
    int shift_val=((sizeof(int)) <<3)-1;
    return (x >>shift_val) && 1;
}
int main(){
    int x=0x80000000;
  printf("%d",int_shifts_are(x));
}
