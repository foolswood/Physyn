all : physyn plugins

physyn : physyn.c capture.o loadmodel.o model.o out_channels.o physyn.o points.o ptrlist.o springs.o tempmodel.o vectors.o action_queue.o
	gcc -Wall -ggdb -rdynamic -lm -ldl `pkg-config --cflags --libs jack` -o physyn *.o

plugins : capture/point_velocity.so

capture.o capture.h : capture.c tempmodel.h ptrlist.h capture/plugin_header.h
	./headergen.py capture.c
	gcc -Wall -ggdb -c capture.c

loadconf.o loadconf.h : loadconf.c ptrlist.h vectors.h
	./headergen.py loadconf.c
	gcc -Wall -ggdb -c loadconf.c

loadmodel.o loadmodel.h : loadmodel.c ptrlist.h model.h tempmodel.h loadconf.h capture.h out_channels.h action_queue.h actions.h
	./headergen.py loadmodel.c
	gcc -Wall -ggdb -c loadmodel.c

model.o model.h : model.c points.h springs.h
	./headergen.py model.c
	gcc -Wall -ggdb -c model.c

out_channels.o out_channels.h : out_channels.c
	./headergen.py out_channels.c
	gcc -Wall -ggdb -c out_channels.c

points.o points.h : points.c vectors.h
	./headergen.py points.c
	gcc -Wall -ggdb -c points.c

ptrlist.o ptrlist.h : ptrlist.c
	./headergen.py ptrlist.c
	gcc -Wall -ggdb -c ptrlist.c

springs.o springs.h : springs.c vectors.h
	./headergen.py springs.c
	gcc -Wall -ggdb -c springs.c

tempmodel.o tempmodel.h : tempmodel.c vectors.h points.h springs.h model.h
	./headergen.py tempmodel.c
	gcc -Wall -ggdb -c tempmodel.c

vectors.o vectors.h : vectors.c
	./headergen.py vectors.c
	gcc -Wall -ggdb -c vectors.c

action_queue.o action_queue.h : action_queue.c
	./headergen.py action_queue.c
	gcc -Wall -ggdb -c action_queue.c

actions.o actions.h : actions.c action_queue.h ptrlist.h loadconf.h tempmodel.h
	./headergen.py actions.c
	gcc -Wall -ggdb -c actions.c

capture/plugin_header.h capture/plugin_header.o : capture/plugin_header.c
	cd capture; gcc -Wall -ggdb -c plugin_header.c; ../headergen.py plugin_header.c

capture/point_velocity.so : capture/point_velocity.c capture/plugin_header.h loadconf.h ptrlist.h tempmodel.h vectors.h
	cd capture; gcc -Wall -c -fpic -ggdb point_velocity.c; gcc -shared -Wl,-soname,point_velocity.so -o point_velocity.so point_velocity.o

clean :
	rm *.o *.h physyn
	cd capture; rm *.so *.o *.h
