#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    animation = new QPropertyAnimation(this, "size");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InOutCubic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ManualCameraControl *MainWindow::label()
{
    return ui->cameraPos;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    const int expandButtonWidth = 20;

    int mainHeight = ui->centralWidget->height();

    if (animation->state() == QPropertyAnimation::Stopped) {
        if ( !ui->buttonExpandRight->isChecked() ) {
            _mainSize = size();
        }
    }

    int availableWidth = ui->centralWidget->width()-_mainSize.width();
    ui->cameraPos->setGeometry(_mainSize.width(), 0, availableWidth, mainHeight);

    ui->buttonExpandRight->setGeometry(_mainSize.width() - expandButtonWidth, 0, expandButtonWidth, mainHeight);
    ui->labelCameraImage->setGeometry(0, 0, _mainSize.width() - ui->buttonExpandRight->width(), mainHeight);
}

void MainWindow::on_buttonExpandRight_toggled(bool checked)
{
    if ( checked ) {
        _mainSize = size();
        auto _expandSize = QSize(width() + _expand ,height());

        animation->setStartValue(_mainSize);
        animation->setEndValue(_expandSize);
    }
    else {
        _expand = size().width() - _mainSize.width();
        _mainSize = QSize( _mainSize.width(), size().height() );

        animation->setStartValue( size() );
        animation->setEndValue( _mainSize );
    }

    animation->start();
}
