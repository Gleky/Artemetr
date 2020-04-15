#ifndef MANUALCAMERACONTROL_H
#define MANUALCAMERACONTROL_H

#include "iobserver.h"
#include "point.h"
#include <QLabel>


class ICamera;
class QLabel;

class ManualCameraControl : public QLabel, public Subscriber
{
    Q_OBJECT
public:
    ManualCameraControl(QWidget *parent);
    ~ManualCameraControl();

    void setCamera(ICamera *camera);

    void updateSub() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void checkKeys();

private:
    ICamera *_camera = nullptr;
    Point _currentPos, _targetPos;

    int _topBorder = 0,
        _leftBorder = 0,
        _width = 0,
        _height = 0,
        _pointSize = 10;

    const int Hmm = 500;
    const int Wmm = 500;

    void stepUp(Point &newPos) const;
    void stepDown(Point &newPos) const;
    void stepLeft(Point &newPos) const;
    void stepRight(Point &newPos) const;
    void moveTo(const Point &newPos);

    enum Dimention {Y, X};
    int mappedValue(const Point &pos, Dimention dimention) const;

    bool buttonUp = false,
         buttonDown = false,
         buttonRight = false,
         buttonLeft = false;
};

#endif // MANUALCAMERACONTROL_H
