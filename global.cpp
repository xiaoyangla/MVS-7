#include "global.h"

QMutex stoppedMutex;
QMutex stoppedMutex1;
ImageBuffer *imageBuffer = new ImageBuffer(300,true);
ImageBuffer *imageBuffer2 = new ImageBuffer(200);
volatile bool stopped = true;
volatile bool stopped1 = true;
void* m_handle=nullptr;
unsigned int m_nBufSizeForSaveImage=NULL;
unsigned char* m_pBufForSaveImage1=nullptr;
unsigned char* m_pBufForSaveImage2=nullptr;
//int exposureTime=3500;
