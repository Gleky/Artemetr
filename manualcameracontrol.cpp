#include "manualcameracontrol.h"
#include "icamera.h"

#include <QKeyEvent>
#include <QPainter>


ManualCameraControl::ManualCameraControl(QWidget *parent)
    :QLabel(parent)
{
    setFocusPolicy( Qt::StrongFocus );
}

ManualCameraControl::~ManualCameraControl()
{

}

void ManualCameraControl::setCamera(ICamera *camera)
{
    _camera = camera;
}

void ManualCameraControl::updateSub()
{
    if ( _camera == nullptr )
        return;

    _currentPos = _camera->currentPos();
}

void increaseValue(int &val)
{
    const int step = 20,
              max = 300;
    val += step;
    if ( val > max )
        val = max;
}
void decreaseValue(int &val)
{
    const int step = 20,
              min = 0;
    val -= step;
    if ( val < min )
        val = min;
}

void ManualCameraControl::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Up ) {
        decreaseValue( _targetPos.Y );
    } else if ( event->key() == Qt::Key_Down ) {
        increaseValue( _targetPos.Y );
    } else if ( event->key() == Qt::Key_Left ) {
        decreaseValue( _targetPos.X );
    } else if ( event->key() == Qt::Key_Right ) {
        increaseValue( _targetPos.X );
    }
    _camera->move(_targetPos);
    update();
}

void ManualCameraControl::paintEvent(QPaintEvent *event)
{
    QPainter painter( this );
    QPen linepen( Qt::red );
    linepen.setCapStyle( Qt::RoundCap );
    linepen.setWidth( 10 );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setPen( linepen );
    painter.drawPoint( _targetPos.X + 10, _targetPos.Y + 10 );

    linepen.setColor( Qt::green );
    painter.setPen( linepen );
    painter.drawPoint( _currentPos.X + 10, _currentPos.Y + 10 );
}
