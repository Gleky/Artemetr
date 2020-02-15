#pragma once

#include "iobserver.h"

struct Point;
class IComPort;

class ICamera : public Publisher, public Subscriber
{
public:
    virtual void move( Point newPos ) = 0;
    virtual void moveX( int x ) = 0;
    virtual void moveY( int y ) = 0;
    virtual int currentX() const = 0;
    virtual int currentY() const = 0;
    virtual Point currentPos() const = 0;
};
