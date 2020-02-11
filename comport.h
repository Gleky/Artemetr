#pragma once

#include "icomport.h"

class ComPort : public IComPort
{
public:
    ComPort();

    void sendMessage(std::string);
    std::string readMessage();
};

