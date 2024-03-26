#include "imagebuffer.h"

ImageBuffer::ImageBuffer(int size)
{
    printf( "Semaphore2 initializations\n");
    // Semaphore initializations
    freeSlots = new QSemaphore(size);//缓冲区中空闲位置的个数
    std::cout<<"QSemaphore Size: "<<size<<endl;
    usedSlots = new QSemaphore(0);//缓冲区中已使用位置的个数
//    clearBuffer1 = new QSemaphore(1);//用来锁定向缓冲区中添加帧的操作，即同时只能有一个进程向缓冲区中添加帧
//    clearBuffer2 = new QSemaphore(1);//用来锁定从缓冲区中拿走帧的操作，即同时只能有一个进程从缓冲区中拿走帧
    dropFrame=true;
    // Save value of dropFrame to private member
}

ImageBuffer::~ImageBuffer()
{
  rawQueue.clear();
//  qDeleteAll(rawQueue);
}

ImageBuffer::ImageBuffer(int size, bool dropFrame)
{
    printf( "Semaphore initializations\n");
    // Semaphore initializations
    freeSlots = new QSemaphore(size);//缓冲区中空闲位置的个数
    std::cout<<"QSemaphore Size: "<<size<<endl;
    usedSlots = new QSemaphore(0);//缓冲区中已使用位置的个数
//    clearBuffer1 = new QSemaphore(1);//用来锁定向缓冲区中添加帧的操作，即同时只能有一个进程向缓冲区中添加帧
//    clearBuffer2 = new QSemaphore(1);//用来锁定从缓冲区中拿走帧的操作，即同时只能有一个进程从缓冲区中拿走帧
    // Save value of dropFrame to private member
    this->dropFrame=dropFrame;//是否允许丢帧的标志
//     getFrameNum=0;
//     addFrameNum=0;
}

void ImageBuffer::addFrame(unsigned char* pData,MV_FRAME_OUT_INFO_EX* stImageInfo)
{
    // Acquire semaphore
    printf( "start addFrame.\n");

//    clearBuffer1->acquire();
    QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* > qPair;
    qPair.first=pData;
    qPair.second=stImageInfo;
    // If frame dropping is enabled, do not block if buffer is full
    if(dropFrame)//若允许丢帧
    {
        //空闲位置为0时，只是不把帧放到缓冲区中，而不阻塞进程
        // Try and acquire semaphore to add frame
        if(freeSlots->tryAcquire())
        {
            //若空闲位置不为0，将帧添加到缓冲区，添加过程要加锁
            // Add frame to queue
            rawQueueProtect.lock();

            rawQueue.enqueue(qPair);//添加到队尾
            rawQueueProtect.unlock();
            // Release semaphore
            usedSlots->release();//使已使用位置加1
        }
    }
    // If buffer is full, wait on semaphore
    else//若不允许丢帧
    {

        // Acquire semaphore
        freeSlots->acquire();//使空闲位置减1，若空闲位置为0，则阻塞抓帧进程//
        // Add frame to queue
        rawQueueProtect.lock();
        rawQueue.enqueue(qPair);
        rawQueueProtect.unlock();
        // Release semaphore
        usedSlots->release();//使已使用位置加1
    }
    // Release semaphore
//    clearBuffer1->release();
//    addFrameNum++;
    GrabedFrameNum=qPair.second->nFrameNum;
//    emit GrabedFrameFinish();
    printf( "addFrame Num:[%d]\n",GrabedFrameNum);
}

QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* > ImageBuffer::getFrame()
{
//    printf( "start getFrame.\n");
    // Acquire semaphores
//    clearBuffer2->acquire();
       usedSlots->acquire();//使已使用位置减1，若为0则阻塞进程
    // Temporary data
//    if(!GrabImgThread::rawQueue.empty()){

        QPair<unsigned char *, MV_FRAME_OUT_INFO_EX* >  tempFrame;
        // Take frame from queue
        rawQueueProtect.lock();
        tempFrame=rawQueue.dequeue();//从队头取出帧
        rawQueueProtect.unlock();
        // Release semaphores
        freeSlots->release();//使空闲位置加1
//        clearBuffer2->release();

//        getFrameNum++;
        printf( "Get Frame Num:[%d]\n",tempFrame.second->nFrameNum);
        // Return frame to caller
        return tempFrame;//返回深拷贝
//    }

}

void ImageBuffer::clearBuffer()
{
    //若当前缓冲区不为空
    // Check if buffer is not empty
    if(rawQueue.size()!=0)
    {
        // Stop adding frames to buffer
//        clearBuffer1->acquire();//锁定向缓冲区中添加帧的操作
        // Stop taking frames from buffer
//        clearBuffer2->acquire();//锁定从缓冲区中拿走帧的操作
        // Release all remaining slots in queue
        freeSlots->release(rawQueue.size());//使空闲位置变满
        // Acquire all queue slots
        freeSlots->acquire(bufferSize);//
        // Reset usedSlots to zero
        usedSlots->acquire(rawQueue.size());
        // Clear buffer
        rawQueue.clear();
        // Release all slots
        freeSlots->release(bufferSize);//
        // Allow getFrame() to resume
//        clearBuffer2->release();
        // Allow addFrame() to resume
//        clearBuffer1->release();
        printf( "Image buffer successfully cleared.\n");
    }
    else
        printf("WARNING: Could not clear image buffer: already empty.\n") ;
}

int ImageBuffer::getSizeOfImageBuffer()
{
  return rawQueue.size();
}
