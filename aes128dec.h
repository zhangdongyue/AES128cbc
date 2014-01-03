#ifndef __AES128_DEC_H__
#define __AES128_DEC_H__
#include"common.h"
#include"aes128_com.h"

/*decryption*/
int Aes128_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText);
int Aes128cbc_Pkcs7_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText,int * ptlen,const byte_t * iv);

#endif
