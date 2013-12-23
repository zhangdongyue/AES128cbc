#ifndef __AES_128_CBC_H_
#define __AES_128_CBC_H_

#include<stdio.h>
#include<stdlib.h>

/* The irreducible polynomial:x^8+x^4+x^3+x+1
   bit type:00011011*/
#define IRR_POLY 0x1b
#define ZERO (int)0

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

int SBOX[16][16];

/*GF2^8*/
int GF2sup8_add(int a,int b);//+
int GF2sup8_reduce(int a,int b);//-
int GF2sup8_mul(int a,int b);//x
int GF2sup8_divid(int a,int b,int * mod);// /
int GF2sup8_mod(int a,int b);//%   

/*GCB*/
int euclid_gcb(int a,int b);
int euclid_gcb_ext(int a,int b);
int euclid_gcb_GF2sup8_ext(int mx,int bx);

/*S-Box*/
void sbox_init();
void sbox_inverse_gf28();
void sbox_bit_column_vector();

#endif



