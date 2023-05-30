#include"Packet.h"

Packet::Packet()
{
    _type = 0;
    _numberOfPacket = 0;
    _lengthOfData = 0;

    uint8_t *tmp = new uint8_t[7];

    tmp[0] = (uint8_t) _header >> 8;
    tmp[1] = (uint8_t) _header &0XFF;

    tmp[2] = _type;

    tmp[3] = (uint8_t) _numberOfPacket >> 8;
    tmp[4] = (uint8_t) _numberOfPacket &0XFF; 

    tmp[5] = (uint8_t) _lengthOfData >> 8;
    tmp[6] = (uint8_t) _lengthOfData &0XFF; 

    _summa = CRC16_2(tmp, 7);

    delete[] tmp;

    _data.assign(_lengthOfData, 0);
}

unsigned int CRC16_2(unsigned char *buf, int len)
{  
  unsigned int crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {

  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

  for (int i = 8; i != 0; i--) 
  {    // Loop over each bit
    if ((crc & 0x0001) != 0)
    {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
    }
  }

  return crc;
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

    uint8_t *src = new uint8_t[tempPackedPacket.size()];

    for(int index = 0; index != tempPackedPacket.size(); ++index)
        src[index] = tempPackedPacket.at(index);

    x(CRC16_2(src, tempPackedPacket.size()));

    delete[]src;

    uint8_t *packedPacket = new uint8_t [tempPackedPacket.size()];

    for(int index = 0; index != tempPackedPacket.size(); ++index)
        packedPacket[index] = tempPackedPacket.at(index);

    return packedPacket;
}

void Packet::Unpack(uint8_t *data)
{
    if(data[0] != _header) throw "INVALID DATA";
}
