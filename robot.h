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
    void result(CellImage);
    void done();
    void packDone();

private slots:
    void cameraAtTargetPoint();
    void imageCaptured(int id, const QImage &image);
    void packPresence( bool presence );

private:
    enum RobotState {TableCheck, PhotoAndIodine, PhotoAndChlorine, Photo, Stop, Close};
    RobotState _state = Stop;

    TablePoints *_points = new TablePoints;
    void next();

    ImageAnalyzer _imageAnalyzer;

    CameraControl *const _cameraController;
    CameraWidget *const _cameraWidget;
    QTimer _delay;

    void sendPhoto(const QImage &image, PhotoType type);
    void putIodine() const;
    void putChlorine() const;
    void waitAfterIodine();
    void waitAfterChlorine();
    QTimer _chemicalWaiting;

    void runNn() const;
};

#endif // ROBOT_H
