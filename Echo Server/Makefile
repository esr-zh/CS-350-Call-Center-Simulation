CC=gcc
LDFLAGS=-pthread

make: echo_client.c echo_server.c
	$(CC) -o server echo_server.c -lpthread
	$(CC) -o client echo_client.c
clean:
	rm -f server
	rm -f client

