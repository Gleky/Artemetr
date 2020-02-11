#pragma once

#include <string>

class IComPort
{
    virtual void sendMessage(std::string);
    virtual void readMessage();
    virtual ~IComPort();

};
