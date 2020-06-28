#ifndef MANUALCONTROLWIDGET_H
#define MANUALCONTROLWIDGET_H

#include <QWidget>

class CameraControl;

class ManualControlWidget : public QWidget
{
    Q_OBJECT
public:
    ManualControlWidget(CameraControl *cameraController, QWidget *parent = nullptr);

signals:

private slots:
    void goToCorner();
    void indexCanged(int index);

private:
    void setupWidget();

    CameraControl *const _camera = nullptr;
    int _packIndex = 1;
};

#endif // MANUALCONTROLWIDGET_H
