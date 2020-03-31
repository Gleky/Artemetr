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
    update();
}

void ManualCameraControl::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Up ) {
        stepUp();
    } else if ( event->key() == Qt::Key_Down ) {
        stepDown();
    } else if ( event->key() == Qt::Key_Left ) {
        stepLeft();
    } else if ( event->key() == Qt::Key_Right ) {
        stepRight();
    }
    _camera->move(_targetPos);
    update();
}

void ManualCameraControl::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter( this );
    QPen linepen( Qt::red );
    linepen.setCapStyle( Qt::RoundCap );
    linepen.setWidth( _pointSize );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setPen( linepen );
    painter.drawPoint( mappedValue(_targetPos, X), mappedValue(_targetPos, Y) );

    linepen.setColor( Qt::green );
    linepen.setWidth( _pointSize*0.8 );
    painter.setPen( linepen );
    painter.drawPoint( mappedValue(_currentPos, X), mappedValue(_currentPos, Y) );
}



void ManualCameraControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    const int widgetWidth = width();
    const int widgetHeight = height();

    const double tableRatio = static_cast<double>(Wmm)/Hmm;
    const double windowRatio = static_cast<double>(widgetWidth)/widgetHeight;

    if ( tableRatio < windowRatio ) {
        _topBorder = widgetHeight*0.03;
        _height = widgetHeight - 2*_topBorder;
        _width = _height * tableRatio;
        _leftBorder = (widgetWidth - _width)/2;
    }
    else {
        _leftBorder = widgetWidth*0.03;
        _width = widgetWidth - 2*_leftBorder;
        _height = _width / tableRatio;
        _topBorder = (widgetHeight - _height)/2;
    }
    _pointSize = sqrt(_height*_width)*0.05;
}


const int step = 20;

void ManualCameraControl::stepUp()
{
    _targetPos.Y -= step;
    if ( _targetPos.Y < 0 )
        _targetPos.Y = 0;
}

void ManualCameraControl::stepDown()
{
    _targetPos.Y += step;
    if ( _targetPos.Y > Hmm )
        _targetPos.Y = Hmm;
}

void ManualCameraControl::stepLeft()
{
    _targetPos.X -= step;
    if ( _targetPos.X < 0 )
        _targetPos.X = 0;
}

void ManualCameraControl::stepRight()
{
    _targetPos.X += step;
    if ( _targetPos.X > Wmm )
        _targetPos.X = Wmm;
}

int ManualCameraControl::mappedValue(const Point &pos, ManualCameraControl::Dimention dimention)
{
    if ( dimention == Y ) {
        return round( static_cast<double>(pos.Y)*_height/Hmm ) + _topBorder;
    }
    else {
        return round( static_cast<double>(pos.X)*_width/Wmm ) + _leftBorder;
    }
}
