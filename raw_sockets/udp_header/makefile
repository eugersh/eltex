all: main client

main: main.o
	gcc main.c -o main
client: udp_server.o
	gcc udp_server.c -o udp_server

clean:
	rm *.o
