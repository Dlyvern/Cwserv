#ifndef PACKET_H

#define PACKET_H

#include<cstdint>
#include<vector>
#include<fstream>
#include<ostream>
#include <bitset>
#include"CRC16.h"

enum TypeOfPackets
{
    Ping = 0x01,
    Pong = 0x81,
    REQ = 0x02,
    ANS = 0x82,
    FND = 0x00,
    FNDA = 0x80,
    EVENT = 0x03,
    EVENTA = 0x83
};

class Packet
{
private:
    const uint16_t _header = 0XAA55;
    uint8_t _type;
    uint16_t _numberOfPacket;
    uint16_t _lengthOfData;
    std::vector<uint8_t>_data;
    uint16_t _summa;
    void MakeAPacketForCalculatingSRC(std::vector<uint8_t>&arrayForCalculatingCRC);
public:

    Packet();

    Packet(std::vector<uint8_t>&data);
    
    std::vector<uint8_t> Pack();

    bool Unpack(std::vector<uint8_t>&data);
};

#endif
