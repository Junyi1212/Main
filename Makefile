CC = g++
INCLUDE = /usr/include/gdbm
OBJECTS = Main.o
OBJECTS += SyncAdd.o
OBJECTS += ShareLib.o
OBJECTS += Exception.o
OBJECTS += Signal.o
OBJECTS += CPlusPlus.o
OBJECTS += CPlusPlusNew.o
OBJECTS += Affinity.o
OBJECTS += SimpleList.o
OBJECTS += ListExample.o
OBJECTS += PrintDir.o
OBJECTS += Limits.o
OBJECTS += Process.o
OBJECTS += Thread.o
OBJECTS += Pipe.o
OBJECTS += Sem.o
OBJECTS += Shm.o
OBJECTS += Msg.o
OBJECTS += Atomic.o
OBJECTS += BoostThread.o
OBJECTS += BoostAsio.o
OBJECTS += BoostSignal.o

CFLAGS = -g -O0 -Werror -std=c++14
LDFLAGS = -lpthread -rdynamic -ldl -lrt -lm -lgdbm -lboost_system

all:MyTest MyArgOpt PipeChild FifoChild

MyTest:$(OBJECTS) Dbm.o
	make -C lib/
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

Dbm.o:Dbm.c
	gcc -I$(INCLUDE) -c Dbm.c

MyArgOpt:
	$(CC) $(CFLAGS) ArgOpt.c -o MyArgOpt

PipeChild:
	$(CC) $(CFLAGS) PipeChild.c -o PipeChild

FifoChild:
	$(CC) $(CFLAGS) FifoChild.c -o FifoChild

test:
	echo "do something"

.PHONY:clean
clean:
	-make clean -C lib/
	-rm MyTest MyArgOpt PipeChild FifoChild $(OBJECTS) Dbm.o