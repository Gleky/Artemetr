#pragma once

//#include <string>
#include "iobserver.h"

class IComPort : public Publisher
{
public:
    virtual void sendMessage(char *) = 0;
    virtual const char *readMessage() = 0;
    virtual ~IComPort() {}
};
