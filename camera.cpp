#include "camera.h"
#include "icomport.h"
#include "transpoint.h"

#include <QString>
#include <QDebug>

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
        Point *newPosition = TransPoint::convertToPoint(message);
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
