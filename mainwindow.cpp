#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "global.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->btnConCam->setEnabled(true);
    ui->btnCloseDev->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(false);
    ui->btnSoftTriggerOnce->setEnabled(false);
    ui->triSouComBox->setEnabled(false);
    uiDisplayImageNum=0;
    m_bClose=true;
    ui->triSouComBox->addItem("Hard Trigger");
    ui->triSouComBox->addItem("Soft Trigger");


    m_Image=new ImageWidget;
    qgraphicsScene= new QGraphicsScene;
    grabImageThread = new GrabImgThread;
    processThread = new ProcessThread;
    saveThread = new SaveThread;

    ui->lineExpo->setText(QString("%1").arg(5000));
    ui->bstFraLineEdit->setText(QString("%1").arg(7));
    ui->savePathLineEdit->setText("D:/qt-pro/qt-MVS/MVS-7/images/");

    connect(ui->triModCheBox, SIGNAL(stateChanged(int)), this, SLOT(OnTriModCbxChanged()));
    connect(ui->triSouComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectTriSou()));


    connect(processThread,SIGNAL(imageProcessfinish()),this,SLOT(recvShowPicSignal()));

    connect(saveThread,SIGNAL(saveImageFinish()),this,SLOT(getGrabedFrameNum()));

//    connect(processThread,SIGNAL(gotImage()),processThread,SLOT(saveImage()));

    connect(grabImageThread,SIGNAL(statusBarMessge()),this,SLOT(showMessage()));
    connect(processThread,SIGNAL(gotFrameFinish()),this,SLOT(getFrameRateDis()));

    connect(ui->btnConCam,SIGNAL(clicked()),this,SLOT(connectCamera()));
    connect(ui->lineExpo,SIGNAL(editingFinished()),this,SLOT(setExposureTime()));
    connect(ui->bstFraLineEdit,SIGNAL(editingFinished()),this,SLOT(setBurstFrameNum()));
    connect(ui->btnSoftTriggerOnce,SIGNAL(clicked()),this,SLOT(SoftTriggerCrt()));
    connect(ui->btnStart,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->btnStop,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->btnSavePath, SIGNAL(clicked()), this, SLOT(getSavePath()));
    connect(ui->saveCheckBox, SIGNAL(stateChanged(int)), this, SLOT(OnSaveCbxChanged()));
//    connect(grabImageThread,SIGNAL(changeMode()),this,SLOT(OnHtrCbxChanged()));

    connect(ui->btnCloseDev,SIGNAL(clicked()),this,SLOT(close()));//this指针指向被调用的成员函数所属的对象

//    connect(grabImgThread1,SIGNAL(doneFinsh()),this,SLOT(uiDisplayImage()));


}

MainWindow::~MainWindow()
{
    delete grabImageThread;
    delete processThread;
    delete saveThread;
    delete imageBuffer;
    delete imageBuffer2;
//    free(m_pBufForSaveImage1);
//    free(m_pBufForSaveImage2);
    delete ui;
}

void MainWindow::showMessage()
{
   ui->statusbar->showMessage(grabImageThread->messge);
}

void MainWindow::recvShowPicSignal()
{
//    QPixmap ConvertPixmap=QPixmap::fromImage(processThread->image);//The QPixmap class is an off-screen image representation that can be used as a paint device
//    QGraphicsScene  *qgraphicsScene = new QGraphicsScene;//要用QGraphicsView就必须要有QGraphicsScene搭配着用
//    m_Image = new ImageWidget(&ConvertPixmap);//实例化类ImageWidget的对象m_Image，该类继承自QGraphicsItem，是自己写的类
    std::cout<<"DisplayImage..."<<endl;
    m_Image->m_pix=QPixmap::fromImage(processThread->image);
    int nwith = ui->imageGraphic->width();//获取界面控件Graphics View的宽度
    int nheight = ui->imageGraphic->height();//获取界面控件Graphics View的高度
    m_Image->setQGraphicsViewWH(nwith,nheight);//将界面控件Graphics View的width和height传进类m_Image中
    qgraphicsScene->addItem(m_Image);//将QGraphicsItem类对象放进QGraphicsScene中
//    qgraphicsScene->addPixmap(m_Image->m_pix);
    ui->imageGraphic->setSceneRect(QRectF(-(nwith/2),-(nheight/2),nwith,nheight));//使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大），防止图片放大后重新缩小的时候视窗太大而不方便观察图片
    ui->imageGraphic->setScene(qgraphicsScene);//Sets the current scene to scene. If scene is already being viewed, this function does nothing.
    ui->imageGraphic->setFocus();//将界面的焦点设置到当前Graphics View控件
    std::cout<<"finish uiDisplayImage:"<<uiDisplayImageNum<<endl;
    uiDisplayImageNum++;
}


void MainWindow::uiDisplayImage()
{
//    ui->label_img->setPixmap(QPixmap::fromImage(processThread->image).scaled(840,560));
    std::cout<<"finish uiDisplayImage:"<<uiDisplayImageNum<<endl;
    uiDisplayImageNum++;
}

int MainWindow::OnSaveCbxChanged()
{
    if(ui->saveCheckBox->isChecked())
    {
        //            ui->saveCheckBox->setEnabled(true);
        std::cout<<"AutoSave is true"<<endl;
        saveThread->m_bSaveThread=true;
        saveThread->start(QThread :: LowestPriority);

    }
    else
    {
        //            ui->saveCheckBox->setEnabled(false);
        std::cout<<"AutoSave is false"<<endl;
        saveThread->m_bSaveThread=false;
        //            saveThread->requestInterruption();
        //            saveThread->quit();
        //            saveThread->wait();
        std::cout<<"saveThread stop"<<endl;
        //            saveThread->wait();
        //          disconnect(processThread,SIGNAL(gotImage()),processThread,SLOT(saveImage()));
    }
    return 0;

}

void MainWindow::SoftTriggerCrt()
{
    grabImageThread->m_bSoftTriggerOnce=true;
    std::cout<<"Soft Trigger Once True"<<endl;
    grabImageThread->softTriggerOnce();
}


void MainWindow::setBurstFrameNum()
{
    QString str1 = ui->bstFraLineEdit->text();
    grabImageThread->burstFrameNum = str1.toInt();
}

void MainWindow::setExposureTime()
{
    QString str1 = ui->lineExpo->text();
    grabImageThread->exposureTimeNum = str1.toFloat();
}

void MainWindow::getGrabedFrameNum()
{
    int num1=imageBuffer->GrabedFrameNum;
    int num2=uiDisplayImageNum;
    int num3=saveThread->saveCount;
    QString s =  QString::number(num1+1)+"/"+QString::number(num2)+"/"+QString::number(num3);
    ui->lineEditFrame->setText(s);
}



void MainWindow::getFrameRateDis()
{
    char strff[21];
    memset(strff,0,sizeof(strff));

    // 把浮点数ff转换为字符串，存放在strff中。
    sprintf(strff,"%.2f",processThread->m_frameRateEdit);
    ui->lineFrame->setText(strff);
}

void MainWindow::getSavePath()
{
    QString filePath = QFileDialog::getExistingDirectory(this);
    filePath=filePath+"/";
    ui->savePathLineEdit->setText(filePath);
    saveThread->savePath=filePath;
}

int MainWindow::OnTriModCbxChanged()
{
    if(ui->triModCheBox->isChecked())
    {
        std::cout<<"triggerMode is true"<<endl;
        grabImageThread->m_bTriggerMode=true;
        ui->triSouComBox->setEnabled(true);
    }
    else
    {
        std::cout<<"triggerMode is false"<<endl;
        grabImageThread->m_bTriggerMode=false;
        ui->triSouComBox->setEnabled(false);
        ui->btnSoftTriggerOnce->setEnabled(false);
    }
    return 0;


}

void MainWindow::selectTriSou()
{
    int sourceIndex=ui->triSouComBox->currentIndex();
    //    switch(sourceIndex){
    //    case 0:
    //        grabImageThread->m_triggerSource=2;
    //        ui->btnSoftTriggerOnce->setEnabled(false);
    //    case 1:
    //        grabImageThread->m_triggerSource=7;
    //        ui->btnSoftTriggerOnce->setEnabled(true);
    //    }
    if(sourceIndex){
        grabImageThread->m_triggerSource=7;
        ui->btnSoftTriggerOnce->setEnabled(true);}
    else{
        grabImageThread->m_triggerSource=2;
        ui->btnSoftTriggerOnce->setEnabled(false);}
}

void MainWindow::connectCamera()
{
    ui->btnConCam->setEnabled(false);
    ui->boxDeviceList->clear();
    grabImageThread->initCamera();
    ui->boxDeviceList->addItems(grabImageThread->strList);
    if(grabImageThread->m_bOpenDevice){
        ui->btnStart->setEnabled(true);
        ui->triModCheBox->setEnabled(false);
//        ui->hardTriggerCheBox->setEnabled(false);

        ui->btnCloseDev->setEnabled(true);}

}


void MainWindow::start()
{
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    if(grabImageThread->m_bOpenDevice){
        m_bClose=false;
        grabImageThread->start(QThread :: HighestPriority);
        //    saveThread->start(QThread :: LowestPriority);
        //    if(processThread->isRunning()){
        processThread->m_bProcessing=true;
        processThread->start(QThread :: LowestPriority);
    }
}

void MainWindow::stop()
{
    ui->btnStop->setEnabled(false);
    ui->btnStart->setEnabled(true);
//    std::cout<<"starting Stop Process Thread "<<endl;
//   if(grabImageThread->m_bGrabbing){e
//        std::cout<<"Stop GrabImageThread "<<endl;
    grabImageThread->stopGrabImg();
//   }
    processThread->stopProcessThread();

    saveThread->stopSaveThread();
//    if(processThread->isRunning()){
//        std::cout<<"Stop ProcessThread "<<endl;
//        processThread->requestInterruption();
//        processThread->stopProcessThread();
//        processThread->wait();
//    }

}

void MainWindow::close()
{
    QMessageBox message;
    message.setInformativeText("        Do you want to close camera ? ");
    message.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    //    message.setDefaultButton(QMessageBox::Yes);
    int choice=message.exec();
    switch (choice) {
    case QMessageBox::Yes:
//        if(grabImageThread->m_bGrabbing){grabImageThread->stopGrabImg();}
        ui->btnCloseDev->setEnabled(false);
        ui->btnStart->setEnabled(false);
        ui->btnConCam->setEnabled(true);
        ui->btnStop->setEnabled(false);
        grabImageThread->stopGrabImg();

        processThread->stopProcessThread();
    //    free(m_pBufForSaveImage1);
        saveThread->stopSaveThread();
    //    free(m_pBufForSaveImage2);
        grabImageThread->closeDevice();
        m_bClose=true;

        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        break;
    }

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox message;
    message.setInformativeText("        Do you want to exsit ? ");
    message.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    //    message.setDefaultButton(QMessageBox::Yes);
    int choice=message.exec();
    switch (choice) {
    case QMessageBox::Yes:
//        if(grabImageThread->m_bGrabbing){grabImageThread->stopGrabImg();}
        if(m_bClose==false){this->close();}

        qApp->closeAllWindows();
        qApp->quit();
        event->accept();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    case QMessageBox::Cancel:
        event->ignore();
        break;
    }

}

