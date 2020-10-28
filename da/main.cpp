#include <QtGui/QApplication>
#include "dialog.h"

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
    Dialog w;

    signal(SIGINT, sig_handler);

    w.setPortFileName(QString(argv[1]));
    w.show();

    return a.exec();
}
