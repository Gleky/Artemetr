#include "camerawidget.h"

#include <QCameraInfo>

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent),
      _capture(&_camera)
{
    connect(&_showResultTimer, &QTimer::timeout, this, &CameraWidget::hideResult);
    _showResultTimer.setInterval(2500);

    _viewfinder.setParent(this);
    _resultLabel.setParent(this);

    _capture.setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    _camera.setViewfinder(&_viewfinder);
    _camera.load();
    auto resolutions = _camera.supportedViewfinderResolutions();
    QCameraViewfinderSettings settings;
    settings.setResolution(resolutions.last());
    _camera.setViewfinderSettings(settings);
    _camera.start();
    _viewfinder.show();
}

void CameraWidget::showResult(Result result)
{
    _resultLabel.setPixmap(QPixmap::fromImage(result.result).scaled(_resultLabel.size(),Qt::KeepAspectRatio));
    _resultLabel.show();
    _viewfinder.hide();

    _showResultTimer.start();
}

void CameraWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    _viewfinder.resize(size());
    _resultLabel.setGeometry(_viewfinder.geometry());
}

void CameraWidget::hideResult()
{
    _viewfinder.show();
    _resultLabel.hide();
}
