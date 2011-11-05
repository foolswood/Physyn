all : physyn plugins

physyn : physyn.c capture.o loadmodel.o model.o out_channels.o physyn.o points.o linelist.o charlist.o springs.o tempmodel.o vectors.o action_queue.o
	gcc -Wall -ggdb -rdynamic -lm -ldl `pkg-config --cflags --libs jack` -o physyn *.o

plugins : capture/point_velocity.so

action_queue.o action_queue.h : action_queue.c 
	./headergen.py action_queue.c
	gcc -Wall -ggdb -c action_queue.c

actions.o actions.h : actions.c action_queue.h linelist.h tempmodel.h parsingfuncs.h vectors.h 
	./headergen.py actions.c
	gcc -Wall -ggdb -c actions.c

capture.o capture.h : capture.c tempmodel.h linelist.h capture/plugin_header.h 
	./headergen.py capture.c
	gcc -Wall -ggdb -c capture.c

charlist.o charlist.h : charlist.c 
	./headergen.py charlist.c
	gcc -Wall -ggdb -c charlist.c

linelist.o linelist.h : linelist.c charlist.h 
	./headergen.py linelist.c
	gcc -Wall -ggdb -c linelist.c

loadmodel.o loadmodel.h : loadmodel.c linelist.h model.h tempmodel.h readfile.h parsingfuncs.h capture.h out_channels.h actions.h 
	./headergen.py loadmodel.c
	gcc -Wall -ggdb -c loadmodel.c

model.o model.h : model.c points.h springs.h 
	./headergen.py model.c
	gcc -Wall -ggdb -c model.c

out_channels.o out_channels.h : out_channels.c 
	./headergen.py out_channels.c
	gcc -Wall -ggdb -c out_channels.c

parsingfuncs.o parsingfuncs.h : parsingfuncs.c linelist.h charlist.h vectors.h 
	./headergen.py parsingfuncs.c
	gcc -Wall -ggdb -c parsingfuncs.c

points.o points.h : points.c vectors.h 
	./headergen.py points.c
	gcc -Wall -ggdb -c points.c

readfile.o readfile.h : readfile.c linelist.h charlist.h 
	./headergen.py readfile.c
	gcc -Wall -ggdb -c readfile.c

springs.o springs.h : springs.c vectors.h points.h 
	./headergen.py springs.c
	gcc -Wall -ggdb -c springs.c

tempmodel.o tempmodel.h : tempmodel.c vectors.h points.h springs.h model.h 
	./headergen.py tempmodel.c
	gcc -Wall -ggdb -c tempmodel.c

vectors.o vectors.h : vectors.c 
	./headergen.py vectors.c
	gcc -Wall -ggdb -c vectors.c

capture/plugin_header.h capture/plugin_header.o : capture/plugin_header.c
	cd capture; gcc -Wall -ggdb -c plugin_header.c; ../headergen.py plugin_header.c

capture/point_velocity.so : capture/point_velocity.c capture/plugin_header.h parsingfuncs.h linelist.h tempmodel.h vectors.h
	cd capture; gcc -Wall -c -fpic -ggdb point_velocity.c; gcc -shared -Wl,-soname,point_velocity.so -o point_velocity.so point_velocity.o

clean :
	rm *.o *.h physyn
	cd capture; rm *.so *.o *.h
