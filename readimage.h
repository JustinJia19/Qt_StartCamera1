#ifndef READIMAGE_H
#define READIMAGE_H

#include <QObject>
#include <opencv2/opencv.hpp>
using namespace cv;

class ReadImage : public QObject
{
    Q_OBJECT
public:
    explicit ReadImage(QObject *parent = nullptr);
    VideoCapture cap; // 操控摄像头的对象
    CascadeClassifier face_cascade; // 人脸识别分类器
    void readImage();
    void detectFaces(cv::Mat& frame);

signals:
    void readImageDone(Mat frame); // 读取完成之后发出信号

public:
    bool isEnable = false; // 控制人脸识别的标志
};

#endif // READIMAGE_H
