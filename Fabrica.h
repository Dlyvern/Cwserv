#include<memory>
#include<unordered_map>
#include<random>
#include"Packet.h"

struct Fabrica
{
private:
    static int _numberOfPacket;

public:
    static std::unique_ptr<Packet>CreateARandomPacket();

    Fabrica();
};
