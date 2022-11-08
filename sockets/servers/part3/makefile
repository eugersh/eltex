all: main client

main: main.o
	gcc main.c -o main -lpthread
        
client: client.o
	gcc client.c -o client

clean:
	rm *.o

