#ifndef MANUALCAMERACONTROL_H
#define MANUALCAMERACONTROL_H

#include "iobserver.h"
#include "point.h"
#include <QLabel>


class ICamera;
class QLabel;

class ManualCameraControl : public QLabel, public Subscriber
{
public:
    ManualCameraControl(QWidget *parent);
    ~ManualCameraControl();

    void setCamera(ICamera *camera);

    void updateSub() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    ICamera *_camera = nullptr;
    Point _currentPos, _targetPos;
};

#endif // MANUALCAMERACONTROL_H
