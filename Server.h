#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<iostream>
#include<unistd.h>
#include<cstring>
#include"Packet.h"
#include<boost/archive/binary_oarchive.hpp>
#include<boost/archive/binary_iarchive.hpp>
#include<thread>

class Server
{
private:
    uint16_t _port = 8080;
    const char* _ip = "127.0.0.1";
    uint64_t _maxLengthOfConnections = 100;
    std::thread _acceptThread;
    std::thread _receivingThread;

    void AcceptingClients(int sock);

    void ReceivingMessages(int newSocket);
public:
    void Connection();
};

