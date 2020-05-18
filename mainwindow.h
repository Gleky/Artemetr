#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QCameraViewfinder>
#include <QCamera>


class QTextEdit; //KOSTIL'
class Robot;

namespace Ui {
class MainWindow;
}

class CameraControl;
class QPropertyAnimation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setCameraView(QWidget *cameraview);
    void setCameraPos(QWidget *camerapos);

    void connectButtons(Robot *robot);

    QTextEdit *console(); //KOSTIL'

private:
    Ui::MainWindow *ui = nullptr;
    Robot* _robot = nullptr;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
