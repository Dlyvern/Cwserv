#include "CRC16.h"

unsigned int CRC16::CRC16_2(std::vector<uint8_t> &buf)
{
    unsigned int crc = 0xFFFF;

    for (const auto &it : buf)
    {

        crc ^= (unsigned int)it; 

        for (int i = 8; i != 0; i--)
        { // Loop over each bit
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
