physyn : physyn.c capture.o loadmodel.o model.o out_channels.o physyn.o points.o ptrlist.o springs.o tempmodel.o vectors.o capture_plugins
	gcc -Wall -ggdb -rdynamic -lm -ldl `pkg-config --cflags --libs jack` -o physyn *.o

capture.o capture.h : capture.c tempmodel.h ptrlist.h capture/plugin_header.h
	./headergen.py capture.c
	gcc -Wall -ggdb -c capture.c

loadconf.o loadconf.h : loadconf.c ptrlist.h vectors.h
	./headergen.py loadconf.c
	gcc -Wall -ggdb -c loadconf.c

loadmodel.o loadmodel.h : loadmodel.c ptrlist.h model.h tempmodel.h loadconf.h capture.h out_channels.h
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

capture/plugin_header.h : capture/plugin_header.c
	./headergen.py capture/plugin_header.c
	gcc -Wall -ggdb -c capture/plugin_header.c

capture_plugins : capture/plugin_header.h
	cd capture; make

clean :
	rm *.o
	rm *.h
	rm physyn
	cd capture; make clean
