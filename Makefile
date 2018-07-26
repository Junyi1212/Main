objects = Main.o SyncAdd.o CPlusPlus.o ShareLib.o

Mytest:$(objects)
	make -C lib/
	g++ -o Mytest $(objects) -lpthread -rdynamic -ldl

Main.o:Main.c
	g++ -c Main.c
	
SyncAdd.o:SyncAdd.c SyncAdd.h
	g++ -c SyncAdd.c
	
CPlusPlus.o:CPlusPlus.cpp CPlusPlus.h
	g++ -c CPlusPlus.cpp

ShareLib.o:ShareLib.c
	g++ -c ShareLib.c

.PHONY:clean
clean:
	-make clean -C lib/
	-rm Mytest $(objects)
#在rm加一个-，表示如果某些文件出现问题，不用管，继续做后面的事情
test:
	echo "do something"