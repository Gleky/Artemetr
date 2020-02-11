#ifndef CAMERA_H
#define CAMERA_H

#include "icamera.h"
#include "transpoint.h"


class Camera : public ICamera
{
public:
    Camera();

    void move(Point newPos) override;
    void moveX(int x) override;
    void moveY(int y) override;
    int currentX() override;
    int currentY() override;
    Point currentPos() override;

    void update() override;
    void setComPort(IComPort *port) override;

private:
    TransPoint _currentPosition;
    TransPoint _targetPosition;

    IComPort *_port;
};

#endif // CAMERA_H
