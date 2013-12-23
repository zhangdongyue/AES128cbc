#include"aes128.h"
void sbox_init()
{
	int8_t i,j;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++)
			SBOX[i][j]=i<<4|j;
}

void sbox_inverse_gf28()
{
	return ;
}

void sbox_bit_column_vector()
{
	return ;
}

