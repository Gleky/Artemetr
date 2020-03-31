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
    void resizeEvent(QResizeEvent *event) override;

private:
    ICamera *_camera = nullptr;
    Point _currentPos, _targetPos;

    int _topBorder = 0,
        _leftBorder = 0,
        _width = 0,
        _height = 0,
        _pointSize = 10;

    const int Hmm = 300;
    const int Wmm = 300;

    void stepUp();
    void stepDown();
    void stepLeft();
    void stepRight();

    enum Dimention {Y, X};
    int mappedValue(const Point &pos, Dimention dimention);
};

#endif // MANUALCAMERACONTROL_H
