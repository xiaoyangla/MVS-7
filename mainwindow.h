#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileDialog"
#include "opencv2/opencv.hpp"
#include "QImage"
#include"imagewidget.h"

#include <iostream>
#include "grabimgthread.h"
#include "processthread.h"
#include "savethread.h"
#include "QMessageBox"
#include <QCloseEvent>



//using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class GrabImgThread;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int uiDisplayImageNum;
//    QFileDialog* openFilePath;

private:
    Ui::MainWindow *ui;

    GrabImgThread *grabImageThread;
    ProcessThread *processThread;
    SaveThread *saveThread;
    bool m_bClose;
    ImageWidget *m_Image;
    QGraphicsScene  *qgraphicsScene;




public slots:
   void showMessage();
   void recvShowPicSignal();//接收并显示图片的函数
   void uiDisplayImage();
   void connectCamera();
   void start();
   void stop();
   void getFrameRateDis();
   void getSavePath();
   int  OnSaveCbxChanged();
   int  OnTriModCbxChanged();
   void selectTriSou();
   void SoftTriggerCrt();
   void close();
   void setBurstFrameNum();
   void setExposureTime();
   void getGrabedFrameNum();

protected:
   void closeEvent(QCloseEvent* event);


};
#endif // MAINWINDOW_H
#pragma once
