#ifndef SERVER_H

#ifndef PACKET_PARSER_H
#include"PacketParser.h"
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

class Server
{
private:
    int16_t _port = 8080;
    const char* _ip = "127.0.0.1";
    uint64_t _maxLengthOfConnections = 100;
    std::thread _acceptThread;
    std::vector<std::thread>_receivingMessagesThread;
    std::mutex _mutex;
    void AcceptingClients(int sock);

    void ReceivingMessages(int newSocket);
public:
    void Connection();
};

#endif
