#ifndef __AES128_ENC_H__
#define __AES128_ENC_H__
#include"common.h"
#include"aes128_com.h"

int Aes128_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output);
int Aes128cbc_Pkcs7_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output,const byte_t * iv);

#endif

