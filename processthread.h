#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include "stdio.h"
#include "QMutex"
#include "iostream"

#include "QThread"
#include <QObject>
#include "opencv2/opencv.hpp"
#include "imagebuffer.h"
#include "QImage"
#include "grabimgthread.h"
#include "QApplication"

using namespace cv;
using namespace std;

class ProcessThread : public QThread
{
    Q_OBJECT
public:
    ProcessThread();
    ~ProcessThread();
    bool isColor(MvGvspPixelType enType);
    bool isMono(MvGvspPixelType enType);
    bool m_bProcessing;
//    Mat convertPixelType(void*);
    Mat convertPixelType();
    QImage matToQImage(Mat mat);
    void stopProcessThread();
    void getFrameRate();

//    GrabImgThread* grabImageThread;

    QImage image;
    QMutex imageProcessMutex;
    int imageProcessFinishNum;
    int matToQImageNum;
    int currentSizeOfBuffer;
    int ConvertNum;
    float m_frameRateEdit;
    QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* > currentFrame;
//    unsigned int m_nBufSizeForSaveImage;
//    unsigned char* m_pBufForSaveImage ;

//    ImageBuffer * imageBuffer;

protected:
   void run();

signals:
   void imageProcessfinish();
   void gotImage();
   void gotFrameFinish();

public slots:
   void saveImage();

};

#endif // PROCESSTHREAD_H
