#ifndef __AES_128_CBC_H_
#define __AES_128_CBC_H_

#include<stdio.h>
#include<stdlib.h>

/* The irreducible polynomial:x^8+x^4+x^3+x+1
   bit type:00011011*/
#define IRR_POLY8  0x001B
#define IRR_POLY16 0x011B
#define ZERO (int)0

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

int SBOX[16][16];

/*GF2^8*/
int GF2sup8_add(const int a,const int b);//+
int GF2sup8_red(const int a,const int b);//-
int GF2sup8_mul8(const int a,const int b);//x
int GF2sup8_mul16(const int a,const int b);//x
int GF2sup8_divid(const int a,const int b,int * mod);// /
int GF2sup8_mod(const int a,const int b);//%   

/*GCB*/
int euclid_gcb(int a,int b);
int euclid_gcb_ext(int a,int b);
int euclid_gcb_GF2sup8_ext(int mx,int bx,int * inverse);

/*S-Box*/
void sbox_init();
int sbox_inverse_gf28();
void sbox_bit_column_vector();

#endif



