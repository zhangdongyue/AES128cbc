#include"aes128.h"
static byte_t State[4*4];//4x4 byte
static byte_t Key[4*4];//4x4 byte
static word_t W[11*4];//4x4x11 byte

/*RC[0]=1,RC[j]=2xRC[j-1],x:GF2sup8_mul8*/
static RC[10]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

/*Number 0x1f,Big endian:[01 0f];
Little endian:[0f 01]*/
int is_big_endian()
{
    union A k;
    k.n[0]=0;
    k.n[1]=1;

    if(k.m==1)
        return 1;
    return 0;
}

void subbyte(byte_t * b)
{
	int si,sj;
	byte_t bv=*b;
	si=(bv&0xf0)>>4;
	sj=bv&0x0f;
	*b=sbox[si][sj];
}

void subword(word_t * w)
{
	byte_t * cw=(byte_t *)w;
	int i,si,sj;
	for(i=0;i<4;i++)
	{
		si=(cw[i]&0xf0)>>4;
		sj=cw[i]&0x0f;	
		cw[i]=sbox[si][sj];
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
			subword(&temp);
			temp=GF2sup8_add(temp,RC[i/4-1]<<24);
			temp=GF2sup8_add(temp,w[i-4]);
		}	
	}
}

void state_init(byte_t * state)
{
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			state[4*i+j]=0xff;
		}
}

int state_put(char * input,byte_t * state)
{
	int i,j;
	char * p=input;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			state[j*4+i]=*p;
			p++;	
		}
	return 0;
}

int state_line_move_left(byte_t * state,int n)
{
	if(n>3 || n<0 || !state) 
		return -1;
	word_t * swt = (word_t*)state;
	word_t sw=*swt;
	if(is_big_endian()){
		*swt=ROTATE_LEFT(sw,32,n*8); 
	}else{
		*swt=ROTATE_RIGHT(sw,32,n*8);
	}		

	return 0;
}
#if 0
static int state_line_move_left(byte_t * state,int n)
{
	if(n>3 || n<0 || !state) 
		return -1;

	switch(n){
		case 0:
			return 0;
			break;
		case 1:
			{
				byte_t tmp=state[0];
				state[0]=state[1];
				state[1]=state[2];
				state[2]=state[3];
				state[3]=tmp;	
			}
			break;
		case 2:
			{
				byte_t tmp=state[0];
				state[0]=state[2];
				state[2]=tmp;
				tmp=state[1];
				state[1]=state[3];
				state[3]=tmp;
			}
			break;
		case 3:
			{
				byte_t tmp=state[3];
				state[3]=state[2];
				state[2]=state[1];
				state[1]=state[0];
				state[0]=tmp;
			}
			break;
	}
	return 0;
}
#endif

int state_bvary_lmove(byte_t * state)
{
	int i,j,ret;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			subbyte(&state[i*4+j]);
		}
		ret = state_line_move_left(state,i);
		if(ret < 0)
			return ret;
	}
	return 0;		
}

