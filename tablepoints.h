#pragma once

#include <QList>

#include "point.h"


class TabelPoints
{
public:
    TabelPoints();

    bool hasNextCheckPoint() const;
    const Point nextCheckPoint();
    void currentCheckIs( bool checkResult );

    bool hasNextTargetPoint() const;
    Point nextTargetPoint();

    void reset();

private:
    QList<Point> _packList;
    Point _currentPack;

    QList<Point> _packsForCheck;

    void getTargetPoints();
    QList<Point> _targetPoints;
};
