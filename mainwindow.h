#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "readimage.h"
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ReadImage* readImage;
    QThread* thread;
    QTimer* timer;
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
