/*
Suppose we number the bytes in a w-bit word from 0 (least significant) to w/8 − 1
(most significant). Write code for the following C function, which will return an
unsigned value in which byte i of argument x has been replaced by byte b:
unsigned replace_byte (unsigned x, int i, unsigned char b);
Here are some examples showing how the function should work:
replace_byte(0x12345678, 2, 0xAB) --> 0x12AB5678
replace_byte(0x12345678, 0, 0xAB) --> 0x123456AB
*/

unsigned replace_byte(unsigned x,int i, unsigned char b){
    unsigned shift_bit = i*8;
    unsigned mask_byte=(1 << shift_bit)-1; // mask for add_byte
    unsigned add_byte= (0xab <<shift_bit) + (x&mask_byte);

    mask_byte=(mask_byte << 8) +0xff;//mask for x
    x=x&(~mask_byte);
    x+=add_byte;
    return x;
}

int main(){
   unsigned x=0x12345678;
   unsigned y=replace_byte(x,0,0xab);
   printf("0/0x%x\n",y);
    y=replace_byte(x,1,0xab);
   printf("1/0x%x\n",y);
    y=replace_byte(x,2,0xab);
   printf("2/0x%x\n",y);
    y=replace_byte(x,3,0xab);
   printf("3/0x%x\n",y);
}

