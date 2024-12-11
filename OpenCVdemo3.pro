QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    childwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    readimage.cpp

HEADERS += \
    childwindow.h \
    mainwindow.h \
    opencv.hpp \
    readimage.h

FORMS += \
    childwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world450
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world450d
else:unix: LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world450

INCLUDEPATH += $$PWD/../../OpenCV/opencv/build/include
DEPENDPATH += $$PWD/../../OpenCV/opencv/build/x64/vc15/lib

RESOURCES += \
    xml.qrc

DISTFILES += \
    haarcascade_frontalface_alt.xml
