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

    void capture() const;

public slots:
    void showResult(CellImage result);

signals:
    void imageCaptured(int id, const QImage &image);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void hideResult();
    void imageReady(int id, const QImage &image);

private:
    QCamera *_camera = nullptr;
    QCameraViewfinder _viewfinder;
    QLabel _resultLabel;
    QTimer _showResultTimer;
    QCameraImageCapture *_capture = nullptr;
};

#endif // CAMERAWIDGET_H
