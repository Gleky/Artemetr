#pragma once

class IRobot
{
    ~IRobot() {}
    virtual void home() = 0;
    virtual void checkTable() = 0;

    virtual bool hasNext() const = 0;
    virtual void goNext() = 0;
};
