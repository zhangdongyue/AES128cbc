OBJS=$(patsubst %.c,%.o,$(wildcard *.c)) 
TARGET=test
GCC=gcc -Wl 

all:$(OBJS)
	$(GCC) -o $(TARGET) $(OBJS)

.PHONY:
clean:
	-rm -rf $(OBJS) $(TARGET)
