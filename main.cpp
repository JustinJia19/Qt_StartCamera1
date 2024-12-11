#include "mainwindow.h"
#include <QApplication>
#include <QResource>
#include <QtResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
