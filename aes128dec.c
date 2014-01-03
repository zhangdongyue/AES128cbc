#include"aes128_com.h"
/*Shift one row right*/
static int state_shift_row_right(byte_t * state,int n)
{
	if(n>3 || n<0 || !state) 
		return -1;
	if(n==0) return 0;
	word_t * swt = (word_t*)state;
	word_t sw=*swt;
	if(is_big_endian()){
		*swt=ROTATE_RIGHT(sw,32,n*8); 
	}else{
		*swt=ROTATE_LEFT(sw,32,n*8);
	}		

	return 0;
}


/*Substitute bytes and shift rows*/
static int istate_rshift_bvary(byte_t * state)
{
	int i,j,ret;

	for(i=0;i<4;i++)
	{
		ret = state_shift_row_right(&state[i*4],i);//shift rows
		if(ret < 0)
			return ret;
		for(j=0;j<4;j++)
			subbyte(&state[i*4+j],isbox);//substitute bytes
	}
	return 0;		
}

/*Invert mix columns*/
static void istate_mix_columns(byte_t * state)
{
	int i,j,ret;
	byte_t tr[4];
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			tr[j]=GF2sup8_mul8(IMixC[j][0],state[0*4+i])^
				GF2sup8_mul8(IMixC[j][1],state[1*4+i])^
				GF2sup8_mul8(IMixC[j][2],state[2*4+i])^
				GF2sup8_mul8(IMixC[j][3],state[3*4+i]);
		}
		state[0*4+i]=tr[0];
		state[1*4+i]=tr[1];
		state[2*4+i]=tr[2];
		state[3*4+i]=tr[3];
	}
}

/*AES128 decryption*/
int Aes128_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText)
{
	if(!cipherText || cilen<=0 || !key || keylen!=16 || !plainText)
		return -1;	
	int i=0,ret=0;
	if(cilen<16)
		return -1;
	state_put(cipherText,plainText);
	key_expansion(key,W);
	i=10;
	state_add_rou_key(plainText,W+i*4);
	i--;
	while(i>0){
		if(istate_rshift_bvary(plainText)<0)
			return -1;
		if(state_add_rou_key(plainText,W+i*4)<0)
			return -1;
		istate_mix_columns(plainText);
		i--;	
	}
	if(istate_rshift_bvary(plainText)<0)
		return -1;
	if(state_add_rou_key(plainText,W+i*4)<0)
		return -1;
	return 0;
}

int Aes128cbc_Pkcs7_Dec(byte_t * cipherText,int cilen,byte_t * key,int keylen,byte_t * plainText,int * ptlen,const byte_t * iv)
{
	if(!cipherText || cilen<=0 || !key || keylen!=16 || !plainText)
		return -1;	
	int i=0,ret=0;
	byte_t inPadBuf[16];
	state_put(cipherText,plainText);

	key_expansion(key,W);
	i=10;
	state_add_rou_key(plainText,W+i*4);
	i--;
	while(i>0){
		if(istate_rshift_bvary(plainText)<0)
			return -1;
		if(state_add_rou_key(plainText,W+i*4)<0)
			return -1;
		istate_mix_columns(plainText);
		i--;	
	}
	if(istate_rshift_bvary(plainText)<0)
		return -1;
	if(state_add_rou_key(plainText,W+i*4)<0)
		return -1;

	int j;
	for(i=0;i<4;i++) 
		for(j=0;j<4;j++)
			plainText[i*4+j]=plainText[i*4+j]^iv[j*4+i];
	
	reverse4x(plainText);
	//depkcs7
	PKCS7_UnPad(plainText,16,16,ptlen);
	return 0;
}
