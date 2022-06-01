/*
	C socket server call center simulation example, 
    handles multiple clients using threads
    by Esrah Zahid (S020289)
*/

#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <time.h>   //for 10 sec timer for each client

//the thread function
void *connection_handler(void *);
    int client_size = 0;
    
    char *okay_msg = "0";//"okay";//"Client may now join session"//When space is created for 3rd client
    char *hold_msg = "1";//"hold";//"You are put on hold, wait for other clients to finish"; //when 3rd client arrives
    char *time_msg = "2";//"time";//"your time is up"; //when 10 second duration ends
    char *busy_msg = "3";//"busy";//"Server is full, try again later"; //when there are 3 clients and 4th joins
    

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
    int msec = 0, trigger = 10; //for timed connection
    
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept any incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		printf("Current client Size: %d\n",client_size);
        
        if(client_size==2){
        client_size++;
        write(client_sock,hold_msg,1);

        while(client_size==3){
            sleep(2);
            if(client_size==1){
                write(client_sock,okay_msg,1);
                break;
            }
        }
    }

    else if(client_size>=3){
        write(client_sock,busy_msg,1);
        close(client_sock);
    }

    write(client_sock,okay_msg,strlen(okay_msg));
    

    puts("Connection accepted. New client joined.");
    client_size++;
        
		pthread_t client_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &client_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( client_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc_ptr)
{
    int client_id = client_size;
    
    time_t start_time;
    time_t current_time;
    start_time = time(NULL);
    current_time = time(NULL);
	
    //Get the socket descriptor
	int sock = *(int*)socket_desc_ptr;
	int read_size;
	char *message , client_message[2000];
	
	//Send some messages to the client
	message = "Greetings! I am your connection handler\n";
	//write(sock , message , strlen(message));
	
	//Receive a message from client
	while( ((read_size = recv(sock , client_message , 2000 , 0)) > 0 ))
	{
        current_time = time(NULL);
        if ((current_time) >= (start_time+10)){
            message = "Time's up. You will be disconnected shortly";
            printf("Client %d disconnected.\n",client_id);
            write(sock , message , strlen(message));
            client_size--;
	    close(sock);           
	    pthread_exit(NULL);
            fflush(stdout);
            break;
        }
        
        printf("Client %d wrote: %s \n",client_id, client_message);
		//Send the message back to client
		write(sock , client_message , strlen(client_message));
        memset(client_message, 0, sizeof (client_message));
	}
	
	if(read_size == 0)
	{
		printf("Client %d disconnected.\n",client_id);
        client_size--;
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	//close(sock);
	//Free the socket pointer
	free(socket_desc_ptr);
	
	return 0;
}
