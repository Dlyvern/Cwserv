#include <thread>
#include "Server.h"
#include "Client.h"

std::string ByteToHex(std::vector<uint8_t> &data)
{
  std::stringstream ss;

  for (int i = 0; i < data.size(); ++i)
    ss << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (unsigned int)data[i] << ' ';

  std::string result;

  result = ss.str();

  return result;
}

void ClientThread()
{
  Client *client = new Client();
  client->Connection();
}

void ServerThread()
{
  Server *server = new Server();
  server->Connection();
}

int main()
{
  std::thread th2(ServerThread);
  th2.detach();
  
  std::thread th(ClientThread);
  th.join();
  
  return {};
}
