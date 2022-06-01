/*
	C socket client call center simulation example, 
    handles multiple clients using threads
    by Esrah Zahid (S020289)
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
    char server_response[1];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
    
    while(1)
	{       
        memset(server_response, 0, sizeof(server_response));	
        if(recv(sock , server_response,1,0 ) < 0)
		{
			puts("Error getting server response");
			break;
		}

		if(server_response[0]=='1'){
			printf("You are put on hold, wait for other clients to finish...\n");	
			while(server_response[0]=='1'){

				recv(sock,server_response,1,0) >= 0;	
				sleep(2);

				if(server_response[0]=='0'){
					break;
				}
                if(server_response[0]=='3'){
					printf("Cannot join. \n");
                    break;
				}
			}
			break;
		}
		else if(server_response[0]=='3'){
			printf("Server is full, try again later...\n");
			return 0;
		}
		else{
			break; //server_response: okay
            printf("Compare break\n");
		}
	}
	
	puts("Connected\nType [x] to quit call.\nYou have a time limit of 10 seconds.\n\n");
	
	//keep communicating with server
	while(1)
	{       
        
		printf("Enter message: ");
		scanf("%s" , message);
        
        if(!strcmp(message,"x")){
			puts("You typed 'x'. Leaving server...");
			break;
		}
		
		//Send some data
		if( send(sock , message , strlen(message) , 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 2000 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
        
        if(!strcmp(server_reply,"4")){
			puts("Your time is up. Leaving server...");
			break;
		}
		
        printf("Server Reply: %s \n",server_reply );
        memset(server_reply, 0, sizeof (server_reply));
	}
	
	close(sock);
	return 0;
}
