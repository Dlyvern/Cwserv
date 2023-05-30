#include"Server.h"

void Server::Connection()
{
    const int opt = 1; 
    int sock;
    socklen_t optLen = sizeof(opt);
    sockaddr_in addr, peer_addr;
    socklen_t peer_addr_size;
    
    peer_addr_size = sizeof(peer_addr);

    close(sock); //In case if the old socket is still opened

    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);

    bzero((char*)&addr, sizeof(addr));

    if (setsockopt(sock, SOL_SOCKET, SO_OOBINLINE, (char*)&opt, sizeof(int)) < 0)  
        exit(EXIT_FAILURE);

    addr.sin_family  = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        exit(EXIT_FAILURE);

    if(listen(sock, _maxLengthOfConnections) < 0)
        exit(EXIT_FAILURE);

    if(accept(sock, (struct sockaddr *) &peer_addr, &peer_addr_size) < 0)
        exit(EXIT_FAILURE);
        
    int bytesRead = 0;

    int bytesWritten = 0;

    while(1)
    {        
        //Receiving message from Client
        
        Packet *packet = new Packet();

        memset(&packet, 0, sizeof(packet));

        //bytesRead += recv(sock,  sizeof(packet), 0);
        
        //Sending message to Client

        //bytesWritten += send(sock, , sizeof(packet), 0);
    }

    close(sock);
}
