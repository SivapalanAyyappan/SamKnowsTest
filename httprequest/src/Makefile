CC=gcc
CFLAGS= -Wall -g -O -fPIC -I ../../include/
RM= rm -f
.PHONY: all clean

all: libHttpStats.so
clean:
	$(RM) *.o *.so

libHttpStats.so: HttpLib.o
	$(LINK.c) -shared $^ -o $@ -lcurl

HttpLib.o: HttpLib.c 
