#OBJS=$(patsubst %.c,%.o,$(wildcard *.c)) 
OBJS=aes128_com.o aes128dec.o aes128enc.o common.o euclid_gcb.o GF_2sup8.o s_box.o
TARGET=test
AESLIB=libaes128.a
AR=ar
GCC=gcc -Wl 

all:main.o $(AESLIB) 
	$(GCC) -o $(TARGET) $?

#all:$(OBJS)
#	$(GCC) -o $(TARGET) $(OBJS)

lib:$(AESLIB)

$(AESLIB):$(OBJS)
	$(AR) rs $@ $?


.PHONY:
clean:
	-rm -rf $(OBJS) $(TARGET) $(AESLIB) main.o
