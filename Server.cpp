#include"Server.h"

void Server::Connection()
{
    const int opt = 1; 

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)  
        exit(EXIT_FAILURE);

    sockaddr_in addr;

    bzero((char*)&addr, sizeof(addr));

    addr.sin_family  = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
        exit(EXIT_FAILURE);

    listen(sock, _maxLengthOfConnections);

    _acceptThread = std::thread([this, sock](){AcceptingClients(sock);});

}

void Server::AcceptingClients(int sock)
{
    sockaddr_in newAddr;//To handle a new connection with a client
    socklen_t newAddrSize = sizeof(newAddr);
    int newSocket;
    
    for(;;)
    {
        if(newSocket = accept(sock, reinterpret_cast<sockaddr*>(&newAddr), &newAddrSize) != -1)
        {
            //_receivingMessagesThread.push_back(std::thread([this, newSocket](){ReceivingMessages(newSocket);}));
        }
    }
}

void Server::ReceivingMessages(int newSocket)
{
    while(1)
    {        
        //Receiving message from Client
        //Receiving bytes and giving them to PacketParser. PacketPareser making normal packets from bytes

        


        //bytesRead += recv(sock,  sizeof(packet), 0);
        
        //Sending message to Client

        //bytesWritten += send(sock, , sizeof(packet), 0);
    }
}
