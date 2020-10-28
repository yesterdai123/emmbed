#include "mainwindow.h"

#include "paintwaveform.h"
#include "lcdpainter.h"

#include <QTimer>
#include <QFile>
#include <QThread>
#include <QApplication>

class MainWindowPrivate
{
public:
    MainWindowPrivate(MainWindow * parent):
            q_ptr( parent )
    {}

    //  Members
    MainWindow * const q_ptr;
    Q_DECLARE_PUBLIC(MainWindow);

public:
    QPaintWaveform  *paintWaveform;
    QTimer          *adTimer;
    unsigned int     toggle;
    int              frameSize;
    int              timeInterval;
    int             *points[5];
    int              curIndex;
    QString          portFileName;
    QFile           *portFile;

    CTR_lcd          *lcd;
    LCDPainter        lcdPainter;
    QThread           drawThread;

};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      d_ptr(new MainWindowPrivate(this))
{
    Q_D(MainWindow);

    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    //初始化私有数据
    d->toggle = 0;
    d->frameSize = 200;
    d->timeInterval = 1;
    for(unsigned int i = 0 ; i < (sizeof(d->points)/sizeof(d->points[0])) ; i++)
        d->points[i] = new int[d->frameSize];
    d->curIndex = 0;

    d->paintWaveform = new QPaintWaveform(this);
    setCentralWidget(d->paintWaveform);
    //建立信号/槽，异步响应
    connect(this, SIGNAL(newDatas(const int *, int )), d->paintWaveform, SLOT(addDatas(const int *, int )), Qt::QueuedConnection);
    connect(this, SIGNAL(watchSizeChange(int)), d->paintWaveform, SLOT(setPointsSize(int)));
    //设定监视的点数
    emit watchSizeChange(500);

    d->lcd = new CTR_lcd();
    d->lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/ad.bin"));
    d->lcdPainter.setLcd(d->lcd);
    d->lcdPainter.setPointsSize(500);
    d->lcdPainter.connect(this, SIGNAL(newDatas(const int *, int )), SLOT(addDatas(const int *, int)), Qt::QueuedConnection);
    d->lcdPainter.connect(this, SIGNAL(refreshCTRlcd()), SLOT(drawWave()));

    d->lcdPainter.moveToThread(&d->drawThread);
    d->drawThread.connect(this, SIGNAL(stopRefreshCTRlcd()), SLOT(quit()));
    d->lcdPainter.connect(&d->drawThread, SIGNAL(started()), SLOT(drawWave()));

    //设定Timer函数采样
    d->adTimer = new QTimer(this);
    connect(d->adTimer, SIGNAL(timeout()), this, SLOT(getADInputDatas()));

    d->drawThread.start();
    QTimer::singleShot(1000, this, SLOT(drawWave()));
}

MainWindow::~MainWindow()
{
    Q_D(MainWindow);

    emit stopRefreshCTRlcd();

    if(d->adTimer)
    {
       d->adTimer->stop();
       delete d->adTimer;
    }

    for(unsigned int i = 0 ; i < (sizeof(d->points)/sizeof(d->points[0])) ; i++)
        delete d->points[i];

    if(d->lcd)
    {
        delete d->lcd;
    }

    if(d->paintWaveform)
    {
        delete d->paintWaveform;
    }

    if(d->portFile)
    {
        if(d->portFile->isOpen())
            d->portFile->close();
        delete d->portFile;
    }

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/enable");

}

void MainWindow::keyPressEvent ( QKeyEvent * e )
{
    int ivalue;
    switch(e->key())
    {
     case Qt::Key_9:
         QApplication::quit();
        break;

    }
}

void MainWindow::drawWave()
{
    Q_D(MainWindow);
    emit refreshCTRlcd();
    QTimer::singleShot(1000, this, SLOT(drawWave()));
}

void MainWindow::getADInputDatas()
{
    Q_D(MainWindow);
    if(!d->portFile->open(QIODevice::ReadOnly))
        return;

    QString adValue = d->portFile->readLine();
    if(d->curIndex == d->frameSize)
    {
        emit newDatas(d->points[d->toggle++], d->frameSize);
        if(d->toggle >= (sizeof(d->points)/sizeof(d->points[0])))
        {
            d->toggle = 0;
        }
        d->curIndex = 0;
    }

    d->points[d->toggle][d->curIndex++] = adValue.toInt();
    d->portFile->close();
}

void MainWindow::setPortFileName(QString name)
{
     Q_D(MainWindow);
     d->portFileName = name;
}

void MainWindow::startReadData()
{
    Q_D(MainWindow);
    d->portFile = new QFile(d->portFileName);
    d->adTimer->start(d->timeInterval);
}
