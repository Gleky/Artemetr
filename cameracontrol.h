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
//class QLabel;

class CameraControl : public QObject, public Subscriber /*public QFrame,*/
{
    Q_OBJECT
public:
    CameraControl(ICamera *camera, const PC::ComPort *port/*, QWidget *parent = nullptr*/);

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

//protected:
//    void resizeEvent(QResizeEvent *event) override;

//private slots:
//    void moveToPoint(const Point &newPos);
//    void lightButtonToggled(bool checked);

private:
    ICamera *const _camera = nullptr;
//    CameraSchemeWidget _scheme;
//    QPushButton _lightButton;

    bool _goingToNewPos = false;
    bool _cameraConnected = false;
};

Q_DECLARE_METATYPE(Point)

#endif // CAMERACONTROL_H
