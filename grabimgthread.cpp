#include "grabimgthread.h"
#include "imagebuffer.h"
#include "global.h"

//QMutex GrabImgThread::workingFrameQueueProtect;
/*
std::string GetTimeAsFileName()
{
    SYSTEMTIME st = { 0 };
    GetLocalTime(&st);  //获取当前时间 可精确到ms
    char filename[100] = { 0 };

    sprintf(filename, "pictures_src/%d%02d%02d_%02d%02d%02d%03d.bmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return filename;
}
*/

void  GrabImgThread::getCameraImageCallback(unsigned char *pData, MV_FRAME_OUT_INFO_EX *stImageInfo, void *handle)
{

//    workingFrameQueueProtect.lock();
//    sleep(50);
//    stoppedMutex.lock();//测试是否停止的过程要加锁
//             if (0)
//             {
//                 stopped=false;
//                 stoppedMutex.unlock();
//             }
//             stoppedMutex.unlock();
    if (stImageInfo)
    {
        printf("Grabed One Frame: Width[%d], Height[%d], GrabedFrameNum[%d]\n",
               stImageInfo->nWidth, stImageInfo->nHeight, stImageInfo->nFrameNum);


        int nRet= MV_CC_ClearImageBuffer(m_handle);
        if (nRet != MV_OK)
        {
            printf("Clear Image Buffer fail! nRet [0x%x]\n", nRet);

        }

    }
//    std::cout<<"getCameraImageCallback suceess:"<<stImageInfo->nFrameNum<<endl;
//    unsigned char *mpData;
//    memcpy(mpData,pData,m_nBufSizeForSaveImage);

//    exHardwareTrigger();
    imageBuffer->addFrame(pData,stImageInfo);
    imageBuffer2->addFrame(pData,stImageInfo);
//    workingFrameQueueProtect.unlock();
//        msleep(30);

//    if(triggerMode){
//        exHardwareTrigger();
//    }

}
/*
unsigned int GrabImgThread::getCameraImageCallback1(void *pUser)
{

    int nRet = MV_OK;
    /*
    unsigned int g_nPayloadSize = 0;
    MVCC_INTVALUE stParam = { 0 };
     MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
     nRet = MV_CC_GetIntValue(pUser, "PayloadSize", &stParam);
     if (MV_OK != nRet)
     {
         printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);

     }

     g_nPayloadSize = stParam.nCurValue;
     unsigned int nDataSize = g_nPayloadSize;
     unsigned char* pData = (unsigned char*)malloc(sizeof(unsigned char) * (g_nPayloadSize));
     std::cout<<"start getCameraImageCallback "<<endl;
     while(true){ nRet = MV_CC_GetOneFrameTimeout(pUser, pData, nDataSize, &stImageInfo, 1000);
      if (nRet == MV_OK)
      {
          printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
              stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
      }
      else printf("No data[0x%x]\n", nRet);}
      printf("finish [0x%x]\n", nRet);
      */
//      if (pData == NULL)
//      {
//          return 0;
//      }

//    MV_FRAME_OUT stOutFrame = {0};
/*
    MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
    for(int i = 0; ; i++)
    {
        std::cout<<" while(true) "<<endl;

        nRet = MV_CC_GetOneFrameTimeout(pUser, m_pBufForSaveImage1, m_nBufSizeForSaveImage, &stImageInfo, 1000);
        //           nRet = MV_CC_GetImageBuffer(pUser, &stOutFrame, 500);
        if (nRet == MV_OK)
        {
           nRet= MV_CC_ClearImageBuffer(pUser);
           if (nRet != MV_OK)
           {
               printf("Clear Image Buffer fail! nRet [0x%x]\n", nRet);

           }
           unsigned int triggerSource=2;
           MVCC_ENUMVALUE struEnumValue = {triggerSource};
           nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
           std::cout<< "get TriggerSource:"<<struEnumValue.nCurValue<<endl ;

           if(2 == struEnumValue.nCurValue)
           {
//               msleep(5);
               triggerSource=7;
       //        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
               nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource",triggerSource);
               if(nRet != 0){
                   std::cout<< "Set TriggerSource Fail "<<endl ;
               }


               for(int j=0;i<5;i++){

                   int comdValue= MV_CC_SetCommandValue(m_handle, "TriggerSoftware");//int tempValue = MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");
                   //      comdValue = m_handle->CommandExecute("TriggerSoftware");
                   if(comdValue != 0){std::cout<<"emit software trigger once fail"<<endl;}
                   else std::cout<<"emit software trigger Num:"<<j<<endl;

//                   QThread::msleep(5);
               }
               //qDebug() << image;
               //emit doneFinsh(image);
               nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
               if(nRet != 0){std::cout<<"get trigger Source fail"<<endl;}
               std::cout<< "TriggerSource:"<<struEnumValue.nCurValue<<endl ;   //打印当前触发源
           }
           if(7 == struEnumValue.nCurValue){
//               QThread::msleep(5);
               //            nRet = MV_CC_StopGrabbing(m_handle);//关闭采集
               //            if(nRet != 0){qDebug() << "StopGrabbing 失败";}
               //设置硬触发模式
               //        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
               triggerSource=2;
       //        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
               nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource", triggerSource);//0:Line0 1:Line1 2:Line2  7:Software 8:FrequencyConverter 13：anyway
               if(nRet != 0){ std::cout<<"softwareTrigger to hardwareTrigger fail"<<endl;}

               nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
               std::cout<< "Current TriggerSource:"<<struEnumValue.nCurValue<<endl ;
           }

            printf("Get Image Buffer: Width[%d], Height[%d], FrameNum[%d]\n",
                   stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
            //                   unsigned char *mpData;
            //                   memcpy(mpData,stOutFrame.pBufAddr,m_nBufSizeForSaveImage);
            imageBuffer->addFrame(m_pBufForSaveImage1,stImageInfo);
            imageBuffer2->addFrame(m_pBufForSaveImage1,stImageInfo);
        }
        else
        {
            printf("No data[0x%x]\n", nRet);
        }
        if(bExit)
        {
            break;
        }
    }
    return 0;

}
*/


GrabImgThread::GrabImgThread()
{
//    i=NULL;
    exposureTimeNum=3500;
    m_bOpenDevice=false;
    m_bGrabbing=false;
    m_isColor=false;
    m_bTriggerMode=false;
    burstFrameNum=7;
    m_triggerSource=2;
    m_bSoftTriggerOnce=false;


//    triggerMode=0;

}

GrabImgThread::~GrabImgThread()
{

}

void GrabImgThread::eventCallBack(unsigned int nExternalEventId, void *pUser)
{
     std::cout<< "start eventCallBack"<<endl ;
    if (nExternalEventId)
    {
        int ret = MV_CC_RegisterImageCallBackEx(m_handle,GrabImgThread::getCameraImageCallback,m_handle);
        if(MV_OK != ret)
            std::cout<< "MV_CC_RegisterImageCallBackEx Fail!"<<endl ;
//        exHardwareTrigger();
    }

}

//unsigned char* GrabImgThread::m_pBufForSaveImage=NULL;//静态成员需要定义
//unsigned int GrabImgThread::m_nBufSizeForSaveImage=NULL;
//Mat* GrabImgThread::getImage=NULL;
//unsigned char* GrabImgThread::m_pData=NULL;
//MV_FRAME_OUT_INFO_EX * GrabImgThread::m_stImageInfo=NULL;


void GrabImgThread::initCamera()
{
    std::cout<<"access initCamera"<<endl;
    int nRet = -1;

        MV_CC_DEVICE_INFO_LIST stDeviceList;
        memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

        // 枚举相机设备
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
        if (MV_OK != nRet)
        {
            std::cout<<"MV_CC_EnumDevices fail!"<<endl;

        }

        unsigned int nIndex = 0;
        MV_CC_DEVICE_INFO* pDeviceInfo;
        if (stDeviceList.nDeviceNum > 0)
        {
            for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
            {
                std::cout<<"device id: "<<i<<endl;
                pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (NULL == pDeviceInfo)
                {
                    break;
                }
                if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
                {
                    int nIp1 = ((stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
                    int nIp2 = ((stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
                    int nIp3 = ((stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
                    int nIp4 = (stDeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

                    char strUserName[256] = {0};
                    char strDisplayName[256] = {0};
                    if (std::strcmp("", (const char*)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
                    {
                        memcpy(strUserName, pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName,
                            sizeof(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName));
                    }
                    else
                    {
                        sprintf(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                            pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                            pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                    }
                    sprintf(strDisplayName, "[%d]GigE:    %s  (%d.%d.%d.%d)",i,
                        strUserName, nIp1, nIp2, nIp3, nIp4);
                    //AnsiString strDeviceName =  strDisplayName;
                    QString strDeviceName =  strDisplayName;
                    strList.append(strDeviceName);
//                    ui.deviceList->addItem(strDeviceName, NULL);
                }

                else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
                {
                    char strUserName[256] = {0};
                    char strDisplayName[256] = {0};
                    if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
                    {
                        memcpy(strUserName, pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName,
                            sizeof(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName));
                    }
                    else
                    {
                        sprintf(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                            pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                            pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                    }

                    sprintf(strDisplayName, "[%d]UsbV3:  %s", i, strUserName);
                    //AnsiString strDeviceName =  strDisplayName;
                    QString strDeviceName =  strDisplayName;
                    strList.append(strDeviceName);
//                    ui.deviceList->addItem(strDeviceName, NULL);
                }
            }
        }
        else
        {
            std::cout<<"Find No Devices!"<<endl;
            messge="Find No Devices!";

            emit statusBarMessge();
            return;
        }


        pDeviceInfo = stDeviceList.pDeviceInfo[0];//selected camera index
        //打印相机型号及序列号
//        qDebug() << QString::fromLocal8Bit(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chModelName));
//        qDebug() << QString::fromLocal8Bit(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber));

        //彩色相机判别
        if('C' == pDeviceInfo->SpecialInfo.stGigEInfo.chModelName[12])
        {
            m_isColor = true;
           std::cout<<"Camera is color!"<<endl;
        }
        else
        {
            m_isColor = false;
            std::cout<<"Camera is uncolor!"<<endl;
        }

        // 选择当前设备创建句柄
        nRet = MV_CC_CreateHandle(&m_handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            std::cout<<"MV_CC_CreateHandle fail!"<<endl;

        }

        // 打开设备
        nRet = MV_CC_OpenDevice(m_handle);
        if (MV_OK != nRet)
        {
            std::cout<<"MV_CC_OpenDevice fail!"<<endl;
            emit statusBarMessge();

        }
        else {
            std::cout<<"Open Device Success!"<<endl;
            m_bOpenDevice=true;
        }
//        emit changeMode();
        setTriggerMode();
        setExposureTime();

        MVCC_INTVALUE stParam = { 0 };
        nRet = MV_CC_GetIntValue(m_handle, "PayloadSize", &stParam);
        if (MV_OK != nRet)
        {
            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
        }
        unsigned int g_nPayloadSize = stParam.nCurValue;
        unsigned int m_nBufSizeForSaveImage1 = 3072*2048*3+2048;
        if(g_nPayloadSize==m_nBufSizeForSaveImage1){
            std::cout<<"true"<<endl;
            m_nBufSizeForSaveImage = g_nPayloadSize;}
        else {
            m_nBufSizeForSaveImage = m_nBufSizeForSaveImage1;
            std::cout<<"false"<<endl;}

        m_pBufForSaveImage1 = (unsigned char*)malloc(sizeof(unsigned char) * (m_nBufSizeForSaveImage));
        m_pBufForSaveImage2 = (unsigned char*)malloc(sizeof(unsigned char) * (m_nBufSizeForSaveImage));
        //获取图像的宽度和高度
//         MVCC_INTVALUE ptValue;
     //    int w,h;
/*         MV_CC_GetWidth(m_handle, &ptValue);
         unsigned int imageWidth = ptValue.nCurValue;
         MV_CC_GetHeight(m_handle, &ptValue);
         unsigned int imageHeight = ptValue.nCurValue;
         m_nBufSizeForSaveImage = imageWidth*imageHeight*3+2048;
//         m_nBufSizeForSaveImage = 3072*2048*3+2048;

         m_pBufForSaveImage1 = (unsigned char *)malloc(m_nBufSizeForSaveImage);
         m_pBufForSaveImage2 = (unsigned char *)malloc(m_nBufSizeForSaveImage);
*/





//        //获取图像的宽度和高度
//        MVCC_INTVALUE ptValue;
//    //    int w,h;
//        MV_CC_GetWidth(m_handle, &ptValue);
//        m_imageWidth = ptValue.nCurValue;
//        MV_CC_GetHeight(m_handle, &ptValue);
//        m_imageHeight = ptValue.nCurValue;

//        if(m_isColor)
//            nDataSize = m_imageWidth * m_imageHeight * 3;
//        else
//            nDataSize = m_imageWidth * m_imageHeight;

        //设置硬触发模式
//        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
//        nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource", 0);


//        m_pData = (unsigned char *)malloc(nDataSize); //获取一帧图像的内存大小



}

void GrabImgThread::setExposureTime()
{
    std::cout<< "set Exposure Time..."<<endl ;
    int ret = MV_CC_SetEnumValue(m_handle, "ExposureMode",0);
    //   ret = MV_CC_SetFloatValue(m_handle, "ExposureTime",exposureTimeNum );
    //    ret = MV_CC_SetExposureTime(m_handle, 4000.f);
    ret = MV_CC_SetExposureTime(m_handle, exposureTimeNum);
    if(MV_OK != ret){
        std::cout<< "set Exposure Time Fail!"<<endl ;
    }
}

void GrabImgThread::setTriggerMode()
{
    int nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", m_bTriggerMode);
    if(MV_OK != nRet)
        std::cout<< "set Trigger Mode Fail!"<<endl ;

    if(m_bTriggerMode){
        std::cout<< "Trigger Mode true"<<endl ;
        if(m_triggerSource==2){
            //        int nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
            //        if(MV_OK != nRet)
            //            std::cout<< "set Trigger Mode Fail!"<<endl ;

            nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource", m_triggerSource);
            if(nRet != MV_OK)
            {

                printf("Warning: Set TriggerSource fail nRet [0x%x]!", nRet);
            }
            nRet = MV_CC_SetEnumValue(m_handle, "LineSelector", 0);//0:Line0  2:Line2
            //        if (MV_OK != nRet)
            //        {
            //            printf("LineSelector fail! nRet [0x%x]\n", nRet);
            //        }


            //        nRet = MV_CC_SetIntValue(m_handle,"AcquisitionBurstFrameCount",5);
            nRet=MV_CC_SetBurstFrameCount(m_handle,burstFrameNum);
            if(nRet != MV_OK)
            {
                printf("Warning: Set SetBurstFrameCount fail nRet [0x%x]!", nRet);
            }

            //设置相机帧率，需注意不要超过相机支持的最大的帧率（相机规格书），超过了也没有意义

            //帧率控制使能，true表示打开，false标识关闭
            //        nRet = MV_CC_SetBoolValue(m_handle, "AcquisitionFrameRateEnable", true);
            //        if(nRet != MV_OK)
            //        {

            //            printf("Warning: Set AcquisitionFrameRateEnable fail nRet [0x%x]!", nRet);
            //        }

            //        nRet =MV_CC_SetFloatValue(m_handle, "AcquisitionFrameRate", 30);
            //        if(nRet != MV_OK)
            //        {

            //            printf("Warning: Set AcquisitionFrameRate fail nRet [0x%x]!", nRet);
            //        }

            nRet = MV_CC_SetEnumValue(m_handle, "AcquisitionMode", 2);////0:SingleFrame 1:MultiFrame 2:Continuous
            if (MV_OK != nRet)
            {

                printf("AcquisitionMode fail! nRet [0x%x]\n", nRet);
            }

            nRet = MV_CC_SetEnumValue(m_handle, "TriggerActivation", 0);//0：Rising Edge  1：Falling Edge 2：LevelHigh 3：LevelLow
            if (MV_OK != nRet)
            {

                printf("TriggerActivation fail! nRet [0x%x]\n", nRet);
            }

            nRet = MV_CC_SetFloatValue(m_handle,"TriggerDelay",0);//根据实际情况设置，默认0 us
            if (MV_OK != nRet)
            {

                printf("TriggerDelay fail! nRet [0x%x]\n", nRet);
            }

            nRet = MV_CC_SetBoolValue(m_handle,"TriggerCacheEnable",false);//开启后会默认缓存1个信号
            if (MV_OK != nRet)
            {

                printf("TriggerCacheEnable fail! nRet [0x%x]\n", nRet);
            }
            //滤波设置
            nRet = MV_CC_SetIntValue(m_handle,"LineDebouncerTime",100);//硬件滤波时间，可加大此参数防抖us
            if (MV_OK != nRet)
            {
                printf("LineDebouncerTime fail! nRet [0x%x]\n", nRet);
            }

            std::cout<< " Trigger Mode on!"<<endl ;
        }
        else{
            nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource",m_triggerSource);
            if (MV_OK != nRet)
            {
                printf("Set SoftTrigger fail! nRet [0x%x]\n", nRet);
            }
        }
    }

    else{
        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 0);
        if(MV_OK != nRet)
            std::cout<< "set Trigger Mode Fail!"<<endl ;
    }
    std::cout<< "set Trigger Mode Success"<<endl ;

}

void GrabImgThread::closeDevice()
{
    // ch:关闭设备 | Close device
//    std::cout<< "Close Device now..."<<endl ;
    int nRet = MV_CC_CloseDevice(m_handle);
    if (MV_OK != nRet){
        printf("ClosDevice Fail! nRet [0x%x]\n", nRet);
    }
    else{
        printf("ClosDevice Success! nRet [0x%x]\n", nRet);
        m_bOpenDevice=false;
    }
    // ch:销毁句柄 | Destroy handle
    nRet = MV_CC_DestroyHandle(m_handle);
    if (MV_OK != nRet)
    {
        printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
    }
}


void GrabImgThread::exHardwareTrigger()
{

    //设置为硬触发
    std::cout<< "start exHardwareTrigger"<<endl ;
    unsigned int triggerSource=2;
    MVCC_ENUMVALUE struEnumValue = {triggerSource};
    int nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
    std::cout<< "get TriggerSource:"<<struEnumValue.nCurValue<<endl ;
/*
    //必须停止采集状态下设置，一般在MV_CC_StartGrabbing接口之前调用
    //开启相机图像水印，如下，打开了Exposure、BrightnessInfo、Framecounter
    nRet = MV_CC_SetEnumValue(m_handle, "FrameSpecInfoSelector", 2);//Exposure
    nRet = MV_CC_SetBoolValue(m_handle, "FrameSpecInfo", true);
    nRet = MV_CC_SetEnumValue(m_handle, "FrameSpecInfoSelector", 3);//BrightnessInfo
    nRet = MV_CC_SetBoolValue(m_handle, "FrameSpecInfo", true);
    nRet = MV_CC_SetEnumValue(m_handle, "FrameSpecInfoSelector", 5);//Framecounter
    nRet = MV_CC_SetBoolValue(m_handle, "FrameSpecInfo", true);
    nRet = MV_CC_SetEnumValue(m_handle, "FrameSpecInfoSelector", 6);//ExtTriggerCount
    nRet = MV_CC_SetBoolValue(m_handle, "FrameSpecInfo", true);
    if (MV_OK != nRet)
    {

        printf("Set FrameSpecInfofail! nRet [0x%x]\n", nRet);
    }
    //    nRet = MV_CC_SetEnumValue(m_handle, "FrameSpecInfoSelector", 6);//Framecounter
    //    if(nRet != 0){
    //        std::cout<< "Set ExtTriggerCount Fail "<<endl ;
    //    }
    nRet = MV_CC_SetEnumValue(m_handle, "AcquisitionMode", 2);////0:SingleFrame 1:MultiFrame 2:Continuous
    if (MV_OK != nRet)
    {

        printf("AcquisitionMode fail! nRet [0x%x]\n", nRet);
    }

    nRet = MV_CC_SetEnumValue(m_handle, "TriggerActivation", 0);//0：Rising Edge  1：Falling Edge 2：LevelHigh 3：LevelLow
    if (MV_OK != nRet)
    {

        printf("TriggerActivation fail! nRet [0x%x]\n", nRet);
    }

    nRet = MV_CC_SetFloatValue(m_handle,"TriggerDelay",0);//根据实际情况设置，默认0 us
    if (MV_OK != nRet)
    {

        printf("TriggerDelay fail! nRet [0x%x]\n", nRet);
    }
    */
//    unsigned int triggerSource2=7;
//    MVCC_ENUMVALUE struEnumValue2 = {triggerSource2};
    if(2 == struEnumValue.nCurValue)
    {
        msleep(5);
        triggerSource=7;
//        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
        nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource",triggerSource);
        if(nRet != 0){
            std::cout<< "Set TriggerSource Fail "<<endl ;
        }


        for(int i=0;i<5;i++){

            int comdValue= MV_CC_SetCommandValue(m_handle, "TriggerSoftware");//int tempValue = MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");
            //      comdValue = m_handle->CommandExecute("TriggerSoftware");
            if(comdValue != 0){std::cout<<"emit software trigger once fail"<<endl;}
            else std::cout<<"emit software trigger Num:"<<i<<endl;
            QThread::msleep(5);
        }
        //qDebug() << image;
        //emit doneFinsh(image);
        nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
        if(nRet != 0){std::cout<<"get trigger Source fail"<<endl;}
        std::cout<< "TriggerSource:"<<struEnumValue.nCurValue<<endl ;   //打印当前触发源
    }
    if(7 == struEnumValue.nCurValue){
        QThread::msleep(5);
        //            nRet = MV_CC_StopGrabbing(m_handle);//关闭采集
        //            if(nRet != 0){qDebug() << "StopGrabbing 失败";}
        //设置硬触发模式
        //        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
        triggerSource=2;
//        nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 1);
        nRet = MV_CC_SetEnumValue(m_handle, "TriggerSource", triggerSource);//0:Line0 1:Line1 2:Line2  7:Software 8:FrequencyConverter 13：anyway
        if(nRet != 0){ std::cout<<"softwareTrigger to hardwareTrigger fail"<<endl;}

        nRet = MV_CC_GetEnumValue(m_handle, "TriggerSource", &struEnumValue);
        std::cout<< "Current TriggerSource:"<<struEnumValue.nCurValue<<endl ;
    }
}

void GrabImgThread::softTriggerOnce()
{
    int nRet= MV_CC_SetCommandValue(m_handle, "TriggerSoftware");
    if(MV_OK != nRet)
        std::cout<< "SoftTrigger Once Fail!"<<endl;
}

void GrabImgThread::run()
{

    //step 1： 初始化相机
    //       initCamera();


    //    int nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 0);

    //获取Enum型参数:获取触发模式
    setExposureTime();
    MVCC_ENUMVALUE struEnumValue = {m_bTriggerMode};
    int nRet = MV_CC_GetEnumValue(m_handle, "TriggerMode", &struEnumValue);
    std::cout<< "TriggerMode:"<<struEnumValue.nCurValue<<endl ;
    //判断是否为硬触发
    if(0 == struEnumValue.nCurValue){

        //step2： 注册取图回调函数

        if(m_isColor){
            int ret = MV_CC_RegisterImageCallBackForRGB(m_handle, GrabImgThread::getCameraImageCallback,m_handle);
            if(MV_OK != ret)
                std::cout<< "MV_CC_RegisterImageCallBackForRGB Fail!"<<endl ;
        }
        else{
            int ret = MV_CC_RegisterImageCallBackEx(m_handle,GrabImgThread::getCameraImageCallback,m_handle);
            if(MV_OK != ret)
                std::cout<< "MV_CC_RegisterImageCallBackEx Fail!"<<endl ;
        }

        //step 3: 开始取图

        nRet =MV_CC_StartGrabbing(m_handle);
        if (MV_OK == nRet)
        {
            printf("StartGrabbing Success! nRet [0x%x]\n", nRet);
            m_bGrabbing=true;

        }
        else m_bGrabbing=false;
        std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
        nRet= MV_CC_ClearImageBuffer(m_handle);
        if (nRet != MV_OK)
        {
            printf("Clear Image Buffer fail! nRet [0x%x]\n", nRet);

        }
 /*
        std::cout<< "star get image buffer!"<<endl ;
        MV_FRAME_OUT stOutFrame = {0};
        nRet = MV_CC_GetImageBuffer(m_handle, &stOutFrame, 500);
        if(nRet != MV_OK)
        {
            printf("Get Image Buffer fail! nRet [0x%x]\n", nRet);
        }
         std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
 */
       /*
        unsigned int nThreadID = 0;
        void* hThreadHandle = (void*) _beginthreadex(NULL, 0, GrabImgThread::getCameraImageCallback1, m_handle, 0, &nThreadID );
        if (NULL == hThreadHandle)
        {

          std::cout<< "hThreadHandle = null"<<endl ;
        }

        std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
        std::cout<< "_beginthreadex id:"<<nThreadID<<endl ;
*/
//    msleep(2);
    }
    else{

        if(m_isColor){
            int ret = MV_CC_RegisterImageCallBackForRGB(m_handle, GrabImgThread::getCameraImageCallback,m_handle);
            if(MV_OK != ret)
                std::cout<< "MV_CC_RegisterImageCallBackForRGB Fail!"<<endl ;
        }
        else{
            int ret = MV_CC_RegisterImageCallBackEx(m_handle,GrabImgThread::getCameraImageCallback,m_handle);
            if(MV_OK != ret)
                std::cout<< "MV_CC_RegisterImageCallBackEx Fail!"<<endl ;

        }

//        if(triggerMode){
//            exHardwareTrigger();
//        }

        // Register event callback
//        nRet = MV_CC_RegisterEventCallBack(m_handle, GrabImgThread::eventCallBack, m_handle);
//        if (MV_OK != nRet)
//        {
//            printf("Register Event CallBack fail! nRet [0x%x]\n", nRet);
//        }


        nRet =MV_CC_StartGrabbing(m_handle);
        if (MV_OK == nRet)
        {
            printf("StartGrabbing Success! nRet [0x%x]\n", nRet);
            m_bGrabbing=true;

        }
        else m_bGrabbing=false;
        std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
        nRet= MV_CC_ClearImageBuffer(m_handle);
        if (nRet != MV_OK)
        {
            printf("Clear Image Buffer fail! nRet [0x%x]\n", nRet);

        }
/*
        unsigned int nThreadID = 0;
        void* hThreadHandle = (void*) _beginthreadex(NULL, 0, GrabImgThread::getCameraImageCallback1, m_handle, 0, &nThreadID );
        if (NULL == hThreadHandle)
        {

          std::cout<< "hThreadHandle = null"<<endl ;
        }

        std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
        std::cout<< "_beginthreadex id:"<<nThreadID<<endl ;
*/
//        nRet = MV_CC_SetCommandValue(m_handle, "TriggerSoftware");//相机拍照
//        if (MV_OK != nRet)
//        {

//            printf("TriggerSoftware fail! nRet [0x%x]\n", nRet);
//        }
/*
        unsigned int nThreadID = 0;
        void* hThreadHandle = (void*) _beginthreadex( NULL , 0 , GrabImgThread::getCameraImageCallback1, m_handle, 0 , &nThreadID );
        if (NULL == hThreadHandle)
        {

          std::cout<< "hThreadHandle = null"<<endl ;
        }


        std::cout<< "GrabImgThread id:"<<QThread::currentThread()<<endl ;
        while(MV_OK ==nRet){

        }

*/
//        msleep(30);
    }
}


void GrabImgThread::stopGrabImg()
{

    //    stoppedMutex.lock();//测试是否停止的过程要加锁
//    if (0)
//    {
//        stopped=false;
//        stoppedMutex.unlock();
//    }
    grabImgMutex.lock();
    if (false == m_bOpenDevice || false == m_bGrabbing || nullptr == m_handle)
      {
          return;
      }
    if(m_bGrabbing){
        printf("Stop Grabbing...\n");

        int nRet =MV_CC_StopGrabbing(m_handle);
        if (MV_OK != nRet)
        {
            printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
           m_bGrabbing=false;
        }
        else {
            printf("Stop Grabbing Success! nRet [0x%x]\n", nRet);
        }
    }
grabImgMutex.unlock();
//        stoppedMutex.unlock();
//    imageBuffer->rawQueue.clear();

}
