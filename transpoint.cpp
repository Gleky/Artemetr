#include "transpoint.h"
#include "point.h"

#include <QString>
#include <QStringList>
#include <QDebug>


char *TransPoint::convertToMessage(const Point point)
{
    return nullptr;
}

Point *TransPoint::convertToPoint( const char *message)
{
    QString msg(message);
    QStringList messageParts = msg.split(' ');

    if (messageParts.size() > 2 && messageParts[0] == "NEW_POS"){
        bool parsOkX = false;
        bool parsOkY = false;
        int x = messageParts[1].toInt(&parsOkX),
            y = messageParts[2].toInt(&parsOkY);
        if (parsOkX && parsOkY)
            return new Point{x,y};
    }
    return nullptr;
}
