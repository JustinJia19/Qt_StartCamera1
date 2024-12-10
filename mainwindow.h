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
    QString dirPath;
    void closeEvent(QCloseEvent *event);

private slots:
    void on_DetectBtn_clicked();
    void on_ChooseBtn_clicked();
    void on_shotBtn_clicked();
    void saveScreenshot(const cv::Mat& frame); // 新增槽函数用于保存截图

signals:
    void requestFrame();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
