#pragma once

#include <string>
#include "point.h"

class TransPoint
{
public:
    TransPoint();
    bool operator == (TransPoint &anotherPoint);
    void operator = (Point& point);

    std::string convertToMessage();
    static TransPoint convertToPoint(std::string message);

private:
    Point point;
};

