#include"aes128.h"

void sboxPrint()
{
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++)
			printf("0x%02x, ",SBOX[i][j]);
		putchar('\n');
		putchar('\n');
	}
}
void print4x(byte_t * box)
{
	int i;
	for(i=0;i<16;i++)
	{
		if(i%4==0) putchar('\n');
		printf("%x ",box[i]);
	}
	putchar('\n');
	putchar('\n');
}

int main(void)
{
	//10000011 83
#if 0
	//test1
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
	//test2
	sbox_init();
	sbox_inverse_gf28();
	sbox_bit_column_vector();
	sbox_inverse();
	sboxPrint();
	//test3
	byte_t key[16]={0xea,0xd2,0x73,0x21,0xb5,0x8d,0xba,0xd2,0x31,0x2b,0xf5,0x60,0x7f,0x8d,0x29,0x2f};
	word_t temp=0x7f8d292f;
	temp=ROTATE_LEFT(temp,32,8);//rotword
	subword(&temp);
	temp=GF2sup8_add(temp,0x1b<<24);
	temp=GF2sup8_add(temp,0xead27321);
	printf("temp =%X\n",temp);
#endif
	//test4
	int i;
	word_t  key[4]={0xac192857,0x77fad15c,0x66dc2900,0xf321416a};
	byte_t  state[16]={0x87,0xf2,0x4d,0x97,0xec,0x6e,0x4c,0x90,0x4a,0xc3,0x46,0xe7,0x8c,0xd8,0x95,0xa6};

	printf("[State origin]\n");
	print4x(state);

	for(i=0;i<4;i++)
		state_shift_row_left(&state[i*4],i);
	printf("[After shift rows]\n");
	print4x(state);

	state_mix_columns(state);
	printf("[After mix columns]\n");
	print4x(state);
	
	state_add_rou_key(state,key);
	printf("[After add round key]%x\n",0xbc^0x6a);
	print4x(state);

	return 0;
}
