#include "robot.h"

#include "camerawidget.h"
#include "keywords.h"

#include <QApplication>

Robot::Robot()
{
    connect(&_imageAnalyzer, &ImageAnalyzer::resultReady, this, &Robot::resultReady, Qt::QueuedConnection);
}

Robot::~Robot()
{}

void Robot::setCameraControl(CameraControl *camera)
{
    _cameraController = camera;
    connect(_cameraController, &CameraControl::cameraReachedTargetPoint, this, &Robot::cameraAtTargetPoint, Qt::QueuedConnection);
}

void Robot::setCameraView(CameraWidget *cameraWidget)
{
    _cameraWidget = cameraWidget;
    connect(&cameraWidget->_capture, &QCameraImageCapture::imageCaptured, this, &Robot::imageCaptured);
    connect(this, &Robot::result, _cameraWidget, &CameraWidget::showResult, Qt::QueuedConnection);
}

void Robot::prepareToClose()
{
    if ( !_cameraController->isConnected() ){
        QApplication::exit();
        return;
    }

    _cameraController->setBacklight(false);
    _cameraController->moveCamera(Point(xMaxPos-10,yMaxPos-10));

    _state = Closing;
}

void Robot::start()
{
    _cameraController->setBacklight(true);

    if ( !(_state == Started || _state == Paused) )
        findTargetPoints();

    _state = Started;
    getNext();
}

void Robot::pause()
{
    _state = Paused;

    if ( _state == Paused )
        getNext();
}

void Robot::stop()
{
    _state = Stoped;
    _targetPoints.clear();
}

void Robot::getNext()
{
    if (_targetPoints.size() > 0) {
        _cameraController->moveCamera(_targetPoints.first());
    } else {
        _state = Stoped;
    }
}

void Robot::cameraAtTargetPoint()
{
    if ( _state == Stoped || _state == Closing) {
        if ( _state == Closing )
            QApplication::exit();
        return;
    }
    _cameraWidget->_capture.capture(); //KOSTIL'
    _targetPoints.removeFirst();
}

void Robot::imageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id)
    _imageAnalyzer.getResult(preview);
}

void Robot::resultReady(Result res)
{
    emit result(res);

    if ( _state == Started )
        getNext();
}

void Robot::findTargetPoints()
{
    _targetPoints.clear();
    _targetPoints.append(Point(0,0));
    _targetPoints.append(Point(15,0));
    _targetPoints.append(Point(30,0));
    _targetPoints.append(Point(45,0));
    _targetPoints.append(Point(0,15));
    _targetPoints.append(Point(15,15));
    _targetPoints.append(Point(30,15));
    _targetPoints.append(Point(45,15));
}
