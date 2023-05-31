#include"Client.h"

void Client::Connection()
{  
    int sock;

    close(sock);

    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);

    sockaddr_in addr; 
    bzero((char*)&addr, sizeof(addr));
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);
    int socketForConnection;
    
    for(;;)
    {
        if(socketForConnection = connect(sock, (sockaddr *)&addr, sizeof(addr)) != 1)
            break;
    }

    char msg[1500]; 
    std::string data = "Hello";
    int bytesWritten = 0;
    int bytesRead = 0;

    while(1)
    {
        memset(&msg, 0, sizeof(msg));
        
        strcpy(msg, data.c_str());

        bytesWritten += send(socketForConnection, (char*)&msg, strlen(msg), 0);

        bytesRead += recv(socketForConnection, (char*)&msg, sizeof(msg), 0);
    }

    close(socketForConnection);
    close(sock);
}
