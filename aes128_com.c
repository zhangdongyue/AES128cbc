#include"aes128_com.h"
void subbyte(byte_t * b,byte_t * s)
{
    int si,sj;
    byte_t bv=*b;
    si=(bv&0xf0)>>4;
    sj=bv&0x0f;
    *b=s[si*16+sj];
}

void subword(word_t * w,byte_t * s)
{
    byte_t * cw=(byte_t *)w;
    int i,si,sj;
    for(i=0;i<4;i++)
    {   
        si=(cw[i]&0xf0)>>4;
        sj=cw[i]&0x0f;
        cw[i]=s[si*16+sj];
    }   
}

void key_expansion(byte_t * key,word_t * w)
{
    word_t  temp;
    int i,j;
    for(i=0;i<4;i++)
    {
        w[i]=key[4*i]<<24 |
             key[4*i+1]<<16 |
             key[4*i+2]<<8  |
             key[4*i+3];
    }
    for(i=4;i<44;i++)
    {
        temp=w[i-1];
        if(i%4==0)
        {
            temp=ROTATE_LEFT(temp,32,8);//rotword
            subword(&temp,sbox);
            temp=GF2sup8_add(temp,RC[i/4-1]<<24);
        }
        w[i]=GF2sup8_add(w[i-4],temp);
    }
}
void state_put(char * input,byte_t * state)
{
    int i,j;
    char * p=input;
    for(i=0;i<4;i++)
        for(j=0;j<4;j++){
            state[j*4+i]=*p;
            p++;
        }
}

/*Add round key*/
int state_add_rou_key(byte_t * state,word_t* key)
{
    if(!state || !key)
        return -1;
    int i;
    for(i=0;i<4;i++)
    {
        state[i]=GF2sup8_add(state[i],ROTATE_LEFT(key[i],32,8)&0xff);
    }
    for(i=0;i<4;i++)
    {
        state[4+i]=GF2sup8_add(state[4+i],ROTATE_LEFT(key[i],32,16)&0x00ff);
    }
    for(i=0;i<4;i++)
    {
        state[2*4+i]=GF2sup8_add(state[2*4+i],ROTATE_RIGHT(key[i],32,8)&0xff);
    }
    for(i=0;i<4;i++)
    {
        state[3*4+i]=GF2sup8_add(state[3*4+i],key[i]&0xff);
    }
    return 0;
}
