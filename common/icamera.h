#pragma once

#include "iobserver.h"

enum BacklightState:unsigned char {On, Off};
struct Point;

class ICamera : public Publisher, public Subscriber
{
public:
    virtual void move( Point newPos ) = 0;
    virtual Point currentPos() const = 0;
    virtual Point targetPos() const = 0;

    virtual void setBacklight(BacklightState state) = 0;
};
