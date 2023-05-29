#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h> 
#include<iostream>
#include<unistd.h>

class Server
{
private:
    uint64_t _port = 8080;
    const char* _ip = "127.0.0.1";
public:
    void Connection();
};
