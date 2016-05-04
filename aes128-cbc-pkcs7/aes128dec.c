#include"aes128dec.h"

/*add round key,wo,wk:(word_t *),i:(int)*/
#define iAddRoundKey(wo,wk) \
{\
    (wo)[0]^=(wk)[0];\
    (wo)[1]^=(wk)[1];\
    (wo)[2]^=(wk)[2];\
    (wo)[3]^=(wk)[3];\
}

/*shift rows & subbytes*/
#define iShiftRows_SubBytes(s,k)\
{\
		(s)[0]=isbox[(s)[0]];\
		(s)[4]=isbox[(s)[4]];\
		(s)[8]=isbox[(s)[8]];\
		(s)[12]=isbox[(s)[12]];\
		k=isbox[(s)[13]];\
		(s)[13]=isbox[(s)[9]];\
		(s)[9]=isbox[(s)[5]];\
		(s)[5]=isbox[(s)[1]];\
		(s)[1]=k;\
		k=isbox[(s)[2]];\
		(s)[2]=isbox[(s)[10]];\
		(s)[10]=k;\
		k=isbox[(s)[6]];\
		(s)[6]=isbox[(s)[14]];\
		(s)[14]=k;\
		k=isbox[(s)[3]];\
		(s)[3]  =isbox[(s)[7]];\
		(s)[7]=isbox[(s)[11]];\
		(s)[11]=isbox[(s)[15]];\
		(s)[15]=k;\
}

/*SubWords*/
#define subword(w,s)\
{\
	(w)[0]=(s)[(w)[0]];\
	(w)[1]=(s)[(w)[1]];\
	(w)[2]=(s)[(w)[2]];\
	(w)[3]=(s)[(w)[3]];\
}

/*Invert mix columns*/
#define iRouKey_MixCols(s,wk,i)\
	for(i=0;i<4;i++){						\
		(s)->w[i]^=(wk)[i];					\
		(s)->w[i]=ism0[(s)->b[i*4]].w ^		\
		ism1[(s)->b[i*4+1]].w ^	\
		ism2[(s)->b[i*4+2]].w ^	\
		ism3[(s)->b[i*4+3]].w; 	\
	}

/*Unpadding pkcs7*/
void PKCS7_UnPad(byte_t * buf,int buflen,int blocksize,int * orilen)
{
	int i,j=0;
	int pn=buf[blocksize-1];
	if(pn<16 && pn!=0){
		for(i=16-pn;i<16;i++){
			if(buf[i]!=pn)
				break;
			j++;
		}
	}
	*orilen = (j==pn)?16-pn:16;
}

/*Key Expansion wordx4x11*/
static void key_expansion(const bw4 * key,word_t * w)
{
	bw  temp;
	int i,j;

	for(i=0;i<4;i++)
		w[i]=key->w[i];

	for(i=4;i<44;i++)
	{
		temp.w=w[i-1];
		if(i%4==0)
		{
			temp.w=ROTATE_RIGHT(temp.w,32,8);//rotword
			subword(temp.b,sbox);
			temp.w=temp.w^(RC[i/4-1]);
		}
		w[i]=w[i-4]^temp.w;
	}
}

/*AES128 decryption*/
int Aes128_Dec(bw4 * cipherText,int cilen,word_t * wkey,bw4 * plainText)
{
	if(!cipherText || cilen<=0 || !wkey || !plainText)
		return -1;

	int round=10,i,j;
	byte_t * bw,k;

	if(cilen<16)
		return -1;

	for(i=0;i<4;i++)
		plainText->w[i]=cipherText->w[i];

	//Add round key
	iAddRoundKey(plainText->w,wkey+round*4);

	round--;

	while(round>0){
		//Shift rows & SubBytes
		iShiftRows_SubBytes(plainText->b,k);

		//Add round key & Mix cloumns
		iRouKey_MixCols(plainText,wkey+round*4,i);

		round--;
	}

	//Shift rows & subBytes
	iShiftRows_SubBytes(plainText->b,k);

	//Add round key
	iAddRoundKey(plainText->w,wkey+round*4);

	return 0;
}

int Aes128cbc_Dec(bw4 * state,int cilen,word_t * wkey,const bw4 * iv)
{
	if(!state || cilen!=0 || !wkey || !iv)
		return -1;

	int round=10,i,j;
	byte_t * bw,k;


	//Add round key
	iAddRoundKey(state->w,wkey+round*4);

	round--;

	while(round>0){
		//Shift rows & SubBytes
		iShiftRows_SubBytes(state->b,k);

		//Add round add & Mix cloumns
		iRouKey_MixCols(state,wkey+round*4,i);

		round--;
	}

	//Add round key & IV inverse xor
	state->w[0]^=(wkey+round*4)[0]^iv->w[0];
	state->w[1]^=(wkey+round*4)[1]^iv->w[1];
	state->w[2]^=(wkey+round*4)[2]^iv->w[2];
	state->w[3]^=(wkey+round*4)[3]^iv->w[3];

	return 0;
}

int Aes128cbc_Pkcs7_Dec(bw4 * cipherText,int cilen,word_t * wkey,bw4 * plainText,int * ptlen,const bw4 * iv)
{
	if(!cipherText || cilen<=0 || !wkey || !plainText)
		return -1;

	int round=10,i;
	byte_t k;

	for(i=0;i<4;i++)
		plainText->w[i]=cipherText->w[i];

	//add round key
	iAddRoundKey(plainText->w,wkey+round*4);

	round--;

	while(round>0){
		//shift rows & subBytes
		iShiftRows_SubBytes(plainText->b,k);

		//add round key & mix columns
		iRouKey_MixCols(plainText,wkey+round*4,i);

		round--;
	}

	//shift rows & subbytes
	iShiftRows_SubBytes(plainText->b,k);

	//Add round key & IV inverse xor
	plainText->w[0]^=(wkey+round*4)[0]^iv->w[0];
	plainText->w[1]^=(wkey+round*4)[1]^iv->w[1];
	plainText->w[2]^=(wkey+round*4)[2]^iv->w[2];
	plainText->w[3]^=(wkey+round*4)[3]^iv->w[3];

	//Unpadding depkcs7
	PKCS7_UnPad(plainText->b,16,16,ptlen);
	return 0;
}

#if 0
void gen_sm()
{
	int i;
	bw sm0[256];
	bw sm1[256];
	bw sm2[256];
	bw sm3[256];
	for(i=0;i<0x100;i++)
	{
		sm0[i].b[0]=gf28_xe[i];
		sm0[i].b[1]=gf28_x9[i];
		sm0[i].b[2]=gf28_xd[i];
		sm0[i].b[3]=gf28_xb[i];


		sm1[i].b[0]=gf28_xb[i];
		sm1[i].b[1]=gf28_xe[i];
		sm1[i].b[2]=gf28_x9[i];
		sm1[i].b[3]=gf28_xd[i];

		sm2[i].b[0]=gf28_xd[i];
		sm2[i].b[1]=gf28_xb[i];
		sm2[i].b[2]=gf28_xe[i];
		sm2[i].b[3]=gf28_x9[i];

		sm3[i].b[0]=gf28_x9[i];
		sm3[i].b[1]=gf28_xd[i];
		sm3[i].b[2]=gf28_xb[i];
		sm3[i].b[3]=gf28_xe[i];
	}


#if 1
	for(i=0;i<0x100;i++)
	{
		printf("0x%08x,",sm0[i].w);
		if((i+1)%8==0)
			putchar('\n');
	}
	putchar('\n');
#endif
}
#endif
