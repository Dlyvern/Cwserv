#include"Packet.h"

void Packet::MakeAPacketForCalculatingSRC(std::vector<uint8_t>&arrayForCalculatingCRC)
{
  arrayForCalculatingCRC = 
  {
    {static_cast<uint8_t>(_header >> 8)},
    {static_cast<uint8_t>(_header &0XFF)}, 
    _type, 
    {static_cast<uint8_t>(_numberOfPacket >> 8)},
    {static_cast<uint8_t>(_numberOfPacket &0XFF)},
    {static_cast<uint8_t>(_lengthOfData >> 8)},
    {static_cast<uint8_t>( _lengthOfData &0XFF)}
  };

  for(const auto&it : _data)
    arrayForCalculatingCRC.push_back(it);
}

Packet::Packet(std::vector<uint8_t>&data)
{
    _type = data.at(2);

    _numberOfPacket = {static_cast<uint16_t>(data.at(3) << 8 | data.at(4))};

    _lengthOfData = {static_cast<uint16_t>(data.at(5) << 8 | data.at(6))};

    int index = 0;
    
    for(; index < _lengthOfData - 1; ++index)
        _data.push_back(data[index + 7]);

    _summa = {static_cast<uint16_t>(data.at(index + 7) << 8 | data.at(index + 8))};
}

Packet::Packet()
{
  _type = 0;
  _numberOfPacket = 0;
  _lengthOfData = 0;
  std::vector<uint8_t>data;
  MakeAPacketForCalculatingSRC(data);
  _summa = CRC16_2(data);
  _data.assign(_lengthOfData, 0);
}

void Packet::Pack(std::vector<uint8_t>&packedPacket )
{
    MakeAPacketForCalculatingSRC(packedPacket);

    uint16_t crc = (CRC16_2(packedPacket));

    uint8_t c = {static_cast<uint8_t> (crc >> 8)};
    uint8_t rc = {static_cast<uint8_t> (crc &0XFF)};


    packedPacket.push_back(c);
    packedPacket.push_back(rc);
}

void Packet::Unpack(std::vector<uint8_t>&data)
{
    if(data.at(0) != ((uint8_t) _header >> 8))
      throw "INVALID DATA";
    
    _type = data.at(2);

    _numberOfPacket = {static_cast<uint16_t>(data.at(3) << 8 | data.at(4))};

    _lengthOfData = {static_cast<uint16_t>(data.at(5) << 8 | data.at(6))};

    int index = 0;

    for(; index < _lengthOfData; ++index)
        _data.push_back(data[index + 7]); //Data begins at 7 index

    _summa = {static_cast<uint16_t>(data.at(index + 7) << 8 | data.at(index + 8))};
}
