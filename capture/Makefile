point_velocity.so : point_velocity.c plugin_header.h
	gcc -Wall -c -fpic -ggdb point_velocity.c
	gcc -shared -Wl,-soname,point_velocity.so -o point_velocity.so point_velocity.o

clean :
	rm *.so
	rm *.o
	rm *.h
