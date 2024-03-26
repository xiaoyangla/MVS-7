#ifndef SAVETHREAD_H
#define SAVETHREAD_H

#include <QObject>
#include "stdio.h"
#include "QMutex"
#include "iostream"

#include "QThread"
#include <QObject>
#include "opencv2/opencv.hpp"
#include "imagebuffer.h"
using namespace std;
class SaveThread : public QThread
{
    Q_OBJECT
public:
//    explicit SaveThread();
    SaveThread();
    QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* > currentFrame1;
//    unsigned int m_nBufSizeForSaveImage1;
//    unsigned char* m_pBufForSaveImage1 ;
    int saveCount;
    QString savePath;
    QMutex imageSaveMutex;
    volatile bool m_bSaveThread;
    int currentSizeOfBuffer;
    void run();
    void saveImage();
    void stopSaveThread();
signals:
    void saveImageFinish();
};

#endif // SAVETHREAD_H
