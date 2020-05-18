#include "robot.h"

#include "camerawidget.h"
#include "keywords.h"

#include <QApplication>

#include <QTextEdit> //KOSTIL'
#include <QFile>

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

    if (_cameraWidget->_capture != nullptr)
        connect(cameraWidget->_capture, &QCameraImageCapture::imageCaptured, this, &Robot::imageCaptured);
    connect(this, &Robot::result, _cameraWidget, &CameraWidget::showResult, Qt::QueuedConnection);
}

void Robot::setConsole(QTextEdit *console)
{
    _console = console;

    QFile points("../Points.txt");
    points.open(QIODevice::ReadWrite);
    QTextStream stream(&points);
    auto text = stream.readAll();
    _console->setText(text);
    points.close();
}

void Robot::prepareToClose()
{
    findTargetPoints(); //KOSTIL'

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
    if (_cameraWidget->_capture != nullptr) //KOSTIL'
        _cameraWidget->_capture->capture(); //KOSTIL'
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

    QString text = _console->toPlainText();
    auto list = text.split("\n", QString::SkipEmptyParts);

    while (list.size() > 0)
    {
        auto pair = list.takeFirst().split(" ",  QString::SkipEmptyParts);
        if (pair.size() > 1)
        {
            bool checkX = false;
            int x = pair.takeFirst().toInt(&checkX);
            bool checkY = false;
            int y = pair.takeFirst().toInt(&checkY);
            if (checkX && checkY)
            {
                Point point = {x,y};
                _targetPoints.append(point);
            }
        }
    }

    QFile points("../Points.txt");
    points.open(QIODevice::ReadWrite);
    QTextStream stream(&points);
    stream << _console->toPlainText();
    points.close();
}
