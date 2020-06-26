#include <QApplication>

#include "mainwindow.h"
#include "robot.h"
#include "camera.h"
#include "comport.h"
#include "cameracontrol.h"
#include "camerawidget.h"
#include "resultsaver.h"

#include <QFile>
const QString logFileName = "../log.txt";
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();

    static QMutex mutex;
    QFile outFile(logFileName);

    mutex.lock();
    outFile.open(QIODevice::Append);
    QTextStream ts(&outFile);

    switch (type) {
    case QtDebugMsg:
        ts << "D: " + msg + '\n';
        break;
    case QtInfoMsg:
        ts << "D: " + msg + '\n';
        break;
    case QtWarningMsg:
        ts << "D: " + msg + '\n';
        break;
    case QtCriticalMsg:
        ts << "D: " + msg + '\n';
        break;
    case QtFatalMsg:
        ts << "D: " + msg + '\n';
        mutex.unlock();
        abort();
        break;
    }
    outFile.close();
    mutex.unlock();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;

    QFile::remove(logFileName);
    qInstallMessageHandler(myMessageOutput);

    PC::Camera cam;
    PC::ComPort port;
    cam.setComPort(&port);
    port.subscribe(&cam);

    CameraControl cameraController;
    cameraController.setCamera(&cam);
    cam.subscribe(&cameraController);
    window.setCameraPos(&cameraController);
    cameraController.connect(&port, &PC::ComPort::connected, &cameraController, &CameraControl::cameraConnected);
    cameraController.connect(&port, &PC::ComPort::disconnected, &cameraController, &CameraControl::cameraDisconnected);

    CameraWidget cameraWidget;
    window.setCameraView(&cameraWidget);

    Robot robot(&cameraController, &cameraWidget);
    window.connectButtons(&robot);

    ResultSaver saver("results");
    saver.connect(&robot, &Robot::result, &saver, &ResultSaver::saveResult);
    saver.connect(&robot, &Robot::packDone, &saver, &ResultSaver::savePackResultsText);
    saver.connect(&robot, &Robot::done, &saver, &ResultSaver::saveSummaryResultsText);

    qDebug() << "Init done";

    return a.exec();
}
