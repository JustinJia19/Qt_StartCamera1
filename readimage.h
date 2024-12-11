#ifndef READIMAGE_H
#define READIMAGE_H

#include <QObject>
#include <opencv2/opencv.hpp>

class ReadImage : public QObject
{
    Q_OBJECT

public:
    explicit ReadImage(QObject *parent = nullptr);

signals:
    void readImageDone(const cv::Mat& frame);
    void frameReady(const cv::Mat& frame);
    void facesDetected(int count);

public slots:
    void readImage();
    void provideLatestFrame();

public:
    void detectFaces(cv::Mat &frame);

    cv::VideoCapture cap; // 摄像头捕获对象
    cv::Mat latestFrame;  // 最新的帧数据
    bool isEnable;        // 是否启用人脸识别
    cv::CascadeClassifier face_cascade; // Haar 级联分类器
};

#endif // READIMAGE_H
