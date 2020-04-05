#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class ManualCameraControl;
class QPropertyAnimation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    ManualCameraControl *label();

private:
    Ui::MainWindow *ui = nullptr;

    QSize _mainSize;
    int _expand = 400;

    QPropertyAnimation *animation = nullptr;

    // QWidget interface
    protected:
        void resizeEvent(QResizeEvent *event) override;
private slots:
        void on_buttonExpandRight_toggled(bool checked);
};

#endif // MAINWINDOW_H
