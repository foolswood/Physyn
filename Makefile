CC=gcc
CFLAGS=-Wall -ggdb
all : physyn plugins

physyn : physyn.c action_queue.o actions.o capture.o charlist.o io.o linelist.o loadmodel.o midi.o model.o parsingfuncs.o points.o readfile.o springs.o tempmodel.o vectors.o loadmodel.h io.h springs.h points.h capture.h action_queue.h 
	$(CC) $(CFLAGS) -rdynamic -lm -ldl  -ljack   physyn.c -o physyn *.o

plugins : actions/move.so capture/point_velocity.so io/jack.so 

action_queue.h : action_queue.c 
	./headergen.py action_queue.c

action_queue.o : action_queue.c 
	$(CC) $(CFLAGS) -c action_queue.c

actions.h : actions.c action_queue.h linelist.h tempmodel.h parsingfuncs.h 
	./headergen.py actions.c

actions.o : actions.c action_queue.h linelist.h tempmodel.h parsingfuncs.h 
	$(CC) $(CFLAGS) -c actions.c

capture.h : capture.c tempmodel.h linelist.h io.h 
	./headergen.py capture.c

capture.o : capture.c tempmodel.h linelist.h io.h 
	$(CC) $(CFLAGS) -c capture.c

charlist.h : charlist.c 
	./headergen.py charlist.c

charlist.o : charlist.c 
	$(CC) $(CFLAGS) -c charlist.c

io.h : io.c tempmodel.h 
	./headergen.py io.c

io.o : io.c tempmodel.h 
	$(CC) $(CFLAGS) -c io.c

linelist.h : linelist.c charlist.h 
	./headergen.py linelist.c

linelist.o : linelist.c charlist.h 
	$(CC) $(CFLAGS) -c linelist.c

loadmodel.h : loadmodel.c linelist.h model.h tempmodel.h readfile.h parsingfuncs.h capture.h actions.h 
	./headergen.py loadmodel.c

loadmodel.o : loadmodel.c linelist.h model.h tempmodel.h readfile.h parsingfuncs.h capture.h actions.h 
	$(CC) $(CFLAGS) -c loadmodel.c

midi.h : midi.c io.h 
	./headergen.py midi.c

midi.o : midi.c io.h 
	$(CC) $(CFLAGS) -c midi.c

model.h : model.c points.h springs.h 
	./headergen.py model.c

model.o : model.c points.h springs.h 
	$(CC) $(CFLAGS) -c model.c

parsingfuncs.h : parsingfuncs.c linelist.h charlist.h vectors.h 
	./headergen.py parsingfuncs.c

parsingfuncs.o : parsingfuncs.c linelist.h charlist.h vectors.h 
	$(CC) $(CFLAGS) -c parsingfuncs.c

points.h : points.c vectors.h 
	./headergen.py points.c

points.o : points.c vectors.h 
	$(CC) $(CFLAGS) -c points.c

readfile.h : readfile.c linelist.h charlist.h 
	./headergen.py readfile.c

readfile.o : readfile.c linelist.h charlist.h 
	$(CC) $(CFLAGS) -c readfile.c

springs.h : springs.c vectors.h points.h 
	./headergen.py springs.c

springs.o : springs.c vectors.h points.h 
	$(CC) $(CFLAGS) -c springs.c

tempmodel.h : tempmodel.c vectors.h points.h springs.h model.h 
	./headergen.py tempmodel.c

tempmodel.o : tempmodel.c vectors.h points.h springs.h model.h 
	$(CC) $(CFLAGS) -c tempmodel.c

vectors.h : vectors.c 
	./headergen.py vectors.c

vectors.o : vectors.c 
	$(CC) $(CFLAGS) -c vectors.c

actions/move.so : actions/move.c action_queue.h linelist.h tempmodel.h parsingfuncs.h vectors.h midi.h 
	$(CC) $(CFLAGS) -fpic -c actions/move.c -o actions/move.o
	gcc -shared -Wl,-soname,actions/move.so -o actions/move.so actions/move.o

capture/point_velocity.so : capture/point_velocity.c parsingfuncs.h linelist.h tempmodel.h vectors.h 
	$(CC) $(CFLAGS) -fpic -c capture/point_velocity.c -o capture/point_velocity.o
	gcc -shared -Wl,-soname,capture/point_velocity.so -o capture/point_velocity.so capture/point_velocity.o

io/jack.so : io/jack.c 
	$(CC) $(CFLAGS) -fpic -c io/jack.c -o io/jack.o
	gcc -shared -Wl,-soname,io/jack.so -o io/jack.so io/jack.o

clean :
	rm *.o *.h physyn
	rm */*.so */*.o
