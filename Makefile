# Queue tail-head makefile
all: libqth.so libqth.a

clean:
	rm -f qthlib.o libqth.so libqth.a

libqth.so : qthlib.o
	gcc -o libqth.so qthlib.o -shared
	ar rcs libqth.a qthlib.o

qthlib.o : qthlib.c qthlib.h
	gcc -c -g -Wall qthlib.c -fpic
