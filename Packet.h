#include<cstdint>
#include<vector>
class Packet
{
private:
    uint16_t _header = 0x55;
    uint8_t _type;
    uint16_t _numberOfPacket;
    std::vector<uint64_t>_data;
    uint16_t _summa;
public:

    uint8_t Pack();

    void Unpack();
};
