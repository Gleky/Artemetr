#pragma once

#include <string>

struct Point;


class TransPoint
{
public:

//    bool operator == (TransPoint &anotherPoint);
//    void operator = (Point& point);

    static char *convertToMessage(const Point point);
    static Point *convertToPoint(const char *message);

private:
    TransPoint();
    TransPoint(TransPoint &);
};

