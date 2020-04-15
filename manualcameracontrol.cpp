#include "manualcameracontrol.h"
#include "icamera.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>


ManualCameraControl::ManualCameraControl(QWidget *parent)
    :QLabel(parent)
{
    setFocusPolicy( Qt::StrongFocus );
    QTimer::singleShot(30, this, &ManualCameraControl::checkKeys);
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

    auto newPos = _camera->currentPos();
    if ( _currentPos == newPos )
        return;

    _currentPos = _camera->currentPos();
    update();
}

void ManualCameraControl::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Up ) {
        buttonUp = true;
    } else if ( event->key() == Qt::Key_Down ) {
        buttonDown = true;
    } else if ( event->key() == Qt::Key_Left ) {
        buttonLeft = true;
    } else if ( event->key() == Qt::Key_Right ) {
        buttonRight = true;
    }
}

void ManualCameraControl::keyReleaseEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Up ) {
        buttonUp = false;
    } else if ( event->key() == Qt::Key_Down ) {
        buttonDown = false;
    } else if ( event->key() == Qt::Key_Left ) {
        buttonLeft = false;
    } else if ( event->key() == Qt::Key_Right ) {
        buttonRight = false;
    }
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

void ManualCameraControl::mousePressEvent(QMouseEvent *event)
{
    auto pos = event->pos();

    Point newPos = {(int)round( static_cast<double>(pos.x()-_leftBorder)*Hmm/_height ),
                    Hmm -(int)round( static_cast<double>(pos.y()-_topBorder)*Hmm/_height )};

    if (newPos.X < 0)
        newPos.X = 0;
    if (newPos.X > Wmm)
        newPos.X = Wmm;
    if (newPos.Y < 0)
        newPos.Y = 0;
    if (newPos.Y > Hmm)
        newPos.Y = Hmm;

    moveTo(newPos);
}

void ManualCameraControl::checkKeys()
{
    Point newPos = _targetPos;
    if ( buttonUp )
        stepUp(newPos);
    if ( buttonDown )
        stepDown(newPos);
    if ( buttonLeft )
        stepLeft(newPos);
    if ( buttonRight )
        stepRight(newPos);

    moveTo(newPos);
    QTimer::singleShot(100, this, &ManualCameraControl::checkKeys);
}


const int step = 5;

void ManualCameraControl::stepUp(Point &newPos) const
{
    newPos.Y += step;
    if ( newPos.Y > Hmm )
        newPos.Y = Hmm;
}

void ManualCameraControl::stepDown(Point &newPos) const
{
    newPos.Y -= step;
    if ( newPos.Y < 0 )
        newPos.Y = 0;
}

void ManualCameraControl::stepLeft(Point &newPos) const
{
    newPos.X -= step;
    if ( newPos.X < 0 )
        newPos.X = 0;
}

void ManualCameraControl::stepRight(Point &newPos) const
{
    newPos.X += step;
    if ( newPos.X > Wmm )
        newPos.X = Wmm;
}

int ManualCameraControl::mappedValue(const Point &pos, ManualCameraControl::Dimention dimention) const
{
    if ( dimention == Y ) {
        return round( static_cast<double>(Hmm - pos.Y)*_height/Hmm ) + _topBorder;
    }
    else {
        return round( static_cast<double>(pos.X)*_width/Wmm ) + _leftBorder;
    }
}

void ManualCameraControl::moveTo(const Point &newPos)
{
    if ( newPos == _targetPos )
        return;

    _targetPos = newPos;
    _camera->move(newPos);
//    update();
}
