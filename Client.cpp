#include "Client.h"

void Client::SendMessage()
{
    std::unique_ptr<Packet> packet;
    unsigned int bytesSent = 0;
    std::vector<uint8_t> dataToSent;
    const char *ptrDataToSent; // Because we can't send data using vector ;(
    int sizeOfData;

    while (_connected)
    {
        packet = Fabrica::CreateARandomPacket();

        dataToSent = packet->Pack();

        ptrDataToSent = (const char *)&dataToSent.at(0);

        sizeOfData = strlen(ptrDataToSent);

        _alive = true;

        while (bytesSent < dataToSent.size())
        {
            int result = send(_sock, ptrDataToSent, sizeOfData, 0);

            if (result > 0)
            {
                ptrDataToSent += result; // Moving ptr
                sizeOfData -= result;
                bytesSent += result;
            }
        }
        ptrDataToSent = nullptr;

        dataToSent.clear();
        dataToSent.shrink_to_fit();
        bytesSent = 0;
        _alive = false;
        // To avoid memory leak
        Packet *rawPtr = packet.release();
        sizeOfData = 0;
        delete (rawPtr);
    }
}

Client::Client(int id, int port, std::string ip) : _id(id), _port(port), _addrInfo{0}, _sock(0), _connected(false), _alive(false)
{
    //_ip = ip.c_str();
    _ip = "192.168.31.233";
    _sizeAddrInfo = sizeof(_addrInfo);
}

void Client::Initialization()
{
    bzero((char *)&_addrInfo, _sizeAddrInfo);
    _addrInfo.sin_family = AF_INET;
    _addrInfo.sin_port = htons(_port);
    _addrInfo.sin_addr.s_addr = inet_addr(_ip);

    _sock = socket(AF_INET, SOCK_STREAM, 0);

    if ((connect(_sock, (sockaddr *)&_addrInfo, _sizeAddrInfo)) < 0)
        exit(EXIT_FAILURE);

    _connected = true;
}

void Client::Start()
{
    Initialization();

    _sendingThread = std::thread([this]()
                                 { SendMessage(); });
}

Client::~Client()
{
    _sendingThread.~thread();
    _receivingThread.~thread();
    close(_sock);
}

void Client::Disconnect()
{
    _connected = false;
    _alive = false;
    close(_sock);
}

bool Client::GetStatusConnection()
{
    return _connected;
}

bool Client::GetStatusAlive()
{
    return _alive;
}

uint16_t Client::GetPort()
{
    return _addrInfo.sin_port;
}

int Client::GetId()
{
    return _id;
}
