#include "readimage.h"
#include <QDebug>
#include <QDir>
#include <opencv2/opencv.hpp>

ReadImage::ReadImage(QObject *parent)
    : QObject(parent)
{
    if (!cap.open(0)) { // 尝试打开默认摄像头 (index 0)
        qCritical() << "Failed to open camera!";
    } else {
        qDebug() << "Camera opened successfully.";
    }
    if (!face_cascade.load("D:/software/Qt/OpenCVdemo3/haarcascade_frontalface_alt.xml")) {
        qDebug() << "Error loading cascade file from resource";
    }
}

// 人脸识别函数
void ReadImage::detectFaces(cv::Mat& frame) {
    // 将帧转换为灰度图，因为Haar分类器需要灰度图像
    cv::Mat gray;
    cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // 检测人脸
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    // 在检测到的人脸上绘制矩形框
    for (const auto& rect : faces) {
        rectangle(frame, rect, cv::Scalar(255, 0, 0), 2); // 绘制蓝色矩形
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

    cv::resize(frame, frame, cv::Size(450, 300)); // 调整图像大小

    // 根据标志决定是否调用 detectFaces 函数
    if (isEnable) {
        detectFaces(frame);
    }

    emit readImageDone(frame); // 通知主线程绘图
}
