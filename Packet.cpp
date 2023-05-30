#include"Packet.h"

Packet::Packet()
{
    _type = 0;
    _numberOfPacket = 0;
    _lengthOfData = 0;
    uint16_t _summa = ;
    _data.assign(_lengthOfData, 0);
}

uint8_t* Packet::Pack()
{
    std::vector<uint8_t>tempPackedPacket;

    auto x = [&](uint8_t data)->void{tempPackedPacket.push_back(data);};

    //Splitting data into pieces
    x((uint8_t) _header >> 8);
    x((uint8_t) _header &0XFF);

    x(_type);

    x((uint8_t) _numberOfPacket >> 8);
    x((uint8_t) _numberOfPacket &0XFF);

    x((uint8_t) _lengthOfData >> 8);
    x((uint8_t) _lengthOfData &0XFF);
    
    for(auto it = _data.begin(); it != _data.end(); ++it)
        x(*it);

    x((uint8_t) _summa >> 8);
    x((uint8_t) _summa &0XFF);

    uint8_t *packedPacket = new uint8_t [tempPackedPacket.size()];

    for(int index = 0; index != tempPackedPacket.size(); ++index)
        packedPacket[index] = tempPackedPacket.at(index);

    return packedPacket;
}

void Packet::Unpack(uint8_t *data)
{
    if(data[0] != _header) throw "INVALID DATA";
}
