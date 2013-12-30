#ifndef __EUCLID_GCB_H_ 
#define __EUCLID_GCB_H_
#include<stdio.h>
#include"GF_2sup8.h"

int euclid_gcb(int a,int b); 
int euclid_gcb_ext(int a,int b); 
int euclid_gcb_GF2sup8_ext(int mx,int bx,int * inverse);

#endif
