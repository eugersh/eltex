CC = gcc
OBJECTS = dividing.o multiplexing.o subtraction.o addition.o
CSRC = dividing.c multiplexing.c subtraction.c addition.c

main: main.o libdyn.so
	$(CC) -o main *.o -L. -ldyn -Wl,-rpath,. -ldl
main.o: main.c
	$(CC) main.c -c

addition.o: addition.c
	$(CC) addition.c -c

subtraction.o: subtraction.c
	$(CC) subtraction.c -c

multiplexing.o: multiplexing.c
	$(CC) multiplexing.c -c

dividing.o: dividing.c
	$(CC) dividing.c -c

libdyn.so: $(OBJECTS)
	$(CC) -c -fPIC $(CSRC)
	$(CC) -shared -o libdyn.so $(OBJECTS)

clean:
	rm $(OBJECTS)
