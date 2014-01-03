#include"common.h"
/*Number 0x1f,Big endian:[01 0f]; Little endian:[0f 01]*/
int is_big_endian()
{
    union A k;
    k.n[0]=0;
    k.n[1]=1;
    return (k.m==1)?1:0;
}

void swap(byte_t * a,byte_t * b)
{
    *a=*a-*b;
    *b=*a+*b;
    *a=*b-*a;
}

void reverse4x(byte_t * box)
{
    int i,j;
    for(i=0;i<3;i++)
        for(j=i+1;j<4;j++)
            swap(&box[4*i+j],&box[4*j+i]);
}

#if 0
void PKCS5Padding(byte_t * buf,int len,byte_t * paddingBuf)
{
    int i,j;
    int p=8-len%8;
    for(i=0;i<len;i++)
        paddingBuf[i]=buf[i];
    for(j=len;j<16;j++)
    {
        if(j<8)
            paddingBuf[i+p]=8-len%8;
        else
            paddingBuf[i+p]=8;
    }
}
#endif

void PKCS7_Pad(byte_t * buf,int buflen,int blocksize,byte_t * paddingBuf)
{
    int i;
    int p=blocksize-buflen%blocksize;
    for(i=0;i<buflen;i++)
        paddingBuf[i]=buf[i];

    for(i=buflen;i<blocksize;i++)
        paddingBuf[i]=p;

}

void PKCS7_UnPad(byte_t * buf,int buflen,int blocksize,int * orilen)
{
    int i,j=0;
    int pn=buf[blocksize-1];
    if(pn<16 && pn!=0){
        for(i=16-pn;i<16;i++){
            if(buf[i]!=pn)
                break;
            j++;
        }   
    }   
    *orilen = (j==pn)?16-pn:16;	
}
