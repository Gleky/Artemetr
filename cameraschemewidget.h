#ifndef CAMERASCHEMEWIDGET_H
#define CAMERASCHEMEWIDGET_H

#include "point.h"

#include <QWidget>
#include <QPushButton>

class CameraControl;
class ManualControlWidget;

class CameraSchemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraSchemeWidget(CameraControl *camera, ManualControlWidget *manualControl, QWidget *parent = nullptr);

public slots:
    void updateScheme();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    CameraControl *const _camera;
    ManualControlWidget *const _manualControl;

//    QPushButton _settingsButton;

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
