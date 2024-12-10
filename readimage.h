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
    VideoCapture cap;//操控摄像头的对象
    void readImage();

signals:
    void readImageDone(Mat frame);//读取完成之后发出信号
};

#endif // READIMAGE_H
