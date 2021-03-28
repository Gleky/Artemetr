#ifndef ROBOT_H
#define ROBOT_H

#include "cameracontrol.h"
#include "imageanalyzer.h"
#include "tablepoints.h"
#include "struct_result.h"

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

signals:
    void result(CellResult);
    void done();
    void packDone();

private slots:
    void cameraAtTargetPoint();
    void imageCaptured(int id, const QImage &image);
    void packPresence( bool presence );

private:
    enum RobotState {Start, TableCheck, Stop, Close};
    RobotState _state = Stop;

    TablePoints *_points = new TablePoints;
    void next();

    ImageAnalyzer _imageAnalyzer;

    CameraControl *const _cameraController;
    CameraWidget *const _cameraWidget;
    QTimer _delay;

    void runNn() const;
};

#endif // ROBOT_H
