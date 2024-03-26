QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += console #后台调戏输出窗口显示

INCLUDEPATH += "C:\Program Files (x86)\MVS\Development\Includes"#路径不用引号容易报错,直接运行虽然pro包含这个路径，但是用别的编译时，这个路径仍然没有包含到环境变量要么用qmake执行，要么添加到环境变量
LIBS += -L"C:\Program Files (x86)\MVS\Development\Libraries\win64"#有空格要用引号括起来
LIBS += -lMvCameraControl

INCLUDEPATH += E:\Opencv\opencv\build\include
Release: LIBS += -LE:\Opencv\opencv\build\x64\vc15\lib -lopencv_world450


Debug: LIBS += -LE:\Opencv\opencv\build\x64\vc15\lib -lopencv_world450d


SOURCES += \
    global.cpp \
    grabimgthread.cpp \
    imagebuffer.cpp \
    imagewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    processthread.cpp \
    savethread.cpp

HEADERS += \
    global.h \
    grabimgthread.h \
    imagebuffer.h \
    imagewidget.h \
    mainwindow.h \
    processthread.h \
    savethread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
