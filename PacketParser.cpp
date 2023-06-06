#include "PacketParser.h"

PacketParser::PacketParser(std::vector<uint8_t> *input, std::mutex *mutex)
{
    _damagePacketsCounter = 0;
    _input = input;
    _successfulPackets = 0;
    _run = false;
    _mutex = mutex;
}

void PacketParser::StartProcess()
{
    _run = true;
    _workingThread = std::thread([this]() -> void
                                 { this->Run(); });
}

void PacketParser::StopProceess()
{
    _run = false;
    _workingThread.~thread();
}

void PacketParser::Run()
{
    while (_run)
    {

        if (_input->size() < 9)
            continue;

        if (_input->at(0) != (uint8_t)(0XAA55 >> 8))
        {
            _mutex->lock();
            _input->erase(_input->begin());
            _mutex->unlock();
            continue;
        }
        if (_input->at(1) != (uint8_t)(0xAA55 & 0XFF))
        {
            _mutex->lock();
            _input->erase(_input->begin(), _input->begin() + 1);
            _mutex->unlock();
            continue;
        }

        uint16_t sizeOfData = {static_cast<uint16_t>(_input->at(5) << 8 | _input->at(6))};

        if (_input->size() < sizeOfData + 8)
            continue;

        std::vector<uint8_t> dataForCRC;

        for (int index = 0; index < 7 + sizeOfData; ++index)
            dataForCRC.push_back(_input->at(index));

        uint16_t CRC = CRC16::CRC16_2(dataForCRC);

        dataForCRC.clear();
        dataForCRC.shrink_to_fit();

        uint16_t CRCOfInput = {static_cast<uint16_t>(_input->at(sizeOfData + 6) << 8 | _input->at(sizeOfData + 7))};

        if (CRC != CRCOfInput)
        {
            _mutex->lock();
            _input->erase(_input->begin(), _input->begin() + sizeOfData + 7);
            _mutex->unlock();
            continue;
        }

        std::vector<uint8_t> dataForPacket;

        for (int index = 0; index < sizeOfData + 8; ++index)
            dataForPacket.push_back(_input->at(index));

        std::unique_ptr<Packet>packet(new Packet(dataForPacket));

       _output.emplace_back(std::move(packet));

        _mutex->lock();
        _input->erase(_input->begin(), _input->begin() + sizeOfData + 8);
        _mutex->unlock();

        dataForPacket.clear();
        dataForPacket.shrink_to_fit();
    }
}
