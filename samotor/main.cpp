#include <QtGui/QApplication>
#include "mainwindow.h"

#include <sys/signal.h>

void sig_handler(int sig)            //信号处理函数
{

    if(sig == SIGINT)
    {
        QApplication::quit();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    signal(SIGINT, sig_handler);

    w.init();
    w.show();

    return a.exec();
}
