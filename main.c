#include"aes128.h"

void sboxPrint()
{
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++)
			printf("0x%02x ",SBOX[i][j]);
		putchar('\n');
		putchar('\n');
	}
}
int main(void)
{
		//10000011 83
#if 0
        int a=0x80,b=0x83;
        int ret = GF2sup8_mul8(a,b);
        printf("ret = %02x\n",ret);

		a=0x83,b=0x1d;
		int mod=0;
		ret = GF2sup8_divid(a,b,&mod);
		printf("ret= %02x,mod= %02x\n",ret,mod);

		a=0x2,b=0x83;
		ret=GF2sup8_red(IRR_POLY16,GF2sup8_mul16(a,b));
		printf("ret= %02x\n",ret);

		a=0xd3;
		int inverse=0;
		euclid_gcb_GF2sup8_ext(IRR_POLY16,a,&inverse);
		printf("inverse of 0x%x=0x%x\n",a,inverse);
#endif
		sbox_init();
		sbox_inverse_gf28();
		sbox_bit_column_vector();
		sboxPrint();
        return 0;
}
