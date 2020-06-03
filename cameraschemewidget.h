#ifndef CAMERASCHEMEWIDGET_H
#define CAMERASCHEMEWIDGET_H

#include "point.h"

#include <QWidget>

class CameraControl;

class CameraSchemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraSchemeWidget(QWidget *parent = nullptr);
    void setController(CameraControl *controller);

signals:
    void moveToPoint(Point newPos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    CameraControl *_controller = nullptr;
    void checkKeys();

    int _topBorder = 0,
        _leftBorder = 0,

        _pointSize = 4;

    double _scale = 1.0;

    bool buttonUp = false,
         buttonDown = false,
         buttonRight = false,
         buttonLeft = false;

    void stepUp(Point &newPos) const;
    void stepDown(Point &newPos) const;
    void stepLeft(Point &newPos) const;
    void stepRight(Point &newPos) const;
};

#endif // CAMERASCHEMEWIDGET_H
