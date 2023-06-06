#ifndef SERVER_H

#ifndef PACKET_PARSER_H
#include"PacketParser.h"
#endif

#ifndef CLIENT_H
#include"Client.h"
#endif

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<iostream>
#include<unistd.h>
#include<cstring>
#include<thread>
#include<vector>
#include<mutex>
#include<memory>

class Server
{
private:
    int16_t _port;
    const char* _ip;
     static bool _work;
    static unsigned int _clientId;
    int _sock;
    struct sockaddr_in _addrInfo;
    socklen_t _sizeAddrInfo;
    int _maxNumberOfConnections;

    std::thread _acceptThread;
    std::vector<std::thread>_receivingMessagesThread;
    std::mutex _mutex;
    std::vector<std::unique_ptr<Client>>_clients;

    void AcceptingClients();

    void ReceivingMessages(int newSocket);

    void Initialization();

    std::unique_ptr<Client> FindClient(int id);

public:

    void Start();
    void Stop();
    bool DisconnectClient(int id);
    void DisconnectClients();

    Server(int port, std::string ip, int maxNumberOfConnections);
    ~Server();
};

#endif
