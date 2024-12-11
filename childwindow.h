#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H
#include <opencv2/opencv.hpp>
#include <QMainWindow>

namespace Ui {
class ChildWindow;
}

class ChildWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChildWindow(QWidget *parent = nullptr);
    ~ChildWindow();
    QString dirPath;

private slots:
    void on_inputBtn_clicked();

    void on_getBtn_clicked();

    void on_saveBtn_clicked();

private:
    Ui::ChildWindow *ui;

private:
    cv::Mat image;
    bool ishave;
};

#endif // CHILDWINDOW_H
