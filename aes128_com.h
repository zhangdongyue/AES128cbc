#ifndef __AES_128_COM_H__
#define __AES_128_COM_H__
#include"common.h"
#include"s_box.h"
#include"GF_2sup8.h"
/*RC[0]=1,RC[j]=2xRC[j-1],x:GF2sup8_mul8*/
static const byte_t RC[10]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

/*Used to mix columns*/
static const byte_t MixC[4][4]=
{
  0x02,0x03,0x01,0x01,
  0x01,0x02,0x03,0x01,
  0x01,0x01,0x02,0x03,
  0x03,0x01,0x01,0x02
};

/*Used to invert mix columns*/
static const byte_t IMixC[4][4]=
{
  0x0e,0x0b,0x0d,0x09,
  0x09,0x0e,0x0b,0x0d,
  0x0d,0x09,0x0e,0x0b,
  0x0b,0x0d,0x09,0x0e
};

/*Expansion key by word_t*/
word_t W[11*4];//4x4x11 byte

/*common fuctions*/
void subword(word_t * w,byte_t * s); 
void subbyte(byte_t * b,byte_t * s); 
void key_expansion(byte_t * key,word_t * w); 
void state_put(char * input,byte_t * state);
int  state_add_rou_key(byte_t * state,word_t* key);


/*decryption*/
int Aes128_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText);
int Aes128cbc_Pkcs7_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText,byte_t * ptlen,const byte_t * iv);

#endif
