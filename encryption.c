#include"aes128.h"

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

/*Number 0x1f,Big endian:[01 0f]; Little endian:[0f 01]*/
int is_big_endian()
{
    union A k;
    k.n[0]=0;
    k.n[1]=1;
    return (k.m==1)?1:0;
}

void subbyte(byte_t * b,byte_t * s)
{
	int si,sj;
	byte_t bv=*b;
	si=(bv&0xf0)>>4;
	sj=bv&0x0f;
	*b=s[si*16+sj];
}

void subword(word_t * w,byte_t * s)
{
	byte_t * cw=(byte_t *)w;
	int i,si,sj;
	for(i=0;i<4;i++)
	{
		si=(cw[i]&0xf0)>>4;
		sj=cw[i]&0x0f;	
		cw[i]=s[si*16+sj];
	}
}

void key_expansion(byte_t * key,word_t * w)
{
	word_t  temp;
	int i,j;
	for(i=0;i<4;i++)
	{
		w[i]=key[4*i]<<24 | 
			 key[4*i+1]<<16 |
			 key[4*i+2]<<8  |
			 key[4*i+3];
	}
	for(i=4;i<44;i++)
	{
		temp=w[i-1];
		if(i%4==0)
		{
			temp=ROTATE_LEFT(temp,32,8);//rotword
			subword(&temp,sbox);
			temp=GF2sup8_add(temp,RC[i/4-1]<<24);
		}	
		w[i]=GF2sup8_add(w[i-4],temp);
	}
}

static void state_init(byte_t * state)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			state[4*i+j]=0xff;
		}
}

void state_put(char * input,byte_t * state)
{
	int i,j;
	char * p=input;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			state[j*4+i]=*p;
			p++;	
		}
}


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

/*Add round key*/
int state_add_rou_key(byte_t * state,word_t* key)
{
	if(!state || !key) 
		return -1;
	int i;
	for(i=0;i<4;i++)
	{
		state[i]=GF2sup8_add(state[i],ROTATE_LEFT(key[i],32,8)&0xff);
	}
	for(i=0;i<4;i++)
	{
		state[4+i]=GF2sup8_add(state[4+i],ROTATE_LEFT(key[i],32,16)&0x00ff);
	}
	for(i=0;i<4;i++)
	{
		state[2*4+i]=GF2sup8_add(state[2*4+i],ROTATE_RIGHT(key[i],32,8)&0xff);
	}
	for(i=0;i<4;i++)
	{
		state[3*4+i]=GF2sup8_add(state[3*4+i],key[i]&0xff);
	}
	return 0;
}

static void PKCS5Padding(byte_t * buf,int len,byte_t * paddingBuf)
{
	int i,j;
	int p=8-len%8;
	for(i=0;i<len;i++)
		paddingBuf[i]=buf[i];
	for(j=len;j<16;j++)
	{
		if(j<8)
			paddingBuf[i+p]=8-len%8;
		else
			paddingBuf[i+p]=8;
	}
}

static void PKCS7Padding(byte_t * buf,int len,int keylen,byte_t * paddingBuf)
{
	int i,j;
	int p=keylen-len%8;
	for(i=0;i<len;i++)
		paddingBuf[i]=buf[i];

	for(j=len;j<keylen;j++)
		paddingBuf[j]=p?p:keylen;

}
//ZeroPadding
/*AES128 encryption*/
int Aes128_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output)
{
	if(!input || inlen<=0 || !key || keylen!=16 || !output)
		return -1;	
	int i=0,ret=0;
	byte_t inPadBuf[16];
	if(inlen<16){
		PKCS5Padding(input,inlen,inPadBuf);
		state_put(inPadBuf,output);
	}else{
		state_put(input,output);
	}	

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

int Aes128cbc_Pkcs7_Enc(byte_t * input,int inlen,byte_t * key,int keylen,byte_t * output,const byte_t * iv)
{
	if(!input || inlen<=0 || !key || keylen!=16 || !output)
		return -1;	
	int i=0,ret=0;
	byte_t inPadBuf[16];
	if(inlen<16){
		PKCS7Padding(input,inlen,16,inPadBuf);
		for(i=0;i<16;i++) inPadBuf[i]^=iv[i];
	}else{
		for(i=0;i<16;i++) inPadBuf[i]=input[i]^iv[i];
	}	
	state_put(inPadBuf,output);

	key_expansion(key,W);
	i=0;
	state_add_rou_key(output,W+i*4);
	printf("the %d round\n",i);
	i++;
	while(i<10){
		printf("the %d round\n",i);
		if(state_bvary_lshift(output)<0)
			return -1;
		state_mix_columns(output);
		state_add_rou_key(output,W+i*4);
		i++;	
	}
	printf("the %d round\n",i);
	if(state_bvary_lshift(output)<0)
		return -1;
	state_add_rou_key(output,W+i*4);
	return 0;
}
