#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readimage.h"
#include "childwindow.h"
#include <QThread>
#include <QTimer>
#include <qDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QCloseEvent>
#include <opencv2/opencv.hpp>

// 将 cv::Mat 转换为 QImage
QImage MatToQImage(const cv::Mat& mat)
{
    if(mat.type() == CV_8UC1) {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++) {
            image.setColor(i, qRgb(i, i, i));
        }
        uchar* pSrc = mat.data;
        for(int row = 0; row < mat.rows; row++) {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    } else if(mat.type() == CV_8UC3) {
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    } else if(mat.type() == CV_8UC4) {
        qDebug() << "CV_8UC4";
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    } else {
        qDebug() << "ERROR: Mat could not be converted to QImage!";
        return QImage();
    }
}

void MainWindow::updateFaceCountLabel(int count) {
    ui->text_label->setText(QString("Detected %1 face(s).").arg(count));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readImage = new ReadImage();
    thread = new QThread(this);
    readImage->moveToThread(thread);
    thread->start();

    timer = new QTimer(); // 控制读取摄像头的速度
    timer->start(100);

    // 连接定时器超时信号到 ReadImage 的读取函数
    connect(timer, &QTimer::timeout, readImage, &ReadImage::readImage);

    // 连接 ReadImage 的图像读取完成信号到显示图像的槽函数
    connect(readImage, &ReadImage::readImageDone, this, [=](const cv::Mat& frame) { // 读取完成后显示图像
        QImage img = MatToQImage(frame);
        if (!img.isNull()) {
            ui->label->setPixmap(QPixmap::fromImage(img));
        } else {
            qWarning() << "Failed to convert cv::Mat to QImage.";
        }
    });

    // 连接截图按钮的信号与槽
    connect(ui->shotBtn, &QPushButton::clicked, this, &MainWindow::on_shotBtn_clicked);

    // 连接 requestFrame 信号到 ReadImage 的 provideLatestFrame 槽
    connect(this, &MainWindow::requestFrame, readImage, &ReadImage::provideLatestFrame);

    // 连接 frameReady 信号到保存截图的槽
    connect(readImage, &ReadImage::frameReady, this, &MainWindow::saveScreenshot);

    //连接 ReadImage 的 facesDetected 信号到更新 QLabel 的槽函数
    connect(readImage, &ReadImage::facesDetected, this, &MainWindow::updateFaceCountLabel);

    // 初始化截图保存目录为空字符串
    dirPath = "";
}

MainWindow::~MainWindow()
{
    delete ui;
    thread->quit();
    thread->wait();
    delete readImage;

    if (childwindow_ui) { // 确保子窗口也被正确清理
        delete childwindow_ui;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    disconnect(timer, &QTimer::timeout, readImage, &ReadImage::readImage);
    event->accept();
}

void MainWindow::on_DetectBtn_clicked()
{
    readImage->isEnable = !readImage->isEnable; // 切换人脸识别开关

    if (readImage->isEnable) {
        qDebug() << "Face detection enabled.";
    } else {
        qDebug() << "Face detection disabled.";
    }
}

void MainWindow::on_ChooseBtn_clicked()
{
    dirPath = QFileDialog::getExistingDirectory(
        this,                    // 父窗口部件
        tr("Open Directory"),    // 对话框标题
        QDir::homePath(),        // 默认打开的路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks // 选项
        );
    qDebug() << "Selected directory:" << dirPath; // 添加调试信息
}

void MainWindow::on_shotBtn_clicked()
{
    if (dirPath.isEmpty()) {
        // 如果尚未选择保存目录，则打开文件对话框让用户选择
        dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory to Save Screenshots"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (dirPath.isEmpty()) {
            qDebug() << "User canceled the directory selection.";
            return;
        }
        qDebug() << "Selected directory:" << dirPath; // 添加调试信息
    }

    // 请求最新的一帧图像
    qDebug() << "Emitting requestFrame signal.";
    emit requestFrame();
}

void MainWindow::saveScreenshot(const cv::Mat& frame)
{
    qDebug() << "saveScreenshot called.";

    if (frame.empty()) {
        qWarning() << "Received an empty frame in saveScreenshot.";
        return;
    }

    // 创建唯一的文件名
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz") + ".png";
    QString filePath = QDir(dirPath).filePath(fileName);

    // 使用 OpenCV 的 imwrite 函数保存图像到文件
    if (cv::imwrite(filePath.toStdString(), frame)) {
        qDebug() << "Screenshot saved to" << filePath;
    } else {
        qWarning() << "Failed to save screenshot!";
    }
}

void MainWindow::on_ChildBtn_clicked()
{
    if (!childwindow_ui)
    {
        childwindow_ui = new ChildWindow(this);
        childwindow_ui->show();
    }

}

