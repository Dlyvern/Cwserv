#include"Packet.h"

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

uint8_t* Packet::MakeAPacketForCalculatingSRC()
{
  uint8_t *arrayForCalculatingCRC{new uint8_t[7]
  {
    (uint8_t) _header >> 8, 
    (uint8_t) _header &0XFF, 
    _type, 
    (uint8_t) _numberOfPacket >> 8, 
    (uint8_t) _numberOfPacket &0XFF, 
    (uint8_t) _lengthOfData >> 8,
    (uint8_t) _lengthOfData &0XFF
  }}; 

  for(int index = 0; index < _data.size(); ++index)
    arrayForCalculatingCRC[index + 7] = _data.at(index);

  return arrayForCalculatingCRC;
}

Packet::Packet()
{
  _type = 0;
  _numberOfPacket = 0;
  _lengthOfData = 0;
  _summa = CRC16_2(MakeAPacketForCalculatingSRC(), 7);
  _data.assign(_lengthOfData, 0);
}

uint8_t* Packet::Pack()
{
    uint8_t *packedPacket = MakeAPacketForCalculatingSRC();

    int size = sizeof(packedPacket) / sizeof(uint8_t);

    packedPacket[size] = ((CRC16_2(packedPacket, size)));

    return packedPacket;
}

void Packet::Unpack(uint8_t *data)
{

}
