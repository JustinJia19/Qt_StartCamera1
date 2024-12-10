#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readimage.h"
#include <QThread>
#include <QTimer>
#include <qDebug>
#include <opencv2/opencv.hpp>

QImage MatToQImage(const cv::Mat& mat)
{
    if(mat.type()==CV_8UC1)
    {
        QImage image(mat.cols,mat.rows,QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0;i<256;i++)
        {
            image.setColor(i,qRgb(i,i,i));
        }
        uchar* pSrc = mat.data;
        for(int row = 0;row<mat.rows;row++)
        {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest,pSrc,mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if(mat.type()==CV_8UC3)
    {
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type()==CV_8UC4)
    {
        qDebug()<<"CV_8UC4";
        const uchar* pSrc = (const uchar*)mat.data;
        QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug()<<"ERROR:Mat could not be converted to QImage!";
        return QImage();
    }
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
    timer = new QTimer();//控制读取摄像头的速度
    timer->start(100);
    connect(timer,&QTimer::timeout,readImage,&ReadImage::readImage);
    connect(readImage, &ReadImage::readImageDone, [=](const cv::Mat& frame) { // 读取完成后显示图像
        QImage img = MatToQImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(img));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)//手动关闭子线程
{
    disconnect(timer,&QTimer::timeout,readImage,&ReadImage::readImage);
    thread->quit();
    thread->wait();
    delete readImage;
}


