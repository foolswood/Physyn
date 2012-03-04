all : physyn plugins

physyn : physyn.c action_queue.o actions.o capture.o charlist.o io.o linelist.o loadmodel.o model.o parsingfuncs.o points.o readfile.o springs.o tempmodel.o vectors.o loadmodel.h io.h springs.h points.h capture.h action_queue.h 
	gcc -Wall -ggdb -rdynamic -lm -ldl  -ljack -lpthread -lrt   physyn.c -o physyn *.o

plugins : actions/move.so capture/point_velocity.so io/jack.so 

action_queue.h : action_queue.c 
	./headergen.py action_queue.c

action_queue.o : action_queue.c 
	gcc -Wall -ggdb -c action_queue.c

actions.h : actions.c action_queue.h linelist.h tempmodel.h parsingfuncs.h 
	./headergen.py actions.c

actions.o : actions.c action_queue.h linelist.h tempmodel.h parsingfuncs.h 
	gcc -Wall -ggdb -c actions.c

capture.h : capture.c tempmodel.h linelist.h io.h 
	./headergen.py capture.c

capture.o : capture.c tempmodel.h linelist.h io.h 
	gcc -Wall -ggdb -c capture.c

charlist.h : charlist.c 
	./headergen.py charlist.c

charlist.o : charlist.c 
	gcc -Wall -ggdb -c charlist.c

io.h : io.c tempmodel.h 
	./headergen.py io.c

io.o : io.c tempmodel.h 
	gcc -Wall -ggdb -c io.c

linelist.h : linelist.c charlist.h 
	./headergen.py linelist.c

linelist.o : linelist.c charlist.h 
	gcc -Wall -ggdb -c linelist.c

loadmodel.h : loadmodel.c linelist.h model.h tempmodel.h readfile.h parsingfuncs.h capture.h actions.h 
	./headergen.py loadmodel.c

loadmodel.o : loadmodel.c linelist.h model.h tempmodel.h readfile.h parsingfuncs.h capture.h actions.h 
	gcc -Wall -ggdb -c loadmodel.c

model.h : model.c points.h springs.h 
	./headergen.py model.c

model.o : model.c points.h springs.h 
	gcc -Wall -ggdb -c model.c

parsingfuncs.h : parsingfuncs.c linelist.h charlist.h vectors.h 
	./headergen.py parsingfuncs.c

parsingfuncs.o : parsingfuncs.c linelist.h charlist.h vectors.h 
	gcc -Wall -ggdb -c parsingfuncs.c

points.h : points.c vectors.h 
	./headergen.py points.c

points.o : points.c vectors.h 
	gcc -Wall -ggdb -c points.c

readfile.h : readfile.c linelist.h charlist.h 
	./headergen.py readfile.c

readfile.o : readfile.c linelist.h charlist.h 
	gcc -Wall -ggdb -c readfile.c

springs.h : springs.c vectors.h points.h 
	./headergen.py springs.c

springs.o : springs.c vectors.h points.h 
	gcc -Wall -ggdb -c springs.c

tempmodel.h : tempmodel.c vectors.h points.h springs.h model.h 
	./headergen.py tempmodel.c

tempmodel.o : tempmodel.c vectors.h points.h springs.h model.h 
	gcc -Wall -ggdb -c tempmodel.c

vectors.h : vectors.c 
	./headergen.py vectors.c

vectors.o : vectors.c 
	gcc -Wall -ggdb -c vectors.c

actions/move.so : actions/move.c action_queue.h linelist.h tempmodel.h parsingfuncs.h vectors.h 
	gcc -Wall -ggdb -fpic -c actions/move.c -o actions/move.o
	gcc -shared -Wl,-soname,actions/move.so -o actions/move.so actions/move.o

capture/point_velocity.so : capture/point_velocity.c parsingfuncs.h linelist.h tempmodel.h vectors.h 
	gcc -Wall -ggdb -fpic -c capture/point_velocity.c -o capture/point_velocity.o
	gcc -shared -Wl,-soname,capture/point_velocity.so -o capture/point_velocity.so capture/point_velocity.o

io/jack.so : io/jack.c 
	gcc -Wall -ggdb -fpic -c io/jack.c -o io/jack.o
	gcc -shared -Wl,-soname,io/jack.so -o io/jack.so io/jack.o

clean :
	rm *.o *.h physyn
	rm */*.so */*.o
