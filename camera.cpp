#include "camera.h"
#include "icomport.h"

#include <QString>
#include <QDebug>


namespace PC {

char *convertToMessage(const Point point);
Point *convertToPoint( const char *message);


Camera::Camera()
{

}

void Camera::move(Point newPos)
{

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

void Camera::update()
{
    if (_port != nullptr) {
        const char *message = _port->readMessage();
        Point *newPosition = convertToPoint(message);
        if (newPosition != nullptr) {
            _currentPosition = *newPosition;
            delete newPosition;
            qDebug() << _currentPosition.X << ' ' << _currentPosition.Y;
        }
    }
}

void Camera::setComPort(IComPort *port)
{
    _port = port;
}


/////////////////// -- Converting -- ///////////////////////////

char *convertToMessage(const Point point)
{
    return nullptr;
}

Point *convertToPoint( const char *message)
{
    QString msg(message);
    QStringList messageParts = msg.split(' ');

    if (messageParts.size() >= 3 && messageParts[0] == "NEW_POS"){
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



