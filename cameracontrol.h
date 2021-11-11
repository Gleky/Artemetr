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
    CameraControl(ICamera *camera, PC::ComPort *port);

    void moveTo(Point newPos);
    void moveToWihtoutSignal(Point newPos);
    void goHome();
    void lightOn();
    void lightOff();

    void putIodine(int workTime, int afterWorkTime);
    void putChlorine(int workTime, int afterWorkTime);

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
    PC::ComPort *const _port;

    bool _goingToNewPos = false;
    bool _cameraConnected = false;
};

Q_DECLARE_METATYPE(Point)

#endif // CAMERACONTROL_H
