#include "dialog.h"
#include "ui_dialog.h"

#include <QtGui>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    system("echo 1 > /sys/devices/platform/davinci_ctr.2/enable");

    ui->setupUi(this);

    lcd = new CTR_lcd();
    if(lcd)
        lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/dcmotor.bin"));
}

Dialog::~Dialog()
{
    if(lcd)
    {
        delete lcd;
    }

    system("echo 0 > /sys/devices/platform/davinci_ctr.2/dc_motor/enable");

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
    int ivalue;
    switch(e->key())
    {
    case Qt::Key_2:
        ui->radioButton_clock->setChecked(true);
        emit clockwise();
        break;
    case Qt::Key_8:
        ui->radioButton_conter->setChecked(true);
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

void Dialog::on_pushButton_run_clicked()
{
    m_bEnable = !m_bEnable;
    QString cmdline;


    if(m_bEnable)
    {
        //set clockwise
        cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/dc_motor/direction").arg(m_bClockwise?1:0);
        system(cmdline.toAscii());

        //set speed
        cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/dc_motor/duty_cycle").arg(m_nSpeed);
        system(cmdline.toAscii());

        //enabled
        cmdline = QString::fromUtf8("echo 1 > /sys/devices/platform/davinci_ctr.2/dc_motor/enable");
        system(cmdline.toAscii());

        ui->pushButton_run->setText(QString::fromUtf8("Stop"));
    }
    else
    {
        //disenabled
        cmdline = QString::fromUtf8("echo 0 > /sys/devices/platform/davinci_ctr.2/dc_motor/enable");
        system(cmdline.toAscii());

        ui->pushButton_run->setText(QString::fromUtf8("Run"));
    }
}

void Dialog::on_radioButton_clock_clicked()
{
    QString cmdline;

    m_bClockwise = false;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/dc_motor/direction").arg(m_bClockwise?1:0);
    system(cmdline.toAscii());

}

void Dialog::on_radioButton_conter_clicked()
{
    QString cmdline;

    m_bClockwise = true;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/dc_motor/direction").arg(m_bClockwise?1:0);
    system(cmdline.toAscii());
}

void Dialog::on_horizontalSlider_speed_valueChanged( int value)
{
    QString cmdline;

    m_nSpeed = value;
    cmdline = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/dc_motor/duty_cycle").arg(m_nSpeed);
    system(cmdline.toAscii());

}

void Dialog::init()
{
    QString dcMotorCtrlPath = QString::fromUtf8("/sys/devices/platform/davinci_ctr.2/dc_motor/");

    //direction
    QFile *directionFile = new QFile();
    directionFile->setFileName(dcMotorCtrlPath + QString::fromUtf8("direction"));
    if(!directionFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete directionFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }
    QString direction = directionFile->readLine(0);
    directionFile->close();
    delete directionFile;

    if(direction.at(0) == QChar::fromAscii('1'))
    {
        m_bClockwise = true;
        ui->radioButton_conter->setChecked(true);
    }
    else
    {
        m_bClockwise = false;
        ui->radioButton_clock->setChecked(true);
    }


    //speed
    QFile *speedFile = new QFile();
    speedFile->setFileName(dcMotorCtrlPath + QString::fromUtf8("duty_cycle"));
    if(!speedFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete speedFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }

    QString szSpeed = speedFile->readLine(0);
    speedFile->close();
    delete speedFile;

    ui->horizontalSlider_speed->setRange(20,80);

    m_nSpeed = szSpeed.trimmed().toInt();

    m_nSpeed = m_nSpeed < 20 ?  20 : m_nSpeed;
    m_nSpeed = m_nSpeed > 80 ?  80 : m_nSpeed;

    ui->horizontalSlider_speed->setValue(m_nSpeed);


    //enable
    QFile *enableFile = new QFile();
    enableFile->setFileName(dcMotorCtrlPath + QString::fromUtf8("enable"));
    if(!enableFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete enableFile;
        QMessageBox::warning(0,tr("warning"),tr("Open error!"),QMessageBox::Ok);
        return;
    }

    QString dcEnnabled = enableFile->readLine(0);
    enableFile->close();
    delete enableFile;

    if(dcEnnabled.at(0) == QChar::fromAscii('1'))
    {
        m_bEnable = true;
        ui->pushButton_run->setText(QString::fromUtf8("Stop"));
    }
    else
    {
        m_bEnable = false;
        ui->pushButton_run->setText(QString::fromUtf8("Run"));
    }

    QObject::connect(this, SIGNAL(clockwise()), this, SLOT(on_radioButton_clock_clicked()));
    QObject::connect(this, SIGNAL(counterclockwise()), this, SLOT(on_radioButton_conter_clicked()));
    QObject::connect(this, SIGNAL(speedChange(int)), this, SLOT(on_horizontalSlider_speed_valueChanged(int)));

    if(!m_bEnable)
        QTimer::singleShot(0, this, SLOT(on_pushButton_run_clicked()));
}
