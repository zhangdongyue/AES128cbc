#include"aes128.h"

static int bit_add_8(int8_t bx)
{
	int i=1;
	int ret=bx&i;
	while(i!=0x80) {	
		i<<=1;
		if(bx&i)
			ret=GF2sup8_add(ret,1);
		else
			ret=GF2sup8_add(ret,0);

	}
	return ret;
}

//step 1
static void sbox_init(byte_t * s)
{
	int8_t i,j;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++)
			s[i*16+j]=i<<4|j;
}

//step 2
static int sbox_inverse_gf28(byte_t * s)
{
	int inverse_s8=-1;
	int i,j,tmp;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++){
			tmp=s[i*16+j];
			if(tmp==0)s[i*16+j]=0;
			else{
				euclid_gcb_GF2sup8_ext(IRR_POLY16,s[i*16+j],&inverse_s8);	
				if(inverse_s8!=-1)
					s[i*16+j]=inverse_s8;
				else
					return -1;
			}

		}
	return 0;
}

//step 3 sbox
static void sbox_bit_column_vector(byte_t * s)
{
	int i,j,k;
	int vary,sxy;
	int bx=0,bt=0;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++){
			bx=0;
			sxy = s[i*16+j];
			for(k=0;k<8;k++) {
				bt=bit_add_8(sxy&vary_matrix[k]);
				if(bt)
					SET_BIT(&bx,k);	
			}
			vary=GF2sup8_add(bx,VARY_C);
			s[i*16+j]=vary;
		}
}

//step3 inverse_sbox
static void isbox_bit_column_vector(byte_t * s)
{
	int i,j,k;
	int vary,sxy;
	int bx=0,bt=0;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++){
			bx=0;
			sxy = s[i*16+j];
			for(k=0;k<8;k++) {
				bt=bit_add_8(sxy&ivary_matrix[k]);
				if(bt)
					SET_BIT(&bx,k);	
			}
			vary=GF2sup8_add(bx,IVARY_C);
			s[i*16+j]=vary;
		}
} 

void gen_sbox(byte_t * s)
{
	sbox_init(s);
	sbox_inverse_gf28(s);	
	sbox_bit_column_vector(s);
}

void gen_isbox(byte_t * is)
{
	sbox_init(is);
	isbox_bit_column_vector(is);
	sbox_inverse_gf28(is);	
}


