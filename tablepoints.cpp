#include "tablepoints.h"

#include "keywords.h"

QList<Point> getPackList();

TablePoints::TablePoints()
{
    _packsForCheck = getPackList();
}

bool TablePoints::hasNextCheckPoint() const
{
    bool ret = false;

    if ( !_packsForCheck.isEmpty() )
        ret = true;

    return ret;
}

const Point TablePoints::nextCheckPoint() const
{
    Point checkPoint;

    if ( !_packsForCheck.isEmpty() )
    {
        auto currentPack = _packsForCheck.first();
//        constexpr int gap = (xMaxPos - 2*tableMargin - packWidth*horisontalPackCount)/(horisontalPackCount - 1);

        checkPoint.X = leftTableMargin + currentPack.X*(packWidth + xSpace) + packWidth/2;
        checkPoint.Y = botTableMargin + currentPack.Y*(packHeight + ySpace) + packHeight/2;
    }

    return checkPoint;
}

void TablePoints::currentCheckIs(bool checkResult)
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

void TablePoints::getTargetPoints()
{
    while ( !_packList.isEmpty() )
    {
        auto currentPackPosition = _packList.takeLast();
        bool rightToLeft = currentPackPosition.Y % 2;

        _targetPoints += packTargetPoints(currentPackPosition, rightToLeft);
    }
}

bool TablePoints::hasNextTargetPoint() const
{
    bool ret = false;

    if ( !_targetPoints.isEmpty() )
        ret = true;

    return ret;
}

Point TablePoints::nextTargetPoint()
{
    return _targetPoints.takeFirst();
}

bool TablePoints::packDone() const
{
    if ( ( _targetPoints.size() % (verticalCellCount*horisontalCellCount) ) == 0 )
        return true;
    return false;
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
//    constexpr int gap = (xMaxPos - 2*tableMargin - packWidth*horisontalPackCount)/(horisontalPackCount - 1);

    double startX = leftTableMargin + packPosition.X*(packWidth + xSpace);
    const double startY = botTableMargin + packPosition.Y*(packHeight + ySpace);

    double xStep =  static_cast<double>(packWidth)/(horisontalCellCount - 1);
    double yStep = static_cast<double>(packHeight)/(verticalCellCount - 1);

    if ( rightToLeftDirection )
    {
        startX += packWidth;
        xStep = -xStep;
    }
//    const Point startPoint(startX,startY);

    QList<Point> targetPoints;

    for ( int m = 0; m < horisontalCellCount; ++m)
    {
        int x = lround(startX + xStep*m);

        double localStartY = startY;
        if ( yStep < 0 )
            localStartY += packHeight;

        for ( int n = 0; n < verticalCellCount; ++n)
        {
            int y = lround(localStartY + yStep*n);
            targetPoints.append( Point(x,y) );
        }
        yStep = -yStep;
    }

    return targetPoints;
}

PackCornerPoints::PackCornerPoints(Point packPosition)
    :_cellPoints( packTargetPoints(packPosition, false) )
{

}

QList<Point> PackCornerPoints::points() const
{
    QList<Point> cornerPoints;
    cornerPoints.append( botLeft() );
    cornerPoints.append( topLeft() );
    cornerPoints.append( topRight() );
    cornerPoints.append( botRight() );
    return cornerPoints;
}

Point PackCornerPoints::topLeft() const
{
    return  _cellPoints[verticalCellCount-1];
}

Point PackCornerPoints::topRight() const
{
    return _cellPoints[verticalCellCount*horisontalCellCount - verticalCellCount];
}

Point PackCornerPoints::botLeft() const
{
    return _cellPoints[0];
}

Point PackCornerPoints::botRight() const
{
    return _cellPoints[verticalCellCount*horisontalCellCount -1];
}
