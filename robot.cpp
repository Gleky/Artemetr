#include "robot.h"

#include "camerawidget.h"
#include "keywords.h"

#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QSettings>


// NEED TO SPLIT INTO STATE OBJECTS !!!

Robot::Robot(CameraControl *camera, CameraWidget *cameraWidget)
    :_cameraController(camera),
      _cameraWidget(cameraWidget)
{
    connect(&_imageAnalyzer, &ImageAnalyzer::packPresence, this, &Robot::packPresence, Qt::QueuedConnection);
#if defined (TEST) || defined (TEST_WITH_ARDUINO)
    _delay.setInterval(300);
#else
    _delay.setInterval(1000);
#endif
    _delay.setSingleShot(true);

    connect(_cameraController, &CameraControl::cameraReachedTargetPoint, &_delay, qOverload<>(&QTimer::start));
    connect(&_delay, &QTimer::timeout, this, &Robot::cameraAtTargetPoint);

    connect(cameraWidget, &CameraWidget::imageCaptured, this, &Robot::imageCaptured);
    connect(this, &Robot::result, _cameraWidget, &CameraWidget::showResult, Qt::QueuedConnection);

    _chemicalWaiting.setSingleShot(true);
    connect(&_chemicalWaiting, &QTimer::timeout, _cameraController, &CameraControl::lightOn);
    connect(&_chemicalWaiting, &QTimer::timeout, this, &Robot::next, Qt::QueuedConnection);
}

Robot::~Robot()
{}

void Robot::start()
{
    qDebug() << "Start button pressed";
    switch(_state)
    {
    case TableCheck:
    case PhotoAndIodine:
    case PhotoAndChlorine:
    case Photo:
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
    case TableCheck:
        if ( _points->hasNextCheckPoint() )
            _cameraController->moveTo( _points->nextCheckPoint() );
        else
        {
//            _state = PhotoAndIodine;
            _state = Photo;                //1 loop
//            _cameraController->lightOff(); //without light
            next();
        }
        break;

    case PhotoAndIodine:
        if ( _points->hasNextTargetPoint() ) _cameraController->moveTo( _points->nextTargetPoint() );
        else
        {
            _points->restartTargetPoints();
            _state = PhotoAndChlorine;
            waitAfterIodine();
        }
        break;

    case PhotoAndChlorine:
        if ( _points->hasNextTargetPoint() ) _cameraController->moveTo( _points->nextTargetPoint() );
        else
        {
            _points->restartTargetPoints();
            _state = Photo;
            waitAfterChlorine();
        }
        break;

    case Photo:
        if ( _points->hasNextTargetPoint() ) _cameraController->moveTo( _points->nextTargetPoint() );
        else stop();
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
    case TableCheck:
        qDebug() << "Try capture check image";
        _cameraWidget->capture();
        break;

    case PhotoAndIodine:
    case PhotoAndChlorine:
    case Photo:
        if (!_chemicalWaiting.isActive())
        {
            qDebug() << "Try capture target image";
            _cameraWidget->capture();
        }
        break;

    case Stop:
        break;

    case Close:
        QApplication::exit();
        break;
    }
}

void Robot::imageCaptured(int id, const QImage &image)
{
    Q_UNUSED(id)

    switch(_state)
    {
    case TableCheck:
        qDebug() << "Image captured, send it to analyzer";
        _imageAnalyzer.checkPackPresence(image);
        break;

    case PhotoAndIodine:
        qDebug() << "Image captured, send it for save, put iodine and go to next point";
        sendPhoto(image, Original);
        putIodine();
        next();
        break;

    case PhotoAndChlorine:
        qDebug() << "Image captured, send it for save, put chlorine and go to next point";
        sendPhoto(image, AfterIodine);
        putChlorine();
        next();
        break;

    case Photo:
        qDebug() << "Image captured, send it for save and go to next point";
        sendPhoto(image, AfterChlorine);
        next();
        break;

    case Stop:
    case Close:
        break;
    }
}

void Robot::sendPhoto(const QImage &image, PhotoType type)
{
    CellImage res;
    res.img = image;
    res.photoType = type;
    emit result(res);
    if ( _points->packDone() )
        emit packDone();
}

void Robot::putIodine() const
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    auto workTime = settings.value("msecIodinePumpWork").toInt();
    auto afterWorkTime = settings.value("msecWaitAfterPumpWork").toInt();
    _cameraController->putIodine(workTime, afterWorkTime);
}

void Robot::putChlorine() const
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    auto workTime = settings.value("msecClorinePumpWork").toInt();
    auto afterWorkTime = settings.value("msecWaitAfterPumpWork").toInt();
    _cameraController->putChlorine(workTime, afterWorkTime);
}

void Robot::waitAfterIodine()
{
    _cameraController->goHome();
    QSettings settings("settings.ini", QSettings::IniFormat);
    _chemicalWaiting.setInterval(settings.value("secWaitForIodine").toInt()*1000);
    _chemicalWaiting.start();
}

void Robot::waitAfterChlorine()
{
    _cameraController->goHome();
    disconnect(&_chemicalWaiting, &QTimer::timeout, _cameraController, &CameraControl::lightOn);
    QSettings settings("settings.ini", QSettings::IniFormat);
    _chemicalWaiting.setInterval(settings.value("secWaitForChlorine").toInt()*1000);
    _chemicalWaiting.start();
}

void Robot::packPresence(bool presence)
{
    _points->currentCheckIs(presence);
    next();
}

void Robot::prepareToClose()
{
    qDebug() << "Start preparing to close";
    if ( _state != Stop )
        stop();

    if ( !_cameraController->isConnected() ){
        QApplication::exit();
        return;
    }

    qDebug() << "Parking camera";
    _cameraController->goHome();
    _state = Close;
}

void Robot::runNn() const
{
    QStringList param;
    QString path = QDir::current().absoluteFilePath("../nn/start.py");
    QString workDir = QDir::current().absoluteFilePath("../nn/");

    qDebug() << "Running script path: " << path;

#ifdef Q_OS_WIN32
    param << "/C" << path;
    qDebug() << "Result of startDetached: " << QProcess::startDetached("python", param, workDir);
#endif

#ifdef Q_OS_LINUX
    param << path;
    qDebug() << "Result of startDetached: " << QProcess::startDetached("sh", param, workDir);
#endif
}
