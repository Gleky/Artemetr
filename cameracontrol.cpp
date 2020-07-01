#include "cameracontrol.h"
#include "icamera.h"
#include "comport.h"

//#include <QKeyEvent>
//#include <QPainter>
#include <keywords.h>

QString const lightButtonStyle("QPushButton{background: rgb(180,180,180);"
                                           "color: #000;"
                                           "border: 0.05em solid rgb(140,140,140);"
                                           "border-radius:4px;"
                                           "font-size:12px;}"
                               "QPushButton:hover{border-width: 0.12em;}"
                               "QPushButton:pressed{background: rgb(165,165,165);}"

                               "QPushButton:checked{background: rgb(245,245,245);"
                                                   "color: #000;}"
                               "QPushButton:cheched:hover {}"
                               "QPushButton:checked:pressed {background: rgb(230,230,230);}"
                               );

QString const borderStyle("QFrame{background-color: rgb(250,250,250);"
                           "border-radius: 3px;"
                           "border: 1px solid rgb(220,220,220);}");

CameraControl::CameraControl(ICamera *camera, const PC::ComPort *port)
    :_camera(camera)
//    _scheme(this),
//    _lightButton(this)
{
    connect(port, &PC::ComPort::connected, this, &CameraControl::cameraConnected);
    connect(port, &PC::ComPort::disconnected, this, &CameraControl::cameraDisconnected);
//    _lightButton.setText("Light - OFF");
//    _lightButton.setCheckable(true);
//    _lightButton.setStyleSheet(lightButtonStyle);
//    connect(&_lightButton, &QPushButton::toggled, this, &CameraControl::lightButtonToggled);

//    _scheme.setController(this);
//    connect(&_scheme, &CameraSchemeWidget::moveToPoint, this, &CameraControl::moveToPoint);

//    setStyleSheet("QFrame{background-color: rgb(250,250,250);"
//                         "border-radius: 3px;"
//                         "border: 1px solid rgb(220,220,220);}");

}

//void CameraControl::setCamera(ICamera *camera)
//{
//    _camera = camera;
//}

void CameraControl::moveTo(Point newPos)
{
    moveToWihtoutSignal(newPos);
#ifndef TEST
    _goingToNewPos = true;
#else
    emit cameraReachedTargetPoint();
#endif
}

void CameraControl::moveToWihtoutSignal(Point newPos)
{
    if ( _goingToNewPos )
        return;

    _camera->move(newPos);
    emit cameraUpdated();
}

void CameraControl::goHome()
{
    _goingToNewPos = false;
    moveTo( Point(homeX,homeY) );
    _camera->setBacklight(Off);
//    _lightButton.setChecked(false);
}

void CameraControl::lightOn()
{
    if ( !_cameraConnected )
        return;
    _camera->setBacklight(On);
//    _lightButton.setChecked(true);
}

void CameraControl::publisherUpdated()
{
    if ( _camera == nullptr )
        return;
    if ( _goingToNewPos ) {
        auto currentPos = _camera->currentPos();
        auto targetPos = _camera->targetPos();
        if ( currentPos == targetPos ) {
            _goingToNewPos = false;
            emit cameraReachedTargetPoint();
        }
    }
    emit cameraUpdated();
}

Point CameraControl::currentPos() const
{
    if ( _camera == nullptr )
        return Point();

    return _camera->currentPos();
}

Point CameraControl::targetPos() const
{
    if ( _camera == nullptr )
        return Point();

    return _camera->targetPos();
}

bool CameraControl::isConnected() const
{
    return _cameraConnected;
}

void CameraControl::cameraConnected()
{
    _cameraConnected = true;
    emit cameraUpdated();
}

void CameraControl::cameraDisconnected()
{
    _cameraConnected = false;
    emit cameraUpdated();
}

//void CameraControl::moveToPoint(const Point &newPos)
//{
//    if ( _goingToNewPos )
//        return;

//    if ( newPos == _camera->targetPos() )
//        return;

//    _camera->move(newPos);
//    _scheme.update();
//}

//void CameraControl::lightButtonToggled(bool checked)
//{
//    if ( checked ) {
//        _lightButton.setText("Light - ON");
//        _camera->setBacklight(On);
//    } else {
//        _lightButton.setText("Light - OFF");
//        _camera->setBacklight(Off);
//    }
//}

//void CameraControl::resizeEvent(QResizeEvent *event)
//{
//    Q_UNUSED(event);
//    int margin = 3;
//    int buttonHeight = 22;
//    _lightButton.setGeometry(margin, height() - buttonHeight - margin, width() - 2*margin, buttonHeight);
//    _scheme.setGeometry(0,0,width(), height()-2*margin - buttonHeight);
//}
