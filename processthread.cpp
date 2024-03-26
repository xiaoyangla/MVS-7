#include "processthread.h"
#include "global.h"
#include "imagebuffer.h"
ProcessThread::ProcessThread()
{
   imageProcessFinishNum=0;
   matToQImageNum=0;
   ConvertNum=0;
   m_bProcessing=false;
   currentSizeOfBuffer=0;
//   savePath="D:/qt-pro/qt-MVS/MVS-7/images";
//   m_pBufForSaveImage=nullptr;
//   m_pBufForSaveImage1=nullptr;
}

ProcessThread::~ProcessThread()
{

//    qDeleteAll(image);
//    qDeleteAll(currentFrame);

}

bool ProcessThread::isColor(MvGvspPixelType enType)
{
    switch(enType)
        {
        case PixelType_Gvsp_BGR8_Packed:
        case PixelType_Gvsp_YUV422_Packed:
        case PixelType_Gvsp_YUV422_YUYV_Packed:
        case PixelType_Gvsp_BayerGR8:
        case PixelType_Gvsp_BayerRG8:
        case PixelType_Gvsp_BayerGB8:
        case PixelType_Gvsp_BayerBG8:
        case PixelType_Gvsp_BayerGB10:
        case PixelType_Gvsp_BayerGB10_Packed:
        case PixelType_Gvsp_BayerBG10:
        case PixelType_Gvsp_BayerBG10_Packed:
        case PixelType_Gvsp_BayerRG10:
        case PixelType_Gvsp_BayerRG10_Packed:
        case PixelType_Gvsp_BayerGR10:
        case PixelType_Gvsp_BayerGR10_Packed:
        case PixelType_Gvsp_BayerGB12:
        case PixelType_Gvsp_BayerGB12_Packed:
        case PixelType_Gvsp_BayerBG12:
        case PixelType_Gvsp_BayerBG12_Packed:
        case PixelType_Gvsp_BayerRG12:
        case PixelType_Gvsp_BayerRG12_Packed:
        case PixelType_Gvsp_BayerGR12:
        case PixelType_Gvsp_BayerGR12_Packed:
            return true;
        default:
            return false;
    }
}

bool ProcessThread::isMono(MvGvspPixelType enType)
{
    switch(enType)
        {
        case PixelType_Gvsp_Mono10:
        case PixelType_Gvsp_Mono10_Packed:
        case PixelType_Gvsp_Mono12:
        case PixelType_Gvsp_Mono12_Packed:
            return true;
        default:
            return false;
        }
}

Mat ProcessThread::convertPixelType()
//Mat ProcessThread::convertPixelType(void *m_handle)
{

  std::cout<<"start convertPixelType"<<endl;
//  QPair<unsigned char *, MV_FRAME_OUT_INFO_EX *> currentFrame;

  currentFrame=imageBuffer->getFrame();
  Mat getImage;

  MvGvspPixelType enDstPixelType = PixelType_Gvsp_Undefined;
  unsigned int nChannelNum = 0;

  if (isColor(currentFrame.second->enPixelType))
             {
                 nChannelNum = 3;
                 enDstPixelType = PixelType_Gvsp_RGB8_Packed;
                 printf("AfterConvert.rgb\n");
             }
             //If it's Mono, converted to Mono8
             else if (isMono(currentFrame.second->enPixelType))
             {
                 nChannelNum = 1;
                 enDstPixelType = PixelType_Gvsp_Mono8;
                 printf("AfterConvert.gray\n");
             }
             else
             {
                 printf("Don't need to convert!\n");
             }

           printf("enDstPixelType[%d]\n",enDstPixelType);


//           m_nBufSizeForSaveImage=currentFrame.second->nWidth* currentFrame.second->nHeight*nChannelNum+2048;
//           m_pBufForSaveImage =(unsigned char*)malloc(m_nBufSizeForSaveImage);

//         unsigned char* m_pBufForSaveImage =(unsigned char*)(m_nBufSizeForSaveImage);

//           imageBuffer2->addFrame(currentFrame.first,currentFrame.second);
           emit gotImage();

//    bool isMono;
//    switch (m_stImageInfo->enPixelType)
//    {
//    case PixelType_Gvsp_Mono8:
//    case PixelType_Gvsp_Mono10:
//    case PixelType_Gvsp_Mono10_Packed:
//    case PixelType_Gvsp_Mono12:
//    case PixelType_Gvsp_Mono12_Packed:
//        isMono = true;
//        std::cout<<"isMono = true"<<endl;
//        break;
//    default:
//        isMono = false;
//        std::cout<<"isMono = false"<<endl;
//        break;
//    }
           if (0)
           {
               std::cout<<"isMono = true"<<endl;
               getImage = cv::Mat(currentFrame.second->nHeight, currentFrame.second->nWidth, CV_8UC1,currentFrame.first);
               std::cout<<"unsigned char to mat"<<endl;
               //imwrite("d:\\测试opencv_Mono.tif", image);
           }
           else
           {
               //转换图像格式为BGR8
               std::cout<<"Start Convert Images To BGR8"<<endl;
               MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
               memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
               stConvertParam.nWidth = currentFrame.second->nWidth;                 //ch:图像宽 | en:image width
               stConvertParam.nHeight = currentFrame.second->nHeight;               //ch:图像高 | en:image height
               //stConvertParam.pSrcData = m_pBufForDriver;                  //ch:输入数据缓存 | en:input data buffer
               stConvertParam.pSrcData = currentFrame.first;                  //ch:输入数据缓存 | en:input data buffer
               stConvertParam.nSrcDataLen = currentFrame.second->nFrameLen;         //ch:输入数据大小 | en:input data size
               stConvertParam.enSrcPixelType = currentFrame.second->enPixelType;    //ch:输入像素格式 | en:input pixel format
               stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed; //ch:输出像素格式 | en:output pixel format  适用于OPENCV的图像格式
               //stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;   //ch:输出像素格式 | en:output pixel format
               stConvertParam.pDstBuffer = m_pBufForSaveImage1;                    //ch:输出数据缓存 | en:output data buffer
               stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;            //ch:输出缓存大小 | en:output buffer size
               std::cout<<"MV_CC_ConvertPixelType"<<endl;
               int nRet = MV_CC_ConvertPixelType(m_handle, &stConvertParam);
               if (MV_OK != nRet)
               {
                   std::cout<<"MV_CC_ConvertPixelType fail:"<<nRet<<endl;
               }
               std::cout<<"Convert images Pixel Type finish"<<endl;
               getImage = cv::Mat(currentFrame.second->nHeight,currentFrame.second->nWidth,CV_8UC3, m_pBufForSaveImage1);
               std::cout<<"Convert to Qimage"<<endl;
           }
           std::cout<<"Converted Qimage:"<<ConvertNum<<endl;
           ConvertNum++;
           return getImage;

}

QImage ProcessThread::matToQImage(Mat mat)
{
    std::cout<<"start matToQImage"<<endl;
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        std::cout<<"ERROR: Mat could not be converted to QImage."<<endl;
        return QImage();
    }

    std::cout<<"matToQImageNum:"<<matToQImageNum<<endl;
    matToQImageNum++;
}


void ProcessThread::getFrameRate()
{

    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = MV_CC_GetFloatValue(m_handle,"ResultingFrameRate", &stFloatValue);
    if (MV_OK != nRet)
    {
        printf("Get Frame Rate Fail! nRet [0x%x]\n", nRet);
    }
    m_frameRateEdit = stFloatValue.fCurValue;
    emit gotFrameFinish();

}

void ProcessThread::run()
{
//    m_bProcessing=true;
    while(m_bProcessing)// use callback function much better
    {
        std::cout<<"Start Process Thread "<<endl;
        getFrameRate();
        Mat convertFrame = convertPixelType();

        image = matToQImage(convertFrame);

        emit imageProcessfinish();

        std::cout<<"Image Process finish: "<<imageProcessFinishNum<<endl;
        imageProcessFinishNum++;

        currentSizeOfBuffer=imageBuffer->getSizeOfImageBuffer();
        std::cout<<"Current Size Of Buffer for Processing: "<<currentSizeOfBuffer<<endl;
        QApplication::processEvents();
        msleep(5);
    }
}

void ProcessThread::stopProcessThread()
{
//    imageProcessMutex.lock();
    //use callback function much better than use while
    while(m_bProcessing){
        std::cout<<"stoping ProcessThread"<<endl;
        if(currentSizeOfBuffer==0){
            m_bProcessing=false;
//            free(m_pBufForSaveImage1);
//            delete imageBuffer;
            std::cout<<"ProcessThread stoped"<<endl;
            break;//break from whole loop;comtinue, break from current loop,into next loop
        }
        QApplication::processEvents();//后加的，没有测试
        msleep(5);
    }
//    imageProcessMutex.unlock();
    std::cout<<"Stop Process Thread finish "<<endl;
    return;

}

void ProcessThread::saveImage()
{

}
