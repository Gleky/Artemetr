#include "manualcontrolwidget.h"

#include "tablepoints.h"
#include "cameracontrol.h"

#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QCloseEvent>

ManualControlWidget::ManualControlWidget(CameraControl *cameraController, QWidget *parent)
    : QWidget(parent),
      _camera(cameraController)
{
    setupWidget();
}

void ManualControlWidget::goToCorner()
{
    static int packIndex = 0;
    static QList<Point> corners;
    static int cornerIndex = 0;

    if ( packIndex != _packIndex )
    {
        packIndex = _packIndex;
        cornerIndex = 0;
        const Point packPosition( (_packIndex-1)%5, (_packIndex-1)/5 ); //HARDCODE!!

        PackCornerPoints packCorners( packPosition );
        corners = packCorners.points();
    }

    _camera->moveToWihtoutSignal( corners[cornerIndex] );

    if ( cornerIndex < 3 )
        ++cornerIndex;
    else
        cornerIndex = 0;
}

void ManualControlWidget::indexCanged(int index)
{
    _packIndex = index;
}

void ManualControlWidget::setupWidget()
{
    setFocusPolicy(Qt::NoFocus);
    setWindowTitle("Manual control");
    setWindowFlag(Qt::WindowStaysOnTopHint);

    auto layout = new QGridLayout;
    setLayout( layout );
    layout->setSpacing(14);

    auto packIndex = new QSpinBox(this);
    layout->addWidget(packIndex,0,0);
    packIndex->setRange(1, 15); //HARDCODE!!
    packIndex->setValue(1);
    packIndex->setFixedHeight(35);
    packIndex->setFont( QFont(packIndex->font().family(), 15) );
    connect( packIndex, QOverload<int>::of(&QSpinBox::valueChanged), this, &ManualControlWidget::indexCanged );

    auto goToCornerButton = new QPushButton("Go to corner", this);
    layout->addWidget(goToCornerButton,0,1);
    goToCornerButton->setFixedHeight( packIndex->height() );
    connect( goToCornerButton, &QPushButton::clicked, this, &ManualControlWidget::goToCorner );

    resize(250,80);
}

void ManualControlWidget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
