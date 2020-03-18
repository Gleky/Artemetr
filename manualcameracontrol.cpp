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

void ManualCameraControl::keyPressEvent(QKeyEvent *event)
{
    const int step = 20;
    if ( event->key() == Qt::Key_Up ) {
//        _camera->moveY(_camera->currentY() + step);
        _targetPos.Y -= step;
    } else if ( event->key() == Qt::Key_Down ) {
//        _camera->moveY(_camera->currentY() - step);
        _targetPos.Y += step;
    } else if ( event->key() == Qt::Key_Left ) {
//        _camera->moveX(_camera->currentX() - step);
       _targetPos.X -= step;
    } else if ( event->key() == Qt::Key_Right ) {
//        _camera->moveX(_camera->currentX() + step);
        _targetPos.X += step;
    }
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
    painter.drawPoint( _targetPos.X, _targetPos.Y );

    linepen.setColor( Qt::green );
    painter.setPen( linepen );
    painter.drawPoint( _currentPos.X, _currentPos.Y );
}
