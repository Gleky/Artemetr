#include "camerawidget.h"

#include <QCameraInfo>
#include <QMessageBox>

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    connect(&_showResultTimer, &QTimer::timeout, this, &CameraWidget::hideResult);
    _showResultTimer.setInterval(2500);

    _viewfinder.setParent(this);
    _resultLabel.setParent(this);

    auto cameras = QCameraInfo::availableCameras();

    for (auto camera : cameras)
    {
        _camera = new QCamera(camera);
        _camera->setViewfinder(&_viewfinder);
        _camera->load();
        auto resolutions = _camera->supportedViewfinderResolutions();
        QSize needResolution = {0,0};

        for ( auto res : resolutions)
        {
#if defined (TEST) || defined (TEST_WITH_ARDUINO)
            if ( res.height() == 720 && res.width() == 1280)
#else
            if ( res.height() == 960 && res.width() == 1280)
#endif
                needResolution = res;
        }

        if ( needResolution.height() > 0)
        {
            QCameraViewfinderSettings settings;
            settings.setResolution(needResolution);
            _camera->setViewfinderSettings(settings);
            break;
        }
        else
        {
            _camera->deleteLater();
            _camera = nullptr;
        }
    }

    if ( _camera != nullptr )
    {
        _capture = new QCameraImageCapture(_camera);
        _capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
        _camera->start();
        _viewfinder.show();

        connect(_capture, &QCameraImageCapture::imageCaptured, this, &CameraWidget::imageReady);
    }
    else
    {
        QMessageBox::warning(this, "Camera not found", "Please connect camera and restart app");
    }
}

void CameraWidget::capture() const
{
    if ( _capture != nullptr )
        _capture->capture();
}

void CameraWidget::imageReady(int id, const QImage &image)
{
    emit imageCaptured(id, image);
}


void CameraWidget::showResult(CellImage result)
{
    _resultLabel.setPixmap(QPixmap::fromImage(result.img).scaled(_resultLabel.size(),Qt::KeepAspectRatio));
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


