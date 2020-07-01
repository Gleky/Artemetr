#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "robot.h"
#include <QTimer>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(1000,this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCameraView(QWidget *cameraview)
{
    ui->frameCameraView->addWidget(cameraview);
    cameraview->setMinimumSize(640,480);
}

void MainWindow::setCameraPos(QWidget *camerapos)
{
    ui->frameCameraPos->addWidget(camerapos);
    camerapos->setFixedSize(400,424);

    ui->frameCameraPos->addWidget(&labelCrayfish);
    labelCrayfish.setMargin(6);
    ui->frameCameraPos->addWidget(&labelEgg);
    labelEgg.setMargin(6);

#ifdef TEST
    labelCrayfish.setText("Test: 0");
    labelEgg.setText("Test: 0");
#endif
}

void MainWindow::connectButtons(Robot *robot)
{
    _robot = robot;
    connect(ui->pushButtonStart, &QPushButton::clicked, robot, &Robot::start);
    connect(ui->pushButtonStop, &QPushButton::clicked, robot, &Robot::stop);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    static bool firstCall = true;

    if ( !firstCall ) {
        QApplication::exit();
        return;
    }

    emit closing();
    _robot->prepareToClose();
    event->ignore();
    firstCall = false;
    QTimer::singleShot(6000, this, &MainWindow::close);
    hide();
}

