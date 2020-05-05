#include "mainwindow.h"
#include <QApplication>

#include "robot.h"
#include "camera.h"
#include "comport.h"
#include "cameracontrol.h"
#include "camerawidget.h"
#include "resultsaver.h"


using namespace PC;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    Camera cam;
    ComPort port;
    cam.setComPort(&port);
    port.subscribe(&cam);

    CameraControl cameraController;
    cameraController.setCamera(&cam);
    cam.subscribe(&cameraController);
    window.setCameraPos(&cameraController);
    cameraController.connect(&port, &ComPort::connected, &cameraController, &CameraControl::cameraConnected);
    cameraController.connect(&port, &ComPort::disconnected, &cameraController, &CameraControl::cameraDisconnected);

    CameraWidget cameraWidget;
    window.setCameraView(&cameraWidget);

    Robot robot;
    robot.setCameraControl(&cameraController);
    robot.setCameraView(&cameraWidget);
    window.connectButtons(&robot);

    ResultSaver saver;
    saver.setFolder("results");
    saver.connect(&robot, &Robot::result, &saver, &ResultSaver::saveResult);

    return a.exec();
}
