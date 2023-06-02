#ifndef CRC16_H

#include <vector>
#include <stdint.h>

class CRC16
{
public:
    static unsigned int CRC16_2(std::vector<uint8_t> &buf);
};

#endif
