#include"Server.h"

void Server::Connection()
{
    const int opt = 1; 
    int sock;

    close(sock); //In case if the old socket is still opened

    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);

    if (setsockopt(sock, SOL_SOCKET, SO_OOBINLINE, (char*)&opt, sizeof(int)) < 0)  
        exit(EXIT_FAILURE);

    sockaddr_in addr;
    bzero((char*)&addr, sizeof(addr));

    addr.sin_family  = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
        exit(EXIT_FAILURE);

    listen(sock, _maxLengthOfConnections);

    _acceptThread = std::thread([this, sock](){AcceptingClients(sock);});
    
    close(sock);
}

void Server::AcceptingClients(int sock)
{
    sockaddr_in newAddr;//To handle a new connection with a client
    socklen_t newAddrSize = sizeof(newAddr);
    int newSocket;
    
    for(;;)
    {
        if(newSocket = accept(sock, (sockaddr *)&newAddr, &newAddrSize) != -1)
        {
            _receivingThread = std::thread([this, newSocket](){ReceivingMessages(newSocket);});
        }
    }
}

void Server::ReceivingMessages(int newSocket)
{
    int bytesRead = 0;

    int bytesWritten = 0;

    while(1)
    {        
        //Receiving message from Client
        
        Packet *packet = new Packet();

        


        //bytesRead += recv(sock,  sizeof(packet), 0);
        
        //Sending message to Client

        //bytesWritten += send(sock, , sizeof(packet), 0);
    }
}
