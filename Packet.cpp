#include"Packet.h"

Packet::Packet()
{
    _type = 0;
    _numberOfPacket = 0;
    _lengthOfData = 0;
    uint16_t _summa = ;
    _data.assign(_lengthOfData, 0);
}
 uint8_t* Packet::Resize(uint8_t *oldMassiv, uint8_t newElement, int size)
 {

    uint8_t *newMassiv = new uint8_t[size];

if(oldMassiv == nullptr)
{
    newMassiv[size - 1] = newElement;
    uint8_t *newPtr = newMassiv;

    return newMassiv;
}

    for(int i = 0; i < size; ++i)
        newMassiv[i] = oldMassiv[i];

    newMassiv[size - 1]  = newElement;

    delete[]oldMassiv;

    uint8_t *newPtr = newMassiv;

    return newMassiv;
 }

uint8_t* Packet::Pack()
{
    int size = 1;
    uint8_t *packedPacket;
Resize(packedPacket,(uint8_t) _header >> 8, size);
size++;

Resize(packedPacket,(uint8_t) _header &0XFF, size);
size++;

Resize(packedPacket,_type, size);
size++;

Resize(packedPacket, (uint8_t) _numberOfPacket >> 8, size);
size++;

Resize(packedPacket, (uint8_t) _numberOfPacket &0XFF, size);
size++;

Resize(packedPacket, (uint8_t) _lengthOfData >> 8, size);
size++;

Resize(packedPacket, (uint8_t) _lengthOfData &0XFF, size);
size++;

    auto it = _data.begin();
    int index = 0;
    for(; it != _data.end(); ++index, ++it )
        packedPacket[7 + index] += *it;

    packedPacket[++index] = (uint8_t) _summa >> 8;
    packedPacket[++index] = (uint8_t) _summa &0XFF;

    return packedPacket;
}

void Packet::Unpack(uint8_t *data)
{
    if(data[0] != _header) throw "INVALID DATA";
}
