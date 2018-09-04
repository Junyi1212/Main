CC = g++
OBJECTS = Main.o SyncAdd.o ShareLib.o Exception.o Signal.o CPlusPlus.o CPlusPlusNew.o Affinity.o SimpleList.o ListExample.o
CFLAGS = -g -O0 -Werror -std=c++14
LDFLAGS = -lpthread -rdynamic -ldl -lrt

Mytest:$(OBJECTS)
	make -C lib/
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^
	
.PHONY:clean
clean:
	-make clean -C lib/
	-rm Mytest $(OBJECTS)

test:
	echo "do something"