#include "tablepoints.h"

#include "keywords.h"

TabelPoints::TabelPoints()
{
    reset();
    for (auto unused : _packList) //test
        nextCheckPoint(); //test
}

bool TabelPoints::hasNextCheckPoint() const
{
    bool ret = false;

    if ( !_packsForCheck.isEmpty() )
        ret = true;

    return ret;
}

const Point TabelPoints::nextCheckPoint()
{
    Point checkPoint;

    if ( !_packsForCheck.isEmpty() )
    {
        _currentPack = _packsForCheck.takeFirst();
        constexpr int gap = (xMaxPos - 2*tableMargin - packWidth*horisontalPackCount)/(horisontalPackCount - 1);

        checkPoint.X = tableMargin + _currentPack.X*(packWidth + gap) + packWidth/2;
        checkPoint.Y = tableMargin + _currentPack.Y*(packHeight + gap) + packHeight/2;
    }

    return checkPoint;
}

void TabelPoints::currentCheckIs(bool checkResult)
{
    if ( !checkResult )
    {
        _packList.removeOne(_currentPack);
    }
}

bool TabelPoints::hasNextTargetPoint() const
{
    bool ret = false;
    return ret;
}

Point TabelPoints::nextTargetPoint()
{
    Point ret;
    return ret;
}

void TabelPoints::reset()
{
    int step = -1;

    int currentX = horisontalPackCount;
    int currentY = verticalPackCount;

    for ( int m = 0; m < verticalPackCount; ++m)
    {
        currentY -= 1;
        for ( int n = 0; n < horisontalPackCount; ++n)
        {
            currentX += step;
            _packList.append( Point(currentX,currentY) );
        }
        currentX += step;
        step = -step;
    }

    _packsForCheck = _packList;
}

void TabelPoints::getTargetPoints()
{
    //start from end of pack sequence

    //find target points depends on pack position
}
