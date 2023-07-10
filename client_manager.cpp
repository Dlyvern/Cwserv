#include"ClientManager.h"

void ClientManager::AddClient(uint16_t& id, std::unique_ptr<Client>client)
{
    _clients.AddNewClient(id, std::move(client));
    _futureService.push_back(std::async(std::launch::async, [this]{ClientService();}));
}

SafeVectorPackets& ClientManager::GetSafeVector()
{
    return _packetsForWorkers;
}

void ClientManager::ClientService()
{

}

void ClientManager::SendToWorkers()
{

}

void ClientManager::SendAnswer(const std::vector<std::shared_ptr<Packet>> &answerPackets)
{

}

bool ClientManager::DisconnectClient(uint16_t id)
{
    auto client = _clients.GetClient(id);
    if(client == nullptr) return false;
    client->Disconnect();
    _clients.DeleteClient(client->GetId());
    return true;
}

void ClientManager::DisconnectAllClients()
{
    auto client = _clients.GetClient();

    while(client != nullptr)
    {
        client->Disconnect();
        _clients.DeleteClient(client->GetId());
        client = _clients.GetClient();
    }
}

void ClientManager::Work()
{

}
