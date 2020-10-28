#include "dialog.h"
#include "ui_dialog.h"

#include <QFile>
#include <QKeyEvent>
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    ui->setupUi(this);

    flag = none;

    ui->toolButton_square->setIconSize(QSize(96,96));
    ui->toolButton_sine->setIconSize(QSize(96,96));
    ui->toolButton_white->setIconSize(QSize(96,96));

    ui->toolButton_square->setStyleSheet(QString("QToolButton{border-image:url(:/da/images/images-square.png)}"
                                                 "QToolButton:pressed{border-image:url(:/da/images/images-square.png)}"));
    ui->toolButton_sine->setStyleSheet(QString("QToolButton{border-image:url(:/da/images/images-sine.png)}"
                                                 "QToolButton:pressed{border-image:url(:/da/images/images-sine.png)}"));
    ui->toolButton_white->setStyleSheet(QString("QToolButton{border-image:url(:/da/images/images-white.png)}"
                                                "QToolButton:pressed{border-image:url(:/da/images/images-white.png)}"));

    timeLine = new QTimeLine(500, this);
    if(timeLine)
    {
        timeLine->setFrameRange(1, 50);
        timeLine->setLoopCount(0);
        timeLine->setUpdateInterval(1);

        connect(timeLine, SIGNAL(valueChanged(qreal)), this, SLOT(valueChange(qreal)));
        connect(timeLine, SIGNAL(frameChanged ( int )), this, SLOT(frameChanged ( int )));
    }


    lcd = new CTR_lcd();
    if(lcd)
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/da.bin"));

    connect(this, SIGNAL(squareSinal()), SLOT(on_toolButton_square_clicked()));
    connect(this, SIGNAL(sineSinal()), SLOT(on_toolButton_sine_clicked()));
    connect(this, SIGNAL(whiteSinal()), SLOT(on_toolButton_white_clicked()));

}

Dialog::~Dialog()
{
    if(lcd)
    {
        delete lcd;
    }

    delete timeLine;

    delete ui;

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/enable");

}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::keyPressEvent ( QKeyEvent * e )
{
    switch(e->key())
    {
    case Qt::Key_1:
        emit squareSinal();
        break;
    case Qt::Key_2:
        emit sineSinal();
        break;
    case Qt::Key_3:
        emit whiteSinal();
        break;
    case Qt::Key_9:
         QApplication::quit();
        break;
    }
}

void Dialog::frameChanged ( int frame )
{
    int data;
    switch(flag)
    {
    case square:
        if(frame & 0x01)
            data = 0x00FF;
        else
            data = 0x0000;
        outData(data);
        break;
    case sine:
    case white:
    case none:
    default:
        break;
    }
}

void Dialog::valueChange(qreal value)
{
    int data;
    switch(flag)
    {
    case sine:
        data = value * 0x00FF;
        outData(data);
        break;
    case white:
        data = qrand() % 0x00FF;
        outData(data);
        break;
    case square:
    case none:
    default:
        break;
    }
}

void Dialog::setPortFileName(QString name)
{
    portName = name;

    if(timeLine)
        timeLine->start();

     QTimer::singleShot(0, this, SLOT(on_toolButton_square_clicked()));
}

void Dialog::on_toolButton_square_clicked()
{
    if(timeLine)
    {
        timeLine->setPaused(true);
        flag = square;
        timeLine->setCurveShape(QTimeLine::LinearCurve);
        timeLine->setPaused(false);
    }
}

void Dialog::on_toolButton_sine_clicked()
{
    if(timeLine)
    {
        timeLine->setPaused(true);
        flag = sine;
        timeLine->setCurveShape(QTimeLine::SineCurve);
        timeLine->setPaused(false);
    }
}

void Dialog::on_toolButton_white_clicked()
{
    if(timeLine)
    {
        timeLine->setPaused(true);
        flag = white;
        timeLine->setPaused(false);
    }
}

void Dialog::outData(int data)
{
    unsigned char value = data;

    QString cmdLine = QString::fromUtf8("echo %1 > %2").arg(value).arg(portName);
    system(cmdLine.toAscii());
}
