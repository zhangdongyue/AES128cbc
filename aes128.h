#ifndef __AES_128_CBC_H_
#define __AES_128_CBC_H_

#include<stdio.h>
#include<stdlib.h>

#define IRR_POLY 0x1b
#define ZERO (int8_t)0

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

int SBOX[16][16];

/*GF2^8*/
int8_t GF2sup8_xor(int8_t a,int8_t b);
int8_t GF2sup8_reduce(int8_t a,int8_t b);
int8_t GF2sup8_division(int8_t a,int8_t b);
int8_t GF2sup8_mul(int8_t a,int8_t b);

/*GCB*/
int euclid_gcb(int a,int b);
int euclid_gcb_ext(int a,int b);
int8_t euclid_gcb_GF2sup8_ext(int8_t mx,int8_t bx);

/*S-Box*/
void sbox_init();
void sbox_inverse_gf28();
void sbox_bit_column_vector();

#endif



