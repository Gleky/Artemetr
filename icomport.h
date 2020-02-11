#pragma once

#include <string>
#include "observer.h"

class IComPort : public Publisher
{
public:
    virtual void sendMessage(std::string);
    virtual std::string readMessage();
    virtual ~IComPort();
};
