#include "Server.h"

bool Server::_work;
unsigned int Server::_clientId;

Server::Server(int port, std::string ip, int maxNumberOfConnections) : _port(port), _addrInfo{0}, _sock(-1), _maxNumberOfConnections(maxNumberOfConnections)
{
    _ip = ip.c_str();
    _sizeAddrInfo = sizeof(_addrInfo);
    _clientId = 0;
    _work = false;
}

void Server::Initialization()
{
    const int opt = 1;

    bzero((char *)&_addrInfo, sizeof(_addrInfo));

    _addrInfo.sin_family = AF_INET;
    _addrInfo.sin_port = htons(_port);
    _addrInfo.sin_addr.s_addr = inet_addr(_ip);

    _sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        exit(EXIT_FAILURE);

    if (bind(_sock, (sockaddr *)&_addrInfo, _sizeAddrInfo) < 0)
        exit(EXIT_FAILURE);

    if ((listen(_sock, _maxNumberOfConnections)) == -1)
        exit(EXIT_FAILURE);

    _work = true;
}

void Server::Start()
{
    Initialization();

    _acceptThread = std::thread([this]()
                                { AcceptingClients(); });
}

void Server::AcceptingClients()
{
    while (_work)
    {
        if(_clients.size() >= _maxNumberOfConnections)
            continue;

        int clientSocket = accept(_sock, (struct sockaddr *)&_addrInfo, &_sizeAddrInfo);

        if (clientSocket != -1 && _clients.size() < _maxNumberOfConnections)
        {
            std::unique_ptr<Client> client(new Client(_clientId, _port, "192.168.31.233"));    
            _clientId++;
            client->Start();
             _clients.push_back((std::move(client)));
            _receivingMessagesThread.emplace_back(std::thread([this, clientSocket]()
                                                              { this->ReceivingMessages(clientSocket); }));
        }
    }
}

void Server::ReceivingMessages(int newSocket)
{
    unsigned int bytesReceived = 0;
    std::vector<uint8_t> data;
    uint8_t x;
    std::unique_ptr<PacketParser> packetParser{new PacketParser(&data, &_mutex)};
    // packetParser->StartProcess();

    while (_work)
    {
        int result = recv(newSocket, &x, 1, 0);

        if (result != -1)
        {
            _mutex.lock();
            data.push_back(x);
            _mutex.unlock();
            bytesReceived += result;
        }
    }
}

bool Server::DisconnectClient(int id)
{
    for (int index = 0; index < _clients.size(); ++index)
    {
        if (_clients.at(index)->GetId() == id)
        {
            _clients.at(index)->Disconnect();
            return true;
        }
    }

    return false;
}

void Server::DisconnectClients()
{
    for (int index = 0; index < _clients.size(); ++index)
        _clients.at(index)->Disconnect();
}

void Server::Stop()
{
    _work = false;
    _acceptThread.~thread();
    for (int i = 0; i < _receivingMessagesThread.size(); ++i)
        _receivingMessagesThread.at(i).~thread();
    close(_sock);
    _clients.clear();
    _clientId = 0;
}

Server::~Server()
{
    _acceptThread.~thread();
    for (int i = 0; i < _receivingMessagesThread.size(); ++i)
        _receivingMessagesThread.at(i).~thread();
    close(_sock);
}
