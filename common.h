#ifndef __COMMON_AES_H_
#define __COMMON_AES_H_ 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define ROTATE_LEFT(x, s, n)  ((x) << (n))|((x) >> ((s) - (n)))
#define ROTATE_RIGHT(x, s, n) ((x) >> (n))|((x) << ((s) - (n)))


#define SET_BIT(s,off)\
{\
    int b=1;\
    b<<=off;\
    *s|=b;\
}

typedef  unsigned char byte_t;  
typedef  unsigned int  word_t; 
typedef  unsigned char byte16x_t[16][16];

union A{
    int16_t m;
    char n[2];  
};

void swap(byte_t * a,byte_t * b);
int  is_big_endian();
void reverse4x(byte_t * box);

void PKCS5Padding(byte_t * buf,int len,byte_t * paddingBuf);
void PKCS7Padding(byte_t * buf,int len,int keylen,byte_t * paddingBuf);
#endif



