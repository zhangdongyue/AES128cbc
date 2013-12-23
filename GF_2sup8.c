#include"aes128.h"
int8_t bit_vector[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

static void set_bit(int * s,int off)
{
	int b=1;
	b<<=off;
	*s|=b;	
}

static int move_left_by(const int s,const int b)
{
    if(s>b || s<=0)
        return -1; 
    int i,si=-1,bi=-1;
    for(i=0;i<8;i++)
    {   
        if(s&bit_vector[i] && si==-1)
            si=i;
        if(b&bit_vector[i] && bi==-1)
            bi=i;

        if(si>0&&bi>0)
            break;
    }   
    int m_n=si-bi;
    return m_n;
}

/*a+b*/
int GF2sup8_add(int a,int b)
{
        return a^b;
}

/*a-b*/
int GF2sup8_red(int a,int b)
{
	return GF2sup8_add(a,b);
}

/*axb*/
int GF2sup8_mul(int a,int b)
{
        int BX[8];
        int last;
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
        int n=1;
        int ret=0;
        while(n){
                if(n&b) ret^=BX[i];
                i++;
                n<<=1;
        }

        return ret;
}


/*a/b ,return divide number,
a:dividend,b:divisor,mod:remainder*/
int GF2sup8_divid(int a,int b,int * mod)
{
	int ret = 0;
	int c=0;
	while(a>b){
		c=move_left_by(b,a);
		if(c<0) break;
		set_bit(&ret,c);
		a=(b<<c)^a;
	}
	*mod=a;
	return ret;
}

/*a%b,return remainder,
a:dividend,b:divisor*/
int GF2sup8_mod(int a,int b)
{
	int c=0;
	while(a>b)
	{
		c=move_left_by(b,a);
		if(c<0) break;
		a=(b<<c)^a;
	}
	return a;
}
