#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

//directed by The Lobsters

#define WIDTH 60
#define HEIGHT 80

//Client side
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        std::cerr << "Usage: ip_address port" << std::endl; exit(0);
    } //grab the IP address and port number
    char *serverIp = argv[1]; int port = atoi(argv[2]);
    //create a message buffer
    size_t sum = WIDTH * HEIGHT*2;
    char *msg;
    //setup a socket and connection tools
    struct hostent* host = gethostbyname(serverIp);
    struct sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cout<<"Error connecting to socket!"<<std::endl;
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    
    while(1)
    {
        std::cout << ">";
        //std::string data;
        //getline(std::cin, data);
        getline(&msg, &sum, stdin);
        getchar();
        memset(&msg, 0, sizeof(msg));//clear the buffer
        if(strcmp(msg, "exit") ==  0)
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        send(clientSd, (char*)&msg, strlen(msg), 0);
        std::cout << "Awaiting server response..." << std::endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << "Server: " << msg << std::endl;
        free(msg);
    }
    std::cout << "Connection closed" << std::endl;
   
    return 0;
}


