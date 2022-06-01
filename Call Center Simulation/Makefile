CC=gcc
LDFLAGS=-pthread

make: call_client.c call_server.c
	$(CC) -o server call_server.c -lpthread
	$(CC) -o client call_client.c
clean:
	rm -f server
	rm -f client

