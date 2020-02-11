#pragma once

#include <string>

class Point
{
public:
    Point();
    std::string convertToMessage();
    static Point convertToPoint(std::string message);

private:
    int x = 0;
    int y = 0;
};

