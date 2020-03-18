#include "mainwindow.h"
#include <QApplication>

#include "camera.h"
#include "comport.h"
#include "manualcameracontrol.h"


using namespace PC;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto controller = w.label();
    Camera cam;
    ComPort port;

    cam.setComPort(&port);
    port.subscribe(&cam);

    controller->setCamera(&cam);
    cam.subscribe(controller);

    return a.exec();
}
