all: main tcp_client udp_client

main: main.o
	gcc main.c -o main -lpthread
        
tcp_client: tcp_client.o
	gcc tcp_client.c -o tcp_client
udp_client: udp_client.o
	gcc udp_client.c -o udp_client
	
clean: 
	rm *.o

