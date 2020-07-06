#include "robot.h"

#include "camerawidget.h"
#include "keywords.h"

#include <QApplication>
#include <QFile>

// NEED TO SPLIT INTO STATE OBJECTS !!!

Robot::Robot(CameraControl *camera, CameraWidget *cameraWidget)
    :_cameraController(camera),
      _cameraWidget(cameraWidget)
{
    connect(&_imageAnalyzer, &ImageAnalyzer::resultReady, this, &Robot::resultReady, Qt::QueuedConnection);
    connect(&_imageAnalyzer, &ImageAnalyzer::packPresence, this, &Robot::packPresence, Qt::QueuedConnection);
#ifndef TEST
    _delay.setInterval(1000);
#else
    _delay.setInterval(300);
#endif
    _delay.setSingleShot(true);

    connect(_cameraController, &CameraControl::cameraReachedTargetPoint, &_delay, qOverload<>(&QTimer::start), Qt::QueuedConnection);
    connect(&_delay, &QTimer::timeout, this, &Robot::cameraAtTargetPoint, Qt::QueuedConnection);

    connect(cameraWidget, &CameraWidget::imageCaptured, this, &Robot::imageCaptured);
    connect(this, &Robot::result, _cameraWidget, &CameraWidget::showResult, Qt::QueuedConnection);
}

Robot::~Robot()
{}

void Robot::start()
{
    qDebug() << "Start button pressed";
    switch(_state)
    {
    case Start:
    case TableCheck:
        break;

    case Stop:
        delete _points;
        _points = new TablePoints;
        _cameraController->lightOn();
        _state = TableCheck;
        next();
        break;

    case Close:
        break;
    }
}

void Robot::stop()
{
    qDebug() << "Stoped";

    _state = Stop;

    _cameraController->goHome();

    emit done();
}

void Robot::next()
{
    qDebug() << "Send camera to next point";

    switch(_state)
    {
    case Start:
        if ( _points->hasNextTargetPoint() )
            _cameraController->moveTo( _points->nextTargetPoint() );
        else
            stop();
        break;

    case TableCheck:
        if ( _points->hasNextCheckPoint() )
            _cameraController->moveTo( _points->nextCheckPoint() );
        else
        {
            _state = Start;
            next();
        }
        break;

    case Stop:
    case Close:
        break;
    }
}

void Robot::cameraAtTargetPoint()
{
    qDebug() << "Camera have arrived to target point";

    switch(_state)
    {
    case Start:
        qDebug() << "Try capture target image";
        _cameraWidget->capture();
        break;

    case TableCheck:
        qDebug() << "Try capture check image";
        _cameraWidget->capture();
        break;

    case Close:
        QApplication::exit();
        break;

    case Stop:
        break;
    }
}

void Robot::imageCaptured(int id, const QImage &image)
{
    Q_UNUSED(id)
    qDebug() << "Image captured, send it to analyzer";

    switch(_state)
    {
    case Start:
        _imageAnalyzer.getResult(image);
        break;

    case TableCheck:
        _imageAnalyzer.checkPackPresence(image);
        break;

    case Stop:
    case Close:
        break;
    }

}

void Robot::packPresence(bool presence)
{
    _points->currentCheckIs(presence);
    next();
}

void Robot::resultReady(CellResult res)
{
    qDebug() << "Result received, send it for save and go to next point";
    emit result(res);

    if ( _points->packDone() )
        emit packDone();

    switch(_state)
    {
    case Start:
        next();
        break;

    case TableCheck:
    case Stop:
    case Close:
        break;
    }
}

void Robot::prepareToClose()
{
    qDebug() << "Start preparing to close";
    if ( _state == Start )
        stop();

    if ( !_cameraController->isConnected() ){
        QApplication::exit();
        return;
    }

    qDebug() << "Parking camera";
    _cameraController->goHome();
    _state = Close;
}
