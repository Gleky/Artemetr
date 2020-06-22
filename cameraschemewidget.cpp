#include "cameraschemewidget.h"


#include "keywords.h"
#include "cameracontrol.h"

#include <QPainter>
#include <QKeyEvent>


CameraSchemeWidget::CameraSchemeWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy( Qt::StrongFocus );
}

void CameraSchemeWidget::setController(CameraControl *controller)
{
    _controller = controller;
}

void CameraSchemeWidget::mousePressEvent(QMouseEvent *event)
{
    auto pos = event->pos();

    Point newPos = {(int)round( static_cast<double>(pos.x()-_leftBorder)/_scale ),
                    yMaxPos -(int)round( static_cast<double>(pos.y()-_topBorder)/_scale )};

    if (newPos.X < 0)
        newPos.X = 0;
    if (newPos.X > xMaxPos)
        newPos.X = xMaxPos;
    if (newPos.Y < 0)
        newPos.Y = 0;
    if (newPos.Y > yMaxPos)
        newPos.Y = yMaxPos;

    moveToPoint(newPos);
}

void CameraSchemeWidget::keyPressEvent(QKeyEvent *event)
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

    if ( buttonUp || buttonDown || buttonLeft || buttonRight ) {
        checkKeys();
    }
}

void CameraSchemeWidget::keyReleaseEvent(QKeyEvent *event)
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

void CameraSchemeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter( this );
    QPen linepen;
    linepen.setCapStyle( Qt::RoundCap );
    painter.setRenderHint( QPainter::Antialiasing, true );
    QColor red(235,90,100);
    QColor green(100,210,120);

    linepen.setColor( QColor(0,0,0) );
    linepen.setWidth( 2 );
    painter.setPen( linepen );
    painter.drawRect(_leftBorder, _topBorder, xMaxPos*_scale, yMaxPos*_scale);

    if ( _controller == nullptr )
        return;

    Point currentPos = _controller->currentPos();
    Point targetPos = _controller->targetPos();

    if ( targetPos.X >= 0 && targetPos.Y >= 0 ) {
        linepen.setWidth( _pointSize );
        linepen.setColor( red );
        painter.setPen( linepen );
        painter.drawPoint( _leftBorder + targetPos.X*_scale, _topBorder + (yMaxPos - targetPos.Y)*_scale );
    }

    if ( currentPos.X >= 0 && currentPos.Y >= 0 ){
        linepen.setColor( green );
        linepen.setWidth( _pointSize*0.8 );
        painter.setPen( linepen );
        painter.drawPoint( _leftBorder + currentPos.X*_scale, _topBorder + (yMaxPos - currentPos.Y)*_scale );
    }

    if ( !_controller->isConnected() ) {
        linepen.setColor( red );
        painter.setPen( linepen );
        painter.drawText((width() - 80)/2, (height() - 15)/2, 80, 15, Qt::AlignCenter, "DISCONNECTED");
    }

    if ( currentPos.X >= 0 && currentPos.Y >= 0 ) {
        linepen.setColor( QColor(80,80,80) );
        painter.setPen( linepen );
        QFont font = painter.font();
        font.setPixelSize(16);
        painter.setFont(font);
        painter.drawText((width() - 80)/2, (height() - 25)/2, 80, 25, Qt::AlignCenter,
                         QString::number(currentPos.X)+"  "+QString::number(currentPos.Y));
    }
}

void CameraSchemeWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    const int widgetWidth = width();
    const int widgetHeight = height();

    const double tableRatio = static_cast<double>(xMaxPos)/yMaxPos;
    const double windowRatio = static_cast<double>(widgetWidth)/widgetHeight;

    double height,width;
    if ( tableRatio < windowRatio ) {
        _topBorder = widgetHeight*0.04;
        height = widgetHeight - 2*_topBorder;
        width = height * tableRatio;
        _leftBorder = (widgetWidth - width)/2;
    }
    else {
        _leftBorder = widgetWidth*0.04;
        width = widgetWidth - 2*_leftBorder;
        height = width / tableRatio;
        _topBorder = (widgetHeight - height)/2;
    }
    _pointSize = sqrt(height*width)*0.03;
    _scale = height/yMaxPos;
}

void CameraSchemeWidget::checkKeys()
{
    if ( _controller == nullptr )
        return;

    Point newPos = _controller->targetPos();
    if ( buttonUp )
        stepUp(newPos);
    if ( buttonDown )
        stepDown(newPos);
    if ( buttonLeft )
        stepLeft(newPos);
    if ( buttonRight )
        stepRight(newPos);

    emit moveToPoint(newPos);
}

const int step = 1;

void CameraSchemeWidget::stepUp(Point &newPos) const
{
    newPos.Y += step;
    if ( newPos.Y > yMaxPos )
        newPos.Y = yMaxPos;
}
void CameraSchemeWidget::stepDown(Point &newPos) const
{
    newPos.Y -= step;
    if ( newPos.Y < 0 )
        newPos.Y = 0;
}
void CameraSchemeWidget::stepLeft(Point &newPos) const
{
    newPos.X -= step;
    if ( newPos.X < 0 )
        newPos.X = 0;
}
void CameraSchemeWidget::stepRight(Point &newPos) const
{
    newPos.X += step;
    if ( newPos.X > xMaxPos )
        newPos.X = xMaxPos;
}
