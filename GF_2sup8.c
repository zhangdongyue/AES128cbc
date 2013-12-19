#include<stdio.h>
#include<stdlib.h>
//The irreducible polynomial:x(sup8)+x(sup4)+x(sup3)+x+1,00011011
#define IRR_POLY 0x1b
#define ZERO (int8_t)0
int8_t GF2sup8_xor(int8_t a,int8_t b)
{
        return a^b;
}
int8_t GF2sup8_mul(int8_t a,int8_t b)
{
        int8_t BX[8];
        int8_t last;
        BX[0]=a;
        int i;
        for(i=1;i<8;i++){
                if(BX[i-1] & 0x80){
                        BX[i]=BX[i-1]<<1;
                        BX[i]^=IRR_POLY;
                }else
                        BX[i]=BX[i-1]<<1;
        }
        i=0;
        int8_t n=1;
        int8_t ret=0;
        while(n){
                if(n&b) ret^=BX[i];
                i++;
                n<<=1;
        }

        return ret;
}

int main(void)
{
        int8_t a=0x57,b=0x83;
        int8_t ret = GF2sup8_mul(a,b);
        printf("ret = %02x\n",ret);
        return 0;
}
