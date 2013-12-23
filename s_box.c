#include"aes128.h"
int euclid_gcb(int a,int b)
{
	int A,B,R;

	if(a<0) a=-a;
	if(b<0) b=-b;

	if(a>b) A=a,B=b;
	else    A=b,B=a;

	while(B){
		R=A%B;
		A=B;
		B=R;
	}

	return A;
}

int euclid_gcb_ext(int a,int b)
{
	int A,B;
	int A1,A2,A3,B1,B2,B3;

	if(a>b)A=a,B=b;
	else   A=b,B=a;

	A1=1,A2=0,A3=A;
	B1=0,B2=1,B3=B;
	int Q,T1,T2,T3;
	while(B3!=1 && B3!=0){
		Q=A3/B3;
		T1=A1-Q*B1;
		T2=A2-Q*B2;
		T3=A3-Q*B3;
		A1=B1,A2=B2,A3=B3;
		B1=T1,B2=T2,B3=T3;
	}

	if(B3==0) return A3;
	if(B3==1) return B2;
}

int8_t euclid_gcb_GF2sup8_ext(int8_t mx,int8_t bx)
{
	int8_t a1x=1,a2x=0,a3x=mx;
	int8_t b1x=0,b2x=1,b3x=bx;
	int8_t qx;

	while(b3x!=0 && b3x!=1)
	{

	}
}

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

