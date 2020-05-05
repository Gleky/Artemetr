#pragma once

#include "iobserver.h"
#include "struct_result.h"


class IRobot
{
public:
    ~IRobot() {}

    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
};
