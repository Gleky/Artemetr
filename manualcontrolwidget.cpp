#include "manualcontrolwidget.h"

#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>

ManualControlWidget::ManualControlWidget(CameraControl *cameraController, QWidget *parent)
    : QWidget(parent),
      _controller(cameraController)
{
    setupWidget();
}

void ManualControlWidget::goToCorner()
{

}

void ManualControlWidget::indexCanged(int index)
{
    _packIndex = index;
}

void ManualControlWidget::setupWidget()
{
    setFocusPolicy(Qt::NoFocus);

    auto layout = new QGridLayout;
    setLayout( layout );

    auto packIndex = new QSpinBox(this);
    layout->addWidget(packIndex,0,0);
    packIndex->setRange(1, 15); //HARDCODE!!
    packIndex->setValue(1);
    connect( packIndex, QOverload<int>::of(&QSpinBox::valueChanged), this, &ManualControlWidget::indexCanged );

    auto goToCornerButton = new QPushButton("Go to corner", this);
    layout->addWidget(goToCornerButton,1,0);
    connect( goToCornerButton, &QPushButton::clicked, this, &ManualControlWidget::goToCorner );

    resize(300,300);
}
