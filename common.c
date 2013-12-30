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

void PKCS7Padding(byte_t * buf,int len,int keylen,byte_t * paddingBuf)
{
    int i,j;
    int p=keylen-len%keylen;
    for(i=0;i<len;i++)
        paddingBuf[i]=buf[i];

    for(j=len;j<keylen;j++)
        paddingBuf[j]=p?p:keylen;

}
