#ifndef GRABIMGTHREAD_H
#define GRABIMGTHREAD_H
#include "QMutex"
#include "iostream"
#include "QImage"
#include "QDebug"
#include <QObject>
#include "QThread"
#include "MvCameraControl.h"
#include "opencv2/opencv.hpp"
#include "QPair"
#include "QQueue"
#include "time.h"
#include "QString"
#include "QStringList"
using namespace std;
using namespace cv;
using namespace cv::dnn;


class GrabImgThread : public QThread
{
    Q_OBJECT
public:
         GrabImgThread();
         ~GrabImgThread();

    void displayCameraImage();
    static void __stdcall eventCallBack(unsigned int nExternalEventId, void *pUser);
    static void __stdcall  getCameraImageCallback(unsigned char *pData, MV_FRAME_OUT_INFO_EX *stImageInfo, void *handle);
    static  unsigned int __stdcall getCameraImageCallback1(void* pUser);

//    static QQueue<QPair<unsigned char *, MV_FRAME_OUT_INFO_EX *>> rawQueue;
    QMutex grabImgMutex;
    void run();

     void exHardwareTrigger();
     void softTriggerOnce();


//    unsigned int i;
//    QImage image;
//    void* m_handle;
    void stopGrabImg();
    int m_isColor;
    unsigned int burstFrameNum;
    bool m_bOpenDevice;
    bool m_bGrabbing;
    bool m_bTriggerMode;
    bool m_bSoftTriggerOnce;
    int m_triggerSource;
    QString messge;
    float exposureTimeNum;
    QStringList strList;


//    static unsigned int triggerMode;



private:

signals:
    void doneFinsh();
    void statusBarMessge();

    void gotFrameFinish();
//    void changeMode();


public slots:
    void initCamera();
    void setExposureTime();
    void setTriggerMode();
    void closeDevice();


};

#endif // GRABIMGTHREAD_H
#pragma once
