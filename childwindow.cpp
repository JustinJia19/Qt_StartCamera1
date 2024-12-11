#include "childwindow.h"
#include "ui_childwindow.h"
#include <QFileDialog>
#include <opencv2/opencv.hpp>

ChildWindow::ChildWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChildWindow)
{
    ui->setupUi(this);
}

QImage cvMatToQImage(const cv::Mat& mat)
{
    QImage image;
    if(!mat.empty())
    {
        image = QImage(mat.data,mat.cols,mat.rows,mat.step,QImage::Format_BGR888);
    }
    return image;
}

ChildWindow::~ChildWindow()
{
    delete ui;
}

void ChildWindow::on_inputBtn_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
        this,tr("选择图片"),"", // 当前目录或默认目录
        tr("Images (*.png *.jpg *.jpeg *.webp)") // 文件过滤器
        );
    if(!imagePath.isEmpty())
    {
        QImage image(imagePath);
        if(!image.isNull())
        {
            ui->input_label->setPixmap(QPixmap::fromImage(image));
            ui->input_label->setScaledContents(true);
            ui->input_label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        }
    }
    else{
        return;
    }
    //从文件路径加载图像，并将该图像存储在一个cv::Mat类型的对象中
    image = cv::imread(imagePath.toStdString());
    ishave = true;
}



void ChildWindow::on_getBtn_clicked()
{
    if(!ishave)
    {
        return;
    }

    //将图像转为灰度图像
    cv::Mat gray;
    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);
    //gray.data：指向gray对象的指针
    QImage gray_image(gray.data,gray.cols,gray.rows,gray.step,QImage::Format_Grayscale8);
    ui->gray_label->setPixmap(QPixmap::fromImage(gray_image));
    ui->gray_label->setScaledContents(true);
    ui->gray_label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    //对灰度图进行图像处理
    cv::Mat blur;
    cv::GaussianBlur(gray,blur,cv::Size(3,3),0,0);
    QImage guass_img(gray.data,gray.cols,gray.rows,gray.step,QImage::Format_Grayscale8);

    //阈值化
    cv::Mat threshold;
    cv::threshold(blur,threshold,100,255,cv::THRESH_BINARY);
    QImage th_image(gray.data,gray.cols,gray.rows,gray.step,QImage::Format_Grayscale8);

    //轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(threshold,contours,cv::RETR_CCOMP,cv::CHAIN_APPROX_NONE);

    //创建空白图像绘制轮廓
    cv::Mat contourImage(image.size(),CV_8UC3,cv::Scalar(255,255,255));
    //遍历每个轮廓并绘制
    cv::drawContours(contourImage,contours,-1,cv::Scalar(0,0,0),3);
    QImage outputImg;
    outputImg = cvMatToQImage(contourImage);
    ui->outputImg_label->setPixmap(QPixmap::fromImage(outputImg));
    ui->outputImg_label->setScaledContents(true);
    ui->outputImg_label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    image = contourImage;

    // 初始化保存目录为空字符串
    dirPath = "";
}


void ChildWindow::on_saveBtn_clicked()
{
    dirPath = QFileDialog::getExistingDirectory(
        this,                    // 父窗口部件
        tr("Open Directory"),    // 对话框标题
        QDir::homePath(),        // 默认打开的路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks // 选项
        );
    qDebug() << "Selected directory:" << dirPath; // 添加调试信息
    // 创建唯一的文件名
    QString fileName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz") + ".png";
    QString filePath = QDir(dirPath).filePath(fileName);

    // 使用 OpenCV 的 imwrite 函数保存图像到文件
    if (cv::imwrite(filePath.toStdString(), image)) {
        qDebug() << "Screenshot saved to" << filePath;
    } else {
        qWarning() << "Failed to save screenshot!";
    }
}

