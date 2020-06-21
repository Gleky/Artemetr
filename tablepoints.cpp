#include "tablepoints.h"

#include "keywords.h"

QList<Point> getPackList();

TabelPoints::TabelPoints()
{
    _packsForCheck = getPackList();

    while ( hasNextCheckPoint() ) //test
        currentCheckIs(true);     //test
}

bool TabelPoints::hasNextCheckPoint() const
{
    bool ret = false;

    if ( !_packsForCheck.isEmpty() )
        ret = true;

    return ret;
}

const Point TabelPoints::nextCheckPoint() const
{
    Point checkPoint;

    if ( !_packsForCheck.isEmpty() )
    {
        auto currentPack = _packsForCheck.first();
        constexpr int gap = (xMaxPos - 2*tableMargin - packWidth*horisontalPackCount)/(horisontalPackCount - 1);

        checkPoint.X = tableMargin + currentPack.X*(packWidth + gap) + packWidth/2;
        checkPoint.Y = tableMargin + currentPack.Y*(packHeight + gap) + packHeight/2;
    }

    return checkPoint;
}

void TabelPoints::currentCheckIs(bool checkResult)
{
    if ( _packsForCheck.isEmpty() )
        return;

    if ( checkResult )
    {
        _packList.append( _packsForCheck.takeFirst() );
    }
    else
    {
        _packsForCheck.removeFirst();
    }

    if ( _packsForCheck.isEmpty() )
        getTargetPoints();
}

QList<Point> packTargetPoints(Point packPosition, bool rightToLeftDirection);

void TabelPoints::getTargetPoints()
{
    while ( !_packList.isEmpty() )
    {
        auto currentPackPosition = _packList.takeLast();
        bool rightToLeft = currentPackPosition.Y % 2;

        _targetPoints += packTargetPoints(currentPackPosition, rightToLeft);
    }
}

bool TabelPoints::hasNextTargetPoint() const
{
    bool ret = false;

    if ( !_targetPoints.isEmpty() )
        ret = true;

    return ret;
}

Point TabelPoints::nextTargetPoint()
{
    return _targetPoints.takeFirst();
}




QList<Point> getPackList()
{
    int step = -1;

    QList<Point> packList;

    int currentX = horisontalPackCount;
    int currentY = verticalPackCount;

    for ( int m = 0; m < verticalPackCount; ++m)
    {
        currentY -= 1;
        for ( int n = 0; n < horisontalPackCount; ++n)
        {
            currentX += step;
            packList.append( Point(currentX,currentY) );
        }
        currentX += step;
        step = -step;
    }

    return packList;
}

QList<Point> packTargetPoints(Point packPosition, bool rightToLeftDirection)
{
    constexpr int gap = (xMaxPos - 2*tableMargin - packWidth*horisontalPackCount)/(horisontalPackCount - 1);

    int startX = tableMargin + packPosition.X*(packWidth + gap);
    const int startY = tableMargin + packPosition.Y*(packHeight + gap);

    double xStep =  static_cast<double>(packWidth)/(horisontalCellCount - 1);
    double yStep = static_cast<double>(packHeight)/(verticalCellCount - 1);

    if ( rightToLeftDirection )
    {
        startX += packWidth;
        xStep = -xStep;
    }
    const Point startPoint(startX,startY);

    QList<Point> targetPoints;

    for ( int m = 0; m < horisontalCellCount; ++m)
    {
        int x = startPoint.X + lround(xStep*m);

        int localStartY = startPoint.Y;
        if ( yStep < 0 )
            localStartY += packHeight;

        for ( int n = 0; n < verticalCellCount; ++n)
        {
            int y = localStartY + lround(yStep*n);
            targetPoints.append( Point(x,y) );
        }
        yStep = -yStep;
    }

    return targetPoints;
}
