CC = g++
INCLUDE = /usr/include/gdbm
OBJECTS = Main.o SyncAdd.o ShareLib.o Exception.o Signal.o CPlusPlus.o CPlusPlusNew.o \
			Affinity.o SimpleList.o ListExample.o PrintDir.o Limits.o
CFLAGS = -g -O0 -Werror -std=c++14
LDFLAGS = -lpthread -rdynamic -ldl -lrt -lm -lgdbm

all:MyTest MyArgOpt

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

test:
	echo "do something"

.PHONY:clean
clean:
	-make clean -C lib/
	-rm MyTest MyArgOpt  $(OBJECTS) Dbm.o