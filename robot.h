#ifndef ROBOT_H
#define ROBOT_H

//#include "irobot.h"
#include "cameracontrol.h"
#include "imageanalyzer.h"
#include "tablepoints.h"

#include <QTimer>


class ImageAnalyzer;
class CameraWidget;

class Robot : public QObject
{
    Q_OBJECT
public:
    Robot(CameraControl *camera,CameraWidget *cameraWidget);
    ~Robot();

    void prepareToClose();

public slots:
    void start();
    void stop();

    void cameraAtTargetPoint();
    void imageCaptured(int id, const QImage &image);
    void packPresence( bool presence );
    void resultReady(CellResult res);

signals:
    void result(CellResult);
    void done();
    void packDone();

private:
    enum RobotState {Start, TableCheck, Stop, Close};
    RobotState _state = Stop;

    TablePoints *_points = new TablePoints;
    void next();

    ImageAnalyzer _imageAnalyzer;

    CameraControl *const _cameraController = nullptr;
    CameraWidget *const _cameraWidget = nullptr;
    QTimer _delay;
    Robot();
};

#endif // ROBOT_H
