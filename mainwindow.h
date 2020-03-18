#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
  class MainWindow;
}

class ManualCameraControl;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

    ManualCameraControl *label();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
