#include <QApplication>

#include "mainwindow.h"
#include "robot.h"
#include "camera.h"
#include "comport.h"
#include "cameracontrol.h"
#include "camerawidget.h"
#include "resultsaver.h"
#include "manualcontrolwidget.h"
#include "cameraschemewidget.h"

#include <QFile>

const QString logFileName = "log.txt";
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


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

    CameraWidget cameraWidget;
    window.setCameraView(&cameraWidget);

    CameraControl cameraController( &cam, &port );
    cam.subscribe( &cameraController );

    ManualControlWidget manualControl( &cameraController );
    window.connect(&window, &MainWindow::closing, &manualControl, &ManualControlWidget::close);

    CameraSchemeWidget cameraScheme( &cameraController, &manualControl, &window );
    window.setCameraPos( &cameraScheme );
    cameraScheme.connect( &cameraController, &CameraControl::cameraUpdated, &cameraScheme, &CameraSchemeWidget::updateScheme );

    Robot robot(&cameraController, &cameraWidget);
    window.connectButtons(&robot);

    ResultSaver saver("results");
    saver.connect(&robot, &Robot::result, &saver, &ResultSaver::saveResult);
    saver.connect(&robot, &Robot::packDone, &saver, &ResultSaver::savePackResultsText);
    saver.connect(&robot, &Robot::done, &saver, &ResultSaver::saveSummaryResultsText);

    qDebug() << "Init done";

    return a.exec();
}


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
