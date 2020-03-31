#pragma once

struct Point
{
    Point() {}
    Point (int x, int y) {X = x; Y = y;}
    int X = 0;
    int Y = 0;

    bool operator == (const Point &anotherPoint) const
    {
        if (this->X == anotherPoint.X && this->Y == anotherPoint.Y)
            return true;
        return false;
    }

    void operator = (const Point& anotherPoint)
    {
        X = anotherPoint.X;
        Y = anotherPoint.Y;
    }
};
