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
    void moveX(int x) override;
    void moveY(int y) override;
    int currentX() const override;
    int currentY() const override;
    Point currentPos() const override;

    void updateSub() override;
    void setComPort(IComPort *port);

private:
    Point _currentPosition;
    Point _targetPosition;

    mutable QMutex mutex ;

    IComPort *_port = nullptr;
};

}
#endif // CAMERA_H
