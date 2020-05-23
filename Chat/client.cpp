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
#include <thread>
#include <chrono>

using namespace std;
//Client side
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } //grab the IP address and port number 
    char *serverIp = argv[1]; int port = atoi(argv[2]); 
    //create a message buffer 
    
    std::ifstream camera("cam.txt");
    int length = 60*80*2;
    char MESSAGE[length];
    camera.read(MESSAGE, length);
    camera.close();

    char msg[9600]; 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
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
        cout<<"Error connecting to socket!"<<endl; return 1;
    }
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    while(1)
    {
        cout << ">";
        int begin = 0;
        int arraySize = sizeof(MESSAGE)/sizeof(char);

        //send(clientSd, (char*)(std::to_string(arraySize)).c_str(), strlen(msg), 0);
        // while(begin+1023<=arraySize){
        //     std::string data(&MESSAGE[begin], &MESSAGE[begin+1023]);  
        //     memset(&msg, 0, sizeof(msg));//clear the buffer
        //     strcpy(msg, data.c_str());  
        //     bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        //     std::cout << begin+1023 << "bytes sent\n";
        //     if(data == "exit")
        //     {
        //         send(clientSd, (char*)&msg, strlen(msg), 0);
        //         break;
        //     }
        //     begin+=1023;
        // }

        
        std::string data(MESSAGE);
        const int SIZE = data.length();
        std::string portion = data.substr(0, 4095);
        for (int b = 0; !portion.empty(); b += 4095) {
            strcpy(msg, portion.c_str());
            send(clientSd, (char*)&msg, strlen(msg), 0);
            try {
                portion = data.substr(b, 4095);
            } catch (const std::out_of_range& e) {
                try {
                    portion = data.substr(b, data.size() - b);
                } catch (const std::out_of_range& e1) {
                    portion = "";
                }
            }
        }

        
        data.clear();
        /*
        for (int b = 0; b < SIZE;) {
            recv(clientSd, (char*)&msg, 9600, 0);
            data += msg;
            b += strlen(msg);
        }
        /*

        std::cout << data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        
        // for (int b = 0; b < strlen(MESSAGE); b += 4095) {
        //     std::string data(&MESSAGE[begin], &MESSAGE[begin+std::min(4095, );  
        //     memset(&msg, 0, sizeof(msg));//clear the buffer
        //     strcpy(msg, data.c_str());  
        //     bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        //     std::cout << begin+1023 << "bytes sent\n";
        //     if(data == "exit")
        //     {
        //         send(clientSd, (char*)&msg, strlen(msg), 0);
        //         break;
        //     }
        //     begin += 4095;
        // }

        // int rem = (arraySize%1023);
        // std::string data(&MESSAGE[begin], &MESSAGE[begin+rem]);  
        // memset(&msg, 0, sizeof(msg));//clear the buffer
        // strcpy(msg, data.c_str());  
        // bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        // std::cout << begin+1023 << "bytes sent\n";
        // if(data == "exit")
        // {
        //     send(clientSd, (char*)&msg, strlen(msg), 0);
        //     break;
        // }
        
        
        
        
        //getline(cin, data);
        //memset(&msg, 0, sizeof(msg));//clear the buffer
        //strcpy(msg, data.c_str());
        

/*
        if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        */
        //bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
        // cout << "Awaiting server response..." << endl;
        // memset(&msg, 0, sizeof(msg));//clear the buffer
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // data.clear();
        // for (int i = 0; i < 60 * 80 * 2 / 4095; i++) {
        //     recv(clientSd, (char*)&msg, sizeof(msg), 0);
        //     data += msg;
        // }
        // while(bytesRead < 2000){ 
        //     bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);
        //     data += msg;
        // }
        // recv(clientSd, (char*)&msg, sizeof(msg), 0);
        // data += msg;
        // bytesRead=0;
        // strcpy(msg, data.c_str());
        // std::cout << "we read " << bytesRead << std::endl;
        // if(!strcmp(msg, "exit"))
        // {
        //     cout << "Server has quit the session" << endl;
        //     break;
        // }
        // cout << "Server: " << data << endl;
    }
    gettimeofday(&end1, NULL);
    close(clientSd);
    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << 
    " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec- start1.tv_sec) 
      << " secs" << endl;
    cout << "Connection closed" << endl;
    return 0;    
}