#include "Client.h"

struct Fabrica
{
private:
    int _numberOfPacket = 0;

public:
    Packet *CreateARandomPacket()
    {
        auto randomNumber = [](int min, int max) -> int
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);

            return dist6(rng);
        };

        std::vector<std::string> messages =
            {
                "Hello World!",
                "Who is there?",
                "I'm a programmer!"};

        const uint8_t header1 = 0XAA;
        const uint8_t header2 = 0X55;

        std::unordered_map<int, uint8_t> types =
            {
                {0, 0x01},
                {1, 0x81},
                {2, 0x02},
                {3, 0x82},
                {4, 0x00},
                {5, 0x80},
                {6, 0x03},
                {7, 0x83},
            };

        // std::vector<uint8_t> data = {0xAA, 0x55, TypeOfPackets::Ping, 0x00, 0x00, 0x00, 0xD};

        std::vector<uint8_t> data;

        data.push_back(header1);
        data.push_back(header2);

        auto it = types.begin();

        advance(it, randomNumber(0, types.size() - 1));

        data.push_back(it->second);

        if (_numberOfPacket < 9)
        {
            data.push_back(0x00);
            data.push_back((uint8_t)_numberOfPacket);
        }

        std::string myString = messages.at(randomNumber(0, messages.size() - 1));
        int size = myString.length();
        data.push_back(0x00);
        data.push_back((uint8_t)size);

        for (int i = 0; i < size; ++i)
            data.push_back((uint8_t)myString[i]);

        uint16_t CRC = CRC16::CRC16_2(data);

        data.push_back({static_cast<uint8_t>(CRC >> 8)});
        data.push_back({static_cast<uint8_t>(CRC & 0XFF)});

        Packet *packet = new Packet(data);

        _numberOfPacket++;

        return packet;
    }
};

void Client::Connection()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = inet_addr(_ip);

    for (;;)
    {
        if (connect(sock, (sockaddr *)&addr, sizeof(addr)) != -1)
            SendMessage(sock);
    }
}

void Client::SendMessage(int &sock)
{
    Fabrica *f = new Fabrica();

    Packet *packet = f->CreateARandomPacket();

    unsigned int numberOfSendedBytes = 0;

    int result;

    // std::vector<uint8_t> data = packet->Pack();

    // const char* dataa = (const char*)data.data();

    // std::transform(data.begin(), data.end(), data.begin(), htonl);

    const char *data = "Hello World!";

    numberOfSendedBytes = 0;

    int size = strlen(data);

    while ((result = send(sock, data, size, 0)) > 0)
    {
        data += result;
        size -= result;

        numberOfSendedBytes += result;
    }
}
