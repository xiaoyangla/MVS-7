#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("mvs");
    return a.exec();//多线程可以在主线程运行时，新建一个子线程运行。主线程可以不等子线程执行完继续执行下去
}
