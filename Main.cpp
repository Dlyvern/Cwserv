#include<thread>
#include"Server.h"
#include"Client.h"

unsigned int CRC16_2(std::vector<uint8_t>&buf)
{  
  unsigned int crc = 0xFFFF;

  for (const auto&it : buf)
  {

  crc ^= (unsigned int)it;    // XOR byte into least sig. byte of crc

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

std::string ByteToHex(std::vector<uint8_t>&data)
{
    std::stringstream ss;

    for(int i = 0; i < data.size(); ++i)
        ss <<  "0x" << std::setfill('0') << std::setw(2)  << std::right << std::hex << (unsigned int) data[i] << ' ';

    std::string result;

    result = ss.str();

    return result;
}

void CreateServer()
{
    Server* server = new Server();
    server->Connection();
    delete(server);
}

int main()
{
    std::thread thServer(CreateServer);

    Client*client = new Client();

    client->Connection();
    
    delete(client);
    
    return {};
}
