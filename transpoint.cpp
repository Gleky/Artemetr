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
    qDebug() << msg;
    QStringList messageParts = msg.split(' ');
    if (messageParts.size() > 2 && messageParts[0] == "NEW_POS"){
        return new Point{3,5};
    }
    return nullptr;
}
