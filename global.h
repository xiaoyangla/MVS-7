#ifndef GLOBAL_H
#define GLOBAL_H
#include "ImageBuffer.h"
extern ImageBuffer *imageBuffer;
extern ImageBuffer *imageBuffer2;
extern QMutex stoppedMutex;
extern volatile bool stopped;
extern QMutex stoppedMutex1;//防止锁之间产生矛盾，可以只用一把锁，上锁后会阻塞其他线程访问共享资源
extern volatile bool stopped1;
extern void* m_handle;
extern unsigned int m_nBufSizeForSaveImage;
extern unsigned char* m_pBufForSaveImage1;
extern unsigned char* m_pBufForSaveImage2;
//extern int exposureTimeNum;
#endif // GLOBAL_H
