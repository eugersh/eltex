editor : main.o functions.o
	gcc -o editor main.o functions.o -lncurses
main.o : main.c functions.h
	gcc -c main.c
functions.o : functions.c functions.h
	gcc -c functions.c

clean :
	rm *.o
