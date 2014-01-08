#include"aes128enc.h"
#include"aes128dec.h"

byte_t SBOX[16*16];
static const byte_t IV[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
void sboxPrint(byte_t * s)
{
	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++)
			printf("0x%02x, ",s[i*16+j]);
		putchar('\n');
		putchar('\n');
	}
}
void print4x(byte_t * box)
{
	int i;
	for(i=0;i<16;i++)
	{
		//if(i%4==0) putchar('\n');
		printf("%02x",box[i]);
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
#endif
#if 0
	//test2
	gen_sbox(SBOX);	
	sboxPrint(SBOX);
#endif
#if 1
	//test4
	int i;
	//byte_t  input[16]={0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
	byte_t  input[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
	byte_t  k[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
	//3925841d02dc09fbdc118597196a0b32
	byte_t * S=calloc(16+1,sizeof(byte_t));
	//print4x(input);
	Aes128cbc_Pkcs7_Enc(input,sizeof(input),k,16,S,IV);
	//Aes128_Enc(input,16,k,16,S);
	if(S){
		//reverse4x(S);
		print4x(S);
	}
	byte_t * plain=calloc(16+1,sizeof(byte_t));
	//Aes128_Dec(S,16,k,16,plain);
	int ptlen=0;
	Aes128cbc_Pkcs7_Dec(S,16,k,16,plain,&ptlen,IV);
	print4x(plain);
	printf("ptlen=%d\n",ptlen);
#endif
	return 0;
}
