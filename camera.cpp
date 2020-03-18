#include "camera.h"
#include "icomport.h"
#include "keywords.h"

#include <QString>
#include <QDebug>


namespace PC {

QString convertToMessage(const Point point);
Point *convertToPoint( const char *message);


Camera::Camera()
{

}

void Camera::move(Point newPos)
{
    auto message = convertToMessage(newPos);
    _port->sendMessage( message.toStdString().c_str() );
}

void Camera::moveX(int x)
{

}

void Camera::moveY(int y)
{

}

int Camera::currentX() const
{
    return _currentPosition.X;
}

int Camera::currentY() const
{
    return _currentPosition.Y;
}

Point Camera::currentPos() const
{
    return _currentPosition;
}

void Camera::updateSub()
{
    if (_port != nullptr) {
        const char *message = _port->readMessage();
        Point *newPosition = convertToPoint(message);
        if (newPosition != nullptr) {
            _currentPosition = *newPosition;
            delete newPosition;
            qDebug() << _currentPosition.X << ' ' << _currentPosition.Y;
            notifySubscribers();
        }
    }
}

void Camera::setComPort(IComPort *port)
{
    _port = port;
}


/////////////////// -- Converting -- ///////////////////////////

QString convertToMessage(const Point point)
{
    QString ret = QString(moveTo) + " " + QString::number(point.X) + " " + QString::number(point.Y);
    return ret;
}

Point *convertToPoint( const char *message)
{
    QString msg(message);
    QStringList messageParts = msg.split(' ');

    if (messageParts.size() >= 3 && messageParts[0] == currentPosition){
        bool xParsOk = false;
        bool yParsOk = false;
        int x = messageParts[1].toInt(&xParsOk),
            y = messageParts[2].toInt(&yParsOk);
        if (xParsOk && yParsOk)
            return new Point{x,y};
    }
    return nullptr;
}

}



