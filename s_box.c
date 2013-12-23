#include"aes128.h"
//step 1
void sbox_init()
{
	int8_t i,j;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++)
			SBOX[i][j]=i<<4|j;
}
//step 2
int sbox_inverse_gf28()
{
	int inverse_s8=-1;
	int i,j,tmp;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++){
			tmp=SBOX[i][j];
			if(tmp==0)SBOX[i][j]=0;
			else{
				euclid_gcb_GF2sup8_ext(IRR_POLY16,SBOX[i][j],&inverse_s8);	
				if(inverse_s8!=-1)
					SBOX[i][j]=inverse_s8;
				else
					return -1;
			}

		}
	return 0;
}
//step 3
void sbox_bit_column_vector()
{
	return ;
}

