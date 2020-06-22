#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include "iobserver.h"
#include "point.h"
#include "cameraschemewidget.h"

#include <QPushButton>
#include <QFrame>

class ICamera;
class QLabel;

class CameraControl : public QFrame, public Subscriber
{
    Q_OBJECT
public:
    CameraControl(QWidget *parent = nullptr);
    ~CameraControl();

    void setCamera(ICamera *camera);
    void moveCamera(Point newPos);
    void goHome();
    void lightOn();

    void publisherUpdated() override;

    Point currentPos() const;
    Point targetPos() const;
    bool isConnected() const;

signals:
    void cameraReachedTargetPoint();

public slots:
    void cameraConnected();
    void cameraDisconnected();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void moveToPoint(const Point &newPos);
    void lightButtonToggled(bool checked);

private:
    ICamera *_camera = nullptr;
    CameraSchemeWidget _scheme;
    QPushButton _lightButton;

    bool _goingToNewPos = false;
    bool _cameraConnected = false;
};

Q_DECLARE_METATYPE(Point)

#endif // CAMERACONTROL_H
