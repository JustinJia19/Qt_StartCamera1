#include "readimage.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <QResource>

ReadImage::ReadImage(QObject *parent)
    : QObject(parent), isEnable(false) // 初始化 isEnable 为 false
{

    QString faceCascadeFileName = QFileDialog::getOpenFileName(
        nullptr,tr("选择关联器"),"", // 当前目录或默认目录
        tr("Images (*.xml)") // 文件过滤器
        );
    //QString faceCascadeFileName = "D:\\software\\Qt\\OpenCVdemo3\\haarcascade_frontalface_alt.xml";
    std::string faceCascadeFileNameStd = faceCascadeFileName.toStdString();

    if (!cap.open(0)) { // 尝试打开默认摄像头 (index 0)
        qCritical() << "Failed to open camera!";
    } else {
        qDebug() << "Camera opened successfully.";
    }

    // 加载 Haar 级联分类器文件
    if (!face_cascade.load(faceCascadeFileNameStd)) {
        qCritical() << "Error loading cascade file from resource";
    } else {
        qDebug() << "Cascade file loaded successfully.";
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

    qDebug() << "Detected" << faces.size() << "faces.";
    emit facesDetected(faces.size());

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

    // 更新 latestFrame 成员变量
    latestFrame = frame.clone();

    // 根据标志决定是否调用 detectFaces 函数
    if (isEnable) {
        detectFaces(latestFrame);
    }

    emit readImageDone(latestFrame); // 通知主线程绘图
}

// 提供最新的帧给需要的地方
void ReadImage::provideLatestFrame()
{
    if (!latestFrame.empty()) {
        qDebug() << "Emitting frameReady signal with a non-empty frame.";
        emit frameReady(latestFrame.clone()); // 发出信号表示新帧已经准备好了
    } else {
        qWarning() << "No frame available to provide.";
    }
}
