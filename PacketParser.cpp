#include"PacketParser.h"

unsigned int CRC16_2(const std::vector<uint8_t>&buf, int len)
{  
  unsigned int crc = 0xFFFF;

  for (int pos = 0; pos < len; pos++)
  {

  crc ^= (unsigned int)buf[pos];

  for (int i = 8; i != 0; i--) 
  {    
    if ((crc & 0x0001) != 0)
    {      
      crc >>= 1;          
      crc ^= 0xA001;
    }
    else                            
      crc >>= 1;
    }
  }
  return crc;   
}


PacketParser::PacketParser(const std::vector<uint8_t>&input)
{
    _damagePacketsCounter = 0;
    _input = input;
    _successfulPackets = 0;
}

void PacketParser::AddASuccessfulPacketToTheQueue(Packet*packet)
{
    _output.push_back(packet);
}

void PacketParser::StartProcess()
{
    _workingThread = std::thread([this]()->void{this->Run();}); 
}

void PacketParser::StopProceess()
{
    _workingThread.~thread();
}

void PacketParser::Run()
{
    while (true)
    {   
        if(_input.at(0) != ((uint8_t) 0xAA55 >> 8))
        {
            _mutex.lock();
            _input.erase(_input.begin());
            _mutex.unlock();
            continue;
        }

        if(_input.at(1) != 0xAA55 &0XFF)
        {
            _mutex.lock();
            _input.erase(_input.begin(), _input.begin() + 1);
            _mutex.unlock();
            continue;
        }
        
        uint16_t sizeOfData = (uint16_t) _input[5] << 8 | _input[6];

        uint16_t CRC = CRC16_2(_input, sizeOfData + 7);

        uint16_t CRCOfInput = (uint16_t) _input[sizeOfData + 7] << 8 | _input[sizeOfData + 8];

        if(CRC != CRCOfInput)
        {
            _mutex.lock();
            _input.erase(_input.begin(), _input.begin() + sizeOfData + 9);
            _mutex.unlock();
            continue;
        }

        std::vector<uint8_t>dataForPacket;
        
        for(int index = 0; index < sizeOfData + 9; ++index)
            dataForPacket.at(index) = _input.at(index + 9);
        

        Packet *packet = new Packet(dataForPacket);

        AddASuccessfulPacketToTheQueue(packet);

        _mutex.lock();
        _input.erase(_input.begin(), _input.begin() + sizeOfData + 9);
        _mutex.unlock();
    }
}
