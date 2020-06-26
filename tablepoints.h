#pragma once

#include <QList>

#include "point.h"


class TablePoints
{
public:
    TablePoints();

    bool hasNextCheckPoint() const;
    const Point nextCheckPoint() const;
    void currentCheckIs( bool checkResult );

    bool hasNextTargetPoint() const;
    Point nextTargetPoint();
    bool packDone() const;

private:
    QList<Point> _packList;

    QList<Point> _packsForCheck;

    void getTargetPoints();
    QList<Point> _targetPoints;
};
