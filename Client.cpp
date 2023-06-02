#include "Server.h"

void Server::Connection()
{
    const int opt = 1;

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        exit(EXIT_FAILURE);

    sockaddr_in addr;

    bzero((char *)&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
        exit(EXIT_FAILURE);

    listen(sock, _maxLengthOfConnections);
    _acceptThread = std::thread([this, sock]()
                                { AcceptingClients(sock); });
}

void Server::AcceptingClients(int sock)
{
    sockaddr_in newAddr; // To handle a new connection with a client
    socklen_t newAddrSize = sizeof(newAddr);
    int newSocket;
    for (;;)
    {
        newSocket = accept(sock, reinterpret_cast<sockaddr *>(&newAddr), &newAddrSize);

        if (newSocket != -1)
        {
            _receivingMessagesThread.push_back(std::thread([this, newSocket]()
                                                   { this->ReceivingMessages(newSocket); }));
            
        }
    }
}

void Server::ReceivingMessages(int newSocket)
{
    unsigned int bytesReceived = 0;
    unsigned int bytesReceivedPast = 0;
    int tmp = 0;
    std::vector<uint8_t> data;
    uint8_t x;
    PacketParser *packetParser = new PacketParser(&data, &_mutex);
    packetParser->StartProcess();
    while (1)
    {
        bytesReceivedPast = bytesReceived;

        tmp = recv(newSocket, &x, 1, 0);

        if (tmp > 0)
        {
            _mutex.lock();
            bytesReceived += tmp;
            data.push_back(x);
            _mutex.unlock();
        }
    }
}
