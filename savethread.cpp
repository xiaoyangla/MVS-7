#include "savethread.h"
#include "global.h"
#include "imagebuffer.h"

SaveThread::SaveThread()
{
    m_bSaveThread=false;
    savePath="D:/qt-pro/qt-MVS/MVS-7/images/";
    saveCount=0;
}

void SaveThread::run()
{
    std::cout<<"saveThread is running"<<endl;
//    while(!isInterruptionRequested()){
    //use callback function much better than use while
    while(m_bSaveThread){
//        imageSaveMutex.lock();
//         if (!m_bSaveThread)
//         {
////             return;
//             break;
//         }
//        qDebug("please wait...");
//        imageBuffer2->rawQueueProtect.lock();
        //        if (imageBuffer2->rawQueue.empty()) { continue; }
        currentSizeOfBuffer=imageBuffer2->getSizeOfImageBuffer();
        std::cout<<"Current Size Of Buffer before get for save: "<<currentSizeOfBuffer<<endl;
        saveImage();
//        msleep(5);
//        imageSaveMutex.unlock();

        //    m_nBufSizeForSaveImage1=currentFrame1.second->nWidth* currentFrame1.second->nHeight*3+2048;
        //    m_pBufForSaveImage1 =(unsigned char*)malloc(m_nBufSizeForSaveImage1);
//        imageBuffer2->rawQueueProtect.unlock();
    }


}

void SaveThread::saveImage()
{

//    imageSaveMutex.lock();
//    imageBuffer2->rawQueueProtect.lock();
    currentFrame1=imageBuffer2->getFrame();
    std::cout<<"Get frame for Save FrameNum:"<<currentFrame1.second->nFrameNum<<endl;
//    m_nBufSizeForSaveImage1=currentFrame1.second->nWidth* currentFrame1.second->nHeight*3+2048;
//    m_pBufForSaveImage1 =(unsigned char*)malloc(m_nBufSizeForSaveImage1);

    MV_SAVE_IMAGE_PARAM_EX stParam = { 0 };

    stParam.enImageType = MV_Image_Jpeg; // ch:需要保存的图像类型 | en:Image format to save;
    stParam.enPixelType = currentFrame1.second->enPixelType;  // 相机对应的像素格式 | en:Pixel format
    stParam.nBufferSize = m_nBufSizeForSaveImage;  // 存储节点的大小 | en:Buffer node size
    stParam.nWidth = currentFrame1.second->nWidth;         // 相机对应的宽 | en:Width
    stParam.nHeight = currentFrame1.second->nHeight;          // 相机对应的高 | en:Height
    stParam.nDataLen = currentFrame1.second->nFrameLen;
    stParam.pData = currentFrame1.first;
    stParam.pImageBuffer = m_pBufForSaveImage2;
    stParam.nJpgQuality = 90;       // ch:jpg编码，仅在保存Jpg图像时有效。保存BMP时SDK内忽略该参数

    int nRet = MV_CC_SaveImageEx2(m_handle, &stParam);
    if (MV_OK != nRet)
    {
        std::cout<<"MV_CC_SaveImageEx2 fail:"<<nRet<<endl;
    }

    char chImageName[128] = { 0 };

    if (MV_Image_Bmp == stParam.enImageType)
    {
        sprintf_s(chImageName, 128,"%s %01d.bmp",savePath.toStdString().c_str(),currentFrame1.second->nFrameNum);//sprintf_s(filename,"%s %s %s %s",path1,filepath,path3,path4);
    }
    else if (MV_Image_Jpeg == stParam.enImageType)
    {
//        sprintf_s(chImageName, 128, "D:\\qt-pro\\qt-MVS\\MVS-7\\images\\%01d.jpeg", saveCount);
       sprintf_s(chImageName, 128, "%s %01d.jpeg",savePath.toStdString().c_str(),saveCount);
    }

    FILE* fp = fopen(chImageName, "wb");
    fwrite(m_pBufForSaveImage2, 1,  currentFrame1.second->nFrameLen, fp);
//    saveCount=currentFrame1.second->nFrameNum;
    std::cout<<"Save Finish FrameNum:"<<currentFrame1.second->nFrameNum<<endl;
    fclose(fp);
    emit saveImageFinish();
    saveCount++;
//    imageBuffer2->rawQueueProtect.unlock();
//    imageSaveMutex.unlock();
    //     free(m_pBufForSaveImage);

//    msleep(2);
    //     free(currentFrame);
}

void SaveThread::stopSaveThread()
{
    //use callback function much better than use while
    while(m_bSaveThread){
        std::cout<<"stoping SaveThread"<<endl;
        if(currentSizeOfBuffer==0){
            m_bSaveThread=false;
//            free(m_pBufForSaveImage2);
//            delete imageBuffer2;
            std::cout<<"SaveThread stoped"<<endl;
            break;//break from whole loop;comtinue, break from current loop,into next loop
        }
        msleep(5);
    }
//    imageProcessMutex.unlock();
    std::cout<<"Stop save Thread finish "<<endl;
    return;

}
