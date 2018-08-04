CC = g++
OBJECTS = Main.o SyncAdd.o ShareLib.o Exception.o Signal.o CPlusPlus.o Affinity.o
CFLAGS = -g -O0 -Werror
LDFLAGS = -lpthread -rdynamic -ldl -lrt

Mytest:$(OBJECTS)
	make -C lib/
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^
	
.PHONY:clean
clean:
	-make clean -C lib/
	-rm Mytest $(OBJECTS)

test:
	echo "do something"