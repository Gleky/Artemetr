#pragma once

#include "observer.h"

struct Point;
class IComPort;

class ICamera : public Publisher, public Subscriber
{
public:
    virtual void move( Point newPos ) = 0;
    virtual void moveX( int x ) = 0;
    virtual void moveY( int y ) = 0;
    virtual int currentX() = 0;
    virtual int currentY() = 0;
    virtual Point currentPos() = 0;

    virtual void setComPort(IComPort *port) = 0;
};
