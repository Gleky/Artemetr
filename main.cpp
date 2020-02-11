#include "mainwindow.h"
#include <QApplication>

#include "camera.h"
#include "comport.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  Camera cam;
  ComPort port;

  cam.setComPort(&port);
  port.subscribe(&cam);

  return a.exec();
}
