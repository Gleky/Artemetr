#ifndef ROBOT_H
#define ROBOT_H

#include "irobot.h"
#include "cameracontrol.h"
#include "imageanalyzer.h"

#include <QTimer>

class QTextEdit; //KOSTIL'
class ImageAnalyzer;
class CameraWidget;

class Robot : public QObject, public IRobot
{
    Q_OBJECT
public:
    Robot();
    ~Robot();

    void setCameraControl(CameraControl *camera);
    void setCameraView(CameraWidget *cameraWidget);

    void setConsole(QTextEdit *console); //KOSTIL'

    void prepareToClose();

public slots:
    void start() override;
    void pause() override;
    void stop() override;

    void cameraAtTargetPoint();
    void resultReady(Result res);

    void imageCaptured(int id, const QImage &preview);

signals:
    void result(Result);
    void done();

private:
    void findTargetPoints();
    QVector<Point> _targetPoints;
    void getNext();
    QTimer _delay;

    enum RobotState {Started, Paused, Stoped, Closing};
    RobotState _state = Stoped;

    ImageAnalyzer _imageAnalyzer;

    CameraControl *_cameraController = nullptr;
    CameraWidget *_cameraWidget = nullptr;

    QTextEdit *_console = nullptr; //KOSTIL'
};

#endif // ROBOT_H
