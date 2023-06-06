#ifndef CLIENT_H

#define CLIENT_H

#ifndef PACKET_PARSER_H
#include"PacketParser.h"
#endif

#ifndef PACKET_H
#include"Packet.h"
#endif

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include<string.h>
#include<thread>
#include<algorithm>
#include <sstream>
#include <cstring>
#include<iostream>
#include<iomanip>
#include<mutex>
#include"Fabrica.h"

std::string ByteToHex(std::vector<uint8_t>&data);

class Client
{
private:
    int _sock;
    struct sockaddr_in _addrInfo;
    socklen_t _sizeAddrInfo;
    int _id;
    int _port;
    const char* _ip;
    bool _connected;
    bool _alive;
    
    std::thread _sendingThread;
    std::thread _receivingThread;
    void SendMessage();
    void Initialization();
 public:

    Client(int id, int port, std::string ip);

    void Start();

    void Disconnect();

    uint16_t GetPort();

    bool GetStatusConnection();

    bool GetStatusAlive();

    int GetId();

    ~Client();
};


#endif
