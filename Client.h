#ifndef CLIENT_H

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include<string.h>
#include<thread>
#include"PacketParser.h"
#include<algorithm>
#include <sstream>
#include <cstring>
#include<iostream>
#include<iomanip>

std::string ByteToHex(std::vector<uint8_t>&data);

class Client
{
private:
    uint16_t _port = 8080;
    const char* _ip = "127.0.0.1";
    std::thread _sendingThread;
    std::thread _receivingThread;
 public:
    void Connection();  
};


#endif
