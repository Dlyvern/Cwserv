#include"Server.h"


void Server::Connection()
{
    sockaddr_in addr;
    int sock;
    addr.sin_family  = AF_INET;
    addr.sin_port = _port;
    addr.sin_addr.s_addr = inet_addr(_ip);

    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);
    
    // if( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    //     exit(EXIT_FAILURE);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr))<0)
        exit(EXIT_FAILURE);
    
    close(sock);

}
