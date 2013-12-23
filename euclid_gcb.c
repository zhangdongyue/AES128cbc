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

/*return GCB(mx,bx),inverse=inverse(bx)mod(mx)*/
int euclid_gcb_GF2sup8_ext(int mx,int bx,int * inverse) 
{
    int a1x=1,a2x=0,a3x=mx;
    int b1x=0,b2x=1,b3x=bx;
	int t1x=0,t2x=0,t3x=0;
    int qx; 

    while(b3x!=0 && b3x!=1)
    {   
		qx=GF2sup8_divid(a3x,b3x,NULL);
		t1x=GF2sup8_red(a1x,GF2sup8_mul16(qx,b1x));	
		t2x=GF2sup8_red(a2x,GF2sup8_mul16(qx,b2x));
		t3x=GF2sup8_red(a3x,GF2sup8_mul16(qx,b3x));

		a1x=b1x,a2x=b2x,a3x=b3x;
		b1x=t1x,b2x=t2x,b3x=t3x;
    }   

	/*
	b3x=1:b3x=(mx,bx),b2x=inverse(bx)mod(mx)
	b3x=0:a3x=gcb(mx,bx),no inverse
	*/
	if(b3x){
		*inverse=b2x;
		return b3x;
	}else{
		*inverse=-1;
		return a3x;
	}
}

