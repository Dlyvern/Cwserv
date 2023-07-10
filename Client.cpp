#include "Client.h"

Client::Client(uint16_t id, uint16_t port, const char * ip, int socket, SafeVectorPackets* output) : _id{id}, _port{port}, _sock{socket}, _output{output}
{
    _ip = ip;
    _sizeAddrInfo = sizeof(_addrInfo);
}

void Client::Start()
{
    _connected = true;
    _futureReader = std::async(std::launch::async, [this]{Reader();});
}

uint16_t Client::GetId() const
{
    return _id;
}

void Client::Reader() const
{
    while(_connected)
    {
        printf("Reading the message\n");
        long received_bytes{0};
        long result{0};
        SafeVectorData data;
        uint8_t x;

       PacketParser packetParser(data, *_output);
       packetParser.StartProcess();

        while (result != -1)
        {
            result = recv(_sock, &x, 1, 0);
            data.Push_back(x);
            received_bytes += result;
        }

        packetParser.StopProcess();
    }
}

void Client::Sender(std::vector<std::shared_ptr<Packet>> &packetsToSend)
{
    while(_connected)
    {
        for(const auto &packet : packetsToSend)
        {
            std::vector<uint8_t> dataToSend = packet->Pack();

            _futureSendPacket.push_back(std::async(std::launch::async, [&, this]
            {
                uint16_t sent_bytes{0};
                long result;
                const char *ptr_data_to_sent; // Because we can't send data using vector ;(
                uint16_t size_of_data;

                ptr_data_to_sent = (const char *)&dataToSend[0];

                size_of_data = strlen(ptr_data_to_sent);

                while (sent_bytes < dataToSend.size())
                {
                    if((result = send(_sock, ptr_data_to_sent, size_of_data, 0)) == -1) break;

                    ptr_data_to_sent += result; // Moving ptr
                    size_of_data -= result;
                    sent_bytes += result;
                }
            }));
        }
    }
}

Client::~Client()
{
    close(_sock);
}

void Client::Disconnect()
{
    _connected = false;
    _alive = false;
    close(_sock);
}
