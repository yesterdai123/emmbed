#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    ui->setupUi(this);

    lcd = new CTR_lcd();
    if(lcd)
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/samotor.bin"));
}

MainWindow::~MainWindow()
{
    if(lcd)
    {
        delete lcd;
    }

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/sa_motor/enable");

    delete ui;

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/enable");

}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent ( QKeyEvent * e )
{
    int ivalue;
    switch(e->key())
    {
    case Qt::Key_2:
        ui->radioButton_clock->setChecked(true);
        emit clockwise();
        break;
    case Qt::Key_8:
        ui->radioButton_counterclock->setChecked(true);
        emit counterclockwise();
        break;
    case Qt::Key_4:
        ivalue = ui->horizontalSlider_speed->value();
        if(ivalue > ui->horizontalSlider_speed->minimum())
            ivalue--;
        ui->horizontalSlider_speed->setValue(ivalue);
        break;
    case Qt::Key_6:
        ivalue = ui->horizontalSlider_speed->value();
        if(ivalue < ui->horizontalSlider_speed->maximum())
            ivalue++;
        ui->horizontalSlider_speed->setValue(ivalue);
        break;
    case Qt::Key_9:
         QApplication::quit();
        break;

    }
}

void MainWindow::init()
{
    QString saMotorCtrlPath = QString::fromUtf8("/sys/devices/platform/davinci_ctr.2/sa_motor/");

    //direction
    QFile *directionlFile = new QFile();
    directionlFile->setFileName(saMotorCtrlPath + QString::fromUtf8("direction"));
    if(!directionlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete directionlFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }
    QString direction = directionlFile->readLine(0);
    directionlFile->close();
    delete directionlFile;

    if(direction.at(0) == QChar::fromAscii('1'))
    {
        m_bClockwise = true;
        ui->radioButton_counterclock->setChecked(true);
    }
    else
    {
        m_bClockwise = false;
        ui->radioButton_clock->setChecked(true);
    }


    //speed
    QFile *speedFile = new QFile();
    speedFile->setFileName(saMotorCtrlPath + QString::fromUtf8("rapid"));
    if(!speedFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete speedFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }

    QString szSpeed = speedFile->readLine(0);
    speedFile->close();
    delete speedFile;

    ui->horizontalSlider_speed->setRange(2,10);

    m_nSpeed = szSpeed.toInt();

    m_nSpeed = m_nSpeed < 2 ?  2 : m_nSpeed;
    m_nSpeed = m_nSpeed > 10 ?  10 : m_nSpeed;

    ui->horizontalSlider_speed->setValue(12 - m_nSpeed);

    //enable
    QFile *enableFile = new QFile();
    enableFile->setFileName(saMotorCtrlPath + QString::fromUtf8("enable"));
    if(!enableFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete enableFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }

    QString saEnnabled = enableFile->readLine(0);
    enableFile->close();
    delete enableFile;

    if(saEnnabled.at(0) == QChar::fromAscii('1'))
    {
        m_bEnable = true;
        ui->toolButton_run->setText(QString::fromUtf8("Stop"));
    }
    else
    {
        m_bEnable = false;
        ui->toolButton_run->setText(QString::fromUtf8("Run"));
    }

    QObject::connect(this, SIGNAL(clockwise()), this, SLOT(on_radioButton_clock_clicked()));
    QObject::connect(this, SIGNAL(counterclockwise()), this, SLOT(on_radioButton_counterclock_clicked()));
    QObject::connect(this, SIGNAL(speedChange(int)), this, SLOT(on_horizontalSlider_speed_valueChanged(int)));

    if(!m_bEnable)
        QTimer::singleShot(0, this, SLOT(on_toolButton_run_clicked()));

}

void MainWindow::on_toolButton_run_clicked()
{
    m_bEnable = !m_bEnable;
    QString cmdline;

    if(m_bEnable)
    {
        ui->dialSACtrlFine->setEnabled(false);
        //set clockwise
        cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction").arg(m_bClockwise?1:0);
        system(cmdline.toAscii());

        //set speed
        cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/rapid").arg(m_nSpeed);
        system(cmdline.toAscii());

        //enabled
        cmdline = QString::fromUtf8("echo 1 > /sys/devices/platform/davinci_ctr.2/sa_motor/enable");
        system(cmdline.toAscii());

        ui->toolButton_run->setText(QString::fromUtf8("Stop"));
    }
    else
    {
        ui->dialSACtrlFine->setEnabled(true);
        //disenabled
        cmdline = QString::fromUtf8("echo 0 > /sys/devices/platform/davinci_ctr.2/sa_motor/enable");
        system(cmdline.toAscii());

        ui->toolButton_run->setText(QString::fromUtf8("Run"));
    }
}

void MainWindow::on_radioButton_clock_clicked()
{
    QString cmdline;

    m_bClockwise = false;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction").arg(m_bClockwise?1:0);
    system(cmdline.toAscii());

}

void MainWindow::on_radioButton_counterclock_clicked()
{
    QString cmdline;

    m_bClockwise = true;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction").arg(m_bClockwise?1:0);
    system(cmdline.toAscii());
}

void MainWindow::on_horizontalSlider_speed_valueChanged( int value)
{
    QString cmdline;

    m_nSpeed = 12 - value;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/rapid").arg(m_nSpeed);
    system(cmdline.toAscii());

}

void MainWindow::on_dialSACtrlFine_sliderPressed()
{
    m_nPreValue = ui->dialSACtrlFine->value();
}

void MainWindow::on_dialSACtrlFine_sliderReleased()
{
    QString cmdline;

    if(m_bClockwise)
    {
        cmdline = QString::fromUtf8("echo 1 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction");
    }
    else
    {
        cmdline = QString::fromUtf8("echo 0 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction");
    }

    system(cmdline.toAscii());

}

void MainWindow::on_dialSACtrlFine_sliderMoved ( int value )
{
    QString cmdline;

    int nDiff = value - m_nPreValue;
    if(nDiff > 0)
    {
        if(nDiff <= 5)
            cmdline = QString::fromUtf8("echo 0 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction");
    }
    else
    {
        if(abs(nDiff) <= 5)
            cmdline = QString::fromUtf8("echo 1 > /sys/devices/platform/davinci_ctr.2/sa_motor/direction");
    }

    system(cmdline.toAscii());
    m_nPreValue = value;

    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/sa_motor/steps").arg(200*abs(nDiff));
    system(cmdline.toAscii());
}
