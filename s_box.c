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
//step 3
void sbox_bit_column_vector()
{
	int i,j,k;
	int vary,sxy;
	int bx=0,bt=0;
	for(i=0;i<16;i++)
		for(j=0;j<16;j++){
			bx=0;
			sxy = SBOX[i][j];
			for(k=0;k<8;k++) {
				bt=bit_add_8(sxy&vary_matrix[k]);
				if(bt)
					set_bit(&bx,k);	
			}
			vary=GF2sup8_add(bx,VARY_C);
			SBOX[i][j]=vary;
		}
	return ;
}

