#include "camera.h"
#include "icomport.h"
#include "keywords.h"

#include <QString>
#include <QDebug>


namespace PC {

void convertToMessage(const Point point, char *msg);
Point convertToPoint( const char *message);


Camera::Camera()
{}

void Camera::move(Point newPos)
{
    _targetPosition = newPos;

    char message[commandSize] = {0};
    convertToMessage(newPos, message);
    _port->sendMessage( message );
}

Point Camera::currentPos() const
{
    return _currentPosition;
}

Point Camera::targetPos() const
{
    return  _targetPosition;
}

void Camera::setBacklight(BacklightState state)
{
    if ( _port == nullptr )
        return;

    char msg[5] = {0};
    *msg = setBacklightState;
    BacklightState *st = reinterpret_cast<BacklightState*>(msg+backlightStateIndex);
    *st = state;
    _port->sendMessage(msg);
}

void Camera::publisherUpdated()
{
    if (_port == nullptr)
        return;
    const char *message = _port->readMessage();
    Point newPosition = convertToPoint(message);

    if ( newPosition == Point() )
        return;

    if ( newPosition == _currentPosition && _currentPosition != _targetPosition )
        move(_targetPosition);

    _currentPosition = newPosition;

    bool movingDone = _currentPosition == _targetPosition;
    static bool movingDoneUpdated = false;
    if ( !movingDone ) {
        notifySubscribers();
        movingDoneUpdated = false;
    } else
        if ( !movingDoneUpdated ){
            notifySubscribers();
            movingDoneUpdated = true;
        }
}

void Camera::setComPort(IComPort *port)
{
    _port = port;
}


/////////////////// -- Converting -- ///////////////////////////

void convertToMessage(const Point point, char *msg)
{
    *msg = moveCamTo;
    ++msg;
    int16_t *x = reinterpret_cast<int16_t *>(msg);
    int16_t *y = reinterpret_cast<int16_t *>(msg+2);
    *x = static_cast<int16_t>(point.X);
    *y = static_cast<int16_t>(point.Y);
}

Point convertToPoint( const char *message)
{
    if ( message[0] == currentCamPosition ){
        int16_t x = *reinterpret_cast<const int16_t *>( message+xPosIndex );
        int16_t y = *reinterpret_cast<const int16_t *>( message+yPosIndex );
        return Point{x,y};
    }
    return Point();
}

} //namespace PC
