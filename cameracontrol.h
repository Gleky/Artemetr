#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include "iobserver.h"
#include "point.h"
#include "cameraschemewidget.h"

#include <QObject>
//#include <QPushButton>
//#include <QFrame>

class ICamera;

namespace PC {
class ComPort;
}


class CameraControl : public QObject, public Subscriber
{
    Q_OBJECT
public:
    CameraControl(ICamera *camera, const PC::ComPort *port);

    void moveTo(Point newPos);
    void moveToWihtoutSignal(Point newPos);
    void goHome();
    void lightOn();

    void publisherUpdated() override;

    Point currentPos() const;
    Point targetPos() const;
    bool isConnected() const;

signals:
    void cameraUpdated();
    void cameraReachedTargetPoint();

private slots:
    void cameraConnected();
    void cameraDisconnected();

private:
    ICamera *const _camera;

    bool _goingToNewPos = false;
    bool _cameraConnected = false;
};

Q_DECLARE_METATYPE(Point)

#endif // CAMERACONTROL_H
