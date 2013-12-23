#ifndef __AES_128_CBC_H_
#define __AES_128_CBC_H_

#include<stdio.h>
#include<stdlib.h>

/* The irreducible polynomial:x^8+x^4+x^3+x+1
   bit type:00011011*/
#define IRR_POLY 0x1b
#define ZERO (BINT)0

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define BINT int8_t 

int SBOX[16][16];

/*GF2^8*/
BINT GF2sup8_add(BINT a,BINT b);//+
BINT GF2sup8_reduce(BINT a,BINT b);//-
BINT GF2sup8_mul(BINT a,BINT b);//x
BINT GF2sup8_divid(BINT a,BINT b,BINT * mod);// /
BINT GF2sup8_mod(BINT a,BINT b);//%   

/*GCB*/
int euclid_gcb(int a,int b);
int euclid_gcb_ext(int a,int b);
BINT euclid_gcb_GF2sup8_ext(BINT mx,BINT bx);

/*S-Box*/
void sbox_init();
void sbox_inverse_gf28();
void sbox_bit_column_vector();

#endif



