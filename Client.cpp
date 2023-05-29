#include"Client.h"

void Client::Connection()
{
    sockaddr_in addr; 
    
    int sock;
    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);

    addr.sin_family = AF_INET; 
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        exit(EXIT_FAILURE);
}
