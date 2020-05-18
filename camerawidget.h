#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QLabel>
#include <QTimer>

#include "struct_result.h"

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);

public slots:
    void showResult(Result result);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void hideResult();

private:
    QCamera *_camera = nullptr;
    QCameraViewfinder _viewfinder;
    QLabel _resultLabel;
    QTimer _showResultTimer;
public:
    QCameraImageCapture *_capture = nullptr;
};

#endif // CAMERAWIDGET_H
