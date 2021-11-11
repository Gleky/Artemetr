#include "cameracontrol.h"
#include "icamera.h"
#include "comport.h"

#include <keywords.h>

CameraControl::CameraControl(ICamera *camera, PC::ComPort *port)
    : _camera(camera),
      _port(port)
{
    connect(port, &PC::ComPort::connected, this, &CameraControl::cameraConnected);
    connect(port, &PC::ComPort::disconnected, this, &CameraControl::cameraDisconnected);
}

void CameraControl::moveTo(Point newPos)
{
    moveToWihtoutSignal(newPos);
#ifndef TEST
    _goingToNewPos = true;
#else
    emit cameraReachedTargetPoint();
#endif
}

void CameraControl::moveToWihtoutSignal(Point newPos)
{
    if ( _goingToNewPos )
        return;

    _camera->move(newPos);
    emit cameraUpdated();
}

void CameraControl::goHome()
{
    _goingToNewPos = false;
    moveTo( Point(homeX,homeY) );
    _camera->setBacklight(Off);
}

void CameraControl::lightOn()
{
    if ( !_cameraConnected )
        return;
    _camera->setBacklight(On);
}

void CameraControl::lightOff()
{
    if ( !_cameraConnected )
        return;
    _camera->setBacklight(Off);
}

void CameraControl::putIodine(int workTime, int afterWorkTime)
{
    char msg[commandSize];
    *msg = iodinePumpSign;
    int16_t *wt = reinterpret_cast<int16_t *>(msg+xPosIndex);
    int16_t *awt = reinterpret_cast<int16_t *>(msg+yPosIndex);
    *wt = static_cast<int16_t>(workTime);
    *awt = static_cast<int16_t>(afterWorkTime);
    _port->sendMessage(msg);
}

void CameraControl::putChlorine(int workTime, int afterWorkTime)
{
    char msg[commandSize];
    *msg = chlorinePumpSign;
    int16_t *wt = reinterpret_cast<int16_t *>(msg+xPosIndex);
    int16_t *awt = reinterpret_cast<int16_t *>(msg+yPosIndex);
    *wt = static_cast<int16_t>(workTime);
    *awt = static_cast<int16_t>(afterWorkTime);
    _port->sendMessage(msg);
}

void CameraControl::publisherUpdated()
{
    if ( _camera == nullptr )
        return;
    if ( _goingToNewPos ) {
        auto currentPos = _camera->currentPos();
        auto targetPos = _camera->targetPos();
        if ( currentPos == targetPos ) {
            _goingToNewPos = false;
            emit cameraReachedTargetPoint();
        }
    }
    emit cameraUpdated();
}

Point CameraControl::currentPos() const
{
    if ( _camera == nullptr )
        return Point();

    return _camera->currentPos();
}

Point CameraControl::targetPos() const
{
    if ( _camera == nullptr )
        return Point();

    return _camera->targetPos();
}

bool CameraControl::isConnected() const
{
    return _cameraConnected;
}

void CameraControl::cameraConnected()
{
    _cameraConnected = true;
    emit cameraUpdated();
}

void CameraControl::cameraDisconnected()
{
    _cameraConnected = false;
    emit cameraUpdated();
}
