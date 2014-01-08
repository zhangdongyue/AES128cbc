#include"aes128_com.h"

/*Shift one row left*/
static int state_shift_row_left(byte_t * state,int n)
{
	if(n>3 || n<0 || !state) 
		return -1;
	if(n==0) return 0;
	word_t * swt = (word_t*)state;
	word_t sw=*swt;
	if(is_big_endian()){
		*swt=ROTATE_LEFT(sw,32,n*8); 
	}else{
		*swt=ROTATE_RIGHT(sw,32,n*8);
	}		

	return 0;
}


/*Substitute bytes and shift rows*/
static int state_bvary_lshift(byte_t * state)
{
	int i,j,ret;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			subbyte(&state[i*4+j],sbox);//substitute bytes
		}
		ret = state_shift_row_left(&state[i*4],i);//shift rows
		if(ret < 0)
			return ret;
	}
	return 0;		
}

/*Mix columns*/
static void state_mix_columns(byte_t * state)
{
	int i,j,ret;
	byte_t tr[4];
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			tr[j]=GF2sup8_mul8(MixC[j][0],state[0*4+i])^
				GF2sup8_mul8(MixC[j][1],state[1*4+i])^
				GF2sup8_mul8(MixC[j][2],state[2*4+i])^
				GF2sup8_mul8(MixC[j][3],state[3*4+i]);
		}
		state[0*4+i]=tr[0];
		state[1*4+i]=tr[1];
		state[2*4+i]=tr[2];
		state[3*4+i]=tr[3];
	}
}

/*AES128 encryption*/
int Aes128_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output)
{
	if(!input || inlen!=16 || !key || keylen!=16 || !output)
		return -1;	
	int i=0,ret=0;
	state_put(input,output);

	key_expansion(key,W);
	i=0;
	state_add_rou_key(output,W+i*4);
	i++;
	while(i<10){
		if(state_bvary_lshift(output)<0)
			return -1;
		state_mix_columns(output);
		if(state_add_rou_key(output,W+i*4)<0)
			return -1;
		i++;	
	}
	if(state_bvary_lshift(output)<0)
		return -1;
	if(state_add_rou_key(output,W+i*4)<0)
		return -1;
	return 0;
}

int Aes128cbc_Enc(byte_t * state,int slen,byte_t * key,int keylen,const byte_t * iv)
{
	if(!state || slen!=16 || !key || keylen!=16 )
		return -1;	
	int i=0;
	reverse4x(state);
	key_expansion(key,W);
	i=0;
	state_add_rou_key(state,W+i*4);
	i++;
	while(i<10){
		if(state_bvary_lshift(state)<0)
			return -1;
		state_mix_columns(state);
		state_add_rou_key(state,W+i*4);
		i++;	
	}
	if(state_bvary_lshift(state)<0)
		return -1;
	state_add_rou_key(state,W+i*4);
	reverse4x(state);
	return 0;
}


int Aes128cbc_Pkcs7_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output,const byte_t * iv)
{
	if(!input || inlen<=0 || !key || keylen!=16 || !output)
		return -1;	
	int i=0,ret=0;
	if(inlen<16){
		PKCS7_Pad(input,inlen,16,output);
		for(i=0;i<16;i++) output[i]^=iv[i];
	}else{
		for(i=0;i<16;i++) output[i]=input[i]^iv[i];
	}	
	reverse4x(output);
	key_expansion(key,W);
	i=0;
	state_add_rou_key(output,W+i*4);
	i++;
	while(i<10){
		if(state_bvary_lshift(output)<0)
			return -1;
		state_mix_columns(output);
		state_add_rou_key(output,W+i*4);
		i++;	
	}
	if(state_bvary_lshift(output)<0)
		return -1;
	state_add_rou_key(output,W+i*4);
	reverse4x(output);
	return 0;
}

