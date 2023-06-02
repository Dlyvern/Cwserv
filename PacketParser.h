#ifndef PACKET_PARSER_H
#define PACKET_PARSER_H
#include<vector>
#include<stdint.h>
#include"Packet.h"
#include<thread>
#include<mutex>

class PacketParser
{
private:
    std::vector<uint8_t>*_input;
    std::vector<Packet*>_output;
    std::thread _workingThread;
    std::mutex *_mutex;
    uint64_t _damagePacketsCounter;
    uint64_t _successfulPackets;
    bool _run;

    void Run();

    void AddASuccessfulPacketToTheQueue(Packet*packet);
public:
    PacketParser(std::vector<uint8_t>*input, std::mutex *mutex);

    void StartProcess();

    void StopProceess();
};

#endif
