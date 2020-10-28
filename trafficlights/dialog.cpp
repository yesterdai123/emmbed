#include "dialog.h"
#include "ui_dialog.h"
#include <QtGui>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
//    注销掉此行就可以连续定时timer->setSingleShot(true)
    connect(timer, SIGNAL(timeout()), this, SLOT(testLights()));
    connect(this, SIGNAL(toggleLeds(QLeds *)), this, SLOT(trafficled_light_clicked(QLeds *)));

    indexToggle = 0;
    isRunning = false;

    lcd = new CTR_lcd();
    if(lcd)
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/traffic.bin"));

    QTimer::singleShot(0, this, SLOT(on_pushButton_run_clicked()));

}

Dialog::~Dialog()
{
    if(lcd)
    {
        delete lcd;
    }

    timer->stop();

    delete timer;

    delete ui;
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
    int ivalue;
    switch(e->key())
    {
     case Qt::Key_9:
         QApplication::quit();
        break;

    }
}

void Dialog::trafficled_light_clicked(QLeds *led)
{
    if(led)
    {
        QString cmdline;
        QString ledName = led->objectName() ;
        QString ctrlFileName = QString::fromUtf8("/sys/devices/platform/davinci_ctr.2/traffic_lights/") + ledName.left(ledName.lastIndexOf(QString::fromUtf8(".")));

        QFile *ctrlFile = new QFile();
        ctrlFile->setFileName(ctrlFileName);
        if(!ctrlFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            delete ctrlFile;
            QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
            return;
        }
        QString oldState = ctrlFile->readLine(0);
        QString newState = oldState;
        int index;
        if(ledName.right(1) == QString::fromUtf8("r"))
        {
            index = 0;
        }
        else if(ledName.right(1) == QString::fromUtf8("y"))
        {
            index = 2;
        }
        else
        {
            index = 4;
        }

        if(newState.at(index) == QChar::fromAscii('1'))
        {
            newState[index] = QChar::fromAscii('0');
        }
        else
        {
            newState[index] = QChar::fromAscii('1');
        }

        if(newState != oldState)
        {
            ctrlFile->reset();
            if(-1 != ctrlFile->write(newState.toAscii()))
                led->toggle();
        }
        ctrlFile->close();

        delete ctrlFile;
    }
}

void Dialog::Mytraffic()
{
    QList<QLeds *> allQleds=findChildren<QLeds *>();

}

void Dialog::testLights()
{
    QList<QLeds *> allQleds = findChildren<QLeds *>();
    if(indexToggle >= allQleds.size())
        indexToggle = 0;

    toggleLeds(allQleds.at(indexToggle++));
}

void  Dialog::on_pushButton_run_clicked()
{

    if(!isRunning)
    {
         timer->start(500);
         ui->pushButton_run->setText(QString::fromUtf8("Stop"));
    }
    else
    {
         timer->stop();
         ui->pushButton_run->setText(QString::fromUtf8("Run"));
    }

    isRunning = !isRunning;
}
