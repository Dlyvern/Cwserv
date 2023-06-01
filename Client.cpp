#include"Client.h"

void Client::Connection()
{  
    int sock;

    close(sock);

    if(sock = socket(AF_INET, SOCK_STREAM, 0) < 0)
        exit(EXIT_FAILURE);

    sockaddr_in addr;
    bzero((char*)&addr, sizeof(addr));
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);
    int socketForConnection;
    
    for(;;)
    {
        if(socketForConnection = connect(sock, (sockaddr *)&addr, sizeof(addr)) != -1)
            break;
    }

    while(1)
    {
        std::vector<uint8_t>data = {0xAA, 0x55,  TypeOfPackets::Ping, 0x00, 0x00, 0x00, 0xD};

        std::string myString = "Hello World!";

        for(int i = 0; i < myString.length(); ++i)
            data.push_back((uint8_t) myString[i]);

        data.push_back(0xB2);
        data.push_back(0xE0);

        Packet*packet = new Packet(data);

        std::vector<uint8_t> resPack;

        packet->Pack(resPack);
    }

    close(socketForConnection);
    close(sock);
}
