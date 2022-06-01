# CS-350-Call-Center-Simulation
TCP/IP-socket based client-server call center simulation program with C language for CS 350 course. 

#### Assignment description:
#####  Part 1) Echo Server
• The server (echo_server.c) will bind() to an IP-address and a port number (localhost / 127.0.0.1 and port 8888) to listen() to client connection requests on this line and accept() connections.
• The client program (echo_client.c) will connect() to the echo_server (localhost:8888) to send() and recv() messages.
• The server will recv() text messages from the socket and send /echo/ write() them back to the client without any change.
##### Part 2) Call Center Simulation
• With the adjustments made to the above code I simulated a Call Center, where up to 2 clients/customers can be simultaneously accepted by the Call Center for echo chatting
• Made use of pthreads to handle the number of clients. Max 3 can be echoing with each other at a time
• Each client has 10 seconds to Chat/Echo with the server, after which the server will close the client connection to accept new clients.
• A 3rd client is accepted (3rd thread), but not allowed to echo/chat until one of the two clients finishes. This represents a call center wait queue without an actual queue data structure
