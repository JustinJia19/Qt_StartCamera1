#include "readimage.h"
#include <QDebug>
#include <opencv2/opencv.hpp>

ReadImage::ReadImage(QObject *parent)
    : QObject(parent)
{
    if (!cap.open(0)) { // 尝试打开默认摄像头 (index 0)
        qCritical() << "Failed to open camera!";
    } else {
        qDebug() << "Camera opened successfully.";
    }
}

void ReadImage::readImage()
{
    cv::Mat frame;
    if (!cap.isOpened()) {
        qWarning() << "Camera is not opened!";
        return;
    }

    cap.read(frame);
    if (frame.empty()) {
        qWarning() << "Failed to read frame from camera!";
        return;
    }

    cv::resize(frame, frame, cv::Size(300, 200)); // 调整图像大小
    emit readImageDone(frame); // 通知主线程绘图
}
