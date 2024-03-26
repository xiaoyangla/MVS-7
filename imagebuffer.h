#ifndef IMAGEBUFFER_H
#define IMAGEBUFFER_H
#include <QObject>

#include <QMutex>
#include <QQueue>
#include "QPair"
#include <QSemaphore>

#include "MvCameraControl.h"
#include"grabimgthread.h"

class ImageBuffer
{
//    Q_OBJECT
public:
    explicit ImageBuffer();
    ImageBuffer(int size);
    ~ImageBuffer();
    ImageBuffer(int size, bool dropFrame);
    void addFrame(unsigned char* ,MV_FRAME_OUT_INFO_EX* );
    QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* > getFrame();
    void clearBuffer();
    int getSizeOfImageBuffer();
//    GrabImgThread*grabImageThread;
    QMutex rawQueueProtect;

    QQueue<QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* >> rawQueue;//帧缓冲区队列
    QSemaphore *freeSlots;
    QSemaphore *usedSlots;
//    QSemaphore *clearBuffer1;
//    QSemaphore *clearBuffer2;
    int bufferSize;
    bool dropFrame;
    int GrabedFrameNum;
//signals:
//    void GrabedFrameFinish();
//    int getFrameNum;
//    int addFrameNum;


};

#endif // IMAGEBUFFER_H
