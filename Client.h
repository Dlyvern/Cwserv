#ifndef CLIENT_H

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
#include<unordered_map>
#include<random>

#include<mutex>

std::string ByteToHex(std::vector<uint8_t>&data);

class Client
{
private:
    uint16_t _port = 8080;
    const char* _ip = "127.0.0.1";
    // "192.168.31.152";
    //"127.0.0.1";
    std::thread _sendingThread;
    std::thread _receivingThread;
    void SendMessage(int&sock);
 public:
    void Connection();  
};


#endif
