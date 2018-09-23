all : qthtest qthtest_dyn

clean:
	rm -f qthtest qthtest_dyn

qthtest :  qthtest.c
	gcc -Wall -g -o qthtest qthtest.c ../libqth.a ../../memblock/libmb.a

qthtest_dyn: qthtest.c
	gcc -Wall -g -o qthtest_dyn qthtest.c ../libqth.so ../../memblock/libmb.a

memcheck : qthtest
	valgrind --leak-check=full ./qthtest

