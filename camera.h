#ifndef CAMERA_H
#define CAMERA_H

#include "icamera.h"
#include "point.h"
#include <QMutex>

class IComPort;

namespace PC {

class Camera : public ICamera
{
public:
    Camera();

    void move(Point newPos) override;
    Point currentPos() const override;
    Point targetPos() const override;

    void setBacklight(BacklightState state) override;

    void publisherUpdated() override;
    void setComPort(IComPort *port);

private:
    Point _currentPosition;
    Point _targetPosition = {-1,-1};

    mutable QMutex mutex ;

    IComPort *_port = nullptr;
};

}
#endif // CAMERA_H
