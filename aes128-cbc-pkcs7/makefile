#OBJS=$(patsubst %.c,%.o,$(wildcard *.c)) 
OBJS=aes128dec.o aes128enc.o
#TARGET=test
AESLIBA=libaes128.a 
AESLIBSO=libaes128.so
AR=ar -rs	
CC=gcc 
CFLAGS= -w -O2 -fPIC

all:$(AESLIBA) $(AESLIBSO) 

lib:$(AESLIB)

$(AESLIBA):$(OBJS)
	$(AR) $@ $?

$(AESLIBSO):$(OBJS)
	$(CC) -shared $? -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY:

clean:
	-rm -rf $(OBJS) $(AESLIBA) $(AESLIBSO) 
