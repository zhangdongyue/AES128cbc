#ifndef _GF_2SUB8_H_
#define _GF_2SUB8_H_
#include"common.h"

//The irreducible polynomial:x^8+x^4+x^3+x+1 bit type:00011011
#define IRR_POLY8  0x001B
#define IRR_POLY16 0x011B

static int16_t bit_vector[16]=
{
    0x8000,0x4000,0x2000,0x1000,
    0x0800,0x0400,0x0200,0x0100,
    0x0080,0x0040,0x0020,0x0010,
    0x0008,0x0004,0x0002,0x0001
};

//a+b
#define GF2sup8_add(a,b) ((a)^(b))
//a-b
#define GF2sup8_red(a,b) ((a)^(b))

int GF2sup8_mul8(const int a,const int b);//x
int GF2sup8_mul16(const int a,const int b);//x
int GF2sup8_divid(const int a,const int b,int * mod);// /
int GF2sup8_mod(const int a,const int b);//%  

#endif
