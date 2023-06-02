#include "Client.h"

// Make fabrica for creating random packages

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
            std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max); // distribution in range [1, 6]

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

        if(_numberOfPacket < 9)
        {
            data.push_back(0x00);
            data.push_back( (uint8_t)_numberOfPacket);
        }

        std::string myString = messages.at(randomNumber(0, messages.size() - 1));

        data.push_back(0x00);
        data.push_back((uint8_t) myString.length());

        for (int i = 0; i < myString.length(); ++i)
            data.push_back((uint8_t)myString[i]);

        uint16_t CRC = CRC16_2(data);
        data.push_back({static_cast<uint8_t> (CRC >> 8)});
        data.push_back({static_cast<uint8_t> (CRC &0XFF)});
        
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
    int socketForConnection;

    for (;;)
    {
        if (socketForConnection = connect(sock, (sockaddr *)&addr, sizeof(addr)) != -1)
            break;
    }

    while (1)
    {
        Fabrica * f = new Fabrica();

        Packet *packet = f->CreateARandomPacket();

        std::vector<uint8_t> resPack;

        packet->Pack(resPack);

        std::string result = ByteToHex(resPack);

        std::cout << result << '\n';

        PacketParser *packetParser = new PacketParser(resPack);

        packetParser->StartProcess();
    }

    close(socketForConnection);
    close(sock);
}
