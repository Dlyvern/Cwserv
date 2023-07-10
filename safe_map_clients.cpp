#include "SafeMapClients.h"

SafeMapClients::SafeMapClients() = default;

void SafeMapClients::AddNewClient(uint16_t id, const std::shared_ptr<Client> &client)
{
    std::lock_guard<std::mutex>lockGuard(_mutex);

    _map.insert(std::make_pair(id, client));
}

uint16_t SafeMapClients::GetSize() const
{
    std::lock_guard<std::mutex>lockGuard(_mutex);
    return _map.size();
}

std::shared_ptr<Client> SafeMapClients::GetClient(uint16_t id)
{
    std::lock_guard<std::mutex>lockGuard(_mutex);
    auto client = _map.find(id);
    if(client == _map.end()) return nullptr;
    return client->second;
}

std::shared_ptr<Client> SafeMapClients::GetClient()
{
    std::lock_guard<std::mutex>lockGuard(_mutex);
   if(_map.empty()) return nullptr;
   return _map.begin()->second;
}

void SafeMapClients::DeleteClient(uint16_t id)
{
    std::lock_guard<std::mutex>lockGuard(_mutex);

    auto client = _map.find(id);

    if(client == _map.end()) return;

    _map.erase(client);
}
