/*Fill in code for the following C functions. Function srl performs a logical right
shift using an arithmetic right shift (given by value xsra), followed by other oper-
ations not including right shifts or division. Function sra performs an arithmetic
right shift using a logical right shift (given by value xsrl), followed by other
operations not including right shifts or division. You may use the computation
8*sizeof(int) to determine w, the number of bits in data type int. The shift
amount k can range from 0 to w − 1.
unsigned srl(unsigned x, int k) {
// Perform shift arithmetically 
unsigned xsra = (int) x >> k;
.
.
.
}
int sra(int x, int k) {
// Perform shift logically 
int xsrl = (unsigned) x >> k;
.
.
.
}
*/
 #include<stdio.h>
unsigned srl(unsigned x,int k){
    unsigned size_int=8*sizeof(int);//Perform shift logic
    unsigned xrsa=(int) x>>k;
    unsigned mask=(1<<(size_int-k))-1;
    xrsa=xrsa&mask;
    return xrsa;
}
int sra(int x,int k){

    int xrsl=(unsigned) x >> k;//Perform shift arithmetically
    int size_int=8*sizeof(int);
    unsigned mask=(1<<(size_int-k))-1;
    xrsl=xrsl^(~mask);
    return xrsl;

}
int main(){
 unsigned b=0x80000000;
 printf("0x%x\n",sra(b,28));//right shift arthimetic
 printf("0x%x",srl(b,28));//right shift logic

}
