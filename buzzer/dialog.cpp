#include "dialog.h"
#include "ui_dialog.h"

#include <QtGui>
#define DELAY_TIME_TO_STOP (200)

unsigned int music[][3]=
{
        {3,130,2},{3,130,2},{4,58 ,2},{4,3  ,2},{3,226,2},{3,130,6},{3,130,1},{3,130,1},
        {3,130,2},{4,58 ,2},{4,3  ,2},{3,226,2},{3,172,6},{3,172,2},{3,172,2},{4,83 ,2},
        {4,58 ,2},{4,3  ,2},{3,172,6},{3,172,2},{4,130,2},{4,130,2},{4,83 ,2},{4,3  ,2},
        {4,58 ,4},{3,226,2},{3,130,2},{3,130,2},{4,58 ,2},{4,3  ,2},{4,226,2},{3,130,6},
        {3,130,2},{4,58 ,2},{4,3  ,2},{3,226,2},{3,172,6},{3,172,2},{3,172,2},{4,83,2 },
        {4,58 ,2},{4,3  ,2},{4,130,2},{4,130,2},{4,130,2},{4,130,2},{4,172,2},{4,130,2},
        {4,83 ,2},{4,3  ,2},{3,226,6},{0,0  ,2},{4,58 ,2},{4,58 ,2},{4,58 ,4},{4,58 ,2},
        {4,58 ,2},{4,58 ,4},{4,58 ,2},{4,130,2},{3,226,2},{4,3  ,2},{4,58 ,8},{4,83 ,2},
        {4,83 ,2},{4,83 ,3},{4,83 ,1},{4,83 ,2},{4,58 ,2},{4,58 ,2},{4,58 ,1},{4,58 ,1},
        {4,130,2},{4,130,2},{4,83 ,2},{4,3  ,2},{3,226,4},{0,0  ,4},
};

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    lcd = new CTR_lcd();
    lcd->setShowBitsFile(QString::fromUtf8("/home/root/ctr/data/buzzer.bin"));

    isSounding = false;
    isPlaying = false;
    curMusic = 0;

    soundDelayTimer = new QTimer(this);
    soundDelayTimer->setSingleShot(true);
    connect(soundDelayTimer, SIGNAL(timeout()), this, SLOT(soundTimeOut()));

}

Dialog::~Dialog()
{
    if(isSounding || isPlaying)
    {
        system("echo 0 > /sys/devices/platform/davinci_ctr.2/buzzer/enable");
    }

    if(soundDelayTimer)
    {
       soundDelayTimer->stop();
       delete soundDelayTimer;
    }

    if(lcd)
    {
        delete lcd;
    }

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

/*
音调频率(HZ)=(15625*(2^音阶数))/(511-音调值)

  音符   音调值
  A   3
  A#   31
  B   58
  C   83
  C#   107
  D   130
  D#   151
  E   172
  F   191
  F#   209
  G   226
  G#   242


  音阶         范围(HZ)
  0   31-61
  1   61-122
  2   122-244
  3   244-488
  4   489-977
  5   978-1.95k
  6   1.96-3.91k
  7   3.910-7.81k
*/
/*
  data format: 音阶，音符，图标路径
*/
void Dialog::intKeypadMap(QString fileName)
{
    QFile *initFile = new QFile();
    initFile->setFileName(fileName);
    if(!initFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        delete initFile;
        QMessageBox::warning(0,tr("warning"),tr("Open init file error!"),QMessageBox::Ok);
        return;
    }
    QString readString;
    QStringList list;
    QPushButton *pushButtons[MAX_PATCH_ENABLED];

    pushButtons[0] = ui->pushButton_01;
    pushButtons[1] = ui->pushButton_02;
    pushButtons[2] = ui->pushButton_03;
    pushButtons[3] = ui->pushButton_04;
    pushButtons[4] = ui->pushButton_05;
    pushButtons[5] = ui->pushButton_06;
    pushButtons[6] = ui->pushButton_07;
    pushButtons[7] = ui->pushButton_08;
    pushButtons[8] = ui->pushButton_09;
    pushButtons[9] = ui->pushButton_10;
    pushButtons[10] = ui->pushButton_11;
    pushButtons[11] = ui->pushButton_12;

    long step;
    long pitch;
    unsigned int i = 0;
    while( i < MAX_PATCH_ENABLED)
    {
        readString = initFile->readLine(0);

        if(readString.isEmpty()) break;

        list = readString.split(',');

        step = list.at(0).toLong();
        pitch = list.at(1).toLong();

        if(list.size() >= 3)
        {
            if(!list.at(2).isEmpty())
                pushButtons[i]->setText(list.at(2).trimmed());
        }

        if(list.size() >= 4)
        {
            if(!list.at(3).isEmpty())
            {
                QImage image;
                bool result = image.load(list.at(3).trimmed());
                if(result)
                {
                    QPixmap pixmapToShow = QPixmap::fromImage(image.scaled(64, 64, Qt::KeepAspectRatio) );

                    pushButtons[i]->setIcon(pixmapToShow);
                    pushButtons[i]->setIconSize(pixmapToShow.size());
                }
            }
        }

        m_nfrequency[i++] = (15625*pow(2,step))/(511-pitch);
    }

    initFile->close();

    delete initFile;

    nMusicNumber = sizeof(music)/sizeof(music[0]);
    // 设置合适的音长和音高
    for ( i=0 ; i<nMusicNumber ; i++ )
    {
        if(music[i][1])
           music[i][0]  = 15625 * pow(2,music[i][0]) / (511 - music[i][1]);
        music[i][2]  = music[i][2] * 100;
    }

    isPlaying = true;
    soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::soundTimeOut()
{
    if(isPlaying)
    {
        int now = curMusic++ % nMusicNumber;
        soundDelayTimer->stop();
        system("echo 0 > /sys/devices/platform/davinci_ctr.2/buzzer/enable");
        if(music[now][1])
        {
            QString cmdLine = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/buzzer/frequency").arg(music[now][0]);
            system(cmdLine.toAscii());
        }
        soundDelayTimer->start(music[now][2]);
        if(music[now][1])
            system("echo 1 > /sys/devices/platform/davinci_ctr.2/buzzer/enable");
    }
    else
    {
        system("echo 0 > /sys/devices/platform/davinci_ctr.2/buzzer/enable");
        isSounding = false;
    }
}

void Dialog::sound(int frequency)
{
    soundDelayTimer->stop();
    isPlaying = false;

    if(!isSounding)
    {
        system("echo 1 > /sys/devices/platform/davinci_ctr.2/buzzer/enable");
    }

    QString cmdLine = QString::fromUtf8("echo %1 > /sys/devices/platform/davinci_ctr.2/buzzer/frequency").arg(frequency);
    system(cmdLine.toAscii());
    isSounding = true;
}

void Dialog::on_pushButton_01_pressed()
{
    sound(m_nfrequency[0]);
}

void Dialog::on_pushButton_02_pressed()
{
    sound(m_nfrequency[1]);
}

void Dialog::on_pushButton_03_pressed()
{
    sound(m_nfrequency[2]);
}

void Dialog::on_pushButton_04_pressed()
{
    sound(m_nfrequency[3]);
}

void Dialog::on_pushButton_05_pressed()
{
    sound(m_nfrequency[4]);
}

void Dialog::on_pushButton_06_pressed()
{
    sound(m_nfrequency[5]);
}

void Dialog::on_pushButton_07_pressed()
{
    sound(m_nfrequency[6]);
}

void Dialog::on_pushButton_08_pressed()
{
    sound(m_nfrequency[7]);
}

void Dialog::on_pushButton_09_pressed()
{
    sound(m_nfrequency[8]);
}

void Dialog::on_pushButton_10_pressed()
{
    sound(m_nfrequency[9]);
}

void Dialog::on_pushButton_11_pressed()
{
    sound(m_nfrequency[10]);
}

void Dialog::on_pushButton_12_pressed()
{
    sound(m_nfrequency[11]);
}

void Dialog::on_pushButton_01_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_02_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_03_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}
void Dialog::on_pushButton_04_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_05_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_06_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_07_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_08_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_09_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_10_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_11_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}

void Dialog::on_pushButton_12_released()
{
     soundDelayTimer->start(DELAY_TIME_TO_STOP);
}
