#include "cameracontrol.h"
#include "icamera.h"

#include <QKeyEvent>
#include <QPainter>
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

CameraControl::CameraControl(QWidget *parent)
    :QFrame(parent),
    _scheme(this),
    _lightButton(this)
{
    _lightButton.setText("Light - OFF");
    _lightButton.setCheckable(true);
    _lightButton.setStyleSheet(lightButtonStyle);
    connect(&_lightButton, &QPushButton::toggled, this, &CameraControl::lightButtonToggled);

    _scheme.setController(this);
    connect(&_scheme, &CameraSchemeWidget::moveToPoint, this, &CameraControl::moveToPoint);

    setStyleSheet("QFrame{background-color: rgb(250,250,250);"
                         "border-radius: 3px;"
                         "border: 1px solid rgb(220,220,220);}");

}

CameraControl::~CameraControl()
{}

void CameraControl::setCamera(ICamera *camera)
{
    _camera = camera;
}

void CameraControl::moveCamera(Point newPos)
{
    _camera->move(newPos);
    _goingToNewPos = true;

#ifdef TEST
    emit cameraReachedTargetPoint();
    _scheme.update();
#endif
}

void CameraControl::goHome()
{
    moveCamera( Point(homeX,homeY) );
    _lightButton.setChecked(false);
}

void CameraControl::lightOn()
{
    if ( !_cameraConnected )
        return;
    _lightButton.setChecked(true);
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
    _scheme.update();
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
    _scheme.update();
}

void CameraControl::cameraDisconnected()
{
    _cameraConnected = false;
    _scheme.update();
}

void CameraControl::moveToPoint(const Point &newPos)
{
    if ( _goingToNewPos )
        return;

    if ( newPos == _camera->targetPos() )
        return;

    _camera->move(newPos);
    _scheme.update();
}

void CameraControl::lightButtonToggled(bool checked)
{
    if ( checked ) {
        _lightButton.setText("Light - ON");
        _camera->setBacklight(On);
    } else {
        _lightButton.setText("Light - OFF");
        _camera->setBacklight(Off);
    }
}

void CameraControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int margin = 3;
    int buttonHeight = 22;
    _lightButton.setGeometry(margin, height() - buttonHeight - margin, width() - 2*margin, buttonHeight);
    _scheme.setGeometry(0,0,width(), height()-2*margin - buttonHeight);
}
