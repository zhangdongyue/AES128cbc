#include"aes128.h"
int8_t bit_vector[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

static void set_bit(BINT * s,BINT off)
{
	BINT b=1;
	b<<=off;
	*s|=b;	
}

static int move_left_by(BINT * s,const BINT b)
{
	BINT S=*s,B=b;
	if(S>B)
		return -1;
	int i,si=0,bi=0;
	for(i=0;i<8;i++)
	{
		if(S&bit_vector[i])
			si=i;
		if(B&bit_vector[i])
			bi=i;

		if(si&&bi)
			break;
	}
	int m_n=bi-si;
	*s=S<<m_n;
	return m_n;
}

/*a+b*/
BINT GF2sup8_add(BINT a,BINT b)
{
        return a^b;
}
/*a-b*/
BINT GF2sup8_red(BINT a,BINT b)
{
	return GF2sup8_add(a,b);
}
/*axb*/
BINT GF2sup8_mul(BINT a,BINT b)
{
        BINT BX[8];
        BINT last;
        BX[0]=a;
        int i;
        for(i=1;i<8;i++){
                if(BX[i-1] & 0x80){
                        BX[i]=BX[i-1]<<1;
                        BX[i]^=IRR_POLY;
                }else
                        BX[i]=BX[i-1]<<1;
        }
        i=0;
        BINT n=1;
        BINT ret=0;
        while(n){
                if(n&b) ret^=BX[i];
                i++;
                n<<=1;
        }

        return ret;
}


/*a/b ,return divide number,
a:dividend,b:divisor,mod:remainder*/
BINT GF2sup8_divid(BINT a,BINT b,BINT * mod)
{
	BINT ret = 0;
	int c=0;
	while(a>b){
		c=move_left_by(&b,a);
		set_bit(&ret,c);
		a=b^a;
	}
	*mod=a;
	return ret;
}

/*a%b,return remainder,
a:dividend,b:divisor*/
BINT GF2sup8_mod(BINT a,BINT b)
{
	while(a>b)
	{
		move_left_by(&b,a);
		a=b^a;
	}
	return a;
}
